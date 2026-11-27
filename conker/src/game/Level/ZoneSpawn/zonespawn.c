#include <ultra64.h>

// This file's .bss, defined before the headers that declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init/Sched/SchedInit/schedinit.c).
u16 *gScreenCopy; // RGBA16 screen copy, gScreenWidth x gScreenHeight (game/Screen/FrameImage/frameimage)
u8 gScreenCopyAlpha;
u8 gFrameEchoPass;

#include <math.h>
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/rope.h"
#include "conker/script.h"
#include "conker/sight.h"
#include "conker/sound.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/zone.h"

/* Spawn handler of each zone type (TriggerZone.kind >> 2, see enum ZoneType); NULL spawns nothing. */
static s32 (*gZoneSpawnHandlers[26])(TriggerZone *) = {
    /*  0 */ NULL,
    /*  1 */ NULL,
    /*  2 */ NULL,
    /*  3 */ level_spawnNeutralZone,
    /*  4 */ NULL,
    /*  5 */ NULL,
    /*  6 */ NULL,
    /*  7 */ NULL,
    /*  8 */ NULL,
    /*  9 */ NULL,
    /* 10 */ NULL,
    /* 11 */ NULL,
    /* 12 */ level_spawnEffectZone,
    /* 13 */ NULL,
    /* 14 */ NULL,
    /* 15 */ NULL,
    /* 16 */ NULL,
    /* 17 */ level_spawnPointGlowZone,
    /* 18 */ level_spawnFireZone,
    /* 19 */ level_spawnFlackZone,
    /* 20 */ level_spawnBulletZone,
    /* 21 */ level_spawnMissileZone,
    /* 22 */ level_spawnDiscoBallZone,
    /* 23 */ level_spawnLaserTubeZone,
    /* 24 */ NULL,
    /* 25 */ NULL,
};




void level_resetScreenCopy(void) {
    gScreenCopy = NULL;
    gScreenCopyAlpha = 0;
    gFrameEchoPass = 0;
}

/** Runs the spawn handler of every level object, chosen by the object's type. */
void level_spawnZones(void) {
    u32 i;

    gDustRegions = NULL;
    gDustRegionsArea = 0.0f;
    // matching: the label ends the block, so IDO cannot hoist the gZoneCount load above the stores
start:
    for (i = 0; i < gZoneCount; i++) {
        u8 type = ZONE_TYPE(&zones[i]);
        s32 (*handler)(TriggerZone *) = gZoneSpawnHandlers[type];

        if (handler != NULL) {
            handler(&zones[i]);
        }
    }
}

/* GlowOrbit as level_spawnPointGlowZone fills it: the object whose t and v rotations GlowOrbit.angles
 * points at (its first field, so object = angles). */
typedef struct {
    /* 0x00 */ f32 distance;
    /* 0x04 */ f32 spread;
    /* 0x08 */ WorldObject *object;
    /* 0x0C */ f32 pivot[3];
} Payload18_150130B4;

/** Spawns the particle emitter a level object describes; data2 selects the kind of emitter. */
s32 level_spawnPointGlowZone(TriggerZone *obj) {
    GlowShape emit;
    ParticleDef spawn;
    s32 kind;
    s32 index;
    u8 onGround;

    obj->flags |= 4;
    onGround = (obj->data3 >> 24) & 0xFF;
    kind = obj->data2;
    {
        MtxF mtx;

        spawn.pos.x = obj->x;
        spawn.pos.y = obj->y;
        spawn.pos.z = obj->z;
        math_mtxRotateRPYF(mtx, obj->pitch, obj->yaw, 0.0f);
        math_transformPoint(mtx, 0.0f, 0.0f, -500.0f, &spawn.rot.x, &spawn.rot.y, &spawn.rot.z);
        spawn.rot.x += spawn.pos.x;
        spawn.rot.y += spawn.pos.y;
        spawn.rot.z += spawn.pos.z;
    }
    emit.height1 = obj->xSize;
    emit.width1 = emit.height1;
    emit.height0 = obj->ySize;
    emit.width0 = emit.height0;
    emit.alpha1 = ((obj->data3 >> 8) & 0xFF) * 0.003921568859f;
    emit.alpha0 = (obj->data3 & 0xFF) * 0.003921568859f;
    index = (obj->data1 >> 24) & 0xFF;
    spawn.texture = gPointGlowTextures[index];
    spawn.renderModes = gPointGlowRenderModes[index] + 3;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = obj->data1 & 0xFF;
    spawn.rgba[1] = (obj->data1 >> 8) & 0xFF;
    spawn.rgba[2] = (obj->data1 >> 16) & 0xFF;
    spawn.rgba[3] = 0xFF;
    spawn.width = 0.0f;
    spawn.height = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.colorLevel = 0xFF;
    spawn.alpha = 0xFF;
    spawn.primMode = 0;
    spawn.envMode = 6;
    spawn.type = 3;
    emit.visibility[0] = 0;
    emit.visibility[1] = 0;
    emit.visibility[2] = 0;
    emit.visibility[3] = 0;
    emit.scale = 1.0f;
    emit.offset1 = 0.4099999964f;
    emit.offset0 = 0.200000003f;
    emit.fadeRate = 9.999999975e-07f;
    if (onGround) {
        collision_selectMap(0);
        spawn.zone = collision_findZoneLeaf(spawn.pos.x, spawn.pos.z);
    } else {
        spawn.zone = 0;
        onGround = FALSE; // matching: redundant store, but IDO's allocation needs it
    }
    emit.type = 0;
    spawn.flags = 0x2006 | (((obj->data3 >> 16) & 0xFF) ? 0x10 : 0) | (onGround ? 0x01000000 : 0) | 0x40000 | 0x80000 | 0x10000 | 0x400000;
    switch (kind) {
    case 8: {
        GlowPulse pulse;
        GlowObj *particle;

        pulse.base = 0.9750000238f;
        pulse.amplitude = 0.3249999881f;
        pulse.phase = 0.0f;
        pulse.speed = 0.05959999934f;
        particle = flare_spawnGlow(&emit, 0x70, &spawn, 0, 0x29, 0, 0, -1, 0xFF, 1);
        if (particle != NULL) {
            memcpy(&particle->params.anim.pulse, &pulse, sizeof(pulse));
        }
        break;
    }
    case 1:
    case 2:
    case 3:
    case 4: {
        void *unused; // matching: the dead store of the result steers as1's scheduling

        emit.type = gPointGlowEmitterTypes[kind - 1];
        unused = flare_spawnGlow(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 9:
    case 10:
    case 11:
    case 12: {
        void *unused; // matching: as above

        emit.type = gPointGlowEmitterTypes[kind - 5];
        unused = flare_spawnGlow(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 5:
    case 6:
    case 7: {
        const s32 ids[3] = { 12, 13, 14 };

        emit.type = ids[kind - 5];
        flare_spawnGlow(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    }
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18: {
        GlowObj *particle;
        Payload18_150130B4 orbit;
        u8 ids[6] = { 2, 3, 4, 5, 6, 7 };

        orbit.object = gfx_findObjectById(ids[kind - 13]);
        if (orbit.object == NULL) {
            return 1;
        }
        orbit.distance = -80.0f;
        orbit.spread = fabsf(-0.01250000019f);
        orbit.pivot[0] = orbit.object->x;
        orbit.pivot[1] = orbit.object->y;
        orbit.pivot[2] = orbit.object->z;
        emit.type = 0;
        particle = flare_spawnGlow(&emit, 0x78, &spawn, 0x25, 0, 0, 0, -1, 0xFF, 1);
        if (particle != NULL) {
            memcpy(&particle->params.anim.orbit, &orbit, sizeof(orbit));
        }
        break;
    }
    case 19:
        emit.type = 0x11;
        spawn.flags &= ~2;
        flare_spawnGlow(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        return 1;
    default:
        flare_spawnGlow(&emit, 0x60, &spawn, 0, 0, 0, 0, -1, 0xFF, 1);
        break;
    }
    return 1;
}

/* Particle texture and render modes per glow kind (TriggerZone.data1 >> 24), and the emitter type for
 * spawn kinds 1-4 and 9-12. */
const s32 gPointGlowTextures[5] = { 3, 0x15, 0x68, 0x69, 0x7E };
const s32 gPointGlowRenderModes[5] = { 0x3400, 0x3400, 0x2200, 0x3400, 0x3400 };
const s32 gPointGlowEmitterTypes[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

/* Handlers of the effect zones (ZONE_TYPE_EFFECT), chosen by TriggerZone.subTypeData. */
static s32 (*gEffectZoneSpawnHandlers[11])(TriggerZone *zone) = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    level_spawnTrailZone,
    level_spawnZoneBeam,
    NULL,
    sight_placeBeam,
    level_spawnHandlerZone,
    level_spawnStreamZone,
};

/** Runs the object's handler from gEffectZoneSpawnHandlers, chosen by subTypeData. */
s32 level_spawnEffectZone(TriggerZone *obj) {
    u8 index = obj->subTypeData;
    s32 (*handler)(TriggerZone *) = gEffectZoneSpawnHandlers[index];

    if (handler != NULL) {
        handler(obj);
    }
    return 1;
}

s32 level_spawnTrailZone(TriggerZone *obj) {
    obj->flags |= 4;
    trail_spawnLevelEffect(obj);
    return 1;
}

typedef struct Game40490BeamPacket {
    f32 position[3];
    f32 direction[3];
    f32 height;
    f32 width;
    f32 rotation[2];
    f32 zero;
    f32 field2C;
    f32 field30;
} Game40490BeamPacket;

/** Spawns a beam effect of twice the object's sizeY, pointing along its rotation. */
s32 level_spawnZoneBeam(TriggerZone *obj) {
    TimedEffect *effect;
    Game40490BeamPacket packet;
    MtxF matrix;

    if (obj->ySize != 0) {
        f32 inverse;
        s16 height;

        packet.height = 2.0f * obj->ySize;
        inverse = 1.0f / packet.height;
        packet.width = obj->xSize;
        packet.rotation[0] = obj->pitch;
        packet.rotation[1] = obj->yaw;
        packet.zero = 0.0f;
        packet.field2C = 0.2001999915f;
        packet.field30 = 0.04999999702f;
        packet.position[0] = obj->x;
        packet.position[1] = obj->y;
        packet.position[2] = obj->z;
        math_mtxRotateRPYF(matrix, obj->pitch, obj->yaw, 0.0f);
        math_transformPoint(matrix, 0.0f, packet.height, 0.0f,
                      &packet.direction[0], &packet.direction[1], &packet.direction[2]);
        packet.direction[0] *= inverse;
        packet.direction[1] *= inverse;
        packet.direction[2] *= inverse;
        effect = effect_newTimedEffect(300, -1, 0x19, -1, 0, 0x17, sizeof(packet), 0xFF, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        }
        height = obj->ySize;
        sound_addDynamicSound(0x67C, obj->x, obj->y + height, obj->z,
                      12000, height * 2, height / 2, sound_stopWhenSilentCallback, 0, 0, 8, 0);
    }
    return 1;
}

/* Handlers of the neutral zones (ZONE_TYPE_NEUTRAL), chosen by TriggerZone.subTypeData. Declared without a
 * prototype: level_spawnZoneAreaEffect takes an extra argument and level_addDustRegionZone none. */
static s32 (*gNeutralZoneSpawnHandlers[23])() = {
    NULL,
    NULL,
    NULL,
    level_spawnZoneAreaEffect,
    beam_runZoneHandler,
    level_spawnRopeZone,
    level_spawnBoltRainZone,
    NULL,
    level_spawnAreaParticlesZone,
    level_addDustRegionZone,
    level_spawnShapeEmitterZone,
    NULL,
    NULL,
    func_15013D38,
    level_spawnChannelParticlesZone,
    NULL,
    level_registerEmissionSourceZone,
    level_spawnRandomPointZone,
    level_spawnAreaEmitterZone,
    level_registerSprayAreaZone,
    level_spawnSparkRegionZone,
    NULL,
    level_spawnZoneSpray,
};

/** Runs the object's handler from gNeutralZoneSpawnHandlers, chosen by subTypeData. */
s32 level_spawnNeutralZone(TriggerZone *obj) {
    u8 index = obj->subTypeData;
    s32 (*handler)() = gNeutralZoneSpawnHandlers[index];

    if (handler != NULL) {
        handler(obj);
    }
    return 1;
}

/** Spawns the area effect described by entry data1 of gAreaEffectDefs over the object's footprint. */
// matching: index lives in the (unused) second argument register
s32 level_spawnZoneAreaEffect(TriggerZone *obj, s32 index) {
    RegionEmitterState data;
    const AreaEffectDef *entry;
    TimedEffect *effect;

    index = obj->data1;
    obj->flags |= 4;
    data.region = obj;
    data.pending = 0.0f;

    switch (ZONE_AREA_TYPE(obj)) {
    default:
        data.rate = 1.0f;
        data.maxHeight = 1000.0f;
        break;
    case ZONE_AREA_BOX:
        /* box: area of the footprint */
        data.rate = (obj->xSize * obj->zSize) * 4.0f;
        data.maxHeight = obj->y + obj->ySize;
        break;
    case ZONE_AREA_CYLINDER:
    case ZONE_AREA_SPHERE:
        /* cylinder: area of the circle */
        data.rate = (obj->xSize * obj->xSize) * 3.141592741f;
        data.maxHeight = obj->y + obj->ySize;
        break;
    }

    entry = &gAreaEffectDefs[index];
    data.xScaleBase = entry->xScaleBase;
    data.xScaleRange = entry->xScaleRange;
    data.yScaleBase = entry->yScaleBase;
    data.yScaleRange = entry->yScaleRange;
    data.scaleBase = entry->scaleBase;
    data.scaleRange = entry->scaleRange;
    data.ground.material = 0;
    data.ground.flags = 0;
    data.ground.source = 0;
    data.ground.surface = 0;
    data.lifeBase = entry->lifeBase;
    data.lifeRange = entry->lifeRange;
    data.shrinkRateBase = entry->shrinkRateBase;
    data.shrinkRateRange = entry->shrinkRateRange;
    data.growRateBase = entry->growRateBase;
    data.growRateRange = entry->growRateRange;
    data.scale = entry->density;
    data.useGround = entry->useGround;
    data.alphaBase = entry->alphaBase;
    data.alphaRange = entry->alphaRange;
    data.ground.height = -10000.0f;

    if (entry->useZone != 0) {
        collision_selectMap(0);
        data.zone = collision_findZoneLeaf(obj->x, obj->z);
    } else {
        data.zone = NULL;
    }

    if (entry->hasSound != 0) {
        data.soundHandle = sound_addDynamicSound(0x4D, obj->x, obj->y, obj->z, 24000, 1000, 300, 0, NULL, 0, 0, 0);
    } else {
        data.soundHandle = 0;
    }

    effect = effect_newTimedEffectNoDraw(300, -1, 6, 0, 10, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/* Area effects of level_spawnAreaParticlesZone, chosen by TriggerZone.data1, with the density data2 / 1000. */
static void (*gZoneAreaParticleHandlers[6])(TriggerZone *, f32) = {
    particle_spawnAreaParticles, particle_spawnAreaSprites, particle_spawnAreaQuads, func_1500D3AC, func_1500D8C8, particle_spawnAreaLargeSprites,
};

/** Runs the object's handler from gZoneAreaParticleHandlers (chosen by data1) with data2 / 1000 as argument. */
s32 level_spawnAreaParticlesZone(TriggerZone *obj) {
    s32 index;
    f32 value;
    void (*handler)(TriggerZone *, f32);

    obj->flags |= 4;
    index = obj->data1;

    if ((gTaskDoneBits[0x11] & 4) && (level == LEVEL_W2_HARBOUR)) { // low byte of gTaskDoneBits[0x10]
        return 1;
    }

    if (gCutsceneStates[0] == 1) {
        if ((gCutsceneIds[0] == 15) || (gCutsceneIds[0] == 16) || (gCutsceneIds[0] == 17)) {
            return 1;
        }
    }

    if (index >= 6) {
        return 1;
    }

    handler = gZoneAreaParticleHandlers[index];
    if (handler != NULL) {
        value = obj->data2 * 0.001000000047f;
        handler(obj, value);
    }

    return 1;
}

/** Calls func_151BE850 at the object's position, with a count of data1 (at least 1). */
s32 func_15013D38(TriggerZone *obj) {
    Vec3f pos;
    s32 count;

    obj->flags |= 4;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;

    count = obj->data1;
    func_151BE850(&pos, obj->yaw, count ? count : 1, obj->data2, 1, 0xFF, 1);
    return 1;
}

/** Emits particles from the object's position, scaled by its sizeX and sizeY. */
s32 level_spawnChannelParticlesZone(TriggerZone *obj) {
    ParticleEmitterDesc params;
    f32 scaleX;
    f32 scaleY;

    obj->flags |= 4;

    scaleX = obj->xSize * 0.05000000075f;
    scaleY = obj->ySize * 0.01999999955f;

    params.maxC = 0x6231; params.minC = 0x1A4D; // matching: on one line, as1 keeps these stores in this order
    params.color[0] = 0;
    params.color[1] = 0;
    params.color[2] = 0;
    params.color[3] = 0xFF;
    params.flags = 0;
    params.colorLevel = 0xFF;
    params.extraSize = 0;
    params.updateFn1 = 0;
    params.expireFn = 0;
    params.effect = 0;
    params.maxHeight = 99999.89844f;

    params.pos.x = obj->x;
    params.pos.y = obj->y;
    params.pos.z = obj->z;

    params.life = 300;
    params.scale = 0.0f;
    params.shrinkRate = 0.04500000179f;
    params.growRate = 0.6999999881f;

    params.alpha = (math_random() % 56U) + 200;

    params.maxA = 22.60000038f * scaleX;
    params.minA = 13.78600025f * scaleX;
    params.maxB = 57.96900177f * scaleY; params.minB = 42.7140007f * scaleY; // matching: on one line, as1 keeps these stores in this order
    params.altB = 118.7630005f * scaleY;

    particle_spawnChannelParticle(&params, 10, 1, obj->data1 ? 2 : 1, 0xFF, 1);
    return 1;
}

s32 level_spawnStreamZone(TriggerZone *obj) {
    trail_createStreamTask(obj, 0xFF, 1);
    return 1;
}

/* Handlers of level_spawnHandlerZone, chosen by the low byte of TriggerZone.data1. */
static s32 (*gHandlerZoneSpawns[12])(TriggerZone *) = {
    level_registerFlamethrowerZone,
    NULL,
    NULL,
    func_1501407C,
    level_spawnZoneEffect,
    level_spawnBoltZone,
    NULL,
    NULL,
    level_spawnBoltZone,
    level_spawnStageZone,
    NULL,
    NULL,
};

/** Runs the object's handler from gHandlerZoneSpawns, chosen by the low byte of data1. */
s32 level_spawnHandlerZone(TriggerZone *obj) {
    u8 index = obj->data1;
    s32 (*handler)(TriggerZone *) = gHandlerZoneSpawns[index];

    if (handler != NULL) {
        handler(obj);
    }
    return 1;
}

/** Registers the object in slot data2 (0-5) of gFlamethrowerZones. */
s32 level_registerFlamethrowerZone(TriggerZone *obj) {
    s32 slot = obj->data2;

    if (slot < 0) {
        return 1;
    }
    if (slot >= 6) {
        return 1;
    }
    gFlamethrowerZones[slot] = obj;
    return 1;
}

/** Registers the object as gEmissionSourceZones[data1] (data1 0 or 1). */
s32 level_registerEmissionSourceZone(TriggerZone *obj) {
    s32 which = obj->data1;

    obj->flags |= 4;
    if (which == 0) {
        gEmissionSourceZones[0] = obj;
    } else if (which == 1) {
        gEmissionSourceZones[1] = obj;
    }
    return 1;
}

s32 func_1501407C(TriggerZone *obj) {
    D_800D9860.flags = 0;
    return 1;
}

/* Declared s32 but returns no value; giving it a return value or void changes the code. */
s32 level_spawnRandomPointZone(TriggerZone *obj) {
    Game1FBB80Effect data;
    TimedEffect *effect;

    obj->flags |= 4;
    data.shape = obj;
    data.rate = math_getZoneArea(obj);
    data.accumulator = 0.0f;

    collision_selectMap(0);
    data.zone = collision_findZoneLeaf(obj->x, obj->z);
    data.floor.flags = 0;

    effect = effect_newTimedEffect(300, -1, 0x21, -1, 0, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/** Calls beam_spawnEmitterPreset for the object with option bits taken from data1 (bit 1 inverted). */
s32 level_spawnFireZone(TriggerZone *obj) {
    s32 flags;
    s32 bit8;
    s32 bit4;
    s32 bit2;
    s32 bit1;
    s32 bit16;

    flags = obj->data1;
    bit8 = (flags & 8) ? 8 : 0;
    bit4 = (flags & 4) ? 4 : 0;
    bit2 = (flags & 2) ? 0 : 2;
    bit1 = (flags & 1) ? 1 : 0;
    bit16 = (flags & 0x10) ? 0x10 : 0;

    beam_spawnEmitterPreset(bit16 | bit1 | bit2 | bit4 | bit8, obj->data2, obj, 0, -10000.0f, 100.0f, -1, 0xFF, 1);
    return 1;
}

s32 level_spawnAreaEmitterZone(TriggerZone *obj) {
    AreaEmitterData data;
    TimedEffect *effect;

    obj->flags |= 4;
    data.flags = 1;
    data.area = obj;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffect(300, -1, 0x26, -1, 0, 0x24, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Registers the object in slot (low byte of data1, 0-2) of gSprayAreas. */
s32 level_registerSprayAreaZone(TriggerZone *obj) {
    s32 slot;

    slot = (u8)obj->data1;
    obj->flags |= 4;
    if ((slot < 0) || (slot >= 3)) {
        return 1;
    }
    gSprayAreas[slot] = obj;
    return 1;
}

s32 level_spawnFlackZone(TriggerZone *obj) {
    BurstSpawner data;
    TimedEffect *effect;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 scale;

    obj->flags |= 4;
    if (gScreenLayout >= 2) {
        return 1;
    }

    if ((gTaskDoneBits[0x11] & 4) && (level == LEVEL_W2_HARBOUR)) { // low byte of gTaskDoneBits[0x10]
        return 1;
    }

    /* four 16-bit values, in thousandths */
    a = (obj->data2 & 0xFFFF) * 0.001000000047f;
    b = ((obj->data2 >> 16) & 0xFFFF) * 0.001000000047f;
    c = (obj->data3 & 0xFFFF) * 0.001000000047f;
    d = ((obj->data3 >> 16) & 0xFFFF) * 0.001000000047f;

    scale = math_getZoneVolume(obj);

    data.source = obj;
    data.rateBase = (a * scale) * 1.271515948e-10f;
    data.rateRange = (b * scale) * 1.271515948e-10f;
    data.scaleBase = c;
    data.scaleRange = d;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffect(300, -1, 0x29, -1, 0, 0, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Spawns a StreamEmitter aimed along the object's rotation, configured by the bytes of data1 and data2. */
s32 level_spawnBulletZone(TriggerZone *obj) {
    StreamEmitter emitter;
    u32 value; // matching: holds the random number, then the effect address; two locals grow the frame
    struct {
        Vec3f direction;
        MtxF matrix;
    } scratch;

    obj->flags |= 4;
    if (gScreenLayout >= 2) {
        return 1;
    }
    math_mtxRotateRPYF(scratch.matrix, obj->pitch, obj->yaw, 0.0f);
    math_transformPoint(scratch.matrix, 0.0f, 100.0f, 0.0f, &scratch.direction.x, &scratch.direction.y, &scratch.direction.z);
    math_getHeadingElevation(&scratch.direction, &emitter.angleX, &emitter.angleY);
    emitter.angleX *= 0.017453292f;
    emitter.angleY *= 0.017453292f;
    emitter.minDelay = obj->data1 & 0xFF;
    emitter.delayRange = (obj->data1 >> 8) & 0xFF;
    value = math_random();
    emitter.position = &obj->x;
    emitter.timer = value % (emitter.delayRange + 1) + emitter.minDelay;
    emitter.minLife = (obj->data1 >> 16) & 0xFF;
    emitter.lifeRange = (obj->data1 >> 24) & 0xFF;
    emitter.rateBase = (obj->data2 & 0xFF) * 0.015625f;
    emitter.rateRange = ((obj->data2 >> 8) & 0xFF) * 0.015625f;
    emitter.speed = (obj->data2 >> 24) & 0xFF;
    emitter.distScale = 1.0f / emitter.speed;
    emitter.defaultRemaining = obj->ySize * emitter.distScale;
    emitter.spread = math_atan2(obj->xSize, obj->ySize);
    emitter.wobbleSpeed = ((obj->data2 >> 16) & 0xFF) * 0.00390625f * 0.7853982f;
    value = (u32)effect_newTimedEffect(300, -1, 0x2A, -1, 0, 0, sizeof(emitter), 0xFF, 0);
    if (value != 0) {
        memcpy((void *)(value + 0x28U), &emitter, sizeof(emitter));
    }
    return 1;
}

typedef struct Game40490PlanePoint {
    f32 x;
    f32 z;
} Game40490PlanePoint;

typedef struct Game40490PlanePacket {
    Game40490PlanePoint start;
    Game40490PlanePoint end;
    f32 edge[4];
    f32 top;
    f32 bottom;
    f32 scaleX;
    f32 scaleZ;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s8 bytes[4];
} Game40490PlanePacket;

/** Spawns an effect over the object's rotated rectangle (sizeX by sizeZ, sizeY high). */
s32 level_spawnMissileZone(TriggerZone *obj) {
    Game40490PlanePacket packet;
    TimedEffect *effect;
    Game40490PlanePoint corners[4];
    f32 s;
    f32 c;
    f32 z;
    s32 pad; // matching: stack slot of a local the original had here
    f32 x;

    obj->flags |= 4;
    x = obj->x;
    z = obj->z;
    s = sinf(obj->yaw * 0.01745329238f);
    c = cosf(obj->yaw * 0.01745329238f);
    corners[0].x = obj->xSize * c + x + obj->zSize * s;
    corners[0].z = obj->zSize * c + z - obj->xSize * s;
    corners[1].x = obj->xSize * c + x + -obj->zSize * s;
    corners[1].z = -obj->zSize * c + z - obj->xSize * s;
    corners[2].x = -obj->xSize * c + x + -obj->zSize * s;
    corners[2].z = -obj->zSize * c + z - -obj->xSize * s;
    corners[3].x = -obj->xSize * c + x + obj->zSize * s;
    corners[3].z = obj->zSize * c + z - -obj->xSize * s;
    packet.start = corners[1];
    packet.end = corners[2];
    packet.edge[0] = corners[0].x - corners[1].x;
    packet.edge[1] = corners[0].z - corners[1].z;
    packet.edge[2] = corners[3].x - corners[2].x;
    packet.edge[3] = corners[3].z - corners[2].z;
    packet.top = obj->y + obj->ySize;
    packet.bottom = obj->y;
    packet.scaleX = (obj->data2 & 0xFF) * 0.00390625f * 0.5f;
    packet.scaleZ = ((obj->data2 >> 8) & 0xFF) * 0.00390625f * 0.5f;
    packet.unk30 = 0.0f;
    packet.unk34 = ((obj->data2 >> 16) & 0xFF) * 0.00390625f * 250.0f;
    packet.unk38 = ((obj->data2 >> 24) & 0xFF) * 0.00390625f * 250.0f;
    packet.bytes[0] = obj->data1 & 0xFF;
    packet.bytes[1] = (obj->data1 >> 8) & 0xFF;
    packet.bytes[2] = (obj->data1 >> 16) & 0xFF;
    packet.bytes[3] = (obj->data1 >> 24) & 0xFF;
    effect = effect_newTimedEffect(300, -1, 0x2C, -1, 0, 0, sizeof(packet), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
    }
    return 1;
}

typedef struct Game4049014B60Packet {
    f32 pos[3];
    f32 rot[3];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    LightEffect *effects[6];
    f32 unk3C;
    void *owner;
    u8 active;
} Game4049014B60Packet;

/** Creates six light_createWithEffect objects at the object's position, coloured from gRaySpinnerChildColors, and an effect
 * with a random rotation that owns them (they are freed again if no effect slot is free). */
s32 level_spawnDiscoBallZone(TriggerZone *obj) {
    Game4049014B60Packet packet;
    s32 i;
    TimedEffect *effect;
    Vec3i pos;
    LightEffectHeader header;
    u32 count;

    obj->flags |= 4;
    packet.active = 1;
    packet.owner = obj;
    packet.pos[0] = obj->x;
    packet.pos[1] = obj->y;
    packet.pos[2] = obj->z;
    count = osGetCount();
    packet.rot[0] = ((math_random() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    count = osGetCount();
    packet.rot[1] = ((math_random() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    count = osGetCount();
    packet.rot[2] = ((math_random() * count) & 0xFFFF) * 1.525902189e-05f * 360.0f;
    packet.unk18 = (obj->data1 & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk1C = ((obj->data1 >> 16) & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk20 = (obj->data2 & 0xFFFF) * 0.152587890625f * 0.001000000047f;
    packet.unk3C = ((obj->data2 >> 16) & 0xFFFF) * 0.457763671875f * 9.999999975e-07f;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;
    for (i = 0; i < 6; i++) {
        s32 r;
        s32 g;

        header.flags = 2;
        header.update = -1;
        header.lifetime = 300;
        header.lightType = 0;
        r = (level == LEVEL_W5_END_ARENA) ? 0xFF : gRaySpinnerChildColors[i][0];
        g = (level == LEVEL_W5_END_ARENA) ? 0 : gRaySpinnerChildColors[i][1];
        count = (level == LEVEL_W5_END_ARENA) ? 0 : gRaySpinnerChildColors[i][2]; // matching: blue reuses count's register
        packet.effects[i] = light_createWithEffect(&header, &pos, 5, r, g, count, 0xFF, 0, 0, 0xFF, 0);
    }
    effect = effect_newTimedEffect(300, -1, 0x2E, -1, 0, 0x29, sizeof(packet), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &packet, sizeof(packet));
        return 1;
    }
    for (i = 0; i < 6; i++) {
        if (packet.effects[i] != NULL) {
            effect_destroyObject(packet.effects[i]);
        }
    }
    return 1;
}

s32 level_spawnLaserTubeZone(TriggerZone *obj) {
    EmitterTrigger data;
    TimedEffect *effect;
    s32 flag;

    obj->flags |= 4;
    data.params[1] = obj->data3;
    flag = (obj->data2 & 1) ? 1 : 0;
    data.params[0] = flag;
    data.record = obj;
    /* data1 holds a minimum and a range: countdown is random in [min, min + range] */
    data.minDelay = obj->data1 & 0xFFFF;
    data.delayRange = (obj->data1 >> 16) & 0xFFFF;
    data.countdown = (math_random() % (data.delayRange + 1)) + data.minDelay;
    math_mtxRotateRPYF(data.matrix, obj->pitch, obj->yaw, 0.0f);

    data.matrix[3][0] = obj->x;
    data.matrix[3][1] = obj->y;
    data.matrix[3][2] = obj->z;

    effect = effect_newTimedEffect(300, -1, 0x31, -1, 0, 0x2A, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

s32 level_addDustRegionZone(void) {
    RegionNode *item = light_newRegionNode();

    if (item == NULL) {
        return 1;
    }

    light_pushRegionNode(&gDustRegions, item);
    gDustRegionsArea += item->area;
    return 1;
}

s32 level_spawnZoneEffect(TriggerZone *obj) {
    ZoneEffectData data;
    ZoneEffect *effect;
    s32 bit1;
    s32 bit2;
    u32 value;

    obj->inactive = 1;
    data.zone = obj;
    data.id = obj->data2;

    collision_selectMap(0);
    data.node = collision_findZoneLeaf(obj->x, obj->z);

    value = obj->data3;
    bit1 = (value != 0) ? 1 : 0;
    bit2 = (value != 0) ? 2 : 0;
    data.flags = bit2 | bit1;
    data.actor = NULL;

    effect = effect_newTimedEffect(300, -1, -1, -1, 0, 0x2C, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return 1;
}
s32 level_spawnBoltRainZone(TriggerZone *obj) {
    BoltRainState data;
    TimedEffect *effect;

    obj->flags |= 4;
    obj->inactive = 1;

    data.region = obj;
    data.pending = 0.0f;
    data.timer = -1;
    data.pos.x = obj->x;
    data.pos.y = obj->y;
    data.pos.z = obj->z;
    data.radius = obj->xSize;
    data.height = obj->ySize;
    data.ground.height = -10000.0f;
    data.ground.surface = NULL;
    data.ground.source = 0;
    data.ground.flags = 0;
    data.ground.material = 0;

    collision_selectMap(0);
    data.zone = collision_findZoneLeaf(obj->x, obj->z);

    effect = effect_newTimedEffect(300, -1, 0x3C, -1, 0, 0x2D, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/* Handlers of level_spawnBoltZone, chosen by TriggerZone.data2. */
static void (*gBoltZoneSpawns[2])(TriggerZone *) = {
    level_spawnBoltArcZone,
    level_spawnBoltRingZone,
};

/** Runs the object's handler from gBoltZoneSpawns, chosen by data2 (0 or 1). */
s32 level_spawnBoltZone(TriggerZone *obj) {
    void (*handler)(TriggerZone *);
    s32 index = obj->data2;

    if ((index < 0) || (index >= 2)) {
        return 1;
    }

    handler = gBoltZoneSpawns[index];
    if (handler != NULL) {
        handler(obj);
    }

    return 1;
}

/** Spawns an effect on the object's rotated rectangle: its four corners and the edge vectors between them. */
void level_spawnBoltArcZone(TriggerZone *obj) {
    BoltArcState data;
    BoltArcObj *effect;
    f32 z;
    f32 y;
    f32 x;
    f32 corners[4][3];
    MtxF mtx;
    u8 i;

    data.region = obj;
    data.timer = -1;
    data.pending = 0.0f;
    obj->inactive = 1;
    x = obj->x;
    y = obj->y;
    z = obj->z;

    corners[0][0] = obj->xSize;
    corners[0][1] = 0.0f;
    corners[0][2] = obj->zSize;
    corners[1][0] = obj->xSize;
    corners[1][1] = 0.0f;
    corners[1][2] = -obj->zSize;
    corners[2][0] = -obj->xSize;
    corners[2][1] = 0.0f;
    corners[2][2] = -obj->zSize;
    corners[3][0] = -obj->xSize;
    corners[3][1] = 0.0f;
    corners[3][2] = obj->zSize;

    math_mtxRotateRPYF(mtx, obj->pitch, obj->yaw, 0.0f);

    mtx[3][0] = x;
    mtx[3][1] = y;
    mtx[3][2] = z;

    for (i = 0; i < 4; i++) {
        math_transformPoint(mtx, corners[i][0], corners[i][1], corners[i][2],
                      &data.edgeStart[i].x, &data.edgeStart[i].y, &data.edgeStart[i].z);
    }

    data.edgeDelta[0].x = data.edgeStart[1].x - data.edgeStart[0].x;
    data.edgeDelta[0].y = data.edgeStart[1].y - data.edgeStart[0].y;
    data.edgeDelta[0].z = data.edgeStart[1].z - data.edgeStart[0].z;
    data.edgeDelta[1].x = data.edgeStart[2].x - data.edgeStart[1].x;
    data.edgeDelta[1].y = data.edgeStart[2].y - data.edgeStart[1].y;
    data.edgeDelta[1].z = data.edgeStart[2].z - data.edgeStart[1].z;
    data.edgeDelta[2].x = data.edgeStart[3].x - data.edgeStart[2].x;
    data.edgeDelta[2].y = data.edgeStart[3].y - data.edgeStart[2].y;
    data.edgeDelta[2].z = data.edgeStart[3].z - data.edgeStart[2].z;
    data.edgeDelta[3].x = data.edgeStart[0].x - data.edgeStart[3].x;
    data.edgeDelta[3].y = data.edgeStart[0].y - data.edgeStart[3].y;
    data.edgeDelta[3].z = data.edgeStart[0].z - data.edgeStart[3].z;

    collision_selectMap(0);
    data.zone = collision_findZoneLeaf(obj->x, obj->z);
    data.actor = NULL;

    effect = effect_newTimedEffect(300, -1, 0x3D, -1, 0, 0x2E, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->state, &data, sizeof(data));
    }
}

s32 level_spawnSparkRegionZone(TriggerZone *obj) {
    SparkState data;
    TimedEffect *effect;

    obj->flags |= 4;
    obj->inactive = 1;

    data.region = obj;
    data.rate = math_getZoneArea(obj);
    collision_selectMap(0);
    data.zone = collision_findZoneLeaf(obj->x, obj->z);
    data.flags = 0;

    effect = effect_newTimedEffect(300, -1, 0x44, -1, 0, 0x2F, sizeof(data), 0xFF, 0);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
    return 1;
}

/** Passes the object's two end points (its position, and sizeY along its rotation from there) to
 * rope_create, higher end first. In levels 0xB, 0x2C and 0x26 only objects with a large enough data1 do. */
s32 level_spawnRopeZone(TriggerZone *obj) {
    Vec3f points[2];
    MtxF mtx;
    RopeSpawn payload;

    if (level == LEVEL_W1_COG_LAND) {
        if (obj->data1 < 6U) {
            return 1;
        }
    } else if (level == LEVEL_W1_POO_CABIN) {
        if (obj->data1 < 7U) {
            return 1;
        }
    } else if (level == LEVEL_W2_MACHINEGUNNEST) {
        if (obj->data1 < 2U) {
            return 1;
        }
    }

    math_mtxRotateRPYF(mtx, obj->pitch, obj->yaw, 0.0f);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y;
    mtx[3][2] = obj->z;

    math_transformPoint(mtx, 0.0f, obj->ySize, 0.0f, &points[1].x, &points[1].y, &points[1].z);

    points[0].x = obj->x;
    points[0].y = obj->y;
    points[0].z = obj->z;
    payload.id = obj->data1;

    /* the higher end is the top */
    payload.ends.top = points[points[0].y < points[1].y];
    payload.ends.bottom = points[points[1].y < points[0].y];

    payload.unk20 = 0;
    payload.length = fabsf(points[1].y - points[0].y);
    rope_create(&payload, 0);

    return 1;
}
