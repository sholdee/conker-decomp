#include <ultra64.h>
#include "structs.h"

/*
 * Multiplayer menus and match rules: settings, pause menu, menu pages, scoring and the match HUD.
 * One compilation unit (the ROM interleaves the static data of all its parts).
 */

/*
 * .bss, linked at 0x800E0BA0..0x800E0C00 in this order. Defined before variables.h declares them:
 * IDO lays out .bss in first-declaration order, and an extern there would count.
 */
Actor *D_800E0BA0[4]; /* characters spawned by func_151E7F60 */
s8 D_800E0BB0;
s8 D_800E0BB1;
u8 D_800E0BB8[16];
s8 D_800E0BC8[4];
u16 D_800E0BCC;
s8 D_800E0BCE[2];
s8 D_800E0BD0;
u8 D_800E0BD1;
s8 D_800E0BD2;
s8 D_800E0BD3;
s32 D_800E0BD4;
u8 **D_800E0BD8; // text strings of the current language ([8 + n]: player names)
u8 D_800E0BDC;
/*
 * Settings record (option values, see func_151E3344): 23 entries used here, 0x1C saved to EEPROM by game_33990.c.
 * Defaults and valid [min, max] ranges are D_8008FE54/D_8008FE6C/D_8008FE84.
 */
// matching: defined here, not extern; IDO then knows the copy loop's stores can't alias the source
s8 D_800E0BE0[0x1C];

#include "functions.h"
#include "variables.h"

/* .data, linked at 0x8008FD70..0x8008FFC0. IDO keeps the definition order, function statics included. */
s8 D_8008FD70 = 0;
u8 D_8008FD74 = 8;
s32 D_8008FD78 = 1000;
s8 D_8008FD7C = 0;
u8 D_8008FD80 = 0;
s8 D_8008FD84[4] = { 0 };
s8 D_8008FD88 = 1;
s8 D_8008FD8C = 1;
s8 D_8008FD90 = 1;
s8 D_8008FD94 = 1;
s8 D_8008FD98 = 0;
s8 D_8008FD9C = 0;
s8 D_8008FDA0 = 0; /* extra (non-controller) slots in the player list */
u8 D_8008FDA4 = 0;
s8 D_8008FDA8 = -1;
s16 D_8008FDAC = 0;
s8 D_8008FDB0 = 0; /* highlighted row of the func_151DF574 list */
u8 D_8008FDB4 = 0;
u8 D_8008FDB8 = 0;
u16 D_8008FDBC = 0;
u16 D_8008FDC0 = 0;
s8 D_8008FDC4 = 0;
s8 D_8008FDC8 = 0;
s16 D_8008FDCC = 0; /* func_151DDC20 reads its low byte */
f32 D_8008FDD0 = 1.0f;
MenuState *D_8008FDD4 = NULL;
s32 D_8008FDD8 = 0;
s8 D_8008FDDC = 0;
s8 D_8008FDE0 = 0;
s8 D_8008FDE4 = 0;
f32 D_8008FDE8 = 0.0f;
/* Per-mode (D_800E0B94) update functions. */
void (*D_8008FDEC[12])(void) = {
    func_151DDC20, func_151DE81C, func_151DE8E8, func_151DE8F0, func_151E4E64, func_151E4264,
    func_151E43DC, func_151E4EE8, func_151E82B8, func_151E83E8, func_151E4DD8, func_151DE6D4,
};
f32 D_8008FE1C = 1.0f;
f32 D_8008FE20 = 1.0f;
s32 D_8008FE24 = 0;
s8 D_8008FE28 = 0;
s8 D_8008FE2C = 0;
s8 D_8008FE30 = 0;
u8 D_8008FE34 = 0;
f32 D_8008FE38 = 0.0f;
f32 D_8008FE3C = 0.0f;
s8 D_8008FE40 = 1; /* number of players in D_8008FE44 */
s8 D_8008FE44[16] = { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; /* controller of each player; -2: extra slot, -1: unused */
s8 D_8008FE54[23] = { 0, 0, 3, 10, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 2, 1, 20, 0, 1, 0, 1 }; /* settings defaults */
s8 D_8008FE6C[23] = { 0, 0, 1, 2, 1, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0 }; /* settings minimums */
s8 D_8008FE84[23] = { 2, 1, 5, 10, 5, 4, 10, 20, 1, 2, 4, 1, 2, 20, 1, 1, 4, 1, 40, 1, 1, 0, 1 }; /* settings maximums */
s8 D_8008FE9C[8] = { 1, 1, 1, 5, 1, 2, 2, 0 };
u32 D_8008FEA4[8] = { 0x815, 0x815, 0x815, 0x831, 0x5C0, 0x4380, 0x23800, 0 };
/* [preset][4] team per player; a 2D declaration mismatches */
s8 D_8008FEC4[32] = {
    0, 2, 11, 4, 0, 2, 11, 4, 0, 2, 11, 4, 4, 5, 0, 11, 6, 7, 8, 10, 7, 8, 9, 14, 13, 12, 11, 11, 0, 0, 0, 0,
};
s8 D_8008FEE4[8] = { 0, 0, 0, 0, 0, 1, 0, 1 }; /* per team preset: row of D_800AB74C */
s8 D_8008FEEC = 0;
s8 D_8008FEF0 = 0;

/* Resets every setting to its default. */
void func_151DD970(void) {
    s32 i;

    for (i = 0; i < 23; i++) {
        D_800E0BE0[i] = D_8008FE54[i];
    }
}

/* Resets each setting that lies outside its valid range to its default. */
void func_151DD9E4(void) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if (D_800E0BE0[i] < D_8008FE6C[i] || D_800E0BE0[i] > D_8008FE84[i]) {
            D_800E0BE0[i] = D_8008FE54[i];
        }
    }
}

/* Returns the bitwise complement of value. */
s32 func_151DDB94(s32 value) {
    return ~value;
}

/* ---- formerly game_20B050.c ---- */

/* Requests a switch to map 0x1D (func_1501C730 mode 6) and resets this mode's state flags. */
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

/* ---- formerly game_20B0D0.c ---- */

extern OSIoMesg D_800388E0;
extern void (*D_800E0A88)(void);

extern s8 D_800E0A85;
extern u8 D_800E0A98;
extern s8 D_800E0A99;

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

/*
 * Pause menu, run every frame: opens on START from an allowed controller, advances the play-time
 * counter, draws the countdown while D_800BE616 is set, and handles the menu cursor (D_8008FE2C) and the
 * yes/no confirmation (D_8008FD98).
 */
void func_151DDC20(void) {
    static s8 D_8008FEF4 = 0;
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
    if (level == 0x22) {
        reducedMenu = 1;
    }
    if ((D_800C35EA[0] != 1) || (level != 0x18) || (D_800C35E8[0] != 1)) {
        if (func_1517EFDC() != 0) {
            blocked = 1;
        }
    }
    if ((D_800C35EA[0] == 1) && (D_800C3683 != 0)) {
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
    if ((D_800BE616 != 0) && (D_800BEAC1 == 0) && (D_800E0BE0[3] >= 3)) {
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
        func_1504332C(0xFF, 0xFF, 0xFF, ((u8 *)&D_8008FDCC)[1]);
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
                        D_8008FDD4->page = 4;
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
    movement_obj[0].task_attrib |= 0x200;
    movement_obj[0].ypos = 1000.0f;

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
    D_8008FDD4->menu = 0;
    D_8008FDD4->unk2B = 5;
    D_8008FDD4->unk2C = D_8008FDD4->unk2B;
}

void func_151DE8E8(void) {
}

/* ---- formerly game_20BDA0.c ---- */

/* Menu input bits (MenuState.input / padInput), built from the pads in func_151DE8F0 */
#define MENU_UP     0x01 /* stick up */
#define MENU_DOWN   0x02 /* stick down */
#define MENU_LEFT   0x04 /* stick left */
#define MENU_RIGHT  0x08 /* stick right */
#define MENU_ACCEPT 0x10 /* A or Start */
#define MENU_BACK   0x20 /* B */
#define MENU_CLEFT  0x40 /* C-left */
#define MENU_CRIGHT 0x80 /* C-right */

extern char D_800AB850[];
extern s8 D_800E0A96; /* page to switch to once faded out; -1: none */

s8 D_8008FEF8 = 0;

/* Menu frame update: turns the pads into MENU_* input bits, then runs the active menu page. */
void func_151DE8F0(void) {
    static s16 messageTimer = 240;
    s32 i;
    s32 j;
    s16 unused; // matching: unused stack slot sets IDO's frame layout
    OSContPad *cont;

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
        if (0) goto lbl1; // matching: dead goto splits the block so as1 xbb skips a cross-branch hoist
        if (D_8008FE30 == 0) {
            func_1500764C();
        }
    }
    if (D_8008FD80) {
        camera_global_Cameras->Mode = 0x80000;
        func_1501D348(level, 1, 0, 0, 0);
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
        D_8008FDD4->padInput[i] = 0;
        if (D_800E0B96 == 0) {
            cont = &D_800BE918[i];
            if (D_8008FDD4->stickY[i] > -50 && cont->stick_y <= -50) {
                D_8008FDD4->padInput[i] |= MENU_DOWN;
            }
            if (D_8008FDD4->stickY[i] < 50 && cont->stick_y >= 50) {
                D_8008FDD4->padInput[i] |= MENU_UP;
            }
            if (D_8008FDD4->stickX[i] > -50 && cont->stick_x <= -50) {
                D_8008FDD4->padInput[i] |= MENU_LEFT;
            }
            if (D_8008FDD4->stickX[i] < 50 && cont->stick_x >= 50) {
                D_8008FDD4->padInput[i] |= MENU_RIGHT;
            }
            if (D_800BE930[i] & START_BUTTON) {
                D_8008FDD4->padInput[i] |= MENU_ACCEPT;
            }
            if (D_800BE930[i] & A_BUTTON) {
                D_8008FDD4->padInput[i] |= MENU_ACCEPT;
            }
            if (D_800BE930[i] & B_BUTTON) {
                D_8008FDD4->padInput[i] |= MENU_BACK;
            }
            if (D_800BE930[i] & L_CBUTTONS) {
                D_8008FDD4->padInput[i] |= MENU_CLEFT;
            }
            if (D_800BE930[i] & R_CBUTTONS) {
                D_8008FDD4->padInput[i] |= MENU_CRIGHT;
            }
            D_8008FDD4->stickY[i] = cont->stick_y;
            D_8008FDD4->stickX[i] = cont->stick_x;
        } else {
        lblQ:
            if (0) goto lblQ; // matching: same as1 xbb fence as above
            D_8008FDD4->stickX[i] = 0;
            D_8008FDD4->stickY[i] = 0;
        }
        if (D_8008FDD4->stickX[i] > -50 && D_8008FDD4->stickX[i] < 50) {
            D_8008FDD4->repeatX[i] = 0;
            // matching: store via an alias of D_8008FDDC so IDO re-materialises its address
            *(s8 *)((u8 *)&D_8008FDE0 - 4) = D_8008FDDC & ~(1 << i);
        } else if (!(D_8008FDDC & (1 << i))) {
            D_8008FDD4->repeatX[i] += D_800BE9E4;
            if (D_8008FDD4->repeatX[i] > 50) {
                D_8008FDD4->repeatX[i] = 40;
                D_8008FDD4->stickX[i] = 0;
            }
        }
        if (D_8008FDD4->stickY[i] > -50 && D_8008FDD4->stickY[i] < 50) {
            D_8008FDD4->repeatY[i] = 0;
        } else {
            D_8008FDD4->repeatY[i] += D_800BE9E4;
            if (D_8008FDD4->repeatY[i] > 50) {
                D_8008FDD4->repeatY[i] = 40;
                D_8008FDD4->stickY[i] = 0;
            }
        }
    }
    D_8008FDD4->input = D_8008FDD4->padInput[0];
    if (D_800C35EA[0] == 1) {
        D_8008FE24 = 0;
        if (D_800E0A90 < 0x546 && D_800BE930[0] != 0) {
            D_8008FD84[0] = 1;
        }
        return;
    }
    if (messageTimer > 0) {
        messageTimer -= D_800BE9E4;
        if (messageTimer > 60) {
            s32 alpha;

            if (messageTimer > 180) {
                alpha = 0xFF - ((messageTimer - 180) << 3);
                if (alpha < 0) {
                    alpha = 0;
                }
            } else {
                alpha = (messageTimer - 60) << 3;
                if (alpha > 0xFF) {
                    alpha = 0xFF;
                }
                if (alpha == 0xFF) {
                    if (messageTimer > 120) {
                        messageTimer = 140;
                    }
                }
                if (D_8008FDD4->input & MENU_ACCEPT) {
                    messageTimer = 120;
                }
            }
            func_1504332C(0xFF, 0x32, 0x32, alpha - (alpha >> 2));
            func_15042D94(0x94, 0xC0, 0x81, D_800E0BD8[0x8F]);
        }
    }
    camera_global_Cameras->Mode = 0x80000;
    func_1000E7A0(4, D_8008FDD4->menu);
    if (D_8008FDD4 == NULL || D_8008FDA4 < 5) {
        return;
    }
    D_8008FDD4->prevPage = D_8008FDD4->page;
    {
        s32 fade;

        fade = (D_8008FDD4->unk8 - 0.5f) * 512.0f;
        if (fade < 0) {
            fade = 0;
        } else if (fade > 0xFF) {
            fade = 0xFF;
        }
        D_800E0A94 = fade;
    }
    if (D_8008FDD4->unk8 >= (f32)0.5) { // matching: a double constant narrowed, not 0.5f
        if (D_8008FDD4->unk8 < 1.0f) {
            D_8008FDD4->input &= ~MENU_ACCEPT;
        }
        switch (D_8008FDD4->menu) {
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
                switch (D_8008FDD4->page) {
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
        D_8008FDD4->page = 0;
        D_8008FDD4->cursor = 0;
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

extern char D_800AB800[];

/* Menu page for unk2C == 1: fades in and out, toggling D_8008FEF8 and object 9's animation (0xE/0xF) each fade-out. */
void func_151DF1BC(void) {
    u16 unused; // matching: unused stack slot sets IDO's frame layout
    s32 alpha;
    s32 anim;
    Actor *obj;

    obj = func_15083E90(9);
    if (D_800E0A96 != -1) {
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            alpha = 0;
            D_8008FEF8 = D_8008FEF8 == 0;
            if (D_8008FEF8 == 0) {
                anim = 0xE;
                func_1001263C((func_150ADA20() & 1) + 0xC8, 0x7D00, 0x40);
            } else {
                anim = 0xF;
                func_1001263C((func_150ADA20() % 3U) + 0xBE, 0x7D00, 0x40);
            }
            if (obj != NULL) {
                obj->intelwalk = anim;
                D_800D154C = obj;
                dc_glbl_CharNum = D_800D154C - movement_obj;
                obj->scriptCmd = func_1507BB28(0, obj->intelwalk);
                obj->unk21C = 0;
                obj->unk201 = 0x14;
            }
            D_800E0A96 = -1;
        }
        D_8008FDD4->input &= ~(MENU_ACCEPT | MENU_BACK);
    } else {
        alpha = D_800E0A95 + (D_800BE9E4 << 5);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }
    D_800E0A95 = alpha;
    alpha = (D_800E0A94 * alpha) >> 8;
    if (alpha >= 0xFE) {
        alpha = 0xFF;
    }
    if (D_8008FEF8 == 0) {
        func_151E3344(0x200200, 0, 1, alpha, 1);
        if ((D_8008FE34 == 9) && (D_8008FDD4->input & (MENU_LEFT | MENU_RIGHT))) {
            func_151E7E9C();
            func_1000DE1C(D_800E0A97, 0);
            D_800E0A97 = func_1000EA94(D_800E0BE0[9]);
        }
        {
            s32 input;

            input = D_8008FDD4->input;
            if ((input & MENU_ACCEPT) && (D_8008FE34 == 21) && (D_8008FDE8 == 0.0f)) {
                D_800E0A96 = 1;
                D_8008FDD4->input &= ~(MENU_ACCEPT | MENU_BACK);
                input = D_8008FDD4->input;
            }
            if (input & MENU_BACK) {
                func_15007668();
            }
        }
    }
    if (obj != NULL) {
        if (func_151F2CDC() == 1) {
            if (obj->levelMode == 0) {
                obj->levelMode = 1;
            }
        } else {
            obj->levelMode = 0;
        }
    }
    if (D_8008FEF8 != 0) {
        if (D_8008FDD4->input & MENU_BACK) {
            D_800E0A96 = 1;
        }
        D_8008FDD4->input &= ~MENU_BACK;
        if (D_8008FEF8 != 0) { // matching: redundant re-test; IDO reloads D_8008FEF8 here
            func_1504332C(0xFF, 0, 0, alpha);
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

extern s8 D_800AB804[];
extern char D_800AB810[];
extern s8 D_800BE91B; /* == D_800BE918[0].stick_y; the field spelling mismatches */

s8 D_8008FF00 = 0;
u8 D_8008FF04 = 0;
u8 D_8008FF08[28] = {
    0xD0, 0x3A, 0xFF, 0x70, 0xB0, 0x31, 0xFF, 0x29, 0x16, 0xFF, 0xD9, 0x00, 0xD1, 0x64,
    0x00, 0xFF, 0x80, 0x00, 0x98, 0x83, 0x95, 0x29, 0xA6, 0x60, 0xFF, 0xD9, 0x00, 0x00,
};

/*
 * Menu page for unk2C == 2: a scrolling list of the entries in category D_8008FF00.
 * Left/right changes category, the stick scrolls; the highlighted entry goes to D_8008FF04.
 */
void func_151DF574(void) {
    s32 r;
    s32 redo;
    s32 i;
    s32 j;
    s32 count;
    s32 mask;
    s32 offset;
    s32 alpha;
    s32 y;
    s32 height;
    s32 shade;
    s32 rowAlpha;
    s32 wave;
    s8 pass;
    s32 sfx;
    s8 prevCategory;
    s32 g;
    s32 color;
    s32 scale;
    s32 b;
    s8 list[20];
    static s8 scroll = 0;
    static s8 prevRow = -1;
    static s8 step = 1;
    static u8 pulse = 0;

    sfx = 0;
    scale = D_800E0A94;
    do {
        redo = 0;
        prevCategory = D_8008FF00;
        if (D_800E0A96 == 1) {
            D_8008FF00--;
            if (D_8008FF00 < 0) {
                D_8008FF00 = 8;
            }
            step = -1;
        } else if (D_800E0A96 == 2) {
            D_8008FF00++;
            if (D_8008FF00 >= 9) {
                D_8008FF00 = 0;
            }
            step = 1;
        }
        do {
            offset = 1;
            i = 0; // matching: i is initialised before mask/count, outside the for
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
            rowAlpha = func_1509CF28(D_8008FF00, &i); // matching: reuses rowAlpha; a separate local reallocates
            if (D_8008FF00 == 0) {
                mask |= 1;
            }
            if (rowAlpha) {
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
                D_8008FF00 += step;
                if (D_8008FF00 >= 9) {
                    D_8008FF00 = 0;
                }
                if (D_8008FF00 < 0) {
                    D_8008FF00 = 8;
                }
            }
        } while (count == 0);
        if ((D_800E0A96 == 1) || (D_800E0A96 == 2)) {
            if (prevCategory == D_8008FF00) {
                D_800E0A96 = -1;
            } else {
                D_800E0A96 += 2;
                D_8008FF00 = prevCategory;
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
                step = -1;
            } else {
                D_800E0A96 = -1;
                D_8008FF00++;
                if (D_8008FF00 >= 9) {
                    D_8008FF00 = 0;
                }
                step = 1;
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
    if (scroll > 0) {
        scroll -= 2;
    } else if (scroll < 0) {
        scroll += 2;
    }
    if ((scroll == 0) && (D_800E0A95 == 0xFF)) {
        if (D_800BE91B < -49) {
            prevRow = D_8008FDB0;
            D_8008FDB0++;
            scroll = -10;
            sfx = 0x4FF;
        } else if (D_800BE91B >= 50) {
            prevRow = D_8008FDB0;
            D_8008FDB0--;
            scroll = 10;
            sfx = 0x4FF;
        }
    }
    if (D_8008FDB0 < 0) {
        D_8008FDB0 = 0;
        scroll = 0;
        sfx = 0;
    }
    if (D_8008FDB0 >= count) {
        D_8008FDB0 = count - 1;
        scroll = 0;
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
draw_rows: // matching: unused label splits the basic block for as1
    color = D_8008FF00 + D_8008FF00 + D_8008FF00; // matching: the sum, not * 3
    func_1504332C(0xFF, 0, 0, alpha);
    if (count < 6) {
        y = 0x2F;
        height = count * 11;
    } else {
        y = -(D_8008FDB0 * 11) - scroll + 0x45;
        height = 0x37;
        if (y >= 0x30) {
            y = 0x2F;
        }
    }
    pass = -1;
    i = count - 3;
    for (j = -3; j < count + 3; j++) {
        shade = scroll * 12;
        if (shade < 0) {
            shade = -shade;
        }
        if (i == D_8008FDB0) {
            shade = 0xFF - shade;
            if (shade == 0xFF) {
                wave = (pulse * 2) & 0x7F;
                if (wave >= 0x40) {
                    wave = 0x7F - wave;
                }
                shade = wave + 0xC0;
                pulse += D_800BE9E4;
            } else {
                pulse = 0x1F;
            }
        } else if (prevRow == i) {
            shade += 0x80;
        } else {
            shade = 0x80;
        }
        if (i == D_8008FDB0) {
            D_8008FF04 = list[i];
        }
        if (y < 0x50) {
            rowAlpha = y * 25 - 0x6D6;
        } else if ((height + 0x46 < y) && (y < height + 0x50)) {
            rowAlpha = (10 - (y - height)) * 25 + 0x6D6;
        } else {
            rowAlpha = 0xFF;
        }
        if (rowAlpha < 0) {
            rowAlpha = 0;
        }
        if (rowAlpha >= 0xFA) {
            rowAlpha = 0xFF;
        }
        if (alpha < rowAlpha) {
            rowAlpha = alpha;
        }
        r = (D_8008FF08[color] * shade) >> 8;
        g = (D_8008FF08[color + 1] * shade) >> 8;
        b = (D_8008FF08[color + 2] * shade) >> 8;
        func_1504332C(r, g, b, rowAlpha);
        if ((y >= 0x46) && (y < height + 0x50) && (pass == 0)) {
            func_15042D94(0x94, y + 0xF, 0x81, D_800E0B88[list[i] + offset]);
        }
        y += 11;
        i++;
        if (i >= count) {
            pass++;
            i = 0;
        }
    }
    if (D_800E0A95 == 0xFF) {
        if (D_8008FDD4->input & MENU_LEFT) {
            D_800E0A96 = 1;
            sfx = 0x4FF;
        } else if (D_8008FDD4->input & MENU_RIGHT) {
            D_800E0A96 = 2;
            sfx = 0x4FF;
        }
        if ((D_8008FDD4->input & MENU_ACCEPT) && (D_8008FF04 != 0xFF)) {
            D_800E0B94 = 7;
            D_800E0A88 = func_151E30C4;
            *(u8 *)&D_8008FD74 = 3; // matching: a u8 view of the s8 (same sb) keeps IDO's schedule
            sfx = 0x500;
        }
    }
    if (sfx != 0) {
        func_10010F30(sfx, 0x4650, 0x40, 0, 0);
    }
}

/* Saved-record slot (D_800BE3F8[]); see game_33990.c */
extern char D_800AB814[];
extern char D_800AB818[];
extern char D_800AB828[];
extern char D_800AB82C[];
extern char D_800AB830[];
extern char D_800AB834[];
extern char D_800AB838[];
extern char D_800AB83C[];
extern char D_800AB840[];

/* Menu page for unk2C == 3..5: shows saved record slot unk2C - 3 (score, h:m:s time) with play/erase options. */
void func_151DFF38(void) {
    s32 slot;
    u8 alpha;
    s32 sfx;
    s32 score;
    SaveSlotInfo *entry;
    s32 y;

    alpha = D_800E0A94;
    sfx = 0;
    slot = D_8008FDD4->unk2C - 3;
    if (D_8008FE2C < 2) {
        if (D_8008FDD4->input & (MENU_LEFT | MENU_RIGHT)) {
            D_8008FE2C = 1 - D_8008FE2C;
            sfx = 0x62D;
        }
    } else if (D_8008FDD4->input & (MENU_LEFT | MENU_RIGHT)) {
        D_8008FE2C = 5 - D_8008FE2C;
        sfx = 0x62D;
    }
    func_150432FC(0x94, 0x46);
    entry = &D_800BE3F8[slot];
    if (entry->sequence == -1) {
        D_8008FE2C = 0;
    }
    if (slot == 2) {
        func_1504332C(0xFF, 0x37, 0x37, alpha);
    } else if (slot == 1) {
        func_1504332C(0x78, 0xFF, 0x16, alpha);
    } else {
        func_1504332C(0x20, 0x55, 0xFF, alpha);
    }
    if (entry->sequence != -1) {
        score = entry->unk4;
        y = 0;
        if (score == 32000) {
            score = 1000000;
            func_150432BC(0.75f);
            y = 2;
        }
        func_15042D94(0x70, y + 0x33, 0, D_800AB814, score);
        func_150432BC(1.0f);
        {
            u32 time;
            u32 seconds;

            time = entry->playTime;
            seconds = time % 60;
            time /= 60;
            func_15042D94(0xA4, 0x33, 0, D_800AB818, time / 60, time % 60, seconds);
        }
    }
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    if (entry->sequence >= 0) {
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
        if (entry->sequence >= 0) {
            func_15042E3C(D_800AB838, (D_8008FE2C != 0) + 0x18);
        }
        if (D_8008FE2C == 0) {
            if (D_8008FDD4->input & MENU_ACCEPT) {
                s32 pad; // matching: -g3 block scope keeps the stores in order

                D_800E0B94 = 7;
                D_800E0A88 = func_151E327C;
                D_8008FD74 = 3;
                D_8008FDA8 = slot;
                D_800BE3DC = -1;
                D_80082BC0 = 0;
                func_150076BC(entry->eepromSlot);
                sfx = 0x500;
            }
        } else if (D_8008FDD4->input & MENU_ACCEPT) {
            D_8008FE2C = 2;
            sfx = 0x500;
        }
    } else {
        func_15042D94(0x6C, 0xAF, 1, D_800AB83C, (D_8008FE2C == 3) + 0x1F);
        func_15042D94(0xBC, 0xAF, 1, D_800AB840, (D_8008FE2C == 2) + 0x1D);
        if (D_8008FDD4->input & MENU_ACCEPT) {
            if (D_8008FE2C == 3) {
                D_8008FE2C = 0;
                func_15007750(slot);
                D_800BE3DF = 0x29;
                D_800BE3E0 = 0;
                D_800BE3E4 = 0;
                func_15017790();
                entry->unkC = 0x18;
                entry->unk4 = 0;
                entry->sequence = -1;
                D_8008FDA4 = 0;
            } else {
                D_8008FE2C = 1;
            }
            sfx = 0x500;
        } else if (D_8008FDD4->input & MENU_BACK) {
            D_8008FE2C = 1;
        }
        D_8008FDD4->input &= ~MENU_BACK;
    }
    if (sfx != 0) {
        func_10010F30(sfx, 0x4650, 0x40, 0, 0);
    }
}

extern char D_800AB844[];
extern char D_800AB848[];
extern char D_800AB84C[];

/* Mode-select page: up/down picks a game mode of category unk2C; accept opens its setup (page 4). */
void func_151E0424(void) {
    s32 titleAlpha;
    s32 modeCount;
    s32 first;
    s32 cursor;
    s32 i;
    s32 x;
    s32 value;
    s32 alpha;
    s32 flags;
    f32 step;
    s8 teams;

    alpha = D_800E0A94;
    if (D_8008FDD4->unk2C != 0) {
        titleAlpha = alpha;
        if (D_800E0A96 != -1) {
            D_8008FDD4->input &= ~MENU_BACK;
            i = D_800E0A95 - (D_800BE9E4 << 5); // matching: the fade level shares i's register
            if (i <= 0) {
                D_800E0A95 = 0;
                D_8008FDD4->page = D_800E0A96;
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
            alpha = D_800E0A95;
            if (D_800E0A96 != 2) {
                titleAlpha = alpha;
            }
        } else {
            i = D_800E0A95 + (D_800BE9E4 << 5);
            if (i >= 0x100) {
                i = 0xFF;
            }
            D_800E0A95 = i;
            alpha = (alpha * i) >> 8;
            if (alpha >= 0xFE) {
                alpha = 0xFF;
            }
        }
        modeCount = D_8008FE9C[D_8008FDD4->unk2C - 1];
        D_8008FDA0 = 0;
        D_8008FDC4 = 0;
        D_800E0B8C = 0;
        if (D_8008FDD4->input & (MENU_LEFT | MENU_RIGHT)) {
            D_8008FE2C = 1 - D_8008FE2C;
            func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
        }
        func_1504332C(0xFF, 0xFF, 0xFF, titleAlpha);
        func_15042D94(0x94, 0x7E, 1, D_800AB844, 0x1A);
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(0xD5, 0x7E, 1, D_800AB848, (D_8008FE2C == 1) + 0x1B);
        if ((alpha == 0xFF) && (D_8008FE2C == 0)) {
            if ((modeCount != 5) || ((modeCount == 5) && (D_8008FDE8 == 0.0f))) { // matching: redundant == 5 test
                flags = D_8008FDD4->input;
                if (flags & MENU_UP) {
                    D_8008FDE8 = 1.0f;
                    D_8008FDD4->cursor--;
                    func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
                } else if (flags & MENU_DOWN) {
                    D_8008FDE8 = -1.0f;
                    D_8008FDD4->cursor++;
                    func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
                }
            }
        }
        value = D_8008FDD4->cursor;
        if (value < 0) {
            D_8008FDD4->cursor = value + modeCount;
            value = D_8008FDD4->cursor;
        }
        if (value >= modeCount) {
            D_8008FDD4->cursor = value - modeCount;
            value = D_8008FDD4->cursor;
        }
        cursor = value;
        first = 0;
        for (i = 0; i < D_8008FDD4->unk2C - 1; i++) {
            first += D_8008FE9C[i];
        }
        D_8008FDD4->selection = cursor + first;
        if (modeCount != 5) {
            x = 140 - ((modeCount * 28) >> 1);
            for (i = 0; i < modeCount; i++) {
                func_150432FC(0x53, x);
                value = D_800AB68C[first + i].nameText;
                if ((i == cursor) && (D_8008FE2C == 0)) {
                    value++;
                }
                func_15042E3C(D_800AB84C, value);
                x += 28;
            }
        } else {
            step = D_800BE9E4 * 0.05f;
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
                D_8008FDD4->input &= ~MENU_ACCEPT;
            }
        }
        if (alpha == 0xFF) {
            flags = D_8008FDD4->input;
            if (flags & MENU_ACCEPT) {
                D_8008FDD4->input = flags & ~MENU_BACK;
                if (D_8008FE2C == 0) {
                    D_800E0A96 = 4;
                    D_800E0BE0[0] = D_8008FDD4->cursor;
                    teams = D_800AB68C[D_8008FDD4->selection].unk5;
                    if (teams < 0) {
                        D_800E0B90[0] = D_8008FEC4[(-teams - 1) << 2];
                    } else {
                        D_800E0B90[0] = 0;
                    }
                    D_8008FE40 = 1;
                } else {
                    D_800E0A96 = 2;
                    D_8008FDD4->input &= ~MENU_BACK;
                }
            }
        }
    }
}

/* Options page for the selected mode (D_800AB754[mode]); leaving it saves the settings if D_8008FDE0 is set. */
void func_151E09DC(void) {
    s32 options;
    s32 alpha;

    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    func_15042D94(0x94, 0x7E, 1, D_800AB850, 0x1A);
    func_15042D78(0x81);

    options = D_800AB754[D_800AB68C[D_8008FDD4->selection].mode];
    if (D_800E0A96 == 0) {
        D_8008FDD4->input &= ~MENU_BACK;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_8008FDD4->page = 0;
            if (D_8008FDE0 != 0) {
                func_151E2404();
            }
            D_8008FDE0 = 0;
            D_800E0A96 = -1;
            return;
        }
        D_800E0A95 = alpha; // matching: stored twice on this path
    } else {
        alpha = D_800E0A95 + (D_800BE9E4 << 5);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }

    D_800E0A95 = alpha;
    if (alpha != 0xFF) {
        D_8008FDD4->input = 0;
    }
    func_151E3344(options, 1, 0, alpha, 0);

    if (D_8008FDD4->input & MENU_BACK) {
        D_800E0A96 = 0;
    }
    D_8008FDD4->input &= ~MENU_BACK;
}

extern char D_800AB854[];

/*
 * Player-join page: controllers 2-4 press Start to join or leave (D_8008FE44 lists the players).
 * Modes with a character choice let each player cycle D_800E0B90[] with left/right.
 */
void func_151E0B70(void) {
    s32 alpha;
    s32 dir;
    s32 i;
    s32 j;
    s32 k;
    s32 left;
    s32 nextPage;
    s8 *remap;
    u32 allowed;
    s8 maxPlayers;
    s8 reload;

    remap = NULL;
    allowed = -1;
    reload = 0;
    if (D_800E0A96 != -1) {
        D_8008FDD4->input &= ~MENU_BACK;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_800E0A95 = 0;
            D_8008FDD4->page = D_800E0A96;
            D_800E0A96 = -1;
            for (i = 0; i < 4; i++) {
                if (D_800E0BA0[i] != NULL) {
                    func_15060F28(D_800E0BA0[i], 1);
                    D_800E0BA0[i] = NULL;
                }
            }
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
    if (D_800AB68C[D_8008FDD4->selection].unk5 < 0) {
        nextPage = 5;
        i = -D_800AB68C[D_8008FDD4->selection].unk5 - 1;
        remap = D_8008FEC4 + (i << 2);
        allowed = D_8008FEA4[i];
        if (D_800E9D00 & 0x20) {
            allowed |= 0x8000;
        }
        if (D_800E9D00 & 0x10) {
            allowed |= 0x10000;
        }
        if (D_800E9D00 & 0x40) {
            allowed |= 0x30;
        }
        if (D_800E9D00 & 0x80) {
            allowed |= 0x43C0;
        }
        if (D_800E9D00 & 0x100) {
            allowed |= 0xE0800;
        }
        if (D_800E9D00 & 0x10000) {
            allowed |= 0xA;
        }
        if (D_800E9D00 & 0x20000) {
            allowed |= 0x300000;
        }
    } else {
        nextPage = 1;
    }
    if (D_8008FDD4->unk2C == 7) {
        maxPlayers = 2;
    } else {
        maxPlayers = 4;
    }
    D_800E0B8C = 0;
    if ((nextPage == 5) && (D_800AB68C[D_8008FDD4->selection].unk8 != 8)) {
        if (D_800E0BA0[0] == NULL) {
            func_151E7F60(0, D_800E0B90[0]);
        }
        func_15042D94(0x94, 0x9B, 1, D_800AB854, 0x1A);
        D_800E0B8C = 1;
    }
    for (i = 1; i < 4; i++) {
        if ((i < maxPlayers) && (alpha == 0xFF) && (D_800BE930[i] & START_BUTTON)) {
            left = 0;
            for (j = 0; j < D_8008FE40; j++) {
                if (i == D_8008FE44[j]) {
                    D_8008FE40--;
                    left = 1;
                    for (k = j; k < D_8008FE40; k++) {
                        D_8008FE44[k] = D_8008FE44[k + 1];
                        D_800E0B90[k] = D_800E0B90[k + 1];
                    }
                }
            }
            if (!left) {
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
                    reload = 1;
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
            if (D_8008FDD4->padInput[j] & MENU_RIGHT) {
                D_800E0B90[i]++;
                if (D_800E0B90[i] >= 22) {
                    D_800E0B90[i] = 0;
                }
            }
            if (D_8008FDD4->padInput[j] & MENU_LEFT) {
                D_800E0B90[i]--;
                dir = -1;
                if (D_800E0B90[i] < 0) {
                    D_800E0B90[i] = 21;
                }
            }
            while (!((1 << D_800E0B90[i]) & allowed)) {
                D_800E0B90[i] += dir;
                if (D_800E0B90[i] < 0) {
                    D_800E0B90[i] = 21;
                }
                if (D_800E0B90[i] >= 22) {
                    D_800E0B90[i] = 0;
                }
            }
            if ((k != D_800E0B90[i]) || reload) {
                func_151E7F60(j, D_800E0B90[i]);
            }
        }
    }
    for (i = D_8008FE40; i < 16; i++) {
        D_8008FE44[i] = -1;
    }
    if (alpha == 0xFF) {
        if (D_8008FDD4->input & MENU_ACCEPT) {
            D_8008FD74 = 8;
            func_10010F30(0x500, 0x7D00, 0x40, 0, 0);
            D_800E0A96 = nextPage;
        }
        if (D_8008FDD4->input & MENU_BACK) {
            D_800E0A96 = 0;
            D_8008FE2C = 0;
        }
    }
    D_8008FDD4->input &= ~MENU_BACK;
}

extern s8 D_800AB74C[][4];
extern char D_800AB858[];
extern char D_800AB85C[];
extern char D_800AB860[];

/* Computer-player page: up/right and down/left change the number of CPU players D_8008FDA0 (list entries of -2). */
void func_151E1214(void) {
    s32 alpha;
    s8 *limits;
    s32 limit;
    s32 i;
    s32 x;

    if (D_800E0A96 != -1) {
        D_8008FDD4->input &= ~MENU_BACK;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_800E0A95 = 0;
            D_8008FDD4->page = D_800E0A96;
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
    // unk5 is -(preset + 1) here, so -1 - unk5 is the preset
    limits = D_800AB74C[D_8008FEE4[-1 - D_800AB68C[D_8008FDD4->selection].unk5]];
    limit = (limits - 1)[D_8008FE40]; // matching: limits[D_8008FE40 - 1] schedules differently
    for (i = 0; i < D_8008FE40; i++) {
        if (D_8008FE44[i] >= 0) {
            if (D_8008FDD4->padInput[D_8008FE44[i]] & (MENU_UP | MENU_RIGHT)) {
                D_8008FDA0++;
                if (limit >= D_8008FE40 + D_8008FDA0) {
                    func_10010F30(0x4FF, 0x4650, 0x40, 0, 0);
                }
            }
            if (D_8008FDD4->padInput[D_8008FE44[i]] & (MENU_DOWN | MENU_LEFT)) {
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
    for (i = limit; i < 16; i++) D_8008FE44[i] = -1; // matching: on one line; -g3 line numbers steer as1
    if (alpha == 0xFF && (D_8008FDD4->input & MENU_ACCEPT)) {
        D_800E0B94 = 7;
        D_8008FD74 = 8;
        func_10010F30(0x500, 0x7D00, 0x40, 0, 0);
        if (D_800AB68C[D_8008FDD4->selection].unk6 == -1) {
            D_800E0A88 = func_151E2834;
        } else {
            D_800E0A88 = func_151E4314;
        }
    }
    func_15042D94(0x94, 0x55, 1, D_800AB858, 0x1A);
    x = 161 - D_8008FE40 * 13 - D_8008FDA0 * 10;
    for (i = 0; i < limit; i++) {
        func_150432FC(x, 0x82);
        if (D_8008FE44[i] >= 0) {
            func_15042E3C(D_800AB85C, D_8008FE44[i] + 1);
            x += 26;
        } else {
            func_15042E3C(D_800AB860, 5);
            x += 20;
        }
    }
    if (alpha == 0xFF && (D_8008FDD4->input & MENU_BACK)) {
        D_800E0A96 = 4;
    }
    D_8008FDD4->input &= ~MENU_BACK;
}

Offs4 D_8008FF34 = { { -12, -12, -12, -12 } }; /* per-team x jitter for stacked names */
s8 D_8008FF38[4] = { 0, 2, 3, 4 }; /* indexed from 1 through D_8008FF37 */
/* Team layout of a mode (D_800AB710[ModeEntry.unk5]) */
typedef struct {
    /* 0x0 */ s8 teamCount;
    /* 0x1 */ s8 minSlots;    /* bounds on teamCount * team size, with fewer than 3 players */
    /* 0x2 */ s8 maxSlots;
    /* 0x3 */ s8 teamMax;     /* players allowed per team */
    /* 0x4 */ s8 minSlots3;   /* the same, with 3 or more players */
    /* 0x5 */ s8 maxSlots3;
    /* 0x6 */ s8 teamMax3;
    /* 0x7 */ s8 unk7;        /* if nonzero: team 1's size, and the cap of teams 1-3 */
    /* 0x8 */ s8 teamColor[4];
} TeamSetup;

extern TeamSetup D_800AB710[];
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

/*
 * Team page: left/right moves each player between the D_800E0BB0 teams, up/down sets the extra
 * slots D_8008FDA0, and the team size D_800E0BB1 is kept within the mode's TeamSetup bounds.
 */
void func_151E1744(void) {
    s32 alpha;
    TeamSetup *setup;
    s32 i;
    s8 special;
    s8 minSlots;
    s8 maxSlots;
    s8 teamMax;
    s8 unused; // matching: unused stack slot sets IDO's frame layout
    s8 bound;
    s8 cap;
    s32 extra[3];
    s8 teamPlayers[8];
    s32 hasExtra;
    s16 step;
    s16 x;
    s16 y;
    s16 slots;
    s16 teamX[4];
    s16 teamY[4];
    Offs4 jitter;
    s8 teamCap[4];
    s8 teamSize[4];
    s32 sfx;
    s32 v;
    s32 old; // matching: one local for the old team, the colour index and the text base

    sfx = 0;
    jitter = D_8008FF34;
    if (D_800E0A96 != -1) {
        D_8008FDD4->input &= ~MENU_BACK;
        alpha = D_800E0A95 - (D_800BE9E4 << 5);
        if (alpha <= 0) {
            D_800E0A95 = 0;
            D_8008FDD4->page = D_800E0A96;
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
    setup = &D_800AB710[D_800AB68C[D_8008FDD4->selection].unk5];
    special = setup->unk7;
    D_800E0BB0 = setup->teamCount;
    D_800E0BC8[0] = setup->teamColor[0];
    D_800E0BC8[1] = setup->teamColor[1];
    D_800E0BC8[2] = setup->teamColor[2];
    D_800E0BC8[3] = setup->teamColor[3];
    if (D_8008FE40 < 3) {
        minSlots = setup->minSlots;
        maxSlots = setup->maxSlots;
        teamMax = setup->teamMax;
    } else {
        minSlots = setup->minSlots3;
        maxSlots = setup->maxSlots3;
        teamMax = setup->teamMax3;
    }
    for (i = 0; i < 4; i++) {
        teamCap[i] = teamMax;
        if (special != 0 && i >= 1) {
            teamCap[i] = special;
        }
    }
    i = 0; // matching: while with i++ in the store; a for loop schedules differently
    while (i < D_800E0BB0) {
        teamPlayers[i] = 0;
        teamY[i++] = 0x6E;
    }
    for (i = 0; i < D_8008FE40; i++) {
        teamPlayers[D_800E0C00[i]]++;
    }
    for (i = D_8008FE40; i < 16; i++) {
        D_8008FE44[i] = -1;
    }
    for (i = 0; i < D_8008FE40; i++) {
        if (D_8008FE44[i] >= 0) {
            old = D_800E0C00[i];
            if (D_8008FDD4->padInput[D_8008FE44[i]] & (MENU_UP | MENU_DOWN | MENU_LEFT | MENU_RIGHT)) {
                sfx = 0x4FF;
            }
            if (D_8008FDD4->padInput[D_8008FE44[i]] & MENU_RIGHT) {
                D_800E0C00[i]++;
            }
            if (D_8008FDD4->padInput[D_8008FE44[i]] & MENU_LEFT) {
                D_800E0C00[i]--;
            }
            if (D_8008FDD4->padInput[D_8008FE44[i]] & MENU_DOWN) {
                D_8008FDA0++;
            }
            if (D_8008FDD4->padInput[D_8008FE44[i]] & MENU_UP) {
                D_8008FDA0--;
            }
            if (D_8008FDA0 < 0) {
                D_8008FDA0 = 0;
                sfx = 0;
            }
            D_800E0C00[i] &= D_800E0BB0 - 1;
            if (teamPlayers[D_800E0C00[i]] >= teamCap[D_800E0C00[i]]) {
                D_800E0C00[i] = old;
            }
            teamPlayers[D_800E0C00[i]]++;
            teamPlayers[old]--;
        }
    }
    D_800E0BB1 = 1;
    for (i = 0; i < D_800E0BB0; i++) {
        if (D_800E0BB1 < teamPlayers[i]) {
            D_800E0BB1 = teamPlayers[i];
        }
    }
    D_800E0BB1 += D_8008FDA0;
    while (D_800E0BB1 * D_800E0BB0 < minSlots) {
        D_800E0BB1++;
        D_8008FDA0++;
    }
    while (maxSlots < D_800E0BB1 * D_800E0BB0) {
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
        teamSize[i] = v;
    }
    if (D_800E0BB0 == 2) {
        if (D_8008FDD4->input & MENU_CLEFT) {
            D_8008FDC4++;
        } else if (D_8008FDD4->input & MENU_CRIGHT) {
            D_8008FDC4--;
        }
        if (D_8008FE40 >= 3) {
            D_8008FDC4 = 0;
        }
        bound = D_800E0BB1;
        if (D_8008FDC4 < -bound) {
            D_8008FDC4 = -bound;
        } else if (bound < D_8008FDC4) {
            D_8008FDC4 = bound;
        } else if (D_8008FDD4->input & (MENU_CLEFT | MENU_CRIGHT)) {
            sfx = 0x4FF;
        }
        teamSize[0] += D_8008FDC4;
        teamSize[1] -= D_8008FDC4;
        for (i = 0; i < 2; i++) {
            cap = (i == 1 && special != 0) ? special : 8;
            if (teamSize[i] <= 0) {
                teamSize[i] = 1;
            }
            if (cap < teamSize[i]) {
                teamSize[i] = cap;
            }
        }
    }
    step = 0x128 / D_800E0BB0;
    x = step >> 1;
    slots = D_8008FE40;
    for (i = 0; i < D_800E0BB0; i++) {
        teamX[i] = x;
        x += step;
        v = teamSize[i];
        while (teamPlayers[i] < v) {
            D_8008FE44[slots] = -2;
            D_800E0C00[slots] = i;
            teamPlayers[i]++;
            slots++;
        }
    }
    if (alpha == 0xFF) {
        if (D_8008FDD4->input & MENU_ACCEPT) {
            D_800E0B94 = 7;
            D_800E0A88 = func_151E4314;
            D_8008FD74 = 8;
            sfx = 0x500;
        } else if (D_8008FDD4->input & MENU_BACK) {
            D_800E0A96 = 4;
        }
    }
    D_8008FDD4->input &= ~MENU_BACK;
    func_1504332C(0xFF, 0xFF, 0, alpha);
    func_150432FC(0x94, ((D_800E0BB0 == 4) << 5) + 0x78);
    func_15042E3C(D_800AB864, 0x1A);
    for (i = 0; i < D_800E0BB0; i++) {
        x = teamX[i];
        old = D_800E0BC8[i] * 3;
        // matching: flat byte index into the RGB rows (old is the tripled colour)
        func_1504332C(D_800AB62C[0][old], D_800AB62C[0][old + 1], D_800AB62C[0][old + 2], alpha);
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
    hasExtra = func_151E24F0(extra);
    func_1504332C(0xFF, 0xFF, 0xFF, alpha);
    for (i = 0; i < slots; i++) {
        old = D_800E0C00[i];
        if (slots == D_800E0BB0) {
            jitter.v[old] = 0;
        }
        x = teamX[old] + jitter.v[old];
        y = teamY[old];
        jitter.v[old] = -jitter.v[old];
        if (jitter.v[old] < 0) {
            teamY[old] += 0x18;
        }
        func_150432FC(x, y);
        if (D_8008FE44[i] >= 0) {
            func_15042E3C(D_800AB880, D_8008FE44[i] + 1);
            if (hasExtra != 0) {
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

/* Menu page 3: runs func_151E530C and func_151E43DC with D_8008FD80 = 3. */
void func_151E2284(void) {
    D_8008FD80 = 3;
    func_151E530C();
    func_151E43DC();
    D_8008FD80 = 0;
}

/* Loads the selected mode's saved settings (D_80082BBC[mode]) into the live settings D_800E0BE0[]. */
void func_151E22BC(void) {
    s32 mode;
    ModeSettings *settings;

    mode = D_800AB68C[D_8008FDD4->selection].mode;
    settings = &D_80082BBC[mode];
    if (settings->unk3 >= 5) {
        settings->unk3 = 4;
    }
    if (settings->unk3 < 0) { // matching: never true for a u8, but IDO still emits the test
        settings->unk3 = 0;
    }
    if (settings->unk1 <= 0) {
        settings->unk1 = 1;
    }
    D_800E0BE0[2] = settings->unk1;
    D_800E0BE0[3] = settings->unk2;
    D_800E0BE0[11] = 0;
    D_800E0BE0[16] = settings->unk3;
    D_800E0BE0[17] = settings->unk4;
    switch (mode) {
        case 0:
            D_800E0BE0[4] = settings->unk5;
            break;
        case 1:
        case 2:
            D_800E0BE0[13] = settings->unk5;
            D_800E0BE0[14] = settings->unk6;
            break;
        case 7:
            D_800E0BE0[18] = settings->unk5;
            break;
        case 5:
            D_800E0BE0[17] = 0;
            D_800E0BE0[18] = settings->unk5;
            break;
        case 8:
        case 9:
            D_800E0BE0[7] = settings->unk5;
            D_800E0BE0[19] = settings->unk6;
            D_800E0BE0[17] = 1;
            D_800E0BE0[3] = 2;
            break;
        default:
            break;
    }
    D_800E0BE0[22] = settings->unk8;
    D_800E0BE0[6] = settings->unk9;
    func_151DD9E4();
}

/* Stores the live settings D_800E0BE0[] back into the selected mode's record; the inverse of func_151E22BC. */
void func_151E2404(void) {
    ModeSettings *settings;
    s32 mode;

    mode = D_800AB68C[D_8008FDD4->selection].mode;
    settings = &D_80082BBC[mode];
    settings->unk0 = 0;
    settings->unk1 = D_800E0BE0[2];
    settings->unk2 = D_800E0BE0[3];
    settings->unk3 = D_800E0BE0[16];
    settings->unk4 = D_800E0BE0[17];
    switch (mode) {
        case 0:
            settings->unk5 = D_800E0BE0[4];
            break;
        case 1:
        case 2:
            settings->unk5 = D_800E0BE0[13];
            settings->unk6 = D_800E0BE0[14];
            break;
        case 5:
        case 7:
            settings->unk5 = D_800E0BE0[18];
            break;
        case 8:
        case 9:
            settings->unk5 = D_800E0BE0[7];
            settings->unk6 = D_800E0BE0[19];
            break;
    }
    settings->unk8 = D_800E0BE0[22];
    settings->unk9 = D_800E0BE0[6];
    func_150076A0(D_800E0BE0);
}

/* ---- formerly game_20F9A0.c ---- */

/* Cue table row: func_1501D348(unk0, unk2 or unk3). */
struct Struct151E4314 {
    s16 unk0;
    s8 unk2;
    s8 unk3;
};

extern struct Struct151E4314 D_800AB7A4[];

/*
 * For modes whose entry has unk8 == 8: finds connected controllers (D_800BE740) that are not yet in the
 * roster D_8008FE44[0..3]. With out == NULL they are inserted as extra roster slots; otherwise they are
 * written to out. Returns the number of rostered players, or 0 when nothing was added.
 */
s32 func_151E24F0(s32 *out) {
    s32 i;
    s32 j;
    s32 count;
    s32 player;
    s8 used[4];
    s8 allowed[4];
    u8 mask;

    if (out == NULL) {
        D_800E0BCE[0] = 0;
        D_800E0BCE[1] = 0;
    }

    if (D_800AB68C[D_8008FDD4->selection].unk8 == 8) {
        mask = 0;
        count = 0;
        if (D_800E0BE0[14] != 0) {
            for (i = 0; i < 4; i++) {
                used[i] = 0;
            }
            for (i = 0; i < 4; i++) {
                player = D_8008FE44[i];
                if (player >= 0) {
                    count++;
                    used[player] = 1;
                    mask |= 1 << player;
                }
            }
            mask = D_800BE740 & ~mask;
            if (count < 3) {
                if (mask != 0) {
                    for (i = 0; i < 4; i++) {
                        allowed[i] = 1;
                        if (count == 1) {
                            if ((D_800BE740 & (1 << i)) == 0) {
                                allowed[i] = 0;
                            }
                        }
                    }
                    j = 0;
                    if (out == NULL) {
                        for (j = 16; j >= count * 2; j--) {
                            D_8008FE44[j] = D_8008FE44[j - count];
                            D_800E0C00[j] = D_800E0C00[j - count];
                        }
                        for (j = count, i = 0; (i < 4) && (j < count * 2); i++) {
                            if ((used[i] == 0) && (allowed[i] != 0)) {
                                D_8008FE44[j] = i;
                                D_800E0C00[j] = D_800E0C00[j - count];
                                D_800E0BCE[j - count] = j;
                                D_80087270[j] = 10;
                                j++;
                            }
                        }
                    } else {
                        for (i = 0; (i < 4) && (j < count); i++) {
                            if ((used[i] == 0) && (allowed[i] != 0)) {
                                out[j] = i;
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

extern Entry151E7F60 D_800AB57C[];
extern s16 D_800AB77C[];
extern s16 D_800AB790[];
extern s8 D_800E0AB0[];
extern u8 D_800E0AA8;
extern s8 D_800E0AA9;
extern s8 D_800E0AAA;

/* Sets up a match from the current mode's table entry: roster, teams, rules flags and per-player kinds. */
void func_151E2834(void) {
    s32 i;
    s32 flags;
    s32 k;
    s32 param;
    s32 usedTeams;
    s32 playerMask;
    s32 v;
    ModeEntry *entry;

    entry = &D_800AB68C[D_8008FDD4->selection];
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
    bzero(D_8008FDD4->stats, sizeof(D_8008FDD4->stats));
    bzero(D_8008FDD4->score, sizeof(D_8008FDD4->score));
    bzero(D_800E0AD0, sizeof(D_800E0AD0));
    D_800BEAC1 = 0;
    D_800BEAC0 = 0;
    D_800E0B94 = 0;
    D_8008FDA8 = -1;
    D_80084060[1] = -1;
    D_80084060[2] = -1;
    D_80084060[3] = -1;
    param = entry->unk2;
    D_8008FDC8 = entry->unk5 < 0;
    flags = D_800AB754[entry->mode];
    D_8008FDC0 = D_800AB77C[entry->mode];
    D_8008FDBC = D_800AB790[entry->mode] | 1;
    if (flags & 0x80000) {
        D_800D23A8 = D_800E0BE0[19];
    } else {
        D_800D23A8 = 0;
    }
    if (flags & 8) {
        D_8008FD78 = D_800E0BE0[3] * 3600;
    } else {
        D_8008FD78 = 0;
    }
    if (flags & 0x10000) {
        D_800E0BD0 = D_800E0BE0[16];
    } else {
        D_800E0BD0 = 2;
    }
    if ((flags & 4) && (D_8008FD78 == 0)) {
        D_8008FDC0 |= 2;
        D_8008FDBC &= ~1;
    }
    if (D_8008FDC0 & 0x80) {
        D_8008FDC0 |= 0x1000;
    }
    if ((flags & 0x20000) && (D_800E0BE0[17] != 0)) {
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
        // matching: a do-while (not a for) gives the target's store order
        i = 0;
        do {
            D_80087270[i] = 8;
        } while (++i < 16);
        D_8008FD70 = func_151E24F0(NULL);
    }
    playerMask = 1;
    D_8008FD88 = 1;
    if (D_8008FE44[1] != -1) {
        playerMask |= 2;
        D_80084060[1] = D_8008FE44[1];
        D_8008FD88++;
    }
    if (D_8008FE44[2] != -1) {
        playerMask |= 4;
        D_8008FD88++;
        D_80084060[2] = D_8008FE44[2];
    }
    if (D_8008FE44[3] != -1) {
        playerMask |= 8;
        D_80084060[3] = D_8008FE44[3];
        D_8008FD88++;
    }
    for (i = 4; i < 16; i++) {
        if (D_8008FE44[i] != -1) {
            playerMask |= 1 << i;
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
        } else { // matching: the empty else changes IDO's branch layout (16 bytes)
        }
        for (i = v; i < 16; i++) {
            D_80087270[i] = entry->unk8;
            if (D_8008FDD4->selection == 7) {
                if (D_8008FE44[i] < -1) {
                    D_80087270[i] = 11;
                }
            } else if (D_8008FDD4->selection == 2) {
                if (D_800E0C00[i] == 0) {
                    D_80087270[i] = 11;
                }
            } else if (D_8008FDD4->selection == 8) {
                if (D_800E0C00[i] != 0) {
                    D_80087270[i] = 3;
                } else {
                    D_80087270[i] = (func_150ADA20() & 0xF) + 17;
                }
            } else if ((D_800E0C00[i] != 0) && (entry->unk8 == 4)) {
                D_80087270[i] = 3;
            } else if ((D_8008FE44[i] < -1) && (entry->unk8 == 11)) {
                D_80087270[i] = (i & 3) + 12;
            }
        }
    }
    D_800E0B99 = 2;
    D_8008FD8C = D_8008FD88;
    D_8008FD90 = 0;
    D_800E0AA8 = 0;
    for (i = 0; i < D_8008FD88; i++) {
        if (D_8008FE44[i] >= 0) {
            D_8008FD90++;
            D_800E0AA8 |= 1 << D_800E0C00[i];
        }
    }
    D_8008FD94 = D_8008FD90;
    D_800BE618 = D_8008FD8C;
    bzero(D_800E0BB8, 0x10);
    D_800E0AAA = -1;
    D_800E0AA9 = 4;
    if (1) {} // matching: lowers the priority of the -1 web so IDO splits it like the target
    for (i = 0; i < 16; i++) {
        D_800E0AB0[i] = -1;
        D_800E0AC0[i] = 0;
    }
    usedTeams = 0;
    if (D_8008FDC8 != 0) {
        k = 0;
        D_800E0AA9 = D_8008FD8C;
        for (i = 0; i < D_8008FD88; i++) {
            v = D_8008FE44[i];
            if (v >= 0) {
                D_800E0C00[i] = v;
                usedTeams |= 1 << D_800E0C00[i];
            } else {
                while (usedTeams & (1 << k)) {
                    k++;
                }
                D_800E0C00[i] = k;
                usedTeams |= 1 << k;
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
    D_800E0C20 = 0;
    D_800D2E40 = 0;
    D_8008FD7C = D_80087270[0];
    func_1501C730(6, param, 0, (D_8008FF38 - 1)[D_8008FD88], playerMask);
}

/* Sets up a single-player scene from resource table entry D_8008FF04 (or the preset D_8008FDAC). */
void func_151E30C4(void) {
    s32 resourceCount;
    s32 resource;
    s32 unk0;

    unk0 = 0;
    if (D_8008FDB4 != 0) {
        resource = D_8008FDAC;
    } else {
        func_1509CF28(D_8008FF00, &resourceCount);
        func_1509D08C(D_8008FF00, D_8008FF04, &resource, &unk0);
        func_1509D054();
    }

    func_15017790();
    func_1509CC94(resource);
    func_1509CDDC(resource);
    D_800BE9F4 = func_1509CA30(resource);
    func_15085710(0, 9, unk0);
    D_800BE3E4 = unk0;

    D_800DD2C0[0] = func_1509CA50(resource);
    if (D_8008FDB4 == 2) {
        D_800DD2C0[0] = D_800E0BD1;
    }

    D_8008FD70 = 0;
    D_800D23A8 = 0;
    D_8008FDBC = 0;
    D_8008FDA8 = -1;
    D_800E0B94 = 0;
    D_800E0B99 = 1;
    D_8008FD8C = 1;
    D_8008FD90 = 1;
    D_8008FD94 = 1;

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
    func_1501C730(6, D_800BE9F4, func_1509CA50(resource), 0, 1);
}

/* Resets to a one-player session using the cue D_800BE3DF/D_800BE3E0. */
void func_151E327C(void) {
    D_80087260 = 3;
    if (D_800E9D00 & 0x200) {
        D_80087260 = 0x33;
    }

    D_80087270[0] = 0;
    D_8008FD8C = 1;
    D_8008FD90 = 1;
    D_8008FD94 = 1;
    D_8008FDBC = 0;
    D_800BE618 = 1;
    D_800D23A8 = 0;
    D_800E0B94 = 0;
    D_800D2E40 = 0;
    func_1501C730(6, D_800BE3DF, D_800BE3E0, 0, 1);
    D_800E0B99 = 1;
    D_800E0BB1 = 1;
    func_151E7EF8();
}

extern s8 D_800AB64C[];
extern s16 D_800E0A82;
extern s8 D_800E0A84;
extern char D_800AB89C[];
extern char D_800AB8A0[];
extern char D_800AB8A4[];
extern char D_800AB8A8[];
extern char D_800AB8AC[];
extern char D_800AB8B0[];
extern char D_800AB8B4[];
extern char D_800AB8B8[];
extern char D_800AB8BC[];
extern char D_800AB8C4[];
extern char D_800AB8C8[];

/*
 * Draws the option menu (D_800E0BE0 values described by D_800AB64C) for the options in optionMask,
 * either as a list or as a rotating carousel, and handles the d-pad to change the selection and values.
 */
void func_151E3344(s32 optionMask, s32 carousel, s32 showArrows, u8 alpha, s8 fade) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 row;
    s32 count;
    s32 halfCount;
    s32 slot;
    s32 rows;
    s32 rowStep;
    s32 ypos;
    s32 descOff;
    s32 value;
    s32 k;
    s32 kind;
    s32 selRow;
    s32 drawAlpha;
    f32 xs[15];
    f32 ys[14];
    f32 scrollT;
    s8 fadeValue;
    s32 prevSel;
    s32 prevValue;
    s32 forward;
    s32 arrowX;
    s32 arrowY;

    // matching: these dead stores create uopt spill temps in the order the target's stack slots need
    value = (s16)x;
    kind = D_800AB64C[descOff + 1];
    arrowX = alpha;
    count = 0;
    for (i = 0, j = 1; i < 23; i = j++) { // matching: j = i + 1 induction var, as in the draw loop
        if ((1 << i) & optionMask) {
            count++;
        }
    }
    slot = halfCount = count >> 1;
    if (showArrows && D_8008FDE8 != 0.0f) {
        D_8008FDD4->input &= ~3;
    }
    ypos = 70;
    if (carousel) {
        rows = halfCount;
        if (count >= 4 && (count & 1)) {
            rows = halfCount + 1;
        }
        rowStep = 130 / (rows + 1);
        if (count >= 4) {
            rowStep -= 5;
        }
        for (i = 0; i < count; i++) {
            j = i + 1;
            ys[i + 1] = ypos;
            if (i == 0) {
                xs[j] = 148.0f;
                ypos += rowStep;
            } else if (i < halfCount) {
                xs[j] = 222.0f;
                ypos += rowStep;
                if (i + 1 != halfCount || (count & 1)) {
                    ypos += 16;
                }
            } else if (i == halfCount) {
                if (count & 1) {
                    xs[j] = 222.0f;
                } else {
                    xs[j] = 148.0f;
                    ypos -= rowStep;
                }
            } else {
                ypos -= rowStep;
                ypos -= 16;
                xs[j] = 74.0f;
            }
        }
        ypos = 70;
        xs[0] = xs[count];
        ys[0] = ys[count];
        xs[count + 1] = xs[1];
        ys[count + 1] = ys[1];
        xs[count + 2] = xs[2];
        ys[count + 2] = ys[2];
        scrollT = sinf(D_8008FE38 * 3.141590118f - 1.570795059f) * 0.5f + 0.5f;
        slot = 0;
        for (i = 0; i < 23; i++) {
            if (i < D_8008FE34 && ((1 << i) & optionMask)) {
                slot++;
            }
        }
        if (slot >= count) {
            slot = 0;
        }
    }
    value = (u8)drawAlpha; // matching: dead store, orders spill temps (see function top)
    if (fade) {
        D_800E0A82 += 0x5C;
        if (D_800E0A82 > 255) {
            D_800E0A82 = 0xFF;
        }
        drawAlpha = D_800E0A82;
        if (drawAlpha < 0) {
            drawAlpha = -drawAlpha;
        }
        drawAlpha = (alpha * drawAlpha) >> 8;
        if (drawAlpha >= 0xFE) {
            drawAlpha = 0xFF;
        }
        if (D_800E0A82 < 0) {
            fadeValue = D_800E0A84;
        } else {
            fadeValue = D_800E0BE0[D_8008FE34];
        }
    } else {
        D_800E0A82 = 0xFF;
        drawAlpha = alpha;
    }
    selRow = 0;
    descOff = 0;
    row = 0;
    for (i = 0, j = 1, k = 0; i < 23; i = j++, k++) { // matching: k (== i) is IDO's separate index var
        kind = D_800AB64C[descOff + 1];
        if ((1 << i) & optionMask) {
            value = D_800E0BE0[k];
            if (i == D_8008FE34) {
                selRow = row;
            }
            if (carousel) {
                x = func_150497E0(xs, slot, scrollT);
                y = func_150497E0(ys, slot, scrollT);
                slot--;
                if (slot < 0) {
                    slot += count;
                }
            } else {
                y = ypos;
                x = 148;
                ypos += 34;
            }
            func_1504332C(0xFF, 0xFF, 0xFF, drawAlpha);
            func_150432FC(x, y);
            if (D_800AB64C[descOff] >= 0) {
                func_15042E3C(D_800E0BD8[D_800AB64C[descOff]]);
            } else if (D_800AB64C[descOff] < -1) {
                func_150432FC(x, y + 9);
                func_15042E3C(D_800AB89C, -D_800AB64C[descOff]);
                func_150432FC(x, y + 22);
            }
            if (i == D_8008FE34 && D_8008FE38 <= 0.0f) {
                func_1504332C(0xFF, 0xFF, 0xFF, drawAlpha);
                if (fade) {
                    value = fadeValue;
                }
            } else if (fade) {
                func_1504332C(0xFF, 0xFF, 0xFF, alpha);
            } else {
                func_1504332C(0xFF, 0xFF, 0xFF, alpha >> 2);
            }
            if (kind < 0) {
                if (kind < -1) {
                    func_15042E3C(D_800AB8A0, value - kind);
                }
            } else {
                switch (kind) {
                    case 0x7F:
                        func_15042E3C(D_800E0BD8[value != 0]);
                        break;
                    case 0x7E:
                        func_15042E3C(D_800AB8A4, value);
                        break;
                    case 0x7A:
                        if (value == 0) {
                            func_15042E3C(D_800AB8A8, D_800E0BD8[0x73]);
                        } else {
                            func_15042E3C(D_800AB8AC, value);
                        }
                        break;
                    case 0x7B:
                        if (D_800E0BE0[3] >= 3) {
                            func_15042E3C(D_800AB8B0, D_800E0BD8[45]);
                        } else {
                            func_15042E3C(D_800AB8B4, value);
                        }
                        break;
                    case 0x7C:
                        func_15042E3C(D_800AB8B8, value * 1000);
                        break;
                    case 0x7D:
                        if (value >= 3) {
                            func_15042E3C(D_800AB8BC, value, D_800E0BD8[0x72]);
                        } else {
                            func_15042E3C(D_800E0BD8[0x73]);
                        }
                        break;
                    default:
                        func_15042E3C(D_800E0BD8[D_800AB64C[descOff + 1 + value]]);
                        break;
                }
            }
            row++;
        }
        switch (kind) {
            case 0x7A:
            case 0x7B:
            case 0x7C:
            case 0x7D:
            case 0x7E:
            case 0x7F:
                descOff += 2;
                break;
            default:
                if (kind >= 0) {
                    descOff += D_8008FE84[k] + 2;
                } else {
                    descOff += 2;
                }
                break;
        }
        if (D_800E0BE0[k] < D_8008FE6C[k]) {
            D_800E0BE0[k] = D_8008FE6C[k];
            D_8008FDE0 = 1;
        }
        if (D_8008FE84[k] < D_800E0BE0[k]) {
            D_800E0BE0[k] = D_8008FE84[k];
            D_8008FDE0 = 1;
        }
    }
    descOff = D_8008FE34;
    forward = 1;
    if (D_800E0A82 != 0xFF) {
        D_8008FDD4->input = 0;
    }
    prevSel = descOff;
    prevValue = D_800E0BE0[descOff];
    if (D_8008FE38 <= 0.0f) {
        i = 0;
        if (carousel) {
            if (D_8008FDD4->input & 1) {
                D_800E0BE0[descOff]++;
                D_8008FDE0 = 1;
                i = 1;
            } else if (D_8008FDD4->input & 2) {
                D_800E0BE0[descOff]--;
                D_8008FDE0 = 1;
                i = 1;
            } else if (D_8008FDD4->input & 8) {
                D_8008FE3C = 0.07500000298f;
            } else if (D_8008FDD4->input & 4) {
                descOff--;
                if (descOff < 0) {
                    descOff += 23;
                }
                D_8008FE3C = -0.07500000298f;
                forward = 0;
                D_8008FE38 = 1.0f;
            }
            if (i && descOff == 2) {
                D_800E0BE0[3] = 2;
            }
        } else {
            if (D_8008FDD4->input & 4) {
                D_800E0BE0[descOff]--;
                D_8008FDE0 = 1;
                i = 1;
            } else if (D_8008FDD4->input & 8) {
                D_800E0BE0[descOff]++;
                D_8008FDE0 = 1;
                i = 1;
            } else if (D_8008FDD4->input & 2) {
                D_8008FE3C = 1.0f;
            } else if (D_8008FDD4->input & 1) {
                descOff--;
                if (descOff < 0) {
                    descOff += 23;
                }
                forward = 0;
                D_8008FE38 = 0.0f;
            }
            if (i && descOff == 2) {
                D_800E0BE0[3] = 2;
            }
        }
        if (D_8008FDD4->input & 0xF) {
            func_10010F30(0x4FF, 0x4650, 0x40, 0, 0);
        }
        if (D_8008FE6C[descOff] > D_800E0BE0[descOff]) {
            D_800E0BE0[descOff] = D_8008FE84[descOff];
            D_8008FDE0 = 1;
        }
        if (D_8008FE84[descOff] < D_800E0BE0[descOff]) {
            D_800E0BE0[descOff] = D_8008FE6C[descOff];
            D_8008FDE0 = 1;
        }
    }
    D_8008FE38 += D_8008FE3C * D_800BE9E4;
    if (D_8008FE38 >= 1.0f) {
        descOff++;
        if (descOff >= 23) {
            descOff -= 23;
        }
        D_8008FE3C = 0.0f;
        D_8008FE38 = 0.0f;
    } else if (D_8008FE38 <= 0.0f) {
        D_8008FE3C = 0.0f;
        D_8008FE38 = 0.0f;
    }
    while (forward && !((1 << descOff) & optionMask)) {
        descOff++;
        if (descOff >= 23) {
            descOff -= 23;
        }
    }
    while (!forward && !((1 << descOff) & optionMask)) {
        descOff--;
        if (descOff < 0) {
            descOff += 23;
        }
    }
    if (descOff == prevSel && prevValue != D_800E0BE0[descOff]) {
        D_800E0A82 = -0xFF;
        D_800E0A84 = prevValue;
    }
    D_8008FE34 = descOff;
    if (showArrows) {
        arrowX = 100;
        arrowX = (arrowX + D_8008FDE8 * (selRow * 0)) * 0.5f; // matching: int const start + `* 0` fixes FP operand order
        arrowY = (f32)(D_8008FDE4 * 34 + 70) + D_8008FDE8 * ((selRow - D_8008FDE4) * 34);
        arrowX += 20.0f * sinf(D_8008FDE8 * 3.141499996f);
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(arrowX + 148, arrowY, 1, D_800AB8C4, 0x1A);
        func_15042D94(148 - arrowX, arrowY, 1, D_800AB8C8, 0x1A);
        if (selRow != D_8008FDE4) {
            D_8008FDE8 += 0.1000000015f * D_800BE9E4;
            if (D_8008FDE8 >= 1.0f) {
                D_8008FDE4 = selRow;
                D_8008FDE8 = 0.0f;
            }
        }
    }
}

/* State 5: skips one frame after the intro cue, then fades out into func_151E2834 on input (or unless D_800C35EA == 1). */
void func_151E4264(void) {
    if (D_8008FD80 != 0) {
        D_8008FD80 = 0;
        return;
    }

    func_151E530C();
    if ((D_800E0B9A != 0) || (D_800C35EA[0] != 1)) {
        if ((D_800C35EA[0] == 1) && (func_151F2CDC() == 1)) {
            func_151F2D6C(0, 0x2DE0);
        }
        D_800E0B94 = 7;
        D_800E0A88 = func_151E2834;
        D_8008FD74 = 8;
    }
}

/* Plays the mode's intro cue (entry unk6) and waits in state 5, or sets up the match directly. */
void func_151E4314(void) {
    struct Struct151E4314 *entry;
    s32 cueIdx;
    s32 param;

    cueIdx = D_800AB68C[D_8008FDD4->selection].unk6;
    if (cueIdx >= 0) {
        entry = &D_800AB7A4[cueIdx];
        if (D_800E0C00[0]) {
            param = entry->unk3;
        } else {
            param = entry->unk2;
        }
        func_1501D348(entry->unk0, param, 0, 0, 0);
        D_8008FD80 = 1;
        D_800E0B94 = 5;
        D_8008FD8C = 1;
        D_8008FD90 = 1;
    } else {
        func_151E2834();
    }
}
extern s8 D_800BE91A;
extern char D_800AB8CC[];
extern char D_800AB8D0[];
extern char D_800AB8D4[];
extern char D_800AB8E0[];
extern char D_800AB8E4[];
extern char D_800AB8E8[];
extern char D_800AB8EC[];
extern char D_800AB8F0[];
extern char D_800AB8F4[];
extern char D_800AB8F8[];
extern char D_800AB8FC[];
extern char D_800AB900[];
extern char D_800AB904[];
extern struct Struct151E4314 D_800AB7BC[];

/* State 6: results screen; plays the outcome cue, draws ranks and the continue/quit choice. */
void func_151E43DC(void) {
    static s8 D_8008FF3C = 0;
    s32 idx;
    s32 param;
    s32 unused1; // matching: unused locals keep uopt's variable numbering
    s32 i;
    s32 y;
    s32 count;
    s8 n;
    s8 cur;
    s8 unused2;
    s32 j;

    if (D_8008FD80 == 1) {
        idx = D_800AB68C[D_8008FDD4->selection].unk7;
        param = D_800AB7BC[idx].unk2;
        if (D_8008FDD4->selection == 0) {
            count = 1;
            for (i = 0; i < D_8008FD88; i++) {
                if (D_8008FE44[i] >= 0 && D_800E0AC0[D_800E0C00[i]] == 1) {
                    count = 0;
                }
            }
            if (count) {
                param = D_800AB7BC[idx].unk3;
            }
        } else if (D_800E0AC0[0] != 1) {
            param = D_800AB7BC[idx].unk3;
        }
        func_1501D348(D_800AB7BC[idx].unk0, param, 0, 0, 0);
        D_8008FD80 = 0;
    } else {
        if (D_8008FDD4->selection != 11 && D_8008FDD4->selection != 12) {
            func_151E530C();
        }
        if (D_8008FD80 == 2 || D_8008FD80 == 3 || D_8008FD80 == 4) {
            D_8008FDCC += D_800BE9E4 * 4;
            if (D_8008FDCC > 255) {
                D_8008FDCC = 0xFF;
            }
            D_800E0BDC = 0xFF - D_8008FDCC;
            func_1504332C(0xFF, 0xFF, 0, D_8008FDCC);
            func_15042D94(148, 20, 0x81, D_800AB8CC, 0x4B);
            func_15042E3C(D_800AB8D0);
            func_1504332C(0xFF, 0xFF, 0xFF, (u8)D_8008FDCC);
            func_150432FC(148, 50);
            if (D_8008FD80 == 4) {
                camera_global_Cameras->Mode = 0x80000;
            }
            if (D_800AB68C[D_8008FDD4->selection].unk5 < 0) {
                if (D_8008FDD4->selection == 11 || D_8008FDD4->selection == 12) {
                    y = 0x48;
                    for (i = 0; i < D_8008FD88; i++) {
                        if (D_8008FE44[i] >= 0) {
                            count = D_800E0AC0[i];
                            func_15042D94(88, y, 0x81, D_800AB8D4, D_800E0BD8[count + 0x5D]);
                            func_150916B4(0x8A, y, D_800E0C18[i], 0x80);
                            func_150916B4(0xBC, y, D_800E0C10[i], 0x80);
                            func_15042D94(168, y, 0x80, D_800AB8E0);
                            func_15042D94(214, y, 0x80, D_800AB8E4);
                            y += 0x12;
                        }
                    }
                }
            } else {
                count = 0;
                for (i = 0; i < D_8008FD88; i++) {
                    if (D_8008FE44[i] >= 0) {
                        count++;
                    }
                }
                n = 0;
                for (i = 0; i < 4; i++) {
                    if (D_800E0AC0[i] == 1) {
                        n++;
                    }
                }
                n = n >= 2;
                if (n) {
                    func_15042D94(148, 50, 0x81, D_800AB8E8, 0x58);
                } else if (count == 1) {
                    func_15042D94(118, 50, 0x81, D_800AB8EC, 0x52);
                    count = (D_800E0AC0[D_800E0C00[0]] == 1) ? 0x56 : 0x57;
                    func_15042D94(178, 50, 0x81, D_800AB8F0, count);
                } else {
                    for (i = 0; i < 4; i++) {
                        if (D_800E0AC0[i] == 1) {
                            func_15042D94(113 - ((D_800E0BC8[i] == 4) ? -12 : 0), 50, 0x81, D_800AB8F4, D_800E0BC8[i] + 0x4C);
                            func_15042D94(((D_800E0BC8[i] == 4) ? 21 : 0) + 183, 50, 0x81, D_800AB8F8, 0x56);
                            i = 4; // matching: ends the loop; `break` schedules differently
                        }
                    }
                }
            }
            cur = D_800BE91A;
            if ((D_8008FF3C >= -30 && cur < -30) || (D_8008FF3C < 31 && cur >= 31)) {
                D_8008FE2C = 1 - D_8008FE2C;
                func_10010F30(0x62D, 0x4650, 0x40, 0, 0);
            }
            D_8008FF3C = cur;
            func_15042D94(83, 180, 1, D_800AB8FC, (D_8008FE2C == 0) + 0x21);
            func_15042D94(213, 180, 1, D_800AB900, (D_8008FE2C == 1) + 0xE);
            func_15042D94(148, 180, 1, D_800AB904, 0x1A);
            if ((D_800E0B9A & 0x8000) || D_800E0A90 > 2400) {
                if (D_800C35EA[0] == 1 && func_151F2CDC() == 1) {
                    func_151F2D6C(0, 0x2DE0);
                }
                if (D_800E0A90 > 2400) {
                    D_8008FE2C = 1;
                }
                D_800E0B94 = 7;
                if (D_8008FE2C == 0) {
                    if (D_8008FD70 != 0) {
                        for (j = D_8008FD70; j < 0x10 - D_8008FD70; j++) {
                            i = j + D_8008FD70;
                            D_8008FE44[j] = D_8008FE44[i];
                            D_800E0C00[j] = D_800E0C00[i];
                        }
                        D_8008FD70 = 0;
                    }
                    D_800E0A88 = func_151E2834;
                } else {
                    D_8008FDD4->page = 4;
                    if (D_8008FD80 == 3) {
                        D_800E0B94 = 3;
                    } else {
                        D_800E0A88 = func_151E4E00;
                    }
                }
                D_8008FD74 = 8;
                D_8008FD80 = 0;
                func_151E557C();
                D_8008FD8C = 1;
                D_8008FD90 = 1;
            }
        } else if (D_800E0B9A != 0 || D_800C3C8C == 2) {
            D_8008FDCC = 0;
            D_8008FD80 = 2;
        }
        D_8008FDD4->input &= 0xFFDF;
    }
}

extern s8 D_800E0A86;

/* End of round: picks the results screen state and cue from the mode entry (unk7) and the ranks. */
void func_151E4BD8(void) {
    s8 resultCue;
    s8 count;
    s16 param;
    s32 i;

    D_8008FE2C = 0;
    D_800BEAC1 = 0;
    D_8008FDA4 = 0;
    D_800E0A90 = 0;
    count = 0;
    resultCue = D_800AB68C[D_8008FDD4->selection].unk7;
    D_8008FDC0 &= 0xF369;
    if (D_800AB68C[D_8008FDD4->selection].unk5 >= 0) {
        for (i = 0; i < 4; i++) {
            if (D_800E0AC0[i] == 1) {
                count++;
            }
        }
        count = count >= 2;
    }
    if (resultCue < 0 || count != 0) {
        if (resultCue == -2 && D_800E0A86 == 0) {
            D_800E0B94 = 6;
            D_8008FD80 = 2;
            D_8008FDC0 = 0;
            return;
        }
        param = 0x1D;
        if (count == 0) {
            D_800E0B94 = 3;
        } else {
            D_800E0B94 = 6;
            D_8008FD80 = 4;
        }
    } else {
        param = D_800AB7BC[resultCue].unk0;
        D_800E0B94 = 6;
        D_8008FD80 = 1;
    }
    D_800D2E40 = 0;
    func_1501C730(6, param, 0, 0, 1);
    func_151E557C();
    D_8008FD8C = 1;
    D_8008FD90 = 1;
}

/* Switches the menu to state 10. */
void func_151E4DC4(void) {
    D_800E0B94 = 10;
}

/* Switches to state 4 when input bits 0x8020 are set. */
void func_151E4DD8(void) {
    if (D_800E0B9A & 0x8020) {
        D_800E0B94 = 4;
    }
}

/* Returns to state 3 and plays cue 0x1D. */
void func_151E4E00(void) {
    D_8008FDCC = 0;
    func_151E557C();
    D_800E0B94 = 3;
    D_8008FDA4 = 0;
    D_8008FD80 = 0;
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
}

/* Waits for input (or 1200 frames) and then fades out into func_151E4E00. */
void func_151E4E64(void) {
    func_151E530C();
    func_151E55A8();
    if (D_800E0A90 > 1200) {
        D_800E0B9A |= 0x8000;
    }
    if (D_800E0B9A != 0) {
        D_800E0B94 = 7;
        D_800E0A88 = func_151E4E00;
        D_8008FD74 = 8;
    }
}

/* Advances the screen fade D_800E0B96; once fully faded, runs the pending transition D_800E0A88. */
void func_151E4EE8(void) {
    s32 fade;

    D_8008FDD0 = 1.0f - (f32)D_800E0B96 / 255.0f;
    if (D_8008FD74 == 0xFF) {
        if (0.8700000048f < D_800DDDC8[0]) {
            D_800E0B96 += D_800BE9E4;
            if (D_800E0B96 < 0x55) {
                D_800DDDD8[0] = 9.999999975e-07f;
            } else {
                D_800DDDD8[0] = 0.01111111138f;
            }
        }
        if (D_800DDDC8[0] < 1.0f) {
            return;
        }
        if (D_800E0B96 < 0x6E) {
            return;
        }
    } else if (D_800E0B96 != 0xFF) {
        fade = D_800E0B96 + D_800BE9E4 * D_8008FD74;
        if (fade > 255) {
            fade = 0xFF;
        }
        D_800E0B96 = fade;
        return;
    }
    D_800E0B96 = 0xFF;
    D_8008FD74 = 8;
    D_8008FDD0 = 1.0f;
    D_800E0A88();
}

/* Resets the path/selection state in D_8008FDD4. */
void func_151E5034(void) {
    D_8008FDD4->unk0 = 0.0f;
    D_8008FDD4->unk4 = 0.0f;
    D_8008FDD4->unk2B = 0;
    D_8008FDD4->input = 0;
    D_8008FDD4->menu = 0;
    D_8008FDD4->page = 0;
    D_8008FDD4->cursor = 0;
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

extern u8 D_800AB570[];

/* Menu init: default option values, text bank reset, D_8008FDD4 state, then state 11. */
void func_151E50C8(void) {
    s32 i;

    for (i = 0; i < 23; i++) D_800E0BE0[i] = D_8008FE54[i]; // matching: one line (-g3 line numbers steer as1)
    D_800E0A90 = 0;
    func_151E6BFC();
    if (D_8008FDD4 == NULL) {
        D_8008FDD4 = &D_800E0AF0;
        func_151E5034();
        if (D_8008FDD4->menu == 1) {
            D_8008FDD4->unk2C = D_800AB570[D_8008FDD4->unk2C];
        }
    }
    func_15017790();
    D_800E0B94 = 11;
    D_800D2E40 = 0;
    func_1501C730(6, 0x25, 0, 0, 1);
    D_800E0B95 = D_800E0B94;
}

/* Per-frame menu update: reads input, loads the text bank and runs the handler for state D_800E0B94. */
void func_151E51EC(void) {
    u8 prevState;
    s32 timer;

    prevState = D_800E0B94;
    D_8003C8E0 = 0x08000000;
    D_800E0BDC = 0xFF;
    func_15042D78(0x81);

    D_800E0B9A = D_800BE930[0];
    timer = D_8008FE28;
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
        if (D_800E0B94 != prevState) {
            D_800E0B95 = prevState;
            D_800E0A90 = 0;
        }
    }
    D_8003C8E0 = 0;
}

/* Fades the screen in, or derives the fade from the state's unk8 transition. */
void func_151E530C(void) {
    s32 fade;
    MenuState *state;

    if (D_800E0B94 != 2) {
        if (D_800E0B96 != 0) {
            fade = D_800E0B96 - D_800BE9E4 * D_8008FD74;
            if (fade < 0) {
                fade = 0;
            }
            D_800E0B96 = fade;
        }
        state = D_8008FDD4;
        if (state != NULL && state->unk2B == 0 && state->unkC > 0.0f && state->unk8 > 0.5f) {
            fade = (state->unk8 - 0.5f) * 524.0f;
            if (fade > 255) {
                fade = 0xFF;
            }
            D_800E0B96 = fade;
        }
    }
}

/* End of a match: if D_800BE616 is set, records each player's unk1AA and schedules the results transition; else state 4. */
void func_151E53E8(s32 flag) {
    s32 i;

    D_800E0A90 = 0;
    D_8008FE2C = 0;
    D_80087270[0] = 0;
    if (D_800BE616 != 0) {
        D_800E0B94 = 7;
        D_8008FD74 = 8;
        if (D_800AB68C[D_8008FDD4->selection].unk7 != -1) {
            D_8008FDD4->page = 4;
            D_800E0A88 = func_151E4BD8;
        } else {
            D_8008FDD4->page = 3;
            D_800E0A88 = func_151DDBA0;
        }
        for (i = 0; i < D_8008FD90; i++) {
            D_8008FDD4->stats[i].unk6 = movement_obj[i].mv->shotsfired;
        }
        D_8008FDC0 &= 0xE35F;
        D_800E0A86 = flag;
        if (D_800AB68C[D_8008FDD4->selection].unk7 == -2) {
            if (flag == 0) {
                func_151E4BD8();
                return;
            }
            D_800E0A88 = func_151DDBA0;
            D_8008FDD4->page = 3;
        }
    } else {
        D_800E0B94 = 4;
    }
}

/* Resets the player-to-controller map D_80084060 to identity. */
void func_151E557C(void) {
    D_80084060[0] = 0;
    D_80084060[1] = 1;
    D_80084060[2] = 2;
    D_80084060[3] = 3;
}

/* Ping-pongs the pulse level D_800E0B97 between 0 and 255. */
void func_151E55A8(void) {
    s32 intensity;
    s32 step;
    s32 direction;

    direction = D_800E0B98;
    if (direction == 0) {
        step = D_800BE9E4 * 8;
    } else {
        step = -D_800BE9E4 * 8;
    }
    intensity = D_800E0B97;
    intensity += step;
    if (intensity > 255) {
        intensity = 0x1FE - intensity;
        D_800E0B98 = direction ^ 1;
    } else if (intensity < 0) {
        intensity = -intensity;
        D_800E0B98 = direction ^ 1;
    }
    D_800E0B97 = intensity;
}

/* Clears D_800E0A8C. */
void func_151E562C(void) {
    if (D_800E0A8C != 0) {
        D_800E0A8C = 0;
    }
}

/* Returns D_8008FDC8: set when the mode entry's unk5 is negative (every player then gets a team of their own). */
s8 func_151E564C(void) {
    return D_8008FDC8;
}

/*
 * Checks whether the match is over (score/time limits, players or teams left) and computes the
 * rank of every player (or team) into D_800E0AC0. Caches and returns the result in D_800E0C20.
 */
s32 func_151E565C(void) {
    s32 i;
    s32 j;
    s32 *pi; // matching: pi/pj are unused but keep the frame slots
    s32 k;
    s32 team;
    s32 hits;
    s16 winScore;
    s16 score;
    s16 other;
    s8 liveTeams;
    s32 gameOver;
    s32 teamStats[16];
    s32 statBase;
    s32 *pj;

    gameOver = 0;
    if (D_800E0C20 != 0) {
        return 1;
    }
    if (D_800E0BE0[3] >= 3 && D_8008FD78 <= 0) {
        D_800E0AAA = -1;
        gameOver = 1;
    }
    if (D_8008FDC8 != 0) {
        winScore = -1;
        for (i = 0; i < D_8008FD8C; i++) {
            if (D_800E0BE0[6] != 0 && D_8008FDD4->score[i] >= D_800E0BE0[6]) {
                gameOver = 1;
                winScore = D_800E0BE0[6];
            }
            if (D_800D18A0 & (1 << i)) {
                if (D_800E0AB0[i] == -1) {
                    D_800E0AB0[i] = D_800E0AA9--;
                }
            }
        }
        if (D_800BE618 - D_8008FD70 < 2 || D_8008FD94 - D_8008FD70 <= 0) {
            gameOver = 1;
        }
        D_800E0AAA = -1;
        if (D_8008FDC0 & 0x80) {
            for (i = 0; i < D_8008FD90; i++) {
                if (D_800E0AB0[i] != -1) {
                    D_800E0AC0[i] = D_800E0AB0[i];
                } else {
                    if (D_800872A0[i].finishOrder == 0x7F) {
                        D_800E0AC0[i] = D_800872A0[i].rank;
                    } else {
                        D_800E0AC0[i] = D_800872A0[i].finishOrder;
                    }
                }
            }
        } else {
            for (i = 0; i < D_8008FD90; i++) {
                if (D_80087270[i] != 10) {
                    if (D_800E0AB0[i] != -1) {
                        D_800E0AC0[i] = D_800E0AB0[i];
                    } else {
                        D_800E0AC0[i] = 1;
                        score = D_8008FDD4->score[i];
                        if (score >= winScore) {
                            score += 1000;
                        }
                        score -= D_800E0AD0[i];
                        for (j = 0; j < D_8008FD8C; j++) {
                            if (j != i && D_800E0AB0[j] == -1 && D_80087270[j] != 10) {
                                other = D_8008FDD4->score[j];
                                if (other >= winScore) {
                                    other += 1000;
                                }
                                other -= D_800E0AD0[j];
                                if (score < other) {
                                    D_800E0AC0[i]++;
                                } else if (score == other) {
                                    if (movement_obj[j].mv->shotsfired < movement_obj[i].mv->shotsfired) {
                                        D_800E0AC0[i]++;
                                    } else if (movement_obj[i].mv->shotsfired == movement_obj[j].mv->shotsfired) {
                                        if (j < i && D_80082FA0 < j) {
                                            D_800E0AC0[i]++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        D_800E0C20 = gameOver;
        return gameOver;
    }

    for (k = 0; k < 16; k++) {
        teamStats[k] = 0;
    }
    k = 0; // matching: k is 0 here; teamStats[i + k] below shares CSE temps like the target
    for (i = 0; i < D_8008FD8C; i++) {
        team = movement_obj[i].team; // team: first byte of an unnamed Actor field
        teamStats[team] += D_8008FDD4->score[i];
        teamStats[team + 4] += func_150859AC(i, 3) * 6 + func_150859AC(i, 4);
        teamStats[team + 8] = func_150859AC(team, 6);
        teamStats[team + 12] += movement_obj[i].mv->shotsfired;
    }
    statBase = 8;
    team = -1;
    switch (D_8008FDD4->selection) {
        case 0:
            team = D_800E0BE0[4];
            break;
        case 2:
        case 8:
        case 10:
            team = D_800E0BE0[0x12];
            break;
        case 9:
            statBase = (D_800E0BE0[3] < 3) ? 4 : 0;
            break;
    }
    if (team > 0) {
        for (i = 0; i < D_800E0BB0; i++) {
            j = teamStats[i + statBase];
            if (j >= team) {
                gameOver = 1;
                D_800E0AAA = i;
            }
        }
    }
    func_150432FC(20, 20);
    for (i = 0; i < D_800E0BB0; i++) {
        if (D_800E0AB0[i] != -1) {
            D_800E0AC0[i] = D_800E0AB0[i];
            continue;
        }
        D_800E0AC0[i] = 1;
        for (j = 0; j < D_800E0BB0; j++) {
            if (j != i && D_800E0AB0[j] == -1) {
                if (teamStats[i + statBase] < teamStats[j + statBase]) {
                    D_800E0AC0[i]++;
                } else if (teamStats[i + statBase] == teamStats[j + statBase]) {
                    if (teamStats[i + k] < teamStats[j + k]) {
                        D_800E0AC0[i]++;
                    } else if (teamStats[i + k] == teamStats[j + k]) {
                        if (teamStats[j + 12] < teamStats[i + 12]) {
                            D_800E0AC0[i]++;
                        }
                    }
                }
            }
        }
    }
    liveTeams = 0;
    j = 0;
    for (i = 0; i < D_8008FD8C; i++) {
        if (D_80087270[i] != 10) {
            team = movement_obj[i].team;
            if (!(D_800D18A0 & (1 << i))) {
                liveTeams |= 1 << team;
            }
        }
    }
    hits = 0;
    for (i = 0; i < 4; i++) {
        if (liveTeams & (1 << i)) {
            j++;
            if (D_800E0AA8 & (1 << i)) {
                hits++;
            }
        } else if (D_800E0AB0[i] == -1) {
            D_800E0AB0[i] = D_800E0AA9--;
        }
    }
    if (hits == 0 || j < 2) {
        gameOver = 1;
        D_800E0AAA = -1;
    }
    D_800E0C20 = gameOver;
    return gameOver;
}

/* Returns the team of player during a match (0 if none), else player itself. */
s32 func_151E5F64(s32 player) {
    s32 team;

    if ((D_800BE616 != 0) || (D_800E0B94 != 0)) {
        team = D_800E0C00[player];
        if (team < 0) {
            team = 0;
        }
        return team;
    }
    return player;
}

/* Returns D_8008FD8C when D_800E0BEB is set and it is below 5, else D_8008FD90. */
s8 func_151E5FAC(void) {
    if (D_800E0BE0[11] != 0) {
        if (D_8008FD8C >= 5) {
            return D_8008FD90;
        }
        return D_8008FD8C;
    }
    return D_8008FD90;
}

/*
 * Records a hit by source (or the player owning it) on victim: per-player hit/kill tallies in D_8008FDD4,
 * then picks a character-specific voice line for the attacker.
 */
void func_151E5FF4(Actor *source, Actor *victim, s32 cause) {
    Actor *attacker;
    s32 pad50; // matching: unused stack slot
    s32 attackerIdx;
    s32 victimIdx;
    s32 sound;
    s32 delay;
    s32 variants;
    s32 sourceState;
    s32 sourceState2;
    s32 roll;
    u8 victimKind;
    s32 health;

    attacker = source;
    attackerIdx = source - movement_obj;
    if (attackerIdx >= D_8008FD8C) {
        attackerIdx = source->owner - 1;
        if (attackerIdx >= 0) {
            attacker = &movement_obj[attackerIdx];
        }
    }
    if ((attackerIdx >= 0) && (attackerIdx < D_8008FD8C) && (attackerIdx < D_8008FD8C) /* matching: repeated test */) {
        victimIdx = victim - movement_obj;
        D_800E0BB8[victimIdx] = attackerIdx | 0x80;
        if (attackerIdx < D_8008FD90) {
            if (D_800E0C00[attackerIdx] != D_800E0C00[victimIdx]) {
                D_8008FDD4->stats[attackerIdx].unk0++;
                if (cause == 0x2E) {
                    D_8008FDD4->stats[attackerIdx].unk4++;
                }
            }
            if ((victim->energy <= 0) && (D_800E0C00[attackerIdx] == D_800E0C00[victimIdx]) && (cause != 0x2D)) {
                D_8008FDD4->stats[attackerIdx].unkA++;
            }
        }
        if (victim->energy <= 0) {
            if ((D_8008FDD4->score[attackerIdx] < 9999) && (D_800E0C00[attackerIdx] != D_800E0C00[victimIdx])) {
                D_8008FDD4->score[attackerIdx]++;
                if (D_8008FDBC & 0x200) {
                    func_15085710(D_800E0C00[attackerIdx] != 0, 10, 1);
                }
            }
        }
        if (victimIdx < D_8008FD90) {
            D_8008FDD4->stats[victimIdx].unk8++;
        }
        if ((attacker != victim) && (victim->mv != NULL) && (attacker->energy != 0)) {
            sound = 0;
            delay = 0;
            attacker->mv->unkAF = 0;
            variants = 0;
            if (cause == 0x33) {
                victimKind = victim->mv->rt_context_b;
                if ((victimKind == 0x21) || (victimKind == 0x22)) {
                    func_10010630(0x510, victim, 0x61A8, 0x12C, 0x190);
                }
            } else if ((attacker->character_type == 0x75) && (victim->energy <= 0)) {
                if (attacker->character_type == victim->character_type) {
                    if ((victimIdx < D_8008FD90) && (D_8008FDD4->stats[victimIdx].unkA >= 4)) {
                        sound = 0x665;
                        variants = 4;
                    }
                } else if (cause == 0x34) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52A;
                    }
                } else {
                    sound = 0x4D1;
                    variants = 3;
                }
                if (sound != 0) {
                    if (variants != 0) {
                        sound = func_1000F568(sound, variants);
                    }
                    func_10010558(sound, attacker, 0x6D60, 0x320, 0xBB8, delay);
                }
            } else if (attacker->character_type == 0x80) {
                sourceState = -1;
                if ((source != attacker) && (source->type == 4)) {
                    sourceState = (u16)source->headEffectState;
                }
                if (attacker->character_type == victim->character_type) {
                    if ((victimIdx < D_8008FD90) && (D_8008FDD4->stats[victimIdx].unkA >= 4)) {
                        variants = 8;
                        sound = 0x65A;
                    }
                } else if (sourceState == 2) {
                    if ((victim->energy <= 0) || ((func_150ADA20() & 7) == 0)) {
                        variants = 3;
                        sound = 0x571;
                    }
                } else if (victim->mv->on_fire != 0) {
                    if ((cause != 0x21) && ((victim->energy <= 0) || ((func_150ADA20() & 3) == 3))) {
                        variants = 4;
                        sound = 0x582;
                        delay = 0x1E;
                    }
                } else if (cause == 0x2E) {
                    delay = 0x1E;
                    if ((attacker->mv->rt_context_b == 0x23) && ((func_150ADA20() & 3) == 0)) {
                        sound = 0x587;
                    } else {
                        sound = 0x579;
                        variants = 3;
                    }
                } else if (cause == 0x2F) {
                    if (func_150ADA20() & 1) {
                        sound = 0x588;
                        delay = 0x1E;
                    }
                } else if (cause == 0x2C) {
                    sound = 0x56D;
                    variants = 7;
                    delay = 0x3C;
                } else if (((cause == 0x21) || (cause == 0x20) || (cause == 0x31)) &&
                           ((victim->energy <= 0) || ((func_150ADA20() & 7) == 3))) {
                    sound = 0x571;
                    variants = 3;
                    delay = 0x1E;
                }
                if (sound != 0) {
                    if (variants != 0) {
                        sound = func_1000F568(sound, variants);
                    }
                    func_10010558(sound, attacker, 0x7D00, 0x258, 0x7D0, delay);
                }
            } else if (attacker->character_type == 0x3B) {
                sourceState2 = -1;
                if ((source != attacker) && (source->type == 4)) {
                    sourceState2 = (u16)source->headEffectState;
                }
                if (sourceState2 == 2) {
                    if ((victim->energy <= 0) || ((func_150ADA20() & 7) == 7)) {
                        sound = 0x538;
                        variants = 3;
                    }
                } else if (cause == 0x2E) {
                    sound = 0x52F;
                    variants = 3;
                    delay = 0x1E;
                } else if ((cause == 0x2C) || (cause == 0x2F)) {
                    sound = 0x540;
                    variants = 2;
                    delay = 0x3C;
                } else if ((cause == 0x25) || (cause == 0x26)) {
                    health = victim->energy;
                    if (attackerIdx & 1) {
                        if (health >= 2) {
                            sound = 0x55F - health;
                            delay = 0x1E;
                        }
                    } else {
                        sound = 0x565 - health;
                        delay = 0x1E;
                    }
                } else if (cause == 0x21) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52C;
                        delay = 0x1E;
                    }
                } else if (cause == 0x20) {
                    roll = func_150ADA20() & 0xF;
                    if (roll != 0) {
                        roll++;
                    }
                    if (roll < 4) {
                        sound = roll + 0x52B;
                        delay = 0x1E;
                    }
                }
                if (sound != 0) {
                    if (variants != 0) {
                        sound = func_1000F568(sound, variants);
                    }
                    func_10010558(sound, attacker, 0x7D00, 0x258, 0x7D0, delay);
                }
            } else if ((attacker->character_type == 0x88) && (victim->energy <= 0)) {
                if (cause == 0x34) {
                    if ((func_150ADA20() & 3) == 3) {
                        sound = 0x52A;
                    }
                } else if (cause == 0x21) {
                    sound = 0x41A;
                } else {
                    sound = 0x415;
                    delay = 0x1E;
                    variants = 5;
                }
                if (sound != 0) {
                    if (variants != 0) {
                        sound = func_1000F568(sound, variants);
                    }
                    func_10010558(sound, attacker, 0x6D60, 0x320, 0xBB8, delay);
                }
            }
        }
    }
}


/*
 * Loads text bank (bank - 1) of language D_800E0BEA and indexes its NUL-separated strings into D_800E0BD8;
 * bank 1 also loads bank 2's 80 strings into D_800E0B88. Bank 0 just frees the current one.
 */
void func_151E6964(s32 bank) {
    u8 *cursor;
    u8 *end;
    s32 count;
    s32 size;

    if (D_800E0BE0[10] != D_800BEAAB) {
        D_800BEAAB = D_800E0BE0[10];
        D_800E0BD3 = 0;
    }
    if (bank == D_800E0BD3) {
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
    D_800E0BD3 = bank;
    if (D_800E0BD3 == 0) {
        return;
    }

    D_800E0BD4 = (s32)func_1502B5C8(&size, 3, 0x1C, D_800BEAAB, bank - 1);
    cursor = (u8 *)D_800E0BD4;
    end = cursor + size;
    count = 0;
    do {
        while (*cursor != 0) {
            cursor++;
        }
        while (*cursor == 0) {
            cursor++;
        }
        count++;
    } while (cursor < end);

    D_800E0BD8 = allocate_memory((count + 1) << 2, 1, 0, 0); // matching: a shift, not `* 4` or sizeof
    cursor = (u8 *)D_800E0BD4;
    end = cursor + size;
    count = 0;
    do {
        D_800E0BD8[count] = cursor;
        count++;
        while (*cursor != 0) {
            cursor++;
        }
        while (*cursor == 0) {
            cursor++;
        }
    } while (cursor < end);

    if (bank - 1 != 0) {
        return;
    }

    D_8008FDD8 = (s32)func_1502B5C8(&size, 3, 0x1C, D_800BEAAB, 2);
    D_800E0B88 = allocate_memory(80 * sizeof(u8 *), 1, 0, 0);
    cursor = (u8 *)D_8008FDD8;
    count = 0;
    do {
        D_800E0B88[count] = cursor;
        count++;
        while (*cursor != 0) {
            cursor++;
        }
        while (*cursor == 0) {
            cursor++;
        }
    } while (count != 80);
}

/* Forgets the loaded text banks without freeing them. */
void func_151E6BFC(void) {
    D_800E0BD3 = 0;
    D_800E0BD4 = 0;
    D_8008FDD8 = 0;
}

typedef struct {
    u16 unk0;
    u16 unk2;
} Snd151E6C1C;

u8 D_8008FF40[6] = { 1, 2, 3, 4, 5, 6 };
f32 D_8008FF48[6] = { 0.0085f, 0.05f, 0.03f, 0.03f, 0.03f, 0.03f };
f32 D_8008FF60[6] = { 0.0f, 0.0f, 0.47000003f, 0.5f, 0.5f, 0.5f };
s8 D_8008FF78[6] = { 30, 0, 45, 40, 45, 45 };
s8 D_8008FF80[6] = { 30, 100, 80, 60, 60, 60 };
u8 D_8008FF88[8] = { 17, 18, 19, 20, 21, 22, 23, 24 };
f32 D_8008FF90[8] = { 0.012f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f };
u8 D_8008FFB0[2][6] = { { 1, 0xFF, 4, 6, 8, 3 }, { 0xFF, 0xFF, 0xFF, 0xFF, 9, 5 } };
extern Snd151E6C1C D_800AB908[];
extern Snd151E6C1C D_800AB920[];

#define UNK2AC_F32 (*(f32 *)&obj->focus.z) // an f32; structs.h declares Camera.unk2AC as s32

/*
 * Moves object index along the node path selected by D_8008FDD4 (unk2B = current stop, unk8 = progress
 * along it), spline-interpolating its position (unk2F8..) and look-at point (unk2A4..), and drives the
 * related objects returned by func_151149AC.
 */
void func_151E6C1C(s32 index) {
    Camera *obj;
    s32 pad1A0; // matching: unused stack slot
    s32 idx;
    s32 pathLen;
    s32 homeNode;
    s32 nodeCount;
    f32 *speeds;
    f32 turnT;
    f32 pathT;
    s32 i;
    s32 k;
    f32 scale;
    f32 x[16];
    f32 y[16];
    f32 z[16];
    s8 path[16];
    u8 *nodeTable;
    Snd151E6C1C *sounds;
    WorldObject *r;
    WorldObject *r2;
    PathNode *node;
    PathNode *cur;
    s32 seg;
    u8 link;
    s32 state;
    u8 a;
    f32 cx, cy, cz;
    f32 bx, by, bz;
    f32 ax, ay, az;
    f32 dx, dy, dz;

    obj = &camera_global_Cameras[index];
    if (D_8008FDD4->menu == 0) {
        homeNode = 0;
        nodeCount = 6;
        nodeTable = D_8008FF40;
        speeds = D_8008FF48;
        sounds = D_800AB908;
    } else {
        homeNode = 0x10;
        nodeCount = 8;
        nodeTable = D_8008FF88;
        speeds = D_8008FF90;
        sounds = D_800AB920;
    }
    func_15086CBC(func_15086D48(homeNode), &obj->eye.x, &obj->eye.y, &obj->eye.z);

    if (D_8008FDD4->unk4 != 0.0f) {
        D_8008FDD4->unk0 += D_8008FDD4->unk4 * D_800BE9E4;
        if (D_8008FDD4->unk0 <= 0.0f) {
            D_8008FDD4->unk0 = 0.0f;
            D_8008FDD4->unk4 = 0.0f;
        }
        if (D_8008FDD4->unk0 >= 1.0f) {
            D_8008FDD4->unk0 = 0.0f;
            D_8008FDD4->unk4 = 0.0f;
            D_8008FDD4->unk2B++;
            if (D_8008FDD4->unk2B >= nodeCount) {
                D_8008FDD4->unk2B -= nodeCount;
            }
        }
    } else if (D_8008FDD4->unkC != 0.0f || D_8008FDD4->unk8 != 0.0f) {
        if ((D_8008FDD4->input & 0x20) && (D_8008FDD4->menu == 0 || D_8008FDD4->unk2B != 0)) {
            D_8008FDD4->unkC = -speeds[D_8008FDD4->unk2B];
            func_1000D96C(sounds[D_8008FDD4->unk2B].unk2, D_800E0A97, 0);
            D_800E0A97 = sounds[D_8008FDD4->unk2B].unk2;
        }
        if (D_8008FDD4->unk2A != 0 && D_8008FDD4->menu == 1 && D_8008FDD4->unk2B == 0) {
            if (D_8008FDD4->input & 0x20) {
                D_8008FDD4->unkC = speeds[D_8008FDD4->unk2B];
            }
        } else if (D_8008FDD4->input & 0x10) {
            D_8008FDD4->unkC = speeds[D_8008FDD4->unk2B];
        }
        D_8008FDD4->unk8 += D_8008FDD4->unkC * D_800BE9E4;
        if (D_8008FDD4->unk8 >= 1.0f) {
            D_8008FDD4->unk8 = 1.0f;
            D_8008FDD4->unkC = 0.0f;
        } else if (D_8008FDD4->unk8 < 0.0f) {
            D_8008FDD4->unk8 = 0.0f;
            D_8008FDD4->unkC = 0.0f;
            if (D_8008FDD4->unk2C == 0 && D_8008FDD4->menu == 1 && D_8008FDD4->unk44 != 0) {
                D_8008FDD4->unk43 = 1;
                D_8008FDD4->unk44 = 0;
            }
        }
    } else {
        if (D_8008FDD4->input & 0x10) {
            D_8008FDD4->unkC = speeds[D_8008FDD4->unk2B];
            func_1000D96C(sounds[D_8008FDD4->unk2B].unk0, D_800E0A97, 0);
            D_800E0A97 = sounds[D_8008FDD4->unk2B].unk0;
        } else {
            if (D_8008FDD4->input & 4) {
                D_8008FDD4->unk2B--;
                D_8008FDD4->unk0 = 1.0f;
                D_8008FDD4->unk4 = -0.05000000075f;
                if (D_8008FDD4->unk2B < 0) {
                    D_8008FDD4->unk2B += nodeCount;
                }
            }
            if ((D_8008FDD4->input & 8) || D_8008FDD4->unk43 != 0) {
                D_8008FDD4->unk4 = 0.05000000075f;
            }
            D_8008FDD4->unk43 = 0;
            if (D_8008FDD4->unk2B < 0) {
                D_8008FDD4->unk2B += nodeCount;
            }
            if (D_8008FDD4->unk2B >= nodeCount) {
                D_8008FDD4->unk2B -= nodeCount;
            }
        }
    }

    scale = D_8008FDD4->unk8;
    if (D_8008FDD4->menu == 0) {
        scale *= 1.0f - D_8008FF60[D_8008FDD4->unk2C] * D_8008FDD0;
    } else if (D_8008FDD4->unk2C != 0) {
        scale *= 0.6000000238f;
    }
    turnT = sinf(D_8008FDD4->unk0 * 3.141590118f - 1.570795059f) * 0.5f + 0.5f;
    pathT = sinf(scale * 3.141590118f - 1.570795059f) * 0.5f + 0.5f;

    if (D_8008FDD4->menu == 0) {
        for (i = 0; i < 2; i++) {
            a = D_8008FFB0[i][D_8008FDD4->unk2C];
            if (a != 0xFF) {
                r = func_151149AC(a);
                if (r != NULL) {
                    if (D_8008FDD4->unkC > 0.0f || D_8008FDD0 < 1.0f) {
                        if (D_8008FF78[D_8008FDD4->unk2C] < (s32)(scale * 100.0f)) {
                            r->touch.i.b = 1;
                        }
                    } else if (D_8008FDD4->unkC < 0.0f) {
                        if (D_8008FF80[D_8008FDD4->unk2C] >= (s32)(scale * 100.0f)) {
                            r->touch.i.b = 0;
                        }
                    }
                }
            }
        }
    }

    obj->CharPtr->task_attrib |= 0x200;
    obj->CharPtr->ypos = 1000.0f;

    if (scale > 0.0f) {
        path[0] = func_15086D48(homeNode);
        path[1] = func_15086D48(nodeTable[D_8008FDD4->unk2B]);
        node = &D_800D2350[path[1]];
        pathLen = 1;
        while (node != NULL) {
            cur = node;
            node = NULL;
            for (i = 0; i < 5; i++) {
                link = cur->links[i];
                if (link != 0xFF && link != path[pathLen - 1]) {
                    pathLen++;
                    path[pathLen] = link;
                    node = &D_800D2350[cur->links[i]];
                    i = 5;
                }
            }
        }
        pathLen++;
        if (pathLen >= 4) {
            pathT *= (f32)pathLen - 2.0f;
            seg = pathT;
            turnT = pathT - (f32)seg;
            if (seg >= pathLen - 2) {
                seg = seg - 1;
                turnT = 1.0f;
            }
            // matching: guarded do-while; a plain for loop rotates differently
            i = 1;
            if (pathLen > 0) {
                do {
                    func_15086CBC(path[i - 1], &x[i], &y[i], &z[i]);
                    i++;
                } while (i <= pathLen);
            }
            x[0] = x[1] - (x[2] - x[1]);
            y[0] = y[1] - (y[2] - y[1]);
            z[0] = z[1] - (z[2] - z[1]);
            x[pathLen + 1] = x[pathLen] + (x[pathLen - 1] - x[pathLen]);
            y[pathLen + 1] = y[pathLen] + (y[pathLen - 1] - y[pathLen]);
            z[pathLen + 1] = z[pathLen] + (z[pathLen - 1] - z[pathLen]);
            obj->eye.x = func_150497E0(x, seg, turnT);
            obj->eye.y = func_150497E0(y, seg, turnT);
            obj->eye.z = func_150497E0(z, seg, turnT);
            obj->focus.x = func_150497E0(x, seg + 1, turnT);
            obj->focus.y = func_150497E0(y, seg + 1, turnT);
            UNK2AC_F32 = func_150497E0(z, seg + 1, turnT);
        } else {
            func_15086CBC(path[0], &x[0], &y[0], &z[0]);
            func_15086CBC(path[1], &x[1], &y[1], &z[1]);
            if (pathLen == 3) {
                func_15086CBC(path[2], &x[2], &y[2], &z[2]);
            } else {
                x[2] = x[1] + (x[1] - x[0]);
                y[2] = y[1] + (y[1] - y[0]);
                z[2] = z[1] + (z[1] - z[0]);
            }
            obj->eye.x = x[0] + (x[1] - x[0]) * pathT;
            obj->eye.y = y[0] + (y[1] - y[0]) * pathT;
            obj->eye.z = z[0] + (z[1] - z[0]) * pathT;
            obj->focus.x = x[1] + (x[2] - x[1]) * pathT;
            obj->focus.y = y[1] + (y[2] - y[1]) * pathT;
            UNK2AC_F32 = z[1] + (z[2] - z[1]) * pathT;
        }
    } else {
        idx = D_8008FDD4->unk2B - 1;
        if (idx < 0) {
            idx += nodeCount;
        }
        for (i = 0; i < 4; i++) {
            func_15086CBC(func_15086D48(nodeTable[idx]), &x[i], &y[i], &z[i]);
            idx++;
            if (idx >= nodeCount) {
                idx -= nodeCount;
            }
        }
        obj->focus.x = func_150497E0(x, 0, turnT);
        obj->focus.y = func_150497E0(y, 0, turnT);
        UNK2AC_F32 = func_150497E0(z, 0, turnT);
    }

    if (D_8008FDD4->unk2A != 0) {
        dx = obj->eye.x - obj->focus.x;
        dy = obj->eye.y - obj->focus.y;
        dz = obj->eye.z - UNK2AC_F32;
        obj->focus.x = obj->eye.x + dx;
        obj->focus.y = obj->eye.y + dy;
        UNK2AC_F32 = obj->eye.z + dz;
    }
    obj->focus.x += D_8008FDD4->unk10 * D_8008FDD4->unk14;
    obj->focus.y += D_8008FDD4->unk10 * D_8008FDD4->unk18;
    UNK2AC_F32 += D_8008FDD4->unk10 * D_8008FDD4->unk1C;
    if (D_8008FDD4->unk2A != 3) {
        D_8008FDD4->unk10 *= 0.8500000238f;
    } else {
        D_8008FDD4->unk10 = -((0.25f - D_8008FDD4->unk8) * 4.0f);
    }

    if (D_8008FDD4->menu == 1) {
        state = 0;
        func_15086CBC(func_15086D48(homeNode), &ax, &ay, &az);
        idx = D_8008FDD4->unk2B;
        if (idx >= nodeCount) {
            idx = 0;
        }
        func_15086CBC(func_15086D48(nodeTable[idx]), &bx, &by, &bz);
        bx -= ax;
        by -= ay;
        bz -= az;
        if (D_8008FDD4->unk2A != 0) {
            bx = -bx;
            by = -by;
            bz = -bz;
        }
        if (D_8008FDD4->unk8 == 0.0f && (D_8008FDD4->input & 0x20) && D_8008FDD4->unk10 < 0.009999999776f &&
            D_8008FDD4->unk4 == 0.0f) {
            D_8008FDD4->unk2B = 0;
            if (D_8008FDD4->unk2D >= 2 && D_8008FDD4->unk2D < 7) {
                D_8008FDD4->unk2A = 2;
            }
            state = 1;
            D_8008FDD4->unkC = speeds[D_8008FDD4->unk2B];
            func_1000D96C(sounds[D_8008FDD4->unk2B].unk0, D_800E0A97, 0);
            D_800E0A97 = sounds[D_8008FDD4->unk2B].unk0;
        }
        if (D_8008FDD4->unk8 < 0.25f && D_8008FDD4->unk2A == 1) {
            D_8008FDD4->unk2B = 4;
            state = 2;
            D_8008FDD4->unk2A = 0;
        }
        if (D_8008FDD4->unk8 == 0.0f && D_8008FDD4->unk2A == 3) {
            D_8008FDD4->unk2B = 4;
            D_8008FDD4->unk2A = 0;
            D_8008FDD4->unk10 = 0.0f;
        }
        if (state != 0) {
            func_15086CBC(func_15086D48(nodeTable[D_8008FDD4->unk2B]), &cx, &cy, &cz);
            cx -= ax;
            cy -= ay;
            cz -= az;
            if (D_8008FDD4->unk2A != 0) {
                cx = -cx;
                cy = -cy;
                cz = -cz;
            }
            D_8008FDD4->unk10 = 1.0f;
            D_8008FDD4->unk14 = bx - cx;
            D_8008FDD4->unk18 = by - cy;
            D_8008FDD4->unk1C = bz - cz;
        }
        if (state == 2) {
            D_8008FDD4->unk10 = 0.0f;
            D_8008FDD4->unk2B = 0;
            D_8008FDD4->unk2A = 3;
        }
    }

    if (D_8008FDD4->unk8 >= 1.0f && D_8008FDD4->unk2B == 0) {
        D_8008FDD4->menu = 1 - D_8008FDD4->menu;
        D_8008FDD4->unk8 = 0.9900000095f;
        if (D_8008FDD4->menu != 0) {
            D_8008FDD4->unkC = -D_8008FF90[0];
        } else {
            D_8008FDD4->unkC = -D_8008FF48[0];
        }
        if (D_8008FDD4->menu != 0) {
            D_8008FDD4->unk2A = 1;
        } else {
            D_8008FDD4->unk2A = 0;
        }
    }
    D_8008FDD4->unk2C = D_8008FDD4->unk2B;
    if (D_8008FDD4->unk0 > 0.5f) {
        D_8008FDD4->unk2C++;
    }
    if (D_8008FDD4->unk2C < 0) {
        D_8008FDD4->unk2C += nodeCount;
    }
    if (D_8008FDD4->unk2C >= nodeCount) {
        D_8008FDD4->unk2C -= nodeCount;
    }
    D_8008FDD4->unk2D = D_8008FDD4->unk2C;
    if (D_8008FDD4->menu == 1) {
        D_8008FDD4->unk2C = D_800AB570[D_8008FDD4->unk2C];
    }

    obj->target.x = obj->focus.x;
    obj->target.y = obj->focus.y;
    obj->target.z = UNK2AC_F32;
    obj->focus = obj->target;
    obj->CharRelAngle = 0.0f;
    obj->TgetFOVX = 20.0f;
    obj->FOVX = 20.0f;
    obj->TgetFOVY = 15.0f;
    obj->FOVY = 15.0f;

    {
    WorldObject *q1;
    r = func_151149AC(0xF8);
    if (r != NULL) {
        r->inactive = 1;
    }
    q1 = func_151149AC(0xF6);
    r2 = func_151149AC(0xF7);
    if (q1 != NULL && r2 != NULL) {
        if (D_8008FDD4->menu == 0) {
            q1->inactive = 0;
            r2->inactive = 1;
        } else {
            q1->inactive = 1;
            r2->inactive = 0;
        }
    }
    }
}

/* Sets unk80 = 2 on both objects of the current stop once the transition is far enough along. */
void func_151E7DC0(void) {
    MenuState *state;
    WorldObject *r;
    s32 i;
    u8 objId;

    state = D_8008FDD4;
    if (state->unk8 < 0.8000000119f && state->menu == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        state = D_8008FDD4;
        if (state->menu == 0) {
            objId = D_8008FFB0[i][state->unk2C];
        } else {
            objId = D_8008FFB0[i][0];
        }
        r = func_151149AC(objId);
        if (r != NULL) {
            r->touch.i.b = 2;
        }
    }
}

/* Calls func_10017870 with 1, 2 or 4 depending on D_800E0BE9. */
void func_151E7E9C(void) {
    if (D_800E0BE0[9] == 2) {
        func_10017870(1);
    } else if (D_800E0BE0[9] == 0) {
        func_10017870(2);
    } else {
        func_10017870(4);
    }
}

/* Anti-tamper: checksums the code from func_151DDC20 to func_151DE7D4 and breaks osSpTaskLoad if it was modified. */
void func_151E7EF8(void) {
    s32 *word;
    s32 *end;
    s32 sum;

    func_151E7E9C();
    sum = 0;
    word = (s32 *)func_151DDC20;
    end = (s32 *)func_151DE7D4;
    for (; word < end; word++) {
        sum += *word;
    }
    if (sum != (s32)0xBFC924E3) {
        *(s32 *)osSpTaskLoad = 0;
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 pad7;
} Pos151E7F60;

extern Pos151E7F60 D_800AB940[];

/* Spawns the character D_800AB57C[charIdx] into slot slotIdx (replacing any previous one) at its D_800AB940 position. */
void func_151E7F60(s32 slotIdx, s32 charIdx) {
    s32 objSlot;
    s32 spawnId;
    SpawnRecord *spawn;
    s32 posIdx;
    s32 kind;
    Actor *obj;

    if (D_800E0BA0[slotIdx] != NULL) {
        func_15060F28(D_800E0BA0[slotIdx], 1);
    }
    spawnId = func_15083E0C(slotIdx + 0x10);
    spawn = (SpawnRecord *)((spawnId * 0x30) + (s32)D_800D20FC); // matching: offset + base operand order
    kind = D_800AB57C[charIdx].unk5;
    posIdx = slotIdx;
    if (D_8008FDD4->unk2C == 7) {
        posIdx++;
    }
    spawn->type = kind;
    spawn->x = D_800AB940[posIdx].unk0;
    spawn->y = D_800AB940[posIdx].unk2;
    spawn->y += D_800AB57C[charIdx].unk4;
    spawn->z = D_800AB940[posIdx].unk4;
    spawn->angle = D_800AB940[posIdx].unk6;
    if (kind == 0x53) {
        spawn->unkD = 0x24;
    } else {
        spawn->unkD = 0xE;
    }
    objSlot = func_15082A44(spawn, spawnId, 0, 0, 0);
    if (objSlot != 0) {
        obj = &movement_obj[objSlot - 1];
        D_800E0BA0[slotIdx] = obj;
        obj->drawtype = 7;
        obj->unk2F8 |= 3;
        obj->scale_xz = obj->scale_y = D_800AB57C[charIdx].unk0;
        if (obj->character_type == 0 || obj->character_type == 0x80) {
            obj->mv = allocate_memory(0x1C0, 1, 0, 0);
            bzero(obj->mv, 0x1C0);
        }
        if (kind == 0x3B) {
            obj->unk68[0] = slotIdx + 1;
        }
        func_15083384(obj, D_800AB57C[charIdx].unk6);
        func_1505E650(obj, 0xF, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    }
}

/* Clears the spawned character slots. */
void func_151E81EC(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        D_800E0BA0[i] = NULL;
    }
    D_8008FD84[0] = 0;
}

/* Switches to state 8 once D_800E0A90 passes 160 frames (held at 0 while func_1517EFDC() returns 0). */
void func_151E8214(void) {
    if (D_800E0B94 != 8) {
        if (func_1517EFDC() == 0) {
            D_800E0A90 = 0;
        }
        if (D_800E0A90 > 160) {
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

/*
 * Pages through the text bank: every 445 frames D_800E0A80 skips past the next line starting with '*';
 * a line starting with '=' ends paging, and 120 frames later the menu switches to state 9.
 */
void func_151E82B8(void) {
    func_151E530C();
    if (D_800E0A80 == -1) {
        if (D_800E0A90 > 120) {
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

    if (D_800E0A90 > 445) {
        if (D_800E0A80 >= 0) {
            while (D_800E0BD8[D_800E0A80][0] != '*') {
                D_800E0A80++;
            }
            D_800E0A80++;
            if (D_800E0BD8[D_800E0A80][0] == '=') {
                D_800E0A80 = -1;
            }
            D_800E0A90 = 0;
        }
    }
}

/* State 9: plays cue 0x1D once, then after 100 frames (timer held while func_1517EFDC() is 0) goes to state 1. */
void func_151E83E8(void) {
    if (D_800E0A80 == 0) {
        D_800E0A80 = -1;
        func_1501D348(0x1D, 6, 0, 0, 0);
    }
    func_151E530C();
    if (func_1517EFDC() == 0) {
        D_800E0A90 = 0;
    }
    if (D_800E0A90 > 100) {
        func_151E5034();
        D_8008FDA4 = 0;
        D_800E0B94 = 1;
        D_800E0A90 = 0;
        D_800D2E40 = 0;
        func_1501C730(6, 0x21, 0, 0, 1);
    }
}
