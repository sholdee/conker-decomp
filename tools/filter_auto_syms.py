#!/usr/bin/env python3
"""filter_auto_syms.py — drop splat auto-symbol entries that committed sources already define.

splat lists, in undefined_funcs_auto.txt / undefined_syms_auto.txt, symbols that its own asm references
but its symbol table lacks. For labels that a committed conker/src/*.s file defines with `glabel`, that
entry is an absolute linker-script assignment which overrides the real label: the bytes are the same,
but the ELF loses the function symbol (tools/accounting.py then cannot split there). The Makefile runs
this after every splat run so that every checkout links the same symbol set.
"""
import re
from pathlib import Path

import subprocess
import sys
args = [a for a in sys.argv[1:] if not a.startswith("--")]
M = Path(args[0]).resolve() if args else Path(__file__).resolve().parent.parent / "conker"
defs = set()
if "--linked" in sys.argv:
    # Link-time mode (the Makefile runs it just before ld): the linker script names every linked
    # (object, section) pair, e.g. build/src/game_X.c.o(.rodata). A global that such a section
    # defines must not also get an absolute assignment, which would override it.
    ld_text = (M / "conker.ld").read_text()
    pairs = set(re.findall(r"(build/\S+?\.o)\((\.\w+)\)", ld_text))
    objs = sorted({o for o, _ in pairs if (M / o).exists()})
    want = {(o, sec) for o, sec in pairs}
    for i in range(0, len(objs), 400):
        out = subprocess.run(["mips-linux-gnu-objdump", "-t", *objs[i:i + 400]], cwd=M, capture_output=True, text=True).stdout
        cur = None
        for line in out.splitlines():
            m = re.match(r"^(\S+\.o):\s+file format", line)
            if m:
                cur = m.group(1)
                continue
            m = re.match(r"^[0-9a-f]{8} g\s+\S*\s+(\.\w+)\s+[0-9a-f]{8}\s+(\S+)$", line)
            if m and cur and (cur, m.group(1)) in want:
                defs.add(m.group(2))
# labels in committed hand-written asm and in splat's generated data/bss files (both are linked)
ld = (M / "conker.ld").read_text() if (M / "conker.ld").exists() else ""
linked = [M / "asm" / "data" / f for f in set(re.findall(r"build/asm/data/([\w.]+\.s)\.o", ld))]
for p in list((M / "src").rglob("*.s")) + [q for q in linked if q.exists()]:
    defs |= set(re.findall(r"^\s*(?:glabel|dlabel|jlabel) (\w+)", p.read_text(errors="replace"), re.M))
for p in (M / "src").rglob("*.c"):  # C function definitions
    defs |= set(re.findall(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{", p.read_text(errors="replace"), re.M))
for name in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt"):
    f = M / name
    if not f.exists():
        continue
    lines = f.read_text().splitlines(True)
    keep = [l for l in lines if not ((m := re.match(r"\s*(\w+)\s*=", l)) and m.group(1) in defs)]
    if len(keep) != len(lines):
        f.write_text("".join(keep))
        print(f"{name}: dropped {len(lines) - len(keep)} entries defined by {'linked objects' if '--linked' in sys.argv else 'src'}")
