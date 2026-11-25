#!/usr/bin/env python3
"""pad_to.py SRC LENGTH OUT — write OUT as exactly LENGTH bytes: the first bytes of SRC, then zeros.

The game segment has a fixed size in the ROM. Its code part and its data part are each padded to a
reserved length, so the debugger and the assets after it keep their ROM addresses. A negative LENGTH
means that a compressed part is too large for its reserved space. That is a build error.
SRC is the original padding (its first bytes are not all zero), or /dev/null for zero padding.
"""
import sys
from pathlib import Path

src, length, out = sys.argv[1], int(sys.argv[2]), sys.argv[3]
if length < 0:
    sys.exit(f"pad_to: {out}: the compressed data is {-length} bytes larger than its reserved space")
data = Path(src).read_bytes()[:length] if src != "/dev/null" else b""
Path(out).write_bytes(data + b"\0" * (length - len(data)))
