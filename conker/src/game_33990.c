#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern u8 D_800BE2F0[];
extern u8 D_800BE2F2;
extern u8 D_800BE2F3;
extern u8 D_800BE2F4[];
extern u8 D_800BE35A[];
extern s8 D_800E0BE0[0x1C];
extern u8 D_800E0BFC;
extern u8 D_800D2E69;
extern void *D_80082BBC;
extern u8 D_8008FE30;

extern void func_151DD3A0(void *, s32, void *, s32);
extern s32 func_151DCEF0(void *, s32, void *, s32);
extern s32 func_151DD4E0(void *, u8, void *);
extern void func_151DD9E4(void);
extern void func_151E7EF8(void);
extern void func_15007168(void);

typedef struct Struct33990 {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
} Struct33990; /* size = 0xA */


void func_150064E0(void) {
    s32 i = 0;
    func_15017790();

    do {
        D_800C3A60[i++] = 0;
    } while (i < 69);

    D_800BE3DF = 24;
    D_800BE3E8 = 0;
    D_800D2E45 = 1;
}

extern u8 *D_800BE3F0;
extern u32 D_800BE3E4;
extern s32 func_1502B5C8();

typedef struct LoadSlot33990 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s8 unkE;
    /* 0x0F */ s8 padF;
} LoadSlot33990;

void func_15006590(s32 arg0) {
    u64 mask;
    s32 i;
    s32 j;
    u16 sum;
    u8 *p;
    u8 *q;
    u8 byte;
    u8 bit;
    s32 cnt;
    u8 v;
    u32 t;
    LoadSlot33990 *ent;
    u8 sp7F;
    s32 sp78;
    u8 buf[27];

    for (i = 0; i < 27; i++) {
        buf[i] = ((u8 *)D_800D2E4C)[i];
    }
    if (D_800BE3F0 == NULL) {
        D_800BE3F0 = (u8 *)func_1502B5C8(0, 1, 7) + 8;
    }
    osWritebackDCache(D_800BE358, 0x80);
    osInvalDCache(D_800BE358, 0x80);
    func_151DD3A0(&D_800BE900, (u8)(arg0 * 16 + 4), D_800BE358, 0x80);
    p = D_800BE358;
    sum = 0xCC;
    i = 2;
    do {
        sum += p[i] << (i & 3);
        i++;
    } while (i < 0x80);
    p += 2;
    i = 0;
    if (sum != *(u16 *)D_800BE358) {
        func_150064E0();
        if (D_800BE3DC >= 0 && D_80082BC0 != 0) {
            for (i = 0; i < 27; i++) {
                ((u8 *)D_800D2E4C)[i] = buf[i];
            }
        }
        if (arg0 * 16 + 4 == 0x34) {
            D_80082BC0 = 0;
        }
    } else {
        sp7F = *p;
        p += 5;
        sp78 = p[-4] << 24;
        sp78 |= p[-3] << 16;
        sp78 |= p[-2] << 8;
        sp78 |= p[-1];
        do {
            ((u8 *)D_800D2E4C)[i] = *p++;
            i++;
        } while (i < 27);
        for (i = 0; i < 9; i++) {
            D_800D2E60[i] = *p++;
        }
        D_800BE3DF = *p++;
        D_800BE3E0 = *p++;
        D_800BE3E4 = *p++ << 8;
        D_800BE3E4 |= *p++;
        func_15085710(0, 9, D_800BE3E4);
        t = *p++ << 24;
        t |= *p++ << 16;
        t |= *p++ << 8;
        t |= *p++;
        D_800BE3E8 = t;
        byte = *p++;
        bit = 0x80;
        for (i = 0; i < 0x45; i++) {
            D_800C3A60[i] = 0;
            mask = (u64)1 << (D_800BE3F0[i] - 1);
            for (j = 0; j < D_800BE3F0[i]; j++) {
                if (byte & bit) {
                    D_800C3A60[i] |= mask;
                }
                if (bit == 1) {
                    byte = *p++;
                    bit = 0x80;
                } else {
                    bit >>= 1;
                }
                mask >>= 1;
            }
        }
        if (D_80082BC0 != 0 && sp7F < 3) {
            (&D_800BE3D8)[arg0] = sp7F;
            if (((LoadSlot33990 *)&D_800BE3F8)[sp7F].unk8 < sp78) {
                cnt = 0;
                for (i = 0; i < 27; i++) {
                    v = ((u8 *)D_800D2E4C)[i];
                    for (j = 8; j != 0; j--) {
                        if (v & 1) {
                            cnt++;
                        }
                        v >>= 1;
                    }
                }
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unk0 = D_800BE3E8;
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unkD = cnt;
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unkC = *(s8 *)&D_800BE3DF;
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unk4 = D_800BE3E4;
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unk8 = sp78;
                ((LoadSlot33990 *)&D_800BE3F8)[sp7F].unkE = arg0;
                if ((u8)cnt >= D_800BE3DC) {
                    D_800BE3DC = (u8)cnt;
                    if (D_800D2E68 & 0x20) {
                        D_800BE3DE = 1;
                    }
                } else {
                    for (i = 0; i < 27; i++) {
                        ((u8 *)D_800D2E4C)[i] = buf[i];
                    }
                }
            } else {
                for (i = 0; i < 27; i++) {
                    ((u8 *)D_800D2E4C)[i] = buf[i];
                }
            }
        }
        if (arg0 * 16 + 4 == 0x34) {
            D_80082BC0 = 0;
        }
    }
}
typedef struct SaveSlot33990 {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ s8 unkE;
    /* 0x0F */ s8 padF;
} SaveSlot33990; /* size = 0x10 */

extern u8 *D_800BE3F0;
extern u32 D_800BE3E4;
extern s32 func_1502B5C8(s32, s32, s32, s32);

void func_15006BEC(s32 arg0) {
    u64 mask;
    s32 i;
    s32 j;
    volatile s32 ret;
    u8 *p;
    s32 slot;
    s32 id;
    u8 bit;
    u8 byte;
    u16 sum;

    id = (((SaveSlot33990 *)&D_800BE3F8)[arg0].unk8 + 1) & 0x7FFFFFFF;
    j = -1;
    for (i = 0; i < 3; i++) {
        if (((SaveSlot33990 *)&D_800BE3F8)[i].unk8 >= 0) {
            j &= ~(1 << ((SaveSlot33990 *)&D_800BE3F8)[i].unkE);
        }
    }
    slot = 0;
    for (i = 0; i < 4; i++) {
        if ((1 << i) & j) {
            slot = i;
        }
    }
    ((SaveSlot33990 *)&D_800BE3F8)[arg0].unkE = slot;
    ((SaveSlot33990 *)&D_800BE3F8)[arg0].unk8 = id;
    if (D_8002AC5C == 0) {
        for (i = 0; i < 8; i++) { D_800BE358[i] = 0xFE; }
        osWritebackDCacheAll();
        func_151DD4E0(&D_800BE900, (slot << 4) + 0x13, D_800BE358);
    }
    if (D_800BE3F0 == NULL) {
        D_800BE3F0 = (u8 *)func_1502B5C8(0, 1, 7, slot) + 8;
    }
    p = (u8 *)(u32)D_800BE35A;
    *p++ = arg0;
    *p++ = (id >> 24) & 0xFF;
    *p++ = (id >> 16) & 0xFF;
    *p++ = (id >> 8) & 0xFF;
    *p++ = id & 0xFF;
    for (i = 0; i < 27; i++) { *p++ = ((u8 *)D_800D2E4C)[i]; }
    for (i = 0; i < 9; i++) { *p++ = D_800D2E60[i]; }
    *p++ = *(s8 *)&D_800BE3DF;
    *p++ = D_800BE3E0;
    D_800BE3E4 = func_150859AC(0, 6);
    *p++ = (D_800BE3E4 >> 8) & 0xFF;
    *p++ = (D_800BE3E4) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 24) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 16) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 8) & 0xFF;
    *p++ = (D_800BE3E8) & 0xFF;
    for (i = 0; i < 0x45; i++) { p[i] = 0; }
    byte = 0;
    bit = 0x80;
    for (i = 0; i < 0x45; i++) {
        mask = (u64)1 << (D_800BE3F0[i] - 1);
        for (j = 0; j < D_800BE3F0[i]; j++) {
            if (D_800C3A60[i] & mask) {
                byte |= bit;
            }
            if (bit == 1) {
                *p++ = byte;
                byte = 0;
                bit = 0x80;
            } else {
                bit >>= 1;
            }
            mask >>= 1;
        }
    }
    sum = 0xCC;
    for (i = 2; i < 0x80; i++) { sum += D_800BE358[i] << (i & 3); }
    *(u16 *)D_800BE358 = sum;
    osWritebackDCacheAll();
    if (D_8002AC5C == 0) {
        ret = func_151DCEF0(&D_800BE900, (u8)((slot << 4) + 4), D_800BE358, 0x80);
    }
}
void func_1500707C(s32 arg0) {
    s32 i;
    s32 ret;
    s32 idx;
    s32 idx2;
    u8 *p358 = D_800BE358;
    s8 *p3D8;
    for (i = 0; i < 8; i++) { p358[i] = 0xFF; }
    osWritebackDCacheAll();
    p3D8 = &D_800BE3D8;
    if (D_8002AC5C == 0) {
        for (i = 0; i < 4; i++) {
            if (arg0 == *p3D8) {
                idx = i * 16 + 4;
                ret = func_151DD4E0(&D_800BE900, idx, p358);
                idx2 = idx + 1;
                ret = func_151DD4E0(&D_800BE900, idx2, p358) + ret - ret;
                if (ret) {}
            }
            p3D8++;
        }
    }
}
void func_15007168(void) {
    s32 i;

    for (i = 0; i < 0x68; i++) {
        D_800BE2F0[i] = 0;
    }

    for (i = 0; i != 10; i++) {
        ((Struct33990 *)D_80082BBC)[i].unk1 = 3;
        ((Struct33990 *)D_80082BBC)[i].unk3 = 2;
        ((Struct33990 *)D_80082BBC)[i].unk4 = 1;
        ((Struct33990 *)D_80082BBC)[i].unk2 = 10;
        switch (i) {
            case 0:
                ((Struct33990 *)D_80082BBC)[i].unk5 = 3;
                break;
            case 1:
            case 2:
                ((Struct33990 *)D_80082BBC)[i].unk5 = 5;
                break;
            case 7:
                ((Struct33990 *)D_80082BBC)[i].unk5 = 20;
                break;
            case 5:
                ((Struct33990 *)D_80082BBC)[i].unk5 = 20;
                ((Struct33990 *)D_80082BBC)[i].unk2 = 0;
                break;
            case 8:
            case 9:
                ((Struct33990 *)D_80082BBC)[i].unk5 = 3;
                break;
        }
        ((Struct33990 *)D_80082BBC)[i].unk9 = 0;
    }
}
// PERMUTER CANDIDATE best 1165: loop structure byte-exact; remaining diff is a clean 3-register
// rotation {sum,i,p}->{v0,v1,a0} (target sum=v0,i=v1,p=a0) plus seed 0xCC reassociation.
// void func_1500727C(void) {
//     u16 sum;
//     s32 i;
//     u8 *p;
//     func_151DD3A0(&D_800BE900, 0x44, &D_800BE2F0, 0x70);
//     sum = (D_800BE2F2 << 2) + 0xCC + (D_800BE2F3 << 3);
//     p = D_800BE2F4;
//     i = 4;
//     do {
//         sum += *p++ << (i & 3);
//         i++;
//     } while (i != 0x68);
//     if (sum != D_800BE2F0) {
//         func_15007168();
//     }
// }
void func_1500727C(void) {
    s32 i;
    u16 sum;

    func_151DD3A0(&D_800BE900, 0x44, (s16 *)D_800BE2F0, 0x70);
    sum = 0xCC;
    i = 2;
    do {
        sum += D_800BE2F0[i] << (i & 3);
        i++;
    } while (i < 0x68);
    if (*(u16 *)D_800BE2F0 != sum) {
        func_15007168();
    }
}
// PERMUTER CANDIDATE best 1265: same unrolled-checksum 3-register rotation {sum,i,p}->{v0,v1,a0}
// as func_1500727C, plus target keeps the (dead) func_151DCEF0 return in a stack slot (frame -0x20
// vs my -0x18) which my build DCEs. Reconstruction:
// void func_15007360(void) {
//     u16 sum; s32 i; u8 *p; s32 ret;
//     sum = (D_800BE2F2 << 2) + 0xCC + (D_800BE2F3 << 3);
//     p = D_800BE2F4; i = 4;
//     do { sum += *p++ << (i & 3); i++; } while (i != 0x68);
//     D_800BE2F0 = sum;
//     if (D_8002AC5C == 0) { ret = func_151DCEF0(&D_800BE900, 0x44, &D_800BE2F0, 0x70); }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007360.s")
/* Keep each loop body on one line: under -g3 the line table changes the unrolled store order. */
void func_15007440(void) {
    u16 sum;
    s32 i;
    s8 saved;

    func_151DD3A0(&D_800BE900, 0, &D_800BE358, 0x20);
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += D_800BE358[i] << (i & 3); }
    saved = D_800E0BE0[0x10];
    if (sum != *(u16 *)D_800BE358) {
        for (i = 0; i < 0x1C; i++) { D_800E0BE0[i] = -1; }
    } else {
        for (i = 0; i < 0x1C; i++) { D_800E0BE0[i] = D_800BE358[i + 2]; }
    }
    D_800E0BE0[0x10] = saved;
    func_151DD9E4();
    func_151E7EF8();
    D_8008FE30 = 1;
}

/* The dead func_151DCEF0 result is kept on the stack via a volatile local (sw v0,0x18(sp)). */
void func_15007558(void) {
    u16 sum;
    volatile s32 ret;
    s32 i;

    for (i = 0; i < 0x1C; i++) { D_800BE358[i + 2] = D_800E0BE0[i]; }
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += D_800BE358[i] << (i & 3); }
    *(u16 *)D_800BE358 = sum;
    if (D_8002AC5C == 0) {
        ret = func_151DCEF0(&D_800BE900, 0, D_800BE358, 0x20);
    }
}

void func_15007644(void) {
}

void func_1500764C(void) {
    D_80082BB4 = (u8)6;
    D_800BE3EC = (u8)0;
}

void func_15007668(void) {
    D_80082BB4 = (u8)7;
    D_800BE3EC = (u8)0;
}

void func_15007684(void) {
    D_80082BB4 = (u8)4;
    D_800BE3EC = (u8)0;
}

void func_150076A0(void) {
    D_80082BB4 = (u8)5;
    D_800BE3EC = (u8)0;
}

void func_150076BC(s32 arg0) {
    if (arg0 < 0) {
        func_150064E0();
    }
    if ((arg0 >= 0) && (arg0 < 4) && (D_800BE616 == 0)) {
        D_80082BB4 = (u8)1;
        D_800BE3EC = arg0;
    }
}

void func_15007718(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3) && (D_800BE616 == 0)) {
        D_80082BB4 = (u8)2;
        D_800BE3EC = arg0;
    }
}

void func_15007750(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3)) {
        D_80082BB4 = (u8)3;
        D_800BE3EC = arg0;
    }
}

// PERMUTER CANDIDATE best 3600: struct stores + loop2 (D_800D2E60 clear) match; blocker is IDO
// unrolling the 27-iter D_800D2E4C byte-clear (target leaves it rolled, reloading the pointer each
// iter) + the final four -1 stores materialize fresh regs in target. Reconstruction:
// void func_15007778(void) {
//     u8 *q; s32 i;
//     ((struct162 *)&D_800BE3F8)->unk8 = -1;
//     ((struct162 *)&D_800BE3F8)->unk18 = -1;
//     ((struct162 *)&D_800BE3F8)->unk28 = -1;
//     ((struct162 *)&D_800BE3F8)->unkE = -1;
//     ((struct162 *)&D_800BE3F8)->unk1E = -1;
//     ((struct162 *)&D_800BE3F8)->unk2E = -1;
//     D_800BE3DC = -1; D_800BE3DE = 0; D_80082BC0 = 1;
//     i = 0; do { ((u8 *)D_800D2E4C)[i] = 0; i++; } while (i < 27);
//     q = D_800D2E60; do { *q++ = 0; } while (q < &D_800D2E69);
//     D_800BE3DB = -1; D_800BE3DA = -1; D_800BE3D9 = -1; D_800BE3D8 = -1;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007778.s")
