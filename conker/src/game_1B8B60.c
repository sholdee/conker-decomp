#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


extern void func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_1518CA80(void *, u8);
extern void func_151429E0(u8, u8 *, u8 *, u8 *);

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
typedef struct ParticleSpawn78 {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    u8 pad0C[0xA];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    u8 pad32[6];
} Struct1518B6B0b;

void func_1518B6B0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
    s32 i;
    ParticleSpawn78 sp;
    Struct1518B6B0b sp2;
    f32 scale;
    f32 speed;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 spread;
    u8 yaw;
    u8 pitch;

    sp.elasticity = 0.4f;
    sp.model = 4;
    sp.scale.x = 1.0f;
    sp.scale.y = 1.0f;
    sp.scale.z = 1.0f;
    sp.rotVel.z = 0.0f;
    sp.gravity = -1.3f;
    sp.flags = 0x29E8;
    sp.variant = 0;
    sp.unk5C = 0;
    sp.alpha = 0xFF;
    sp.updateFn = 1;
    sp.update2Fn = 0;
    sp.floorHitFn = 0;
    sp.floorType3Fn = 0;
    sp.wallHitFn = 0;
    sp.destroyFn = 0;
    sp.squashFn = 0;
    sp.matrixFn = 0;
    sp.unk6A = 2;
    sp.owner = 0;
    sp.ownerId = 0;
    sp.fadeStart = 0x4000;
    sp.fadeRate = 0;

    for (i = (func_150ADA20() % 8U) + 3; i > 0; i--) {
        scale = (func_150ADA68() * 0.7f) + 0.3f;
        yaw = func_150ADA20() & 0xFF;
        pitch = -0x10 - (func_150ADA20() % 32U);
        speed = (func_150ADA68() * 10.0f) + 5.0f;
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        cosYaw = func_151423D8(yaw);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosPitch = func_151423D8(pitch);
        spread = func_150ADA68() * 20.0f;
        sp.pos.x = (spread * sinYaw) + arg0;
        sp.pos.y = arg1;
        sp.pos.z = (spread * cosYaw) + arg2;
        sp.vel.x = speed * cosPitch * sinYaw;
        sp.vel.y = -speed * sinPitch;
        sp.vel.z = speed * cosPitch * cosYaw;
        sp.lifetime = (func_150ADA20() % 32U) + 0x14;
        sp.radius = scale;
        sp.sizeXZ = scale;
        sp.sizeY = scale;
        sp.rot.x = func_150ADA68() * 360.0f;
        sp.rot.y = func_150ADA68() * 360.0f;
        sp.rot.z = func_150ADA68() * 360.0f;
        sp.rotVel.x = 25.0f - (func_150ADA68() * 50.0f);
        sp.rotVel.y = 25.0f - (func_150ADA68() * 50.0f);
        func_15132A4C(&sp, 3, 0xFF, 0, arg3, arg4);
    }

    sp2.unk00 = arg0;
    sp2.unk04 = arg1;
    sp2.unk08 = arg2;
    sp2.unk16 = 0x1F4;
    sp2.unk18 = 0x1F4;
    sp2.unk1A = (func_150ADA20() % 16U) + 0x32;
    sp2.unk1C = (func_150ADA20() % 16U) + 0x32;
    sp2.unk1E = 0;
    sp2.unk20 = 0;
    sp2.unk22 = (func_150ADA20() % 201U) + 0x12C;
    sp2.unk24 = 0;
    sp2.unk26 = 0x258;
    func_151429E0(1, &sp2.unk28, &sp2.unk29, &sp2.unk2A);
    func_151429E0(1, &sp2.unk2B, &sp2.unk2C, &sp2.unk2D);
    sp2.unk2E = 0xFF;
    sp2.unk2F = 0xFF;
    sp2.unk30 = 0xA;
    sp2.unk31 = 0;
    func_1518CA80(&sp2, 1);
}
