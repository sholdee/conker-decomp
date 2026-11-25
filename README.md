# Conker's Bad Fur Day decompilation

A decompilation of the CPU code of *Conker's Bad Fur Day* (Nintendo 64, US release). The build
reproduces the original US ROM byte for byte.

You need your own copy of the game to build it. This repository contains no ROM and no game assets.

## Status (US only)

- **US code: 100% accounted for.** 97.17% of the US CPU code (2,187,188 of 2,250,992 bytes, in the
  init, game and debugger segments) is C that compiles to byte-identical code with IDO 5.3. The
  remaining 2.8% is hand-written assembly with a recorded proof that it was never C. That is 141
  functions proven by rules, plus 8 functions (592 bytes) in a "search tier" that no rule covers. The
  project accepted those 8 as hand-written because about 10,000 compiler and flag combinations never
  reproduced them from C. [ACCOUNTING.md](ACCOUNTING.md) has the per-segment table, the proof rules and
  the search-tier caveat.
- **Data: not decompiled yet.** The game's `.data`, `.rodata` and `.bss` are assembled byte-identically
  from the original ROM data and are not yet decompiled into C. That is the next milestone, after
  symbol naming. Most functions and globals still have address names (`func_XXXXXXXX`, `D_XXXXXXXX`).
- **Assets are out of scope.** Models, textures, audio, animations, scripts and text are packed into
  the ROM as the original binaries. For asset extraction and documented asset formats, see
  [DevOldSchool/conkers-bfd-decomp](https://github.com/DevOldSchool/conkers-bfd-decomp).
- **Other versions.** Splat configurations for the EU, debug and ECTS ROMs exist (`*.eu.yaml`,
  `*.debug.yaml`, `*.ects.yaml`), but those versions are not decompiled and their builds are not
  verified.

## Building

You need:

- an x86-64 Linux host, or a Linux VM (the IDO compiler in `ido/` and `tools/gzip` are prebuilt
  x86-64 Linux binaries);
- [Podman](https://podman.io/) or Docker. The toolchain image (Ubuntu 24.04, MIPS binutils, Python
  packages pinned in `tools/toolchain/constraints.txt`) is defined in
  [tools/toolchain/Containerfile](tools/toolchain/Containerfile);
- a US ROM of the game in big-endian `.z64` format, with sha1 `4cbadd3c4e0729dec46af64ad018050eada4f47a`.

```sh
git clone --recursive https://github.com/sholdee/conker.git
cd conker
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
| `tools/toolchain/patch_splat.py` | Patches the splat submodule for spimdisasm 1.42. It is idempotent; run it again after any `git submodule update` |
| `make extract` | Checks the ROM's sha1, splits the ROM into `assets/`, decompresses the game code segment and joins the code segments into `conker/conker.us.bin` |
| `make -C conker extract` | Checks `conker/conker.us.bin` and splits it with splat into `conker/asm/` (the data, and the assembly the build includes) |
| `make -C conker` | Compiles the C (IDO 5.3 via asm-processor), assembles the `.s` files and data, links, and checks `conker/build/conker.us.bin` |
| `make -C conker replace` | Recompresses the game segment and puts the rebuilt code back into the ROM layout |
| `make` | Links the ROM and checks it |

A successful build ends with:

```
build/conker.us.bin: OK      # conker/build/conker.us.bin, sha1 842e3d348e3c8ae0039e2ab367ad492f9b5266d8
build/conker.us.z64: OK      # the rebuilt ROM, sha1 4cbadd3c4e0729dec46af64ad018050eada4f47a
```

After you edit a source file, run the last three steps again (`make -C conker`, `make -C conker replace`,
`make`). Everything the build writes (`assets/`, `rzip/`, `build/`, `conker/asm/`, `conker/build/`) is
gitignored and must never be committed.

To diff a function with [asm-differ](https://github.com/simonlindholm/asm-differ), save a copy of a
matching build first: `mkdir -p conker/expected && cp -r conker/build conker/expected/`. Then run
`python3 ../tools/asm-differ/diff.py -mwo FUNC` from `conker/`, inside the container.

## Layout

| Path | Contents |
|------|----------|
| `conker/src/` | The code: C files and the hand-written `.s` files, one per original object where known (`init_*`, `game_*`, `debugger_*`, `libultra/`) |
| `conker/include/` | Project headers (`structs.h`, `functions.h`, `variables.h`, `effects.h`) and the SDK 2.0L headers |
| `conker/lib/ultralib/` | ultralib headers for the libultra files built from ultralib source (`conker/ultralib.mk`) |
| `conker/conker.us.yaml` | splat configuration of the code segments; `conker.us.yaml` at the top splits the ROM |
| `conker/symbol_addrs.us.txt`, `conker/undefined_*.us.txt` | Symbols |
| `tools/` | Build tools (rarezip compression, splat extensions, `rodata_extern.py`) and contributor tooling |
| `ido/ido5.3_recomp/` | The IDO 5.3 compiler, statically recompiled for Linux |

### Contributor tooling

The matching and cleanup work was done mostly by AI agent fleets, using the runbooks in
[AGENTS.md](AGENTS.md) and `tools/*.md` and the workspace tools `tools/mw.py` (matching) and
`tools/cw.py` (cleanup). Each change is built in a private workspace and lands only if the whole ROM
still matches. `tools/accounting.py` checks the code accounting in ACCOUNTING.md. These tools expect a
long-lived container (`tools/toolchain/run.sh`) and the maintainer's paths (`~/conker`, `~/conker-ws`).
The repo root follows the script's location, and `CONKER_WS`, `CONKER_CONTAINER` and `CONKER_REPO`
override the rest. You do not need any of them to build.

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
  were adopted here by address. It is also the place to go for assets.
- [ultralib](https://github.com/decompals/ultralib) (decompiled libultra),
  [splat](https://github.com/ethteck/splat) and [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [asm-processor](https://github.com/simonlindholm/asm-processor),
  [asm-differ](https://github.com/simonlindholm/asm-differ), [m2c](https://github.com/matt-kempster/m2c),
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
  [ido-static-recomp](https://github.com/decompals/ido-static-recomp), and
  [gzip](https://github.com/mkst/gzip) patched for pre-1.5 compression output.
