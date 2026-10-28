#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15174BF0.s")

typedef struct Game174BF0Actor {
    u8 pad0[0x90];
    s8 group, phase;
    s16 flags;
    u8 *model;
    f32 x, y, z;
    s16 timer, enabled;
    f32 pitch, yaw, roll, velocity, yawStep, fieldBC, scale;
    u8 padC4[4];
    f32 baseHeight;
    u8 padCC[0x1C];
    u32 surfaceFlags;
    u8 brightness;
    s8 modelIndex;
    u16 sound;
    s8 opacity, fieldF1;
} Game174BF0Actor;

typedef struct Game174FA4Surface {
    s32 height;
    u8 pad04[0xC];
} Game174FA4Surface;

void *func_15167A68(s32, s32, s32, s32, u8, u8);
s32 func_150A3A70(s32, s32);
void func_1510F800(s32);
extern s32 D_800DDF14;
u32 func_150ADA20(void);
#define SURF ((Game174FA4Surface *)&D_800D3300)

void func_15174FA4(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                   f32 arg8, f32 arg9) {
    f32 height;
    s32 initialX;
    s32 initialHeight;
    s32 x;
    s32 z;
    s32 radius;
    s32 minimum;
    s32 count;
    s32 i;
    s32 index;
    Game174BF0Actor *actor;

    initialX = arg2;
    initialHeight = arg3;
    if (arg7 == 0) func_1510F800(0);
    index = 0;
    if (arg0 > 0) {
        do {
            actor = func_15167A68(0x13, 0, 0xF8, 1, 0xFF, 1);
            if (actor == NULL) return;
            radius = arg5 >> 1;
            height = (f32)initialHeight;
            x = (func_150ADA20() % (u32)arg5) + initialX - radius;
            z = (func_150ADA20() % (u32)arg5) + arg4 - radius;
            actor->group = arg1;
            actor->x = (f32)x;
            actor->y = height;
            actor->z = (f32)z;
            actor->timer = func_150ADA20() % 3U + 4;
            actor->enabled = 0;
            actor->pitch = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->yaw = (f32)(func_150ADA20() % 360U);
            actor->roll = (f32)((s32)(func_150ADA20() & 0x1F) - 0xF);
            actor->velocity = 0.0f;
            actor->fieldBC = 0.0f;
            actor->yawStep = 3.0f;
            actor->scale = (f32)(s32)(func_150ADA20() & 0xFFFF) * 0.000030517578125f * arg9 + arg8;
            actor->model = (u8 *)D_800DDF14;
            actor->modelIndex = 7;
            count = func_150A3A70(x, z);
            if (count == 0) {
                func_1516979C((struct102 *)actor);
            } else {
                minimum = SURF[0].height;
                for (i = 1; i < count; i++) {
                    if (SURF[i].height < minimum) {
                        minimum = SURF[i].height;
                    }
                }
                minimum >>= 8;
                actor->baseHeight = (f32)minimum;
                if (arg7 == 0) {
                    actor->phase = 0;
                    actor->y = (f32)(minimum + 10);
                    actor->flags = 1;
                } else {
                    actor->phase = 2;
                    actor->flags = 4;
                    *(f32 *)((u8 *)actor + 0xE4) = (f32)((s32)(func_150ADA20() & 0x7F) + minimum + 300);
                    actor->y = (f32)((s32)(func_150ADA20() & 0x7F) + minimum + 300);
                }
                *(f32 *)((u8 *)actor + 0xCC) = 0;
                *(s16 *)((u8 *)actor + 0xD0) = initialX;
                *(s16 *)((u8 *)actor + 0xD4) = arg4;
                *(s16 *)((u8 *)actor + 0xD6) = radius;
                *(f32 *)((u8 *)actor + 0xE0) = 0;
                *(s16 *)((u8 *)actor + 0xDC) = 0;
                actor->surfaceFlags = 0;
                actor->brightness = 0;
                actor->opacity = 0;
                actor->fieldF1 = 0;
            }
            index++;
        } while (index != arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15175390.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15175958.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_1517685C.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct15176B84;

typedef struct {
    u8  pad0[0x90];
    u8  unk90;
    u8  unk91;
    u8  pad92[0x2];
    s32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    u8  padA4[0x24];
    f32 unkC8;
    u8  padCC[0x6];
    s16 unkD2;
    u8  padD4[0x6];
    s16 unkDA;
    u8  padDC[0x8];
    f32 unkE4;
    u8  padE8[0x5];
    u8  unkED;
    u8  padEE[0x2];
    u8  unkF0;
    u8  unkF1;
} Obj15176B84;

s32 func_150AD960(s32, s32, s32, s32);
void func_15175958(Obj15176B84 *arg0);

void func_15176B84(Obj15176B84 *obj) {
    s32 fade;

    if ((((Struct15176B84 *)D_800DDE80)[obj->unkED].unk0 == 0) || (D_800DDF78[obj->unkED] == 0)) {
        func_1516972C((struct102 *)obj);
        return;
    }

    fade = obj->unkF0;
    if (obj->unkF1 == 0) {
        if (D_800DDF5C[obj->unk90] == 0) {
            obj->unkF1 = 1;
        } else if (fade != 0xFF) {
            fade += D_800BE9E4 * 4;
            if (fade >= 0x100) {
                fade = 0xFF;
            }
            obj->unkF0 = fade;
        }
    } else {
        if (fade == 0) {
            func_1516972C((struct102 *)obj);
            return;
        }
        fade -= D_800BE9E4 * 4;
        if (fade < 0) {
            fade = 0;
        }
        obj->unkF0 = fade;
    }

    D_800DDF68[obj->unkED] = 1;

    switch (obj->unk91) {
        case 0:
            if (func_150AD960((s32)D_800CC2D0[0].x_position, (s32)D_800CC2D0[0].z_position, (s32)obj->unk98,
                              (s32)obj->unkA0) < 0xC8) {
                obj->unk91 = 2;
                obj->unkE4 = (f32)((u32)func_150ADA20() % obj->unkDA) + obj->unkD2;
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk8;
            } else {
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk4;
            }
            return;
        case 2:
        case 3:
            break;
        case 4:
            if (obj->unk9C < obj->unkC8) {
                func_1516972C((struct102 *)obj);
                return;
            }
            break;
    }
    func_15175958(obj);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15176DF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_151770C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_1517725C.s")
