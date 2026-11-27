#ifndef CONKER_FLAMETHROWER_H
#define CONKER_FLAMETHROWER_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "flamethrower": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Effects/CharacterFlameThrower/characterflamethrower.c
 */

/* Types that game/Effects/CharacterFlameThrower/characterflamethrower.c defines and only it uses in full. */
typedef struct ListNode ListNode;
typedef struct TexScroll TexScroll;
typedef struct CharacterFlamethrowerActor CharacterFlamethrowerActor;
typedef struct SceneParticle SceneParticle;
typedef struct FlamethrowerController FlamethrowerController;
typedef struct CharacterFlamethrowerSetup CharacterFlamethrowerSetup;
typedef struct JetChildPreset JetChildPreset;
typedef struct JetSpritePreset JetSpritePreset;
typedef struct CharacterFlamethrowerJetTemplate CharacterFlamethrowerJetTemplate;

/* First 0x4C bytes of a flame's state, also given in its CharacterFlamethrowerSetup. */
typedef struct FlameSeed {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 flags;
    /* 0x07 */ u8 pad07;
    /* 0x08 */ Vec3f startOffset;
    /* 0x14 */ Vec3f endOffset;
    /* 0x20 */ f32 gravity;
    /* 0x24 */ f32 emitRate;
    /* 0x28 */ f32 maxLength;
    /* 0x2C */ f32 size;
    /* 0x30 */ f32 sizeRate;
    /* 0x34 */ s8 initFn;      /* gFlameInitProcs index */
    /* 0x35 */ s8 attackFn;    /* gFlameAttackProcs index */
    /* 0x36 */ s8 destroyFn;   /* gFlameDestroyProcs index */
    /* 0x37 */ s8 soundFn;     /* gFlameSoundStartProcs index */
    /* 0x38 */ s8 soundUpdateFn; /* gFlameSoundUpdateProcs index */
    /* 0x39 */ s8 soundStopFn; /* gFlameSoundStopProcs index */
    /* 0x3A */ u8 alpha;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ f32 fadeStart; /* fraction of the length after which points fade out */
    /* 0x40 */ f32 texSpeed;
    /* 0x44 */ f32 texScale;
    /* 0x48 */ s8 updateFn; /* gFlameUpdateProcs index */
    /* 0x49 */ u8 pad49[3];
} FlameSeed;

/* Parameters of the line particles (particle_newQuadParticle) a flame sheds near its far end; random values are min + range. */
typedef struct FlameLineParams {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ s16 lifeMin;
    /* 0x0A */ s16 lifeRange;
    /* 0x0C */ f32 widthMin;
    /* 0x10 */ f32 heightMin;
    /* 0x14 */ f32 widthRange;
    /* 0x18 */ f32 heightRange;
    /* 0x1C */ f32 speedMin;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 gravityMin;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ f32 dragA;
    /* 0x30 */ f32 dragB;
    /* 0x34 */ s16 fadeStart;
    /* 0x36 */ s16 alphaPerLife;
    /* 0x38 */ f32 position; /* fraction of the length where they spawn */
    /* 0x3C */ u8 alphaMin;
    /* 0x3D */ u8 alphaRange;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ f32 shrinkRate;
} FlameLineParams; // size 0x44

/* Parameters of the particles (trail_createMotionTrail) a flame scatters towards `target`; random values are min + range. */
typedef struct FlameScatterParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s32 itemCountMin;
    /* 0x18 */ s32 itemCountRange;
    /* 0x1C */ s16 lifeMin;
    /* 0x1E */ s16 lifeRange;
    /* 0x20 */ f32 halfWidthMin;
    /* 0x24 */ f32 halfWidthRange;
    /* 0x28 */ f32 speedMin;
    /* 0x2C */ f32 speedRange;
    /* 0x30 */ f32 spread;
    /* 0x34 */ f32 gravityMin;
    /* 0x38 */ f32 gravityRange;
    /* 0x3C */ s16 alphaMin;
    /* 0x3E */ s16 alphaRange;
    /* 0x40 */ s16 fadeStart;
    /* 0x42 */ s16 fadeRate;
} FlameScatterParams; // size 0x44

/* Parameters of the particles (particle_newSpriteParticleWithFloor) a flame emits towards `target`; random values are min + range. */
typedef struct FlameEmitParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s16 lifeMin;
    /* 0x16 */ s16 lifeRange;
    /* 0x18 */ s16 alphaMin;
    /* 0x1A */ s16 alphaRange;
    /* 0x1C */ f32 sizeMin;
    /* 0x20 */ f32 sizeRange;
    /* 0x24 */ f32 speedMin;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ f32 spread;
    /* 0x30 */ f32 gravityMin;
    /* 0x34 */ f32 gravityRange;
    /* 0x38 */ s16 fadeStart;
    /* 0x3A */ s16 fadeRate;
} FlameEmitParams; // size 0x3C

/* Spawn parameters for flamethrower_spawnFlame. */
struct CharacterFlamethrowerSetup {
    /* 0x00 */ s32 kind;
    /* 0x04 */ FlameSeed seed;
    /* 0x50 */ u8 red;
    /* 0x51 */ u8 green;
    /* 0x52 */ u8 blue;
    /* 0x53 */ u8 alpha;
    /* 0x54 */ u8 flags;
    /* 0x55 */ u8 pad55[3];
    /* 0x58 */ FlameLineParams line;
    /* 0x9C */ FlameScatterParams scatter;
    /* 0xE0 */ FlameEmitParams emit;
};

/* game/Effects/CharacterFlameThrower/characterflamethrower.c */
void *flamethrower_appendListNode(s32 size, ListNode **head, ListNode **tail);
void trailing_151957B0(void);
void flamethrower_clearTexScrolls(void);
s32 flamethrower_findTileSizeCmd(Gfx *dl, s32 texture, s32 occurrence, s32 *levels);
TexScroll *flamethrower_newTexScroll(s32 id, s32 size, s32 pos, s32 doubled);
s16 flamethrower_returnCmdIndex(s16 cmdIndex, TexScroll *scroll, s32 size, s32 doubled);
TexScroll *flamethrower_addTexScroll(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 which, s32 speedS,
                         s32 speedT);
void flamethrower_addTexScrollRef(TexScroll *scroll, Gfx *dl, s32 cmdIndex, s32 count);
TexScroll *flamethrower_addTexScrollInDl(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS, s32 speedT);
TexScroll *flamethrower_addObjectTexScroll(WorldObject *obj, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                         s32 speedT);
void flamethrower_updateTexScrolls(Gfx *dl0, Gfx *dl1);
void flamethrower_setTexScrollSpeed(TexScroll *scroll, s32 speedS, s32 speedT);
void flamethrower_destroyJet(CharacterFlamethrowerActor *actor);
void flamethrower_freeJet(CharacterFlamethrowerActor *actor);
CharacterFlamethrowerActor *flamethrower_spawnJet(Actor *owner, s32 preset, u8 fxOwner, s32 fxList);
s32 flamethrower_updateJet(CharacterFlamethrowerActor *actor);
s32 flamethrower_emitJet(CharacterFlamethrowerActor *actor, s32 dead);
Gfx *flamethrower_drawJet(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view);
f32 flamethrower_getSlowJetSpeed(CharacterFlamethrowerActor *actor);
f32 flamethrower_getPowerTunable(CharacterFlamethrowerActor *actor);
f32 flamethrower_getFastJetSpeed(CharacterFlamethrowerActor *actor);
void flamethrower_stopJets(Actor *owner);
s32 flamethrower_moveJetLine(SceneParticle *p);
s32 flamethrower_fadeJetLine(SceneParticle *p);
void *flamethrower_buildJetLineQuad(SceneParticle *beam, s16 view);
void flamethrower_onJetMessage(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind);
void flamethrower_shedJetLines(CharacterFlamethrowerActor *actor, s32 index);
s32 flamethrower_hitPlayer(CharacterFlamethrowerActor *actor);
s32 flamethrower_hitObjects(CharacterFlamethrowerActor *actor);
s32 flamethrower_burnSwarm(CharacterFlamethrowerActor *actor);
void flamethrower_startJetSound(void);
void flamethrower_moveJetSound(CharacterFlamethrowerActor *actor);
void flamethrower_stopJetSound(CharacterFlamethrowerActor *actor);
void flamethrower_emptyJetProc(CharacterFlamethrowerActor *actor);
void flamethrower_scatterParticles(CharacterFlamethrowerActor *actor, Vec3f *end);
void flamethrower_emitSprites(CharacterFlamethrowerActor *actor, Vec3f *end);
void flamethrower_stopJet(CharacterFlamethrowerActor *actor);
void flamethrower_activateJetZone(CharacterFlamethrowerActor *actor);
void flamethrower_deactivateJetZone(CharacterFlamethrowerActor *actor);
CharacterFlamethrowerActor *flamethrower_spawnFlame(s32 extraSize, CharacterFlamethrowerSetup *setup, u8 fxOwner, s32 fxList);
void flamethrower_stopFlames(Actor *owner);
void flamethrower_onFlameMessage(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind);
void flamethrower_destroyFlame(CharacterFlamethrowerActor *actor);
void flamethrower_freeFlame(CharacterFlamethrowerActor *actor);
void flamethrower_releaseFlame(CharacterFlamethrowerActor *actor);
s32 flamethrower_updateFlame(CharacterFlamethrowerActor *actor);
s32 flamethrower_emitFlame(CharacterFlamethrowerActor *actor);
s32 flamethrower_shapeFlame(CharacterFlamethrowerActor *actor);
Gfx *flamethrower_drawFlame(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view);
void flamethrower_integratePoint(Vec3f *pos, Vec3f *vel, f32 gravity, f32 dt);
void flamethrower_activateFlameZone(CharacterFlamethrowerActor *actor);
void flamethrower_deactivateFlameZone(CharacterFlamethrowerActor *actor);
void flamethrower_startFlameSound(void);
void flamethrower_moveFlameSound(CharacterFlamethrowerActor *actor);
void flamethrower_stopFlameSound(CharacterFlamethrowerActor *actor);
LightEffect *flamethrower_createFlameLight(CharacterFlamethrowerActor *flame, Vec3f *pos, u8 red, u8 green, u8 blue, u8 alpha, u8 fxOwner,
                           s32 fxList);
void flamethrower_destroyFlameLight(LightEffect *light);
void flamethrower_freeFlameLight(LightEffect *light);
void flamethrower_detachFlameLight(LightEffect *light);
s32 flamethrower_moveFlameLine(SceneParticle *beam);
s32 flamethrower_fadeFlameLine(SceneParticle *beam);
void *flamethrower_buildFlameLineQuad(SceneParticle *beam, s16 view);
s32 flamethrower_checkOwnerAlive(CharacterFlamethrowerActor *actor);
s32 flamethrower_attackProc0(CharacterFlamethrowerActor *actor);
s32 flamethrower_attackProc1(CharacterFlamethrowerActor *actor);
s32 flamethrower_attackProc2(CharacterFlamethrowerActor *actor);
void flamethrower_spawnControllerFlame(FlamethrowerController *ctrl);
void flamethrower_updateController(FlamethrowerController *ctrl);
void flamethrower_attackAlongFlame(CharacterFlamethrowerActor *actor, f32 fraction, s32 kind);
extern f32 (*gJetSpeedProcs[])(void *, s32);
extern void (*gJetInitProcs[])(void *);
extern s32 (*gJetUpdateProcs[])(void *);
extern void (*gJetDestroyProcs[])(void *);
extern s16 (*gJetSoundStartProcs[])(void);
extern void (*gJetSoundUpdateProcs[])(void *);
extern void (*gJetSoundStopProcs[])(void *);
extern void (*gFlamethrowerJetProcs[])(void *);
extern void (*gFlameInitProcs[])(void *);
extern void (*gFlameAttackProcs[])(void *);
extern void (*gFlameDestroyProcs[])(void *);
extern s16 (*gFlameSoundStartProcs[])(void);
extern void (*gFlameSoundUpdateProcs[])(void *);
extern void (*gFlameSoundStopProcs[])(void *);
extern s32 (*gFlameUpdateProcs[])(void *);
extern const JetChildPreset gJetChildPresets[];
extern const ScatterPreset gJetScatterPresets[];
extern const JetSpritePreset gJetSpritePresets[];
extern const CharacterFlamethrowerJetTemplate gJetTemplates[];
extern const u8 gFlamethrowerCharacterIds[];
extern TriggerZone *gFlamethrowerZones[6]; /* trigger zones of the flamethrower characters, by slot */

#endif
