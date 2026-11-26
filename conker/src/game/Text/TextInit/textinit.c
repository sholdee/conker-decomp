#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init_50A0.c).
u8 D_800BE580[9]; // bit set over the 0x43 text ids (func_15016588 clears only the first 8 bytes)

#include "functions.h"
#include "variables.h"

void text_resetContextText(void) {
    gContextTextId = 0;
    gContextTextOpen = 0;
    gContextText = 0;
    gContextHeaderText = 0;
    gContextCombinedText = 0;
    gContextTextBox = 0;
    bzero(&gContextTextBoxConfig, sizeof(TextBoxConfig));
    gContextTextScreenX = 0.0f;
    gContextTextScreenY = 0.0f;
    gContextTextAngle = 0.0f;
    gContextSoundNext = 0;
    gContextSoundCount = 0;
    gContextSounds = 0;
}

/* Builds D_800BE580: bit i is set when the data block (0x1A, D_800BEAAB, i) has a non-zero size. */
void func_15016588(void) {
    s32 i;
    s32 idx;
    s32 mask;
    s32 value;

    bzero(D_800BE580, 8);

    idx = -1;

    for (i = 0; i != 0x43;) {
        if ((i & 7) == 0) {
            mask = 1;
            idx++;
        } else {
            mask = mask << 1;
        }

        rom_findPath(&value, 3, 0x1A, D_800BEAAB, i);
        i++;
        if (value != 0) {
            D_800BE580[idx] |= mask;
        }
    }
}
