# Conker decompilation — maintainer and agent runbook

All US code is byte-matching C (IDO 5.3, mostly `-O2 -g3`) or proven hand-written asm (ACCOUNTING.md).
Every change must keep the ROM byte-identical. The gate is the ROM sha1 (inner
`conker/build/conker.us.bin` = `842e3d34…`, outer `build/conker.us.z64` = `4cbadd3c…`) plus
`tools/accounting.py`. Locally `mw.py land`, `cw.py land` and `rename.py` enforce it; on GitHub the
`Matching gate` workflow does.

Paths: `~/conker` is the repository and `~/conker-ws` the workspace root on the maintainer's host.
The tools take the repo root from their own location and honour `CONKER_REPO`, `CONKER_WS` and
`CONKER_CONTAINER` (default `conker-tc`).

## Toolchain
Builds run in the podman container `conker-tc`, made from `tools/toolchain/Containerfile` (Ubuntu
24.04, MIPS binutils, pinned Python packages, IDO 5.3 from the ido-static-recomp release at
`/opt/ido/5.3`, gzip-rare built from source). CI uses the same image. It must be running:
```
podman start conker-tc   # or create it: tools/toolchain/run.sh
podman exec -d conker-tc python3 ~/conker/tools/toolchain/tcd.py ~/conker-ws/.tcd.sock   # after a start
podman exec -w ~/conker conker-tc bash -c 'make -C conker -j32 && make -C conker replace && make -j16'
```
A clean build of the code bin is ~5 s; one file + relink + sha ~0.6 s. `mw.py` sends commands to
the `tcd` socket daemon when it is up — `podman exec` serializes on podman's lock and took
20-40 s per call with a 16-agent fleet; it remains the fallback.

## Workspaces
- Editing matched C (cleanup, naming, types): `tools/cw.py` (see `tools/CLEANUP.md`, `tools/SHARED.md`).
  `cw.py new FILE` or `--tree ITEM`, then `check` (names every function whose bytes moved) and `land`.
- Renaming symbols tree-wide: `tools/rename.py` (see `tools/RENAMING.md`).
- Re-matching a function from asm: `tools/mw.py` (see `tools/MATCHING.md`). Replace its C body with a
  `#pragma GLOBAL_ASM` stub, then `mw.py new FUNC`, `check`, `land`.
Workspaces never touch the main tree, so any number can run in parallel; `land` is the only writer
and re-gates every merge.

## Build features that make C matchable
- `tools/rodata_extern.py` (post-compile, in the Makefile): removes IDO's alignment padding at file
  boundaries where one original file is split into several project files, sets the o32 ABI flag, and
  makes the definitions of the game's private libultra copies weak. It can also retarget a C object's
  `.rodata` references to data that is still asm; today every C object owns its `.rodata`, so this is
  unused. Write literals naturally — `extern f32 D_...` is NOT codegen-equivalent.
- `conker/jtbl_labels.us.txt`: PROVIDEs the case labels the asm jump tables reference.

## Fresh host
```
git submodule update --init
python3 tools/toolchain/patch_splat.py                  # splat marker-label patch (spimdisasm 1.42)
git config core.hooksPath tools/git-hooks
tools/toolchain/run.sh                                   # image + conker-tc container
cp <rom> baserom.us.z64                                  # sha1 4cbadd3c…
podman exec -w ~/conker conker-tc bash -c 'make extract && make -C conker extract && make -C conker -j32 && make -C conker replace && make -j16'
mkdir -p conker/expected && cp -r conker/build conker/expected/   # golden objects for asm-differ
```

## Translation-unit splits (tools/split_tu.py)
IDO carries state across a file, so a function can be unmatchable only because it shares a file
with code that was really in another TU. To split `src/game_X.c` at ROM offsets A, B…:
1. add `- [0xA, c, game_A]` (…) after the file's entry in `conker/conker.us.yaml`;
2. run splat in the container (`python3 ../tools/n64splat/split.py conker.us.yaml` from `conker/`;
   keep main's `undefined_*_auto.txt`) — it writes the new asm dirs and `conker.ld` (both gitignored);
3. `python3 tools/split_tu.py apply game_X A,B` — every new file keeps all top-level declarations,
   gets its own functions, and a prototype in place of each moved definition (what later callers saw);
4. delete `conker/build/conker.us.bin`, rebuild, and check the sha1 (a stale bin passes silently).
`split_tu.py transplant FUNC OLDFILE NEWFILE` moves an attempt written against the old layout.
DevOldSchool/conkers-bfd-decomp's yaml was the source for the first 39 files (2026-11-16).

## libultra from ultralib (conker/ultralib.mk)

libultra files whose SDK source is C are built from ultralib's 2.0G sources, unmodified
(`conker/lib/ultralib` holds its headers; pinned commit in its README). To port another segment:
1. Find the source: same stem under ultralib `src/` (watch for mislabeled yaml segments - check what
   the code does; e.g. 0x24400 was vigetcurrcontext, not pigettype). Test flags offline first:
   2.0G is `os/ io/` -O1, `libc/ gu/` -O3, `libc/ll.c` -O1 -mips3 -32; Rare rebuilt some files with the
   game's -O2 -g3 (io/vimgr). Compare the object's .text with the ROM range, relocations masked.
2. yaml: segment `asm` -> `c`; copy the source to `src/libultra/<stem>.c`; add the stem to
   `ultralib.mk` (it gets `ULTRA_CFLAGS` and the directory's -O level; per-file overrides follow in the
   Makefile). Run splat, then restore `undefined_*_auto.txt` (splat drops entries; they are gitignored).
3. Link: an ultralib name that our asm knows only by address gets a symbol (rename the label, or an
   alias of a linked symbol in `undefined_syms.us.txt`), never a fixed number: tools/linkcheck.py fails
   on pinned addresses. Globals the C
   defines in data sections are made weak by rodata_extern, so the asm's data wins.
4. The game's private copies (`*2` stems) call the init copy for everything, even within their own
   file: rodata_extern `--weak-text` (set per file) makes their definitions weak and their game entry
   names are address aliases. Game copies that call each other (gu/ortho, gu/mtxutil) instead get
   their definitions renamed onto the game names with per-file `-D` flags.
5. Gate: tools/verify_rom.sh (clean build). A stale object can hide a missing ultralib.mk entry.

## Hand-written asm (hasm segments, conker/src/*.s)
Functions proven hand-written live as committed `conker/src/<name>.s` files, from yaml `hasm` segments
(`hasm_in_src_path: True`). splat writes each file once and never overwrites it. Proof rule, which is strict:
a function becomes hasm only if (a) a hand-written SDK `.s` reproduces its bytes exactly, or (b) it contains an
instruction IDO 5.3 -mips2 cannot emit (cop0/TLB/cache ops, 64-bit `d*`/`ld`/`sd`, trapping
`add`/`addi`/`sub`), or (c) a dataflow check shows it reads a register that is neither an ABI input nor
written first. Two further rules (2026-11-24) were checked against IDO's own output, 5,746 matched C
functions, with zero occurrences: (d) an opcode-2 `j`, or a branch that leaves the function
(IDO uses `b` for local jumps and never tail-calls); (e) a bare `cvt.w.s`/`cvt.w.d` with no `ctc1` in the
function. GCC output is ruled out because GCC's `move` assembles to `addu rd,rs,zero` (9 in all compiled C,
0 in these functions). Patterns that do appear in compiled C are not proof: `ra` saved above the frame,
a second sp allocation, `addiu r,zero,0`. The 2026-11-24 audit converted 94 functions, and a second pass converted 17 more (rules d/e) plus func_150ADAF0. A third pass (2026-11-25) added
rules each found 0 times in compiled C: `syscall`; no return (falls into the next function); a store below `$sp`
before the frame exists; `ra` saved in the outgoing-argument area or without allocating a frame; an odd FPR as a
single-precision arithmetic operand; `jal` without saving `ra`; every `mul.s` (6 or more) followed by a `nop`
(compiled C peaks at 67%); and object boundaries (IDO pads every C object's `.text` and `.rodata` to 16 at both
ends, so a function that meets proven asm at a non-16-aligned code or rodata address shares its object).
`bzero` and `__osSetFpcCsr` are source-identical with ultralib assembled by the full IDO 5.3 toolchain
(`/opt/ido/5.3/cc -O1` in the image, which includes `as0`). See ACCOUNTING.md. No unproven
function remains: `tools/accounting.py` checks that all US code is matched C or hasm with a recorded proof
(rules computed from the instructions, plus `tools/hasm_proofs.tsv` for rules a, c and the search tier).
Layout notes:
- The init and game code segments use `subalign: 4`: hasm splits put object boundaries on 4-byte addresses.
- ASFLAGS has `-no-pad-sections`.
- `tools/rodata_extern.py` trims IDO's 16-byte `.text` padding from C segments that end on a 4-byte boundary.
- A local `.L` label referenced across a split boundary is made global (`.globl`) in the file that
  defines it. Example: `taskexit.s` branches into `scriptinterp.s` at `.L150AE0AC`.
