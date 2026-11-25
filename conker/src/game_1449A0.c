#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects that swing one rotation axis toward a target angle.
 * Same object layout as MoverObj in game_143DE0.c.
 */
typedef struct MoverObj {
    /* 0x00 */ f32 rot[3]; /* degrees */
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 pos[3];
    /* 0x16 */ u8 pad16[0xA];
    /* 0x20 */ Vtx *vertices[2]; /* one per frame buffer */
    /* 0x28 */ u8 pad28[0x14];
    /* 0x3C */ s32 params;
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 flags;
    /* 0x50 */ u8 pad50[0xA];
    /* 0x5A */ s16 move[3]; /* translation this frame */
    /* 0x60 */ f32 spin[3]; /* rotation speed, degrees per tick */
    /* 0x6C */ u8 pad6C[7];
    /* 0x73 */ u8 state;
    /* 0x74 */ u16 sound; /* handle from func_15114D24 */
    /* 0x76 */ u8 pad76[6];
    /* 0x7C */ union {
        struct {
            f32 damping;
            f32 acceleration;
            f32 velocity;
        } swing;
        struct {
            f32 rebound; /* fraction of the velocity kept (reversed) when the target is crossed */
            f32 acceleration; /* per tick */
            f32 velocity;
        } bounce;
    } work;
} MoverObj;

/* One row of D_80089260, selected by params >> 10. */
typedef struct MoverSounds {
    /* 0x0 */ s16 start; /* played when the object starts moving */
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 stop;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 arrive;
    /* 0xA */ s16 arriveLead;
} MoverSounds;

extern MoverSounds D_80089260[];

extern void func_15114D24(MoverObj *obj, s32 sound, s32 arg2, s16 arg3, s16 arg4, s32 flags);
f32 func_151172D8(MoverObj *obj, f32 angle);
f32 func_15117518(MoverObj *obj, f32 angle);

/* Swings rot[0] toward the target angle with func_151172D8's damped spring. */
void func_151174F0(MoverObj *obj) {
    obj->rot[0] = func_151172D8(obj, obj->rot[0]);
}

/*
 * Accelerates angle toward the target in the low 10 bits of params (signed degrees). When it crosses the
 * target it snaps there and bounces back with -rebound times its velocity; it comes to rest once close and
 * slow. Starts the params >> 10 start sound when it begins moving. Returns the new angle.
 */
f32 func_15117518(MoverObj *obj, f32 angle) {
    f32 velocity;
    s32 soundIndex;
    f32 target;
    f32 after;
    f32 before;
    f32 rebound;
    f32 acceleration;

    rebound = obj->work.bounce.rebound;
    acceleration = obj->work.bounce.acceleration * D_800BE9E4;
    velocity = obj->work.bounce.velocity;
    target = (obj->params << 22) >> 22;
    if (angle != target || velocity != 0.0f) {
        soundIndex = obj->params >> 10;
        if (velocity == 0.0f && soundIndex != 0 && D_80089260[soundIndex].start != 0) {
            func_15114D24(obj, D_80089260[soundIndex].start, 24000, 200, 2500, 0);
        }
        before = func_15048A70(angle, target);
        angle += velocity * D_800BE9E4;
        after = func_15048A70(angle, target);
        if ((after <= 0.0f && before > 0.0f) || (after >= 0.0f && before < 0.0f)) {
            angle = target;
            velocity = -rebound * velocity;
            after = 0.0; // matching: a double 0.0 (or int 0) gets IDO to reuse $f18 for the zero; 0.0f does not
        } else if (after > 0.0f) {
            velocity += acceleration;
        } else {
            velocity -= acceleration;
        }
        if (fabsf(after) < 2.0f && fabsf(velocity) < 1.0f) {
            angle = target;
            velocity = 0.0f;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        obj->work.bounce.velocity = velocity;
    }
    return angle;
}

/* Bounces rot[0] toward the target angle (func_15117518). */
void func_15117770(MoverObj *obj) {
    obj->rot[0] = func_15117518(obj, obj->rot[0]);
}

/* Bounces rot[2] toward the target angle (func_15117518). */
void func_15117798(MoverObj *obj) {
    obj->rot[2] = func_15117518(obj, obj->rot[2]);
}
