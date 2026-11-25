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
extern u16 D_800BE930[]; /* button bits per controller, read by the menus */
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


/*
 * Pause menu, run every frame: opens on START from an allowed controller, advances the play-time
 * counter, draws the countdown while D_800BE616 is set, and handles the menu cursor (D_8008FE2C) and the
 * yes/no confirmation (D_8008FD98).
 */
void func_151DDC20(void) {
    static s8 D_8008FEF4;
    s8 stickX;
    s8 stickY;
    u8 blocked;
    volatile u8 reducedMenu; // matching: IDO must keep this in its stack slot (a register copy mismatches)
    s32 i;
    s32 menuY;
    s32 numOptions;
    s32 sfx;
    s32 textY;
    OSContPad *cont;
    s32 seconds;
    s32 timerY;
    u32 dmaBuf[8];
    u32 romAddr;
    s32 pad; // matching: unused stack slot keeps the frame size

    D_800E0B96 = 0;
    sfx = 0;
    blocked = 0;
    reducedMenu = D_8008FDB8;
    if (D_800BE9F0 == 0x22) {
        reducedMenu = 1;
    }
    if ((D_800C35EA != 1) || (D_800BE9F0 != 0x18) || (D_800C35E8 != 1)) {
        if (func_1517EFDC() != 0) {
            blocked = 1;
        }
    }
    if ((D_800C35EA == 1) && (D_800C3683 != 0)) {
        blocked = 1;
    }
    if (D_8008FEEC > 0) {
        D_8008FEEC--;
    }
    if ((D_800BEAC0 == 0) && (D_800BEAC1 == 0) && (blocked == 0) && (D_8008FEEC == 0)) {
        for (i = 0; (i <= D_80082FA0) && (D_800BEAC1 == 0); i++) {
            if ((D_800BE930[i] & START_BUTTON) && !(D_800D18A0 & (1 << i))) {
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
        timerY = 198;
        if (D_80082FA0 > 0) timerY = 100; // matching: on one line (-g3 line order schedules the D_8008FD78 load)
        if (D_8008FD78 > 0) {
            D_8008FD78 -= D_800BEA08;
        } else {
            D_8008FD78 = 0;
        }
        seconds = D_8008FD78 / 60;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(135, timerY, 0x80, D_800AB7D4, seconds / 60, seconds % 60);
    }
    if (D_8008FEF0 > 0) {
        D_8008FEF0--;
        if (D_8008FEF0 < 2) {
            D_800BEAC1 = 0;
        }
    } else if (D_800BEAC0 != 0) {
        if (D_800BE616 != 0) {
            menuY = -36;
            numOptions = 3;
        } else {
            menuY = 5;
            numOptions = 2;
        }
        D_800E0B9A = D_800BE930[D_800E0A98];
        if (D_8008FEEC != 0) {
            D_800E0B9A = 0;
        }
        cont = &D_800BE918[D_800E0A98];
        stickX = cont->stick_x;
        stickY = cont->stick_y;
        if (D_8008FE2C < numOptions) {
            if (((D_8008FEF4 < 31) && (stickX >= 31)) || ((D_8008FEF4 >= -30) && (stickX < -30))) {
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 0;
                } else {
                    D_8008FE2C = 1 - D_8008FE2C;
                }
                sfx = 0x62D; /* cursor move */
            }
            if ((D_8008FE2C != 0) && (numOptions == 3) && (((D_800E0A85 >= -30) && (stickY < -30)) || ((D_800E0A85 < 31) && (stickY >= 31)))) {
                sfx = 0x62D; /* cursor move */
                if (D_8008FE2C == 2) {
                    D_8008FE2C = 1;
                } else {
                    D_8008FE2C = 2;
                }
            }
        } else if (((D_8008FEF4 >= -30) && (stickX < -30)) || ((D_8008FEF4 < 31) && (stickX >= 31))) {
            D_8008FD98 = 1 - D_8008FD98;
            sfx = 0x62D; /* cursor move */
        }
        D_8008FEF4 = stickX & 0xFF; // matching: the masked temp fixes the t-register rotation
        D_800E0A85 = stickY;
        func_1504332C(0xFF, 0xFF, 0xFF, D_8008FDCD);
        if (reducedMenu != 0) {
            textY = menuY + 115;
            D_8008FE2C = 0;
            func_15042D94(148, textY, 1, D_800AB7DC, 0xD);
        } else {
            textY = menuY + 115;
            func_15042D94(148, textY, 1, D_800AB7E0, 0x1A);
            if (D_8008FE2C < numOptions) {
                func_15042D94(213, textY, 1, D_800AB7E4, (D_8008FE2C == 0) + 0xC);
            }
            func_150432FC(148, menuY + 85);
            if (numOptions == D_8008FE2C) {
                func_15042E3C(D_800AB7E8, 0x32);
            } else if (numOptions < D_8008FE2C) {
                func_15042E3C(D_800AB7EC, 0x33);
            }
        }
        func_150432FC(83, textY);
        if (D_8008FE2C < numOptions) {
            if (reducedMenu == 0) {
                i = 1;
                if (numOptions == 3) {
                    func_150432FC(83, menuY + 99);
                    func_15042E3C(D_800AB7F0, (D_8008FE2C == 1) + 0x21);
                    i = 2;
                    func_150432FC(83, menuY + 130);
                }
                func_15042E3C(D_800AB7F4, (i == D_8008FE2C) + 0xE);
            }
            if (D_800E0B9A & START_BUTTON) {
                D_8008FEF0 = 5;
                D_8008FEEC = 10;
            } else if ((D_8008FE2C == 0) || (D_800E0B9A & B_BUTTON)) {
                if (D_800E0B9A & (A_BUTTON | B_BUTTON)) {
                    D_8008FEF0 = 5;
                    D_8008FEEC = 10;
                }
            } else if (D_800E0B9A & A_BUTTON) {
                D_8008FD98 = 0;
                D_8008FE2C += numOptions - 1;
                if (numOptions == 2) {
                    D_8008FE2C++;
                }
                sfx = 0x500; /* confirm */
            }
        } else {
            func_15042D94(108, textY, 1, D_800AB7F8, D_8008FD98 + 0x1F);
            func_15042D94(188, textY, 1, D_800AB7FC, 0x1E - D_8008FD98);
            if (D_800E0B9A & A_BUTTON) {
                if (D_8008FD98 != 0) {
                    D_800E0B94 = 7;
                    D_800E0A88 = func_151DDBA0;
                    D_8008FD74 = 8;
                    if (D_800BE616 != 0) {
                        D_8008FDD4->unk3F = 4;
                        if (numOptions == D_8008FE2C) {
                            D_8008FD9C = 1;
                            D_800E0A88 = func_151E2834;
                        }
                    } else if (D_8008FDA8 >= 0) {
                        func_15007718(D_8008FDA8);
                    }
                    D_8008FE2C = 0;
                    D_80087270[0] = 0;
                } else {
                    u32 *aligned;

                    /* ROM integrity check: if 8 decoded bytes read from cart don't match, DMA 0x100 bytes over RAM */
                    aligned = dmaBuf;
                    while ((u32)aligned % 16) {
                        aligned++;
                    }
                    romAddr = func_151DDB94(-0xF81);
                    osWritebackDCache(aligned, 0x10);
                    osPiStartDma(&D_800388E0, 0, 0, romAddr, aligned, 0x10, gMessageQueue);
                    osRecvMesg(gMessageQueue, NULL, OS_MESG_BLOCK);
                    osInvalDCache(aligned, 0x10);
                    aligned[0] -= 0x3E0EF0F0;
                    aligned[1] -= 0x3E0EF0F0;
                    if ((aligned[0] != 0x3050A14A) || (aligned[1] != 0xBCFCF506)) {
                        osPiStartDma(&D_800388E0, 0, 0, func_150ADA20() & 0xFFFF, (void *)((func_150ADA20() & 0xFFFFE) + 0x80300000), 0x100, gMessageQueue);
                        osRecvMesg(gMessageQueue, NULL, OS_MESG_BLOCK);
                    }
                    D_8008FE2C -= 2;
                }
                sfx = 0x500; /* confirm */
            } else if (D_800E0B9A & B_BUTTON) {
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

/* Per-frame update of a timed sequence: A skips ahead to frame 480 and later 720; at frame 841 it hands over (func_1501C730(6, 0x21, ...)). */
void func_151DE6D4(void) {
    s32 timer;

    func_151E530C();
    D_800CC2D0[0].unk25C |= 0x200;
    D_800CC2D0[0].y_position = 1000.0f;

    if (D_80000300 == OS_TV_PAL) { /* D_80000300 is osTvType */
        D_800E0A90 = 0;
    }

    timer = D_800E0A90;
    if ((timer >= 331) && (D_800E0B9A & A_BUTTON)) {
        if (timer < 480) {
            timer = D_800E0A90 = 480;
        }
        if ((timer >= 601) && (timer < 720)) {
            timer = D_800E0A90 = 720;
        }
    }

    if (timer >= 841) {
        D_8008FE28 = 2;
        D_800E0B94 = 1;
        D_800D2E40 = 0;
        func_1501C730(6, 0x21, 0, 0, 1);
        D_800E0B96 = 0xFF;
    }
}

/* Resets the sequence timer and counters, then enters state 3 via func_151DE85C. */
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

/* Enters state 3 (D_800E0B94) and puts the path state back on path 0, stop 5. */
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
