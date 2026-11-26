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


SPLAT_DATA = SPLAT_DIS.parent.parent / "segtypes" / "common" / "data.py"
DATA_ANCHOR = ("                # Implied C file\n"
               "                return options.opts.src_path / self.dir / f\"{self.name}.c\"\n")
DATA_PATCH = ("                # Implied C file, or the hand-written asm file of the same name (a data\n"
              "                # subsegment in another segment than its code, e.g. init_data for init)\n"
              "                asm = options.opts.src_path / self.dir / f\"{self.name}.s\"\n"
              "                if asm.exists():\n"
              "                    return asm\n"
              "                return options.opts.src_path / self.dir / f\"{self.name}.c\"\n")


def patch_data_out_path():
    """A named data subsegment whose code is a hand-written .s file links that file's object."""
    s = SPLAT_DATA.read_text()
    if "hand-written asm file of the same name" in s:
        return
    if DATA_ANCHOR not in s:
        sys.exit("data.py anchor not found; n64splat layout changed")
    SPLAT_DATA.write_text(s.replace(DATA_ANCHOR, DATA_PATCH, 1))
    print("  patched", SPLAT_DATA)


def main():
    patch_data_out_path()
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
