#!/usr/bin/env bash
# Clean full build of main (inner code bin + outer z64) through the tcd daemon; prints both sha1s.
# Takes the land lock so no `mw.py land` can run mid-build:   tools/verify_rom.sh
set -uo pipefail
cd "$(dirname "$0")/.." || exit 1
exec 9>.mw_land.lock
flock 9
rm -rf conker/build build/conker.us.z64
python3 - <<'PY'
import sys
sys.path.insert(0, "tools")
import mw
r = mw.run("make -C conker -j24 2>&1 | grep -E 'Error|OK' | head -5; "
           "make -C conker replace >/dev/null 2>&1; make -j16 2>&1 | tail -1", str(mw.REPO), timeout=900)
print(r.stdout)
PY
sha1sum conker/build/conker.us.bin build/conker.us.z64
