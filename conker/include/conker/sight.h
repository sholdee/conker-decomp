#ifndef CONKER_SIGHT_H
#define CONKER_SIGHT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "sight": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_1F4650.c
 *   game_1F7050.c
 */

typedef struct {
    u8 pad0[0x6F];
    u8 unk6F;
} SightStatusBlock;

typedef struct {
    EffectHeader header;
    u8 pad10[0x48];
    s32 flags;
    u8 pad5C[0x12C];
    SightStatusBlock *status;
} SightStatusObject;

/* Message 0x37 (see func_151CB918). */
typedef struct {
    u8 side;
    Camera *player;
} SightPanelMsg;

/* Effect spawned by func_151CCE94; +0x20 holds the owning player. */
typedef struct {
    EffectHeader header;
    u8 pad10[0x10];
    Camera *player;
} SightOwnedEffect;

/* State of the lens effect (func_151CB110), stored at +0x18 of its object. */
typedef struct {
    Camera *player;
    RenderView *view;
    u16 *pixels;
    s32 left;
    u32 topOffset;
} SightLens;

typedef struct {
    EffectHeader header;
    u8 pad10[0x8];
    SightLens lens;
    u8 pad2C[4];
    u16 pixels[24 * 24]; /* 24x24 copy of the framebuffer under the lens */
} SightLensObject;

/* game_1F4650.c */
s32 func_151C71A0(SightBeamParams *beam);
SightBeamObject *func_151C756C(Vec3f *start, CollisionTri *startTri, Vec3f *end, CollisionTri *endTri,
                               SightBeamParams *beam, SightBeamParams *source, u8 red, u8 green, u8 blue,
                               u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks,
                               u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle,
                               f32 pulseSpeed, u8 keepEnabled, u8 arg16, s32 arg17);
void *func_151C7E98(SightBeamObject *obj, s16 view);
s32 func_151C82D0(SightBeamObject *obj);
void func_151C8674(Actor *actor, TriggerZone *source);
void func_151C86AC(SightBeamObject *obj, void *msg, u8 msgId);
s32 func_151C87AC(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume);
s32 func_151C87E0(Actor *actor, SightBeamObject *obj);
void func_151C899C(Actor *actor, SightBeamObject *obj);
SightExtension *func_151C8FCC(SightBeamObject *owner, Vec3f *start, Vec3f *end, u8 red, u8 green, u8 blue,
                              SightZone *zone, u8 zoneMasked, u8 arg8, s32 arg9);
void func_151C9198(SightEmitter **emitters, Vec3f *pos, CollisionTri *triangle, Vec3f *dir, SightZone *zone,
                   u8 altColors, u8 zoneMasked, u8 arg7, s32 arg8);
void func_151C94D4(SightBeamObject *obj);
void func_151C95D8(SightBeamObject *obj);
s32 func_151C96DC(SightExtension *effect, s32 arg1);
void func_151C970C(s32 command, TriggerZone *source);
void func_151C9740(Actor *actor, u8 arg1, s32 arg2);
void func_151C9AC0(Actor *actor, u8 arg1, s32 arg2);
s32 func_151C9B30(LightEffect *obj);
s32 func_151C9B64(SightStatusObject *obj, u8 *visible);
extern const s32 D_800AAC30[];

/* game_1F7050.c */
void func_151C9BA0(Camera *player, s32 state);
void func_151C9DE8(Camera *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4);
void func_151C9ED4(Camera *player);
void func_151C9F38(Camera *player, u8 type, u8 arg2, s8 arg3, u8 arg4);
void func_151CA6A0(Camera *player, u8 big);
void func_151CAACC(SightSprite *sprite, void **msg, u8 msgId);
void func_151CAB78(Camera *player, u8 big);
void func_151CAD28(Camera *player, u8 big);
void func_151CB110(SightLensObject *obj, u16 *framebuffer);
void func_151CB49C(SightLensObject *obj, void **msg, u8 msgId);
void func_151CB510(SightSprite *sprite);
void func_151CB5FC(Camera *player, u8 kind, u8 arg2, s8 arg3, u8 arg4);
void func_151CB918(SightSprite *sprite, SightPanelMsg *msg, u8 msgId);
s32 func_151CB970(SightSprite *sprite);
void func_151CBB6C(SightOwnedEffect *effect, void **msg, u8 msgId);
s32 func_151CBBE0(SightSprite *sprite);
s32 func_151CBC60(SightSprite *sprite);
f32 func_151CC1D4(SightSprite *sprite);
void func_151CC290(Camera *player);
void func_151CC2BC(SightSprite *sprite, s32 *msg, u8 msgId);
void func_151CC524(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5);
s32 func_151CC77C(SightSprite *sprite);
void func_151CC840(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5);
void func_151CCD1C(Camera *player, u8 red, u8 green, u8 blue);
void func_151CCE94(Camera *player);
void func_151CCF08(Camera *player, u8 arg1, s8 arg2, u8 arg3);
void func_151CD224(SightSprite *sprite);
extern const SightColor D_800AAC80;
extern const SightColor D_800AAC90[];
extern const SightColor D_800AACE0[];
extern const f32 D_800AAD30[];
extern const f32 D_800AAD44[];
extern const f32 D_800AAD58[];
extern const f32 D_800AAD6C[][2];
extern const f32 D_800AAD94[][2];

#endif
