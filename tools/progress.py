#!/usr/bin/env python3
"""progress.py — progress figures, a treemap of the code and badges, from committed files only.

    progress.py OUTDIR [--linked yes|no]

Reads tools/accounting.tsv (every function: size, origin, object) and conker/conker.us.yaml (every data
slice). No ROM and no build are needed. Writes into OUTDIR:
  progress.json          the figures (code, function names, field names, data names, data in source)
  treemap.svg            one rectangle per source file, sized by its code bytes, grouped by segment and
                         area, coloured by how it matches (C or hand-written asm)
  badge-*.svg            code matched, fully linked, functions named, data in source
`--linked` comes from the caller (CI runs it only after the gate, whose linkcheck passed).
"""
import argparse
import json
import math
import re
from pathlib import Path

import yaml

REPO = Path(__file__).resolve().parent.parent
# an address name: func_XXXXXXXX, an asm label D_XXXXXXXX, or a name that only adds a word to an address
ADDR_NAME = re.compile(r"^(?:func|D)_[0-9A-F]{8}$|_[0-9A-F]{8}$")
# Data that stays extracted on purpose (assets and game text, see tools/PERFILE.md rule 5), by ROM start
ASSET_DATA = {
    0x290D0: "rspboot and audio microcode text",
    0x2AC84: "crash screen font",
    0x2C960: "audio microcode data",
    0x2293F0: "font texture",
    0x259560: "debugger font",
    0x242F30: "resource manifest lists and names",
}
# Gaps that are padding, not data (reviewed): ROM start for .data/.rodata, address for .bss
PADDING_DATA = {
    0x24CAC8: "unit padding at the end of the actoremitter unit's .rodata",
    0x25A1E0: "ROM fill between the debugger code and the 8-aligned spawn tables",
    0x8003BC28: "unit padding in the .bss of the scheduler unit",
}
COLOURS = {"c": "#3fb950", "hasm": "#388bfd"}


# ---------------------------------------------------------------- figures
def code_figures():
    rows = []
    for line in (REPO / "tools" / "accounting.tsv").read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        name, _addr, size, seg, origin, _proof, obj = line.split("\t")[:7]
        rows.append((name, int(size), seg, origin, obj))
    funcs = [r for r in rows if r[3] in ("c", "hasm")]
    total = sum(r[1] for r in rows if r[3] != "pad")
    matched = sum(r[1] for r in funcs)
    named = sum(1 for r in funcs if not ADDR_NAME.match(r[0]))
    return rows, {
        "code_bytes": total, "matched_bytes": matched,
        "c_bytes": sum(r[1] for r in funcs if r[3] == "c"),
        "hasm_bytes": sum(r[1] for r in funcs if r[3] == "hasm"),
        "functions": len(funcs), "functions_named": named,
    }


def data_figures():
    """Sizes of the data slices in the init_data, game_data and debugger segments of the yaml."""
    y = yaml.safe_load((REPO / "conker" / "conker.us.yaml").read_text())
    segs = y["segments"]
    starts = [s["start"] if isinstance(s, dict) else s[0] for s in segs]
    out = {"in_source": 0, "asm": 0, "asset": 0, "padding": 0}
    for i, s in enumerate(segs):
        if not isinstance(s, dict) or "subsegments" not in s:
            continue
        rom_end = next((x for x in starts[i + 1:] if isinstance(x, int)), None)
        subs = []
        for e in s["subsegments"]:
            if isinstance(e, dict):
                subs.append((e.get("start"), e.get("type"), e.get("vram"), e.get("name")))
            else:
                subs.append((e[0], e[1], None, e[2] if len(e) > 2 else None))
        data = [x for x in subs if str(x[1]).lstrip(".") in ("data", "rodata")]
        bss = [x for x in subs if str(x[1]).lstrip(".") == "bss" and x[2] is not None]
        if not data and not bss:
            continue
        # .data/.rodata: a slice runs to the next subsegment start (any type) or the segment end
        all_starts = sorted({x[0] for x in subs if isinstance(x[0], int)} | ({rom_end} if rom_end else set()))
        for start, typ, _vram, _name in data:
            nxt = next((a for a in all_starts if a > start), None)
            if nxt is None:
                continue
            kind = "asset" if start in ASSET_DATA else "padding" if start in PADDING_DATA else (
                "in_source" if str(typ).startswith(".") else "asm")
            out[kind] += nxt - start
        # .bss: a slice runs to the next bss vram, the last one to vram + bss_size
        if bss and "bss_size" in s:
            vram0 = s["vram"] + (rom_end - s["start"])
            end = vram0 + s["bss_size"]
            vrams = sorted(x[2] for x in bss)
            for _start, typ, vram, _name in bss:
                nxt = next((v for v in vrams if v > vram), end)
                out["padding" if vram in PADDING_DATA else "in_source" if str(typ).startswith(".") else "asm"] += nxt - vram
    total = out["in_source"] + out["asm"]
    return {"data_bytes": total + out["asset"] + out["padding"], "data_in_source_bytes": out["in_source"],
            "data_asm_bytes": out["asm"], "data_asset_bytes": out["asset"], "data_padding_bytes": out["padding"]}


# ---------------------------------------------------------------- names of fields and data
# an unnamed field: unk, unk_1C, unk1C, unknown, unk1CRange (a guess on top of an offset is still unknown)
UNK_FIELD = re.compile(r"^unk(?:nown)?(?:_?[0-9A-Fa-f]|$|_?$)")
PAD_FIELD = re.compile(r"^pad")


def strip_c(text):
    """The text with comments, strings and character constants blanked, and continued lines joined."""
    text = re.sub(r"(\\|\?\?/)\r?\n", "", text)
    text = re.sub(r"^[ \t]*#.*$", "", text, flags=re.M)  # preprocessor lines
    out, i, n = [], 0, len(text)
    while i < n:
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(" " + "\n" * text.count("\n", i, j))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            i = n if j < 0 else j
        elif text[i] in "\"'":
            q, j = text[i], i + 1
            while j < n and text[j] != q and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            out.append(q + q)
            i = j + 1
        else:
            out.append(text[i])
            i += 1
    return "".join(out)


TAG_ONLY = re.compile(r"^\s*(?:(?:static|const|volatile|typedef)\s+)*(?:struct|union|enum)\s+\w+\s*$")


def declarator_names(stmt):
    """The names that a declaration statement (without its ';') declares, at its top level. A struct,
    union or enum definition without a declarator (`struct Tag { ... };`) declares none."""
    if TAG_ONLY.match(stmt):
        return []
    depth, parts, cur = 0, [], []
    for c in stmt:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if c == "," and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
    parts.append("".join(cur))
    names = []
    for part in parts:
        part = part.split("=", 1)[0]
        m = re.search(r"\(\s*\*+\s*(\w+)\s*(?:\[[^\]]*\]\s*)*\)", part)  # (*name)(...) or (*name[4])(...)
        if m:
            names.append(m.group(1))
            continue
        part = re.sub(r"\[[^\]]*\]", "", part.split(":", 1)[0])
        ids = re.findall(r"[A-Za-z_]\w*", part)
        if ids:
            names.append(ids[-1])
    return names


def statements(text):
    """(kind of the innermost block, statement) for each ';'-terminated statement of C text, where the kind
    is 'file', 'aggregate' (a struct or union body), 'enum' or 'other' (a function body or initializer).
    A statement after a struct body (`} name;`) keeps the head before the body (`typedef struct X`), and the
    parameter declarations of a K&R definition are not file-level statements."""
    stack, out, start, prefix, kr = [("file", "")], [], 0, "", []
    for i, c in enumerate(text):
        if c == "{":
            head = prefix + text[start:i]
            outer = stack[-1][0]
            kind = ("other" if outer in ("enum", "other") or (outer == "file" and "=" in head) else
                    "aggregate" if re.search(r"\b(struct|union)\b[^;{}()]*$", head) else
                    "enum" if re.search(r"\benum\b[^;{}()]*$", head) else "other")
            if outer == "file" and kind == "other" and kr:
                out = out[:len(out) - len(kr)]  # the K&R parameter declarations of this function
            kr = []
            stack.append((kind, head))
            start, prefix = i + 1, ""
        elif c == "}":
            kind, head = stack.pop() if len(stack) > 1 else ("file", "")
            start, prefix = i + 1, (re.sub(r"\{.*", "", head, flags=re.S) + " " if kind == "aggregate" else "")
        elif c == ";":
            stmt = prefix + text[start:i]
            out.append((stack[-1][0], stmt))
            if stack[-1][0] == "file" and (kr or re.search(r"\)\s*[A-Za-z_]", stmt)):
                kr.append(stmt)  # declarator `f(a, b)` followed by a type: a K&R parameter list
            start, prefix = i + 1, ""
    return out


def field_figures():
    """Members of the game's structs and unions (the SDK headers excluded). A field named pad* that no
    source file uses is padding and is not counted."""
    inc = REPO / "conker" / "include"
    headers = [inc / "structs.h", inc / "effects.h"] + sorted((inc / "conker").glob("*.h"))
    fields = []
    for h in headers:
        for kind, stmt in statements(strip_c(h.read_text())):
            if kind == "aggregate" and not stmt.lstrip().startswith("#"):
                fields += declarator_names(stmt)
    used = set()
    for p in (REPO / "conker" / "src").rglob("*"):
        if p.suffix in (".c", ".h"):
            used |= set(re.findall(r"(?:\.|->)\s*(pad\w*)", strip_c(p.read_text(errors="replace"))))
    counted = [f for f in fields if not (PAD_FIELD.match(f) and f not in used)]
    named = sum(1 for f in counted if not UNK_FIELD.match(f))
    return {"fields": len(counted), "fields_named": named}


def data_symbols():
    """{source file: [data symbols it defines]}: file-scope variables of the C files and the labels in the
    .data, .rodata and .bss sections of the hand-written .s files (the extracted assets are not source)."""
    out = {}
    src = REPO / "conker" / "src"
    for p in sorted(src.rglob("*")):
        rel = str(p.relative_to(src)).rsplit(".", 1)[0]
        if p.suffix == ".c":
            names = []
            for kind, stmt in statements(strip_c(p.read_text(errors="replace"))):
                s = stmt.strip()
                if kind != "file" or not s or s.startswith("#") or re.match(r"(typedef|extern)\b", s):
                    continue
                s = s.split("=", 1)[0]
                if re.search(r"\b\w+\s*\((?!\s*\*)", s):  # a function prototype, not a pointer to one
                    continue
                names += [n for n in declarator_names(s) if n not in ("struct", "union", "enum")]
            if names:
                out[rel] = names
        elif p.suffix == ".s":
            section, names = None, []
            for line in p.read_text(errors="replace").splitlines():
                m = re.match(r"\s*\.section\s+(\.\w+)", line)
                if m:
                    section = m.group(1)
                m = re.match(r"\s*glabel\s+(\w+)", line)
                if m and section in (".data", ".rodata", ".bss"):
                    names.append(m.group(1))
            if names:
                out[rel] = names
    return out


# ---------------------------------------------------------------- treemap
def squarify(items, x, y, w, h):
    """items: [(value, payload)] sorted descending. Returns [(x, y, w, h, payload)]."""
    out, items = [], [i for i in items if i[0] > 0]
    total = sum(v for v, _ in items)
    if not items or w <= 0 or h <= 0:
        return out
    scale = w * h / total
    vals = [(v * scale, p) for v, p in items]

    def worst(row, side):
        s = sum(v for v, _ in row)
        return max(max(side * side * v / (s * s), (s * s) / (side * side * v)) for v, _ in row)

    while vals:
        side = min(w, h)
        row = [vals.pop(0)]
        while vals and worst(row + [vals[0]], side) <= worst(row, side):
            row.append(vals.pop(0))
        s = sum(v for v, _ in row)
        if w >= h:
            cw = s / h
            cy = y
            for v, p in row:
                out.append((x, cy, cw, v / cw, p))
                cy += v / cw
            x, w = x + cw, w - cw
        else:
            ch = s / w
            cx = x
            for v, p in row:
                out.append((cx, y, v / ch, ch, p))
                cx += v / ch
            y, h = y + ch, h - ch
    return out


def esc(s):
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def treemap(rows, figures, syms=None, width=1200, height=720):
    files = {}
    for name, size, seg, origin, obj in rows:
        if origin not in ("c", "hasm"):
            continue
        path = re.sub(r"^build/src/|\.[cs]\.o$", "", obj)
        f = files.setdefault(path, {"bytes": 0, "origin": origin, "funcs": 0, "named": 0})
        f["bytes"] += size
        f["funcs"] += 1
        f["named"] += not ADDR_NAME.match(name)

    def group(path):
        parts = path.split("/")
        if parts[0] in ("game", "init", "debugger") and len(parts) > 2:
            return f"{parts[0]}/{parts[1]}"
        if parts[0] == "libultra":
            return "libultra"
        return f"{parts[0]}" if len(parts) > 1 else "address-named files"

    def data_note(path):
        ds = (syms or {}).get(path)
        if not ds:
            return ""
        return f', {len(ds)} data symbols ({sum(1 for n in ds if not ADDR_NAME.match(n))} named)'

    groups = {}
    for p, f in files.items():
        groups.setdefault(group(p), []).append((f["bytes"], (p, f)))
    head = 44
    svg = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height + head}" '
           f'font-family="Verdana,DejaVu Sans,sans-serif">',
           f'<rect width="{width}" height="{height + head}" fill="#0d1117"/>',
           f'<text x="10" y="18" font-size="14" fill="#e6edf3">Conker\'s Bad Fur Day (US): code by source file '
           f'({figures["code_bytes"]:,} bytes)</text>',
           f'<rect x="10" y="27" width="12" height="12" fill="{COLOURS["c"]}"/>'
           f'<text x="27" y="38" font-size="11" fill="#e6edf3">C, byte-identical ({figures["c_bytes"]:,} bytes)</text>',
           f'<rect x="300" y="27" width="12" height="12" fill="{COLOURS["hasm"]}"/>'
           f'<text x="317" y="38" font-size="11" fill="#e6edf3">hand-written asm ({figures["hasm_bytes"]:,} bytes)</text>']
    gitems = sorted(((sum(v for v, _ in g), (name, g)) for name, g in groups.items()), key=lambda t: -t[0])
    for gx, gy, gw, gh, (gname, members) in squarify(gitems, 0, head, width, height):
        pad = 14 if gw > 60 and gh > 30 else 0
        svg.append(f'<g><rect x="{gx:.1f}" y="{gy:.1f}" width="{gw:.1f}" height="{gh:.1f}" fill="#161b22" '
                   f'stroke="#0d1117" stroke-width="2"/>')
        if pad:
            svg.append(f'<text x="{gx + 3:.1f}" y="{gy + 11:.1f}" font-size="10" fill="#8b949e">'
                       f'{esc(gname)}</text>')
        for fx, fy, fw, fh, (path, f) in squarify(sorted(members, key=lambda t: -t[0]),
                                                  gx + 1, gy + pad, gw - 2, gh - pad - 1):
            svg.append(f'<rect x="{fx:.1f}" y="{fy:.1f}" width="{max(fw - 1, 0):.1f}" height="{max(fh - 1, 0):.1f}" '
                       f'fill="{COLOURS[f["origin"]]}"><title>{esc(path)}: {f["bytes"]:,} bytes, '
                       f'{f["funcs"]} functions ({f["named"]} named){data_note(path)}</title></rect>')
            label = path.split("/")[-1]
            if fw > 7 * len(label) + 6 and fh > 14:
                svg.append(f'<text x="{fx + 3:.1f}" y="{fy + 11:.1f}" font-size="9" fill="#0d1117">{esc(label)}</text>')
        svg.append("</g>")
    svg.append("</svg>")
    return "\n".join(svg)


# ---------------------------------------------------------------- badges
def badge(label, value, colour):
    def width(s):
        return int(6.6 * len(s)) + 12
    lw, vw = width(label), width(value)
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{lw + vw}" height="20" role="img" '
            f'aria-label="{esc(label)}: {esc(value)}"><title>{esc(label)}: {esc(value)}</title>'
            f'<linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/>'
            f'<stop offset="1" stop-opacity=".1"/></linearGradient>'
            f'<clipPath id="r"><rect width="{lw + vw}" height="20" rx="3" fill="#fff"/></clipPath>'
            f'<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="#555"/>'
            f'<rect x="{lw}" width="{vw}" height="20" fill="{colour}"/>'
            f'<rect width="{lw + vw}" height="20" fill="url(#s)"/></g>'
            f'<g fill="#fff" text-anchor="middle" font-family="Verdana,DejaVu Sans,sans-serif" font-size="11">'
            f'<text x="{lw / 2:.1f}" y="14">{esc(label)}</text>'
            f'<text x="{lw + vw / 2:.1f}" y="14">{esc(value)}</text></g></svg>')


def pct_colour(p):
    return "#3fb950" if p >= 99.95 else "#2da44e" if p >= 90 else "#d29922" if p >= 60 else "#e5534b"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("outdir")
    ap.add_argument("--linked", choices=("yes", "no"), default="yes")
    a = ap.parse_args()
    out = Path(a.outdir)
    out.mkdir(parents=True, exist_ok=True)
    rows, code = code_figures()
    data = data_figures()
    syms = data_symbols()
    names = [n for ns in syms.values() for n in ns]
    fig = {**code, **data, **field_figures(), "data_symbols": len(names),
           "data_symbols_named": sum(1 for n in names if not ADDR_NAME.match(n)), "linked": a.linked == "yes"}
    fig["matched_pct"] = round(100 * code["matched_bytes"] / code["code_bytes"], 2)
    fig["named_pct"] = round(100 * code["functions_named"] / code["functions"], 1)
    fig["fields_named_pct"] = round(100 * fig["fields_named"] / fig["fields"], 1) if fig["fields"] else 100.0
    fig["data_named_pct"] = (round(100 * fig["data_symbols_named"] / fig["data_symbols"], 1)
                             if fig["data_symbols"] else 100.0)
    denom = data["data_in_source_bytes"] + data["data_asm_bytes"]
    fig["data_in_source_pct"] = round(100 * data["data_in_source_bytes"] / denom, 1) if denom else 100.0
    (out / "progress.json").write_text(json.dumps(fig, indent=2) + "\n")
    (out / "treemap.svg").write_text(treemap(rows, code, syms))
    m = fig["matched_pct"]
    (out / "badge-matched.svg").write_text(badge("code matched", f"{m:g}%", pct_colour(m)))
    (out / "badge-linked.svg").write_text(badge("fully linked", "yes" if fig["linked"] else "no",
                                                "#3fb950" if fig["linked"] else "#e5534b"))
    n = fig["named_pct"]
    (out / "badge-named.svg").write_text(badge("functions named", f"{n:g}%", pct_colour(n)))
    d = fig["data_in_source_pct"]
    (out / "badge-data.svg").write_text(badge("data in source (excl. assets)", f"{d:g}%", pct_colour(d)))
    print(json.dumps({k: fig[k] for k in ("matched_pct", "named_pct", "fields_named_pct", "data_named_pct",
                                          "data_in_source_pct", "linked")}))


if __name__ == "__main__":
    main()
