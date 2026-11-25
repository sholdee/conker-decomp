#!/usr/bin/env python3
"""Patch the tools/n64splat submodule for spimdisasm >= 1.42 (idempotent).

This old splat predates spimdisasm 1.42's nonmatching/enddlabel marker labels and never
disables them, so extraction emits markers that break the assembler and asm-processor.
`git submodule update` reverts the patch, so run this again after every submodule update.
"""
import sys
from pathlib import Path

SPLAT_DIS = (Path(__file__).resolve().parent.parent / "n64splat" / "src" / "splat" /
             "disassembler" / "spimdisasm_disassembler.py")
ANCHOR = ("        spimdisasm.common.GlobalConfig.ASM_EHTBL_LABEL = (\n"
          "            options.opts.asm_ehtable_label_macro\n"
          "        )\n")
PATCH = ANCHOR + ('        spimdisasm.common.GlobalConfig.ASM_NM_LABEL = ""\n'
                  '        spimdisasm.common.GlobalConfig.ASM_DATA_END_LABEL = ""\n')


def main():
    if not SPLAT_DIS.exists():
        sys.exit(f"{SPLAT_DIS}: missing (git submodule update --init first)")
    s = SPLAT_DIS.read_text()
    if 'ASM_NM_LABEL = ""' in s:
        print("  already patched")
        return
    if ANCHOR not in s:
        sys.exit("anchor not found; n64splat layout changed")
    SPLAT_DIS.write_text(s.replace(ANCHOR, PATCH, 1))
    print("  patched", SPLAT_DIS)


if __name__ == "__main__":
    main()
