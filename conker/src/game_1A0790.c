#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800DBEA8[];
extern u8 D_800DD2E4[];
void func_151733E4(s32 arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4);

extern struct104 *D_800B0DF0;
void func_151733E4(s32 arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4);
extern s8 D_800DD2E0;

/* View of the level header (D_800B0DF0) past the fields struct104 declares. */
typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[0x36];
    s16 unk3A;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    u8 colour[4];
    s8 fogPreset; /* index into D_8008CC70, or -1 for none */
} LevelFogHeader;

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

#define sLevelFog ((LevelFogHeader *)D_800B0DF0)

/* Copies the level header's fog settings into preset 0 and applies the level's fog preset. */
void func_151732E0(s32 arg0) {
    s8 preset;

    if (sLevelFog->unk3C == sLevelFog->unk40 && sLevelFog->unk3C == 0) {
        sLevelFog->unk40 = 100;
    }
    D_8008CC70.colour[0] = sLevelFog->colour[0];
    D_8008CC70.colour[1] = sLevelFog->colour[1];
    D_8008CC70.colour[2] = sLevelFog->colour[2];
    D_8008CC70.colour[3] = sLevelFog->colour[3];
    D_8008CC70.unk4 = sLevelFog->unk3A;
    D_8008CC70.unk6 = sLevelFog->unk3C;
    D_8008CC70.unk8 = sLevelFog->unk3E;
    D_8008CC70.unkA = sLevelFog->unk40;
    D_8008CC70.unkC = sLevelFog->unk2;
    D_8008CC70.unkE = sLevelFog->unk0;
    if (sLevelFog->fogPreset != -1) {
        D_800DD2E0 = 1;
        preset = sLevelFog->fogPreset;
        func_151733E4(preset, preset, arg0, 0, -1);
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

extern s16 D_800DD2E8;
extern s16 D_800DD2EA;
extern u16 D_800DD2EC;
extern u16 D_800DD2EE;
extern s16 D_800DD2F0;
extern s16 D_800DD2F2;
extern s16 D_800DD2F4;
s8 func_15173994(s32 arg0);
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    s16 unkC;
    s16 unkE;
} struct151733E4_state;

#define st ((struct151733E4_state *)&D_8008CC70)
void func_151733E4(s32 arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4) {
    s32 i;
    s32 *p;
    s32 c;
    s32 d;
    s16 a;
    s16 b;
    u8 *out;
    u16 *p4;
    u16 *p6;
    u16 *p8;
    u16 *pA;
    s16 *pC;
    s16 *pE;

    D_800DD2F0 = arg3;
    i = 0;
    if (arg4 == -1) {
        out = D_800DD2E4;
        p4 = &D_800DD2EC;
        p6 = &D_800DD2EE;
        p8 = &D_800DD2E8;
        pA = &D_800DD2EA;
        pC = &D_800DD2F2;
        pE = &D_800DD2F4;
    } else {
        out = (u8 *)&st[arg4];
        p4 = (u16 *)(out + 4);
        p6 = (u16 *)(out + 6);
        p8 = (u16 *)(out + 8);
        pA = (u16 *)(out + 0xA);
        pC = (s16 *)(out + 0xC);
        pE = (s16 *)(out + 0xE);
    }
    do {
        if (i == 0) {
            p = &arg0;
        } else {
            p = &arg1;
        }
        switch (*p) {
        case 0x7C:
            *p = 1;
            break;
        case 0x7D:
            *p = 2;
            break;
        case 0x7E:
            *p = func_15173994(arg2);
            break;
        case 0x7F:
            *p = 2;
            break;
        }
        i++;
    } while (i < 2);
    for (i = 0; i < 4; i++) {
        c = ((u8 *)&st[arg0])[i];
        d = ((u8 *)&st[arg1])[i];
        out[i] = (s32)((d - c) * arg3) / 4096 + c;
    }
    *p4 = (s32)((st[arg1].unk4 - st[arg0].unk4) * arg3) / 4096 + st[arg0].unk4;
    *p6 = (s32)((st[arg1].unk6 - st[arg0].unk6) * arg3) / 4096 + st[arg0].unk6;
    *p8 = (s32)((st[arg1].unk8 - st[arg0].unk8) * arg3) / 4096 + st[arg0].unk8;
    *pA = (s32)((st[arg1].unkA - st[arg0].unkA) * arg3) / 4096 + st[arg0].unkA;
    a = (st[arg0].unkC == -1) ? (s16)D_800B0DF0->unk2 : st[arg0].unkC;
    b = (st[arg1].unkC == -1) ? (s16)D_800B0DF0->unk2 : st[arg1].unkC;
    *pC = (s32)((b - a) * arg3) / 4096 + a;
    a = (st[arg0].unkE == -1) ? (s16)D_800B0DF0->unk0 : st[arg0].unkE;
    b = (st[arg1].unkE == -1) ? (s16)D_800B0DF0->unk0 : st[arg1].unkE;
    *pE = (s32)((b - a) * arg3) / 4096 + a;
}


void func_151738C4(s32 arg0) {
    if (*((s8 *)D_800B0DF0 + 0x46) == -1) {
        if (D_800DBFF0->unk5F0 & 1) {
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

s8 func_15173994(s32 arg0) {
    return *((s8*)D_800B0DF0 + 0x46);
}
