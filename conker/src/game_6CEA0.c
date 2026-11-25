#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* variables.h declares D_800BE728 as s32[]; it holds a pointer to the per-controller OSContPad pointers. */
#define CONT_PADS (*(OSContPad ***)&D_800BE728)

#define STICK_DEADZONE 20

/*
 * Menu repeat for the analog stick: every 5th call, reports the direction (-1, 0 or 1) the stick of
 * `controller` points along each axis, ignoring deflections within STICK_DEADZONE, and clears the
 * stick reading. On the other calls both outputs are 0.
 */
void func_1503F9F0(s16 controller, s16 *dirX, s16 *dirY) {
    static s16 D_80084484; // frames until the next repeat
    OSContPad *pad;
    s16 stickX;

    pad = CONT_PADS[controller];
    if (--D_80084484 != 0) {
        *dirX = 0;
        *dirY = 0;
        return;
    }
    stickX = pad->stick_x;
    D_80084484 = 5;
    if (stickX < -STICK_DEADZONE || stickX > STICK_DEADZONE) {
        *dirX = stickX;
    } else {
        *dirX = 0;
    }
    if (pad->stick_y < -STICK_DEADZONE || pad->stick_y > STICK_DEADZONE) {
        *dirY = pad->stick_y;
    } else {
        *dirY = 0;
    }
    pad->stick_x = 0;
    pad->stick_y = 0;
    *dirX = (*dirX < -1) ? -1 : (*dirX > 1) ? 1 : *dirX;
    *dirY = (*dirY < -1) ? -1 : (*dirY > 1) ? 1 : *dirY;
}

extern s32 D_80084488;
extern f32 D_800C6850;
extern s32 D_8008448C;
extern u8 D_80084490;
extern u8 D_80084494;

void func_1503FB08(void) {
    D_80084488 = 0;
    D_800C6850 = 0.0f;
    D_8008448C = 0;
    D_80084490 = 30;
    D_80084494 = 0;
}

extern u8 D_80084498;
extern char D_8008449C[];
extern Gfx D_800859A0[];
extern char D_80098930[];
extern char D_80098944[];
extern char D_80098948[];
Gfx *func_150417AC(Gfx *, f32, f32, char *, u8, u8, u8, u8, f32, f32, u32);
void func_150428D4(char *, s32 *, s32 *, s32 *);
char *func_1509CA10(s32);
s32 func_1509CA78(s32);
s32 func_1509CA98(s32);
s32 func_1509CB68(void);
s32 strlen(const char *);

s32 func_1503FB40(Gfx **arg0, u16 arg1, u16 arg2) {
    s32 id;
    s16 spE2;
    s16 spE0;
    u8 alpha;
    u8 i;
    s32 y;
    s32 row;
    s32 width;
    s32 height;
    s32 spC8;
    s32 total;
    f32 x;
    f32 ty;
    f32 sy;
    char *text;
    s32 dir;
    u16 buttons;
    u8 start;
    u8 count;
    s32 spA8;

    alpha = 0;
    buttons = D_800BE710[0];
    i = 0;
    row = 0;
    width = 0;
    height = 0;
    total = func_1509CB68();
    gSPDisplayList((*arg0)++, D_800859A0);
    if (total < 19) {
        count = total;
    } else {
        count = 19;
    }
    y = 1;
    if (D_8008448C == 0 && (buttons & 0x20) && (buttons & 0x10)) {
        D_8008448C = 60;
        func_15017790();
        func_150177F8();
    }
    if (D_8008448C != 0) {
        buttons = 0;
        D_8008448C -= D_800BE9E4;
        if (D_8008448C <= 0) {
            D_8008448C = 0;
        }
    }
    if (buttons & arg2) {
        if (!(*((u8 *)D_800D2E4C + (func_1509CA98(D_80084498 + 1) >> 3)) & (1 << (func_1509CA98(D_80084498 + 1) & 7)))) {
            return func_1509CA98(D_80084498 + 1);
        }
    }
    if ((buttons & arg1) || (buttons & 0x4000)) {
        return -1;
    }
    func_1503F9F0(0, &spE2, &spE0);
    if (D_80084488 != 0) {
        D_800C6850 += 1.25f * (f32)(D_80084488 * D_800BE9E4);
        ty = -11.0f;
        if (D_800C6850 < -11.0f) {
        } else if (D_800C6850 > 11.0f) {
            ty = 11.0f;
        } else {
            ty = D_800C6850;
        }
        D_800C6850 = ty;
        if (fabsf(D_800C6850) >= 11.0f) {
            dir = (**(u16 **)D_800BE728[0] & 0x2000) ? 3 : 1;
            if (D_80084488 == 1) {
                D_80084498 -= dir;
                if (total < D_80084498) {
                    D_80084498 += total;
                }
            } else if (D_80084488 == -1) {
                D_80084498 = (D_80084498 + dir) % total;
            }
            D_80084488 = 0;
            D_800C6850 = 0.0f;
        }
    } else if ((buttons & 4) || (spE0 == -1)) {
        D_80084488 = -1;
        D_800C6850 = 0.0f;
    } else if ((buttons & 8) || (spE0 == 1)) {
        D_80084488 = 1;
        D_800C6850 = 0.0f;
    }
    start = D_80084498 - (u8)((count - 1) >> 1);
    if (total < start) {
        start += total;
    }
    *(s8 *)&D_80084490 -= D_800BE9E4;
    if (*(s8 *)&D_80084490 <= 0) {
        D_80084490 = 30;
        D_80084494 ^= 1;
    }
    do {
        {
        s32 width2;
        s32 height2;
        f32 x2;
        f32 y2;

        id = func_1509CA98(start + 1);
        if (D_8008448C != 0) {
            text = D_80098930;
            func_150428D4(text, &width, &height, &spC8);
        } else {
            func_150428D4(func_1509CA10(id), &width, &height, &spC8);
            if (*((id >> 3) + (u8 *)D_800D2E4C) & (1 << (id & 7))) {
                if (start == D_80084498) {
                    if (D_80084494 != 0) {
                        text = D_80098948;
                    } else {
                        text = func_1509CA10(id);
                    }
                    func_150428D4(text, &width, &spA8, &spC8);
                } else {
                    text = func_1509CA10(id);
                }
            } else {
                text = func_1509CA10(id);
            }
        }
        ty = (f32)y + 1.0f;
        x = (f32)D_800BE620 * 0.5f - (f32)(width >> 1);
        sy = ty + D_800C6850;
        if (func_1509CA78(id) & 0x80000000) {
            func_150428D4(D_80098944, &width2, &height2, &spC8);
            x2 = (f32)D_800BE620 * 0.5f - (f32)(width2 >> 1);
            y2 = ty + D_800C6850;
            *arg0 = func_150417AC(*arg0, x2, y2, D_80098944,
                                  0xFF, 0xFF, 0xFF, alpha, 4096.0f, 4096.0f, strlen(D_80098944));
            y += height2;
            sy += (f32)height2;
        }
        if (row < 3) {
            alpha = (row << 6) + 0x40;
        } else if (count - 3 < row) {
            alpha = (count - row) << 6;
        } else {
            alpha = 0xFF;
        }
        if (text == NULL) {
            text = D_8008449C;
        }
        if (D_8008448C == 0 && (start != D_80084498 || D_80084488 != 0)) {
            *arg0 = func_150417AC(*arg0, x, sy, text, 0xFF, 0xFF, 0xFF, alpha, 4096.0f, 4096.0f, strlen(text));
        } else {
            *arg0 = func_150417AC(*arg0, x, sy, text, 0xFF, 0, 0, alpha, 4096.0f, 4096.0f, strlen(text));
        }
        y += height;
        row++;
        start++;
        start %= total;
        }
        i++;
    } while (i < count);
    return -2;
}
