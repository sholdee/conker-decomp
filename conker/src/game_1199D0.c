#include <ultra64.h>
#include "functions.h"
#include "variables.h"


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

/* Particle spawn template passed to func_15130280 (same layout as ParticleSpawn in game_1028F0.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67[0x70 - 0x67];
} ParticleSpawn; /* size 0x70 */

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

/* Live particle, as created by func_15130280 from a ParticleSpawn. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ SourceParticleData data;
} Particle;

extern Particle *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);

/* Particle source effect: emits particles from its position at a random rate. */
void func_150EC6B0(Effect *effect) {
    ParticleSourceData *source;

    source = &effect->data.source;
    source->accumulator += (1352.0f + func_150ADA68() * 1698.0f) * 0.0001f * D_800BE9A4;
    if (source->accumulator > 1.0f) {
        ParticleSpawn spawn;
        SourceParticleData data;
        Particle *particle;

        data.unkC = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 70;
        spawn.unk20 = 3;
        spawn.flags = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        data.unk0 = 0;
        data.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk62 = 22;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 70;
        spawn.unk14 = 0x47;
        spawn.unk15 = 0xC2;
        spawn.unk16 = 0;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0xC0;
        spawn.unk19 = 0x49;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.021028996f;
        spawn.pos = source->pos;
        spawn.unk3C.x = 0.0f;
        spawn.unk3C.y = 0.0f;
        spawn.unk3C.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            data.unk2 = func_150ADA20() % 5U + 4;
            data.unk3 = func_150ADA20() % 5U + 4;
            data.unk4 = func_150ADA68() * 11.0f;
            data.unk8 = func_150ADA68() * 11.0f;
            spawn.unk54 = func_150ADA68() * 0.172f + 0.09800000489f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 101U + 100;
            spawn.unkA = func_150ADA20() % 51U + 80;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 80.0f + 92.0f;
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

typedef struct Game1199D0Camera {
    u8 pad00[0x84];
    s32 flags;
    u8 pad88[0xAC];
    s32 field134;
    u8 pad138[0x58];
    f32 field190;
    u8 pad194[0x620];
    f32 delta;
} Game1199D0Camera;

void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
s32 func_1509BE40(s32, s32, s32, ...);
extern f32 D_80088AD0;
extern f32 D_80088AD4;
extern f32 D_80088AD8;
extern f32 D_80088ADC;
extern f32 D_80088AE0;
extern f32 D_80088AE4;
extern f32 D_80088AE8;
extern f32 D_80088AEC;
extern f32 D_80088AF0;

void func_150ECC70(Game1199D0Camera *arg0) {
    s32 value;
    s32 width;
    s32 height;

    if (func_1509BE40(1, 0x403B, 6, 0x2000) != 0) {
        s32 mode;

        func_1509BFB0(0, 0x4031, 1);
        func_1509BFB0(0, 0x4032, 1);
        func_1509BFB0(0, 0x4033, 1);
        arg0->flags = (s32) (arg0->flags | 0x80021200);
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
            arg0->field134 = 0;
            arg0->field190 = 0.0f;
            return;
        }
        if (mode == 0) {
            return;
        }
        arg0->field134 = 1;
        value = func_1509BE40(1, mode, 0x9C, 0x2000);
        func_1509BFB0(1, 0x9000, 0x17, value);
        width = func_1509BE40(1, mode, 0x9A, 0x2000);
        height = width >> 1;
        if (width < 900) {
            width = 900;
        } else if (width > 1190) {
            width = 1190;
        }
        if (height < 180) {
            height = 180;
        } else if (height > 231) {
            height = 231;
        }
        func_1509BFB0(2, 0x9000, 8, width, height);
        arg0->field190 = 220.0f;
    } else {
        D_80088ADC = 255.0f;
        D_80088AE0 = 255.0f;
        D_80088AE4 = 255.0f;
        func_1509BFB0(0, 0x4031, 0);
        func_1509BFB0(0, 0x4032, 0);
        func_1509BFB0(0, 0x4033, 0);
        arg0->flags &= 0x7FFDEDFF;
        arg0->flags |= 8;
    }
    func_150495B0(&D_80088AD0, D_80088ADC, &D_80088AE8, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD4, D_80088AE0, &D_80088AEC, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD8, D_80088AE4, &D_80088AF0, 4.0f, 6.0f, arg0->delta);
    func_1509BFB0(1, 0x30F3, 0x12, (u32) D_80088AD0 & 0xFF);
    func_1509BFB0(1, 0x30F4, 0x12, (u32) D_80088AD4 & 0xFF);
    func_1509BFB0(1, 0x30F5, 0x12, (u32) D_80088AD8 & 0xFF);
}
