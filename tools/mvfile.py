#!/usr/bin/env python3
"""mvfile.py — move (rename) source files, with every reference, in one gated batch.

    mvfile.py MOVES.tsv [--dry-run] [-m MESSAGE]

MOVES.tsv has one `old<TAB>new` pair per line (# comments allowed). Both are paths under conker/src
without the extension, as the yaml names them: `game_C4DC0` -> `game/Script/Interp/interp`. A hand-written
asm unit (`.s`) moves the same way. The batch:
  1. validates: each old file exists; each new path is free; new file names (the last part) are unique in
     the tree; path parts are lowercase-or-CamelCase identifiers;
  2. under the land lock: git-moves the files, then rewrites every reference: the yaml entries (code and
     data slices), per-file Makefile flags, tools/subsystems.tsv, tools/hasm_proofs.tsv,
     tools/linkcheck_allow.tsv, and mentions of the
     old name in conker/src and conker/include (header group comments, `game_X.c` in comments);
  3. re-runs splat (the yaml changed), rebuilds with make -B, and commits only if the whole-ROM sha1 still
     matches (also regenerating tools/accounting.tsv); otherwise it restores everything exactly.
"""
import argparse
import fcntl
import os
import re
import subprocess
import sys
from pathlib import Path

COAUTHOR = os.environ.get("CONKER_COAUTHOR", "Claude Opus 5.5 <noreply@anthropic.com>")  # the commit trailer (AGENTS.md, Paths)

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mw  # noqa: E402

M, REPO = mw.MAIN, mw.REPO
SRC = M / "src"
PART = re.compile(r"^[A-Za-z][A-Za-z0-9_]*$")
SPLAT = "python3 ../tools/n64splat/split.py conker.us.yaml >/dev/null 2>&1 && python3 ../tools/filter_auto_syms.py . >/dev/null"
TEXT_FILES = ["src/**/*.c", "src/**/*.h", "src/**/*.s", "include/**/*.h"]


def load(path):
    out = []
    for line in Path(path).read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            old, new = line.split()[:2]
            out.append((old.strip("/"), new.strip("/")))
    return out


def source_of(name):
    for ext in (".c", ".s"):
        if (SRC / f"{name}{ext}").exists():
            return SRC / f"{name}{ext}"
    return None


def validate(moves):
    errors = []
    olds = [o for o, _ in moves]
    news = [n for _, n in moves]
    if len(set(olds)) != len(olds) or len(set(news)) != len(news):
        errors.append("an old or new path appears twice")
    existing = {p.stem for p in SRC.rglob("*") if p.suffix in (".c", ".s")} - {Path(o).name for o in olds}
    seen = set()
    for old, new in moves:
        if source_of(old) is None:
            errors.append(f"{old}: no src/{old}.c or .s")
        if source_of(new) is not None and new not in olds:  # a path that this batch frees is allowed
            errors.append(f"{new}: already exists")
        if not all(PART.match(p) for p in new.split("/")):
            errors.append(f"{new}: each path part must be an identifier")
        base = Path(new).name
        if base in existing or base in seen:
            errors.append(f"{new}: file name '{base}' is not unique in src/")
        seen.add(base)
    return errors


def ordered(moves):
    """The moves in an order in which each new path is free: a move whose old path is another move's new
    path goes first. A cycle (a swap) has no such order."""
    left, out = list(moves), []
    while left:
        olds = {o for o, _ in left}
        free = [m for m in left if m[1] not in olds]
        if not free:
            sys.exit("INVALID:\n  the moves form a cycle: " + ", ".join(f"{o} -> {n}" for o, n in left))
        out += free
        left = [m for m in left if m not in free]
    return out


def yaml_mentions(name):
    """True if the yaml still has `name` as a whole path (a subsegment that the rewrite did not reach)."""
    return re.search(rf"(?<![\w/]){re.escape(name)}(?![\w/])", (M / "conker.us.yaml").read_text()) is not None


def rewrite(moves):
    """Return {path: original_bytes} of every file changed (for restore)."""
    changed = {}

    def edit(path, fn):
        if not path.exists():
            return
        old = path.read_text(errors="surrogateescape")
        new = fn(old)
        if new != old:
            changed.setdefault(path, old.encode(errors="surrogateescape"))
            path.write_text(new, errors="surrogateescape")

    for old, new in moves:
        o = re.escape(old)
        # yaml: [0x.., c|hasm|.rodata|.data|.bss, old] and {..., name: old, ...}; any spacing, any hex case
        edit(M / "conker.us.yaml", lambda s: re.sub(
            rf"(\[\s*0x[0-9A-Fa-f]+\s*,\s*(?:c|hasm|asm|\.rodata|\.data|\.bss)\s*,\s*){o}(\s*\])", rf"\g<1>{new}\g<2>",
            re.sub(rf"(name:\s*){o}([,}}\s])", rf"\g<1>{new}\g<2>", s)))
        # Makefile per-file flags
        edit(M / "Makefile", lambda s: re.sub(rf"(\$\(SRC_DIR\)/){o}(\.[cs]\.o)", rf"\g<1>{new}\g<2>", s))
        # subsystem map and proofs
        edit(REPO / "tools" / "subsystems.tsv", lambda s: re.sub(rf"^src/{o}(\.[cs])\t", rf"src/{new}\g<1>\t", s, flags=re.M))
        # mentions in sources and headers (comments, file lists, group headers): whole word, not part of a path
        # (a `src/` or `conker/src/` in front is the same file)
        word = re.compile(rf"(?<![\w/])((?:conker/)?(?:src/)?){o}(?![\w])")
        for g in TEXT_FILES:
            for p in M.glob(g):
                edit(p, lambda s: word.sub(rf"\g<1>{new}", s))
        edit(REPO / "tools" / "hasm_proofs.tsv", lambda s: word.sub(rf"\g<1>{new}", s))
        # the reviewed exceptions of tools/linkcheck.py name the object: build/src/<path>.c.o
        edit(REPO / "tools" / "linkcheck_allow.tsv", lambda s: re.sub(rf"^(build/src/){o}(\.[cs]\.o\t)", rf"\g<1>{new}\g<2>", s, flags=re.M))
    return changed


def git(*a, check=True):
    return subprocess.run(["git", "-C", str(REPO), *a], capture_output=True, text=True, check=check)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("moves")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("-m", "--message", default="")
    a = ap.parse_args()
    moves = load(a.moves)
    errors = validate(moves)
    if errors:
        sys.exit("INVALID:\n  " + "\n  ".join(errors))
    moves = ordered(moves)
    if a.dry_run:
        for old, new in moves:
            print(f"{source_of(old).relative_to(M)} -> src/{new}{source_of(old).suffix}")
        return
    with open(mw.LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        if git("status", "--porcelain", "--", "conker/src", "conker/include").stdout.strip():
            sys.exit("main has uncommitted changes under conker/src or conker/include: refusing")
        moved = []
        for old, new in moves:
            src = source_of(old)
            dst = SRC / f"{new}{src.suffix}"
            dst.parent.mkdir(parents=True, exist_ok=True)
            git("mv", str(src), str(dst))
            moved.append((src, dst))
        changed = rewrite(moves)
        news = {n for _, n in moves}
        missed = [o for o, _ in moves if o not in news and yaml_mentions(o)]
        ok, r = not missed, None
        if ok:  # with an old name left in the yaml, splat would write a new stub file at the old path
            r = mw.run(SPLAT, M, timeout=1800)
            (M / mw.BIN).unlink(missing_ok=True)
            r = mw.run("make -B -j16 2>&1", M, timeout=3600)
            ok = (M / mw.BIN).exists() and mw.sha1(M / mw.BIN) == mw.INNER_SHA
        if ok:
            acc = subprocess.run([sys.executable, str(REPO / "tools" / "accounting.py")], capture_output=True, text=True)
            ok = acc.returncode == 0
        if not ok:
            for path, data in changed.items():
                path.write_bytes(data)
            for src, dst in reversed(moved):
                if src.exists() and not git("ls-files", "--error-unmatch", str(src), check=False).stdout.strip():
                    src.unlink()  # a stub that splat wrote at the old path; the real file is at dst
                git("mv", str(dst), str(src))
                d = dst.parent
                while d != SRC and d.exists() and not any(d.iterdir()):
                    d.rmdir()
                    d = d.parent
            mw.run(SPLAT, M, timeout=1800)
            mw.run("make -B -j16 2>&1", M, timeout=3600)
            if missed:
                sys.exit("MOVE-FAIL: the yaml still names " + ", ".join(missed) + " (an entry in a form that the "
                         "rewrite does not know); everything is restored")
            print(r.stdout[-2500:])
            sys.exit("MOVE-FAIL: the ROM does not match after the move; everything is restored")
        for src, _ in moved:  # directories that the moves emptied
            d = src.parent
            while d != SRC and d.exists() and not any(d.iterdir()):
                d.rmdir()
                d = d.parent
        git("add", "-A", "--", "conker/src", "conker/include", "conker/conker.us.yaml", "conker/Makefile",
            "tools/subsystems.tsv", "tools/hasm_proofs.tsv", "tools/linkcheck_allow.tsv", "tools/accounting.tsv")
        msg = a.message or "move: " + ", ".join(f"{o} -> {n}" for o, n in moves[:4]) + (" ..." if len(moves) > 4 else "")
        git("commit", "-q", "-m", msg + "\n\nCo-Authored-By: " + COAUTHOR)
        print(f"MOVED {len(moves)} file(s): " + git("rev-parse", "--short", "HEAD").stdout.strip())


if __name__ == "__main__":
    main()
