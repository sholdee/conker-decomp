#ifndef CONKER_SCREEN_H
#define CONKER_SCREEN_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "screen": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_35EC0.c
 *   game_FB600.c
 *   game_FBBF0.c
 *   game_FF5C0.c
 *   game_126F60.c
 *   game_131620.c
 *   game_1AC2F0.c
 *   game_1D2480.c
 *   game_1FFB70.c
 *   game_203340.c
 */

/*
 * Mosaic (pixelate) effect: a screen rectangle around one of the owner's bones is redrawn in
 * blocks of 0x10000 / step pixels.
 */
typedef struct {
    /* 0x000 */ Actor *owner;
    /* 0x004 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x008 */ f32 halfWidth;
    /* 0x00C */ f32 halfHeight;
    /* 0x010 */ s32 step;  /* 16.16 source advance per pixel */
    /* 0x014 */ u8 flags;  /* 1: the rectangle is on screen */
    /* 0x018 */ s32 left;
    /* 0x01C */ s32 top;
    /* 0x020 */ s32 right;
    /* 0x024 */ s32 bottom;
    /* 0x028 */ f32 center[2];
    /* 0x030 */ s16 srcColumn[640];
    /* 0x530 */ s16 srcRow[480];
} MosaicData; /* size 0x8F0 */

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ MosaicData data;
} MosaicEffect;

/* Per-effect data of a strobe: the screen alternates between lit (litTime) and dark (darkTime). */
typedef struct {
    /* 0x00 */ u8 lit;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 timer; /* position in the current period */
    /* 0x08 */ f32 litTime;
    /* 0x0C */ f32 darkTime;
    /* 0x10 */ f32 period; /* litTime + darkTime */
    /* 0x14 */ u8 litAlpha;
    /* 0x15 */ u8 darkAlpha;
    /* 0x16 */ u8 pad16[2];
} StrobeData; /* size 0x18 */

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ StrobeData data;
} StrobeEffect;

/* Per-effect data of a strobe spawner: spawns a strobe after a random delay, then rearms. */
typedef struct {
    /* 0x00 */ s32 minDelay;
    /* 0x04 */ s32 delayRange;
    /* 0x08 */ s32 delay; /* frames until the next strobe */
    /* 0x0C */ s16 minDuration;
    /* 0x0E */ s16 durationRange;
    /* 0x10 */ f32 litTime;
    /* 0x14 */ f32 darkTime;
    /* 0x18 */ u8 litAlpha;
    /* 0x19 */ u8 darkAlpha;
} StrobeSpawnerData;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x28 - 0xD];
    /* 0x28 */ StrobeSpawnerData data;
} StrobeSpawner;

// Screen fade-to-black effect spawned for a pair of objects (caller: game_1C1150.c).
typedef struct {
    EffectHeader header;
    Actor *owner;  // 0x10
    Actor *source; // 0x14
    u16        timer;  // 0x18
    u8         state;  // 0x1A
    u8         alpha;  // 0x1B
    u8         mode;   // 0x1C: 0 = trigger on owner's animation frame, 1 = trigger after timer
} FadeEffect;

/* Effect data of a colour flash spawned by func_15182670. */
typedef struct {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 alpha;
    /* 0x4 */ u8 view;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ s16 alphaStep; // alpha per remaining frame
} FlashData;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ FlashData data;
} FlashEffect;

/* Full-screen tint effect: redraws a viewport's framebuffer modulated by a pulsing prim colour. */
struct Obj151A4FD0 {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 timer;       /* frames left; 0 = unlimited */
    /* 0x14 */ u8 type;         /* index into D_8008F900 */
    /* 0x15 */ u8 red;
    /* 0x16 */ u8 green;
    /* 0x17 */ u8 blue;
    /* 0x18 */ u8 rising;       /* red pulse direction: 0 fading, 1 brightening */
    /* 0x19 */ u8 unk19;        /* index into D_8008F904; 0 also calls func_10011FA0(2) */
    /* 0x1A */ u8 viewport;     /* index into D_800BE628 */
    /* 0x1B */ u8 pad1B[0x5];
}; /* size 0x20 */

/*
 * Object type 0x3C (list 1): a full-screen overlay that fades in, scrolls its texture horizontally and fades
 * out again. Several overlays can coexist; each is drawn only for the matching id.
 */
typedef struct ScreenOverlay {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s16 id;
    /* 0x12 */ s16 scroll; /* horizontal texture offset in 1/4 texels, wraps at 256 */
    /* 0x14 */ s16 alpha;  /* 0..128 */
    /* 0x16 */ s8 speed;   /* 1 while running, -2 once stopped (fades back out) */
} ScreenOverlay;

/* game/done/game_35EC0.c */
void func_15008A10(void);
void func_15008A60(void);

/* game_FB600.c */
MosaicEffect *func_150CE150(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
s32 func_150CE200(MosaicEffect *effect);
void func_150CE450(MosaicEffect *effect, u16 *fb);
void func_150CE694(MosaicEffect *effect, ActorMsg *msg, u8 type);

/* game_FBBF0.c */
void func_150CE740(Actor *obj);

/* game_FF5C0.c */
void func_150D2110(s16 duration, f32 litTime, f32 darkTime, u8 litAlpha, u8 darkAlpha, u8 owner, s32 kind);
void func_150D21CC(StrobeEffect *effect);
void func_150D227C(StrobeEffect *effect);
void func_150D22A8(StrobeEffect *effect);
void func_150D22D4(StrobeEffect *effect);
Gfx *func_150D22F4(Gfx *gfx, StrobeEffect *effect, s16 view);
extern u8 D_800D9900;
void func_150D2374(StrobeSpawner *spawner);

/* game_126F60.c */
void func_150F9AB0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 pitch, f32 yaw, f32 roll);

/* game_131620.c */
void func_15104170(s32 mode, Actor *owner, Actor *source);
void func_151041E4(FadeEffect *effect);
Gfx *func_1510448C(Gfx *gfx, FadeEffect *effect, s16 view);
u8 func_151044F4(void);

/* game_1AC2F0.c */
void func_1517EE40(s32 r, s32 g, s32 b, s32 duration, s8 toColor, s32 view);
s32 func_1517EF00(s32 view);
s32 func_1517EFAC(s32 view);
s32 func_1517EFDC(void);
Gfx *func_1517F08C(Gfx *gfx, s32 alpha, s32 r, s32 g, s32 b, s32 view);
Gfx *func_1517F3A0(Gfx *gfx, s32 view);
s32 func_1517F40C(s32 view);
void func_1517F448(s32 view);
void func_1517F488(s32 r, s32 g, s32 b, s32 alpha, s32 duration, s32 view);
Gfx *func_1517F4D8(Gfx *gfx, s32 view);
Gfx *func_1517F564(Gfx *gfx);
void func_1517F720(s32 colorIndex, s32 duration, s32 speed, s32 alpha, s32 pulse);
void func_1517F75C(void);
void func_1517F7B4(void);
void func_1517F814(s32 onScreen, f32 x, f32 y, f32 z, s32 view);
Gfx *func_1517F9F4(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 view);
Gfx *func_1517FB9C(Gfx *gfx, s32 size, s32 texSize, f32 dx, f32 dy, s32 view);
Gfx *func_15180580(Gfx *gfx, s32 view);
void func_151814FC(void);
s32 func_15181CC8(s32 view);
void func_15181D00(s32 view, s32 style);
void func_15181D70(s32 view);
void func_15181DC8(s32 view);
void func_15181E18(s32 view);
Gfx *func_15181EE0(Gfx *gfx);
void func_15182670(u8 r, u8 g, u8 b, u8 alpha, s32 duration, u8 view, u8 owner, s32 kind);
void func_15182748(FlashEffect *effect);
Gfx *func_15182768(Gfx *gfx, FlashEffect *effect, s16 view);

/* game_1D2480.c */
struct Obj151A4FD0 *func_151A4FD0(s32 type, s32 timer, s32 red, s32 green, s32 blue, s32 viewport, u8 unk19,
                                  s32 extraSize);
void func_151A5070(struct Obj151A4FD0 *effect);
void func_151A5130(void *arg0, struct Obj151A4FD0 *effect, s16 arg2);
Gfx *func_151A5170(Gfx *gfx, struct Obj151A4FD0 *effect, s32 arg2);
void func_151A55D4(struct Obj151A4FD0 *effect, void *arg1, u8 arg2);
void func_151A561C(s32 arg0, u8 arg1);

/* game_1FFB70.c */
ScreenOverlay *func_151D26C0(s16 id);
void func_151D2718(s16 id);
void func_151D275C(ScreenOverlay *overlay);
Gfx *func_151D2830(Gfx *gfx, ScreenOverlay *overlay, s16 id);

/* game_203340.c */
Gfx *func_151D5E90(Gfx *gfx, void *img, s32 arg2, s32 arg3);
void func_151D61B0(u16 *arg0);
Gfx *func_151D6418(Gfx *gfx, u8 *img, s32 arg2, s32 alpha);
void func_151D66F0(s32 arg0, s32 arg1);
Gfx *func_151D6778(Gfx *gfx);

#endif
