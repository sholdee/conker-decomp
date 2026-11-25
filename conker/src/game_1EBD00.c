#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Effect spawn parameters; same layout as EffectParams in game_1844C0.c. */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ s8 updateFunc;
    /* 0x02 */ s8 updateFunc2;
    /* 0x03 */ s8 preDrawFunc;
    /* 0x04 */ s8 postDrawFunc;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 lifetime;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 otherModeH;
    /* 0x14 */ s32 geometrySet;
    /* 0x18 */ s32 geometryClear;
    /* 0x1C */ s32 renderMode;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 otherModeH2;
    /* 0x28 */ s32 otherModeH3;
    /* 0x2C */ s8 primMode;
    /* 0x2D */ s8 envMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 color0[4];
    /* 0x34 */ u8 color1[4];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ void *owner;
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f pos;
    /* 0x50 */ s32 zone;
    /* 0x54 */ s16 fadeStart;
    /* 0x56 */ s16 fadeRate;
} EffectParams; /* size 0x58 */

/* Extension block copied into the spawned effect at 0x120. */
typedef struct {
    /* 0x00 */ f32 angle;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 count;
} Extra151BE850;

extern void func_1510F800(s32);
extern void func_151436B4(f32 yaw, f32 pitch, f32 length, Vec3f *out);
void *func_151BEEE0(f32 arg0, Vec3f *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern void *func_15157010(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize,
                           u8 owner, s32 list);

/*
 * Spawns an effect (variant 0-2) at `pos`, facing `angle` degrees. In one mode (D_800D2E4C->unk19 bit 2,
 * or variant 2) it is a model effect 39 units ahead of and 40 below `pos`; otherwise func_151BEEE0 makes it.
 */
void *func_151BE850(Vec3f *pos, f32 angle, u8 count, u8 arg3, u8 variant) {
    void *fx;
    s32 zone;
    EffectParams params;
    Extra151BE850 extra;

    func_1510F800(0);
    zone = func_1510FD20((s32)pos->x, (s32)pos->z);
    if (variant >= 3) {
        return NULL;
    }
    if ((D_800D2E4C->unk19 & 4) || (variant == 2)) {
        u8 variantIds[3] = { 3, 1, 2 };
        Vec3f offset;
        s32 flag;

        extra.angle = angle;
        params.owner = NULL;
        params.ownerId = 0;
        params.unk41 = 0;
        params.pos = *pos;
        extra.count = count;
        {
            s32 pad; // matching: stack slot

            func_151436B4(angle * 0.01745329238f, 0.0f, 39.0f, &offset);
            extra.pos.x = pos->x + offset.x;
            extra.pos.y = (pos->y + offset.y) - 40.0f;
            extra.pos.z = pos->z + offset.z;
        }
        params.unk38 = variantIds[variant];
        // matching: the (u8) narrowing of a separate local steers IDO's temp-register order
        flag = arg3 ? 0x10 : 0;
        params.flags = (u8)flag | 0xE;
        params.updateFunc = 1;
        if (D_800BE9F0 == 7) {
            params.updateFunc2 = 6;
        } else {
            params.updateFunc2 = 2;
        }
        params.preDrawFunc = 0;
        params.postDrawFunc = -1;
        params.lifetime = 300;
        params.model = 0xA2;
        params.unkC = 8;
        params.otherModeH = 0;
        params.geometrySet = 0x620405;
        params.geometryClear = 0x40200;
        params.renderMode = 20;
        params.unk20 = 0x37;
        params.otherModeH2 = 0x80;
        params.otherModeH3 = 0x20;
        params.primMode = 0;
        params.envMode = 8;
        params.color0[0] = 0xFF;
        params.color0[1] = 0xFF;
        params.color0[2] = 0xFF;
        params.color0[3] = 0xFF;
        params.color1[0] = 0xFF;
        params.color1[1] = 0xFF;
        params.color1[2] = 0xFF;
        params.color1[3] = 0xFF;
        params.zone = zone;
        fx = func_15157010(&params, 0, 1.0f, 3, 0xFF, 0x18, 0xFF, 1);
        if (fx != NULL) {
            // copies the whole 0x18-byte extension (extraSize above); a 0x18-byte Extra151BE850 grows the frame
            memcpy((u8 *)fx + 0x120, &extra, 0x18);
        }
    } else {
        fx = func_151BEEE0(angle, pos, 3, 0xFF, variant, arg3, zone, 0xFF, 1);
    }
    return fx;
}
