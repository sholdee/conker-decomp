#ifndef CONKER_CUTSCENE_H
#define CONKER_CUTSCENE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "cutscene": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Cutscene/Sequence/sequence.c
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

/* The sound voice handed to the cutscene_updateSound callback. */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ SeqSound *userData;
} SeqVoice;

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
s32 cutscene_getLookTarget(f32 *outX, f32 *outY, f32 *outZ, f32 *target, s32 objIndex, s32 scene);
s32 cutscene_getHeadAngles(f32 *outA, f32 *outB, s32 objIndex, s32 scene);
void cutscene_setAnchor(s16 x, s16 y, s16 z, f32 value);
void cutscene_setNextAnchor(s16 x, s16 y, s16 z, f32 value);
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
/* Defined outside the C files; sequence.c uses them: */
extern u64 D_800CBE10[0x400 / sizeof(u64)]; /* RSP dram stack of the graphics tasks (bss_800CBE10, a .bss slice of conker.us.yaml) */
extern u8 ucode_ROM_START[];                 /* ROM start of the RSP microcodes (linker-defined, undefined_syms.us.txt) */

/* game/Cutscene/StartCallback/startcallback.c */
void cutscene_startCallback1(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game/Cutscene/SeqEvents/seqevents.c */
void cutscene_emptyStartHook(void);
s32 cutscene_updateSound(SeqVoice *voice, s32 arg1, u32 *volume, s32 arg3, s32 arg4, s32 arg5, s16 *arg6);
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
EvTimer *cutscene_startTimer(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 arg5,
                       s32 arg6);
void cutscene_updateTimer(EvTimer *timer);
void cutscene_spawnDebrisBurst(Vec3f *pos, u16 id, f32 scale, u8 arg3, s32 arg4);
void cutscene_spawnTrackEffect(SeqTrackState *track, u16 id, s16 duration, u8 arg3, s32 arg4);
s32 cutscene_updateTrackEffect(TrackPhysicsParticle *obj);
void cutscene_spawnTrackParticle(SeqTrackState *track, s32 texture, s16 lifetime, u8 arg3, s32 arg4);
s32 cutscene_updateTrackParticle(TrackParticle *particle);

#endif
