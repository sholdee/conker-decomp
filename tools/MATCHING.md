# Matching a function (mw workflow)

Goal: replace one `#pragma GLOBAL_ASM(...)` stub with C that compiles (IDO 5.3, `-O2 -g3`,
mips2) to the EXACT original bytes. The verdict is the whole code bin's sha1 — nothing else.
Commands run from the repository root. `$CONKER_WS` is the workspace directory (default
`~/conker-ws`; see "Paths" in AGENTS.md).

**First, claim the function:** `mkdir -p "$CONKER_WS/.claims" && mkdir "$CONKER_WS/.claims/FUNC"`. If that fails because the
directory exists, another agent owns FUNC: do nothing else and stop at once with status
BLOCKED and note "claimed". (A coordinator running two fleets over one list relies on this.)
Exception — stale claims: if nothing in `$CONKER_WS/FUNC/` (checks.jsonl, the workspace C file, my/)
has changed in the last 30 minutes (`find "$CONKER_WS/FUNC" -newermt "-30 min" | head`), the owner
died (e.g. a usage-limit pause); `touch` the claim dir and continue as the new owner.

## Loop
```
python3 tools/mw.py new FUNC        # private workspace $CONKER_WS/FUNC (main tree is never touched)
#   edit  $CONKER_WS/FUNC/conker/src/<file>.c   — replace the stub line for FUNC with C
#   read  $CONKER_WS/FUNC/target.s (target asm), $CONKER_WS/FUNC/m2c.c (machine draft — types are guesses)
#         $CONKER_WS/FUNC/prev.c if present: an earlier attempt's best C (often close — start from it)
python3 tools/mw.py check FUNC      # ~2s: build + link + sha1; prints RESULT: MATCH | DIFF | BUILD-FAIL | STUB
                                    #   -q = summary only; a link failure still shows the object diff
python3 tools/mw.py land FUNC       # only after RESULT: MATCH — merges into main, re-gates, commits
python3 tools/mw.py batch FUNC a.c b.c …   # try whole-file variants in parallel, one summary line each
python3 tools/mw.py objdump FUNC    # your build's disassembly (with relocations)
python3 tools/mw.py reset FUNC [--all]     # restore the file(s) from the pristine base
python3 tools/mw.py similar FUNC    # matched TWINS (same code modulo symbols/regs/imms) — check FIRST
python3 tools/mw.py note FUNC "…"   # ledger ($CONKER_WS/FUNC/notes.md): record each hypothesis and its result
```
**If `land` prints LAND-FAIL after your check said MATCH**, look at the cfe errors it prints. They are
almost always a declaration that another function landed into the same file after your workspace
was made (a `redeclaration of X`, `Incompatible type`, a typedef that now exists twice). Your MATCH is
real, so do not stop: copy main's current file (`conker/<file>` in the main tree) over your workspace file,
put your function back in place of its GLOBAL_ASM stub, make the declarations agree (prefer the
type the definition uses; keep main's existing declarations if they are compatible), re-run
`check` until MATCH, then `land` again. Never leave a matched function unlanded.

Before writing anything, run `similar`: a matched twin's C is the fastest start (copy it,
adapt names/constants, declare this function's own data). Read `mw.py note FUNC` (the ledger)
when resuming, and append one line per hypothesis family you try (what, result) so the next
agent does not repeat dead ends.
Judge progress by the `ROWS:` line (structural / register-only / immediate counts), not the
asm-differ `CURRENT` score, which is not monotonic. Get structural to 0 first. When only
register-only rows remain and ~15 focused tries make no progress, stop and report it with the
ledger up to date: register allocation residue needs a different approach, not more variants.
`check` output: a `DIFF:` summary line (words equal, size, first mismatch offset) and an
asm-differ table: left = TARGET, right = CURRENT (your build), with `r` = register-only diff,
`i` = immediate diff, `|` = different instruction, `<`/`>` = missing/extra instruction.
The `CURRENT (N)` number is the diff score (lower is closer; 0 with RESULT: DIFF means the
function is right but something ELSE in the bin changed — e.g. a new literal in .rodata).

## Rules
- Edit ONLY files under `$CONKER_WS/FUNC/conker/src/` and (sparingly) `include/`. Prefer
  local `extern`/prototype/struct declarations at the top of your own `.c` file. A shared
  prototype in a header that is provably wrong (the callee's asm shows the real
  param widths) may be corrected there — `land` re-gates the whole ROM, so a change that alters
  any other function is rejected. Never edit asm/, Makefile, yaml, tools.
- Never add a `#pragma GLOBAL_ASM`. Other stubs in the same file stay as they are.
- Keep ALL scratch files (variants, scripts, notes) inside your own workspace, e.g.
  `$CONKER_WS/FUNC/my/`. Never use /tmp or the session scratchpad: other agents share them and
  overwrite each other's files.
- Scripted variant sweeps (`mw.py batch`, generators) are fine, but keep each sweep to a few
  hundred variants; the machine is shared by many agents. Stop and report REGALLOC instead of
  brute-forcing register-only residue.
- C89: all declarations at the top of a block. No `//`-only tricks needed (`-Xcpluscomm` is on).
- Literals are fine: write float/double/string literals naturally (`10000.0f`, `"name"`). The
  build retargets every .rodata reference in your object to the ROM's original address
  (tools/rodata_extern.py), using the original instruction at the same offset. Do NOT replace a
  literal with `extern f32 D_...` — IDO allocates registers differently for a global load.
  Use `extern` only for genuine globals/tables the function indexes or that are not literals.
  Write floats at FULL precision (`0.1290000081f`, not `0.129f`) — recover the decimal from the
  ROM word. `check` prints `LITERALS:` when a literal differs from the original rodata word.
- Statics are fine: a function-local or file-level `static` (e.g. `static u8 D_800900D4;`) links —
  the build retargets references into the object's own .data/.bss to the ROM's original address
  (tools/rodata_extern.py). Name each static `D_<ADDR>` with its original address (read it off the
  target's %hi/%lo); an unnamed one is placed from the original instruction pair only once the
  function matches. Use a static only where the target shows one (e.g. a lui on every access).
- Switches are fine: write a normal `switch`. The build retargets the compiler's jump table to
  the ROM's original table (tools/rodata_extern.py), so case order/density must make IDO
  emit the same table (same first case and length — read them off the target's `sltiu`/`addiu`).
- Do not stop at "close". A function is done only at RESULT: MATCH (then land it).

## Method (works for big functions)
1. Read target.s fully. Note the frame size (`addiu sp,sp,-N`), saved regs, calls, loops
   (backward branches), and the `D_` globals touched.
2. Start from m2c.c, clean it into plausible source (real types, struct field names via
   casts or a local struct typedef, loops instead of gotos). Get it to BUILD first.
3. Fix STRUCTURE before registers: control flow, call order, loads/stores, instruction
   count. Work top-down from the first mismatch; re-check after each edit (it is cheap).
4. Then registers/scheduling: variable declaration order, temporaries, expression
   ordering, `s32` vs `u32`/`s16`, combining/splitting statements. Frame-size diffs: unused
   or extra locals. `check` prints `FRAME:` when the stack size differs. Under `-g3` every named
   scalar local gets its own 4-byte stack slot in declaration order even when it lives in a
   register (`register` does not avoid it); arrays/structs are sized as declared; a float passed
   in a GPR (`mfc1` to a0-a3) or a comparison result passed as a stack argument can reserve
   extra unused words. Use `-U 1000` to see every sp offset unfolded.
5. Diagnostic probes that are deliberately WRONG C (a changed constant, a `? 3 : 5` to see what
   moves) must use `mw.py check FUNC --probe` / `mw.py batch FUNC --probe …` so they are never
   recorded as your best attempt. If a wrong version got recorded anyway, restore your correct
   file and run `mw.py best FUNC --reset`.
6. Your best version is kept automatically: every check that beats your best so far is
   snapshotted to `best/file.c` in the workspace and to the local `.nearmiss/` store (nothing is ever
   lost, and a later attempt starts from it). Still, don't wander away from your best — restore
   from `best/file.c` when a line of attack fails.
7. After one failed sweep of a hypothesis family (e.g. statement orders), switch families
   instead of repeating it; record the sweep in the ledger.

## Diagnosis checklist (check these before reshaping statements)
- **Callee contracts first.** Extra argument homes, odd int<->float moves, or a value kept
  live across a call often mean a callee's PROTOTYPE is wrong (pointer vs int, float vs int,
  width, return type). Read the callee's asm; fix the declaration before tuning expressions.
- **Frame accounting** (when FRAME differs or every sp offset is shifted). The local area is:
  named homes (every named local, in declaration order from the frame top, unused /
  register-only / `register` ones included, natural alignment; block-scoped locals count and
  sibling blocks do not share) + call-arg temps (each argument expression that itself
  contains a call reserves a word: `f(g(x))` one, a 3-deep chain three; the area holds the
  largest statement's count) + spill words + 0/1 alignment word + other uopt temps (a
  variable array index `a[i]` can reserve one where `*(a + i)` does not). Count the target's
  words, then add/remove real locals or split nested calls until the counts agree, THEN try
  declaration orders.
- **Float registers.** IDO's FP temporaries rotate through $f4 $f6 $f8 $f10 $f16 $f18, never
  $f12/$f14 — those mean a register-allocated value. A literal and an `extern f32` load get
  different allocation priority: try the literal (full precision).
- **Real state vs recomputable addresses.** Keep genuine loop state / value snapshots in
  named variables; do not name values IDO recomputes (addresses, `&arr[i]`). Test one storage
  change at a time.
- **Lifetimes explain scheduling.** Neither shorter nor longer scopes are universally better;
  reason from where each value is last used.

## IDO source idioms (m2c shows what IDO emitted, not what was written)
- **Strength-reduced loops:** a counter plus a cursor stepped by the element size
  (`p += 0xC`) is a plain indexed loop over `table[i]` — write that; IDO re-reduces it. A dead
  `or vN,zero,zero` before a pointer-compared loop is the eliminated counter. Index
  fixed-stride arrays with a row view, e.g. `((u8 (*)[0x32C])&D_800CC2D0)[i]`.
- **Pointer-range loops:** a `bne` against an end symbol can be `for (i = 0; i < N; i++)`
  over the blocks before it.
- **Guarded do/while:** `if (c) { do {...} while (c); }` == `while (c) {...}` — prefer `while`.
  Keep a goto loop if the structured form gets unrolled (search loops with an early `return`
  unroll 4x).
- **Compiler unrolling:** a short scalar prologue then a 4-store loop is ONE simple loop,
  remainder peeled first — write the single loop before tuning its schedule.
- **Named pointer in v0:** `lw v0,X(a0)` used as a base then `li v0,1` for the return comes
  from a named pointer local (`data = state->field;`), not a chained expression.
- **Display lists:** `or v1,a0,zero … addiu a0,a0,8 … or v0,a0,zero` is a GBI block on
  `arg0++` returning `arg0` (use the gSP/gDP macros from `include/2.0L/PR/gbi.h`).
- **Block scopes are scheduling barriers (-g3).** Wrapping a run of statements in a new `{ }`
  block — often with its locals declared inside, sometimes with an unused `s32 pad;` — makes -g3
  emit a debug block marker that the assembler won't schedule across. This fixed many "one
  instruction moved up/down" residues (an early `lui`/`lwc1`/reload, a delay-slot fill, float
  zero stores out of order). Try it before reshaping expressions.
- **Look at the compiler's own view.** `cc -K` keeps IDO's intermediate files (e.g. the binasm
  before the assembler's scheduling pass) — compare pre- and post-scheduling order to tell a
  code-generation difference from a scheduling one. Offline compiles don't count as checks.
- **Address-taken functions schedule differently.** IDO's assembler (as1) only hoists a switch's
  jump-table index arithmetic into earlier delay slots, and keeps some dead copies, when the
  function's entry is address-taken. If the target's jtbl code looks "impossible", grep asm/data for
  the function's address (a handler table). Then add a static table in the C file, for example
  `static s32 (*const D_800841D4_entry[])(...) = { func_X };`. Game `.rodata` from C objects is
  discarded at link, so this adds no bytes. It landed func_15072B44 and func_15031FC8, which had been
  written off as hand-written asm.
- **Offline variant sweeps land near-misses in 2-3 checks.** Agents who closed register,
  stack-slot and scheduling residues fastest wrote a small script in `my/`. It runs the
  image's `/opt/ido/5.3/cc` through `mw.run(cmd, cwd)`, using the same flags
  the Makefile gives this file (game files add `-Olimit 3000`). The script compiles dozens of
  generated variants, disassembles each with `mips-linux-gnu-objdump -dr`, and scores it
  against `target.s`. Only the winner goes through `mw.py check`. If `-K` crashes ugen,
  run `as1` by hand on the `.G` file instead. Levers found this way:
  - the order in which values are first defined (IDO hands out FP registers and spill
    slots in that order, so a dead `d = 0.0f;` up front can do it);
  - copying a param into a local;
  - same-line `if (++k == n) k = 0;` (the -g3 line numbers steer as1).
- **Local widths:** widening an m2c `u8` temporary to `s32` can fix allocation; when only byte
  stack offsets differ, reorder named locals or change their widths. Declaring a loop
  pointer inside the loop body opens a -g3 scope that keeps stores before the increments.

IDO idioms: `tools/ido_cookbook.md` (read it), `tools/ido_reference.md`
(large, grep it for a specific symptom). Matched C in other `src/*.c` files is a good style
reference for this codebase.
