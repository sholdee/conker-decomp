# ultralib headers

Header tree (`include/`) and the private headers of `src/` from the ultralib decompilation of
libultra, as pinned by DevOldSchool/conkers-bfd-decomp: https://github.com/DevOldSchool/ultralib
(branch `codex/libultra-2.0g`, commit `87af1e4d8ed666f2ad407dc11c6e47736094f2f8`).

The libultra 2.0G files listed in `../../ultralib.mk` are ultralib's sources, unmodified, compiled
with `ULTRA_CFLAGS` (see the Makefile): ultralib's own defines and include paths, `BUILD_VERSION=VERSION_G`,
os/ and io/ at -O1, libc/ and gu/ at -O3.
