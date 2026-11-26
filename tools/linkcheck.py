#!/usr/bin/env python3
"""linkcheck.py — check that the linked ELF is fully linked (run after a build).

    linkcheck.py [CONKER_DIR] [--list]

1. Address names: every symbol named for an address (D_XXXXXXXX, func_XXXXXXXX, jtbl_XXXXXXXX) must
   be at that address. The ROM sha1 cannot see .bss, so this catches a misplaced bss label.
2. Pinned addresses: an absolute (SHN_ABS) symbol whose value falls inside a linked section (code,
   data, rodata or bss) pins that address. It would not move with its section. The goal is zero.
   Absolute symbols outside every section (hardware registers, fixed RAM regions, ROM offsets) are
   counted separately; they are part of the memory map, not of any object.
   IPL3 loads ROM offset X to 0x80000000 + X, and the TLB shows the same bytes at 0x10000000 + X, so
   init and init_data each have other views (TLB, KSEG0, KSEG1). An absolute symbol in another view of
   one of them is pinned too: the boot code jumps to init code through its KSEG0 alias before the TLB
   is set up.
Exit status 1 if a name is at the wrong address or any address is pinned.
"""
import re, struct, subprocess, sys
from pathlib import Path

ADDR_NAME = re.compile(r"^(?:D|func|jtbl)_([0-9A-F]{8})$")


def read_symbols(elf):
    try:
        out = subprocess.run(["mips-linux-gnu-readelf", "-W", "-s", "-S", str(elf)], capture_output=True, text=True)
    except FileNotFoundError:  # on the host the toolchain lives in the container
        out = None
    if out is None or out.returncode:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import mw
        out = mw.run(f"mips-linux-gnu-readelf -W -s -S {elf}", Path(elf).parent)
    text = out.stdout
    secs = []
    for m in re.finditer(r"^\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]{8})\s+[0-9a-f]+\s+([0-9a-f]+)\s+\S+\s+(\S*)", text, re.M):
        name, typ, addr, size, flags = m.groups()
        # .header and .boot are ROM blobs with placeholder addresses (0x0-0x1000; IPL3 runs at
        # 0xA4000040): small numbers there (asset ids, sizes) are constants, not pinned addresses
        if "A" in flags and int(size, 16) and name not in (".header", ".boot"):
            secs.append((int(addr, 16), int(addr, 16) + int(size, 16), name))
    syms = []
    for m in re.finditer(r"^\s*\d+:\s+([0-9a-f]{8})\s+\d+\s+(\w+)\s+(\w+)\s+\w+\s+(\w+)\s+(\S+)$", text, re.M):
        value, typ, bind, ndx, name = m.groups()
        syms.append((name, int(value, 16), ndx))
    return secs, syms


LINKER_MADE = re.compile(r"_(ROM_START|ROM_END|VRAM|VRAM_END|DATA_START|DATA_END|DATA_SIZE|TEXT_START|TEXT_END|TEXT_SIZE|"
                         r"BSS_START|BSS_END|BSS_SIZE|RODATA_START|RODATA_END|RODATA_SIZE)$|^_binary_|\.o$|\.c$|\.s$")


def referenced(conker):
    """Every symbol that a relocation in a LINKED object refers to. Objects come from the linker
    script, so stale objects left in build/ are ignored. Relocations, not `nm -u`, because IDO -g3
    writes every declared extern into each object's symbol table even when nothing uses it."""
    ld = (conker / "conker.ld").read_text()
    objs = sorted(set(re.findall(r"(build/\S+?\.o)\(", ld)))
    objs = [o for o in objs if (conker / o).exists()]
    cmd = "mips-linux-gnu-objdump -r " + " ".join(objs)
    try:
        out = subprocess.run(cmd, shell=True, cwd=conker, capture_output=True, text=True)
    except FileNotFoundError:
        out = None
    if out is None or out.returncode or not out.stdout:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import mw
        out = mw.run(cmd, conker, timeout=1800)
    names = set()
    for line in out.stdout.splitlines():
        m = re.match(r"^[0-9a-f]{8}\s+R_MIPS_\w+\s+([^\s+]+)", line)
        if m:
            names.add(m.group(1))
    return names


def unrelocated_jumps(conker):
    """5. Absolute jumps (`jal`/`j`) in linked code without an R_MIPS_26 relocation. Such a jump is a
    raw instruction word (for example `.word 0x0D42A9DE` in hand-written asm): it matches the ROM, but
    it keeps its old target when the code moves, so the build is no longer shiftable."""
    ld = (conker / "conker.ld").read_text()
    objs = sorted(set(re.findall(r"(build/\S+?\.o)\(\.text\)", ld)))
    objs = [o for o in objs if (conker / o).exists()]
    cmd = "mips-linux-gnu-objdump -dr --no-show-raw-insn -j .text " + " ".join(objs)
    try:
        out = subprocess.run(cmd, shell=True, cwd=conker, capture_output=True, text=True)
    except FileNotFoundError:
        out = None
    if out is None or out.returncode or not out.stdout:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import mw
        out = mw.run(cmd, conker, timeout=1800)
    bad, obj, pending = [], None, None
    for line in out.stdout.splitlines():
        m = re.match(r"^(\S+\.o):\s+file format", line)
        if m:
            obj = m.group(1)
            continue
        if pending:
            if not re.match(rf"^\s+{pending[0]}: R_MIPS_26\b", line):
                bad.append(f"{obj} +0x{pending[0]}: {pending[1]} without a relocation")
            pending = None
        m = re.match(r"^\s+([0-9a-f]+):\s+(jal|j)\s+(\S+)", line)
        if m:
            pending = (m.group(1), f"{m.group(2)} {m.group(3)}")
    return bad


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    conker = Path(args[0]) if args else Path(__file__).resolve().parent.parent / "conker"
    elf = conker / "build" / "conker.us.elf"
    if not elf.exists():
        sys.exit(f"{elf}: not built")
    secs, syms = read_symbols(elf)
    if not syms:
        sys.exit("no symbols read from the ELF")
    secs.sort()
    used = referenced(conker)

    # the other views of the sections that IPL3 loads at 0x80000000 + ROM offset (see the docstring)
    views = []
    for lo, hi, name in secs:
        if name == ".init" or name.startswith(".init_data"):
            base = lo & 0xF0000000
            for other in (0x10000000, 0x80000000, 0xA0000000):
                if other != base:
                    views.append((lo - base + other, hi - base + other, f"{name} ({other >> 28:X}xxxxxxx view)"))

    def inside(v):
        return next((s[2] for s in secs + views if s[0] <= v < s[1]), None)

    # A hand-written definition such as `rspbootTextStart = D_800290D0 - 0x70000000;` is derived from a linked
    # symbol, so it moves with it. ld marks it ABS (it is assigned outside a section), but it is not pinned.
    derived = set()
    for hand in ("undefined_syms.us.txt", "undefined_funcs.us.txt"):
        if (conker / hand).exists():
            text = re.sub(r"/\*.*?\*/", "", (conker / hand).read_text(), flags=re.S)
            for m in re.finditer(r"^\s*(\w+)\s*=\s*([^;]+);", text, re.M):
                if re.search(r"[A-Za-z_]\w*", re.sub(r"\b0x[0-9A-Fa-f]+\b", "", m.group(2))):
                    derived.add(m.group(1))
    wrong, pinned, fixed = [], {}, 0
    seen = set()
    for name, value, ndx in syms:
        if (name, value) in seen:
            continue
        seen.add((name, value))
        m = ADDR_NAME.match(name)
        if m and int(m.group(1), 16) != value:
            wrong.append(f"{name} is at 0x{value:08X}")
        if ndx == "ABS" and name in used and not LINKER_MADE.search(name) and name not in derived:
            sec = inside(value)
            if sec:
                pinned.setdefault(sec, []).append(name)
            else:
                fixed += 1
    # 4. Raw pointers: a data word in a linked asm data file that equals a symbol's start address is a
    #    pointer written as a number, so it would not move with its target (the relocation check above
    #    cannot see it). Register the target in symbol_addrs.us.txt so splat writes it by name.
    starts = {}
    for name, value, ndx in syms:
        if ndx not in ("ABS", "UND") and not name.startswith("$") and inside(value):
            starts.setdefault(value, name)
    raw = []
    for f in sorted(set(re.findall(r"build/(asm/data/[\w.]+\.s)\.o", (conker / "conker.ld").read_text()))):
        if not (conker / f).exists():
            continue
        for m in re.finditer(r"\.word\s+0x([0-9A-Fa-f]{8})\b", (conker / f).read_text()):
            v = int(m.group(1), 16)
            if v in starts:
                raw.append(f"{f}: .word 0x{v:08X} is the address of {starts[v]}")
    for r in raw[:20]:
        print("RAW POINTER:", r)
    print(f"raw pointers in linked data: {len(raw)}")
    wrong += raw
    jumps = unrelocated_jumps(conker)
    for j in jumps[:20]:
        print("RAW JUMP:", j)
    print(f"absolute jumps without a relocation: {len(jumps)}")
    wrong += jumps
    # 3. ROM layout: the code's fixed ROM offsets must equal where the outer link put the segments
    #    (the game segment has a reserved size, so this holds unless a reservation overflows)
    layout = {"game_us_rzip_ROM_START": "game_us_rzip_ROM_START", "debugger_us_ROM_START": "debugger_us_ROM_START",
              "compressed_ROM_START": "compressed_ROM_START", "ucode_ROM_START": "_2D4B0_ROM_START",
              "assets_offsets_table_ROM_START": "assets_offsets_table_ROM_START",
              "font_ROM_START": "font_ROM_START"}
    outer_map = conker.parent / "build" / "conker.us.map"
    if outer_map.exists():
        outer = {m.group(2): int(m.group(1), 16) for m in
                 re.finditer(r"^\s+0x([0-9a-f]+)\s+(\w+_ROM_START) = __romPos", outer_map.read_text(), re.M)}
        inner = {n: v for n, v, _ in syms}
        for code_name, link_name in layout.items():
            if code_name in inner and link_name in outer and inner[code_name] != outer[link_name]:
                wrong.append(f"{code_name} is 0x{inner[code_name]:X} but the ROM has {link_name} at 0x{outer[link_name]:X}")
        print(f"ROM layout constants checked against the outer link: {sum(1 for c, l in layout.items() if c in inner and l in outer)}")
    for w in wrong[:50]:
        print("WRONG ADDRESS:", w)
    total = sum(len(v) for v in pinned.values())
    print(f"pinned addresses inside linked sections: {total}")
    for sec, names in sorted(pinned.items(), key=lambda kv: -len(kv[1])):
        print(f"  {sec:12s} {len(names):6d}  e.g. {', '.join(names[:4])}")
        if "--list" in sys.argv:
            print("    " + " ".join(names))
    print(f"referenced absolute symbols outside every section (memory map): {fixed}")
    print(f"address-named symbols at the wrong address: {len(wrong)}")
    if total:
        print("NOT FULLY LINKED: the pinned addresses above must be reached through their sections")
    sys.exit(1 if wrong or total else 0)


if __name__ == "__main__":
    main()
