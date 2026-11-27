# Conker's Bad Fur Day decompilation

![code matched](../../raw/progress/badge-matched.svg)
![fully linked](../../raw/progress/badge-linked.svg)
![functions named](../../raw/progress/badge-named.svg)
![data in source, excluding assets](../../raw/progress/badge-data.svg)

A decompilation of the CPU code of *Conker's Bad Fur Day* (Nintendo 64, US release). The build
reproduces the original US ROM byte for byte, and it is fully linked: code and data can move.

You need your own copy of the game to build it. This repository contains no ROM, no game assets and no
game text.

## Status (US only)

![The US code by source file](../../raw/progress/treemap.svg)

The map shows every source file, sized by its code. Green is C and blue is hand-written assembly. CI
redraws the map and the badges after each merge to `master` (`tools/progress.py`).
[progress.json](../../blob/progress/progress.json) has all the figures, including how many struct fields
and data symbols have names.

- All CPU code is matched. Each function in the init, game and debugger segments is C that compiles to
  the original bytes, or hand-written assembly.
- A small part of the code is hand-written assembly, as in other N64 games. Examples are the exception
  handler, the memory paging and some matrix maths. Most pieces have a recorded proof that the compiler
  did not produce them. For a few pieces, the evidence is compiler searches that found no match from
  natural C. [ACCOUNTING.md](ACCOUNTING.md) gives the counts, the proofs and the evidence.
- The code and data can move. Each reference uses a symbol, not a fixed address, and CI checks this on
  every change. A separate test, `tools/shifttest/shift_test.sh`, adds code and boots the game in an
  emulator. A few fixed sizes remain. [CONTRIBUTING.md](CONTRIBUTING.md) lists them, and the build stops
  with a message before you reach one.
- The data is in the source. Each file defines its own data, with real types. A few small pieces stay
  extracted, because the evidence does not show which file owns them. The assets in the code's data
  (microcode, fonts and the resource manifest) are not in the repository. The build extracts them from
  your ROM, and it does the same for the game's text and the other assets.
- The code is organised by subsystem. Each function and global that other files use is declared once, in
  the header of its subsystem. A few local declarations remain where IDO needs them for the original
  code, and a comment says why. The files follow the pattern of Rare's own source paths, for example
  `src/game/Effects/PipeExplode/pipeexplode.c`. A few files have Rare's exact path, from path strings in
  the ROM and in prerelease builds. The other names are descriptive.
- Naming is the main open work. Some functions, struct fields and data keep names made from their
  address or offset (`func_XXXXXXXX`, `D_XXXXXXXX`, `unk1C`), because no evidence shows yet what they do.
  [tools/RENAMING.md](tools/RENAMING.md) gives the rules: Rare's own names first, then Nintendo SDK
  names, then names that the code supports.
- Other open work: the C files that still have address names (`conker/src/*.c`), a few known fake
  matches, and the assembly pieces that rest only on search evidence. The EU, debug and ECTS versions
  have splat configurations (they split a ROM into code and data), but these versions are not
  decompiled, and their builds are not verified. [CONTRIBUTING.md](CONTRIBUTING.md) lists the fake
  matches and explains how to help.

## Building

You need:

- An x86-64 Linux host, or a Linux VM. The toolchain image installs the x86-64 Linux build of the IDO
  compiler.
- [Podman](https://podman.io/) or Docker. [tools/toolchain/Containerfile](tools/toolchain/Containerfile)
  defines the toolchain image. It has Ubuntu 24.04, MIPS binutils and pinned Python packages
  (`tools/toolchain/constraints.txt`). It also has the IDO 5.3 compiler from a pinned ido-static-recomp
  release, and a gzip with Rare's compression behaviour, built from source.
- A US ROM of the game in big-endian `.z64` format, with sha1 `4cbadd3c4e0729dec46af64ad018050eada4f47a`.

```sh
git clone --recursive https://github.com/sholdee/conker-decomp.git
cd conker-decomp
cp /path/to/your/rom.z64 baserom.us.z64
sha1sum baserom.us.z64          # must print 4cbadd3c4e0729dec46af64ad018050eada4f47a

podman build -t conker -f tools/toolchain/Containerfile .
podman run --rm --userns=keep-id --security-opt label=disable -v "$PWD":/conker -w /conker conker \
  bash -c 'python3 tools/toolchain/patch_splat.py && make extract && make -C conker extract &&
           make -C conker -j"$(nproc)" && make -C conker replace && make -j"$(nproc)"'
```

With Docker, use `docker build` with the same arguments. For `docker run`, pass
`--user "$(id -u):$(id -g)"` in place of `--userns=keep-id --security-opt label=disable`.

What the steps do:

| Step | Result |
|------|--------|
| `tools/toolchain/patch_splat.py` | Patches the splat submodule. It is idempotent. Run it again after each `git submodule update` |
| `make extract` | Checks the ROM's sha1, splits the ROM into `assets/`, decompresses the game code segment and joins the code segments into `conker/conker.us.bin` |
| `make -C conker extract` | Checks `conker/conker.us.bin` and splits it with splat into `conker/asm/` (the extracted data, and the assembly the build includes) |
| `make -C conker` | Compiles the C (IDO 5.3 via asm-processor), assembles the `.s` files and data, links, and checks `conker/build/conker.us.bin` |
| `make -C conker replace` | Recompresses the game segment and puts the rebuilt code back into the ROM layout |
| `make` | Links the ROM and checks it |

A successful build ends with:

```
build/conker.us.bin: OK      # conker/build/conker.us.bin, sha1 842e3d348e3c8ae0039e2ab367ad492f9b5266d8
build/conker.us.z64: OK      # the rebuilt ROM, sha1 4cbadd3c4e0729dec46af64ad018050eada4f47a
```

`git status` does not show changes inside `tools/n64splat`, because `.gitmodules` sets
`ignore = dirty` for it. In the submodule itself, `git status` shows the files that `patch_splat.py`
changes, and `src/splat/segtypes/n64/i1.py` even in a fresh clone. The pinned upstream commit stores
`i1.py` with CRLF line endings, but its `.gitattributes` asks for LF, so Git reports a change. The
content is the same.

After you edit a source file, run the last three steps again (`make -C conker`, `make -C conker replace`,
`make`). Everything the build writes (`assets/`, `rzip/`, `build/`, `conker/asm/`, `conker/build/`) is
gitignored and must never be committed.

## Layout

| Path | Contents |
|------|----------|
| `conker/src/game/<Area>/<Name>/` | The game segment: C files and hand-written `.s` files, in Rare's layout |
| `conker/src/init/<Area>/<Name>/` | The init segment (boot, audio, OS and controller code) |
| `conker/src/debugger/` | The debugger segment |
| `conker/src/libultra/` | libultra (SDK) files, some built from [ultralib](https://github.com/decompals/ultralib) source |
| `conker/src/checks/` | Struct size checks (`structsizes.c`), compiled but not linked |
| `conker/src/*.c` | A few files whose purpose is not clear yet. They keep address names |
| `conker/include/conker/<subsystem>.h` | One header for each subsystem: the declarations of its functions and globals |
| `conker/include/` | Shared types (`structs.h`, `effects.h`) and the SDK 2.0L headers, with small libc headers (`libc/`) and assembler macros (`macro.inc`) |
| `conker/conker.us.yaml` | The splat configuration of the code segments. `conker.us.yaml` at the top splits the ROM |
| `tools/` | Build tools, checks (`accounting.py`, `linkcheck.py`, `warncheck.py`) and contributor tooling |

## How it was made

Most of the matching, naming and cleanup was done by fleets of AI agents (Claude), directed and
reviewed by the maintainer. They followed the runbooks in [AGENTS.md](AGENTS.md) and `tools/*.md`.
Each agent worked in its own workspace (`tools/mw.py`, `tools/cw.py`). Before a change landed, the
whole ROM had to build byte-identically.

The project relies on mechanical checks rather than on the agents' word. CI rebuilds the ROM on every
change, and its checks run from the trusted base branch:
- Every function is compiled C or hand-written assembly with a recorded proof (or, for a few pieces,
  search evidence).
- No address is pinned, and no pointer or address is written as a number, in data or in code.
- The committed accounting manifest agrees with the build.
- The number of compiler warnings does not rise.

Independent AI audits tried to defeat these checks and to disprove the claims in this README. Each round
found ways past the checks, and each was closed. The checks are strong evidence, not a proof: if you
find a way past one, please open an issue.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). It gives the checks that every change must pass, and the rules
for names, types and data. It also explains why no assets or game text go into the repository.

## Licence

The project's own code is released under the [MIT licence](LICENSE). Material from other projects keeps
its own terms. [THIRD_PARTY.md](THIRD_PARTY.md) lists it with the required notices, including the
original mkst/conker code, the Nintendo SDK headers and sources, and the bundled tools. *Conker's Bad
Fur Day* is the property of its owners, and this project is not affiliated with them.

## Credits

- [mkst/conker](https://github.com/mkst/conker) by Mark Street and contributors: the original project
  this repository grew from. It worked out the ROM layout, the compressed sections and the build, and
  matched the first functions.
- [DevOldSchool/conkers-bfd-decomp](https://github.com/DevOldSchool/conkers-bfd-decomp): an independent
  decompilation of the game. Its translation-unit boundaries and many struct, field and function names
  were adopted here by address, and its data-layout notes informed the data work. It is also the place
  to go for assets.
- The decompilations of Rare's other N64 games, which share the modified audio library:
  [Perfect Dark](https://github.com/n64decomp/perfect_dark) by Ryan Dwyer,
  [Donkey Kong 64](https://gitlab.com/dk64_decomp/dk64),
  [Diddy Kong Racing](https://github.com/DavidSM64/Diddy-Kong-Racing) and
  [Banjo-Kazooie](https://github.com/n64decomp/banjo-kazooie). Byte matches with their code gave the
  SDK names of many audio functions and the file layout of `src/libultra/audio/`.
- [ultralib](https://github.com/decompals/ultralib) (decompiled libultra),
  [splat](https://github.com/ethteck/splat) and [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [asm-processor](https://github.com/simonlindholm/asm-processor),
  [asm-differ](https://github.com/simonlindholm/asm-differ), [m2c](https://github.com/matt-kempster/m2c),
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
  [ido-static-recomp](https://github.com/decompals/ido-static-recomp), and
  [gzip](https://github.com/mkst/gzip) patched for pre-1.5 compression output.
