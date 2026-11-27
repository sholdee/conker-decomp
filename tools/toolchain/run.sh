#!/usr/bin/env bash
# Build the toolchain image and start the long-lived `conker-tc` container that mw.py and the
# runbook use. Paths are mounted at the same location inside, so commands run unchanged.
# Overrides (use the same values for mw.py and cw.py): CONKER_WS (workspace directory, default
# ~/conker-ws), CONKER_CONTAINER and CONKER_IMAGE (both default conker-tc), CONKER_RAM (default
# /dev/shm/conker-build).
# Note: this replaces any existing container of the same name.
set -euo pipefail
REPO="$(cd "$(dirname "$0")/../.." && pwd)"
WS="${CONKER_WS:-$HOME/conker-ws}"
NAME="${CONKER_CONTAINER:-conker-tc}"
IMAGE="${CONKER_IMAGE:-conker-tc}"
RAM="${CONKER_RAM:-/dev/shm/conker-build}"
mkdir -p "$WS" "$RAM"   # workspace build/ dirs live in RAM (mw.py ensure_build)
podman build -t "$IMAGE" -f "$REPO/tools/toolchain/Containerfile" "$REPO"
podman rm -f "$NAME" >/dev/null 2>&1 || true
podman run -d --name "$NAME" --userns=keep-id --security-opt label=disable \
  -v "$REPO:$REPO" -v "$WS:$WS" -v "$RAM:$RAM" \
  --tmpfs /tmp:rw,size=8g,mode=1777 -e HOME="$HOME" "$IMAGE" sleep infinity
# Command daemon: mw.py talks to it over a socket instead of paying podman's exec lock per call.
podman exec -d "$NAME" python3 "$REPO/tools/toolchain/tcd.py" "$WS/.tcd.sock"
