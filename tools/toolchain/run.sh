#!/usr/bin/env bash
# Build the toolchain image and start the long-lived `conker-tc` container that mw.py and the
# runbook use. Paths are mounted at the same location inside, so commands run unchanged.
set -euo pipefail
REPO="$(cd "$(dirname "$0")/../.." && pwd)"
WS="${CONKER_WS:-$HOME/conker-ws}"
mkdir -p "$WS"
podman build -t conker-tc -f "$REPO/tools/toolchain/Containerfile" "$REPO"
podman rm -f conker-tc >/dev/null 2>&1 || true
podman run -d --name conker-tc --userns=keep-id --security-opt label=disable \
  -v "$REPO:$REPO" -v "$WS:$WS" -e HOME="$HOME" conker-tc sleep infinity
