#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_80088BA0;
extern Vec3f D_800A2000;
extern Vec3f D_800A200C;
extern Vec3f D_800A2018;
extern Vec3f D_800A2024;

/* Spawns a projectile (func_151C229C) along `actor`'s direction. With bone matrices it starts at
 * bone D_80088BA0 and is followed by the func_151D3F14 / func_151D4408 / func_150FEC28 effects;
 * without them it starts 34 units to the side of and 49 units above the actor. */
void func_150FE860(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f position;
    Vec3f direction;
    Vec3f axis0;
    Vec3f axis1;
    Vec3f origin;
    Vec3f delta;
    f32 sideZ;
    f32 sideX;
    void *mtx;

    func_151D5148(actor);
    func_15145740(actor, &direction, &axis0, &axis1, 0.8110000491f);
    if (actor->ob_matrix_ptr != NULL) {
        Vec3f *sources[3];
        Vec3f *destinations[3];

        mtx = actor->ob_matrix_ptr[D_80088BA0];
        sources[0] = &D_800A2000;
        sources[1] = &D_800A2018;
        sources[2] = &D_800A2024;
        destinations[0] = &position;
        destinations[1] = &origin;
        destinations[2] = &delta;
        func_15145EA4(sources, destinations, mtx, 3);
        delta.x -= origin.x;
        delta.y -= origin.y;
        delta.z -= origin.z;
    } else {
        f32 x = direction.x;
        f32 z = direction.z;

        mtx = NULL;
        if (0.0001f < fabsf(x) || 0.0001f < fabsf(z)) {
            f32 length = sqrtf(x * x + z * z);
            f32 inverse = 1.0f / length;

            sideZ = z * inverse;
            sideX = -x * inverse;
        } else {
            sideZ = 1.0f;
            sideX = 0.0f;
        }
        position.x = actor->xpos + 34.0f * sideX;
        position.y = actor->ypos + 49.0f;
        position.z = actor->zpos + 34.0f * sideZ;
    }
    func_151C229C(&position, &direction, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                  func_150ADA68() * 10.0f + 25.0f, func_150ADA68() * 200.0f + 400.0f, 50.0f,
                  (func_150ADA20() % 156U) + 100, actor, 1, 1, 0, 0, 1, 1, 0, 0x35, 0.0f, 0xFF, -1, 0,
                  arg1, arg2);
    if (actor->ob_matrix_ptr != NULL && (actor->unk74 & 0xF) != 0xF) {
        func_151D3F14(&position, arg1, arg2);
        func_151D4408(&origin, &delta, mtx, actor, 1.0f, arg1, arg2);
        if (func_150ADA20() & 1) {
            func_150FEC28(actor, D_80088BA0, &D_800A2000, &D_800A200C, &position, arg1, arg2);
        }
    }
}

/* Transforms the point D_800A2000 by bone D_80088BA0 of `actor` into `*out`. */
void func_150FEBC8(Actor *actor, s32 arg1, Vec3f *out) {
    void *mtx;
    Vec3f *src;
    Vec3f *dst;

    mtx = actor->ob_matrix_ptr[D_80088BA0];
    src = &D_800A2000;
    dst = out;
    func_15145EA4(&src, &dst, mtx, 1);
}

/* Spawns a randomized effect (func_15102B38) on bone `bone` of `actor` at `position`. */
void func_150FEC28(Actor *actor, u8 bone, Vec3f *arg2, Vec3f *arg3, Vec3f *position, u8 arg5, s32 arg6) {
    f32 motion[2];
    s32 pad; // matching: unused stack slot between motion and random
    s32 random[2];

    motion[1] = func_150ADA68() * 1.001f + 0.6040000319f;
    motion[0] = func_150ADA68() * 5.9f + 14.1f;
    random[0] = func_150ADA20();
    random[1] = func_150ADA20();
    func_15102B38(actor, bone, arg2, arg3, motion, (random[0] & 3) + 6, 0xFF, func_150ADA68() * 270.0f + 359.0f,
                  position, 0xFF, 0, -1, arg5, arg6);
}
