#!/usr/bin/env python3
"""accounting.py — prove that every byte of US CPU code is matched C or hand-written asm.

    accounting.py [--tsv OUT]      (run after a build; needs build/conker.us.map and build/conker.us.elf)

For every function in the init, game and debugger code segments it determines the origin:
  c     compiled from a C file in conker/src
  hasm  committed hand-written assembly (conker/src/*.s), with the proof rule(s) that apply
It exits non-zero if anything is left over: a GLOBAL_ASM stub, an inline GLOBAL_ASM block, an
asm segment outside src/, or a hasm function with no proof.

Proof rules (see AGENTS.md "Hand-written asm"). The rules marked * are computed from the instructions here;
every computed rule was checked to occur 0 times in the 5,761 functions compiled by IDO 5.3.
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
        sys.path.insert(0, str(Path(__file__).resolve().parent))  # never from the tree under test (--repo)
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
    # a static function that IDO declared before defining it leaves no symbol in the ELF; symbol_addrs
    # still names its start, so the manifest keeps one row for each function
    for line in (M / "symbol_addrs.us.txt").read_text().splitlines():
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func", line)
        if m and int(m.group(2), 16) in insns:
            syms.setdefault(int(m.group(2), 16), m.group(1))
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
    return out, insns


def entry_points(pieces, insns):
    """Piece starts that are real entry points: a jal target anywhere, an address stored in data, or the
    target of a j or branch in another object. A label that nothing outside its own object calls, jumps
    to or stores is not an entry point. Its piece is merged into the one before it when the rules are
    computed, so that a label placed inside compiled code cannot create a rule (g, d, k...)."""
    starts = {start: o for _, start, _, o, _ in pieces}
    ranges = sorted((start, end, o) for start, end, o in objects())
    obj_at = {}
    for a in insns:
        lo, hi = 0, len(ranges)
        while lo < hi:
            mid = (lo + hi) // 2
            if ranges[mid][1] <= a:
                lo = mid + 1
            else:
                hi = mid
        if lo < len(ranges) and ranges[lo][0] <= a:
            obj_at[a] = ranges[lo][2]
    entries = set()
    for a, (op, args) in insns.items():
        m = re.search(r"(?:^|,)([0-9a-f]{8})(?:<|$)", args)
        if not m:
            continue
        t = int(m.group(1), 16)
        if t in starts and (op == "jal" or obj_at.get(a) != starts[t]):
            entries.add(t)
    dump = run("mips-linux-gnu-objdump -s -j .init_data -j .game_data -j .debugger build/conker.us.elf")
    for line in dump.splitlines():
        m = re.match(r"^ ([0-9a-f]{8}) ((?:[0-9a-f]{8} ?){1,4})", line)
        if not m:
            continue
        base = int(m.group(1), 16)
        for k, w in enumerate(m.group(2).split()):
            if (base + 4 * k) not in obj_at and int(w, 16) in starts:
                entries.add(int(w, 16))
    return entries


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


def strip_c(text):
    """C text with comments replaced by spaces (newlines kept), scanned in order like the compiler does:
    a `/*` inside a `//` comment or a string does not open a comment, and the reverse."""
    # translation phases 1-2 first: a ??/ trigraph is a backslash, and backslash-newline joins two lines
    # (so a comment can end in "*\\" + newline + "/")
    text = re.sub(r"(?:\\|\?\?/)\r?\n", "", text.replace("??/", "\\"))
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i)); i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j])); i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            out.append(text[i:j + 1]); i = j + 1
        else:
            out.append(c); i += 1
    return "".join(out)


ASM_TOKENS = re.compile(r"\b(GLOBAL_ASM|INCLUDE_ASM|INCLUDE_RODATA|asmproc|__asm__|__asm|asm)\b")


def source_asm(m):
    """Asm in the C sources and headers: every GLOBAL_ASM, INCLUDE_ASM, INCLUDE_RODATA, asm-processor pragma
    and asm statement outside comments is a problem, except a GLOBAL_ASM block that is one label followed
    only by nops (alignment padding between objects, not code). Returns (problems, padding labels)."""
    problems, pads = [], set()
    files = sorted(p for d in ("src", "include") for p in (m / d).rglob("*") if p.is_file()
                   and p.suffix in (".c", ".h", ".inc", ".inc.c"))
    for p in files:
        s = strip_c(p.read_text(errors="replace"))
        rel = p.relative_to(m)
        for t in ASM_TOKENS.finditer(s):
            line = s[s.rfind("\n", 0, t.start()) + 1:s.find("\n", t.start())].strip()
            if t.group(1) == "GLOBAL_ASM" and re.match(r"GLOBAL_ASM\s*\(\s*$", line) and p.suffix == ".c":
                end = s.find("\n)", t.end())
                words = [w.strip() for w in s[s.find("(", t.end()) + 1:end if end >= 0 else len(s)].splitlines() if w.strip()]
                labels = [w.split()[1] for w in words if re.fullmatch(r"glabel\s+\w+", w)]
                if len(labels) == 1 and all(w == "nop" or w.startswith("glabel") for w in words) and "nop" in words:
                    pads.add(labels[0])  # alignment padding between objects, not code
                    continue
            problems.append(f"asm in a C source: {rel}: {line[:60]}")
    return problems, pads


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

    problems, pads = source_asm(M)

    rows, byseg = [], collections.defaultdict(collections.Counter)
    proven = {}
    pieces, insns = functions()
    # rules are computed on runs: a piece that is not an object start and not an entry point belongs to the
    # run of the piece before it (see entry_points)
    # A run starts only at an entry point or where the code changes kind (compiled C, hand-written asm or
    # padding); an object start alone does not begin one. So a compiled function cut into pieces, by labels
    # or by splitting it across .s files, stays one run, and its pieces cannot prove each other (g, d, k).
    entries = entry_points(pieces, insns)

    def kind_of(name, o):
        if name in pads:
            return "pad"
        return "c" if o.endswith(".c.o") else "hasm" if o.endswith(".s.o") and o.startswith("build/src/") else "other"
    def ends_routine(ins):
        """The code before ends in a return or an unconditional jump (with its delay slot)."""
        ins = list(ins)
        while ins and (ins[-1][1], ins[-1][2]) in (("nop", ""), ("sll", "zero,zero,0x0")):
            ins.pop()  # alignment padding after the routine
        if len(ins) < 2:
            return False
        op, args = ins[-2][1], ins[-2][2]
        return op in ("jr", "j", "eret") or (op == "beq" and args.startswith("zero,zero,"))
    run_of, runs = {}, collections.OrderedDict()
    prev_kind = prev_obj = cur = None
    for name, start, ins, o, _ in pieces:
        k = kind_of(name, o)
        # a 16-aligned object start after a finished routine is a routine of its own, even if nothing calls
        # it; a compiled function cut in the middle does not end in a return there, so it stays one run
        new_object = o != prev_obj and start % 16 == 0 and cur is not None and ends_routine(runs[cur])
        prev_obj = o
        if start in entries or k != prev_kind or cur is None or new_object:
            cur = start
            runs[cur] = []
        runs[cur].extend(ins)
        run_of[start] = cur
        prev_kind = k
    run_rules = {r: computed_rules(r, i) for r, i in runs.items()}
    for name, start, ins, o, _ in pieces:
        seg = "init" if start < 0x15000000 else "game"
        if start >= 0x16000000:
            seg = "debugger"
        if name in pads:
            kind = "pad"
            if any((op, args) not in (("nop", ""), ("sll", "zero,zero,0x0")) for _, op, args in ins):
                problems.append(f"padding block contains code: {name}")
        elif o.endswith(".c.o"):
            kind = "c"
        elif o.endswith(".s.o") and o.startswith("build/src/"):
            kind = "hasm"
        elif o.endswith(".s.o"):
            kind = "asm"
            problems.append(f"asm outside src/: {name} in {o}")
        else:
            continue
        rules = set(run_rules[run_of[start]]) if kind == "hasm" else set()
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
