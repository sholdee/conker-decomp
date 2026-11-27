# Third-party material and notices

The project's own code is released under the MIT licence in [LICENSE](LICENSE). That licence covers
only work its copyright holders can license. It does not cover the material listed below, which keeps
its own terms, and it grants no rights in Conker's Bad Fur Day itself. The game, its code and its data
are the property of their owners (Rare Ltd. / Microsoft). This repository contains no ROM image and no
extracted assets. The game code it reconstructs is published as C source and as assembly, as other
N64 decompilations publish theirs; you need your own copy of the game to build it.

## Projects this repository derives from

### mkst/conker (Mark Street and contributors, 2020-2021)

This repository started as a fork of <https://github.com/mkst/conker>, which was published without
a licence. The ROM layout, the splat and compressed-section configuration (`*.yaml`, `config/`), the
original build system and Makefiles, the rarezip tools (`tools/rarezip.py`, `tools/rareunzip.py`,
`tools/splat_ext/`, `tools/compress_dir.py`, `tools/extract_compressed.py`), the first matched functions and headers, and the SDK headers under
`conker/include/2.0L/` come from that project. Its history is kept in this repository's git history.
Contributors there were Mark Street (mkst), ianwal, Mittenshugg and alf. Those parts remain the
copyright of their authors.

### DevOldSchool/conkers-bfd-decomp (MIT)

<https://github.com/DevOldSchool/conkers-bfd-decomp> is an independent decompilation of the same game.
This repository adopted from it, by address:

- the translation-unit boundaries of 39 game files (its game yaml; commit c8885774) and the split of
  the init motor code into two files (commit 8fc29318);
- struct, field, enum, function and local names and types, adopted during the 2026-11 cleanup and
  naming passes where they describe the same memory layout (for example the model-id enum in
  `conker/src/game/Effects/Blood/blood.c`).

It uses the following notice:

```
MIT License

Copyright (c) 2026 DevOldSchool

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

DevOldSchool's project is not a fork. Its own reused material (ultralib, Rare-modified library sources
from n64decomp/007, RSP sources) was not copied here. Only its configuration and names were used.

## Other decompilations used as references

Rare's other N64 games use the same modified audio library (n_audio). The decompilations below were
compared with this game's audio code function by function, with their sources compiled by this
project's own IDO 5.3. No code was copied from them. What was taken:

- the SDK names of functions whose bytes match (for example `n_alCSPSetBank`, `alCSeqSetLoc`,
  `_bnkfPatchSound`, `_n_sndpVoiceHandler`, `_depth2Cents`, `initOsc`);
- the split of `conker/src/libultra/audio/` into translation units where Perfect Dark and Donkey Kong 64
  have the same functions in the same order (for example `bnkf.c` and `osc.c`, `n_cspsetbank.c` and
  `n_sndplayer.c`, `n_seqplayer.c`, `n_event.c`). The file names are the SDK's. `osc.c`, which the SDK
  does not have, is named after its functions (the SDK demo code's `initOsc`, `updateOsc`, `stopOsc`).

| Project | Licence |
|---------|---------|
| Perfect Dark, <https://github.com/n64decomp/perfect_dark> | MIT, Copyright (c) 2022 Ryan Dwyer (notice below) |
| Donkey Kong 64, <https://gitlab.com/dk64_decomp/dk64> | CC0 1.0 |
| Diddy Kong Racing, <https://github.com/DavidSM64/Diddy-Kong-Racing> | CC0 1.0 |
| Banjo-Kazooie, <https://github.com/n64decomp/banjo-kazooie> | CC0 1.0 |

Perfect Dark uses the following notice:

```
MIT License

Copyright (c) 2022 Ryan Dwyer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Nintendo SDK (libultra) material

| Path | Origin | Terms |
|------|--------|-------|
| `conker/lib/ultralib/` | Headers and private headers of ultralib, the decompiled libultra, as pinned by DevOldSchool: <https://github.com/DevOldSchool/ultralib> (fork of <https://github.com/decompals/ultralib>), commit `87af1e4d8ed666f2ad407dc11c6e47736094f2f8` | No licence. The headers carry Nintendo / Silicon Graphics copyright notices |
| `conker/src/libultra/` | libultra 2.0G sources from the same ultralib commit (see `conker/ultralib.mk`), and SDK library sources from mkst/conker | Several files carry Silicon Graphics / Nintendo "unpublished proprietary source code" notices, kept as found |
| `conker/include/2.0L/` | N64 SDK 2.0L headers, from mkst/conker (most are identical to ultralib's) | Nintendo / Silicon Graphics copyright notices |

Other public N64 decompilations publish the same material, but none of it is under this repository's
licence.

## Tools included in the repository

| Path | Origin | Licence | Notes |
|------|--------|---------|-------|
| IDO 5.3 (toolchain image) | Static recompilation of the IRIX IDO 5.3 C compiler: release v1.2 of <https://github.com/decompals/ido-static-recomp> (`ido-5.3-recomp-linux.tar.gz`, sha256 `ab5c7415…e506`) | ido-static-recomp has no licence. The recompiled programs are translations of SGI's proprietary IDO binaries | Not distributed in this repository. `tools/toolchain/Containerfile` downloads it |
| `gzip-rare` (toolchain image) | GNU gzip from <https://github.com/mkst/gzip> at commit `f6f21831c51dd3d13c98f536e836aa1b43a0b0b2`, configured with `--enable-rare-deflate` (restores pre-1.5 deflate output) | GPL-3.0-or-later. Copyright (C) 2018 Free Software Foundation, Inc.; Copyright (C) 1993 Jean-loup Gailly | Not distributed in this repository. `tools/toolchain/Containerfile` builds it from that source |
| mupen64plus (emulator image) | Release 2.6.0 bundle of <https://github.com/mupen64plus/mupen64plus-core> (`mupen64plus-bundle-linux64-2.6.0.tar.gz`, sha256 `37675357…3e3b`) | GPL-2.0-or-later | Not distributed in this repository. `tools/shifttest/Containerfile` downloads it for the emulator shift test |

## Git submodules (not vendored; fetched from upstream)

| Path | Upstream | Licence |
|------|----------|---------|
| `tools/n64splat` | <https://github.com/ethteck/splat> | MIT, Copyright (c) 2021 Ethan Roseman |
| `tools/asm-differ` | <https://github.com/simonlindholm/asm-differ> | Unlicense (public domain) |
| `tools/asm-processor` | <https://github.com/simonlindholm/asm-processor> | Unlicense (public domain) |
| `tools/mips_to_c` | <https://github.com/matt-kempster/m2c> (m2c) | GPL-3.0 |

