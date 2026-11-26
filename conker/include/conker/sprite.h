#ifndef CONKER_SPRITE_H
#define CONKER_SPRITE_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "sprite": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_C1D70.c
 *   game_C2350.c
 *   game_E4070.c
 *   game_137DE0.c
 *   game_182140.c
 *   game_188F90.c
 *   game_196DB0.c
 *   game_1A89B0.c
 */

/* One 16-byte entry of a point group; only the position is read here. */
typedef struct GroupPoint {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0xA];
} GroupPoint;

typedef struct PointGroup {
    u8 pad0;
    u8 slots; /* low nibble: first slot, high nibble: last slot */
    u8 pad2[2];
    GroupPoint *points;
} PointGroup;

typedef struct ScreenSprite {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ScreenSpriteInfo info;
    /* 0x68 */ u8 unk68;      /* low nibble: index into D_8008ACC8 */
} ScreenSprite;

/* Animated, textured quad (update func_1515BAE0, draw func_1515BBF0). */
typedef struct TexturedQuad {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 texture;          // index into D_80090B60
    /* 0x11 */ u8 flags;            // 1: lifetime counts down, 2: two-cycle mode
    /* 0x12 */ u8 animFlags;        // end-of-animation behaviour, see func_1514401C
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 frame;           // 16.16 frame position
    /* 0x1C */ u32 frameSpeed;
    /* 0x20 */ u8 color0[4];
    /* 0x24 */ u8 color1[4];
    /* 0x28 */ u8 colorLevel;
    /* 0x29 */ u8 primMode;
    /* 0x2A */ u8 envMode;
    /* 0x2B */ u8 renderMode;       // index into D_800A4AC8
    /* 0x2C */ u8 combiner;
    /* 0x2D */ u8 pad2D[3];
    /* 0x30 */ s32 geometrySet;
    /* 0x34 */ s32 geometryClear;
    /* 0x38 */ s8 updateFunc;       // index into D_8008B078, -1 = none
    /* 0x39 */ s8 destroyFunc;      // index into D_8008B07C, -1 = none
    /* 0x3A */ u8 pad3A[0x16];
    /* 0x50 */ Vtx vtx[4];
} TexturedQuad;

/* What an actor tracker is created from (func_1514EE70 builds one). */
typedef struct ActorTrackerDesc {
    /* 0x0 */ Actor *actor;
    /* 0x4 */ u8 actorId;           // the actor's unique_id
    /* 0x5 */ u8 flags;             // 1: lifetime counts down
    /* 0x6 */ s16 lifetime;
} ActorTrackerDesc;

/* Object type 0x32: follows an actor's position every frame and keeps its frame-to-frame motion. */
typedef struct ActorTracker {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ ActorTrackerDesc desc;
    /* 0x20 */ Vec3f pos;
    /* 0x2C */ Vec3f prevPos;
    /* 0x38 */ Vec3f motion;        // (prevPos - pos) * D_800BE9A8
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} ActorTracker;

/* game_C1D70.c */
extern f32 (*D_800D2C20)[4]; // the current float matrix
extern s32 D_800D2C28[16];   // segment base addresses
extern u8 D_800D2C68;
void func_150948C0(PointGroup *group, f32 (*out)[3]);
void func_1509499C(PointGroup *group, GroupPoint **slots);
void func_15094AB8(Vtx *verts, s16 (*texcoords)[2], s32 count, f32 angle, s32 width, s32 height);
void func_15094E98(void);

/* game_C2350.c */
extern Gfx D_800873D0[];
extern Gfx D_80087408[];
extern const u8 D_8009DEB0[4]; // per G_IM_SIZ: G_IM_SIZ_*_INCR
extern const u8 D_8009DEB4[4]; // G_IM_SIZ_*_SHIFT
extern const u8 D_8009DEB8[4]; // line bits factor
extern const u8 D_8009DEBC[4]; // G_IM_SIZ_*_LOAD_BLOCK
extern s32 D_800D2C9C; // G_TT_RGBA16 when the last loaded texture has a palette, else G_TT_NONE
extern u8 *D_800D2CA0; // current texture frame (0x80000000 = missing asset)
extern MtxF D_800D2CA8[4]; // view matrix per player
extern u8 D_800D2DA8; // sprite prim colour r, g, b (used while D_800D2DAB is 0)
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern u8 D_800D2DAB;
void func_15094EA0(s32 view);
Gfx *func_15094F40(Gfx *gfx);
Gfx *func_15094F70(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 assetArg);
Gfx *func_15094FE8(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg);
void func_15095060(TextureDesc *desc, s32 frame, SpriteParams *sprite);
Gfx *func_150950D4(Gfx *gfx, Texture *tex, s32 tmem, s32 tile, s32 frame, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg);
s32 func_1509563C(f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW, f32 maxW);
Gfx *func_15095760(Gfx *gfx, SpriteParams *sprite);
Gfx *func_150958B0(Gfx *gfx, SpriteParams *sprite, s32 *visible);
Gfx *func_15095A48(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y);
Gfx *func_15095A90(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 sOffset, s32 tOffset, s32 extraWidth);
void func_15095B08(SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 *visible);
Gfx *func_15095D0C(Gfx *gfx, SpriteParams *sprite);
Gfx *func_15095D34(Gfx *gfx, SpriteParams *sprite, s32 sOffset, s32 tOffset, s32 extraWidth);
Gfx *func_1509629C(Gfx *gfx, TextureDesc *desc, f32 scaleX, f32 scaleY, f32 x, f32 y, s32 width, s32 height, s32 flags, s32 sOffset, s32 extraWidth, s32 frame);
Gfx *func_15096934(Gfx *gfx);

/* game_E4070.c */
void func_150B6BC0(s32 unused);
void func_150B6C90(void);
void func_150B6D34(void);
void func_150B6D78(void);
void func_150B6DFC(Sprite2D *sprite);
void func_150B6E3C(Sprite2D *sprite);
void func_150B709C(Sprite2D *sprite);
void func_150B71A8(Sprite2D *sprite);
void func_150B7220(s32 arg0, s32 arg1, s32 duration, s32 arg3);
void func_150B73F0(Sprite2D *sprite);
void func_150B7484(void);
void func_150B7560(void);
void func_150B765C(void);
void func_150B768C(void);
void func_150B76BC(s32 x, s32 slot);
void func_150B77A8(void);
void func_150B791C(void);
extern u8 D_800D9890; // queued command for func_150B6BC0
extern EffectHeader *D_800D9894;

/* game_137DE0.c */
void func_1510A930(s32 fadeIn, s32 hold, s32 fadeOut);

/* game_182140.c */
extern Gfx D_8008AC70[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_8008AC98[]; /* render teardown (object type table D_8008B4A8) */
void func_15154C90(ScreenSprite *sprite);
Gfx *func_15154D80(Gfx *gfx, ScreenSprite *sprite, s16 viewport);
void *func_1515548C(ScreenSpriteInfo *info, u8 flags, s32 *kinds, s32 kindCount, s32 arg4, u8 arg5, s32 arg6);
void func_15155564(ScreenSprite *sprite, s32 arg1, u8 arg2);
s32 func_151555AC(f32 *center, f32 *halfSize, s32 *kinds, s32 kindCount);
void func_1515572C(s32 arg0, u8 arg1);

/* game_188F90.c */
extern Gfx D_8008B050[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_8008B068[]; /* render teardown (object type table D_8008B4A8) */
void func_1515BAE0(TexturedQuad *quad);
Gfx *func_1515BBF0(Gfx *gfx, TexturedQuad *quad, s32 view);
void func_1515BE48(void);
ActorTracker *func_1515BE50(ActorTrackerDesc *desc, s32 extraSize, u8 owner, s32 list);
void func_1515BF50(ActorTracker *tracker);
void func_1515BF7C(ActorTracker *tracker);
void func_1515BFA8(ActorTracker *tracker);
void func_1515C0B8(ActorTracker *tracker, s32 event, u8 eventType);
s32 func_1515C0F8(Actor *actor, const Vec3f **motion);
void func_1515C158(void);
void func_1515C1A0(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3);
void func_1515C244(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3);

/* game_196DB0.c */
Sprite2D *func_15169900(Sprite2DParams *params, s32 arg1);
Sprite2D *func_15169968(Sprite2DParams *params);
void func_15169988(Sprite2D *sprite);
Gfx *func_15169A48(Gfx *gfx, Sprite2D *sprite, s32 arg2);

/* game_1A89B0.c */
/*
 * One sprite placed on a level triangle (D_800DDD18 holds D_800DDD20 of them, sorted by x or z; filled by
 * func_1500AF08 when the level loads).
 */
typedef struct Game1A89B0Position {
    s16 x;
    s16 y;
    s16 z;
    u8 unk6;     /* (subtype << 5) | sprite type (texture slot 0..14) */
    u8 phaseIdx; /* index into D_800DDC90 (type * 4 + phase); 60: not animated */
    u8 texInfo;  /* index into D_80095CE0 */
    u8 pad9;
    u8 kind;     /* column of D_8008CF00 */
    u8 disturbState; /* 0x21 or more: disturbed (shows the type's disturbed frame); counts down from 0x22 to 0x21 */
    s8 halfWidth;
    s8 height;
} Game1A89B0Position; /* size 0xE */

/*
 * A sprite kind (D_8008CEB8): emit places sprites of this kind on a level triangle when the level loads
 * (func_1500BEC0) and returns the next free index; anim runs for each sprite every frame (func_1517BBAC),
 * nonzero hides the sprite.
 */
typedef struct SpriteKind {
    s32 (*emit)(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri);
    s32 (*anim)(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
    s32 unk8;
} SpriteKind;

/* One cell of D_8008CF00: the sprite kind to emit and its count (the first argument of emit). */
typedef struct SpriteKindChoice {
    s16 emitter; /* index into D_8008CEB8 */
    s16 count;
} SpriteKindChoice;

/* Texture coordinate extents of a sprite type's frame. */
typedef struct SpriteTexSize {
    s16 s;
    s16 t;
} SpriteTexSize;

extern u8 D_8008CEB0; /* billboards queued by func_1517D578 */
extern s32 D_8008CEB4; /* axis D_800DDD18 is sorted along: 0 = x, else z */
extern SpriteKind D_8008CEB8[6];
extern SpriteKindChoice D_8008CF00[5][8]; /* [D_800DDC80 row][sprite kind] */
/* Per sprite type (15 types) unless noted: */
extern s32 D_800DD478[15 * 26]; /* 26 frame addresses */
extern ParticleTexFile D_800DDA90[15];
extern SpriteTexSize D_800DDB80[15];
extern u8 D_800DDBC0[15]; /* index into D_80095CE0 */
extern u8 D_800DDBD0[15]; /* frame count the phases cycle through */
extern u8 D_800DDBE0[15]; /* frame shown while a sprite is disturbed (unkB >= 0x21) */
extern u8 D_800DDBF0[15]; /* draw pass (see func_1517D7B0) */
extern s32 D_800DDC00; /* bit per type */
extern s32 D_800DDC04; /* bit per type: ping-pong animation */
extern s32 D_800DDC08; /* bit per type: random flips of the ping-pong direction */
extern u8 D_800DDC10[15]; /* per-phase direction bits for ping-pong animation */
extern u8 D_800DDC20[15][2];
extern u8 D_800DDC80[15]; /* row of D_8008CF00 */
extern s16 D_800DDC90[60]; /* animation phases, 4 per type (frame << 8) */
extern s32 D_800DDD0C; /* most sprites built per frame */
extern Vtx *D_800DDD10[2]; /* this frame's sprite vertex buffer, one per frame buffer */
extern Game1A89B0Position *D_800DDD18; /* the level's sprites */
extern u8 D_800DDD1C; /* billboard animation counter */
extern s32 D_800DDD20; /* number of sprites in D_800DDD18 */
s32 func_1517B500(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B6E8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B7A8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B7F8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
void func_1517B89C(s32 first, s32 last);
void func_1517BBAC(s32 x, s32 y, s32 z, s32 radius, f32 angle, s32 fadeX, s32 fadeY, s32 fadeZ);
void func_1517CFC4(void);
Gfx *func_1517D074(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 texWidth, u8 texHeight, s32 texture);
void func_1517D578(s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 frame);
void func_1517D5FC(s16 x, s16 y, s16 z, s32 player, s32 halfWidth, s32 height);
Gfx *func_1517D690(Gfx *gfx, s32 texture);
void func_1517D7B0(Gfx **gfx, s32 pass);
void func_1517DE5C(void);
void func_1517E05C(s32 x, s32 y, s32 z);

#endif
