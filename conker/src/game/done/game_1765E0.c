#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata: the two object types of the timed effects (effect_newTimedEffect), read by game/Beam/BoltRain/boltrain.c and
 * game_1D3FC0.c too. */
const s32 gTimedEffectTypes[2] = { 0x23, 0x5F };

/*
 * This file's .data. The render setup and reset display lists of the timed effects; their entry in the object
 * type table gObjectTypes points to them.
 */
Gfx gTimedEffectRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
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
    func_150AE790, effect_dropFallingObject, effect_startTrapStream, func_151A18DC, func_151A1EE8, func_150C29F0, trail_stopTrailPair,
    flamethrower_spawnControllerFlame, effect_dropPointEffect, func_150DEBE0,
};
/* Tick callbacks (TimedEffect.tickCallback), run every frame. */
static void (*gTimedEffectTickFuncs[98])() = {
    effect_updatePuffTimer, func_151A0AF8, func_151A11E4, func_151A1998, func_151A1FB4, trail_updateBoneEmitter, func_151A5660,
    func_15156B54, trail_updateStartTask, trail_updateTrailPairTask, trail_updateRetryTask, trail_updateSurfaceSpawner, func_1515B21C, func_151918BC,
    effect_updateSparkEmitterTask, func_150E6B84, func_150E7C9C, trail_updateBurstEmitter, func_150C2C00, effect_updateJointEmitterTask, func_151929A4,
    func_151B1B34, func_151B2100, func_150CD7F8, func_15192DF0, effect_updateDiskEmitter, func_151094FC, func_15109FB8,
    func_1510A40C, func_15191D54, light_updateColourCycle, effect_updatePropEmitter, func_150AF328, trail_updateRandomPointEmitter, beam_updateEmitter,
    flamethrower_updateController, effect_updateOwnerSpark, beam_updateAreaEmitter, func_151082D0, func_150FC614, func_150E8470, func_150E9E34,
    weapon_updateStreamEmitter, func_151C4820, effect_spawnGroundFlashes, light_updateColourWave, func_150D1C30, screen_updateStrobe, screen_updateStrobeSpawner,
    func_150D26F0, func_150D2924, func_150E8B1C, func_150E8D5C, func_150E88C0, func_150E9178, func_150E93DC,
    func_150D54C8, screen_updateFlash, effect_updateActorEmitter, func_150CF530, beam_updateBoltRain, beam_updateBoltArc, beam_updateBoltChain,
    effect_updateRiseEmitter, effect_updateBoneBurst, func_151D4794, trail_updateAnchoredEmitter, beam_updateCastBeam, func_15105BC8, effect_updateObjBurst,
    effect_updateBoltRing, func_150D1B40, effect_updateAmbientPulse, func_150B1F90, func_151D9EB0, func_150AF7C4, beam_updateTrailEmitter,
    effect_updateDelayedBurst, effect_updateOwnerEffect, effect_updateEmitterSpawner, effect_updateBoltEmitter, beam_updateBoneSparks, func_150BA55C, effect_updatePulseEffect,
    beam_updateModelBeam, beam_updateFlashBurst, func_150F4DEC, effect_updateLineEmitter, func_150B0D20, func_150EC520, func_150EC6B0,
    func_151DC97C, func_150F2D14, func_150ECB8C, effect_updateZonePairEmitter, trail_updateTrackerEffect, effect_updateStatCounter, trail_updateStripEffect,
};
/* Draw callbacks (TimedEffect.drawCallback): return the advanced display list. */
static Gfx *(*gTimedEffectDrawFuncs[6])() = {
    func_151B2974, beam_drawWavyBeam, screen_drawStrobe, screen_drawFlash, trail_drawStripEffect, effect_drawTextBox,
};
/* Destroy handlers, per callback set (TimedEffect.callbackSet). */
static void (*gTimedEffectDestroyFuncs[74])() = {
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, trail_destroyTask, effect_defaultDestroyTimedEffect, trail_destroyTrailPairTask, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, func_151A5CAC, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_destroyJointEmitterTask, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, func_151B229C, func_151B2FE8, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, beam_destroyEmitter, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_destroyOwnerSpark, effect_defaultDestroyTimedEffect, func_150FC9A4, screen_freeStrobe, effect_defaultDestroyTimedEffect, func_150D5A2C, func_150D20B0,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, beam_destroyBoltChain,
    effect_defaultDestroyTimedEffect, effect_destroyTimedHandler, effect_destroyBoneBurst, beam_destroyModelBeamEffect, effect_defaultDestroyTimedEffect, trail_destroyAnchoredEmitter, beam_destroyCastBeam,
    func_150F9794, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_destroyPulseEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect,
    effect_defaultDestroyTimedEffect, effect_defaultDestroyTimedEffect, trail_destroyTracker, effect_destroyStatCounter,
};
/* Free handlers, per callback set. */
static void (*gTimedEffectFreeFuncs[74])() = {
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, trail_freeTask, effect_defaultFreeTimedEffect, trail_freeTrailPairTask, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, func_151A5CEC, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_freeJointEmitterTask, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, func_151B22C8, func_151B3014, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, beam_freeEmitter, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_freeOwnerSpark, effect_defaultFreeTimedEffect, func_150FC9D0, func_150D22A8, effect_defaultFreeTimedEffect, func_150D5A4C, func_150D20DC,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, beam_freeBoltChain,
    effect_defaultFreeTimedEffect, effect_freeTimedHandler, effect_freeBoneBurst, beam_freeModelBeamEffect, effect_defaultFreeTimedEffect, trail_freeAnchoredEmitter, beam_freeCastBeam,
    func_150F97C0, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_freePulseEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect,
    effect_defaultFreeTimedEffect, effect_defaultFreeTimedEffect, trail_freeTracker, func_151BFBD0,
};
/* Message handlers, per callback set; optional. */
static void (*gTimedEffectMessageFuncs[74])(TimedEffect *object, s32 msg, u8 msgType) = {
    NULL, trail_onBoneEmitterMessage, trail_onTaskEvent, func_1519E61C, trail_onTrailPairTaskEvent, trail_onRetryTaskMessage, trail_onSurfaceSpawnerEvent, NULL,
    func_15191A84, effect_emitterTaskMessage, NULL, NULL, NULL, NULL, NULL, effect_jointEmitterTaskMessage, func_15193234, func_151B1FAC,
    func_151B2F04, func_151B2EC4, NULL, beam_wavyBeamMessage, func_151932E0, NULL, NULL, NULL, func_1510A870, NULL,
    func_1510A8CC, func_1519277C, trail_onStreamTaskEvent, NULL, NULL, NULL, beam_areaEmitterMessage, effect_ownerSparkMessage, func_151087FC,
    func_150FC818, NULL, effect_attachedEffectMessage, func_150D596C, func_150D1F6C, func_150D32FC, effect_actorEmitterMessage, func_151A5F70,
    beam_boltRainMessage, beam_boltArcMessage, beam_sparkRegionMessage, beam_boltChainMessage, trail_onOwnedEffectMessage, effect_timedHandlerMessage, effect_boneBurstMessage,
    beam_modelBeamMessage, func_151D4C38, trail_onAnchoredEmitterMessage, beam_castBeamMessage, effect_stageMessage, NULL, beam_trailEmitterMessage, effect_ownerEffectMessage,
    effect_emitterSpawnerMessage, effect_boltEmitterMessage, beam_boneSparksMessage, func_150BA8F0, effect_pulseEffectMessage, func_150FA468, beam_flashBurstMessage,
    func_150B0CE0, func_150F3194, func_150ECB4C, func_150FACE4, effect_textBoxMessage, trail_onTrackerMessage, func_151BFBFC,
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
        object->header.state -= D_800BE9E4;
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
    func_151D5E30(object->buffers);
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

void effect_handleTimedEffectMessage(TimedEffect *object, s32 arg1, u8 arg2) {
    s32 set = object->callbackSet;

    if (set < 0 || set >= 74) {
        set = 0;
    }
    if (gTimedEffectMessageFuncs[set] != NULL) {
        gTimedEffectMessageFuncs[set](object, arg1, arg2);
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

void effect_sendEventToTimedEffects(s32 arg0, u8 arg1) {
    effect_sendEventToTypes(gTimedEffectTypes, 2, arg0, arg1);
}

/* effect_handleOwnerActorEvent for effects: tracks or retargets the effect's owner on actor events. */
void effect_handleTimedEffectOwnerEvent(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    effect_handleOwnerActorEvent(event, eventType, owner, ownerId, obj);
}
