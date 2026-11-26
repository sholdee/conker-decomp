#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init_50A0.c).
u8 D_800BE580[9]; // bit set over the 0x43 text ids (func_15016588 clears only the first 8 bytes)

#include "functions.h"
#include "variables.h"

void func_15016500(void) {
    D_800D1940 = 0;
    D_800D1941 = 0;
    D_800D1944 = 0;
    D_800D1948 = 0;
    D_800D194C = 0;
    D_800D1950 = 0;
    bzero(&D_800D1958, sizeof(TextBoxConfig));
    D_800D1988 = 0.0f;
    D_800D198C = 0.0f;
    D_800D1990 = 0.0f;
    D_800D1994 = 0;
    D_800D1995 = 0;
    D_800D1998 = 0;
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

        func_1502B020(&value, 3, 0x1A, D_800BEAAB, i);
        i++;
        if (value != 0) {
            D_800BE580[idx] |= mask;
        }
    }
}
