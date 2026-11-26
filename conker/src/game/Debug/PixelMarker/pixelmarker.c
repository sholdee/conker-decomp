#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .data: 16 zero bytes between game/Debug/DlCheck/dlcheck's .data and game/Text/FixedFont/fixedfont's. */
static s32 D_800848C0[4] = { 0, 0, 0, 0 }; /* not referenced */

void func_15040D90(s32 arg0) {

}

void debug_drawFilledMarker(s32 arg0) {
    u16 *ptr;
    s32 i;
    s32 j;

    i = gDrawBufferIndex;
    ptr = gFramebuffers[i == 0];
    for (j = 0; j != 8; j++) {
        for (i = 0; i != 8; i += 4) {
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 200) = 0xF800;
            ptr = gFramebuffers[gDrawBufferIndex];
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 200) = 0xF800;
            ptr = gZBuffer;
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 200) = 0;

            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 201) = 0xF800;
            ptr = gFramebuffers[gDrawBufferIndex];
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 201) = 0xF800;
            ptr = gZBuffer;
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 201) = 0;

            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 202) = 0xF800;
            ptr = gFramebuffers[gDrawBufferIndex];
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 202) = 0xF800;
            ptr = gZBuffer;
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 202) = 0;

            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 203) = 0xF800;
            ptr = gFramebuffers[gDrawBufferIndex];
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 203) = 0xF800;
            ptr = gZBuffer;
            *(ptr + (arg0 + i) + gScreenWidth * (j + 225) + 203) = 0;
        }
    }
}

void debug_drawHollowMarker(s32 arg0) {
    u16 *fb1;
    u16 *fb2;
    u16 *zb;
    s32 i;
    s32 j;

    j = gDrawBufferIndex == 0;
    fb1 = gFramebuffers[j];
    fb2 = gFramebuffers[gDrawBufferIndex];
    zb = gZBuffer;
    j = 0;
    for (i = 0; i < 8; i++) {
        s32 idx;

        idx = arg0 + i + gScreenWidth * (j + 225) + 200;
        fb1[idx] = 0xF800;
        fb2[idx] = 0xF800;
        zb[idx] = 0;
    }
    for (j = 1; j < 7; j++) {
        for (i = 0; i < 8; i += 7) {
            s32 idx;

            idx = arg0 + i + gScreenWidth * (j + 225) + 200;
            fb1[idx] = 0xF800;
            fb2[idx] = 0xF800;
            zb[idx] = 0;
        }
    }
    for (j = 1; j < 7; j++) {
        for (i = 1; i < 7; i++) {
            s32 idx;

            idx = arg0 + i + gScreenWidth * (j + 225) + 200;
            fb1[idx] = 0;
            fb2[idx] = 0;
            zb[idx] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        s32 idx;

        idx = arg0 + i + gScreenWidth * (j + 225) + 200;
        fb1[idx] = 0xF800;
        fb2[idx] = 0xF800;
        zb[idx] = 0;
    }
}

void debug_busyWait(void) {
    s32 i = 0;
    do {} while (++i < 0x18000000);
}
