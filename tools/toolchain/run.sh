#!/usr/bin/env bash
# Build the toolchain image and start the long-lived `conker-tc` container that mw.py and the
# runbook use. Paths are mounted at the same location inside, so commands run unchanged.
set -euo pipefail
REPO="$(cd "$(dirname "$0")/../.." && pwd)"
WS="${CONKER_WS:-$HOME/conker-ws}"
mkdir -p "$WS" /dev/shm/conker-build   # workspace build/ dirs live in RAM (mw.py ensure_build)
podman build -t conker-tc -f "$REPO/tools/toolchain/Containerfile" "$REPO"
podman rm -f conker-tc >/dev/null 2>&1 || true
podman run -d --name conker-tc --userns=keep-id --security-opt label=disable \
  -v "$REPO:$REPO" -v "$WS:$WS" -v /dev/shm/conker-build:/dev/shm/conker-build \
  --tmpfs /tmp:rw,size=8g,mode=1777 -e HOME="$HOME" conker-tc sleep infinity
# Command daemon: mw.py talks to it over a socket instead of paying podman's exec lock per call.
podman exec -d conker-tc python3 "$REPO/tools/toolchain/tcd.py" "$WS/.tcd.sock"
