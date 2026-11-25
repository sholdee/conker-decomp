#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 func_15145EA4(struct17 **src, struct17 **dst, void *mtx, s32 count);
extern u8 D_80088BA0;
extern struct17 D_800A2000;
extern struct17 D_800A200C;
extern struct17 D_800A2018;
extern struct17 D_800A2024;
void func_151D5148(struct127 *obj);
void func_151D3F14(struct17 *pos, u8 arg1, s32 arg2);
void func_15145740(struct127 *obj, struct17 *direction, struct17 *axis0, struct17 *axis1, f32 arg4);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, u8 arg25, s32 arg26);
void func_151D4408(struct17 *pos, struct17 *dir, void *mtx, struct127 *owner, f32 scale, u8 arg5, s32 arg6);
void func_150FEC28(struct127 *actor, u8 bone, struct17 *arg2, struct17 *arg3, struct17 *position, u8 arg5, s32 arg6);

/* Spawns a projectile (func_151C229C) along `actor`'s direction. With bone matrices it starts at
 * bone D_80088BA0 and is followed by the func_151D3F14 / func_151D4408 / func_150FEC28 effects;
 * without them it starts 34 units to the side of and 49 units above the actor. */
void func_150FE860(struct127 *actor, u8 arg1, s32 arg2) {
    struct17 position;
    struct17 direction;
    struct17 axis0;
    struct17 axis1;
    struct17 origin;
    struct17 delta;
    f32 sideZ;
    f32 sideX;
    void *mtx;

    func_151D5148(actor);
    func_15145740(actor, &direction, &axis0, &axis1, 0.8110000491f);
    if (actor->unk1D4 != NULL) {
        struct17 *sources[3];
        struct17 *destinations[3];

        mtx = ((f32 (*)[4][4])actor->unk1D4)[D_80088BA0];
        sources[0] = &D_800A2000;
        sources[1] = &D_800A2018;
        sources[2] = &D_800A2024;
        destinations[0] = &position;
        destinations[1] = &origin;
        destinations[2] = &delta;
        func_15145EA4(sources, destinations, mtx, 3);
        delta.unk0 -= origin.unk0;
        delta.unk4 -= origin.unk4;
        delta.unk8 -= origin.unk8;
    } else {
        f32 x = direction.unk0;
        f32 z = direction.unk8;

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
        position.unk0 = actor->x_position + 34.0f * sideX;
        position.unk4 = actor->y_position + 49.0f;
        position.unk8 = actor->z_position + 34.0f * sideZ;
    }
    func_151C229C(&position, &direction, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                  func_150ADA68() * 10.0f + 25.0f, func_150ADA68() * 200.0f + 400.0f, 50.0f,
                  (func_150ADA20() % 156U) + 100, actor, 1, 1, 0, 0, 1, 1, 0, 0x35, 0.0f, 0xFF, -1, 0,
                  arg1, arg2);
    if (actor->unk1D4 != NULL && (actor->unk74 & 0xF) != 0xF) {
        func_151D3F14(&position, arg1, arg2);
        func_151D4408(&origin, &delta, mtx, actor, 1.0f, arg1, arg2);
        if (func_150ADA20() & 1) {
            func_150FEC28(actor, D_80088BA0, &D_800A2000, &D_800A200C, &position, arg1, arg2);
        }
    }
}

/* Transforms the point D_800A2000 by bone D_80088BA0 of `actor` into `*out`. */
void func_150FEBC8(struct127 *actor, s32 arg1, struct17 *out) {
    void *mtx;
    struct17 *src;
    struct17 *dst;

    mtx = ((f32 (*)[4][4])actor->unk1D4)[D_80088BA0];
    src = &D_800A2000;
    dst = out;
    func_15145EA4(&src, &dst, mtx, 1);
}

extern void func_15102B38(struct127 *obj, u8 bone, struct17 *arg2, struct17 *arg3, f32 *arg4, s32 arg5, s32 arg6,
                          f32 arg7, struct17 *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);

/* Spawns a randomized effect (func_15102B38) on bone `bone` of `actor` at `position`. */
void func_150FEC28(struct127 *actor, u8 bone, struct17 *arg2, struct17 *arg3, struct17 *position, u8 arg5, s32 arg6) {
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
