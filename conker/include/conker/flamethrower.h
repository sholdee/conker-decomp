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
typedef struct ResourceEntry ResourceEntry;
typedef struct CharacterFlamethrowerActor CharacterFlamethrowerActor;
typedef struct SceneParticle SceneParticle;
typedef struct FlamethrowerController FlamethrowerController;
typedef struct CharacterFlamethrowerSetup CharacterFlamethrowerSetup;
typedef struct JetChildPreset JetChildPreset;
typedef struct JetSpritePreset JetSpritePreset;
typedef struct CharacterFlamethrowerJetTemplate CharacterFlamethrowerJetTemplate;

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
TexScroll *func_15195FB0(ResourceEntry *res, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
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
