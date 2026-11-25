#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_150DBD70(u8 kind);

/* Spawn template of a sprite (as declared in game_169510.c). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 frame;          /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 unk10;           /* faded out with the lifetime */
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 renderModes;    /* low byte: D_800A4AC8 index, high half: combiner index */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
} SpriteDef; /* size 0x28 */

/* Sprite effect created by func_1513C350 (see game_169510.c); only the template is used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ SpriteDef def;
} SpriteFx;

/* Billboard particle (see game_1944C0.c); only the fields used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
} Particle;

/* The trap object func_1511BEBC passes to its trigger callback. */
typedef struct {
    /* 0x00 */ u8 pad0[0x72];
    /* 0x72 */ u8 tag;
} TrapTrigger;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ u8 unk20;
} EffectSpawnState;

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
} EffectSpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 padD[0x20];
    /* 0x2D */ s8 pointIndex;
    /* 0x2E */ u8 pad2E[0x66];
    /* 0x94 */ EffectSpawnPoint *points;
    /* 0x98 */ EffectSpawnState *state;
} EffectSpawnSource;

/* Sprite request for func_1513C73C, with the source's tables kept alongside. */
typedef struct {
    /* 0x00 */ SpriteDef def;
    /* 0x28 */ EffectSpawnPoint *points;
    /* 0x2C */ EffectSpawnState *state;
} EffectSpawnRequest;

/* Trap trigger callback: spawns a type 100 object whose kind follows the trap's tag (0xFB-0xFE). */
void func_150DDED0(void *arg0) {
    struct102 *temp_v0;
    s32 temp_v1;

    temp_v0 = (struct102 *)func_151491F4(0x64, 2, -1, 1, 0, 1, 0xFF, 0);
    if (temp_v0 != 0) {
        temp_v1 = ((u8 *)arg0)[0x72];
        switch (temp_v1) {
        case 0xFB:
            ((u8 *)temp_v0)[0x28] = 1;
            return;
        case 0xFC:
            ((u8 *)temp_v0)[0x28] = 2;
            return;
        case 0xFD:
            ((u8 *)temp_v0)[0x28] = 4;
            return;
        case 0xFE:
            ((u8 *)temp_v0)[0x28] = 3;
            return;
        default:
            func_1516972C(temp_v0);
            break;
        }
    }
}

s32 func_150DDF88(u8 *arg0) {
    return func_150DBD70(arg0[0x28]);
}

s32 func_150DDFAC(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnRequest tmp;
    f32 scale;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    tmp.state = arg0->state;
    tmp.points = arg0->points;
    scale = func_150ADA68() * 70.0f + 10.0f;
    tmp.state->unk20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        tmp.def.texture = 0x1D;
        break;
    case 1:
        tmp.def.texture = 0x1E;
        break;
    }

    tmp.def.frame = 0;
    tmp.def.frameSpeed = 0;
    tmp.def.flags = 0x11;
    tmp.def.lifetime = 0x64;
    tmp.def.unk10 = 0xFF;
    tmp.def.unk11 = 0xFF;
    tmp.def.rgba[0] = 0;
    tmp.def.rgba[1] = 0;
    tmp.def.rgba[2] = 0;
    tmp.def.rgba[3] = 0xFF;
    tmp.def.renderModes = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 5, 0, arg5,
                  tmp.points[arg0->pointIndex].x,
                  arg4,
                  tmp.points[arg0->pointIndex].z,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->unk0C, arg0->unk01);
    return 1;
}

s32 func_150DE12C(EffectSpawnSource *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnRequest tmp;
    f32 scale;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    tmp.state = arg0->state;
    tmp.points = arg0->points;
    scale = func_150ADA68() * 10.0f + 10.0f;
    tmp.state->unk20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        tmp.def.texture = 0x1B;
        break;
    case 1:
        tmp.def.texture = 0x1C;
        break;
    }

    tmp.def.frame = 0;
    tmp.def.frameSpeed = 0;
    tmp.def.flags = 0x11;
    tmp.def.lifetime = 0x64;
    tmp.def.unk10 = 0xFF;
    tmp.def.unk11 = 0xFF;
    tmp.def.rgba[0] = 0;
    tmp.def.rgba[1] = 0;
    tmp.def.rgba[2] = 0;
    tmp.def.rgba[3] = 0xAA;
    tmp.def.renderModes = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 5, 0, arg5,
                  tmp.points[arg0->pointIndex].x,
                  arg4,
                  tmp.points[arg0->pointIndex].z,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->unk0C, arg0->unk01);
    return 1;
}

s32 func_150DE2A4(void *arg0) {
    s16 temp = *(s16 *)((u8 *)arg0 + 0x1C);
    if (temp < 0x20) {
        *(s8 *)((u8 *)arg0 + 0x28) = temp << 3;
    }
    return 1;
}

void func_150DE2C4(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x34) = *(s16 *)((u8 *)arg0 + 0x34) - 0x1A;
    *(s16 *)((u8 *)arg0 + 0x36) = *(s16 *)((u8 *)arg0 + 0x36) - 0x1A;
    if (*(s16 *)((u8 *)arg0 + 0x34) >= 0xA && *(s16 *)((u8 *)arg0 + 0x36) >= 0xA) {
        return;
    }
    *(s16 *)((u8 *)arg0 + 0x38) = 0;
}
