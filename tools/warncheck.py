#!/usr/bin/env python3
"""Fail if the build has more IDO warnings than tools/warnings_max.txt allows (a ratchet), or if a
source file includes one of the removed umbrella headers.

    warncheck.py BUILD_LOG LIMIT_FILE [CODE_DIR]

BUILD_LOG is the output of a clean `make -C conker` (CI writes it with tee). LIMIT_FILE holds one
number, the most warnings that the build may have. CI reads the limit from the trusted base branch,
so a pull request can lower the limit but not raise it.

CODE_DIR is the code tree (conker/). With it, every .c and .h file under its src/ and include/ must
include the headers that it uses: functions.h, variables.h and subsystems.h, which included every
subsystem header, are gone (tools/HEADERS.md).

Most remaining warnings are intended. They show the original source's form, for example handler
tables whose entries take their own object types (see the comment at ObjType in include/structs.h),
K&R definitions with a prototype (642) and the short stack views of tools/PERFILE.md.
"""
import collections
import re
import sys
from pathlib import Path

WARNING = re.compile(r"cfe: Warning (\d+):")
UMBRELLA = re.compile(r'^\s*#\s*include\s*[<"](?:[^>"]*/)?(functions|variables|subsystems)\.h[>"]', re.M)


def umbrella_includes(code_dir):
    """(file, header) for each include of an umbrella header under code_dir/src and code_dir/include."""
    hits = []
    for top in ("src", "include"):
        for path in sorted((code_dir / top).rglob("*")):
            if path.suffix not in (".c", ".h") or not path.is_file():
                continue
            # join continued lines first, as the preprocessor does (backslash-newline and its ??/ trigraph)
            text = re.sub(r"(\\|\?\?/)\r?\n", "", path.read_text(errors="replace"))
            hits += [(path.relative_to(code_dir), m.group(1) + ".h") for m in UMBRELLA.finditer(text)]
    return hits


def main():
    if len(sys.argv) not in (3, 4):
        sys.exit(__doc__)
    log, limit_file = Path(sys.argv[1]), Path(sys.argv[2])
    if len(sys.argv) == 4:
        hits = umbrella_includes(Path(sys.argv[3]))
        for path, header in hits:
            print(f"{path}: includes {header}")
        if hits:
            sys.exit(f"FAIL: {len(hits)} includes of removed umbrella headers. Include the subsystem headers "
                     "that the file uses instead (tools/HEADERS.md).")
    codes = collections.Counter(m.group(1) for line in log.read_text(errors="replace").splitlines()
                                if (m := WARNING.search(line)))
    count = sum(codes.values())
    limit = int(next(l for l in limit_file.read_text().splitlines() if l.strip() and not l.startswith("#")))
    detail = ", ".join(f"{n} x {c}" for c, n in codes.most_common())
    print(f"IDO warnings: {count} (limit {limit}){': ' + detail if detail else ''}")
    if count > limit:
        sys.exit(f"FAIL: {count - limit} more warnings than the limit. Fix the types (tools/CASTS.md); "
                 "do not add casts to hide them.")
    if count < limit:
        print(f"The build has {limit - count} fewer warnings than the limit: lower tools/warnings_max.txt "
              f"to {count} in this change.")


if __name__ == "__main__":
    main()
