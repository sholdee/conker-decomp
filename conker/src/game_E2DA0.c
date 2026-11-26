#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Effect instance whose per-type data (from 0x28) tracks a game object. */
struct ObjBurstEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Actor *obj;
    /* 0x2C */ u8 objUniqueId;
};


/** Emits command 0x1A carrying object objIndex's facing angle (skipped when D_800C35EA == 1). */
GameCommand *func_150B58F0(GameCommand *cmd, s32 objIndex) {
    if (D_800C35EA[0] == 1) {
        return cmd;
    }
    cmd->opcode = 0x1A;
    cmd->value = movement_obj[objIndex].langle;
    return cmd + 1;
}

/** Randomly spawns particle bursts at the tracked object; ends the effect once that object is gone. */
void func_150B5950(ObjBurstEffect *effect) {
    Vec3f *posp;
    Vec3f pos;
    Actor *obj;

    obj = effect->obj;
    posp = &pos; // matching: IDO keeps &pos in a saved register only via a named pointer
    if ((obj->type == 0) || (obj->unique_id != effect->objUniqueId)) {
        effect->base.header.state = -1;
        return;
    }

    if (obj->ob_matrix_ptr != NULL) {
        if (func_150ADA68() < 0.10824f) {
            pos.x = obj->xpos;
            pos.y = obj->ypos;
            pos.z = obj->zpos;
            func_150B5A3C(posp, effect->base.header.owner, effect->base.header.kind);
        }

        if (func_150ADA68() < 0.10824f) {
            func_150B60E0(obj, posp);
            func_150B5A3C(posp, effect->base.header.owner, effect->base.header.kind);
        }
    }
}

/** Spawns a func_15152B38 effect at pos, plus the func_150B5E34 and func_150B6000 particles. */
void func_150B5A3C(Vec3f *pos, u8 owner, s32 kind) {
    TrailBurst spawn;

    spawn.countBase = 5;
    spawn.countRange = 5;
    spawn.pos = *pos;
    spawn.sizeBase = 20.0f;
    spawn.sizeRange = 10.08f;
    spawn.gravityBase = -1.353f;
    spawn.gravityRange = 0.731f;
    spawn.speedBase = 39.0f;
    spawn.speedRange = 35.0f;
    spawn.angles.yawBase = 0;
    spawn.angles.yawRange = 0xFF;
    spawn.angles.pitchBase = -31;
    spawn.angles.pitchRange = 80;
    spawn.capacityBase = 3;
    spawn.capacityRange = 2;
    spawn.lifeBase = 20;
    spawn.lifeRange = 30;
    spawn.headerFlags = 1;
    spawn.flags = 4;
    spawn.primMode = 2;
    spawn.envMode = 3;
    spawn.colorBase.red = 0xFF;
    spawn.colorBase.green = 0xFF;
    spawn.colorBase.blue = 0xFF;
    spawn.colorBase.alpha = 0xFF;
    spawn.colorRange.red = 0;
    spawn.colorRange.green = 0;
    spawn.colorRange.blue = 0;
    spawn.colorRange.alpha = 0;
    spawn.color2Base.red = 0xFF;
    spawn.color2Base.green = 0xFF;
    spawn.color2Base.blue = 0xFF;
    spawn.color2Base.alpha = 0xFF;
    spawn.color2Range.red = 0;
    spawn.color2Range.green = 0;
    spawn.color2Range.blue = 0;
    spawn.color2Range.alpha = 0;
    spawn.colorLevelBase = 0xFF;
    spawn.colorLevelRange = 0;
    spawn.renderMode = 3;
    spawn.unk21 = 36;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0x60600;
    spawn.fadeStart = 8;
    spawn.fadeRate = 31;
    spawn.shrinkStart = 1;
    spawn.shrinkPaused = 0;
    spawn.shrinkFactor = 1.0f;
    spawn.unk38 = -1;
    spawn.unk39 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, owner, kind);
    func_150B5E34(pos, owner, kind);
    func_150B6000(pos, owner, kind);
}

/** Larger variant of func_150B5A3C (different template values). */
void func_150B5C38(Vec3f *pos, u8 owner, s32 kind) {
    TrailBurst spawn;

    spawn.countBase = 13;
    spawn.countRange = 8;
    spawn.pos = *pos;
    spawn.sizeBase = 15.88f;
    spawn.sizeRange = 36.1f;
    spawn.gravityBase = -3.603000164f;
    spawn.gravityRange = 2.166000128f;
    spawn.speedBase = 152.0f;
    spawn.speedRange = 100.0f;
    spawn.angles.yawBase = 0;
    spawn.angles.yawRange = 0xFF;
    spawn.angles.pitchBase = -20;
    spawn.angles.pitchRange = 50;
    spawn.capacityBase = 3;
    spawn.capacityRange = 2;
    spawn.lifeBase = 20;
    spawn.lifeRange = 15;
    spawn.headerFlags = 1;
    spawn.flags = 4;
    spawn.primMode = 2;
    spawn.envMode = 3;
    spawn.colorBase.red = 0xFF;
    spawn.colorBase.green = 0xFF;
    spawn.colorBase.blue = 0xB4;
    spawn.colorBase.alpha = 0xFF;
    spawn.colorRange.red = 0;
    spawn.colorRange.green = 0;
    spawn.colorRange.blue = 0;
    spawn.colorRange.alpha = 0;
    spawn.color2Base.red = 0xFF;
    spawn.color2Base.green = 0xFF;
    spawn.color2Base.blue = 0xB4;
    spawn.color2Base.alpha = 0xFF;
    spawn.color2Range.red = 0;
    spawn.color2Range.green = 0;
    spawn.color2Range.blue = 0;
    spawn.color2Range.alpha = 0;
    spawn.colorLevelBase = 0xFF;
    spawn.colorLevelRange = 0;
    spawn.renderMode = 3;
    spawn.unk21 = 36;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0x60600;
    spawn.fadeStart = 8;
    spawn.fadeRate = 31;
    spawn.shrinkStart = 1;
    spawn.shrinkPaused = 0;
    spawn.shrinkFactor = 1.0f;
    spawn.unk38 = -1;
    spawn.unk39 = 0;
    spawn.unk72 = -1;
    spawn.unk73 = -1;

    func_15152B38(&spawn, owner, kind);
    func_150B5E34(pos, owner, kind);
    func_150B6000(pos, owner, kind);
}

/** Spawns one func_15130280 particle at pos with some randomized fields and flags. */
void func_150B5E34(Vec3f *pos, u8 owner, s32 kind) {
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

/** Spawns a func_151602C0 effect at pos (as integer coordinates) with randomized parameters. */
void func_150B6000(Vec3f *pos, u8 owner, s32 kind) {
    LightEffectHeader header;
    Vec3i ipos;

    header.flags = 3;
    header.update = -1;
    header.lifetime = func_150ADA20() % 9U + 3;
    header.lightType = 0;

    ipos.x = pos->x;
    ipos.y = pos->y;
    ipos.z = pos->z;

    func_151602C0(&header, &ipos, func_150ADA20() % 201U + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, owner, kind);
}

static const Vec3f D_8009FC30 = { 0.0f, -205.0f, 0.0f };

/** Transforms the fixed point D_8009FC30 by bone matrix 5 of obj into out. */
void func_150B60E0(Actor *obj, Vec3f *out) {
    func_15143134(&D_8009FC30, out, &obj->ob_matrix_ptr[5]);
}


/* Effect instance whose per-type data (from 0x28) is a BoltRingData. */
struct BoltRingEffect {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ BoltRingData data;
};

/**
 * While the emitter is not in state 1, spawns bolts between random points on the rim of its
 * cylinder at a randomized rate; sends message 0x4A (see func_150B6450) when it enters state 1.
 */
void func_150B6110(BoltRingEffect *effect) {
    BoltRingData *data;
    TriggerZone *emitter;
    u8 prevState;
    u8 angle;

    emitter = effect->data.emitter;
    prevState = effect->data.emitterState;
    effect->data.emitterState = emitter->inactive;
    if (emitter->inactive == 1) {
        if (effect->data.emitterState != prevState) {
            func_151494E0(0, 0x4A);
        }
        return;
    }
    data = &effect->data;
    data->spawnAccum += (0.15f + func_150ADA68() * 0.155f) * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        BoltChainConfig config;
        Vec3f start;
        Vec3f end;
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
        start.y = emitter->y + emitter->ySize;
        end.y = emitter->y + emitter->ySize;
        do {
            angle = func_150ADA20();
            func_1514373C((u32)angle, emitter->xSize, &start.x, &start.z); // matching: (u32) selects IDO's unsigned int-to-float sequence
            start.x += emitter->x;
            start.z += emitter->z;
            angle += func_150ADA20() % 129 + 64;
            func_1514373C((u32)angle, emitter->xSize, &end.x, &end.z);
            end.x += emitter->x;
            end.z += emitter->z;
            func_15106F98(&start, &end, 4, &config, spread, 1, effect->owner, effect->kind);
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/** Message handler: on message 0x4A, releases the effect object via func_1516972C. */
void func_150B6450(EffectHeader *effect, s32 arg1, u8 msg) {
    if (msg == 0x4A) {
        func_1516972C(effect);
    }
}

/* Per-variant trail settings used by func_150B648C. */
static const Vec3f D_8009FC3C[] = { /* emitter position (variants 1 and 2) */
    { 0.0f, 0.0f, 0.0f },
    { 2325.0f, 2915.0f, 904.0f },
    { -1815.0f, 1412.0f, -3192.0f },
};
static const f32 D_8009FC60[] = { -3.71f, -1.302f, -1.302f }; /* gravity */
static const f32 D_8009FC6C[] = { 1.84f, 1.5430001f, 1.5430001f }; /* widthGrowth */
static const f32 D_8009FC78[] = { 47.0f, 130.0f, 130.0f };    /* width */
static const s16 D_8009FC84[] = { 30, 70, 70 };               /* life */
static const s32 D_8009FC8C[] = { 20, 50, 50 };               /* capacity */

/** Spawns trail variant 0, 1 or 2 (variant 0 at a position from func_151423D8). */
void func_150B648C(u8 variant) {
    TrailStateParams params;
    Vec3f pos;

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
    func_150B3F5C(&params, &pos, D_8009FC8C[variant]);
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

struct Arg150B66DC {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Output150B66DC *out;
    /* 0x18 */ Input150B66DC *in;
};

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

/** Flashes view 0 light blue (0xCC, 0xCC, 0xFF) at a random alpha 200-255 for 15-25 frames. */
void func_150B6754(u8 owner, s32 kind) {
    u32 rand2;
    u32 rand1;

    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    func_15182670(0xCC, 0xCC, 0xFF, rand1 % 56 + 200, rand2 % 11 + 15, 0, owner, kind);
}
