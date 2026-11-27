#ifndef CONKER_SIGHT_H
#define CONKER_SIGHT_H

#include <ultra64.h>
#include "structs.h"
#include "conker/screen.h"

/*
 * Subsystem "sight": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Effects/Sight/SightBeam/sightbeam.c
 *   game/Effects/Sight/effects_sight.c
 */

/* Types that other headers define; the declarations below use them. */
struct BoneGlow; /* conker/particle.h */

/* Message 0x37 (see sight_handlePanelMessage). */
typedef struct {
    u8 side;
    Camera *player;
} SightPanelMsg;

/* Effect spawned by sight_spawnMultiplayerTint: a screen_spawnTintEffect screen tint with the owning player as its extra data. */
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

/* game/Effects/Sight/SightBeam/sightbeam.c */
s32 sight_placeBeam(TriggerZone *beam);
SightBeamObject *sight_spawnBeam(Vec3f *start, CollisionTri *startTri, Vec3f *end, CollisionTri *endTri,
                               TriggerZone *beam, TriggerZone *source, u8 red, u8 green, u8 blue,
                               u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks,
                               u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle,
                               f32 pulseSpeed, u8 keepEnabled, u8 owner, s32 list);
void *sight_buildBeamQuad(SightBeamObject *obj, s16 view);
s32 sight_updateBeam(SightBeamObject *obj);
void sight_notifyActorMoved(Actor *actor, TriggerZone *source);
void sight_handleBeamMessage(SightBeamObject *obj, void *msg, u8 msgId);
s32 sight_onBeamSound(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume);
s32 sight_doesBeamHitActor(Actor *actor, SightBeamObject *obj);
void sight_breakBeam(Actor *actor, SightBeamObject *obj);
SightExtension *sight_spawnExtension(SightBeamObject *owner, Vec3f *start, Vec3f *end, u8 red, u8 green, u8 blue,
                              ZoneNode *zone, u8 zoneMasked, u8 fxOwner, s32 fxList);
void sight_spawnEmitter(SightEmitter **emitters, Vec3f *pos, CollisionTri *triangle, Vec3f *dir, ZoneNode *zone,
                   u8 altColors, u8 zoneMasked, u8 owner, s32 list);
void sight_destroyBeam(SightBeamObject *obj);
void sight_freeBeam(SightBeamObject *obj);
s32 sight_isExtensionOn(SightExtension *effect, s32 view);
void sight_sendBeamCommand(s32 command, TriggerZone *source);
void sight_explodeAtActor(Actor *actor, u8 owner, s32 list);
void sight_spawnFloorImpact(Actor *actor, u8 owner, s32 list);
s32 sight_updateStatusLight(LightEffect *obj);
s32 func_151C9B64(struct BoneGlow *emitter, u8 *visible);
extern const s32 gSightBeamBreakHits[];

/* game/Effects/Sight/effects_sight.c */
void sight_startPlayerEffects(Camera *player, s32 state);
void sight_spawnScopeSight(Camera *player, u8 hideInCutscene, s8 sourceIndex, u8 owner, s32 list);
void sight_resetPlayerEffects(Camera *player);
void sight_spawnRing(Camera *player, u8 type, u8 hideInCutscene, s8 sourceIndex, u8 owner);
void sight_spawnBasicSight(Camera *player, u8 big);
void sight_handleSpriteMessage(SightSprite *sprite, void **msg, u8 msgId);
void sight_spawnDotSight(Camera *player, u8 big);
void sight_spawnZoomSight(Camera *player, u8 big);
void sight_drawLens(SightLensObject *obj, u16 *framebuffer);
void sight_handleLensMessage(SightLensObject *obj, void **msg, u8 msgId);
s32 sight_updateZTriggerFade(SightSprite *sprite);
void sight_spawnSidePanel(Camera *player, u8 kind, u8 hideInCutscene, s8 sourceIndex, u8 owner);
void sight_handlePanelMessage(SightSprite *sprite, void *msg, u8 msgId);
s32 sight_updateSidePanel(SightSprite *sprite);
void sight_handleTintMessage(SightOwnedEffect *effect, void **msg, u8 msgId);
s32 sight_updateCircleDot(SightSprite *sprite);
s32 sight_updateRingGauge(SightSprite *sprite);
f32 sight_getSpriteValue(SightSprite *sprite);
void sight_flashSprites(Camera *player);
void sight_startSpriteFlash(SightSprite *sprite, void *msg, u8 msgId);
void sight_spawnCrossBars(Camera *player, u8 red, u8 green, u8 blue, u8 hideInCutscene, u8 owner);
s32 sight_updateCrossBar(SightSprite *sprite);
void sight_spawnCrossSight(Camera *player, u8 red, u8 green, u8 blue, u8 hideInCutscene, u8 owner);
void sight_spawnLargeRing(Camera *player, u8 red, u8 green, u8 blue);
void sight_spawnMultiplayerTint(Camera *player);
void sight_spawnScopeFrame(Camera *player, u8 hideInCutscene, s8 sourceIndex, u8 owner);
s32 sight_updateScopeStrip(SightSprite *sprite);
extern const SightColor gSightRingFlashColour;
extern const SightColor gSightRingColours[];
extern const SightColor gSightRingOutlineColours[];
extern const f32 gSightRingHalfWidths[];
extern const f32 gSightRingHalfHeights[];
extern const f32 gSightRingXs[];
extern const f32 gSightValueRangeMins[][2];
extern const f32 gSightValueRangeScales[][2];

#endif
