#ifndef CONKER_CUTSCENE_H
#define CONKER_CUTSCENE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "cutscene": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_49D30.c
 *   game_50D20.c
 *   game_50D80.c
 *   game_1CBE20.c
 */

/* An event queued for dispatch by func_150241B4, kept sorted by time. */
typedef struct SeqQueueEntry {
    /* 0x0 */ SeqEvent *event;
    /* 0x4 */ s32 time;
    /* 0x8 */ u8 track;
    /* 0xA */ u16 index;
} SeqQueueEntry;

/* Per-sound state allocated by func_15029BB8 for a cutscene sound event (0x10 bytes). */
typedef struct {
    /* 0x0 */ u8 scene;
    /* 0x1 */ u8 track;
    /* 0x2 */ u8 stopAtEnd;  /* stop when the scene reaches its end time */
    /* 0x4 */ u16 remaining; /* ticks left, 0xFFFF = unlimited */
    /* 0x6 */ u16 duration;
    /* 0x8 */ u16 fadeIn;
    /* 0xA */ u16 fadeOut;
    /* 0xC */ u16 volume;
    /* 0xE */ u8 sceneTag;   /* D_800C35E8[scene] when the sound started */
} SeqSound;

/* The sound voice handed to the func_150238D8 callback. */
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

/* Owner of a func_1519E970 timer (an attachment handle in game_50D80.c): `timer` holds the running timer. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ struct EvTimer *timer;
} EvTimerOwner;

/* Effect created by func_1519E970 (0x2C bytes), counted down by func_1519EA04. */
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

/* An object spawned by func_1513264C (ProjectileParticle in game_124920.c), here following a cutscene track. */
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

/* A particle spawned by func_1513D2F0 (a DefParticleHead, as BoneBeamParticle in game_12FDD0.c), here following a cutscene track. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ SeqTrackState *track;
} TrackParticle;

/* game_49D30.c */
void func_1501C880(s32 unused, s32 swapBuffers);
void func_1501CC3C(void);
void func_1501CDC0(s32 scene);
void func_1501CE54(s32 scene);
s32 func_1501CFF8(s32 scene);
void func_1501D044(s32 scene);
struct SeqFileEntry *func_1501D1D4(s32 map, s32 cutscene, s32 scene);
void func_1501D258(s32 map, s32 cutscene);
s32 func_1501D2C4(s32 map, s32 cutscene);
void func_1501D348(s32 map, s32 cutscene, s32 scene, s32 callback, s32 callbackArg);
void func_1501DAAC(s32 scene);
void func_1501DE18(s32 scene);
void func_1501DF04(s32 scene);
s32 func_1501E05C(s32 scene);
void func_1501E1B4(s32 scene);
void func_1501E2F8(s32 scene);
void func_1501E400(s32 scene);
void func_1501E540(s32 scene);
void func_1501E73C(s32 scene);
void func_1501E81C(s32 skipped, s32 scene);
void func_1501EA18(s32 scene);
void func_1501EC38(s32 scene);
s32 func_1501F72C(s32 track, f32 dist, f32 *out, s32 scene, s32 *segmentOut);
void func_1501FC8C(s32 track, f32 percent, f32 *x, f32 *y, f32 *z, s32 scene);
void func_1501FE68(s32 track, f32 percent, f32 *heading, s32 scene);
void func_1501FFE8(f32 *out, SeqPathKey *keys, s32 key, s32 numKeys);
void func_15020388(s32 track, s32 scene);
void func_15020878(s32 track, s32 segment, s32 scene);
void func_15020EC4(s32 scene);
void func_1502178C(Actor *target, s32 scene, s32 onlyTrack);
s32 func_15021DB8(f32 *outX, f32 *outY, f32 *outZ, f32 *target, s32 objIndex, s32 scene);
s32 func_15022024(f32 *outA, f32 *outB, s32 objIndex, s32 scene);
void func_15022190(s16 x, s16 y, s16 z, f32 value);
void func_150221E8(s16 x, s16 y, s16 z, f32 value);
void func_15022234(s32 scene);
void func_15022248(s32 scene);
void func_1502225C(s32 id, s32 scene);
void func_150222E0(s32 track, s32 scene);
void func_15022398(s32 track, s32 scene);
void func_15022528(s32 scene);
void func_15022640(s32 id, s32 scene);
void func_150226BC(s32 track, s32 scene);
void func_15022754(s32 scene);
void func_150227BC(s32 scene);
void func_15022848(s32 scene);
void func_150228E4(s32 scene);
void func_15022998(Actor *obj);
s32 func_150229E4(Actor *obj);
s32 func_15022B08(s32 actorIndex, s32 scene);
void func_15022BA4(s32 scene);
s32 func_15023264(s32 scene, s32 time);
void func_150233BC(void);
void func_150233E4(void);
void func_15023440(SeqSubtitle *sub, s32 force);
SeqSubtitle *func_150234A4(s32 track, s32 replaceOpen);
s32 func_150235DC(s32 scene);
void func_1502378C(void);

/* game_50D20.c */
void func_15023870(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_50D80.c */
void func_150238D0(void);
s32 func_150238D8(SeqVoice *voice, s32 arg1, u32 *volume, s32 arg3, s32 arg4, s32 arg5, s16 *arg6);
s32 func_15023BB0(SeqEvent *after, s32 type, s32 track, SeqEvent **found, s32 matchValue, s32 value,
                  s32 matchUnk4, s32 unk4, s32 matchUnk5, s32 unk5, s32 scene);
void func_15023DE0(SeqEvent **prev, SeqEvent **next, s32 *prevTime, s32 *nextTime, s32 track, s32 type,
                   s32 matchValue, s32 value, s32 matchUnk4, s32 unk4, s32 time, s32 scene);
void func_15024130(s32 count, s32 scene);
void func_150241B4(SeqQueueEntry *queue, s32 *count, SeqEvent *event, s32 track, s32 index, s32 time, s32 scene);
void func_150242F8(s32 mode, s32 scene);
s32 func_1502460C(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag);
s32 func_150265CC(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag);
s32 func_15029BB8(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene, Actor *obj, WorldObject *actor, f32 *pos, s32 sceneTag, s32 skipping);
void func_1502A8A0(SeqEvent *event, s32 track, s32 index, s32 eventTime, s32 scene);
extern Color24 D_80084110[2]; /* screen fade colours for screen_startViewFade, picked by sequence events */

/* game_1CBE20.c */
EvTimer *func_1519E970(s16 duration, EvTimerOwner *owner, u8 keepOwnerLink, f32 *arg3, f32 *arg4, u8 arg5,
                       s32 arg6);
void func_1519EA04(EvTimer *timer);
void func_1519EA78(Vec3f *pos, u16 id, f32 scale, u8 arg3, s32 arg4);
void func_1519EB8C(SeqTrackState *track, u16 id, s16 duration, u8 arg3, s32 arg4);
s32 func_1519ED24(TrackPhysicsParticle *obj);
void func_1519ED84(SeqTrackState *track, s32 texture, s16 lifetime, u8 arg3, s32 arg4);
s32 func_1519EF04(TrackParticle *particle);

#endif
