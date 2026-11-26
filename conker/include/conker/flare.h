#ifndef CONKER_FLARE_H
#define CONKER_FLARE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "flare": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_36600.c
 *   game_16DC80.c
 *   game_1A0420.c
 *   game_1A11B0.c
 *   game_1AB530.c
 */

/* A glow sprite (game_16DC80.c). */
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
    /* 0x100 */ Vtx *vtxBuf[4]; /* per view, double-buffered (func_151D5D60) */
    /* 0x110 */ GlowParams params;
} GlowObj;

/* Caller data copied into a z-buffer probe (game_1A11B0.c) and handed back to its callback. */
typedef struct ProbeData {
    s32 unk0[9];
} ProbeData;

/* game/done/game_36600.c */
void func_15009150(s32 arg0);

/* game_16DC80.c */
/* Spawns a glow and copies paramSize bytes of params (a GlowParams, or a caller struct that starts with a
 * GlowShape) into it. Callers keep their own view of the glow object. */
void *func_151407D0(void *params, s32 paramSize, ParticleDef *spawn, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s8 handler, u8 arg8, s32 arg9);
Vtx *func_151408A4(GlowObj *obj, s16 view);
void func_151411A4(DefParticleHead *arg0);
void func_151411C4(DefParticleHead *arg0);
void func_151411E4(GlowObj *obj);
void func_15141250(GlowObj *obj);
void func_151412BC(void);
void func_1514143C(GlowObj *obj);
s32 func_15141478(GlowObj *obj);
s32 func_15141564(GlowObj *obj);
s32 func_151415D4(GlowObj *obj);
void func_151416E8(GlowObj *obj, u8 *msg, u8 msgType);
void func_151417C4(u8 arg0, u8 arg1);
s32 func_15141818(s32 arg0, s32 arg1);
void func_1514182C(GlowObj *obj, f32 *pivot, f32 distance, f32 spread, f32 angleX, f32 angleZ);
s32 func_15141928(GlowObj *obj);

/* game_1A0420.c */
Gfx *func_15172F70(Gfx *arg0);

/* game_1A11B0.c */
Gfx *func_15173D00(Gfx *gfx, s32 x, s32 y, s32 unused, s32 zDest, s32 colorDest, s16 view);
Gfx *func_151742EC(Gfx *gfx, s32 view);
s32 func_151745F0(f32 x, f32 y, f32 z, ProbeData *data, s8 callback, s32 arg5);

/* game_1AB530.c */
LensFlare *func_1517E080(s32 type, s32 alpha);
void func_1517E134(LensFlare *flare);
void func_1517E1AC(void);
Gfx *func_1517E28C(Gfx *gfx, s32 arg1);
Gfx *func_1517E4A8(Gfx *gfx, LensFlare *flare, s32 sizeRow, s32 count, s32 falloff, s32 strength, u8 *flash);
Gfx *func_1517EA4C(Gfx *gfx);
s32 func_1517EAAC(f32 x, f32 y, f32 z, f32 *outX, f32 *outY);
s32 func_1517EC1C(LensFlare *flare, s32 *outZbufDepth);

#endif
