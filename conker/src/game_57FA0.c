#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#include "macros.h"
#include "libc/stdarg.h"

extern void *allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_10004074(void *arg0);
extern s32 func_10006240(void *arg0, void *arg1, u32 arg2);

void func_1502AAF0(void) {
}

void func_1502AAF8(s32 arg0) {
}

/* One 8-byte entry of the ROM asset table at D_AB1950. */
typedef struct {
    s32 offset; /* added to the table base to reach the next level */
    s32 info;   /* bits 0-27: size (0 = missing), bits 28-30: compression (1 = compressed) */
} RomTableEntry;

/* Most-recently-used cache of ROM table entries; the newest is at the end. */
typedef struct {
    u32 romAddr;  /* address of the entry in ROM, with bit 31 set */
    s32 lastUsed; /* value of D_800C3D60 when the entry was last read */
    RomTableEntry entry;
} RomTableCacheLine;

extern RomTableCacheLine D_800C3D68[16];

/* Drops the oldest `count` cache lines and appends `count` entries read from `romAddr` onwards. */
void func_1502AB04(s32 count, RomTableEntry *entries, s32 tick, u32 romAddr) {
    u32 n;
    u32 i;

    n = count; // matching: IDO keeps the count in a callee-saved register only through a local copy
    if (n != 0) {
        bcopy(&D_800C3D68[n], D_800C3D68, (16 - n) * sizeof(RomTableCacheLine));
    }

    for (i = 16 - n; i < 16; i++) {
        D_800C3D68[i].entry = *entries++;
        D_800C3D68[i].lastUsed = tick;
        D_800C3D68[i].romAddr = romAddr;
        romAddr += sizeof(RomTableEntry);
    }
}

extern u32 D_800C3D60;

/*
 * Looks up entry `index` of the ROM table at `table` and returns its offset, storing its info word in
 * *info. Hits move to the newest cache line; misses DMA the entry and the one after it into the cache.
 */
s32 func_1502AC88(u8 *table, s32 index, s32 *info) {
    u8 buf[0x38];
    u32 dmaAddr;
    u32 dmaSize;
    RomTableEntry *dmaBuf;
    s32 offset;
    RomTableEntry *entries;
    u32 i;
    u32 j;
    RomTableCacheLine line;

    table += index * sizeof(RomTableEntry);
    table = (u8 *)((u32)table | 0x80000000);

    for (i = 0; i < 16; i++) {
        if ((u32)table == D_800C3D68[i].romAddr) {
            line = D_800C3D68[i];
            for (j = i; j < 15; j++) {
                D_800C3D68[j] = D_800C3D68[j + 1];
            }
            D_800C3D68[15] = line;
            D_800C3D68[15].lastUsed = D_800C3D60;
            *info = D_800C3D68[15].entry.info;
            return D_800C3D68[15].entry.offset;
        }
    }

    D_800C3D60++;
    dmaAddr = (u32)table & 0x7FFFFFF0;
    dmaSize = (((u32)table & 0xE) + 0x1F) & ~0xF;
    dmaBuf = (RomTableEntry *)((u32)buf & ~0xF);
    func_10004514(dmaAddr, dmaBuf, dmaSize, 1);
    entries = (RomTableEntry *)((u8 *)dmaBuf + ((u32)table & 0xF));
    offset = entries[0].offset;
    *info = entries[0].info;
    func_1502AB04(2, entries, D_800C3D60, (u32)table);
    return offset;
}

s32 func_1502AF04(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    typedef struct { u32 address; u32 flags; } Relocation;
    u32 i;
    s32 source;
    u8 *aligned;
    Relocation *entries;

    arg2 <<= 3;
    source = (s32)arg0 + arg2;
    aligned = (u8 *)((arg1 + 8) & ~0xF);
    func_10004514(source & ~0xF, aligned, ((source & 0xE) + (arg3 << 3) + 0xF) & ~0xF, 1);
    entries = (Relocation *)((s32)aligned + (source & 0xFU));
    for (i = 0; i < (u32)arg3; i++) {
        entries[i].address += (u32)arg0;
    }
    return (s32)aligned + (source & 0xFU);
}
extern u8 D_AB1950;
s32 func_1502AC88(u8 *arg0, s32 arg1, s32 *arg2);
s32 func_1502AF04(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_1502B020(s32 *arg0, s32 arg1, ...) {
    va_list args;
    u8 *offset;
    s32 n;
    s32 sp38;

    sp38 = 1;
    offset = &D_AB1950;
    va_start(args, arg1);

    for (; arg1 != 0; arg1--) {
        n = va_arg(args, s32);
        if (sp38 != 0) {
            offset += func_1502AC88(offset, n, &sp38);
        }
        sp38 = sp38 & 0xFFFFFFF;
    }

    if (arg0 != 0) {
        *arg0 = sp38 & 0xFFFFFFF;
    }

    if (sp38 == 0) {
        return 0;
    } else {
        return (s32)offset;
    }
}
s32 func_1502B110(u8 *offset, s32 arg1, s32 arg2, u32 arg3) {
    u8 *unused;
    s32 *tmp;
    s32 more;
    s32 ret;
    s32 n;

    unused = offset;
    more = 1;
    ret = 0;
    if (offset != 0) {
    } else {
        offset = &D_AB1950;
    }

    tmp = (s32 *)(&arg3 + 1);
    for (; arg3 >= 2U; arg3--) {
        tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
        n = *(tmp - 1);
        if (more != 0) {
            offset += func_1502AC88(offset, n, &more);
        }
        more = more & 0xFFFFFFF;
    }

    tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
    n = *(tmp - 1);
    if (more != 0) {
        ret = func_1502AF04(offset, arg2, n, arg1);
    }

    return ret;
}
s32 func_1502B224(s32 arg0, void *arg1, u32 arg2, u32 arg3) {
    u32 size;
    void *buf;
    s32 expected;

    size = ((arg2 & 0x0FFFFFFF) + 1) & ~1;
    if (arg3 != 0) {
        if (arg3 < size) {
            size = arg3;
        }
    }

    if ((arg2 & 0x70000000) == 0x10000000) {
        buf = allocate_memory(size, 1, 2, 2);
        if (buf == 0) {
            return 0;
        }

        func_10004514(arg0, buf, (size + 0xF) & ~0xF, 1);
        expected = *(s32 *)buf & 0x7FFFFFFF;
        if ((size = func_10006240(buf, arg1, D_8003809C)) == expected) {
        } else {
            D_8003C8E0 = 0x0C000036;
            func_150AD770();
        }
        func_10004074(buf);
        return size;
    }

    func_10004514(arg0, arg1, (size + 0xF) & ~0xF, 1);
    return size;
}
s32 func_1502B350(u8 *arg0, s32 arg1, s32 *arg2) {
    u32 size;
    void *buf;
    void *out;

    size = ((arg1 & 0x0FFFFFFF) + 1) & ~1;
    out = buf = allocate_memory(size, 1, 2, 2);
    if (buf == NULL) {
        return 0;
    }
    func_10004514((s32)arg0, buf, (size + 0xF) & ~0xF, 1);
    if ((arg1 & 0x70000000) == 0x10000000) {
        *arg2 = *(s32 *)buf & 0x7FFFFFFF;
        if (*arg2 != 0) {
            out = NULL;
            if ((u32)*arg2 < 1000000) {
                out = allocate_memory((u32)*arg2, 1, 2, 2);
                if (out != NULL) {
                    size = func_10006240(buf, out, D_8003809C);
                } else {
                    size = 0;
                }
            } else {
                size = 0;
            }
        } else {
            out = NULL;
            size = 0;
        }
        func_10004074(buf);
    }
    *arg2 = size;
    return (s32)out;
}
/* Offset-table entry: unk0 is rebased onto the table (-1 = none); bit 31 of unk4 ends the table. */
typedef struct {
    s32 unk0;
    s32 unk4;
} Pair1502B4A8;

s32 func_1502B4A8(Pair1502B4A8 *arg0, s32 arg1) {
    s32 i;

    if (arg1 == 0) {
        while ((arg0[arg1++].unk4 & 0x80000000) == 0) {
        }
    }
    for (i = 0; i < arg1; i++) {
        arg0[i].unk4 &= 0x0FFFFFFF;
        if (arg0[i].unk0 == -1 || arg0[i].unk4 == 0) {
            arg0[i].unk0 = 0;
        } else {
            arg0[i].unk0 += (s32)arg0;
        }
    }
    return arg1;
}
extern u8 D_AB1950;
s32 func_1502AC88(u8 *arg0, s32 arg1, s32 *arg2);
s32 func_1502B350(u8 *arg0, s32 arg1, s32 *arg2);

s32 func_1502B5C8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *more;
    u8 *offset;
    s32 *tmp;
    s32 sp40;
    s32 ret;
    s32 sp38;
    s32 n;

    more = &sp40;
    if (arg0 != 0) {
        more = arg0;
    }

    *more = 1;
    offset = &D_AB1950;
    tmp = &arg1 + 1;

    if (arg1 != 0) {
        do {
            tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
            n = *(tmp - 1);
            if (*more != 0) {
                offset += func_1502AC88(offset, n, &sp38);
            }
            *more = sp38 & 0xFFFFFFF;
        } while (--arg1 != 0);
    }

    if (*more != 0) {
        ret = func_1502B350(offset, sp38, more);
    } else {
        ret = 0;
    }

    return ret;
}
s32 func_1502B4A8(Pair1502B4A8 *arg0, s32 arg1);

s32 func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3) {
    s32 *more;
    u8 *offset;
    s32 *tmp;
    s32 sp44;
    s32 ret;
    s32 sp38;
    s32 n;

    more = &sp44 + 1;
    if (arg0 != 0) {
        more = arg0;
    }

    *more = 1;
    offset = &D_AB1950;
    tmp = &arg3 + 1;

    if (arg3 != 0) {
        do {
            tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
            n = *(tmp - 1);
            if (*more != 0) {
                offset += func_1502AC88(offset, n, &sp38);
            }
            *more = sp38 & 0xFFFFFFF;
        } while (--arg3 != 0);
    }

    if (*more != 0) {
        ret = func_1502B350(offset, sp38, more);
        if (*more != 0 && ret != 0) {
            arg1 = func_1502B4A8(ret, arg1);
        } else {
            arg1 = 0;
        }
        if (arg2 != 0) {
            *arg2 = arg1;
        }
    } else {
        ret = 0;
    }

    return ret;
}
s32 func_1502B7F0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 *offset;
    s32 *tmp;
    s32 n;
    s32 sp38;
    s32 sp34;

    sp38 = 1;
    offset = &D_AB1950;
    tmp = &arg1 + 1;

    for (; arg1 != 0; arg1--) {
        tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
        n = *(tmp - 1);
        if (sp38 != 0) {
            offset += func_1502AC88(offset, n, &sp34);
        }
        sp38 = sp34 & 0xFFFFFFF;
    }

    if (sp38 != 0) {
        *arg0 = func_1502B350(offset, sp34, &sp38);
    } else {
        *arg0 = 0;
    }

    return sp38;
}

s32 func_1502B8E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *offset;
    s32 *tmp;
    s32 n;
    s32 more;
    s32 sp34;

    more = 1;
    offset = &D_AB1950;
    tmp = &arg2 + 1;

    for (; arg2 != 0; arg2--) {
        tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
        n = *(tmp - 1);
        if (more != 0) {
            offset += func_1502AC88(offset, n, &sp34);
        }
        more = sp34 & 0xFFFFFFF;
    }

    if (more != 0) {
        more = func_1502B224(offset, arg0, sp34, arg1);
    }

    return more;
}

s32 func_1502B9B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *offset;
    s32 *tmp;
    s32 n;
    s32 more;
    s32 stack0;
    s32 pad[3];
    s32 stack2[2];
    s32 stack1[2];
    s32 pad0;

    more = 1;
    offset = &D_AB1950;
    tmp = &arg0 + 1;

    for (; arg0 != 0; arg0--) {
        tmp = (s32 *)(((s32)tmp + 3) & -4) + 1;
        n = *(tmp - 1);
        if (more != 0) {
            offset += func_1502AC88(offset, n, &stack0);
        }
        more = stack0 & 0xFFFFFFF;
    }

    if (more != 0) {
        more = ((stack0 & 0xFFFFFFF) + 1) & -2;
        if ((stack0 & 0x70000000) == 0x10000000) {
            tmp = stack1;
            if (((s32)stack1 & 8) != 0) {
                tmp = stack2;
            }
            func_10004514((s32)offset, tmp, 0x10, 1);
            more = tmp[0];
        }
    }

    return more;
}
// NON-MATCHING: maybe 50% there?
// s32 func_1502B9B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//
//     s32 stack2[2];
//     s32 stack1[2];
//     s32 stack0[5];
//
//     s32 more;
//     s32 offset;
//     s32 *tmp;
//     s32 i;
//
//     more = 1;
//     offset = &D_00AB1950;
//     tmp = &arg1;
//
//     for (i = arg0; i != 0; i--) {
//         tmp = ALIGN4(tmp) + 4;
//         if (more != 0) {
//             offset += func_1502AC88(offset, tmp - 4, &stack0);
//         }
//         more = *stack0 & 0xFFFFFFF;
//     }
//
//     if (more != 0) {
//         more = ALIGN2(stack0[0] & 0xFFFFFFF);
//         if ((*stack0 & 0x70000000) == 0x10000000) {
//             if (((s32) &stack1 & 8) != 0) {
//                 *stack1 = &stack2;
//             }
//             func_10004514(offset, stack1, 0x10, 1); // decompress?
//             more = *stack1;
//         }
//     }
//
//     return more;
// }
