# Constant data: a C file links its own .rodata

**Goal.** The C file's own `.rodata` becomes the constant data in the ROM for that file. `.rodata` holds
float and string literals, switch jump tables and const tables. After the change, no word of it is
pinned to an absolute address. The ROM must stay byte-identical. The whole-ROM sha1 decides.

**Background.** Most C files already do this. The yaml names the slice of each such file:
`[0xSTART, .rodata, file]`. For a named slice, `tools/rodata_extern.py` does not change the object, and
it trims the 16-byte IDO padding to the slice. For the open files, the `.rodata` of the object is not
equal to its ROM slice yet.

Commands run from the repository root. `$CONKER_WS` and `$CONKER_REF` are described in "Paths" in
AGENTS.md.

## Loop
```
python3 tools/cw.py new game_X.c                    # workspace $CONKER_WS/clean.game_X
python3 tools/ro_migrate.py check game_X --root $CONKER_WS/clean.game_X/conker
#   own .rodata against the ROM window between the slices of the neighbouring files, word by word
#   edit conker/src/game_X.c (and, if necessary, conker/conker.us.yaml) in the workspace
python3 tools/cw.py check clean.game_X              # builds; runs splat again if the yaml changed
python3 tools/ro_migrate.py carve game_X 0xSTART 0xEND --root $CONKER_WS/clean.game_X/conker
python3 tools/cw.py check clean.game_X              # must be MATCH with the slice carved
python3 tools/cw.py land  clean.game_X -m "own .rodata"
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
- **IDO order (checked by the first fleet).** In one compilation unit, IDO writes all front-end data
  first: const globals, string literals and const local initializers, in source order. After that come
  all float literals and jump tables, in function order. A definition placed between two functions
  still goes before every literal of the unit.
- **IDO placement rules.** A const scalar at file scope (static or not) goes to `.data`, not to
  `.rodata`. Use a const aggregate (array or struct) for `.rodata`. A static const local also goes to
  `.data`. A one-element const array can change the float register allocation where a const struct
  does not.
- **16-byte zero padding inside the window marks a unit boundary.** IDO pads the `.rodata` of each
  unit to 16 bytes. If the window has such padding between your words, the file was two (or more)
  original compilation units. See "Splitting a file" below.
- **A literal has a different value** (often by one unit in the last place). Write the float at full
  precision. Get the decimal value from the ROM word: `0.1290000081f`, not `0.129f`.
- **The order is different.** Usually a literal is hoisted or used twice. IDO merges identical
  literals in one file.
- **The slice starts or stops in the data of another file.** This occurs where our file split is not
  the same as the original compilation unit. Carve only your words. The words of a neighbour stay
  anonymous rodata.
- **Data that is not yours** (constants of a hand-written asm file, a table of another file): do not
  change it.

## Splitting a file
Split a file only for a unit boundary that the ROM shows inside the file. The signs are end-of-unit
zero padding, or front-end data (strings, const tables) after the literals of earlier functions. The first function of the new unit
must start on a 16-byte boundary in the text.
```
python3 tools/cw.py new --tree split.game_X          # a tree workspace: several files change
#   in $CONKER_WS/clean.split.game_X/conker/conker.us.yaml, add `- [0xOFF, c, game_OFF]` after the file's entry
SPLIT_ROOT=$CONKER_WS/clean.split.game_X/conker python3 tools/split_tu.py plan  game_X 0xOFF
SPLIT_ROOT=$CONKER_WS/clean.split.game_X/conker python3 tools/split_tu.py apply game_X 0xOFF
python3 tools/cw.py check clean.split.game_X           # runs splat for the new entry
#   carve one slice per unit (ro_migrate.py carve ... --root), check again, then land
```
- Each new file keeps all top-level declarations. Delete the ones it does not use. Then make sure
  that `check` still matches.
- If `conker/Makefile` gives the old file special flags (search it for the file name), stop and report.
  The new files need the same flags, and agents do not edit the Makefile.
- `cw.py land` lands the yaml, the changed file and the new files together.

## Pinned words
After a file links its own `.rodata`, every word of the slice must be reached through the object, not
through an absolute `D_` name. Run `python3 tools/linkcheck.py $CONKER_WS/<workspace>/conker --list`.
A pinned `D_` name can be in your file: an `extern` read of one of your own words. Replace that read
with the literal, or define the data under that name.

## Rules
- The bytes of the code must not change. `cw.py check` shows each function whose bytes moved.
- A pointer in your const data (`&D_...` or a function pointer) becomes a relocation. That is correct,
  and it is the purpose of this work. If the ROM word is a number that is not an address, keep it as a
  number.
- `$CONKER_REF` (your optional clone of DevOldSchool/conkers-bfd-decomp) is a reference that you only read. For many files,
  `docs/evidence/data-layout/<file>_*.md` shows which ROM words belong to which unit. The files in
  `config/symbols/*.txt` give the sizes of data symbols. Use these facts (addresses, sizes,
  boundaries). Do not copy its code. In some places its unit boundaries are not the same as ours.
  Thus, use them as hints only.
- Do not edit `tools/` or the Makefile. In the yaml, change only the slice of your own file.
- If a file cannot be done cleanly (for example, it needs a file split or a merge), stop. Write the
  reason in `$CONKER_WS/clean.<name>/notes.md`.

## Static data and bss (.data and .bss of a C file)
The same work applies to the `static` (and global) variables of a C file. The yaml must name the
`.data` and `.bss` of the file. Otherwise the link discards that section. Then `rodata_extern.py`
sends each reference to an absolute symbol named `.da.<file>.<ADDR>`. `linkcheck.py --list` shows
these.
- **.data:** carve `[0xROM, .data, file]` from the anonymous data, like a `.rodata` slice. The bytes
  must be equal. A pointer in the data is a relocation, so its target needs a symbol.
- **.bss:** a bss slice has no ROM bytes, so give its address with the dict form:
  `- {start: 0xROM, type: .bss, vram: 0xVRAM, name: file}`. For `start`, use the `start` value of the
  bss entry that contains your slice. Put a plain `{start: ..., type: bss, vram: ...}` entry after
  your slice for the rest. IDO pads each object's `.bss` to 16 bytes, like `.rodata`.
- The order of the variables in the section must equal the original. IDO keeps the order of the
  definitions in the file. `linkcheck.py` reports a label that is not at its address.
- After the carve, the `.da.` symbols of the file must be gone from `linkcheck.py --list`.
