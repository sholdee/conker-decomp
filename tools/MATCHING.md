# Matching a function (mw workflow)

Goal: replace one `#pragma GLOBAL_ASM(...)` stub with C that compiles (IDO 5.3, `-O2 -g3`,
mips2) to the EXACT original bytes. The verdict is the whole code bin's sha1 — nothing else.

## Loop
```
python3 ~/conker/tools/mw.py new FUNC        # private workspace ~/conker-ws/FUNC (main tree is never touched)
#   edit  ~/conker-ws/FUNC/conker/src/<file>.c   — replace the stub line for FUNC with C
#   read  ~/conker-ws/FUNC/target.s (target asm), ~/conker-ws/FUNC/m2c.c (machine draft — types are guesses)
python3 ~/conker/tools/mw.py check FUNC      # ~2s: build + link + sha1; prints RESULT: MATCH | DIFF | BUILD-FAIL | STUB
python3 ~/conker/tools/mw.py land FUNC       # only after RESULT: MATCH — merges into main, re-gates, commits
```
`check` output: a `DIFF:` summary line (words equal, size, first mismatch offset) and an
asm-differ table: left = TARGET, right = CURRENT (your build), with `r` = register-only diff,
`i` = immediate diff, `|` = different instruction, `<`/`>` = missing/extra instruction.
The `CURRENT (N)` number is the diff score (lower is closer; 0 with RESULT: DIFF means the
function is right but something ELSE in the bin changed — e.g. a new literal in .rodata).

## Rules
- Edit ONLY files under `~/conker-ws/FUNC/conker/src/` and (sparingly) `include/`. Prefer
  local `extern`/prototype/struct declarations at the top of your own `.c` file over header
  edits. Never edit asm/, Makefile, yaml, tools.
- Never add a `#pragma GLOBAL_ASM`. Other stubs in the same file stay as they are.
- C89: all declarations at the top of a block. No `//`-only tricks needed (`-Xcpluscomm` is on).
- Rodata: the game's float/double constants, strings and tables live in asm data files.
  Reference them as `extern f32 D_800XXXXX;` (the `%hi/%lo(D_...)` symbol in target.s)
  instead of writing a literal — a literal adds a NEW .rodata entry and breaks the bin.
  Exceptions: constants built with `lui`/`mtc1` (no D_ load) ARE literals — write them.
- Switches are fine: write a normal `switch`. The build retargets the compiler's jump table to
  the ROM's original `jtbl_<vram>` (tools/jtbl_extern.py), so case order/density must make IDO
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
   or extra locals, `-g3` keeps every named local on the stack — remove/add locals.
5. Keep your best version (copy the .c aside) — don't wander away from a lower score.

IDO idioms: `~/conker/tools/ido_cookbook.md` (read it), `~/conker/tools/ido_reference.md`
(large, grep it for a specific symptom). Matched C in other `src/*.c` files is a good style
reference for this codebase.
