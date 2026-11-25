#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s8 D_800E0BE0[];
/* Menu input bits (MenuState.input / padInput), built from the pads in func_151DE8F0 */
#define MENU_UP     0x01 /* stick up */
#define MENU_DOWN   0x02 /* stick down */
#define MENU_LEFT   0x04 /* stick left */
#define MENU_RIGHT  0x08 /* stick right */
#define MENU_ACCEPT 0x10 /* A or Start */
#define MENU_BACK   0x20 /* B */
#define MENU_CLEFT  0x40 /* C-left */
#define MENU_CRIGHT 0x80 /* C-right */

extern void func_1501D348(s32, s32, s32, s32, s32);
void func_151E2834(void);
extern char D_800AB850[];
extern void (*D_800E0A88)(void);
extern s8 D_800E0A96; /* page to switch to once faded out; -1: none */
void func_15042D94(s32, s32, s32, char *, ...);
void func_151E2404(void);
void func_151E3344(s32, s32, s32, s32, s32);
void func_151E530C(void);
s32 func_151F2CDC(void);
s32 func_1509CF28(s32, s32 *);
void func_1509D054(void);
s32 func_1509D08C(s32, s32, s32 *, s32 *);

void func_151DD9E4(void);






extern s8 D_8008FDDC;

extern s32 D_8008FE24;
extern u8 D_8008FF08[];
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
ObjScriptCmd *func_1507BB28(s32, s32);
void func_1000DE1C(s32, s32);
void func_151E7E9C(void);

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
        func_151E3344(0x200200, 0, 1, alpha & 0xFF, 1);
        if ((D_8008FE34 == 9) && (D_8008FDD4->input & (MENU_LEFT | MENU_RIGHT))) {
            func_151E7E9C();
            func_1000DE1C(D_800E0A97, 0);
            D_800E0A97 = func_1000EA94(D_800E0BE9);
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


extern s8 D_8008FDB0; /* highlighted row of the func_151DF574 list */
extern s8 D_800AB804[];
extern char D_800AB810[];
extern s8 D_800BE91B; /* == D_800BE918[0].stick_y; the field spelling mismatches */
void func_151E30C4(void);

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
void func_15042E3C(char *, s32);
void func_151E327C(void);

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
extern s8 D_8008FE9C[];
extern s8 D_8008FEC4[]; /* [preset][4] team per player; a 2D declaration mismatches */
extern s8 D_8008FDA0; /* extra (non-controller) slots in the player list */
extern s8 D_8008FDC4;
void func_151E22BC(void);

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
    func_151E3344(options, 1, 0, alpha & 0xFF, 0);

    if (D_8008FDD4->input & MENU_BACK) {
        D_800E0A96 = 0;
    }
    D_8008FDD4->input &= ~MENU_BACK;
}

extern u32 D_8008FEA4[];
extern char D_800AB854[];
void func_151E7F60(s32, s32);

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

void func_151E4314(void);
extern s8 D_8008FEE3[];
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
    // unk5 is -(preset + 1) here, so D_8008FEE3[-unk5] is the preset's entry of D_8008FEE4[]
    limits = D_800AB74C[D_8008FEE3[-D_800AB68C[D_8008FDD4->selection].unk5]];
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

/* Per-team x jitter for stacked names; a struct so the initial copy is one word */
typedef struct { s8 v[4]; } Offs4;
extern Offs4 D_8008FF34;
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
s32 func_151E24F0(s32 *);

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

void func_151E43DC(void);

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
