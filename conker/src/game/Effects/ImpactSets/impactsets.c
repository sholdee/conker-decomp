#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 base;
    /* 0x06 */ u16 range;
} FxSpawnRange; // size 0x8

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ f32 unk8;
} FxImpactEntry; // size 0xC

/* Random ranges for one spray particle: each value is base + rand() % (range + 1) or
 * base + randf() * range. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 pitchBase;
    /* 0x04 */ u16 pitchRange;
    /* 0x08 */ f32 speedBase;
    /* 0x0C */ f32 speedRange;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 chance;
} FxSprayParams; // size 0x30

/* Passed through particle_emitOnRandomCircle to the spawn callbacks (e.g. effect_spawnImpactSprayParticle). */
struct FxSpawnPayload {
    /* 0x00 */ CollisionHit *hit;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 kind;
};

/* The impact types' effect selections (effect_spawnImpact, effect_spawnImpactSprayParticle, particle_spawnYawSteam). */
const FxImpactSet gImpactSets[] = {
    { 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 0 },
    { 2, 2, 2, 2, 0 },
    { 3, 3, 3, 3, 0 },
    { -1, -1, -1, -1, 0 },
    { 4, 4, 4, 4, 0 },
};

/* The first effect of an impact (FxImpactSet.first). */
static const FxSpawnRange gImpactFirstEffects[] = {
    { 10.0f, 10, 0 },
    { 6.0f, 8, 4 },
    { 8.0f, 10, 5 },
    { 6.0f, 7, 2 },
    { 3.0f, 3, 2 },
};

/* The spray particles of an impact (FxImpactSet.firstStyle). */
static const FxSprayParams gImpactSprays[] = {
    { 120, 0, -45, 30, 10.0f, 0.0f, 6, 0, 300, 0, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    { 100, 40, -50, 13, 13.99f, 3.04f, 3, 2, 45, 25, 5.0499997f, 1.1999999f, 0.9f, 0.2f, 0.5f },
    { 100, 40, -63, 8, 17.23f, 7.3599997f, 4, 2, 80, 50, 6.0f, 1.0f, 0.765f, 0.15f, 0.4f },
    { 100, 40, -46, 11, 9.48f, 1.55f, 3, 2, 60, 30, 3.82f, 2.3799999f, 0.583f, 0.12f, 0.5f },
    { 120, 120, -63, 20, 9.0f, 3.0f, 4, 1, 60, 30, 4.49f, 1.4499999f, 0.583f, 0.12f, 0.5f },
};

/* The second effect of an impact (FxImpactSet.second). */
static const FxSpawnRange gImpactSecondEffects[] = {
    { 10.0f, 10, 0 },
    { 10.0f, 15, 10 },
    { 10.0f, 15, 5 },
    { 10.0f, 15, 5 },
    { 5.0f, 15, 10 },
};

/* The particle parameters of the impact emitters (FxImpactSet.emitter, particle_spawnYawSteam in game/Particle/EmitterQuads/emitterquads.c). */
const EmitterConfig gImpactEmitterConfigs[] = {
    { 10, 25, -63, 50, 5.19f, 9.849999f, 40, 30, 100, 155, 49.0f, 105.5f, -1.0780001f, 0.49400002f, 1.0f },
    { 10, 25, -48, 4, 7.0699997f, 3.9099998f, 50, 20, 100, 100, 39.5f, 65.0f, -0.57000005f, 0.07f, 0.4f },
    { 10, 25, -61, 8, 6.99f, 3.76f, 50, 20, 100, 100, 40.0f, 70.0f, -0.61f, 0.07f, 0.4f },
    { 10, 25, -53, 6, 7.0699997f, 3.9099998f, 50, 20, 100, 100, 40.0f, 60.0f, -0.57000005f, 0.07f, 0.4f },
    { 10, 25, -63, 20, 7.0699997f, 3.9099998f, 50, 20, 100, 155, 45.0f, 70.0f, -0.57000005f, 0.07f, 0.4f },
};

/* The ground impact of an impact (FxImpactSet.impact). */
static const FxImpactEntry gImpactGroundEffects[] = {
    { 60.0f, 75, 0.1f },
};


/* Spawns the effects listed for impact type `type` at `pos` on surface `hit`. */
void effect_spawnImpact(Vec3f *pos, CollisionHit *hit, s32 type, u8 arg3, s32 arg4) {
    const FxImpactSet *set;
    u8 kind;

    kind = particle_getContextEffectKind();
    if (hit == NULL) {
        return;
    }
    set = &gImpactSets[type];
    if (set->first != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            const FxSpawnRange *spawn = &gImpactFirstEffects[set->first];

            particle_emitOnRandomCircle(pos->x, pos->y, pos->z, spawn->unk0, 0, 255,
                          func_150ADA20() % (spawn->range + 1) + spawn->base, 11, type, 0.0f,
                          &payload, arg3);
        }
    }
    if (set->second != -1) {
        FxSpawnPayload payload;

        payload.hit = hit;
        payload.unk4 = arg4;
        payload.kind = kind;
        {
            const FxSpawnRange *spawn = &gImpactSecondEffects[set->second];

            particle_emitOnRandomCircle(pos->x, pos->y, pos->z, spawn->unk0, 0, 255,
                          func_150ADA20() % (spawn->range + 1) + spawn->base, 12, type, 0.0f,
                          &payload, arg3);
        }
    }
    if (set->impact != -1 && (hit->flags & 1)) {
        const FxImpactEntry *impact = &gImpactGroundEffects[set->impact];

        particle_startKindEffect(kind, impact->unk0, impact->unk8, impact->unk4, pos, &hit->tri, 150, 0, arg3, arg4);
    }
}


/* Spawn callback: launches one spray particle at (x, y, z) heading along `yaw`, with random
 * pitch and speed from the impact type's spray parameters. */
s32 effect_spawnImpactSprayParticle(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9,
                  s32 argA, s32 type, s32 argC, FxSpawnPayload *payload, u8 argE) {
    const FxSprayParams *params;
    EffectTemplate18 spawn;
    TrailMotion part;
    f32 roll;
    TrailRender style;
    f32 sinYaw;
    f32 cosYaw;
    f32 speed;
    f32 horizontal;
    f32 vertical;
    s16 pitch;
    f32 sinPitch;
    f32 cosPitch;
    u32 random;
    s32 firstFlag;
    s32 secondFlag;

    {
        s32 colorFlag;

        params = &gImpactSprays[gImpactSets[type].firstStyle];
        sinYaw = math_cosBinAngleMasked(yaw);
        cosYaw = math_cosBinAngleMasked(yaw - 0x40);
        style.unk0 = 0;
        style.setGeometryMode = 1;
        style.clearGeometryMode = 0x160600;
        style.renderMode = 1;
        style.unk10 = 16;
        style.unk14 = 128;
        style.unk18 = 32;
        style.primMode = 0;
        style.envMode = 9;
        spawn.kind = 1;
        spawn.flags = 1;
        spawn.pos.y = y;
        part.unk14 = y;
        spawn.pos.x = x;
        spawn.pos.z = z;
        part.texture = gKindSplashTextures[payload->kind];
        part.shade = 255;
        colorFlag = D_800AB404[payload->kind] ? 0x20 : 0;
        part.flags = colorFlag | 8;
    }
    roll = func_150ADA68();
    if (roll < params->chance) {
        part.flags |= 3;
        firstFlag = 6;
        secondFlag = 2;
    } else {
        firstFlag = 0;
        secondFlag = 0;
        part.flags &= ~3;
    }
    random = func_150ADA20();
    part.alpha = random % (params->unk1 + 1) + params->unk0;
    random = func_150ADA20();
    pitch = random % (params->pitchRange + 1) + params->pitchBase;
    sinPitch = math_cosBinAngleMasked(pitch);
    cosPitch = math_cosBinAngleMasked(pitch - 0x40);
    speed = func_150ADA68() * params->speedRange + params->speedBase;
    horizontal = speed * sinPitch;
    part.velocity.x = horizontal * cosYaw;
    vertical = -speed * cosPitch;
    part.velocity.y = vertical;
    part.velocity.z = horizontal * sinYaw;
    random = func_150ADA20();
    spawn.itemCount = random % (params->unk14 + 1) + params->unk10;
    random = func_150ADA20();
    spawn.life = random % (params->unk1A + 1) + params->unk18;
    part.halfWidth = func_150ADA68() * params->unk20 + params->unk1C;
    part.gravity = func_150ADA68() * params->unk28 + params->unk24;
    trail_createMotionTrail(&spawn, &part, 0, 1, 8, 0, firstFlag, secondFlag, 0, 0, 0, &style, payload->hit, argE, payload->unk4);
    return 1;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} FxEmitterPoint; // size 0x14

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ u8 pad4[0x17];
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ u8 unk20;
} FxEmitterData;

/* Effect state passed to the callbacks in the gMotionTrailUpdateProcs / D_8008A430 handler tables. */
struct FxEmitter {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ FxEmitterPoint *points;
    /* 0x98 */ FxEmitterData *data;
};

/* Lowers the effect's alpha to unk1C * 8 (capped at 255) once that is below it. */
s32 func_151AC3CC(FxEmitter *emitter) {
    FxEmitterData *data = emitter->data;
    s32 alpha = emitter->head.tmpl.life * 8;

    if (alpha >= 256) {
        alpha = 255;
    }
    if (alpha < data->alpha) {
        data->alpha = alpha;
    }
    return 1;
}

/* Spawns a burst at the emitter's current point (at the given height): randomly one of two
 * effects, scaled from the emitter's data. */
s32 func_151AC408(FxEmitter *emitter, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
    FxEmitterData *data = emitter->data;
    FxEmitterPoint *points = emitter->points;
    Vec3f pos;
    u8 kind;
    s32 life;

    pos.x = points[emitter->head.start].x;
    pos.y = height;
    pos.z = points[emitter->head.start].z;
    kind = particle_getContextEffectKind();
    if (func_150ADA20() & 1) {
        life = func_150ADA20() % 41 + 80;
        particle_spawnKindEmitter(kind, data->scale * 6.0f, data->alpha, surface, &pos, life, 1, 1, 0, emitter->head.header.owner,
                      emitter->head.header.kind);
    } else {
        particle_spawnSquash(kind, data->scale * 1.5f, data->alpha, &pos, 1, emitter->head.header.owner, emitter->head.header.kind);
    }
    data->unk20 = 4;
    return 1;
}
