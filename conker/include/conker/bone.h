#ifndef CONKER_BONE_H
#define CONKER_BONE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "bone": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Bone/BoneRemap/boneremap.c (data only; linked between game_5D2C0 and game/Bone/BoneCommands/bonecommands)
 *   game/Bone/BoneCommands/bonecommands.c
 *   game/Bone/BoneChannels/bonechannels.c
 *   game/Bone/BoneLookAt/bonelookat.c
 *   game/Bone/BoneChain/bonechain.c
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

/* game/Bone/BoneRemap/boneremap.c */
extern const u8 *gBoneRemapLists[]; // per remap set (1-based): (bone, value) pairs terminated by 0xFF

/* game/Bone/BoneCommands/bonecommands.c */
void bone_buildCommands(GameCommand *commands, s32 index);
GameCommand *func_150341BC(GameCommand *cmd, s32 index);
GameCommand *func_15034340(GameCommand *cmd, s32 index);
GameCommand *func_150343B0(GameCommand *cmd, s32 index);
GameCommand *func_15034420(GameCommand *cmd, s32 index);
GameCommand *func_150344A0(GameCommand *cmd, s32 index);
void bone_updateChainByType(s32 index);
void bone_updateObject(s32 index);
void bone_collapseMaskedBones(Actor *obj);
void bone_collapseAllMaskedBones(void);

/* game/Bone/BoneChannels/bonechannels.c */
GameCommand *func_15034860(GameCommand *cmd, Actor *actor, s32 channel, s32 channel2);
void bone_lowerBones(Actor *actor, s32 first, s32 second);

/* game/Bone/BoneLookAt/bonelookat.c */
void bone_lookAtCamera(s32 unused, s32 actorIndex, s32 bone, s32 quarterTurn);

/* game/Bone/BoneChain/bonechain.c */
extern u8 D_800C4000; // one bit per object: reset its bone chain once it has bones again
void bone_initChain(Actor *obj);
void bone_updateChainPoseFlags(Actor *obj);
s32 bone_isChainSimulated(s32 objIndex, BoneChain *chain);
void bone_updateChain(s32 objIndex);
void bone_rebuildChainMatrices(s32 objIndex, s32 mode, f32 blend, BoneChain *chain);

#endif
