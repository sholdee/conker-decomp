#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Spawns an emitter object for def with extraSize bytes of user data. lifetime -1 means untimed.
 * The spawn rate is scaled by the emitter's area: the anchor's, or that of a circle of def->radius.
 */
Emitter *beam_newAreaEmitter(EmitterDef *def, s16 lifetime, s32 extraSize, u8 arg3, s32 arg4) {
    Emitter *emitter;
    EmitterState init;
    EmitterState *state;

    memcpy(&init, def, sizeof(EmitterDef));

    init.pending = 0.0f;
    init.ground.height = 99999.9f;
    init.ground.material = 0;
    init.ground.flags = 0;
    init.ground.source = 0;
    init.ground.surface = 0;

    emitter = effect_newTimedEffect((lifetime == -1) ? 300 : lifetime, -1, 0x25, -1, (lifetime == -1) ? 0 : 1, 0x22, (extraSize + sizeof(EmitterState)), arg3, arg4);
    if (emitter != NULL) {
        state = &emitter->state;
        memcpy(state, &init, sizeof(EmitterState));

        if (state->def.flags & EMITTER_ZONED) {
            f32 z;
            f32 x;

            if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
                x = state->def.anchor->x;
                z = state->def.anchor->z;
            } else {
                x = state->def.pos.x;
                z = state->def.pos.z;
            }
            collision_selectMap(0);
            state->zone = collision_findZoneLeaf(x, z);
        } else {
            state->zone = 0;
        }

        if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
            state->area = math_getZoneArea(state->def.anchor);
        } else {
            state->area = (state->def.radius * state->def.radius) * 3.14159265f;
        }
    }
    return emitter;
}

/* Spawn hooks (EmitterDef.spawn), called once per particle at a point of the area; the last two take their
   own views of the emitter. */
static void (*gEmitterSpawnHooks[])(Emitter *, Vec3f *, f32, u8) = {
    beam_spawnSpriteParticle, func_151A9634, effect_spawnRiseEmitter, particle_spawnPointEmitter,
};
/* Update hooks (EmitterDef.update); a hook that returns 0 ends the emitter. */
static s32 (*gEmitterUpdateHooks[])(Emitter *) = { NULL };

/*
 * Per-frame update: runs the update hook (0 kills the object), then while switched on (and its zone
 * is visible) accumulates the spawn rate and calls the spawn hook once per whole particle, at a
 * random point of the area dropped to the ground.
 */
void beam_updateAreaEmitter(Emitter *emitter) {
    EmitterState *state;
    void (*pick)(Emitter *, Vec3f *, f32 *, f32 *);
    Vec3f pos;
    f32 bottom;
    f32 top;
    u8 noGround;

    state = &emitter->state;

    if (state->def.update != -1) {
        if (gEmitterUpdateHooks[state->def.update](emitter) == 0) {
            emitter->base.header.state = -1;
            return;
        }
    }

    if ((state->def.flags & EMITTER_ZONED) && (state->zone != 0)) {
        if (math_isZoneHiddenFromViews(state->zone) != 0) {
            return;
        }
    }
    if (!(state->def.flags & EMITTER_ON)) {
        return;
    }

    state->pending += (state->def.rate + func_150ADA68() * state->def.rateRange) * gFrameDelta * state->area;

    if (state->pending > 1.0f) {
        if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
            pick = beam_pickAnchorPoint;
        } else {
            pick = beam_pickRadiusPoint;
        }
        do {
            pick(emitter, &pos, &top, &bottom);
            if (state->def.flags & EMITTER_NO_PROBE) {
                pos.y = bottom;
                noGround = TRUE;
            } else if (collision_probeFloorSurfABSet0Cached(&pos, 0, bottom, &state->ground) != 0) {
                pos.y = state->ground.height; /* noGround keeps its previous (at first unset) value */
            } else {
                pos.y = bottom;
                noGround = TRUE;
            }
            if (state->def.spawn != -1) {
                gEmitterSpawnHooks[state->def.spawn](emitter, &pos, top, noGround);
            }
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
    }
}

/* Picks a random point of the anchor object's area; *top is its height. */
void beam_pickAnchorPoint(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom) {
    math_randomZoneFootprintPoint(emitter->state.def.anchor, &pos->x, &pos->z, top, bottom);
    pos->y = *top;
}

/* Picks a random point within def.radius of def.pos; *top is def.pos.y, *bottom def.bottom. */
void beam_pickRadiusPoint(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom) {
    EmitterDef *def;

    def = &emitter->state.def;
    math_dirFromBinAngleXZ(func_150ADA20() & 0xFF, def->radius * func_150ADA68(), &pos->x, &pos->z);
    pos->x += def->pos.x;
    pos->z += def->pos.z;
    pos->y = def->pos.y;
    *top = def->pos.y;
    *bottom = def->bottom;
}

/* Message handler: type 1 emitters are switched on and off by messages. */
void beam_areaEmitterMessage(Emitter *emitter, u8 *msg, u8 msgType) {
    if (emitter->state.def.type == 1) {
        beam_switchEmitterMessage(emitter, msg, msgType);
    }
}

/* Zone handlers by kind (TriggerZone.data1); declared without a prototype as two take their own view of
   the zone. */
static void (*gEmitterZoneHandlers[])() = {
    beam_spawnZoneEmitter, beam_spawnZoneEmitter, beam_spawnZoneEmitter, beam_spawnZoneEmitter, beam_spawnZoneEmitter, beam_spawnZoneEmitter, effect_spawnRiseZoneEmitter,
    func_15012020,
};

/* Zone handler (D_80082ECC): marks the zone and runs the handler for its kind (data1, 0 to 7). */
s32 beam_runZoneHandler(TriggerZone *obj) {
    s32 kind;

    obj->flags |= 4;
    kind = obj->data1;
    if (kind < 0 || kind >= 8) {
        return 1;
    }
    if (gEmitterZoneHandlers[kind] != NULL) {
        gEmitterZoneHandlers[kind](obj, kind);
    }
    return 1;
}

/*
 * Kind 0-5 zone handler: an emitter anchored to the zone obj, with message id `kind`; odd kinds
 * spawn faster.
 */
void beam_spawnZoneEmitter(TriggerZone *obj, s32 kind) {
    EmitterDef def;
    // matching: the id byte sits 4 bytes below this unused slot, in frame padding no declared
    // local reaches (a u8 local lands at sp+0x2F or grows the frame)
    s32 idSlot;
    f32 odd;
    Emitter *emitter;

    def.flags = EMITTER_ANCHORED;
    def.anchor = obj;
    odd = kind & 1;
    if (odd) {
        def.rate = 1.4e-6f;
    } else {
        def.rate = 4e-7f;
    }

    if (odd) {
        def.rateRange = 8e-7f;
    } else {
        def.rateRange = 6e-7f;
    }

    def.pos.x = 0.0f;
    def.pos.y = 0.0f;
    def.pos.z = 0.0f;
    def.bottom = 0.0f;
    def.radius = 0.0f;
    def.type = 1;
    def.update = -1;
    def.spawn = 0;
    ((u8 *)&idSlot)[-4] = kind;

    emitter = beam_newAreaEmitter(&def, -1, 1, 0xFF, 0);
    if (emitter != NULL) {
        memcpy(emitter->data, (u8 *)&idSlot - 4, 1);
    }
}

typedef struct {
    s16 ids[3];
} SpriteChoices;


/* Spawn descriptor passed to particle_spawnSurfaceQuadSprite. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} SpriteSpawn;

/* Spawn hook 0: a sprite of random size (50 to 100) using one of three images, at pos. */
void beam_spawnSpriteParticle(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround) {
    SpriteSpawn spawn;
    f32 size;
    SpriteChoices choices = { 15, 16, 17 };
    u32 pick;
    s32 rand2;
    s32 rand1;

    size = func_150ADA68() * 50.0f + 50.0f;
    pick = func_150ADA20() % 3;

    spawn.unk6 = choices.ids[pick];
    spawn.unk7 = 0;
    spawn.unk16 = 0;
    spawn.unk17 = 7;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.flags = 0x1701;
    spawn.lifetime = 60;
    spawn.unk10 = 0xA0;
    spawn.unk11 = 0xFF;
    spawn.unk12 = 0;
    spawn.unk13 = 0;
    spawn.unk14 = 0;
    spawn.unk15 = 0xFF;
    spawn.unk18 = 0x3B0002;

    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    particle_spawnSurfaceQuadSprite(&spawn, 1, 0, &emitter->state.ground.tri, pos->x, pos->y, pos->z,
                  size, size, rand1 & 0xFF,
                  (rand2 & 1) + (func_150ADA20() & 1), 3, 0xFF, 0,
                  emitter->base.header.owner, emitter->base.header.kind);
}

/* Type 1 message handler: 0x17 switches the emitter on, 0x18 off, when the message carries its id. */
void beam_switchEmitterMessage(Emitter *emitter, u8 *msg, u8 msgType) {
    EmitterState *state;

    if (msgType == 0x17) {
        state = &emitter->state;
        // matching: the goto keeps IDO from sharing the 0x18 branch's return
        if (*msg != emitter->data[0]) {
            goto ret;
        }
        state->def.flags |= EMITTER_ON;
    ret:
        return;
    } else if (msgType == 0x18) {
        state = &emitter->state;
        if (*msg == emitter->data[0]) {
            state->def.flags &= ~EMITTER_ON;
        }
    }
}

/* One of the nine emitter presets of beam_spawnEmitterPreset. */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ u8 unk8;              /* unk8..unkC go to light_createJitterAt */
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ const ParticleSettings *settings;
} EmitterPreset;

/* The settings of the emitter presets (referenced only through gEmitterPresets). */
static const ParticleSettings D_800A8DF0 = { 1.0f, 1.0f, 0.0f, 0.0f, { 50, 0 }, { 50, 0 }, { 255, 0 }, 1, 255, 1.0f, 0.0f, 0 };
static const ParticleSettings D_800A8E1C = { 0.80100006f, 0.901f, 0.906f, 0.702f, { 16, 50 }, { 50, 50 }, { 80, 120 }, 20, 12, 0.1f, 0.29900002f, 0 };
static const ParticleSettings D_800A8E48 = { 0.499f, 0.79700005f, 0.80600005f, 0.508f, { 10, 50 }, { 50, 25 }, { 80, 150 }, 16, 15, 0.10700001f, 0.252f, 0 };
static const ParticleSettings D_800A8E74 = { 0.814f, 0.69500005f, 0.32200003f, 0.54200006f, { 10, 69 }, { 60, 25 }, { 80, 120 }, 16, 15, 0.08400001f, 0.29500002f, 0 };
static const ParticleSettings D_800A8EA0 = { 1.003f, 1.291f, 1.3050001f, 1.1680001f, { 10, 50 }, { 34, 20 }, { 80, 120 }, 20, 12, 0.279f, 0.30100003f, 0 };
static const ParticleSettings D_800A8ECC = { 1.322f, 1.7500001f, 1.238f, 1.108f, { 4, 91 }, { 60, 40 }, { 100, 100 }, 30, 8, 0.106000006f, 0.305f, 0 };
static const ParticleSettings D_800A8EF8 = { 1.141f, 1.087f, 1.5400001f, 0.688f, { 46, 74 }, { 35, 25 }, { 100, 100 }, 30, 8, 0.037f, 0.09900001f, 0 };
static const ParticleSettings D_800A8F24 = { 0.41700003f, 0.48900002f, 0.38000003f, 0.344f, { 30, 120 }, { 49, 25 }, { 100, 100 }, 15, 17, 0.09f, 0.05f, 0 };

static EmitterPreset gEmitterPresets[] = {
    { 0.31f, 0.33900002f, 255, 200, 100, 6, 12, &D_800A8DF0 },
    { 9.867606e-06f, 1.0790705e-05f, 255, 200, 100, 6, 12, &D_800A8E1C },
    { 9.867606e-06f, 1.0790705e-05f, 255, 200, 100, 6, 12, &D_800A8E1C },
    { 3.573e-05f, 2.592e-05f, 255, 200, 100, 6, 12, &D_800A8E48 },
    { 2.492e-05f, 2.0e-05f, 255, 194, 94, 6, 13, &D_800A8E74 },
    { 1.456e-05f, 1.044e-05f, 255, 194, 94, 6, 13, &D_800A8EA0 },
    { 3.7e-07f, 7.4e-07f, 255, 194, 94, 6, 13, &D_800A8ECC },
    { 1.27e-06f, 9.1e-07f, 255, 194, 94, 6, 13, &D_800A8EF8 },
    { 7.143e-05f, 5.639e-05f, 255, 194, 94, 6, 13, &D_800A8F24 },
};

/*
 * Spawns particle emitter preset `preset` (0-8) on obj, or at pos when given. options:
 * 1 EMITTER_ZONED, 2 EMITTER_NO_PROBE, 4 also call light_createJitterAt there, 8 and 0x10 set settings
 * flags 1 and 2. lifetime -1 means untimed.
 */
void beam_spawnEmitterPreset(u8 options, u8 preset, TriggerZone *obj, Vec3f *pos,
                   f32 bottom, f32 radius, s16 lifetime, u8 arg7, s32 arg8) {
    EmitterDef def;
    Emitter *emitter;

    if (preset >= 9) {
        return;
    }

    def.flags = EMITTER_ON | ((obj != NULL) ? EMITTER_ANCHORED : 0) | ((options & 1) ? EMITTER_ZONED : 0) |
                ((options & 2) ? EMITTER_NO_PROBE : 0);
    def.anchor = obj;
    def.rate = gEmitterPresets[preset].rate;
    def.rateRange = gEmitterPresets[preset].rateRange;
    if (pos != NULL) {
        def.pos = *pos;
    } else {
        def.pos.x = 0.0f;
        def.pos.y = 0.0f;
        def.pos.z = 0.0f;
    }
    def.bottom = bottom;
    def.radius = radius;
    def.type = 2;
    def.update = -1;
    def.spawn = 1;

    emitter = beam_newAreaEmitter(&def, lifetime, sizeof(ParticleSettings), arg7, arg8);
    if (emitter != NULL) {
        ParticleSettings *settings;

        settings = (ParticleSettings *)emitter->data;
        memcpy(settings, gEmitterPresets[preset].settings, sizeof(ParticleSettings));
        if (options & 8) {
            settings->flags |= 1;
        }
        if (options & 0x10) {
            settings->flags |= 2;
        }
    }

    if (options & 4) {
        LightEffectHeader spawn;
        Vec3i spawnPos;

        spawn.flags = ((lifetime == -1) ? 0 : 1) | 2;
        spawn.update = 2;
        spawn.lifetime = (lifetime == -1) ? 300 : lifetime;
        spawn.lightType = gEmitterPresets[preset].unkB;

        if (pos != NULL) {
            spawnPos.x = pos->x;
            spawnPos.y = pos->y;
            spawnPos.z = pos->z;
        } else {
            spawnPos.x = obj->x;
            spawnPos.y = obj->y;
            spawnPos.z = obj->z;
        }

        light_createJitterAt(&spawn, &spawnPos, gEmitterPresets[preset].unk8, gEmitterPresets[preset].unk9,
                      gEmitterPresets[preset].unkA, 0xFF, 0, 0, gEmitterPresets[preset].unkC,
                      0xFF, 1);
    }
}

// matching: this file was compiled with s32 for the u8, s16 and pointer parameters of the definition
// (game/Particle/GroundCluster/groundcluster.c), so its call does not narrow them. The definition is (Vec3f *pos, Struct1514B8E4Size *scale,
// s16 life, u8 arg3, Vec3f *velocity, f32 gravity, f32 drag, f32 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB,
// u8 flags, s16 argD, s16 argE, s32 argF, u8 arg10, s32 arg11), returning void *.
u8 *particle_spawnJitterParticle(Vec3f *, f32 *, s16, s32, s32, f32, f32, f32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawn hook 1: launches one particle at pos with values drawn from the emitter's ParticleSettings. */
void func_151A9634(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround) {
    f32 vec[2];
    s32 choice;
    JitterRelaunch extra;
    s32 kind;
    s32 flags;
    u8 *particle;
    ParticleSettings *settings;
    u32 rand2;
    u32 rand1;

    settings = (ParticleSettings *)emitter->data;
    vec[0] = func_150ADA68() * settings->vec0Range + settings->vec0Base;
    vec[1] = func_150ADA68() * settings->vec1Range + settings->vec1Base;
    extra.gravity = func_150ADA68() * settings->unk20Range + settings->unk20Base;
    extra.lifetime = func_150ADA20() % (settings->relaunchLifetime.range + 1) + settings->relaunchLifetime.base;
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    if (settings->flags & 1) {
        kind = 0x71;
    } else {
        if (func_150ADA20() & 1) {
            choice = 0x13;
        } else {
            choice = 0x14;
        }
        kind = choice; // matching: the extra copy shapes IDO's register use
    }
    flags = (settings->flags & 2) ? 0 : 2;
    particle = particle_spawnJitterParticle(pos, vec,
        (s16)(rand1 % (settings->life.range + 1) + settings->life.base),
        (rand2 % (settings->unk18.range + 1) + settings->unk18.base) & 0xFF,
        0, 0.0f, 1.0f, 1.0f, 0x21, 0x23, 2, kind, flags,
        settings->unk1C, settings->unk1E, 8, emitter->base.header.owner, emitter->base.header.kind);
    if (particle != NULL) {
        memcpy(particle + 0x160, &extra, sizeof(extra));
    }
}

/* The spawn functions; `data` is passed through from beam_spawnAtGroundPoints (particle_spawnKindEmitterAbove reads a u8 kind there). */
static void (*gGroundSpawnFuncs[])(Vec3f *, CollisionHit *, void *, u8, s32) = { particle_spawnKindEmitterAbove, effect_spawnScatteredParticles };

/*
 * Calls spawn function gGroundSpawnFuncs[spawnFunc] at `count` random points within radius of center
 * that have ground above minY (the spawn point is on the ground). hit is the probe result buffer;
 * NULL uses a local one.
 */
void beam_spawnAtGroundPoints(Vec3f *center, f32 minY, f32 radius, CollisionHit *hit,
                   s32 count, u8 spawnFunc, void *arg6, u8 arg7, s32 arg8) {
    CollisionHit localHit;
    Vec3f probe;
    Vec3f spawnPos;
    f32 angle;
    f32 dist;

    if (hit == NULL) {
        hit = &localHit;
        localHit.height = 99999.9f;
        localHit.material = 0;
        localHit.flags = 7;
        localHit.source = 0;
        localHit.surface = 0;
    }

    probe.y = center->y;
    for (; count > 0; count--) {
        angle = func_150ADA68();
        dist = func_150ADA68();
        math_dirFromAngleXZ((angle + angle) * 3.14159265f, dist * radius, &probe.x, &probe.z);
        probe.x += center->x;
        probe.z += center->z;

        if (collision_probeFloorSurfABSet0Cached(&probe, 0, minY, hit) != 0) {
            spawnPos.x = probe.x;
            spawnPos.y = hit->height;
            spawnPos.z = probe.z;
            gGroundSpawnFuncs[spawnFunc](&spawnPos, hit, arg6, arg7, arg8);
        }
    }
}
