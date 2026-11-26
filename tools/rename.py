#!/usr/bin/env python3
"""rename.py — central rename of exported symbols (func_XXXXXXXX / D_XXXXXXXX / any named symbol).

    rename.py FILE.tsv [--dry-run] [-m MESSAGE]

FILE.tsv has one `old<TAB>new` pair per line (# comments allowed). The whole batch is applied at once:
  1. validate: old names exist; new names are valid identifiers, unique, and unused anywhere in the tree;
  2. under the land lock: rewrite symbol_addrs.us.txt (the splat name source), replace every whole-word
     occurrence in conker/src (.c/.h/.s), conker/include, and the linker symbol files;
  3. re-run splat (regenerates asm/ with the new names), restore the auto symbol files, rebuild with make -B;
  4. commit only if the whole-ROM sha1 still matches; otherwise restore everything exactly.
"""
import argparse, fcntl, re, shutil, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import mw  # noqa: E402

M, REPO = mw.MAIN, mw.REPO
SYMS = M / "symbol_addrs.us.txt"
ID = re.compile(r"^[A-Za-z_]\w*$")
ALLOW_USED = set()
TEXT_GLOBS = ["src/**/*.c", "src/**/*.h", "src/**/*.s", "include/**/*.h",
              "undefined_syms.us.txt", "undefined_funcs.us.txt", "jtbl_labels.us.txt",
              "undefined_syms_auto.txt", "undefined_funcs_auto.txt"]
AUTO = ("undefined_funcs_auto.txt", "undefined_syms_auto.txt")


def load_pairs(path):
    pairs = []
    for line in Path(path).read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        old, new = line.split()[:2]
        pairs.append((old, new))
    return pairs


def symbol_table():
    t = {}
    for line in SYMS.read_text().splitlines():
        m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*0x([0-9A-Fa-f]+)", line)
        if m:
            t[m.group(1)] = int(m.group(2), 16)
    return t


def address_of(name, table):
    m = re.fullmatch(r"(?:func|D)_([0-9A-Fa-f]{1,8})", name)
    if m:
        return int(m.group(1), 16)
    return table.get(name)


def files():
    out = []
    for g in TEXT_GLOBS:
        out += [p for p in M.glob(g) if p.is_file()]
    return sorted(set(out))


def word_re(names):
    return re.compile(r"\b(" + "|".join(re.escape(n) for n in sorted(names, key=len, reverse=True)) + r")\b")


def validate(pairs):
    errs, table = [], symbol_table()
    olds = [o for o, _ in pairs]
    news = [n for _, n in pairs]
    if len(set(olds)) != len(olds):
        errs.append("duplicate old names in the batch")
    if len(set(news)) != len(news):
        dup = sorted({n for n in news if news.count(n) > 1})
        errs.append(f"duplicate new names: {dup}")
    for o, n in pairs:
        if not ID.match(n):
            errs.append(f"{n}: not a valid identifier")
        if address_of(o, table) is None:
            errs.append(f"{o}: no address (not func_/D_<hex> and not in symbol_addrs)")
    body = "\n".join(p.read_text(errors="replace") for p in files())
    used_new = set(word_re(news).findall(body)) if news else set()
    for n in sorted(used_new - ALLOW_USED):
        errs.append(f"{n}: already used somewhere in the tree (if only as a struct field or in comments, pass --allow-used {n})")
    seen_old = set(word_re(olds).findall(body)) if olds else set()
    missing = [o for o in olds if o not in seen_old]
    return errs, missing, table


def apply(pairs, table):
    mapping = dict(pairs)
    rx = word_re(mapping)
    changed = []
    for p in files():
        s = p.read_text(errors="replace")
        t = rx.sub(lambda m: mapping[m.group(1)], s)
        if t != s:
            p.write_text(t)
            changed.append(p)
    # symbol_addrs: rename existing definitions in place (an appended duplicate at the same address makes
    # splat refuse the whole file), and append the names that are not listed yet
    s = SYMS.read_text()
    have = symbol_table()
    renamed = re.compile(r"^(\s*)(" + "|".join(re.escape(o) for o in mapping if o in have) + r")(\s*=)", re.M) \
        if any(o in have for o in mapping) else None
    if renamed:
        s = renamed.sub(lambda m: m.group(1) + mapping[m.group(2)] + m.group(3), s)
        SYMS.write_text(s)
        changed.append(SYMS)
    lines = []
    for o, n in pairs:
        if n in have or o in have:
            continue
        a = address_of(o, table)
        kind = " // type:func" if o.startswith("func_") else ""
        lines.append(f"{n} = 0x{a:08X};{kind}")
    if lines:
        SYMS.write_text(s.rstrip("\n") + "\n" + "\n".join(lines) + "\n")
        changed.append(SYMS)
    return changed


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pairs")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("-m", "--message", default="")
    ap.add_argument("--allow-used", action="append", default=[],
                    help="accept a new name that already occurs (struct fields and comments are separate from globals; "
                         "a local that would capture the global changes the bytes, which the sha1 gate rejects)")
    a = ap.parse_args()
    ALLOW_USED.update(a.allow_used)
    pairs = load_pairs(a.pairs)
    with open(mw.LOCK, "a") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        errs, missing, table = validate(pairs)
        if missing:
            print(f"note: {len(missing)} old names do not occur in the tree (symbol_addrs entry only): {missing[:10]}")
        if errs:
            print("\n".join(errs))
            sys.exit("rename refused")
        if a.dry_run:
            print(f"ok: {len(pairs)} renames validate")
            return
        dirty = mw.git("status", "--porcelain", "--", "conker/src", "conker/include", "conker/symbol_addrs.us.txt").strip()
        if dirty:
            sys.exit(f"main has uncommitted edits: {dirty}")
        tmp, orig = Path(tempfile.mkdtemp()), Path(tempfile.mkdtemp())
        for f in AUTO:
            shutil.copy(M / f, orig / f)
        changed = apply(pairs, table)
        # the auto files are restored after splat (it drops entries); keep the renamed copies
        for f in AUTO:
            shutil.copy(M / f, tmp / f)
        sp = mw.run("python3 ../tools/n64splat/split.py conker.us.yaml 2>&1", M, timeout=3600)
        splat_error = [l for l in sp.stdout.splitlines() if "error" in l.lower() or "Duplicate symbol" in l]
        for f in AUTO:
            shutil.copy(tmp / f, M / f)
        (M / mw.BIN).unlink(missing_ok=True)
        r = mw.run("make -B -j16 2>&1", M, timeout=3600) if not splat_error else sp
        ok = not splat_error and (M / mw.BIN).exists() and mw.sha1(M / mw.BIN) == mw.INNER_SHA
        if not ok:
            errs = [l for l in r.stdout.splitlines() if "rror" in l or "undefined reference" in l][:20]
            mw.git("checkout", "--", "conker/src", "conker/include", "conker/symbol_addrs.us.txt",
                   "conker/undefined_syms.us.txt", "conker/undefined_funcs.us.txt", "conker/jtbl_labels.us.txt")
            for f in AUTO:  # untracked: put the pre-rename copies back
                shutil.copy(orig / f, M / f)
            mw.run("python3 ../tools/n64splat/split.py conker.us.yaml >/dev/null 2>&1", M, timeout=3600)
            mw.run("make -B -j16 2>&1", M, timeout=3600)
            print("\n".join(errs) or r.stdout[-2000:])
            sys.exit("RENAME-FAIL: ROM mismatch or build error; everything restored")
        rels = [str(p.relative_to(REPO)) for p in changed if p.name not in AUTO]  # the auto files are gitignored
        r = subprocess.run([sys.executable, str(REPO / "tools" / "accounting.py")], capture_output=True, text=True)
        if r.returncode:
            print(r.stdout[-1500:])
            sys.exit("RENAME-FAIL: tools/accounting.py fails after the rename (tree left as is: inspect)")
        mw.git("add", *rels, "tools/accounting.tsv")
        msg = f"rename: {len(pairs)} symbols" + (f" ({a.message})" if a.message else "")
        body = "\n".join(f"{o} -> {n}" for o, n in pairs)
        mw.git("commit", "-q", "-m", msg + "\n\n" + body + "\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>")
        print(f"RENAMED {len(pairs)} — {mw.git('rev-parse', '--short', 'HEAD').strip()}")


if __name__ == "__main__":
    main()
