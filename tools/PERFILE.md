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
   - The references: which files use its symbols. `~/conker-ws/hdr/data_pieces.json` lists the C
     and asm files that use the symbols of each piece. It does not count references from other data.
   - IDO's layout: each unit pads its `.rodata`, `.data` and `.bss` to 16 bytes. Front-end data
     (const tables, strings) comes before the float literals and jump tables of the unit.
   - Hints only: `~/ext/conkers-bfd-decomp/docs/evidence/data-layout/` and `config/symbols/`. This is
     a read-only reference. Use its facts (addresses, sizes, boundaries). Never copy its code.
2. **A C unit's data goes into its C file.** Define the data with the correct type and initializer, in
   ROM order. Then give the file its slice in the yaml, and remove the externs. The procedures are in
   `tools/RODATA.md`: "Loop" for `.rodata`, "Static data and bss" for `.data` and `.bss`.
3. **Data of a hand-written asm unit** goes into that `.s` file, in its own `.section .rodata` (or
   `.data`). The ROM must match. If it does not match, leave the piece and write the reason in `notes.md`.
4. **Data that nothing uses** still belongs to a unit. Put it in the unit that its position shows.
   Rare's code had unused data too. Do not delete it.
5. **Assets stay out of the repository.** Microcode, textures, palettes, audio, model and animation
   data, game text (dialogue, menus, subtitles) and other large binary blobs are not code data. They
   stay extracted from the ROM at build time (an `incbin` of the extracted file). Never write them as
   C initializers. Tables of numbers, pointers and structs that the code uses, and the format and debug
   strings of the code, are code data: they go into C.
6. A global that other files use gets its `extern` in the subsystem header of its owner (see
   `tools/HEADERS.md`). If only its file uses a variable, and the ROM still matches, make it `static`.

## Part 2: types and casts
Then apply `tools/CASTS.md` to each file of your range:
- Correct the types that cause the remaining IDO warnings. The current list is
  `~/conker-ws/hdr/warnings_after2.txt`. Warning 642 (K&R) stays. The short struct views (for example
  `ParticleSpawn78`) stay, with their warnings: the owner decided this.
- Read each scalar cast (`(u8)`, `(s16)`, `(s32)`, `(f32)` ...). Remove a cast that does nothing:
  the value already has the type, or the implicit conversion is the same, and the bytes stay the same.
  Keep a cast that changes the code. Most of these are normal C and need no comment.

## Loop
```
python3 ~/conker/tools/cw.py new --tree pf.<name>            # a tree workspace for your range
python3 ~/conker/tools/ro_migrate.py check game_X --root ~/conker-ws/clean.pf.<name>/conker
#   edit the C files, the .s files and the yaml slices of your range
python3 ~/conker/tools/cw.py check clean.pf.<name>            # must print RESULT: MATCH
python3 ~/conker/tools/linkcheck.py ~/conker-ws/clean.pf.<name>/conker   # 0 pinned, 0 raw pointers
python3 ~/conker/tools/cw.py land  clean.pf.<name> -m "<file>: <what>"
```
- Land after each file or two. After each land, start fresh with `cw.py new --tree --force`.
- Other agents edit other ranges at the same time. The yaml lines near the ends of your range can
  conflict. On a merge conflict, do what the message says.
- In the yaml, change only the entries inside your range.
- Sometimes the neighbouring slices are not in text order. Then `ro_migrate.py check` can show a
  wrong window, so read the yaml directly.

## Rules
- Do not rename `func_` or `D_` symbols. Do not change the Makefile or `tools/`.
- The `.bss` order depends on the declarations before it. Read the comment at the top of
  `src/init_50A0.c`.
- If something cannot be done cleanly, stop and write the reason in the `notes.md` of the workspace.
