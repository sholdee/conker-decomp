#!/usr/bin/env python3
"""mw — match workspaces for the conker decomp.

Every attempt at a function gets its own PRIVATE reflinked copy of conker/
(~0.15s, near-zero disk on btrfs). The workspace builds the real file in the real
project context, links the full code bin, and compares it against the ROM — so the
verdict is exact (whole-bin sha1) and there is no "isolated vs in-project" gap.
Workspaces never touch the main tree; `land` is the only writer, and it re-gates.

  mw.py new   FUNC [--force]   create $CONKER_WS/FUNC (+ m2c draft)
  mw.py check FUNC [-U N] [--no-diff] [-q]
                               build + verdict + asm diff
  mw.py batch FUNC V1.c V2.c … [-j N]
                               try full replacements of the function's .c file in parallel
  mw.py reset FUNC [--all]     restore the .c (or every edited file) from the base snapshot
  mw.py objdump FUNC           disassembly of the function as built (with relocations)
  mw.py land  FUNC             merge src/include edits into main, full sha gate, commit
  mw.py pick  [--min B] [--max B] [--no-jtbl] [--desc] [-n N]
                               list remaining stubs by size
  mw.py rm    FUNC             delete a workspace
  mw.py similar FUNC [-n N]    matched twins (identical code modulo symbols/regs/imms) + near twins
  mw.py note  FUNC [text…]     append to / print the attempt ledger (survives resumes)
  mw.py near  [-n N] [--backfill]
                               ratchet queue: best attempt of every open function, closest first

Builds run inside the `conker-tc` podman container (see tools/toolchain/).
Environment: CONKER_WS (workspace directory, default ~/conker-ws), CONKER_CONTAINER (default
conker-tc), CONKER_REPO (default: this script's checkout), CONKER_RAM (RAM build directory, default
/dev/shm/conker-build) and CONKER_RAM_MB (its budget).
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

# Repo root: this script's checkout (resolve() follows the tools/ symlinks in workspaces).
REPO = Path(os.environ.get("CONKER_REPO", Path(__file__).resolve().parent.parent))
MAIN = REPO / "conker"
WS_ROOT = Path(os.environ.get("CONKER_WS", Path.home() / "conker-ws"))
CONTAINER = os.environ.get("CONKER_CONTAINER", "conker-tc")
INNER_SHA = (MAIN / "conker.us.sha1").read_text().split()[0]
LOCK = REPO / ".mw_land.lock"
RAM_ROOT = Path(os.environ.get("CONKER_RAM", "/dev/shm/conker-build"))
RAM_BUDGET_MB = int(os.environ.get("CONKER_RAM_MB", "10000"))
BUILD_MB = 230                       # one build/ copy (~200 MB after the TU split; leave headroom)
SHARED_DIRS = {"asm", "expected", "assets"}   # read-only: symlinked to main, never copied
PRAGMA_RE = re.compile(r'^\s*#pragma\s+GLOBAL_ASM\("([^"]+)"\)', re.M)
WORD_RE = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/", re.M)
BIN = "build/conker.us.bin"


TCD_SOCK = WS_ROOT / ".tcd.sock"


def clone_tree(src, dst):
    """Lean private copy of a conker/ tree: read-only dirs are symlinks to main, sources are
    reflinked (on disk, so work survives a reboot), build/ lives in RAM (ensure_build).
    Full copies of ~7000 files per workspace were too slow on a slow disk."""
    dst.mkdir(parents=True)
    for item in src.iterdir():
        if item.name == "build":
            continue
        if item.name in SHARED_DIRS:
            (dst / item.name).symlink_to(MAIN / item.name)
            continue
        subprocess.run(["cp", "-a", "--reflink=auto", str(item), str(dst / item.name)], check=True)
    ensure_build(dst)


def ensure_build(wc):
    """Give tree wc a RAM build/ (copy of main's) unless it has a live one or a real on-disk one."""
    import time
    b = wc / "build"
    if b.exists() and not b.is_symlink():
        return  # full workspace from before the lean layout
    if b.is_symlink() and b.exists():
        os.utime(b.resolve().parent)  # LRU stamp
        return
    ram = RAM_ROOT / hashlib.sha1(str(wc).encode()).hexdigest()[:16]
    if ram.exists():
        shutil.rmtree(ram, ignore_errors=True)
    RAM_ROOT.mkdir(parents=True, exist_ok=True)
    evict_ram(BUILD_MB)
    ram.mkdir()
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_SH)  # not while a land rewrites main's build
        subprocess.run(["cp", "-a", str(MAIN / "build"), str(ram / "build")], check=True)
    if b.is_symlink():
        b.unlink()
    b.symlink_to(ram / "build")
    # Sources this tree changed must rebuild against main's newer objects.
    now = time.time()
    for d in ("src", "include"):
        for f in (wc / d).rglob("*"):
            if f.is_file():
                m = MAIN / f.relative_to(wc)
                if not m.exists() or not filecmp.cmp(f, m, shallow=False):
                    os.utime(f, (now, now))


def evict_ram(need_mb):
    """Drop least-recently-used RAM builds until need_mb more fits in the budget."""
    dirs = sorted((d for d in RAM_ROOT.iterdir() if d.is_dir()), key=lambda d: d.stat().st_mtime)
    while dirs and (len(dirs) * BUILD_MB + need_mb) > RAM_BUDGET_MB:
        shutil.rmtree(dirs.pop(0), ignore_errors=True)


def run(cmd, cwd, check=False, timeout=600):
    """Run a command inside the toolchain container — via the tcd socket when it is up
    (tools/toolchain/tcd.py; `podman exec` serializes on podman's lock under load)."""
    p = None
    if TCD_SOCK.exists():
        import socket
        import types
        try:
            with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as so:
                so.settimeout(timeout + 30)
                so.connect(str(TCD_SOCK))
                so.sendall((json.dumps({"cmd": cmd, "cwd": str(cwd), "timeout": timeout}) + "\n").encode())
                buf = b""
                while not buf.endswith(b"\n"):
                    chunk = so.recv(1 << 16)
                    if not chunk:
                        break
                    buf += chunk
            resp = json.loads(buf)
            p = types.SimpleNamespace(returncode=resp["rc"], stdout=resp["out"], stderr="")
        except (OSError, ValueError):
            p = None
    if p is None:
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
    ws = ws_dir(func)
    if ws.exists():
        if not a.force:
            sys.exit(f"{ws} exists (use --force to recreate)")
        attic = WS_ROOT / ".attic"
        attic.mkdir(exist_ok=True)
        import time
        os.rename(ws, attic / f"{func}.{time.strftime('%Y%m%d-%H%M%S')}")  # ratchet: never delete work
    # Snapshot main under a shared lock so a concurrent `land` can't be mid-build.
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_SH)
        if sha1(MAIN / BIN) != INNER_SHA:
            sys.exit("main conker/build is not a matching build — run `make -C conker` first")
        ws.mkdir(parents=True)
        clone_tree(MAIN, ws / "conker")
        # Pristine copy of the editable trees: the merge base for `land`.
        (ws / "base").mkdir()
        for d in ("src", "include"):
            subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN / d), str(ws / "base" / d)], check=True)
        head = subprocess.run(["git", "-C", str(REPO), "rev-parse", "HEAD"],
                              capture_output=True, text=True).stdout.strip()
    for name in ("ido", "tools"):
        (ws / name).symlink_to(REPO / name)
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
    seed = NEAR / f"{func}.json"
    if seed.exists():
        js = json.loads(seed.read_text())
        how = (f"{js['same']}/{js['n']} words, rows S/R/I {js.get('structural')}/{js.get('register')}/"
               f"{js.get('immediate')}" if "same" in js else f"old score {js.get('score')}")
        (ws / "prev.c").write_text(f"/* best earlier attempt ({how}); start from it */\n" + js.get("c", ""))
        if (NEAR / f"{func}.c").exists():
            shutil.copy(NEAR / f"{func}.c", ws / "prev_full.c")
    print(f"workspace : {ws}")
    print(f"edit      : {wc / cfile}   (replace the GLOBAL_ASM line for {func})")
    print(f"target    : {ws / 'target.s'}  ({m['size']} bytes, vram {m['vram']:08X})")
    print(f"m2c draft : {ws / 'm2c.c'}")
    if (ws / "prev.c").exists():
        print(f"prior try : {ws / 'prev.c'}  (whole file it came from: prev_full.c)")
    print(f"check     : python3 {REPO}/tools/mw.py check {func}")
    if (ws / "notes.md").exists():
        print("ledger    :\n" + (ws / "notes.md").read_text().rstrip())


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


def asm_diff(wc, func, n, U):
    d = run(f"python3 ../tools/asm-differ/diff.py -o {func} --format plain --no-pager "
            f"-U {U} --max-size {n + 16} 2>&1", wc, timeout=300)
    return re.sub(r"\x1b\[[0-9;]*m", "", d.stdout).rstrip()


def frame(words):
    """Stack frame size from the first `addiu sp,sp,-N` (0x27BDxxxx)."""
    for w in words[:8]:
        v = int(w, 16)
        if v >> 16 == 0x27BD and v & 0x8000:
            return 0x10000 - (v & 0xFFFF)
    return None


def diff_counts(wc, func, n):
    """Classify asm-differ rows: structural (insert/delete/opcode), register-only, immediate."""
    d = run(f"python3 ../tools/asm-differ/diff.py -o {func} --format json --no-pager "
            f"--max-size {n + 16} 2>/dev/null", wc, timeout=300)
    try:
        rows = json.loads(d.stdout)["rows"]
    except (ValueError, KeyError):
        return None
    c = {"structural": 0, "register": 0, "immediate": 0}
    for r in rows:
        b, cur = r.get("base"), r.get("current")
        if not b or not cur:
            c["structural"] += 1
            continue
        if b.get("mnemonic") != cur.get("mnemonic"):
            c["structural"] += 1
            continue
        fm = {t.get("format") for t in cur.get("text", []) + b.get("text", [])}

        def body(side):  # instruction text without address / source-line columns
            txt = "".join(t.get("text", "") for t in side.get("text", []) if t.get("format") != "source_line_num")
            txt = re.sub(r"^\s*[0-9a-f]+:\s*", "", txt)
            # same address, different spelling: D_/jtbl_<ADDR> (asm) vs .ro.<file>.<ADDR> (rodata_extern)
            txt = re.sub(r"\.ro\.\w+\.([0-9A-Fa-f]{8})|\b(?:D|jtbl)_([0-9A-Fa-f]{8})",
                         lambda mm: "SYM_" + (mm.group(1) or mm.group(2)).upper(), txt)
            return txt.split()
        if "register" in fm and not fm & {"immediate", "diff_change", "diff_add", "diff_remove"}:
            c["register"] += 1
        elif fm & {"immediate", "diff_change", "diff_add", "diff_remove"} or body(b) != body(cur):
            c["immediate"] += 1  # constants, offsets, and stack-slot differences (asm-differ 's' rows)
    return c


def check_tree(func, m, wc, want, quiet=False, U=2):
    """Build tree wc and judge func. Returns (code, output lines); 0 = MATCH."""
    out = []
    for asm in stubbed_asm(wc):
        p = wc / asm
        if p.exists() and re.search(rf"^glabel {re.escape(func)}\b", p.read_text(), re.M):
            return 3, [f"STUB: {func} is still provided by GLOBAL_ASM({asm}) — replace it with C",
                       "RESULT: STUB"]
    ensure_build(wc)
    obj = wc / "build" / (m["file"].removesuffix(".c") + ".c.o")
    obj.unlink(missing_ok=True)  # always recompile the function's own file
    (wc / BIN).unlink(missing_ok=True)
    r = run("make -j2 build/conker.us.bin 2>&1", wc)
    if r.returncode or not (wc / BIN).exists():
        errs = [l for l in r.stdout.splitlines()
                if re.search(r"error|Error|undefined|discarded|referenced in|multiple definition|"
                             r"cfe:|warning: .*implicit", l)]
        out += ["BUILD-FAIL", "\n".join(errs[:25]) or r.stdout[-3000:]]
        if obj.exists() and not quiet:
            out += ["(object compiled; link failed — object-level diff follows)",
                    asm_diff(wc, func, len(want), U)]
        return 2, out + ["RESULT: BUILD-FAIL"]

    start, got = built_words(wc, m["vram"], m["size"], func)
    if got is None:
        return 2, [f"{func} not found in the linked elf", "RESULT: BUILD-FAIL"]
    if sha1(wc / BIN) == INNER_SHA:
        return 0, [f"MATCH: {func} — whole code bin sha1 matches the ROM. Run: mw.py land {func}",
                   "RESULT: MATCH"]
    same = sum(1 for x, y in zip(want, got) if x == y)
    first = next((i for i, (x, y) in enumerate(zip(want, got)) if x != y), None)
    if first is None and len(got) != len(want):
        first = min(len(got), len(want))
    fw, fg = frame(want), frame(got)
    if fw != fg:
        out.append(f"FRAME: target allocates 0x{fw or 0:X} bytes of stack, yours 0x{fg or 0:X}")
    notes = Path(str(obj) + ".notes")
    if notes.exists():
        out.append("LITERALS: " + notes.read_text().strip().replace("\n", "\nLITERALS: "))
    if first is None:
        out.append("FUNC-BYTES-MATCH but the bin differs elsewhere (rodata/data/other funcs): "
                   "check for new string/float literals, statics, or edited headers.")
    else:
        out.append(f"DIFF: {same}/{len(want)} words equal; size built {4*len(got)} vs target "
                   f"{4*len(want)}; first mismatch at +0x{4*first:X} (function-relative)")
    c = diff_counts(wc, func, len(want))
    if c:
        out.append(f"ROWS: structural {c['structural']}  register-only {c['register']}  "
                   f"immediate/offset {c['immediate']}   (fix structural first; 0/0/0 = match)")
    if not quiet:
        out.append(asm_diff(wc, func, len(want), U))
    return 1, out + ["RESULT: DIFF"]


NEAR = REPO / ".nearmiss"   # ratchet store (local, gitignored): best attempt of every open function


def rank(rec):
    """Higher is better: match, then fewer structural rows, fewer register rows, more words equal."""
    if rec.get("result") == "MATCH":
        return (1, 0, 0, 1.0)
    if rec.get("result") != "DIFF":
        return (0, -10**6, -10**6, -1.0)
    return (0, -rec.get("structural", 10**5), -rec.get("register", 10**5),
            rec.get("same", 0) / max(1, rec.get("n", 1)))


def find_def(text, func):
    """(start, end) of the definition of func in C text, or None."""
    for mm in re.finditer(rf"\b{re.escape(func)}\s*\(", text):
        i = text.index("(", mm.start())
        depth = 0
        for j in range(i, len(text)):
            depth += {"(": 1, ")": -1}.get(text[j], 0)
            if depth == 0:
                break
        k = j + 1
        while k < len(text) and text[k] in " \t\r\n":
            k += 1
        if k >= len(text) or text[k] != "{":
            continue
        start = text.rfind("\n", 0, mm.start()) + 1
        depth = 0
        for e in range(k, len(text)):
            depth += {"{": 1, "}": -1}.get(text[e], 0)
            if depth == 0:
                return start, e + 1
    return None


def ratchet(ws, m, rec, src):
    """If rec beats the function's best, snapshot src (the whole .c) into ws/best/ and .nearmiss/."""
    import time
    func = m["func"]
    if rec.get("result") == "STUB" or not Path(src).exists():
        return
    bj = ws / "best" / "best.json"
    cur = json.loads(bj.read_text()) if bj.exists() else None
    nj = NEAR / f"{func}.json"
    try:
        glob_best = json.loads(nj.read_text()) if nj.exists() else None
    except ValueError:
        glob_best = None
    if cur and rank(rec) <= rank(cur):
        return
    text = Path(src).read_text(errors="replace")
    (ws / "best").mkdir(exist_ok=True)
    (ws / "best" / "file.c").write_text(text)
    bj.write_text(json.dumps(rec))
    if glob_best and "same" in glob_best and rank(rec) <= rank(glob_best):
        return  # an earlier workspace holds a better attempt
    span = find_def(text, func)
    entry = dict(rec, func=func, file=m["file"], size=m.get("size"), head=m.get("head"),
                 checks=sum(1 for _ in open(ws / "checks.jsonl")) if (ws / "checks.jsonl").exists() else None,
                 updated=time.strftime("%Y-%m-%d %H:%M"), c=text[span[0]:span[1]] if span else "")
    NEAR.mkdir(exist_ok=True)
    (NEAR / f"{func}.c").write_text(text)
    tmp = NEAR / f".{func}.json.tmp"
    tmp.write_text(json.dumps(entry, indent=1))
    os.replace(tmp, nj)


def log_check(ws, code, out, n, batch=False, src=None):
    """Append one line per check to ws/checks.jsonl."""
    import time
    rec = {"t": round(time.time(), 1), "result": ["MATCH", "DIFF", "BUILD-FAIL", "STUB"][code], "n": n}
    text = "\n".join(out)
    mm = re.search(r"DIFF: (\d+)/(\d+) words equal", text)
    if mm:
        rec["same"] = int(mm.group(1))
    if code == 0:
        rec["same"] = n
    mm = re.search(r"ROWS: structural (\d+)\s+register-only (\d+)\s+immediate/offset (\d+)", text)
    if mm:
        rec["structural"], rec["register"], rec["immediate"] = map(int, mm.groups())
    if batch:
        rec["batch"] = True
    try:
        with open(ws / "checks.jsonl", "a") as fh:
            fh.write(json.dumps(rec) + "\n")
        if src is not None:
            ratchet(ws, json.loads((ws / "meta.json").read_text()), rec, src)
    except (OSError, ValueError, KeyError):
        pass


def cmd_check(a):
    m = meta(a.func)             # a.func names the workspace; usually the function itself
    ws = ws_dir(a.func)
    want = [w for _, w in target_words(ws / "target.s")]
    code, out = check_tree(m["func"], m, ws / "conker", want, quiet=a.q or a.no_diff, U=a.U)
    log_check(ws, code, out, len(want), src=None if a.probe else ws / "conker" / m["file"])
    print("\n".join(out))
    return code


def cmd_batch(a):
    """Try several full replacements of the function's .c file in parallel slot trees."""
    from concurrent.futures import ThreadPoolExecutor
    func = a.func
    ws = ws_dir(func)
    m = meta(func)
    want = [w for _, w in target_words(ws / "target.s")]
    files = [Path(f).resolve() for f in a.files]
    nslot = min(a.j, len(files))
    slots = ws / "batch"
    for k in range(nslot):
        sd = slots / str(k)
        if sd.exists():
            shutil.rmtree(sd)
        sd.mkdir(parents=True)
        clone_tree(ws / "conker", sd / "conker")
        for name in ("ido", "tools"):
            (sd / name).symlink_to(REPO / name)
    free = list(range(nslot))
    import threading
    lock = threading.Lock()

    def one(f):
        with lock:
            k = free.pop()
        try:
            wc = slots / str(k) / "conker"
            shutil.copy(f, wc / m["file"])
            code, out = check_tree(func, m, wc, want, quiet=True)
            log_check(ws, code, out, len(want), batch=True, src=None if a.probe else f)
            summ = " | ".join(l for l in out if l.startswith(("DIFF", "ROWS", "FRAME", "MATCH", "BUILD-FAIL")))
            return f, code, summ
        finally:
            with lock:
                free.append(k)

    with ThreadPoolExecutor(nslot) as ex:
        results = list(ex.map(one, files))
    for f, code, summ in results:
        print(f"{f.name}\t{['MATCH', 'DIFF', 'BUILD-FAIL', 'STUB'][code]}\t{summ}")
    for f, code, _ in results:
        if code == 0:
            shutil.copy(f, ws / "conker" / m["file"])
            print(f"MATCH: {f.name} copied into the workspace — run: mw.py land {func}")
            break
    for k in range(nslot):  # free the slots' RAM builds too
        b = slots / str(k) / "conker" / "build"
        if b.is_symlink():
            shutil.rmtree(b.resolve().parent, ignore_errors=True)
    shutil.rmtree(slots, ignore_errors=True)


def cmd_reset(a):
    """Restore the function's .c file (or every edited file with --all) from the base snapshot."""
    ws = ws_dir(a.func)
    m = meta(a.func)
    files = changed_files(ws) if a.all else [Path(m["file"])]
    for f in files:
        b = ws / "base" / f
        if b.exists():
            shutil.copy(b, ws / "conker" / f)
        else:
            (ws / "conker" / f).unlink(missing_ok=True)
        print(f"reset {f}")


def cmd_objdump(a):
    """Disassembly of the function as currently built in the workspace."""
    ws = ws_dir(a.func)
    m = meta(a.func)
    wc = ws / "conker"
    obj = "build/" + m["file"].removesuffix(".c") + ".c.o"
    r = run(f"mips-linux-gnu-objdump -drz --disassemble={a.func} {obj}", wc)
    print(r.stdout[r.stdout.find("<" + a.func + ">"):] if r.stdout else r.stderr)


# ---------------------------------------------------------------- land

def changed_files(ws):
    out = []
    for d in ("src", "include"):
        for p in (ws / "conker" / d).rglob("*"):
            if not p.is_file() or p.suffix not in (".c", ".h", ".inc", ".s"):
                continue  # build artifacts (stray .o etc.) must never reach main
            rel = p.relative_to(ws / "conker")
            if not (ws / "base" / rel.parent).is_dir():
                continue  # agent scratch dirs (e.g. src/offtmp/) are not source
            b = ws / "base" / rel
            if not b.exists() or not filecmp.cmp(p, b, shallow=False):
                out.append(rel)
    return out


def git(*args, check=True):
    p = subprocess.run(["git", "-C", str(REPO), *args], capture_output=True, text=True)
    if check and p.returncode:
        raise RuntimeError(f"git {' '.join(args)}: {p.stderr.strip()}")
    return p.stdout


def stub_sizes(funcs):
    """{func: bytes} for funcs that are still stubs in main."""
    st = {Path(a).stem: a for a in stubbed_asm(MAIN)}
    return {f: 4 * len(WORD_RE.findall((MAIN / st[f]).read_text())) for f in funcs if f in st}


def cmd_land(a):
    ws = ws_dir(a.func)
    m = meta(a.func)
    targets = m.get("funcs") or [m["func"]]
    files = changed_files(ws)
    if not files:
        sys.exit("no changes in workspace")
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        # Only a land (holding this lock) edits main's sources, so uncommitted source edits seen
        # here are a land that died mid-way (killed agent, usage limit): restore and rebuild main.
        crashed = git("status", "--porcelain", "--", "conker/src", "conker/include").strip()
        if crashed or not (MAIN / BIN).exists():
            if crashed:
                print(f"recovering from an interrupted land: restoring {crashed}")
                git("checkout", "--", "conker/src", "conker/include")
                git("clean", "-fdq", "--", "conker/src", "conker/include")
            run("make -j16 2>&1", MAIN)
            if sha1(MAIN / BIN) != INNER_SHA:
                sys.exit("main does not build to the target sha even after recovery - fix main by hand")
        sizes = stub_sizes(targets)
        if not sizes:
            sys.exit(f"{', '.join(targets)}: no longer stubs in main (already landed?)")
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
        before = set(sizes)
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
        r = run("make -B -j16 2>&1", MAIN)  # full rebuild: no header deps in the Makefile
        still = {Path(x).stem for x in stubbed_asm(MAIN)}
        landed = sorted(before - still)
        if sha1(MAIN / BIN) != INNER_SHA or not landed:
            restore()
            print(r.stdout[-3000:])
            sys.exit(f"LAND-FAIL: {', '.join(targets)} — main build does not match after merge; reverted")
        total = sum(sizes[f] for f in landed)
        acc = subprocess.run([sys.executable, str(REPO / "tools" / "accounting.py")], capture_output=True, text=True)
        if acc.returncode:
            print(acc.stdout[-1500:])
            sys.exit("LAND-FAIL: tools/accounting.py fails after this change")
        git("add", *rels, "tools/accounting.tsv")
        if len(landed) == 1:
            msg = f"game: match {landed[0]} ({total} bytes) [mw]"
        else:
            msg = (f"game: match {len(landed)} functions ({total} bytes) [mw]\n\n"
                   + "\n".join(f"match {f} ({sizes[f]} bytes)" for f in landed))
        git("commit", "-q", "-m", msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
        for f in landed:  # the ratchet store only holds open functions
            for ext in (".json", ".c"):
                q = NEAR / f"{f}{ext}"
                q.unlink(missing_ok=True)
        print(f"LANDED: {', '.join(landed)} ({total} bytes) — {git('rev-parse', '--short', 'HEAD').strip()}")


# ---------------------------------------------------------------- similar / note

SIB_DIR = WS_ROOT / ".siblings"


def sibling_index():
    """{func: {"obj": path, "lv": [h0, h1, h2], "ops": [...level-1 tokens]}} over the golden build.
    Built once from `objdump -dr` of conker/expected (the golden objects never change)."""
    cache = SIB_DIR / "index.json"
    if cache.exists():
        return json.loads(cache.read_text())
    SIB_DIR.mkdir(parents=True, exist_ok=True)
    dump = SIB_DIR / "objdump.txt"
    run(f"find expected/build/src -name '*.c.o' | sort | while read o; do echo \"@@@ $o\"; "
        f"mips-linux-gnu-objdump -dr --no-show-raw-insn \"$o\"; done > {dump}", MAIN, timeout=1800)
    import hashlib as hl
    idx, cur, obj, insns = {}, None, None, []

    def flush():
        if cur and insns:
            lv = []
            for level in range(3):
                toks = [norm(i, level) for i in insns]
                lv.append(hl.sha1("\n".join(toks).encode()).hexdigest()[:16])
            idx[cur] = {"obj": obj, "lv": lv, "ops": [norm(i, 1) for i in insns]}

    def norm(ins, level):
        mnem, ops, sym, rel = ins
        if sym:
            ops = re.sub(r"(-?0x[0-9a-f]+|-?\d+)(?=\(|$)", "SYM", ops, count=1)
        if rel is not None:
            ops = re.sub(r"[0-9a-f]+ <[^>]*>", f"L{rel:+d}", ops)
        if level >= 1:
            ops = re.sub(r"\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|k[01]|gp|sp|fp|ra|\$?f\d+)\b", "R", ops)
        if level >= 2:
            ops = re.sub(r"-?0x[0-9a-f]+|\b-?\d+\b", "I", ops)
        return mnem + " " + ops

    start = 0
    for line in dump.read_text(errors="replace").splitlines():
        if line.startswith("@@@ "):
            flush(); cur, insns, obj = None, [], line[4:].replace("expected/", "", 1)
            continue
        mm = re.match(r"^([0-9a-f]+) <([\w.]+)>:$", line)
        if mm:
            flush(); insns = []
            cur = None if mm.group(2).startswith(".") else mm.group(2)
            start = int(mm.group(1), 16)
            continue
        mm = re.match(r"^\s+([0-9a-f]+):\s+(\S+)\s*(.*)$", line)
        if mm and cur:
            addr, mnem, ops = int(mm.group(1), 16), mm.group(2), mm.group(3).strip()
            bt = re.search(r"\b([0-9a-f]+) <", ops)
            rel = int(bt.group(1), 16) - addr if bt and not mnem.startswith("jal") else None
            insns.append([mnem, ops, False, rel])
            continue
        if re.match(r"^\s+[0-9a-f]+: R_MIPS_", line) and insns:
            insns[-1][2] = True
    flush()
    cache.write_text(json.dumps(idx))
    return idx


def cmd_similar(a):
    """Matched twins of FUNC (identical normalized code), then near twins, then unmatched twins."""
    import difflib
    idx = sibling_index()
    if a.func not in idx:
        sys.exit(f"{a.func} not in the golden objects")
    stubs = {Path(x).stem for x in stubbed_asm(MAIN)}
    me = idx[a.func]
    exact, unmatched = [], []
    for f, e in idx.items():
        if f == a.func:
            continue
        for level in range(3):
            if e["lv"][level] == me["lv"][level]:
                (unmatched if f in stubs else exact).append((level, f, e["obj"]))
                break
    exact.sort(); unmatched.sort()
    for level, f, obj in exact[: a.n]:
        print(f"TWIN level {level} ({['same code', 'same up to registers', 'same up to registers+immediates'][level]}): "
              f"{f}  in {obj.replace('build/', '').replace('.c.o', '.c')}  (matched — copy its C, adapt names)")
    n = len(me["ops"])
    near = []
    for f, e in idx.items():
        if f == a.func or f in stubs or abs(len(e["ops"]) - n) > max(4, n // 8):
            continue
        sm = difflib.SequenceMatcher(None, me["ops"], e["ops"], autojunk=False)
        if sm.real_quick_ratio() < 0.85 or sm.quick_ratio() < 0.85:
            continue
        r = sm.ratio()
        if r >= 0.85:
            near.append((r, f, e["obj"]))
    for r, f, obj in sorted(near, reverse=True)[: a.n]:
        if any(f == x[1] for x in exact):
            continue
        print(f"NEAR {r:.0%}: {f}  in {obj.replace('build/', '').replace('.c.o', '.c')}  (matched, similar shape)")
    if unmatched:
        print(f"UNMATCHED TWINS (one match may serve all): {', '.join(f for _, f, _ in unmatched[: a.n])}")
    if not exact and not near:
        print("no matched twin or near twin")


def cmd_best(a):
    """Show the ratchet's best attempt; --reset pins the CURRENT workspace file as the best
    (use when the recorded best was a deliberately wrong probe)."""
    ws = ws_dir(a.func)
    m = meta(a.func)
    if a.reset:
        want = [w for _, w in target_words(ws / "target.s")]
        code, out = check_tree(m["func"], m, ws / "conker", want, quiet=True)
        log_check(ws, code, out, len(want))
        rec = json.loads(open(ws / "checks.jsonl").read().splitlines()[-1])
        src = ws / "conker" / m["file"]
        for q in (ws / "best" / "best.json", NEAR / f"{m['func']}.json"):
            q.unlink(missing_ok=True)
        ratchet(ws, m, rec, src)
        print(f"best reset to the current file: {rec}")
    for q in (ws / "best" / "best.json", NEAR / f"{m['func']}.json"):
        if q.exists():
            e = json.loads(q.read_text())
            print(f"{q}: {e.get('result')} {e.get('same')}/{e.get('n')} S/R/I "
                  f"{e.get('structural')}/{e.get('register')}/{e.get('immediate')}")


def cmd_near(a):
    """The ratchet queue: every open function's best attempt, closest first."""
    stubs = {Path(x).stem for x in stubbed_asm(MAIN)}
    if a.backfill:
        n = 0
        for mj in sorted(WS_ROOT.glob("*/meta.json")):
            ws = mj.parent
            try:
                m = json.loads(mj.read_text())
            except ValueError:
                continue
            if m["func"] not in stubs or not (ws / "checks.jsonl").exists() or "funcs" in m:
                continue
            recs = [json.loads(l) for l in open(ws / "checks.jsonl") if l.strip()]
            recs = [r for r in recs if not r.get("batch")]
            if not recs:
                continue
            best = max(recs, key=rank)
            src = ws / "best" / "file.c"
            if not src.exists():
                src = ws / "conker" / m["file"]   # pre-ratchet workspace: latest file, best numbers
                best = dict(best, file_is_latest=True)
            ratchet(ws, m, best, src)
            n += 1
        print(f"backfilled from {n} workspaces")
    rows = []
    for j in NEAR.glob("*.json"):
        try:
            e = json.loads(j.read_text())
        except ValueError:
            continue
        if e.get("func") in stubs and "same" in e:
            rows.append(e)
    rows.sort(key=rank, reverse=True)
    print(f"{'function':16} {'size':>5} {'words':>9} {'S/R/I':>11}  checks  updated")
    for e in rows[: a.n]:
        print(f"{e['func']:16} {e.get('size') or 0:5} {e['same']:4}/{e['n']:<4} "
              f"{e.get('structural', '-')!s:>3}/{e.get('register', '-')!s:>3}/{e.get('immediate', '-')!s:<3}  "
              f"{e.get('checks') or '-':>6}  {e.get('updated', '')}{'  (latest, not best)' if e.get('file_is_latest') else ''}")
    print(f"# {len(rows)} open near-misses", file=sys.stderr)


def cmd_note(a):
    """Append to (or print) the function's attempt ledger, kept across agents and resumes."""
    ws = ws_dir(a.func)
    led = ws / "notes.md"
    if a.text:
        import time
        with open(led, "a") as fh:
            fh.write(f"- {time.strftime('%Y-%m-%d %H:%M')} {' '.join(a.text)}\n")
    if led.exists():
        print(led.read_text().rstrip())


# ---------------------------------------------------------------- pick / rm

def skip_list():
    """Functions in an optional, local tools/skip.txt that `pick` should leave out."""
    p = MAIN.parent / "tools" / "skip.txt"
    return {l.split()[0] for l in p.read_text().splitlines() if l.strip() and not l.startswith("#")} if p.exists() else set()


def cmd_pick(a):
    rows = []
    skip = skip_list()
    for asm, cfile in stubbed_asm(MAIN).items():
        p = MAIN / asm
        if not p.exists() or Path(asm).stem in skip:
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
    p.add_argument("-q", action="store_true", help="summary line only")
    p.add_argument("--probe", action="store_true", help="diagnostic variant: never recorded as a best attempt")
    p = sub.add_parser("batch", help="try variant .c files in parallel")
    p.add_argument("--probe", action="store_true", help="diagnostic variants: never recorded as best attempts")
    p.add_argument("func"); p.add_argument("files", nargs="+"); p.add_argument("-j", type=int, default=4)
    p = sub.add_parser("reset"); p.add_argument("func"); p.add_argument("--all", action="store_true")
    p = sub.add_parser("objdump"); p.add_argument("func")
    p = sub.add_parser("land"); p.add_argument("func")
    p = sub.add_parser("pick"); p.add_argument("--min", type=int, default=0)
    p.add_argument("--max", type=int, default=10**9); p.add_argument("--no-jtbl", action="store_true")
    p.add_argument("--desc", action="store_true"); p.add_argument("-n", type=int, default=50)
    p = sub.add_parser("rm"); p.add_argument("func")
    p = sub.add_parser("similar", help="matched twins / near twins of FUNC")
    p.add_argument("func"); p.add_argument("-n", type=int, default=5)
    p = sub.add_parser("best", help="show the ratchet best; --reset pins the current file as best")
    p.add_argument("func"); p.add_argument("--reset", action="store_true")
    p = sub.add_parser("near", help="ratchet queue: best attempt of every open function, closest first")
    p.add_argument("-n", type=int, default=40); p.add_argument("--backfill", action="store_true")
    p = sub.add_parser("note", help="append to / print the function's attempt ledger")
    p.add_argument("func"); p.add_argument("text", nargs="*")
    a = ap.parse_args()
    sys.exit({"new": cmd_new, "check": cmd_check, "land": cmd_land, "batch": cmd_batch,
              "reset": cmd_reset, "objdump": cmd_objdump,
              "pick": cmd_pick, "rm": cmd_rm, "similar": cmd_similar, "note": cmd_note, "near": cmd_near, "best": cmd_best}[a.cmd](a) or 0)


if __name__ == "__main__":
    main()
