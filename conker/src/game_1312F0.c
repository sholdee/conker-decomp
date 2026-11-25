#include <ultra64.h>
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

u32 func_150ADA20(void);
f32 func_150ADA68(void);
void func_1504715C(GroundHit *, struct127 *);
s32 func_15046C80(struct17 *, s32, f32, GroundHit *);
void func_151D3FF4(struct17 *, u8, s32);
void func_150E7FEC(f32, u8, s16 *, struct17 *, s32, s32, s32, s32, s32, s32, u8, s32);
void func_150E83AC(struct17 *, s16, u8, s32);
void func_151D5404(struct17 *, f32, f32, f32, s32, s32, u8, s32);
void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D40D4(struct17 *, s32, struct127 *, struct127 *, s32, s32, s32, s32);
void func_15055A2C(s32, f32, f32, f32, s32);
void func_151541B8(struct17 *, f32, f32, f32, f32, u8, s32);

/* Spawns an impact burst at pos: a ground splash when there is floor just below, effects, an optional variant-specific effect pair and particles. */
void func_15103E40(struct127 *owner, struct127 *obj, struct17 *pos, u8 variant, s32 arg4, u8 arg5, s32 arg6) {
    struct17 impact;
    u8 hitGround;
    struct17 top;
    GroundHit hit;
    s32 effectA;
    s32 effectB;

    if (owner == NULL) {
        return;
    }

    hitGround = FALSE;
    func_151D3FF4(pos, arg5, arg6);

    top.unk0 = pos->unk0;
    top.unk4 = pos->unk4 + 100.0f;
    top.unk8 = pos->unk8;
    func_1504715C(&hit, obj);

    if (func_15046C80(&top, 0, pos->unk4 - 100.0f, &hit) && (hit.flags & 1)) {
        impact.unk0 = top.unk0;
        hitGround = TRUE;
        impact.unk4 = hit.height;
        impact.unk8 = top.unk8;
        func_150E7FEC(func_150ADA68() * 125.0f + 204.0f, func_150ADA20() % 101U + 155, hit.points, &impact,
                      func_150ADA20() % 302U + 500, 0, 1, 0, 0, 0, arg5, 0);
    }

    func_150E83AC(hitGround ? &impact : pos, func_150ADA20() % 62U + 120, arg5, arg6);
    func_151D5404(pos, 506.0f, 1013.0f, 0.0009871669f, 15, 20, arg5, arg6);
    func_151D5334(pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg5, arg6);
    func_151D5514(pos, arg5, arg6);

    if (variant != 0) {
        if (variant == 2) {
            effectA = 0x42;
            effectB = 0x41;
        } else {
            effectA = 0x2B;
            effectB = 0x2A;
        }
        func_151D40D4(pos, 0, owner, obj, 0, effectA, effectB, arg4);
    }

    func_15055A2C(0, pos->unk0, pos->unk4, pos->unk8, 1);
    func_151541B8(pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f, func_150ADA20() % 56U + 200, 0.0f, arg5, arg6);
}
