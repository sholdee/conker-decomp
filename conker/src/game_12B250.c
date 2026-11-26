#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata: the muzzle point, the end of the muzzle flash beam and the launch point, in bone space. */
static const Vec3f D_800A1F70 = { 7.0f, -136.0f, 45.0f };
static const Vec3f D_800A1F7C = { 7.0f, -169.0f, 51.0f };
static const Vec3f D_800A1F88 = { 7.0f, 40.0f, 13.0f };

/* This file's .data. */
static u8 D_80088B80 = 9; // bone of the muzzle points above

/* Fires a projectile (func_151C229C) from obj's launch point towards its target, after the muzzle
 * effects of func_150FDF38. Returns func_151C229C's result, or NULL without an obj. */
// matching: arg1 is a u8 read from its stack slot at each use; a K&R u8 parameter is
// narrowed once into a spilled copy instead
Game1EF500Trail *func_150FDDA0(Actor *obj, s32 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    Vec3f dir;
    Vec3f muzzle;
    Vec3f target;
    Vec3f launch;
    u32 random;
    s32 kind;
    f32 rand1;
    f32 rand0;

    if (obj == NULL) {
        return NULL;
    }

    func_15145740(obj, &target, &dir, NULL, 0.0f);
    func_150FDF38(obj, ((u8 *)&arg1)[3], arg2, &muzzle, &launch);

    rand0 = func_150ADA68();
    rand1 = func_150ADA68();
    random = func_150ADA20();
    kind = (level == 0x2B) ? 0x27 : 0x1A;

    return func_151C229C(&launch, NULL, &launch, &target, 0, 0, 300.0f, 0.003333333414f, rand0 * 10.0f + 25.0f,
                         rand1 * 201.0f + 601.0f, 50.0f, (random % 56U) + 200, obj, 1, 1, 0, 0xFF, 1, 1, 0, kind,
                         0.0f, 0xFF, -1, 0, ((u8 *)&arg1)[3], arg2);
}

/* Computes obj's muzzle and launch points (bone D_80088B80, or 56 above its origin without bone
 * matrices), runs the muzzle effects there and optionally returns both points. */
void func_150FDF38(obj, arg1, arg2, muzzleOut, launchOut)
    Actor *obj;
    u8 arg1;
    s32 arg2;
    Vec3f *muzzleOut;
    Vec3f *launchOut;
{
    Vec3f muzzle;
    Vec3f launch;
    const Vec3f *src[2];
    Vec3f *dst[2];

    if (obj != NULL) {
        if (obj->ob_matrix_ptr != NULL) {
            src[0] = &D_800A1F70;
            src[1] = &D_800A1F88;
            dst[0] = &muzzle;
            dst[1] = &launch;
            func_15145EA4(src, dst, obj->ob_matrix_ptr + D_80088B80, 2);
        } else {
            muzzle.x = obj->xpos;
            muzzle.y = obj->ypos + 56.0f;
            muzzle.z = obj->zpos;
            launch = muzzle;
        }
        func_151D3E6C(obj, &muzzle, &launch, (level == 0x2B) ? 0x28 : 0x1E);
        func_151D3F14(&muzzle, arg1, arg2);
        func_150FE0B8(obj, &muzzle, arg1, arg2);
        func_150FE288(obj);
        if (muzzleOut != NULL) {
            *muzzleOut = muzzle;
        }
        if (launchOut != NULL) {
            *launchOut = launch;
        }
    }
}

/* Muzzle effect (func_15102B38) at `muzzle` for objects with bone matrices, hidden from the
 * camera of obj's mv target while that has SniperState set. */
void func_150FE0B8(obj, muzzle, arg2, arg3)
    Actor *obj;
    Vec3f *muzzle;
    u8 arg2;
    s32 arg3;
{
    ParticleSize params;
    u8 cameraMask;
    s32 pad; // matching: unused stack slot
    s32 random1;
    s32 random0;

    if (obj->ob_matrix_ptr != NULL && (obj->unk74 & 0xF) != 0xF) {
        params.height = func_150ADA68() * 1.5f + 2.0f;
        params.width = func_150ADA68() * 10.10000038f + 2.059999943f;

        if (obj->mv != NULL && obj->mv->SniperState != 0 && obj->CameraPtr != NULL) {
            cameraMask = ~(1 << obj->CameraPtr->CNumber) & 0xFF;
        } else {
            cameraMask = 0xFF;
        }

        random0 = func_150ADA20();
        random1 = func_150ADA20();
        func_15102B38(obj, D_80088B80, &D_800A1F70, &D_800A1F7C, &params, (random0 % 5U) + 6, (random1 % 101U) + 155,
                      func_150ADA68() * 199.0f + 501.0f, muzzle, cameraMask, 0, -1, arg2, arg3);
    }
}

/* Handler: writes the world position of obj's muzzle point to `out`. */
void func_150FE248(Actor *obj, s16 arg1, Vec3f *out) {
    func_151D3E04(obj, out, &D_800A1F70, D_80088B80, 0.0f);
}

/* Sends a func_151D8868 event (type 1, random duration 15-23) to the camera of obj. */
void func_150FE288(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (func_150ADA20() % 9U) + 15;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        event.strength = (func_150ADA20() & 3) + 3;
        event.updateKind = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}
