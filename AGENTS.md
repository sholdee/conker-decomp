# Conker decompilation — matching runbook (v2, `mw`)

Goal: every asm function → byte-identical C (IDO 5.3 `-O2 -g3`). The gate is the ROM sha1
(inner `conker/build/conker.us.bin` = `842e3d34…`, outer `build/conker.us.z64` = `4cbadd3c…`).

## Toolchain
Builds run in the podman container `conker-tc` (ubuntu 24.04 + binutils-mips-linux-gnu +
python reqs; `tools/toolchain/Containerfile`). It must be running:
```
podman start conker-tc   # or create it: tools/toolchain/run.sh
podman exec -d conker-tc python3 ~/conker/tools/toolchain/tcd.py ~/conker-ws/.tcd.sock   # after a start
podman exec -w ~/conker conker-tc bash -c 'make -C conker -j32 && make -C conker replace && make -j16'
```
A clean build of the code bin is ~5 s; one file + relink + sha ~0.6 s. `mw.py` sends commands to
the `tcd` socket daemon when it is up — `podman exec` serializes on podman's lock and took
20-40 s per call with a 16-agent fleet; it remains the fallback.

## Matching
Read `tools/MATCHING.md` (the agent brief). In short:
```
python3 tools/mw.py pick [--min B] [--max B] [--desc]   # remaining stubs by size
python3 tools/mw.py new FUNC     # private reflinked workspace ~/conker-ws/FUNC (+ m2c draft)
python3 tools/mw.py check FUNC   # exact whole-bin verdict + asm diff (+ FRAME/LITERALS hints)
python3 tools/mw.py land FUNC    # merge into main under a lock, full sha gate, commit
```
Workspaces never touch the main tree, so any number can run in parallel; `land` is the only
writer and re-gates every merge. There is no other invariant to remember.

## Build features that make C matchable
- `tools/rodata_extern.py` (post-compile, in the Makefile): game rodata is still asm and
  game `.c.o(.rodata)` is discarded, so every `.rodata` reference in a C object (float/double/
  string literals, switch jump tables) is retargeted to its original ROM address. Write
  literals naturally — `extern f32 D_...` is NOT codegen-equivalent.
- `conker/jtbl_labels.us.txt`: PROVIDEs the case labels the asm jump tables reference.

## History
The v1 Codex orchestrator / permuter daemon (stalled at ~10 matches/day, July 2026) is
documented in `tools/AGENTS_v1_orchestrator.md`; its tools remain in tools/ but are unused.

## Fresh host
```
git submodule update --init
sed -n '/\[4\/5\]/,/^fi$/p' setup.sh | sed 1d | bash     # splat marker-label patch (spimdisasm 1.42)
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
