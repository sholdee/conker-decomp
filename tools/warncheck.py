#!/usr/bin/env python3
"""Fail if the build has more IDO warnings than tools/warnings_max.txt allows (a ratchet).

    warncheck.py BUILD_LOG LIMIT_FILE

BUILD_LOG is the output of a clean `make -C conker` (CI writes it with tee). LIMIT_FILE holds one
number, the most warnings that the build may have. CI reads the limit from the trusted base branch,
so a pull request can lower the limit but not raise it.

Most remaining warnings are intended. They show the original source's form, for example handler
tables whose entries take their own object types (see the comment at ObjType in include/structs.h),
K&R definitions with a prototype (642) and the short stack views of tools/PERFILE.md.
"""
import collections
import re
import sys
from pathlib import Path

WARNING = re.compile(r"cfe: Warning (\d+):")


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    log, limit_file = Path(sys.argv[1]), Path(sys.argv[2])
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
