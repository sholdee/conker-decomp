#include <ultra64.h>
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


/* Ground probe result filled by func_15046C80 (as game_105760.c's GroundHit). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
} GroundHit; /* 0x24 */

// matching: functions.h declares func_150ADA20 as returning u8; the % below needs the full u32
extern u32 func_150ADA20(void);
void func_1504715C(GroundHit *, struct127 *);
s32 func_15046C80(struct17 *, s32, f32, GroundHit *);
void func_151D3FF4(struct17 *, u8, s32);
void func_150E7FEC(f32, u8, s16 *, struct17 *, s32, s32, s32, s32, s32, s32, u8, s32);
void func_150E83AC(struct17 *, s16, u8, s32);
void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D40D4(struct17 *, s32, struct127 *, struct127 *, s32, s32, s32, s32);
void func_151541B8(struct17 *, f32, f32, f32, f32, u8, s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, u8, s32);

/* Spawns a burst of effects at obj: two sounds, a ground splash when there is floor below, and particles. */
void func_150E0870(struct127 *obj, u8 arg1, s32 arg2) {
    struct17 origin;
    struct17 impact;
    u8 hitGround;

    if (obj == NULL) {
        return;
    }
    if (D_800C35EA != 1) {
        func_10010630(0x2BB, obj, 0x7FFF, 1000, 2000);
        func_10010630(0x2BA, obj, 0x7FFF, 1000, 2000);
    }
    origin.unk0 = obj->x_position;
    origin.unk4 = obj->y_position + 20.0f;
    origin.unk8 = obj->z_position;
    hitGround = FALSE;
    {
        struct17 top;
        GroundHit hit;

        top.unk0 = origin.unk0;
        top.unk4 = origin.unk4 + 100.0f;
        top.unk8 = origin.unk8;
        func_1504715C(&hit, obj);
        if (func_15046C80(&top, 0, origin.unk4 - 200.0f, &hit) && (hit.flags & 1)) {
            impact.unk0 = top.unk0;
            hitGround = TRUE;
            impact.unk4 = hit.height;
            impact.unk8 = top.unk8;
            func_150E7FEC(func_150ADA68() * 125.0f + 204.0f, func_150ADA20() % 101U + 155, hit.points, &impact,
                          func_150ADA20() % 302U + 500, 0, 1, 0, 0, 0, arg1, 0);
        }
    }
    func_151D5404(&origin, 506.0f, 1013.0f, 0.0009871669f, 15, 20, arg1, arg2);
    func_151D5334(&origin, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
    func_151D5514(&origin, arg1, arg2);
    func_151D3FF4(&origin, arg1, arg2);
    func_150E83AC(hitGround ? &impact : &origin, func_150ADA20() % 62U + 120, arg1, arg2);
    func_151541B8(&origin, func_150ADA68() * 4.0f + 12.0f, 1.6409999f, func_150ADA20() % 56U + 200, 0.0f, arg1,
                  arg2);
    func_15136C3C(obj, 1, 1, 1, 1, 0, arg1, arg2);
    func_151D40D4(&origin, 0, obj, obj, 0, 0x16, 0x15, 0);
}

extern u16 D_800CC346;

void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
u8 func_150599C8(struct127 *arg0, u8 arg1, u16 arg2);
s32 func_150535F4(struct127 *arg0);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);

void func_150E0BE0(struct127 *arg0) {
    s32 temp_v0;
    u16 *temp_t1;

    arg0->unkB0 = 0xF;
    arg0->unk80 = 0xA;

    if (arg0->unk87 != 0) {
        temp_v0 = arg0->unk83;
        if (temp_v0 < 0x64) {
            temp_v0 = (arg0->unk83 = temp_v0 + (u32)D_800BE9A0);
            temp_v0 &= 0xFF;
        }
        if (temp_v0 < 5) {
        } else {
            temp_v0 = arg0->unk251;
            temp_t1 = &D_800CC346;
            if (temp_v0 != 3) {
                arg0->unk78 = *temp_t1;
                arg0->unk218 = 0;
                arg0->unk232 = 4;
                if (temp_v0 == 2) {
                    arg0->unk232 = 2;
                }
            }
        }
    } else {
        if (arg0->unk83 >= 0x3D) {
            D_800D1580 = 0xFF020133;
            D_800D154C = D_800CC2D0;
            func_1506E8D8();
            D_800D154C = arg0;
        }
        arg0->unk83 = 0;
    }

    if (arg0->unk232 == 4) {
        if (arg0->unk83 == 0) {
            arg0->unk78 = arg0->unk76;
        }
        func_150599C8(arg0, 3, arg0->unk78);
    }

    arg0->unk87 = 0;
    if (arg0->stunned == 0) {
        func_15056B08(arg0);
    } else {
        func_150585F0(arg0, 0.05000000074505806f);
    }

    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
    func_15059140(arg0);
    *(f32 *)((u8 *)arg0 + 0xBC) = arg0->x_position - arg0->old_x_position;
    arg0->unk148 = arg0->z_position - arg0->old_z_position;
    if (func_150535F4(arg0) == 0) {
        func_1502178C(arg0, 0, -1);
    }
}

