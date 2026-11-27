# Cleaning a matched file (cw workflow)

Goal: turn one already-matching C file into idiomatic, readable C **without changing a single byte of the ROM**.
Every function in it stays byte-identical. The verdict is the whole-ROM sha1, exactly as for matching.
Commands run from the repository root. `$CONKER_WS` and `$CONKER_REF` are described in "Paths" in
AGENTS.md.

## Loop
```
python3 tools/cw.py new FILE          # e.g. game_79F10.c -> workspace $CONKER_WS/clean.game_79F10
#   edit $CONKER_WS/clean.<name>/conker/src/FILE   (one function at a time)
python3 tools/cw.py check clean.<name>  # MATCH | MISMATCH (+ which functions changed bytes) | BUILD-FAIL
python3 tools/cw.py land  clean.<name> -m "what you cleaned"   # merge into main, sha gate, commit
```
- Change one function (or one struct/type), then `check`. On MISMATCH, check names the functions whose
  bytes moved. Revert or adjust that function before going on; never stack unverified edits.
- **Land early and often** (every ~5-10 functions, and always before you run low on context). Other agents
  clean other files in parallel, and small lands keep merges trivial. After a land, your workspace's base is
  refreshed, so keep editing in the same workspace.
- Work only inside your workspace (scratch files in `my/`). Never edit the main tree or run git there.
- If `land` reports a merge conflict, copy main's current `conker/src/FILE` into the workspace,
  re-apply your unlanded edits, copy it to `base/src/FILE` too, then check and land.

## What to clean (wave 1: hacks + types, inside your file)
1. **Types and structs.**
   - Replace raw offset arithmetic (`*(s16 *)((u8 *)p + 0x24)`, `((s32 *)p)[9]`) with struct field access.
   - Give parameters and returns their real types (pointer-to-struct, `Gfx *`, `f32`, `u8` and so on), as
     shown by how callers and the code use them.
   - Name struct fields where the code makes their meaning clear (`unk24` → `timer`). Keep `unkXX` / `padXX`
     where it doesn't: an honest `unk` beats a wrong guess.
   - Prefer a struct already declared in `include/` or earlier in your file. Don't declare a second partial
     copy of a type that exists elsewhere.
2. **Locals and parameters.** Replace m2c names (`temp_v0`, `var_s1`, `sp2C`, `phi_a0`, `arg0`) with
   descriptive names. Remove casts and temporaries that exist only because m2c wrote them, wherever the bytes
   stay the same.
3. **Idioms.**
   - `for` loops instead of goto/do-while ladders.
   - Array indexing instead of pointer arithmetic.
   - Plain literals: `1` not `0x1`, `1.0f`, `'A'` where it is a char.
   - Boolean conditions, `switch` where it reads better.
   - GBI macros (`gSPDisplayList`, `gDPSetPrimColor`, ...) instead of hand-built `Gfx` words.
   - `NULL` for pointers.
   - Remove dead code that doesn't affect output only after `check` confirms it.
4. **Match hacks.** These include `#line` pairs, `if (1) {}`, `| 0` / `* 1` no-ops, dead stores, `volatile`
   that isn't real, padding locals, stray `;` lines, unused labels, gotos and `do {} while (0)` wrappers.
   - Try to replace each one with an idiomatic form that still matches. The causes are often understood:
     see `tools/MATCHING.md`, and the function's matching ledger (`$CONKER_WS/<func>/notes.md`, written by
     `mw.py note`) if you have one.
   - If nothing idiomatic matches, **keep the minimal hack and add a one-line comment** saying which IDO
     behaviour it steers, e.g. `// matching: IDO keeps 0x82 in a register only with a 2nd use`.
   - Block scopes, declaration order and `register` placement are normal IDO idioms; they need no comment.

## Lessons from earlier waves (use them)
- **K&R definitions.** Callers sometimes pass a u8/s16 parameter that the function reloads from the stack and
  narrows (`lbu`/`lh` from the arg slot). That is an old-style K&R definition, `void f(a) u8 a; {...}`, followed by
  the prototype callers use. It replaces fake LOW_U8/LOW_S16 macros and byte-split hacks.
- **Rodata float externs** such as `extern f32 D_800994C0;` used as a constant usually become the plain literal
  (full precision) with identical bytes. Check after each one.
- **Files compiled with `-g`** (see the Makefile's per-file OPT_FLAGS) keep every local in its own stack slot.
  Renaming and re-typing locals is free there; only statement shape matters.
- **Leftovers from file splits** can be deleted when `check` still matches: unused typedefs, prototypes and
  externs for other files' functions, and commented-out old drafts.
- **Unused stack-slot pads** (`s32 pad;`) and argument copies are normal in matched IDO code. Keep them with a
  short `// matching:` comment when every alternative mismatches; don't spend many checks on them.
- Give each function a one-line doc comment when its purpose is clear from the code.

## Out of scope for wave 1 (don't do these)
- Renaming exported symbols (`func_XXXXXXXX`, `D_XXXXXXXX`, anything declared in `include/` or used by other
  files). `tools/rodata_extern.py` derives addresses from `D_<8 hex>` names. A later central pass will apply
  names across the tree.
- Editing existing declarations in shared headers (`include/*.h`).
- Moving functions between files, or touching the yaml, Makefile or asm.
- Reordering functions or statements across functions. -g3 line numbers steer IDO's scheduler (as1 breaks
  ties by source line), so even reformatting can change bytes; check after every change.

## Reference: the other Conker decomp (read-only)
`$CONKER_REF` (your optional clone of DevOldSchool/conkers-bfd-decomp) has matched C for many of the same
functions, under the **same** `func_XXXXXXXX` names (same addresses). For each of your functions,
`grep -rn "func_XXXXXXXX(" "$CONKER_REF/src"` finds its version.
- Adopt its struct types, field names and local names wherever they describe the same memory layout and meaning.
- `config/symbols/*.txt` there names a few addresses; note them in your result, but don't rename (see above).
- Never run its scripts or build it.

## Result
When the file is done, or you are low on context, make sure everything that matches has been landed. Then
write `$CONKER_WS/clean.<name>/notes.md`: what is done, which hacks remain and why, and the next steps.
