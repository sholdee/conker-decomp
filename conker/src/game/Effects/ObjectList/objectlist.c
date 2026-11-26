#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/effect.h describes it. */
EffectHeader *gObjectLists[2][OBJ_TYPE_COUNT];
s8 gObjectWalkDepth;
void *gObjectWalkNext[6];
s32 gCachedTextureId;
s32 gCachedTexture;
s32 gCachedTextureFrame;
s16 gCachedRenderModeKey;
s16 gCachedCombineKey;
s16 gCachedPrimR;
s16 gCachedPrimG;
s16 gCachedPrimB;
s16 gCachedPrimA;
s16 gCachedEnvR;
s16 gCachedEnvG;
s16 gCachedEnvB;
s16 gCachedEnvA;
f32 gMainCameraAngleSinCos[2];
f32 gCameraAngleSines[4];
f32 gCameraAngleCosines[4];
static s16 D_800DD1F8;
s32 gCachedGeomSetBits;
s32 gCachedGeomClearBits;
s16 gCachedPrimMinLevel;
s16 gCachedPrimLodFrac;
s32 gCachedTextureCm;
s32 gCachedTextureS;
s32 gCachedTextureT;
SpriteParams *gCachedTextureSprite;
s32 gCachedOtherModeH;
s32 gCachedOtherModeL;
s32 gDrawHookTextureFrame;
s32 gDrawHookTextureCm;
TextureDesc *gDrawHookTexture;

#include "functions.h"
#include "variables.h"


/*
 * While a list is being walked, the next node is saved in gObjectWalkNext[depth] (depth gObjectWalkDepth) so that
 * callbacks may destroy the current node; effect_fixWalkCursors repairs saved entries that point at a dying node.
 */

/* Type 0x10 object (0xF0 bytes): a display list drawn with a per-frame matrix (created by effect_spawnModelFx). */
struct ModelFx {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ Mtx mtx[2];      /* double-buffered by D_800BE9C0 */
    /* 0x90 */ ModelFxData data;
};

/* Type 3 object (0x28 bytes): a fading, animated billboard sprite. */
struct SpriteFx {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ TextureDesc *anim;
    /* 0x14 */ s16 frame;       /* 8.8 fixed point index into the frame table at anim->addr */
    /* 0x16 */ s16 frameStep;
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 z;
    /* 0x1E */ s16 scaleX;
    /* 0x20 */ s16 scaleY;
    /* 0x22 */ s8 fadeStep;     /* > 0: fade out by this per frame; < 0: fade on the last frame */
    /* 0x23 */ u8 alpha;
    /* 0x24 */ u8 callback;     /* index into gObjectCallbacks, 0 = none */
};

/* Render setup of object type 14 (QuadFx). */
static Gfx gQuadFxRenderSetupDl[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_CULL_BOTH | G_FOG | G_LIGHTING | G_LOD),
    gsSPEndDisplayList(),
};

/* Render teardown of object type 16 (ModelFx): back to the fixed modelview matrix. */
static Gfx gModelFxRenderTeardownDl[] = {
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

/*
 * The object types: per type its update, draw, event and destroy handlers and the display lists drawn
 * around its objects (see ObjType). Entries: { update, preUpdate, draw, preDraw, drawBeginDL, drawBegin, unk18,
 * handleEvent, drawEnd, drawEndDL, destroy, free, drawMode }.
 */
static ObjType gObjectTypes[OBJ_TYPE_COUNT] = {
    /*   0 */ { effect_defaultFree, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   1 */ { effect_updatePendingFree, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   2 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*   3 */ { effect_updateSpriteFx, NULL, effect_drawSpriteFx, NULL, gSpriteRenderDl, NULL, NULL, NULL, NULL, gSpriteEndDl, NULL, NULL, 1 },
    /*   4 */ { effect_updateQuadEffect, NULL, effect_drawQuadEffect, NULL, gQuadEffectRenderSetupDl, effect_beginQuadEffectDraw, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*   5 */ { effect_updateBillboard, NULL, effect_drawBillboard, NULL, gSpriteRenderDl, effect_beginBillboardDraw, NULL, NULL, NULL, gSpriteEndDl, NULL, NULL, 1 },
    /*   6 */ { effect_updateFadeBurst, NULL, effect_drawFadeBurst, NULL, gFadeBurstRenderSetupDl, NULL, NULL, NULL, NULL, gFadeBurstRenderTeardownDl, NULL, NULL, 1 },
    /*   7 */ { particle_updateFallingEmitter, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*   8 */ { particle_updateSpinDebris, NULL, particle_drawSpinDebris, NULL, gSpinDebrisZbSetupDl, particle_beginSpinDebrisDraw, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*   9 */ { particle_updateGroundChunk, NULL, particle_drawSpinDebris, NULL, gSpinDebrisZbSetupDl, particle_beginSpinDebrisDraw, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  10 */ { swarm_updateFlight, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  11 */ { beam_updateTracer, NULL, beam_drawTracer, NULL, gTracerRenderSetupDl, NULL, NULL, beam_tracerSwapMessage, func_150E2F90, NULL, NULL, NULL, 1 },
    /*  12 */ { particle_updateSpinDebris, NULL, particle_drawSpinDebris, NULL, gSpinDebrisNoZSetupDl, particle_beginSpinDebrisDraw, NULL, NULL, particle_endSpinDebrisDraw, gSpriteEndDl, NULL, NULL, 1 },
    /*  13 */ { particle_updatePuffQuad, NULL, particle_drawPuffQuad, NULL, gPuffQuadSetupDl, particle_beginPuffQuadDraw, NULL, NULL, particle_endPuffQuadDraw, NULL, NULL, NULL, 1 },
    /*  14 */ { effect_updateQuadFx, NULL, effect_drawQuadFx, NULL, gQuadFxRenderSetupDl, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  15 */ { particle_updateFloorSprite, NULL, particle_drawFloorSprite, NULL, gFloorSpriteSetupDl, NULL, NULL, NULL, NULL, gFloorSpriteEndDl, NULL, NULL, 0 },
    /*  16 */ { effect_updateModelFx, NULL, effect_drawModelFx, NULL, NULL, NULL, NULL, NULL, NULL, gModelFxRenderTeardownDl, NULL, NULL, 0 },
    /*  17 */ { particle_updateSpriteEmitter, NULL, NULL, NULL, NULL, NULL, particle_tickStyleEmitterTimers, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  18 */ { particle_updateStyleParticle, NULL, particle_drawStyleParticle, NULL, gStyleParticleSetupDl, NULL, NULL, NULL, NULL, gStyleParticleEndDl, NULL, NULL, 1 },
    /*  19 */ { swarm_updatePercher, NULL, swarm_drawActor, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  20 */ { swarm_updateFlight, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  21 */ { swarm_updateHoverer, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  22 */ { swarm_updateHoverer, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  23 */ { swarm_updateHoverer, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  24 */ { swarm_updateHoverer, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  25 */ { particle_updateModelParticle, NULL, particle_drawModelParticle, particle_buildModelParticleMatrix, NULL, NULL, NULL, particle_onModelParticleEvent, NULL, NULL, particle_destroyModelParticle, particle_freeModelParticle, 1 },
    /*  26 */ { trail_updateSprayEmitter, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  27 */ { particle_updateQuadSprite, NULL, particle_drawQuadSprite, NULL, gQuadParticleSetupDl, NULL, NULL, particle_onQuadSpriteEvent, NULL, gQuadParticleEndDl, particle_destroyQuadSprite, particle_freeQuadSprite, 1 },
    /*  28 */ { particle_updateQuadParticle, NULL, particle_drawQuadParticle, NULL, gQuadParticleSetupDl, particle_sortDrawList, NULL, particle_onQuadParticleEvent, NULL, gQuadParticleEndDl, particle_destroyQuadParticle, particle_freeQuadParticle, 1 },
    /*  29 */ { effect_updateQueue, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  30 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  31 */ { trail_updateEmitter, NULL, NULL, NULL, NULL, NULL, NULL, trail_onEmitterMessage, NULL, NULL, NULL, NULL, -1 },
    /*  32 */ { particle_updateDripEmitter, NULL, NULL, NULL, NULL, NULL, NULL, particle_onDripEmitterMsg, NULL, NULL, particle_destroyDripEmitter, particle_freeDripEmitter, -1 },
    /*  33 */ { particle_updateActorEmitter, NULL, NULL, NULL, NULL, NULL, NULL, particle_onActorEmitterMsg, NULL, NULL, particle_destroyActorEmitter, particle_freeActorEmitter, 0 },
    /*  34 */ { effect_updateEffectObj, NULL, effect_drawEffectObj, NULL, gEffectObjRenderSetupDl, NULL, NULL, effect_handleEffectObjMessage, NULL, gEffectObjRenderResetDl, effect_destroyEffectObj, effect_freeEffectObj, 1 },
    /*  35 */ { effect_updateTimedEffect, NULL, effect_drawTimedEffect, NULL, gTimedEffectRenderSetupDl, NULL, NULL, effect_handleTimedEffectMessage, NULL, gTimedEffectRenderResetDl, effect_destroyTimedEffect, effect_freeTimedEffect, 1 },
    /*  36 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  37 */ { particle_updatePoolEmitter, NULL, NULL, NULL, NULL, NULL, NULL, particle_onPoolEmitterEvent, NULL, NULL, particle_destroyPoolEmitter, particle_freePoolEmitter, -1 },
    /*  38 */ { cutscene_updateTimer, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  39 */ { effect_updateHomingEffect, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  40 */ { blood_updateDrip, NULL, blood_clearDripStatic, NULL, NULL, NULL, NULL, blood_handleDripMessage, NULL, NULL, blood_destroyDrip, blood_freeDrip, 0 },
    /*  41 */ { blood_updateStrip, NULL, NULL, NULL, NULL, NULL, NULL, blood_handleStripMessage, NULL, NULL, blood_destroyStrip, blood_freeStrip, -1 },
    /*  42 */ { blood_updateTrail, NULL, NULL, NULL, NULL, NULL, NULL, blood_handleTrailMessage, NULL, NULL, NULL, NULL, -1 },
    /*  43 */ { particle_updateSpriteParticle, NULL, particle_drawSpriteParticle, NULL, gSpriteParticleSetupDl, NULL, NULL, particle_onSpriteEvent, NULL, gSpriteParticleResetDl, particle_destroySpriteParticle, particle_freeSpriteParticle, 1 },
    /*  44 */ { particle_updateStreakParticle, NULL, particle_drawStreakParticle, NULL, gStreakParticleSetupDl, NULL, NULL, NULL, NULL, NULL, particle_destroyStreakParticle, particle_freeStreakParticle, 1 },
    /*  45 */ { beam_updateLightning, NULL, beam_drawLightning, NULL, gLightningRenderSetupDl, NULL, NULL, beam_lightningMessage, NULL, gLightningRenderTeardownDl, beam_destroyLightning, beam_freeLightning, 1 },
    /*  46 */ { effect_updateCharacterWatcher, NULL, NULL, NULL, NULL, NULL, NULL, effect_characterWatcherMessage, NULL, NULL, effect_destroyCharacterWatcher, effect_freeCharacterWatcher, -1 },
    /*  47 */ { NULL, rope_update, rope_draw, NULL, NULL, NULL, NULL, rope_message, NULL, NULL, NULL, NULL, 1 },
    /*  48 */ { NULL, rope_updateRider, NULL, NULL, NULL, NULL, NULL, rope_riderMessage, NULL, NULL, NULL, NULL, -1 },
    /*  49 */ { effect_updateRingEffect, NULL, effect_drawRingEffect, NULL, gRingRenderSetupDl, NULL, NULL, NULL, NULL, gRingRenderResetDl, NULL, NULL, 1 },
    /*  50 */ { sprite_updateActorTracker, NULL, NULL, NULL, NULL, NULL, NULL, sprite_actorTrackerMessage, NULL, NULL, sprite_destroyActorTracker, sprite_freeActorTracker, -1 },
    /*  51 */ { rope_updateEffect, NULL, rope_drawEffect, NULL, gRopeRenderSetupDl, NULL, NULL, rope_effectMessage, NULL, gRopeRenderResetDl, func_151B4C6C, func_151B4C98, 1 },
    /*  52 */ { colourfb_updateScreenEffect, NULL, NULL, NULL, NULL, NULL, NULL, colourfb_dispatchMessage, NULL, NULL, NULL, NULL, -1 },
    /*  53 */ { light_updateEffect, NULL, NULL, NULL, NULL, NULL, NULL, light_handleEffectEvent, NULL, NULL, func_15161804, func_15161860, -1 },
    /*  54 */ { effect_updateModelEffect, NULL, effect_drawModelEffect, NULL, gModelEffectRenderSetupDl, NULL, NULL, effect_modelEffectMessage, NULL, gModelEffectRenderTeardownDl, effect_destroyModelEffect, effect_freeModelEffect, 1 },
    /*  55 */ { effect_updateEffectActor, NULL, effect_drawEffectActor, NULL, gEffectActorRenderSetupDl, NULL, NULL, effect_effectActorMessage, NULL, gEffectActorRenderTeardownDl, effect_destroyEffectActor, effect_freeEffectActor, 1 },
    /*  56 */ { effect_updateDynModel, NULL, effect_drawDynModel, NULL, gDynModelRenderSetupDl, NULL, NULL, effect_dynModelMessage, NULL, gDynModelRenderResetDl, effect_destroyDynModel, effect_freeDynModel, 1 },
    /*  57 */ { camera_updateWobble, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  58 */ { swarm_updateSwarm, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, swarm_freeSwarm, swarm_freeSwarm, 0 },
    /*  59 */ { effect_updateBillboard, NULL, swarm_drawMember, NULL, gSpriteRenderDl, effect_beginBillboardDraw, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  60 */ { screen_updateOverlay, NULL, screen_drawOverlay, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 },
    /*  61 */ { effect_updateBoneFollower, NULL, NULL, NULL, NULL, NULL, NULL, effect_boneFollowerMessage, NULL, NULL, effect_destroyBoneFollower, effect_freeBoneFollower, -1 },
    /*  62 */ { effect_updateFollowerGroup, NULL, NULL, NULL, NULL, NULL, NULL, effect_followerGroupMessage, NULL, NULL, effect_destroyFollowerGroup, effect_freeFollowerGroup, -1 },
    /*  63 */ { cont_updateRumbleObj, NULL, NULL, NULL, NULL, NULL, NULL, cont_handleRumbleMessage, NULL, NULL, cont_destroyRumbleObj, cont_freeRumbleObj, -1 },
    /*  64 */ { beam_updateBolt, NULL, beam_drawBolt, NULL, gBoltRenderSetupDl, NULL, NULL, beam_boltMessage, NULL, gBoltRenderTeardownDl, beam_destroyBolt, beam_freeBolt, 1 },
    /*  65 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  66 */ { effect_updateBillboard, NULL, effect_drawBillboard, NULL, gSpriteRenderDl, effect_beginBillboardDraw, NULL, NULL, NULL, gSpriteEndDl, NULL, NULL, 1 },
    /*  67 */ { particle_updateFloorSprite, NULL, particle_drawFloorSprite, NULL, gFloorSpriteSetupDl, NULL, NULL, NULL, NULL, gFloorSpriteEndDl, NULL, NULL, 0 },
    /*  68 */ { swarm_updateFlight, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  69 */ { swarm_updateFlight, NULL, swarm_drawFlying, NULL, NULL, swarm_callModelSecondDl, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  70 */ { beam_updateStrip, NULL, beam_drawStrip, NULL, gStripRenderSetupDl, NULL, NULL, beam_stripMessage, NULL, gStripRenderResetDl, beam_destroyStrip, beam_freeStrip, 1 },
    /*  71 */ { particle_updateSpriteParticle, NULL, particle_drawSpriteParticle, NULL, gSpriteParticleSetupDl, NULL, NULL, particle_onSpriteEvent, NULL, gSpriteParticleResetDl, particle_destroySpriteParticle, particle_freeSpriteParticle, 1 },
    /*  72 */ { particle_updateModelParticle, NULL, particle_drawModelParticle, particle_buildModelParticleMatrix, NULL, NULL, NULL, particle_onModelParticleEvent, NULL, NULL, particle_destroyModelParticle, particle_freeModelParticle, 1 },
    /*  73 */ { particle_updateQuadParticle, NULL, particle_drawQuadParticle, NULL, gQuadParticleSetupDl, NULL, NULL, particle_onQuadParticleEvent, NULL, gQuadParticleEndDl, particle_destroyQuadParticle, particle_freeQuadParticle, 1 },
    /*  74 */ { particle_updateQuadSprite, NULL, particle_drawQuadSprite, NULL, gQuadParticleSetupDl, NULL, NULL, particle_onQuadSpriteEvent, NULL, gQuadParticleEndDl, particle_destroyQuadSprite, particle_freeQuadSprite, 1 },
    /*  75 */ { shadow_updateBlob, NULL, shadow_drawBlob, NULL, gBlobShadowSetupDl, NULL, NULL, NULL, shadow_loadIdentityMtx, NULL, NULL, NULL, 1 },
    /*  76 */ { effect_updateModelEffect, NULL, effect_drawModelEffect, NULL, gModelEffectRenderSetupDl, NULL, NULL, effect_modelEffectMessage, NULL, gModelEffectRenderTeardownDl, effect_destroyModelEffect, effect_freeModelEffect, 1 },
    /*  77 */ { effect_updateEffectObj, NULL, effect_drawEffectObj, NULL, gEffectObjRenderSetupDl, NULL, NULL, effect_handleEffectObjMessage, NULL, gEffectObjRenderResetDl, effect_destroyEffectObj, effect_freeEffectObj, 1 },
    /*  78 */ { NULL, rope_update, rope_draw, NULL, NULL, NULL, NULL, rope_message, NULL, NULL, NULL, NULL, 1 },
    /*  79 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  80 */ { effect_updatePlayerMarker, NULL, effect_drawPlayerMarker, NULL, NULL, NULL, NULL, effect_playerMarkerMessage, NULL, gPlayerMarkerRenderTeardownDl, effect_freePlayerMarker, effect_freePlayerMarker, 0 },
    /*  81 */ { text_updateBox, NULL, text_drawBox, NULL, gSpriteRenderDl, NULL, NULL, NULL, NULL, gSpriteEndDl, NULL, NULL, -1 },
    /*  82 */ { particle_updateSpriteParticle, NULL, particle_drawSpriteParticle, NULL, gSpriteParticleSetupDl, NULL, NULL, particle_onSpriteEvent, NULL, gSpriteParticleResetDl, particle_destroySpriteParticle, particle_freeSpriteParticle, 1 },
    /*  83 */ { particle_updateStreakParticle, NULL, particle_drawStreakParticle, NULL, gStreakParticleSetupDl, NULL, NULL, NULL, NULL, NULL, particle_destroyStreakParticle, particle_freeStreakParticle, 1 },
    /*  84 */ { effect_updateDynModel, NULL, effect_drawDynModel, NULL, gDynModelRenderSetupDl, NULL, NULL, effect_dynModelMessage, NULL, gDynModelRenderResetDl, effect_destroyDynModel, effect_freeDynModel, 1 },
    /*  85 */ { effect_updateEffectActor, NULL, effect_drawEffectActor, NULL, gEffectActorRenderSetupDl, NULL, NULL, effect_effectActorMessage, NULL, gEffectActorRenderTeardownDl, effect_destroyEffectActor, effect_freeEffectActor, 1 },
    /*  86 */ { particle_updateQuadParticle, NULL, particle_drawQuadParticle, NULL, gQuadParticleSetupDl, particle_sortDrawList, NULL, particle_onQuadParticleEvent, NULL, gQuadParticleEndDl, particle_destroyQuadParticle, particle_freeQuadParticle, 1 },
    /*  87 */ { sprite_updateScreenSprite, NULL, sprite_drawScreenSprite, NULL, gScreenSpriteSetupDl, NULL, NULL, sprite_screenSpriteMessage, NULL, gScreenSpriteTeardownDl, NULL, NULL, 1 },
    /*  88 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  89 */ { particle_updateSpinDebris, NULL, particle_drawSpinDebris, NULL, gSpinDebrisNoZSetupDl, particle_beginSpinDebrisDraw, NULL, NULL, particle_endSpinDebrisDraw, NULL, NULL, NULL, 1 },
    /*  90 */ { screen_updateTintEffect, NULL, func_151A5130, NULL, gSpriteRenderDl, NULL, NULL, func_151A55D4, NULL, gSpriteEndDl, NULL, NULL, 1 },
    /*  91 */ { effect_updateModelEffect, NULL, effect_drawModelEffect, NULL, gModelEffectRenderSetupDl, NULL, NULL, effect_modelEffectMessage, NULL, gModelEffectRenderTeardownDl, effect_destroyModelEffect, effect_freeModelEffect, 1 },
    /*  92 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  93 */ { sprite_updateScreenSprite, NULL, sprite_drawScreenSprite, NULL, gScreenSpriteSetupDl, NULL, NULL, sprite_screenSpriteMessage, NULL, gScreenSpriteTeardownDl, NULL, NULL, 1 },
    /*  94 */ { sprite_updateSprite2D, NULL, sprite_drawSprite2D, NULL, gSpriteRenderDl, effect_beginBillboardDraw, NULL, NULL, NULL, gSpriteEndDl, NULL, NULL, 1 },
    /*  95 */ { effect_updateTimedEffect, NULL, effect_drawTimedEffect, NULL, gTimedEffectRenderSetupDl, NULL, NULL, effect_handleTimedEffectMessage, NULL, gTimedEffectRenderResetDl, effect_destroyTimedEffect, effect_freeTimedEffect, 1 },
    /*  96 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /*  97 */ { sprite_updateTexQuad, NULL, sprite_drawTexQuad, NULL, gTexQuadSetupDl, NULL, NULL, NULL, NULL, gTexQuadResetDl, NULL, NULL, 1 },
    /*  98 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /*  99 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /* 100 */ { NULL, screen_updateFade, screen_drawFade, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 1 },
    /* 101 */ { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, -1 },
    /* 102 */ { particle_updateSpriteEmitter, NULL, NULL, NULL, NULL, NULL, particle_tickStyleEmitterTimers, NULL, NULL, NULL, NULL, NULL, 0 },
    /* 103 */ { particle_updateStyleParticle, NULL, particle_drawStyleParticle, NULL, gStyleParticleSetupDl, NULL, NULL, NULL, NULL, gStyleParticleEndDl, NULL, NULL, 1 },
};

/* Particle callbacks (BillboardTemplate.callback, QuadFx), declared without a prototype: each takes its
 * own particle type. */
static void (*gParticleCallbacks[])() = {
    particle_updateFallingParticle, effect_updateRainDrop, effect_billboardDelayFade, NULL,
    NULL, particle_updateWakePuff, swarm_updateMember, particle_updateDropToSplash,
    NULL, particle_updateGrowingBillboard, func_150AECCC, effect_growBillboard,
    effect_shrinkBillboard, func_150E4CBC, effect_updateRainSplash, effect_updateFallingObject,
    particle_updateGatherParticle, particle_updateHoverParticle, particle_updateSwellParticle, particle_updateSwellTint,
    func_151A77C0, func_151A787C,
};

/* Object callbacks (SpriteFx and ModelFx callback, BillboardTemplate onDeath, Sprite2D behavior),
 * declared without a prototype: each takes its own object type. */
void (*gObjectCallbacks[])() = {
    NULL, NULL, particle_updateBounceDebris, particle_updateSplashFx,
    func_150B6DFC, func_150B6E3C, sprite_updateGrowIn, sprite_updateLinearMove,
    func_150B709C, beam_updateGrowSprite, beam_updateSpriteFade,
};

/* The texture table: the effects' animIndex / texture indices select an entry. */
TextureDesc *gEffectAnimTextures[] = {
    &D_8009053C, &D_80090388, &D_800904FC, &D_8009052C,
    &D_800903C8, &D_800903D4, &D_8009066C, &D_80090660,
    &D_80090678, &D_80090684, &D_8009053C, &D_80090600,
    &D_80090634, &D_8009053C, NULL, &D_8009080C,
    &D_800907FC, &D_80090690, &D_800906C4, &D_800906F8,
    &D_800905C0, &D_800905A8, &D_800905B4, &D_800905CC,
    &D_800905D8, &D_800905E4, &D_800905F0, &D_80090420,
    NULL, NULL, NULL, NULL,
    &D_8009042C, &D_80090438, &D_80090444, &D_80090450,
    &D_80090494, &D_800904C0, &D_800904B4, &D_800904CC,
    &D_80090730, &D_8009075C, &D_800904D8, &D_800904E4,
    &D_800904F0, &D_80090508, &D_8009077C, &D_80090514,
    &D_8009078C, &D_80090798, &D_80090704, &D_80090710,
    &D_80090520, &D_800907EC, &D_80091834, &D_80091864,
    &D_80091870, &D_8009187C, &D_80091888, &D_80091894,
    &D_800918A0, &D_800918AC, &D_800918B8, &D_800918C4,
    &D_800918D0, &D_8009184C, &D_80091858, &D_8009045C,
    &D_80090468, &D_80090474,
};

/* Per-frame hooks run by effect_runFrameHooks. */
static void (*gObjectFrameHooks[])(void) = {
    camera_clearOffsets, sprite_resetActorTrackers, swarm_doNothing,
};

/* Calls every object type's per-frame global hook (gObjectTypes[i].unk18). */
void effect_runTypeGlobalHooks(void) {
    ObjType *entry;
    ObjType *end;
    void (*func)(void);
    s32 unused;

    entry = gObjectTypes;
    end = (ObjType *)((u32)entry + 101 * sizeof(ObjType));
    unused = 0;
    // matching: a goto loop plus the dead `unused` (saved s1) give the target's base-relative end
    // and sltu exit; for/while/do forms fold `end` to a constant address
loop:
    func = entry->unk18;
    if (func != NULL) {
        func();
    }
    entry++;
    unused ^= 0;
    if ((u32)entry < (u32)end) {
        goto loop;
    }
}

/* Calls each of the three hooks in gObjectFrameHooks that is set. */
void effect_runFrameHooks(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (gObjectFrameHooks[i] != NULL) {
            gObjectFrameHooks[i]();
        }
    }
}

/* Runs every type's update callback on each object of both lists. */
void effect_updateObjects(void) {
    s32 list;
    s32 type;
    EffectHeader *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (gObjectTypes[type].update != NULL) {
                node = gObjectLists[list][type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    gObjectTypes[type].update(node);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            }
        }
    }
}

/* Runs every type's preUpdate callback on each object of both lists. */
void effect_preUpdateObjects(void) {
    s32 list;
    s32 type;
    EffectHeader *node;

    for (list = 0; list != 2; list++) {
        for (type = 0; type != 101; type++) {
            if (gObjectTypes[type].preUpdate != NULL) {
                node = gObjectLists[list][type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    gObjectTypes[type].preUpdate(node);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            }
        }
    }
}


/* Caches sin/cos of each player's unk3A0 angle, then runs every type's preDraw callback on its objects. */
void effect_preDrawObjects(void) {
    s32 player;
    f32 angle;

    for (player = 0; player < gScreenLayout + 1; player++) {
        angle = camera_global_Cameras[player].TrueCameraAngleRad;
        gCameraAngleSines[player] = sinf(angle);
        gCameraAngleCosines[player] = cosf(angle);
    }
    angle = camera_global_Cameras->TrueCameraAngleRad;
    gMainCameraAngleSinCos[0] = sinf(angle);
    gMainCameraAngleSinCos[1] = cosf(angle);
    {
        s32 list;
        s32 type;
        EffectHeader *node;

        for (type = 0; type != OBJ_TYPE_COUNT; type++) {
            for (list = 0; list != 2; list++) {
                if (gObjectLists[list][type] != NULL && gObjectTypes[type].preDraw != NULL) {
                    node = gObjectLists[list][type];
                    gObjectWalkDepth++;
                    while (node != NULL) {
                        gObjectWalkNext[gObjectWalkDepth] = node->next;
                        gObjectTypes[type].preDraw(node);
                        node = gObjectWalkNext[gObjectWalkDepth];
                    }
                    gObjectWalkDepth--;
                }
            }
        }
    }
}


/*
 * Draws every object of list `list` whose type is in group `group` (see effect_getDrawGroupTypes). Draw mode 1 types
 * drop any object that overflows the display list buffer; on overflow the pass is abandoned and the
 * display list is rewound to the start of the type that overflowed.
 */
Gfx *effect_drawObjectGroup(Gfx *gfx, s32 group, s16 arg2, s32 list) {
    s32 type;
    s32 first;
    s32 last;
    EffectHeader **head;
    EffectHeader *node;
    s32 overflowed;
    Gfx *mode0Start;
    Gfx *prev;
    Gfx *mode1Start;
    s32 full;

    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    effect_getDrawGroupTypes(group, &first, &last);
    for (type = first; type < last; type++) {
        if (gObjectTypes[type].draw != NULL) {
            head = &gObjectLists[list][type];
            if (*head != NULL) {
                D_8003C8E0 = (type & 0xFFFFFF) | 0x7000000;
                overflowed = FALSE;
                if (gObjectTypes[type].drawMode == 0) {
                    mode0Start = gfx;
                }
                if (gObjectTypes[type].drawMode == 1) {
                    mode1Start = gfx;
                }
                D_800D2DAB = 0;
                gDrawHookTexture = NULL;
                gDrawHookTextureFrame = 0;
                gDrawHookTextureCm = 2;
                *(u32 *)&gCachedTextureId = -1; // matching: an unsigned store keeps the -1 in its own register
                gCachedTexture = -1;
                gCachedTextureFrame = -1;
                gCachedRenderModeKey = 0;
                gCachedCombineKey = 0;
                gCachedPrimR = gCachedPrimG = gCachedPrimB = gCachedPrimA = -1;
                gCachedEnvR = gCachedEnvG = gCachedEnvB = gCachedEnvA = -1;
                D_800DD1F8 = 0;
                gCachedGeomSetBits = 0;
                gCachedGeomClearBits = 0;
                gCachedPrimMinLevel = -1;
                gCachedPrimLodFrac = -1;
                gCachedTextureCm = -1;
                gCachedTextureS = -1;
                gCachedTextureT = -1;
                gCachedTextureSprite = NULL;
                gCachedOtherModeH = 0;
                gCachedOtherModeL = 0;
                if (gObjectTypes[type].drawBeginDL != NULL) {
                    gSPDisplayList(gfx++, gObjectTypes[type].drawBeginDL);
                }
                if (gObjectTypes[type].drawBegin != NULL) {
                    gfx = gObjectTypes[type].drawBegin(gfx, type, list, arg2);
                }
                gSPClearGeometryMode(gfx++, G_LIGHTING);
                node = *head;
                if (gObjectTypes[type].drawMode == 1) {
                    gObjectWalkDepth++;
                    while (node != NULL) {
                        prev = gfx;
                        gObjectWalkNext[gObjectWalkDepth] = node->next;
                        gfx = gObjectTypes[type].draw(gfx, node, arg2);
                        full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                        if (full) {
                            overflowed = TRUE;
                            gfx = prev;
                        }
                        node = gObjectWalkNext[gObjectWalkDepth];
                    }
                    gObjectWalkDepth--;
                } else {
                    gObjectWalkDepth++;
                    while (node != NULL) {
                        gObjectWalkNext[gObjectWalkDepth] = node->next;
                        gfx = gObjectTypes[type].draw(gfx, node, arg2);
                        node = gObjectWalkNext[gObjectWalkDepth];
                    }
                    gObjectWalkDepth--;
                }
                if (gObjectTypes[type].drawEndDL != NULL) {
                    gSPDisplayList(gfx++, gObjectTypes[type].drawEndDL);
                }
                if (gObjectTypes[type].drawEnd != NULL) {
                    gfx = gObjectTypes[type].drawEnd(gfx, type, arg2);
                }
                if (overflowed) {
                    full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode1Start;
                    }
                }
                if (gObjectTypes[type].drawMode == 0) {
                    full = ((gfx - gDisplayListBuffers[D_800BE9C0]) > D_800BEBA4) ? TRUE : FALSE;
                    if (full) {
                        gSPSetGeometryMode(gfx, G_ZBUFFER);
                        return mode0Start;
                    }
                }
                D_8003C8E0 = 0;
            }
        }
    }
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
    return gfx;
}

/* Allocates a `size`-byte object (heap_alloc) and links it into gObjectLists[list][type]. */
void *effect_newObject(type, list, size, arg3, owner, arg5)
    s32 type;
    s32 list;
    s32 size;
    s32 arg3;
    u8 owner;
    u8 arg5;
{
    EffectHeader *obj;

    obj = (EffectHeader *)heap_alloc(size, 1, arg3, 0, arg5);
    if (obj != NULL) {
        obj->kind = list;
        effect_linkObject(obj, type);
        obj->owner = owner;
    }
    return obj;
}

/* Spawns a type 3 SpriteFx from the 0x18-byte template at `src` (anim .. fadeStep), fully opaque. */
void effect_spawnSpriteFx(SpriteFxTemplate *src, u8 owner, s32 list) {
    SpriteFx *fx;

    fx = effect_newObject(3, list, sizeof(SpriteFx), 0, owner, 1);
    if (fx != NULL) {
        bcopy(src, &fx->anim, 0x18);
        fx->alpha = 0xFF;
    }
}

/* Type 3 update: advances the animation and fade; destroys the sprite after its last frame. */
void effect_updateSpriteFx(SpriteFx *fx) {
    s32 index;
    s32 fade;
    s32 rise;
    s32 frame;

    index = fx->callback;
    if (index != 0) {
        gObjectCallbacks[index](&fx->link);
    }

    fade = fx->fadeStep;
    if (fade > 0) {
        index = fx->alpha;
        if (fade < index) {
            fx->alpha = index - fade;
        } else {
            fx->frame = fx->anim->frameCount << 8;
        }
    } else if (fade < 0) {
        rise = -fade;
        frame = fx->frame;
        if (frame / 256 >= fx->anim->frameCount - 1) {
            index = fx->alpha;
            if (rise < index) {
                fx->alpha = index - rise;
                fx->frame = frame - fx->frameStep;
            }
        }
    }

    fx->frame += fx->frameStep;
    if (fx->frame / 256 >= fx->anim->frameCount) {
        effect_destroyObject(&fx->link);
    }
}
typedef struct {
    s32 unk0;
    s32 unk4;
} RenderModeEntry;
extern RenderModeEntry gEffectRenderModes[];
typedef struct {
    s32 unk0;
    s32 unk4;
} PaddedFlag;
/* Type 3 draw: the current animation frame as a billboard. */
Gfx *effect_drawSpriteFx(Gfx *gdl, SpriteFx *fx, u8 arg2) {
    SpriteParams params;
    Gfx *gfx;
    PaddedFlag update; // matching: 8-byte stack object; only the first word (the update flag) is used
    TextureDesc *anim;
    s32 renderMode;
    s16 frame;

    if (arg2) {} // matching: references the otherwise unused arg2
    update.unk0 = 1;
    frame = fx->frame;
    anim = fx->anim;
    gfx = math_gfxLoadTextureFrame(gdl, anim, frame << 8, 2, 0x100, 0x100,
                        ((s32 *)anim->addr)[frame >> 8], 5, &params, (u8 *)&update, 3);
    params.x = fx->x;
    params.y = fx->y;
    params.z = fx->z;
    params.scaleX = fx->scaleX;
    params.scaleY = fx->scaleY;
    params.alpha = fx->alpha;
    params.flip = 0;
    // matching: fadeStep is an s8, so it never equals 0xFF; the target still tests both ways
    if (fx->fadeStep != 0xFF) {
        renderMode = 3;
    } else if (fx->fadeStep == 0xFF) {
        renderMode = 9;
    }
    gfx = math_gfxSetOtherMode(gfx, 0x2C00, gEffectRenderModes[renderMode].unk0 | gEffectRenderModes[renderMode].unk4 | 4, (u8 *)&update);
    return sprite_drawWorld(gfx, &params);
}
/* Spawns a particle (type 0x42 when `altType` is set, else 5) from the 0x38-byte template at `src`. */
BillboardParticle *effect_spawnBillboard(src, altType, extra, onDeath, owner, list)
    BillboardTemplate *src;
    s32 altType;
    s32 extra;
    s8 onDeath;
    u8 owner;
    s32 list;
{
    BillboardParticle *particle;

    particle = effect_newObject((altType == 0) ? 5 : 0x42, list, extra + 0x50, 0, owner, 1);
    if (particle == NULL) {
        return particle;
    }
    bcopy(src, &particle->tmpl, sizeof(BillboardTemplate));
    particle->onDeath = onDeath;
    return particle;
}
/* BillboardParticle update: life, fade, motion and animation; runs onDeath and destroys it when finished. */
void effect_updateBillboard(BillboardParticle *p) {
    void (*func)(EffectHeader *);
    u8 finished;
    s32 limit;

    finished = 0;
    if (p->tmpl.life != 0x7FFF) {
        p->tmpl.life -= D_800BE9E4;
        if (p->tmpl.life < 0) {
            p->tmpl.life = 0;
            finished = 1;
        }
    }

    if (p->tmpl.fadeStep > 0) {
        if (p->tmpl.fadeStep < p->tmpl.primA) {
            p->tmpl.primA -= p->tmpl.fadeStep;
        } else {
            p->tmpl.life = 0;
            finished = 1;
        }
    }

    if (p->tmpl.callback != -1) {
        func = gParticleCallbacks[p->tmpl.callback];
        if (func != NULL) {
            func(&p->link);
        }
    }

    if (p->tmpl.life != 0) {
        s32 posY;
        s32 posX;
        s32 posZ;
        s32 delta;

        p->tmpl.velY += p->tmpl.accelY;

        // matching: 24.8 fixed point moves written step by step (incl. the redundant (u8)) for IDO's temp registers
        posY = p->tmpl.y;
        posY = (posY << 8) | p->tmpl.subY;
        delta = p->tmpl.velY * D_800BE9E4;
        posY += delta;
        p->tmpl.y = posY >> 8;
        posY = (u8)posY;
        p->tmpl.subY = posY;
        posX = p->tmpl.x;
        posX = (posX << 8) | p->tmpl.subX;
        delta = p->tmpl.velX * D_800BE9E4;
        posX += delta;
        p->tmpl.x = posX >> 8;
        posX = (u8)posX;
        p->tmpl.subX = posX;
        posZ = p->tmpl.z;
        posZ = (posZ << 8) | p->tmpl.subZ;
        delta = p->tmpl.velZ * D_800BE9E4;
        posZ += delta;
        p->tmpl.z = posZ >> 8;
        posZ = (u8)posZ;
        p->tmpl.subZ = posZ;
        if (p->tmpl.frameStep != 0) {
            limit = (p->tmpl.anim->frameCount << 8) - 1;
            p->tmpl.frame += p->tmpl.frameStep * D_800BE9E4;
            if (limit < p->tmpl.frame) {
                if (p->tmpl.flags & BILLBOARD_DIE_AT_END) {
                    p->tmpl.life = 0;
                    finished = 1;
                } else {
                    p->tmpl.flags |= BILLBOARD_LOOPED;
                    if (p->tmpl.flags & BILLBOARD_PINGPONG) {
                        p->tmpl.frame = limit - (p->tmpl.frame % limit);
                        p->tmpl.frameStep = -p->tmpl.frameStep;
                    } else {
                        do {
                            p->tmpl.frame -= limit;
                        } while (limit < p->tmpl.frame);
                    }
                }
            } else if (p->tmpl.frame < 0) {
                if (p->tmpl.flags & BILLBOARD_DIE_AT_START) {
                    p->tmpl.life = 0;
                    finished = 1;
                } else if (p->tmpl.flags & BILLBOARD_PINGPONG) {
                    p->tmpl.frame = -p->tmpl.frame % limit;
                    p->tmpl.frameStep = -p->tmpl.frameStep;
                } else {
                    do {
                        p->tmpl.frame += limit;
                    } while (p->tmpl.frame < 0);
                }
            }
        }
    } else {
        finished = 1;
    }

    if (finished != 0) {
        if (p->onDeath != -1) {
            gObjectCallbacks[p->onDeath](&p->link);
        }
        effect_destroyObject(&p->link);
    }
}

/* BillboardParticle draw: sets render/combine modes and colours, then the current frame as a billboard. */
Gfx *effect_drawBillboard(Gfx *gfx, BillboardParticle *p, s16 viewIndex) {
    SpriteParams params;
    s32 view;
    s32 pad1; // matching: stack layout of the target frame
    u8 update;
    s32 pad2[3]; // matching: stack layout of the target frame
    f32 angle;
    s32 size;
    s32 unused; // matching: an unused local the target allocates
    s32 c0, c1, c2; /* colour components, reused for prim and env */
    TextureDesc *anim;
    TextureDesc *white;

    params.flip = 0;
    if (p->tmpl.flags & BILLBOARD_FLIP_S) {
        params.flip = 1;
    }
    if (p->tmpl.flags & BILLBOARD_FLIP_T) {
        params.flip |= 2;
    }
    // matching: two empty tests of the same condition steer IDO's scheduling of the texture load
    if (p->tmpl.anim->addr < 0x10000000) {
    }
    if (p->tmpl.anim->addr < 0x10000000) {
    }
    update = 1;
    {
        s32 texture;

        anim = p->tmpl.anim;
        if (anim->addr < 0x10000000) {
            texture = anim->addr;
        } else {
            texture = ((s32 *)anim->addr)[p->tmpl.frame >> 8];
        }
        gfx = math_gfxLoadTextureFrame(gfx, anim, p->tmpl.frame << 8, 2, 0x100, 0x100, texture, 5, &params, &update, 3);
    }
    view = viewIndex;
    params.x = p->tmpl.x;
    params.y = p->tmpl.y;
    params.z = p->tmpl.z;
    size = p->tmpl.width;
    if (size < 0) {
        /* 0.0111 = 1/90: fold the view angle into 0..90 degrees */
        angle = fabsf(camera_global_Cameras[view].TrueCameraAngle + (p->tmpl.unk8 & 0xFF) * 1.40625f);
        while (angle >= 360.0f) {
            angle -= 360.0f;
        }
        if (angle >= 180.0f) {
            angle -= 180.0f;
        }
        if (angle >= 90.0f) {
            angle = 180.0f - angle;
        }
        size = size * angle * -0.01111111138f;
        if (size < 0x400) {
            size = 0x400;
        }
    }
    params.scaleX = size;
    size = p->tmpl.height;
    if (size < 0) {
        view = (p->tmpl.width < 0) ? 0 : p->tmpl.width;
        size = fabsf(camera_global_Cameras[viewIndex].CameraPitch) * (view + size) * 0.01111111138f - size;
    }
    params.scaleY = size;
    if (p->tmpl.flags & BILLBOARD_CUSTOM_MODE) {
        u8 renderMode;
        u8 combineMode;
        s32 pad3[2]; // matching: stack layout of the target frame

        renderMode = p->tmpl.unk4 & 0xFF;
        combineMode = (p->tmpl.unk4 & 0xFFFF0000) >> 16;
        gfx = math_gfxUpdateGeometryMode(gfx, 5, 0);
        gfx = particle_setCombineMode(gfx, combineMode, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | ((p->tmpl.flags & BILLBOARD_2CYCLE) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[renderMode].unk0 | gEffectRenderModes[renderMode].unk4 | 4, &update);
    } else if (p->tmpl.anim->fmt != 5) {
        gfx = particle_setCombineMode(gfx, 0xD, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[3].unk0 | gEffectRenderModes[3].unk4 | 4, &update);
    } else if (p->tmpl.anim->fmt == 3) {
        gfx = particle_setCombineMode(gfx, 0xE, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[3].unk0 | gEffectRenderModes[3].unk4 | 4, &update);
    } else if (p->tmpl.flags & 0x10) {
        gfx = particle_setCombineMode(gfx, 0xD, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[0x12].unk0 | gEffectRenderModes[0x12].unk4 | 4, &update);
    } else {
        gfx = particle_setCombineMode(gfx, 0xF, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_2CYCLE | G_TP_NONE | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[0x11].unk0 | gEffectRenderModes[0x11].unk4 | 4, &update);
    }
    c0 = p->tmpl.primG;
    c1 = p->tmpl.primB;
    c2 = p->tmpl.primA;
    gfx = math_gfxSetPrimColor(gfx, 1, 0, p->tmpl.primR, c0, c1, c2, &update);
    if (p->tmpl.flags & BILLBOARD_ENV_COLOR) {
        c0 = p->tmpl.envR;
        c1 = p->tmpl.envG;
        c2 = p->tmpl.envB;
    } else {
        anim = p->tmpl.anim;
        white = &D_8009053C;
        if (p->tmpl.anim == &D_800907FC) {
            c0 = 0x80;
            c1 = 0x80;
            c2 = 0xFF;
        } else if (white == anim) {
            c0 = 0xFF;
            c1 = 0xFF;
            c2 = 0xFF;
        } else {
            c0 = 0;
            c1 = 0;
            c2 = 0;
        }
    }
    gfx = math_gfxSetEnvColor(gfx, c0, c1, c2, 0, &update);
    return sprite_drawWorld(gfx, &params);
}

/* Fills a QuadFxData template's vertex colours and texture coordinates from its animation. */
void effect_initQuadFxTemplate(QuadFxData *q, u8 r, u8 g, u8 b, u8 a) {
    s16 tB;
    s16 tA;
    s16 sB;
    s16 sA;
    s32 i;

    tA = 0x2000;
    tB = (gEffectAnimTextures[q->animIndex]->height + 0x100) << 5;
    sA = 0x2000;
    sB = (gEffectAnimTextures[q->animIndex]->width + 0x100) << 5;
    for (i = 0; i < 2; i++) {
        q->vtx[i * 4 + 1].v.flag = 0;
        q->vtx[i * 4 + 1].v.cn[0] = r;
        q->vtx[i * 4 + 1].v.cn[1] = g;
        q->vtx[i * 4 + 1].v.cn[2] = b;
        q->vtx[i * 4 + 1].v.cn[3] = a;
        q->vtx[i * 4 + 2].v.flag = 0;
        q->vtx[i * 4 + 2].v.cn[0] = r;
        q->vtx[i * 4 + 2].v.cn[1] = g;
        q->vtx[i * 4 + 2].v.cn[2] = b;
        q->vtx[i * 4 + 2].v.cn[3] = a;
        q->vtx[i * 4 + 3].v.flag = 0;
        q->vtx[i * 4 + 3].v.cn[0] = r;
        q->vtx[i * 4 + 3].v.cn[1] = g;
        q->vtx[i * 4 + 3].v.cn[2] = b;
        q->vtx[i * 4 + 3].v.cn[3] = a;
        q->vtx[i * 4 + 0].v.flag = 0;
        q->vtx[i * 4 + 0].v.cn[0] = r;
        q->vtx[i * 4 + 0].v.cn[1] = g;
        q->vtx[i * 4 + 0].v.cn[2] = b;
        q->vtx[i * 4 + 0].v.cn[3] = a;
    }
    if (q->flags & 0x80) {
        tB = 0x2000;
        tA = (gEffectAnimTextures[q->animIndex]->height + 0x100) << 5;
    }
    if (q->flags & 0x100) {
        sB = 0x2000;
        sA = (gEffectAnimTextures[q->animIndex]->width + 0x100) << 5;
    }
    q->vtx[1].v.tc[0] = q->vtx[5].v.tc[0] = q->vtx[2].v.tc[0] = q->vtx[6].v.tc[0] = sA;
    q->vtx[2].v.tc[1] = q->vtx[6].v.tc[1] = q->vtx[3].v.tc[1] = q->vtx[7].v.tc[1] = tA;
    q->vtx[0].v.tc[0] = q->vtx[4].v.tc[0] = q->vtx[3].v.tc[0] = q->vtx[7].v.tc[0] = sB;
    q->vtx[0].v.tc[1] = q->vtx[4].v.tc[1] = q->vtx[1].v.tc[1] = q->vtx[5].v.tc[1] = tB;
    q->frame = 0;
    q->unk9C = 0;
    q->heading = 0;
}

/* Spawns a type 0xE QuadFx from a template prepared with effect_initQuadFxTemplate. */
QuadFx *effect_spawnQuadFx(src, owner, list)
    QuadFxData *src;
    u8 owner;
    s32 list;
{
    QuadFx *fx;

    fx = effect_newObject(0xE, list, sizeof(QuadFx), 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    bcopy(src, &fx->data, sizeof(QuadFxData));
    return fx;
}

/* Type 0xE update: advances the animation (wrapping, ping-ponging or finishing) and runs its callback. */
void effect_updateQuadFx(QuadFx *fx) {
    s32 frame;
    s32 limit;
    u8 frameCount;
    s16 step;
    void (*func)(EffectHeader *);

    step = fx->data.frameStep;
    if (step != 0) {
        frameCount = gEffectAnimTextures[fx->data.animIndex]->frameCount;
        limit = (frameCount << 8) - 1;
        fx->data.frame += step * D_800BE9E4;
        frame = fx->data.frame;
        if (limit < frame) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = limit - (frame % limit);
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame - limit;
                    frame = fx->data.frame;
                } while (limit < frame);
            }
        } else if (frame < 0) {
            if (fx->data.flags & 0x40) {
                fx->data.frame = -frame % limit;
                fx->data.frameStep = -step;
            } else if (fx->data.flags & 4) {
                fx->data.frame = -1;
            } else {
                do {
                    fx->data.frame = frame + limit;
                    frame = fx->data.frame;
                } while (frame < 0);
            }
        }
    }

    frame = fx->data.callback;
    if (frame != -1) {
        func = gParticleCallbacks[frame];
        if (func != NULL) {
            func(&fx->link);
        }
    }

    if (fx->data.frame == -1) {
        effect_destroyObject(&fx->link);
    }
}
/* Moves an object to type 0. */
void effect_updatePendingFree(EffectHeader *obj) {
    effect_setObjectType(obj, 0);
}
/* Pushes obj onto the front of list gObjectLists[obj->kind][type] and sets its type. */
void effect_linkObject(EffectHeader *obj, s32 type) {
    s32 list;
    EffectHeader **head;

    list = obj->kind;
    head = (EffectHeader **)&gObjectLists[list][type];
    obj->next = *head;
    if (obj->next != NULL) {
        obj->next->prev = obj;
    }
    obj->type = type;
    obj->prev = NULL;
    *head = obj;
}

/* Unlinks obj from its current list. */
void effect_unlinkObject(EffectHeader *obj) {
    s32 list = obj->kind;
    s32 type = obj->type;
    EffectHeader **head = (EffectHeader **)&gObjectLists[list][type];

    if (obj == *head) {
        *head = obj->next;
    }
    if (obj->next != NULL) {
        obj->next->prev = obj->prev;
    }
    if (obj->prev != NULL) {
        obj->prev->next = obj->next;
    }
}

/* Moves obj to the list for another type. */
void effect_setObjectType(EffectHeader *obj, s32 type) {
    effect_unlinkObject(obj);
    effect_linkObject(obj, type);
}

/*
 * BillboardParticle callback: tmpl.unk4 holds a fade rate (high half) and a delay (low half). Keeps the particle alive
 * (life = 30) while the delay counts down, then fades primA by the rate each call and lets it die.
 * Readable equivalent:
 *     word = p->tmpl.unk4;
 *     if (word & 0xFFFF) {
 *         p->tmpl.unk4 &= 0xFFFF0000; p->tmpl.life = 30; p->tmpl.unk4 |= (u16)((word & 0xFFFF) - 1); return;
 *     }
 *     if ((u16)(word >> 16) < p->tmpl.primA) { p->tmpl.primA -= (u16)(word >> 16); p->tmpl.life = 30; } else { p->tmpl.life = 0; }
 */
// matching: permuter output; every readable form tried allocates `word` and the delay in other registers

void effect_billboardDelayFade(void *arg0)
{
  BillboardParticle *a;
  s32 v1;
  s32 new_var2;
  u16 t6;
  u16 hi;
  u8 a2;
  s32 new_var;
  s32 c0;
  s32 z0;
  s32 z1;
  s32 z2;
  s32 t9;
  s32 z3;
  s32 z4;
  s32 z5;
  s32 keep;
  a = arg0;
  hi = 0x1E;
  v1 = a->tmpl.unk4;
  new_var2 = v1;
  t6 = ((u16) new_var2) ^ 0;
  new_var = t6 - (1 & 0xFFFFFFFFu);
  if (t6 != 0)
  {
    c0 = v1;
    z0 = 0;
    z1 = 0;
    z2 = 0;
    z3 = 0;
    z4 = 0;
    z5 = 0;
    keep = new_var & 0xFFFF;
    new_var = keep * 0;
    if (a)
    {
    }
    hi++;
    hi--;
    t9 = (v1 & 0xFFFF0000) | new_var;
    v1 = 0;
    a->tmpl.unk4 = t9;
    a->tmpl.life = hi;
    if (keep)
    {
      ;
    }
    *((s32 *) (((s8 *) arg0) + 0x14)) = ((t9 | keep) | (((((((c0 * 0) | (z0 * 0)) | (z1 * 0)) | (z2 * 0)) | (z3 * 0)) | (z4 * 0)) | (z4 = z5 * 0))) & 0xFFFFFFFFu;
    return;
  }
  hi = (u16) (v1 >> 16);
  v1 = a->tmpl.primA;
  if (0)
  {
  }
  a2 = v1;
  if (hi < a2)
  {
    a->tmpl.primA = a2 - hi;
    if (1)
    {
    }
    a->tmpl.life = 0x1E;
  }
  else
  {
    a2 = 0;
    a->tmpl.life = (a2, 0);
  }
}
/* Type 0x10 update: runs the object's callback, if any. */
void effect_updateModelFx(ModelFx *fx) {
    u8 index;

    index = fx->data.callback;
    if (index != 0) {
        gObjectCallbacks[index](&fx->link);
    }
}

/* Spawns a type 0x10 ModelFx from the template at `src`, if it has a display list. */
void effect_spawnModelFx(src, owner, list)
    ModelFxData *src;
    u8 owner;
    s32 list;
{
    ModelFx *fx;

    if (src->displayList != NULL) {
        fx = effect_newObject(0x10, list, sizeof(ModelFx), 1, owner, 1);
        if (fx != NULL) {
            bcopy(src, &fx->data, sizeof(ModelFxData));
        }
    }
}

/* Type 0x10 draw: builds this frame's matrix, sets fade/tile/render state and calls the display list. */
Gfx *effect_drawModelFx(Gfx *gfx, ModelFx *fx, s32 arg2) {
    s32 pad; // matching: stack layout of the target frame
    s32 renderMode;
    u8 update;

    math_mtxRotScaleTrans(&fx->mtx[D_800BE9C0], fx->data.rotation[0], fx->data.rotation[1], fx->data.rotation[2],
                  fx->data.scale[0], fx->data.scale[1], fx->data.scale[2], fx->data.position[0], fx->data.position[1], fx->data.position[2]);

    gDPPipeSync(gfx++);
    gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, fx->data.primA);
    /* gCachedPrimR..C6: cached prim colour (reset to -1 per type by effect_drawObjectGroup) */
    gCachedPrimA = fx->data.primA;
    gCachedPrimR = gCachedPrimG = gCachedPrimB = 0;
    gDPSetEnvColor(gfx++, 0, 0, 0, 0);
    gDPSetTileSize(gfx++, 0, 2, fx->data.tileUlt, 0x7E, 0xFE);

    switch (fx->data.renderMode) {
    case 1:
        renderMode = 0x552230;
        break;
    case 2:
        renderMode = 0x504A50;
        break;
    }

    update = 0;
    gfx = math_gfxSetOtherMode(gfx, 0x8ACA0, renderMode, &update);

    if (fx->data.combineMode == 1) {
        gDPSetCombine(gfx++, 0x123824, 0xFF73FFFF);
    }

    gSPMatrix(gfx++, &fx->mtx[D_800BE9C0], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, fx->data.displayList);

    return gfx;
}

/* Adds `offset` to an address unless it is segmented. */
void effect_relocateAddress(s32 *addr, s32 offset) {
    s32 value = *addr;

    if ((value & 0x0F000000) == 0) {
        *addr = value + offset;
    }
}

/* One display list command seen as bytes: opcode, two parameter bytes, a third (G_MOVEMEM index), w1. */
struct DLCommand {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ s8 unk1;
    /* 0x2 */ s8 unk2;
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
};

#define IS_RELOCATABLE(cmd) \
    ((cmd)->opcode == G_VTX || ((cmd)->opcode == (s8)G_MOVEMEM && (cmd)->index == G_MV_MATRIX))

/* Relocates the vertex and matrix addresses of a display list by `offset` (segmented ones are left alone). */
void effect_relocateDisplayList(Gfx *dl, s32 offset) {
    s32 i;
    DLCommand *cmd;

    // matching: indexing the Gfx array (not a DLCommand array) gives the target's saved-register use
    for (i = 0; ((DLCommand *)&dl[i])->opcode != (s8)G_ENDDL; i++) {
        cmd = (DLCommand *)&dl[i];
        if (IS_RELOCATABLE(cmd)) {
            effect_relocateAddress((s32 *)&cmd->addr, offset);
        }
    }
}

/* Rebases the vertex and matrix addresses of a display list: drops their segment and adds `base`. */
void effect_rebaseDisplayList(DLCommand *dl, s32 base) {
    s32 i;
    DLCommand *cmd;
    s8 opcode;

    // matching: the early-out, the dead `opcode` loads and the do-while(0) reproduce the target's
    // double opcode load and register use; plain for/while loops do not
    i = 0;
    cmd = dl;
    if (dl->opcode == (s8)G_ENDDL) {
        return;
    }
    cmd = &dl[i];
    opcode = cmd->opcode;
    do {
        i++;
        if (IS_RELOCATABLE(cmd)) {
            cmd->addr = cmd->addr & 0xFFFFFF;
            cmd->addr += base;
        }
        cmd = &dl[i];
        do {
            opcode = cmd->opcode;
        } while (0);
    } while (cmd->opcode != (s8)G_ENDDL);
}
/* Gives the range [*first, *last) of object types drawn together as draw group `group`. */
void effect_getDrawGroupTypes(s32 group, s32 *first, s32 *last) {
    if (group == 0) {
        *first = 1;
        *last = 65;
    } else if (group == 1) {
        *first = 66;
        *last = 79;
    } else if (group == 2) {
        *first = 80;
        *last = 88;
    } else if (group == 3) {
        *first = 89;
        *last = 92;
    } else if (group == 5) {
        *first = 97;
        *last = 99;
    } else if (group == 6) {
        *first = 100;
        *last = 101;
    } else {
        *first = 93;
        *last = 96;
    }
}

/* Sends an event to every object of every type. */
void effect_broadcastEvent(s32 event, u8 eventType) {
    effect_sendEventToTypeRange(0, OBJ_TYPE_COUNT, event, eventType);
}

/* Sends an event to every object of types [first, last) (clamped by math_clampRange). */
void effect_sendEventToTypeRange(s32 first, s32 last, s32 event, u8 eventType) {
    s32 type;
    EffectHeader *node;

    math_clampRange(&first, &last, 2, OBJ_TYPE_COUNT);
    for (type = first; type < last; type++) {
        s32 list;
        for (list = 0; list != 2; list++) {
            if (gObjectTypes[type].handleEvent != NULL) {
                node = gObjectLists[list][type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    effect_handleOwnerEvent(node, (u8 *)event, eventType);
                    gObjectTypes[type].handleEvent(node, event, eventType);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            } else {
                node = gObjectLists[list][type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    effect_handleOwnerEvent(node, (u8 *)event, eventType);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            }
        }
    }
}

/* Sends an event to every object of the `count` types listed in `types`. */
void effect_sendEventToTypes(const s32 *types, s32 count, s32 event, u8 eventType) {
    const s32 *type;
    s32 i;
    s32 list;
    EffectHeader *node;

    for (i = 0, type = types; i < count; i++) {
        for (list = 0; list != 2; list++) {
            if (gObjectTypes[*type].handleEvent != NULL) {
                node = gObjectLists[list][*type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    effect_handleOwnerEvent(node, (u8 *)event, eventType);
                    gObjectTypes[*type].handleEvent(node, event, eventType);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            } else {
                node = gObjectLists[list][*type];
                gObjectWalkDepth++;
                while (node != NULL) {
                    gObjectWalkNext[gObjectWalkDepth] = node->next;
                    effect_handleOwnerEvent(node, (u8 *)event, eventType);
                    node = gObjectWalkNext[gObjectWalkDepth];
                }
                gObjectWalkDepth--;
            }
        }
        type++;
    }
}

/* Sends an event to every object of one type (clamped to the last type). */
void effect_sendEventToType(s32 type, s32 event, u8 eventType) {
    s32 list;
    EffectHeader *node;

    if (type >= OBJ_TYPE_COUNT) {
        type = OBJ_TYPE_COUNT - 1;
    }
    for (list = 0; list != 2; list++) {
        if (gObjectTypes[type].handleEvent != NULL) {
            node = gObjectLists[list][type];
            gObjectWalkDepth++;
            while (node != NULL) {
                gObjectWalkNext[gObjectWalkDepth] = node->next;
                effect_handleOwnerEvent(node, (u8 *)event, eventType);
                gObjectTypes[type].handleEvent(node, event, eventType);
                node = gObjectWalkNext[gObjectWalkDepth];
            }
            gObjectWalkDepth--;
        } else {
            node = gObjectLists[list][type];
            gObjectWalkDepth++;
            while (node != NULL) {
                gObjectWalkNext[gObjectWalkDepth] = node->next;
                effect_handleOwnerEvent(node, (u8 *)event, eventType);
                node = gObjectWalkNext[gObjectWalkDepth];
            }
            gObjectWalkDepth--;
        }
    }
}
/* Sends an actor event about `actor` to every object. */
void effect_broadcastActorEvent(Actor *actor, u8 eventType) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    effect_broadcastEvent((s32)&event, eventType);
}

/* Sends an actor event about `actor` to every object of one type. */
void effect_sendActorEventToType(s32 type, Actor *actor, u8 eventType) {
    ActorRef event;
    event.actor = actor;
    event.uniqueId = actor->unique_id;
    effect_sendEventToType(type, (s32)&event, eventType);
}

/* Sets D_800D2DAB and returns its first argument unchanged. */
Gfx *effect_beginBillboardDraw(Gfx *gfx, s32 type, s32 list, s16 view) {
    D_800D2DAB = 1;
    return gfx;
}

/* Default event handling: event types 0xF and 0x10 destroy objects owned by event[0]. */
void effect_handleOwnerEvent(EffectHeader *obj, u8 *event, u8 eventType) {
    if (eventType == 0xF || eventType == 0x10) {
        // matching: obj->owner read as a byte array; the struct field form swaps the two loads
        if (event[0] == ((u8 *)obj)[0xC]) {
            effect_destroyObject(obj);
        }
    }
}

/* Repairs saved iteration cursors that point at `obj` (it is about to leave its list). */
void effect_fixWalkCursors(EffectHeader *obj) {
    s8 depth;

    for (depth = 0; depth < gObjectWalkDepth; depth++) {
        if (obj == gObjectWalkNext[depth]) {
            gObjectWalkNext[depth] = obj->next;
        }
    }
}

/* Destroys an object of type 2 or above: its type's destroy hook, or by default effect_defaultDestroy. */
void effect_destroyObject(void *obj) {
    EffectHeader *head;
    void (*destroy)(EffectHeader *obj);

    effect_fixWalkCursors(obj);

    head = obj;
    if (head->type >= 2) {
        destroy = gObjectTypes[head->type].destroy;
        if (destroy != NULL) {
            destroy(obj);
            return;
        }
        effect_defaultDestroy(obj);
    }
}

/* Frees an object: its type's free hook, or by default effect_defaultFree. */
void effect_freeObject(void *obj) {
    EffectHeader *head;
    void (*free)(EffectHeader *obj);

    effect_fixWalkCursors(obj);
    head = obj;
    free = gObjectTypes[head->type].free;
    if (free != NULL) {
        free(obj);
        return;
    }
    effect_defaultFree(obj);
}

/* Default destroy: moves the object to type 1 (pending free). */
void effect_defaultDestroy(void *obj) {
    effect_setObjectType(obj, 1);
}

/* Default free: unlinks the object and releases its memory. */
void effect_defaultFree(void *obj) {
    effect_unlinkObject(obj);
    heap_free(obj);
}

/*
 * Shared actor-event handler for objects that remember an owner (`owner`, `ownerId`): event type 0 destroys
 * `obj` if the event names its owner; type 0x2D hands ownership over to the other actor of the swap.
 */
void effect_handleOwnerActorEvent(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    if (eventType == 0) {
        if (((ActorRef *)event)->actor == *owner || ((ActorRef *)event)->uniqueId == *ownerId) {
            effect_destroyObject(obj);
        }
    } else if (eventType == 0x2D) {
        Actor *actorA = ((ActorSwapMsg *)event)->actorA;

        if (actorA == *owner) {
            *owner = ((ActorSwapMsg *)event)->actorB;
            *ownerId = ((ActorSwapMsg *)event)->idB;
        } else if (((ActorSwapMsg *)event)->actorB == *owner) {
            *owner = actorA;
            *ownerId = ((ActorSwapMsg *)event)->idA;
        }
    }
}
