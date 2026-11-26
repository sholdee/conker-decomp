#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s16 D_80084480 = 0; // the first word of this file's .data; read by game/Camera/Camera/camera.c and game/Game/MainLoop/mainloop.c


#define STICK_DEADZONE 20

/*
 * Menu repeat for the analog stick: every 5th call, reports the direction (-1, 0 or 1) the stick of
 * `controller` points along each axis, ignoring deflections within STICK_DEADZONE, and clears the
 * stick reading. On the other calls both outputs are 0.
 */
void debug_getStickRepeat(s16 controller, s16 *dirX, s16 *dirY) {
    static s16 gStickRepeatTimer = 5; // frames until the next repeat
    OSContPad *pad;
    s16 stickX;

    pad = gPadTable[controller];
    if (--gStickRepeatTimer != 0) {
        *dirX = 0;
        *dirY = 0;
        return;
    }
    stickX = pad->stick_x;
    gStickRepeatTimer = 5;
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

/* State of the task list menu drawn by debug_drawTaskMenu. */
static s32 gTaskMenuScrollDir = 0; // scroll in progress: 1 = up, -1 = down, 0 = idle
static f32 gTaskMenuScrollOffset;     // scroll animation offset, +-11 at the end of a step
static s32 gTaskMenuResetTimer = 0; // frames left showing "RESETTING ALL TASKS"
static s8 gTaskMenuBlinkTimer = 0;  // frames until the selection blink toggles
static u8 gTaskMenuBlinkPhase = 0;  // blink phase

/* Resets the task list menu's scroll and blink state. */
void debug_resetTaskMenu(void) {
    gTaskMenuScrollDir = 0;
    gTaskMenuScrollOffset = 0.0f;
    gTaskMenuResetTimer = 0;
    gTaskMenuBlinkTimer = 30;
    gTaskMenuBlinkPhase = 0;
}

static u8 gTaskMenuSelection = 0;     // selected task (index into the list of tasks in use)
static char D_8008449C[] = "ERROR";
static const char D_80098930[] = "RESETTING ALL TASKS";
static const char D_80098944[] = "."; // drawn above flagged tasks
static const char D_80098948[] = "RESET TO PLAY !";
s32 strlen(const char *);

/* Bit `idx` of the task bitset at gTaskDoneBits (see game/Script/EffectCmd/effectcmd.c). */
#define TASK_BIT_SET(idx) (gTaskDoneBits[(idx) >> 3] & (1 << ((idx) & 7)))

#define MENU_MAX_ROWS 19
#define SCROLL_LIMIT 11.0f
#define TEXT_SCALE_1 4096.0f

/*
 * Debug task menu: draws up to MENU_MAX_ROWS task names centred on the selected one, fading at the
 * top and bottom, and scrolls the selection with C-up/C-down or the stick (3 at a time with Z held).
 * L+R together resets all tasks. Returns the selected task's descriptor index when a button in
 * `selectButtons` is pressed and the task's bit is clear, -1 when a button in `backButtons` or B is
 * pressed, otherwise -2.
 */
s32 debug_drawTaskMenu(Gfx **gfxp, u16 backButtons, u16 selectButtons) {
    s32 taskIndex;
    s16 stickDirX;
    s16 stickDirY;
    u8 alpha;
    u8 i;
    s32 lineY;
    s32 row;
    s32 width;
    s32 height;
    s32 depth;
    s32 taskCount;
    f32 textX;
    f32 rowY;
    f32 textY;
    const char *text;
    s32 step;
    u16 buttons;
    u8 entry;
    u8 rowCount;
    s32 unusedHeight;

    alpha = 0;
    buttons = gButtonsPressed[0]; // buttons newly pressed on controller 1
    i = 0;
    row = 0;
    width = 0;
    height = 0;
    taskCount = script_countTasks();
    gSPDisplayList((*gfxp)++, gText2DSetupDL);
    if (taskCount < MENU_MAX_ROWS) {
        rowCount = taskCount;
    } else {
        rowCount = MENU_MAX_ROWS;
    }
    lineY = 1;
    if (gTaskMenuResetTimer == 0 && (buttons & L_TRIG) && (buttons & R_TRIG)) {
        gTaskMenuResetTimer = 60;
        func_15017790();
        func_150177F8();
    }
    if (gTaskMenuResetTimer != 0) {
        buttons = 0;
        gTaskMenuResetTimer -= D_800BE9E4;
        if (gTaskMenuResetTimer <= 0) {
            gTaskMenuResetTimer = 0;
        }
    }
    if (buttons & selectButtons) {
        if (!TASK_BIT_SET(script_findTaskIndex(gTaskMenuSelection + 1))) {
            return script_findTaskIndex(gTaskMenuSelection + 1);
        }
    }
    if ((buttons & backButtons) || (buttons & B_BUTTON)) {
        return -1;
    }
    debug_getStickRepeat(0, &stickDirX, &stickDirY);
    if (gTaskMenuScrollDir != 0) {
        gTaskMenuScrollOffset += 1.25f * (gTaskMenuScrollDir * D_800BE9E4);
        // matching: clamp through rowY (shares its FP register); a ternary or separate temp mismatches
        rowY = -SCROLL_LIMIT;
        if (gTaskMenuScrollOffset < -SCROLL_LIMIT) {
        } else if (gTaskMenuScrollOffset > SCROLL_LIMIT) {
            rowY = SCROLL_LIMIT;
        } else {
            rowY = gTaskMenuScrollOffset;
        }
        gTaskMenuScrollOffset = rowY;
        if (fabsf(gTaskMenuScrollOffset) >= SCROLL_LIMIT) {
            step = (gPadTable[0]->button & Z_TRIG) ? 3 : 1;
            if (gTaskMenuScrollDir == 1) {
                gTaskMenuSelection -= step;
                if (gTaskMenuSelection > taskCount) {
                    gTaskMenuSelection += taskCount;
                }
            } else if (gTaskMenuScrollDir == -1) {
                gTaskMenuSelection = (gTaskMenuSelection + step) % taskCount;
            }
            gTaskMenuScrollDir = 0;
            gTaskMenuScrollOffset = 0.0f;
        }
    } else if ((buttons & D_CBUTTONS) || (stickDirY == -1)) {
        gTaskMenuScrollDir = -1;
        gTaskMenuScrollOffset = 0.0f;
    } else if ((buttons & U_CBUTTONS) || (stickDirY == 1)) {
        gTaskMenuScrollDir = 1;
        gTaskMenuScrollOffset = 0.0f;
    }
    entry = gTaskMenuSelection - (u8)((rowCount - 1) >> 1);
    if (entry > taskCount) {
        entry += taskCount;
    }
    gTaskMenuBlinkTimer -= D_800BE9E4;
    if (gTaskMenuBlinkTimer <= 0) {
        gTaskMenuBlinkTimer = 30;
        gTaskMenuBlinkPhase ^= 1;
    }
    do {
        {
        s32 dotWidth;
        s32 dotHeight;
        f32 dotX;
        f32 dotY;

        taskIndex = script_findTaskIndex(entry + 1);
        if (gTaskMenuResetTimer != 0) {
            text = D_80098930;
            text_measureString((u8 *)text, &width, &height, &depth);
        } else {
            text_measureString((u8 *)script_getTaskName(taskIndex), &width, &height, &depth);
            // matching: index + base operand order (TASK_BIT_SET's base + index allocates differently)
            if (*((taskIndex >> 3) + gTaskDoneBits) & (1 << (taskIndex & 7))) {
                if (entry == gTaskMenuSelection) {
                    if (gTaskMenuBlinkPhase != 0) {
                        text = D_80098948;
                    } else {
                        text = script_getTaskName(taskIndex);
                    }
                    text_measureString((u8 *)text, &width, &unusedHeight, &depth);
                } else {
                    text = script_getTaskName(taskIndex);
                }
            } else {
                text = script_getTaskName(taskIndex);
            }
        }
        rowY = lineY + 1.0f;
        textX = gScreenWidth * 0.5f - (width >> 1);
        textY = rowY + gTaskMenuScrollOffset;
        if (func_1509CA78(taskIndex) & 0x80000000) {
            /* Flagged tasks get a "." line above them. */
            text_measureString((u8 *)D_80098944, &dotWidth, &dotHeight, &depth);
            dotX = gScreenWidth * 0.5f - (dotWidth >> 1);
            dotY = rowY + gTaskMenuScrollOffset;
            *gfxp = text_drawString(*gfxp, dotX, dotY, (u8 *)D_80098944, 255, 255, 255, alpha, TEXT_SCALE_1, TEXT_SCALE_1, strlen(D_80098944));
            lineY += dotHeight;
            textY += dotHeight;
        }
        if (row < 3) {
            alpha = row * 64 + 64;
        } else if (row > rowCount - 3) {
            alpha = (rowCount - row) * 64;
        } else {
            alpha = 255;
        }
        if (text == NULL) {
            text = D_8008449C;
        }
        if (gTaskMenuResetTimer == 0 && (entry != gTaskMenuSelection || gTaskMenuScrollDir != 0)) {
            *gfxp = text_drawString(*gfxp, textX, textY, (u8 *)text, 255, 255, 255, alpha, TEXT_SCALE_1, TEXT_SCALE_1, strlen(text));
        } else {
            /* the selected task, and every row while resetting, is drawn in red */
            *gfxp = text_drawString(*gfxp, textX, textY, (u8 *)text, 255, 0, 0, alpha, TEXT_SCALE_1, TEXT_SCALE_1, strlen(text));
        }
        lineY += height;
        row++;
        entry++;
        entry %= taskCount;
        }
        i++;
    } while (i < rowCount);
    return -2;
}
