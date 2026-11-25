#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s8 D_800DD2E0;

/* One entry of the fog preset table D_8008CC70. */
typedef struct {
    u8 colour[4];
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    s16 unkC; /* -1: use the level header's unk2 */
    s16 unkE; /* -1: use the level header's unk0 */
} FogPreset;

extern FogPreset D_8008CC70;

/* Copies the level header's fog settings into preset 0 and applies the level's fog preset. */
void func_151732E0(s32 arg0) {
    if (D_800B0DF0->unk3C == D_800B0DF0->unk40 && D_800B0DF0->unk3C == 0) {
        D_800B0DF0->unk40 = 100;
    }
    D_8008CC70.colour[0] = D_800B0DF0->fogColour[0];
    D_8008CC70.colour[1] = D_800B0DF0->fogColour[1];
    D_8008CC70.colour[2] = D_800B0DF0->fogColour[2];
    D_8008CC70.colour[3] = D_800B0DF0->fogColour[3];
    D_8008CC70.unk4 = D_800B0DF0->unk3A;
    D_8008CC70.unk6 = D_800B0DF0->unk3C;
    D_8008CC70.unk8 = D_800B0DF0->unk3E;
    D_8008CC70.unkA = D_800B0DF0->unk40;
    D_8008CC70.unkC = D_800B0DF0->unk2;
    D_8008CC70.unkE = D_800B0DF0->unk0;
    if (D_800B0DF0->fogPreset != -1) {
        D_800DD2E0 = 1;
        func_151733E4(D_800B0DF0->fogPreset, D_800B0DF0->fogPreset, arg0, 0, -1);
    } else {
        D_800DD2E0 = 0;
        func_151733E4(0, 0, arg0, 0, -1);
    }
}


/* Display-list hook that emits nothing. */
Gfx *func_151733D8(Gfx *gfx, s32 arg1) {
    while (1) { // matching: under -g3 a plain return leaves the jr delay slot as a nop
        return gfx;
    }
}

extern u16 D_800DD2EA;
extern u16 D_800DD2EE;
extern s16 D_800DD2F0;
#define sFogPresets (&D_8008CC70)

/*
 * Blends fog presets `from` and `to` by `blend`/4096 into preset `dest`, or into the active fog
 * globals (D_800DD2E4...) when `dest` is -1. Indices 0x7C-0x7F are aliases resolved first.
 */
void func_151733E4(s32 from, s32 to, s32 arg2, s32 blend, s32 dest) {
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

    D_800DD2F0 = blend;
    if (dest == -1) {
        outColour = D_800DD2E4;
        out4 = &D_800DD2EC;
        out6 = &D_800DD2EE;
        out8 = &D_800DD2E8;
        outA = &D_800DD2EA;
        outC = &D_800DD2F2;
        outE = &D_800DD2F4;
    } else {
        preset = &sFogPresets[dest];
        outColour = preset->colour;
        out4 = &preset->unk4;
        out6 = &preset->unk6;
        out8 = &preset->unk8;
        outA = &preset->unkA;
        outC = &preset->unkC;
        outE = &preset->unkE;
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
            *index = func_15173994(arg2);
            break;
        case 0x7F:
            *index = 2;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        fromByte = sFogPresets[from].colour[i];
        toByte = sFogPresets[to].colour[i];
        outColour[i] = (toByte - fromByte) * blend / 4096 + fromByte;
    }
    *out4 = (sFogPresets[to].unk4 - sFogPresets[from].unk4) * blend / 4096 + sFogPresets[from].unk4;
    *out6 = (sFogPresets[to].unk6 - sFogPresets[from].unk6) * blend / 4096 + sFogPresets[from].unk6;
    *out8 = (sFogPresets[to].unk8 - sFogPresets[from].unk8) * blend / 4096 + sFogPresets[from].unk8;
    *outA = (sFogPresets[to].unkA - sFogPresets[from].unkA) * blend / 4096 + sFogPresets[from].unkA;
    fromValue = (sFogPresets[from].unkC == -1) ? (s16)D_800B0DF0->unk2 : sFogPresets[from].unkC;
    toValue = (sFogPresets[to].unkC == -1) ? (s16)D_800B0DF0->unk2 : sFogPresets[to].unkC;
    *outC = (toValue - fromValue) * blend / 4096 + fromValue;
    fromValue = (sFogPresets[from].unkE == -1) ? (s16)D_800B0DF0->unk0 : sFogPresets[from].unkE;
    toValue = (sFogPresets[to].unkE == -1) ? (s16)D_800B0DF0->unk0 : sFogPresets[to].unkE;
    *outE = (toValue - fromValue) * blend / 4096 + fromValue;
}

/* With no level fog preset, re-applies preset 6 (if camera_global_Cameras->SpecialFlags bit 0 is set) or preset 0. */
void func_151738C4(s32 arg0) {
    if (D_800B0DF0->fogPreset == -1) {
        if (camera_global_Cameras->SpecialFlags & 1) {
            func_151733E4(6, 6, arg0, 0, -1);
            D_800DBEA8[0] = D_800DD2E4[0];
            D_800DBEA8[1] = D_800DD2E4[1];
            D_800DBEA8[2] = D_800DD2E4[2];
        } else {
            func_151733E4(0, 0, arg0, 0, -1);
            D_800DBEA8[0] = D_800B0DF0->unk5;
            D_800DBEA8[1] = D_800B0DF0->unk6;
            D_800DBEA8[2] = D_800B0DF0->unk7;
        }
    }
}

/* Returns the level's fog preset index (-1 for none). */
s8 func_15173994(s32 arg0) {
    return D_800B0DF0->fogPreset;
}
