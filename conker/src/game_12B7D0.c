#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Bone whose matrix places the effect points below. */
extern u8 D_80088B94;
/* Effect points in that bone's local space. */
extern Vec3f D_800A1FC8;
extern Vec3f D_800A1FD4;
extern Vec3f D_800A1FE0;

/* Runs the func_150FE49C effects for `obj`, then spawns a func_151C229C effect from the result. */
s32 func_150FE320(Actor *obj, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    Vec3f axis0;
    Vec3f position;
    Vec3f origin;
    Vec3f direction;
    u8 randomInt;
    f32 random1;
    f32 random0;

    if (obj == NULL) {
        return 0;
    }

    func_15145740(obj, &direction, &axis0, NULL, 0.0f);
    func_150FE49C(obj, arg1, arg2, &position, &origin);

    random0 = func_150ADA68();
    random1 = func_150ADA68();
    randomInt = func_150ADA20_u8();

    return func_151C229C(&origin, &direction, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                         random0 * 10.0f + 25.0f,
                         random1 * 200.0f + 600.0f, 50.0f,
                         (randomInt % 56U) + 200, obj, 1, 1, 0, 0xFF, 1, 1,
                         0, 0x23, 0.0f, 0xFF, -1, 0, arg1, arg2);
}

/*
 * Computes the effect position and origin of `obj` (its bone points D_800A1FC8 / D_800A1FD4, or 70 units
 * above it when it has no bone matrices), runs the effects there and copies both to the non-NULL outputs.
 */
void func_150FE49C(Actor *obj, u8 arg1, s32 arg2, Vec3f *outPosition, Vec3f *outOrigin) {
    Vec3f position;
    Vec3f origin;
    const Vec3f *src[2];
    Vec3f *dst[2];

    if (obj != NULL) {
        if (obj->ob_matrix_ptr != NULL) {
            src[0] = &D_800A1FC8;
            src[1] = &D_800A1FD4;
            dst[0] = &position;
            dst[1] = &origin;
            func_15145EA4(src, dst, (Mtx *)obj->ob_matrix_ptr + D_80088B94, 2);
        } else {
            position.x = obj->xpos;
            position.y = obj->ypos + 70.0f;
            position.z = obj->zpos;
            origin = position;
        }
        func_151D3E6C(obj, &position, &origin, 0x23);
        func_151D3F14(&position, arg1, arg2);
        func_150FE604(obj, &position, arg1, arg2);
        func_150FE7D4(obj);
        if (outPosition != NULL) {
            *outPosition = position;
        }
        if (outOrigin != NULL) {
            *outOrigin = origin;
        }
    }
}

/*
 * Spawns a func_15102B38 effect for `obj` between its bone points D_800A1FC8 and D_800A1FE0, from `position`;
 * when the object has bone matrices and unk74's low nibble isn't 0xF.
 */
void func_150FE604(Actor *obj, Vec3f *position, u8 arg2, s32 arg3) {
    ParticleSize motion;
    u8 mask;
    s32 pad; // matching: unused stack slot
    s32 random1;
    s32 random0;

    if (obj->ob_matrix_ptr != NULL) {
        if ((obj->unk74 & 0xF) != 0xF) {
            motion.height = func_150ADA68() * (1.0f + 1.0f) + 3.7099998f; // matching: a plain 2.0f literal mismatches
            motion.width = func_150ADA68() * 15.0f + 35.0f;

            if ((obj->mv != NULL) && (obj->mv->SniperState != 0) && (obj->CameraPtr != NULL)) {
                mask = ~(1 << obj->CameraPtr->CNumber) & 0xFF;
            } else {
                mask = 0xFF;
            }

            random0 = func_150ADA20_u8();
            random1 = func_150ADA20_u8();
            func_15102B38(obj, D_80088B94, &D_800A1FC8, &D_800A1FE0, &motion,
                          (random0 % 3U) + 4, (random1 % 56U) + 200,
                          func_150ADA68() * 301.0f + 500.0f, position, mask,
                          0, -1, arg2, arg3);
        }
    }
}

/* Handler: writes the world position of `obj`'s bone point D_800A1FC8 to `out`. */
void func_150FE794(Actor *obj, s32 arg1, Vec3f *out) {
    func_151D3E04(obj, out, &D_800A1FC8, D_80088B94, 0.0f);
}


/* Sends a func_151D8868 event (kind 1, 20-32 long) to `obj`'s camera, if it has one. */
void func_150FE7D4(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (func_150ADA20_u8() % 13U) + 20;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        func_150ADA20_u8(); // draws a random number that is never used
        event.strength = 8;
        event.updateKind = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}
