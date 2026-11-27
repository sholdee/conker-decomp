# Contributing

This project rebuilds the US ROM of *Conker's Bad Fur Day* byte for byte from C and a small amount of
hand-written assembly, each piece with a proof or, for a few, search evidence. Every change must keep
that true. This file gives the rules and the checks. The longer runbooks are in [AGENTS.md](AGENTS.md)
and `tools/*.md`.

## What never goes into the repository

- **No ROM and no part of a ROM.** The build extracts everything it needs from your own copy.
- **No assets.** Assets are textures, palettes, models, animations, audio, microcode and other large
  binary data. They stay data that the build extracts from the ROM.
- **No game text.** Game text is the text content of the game: dialogue, menus, subtitles, credits,
  and the tables of task, level, character and item names. Do not copy it into the repository as C
  strings or initializers, in comments, or in commit messages. A table that holds game text stays
  extracted data, and the C code refers to it through an `extern`.
  Identifiers are not game text. A symbol can name the character, model, level or item that the code
  handles, for example `MODEL_TEDIZ`. That is how a reader recognises the code.
  The code's own strings are code data and go into C. Examples are `printf` formats, source file paths
  and the names of debug tunables.
- **No secrets.** Never commit a token, a key or a private URL.

## The checks

Each pull request runs the matching gate in CI. The gate builds the ROM from scratch and then runs
these checks. All of them must pass:

| Check | What it requires |
|---|---|
| ROM sha1s | `conker/build/conker.us.bin` and `build/conker.us.z64` are byte-identical to the original |
| `tools/accounting.tsv` | The committed manifest agrees with the build: the same ranges are C, hand-written asm or padding (`tools/manifest_cmp.py`). If your change moves or renames functions, run `python3 tools/accounting.py` and commit the new manifest |
| `tools/accounting.py` | Each function is matched C, or hand-written assembly with a recorded proof |
| `tools/linkcheck.py` | The code and data can move. There is no pinned address, no pointer written as a number in data, no address built from numbers in code, and no jump without a relocation. An address name (`func_XXXXXXXX`, `D_XXXXXXXX`) is at its own address. Reviewed exceptions are listed in `tools/linkcheck_allow.tsv` |
| `tools/warncheck.py` | The number of IDO warnings is not above the limit in `tools/warnings_max.txt`. If you remove warnings, lower the limit in the same change |

CI uses the check scripts of the base branch, not the copies in your change. If you change a check,
the new version applies only after it is merged.

Run the same build locally before you open a pull request. [README.md](README.md) has the commands.
The IDO warning count is correct only after a clean build of the objects.

## Emulator shift test (optional)

The checks above do not boot the game. The shift test moves the code and boots the game in an
emulator. It needs your ROM (`baserom.us.z64`) and Podman or Docker:

```sh
tools/shifttest/shift_test.sh
```

The script copies the tracked files of your working tree to `build/shifttest/tree`. Untracked files
are not copied, so `git add` a new file first. It builds three ROMs with `NON_MATCHING=1`:
- the base ROM, which must still match the original,
- one ROM with 16 bytes of code (two empty functions) added to the first C file of the init segment,
- one ROM with the same change in the game segment.

It boots each ROM in mupen64plus in emulator modes 0 (interpreter) and 2 (dynamic recompiler), and
saves a screenshot at frames 600, 1200 and 2400. Each run must give 3 screenshots. The script prints
a table and writes `build/shifttest/summary.txt` and `build/shifttest/montage.png`. If a check
fails, it exits with 1. `--help` gives the options. For example, `shift_test.sh base debugger`
tests the debugger segment.

If the images do not exist, the script builds them: the toolchain image (`conker`, as in README.md)
and the emulator image (`tools/shifttest/Containerfile`). The output contains game data, so never
commit it.

## C code

- **The original form wins.** Write the code as Rare most likely wrote it. If the evidence shows an
  old style, keep it, for example K&R definitions and unprototyped declarations. A modern form that
  gives the same bytes does not replace it.
- **No tricks.** Do not hide declarations with `#define`, do not use inline assembly, and do not patch
  compiled instructions or data. If C does not match, the function stays assembly until it does.
  The build changes one thing in compiled objects: it removes IDO's alignment padding at some file
  boundaries (`tools/rodata_extern.py`). Some of the project's files are parts of one original file,
  and the original had no padding there.
- **Known fake matches.** A few functions match only with code that the original probably did not
  have. Each is marked `/* matching: */`. Natural C that matches in their place is welcome:
  - `objectsize.c`: dead blocks that call stand-in functions (`dbg3`, `dbgp`, `dbgu`, `dbgx`) that
    exist nowhere.
  - `levelobjects.c`: it leaves out `conker/heap.h`, so that it can declare its own view of
    `allocate_memory`, with a `u32` tag where the header has `s32`.
- **One declaration for each symbol.** A function or global that other files use is declared once,
  in the subsystem header of the file that owns it (`conker/include/conker/<subsystem>.h`). IDO
  rejects a second declaration that does not agree. A local declaration in a C file is allowed only
  when the header route does not match. Examples are a caller compiled with another prototype, or a
  block-scope declaration that shapes IDO's output. Put a `/* matching: */` comment above it that
  says why ([tools/HEADERS.md](tools/HEADERS.md), "Callers that disagree with the definition").
- **Adding an extern can move another file's `.bss`.** IDO orders a file's `.bss` by a counter that
  every declaration before it advances. A file that defines its `.bss` after the headers can change
  when a header gains externs. If you can, define a file's `.bss` before its headers.
- **Data goes with its owner.** A C file defines its own `.data`, `.rodata` and `.bss`, with real
  types and initializers. [tools/PERFILE.md](tools/PERFILE.md) gives the rules and the IDO facts
  about data order.
- **Code and data can move.** You can add code or data in any segment. Some fixed sizes remain, and the
  build stops with a message if a change goes past one of them:
  - The page-fault handler (`pagefault.s`) maps 0x1FC000 bytes of game code. A link `ASSERT` in
    `conker/undefined_syms.us.txt` checks it.
  - The compressed game code and data each have a reserved size in the ROM (`conker/Makefile`).
    When one outgrows its reservation, `tools/pad_to.py` fails.
  - The ROM is 64 MB. When the linked ROM is larger, the build fails.
- **Casts and types.** [tools/CASTS.md](tools/CASTS.md) and [tools/HEADERS.md](tools/HEADERS.md) give
  the rules.

## Hand-written assembly

A function can stay in a `.s` file only with a proof that IDO 5.3 did not produce it, or with the
search evidence that [ACCOUNTING.md](ACCOUNTING.md) describes. A function that has not been matched yet
is not hand-written. ACCOUNTING.md also gives the proof rules. A new hand-written function needs a
proof row in `tools/hasm_proofs.tsv` or a rule that `tools/accounting.py` checks.

If you match a function that is now hand-written assembly, that is a welcome change. Move it to C and
remove its proof.

## Names

[tools/RENAMING.md](tools/RENAMING.md) gives the conventions and the evidence order:

1. original names (from the debug prototype's struct viewer, and from source paths in the US ROM and in the
   prerelease builds),
2. names from the Nintendo SDK,
3. descriptive names that the code supports.

If there is no evidence, the address name stays. Functions are `prefix_camelCase`, for example
`math_mtxIdentity`. Globals are `gCamelCase`. A name can say what the code handles, but it must not
quote game text, such as a line of dialogue or a menu string.

Use `tools/rename.py` to rename symbols. It changes every reference and rebuilds. Then it compares the
ROM, and it commits only a matching result. Use `tools/mvfile.py` to move files in the same way.

## Pull requests

- Keep a pull request to one subject: a set of matches, a set of names, or one tool change.
- Say in the description what changed and how you checked it.
- Commits must not include build output (`assets/`, `rzip/`, `build/`, `conker/asm/`,
  `conker/build/`).
