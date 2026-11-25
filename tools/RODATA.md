# Constant data: a C file links its own .rodata

**Goal.** The C file's own `.rodata` becomes the constant data in the ROM for that file. `.rodata` holds
float and string literals, switch jump tables and const tables. After the change, no word of it is
pinned to an absolute address. The ROM must stay byte-identical. The whole-ROM sha1 decides.

**Background.** Most C files already do this. The yaml names the slice of each such file:
`[0xSTART, .rodata, file]`. For a named slice, `tools/rodata_extern.py` does not change the object, and
it trims the 16-byte IDO padding to the slice. For the open files, the `.rodata` of the object is not
equal to its ROM slice yet.

## Loop
```
python3 ~/conker/tools/cw.py new game_X.c                    # workspace ~/conker-ws/clean.game_X
python3 ~/conker/tools/ro_migrate.py check game_X --root ~/conker-ws/clean.game_X/conker
#   own .rodata against the ROM window between the slices of the neighbouring files, word by word
#   edit conker/src/game_X.c (and, if necessary, conker/conker.us.yaml) in the workspace
python3 ~/conker/tools/cw.py check clean.game_X              # builds; runs splat again if the yaml changed
python3 ~/conker/tools/ro_migrate.py carve game_X 0xSTART 0xEND --root ~/conker-ws/clean.game_X/conker
python3 ~/conker/tools/cw.py check clean.game_X              # must be MATCH with the slice carved
python3 ~/conker/tools/cw.py land  clean.game_X -m "own .rodata"
```
- `ro_migrate.py check` reads the last build of the workspace. After each edit, run `cw.py check`
  first.
- The window shows every ROM word between the slice of the previous migrated file and the slice of the
  next one. Usually your slice is a part of the window. The other words belong to neighbours that are
  not C (hand-written asm or data), or to other open files.

## Usual differences and their fixes
- **The window has more data between or around your literals.** This is the most frequent case. The
  extra data is tables, strings or structs that the original file defined. Our C reads them through
  `extern ... D_XXXXXXXX`. Define them in the C file as `const` or `static const` data, with the
  correct type and initializer, in the ROM order. Then remove the extern. If other files use the
  symbol, keep the same `D_` name for the definition. Wave 3 renames it later.
- **IDO order.** IDO writes the `.rodata` of a file in source order. A literal goes where a function
  first uses it. Const data goes where you define it. Thus the position of each definition relative to
  the functions sets its position in the ROM.
- **A literal has a different value** (often by one unit in the last place). Write the float at full
  precision. Get the decimal value from the ROM word: `0.1290000081f`, not `0.129f`.
- **The order is different.** Usually a literal is hoisted or used twice. IDO merges identical
  literals in one file.
- **The slice starts or stops in the data of another file.** This occurs where our file split is not
  the same as the original compilation unit. Carve only your words. The words of a neighbour stay
  anonymous rodata.
- **Data that is not yours** (constants of a hand-written asm file, a table of another file): do not
  change it.

## Rules
- The bytes of the code must not change. `cw.py check` shows each function whose bytes moved.
- A pointer in your const data (`&D_...` or a function pointer) becomes a relocation. That is correct,
  and it is the purpose of this work. If the ROM word is a number that is not an address, keep it as a
  number.
- `~/ext/conkers-bfd-decomp` is a reference that you only read. For many files,
  `docs/evidence/data-layout/<file>_*.md` shows which ROM words belong to which unit. The files in
  `config/symbols/*.txt` give the sizes of data symbols. Use these facts (addresses, sizes,
  boundaries). Do not copy its code. In some places its unit boundaries are not the same as ours.
  Thus, use them as hints only.
- Do not edit `tools/` or the Makefile. In the yaml, change only the slice of your own file.
- If a file cannot be done cleanly (for example, it needs a file split or a merge), stop. Write the
  reason in `~/conker-ws/clean.<name>/notes.md`.
