#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8, but it returns a full 32-bit random
 * value (dsra32 v0) and func_151AF388 spills it as a word, so this file declares its own. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

/* Descriptor copied into a new emitter effect by func_1513418C (same layout as game_10CD70.c's
 * BloodDripDesc and game_117490.c's EmitterDesc). */
typedef struct {
    /* 0x00 */ s32 target[2];
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ struct127 *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 offset[3];
    /* 0x20 */ f32 radius;
    /* 0x24 */ f32 rate;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ u8 flags;
    /* 0x2B */ u8 emitter;
    /* 0x2C */ s8 callback;
    /* 0x2D */ u8 unk2D;
} EmitterDesc;

/* The leading bytes of an emitter effect (cf. game_10CD70.c's EffectHeader). */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 unkC;
} EffectHeader;

void func_1516962C(s32 type, struct127 *actor, u8 eventType);
void *func_1513418C(EmitterDesc *desc, s32 arg1, u8 arg2, s32 arg3);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC);

/* Attaches emitter 10 (func_151AF338) to bone 1 of `actor` for 320 frames. */
void *func_151AF270(struct127 *actor, u8 arg1, s32 arg2) {
    void *result;
    EmitterDesc desc;

    if (actor == NULL) {
        return NULL;
    }
    func_1516962C(40, actor, 22);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset[0] = 0.0f;
    desc.offset[1] = 0.0f;
    desc.offset[2] = 0.0f;
    desc.radius = 9.800000191f;
    desc.rate = 0.1430000067f;
    desc.flags = 6; /* ON_BONE | LIFETIME (see game_161520.c) */
    desc.lifetime = 320;
    desc.emitter = 10;
    desc.callback = -1;
    desc.unk2D = 9;
    result = func_1513418C(&desc, 0, arg1, arg2);
    return result;
}

/* Emitter callback 10 (D_80089A70): spawns one particle at (x, y, z) pushed against the
 * emitter's movement (dx, dy, dz). */
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151AF388(position, dx, dy, dz, effect->unkC);
}

/* Spawns one randomised func_151D9014 particle at `position`, moving at -0.13 * (dx, dy, dz). */
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC) {
    f32 factor = 0.13000001f;
    f32 velocity[3];
    u8 flag;

    velocity[0] = -dx * factor;
    velocity[1] = -dy * factor;
    velocity[2] = -dz * factor;
    flag = (func_150ADA20() & 1) ? 1 : 0;
    func_151D9014(position, velocity, 0,
                  func_150ADA68() * 0.126f + -0.702f,
                  (func_150ADA20() & 0xF) + 10,
                  func_150ADA20() % 45 + 137,
                  func_150ADA68() * 202.0f + 142.0f,
                  flag, 1.7f, 1.7f, 0, 0, 1, 0,
                  (u8)effectUnkC, 1);
}
