#ifndef CONKER_WEAPON_H
#define CONKER_WEAPON_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "weapon": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_119370.c
 *   game_11A680.c
 *   game_12B250.c
 *   game_12BD10.c
 *   game_12C1E0.c
 *   game_12CCF0.c
 *   game_1E58B0.c
 *   game_1ED0F0.c
 *   game_1EF500.c
 *   game_1F1F60.c
 *   game_1F2730.c
 *   game_1F3780.c
 */

/* A particle spawned by func_151B8400, which keeps its bullet at 0x110. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ Bullet *bullet;
} BulletParticle;

/* A positional sound; see func_151C110C. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ Bullet *bullet;
} SoundEmitter;

/* Projectile state, stored at offset 0x110 of the object func_151C229C spawns. */
typedef struct Game1EF500TrailExt {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f velocity;
    /* 0x10 */ Vec3f accel; /* eases toward drift each frame; also the length of the drawn tail */
    /* 0x1C */ Vec3f drift;
    /* 0x28 */ RayHit path;
    /* 0x8C */ f32 remaining; /* distance left before the projectile expires */
    /* 0x90 */ Actor *owner;
    /* 0x94 */ u8 ownerId; /* owner->unique_id at spawn */
    /* 0x95 */ u8 pad95[3];
    /* 0x98 */ f32 scale;
    /* 0x9C */ s8 expireHandler; /* index into D_8008FBD0, -1 for none */
    /* 0x9D */ u8 pad9D;
    /* 0x9E */ u16 sound;
    /* 0xA0 */ u8 soundEnabled;
    /* 0xA1 */ u8 listener; /* index into camera_global_Cameras of the player the sound follows */
    /* 0xA2 */ u8 padA2[2];
    /* 0xA4 */ s32 fieldA4;
    /* 0xA8 */ s8 fieldA8;
    /* 0xA9 */ u8 padA9[3];
    /* 0xAC */ s32 handle;
} Game1EF500TrailExt; /* 0xB0 */

/* The projectile object (allocated by func_1513D2F0). */
typedef struct Game1EF500Trail {
    /* 0x000 */ DefParticleHead head;    /* head.def.unk44: alpha */
    /* 0x110 */ Game1EF500TrailExt trail;
} Game1EF500Trail;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ StreamEmitter emitter;
} Game1EF500TimedOwner;

/* A pair of linked ids, each with a byte that goes with it. */
typedef struct {
    /* 0x0 */ s32 idA;
    /* 0x4 */ s32 idB;
    /* 0x8 */ u8 tagA;
    /* 0x9 */ u8 tagB;
} GameStruct151C4AB0a;

/* State block stored at offset 0x110 of the object. */
typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 unk90;
    /* 0x94 */ u8 unk94;
} GameStruct151C4AB0b;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ GameStruct151C4AB0b state;
} GameObj151C4AB0;

typedef struct {
    u8 pad0[0x60];
    s32 unk60;
    u8 pad64[0x70 - 0x64];
    u8 unk70;
    u8 pad71[0x7C - 0x71];
    Actor *unk7C;
} Func151C7038Arg;

/* game_119370.c */
Game1EF500Trail *func_150EBEC0(Actor *actor, u8 slot, u8 arg2, s32 arg3);
s32 func_150EC3D4(Actor *actor, Actor *self);
void func_150EC45C(void *particle);

/* game_11A680.c */
f32 func_150ED1D0(f32 from, f32 to);
f32 func_150ED234(Actor *self, Actor *target);
f32 func_150ED298(Actor *obj, f32 pitch, f32 targetX, f32 targetY, f32 targetZ, f32 radius);
void func_150ED578(Actor *obj);
void func_150ED638(Actor *obj, s32 left, s32 right);
void func_150ED748(Actor *obj);
void func_150EEC84(Actor *obj);
void func_150EEDA8(Actor *obj);

/* game_12B250.c */
Game1EF500Trail *func_150FDDA0(Actor *obj, s32 arg1, s32 arg2);
void func_150FDF38(Actor *obj, u8 arg1, s32 arg2, Vec3f *muzzleOut, Vec3f *launchOut);
void func_150FE0B8(Actor *obj, Vec3f *muzzle, u8 arg2, s32 arg3);
void func_150FE248(Actor *obj, s16 arg1, Vec3f *out);
void func_150FE288(Actor *obj);

/* game_12BD10.c */
void func_150FE860(Actor *actor, u8 arg1, s32 arg2);
void func_150FEBC8(Actor *actor, s16 arg1, Vec3f *out);
void func_150FEC28(Actor *actor, u8 bone, const Vec3f *start, const Vec3f *end, Vec3f *position, u8 arg5, s32 arg6);

/* game_12C1E0.c */
void func_150FED30(Actor *obj, s32 arg1, s32 arg2);
void func_150FEFD0(Actor *obj, s16 setId, Vec3f *out);
void func_150FF084(Actor *obj, u8 arg1, s32 arg2);
ModelAttachment *func_150FF288(Actor *obj);
void func_150FF2AC(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void func_150FF2D4(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   Vec3f **value0, Vec3f **value1, Vec3f *endpoint, Actor *obj, ModelAttachment *gun);
void func_150FF474(Vec3f *position, Vec3f *points, u8 arg2, s32 arg3);
s32 func_150FF6B4(Actor *obj, s32 arg1, s32 arg2);
u8 func_150FF6E0(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ModelAttachment *gun);
extern const Vec3f D_800A2050;
extern const Vec3f D_800A205C;
extern const Vec3f D_800A2068;
extern const Vec3f D_800A2074;
extern const Vec3f D_800A2080[2][6];

/* game_12CCF0.c */
void func_150FF840(Actor *obj, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_150FFB6C(Vec3f *position, Vec3f *origin, Actor *obj, s32 type);
void func_150FFBDC(Actor *obj, s16 arg1, Vec3f *out);
void func_150FFC3C(Actor *obj);
void func_150FFCC8(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void func_150FFD2C(Actor *obj, Actor *other, s32 arg2);
void func_150FFD84(Vec3f *position, Vec3f *direction, u8 arg2, s32 arg3);
extern u8 D_80088BB0;
extern const Vec3f D_800A2130;
extern const Vec3f D_800A2148;
extern const Vec3f D_800A2154;

/* game_1E58B0.c */
BulletParticle *func_151B8400(Bullet *bullet);
s32 func_151B85AC(BulletParticle *particle);
void func_151B863C(BulletParticle *particle, s16 viewport);
BloodTrail *func_151B8668(Bullet *bullet, u8 fxOwner, s32 fxList);
void func_151B86F4(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
SpriteParticle *func_151B8908(Bullet *bullet);

/* game_1ED0F0.c */
void func_151BFC40(JetSpray *config, f32 *scale);
void func_151BFDA0(Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList);
void func_151BFE84(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList);
void func_151C0098(Vec3f *pos, Actor *hitObj, u8 withParticles, u8 fxOwner, s32 fxList);
s32 func_151C02E4(Actor *obj, Actor *self, s32 ignoreCount, Actor **ignore);
void func_151C0360(CollisionTri *surface, Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C0418(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C04F8(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C05A4(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C05F0(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C0644(Vec3f *pos, u8 fxOwner, s32 fxList);
void func_151C0698(Actor *shooter, Actor *aimer, IgnoreList *ignore, u8 impactType, u8 fxOwner, s32 fxList);
s32 func_151C110C(SoundEmitter *emitter, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6);
s32 func_151C1180(Bullet *bullet);
void func_151C1570(Bullet *bullet);
void func_151C15FC(Bullet *bullet);
void func_151C1628(Bullet *bullet);
void func_151C1654(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, CollisionTri *surface);
void func_151C1798(Actor *obj, Vec3f *pos);
void func_151C1814(Bullet *bullet, Actor **pair, u8 event);
void func_151C1860(Vec3f *pos, u8 fxOwner, s32 fxList);
s32 func_151C1940(Actor *obj, Actor *self, IgnoreList *ignore);
s32 func_151C196C(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
                  Actor *shooter, IgnoreList *ignore, Vec3f *passPos, f32 *passDist,
                  CollisionHit *pass, s32 mode);
void func_151C1D5C(Actor *shooter, Vec3f *start, Vec3f *dir, RayHit *ray, f32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s8 arg8, IgnoreList *ignore, Vec3f *passPos,
                   f32 *passDist, CollisionHit *pass, s32 mode);
void func_151C1FB8(Actor *obj);
extern s32 (*D_800E0930)(Vec3f *start, Vec3f *delta, Vec3f *hit); /* per-level test of a bullet's step; TRUE stops it */
extern void (*D_800E0934)(s32 x, s32 y, s32 z); /* per-level bullet impact hook (func_151C1654) */
extern const u8 D_800AA954[3];
extern const Vec3f D_800AA958[3];

/* game_1EF500.c */
/* The level hook D_800E0940: (hit position, index of the WorldObject that was hit). */
typedef void (*TrailHitCallback)(Vec3f *pos, s32 index);
extern TrailHitCallback D_800E0940;
void func_151C2050(Actor *obj, Vec3f *pos, Vec3f *dir, s32 soundId, f32 distance);
Game1EF500Trail *func_151C229C(Vec3f *origin, Vec3f *direction, Vec3f *altOrigin, Vec3f *altDirection,
                               u8 altNormalized, RayHit *knownHit, f32 speed, f32 arg7, f32 arg8,
                               f32 driftSpeed, f32 arg10, u8 arg11, Actor *owner, u8 arg13, u8 collide, u8 arg15,
                               u8 soundChance, u8 hitCallback, u8 expireHandler, s8 handlerIndex, s32 arg20,
                               f32 dt, u8 arg22, s8 arg23, s32 extraSize, u8 arg25, s32 arg26);
s8 func_151C2734(Game1EF500Trail *trail);
Vtx *func_151C2AD0(Game1EF500Trail *trail, s16 index);
s32 func_151C2E4C(Actor *obj, Actor *self);
s32 func_151C2E94(Actor *obj, Actor *self);
s32 func_151C2EF0(void *actor, Actor *owner, Game1EF500Trail *trail, Vec3f *hitPos, Vec3f *hitPoint,
                  RayHit *hit);
void func_151C2F48(Game1EF500Trail *trail);
void func_151C329C(Vec3f *pos, u8 arg1, s32 arg2);
void func_151C36D8(Vec3f *pos, u8 arg1, s32 arg2);
void func_151C3B0C(Game1EF500Trail *trail, f32 debrisChance, f32 sparkChance, f32 smokeChance, f32 flashChance,
                   u8 debrisArg, u8 smokeArg, u8 sparkArg);
void func_151C436C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps);
u8 func_151C43E0(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void func_151C4510(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
s32 func_151C455C(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void func_151C4644(Game1EF500TimedOwner *owner);

/* game_1F1F60.c */
void func_151C4AB0(GameObj151C4AB0 *obj, GameStruct151C4AB0a *pair, u8 event);
s32 func_151C4B0C(RayHit *knownHit, RayHit *hit, Vec3f *origin, Vec3f *dir, f32 *remaining,
                  f32 distScale, u8 castRay, Vec3f *altOrigin, Vec3f *altDir, u8 *flags, u8 clearMask,
                  f32 defaultRemaining, void *owner, s8 arg13, u8 collide, u8 arg15, s32 arg16, u8 recast);

/* game_1F2730.c */
void func_151C5280(Actor *obj, u8 arg1, s32 arg2);
void func_151C5588(Actor *obj, Vec3f *pos, s16 arg2, u8 arg3, s32 arg4);
void func_151C56A4(Vec3f *pos, u8 arg1, s32 arg2);
void func_151C577C(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2, u8 doForward, u8 doRing, u8 doScatter, u8 arg6,
                   s32 arg7);
void func_151C5E74(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);
SpriteParticle *func_151C5F44(Vec3f *pos, Vec3f *vel, f32 unk54, f32 unkA8Value, u8 unk1B, s16 unk0A, f32 size,
                        u8 altUnk62, CollisionHit *arg8, u8 arg9, s32 argA);
void func_151C61A0(Vec3f *arg0, CollisionHit *arg1, u8 arg2, s32 arg3);
s32 func_151C6290(Actor *obj, Actor *self, s32 arg2);
extern const Vec3f D_800AAA90; /* bone-attachment offset */
extern const Vec3f D_800AAA9C;
extern const Vec3f D_800AAAA8;

/* game_1F3780.c */
Game1EF500Trail *func_151C62D0(Actor *arg0, u8 side, u8 arg2, ModelAttachment *attachment, s16 arg4, u8 arg5,
                               s32 arg6);
void func_151C6974(Actor *arg0, s16 arg1, Vec3f *out);
void func_151C69A0(Actor *arg0, s16 arg1, Vec3f *out);
void func_151C69CC(Actor *arg0, Vec3f *out, u8 side);
void func_151C6A28(Actor *arg0, u8 side, u8 arg2, s32 arg3);
void func_151C6D70(Actor *arg0, u8 side, Vec3f *pos, s16 arg3, u8 arg4, s32 arg5);
void func_151C6EA0(Actor *owner, u8 arg1, s32 arg2);
s32 func_151C7038(Func151C7038Arg *arg0);

#endif
