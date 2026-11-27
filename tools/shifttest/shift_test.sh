#!/usr/bin/env bash
# shift_test.sh: check that the game still runs when its code moves.
#
# The script copies the tracked files of this working tree to build/shifttest/tree, extracts your ROM
# there, and builds one ROM per variant with NON_MATCHING=1:
#   base   no change (it must still match the original ROM)
#   init   two empty functions (16 bytes of code) added at the end of the first C file of the init segment
#   game   the same for the game segment
#   debugger  the same for the debugger segment (not in the default list)
# Then it boots each ROM in mupen64plus, in emulator modes 0 (interpreter) and 2 (dynamic recompiler),
# and takes screenshots at fixed frame numbers (--testshots). A run that stops early, for example
# because the game crashed or hung, gives fewer screenshots.
#
# Usage: tools/shifttest/shift_test.sh [options] [VARIANT...]     (default variants: base init game)
#   --modes "0 2"            emulator modes
#   --frames 600,1200,2400   frame numbers of the screenshots
#   --timeout 420            seconds that one emulator run may take
#   --jobs N                 emulator runs at the same time (default 2)
#   --rebuild                build the container images again, even if they exist
# Environment: CONTAINER_ENGINE (podman or docker; default: podman if installed),
#   TOOLCHAIN_IMAGE (default conker, the image README.md builds), EMU_IMAGE (default conker-emu-shifttest).
#
# Everything goes to build/shifttest/, which is gitignored: the ROMs, the screenshots, the logs,
# summary.txt and montage.png. Never commit them: they contain game data.
set -euo pipefail

REPO="$(cd "$(dirname "$0")/../.." && pwd)"
OUT="$REPO/build/shifttest"
TREE="$OUT/tree"
MODES="0 2"
FRAMES="600,1200,2400"
EMU_TIMEOUT=420
JOBS=2
REBUILD=0
VARIANTS=()

die() { echo "shift_test: $*" >&2; exit 2; }

while [ $# -gt 0 ]; do
  case "$1" in
    --modes) MODES="$2"; shift 2 ;;
    --frames) FRAMES="$2"; shift 2 ;;
    --timeout) EMU_TIMEOUT="$2"; shift 2 ;;
    --jobs) JOBS="$2"; shift 2 ;;
    --rebuild) REBUILD=1; shift ;;
    -h|--help) sed -n '2,/^set -euo/p' "$0" | sed '$d; s/^# \{0,1\}//'; exit 0 ;;
    base|init|game|debugger) VARIANTS+=("$1"); shift ;;
    *) die "unknown argument: $1 (see --help)" ;;
  esac
done
[ ${#VARIANTS[@]} -gt 0 ] || VARIANTS=(base init game)
[[ "$FRAMES" =~ ^[0-9]+(,[0-9]+)*$ ]] || die "--frames must be numbers separated by commas"
[[ "$EMU_TIMEOUT" =~ ^[0-9]+$ && "$JOBS" =~ ^[1-9][0-9]*$ ]] || die "--timeout and --jobs must be numbers"
for m in $MODES; do [[ "$m" =~ ^[0-2]$ ]] || die "an emulator mode is 0, 1 or 2"; done
NSHOTS=$(( $(tr -cd , <<<"$FRAMES" | wc -c) + 1 ))

ENGINE="${CONTAINER_ENGINE:-}"
if [ -z "$ENGINE" ]; then
  if command -v podman >/dev/null; then ENGINE=podman; elif command -v docker >/dev/null; then ENGINE=docker
  else die "podman or docker is necessary"; fi
fi
case "$(basename "$ENGINE")" in
  podman) USERARGS=(--userns=keep-id --security-opt label=disable) ;;
  *) USERARGS=(--user "$(id -u):$(id -g)") ;;
esac
TOOLCHAIN_IMAGE="${TOOLCHAIN_IMAGE:-conker}"
EMU_IMAGE="${EMU_IMAGE:-conker-emu-shifttest}"

ROM="$REPO/baserom.us.z64"
[ -f "$ROM" ] || die "$ROM is missing: copy your US ROM there (README.md, Building)"
[ -f "$REPO/tools/n64splat/split.py" ] || die "the submodules are missing: git submodule update --init"
command -v git >/dev/null && command -v tar >/dev/null || die "git and tar are necessary"

image() {  # image NAME CONTAINERFILE CONTEXT
  if [ "$REBUILD" = 1 ] || ! "$ENGINE" image inspect "$1" >/dev/null 2>&1; then
    echo "== building the image $1 (log: $OUT/logs/image-$1.log)"
    "$ENGINE" build -t "$1" -f "$2" "$3" >"$OUT/logs/image-$1.log" 2>&1 || die "image build failed, see the log"
  fi
}

# tc COMMAND: run COMMAND in the toolchain image, in the copied tree, without network access.
tc() { "$ENGINE" run --rm --network none "${USERARGS[@]}" -v "$TREE:/conker" -w /conker "$TOOLCHAIN_IMAGE" \
         bash -c "$1"; }

rm -rf "$OUT"
mkdir -p "$OUT/logs" "$OUT/roms" "$OUT/shots" "$OUT/syms" "$TREE"
image "$TOOLCHAIN_IMAGE" "$REPO/tools/toolchain/Containerfile" "$REPO"
image "$EMU_IMAGE" "$REPO/tools/shifttest/Containerfile" "$REPO/tools/shifttest"

echo "== copying the tracked files of the working tree to $TREE"
(cd "$REPO" && git ls-files -z --recurse-submodules | tar --null -T - --ignore-failed-read -cf - 2>/dev/null) \
  | tar -C "$TREE" -xf -
cp "$ROM" "$TREE/baserom.us.z64"

echo "== extracting the ROM (log: $OUT/logs/extract.log)"
tc 'python3 tools/toolchain/patch_splat.py && make extract && make -C conker extract' \
  >"$OUT/logs/extract.log" 2>&1 || die "extraction failed, see the log"

# first_c SEGMENT: the first C file of a code segment in conker/conker.us.yaml (path below conker/src).
first_c() {
  awk -v seg="$1" '
    $1 == "-" && $2 == "name:" { on = ($3 == seg); next }
    on && /\[0x[0-9A-Fa-f]+, *c, *[^]]+\]/ { sub(/.*, *c, */, ""); sub(/\].*/, ""); print; exit }
  ' "$TREE/conker/conker.us.yaml"
}

PAD='
void shift_test_pad(void) {
}

void shift_test_pad2(void) {
}
'
declare -A PADFILE
for v in "${VARIANTS[@]}"; do
  [ "$v" = base ] && continue
  f=$(first_c "$v")
  [ -n "$f" ] && [ -f "$TREE/conker/src/$f.c" ] || die "no first C file found for the $v segment"
  PADFILE[$v]="src/$f.c"
  cp "$TREE/conker/src/$f.c" "$OUT/pristine.$v.c"
done

for v in "${VARIANTS[@]}"; do
  for w in "${!PADFILE[@]}"; do cp "$OUT/pristine.$w.c" "$TREE/conker/${PADFILE[$w]}"; done
  if [ "$v" != base ]; then
    printf '%s' "$PAD" >>"$TREE/conker/${PADFILE[$v]}"
    echo "== building $v: two empty functions added to conker/${PADFILE[$v]}"
  else
    echo "== building base"
  fi
  tc 'set -e
      make -C conker -j"$(nproc)" NON_MATCHING=1
      make -C conker replace NON_MATCHING=1
      rm -f build/conker.us.z64
      make -j"$(nproc)" NON_MATCHING=1
      python3 tools/n64crc.py build/conker.us.z64 --check
      mips-linux-gnu-nm conker/build/conker.us.elf' >"$OUT/logs/build-$v.log" 2>&1 \
    || die "the $v build failed, see $OUT/logs/build-$v.log"
  grep -E '^[0-9a-f]{8} [TtWw] ' "$OUT/logs/build-$v.log" | sort -k3 >"$OUT/syms/$v.txt"
  cp "$TREE/build/conker.us.z64" "$OUT/roms/$v.z64"
done
for w in "${!PADFILE[@]}"; do cp "$OUT/pristine.$w.c" "$TREE/conker/${PADFILE[$w]}"; done

emu() {  # emu VARIANT MODE
  local shots="$OUT/shots/$1-m$2"
  mkdir -p "$shots"
  timeout $((EMU_TIMEOUT + 120)) "$ENGINE" run --rm --network none "${USERARGS[@]}" \
    -v "$shots:/out" -v "$OUT/roms/$1.z64:/rom/rom.z64:ro" "$EMU_IMAGE" bash -c \
    "mkdir -p /tmp/cfg && timeout $EMU_TIMEOUT xvfb-run -a -s '-screen 0 640x480x24' ./mupen64plus \
       --nosaveoptions --corelib ./libmupen64plus.so.2 --plugindir . --datadir . --configdir /tmp/cfg \
       --gfx mupen64plus-video-rice.so --rsp mupen64plus-rsp-hle.so --audio dummy --input dummy \
       --emumode $2 --resolution 320x240 --set Core[ScreenshotPath]=/out --testshots $FRAMES /rom/rom.z64" \
    >"$OUT/logs/emu-$1-m$2.log" 2>&1 || true
}

echo "== booting ${#VARIANTS[@]} ROM(s) in modes $MODES, screenshots at frames $FRAMES ($JOBS at a time)"
for v in "${VARIANTS[@]}"; do
  for m in $MODES; do
    while [ "$(jobs -rp | wc -l)" -ge "$JOBS" ]; do wait -n || true; done
    emu "$v" "$m" &
  done
done
wait

base_sha=$(cut -c1-40 "$REPO/conker.us.sha1")
fail=0
{
  printf '%-9s %-12s %-8s %-22s %s\n' VARIANT ROM_SHA1 MOVED PAD_ADDRESS SCREENSHOTS
  for v in "${VARIANTS[@]}"; do
    sha=$(sha1sum "$OUT/roms/$v.z64" | cut -c1-40)
    moved=-
    [ -f "$OUT/syms/base.txt" ] && moved=$(awk 'NR == FNR { a[$3] = $1; next } ($3 in a) && a[$3] != $1 { n++ }
                                               END { print n + 0 }' "$OUT/syms/base.txt" "$OUT/syms/$v.txt")
    pad=$(awk '$3 == "shift_test_pad" { print $1 }' "$OUT/syms/$v.txt")
    note=""
    if [ "$v" = base ]; then
      if [ "$sha" = "$base_sha" ]; then note=" (matches the original ROM)"; else note=" (DIFFERS from the original ROM)"; fail=1; fi
    elif [ -z "$pad" ]; then
      fail=1; pad=MISSING
    fi
    runs=""
    for m in $MODES; do
      n=$(find "$OUT/shots/$v-m$m" -name '*.png' | wc -l)
      [ "$n" -eq "$NSHOTS" ] || fail=1
      runs+="mode $m: $n/$NSHOTS  "
    done
    printf '%-9s %-12s %-8s %-22s %s%s\n' "$v" "${sha:0:12}" "$moved" "${pad:--}" "$runs" "$note"
  done
  echo "MOVED: symbols whose address differs from the base build. PAD_ADDRESS: the address of shift_test_pad."
  if [ "$fail" = 0 ]; then echo "RESULT: PASS"; else echo "RESULT: FAIL"; fi
  echo "time: $((SECONDS / 60)) min $((SECONDS % 60)) s"
} >"$OUT/summary.txt"
cat "$OUT/summary.txt"

# One row of screenshots for each run, to look at by eye.
"$ENGINE" run --rm --network none "${USERARGS[@]}" -v "$OUT:/st" -w /st/shots "$EMU_IMAGE" bash -c \
  'set -- */*.png; [ -e "$1" ] && montage -font /opt/m64p/font.ttf -label "%d" -geometry 160x120+2+2 -tile '"$NSHOTS"'x */*.png /st/montage.png' \
  >"$OUT/logs/montage.log" 2>&1 && echo "screenshots: $OUT/montage.png" || true
exit "$fail"
