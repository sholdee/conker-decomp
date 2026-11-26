#ifndef CONKER_ACTOR_H
#define CONKER_ACTOR_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "actor": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Actor/ClearModelTables/clearmodeltables.c
 *   game/Actor/ClearActors/clearactors.c
 *   game/Actor/DefaultSize/defaultsize.c
 *   game/Actor/ObjectUpdate/objectupdate.c
 *   game/Actor/Movement/movement.c
 *   game/Actor/ActorMotion/actormotion.c
 *   game/Actor/ActorActions/actoractions.c
 *   game/Actor/MovementHit/movementhit.c
 *   game/Actor/ObjectSize/objectsize.c
 *   game/Actor/KillRespawn/killrespawn.c
 *   game_DE5A0.c
 *   game/Effects/Greyscale/greyscale.s
 *   game/Actor/Follower/follower.c
 */


// 3-byte entries of D_80099140
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
} Struct80099140;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair1506AA48;

typedef struct {
    /* 0x00 */ Actor *unk0;
    /* 0x04 */ Vec3f unk4;
    /* 0x10 */ s32 unk10;
} Arg1506AB7C;

/* game/Actor/ClearModelTables/clearmodeltables.c */
void actor_clearModelTables(void);

/* game/Actor/ClearActors/clearactors.c */
void actor_clearAll(void);

/* game/Actor/DefaultSize/defaultsize.c */
void actor_setDefaultSizes(void);

/* game/Actor/ObjectUpdate/objectupdate.c */
Gfx *actor_drawPass(Gfx *gdl, s32 mode, s16 player);
void actor_updateObject(Actor *obj, s32 index);
void actor_updateObjects(void);
void actor_updateBlobShadows(void);
void actor_resetBonePool(void);
s32 actor_getSpriteTexture(s32 index);
Gfx *actor_drawSprite(Gfx *gdl, s32 index);
void actor_scrollSpriteTexture(s32 index);
void actor_selectLod(s32 index, s16 player, s32 mode);
Gfx *actor_drawModel(Gfx *gdl, s32 index, s32 player, s32 mode, s32 wrap);
void actor_getDrawColours(Actor *obj, s32 player, s32 alpha, s32 *colour, s32 *primR, s32 *primG, s32 *primB, s32 *envR,
                   s32 *envG, s32 *envB, s32 *fogR, s32 *fogG, s32 *fogB, s32 *fogA);
Gfx *actor_emitModel(Gfx *gdl, s32 index, s32 player, void *model, s32 alpha, s32 *colour, s32 mode, s32 wrap);
void func_1502D54C(s32 index, s32 *colour);
/* (Actor *obj, s32 *colour, s32 player); declared without a prototype: game/Attach/Attachment/attachment.c passes 4 arguments */
void actor_getFogAlpha();
void actor_loadAnimFrames(AnimInstance *anim, Actor *obj, u8 *buf);
s32 actor_getBoneMatrixCount(s32 id);
void actor_computeBones(MtxF mtx, s32 index);
void actor_buildRootMatrix(s32 index, s32 rootOnly);
void actor_flushBoneMatrices(void);
void actor_updateCulling(s32 index);
void func_1502E9FC(Actor *obj, s32 dist);
void actor_startFogPulse(Actor *obj, s32 r, s32 g, s32 b, s32 a, s32 rate);
void actor_fadeFogPulse(Actor *obj);
void actor_startFogFadeOut(Actor *obj, s32 rate);
void actor_startFogFadeIn(Actor *obj, s32 rate);
void actor_flashFog(Actor *obj, s32 r, s32 g, s32 b, s32 a, s32 instant, s32 rate);
void actor_updateFog(Actor *obj);
/* (Actor *obj, s32 *r, s32 *g, s32 *b, s32 *a); declared without a prototype: game/Effects/HoltenRope/holtenrope.c passes a u32 *a */
void actor_getFogColour();
s32 actor_getEyeTarget(s32 index, s32 slot);
void actor_updateEyes(s32 index);
Gfx *actor_bindSwapTextures(Gfx *gdl, s32 index);
void actor_followParent(s32 index);
void actor_carryStandingObjects(void);
void actor_movePointWithPlatform(Actor *platform, f32 *x, f32 *y, f32 *z, s32 tri);
void actor_savePrevBones(Actor *obj);
Gfx *actor_setTextureScroll(Gfx *gdl, s32 index);
void actor_applyForcedLod(Actor *obj);
void actor_refreshModelUnload(Actor *arg0);
extern Gfx gObjectPassBeginDL[]; // opens a pass: modelview matrix gIdentityMtx, lit smooth shading
extern Gfx gObjectPassEndDL[]; // closes a pass: clears the lighting modes and the prim/env colours
extern u8 dc_glbl_CharNum;
extern u16 gBoneMatricesUsed; // bone matrices used this frame
extern u16 gBonePoolSize; // bone-matrix pool size, in 0x40-byte matrices
/* The bone-matrix pool of each frame buffer. Bones are built in it as MtxF; actor_flushBoneMatrices converts them in
   place to Mtx for drawing. The cursors move in bytes. */
extern u8 *gBonePools[2];
extern u8 *gBonePoolCursor; // bone-matrix pool cursor
extern u8 *gBonePoolStart; // bone-matrix pool start
extern u8 gBoneMatricesFlushed;

/* game/Actor/Movement/movement.c */
void actor_updateAll(void);
void func_1504ADD0(void);
s32 func_1504AEF4(s32 arg0, s32 *arg1);
void actor_updateAttachedObject(Actor *obj, s32 force, s32 release);
f32 actor_updateSwim(Actor *obj, f32 arg1);
void func_1504BA38(Actor *obj);
void func_1504BAF0(Actor *obj);
void func_1504BB88(Actor *obj);
void actor_tickTimers(Actor *obj);
void actor_updatePissed(Actor *obj, u16 *arg1, f32 *arg2, s8 *arg3);
s32 actor_updateIdleAnim(void);
s32 actor_getDuckWalkAnim(void);
void actor_updateWalkAnim(Actor *obj, s32 arg1, f32 arg2);
void actor_updateButtonTimers(Actor *obj);
s32 actor_interleaveUpdate(Actor *obj);
void func_1504C9E4(Actor *obj, s8 target, u8 mode);
void func_1504CA60(Actor *actor);
void actor_updatePlayer(Actor *actor);
void actor_tiltToGround(Actor *actor);
void func_15051558(Actor *actor, f32 x, f32 z, u16 facing);
f32 actor_slopeToAngles(Actor *actor, f32 slopeX, f32 slopeZ, f32 *pitch, f32 limit);
void func_15052260(Actor *actor);
void actor_leaveWater(Actor *actor);
void func_15052458(Actor *unused);
void actor_removeIfDead(Actor *actor);
void func_15052490(Actor *actor, u16 angle, f32 amount, f32 scale);
void func_1505250C(Actor *actor, s32 unused);
void actor_updateCommon(Actor *obj);
void actor_leanIntoTurn(Actor *obj, s16 prevAngle);
void actor_updatePathFollower(Actor *obj);
void func_15052EF0(Actor *obj);
Actor *func_15052F58(s32 index, s32 arg1);
/* void actor_seatRider(Actor *, f32 height, ...) is declared in the files that call it:
   game/Effects/TintedObject/tintedobject.c was compiled with an s32 return. */
void actor_startRiderJump(Actor *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex);
s32 actor_handleDeath(Actor *obj);
s32 actor_checkDeath(Actor *obj);
void func_15053694(Actor *obj);
void func_150536D0(Actor *obj);
void func_150536E8(Actor *obj);
void func_1505371C(Actor *obj);
void actor_stepScale(Actor *obj);
void func_15053894(Actor *obj);
void func_150538CC(Actor *obj);
void func_150548D8(Actor *obj);
void func_150548E4(Actor *obj);
void actor_removeAtAnimEnd(Actor *obj);
void func_15054A5C(Actor *obj, Actor *other);
void func_15054A94(Actor *obj);
void func_15054F74(Actor *obj);
void func_15055260(Actor *obj, Actor *target, s32 hitIndex);
void func_1505528C(Actor *obj);
void actor_playImpactSound(s32 arg0, f32 x, f32 y, f32 z, s32 arg4);
void func_15055B0C(Actor *obj, s32 duration);
void func_15055B64(Actor *obj);
void actor_updateSinkAway(Actor *obj);
void actor_startOverEdge(Actor *obj);
void actor_updateOverEdge(Actor *obj);
extern s32 D_80086000;
extern void (*gActorUpdateHandlers[])(Actor *); // per-type update handlers
extern s32 D_800CC250;
extern f32 D_800CC254; // object position saved at the hit test (x, y, z)
extern f32 D_800CC258;
extern f32 D_800CC25C;
extern s32 gAttackHitMask;
extern s16 gActorStepPercent; // frame speed in percent
extern s32 gCollidedActors;
extern u8 D_800CC26C;
extern u8 D_800CC26D;
extern u8 D_800CC26E;
extern f32 gHitAttackerX; // attacker position saved at the hit (x, z, y)
extern f32 gHitAttackerZ;
extern f32 gHitAttackerY;
extern u8 gFloorMaterialClass; // floor material class of the object being updated
extern u16 D_800CC27E;
extern s32 D_800CC280;
extern OSContPad *gCurrentPad; // the current player's controller input
extern s32 gActorPressedButtons;
extern s32 gActorNewButtons;
extern s32 gActorHeldButtons;
extern s32 D_800CC294; // not referenced by any code
extern TexAnimState D_800CC298;
extern u8 gActionsPending;
extern u8 D_800CC2B1;
extern s16 gWaistTiltTarget;
extern f32 gWaistTiltDamping;
extern u8 gActorUpdateParity;
extern u16 D_800CC2BA; // frame step of the actor being updated
extern s32 D_800CC2BC;
extern f32 gHitPointX;
extern f32 gHitPointY;
extern f32 gHitPointZ;
extern Actor movement_obj[26]; // the actors
extern s32 gActorFrameCount;
extern Actor *gCurrentActor; // the actor being updated
extern f32 gActorTimeStep; // frame time step

/* game/Actor/ActorMotion/actormotion.c */
void actor_clampToZoneSet(Actor *actor, s32 zoneSetId);
void actor_clampToPathRange(Actor *actor);
void actor_followWaterLevel(Actor *actor);
void actor_moveToHost(Actor *actor);
s32 actor_isTargetInPathArea(Actor *actor, s32 targetIdx);
void func_15056A00(Actor *actor, u8 turn, u8 row);
void actor_updateChaseAi(Actor *actor);
void actor_playMoveAnim(Actor *actor, f32 idleRate);
void actor_updateStunned(Actor *actor, f32 decel);
void actor_updateFloorAndWalls(Actor *actor, f32 prevY);
void func_15058EA4(Actor *actor, f32 maxY, f32 gravityAbove, f32 minY, f32 gravityBelow, f32 maxVelY, f32 minVelY);
void actor_floatOnWater(Actor *actor, f32 depth, f32 arg2);
void actor_updatePhysics(Actor *actor);
void actor_easePushVelocity(Actor *actor, u16 angle, f32 speed, f32 rate);
void actor_applyWaterCurrent(Actor *actor);
void actor_attachToHost(Actor *actor, s32 hostIdx);
void actor_checkGrab(Actor *actor);
u8 actor_turnFacing(Actor *actor, u8 turnRate, u16 targetAngle);
u8 actor_turnMoving(Actor *actor, u16 maxStep);
void actor_moveHorizontal(Actor *actor);
void actor_splitVelocity(u16 angle, f32 speed, f32 pitch, f32 *outX, f32 *outZ, f32 *outY);
void actor_stepVector2D(f32 targetX, f32 targetZ, f32 rate, f32 *x, f32 *z);
void actor_accelerate(f32 targetSpeed, Actor *actor, f32 accel, f32 decel, u8 turnDiff);
f32 actor_getStickDeflection(OSContPad *pad);
u16 actor_getDirectionAngle(f32 x, f32 z, s32 unused);
f32 actor_distanceXZ(Actor *a, Actor *b);
f32 actor_distance3D(Actor *a, Actor *b);
void actor_applyGravity(Actor *actor);
void actor_landPlayer(Actor *actor, f32 floorY);
void actor_landNonPlayer(Actor *actor, f32 floorY);
void actor_applyHit(Actor *attacker, Actor *victim, const HitReaction *base, const HitReaction *hit, u8 index, s32 victimSlot,
                   u8 damageMask);
s16 actor_getAttackAngle(Actor *attacker, Actor *victim);
const HitReaction *actor_getHitReactions(Actor *actor);
s32 actor_resolveHit(Actor *attacker, Actor *victim, const HitReaction *table, s32 index, s32 victimSlot, s32 hitMask,
                  s32 damageMask);
s32 actor_testAttack(Actor *attacker, s32 selfSlot);
s32 actor_hitScripted(Actor *victim, s32 flags, u16 angle, s32 sourceSlot);
s32 actor_runAttackAt(f32 x, f32 y, f32 z, s32 attackId, s32 sourceSlot, u16 angle, s32 losSkipSlot, s32 losOrigin);
void actor_startKnockback(Actor *actor, u8 entryIdx);
f32 actor_rotateOffset(f32 angleDeg, f32 a, f32 b, f32 scale, f32 *outZ);
void actor_pushOut(Actor *actor, Actor *other, f32 nx, f32 ny, f32 nz, f32 distSq, f32 minDist, s32 otherSlot);
void actor_pushOutCylinder(Actor *actor, Actor *other, f32 x, f32 y, f32 z, f32 ratio, f32 radius, s32 otherSlot,
                   f32 extraRadius, f32 extraHeight, f32 extraY, f32 offsetA, f32 offsetB);
void actor_resolveCollisions(Actor *actor, s32 selfIdx);
u8 actor_findTargetInCone(Actor *actor, u16 *outAngle, u8 pitch, u8 skipIdx, u8 halfCone);
s8 actor_findPlayerAhead(Actor *actor, s32 unused, u16 cone, f32 time);
f32 actor_getDistSqTo(Actor *actor, u8 otherIdx, u16 *outAngle, f32 *outDistSqXZ, f32 *outDy, s32 unused);
void actor_stopAnim(Actor *actor);
void actor_saveBlendAnim(AnimInstance *state);
void actor_startAnimState(Actor *actor, u16 *animEntry, AnimInstance *state, s32 arg3, s32 restart, s32 anim,
                   s32 bank, f32 speed, f32 blendFrames, f32 arg9, f32 arg10, s32 flag8000);
void actor_playAnim(Actor *actor, u16 anim, f32 speed, f32 blend, f32 arg4, f32 arg5, s32 flag8000);
u32 actor_findReaction(s32 kind, Actor *actor);
void actor_advanceReaction(s32 sourceIdx, Actor *actor);
Actor *actor_alloc(void);
Actor *actor_findByState(s32 state, s32 *outIdx);
Actor *actor_findOnPath(s32 pathIdx);
Actor *actor_findByPlayerNumber(s32 key);
Actor *actor_findByType(s32 id);
void actor_clearSlot(Actor *actor);
void actor_updatePlayerControls(Actor *actor, OSContPad *pad);
void actor_runPlayerAction(Actor *actor, s32 buttons, s32 pressed, f32 height, s32 nested);
void actor_playPlayerSound(s32 sound, Actor *actor, u16 rate, s32 volume, s32 minDist, s32 maxDist, s32 flags);
void actor_playSound(s32 sound, Actor *actor);
void actor_playUntrackedSound(s32 sound, Actor *actor);
void actor_playUntrackedSoundRate(s32 sound, Actor *actor, s32 rate);
void func_15060B70(s32 sound, void *actor);
s32 actor_addHealth(Actor *actor, s32 amount);
void actor_updateHeadPos(Actor *actor);
void actor_releaseCarried(Actor *actor);
void actor_remove(Actor *actor, s32 mode);
void actor_driftOnCurrent(Actor *actor, s32 pathIdx);
void actor_resetFade(Actor *actor);
void actor_fadeCommand(Actor *actor, u8 cmd, u8 value, u8 step, s32 player);
void actor_updateFade(Actor *actor);
s32 actor_getAlpha(Actor *actor, s32 player);
void actor_updateVisibility(void);
void actor_updateTypeAnims(void);
s32 actor_isVisible(Actor *actor);
void actor_releaseCarriedNonPlayers(Actor *actor, s32 mode);
void actor_onAnimChanged(Actor *actor);
void actor_swapSlots(Actor *a, Actor *b);
void actor_updateBodyRatios(Actor *actor);
void actor_setBodyRadius(Actor *actor, f32 radius);
void actor_setBodyHeight(Actor *actor, f32 height);
void actor_updateHitRatios(Actor *actor);
void actor_setScale(Actor *actor, f32 xzScale, f32 yScale);
void actor_scrollTypeTexture(s32 type, s32 tile, s32 ds, s32 dt, s32 set, s32 absolute);
void func_15062E24(s32 type, s32 firstVtx, s32 arg2, s32 arg3, s32 arg4, s32 set, s32 doubleSize);
void actor_scrollVertexTexCoords(Actor *actor, s32 group, s32 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt);
void actor_igniteTouching(Actor *actor);
void actor_respawnInPlace(Actor *actor, s32 type, s32 interaction_state, f32 scale);
extern const u16 D_80099420[4];
extern const u16 D_80099428[2];
extern const s32 D_8009942C[3];

/* game/Actor/ActorActions/actoractions.c */
s16 actor_getAngleToLinked(Actor *arg0);
void func_15063404(Actor *arg0);
void func_150634E4(Actor *arg0);
void func_15063570(Actor *arg0);
void actor_castAimRay(Actor *arg0, f32 arg1);
void actor_markRayTargetSniped(Actor *arg0);
void actor_runPending(void);
s32 actor_getWaistOffsetToLinked(Actor *arg0);
s32 actor_turnWaist(Actor *arg0, s32 buttons, s32 mask);
void actor_throwGrenade(Actor *arg0);
Actor *actor_findLinked(Actor *arg0, s32 id);
s32 func_15063E84(Actor *arg0, u16 anim, u16 flags, s32 action, s32 buttons);
void actor_fireShot(Actor *arg0, s32 kind);
u16 actor_pickRandomAnim(Actor *arg0, u16 animA, u16 animB, u16 animC);
void func_150642AC(Actor *arg0, s32 buttons, f32 arg2, s32 noPlay, s32 lockB);
void actor_copyAnimPosition(s32 srcIndex, s32 dstIndex);
void actor_syncAnimToLinked(Actor *arg0);
s32 actor_updateMoveAnim(Actor *arg0, f32 arg1, f32 arg2, s32 overrideAnim, f32 arg4);
s32 func_15065A5C(Actor *arg0, s32 heldButtons, s32 pressedButtons, f32 arg3, s32 arg4, s32 act, s32 arg6, s32 arg7,
                  s32 arg8);

/* game/Actor/MovementHit/movementhit.c */
void actor_hitBurstDebris(Actor *obj, s32 arg1);
void actor_hitAttachBloodDrip(Actor *obj, s32 arg1);
void actor_hitShotImpact(Actor *obj, s32 arg1);
void actor_hitMidpointImpact(Actor *obj, s32 arg1);
void func_1506A79C(Actor *obj, s32 arg1);
void func_1506A83C(Actor *obj, s32 arg1);
void actor_hitSpawnFlash(Actor *obj, s32 arg1);
void func_1506A968(Actor *obj, s32 arg1);
void actor_hitSpawnTrails(Actor *obj, s32 arg1);
void func_1506AA48(Actor *obj, Pair1506AA48 *arg);
void actor_hitBloodImpacts(Actor *obj, s32 arg1);
void func_1506AB7C(Actor *obj, Arg1506AB7C *arg);
void func_1506ABC4(Actor *obj, Arg1506AB7C *arg);
void actor_hitSpawnRibbon(Actor *obj, s32 arg1);
void actor_hitSpawnWaveTrail(Actor *obj, s32 arg1);
void actor_runHitHandler(Actor *obj, s32 type, void *arg);

/* game/Actor/ObjectSize/objectsize.c */
void actor_updateSizes(void);
void actor_computeSize(Actor *obj, s16 *outHeight, s16 *outWidth, s16 *outDepth);

/* game/Actor/KillRespawn/killrespawn.c */
void actor_startDeathCountdown(Actor *obj, s32 timer);
void actor_updatePlayerStates(void);
void actor_closeDeathIris(Actor *obj);
void actor_kill(Actor *obj, s32 cause);
void func_1507D158(s32 slot, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void actor_respawn(Actor *obj);
void actor_finishDeath(s32 slot);
s32 actor_runDeathEffect(Actor *obj);
void func_1507DB34(void);
void func_1507DB3C(void);
void actor_breakIntoParts(Actor *obj, s32 arg1);
void func_1507DB64(void);
Gfx *func_1507DB6C(Gfx *gfx, s32 player);
extern u8 D_D0F; // texture id 0xD0F as a symbol (undefined_syms.us.txt): func_1507DB6C passes its address
void func_1507DE4C(Actor *obj);
void func_1507DF10(Actor *obj, s32 part);
void func_1507DFE4(s32 resource, s32 arg1);
s32 func_1507E114(s32 *arg0);
void actor_getTrackPoint(Actor *obj, f32 *x, f32 *y, f32 *z);
extern u16 gEliminatedPlayers;
extern u16 D_800D18A2;
extern u8 D_800D18A4[4]; // per player: half-size of the square, minus 4
extern u8 D_800D18A8;
extern u8 D_800D18A9;

/* game_DE5A0.c */
void func_150B10F0(Actor *obj);
void func_150B12FC(Actor *obj);
void func_150B1484(Actor *obj);
void func_150B17DC(Actor *obj);
void func_150B19E0(Actor *obj);

/* game/Effects/Greyscale/greyscale.s: rearranges the 64-bit words from start up to end */
void colourfb_toGreyscale(void *start, void *end);

/* game/Actor/Follower/follower.c */
void actor_updateFollower(Actor *self);

#endif
