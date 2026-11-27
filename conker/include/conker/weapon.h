#ifndef CONKER_WEAPON_H
#define CONKER_WEAPON_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "weapon": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Weapon/TwinMuzzle/twinmuzzle.c
 *   game/Weapon/Tank/tank.c
 *   game/Weapon/SecondGun/secondgun.c (data-only unit between game/Player/FlyControl/flycontrol and game/Particle/Spiral/spiral)
 *   game/Weapon/Launcher/launcher.c
 *   game/Weapon/ForwardShot/forwardshot.c
 *   game/Weapon/AttachedGun/attachedgun.c
 *   game/Weapon/GunShot/gunshot.c
 *   game/Weapon/BulletParticle/bulletparticle.c
 *   game/Weapon/Bullet/bullet.c
 *   game/Weapon/Projectile/projectile.c
 *   game/Weapon/ProjectileRay/projectileray.c
 *   game/Weapon/SegmentShot/segmentshot.c
 *   game/Weapon/JointShot/jointshot.c
 *   game_data_800DCA20.c (data only, owner unknown; linked after game/Math/MathUtil/mathutil)
 */

/* A particle spawned by weapon_spawnBulletParticle, which keeps its bullet at 0x110. */
typedef struct BulletParticle {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ Bullet *bullet;
} BulletParticle;

/* Projectile state, stored at offset 0x110 of the object weapon_spawnProjectile spawns. */
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
    /* 0xA4 */ s32 hitFlags; /* particle_applyShotHit's flags */
    /* 0xA8 */ s8 filter; /* collision_castRay's filter */
    /* 0xA9 */ u8 padA9[3];
    /* 0xAC */ s32 handle;
} Game1EF500TrailExt; /* 0xB0 */

/* The projectile object (allocated by particle_newQuadParticle). */
typedef struct Game1EF500Trail {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ Game1EF500TrailExt trail;
} Game1EF500Trail;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ StreamEmitter emitter;
} Game1EF500TimedOwner;

/* game/Weapon/TwinMuzzle/twinmuzzle.c */
Game1EF500Trail *weapon_fireTwinMuzzleShot(Actor *actor, u8 slot, u8 fxOwner, s32 fxList);
s32 func_150EC3D4(Actor *actor, Actor *self);
void weapon_spawnHitEffectsNoFlash(void *particle);

/* game/Weapon/Tank/tank.c */
f32 weapon_getAngleDelta(f32 from, f32 to);
f32 weapon_getAngleToTarget(Actor *self, Actor *target);
f32 weapon_getShellAimError(Actor *obj, f32 pitch, f32 targetX, f32 targetY, f32 targetZ, f32 radius);
void weapon_hideTankInSniperView(Actor *obj);
void weapon_scrollTreads(Actor *obj, s32 left, s32 right);
void weapon_updateTank(Actor *obj);
void weapon_releaseTankCamera(Actor *obj);
void weapon_updateHidden(Actor *obj);

/* game/Weapon/SecondGun/secondgun.c (data-only unit between game/Player/FlyControl/flycontrol and game/Particle/Spiral/spiral) */
extern const Vec3f gSecondGunMuzzle; /* the second gun's muzzle point */
extern u8 gSecondGunMuzzleBone;          /* the bone it is on */

/* game/Weapon/Launcher/launcher.c */
Game1EF500Trail *weapon_fireLauncherProjectile(Actor *obj, s32 fxOwner, s32 fxList);
void weapon_runLauncherMuzzle(); /* (Actor *obj, u8 fxOwner, s32 fxList, Vec3f *muzzleOut, Vec3f *launchOut); K&R definition, declared without a prototype */
void weapon_spawnLauncherFlash(); /* (Actor *obj, Vec3f *muzzle, u8 fxOwner, s32 fxList); K&R definition, declared without a prototype */
void weapon_getLauncherMuzzlePos(Actor *obj, s16 unused, Vec3f *out);
void weapon_rumbleLauncher(Actor *obj);

/* game/Weapon/ForwardShot/forwardshot.c */
void weapon_spawnForwardProjectile(Actor *actor, u8 fxOwner, s32 fxList);
void weapon_getForwardProjectileStart(Actor *actor, s16 unused, Vec3f *out);
void weapon_spawnBoneBeam(Actor *actor, u8 bone, const Vec3f *start, const Vec3f *end, Vec3f *position, u8 fxOwner, s32 fxList);

/* game/Weapon/AttachedGun/attachedgun.c */
void weapon_fireAttachedGun(Actor *obj, s32 fxOwner, s32 fxList);
void weapon_getAttachmentPos(Actor *obj, s16 setId, Vec3f *out);
void weapon_fireAttachedGunProjectile(Actor *obj, u8 fxOwner, s32 fxList);
ModelAttachment *weapon_getGunAttachment(Actor *obj);
void weapon_getShotAxes(Actor *obj, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void weapon_findShotStart(u8 *valid, Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal,
                   Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *output0, Vec3f *output1,
                   Vec3f **value0, Vec3f **value1, Vec3f *endpoint, Actor *obj, ModelAttachment *gun);
void weapon_spawnMuzzleFlashes(Vec3f *position, Vec3f *points, u8 fxOwner, s32 fxList);
s32 func_150FF6B4(Actor *obj, s32 self, s32 filterArg);
u8 weapon_getGunMuzzlePoints(Vec3f *points, Vec3f *position, Vec3f *hit, Vec3f *normal, Vec3f *endpoint,
                 Actor *obj, ModelAttachment *gun);
extern const Vec3f gAttachedGunMuzzle;
extern const Vec3f D_800A205C;
extern const Vec3f D_800A2068;
extern const Vec3f D_800A2074;
extern const Vec3f gAttachedGunFlashPoints[2][6];

/* game/Weapon/GunShot/gunshot.c */
void weapon_spawnShotEffects(Actor *obj, u8 spawnAttacks, u8 castRay, u8 fxOwner, s32 fxList);
void weapon_spawnAttackAt(Vec3f *position, Vec3f *origin, Actor *obj, s32 type);
void weapon_getGunMuzzlePos(Actor *obj, s16 unused, Vec3f *out);
void weapon_rumbleGun(Actor *obj);
void weapon_getShotFrame(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1);
void func_150FFD2C(Actor *obj, Actor *other, s32 unused);
void weapon_spawnFannedParticles(Vec3f *position, Vec3f *direction, u8 fxOwner, s32 fxList);
extern u8 gGunMuzzleBone;
extern const Vec3f gGunMuzzleStart;
extern const Vec3f D_800A2148;
extern const Vec3f D_800A2154;

/* game/Weapon/BulletParticle/bulletparticle.c */
BulletParticle *weapon_spawnBulletParticle(Bullet *bullet);
s32 weapon_updateBulletParticle(BulletParticle *particle);
void weapon_drawBulletParticle(BulletParticle *particle, s16 viewport);
BloodTrail *weapon_startBulletTrail(Bullet *bullet, u8 fxOwner, s32 fxList);
void weapon_emitBulletTrailParticle(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
SpriteParticle *weapon_spawnBulletSprite(Bullet *bullet);

/* game/Weapon/Bullet/bullet.c */
void weapon_initImpactSpray(JetSpray *config, f32 *scale);
void weapon_spawnAirHit(Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList);
void weapon_spawnSurfaceHit(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList);
void weapon_spawnObjectHit(Vec3f *pos, Actor *hitObj, u8 withParticles, u8 fxOwner, s32 fxList);
s32 weapon_isBulletTarget(Actor *obj, Actor *self, s32 ignoreCount, Actor **ignore);
void weapon_spawnHitDecal(CollisionTri *surface, Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_spawnHitLight(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_spawnHitGrowFade(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_wobbleNearbyCameras(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_rumbleNearbyPlayers(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_startHitRandomEmitter(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_fireBullet(Actor *shooter, Actor *aimer, IgnoreList *ignore, u8 impactType, u8 fxOwner, s32 fxList);
s32 weapon_bulletSoundCallback(SoundRecord *emitter, s32 active, void **volume, s32 pan, s32 cents, s32 mix, s16 *sound);
s32 weapon_updateBullet(Bullet *bullet);
void weapon_releaseBullet(Bullet *bullet);
void weapon_destroyBullet(Bullet *bullet);
void weapon_freeBullet(Bullet *bullet);
void weapon_runImpactAttack(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, CollisionTri *surface);
void weapon_getBulletMuzzlePos(Actor *obj, Vec3f *pos);
void weapon_onBulletPairEvent(Bullet *bullet, Actor **pair, u8 event);
void weapon_spawnShortLight(Vec3f *pos, u8 fxOwner, s32 fxList);
s32 weapon_filterBulletTarget(Actor *obj, Actor *self, IgnoreList *ignore);
s32 weapon_traceBullet(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
                  Actor *shooter, IgnoreList *ignore, Vec3f *passPos, f32 *passDist,
                  CollisionHit *pass, s32 mode);
void weapon_castPiercingRay(Actor *shooter, Vec3f *start, Vec3f *dir, RayHit *ray, f32 maxDist,
                   s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, IgnoreList *ignore, Vec3f *passPos,
                   f32 *passDist, CollisionHit *pass, s32 mode);
void weapon_rumbleShooter(Actor *obj);
extern s32 (*gBulletHitTest)(Vec3f *start, Vec3f *delta, Vec3f *hit); /* per-level test of a bullet's step; TRUE stops it */
extern void (*gBulletImpactHook)(s32 x, s32 y, s32 z); /* per-level bullet impact hook (weapon_runImpactAttack) */
extern const u8 gBulletMuzzleBones[3];
extern const Vec3f gBulletMuzzleOffsets[3];

/* game/Weapon/Projectile/projectile.c */
/* The level hook gProjectileHitCallback: (hit position, index of the WorldObject that was hit). */
typedef void (*TrailHitCallback)(Vec3f *pos, s32 index);
extern TrailHitCallback gProjectileHitCallback;
void weapon_queueShotSound(Actor *obj, Vec3f *pos, Vec3f *dir, s32 soundId, f32 distance);
Game1EF500Trail *weapon_spawnProjectile(Vec3f *origin, Vec3f *direction, Vec3f *altOrigin, Vec3f *altDirection,
                               u8 altNormalized, RayHit *knownHit, f32 speed, f32 distScale, f32 width,
                               f32 driftSpeed, f32 defaultRemaining, u8 alpha, Actor *owner, u8 castRay, u8 collide, u8 hitModels,
                               u8 soundChance, u8 hitCallback, u8 expireHandler, s8 handlerIndex, s32 hitFlags,
                               f32 dt, u8 viewportMask, s8 filter, s32 extraSize, u8 fxOwner, s32 fxList);
s8 weapon_updateProjectile(Game1EF500Trail *trail);
Vtx *weapon_buildProjectileTail(Game1EF500Trail *trail, s16 index);
s32 weapon_isOtherLiveObject(Actor *obj, Actor *self);
s32 weapon_isOtherLivePlayer(Actor *obj, Actor *self);
s32 weapon_onProjectileImpact(void *actor, Actor *owner, Game1EF500Trail *trail, Vec3f *hitPos, Vec3f *hitPoint,
                  RayHit *hit);
void weapon_onProjectileExpire(Game1EF500Trail *trail);
void weapon_spawnImpactBurst(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_spawnSmallImpactBurst(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_spawnHitEffects(Game1EF500Trail *trail, f32 debrisChance, f32 sparkChance, f32 smokeChance, f32 flashChance,
                   u8 debrisArg, u8 smokeArg, u8 sparkArg);
void weapon_easeProjectileAccel(Game1EF500Trail *trail, Game1EF500TrailExt *ext, s32 steps);
u8 weapon_traceProjectile(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void weapon_moveProjectile(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
s32 weapon_countdownProjectileRange(Game1EF500Trail *trail, Game1EF500TrailExt *ext, f32 dt);
void weapon_updateStreamEmitter(Game1EF500TimedOwner *owner);


/* game/Weapon/ProjectileRay/projectileray.c */
void weapon_projectileMessage(Game1EF500Trail *obj, ActorSwapMsg *swap, u8 event);
s32 weapon_setupProjectileRay(RayHit *knownHit, RayHit *hit, Vec3f *origin, Vec3f *dir, f32 *remaining,
                  f32 distScale, u8 castRay, Vec3f *altOrigin, Vec3f *altDir, u8 *flags, u8 clearMask,
                  f32 defaultRemaining, void *owner, s8 filter, u8 collide, u8 hitModels, s32 ignoreMask, u8 recast);

/* game/Weapon/SegmentShot/segmentshot.c */
void weapon_fireSegmentShot(Actor *obj, u8 fxOwner, s32 fxList);
void weapon_spawnSegmentBeam(Actor *obj, Vec3f *pos, s16 node, u8 fxOwner, s32 fxList);
void weapon_spawnLightAt(Vec3f *pos, u8 fxOwner, s32 fxList);
void weapon_spawnHitBurst(Vec3f *from, Vec3f *to, Vec3f *direction, u8 doForward, u8 doRing, u8 doScatter, u8 fxOwner,
                   s32 fxList);
void weapon_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);
SpriteParticle *weapon_spawnDragSprite(Vec3f *pos, Vec3f *vel, f32 gravity, f32 drag, u8 alpha, s16 timer, f32 size,
                        u8 altUpdate, CollisionHit *ground, u8 fxOwner, s32 fxList);
void weapon_spawnTedizSplash(Vec3f *pos, CollisionHit *ground, u8 fxOwner, s32 fxList);
s32 func_151C6290(Actor *obj, Actor *self, s32 filterArg);
extern const Vec3f gSegmentShotStart; /* bone-attachment offset */
extern const Vec3f gSegmentShotBeamEnd;
extern const Vec3f gSegmentShotEnd;

/* game/Weapon/JointShot/jointshot.c */
Game1EF500Trail *weapon_fireJointShot(Actor *obj, u8 side, u8 jointAim, ModelAttachment *attachment, s16 node, u8 fxOwner,
                               s32 fxList);
void weapon_getSide0JointPos(Actor *obj, s16 unused, Vec3f *out);
void weapon_getSide1JointPos(Actor *obj, s16 unused, Vec3f *out);
void weapon_getJointPos(Actor *obj, Vec3f *out, u8 side);
void weapon_fireJointShotProjectile(Actor *obj, u8 side, u8 fxOwner, s32 fxList);
void weapon_spawnJointBeam(Actor *obj, u8 side, Vec3f *pos, s16 node, u8 fxOwner, s32 fxList);
void weapon_spawnHoverModel(Actor *owner, u8 fxOwner, s32 fxList);
s32 weapon_updateHoverModel(ModelParticle *particle);

/* game_data_800DCA20.c (data only, owner unknown; linked after game/Math/MathUtil/mathutil) */
extern u8 gLayoutDetailShift;
extern f32 gLayoutDetailScale;
extern u8 gPlayerCountDetailShift;
extern f32 gPlayerCountDetailScale;

#endif
