#ifndef CONKER_SIGHT_H
#define CONKER_SIGHT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "sight": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Sight/SightBeam/sightbeam.c
 *   game/Sight/SightHud/sighthud.c
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

/* Message 0x37 (see sight_handlePanelMessage). */
typedef struct {
    u8 side;
    Camera *player;
} SightPanelMsg;

/* Effect spawned by func_151CCE94: a screen_spawnTintEffect screen tint with the owning player as its extra data. */
typedef struct {
    /* 0x00 */ struct ScreenTint tint;
    /* 0x20 */ Camera *player;
} SightOwnedEffect;

/* State of the lens effect (sight_drawLens), stored at +0x18 of its object. */
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

/* game/Sight/SightBeam/sightbeam.c */
s32 sight_placeBeam(TriggerZone *beam);
SightBeamObject *sight_spawnBeam(Vec3f *start, CollisionTri *startTri, Vec3f *end, CollisionTri *endTri,
                               TriggerZone *beam, TriggerZone *source, u8 red, u8 green, u8 blue,
                               u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks,
                               u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle,
                               f32 pulseSpeed, u8 keepEnabled, u8 arg16, s32 arg17);
void *sight_buildBeamQuad(SightBeamObject *obj, s16 view);
s32 sight_updateBeam(SightBeamObject *obj);
void sight_notifyActorMoved(Actor *actor, TriggerZone *source);
void sight_handleBeamMessage(SightBeamObject *obj, void *msg, u8 msgId);
s32 func_151C87AC(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume);
s32 sight_doesBeamHitActor(Actor *actor, SightBeamObject *obj);
void sight_breakBeam(Actor *actor, SightBeamObject *obj);
SightExtension *sight_spawnExtension(SightBeamObject *owner, Vec3f *start, Vec3f *end, u8 red, u8 green, u8 blue,
                              ZoneNode *zone, u8 zoneMasked, u8 arg8, s32 arg9);
void sight_spawnEmitter(SightEmitter **emitters, Vec3f *pos, CollisionTri *triangle, Vec3f *dir, ZoneNode *zone,
                   u8 altColors, u8 zoneMasked, u8 arg7, s32 arg8);
void sight_freeBeam(SightBeamObject *obj);
void func_151C95D8(SightBeamObject *obj);
s32 sight_isExtensionOn(SightExtension *effect, s32 arg1);
void sight_sendBeamCommand(s32 command, TriggerZone *source);
void sight_explodeAtActor(Actor *actor, u8 arg1, s32 arg2);
void func_151C9AC0(Actor *actor, u8 arg1, s32 arg2);
s32 func_151C9B30(LightEffect *obj);
s32 func_151C9B64(SightStatusObject *obj, u8 *visible);
extern const s32 D_800AAC30[];

/* game/Sight/SightHud/sighthud.c */
void sight_startPlayerEffects(Camera *player, s32 state);
void func_151C9DE8(Camera *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4);
void sight_resetPlayerEffects(Camera *player);
void sight_spawnRing(Camera *player, u8 type, u8 arg2, s8 arg3, u8 arg4);
void sight_spawnBasicSight(Camera *player, u8 big);
void sight_handleSpriteMessage(SightSprite *sprite, void **msg, u8 msgId);
void sight_spawnDotSight(Camera *player, u8 big);
void sight_spawnZoomSight(Camera *player, u8 big);
void sight_drawLens(SightLensObject *obj, u16 *framebuffer);
void sight_handleLensMessage(SightLensObject *obj, void **msg, u8 msgId);
s32 func_151CB510(SightSprite *sprite);
void sight_spawnSidePanel(Camera *player, u8 kind, u8 arg2, s8 arg3, u8 arg4);
void sight_handlePanelMessage(SightSprite *sprite, void *msg, u8 msgId);
s32 sight_updateSidePanel(SightSprite *sprite);
void func_151CBB6C(SightOwnedEffect *effect, void **msg, u8 msgId);
s32 func_151CBBE0(SightSprite *sprite);
s32 func_151CBC60(SightSprite *sprite);
f32 sight_getSpriteValue(SightSprite *sprite);
void sight_flashSprites(Camera *player);
void sight_startSpriteFlash(SightSprite *sprite, void *msg, u8 msgId);
void sight_spawnCrossBars(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5);
s32 sight_updateCrossBar(SightSprite *sprite);
void sight_spawnCrossSight(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5);
void sight_spawnLargeRing(Camera *player, u8 red, u8 green, u8 blue);
void func_151CCE94(Camera *player);
void sight_spawnScopeFrame(Camera *player, u8 arg1, s8 arg2, u8 arg3);
s32 sight_updateScopeStrip(SightSprite *sprite);
extern const SightColor D_800AAC80;
extern const SightColor D_800AAC90[];
extern const SightColor D_800AACE0[];
extern const f32 D_800AAD30[];
extern const f32 D_800AAD44[];
extern const f32 D_800AAD58[];
extern const f32 D_800AAD6C[][2];
extern const f32 D_800AAD94[][2];

#endif
