#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/trail.h"

/* Sprays up to 30 trails from a ring around obj; stops early when a trail can't be created. */
void trail_sprayFromRing(Actor *obj, u8 owner) {
    EffectTemplate spawn;
    TrailMotion motion;
    s32 remaining;
    s32 ok;
    TrailRender render;
    u8 yaw;
    u8 pitch;
    f32 speed;
    f32 sinPitch;
    f32 cosPitch;
    f32 sinYaw;
    f32 cosYaw;

    remaining = 30;
    ok = TRUE;
    if (obj != NULL) {
        render.otherModeH = 0;
        render.setGeometryMode = 1;
        render.clearGeometryMode = 0x160600;
        render.renderMode = 3;
        render.combiner = 0x10;
        render.colorDither = 0x80;
        render.alphaDither = 0x20;
        render.primMode = 0;
        render.envMode = 9;
        spawn.kind = 1;
        spawn.flags = 1;
        motion.texture = 8;
        motion.shade = 0xFF - ((obj->floorMaterial >> 5) & 3) * 64;
        motion.flags = 0x28;
        motion.alpha = 0xFF;
        do {
            yaw = math_random() & 0xFF; // matching: the redundant mask changes the schedule
            pitch = -(math_random() % 21U + 40);
            sinPitch = math_cosBinAngleMasked(pitch);
            cosPitch = math_cosBinAngleMasked(pitch - 0x40);
            sinYaw = math_cosBinAngleMasked(yaw);
            cosYaw = math_cosBinAngleMasked(yaw - 0x40);
            speed = math_randomFloat() * 5.0f + 10.0f;
            spawn.pos.x = obj->xpos + 100.0f * cosYaw;
            spawn.pos.y = math_randomFloat() * 15.0f + obj->ypos;
            spawn.pos.z = obj->zpos + 100.0f * sinYaw;
            spawn.itemCount = math_random() % 7U + 3;
            spawn.life = math_random() % 26U + 35;
            motion.halfWidth = math_randomFloat() * 6.0f + 2.0f;
            motion.gravity = math_randomFloat() + 1.0f;
            motion.velocity.x = speed * sinPitch * cosYaw;
            motion.velocity.y = -speed * cosPitch;
            motion.velocity.z = speed * sinPitch * sinYaw;
            if (trail_createMotionTrail(&spawn, &motion, 0, 1, 3, 0, 0, 0, 0, 0, 0, &render, 0, owner, 0) == NULL) {
                ok = FALSE;
            }
        } while (ok && --remaining != 0);
    }
}

/* MotionTrail handler (gMotionTrailUpdateProcs[3]): fades the trail out over its last 8 frames. */
s32 trail_fadeMotionTrail(MotionTrail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->head.tmpl.life * 32;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    if ((u8)alpha < 0) { // matching: never true, but the original emits the andi/bgez test
        return 0;
    }
    return 1;
}
