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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15176B84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_15176DF0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_151770C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A20A0/func_1517725C.s")
