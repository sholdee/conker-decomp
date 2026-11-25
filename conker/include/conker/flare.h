#ifndef CONKER_FLARE_H
#define CONKER_FLARE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "flare": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_36600.c
 *   game_16DC80.c
 *   game_1A0420.c
 *   game_1A11B0.c
 *   game_1AB530.c
 */

/* Defined in game_16DC80.c and game_1A11B0.c. A typedef here would reorder the .bss of game_6EA90.c. */
struct GlowObj;
struct ProbeData;

/* game/done/game_36600.c */
void func_15009150(s32 arg0);

/* game_16DC80.c */
/* Spawns a glow and copies paramSize bytes of params (a GlowParams, or a caller struct that starts with a
 * GlowShape) into it. Callers keep their own view of the glow object. */
void *func_151407D0(void *params, s32 paramSize, ParticleDef *spawn, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s8 handler, u8 arg8, s32 arg9);
Vtx *func_151408A4(struct GlowObj *obj, s16 view);
void func_151411A4(DefParticleHead *arg0);
void func_151411C4(DefParticleHead *arg0);
void func_151411E4(struct GlowObj *obj);
void func_15141250(struct GlowObj *obj);
void func_151412BC(void);
void func_1514143C(struct GlowObj *obj);
s32 func_15141478(struct GlowObj *obj);
s32 func_15141564(struct GlowObj *obj);
s32 func_151415D4(struct GlowObj *obj);
void func_151416E8(struct GlowObj *obj, u8 *msg, u8 msgType);
void func_151417C4(u8 arg0, u8 arg1);
s32 func_15141818(s32 arg0, s32 arg1);
void func_1514182C(struct GlowObj *obj, f32 *pivot, f32 distance, f32 spread, f32 angleX, f32 angleZ);
s32 func_15141928(struct GlowObj *obj);

/* game_1A0420.c */
Gfx *func_15172F70(Gfx *arg0);

/* game_1A11B0.c */
Gfx *func_15173D00(Gfx *gfx, s32 x, s32 y, s32 unused, s32 zDest, s32 colorDest, s16 view);
Gfx *func_151742EC(Gfx *gfx, s32 view);
s32 func_151745F0(f32 x, f32 y, f32 z, struct ProbeData *data, s8 callback, s32 arg5);

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
