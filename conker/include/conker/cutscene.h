#ifndef CONKER_CUTSCENE_H
#define CONKER_CUTSCENE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "cutscene": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Cutscene/Sequence/sequence.c
 *   game_data_800CBE10.c (data only, owner unknown; linked after game/Collision/Probe/probe)
 *   game/Cutscene/StartCallback/startcallback.c
 *   game/Cutscene/SeqEvents/seqevents.c
 *   game/Cutscene/EventEffects/eventeffects.c
 */

/* An event queued for dispatch by cutscene_queueEvent, kept sorted by time. */
typedef struct SeqQueueEntry {
    /* 0x0 */ SeqEvent *event;
    /* 0x4 */ s32 time;
    /* 0x8 */ u8 track;
    /* 0xA */ u16 index;
} SeqQueueEntry;

/* Per-sound state allocated by cutscene_handleEvent3 for a cutscene sound event (0x10 bytes). */
typedef struct {
    /* 0x0 */ u8 scene;
    /* 0x1 */ u8 track;
    /* 0x2 */ u8 stopAtEnd;  /* stop when the scene reaches its end time */
    /* 0x4 */ u16 remaining; /* ticks left, 0xFFFF = unlimited */
    /* 0x6 */ u16 duration;
    /* 0x8 */ u16 fadeIn;
    /* 0xA */ u16 fadeOut;
    /* 0xC */ u16 volume;
    /* 0xE */ u8 sceneTag;   /* gCutsceneIds[scene] when the sound started */
} SeqSound;

/* Owner of a cutscene_startTimer timer (an attachment handle in game/Cutscene/SeqEvents/seqevents.c): `timer` holds the running timer. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct EvTimer *timer;
} EvTimerOwner;

/* Effect created by cutscene_startTimer (0x2C bytes), counted down by cutscene_updateTimer. */
typedef struct EvTimer {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ f32 *unk18;
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ s16 timeLeft;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ EvTimerOwner *owner;
    /* 0x28 */ u8 keepOwnerLink; /* 0: clear owner->timer on expiry */
    /* 0x29 */ u8 pad29[3];
} EvTimer; // size 0x2C

/* An object spawned by particle_newModelParticleWithFloor (ProjectileParticle in game/Effects/ProjectilePuffs/projectilepuffs.c), here following a cutscene track. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ u8 pad44[0x12C];
    /* 0x170 */ SeqTrackState *track;
} TrackPhysicsParticle;

/* A particle spawned by particle_newQuadParticle (a DefParticleHead, as BoneBeamParticle in game/Effects/BoneBeamParticle/bonebeamparticle.c), here following a cutscene track. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ SeqTrackState *track;
} TrackParticle;

/* game/Cutscene/Sequence/sequence.c */
void cutscene_sendGfxTask(s32 unused, s32 swapBuffers);
void cutscene_loadMicrocodes(void);
void cutscene_clearActiveEvents(s32 scene);
void cutscene_computeEndTime(s32 scene);
s32 cutscene_countEvents(s32 scene);
void cutscene_resetScene(s32 scene);
struct SeqFileEntry *cutscene_loadFile(s32 map, s32 cutscene, s32 scene);
void cutscene_setPlayed(s32 map, s32 cutscene);
s32 cutscene_isPlayed(s32 map, s32 cutscene);
void cutscene_start(s32 map, s32 cutscene, s32 scene, s32 callback, s32 callbackArg);
void cutscene_prescanEvents(s32 scene);
void cutscene_jumpToEnd(s32 scene);
void cutscene_skip(s32 scene);
s32 cutscene_canSkip(s32 scene);
void cutscene_updateSkip(s32 scene);
void cutscene_checkEnd(s32 scene);
void cutscene_advanceTime(s32 scene);
void cutscene_freeScene(s32 scene);
void cutscene_resetBoundFaces(s32 scene);
void cutscene_finish(s32 skipped, s32 scene);
void cutscene_restoreControl(s32 scene);
void cutscene_evalTracks(s32 scene);
s32 cutscene_findPathSegment(s32 track, f32 dist, f32 *out, s32 scene, s32 *segmentOut);
void cutscene_getTrackPos(s32 track, f32 percent, f32 *x, f32 *y, f32 *z, s32 scene);
void cutscene_getTrackHeading(s32 track, f32 percent, f32 *heading, s32 scene);
void cutscene_getSegmentKeys(f32 *out, SeqPathKey *keys, s32 key, s32 numKeys);
void cutscene_measurePath(s32 track, s32 scene);
void cutscene_cacheSegment(s32 track, s32 segment, s32 scene);
void cutscene_applyTracks(s32 scene);
void cutscene_applyObjectTrack(Actor *target, s32 scene, s32 onlyTrack);
s32 cutscene_getLookTarget(f32 *yaw, f32 *pitch, f32 *roll, f32 *target, s32 objIndex, s32 scene);
s32 cutscene_getHeadAngles(f32 *yaw, f32 *pitch, s32 objIndex, s32 scene);
void cutscene_setAnchor(s16 x, s16 y, s16 z, f32 heading);
void cutscene_setNextAnchor(s16 x, s16 y, s16 z, f32 heading);
void cutscene_resetSpawnedActors(s32 scene);
void cutscene_resetActivatedActors(s32 scene);
void cutscene_addSpawnedActor(s32 id, s32 scene);
void cutscene_resetTrackFace(s32 track, s32 scene);
void cutscene_spawnTrackActor(s32 track, s32 scene);
void cutscene_applyObjectTracks(s32 scene);
void cutscene_addActivatedActor(s32 id, s32 scene);
void cutscene_activateTrackActor(s32 track, s32 scene);
void cutscene_activateTrackActors(s32 scene);
void cutscene_restoreInactive(s32 scene);
void cutscene_spawnTrackActors(s32 scene);
void cutscene_removeSpawnedActors(s32 scene);
void cutscene_runActorHandler(Actor *obj);
s32 cutscene_isActorBound(Actor *obj);
s32 cutscene_isActorIndexBound(s32 actorIndex, s32 scene);
void cutscene_anchorPaths(s32 scene);
s32 cutscene_pickCameraTrack(s32 scene, s32 time);
void cutscene_freeSubtitles(void);
void cutscene_closeSubtitles(void);
void cutscene_releaseSubtitle(SeqSubtitle *sub, s32 force);
SeqSubtitle *cutscene_getSubtitleSlot(s32 track, s32 replaceOpen);
s32 cutscene_releasePlayer(s32 scene);
void cutscene_updateActors(void);
extern u64 *gUcodeData; // RSP ucode data, right after its text in gUcodeText
extern u8 gStartWithMainUcode;
extern s32 gDisplayListLimit;
extern u64 gUcodeText[0x496]; // RSP ucode text buffer
extern s32 gCutsceneTimes[2]; // per scene: current time
extern s32 gCutscenePrevTimes[2]; /*time at the previous update */
extern u8 gCutsceneEventsDispatched[2];
extern u8 gNextCutscene;
extern u8 gNextCutsceneMap;
extern u8 gPendingCutscene;
extern u8 gPendingCutsceneFromIdle;
extern u16 *gSeqTrackEventCounts[2]; /*each track's event count */
extern u8 **gSeqTextLists[2]; // per scene: text lists
extern u8 gCutsceneIds[2]; // per scene: changes with each run of the scene
extern u8 gCutsceneStates[2]; // per cutscene scene slot (0/1): cutscene state, 1 = playing
extern SeqTrackInfo *gSeqTrackInfos[2]; // per scene: what each track drives
extern u8 gSeqCameraTrack;
extern CamShot gCamShot; // scripted camera shot
extern u8 gCutsceneActorLock;
extern u8 gSeqTrackCounts[2]; /*track count */
extern s32 gCutsceneEndTimes[2]; /*end time */
extern f32 gViewFovOffset; // added to both FOVs of every view
extern f32 gNearPlaneOverride; // near-plane override when non-zero
extern f32 gFarPlaneOverride; // far-plane override when non-zero
extern u8 gCutsceneUpdateUnboundActors[2];
extern u8 gCutsceneShowUnboundActors[2];
extern u8 gCutsceneShowUnboundObjects[2];
extern u8 gCutsceneKeepSpawnedActors[2];
extern u8 D_800C365C[2];
extern u8 gCutsceneRunTimedEvents[2];
extern u8 gCutsceneUseCullDistance[2];
extern u8 gCutsceneHideWorld;
extern u8 gCutsceneAnchorSet;
extern u8 gCutsceneIgnorePlayed;
extern u8 gSpawnCutsceneActive;
extern u8 gCutsceneSkipped;
extern f32 gCutsceneCutbackAngle;
extern f32 gCutsceneEndTransitionYaw; // camera transition parameter (999.0f = unset); gCutsceneEndTransitionTime is its duration
extern s32 gCutsceneEndTransitionTime;
extern u8 gCutsceneKeepCamera;
extern u8 gCutsceneHideSky;
extern u8 gCutsceneSolidSky;
extern u8 gCutsceneNoPause;
extern SeqEvent *gSeqEventTracks[2][30]; // cutscene script event tracks per scene
extern SeqPathKey *gSeqPathKeys[2][30]; // per scene and track: path keyframes
extern SeqTrackState *gSeqTrackStates[2]; // per scene: the track states
extern u8 *gSeqActiveEvents[2][30];
extern u8 gCutsceneExitCountdown;
extern s64 gPlayedCutscenes[69];
extern u8 gCutsceneSkipState;
extern u8 gCutsceneSkipFadeTimer;
extern s8 gCutsceneFadeToColor;
extern u8 gCutsceneFadeInAtEnd;
extern u8 gCutsceneHoldEnd;
extern u8 gCutsceneNoSkipFadeIn;
extern u16 gCutsceneStartTime;
extern u8 gCutsceneCallback;
extern s32 gCutsceneCallbackArg;
extern u8 gLastCutscene;
extern u8 gCutsceneChained;
extern u16 gSeqEventQueueSize;
extern u8 gCutsceneNoSkip;
extern u8 gCutsceneChainBobScale;
extern s16 gCutsceneCashOffset;
extern SeqSubtitle gSubtitles[3];
extern struct SeqQueueEntry *gSeqEventQueue; // dispatch queue, gSeqEventQueueSize entries (type in 50D80)
/* Defined outside the C files; sequence.c uses it: */
extern u8 ucode_ROM_START[];                 /* ROM start of the RSP microcodes (linker-defined, undefined_syms.us.txt) */

/* game_data_800CBE10.c (data only, owner unknown; linked after game/Collision/Probe/probe) */
extern u64 gRspDramStack[SP_DRAM_STACK_SIZE64]; /* RSP dram stack of the graphics tasks */

/* game/Cutscene/StartCallback/startcallback.c */
void cutscene_startCallback1(s32 map, s32 cutscene, s32 scene, s32 actorPath);

/* game/Cutscene/SeqEvents/seqevents.c */
void cutscene_emptyStartHook(void);
s32 cutscene_updateSound(SoundRecord *voice, s32 active, u32 *volume, s32 pan, s32 cents, s32 mix, s16 *sound);
s32 cutscene_findEvent(SeqEvent *after, s32 type, s32 track, SeqEvent **found, s32 matchValue, s32 value,
                  s32 matchUnk4, s32 unk4, s32 matchUnk5, s32 unk5, s32 scene);
void cutscene_findEventsAround(SeqEvent **prev, SeqEvent **next, s32 *prevTime, s32 *nextTime, s32 track, s32 type,
                   s32 matchValue, s32 value, s32 matchUnk4, s32 unk4, s32 time, s32 scene);
void cutscene_flushEventQueue(s32 count, s32 scene);
void cutscene_queueEvent(SeqQueueEntry *queue, s32 *count, SeqEvent *event, s32 track, s32 index, s32 time, s32 scene);
void cutscene_dispatchEvents(s32 mode, s32 scene);
s32 cutscene_handleEvent1(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag);
s32 cutscene_handleEvent2(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag);
s32 cutscene_handleEvent3(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag, s32 skipping);
void cutscene_dispatchEvent(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene);
extern Color24 gCutsceneFadeColours[2]; /* screen fade colours for screen_startViewFade, picked by sequence events */

/* game/Cutscene/EventEffects/eventeffects.c */
EvTimer *cutscene_startTimer(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 fxOwner,
                       s32 fxList);
void cutscene_updateTimer(EvTimer *timer);
void cutscene_spawnDebrisBurst(Vec3f *pos, u16 id, f32 scale, u8 owner, s32 list);
void cutscene_spawnTrackEffect(SeqTrackState *track, u16 id, s16 duration, u8 owner, s32 list);
s32 cutscene_updateTrackEffect(TrackPhysicsParticle *obj);
void cutscene_spawnTrackParticle(SeqTrackState *track, s32 texture, s16 lifetime, u8 owner, s32 list);
s32 cutscene_updateTrackParticle(TrackParticle *particle);

#endif
