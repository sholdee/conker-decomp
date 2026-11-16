#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct Struct151E5034 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u16 unk20;
    char pad22[8];
    s8 unk2A;
    s8 unk2B;
    s8 unk2C;
    char pad2D[0x11];
    s8 unk3E;
    s8 unk3F;
    char pad40;
    s8 unk41;
    char pad42;
    s8 unk43;
    s8 unk44;
};

struct Struct151E4314 {
    s16 unk0;
    s8 unk2;
    s8 unk3;
};

extern s8 D_800AB692[];
extern struct Struct151E4314 D_800AB7A4[];
extern s8 D_800E0C00[];
extern s8 D_800E0BEB;
extern s16 D_800E0A80;
extern s16 D_8008FDCC;
extern u8 **D_800E0BD8;
extern void func_1501D348(s32, s32, s32, s32, s32);
void func_151E2834(void);
s32 func_1517EFDC(void);
extern s32 D_800E0A90;
extern u8 D_8008FE28;
extern s8 D_800E0B96;
extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FDA4;
extern s16 D_800E0B9A;
extern u8 D_800E0B95;
extern u8 D_800E0BDC;
extern u16 D_800BE930;
extern s8 D_800AB690[];
extern s32 D_800AB754[];
extern char D_800AB850[];
extern void (*D_800E0A88)(void);
extern u8 D_800E0A95;
extern s8 D_800E0A96;
extern s8 D_8008FDE0;
extern void (*D_8008FDEC[])(void);
void func_1000E934(void);
void func_1000F1A8(void);
void func_1501C730(s32, s32, s32, s32, s32);
void func_15042D94(s32, s32, s32, char *, ...);
void func_151E2404(void);
void func_151E3344(s32, s32, s32, s32, s32);
void func_151E6964(s32);
void func_151E530C(void);
void func_151E55A8(void);
void func_151E557C(void);
void func_151E7EF8(void);
s32 func_151F2CDC(void);
void func_151F2D6C(s32, s32);
s32 func_1509CA30(s32);
s32 func_1509CA50(s32);
void func_1509CC94(s32);
void func_1509CDDC(s32);
void func_1509CF28(s32, s32 *);
void func_1509D054(void);
void func_1509D08C(s32, s32, s32 *, s32 *);
extern s8 D_8008FD70;
extern s16 D_8008FDAC;
extern u8 D_8008FDB4;
extern s8 D_8008FF00;
extern u8 D_8008FF04;
extern s32 D_800BE3E4;
extern s8 D_800D23A8;
extern s8 D_800E0B99;
extern s8 D_800E0BB1;
extern u8 D_800E0BD1;

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151DD970.s")

extern s8 D_800E0BE0[];
extern s8 D_8008FE54[];
extern s8 D_8008FE6C[];
extern s8 D_8008FE84[];

void func_151DD9E4(void) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if ((D_800E0BE0[i] < D_8008FE6C[i]) || (D_8008FE84[i] < D_800E0BE0[i])) {
            D_800E0BE0[i] = D_8008FE54[i];
        }
    }
}

s32 func_151DDB94(s32 arg0) {
    return ~arg0;
}

void func_151DDBA0(void) {
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
    D_800E0B94 = 3;
    D_8008FDA4 = 0;
    D_800BEAC1 = 0;
    func_151E557C();
    func_1000F1A8();
    func_1000E934();
    D_8008FD8C = 1;
    D_8008FD90 = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151DDC20.s")

void func_151DE6D4(void) {
    s32 timer;

    func_151E530C();
    D_800CC2D0[0].unk25C |= 0x200;
    D_800CC2D0[0].y_position = 1000.0f;

    if (D_80000300 == 0) {
        D_800E0A90 = 0;
    }

    timer = D_800E0A90;
    if (timer >= 0x14B) {
        if (D_800E0B9A & 0x8000) {
            if (timer >= 0x1E0) {
            } else {
                timer = D_800E0A90 = 0x1E0;
            }
            if (timer < 0x259) {
            } else {
                if (timer >= 0x2D0) {
                } else {
                    timer = D_800E0A90 = 0x2D0;
                }
            }
        }
    }

    if (timer >= 0x349) {
        D_8008FE28 = 2;
        D_800E0B94 = 1;
        D_800D2E40 = 0;
        func_1501C730(6, 0x21, 0, 0, 1);
        *(u8 *)&D_800E0B96 = 0xFF;
    }
}

extern s32 D_800E0A90;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern u8 D_800E0A8C;
extern u8 D_8008FE28;
void func_151DE85C(void);

void func_151DE7D4(void) {
    D_800E0A90 = 0;
    D_800E0B97 = 0;
    D_800E0B98 = 0;
    D_800E0A8C = 0;
    D_8008FE28 = 2;
    func_151DE85C();
}

extern s8 D_8008FE30;
extern s8 D_8008FD74;
extern s8 D_800E0B96;

void func_151DE81C(void) {
    D_8008FD74 = 4;
    D_800E0B96 = 0;
    if (D_8008FE30 == 0) {
        func_1500764C();
    }
}

extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FD80;
extern u8 D_8008FDA4;
extern struct Struct151E5034 *D_8008FDD4;
void func_1501C730(s32, s32, s32, s32, s32);

void func_151DE85C(void) {
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
    D_800E0B94 = 3;
    D_8008FD80 = 1;
    D_8008FE28 = 2;
    D_8008FDA4 = 0;
    D_8008FDD4->unk3E = 0;
    D_8008FDD4->unk2B = 5;
    D_8008FDD4->unk2C = D_8008FDD4->unk2B;
}

void func_151DE8E8(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151DE8F0.s")

extern u8 D_800E0A94;
extern s8 D_800E0BE9;
extern s8 D_8008FEF8;
extern u8 D_8008FE34;
extern f32 D_8008FDE8;
extern u8 D_800E0A97;
extern char D_800AB800[];
void func_1001263C(s32 arg0, s32 arg1, s32 arg2);
struct216 *func_1507BB28(s32, s32);
void func_1000DE1C(s32, s32);
void func_151E7E9C(void);

void func_151DF1BC(void) {
    u16 unused;
    s32 val;
    s32 id;
    struct127 *obj;

    obj = func_15083E90(9);
    if (D_800E0A96 != -1) {
        val = D_800E0A95 - (D_800BE9E4 << 5);
        if (val <= 0) {
            val = 0;
            D_8008FEF8 = D_8008FEF8 == 0;
            if (D_8008FEF8 == 0) {
                id = 0xE;
                func_1001263C((func_150ADA20() & 1) + 0xC8, 0x7D00, 0x40);
            } else {
                id = 0xF;
                func_1001263C((func_150ADA20() % 3U) + 0xBE, 0x7D00, 0x40);
            }
            if (obj != NULL) {
                obj->unk232 = id;
                D_800D154C = obj;
                D_800C3E78 = D_800D154C - D_800CC2D0;
                obj->unk218 = func_1507BB28(0, obj->unk232);
                obj->unk21C = 0;
                obj->pad201[0] = 0x14;
            }
            D_800E0A96 = -1;
        }
        D_8008FDD4->unk20 &= 0xFFCF;
    } else {
        val = D_800E0A95 + (D_800BE9E4 << 5);
        if (val >= 0x100) {
            val = 0xFF;
        }
    }
    D_800E0A95 = val;
    val = (D_800E0A94 * val) >> 8;
    if (val >= 0xFE) {
        val = 0xFF;
    }
    if (D_8008FEF8 == 0) {
        func_151E3344(0x200200, 0, 1, val & 0xFF, 1);
        if ((D_8008FE34 == 9) && (D_8008FDD4->unk20 & 0xC)) {
            func_151E7E9C();
            func_1000DE1C(D_800E0A97, 0);
            D_800E0A97 = func_1000EA94(D_800E0BE9);
        }
        {
        s32 flags;
        flags = D_8008FDD4->unk20;
        if ((flags & 0x10) && (D_8008FE34 == 0x15) && (D_8008FDE8 == 0.0f)) {
            D_800E0A96 = 1;
            D_8008FDD4->unk20 &= 0xFFCF;
            flags = D_8008FDD4->unk20;
        }
        if (flags & 0x20) {
            func_15007668();
        }
        }
    }
    if (obj != NULL) {
        if (func_151F2CDC() == 1) {
            if (obj->unk1FF == 0) {
                obj->unk1FF = 1;
            }
        } else {
            obj->unk1FF = 0;
        }
    }
    if (D_8008FEF8 != 0) {
        if (D_8008FDD4->unk20 & 0x20) {
            D_800E0A96 = 1;
        }
        D_8008FDD4->unk20 &= 0xFFDF;
        if (D_8008FEF8 != 0) {
            func_1504332C(0xFF, 0, 0, val);
            func_15042D94(0x94, 0x14, 0x81, D_800AB800, 0x10);
        }
        D_800BE9EC = 0;
        if (D_800E9D00 & 0x400000) {
            D_800BE9EC = 1;
        }
        if (D_800E9D00 & 0x800000) {
            D_800BE9EC = 2;
        }
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151DF574.s")

struct Struct151DFF38 {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s8 unkC;
    s8 padD;
    s8 unkE;
    s8 padF;
};

extern s8 D_8008FE2C;
extern u8 D_800E0A94;
extern char D_800AB814[];
extern char D_800AB818[];
extern char D_800AB828[];
extern char D_800AB82C[];
extern char D_800AB830[];
extern char D_800AB834[];
extern char D_800AB838[];
extern char D_800AB83C[];
extern char D_800AB840[];
void func_15042E3C(char *, s32);
void func_151E327C(void);

void func_151DFF38(void) {
    s32 idx;
    u8 alpha;
    s32 sfx;
    s32 val;
    struct Struct151DFF38 *entry;
    s32 y;

    alpha = D_800E0A94;
    sfx = 0;
    idx = D_8008FDD4->unk2C - 3;
    if (D_8008FE2C < 2) {
        if (D_8008FDD4->unk20 & 0xC) {
            D_8008FE2C = 1 - D_8008FE2C;
            sfx = 0x62D;
        }
    } else if (D_8008FDD4->unk20 & 0xC) {
        D_8008FE2C = 5 - D_8008FE2C;
        sfx = 0x62D;
    }
    func_150432FC(0x94, 0x46);
    entry = &((struct Struct151DFF38 *)&D_800BE3F8)[idx];
    if (entry->unk8 == -1) {
        D_8008FE2C = 0;
    }
    if (idx == 2) {
        func_1504332C(0xFF, 0x37, 0x37, alpha);
    } else if (idx == 1) {
        func_1504332C(0x78, 0xFF, 0x16, alpha);
    } else {
        func_1504332C(0x20, 0x55, 0xFF, alpha);
    }
    if (entry->unk8 != -1) {
        val = entry->unk4;
        y = 0;
        if (val == 32000) {
            val = 1000000;
            func_150432BC(0.75f);
            y = 2;
        }
        func_15042D94(0x70, y + 0x33, 0, D_800AB814, val);
        func_150432BC(1.0f);
        {
        u32 time;
        u32 sec;

        time = entry->unk0;
        sec = time % 60;
        time /= 60;
        func_15042D94(0xA4, 0x33, 0, D_800AB818, time / 60, time % 60, sec);
        }
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    if (entry->unk8 >= 0) {
        func_15042D94(0x94, 0xAF, 1, D_800AB828, 0x1A);
        func_150432FC(0x58, 0xAF);
        if (D_8008FE2C < 2) {
            func_15042E3C(D_800AB82C, (D_8008FE2C == 0) + 0x16);
        } else {
            func_15042D94(0x94, 0x91, 1, D_800AB830, 0x19);
        }
    } else {
        func_15042D94(0x94, 0x6E, 1, D_800AB834, 0x31);
    }
    func_150432FC(0xD0, 0xAF);
    if (D_8008FE2C < 2) {
        if (entry->unk8 >= 0) {
            func_15042E3C(D_800AB838, (D_8008FE2C != 0) + 0x18);
        }
        if (D_8008FE2C == 0) {
            if (D_8008FDD4->unk20 & 0x10) {
                s32 pad;

                D_800E0B94 = 7;
                D_800E0A88 = func_151E327C;
                D_8008FD74 = 3;
                D_8008FDA8 = idx;
                D_800BE3DC = -1;
                D_80082BC0 = 0;
                func_150076BC(entry->unkE);
                sfx = 0x500;
            }
        } else if (D_8008FDD4->unk20 & 0x10) {
            D_8008FE2C = 2;
            sfx = 0x500;
        }
    } else {
        func_15042D94(0x6C, 0xAF, 1, D_800AB83C, (D_8008FE2C == 3) + 0x1F);
        func_15042D94(0xBC, 0xAF, 1, D_800AB840, (D_8008FE2C == 2) + 0x1D);
        if (D_8008FDD4->unk20 & 0x10) {
            if (D_8008FE2C == 3) {
                D_8008FE2C = 0;
                func_15007750(idx);
                D_800BE3DF = 0x29;
                D_800BE3E0 = 0;
                D_800BE3E4 = 0;
                func_15017790();
                entry->unkC = 0x18;
                entry->unk4 = 0;
                entry->unk8 = -1;
                D_8008FDA4 = 0;
            } else {
                D_8008FE2C = 1;
            }
            sfx = 0x500;
        } else if (D_8008FDD4->unk20 & 0x20) {
            D_8008FE2C = 1;
        }
        D_8008FDD4->unk20 &= ~0x20;
    }
    if (sfx != 0) {
        func_10010F30(sfx, 0x4650, 0x40, 0, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E0424.s")

void func_151E09DC(void) {
    s32 arg0;
    s32 temp;

    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    func_15042D94(0x94, 0x7E, 1, D_800AB850, 0x1A);
    func_15042D78(0x81);

    arg0 = D_800AB754[D_800AB690[D_8008FDD4->pad42 * 10]];
    if (D_800E0A96 == 0) {
        D_8008FDD4->unk20 &= ~0x20;
        temp = D_800E0A95 - (D_800BE9E4 << 5);
        if (temp <= 0) {
            D_8008FDD4->unk3F = 0;
            if (D_8008FDE0 != 0) {
                func_151E2404();
            }
            D_8008FDE0 = 0;
            D_800E0A96 = -1;
            return;
        }
        D_800E0A95 = temp;
    } else {
        temp = D_800E0A95 + (D_800BE9E4 << 5);
        if (temp >= 0x100) {
            temp = 0xFF;
        }
    }

    D_800E0A95 = temp;
    if (temp != 0xFF) {
        D_8008FDD4->unk20 = 0;
    }
    func_151E3344(arg0, 1, 0, temp & 0xFF, 0);

    if (D_8008FDD4->unk20 & 0x20) {
        D_800E0A96 = 0;
    }
    D_8008FDD4->unk20 &= ~0x20;
}

extern s8 D_8008FE40;
extern s8 D_8008FE44[];
extern u8 D_800E0B8C;
extern s8 D_800E0B90[];
extern s8 D_800AB68C[];
extern struct127 *D_800E0BA0[];
extern u32 D_8008FEA4[];
extern s8 D_8008FEC4[];
extern char D_800AB854[];
extern s8 D_8008FE2C;
void func_151E7F60(s32 arg0, s32 arg1);

void func_151E0B70(void) {
    s32 val;
    s32 dir;
    s32 i;
    s32 j;
    s32 k;
    s32 found;
    s32 mode;
    s8 *remap;
    u32 mask;
    s8 numSlots;
    s8 force;

    remap = NULL;
    mask = -1;
    force = 0;
    if (D_800E0A96 != -1) {
        D_8008FDD4->unk20 &= 0xFFDF;
        val = D_800E0A95 - (D_800BE9E4 << 5);
        if (val <= 0) {
            D_800E0A95 = 0;
            D_8008FDD4->unk3F = D_800E0A96;
            D_800E0A96 = -1;
            for (i = 0; i < 4; i++) {
                if (D_800E0BA0[i] != NULL) {
                    func_15060F28(D_800E0BA0[i], 1);
                    D_800E0BA0[i] = NULL;
                }
            }
            return;
        }
        D_800E0A95 = val;
    } else {
        val = D_800E0A95 + (D_800BE9E4 << 5);
        if (val >= 0x100) {
            val = 0xFF;
        }
        D_800E0A95 = val;
    }
    func_1504332C(0xFF, 0xFF, 0xFF, val);
    if (D_800AB68C[D_8008FDD4->pad42 * 10 + 5] < 0) {
        mode = 5;
        i = -D_800AB68C[D_8008FDD4->pad42 * 10 + 5] - 1;
        remap = D_8008FEC4 + (i << 2);
        mask = D_8008FEA4[i];
        if (D_800E9D00 & 0x20) {
            mask |= 0x8000;
        }
        if (D_800E9D00 & 0x10) {
            mask |= 0x10000;
        }
        if (D_800E9D00 & 0x40) {
            mask |= 0x30;
        }
        if (D_800E9D00 & 0x80) {
            mask |= 0x43C0;
        }
        if (D_800E9D00 & 0x100) {
            mask |= 0xE0800;
        }
        if (D_800E9D00 & 0x10000) {
            mask |= 0xA;
        }
        if (D_800E9D00 & 0x20000) {
            mask |= 0x300000;
        }
    } else {
        mode = 1;
    }
    if (D_8008FDD4->unk2C == 7) {
        numSlots = 2;
    } else {
        numSlots = 4;
    }
    D_800E0B8C = 0;
    if ((mode == 5) && (D_800AB68C[D_8008FDD4->pad42 * 10 + 8] != 8)) {
        if (D_800E0BA0[0] == NULL) {
            func_151E7F60(0, D_800E0B90[0]);
        }
        func_15042D94(0x94, 0x9B, 1, D_800AB854, 0x1A);
        D_800E0B8C = 1;
    }
    for (i = 1; i < 4; i++) {
        if ((i < numSlots) && (val == 0xFF) && ((&D_800BE930)[i] & 0x1000)) {
            found = 0;
            for (j = 0; j < D_8008FE40; j++) {
                if (i == D_8008FE44[j]) {
                    D_8008FE40--;
                    found = 1;
                    for (k = j; k < D_8008FE40; k++) {
                        D_8008FE44[k] = D_8008FE44[k + 1];
                        D_800E0B90[k] = D_800E0B90[k + 1];
                    }
                }
            }
            if (!found) {
                for (j = 0; j < D_8008FE40 && D_8008FE44[j] < i; j++) {
                }
                for (k = D_8008FE40; k > j; k--) {
                    D_8008FE44[k] = D_8008FE44[k - 1];
                    D_800E0B90[k] = D_800E0B90[k - 1];
                }
                D_8008FE44[j] = i;
                if (remap != NULL) {
                    D_800E0B90[j] = remap[i];
                } else {
                    D_800E0B90[j] = i;
                }
                D_8008FE40++;
                if (D_800E0BA0[i] != NULL) {
                    force = 1;
                }
            }
        }
    }
    for (i = 0; i < D_8008FE40; i++) {
        D_800E0C00[i] = i;
        if (D_800E0B8C != 0) {
            j = D_8008FE44[i];
            k = D_800E0B90[i];
            dir = 1;
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[j] & 8) {
                D_800E0B90[i]++;
                if (D_800E0B90[i] >= 22) {
                    D_800E0B90[i] = 0;
                }
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[j] & 4) {
                D_800E0B90[i]--;
                dir = -1;
                if (D_800E0B90[i] < 0) {
                    D_800E0B90[i] = 21;
                }
            }
            while (!((1 << D_800E0B90[i]) & mask)) {
                D_800E0B90[i] += dir;
                if (D_800E0B90[i] < 0) {
                    D_800E0B90[i] = 21;
                }
                if (D_800E0B90[i] >= 22) {
                    D_800E0B90[i] = 0;
                }
            }
            if ((k != D_800E0B90[i]) || force) {
                func_151E7F60(j, D_800E0B90[i]);
            }
        }
    }
    for (i = D_8008FE40; i < 16; i++) {
        D_8008FE44[i] = 0xFF;
    }
    if (val == 0xFF) {
        if (D_8008FDD4->unk20 & 0x10) {
            D_8008FD74 = 8;
            func_10010F30(0x500, 0x7D00, 0x40, 0, 0);
            D_800E0A96 = mode;
        }
        if (D_8008FDD4->unk20 & 0x20) {
            D_800E0A96 = 0;
            D_8008FE2C = 0;
        }
    }
    D_8008FDD4->unk20 &= 0xFFDF;
}

void func_151E4314(void);
void func_15042E3C(char *, s32);
extern s8 D_8008FDA0;
extern s8 D_8008FE40;
extern s8 D_8008FE44[];
extern s8 D_8008FEE3[];
extern s8 D_800AB74C[][4];
extern char D_800AB858[];
extern char D_800AB85C[];
extern char D_800AB860[];

void func_151E1214(void) {
    s32 alpha;
    s8 *limits;
    s32 limit;
    s32 i;
    s32 x;

    if (D_800E0A96 != -1) {
        *(u16 *)((u8 *)D_8008FDD4 + 0x20) &= 0xFFDF;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_800E0A95 = 0;
            ((u8 *)D_8008FDD4)[0x3F] = D_800E0A96;
            D_800E0A96 = -1;
            return;
        }
        D_800E0A95 = alpha;
    } else {
        alpha = D_800E0A95 + (D_800BE9E4 << 5);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        D_800E0A95 = alpha;
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    limits = D_800AB74C[D_8008FEE3[-D_800AB690[*(s8 *)((u8 *)D_8008FDD4 + 0x42) * 10 + 1]]];
    limit = *(limits - 1 + D_8008FE40);
    for (i = 0; i < D_8008FE40; i++) {
        if (D_8008FE44[i] >= 0) {
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 9) {
                D_8008FDA0++;
                if (limit >= D_8008FE40 + D_8008FDA0) {
                    func_10010F30(0x4FF, 0x4650, 0x40, 0, 0);
                }
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 6) {
                D_8008FDA0--;
                if (D_8008FDA0 >= 0) {
                    func_10010F30(0x4FF, 0x4650, 0x40, 0, 0);
                }
            }
            if (D_8008FDA0 < 0) {
                D_8008FDA0 = 0;
            }
        }
    }
    if (limit < D_8008FE40 + D_8008FDA0) {
        D_8008FDA0 = limit - D_8008FE40;
    }
    limit = D_8008FE40 + D_8008FDA0;
    if (limit < 2) {
        limit = 2;
    }
    D_8008FDA0 = limit - D_8008FE40;
    for (i = D_8008FE40; i < limit; i++) {
        D_8008FE44[i] = -2;
        D_800E0C00[i] = 0;
    }
    for (i = limit; i < 16; i++) D_8008FE44[i] = -1;
    if (alpha == 0xFF && (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x10)) {
        D_800E0B94 = 7;
        D_8008FD74 = 8;
        func_10010F30(0x500, 0x7D00, 0x40, 0, 0);
        if (D_800AB690[*(s8 *)((u8 *)D_8008FDD4 + 0x42) * 10 + 2] == -1) {
            D_800E0A88 = func_151E2834;
        } else {
            D_800E0A88 = func_151E4314;
        }
    }
    func_15042D94(0x94, 0x55, 1, D_800AB858, 0x1A);
    x = 0xA1 - D_8008FE40 * 13 - D_8008FDA0 * 10;
    for (i = 0; i < limit; i++) {
        func_150432FC(x, 0x82);
        if (D_8008FE44[i] >= 0) {
            func_15042E3C(D_800AB85C, D_8008FE44[i] + 1);
            x += 0x1A;
        } else {
            func_15042E3C(D_800AB860, 5);
            x += 0x14;
        }
    }
    if (alpha == 0xFF && (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x20)) {
        D_800E0A96 = 4;
    }
    *(u16 *)((u8 *)D_8008FDD4 + 0x20) &= 0xFFDF;
}

extern s8 D_8008FDC4;
typedef struct { s8 v[4]; } Offs4;
extern Offs4 D_8008FF34;
extern u8 D_800AB62C[];
extern s8 D_800AB710[][12];
extern char D_800AB864[];
extern char D_800AB868[];
extern char D_800AB86C[];
extern char D_800AB870[];
extern char D_800AB874[];
extern char D_800AB878[];
extern char D_800AB87C[];
extern char D_800AB880[];
extern char D_800AB884[];
extern char D_800AB88C[];
extern char D_800AB890[];
extern char D_800AB894[];
extern char D_800AB898[];
extern s8 D_800E0BB0;
extern s8 D_800E0BC8[];
s32 func_151E24F0(s32 *arg0);

void func_151E1744(void) {
    s32 alpha;
    s8 *entry;
    s32 i;
    s8 special;
    s8 min;
    s8 max;
    s8 def;
    s8 team;
    s8 lim;
    s8 cap;
    s32 extra[3];
    s8 counts[8];
    s32 more;
    s16 step;
    s16 x;
    s16 y;
    s16 k;
    s16 xs[4];
    s16 ys[4];
    Offs4 offs;
    s8 maxs[4];
    s8 sizes[4];
    s32 sfx;
    s32 v;
    s32 old;

    sfx = 0;
    offs = D_8008FF34;
    if (D_800E0A96 != -1) {
        *(u16 *)((u8 *)D_8008FDD4 + 0x20) &= 0xFFDF;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_800E0A95 = 0;
            ((u8 *)D_8008FDD4)[0x3F] = D_800E0A96;
            D_800E0A96 = -1;
            return;
        }
        D_800E0A95 = alpha;
    } else {
        alpha = D_800E0A95 + (D_800BE9E4 << 5);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        D_800E0A95 = alpha;
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    entry = D_800AB710[D_800AB690[*(s8 *)((u8 *)D_8008FDD4 + 0x42) * 10 + 1]];
    special = entry[7];
    D_800E0BB0 = entry[0];
    D_800E0BC8[0] = entry[8];
    D_800E0BC8[1] = entry[9];
    D_800E0BC8[2] = entry[10];
    D_800E0BC8[3] = entry[11];
    if (D_8008FE40 < 3) {
        min = entry[1];
        max = entry[2];
        def = entry[3];
    } else {
        min = entry[4];
        max = entry[5];
        def = entry[6];
    }
    for (i = 0; i < 4; i++) {
        maxs[i] = def;
        if (special != 0 && i >= 1) {
            maxs[i] = special;
        }
    }
    i = 0;
    while (i < D_800E0BB0) {
        counts[i] = 0;
        ys[i++] = 0x6E;
    }
    for (i = 0; i < D_8008FE40; i++) {
        counts[D_800E0C00[i]]++;
    }
    for (i = D_8008FE40; i < 16; i++) {
        D_8008FE44[i] = -1;
    }
    for (i = 0; i < D_8008FE40; i++) {
        if (D_8008FE44[i] >= 0) {
            old = D_800E0C00[i];
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 0xF) {
                sfx = 0x4FF;
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 8) {
                D_800E0C00[i]++;
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 4) {
                D_800E0C00[i]--;
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 2) {
                D_8008FDA0++;
            }
            if (((u16 *)((u8 *)D_8008FDD4 + 0x22))[D_8008FE44[i]] & 1) {
                D_8008FDA0--;
            }
            if (D_8008FDA0 < 0) {
                D_8008FDA0 = 0;
                sfx = 0;
            }
            D_800E0C00[i] &= D_800E0BB0 - 1;
            if (counts[D_800E0C00[i]] >= maxs[D_800E0C00[i]]) {
                D_800E0C00[i] = old;
            }
            counts[D_800E0C00[i]]++;
            counts[old]--;
        }
    }
    D_800E0BB1 = 1;
    for (i = 0; i < D_800E0BB0; i++) {
        if (D_800E0BB1 < counts[i]) {
            D_800E0BB1 = counts[i];
        }
    }
    D_800E0BB1 += D_8008FDA0;
    while (D_800E0BB1 * D_800E0BB0 < min) {
        D_800E0BB1++;
        D_8008FDA0++;
    }
    while (max < D_800E0BB1 * D_800E0BB0) {
        D_800E0BB1--;
        D_8008FDA0--;
        if (D_8008FDA0 < 0) {
            D_8008FDA0 = 0;
        }
    }
    if (special != 0 && D_800E0BB1 == 1) {
        special = 1;
    }
    for (i = 0; i < 4; i++) {
        v = D_800E0BB1;
        if (i == 1 && special != 0) {
            v = special;
        }
        sizes[i] = v;
    }
    if (D_800E0BB0 == 2) {
        if (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x40) {
            D_8008FDC4++;
        } else if (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x80) {
            D_8008FDC4--;
        }
        if (D_8008FE40 >= 3) {
            D_8008FDC4 = 0;
        }
        lim = D_800E0BB1;
        if (D_8008FDC4 < -lim) {
            D_8008FDC4 = -lim;
        } else if (lim < D_8008FDC4) {
            D_8008FDC4 = lim;
        } else if (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0xC0) {
            sfx = 0x4FF;
        }
        sizes[0] += D_8008FDC4;
        sizes[1] -= D_8008FDC4;
        for (i = 0; i < 2; i++) {
            cap = (i == 1 && special != 0) ? special : 8;
            if (sizes[i] <= 0) {
                sizes[i] = 1;
            }
            if (cap < sizes[i]) {
                sizes[i] = cap;
            }
        }
    }
    step = 0x128 / D_800E0BB0;
    x = step >> 1;
    k = D_8008FE40;
    for (i = 0; i < D_800E0BB0; i++) {
        xs[i] = x;
        x += step;
        v = sizes[i];
        while (counts[i] < v) {
            D_8008FE44[k] = -2;
            D_800E0C00[k] = i;
            counts[i]++;
            k++;
        }
    }
    if (alpha == 0xFF) {
        if (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x10) {
            D_800E0B94 = 7;
            D_800E0A88 = func_151E4314;
            D_8008FD74 = 8;
            sfx = 0x500;
        } else if (*(u16 *)((u8 *)D_8008FDD4 + 0x20) & 0x20) {
            D_800E0A96 = 4;
        }
    }
    *(u16 *)((u8 *)D_8008FDD4 + 0x20) &= 0xFFDF;
    func_1504332C(0xFF, 0xFF, 0, alpha);
    func_150432FC(0x94, ((D_800E0BB0 == 4) << 5) + 0x78);
    func_15042E3C(D_800AB864, 0x1A);
    for (i = 0; i < D_800E0BB0; i++) {
        x = xs[i];
        old = D_800E0BC8[i] * 3;
        func_1504332C(D_800AB62C[old], D_800AB62C[old + 1], D_800AB62C[old + 2], alpha);
        func_150432FC(x, 0x50);
        if (D_800E0BC8[i] == 4) {
            func_15042E3C(D_800AB868, 9);
        } else if (D_800E0BC8[i] == 5) {
            func_15042E3C(D_800AB86C, 8);
        } else if (D_800E0BC8[i] == 7) {
            func_15042E3C(D_800AB870, 0xA);
        } else if (D_800E0BC8[i] == 8) {
            func_15042E3C(D_800AB874, 0xB);
        } else if (D_800E0BC8[i] == 9) {
            func_15042E3C(D_800AB878, 0x34);
        } else {
            old = (D_800E0BC8[i] < 4) ? 0x59 : 0x4C;
            func_15042E3C(D_800AB87C, D_800E0BC8[i] + old);
        }
    }
    more = func_151E24F0(extra);
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    for (i = 0; i < k; i++) {
        old = D_800E0C00[i];
        if (k == D_800E0BB0) {
            offs.v[old] = 0;
        }
        x = xs[old] + offs.v[old];
        y = ys[old];
        offs.v[old] = -offs.v[old];
        if (offs.v[old] < 0) {
            ys[old] += 0x18;
        }
        func_150432FC(x, y);
        if (D_8008FE44[i] >= 0) {
            func_15042E3C(D_800AB880, D_8008FE44[i] + 1);
            if (more != 0) {
                func_15042E3C(D_800AB884, extra[i] + 1);
            }
        } else {
            func_15042E3C(D_800AB88C, 5);
        }
    }
    if (sfx != 0) {
        func_10010F30(sfx, sfx == 0x4FF ? 0x4650 : 0x7D00, 0x40, 0, 0);
    }
    if (D_800E0BB0 == 2) {
        func_15042D94(0x94, 0xB4, 0x81, D_800AB890, 0x3A);
        func_15042D94(0x7B, 0xB4, 0x81, D_800AB894, 0x3B);
        func_15042D94(0xAD, 0xB4, 0x81, D_800AB898, 0x3C);
    }
}

extern u8 D_8008FD80;
void func_151E530C(void);
void func_151E43DC(void);

void func_151E2284(void) {
    D_8008FD80 = 3;
    func_151E530C();
    func_151E43DC();
    D_8008FD80 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E22BC.s")

extern s8 *D_80082BBC;

void func_151E2404(void) {
    s8 *record;
    s32 index;

    index = D_800AB690[*(s8 *)((u8 *)D_8008FDD4 + 0x42) * 10];
    record = D_80082BBC + (index * 10);
    record[0] = 0;
    record[1] = D_800E0BE0[2];
    record[2] = D_800E0BE0[3];
    record[3] = D_800E0BE0[0x10];
    record[4] = D_800E0BE0[0x11];
    switch (index) {
        case 0:
            record[5] = D_800E0BE0[4];
            break;
        case 1:
        case 2:
            record[5] = D_800E0BE0[0xD];
            record[6] = D_800E0BE0[0xE];
            break;
        case 5:
        case 7:
            record[5] = D_800E0BE0[0x12];
            break;
        case 8:
        case 9:
            record[5] = D_800E0BE0[7];
            record[6] = D_800E0BE0[0x13];
            break;
    }
    record[8] = D_800E0BE0[0x16];
    record[9] = D_800E0BE0[6];
    func_150076A0(D_800E0BE0);
}

extern s8 D_800AB694[];
extern s8 D_8008FE44[];
extern s8 D_800E0BCE[];
extern s8 D_800E0BEE;
extern u8 D_800BE740;

s32 func_151E24F0(s32 *arg0) {
    s32 i;
    s32 j;
    s32 count;
    s32 v;
    s8 used[4];
    s8 sel[4];
    u8 mask;

    if (arg0 == NULL) {
        D_800E0BCE[0] = 0;
        D_800E0BCE[1] = 0;
    }

    if (D_800AB694[D_8008FDD4->pad42 * 10] == 8) {
        mask = 0;
        count = 0;
        if (D_800E0BEE != 0) {
            for (i = 0; i < 4; i++) {
                used[i] = 0;
            }
            for (i = 0; i < 4; i++) {
                v = D_8008FE44[i];
                if (v >= 0) {
                    count++;
                    used[v] = 1;
                    mask |= 1 << v;
                }
            }
            mask = D_800BE740 & ~mask;
            if (count < 3) {
                if (mask != 0) {
                    for (i = 0; i < 4; i++) {
                        sel[i] = 1;
                        if (count == 1) {
                            if ((D_800BE740 & (1 << i)) == 0) {
                                sel[i] = 0;
                            }
                        }
                    }
                    j = 0;
                    if (arg0 == NULL) {
                        for (j = 16; j >= count * 2; j--) {
                            D_8008FE44[j] = D_8008FE44[j - count];
                            D_800E0C00[j] = D_800E0C00[j - count];
                        }
                        for (j = count, i = 0; (i < 4) && (j < count * 2); i++) {
                            if ((used[i] == 0) && (sel[i] != 0)) {
                                D_8008FE44[j] = i;
                                D_800E0C00[j] = D_800E0C00[j - count];
                                D_800E0BCE[j - count] = j;
                                D_80087270[j] = 10;
                                j++;
                            }
                        }
                    } else {
                        for (i = 0; (i < 4) && (j < count); i++) {
                            if ((used[i] == 0) && (sel[i] != 0)) {
                                arg0[j] = i;
                                j++;
                            }
                        }
                    }
                    return count;
                }
            }
        }
    }

    return 0;
}

typedef struct {
    f32 unk0;
    s8 unk4;
    u8 unk5;
    u8 unk6;
    u8 pad7;
} Entry151E7F60;

typedef struct {
    s8 unk0;
    s8 unk1;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s8 unk6;
    s8 unk7;
    s8 unk8;
    s8 unk9;
} Entry151E2834;

extern Entry151E7F60 D_800AB57C[];
extern s16 D_800AB77C[];
extern s16 D_800AB790[];
extern s8 D_8008FD9C;
extern s32 D_8008FD78;
extern s8 D_8008FD7C;
extern s8 D_8008FD88;
extern s8 D_8008FF37[];
extern s8 D_800E0AB0[];
extern s8 D_800E0AC0[];
extern s8 D_800E0AD0[];
extern u8 D_800E0AA8;
extern s8 D_800E0AA9;
extern s8 D_800E0AAA;
extern s16 D_800E0AA0[];
extern u8 D_800E0B8C;
extern s8 D_800E0B90[];
extern s8 D_800E0BB8[];
extern s8 D_800E0BD0;
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];
extern s8 D_800D2130;
extern s8 D_80087268;
extern u8 D_80084060[];
extern s8 D_8008FDC8;
extern s8 D_800AB68C[];
extern u16 D_8008FDC0;

void func_151E2834(void) {
    s32 i;
    s32 count;
    s32 k;
    s32 arg1;
    s32 used;
    s32 mask;
    s32 v;
    Entry151E2834 *entry;

    entry = (Entry151E2834 *)&D_800AB68C[D_8008FDD4->pad42 * 10];
    if (D_800E0BE0[3] < 3) {
        D_800E0BE0[3] = 0;
    }
    if (D_8008FD9C) {
        if (D_8008FD70) {
            for (k = D_8008FD70; k < 16 - D_8008FD70; k++) {
                i = k + D_8008FD70;
                D_8008FE44[k] = D_8008FE44[i];
                D_800E0C00[k] = D_800E0C00[i];
            }
        }
    }
    D_8008FD9C = 0;
    D_8008FD70 = 0;
    func_15017790();
    D_800BE3E4 = 0;
    bzero((u8 *)D_8008FDD4 + 0x66, 0x30);
    bzero((u8 *)D_8008FDD4 + 0x46, 0x20);
    bzero(D_800E0AD0, 0x20);
    D_800BEAC1 = 0;
    D_800BEAC0 = 0;
    D_800E0B94 = 0;
    D_8008FDA8 = -1;
    D_80084060[1] = -1;
    D_80084060[2] = -1;
    D_80084060[3] = -1;
    arg1 = entry->unk2;
    D_8008FDC8 = entry->unk5 < 0;
    count = D_800AB754[entry->unk4];
    D_8008FDC0 = D_800AB77C[entry->unk4];
    D_8008FDBC = D_800AB790[entry->unk4] | 1;
    if (count & 0x80000) {
        D_800D23A8 = D_800E0BE0[0x13];
    } else {
        D_800D23A8 = 0;
    }
    if (count & 8) {
        D_8008FD78 = D_800E0BE0[3] * 3600;
    } else {
        D_8008FD78 = 0;
    }
    if (count & 0x10000) {
        D_800E0BD0 = D_800E0BE0[0x10];
    } else {
        D_800E0BD0 = 2;
    }
    if ((count & 4) && (D_8008FD78 == 0)) {
        D_8008FDC0 |= 2;
        D_8008FDBC &= ~1;
    }
    if (D_8008FDC0 & 0x80) {
        D_8008FDC0 |= 0x1000;
    }
    if ((count & 0x20000) && (D_800E0BE0[0x11] != 0)) {
        D_8008FDC0 |= 0x20;
    }
    if ((D_8008FDC0 & 0x100) && (D_8008FD78 != 0)) {
        D_8008FDC0 &= ~0x100;
        D_8008FDC0 |= 0x200;
    }
    D_80087260 = D_800E0BE0[2];
    D_800D2130 = 3;
    D_80087264 = 0;
    D_800D2132 = 0;
    D_80087268 = D_800E0BE0[4];
    if (D_800E0BE0[3] >= 3) {
        D_80087260 = 99;
    }
    if (entry->unk8 == 8) {
        i = 0;
        do {
            D_80087270[i] = 8;
        } while (++i < 16);
        D_8008FD70 = func_151E24F0(NULL);
    }
    mask = 1;
    D_8008FD88 = 1;
    if (D_8008FE44[1] != -1) {
        mask |= 2;
        D_80084060[1] = D_8008FE44[1];
        D_8008FD88++;
    }
    if (D_8008FE44[2] != -1) {
        mask |= 4;
        D_8008FD88++;
        D_80084060[2] = D_8008FE44[2];
    }
    if (D_8008FE44[3] != -1) {
        mask |= 8;
        D_80084060[3] = D_8008FE44[3];
        D_8008FD88++;
    }
    for (i = 4; i < 16; i++) {
        if (D_8008FE44[i] != -1) {
            mask |= 1 << i;
            D_8008FD88++;
        }
    }
    if (entry->unk8 != 8) {
        v = 0;
        if (D_800E0B8C != 0) {
            for (i = 0; D_8008FE44[i] >= 0 && i < 4; i++) {
                D_80087270[i] = D_800AB57C[D_800E0B90[i]].unk6;
            }
            v = i;
        } else {
        }
        for (i = v; i < 16; i++) {
            D_80087270[i] = entry->unk8;
            if (D_8008FDD4->pad42 == 7) {
                if (D_8008FE44[i] < -1) {
                    D_80087270[i] = 11;
                }
            } else if (D_8008FDD4->pad42 == 2) {
                if (D_800E0C00[i] == 0) {
                    D_80087270[i] = 11;
                }
            } else if (D_8008FDD4->pad42 == 8) {
                if (D_800E0C00[i] != 0) {
                    D_80087270[i] = 3;
                } else {
                    D_80087270[i] = (func_150ADA20() & 0xF) + 0x11;
                }
            } else if ((D_800E0C00[i] != 0) && (entry->unk8 == 4)) {
                D_80087270[i] = 3;
            } else if ((D_8008FE44[i] < -1) && (entry->unk8 == 11)) {
                D_80087270[i] = (i & 3) + 12;
            }
        }
    }
    *(u8 *)&D_800E0B99 = 2;
    D_8008FD8C = D_8008FD88;
    D_8008FD90 = 0;
    D_800E0AA8 = 0;
    for (i = 0; i < D_8008FD88; i++) {
        if (D_8008FE44[i] >= 0) {
            D_8008FD90++;
            D_800E0AA8 |= 1 << D_800E0C00[i];
        }
    }
    *(s8 *)&D_8008FD94 = D_8008FD90;
    D_800BE618 = D_8008FD8C;
    bzero(D_800E0BB8, 0x10);
    D_800E0AAA = -1;
    D_800E0AA9 = 4;
    if (1) {}
    for (i = 0; i < 16; i++) {
        D_800E0AB0[i] = -1;
        D_800E0AC0[i] = 0;
    }
    used = 0;
    if (D_8008FDC8 != 0) {
        k = 0;
        D_800E0AA9 = D_8008FD8C;
        for (i = 0; i < D_8008FD88; i++) {
            v = D_8008FE44[i];
            if (v >= 0) {
                D_800E0C00[i] = v;
                used |= 1 << D_800E0C00[i];
            } else {
                while (used & (1 << k)) {
                    k++;
                }
                D_800E0C00[i] = k;
                used |= 1 << k;
                k++;
            }
        }
    }
    D_800E0C10[0] = -1;
    D_800E0C10[1] = -1;
    D_800E0C18[0] = -1;
    D_800E0C18[1] = -1;
    D_800E0AA0[0] = 0;
    D_800E0AA0[1] = 0;
    func_151E7EF8();
    *(s8 *)&D_800E0C20 = 0;
    D_800D2E40 = 0;
    D_8008FD7C = D_80087270[0];
    func_1501C730(6, arg1, 0, D_8008FF37[D_8008FD88], mask);
}

void func_151E30C4(void) {
    s32 sp2C;
    s32 sp28;
    s32 sp24;

    sp24 = 0;
    if (D_8008FDB4 != 0) {
        sp28 = D_8008FDAC;
    } else {
        func_1509CF28(D_8008FF00, &sp2C);
        func_1509D08C(D_8008FF00, D_8008FF04, &sp28, &sp24);
        func_1509D054();
    }

    func_15017790();
    func_1509CC94(sp28);
    func_1509CDDC(sp28);
    D_800BE9F4 = (u16 *)func_1509CA30(sp28);
    func_15085710(0, 9, sp24);
    D_800BE3E4 = sp24;

    D_800DD2C0 = func_1509CA50(sp28);
    if (D_8008FDB4 == 2) {
        D_800DD2C0 = D_800E0BD1;
    }

    D_8008FD70 = 0;
    D_800D23A8 = 0;
    D_8008FDBC = 0;
    D_8008FDA8 = -1;
    D_800E0B94 = 0;
    *(u8 *)&D_800E0B99 = 1;
    D_8008FD8C = 1;
    D_8008FD90 = 1;
    *(s8 *)&D_8008FD94 = 1;

    D_800BE618 = 1;
    D_80087260 = 3;
    if (D_800E9D00 & 0x200) {
        D_80087260 = 0x33;
    }

    D_80087270[0] = 0;
    D_800D18A8 = 0;
    D_800E0BB1 = 1;
    func_151E7EF8();
    D_800D2E40 = 1;
    func_1501C730(6, (s32)D_800BE9F4, func_1509CA50(sp28), 0, 1);
}

void func_151E327C(void) {
    D_80087260 = 3;
    if (D_800E9D00 & 0x200) {
        D_80087260 = 0x33;
    }

    D_80087270[0] = 0;
    D_8008FD8C = 1;
    D_8008FD90 = 1;
    *(s8 *)&D_8008FD94 = 1;
    D_8008FDBC = 0;
    D_800BE618 = 1;
    D_800D23A8 = 0;
    D_800E0B94 = 0;
    D_800D2E40 = 0;
    func_1501C730(6, *(s8 *)&D_800BE3DF, D_800BE3E0, 0, 1);
    *(u8 *)&D_800E0B99 = 1;
    D_800E0BB1 = 1;
    func_151E7EF8();
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E3344.s")

void func_151E4264(void) {
    if (D_8008FD80 != 0) {
        D_8008FD80 = 0;
        return;
    }

    func_151E530C();
    if ((D_800E0B9A != 0) || (D_800C35EA != 1)) {
        if ((D_800C35EA == 1) && (func_151F2CDC() == 1)) {
            func_151F2D6C(0, 0x2DE0);
        }
        D_800E0B94 = 7;
        D_800E0A88 = func_151E2834;
        D_8008FD74 = 8;
    }
}

void func_151E4314(void) {
    struct Struct151E4314 *entry;
    s32 idx;
    s32 arg1;

    idx = D_800AB692[D_8008FDD4->pad42 * 10];
    if (idx >= 0) {
        entry = &D_800AB7A4[idx];
        if (D_800E0C00[0]) {
            arg1 = entry->unk3;
        } else {
            arg1 = entry->unk2;
        }
        func_1501D348(entry->unk0, arg1, 0, 0, 0);
        D_8008FD80 = 1;
        D_800E0B94 = 5;
        D_8008FD8C = 1;
        D_8008FD90 = 1;
    } else {
        func_151E2834();
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E43DC.s")

extern s8 D_8008FE2C;
extern u16 D_8008FDC0;
extern s8 D_800AB68C[];
extern struct Struct151E4314 D_800AB7BC[];
extern s8 D_800E0A86;
extern s8 D_800E0AC0[];
extern s8 D_800E0AC1;
extern s8 D_800E0AC2;
extern s8 D_800E0AC3;

void func_151E4BD8(void) {
    s8 idx;
    s8 count;
    s16 arg1;
    s32 i;

    D_8008FE2C = 0;
    D_800BEAC1 = 0;
    D_8008FDA4 = 0;
    D_800E0A90 = 0;
    count = 0;
    idx = D_800AB68C[D_8008FDD4->pad42 * 10 + 7];
    D_8008FDC0 &= 0xF369;
    if (D_800AB68C[D_8008FDD4->pad42 * 10 + 5] >= 0) {
        for (i = 0; i < 4; i++) {
            if (D_800E0AC0[i] == 1) {
                count++;
            }
        }
        count = count >= 2;
    }
    if (idx < 0 || count != 0) {
        if (idx == -2 && D_800E0A86 == 0) {
            D_800E0B94 = 6;
            D_8008FD80 = 2;
            D_8008FDC0 = 0;
            return;
        }
        arg1 = 0x1D;
        if (count == 0) {
            D_800E0B94 = 3;
        } else {
            D_800E0B94 = 6;
            D_8008FD80 = 4;
        }
    } else {
        arg1 = D_800AB7BC[idx].unk0;
        D_800E0B94 = 6;
        D_8008FD80 = 1;
    }
    D_800D2E40 = 0;
    func_1501C730(6, arg1, 0, 0, 1);
    func_151E557C();
    D_8008FD8C = 1;
    D_8008FD90 = 1;
}

extern u8 D_800E0B94;

void func_151E4DC4(void) {
    D_800E0B94 = 0xA;
}

extern s16 D_800E0B9A;

void func_151E4DD8(void) {
    if (D_800E0B9A & 0x8020) {
        D_800E0B94 = 4;
    }
}

void func_151E4E00(void) {
    D_8008FDCC = 0;
    func_151E557C();
    D_800E0B94 = 3;
    D_8008FDA4 = 0;
    D_8008FD80 = 0;
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
}

void func_151E4E64(void) {
    func_151E530C();
    func_151E55A8();
    if (D_800E0A90 >= 0x4B1) {
        D_800E0B9A |= 0x8000;
    }
    if (D_800E0B9A != 0) {
        D_800E0B94 = 7;
        D_800E0A88 = func_151E4E00;
        D_8008FD74 = 8;
    }
}

extern f32 D_8008FDD0;
extern f32 D_800ABA2C;
extern f32 D_800ABA30;
extern f32 D_800ABA34;
extern f32 D_800DDDC8;
extern f32 D_800DDDD8;

void func_151E4EE8(void) {
    s32 v;

    D_8008FDD0 = 1.0f - (f32)*(u8 *)&D_800E0B96 / 255.0f;
    if ((u8)D_8008FD74 == 0xFF) {
        if (D_800ABA2C < D_800DDDC8) {
            *(u8 *)&D_800E0B96 += D_800BE9E4;
            if (*(u8 *)&D_800E0B96 < 0x55) {
                D_800DDDD8 = D_800ABA30;
            } else {
                D_800DDDD8 = D_800ABA34;
            }
        }
        if (D_800DDDC8 < 1.0f) {
            return;
        }
        if (*(u8 *)&D_800E0B96 < 0x6E) {
            return;
        }
    } else if (*(u8 *)&D_800E0B96 != 0xFF) {
        v = *(u8 *)&D_800E0B96 + D_800BE9E4 * (u8)D_8008FD74;
        if (v >= 0x100) {
            v = 0xFF;
        }
        *(u8 *)&D_800E0B96 = v;
        return;
    }
    *(u8 *)&D_800E0B96 = 0xFF;
    D_8008FD74 = 8;
    D_8008FDD0 = 1.0f;
    D_800E0A88();
}

extern struct Struct151E5034 *D_8008FDD4;

void func_151E5034(void) {
    D_8008FDD4->unk0 = 0.0f;
    D_8008FDD4->unk4 = 0.0f;
    D_8008FDD4->unk2B = 0;
    D_8008FDD4->unk20 = 0;
    D_8008FDD4->unk3E = 0;
    D_8008FDD4->unk3F = 0;
    D_8008FDD4->unk41 = 0;
    D_8008FDD4->unk43 = 0;
    D_8008FDD4->unk44 = 0;
    D_8008FDD4->unk10 = 0.0f;
    D_8008FDD4->unk2A = 0;
    D_8008FDD4->unk14 = 0.0f;
    D_8008FDD4->unk18 = 0.0f;
    D_8008FDD4->unk1C = 0.0f;
    D_8008FDD4->unkC = 0.0f;
    D_8008FDD4->unk8 = 0.0f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E50C8.s")

void func_151E51EC(void) {
    u8 state;
    s32 timer;

    state = D_800E0B94;
    D_8003C8E0 = 0x08000000;
    D_800E0BDC = 0xFF;
    func_15042D78(0x81);

    D_800E0B9A = D_800BE930;
    timer = *(s8 *)&D_8008FE28;
    if (timer != 0) {
        D_8008FE28 = timer - 1;
        D_800E0B9A = 0;
    }

    if (D_800E0B94 == 0) {
        func_151E6964(2);
    } else if (D_800E0B94 == 8) {
        func_151E6964(4);
    } else {
        func_151E6964(1);
    }

    D_8008FDEC[D_800E0B94]();

    if (D_800E0B94 != 7) {
        D_800E0A90 += D_800BE9E4;
        if (D_800E0B94 == state) {
        } else {
            D_800E0B95 = state;
            D_800E0A90 = 0;
        }
    }
    D_8003C8E0 = 0;
}

void func_151E530C(void) {
    s32 v;
    struct Struct151E5034 *p;

    if (D_800E0B94 != 2) {
        if (*(u8 *)&D_800E0B96 != 0) {
            v = *(u8 *)&D_800E0B96 - D_800BE9E4 * (u8)D_8008FD74;
            if (v < 0) {
                v = 0;
            }
            *(u8 *)&D_800E0B96 = v;
        }
        p = D_8008FDD4;
        if (p != NULL && p->unk2B == 0 && p->unkC > 0.0f && p->unk8 > 0.5f) {
            v = (p->unk8 - 0.5f) * 524.0f;
            if (v >= 0x100) {
                v = 0xFF;
            }
            *(u8 *)&D_800E0B96 = v;
        }
    }
}


void func_151E53E8(s32 arg0) {
    s32 i;

    D_800E0A90 = 0;
    D_8008FE2C = 0;
    D_80087270[0] = 0;
    if (D_800BE616 != 0) {
        D_800E0B94 = 7;
        D_8008FD74 = 8;
        if (D_800AB68C[D_8008FDD4->pad42 * 10 + 7] != -1) {
            D_8008FDD4->unk3F = 4;
            D_800E0A88 = func_151E4BD8;
        } else {
            D_8008FDD4->unk3F = 3;
            D_800E0A88 = func_151DDBA0;
        }
        for (i = 0; i < D_8008FD90; i++) {
            *(s16 *)((u8 *)D_8008FDD4 + i * 12 + 0x6C) = D_800CC2D0[i].unk31C->unk1AA;
        }
        D_8008FDC0 &= 0xE35F;
        D_800E0A86 = arg0;
        if (D_800AB68C[D_8008FDD4->pad42 * 10 + 7] == -2) {
            if (arg0 == 0) {
                func_151E4BD8();
                return;
            }
            D_800E0A88 = func_151DDBA0;
            D_8008FDD4->unk3F = 3;
        }
    } else {
        D_800E0B94 = 4;
    }
}

extern u8 D_80084060[];

void func_151E557C(void) {
    D_80084060[0] = 0;
    D_80084060[1] = 1;
    D_80084060[2] = 2;
    D_80084060[3] = 3;
}

void func_151E55A8(void) {
    s32 temp;
    s32 step;
    s32 direction;

    direction = D_800E0B98;
    if (direction == 0) {
        step = D_800BE9E4 * 8;
    } else {
        step = -D_800BE9E4 * 8;
    }
    temp = D_800E0B97;
    temp += step;
    if (temp >= 0x100) {
        temp = 0x1FE - temp;
        D_800E0B98 = direction ^ 1;
    } else if (temp < 0) {
        temp = -temp;
        D_800E0B98 = direction ^ 1;
    }
    D_800E0B97 = temp;
}

extern u8 D_800E0A8C;

void func_151E562C(void) {
    if (D_800E0A8C != 0) {
        D_800E0A8C = 0;
    }
}

extern s8 D_8008FDC8;

s8 func_151E564C(void) {
    return D_8008FDC8;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E565C.s")

extern u8 D_800BE616;
extern u8 D_800E0B94;
extern s8 D_800E0C00[];

s32 func_151E5F64(s32 arg0) {
    s32 ret;

    if ((D_800BE616 != 0) || (D_800E0B94 != 0)) {
        ret = D_800E0C00[arg0];
        if (ret < 0) {
            ret = 0;
        }
        return ret;
    }
    return arg0;
}

s8 func_151E5FAC(void) {
    if (D_800E0BEB != 0) {
        if (D_8008FD8C >= 5) {
            return D_8008FD90;
        }
        return D_8008FD8C;
    }
    return D_8008FD90;
}

typedef struct {
    /* 0x00 */ char pad0[0x46];
    /* 0x46 */ s16 unk46[4];
    /* 0x4E */ char pad4E[0x18];
    /* 0x66 */ struct {
        /* 0x00 */ s16 unk0;
        /* 0x02 */ s16 unk2;
        /* 0x04 */ s16 unk4;
        /* 0x06 */ s16 unk6;
        /* 0x08 */ s16 unk8;
        /* 0x0A */ s16 unkA;
    } rec[4];
} ScoreState151E5FF4;

s32 func_1000F568(s32 arg0, s32 arg1);
void func_10010558(u16 arg0, struct127 *arg1, s32 arg2, s16 arg3, u16 arg4, s32 arg5);

#define SCORE ((ScoreState151E5FF4 *)D_8008FDD4)

void func_151E5FF4(struct127 *arg0, struct127 *arg1, s32 arg2) {
    struct127 *player;
    s32 pad50;
    s32 idx;
    s32 idx2;
    s32 sound;
    s32 delay;
    s32 count;
    s32 state;
    s32 state2;
    s32 r;
    u8 v;
    s32 h;

    player = arg0;
    idx = arg0 - D_800CC2D0;
    if (idx >= D_8008FD8C) {
        idx = arg0->unk124 - 1;
        if (idx >= 0) {
            player = &D_800CC2D0[idx];
        }
    }
    if ((idx >= 0) && (idx < D_8008FD8C) && (idx < D_8008FD8C)) {
        idx2 = arg1 - D_800CC2D0;
        D_800E0BB8[idx2] = idx | 0x80;
        if (idx < D_8008FD90) {
            if (D_800E0C00[idx] != D_800E0C00[idx2]) {
                SCORE->rec[idx].unk0++;
                if (arg2 == 0x2E) {
                    SCORE->rec[idx].unk4++;
                }
            }
            if ((arg1->health <= 0) && (D_800E0C00[idx] == D_800E0C00[idx2]) && (arg2 != 0x2D)) {
                SCORE->rec[idx].unkA++;
            }
        }
        if (arg1->health <= 0) {
            if ((SCORE->unk46[idx] < 9999) && (D_800E0C00[idx] != D_800E0C00[idx2])) {
                SCORE->unk46[idx]++;
                if (D_8008FDBC & 0x200) {
                    func_15085710(D_800E0C00[idx] != 0, 10, 1);
                }
            }
        }
        if (idx2 < D_8008FD90) {
            SCORE->rec[idx2].unk8++;
        }
        if ((player != arg1) && (arg1->unk31C != NULL) && (player->health != 0)) {
            sound = 0;
            delay = 0;
            player->unk31C->pad98[0x17] = 0;
            count = 0;
            if (arg2 == 0x33) {
                v = arg1->unk31C->unk78;
                if ((v == 0x21) || (v == 0x22)) {
                    func_10010630(0x510, arg1, 0x61A8, 0x12C, 0x190);
                }
            } else if ((player->id == 0x75) && (arg1->health <= 0)) {
                if (player->id == arg1->id) {
                    if ((idx2 < D_8008FD90) && (SCORE->rec[idx2].unkA >= 4)) {
                        sound = 0x665;
                        count = 4;
                    }
                } else if (arg2 == 0x34) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52A;
                    }
                } else {
                    sound = 0x4D1;
                    count = 3;
                }
                if (sound != 0) {
                    if (count != 0) {
                        sound = func_1000F568(sound, count);
                    }
                    func_10010558(sound, player, 0x6D60, 0x320, 0xBB8, delay);
                }
            } else if (player->id == 0x80) {
                state = -1;
                if ((arg0 != player) && (arg0->interaction_state == 4)) {
                    state = (u16)arg0->unk278;
                }
                if (player->id == arg1->id) {
                    if ((idx2 < D_8008FD90) && (SCORE->rec[idx2].unkA >= 4)) {
                        count = 8;
                        sound = 0x65A;
                    }
                } else if (state == 2) {
                    if ((arg1->health <= 0) || ((func_150ADA20() & 7) == 0)) {
                        count = 3;
                        sound = 0x571;
                    }
                } else if (*((u8 *)arg1->unk31C + 0x1AC) != 0) {
                    if ((arg2 != 0x21) && ((arg1->health <= 0) || ((func_150ADA20() & 3) == 3))) {
                        count = 4;
                        sound = 0x582;
                        delay = 0x1E;
                    }
                } else if (arg2 == 0x2E) {
                    delay = 0x1E;
                    if ((player->unk31C->unk78 == 0x23) && ((func_150ADA20() & 3) == 0)) {
                        sound = 0x587;
                    } else {
                        sound = 0x579;
                        count = 3;
                    }
                } else if (arg2 == 0x2F) {
                    if (func_150ADA20() & 1) {
                        sound = 0x588;
                        delay = 0x1E;
                    }
                } else if (arg2 == 0x2C) {
                    sound = 0x56D;
                    count = 7;
                    delay = 0x3C;
                } else if (((arg2 == 0x21) || (arg2 == 0x20) || (arg2 == 0x31)) &&
                           ((arg1->health <= 0) || ((func_150ADA20() & 7) == 3))) {
                    sound = 0x571;
                    count = 3;
                    delay = 0x1E;
                }
                if (sound != 0) {
                    if (count != 0) {
                        sound = func_1000F568(sound, count);
                    }
                    func_10010558(sound, player, 0x7D00, 0x258, 0x7D0, delay);
                }
            } else if (player->id == 0x3B) {
                state2 = -1;
                if ((arg0 != player) && (arg0->interaction_state == 4)) {
                    state2 = (u16)arg0->unk278;
                }
                if (state2 == 2) {
                    if ((arg1->health <= 0) || ((func_150ADA20() & 7) == 7)) {
                        sound = 0x538;
                        count = 3;
                    }
                } else if (arg2 == 0x2E) {
                    sound = 0x52F;
                    count = 3;
                    delay = 0x1E;
                } else if ((arg2 == 0x2C) || (arg2 == 0x2F)) {
                    sound = 0x540;
                    count = 2;
                    delay = 0x3C;
                } else if ((arg2 == 0x25) || (arg2 == 0x26)) {
                    h = arg1->health;
                    if (idx & 1) {
                        if (h >= 2) {
                            sound = 0x55F - h;
                            delay = 0x1E;
                        }
                    } else {
                        sound = 0x565 - h;
                        delay = 0x1E;
                    }
                } else if (arg2 == 0x21) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52C;
                        delay = 0x1E;
                    }
                } else if (arg2 == 0x20) {
                    r = func_150ADA20() & 0xF;
                    if (r != 0) {
                        r++;
                    }
                    if (r < 4) {
                        sound = r + 0x52B;
                        delay = 0x1E;
                    }
                }
                if (sound != 0) {
                    if (count != 0) {
                        sound = func_1000F568(sound, count);
                    }
                    func_10010558(sound, player, 0x7D00, 0x258, 0x7D0, delay);
                }
            } else if ((player->id == 0x88) && (arg1->health <= 0)) {
                if (arg2 == 0x34) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52A;
                    }
                } else if (arg2 == 0x21) {
                    sound = 0x41A;
                } else {
                    sound = 0x415;
                    delay = 0x1E;
                    count = 5;
                }
                if (sound != 0) {
                    if (count != 0) {
                        sound = func_1000F568(sound, count);
                    }
                    func_10010558(sound, player, 0x6D60, 0x320, 0xBB8, delay);
                }
            }
        }
    }
}

extern s8 D_800E0BEA;
extern s8 D_800E0BD3;
extern s32 D_800E0BD4;
extern s32 D_8008FDD8;
extern u8 **D_800E0B88;
extern void func_10004074(void *);
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1502B5C8();

void func_151E6964(s32 arg0) {
    u8 *p;
    u8 *end;
    s32 i;
    s32 size;

    if (D_800E0BEA != D_800BEAAB) {
        D_800BEAAB = D_800E0BEA;
        D_800E0BD3 = 0;
    }
    if (arg0 == D_800E0BD3) {
        return;
    }
    if (D_800E0BD4 != 0) {
        func_10004074((void *)D_800E0BD4);
        D_800E0BD4 = 0;
        if (D_8008FDD8 != 0) {
            func_10004074((void *)D_8008FDD8);
            func_10004074(D_800E0B88);
            func_10004074(D_800E0BD8);
            D_8008FDD8 = 0;
        }
    }
    D_800E0BD3 = arg0;
    if (D_800E0BD3 == 0) {
        return;
    }

    D_800E0BD4 = func_1502B5C8(&size, 3, 0x1C, D_800BEAAB, arg0 - 1);
    p = (u8 *)D_800E0BD4;
    end = p + size;
    i = 0;
    do {
        while (*p != 0) {
            p++;
        }
        while (*p == 0) {
            p++;
        }
        i++;
    } while (p < end);

    D_800E0BD8 = allocate_memory((i + 1) << 2, 1, 0, 0);
    p = (u8 *)D_800E0BD4;
    end = p + size;
    i = 0;
    do {
        D_800E0BD8[i] = p;
        i++;
        while (*p != 0) {
            p++;
        }
        while (*p == 0) {
            p++;
        }
    } while (p < end);

    if (arg0 - 1 != 0) {
        return;
    }

    D_8008FDD8 = func_1502B5C8(&size, 3, 0x1C, D_800BEAAB, 2);
    D_800E0B88 = allocate_memory(0x140, 1, 0, 0);
    p = (u8 *)D_8008FDD8;
    i = 0;
    do {
        D_800E0B88[i] = p;
        i++;
        while (*p != 0) {
            p++;
        }
        while (*p == 0) {
            p++;
        }
    } while (i != 80);
}

void func_151E6BFC(void) {
    D_800E0BD3 = 0;
    D_800E0BD4 = 0;
    D_8008FDD8 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E6C1C.s")

struct Struct151149AC {
    char pad0[0x80];
    s32 unk80;
};
extern struct Struct151E5034 *D_8008FDD4;
extern u8 D_8008FFB0[2][6];
extern f32 D_800ABA8C;
struct Struct151149AC *func_151149AC(s32);

void func_151E7DC0(void) {
    struct Struct151E5034 *sp;
    struct Struct151149AC *r;
    s32 i;
    s32 a;

    sp = D_8008FDD4;
    if (sp->unk8 < D_800ABA8C && sp->unk3E == 0) {
        return;
    }
    for (i = 0; i != 2; i++) {
        sp = D_8008FDD4;
        if (sp->unk3E == 0) {
            a = D_8008FFB0[i][sp->unk2C];
        } else {
            a = D_8008FFB0[i][0];
        }
        r = func_151149AC(a);
        if (r != 0) {
            r->unk80 = 2;
        }
    }
}

extern s8 D_800E0BE9;

void func_151E7E9C(void) {
    if (D_800E0BE9 == 2) {
        func_10017870(1);
    } else if (D_800E0BE9 == 0) {
        func_10017870(2);
    } else {
        func_10017870(4);
    }
}

extern void func_151DDC20(void);

void func_151E7EF8(void) {
    s32 *p;
    s32 *end;
    s32 sum;

    func_151E7E9C();
    sum = 0;
    p = (s32 *)func_151DDC20;
    end = (s32 *)func_151DE7D4;
    for (; p < end; p++) {
        sum += *p;
    }
    if (sum != (s32)0xBFC924E3) {
        *(s32 *)osSpTaskLoad = 0;
    }
}

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 padE[0x22];
} Spawn151E7F60;


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 pad7;
} Pos151E7F60;

extern struct127 *D_800E0BA0[];
extern Entry151E7F60 D_800AB57C[];
extern Pos151E7F60 D_800AB940[];
extern s32 func_15082A44(void *, s32, s32, s32, s32);
extern void func_15083384(struct127 *, u8);

void func_151E7F60(s32 arg0, s32 arg1) {
    s32 slot;
    s32 id;
    Spawn151E7F60 *spawn;
    s32 i;
    s32 kind;
    struct127 *obj;

    if (D_800E0BA0[arg0] != NULL) {
        func_15060F28(D_800E0BA0[arg0], 1);
    }
    id = func_15083E0C(arg0 + 0x10);
    spawn = (Spawn151E7F60 *)((id * 0x30) + (s32)D_800D20FC);
    kind = D_800AB57C[arg1].unk5;
    i = arg0;
    if (D_8008FDD4->unk2C == 7) {
        i++;
    }
    spawn->unk4 = kind;
    spawn->unk6 = D_800AB940[i].unk0;
    spawn->unk8 = D_800AB940[i].unk2;
    spawn->unk8 += D_800AB57C[arg1].unk4;
    spawn->unkA = D_800AB940[i].unk4;
    spawn->unkC = D_800AB940[i].unk6;
    if (kind == 0x53) {
        spawn->unkD = 0x24;
    } else {
        spawn->unkD = 0xE;
    }
    slot = func_15082A44(spawn, id, 0, 0, 0);
    if (slot != 0) {
        obj = &D_800CC2D0[slot - 1];
        D_800E0BA0[arg0] = obj;
        obj->unk5 = 7;
        obj->unk2F8 |= 3;
        obj->xz_scale = obj->y_scale = D_800AB57C[arg1].unk0;
        if (obj->id == 0 || obj->id == 0x80) {
            obj->unk31C = allocate_memory(0x1C0, 1, 0, 0);
            bzero(obj->unk31C, 0x1C0);
        }
        if (kind == 0x3B) {
            *(s8 *)&obj->pad68 = arg0 + 1;
        }
        func_15083384(obj, D_800AB57C[arg1].unk6);
        func_1505E650(obj, 0xF, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_20AE20/func_151E81EC.s")

void func_151E8214(void) {
    if (D_800E0B94 != 8) {
        if (func_1517EFDC() == 0) {
            D_800E0A90 = 0;
        }
        if (D_800E0A90 >= 0xA1) {
            D_8008FDCC = 0;
            D_800E0B94 = 8;
            D_8008FD8C = 1;
            D_8008FD90 = 1;
            D_8008FDA4 = 0;
            D_800E0A80 = -2;
            D_800E0A90 = 0;
            D_800D2E43 = 1;
        }
    }
}

void func_151E82B8(void) {
    func_151E530C();
    if (D_800E0A80 == -1) {
        if (D_800E0A90 >= 0x79) {
            D_800E0B94 = 9;
            D_800E0A90 = 0;
            D_8008FDCC = 0xFF;
            D_800E0A80 = 0;
            func_1501C730(6, 0x1D, 0, 0, 1);
            return;
        }
    }
    if (D_800E0A80 == -2) {
        D_800E0A80 = 0;
    }

    if (D_800E0A90 >= 0x1BE) {
        if (D_800E0A80 >= 0) {
            while (D_800E0BD8[D_800E0A80][0] != 0x2A) {
                D_800E0A80++;
            }
            D_800E0A80++;
            if (D_800E0BD8[D_800E0A80][0] == 0x3D) {
                D_800E0A80 = -1;
            }
            D_800E0A90 = 0;
        }
    }
}

void func_151E83E8(void) {
    if (D_800E0A80 == 0) {
        D_800E0A80 = -1;
        func_1501D348(0x1D, 6, 0, 0, 0);
    }
    func_151E530C();
    if (func_1517EFDC() == 0) {
        D_800E0A90 = 0;
    }
    if (D_800E0A90 >= 0x65) {
        func_151E5034();
        D_8008FDA4 = 0;
        D_800E0B94 = 1;
        D_800E0A90 = 0;
        D_800D2E40 = 0;
        func_1501C730(6, 0x21, 0, 0, 1);
    }
}
