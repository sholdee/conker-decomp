#!/usr/bin/env python3
"""cw — cleanup workspaces: edit an already-matching C file into idiomatic C without changing a byte.

    cw.py new  FILE       private workspace ~/conker-ws/clean.<name> for src/FILE (e.g. game_79F10.c)
    cw.py check NAME      rebuild the workspace; RESULT: MATCH | MISMATCH | BUILD-FAIL, and on MISMATCH
                          the functions whose ROM bytes changed (with the first differing offset)
    cw.py land NAME [-m MSG]  3-way merge into main, rebuild, whole-bin sha gate, commit "MSG [cw]" (default "cleanup: FILE [cw]")
    cw.py list            open cleanup workspaces

NAME is the workspace name printed by `new` (clean.game_79F10). Same lock, RAM builds, crash recovery and
sha gate as mw.py. The verdict is the whole code bin's sha1: a cleanup lands only if the ROM is unchanged.
"""
import argparse, fcntl, filecmp, json, re, shutil, subprocess, sys, time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mw  # noqa: E402
from mw import MAIN, REPO, WS_ROOT, LOCK, BIN, INNER_SHA, run, sha1, git, clone_tree, ensure_build, changed_files, PRAGMA_RE  # noqa

TARGET_BIN = MAIN / "conker.us.bin"          # the original decompressed bin (target_path in the yaml)
HACK_RE = re.compile(r"#line\b|if \(1\) \{\s*\}|\| 0\b|\* 1\b|\bpad\w*\b|\bunused\w*\b|& 0\)|\bvolatile\b|"
                     r"\bdo \{\s*\} while \(0\)|^\s*;\s*$|\bgoto\b|\(void\) ?0", re.M)


def name_of(rel):
    return "clean." + rel.removeprefix("src/").removesuffix(".c").replace("/", "__")


def ws_of(name):
    ws = WS_ROOT / name
    if not (ws / "meta.json").exists():
        sys.exit(f"{ws}: not a cleanup workspace (cw.py new FILE first)")
    return ws, json.loads((ws / "meta.json").read_text())


def code_segments():
    import yaml
    cfg = yaml.safe_load((MAIN / "conker.us.yaml").read_text())
    segs = [s for s in cfg["segments"] if isinstance(s, dict) and s.get("type") == "code"]
    out = []
    for s, nxt in zip(segs, segs[1:] + [None]):
        out.append((s["vram"], s["start"]))
    return out  # [(vram, rom_start)]


def obj_functions(wc, rel):
    """[(name, vram, size)] for the .text symbols of this file's object (rel=None: every object), from the map."""
    obj = f"build/{rel[:-2]}.c.o" if rel else None
    rows, cur, objs = [], False, []
    start = end = 0
    for line in (wc / "build" / "conker.us.map").read_text(errors="replace").splitlines():
        m = re.match(r"^ \.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)", line)
        if m:
            cur = (m.group(3) == obj) if obj else m.group(3).endswith(".c.o")
            if cur:
                start, end = int(m.group(1), 16), int(m.group(1), 16) + int(m.group(2), 16)
                objs.append([start, end, []])
            continue
        if cur:
            m = re.match(r"^\s+0x([0-9a-f]+)\s+([A-Za-z_]\w*)\s*$", line)
            if m and start <= int(m.group(1), 16) < end:
                objs[-1][2].append((int(m.group(1), 16), m.group(2)))
    for start, end, syms in objs:
        syms.sort()
        for i, (a, n) in enumerate(syms):
            b = syms[i + 1][0] if i + 1 < len(syms) else end
            rows.append((n, a, b - a))
    return rows


def vram_to_rom(v, segs):
    best = None
    for vram, rom in segs:
        if vram <= v and (best is None or vram > best[0]):
            best = (vram, rom)
    return v - best[0] + best[1]


def cmd_new(a):
    if a.tree:
        return new_tree(a)
    rel = a.file if a.file.startswith("src/") else "src/" + a.file
    if not rel.endswith(".c"):
        rel += ".c"
    if not (MAIN / rel).exists():
        sys.exit(f"{MAIN / rel}: no such file")
    name = name_of(rel)
    ws = WS_ROOT / name
    if ws.exists():
        if not a.force:
            sys.exit(f"{ws} exists (use --force to recreate)")
        attic = WS_ROOT / ".attic"
        attic.mkdir(exist_ok=True)
        ws.rename(attic / f"{name}.{time.strftime('%Y%m%d-%H%M%S')}")
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_SH)
        if sha1(MAIN / BIN) != INNER_SHA:
            sys.exit("main is not a matching build right now; retry in a minute")
        ws.mkdir(parents=True)
        clone_tree(MAIN, ws / "conker")
        (ws / "base").mkdir()
        for d in ("src", "include"):
            subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN / d), str(ws / "base" / d)], check=True)
        for n in SPLAT_INPUTS:
            shutil.copy(MAIN / n, ws / "base" / n)
        head = git("rev-parse", "HEAD").strip()
    for n in ("ido", "tools"):
        (ws / n).symlink_to(REPO / n)
    (ws / "my").mkdir()
    (ws / "meta.json").write_text(json.dumps({"mode": "clean", "file": rel, "head": head}, indent=1))
    src = (ws / "conker" / rel).read_text(errors="replace")
    hacks = [(src.count("\n", 0, m.start()) + 1, m.group(0).strip()) for m in HACK_RE.finditer(src)]
    funcs = obj_functions(MAIN, rel)
    print(f"workspace : {ws}")
    print(f"edit      : {ws / 'conker' / rel}   ({len(funcs)} functions)")
    print(f"check     : python3 {REPO}/tools/cw.py check {name}")
    print(f"land      : python3 {REPO}/tools/cw.py land {name}")
    print(f"hack-like markers ({len(hacks)}): " + ", ".join(f"L{l}:{t}" for l, t in hacks[:40]))


def new_tree(a):
    """Workspace for a cross-file change (a shared header type and all its users): name = clean.<NAME>."""
    name = "clean." + a.file
    ws = WS_ROOT / name
    if ws.exists():
        if not a.force:
            sys.exit(f"{ws} exists (use --force to recreate)")
        attic = WS_ROOT / ".attic"
        attic.mkdir(exist_ok=True)
        ws.rename(attic / f"{name}.{time.strftime('%Y%m%d-%H%M%S')}")
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_SH)
        if sha1(MAIN / BIN) != INNER_SHA:
            sys.exit("main is not a matching build right now; retry in a minute")
        ws.mkdir(parents=True)
        clone_tree(MAIN, ws / "conker")
        (ws / "base").mkdir()
        for d in ("src", "include"):
            subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN / d), str(ws / "base" / d)], check=True)
        for n in SPLAT_INPUTS:
            shutil.copy(MAIN / n, ws / "base" / n)
        head = git("rev-parse", "HEAD").strip()
    for n in ("ido", "tools"):
        (ws / n).symlink_to(REPO / n)
    (ws / "my").mkdir()
    (ws / "meta.json").write_text(json.dumps({"mode": "clean", "file": None, "tree": a.file, "head": head}, indent=1))
    print(f"workspace : {ws}   (tree-wide: edit any file under conker/src and conker/include)")
    print(f"check     : python3 {REPO}/tools/cw.py check {name}")
    print(f"land      : python3 {REPO}/tools/cw.py land {name}")


YAML = "conker.us.yaml"
# splat's inputs: a workspace may change them (e.g. to link a file's own .rodata, or to label bss)
SPLAT_INPUTS = (YAML, "symbol_addrs.us.txt", "undefined_syms.us.txt", "undefined_funcs.us.txt")
SPLAT = "python3 ../tools/n64splat/split.py conker.us.yaml >/dev/null 2>&1 && python3 ../tools/filter_auto_syms.py . >/dev/null"


def splat_changes(ws):
    """The splat inputs the workspace changed (empty for workspaces made before this support)."""
    out = []
    for n in SPLAT_INPUTS:
        b = ws / "base" / n
        if b.exists() and not filecmp.cmp(b, ws / "conker" / n, shallow=False):
            out.append(n)
    return out


def yaml_changed(ws):
    return bool(splat_changes(ws))


def sync_from_main(ws):
    """Bring main's changes since the workspace's base into the workspace (3-way, like land in reverse).

    Other agents land while a workspace is open. Without this, a check pairs main's newer data layout
    (linker script, splat outputs) with the workspace's older sources and fails to link. Returns the
    files that conflict (left untouched); an empty list means the workspace is now on main."""
    conflicts = []
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_SH)  # never read main in the middle of a land
        rels = set()
        for d in ("src", "include"):
            for root in (MAIN / d, ws / "base" / d):
                rels |= {p.relative_to(root.parent) for p in root.rglob("*")
                         if p.is_file() and p.suffix in (".c", ".h", ".inc", ".s")}
        rels |= {Path(n) for n in SPLAT_INPUTS}
        for rel in sorted(rels):
            main_f, base_f, ws_f = MAIN / rel, ws / "base" / rel, ws / "conker" / rel
            if not base_f.parent.is_dir() and not main_f.exists():
                continue
            if main_f.exists() and base_f.exists() and filecmp.cmp(main_f, base_f, shallow=False):
                continue  # main did not change this file
            if not main_f.exists() and not base_f.exists():
                continue
            if not main_f.exists():  # main deleted it
                if ws_f.exists() and filecmp.cmp(ws_f, base_f, shallow=False):
                    ws_f.unlink()
                    base_f.unlink()
                continue
            untouched = (not ws_f.exists() and not base_f.exists()) or (
                ws_f.exists() and base_f.exists() and filecmp.cmp(ws_f, base_f, shallow=False))
            if untouched:
                merged = main_f.read_bytes()
            else:
                p = subprocess.run(["git", "merge-file", "-p", str(ws_f), str(base_f) if base_f.exists() else "/dev/null",
                                    str(main_f)], capture_output=True)
                if p.returncode != 0:
                    conflicts.append(str(rel))
                    continue
                merged = p.stdout
            ws_f.parent.mkdir(parents=True, exist_ok=True)
            base_f.parent.mkdir(parents=True, exist_ok=True)
            ws_f.write_bytes(merged)
            shutil.copy(main_f, base_f)
    return conflicts


def build(ws):
    wc = ws / "conker"
    conflicts = sync_from_main(ws)
    if conflicts:
        print("REBASE-CONFLICT: main changed these files in a way that conflicts with your edits: "
              + ", ".join(conflicts) + ". Copy main's file into the workspace and base/, apply your edit again, check.")
    ensure_build(wc)
    (wc / BIN).unlink(missing_ok=True)
    stamp = ws / ".splat_yaml"
    if not yaml_changed(ws) and not stamp.exists() and (wc / "asm").is_symlink():
        # this workspace does not change splat's inputs, but main may have (another land): its asm/ is
        # main's through the symlink, so take main's matching splat outputs too, then rebuild fully
        stale = [n for n in SPLAT_INPUTS + ("conker.ld", "undefined_syms_auto.txt", "undefined_funcs_auto.txt")
                 if (MAIN / n).exists() and (not (wc / n).exists() or not filecmp.cmp(MAIN / n, wc / n, shallow=False))]
        if stale:
            for n in stale:
                shutil.copy(MAIN / n, wc / n)
                if n in SPLAT_INPUTS:
                    shutil.copy(MAIN / n, ws / "base" / n)
            return wc, run("make -B -j8 2>&1", wc, timeout=1800)
    if yaml_changed(ws) or stamp.exists():
        cur = b"".join((wc / n).read_bytes() for n in SPLAT_INPUTS)
        if not stamp.exists() or stamp.read_bytes() != cur:
            # asm/ is a symlink to main's in a lean workspace: splat must never write through it
            if (wc / "asm").is_symlink():
                (wc / "asm").unlink()
                subprocess.run(["cp", "-a", "--reflink=auto", str(MAIN / "asm"), str(wc / "asm")], check=True)
            run(SPLAT, wc, timeout=1800)  # the yaml changed: regenerate asm/ and the linker script
            stamp.write_bytes(cur)
            return wc, run("make -B -j8 2>&1", wc, timeout=1800)
    # The Makefile has no header dependencies: after any include/ change, rebuild everything (~15 s),
    # or stale objects would hide breakage in files that use the changed header.
    # Workspaces created from a Makefile with generated header deps (cpp -MM) rebuild exactly the users.
    has_deps = "cpp -MM" in (wc / "Makefile").read_text(errors="replace")
    hdr = not has_deps and any(str(f).startswith("include/") for f in changed_files(ws))
    r = run(f"make {'-B ' if hdr else ''}-j8 2>&1", wc, timeout=1800)
    return wc, r


def cmd_check(a):
    ws, m = ws_of(a.name)
    rel = m["file"]
    for f in ([rel] if rel else [str(x) for x in changed_files(ws) if str(x).endswith(".c")]):
        src = (ws / "conker" / f).read_text(errors="replace")
        b = ws / "base" / f
        base = b.read_text(errors="replace") if b.exists() else ""
        if set(PRAGMA_RE.findall(src)) - set(PRAGMA_RE.findall(base)):
            print(f"RESULT: BUILD-FAIL (new GLOBAL_ASM pragmas in {f} are not allowed in a cleanup)")
            return 2
    wc, r = build(ws)
    if not (wc / BIN).exists():
        errs = [l for l in r.stdout.splitlines() if "rror" in l or "undefined reference" in l]
        print("\n".join(errs[-25:]) or r.stdout[-2500:])
        print("RESULT: BUILD-FAIL")
        log(ws, "BUILD-FAIL")
        return 2
    if sha1(wc / BIN) == INNER_SHA:
        print("RESULT: MATCH   (whole ROM byte-identical; land with cw.py land)")
        log(ws, "MATCH")
        return 0
    got, want = (wc / BIN).read_bytes(), TARGET_BIN.read_bytes()
    segs = code_segments()
    bad = []
    for n, v, size in obj_functions(wc, rel):
        rom = vram_to_rom(v, segs)
        g, w = got[rom:rom + size], want[rom:rom + size]
        if g != w:
            first = next(i for i in range(min(len(g), len(w))) if g[i] != w[i]) if g[:len(w)] != w[:len(g)] else min(len(g), len(w))
            bad.append(f"  {n}: first diff at +0x{first:X}")
    if len(got) != len(want):
        print(f"bin size {len(got)} vs target {len(want)} — a function in this file changed size")
    other = ""
    if not bad:
        d = next(i for i in range(min(len(got), len(want))) if got[i] != want[i])
        other = f" (difference outside this file's functions, first at bin offset 0x{d:X} — data/rodata or another file)"
    print("functions whose bytes changed:\n" + ("\n".join(bad) if bad else "  none" + other))
    print("RESULT: MISMATCH")
    log(ws, "MISMATCH", len(bad))
    return 1


def log(ws, res, n=None):
    with open(ws / "checks.jsonl", "a") as f:
        f.write(json.dumps({"t": time.time(), "result": res, "funcs_differ": n}) + "\n")


def refresh_accounting():
    """Regenerate tools/accounting.tsv (committed with every land); False if the accounting fails."""
    r = subprocess.run([sys.executable, str(REPO / "tools" / "accounting.py")], capture_output=True, text=True)
    if r.returncode:
        print(r.stdout[-1500:])
    return r.returncode == 0


def cmd_land(a):
    ws, m = ws_of(a.name)
    files = changed_files(ws)
    # files the workspace deleted (present in base, gone from the workspace), e.g. after merging files
    deleted = [p.relative_to(ws / "base") for d in ("src", "include") for p in (ws / "base" / d).rglob("*")
               if p.is_file() and p.suffix in (".c", ".h", ".s") and not (ws / "conker" / p.relative_to(ws / "base")).exists()]
    if not files and not splat_changes(ws) and not deleted:
        sys.exit("no changes in workspace")
    with open(LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        crashed = git("status", "--porcelain", "--", "conker/src", "conker/include").strip()
        if crashed or not (MAIN / BIN).exists():
            if crashed:
                print(f"recovering from an interrupted land: restoring {crashed}")
                git("checkout", "--", "conker/src", "conker/include")
                git("clean", "-fdq", "--", "conker/src", "conker/include")
            run("make -j16 2>&1", MAIN)
            if sha1(MAIN / BIN) != INNER_SHA:
                sys.exit("main does not build to the target sha even after recovery - fix main by hand")
        rels = [str(Path("conker") / f) for f in files]
        merged = {}
        for f in files:
            cur, base, new = MAIN / f, ws / "base" / f, ws / "conker" / f
            if not base.exists():
                if cur.exists():
                    sys.exit(f"{f}: created in workspace but also exists in main")
                merged[f] = new.read_bytes()
                continue
            if set(PRAGMA_RE.findall(new.read_text(errors="replace"))) - set(PRAGMA_RE.findall(base.read_text(errors="replace"))):
                sys.exit(f"{f}: adds GLOBAL_ASM pragmas — refusing")
            p = subprocess.run(["git", "merge-file", "-p", str(cur), str(base), str(new)], capture_output=True)
            if p.returncode != 0:
                sys.exit(f"{f}: merge conflict with main — rebase: copy main's file into the workspace, "
                         f"re-apply your edits, update base/{f} from main, check, land again")
            merged[f] = p.stdout
        ychanged = yaml_changed(ws)
        for n in splat_changes(ws):
            p = subprocess.run(["git", "merge-file", "-p", str(MAIN / n), str(ws / "base" / n), str(ws / "conker" / n)],
                               capture_output=True)
            if p.returncode != 0:
                sys.exit(f"{n}: merge conflict with main — copy main's file into the workspace and base/, re-apply, check, land")
            merged[Path(n)] = p.stdout
            files = files + [Path(n)]
            rels.append(str(Path("conker") / n))
        if ychanged:
            autos = {n: (MAIN / n).read_bytes() for n in ("undefined_funcs_auto.txt", "undefined_syms_auto.txt")}
        backup = {f: (MAIN / f).read_bytes() if (MAIN / f).exists() else None for f in files}
        for f, data in merged.items():
            (MAIN / f).write_bytes(data)
        for f in deleted:
            backup.setdefault(f, (MAIN / f).read_bytes() if (MAIN / f).exists() else None)
            (MAIN / f).unlink(missing_ok=True)
        if ychanged:
            run(SPLAT, MAIN, timeout=1800)
        (MAIN / BIN).unlink(missing_ok=True)
        r = run("make -B -j16 2>&1", MAIN)  # full rebuild: no header deps in the Makefile
        if not (MAIN / BIN).exists() or sha1(MAIN / BIN) != INNER_SHA:
            for f, data in backup.items():
                if data is None:
                    (MAIN / f).unlink(missing_ok=True)
                else:
                    (MAIN / f).write_bytes(data)
            if ychanged:
                run(SPLAT, MAIN, timeout=1800)
                for n, data in autos.items():
                    (MAIN / n).write_bytes(data)
            run("make -B -j16 2>&1", MAIN)
            print(r.stdout[-2500:])
            sys.exit("LAND-FAIL: main build does not match after merge; reverted")
        if not refresh_accounting():
            sys.exit("LAND-FAIL: tools/accounting.py fails after this change (main is matching but uncommitted: fix and land again)")
        git("add", *rels, "tools/accounting.tsv")
        for f in deleted:
            git("rm", "-q", "--cached", "--ignore-unmatch", str(Path("conker") / f))
        msg = f"{a.message.splitlines()[0]} [cw]" if a.message else f"cleanup: {m['file'] or m.get('tree')} [cw]"
        if a.message and len(a.message.splitlines()) > 1:
            msg += "\n\n" + "\n".join(a.message.splitlines()[1:]).strip()
        git("commit", "-q", "-m", msg + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
        # the merged files are now main's: make them the new merge base for further edits
        for f in files:
            (ws / "base" / f).parent.mkdir(parents=True, exist_ok=True)
            (ws / "base" / f).write_bytes((MAIN / f).read_bytes())
            (ws / "conker" / f).write_bytes((MAIN / f).read_bytes())
        print(f"LANDED: {m['file'] or m.get('tree')} — {git('rev-parse', '--short', 'HEAD').strip()}")


def cmd_list(a):
    for ws in sorted(WS_ROOT.glob("clean.*")):
        last = ""
        if (ws / "checks.jsonl").exists():
            lines = (ws / "checks.jsonl").read_text().splitlines()
            last = json.loads(lines[-1])["result"] if lines else ""
        print(f"{ws.name:40s} {last}")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    q = sub.add_parser("new"); q.add_argument("file", help="src file, or a NAME with --tree")
    q.add_argument("--tree", action="store_true", help="cross-file workspace named clean.NAME"); q.add_argument("--force", action="store_true")
    q = sub.add_parser("check"); q.add_argument("name")
    q = sub.add_parser("land"); q.add_argument("name"); q.add_argument("-m", "--message")
    sub.add_parser("list")
    a = p.parse_args()
    return {"new": cmd_new, "check": cmd_check, "land": cmd_land, "list": cmd_list}[a.cmd](a) or 0


if __name__ == "__main__":
    sys.exit(main())
