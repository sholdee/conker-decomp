# Per-file pass: data definitions in C, then types and casts

**Goal.** Each C file defines its own data in C, and its code is clean. The ROM must stay
byte-identical. The whole-ROM sha1 decides. Each agent owns a contiguous range of files in ROM order,
and the data between their slices.

## The owner's rule
The code must be clean, and faithful to the original source. If the original form is out of date
(K&R definitions, declarations without a prototype, an integer that carries a pointer), the original
form wins. Never add a cast only to hide a warning.

## Part 1: data
At the start, 754 pieces of data were still in asm (`asm/data/*.s`): 134 KB `.rodata`, 24 KB `.data`
and 313 KB `.bss`. Each piece lies between the slices that C files own in `conker/conker.us.yaml`.
The data of a compilation unit follows the same order as its code: if file A comes before file B in
the `.text`, then A's `.rodata`, `.data` and `.bss` also come before B's.
1. **Find the owner of each word.** Use this evidence:
   - The position: the piece lies after the slice of one file and before the slice of the next.
   - The references: which files use its symbols. Grep `conker/src/` and `conker/asm/` for each symbol
     of the piece. Also check references from other data.
   - IDO's layout: each unit pads its `.rodata`, `.data` and `.bss` to 16 bytes. Front-end data
     (const tables, strings) comes before the float literals and jump tables of the unit.
   - Hints only: `docs/evidence/data-layout/` and `config/symbols/` in `$CONKER_REF` (your optional
     clone of DevOldSchool/conkers-bfd-decomp, see "Paths" in AGENTS.md). This is a read-only reference. Use its facts (addresses, sizes, boundaries). Never copy its code.
2. **A C unit's data goes into its C file.** Define the data with the correct type and initializer, in
   ROM order. Then give the file its slice in the yaml, and remove the externs. The procedures are in
   `tools/RODATA.md`: "Loop" for `.rodata`, "Static data and bss" for `.data` and `.bss`.
3. **Data of a hand-written asm unit** goes into that `.s` file, in its own `.section .rodata` (or
   `.data`). The ROM must match. If it does not match, leave the piece and write the reason in `notes.md`.
4. **Data that nothing uses** still belongs to a unit. Put it in the unit that its position shows.
   Rare's code had unused data too. Do not delete it.
5. **Assets stay out of the repository.** Microcode, textures, palettes, audio, model and animation
   data, game text and other large binary blobs are not code data. Game text is the text content of the
   game: dialogue, menus, subtitles, credits, and the tables of task, level, character and item names.
   These stay extracted from the ROM at build time. A table that holds them also stays extracted data,
   for example the resource names at 0x242F30. The C code refers to such a table through an `extern`.
   Never write game text as C strings, initializers or comments. Identifiers are not game text: a symbol
   can name the character, model or level that the code handles (`MODEL_TEDIZ`).
   Code data goes into C: the tables of numbers, pointers and structs that the code uses. The format
   and debug strings of the code are code data too, for example `printf` formats, source file paths
   and tunable names.
6. A global that other files use gets its `extern` in the subsystem header of its owner (see
   `tools/HEADERS.md`). If only its file uses a variable, and the ROM still matches, make it `static`.

## Part 2: types and casts
Then apply `tools/CASTS.md` to each file of your range:
- Correct the types that cause the remaining IDO warnings. To list a file's warnings, rebuild its
  object and grep for `Warning` (the loop in `tools/CASTS.md` shows how). Warning 642 (K&R) stays. The short struct views (for example
  `ParticleSpawn78`) stay, with their warnings: the owner decided this.
- Read each scalar cast (`(u8)`, `(s16)`, `(s32)`, `(f32)` ...). Remove a cast that does nothing:
  the value already has the type, or the implicit conversion is the same, and the bytes stay the same.
  Keep a cast that changes the code. Most of these are normal C and need no comment.

## Loop
```
python3 tools/cw.py new --tree pf.<name>            # a tree workspace for your range
python3 tools/ro_migrate.py check game_X --root $CONKER_WS/clean.pf.<name>/conker
#   edit the C files, the .s files and the yaml slices of your range
python3 tools/cw.py check clean.pf.<name>            # must print RESULT: MATCH
python3 tools/linkcheck.py $CONKER_WS/clean.pf.<name>/conker   # 0 pinned, 0 raw pointers
python3 tools/cw.py land  clean.pf.<name> -m "<file>: <what>"
```
- Land after each file or two. After each land, start fresh with `cw.py new --tree --force`.
- Other agents edit other ranges at the same time. The yaml lines near the ends of your range can
  conflict. On a merge conflict, do what the message says.
- In the yaml, change only the entries inside your range.
- Sometimes the neighbouring slices are not in text order. Then `ro_migrate.py check` can show a
  wrong window, so read the yaml directly.

## Decisions already made (do not ask again)
- **Event messages stay `s32`.** The ROM shows that the original message API took the payload as an
  integer. Callers pass `(s32)&msg`. Do not change the API to `void *`.
- **A table of handlers with mixed signatures** is declared without a prototype, for example
  `void (*D_80086694[])()`. This is the original form.
- **A field that holds a colour in one mode and a pointer in another** (for example `Actor.fogColor`)
  keeps its main type, with casts at the other uses. Do not invent a union.
- **libultra files** keep the SDK names of their statics (for example `P`, `rpi` in `sinf.c`).
- **Short struct views** (for example `ParticleSpawn78`) stay, with their warnings.
- **Two files that the ROM proves are one unit** (for example a `.rodata` slice that starts at an
  address that is not 16-aligned) are listed in your report. They are merged in the closing pass.
- **A data-only unit** between two files can become its own file, for example `game_228CF0.c`. Name
  it in your report, so that it gets a row in `tools/subsystems.tsv`.

## IDO data facts (found in the first waves)
- **The .bss order.** The order is the definition order, rotated by the number of typedefs and
  `extern` variables declared before the definitions. Struct tags, struct fields and prototypes do not
  count. Each extra declaration moves the start back by one place.
- **An earlier extern moves a variable.** A `.bss` variable with an `extern` before its definition
  (for example in a subsystem header) goes after the others. A file whose globals have header externs
  defines its `.bss` before `#include "functions.h"`, after the includes that its types need.
- **Statics come last.** A `static` variable defined after the includes is placed after all the
  globals.
- **Alignment.** In `.bss`, an object larger than 4 bytes is 8-aligned. A 4-byte gap before such an
  object is alignment, not a hidden variable.
- **Base registers.** IDO shares one base register between all variables defined in the same file,
  but not with symbols that the file only declares `extern`. Sometimes the ROM uses a separate `lui`
  for a neighbouring variable. Then that variable was defined in another unit.
- **Unreferenced data.** Define a word that nothing uses as a named variable with a
  `/* not referenced */` comment. Give it the size that the layout shows.
- **A new file.** A file that you split off needs a row in `tools/subsystems.tsv`. Do not edit
  `tools/`: put its prototypes in a header group and name the file in your report.
- **Link errors that are not yours.** A workspace made just after other lands can fail to link. Then
  make it again with `cw.py new --tree --force` and apply your diff again.

## Rules
- Do not rename `func_` or `D_` symbols. Do not change the Makefile or `tools/`.
- The `.bss` order depends on the declarations before it. Read the comment at the top of
  `src/init_50A0.c`.
- If something cannot be done cleanly, stop and write the reason in the `notes.md` of the workspace.
