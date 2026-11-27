#!/usr/bin/env python3
"""manifest_cmp.py — check that a committed accounting manifest agrees with a freshly computed one.

    manifest_cmp.py COMMITTED.tsv COMPUTED.tsv

The gate runs the trusted tools/accounting.py into a scratch file and compares it with the committed
tools/accounting.tsv. Two things must agree:
- the coverage: which address ranges are matched C, hand-written asm or padding;
- the name and size of every row that both manifests have at the same address (the progress figures
  count names from the committed manifest).
Rows that only one side has, and the proof column, can change with the checker's version (for example
when it learns to cut at another kind of function start); the checker's exit code already requires a
proof for every hand-written piece. Exit status 1 on a difference.
"""
import sys


def ranges(lines):
    """Merged (start, end, origin) ranges of a manifest."""
    rows = []
    for line in lines:
        if not line.strip() or line.startswith("#"):
            continue
        f = line.rstrip("\n").split("\t")
        rows.append((int(f[1], 16), int(f[2]), f[4]))
    rows.sort()
    out = []
    for start, size, origin in rows:
        if out and out[-1][2] == origin and out[-1][1] == start:
            out[-1][1] = start + size
        else:
            out.append([start, start + size, origin])
    return [tuple(r) for r in out]


def rows(lines):
    out = {}
    for line in lines:
        if line.strip() and not line.startswith("#"):
            f = line.rstrip("\n").split("\t")
            out[int(f[1], 16)] = (f[0], int(f[2]))
    return out


def main():
    # each file is read once, so that a pipe (bash process substitution) works too
    la, lb = open(sys.argv[1]).read().splitlines(True), open(sys.argv[2]).read().splitlines(True)
    a, b = ranges(la), ranges(lb)
    ra, rb = rows(la), rows(lb)
    bad = [(k, ra[k], rb[k]) for k in sorted(set(ra) & set(rb)) if ra[k] != rb[k]]
    for k, x, y in bad[:10]:
        print(f"row 0x{k:08X}: committed {x[0]} ({x[1]} bytes), computed {y[0]} ({y[1]} bytes)")
    if bad:
        sys.exit(f"{len(bad)} rows differ: run tools/accounting.py and commit tools/accounting.tsv")
    if a == b:
        print(f"manifest coverage agrees: {len(a)} ranges, {len(set(ra) & set(rb))} rows")
        return
    for x, y in zip(a, b):
        if x != y:
            print(f"first difference: committed 0x{x[0]:08X}-0x{x[1]:08X} {x[2]}, "
                  f"computed 0x{y[0]:08X}-0x{y[1]:08X} {y[2]}")
            break
    else:
        print(f"committed has {len(a)} ranges, computed has {len(b)}")
    sys.exit("the committed tools/accounting.tsv does not match the build: run tools/accounting.py and commit it")


if __name__ == "__main__":
    main()
