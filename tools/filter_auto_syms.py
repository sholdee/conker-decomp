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

import sys
M = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path(__file__).resolve().parent.parent / "conker"
defs = set()
for p in (M / "src").rglob("*.s"):
    defs |= set(re.findall(r"^\s*glabel (\w+)", p.read_text(errors="replace"), re.M))
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
        print(f"{name}: dropped {len(lines) - len(keep)} entries defined in src")
