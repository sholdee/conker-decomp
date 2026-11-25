#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Bone whose matrix places the effect points below. */
extern u8 D_80088B94;
/* Effect points in that bone's local space. */
extern struct17 D_800A1FC8;
extern struct17 D_800A1FD4;
extern struct17 D_800A1FE0;

s32 func_15145EA4(struct17 **src, struct17 **dst, Mtx *mtx, s32 count);
void func_151D3E6C(struct127 *obj, struct17 *arg1, struct17 *arg2, s32 arg3);
void func_151D3F14(struct17 *position, u8 arg1, s32 arg2);
void func_150FE604(struct127 *obj, struct17 *position, u8 arg2, s32 arg3);
void func_150FE7D4(struct127 *obj);
void func_15102B38(struct127 *obj, u8 bone, struct17 *arg2, struct17 *arg3, f32 *arg4, s32 arg5, s32 arg6, f32 arg7,
                   struct17 *position, s32 arg9, s32 arg10, s32 arg11, u8 arg12, s32 arg13);
void func_15145740(struct127 *obj, struct17 *direction, struct17 *axis0, struct17 *axis1, f32 arg4);
void func_150FE49C(struct127 *obj, u8 arg1, s32 arg2, struct17 *outPosition, struct17 *outOrigin);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);

/* Runs the func_150FE49C effects for `obj`, then spawns a func_151C229C effect from the result. */
s32 func_150FE320(struct127 *obj, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    struct17 axis0;
    struct17 position;
    struct17 origin;
    struct17 direction;
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
    randomInt = func_150ADA20();

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
void func_150FE49C(struct127 *obj, u8 arg1, s32 arg2, struct17 *outPosition, struct17 *outOrigin) {
    struct17 position;
    struct17 origin;
    struct17 *src[2];
    struct17 *dst[2];

    if (obj != NULL) {
        if (obj->unk1D4 != NULL) {
            src[0] = &D_800A1FC8;
            src[1] = &D_800A1FD4;
            dst[0] = &position;
            dst[1] = &origin;
            func_15145EA4(src, dst, (Mtx *)obj->unk1D4 + D_80088B94, 2);
        } else {
            position.unk0 = obj->x_position;
            position.unk4 = obj->y_position + 70.0f;
            position.unk8 = obj->z_position;
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
void func_150FE604(struct127 *obj, struct17 *position, u8 arg2, s32 arg3) {
    f32 motion[2];
    u8 mask;
    s32 pad; // matching: unused stack slot
    s32 random1;
    s32 random0;

    if (obj->unk1D4 != NULL) {
        if ((obj->unk74 & 0xF) != 0xF) {
            motion[1] = func_150ADA68() * (1.0f + 1.0f) + 3.7099998f; // matching: a plain 2.0f literal mismatches
            motion[0] = func_150ADA68() * 15.0f + 35.0f;

            // byte 0x197 of unk31C is padding in the shared struct126 declaration
            if ((obj->unk31C != NULL) && (((u8 *)obj->unk31C)[0x197] != 0) && (obj->camera != NULL)) {
                mask = ~(1 << obj->camera->unk23D) & 0xFF;
            } else {
                mask = 0xFF;
            }

            random0 = func_150ADA20();
            random1 = func_150ADA20();
            func_15102B38(obj, D_80088B94, &D_800A1FC8, &D_800A1FE0, motion,
                          (random0 % 3U) + 4, (random1 % 56U) + 200,
                          func_150ADA68() * 301.0f + 500.0f, position, mask,
                          0, -1, arg2, arg3);
        }
    }
}

void func_151D3E04(struct127 *obj, struct17 *out, struct17 *localPos, u8 bone, f32 yOffset);

/* Handler: writes the world position of `obj`'s bone point D_800A1FC8 to `out`. */
void func_150FE794(struct127 *obj, s32 arg1, struct17 *out) {
    func_151D3E04(obj, out, &D_800A1FC8, D_80088B94, 0.0f);
}

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Event151D8868;

void func_151D8868(Event151D8868 *event, s32 arg1, s32 arg2, s32 arg3);

/* Sends a func_151D8868 event (kind 1, 20-32 long) to `obj`'s camera, if it has one. */
void func_150FE7D4(struct127 *obj) {
    Event151D8868 event;

    if (obj->camera != NULL) {
        event.unk0 = 1;
        event.unk2 = (func_150ADA20() % 13U) + 20;
        event.unk5 = 1 << obj->camera->unk23D;
        func_150ADA20(); // draws a random number that is never used
        event.unk4 = 8;
        event.unk6 = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}
