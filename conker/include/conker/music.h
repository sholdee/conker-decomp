#ifndef CONKER_MUSIC_H
#define CONKER_MUSIC_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "music": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Audio/Tune/tune.c
 *   game/Audio/LevelMusic/levelmusic.c
 */

/* A playing music sequence (defined in init/Audio/Tune/tune.c). */
typedef struct MusicTrack MusicTrack;

/* init/Audio/Tune/tune.c */
MusicTrack *music_findTune(s32 id);
MusicTrack *music_findTuneOrQueued(s32 id);
void music_clearSyncRefs(MusicTrack *track);
MusicTrack *music_allocTune(s32 id);
void music_queueTune(MusicTrack *track, MusicTrack *current);
s32 music_getPlayingIds(s32 *ids);
s32 music_updateSeq1(s32 state, u8 channel, s32 arg2, s32 arg3);
s32 music_updateSeq16(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 music_updateSeq19(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq77(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq147(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq20(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 music_updateSeq25(s32 state, u8 channel, s32 arg2, s32 arg3);
s32 music_updateSeq33(s32 state, u8 channel, f32 x, f32 y, f32 z);
u32 music_updateSeq34(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq35(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq36(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq62(s32 state, s32 channel, f32 x, f32 y, f32 z);
s32 music_updateSeq76(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 music_updateSeq84(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 music_updateSeq88(s32 state, u8 channel, f32 x, f32 y, f32 z);
void func_1000CBA8(s32 volume);
void music_setDuck(s32 target, s32 rate, s32 channelMask);
void music_updateChannelVolume(s32 channel);
s32 music_stepVolume(s32 current, s32 target, s32 rate);
s32 music_isSyncReached(u8 waitMask, MusicTrack *syncTo);
void music_readMarkers(s32 channel);
void music_updateChannel(s32 index, f32 x, f32 y, f32 z);
void music_update(f32 x, f32 y, f32 z);
void music_playTune(s32 newId, s32 oldId, s32 transition);
void music_stopTune(s32 id, s32 transition);
void music_freeStoppedTunes(void);
void music_setFade(s32 id, s32 volume, s32 frames);
void music_pauseTune(s32 id, s32 pause);
s32 func_1000E0F8(s32 id);
s32 func_1000E134(s32 id);
void func_1000E17C(void);
void music_pauseAll(s32 pause);
void music_setVolume(s32 id, s32 volume);
s32 music_fadeChannelVolume(s32 id, s32 percent, s32 channelMask, s32 frames);
s32 music_setChannelVolume(s32 id, s32 percent, s32 channelMask);
s32 music_jumpToMarker(s32 id, s32 marker, s32 waitMask, s32 syncId);
s32 func_1000E704(s32 id, s32 arg1, s32 arg2);
void func_1000E75C(s32 arg0);
/* (s32 *mode, s32 *value); declared without a prototype: game/Effects/Light/light.c passes 4 arguments */
s32 music_getEventState();
void music_postEvent(u32 event, s32 value);
void music_clearEvent(s32 event);
u8 music_getMarkers(s32 id);
void music_reset(void);
u16 func_1000EA94(s32 arg0);
extern s32 gMusicEventLevels[3][16];  /* per-channel music event level */
extern u8 gLastMarkerChannel;          /* channel of the last marker event */
extern OSMesgQueue gMarkerQueues[3];
extern OSMesg gMarkerQueueBufs[3][8]; /* message buffers of the gMarkerQueues queues */
extern u8 gMusicPaused;
extern s32 gMusicEventFlags;
extern s32 D_80041F08;
extern s32 D_80041F0C;

/* game/Audio/LevelMusic/levelmusic.c */
void music_init(void);
void func_15000090(void);
void music_startLevelMusic(void);
/* (void); declared without a prototype: callers pass the level */
void func_150006E0();

#endif
