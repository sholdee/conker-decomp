#ifndef CONKER_MUSIC_H
#define CONKER_MUSIC_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "music": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_B1B0.c
 *   game_2D4B0.c
 */

/* A playing music sequence (defined in init_B1B0.c). */
typedef struct MusicTrack MusicTrack;

/* init_B1B0.c */
MusicTrack *func_1000B1B0(s32 id);
MusicTrack *func_1000B1FC(s32 id);
void func_1000B294(MusicTrack *track);
MusicTrack *func_1000B2F4(s32 id);
void func_1000B3D4(MusicTrack *track, MusicTrack *current);
s32 func_1000B548(s32 *ids);
s32 func_1000B638(s32 state, u8 channel, s32 arg2, s32 arg3);
s32 func_1000B830(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 func_1000B8B8(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000BA18(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000BAFC(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000BBE8(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 func_1000BC28(s32 state, u8 channel, s32 arg2, s32 arg3);
s32 func_1000BCBC(s32 state, u8 channel, f32 x, f32 y, f32 z);
u32 func_1000BF60(u32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000C350(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000C530(s32 state, u8 channel, f32 x, f32 y, f32 z);
s32 func_1000C7E8(s32 state, s32 channel, f32 x, f32 y, f32 z);
s32 func_1000C934(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 func_1000CA18(s32 state, s32 channel, s32 arg2, s32 arg3);
s32 func_1000CAE4(s32 state, u8 channel, f32 x, f32 y, f32 z);
void func_1000CBA8(s32 volume);
void func_1000CBF0(s32 target, s32 rate, s32 channelMask);
void func_1000CC54(s32 channel);
s32 func_1000CD40(s32 current, s32 target, s32 rate);
s32 func_1000CDA0(u8 waitMask, MusicTrack *syncTo);
void func_1000CEAC(s32 channel);
void func_1000D2F8(s32 index, f32 x, f32 y, f32 z);
void func_1000D758(f32 x, f32 y, f32 z);
void func_1000D96C(s32 newId, s32 oldId, s32 transition);
void func_1000DE1C(s32 id, s32 transition);
void func_1000DEC4(void);
void func_1000DF68(s32 id, s32 volume, s32 frames);
void func_1000E054(s32 id, s32 pause);
s32 func_1000E0F8(s32 id);
s32 func_1000E134(s32 id);
void func_1000E17C(void);
void func_1000E2F4(s32 pause);
void func_1000E40C(s32 id, s32 volume);
s32 func_1000E46C(s32 id, s32 percent, s32 channelMask, s32 frames);
s32 func_1000E588(s32 id, s32 percent, s32 channelMask);
s32 func_1000E654(s32 id, s32 marker, s32 waitMask, s32 syncId);
s32 func_1000E704(s32 id, s32 arg1, s32 arg2);
void func_1000E75C(s32 arg0);
/* (s32 *mode, s32 *value); declared without a prototype: game/Effects/Light/light.c passes 4 arguments */
s32 func_1000E770();
void func_1000E7A0(u32 event, s32 value);
void func_1000E8C4(s32 event);
u8 func_1000E8F0(s32 id);
void func_1000E934(void);
u16 func_1000EA94(s32 arg0);
extern s32 D_800418B0[3][16];  /* per-channel music event level */
extern u8 D_800419A0;          /* channel of the last marker event */
extern OSMesgQueue D_80041E58[3];
extern OSMesg D_80041EA0[3][8]; /* message buffers of the D_80041E58 queues */
extern u8 D_80041F00;
extern s32 D_80041F04;
extern s32 D_80041F08;
extern s32 D_80041F0C;

/* game_2D4B0.c */
void func_15000000(void);
void func_15000090(void);
void func_150000B0(void);
/* (void); declared without a prototype: callers pass the level */
void func_150006E0();

#endif
