#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  unk5;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[3];
    /* 0x0C */ WorldObject *unkC[8]; /* the eight stage parts (StageState in game_124920.c) */
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 pad3E;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
} Struct11D60; /* size 0x4C */

void func_15011D40(void) {
    func_15103800();
}

s32 func_15011D60(TriggerZone *zone) {
    Struct11D60 tmp;
    u8 pad[0xC]; // matching: stack space
    TriggerZone key;
    TriggerZone *found;
    TimedEffect *effect;
    u8 flag;

    bzero(&tmp, sizeof(tmp));
    tmp.unk8 = 0;
    tmp.unk6 = (func_150ADA20() % 0x79U) + 0x12C;
    tmp.unk0 = zone;

    tmp.unk4 = zone->data2;
    if (tmp.unk4 >= 4) {
        flag = 1;
    } else {
        flag = 0;
    }
    tmp.unk5 = flag;
    {
        u8 i;
        for (i = 0; i < 8; i++) {
            tmp.unkC[i] = func_151149AC(D_800A1C00[i][tmp.unk4]);
        }
        tmp.unk3C = 0;
        tmp.unk2C = 0.0f;
        tmp.unk30 = 0.0f;
        tmp.unk34 = 0.0f;
        tmp.unk38 = 0.0f;
    }
    key.kind = 3;
    key.subTypeData = 0x15;
    key.data1 = tmp.unk4;
    found = func_151438D8(0, gZoneCount, 0x11A0, &key);
    if (found != NULL) {
        tmp.unk40 = found->x;
        tmp.unk44 = found->z;
        tmp.unk48 = found->xSize;
    } else {
        tmp.unk48 = 10.0f;
    }
    effect = func_15149130(0x12C, -1, -1, -1, 0, 0x38, sizeof(tmp), 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
    return 1;
}
