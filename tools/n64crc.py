#!/usr/bin/env python3
"""n64crc.py ROM [--check] — compute (or check) the two header checksums of an N64 ROM (CIC-6105).

The boot code checks the words at 0x10 and 0x14 against a checksum of the first 1 MiB of the program.
A ROM built from changed code needs new checksums to boot on hardware. Conker uses CIC-NUS-6105.
Without --check the file is updated in place. With --check it only reports and returns 1 on a mismatch.
"""
import struct
import sys
from pathlib import Path

M = 0xFFFFFFFF
SEED_6105 = 0xDF26F436


def word(b, o):
    return struct.unpack_from(">I", b, o)[0]


def checksums(rom):
    t1 = t2 = t3 = t4 = t5 = t6 = SEED_6105
    for i in range(0x1000, 0x101000, 4):
        d = word(rom, i)
        if (t6 + d) & M < t6:
            t4 = (t4 + 1) & M
        t6 = (t6 + d) & M
        t3 ^= d
        r = ((d << (d & 0x1F)) | (d >> (32 - (d & 0x1F)))) & M if d & 0x1F else d
        t5 = (t5 + r) & M
        t2 = t2 ^ r if t2 > d else t2 ^ t6 ^ d
        t1 = (t1 + (word(rom, 0x0750 + (i & 0xFF)) ^ d)) & M  # CIC-6105 mixes in the boot code
    return t6 ^ t4 ^ t3, t5 ^ t2 ^ t1


def main():
    path = Path(sys.argv[1])
    rom = bytearray(path.read_bytes())
    c1, c2 = checksums(rom)
    have = word(rom, 0x10), word(rom, 0x14)
    if "--check" in sys.argv:
        ok = have == (c1, c2)
        print(f"{path}: header {have[0]:08X} {have[1]:08X}, computed {c1:08X} {c2:08X}: {'OK' if ok else 'MISMATCH'}")
        sys.exit(0 if ok else 1)
    struct.pack_into(">II", rom, 0x10, c1, c2)
    path.write_bytes(rom)
    print(f"{path}: checksums {c1:08X} {c2:08X}")


if __name__ == "__main__":
    main()
