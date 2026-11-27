#include <ultra64.h>
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/hud.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/rope.h"
#include "conker/screen.h"
#include "conker/trail.h"
#include "conker/weapon.h"

/* This file's .rodata: the two object types of the timed effects (effect_newTimedEffect), read by game/Beam/BoltRain/boltrain.c and
 * game/Effects/ZoneEffect/zoneeffect.c too. */
const s32 gTimedEffectTypes[2] = { 0x23, 0x5F };

/*
 * This file's .data. The render setup and reset display lists of the timed effects; their entry in the object
 * type table gObjectTypes points to them.
 */
Gfx gTimedEffectRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx gTimedEffectRenderResetDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5),
    gsSPEndDisplayList(),
};

/*
 * The callback tables of the timed effects. Each effect kind keeps its own view of the object, so the tables are
 * declared without a prototype, but for the message handlers: effect_handleTimedEffectMessage passes the message type as a u8.
 */
/* Expire callbacks (TimedEffect.expireCallback). */
static void (*gTimedEffectExpireFuncs[10])() = {
    effect_updateAttachedBurst, effect_dropFallingObject, effect_startTrapStream, particle_startHoverEmitter, particle_startSwellEmitter, particle_emitDiscParticle, trail_stopTrailPair,
    flamethrower_spawnControllerFlame, effect_dropPointEffect, effect_sendFirstLightAndGlowEvents,
};
/* Tick callbacks (TimedEffect.tickCallback), run every frame. */
static void (*gTimedEffectTickFuncs[98])() = {
    effect_updatePuffTimer, particle_updateSparkEmitterEffect, particle_updateGatherEmitter, particle_updateHoverEmitter, particle_updateSwellEmitter, trail_updateBoneEmitter, particle_updateRegionEmitter,
    particle_updateRandomBurstEffect, trail_updateStartTask, trail_updateTrailPairTask, trail_updateRetryTask, trail_updateSurfaceSpawner, particle_updateUnderwaterRiseEmitter, particle_retryDripEmitter,
    effect_updateSparkEmitterTask, particle_updateSourceEmission, particle_updateScreenBurst, trail_updateBurstEmitter, particle_updateOriginRingEmitter, effect_updateJointEmitterTask, particle_updateSideEmitter,
    particle_updateOwnerSideTask, rope_updateTie, particle_updateDriftBurstEmitter, particle_updateBoneSideEmitter, effect_updateDiskEmitter, particle_updateSegmentEmitter, particle_updateMatrixSegmentEmitter,
    particle_updatePointPairEmitter, particle_updateDustEmitter, light_updateColourCycle, effect_updatePropEmitter, particle_updateSmokeEmitter, trail_updateRandomPointEmitter, beam_updateEmitter,
    flamethrower_updateController, effect_updateOwnerSpark, beam_updateAreaEmitter, particle_updateAreaEmitter, particle_updateTwoSidedEmitter, particle_updateRandomEmitter, particle_updateRandomBurstEmitter,
    weapon_updateStreamEmitter, particle_updateWobbleStream, effect_spawnGroundFlashes, light_updateColourWave, effect_updateRaySpinner, screen_updateStrobe, screen_updateStrobeSpawner,
    particle_updateSparkSpawner, particle_updateSparkEmitter, particle_updateDustRegionSpawner, particle_updateFallingDustSpawner, particle_updateShakeTimer, particle_updateDebrisRegionSpawner, particle_updateFallingDebrisSpawner,
    particle_updateFollowEffect, screen_updateFlash, effect_updateActorEmitter, effect_spawnGroundFxNearPlayer, beam_updateBoltRain, beam_updateBoltArc, beam_updateBoltChain,
    effect_updateRiseEmitter, effect_updateBoneBurst, particle_updatePointSmoke, trail_updateAnchoredEmitter, beam_updateCastBeam, beam_updateSparkZone, effect_updateObjBurst,
    effect_updateBoltRing, light_updateAmbientRedDrift, effect_updateAmbientPulse, particle_updatePointEmitter, particle_updateDripEffect, particle_updateSmokeSource, beam_updateTrailEmitter,
    effect_updateDelayedBurst, effect_updateOwnerEffect, effect_updateEmitterSpawner, effect_updateBoltEmitter, beam_updateBoneSparks, particle_updateOwnerEmitter, effect_updatePulseEffect,
    beam_updateModelBeam, beam_updateFlashBurst, particle_updateCameraSpiralEmitter, effect_updateLineEmitter, effect_updatePerimeterEmitter, particle_updateShapeEmitter, particle_updateSourceEmitter,
    particle_updateColorEmitter, particle_updateBoneBurstEmitter, particle_updateFogFlash, effect_updateZonePairEmitter, trail_updateTrackerEffect, effect_updateStatCounter, trail_updateStripEffect,
};
/* Draw callbacks (TimedEffect.drawCallback): return the advanced display list. */
static Gfx *(*gTimedEffectDrawFuncs[6])() = {
    rope_drawLine, beam_drawWavyBeam, screen_drawStrobe, screen_drawFlash, trail_drawStripEffect, effect_drawTextBox,
};
/* Destroy handlers, per callback set (TimedEffect.callbackSet). */
static void (*gTimedEffectDestroyFuncs[74])() = {
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, trail_destroyTask, effect_defaultDestroyTimedEffect, trail_destroyTrailPairTask, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, particle_destroyRegionEmitter, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_destroyJointEmitterTask, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, rope_destroyTie, rope_destroyLine, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, beam_destroyEmitter, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_destroyOwnerSpark, effect_defaultDestroyTimedEffect, effect_destroyWithChild, screen_destroyStrobe, effect_defaultDestroyTimedEffect, particle_destroyFollowEffect, effect_destroyRaySpinner,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, beam_destroyBoltChain,
    effect_defaultDestroyTimedEffect, effect_destroyTimedHandler, effect_destroyBoneBurst, beam_destroyModelBeamEffect, effect_defaultDestroyTimedEffect, trail_destroyAnchoredEmitter, beam_destroyCastBeam,
    effect_destroyStage, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_destroyPulseEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, trail_destroyTracker, effect_destroyStatCounter,
};
/* Free handlers, per callback set. */
static void (*gTimedEffectFreeFuncs[74])() = {
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, trail_freeTask, effect_defaultFreeTimedEffect, trail_freeTrailPairTask, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, particle_freeRegionEmitter, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_freeJointEmitterTask, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, rope_freeTie, rope_freeLine, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, beam_freeEmitter, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_freeOwnerSpark, effect_defaultFreeTimedEffect, effect_freeWithChild, screen_freeStrobe, effect_defaultFreeTimedEffect, particle_freeFollowEffect, effect_freeRaySpinner,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, beam_freeBoltChain,
    effect_defaultFreeTimedEffect, effect_freeTimedHandler, effect_freeBoneBurst, beam_freeModelBeamEffect, effect_defaultFreeTimedEffect, trail_freeAnchoredEmitter, beam_freeCastBeam,
    effect_freeStage, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_freePulseEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, trail_freeTracker, hud_freeCounter,
};
/* Message handlers, per callback set; optional. */
static void (*gTimedEffectMessageFuncs[74])(TimedEffect *object, s32 msg, u8 msgType) = {
    NULL, trail_onBoneEmitterMessage, trail_onTaskEvent, trail_destroyOnEndEvent, trail_onTrailPairTaskEvent, trail_onRetryTaskMessage, trail_onSurfaceSpawnerEvent, NULL,
    particle_onDripRetryMsg, effect_emitterTaskMessage, NULL, NULL, NULL, NULL, NULL, effect_jointEmitterTaskMessage, particle_onSideEmitterMsg, particle_onOwnerSideTaskMsg,
    rope_tieMessage, rope_lineMessage, NULL, beam_wavyBeamMessage, particle_onBoneSideEmitterMsg, NULL, NULL, NULL, particle_onSegmentEmitterSwap, NULL,
    particle_onPointPairEmitterSwap, particle_onDustEmitterEvent, trail_onStreamTaskEvent, NULL, NULL, NULL, beam_areaEmitterMessage, effect_ownerSparkMessage, particle_onAreaEmitterEvent,
    particle_onTwoSidedEmitterMsg, NULL, effect_attachedEffectMessage, particle_onFollowEffectMsg, effect_raySpinnerMessage, particle_onSparkTrigger, effect_actorEmitterMessage, particle_handleRegionEmitterMessage,
    beam_boltRainMessage, beam_boltArcMessage, beam_sparkRegionMessage, beam_boltChainMessage, trail_onOwnedEffectMessage, effect_timedHandlerMessage, effect_boneBurstMessage,
    beam_modelBeamMessage, particle_onPointEmitterMsg, trail_onAnchoredEmitterMessage, beam_castBeamMessage, effect_stageMessage, NULL, beam_trailEmitterMessage, effect_ownerEffectMessage,
    effect_emitterSpawnerMessage, effect_boltEmitterMessage, beam_boneSparksMessage, particle_onOwnerEmitterEvent, effect_pulseEffectMessage, effect_handleLightSwitchEvent, beam_flashBurstMessage,
    effect_perimeterEmitterMessage, particle_onBoneBurstEmitterEvent, particle_onFogFlashEvent, func_150FACE4, effect_textBoxMessage, trail_onTrackerMessage, hud_counterMessage,
};


/* Allocates a timed callback object (0x28 header bytes plus dataSize) and initializes its header. */
void *effect_newTimedEffect(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags,
                    u8 callbackSet, s32 dataSize, u8 owner, s32 list) {
    TimedEffect *object;
    u8 type;
    s32 size;

    size = dataSize + sizeof(TimedEffect);
    type = (flags & 2) ? 95 : 35;

    object = effect_newObject(type, list, size, 1, owner, 1);
    if (object == NULL) {
        return NULL;
    }
    object->header.state = timer;
    object->expireCallback = expireCallback;
    object->tickCallback = tickCallback;
    object->drawCallback = drawCallback;
    object->header.flags = flags;
    object->callbackSet = callbackSet;
    bzero(object->buffers, sizeof(object->buffers));

    return object;
}

/* effect_newTimedEffect without a draw callback. */
void *effect_newTimedEffectNoDraw(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                    u8 owner, s32 list) {
    return effect_newTimedEffect(timer, expireCallback, tickCallback, -1, flags, callbackSet, dataSize, owner, list);
}

/* Per-frame update: counts the timer down (flag 1), runs the tick callback, then the expire callback and
 * removal once the timer goes negative. */
void effect_updateTimedEffect(TimedEffect *object) {
    if (object->header.flags & 1) {
        object->header.state -= gFrameTicks;
    }
    if (object->tickCallback != -1) {
        gTimedEffectTickFuncs[object->tickCallback](object);
    }
    if (object->header.state < 0) {
        if (object->expireCallback != -1) {
            gTimedEffectExpireFuncs[object->expireCallback](object);
        }
    }
    if (object->header.state < 0) {
        effect_destroyObject(&object->header);
    }
}

void effect_releaseTimedEffectBuffers(TimedEffect *object) {
    particle_freeViewBuffers(object->buffers);
}

void effect_defaultDestroyTimedEffect(TimedEffect *object) {
    effect_releaseTimedEffectBuffers(object);
    effect_defaultDestroy(&object->header);
}

void effect_defaultFreeTimedEffect(TimedEffect *object) {
    effect_releaseTimedEffectBuffers(object);
    effect_defaultFree(&object->header);
}

/* Dispatches through the object's callback set (out-of-range sets fall back to set 0). */
void effect_destroyTimedEffect(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    gTimedEffectDestroyFuncs[set](object);
}

void effect_freeTimedEffect(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    gTimedEffectFreeFuncs[set](object);
}

void effect_handleTimedEffectMessage(TimedEffect *object, s32 event, u8 eventType) {
    s32 set = object->callbackSet;

    if (set < 0 || set >= 74) {
        set = 0;
    }
    if (gTimedEffectMessageFuncs[set] != NULL) {
        gTimedEffectMessageFuncs[set](object, event, eventType);
    }
}

/* Runs the object's draw callback, if any; returns the advanced display list. */
Gfx *effect_drawTimedEffect(Gfx *gfx, TimedEffect *object, s16 view) {
    s32 drawCallback = object->drawCallback;

    if (drawCallback != -1) {
        gfx = gTimedEffectDrawFuncs[drawCallback](gfx, object, view);
    }
    return gfx;
}

void effect_sendEventToTimedEffects(s32 event, u8 eventType) {
    effect_sendEventToTypes(gTimedEffectTypes, 2, event, eventType);
}

/* effect_handleOwnerActorEvent for effects: tracks or retargets the effect's owner on actor events. */
void effect_handleTimedEffectOwnerEvent(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    effect_handleOwnerActorEvent(event, eventType, owner, ownerId, obj);
}
