#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/math.h"

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color8;

/*
 * Particle-burst config read by func_15152520 (same layout as game_17CAF0.c's struct Local15152520Arg).
 * Integer pairs are random ranges, (rand % (range + 1)) + base; float pairs are rand01 * range + base.
 */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ s16 yawBase;
    /* 0x16 */ s16 yawRange;
    /* 0x18 */ s16 pitchBase;
    /* 0x1A */ s16 pitchRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ Vec3f unk50;
    /* 0x5C */ Vec3f unk5C;
    /* 0x68 */ Color8 unk68;
    /* 0x6C */ Color8 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 unk71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
} ParticleBurst;

void func_15142314(struct255 *bones, s32 bone, Vec3f *out);
void func_15152520(ParticleBurst *burst, u8 arg1, s32 arg2);

/* Spawns a particle burst at bone 10 of `actor`. */
void func_15193660(Actor *actor, u8 arg1, s32 arg2) {
    ParticleBurst burst;
    struct255 *bones;

    if ((actor != NULL) && (actor->interaction_state != 0)) {
        bones = actor->unk1D4;
        if (actor->unk1D4 == NULL) { // matching: the original reloads the field rather than testing bones
            return;
        }
        func_15142314(bones, 10, &burst.position);
        burst.countBase = 8;
        burst.countRange = 7;
        burst.yawBase = 0;
        burst.yawRange = 255;
        burst.pitchBase = -33;
        burst.pitchRange = 24;
        burst.speedBase = 12.40300083f;
        burst.speedRange = 5.039000034f;
        burst.unk24 = 0.908870995f;
        burst.unk28 = -0.7070000172f;
        burst.unk2C = 0.2610000074f;
        burst.unk30 = 40;
        burst.unk32 = 35;
        burst.unk34 = 0xA0;
        burst.unk38 = 0x13;
        burst.unk3C = 0;
        burst.unk40 = 0.0f;
        burst.unk44 = 1.960000038f;
        burst.unk48 = 0.3070000112f;
        burst.unk4C = 0.0f;
        burst.unk50.x = burst.unk50.z = -12.85999966f;
        burst.unk50.y = 0; // matching: integer 0 (sw zero); 0.0f emits an FP-register store
        burst.unk5C.x = burst.unk5C.z = 25.37999916f;
        burst.unk5C.y = 0;
        burst.unk70 = 0x21;
        burst.unk68.red = 255;
        burst.unk68.green = 255;
        burst.unk68.blue = 255;
        burst.unk68.alpha = 255;
        burst.unk6C.red = 255;
        burst.unk6C.green = 255;
        burst.unk6C.blue = 255;
        burst.unk6C.alpha = 255;
        burst.unk71 = 0xF;
        burst.unk72 = 25;
        burst.unk74 = 10;
        func_15152520(&burst, arg1, arg2);
    }
}

/* Descriptor passed to func_15134DAC (same layout as game_1228D0.c's struct_150F6890). */
typedef struct {
    /* 0x00 */ u8  ownerId;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ Actor *owner;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} EffectDesc;

void *func_15134DAC(EffectDesc *desc, s32 arg1);

/* Creates a func_15134DAC object attached to `owner`. */
void func_151937F4(Actor *owner, s32 arg1, s32 arg2) {
    EffectDesc desc;

    if ((owner != NULL) && (owner->interaction_state != 0)) {
        desc.ownerId = owner->unique_id;
        desc.owner = owner;
        desc.unk8 = 0;
        desc.unkC = -28.0f;
        desc.unk10 = 6.0f;
        desc.unk14 = 1.0f;
        desc.unk18 = -58.0f;
        desc.unk1C = -3.0f;
        desc.unk20 = -31.0f;
        desc.unk24 = 2;
        desc.unk26 = 30;
        desc.unk28 = 25;
        desc.unk2A = 750;
        desc.unk2E = 3;
        desc.unk2F = 3;
        desc.unk30 = -1;
        desc.unk34 = 0.3115699887f;
        desc.unk38 = 4;
        desc.unk39 = -1;
        func_15134DAC(&desc, 0);
    }
}

/* Particle emitter that func_151938E4 initialises and func_151938FC spawns from. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0xF];
    /* 0x1C */ Actor *owner;
    /* 0x20 */ u8 pad20[0x54];
    /* 0x74 */ f32 scale; /* scales (to - from) into the spawn velocity */
} Emitter;

/* Sets the emitter's velocity scale. */
s32 func_151938E4(Emitter *emitter) {
    emitter->scale = 0.40200001f;
    return TRUE;
}

/* Second argument of func_15157898 (game_17CAF0.c's struct Local15152520Motion). */
typedef struct {
    /* 0x00 */ Vec3f rotation; /* degrees */
    /* 0x0C */ f32 unkC;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f unk1C;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[3];
} ParticleMotion;

/* First argument of func_15157898 (game_17CAF0.c's struct Local15152520Params). */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ Color8 unk30;
    /* 0x34 */ Color8 unk34;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ Actor *owner;
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f position;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} ParticleParams;

extern f32 D_800A81F0;

s32 func_1515C0F8(Actor *actor, Vec3f **motion);
void func_15157898(ParticleParams *params, ParticleMotion *motion, s32, f32, s32, s32, s32, u8, s32);

/*
 * Spawns one particle on the segment `from` -> `to`, at fraction `t` along it, moving with velocity
 * (to - from) * emitter->scale. Skipped while the owner is in water or moving slower than 8.1.
 */
void func_151938FC(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, Emitter *emitter) {
    ParticleParams params;
    ParticleMotion motion;
    Vec3f *ownerMotion;
    s32 pad[2]; // matching: unused stack slots
    f32 x;
    f32 y;
    f32 z;

    if (!func_1515C0F8(emitter->owner, &ownerMotion)) {
        return;
    }
    if (emitter->owner->in_water) {
        return;
    }

    x = ownerMotion->x;
    y = ownerMotion->y;
    z = ownerMotion->z;
    if (sqrtf((x * x) + (y * y) + (z * z)) < 8.100000381f) {
        return;
    }

    params.unk38 = 0;
    params.flags = 0x27;
    params.unk01 = 0;
    params.unk02 = 1;
    params.unk03 = -1;
    params.unk04 = -1;
    params.unk06 = (func_150ADA20() % 41U) + 90;
    params.unk08 = 0xA0;
    params.unk0C = 0x13;
    params.unk14 = 0x220405;
    params.unk18 = 0x40200;
    params.unk2D = 8;
    params.unk1C = 1;
    params.unk20 = 0x38;
    params.unk10 = 0;
    params.unk2C = 0;
    params.unk24 = 0x80;
    params.unk28 = 0x20;
    params.unk30.red = 255;
    params.unk30.green = 255;
    params.unk30.blue = 255;
    params.unk30.alpha = 255;
    params.unk34.red = 255;
    params.unk34.green = 255;
    params.unk34.blue = 255;
    params.unk34.alpha = 255;
    params.owner = emitter->owner;
    params.ownerId = emitter->owner->unique_id;
    params.unk41 = 1;
    params.position = *from;
    motion.velocity.x = (to->x - from->x) * emitter->scale;
    motion.velocity.y = (to->y - from->y) * emitter->scale;
    motion.velocity.z = (to->z - from->z) * emitter->scale;
    params.position.x += motion.velocity.x * t;
    params.position.y += motion.velocity.y * t;
    params.position.z += motion.velocity.z * t;
    motion.rotation.x = func_150ADA68() * 360.0f;
    motion.rotation.y = func_150ADA68() * 360.0f;
    motion.rotation.z = func_150ADA68() * 360.0f;
    func_150ADA68();
    motion.unkC = 340.0f * D_800A81F0; // matching: the original multiplies at run time; a literal 0.001f would be constant-folded
    motion.unk1C.x = ((func_150ADA68() * 1000.0f) + -500.0f) * 0.009999999776f;
    motion.unk1C.y = 0.0f;
    motion.unk1C.z = ((func_150ADA68() * 1000.0f) + -500.0f) * 0.009999999776f;
    motion.unk28 = ((func_150ADA68() * 121.0f) + -135.0f) * 0.001000000047f;
    motion.unk2C = 0.9828829765f;
    params.unk54 = 25;
    params.unk56 = 10;
    motion.unk30 = 0xF;
    func_15157898(&params, &motion, 0, (func_150ADA68() * 1814.0f) * 0.001000000047f, 0, 0, 0, emitter->unkC, emitter->unk1);
}
