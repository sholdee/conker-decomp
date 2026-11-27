#!/usr/bin/env python3
"""ro_migrate.py — give C files their own constant data (.rodata) back.

Today most C objects' .rodata is discarded at link and tools/rodata_extern.py points every literal
reference at the original ROM address instead. That pins addresses. A C file whose own .rodata equals
its slice of the ROM can link its .rodata directly: the yaml names the slice `[0xSTART, .rodata, file]`
and rodata_extern then leaves the object alone (it also trims IDO's 16-byte padding to the slice).

    ro_migrate.py plan [--json OUT]          find every C object's slice in the ROM (needs a build)
    ro_migrate.py apply PLAN.json [FILE...]  rewrite conker.us.yaml for those files (default: all `ok`)
    ro_migrate.py check FILE                 word-by-word: the object's .rodata vs its ROM window (between
                                             the neighbouring files' slices)
    ro_migrate.py carve FILE START END       give FILE the slice [START, END) in the yaml
  --root DIR   work on a workspace (e.g. $CONKER_WS/clean.game_X/conker) instead of the main tree

`plan` compares each object's .rodata (relocated words masked, trailing padding stripped) with the
original rodata regions of the yaml, at 4-byte alignment, and picks the hit that keeps slices in text
order and non-overlapping. Only `ok` rows are applied. The whole-ROM sha1 remains the judge: run splat,
`make -B`, and check after `apply`.
"""
import json, re, struct, sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
M = REPO / "conker"
if "--root" in sys.argv:  # operate on a workspace's conker/ instead of the main tree
    M = Path(sys.argv[sys.argv.index("--root") + 1]).resolve()
    del sys.argv[sys.argv.index("--root"):sys.argv.index("--root") + 2]
YAML = M / "conker.us.yaml"
sys.path.insert(0, str(REPO / "tools" / "asm-processor"))
from asm_processor import ElfFile  # noqa: E402

ENTRY = re.compile(r"^(\s*)- \[(0x[0-9A-Fa-f]+),\s*(\.?\w+)(?:,\s*([\w/]+))?\](.*)$")


def entries():
    """Every subsegment line: (line index, offset, type, name)."""
    out = []
    for i, line in enumerate(YAML.read_text().splitlines()):
        m = ENTRY.match(line)
        if m:
            out.append((i, int(m.group(2), 16), m.group(3), m.group(4)))
    return out


def segments():
    """Top-level yaml segments: [(name, rom start, vram)] in file order."""
    segs, cur = [], None
    for line in YAML.read_text().splitlines():
        m = re.match(r"^  - name:\s*(\S+)", line)
        if m:
            cur = [m.group(1), None, None]
            segs.append(cur)
        elif cur and re.match(r"^    start:", line):
            cur[1] = int(line.split(":")[1].split("#")[0], 16)
        elif cur and re.match(r"^    vram:", line):
            cur[2] = int(line.split(":")[1].split("#")[0], 16)
    return [s for s in segs if s[1] is not None]


# each code segment's constant data lives in this window of the bin
WINDOWS = {"init": ("init_data",), "game": ("game_data",), "debugger": ("debugger",)}


def rodata_regions(ents):
    """[start, end) ROM ranges covered by rodata subsegments (anonymous or owned)."""
    regs = []
    for k, (_, off, typ, _) in enumerate(ents):
        if typ in ("rodata", ".rodata"):
            end = ents[k + 1][1] if k + 1 < len(ents) else off
            if regs and regs[-1][1] == off:
                regs[-1][1] = end
            else:
                regs.append([off, end])
    return regs


def own_rodata(obj, text_vram):
    """The object's .rodata with every R_MIPS_32 against its own .text resolved to the final address
    (jump tables); other relocated words are returned as wildcards."""
    import time
    for attempt in range(20):  # a concurrent land may be rewriting the object
        try:
            e = ElfFile(obj.read_bytes())
            break
        except Exception:
            time.sleep(1)
    else:
        raise RuntimeError(f"cannot read {obj}")
    ro = next((s for s in e.sections if s.name == ".rodata"), None)
    if ro is None or not ro.data:
        return None, set()
    data = bytearray(ro.data)
    text_idx = next((i for i, s in enumerate(e.sections) if s.name == ".text"), None)
    symtab = next((s for s in e.sections if s.name == ".symtab"), None)
    rel = set()
    for s in e.sections:
        if s.name == ".rel.rodata":
            for i in range(0, len(s.data), 8):
                off, info = struct.unpack(">II", s.data[i:i + 8])
                sym, typ = info >> 8, info & 0xFF
                shndx = struct.unpack(">H", symtab.data[sym * 16 + 14:sym * 16 + 16])[0] if symtab else None
                stype = symtab.data[sym * 16 + 12] & 0xF if symtab else None
                if typ == 2 and shndx == text_idx and stype == 3 and text_vram is not None:  # STT_SECTION .text
                    addend = struct.unpack(">I", data[off:off + 4])[0]
                    data[off:off + 4] = struct.pack(">I", text_vram + addend)
                else:
                    rel.add(off // 4)
    return bytes(data), rel


def plan():
    rom = (M / "conker.us.bin").read_bytes()
    ents = entries()
    regs = rodata_regions(ents)
    owned = {n for _, _, t, n in ents if t == ".rodata"}
    segs = segments()
    starts = {n: (a, v) for n, a, v in segs}
    seg_of = lambda off: max((s for s in segs if s[1] <= off), key=lambda s: s[1])
    def window(code_seg):
        names = WINDOWS.get(code_seg, ())
        out = []
        for n in names:
            a = starts[n][0]
            later = [s[1] for s in segs if s[1] > a]
            b = min(later) if later else len(rom)
            out += [[max(x, a), min(y, b)] for x, y in regs if x < b and y > a]
        return out
    rows = []
    cfiles = [(off, name) for _, off, typ, name in ents if typ == "c" and name]
    for text_off, name in sorted(cfiles):
        obj = M / "build" / "src" / f"{name}.c.o"
        if not obj.exists():
            continue
        seg = seg_of(text_off)
        data, rel = own_rodata(obj, seg[2] + text_off - seg[1] if seg[2] else None)
        if data is None:
            continue
        n = len(data) // 4
        while n and data[4 * n - 4:4 * n] == b"\0\0\0\0" and (n - 1) not in rel:
            n -= 1
        row = {"file": name, "text": hex(text_off), "size": 4 * n}
        if name in owned:
            row["status"] = "owned"
            rows.append(row)
            continue
        if n == 0:
            row["status"] = "empty"
            rows.append(row)
            continue
        pat = re.compile(b"".join(b"...." if i in rel else re.escape(data[4 * i:4 * i + 4]) for i in range(n)), re.S)
        hits = [h for a, b in window(seg[0]) for h in (m.start() for m in pat.finditer(rom, a, b))
                if h % 4 == 0 and h + 4 * n <= b]
        row["hits"] = [hex(h) for h in hits[:8]]
        row["seg"], row["_hits"] = seg[0], hits
        row["status"] = "unique" if len(hits) == 1 else ("multi" if hits else "none")
        rows.append(row)
    # pass 2: unique hits must keep text order; a multi hit is placed between its unique neighbours
    for segname in {r.get("seg") for r in rows if r.get("seg")}:
        rs = [r for r in rows if r.get("seg") == segname and r["status"] in ("unique", "multi")]
        uniq = [r for r in rs if r["status"] == "unique"]
        prev = -1
        for r in uniq:  # drop uniques that break monotonic order (a later file's data before an earlier's)
            h = r["_hits"][0]
            r["status"] = "ok" if h > prev else "order"
            if h > prev:
                prev = h + r["size"] - 1
        okd = [r for r in rs if r["status"] == "ok"]
        for i, r in enumerate(rs):
            if r["status"] != "multi":
                continue
            lo = max([o["_hits"][0] + o["size"] for o in okd if rs.index(o) < i], default=0)
            hi = min([o["_hits"][0] for o in okd if rs.index(o) > i], default=1 << 40)
            cand = [h for h in r["_hits"] if lo <= h and h + r["size"] <= hi]
            if len(cand) == 1:
                r["status"], r["_hits"] = "ok", cand
            else:
                r["status"] = "multi" if cand else "order"
    for r in rows:
        if r["status"] == "ok":
            r["start"], r["end"] = hex(r["_hits"][0]), hex(r["_hits"][0] + r["size"])
        r.pop("_hits", None)
    return rows


def apply(plan_rows, only):
    lines = YAML.read_text().splitlines()
    todo = [r for r in plan_rows if r.get("status") == "ok" and (not only or r["file"] in only)]
    for r in sorted(todo, key=lambda r: int(r["start"], 16), reverse=True):
        start, end = int(r["start"], 16), int(r["end"], 16)
        ents = [(i, int(ENTRY.match(l).group(2), 16), ENTRY.match(l).group(3)) for i, l in enumerate(lines) if ENTRY.match(l)]
        # the rodata entry containing start, and the entries inside [start, end)
        cont = max((e for e in ents if e[1] <= start and e[2] == "rodata"), key=lambda e: e[1], default=None)
        nxt = [e for e in ents if e[1] > cont[1]] if cont else []
        if cont is None or (nxt and nxt[0][1] < start):
            sys.exit(f"{r['file']}: no anonymous rodata entry contains {start:#x}")
        inside = [e for e in nxt if start < e[1] < end]
        if any(e[2] != "rodata" for e in inside):
            sys.exit(f"{r['file']}: slice {start:#x}-{end:#x} crosses a non-anonymous entry")
        indent = ENTRY.match(lines[cont[0]]).group(1)
        after_slice = [e for e in nxt if e[1] >= end]
        new = [f"{indent}- [0x{start:X}, .rodata, {r['file']}]"]
        if not after_slice or after_slice[0][1] != end:
            new.append(f"{indent}- [0x{end:X}, rodata]")
        # drop entries strictly inside the slice; keep the containing one only if it starts before the slice
        drop = {e[0] for e in inside}
        if cont[1] == start:
            drop.add(cont[0])
        insert_at = (inside[-1][0] if inside else cont[0]) + 1
        lines = [l for i, l in enumerate(lines[:insert_at]) if i not in drop] + new + \
                [l for i, l in enumerate(lines[insert_at:], insert_at) if i not in drop]
    YAML.write_text("\n".join(lines) + "\n")
    print(f"applied {len(todo)} slices")


def check(name):
    rom = (M / "conker.us.bin").read_bytes()
    ents = entries()
    segs = segments()
    seg_of = lambda off: max((s for s in segs if s[1] <= off), key=lambda s: s[1])
    text = next((off for _, off, typ, nm in ents if typ == "c" and nm == name), None)
    if text is None:
        sys.exit(f"{name}: no c entry in the yaml")
    seg = seg_of(text)
    data, rel = own_rodata(M / "build" / "src" / f"{name}.c.o", seg[2] + text - seg[1])
    if data is None:
        sys.exit(f"{name}: the object has no .rodata")
    n = len(data) // 4
    while n and data[4 * n - 4:4 * n] == b"\0\0\0\0" and (n - 1) not in rel:
        n -= 1
    # window: from the end of the previous C file's owned slice to the start of the next one
    owned = [(off, nm) for _, off, typ, nm in ents if typ == ".rodata"]
    texts = {nm: off for _, off, typ, nm in ents if typ == "c"}
    prev = [o for o, nm in owned if texts.get(nm, -1) < text and seg_of(texts.get(nm, 0))[0] == seg[0]]
    nxt = [o for o, nm in owned if texts.get(nm, 1 << 40) > text and seg_of(texts.get(nm, 0))[0] == seg[0]]
    starts = sorted(o for _, o, _, _ in ents)
    lo = max(prev) if prev else None
    if lo is not None:
        lo = next(o for o in starts if o > lo)  # the entry after the previous owned slice
    hi = min(nxt) if nxt else None
    print(f"{name}: own .rodata {4 * n} bytes ({len(rel)} relocated words marked ????)")
    if lo is None or hi is None:
        print("no owned neighbour on one side; window unknown")
        return
    win = rom[lo:hi]
    print(f"ROM window 0x{lo:X}-0x{hi:X} ({hi - lo} bytes). own | rom")
    for k in range(max(n, len(win) // 4)):
        o = "????????" if k in rel else (data[4 * k:4 * k + 4].hex() if k < n else "")
        r = win[4 * k:4 * k + 4].hex() if 4 * k < len(win) else ""
        mark = "" if o == r or o == "????????" else "  <--"
        print(f"  +0x{4 * k:03X} 0x{lo + 4 * k:X}  {o:8s} | {r:8s}{mark}")


def carve(name, start, end):
    row = {"file": name, "status": "ok", "start": hex(start), "end": hex(end)}
    apply([row], {name})


if __name__ == "__main__":
    if len(sys.argv) == 3 and sys.argv[1] == "check":
        check(sys.argv[2]); sys.exit(0)
    if len(sys.argv) == 5 and sys.argv[1] == "carve":
        carve(sys.argv[2], int(sys.argv[3], 16), int(sys.argv[4], 16)); sys.exit(0)
    if len(sys.argv) >= 2 and sys.argv[1] == "plan":
        rows = plan()
        out = sys.argv[sys.argv.index("--json") + 1] if "--json" in sys.argv else None
        if out:
            Path(out).write_text(json.dumps(rows, indent=1))
        from collections import Counter
        print(Counter(r["status"] for r in rows))
    elif len(sys.argv) >= 3 and sys.argv[1] == "apply":
        apply(json.loads(Path(sys.argv[2]).read_text()), set(sys.argv[3:]))
    else:
        sys.exit(__doc__)
