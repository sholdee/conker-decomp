#ifndef CONKER_FLAMETHROWER_H
#define CONKER_FLAMETHROWER_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "flamethrower": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_1C2C60.c
 */

/* Types that game_1C2C60.c defines and only it uses in full. */
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
    /* 0x34 */ s8 initFn;      /* D_8008F8A4 index */
    /* 0x35 */ s8 unk35;       /* D_8008F8A8 index */
    /* 0x36 */ s8 destroyFn;   /* D_8008F8B4 index */
    /* 0x37 */ s8 soundFn;     /* D_8008F8B8 index */
    /* 0x38 */ s8 soundUpdateFn; /* D_8008F8BC index */
    /* 0x39 */ s8 soundStopFn; /* D_8008F8C0 index */
    /* 0x3A */ u8 alpha;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ f32 fadeStart; /* fraction of the length after which points fade out */
    /* 0x40 */ f32 texSpeed;
    /* 0x44 */ f32 texScale;
    /* 0x48 */ s8 updateFn; /* D_8008F8C4 index */
    /* 0x49 */ u8 pad49[3];
} FlameSeed;

/* Parameters of the line particles (func_1513D2F0) a flame sheds near its far end; random values are min + range. */
typedef struct FlameLineParams {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ s16 lifeMin;
    /* 0x0A */ s16 lifeRange;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 speedMin;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 gravityMin;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ f32 dragA;
    /* 0x30 */ f32 dragB;
    /* 0x34 */ s16 fadeStart;
    /* 0x36 */ s16 alphaPerLife;
    /* 0x38 */ f32 position; /* fraction of the length where they spawn */
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 pad3E[2];
    /* 0x40 */ f32 shrinkRate;
} FlameLineParams;

/* Parameters of the particles (func_15147DA0) a flame scatters towards `target`; random values are min + range. */
typedef struct FlameScatterParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 speedMin;
    /* 0x2C */ f32 speedRange;
    /* 0x30 */ f32 spread;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42;
} FlameScatterParams;

/* Parameters of the particles (func_15130280) a flame emits towards `target`; random values are min + range. */
typedef struct FlameEmitParams {
    /* 0x00 */ Vec3f target; /* bone-relative */
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 speedMin;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ f32 spread;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
} FlameEmitParams;

/* Spawn parameters for func_151994B8. */
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

/* game_1C2C60.c */
void *func_151957B0(s32 size, ListNode **head, ListNode **tail);
void trailing_151957B0(void);
void func_1519582C(void);
s32 func_15195868(Gfx *dl, s32 texture, s32 occurrence, s32 *levels);
TexScroll *func_15195984(s32 id, s32 size, s32 pos, s32 doubled);
s16 func_15195A84(s16 cmdIndex, s32 arg1, s32 arg2, s32 arg3);
TexScroll *func_15195AA8(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 which, s32 speedS,
                         s32 speedT);
void func_15195D00(TexScroll *scroll, Gfx *dl, s32 cmdIndex, s32 count);
TexScroll *func_15195DD4(Gfx *dl, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS, s32 speedT);
TexScroll *func_15195FB0(WorldObject *obj, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                         s32 speedT);
void func_15195FF0(Gfx *dl0, Gfx *dl1);
void func_15196318(TexScroll *scroll, s32 speedS, s32 speedT);
void func_15196330(CharacterFlamethrowerActor *actor);
void func_151963B4(CharacterFlamethrowerActor *actor);
CharacterFlamethrowerActor *func_15196438(Actor *owner, s32 preset, u8 arg2, s32 arg3);
s32 func_15196748(CharacterFlamethrowerActor *actor);
s32 func_15196B4C(CharacterFlamethrowerActor *actor, s32 arg1);
Gfx *func_15197148(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view);
f32 func_151979F8(CharacterFlamethrowerActor *actor);
f32 func_15197A0C(CharacterFlamethrowerActor *actor);
f32 func_15197A68(CharacterFlamethrowerActor *actor);
void func_15197A7C(Actor *owner);
s32 func_15197AB4(SceneParticle *p);
s32 func_15197BBC(SceneParticle *p);
void *func_15197C10(SceneParticle *beam, s16 view);
void func_15198054(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind);
void func_15198110(CharacterFlamethrowerActor *actor, s32 index);
s32 func_15198570(CharacterFlamethrowerActor *actor);
s32 func_151987CC(CharacterFlamethrowerActor *actor);
s32 func_1519897C(CharacterFlamethrowerActor *actor);
void func_15198C60(void);
void func_15198C90(CharacterFlamethrowerActor *actor);
void func_15198D40(CharacterFlamethrowerActor *actor);
void func_15198D7C(CharacterFlamethrowerActor *actor);
void func_15198D88(CharacterFlamethrowerActor *actor, Vec3f *end);
void func_151990AC(CharacterFlamethrowerActor *actor, Vec3f *end);
void func_151993B4(CharacterFlamethrowerActor *actor);
void func_151993E4(CharacterFlamethrowerActor *actor);
void func_1519944C(CharacterFlamethrowerActor *actor);
CharacterFlamethrowerActor *func_151994B8(s32 extraSize, CharacterFlamethrowerSetup *setup, u8 arg2, s32 arg3);
void func_15199834(Actor *owner);
void func_1519986C(CharacterFlamethrowerActor *actor, ActorMsg *msg, u8 kind);
void func_15199928(CharacterFlamethrowerActor *actor);
void func_15199954(CharacterFlamethrowerActor *actor);
void func_15199980(CharacterFlamethrowerActor *actor);
s32 func_15199A10(CharacterFlamethrowerActor *actor);
s32 func_15199C34(CharacterFlamethrowerActor *actor);
s32 func_1519A9A4(CharacterFlamethrowerActor *actor);
Gfx *func_1519B4B8(CharacterFlamethrowerActor *actor, Gfx *gfx, s16 view);
void func_1519BE1C(Vec3f *pos, Vec3f *vel, f32 gravity, f32 dt);
void func_1519BEB8(CharacterFlamethrowerActor *actor);
void func_1519BF20(CharacterFlamethrowerActor *actor);
void func_1519BF8C(void);
void func_1519BFBC(CharacterFlamethrowerActor *actor);
void func_1519C06C(CharacterFlamethrowerActor *actor);
LightEffect *func_1519C09C(CharacterFlamethrowerActor *flame, Vec3f *pos, u8 red, u8 green, u8 blue, u8 alpha, u8 arg6,
                           s32 arg7);
void func_1519C200(LightEffect *light);
void func_1519C22C(LightEffect *light);
void func_1519C258(LightEffect *light);
s32 func_1519C26C(SceneParticle *beam);
s32 func_1519C4E4(SceneParticle *beam);
void *func_1519C56C(SceneParticle *beam, s16 view);
s32 func_1519C910(CharacterFlamethrowerActor *actor);
s32 func_1519C948(CharacterFlamethrowerActor *actor);
s32 func_1519C970(CharacterFlamethrowerActor *actor);
s32 func_1519C998(CharacterFlamethrowerActor *actor);
void func_1519C9C4(FlamethrowerController *ctrl);
void func_1519CD64(FlamethrowerController *ctrl);
void func_1519CDB0(CharacterFlamethrowerActor *actor, f32 fraction, s32 kind);
extern f32 (*D_8008F870[])(void *, s32);
extern void (*D_8008F87C[])(void *);
extern s32 (*D_8008F880[])(void *);
extern void (*D_8008F88C[])(void *);
extern s16 (*D_8008F890[])(void);
extern void (*D_8008F894[])(void *);
extern void (*D_8008F898[])(void *);
extern void (*D_8008F89C[])(void *);
extern void (*D_8008F8A4[])(void *);
extern void (*D_8008F8A8[])(void *);
extern void (*D_8008F8B4[])(void *);
extern s16 (*D_8008F8B8[])(void);
extern void (*D_8008F8BC[])(void *);
extern void (*D_8008F8C0[])(void *);
extern s32 (*D_8008F8C4[])(void *);
extern const JetChildPreset D_800A8710[];
extern const ScatterPreset D_800A8728[];
extern const JetSpritePreset D_800A8770[];
extern const CharacterFlamethrowerJetTemplate D_800A87A0[];
extern const u8 D_800A8A9C[];

#endif
