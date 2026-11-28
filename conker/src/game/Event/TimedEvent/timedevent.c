#include <ultra64.h>

#include "structs.h"

/* This file's .bss (0x800D1580-0x800D188F), defined in address order before the headers declare it
   (see game/Actor/Movement/movement.c). */
s32 gEventParam; // parameter of the animation event being run (gEventParamF: the same bits as an f32)
ActorAnim *gModelAnimTables[187]; // per model id: animation table, after its AnimHeader (see model_loadAnimTable)
static f32 gEventParamF;
static f32 gEventResult;
static s32 gDeferredSound;
static s32 gEventLoopRunning;

#include <math.h>
#include "conker/actor.h"
#include "conker/aim.h"
#include "conker/anim.h"
#include "conker/attach.h"
#include "conker/audio.h"
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/flamethrower.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/math.h"
#include "conker/mp3.h"
#include "conker/particle.h"
#include "conker/prop.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/sight.h"
#include "conker/sound.h"
#include "conker/text.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/weapon.h"

/* matching: not in particle.h, as game/Effects/ModelBurst/modelburst.c passes an s32 style (definition: game/Particle/StyleParticle/styleparticle.c) */
void particle_spawnQuadEdgeStyleParticles(f32 (*verts)[3], f32 x, f32 z, u8 style, f32 scale, s32 type, s32 countA, s32 countB, u8 owner,
                                          s32 list);

/* matching: four scalars, not s32[4] (an array gets an 8-aligned, larger frame) */
struct SoundChoices {
    s32 sound0;
    s32 sound1;
    s32 sound2;
    s32 sound3;
};

/* Zero-terminated list of the event ids that run when event_runTimedEvents is filtered. */
const u8 gFilteredEventIds[12] = { 0x69, 0x6A, 0x9D, 0x94, 0x7E, 0x7F, 0x7A, 0x29, 0 };

/* Turn-angle thresholds and animations per row (also read by game/Actor/ActorMotion/actormotion.c and game/Script/ObjScript/objscript.c). */
const u8 gTurnAnimTable[12][10] = {
    { 0x10, 0x00, 0x30, 0x04, 0x05, 0x02, 0x03, 0xFF, 0x02, 0x03 },
    { 0x20, 0x03, 0xFF, 0x06, 0x05, 0x06, 0x05, 0xFF, 0x06, 0x05 },
    { 0x20, 0x00, 0x40, 0x0A, 0x0B, 0x04, 0x03, 0x70, 0x08, 0x09 },
    { 0x0E, 0x00, 0x20, 0x04, 0x03, 0x02, 0x01, 0xFF, 0x02, 0x01 },
    { 0x20, 0xFF, 0x40, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x15, 0x00, 0x30, 0x0C, 0x0D, 0x0C, 0x0D, 0xFF, 0x0C, 0x0D },
    { 0x28, 0xFF, 0x68, 0x05, 0x04, 0x0D, 0x10, 0xFF, 0x0D, 0x10 },
    { 0x40, 0x00, 0xFF, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x10, 0x00, 0x15, 0x0B, 0x0C, 0x10, 0x0F, 0xFF, 0x10, 0x0F },
    { 0x16, 0x00, 0xFF, 0x05, 0x04, 0x05, 0x04, 0xFF, 0x05, 0x04 },
    { 0x20, 0x00, 0xFF, 0x09, 0x08, 0x09, 0x08, 0x50, 0x12, 0x12 },
    { 0x60, 0xFF, 0x60, 0x0D, 0x10, 0x0D, 0x10, 0xFF, 0x0D, 0x10 },
};

/* pissed_walk animations, one picked at random (gPissedWalkFirstAnims for the first pick). */
const u8 gPissedWalkAnims[8] = { 0xA6, 0xA7, 0xA6, 0xA6, 0xA6, 0xA7, 0xA6, 0xA8 };
const u8 gPissedWalkFirstAnims[8] = { 0xAC, 0xAD, 0xAC, 0xAC, 0xAC, 0xAD, 0xAC, 0xA9 };

/* Per projectile kind: hit value (also read by game/Collision/RayCast/raycast.c) and spawn offset x, y, z + bone. */
const u8 gProjectileHitValues[20] = {
    0x00, 0x00, 0x10, 0x10, 0x00, 0x13, 0x10, 0x12, 0x23, 0x24,
    0x25, 0x10, 0x10, 0x10, 0x10, 0x3C, 0x1A, 0x10, 0x10, 0x00,
};
const s16 gProjectileSpawnOffsets[19][4] = {
    { -2, -14, 60, 4 },  { -2, -14, 60, 4 },  { -1, -22, 77, 9 },  { 0, 0, 0, 2 },
    { 14, -28, 37, 9 },  { -4, 23, 201, 3 },  { 0, 0, 0, 2 },      { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },      { -4, 23, 201, 3 },  { -4, 23, 201, 3 },  { -1, -20, 12, 14 },
    { -1, -20, 12, 14 }, { 60, 0, 0, 23 },    { -40, 0, 0, 23 },   { 48, 148, 74, 0 },
    { 0, 0, 0, 0 },      { 0, 0, 0, 9 },      { -1, -22, 77, 9 },
};

/* Effect offsets (the words are f32 bit patterns). */
const Vec3f gTankMuzzleStart = { 0.0f, 12.0f, 120.0f };
const Vec3f gTankMuzzleEnd = { 0.0f, 27.0f, 215.0f };
const Vec3f D_80099B88 = { 0.0f, 0.0f, 544.0f };
const Vec3f D_80099B94 = { 0.0f, 0.0f, 700.0f };
const Vec3f D_80099BA0 = { 0.0f, 0.0f, 278.0f };
const Vec3f D_80099BAC = { 0.0f, 0.0f, 556.0f };

/* Locals of the effect spawners event_spawnBone1BeamFlash / event_spawnBoneBeamFlashRumble, in stack order. */
typedef struct EffectLocals {
    Vec3i header2;
    LightEffectHeader header;
    u8 pad16[2];
    Vec3f worldPos;
    ParticleSize randoms;
    Vec3f vec78;
    Vec3f localPos;
} EffectLocals;

/* A floor probe result together with a point (func_15071A64, func_15074C00). */
typedef struct {
    /* 0x00 */ CollisionHit ground;
    /* 0x24 */ Vec3f pos;
} GroundPoint; // size 0x30

/* One entry of an object's timed event list. */
typedef struct TimedEvent {
    f32 time;
    u8 pad4[3];
    u8 id;
    s32 parameter;
} TimedEvent;

typedef void EventHandler(void);

/* Animation-command handlers, indexed by the command (see event_runAnimCommand). */
void (*gAnimCommandProcs[])(s32) = {
    func_1506EFC8, func_1506F004, NULL, event_attachOwnerEmitter,
    event_startDustEmitter50, NULL, event_spawnFoot0Dust, event_spawnFoot1Dust,
    event_sprayTrails, event_spawnBone12Flame, event_stopFlamethrowerJets, event_callEmptyActorEventHook,
    event_spawnBone13Flame, event_stopFlames, NULL, func_1506F8F0,
    func_1506F9C0, event_spawnBone26Tracer, NULL, event_fireJointShot,
    event_fireJointShot, NULL, NULL, event_startSparkEmitter70,
    event_cancelTrailPair, event_startSparkEmitter40, NULL, event_spawnSide0StepEffects,
    event_spawnSide1StepEffects, event_spawnBone25StreakBurst, NULL, event_stopFlamethrowerJets,
    event_spawnBone1Tracer, event_spawnJetPreset5, event_queueTrailPair, event_spawnGroundMark0,
    event_spawnGroundMark1, event_spawnGroundMark0Alt, event_spawnGroundMark1Alt, event_spawnGroundMark2,
    event_spawnGroundMark3, func_1506FEF8, func_1506FF24, event_spawnSparkEmitterTask,
    event_spawnBoneGroundBurst0, event_spawnBoneGroundBurst1, event_spawnBoneGroundBurst2, event_spawnImpactRing,
    event_spawnJointImpact, event_spawnLandingBurst, event_spawnBloodGroundBurst, event_spawnJetPreset6,
    event_stopFlamethrowerJets, event_spawnBoneModelBurst, event_attachBloodStrip, event_fireBullet,
    event_fireSegmentShot, NULL, NULL, event_explodeSightAtActor,
    event_spawnShortLight, event_fireTankGun, event_fireBeamSlot, event_fireBeamSlot,
    event_releaseBeamSlot, event_releaseBeamSlot, event_fireSlotProjectiles, event_fireMuzzle0,
    event_fireMuzzle1, event_fireLauncher, event_runLauncherMuzzle, event_fireBonePointProjectile,
    event_spawnBonePointShotFx, event_spawnBone9Flame, event_spawnImpactBlast1, event_spawnImpactBlast0,
    event_spawnBone1BeamFlash, event_spawnBone2BeamFlashRumble, event_spawnBone1BeamFlashRumble, NULL,
    NULL, event_spawnTwoSidedEmitter, event_fireForwardProjectile, event_fireAttachedGun,
    event_spawnShotEffectsWithAttacks, event_spawnStripEffect, event_spawnStripEffect, event_notifyFollowersGone,
    event_attachPointEmitter4, event_attachPointEmitter2, event_attachPointEmitter3, event_emptyAnimCommand91,
    event_spawnShotEffects, event_spawnShotEffectsWithRay, event_spawnGunMuzzleSegmentParticle, event_spawnLargeObjBurst,
    event_rumbleNearbyPlayers, event_attachBeamTargetEffects, event_releaseBeamTargetEffects, event_spawnDelayedBurst,
    event_destroyActorEmitters, event_spawnBonePointSegmentParticle, event_spawnRandomBoneImpactBurst, event_spawnBonePointImpact0,
    event_spawnBonePointImpact1, event_spawnBonePointImpact2, event_emitLineBursts, func_15071A64,
    event_startDustEmitter, func_15071B18, event_spawnImpactBurstNearBone,
};
/* End-of-anim state handlers (run when the anim reaches its last frame), indexed by the state number of an animation entry (see game/Actor/ActorMotion/actormotion.c). */
void (*gAnimStateProcs[])(void) = {
    event_doNothing, anim_updateIdle, func_1506AF74, func_1506AFE0,
    func_1506B020, event_emptyAnimState05, func_1506B078, event_doNothing,
    event_playNextAnim, event_playNextAnimHoldLastFrame, func_1506B1E8, func_1506B228,
    event_playDefaultIdleAnim, func_1506B328, func_1506B2BC, event_emptyAnimState15,
    event_playNextAnimDefaultSpeed, event_finishDeath, NULL, event_emptyAnimState19,
    event_emptyAnimState20, func_1506B5B4, func_1506B5CC, NULL,
    event_freezeAndFade, event_doNothing, event_doNothing, event_doNothing,
    NULL, event_emptyAnimState29, NULL, event_pickTurnAnim,
    event_clearScriptTimer, func_1506B520, func_15071ED4, func_15071F14,
    event_repickPissedWalk, func_1506B82C, func_1506B7F4, event_removeSelf,
    func_15072968, func_150729D0, event_playFollowUpAnim, event_clearIdleDelay,
    func_1506B860, func_1506B88C, func_1506B8B4, func_1506B91C,
    func_1506B984, event_updateIdle, event_emptyAnimState50, func_1506B944,
    func_1506B8F4, event_emptyAnimState53, event_clearIntelAnimSeq, event_markContextTextSeen,
    event_finishGrenadeThrow, func_15072A7C, event_removeSelfAndStopParts, event_finishLadderGrabFromTop,
};
/* Timed event handlers, indexed by the event id (see event_runTimedEvents). */
static EventHandler *gTimedEventProcs[] = {
    event_doNothing, event_setYSpeed, event_jump, event_setYGravity,
    event_clearNoRun, event_setWantSpeed, event_setNoRun, event_setDoingHit,
    event_playSoundCommandAboveWater, event_playSoundCommand, event_playDeferredSound, event_fireProjectile,
    event_requestExpression, event_resetBlink, event_setHeadEffectOffset, event_doNothing,
    event_emptyTimedEvent16, event_stopHeadEffect, func_1506D4F4, event_finishDeathOutsideCutscene,
    event_setTurnAnimRow, event_doNothing, event_finishClimbUp, event_setYGravityTenths,
    event_setHeldObjectState, event_dropHeldObject, func_1506D934, event_doNothing,
    effect_rearmPlayerMarker, func_1506D958, event_setPlayerIvehitit, event_setImmune,
    func_1506DA94, event_startChasing, event_callEmptyScriptCommand, event_setBreakStun,
    event_setSpeed, event_setTaskInt2, event_setIntelHitRoute, event_setTurnRates,
    func_1506DC10, event_removeWeaponProp, event_setBodyHeight, event_setTaskInt1,
    event_setEnergy, event_emptyTimedEvent45, event_skipNextEventIfNotLifting, event_scaleByPlayerDistance,
    event_addNoShove, event_setMatrixPhysics, event_emptyTimedEvent50, event_spawnThrottledEmitterIfCameraAboveLimit,
    event_spawnWakePuffPair, event_playSound2Range1800, event_throwHeldObject, event_playSound1Range8000,
    event_playSound2Range8000, event_jumpToFrame, NULL, NULL,
    func_1506EC50, event_holdAnimLastFrame, NULL, event_setHopSpeed,
    NULL, NULL, func_1506ED0C, NULL,
    func_1506ED4C, func_1506ED68, event_setScriptTimer, event_pickTurnAnimAlt,
    event_setMvmtControl, event_addTaskAttrib, event_clearTaskAttrib, event_clearMvmtControl,
    event_playSound2OnGroundUnlessIvehitit, event_playQuietSound2Range1800, func_1506DE84, event_setAfterimage,
    event_playSound2Range800, event_setWallHitScript, event_playSound1Range1800, event_playSound1Range800,
    event_playQuietSound1Range3000, event_addMvmtControl, event_setIntelAccel, event_setIntelDecel,
    event_playIntelAnimSeq, event_skipNextEventInCutscene, event_runAnimCommand, event_startHeadEffect,
    event_startDeathCountdown, event_clearReactionTimerOnGround, event_launchPlayer, event_fadeOwnedObject,
    event_attachTargetToBoneAndSetState, event_runActorEffects, event_runSurfaceHandler, event_setAnimSpeedTenths,
    event_setOwnedObjectsState, event_runChildAnimCommand, event_requestSubmergedExpression, event_setTaskInt0,
    event_setAimTargetMode, event_createPartSet, event_removePartSet, event_stopSound1,
    event_stopSound2, event_playSound2IfNotPlaying, event_playSound2Range3000, event_playQuietSound2Range3000,
    event_runFadeCommand, event_setReactionTimer, event_scaleAnimSpeed, event_clearReactionTimerBelowAnimSpeed,
    event_setStunned, event_fireRayAtObjects, func_1506BDE8, event_spawnObjectAhead,
    event_emptyTimedEvent120, event_setAlertCooldown, event_respawnObject, event_showOnlyParts,
    event_showParts, event_hideParts, event_removeObjectById, event_removeAttachedObjectById,
    event_startCameraEnvelope, func_150730A4, event_fireVolley, func_150730D0,
    event_releaseHeldObject, event_setWaitTimer, event_waitForTimer, event_setStickSpeed,
    func_15073A50, event_setAfterimageHiddenParts, event_setRopeSwing, func_15073C50,
    event_attachTargetToBone, event_playSound1Range3000, func_15073CB8, event_playSound1Range4000,
    event_playSound2Range4000, event_playSound2Range1300, event_playSound1Range1300, event_setBodyRadius,
    event_detachObjectById, func_15073DA4, func_1506B82C, event_setBouncePercent,
    event_clearNoShove, func_15073F78, event_startMp3TrackRange1800, event_spawnThrownItem,
    event_dealDamage, event_setWeaponStatus, func_15074664, event_setOwnerState,
    event_addShotsFired, event_skipEventsOnTaskBit, func_15073E2C, func_15073EA4,
    func_15073F1C,
};
s32 D_80086690 = 0x0FA10CDF; /* not referenced */
/* Hit-reaction handlers, indexed by HitReaction.handler (see game/Actor/ActorMotion/actormotion.c). Declared without a
   prototype: most take (Actor *self, Actor *other, u8 *result), a few take other arguments. */
void (*gHitReactionProcs[])() = {
    event_emptyHitReaction0, event_emptyHitReaction1, effect_hitReactLavaBoulder, func_15074C00,
    event_hitReactProjectile, effect_hitReactFireImp, event_hitReactSetTaskInt1, event_hitReactSetUpSmoke,
    effect_hitReactStripsSprayAndDrip, func_15074E80, effect_hitReactBloodSpray, effect_hitReactSprayAndRumble,
    func_15074EE8, func_15074F30, func_15074F48, func_15074FD4,
    effect_hitReactNone, event_hitReactHop, effect_hitReactKill, effect_hitReactSetOnFireDamaging,
    effect_hitReactHeadShot, effect_hitReactImpactDebris, func_15194B94, effect_hitReactFlashAndKill,
    effect_hitReactBulletHitAndKill, effect_hitReactLightBurst, effect_hitReactSetOnFire, effect_hitReactActorImpact,
    effect_hitReactSplash, event_hitReactSetYGravity, func_150750C4, func_150FFD2C,
    effect_hitReactJointImpact, effect_hitReactSmallSplash, func_15195698, effect_hitReactLava,
    effect_hitReactRumble,
};

/*
 * Runs every event of the object's event list whose time is <= `time`. Each event stores its
 * parameter in gEventParam (and as a float in gEventParamF) and calls handler gTimedEventProcs[id].
 * With `filtered` set, only ids in the zero-terminated list gFilteredEventIds run. A handler that
 * restarts the object's state (new token) rewinds `time` to the state's time. Returns
 * gEventResult, which handlers may set.
 */
f32 event_runTimedEvents(Actor *obj, f32 time, u8 filtered) {
    s32 token;
    u8 id;

    if (obj->drawtype == 4) {
        return 0.0f;
    }
    gEventResult = 0.0f;
    if (time < obj->lastEventTime) {
        obj->speccnt = 0;
    }
    gEventLoopRunning = 1;
    while (obj->events != NULL && obj->events[obj->speccnt].time <= time && gEventLoopRunning != 0) {
        if (obj->events[obj->speccnt].time < 900.0f) {
            gEventParam = obj->events[obj->speccnt].parameter;
            gEventParamF = *(f32 *)&gEventParam;
            token = obj->anim->frameRom[0];
            if (gCutsceneStates[0] == 0 || gCutsceneRunTimedEvents[0] != 0) {
                id = obj->events[obj->speccnt].id;
                if (filtered) {
                    s32 i;
                    for (i = 0; filtered; i++) {
                        if (gFilteredEventIds[i] == 0) {
                            id = 0;
                        }
                        if (id == gFilteredEventIds[i]) {
                            break;
                        }
                    }
                }
                if (id != 0) {
                    gTimedEventProcs[id]();
                }
            }
            if (token != obj->anim->frameRom[0]) {
                time = obj->anim->frame[0];
            } else {
                obj->speccnt++;
            }
        }
    }
    return gEventResult;
}

void func_1506AF74(void) {
    if ((gCurrentActor->unk100 & 8) != 0) {
        gCurrentActor->unk100 |= 4;
    }
    gCurrentActor->unk100 &= 0xF7;
    actor_playAnim(gCurrentActor, 0x38, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506AFE0(void) {
    actor_playAnim(gCurrentActor, 0x48, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void func_1506B020(void) {
    gCurrentActor->unk1CB = 1;
    actor_playAnim(gCurrentActor, 0x39, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void event_emptyAnimState05(void) {
}

void func_1506B078(void) {
    f32 speed = (gCurrentActor->ypos - (gCurrentActor->waterHeight - 150.0f)) * 0.005f;

    if (speed < 0.0f) {
        speed = 1.0f;
    } else {
        speed += 1.0f;
    }
    event_playAnim(209, speed, 4.0f);
}

/* actor_playAnim on the current object with the given anim, speed and blend. */
void event_playAnim(s32 anim, f32 speed, f32 blend) {
    actor_playAnim(gCurrentActor, anim, speed, blend, 0.0f, 0.0f, 0);
}

void event_playNextAnim(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, gCurrentActor->animsp, 3.0f, 0.0f, 0.0f, 0);
}

void event_playNextAnimHoldLastFrame(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, gCurrentActor->animsp, 3.0f, 0.0f, 0.0f, 1);
}

void func_1506B1E8(void) {
    actor_playAnim(gCurrentActor, 0x3C, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_1506B228(void) {
    actor_playAnim(gCurrentActor, 0x54, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void event_playDefaultIdleAnim(void) {
    gCurrentActor->count5 = 0;
    gCurrentActor->norun = 0;
    actor_playAnim(gCurrentActor, 0xF, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

void func_1506B2BC(void) {
    Actor *obj = event_getFirstOwnedObject(gCurrentActor);

    if (obj != NULL && (obj - movement_obj) + 1 == movement_obj[0].stand_on_char) {
        gCurrentActor->scriptCmd = NULL;
        gCurrentActor->intelwalk = 4;
    }
}

void func_1506B328(void) {
    actor_playAnim(gCurrentActor, 0x3E, 1.0f, 5.0f, 0.0f, 0.0f, 0);
}

void event_emptyAnimState15(void) {
}

void event_finishDeath(void) {
    actor_finishDeath(gCurrentActor - movement_obj);
}

void event_emptyAnimState29(void) {
}

/* Picks unk244 from the gTurnAnimTable table by the angle between the object's facing and the player. */
void event_pickTurnAnim(void) {
    s32 absDiff32;
    s16 angleDiff;
    u8 sel = 0;
    u8 absDiff;

    angleDiff = (gCurrentActor->langle + (gCurrentActor->animEndTurn << 8)) -
                actor_getDirectionAngle(movement_obj[0].xpos - gCurrentActor->xpos,
                              gCurrentActor->zpos - movement_obj[0].zpos, 0);
    absDiff = angleDiff >> 8;
    if (absDiff & 0x80) {
        absDiff = -absDiff;
    }
    absDiff32 = absDiff;
    if (absDiff < gTurnAnimTable[0][sel]) { // matching: all three indexed by sel (still 0); constant indices schedule differently
        sel = 1;
    } else {
        if (gTurnAnimTable[0][sel + 7] < absDiff32) {
            sel = 5;
        } else if (gTurnAnimTable[0][sel + 2] < absDiff32) {
            sel = 2;
        }
        if (angleDiff < 0) {
            sel += 4;
        } else {
            sel += 3;
        }
    }
    gCurrentActor->speccnt = 0;
    gCurrentActor->intel_animseq = gTurnAnimTable[0][sel];
    gCurrentActor->scriptTimer = 20000;
}

void event_pickTurnAnimAlt(void) {
    event_pickTurnAnim();
}

void event_clearScriptTimer(void) {
    gCurrentActor->scriptTimer = 0;
}

void func_1506B520(void) {
    s32 anim;

    if (gCurrentActor->energy != 0) {
        if (gCurrentActor->mv != NULL && gCurrentActor->mv->rt_context_b == 0x25) {
            anim = 427;
        } else {
            anim = 240;
        }
        actor_playAnim(gCurrentActor, anim, 1.5f, 3.0f, 0.0f, 0.0f, 0);
    } else {
        actor_kill(gCurrentActor, 1);
    }
}

void event_emptyAnimState19(void) {
}

void event_emptyAnimState20(void) {
}

void func_1506B5B4(void) {
    gCurrentActor->unk1CB = 1;
}

void func_1506B5CC(void) {
    gCurrentActor->unk1CB = 1;
}

void event_freezeAndFade(void) {
    actor_fadeCommand(gCurrentActor, 2, 7, 8, 0);
    gCurrentActor->animsp = 0.0f;
    gCurrentActor->anim->speed[0] = 0.0f;
}

/* Picks a random entry (0..count-1) of gPissedWalkFirstAnims / gPissedWalkAnims for the object's mv->pissed_walk. */
void event_pickPissedWalk(s32 count) {
    u32 rand;
    u32 pick;
    u8 index;
    ActorCtl *ext;

    rand = math_random();
    pick = rand % (u8)count;
    index = pick;
    ext = gCurrentActor->mv;
    if (ext->pissed_hangover != 0) {
        if (level == LEVEL_W1_TRAINING) {
            if ((u8)pick == 7) {
                index = 0;
            }
        }
        ext->pissed_walk = gPissedWalkFirstAnims[index];
    } else {
        ext->pissed_walk = gPissedWalkAnims[(u8)pick];
    }

    if (gCurrentActor->mv->pissed_walk == 0xA7) {
        gEventParam = 0xFF020144;
        gCurrentActor->mv->pissed_stumble = (math_random() % 3U) + 2;
        event_playSound1Range1800();
        gCurrentActor->speed = gCurrentActor->speed * 0.2f;
    }
}

void event_repickPissedWalk(void) {
    u8 roll = math_random() & 0xFF;

    if (gCurrentActor->waterHeight != -10000.0f && gCurrentActor->ypos < gCurrentActor->waterHeight) {
        gCurrentActor->mv->pissed_walk = 720;
    } else {
        if (gCurrentActor->animseq.uh == 0xA7) {
            if (gCurrentActor->mv->pissed_stumble == 0) {
                roll = 0;
            } else {
                gCurrentActor->mv->pissed_stumble--;
                roll = 0xFF;
            }
        }
        if (roll < 0x80) {
            event_pickPissedWalk(8);
        }
    }
}

void func_1506B7F4(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    event_pickPissedWalk(2);
}

void func_1506B82C(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    anim_updateIdle();
}

void func_1506B860(void) {
    event_playAnim(181, 0.63f, 5.0f);
}

void func_1506B88C(void) {
    event_playAnim(182, 1.0f, 4.0f);
}

void func_1506B8B4(void) {
    gCurrentActor->mv->rt_context_b = 0;
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    anim_updateIdle();
}

void func_1506B8F4(void) {
    event_playAnim(222, 0.75f, 4.0f);
}

void func_1506B91C(void) {
    event_playAnim(191, 1.0f, 4.0f);
}

void func_1506B944(void) {
    gCurrentActor->norun = 0;
    gCurrentActor->count5 = 0;
    event_playAnim(197, 1.0f, 4.0f);
}

void func_1506B984(void) {
    event_playAnim(175, 1.0f, 4.0f);
}

void event_clearIntelAnimSeq(void) {
    gCurrentActor->intel_animseq = 0;
}

void event_markContextTextSeen(void) {
    ActorCtl *ext;

    gPendingButtons[dc_glbl_CharNum] |= 0x4000;
    ext = gCurrentActor->mv;
    if (ext != NULL) {
        ext->rt_context_b = 0;
        gSeenTextBits[gContextTextId >> 3] |= 1 << (gContextTextId & 7);
    }
    anim_updateIdle();
}

/* Sound handlers: play sound gEventParam (its low half) at the current object (0 stops it). */
void event_playOrStopSound2(s32 minDistance, s32 maxDistance) {
    if (gEventParam == 0) {
        sound_stopActorSound2(gCurrentActor);
    } else {
        if (gCurrentActor->CameraPtr == 0) {
            sound_playActorSound2(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
        } else {
            sound_playActorSound2(gEventParam, gCurrentActor, 24000, 500, 2500);
        }
    }
}

void event_playOrStopSound1(s32 minDistance, s32 maxDistance) {
    if (gEventParam == 0) {
        sound_stopActorSound1(gCurrentActor);
    } else {
        if (gCurrentActor->CameraPtr == 0) {
            sound_playActorSound1(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
        } else {
            sound_playActorSound1(gEventParam, gCurrentActor, 24000, 500, 2500);
        }
    }
}

void event_startMp3Track(s32 minDistance, s32 maxDistance) {
    mp3_startTrackAt(gEventParam, gCurrentActor, 28000, minDistance, maxDistance);
}

void event_playQuietSound2(s32 minDistance, s32 maxDistance) {
    if (gCurrentActor->CameraPtr == 0) {
        sound_playActorSound2(gEventParam, gCurrentActor, 14000, minDistance, maxDistance);
    } else {
        sound_playActorSound2(gEventParam, gCurrentActor, 12000, 500, 2500);
    }
}

void event_playQuietSound1(s32 minDistance, s32 maxDistance) {
    if (gCurrentActor->CameraPtr == 0) {
        sound_playActorSound1(gEventParam, gCurrentActor, 14000, minDistance, maxDistance);
    } else {
        sound_playActorSound1(gEventParam, gCurrentActor, 12000, 500, 2500);
    }
}

void event_setYSpeed(void) {
    gCurrentActor->yspd = gEventParam;
}

/* Jump: sets yspd from gEventParam when the object is (nearly) at height unk180. */
void event_jump(void) {
    Actor *obj;
    f32 height;

    obj = gCurrentActor;
    if (obj->type == 0x16) {
        if (obj->ypos == obj->floorY) {
            gCurrentActor->yspd = (math_rand() % 8) + gEventParam;
            obj = gCurrentActor;
            if (obj->speed != 0.0f) {
                obj->yspd = obj->yspd * (obj->speed / 40.0f);
            } else {
                obj->yspd = 0.0f;
            }
        }
    } else {
        height = obj->ypos - obj->floorY;
        if (-5.0f < height && height < 5.0f) {
            obj->yspd = gEventParam;
        }
    }
}

void func_1506BDE8(void) {
    if (gCurrentActor->ypos < -1750.0f) {
        gCurrentActor->yspd = gEventParam;
    }
}

void event_setYGravity(void) {
    gCurrentActor->ygrav = gEventParam;
}

void event_setYGravityTenths(void) {
    gCurrentActor->ygrav = gEventParam * 0.1f;
}

void event_clearNoRun(void) {
    gCurrentActor->norun = 0;
}

void event_setWantSpeed(void) {
    gCurrentActor->wantspeed = gEventParam;
}

void event_setNoRun(void) {
    gCurrentActor->norun = gEventParam;
}

void event_setDoingHit(void) {
    gCurrentActor->doing_hit = gEventParam;
    gCurrentActor->doingHitTimer = gEventParam >> 8;
}

void event_playSoundCommandAboveWater(void) {
    if (gCurrentActor->waterHeight <= gCurrentActor->ypos) {
        event_playSoundCommand();
    }
}

/*
 * Plays sound command gEventParam at the current object: bits 0-10 the sound id, bits 14/15 play
 * only for / except owner id (top byte), bit 16 defers to event_playDeferredSound, bits 11-13 the mode
 * (0x2000: surface-dependent sound from sound_pickEffectVariant, volume scaled by speed).
 */
void event_playSoundCommand(void) {
    u32 sound;
    u32 owner;
    u16 volume;
    f32 intensity;
    f32 max;
    u32 ownerFilter;
    s32 type;
    u8 id;

    sound = gEventParam & 0x7FF;
    id = gCurrentActor->character_type;
    ownerFilter = (u32)gEventParam >> 24;
    if (gModelDefs[id]->subModelCount != 0) {
        owner = *gSubModelLists[gModelDefs[id]->subModelList];
    } else {
        owner = id;
    }
    gDeferredSound = 0;
    switch (gEventParam & 0xC000) {
    case 0x4000:
        if (owner != ownerFilter) {
            return;
        }
        break;
    case 0x8000:
        if (owner == ownerFilter) {
            return;
        }
        break;
    }
    if (gEventParam & 0x10000) {
        gDeferredSound = gEventParam + 1;
        return;
    }
    volume = 32000;
    switch (gEventParam & 0x3800) {
    case 0x2000:
        if (gScreenLayout >= 2) {
            break;
        }
        type = collision_getSurfaceType(gCurrentActor->floorMaterial);
        if (type == 0xC && gCurrentActor->heightOffset > -17.0f) {
            sound = sound_pickVariant(0x9C, 2);
        } else if (gCurrentActor->waterHeight != -10000.0f && gCurrentActor->ypos <= gCurrentActor->waterHeight) {
            sound = sound_pickEffectVariant(8, NULL, sound);
            type = 8;
        } else {
            sound = sound_pickEffectVariant(type, (s16 *)&volume, sound);
        }
        if (sound == 0) {
            break;
        }
        if (gCurrentActor->mv != NULL) {
            intensity = gCurrentActor->mv->actual_speed;
        } else {
            intensity = gCurrentActor->yspd;
        }
        if (type == 0xC) {
            max = 10.0f;
        } else {
            max = 30.0f;
        }
        if (max < intensity) {
            intensity = max;
        }
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        intensity *= (f32)0.5 / max; // matching: 0.5f compiles differently
        intensity += 0.5f;
        volume = volume * intensity;
        if (type == 8) {
            sound_playActorUntracked(sound, gCurrentActor, volume, 500, 2500);
            break;
        }
        /* fallthrough */
    case 0:
        sound_playActorSound2(sound, gCurrentActor, volume, 500, 2500);
        break;
    case 0x1000:
        sound_playActorSound1(sound, gCurrentActor, 32000, 500, 2500);
        break;
    case 0x3800:
        if (sound_getPlayingSound(gCurrentActor->soundHandle2) != sound) {
            sound_playActorSound2(sound, gCurrentActor, volume, 500, 2500);
        }
        break;
    case 0x3000:
        sound_playActorUntracked(sound, gCurrentActor, 32000, 500, 2500);
        break;
    }
}

/*
 * Follow-up of event_playSoundCommand's deferred sound (gDeferredSound = parameter + 1): picks one of up to
 * four 11-bit sound ids (the deferred one plus three packed in gEventParam) and plays it.
 */
void event_playDeferredSound(void) {
    s32 cmd;
    struct SoundChoices sounds;
    s32 index;
    s32 sound;

    if (gDeferredSound != 0) {
        gDeferredSound--;
        cmd = gEventParam;
        sounds.sound0 = gDeferredSound & 0x7FF;
        sounds.sound1 = cmd & 0x7FF;
        sounds.sound2 = (cmd >> 11) & 0x7FF;
        sounds.sound3 = (cmd >> 22) & 0x7FF;

        if (sounds.sound3 != 0) {
            index = 4;
        } else if (sounds.sound2 != 0) {
            index = 3;
        } else if (sounds.sound1 != 0) {
            index = 2;
        } else {
            index = 0;
        }

        if (index != 0) {
            index = sound_pickVariant(sounds.sound0, index) - sounds.sound0;
        }

        sound = (&sounds.sound0)[index];
        if (sound != 0) {
            gDeferredSound &= 0xFFFEF800;
            gEventParam = sound | gDeferredSound;
            event_playSoundCommand();
        }
    }
}

void event_stopSound1(void) {
    sound_stopActorSound1(gCurrentActor);
}

void event_stopSound2(void) {
    sound_stopActorSound2(gCurrentActor);
}


/*
 * Spawns a projectile object of the given kind (gProjectileSpawnOffsets / gProjectileHitValues rows) from the current
 * object, at `pos` (an f32[3]), at the bone from gProjectileSpawnOffsets, or just above the object. `flags`
 * aims it: 1 = no auto-aim, 2 = face the player, 4 = arc onto the player, 8 = random yaw spread,
 * 0x10 = lead the target targetIndex, 0x20 = remember targetIndex. Returns the new object.
 */
u8 *event_spawnProjectile(f32 yaw, f32 speed, f32 yVelocity, f32 gravity, s32 count5, s32 kind, f32 minHeight,
                  f32 scale, f32 *pos, s32 flags, s32 sweep) {
    Actor *obj;
    u16 aimAngle;
    s16 angle;
    s32 slot;
    s32 anim;
    s32 count;
    s32 i;
    MtxF *mtx;
    f32 x;
    f32 y;
    f32 z;
    MtxF mf;

    count = 0;
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type == 4) {
            count++;
        }
    }
    if (count >= 5) {
        obj = actor_findByState(4, &slot);
        actor_remove(obj, 0);
    } else {
        obj = actor_alloc();
        if ((u32)(obj - movement_obj) >= 25 && count > 0) {
            obj = actor_findByState(4, &slot);
            actor_remove(obj, 0);
        }
    }
    obj->spriteTexture = 11;
    obj->type = 4;
    obj->energy = 1;
    obj->langle = gCurrentActor->langle;
    if (pos != 0) {
        obj->xpos = pos[0];
        obj->ypos = pos[1];
        obj->zpos = pos[2];
    } else if (gCurrentActor->ob_matrix_ptr == NULL) {
        obj->xpos = gCurrentActor->xpos;
        obj->ypos = gCurrentActor->ypos + 50.0f;
        obj->zpos = gCurrentActor->zpos;
    } else {
        mtx = gCurrentActor->ob_matrix_ptr;
        if (gBoneMatricesFlushed) {
            guMtxL2F(mf, (Mtx *)&mtx[gProjectileSpawnOffsets[kind][3]]);
            mtx = &mf;
        } else {
            mtx = &mtx[gProjectileSpawnOffsets[kind][3]];
        }
        math_transformPoint(*mtx, gProjectileSpawnOffsets[kind][0], gProjectileSpawnOffsets[kind][1], gProjectileSpawnOffsets[kind][2], &x, &y, &z);
        obj->xpos = x;
        obj->ypos = y;
        obj->zpos = z;
    }
    if (minHeight != 0.0f) {
        if (obj->ypos < gCurrentActor->ypos + minHeight) {
            obj->ypos = gCurrentActor->ypos + minHeight;
        }
    }
    obj->shadow_size = 110;
    obj->unique_id = 0xFF - (obj - movement_obj);
    if (kind == 0 || kind == 1) {
        obj->drawtype = 2;
        obj->unk103 = 1;
    } else {
        obj->drawtype = 5;
        for (i = 0; i <= gScreenLayout; i++) {
            obj->gfxLights[i] = light_allocGfxLights(1);
        }
        if (obj->gfxLights[i - 1] != 0) {
            obj->maxLights = 1;
        }
        obj->ambients = light_allocAmbients();
        switch (kind) {
        case 2:
            obj->vtilt = 260.0f;
            anim = 0x2A;
            obj->unk103 = 1;
            break;
        case 18:
            obj->vtilt = 260.0f;
            anim = 0x22;
            obj->unk103 = 1;
            break;
        case 11:
            anim = 0x6D;
            break;
        case 12:
            anim = 0x6B;
            break;
        case 3:
            anim = 0x39;
            break;
        case 6:
            anim = 0x51;
            break;
        case 15:
            anim = 0x93;
            break;
        case 17:
            anim = 0x39;
            break;
        default:
            anim = 0x50;
            break;
        }
        collision_createPartSet(obj, anim, 1.0f, 0);
    }
    angle = (s16)(yaw * 182.04445f) - 0x4000;
    if (gIsMultiplayer && !(flags & 1) && kind != 5 && kind != 9 && kind != 10 && kind != 4) {
        u8 targetIndex;
        f32 dist;
        u16 rawAngle;
        f32 targetY;

        aimAngle = rawAngle = actor_getDirectionAngle(speed, yVelocity, 0);
        targetIndex = actor_findTargetInCone(obj, &aimAngle, rawAngle >> 8, 0xFF, 20);
        if (targetIndex != 0xFF) {
            targetY = movement_obj[targetIndex].ypos;
            dist = actor_distanceXZ(gCurrentActor, &movement_obj[targetIndex]);
            angle = aimAngle;
            if (kind == 2 || kind == 18) {
                targetY -= 30.0f;
            }
            yVelocity = (targetY - gCurrentActor->ypos) / (dist / speed);
            gravity = 0.0f;
        }
    }
    if (flags & 2) {
        angle = actor_getDirectionAngle(movement_obj[0].xpos - obj->xpos, obj->zpos - movement_obj[0].zpos, 0);
    }
    if (flags & 4) {
        yVelocity = ((movement_obj[0].ypos + 60.0f) - obj->ypos) / (actor_distanceXZ(obj, movement_obj) / speed);
    }
    if (flags & 8) {
        angle += (u16)(math_random() % 0x1200U) - 0x900;
    }
    if (flags & 0x10) {
        f32 pad1; // matching: three unused stack slots
        f32 pad2;
        f32 pad3;
        f32 tx;
        f32 ty;
        f32 tz;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 offx;
        f32 offz;
        f32 offy;
        Actor *target;
        f32 hdist;
        f32 pitch;
        f32 dummy;

        target = &movement_obj[gCurrentActor->targetIndex];
        actor_splitVelocity(target->angle, target->speed * 4.0f, 0.0f, &offx, &offz, &offy);
        tx = target->xpos + offx;
        tz = target->zpos + offz;
        ty = (target->ypos + 30.0f) + target->yspd * 4.0f;
        dx = tx - obj->xpos;
        dy = ty - obj->ypos;
        dz = tz - obj->zpos;
        hdist = sqrtf(dx * dx + dz * dz);
        pitch = math_atan2(dy, hdist) * 10430.378f; /* radians to s16 angle units */
        angle = actor_getDirectionAngle(dx, -dz, 0);
        actor_splitVelocity((s16)pitch, speed, 0.0f, &speed, &yVelocity, &dummy);
        yVelocity = -yVelocity;
    }
    obj->langle = angle;
    obj->angle = angle;
    obj->theta = (s16)((u16)angle + 0x4000) * 0.0054931640625f;
    obj->ygrav = gravity;
    obj->yspd = yVelocity;
    obj->speed = speed;
    obj->count5 = *((u8 *)&count5 + 3); // matching: lbu from the arg slot (a cast emits andi)
    obj->count3 = 0;
    obj->count1 = 0;
    obj->count2 = 0xFF;
    if (flags & 0x20) {
        obj->homingTarget = obj->count2 = gCurrentActor->targetIndex;
    }
    obj->count4 = 0x26;
    if (sweep == 0) {
        obj->doing_hit = gProjectileHitValues[kind] + 1;
    }
    obj->shad = 100.0f;
    obj->doingHitTimer = 255.0f;
    obj->bodyRadius = 5;
    obj->bodyHeight = 5;
    obj->bodyRadiusOverHeight = 1.0f;
    obj->bodyHeightOverRadius = 1.0f;
    obj->hitRadiusOverHeight = 1.0f;
    obj->hitHeightOverRadius = 1.0f;
    obj->owner = dc_glbl_CharNum + 1;
    obj->spriteSize = 0x11;
    obj->intelwalk = 0;
    obj->imune = obj->intelwalk * 0 + 0xFF; // matching: IDO reuses the zero of the unk232 store
    obj->noshove = 0x100E;
    if (sweep) {
        obj->noshove |= 0x8000;
        bzero(obj->mv = allocate_memory(0x1C0, 1, 0, 0), 0x1C0);
        obj->unk103 = 1;
    }
    obj->scale_xz = scale;
    obj->scale_y = scale;
    obj->unk66 = gCurrentActor->unk66 & 0xC;
    obj->headEffectState = kind;
    obj->headEffectOffset[0][0] = sweep;
    if (kind == 2 || kind == 18) {
        obj->headEffectTimer = 60;
    }
    switch (kind) {
    case 1:
        actor_runHitHandler(obj, 1, NULL);
        effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
        break;
    case 15:
        actor_runHitHandler(obj, 13, NULL);
        break;
    case 13:
        sound_playActorSound2(567, obj, 30000, 500, 2000);
        /* fallthrough */
    case 5:
    case 9:
    case 10:
        actor_runHitHandler(obj, 12, NULL);
        break;
    case 0:
        actor_runHitHandler(obj, 8, NULL);
        effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
        break;
    }
    return (u8 *)obj;
}

/* Fires projectile kind gEventParam from the current object, aimed by its facing and camera pitch. */
void event_fireProjectile(void) {
    s32 bone;
    s32 kind;
    f32 elevation;
    f32 heading;
    f32 dist;
    f32 gravity;
    f32 speed;
    f32 yVelocity;
    f32 scale;
    f32 unused;
    MtxF *mtxs;
    f32 x;
    f32 y;
    f32 z;
    f32 boneHeading;
    f32 boneElevation;
    f32 pad; // matching: unused stack slot
    s32 sweep;
    s32 flags;
    f32 *pos;
    f32 origin[3];

    flags = 0;
    gravity = 0.0f;
    elevation = 45.0f;
    dist = 323.0f;
    scale = 0.5f;
    heading = (gCurrentActor->langle + 0x4000) * 0.0054931640625f;
    bone = -1;
    sweep = 0;
    pos = NULL;
    kind = gEventParam;
    if (kind == 5 || kind == 9 || kind == 10) {
        bone = 3;
    }
    if (bone != -1) {
        if (gCurrentActor->ob_matrix_ptr != NULL) {
            mtxs = gCurrentActor->ob_matrix_ptr;
            math_transformPoint(mtxs[bone], 0.0f, 0.0f, 1.0f, &x, &y, &z);
            x -= mtxs[bone][3][0];
            y -= mtxs[bone][3][1];
            z -= mtxs[bone][3][2];
            boneHeading = math_atan2(x, z);
            boneElevation = math_atan2(y, sqrtf(x * x + z * z));
            boneHeading *= 57.295776f;
            boneElevation *= 57.295776f;
            if (boneHeading > 180.0f) {
                boneHeading -= 360.0f;
            }
            if (boneElevation > 180.0f) {
                boneElevation -= 360.0f;
            }
            heading = boneHeading;
            elevation = boneElevation;
        }
    } else if (gCurrentActor->CameraPtr != NULL && gCurrentActor->CameraPtr->Mode == 0x100) {
        elevation = gCurrentActor->CameraPtr->aim.signedPitch;
        if (elevation < 0.0f) {
            elevation = elevation * -0.5f + elevation * -0.2333f + 7.0f;
        } else {
            elevation = elevation * -0.5f + (60.0f - elevation) * 0.133f;
        }
        elevation *= 1.40625f;
    } else {
        elevation = -gCurrentActor->aimPitch;
    }

    switch (kind) {
    case 2:
    case 11:
    case 12:
    case 18:
        sweep = 1;
        break;
    case 15:
        elevation *= 2.2f;
        elevation += 7.0f;
        gravity = 3.3f;
        break;
    case 1:
        gravity = 2.4f;
        break;
    case 3:
        dist = 500.0f;
        math_getHeadingElevation(&movement_obj[0].mv->rayHit.dir, &heading, &elevation);
        origin[0] = movement_obj[0].mv->rayHit.origin.x;
        origin[1] = movement_obj[0].mv->rayHit.origin.y;
        origin[2] = movement_obj[0].mv->rayHit.origin.z;
        sweep = 1;
        pos = origin;
        elevation = -elevation;
        break;
    case 6:
        sweep = 1;
        elevation = 0.0f;
        dist = 500.0f;
        break;
    case 17:
        flags = 0x30;
        elevation = 0.0f;
        dist = 150.0f;
        break;
    case 4:
        dist = 250.0f;
        break;
    case 5:
    case 9:
    case 10:
        sweep = 1;
        gravity = 5.0f;
        dist = 593.0f;
        scale = 1.5f;
        break;
    }
    actor_splitVelocity((s16)(elevation * 182.04445f), dist, 0.0f, &speed, &yVelocity, &unused);
    yVelocity = -yVelocity;
    event_spawnProjectile(heading, speed, yVelocity, gravity, 100, kind, 0.0f, scale, pos, flags, sweep);
}

/* Spawns object gEventParam & 0xFF ahead of the current object, with its marker 5000 units out. */
void event_spawnObjectAhead(void) {
    Actor *obj;
    f32 xOff;
    f32 zOff;
    f32 yOff;
    s32 spawnId;
    s32 angle;
    f32 height;

    spawnId = gEventParam & 0xFF;
    obj = actor_findOnPath(collision_findSpawnById(spawnId));
    if (obj == NULL) {
        return;
    }

    height = 0.0f;
    trail_spawnActorBoneTrail(obj, 0xFF);
    if ((gEventParam & 0xFF00) != 0) {
        height = gCurrentActor->CameraPtr->aim.signedPitch - 15.0f;
    }

    angle = gCurrentActor->langle;
    obj->fangle = angle;
    obj->langle = angle;
    obj->angle = angle;
    actor_splitVelocity(angle, 5000.0f, height, &xOff, &zOff, &yOff);

    gSpawnPaths[obj->listpos][1].x = gCurrentActor->xpos + xOff;
    gSpawnPaths[obj->listpos][1].z = gCurrentActor->zpos + zOff;
    gSpawnPaths[obj->listpos][1].y = gCurrentActor->ypos + yOff;
    obj->attach_char = 0;
    obj->scriptCmd = NULL;

    if ((gEventParam & 0xFF00) != 0) {
        obj->ypos = gCurrentActor->ypos + 70.0f;
        obj->intelwalk = 4;
        obj->scale_xz = 0.25f;
        obj->scale_y = 0.25f;
    } else {
        obj->intelwalk = 2;
        obj->speed = -15.0f;
        obj->yspd = 34.0f;
        obj->ygrav = 1.1f;
        obj->scale_xz = 0.5f;
        obj->scale_y = 0.5f;
    }
}

void event_emptyTimedEvent16(void) {
}

void func_1506D4F4(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh, 0.01f, 0.0f, 0.0f, 0.0f, 0);
}

void event_finishDeathOutsideCutscene(void) {
    if (gCutsceneStates[0] != 1) {
        actor_finishDeath(dc_glbl_CharNum);
    }
}

void event_resetBlink(void) {
    gCurrentActor->blinkTimer = 0;
}

/*
 * face_requestExpression with the two bytes of gEventParam's high half and its low half (0xFFFF = none)
 * scaled by 1 / anim->speed[0].
 */
void event_requestExpression(void) {
    Actor *obj = gCurrentActor;
    f32 divisor;
    u16 timer;
    u8 expression;
    u8 priority;

    if (obj->defaultExpression != 0x2A) {
        expression = gEventParam >> 24;
        priority = (gEventParam >> 16) & 0xFF;
        timer = gEventParam & 0xFFFF;
        divisor = obj->anim->speed[0];
        if (divisor == 0.0f) {
            divisor = 0.01f;
        }
        if (timer != 0xFFFF) {
            timer = timer / divisor;
        }
        face_requestExpression(obj, expression, priority, timer, 0);
    }
}

void event_requestSubmergedExpression(void) {
    Actor *obj = gCurrentActor;
    s32 expression;

    if (obj->waterHeight == -10000.0f) {
        return;
    }
    if (obj->waterHeight < obj->aim.y) {
        return;
    }
    if (obj->energy >= 2) {
        expression = 0x2C;
    } else {
        expression = 0x29;
    }
    gEventParam = (gEventParam & 0xFFFF) | (expression = (expression << 10) << 14); // matching: two-step shift + store to expression
    event_requestExpression();
}

void event_finishClimbUp(void) {
    f32 xOff;
    f32 zOff;
    f32 yOff;

    if (gCurrentActor->inwater == 0) {
        gCurrentActor->ygrav = 5.0f;
        gCurrentActor->yspd = 0.0f;
    }
    gCurrentActor->ypos += 70.0f;
    gCurrentActor->ledgeState = 0;
    gCurrentActor->norun = 0;
    actor_splitVelocity(gCurrentActor->angle, gEventParam, 0, &xOff, &zOff, &yOff);
    actor_playAnim(gCurrentActor, 15, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    gCurrentActor->xpos += xOff;
    gCurrentActor->zpos += zOff;
    gCurrentActor->unk100 |= 2;
    gCurrentActor->shad = 0.0f;
    gCurrentActor->count5 = 20;
    gCurrentActor->mv->camjolt = 12;
    gCurrentActor->mv->side = 10;
}

void event_finishLadderGrabFromTop(void) {
    gCurrentActor->ypos -= 80.0f;
    actor_playAnim(gCurrentActor, 0x297, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    gCurrentActor->unk100 &= 0xFFDF;
    gCurrentActor->count5 = 0;
    gCurrentActor->norun = 0;
    gCurrentActor->mv->ladderFromTop = 0;
    gCurrentActor->mv->camjolt = 12;
}

void func_1506D934(void) {
    gCurrentActor->unk103 = gEventParam;
}

void event_doNothing(void) {
}

void func_1506D958(void) {
    s32 i;
    s32 id = gEventParam & 0xFF;
    s32 flags = (gEventParam >> 8) & 0xFF;
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0 && id == movement_obj[i].unkAB &&
            dc_glbl_CharNum + 1 == movement_obj[i].attach_char) {
            movement_obj[i].unk101 &= ~1;
            movement_obj[i].unk101 |= flags;
            return;
        }
    }
}

void event_setImmune(void) {
    gCurrentActor->imune = gEventParam;
}

void func_1506DA94(void) {
    if (gEventParam != 0) {
        if ((gCurrentActor->stickspeed > 20.0f) && (gCurrentActor->speed > 20.0f)) {
            gCurrentActor->count5 = 0;
        }
    }
    particle_spawnWakePuff(dc_glbl_CharNum, 0, 24, 0, 409, 4, 170, 0xFF, 0);
}

void event_callEmptyScriptCommand(void) {
    gScriptArg0 = gEventParam;
    script_emptyCommand70();
}

void event_setSpeed(void) {
    gCurrentActor->speed = gEventParam;
}

void event_setIntelHitRoute(void) {
    gCurrentActor->intelhitroute = gEventParam;
}

void event_setTurnRates(void) {
    gCurrentActor->moveTurnRate = gEventParam & 0xFF;
    gCurrentActor->faceTurnRate = (gEventParam >> 8) & 0xFF;
}

void event_removeWeaponProp(void) {
    Actor *obj = event_getOwnedObject(gCurrentActor, 0);

    if (obj != NULL) {
        actor_removeWeaponProp(obj, gCurrentActor);
    }
}

void func_1506DC10(void) {
    s32 sound;
    s32 roll;

    if (gCurrentActor->waterHeight - 60.0f < gCurrentActor->ypos || gCurrentActor->waterHeight == -10000.0f) {
        roll = math_random() & 3;
        if (roll >= 2) {
            sound = roll + 0x612;
        } else {
            sound = roll + 0x8F;
        }
    } else {
        sound = 9;
    }
    actor_playUntrackedSound(sound, gCurrentActor);
}

void event_setTaskInt1(void) {
    gCurrentActor->taskint[1] = gEventParam;
}

void event_setTaskInt2(void) {
    gCurrentActor->taskint[2] = gEventParam;
}

void event_setEnergy(void) {
    gCurrentActor->energy = gEventParam;
}

void event_emptyTimedEvent45(void) {
}

/* Sets bits gEventParam in unkF8, or resets it to the spawn record default when 0. */
void event_addNoShove(void) {
    if (gEventParam == 0) {
        gCurrentActor->noshove = gCurrentActor->listpos_ptr->objFlags;
    } else {
        gCurrentActor->noshove |= gEventParam;
    }
}

void event_clearNoShove(void) {
    gCurrentActor->noshove &= ~gEventParam;
}

void event_setMatrixPhysics(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->matrix_physics = gEventParam & 0x7F;
    }
    if (gEventParam == 0) {
        gCurrentActor->angle = gCurrentActor->langle;
    }
}

void event_emptyTimedEvent50(void) {
}

void event_spawnThrottledEmitterIfCameraAboveLimit(void) {
    if (level_isCameraAboveLimit(level) != 0) {
        particle_spawnThrottledEmitter(dc_glbl_CharNum, gEventParam, 0xFF, 0);
    }
}

void event_spawnWakePuffPair(void) {
    s32 i;
    for (i = 0; i < 2; i++) {
        particle_spawnWakePuff(dc_glbl_CharNum, 10, 18, 12, 409, 4, 170, 0xFF, 0);
    }
}


void func_1506DE84(void) {
    switch (gEventParam) {
    case 0:
        if (gCurrentActor->shad == 0.0f) {
            effect_spawnFadeBurst(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                          30, 30, 7, 0.1f, 0.1f);
        }
        return;
    case 1:
        if ((math_random() & 0xFF) < 0x40U) { // 1 in 4
            effect_spawnFadeBurst(gCurrentActor->xpos, gCurrentActor->ypos, gCurrentActor->zpos,
                          10, 20, 7, 0.03f, 0.02f);
            gEventParam = 262;
            event_playOrStopSound2(300, 1800);
        }
        return;
    case 2:
        func_1516F1C0(dc_glbl_CharNum, 16, 0xFF, 0);
        return;
    case 3:
        func_1516F1C0(dc_glbl_CharNum, 40, 0xFF, 0);
        return;
    case 4:
        func_1516F1C0(dc_glbl_CharNum, 40, 0xFF, 0);
        return;
    case 5:
        func_1516F2F8(dc_glbl_CharNum, 30, 20, 2, 0xFF, 0);
        return;
    case 6:
        func_1516F2F8(dc_glbl_CharNum, 18, 12, 2, 0xFF, 0);
        return;
    case 7:
        func_1516F2F8(dc_glbl_CharNum, 60, 120, 2, 0xFF, 0);
        return;
    case 8:
        event_spawnLargeQuadEdgeParticles();
        return;
    case 9:
        particle_fireActorBoneTracer(0);
        return;
    case 10:
        particle_fireActorBoneTracer(1);
        return;
    case 11:
        event_spawnSmallQuadEdgeParticles();
        return;
    case 12:
        func_1516F2F8(dc_glbl_CharNum, 18, 12, 0, 0xFF, 0);
        return;
    case 13:
        particle_freeCharSpriteEmitters();
        break;
    }
}

/* Passes the corners of a +-270 square in the object's root matrix, at height unk180, to particle_spawnQuadEdgeStyleParticles. */
void event_spawnLargeQuadEdgeParticles(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 height;
    f32 width;
    f32 depth;
    s32 i;
    f32 vertices[4][3];
    f32 posX;
    f32 posZ;

    if (gCurrentActor->ob_matrix_ptr != NULL) {
        width = 270.0f;
        depth = width;
        x = width;
        z = depth;
        height = gCurrentActor->floorY;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            math_transformPoint(*gCurrentActor->ob_matrix_ptr, x, 0.0f, z, &x, &y, &z);
            vertices[i][0] = x;
            vertices[i][1] = height;
            vertices[i][2] = z;
            effect_spawnFadeBurst(x, y, z, 30, 20, 7, 0.13f, 0.1f);
            if (i == 0) {
                z = depth;
                x = -width;
            } else if (i == 1) {
                x = -width;
                z = -depth;
            } else if (i == 2) {
                x = width;
                z = -depth;
            }
        }
        posX = gCurrentActor->xpos;
        posZ = gCurrentActor->zpos;
        particle_spawnQuadEdgeStyleParticles(vertices, posX, posZ, 4, gCurrentActor->scale_xz, 0, 4, 4, 0xFF, 0);
    }
}

/* Same as event_spawnLargeQuadEdgeParticles with a +-100 quad at half scale. */
void event_spawnSmallQuadEdgeParticles(void) {
    f32 x;
    f32 y;
    f32 z;
    f32 halfX;
    f32 halfZ;
    f32 height;
    s32 i;
    f32 verts[4][3];
    f32 posX;
    f32 posZ;

    if (gCurrentActor->ob_matrix_ptr != NULL) {
        halfX = 100.0f;
        halfZ = 100.0f;
        height = gCurrentActor->floorY;
        x = halfX;
        z = halfX;
        for (i = 0; i < 4; i++) {
            y = 0.0f;
            math_transformPoint(*gCurrentActor->ob_matrix_ptr, x, y, z, &x, &y, &z);
            verts[i][0] = x;
            verts[i][1] = height;
            verts[i][2] = z;
            if (i == 0) {
                z = halfX;
                x = -halfX;
            } else if (i == 1) {
                x = -halfX;
                z = -halfZ;
            } else if (i == 2) {
                x = halfX;
                z = -halfX;
            }
        }
        posX = gCurrentActor->xpos;
        posZ = gCurrentActor->zpos;
        particle_spawnQuadEdgeStyleParticles(verts, posX, posZ, 5, gCurrentActor->scale_xz * 0.5f, 0, 4, 4, 0xFF, 0);
    }
}
/*
 * Resolves a sound command in place: bit 21 skips it while sound_isActorSound2Playing / sound_isActorSound1Playing /
 * mp3_getPlayState (by `mode`) reports busy, ids >= 2000 come from the object's gSpawnModelData table,
 * the top byte is a play chance out of 256 and bits 16-20 a count of random variants.
 * Returns 0 if nothing should play.
 */
s32 event_resolveSoundCommand(Actor *obj, s32 *command, s32 mode) {
    s32 pad0; // matching: stack layout (pad0 / pad[3] / inner pad)
    u32 cmd;
    s32 pad[3];
    u32 id;
    u32 entry;
    u32 hi;

    cmd = *command;
    id = *command & 0x7FFF;
    if (cmd & 0x200000) {
        switch (mode) {
        case 0:
            if (sound_isActorSound2Playing(obj) != 0) {
                return 0;
            }
            break;
        case 1:
            if (sound_isActorSound1Playing(obj) != 0) {
                return 0;
            }
            break;
        case 2:
            if (mp3_getPlayState() == 1) {
                return 0;
            }
            break;
        }
    }
    if (id >= 2000) {
        entry = gSpawnModelData[obj->modelTypeIndex][id - 2000];
        if (entry == 0) {
            return 0;
        }
        cmd &= 0xFF200000;
        if (cmd != 0) {
            cmd |= entry & 0xFFFFFF;
        } else {
            cmd |= entry;
        }
    }
    id = cmd >> 16;
    if (id != 0) {
    lab: // matching: an unused label splits the block, which steers as1
        if ((cmd >> 24) >= (math_random() & 0xFF)) {
            id &= 0x1F;
            cmd &= 0x7FFF;
            if (id) {
                s32 pad;

                cmd = sound_pickVariant(cmd, id | 0); // matching: | 0 changes register allocation
            }
        } else {
            return 0;
        }
    }
    *command = cmd;
    return 1;
}

void event_playSound2Range1800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(300, 1800);
    }
}

void event_playSound2Range800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(100, 800);
    }
}

void event_playSound2Range1300(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(190, 1300);
    }
}

void event_playSound2Range3000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(300, 3000);
    }
}

void event_playSound2Range4000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(700, 4000);
    }
}

void event_playSound2Range8000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playOrStopSound2(3000, 8000);
    }
}

void event_playQuietSound2Range1800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playQuietSound2(300, 1800);
    }
}

void event_playQuietSound2Range3000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) {
        event_playQuietSound2(300, 3000);
    }
}

void event_playSound2OnGroundUnlessIvehitit(void) {
    if (gCurrentActor->shad == 0.0f) {
        if (gCurrentActor->ivehitit == 0) {
            event_playSound2Range1800();
        }
    }
}

void event_playSound2IfNotPlaying(void) {
    if ((event_resolveSoundCommand(gCurrentActor, &gEventParam, 0) != 0) && (sound_isSoundPlaying(gEventParam) == 0)) {
        event_playOrStopSound2(80, 800);
    }
}

void event_startMp3TrackRange1800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 2) != 0) {
        event_startMp3Track(300, 1800);
    }
}

void event_playSound1Range1800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(300, 1800);
    }
}

void event_playSound1Range800(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(100, 800);
    }
}

void event_playSound1Range1300(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(190, 1300);
    }
}

void event_playSound1Range3000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(300, 3000);
    }
}

void event_playSound1Range4000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(700, 4000);
    }
}

void event_playSound1Range8000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playOrStopSound1(3000, 8000);
    }
}

void event_playQuietSound1Range3000(void) {
    if (event_resolveSoundCommand(gCurrentActor, &gEventParam, 1) != 0) {
        event_playQuietSound1(300, 3000);
    }
}

/* Throws the object held in unk13C (object index + 100) along the current object's facing. */
void event_throwHeldObject(void) {
    u8 index;
    Actor *held;

    index = gCurrentActor->lifting - 100;
    if (gCurrentActor->lifting != 0) {
        held = &movement_obj[index];
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].carried = 0;
            movement_obj[index].speed = 18.0f;
            movement_obj[index].yspd = 40.0f;
            movement_obj[index].ygrav = 6.0f;
            movement_obj[index].angle = gCurrentActor->langle;
            movement_obj[index].attach_char = 0;
            if (movement_obj[index].character_type == 0x20) {
                movement_obj[index].noshove |= 0x200;
                movement_obj[index].yspd = 0.0f;
                movement_obj[index].ygrav = 0.0f;
                movement_obj[index].speed = 0.0f;
            }
            movement_obj[index].stunned = 0xFE;
            movement_obj[index].stunnednum = 0;
            movement_obj[index].stunnedseq = actor_findReaction(12, held);
            movement_obj[index].animseq.uh = 0xFFFF;
            movement_obj[index].fallStartY = 1000000.0f;
            actor_advanceReaction(index, held);
            movement_obj[index].task_attrib |= 0x40;
        }
        gCurrentActor->lifting = 0;
    }
}

/* Drops the object held in unk13C (object index + 100). */
void event_dropHeldObject(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting) {
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].attach_char = 0;
            movement_obj[index].carried = 0;
            movement_obj[index].speed = 0.0f;
            movement_obj[index].yspd = 0.0f;
            movement_obj[index].ygrav = 6.0f;
            movement_obj[index].fallStartY = 1000000.0f;
        }
        gCurrentActor->lifting = 0;
    }
}

void func_1506EC50(void) {
    if (gCurrentActor->shad > 0.2f) {
        if ((gEventParam & 0x8000) == 0 || movement_obj[0].ypos < gCurrentActor->ypos) {
            gEventResult = gEventParam & 0xFF;
            gEventLoopRunning = 0;
            gCurrentActor->speccnt -= 1;
        }
    }
}

void event_holdAnimLastFrame(void) {
    gCurrentActor->anim->anim[0] |= 0x8000;
}

void event_setHopSpeed(void) {
    gCurrentActor->hopSpeed = gEventParam;
}

void func_1506ED0C(void) {
    gCurrentActor->count5 = gEventParam;
    gCurrentActor->unk100 |= 0x10;
    gCurrentActor->mv->prop = 1;
}

void func_1506ED4C(void) {
    gCurrentActor->unk100 = gEventParam;
}

void func_1506ED68(void) {
    gCurrentActor->unkA9 = 5;
    gCurrentActor->mv->prop = 0;
}

void event_setScriptTimer(void) {
    gCurrentActor->scriptTimer = gEventParam;
}

void event_setMvmtControl(void) {
    gCurrentActor->mvmt_control = gEventParam;
}

void event_clearMvmtControl(void) {
    gCurrentActor->mvmt_control &= ~gEventParam;
}

void event_addMvmtControl(void) {
    gCurrentActor->mvmt_control |= gEventParam;
}

void event_addTaskAttrib(void) {
    gCurrentActor->task_attrib |= gEventParam;
}

void event_clearTaskAttrib(void) {
    gCurrentActor->task_attrib &= ~gEventParam;
}

void event_setAfterimage(void) {
    s32 param = gEventParam;
    s32 duration = gEventParam & 0xFFFF;

    if (param != 0) {
        trail_startActorAfterimage(gCurrentActor, duration, gEventParam >> 16);
        return;
    }
    trail_fadeActorAfterimage(gCurrentActor);
}

void event_setAfterimageHiddenParts(void) {
    trail_setActorAfterimageHiddenParts(gCurrentActor, gEventParam);
}

void event_setWallHitScript(void) {
    gCurrentActor->wallHitScript = gEventParam;
}

void event_startHeadEffect(void) {
    gCurrentActor->headEffect = (gEventParam >> 16) & 0xFFFF;
    gCurrentActor->headEffectState = (gEventParam >> 24) & 0xFF;
    gCurrentActor->headEffectTimer = gEventParam & 0xFFFF;
    gCurrentActor->headEffectOffset[0][0] = 0;
    gCurrentActor->headEffectOffset[0][1] = 0;
    gCurrentActor->headEffectOffset[1][0] = 0;
    gCurrentActor->headEffectOffset[1][1] = 0;
}

/* Starts head effect 5 with no time limit, storing the byte pair (bits 8-15, 0-7) in slot (bits 16-23) of
   headEffectOffset. */
void event_setHeadEffectOffset(void) {
    s32 index;
    s16 first;
    s16 second;

    index = (gEventParam >> 16) & 0xFF;
    first = (gEventParam >> 8) & 0xFF;
    second = gEventParam & 0xFF;
    gCurrentActor->headEffectTimer = 0xFFFF;
    gCurrentActor->headEffect = 5;
    index *= 2;
    (&gCurrentActor->headEffectOffset[0][0])[index] = first;
    (&gCurrentActor->headEffectOffset[0][1])[index] = second;
}

void event_stopHeadEffect(void) {
    gCurrentActor->headEffectTimer = 0;
}

void func_1506EFC8(s32 cmd) {
    if (gCurrentActor->character_type != 0x8C) {
        func_150BB760(gCurrentActor);
    }
}

void func_1506F004(s32 cmd) {
    func_150BCBBC(gCurrentActor);
}

void event_attachOwnerEmitter(s32 cmd) {
    particle_attachOwnerEmitter(gCurrentActor, 0xFF, 0);
}

void event_startDustEmitter50(s32 cmd) {
    particle_startObjDustEmitter(gCurrentActor, 50, 0xFF, 1);
}

void event_startDustEmitter(s32 cmd) {
    particle_startObjDustEmitter(gCurrentActor, -1, 0xFF, 1);
}

void event_spawnFoot0Dust(s32 cmd) {
    particle_spawnFootDust(gCurrentActor, 0);
}

void event_spawnFoot1Dust(s32 cmd) {
    particle_spawnFootDust(gCurrentActor, 1);
}

void event_spawnSide0StepEffects(s32 cmd) {
    effect_spawnJointStepEffects(gCurrentActor, 0, 0xFF);
}

void event_spawnSide1StepEffects(s32 cmd) {
    effect_spawnJointStepEffects(gCurrentActor, 1, 0xFF);
}

void event_sprayTrails(s32 cmd) {
    trail_sprayFromRing(gCurrentActor, 0xFF);
}

void event_spawnBone12Flame(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;
    Actor *obj;

    obj = gCurrentActor;
    descriptor.seed.owner = obj;
    descriptor.seed.ownerId = obj->unique_id;
    descriptor.seed.startOffset.z = 80.0f;
    descriptor.seed.endOffset.z = 102.0f;
    descriptor.seed.gravity = 0.1223f;
    descriptor.seed.emitRate = 0.6f;
    descriptor.seed.maxLength = 1200.0f;
    descriptor.seed.fadeStart = 0.656f;
    descriptor.seed.size = 26.0f;
    descriptor.seed.sizeRate = 4.8f;
    descriptor.seed.texSpeed = -187.0f;
    descriptor.seed.texScale = 4.64f;
    descriptor.line.rate = 0.00502f;
    descriptor.line.rateRange = 0.003f;
    descriptor.line.widthMin = 0.606f;
    descriptor.line.widthRange = 0.418f;
    descriptor.line.heightMin = 0.23900001f;
    descriptor.line.heightRange = 0.20500001f;
    descriptor.line.speedMin = 1.177f;
    descriptor.line.speedRange = 0.572f;
    descriptor.line.gravityMin = 0.81900007f;
    descriptor.line.gravityRange = 0.846f;
    descriptor.line.dragA = 0.94086f;
    descriptor.line.dragB = 0.942411f;
    descriptor.line.position = 0.39200002f;
    descriptor.line.shrinkRate = 0.008f;
    descriptor.scatter.rate = 0.24900001f;
    descriptor.scatter.rateRange = 0.35200003f;
    descriptor.scatter.halfWidthMin = 14.0f;
    descriptor.scatter.halfWidthRange = 9.0f;
    descriptor.scatter.spread = 25.0f;
    descriptor.scatter.gravityMin = 0.44000003f;
    descriptor.scatter.gravityRange = 0.45100003f;
    descriptor.emit.rate = 0.30900002f;
    descriptor.seed.bone = 12;
    descriptor.seed.alpha = 200;
    descriptor.emit.rateRange = 0.312f;
    descriptor.seed.initFn = -1;
    descriptor.seed.attackFn = 2;
    descriptor.seed.destroyFn = -1;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 1;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.emit.sizeMin = 402.0f;
    descriptor.line.lifeMin = 30;
    descriptor.line.lifeRange = 15;
    descriptor.line.fadeStart = 12;
    descriptor.line.alphaPerLife = 21;
    descriptor.line.alphaMin = 80;
    descriptor.line.alphaRange = 120;
    descriptor.scatter.itemCountMin = 4;
    descriptor.scatter.itemCountRange = 2;
    descriptor.scatter.lifeMin = 30;
    descriptor.scatter.lifeRange = 17;
    descriptor.seed.startOffset.x = 0.0f;
    descriptor.seed.startOffset.y = 0.0f;
    descriptor.seed.endOffset.x = 0.0f;
    descriptor.seed.endOffset.y = 0.0f;
    descriptor.seed.soundFn = 0;
    descriptor.seed.soundUpdateFn = 0;
    descriptor.seed.soundStopFn = 0;
    descriptor.scatter.target.x = 0.0f;
    descriptor.scatter.target.y = 5.0f;
    descriptor.scatter.target.z = 100.0f;
    descriptor.scatter.speedMin = 0.5f;
    descriptor.scatter.speedRange = 0.5f;
    descriptor.scatter.alphaMin = 80;
    descriptor.scatter.alphaRange = 120;
    descriptor.scatter.fadeStart = 16;
    descriptor.scatter.fadeRate = 15;
    descriptor.emit.target.x = 0.0f;
    descriptor.emit.target.y = 5.0f;
    descriptor.emit.target.z = 100.0f;
    descriptor.emit.lifeMin = 40;
    descriptor.emit.lifeRange = 15;
    descriptor.emit.alphaMin = 80;
    descriptor.emit.alphaRange = 80;
    descriptor.emit.fadeStart = 20;
    descriptor.emit.fadeRate = 12;
    descriptor.emit.sizeRange = 303.0f;
    descriptor.emit.speedMin = 0.101f;
    descriptor.emit.speedRange = 0.19800001f;
    descriptor.emit.spread = 45.0f;
    descriptor.emit.gravityMin = -0.65500003f;
    descriptor.emit.gravityRange = 0.30800003f;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void event_stopFlamethrowerJets(s32 cmd) {
    flamethrower_stopJets(gCurrentActor);
}

void event_spawnBone13Flame(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;

    descriptor.seed.owner = gCurrentActor;
    descriptor.seed.ownerId = gCurrentActor->unique_id;
    descriptor.seed.bone = 13;
    descriptor.seed.startOffset.x = 22.0f;
    descriptor.seed.startOffset.y = 22.0f;
    descriptor.seed.startOffset.z = 165.0f;
    descriptor.seed.endOffset.x = 22.0f;
    descriptor.seed.endOffset.y = 22.0f;
    descriptor.seed.endOffset.z = 193.0f;
    descriptor.seed.gravity = 1.138f;
    descriptor.seed.emitRate = 0.795f;
    descriptor.seed.maxLength = 370.0f;
    descriptor.seed.fadeStart = 0.615f;
    descriptor.seed.size = 17.800001f;
    descriptor.seed.sizeRate = 4.9f;
    descriptor.seed.alpha = 159;
    descriptor.seed.initFn = 0;
    descriptor.seed.attackFn = -1;
    descriptor.seed.destroyFn = 0;
    descriptor.seed.soundFn = 0;
    descriptor.seed.soundUpdateFn = 0;
    descriptor.seed.soundStopFn = 0;
    descriptor.seed.texSpeed = -191.0f;
    descriptor.seed.texScale = 7.85f;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 1;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.line.rate = 0.0029f;
    descriptor.line.rateRange = 0.00235f;
    descriptor.line.lifeMin = 35;
    descriptor.line.lifeRange = 10;
    descriptor.line.widthMin = 0.80200005f;
    descriptor.line.widthRange = 0.367f;
    descriptor.line.heightMin = 0.58900005f;
    descriptor.line.heightRange = 0.307f;
    descriptor.line.speedMin = 0.614f;
    descriptor.line.speedRange = 0.29900003f;
    descriptor.line.gravityMin = 0.0f;
    descriptor.line.gravityRange = 0.73800003f;
    descriptor.line.dragA = 0.962646f;
    descriptor.line.dragB = 0.961868f;
    descriptor.line.fadeStart = 15;
    descriptor.line.alphaPerLife = 17;
    descriptor.line.position = 0.36900002f;
    descriptor.line.alphaMin = 80;
    descriptor.line.alphaRange = 80;
    descriptor.line.shrinkRate = 0.024f;
    descriptor.scatter.target.x = 22.0f;
    descriptor.scatter.target.y = 58.0f;
    descriptor.scatter.target.z = 212.0f;
    descriptor.scatter.rate = 0.2f;
    descriptor.scatter.rateRange = 0.105000004f;
    descriptor.scatter.itemCountMin = 3;
    descriptor.scatter.itemCountRange = 2;
    descriptor.scatter.lifeMin = 25;
    descriptor.scatter.lifeRange = 10;
    descriptor.scatter.halfWidthMin = 6.0f;
    descriptor.scatter.halfWidthRange = 10.0f;
    descriptor.scatter.speedMin = 0.3f;
    descriptor.scatter.speedRange = 0.2f;
    descriptor.scatter.spread = 37.0f;
    descriptor.scatter.gravityMin = 0.75100005f;
    descriptor.scatter.gravityRange = 0.32200003f;
    descriptor.scatter.alphaMin = 80;
    descriptor.scatter.alphaRange = 80;
    descriptor.scatter.fadeStart = 15;
    descriptor.scatter.fadeRate = 17;
    descriptor.emit.target.x = 22.0f;
    descriptor.emit.target.y = 130.0f;
    descriptor.emit.target.z = 306.0f;
    descriptor.emit.rate = 0.208f;
    descriptor.emit.rateRange = 0.193f;
    descriptor.emit.lifeMin = 35;
    descriptor.emit.lifeRange = 15;
    descriptor.emit.alphaMin = 80;
    descriptor.emit.alphaRange = 100;
    descriptor.emit.sizeMin = 198.0f;
    descriptor.emit.sizeRange = 102.0f;
    descriptor.emit.speedMin = 0.040000003f;
    descriptor.emit.speedRange = 0.040000003f;
    descriptor.emit.spread = 89.0f;
    descriptor.emit.gravityMin = -0.49500004f;
    descriptor.emit.gravityRange = 0.44300002f;
    descriptor.emit.fadeStart = 15;
    descriptor.emit.fadeRate = 17;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void event_callEmptyActorEventHook(s32 cmd) {
    effect_emptyActorEventHook(gCurrentActor, 0, 0);
}

void func_1506F8F0(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x14, -1, 482.0f, -127.0f, -45.0f, 659.0f, -211.0f, -114.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void func_1506F9C0(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x14, -1, 241.0f, -127.0f, -45.0f, 418.0f, -211.0f, -114.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void event_spawnBone26Tracer(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 0x1A, -1, -181.0f, -218.0f, -1.0f, -681.0f, -584.0f, -2.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 0, 0.0f);
}

void event_fireJointShot(s32 cmd) {
    if (screen_getPlayer0MatrixPhysics() != 0) {
        weapon_fireJointShotProjectile(gCurrentActor, cmd - 0x13, 0xFF, 0);
    } else {
        weapon_fireJointShot(gCurrentActor, cmd - 0x13, 0, 0, -1, 0xFF, 0);
    }
}

void event_startSparkEmitter70(s32 cmd) {
    particle_startSparkEmitter(gCurrentActor, 0x46, 0xFF, 0);
}

void event_startSparkEmitter40(s32 cmd) {
    particle_startSparkEmitter(gCurrentActor, 0x28, 0xFF, 0);
}

void event_cancelTrailPair(s32 cmd) {
    trail_cancelTrailPair();
}

void event_queueTrailPair(s32 cmd) {
    trail_queueTrailPair(gCurrentActor);
}

void event_spawnBone25StreakBurst(s32 cmd) {
    effect_spawnBone25StreakBurst(gCurrentActor, 0xFF);
}

void event_spawnJetPreset6(s32 cmd) {
    flamethrower_spawnJet(gCurrentActor, 6, 0xFF, 0);
}

void event_spawnJetPreset5(s32 cmd) {
    flamethrower_spawnJet(gCurrentActor, 5, 0xFF, 0);
}

void event_spawnBone1Tracer(s32 cmd) {
    beam_spawnBoneTracerEx(gCurrentActor, gCurrentActor->unique_id, 1, -1, 0.0f, 0, 345.0f, 0.0f, 0, 414.0f, 3, 3, 5, 20.0f, (math_randomFloat() * 10.0f) + 40.0f, 1, 50.0f);
}

void event_spawnGroundMark0(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 0);
}

void event_spawnGroundMark1(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 1);
}

void event_spawnGroundMark0Alt(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 0);
}

void event_spawnGroundMark1Alt(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 1);
}

void event_spawnGroundMark2(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 2);
}

void event_spawnGroundMark3(s32 cmd) {
    effect_spawnBoneGroundMark(gCurrentActor, 3);
}

void func_1506FEF8(s32 cmd) {
    func_151AB920(gCurrentActor, 0);
}

void func_1506FF24(s32 cmd) {
    func_151AB920(gCurrentActor, 1);
}

void event_spawnSparkEmitterTask(s32 cmd) {
    effect_spawnSparkEmitterTask(gCurrentActor);
}

void event_spawnBoneGroundBurst0(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 0, 1, 0xFF);
}

void event_spawnBoneGroundBurst1(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 1, 1, 0xFF);
}

void event_spawnBoneGroundBurst2(s32 cmd) {
    particle_spawnBoneGroundBurst(gCurrentActor, 2, 1, 0xFF);
}

void event_spawnImpactRing(s32 cmd) {
    effect_spawnImpactRing(gCurrentActor);
}

void event_spawnJointImpact(s32 cmd) {
    effect_spawnJointImpact(gCurrentActor, 0);
    effect_startJointEmitterTask(gCurrentActor, 0, 0x3E8, 0xFF, 0);
}

void event_spawnBloodGroundBurst(s32 cmd) {
    blood_spawnGroundBurst(gCurrentActor, 0xFF, 1);
}

void event_spawnLandingBurst(s32 cmd) {
    effect_spawnLandingBurst(gCurrentActor, 0xFF, 1);
}

void event_spawnBoneModelBurst(s32 cmd) {
    particle_spawnBoneModelBurst(gCurrentActor, 0xFF, 1);
}

void event_attachBloodStrip(s32 cmd) {
    particle_attachBloodStrip(gCurrentActor, 0xFF, 1);
}

void event_fireBullet(s32 cmd) {
    IgnoreList ignore;
    s32 impactType;

    ignore.count = 1;
    ignore.objs[1] = NULL;
    ignore.objs[2] = NULL;
    ignore.objs[0] = gCurrentActor;

    if (level == LEVEL_WMP_BEACH) {
        impactType = 3;
    } else {
        impactType = 0;
    }
    weapon_fireBullet(gCurrentActor, NULL, &ignore, impactType, 0xFF, 1);
    weapon_rumbleShooter(gCurrentActor);
}

void event_fireSegmentShot(s32 cmd) {
    weapon_fireSegmentShot(gCurrentActor, 0xFF, 1);
}

void event_explodeSightAtActor(s32 cmd) {
    sight_explodeAtActor(gCurrentActor, 0xFF, 1);
}

void event_spawnShortLight(s32 cmd) {
    LightEffectHeader header;
    Vec3i position;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (math_random() % 5U) + 4;
    header.lightType = 0;

    position.x = gCurrentActor->xpos;
    position.y = gCurrentActor->ypos;
    position.z = gCurrentActor->zpos;

    light_createWithEffect(&header, &position, (math_random() % 3U) + 4, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
}


/* Objects 0x77 / 0x28: fires (weapon_fireBullet, else event_fireProjectile), then spawns effect_spawnBoneBeamParticle at bone 4. */
void event_fireTankGun(s32 cmd) {
    u8 useBullet;
    Actor *shooter;
    u8 impactType;
    Actor *aimer;
    u8 idx;
    IgnoreList ignore;
    Vec3f localPos;
    Vec3f endPos;
    ParticleSize randoms;
    Vec3f worldPos;
    const Vec3f *srcs[1];
    Vec3f *dsts[1];

    aimer = NULL;
    if (gCurrentActor->character_type == 0x77) {
        impactType = 1;
        shooter = gCurrentActor;
        idx = gCurrentActor - movement_obj;
        if (idx < 2 && gPartnerSlots[idx] != 0) {
            useBullet = 1;
            aimer = &movement_obj[gPartnerSlots[idx]];
            goto done; // matching: an else branch drops the dead useBullet = 0 below
        }
        useBullet = 0;
        if (shooter->mv->SniperState != 0) {
            useBullet = 1;
        } else {
            useBullet = 0;
        }
    done:;
    } else if (gCurrentActor->character_type == 0x28) {
        shooter = event_getFirstLiveOwnedObject(gCurrentActor);
        impactType = 0;
        useBullet = 0;
        if (shooter->mv->SniperState != 0) {
            useBullet = 1;
        } else {
            useBullet = 0;
        }
    } else {
        return;
    }
    weapon_rumbleShooter(shooter);
    if (aimer != NULL) {
        weapon_rumbleShooter(aimer);
    }
    if (gIsMultiplayer != 0) {
        if (gCurrentActor->mv->unk128 & 0x10) {
            gEventParam = 10;
            impactType = 2;
            gCurrentActor->mv->itemRemaining--;
        } else {
            gEventParam = 9;
        }
    } else {
        gEventParam = 5;
    }
    if (useBullet) {
        ignore.count = 3;
        ignore.objs[0] = shooter;
        ignore.objs[1] = aimer;
        ignore.objs[2] = gCurrentActor;
        weapon_fireBullet(shooter, aimer, &ignore, impactType, 0xFF, 1);
    } else {
        event_fireProjectile();
    }
    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }
    if ((gCurrentActor->cullMask & 0xF) == 0xF) {
        return;
    }
    localPos = gTankMuzzleStart;
    endPos = gTankMuzzleEnd;
    randoms.width = (math_randomFloat() * 30.0f) + 40.0f;
    randoms.height = (math_randomFloat() * 2.512f) + 3.5900002f;
    srcs[0] = &localPos;
    dsts[0] = &worldPos;
    math_transformOffsets(srcs, dsts, &gCurrentActor->ob_matrix_ptr[4], 1);
    effect_spawnBoneBeamParticle(gCurrentActor, 4, &localPos, &endPos, &randoms,
                  (math_random() % 5U) + 5, (math_random() % 56U) + 200,
                  (math_randomFloat() * 510.0f) + 1296.0f,
                  &worldPos, 0xFF, 0, -1, 0xFF, 1);
    weapon_spawnShortLight(&worldPos, 0xFF, 1);
}

void event_fireBeamSlot(s32 cmd) {
    beam_fireSlot(gCurrentActor, (cmd - 0x3E) & 0xFF, cmd);
}

void event_releaseBeamSlot(s32 cmd) {
    beam_releaseSlot(gCurrentActor, (cmd - 0x3E) & 0xFF, cmd);
}

void event_fireSlotProjectiles(s32 cmd) {
    if ((gCurrentActor->hiddenParts & 0x10) == 0) {
        beam_fireSlotProjectile(gCurrentActor, 0, 0xFF, 1);
    }
    if ((gCurrentActor->hiddenParts & 8) == 0) {
        beam_fireSlotProjectile(gCurrentActor, 1, 0xFF, 1);
    }
}

void event_fireMuzzle0(s32 cmd) {
    weapon_fireTwinMuzzleShot(gCurrentActor, 0, 0xFF, 1);
}

void event_fireMuzzle1(s32 cmd) {
    weapon_fireTwinMuzzleShot(gCurrentActor, 1, 0xFF, 1);
}

void event_fireLauncher(s32 cmd) {
    weapon_fireLauncherProjectile(gCurrentActor, 0xFF, 1);
}

void event_runLauncherMuzzle(s32 cmd) {
    weapon_runLauncherMuzzle(gCurrentActor, 0xFF, 1, 0, 0);
}

void event_fireBonePointProjectile(s32 cmd) {
    effect_fireBonePointProjectile(gCurrentActor, 0xFF, 1);
}

void event_spawnBonePointShotFx(s32 cmd) {
    effect_spawnBonePointShotFx(gCurrentActor, 0xFF, 1, 0, 0);
}

void event_spawnBone9Flame(s32 cmd) {
    CharacterFlamethrowerSetup descriptor;

    descriptor.seed.owner = gCurrentActor;
    descriptor.seed.ownerId = gCurrentActor->unique_id;
    descriptor.seed.bone = 9;
    descriptor.seed.startOffset.x = -4.734001f;
    descriptor.seed.startOffset.y = -111.957596f;
    descriptor.seed.startOffset.z = 33.473396f;
    descriptor.seed.endOffset.x = -8.837997f;
    descriptor.seed.endOffset.y = -159.75f;
    descriptor.seed.endOffset.z = 33.473396f;
    descriptor.seed.gravity = 0.278f;
    descriptor.seed.emitRate = 0.82800007f;
    descriptor.seed.maxLength = 312.0f;
    descriptor.seed.fadeStart = 0.325f;
    descriptor.seed.size = 7.1f;
    descriptor.seed.sizeRate = 5.0f;
    descriptor.seed.alpha = 180;
    descriptor.seed.initFn = -1;
    descriptor.seed.attackFn = 1;
    descriptor.seed.destroyFn = -1;
    descriptor.seed.soundFn = -1;
    descriptor.seed.soundUpdateFn = -1;
    descriptor.seed.soundStopFn = -1;
    descriptor.seed.texSpeed = -232.0f;
    descriptor.seed.texScale = 9.07f;
    descriptor.seed.updateFn = -1;
    descriptor.kind = 15;
    descriptor.flags = 17;
    descriptor.red = 60;
    descriptor.green = 255;
    descriptor.blue = 235;
    descriptor.alpha = 82;
    descriptor.line.rate = 0.00689f;
    descriptor.line.rateRange = 0.01038f;
    descriptor.line.lifeMin = 25;
    descriptor.line.lifeRange = 10;
    descriptor.line.widthMin = 0.367f;
    descriptor.line.widthRange = 0.256f;
    descriptor.line.heightMin = 0.35000002f;
    descriptor.line.heightRange = 0.20500001f;
    descriptor.line.speedMin = 0.776f;
    descriptor.line.speedRange = 0.81100005f;
    descriptor.line.gravityMin = 0.125f;
    descriptor.line.gravityRange = 1.3110001f;
    descriptor.line.dragA = 0.935119f;
    descriptor.line.dragB = 0.943452f;
    descriptor.line.fadeStart = 12;
    descriptor.line.alphaPerLife = 21;
    descriptor.line.position = 0.307f;
    descriptor.line.alphaMin = 80;
    descriptor.line.alphaRange = 80;
    descriptor.line.shrinkRate = 0.002f;
    descriptor.scatter.target.x = -8.837997f;
    descriptor.scatter.target.y = -159.75f;
    descriptor.scatter.target.z = 33.473396f;
    descriptor.scatter.rate = 0.3f;
    descriptor.scatter.rateRange = 0.40100002f;
    descriptor.scatter.itemCountMin = 4;
    descriptor.scatter.itemCountRange = 2;
    descriptor.scatter.lifeMin = 26;
    descriptor.scatter.lifeRange = 16;
    descriptor.scatter.halfWidthMin = 6.0f;
    descriptor.scatter.halfWidthRange = 5;
    descriptor.scatter.speedMin = 0.3f;
    descriptor.scatter.speedRange = 0.3f;
    descriptor.scatter.spread = 50.0f;
    descriptor.scatter.gravityMin = 0.0f;
    descriptor.scatter.gravityRange = 0.712f;
    descriptor.scatter.alphaMin = 80;
    descriptor.scatter.alphaRange = 120;
    descriptor.scatter.fadeStart = 15;
    descriptor.scatter.fadeRate = 17;
    descriptor.emit.target.x = -8.837997f;
    descriptor.emit.target.y = -159.75f;
    descriptor.emit.target.z = 33.473396f;
    descriptor.emit.rate = 0.20400001f;
    descriptor.emit.rateRange = 0.504f;
    descriptor.emit.lifeMin = 30;
    descriptor.emit.lifeRange = 15;
    descriptor.emit.alphaMin = 80;
    descriptor.emit.alphaRange = 80;
    descriptor.emit.sizeMin = 140.0f;
    descriptor.emit.sizeRange = 182.0f;
    descriptor.emit.speedMin = 0.16000001f;
    descriptor.emit.speedRange = 0.113000005f;
    descriptor.emit.spread = 42.0f;
    descriptor.emit.gravityMin = -0.518f;
    descriptor.emit.gravityRange = 0.37600002f;
    descriptor.emit.fadeStart = 10;
    descriptor.emit.fadeRate = 25;
    flamethrower_spawnFlame(0, &descriptor, 0xFF, 0);
}

void event_stopFlames(s32 cmd) {
    flamethrower_stopFlames(gCurrentActor);
}

void event_spawnImpactBlast(u8 variant) {
    Vec3f pos;
    Actor *obj;
    Actor *owner;

    obj = gCurrentActor;
    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;

    owner = obj->owner ? &movement_obj[obj->owner - 1] : obj;
    effect_spawnImpactBlast(owner, obj, &pos, variant, 0, 0xFF, 0);
}

void event_spawnImpactBlast1(s32 cmd) {
    event_spawnImpactBlast(1);
}

void event_spawnImpactBlast0(s32 cmd) {
    event_spawnImpactBlast(0);
}

void event_spawnBone1BeamFlash(s32 cmd) {
    EffectLocals fx;

    fx.localPos = D_80099B88;
    fx.vec78 = D_80099B94;

    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }

    if ((gCurrentActor->cullMask & 0xF) == 0xF) {
        return;
    }

    fx.randoms.width = (math_randomFloat() * 101.0f) + 151.0f;
    fx.randoms.height = (math_randomFloat() * 4.0f) + 4.0f;

    math_transformOffset(&fx.localPos, &fx.worldPos, &gCurrentActor->ob_matrix_ptr[1]);

    effect_spawnBoneBeamParticle(gCurrentActor, 1, &fx.localPos, &fx.vec78, &fx.randoms,
                  (math_random() % 6U) + 5, (math_random() % 156U) + 100,
                  (math_randomFloat() * 2496.0f) + 3496.0f,
                  &fx.worldPos, 0xFF, 0, -1, 0xFF, 0);

    fx.header.flags = 3;
    fx.header.update = -1;
    fx.header.lifetime = (math_random() % 6U) + 0xA;
    fx.header.lightType = 0;

    fx.header2.x = fx.worldPos.x;
    fx.header2.y = fx.worldPos.y;
    fx.header2.z = fx.worldPos.z;

    light_createWithEffect(&fx.header, &fx.header2, (math_random() % 201U) + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
}
void event_spawnBoneBeamFlashRumble(cmd)
    u8 cmd;
{
    EffectLocals fx;

    fx.localPos = D_80099BA0;
    fx.vec78 = D_80099BAC;

    if (gCurrentActor->ob_matrix_ptr == NULL) {
        return;
    }

    if ((gCurrentActor->cullMask & 0xF) == 0xF) {
        return;
    }

    fx.randoms.width = (math_randomFloat() * 180.0f) + 150.0f;
    fx.randoms.height = math_randomFloat() * 2.0f + 2.5f;

    math_transformOffset(&fx.localPos, &fx.worldPos, &gCurrentActor->ob_matrix_ptr[cmd ? 1 : 2]);

    effect_spawnBoneBeamParticle(gCurrentActor, cmd ? 1 : 2, &fx.localPos, &fx.vec78, &fx.randoms,
                  (math_random() % 5U) + 5, (math_random() % 156U) + 100,
                  (math_randomFloat() * 4000.0f) + 4000.0f,
                  &fx.worldPos, 0xFF, 0, -1, 0xFF, 0);

    fx.header.flags = 3;
    fx.header.update = -1;
    fx.header.lifetime = (math_random() % 5U) + 3;
    fx.header.lightType = 0;

    fx.header2.x = fx.worldPos.x;
    fx.header2.y = fx.worldPos.y;
    fx.header2.z = fx.worldPos.z;

    light_createWithEffect(&fx.header, &fx.header2, (math_random() % 201U) + 55, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 0);
    particle_startPlayerRumbles(&fx.worldPos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, 0xFF, 0);
    particle_startPlayerCameraWobbles(&fx.worldPos, 506.0f, 1013.0f, 0.0009871669f, 5, 0xFF, 0);
}
void event_spawnBone2BeamFlashRumble(s32 cmd) {
    event_spawnBoneBeamFlashRumble(0);
}

void event_spawnBone1BeamFlashRumble(s32 cmd) {
    event_spawnBoneBeamFlashRumble(1);
}

void event_spawnTwoSidedEmitter(s32 cmd) {
    particle_spawnTwoSidedEmitter(gCurrentActor, 0, 1, gCurrentActor->animseq.ub[1]);
}

void event_fireForwardProjectile(s32 cmd) {
    RumbleParams args;

    weapon_spawnForwardProjectile(gCurrentActor, 0xFF, 1);
    if (gCurrentActor->CameraPtr != NULL) {
        args.flags = 1;
        args.duration = (math_random() % 7U) + 10;
        args.playerMask = 1 << gCurrentActor->CameraPtr->CNumber;
        args.strength = (math_random() % 7U) + 2;
        args.updateKind = -1;
        cont_spawnRumble(&args, 0, 0xFF, 1);
    }
}

void event_fireAttachedGun(s32 cmd) {
    RumbleParams args;

    if (screen_getPlayer0MatrixPhysics() != 0) {
        weapon_fireAttachedGunProjectile(gCurrentActor, 0xFF, 1);
    } else {
        weapon_fireAttachedGun(gCurrentActor, 0xFF, 1);
    }
    if (gCurrentActor->CameraPtr != NULL) {
        args.flags = 1;
        args.duration = (math_random() % 5U) + 15;
        args.playerMask = 1 << gCurrentActor->CameraPtr->CNumber;
        args.strength = (math_random() % 4U) + 3;
        args.updateKind = -1;
        cont_spawnRumble(&args, 0, 0xFF, 1);
    }
}

void event_spawnShotEffectsWithAttacks(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 1, 0, 0xFF, 1);
}

void event_spawnShotEffects(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 0, 0, 0xFF, 1);
}

void event_spawnShotEffectsWithRay(s32 cmd) {
    weapon_spawnShotEffects(gCurrentActor, 0, 1, 0xFF, 1);
}

void event_spawnGunMuzzleSegmentParticle(s32 cmd) {
    particle_spawnBoneSegmentParticle(gCurrentActor, &gGunDirParticleFrom, &gGunDirParticleTo, gGunMuzzleBone, 1.0f, 0xFF, 1);
}

void event_attachPointEmitter4(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 4, 0x78, 0xFF, 1);
}

void event_attachPointEmitter2(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 2, 0x78, 0xFF, 1);
}

void event_attachPointEmitter3(s32 cmd) {
    particle_detachObjEmitters(gCurrentActor);
    particle_attachPointEmitter(gCurrentActor, 3, 0x78, 0xFF, 1);
}

void event_emptyAnimCommand91(s32 cmd) {

}

void event_spawnStripEffect(s32 cmd) {
    trail_spawnStripEffect(gCurrentActor, cmd - 0x55, 0xFF, 1);
}

void event_notifyFollowersGone(s32 cmd) {
    trail_notifyFollowersGone(gCurrentActor);
}

void event_spawnLargeObjBurst(s32 cmd) {
    Vec3f pos;
    Actor *obj;

    obj = gCurrentActor;
    if (obj->ob_matrix_ptr != NULL && (obj->cullMask & 0xF) != 0xF) {
        effect_getBone5Point(obj, &pos);
        effect_spawnLargeObjBurstAt(&pos, 0xFF, 1);
    }
}

void event_rumbleNearbyPlayers(s32 cmd) {
    Vec3f pos;
    Actor *obj;

    obj = gCurrentActor;
    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;
    particle_startPlayerRumbles(&pos, 1502.0f, 3000.0f, 0.00033333333f, 12, 15, 0xFF, 0);
}

void event_attachBeamTargetEffects(s32 cmd) {
    Actor *obj = collision_findObjectById(18);
    if (obj != NULL) {
        beam_spawnFollowParticle(obj);
        beam_attachTrailEmitter(obj);
        light_createAtActor(obj, 1, 2, 300, 70, 255, 130, 0, 255, 1);
    }
}

void event_releaseBeamTargetEffects(s32 cmd) {
    ActorRef ref;
    Actor *obj;

    obj = collision_findObjectById(18);
    if (obj != NULL) {
        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        particle_broadcastToSprites((s32)&ref, 0x43);
        effect_sendEventToTimedEffects((s32)&ref, 0x43);
    }
}

void event_spawnDelayedBurst(s32 cmd) {
    effect_spawnDelayedBurst(gCurrentActor, 0xFF, 1);
}

void event_destroyActorEmitters(s32 cmd) {
    effect_destroyActorEmitters(gCurrentActor);
}

void event_spawnBonePointSegmentParticle(s32 cmd) {
    particle_spawnBoneSegmentParticle(gCurrentActor, &D_800A1FB0, &D_800A1FBC, D_80088B90, 0.7f, 0xFF, 1);
}

/* Spawns effect weapon_spawnImpactBurst at one of four random bones. */
void event_spawnRandomBoneImpactBurst(s32 cmd) {
    Vec3f pos;
    const u8 bones[4] = { 13, 16, 24, 27 };

    if (gCurrentActor->ob_matrix_ptr != NULL || (gCurrentActor->cullMask & 0xF) == 0xF) {
        math_transformOffset(&gZeroVec3f, &pos, &gCurrentActor->ob_matrix_ptr[bones[math_random() & 3]]);
        pos.y = gCurrentActor->floorY + 20.0f;
        weapon_spawnImpactBurst(&pos, 0xFF, 1);
    }
}

const Vec3f D_80099BBC = { 0.0f, 17.0f, 7.0f };
const Vec3f D_80099BC8[3] = {
    { 0.0f, 2.0f, 145.0f },
    { 62.0f, 11.0f, 114.0f },
    { -62.0f, 11.0f, 114.0f },
};

/* Debris model ids and their scales (particle_spawnDebrisBurst). */
const s32 gDebrisModelIds[6] = { 0x39, 0x11, 0x12, 0x13, 0x14, 0x15 };
const f32 gDebrisModelScales[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

void event_spawnBonePointImpact0(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 0, 0xFF, 1);
}

void event_spawnBonePointImpact1(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 1, 0xFF, 1);
}

void event_spawnBonePointImpact2(s32 cmd) {
    effect_spawnBonePointImpact(gCurrentActor, 2, 0xFF, 1);
}

void event_emitLineBursts(s32 cmd) {
    trail_emitLineBursts(gCurrentActor, 0xFF, 1);
}

void func_15071A64(s32 cmd) {
    GroundPoint hit;

    if (!(math_random() & 1) && gCurrentActor->ob_matrix_ptr != NULL && (gCurrentActor->cullMask & 0xF) != 0xF &&
        movement_obj[0].stunned != 0 && movement_obj[0].energy > 0) {
        collision_initHitFromObject(&hit.ground, gCurrentActor);
        math_transformOffset(&D_80099BBC, &hit.pos, &gCurrentActor->ob_matrix_ptr[15]);
        particle_spawnSmallSplashAndDrops(&hit.pos.x, &hit.ground, 0, 0xFF, 1);
    }
}

void func_15071B18(s32 cmd) {
    Vec3f point;
    CollisionHit collision;

    if (gCurrentActor->ob_matrix_ptr != NULL && (gCurrentActor->cullMask & 0xF) != 0xF) {
        math_transformOffset(&D_80099BC8[math_random() % 3U], &point, &gCurrentActor->ob_matrix_ptr[7]);
        collision_initHitFromObject(&collision, gCurrentActor);
        if (math_random() & 1) {
            if (math_random() & 1) {
                particle_spawnSplashAndDrops(&point.x, &collision, 0, 0xFF, 1);
            } else {
                particle_spawnSmallSplashAndDrops(&point.x, &collision, 0, 0xFF, 1);
            }
        }
        if (math_random() & 1) {
            DebrisBurst config;

            config.countBase = 8;
            config.countRange = 5;
            config.pos = point;
            config.angles.yawBase = 0;
            config.angles.yawRange = 0xFF;
            config.angles.pitchBase = -64;
            config.angles.pitchRange = 60;
            config.speedBase = 10.0f;
            config.speedRange = 10.0f;
            config.lifeBase = 25;
            config.lifeRange = 20;
            config.gravityBase = -0.97800004f;
            config.gravityRange = 0.263f;
            config.scaleBase = 0.096f;
            config.scaleRange = 0.07f;
            config.spin = 21.892f;
            particle_spawnDebrisBurst(&config, gDebrisModelIds, gDebrisModelScales,
                          6, 25.0f, 0, 0xFF, 1);
        }
    }
}

void event_spawnImpactBurstNearBone(s32 cmd) {
    effect_spawnImpactBurstNearBone(gCurrentActor, 0xFF, 1);
}

/* Runs handler gAnimCommandProcs[gEventParam], passing it the command. */
void event_runAnimCommand(void) {
    void (*func)(s32);

    func = gAnimCommandProcs[gEventParam];
    if (func != NULL) {
        func(gEventParam);
    }
}

/* Runs the gAnimCommandProcs handler on the object event_getFirstOwnedObject returns. */
void event_runChildAnimCommand(void) {
    Actor *target;
    Actor *saved;

    target = event_getFirstOwnedObject(gCurrentActor);
    if (target != NULL) {
        saved = gCurrentActor;
        gCurrentActor = target;
        event_runAnimCommand();
        gCurrentActor = saved;
    }
}

void event_runActorEffects(void) {
    effect_runActorEffects(gCurrentActor, gEventParam);
}

void event_runSurfaceHandler(void) {
    effect_runSurfaceHandler(gCurrentActor, gEventParam);
}

void event_setIntelAccel(void) {
    gCurrentActor->intel_accel = gEventParam;
}

void event_setIntelDecel(void) {
    gCurrentActor->intel_decel = gEventParam;
}

void event_playIntelAnimSeq(void) {
    gEventResult = gEventParam & 0xFF;
    gCurrentActor->intel_animseq = (gEventParam >> 8) & 0xFF;
    actor_playAnim(gCurrentActor, gCurrentActor->intel_animseq, 1.2f, 3.0f, gEventResult, 0.0f, 0);
}

void func_15071ED4(void) {
    actor_playAnim(gCurrentActor, 89, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void func_15071F14(void) {
    actor_playAnim(gCurrentActor, 36, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

void event_startDeathCountdown(void) {
    actor_startDeathCountdown(gCurrentActor, gEventParam);
}

void event_skipNextEventInCutscene(void) {
    if (gCutsceneStates[0] == 1) {
        gCurrentActor->speccnt++;
    }
}

void event_clearReactionTimerOnGround(void) {
    if (gCurrentActor->shad == 0.0f) {
        gCurrentActor->reactionTimer = 0;
    }
}

/*
 * Launches the player (movement_obj[0]) with the current object's stored velocity unk2D8[0..2]
 * and an animation. With gEventParam == 0, only if player->stand_on_char names the object owned by the
 * current player (unk65), and the player is turned around.
 */
void event_launchPlayer(void) {
    Actor *player;
    Actor *entry;
    s32 found;
    s32 anim;
    s32 visible;
    s32 i;

    visible = 1;
    anim = 404;
    player = movement_obj;
    if (gEventParam == 0) {
        found = -1;
        entry = player;
        for (i = 0; i < 25; i++) {
            if (entry->attach_char == dc_glbl_CharNum + 1) {
                found = i;
                break;
            }
            entry++;
        }
        if (found + 1 != player->stand_on_char) {
            return;
        }
        player->angle = gCurrentActor->angle + 0x8000;
        player->fallStartY = -5000.0f;
    } else {
        if (gCurrentActor->taskint[1] >= 113) {
            anim = 741;
            gCurrentActor->unk2D8[2] = 0.0f;
        } else {
            anim = 50;
            player->angle = gCurrentActor->angle;
            if (gCurrentActor->unk251 != 0) {
                player->fallStartY = movement_obj[0].ypos + 800.0f;
            }
            visible = 0;
        }
        gCurrentActor->taskint[0] = 50;
    }
    player->yspd = gCurrentActor->unk2D8[0];
    player->ygrav = gCurrentActor->unk2D8[1];
    player->speed = gCurrentActor->unk2D8[2];
    player->count3 = 1;
    actor_playAnim(player, anim, 1.0f, 2.0f, 6.0f, 0.0f, 0);
    movement_obj[0].mv->prop = visible;
    if (player->CameraPtr != NULL) {
        player->CameraPtr->SpecialFlags |= 0x400;
    }
}

void event_runFadeCommand(void) {
    u16 value = gEventParam;
    u16 step = gEventParam >> 8;
    u16 cmd = gEventParam >> 16;

    actor_fadeCommand(gCurrentActor, cmd, value, step, 0);
}

Actor *event_getFirstOwnedObject(Actor *owner) {
    return event_getOwnedObject(owner, 0);
}

/* Returns the n-th object owned by `owner` (unk65 = owner index + 1), or NULL. */
Actor *event_getOwnedObject(Actor *owner, s32 n) {
    s32 ownerId;
    s32 count;
    s32 i;

    ownerId = (owner - movement_obj) + 1;
    count = 0;
    for (i = 0; i < 25; i++) {
        if (ownerId == movement_obj[i].attach_char) {
            if (count == n) {
                return &movement_obj[i];
            }
            count++;
        }
    }
    return NULL;
}

void event_fadeOwnedObject(void) {
    Actor *child = event_getFirstOwnedObject(gCurrentActor);

    if (child != NULL) {
        s32 value = gEventParam >> 8;
        u16 step = gEventParam;

        actor_fadeCommand(child, 2, value, step, 0);
    }
}

void event_showOnlyParts(void) {
    gCurrentActor->hiddenParts = ~(gEventParam | 1);
}

void event_showParts(void) {
    gCurrentActor->hiddenParts &= ~gEventParam;
}

void event_hideParts(void) {
    gCurrentActor->hiddenParts |= gEventParam;
}

void event_removeObjectById(void) {
    Actor *obj = collision_findObjectById(gEventParam);

    if (obj != NULL) {
        actor_remove(obj, 0);
    }
}

void event_removeAttachedObjectById(void) {
    Actor *obj = collision_findObjectById(gEventParam);

    if (obj != NULL && obj->attach_char != 0) {
        actor_remove(obj, 0);
    }
}

/* Respawns the object whose spawn record (gSpawnRecords, 0x30-byte SpawnRecord entries) gEventParam names. */
void event_respawnObject(void) {
    Actor *obj;
    s32 spawnFlags;
    s32 angle;
    s32 slot;
    s32 spawnIdx;
    s32 noKill;

    angle = (gEventParam >> 8) & 0x7F;
    noKill = (gEventParam >> 15) & 1;
    spawnFlags = (gEventParam >> 16) & 0xFF;
    spawnIdx = collision_findSpawnById(gEventParam);
    if (spawnIdx == -1) {
        return;
    }
    gSpawnInUse[spawnIdx] = 0;
    if (!(spawnFlags & 0x80)) {
        obj = actor_findOnPath(spawnIdx);
        if (obj != NULL) {
            actor_remove(obj, 0);
            actor_clearSlot(obj);
        }
    }
    gSpawnRecords[spawnIdx].removed = 0;
    slot = collision_spawnObject((SpawnRecord *)(spawnIdx * 0x30 + (s32)gSpawnRecords), spawnIdx, 0, 0, 0); // matching: integer address math
    if (slot == 0) {
        return;
    }
    obj = &movement_obj[slot - 1];
    if (obj == NULL) {
        return;
    }
    obj->attach_char = (gCurrentActor - movement_obj) + 1;
    obj->basemtx = angle;
    if (noKill) {
        obj->unk101 = 0;
    } else {
        obj->unk101 = 4;
    }
    obj->intelwalk = spawnFlags & 0x7F;
    obj->owner = dc_glbl_CharNum + 1;
    obj->xpos = gCurrentActor->xpos;
    obj->ypos = gCurrentActor->ypos;
    obj->zpos = gCurrentActor->zpos;
    if (gIsMultiplayer != 0) {
        if (gCurrentActor->character_type == 0x53) {
            gCurrentActor->targetIndex = obj - movement_obj;
        }
    }
    if (level == LEVEL_WMP_RAPTOR && obj->character_type == 0x16) {
        obj->hiddenParts |= 2;
    }
}

void event_attachTargetToBoneAndSetState(void) {
    u16 state;
    s32 bone;
    Actor *obj;

    if (gIsMultiplayer == 0) {
        if (gCurrentActor->targetIndex == 0) {
            return;
        }
    }
    state = gEventParam;
    bone = gEventParam >> 16;
    obj = &movement_obj[gCurrentActor->targetIndex];
    obj->attach_char = dc_glbl_CharNum + 1;
    obj->basemtx = bone;
    obj->unk101 = 4;
    obj->stunned = 0;
    obj->imune = 0xFF;
    obj->scriptCmd = NULL;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->intelwalk = state;
    obj->bodyRadiusOverHeight = 1.0f;
    obj->bodyHeightOverRadius = 1.0f;
    obj->hitRadiusOverHeight = 1.0f;
    obj->hitHeightOverRadius = 1.0f;
    func_151B5BA0(&movement_obj[dc_glbl_CharNum], obj, bone);
}

void event_attachTargetToBone(void) {
    s32 bone;
    Actor *obj;

    bone = gEventParam >> 16;
    obj = &movement_obj[gCurrentActor->targetIndex];
    obj->attach_char = dc_glbl_CharNum + 1;
    obj->basemtx = bone;
    obj->unk101 = 4;
}

void event_setAnimSpeedTenths(void) {
    gCurrentActor->animsp = gEventParam * 0.1f;
    gCurrentActor->anim->speed[0] = gCurrentActor->animsp;
}

void event_setOwnedObjectsState(void) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0 && dc_glbl_CharNum + 1 == movement_obj[i].attach_char) {
            movement_obj[i].scriptCmd = NULL;
            movement_obj[i].intelwalk = gEventParam;
        }
    }
}

void event_removeSelf(void) {
    actor_remove(gCurrentActor, 0);
}

void event_removeSelfAndStopParts(void) {
    actor_remove(gCurrentActor, 1);
}

void func_15072968(void) {
    actor_playAnim(gCurrentActor, 44, 1.5f, 12.0f, 0.0f, 0.0f, 0);
    gCurrentActor->norun = 25;
}

void event_setTaskInt0(void) {
    gCurrentActor->taskint[0] = gEventParam;
}

void func_150729D0(void) {
    gCurrentActor->mv->pissed_up = 1;
    face_setDefaultExpression(gCurrentActor, 21);
    func_1506B82C();
}

void event_clearIdleDelay(void) {
    gCurrentActor->mv->idle.delay = 0;
    anim_updateIdle();
}

void event_finishGrenadeThrow(void) {
    gCurrentActor->mv->GrenadeThrowState = 0;
    gCurrentActor->mv->rt_context_b = 0;
    anim_updateIdle();
}

void func_15072A7C(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->unk1A9++;
        if ((math_random() & 7) < gCurrentActor->mv->unk1A9) {
            actor_playAnim(gCurrentActor, 394, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        }
    }
}

void event_playNextAnimDefaultSpeed(void) {
    actor_playAnim(gCurrentActor, gCurrentActor->animseq.uh + 1, 1.0f, 6.0f, 0.0f, 0.0f, 0);
}

/* Picks the follow-up animation for the current animation unk84 and plays it. */
void event_playFollowUpAnim(void) {
    s32 state;
    s32 anim = -1;

    state = gCurrentActor->animseq.uh;
    switch (state) {
    case 0x26F:
    case 0x270:
    case 0x271:
    case 0x272:
    case 0x273:
        anim = state + 1;
        break;
    case 0x274:
        anim = 0x26F;
        break;
    case 0x1D6:
        anim = 0x1D7;
        break;
    case 0x1D7:
        anim = 0x1D8;
        break;
    case 0x175:
        anim = 0x177;
        break;
    case 0x176:
        anim = 0x177;
        break;
    case 0x35:
        anim = 0x3A;
        break;
    case 0x3F:
        anim = 0x3A;
        break;
    case 0x1CD:
        anim = 0x1CE;
        break;
    case 0x1CF:
        anim = 0x1CE;
        break;
    case 0xFD:
        anim = 0xFE;
        break;
    case 0x10A:
        anim = 0x10A;
        break;
    case 0x12C:
        anim = 0x12D;
        break;
    case 0x142:
        anim = 0x12D;
        break;
    case 0x138:
        anim = 0x3E;
        break;
    case 0x10C:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x10A;
        } else {
            anim_updateIdle();
        }
        break;
    case 0x156:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x157;
        } else {
            anim_updateIdle();
        }
        break;
    case 0x167:
        if (gCurrentActor->mv->SniperState) {
            anim = 0x165;
        } else {
            anim_updateIdle();
        }
        break;
    default:
        if (state == 0xBE || (state == 0xC1 && !(gCurrentActor->mv->old_keys & 0x2000))) {
            anim = 0xBF;
        }
        break;
    }
    if (anim != -1) {
        actor_playAnim(gCurrentActor, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
    }
}

void event_setAimTargetMode(void) {
    gCurrentActor->unk2F8 &= 0xFFF8;
    gCurrentActor->unk2F8 |= gEventParam;
}

void event_createPartSet(void) {
    collision_createPartSet(gCurrentActor, gEventParam, 1.0f, 0);

    if (gCurrentActor->mv != NULL && gCurrentActor->mv->weapon_status == 1) {
        gCurrentActor->mv->weapon_status = 2;
    }
}

void event_removePartSet(void) {
    collision_removePartSet(gCurrentActor, gEventParam);

    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->weapon_status = 0;
    }
}

void event_setReactionTimer(void) {
    gCurrentActor->reactionTimer = gEventParam;
}

void event_scaleAnimSpeed(void) {
    gCurrentActor->anim->speed[0] *= gEventParamF;
}

void event_clearReactionTimerBelowAnimSpeed(void) {
    AnimInstance *anim;

    anim = gCurrentActor->anim;
    if (anim->speed[0] < gEventParamF) {
        gCurrentActor->reactionTimer = 0;
    }
}

void event_setStunned(void) {
    gCurrentActor->stunned = gEventParam;
}

void event_fireRayAtObjects(void) {
    f32 targetX;
    f32 targetZ;
    f32 targetY;
    s16 pitch;

    pitch = gCurrentActor->aimPitch * 200;
    gCurrentActor->mv->gun_before_reload -= 1;
    actor_splitVelocity(gCurrentActor->langle - gCurrentActor->mv->pssd_waist_tilt, 2000.0f, pitch * 0.0054931640625f,
                  &targetX, &targetZ, &targetY);
    targetX = targetX * 5000.0f + gCurrentActor->xpos;
    targetZ = targetZ * 5000.0f + gCurrentActor->zpos;
    targetY = targetY * 5000.0f + gCurrentActor->ypos;
    collision_castRayAtObjects(gCurrentActor, gCurrentActor->xpos, gCurrentActor->ypos + gCurrentActor->hitOffsetY,
                  gCurrentActor->zpos, targetX, targetY, targetZ, dc_glbl_CharNum, 18);
}

void event_emptyTimedEvent120(void) {
}

void event_setAlertCooldown(void) {
    gCurrentActor->alertCooldown = gEventParam;
}

void func_15073070(void) {

}

void event_startCameraEnvelope(void) {
    camera_startEnvelope(0, gEventParam, 1);
}

void func_150730A4(void) {
    gCurrentActor->levelMode = 3;
    gCurrentActor->levelTimer = gEventParam;
}

void func_150730D0(void) {
    if (gEventParam == 1) {
        gCurrentActor->unk2F8 |= 0x100;
    } else {
        gCurrentActor->unk2F8 &= 0xFEFF;
    }
}

/*
 * Spawns a projectile (state 0x21) playing animation `anim`, at bone `bone` of `parent` offset by
 * (offX, offY, offZ), or at the parent's position. Returns the new object.
 */
Actor *event_spawnAnimProjectile(Actor *parent, s32 bone, s32 anim, f32 offX, f32 offY, f32 offZ, f32 yawOffset, f32 speed,
                     f32 yVelocity, f32 gravity, s32 count5) {
    MtxF *mtx;
    MtxF mf;
    Actor *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    obj = actor_alloc();
    obj->ygrav = gravity;
    obj->yspd = yVelocity;
    obj->speed = speed;
    obj->count5 = count5;
    if (parent->ob_matrix_ptr != NULL && bone != -1) {
        mtx = &parent->ob_matrix_ptr[bone];
        if (gBoneMatricesFlushed != 0) {
            guMtxL2F(mf, (Mtx *)mtx);
            mtx = &mf;
        }
        math_transformPoint(*mtx, offX, offY, offZ, &x, &y, &z);
    } else {
        x = parent->xpos;
        y = parent->ypos;
        z = parent->zpos;
    }
    obj->xpos = x;
    obj->ypos = y;
    obj->zpos = z;
    obj->angle = obj->langle = parent->langle + yawOffset * 182.04445f;
    obj->type = 0x21;
    obj->imune = 0xFF;
    obj->noshove = 2;
    if (anim != 0x42) {
        obj->noshove = 0x100E;
    }
    obj->unk66 = parent->unk66 & 0xC;
    obj->bodyRadius = 10;
    obj->bodyHeight = 10;
    obj->bodyRadiusOverHeight = 1.0f;
    obj->bodyHeightOverRadius = 1.0f;
    obj->hitRadiusOverHeight = 1.0f;
    obj->hitHeightOverRadius = 1.0f;
    obj->scale_xz = 0.5f;
    obj->scale_y = 0.5f;
    if (anim == 0x1F) {
        obj->rootOffsetY = 10.0f;
    }
    obj->headEffectState = anim;
    obj->drawtype = 5;
    obj->unique_id = 255U - (obj - movement_obj);
    collision_createPartSet(obj, anim, 1.0f, 0);
    for (i = 0; i <= gScreenLayout; i++) {
        obj->gfxLights[i] = light_allocGfxLights(1);
    }
    if (obj->gfxLights[i - 1] != 0) {
        obj->maxLights = 1;
    }
    obj->ambients = light_allocAmbients();
    effect_attach(effect_newCharacterWatcher(obj, 1, 0), obj, 0x13);
    return obj;
}

/* Fires projectiles from the current object; gEventParam picks the volley (0: aimed at the camera target). */
void event_fireVolley(void) {
    Actor *obj;
    s32 rot;
    f32 yVelocity;
    f32 speed;
    f32 pad[7]; // matching: unused stack space

    if (gEventParam == 0) {
        f32 dist;
        Camera *camera;
        s32 aimAtCamera;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 angle;
        f32 yaw;

        aimAtCamera = 0;
        camera = gCurrentActor->CameraPtr;
        if (aim_isPointInCone(gCurrentActor - movement_obj, 23, 23, camera->eye.x, camera->eye.y, camera->eye.z,
                          &dist, 30.0f, 0, 0) != 0 &&
            math_random() % 1000 < 200) {
            aimAtCamera = 1;
        }
        obj = event_spawnAnimProjectile(gCurrentActor, 23, 0x1F, 0.0f, 0.0f, 16.0f, 0.0f, 42.8f, 49.9f, 2.9f, 100);
        obj->noshove |= 0x01000000;
        if (aimAtCamera != 0) {
            dx = camera->eye.x - obj->xpos;
            dy = camera->eye.y - obj->ypos;
            dz = camera->eye.z - obj->zpos;
            angle = math_atan2(dx, dz) * 57.295776f;
            yaw = angle;
            if (angle > 180.0f) {
                yaw = angle - 360.0f;
            }
            angle = obj->speed * dy / sqrtf(dx * dx + dz * dz);
            obj->yspd = angle;
            obj->ygrav = 0.0f;
            obj->taskint[0] = 1;
            rot = (s16)(yaw * 182.04445f) - 0x4000;
            obj->langle = rot;
            obj->angle = rot;
        } else {
            obj->taskint[0] = 0;
        }
    } else if (gEventParam == 1) {
        f32 jitter;
        f32 spread;

        spread = gCurrentActor->taskint[0] * 0.01f;
        jitter = (s32)(math_random() % 40 - 20) * spread;
        yVelocity = 58.4f + jitter;
        jitter = (s32)(math_random() % 40 - 20) * spread;
        speed = 42.8f + jitter;
        event_spawnAnimProjectile(gCurrentActor, 14, 0x42, 0.0f, 0.0f, 16.0f, (s32)(math_random() % 40 - 20) * spread, speed, yVelocity, 2.9f, 100);
    } else if (gEventParam == 2) {
        obj = event_spawnAnimProjectile(gCurrentActor, 9, 0x35, 0.0f, 27.3f, 0.0f, 180.0f, 23.4f, 45.2f, 3.5f, 100);
        obj->vtilt = 180.0f;
        obj->noshove |= 0x01000000;
    } else if (gEventParam == 3) {
        f32 jitter;

        jitter = (s32)(math_random() % 40 - 20) * 10.0f;
        yVelocity = 58.4f + jitter;
        jitter = (s32)(math_random() % 40 - 20) * 10.0f;
        speed = 42.8f + jitter;
        event_spawnAnimProjectile(gCurrentActor, 14, 0x88, 0.0f, 0.0f, 16.0f, (s32)(math_random() % 40 - 20) * 10.0f, speed, yVelocity, 2.9f, 100);
    }
}

/* Releases the object held in unk13C (object index + 100). */
void event_releaseHeldObject(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting != 0) {
        gCurrentActor->lifting = 0;
        movement_obj[index].noshove = 0x8200;
        movement_obj[index].carried = 0;
        movement_obj[index].angle = gCurrentActor->langle;
        movement_obj[index].attach_char = 0;
        movement_obj[index].intelwalk = 6;
        movement_obj[index].scriptCmd = NULL;
        movement_obj[index].stunned = 0;
        movement_obj[index].langle = 0xE000;
        actor_playAnim(gCurrentActor, 151, 1.0f, 5.0f, 0.0f, 0.0f, 0);
    }
}

void event_setWaitTimer(void) {
    gCurrentActor->eventWaitTimer = gEventParam;
}

void event_waitForTimer(void) {
    gCurrentActor->eventWaitTimer -= gFrameSteps;

    if (gCurrentActor->eventWaitTimer >= 0) {
        gEventLoopRunning = 0;
        gCurrentActor->speccnt--;
        gEventResult = gEventParam;
    }
}

void event_setStickSpeed(void) {
    gCurrentActor->stickspeed = gEventParam;
}

/* Applies actor_hitScripted (gEventParam & 0xFF00FF) to movement_obj[owner]; bit 30 sets its fallStartY to our height. */
void func_15073A50(void) {
    Actor *parent;
    s32 flags;
    s32 index;

    parent = gCurrentActor;
    index = parent->owner;
    if (movement_obj[index].attach_char != 0) {
        flags = gEventParam;
        parent->lifting = 0;
        movement_obj[index].imune = 0;
        actor_hitScripted(&movement_obj[index], gEventParam & 0xFF00FF, 0, dc_glbl_CharNum);
        movement_obj[index].fallStartY = 100000.0f;
        if (flags & 0x40000000) {
            movement_obj[index].fallStartY = gCurrentActor->ypos;
        }
        movement_obj[index].imune = 20;
        movement_obj[index].angle = gCurrentActor->langle + flags;
    }
}

/* Unpacks gEventParam into mv->ropeSwingForce (low half, as f32), ropeSwingAngle and ropeSwingTimer (top bytes). */
void event_setRopeSwing(void) {
    s32 *param;
    Actor **objp;
    f32 force;
    f32 swingAngle;

    param = &gEventParam; // matching: both globals are accessed through pointer locals
    objp = &gCurrentActor;
    force = *param & 0xFFFF;
    (*objp)->mv->ropeSwingForce = force;
    swingAngle = (*param >> 16) & 0xFF;
    (*objp)->mv->ropeSwingAngle = swingAngle;
    (*objp)->mv->ropeSwingTimer = (*param >> 24) & 0xFF;
}

void event_updateIdle(void) {
    anim_updateIdle();
}

void event_emptyAnimState50(void) {

}

void func_15073C50(void) {
    gCurrentActor->unk224 = movement_obj[gCurrentActor->targetIndex].ypos + gEventParam;
}

void func_15073CB8(void) {
    Actor *obj = actor_findByType(0x53);

    if (obj != NULL && (u16)obj->intel_animseq == 0x1F) {
        obj->scriptTimer = 0;
    }
}

void event_setBodyRadius(void) {
    actor_setBodyRadius(gCurrentActor, gEventParam);
}

void event_setBodyHeight(void) {
    actor_setBodyHeight(gCurrentActor, gEventParam);
}

void event_detachObjectById(void) {
    Actor *obj;

    obj = collision_findObjectById(gEventParam);
    if (obj != NULL) {
        obj->attach_char = 0;
    }
}

void func_15073DA4(void) {
    s32 kind;

    if (gEventParam != 0) {
        kind = 18;
    } else {
        kind = 2;
    }
    event_spawnProjectile(gCurrentActor->theta, 150.0f, 0, 0, 100, kind, 60.0f, 0.5f, NULL, 0, 1);
}

void func_15073E2C(void) {
    event_spawnProjectile(gCurrentActor->theta, 80.0f, 0, 0, 100, 11, 40.0f, 0.5f, NULL, 14, 1);
}

void func_15073EA4(void) {
    event_spawnProjectile(gCurrentActor->theta, 60.0f, 0, 0, 100, 12, 40.0f, 0.5f, NULL, 14, 1);
}

void func_15073F1C(void) {
    ModelAttachment *att = attach_find(gCurrentActor, 60, 0);

    if (att != NULL) {
        att->hookState = 2400;
    }
}

void event_emptyAnimState53(void) {
}

void event_setBouncePercent(void) {
    gCurrentActor->bouncePercent = gEventParam;
}

void func_15073F78(void) {
    gCurrentActor->unk10B &= ~gEventParam;
}

/*
 * Spawns the object gEventParam & 0xFFFF as a thrown/fired item of the current object, at a bone
 * or above it, then sets its launch velocity and state (unk232) per thrower id.
 */
void event_spawnThrownItem(void) {
    s32 state;
    s32 slot;
    s32 useDefault;
    s32 spawnId;
    f32 speed;
    Actor *obj;
    s32 bone;
    s32 spawnIdx;
    s32 zeroSpeed;
    f32 fwdSpeed;
    f32 yVelocity;
    f32 unused;
    f32 xOff;
    f32 zOff;
    f32 yOff;
    s32 yawJitter;
    s16 angle;
    Actor *player;

    spawnId = gEventParam & 0xFFFF;
    spawnIdx = collision_findSpawnById(spawnId);
    if (spawnIdx != -1) {
        state = 1;
        gSpawnInUse[spawnIdx] = 0;
        slot = collision_spawnObject((SpawnRecord *)(spawnIdx * 0x30 + (s32)gSpawnRecords), spawnIdx, 0, 0, 0); // matching: integer address math
        if (slot != 0) {
            slot--;
            obj = &movement_obj[slot];
            obj->owner = dc_glbl_CharNum + 1;
            obj->unique_id = dc_glbl_CharNum + 0x96;
            player = gCurrentActor;
            speed = 40.0f;
            useDefault = 1;
            if (player->ob_matrix_ptr != NULL) {
                useDefault = 0;
                switch (player->character_type) {
                case 0x75:
                case 0x80:
                    bone = 4;
                    break;
                case 0x5A:
                    bone = 19;
                    break;
                case 0x7B:
                    if (spawnId == 0x10) {
                        bone = (gEventParam >> 16) & 0xFF;
                    } else {
                        useDefault = 1;
                    }
                    break;
                default:
                    useDefault = 1;
                    break;
                }
                if (!useDefault) {
                    spawnIdx = (s32)&player->ob_matrix_ptr[bone]; // matching: spawnIdx is reused for the bone matrix
                    math_transformOffset(NULL, (Vec3f *)&obj->xpos, spawnIdx); // xpos, ypos, zpos as one Vec3f
                    {
                        typedef s32 padA; // matching: a scope with a declaration steers as1 (-g3)
                        player = gCurrentActor;
                    }
                }
            }
            if (useDefault) {
                obj->xpos = player->xpos;
                obj->ypos = player->ypos + 100.0f;
                obj->zpos = player->zpos;
            }
            if (gIsMultiplayer != 0) {
                obj->taskint[0] = player->mv->GrenadeTimer;
                state = 5;
                player->mv->GrenadeThrowState = 0;
                gCurrentActor->mv->rt_context_b = 0;
                zeroSpeed = (gEventParam >> 16) & 1;
                obj->ygrav = 4.0; // matching: double literal
                player = gCurrentActor;
                speed = ((0xF0 - player->mv->GrenadeTimer) * 60.0f) / 240.0f;
                speed += 40.0f;
                if (zeroSpeed) {
                    speed = 0.0f;
                }
                if (player->mv->AimState != 0) {
                    {
                        typedef s32 padT; // matching: as above
                        actor_splitVelocity((u16)(s32)((45.0f - player->mv->aimPitch) * 182.04445f) & 0xFFFF, speed * 4.0f, 0.0f, &fwdSpeed, &yVelocity, &unused);
                        obj->yspd = -yVelocity;
                    }
                    player = gCurrentActor;
                } else {
                    obj->yspd = 50.0f;
                }
                if (player->character_type == 0x75) {
                    spawnIdx = (s32)attach_find(player, 0x64, 0); // matching: spawnIdx is reused for the attachment
                } else {
                    spawnIdx = (s32)attach_find(player, 0x2C, 0);
                }
                if (spawnIdx != 0) {
                    obj->soundHandle2 = ((ModelAttachment *)spawnIdx)->hookState2;
                    sound_replaceCallback((SoundRecordCallback)attach_soundCallback, (void *)spawnIdx, (s32)gCurrentActor, sound_followOwnerCallback, obj, obj->unique_id);
                }
                player = gCurrentActor;
            }
            angle = player->langle;
            if (player->mv != NULL) {
                angle -= player->mv->pssd_waist_tilt;
            }
            obj->langle = angle;
            obj->angle = angle;
            obj->speed = speed;
            obj->fallStartY = obj->ypos;
            if (player->character_type == 0x4A) {
                state = 5;
                obj->speed = 0.0f;
                trail_spawnActorBoneTrail(obj, 0xFF);
                gSpawnPaths[obj->listpos][1].x = movement_obj[0].xpos;
                gSpawnPaths[obj->listpos][1].z = movement_obj[0].zpos;
            } else if (player->character_type == 0x7B) {
                if (spawnId == 0x10) {
                    state = 7;
                    gSpawnPaths[obj->listpos][1].x = movement_obj[0].xpos;
                    gSpawnPaths[obj->listpos][1].y = 980;
                    gSpawnPaths[obj->listpos][1].z = movement_obj[0].zpos;
                    obj->unique_id = 0xFF - slot;
                    actor_runHitHandler(obj, 0xC, NULL);
                    beam_spawnMuzzleBurst(gCurrentActor, math_random() & 1, math_random() & 3, 0xFF, 1);
                } else if (spawnId == 0xA) {
                    state = 0xE;
                    obj->ypos = player->ypos;
                } else {
                    state = 4;
                    obj->unique_id = 0x12;
                    obj->ypos = 1040.0f;
                    obj->speed = 0.0f;
                }
            } else if (player->character_type == 0x9A) {
                yawJitter = (math_random() & 0xFFF) - 0x800;
                speed = (s16)(math_random() % 800U) * 0.01f;
                state = 7;
                actor_splitVelocity(gCurrentActor->angle + yawJitter, 30.0f - speed, 0.0f, &xOff, &zOff, &yOff);
                obj->xpos -= xOff;
                obj->zpos -= zOff;
                player = gCurrentActor;
                obj->speed = (player->speed * 1.35f) + 6.0f;
                obj->yspd = player->yspd;
                obj->imune = 0xFF;
                obj->ypos = player->ypos - 40.0f;
                obj->targetIndex = player->targetIndex;
            }
            obj->intelwalk = state;
            obj->energy |= 0x80;
        }
    }
}

void event_setWeaponStatus(void) {
    gCurrentActor->mv->weapon_status = gEventParam;
}

void func_15074664(void) {
    if (gCurrentActor->mv != NULL) {
        if (gEventParam == 1) {
            audio_setMixFallback(5);
        } else if (gCurrentActor->mv->unk94 == 1) {
            audio_setMixFallback(0);
        }
        gCurrentActor->mv->unk94 = gEventParam;
    }
}

/* Deals gEventParam damage to the current object (or, while it holds one, to the player). */
void event_dealDamage(void) {
    Actor *target;
    s32 damage;
    ActorCtl *ext;
    Camera *camera;

    target = NULL;
    if (gIsMultiplayer != 0) {
        target = gCurrentActor;
    } else if (gCurrentActor->lifting != 0) {
        target = movement_obj;
    }
    if (target != NULL) {
        damage = gEventParam;
        if (damage >= target->energy) {
            target->energy = 0;
            gCurrentActor->energy = 0;
            if (target->type == 1) {
                target->imune = 0xFF;
                actor_kill(target, 6);
            }
        } else {
            ext = target->mv;
            target->energy = target->energy - damage;
            if (ext != NULL) {
                camera = target->CameraPtr;
                if (camera != NULL) {
                    if (ext->SniperState != 0) {
                        screen_startViewFlash(0xFF, 0, 0, 180, 20, camera->CNumber);
                    }
                }
            }
        }
    }
}
/* Sets the state (unk232) of the current object's owner (unk65 = index + 1). */
void event_setOwnerState(void) {
    s32 i;
    s32 owner;

    owner = gCurrentActor->attach_char;
    i = owner;
    if (i != 0) {
        i = i - 1;
        movement_obj[i].intelwalk = gEventParam;
        movement_obj[i].scriptCmd = NULL;
    }
}

void event_addShotsFired(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->shotsfired += gEventParam;
    }
}

void event_setTurnAnimRow(void) {
    gCurrentActor->turnAnimRow = gEventParam;
}

/* Adds the top byte to unk138 when bit `mask` of word taskint[index] matches the low bit's sense. */
void event_skipEventsOnTaskBit(void) {
    u8 index;
    u8 mask;
    u8 skip;

    index = gEventParam >> 16;
    mask = gEventParam >> 8;
    skip = gEventParam & 1;
    if ((&gCurrentActor->taskint[0])[index] & mask) {
        skip ^= 1;
    }
    if (skip != 0) {
        gCurrentActor->speccnt += (u8)(gEventParam >> 24);
    }
}

void event_setPlayerIvehitit(void) {
    movement_obj[0].ivehitit = gEventParam;
}

void event_setHeldObjectState(void) {
    u8 index = gCurrentActor->lifting - 100;

    if (gCurrentActor->lifting != 0) {
        if (movement_obj[index].carried >= 100) {
            movement_obj[index].intelwalk = gEventParam;
            movement_obj[index].scriptCmd = NULL;
            movement_obj[index].stunned = 0;
        }
    }
}

/* Makes every object within gEventParam * 8 units start chasing (mv->chasing = 20). */
void event_startChasing(void) {
    s32 i;
    ActorCtl *ext;
    f32 dist = gEventParam * 8;

    for (i = 0; i < gPlayerCount; i++) {
        if (actor_distance3D(gCurrentActor, &movement_obj[i]) < dist) {
            ext = movement_obj[i].mv;
            if (ext != NULL) {
                ext->chasing = 20;
            }
        }
    }
}

void event_setBreakStun(void) {
    if (gCurrentActor->mv != NULL) {
        gCurrentActor->mv->break_stun = gEventParam;
    }
}

void event_skipNextEventIfNotLifting(void) {
    if (gCurrentActor->lifting == 0) {
        gCurrentActor->speccnt++;
    }
}

/* Scales the object by distance to the player: 0.3 within 200, unk148 beyond 1500, lerped between. */
void event_scaleByPlayerDistance(void) {
    f32 dist;
    f32 maxScale;
    f32 scale;
    f32 range;

    dist = actor_distance3D(movement_obj, gCurrentActor);
    if (gCurrentActor->unk148 < 0.2f) {
        gCurrentActor->unk148 = gCurrentActor->targetScaleXz;
    }
    if (dist < 200.0f) {
        scale = 0.3f;
    } else {
        maxScale = gCurrentActor->unk148;
        if (1500.0f < dist) {
            scale = maxScale;
        } else {
            range = maxScale - 0.3f;
            maxScale = (dist - 200.0f) / 1300.0f; // matching: reusing maxScale for the fraction gives IDO's register split
            scale = 0.3f + maxScale * range;
        }
    }
    gCurrentActor->targetScaleY = scale;
    gCurrentActor->targetScaleXz = gCurrentActor->targetScaleY;
    gCurrentActor->scaleStep = 0.02f;
}

void event_jumpToFrame(void) {
    gEventLoopRunning = 0;
    gEventResult = gEventParam;

    if (gEventParam == 0) {
        gEventResult = 0.1f;
        gCurrentActor->unk1FC |= 4;
        gCurrentActor->speccnt = 0;
    }
}

void event_emptyHitReaction0(Actor *self, Actor *other, u8 *result) {
}

void event_emptyHitReaction1(Actor *self, Actor *other, u8 *result) {
}

/* Collision handlers (self, other, result): */

void func_15074C00(Actor *self, Actor *other, u8 *result) {
    GroundPoint hit;

    if (((u8)other->intelhitroute & 0x7F) == 5) {
        collision_initHitFromObject(&hit.ground, other);
        hit.pos.x = gHitPointX;
        hit.pos.y = gHitPointY;
        hit.pos.z = gHitPointZ;
        prop_scatterPieces(&hit.pos, &hit.ground, 22, 1, 0xFF, 0);
    }
}

void event_hitReactProjectile(Actor *self, Actor *other) {
    s32 anim;

    if (self->headEffectState == 17) {
        effect_hitReactSmallSplash(self, other);
    }

    if (self->headEffectState == 2 || self->headEffectState == 18 || self->headEffectState == 3 || self->headEffectState == 6) {
        if (other->energy == 0) {
            if (self->headEffectState == 2) {
                anim = 0x2A;
            } else if (self->headEffectState == 18) {
                anim = 0x2F;
            } else {
                if (self->headEffectState == 3) {
                    anim = 0x39;
                } else {
                    anim = 0x51;
                }
            }
            attach_attachInPlace(self, other, anim, 12, 1, 0);
        }
        self->energy = 0;
    }

    if (other->type == 0x19) {
        effect_spawnRotatedQuadEffect(256.0f - (math_atan2(self->yspd, self->speed) * 40.743664f),
                      0xC0 - (self->angle >> 8),
                      self->xpos,
                      self->ypos,
                      self->zpos,
                      16,
                      20,
                      0,
                      0xFF,
                      0);
    }
}

void event_hitReactSetTaskInt1(Actor *self, Actor *other, u8 *result) {
    self->taskint[1] = 1;
}

void event_hitReactSetUpSmoke(Actor *self, Actor *other, u8 *result) {
    func_1516FE1C(other - movement_obj, 180, 0xFF, 0);
    particle_startActorEmitter(other, 11, 0, 1, 0xFF, 0, "mvmt_imp_set_up_smoke");
}

void func_15074E80(Actor *self, Actor *other, u8 *result) {
    u8 hitResult = 0;

    effect_hitReactJointImpactAndDrip(self, other, &hitResult);
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x60000;
        event_attachTargetToBone();
    }
}

void func_15074EE8(Actor *self, Actor *other, u8 *result) {
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x70000;
        event_attachTargetToBone();
    }
}

void func_15074F30(Actor *self, Actor *other, u8 *result) {
    self->intelwalk = other->unk109;
    self->scriptCmd = NULL;
}

/* Sends `other` towards (-2641, 3039). */
void func_15074F48(Actor *self, Actor *other, u8 *result) {
    f32 dx = -2641.0f - self->xpos;
    f32 dz = self->zpos - 3039.0f;

    other->angle = actor_getDirectionAngle(dx, dz, 0);
    other->speed = sqrtf(dx * dx + dz * dz) * 0.03f;
    effect_spawnSprayAndDrip(self, other);
}

void func_15074FD4(Actor *self, Actor *other, u8 *result) {
    if (other->type == 1) {
        self->imune = 0xFF;
        other->imune = 200;
        gEventParam = 0x60000;
        if (self->character_type == 0x8E) {
            gEventParam = 0x140000;
        }
        event_attachTargetToBone();
    }
    effect_hitReactSplash(self, other, result);
}

void event_hitReactHop(Actor *self, Actor *other, u8 *result) {
    self->vtilt = 5.0f;
    if (self->speed > 0.0f) {
        self->vtilt = -5.0f;
    }
    self->ygrav = 5.0f;
    self->yspd = 12.0f;
}

void event_hitReactSetYGravity(Actor *self, Actor *other, u8 *result) {
    self->ygrav = 5.0f;
}

void func_150750C4(Actor *self, Actor *other, u8 *result) {
    if (other->character_type == 0x88 && self->ivehitit < 55) {
        self->speccnt += 3;
    }
    effect_hitReactJointImpactAndDrip(self, other, result);
    if (other->character_type != 0x53) {
        func_15145A50(other);
        other->energy = 0;
        actor_kill(other, 8);
        *result = 39;
    }
}

/* Returns the first live object owned by `owner` (unk65 = owner index + 1), or NULL. */
Actor *event_getFirstLiveOwnedObject(Actor *owner) {
    Actor *obj;
    s32 i;

    obj = movement_obj;
    for (i = 0; i < 26; i++) {
        if (obj->type != 0 && obj->character_type != 0xFF && obj->attach_char == (owner - movement_obj) + 1) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

