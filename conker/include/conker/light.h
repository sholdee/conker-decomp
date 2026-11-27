#ifndef CONKER_LIGHT_H
#define CONKER_LIGHT_H

#include <ultra64.h>
#include "structs.h"
#include "conker/effect.h"
#include "effects.h"

/*
 * Subsystem "light": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Lighting/LevelLights/levellights.c
 *   game_38050.c
 *   game/Lighting/PlayerLight/playerlight.c
 *   game/Effects/AmbientPulse/ambientpulse.c
 *   game/Effects/ColourWave/colourwave.c
 *   game/Effects/SwingingLamp/swinginglamp.c
 *   game/Lighting/PointLight/pointlight.c
 *   game/Lighting/AmbientZone/ambientzone.c
 *   game/Effects/Light/light.c
 *   game_22D1B0.c (data only, owner unknown; linked after game/Effects/AttachedBurst/attachedburst)
 *   game/Effects/ActorFlicker/actorflicker.c
 *   game/Effects/LampParticle/lampparticle.c
 */

/* Per-channel colour wave: channel = base + amplitude * sin(phase). */
typedef struct {
    f32 baseR;
    f32 baseG;
    f32 baseB;
    f32 amplitudeR;
    f32 amplitudeG;
    f32 amplitudeB;
    f32 phaseR;
    f32 phaseG;
    f32 phaseB;
    f32 speedR; // phase advance per frame
    f32 speedG;
    f32 speedB;
} ColourWave;

/* A TimedEffect carrying a ColourWave. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ColourWave wave;
} ColourWaveEffect;

/* Payload of message 0x1D: follow a sequence track for a while. */
typedef struct {
    s32 track;
    s32 duration;
    u8 sceneTag;
} LampTrackMsg;

/* Event message about actors: event 0 removes `actor`; event 0x2D swaps actor and other. */
typedef struct {
    Actor *actor;
    union {
        Actor *other; // event 0x2D
        u8 actorId;       // event 0
    } u4;
    u8 actorId;           // event 0x2D: id of `actor`
    u8 otherId;           // event 0x2D: id of `other`
} ActorEvent;

/* Global colour cycle: a TimedEffect carrying a LightColourPulse. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ LightColourPulse pulse;
} ColourPulseEffect;

/* Event 0x33 message for light_handleFlashEvent. */
typedef struct {
    f32 level;
    u8 id;
    u8 r;
    u8 g;
    u8 b;
} LightFlashEvent; /* size 0x8 */

/* Extra data of the swinging lamp particle: a sine swing whose amplitude and speed ramp at their rates
 * while `timer` runs (started by light_startLampSwing), then ease back toward their targets. */
typedef struct {
    /* 0x00 */ f32 out;         // current swing angle, used as the particle's roll
    /* 0x04 */ f32 amp;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ f32 targetAmp;
    /* 0x14 */ f32 targetSpeed;
    /* 0x18 */ f32 timer;
    /* 0x1C */ f32 ampRate;
    /* 0x20 */ f32 speedRate;
} LampSwing; // size 0x24

/* The swinging lamp: a ModelParticle carrying a LampSwing as its data. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ LampSwing swing;
} LampParticle;

/* game/Lighting/LevelLights/levellights.c */
void light_reset(void);
void light_applyZones(void);
void light_applyZone(s32 index);
void func_150095D8(PointLight *light, s32 id);
void func_15009600(PointLight *light, s32 id);
void func_15009628(PointLight *light, s32 id);
void func_1500969C(PointLight *light, s32 id);
void func_150096C4(PointLight *light, s32 id);
void func_15009740(PointLight *light, s32 id);
void light_cbFlicker0(PointLight *light, s32 id);
void func_150097A4(PointLight *light, s32 id);
void func_150097CC(PointLight *light, s32 id);
void light_cbLampParticle0(PointLight *light, s32 id);
void light_cbLampParticle1(PointLight *light, s32 id);
void light_cbSwingingLamps(PointLight *light, s32 id);
void light_cbFlickerSound0(PointLight *light, s32 id);
void func_150098D0(PointLight *light, s32 id);
void light_cbColourCycle1(PointLight *light, s32 id);
void light_cbJitter1(PointLight *light, s32 id);
void light_cbObjectCycle(PointLight *light, s32 id);
void func_15009A38(PointLight *light, s32 id);
void light_cbLightning1(PointLight *light, s32 id);
void light_cbJitter2(PointLight *light, s32 id);
void light_cbJitter20(PointLight *light, s32 id);
void light_cbJitter3(PointLight *light, s32 id);
void func_15009BD0(PointLight *light, s32 id);
void func_15009C7C(PointLight *light, s32 id);
void light_cbPulse1(PointLight *light, s32 id);
void light_cbPulse2(PointLight *light, s32 id);
void light_cbJitter4(PointLight *light, s32 id);
void light_cbJitter5(PointLight *light, s32 id);
void light_cbFlickerSound1(PointLight *light, s32 id);
void light_cbPulse3(PointLight *light, s32 id);
void light_cbEmptyEffect(PointLight *light, s32 id);
void light_cbFlickerSound3(PointLight *light, s32 id);
void light_cbPulse4(PointLight *light, s32 id);
void func_15009F74(PointLight *light, s32 id);
void light_cbPulse5(PointLight *light, s32 id);
void light_cbPulse6(PointLight *light, s32 id);
void light_cbJitter7(PointLight *light, s32 id);
void light_cbJitter9(PointLight *light, s32 id);
void light_cbJitter10(PointLight *light, s32 id);
void light_cbJitter11(PointLight *light, s32 id);
void light_cbFlickerSound4(PointLight *light, s32 id);
void light_cbPulse7(PointLight *light, s32 id);
void light_cbLightning2(PointLight *light, s32 id);
void light_cbLampParticleById(PointLight *light, s32 id);
void light_cbFlashById(PointLight *light, s32 id);
void light_cbPulse8(PointLight *light, s32 id);
void light_cbPulse9(PointLight *light, s32 id);
void light_cbJitter15(PointLight *light, s32 id);
void light_cbPulse10(PointLight *light, s32 id);
void light_cbFlicker5(PointLight *light, s32 id);
void light_cbPulse11(PointLight *light, s32 id);
void light_cbPulse12(PointLight *light, s32 id);
void light_cbPulse13(PointLight *light, s32 id);
void func_1500A55C(PointLight *light, s32 id);
void light_cbJitter17(PointLight *light, s32 id);
void light_cbJitter18(PointLight *light, s32 id);
void light_cbJitter19(PointLight *light, s32 id);
void light_cbJitter21(PointLight *light, s32 id);
void func_1500A6D8(PointLight *light, s32 id);
void light_cbJitter22(PointLight *light, s32 id);
void light_cbSwingOnObject(PointLight *light, s32 id);
void light_cbFlicker6(PointLight *light, s32 id);
void light_cbFlickerSoundById(PointLight *light, s32 id);
void light_runCallback(PointLight *light, s32 id);
void light_startColourWave(s32 unused);
void light_runAmbientAction(s32 action);

/* game_38050.c */
void light_initParticleSlots(s32 arg0);
void light_loadParticleTextures(void);
void light_setupParticleSlot(s32 slot, s32 id, s32 flags);
s32 light_addLevelParticle(s32 index, s32 slot, s32 subtype, s16 x, s16 y, s16 z, s32 jitter, u8 kind, s32 parity);
s32 light_scatterParticles(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
s32 light_placeParticlePattern(s16 pattern, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
s32 light_placeCentreParticle(s16 amount, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
s32 light_scatterParticles87(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
s32 light_scatterParticles8(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
s32 light_scatterParticles33(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
void light_resetSpriteBuffers(s32 unused);
void light_resetParticles(void);
void light_buildLevelParticles(void);

/* game/Lighting/PlayerLight/playerlight.c */
void light_easePlayerLight(s32 player);
void light_updatePlayerLight(s32 player);

/* game/Effects/AmbientPulse/ambientpulse.c */
void light_updateAmbientRedDrift(AmbientPulse *arg0); /* AmbientPulse: conker/effect.h */
void light_levelHook31(s32 *arg0);

/* game/Effects/ColourWave/colourwave.c */
void light_updateColourWave(ColourWaveEffect *object);

/* game/Effects/SwingingLamp/swinginglamp.c */
void light_spawnSwingingLamps(PointLight *light);
s32 light_updateLampModel(DynModel *model);
void light_updateLampTrackTimer(DynModel *model);
void light_updateLampHeight(DynModel *model);
s32 light_updateLampLitModel(DynModel *model);
s32 light_buildLampMatrix(DynModel *model, s32 viewport);
s32 light_updateLampLight(LightEffect *effect);
void light_setLampTrack(s32 track, s32 duration, u8 sceneTag);
void light_handleLampMessage(DynModel *model, LampTrackMsg *msg, u8 event);
void light_toggleLamp(void);
s32 light_drawLampLitModel(DynModel *model, s16 viewport);

/* game/Lighting/PointLight/pointlight.c */
extern u8 gAmbientColour[3]; // ambient light colour
extern u8 gDirLightColour[3]; // tint colour
extern u8 gAmbientPriority; // priority of the current ambient colour
extern u8 gDirLightAmbientTint[3];
extern s8 gPlayerLightDirs[4][3]; // per-player directional light direction
extern u8 gDirLightAmount;
extern u8 gDirLightAmbientAmount;
extern PointLightGfx gPadLight; // padding light for odd light counts
extern PointLight *gPointLights; // list of all point lights
extern s32 gLightingEnabled;
extern ZoneLink *gAmbientZones; // ambient colour TriggerZones
extern ZoneLink *gDirectionalZones; // tint TriggerZones
void *light_allocAmbients(void);
void *light_allocGfxLights(s32 count);
void light_setAmbient(s32 r, s32 g, s32 b, u8 priority);
PointLight *light_allocPointLight(void);
PointLight *func_1515D5AC(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group);
PointLight *light_createPointLight(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group); /* light_freePointLight frees it */
void light_clearPlayerMasks(void);
void func_1515D6C8(void);
Gfx *light_beginView(Gfx *gfx, s32 player);
Gfx *light_pickLights(Gfx *gfx, s32 player, s32 x, s32 y, s32 z, u32 arg5, PointLightGfx *lightBuf, s32 maxLights,
                   Ambient *ambient, u8 *outCount, s32 mask, u8 *smoothColor, s32 flags, PointLight **outLights);
void light_getAmbientAt(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags);
void light_getDirectionalTintAt(s32 x, s32 y, s32 z, s32 arg3, u8 *colorA, u8 *colorB, u8 *amountA, u8 *amountB);
Gfx *light_reemitLights(Gfx *gfx, PointLightGfx *lightBuf, u8 maxLights, u8 countByte, Ambient *ambient);
void light_getLitColourAt(s32 player, s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg6, s32 flags);
TriggerZone *light_findZone(s32 x, s32 y, s32 z, s32 mask, ZoneLink *node);
void light_fillGfxLight(PointLight *light, s32 player, PointLightGfx *out, s32 x, s32 y, s32 z, s32 flags);
void light_computeLightSpaceScale(s16 *mtx);
f32 light_getMtxElement(s16 *mtx, s32 i);
void light_storeScaleFixed(f32 value, s32 i);
void light_storeScaleInt(f32 value, s32 i);
/* (PointLight *light); declared without a prototype: game/Effects/Light/light.c passes a second argument */
void light_freePointLight();
void light_setGroupDisabled(s32 group, u8 disabled);

/* game/Lighting/AmbientZone/ambientzone.c */
extern f32 gDustRegionsArea; // total area of the gDustRegions spawn regions
extern f32 gAmbientEaseRed; // ambient effect colour being eased (red, green, blue)
extern f32 gAmbientEaseGreen;
extern f32 gAmbientEaseBlue;
extern f32 gAmbientEasePhase; // ambient effect 2 pulse phase
extern f32 gSavedAmbientRed;
extern f32 gSavedAmbientGreen;
extern f32 gSavedAmbientBlue;
extern f32 gSavedDirLightAmbientTintRed;
extern f32 gSavedDirLightAmbientTintGreen;
extern f32 gSavedDirLightAmbientTintBlue;
extern f32 gSavedDirLightAmount;
extern f32 gSavedDirLightAmbientAmount;
extern RegionNode *gDustRegions; // spawn regions; their areas add up to gDustRegionsArea
/* (TriggerZone *region); declared without a prototype: game/Level/ZoneSpawn/zonespawn.c calls it without the region */
RegionNode *light_newRegionNode();
void light_pushRegionNode(RegionNode **list, RegionNode *node);
void light_runHandlerZone(Actor *obj, TriggerZone *zone);
void func_1515F2B8(Actor *obj, TriggerZone *zone);
void light_runAmbientZoneEffect(Actor *obj, TriggerZone *zone);
void light_easeAmbientToGrey(Actor *obj, TriggerZone *zone);
void light_easeAmbientToYellowGreen(Actor *obj, TriggerZone *zone);
void light_easeAmbientToPulse(Actor *obj, TriggerZone *zone);
void func_1515FB70(Actor *obj, TriggerZone *zone);
void func_1515FB94(Actor *obj, TriggerZone *zone);
void func_1515FBC4(Actor *obj, TriggerZone *zone);
void func_1515FC34(Actor *obj, TriggerZone *zone);
void light_killActorInZone(Actor *obj, TriggerZone *zone);

/* game/Effects/Light/light.c */
extern u8 gDaylight;
LightEffect *light_createWithEffect(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                           u8 arg6, u8 arg7, s32 size, u8 arg9, s32 argA);
LightEffect *light_createEffect(LightEffectHeader *header, PointLight *light, s32 size, u8 arg3, s32 arg4);
void light_updateEffect(LightEffect *effect);
void light_handleEffectEvent(void *arg0, u8 *message, u8 event);
s32 light_updateDebugScale(LightEffect *effect);
s32 light_updatePulse(LightEffect *effect);
s32 light_updateJitter(LightEffect *effect);
s32 light_updateMounted(LightEffect *effect);
s32 light_updateFlicker(LightEffect *effect);
void light_startFlicker(f32 *intensity, f32 *peak, f32 *rate, f32 *delay, LightEffect *effect);
LightEffect *light_createAttached(Actor *actor, u8 bone, const Vec3f *offset, u8 arg3, s16 lifetime, s32 arg5, s32 arg6, s32 arg7,
                           s32 arg8, s32 arg9, s32 argA, s8 callback, s32 extraSize, u8 eventHandler, u8 argE,
                           s32 argF);
u8 light_followActor(LightEffect *effect);
LightEffect *light_createBeam(Actor *actor, u8 bone, const Vec3f *start, const Vec3f *end, f32 scale, u8 arg5, s16 lifetime,
                           s32 arg7, s32 arg8, s32 arg9, s32 argA, u8 fromBone, u8 eventHandler, u8 flagHit, u8 argE,
                           s32 argF);
s32 light_updateBeam(LightEffect *effect);
s32 light_isActorLive(Actor *actor, Actor *exclude);
LightEffect *func_1516127C(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161334(PointLight *light, u8 arg1, s32 arg2);
LightEffect *light_createMounted(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161494(PointLight *light, u8 arg1, s32 arg2);
LightEffect *func_15161540(PointLight *light, u8 arg1, s32 arg2);
LightEffect *light_createFlicker(PointLight *light, u8 sound, u8 kind, u8 type, u8 arg4, s32 arg5);
void light_sendEvent(u8 lightType, u8 event, void *arg2);
void light_destroyAttached(LightEffect *effect);
void light_freeAttached(LightEffect *effect);
void light_destroyBeam(LightEffect *effect);
void light_freeBeam(LightEffect *effect);
void light_defaultDestroy(LightEffect *effect);
void light_defaultFree(LightEffect *effect);
void light_destroyEffect(LightEffect *effect);
void light_freeEffect(LightEffect *effect);
void light_playRandomSound(u16 arg0, s16 arg1, u8 arg2, s32 arg3, Vec3f *position, s16 arg5, s16 arg6);
LightEffect *light_createSeqTrack(s32 index, s16 lifetime, u8 arg2, s32 arg3);
s32 light_updateSeqTrack(LightEffect *effect);
LightEffect *light_createAtActor(Actor *actor, u8 eventHandler, u8 arg2, s16 lifetime, s32 arg4, s32 arg5, s32 arg6,
                           s32 arg7, u8 arg8, s32 arg9);
s32 light_updateAtActor(LightEffect *effect);
void light_handleActorEvent(LightEffect *effect, ActorEvent *event, u8 eventId);
LightEffect *func_15162034(PointLight *light, u8 arg1, s32 arg2);
void light_startColourCycle(s32 arg0);
void light_updateColourCycle(ColourPulseEffect *object);
LightEffect *light_createColourCycle(PointLight *light, u8 kind, u8 type, u8 arg3, s8 update, s16 lifetime, u8 arg6, s32 arg7);
s32 light_updateColourCycleEffect(LightEffect *effect);
LightEffect *light_createJitter(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7);
LightEffect *light_createJitterAt(LightEffectHeader *header, Vec3i *position, s32 arg2, s32 arg3, s32 arg4, u8 arg5,
                           u8 arg6, s32 extraSize, u8 kind, u8 arg9, s32 argA);
LightEffect *light_createCycle(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, s8 onLow,
                           s8 onRise, s8 onHigh, s8 onFall, s32 extraSize, u8 argB, s32 argC);
s32 light_updateCycle(LightEffect *effect);
void light_cycleSetObjectFlag(LightEffect *effect);
void light_cycleClearObjectFlag(LightEffect *effect);
LightEffect *light_createLightning(PointLight *light, u8 kind, u8 type, u8 arg3, s16 lifetime, s8 update, u8 arg6, s32 arg7);
s32 light_updateLightning(LightEffect *effect);
LightEffect *light_createTracking(LightEffectHeader *header, f32 *x, f32 *y, f32 *z, s8 callback, u8 handler, u8 arg6,
                           u8 arg7, u8 arg8, u8 arg9, u8 argA, u8 argB, s32 extraSize, u8 argD, s32 argE);
s32 light_updateTracking(LightEffect *effect);
void light_handleTrackingEvent(LightEffect *effect, s32 arg1, u8 eventId);
LightEffect *light_createPulse(PointLight *light, u8 kind, u8 type, s16 lifetime, u8 arg4, s32 arg5, u8 arg6, s32 arg7);
s32 light_updateOneShotPulse(LightEffect *effect);
void light_createEmptyEffect(PointLight *light, u8 type, u8 arg2, s32 arg3);
LightEffect *light_createSizedEffect(PointLight *light, u8 flags, s8 update, s16 lifetime, u8 type, s32 size, u8 arg6, s32 arg7);
s32 light_updateColourAnimFollow(LightEffect *effect);
void light_handleSwitchEvent(LightEffect *effect, s32 arg1, u8 eventId);
void light_handleInvertedSwitchEvent(LightEffect *effect, s32 arg1, u8 eventId);
LightEffect *light_createObjectPulse(u8 objectId, u8 arg1, s32 arg2);
s32 light_updateObjectPulse(LightEffect *effect);
LightEffect *light_createActorPulse(Actor *actor, u8 arg1, s32 arg2);
s32 light_updateAttachedPulse(LightEffect *effect);
void light_applyPulse(LightPulse *pulse, LightEffect *effect);
void light_applyJitter(LightEffect *effect, LightJitter *jitter);
s32 light_trackActorRef(LightEffect *effect, LightActorRef *ref);
void light_handleAttachEvent(LightEffect *effect, ActorEvent *event, u8 eventId);
void light_handleActorRefEvent(LightEffect *effect, ActorEvent *event, u8 eventId);
void light_handleBeamEvent(LightEffect *effect, ActorEvent *event, u8 eventId);
LightEffect *light_createFlash(PointLight *light, u8 id, u8 arg2, s32 arg3);
s32 light_updateFlash(LightEffect *effect);
void light_handleFlashEvent(LightEffect *effect, LightFlashEvent *event, u8 eventId);
void light_handleBeamToggleEvent(LightEffect *effect, s32 arg1, u8 eventId);
s32 light_updateTransformed(LightEffect *effect);
void light_placeTransformed(LightEffect *effect, LightTransformed *point);
s32 light_updateSwing(LightEffect *effect);
void light_placeSwing(LightEffect *effect, Vec3f *pivot, f32 offset, f32 angleX, f32 angleZ);
void light_setDaylight(u8 enable);
extern const f32 gLightJitterLows[24];
extern const f32 gLightJitterMids[24];
extern const f32 gLightJitterHighs[24];

/* game_22D1B0.c (data only, owner unknown; linked after game/Effects/AttachedBurst/attachedburst): the colours that
 * light_setDaylight sets for levels 6 and 57 */
extern u8 gDaylightOnAmbient[3];
extern u8 gDaylightOnBackground[3];
extern u8 gDaylightOffAmbient[3];

/* game/Effects/ActorFlicker/actorflicker.c */
void light_updateActorFlicker(LightEffect *effect);

/* game/Effects/LampParticle/lampparticle.c */
void light_spawnLampParticle(PointLight *light, u8 kind, u8 arg2, s32 arg3);
s32 light_updateLampParticle(LampParticle *particle);
s32 light_getLampParticleMatrix(Mtx *dest, LampParticle *particle);
s32 light_updateLampParticleLight(LightEffect *effect);
void light_startLampSwing(LampParticle *particle);

#endif
