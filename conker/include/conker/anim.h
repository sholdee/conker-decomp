#ifndef CONKER_ANIM_H
#define CONKER_ANIM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "anim": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Anim/MorphReset/morphreset.c
 *   game/Anim/AnimInstance/animinstance.c
 *   game/Anim/AnimPlayback/animplayback.c
 *   game/Anim/IdleAnim/idleanim.c
 *   game/Anim/VertexMorph/vertexmorph.c
 */

/* game/Anim/MorphReset/morphreset.c */
void anim_clearMorphCache(void);

/* game/Anim/AnimInstance/animinstance.c */
void anim_updateInstance(AnimInstance *anim);
void anim_play(AnimInstance *model, s32 restart, s32 anim, f32 speed, f32 blendFrames, s32 flag8000);
s32 anim_createInstance(s32 model, s32 bank, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg5, AnimInstance **out);
void anim_freeInstance(AnimInstance *model);

/* game/Anim/AnimPlayback/animplayback.c */
void anim_advance(AnimInstance *anim, f32 rate, Actor *obj, s32 mode);
void anim_advanceObjects(s32 onlyType4);
void anim_copyTime(Actor *dst, Actor *src);
extern const f32 gCosTable[65];
extern const u8 gHitRuleCounts[56];
extern const HitReaction gKnockbackReactions[20];
extern const HitReaction gDefaultHitReactions[68];
extern const HitRule *gHitRoutes[56];

/* game/Anim/IdleAnim/idleanim.c */
void anim_addIdleCandidate(u8 *list, s32 *count, u8 seq);
void anim_addIdleCategory(u8 *list, s32 *count, s32 category);
s32 anim_filterRecentIdles(u8 *candidates, s32 numCandidates, u8 *out, s32 *outLen, u8 *recent);
void anim_recordIdle(s32 seq, u8 *recent); /* K&R definition: seq is promoted */
void anim_pushRecentIdle(u8 seq, u8 *recent);
void anim_advanceIdlePhase(void);
void anim_forgetRecentIdle(s32 seq, u8 *recent);
void anim_pickIdleSequence(void);
s32 anim_stepIdleSequence(void);
s32 anim_randomIdleDelay(s32 phase);
void anim_playIdle(Actor *actor);
void anim_updateIdle(void);
void anim_applyIdleFrameFlags(Actor *actor);
void anim_updateIdleTimer(Actor *actor);
void anim_sendIdleDelayEvent(Actor *actor);
s32 anim_isIdleWatched(Actor *actor);
extern u8 gIdleRequestFlags;

/* game/Anim/VertexMorph/vertexmorph.c */
extern const VtxMorphGroup gVtxMorphGroups[2]; // vertex morph groups
extern VtxMorphFrame **gVtxMorphCache[2]; // cached keyframes: [group][part][frame]
void anim_ageMorphCache(void);
s32 anim_morphVertices(s32 frame, s32 duration, s32 actorIndex);

#endif
