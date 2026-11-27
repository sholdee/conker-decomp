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

The map shows every source file, sized by its code. Green is C that compiles to byte-identical code, and
blue is hand-written assembly (281 pieces with a proof, 8 with search evidence). CI regenerates it and the
badges from the committed files after each change to `master` (`tools/progress.py`).

- **Code: 100% decompiled and byte-matching.** All CPU code in the init, game and debugger segments is
  matched. As in other N64 decomps, a small part is hand-written assembly: 2.8% of the code, in 289
  labelled pieces. Examples are the exception handler and the 64-bit math helpers. 281 pieces have a
  recorded proof that they were never C. The other 8 (592 bytes) are accepted as hand-written because no
  compiler search reproduced them. [ACCOUNTING.md](ACCOUNTING.md) gives the proofs and the evidence.
- **Fully linked.** No address is pinned: every reference goes through a symbol, so the code and the
  data can move. CI fails on a pinned address, a pointer written as a number, or a jump without a
  relocation. Emulator tests added code to each segment (init, game, debugger) and data to each
  `.data`, `.rodata` and `.bss`. The shifted ROMs still ran the boot and attract sequence.
  `tools/shifttest/shift_test.sh` repeats the code test for the init and game segments
  ([CONTRIBUTING.md](CONTRIBUTING.md)). A few fixed
  sizes remain, for example the 0x1FC000 bytes of game code that the page-fault handler maps (room for
  about 7.9 KB more). The build stops with a message before one of them breaks the game.
  [CONTRIBUTING.md](CONTRIBUTING.md) lists them.
- **Data in source.** The game's `.data`, `.rodata` and `.bss` are defined in C (or in the hand-written
  `.s` file that owns them), except for a few small pieces whose owner the evidence does not show.
  Assets and game text (microcode, fonts, the resource manifest with its names) stay data that the build
  extracts from your ROM.
- **Names.** About 85% of the functions have names. The rest keep address names (`func_XXXXXXXX`)
  because no evidence makes their meaning clear yet. `progress.json` on the `progress` branch also
  counts the named struct fields and data symbols. Names follow [tools/RENAMING.md](tools/RENAMING.md):
  original names first (Rare's debug struct viewer, source paths in ROM strings), then SDK names, then
  descriptive names.
- **Source layout.** The files follow the pattern of the source paths in the game's own strings, for
  example `src/game/Effects/PipeExplode/pipeexplode.c`. Only a few paths are known from the ROM. The
  other directory and file names are descriptive.
- **Clean C.** Each function and global is declared in the subsystem header of the file that defines
  it. Each C file includes the headers that it uses: there are no umbrella headers. About 70 local
  declarations remain in C files. Each is there because IDO needs it to reproduce the original code,
  and a `/* matching: */` comment says why. Most are callers that saw a different prototype than the
  definition, or block-scope declarations that shape the generated code. The number
  of compiler type warnings can only go down. `tools/warnings_max.txt` holds the limit, and CI fails
  above it.
- **Other versions.** Splat configurations for the EU, debug and ECTS ROMs exist, but those versions are
  not decompiled and their builds are not verified.

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
| `conker/src/*.c` | A few files whose purpose is not clear yet. They keep address names |
| `conker/include/conker/<subsystem>.h` | One header for each subsystem: the declarations of its functions and globals |
| `conker/include/` | Shared types (`structs.h`, `effects.h`), the umbrella headers and the SDK 2.0L headers |
| `conker/conker.us.yaml` | The splat configuration of the code segments. `conker.us.yaml` at the top splits the ROM |
| `tools/` | Build tools, checks (`accounting.py`, `linkcheck.py`, `warncheck.py`) and contributor tooling |

## How it was made

Most of the matching, naming and cleanup was done by fleets of AI agents (Claude), directed and
reviewed by the maintainer. They followed the runbooks in [AGENTS.md](AGENTS.md) and `tools/*.md`.
Each agent worked in its own workspace (`tools/mw.py`, `tools/cw.py`). Before a change landed, the
whole ROM had to build byte-identically.

The project relies on mechanical checks rather than on the agents' word. CI rebuilds the ROM on every
change, and its checks run from the trusted base branch:
- Every function is compiled C or hand-written assembly with a recorded proof (or, for 8 pieces,
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
- [ultralib](https://github.com/decompals/ultralib) (decompiled libultra),
  [splat](https://github.com/ethteck/splat) and [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [asm-processor](https://github.com/simonlindholm/asm-processor),
  [asm-differ](https://github.com/simonlindholm/asm-differ), [m2c](https://github.com/matt-kempster/m2c),
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
  [ido-static-recomp](https://github.com/decompals/ido-static-recomp), and
  [gzip](https://github.com/mkst/gzip) patched for pre-1.5 compression output.
