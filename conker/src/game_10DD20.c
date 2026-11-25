#include <ultra64.h>
#include "functions.h"
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

void func_1504715C(GroundHit *, Actor *);
s32 func_15046C80(Vec3f *, s32, f32, GroundHit *);
void func_151D3FF4(Vec3f *, u8, s32);
void func_150E7FEC(f32, u8, s16 *, Vec3f *, s32, s32, s32, s32, s32, s32, u8, s32);
void func_150E83AC(Vec3f *, s16, u8, s32);
void func_151D5334(Vec3f *, f32, f32, f32, s32, u8, s32);
void func_151D5514(Vec3f *, u8, s32);
void func_151D40D4(Vec3f *, s32, Actor *, Actor *, s32, s32, s32, s32);
void func_151541B8(Vec3f *, f32, f32, f32, f32, u8, s32);
void func_15136C3C(Actor *, s32, s32, s32, s32, s32, u8, s32);

/* Spawns a burst of effects at obj: two sounds, a ground splash when there is floor below, and particles. */
void func_150E0870(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f origin;
    Vec3f impact;
    u8 hitGround;

    if (obj == NULL) {
        return;
    }
    if (D_800C35EA != 1) {
        func_10010630(0x2BB, obj, 0x7FFF, 1000, 2000);
        func_10010630(0x2BA, obj, 0x7FFF, 1000, 2000);
    }
    origin.x = obj->x_position;
    origin.y = obj->y_position + 20.0f;
    origin.z = obj->z_position;
    hitGround = FALSE;
    {
        Vec3f top;
        GroundHit hit;

        top.x = origin.x;
        top.y = origin.y + 100.0f;
        top.z = origin.z;
        func_1504715C(&hit, obj);
        if (func_15046C80(&top, 0, origin.y - 200.0f, &hit) && (hit.flags & 1)) {
            impact.x = top.x;
            hitGround = TRUE;
            impact.y = hit.height;
            impact.z = top.z;
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

void func_15056B08(Actor *arg0);
void func_150585F0(Actor *arg0, f32 arg1);
u8 func_150599C8(Actor *arg0, u8 arg1, u16 arg2);
s32 func_150535F4(Actor *arg0);
void func_1502178C(Actor *arg0, s32 arg1, s32 arg2);

/* Per-frame update for obj: steps its unk83 counter while unk87 is set, moves it, then stores its x/z displacement since last frame. */
void func_150E0BE0(Actor *obj) {
    s32 counter;

    obj->unkB0 = 15;
    obj->unk80 = 10;

    if (obj->unk87 != 0) {
        counter = obj->unk83;
        if (counter < 100) {
            counter = obj->unk83 = counter + (u32)D_800BE9A0;
            counter &= 0xFF; // matching: u32 add + explicit wrap orders the sb before the andi
        }
        if (counter >= 5) {
            counter = obj->unk251; // matching: reusing counter keeps both in v0
            if (counter != 3) {
                obj->unk78 = D_800CC346;
                obj->unk218 = NULL;
                obj->unk232 = 4;
                if (counter == 2) {
                    obj->unk232 = 2;
                }
            }
        }
    } else {
        if (obj->unk83 > 60) {
            D_800D1580 = 0xFF020133;
            D_800D154C = D_800CC2D0;
            func_1506E8D8();
            D_800D154C = obj;
        }
        obj->unk83 = 0;
    }

    if (obj->unk232 == 4) {
        if (obj->unk83 == 0) {
            obj->unk78 = obj->unk76;
        }
        func_150599C8(obj, 3, obj->unk78);
    }

    obj->unk87 = 0;
    if (!obj->stunned) {
        func_15056B08(obj);
    } else {
        func_150585F0(obj, 0.05f);
    }

    /* facing angle (0x10000 units per turn) rotated a quarter turn, in degrees */
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * (360.0f / 0x10000);
    func_15059140(obj);
    obj->unkBC = obj->x_position - obj->old_x_position; // 0xBC is an f32 that structs.h still calls padBC
    obj->unk148 = obj->z_position - obj->old_z_position;
    if (func_150535F4(obj) == 0) {
        func_1502178C(obj, 0, -1);
    }
}
