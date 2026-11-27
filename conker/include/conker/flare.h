#ifndef CONKER_FLARE_H
#define CONKER_FLARE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "flare": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Flare/FlareReset/flarereset.c
 *   game/Flare/Glow/glow.c
 *   game/Flare/ScreenFlash/screenflash.c
 *   game/Flare/VisProbe/visprobe.c
 *   game/Flare/LensFlare/lensflare.c
 */

/* A glow sprite (game/Flare/Glow/glow.c). */
typedef struct GlowObj {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x1B];
    /* 0x02B */ u8 unk2B;
    /* 0x02C */ u8 pad2C[8];
    /* 0x034 */ f32 pos[3];
    /* 0x040 */ f32 aim[3]; /* pos + 500 * facing direction */
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x63];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuf[4]; /* per view, double-buffered (particle_getViewBuffer) */
    /* 0x110 */ GlowParams params;
} GlowObj;

/* Caller data copied into a z-buffer probe (game/Flare/VisProbe/visprobe.c) and handed back to its callback. */
typedef struct ProbeData {
    s32 unk0[9];
} ProbeData;

/* game/Flare/FlareReset/flarereset.c */
void flare_resetFlares(s32 arg0);

/* game/Flare/Glow/glow.c */
extern s32 gGlowCount; /* live glow count */
/* Spawns a glow and copies paramSize bytes of params (a GlowParams, or a caller struct that starts with a
 * GlowShape) into it. Callers keep their own view of the glow object. */
void *flare_spawnGlow(void *params, s32 paramSize, ParticleDef *spawn, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s8 handler, u8 arg8, s32 arg9);
Vtx *flare_buildGlowQuad(GlowObj *obj, s16 view);
void flare_destroyGlowDefault(DefParticleHead *arg0);
void flare_freeGlowDefault(DefParticleHead *arg0);
void flare_destroyGlow(GlowObj *obj);
void flare_freeGlow(GlowObj *obj);
void flare_sampleGlowDepths(void);
void flare_moveGlowAttachment(GlowObj *obj);
s32 flare_glowFlicker(GlowObj *obj);
s32 flare_glowPulse(GlowObj *obj);
s32 flare_glowEnvelope(GlowObj *obj);
void flare_glowMessage(GlowObj *obj, u8 *msg, u8 msgType);
void flare_sendGlowEvent(u8 arg0, u8 arg1);
s32 flare_hideGlow(s32 arg0, s32 arg1);
void flare_placeGlowOnPivot(GlowObj *obj, f32 *pivot, f32 distance, f32 spread, f32 angleX, f32 angleZ);
s32 flare_glowOrbit(GlowObj *obj);

/* game/Flare/ScreenFlash/screenflash.c */
extern u8 gScreenFlash; /* screen flash level (0 for none), which the lens flares raise */
Gfx *flare_drawScreenFlash(Gfx *arg0);

/* game/Flare/VisProbe/visprobe.c */
Gfx *flare_copyProbePixels(Gfx *gfx, s32 x, s32 y, s32 unused, void *zDest, void *colorDest, s16 view);
Gfx *flare_runProbes(Gfx *gfx, s32 view);
s32 flare_queueProbe(f32 x, f32 y, f32 z, ProbeData *data, s8 callback, s32 arg5);

/* game/Flare/LensFlare/lensflare.c */
extern u8 D_800DDD61;
extern LensFlare *gLensFlares; /* head of the lens flare list */
extern f32 *gSunPositions[2]; /* per LevelHeader.sunPosition: sun position (FLARE_SUN) */
LensFlare *flare_add(s32 type, s32 alpha);
void flare_remove(LensFlare *flare);
void flare_sampleDepths(void);
Gfx *flare_drawAll(Gfx *gfx, s32 arg1);
Gfx *flare_draw(Gfx *gfx, LensFlare *flare, s32 sizeRow, s32 count, s32 falloff, s32 strength, u8 *flash);
Gfx *flare_setupRenderState(Gfx *gfx);
s32 flare_projectToScreen(f32 x, f32 y, f32 z, f32 *outX, f32 *outY);
s32 flare_getDepth(LensFlare *flare, s32 *outZbufDepth);

#endif
