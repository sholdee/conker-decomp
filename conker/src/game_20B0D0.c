#include <ultra64.h>
// matching: functions.h declares func_150ADA20 as returning u8, but this file uses its full 32-bit result
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

struct Struct151E5034 {
    f32 unk0;  /* progress of a step between path stops (func_151E6C1C) */
    f32 unk4;  /* its speed */
    f32 unk8;  /* progress along the current stop's path */
    f32 unkC;  /* its speed */
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u16 unk20; /* input bits read by the menus */
    char pad22[8];
    s8 unk2A;
    s8 unk2B; /* current path stop */
    s8 unk2C; /* displayed stop */
    s8 unk2D;
    char pad2E[0x10];
    s8 unk3E; /* which of the two paths is in use */
    s8 unk3F;
    char pad40;
    s8 unk41;
    s8 unk42;
    s8 unk43;
    s8 unk44;
    char pad45;
    s16 unk46[16];
    struct {
        s16 unk0;
        s16 unk2;
        s16 unk4;
        s16 unk6;
        s16 unk8;
        s16 unkA;
    } unk66[4];
};

extern OSContPad D_800BE918[];
extern u16 D_800BE930[]; /* buttons newly pressed, per controller */
extern OSIoMesg D_800388E0;
extern struct Struct151E5034 *D_8008FDD4;
extern void (*D_800E0A88)(void);

extern s8 D_8008FD74;
extern s32 D_8008FD78;
extern u8 D_8008FD80;
extern s8 D_8008FD98;
extern s8 D_8008FD9C;
extern u8 D_8008FDA4;
extern u8 D_8008FDB8;
extern u8 D_8008FDCD;
extern u8 D_8008FE28;
extern s8 D_8008FE2C;
extern s8 D_8008FE30;
extern s8 D_8008FEEC;
extern s8 D_8008FEF0;
extern u8 D_800BEAC3;
extern u8 D_800C35E8;
extern u8 D_800D2E40;
extern s32 D_800D2E48;
extern s8 D_800E0A85;
extern u8 D_800E0A8C;
extern s32 D_800E0A90;
extern u8 D_800E0A98;
extern s8 D_800E0A99;
extern u8 D_800E0B94;
extern u8 D_800E0B96;
extern u8 D_800E0B97;
extern u8 D_800E0B98;
extern s16 D_800E0B9A;
extern s8 D_800E0BE3;

extern char D_800AB7D4[];
extern char D_800AB7DC[];
extern char D_800AB7E0[];
extern char D_800AB7E4[];
extern char D_800AB7E8[];
extern char D_800AB7EC[];
extern char D_800AB7F0[];
extern char D_800AB7F4[];
extern char D_800AB7F8[];
extern char D_800AB7FC[];

void func_1501C730(s32, s32, s32, s32, s32);
void func_15042D94(s32, s32, s32, char *, ...);
void func_15042E3C(char *, s32);
s32 func_1517EFDC(void);
s32 func_151DDB94(s32 arg0);
void func_151DDBA0(void);
void func_151DE85C(void);
void func_151E2834(void);
void func_151E530C(void);

#define mode modeA[0]
void func_151DDC20(void) {
    static s8 D_8008FEF4;
    s8 sx;
    s8 sy;
    u8 skip;
    u8 modeA[1];
    s32 i;
    s32 y;
    s32 max;
    s32 sfx;
    s32 ypos;
    OSContPad *pad;
    s32 t;
    s32 x;
    u32 buf[8];
    u32 addr;

    s32 dummy;

    D_800E0B96 = 0;
    sfx = 0;
    skip = 0;
    mode = D_8008FDB8;
    if (D_800BE9F0 == 0x22) {
        mode = 1;
    }
    if ((D_800C35EA != 1) || (D_800BE9F0 != 0x18) || (D_800C35E8 != 1)) {
        if (func_1517EFDC() != 0) {
            skip = 1;
        }
    }
    if ((D_800C35EA == 1) && (D_800C3683 != 0)) {
        skip = 1;
    }
    if (D_8008FEEC > 0) {
        D_8008FEEC--;
    }
    if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0) && (skip == 0) && (D_8008FEEC == 0)) {
        for (i = 0; (i <= D_80082FA0) && (D_800BEAC1 == 0); i++) {
            if ((D_800BE930[i] & 0x1000) && !(D_800D18A0 & (1 << i))) {
                D_800BEAC1 = 1;
                D_800BEAC3 = 1;
                D_800E0A98 = i;
                D_8008FE2C = 0;
                D_8008FEF4 = D_800BE918[i].stick_y;
                D_8008FEEC = 10;
            }
        }
    }
    if ((D_800BEAC0 == 0) && (D_800D2E48 != 0xC7)) {
        D_800E0A99 += D_800BE9E4;
        if (D_800E0A99 >= 60) {
            D_800E0A99 -= 60;
            D_800BE3E8++;
        }
    }
    if ((D_800BE616 != 0) && (D_800BEAC1 == 0) && (D_800E0BE3 >= 3)) {
        x = 0xC6;
        if (D_80082FA0 > 0) x = 0x64;
        if (D_8008FD78 > 0) {
            D_8008FD78 = D_8008FD78 - D_800BEA08;
        } else {
            D_8008FD78 = 0;
        }
        t = D_8008FD78 / 60;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(0x87, x, 0x80, D_800AB7D4, t / 60, t % 60);
    }
    if (D_8008FEF0 > 0) {
        D_8008FEF0--;
        if (D_8008FEF0 < 2) {
            D_800BEAC1 = 0;
        }
    } else if (D_800BEAC0 != 0) {
        if (D_800BE616 != 0) {
            y = -0x24;
            max = 3;
        } else {
            y = 5;
            max = 2;
        }
        D_800E0B9A = D_800BE930[D_800E0A98];
        if (D_8008FEEC != 0) {
            D_800E0B9A = 0;
        }
        pad = &D_800BE918[D_800E0A98];
        sx = pad->stick_x;
        sy = pad->stick_y;
        if (D_8008FE2C < max) {
            if (((D_8008FEF4 < 0x1F) && (sx >= 0x1F)) || ((D_8008FEF4 >= -0x1E) && (sx < -0x1E))) {
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 0;
                } else {
                    D_8008FE2C = 1 - D_8008FE2C;
                }
                sfx = 0x62D;
            }
            if ((D_8008FE2C != 0) && (max == 3) && (((D_800E0A85 >= -0x1E) && (sy < -0x1E)) || ((D_800E0A85 < 0x1F) && (sy >= 0x1F)))) {
                sfx = 0x62D;
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 1;
                } else {
                    D_8008FE2C = 2;
                }
            }
        } else if (((D_8008FEF4 >= -0x1E) && (sx < -0x1E)) || ((D_8008FEF4 < 0x1F) && (sx >= 0x1F))) {
            D_8008FD98 = 1 - D_8008FD98;
            sfx = 0x62D;
        }
        D_8008FEF4 = sx & 0xFF;
        D_800E0A85 = sy;
        func_1504332C(0xFF, 0xFF, 0xFF, D_8008FDCD);
        if (mode != 0) {
            ypos = y + 0x73;
            D_8008FE2C = 0;
            func_15042D94(0x94, ypos, 1, D_800AB7DC, 0xD);
        } else {
            ypos = y + 0x73;
            func_15042D94(0x94, ypos, 1, D_800AB7E0, 0x1A);
            if (D_8008FE2C < max) {
                func_15042D94(0xD5, ypos, 1, D_800AB7E4, (D_8008FE2C == 0) + 0xC);
            }
            func_150432FC(0x94, y + 0x55);
            if (max == D_8008FE2C) {
                func_15042E3C(D_800AB7E8, 0x32);
            } else if (max < D_8008FE2C) {
                func_15042E3C(D_800AB7EC, 0x33);
            }
        }
        func_150432FC(0x53, ypos);
        if (D_8008FE2C < max) {
            if (mode == 0) {
                i = 1;
                if (max == 3) {
                    func_150432FC(0x53, y + 0x63);
                    func_15042E3C(D_800AB7F0, (D_8008FE2C == 1) + 0x21);
                    i = 2;
                    func_150432FC(0x53, y + 0x82);
                }
                func_15042E3C(D_800AB7F4, (i == D_8008FE2C) + 0xE);
            }
            if (D_800E0B9A & 0x1000) {
                D_8008FEF0 = 5;
                D_8008FEEC = 10;
            } else if ((D_8008FE2C == 0) || (D_800E0B9A & 0x4000)) {
                if (D_800E0B9A & 0xC000) {
                    D_8008FEF0 = 5;
                    D_8008FEEC = 10;
                }
            } else if (D_800E0B9A & 0x8000) {
                D_8008FD98 = 0;
                D_8008FE2C = D_8008FE2C + max - 1;
                if (max == 2) {
                    D_8008FE2C++;
                }
                sfx = 0x500;
            }
        } else {
            func_15042D94(0x6C, ypos, 1, D_800AB7F8, D_8008FD98 + 0x1F);
            func_15042D94(0xBC, ypos, 1, D_800AB7FC, 0x1E - D_8008FD98);
            if (D_800E0B9A & 0x8000) {
                if (D_8008FD98 != 0) {
                    D_800E0B94 = 7;
                    D_800E0A88 = func_151DDBA0;
                    D_8008FD74 = 8;
                    if (D_800BE616 != 0) {
                        D_8008FDD4->unk3F = 4;
                        if (max == D_8008FE2C) {
                            D_8008FD9C = 1;
                            D_800E0A88 = func_151E2834;
                        }
                    } else if (D_8008FDA8 >= 0) {
                        func_15007718(D_8008FDA8);
                    }
                    D_8008FE2C = 0;
                    D_80087270[0] = 0;
                } else {
                    u32 *p;

                    p = buf;
                    while ((s32)p & 0xF) {
                        p++;
                    }
                    addr = func_151DDB94(-0xF81);
                    osWritebackDCache(p, 0x10);
                    osPiStartDma(&D_800388E0, 0, 0, addr, p, 0x10, gMessageQueue);
                    osRecvMesg(gMessageQueue, NULL, 1);
                    osInvalDCache(p, 0x10);
                    p[0] -= 0x3E0EF0F0;
                    p[1] -= 0x3E0EF0F0;
                    if ((p[0] != 0x3050A14A) || (p[1] != 0xBCFCF506)) {
                        osPiStartDma(&D_800388E0, 0, 0, func_150ADA20() & 0xFFFF, (void *)((func_150ADA20() & 0xFFFFE) + 0x80300000), 0x100, gMessageQueue);
                        osRecvMesg(gMessageQueue, NULL, 1);
                    }
                    D_8008FE2C -= 2;
                }
                sfx = 0x500;
            } else if (D_800E0B9A & 0x4000) {
                D_8008FE2C -= 2;
            }
        }
        if (sfx != 0) {
            func_10010F30(sfx, 0x5DC0, 0x40, 0, 0);
        }
    }
}

void func_151DE6CC(void) {
}
#undef mode

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
        D_800E0B96 = 0xFF;
    }
}

void func_151DE7D4(void) {
    D_800E0A90 = 0;
    D_800E0B97 = 0;
    D_800E0B98 = 0;
    D_800E0A8C = 0;
    D_8008FE28 = 2;
    func_151DE85C();
}

void func_151DE81C(void) {
    D_8008FD74 = 4;
    D_800E0B96 = 0;
    if (D_8008FE30 == 0) {
        func_1500764C();
    }
}

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
