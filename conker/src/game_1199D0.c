#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_151432BC(void *, f32 *, f32 *, f32 *, f32 *);
void *memcpy(void *, const void *, u32);
u8 func_151464B8(s16 *);

/* Data of the emitter effect (func_150EC520): spawns points on a shape at a random rate. */
typedef struct {
    /* 0x00 */ void *shape;
    /* 0x04 */ f32 rate;
    /* 0x08 */ s16 *owner;
    /* 0x0C */ f32 accumulator;
} EmitterData;

/* Data of each effect the emitter spawns. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 w;
} EmittedPoint;

/* Data of the particle source effect (func_150EC6B0). */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ f32 accumulator;
} ParticleSourceData;

/* Data of the fog flash effect (func_150ECB8C). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 r;
    /* 0x06 */ u8 g;
    /* 0x07 */ u8 b;
    /* 0x08 */ u8 a;
    /* 0x09 */ u8 rate;
} FogFlashData;

/* An effect (a struct260 with per-effect data at 0x28), as created by func_15149130. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* -1 ends the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ union {
        EmitterData emitter;
        EmittedPoint point;
        ParticleSourceData source;
        FogFlashData fog;
    } data;
} Effect;

/* Emitter effect: while its owner is alive, spawns effect 0x5A at random points on its shape. */
void func_150EC520(Effect *effect) {
    EmitterData *emitter;
    EmittedPoint point;
    f32 upper;

    if (func_151464B8(effect->data.emitter.owner) == 0) {
        emitter = &effect->data.emitter;
        emitter->accumulator += (9.04e-08f + func_150ADA68() * 9.87e-08f) * emitter->rate * D_800BE9A4;
        if (emitter->accumulator > 1.0f) {
            point.w = 0.0f;
            do {
                Effect *spawned;

                func_151432BC(emitter->shape, &point.x, &point.z, &upper, &point.y);
                spawned = (Effect *)func_15149130(func_150ADA20() % 131U + 80, -1, 0x5A, -1, TRUE, 0,
                                                  (struct37 *)sizeof(point), effect->owner, effect->kind);
                if (spawned != NULL) {
                    memcpy(&spawned->data, &point, sizeof(point));
                }
                emitter->accumulator -= 1.0f;
            } while (emitter->accumulator > 1.0f);
        }
    }
}

/* Per-particle data of the particles the source effect emits. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} SourceParticleData;

/* Live particle, as created by func_15130280 from a ParticleDesc. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ SourceParticleData data;
} Particle;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Particle source effect: emits particles from its position at a random rate. */
void func_150EC6B0(Effect *effect) {
    ParticleSourceData *source;

    source = &effect->data.source;
    source->accumulator += (1352.0f + func_150ADA68() * 1698.0f) * 0.0001f * D_800BE9A4;
    if (source->accumulator > 1.0f) {
        ParticleDesc spawn;
        SourceParticleData data;
        Particle *particle;

        data.unkC = 0.9704700112f;
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.fadeStart = 70;
        spawn.fadeRate = 3;
        spawn.flags = 0x80DE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        data.unk0 = 0;
        data.unk1 = 0;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.updateFunc = 22;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 70;
        spawn.r = 0x47;
        spawn.g = 0xC2;
        spawn.b = 0;
        spawn.a = 0xFF;
        spawn.r2 = 0xC0;
        spawn.g2 = 0x49;
        spawn.b2 = 0;
        spawn.brightness = 0xFF;
        spawn.scaleRate = 1.021028996f;
        spawn.pos = source->pos;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            data.unk2 = func_150ADA20() % 5U + 4;
            data.unk3 = func_150ADA20() % 5U + 4;
            data.unk4 = func_150ADA68() * 11.0f;
            data.unk8 = func_150ADA68() * 11.0f;
            spawn.gravity = func_150ADA68() * 0.172f + 0.09800000489f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.alpha = func_150ADA20() % 101U + 100;
            spawn.timer = func_150ADA20() % 51U + 80;
            spawn.width = spawn.height = func_150ADA68() * 80.0f + 92.0f;
            particle = func_15130280(&spawn, 0, 0, 0x10, effect->owner, effect->kind);
            if (particle != NULL) {
                memcpy(&particle->data, &data, sizeof(data));
            }
            source->accumulator -= 1.0f;
        } while (source->accumulator > 1.0f);
    }
}

/* Attaches a fog flash effect to obj: flashes it with fog colour (r, g, b, a) each frame,
 * for duration frames or, when duration is -1, until obj goes away. */
void func_150ECA68(struct127 *obj, u8 r, u8 g, u8 b, u8 a, u8 rate, s16 duration, u8 owner, s32 kind) {
    FogFlashData data;
    Effect *effect;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.a = a;
    data.rate = rate;
    data.r = r;
    data.g = g;
    data.b = b;

    effect = (Effect *)func_15149130((duration == -1) ? 300 : duration, -1, 0x5D, -1, (duration == -1) ? FALSE : TRUE,
                                     0x45, (struct37 *)sizeof(data), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect callback for the fog flash effect: hands func_15169850 the owner reference. */
void func_150ECB4C(Effect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&effect->data.fog.owner, (s32)&effect->data.fog.ownerId, (s32)effect);
}

extern void func_1502EA98(struct127 *, u8, u8, u8, u8, s32, u8);

/* Fog flash effect: re-flashes the owner's fog each frame, and ends once the owner is gone. */
void func_150ECB8C(Effect *effect) {
    FogFlashData *fog = &effect->data.fog;
    struct127 *owner = fog->owner;

    if ((owner->interaction_state == 0) || (owner->unique_id != fog->ownerId)) {
        effect->state = -1;
    } else {
        func_1502EA98(owner, fog->r, fog->g, fog->b, fog->a, FALSE, fog->rate);
    }
}

extern void func_151C9AC0(struct127 *, u8, s32);

/* Runs func_151C9AC0 on obj and flashes it green. */
void func_150ECC00(struct127 *obj, u8 owner, s32 kind) {
    func_151C9AC0(obj, owner, kind);
    func_150ECA68(obj, 0, 0xFF, 0, 0xFF, 4, -1, owner, kind);
}

void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
s32 func_1509BE40(s32, s32, s32, ...);
/* Current colour (r, g, b), its target and its easing velocity. */
extern f32 D_80088AD0;
extern f32 D_80088AD4;
extern f32 D_80088AD8;
extern f32 D_80088ADC;
extern f32 D_80088AE0;
extern f32 D_80088AE4;
extern f32 D_80088AE8;
extern f32 D_80088AEC;
extern f32 D_80088AF0;

/* Per-frame handler driven by setting 0x403B: while it is on, picks a target colour (one channel
 * at 128 per settings 0x4038-0x403A) and applies settings 0x9C/0x9A of mode 0x2007; otherwise
 * resets the colour to white. Then eases the current colour toward the target and sends it out. */
void func_150ECC70(struct108 *obj) {
    s32 value9C;
    s32 value9A;
    s32 half;

    if (func_1509BE40(1, 0x403B, 6, 0x2000) != 0) {
        s32 mode;

        func_1509BFB0(0, 0x4031, 1);
        func_1509BFB0(0, 0x4032, 1);
        func_1509BFB0(0, 0x4033, 1);
        obj->unk84 |= 0x80021200;
        if (func_1509BE40(1, 0x4038, 6, 0x2000) != 0) {
            D_80088ADC = 128.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x4039, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 128.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x403A, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 128.0f;
        } else {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        }
        mode = func_1509BE40(0, 0x2007, 0xB7) | 0x2000;
        if (func_1509BE40(1, 0x4010, 6, 0x2000) == 0) {
            obj->unk134 = 0;
            obj->unk190 = 0.0f;
            return;
        }
        // matching: mode always has 0x2000 set, but IDO still emits this test
        if (mode == 0) {
            return;
        }
        obj->unk134 = 1;
        value9C = func_1509BE40(1, mode, 0x9C, 0x2000);
        func_1509BFB0(1, 0x9000, 0x17, value9C);
        value9A = func_1509BE40(1, mode, 0x9A, 0x2000);
        half = value9A >> 1;
        if (value9A < 900) {
            value9A = 900;
        } else if (value9A > 1190) {
            value9A = 1190;
        }
        if (half < 180) {
            half = 180;
        } else if (half > 231) {
            half = 231;
        }
        func_1509BFB0(2, 0x9000, 8, value9A, half);
        obj->unk190 = 220.0f;
    } else {
        D_80088ADC = 255.0f;
        D_80088AE0 = 255.0f;
        D_80088AE4 = 255.0f;
        func_1509BFB0(0, 0x4031, 0);
        func_1509BFB0(0, 0x4032, 0);
        func_1509BFB0(0, 0x4033, 0);
        obj->unk84 &= ~0x80021200;
        obj->unk84 |= 8;
    }
    func_150495B0(&D_80088AD0, D_80088ADC, &D_80088AE8, 4.0f, 6.0f, obj->unk7B4);
    func_150495B0(&D_80088AD4, D_80088AE0, &D_80088AEC, 4.0f, 6.0f, obj->unk7B4);
    func_150495B0(&D_80088AD8, D_80088AE4, &D_80088AF0, 4.0f, 6.0f, obj->unk7B4);
    func_1509BFB0(1, 0x30F3, 0x12, (u8)D_80088AD0);
    func_1509BFB0(1, 0x30F4, 0x12, (u8)D_80088AD4);
    func_1509BFB0(1, 0x30F5, 0x12, (u8)D_80088AD8);
}
