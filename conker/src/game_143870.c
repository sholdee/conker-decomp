#include <ultra64.h>
// functions.h declares these two as returning void; hide those prototypes and redeclare them below.
#define func_15048A40 func_15048A40_void_unused
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_15048A40
#undef func_10010F88
#include "variables.h"

s32 func_10010F88(s32 sound, u16 volume, s16 pitch, u8 arg3, s32 arg4, s16 x, s16 y, s16 z, s16 arg8, s16 arg9);
extern f32 func_15048A40(u8 angle);
s32 func_150490A8(struct17 *delta);

/* The object fields used by the functions in this file. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x2];
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 pad1C[0x20];
    /* 0x3C */ union {
        s32 word;
        struct {
            s16 hi;
            u16 lo;
        } half;
    } params;
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 flags;
    /* 0x50 */ u8 pad50[0xC];
    /* 0x5C */ s16 dy;
    /* 0x5E */ u8 pad5E[0x2];
    /* 0x60 */ f32 dx;
    /* 0x64 */ f32 speed;
    /* 0x68 */ u8 pad68[0xC];
    /* 0x74 */ u16 sound;
    /* 0x76 */ u8 pad76[0x6];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
} Game143870Object;

void func_1511515C(Game143870Object *obj);
s32 func_15116888(s32 x, s32 z, s32 *unused, struct127 *other);

/* Bobs the object: params bytes are (height scale, sway scale, height rate, sway rate) from low to high. */
void func_151163C0(Game143870Object *obj) {
    volatile f32 heightScale;
    u32 heightRate;
    u32 swayRate;
    volatile f32 swayScale;
    s16 previousY;
    f32 previousSway;
    f32 sway;
    u32 swayScaleByte;
    s32 params;

    params = obj->params.word;
    swayScaleByte = (params >> 8) & 0xFF;
    heightRate = (params >> 16) & 0xFF;
    heightScale = (f32)(params & 0xFF);
    swayRate = (params >> 24) & 0xFF;
    swayScale = (f32)swayScaleByte * 1.40625f;
    previousY = obj->y;
    previousSway = obj->unk0;
    obj->unk18 = cosf((f32)obj->unk7C * 9.58738e-05f) * heightScale;
    sway = cosf((f32)obj->unk80 * 9.58738e-05f) * swayScale;
    obj->unk0 = sway;
    obj->unk7C += heightRate * D_800BE9E4;
    obj->unk80 += swayRate * D_800BE9E4;
    obj->dy = obj->y - previousY;
    obj->dx = sway - previousSway;
}
/*
 * Updates the object's push value (high half of params) from the pull of D_800CC2D0[0], letting it decay
 * back to zero, then keeps a looping sound whose volume follows the object's speed.
 */
void func_1511650C(Game143870Object *obj, s32 arg1, s32 sound, f32 pitchScale) {
    s32 push;
    s32 value;
    s32 timer;
    s32 decay;
    f32 speed;
    f32 amount;
    u32 volume;
    u16 handle;

    if (!(obj->flags & 4)) {
        push = 0;
    } else {
        push = func_15116888(obj->x, obj->z, &obj->unk7C, D_800CC2D0) >> 3;
    }
    if ((arg1 & 1) && (push < 0)) {
        push = 0;
    }
    value = obj->params.half.hi;
    value += push;
    if (push != 0) {
        obj->unk80 = 0;
    }
    timer = obj->unk80;
    decay = value * -0.0032;
    if (timer <= 0) {
        value += decay;
        if (value > 0) {
            value -= 1;
            if (value <= 0) {
                if (push == 0) {
                    if (timer == 0) {
                        obj->unk80 = 20;
                    } else {
                        obj->unk80 = 0;
                        value = 0;
                    }
                } else {
                    value = 0;
                }
            }
        } else if (value < 0) {
            value += 1;
            if (value >= 0) {
                if (push == 0) {
                    if (timer == 0) {
                        obj->unk80 = 20;
                    } else {
                        obj->unk80 = 0;
                        value = 0;
                    }
                } else {
                    value = 0;
                }
            }
        }
    } else if (timer > 0) {
        if (value > 0) {
            value += 1;
            obj->unk80 = timer - D_800BE9E4;
            if (obj->unk80 <= 0) {
                obj->unk80 = -1;
            }
        } else {
            value -= 1;
            obj->unk80 = timer - D_800BE9E4;
            if (obj->unk80 <= 0) {
                obj->unk80 = -1;
            }
        }
    }
    value <<= 16;
    obj->params.word = (obj->params.word & 0xFFFF) | value;
    func_1511515C(obj);
    if (sound != 0) {
        f32 one = 1.0f;
        speed = obj->speed;
        amount = fabsf(speed) * 0.1f;
        if (amount > 1.0f) {
            amount = 1.0f;
        }
        volume = (12767.0f * amount) + 20000.0f;
        handle = obj->sound;
        amount = (amount * pitchScale) - 500.0f;
        if (handle == 0) {
            if (0.01f < speed) {
                obj->sound = func_10010F88(sound, volume, amount, 0, -1, obj->x, obj->y, obj->z, 1000, 6000);
            }
        } else {
            if (0.01f < speed) {
                func_1000F91C(handle, volume, amount, 0, -1, obj->x, obj->y, obj->z, 200, 2000);
                return;
            }
            func_100111C8(handle);
            obj->sound = 0;
        }
    }
}

/* Returns other's horizontal speed projected onto the direction from (x, z) towards it. */
s32 func_15116888(s32 x, s32 z, s32 *unused, struct127 *other) {
    s32 otherX;
    s32 otherZ;
    s32 bearing;
    s32 angle;
    f32 result;
    s32 pad; // matching: the original frame has 4 more bytes of locals
    struct17 delta;

    otherX = (s32)other->x_position;
    otherZ = (s32)other->z_position;
    delta.unk0 = otherX - x;
    delta.unk8 = otherZ - z;

    bearing = func_150490A8(&delta);
    angle = other->unk76 >> 8;
    angle = 0x40 - angle;
    angle = angle - bearing;
    result = func_15048A40(angle);
    result = other->xz_velocity * result;
    return result;
}

void func_15116924(s32 arg0) {
}
