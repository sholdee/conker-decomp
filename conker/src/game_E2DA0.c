#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_150B5A3C(vertex *pos, u8 owner, s32 kind);
void func_150B5E34(vertex *pos, u8 owner, s32 kind);
void func_150B6000(vertex *pos, u8 owner, s32 kind);
void func_150B60E0(Actor *obj, vertex *out);

/* Effect instance whose per-type data (from 0x28) tracks a game object. */
typedef struct ObjBurstEffect {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 timer; /* frames left (as in FlashEffect, game_1AC2F0.c) */
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Actor *obj;
    /* 0x2C */ u8 objUniqueId;
} ObjBurstEffect;

/* Effect template passed to func_15152B38. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} EffectSpawn; /* size 0x74 */

void func_15152B38(EffectSpawn *spawn, u8 owner, s32 kind);


typedef struct GameCommand {
    s16 opcode;
    s16 value;
} GameCommand;

/** Emits command 0x1A carrying object objIndex's facing angle (skipped when D_800C35EA == 1). */
GameCommand *func_150B58F0(GameCommand *cmd, s32 objIndex) {
    if (D_800C35EA == 1) {
        return cmd;
    }
    cmd->opcode = 0x1A;
    cmd->value = D_800CC2D0[objIndex].unk7A;
    return cmd + 1;
}

/** Randomly spawns particle bursts at the tracked object; ends the effect once that object is gone. */
void func_150B5950(ObjBurstEffect *effect) {
    vertex *posp;
    vertex pos;
    Actor *obj;

    obj = effect->obj;
    posp = &pos; // matching: IDO keeps &pos in a saved register only via a named pointer
    if ((obj->interaction_state == 0) || (obj->unique_id != effect->objUniqueId)) {
        effect->timer = -1;
        return;
    }

    if (obj->unk1D4 != NULL) {
        if (func_150ADA68() < 0.10824f) {
            pos.x = obj->x_position;
            pos.y = obj->y_position;
            pos.z = obj->z_position;
            func_150B5A3C(posp, effect->owner, effect->kind);
        }

        if (func_150ADA68() < 0.10824f) {
            func_150B60E0(obj, posp);
            func_150B5A3C(posp, effect->owner, effect->kind);
        }
    }
}

/** Spawns a func_15152B38 effect at pos, plus the func_150B5E34 and func_150B6000 particles. */
void func_150B5A3C(vertex *pos, u8 owner, s32 kind) {
    EffectSpawn spawn;

    spawn.unk00 = 5;
    spawn.unk04 = 5;
    spawn.pos = *pos;
    spawn.unk14 = 20.0f;
    spawn.unk18 = 10.08f;
    spawn.unk1C = -1.353f;
    spawn.unk20 = 0.731f;
    spawn.unk24 = 39.0f;
    spawn.unk28 = 35.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -31;
    spawn.unk32 = 80;
    spawn.unk34 = 3;
    spawn.unk38 = 2;
    spawn.unk3C = 20;
    spawn.unk3E = 30;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xFF;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xFF;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 36;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 31;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, owner, kind);
    func_150B5E34(pos, owner, kind);
    func_150B6000(pos, owner, kind);
}

/** Larger variant of func_150B5A3C (different template values). */
void func_150B5C38(vertex *pos, u8 owner, s32 kind) {
    EffectSpawn spawn;

    spawn.unk00 = 13;
    spawn.unk04 = 8;
    spawn.pos = *pos;
    spawn.unk14 = 15.88f;
    spawn.unk18 = 36.1f;
    spawn.unk1C = -3.603000164f;
    spawn.unk20 = 2.166000128f;
    spawn.unk24 = 152.0f;
    spawn.unk28 = 100.0f;
    spawn.unk2C = 0;
    spawn.unk2E = 0xFF;
    spawn.unk30 = -20;
    spawn.unk32 = 50;
    spawn.unk34 = 3;
    spawn.unk38 = 2;
    spawn.unk3C = 20;
    spawn.unk3E = 15;
    spawn.unk40 = 1;
    spawn.unk42 = 4;
    spawn.unk43 = 2;
    spawn.unk44 = 3;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0xFF;
    spawn.unk47 = 0xB4;
    spawn.unk48 = 0xFF;
    spawn.unk49 = 0;
    spawn.unk4A = 0;
    spawn.unk4B = 0;
    spawn.unk4C = 0;
    spawn.unk4D = 0xFF;
    spawn.unk4E = 0xFF;
    spawn.unk4F = 0xB4;
    spawn.unk50 = 0xFF;
    spawn.unk51 = 0;
    spawn.unk52 = 0;
    spawn.unk53 = 0;
    spawn.unk54 = 0;
    spawn.unk55 = 0xFF;
    spawn.unk56 = 0;
    spawn.unk57 = 3;
    spawn.unk58 = 36;
    spawn.unk5C = 0x200005;
    spawn.unk60 = 0x60600;
    spawn.unk64 = 8;
    spawn.unk66 = 31;
    spawn.unk68 = 1;
    spawn.unk6A = 0;
    spawn.unk6C = 1.0f;
    spawn.unk70 = -1;
    spawn.unk71 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, owner, kind);
    func_150B5E34(pos, owner, kind);
    func_150B6000(pos, owner, kind);
}

extern void *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/** Spawns one func_15130280 particle at pos with some randomized fields and flags. */
void func_150B5E34(vertex *pos, u8 owner, s32 kind) {
    ParticleDesc spawn;
    s32 flag80;
    s32 flag40;

    spawn.texture = 0x2B;
    spawn.renderMode = 0x4403;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x20000;
    spawn.timer = func_150ADA20() % 7U + 4;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.alpha = 0xFF;
    spawn.brightness = 0xFF;
    spawn.width = spawn.height = func_150ADA68() * 500.0f + 500.0f;
    spawn.pos = *pos;
    spawn.fadeStart = 3;
    spawn.fadeRate = 0x55;
    spawn.scaleStart = 1;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.gravity = 0.0f;
    spawn.scaleRate = 1.0f;
    flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
    flag80 = (func_150ADA20() & 1) ? 0x80 : 0;
    spawn.flags = flag80 | 1 | flag40 | 0x200 | 0x4000 | 0x8000;
    spawn.primColorMode = 6;
    spawn.envColorMode = 6;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 4;
    func_15130280(&spawn, 1, 0, 0, owner, kind);
}

extern s32 func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/** Spawns a func_151602C0 effect at pos (as integer coordinates) with randomized parameters. */
void func_150B6000(vertex *pos, u8 owner, s32 kind) {
    Header header;
    Header2 ipos;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = func_150ADA20() % 9U + 3;
    header.unk4 = 0;

    ipos.unk0 = pos->x;
    ipos.unk4 = pos->y;
    ipos.unk8 = pos->z;

    func_151602C0(&header, &ipos, func_150ADA20() % 201U + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, owner, kind);
}

extern void func_15143134(vertex *in, vertex *out, void *mtx);
extern vertex D_8009FC30;

/** Transforms the fixed point D_8009FC30 by the matrix at obj->unk1D4 + 0x140 into out. */
void func_150B60E0(Actor *obj, vertex *out) {
    func_15143134(&D_8009FC30, out, (u8 *)obj->unk1D4 + 0x140);
}

/* Emitter of the bolt-ring effect: cylinder centre (x, y, z), radius and height, and its state. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ u8 padA[0xA];
    /* 0x14 */ u8 state;
} BoltEmitter;

typedef struct {
    /* 0x00 */ BoltEmitter *emitter;
    /* 0x04 */ f32 spawnAccum;
    /* 0x08 */ u8 emitterState;
} BoltRingData;

/* Effect instance whose per-type data (from 0x28) is a BoltRingData. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ BoltRingData data;
} BoltRingEffect;

/* Spawn parameters for each bolt of a chain (same layout as in game_131F30.c / game_133190.c). */
typedef struct BoltChainConfig {
    /* 0x00 */ f32 widthBase;
    /* 0x04 */ f32 widthRange;
    /* 0x08 */ f32 jitter;
    /* 0x0C */ f32 bendBase;
    /* 0x10 */ f32 bendRange;
    /* 0x14 */ s16 bendSpread;
    /* 0x16 */ s16 rejitterBase;
    /* 0x18 */ s16 rejitterRange;
    /* 0x1A */ s16 lifeBase;
    /* 0x1C */ s16 lifeRange;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 segmentsBase;
    /* 0x24 */ s32 segmentsRange;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 alphaBase;
    /* 0x2C */ u8 alphaRange;
} BoltChainConfig; /* size 0x30 */

extern void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z);
extern void func_15106F98(vertex *start, vertex *end, s32 depth, BoltChainConfig *config, f32 spread, s32 type, s32 owner, s32 kind);

/**
 * While the emitter is not in state 1, spawns bolts between random points on the rim of its
 * cylinder at a randomized rate; sends message 0x4A (see func_150B6450) when it enters state 1.
 */
void func_150B6110(BoltRingEffect *effect) {
    BoltRingData *data;
    BoltEmitter *emitter;
    u8 prevState;
    u8 angle;

    emitter = effect->data.emitter;
    prevState = effect->data.emitterState;
    effect->data.emitterState = emitter->state;
    if (emitter->state == 1) {
        if (effect->data.emitterState != prevState) {
            func_151494E0(0, 0x4A);
        }
        return;
    }
    data = &effect->data;
    data->spawnAccum += (0.15f + func_150ADA68() * 0.155f) * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        BoltChainConfig config;
        vertex start;
        vertex end;
        f32 spread;

        config.widthBase = 35.0f;
        config.widthRange = 35.0f;
        config.jitter = 76.0f;
        config.bendBase = 103.0f;
        config.bendRange = 281.0f;
        config.bendSpread = 40;
        config.rejitterBase = 0;
        config.rejitterRange = 4;
        config.lifeBase = 3;
        config.lifeRange = 7;
        config.segmentsBase = 4;
        config.segmentsRange = 3;
        config.r = 0x7E;
        config.g = 0xF9;
        config.b = 0xFF;
        config.alphaBase = 0x7F;
        config.alphaRange = 0x80;
        spread = 0.4350000322f;
        start.y = emitter->y + emitter->height;
        end.y = emitter->y + emitter->height;
        do {
            angle = func_150ADA20();
            func_1514373C((u32)angle, emitter->radius, &start.x, &start.z); // matching: (u32) selects IDO's unsigned int-to-float sequence
            start.x += emitter->x;
            start.z += emitter->z;
            angle += func_150ADA20() % 129 + 64;
            func_1514373C((u32)angle, emitter->radius, &end.x, &end.z);
            end.x += emitter->x;
            end.z += emitter->z;
            func_15106F98(&start, &end, 4, &config, spread, 1, effect->owner, effect->kind);
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/** Message handler: on message 0x4A, releases the effect object via func_1516972C. */
void func_150B6450(struct102 *effect, s32 arg1, u8 msg) {
    if (msg == 0x4A) {
        func_1516972C(effect);
    }
}

/*
 * Trail settings passed to func_150B3F5C: the first 0x40 bytes of TrailState (game_E1280.c),
 * whose remaining fields are emitter state that the spawner fills in.
 */
typedef struct {
    /* 0x00 */ vertex baseVel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseJitter;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedSwing;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ f32 emitRate;
    /* 0x24 */ u8 shade;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 alpha;
    /* 0x28 */ f32 widthGrowth;
    /* 0x2C */ f32 width;
    /* 0x30 */ u8 texStep;
    /* 0x31 */ u8 texStepJitter;
    /* 0x32 */ u8 fadeTailCount;
    /* 0x33 */ u8 fadeTailStep;
    /* 0x34 */ u8 fadeHeadCount;
    /* 0x35 */ u8 fadeHeadStep;
    /* 0x36 */ s16 life;
    /* 0x38 */ u8 burstPreset;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 burstRate;
} TrailParams; /* size 0x40 */

extern void *func_150B3F5C(TrailParams *params, vertex *pos, s16 capacity);
extern void func_15143794(s16 yaw, s16 pitch, f32 length, vertex *out);

/* Per-variant trail settings used by func_150B648C. */
extern vertex D_8009FC3C[];   /* emitter position (variants 1 and 2) */
extern f32 D_8009FC60[];      /* gravity */
extern f32 D_8009FC6C[];      /* widthGrowth */
extern f32 D_8009FC78[];      /* width */
extern s16 D_8009FC84[];      /* life */
extern s16 D_8009FC8E[][2];   /* capacity */

/** Spawns trail variant 0, 1 or 2 (variant 0 at a position from func_151423D8). */
void func_150B648C(u8 variant) {
    TrailParams params;
    vertex pos;

    if (variant == 0) {
        params.baseVel.x = func_151423D8(0xAA) * 10.0f;
        params.baseVel.y = 3.0f;
        params.baseVel.z = func_151423D8(0xEA) * 10.0f;
        pos.x = 1005.0f - (func_151423D8(0xAA) * 150.0f);
        pos.y = 351.0f;
        pos.z = -1706.0f - (func_151423D8(0xEA) * 150.0f);
    } else if (variant == 1) {
        func_15143794(-69, 28, 9.349999f, &params.baseVel);
        pos = D_8009FC3C[variant];
    } else {
        func_15143794(24, 15, 9.349999f, &params.baseVel);
        pos = D_8009FC3C[variant];
    }

    params.phaseJitter = 4;
    params.flags = 6; /* fade tail | fade head */
    params.gravity = D_8009FC60[variant];
    params.shade = 0x80;
    params.alpha = 0xFF;
    params.texStep = 2;
    params.texStepJitter = 5;
    params.fadeTailCount = 5;
    params.fadeTailStep = 0x33;
    params.fadeHeadCount = 3;
    params.fadeHeadStep = 0x55;
    params.burstPreset = 0;
    params.speedBase = 3.3f;
    params.speedSwing = 0.7f;
    params.emitRate = 0.4f;
    params.widthGrowth = D_8009FC6C[variant];
    params.width = D_8009FC78[variant];
    params.life = D_8009FC84[variant];
    params.burstRate = 0.3f;
    func_150B3F5C(&params, &pos, D_8009FC8E[variant][0]);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ u8 mode;
} Input150B66DC;

typedef struct {
    /* 0x00 */ u8 pad0[9];
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 padA[0x25];
    /* 0x2F */ u8 unk2F;
} Output150B66DC;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Output150B66DC *out;
    /* 0x18 */ Input150B66DC *in;
} Arg150B66DC;

/** Sets out->unk9 for mode 15; otherwise clears it and sets out->unk2F (20 for mode 16, else 40). */
s32 func_150B66DC(Arg150B66DC *ctx) {
    Input150B66DC *in = ctx->in;

    switch (in->mode - 15) { // matching: `switch (in->mode)` with case 15-17 labels mismatches
    case 0: /* mode 15 */
        ctx->out->unk9 = 1;
        break;
    case 1: /* mode 16 */
        ctx->out->unk9 = 0;
        ctx->out->unk2F = 20;
        break;
    case 2: /* mode 17 */
    default:
        ctx->out->unk9 = 0;
        ctx->out->unk2F = 40;
        break;
    }
    return 1;
}

extern void func_15182670(u8 r, u8 g, u8 b, u8 alpha, s32 duration, u8 view, u8 owner, s32 kind);

/** Flashes view 0 light blue (0xCC, 0xCC, 0xFF) at a random alpha 200-255 for 15-25 frames. */
void func_150B6754(u8 owner, s32 kind) {
    u32 rand2;
    u32 rand1;

    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    func_15182670(0xCC, 0xCC, 0xFF, rand1 % 56 + 200, rand2 % 11 + 15, 0, owner, kind);
}
