#!/usr/bin/env python3
"""linkcheck.py — check that the linked ELF is fully linked (run after a build).

    linkcheck.py [CONKER_DIR] [--list]

1. Address names: every symbol named for an address (D_XXXXXXXX, func_XXXXXXXX, jtbl_XXXXXXXX) must
   be at that address. The ROM sha1 cannot see .bss, so this catches a misplaced bss label.
2. Pinned addresses: an absolute (SHN_ABS) symbol whose value falls inside a linked section (code,
   data, rodata or bss) pins that address. It would not move with its section. The goal is zero.
   Absolute symbols outside every section (hardware registers, fixed RAM regions, ROM offsets) are
   counted separately; they are part of the memory map, not of any object.
Exit status 1 if a name is at the wrong address.
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
    """Every symbol some input object leaves undefined (that is, references)."""
    objs = " ".join(str(p.relative_to(conker)) for p in (conker / "build").rglob("*.o"))
    cmd = f"mips-linux-gnu-nm -u {objs}"
    try:
        out = subprocess.run(cmd, shell=True, cwd=conker, capture_output=True, text=True)
    except FileNotFoundError:
        out = None
    if out is None or out.returncode not in (0, 1) or not out.stdout:
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        import mw
        out = mw.run(cmd, conker, timeout=1800)
    return {l.split()[-1] for l in out.stdout.splitlines() if l.strip().startswith("U ")}


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

    def inside(v):
        return next((s[2] for s in secs if s[0] <= v < s[1]), None)

    wrong, pinned, fixed = [], {}, 0
    seen = set()
    for name, value, ndx in syms:
        if (name, value) in seen:
            continue
        seen.add((name, value))
        m = ADDR_NAME.match(name)
        if m and int(m.group(1), 16) != value:
            wrong.append(f"{name} is at 0x{value:08X}")
        if ndx == "ABS" and name in used and not LINKER_MADE.search(name):
            sec = inside(value)
            if sec:
                pinned.setdefault(sec, []).append(name)
            else:
                fixed += 1
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
    sys.exit(1 if wrong else 0)


if __name__ == "__main__":
    main()
