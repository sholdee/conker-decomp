#ifndef CONKER_BONE_H
#define CONKER_BONE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "bone": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_61490.c
 *   game_61950.c
 *   game_61D10.c
 *   game_637C0.c
 *   game_68C70.c
 */

/* Simulation state of the 5-joint bone chain driven from bones 24-28 (0x50 bytes). */
typedef struct BoneChain {
    /* 0x00 */ f32 pos[5][3];   /* joint world positions, root first */
    /* 0x3C */ f32 bobPhase;    /* degrees, advanced by the walking speed */
    /* 0x40 */ f32 swayAngle;   /* degrees */
    /* 0x44 */ f32 swaySpeed;   /* degrees per frame */
    /* 0x48 */ f32 swayAmp;
    /* 0x4C */ s16 swayTimer;   /* frames until the next idle sway */
    /* 0x4E */ u8 blendTimer;   /* frames left blending back from the animated pose */
} BoneChain;

/* game_61490.c */
void func_15033FE0(GameCommand *commands, s32 index);
GameCommand *func_150341BC(GameCommand *cmd, s32 index);
GameCommand *func_15034340(GameCommand *cmd, s32 index);
GameCommand *func_150343B0(GameCommand *cmd, s32 index);
GameCommand *func_15034420(GameCommand *cmd, s32 index);

/* game_61950.c */
GameCommand *func_150344A0(GameCommand *cmd, s32 index);
void func_1503453C(s32 index);
void func_150345E4(s32 index);
void func_15034728(Actor *obj);
void func_150347E8(void);

/* game_61D10.c */
GameCommand *func_15034860(GameCommand *cmd, Actor *actor, s32 channel, s32 channel2);
void func_15034EB4(Actor *actor, s32 first, s32 second);

/* game_637C0.c */
void func_15036310(s32 unused, s32 actorIndex, s32 bone, s32 quarterTurn);

/* game_68C70.c */
void func_1503B7C0(Actor *obj);
void func_1503B840(Actor *obj);
s32 func_1503B95C(s32 objIndex, BoneChain *chain);
void func_1503B9BC(s32 objIndex);
void func_1503CB98(s32 objIndex, s32 mode, f32 blend, BoneChain *chain);

#endif
