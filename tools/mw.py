#!/usr/bin/env python3
"""mw — match workspaces for the conker decomp.

Every attempt at a function gets its own PRIVATE reflinked copy of conker/
(~0.15s, near-zero disk on btrfs). The workspace builds the real file in the real
project context, links the full code bin, and compares it against the ROM — so the
verdict is exact (whole-bin sha1) and there is no "isolated vs in-project" gap.
Workspaces never touch the main tree; `land` is the only writer, and it re-gates.

  mw.py new   FUNC [--force]   create ~/conker-ws/FUNC (+ m2c draft)
  mw.py check FUNC [-U N] [--no-diff]
                               build + verdict + asm diff
  mw.py land  FUNC             merge src/include edits into main, full sha gate, commit
  mw.py pick  [--min B] [--max B] [--no-jtbl] [--desc] [-n N]
                               list remaining stubs by size
  mw.py rm    FUNC             delete a workspace

Builds run inside the `conker-tc` podman container (see tools/toolchain/).
"""
import argparse
import fcntl
import filecmp
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

REPO = Path(os.environ.get("CONKER_REPO", Path.home() / "conker"))
MAIN = REPO / "conker"
WS_ROOT = Path(os.environ.get("CONKER_WS", Path.home() / "conker-ws"))
CONTAINER = os.environ.get("CONKER_CONTAINER", "conker-tc")
INNER_SHA = (MAIN / "conker.us.sha1").read_text().split()[0]
LOCK = REPO / ".mw_land.lock"
PRAGMA_RE = re.compile(r'^\s*#pragma\s+GLOBAL_ASM\("([^"]+)"\)', re.M)
WORD_RE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/", re.M)
BIN = "build/conker.us.bin"


def run(cmd, cwd, check=False, timeout=600):
    """Run a command inside the toolchain container."""
    full = ["podman", "exec", "-w", str(cwd), CONTAINER, "bash", "-c", cmd]
    p = subprocess.run(full, capture_output=True, text=True, timeout=timeout)
    if check and p.returncode:
        sys.exit(f"command failed: {cmd}\n{p.stdout[-2000:]}{p.stderr[-2000:]}")
    return p


def sha1(path):
    try:
        return hashlib.sha1(Path(path).read_bytes()).hexdigest()
    except FileNotFoundError:
        return None


def find_stub(root, func):
    """(c_file relative to root, asm path) for the GLOBAL_ASM stub of func, or None."""
    for c in sorted((root / "src").rglob("*.c")):
        text = c.read_text(errors="replace")
        if func not in text:
            continue
        for m in PRAGMA_RE.finditer(text):
            if Path(m.group(1)).stem == func:
                return c.relative_to(root), m.group(1)
    return None


def stubbed_asm(root):
    """Every .s path referenced by a live GLOBAL_ASM pragma under root/src."""
    out = {}
    for c in (root / "src").rglob("*.c"):
        for m in PRAGMA_RE.finditer(c.read_text(errors="replace")):
            out[m.group(1)] = c.relative_to(root)
    return out


def ws_dir(func):
    return WS_ROOT / func


def meta(func):
    return json.loads((ws_dir(func) / "meta.json").read_text())


def target_words(asm_path):
    text = Path(asm_path).read_text()
    return [(int(v, 16), w) for v, w in WORD_RE.findall(text)]


# ---------------------------------------------------------------- new

def cmd_new(a):
    func = a.func
    found = find_stub(MAIN, func)
    if not found:
        sys.exit(f"{func}: no GLOBAL_ASM stub in {MAIN}/src")
    cfile, asm = found
    if sha1(MAIN / BIN) != INNER_SHA:
        sys.exit("main conker/build is not a matching build — run `make -C conker` first")
    ws = ws_dir(func)
    if ws.exists():
        if not a.force:
            sys.exit(f"{ws} exists (use --force to recreate)")
        shutil.rmtree(ws)
    ws.mkdir(parents=True)
    subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN), str(ws / "conker")], check=True)
    for name in ("ido", "tools"):
        (ws / name).symlink_to(REPO / name)
    # Pristine copy of the editable trees: the merge base for `land`.
    (ws / "base").mkdir()
    for d in ("src", "include"):
        subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN / d), str(ws / "base" / d)], check=True)
    head = subprocess.run(["git", "-C", str(REPO), "rev-parse", "HEAD"],
                          capture_output=True, text=True).stdout.strip()
    words = target_words(MAIN / asm)
    m = {"func": func, "file": str(cfile), "asm": asm, "head": head,
         "vram": words[0][0], "size": 4 * len(words)}
    (ws / "meta.json").write_text(json.dumps(m, indent=1))
    (ws / "target.s").symlink_to(ws / "conker" / asm)

    # m2c draft, with the file's own declarations as context when it parses.
    wc = ws / "conker"
    ctx = ws / "ctx.c"
    src = (wc / cfile).read_text(errors="replace")
    (ws / "ctx_in.c").write_text(PRAGMA_RE.sub("", src))
    flags = ("-P -undef -D_LANGUAGE_C -D_FINALROM -DF3DEX_GBI_2 -D_MIPS_SZLONG=32 -D__sgi "
             "-DM2CTX -I . -I include -I include/2.0L -I include/2.0L/PR -I include/libc "
             "-I src/libultra/os -I src/libultra/audio -I src/libultra/io")
    run(f"cpp {flags} ../ctx_in.c > {ctx} 2>/dev/null", wc)
    r = run(f"python3 ../tools/mips_to_c/m2c.py --context {ctx} {asm}", wc, timeout=120)
    if r.returncode or not r.stdout.strip():
        r = run(f"python3 ../tools/mips_to_c/m2c.py {asm}", wc, timeout=120)
    (ws / "m2c.c").write_text(r.stdout or r.stderr)
    print(f"workspace : {ws}")
    print(f"edit      : {wc / cfile}   (replace the GLOBAL_ASM line for {func})")
    print(f"target    : {ws / 'target.s'}  ({m['size']} bytes, vram {m['vram']:08X})")
    print(f"m2c draft : {ws / 'm2c.c'}")
    print(f"check     : python3 {REPO}/tools/mw.py check {func}")


# ---------------------------------------------------------------- check

def built_words(wc, vram, size, func):
    """Words of func as linked in the workspace elf (start from the symbol table)."""
    r = run(f"mips-linux-gnu-nm -n build/conker.us.elf", wc)
    syms = []
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "TtWw":
            syms.append((int(parts[0], 16), parts[2]))
    start = next((ad for ad, n in syms if n == func), None)
    if start is None:
        return None, None
    later = [ad for ad, n in syms if ad > start]
    end = min(later) if later else start + size
    end = min(end, start + size + 0x4000)
    r = run(f"mips-linux-gnu-objdump -d -z --start-address=0x{start:x} "
            f"--stop-address=0x{end:x} build/conker.us.elf", wc)
    words = []
    for line in r.stdout.splitlines():
        mm = re.match(r"\s*([0-9a-f]+):\s+([0-9a-f]{8})\b", line)
        if mm:
            words.append(mm.group(2).upper())
    return start, words


def cmd_check(a):
    func = a.func
    ws = ws_dir(func)
    m = meta(func)
    wc = ws / "conker"
    want = [w for _, w in target_words(ws / "target.s")]

    # Must be genuinely de-stubbed: no live pragma may still pull in an .s with this glabel.
    for asm in stubbed_asm(wc):
        p = wc / asm
        if p.exists() and re.search(rf"^glabel {re.escape(func)}\b", p.read_text(), re.M):
            print(f"STUB: {func} is still provided by GLOBAL_ASM({asm}) — replace it with C")
            print("RESULT: STUB")
            return 3

    (wc / BIN).unlink(missing_ok=True)
    r = run("make -j4 build/conker.us.bin 2>&1", wc)
    if r.returncode or not (wc / BIN).exists():
        errs = [l for l in r.stdout.splitlines()
                if re.search(r"error|Error|undefined reference|cfe:|warning: .*implicit", l)]
        print("BUILD-FAIL")
        print("\n".join(errs[:25]) or r.stdout[-3000:])
        print("RESULT: BUILD-FAIL")
        return 2

    start, got = built_words(wc, m["vram"], m["size"], func)
    bin_ok = sha1(wc / BIN) == INNER_SHA
    if got is None:
        print(f"{func} not found in the linked elf")
        print("RESULT: BUILD-FAIL")
        return 2
    same = sum(1 for x, y in zip(want, got) if x == y)
    first = next((i for i, (x, y) in enumerate(zip(want, got)) if x != y), None)
    if first is None and len(got) != len(want):
        first = min(len(got), len(want))
    if bin_ok:
        print(f"MATCH: {func} — whole code bin sha1 matches the ROM. Run: mw.py land {func}")
        print("RESULT: MATCH")
        return 0
    if first is None:
        print(f"FUNC-BYTES-MATCH but the bin differs elsewhere (rodata/data/other funcs): "
              f"check for new string/float literals, statics, or edited headers.")
    else:
        print(f"DIFF: {same}/{len(want)} words equal at same offset; size built {4*len(got)} "
              f"vs target {4*len(want)}; first mismatch at +0x{4*first:X}")
    if not a.no_diff:
        d = run(f"python3 ../tools/asm-differ/diff.py -o {func} --format plain --no-pager "
                f"-U {a.U} --max-size {len(want) + 16} 2>&1", wc, timeout=300)
        out = d.stdout
        out = re.sub(r"\x1b\[[0-9;]*m", "", out)
        print(out.rstrip())
    print("RESULT: DIFF")
    return 1


# ---------------------------------------------------------------- land

def changed_files(ws):
    out = []
    for d in ("src", "include"):
        for p in (ws / "conker" / d).rglob("*"):
            if not p.is_file():
                continue
            rel = p.relative_to(ws / "conker")
            b = ws / "base" / rel
            if not b.exists() or not filecmp.cmp(p, b, shallow=False):
                out.append(rel)
    return out


def git(*args, check=True):
    p = subprocess.run(["git", "-C", str(REPO), *args], capture_output=True, text=True)
    if check and p.returncode:
        raise RuntimeError(f"git {' '.join(args)}: {p.stderr.strip()}")
    return p.stdout


def cmd_land(a):
    func = a.func
    ws = ws_dir(func)
    m = meta(func)
    files = changed_files(ws)
    if not files:
        sys.exit("no changes in workspace")
    with open(LOCK, "w") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        if not find_stub(MAIN, func):
            sys.exit(f"{func} is no longer a stub in main (already landed?)")
        rels = [str(Path("conker") / f) for f in files]
        dirty = git("status", "--porcelain", "--", *rels).strip()
        if dirty:
            sys.exit(f"main tree has uncommitted edits to: {dirty}")
        # Reject any new GLOBAL_ASM pragmas — landing must only remove stubs.
        for f in files:
            new = ws / "conker" / f
            base = ws / "base" / f
            nb = set(PRAGMA_RE.findall(base.read_text(errors="replace"))) if base.exists() else set()
            nn = set(PRAGMA_RE.findall(new.read_text(errors="replace")))
            if nn - nb:
                sys.exit(f"{f}: adds GLOBAL_ASM pragmas {sorted(nn - nb)} — refusing")
        merged = {}
        for f in files:
            cur, base, new = MAIN / f, ws / "base" / f, ws / "conker" / f
            if not base.exists():
                if cur.exists():
                    sys.exit(f"{f}: created in workspace but also exists in main")
                merged[f] = new.read_bytes()
                continue
            p = subprocess.run(["git", "merge-file", "-p", str(cur), str(base), str(new)],
                               capture_output=True)
            if p.returncode != 0:
                sys.exit(f"{f}: merge conflict with main — recreate the workspace or merge by hand")
            merged[f] = p.stdout
        backup = {f: (MAIN / f).read_bytes() if (MAIN / f).exists() else None for f in files}
        for f, data in merged.items():
            (MAIN / f).write_bytes(data)

        def restore():
            for f, data in backup.items():
                if data is None:
                    (MAIN / f).unlink(missing_ok=True)
                else:
                    (MAIN / f).write_bytes(data)
            run("make -j16 2>&1", MAIN)

        (MAIN / BIN).unlink(missing_ok=True)
        r = run("make -j16 2>&1", MAIN)
        ok = sha1(MAIN / BIN) == INNER_SHA and not find_stub(MAIN, func)
        if not ok:
            restore()
            print(r.stdout[-3000:])
            sys.exit(f"LAND-FAIL: {func} — main build does not match after merge; reverted")
        git("add", *rels)
        msg = f"game: match {func} ({m['size']} bytes) [mw]"
        git("commit", "-q", "-m", msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
        with open(REPO / "tools" / "mw_landed.tsv", "a") as fh:
            fh.write(f"{func}\t{m['file']}\t{m['size']}\n")
        git("add", "tools/mw_landed.tsv")
        git("commit", "-q", "--amend", "--no-edit")
        print(f"LANDED: {func} ({m['size']} bytes) — {git('rev-parse', '--short', 'HEAD').strip()}")


# ---------------------------------------------------------------- pick / rm

def cmd_pick(a):
    rows = []
    for asm, cfile in stubbed_asm(MAIN).items():
        p = MAIN / asm
        if not p.exists():
            continue
        text = p.read_text()
        n = len(WORD_RE.findall(text))
        if n * 4 < a.min or n * 4 > a.max:
            continue
        jt = bool(re.search(r"\bjr\s+\$(?!ra\b)", text))
        if jt and a.no_jtbl:
            continue
        if not re.search(r"^glabel ", text, re.M) or ".word 0x" in text and "jr" not in text:
            continue
        rows.append((n * 4, Path(asm).stem, str(cfile), jt))
    rows.sort(key=lambda r: (-r[0] if a.desc else r[0]))
    for size, func, cfile, jt in rows[: a.n]:
        print(f"{size}\t{func}\t{cfile}\t{'jtbl' if jt else ''}")
    print(f"# {len(rows)} stubs, {sum(r[0] for r in rows)} bytes", file=sys.stderr)


def cmd_rm(a):
    shutil.rmtree(ws_dir(a.func), ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("new"); p.add_argument("func"); p.add_argument("--force", action="store_true")
    p = sub.add_parser("check"); p.add_argument("func"); p.add_argument("-U", type=int, default=2)
    p.add_argument("--no-diff", action="store_true")
    p = sub.add_parser("land"); p.add_argument("func")
    p = sub.add_parser("pick"); p.add_argument("--min", type=int, default=0)
    p.add_argument("--max", type=int, default=10**9); p.add_argument("--no-jtbl", action="store_true")
    p.add_argument("--desc", action="store_true"); p.add_argument("-n", type=int, default=50)
    p = sub.add_parser("rm"); p.add_argument("func")
    a = ap.parse_args()
    sys.exit({"new": cmd_new, "check": cmd_check, "land": cmd_land,
              "pick": cmd_pick, "rm": cmd_rm}[a.cmd](a) or 0)


if __name__ == "__main__":
    main()
