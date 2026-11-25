#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The effect argument blocks below mirror the ones in game_1ED0F0.c (same layouts, same callees). */

/* Particle parameters for an impact, filled in by func_151BFC40. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} ImpactFxConfig; /* size 0x54 */

/* Collision surface (0x12 bytes, 2-byte aligned). */
typedef struct {
    /* 0x00 */ s16 unk00[9];
} HitSurface;

/* Argument block for func_1514FB98: an impact with no surface. */
typedef struct {
    /* 0x00 */ vertex vel;
    /* 0x0C */ f32 scale;
    /* 0x10 */ ImpactFxConfig config;
} ImpactFx; /* size 0x64 */

/* Argument block for func_1514FBFC: an impact against a collision surface. */
typedef struct {
    /* 0x00 */ vertex vel;
    /* 0x0C */ HitSurface surface;
    /* 0x20 */ f32 scale;
    /* 0x24 */ ImpactFxConfig config;
} SurfaceImpactFx; /* size 0x78 */

/* Argument block for func_1514FCE8. */
typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ vertex pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8  unk58;
} DebrisFx;

void func_151C04F8(vertex *pos, u8 fxOwner, s32 fxList);
void func_151C05A4(vertex *pos, u8 fxOwner, s32 fxList);
void func_151C05F0(vertex *pos, u8 fxOwner, s32 fxList);
void func_151BFC40(ImpactFxConfig *config, f32 *scale);
void func_1514FB98(ImpactFx *, u8, s32);
void func_1514FBFC(SurfaceImpactFx *, u8, s32);
void func_1514FCE8(DebrisFx *, u8, s32);

/* Hit effects at pos plus a burst of debris particles. */
void func_150D7120(vertex *pos, u8 fxOwner, s32 fxList) {
    DebrisFx fx;

    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);

    fx.unk00 = 0;
    fx.unk02 = 255;
    fx.unk04 = -64;
    fx.unk06 = 71;
    fx.unk08 = 6;
    fx.unk0C = 4;
    fx.pos = *pos;
    fx.unk1C = 23.0f;
    fx.unk20 = 30.0f;
    fx.unk24 = 45.0f;
    fx.unk28 = 53.0f;
    fx.unk2C = 203.0f;
    fx.unk30 = 414.0f;
    fx.unk34 = 7;
    fx.unk38 = 3;
    fx.unk3C = 15.0f;
    fx.unk40 = 0.8720000386f;
    fx.unk44 = -0.3980000317f;
    fx.unk48 = 0.3640000224f;
    fx.unk4C = 25;
    fx.unk4E = 15;
    fx.unk50 = 100;
    fx.unk52 = 100;
    fx.unk54 = 12;
    fx.unk56 = 20;
    fx.unk58 = 0;

    func_1514FCE8(&fx, fxOwner, fxList);
}

/* Hit effects at pos plus impact particles thrown back along -dir, off surface if there is one. */
void func_150D728C(HitSurface *surface, vertex *pos, vertex *dir, u8 fxOwner, s32 fxList) {
    SurfaceImpactFx surfaceFx;
    ImpactFx fx;

    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (surface != NULL) {
        surfaceFx.vel.x = -dir->x;
        surfaceFx.vel.y = -dir->y;
        surfaceFx.vel.z = -dir->z;
        surfaceFx.surface = *surface;
        surfaceFx.config.pos = *pos;
        func_151BFC40(&surfaceFx.config, &surfaceFx.scale);
        func_1514FBFC(&surfaceFx, fxOwner, fxList);
    } else {
        fx.vel.x = -dir->x;
        fx.vel.y = -dir->y;
        fx.vel.z = -dir->z;
        func_151BFC40(&fx.config, &fx.scale);
        fx.config.pos = *pos;
        func_1514FB98(&fx, fxOwner, fxList);
    }
}
