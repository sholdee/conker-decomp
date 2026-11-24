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
    u16 unk22[4];
    s8 unk2A;
    s8 unk2B;
    s8 unk2C;
    char pad2D;
    s8 unk2E[4];
    s8 unk32[4];
    s8 unk36[4];
    s8 unk3A[4];
    s8 unk3E;
    s8 unk3F;
    s8 unk40;
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
s32 func_1509CF28(s32, s32 *);
void func_1509D054(void);
s32 func_1509D08C(s32, s32, s32 *, s32 *);
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

extern s8 D_800E0BE0[];
extern s8 D_8008FE54[];
extern s8 D_8008FE6C[];
extern s8 D_8008FE84[];
void func_151DD9E4(void);
s32 func_151DDB94(s32 arg0);
void func_151DDBA0(void);
void func_151DE6D4(void);

extern s32 D_800E0A90;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern u8 D_800E0A8C;
extern u8 D_8008FE28;
void func_151DE85C(void);
void func_151DE7D4(void);

extern s8 D_8008FE30;
extern s8 D_8008FD74;
extern s8 D_800E0B96;
void func_151DE81C(void);

extern u8 D_800D2E40;
extern u8 D_800E0B94;
extern u8 D_8008FD80;
extern u8 D_8008FDA4;
extern struct Struct151E5034 *D_8008FDD4;
void func_1501C730(s32, s32, s32, s32, s32);
void func_151DE85C(void);
void func_151DE8E8(void);



extern s8 D_8008FDDC;
extern s8 D_8008FE44[];
extern u8 D_800E0A94;
extern s8 D_8008FEF8;
extern s8 D_8008FE40;
extern s8 D_8008FE2C;
extern u8 D_8008FE34;
extern f32 D_8008FDE8;

typedef struct {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 errnum;
    u8 pad;
} Pad151DE8F0;

extern Pad151DE8F0 D_800BE918[];
extern s32 D_8008FE24;
extern s8 D_8008FD84;
/*
 * D_8008FEFC is a function-scope `static s16 timer = 240;` of func_151DE8F0 in the
 * original source (uopt keeps it in a register and writes it back at each region exit),
 * but game .data is not linked from C. Each write-back is spelled through a different
 * neighbouring data symbol (all == &D_8008FEFC) so IDO neither hoists the address nor
 * drops the repeated stores. Restore the static once the file's .data is linked.
 */
extern s8 D_8008FEF0;
extern s8 D_8008FEF4;
extern u8 D_8008FF08[];
#define TIMER_ALIAS(sym, off) (*(s16 *)((u8 *)&(sym) + (off)))
extern f32 D_8008FE38;
extern f32 D_8008FE3C;
extern s8 D_8008FDE4;
void func_151EDF4C(void);
void func_15007778(void);
void func_151E81EC(void);
void func_1000E7A0(s32, s32);
void func_151DF574(void);
void func_151E7DC0(void);
void func_151DF1BC(void);
void func_151DFF38(void);
void func_151E0424(void);
void func_151E09DC(void);
void func_151E2284(void);
void func_151E0B70(void);
void func_151E1744(void);
void func_151E1214(void);

void func_151DE8F0(void) {
    static s16 D_8008FEFC = 240;
    s32 i;
    s32 j;
    s16 timer;
    Pad151DE8F0 *pad;

    if (D_8008FDA4 < 5) {
        if (D_8008FDA4 == 0) {
            func_151EDF4C();
            func_15007778();
            func_151E7DC0();
            func_151E81EC();
        }
        if (D_8008FDA4 < 4) {
            func_150076BC(D_8008FDA4);
        } else {
            func_15007684();
        }
        D_8008FDA4++;
    } else {
    lbl1:
        if (0) goto lbl1; /* dead goto: splits the block so as1 xbb skips a cross-branch hoist */
        if (D_8008FE30 == 0) {
            func_1500764C();
        }
    }
    if (D_8008FD80) {
    lbl2:
        D_800DBFF0->unk2C = 0x80000;
        func_1501D348(D_800BE9F0, 1, 0, 0, 0);
        D_8008FD80 = 0;
        return;
    }
    if (D_8008FD70) {
        for (j = D_8008FD70; j < 16 - D_8008FD70; j++) {
            i = j + D_8008FD70;
            D_8008FE44[j] = D_8008FE44[i];
            D_800E0C00[j] = D_800E0C00[i];
        }
        D_8008FD70 = 0;
    }
    if (D_8008FDA4 >= 5) {
        func_151E530C();
    }
    for (i = 0; i != 4; i++) {
        D_8008FDD4->unk22[i] = 0;
        if (*(u8 *)&D_800E0B96 == 0) {
            pad = &D_800BE918[i];
            if (D_8008FDD4->unk32[i] > -50 && pad->stick_y <= -50) {
                D_8008FDD4->unk22[i] |= 2;
            }
            if (D_8008FDD4->unk32[i] < 50 && pad->stick_y >= 50) {
                D_8008FDD4->unk22[i] |= 1;
            }
            if (D_8008FDD4->unk2E[i] > -50 && pad->stick_x <= -50) {
                D_8008FDD4->unk22[i] |= 4;
            }
            if (D_8008FDD4->unk2E[i] < 50 && pad->stick_x >= 50) {
                D_8008FDD4->unk22[i] |= 8;
            }
            if ((&D_800BE930)[i] & 0x1000) {
                D_8008FDD4->unk22[i] |= 0x10;
            }
            if ((&D_800BE930)[i] & 0x8000) {
                D_8008FDD4->unk22[i] |= 0x10;
            }
            if ((&D_800BE930)[i] & 0x4000) {
                D_8008FDD4->unk22[i] |= 0x20;
            }
            if ((&D_800BE930)[i] & 2) {
                D_8008FDD4->unk22[i] |= 0x40;
            }
            if ((&D_800BE930)[i] & 1) {
                D_8008FDD4->unk22[i] |= 0x80;
            }
            D_8008FDD4->unk32[i] = pad->stick_y;
            D_8008FDD4->unk2E[i] = pad->stick_x;
        } else {
        lblQ:
            if (0) goto lblQ; /* same as1 xbb fence as above */
            D_8008FDD4->unk2E[i] = 0;
            D_8008FDD4->unk32[i] = 0;
        }
        if (D_8008FDD4->unk2E[i] > -50 && D_8008FDD4->unk2E[i] < 50) {
            D_8008FDD4->unk36[i] = 0;
            *(s8 *)((u8 *)&D_8008FDE0 - 4) = D_8008FDDC & ~(1 << i);
        } else if (!(D_8008FDDC & (1 << i))) {
            D_8008FDD4->unk36[i] += D_800BE9E4;
            if (D_8008FDD4->unk36[i] > 50) {
                D_8008FDD4->unk36[i] = 40;
                D_8008FDD4->unk2E[i] = 0;
            }
        }
        if (D_8008FDD4->unk32[i] > -50 && D_8008FDD4->unk32[i] < 50) {
            D_8008FDD4->unk3A[i] = 0;
        } else {
            D_8008FDD4->unk3A[i] += D_800BE9E4;
            if (D_8008FDD4->unk3A[i] > 50) {
                D_8008FDD4->unk3A[i] = 40;
                D_8008FDD4->unk32[i] = 0;
            }
        }
    }
    D_8008FDD4->unk20 = D_8008FDD4->unk22[0];
    if (D_800C35EA == 1) {
    lbl4:
        D_8008FE24 = 0;
        if (D_800E0A90 < 0x546 && D_800BE930 != 0) {
            D_8008FD84 = 1;
        }
        return;
    }
lbl5:
    if (D_8008FEFC > 0) {

        D_8008FEFC -= D_800BE9E4;
        if (D_8008FEFC > 60) {
            s32 alpha;

            if (D_8008FEFC > 180) {
                alpha = 0xFF - ((D_8008FEFC - 180) << 3);
                if (alpha < 0) {
                    alpha = 0;
                }
            } else {
                alpha = (D_8008FEFC - 60) << 3;
                if (alpha > 0xFF) {
                    alpha = 0xFF;
                }
                if (alpha == 0xFF) {
                    if (D_8008FEFC > 120) {
                        D_8008FEFC = 140;
                    }
                }
                if (D_8008FDD4->unk20 & 0x10) {
                    D_8008FEFC = 120;
                }
            }
            func_1504332C(0xFF, 0x32, 0x32, alpha - (alpha >> 2));
            func_15042D94(0x94, 0xC0, 0x81, (char *)D_800E0BD8[0x8F]);
        }
    }
    D_800DBFF0->unk2C = 0x80000;
    func_1000E7A0(4, D_8008FDD4->unk3E);
    if (D_8008FDD4 == NULL || D_8008FDA4 < 5) {
        return;
    }
    D_8008FDD4->unk40 = D_8008FDD4->unk3F;
    {
    s32 v;

    v = (D_8008FDD4->unk8 - 0.5f) * 512.0f;
    if (v < 0) {
        v = 0;
    } else if (v > 0xFF) {
        v = 0xFF;
    }
    D_800E0A94 = v;
    }
    if (D_8008FDD4->unk8 >= (f32)0.5) {
        if (D_8008FDD4->unk8 < 1.0f) {
            D_8008FDD4->unk20 &= 0xFFEF;
        }
        switch (D_8008FDD4->unk3E) {
            case 0:
                if (D_8008FDD4->unk2C != 1) {
                    D_8008FEF8 = 0;
                }
                switch (D_8008FDD4->unk2C) {
                    case 1:
                        func_151DF1BC();
                        break;
                    case 2:
                        func_151DF574();
                        break;
                    case 3:
                    case 4:
                    case 5:
                        func_151DFF38();
                        break;
                    case 6:
                        break;
                }
                break;
            case 1:
                D_8008FDA8 = -1;
                switch (D_8008FDD4->unk3F) {
                    case 0:
                        func_151E0424();
                        break;
                    case 2:
                        func_151E09DC();
                        break;
                    case 3:
                        func_151E2284();
                        break;
                    case 4:
                        func_151E0B70();
                        break;
                    case 1:
                        func_151E1744();
                        break;
                    case 5:
                        func_151E1214();
                        break;
                }
                break;
        }
    } else {
        D_800E0A96 = -1;
        D_800E0A95 = 0xFF;
        D_8008FDD4->unk3F = 0;
        D_8008FDD4->unk41 = 0;
        D_8008FE40 = 1;
        D_8008FE44[0] = 0;
        D_8008FE2C = 0;
        for (i = 1; i < 16; i++) {
            D_8008FE44[i] = -1;
        }
        for (i = 0; i < 16; i++) {
            D_800E0C00[i] = 0;
        }
        D_8008FE34 = 0;
        D_8008FE38 = 0.0f;
        D_8008FE3C = 0.0f;
        D_8008FDE8 = 0.0f;
        D_8008FDE4 = 0;
    }
}

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


extern s8 D_8008FDB0;
typedef struct { u8 r; u8 g; u8 b; } Rgb151DF574;
extern u8 D_8008FF08[];
extern s8 D_800AB804[];
extern char D_800AB810[];
extern s8 D_800BE91B;
extern u8 **D_800E0B88;
extern struct Struct151E5034 *D_8008FDD4;
extern s8 D_8008FD74;
void func_151E30C4(void);

void func_151DF574(void) {
    s32 c0;
    s32 redo;
    s32 i;
    s32 j;
    s32 count;
    s32 mask;
    s32 offset;
    s32 alpha;
    s32 y;
    s32 h;
    s32 t;
    s32 a;
    s32 p;
    s8 row;
    s32 sfx;
    s8 prev;
    s32 c1;
    s32 sel;
    s32 scale;
    s32 c2;
    s8 list[20];
    static s8 D_8008FF24 = 0;
    static s8 D_8008FF28 = -1;
    static s8 D_8008FF2C = 1;
    static u8 D_8008FF30 = 0;

    sfx = 0;
    scale = D_800E0A94;
    do {
        redo = 0;
        prev = D_8008FF00;
        if (D_800E0A96 == 1) {
            D_8008FF00--;
            if (D_8008FF00 < 0) {
                D_8008FF00 = 8;
            }
            D_8008FF2C = -1;
        } else if (D_800E0A96 == 2) {
            D_8008FF00++;
            if (D_8008FF00 >= 9) {
                D_8008FF00 = 0;
            }
            D_8008FF2C = 1;
        }
        do {
            offset = 1;
            i = 0;
            mask = 0;
            count = 0;
            for (; i < D_8008FF00; i++) {
                offset += D_800AB804[i];
            }
            if (D_800E9D00 & 0x8000) {
                mask = -1;
            }
            if ((D_8008FF00 == 8) && (D_800E9D00 & 2)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 7) && (D_800E9D00 & 4)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 6) && (D_800E9D00 & 0x1000)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 5) && (D_800E9D00 & 0x4000)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 4) && (D_800E9D00 & 0x01000000)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 3) && (D_800E9D00 & 0x02000000)) {
                mask |= 1;
            }
            if ((D_8008FF00 == 2) && (D_800E9D00 & 0x04000000)) {
                mask |= 1;
            }
            a = func_1509CF28(D_8008FF00, &i);
            if (D_8008FF00 == 0) {
                mask |= 1;
            }
            if (a) {
                for (i = 0; i < D_800AB804[D_8008FF00] - 1; i++) {
                    if (func_1509D08C(D_8008FF00, i, &j, NULL)) {
                        mask |= 1 << i;
                    }
                    if ((1 << i) & mask) {
                        count++;
                    }
                }
                func_1509D054();
                if ((D_8008FF00 == 8) && (D_800BE3DE != 0) && !(mask & 8)) {
                    mask |= 8;
                    count++;
                }
            } else {
                mask |= 1 << i;
                count = 1;
            }
            if (count == 0) {
                D_8008FF00 += D_8008FF2C;
                if (D_8008FF00 >= 9) {
                    D_8008FF00 = 0;
                }
                if (D_8008FF00 < 0) {
                    D_8008FF00 = 8;
                }
            }
        } while (count == 0);
        if ((D_800E0A96 == 1) || (D_800E0A96 == 2)) {
            if (prev == D_8008FF00) {
                D_800E0A96 = -1;
            } else {
                D_800E0A96 += 2;
                D_8008FF00 = prev;
            }
            redo = 1;
        }
    } while (redo);

    if (D_800E0A96 > 0) {
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            alpha = 0;
            if (D_800E0A96 == 3) {
                D_800E0A96 = -1;
                D_8008FF00--;
                if (D_8008FF00 < 0) {
                    D_8008FF00 = 8;
                }
                D_8008FF2C = -1;
            } else {
                D_800E0A96 = -1;
                D_8008FF00++;
                if (D_8008FF00 >= 9) {
                    D_8008FF00 = 0;
                }
                D_8008FF2C = 1;
            }
        }
    } else {
        alpha = D_800E0A95 + (D_800BE9E4 << 5);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }
    D_800E0A95 = alpha;
    alpha = (alpha * scale) >> 8;
    if (alpha >= 0xFE) {
        alpha = 0xFF;
    }
    if (D_8008FF24 > 0) {
        D_8008FF24 -= 2;
    } else if (D_8008FF24 < 0) {
        D_8008FF24 += 2;
    }
    if ((D_8008FF24 == 0) && (D_800E0A95 == 0xFF)) {
        if (D_800BE91B < -49) {
            D_8008FF28 = D_8008FDB0;
            D_8008FDB0++;
            D_8008FF24 = -10;
            sfx = 0x4FF;
        } else if (D_800BE91B >= 50) {
            D_8008FF28 = D_8008FDB0;
            D_8008FDB0--;
            D_8008FF24 = 10;
            sfx = 0x4FF;
        }
    }
    if (D_8008FDB0 < 0) {
        D_8008FDB0 = 0;
        D_8008FF24 = 0;
        sfx = 0;
    }
    if (D_8008FDB0 >= count) {
        D_8008FDB0 = count - 1;
        D_8008FF24 = 0;
        sfx = 0;
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    func_15042D94(0x94, 0x4D, 0x81, D_800AB810, D_8008FF00 + 0x42);
    for (j = 0, i = 0; i < count; i++) {
        while (!((1 << j) & mask)) {
            j++;
        }
        list[i] = j++;
    }
draw_rows:
    sel = D_8008FF00 + D_8008FF00 + D_8008FF00;
    func_1504332C(0xFF, 0, 0, alpha);
    if (count < 6) {
        y = 0x2F;
        h = count * 11;
    } else {
        y = -(D_8008FDB0 * 11) - D_8008FF24 + 0x45;
        h = 0x37;
        if (y >= 0x30) {
            y = 0x2F;
        }
    }
    row = -1;
    i = count - 3;
    for (j = -3; j < count + 3; j++) {
        t = D_8008FF24 * 12;
        if (t < 0) {
            t = -t;
        }
        if (i == D_8008FDB0) {
            t = 0xFF - t;
            if (t == 0xFF) {
                p = (D_8008FF30 * 2) & 0x7F;
                if (p >= 0x40) {
                    p = 0x7F - p;
                }
                t = p + 0xC0;
                D_8008FF30 += D_800BE9E4;
            } else {
                D_8008FF30 = 0x1F;
            }
        } else if (D_8008FF28 == i) {
            t += 0x80;
        } else {
            t = 0x80;
        }
        if (i == D_8008FDB0) {
            D_8008FF04 = list[i];
        }
        if (y < 0x50) {
            a = y * 25 - 0x6D6;
        } else if ((h + 0x46 < y) && (y < h + 0x50)) {
            a = (10 - (y - h)) * 25 + 0x6D6;
        } else {
            a = 0xFF;
        }
        if (a < 0) {
            a = 0;
        }
        if (a >= 0xFA) {
            a = 0xFF;
        }
        if (alpha < a) {
            a = alpha;
        }
        c0 = (D_8008FF08[sel] * t) >> 8;
        c1 = (D_8008FF08[sel + 1] * t) >> 8;
        c2 = (D_8008FF08[sel + 2] * t) >> 8;
        func_1504332C(c0, c1, c2, a);
        if ((y >= 0x46) && (y < h + 0x50) && (row == 0)) {
            func_15042D94(0x94, y + 0xF, 0x81, (char *)(&D_800E0B88[list[i]])[offset]);
        }
        y += 11;
        i++;
        if (i >= count) {
            row++;
            i = 0;
        }
    }
    if (D_800E0A95 == 0xFF) {
        if (D_8008FDD4->unk20 & 4) {
            D_800E0A96 = 1;
            sfx = 0x4FF;
        } else if (D_8008FDD4->unk20 & 8) {
            D_800E0A96 = 2;
            sfx = 0x4FF;
        }
        if ((D_8008FDD4->unk20 & 0x10) && (D_8008FF04 != 0xFF)) {
            D_800E0B94 = 7;
            D_800E0A88 = func_151E30C4;
            *(u8 *)&D_8008FD74 = 3;
            sfx = 0x500;
        }
    }
    if (sfx != 0) {
        func_10010F30(sfx, 0x4650, 0x40, 0, 0);
    }
}

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

extern s8 D_800AB68C[];
extern char D_800AB844[];
extern char D_800AB848[];
extern char D_800AB84C[];
extern s8 D_8008FE9C[];
extern s8 D_8008FEC4[];
extern s8 D_800E0B90[];
extern u8 D_800E0B8C;
extern s8 D_8008FE40;
extern s8 D_8008FDA0;
extern s8 D_8008FDC4;
extern s8 D_8008FDDC;
void func_151E22BC(void);

void func_151E0424(void) {
    s32 alpha;
    s32 count;
    s32 sum;
    s32 sel;
    s32 i;
    s32 x;
    s32 v;
    s32 alpha2;
    s32 flags;
    f32 step;
    s8 t;

    alpha2 = D_800E0A94;
    if (D_8008FDD4->unk2C != 0) {
        alpha = alpha2;
        if (D_800E0A96 != -1) {
            D_8008FDD4->unk20 &= 0xFFDF;
            i = D_800E0A95 - (D_800BE9E4 << 5);
            if (i <= 0) {
                D_800E0A95 = 0;
                D_8008FDD4->unk3F = D_800E0A96;
                if (D_800E0A96 == 2) {
                    D_8008FDE0 = 0;
                    D_8008FDDC = 1;
                    D_8008FE34 = 0;
                }
                func_151E22BC();
                D_800E0A96 = -1;
                return;
            }
            D_800E0A95 = i;
            alpha2 = D_800E0A95;
            if (D_800E0A96 != 2) {
                alpha = alpha2;
            }
        } else {
            i = D_800E0A95 + (D_800BE9E4 << 5);
            if (i >= 0x100) {
                i = 0xFF;
            }
            D_800E0A95 = i;
            alpha2 = (alpha2 * i) >> 8;
            if (alpha2 >= 0xFE) {
                alpha2 = 0xFF;
            }
        }
        count = D_8008FE9C[D_8008FDD4->unk2C - 1];
        D_8008FDA0 = 0;
        D_8008FDC4 = 0;
        D_800E0B8C = 0;
        if (D_8008FDD4->unk20 & 0xC) {
            D_8008FE2C = 1 - D_8008FE2C;
            func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
        }
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(0x94, 0x7E, 1, D_800AB844, 0x1A);
        func_1504332C(0xFF, 0xFF, 0xFF, alpha2);
        func_15042D94(0xD5, 0x7E, 1, D_800AB848, (D_8008FE2C == 1) + 0x1B);
        if ((alpha2 == 0xFF) && (D_8008FE2C == 0)) {
            if ((count != 5) || ((count == 5) && (D_8008FDE8 == 0.0f))) {
                flags = D_8008FDD4->unk20;
                if (flags & 1) {
                    D_8008FDE8 = 1.0f;
                    D_8008FDD4->unk41--;
                    func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
                } else if (flags & 2) {
                    D_8008FDE8 = -1.0f;
                    D_8008FDD4->unk41++;
                    func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
                }
            }
        }
        v = D_8008FDD4->unk41;
        if (v < 0) {
            D_8008FDD4->unk41 = v + count;
            v = D_8008FDD4->unk41;
        }
        if (v >= count) {
            D_8008FDD4->unk41 = v - count;
            v = D_8008FDD4->unk41;
        }
        sel = v;
        sum = 0;
        for (i = 0; i < D_8008FDD4->unk2C - 1; i++) {
            sum += D_8008FE9C[i];
        }
        *(s8 *)&D_8008FDD4->pad42 = sel + sum;
        if (count != 5) {
            x = 0x8C - ((count * 0x1C) >> 1);
            for (i = 0; i < count; i++) {
                func_150432FC(0x53, x);
                v = *(s16 *)&D_800AB68C[(sum + i) * 10];
                if ((i == sel) && (D_8008FE2C == 0)) {
                    v++;
                }
                func_15042E3C(D_800AB84C, v);
                x += 0x1C;
            }
        } else {
            step = D_800BE9E4 * 0.05000000075f;
            if (D_8008FDE8 < 0.0f) {
                D_8008FDE8 += step;
                if (D_8008FDE8 > 0.0f) {
                    D_8008FDE8 = 0.0f;
                }
            } else if (D_8008FDE8 > 0.0f) {
                D_8008FDE8 -= step;
                if (D_8008FDE8 < 0.0f) {
                    D_8008FDE8 = 0.0f;
                }
            }
            if ((D_8008FE2C == 0) && (D_8008FDE8 != 0.0f)) {
                D_8008FDD4->unk20 &= 0xFFEF;
            }
        }
        if (alpha2 == 0xFF) {
            flags = D_8008FDD4->unk20;
            if (flags & 0x10) {
                D_8008FDD4->unk20 = flags & 0xFFDF;
                if (D_8008FE2C == 0) {
                    D_800E0A96 = 4;
                    D_800E0BE0[0] = D_8008FDD4->unk41;
                    t = D_800AB68C[*(s8 *)&D_8008FDD4->pad42 * 10 + 5];
                    if (t < 0) {
                        D_800E0B90[0] = D_8008FEC4[(-t - 1) << 2];
                    } else {
                        D_800E0B90[0] = 0;
                    }
                    D_8008FE40 = 1;
                } else {
                    D_800E0A96 = 2;
                    D_8008FDD4->unk20 &= 0xFFDF;
                }
            }
        }
    }
}

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

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
} Struct151E22BC;

extern s8 *D_80082BBC;

void func_151E22BC(void) {
    s32 idx;
    Struct151E22BC *p;

    idx = D_800AB690[*(s8 *)&D_8008FDD4->pad42 * 10];
    p = (Struct151E22BC *)(D_80082BBC + idx * 10);
    if (p->unk3 >= 5) {
        p->unk3 = 4;
    }
    if (p->unk3 < 0) {
        p->unk3 = 0;
    }
    if (p->unk1 <= 0) {
        p->unk1 = 1;
    }
    D_800E0BE0[2] = p->unk1;
    D_800E0BE0[3] = p->unk2;
    D_800E0BE0[11] = 0;
    D_800E0BE0[16] = p->unk3;
    D_800E0BE0[17] = p->unk4;
    switch (idx) {
        case 0:
            D_800E0BE0[4] = p->unk5;
            break;
        case 1:
        case 2:
            D_800E0BE0[13] = p->unk5;
            D_800E0BE0[14] = p->unk6;
            break;
        case 7:
            D_800E0BE0[18] = p->unk5;
            break;
        case 5:
            D_800E0BE0[17] = 0;
            D_800E0BE0[18] = p->unk5;
            break;
        case 8:
        case 9:
            D_800E0BE0[7] = p->unk5;
            D_800E0BE0[19] = p->unk6;
            D_800E0BE0[17] = 1;
            D_800E0BE0[3] = 2;
            break;
        default:
            break;
    }
    D_800E0BE0[22] = p->unk8;
    D_800E0BE0[6] = p->unk9;
    func_151DD9E4();
}

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
s32 func_151E24F0(s32 *arg0);

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
void func_151E2834(void);
void func_151E30C4(void);
void func_151E327C(void);
void func_151E4264(void);
void func_151E4314(void);

extern s8 D_8008FE2C;
extern u16 D_8008FDC0;
extern s8 D_800AB68C[];
extern struct Struct151E4314 D_800AB7BC[];
extern s8 D_800E0A86;
extern s8 D_800E0AC0[];
extern s8 D_800E0AC1;
extern s8 D_800E0AC2;
extern s8 D_800E0AC3;
void func_151E4BD8(void);

extern u8 D_800E0B94;
void func_151E4DC4(void);

extern s16 D_800E0B9A;
void func_151E4DD8(void);
void func_151E4E00(void);
void func_151E4E64(void);

extern f32 D_8008FDD0;
extern f32 D_800ABA2C;
extern f32 D_800ABA30;
extern f32 D_800ABA34;
extern f32 D_800DDDC8;
extern f32 D_800DDDD8;
void func_151E4EE8(void);

extern struct Struct151E5034 *D_8008FDD4;
void func_151E5034(void);
void func_151E51EC(void);
void func_151E530C(void);
void func_151E53E8(s32 arg0);

extern u8 D_80084060[];
void func_151E557C(void);
void func_151E55A8(void);

extern u8 D_800E0A8C;
void func_151E562C(void);

extern s8 D_8008FDC8;
s8 func_151E564C(void);

extern u8 D_800BE616;
extern u8 D_800E0B94;
extern s8 D_800E0C00[];
s32 func_151E5F64(s32 arg0);
s8 func_151E5FAC(void);

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
void func_151E5FF4(struct127 *arg0, struct127 *arg1, s32 arg2);

extern s8 D_800E0BEA;
extern s8 D_800E0BD3;
extern s32 D_800E0BD4;
extern s32 D_8008FDD8;
extern u8 **D_800E0B88;
extern void func_10004074(void *);
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1502B5C8();
void func_151E6964(s32 arg0);
void func_151E6BFC(void);

struct Struct151149AC {
    char pad0[0x80];
    s32 unk80;
};
extern struct Struct151E5034 *D_8008FDD4;
extern u8 D_8008FFB0[2][6];
extern f32 D_800ABA8C;
struct Struct151149AC *func_151149AC(s32);
void func_151E7DC0(void);

extern s8 D_800E0BE9;
void func_151E7E9C(void);

extern void func_151DDC20(void);
void func_151E7EF8(void);

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
void func_151E7F60(s32 arg0, s32 arg1);
void func_151E8214(void);
void func_151E82B8(void);
void func_151E83E8(void);
