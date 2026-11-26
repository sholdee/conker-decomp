#include <ultra64.h>
#include "functions.h"
#include "variables.h"



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
void func_150BD740(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f points[4];
    Vec3f dirFrom[4];
    Vec3f dirTo[4];
    s32 i;
    Vec3f dir[4];

    if (actor != NULL) {
        if (actor->ob_matrix_ptr != NULL) {
            for (i = 0; i < 4; i++) {
                func_15143134(&D_8009FF40[i], &points[i], actor->ob_matrix_ptr);
                func_15143134(&D_8009FF70[i], &dirFrom[i], actor->ob_matrix_ptr);
                func_15143134(&D_8009FFA0[i], &dirTo[i], actor->ob_matrix_ptr);
            }
            dir[2].x = dirTo[2].x - dirFrom[2].x;
            dir[2].y = dirTo[2].y - dirFrom[2].y;
            dir[2].z = dirTo[2].z - dirFrom[2].z;
            func_1514C470(points[2].x, points[2].y, points[2].z, points[3].x, points[3].y, points[3].z,
                          func_150ADA68() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[2], arg1);
            dir[3].x = dirTo[3].x - dirFrom[3].x;
            dir[3].y = dirTo[3].y - dirFrom[3].y;
            dir[3].z = dirTo[3].z - dirFrom[3].z;
            func_1514C470(points[3].x, points[3].y, points[3].z, points[0].x, points[0].y, points[0].z,
                          func_150ADA68() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[3], arg1);
        }
    }
}

/* Spawns one trail at (x, y, z) moving along dir. */
s32 func_150BD954(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *dir, u8 arg14) {
    EffectTemplate20 spawn;
    TrailMotion motion;
    f32 speed;
    TrailRender render;

    render.unk0 = 0;
    render.setGeometryMode = 1;
    render.clearGeometryMode = 0x160600;
    render.renderMode = 3;
    render.unk10 = 0x10;
    render.unk14 = 0x80;
    render.unk18 = 0x20;
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

    speed = (func_150ADA68() * 127.0f + 85.0f) * 0.001f;
    motion.velocity.x = dir[0] * speed;
    motion.velocity.y = dir[1] * speed;
    motion.velocity.z = dir[2] * speed;
    motion.alpha = func_150ADA20() % 86U + 180;
    spawn.itemCount = (func_150ADA20() & 3) + 3;
    spawn.life = func_150ADA20() % 31U + 39;
    motion.halfWidth = (func_150ADA68() * 602.0f + 407.0f) * 0.01f;
    motion.gravity = (func_150ADA68() * 356.0f + 1017.0f) * 0.001f;
    func_15147DA0(&spawn, &motion, 0, 1, 11, 0, 0, 0, 0, 0, 0, &render, 0, arg14, 1);
    return 1;
}

struct Sub150BDB3C {
    /* 0x00 */ u8 pad0[0x1B];
    /* 0x1B */ u8 unk1B;
};

struct Obj150BDB3C {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[0x98 - 0x1E];
    /* 0x98 */ struct Sub150BDB3C *unk98;
};

/* Caps obj->unk98->unk1B at obj->unk1C * 8. */
s32 func_150BDB3C(struct Obj150BDB3C *obj) {
    struct Sub150BDB3C *sub = obj->unk98;
    s32 limit = obj->unk1C * 8;

    if (limit < sub->unk1B) {
        sub->unk1B = limit;
    }
    return 1;
}
