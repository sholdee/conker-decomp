#!/usr/bin/env python3
"""accounting.py — prove that every byte of US CPU code is matched C or hand-written asm.

    accounting.py [--tsv OUT]      (run after a build; needs build/conker.us.map and build/conker.us.elf)

For every function in the init, game and debugger code segments it determines the origin:
  c     compiled from a C file in conker/src
  hasm  committed hand-written assembly (conker/src/*.s), with the proof rule(s) that apply
It exits non-zero if anything is left over: a GLOBAL_ASM stub, an inline GLOBAL_ASM block, an
asm segment outside src/, or a hasm function with no proof.

Proof rules (see AGENTS.md "Hand-written asm"). The rules marked * are computed from the instructions here;
every computed rule was checked to occur 0 times in the 5,746 functions compiled by IDO 5.3.
  a  source-identical      the SDK's own .s, assembled by IDO 5.3, gives the same bytes (tools/hasm_proofs.tsv)
  b* unemittable           cop0/TLB/cache/sync/syscall/eret, 64-bit ops, trapping add/addi/sub, $k0/$k1
  c  uninit-read           dataflow: reads a register that is no ABI input and not written first (hasm_proofs.tsv)
  d* j-or-branch-out       opcode-2 `j`, or a branch that leaves the function
  e* bare-cvt.w            cvt.w.s/cvt.w.d with no ctc1 in the function
  f* frame-layout          store below $sp, ra saved in the argument area or without a frame
  g* no-return             no jr/j: execution falls into the next function
  h* fpr-or-call-abi       odd FPR as an arithmetic operand, or jal without saving ra
  i* mul-nop               6+ mul.s, every one followed by a nop (compiled C peaks at 67%)
  k* same-object           meets a proven function at a non-16-aligned address (IDO pads C .text to 16)
  s  search                no rule applies; no compiler reproduces it from C (tools/hasm_proofs.tsv)
"""
import argparse, re, subprocess, sys, collections
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
M = REPO / "conker"
CODE_SEGS = ("init", "game", "debugger")

HARD = {"mtc0", "mfc0", "tlbr", "tlbwi", "tlbwr", "tlbp", "eret", "cache", "sync", "syscall", "ld", "sd", "ldl",
        "ldr", "sdl", "sdr", "dsll", "dsrl", "dsra", "dsll32", "dsrl32", "dsra32", "daddiu", "daddu", "dsubu",
        "dmult", "dmultu", "ddiv", "ddivu", "dsllv", "dsrlv", "dsrav", "dmfc1", "dmtc1", "addi", "add", "sub"}
FPA = re.compile(r"^(add|sub|mul|div|mov|neg|abs|sqrt)\.[sd]$|^(cvt|trunc|round|floor|ceil)\.\w\.\w$|^c\.\w+\.[sd]$")
STORE = {"sw", "sh", "sb", "swc1", "sdc1", "sd", "swl", "swr"}


def run(cmd):
    r = subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=M)
    if r.returncode and not r.stdout:
        # the toolchain lives in the container
        sys.path.insert(0, str(REPO / "tools"))
        import mw
        return mw.run(cmd, M, timeout=1800).stdout
    return r.stdout


def objects():
    """[(start, end, object path)] for every input .text section, from the link map."""
    out = []
    for line in open(M / "build/conker.us.map"):
        m = re.match(r"^ \.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)", line)
        if m and int(m.group(2), 16):
            out.append((int(m.group(1), 16), int(m.group(1), 16) + int(m.group(2), 16), m.group(3)))
    return sorted(out)


def disassembly():
    """{addr: (op, args)} and {addr: symbol name} for the whole ELF."""
    dis = run("mips-linux-gnu-objdump -d -M no-aliases --no-show-raw-insn -z build/conker.us.elf")
    insns, syms = {}, {}
    for line in dis.splitlines():
        m = re.match(r"^([0-9a-f]{8}) <([^>]+)>:", line)
        if m:
            syms.setdefault(int(m.group(1), 16), m.group(2))
            continue
        m = re.match(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$", line)
        if m:
            insns[int(m.group(1), 16)] = (m.group(2), m.group(3).strip().replace(" ", ""))
    return insns, syms


def functions():
    """Split every object's .text at its symbols: [(name, start, ins, object, object end)].
    Bytes before an object's first symbol become a piece of their own, so coverage is total."""
    insns, syms = disassembly()
    out = []
    for start, end, o in objects():
        cuts = sorted({start} | {a for a, n in syms.items() if start < a < end and not n.startswith(".")})
        for i, a in enumerate(cuts):
            b = cuts[i + 1] if i + 1 < len(cuts) else end
            ins = [(x,) + insns[x] for x in range(a, b, 4) if x in insns]
            if len(ins) * 4 != b - a:
                sys.exit(f"disassembly gap in {o} at 0x{a:08X}")
            out.append((syms.get(a, f"{Path(o).stem}+0x{a - start:X}"), a, ins, o, end))
    return out


def computed_rules(start, ins):
    r, ops, end = set(), [i[1] for i in ins], start + 4 * len(ins)
    if set(ops) & HARD or any(re.search(r"\$?\bk[01]\b", a) for _, _, a in ins):
        r.add("b")  # includes $k0/$k1, which IDO never allocates
    if "j" in ops:
        r.add("d")
    if any(o in ("cvt.w.s", "cvt.w.d") for o in ops) and "ctc1" not in ops:
        r.add("e")
    alloc = any(o == "addiu" and re.match(r"\$?sp,\$?sp,-\d+$", a) for _, o, a in ins)
    for _, o, a in ins:
        if o.startswith("b") and o != "break":
            m = re.search(r"([0-9a-f]{7,8})(<[^>]*>)?$", a)
            if m and not start <= int(m.group(1), 16) < end:
                r.add("d")
        m = re.match(r"(\$?\w+),(-?\d+)\(\$?sp\)$", a)
        if o in STORE and m:
            off = int(m.group(2))
            if off < 0:
                r.add("f")
            if o == "sw" and m.group(1).lstrip("$") == "ra" and ((off < 0x10 and "jal" in ops) or not alloc):
                r.add("f")
        if FPA.match(o) and any(int(x) % 2 for x in re.findall(r"\$f(\d+)", a)):
            r.add("h")
    if ins and not any(o in ("jr", "j") for o in ops):
        r.add("g")
    if "jal" in ops and not any(o == "sw" and re.match(r"\$?ra,", a) for _, o, a in ins):
        r.add("h")
    muls = [i for i, x in enumerate(ins) if x[1] == "mul.s"]
    if len(muls) >= 6 and all(i + 1 < len(ins) and ins[i + 1][1] == "sll" and
                              ins[i + 1][2].replace("$", "") == "zero,zero,0x0" for i in muls):
        r.add("i")
    return r


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", help="the tree to check (default: the one this script is in). CI runs the trusted "
                                   "copy of this script on the tree under test.")
    ap.add_argument("--tsv", help="the manifest to write (default: tools/accounting.tsv of the tree)")
    a = ap.parse_args()
    global REPO, M
    if a.repo:
        REPO = Path(a.repo).resolve()
        M = REPO / "conker"
    a.tsv = a.tsv or str(REPO / "tools/accounting.tsv")

    listed = {}
    for line in (REPO / "tools/hasm_proofs.tsv").read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            f, rule, ev = line.split("\t", 2)
            listed.setdefault(f, []).append((rule, ev))

    problems, pads = [], set()
    # leftovers in the sources
    # Any asm in a C file is a problem, except a GLOBAL_ASM block that is one label followed only by nops
    # (alignment padding between objects, not code). Comments are removed first, so a comment line inside
    # or before a block cannot hide it.
    for p in sorted((M / "src").rglob("*.c")):
        s = re.sub(r"/\*.*?\*/", " ", p.read_text(errors="replace"), flags=re.S)
        s = re.sub(r"//[^\n]*", "", s)
        rel = p.relative_to(M)
        for g in re.findall(r"#\s*pragma\s+GLOBAL_ASM\b[^\n]*", s):
            problems.append(f"GLOBAL_ASM stub: {rel}: {g.strip()}")
        for m in re.finditer(r"\bGLOBAL_ASM\s*\(", s):
            if re.match(r"#\s*pragma", s[s.rfind("\n", 0, m.start()) + 1:m.start()].strip()):
                continue  # counted above
            end = s.find("\n)", m.end())
            words = [l.strip() for l in s[m.end():end if end >= 0 else len(s)].splitlines() if l.strip()]
            labels = [w.split()[1] for w in words if re.fullmatch(r"glabel\s+\w+", w)]
            if len(labels) == 1 and all(w == "nop" or w.startswith("glabel") for w in words) and "nop" in words:
                pads.add(labels[0])  # alignment padding between objects, not code
            else:
                problems.append(f"inline GLOBAL_ASM block: {rel}: {labels[0] if labels else '(no label)'}")
        for m in re.finditer(r"\b(?:__asm__|__asm|asm)\s*(?:volatile\s*|__volatile__\s*)?\(", s):
            problems.append(f"inline asm statement: {rel}: {s[m.start():m.start() + 40].splitlines()[0]}")

    rows, byseg = [], collections.defaultdict(collections.Counter)
    proven = {}
    for name, start, ins, o, _ in functions():
        seg = "init" if start < 0x15000000 else "game"
        if start >= 0x16000000:
            seg = "debugger"
        if name in pads:
            kind = "pad"
        elif o.endswith(".c.o"):
            kind = "c"
        elif o.endswith(".s.o") and o.startswith("build/src/"):
            kind = "hasm"
        elif o.endswith(".s.o"):
            kind = "asm"
            problems.append(f"asm outside src/: {name} in {o}")
        else:
            continue
        rules = computed_rules(start, ins) if kind == "hasm" else set()
        rules |= {r for r, _ in listed.get(name, [])}
        rows.append([name, start, 4 * len(ins), seg, kind, rules, o])
        if kind == "hasm" and rules - {"s"}:
            proven[name] = True

    # a proof row must name a hand-written function: a row left behind by a rename silently stops counting
    hasm_names = {r[0] for r in rows if r[4] == "hasm"}
    for name in sorted(set(listed) - hasm_names):
        problems.append(f"tools/hasm_proofs.tsv: {name} is not a hand-written function (renamed or moved to C?)")

    # k: same object as a proven function across a non-16-aligned boundary (both directions, to a fixpoint)
    rows.sort(key=lambda r: r[1])
    changed = True
    while changed:
        changed = False
        for i, r in enumerate(rows):
            if r[4] != "hasm" or r[0] in proven:
                continue
            for j in (i - 1, i + 1):
                if 0 <= j < len(rows) and rows[j][4] == "hasm" and rows[j][0] in proven:
                    boundary = r[1] if j < i else rows[j][1]
                    if boundary % 16:
                        r[5].add("k")
                        proven[r[0]] = True
                        changed = True
                        break

    with open(a.tsv, "w") as out:
        out.write("# func\taddr\tbytes\tsegment\torigin\tproof\tobject   (generated by tools/accounting.py)\n")
        for name, start, size, seg, kind, rules, o in rows:
            byseg[seg][kind] += size
            byseg[seg][kind + "_n"] += 1
            if kind == "hasm":
                tier = "proven" if name in proven else ("search" if "s" in rules else "NONE")
                byseg[seg]["hasm_" + tier] += size
                byseg[seg]["hasm_" + tier + "_n"] += 1
                if tier == "NONE":
                    problems.append(f"hasm without proof: {name} ({o})")
            out.write(f"{name}\t0x{start:08X}\t{size}\t{seg}\t{kind}\t{','.join(sorted(rules))}\t{o}\n")

    for seg in CODE_SEGS:
        c = byseg[seg]
        tot = c["c"] + c["hasm"] + c["asm"] + c["pad"]
        print(f"{seg:9s} {tot:9,d} B  C {c['c']:9,d} ({c['c_n']} funcs)  hasm proven {c['hasm_proven']:7,d} "
              f"({c['hasm_proven_n']})  hasm search {c['hasm_search']:5,d} ({c['hasm_search_n']})  "
              f"nop pad {c['pad']:3d}  other {c['asm'] + c['hasm_NONE']:,d}")
    if problems:
        print("\n".join(problems))
        sys.exit(f"NOT fully accounted: {len(problems)} problem(s)")
    print("OK: every function is matched C or hand-written asm with a recorded proof")


if __name__ == "__main__":
    main()
