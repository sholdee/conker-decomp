#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Same layout as SpawnRegion in game_131F30.c and game_16EE20.c. */
typedef struct SpawnRegion {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; /* degrees */
    /* 0x14 */ u8 unk14;     /* the effects set it to 1 while off */
    /* 0x15 */ u8 flags;     /* bits 0-1: shape */
} SpawnRegion;

/* State at 0x110 of the region particle emitter (func_151A6600, func_151A6AB8). */
typedef struct EmitterState {
    /* 0x00 */ SpawnRegion *region;
    /* 0x04 */ f32 angle;
    /* 0x08 */ f32 angleSpeed;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ struct102 *unk10; /* released by func_151A6AB8 */
    /* 0x14 */ u8 phase;
    /* 0x15 */ u8 phaseSpeed;
    /* 0x16 */ u8 alpha;      /* oscillates in [129, 255] with phase */
    /* 0x17 */ u8 hasZone;    /* nonzero: register a zone around the object each frame */
    /* 0x18 */ f32 pending;   /* particles owed, accumulated per frame */
} EmitterState;

typedef struct EmitterObj {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x27];
    /* 0x034 */ f32 x;
    /* 0x038 */ f32 y;
    /* 0x03C */ f32 z;
    /* 0x040 */ u8 pad40[0x20];
    /* 0x060 */ s16 *unk60;
    /* 0x064 */ u8 pad64[0xAC];
    /* 0x110 */ EmitterState state;
} EmitterObj;

extern void *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
extern void func_15143794(s16 yaw, s16 pitch, f32 length, Vec3f *out);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern f32 func_15144528(f32 value, f32 hi, f32 lo);
extern s32 func_151464B8(s16 *);

/* Spins the emitter and, while func_151464B8 allows it, throws particles from random points of its region. */
s32 func_151A6600(EmitterObj *obj) {
    EmitterState *state;

    state = &obj->state;
    if (state->hasZone) {
        func_1508B20C(obj->x, obj->y, obj->z, 500.0f);
    }
    state->angle += state->angleSpeed * D_800BE9A4;
    state->angle = func_15144528(state->angle, 2048.0f, 0.0f);
    state->phase += state->phaseSpeed * D_800BE9E4;
    state->alpha = func_151423D8(state->phase - 0x40) * 63.0f + 192.0f;
    if (func_151464B8(obj->unk60) == 0) {
        state->pending += (0.3930000067f + func_150ADA68() * 0.4370000064f) * D_800BE9A4;
        if (state->pending > 1.0f) {
            ParticleDesc spawn;

            spawn.texture = 40;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x1F0600;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.r2 = 0xFF;
            spawn.g2 = 0xFF;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.b2 = 0xFF;
            spawn.fadeStart = 12;
            spawn.fadeRate = 21;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0xC207;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            do {
                func_15143874((u8)func_150ADA20(), state->region->radiusX * func_150ADA68(), &spawn.pos.x,
                              &spawn.pos.z);
                spawn.pos.x += state->region->x;
                spawn.pos.y = state->region->y;
                spawn.pos.z += state->region->z;
                spawn.timer = (func_150ADA20() & 0xF) + 25;
                spawn.alpha = func_150ADA20() % 101U + 100;
                spawn.width = spawn.height = func_150ADA68() * 205.0f + 98.0f;
                func_15143794((u8)func_150ADA20(), func_150ADA20() % 35U - 64, func_150ADA68() * 10.0f + 10.0f,
                              &spawn.vel);
                // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
                spawn.gravity = func_150ADA68() * 0.2840000093f + -0.9100000262f;
                func_15130280(&spawn, 1, 0, 0, obj->unkC, obj->unk1);
                state->pending -= 1.0f;
            } while (state->pending > 1.0f);
        }
    }
    return 1;
}

extern void func_1000FD38(void *, void *, s32, void *);

/* Shuts the emitter down: marks its region off, calls func_1000FD38 with func_1000EF40 on it, and frees unk10. */
void func_151A6AB8(EmitterObj *obj) {
    obj->state.region->unk14 = 1;
    func_1000FD38(func_1000EF40, obj->state.region, 0, obj);
    if (obj->state.unk10 != NULL) {
        func_1516972C(obj->state.unk10);
    }
}
