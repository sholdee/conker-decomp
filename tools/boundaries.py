#!/usr/bin/env python3
"""boundaries.py: grade the evidence for every file boundary in the US code, and list split and merge
candidates. It only reports. It changes no file of the source layout.

    python3 tools/boundaries.py [--ects ECTS_BUILD] [--debug DEBUG_BUILD]
                                [--ects-pairs ECTS_PAIRS_JSON]
                                [--debug-pairs FUNCTION_PAIRS_TSV --debug-functions DEBUG_FUNCTIONS_TSV]
                                [--us-bin conker/conker.us.bin] [--out TSV] [--candidates TSV]

A byte match does not show that our files have Rare's boundaries. This tool reads the US layout
(conker/conker.us.yaml), the function list (tools/accounting.tsv), the per-file flags (conker/Makefile,
conker/ultralib.mk) and the US code (conker/conker.us.bin, from `make extract`). When it gets the two
older builds and their function pairs, it also uses them:

- ECTS build (August 2000): the ROM (sha1 in conker.ects.sha1), or its game segment alone.
- Debug build (October 2000): the ROM (sha1 in conker.debug.sha1), or its decompressed game code alone.
- The pairs: `ects_pairs.json` (US function -> ECTS function, and the ECTS function list),
  `function_pairs.tsv` (US function -> debug function) and `debug_functions.tsv` (the debug function list).

The builds and the pairs stay outside the repository. Without them the tool says what it cannot check.

How it uses an older build:
- It reads the code segments at the ROM offsets of conker.ects.yaml / conker.debug.yaml (the debug game
  code is decompressed with the key of game.debug.rzip.yaml).
- It checks the pairs against the code. A pairs file can use other addresses than the build's own layout
  (ects_pairs.json was made from the ECTS game segment read 0x19550 bytes too early, and its function list
  misses the ends of that segment); the tool finds that shift from the identical functions and corrects it.
  It takes the end of each code segment from the code (the first `jr ra` at or after the list's end).
- It adds the function starts that the code shows (jal targets, code addresses built in code or stored in
  data, the first word after padding) to the list from the pairs file.
- It keeps the pairs whose code is the same (identical, or other only in offsets and constants; not the
  pairs of code that the build has more than once). It keeps any other pair only when it lies in order
  between two of those. It pairs the US functions that have no partner with an identical function of the
  build (one match only), or by similar code in order between two pairs.
- A pair whose whole US code is at its partner (immediates masked) is right: no start inside it counts,
  and the next function starts after it. The other pairs are checked again against the extents that the
  new starts give: a pair moves to the best matching function inside its old extent, or is dropped.

The facts it uses (IDO 5.3):
- IDO pads each C object's .text to 16 bytes and puts no padding between the functions of one object. So
  zero words after a return, between two C functions, mean a file boundary ("padding"). A function that
  starts on an address that is not 16-aligned, right after the previous function, is in the same object
  ("join"). A function that starts on a 16-aligned address with no zero words before it shows nothing
  ("flush"). Hand-written asm can hold .align, data words and nops, so padding next to asm is no proof.
- Each C object's .data and .rodata are each one block, in link order, and each block starts on a
  16-aligned address. Not so for hand-written asm (4-aligned) or .bss, which the tool leaves out.
- One object is compiled once, with one set of flags, and C and hand-written asm are never one object.

Evidence kinds for a boundary between objects L and R (X = last function of L, Y = first function of R):
  padding-us     zero words between X and Y in US.
  padding-ects   the ECTS partners of the functions next to the boundary are in two objects there
  padding-debug  (padding between them), in the US order. "Pinned" when the paired functions are X and
                 Y, or are tied to X and Y by US joins; otherwise the boundary could be a function away.
  data-edge      two C objects: in .data or .rodata, R's block follows L's block directly and starts
                 16-aligned.
  flags-change   L and R have other -O or -mips flags, or one is C and the other hand-written asm.
  rare-name      functions of L and of R use two different source path strings (in any build).
Grades: proven = between two C objects, padding-us, or pinned padding-ects/padding-debug that the other old
build does not contradict; flags-change (not when the asm side has only search evidence); pinned
rare-name. likely = any other evidence. unproven = none.
Contradictions (the two sides are one object somewhere) go into the note and the merge candidates. A C
data block that starts unaligned where the code proves two objects is noted as "data split suspect".

The output holds addresses, object names, function names and grades only: no ROM bytes.
"""
import argparse
import bisect
import csv
import difflib
import hashlib
import json
import re
import struct
import sys
import zlib
from pathlib import Path

import yaml

REPO = Path(__file__).resolve().parent.parent
CODE_SEGMENTS = ("init", "game", "debugger")
SEG_OF_VRAM = {0x10: "init", 0x15: "game", 0x16: "debugger"}
DATA_KINDS = {".data": "data", "data": "data", ".rodata": "rodata", "rodata": "rodata",
              ".bss": "bss", "bss": "bss"}
# sha1 of the segment-only forms that --ects / --debug also accept (hashes, not ROM bytes)
ECTS_GAME_SHA1 = "b108bdc78a40c7d643ea96625ac2dc0f7e124005"   # ECTS ROM from game.ects to debugger.ects
DEBUG_GAME_SHA1 = "a1c41378211a6e50c60fd16ffeccedb3909d7bf1"  # debug game code, decompressed
NEAR = 4                 # a split candidate's partners have at most this many functions between them
PATH_STRING = re.compile(rb"\.\./[A-Za-z0-9_/]+\.c\x00")


def h(a):
    return "0x%08X" % a


def sha1(b):
    return hashlib.sha1(b).hexdigest()


def note(msg):
    print("boundaries.py: " + msg, file=sys.stderr)


# --------------------------------------------------------------------------------------------- inputs

def load_layout(yaml_path):
    """Code objects in link order for each code segment, and the data blocks of each data section."""
    cfg = yaml.safe_load(Path(yaml_path).read_text())
    segs = [s for s in cfg["segments"] if isinstance(s, dict)]
    objects = []          # (segment, rom, vram, type, name)
    blocks = []           # (segment group, kind, vram, owner or None)
    seginfo = {}
    for s in segs:
        if "subsegments" not in s or "vram" not in s:
            continue
        start, vram = s["start"], s["vram"]
        if not isinstance(vram, int):
            continue
        seginfo[s["name"]] = (start, vram)
        group = s["name"].replace("_data", "")
        for ss in s["subsegments"]:
            if isinstance(ss, list):
                if len(ss) < 2:
                    continue
                rom, typ, name, sv = ss[0], ss[1], (ss[2] if len(ss) > 2 else None), None
            else:
                rom, typ, name, sv = ss.get("start"), ss.get("type"), ss.get("name"), ss.get("vram")
            va = sv if sv is not None else vram + (rom - start)
            if typ in ("c", "hasm", "asm") and s["name"] in CODE_SEGMENTS:
                objects.append((s["name"], rom, va, typ, name))
            elif typ in DATA_KINDS:
                blocks.append((group, DATA_KINDS[typ], va, name if typ.startswith(".") else None))
    return cfg, seginfo, objects, blocks


def load_accounting(path):
    funcs = []            # (addr, size, name, segment, origin, proof, object)
    for r in csv.reader(open(path), delimiter="\t"):
        if not r or r[0].startswith("#"):
            continue
        obj = re.sub(r"^build/src/", "", r[6])
        obj = re.sub(r"\.(c|s)\.o$", "", obj)
        funcs.append((int(r[1], 16), int(r[2]), r[0], r[3], r[4], r[5], obj))
    funcs.sort()
    return funcs


def load_flags(makefile, ultralib_mk):
    """Per object: OPT_FLAGS, MIPSBIT and the CFLAGS family, from the target-specific rules."""
    srcs = []
    text = Path(ultralib_mk).read_text().replace("\\\n", " ")
    for m in re.finditer(r"^ULTRALIB_SRCS\s*\+?=\s*(.*)$", text, re.M):
        srcs += m.group(1).split()
    rules = []            # (pattern or exact target, var, value), in file order
    pre = r"\$\(BUILD_DIR\)/\$\(SRC_DIR\)/"
    for line in Path(makefile).read_text().splitlines():
        if line.lstrip().startswith("#"):
            continue
        m = re.match(r"^\$\(foreach s,(?:\$\(filter ([^,]+),\$\(ULTRALIB_SRCS\)\)|\$\(ULTRALIB_SRCS\)),\$\(eval " + pre +
                     r"(\S+): (\w+) (:=|\+=) (.*?)\)\)\s*$", line)
        if m:
            pats = m.group(1).split() if m.group(1) else None
            for s in srcs:
                if pats and not any(re.fullmatch(p.replace("%", ".*"), s) for p in pats):
                    continue
                rules.append((m.group(2).replace("$(s)", s), m.group(3), m.group(4), m.group(5)))
            continue
        m = re.match(r"^" + pre + r"(\S+): (\w+) (:=|\+=) (.*)$", line)
        if m:
            rules.append(m.groups())
    flags = {}

    def get(obj, typ):
        if typ != "c":
            return "asm"
        key = obj + ".c.o"
        val = {"OPT_FLAGS": "-O2 -g3", "MIPSBIT": "-mips2 -o32", "CFLAGS": "game"}
        for exact in (False, True):        # pattern rules first, explicit targets win
            for tgt, var, op, v in rules:
                if var not in val or ("%" in tgt) == exact:
                    continue
                if not re.fullmatch(re.escape(tgt).replace("%", ".*"), key):
                    continue
                if var == "CFLAGS":
                    if op == ":=":
                        val["CFLAGS"] = "ultralib" if "ULTRA_CFLAGS" in v else v
                    continue               # += adds -D renames only: no codegen change
                val[var] = v.strip()
        return "%s %s %s" % (val["OPT_FLAGS"], val["MIPSBIT"], val["CFLAGS"])
    return get


# ------------------------------------------------------------------------------------------ MIPS bits

def is_branch(w):
    op = w >> 26
    if op in (2, 3) or 4 <= op <= 7 or 20 <= op <= 23:
        return True
    if op == 0:
        return (w & 0x3F) in (8, 9)
    if op == 1:
        return ((w >> 16) & 31) in (0, 1, 2, 3, 16, 17, 18, 19)
    if op in (0x11, 0x12):
        return ((w >> 21) & 31) == 8
    return False


def data_refs(code, vram):
    """lui + addiu/load/store pairs: the set of addresses the code builds (as in the earlier refs.py)."""
    n = len(code) // 4
    ws = struct.unpack(">%dI" % n, code[:n * 4])
    out = set()
    reg = {}
    for i, w in enumerate(ws):
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:
            reg[rt] = (imm << 16, i)
        elif op in (9, 0x0D) or 0x20 <= op <= 0x2E or op in (0x31, 0x35, 0x37, 0x39, 0x3D, 0x3F):
            if rs in reg and i - reg[rs][1] < 24:
                out.add((reg[rs][0] + (imm if op == 0x0D else simm)) & 0xFFFFFFFF)
                if op == 9 and rt == rs:
                    del reg[rs]
                elif (op == 9 or 0x20 <= op <= 0x27) and rt != rs:
                    reg.pop(rt, None)
            elif op in (9, 0x0D) or 0x20 <= op <= 0x27:
                reg.pop(rt, None)
        elif op == 0:
            f, rd = w & 0x3F, (w >> 11) & 31
            if f == 0x21 and (rs in reg or rt in reg):
                reg[rd] = reg.get(rs) or reg.get(rt)
            elif f not in (8, 9, 0x18, 0x19, 0x1A, 0x1B):
                reg.pop(rd, None)
        elif op in (8, 0x0A, 0x0B, 0x0C, 0x0E):
            reg.pop(rt, None)
    return out


# ---------------------------------------------------------------------------------------------- builds

class Build:
    """One build: its function starts (sorted), its code bytes, and the joins between its functions.

    join[i] describes the meeting of function i-1 and function i (same segment):
      ("pad", gap, start)    zero words between them: two objects
      ("join", 0, start)     no gap, start not 16-aligned: one object
      ("flush", 0, start)    no gap, start 16-aligned: no evidence
      ("odd", gap, start)    the code before does not end with a return or jump (a wrong start, or
                             hand-written asm that falls through, or data or nops in asm), or zero
                             words end on an unaligned start: no evidence
    """

    def __init__(self, name, starts, code, exact_starts):
        self.name = name
        self.code = code                 # segment -> (bytes, vram)
        self.starts = sorted(starts)     # (addr, segment)
        self.index = {a: i for i, (a, _) in enumerate(self.starts)}
        self.partner = {}                # US addr -> build addr
        self.rev = {}                    # build addr -> [US addr]
        self.join = [None] * len(self.starts)
        self.real = [None] * len(self.starts)  # (real start, real end)
        for i, (a, seg) in enumerate(self.starts):
            nxt = self.starts[i + 1][0] if i + 1 < len(self.starts) and self.starts[i + 1][1] == seg else None
            self.real[i] = self._extent(seg, a, nxt, exact_starts)
        for i in range(1, len(self.starts)):
            if self.starts[i][1] != self.starts[i - 1][1] or self.real[i] is None or self.real[i - 1] is None:
                continue
            s, e = self.real[i][0], self.real[i - 1][1]
            gap = s - e
            ret = self.ends_like_return(self.starts[i][1], e)
            if not ret or (gap > 0 and s % 16):
                kind = "odd"             # not a function end, or zero words before an unaligned start
            else:
                kind = "pad" if gap > 0 else ("join" if s % 16 else "flush")
            self.join[i] = (kind, gap, s)

    def ends_like_return(self, seg, e):
        """The code before e ends with a return or an unconditional jump and its delay slot."""
        w2, w1 = self.word(seg, e - 8), self.word(seg, e - 4)
        if w2 is None:
            return False
        return (w2 >> 26 == 0 and (w2 & 0x3F) == 8) or w2 >> 26 == 2 or (w2 >> 16) == 0x1000 or \
            w1 == 0x42000018 or (w1 >> 26 == 0 and (w1 & 0x3F) in (0x0C, 0x0D))   # eret, syscall, break

    def word(self, seg, a):
        b, v = self.code[seg]
        o = a - v
        if o < 0 or o + 4 > len(b):
            return None
        return struct.unpack_from(">I", b, o)[0]

    def _extent(self, seg, a, nxt, exact_starts):
        if seg not in self.code:
            return None
        b, v = self.code[seg]
        end = nxt if nxt is not None else v + len(b)
        s = a
        if not exact_starts:             # a list start can sit on the padding before the function
            while s < end and self.word(seg, s) == 0:
                s += 4
        e = end
        while e > s and self.word(seg, e - 4) == 0:
            e -= 4
        if e > s and is_branch(self.word(seg, e - 4)):
            e += 4                       # the delay slot nop
        return (s, min(e, end))

    def func_code(self, i):
        a, seg = self.starts[i]
        s, e = self.real[i]
        b, v = self.code[seg]
        return b[s - v:e - v], s

    def relation(self, i, j):
        """Objects of functions i < j in this build: 'diff', 'same' or 'unknown', and the first padding."""
        kinds = [self.join[k] for k in range(i + 1, j + 1)]
        if any(k is None for k in kinds):
            return "unknown", None
        pads = [k for k in kinds if k[0] == "pad"]
        if pads:
            return "diff", pads[0]
        if all(k[0] == "join" for k in kinds):
            return "same", None
        return "unknown", None


def read_rom_or_segment(path, label, rom_sha1, seg_sha1):
    b = Path(path).read_bytes()
    d = sha1(b)
    if d == rom_sha1:
        return "rom", b
    if d == seg_sha1:
        return "segment", b
    sys.exit("boundaries.py: %s: sha1 %s is neither the %s ROM nor its game segment" % (path, d, label))


def outer_starts(yaml_path):
    cfg = yaml.safe_load(Path(yaml_path).read_text())
    return {s["name"]: s["start"] for s in cfg["segments"] if isinstance(s, dict) and "name" in s}


def seg_ranges(funcs):
    r = {}
    for a, n, seg in funcs:
        lo, hi = r.get(seg, (a, a + n))
        r[seg] = (min(lo, a), max(hi, a + n))
    return r


VRAM = {"init": 0x10001000, "game": 0x15000000, "debugger": 0x16000000}


def code_length(b, start, n):
    """The length of a code segment that starts at b[start]: the end of the function list (n), moved on
    to the first `jr ra` and its delay slot when the list cuts the last function."""
    for p in range(n, n + 0x400, 4):
        if start + p > len(b):
            break
        if struct.unpack_from(">I", b, start + p - 8)[0] == 0x03E00008:
            return p
    return n


def load_ects(path, pairs_path):
    """Code, game data, list starts and raw pairs of the ECTS build."""
    e = json.load(open(pairs_path))
    flist = [(f["addr"], f["size"], f["seg"]) for f in e["dbg"]]
    rng = seg_ranges(flist)
    form, b = read_rom_or_segment(path, "ECTS", (REPO / "conker.ects.sha1").read_text().split()[0],
                                  ECTS_GAME_SHA1)
    st = outer_starts(REPO / "conker.ects.yaml")
    rom_of = {"init": st["init.ects"], "game": st["game.ects"], "debugger": st["debugger.ects"]}
    code = {}
    if form == "rom":
        glen = code_length(b, rom_of["game"], rng["game"][1] - VRAM["game"])
        for seg in CODE_SEGMENTS:
            n = code_length(b, rom_of[seg], rng[seg][1] - VRAM[seg])
            code[seg] = (b[rom_of[seg]:rom_of[seg] + n], VRAM[seg])
        data = b[rom_of["game"] + glen:st["debugger.ects"]]
    else:
        glen = code_length(b, 0, rng["game"][1] - VRAM["game"])
        code["game"] = (b[:glen], VRAM["game"])
        data = b[glen:]
    starts = [(a, s) for a, n, s in flist]
    pairs = [(f["addr"], f["daddr"], f.get("tier", "")) for f in e["us"]
             if "daddr" in f and f.get("tier", "none") != "none"]
    return form, code, data, starts, pairs


def load_debug(path, pairs_path, funcs_path):
    """Code, game data, list starts and raw pairs of the debug build."""
    flist = []
    for r in csv.reader(open(funcs_path), delimiter="\t"):
        if r and not r[0].startswith("#"):
            flist.append((int(r[1], 16), int(r[2]), r[0]))
    rng = seg_ranges(flist)
    form, b = read_rom_or_segment(path, "debug", (REPO / "conker.debug.sha1").read_text().split()[0],
                                  DEBUG_GAME_SHA1)
    code, data = {}, None
    if form == "rom":
        st = outer_starts(REPO / "conker.debug.yaml")
        rz = yaml.safe_load((REPO / "game.debug.rzip.yaml").read_text())["segments"]
        xor = rz[0]["xor"]
        code_end = next(s[0] for s in rz if isinstance(s, list) and len(s) > 2 and s[2] == "code_padding")
        data_at = next(s["start"] for s in rz if isinstance(s, dict) and s.get("name") == "data")
        data_end = next(s[0] for s in rz if isinstance(s, list) and len(s) > 2 and s[2] == "data_padding")
        g0 = st["game.debug.rzip"]
        blob = b[g0:g0 + code_end]
        offs, i = [], 1
        while True:                      # the block offset table (XOR key from game.debug.rzip.yaml)
            v = struct.unpack_from(">I", blob, i * 4)[0]
            i += 1
            if v == 0:
                break
            offs.append(v ^ xor)
        game = b"".join(zlib.decompressobj(wbits=-15).decompress(blob[x + 4:y]) for x, y in zip(offs, offs[1:]))
        code["game"] = (game, VRAM["game"])
        for seg, key in (("init", "init.debug"), ("debugger", "debugger.debug")):
            n = code_length(b, st[key], rng[seg][1] - VRAM[seg])
            code[seg] = (b[st[key]:st[key] + n], VRAM[seg])
        data = zlib.decompressobj(wbits=-15).decompress(b[g0 + data_at + 4:g0 + data_end])
    else:
        code["game"] = (b, VRAM["game"])
    starts = [(a, s) for a, n, s in flist]
    pairs = []
    for r in csv.reader(open(pairs_path), delimiter="\t"):
        if r and not r[0].startswith("#") and len(r) > 8 and r[8] and not r[6].startswith("F"):
            pairs.append((int(r[1], 16), int(r[8], 16), r[6]))
    return form, code, data, starts, pairs


LS_OPS = {0x09, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x31, 0x35, 0x37,
          0x39, 0x3D, 0x3F}


def masked(words):
    """Words with relocated fields masked (jal targets, %hi, and the %lo of loads, stores and addiu)."""
    reg, out = set(), []
    for w in words:
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        m = 0xFFFFFFFF
        if op in (2, 3):
            m = 0xFC000000
        elif op == 0x0F:
            m = 0xFFFF0000
            reg.add(rt)
        elif op in LS_OPS and rs in reg:
            m = 0xFFFF0000
            if (op == 9 or 0x20 <= op <= 0x26) and rt != rs:
                reg.discard(rt)
        elif op == 0 and (w & 0x3F) == 0x21 and (rs in reg or rt in reg):
            reg.add((w >> 11) & 31)
        elif op == 0 and (w & 0x3F) not in (8, 9, 0x18, 0x19, 0x1A, 0x1B):
            reg.discard((w >> 11) & 31)
        elif op in (0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or 0x20 <= op <= 0x26:
            reg.discard(rt)
        out.append((w & m, m))
    return out


def us_words(us, ua):
    i = us.index[ua]
    c, _ = us.func_code(i)
    ws = list(struct.unpack(">%dI" % (len(c) // 4), c))
    return ws


def matches_at(code, vram, a, mw):
    o = a - vram
    if o < 0 or o + 4 * len(mw) > len(code):
        return False
    return all((struct.unpack_from(">I", code, o + 4 * k)[0] & m) == w for k, (w, m) in enumerate(mw))


def find_masked(code, mw):
    """Offsets in code where the masked words match, found through their longest unmasked run."""
    best, i = (0, 0), 0
    while i < len(mw):
        j = i
        while j < len(mw) and mw[j][1] == 0xFFFFFFFF:
            j += 1
        if j - i > best[1] - best[0]:
            best = (i, j)
        i = j + 1
    a, b = best
    if b - a < 3:
        return None
    anchor = b"".join(struct.pack(">I", w) for w, _ in mw[a:b])
    hits, p = [], code.find(anchor)
    while p != -1:
        s = p - 4 * a
        if s >= 0 and s % 4 == 0 and matches_at(code, 0, s, mw):
            hits.append(s)
        p = code.find(anchor, p + 4)
    return hits


def fix_coordinates(name, code, starts, pairs, us):
    """The pairs files can use another base address for a segment than the build's own layout (the ECTS
    pairs were made from the game segment read at the wrong ROM offset). Find each segment's shift from
    the pairs whose code is identical, and move the list and the pairs onto the build's addresses."""
    shift = {}
    for seg, (b, v) in sorted(code.items()):
        votes = {}
        tested = 0
        for ua, ba, tier in pairs:
            if seg != SEG_OF_VRAM.get(ba >> 24) or ua not in us.index or tested >= 40:
                continue
            mw = masked(us_words(us, ua))
            while mw and mw[-1] == (0, 0xFFFFFFFF):
                mw.pop()
            if len(mw) < 8:
                continue
            hits = find_masked(b, mw)
            if not hits or len(hits) != 1:
                continue
            tested += 1
            d = v + hits[0] - ba
            votes[d] = votes.get(d, 0) + 1
        if votes:
            d, n = max(sorted(votes.items()), key=lambda t: t[1])
            if n >= max(3, tested // 2):
                shift[seg] = d
    for seg, d in sorted(shift.items()):
        if d:
            note("%s: the pairs use addresses %s0x%X from the build's own for %s; corrected"
                 % (name, "-" if d > 0 else "+", abs(d), seg))

    def mv(a):
        seg = SEG_OF_VRAM.get(a >> 24)
        if seg not in code:
            return None
        a2 = a + shift.get(seg, 0)
        b, v = code[seg]
        return a2 if v <= a2 < v + len(b) else None
    starts = [(mv(a), s) for a, s in starts if mv(a) is not None]
    pairs = [(ua, mv(ba), t) for ua, ba, t in pairs if mv(ba) is not None]
    return starts, pairs


def switch_jumps(ws, v):
    """Addresses of `jr` instructions that jump through a switch table (a load from a lui-built address)."""
    reg, loaded, out = {}, {}, []
    for k, w in enumerate(ws):
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        if op == 0x0F:
            reg[rt] = k
        elif op == 0 and (w & 0x3F) == 0x21 and (rs in reg or rt in reg):
            reg[(w >> 11) & 31] = k
        elif op == 0x23 and rs in reg:
            loaded[rt] = k
        elif op == 0 and (w & 0x3F) == 8 and rs != 31 and rs in loaded and k - loaded[rs] < 8:
            out.append(v + 4 * k)
    return out


def discover_starts(code, data):
    """Function starts that the code and data show: jal targets, code addresses built with lui/addiu,
    the first word after the zero padding that follows a return, and code addresses stored in data
    (except the case labels of switch tables). A start must follow a return, an unconditional jump or a
    zero word."""
    allc = [(seg, b, v) for seg, (b, v) in sorted(code.items())]
    words = {seg: struct.unpack(">%dI" % (len(b) // 4), b[:len(b) // 4 * 4]) for seg, b, v in allc}

    def plausible(a):
        for seg, b, v in allc:
            if v <= a < v + len(b) and a % 4 == 0:
                ws, k = words[seg], (a - v) // 4
                if k < 2 or ws[k] == 0:
                    return None
                p1, p2 = ws[k - 1], ws[k - 2]
                if p1 == 0 or p2 == 0x03E00008 or p2 >> 26 == 2 or (p2 >> 16) == 0x1000:
                    return seg
                return None
        return None
    found, jumps = set(), []
    for seg, b, v in allc:
        ws = words[seg]
        n = len(ws)
        for k, w in enumerate(ws):
            if w >> 26 == 3:
                found.add(((v + 4 * k) & 0xF0000000) | ((w & 0x3FFFFFF) << 2))
        found |= data_refs(b, v)
        jumps += switch_jumps(ws, v)
        k = 0
        while k < n:
            if ws[k] == 0x03E00008 and k + 2 < n:     # jr ra; delay slot; zero words; 16-aligned start
                j = k + 2
                while j < n and ws[j] == 0:
                    j += 1
                if j > k + 2 and j < n and (v + 4 * j) % 16 == 0:
                    found.add(v + 4 * j)
                k = j
                continue
            k += 1
    sure = sorted(a for a in found if plausible(a))
    if data:
        jumps.sort()
        n = len(data) // 4
        for w in sorted(set(struct.unpack(">%dI" % n, data[:n * 4]))):
            if w in found or not plausible(w):
                continue
            # a case label: a switch jump before it and no known start between them
            j = bisect.bisect_left(jumps, w) - 1
            if j >= 0 and w - jumps[j] < 0x2000:
                s = bisect.bisect_left(sure, jumps[j])
                if s >= len(sure) or sure[s] > w:
                    continue
            found.add(w)
    return [(a, plausible(a)) for a in sorted(found) if plausible(a)]


def masked_values(ws):
    mw = masked(ws)
    while mw and mw[-1] == (0, 0xFFFFFFFF):
        mw.pop()
    return [w for w, _ in mw]


def align_gaps(bld, us, real_us, min_ratio=0.5):
    """Pair the unpaired US functions that lie between two pairs with the unpaired functions of the build
    between the same two pairs, in order, by the similarity of their masked code."""
    added = []
    byseg = {}
    for ua in real_us:
        byseg.setdefault(us.starts[us.index[ua]][1], []).append(ua)
    bseg_starts = {}
    for i, (a, seg) in enumerate(bld.starts):
        bseg_starts.setdefault(seg, []).append(i)
    for seg, fl in sorted(byseg.items()):
        anchors = [k for k, ua in enumerate(fl) if ua in bld.partner]
        bounds = [(-1, None)] + [(k, bld.index[bld.partner[fl[k]]]) for k in anchors] + [(len(fl), None)]
        for (k1, i1), (k2, i2) in zip(bounds, bounds[1:]):
            uf = fl[k1 + 1:k2]
            if not uf:
                continue
            bsg = bld.starts[i1][1] if i1 is not None else (bld.starts[i2][1] if i2 is not None else None)
            if bsg is None or bsg not in bseg_starts:
                continue
            if i1 is not None and i2 is not None and (i2 <= i1 or bld.starts[i2][1] != bsg):
                continue
            lo = i1 + 1 if i1 is not None else bseg_starts[bsg][0]
            hi = i2 if i2 is not None else bseg_starts[bsg][-1] + 1
            if i1 is None and seg != bsg or i2 is None and seg != bsg:
                continue
            bf = [i for i in range(lo, hi) if not bld.rev.get(bld.starts[i][0]) and bld.real[i]]
            if not bf or len(uf) * len(bf) > 6000:
                continue
            uw = [masked_values(us_words(us, ua)) for ua in uf]
            bw = []
            for i in bf:
                c, _ = bld.func_code(i)
                bw.append(masked_values(list(struct.unpack(">%dI" % (len(c) // 4), c))))
            n, m = len(uf), len(bf)
            sim = [[0.0] * m for _ in range(n)]
            for x in range(n):
                for y in range(m):
                    a_, b_ = len(uw[x]), len(bw[y])
                    if not a_ or not b_ or not (0.5 <= a_ / b_ <= 2.0):
                        continue
                    r = difflib.SequenceMatcher(None, uw[x], bw[y], autojunk=False).ratio()
                    if r >= min_ratio:
                        sim[x][y] = r
            best = [[0.0] * (m + 1) for _ in range(n + 1)]
            for x in range(n - 1, -1, -1):
                for y in range(m - 1, -1, -1):
                    best[x][y] = max(best[x + 1][y], best[x][y + 1],
                                     (sim[x][y] + best[x + 1][y + 1]) if sim[x][y] else 0.0)
            x = y = 0
            while x < n and y < m:
                if sim[x][y] and best[x][y] == sim[x][y] + best[x + 1][y + 1]:
                    added.append((uf[x], bld.starts[bf[y]][0]))
                    x += 1
                    y += 1
                elif best[x][y] == best[x + 1][y]:
                    x += 1
                else:
                    y += 1
    for ua, ba in added:
        add_pair(bld, ua, ba)
    return len(added)


# Tiers of pairs whose code is the same. "identical-dup" (code found more than once) is not one.
RELIABLE = ("identical", "identical*", "imm", "imm*", "masked")
RELIABLE_DEBUG = ("A ", "B ")


def reliable(t):
    return t in RELIABLE or t.startswith(RELIABLE_DEBUG)


def drop_unanchored_pairs(pairs, us, small=8):
    """Keep a pair whose code is the same (identical, or other only in offsets and constants) and at
    least `small` words long. Keep any other pair (similar code, or a short function, which repeats)
    only when its partner lies between the partners of the nearest kept pairs before and after it, in
    the same segment."""
    def size(ua):
        return len(masked_values(us_words(us, ua))) if ua in us.index else 0
    shared = {}
    for ua, ba, t in pairs:
        shared[ba] = shared.get(ba, 0) + 1
    sure = sorted((ua, ba) for ua, ba, t in pairs
                  if ua in us.index and reliable(t) and size(ua) >= small and shared[ba] == 1)
    keys = [ua for ua, _ in sure]
    keep = set(keys)
    out = []
    for ua, ba, t in pairs:
        if ua not in us.index:
            continue
        if ua in keep:
            out.append((ua, ba, t))
            continue
        k = bisect.bisect_left(keys, ua)
        if 0 < k < len(sure):
            (pa, pb), (na, nb) = sure[k - 1], sure[k]
            if pb < ba < nb and SEG_OF_VRAM.get(pb >> 24) == SEG_OF_VRAM.get(nb >> 24) and \
                    SEG_OF_VRAM.get(pa >> 24) == SEG_OF_VRAM.get(na >> 24) == SEG_OF_VRAM.get(ua >> 24):
                out.append((ua, ba, t))
    return out


def make_old_build(name, form, code, data, starts, pairs, us, real_us):
    starts, pairs = fix_coordinates(name, code, starts, pairs, us)
    have = {ua for ua, _, _ in pairs}
    extra = []                            # US functions with no pair: an identical copy, if unique
    for ua in real_us:
        if ua in have:
            continue
        mw = masked(us_words(us, ua))
        while mw and mw[-1] == (0, 0xFFFFFFFF):
            mw.pop()
        if len(mw) < 4:
            continue
        hits = []
        for seg, (b, v) in sorted(code.items()):
            for o in find_masked(b, mw) or []:
                hits.append(v + o)
        if len(hits) == 1:
            extra.append((ua, hits[0], "masked"))
    pairs = drop_unanchored_pairs(pairs + extra, us)
    extra = [p for p in pairs if p[2] == "masked"]
    pairs = [p for p in pairs if p[2] != "masked"]
    found = set(discover_starts(code, data))
    # A pair whose whole US code is at its partner (immediates masked) is right. No function starts
    # inside it, and the next function starts at the first word after it that is not zero.
    inside, whole = set(), set()
    for ua, ba, _ in pairs + extra:
        seg = SEG_OF_VRAM[ba >> 24]
        if seg not in code:
            continue
        sk = skeleton(us_words(us, ua))
        b, v = code[seg]
        o = ba - v
        if len(sk) < 4 or o + 4 * len(sk) > len(b):
            continue
        if skeleton(list(struct.unpack_from(">%dI" % len(sk), b, o))) != sk:
            continue
        whole.add((ua, ba))
        inside |= {(a, seg) for a in range(ba + 4, ba + 4 * len(sk), 4)}
        e = o + 4 * len(sk)
        while e + 4 <= len(b) and struct.unpack_from(">I", b, e)[0] == 0:
            e += 4
        if e + 4 <= len(b):
            found.add((v + e, seg))
    all_starts = (set(starts) | found) - inside
    for ua, ba, _ in pairs + extra:
        all_starts.add((ba, SEG_OF_VRAM[ba >> 24]))
    bld = Build(name, sorted(all_starts), code, exact_starts=False)
    listed = sorted(a for a, _ in starts)
    moved = dropped = 0
    for ua, ba, _ in sorted(pairs + extra):
        if ua not in us.index:
            continue
        nb = ba if (ua, ba) in whole else recheck_pair(bld, us, ua, ba, listed)
        if nb is None:
            dropped += 1
            continue
        moved += nb != ba
        add_pair(bld, ua, nb)
    n_al = align_gaps(bld, us, real_us)
    note("%s: %d functions, %d US functions paired (%d by identical code, %d by similar code in order); "
         "after the new starts, %d pairs moved to the matching function and %d dropped"
         % (name, len(bld.starts), len(bld.partner), len(extra), n_al, moved, dropped))
    return bld


def skeleton(ws):
    """The words with every immediate and jump target masked: opcode and registers only."""
    out = []
    for w in ws:
        op = w >> 26
        out.append(w if op == 0 else (op << 26 if op in (2, 3) else w & 0xFFFF0000))
    while out and out[-1] == 0:
        out.pop()
    return out


def opcodes(ws):
    """The operation of each word only (opcode, function field, coprocessor format): no registers."""
    out = []
    for w in ws:
        op = w >> 26
        out.append((op, w & 0x3F) if op == 0 else (op, (w >> 21) & 31) if op in (0x10, 0x11) else (op,))
    while out and out[-1] == (0, 0):
        out.pop()
    return out


def pair_score(bld, us, ua, i, uw=None):
    """How well the build's function i matches US function ua: the similarity of the sequences of
    operations, and 0 when the sizes differ by more than 2x."""
    if bld.real[i] is None:
        return 0.0
    uw = uw if uw is not None else opcodes(us_words(us, ua))
    c, _ = bld.func_code(i)
    bw = opcodes(list(struct.unpack(">%dI" % (len(c) // 4), c)))
    if uw == bw:
        return 1.0
    if not uw or not bw or not (0.5 <= len(uw) / len(bw) <= 2.0):
        return 0.0
    sm = difflib.SequenceMatcher(None, uw, bw, autojunk=False)
    if sm.real_quick_ratio() < 0.6 or sm.quick_ratio() < 0.6:
        return 0.0
    return sm.ratio()


def recheck_pair(bld, us, ua, ba, listed, min_ratio=0.6):
    """A pair from a pairs file points at a function of that file's list. The new starts can cut that
    extent into several functions. Keep the pair when its own extent still matches; else move it to the
    best matching function inside the old extent; else drop it (None)."""
    i = bld.index.get(ba)
    if i is None:
        return None
    uw = opcodes(us_words(us, ua))
    if pair_score(bld, us, ua, i, uw) >= min_ratio:
        return ba
    k = bisect.bisect_right(listed, ba)
    end = listed[k] if k < len(listed) else None
    best, at = 0.0, None
    j = i + 1
    while j < len(bld.starts) and bld.starts[j][1] == bld.starts[i][1] and (end is None or bld.starts[j][0] < end):
        sc = pair_score(bld, us, ua, j, uw)
        if sc > best:
            best, at = sc, bld.starts[j][0]
        j += 1
    return at if best >= min_ratio else None


def add_pair(bld, ua, ba):
    if ba not in bld.index or ua in bld.partner:
        return
    bld.partner[ua] = ba
    bld.rev.setdefault(ba, []).append(ua)


def path_strings(data):
    return [(m.start(), m.group(0)[3:-1].decode()) for m in PATH_STRING.finditer(data)]


def strings_by_vote(bld, data):
    """{vram: path} for the path strings in a build's game data. Its base address is not in the pairs
    files, so it is the one base that puts every path string under an address the code builds."""
    if not data:
        return {}
    strs = path_strings(data)
    if not strs:
        return {}
    b, v = bld.code["game"]
    built = data_refs(b, v)
    votes = {}
    for o, _ in strs:
        for a in built:
            if a >= 0x80000000 and a - o >= 0x80000000:
                votes[a - o] = votes.get(a - o, 0) + 1
    base, n = max(sorted(votes.items()), key=lambda t: t[1])
    if n < len(strs):
        note("%s: no data base puts all %d path strings under a code address; rare-name skips it"
             % (bld.name, len(strs)))
        return {}
    return {base + o: p for o, p in strs}


def names_of_functions(bld, strings):
    """{build function index: set of paths it references}."""
    out = {}
    if not strings:
        return out
    for i in range(len(bld.starts)):
        if bld.real[i] is None:
            continue
        c, s = bld.func_code(i)
        hit = {strings[a] for a in data_refs(c, s) if a in strings}
        if hit:
            out[i] = hit
    return out


# -------------------------------------------------------------------------------------------- analysis

def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--ects", help="ECTS build (August 2000): the ROM, or its game segment")
    ap.add_argument("--debug", help="debug build (October 2000): the ROM, or its decompressed game code")
    ap.add_argument("--ects-pairs", help="ects_pairs.json")
    ap.add_argument("--debug-pairs", help="function_pairs.tsv")
    ap.add_argument("--debug-functions", help="debug_functions.tsv")
    ap.add_argument("--us-bin", default=str(REPO / "conker" / "conker.us.bin"),
                    help="the US code bin from `make extract` (default conker/conker.us.bin)")
    ap.add_argument("--out", default=str(REPO / "docs" / "file-boundaries.tsv"))
    ap.add_argument("--candidates", default=str(REPO / "docs" / "file-boundary-candidates.tsv"))
    args = ap.parse_args()

    ypath = REPO / "conker" / "conker.us.yaml"
    cfg, seginfo, objects, blocks = load_layout(ypath)
    funcs = load_accounting(REPO / "tools" / "accounting.tsv")
    flags_of = load_flags(REPO / "conker" / "Makefile", REPO / "conker" / "ultralib.mk")

    usb = Path(args.us_bin)
    if not usb.exists():
        sys.exit("boundaries.py: %s is missing: run `make -C conker extract` first, or pass --us-bin" % usb)
    us_bytes = usb.read_bytes()
    if sha1(us_bytes) != cfg["sha1"]:
        sys.exit("boundaries.py: %s does not have the sha1 of conker.us.yaml" % usb)

    # ---- US
    real_funcs = [f for f in funcs if f[4] != "pad"]
    us_code = {}
    for seg in CODE_SEGMENTS:
        rom, vram = seginfo[seg]
        hi = max(a + n for a, n, _, s, *_ in funcs if s == seg)
        us_code[seg] = (us_bytes[rom:rom + hi - vram], vram)
    us = Build("us", [(f[0], f[3]) for f in real_funcs], us_code, exact_starts=True)
    for f in real_funcs:
        us.partner[f[0]] = f[0]
        us.rev[f[0]] = [f[0]]
    fname = {f[0]: f[2] for f in funcs}
    fproof = {f[0]: f[5] for f in funcs}
    obj_funcs = {}
    for f in real_funcs:
        obj_funcs.setdefault(f[6], []).append(f[0])

    # US path strings: every data segment of the yaml maps ROM to vram linearly
    us_strings = {}
    for name, (rom, vram) in seginfo.items():
        if name in CODE_SEGMENTS and name != "debugger":
            continue
        nxt = min([r for r, _ in seginfo.values() if r > rom] + [len(us_bytes)])
        for o, p in path_strings(us_bytes[rom:nxt]):
            us_strings[vram + o] = p

    # ---- old builds
    missing = []
    olds = []
    forms = {}
    if args.ects and args.ects_pairs:
        form, code, data, starts, pairs = load_ects(args.ects, args.ects_pairs)
        bld = make_old_build("ects", form, code, data, starts, pairs, us, [f[0] for f in real_funcs])
        olds.append((bld, data))
        forms["ects"] = form
    else:
        missing.append("ECTS")
        note("ECTS build or pairs not given (--ects, --ects-pairs): padding-ects is not checked")
    if args.debug and args.debug_pairs and args.debug_functions:
        form, code, data, starts, pairs = load_debug(args.debug, args.debug_pairs, args.debug_functions)
        bld = make_old_build("debug", form, code, data, starts, pairs, us, [f[0] for f in real_funcs])
        olds.append((bld, data))
        forms["debug"] = form
    else:
        missing.append("debug")
        note("debug build or pairs not given (--debug, --debug-pairs, --debug-functions): "
             "padding-debug is not checked")
    for k, f in forms.items():
        if f == "segment":
            note("%s: only the game segment was given: its init and debugger code%s are not checked"
                 % (k, " and its game data (path strings)" if k == "debug" else ""))

    # path names per US function, from every build
    names = {}                                   # US addr -> {(path, build)}
    for i, ps in names_of_functions(us, us_strings).items():
        for p in ps:
            names.setdefault(us.starts[i][0], set()).add((p, "us"))
    for bld, data in olds:
        for i, ps in names_of_functions(bld, strings_by_vote(bld, data)).items():
            for ua in bld.rev.get(bld.starts[i][0], []):
                for p in ps:
                    names.setdefault(ua, set()).add((p, bld.name))

    # data blocks per (group, kind), sorted
    dsec = {}
    for group, kind, va, owner in blocks:
        dsec.setdefault((group, kind), []).append((va, owner))
    for k in dsec:
        dsec[k].sort()

    def us_join(ua):
        return us.join[us.index[ua]] if ua in us.index else None

    def chained(fl, frm, to):
        """True when the US functions after fl[frm] up to fl[to] (frm <= to) all join the one before."""
        return all((us_join(fl[k]) or ("?",))[0] == "join" for k in range(frm + 1, to + 1))

    def old_relation(bld, ux, uy, lo_us, hi_us):
        """Relation of the partners of US functions ux < uy in bld, with the US order check."""
        bx, by = bld.partner.get(ux), bld.partner.get(uy)
        if bx is None or by is None:
            return None
        i, j = bld.index[bx], bld.index[by]
        if i >= j:
            return {"order": False, "rel": "unknown", "pad": None, "bx": bx, "by": by}
        if bld.starts[i][1] != bld.starts[j][1]:
            return {"order": False, "rel": "unknown", "pad": None, "bx": bx, "by": by}
        order = True
        for k in range(i + 1, j):
            ps = bld.rev.get(bld.starts[k][0], [])
            if ps and not any(lo_us < p < hi_us for p in ps):
                order = False
        rel, pad = bld.relation(i, j)
        return {"order": order, "rel": rel, "pad": pad, "bx": bx, "by": by}

    def nearest(bld, fl, side):
        """Index in fl of the paired function nearest the boundary: the last one (left) or first (right)."""
        rng = range(len(fl) - 1, -1, -1) if side == "left" else range(len(fl))
        for k in rng:
            if fl[k] in bld.partner:
                return k
        return None

    rows, cands = [], []
    by_seg = {}
    for o in objects:
        by_seg.setdefault(o[0], []).append(o)
    for seg in CODE_SEGMENTS:
        objs = by_seg.get(seg, [])
        for (s1, rom1, va1, t1, L), (s2, rom2, va2, t2, R) in zip(objs, objs[1:]):
            lf, rf = obj_funcs.get(L, []), obj_funcs.get(R, [])
            ev, notes, contra = [], [], []
            if not lf or not rf:
                rows.append((va2, L, R, "unproven", "none", "an object has no functions"))
                continue
            X, Y = lf[-1], rf[0]
            # US code
            j = us_join(Y)
            us_merge = None
            if j and j[0] == "pad":
                ev.append("padding-us")
                if j[1] > 12:
                    notes.append("us gap %d bytes" % j[1])
            elif j and j[0] == "join":
                asm = {0: "", 1: "asm on one side", 2: "asm on both sides"}[(t1 == "hasm") + (t2 == "hasm")]
                contra.append("us: R starts at a non-16-aligned address after L%s" % (" (%s)" % asm if asm else ""))
                us_merge = (X, Y, asm)
            # flags
            flags_proof = False
            both_c = t1 == "c" and t2 == "c"       # padding is proof only between two C objects
            asm_note = {0: "", 1: "asm on one side", 2: "asm on both sides"}[(t1 == "hasm") + (t2 == "hasm")]
            f1, f2 = flags_of(L, t1), flags_of(R, t2)
            if f1 != f2 and f1.rsplit(" ", 1)[0] == f2.rsplit(" ", 1)[0]:
                notes.append("CFLAGS differ (%s | %s), the -O and -mips flags do not"
                             % (f1.rsplit(" ", 1)[1], f2.rsplit(" ", 1)[1]))
            elif f1 != f2:
                ev.append("flags-change")
                # a hand-written side whose function next to the boundary has only search evidence
                weak = [fname[a] for a, t in ((X, t1), (Y, t2)) if t == "hasm" and fproof.get(a) == "s"]
                flags_proof = not weak
                notes.append("flags %s | %s%s" % (f1, f2, (" (search-tier asm, not a proof: %s)"
                                                           % ",".join(weak)) if weak else ""))
            # old builds
            proven_old = []
            rel_by = {}
            for bld, _ in olds:
                kx, ky = nearest(bld, lf, "left"), nearest(bld, rf, "right")
                if kx is None or ky is None:
                    rel_by[bld.name] = None
                    continue
                ux, uy = lf[kx], rf[ky]
                r = old_relation(bld, ux, uy, ux, uy)
                pinned = chained(lf, kx, len(lf) - 1) and chained(rf, 0, ky)
                r["pinned"] = pinned
                r["ux"], r["uy"] = ux, uy
                rel_by[bld.name] = r
            for bld, _ in olds:
                r = rel_by.get(bld.name)
                if not r or not r["order"]:
                    if r and not r["order"]:
                        notes.append("%s: order differs" % bld.name)
                    continue
                other = [o for o in rel_by if o != bld.name]
                ro = rel_by.get(other[0]) if other else None
                if r["rel"] == "diff":
                    ev.append("padding-" + bld.name)
                    k, gap, at = r["pad"]
                    msg = "%s: %d zero bytes before %s" % (bld.name, gap, h(at))
                    if not r["pinned"]:
                        msg += " (not pinned: %s..%s)" % (fname[r["ux"]], fname[r["uy"]])
                    notes.append(msg)
                    if r["pinned"] and not (ro and ro["order"] and ro["rel"] == "same"):
                        proven_old.append(bld.name)
                    elif r["pinned"]:
                        notes.append("%s contradicts" % other[0])
                elif r["rel"] == "same":
                    contra.append("%s: one object (%s joins %s)" % (bld.name, fname[r["uy"]], fname[r["ux"]]))
                    oc = "n.a."
                    if ro is None:
                        oc = "no partners" if other else "build missing"
                    elif not ro["order"]:
                        oc = "order differs"
                    else:
                        oc = {"diff": "contradicts", "same": "agrees", "unknown": "silent"}[ro["rel"]]
                    cands.append(merge_cand(va2, L, R, r["ux"], r["uy"], bld.name,
                                            "%s starts at %s right after %s, not 16-aligned"
                                            % (fname[r["uy"]], h(r["by"]), fname[r["ux"]]),
                                            order="yes", us_al=yn(Y % 16 == 0), other=oc, us=us_shows(j),
                                            extra=", ".join(x for x in ("" if r["pinned"] else "not pinned",
                                                                        asm_note) if x)))
            # data edge: C objects only (hand-written asm data is 4-aligned), .data and .rodata only (the
            # linker script places the sections with SUBALIGN(4), and .bss blocks need not be 16-aligned)
            code_proof = (both_c and ("padding-us" in ev or proven_old)) or flags_proof
            for (group, kind), bl in sorted(dsec.items()):
                if group != seg or kind == "bss" or not both_c:
                    continue
                rb = [k for k, (va, ow) in enumerate(bl) if ow == R]
                if not rb or rb[0] == 0:
                    continue
                va, _ = bl[rb[0]]
                prev_owner = bl[rb[0] - 1][1]
                if prev_owner != L:
                    continue
                if va % 16 == 0:
                    if "data-edge" not in ev:
                        ev.append("data-edge")
                    notes.append("%s at %s" % (kind, h(va)))
                elif code_proof:
                    notes.append("data split suspect: R's .%s starts at %s, not 16-aligned, but the code shows"
                                 " two objects" % (kind, h(va)))
                else:
                    contra.append("%s: R's block starts at %s, not 16-aligned" % (kind, h(va)))
                    cands.append(merge_cand(va2, L, R, X, Y, "us-" + kind,
                                            "R's .%s starts at %s, not 16-aligned, right after L's" % (kind, h(va)),
                                            order="n.a.", us_al=yn(Y % 16 == 0), other="n.a.", us=us_shows(j),
                                            extra=""))
            # rare names
            ln = [(k, p, b) for k, a in enumerate(lf) for p, b in sorted(names.get(a, ()))]
            rn = [(k, p, b) for k, a in enumerate(rf) for p, b in sorted(names.get(a, ()))]
            pinned_name = None
            for k1, p1, b1 in ln:
                for k2, p2, b2 in rn:
                    if p1 == p2:
                        c = "rare-name: both use %s (%s, %s)" % (p1, fname[lf[k1]], fname[rf[k2]])
                        if c not in contra:
                            contra.append(c)
                            cands.append(merge_cand(va2, L, R, lf[k1], rf[k2], "name-" + b1,
                                                    "%s and %s both use %s" % (fname[lf[k1]], fname[rf[k2]], p1),
                                                    order="n.a.", us_al=yn(Y % 16 == 0), other="n.a.",
                                                    us=us_shows(j), extra=""))
                    else:
                        pin = chained(lf, k1, len(lf) - 1) and chained(rf, 0, k2)
                        if pinned_name is None or (pin and not pinned_name[0]):
                            pinned_name = (pin, p1, p2, b1, b2)
            if pinned_name:
                ev.append("rare-name")
                pin, p1, p2, b1, b2 = pinned_name
                notes.append("names %s (%s) | %s (%s)%s" % (p1, b1, p2, b2, "" if pin else " (not pinned)"))
            if us_merge:
                ux, uy, asm = us_merge
                others = []
                for bld, _ in olds:
                    r = rel_by.get(bld.name)
                    others.append("%s %s" % (bld.name, "no partners" if r is None else "order differs"
                                             if not r["order"] else {"same": "agrees", "diff": "contradicts",
                                                                      "unknown": "silent"}[r["rel"]]))
                for m in missing:
                    others.append("%s missing" % m.lower())
                cands.append(merge_cand(va2, L, R, ux, uy, "us", "R starts at %s, not 16-aligned, right after L"
                                        % h(uy), order="n.a.", us_al="no", other=", ".join(others), us="join",
                                        extra=asm))
            # grade
            if contra and any(c.startswith("us:") for c in contra):
                grade = "unproven"
            elif (both_c and ("padding-us" in ev or proven_old)) or flags_proof or (pinned_name and pinned_name[0]):
                grade = "proven"
            elif ev:
                grade = "likely"
            else:
                grade = "unproven"
            if not both_c and any(k.startswith("padding-") for k in ev):
                notes.append("padding next to hand-written asm is not a proof (asm can hold .align, data and nops)")
            if contra:
                notes.append("against: " + "; ".join(contra))
            order = ["padding-us", "padding-ects", "padding-debug", "data-edge", "flags-change", "rare-name"]
            evs = ",".join(k for k in order if k in ev) or "none"
            rows.append((va2, L, R, grade, evs, "; ".join(notes)))

    # ---- split candidates: two adjacent functions of one US C object (padding inside hand-written asm
    # can be .align, data or nops, so asm objects give none)
    otype = {o[4]: o[3] for o in objects}
    for obj, fl in sorted(obj_funcs.items(), key=lambda t: t[1][0]):
        if otype.get(obj) != "c":
            continue
        # US padding inside the object
        for k in range(1, len(fl)):
            j = us_join(fl[k])
            if j and j[0] == "pad":
                cands.append(split_cand(fl[k], obj, fl[k - 1], fl[k], "us",
                                        "%d zero bytes before %s" % (j[1], h(j[2])),
                                        order="n.a.", al=yn(fl[k] % 16 == 0), other="n.a.", us="padding",
                                        extra=""))
        for bld, _ in olds:
            paired = [k for k, a in enumerate(fl) if a in bld.partner]
            for k1, k2 in zip(paired, paired[1:]):
                r = old_relation(bld, fl[k1], fl[k2], fl[k1], fl[k2])
                if not r or r["rel"] != "diff":
                    continue
                if bld.index[r["by"]] - bld.index[r["bx"]] - 1 > NEAR:
                    continue                  # far apart in that build: no candidate
                pos = [k for k in range(k1 + 1, k2 + 1) if (us_join(fl[k]) or ("?",))[0] != "join"]
                at = fl[pos[0]] if pos else fl[k2]
                other = [o for o, _ in olds if o is not bld]
                oc = "build missing"
                if other:
                    ob = other[0]
                    lk = [k for k in range(0, (pos[0] if pos else k2)) if fl[k] in ob.partner]
                    rk = [k for k in range((pos[0] if pos else k2), len(fl)) if fl[k] in ob.partner]
                    if not lk or not rk:
                        oc = "no partners"
                    else:
                        ro = old_relation(ob, fl[lk[-1]], fl[rk[0]], fl[lk[-1]], fl[rk[0]])
                        if not ro["order"]:
                            oc = "order differs"
                        else:
                            oc = {"diff": "agrees", "same": "contradicts", "unknown": "silent"}[ro["rel"]]
                _, gap, padat = r["pad"]
                al = "no"
                if pos:
                    al = "yes" if len(pos) == 1 else "yes (%d places: %s)" % (
                        len(pos), ",".join(h(fl[k]) for k in pos))
                cands.append(split_cand(at, obj, fl[k1], fl[k2], bld.name,
                                        "%s: %d zero bytes before %s" % (bld.name, gap, h(padat)),
                                        order=yn(r["order"]), al=al, other=oc, us=us_shows(us_join(at)),
                                        extra=""))

    # ---- write
    def header(what):
        cmd = ("python3 tools/boundaries.py --ects <ECTS ROM> --ects-pairs <ects_pairs.json> "
               "--debug <debug ROM> --debug-pairs <function_pairs.tsv> --debug-functions <debug_functions.tsv>")
        lines = ["# %s. Made by tools/boundaries.py; do not edit by hand." % what,
                 "# Command: " + cmd,
                 "# Builds used: %s. Missing: %s." % (
                     ", ".join("%s %s" % ("ECTS" if k == "ects" else k, "ROM" if f == "rom" else "game segment only")
                               for k, f in forms.items()) or "none", ", ".join(missing) or "none"),
                 "# Addresses are US vram. No ROM bytes. See ACCOUNTING.md, \"File boundaries\"."]
        return "\n".join(lines) + "\n"

    rows.sort()
    with open(args.out, "w") as f:
        f.write(header("File boundaries of the US code: one row for each boundary between two objects"))
        f.write("us_address\tleft_object\tright_object\tgrade\tevidence\tnote\n")
        for va, L, R, g, e, n in rows:
            f.write("%s\t%s\t%s\t%s\t%s\t%s\n" % (h(va), L, R, g, e, n))
    cands = sorted(set(cands))
    with open(args.candidates, "w") as f:
        f.write(header("Split and merge candidates for the US file boundaries"))
        f.write("kind\tus_address\tleft_object\tright_object\tleft_function\tright_function\tsource\t"
                "evidence\torder_agrees\tus_16_aligned\tother_old_build\tus_code\tnote\n")
        for c in cands:
            f.write("\t".join(c) + "\n")

    def count(key):
        out = {}
        for r in rows:
            for k in key(r):
                out[k] = out.get(k, 0) + 1
        return out
    print("boundaries: %d" % len(rows))
    print("by grade: " + ", ".join("%s %d" % kv for kv in sorted(count(lambda r: [r[3]]).items())))
    print("by evidence: " + ", ".join("%s %d" % kv for kv in sorted(count(lambda r: r[4].split(",")).items())))
    print("candidates: split %d, merge %d" % (sum(c[0] == "split" for c in cands), sum(c[0] == "merge" for c in cands)))
    return rows, cands


def merge_cand(va, L, R, ux, uy, source, evidence, order, us_al, other, us, extra):
    return ("merge", h(va), L, R, FN.get(ux, h(ux)), FN.get(uy, h(uy)), source, evidence, order, us_al, other,
            us, extra)


def split_cand(va, obj, ux, uy, source, evidence, order, al, other, us, extra):
    return ("split", h(va), obj, obj, FN.get(ux, h(ux)), FN.get(uy, h(uy)), source, evidence, order, al, other,
            us, extra)


def yn(b):
    return "yes" if b else "no"


def us_shows(j):
    """What the US code shows at an address: padding, join, flush (or odd)."""
    return {"pad": "padding"}.get(j[0], j[0]) if j else "n.a."


FN = {}

if __name__ == "__main__":
    for f in load_accounting(REPO / "tools" / "accounting.tsv"):
        FN[f[0]] = f[2]
    main()
