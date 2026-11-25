#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn parameters of a particle (same layout as ParticleDesc in game_15D730.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode;
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ s32 flags;
    /* 0x5C */ void *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;
    /* 0x63 */ s8 deathFunc;
    /* 0x64 */ s8 drawFunc;
    /* 0x65 */ u8 handlerSet;
    /* 0x66 */ u8 views;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 maxSize;
} ParticleDesc; /* size 0x70 */

/* Per-particle data of a pulsing particle: two sizes oscillating around a base size
 * (same layout as ParticlePulse in game_15D730.c). */
typedef struct {
    u8 phase[2];
    s8 phaseStep[2];
    f32 baseSize;
    f32 amplitude[2];
} ParticlePulse;

/* Partial view of the particle func_15130280 creates (Particle in game_15D730.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0xA8 - 0x80];
    /* 0xA8 */ ParticlePulse pulse;
} Particle;

/* Data of the effect spawned by func_150C4120 (at +0x28 of the effect object). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;       /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x08 */ f32 unk8;
    /* 0x0C */ Particle *particle;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 flags;
    /* 0x18 */ s32 unk18;
} EffectData; /* size 0x1C */

/* Effect object (struct260 is a shorter view of its head). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ EffectData data;
} Effect;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spawns an effect that follows `owner` (lifetime `duration`, -1 for the default 300) together with
 * a pulsing particle. */
void func_150C4120(struct127 *owner, s16 duration, u8 arg2, s32 arg3) {
    EffectData data;
    Effect *effect;
    EffectData *state;
    ParticleDesc spawn;
    ParticlePulse pulse;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.unk8 = 0.0f;
    data.particle = NULL;
    data.unk10 = 0.0f;
    data.unk14 = -1;
    data.flags = 0;
    data.unk18 = 0;
    if (owner->id == 0x34) {
        data.unk14 = 0x54;
        data.flags = 1;
    }
    effect = (Effect *)func_15149130((duration == -1) ? 300 : duration, -1, 0x53, -1, (duration == -1) ? 0 : 1,
                                     0x40, (struct37 *)sizeof(EffectData), arg2, arg3); /* arg6 is the data size; its prototype is wrong */
    if (effect != NULL) {
        state = &effect->data;
        memcpy(state, &data, sizeof(EffectData));
        {
        const s32 textures[4] = { 0x60, 0x61, 0x62, 0x63 };

        pulse.phase[0] = func_150ADA20();
        pulse.phase[1] = func_150ADA20();
        pulse.baseSize = spawn.width = spawn.height = 700.0f;
        pulse.phaseStep[0] = (s8)(func_150ADA20() % 6U) + 5; // matching: the (s8) steers t-register allocation
        pulse.phaseStep[1] = func_150ADA20() % 6U + 5;
        pulse.amplitude[0] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
        pulse.amplitude[1] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
        spawn.texture = *(textures + (func_150ADA20() & 3)); // matching: textures[i] reserves an extra temp
        spawn.renderMode = 0x2203;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r = 0;
        spawn.g = 0;
        spawn.b = 0;
        spawn.a = 0xFF;
        spawn.r2 = 0;
        spawn.g2 = 0;
        spawn.b2 = 0;
        spawn.alpha = 0xFF;
        spawn.brightness = 0xFF;
        spawn.pos = *(Vec3f *)&D_800A5480;
        spawn.offset = *(Vec3f *)&D_800A5480;
        spawn.vel = *(Vec3f *)&D_800A5480;
        spawn.scaleStart = 1;
        spawn.gravity = 0.0f;
        spawn.scaleRate = 1.0f;
        spawn.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000;
        spawn.timer = 300;
        spawn.maxSize = 10000.0f;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.primColorMode = 6;
        spawn.envColorMode = 8;
        spawn.drawFunc = -1;
        spawn.updateFunc = 0x26;
        spawn.deathFunc = -1;
        spawn.handlerSet = 0;
        spawn.viewMask = NULL;
        spawn.views = 0xFF;
        spawn.minSize = 0;
        state->particle = func_15130280(&spawn, 1, 0, sizeof(ParticlePulse), arg2, arg3);
        if (state->particle != NULL) {
            memcpy(&state->particle->pulse, &pulse, sizeof(ParticlePulse));
        }
        }
    }
}

/* Ground probe result filled by func_1504715C (same layout as GroundHit in game_105760.c). */
typedef struct {
    f32 height;
    s16 points[9];
    s32 unk18;
    u8 flags;
    u8 unk1D;
    s32 unk20;
} GroundHit; /* size 0x24 */

extern void func_1504715C(GroundHit *, struct127 *);
extern s32 func_1514654C(struct127 *, struct126 *, s32, Vec3f **, Vec3f **, s32);
extern void func_15143134(Vec3f *, Vec3f *, Mtx *);
extern void func_15143794(s16 yaw, s16 pitch, f32 length, Vec3f *out);
extern s32 func_1515C0F8(struct127 *, Vec3f **);
extern void func_151C36D8(Vec3f *, s32, s32);
extern void func_151D9014(Vec3f *, Vec3f *, s32, f32, s32, s32, f32, s32, f32, f32, s32, void *, s32, s32, s32, s32);
extern Vec3f D_800A0350[];
extern u8 D_800A0374[];
extern Vec3f D_800A0378[];
extern u8 D_800A03A8[];

/* Per-frame update of the func_150C4120 effect: keeps its particle at a point on the owner and emits
 * smaller particles from there. Frees the effect once the owner is gone. */
void func_150C44A4(Effect *effect) {
    struct127 *owner;
    EffectData *data;
    Vec3f pos;
    Vec3f offset;
    f32 progress;
    s32 i;
    f32 t;
    GroundHit ground;
    Vec3f *offsetPtr;
    Vec3f *posPtr;
    struct126 *bone;
    s32 emit;
    Vec3f *wind;
    Vec3f vel;
    s32 rand1;
    s32 rand0;
    f32 randF0;
    f32 randF1;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->interaction_state == 0) || (owner->unique_id != data->ownerId)) {
        effect->state = -1;
        return;
    }
    if ((owner->unk1D4 == NULL) || (data->flags & 2)) {
        if (data->particle != NULL) {
            data->particle->desc.drawFunc = 3;
        }
        return;
    }
    func_1504715C(&ground, owner);
    if (data->unk18 == 0) {
        data->unk18 = func_10010154(150, owner, 0x36B0, 400, 0x4268);
    }
    if (data->flags & 1) {
        bone = func_1503195C(owner, data->unk14, 0);
        if (bone == NULL) {
            return;
        }
        /* struct127 has no fields at 0x2D8/0x2DC yet */
        if (owner->unk2EC != 0) {
            progress = *(f32 *)((u8 *)owner + 0x2D8) * *(f32 *)((u8 *)owner + 0x2DC);
        } else {
            progress = 0.0f;
        }
        i = progress * 4.0f;
        if (i >= 4) {
            i = 3;
        }
        t = (progress - i * 0.25f) * 4.0f;
        offset.x = D_800A0378[i].x * t;
        offset.y = D_800A0378[i].y * t;
        offset.z = D_800A0378[i].z * t;
        offsetPtr = &offset;
        posPtr = &pos;
        if (func_1514654C(owner, bone, D_800A03A8[i], &offsetPtr, &posPtr, 1) == 0) {
            pos.x = owner->x_position;
            pos.y = owner->y_position + 800.0f;
            pos.z = owner->z_position;
        }
    } else {
        if (owner->unk2E8 != 0) {
            progress = (f32)owner->unk2E4 / (f32)owner->unk2E8;
        } else {
            progress = 1.0f;
        }
        i = progress * 3.0f;
        if (i >= 3) {
            i = 2;
        }
        t = (progress - i * 0.33333334f) * 3.0f;
        offset.x = D_800A0350[i].x * t;
        offset.y = D_800A0350[i].y * t;
        offset.z = D_800A0350[i].z * t;
        func_15143134(&offset, &pos, (Mtx *)owner->unk1D4 + D_800A0374[i]);
    }
    if (data->particle != NULL) {
        data->particle->desc.drawFunc = -1;
        data->particle->desc.pos = pos;
    }
    emit = TRUE;
    if ((D_800BE9F0 == 2) && (func_150A29C8(owner - D_800CC2D0, 0x4065) == 0)) {
        emit = FALSE;
    }
    if (emit) {
        data->unk8 += (0.1290000081f + func_150ADA68() * 0.1170000061f) * D_800BE9A4;
        while (data->unk8 > 1.0f) {
            func_151C36D8(&pos, effect->unkC, effect->unk1);
            data->unk8 -= 1.0f;
        }
    }
    data->unk10 += (0.1300000101f + func_150ADA68() * 0.2f) * D_800BE9A4;
    if (data->unk10 > 1.0f) {
        if (func_1515C0F8(owner, &wind) == 0) {
            wind = (Vec3f *)&D_800A5480;
        }
        do {
            rand1 = func_150ADA20();
            rand0 = func_150ADA20();
            func_15143794(rand1 & 0xFF, (rand0 % 25U) - 0x40, func_150ADA68() * 2.597000122f + 4.870000362f, &vel);
            vel.x -= wind->x * 0.6f;
            vel.z -= wind->z * 0.6f;
            randF0 = func_150ADA68();
            rand1 = func_150ADA20();
            rand0 = func_150ADA20();
            randF1 = func_150ADA68();
            func_151D9014(&pos, &vel, 0xC, randF0 * 0.2580000162f + -0.6080000401f, (rand1 % 41U) + 60, (rand0 % 56U) + 200,
                          randF1 * 160.0f + 121.0f, func_150ADA68() < 0.7000000477f, 1.0f, 1.0f, 0, &ground, 1, 1,
                          effect->unkC, effect->unk1);
            data->unk10 -= 1.0f;
        } while (data->unk10 > 1.0f);
    }
}

/* Frees the effect's particle, if it has one. */
void func_150C4AD8(Effect *effect) {
    // matching: the s32 read keeps IDO from merging the test and the argument into one load
    if (*(s32 *)&effect->data.particle != 0) {
        func_1516972C((struct102 *)effect->data.particle);
    }
}

/* Frees the effect's particle, then calls func_1514933C on the effect. */
void func_150C4B08(Effect *effect) {
    func_150C4AD8(effect);
    func_1514933C((struct260 *)effect);
}

/* Frees the effect's particle, then calls func_15149368 on the effect. */
void func_150C4B34(Effect *effect) {
    func_150C4AD8(effect);
    func_15149368((struct260 *)effect);
}

/* Event handler: events 0x55/0x56 clear/set flag 2 (pausing the effect), others go to func_15149514. */
void func_150C4B60(Effect *effect, s32 arg1, u8 event) {
    EffectData *data = &effect->data;

    if (event == 0x55) {
        data->flags &= ~2;
    } else if (event == 0x56) {
        data->flags |= 2;
    } else {
        func_15149514(arg1, event, (s32)data, (s32)&data->ownerId, (s32)effect);
    }
}
