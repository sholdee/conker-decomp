#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Effect object header from func_15149130; effect-specific data starts at 0x28. */
struct PointEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Vec3f pos;
};

extern Vec3f D_800A0D60[15];
extern f32 D_800A0E14[15];
extern f32 D_800A0E50[15];
extern f32 D_800A0E8C[15];
extern f32 D_80088960[3];
extern f32 D_8008896C[3];

void *memcpy(void *, const void *, u32);

/*
 * Explosion at pos: 15 spawned effects (offsets from D_800A0D60), followed by func_15165F80,
 * func_151D5404/5334/5514/3FF4 and func_151541B8 effects, a burst of 3-6 debris aimed
 * around the camera yaw, and a short-lived PointEffect at pos.
 */
void func_150DECC0(Vec3f *pos, u8 owner, s32 kind) {
    s32 yaw;
    s32 i;
    ParticleSpawn spawn;
    u8 idx;
    DebrisEmitter debris;

    // camera yaw in degrees -> 0..255 binary angle
    yaw = (s32)(camera_global_Cameras[D_80082FA4].TrueCameraAngle * 0.7111111283f);
    {
        extern f32 D_800A0EC8[15]; // matching: a block with a decl is a -g3 scheduling barrier; an extern takes no stack slot

        spawn.radius = 15.0f;
        spawn.sizeY = 1.0f;
        spawn.sizeXZ = 1.0f;
        spawn.elasticity = 0.200000003f;
        spawn.rot = D_800A5480;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.rotVel.y = 0.0f;
        spawn.flags = 0x29E9;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 255;
        spawn.updateFn = 13;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 10;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 35;
        spawn.fadeRate = 7;
        spawn.sound = 0;
        spawn.vel.x = 0.0f;
        spawn.vel.z = 0.0f;
        for (i = 0; i < 15; i++) {
            idx = D_800A0EC8[i];
            spawn.pos.x = D_800A0D60[i].x + pos->x;
            spawn.pos.y = D_800A0D60[i].y + pos->y;
            spawn.pos.z = D_800A0D60[i].z + pos->z;
            spawn.vel.y = D_80088960[idx] + func_150ADA68() * D_8008896C[idx];
            spawn.rotVel.x = D_800A0E8C[i] * D_800A0E50[i] * 0.01900000125f;
            spawn.rotVel.z = D_800A0E8C[i] * D_800A0E14[i] * 0.01900000125f;
            spawn.gravity = func_150ADA68() * 0.2080000043f + -1.032000065f;
            spawn.lifetime = func_150ADA20() % 111U + 130;
            if (func_150ADA20() & 1) {
                spawn.model = 0xC4;
            } else {
                spawn.model = 0xC5;
            }
            // matching: *(u8 *)&owner re-reads owner from its stack slot at every use (a K&R u8 gets CSE'd)
            func_1513264C(&spawn, 3, 0xFF, 0, 0, *(u8 *)&owner, kind);
        }
    }
    func_15165F80(-1, (s32)pos->x, (s32)pos->y, (s32)pos->z, 30, 50, 0, *(u8 *)&owner, kind);
    func_151D5404(pos, 506.0f, 1013.0f, 0.0009871668881f, 15, 20, *(u8 *)&owner, kind);
    func_151D5334(pos, 506.0f, 1013.0f, 0.0009871668881f, 5, *(u8 *)&owner, kind);
    func_151D5514(pos, *(u8 *)&owner, kind);
    func_151D3FF4(pos, *(u8 *)&owner, kind);
    func_151541B8(pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f,
                  (f32)(func_150ADA20() % 56U + 200), 0.0f, *(u8 *)&owner, kind);
    debris.count = (func_150ADA20() & 3) + 3;
    debris.lifetime = 190;
    debris.yawBase = yaw - 0x40;
    debris.yawRange = 0x80;
    debris.pitchBase = -0x3F;
    debris.pitchRange = 0x5A;
    debris.radius = 250.0f;
    debris.pos = *pos;
    debris.speedBase = 5.0f;
    debris.velYBase = 15.0f;
    debris.velYRange = 15.0f;
    debris.dir.x = 0.0f;
    debris.dir.y = 0.0f;
    debris.landPuffs = 1;
    debris.landPuffFlag = 1;
    debris.landPuffLifeBase = 100;
    debris.landPuffLifeRange = 50;
    debris.speedRange = 10.0f;
    debris.spinRange = 10.0f;
    debris.dir.z = 0.0f;
    debris.sizeBase = 0.25f;
    debris.sizeRange = 0.5f;
    debris.gravityBase = -0.6999999881f;
    debris.gravityRange = -0.6200000048f;
    debris.spinBase = -5.0f;
    func_151A6F00(&debris, 0, *(u8 *)&owner, kind);
    {
        TimedEffect *effect;
        Vec3f effectPos;

        effectPos = *pos;
        effect = func_15149130(15, 8, -1, -1, 1, 0, sizeof(effectPos), *(u8 *)&owner, kind);
        if (effect != NULL) {
            memcpy(&((PointEffect *)effect)->pos, &effectPos, sizeof(effectPos));
        }
    }
}

/*
 * Effect update: drops the effect's position onto the ground below it (if any), spawns a
 * point effect (func_150E83AC) there and scatters 3-5 func_151A9834 spawns around it.
 */
void func_150DF334(PointEffect *effect) {
    Vec3f *pos;
    CollisionHit hit;
    u8 onGround;
    Vec3f groundPos;
    Vec3f probe;
    Vec3f center;

    pos = &effect->pos;
    onGround = 0;
    probe.x = effect->pos.x;
    probe.y = effect->pos.y + 500.0f;
    probe.z = effect->pos.z;
    if (func_1504697C(&probe, 0, probe.y - 1000.0f, &hit) != 0) {
        groundPos.x = pos->x;
        groundPos.y = hit.height;
        groundPos.z = pos->z;
        onGround = 1;
    }
    func_150E83AC(onGround ? &groundPos : pos, (s16)(func_150ADA20() % 62U + 120), effect->base.header.owner, effect->base.header.kind);
    pos = onGround ? &groundPos : &effect->pos;
    center = *pos;
    center.y += 500.0f;
    func_151A9834(&center, center.y - 1000.0f, 250.0f, &hit, func_150ADA20() % 3U + 3, 1, 0, effect->base.header.owner,
                  effect->base.header.kind);
}

/* Spawns a particle at pos, then 1-3 more scattered within 30 units of it. */
void func_150DF4B8(Vec3f *pos, s32 arg1, s32 arg2, u8 owner, s32 kind) {
    ParticleEmitterDesc emitter;
    f32 scaleA;
    f32 scaleB;
    s32 count;

    emitter.pos = *pos;
    scaleA = func_150ADA68() * 0.5f + 1.0f;
    scaleB = func_150ADA68() * 0.5f + 1.0f;
    emitter.maxA = 22.6f * scaleA;
    emitter.minA = 13.786f * scaleA;
    emitter.maxC = 0x6231; emitter.minC = 0x1A4D; // matching: same line, -g3 line numbers order the two sh
    emitter.maxB = 57.969f * scaleB;
    emitter.minB = 42.714f * scaleB;
    emitter.altB = 118.763f * scaleB;
    emitter.life = func_150ADA20() % 151U + 150;
    emitter.color[0] = 0;
    emitter.color[1] = 0;
    emitter.color[2] = 0;
    emitter.color[3] = 255;
    emitter.flags = 1;
    emitter.unk2C = func_150ADA20() % 156U + 100;
    emitter.unk2D = 255;
    emitter.unk2E = 0;
    emitter.unk2F = 1;
    emitter.extraSize = 0;
    emitter.effect = 15;
    emitter.scale = 0.0f;
    emitter.maxHeight = 10000.0f;
    emitter.shrinkRate = 0.0450000018f;
    emitter.growRate = 0.699999988f;
    func_15149550(&emitter, 10, 0, 0, owner, kind);
    for (count = func_150ADA20() % 3U + 1; count > 0; count--) {
        func_15143874(func_150ADA20() & 0xFF, 30.0f, &emitter.pos.x, &emitter.pos.z);
        emitter.pos.x += pos->x;
        emitter.pos.z += pos->z;
        scaleA = func_150ADA68() * 0.5f + 0.75f;
        scaleB = func_150ADA68() * 0.5f + 0.75f;
        emitter.maxA = 22.6f * scaleA;
        emitter.minA = 13.786f * scaleA;
        emitter.maxB = 57.969f * scaleB;
        emitter.minB = 42.714f * scaleB;
        emitter.altB = 118.763f * scaleB;
        emitter.life = func_150ADA20() % 151U + 150;
        emitter.unk2C = func_150ADA20() % 156U + 100;
        emitter.unk2F = 0;
        func_15149550(&emitter, 10, 0, 0, owner, kind);
    }
}
