#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern s32 func_15145EA4(struct17 **src, struct17 **dst, void *mtx, s32 count);
extern u8 D_80088BA0;
extern struct17 D_800A2000;
extern struct17 D_800A200C;
extern struct17 D_800A2018;
extern struct17 D_800A2024;
void func_151D5148(struct127 *arg0);
void func_151D3F14(struct17 *arg0, u8 arg1, s32 arg2);
void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4);
s32 func_151C229C(struct17 *arg0, struct17 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, s32, s32);
void func_150FEC28(struct127 *actor, u8 bone, struct17 *arg2, struct17 *arg3, struct17 *position, u8 arg5, s32 arg6);

void func_150FE860(struct127 *arg0, s32 arg1, s32 arg2) {
    struct17 sp104;
    struct17 spF8;
    struct17 spEC;
    struct17 spE0;
    struct17 spD4;
    struct17 spC8;
    f32 var_f14;
    f32 var_f16;
    s32 spBC;

    func_151D5148(arg0);
    func_15145740(arg0, &spF8, &spEC, &spE0, 0.8110000491f);
    if (arg0->unk1D4 != 0) {
        s32 spB0[3];
        s32 spA4[3];

        spBC = (s32)arg0->unk1D4 + (D_80088BA0 << 6);
        spB0[0] = (s32)&D_800A2000;
        spB0[1] = (s32)&D_800A2018;
        spB0[2] = (s32)&D_800A2024;
        spA4[0] = (s32)&sp104;
        spA4[1] = (s32)&spD4;
        spA4[2] = (s32)&spC8;
        func_15145EA4(spB0, spA4, spBC, 3);
        spC8.unk0 -= spD4.unk0;
        spC8.unk4 -= spD4.unk4;
        spC8.unk8 -= spD4.unk8;
    } else {
        f32 pad2[3];

        spBC = 0;
        if ((9.999999747e-05f < fabsf(spF8.unk0)) || (9.999999747e-05f < fabsf(spF8.unk8))) {
            f32 temp_f2;

            temp_f2 = 1.0f / sqrtf((spF8.unk0 * spF8.unk0) + (spF8.unk8 * spF8.unk8));
            var_f14 = spF8.unk8 * temp_f2;
            var_f16 = -spF8.unk0 * temp_f2;
        lbl2:;
        } else {
            var_f14 = 1.0f;
            var_f16 = 0.0f;
        }
        sp104.unk0 = arg0->x_position + (34.0f * var_f16);
        sp104.unk4 = arg0->y_position + 49.0f;
        sp104.unk8 = arg0->z_position + (34.0f * var_f14);
    }
    func_151C229C(&sp104, &spF8, 0, 0, 0, 0, 300.0f, 0.003333333414f,
                  func_150ADA68() * 10.0f + 25.0f, func_150ADA68() * 200.0f + 400.0f, 50.0f,
                  (func_150ADA20() % 156U) + 100, arg0, 1, 1, 0, 0, 1, 1, 0, 0x35, 0.0f, 0xFF, -1, 0,
                  ((u8 *)&arg1)[3], arg2);
    if (arg0->unk1D4 != 0) {
        if ((arg0->unk74 & 0xF) != 0xF) {
            func_151D3F14(&sp104, ((u8 *)&arg1)[3], arg2);
            func_151D4408(&spD4, &spC8, spBC, arg0, 1.0f, ((u8 *)&arg1)[3], arg2);
            if (func_150ADA20() & 1) {
                func_150FEC28(arg0, D_80088BA0, &D_800A2000, &D_800A200C, &sp104, ((u8 *)&arg1)[3], arg2);
            }
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
