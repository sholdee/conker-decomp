#!/usr/bin/env python3
"""split_tu — split a C file into several translation units at given ROM offsets.

A TU boundary changes IDO's codegen (uopt carries state across a file), so a function can be
unmatchable only because it shares a file with code that was really in another file.

  split_tu.py plan  FILE OFF[,OFF...]        show which functions land in which new file
  split_tu.py apply FILE OFF[,OFF...]        rewrite src/FILE.c and create src/game_<OFF>.c files

FILE is a src-relative stem (game_1FA770). Each resulting file keeps EVERY top-level
declaration of the original (typedefs, externs, prototypes, file statics — unused ones cost
nothing) and only the functions whose address falls in its range. Stubs are re-pointed at
asm/nonmatchings/<newfile>/ (where splat writes them once the yaml has the split). The yaml
and the linker script are not touched here: run splat (see AGENTS.md) for those.
"""
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mw  # noqa: E402

PRAGMA = re.compile(r'#pragma GLOBAL_ASM\("asm/nonmatchings/([^"/]+)/(\w+)\.s"\)')
# `type name(a, b)` followed by a declaration: a K&R definition header (never a prototype, which
# would end right after the `)`).
KNR_HEAD = re.compile(r"\b[A-Za-z_]\w*\s*\(\s*[A-Za-z_]\w*(?:\s*,\s*[A-Za-z_]\w*)*\s*\)\s*[A-Za-z_][^=]*$", re.S)
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "__attribute__"}


def chunks(text):
    """Top-level items in order: [(kind, text)], kind in func|pragma|decl|pp|tail.
    Leading comments/whitespace belong to the item that follows them."""
    out, i, n, start = [], 0, len(text), 0
    depth, kind_open, prev_sig, knr = 0, None, "", False
    at_line_start = True
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2); i = n if j < 0 else j + 2; continue
        if text.startswith("//", i):
            j = text.find("\n", i); i = n if j < 0 else j; continue
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1; prev_sig = c; continue
        if c == "#" and depth == 0 and at_line_start:
            j = i
            while True:
                e = text.find("\n", j)
                if e < 0:
                    e = n; break
                if text[e - 1] == "\\":
                    j = e + 1; continue
                break
            body = text[start:e + 1]
            out.append(("pragma" if PRAGMA.search(body) else "pp", body))
            i = start = e + 1; at_line_start = True; continue
        if c == "\n":
            at_line_start = True; i += 1; continue
        if not c.isspace():
            at_line_start = False
        if c == "{":
            if depth == 0:
                kind_open = "func" if prev_sig == ")" or knr else "agg"
                knr = False
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and kind_open == "func":
                e = text.find("\n", i); e = n if e < 0 else e + 1
                out.append(("func", text[start:e])); i = start = e; at_line_start = True
                kind_open = None; prev_sig = "}"; continue
        elif c == ";" and depth == 0 and KNR_HEAD.search(strip_comments(text[start:i])):
            knr = True  # K&R parameter declaration: still inside the function header
        elif c == ";" and depth == 0:
            e = text.find("\n", i); e = n if e < 0 else e + 1
            if text[i + 1:e].strip():  # something else on the line: cut right after ';'
                e = i + 1
            out.append(("decl", text[start:e])); i = start = e
            at_line_start = text[e - 1] == "\n"
            kind_open = None; prev_sig = ";"; continue
        if not c.isspace():
            prev_sig = c
        i += 1
    if text[start:].strip() or start < n:
        out.append(("tail", text[start:]))
    return out


def strip_comments(s):
    return re.sub(r"/\*.*?\*/|//[^\n]*", " ", s, flags=re.S)


def func_name(chunk):
    head = strip_comments(chunk).split("{", 1)[0]
    for m in re.finditer(r"(\w+)\s*\(", head):
        if m.group(1) not in KEYWORDS:
            return m.group(1)
    return None


def prototype(chunk):
    head = strip_comments(chunk).split("{", 1)[0].strip()
    head = re.sub(r"^static\s+", "", re.sub(r"\s+", " ", head))
    m = KNR_HEAD.search(head)
    if m and not head.endswith(")"):  # K&R: declare without parameters, as the original saw it
        return head[:head.index("(", m.start())] + "();\n"
    return head + ";\n"


def name_of(kind, chunk):
    if kind == "pragma":
        return PRAGMA.search(chunk).group(2)
    if kind == "func":
        return func_name(chunk)
    return None


_MAP = None


def addr_of(name, root=mw.MAIN):
    """ROM offset of a function: from its .s header for stubs, else the link map."""
    global _MAP
    m = re.fullmatch(r"func_([0-9A-F]{8})", name)
    if m:
        return int(m.group(1), 16)
    if _MAP is None:
        _MAP = {}
        mp = root / "build" / "conker.us.map"
        for m in re.finditer(r"^\s+0x([0-9a-f]{8,16})\s+(\w+)\s*$", mp.read_text(), re.M):
            _MAP.setdefault(m.group(2), int(m.group(1), 16))
        bad = [n for n, v in _MAP.items() if re.fullmatch(r"func_[0-9A-F]{8}", n) and int(n[5:], 16) != v]
        if bad:
            raise SystemExit(f"{mp} is stale ({len(bad)} func_ symbols at the wrong address, e.g. {bad[0]}): rebuild first")
    return _MAP.get(name)


def file_segment(stem, root=mw.MAIN):
    """(rom_start, vram_start) of src/<stem>.c from the yaml."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    from rodata_extern import Rom
    rom = Rom.__new__(Rom)
    from rodata_extern import flat_segments
    rom.flat = flat_segments(root / "conker.us.yaml")
    return rom.text_vram(stem)


def plan(stem, offs, root=mw.MAIN):
    src = root / "src" / f"{stem}.c"
    text = src.read_text()
    rom0, vram0 = file_segment(stem, root)
    bounds = [(rom0, stem)] + [(o, f"game_{o:X}") for o in sorted(offs)]
    items = chunks(text)
    assert "".join(t for _, t in items) == text, "chunker lost text"
    placed = []  # (kind, text, target stem or None for declarations)
    for kind, t in items:
        nm = name_of(kind, t)
        if nm is None:
            placed.append((kind, t, None)); continue
        v = addr_of(nm, root)
        if v is None:
            raise SystemExit(f"{stem}: no address for {nm}")
        rom = v - vram0 + rom0
        tgt = [s for o, s in bounds if o <= rom][-1]
        placed.append((kind, t, tgt))
    return bounds, placed


def render(bounds, placed, stem):
    out = {}
    for _, s in bounds:
        parts = []
        for kind, t, tgt in placed:
            if tgt is None:
                parts.append(t)
            elif tgt == s:
                if kind == "pragma" and s != stem:
                    t = PRAGMA.sub(lambda m: f'#pragma GLOBAL_ASM("asm/nonmatchings/{s}/{m.group(2)}.s")', t)
                parts.append(t)
            elif kind == "func":
                # A definition earlier in the file was the prototype later callers saw: keep it.
                parts.append(prototype(t))
        out[s] = "".join(parts)
    return out


def transplant(func, attempt_text, target_text):
    """target_text with `func`'s stub (or old definition) replaced by its definition from
    attempt_text (a whole file from an earlier layout), plus the attempt's top-level
    declarations that target_text lacks, inserted just before it. None if func is absent."""
    att = chunks(attempt_text)
    fdef = next((t for k, t in att if k == "func" and func_name(t) == func), None)
    if fdef is None:
        return None
    norm = lambda t: re.sub(r"\s+", " ", strip_comments(t)).strip()
    tgt = chunks(target_text)
    have = {norm(t) for k, t in tgt if k in ("decl", "pp", "func")}
    have |= {norm(prototype(t)) for k, t in tgt if k == "func"}
    known = " ".join(strip_comments(t) for k, t in tgt if k != "pragma")

    def declares_known(t):
        """The identifier this declaration introduces is already declared in the target."""
        c = strip_comments(t)
        if "(" in c.split("{", 1)[0] and not c.lstrip().startswith("typedef"):
            nm = func_name(c)
        else:
            ids = re.findall(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?:=[^;]*)?;\s*$", c.strip())
            nm = ids[-1] if ids else None
        return nm is not None and re.search(rf"\b{re.escape(nm)}\b", known) is not None
    extra = [t for k, t in att if k in ("decl", "pp") and norm(t) and norm(t) not in have
             and not (k == "decl" and declares_known(t))]
    out, done = [], False
    for k, t in tgt:
        if not done and name_of(k, t) == func:
            out.extend(extra); out.append(fdef if fdef.endswith("\n") else fdef + "\n"); done = True
        else:
            out.append(t)
    return "".join(out) if done else None


def main():
    if sys.argv[1] == "transplant":  # transplant FUNC ATTEMPT_FILE TARGET_FILE (rewrites target)
        new = transplant(sys.argv[2], Path(sys.argv[3]).read_text(), Path(sys.argv[4]).read_text())
        if new is None:
            raise SystemExit(f"{sys.argv[2]}: not found in attempt or target")
        Path(sys.argv[4]).write_text(new)
        return
    cmd, stem, offs = sys.argv[1], sys.argv[2], [int(x, 16) for x in sys.argv[3].split(",")]
    root = Path(os.environ.get("SPLIT_ROOT", mw.MAIN))
    bounds, placed = plan(stem, offs, root)
    if cmd == "plan":
        for _, s in bounds:
            fs = [(k, name_of(k, t)) for k, t, g in placed if g == s]
            print(f"{s}: {len(fs)} functions ({sum(k == 'pragma' for k, _ in fs)} stubs)  "
                  + " ".join(n for _, n in fs))
        return
    for s, text in render(bounds, placed, stem).items():
        (root / "src" / f"{s}.c").write_text(text)
        print("wrote", f"src/{s}.c")


if __name__ == "__main__":
    main()
