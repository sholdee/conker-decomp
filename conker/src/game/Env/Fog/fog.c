#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/env.h describes it. */
s8 gHasLevelFogPreset;
u8 gFogColour[4];
u16 D_800DD2E8;
u16 D_800DD2EA;
u16 D_800DD2EC;
u16 D_800DD2EE;
s16 gFogBlend;
s16 gFogNear;
s16 gFogFar;

#include "functions.h"
#include "variables.h"

/* One entry of the fog preset table gFogPresets[0]. */
typedef struct {
    u8 colour[4];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    s16 nearPlane; /* -1: use the level header's unk2 (gDefaultNear) */
    s16 farPlane; /* -1: use the level header's unk0 (gDefaultFar) */
} FogPreset;

/* The fog presets; env_initFog fills preset 0 from the level header. */
static FogPreset gFogPresets[9] = {
    { { 0x00, 0x00, 0x00, 0x00 }, 0, 0, 0, 0, 0, 0 },
    { { 0x08, 0x07, 0x25, 0x00 }, 821, 943, 1041, 948, 100, 17602 },
    { { 0xFF, 0x34, 0x00, 0x00 }, 935, 943, 1075, 948, 100, 17602 },
    { { 0x00, 0x00, 0x00, 0x00 }, 1200, 3943, 20300, 30948, 100, 17602 },
    { { 0x00, 0x00, 0x00, 0x00 }, 0, 943, 0, 948, 0, 0 },
    { { 0x00, 0x00, 0x00, 0x00 }, 0, 943, 0, 948, 0, 0 },
    { { 0x0D, 0x28, 0x03, 0x00 }, 952, 956, 1000, 993, 100, 6962 },
    { { 0x77, 0x9E, 0xFF, 0x00 }, 976, 943, 1024, 948, 100, 10738 },
    { { 0xE1, 0xE7, 0xFF, 0x00 }, 886, 943, 1070, 948, 100, 9816 },
};

/* Copies the level header's fog settings into preset 0 and applies the level's fog preset. */
void env_initFog(s32 arg0) {
    if (D_800B0DF0->unk3C == D_800B0DF0->unk40 && D_800B0DF0->unk3C == 0) {
        D_800B0DF0->unk40 = 100;
    }
    gFogPresets[0].colour[0] = D_800B0DF0->fogColour[0];
    gFogPresets[0].colour[1] = D_800B0DF0->fogColour[1];
    gFogPresets[0].colour[2] = D_800B0DF0->fogColour[2];
    gFogPresets[0].colour[3] = D_800B0DF0->fogColour[3];
    gFogPresets[0].unk4 = D_800B0DF0->unk3A;
    gFogPresets[0].unk6 = D_800B0DF0->unk3C;
    gFogPresets[0].unk8 = D_800B0DF0->unk3E;
    gFogPresets[0].unkA = D_800B0DF0->unk40;
    gFogPresets[0].nearPlane = D_800B0DF0->nearPlane;
    gFogPresets[0].farPlane = D_800B0DF0->farPlane;
    if (D_800B0DF0->fogPreset != -1) {
        gHasLevelFogPreset = 1;
        env_blendFog(D_800B0DF0->fogPreset, D_800B0DF0->fogPreset, arg0, 0, -1);
    } else {
        gHasLevelFogPreset = 0;
        env_blendFog(0, 0, arg0, 0, -1);
    }
}


/* Display-list hook that emits nothing. */
Gfx *func_151733D8(Gfx *gfx, s32 arg1) {
    while (1) { // matching: under -g3 a plain return leaves the jr delay slot as a nop
        return gfx;
    }
}


/*
 * Blends fog presets `from` and `to` by `blend`/4096 into preset `dest`, or into the active fog
 * globals (gFogColour...) when `dest` is -1. Indices 0x7C-0x7F are aliases resolved first.
 */
void env_blendFog(s32 from, s32 to, s32 arg2, s32 blend, s32 dest) {
    s32 i;
    s32 *index;
    s32 fromByte;
    s32 toByte;
    s16 fromValue;
    s16 toValue;
    u8 *outColour;
    u16 *out4;
    u16 *out6;
    u16 *out8;
    u16 *outA;
    s16 *outC;
    s16 *outE;
    FogPreset *preset;

    gFogBlend = blend;
    if (dest == -1) {
        outColour = gFogColour;
        out4 = &D_800DD2EC;
        out6 = &D_800DD2EE;
        out8 = &D_800DD2E8;
        outA = &D_800DD2EA;
        outC = &gFogNear;
        outE = &gFogFar;
    } else {
        preset = &gFogPresets[dest];
        outColour = preset->colour;
        out4 = &preset->unk4;
        out6 = &preset->unk6;
        out8 = &preset->unk8;
        outA = &preset->unkA;
        outC = &preset->nearPlane;
        outE = &preset->farPlane;
    }
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            index = &from;
        } else {
            index = &to;
        }
        switch (*index) {
        case 0x7C:
            *index = 1;
            break;
        case 0x7D:
            *index = 2;
            break;
        case 0x7E:
            *index = env_getLevelFogPreset(arg2);
            break;
        case 0x7F:
            *index = 2;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        fromByte = gFogPresets[from].colour[i];
        toByte = gFogPresets[to].colour[i];
        outColour[i] = (toByte - fromByte) * blend / 4096 + fromByte;
    }
    *out4 = (gFogPresets[to].unk4 - gFogPresets[from].unk4) * blend / 4096 + gFogPresets[from].unk4;
    *out6 = (gFogPresets[to].unk6 - gFogPresets[from].unk6) * blend / 4096 + gFogPresets[from].unk6;
    *out8 = (gFogPresets[to].unk8 - gFogPresets[from].unk8) * blend / 4096 + gFogPresets[from].unk8;
    *outA = (gFogPresets[to].unkA - gFogPresets[from].unkA) * blend / 4096 + gFogPresets[from].unkA;
    fromValue = (gFogPresets[from].nearPlane == -1) ? D_800B0DF0->nearPlane : gFogPresets[from].nearPlane;
    toValue = (gFogPresets[to].nearPlane == -1) ? D_800B0DF0->nearPlane : gFogPresets[to].nearPlane;
    *outC = (toValue - fromValue) * blend / 4096 + fromValue;
    fromValue = (gFogPresets[from].farPlane == -1) ? D_800B0DF0->farPlane : gFogPresets[from].farPlane;
    toValue = (gFogPresets[to].farPlane == -1) ? D_800B0DF0->farPlane : gFogPresets[to].farPlane;
    *outE = (toValue - fromValue) * blend / 4096 + fromValue;
}

/* With no level fog preset, re-applies preset 6 (if camera_global_Cameras->SpecialFlags bit 0 is set) or preset 0. */
void env_reapplyFog(s32 arg0) {
    if (D_800B0DF0->fogPreset == -1) {
        if (camera_global_Cameras->SpecialFlags & 1) {
            env_blendFog(6, 6, arg0, 0, -1);
            gBackgroundColour[0] = gFogColour[0];
            gBackgroundColour[1] = gFogColour[1];
            gBackgroundColour[2] = gFogColour[2];
        } else {
            env_blendFog(0, 0, arg0, 0, -1);
            gBackgroundColour[0] = D_800B0DF0->backgroundRed;
            gBackgroundColour[1] = D_800B0DF0->backgroundGreen;
            gBackgroundColour[2] = D_800B0DF0->backgroundBlue;
        }
    }
}

/* Returns the level's fog preset index (-1 for none). */
s8 env_getLevelFogPreset(s32 arg0) {
    return D_800B0DF0->fogPreset;
}
