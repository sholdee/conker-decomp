#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/trail.h"



const Vec3f D_8009FF40[4] = {
    { -270.0f, 3.0f, -270.0f },
    { 270.0f, 3.0f, -270.0f },
    { 270.0f, 3.0f, 270.0f },
    { -270.0f, 3.0f, 270.0f },
};
const Vec3f D_8009FF70[4] = {
    { 0.0f, 3.0f, -270.0f },
    { 270.0f, 3.0f, 0.0f },
    { 0.0f, 3.0f, 270.0f },
    { -270.0f, 3.0f, 0.0f },
};
const Vec3f D_8009FFA0[4] = {
    { 0.0f, 139.0f, -360.0f },
    { 360.0f, 139.0f, 0.0f },
    { 0.0f, 139.0f, 360.0f },
    { -360.0f, 139.0f, 0.0f },
};

/* Transforms three 4-point tables by the actor's ob_matrix_ptr and emits two segments (point 2->3 and 3->0). */
void func_150BD740(Actor *actor, u8 owner, s32 list) {
    Vec3f points[4];
    Vec3f dirFrom[4];
    Vec3f dirTo[4];
    s32 i;
    Vec3f dir[4];

    if (actor != NULL) {
        if (actor->ob_matrix_ptr != NULL) {
            for (i = 0; i < 4; i++) {
                math_transformOffset(&D_8009FF40[i], &points[i], actor->ob_matrix_ptr);
                math_transformOffset(&D_8009FF70[i], &dirFrom[i], actor->ob_matrix_ptr);
                math_transformOffset(&D_8009FFA0[i], &dirTo[i], actor->ob_matrix_ptr);
            }
            dir[2].x = dirTo[2].x - dirFrom[2].x;
            dir[2].y = dirTo[2].y - dirFrom[2].y;
            dir[2].z = dirTo[2].z - dirFrom[2].z;
            particle_emitOnLine(points[2].x, points[2].y, points[2].z, points[3].x, points[3].y, points[3].z,
                          math_randomFloat() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[2], owner);
            dir[3].x = dirTo[3].x - dirFrom[3].x;
            dir[3].y = dirTo[3].y - dirFrom[3].y;
            dir[3].z = dirTo[3].z - dirFrom[3].z;
            particle_emitOnLine(points[3].x, points[3].y, points[3].z, points[0].x, points[0].y, points[0].z,
                          math_randomFloat() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[3], owner);
        }
    }
}

/* Spawns one trail at (x, y, z) moving along dir. */
s32 func_150BD954(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX,
                  f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius, s32 arg11,
                  s32 arg12, f32 *dir, u8 owner) {
    u8 pad[4]; // matching: frame size
    EffectTemplate spawn;
    TrailMotion motion;
    f32 speed;
    TrailRender render;

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
    spawn.pos.x = x;
    spawn.pos.y = y;
    motion.unk14 = y;
    motion.texture = 9;
    motion.shade = 0xFF;
    motion.flags = 8;
    spawn.pos.z = z;

    speed = (math_randomFloat() * 127.0f + 85.0f) * 0.001f;
    motion.velocity.x = dir[0] * speed;
    motion.velocity.y = dir[1] * speed;
    motion.velocity.z = dir[2] * speed;
    motion.alpha = math_random() % 86U + 180;
    spawn.itemCount = (math_random() & 3) + 3;
    spawn.life = math_random() % 31U + 39;
    motion.halfWidth = (math_randomFloat() * 602.0f + 407.0f) * 0.01f;
    motion.gravity = (math_randomFloat() * 356.0f + 1017.0f) * 0.001f;
    trail_createMotionTrail(&spawn, &motion, 0, 1, 11, 0, 0, 0, 0, 0, 0, &render, 0, owner, 1);
    return 1;
}

/* Motion trail update (gMotionTrailUpdateProcs[11], the trails func_150BD954 creates): caps the trail's alpha at its
 * life * 8. */
s32 effect_fadeSegmentTrailByLife(MotionTrail *obj) {
    TrailParams *sub = obj->params;
    s32 limit = obj->head.tmpl.life * 8;

    if (limit < sub->motion.alpha) {
        sub->motion.alpha = limit;
    }
    return 1;
}
