#include <ultra64.h>
#include "functions.h"
#include "variables.h"



typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ s8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ s8 unk68;
    /* 0x69 */ u8 unk69;
    /* 0x6A */ s8 unk6A;
    /* 0x6C */ void *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} Spawn150DECC0;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ struct17 pos;
    /* 0x10 */ s16 unk10;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 velBase;
    /* 0x24 */ f32 velRange;
    /* 0x28 */ f32 velYBase;
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ struct17 vel;
    /* 0x3C */ f32 scaleBase;
    /* 0x40 */ f32 scaleRange;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} Params150DECC0;

/* Effect object header from func_15149130; effect-specific data starts at 0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ struct17 pos;
} PointEffect;

extern struct17 D_800A0D60[15];
extern f32 D_800A0E14[15];
extern f32 D_800A0E50[15];
extern f32 D_800A0E8C[15];
extern f32 D_80088960[3];
extern f32 D_8008896C[3];

void *memcpy(void *, const void *, u32);
s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D3FF4(struct17 *, u8, s32);
void func_151541B8(struct17 *, f32, s32, f32, f32, u8, s32);
void func_151A6F00(void *, s32, u8, s32);

void func_150DECC0(struct17 *arg0, u8 arg1, s32 arg2) {
    s32 yaw;
    s32 i;
    Spawn150DECC0 spawn;
    u8 idx;
    Params150DECC0 params;

    yaw = (s32)(D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f);
    {
        extern f32 D_800A0EC8[15];

        spawn.unk00 = 15.0f;
        spawn.unk0C = 1.0f;
        spawn.unk08 = 1.0f;
        spawn.unk04 = 0.200000003f;
        spawn.unk10 = *(struct17 *)&D_800A5480;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk44 = 0.0f;
        spawn.unk50 = 0x29E9;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 0xD;
        spawn.unk62 = 0;
        spawn.unk63 = 0xA;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 2;
        spawn.unk6C = NULL;
        spawn.unk70 = 0;
        spawn.unk72 = 0x23;
        spawn.unk74 = 7;
        spawn.unk78 = 0;
        spawn.unk34.unk0 = 0.0f;
        spawn.unk34.unk8 = 0.0f;
        for (i = 0; i < 15; i++) {
            idx = D_800A0EC8[i];
            spawn.unk28.unk0 = D_800A0D60[i].unk0 + arg0->unk0;
            spawn.unk28.unk4 = D_800A0D60[i].unk4 + arg0->unk4;
            spawn.unk28.unk8 = D_800A0D60[i].unk8 + arg0->unk8;
            spawn.unk34.unk4 = D_80088960[idx] + func_150ADA68() * D_8008896C[idx];
            spawn.unk40 = D_800A0E8C[i] * D_800A0E50[i] * 0.01900000125f;
            spawn.unk48 = D_800A0E8C[i] * D_800A0E14[i] * 0.01900000125f;
            spawn.unk4C = func_150ADA68() * 0.2080000043f + -1.032000065f;
            spawn.unk54 = func_150ADA20() % 111U + 130;
            if (func_150ADA20() & 1) {
                spawn.unk56 = 0xC4;
            } else {
                spawn.unk56 = 0xC5;
            }
            func_1513264C(&spawn, 3, 0xFF, 0, 0, *(u8 *)&arg1, arg2);
        }
    }
    func_15165F80(-1, (s32)arg0->unk0, (s32)arg0->unk4, (s32)arg0->unk8, 0x1E, 0x32, 0, *(u8 *)&arg1, arg2);
    func_151D5404(arg0, 506.0f, 1013.0f, 0.0009871668881f, 0xF, 0x14, *(u8 *)&arg1, arg2);
    func_151D5334(arg0, 506.0f, 1013.0f, 0.0009871668881f, 5, *(u8 *)&arg1, arg2);
    func_151D5514(arg0, *(u8 *)&arg1, arg2);
    func_151D3FF4(arg0, *(u8 *)&arg1, arg2);
    func_151541B8(arg0, func_150ADA68() * 4.0f + 12.0f, 0x3FD20C49,
                  (f32)(func_150ADA20() % 56U + 200), 0.0f, *(u8 *)&arg1, arg2);
    params.count = (func_150ADA20() & 3) + 3;
    params.unk10 = 0xBE;
    params.yawBase = yaw - 0x40;
    params.yawRange = 0x80;
    params.pitchBase = -0x3F;
    params.pitchRange = 0x5A;
    params.speed = 250.0f;
    params.pos = *arg0;
    params.velBase = 5.0f;
    params.velYBase = 15.0f;
    params.velYRange = 15.0f;
    params.vel.unk0 = 0.0f;
    params.vel.unk4 = 0.0f;
    params.unk54 = 1;
    params.unk55 = 1;
    params.unk56 = 0x64;
    params.unk58 = 0x32;
    params.velRange = 10.0f;
    params.unk50 = 10.0f;
    params.vel.unk8 = 0.0f;
    params.scaleBase = 0.25f;
    params.scaleRange = 0.5f;
    params.unk44 = -0.6999999881f;
    params.unk48 = -0.6200000048f;
    params.unk4C = -5.0f;
    func_151A6F00(&params, 0, *(u8 *)&arg1, arg2);
    {
        struct260 *ret;
        struct17 pos;

        pos = *arg0;
        ret = func_15149130(0xF, 8, -1, -1, 1, 0, (struct37 *)0xC, *(u8 *)&arg1, arg2);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x28, &pos, sizeof(pos));
        }
    }
}

/* Result of a ground probe (func_1504697C). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x20];
} GroundHit; /* size 0x24 */

s32 func_1504697C(struct17 *, s32, f32, GroundHit *);
void func_150E83AC(struct17 *, s16, u8, s32);
void func_151A9834(struct17 *, f32, f32, GroundHit *, s32, u8, s32, u8, s32);

/*
 * Effect update: drops the effect's position onto the ground below it (if any), spawns a
 * point effect (func_150E83AC) there and scatters 3-5 func_151A9834 spawns around it.
 */
void func_150DF334(PointEffect *effect) {
    struct17 *pos;
    GroundHit hit;
    u8 onGround;
    struct17 groundPos;
    struct17 probe;
    struct17 center;

    pos = &effect->pos;
    onGround = 0;
    probe.unk0 = effect->pos.unk0;
    probe.unk4 = effect->pos.unk4 + 500.0f;
    probe.unk8 = effect->pos.unk8;
    if (func_1504697C(&probe, 0, probe.unk4 - 1000.0f, &hit) != 0) {
        groundPos.unk0 = pos->unk0;
        groundPos.unk4 = hit.height;
        groundPos.unk8 = pos->unk8;
        onGround = 1;
    }
    func_150E83AC(onGround ? &groundPos : pos, (s16)(func_150ADA20() % 62U + 120), effect->owner, effect->kind);
    pos = onGround ? &groundPos : &effect->pos;
    center = *pos;
    center.unk4 += 500.0f;
    func_151A9834(&center, center.unk4 - 1000.0f, 250.0f, &hit, func_150ADA20() % 3U + 3, 1, 0, effect->owner,
                  effect->kind);
}

/* Particle emitter description passed to func_15149550. */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 maxA;
    /* 0x10 */ f32 maxB;
    /* 0x14 */ f32 minA;
    /* 0x18 */ f32 minB;
    /* 0x1C */ f32 altB;
    /* 0x20 */ s16 maxC;
    /* 0x22 */ s16 minC;
    /* 0x24 */ s16 life;
    /* 0x26 */ u8 color[4];
    /* 0x2A */ u16 flags;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ s32 extraSize;
    /* 0x34 */ u8 effect;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 scale;
    /* 0x3C */ f32 maxHeight;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
} ParticleEmitter;

void *func_15149550(ParticleEmitter *emitter, u8 arg1, u8 arg2, u8 arg3, u8 owner, s32 kind);
void func_15143874(s16 angle, f32 dist, f32 *x, f32 *z);

/* Spawns a particle at pos, then 1-3 more scattered within 30 units of it. */
void func_150DF4B8(struct17 *pos, s32 arg1, s32 arg2, u8 owner, s32 kind) {
    ParticleEmitter emitter;
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
        func_15143874(func_150ADA20() & 0xFF, 30.0f, &emitter.pos.unk0, &emitter.pos.unk8);
        emitter.pos.unk0 += pos->unk0;
        emitter.pos.unk8 += pos->unk8;
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
