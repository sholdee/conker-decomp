#ifndef EFFECTS_H
#define EFFECTS_H

#include "structs.h"

/* Particle / effect spawn descriptors shared by the spawn functions and their callers. */

/* EffectHeader (the common object head) is in structs.h. */

struct Actor;

/* Weak reference to an actor: valid while actor->unique_id still equals uniqueId. Also the payload of the
 * object messages (particle_broadcastToQuadParticles) that name an actor, e.g. message 0: the actor went away. */
typedef struct ActorRef {
    /* 0x0 */ struct Actor *actor;
    /* 0x4 */ u8 uniqueId;
} ActorRef; /* size 0x8 */

/* Object message 0x2D payload: two actors exchange identities. */
typedef struct ActorSwapMsg {
    /* 0x0 */ struct Actor *actorA;
    /* 0x4 */ struct Actor *actorB;
    /* 0x8 */ u8 idA;
    /* 0x9 */ u8 idB;
} ActorSwapMsg; /* size 0xC */

/* Payload of the actor object messages (0 and similar: an actor, 0x2D: an actor swap). */
typedef union ActorMsg {
    ActorRef ref;
    ActorSwapMsg swap;
} ActorMsg;

/* ParticleDesc.flags (sprite particles, particle_newSpriteParticleWithFloor) */
#define PARTICLE_LIFETIME          0x1      /* timer counts down; the particle dies below 0 */
#define PARTICLE_GRAVITY           0x2
#define PARTICLE_MOVE              0x4
#define PARTICLE_ANIM_PINGPONG     0x8
#define PARTICLE_ANIM_END_KILLS    0x10
#define PARTICLE_ANIM_START_KILLS  0x20
#define PARTICLE_FLIP_S            0x40
#define PARTICLE_FLIP_T            0x80
#define PARTICLE_FADE              0x200    /* alpha fades once timer < fadeStart */
#define PARTICLE_SCALE             0x400    /* size changes once timer < scaleStart */
#define PARTICLE_SCALE_MUL         0x800    /* multiply the size by scaleRate instead of adding it */
#define PARTICLE_USE_OFFSET        0x1000
#define PARTICLE_HANDLERS          0x4000   /* handlerSet is valid */
#define PARTICLE_DRAW_FUNC         0x8000
#define PARTICLE_LIT               0x10000  /* env color comes from light_getLitColourAt */
#define PARTICLE_VIEW_MASK         0x20000
#define PARTICLE_VIEWS             0x40000
#define PARTICLE_SCALE_WIDTH_ONLY  0x80000
#define PARTICLE_SCALE_HEIGHT_ONLY 0x100000
#define PARTICLE_MAX_SIZE          0x200000
#define PARTICLE_MIN_SIZE          0x400000

/* Spawn parameters of a sprite particle; particle_newSpriteParticleWithFloor / particle_newOwnedSpriteParticle / particle_newSpriteParticle
 * (game/Particle/SpriteParticle/spriteparticle.c) copy them to DescParticleHead.desc. */
typedef struct ParticleDesc {
    /* 0x00 */ s32 setGeometryMode;   /* math_gfxUpdateGeometryMode's setBits */
    /* 0x04 */ s32 clearGeometryMode; /* and clearBits (with 0x10000 | 0x100000 added) */
    /* 0x08 */ u16 renderMode; /* low byte: gEffectRenderModes index, high byte: particle_setCombineMode arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* gEffectTextures index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ ZoneNode *viewMask; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* gSpriteUpdateFuncs index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* gSpriteDeathFuncs index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* gSpriteDrawFuncs index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* gSpriteDestroyFuncs/gSpriteFreeFuncs/gSpriteEventFuncs index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 maxSize;
} ParticleDesc; /* size 0x70 */

/* Four particle textures to pick one from at random (ParticleDesc.texture): gFannedParticleTextures (game/Weapon/GunShot/gunshot.c) and
 * gBulletSpriteTextures (weapon_spawnBulletSprite), both 0x60-0x63. A struct, so the callers copy it to the stack in one go. */
typedef struct TextureChoices {
    /* 0x0 */ s32 texture[4];
} TextureChoices; /* size 0x10 */

/* Older revisions of the spawn descriptors. Some callers hold a shorter descriptor on the stack than the callee
 * copies: the fields at its end were added later (ParticleDesc drawFunc..maxSize, ParticleSpawn.sound,
 * ParticleDef zone..fadeRate, PuffSpray.material, TrailMotion fadeStart/fadeRate, EffectTemplate.material,
 * BloodTrailDesc mode/callback, SpriteParams.tex), and these callers were built against the older layout.
 * The callee still copies (or writes) the full size. A full type on the stack changes their frame, so they
 * keep these views, and the IDO warning 709 at the call is the cost: no cast hides it. */

/* matching: ParticleDesc before drawFunc (0x64 bytes); see above. */
typedef struct ParticleDesc64 {
    /* 0x00 */ s32 setGeometryMode;
    /* 0x04 */ s32 clearGeometryMode;
    /* 0x08 */ u16 renderMode; /* low byte: gEffectRenderModes index, high byte: particle_setCombineMode arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* gEffectTextures index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ ZoneNode *viewMask; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* gSpriteUpdateFuncs index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* gSpriteDeathFuncs index, -1 for none */
} ParticleDesc64;

/* matching: ParticleDesc before minSize (0x68 bytes); see ParticleDesc64. */
typedef struct ParticleDesc68 {
    /* 0x00 */ s32 setGeometryMode;
    /* 0x04 */ s32 clearGeometryMode;
    /* 0x08 */ u16 renderMode; /* low byte: gEffectRenderModes index, high byte: particle_setCombineMode arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* gEffectTextures index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ ZoneNode *viewMask; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* gSpriteUpdateFuncs index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* gSpriteDeathFuncs index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* gSpriteDrawFuncs index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* gSpriteDestroyFuncs/gSpriteFreeFuncs/gSpriteEventFuncs index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
} ParticleDesc68;

/* matching: ParticleDesc before maxSize (0x6C bytes); see ParticleDesc64. */
typedef struct ParticleDesc6C {
    /* 0x00 */ s32 setGeometryMode;
    /* 0x04 */ s32 clearGeometryMode;
    /* 0x08 */ u16 renderMode; /* low byte: gEffectRenderModes index, high byte: particle_setCombineMode arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* gEffectTextures index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ ZoneNode *viewMask; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* gSpriteUpdateFuncs index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* gSpriteDeathFuncs index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* gSpriteDrawFuncs index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* gSpriteDestroyFuncs/gSpriteFreeFuncs/gSpriteEventFuncs index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
} ParticleDesc6C;

/* Spawn template of a model (debris) particle passed to particle_newModelParticle (game/Particle/ModelParticle/modelparticle.c):
 * the layout of a ModelParticle's bytes 0x10..0x8B. */
typedef struct ParticleSpawn {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ Vec3f rot;
    /* 0x1C */ Vec3f scale;
    /* 0x28 */ Vec3f pos;
    /* 0x34 */ Vec3f vel;
    /* 0x40 */ Vec3f rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ ZoneNode *zone; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 lightMode;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
} ParticleSpawn; /* size 0x7C */

/* matching: ParticleSpawn before sound (0x78 bytes; particle_newModelParticleWithFloor copies 0x7C); see ParticleDesc64. */
typedef struct ParticleSpawn78 {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ Vec3f rot;
    /* 0x1C */ Vec3f scale;
    /* 0x28 */ Vec3f pos;
    /* 0x34 */ Vec3f vel;
    /* 0x40 */ Vec3f rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ ZoneNode *zone; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 lightMode;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

/* Point lights attached to a sprite or particle, applied through math_emitPointLights (light_pickLights and
 * light_reemitLights). */
typedef struct ModelLink {
    /* 0x00 */ s32 maxLights;              /* light_pickLights's maxLights; 0 = no lights */
    /* 0x04 */ u8 lightCount;              /* light_pickLights's light count */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ PointLightGfx *handles[4];  /* light_allocGfxLights buffers, one per viewport, 0..gScreenLayout */
    /* 0x18 */ Ambient *sharedHandle;      /* light_allocAmbients buffer */
    /* 0x1C */ s32 lightMask;              /* light_pickLights's mask */
} ModelLink; /* size 0x20 */

/* A pooled model (physics) particle of game/Particle/ModelParticle/modelparticle.c (at most 300 live, see gModelParticleCount). Bytes 0x10..0x8B are
 * copied from the spawn template (ParticleSpawn) passed to particle_newModelParticleWithFloor; the u8 callback fields index the
 * D_8008xxxx handler tables. Its kind's data, of the size passed to particle_newModelParticleWithFloor, follows it at 0x170
 * (MODEL_PARTICLE_DATA); a kind's own type is a ModelParticle followed by that data. */
typedef struct ModelParticle {
    /* 0x000 */ EffectHeader header;
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ Vec3f rot;
    /* 0x02C */ Vec3f scale;
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u16 model;
    /* 0x068 */ u8 variant;
    /* 0x069 */ u8 pad69[3];
    /* 0x06C */ ZoneNode *zone; /* ParticleSpawn.zone */
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 updateFn;
    /* 0x072 */ u8 update2Fn;
    /* 0x073 */ u8 floorHitFn;
    /* 0x074 */ u8 floorType3Fn;
    /* 0x075 */ u8 wallHitFn;
    /* 0x076 */ u8 destroyFn;
    /* 0x077 */ u8 squashFn;
    /* 0x078 */ u8 matrixFn;
    /* 0x079 */ s8 drawFn;
    /* 0x07A */ u8 lightMode; /* math_emitPointLights's mode (ParticleSpawn.lightMode) */
    /* 0x07B */ u8 pad7B;
    /* 0x07C */ struct Actor *owner;
    /* 0x080 */ u8 ownerId;
    /* 0x081 */ u8 pad81;
    /* 0x082 */ s16 fadeStart;
    /* 0x084 */ s16 fadeRate;
    /* 0x086 */ u8 pad86[2];
    /* 0x088 */ s32 sound; /* ParticleSpawn.sound */
    /* 0x08C */ struct ParticleNode *node;
    /* 0x090 */ Mtx mtx[2];
    /* 0x110 */ CollisionHit floor;
    /* 0x134 */ Vec3f squashOrigin;
    /* 0x140 */ f32 speed;
    /* 0x144 */ f32 invSpeed;
    /* 0x148 */ u8 hitFlags;
    /* 0x149 */ u8 squashPhase;
    /* 0x14A */ s8 squashAngle;
    /* 0x14B */ u8 pad14B;
    /* 0x14C */ ModelLink link;
    /* 0x16C */ u8 pad16C[4];
} ModelParticle; /* size 0x170 */

/* The kind's data that follows a ModelParticle. */
#define MODEL_PARTICLE_DATA(particle) ((u8 *)((particle) + 1))

/* A ModelParticle whose data is the height of the floor below it when it was spawned (the droplets of
 * game/Particle/Droplets/droplets.c, the dust of game/Particle/FootDust/footdust.c and game/Effects/BoneRing/bonering.c). */
typedef struct FloorParticle {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ f32 floorY;
} FloorParticle;

/* Per-bullet state, stored at Bullet.state (0xD0 bytes). */
typedef struct BulletState {
    /* 0x000 */ RayHit ray;
    /* 0x064 */ f32 lifetime;
    /* 0x068 */ f32 speed;
    /* 0x06C */ Actor *owner;
    /* 0x070 */ IgnoreList ignore;
    /* 0x080 */ LightEffect *trail;
    /* 0x084 */ struct SpriteParticle *follow;    /* weapon_spawnBulletSprite (game/Weapon/BulletParticle/bulletparticle.c) */
    /* 0x088 */ struct BulletParticle *particle;  /* weapon_spawnBulletParticle's particle; it follows the bullet until
                                                  * weapon_releaseBullet clears its bullet */
    /* 0x08C */ struct BloodTrail *bloodTrail;    /* weapon_startBulletTrail; NULL with three or more views (gScreenLayout >= 2) or gPlayerCount >= 4 */
    /* 0x090 */ u16 sound;
    /* 0x092 */ u8 impactType;
    /* 0x093 */ u8 pad93;
    /* 0x094 */ f32 passTimer;
    /* 0x098 */ Vec3f passPos;
    /* 0x0A4 */ CollisionHit pass;
    /* 0x0C8 */ u8 flags; /* bit 0: the trace went through a pass-through surface */
    /* 0x0C9 */ u8 padC9[3];
    /* 0x0CC */ s32 mode;
} BulletState; /* size 0xD0 */

/* A bullet: a ModelParticle (particle_newModelParticle) spawned by weapon_fireBullet (game/Weapon/Bullet/bullet.c) and updated by
 * weapon_updateBullet, with its BulletState as data. */
typedef struct Bullet {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ BulletState state;
} Bullet; /* size 0x240 */

/* A particle's width and height, copied as one struct by some callers. */
typedef struct ParticleSize {
    /* 0x0 */ f32 width;
    /* 0x4 */ f32 height;
} ParticleSize;

/* Spawn template of a particle, particle_newQuadParticle / particle_newQuadParticleNoModel / particle_newQuadParticleWhite / particle_spawnAxisQuadParticle
 * (game/Particle/QuadParticle/quadparticle.c) copy it to DefParticleHead.def. */
typedef struct ParticleDef {
    /* 0x00 */ u8 texture;      /* gEffectTextures index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the gQuadParticleEventFuncs handler */
    /* 0x02 */ u16 renderModes; /* low byte: gEffectRenderModes index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* or a point: particle_buildBendingQuad bend target, beam end, game/Effects/LandingBurst/landingburst previous position */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 alpha;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 colorLevel;   /* weight of math_pickPrimColour / math_pickEnvColour */
    /* 0x46 */ u8 primMode;     /* math_pickPrimColour's mode with flags & 0x40000 (else 4) */
    /* 0x47 */ u8 envMode;      /* math_pickEnvColour's mode with flags & 0x80000 (else 5) */
    /* 0x48 */ ZoneNode *zone;  /* collision_findZoneLeaf zone of the spawn point, or NULL; with flags & 0x1000000, drawn only in its views */
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 material;    /* math_emitPointLights's surface material, with flags & 0x4000000 */
    /* 0x54 */ s16 fadeStart;   /* lifetime below which alpha / rgba[3] fade */
    /* 0x56 */ s16 fadeRate;
} ParticleDef; /* size 0x58 */

/* matching: ParticleDef before zone (0x48 bytes); see ParticleDesc64. */
typedef struct ParticleDef48 {
    /* 0x00 */ u8 texture;      /* gEffectTextures index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the gQuadParticleEventFuncs handler */
    /* 0x02 */ u16 renderModes; /* low byte: gEffectRenderModes index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* or a point: particle_buildBendingQuad bend target, beam end, game/Effects/LandingBurst/landingburst previous position */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 alpha;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 colorLevel;
    /* 0x46 */ u8 primMode;
    /* 0x47 */ u8 envMode;
} ParticleDef48;

/* matching: ParticleDef before viewportMask (0x4C bytes); see ParticleDesc64. */
typedef struct ParticleDef4C {
    /* 0x00 */ u8 texture;      /* gEffectTextures index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the gQuadParticleEventFuncs handler */
    /* 0x02 */ u16 renderModes; /* low byte: gEffectRenderModes index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* or a point: particle_buildBendingQuad bend target, beam end, game/Effects/LandingBurst/landingburst previous position */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 alpha;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 colorLevel;
    /* 0x46 */ u8 primMode;
    /* 0x47 */ u8 envMode;
    /* 0x48 */ ZoneNode *zone; /* collision_findZoneLeaf zone of the spawn point, or NULL */
} ParticleDef4C;

/* matching: ParticleDef before material (0x50 bytes); see ParticleDesc64. */
typedef struct ParticleDef50 {
    /* 0x00 */ u8 texture;      /* gEffectTextures index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the gQuadParticleEventFuncs handler */
    /* 0x02 */ u16 renderModes; /* low byte: gEffectRenderModes index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* or a point: particle_buildBendingQuad bend target, beam end, game/Effects/LandingBurst/landingburst previous position */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 alpha;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 colorLevel;
    /* 0x46 */ u8 primMode;
    /* 0x47 */ u8 envMode;
    /* 0x48 */ ZoneNode *zone; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
} ParticleDef50;

/* matching: ParticleDef before fadeStart (0x54 bytes); see ParticleDesc64. */
typedef struct ParticleDef54 {
    /* 0x00 */ u8 texture;      /* gEffectTextures index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the gQuadParticleEventFuncs handler */
    /* 0x02 */ u16 renderModes; /* low byte: gEffectRenderModes index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* or a point: particle_buildBendingQuad bend target, beam end, game/Effects/LandingBurst/landingburst previous position */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 alpha;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 colorLevel;
    /* 0x46 */ u8 primMode;
    /* 0x47 */ u8 envMode;
    /* 0x48 */ ZoneNode *zone; /* collision_findZoneLeaf zone of the spawn point, or NULL */
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 material;
} ParticleDef54;

/* Debris emitter settings read by blood_spawnDebris (game/Effects/BloodDebris/blooddebris.c). Angles are 0..255 binary angles. */
typedef struct DebrisEmitter {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s16 lifetime;
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ f32 radius;     /* spawn points lie within this distance of pos */
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 speedBase;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 velYBase;   /* mode 0 only */
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ Vec3f dir;     /* modes 2 and 3 */
    /* 0x3C */ f32 sizeBase;
    /* 0x40 */ f32 sizeRange;
    /* 0x44 */ f32 gravityBase;
    /* 0x48 */ f32 gravityRange;
    /* 0x4C */ f32 spinBase;
    /* 0x50 */ f32 spinRange;
    /* 0x54 */ u8 landPuffs;   /* see DebrisData in game/Effects/BloodDebris/blooddebris.c */
    /* 0x55 */ u8 landPuffFlag;
    /* 0x56 */ s16 landPuffLifeBase;
    /* 0x58 */ s16 landPuffLifeRange;
} DebrisEmitter; /* size 0x5C */

/* Spawn header of an effect object, copied into EffectObj.header by effect_newEffectObj (game/Effects/EffectObj/effectobj.c);
 * the template of trails, streams and other effect_newEffectObj effects. */
typedef struct EffectTemplate {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;     /* bit 0: life counts down; 0x10: run shapeKind's handler; 0x20: pass material to the draw;
                               * 0x40: allocate as type 0x4D (else 0x22); trails: 2 attached (the strip starts
                               * at pos), 4 pos is valid, 8 fading */
    /* 0x10 */ s32 kind;      /* index into gEffectObjDestroyFuncs / gEffectObjFreeFuncs / gEffectObjMessageFuncs */
    /* 0x14 */ s8 shapeKind;  /* index into gEffectObjShapeFuncs, -1: none */
    /* 0x15 */ u8 itemCount;  /* number of items allocated after the user data */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 material; /* math_emitPointLights's surface material, with flags 0x20 */
} EffectTemplate; /* size 0x1C */

/* Spawn parameters of a screen effect, copied to ScreenEffect.desc by colourfb_createScreenEffect (game/Effects/ColourFrameBuffer/colourframebuffer.c). */
typedef struct EffectDesc {
    /* 0x0 */ u8 flags;        /* 1: ends when lifetime runs out, 2/8: hidden in some states, 4: skip one draw */
    /* 0x1 */ s8 updateIndex;  /* gScreenEffectUpdateProcs handler, -1 for none */
    /* 0x2 */ s8 drawIndex;    /* gScreenEffectDrawProcs handler, -1 for none */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 lifetime;
    /* 0x6 */ u8 messageIndex; /* gScreenEffectMessageProcs handler */
    /* 0x7 */ u8 pad7;
} EffectDesc; /* size 0x8 */

/* Spawn parameters of a blood drip, copied to BloodDrip.target by blood_spawnDrip (game/Effects/Blood/blood.c). */
typedef struct BloodDripDesc {
    /* 0x00 */ Mtx *target[2]; /* matrices to transform offset by, double-buffered */
    /* 0x08 */ u8 ownerId;     /* owner->unique_id when attached */
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ struct Actor *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ Vec3f offset;
    /* 0x20 */ f32 radius;
    /* 0x24 */ f32 rate;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ u8 flags;
    /* 0x2B */ u8 emitter;
    /* 0x2C */ s8 callback;
    /* 0x2D */ u8 handlerSet; /* index into gBloodDripDestroyHandlers / gBloodDripFreeHandlers (0 when out of range) */
    /* 0x2E */ u8 pad2E[2];
} BloodDripDesc; /* size 0x30 */

/* Blood dripping from a point, optionally attached to an actor or one of its bones (blood_spawnDrip,
 * game/Effects/Blood/blood.c). The caller's payload follows it; game/Effects/BloodEmitters/bloodemitters.c's emitters keep one byte there. */
typedef struct BloodDrip {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ BloodDripDesc desc;  /* copied from the spawn template */
    /* 0x40 */ Vec3f previous;
    /* 0x4C */ f32 inverseDiameter;
    /* 0x50 */ f32 accumulator;
    /* 0x54 */ u8 pad54[4];
    /* 0x58 */ u8 payload[1];
} BloodDrip;

/* BloodDripDesc.flags */
#define BLOOD_DRIP_FOLLOW_OWNER  0x01
#define BLOOD_DRIP_ON_BONE       0x02   /* offset is in the space of owner's bone `bone` */
#define BLOOD_DRIP_LIFETIME      0x04   /* lifetime counts down */
#define BLOOD_DRIP_NO_PREVIOUS   0x08
#define BLOOD_DRIP_STATIC        0x10

/* Glow animations (GlowParams.anim); GlowParams.type selects the handler (game/Flare/Glow/glow.c). */

/* flare_glowFlicker: random flicker */
typedef struct GlowFlicker {
    /* 0x00 */ f32 base;
    /* 0x04 */ f32 low;
    /* 0x08 */ f32 high;
    /* 0x0C */ f32 target;
    /* 0x10 */ f32 timer;
    /* 0x14 */ f32 period;
    /* 0x18 */ f32 rate;
} GlowFlicker; /* size 0x1C */

/* flare_glowPulse: sine pulse */
typedef struct GlowPulse {
    /* 0x00 */ f32 base;
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
} GlowPulse; /* size 0x10 */

/* flare_glowEnvelope: periodic rise / hold / fall */
typedef struct GlowEnvelope {
    /* 0x00 */ f32 peak;
    /* 0x04 */ f32 base;
    /* 0x08 */ f32 range;
    /* 0x0C */ f32 time;
    /* 0x10 */ f32 riseStart;
    /* 0x14 */ f32 riseEnd;
    /* 0x18 */ f32 fallStart;
    /* 0x1C */ f32 period;
    /* 0x20 */ f32 rampRate;
} GlowEnvelope; /* size 0x24 */

/* flare_glowOrbit: placed around a pivot */
typedef struct GlowOrbit {
    /* 0x00 */ f32 distance;
    /* 0x04 */ f32 spread;
    /* 0x08 */ f32 *angles;
    /* 0x0C */ f32 pivot[3];
} GlowOrbit; /* size 0x18 */

/* Shape of a glow sprite (game/Flare/Glow/glow.c): the part of GlowParams every caller of flare_spawnGlow fills in. */
typedef struct GlowShape {
    /* 0x00 */ f32 width1;   /* width, height and offset when facing the viewer (intensity 1)... */
    /* 0x04 */ f32 height1;
    /* 0x08 */ f32 width0;   /* ...and at intensity 0 */
    /* 0x0C */ f32 height0;
    /* 0x10 */ f32 offset1;
    /* 0x14 */ f32 offset0;
    /* 0x18 */ f32 alpha1;
    /* 0x1C */ f32 alpha0;
    /* 0x20 */ f32 fadeRate; /* intensity lost per unit of distance */
    /* 0x24 */ s32 screenX[4]; /* per view, -1 when off screen */
    /* 0x34 */ s32 screenY[4];
    /* 0x44 */ LensFlare *attach; /* follows the glow's position and takes its alpha; freed with flare_remove */
    /* 0x48 */ f32 scale;
    /* 0x4C */ u16 zbufSample[4]; /* z-buffer value at the glow's screen position, per view */
    /* 0x54 */ u8 visibility[4];  /* per view, eases towards 0xFF (visible) or 0 */
    /* 0x58 */ u8 type;           /* index into the gGlowDestroyHandlers/gGlowFreeHandlers/gGlowMsgHandlers handler tables */
    /* 0x59 */ s8 handler;        /* index into gGlowVisibilityTests, or -1 */
    /* 0x5A */ u8 pad5A[2];
} GlowShape; /* size 0x5C */

/* Glow parameters: flare_spawnGlow (game/Flare/Glow/glow.c) copies paramSize bytes of them into the glow at +0x110. */
typedef struct GlowParams {
    /* 0x00 */ GlowShape shape;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ union {
        GlowFlicker flicker;
        GlowPulse pulse;
        GlowEnvelope envelope;
        GlowOrbit orbit;
    } anim;
} GlowParams; /* size 0x84 */

/* Attachment of a type 0xB glow (particle_spawnBoneGlow, game/Particle/BoneGlow/boneglow.c), stored in GlowParams.anim's place at
 * glow+0x170: two points (p0, p1) given in the space of bone `bone` of `target`, which particle_followBoneGlow
 * transforms every frame. */
typedef struct EmitterAttach {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ struct Actor *target;
    /* 0x1C */ u8 bone;
    /* 0x1D */ u8 targetId;   /* the target's unique_id */
    /* 0x1E */ s8 handler;    /* index into gBoneGlowFollowFuncs, or -1 */
    /* 0x1F */ u8 pad1F;
} EmitterAttach; /* size 0x20 */

/* An RGBA color. */
typedef struct Color8 {
    /* 0x0 */ u8 red;
    /* 0x1 */ u8 green;
    /* 0x2 */ u8 blue;
    /* 0x3 */ u8 alpha;
} Color8;

/* Render settings of a trail_createMotionTrail effect (game/Effects/Trail/MotionTrail/motiontrail.c), its 12th argument, copied into the effect;
 * also ScreenSpriteInfo.render. */
typedef struct TrailRender {
    /* 0x00 */ s32 otherModeH;        /* ORed into the other-mode high word */
    /* 0x04 */ s32 setGeometryMode;
    /* 0x08 */ s32 clearGeometryMode;
    /* 0x0C */ s32 renderMode;        /* index into gEffectRenderModes */
    /* 0x10 */ s32 combiner;          /* its low byte is particle_setCombineMode's argument */
    /* 0x14 */ s32 colorDither;       /* ORed into the other-mode high word */
    /* 0x18 */ s32 alphaDither;       /* ORed into the other-mode high word */
    /* 0x1C */ s8 primMode;           /* mode of math_pickPrimColour (prim color) */
    /* 0x1D */ s8 envMode;            /* mode of math_pickEnvColour (env color) */
    /* 0x1E */ u8 pad1E[2];
} TrailRender; /* size 0x20 */

/* Spawn parameters of a screen sprite, copied to ScreenSprite.info by sprite_newScreenSprite (game/Sprite/ScreenSprite/screensprite.c). */
typedef struct ScreenSpriteInfo {
    /* 0x00 */ f32 x;           /* centre, in viewport units */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 halfWidth;
    /* 0x0C */ f32 halfHeight;
    /* 0x10 */ u8 texture;    /* index into gEffectTextures, 0xFF = none */
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 timer;     /* counts down by gFrameTicks when (flags & 1) */
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 fadeStart; /* (flags & 8): below this timer, alpha = timer * fadeRate */
    /* 0x18 */ s16 fadeRate;
    /* 0x1A */ u8 kind;       /* index into gScreenSpriteMsgHandlers */
    /* 0x1B */ Color8 color1;
    /* 0x1F */ Color8 color2;
    /* 0x23 */ u8 colorLevel;     /* weight of math_pickPrimColour / math_pickEnvColour */
    /* 0x24 */ TrailRender render;
    /* 0x44 */ u8 viewport;   /* the only viewport drawn in when (flags & 0x10) */
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 scaleX;    /* (flags & 0x80) */
    /* 0x4C */ f32 scaleY;
    /* 0x50 */ f32 offsetS;   /* (flags & 0x100) */
    /* 0x54 */ f32 offsetT;
} ScreenSpriteInfo; /* size 0x58 */

/* BillboardTemplate.flags (billboard particles, game/Effects/ObjectList/objectlist.c) */
#define BILLBOARD_ENV_COLOR     0x1    /* use envR/G/B instead of the per-texture default */
#define BILLBOARD_PINGPONG      0x2    /* reverse the animation at either end instead of wrapping */
#define BILLBOARD_FLIP_S        0x4
#define BILLBOARD_FLIP_T        0x8
#define BILLBOARD_DIE_AT_END    0x20
#define BILLBOARD_DIE_AT_START  0x40
#define BILLBOARD_LOOPED        0x80   /* set once the animation has passed its last frame */
#define BILLBOARD_CUSTOM_MODE   0x100  /* render/combine modes come from unk4 */
#define BILLBOARD_2CYCLE        0x200

/* Spawn template of a billboard particle (type 5 / 0x42): effect_spawnBillboard (game/Effects/ObjectList/objectlist.c) copies it to
 * the particle at +0x10. */
typedef struct BillboardTemplate {
    /* 0x00 */ TextureDesc *anim;
    /* 0x04 */ s32 unk4;       /* BILLBOARD_CUSTOM_MODE: bits 0-7 render mode, bits 16-31 combine mode;
                                  else free for the particle's callback */
    /* 0x08 */ s32 unk8;       /* low byte: angle offset in 1/256ths of a turn */
    /* 0x0C */ s16 frame;      /* 8.8 fixed point frame of anim */
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 unk1A;      /* free for the particle's callback */
    /* 0x1C */ u8 subX;        /* fractional (1/256) parts of x, z, y */
    /* 0x1D */ u8 subZ;
    /* 0x1E */ u8 subY;
    /* 0x1F */ s8 callback;    /* index into gParticleCallbacks, -1 = none */
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;      /* < 0: scaled by the viewing angle */
    /* 0x26 */ s16 height;     /* < 0: scaled by the viewing angle */
    /* 0x28 */ s16 life;       /* 0x7FFF = forever */
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 primR;
    /* 0x2D */ u8 primG;
    /* 0x2E */ u8 primB;
    /* 0x2F */ u8 primA;
    /* 0x30 */ u8 envR;
    /* 0x31 */ u8 envG;
    /* 0x32 */ u8 envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ u16 flags;      /* BILLBOARD_* */
    /* 0x36 */ u8 pad36[2];
} BillboardTemplate; /* size 0x38 */

/* Type 5 / 0x42 object: a moving, animated billboard particle (created by effect_spawnBillboard in game/Effects/ObjectList/objectlist.c).
 * A spawner's extra data follows it (particle + 1). */
typedef struct BillboardParticle {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ BillboardTemplate tmpl;
    /* 0x48 */ s8 onDeath;      /* index into gObjectCallbacks, -1 = none */
    /* 0x49 */ u8 pad49[7];
} BillboardParticle; /* size 0x50 */

/* Random spawn angles: yaw = yawBase + rand % (yawRange + 1), likewise pitch (256-step binary angles).
 * Most spawn configs of game/Particle/BurstConfigs/burstconfigs.c store random ranges as base/range pairs like this. */
typedef struct AngleRange {
    /* 0x0 */ s16 yawBase;
    /* 0x2 */ s16 yawRange;
    /* 0x4 */ s16 pitchBase;
    /* 0x6 */ s16 pitchRange;
} AngleRange; /* size 0x8 */

/* TrailBurstParams.flags */
#define TRAIL_BURST_TWO_CYCLE 0x1
#define TRAIL_BURST_FADE      0x4   /* alpha fades once life < fadeStart */
#define TRAIL_BURST_SHRINK    0x8   /* width shrinks once life < shrinkStart */

/* Per-trail settings, copied into the trail (FallingTrail.data) by trail_spawnFallingTrail (game/Effects/Trail/FallingTrail/fallingtrail.c). */
typedef struct TrailBurstParams {
    /* 0x00 */ f32 width;     /* half-width of the ribbon */
    /* 0x04 */ Vec3f vel;     /* initial point velocity; only vel.y is kept per point */
    /* 0x10 */ f32 gravity;
    /* 0x14 */ u8 flags;      /* TRAIL_BURST_* */
    /* 0x15 */ u8 primMode;
    /* 0x16 */ u8 envMode;
    /* 0x17 */ Color8 color;
    /* 0x1B */ Color8 color2;
    /* 0x1F */ u8 colorLevel;
    /* 0x20 */ u8 renderMode; /* index into gEffectRenderModes */
    /* 0x21 */ u8 combiner;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s32 setGeometryMode;
    /* 0x28 */ s32 clearGeometryMode;
    /* 0x2C */ s16 fadeStart;  /* life below which the alpha fades */
    /* 0x2E */ s16 fadeRate;   /* alpha per unit of life */
    /* 0x30 */ s16 shrinkStart; /* life below which the width shrinks */
    /* 0x32 */ u8 shrinkPaused;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ f32 shrinkFactor; /* width multiplier per frame */
    /* 0x38 */ s8 moveFn;     /* index into gFallingTrailMoveProcs, -1: none */
    /* 0x39 */ s8 shapeFn;    /* index into gFallingTrailShapeProcs, -1: none */
    /* 0x3A */ u8 pad3A[2];
} TrailBurstParams; /* size 0x3C */

/* particle_spawnTrailBurst (game/Particle/BurstConfigs/burstconfigs.c): spawns countBase + rand % (countRange + 1) trails (trail_spawnFallingTrail) at pos,
 * each with random size, gravity, speed, direction, item count, life and colors from these base/range pairs. */
typedef struct TrailBurst {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 gravityBase;
    /* 0x20 */ f32 gravityRange;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ AngleRange angles;
    /* 0x34 */ s32 capacityBase;   /* EffectTemplate.itemCount */
    /* 0x38 */ s32 capacityRange;
    /* 0x3C */ s16 lifeBase;
    /* 0x3E */ s16 lifeRange;
    /* 0x40 */ u16 headerFlags;    /* EffectTemplate.flags */
    /* 0x42 */ u8 flags;           /* TrailBurstParams.flags */
    /* 0x43 */ u8 primMode;
    /* 0x44 */ u8 envMode;
    /* 0x45 */ Color8 colorBase;
    /* 0x49 */ Color8 colorRange;
    /* 0x4D */ Color8 color2Base;
    /* 0x51 */ Color8 color2Range;
    /* 0x55 */ u8 colorLevelBase;
    /* 0x56 */ u8 colorLevelRange;
    /* 0x57 */ u8 renderMode;
    /* 0x58 */ u8 combiner;        /* TrailBurstParams.combiner */
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 setGeometryMode;
    /* 0x60 */ s32 clearGeometryMode;
    /* 0x64 */ s16 fadeStart;
    /* 0x66 */ s16 fadeRate;
    /* 0x68 */ s16 shrinkStart;
    /* 0x6A */ u8 shrinkPaused;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 shrinkFactor;
    /* 0x70 */ s8 moveFn;          /* TrailBurstParams.moveFn */
    /* 0x71 */ s8 shapeFn;         /* TrailBurstParams.shapeFn */
    /* 0x72 */ s8 unk72;           /* not read by particle_spawnTrailBurst; callers store -1 */
    /* 0x73 */ s8 unk73;
} TrailBurst; /* size 0x74 */

/* particle_spawnSplashSprayAtAngles / particle_spawnSplashSpray / particle_spawnSplashSprayOnTri (game/Particle/BurstConfigs/burstconfigs.c): throws countBase + rand % (countRange + 1)
 * splashes (particle_spawnSplash, game/Particle/Steam/steam.c) from pos, each value drawn from its base/range pair. */
typedef struct SplashSpray {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 scaleBase;
    /* 0x10 */ f32 scaleRange;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ s16 itemCountBase;   /* EffectTemplate.itemCount of each splash */
    /* 0x2A */ s16 itemCountRange;
    /* 0x2C */ s16 lifeBase;
    /* 0x2E */ s16 lifeRange;
    /* 0x30 */ s16 alphaBase;       /* TrailMotion.alpha of each splash (particle_spawnSplash's alpha) */
    /* 0x32 */ s16 alphaRange;      /* its range */
    /* 0x34 */ u8 kind;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 burstChance;     /* probability of particle_spawnSplash's hitsFloor: burst and spray hit handlers on landing */
    /* 0x3C */ s16 fadeStart;       /* TrailMotion.fadeStart (particle_spawnSplash's fadeStart) */
    /* 0x3E */ s16 fadeRate;        /* TrailMotion.fadeRate (particle_spawnSplash's fadeRate) */
    /* 0x40 */ s32 material;        /* particle_spawnSplash's material (EffectTemplate.material) */
} SplashSpray; /* size 0x44 */

/* What the callers of particle_spawnSplashSprayAtAngles build: the direction ranges followed by the spray. */
typedef struct SplashSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ SplashSpray spray;
} SplashSpraySpec; /* size 0x4C */

/* particle_spawnDropSprayAtAngles / particle_spawnDropSpray / particle_spawnDropSprayOnTri (game/Particle/BurstConfigs/burstconfigs.c): throws countBase + rand % (countRange + 1)
 * wobbling drops (particle_spawnSteamParticle, game/Particle/Steam/steam.c) from pos, each value drawn from its base/range pair. */
typedef struct DropSpray {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 countBase;
    /* 0x0E */ s16 countRange;
    /* 0x10 */ f32 speedBase;
    /* 0x14 */ f32 speedRange;
    /* 0x18 */ s16 lifeBase;
    /* 0x1A */ s16 lifeRange;
    /* 0x1C */ f32 gravityBase;
    /* 0x20 */ f32 gravityRange;
    /* 0x24 */ u8 alphaBase;        /* ParticleDesc.alpha of each drop (particle_spawnSteamParticle's alpha) */
    /* 0x25 */ u8 alphaRange;       /* its range */
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ f32 sizeBase;
    /* 0x2C */ f32 sizeRange;
    /* 0x30 */ s8 unk30;            /* not read by the spray functions */
    /* 0x31 */ u8 kind;
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ f32 floorHitChance;  /* probability of particle_spawnSteamParticle's hitsFloor */
    /* 0x38 */ u8 wobbles;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 bounceChance;
} DropSpray; /* size 0x40 */

/* What the callers of particle_spawnDropSprayAtAngles build: the direction ranges followed by the spray. */
typedef struct DropSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ DropSpray spray;
} DropSpraySpec; /* size 0x48 */

/* particle_spawnDebrisBurst (game/Particle/BurstConfigs/burstconfigs.c): throws countBase + rand % (countRange + 1) model pieces (particle_newModelParticle) out of a
 * sphere of the given radius around pos, each a random entry of the caller's model / scale tables. */
typedef struct DebrisBurst {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 gravityBase;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ s16 lifeBase;
    /* 0x2E */ s16 lifeRange;
    /* 0x30 */ f32 scaleBase;    /* times the variant's scale */
    /* 0x34 */ f32 scaleRange;
    /* 0x38 */ f32 spin;         /* rotVel.x / rotVel.z are random in [-spin, spin] */
} DebrisBurst; /* size 0x3C */

/* A random direction inside a cone around `direction`, built lazily by particle_completeConeBasis (game/Particle/BurstConfigs/burstconfigs.c);
 * the first argument of particle_spawnDropSpray / particle_spawnSplashSpray. */
typedef struct ConeBasis {
    /* 0x00 */ u8 flags;         /* 1: direction normalized and scaled, 2: axes built, 4: axes normalized */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f direction;
    /* 0x10 */ Vec3f axis0;
    /* 0x1C */ Vec3f axis1;
    /* 0x28 */ f32 spread;
} ConeBasis; /* size 0x2C */

/* A cone described by a surface triangle (three s16 vertices) and the side its normal faces
 * (particle_initConeFromTri); the first argument of particle_spawnDropSprayOnTri / particle_spawnSplashSprayOnTri. */
typedef struct TriangleSource {
    /* 0x00 */ Vec3f normal;
    /* 0x0C */ CollisionTri triangle;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ f32 spread;
} TriangleSource; /* size 0x24 */

/* particle_spawnJetSpray (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) jets of particles from pos
 * (particle_burstAlongDir / particle_burstAtYawPitch, game/Particle/SphereEmitter/sphereemitter.c), each with particleCount particles whose speed falls off
 * over the jet. */
typedef struct JetSpray {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedRange;
    /* 0x1C */ f32 sizeBase;
    /* 0x20 */ f32 sizeRange;
    /* 0x24 */ f32 sizeSlopeBase;
    /* 0x28 */ f32 sizeSlopeRange;
    /* 0x2C */ s32 particleCountBase;
    /* 0x30 */ s32 particleCountRange;
    /* 0x34 */ f32 spread;
    /* 0x38 */ f32 drag;
    /* 0x3C */ f32 gravityBase;
    /* 0x40 */ f32 gravityRange;
    /* 0x44 */ s16 delayBase;
    /* 0x46 */ s16 delayRange;
    /* 0x48 */ s16 alphaBase;
    /* 0x4A */ s16 alphaRange;
    /* 0x4C */ s16 fadeStart;
    /* 0x4E */ s16 fadeOutStart;
    /* 0x50 */ s8 eventFunc;      /* index into gDragFadeEventFuncs, -1: none */
    /* 0x51 */ u8 pad51[3];
} JetSpray; /* size 0x54 */

/* particle_spawnJetSprayBursts: jets in random yaw/pitch directions from `angles`. */
typedef struct JetSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ JetSpray spray;
} JetSpraySpec; /* size 0x5C */

/* particle_spawnDirectedJetSpray: jets inside a cone of the given spread around `direction`. */
typedef struct DirectedJetSpray {
    /* 0x00 */ Vec3f direction;
    /* 0x0C */ f32 spread;
    /* 0x10 */ JetSpray spray;
} DirectedJetSpray; /* size 0x64 */

/* particle_spawnSurfaceJetSpray: jets inside a cone around the normal of a surface triangle. */
typedef struct SurfaceJetSpray {
    /* 0x00 */ TriangleSource source;
    /* 0x24 */ JetSpray spray;
} SurfaceJetSpray; /* size 0x78 */

/* particle_spawnPuffSpray (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) puffs (trail_createMotionTrail effects) at pos,
 * each moving at a random yaw/pitch from `angles`, with random size, growth, speed, item count, life and
 * alpha. */
typedef struct PuffSpray {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ AngleRange angles;
    /* 0x30 */ s16 itemCountBase;   /* EffectTemplate.itemCount */
    /* 0x32 */ s16 itemCountRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 headerFlags;      /* EffectTemplate.flags */
    /* 0x3A */ u8 red;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ s32 handlers[6];     /* trail_createMotionTrail's arguments 3..8 (state and handler indices) */
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x57 */ u8 pad57;
    /* 0x58 */ s32 maxLights;       /* trail_createMotionTrail's arg9 (ModelLink.maxLights) */
    /* 0x5C */ s32 lightMask;       /* trail_createMotionTrail's arg10 (ModelLink.lightMask) */
    /* 0x60 */ TrailRender render;
    /* 0x80 */ s16 fadeStart;       /* TrailMotion.fadeStart */
    /* 0x82 */ s16 fadeRate;        /* TrailMotion.fadeRate */
    /* 0x84 */ s32 material;        /* EffectTemplate.material */
} PuffSpray; /* size 0x88 */

/* matching: PuffSpray before material (0x84 bytes; particle_spawnPuffSpray reads material); see ParticleDesc64. */
typedef struct PuffSpray84 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ AngleRange angles;
    /* 0x30 */ s16 itemCountBase;   /* EffectTemplate.itemCount */
    /* 0x32 */ s16 itemCountRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 headerFlags;      /* EffectTemplate.flags */
    /* 0x3A */ u8 red;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ s32 handlers[6];     /* trail_createMotionTrail's arguments 3..8 (state and handler indices) */
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x57 */ u8 pad57;
    /* 0x58 */ s32 maxLights;       /* trail_createMotionTrail's arg9 (ModelLink.maxLights) */
    /* 0x5C */ s32 lightMask;       /* trail_createMotionTrail's arg10 (ModelLink.lightMask) */
    /* 0x60 */ TrailRender render;
    /* 0x80 */ s16 fadeStart;       /* TrailMotion.fadeStart */
    /* 0x82 */ s16 fadeRate;        /* TrailMotion.fadeRate */
} PuffSpray84;

/* particle_spawnSurfacePuffSpray (game/Particle/BurstConfigs/burstconfigs.c): like PuffSpray, but each puff moves in a random direction inside the cone
 * around a surface triangle's normal. */
typedef struct SurfacePuffSpray {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ TriangleSource source;
    /* 0x30 */ f32 sizeBase;
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ f32 growthBase;
    /* 0x3C */ f32 growthRange;
    /* 0x40 */ f32 speedBase;
    /* 0x44 */ f32 speedRange;
    /* 0x48 */ s16 countBase;
    /* 0x4A */ s16 countRange;
    /* 0x4C */ s16 itemCountBase;   /* EffectTemplate.itemCount */
    /* 0x4E */ s16 itemCountRange;
    /* 0x50 */ s16 lifeBase;
    /* 0x52 */ s16 lifeRange;
    /* 0x54 */ u8 green;
    /* 0x55 */ u8 headerFlags;      /* EffectTemplate.flags */
    /* 0x56 */ u8 red;
    /* 0x57 */ u8 pad57;
    /* 0x58 */ s32 handlers[6];     /* trail_createMotionTrail's arguments 3..8 (state and handler indices) */
    /* 0x70 */ u8 blue;
    /* 0x71 */ u8 alphaBase;
    /* 0x72 */ u8 alphaRange;
    /* 0x73 */ u8 pad73;
    /* 0x74 */ s32 maxLights;       /* trail_createMotionTrail's maxLights */
    /* 0x78 */ s32 lightMask;       /* trail_createMotionTrail's lightMask */
    /* 0x7C */ TrailRender render;
    /* 0x9C */ s16 fadeStart;       /* TrailMotion.fadeStart */
    /* 0x9E */ s16 fadeRate;        /* TrailMotion.fadeRate */
    /* 0xA0 */ s32 material;        /* EffectTemplate.material */
} SurfacePuffSpray; /* size 0xA4 */

/* particle_spawnSpriteBurst (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) sprite particles (particle_newSpriteParticleWithFloor) at pos,
 * each moving at a random yaw/pitch; the ParticleDesc fields are copied, the base/range pairs drawn per
 * particle, and `extra` is copied into each particle at +0xA8. Note the yaw/pitch order differs from
 * AngleRange. */
typedef struct SpriteBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ u8 texture;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 renderMode;
    /* 0x08 */ s32 setGeometryMode; /* ParticleDesc.setGeometryMode */
    /* 0x0C */ s32 clearGeometryMode; /* ParticleDesc.clearGeometryMode */
    /* 0x10 */ s16 timerBase;
    /* 0x12 */ s16 timerRange;
    /* 0x14 */ s32 frameRateBase;
    /* 0x18 */ s32 frameRateRange;
    /* 0x1C */ u8 r;
    /* 0x1D */ u8 g;
    /* 0x1E */ u8 b;
    /* 0x1F */ u8 a;
    /* 0x20 */ u8 r2;
    /* 0x21 */ u8 g2;
    /* 0x22 */ u8 b2;
    /* 0x23 */ u8 alphaBase;
    /* 0x24 */ u8 alphaRange;
    /* 0x25 */ u8 brightness;
    /* 0x26 */ s16 fadeStart;
    /* 0x28 */ s16 fadeRate;
    /* 0x2A */ s16 scaleStart;
    /* 0x2C */ f32 scaleRate;
    /* 0x30 */ f32 sizeBase;        /* width and height */
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ Vec3f pos;
    /* 0x44 */ s16 yawBase;
    /* 0x46 */ s16 pitchBase;
    /* 0x48 */ s16 yawRange;
    /* 0x4A */ s16 pitchRange;
    /* 0x4C */ f32 speedBase;
    /* 0x50 */ f32 speedRange;
    /* 0x54 */ f32 gravityBase;
    /* 0x58 */ f32 gravityRange;
    /* 0x5C */ s32 flags;           /* PARTICLE_* */
    /* 0x60 */ s8 updateFunc;
    /* 0x61 */ s8 deathFunc;
    /* 0x62 */ u8 primColorMode;
    /* 0x63 */ u8 envColorMode;
    /* 0x64 */ u8 kind;             /* particle_newSpriteParticleWithFloor's kind: 0, 1 or 2 */
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 extra;           /* copied into each particle at +0xA8 */
} SpriteBurst; /* size 0x6C */

/* particle_spawnSimplePuffSpray (game/Particle/BurstConfigs/burstconfigs.c): PuffSpray with a fixed alpha, no per-puff alpha range and fixed render
 * settings. */
typedef struct SimplePuffSpray {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ AngleRange angles;
    /* 0x30 */ s16 itemCountBase;   /* EffectTemplate.itemCount */
    /* 0x32 */ s16 itemCountRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 red;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ s32 handlers[6];     /* trail_createMotionTrail's arguments 3..8 (state and handler indices) */
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alpha;
    /* 0x56 */ u8 pad56[2];
} SimplePuffSpray; /* size 0x58 */

/* particle_spawnDebrisSpray (game/Particle/BurstConfigs/burstconfigs.c): DebrisBurst whose pieces each use a different entry of the variant tables,
 * and bounce (floor-hit handler floorHitFn) with probability floorHitChance. */
typedef struct DebrisSpray {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 gravityBase;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ s16 lifeBase;
    /* 0x2E */ s16 lifeRange;
    /* 0x30 */ f32 scaleBase;       /* times the variant's scale */
    /* 0x34 */ f32 scaleRange;
    /* 0x38 */ f32 spin;            /* rotVel.x / rotVel.z are random in [-spin, spin] */
    /* 0x3C */ f32 floorHitChance;
    /* 0x40 */ u8 floorHitFn;       /* ParticleSpawn.floorHitFn of the pieces that bounce */
    /* 0x41 */ u8 pad41[3];
} DebrisSpray; /* size 0x44 */

/* particle_spawnDecalSpray (game/Particle/BurstConfigs/burstconfigs.c): casts countBase + rand % (countRange + 1) rays from pos in random directions
 * and spawns a particle_spawnKindEmitter emitter on each suitable surface hit closer than maxDistance. */
typedef struct DecalSpray {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 maxDistance;
    /* 0x20 */ f32 sizeBase;
    /* 0x24 */ f32 sizeRange;
    /* 0x28 */ s16 alphaBase;       /* SpriteDef.alpha of each emitter (particle_spawnKindEmitter's alpha) */
    /* 0x2A */ s16 alphaRange;      /* its range */
    /* 0x2C */ u8 kind;
    /* 0x2D */ u8 pad2D[3];
} DecalSpray; /* size 0x30 */

/* particle_spawnPieceSpray (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) randomly rotated pieces (effect_spawnTumblingPiece)
 * flying out of pos. */
typedef struct PieceSpray {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedRange;
    /* 0x1C */ AngleRange angles;
    /* 0x24 */ f32 directionBase;   /* length of the direction vector */
    /* 0x28 */ f32 directionRange;
    /* 0x2C */ f32 jitter;          /* x/z jitter is random in [-jitter, jitter] */
    /* 0x30 */ f32 sizeBase;
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ f32 scale;
    /* 0x3C */ u8 kind;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ s16 lifeBase;
    /* 0x40 */ s16 lifeRange;
} PieceSpray; /* size 0x44 */

/* particle_spawnSurfaceDebrisBurst (game/Particle/BurstConfigs/burstconfigs.c): DebrisBurst thrown off a surface triangle, inside the cone around its
 * normal. */
typedef struct SurfaceDebrisBurst {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ TriangleSource source;
    /* 0x38 */ f32 speedBase;
    /* 0x3C */ f32 speedRange;
    /* 0x40 */ f32 gravityBase;
    /* 0x44 */ f32 gravityRange;
    /* 0x48 */ s16 lifeBase;
    /* 0x4A */ s16 lifeRange;
    /* 0x4C */ f32 scaleBase;       /* times the variant's scale */
    /* 0x50 */ f32 scaleRange;
    /* 0x54 */ f32 spin;            /* rotVel.x / rotVel.z are random in [-spin, spin] */
} SurfaceDebrisBurst; /* size 0x58 */

/* particle_spawnStreakBurst (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) streaks (particle_newOwnedStreakParticle) at pos, each
 * pointing at a random yaw and pitch. Note the yaw/pitch order differs from AngleRange. */
typedef struct StreakBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s16 yawBase;
    /* 0x12 */ s16 pitchBase;
    /* 0x14 */ s16 yawRange;
    /* 0x16 */ s16 pitchRange;
    /* 0x18 */ f32 headSpeedBase;
    /* 0x1C */ f32 headSpeedRange;
    /* 0x20 */ f32 tailSpeedBase;
    /* 0x24 */ f32 tailSpeedRange;
    /* 0x28 */ Color8 headColor;
    /* 0x2C */ Color8 tailColor;
    /* 0x30 */ u8 flags;
    /* 0x31 */ u8 pad31;
    /* 0x32 */ s16 lifetimeBase;
    /* 0x34 */ s16 lifetimeRange;
    /* 0x36 */ u16 renderMode;
    /* 0x38 */ f32 widthBase;
    /* 0x3C */ f32 widthRange;
    /* 0x40 */ u8 alphaBase;
    /* 0x41 */ u8 alphaRange;
    /* 0x42 */ s16 fadeStart;
    /* 0x44 */ s16 fadeRate;
    /* 0x46 */ u8 kind;
    /* 0x47 */ u8 pad47;
} StreakBurst; /* size 0x48 */

/* effect_newModelEffect (game/Effects/ModelEffect/modeleffect.c): spawn parameters of a model effect, copied into the effect. */
typedef struct EffectParams {
    /* 0x00 */ u8 flags;
    /* 0x01 */ s8 updateFunc;      /* index into gModelEffectUpdateFuncs, -1 = none */
    /* 0x02 */ s8 updateFunc2;     /* index into gModelEffectUpdate2Funcs, -1 = none */
    /* 0x03 */ s8 preDrawFunc;     /* index into gModelEffectPreDrawFuncs, -1 = none */
    /* 0x04 */ s8 postDrawFunc;    /* index into gModelEffectPostDrawFuncs, -1 = none */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 lifetime;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 bank;
    /* 0x10 */ TrailRender render;
    /* 0x30 */ Color8 color0;
    /* 0x34 */ Color8 color1;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ struct Actor *owner; /* see effect_handleOwnerActorEvent */
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 lightMode;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f pos;
    /* 0x50 */ ZoneNode *mask;    /* with flags & 8: drawn only in the views that see this zone */
    /* 0x54 */ s16 fadeStart;      /* alpha fades once lifetime drops below this */
    /* 0x56 */ s16 fadeRate;
} EffectParams; /* size 0x58 */

/* particle_spawnBillboardBurst (game/Particle/BurstConfigs/burstconfigs.c): spawns countBase + rand % (countRange + 1) billboards (effect_spawnBillboard) at
 * position, each moving at a random yaw/pitch and speed. Each *Base/*Range pair is one BillboardTemplate field. */
typedef struct BillboardBurst {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ s16 countBase;
    /* 0x0E */ s16 countRange;
    /* 0x10 */ AngleRange angles;
    /* 0x18 */ f32 speedBase;
    /* 0x1C */ f32 speedRange;
    /* 0x20 */ s16 lifeBase;
    /* 0x22 */ s16 lifeRange;
    /* 0x24 */ s16 accelYBase;
    /* 0x26 */ s16 accelYRange;
    /* 0x28 */ s8 callback;        /* BillboardTemplate.callback */
    /* 0x29 */ u8 flags;           /* BILLBOARD_* */
    /* 0x2A */ s16 fadeBase;
    /* 0x2C */ s16 fadeRange;
    /* 0x2E */ s16 frameStep;
    /* 0x30 */ s16 widthBase;
    /* 0x32 */ s16 widthRange;
    /* 0x34 */ s16 heightBase;
    /* 0x36 */ s16 heightRange;
    /* 0x38 */ TextureDesc *anim;
    /* 0x3C */ u8 color;           /* prim r = g = b */
    /* 0x3D */ u8 alpha;
    /* 0x3E */ u8 pad3E[2];
} BillboardBurst; /* size 0x40 */

/* Motion extension of a model effect: effect_newMovingModelEffect (game/Effects/ModelEffect/modeleffect.c) copies it to ModelEffect.extra, and the
 * gModelEffectUpdateFuncs / gModelEffectUpdate2Funcs update callbacks integrate it. */
typedef struct EffectMotion {
    /* 0x00 */ Vec3f rot;          /* degrees */
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 drag;
    /* 0x30 */ u8 flags;           /* EFFECT_MOTION_* */
} EffectMotion; /* size 0x34 */

#define EFFECT_MOTION_MOVE      0x1
#define EFFECT_MOTION_SPIN      0x2
#define EFFECT_MOTION_GRAVITY   0x4
#define EFFECT_MOTION_DRAG      0x8

typedef union EffectExtra {
    f32 raw[0xE];
    EffectMotion motion;
} EffectExtra; /* size 0x38 */


/* Model effect object (types 0x36 / 0x4C / 0x5B), made by effect_newModelEffect / effect_newMovingModelEffect (game/Effects/ModelEffect/modeleffect.c). */
typedef struct ModelEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x010 */ EffectParams params;
    /* 0x068 */ AnimInstance *model; /* created by anim_createInstance */
    /* 0x06C */ Gfx **displayLists;
    /* 0x070 */ u8 displayListCount;
    /* 0x071 */ u8 pad71[3];
    /* 0x074 */ u8 *segment1; /* the model's vertex data */
    /* 0x078 */ s32 unk78;
    /* 0x07C */ MtxF mtx[2]; /* per frame */
    /* 0x0FC */ ModelLink attach;
    /* 0x11C */ u8 pad11C[4];
    /* 0x120 */ EffectExtra extra; /* optional extension (effect_newMovingModelEffect) */
} ModelEffect; /* size 0x120 + the optional extension */

/* blood_spawnStrip (game/Effects/Blood/blood.c): spawn parameters of a blood strip (object type 0x29), copied to the strip
 * at +0x18. Every duration frames (after pause) offsetA/offsetB are sampled in bone space. */
typedef struct BloodStripDesc {
    /* 0x00 */ u8 ownerId;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ struct Actor *owner;
    /* 0x08 */ u8 bone;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ Vec3f offsetA;
    /* 0x18 */ Vec3f offsetB;
    /* 0x24 */ u8 flags;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 duration;
    /* 0x28 */ s16 pause;
    /* 0x2A */ s16 lifetime;
    /* 0x2C */ u16 sound;
    /* 0x2E */ s8 sampleCallback;   /* index into gBloodStripSampleCallbacks, -1 = none */
    /* 0x2F */ s8 emitCallback;     /* index into gBloodStripEmitCallbacks */
    /* 0x30 */ s8 rateCallback;     /* index into gBloodStripRateCallbacks, -1 = none */
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ f32 rate;
    /* 0x38 */ u8 mode;
    /* 0x39 */ s8 updateCallback;   /* index into gBloodStripUpdateCallbacks, -1 = none */
    /* 0x3A */ u8 pad3A[2];
} BloodStripDesc; /* size 0x3C */

/*
 * Blood swept between two points (offsetA, offsetB) on an actor's bone (object type 0x29, spawned by
 * blood_spawnStrip in game/Effects/Blood/blood.c): every frame the points are transformed and the emit callback runs at
 * steps between the old and new spots. Its fields ownerId..updateCallback are a copy of BloodStripDesc.
 */
typedef struct BloodStrip {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 active;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 ownerId;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ struct Actor *owner;
    /* 0x20 */ u8 bone;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ Vec3f offsetA;
    /* 0x30 */ Vec3f offsetB;
    /* 0x3C */ u8 flags;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ s16 duration;   // frames of emission after each sample
    /* 0x40 */ s16 pause;      // frames to wait (below 0) before the next sample
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ u16 sound;      // playing sound, stopped when the strip is freed
    /* 0x46 */ s8 sampleCallback;
    /* 0x47 */ s8 emitCallback;
    /* 0x48 */ s8 rateCallback;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 rate;
    /* 0x50 */ u8 mode;        // selects the message handler
    /* 0x51 */ s8 updateCallback;
    /* 0x52 */ u8 pad52[2];
    /* 0x54 */ s16 timer;
    /* 0x56 */ u8 pad56[2];
    /* 0x58 */ Vec3f posA;
    /* 0x64 */ Vec3f posB;
    /* 0x70 */ f32 accumulator;
    /* 0x74 */ f32 power;      // set by the rate or sample callback
    /* 0x78 */ f32 time;
} BloodStrip; /* size 0x7C; extra data may follow */

/* particle_spawnParticleBurst (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) weapon_spawnDragSprite particles (game/Weapon/SegmentShot/segmentshot.c)
 * from pos, each at a random yaw (the yaw fields are unused) and a pitch/speed from the ranges below. */
typedef struct ParticleBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ Vec3f *pos;
    /* 0x08 */ s16 yawBase;         /* unused */
    /* 0x0A */ s16 pitchBase;
    /* 0x0C */ s16 yawRange;        /* unused */
    /* 0x0E */ s16 pitchRange;
    /* 0x10 */ f32 speedBase;
    /* 0x14 */ f32 speedRange;
    /* 0x18 */ f32 gravityBase;     /* ParticleDesc.gravity */
    /* 0x1C */ f32 gravityRange;
    /* 0x20 */ f32 drag;            /* weapon_spawnDragSprite: the particle's data.dragBounce.drag */
    /* 0x24 */ u8 alphaBase;        /* ParticleDesc.alpha */
    /* 0x25 */ u8 alphaRange;
    /* 0x26 */ s16 timerBase;       /* ParticleDesc.timer; -1 = 300 */
    /* 0x28 */ s16 timerRange;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 sizeBase;
    /* 0x30 */ f32 sizeRange;
    /* 0x34 */ f32 altChance;       /* probability of the alternative update function */
    /* 0x38 */ void *ground;        /* particle_newSpriteParticleWithFloor's ground argument */
} ParticleBurst; /* size 0x3C */

/* particle_spawnSurfaceBurst (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) particle_newQuadParticle particles at position,
 * each launched inside a cone of half-width `spread` around the normal of `triangle` (flipped to face
 * `normal`). */
typedef struct SurfaceBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ Vec3f normal;
    /* 0x1C */ CollisionTri triangle;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ f32 spread;
    /* 0x34 */ u8 texture;          /* ParticleDef.texture */
    /* 0x35 */ u8 type;             /* ParticleDef.type */
    /* 0x36 */ u16 renderModes;     /* ParticleDef.renderModes */
    /* 0x38 */ s16 lifetimeBase;
    /* 0x3A */ s16 lifetimeRange;
    /* 0x3C */ f32 scaleBase;       /* ParticleDef.width = height */
    /* 0x40 */ f32 scaleRange;
    /* 0x44 */ s32 flags;           /* ParticleDef.flags (0x08000000 is always added) */
    /* 0x48 */ u8 alphaBase;        /* ParticleDef.alpha */
    /* 0x49 */ u8 alphaRange;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ f32 gravityBase;     /* base and range of the particle's ballistic gravity (DefParticleMotion, +0x11C) */
    /* 0x50 */ f32 gravityRange;
    /* 0x54 */ f32 speedBase;
    /* 0x58 */ f32 speedRange;
    /* 0x5C */ u8 primMode;         /* ParticleDef.primMode */
    /* 0x5D */ u8 envMode;          /* ParticleDef.envMode */
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s32 maxLights;       /* particle_newQuadParticle's maxLights argument */
    /* 0x64 */ s32 lightMask;       /* particle_newQuadParticle's lightMask argument */
    /* 0x68 */ s16 fadeStart;
    /* 0x6A */ s16 fadeRate;
    /* 0x6C */ f32 drag;            /* the particle's ballistic drag (DefParticleMotion, +0x120) */
    /* 0x70 */ f32 spin;            /* spin.x / spin.z are random in [-spin, spin] */
} SurfaceBurst; /* size 0x74 */

/* particle_spawnDriftQuad (game/Particle/DriftQuad/driftquad.c): spawn description of a drifting quad (filled by particle_spawnDriftBurst). */
typedef struct DriftSpawn {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 drag;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ Color8 color;        /* alpha goes to ParticleDef.alpha */
    /* 0x24 */ s32 life;
    /* 0x28 */ ParticleSize size;
    /* 0x30 */ f32 minVy;
    /* 0x34 */ u8 pad34[4];
} DriftSpawn; /* size 0x38 */

/* particle_spawnDriftBurst (game/Particle/BurstConfigs/burstconfigs.c): countBase + rand % (countRange + 1) drifting quads (particle_spawnDriftQuad) at pos,
 * each launched at a random yaw/pitch and speed. */
typedef struct DriftBurst {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ AngleRange angles;   /* binary angles (256 per turn) */
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 drag;
    /* 0x28 */ f32 gravityBase;
    /* 0x2C */ f32 gravityRange;
    /* 0x30 */ s8 colorFunc;        /* index into gBurstColorFuncs, or -1 for opaque white */
    /* 0x31 */ u8 pad31;
    /* 0x32 */ s16 lifeBase;
    /* 0x34 */ s16 lifeRange;
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ f32 widthBase;
    /* 0x3C */ f32 heightBase;
    /* 0x40 */ f32 widthRange;
    /* 0x44 */ f32 heightRange;
    /* 0x48 */ f32 minVy;
} DriftBurst; /* size 0x4C */

/* particle_spawnDebrisScatter (game/Particle/BurstConfigs/burstconfigs.c; the direction ranges are a separate argument): throws countBase + rand %
 * (countRange + 1) model pieces (particle_newModelParticleWithFloor) from a sphere of the given radius around origin, each a random
 * entry of the variant tables, with a copy of extraData at +0x170. */
typedef struct DebrisScatter {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f origin;
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedRange;
    /* 0x1C */ f32 gravityBase;
    /* 0x20 */ f32 gravityRange;
    /* 0x24 */ s16 lifetimeBase;
    /* 0x26 */ s16 lifetimeRange;
    /* 0x28 */ f32 scaleBase;       /* times the variant's scale */
    /* 0x2C */ f32 scaleRange;
    /* 0x30 */ f32 spin;            /* rotVel.x / rotVel.z are random in [-spin, spin] */
    /* 0x34 */ void *floor;         /* particle_newModelParticleWithFloor's floor argument */
    /* 0x38 */ f32 floorHitChance;  /* pieces that bounce use floorHitFn */
    /* 0x3C */ f32 elasticityBase;
    /* 0x40 */ f32 elasticityRange;
    /* 0x44 */ f32 radiusScale;     /* ParticleSpawn.radius = radiusScale * size */
    /* 0x48 */ s32 *variantIds;
    /* 0x4C */ const f32 *variantScales;
    /* 0x50 */ u32 variantCount;
    /* 0x54 */ f32 radius;
    /* 0x58 */ u8 flags;            /* 1: attached pieces (0x800) */
    /* 0x59 */ u8 floorHitFn;       /* ParticleSpawn.floorHitFn of the pieces that bounce */
    /* 0x5A */ u8 pad5A[2];
    /* 0x5C */ s32 extraSize;
    /* 0x60 */ void *extraData;     /* copied into each piece at +0x170 */
} DebrisScatter; /* size 0x64 */

/* Spawn template of a sprite: particle_newQuadSprite / particle_spawnFlatQuadSprite (game/Particle/QuadParticle/quadparticle.c) copy it to the sprite at +0x18. */
typedef struct SpriteDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;          /* gEffectTextures index, 0xFF = untextured */
    /* 0x07 */ u8 handlerSet;       /* index into gQuadSpriteDestroyFuncs / gQuadSpriteFreeFuncs */
    /* 0x08 */ s32 frame;           /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 alpha;            /* faded out with the lifetime (flag 0x8000) */
    /* 0x11 */ u8 colorLevel;       /* weight of math_pickPrimColour / math_pickEnvColour */
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 primMode;         /* math_pickPrimColour's mode with flags & 0x100 (else 3) */
    /* 0x17 */ u8 envMode;          /* math_pickEnvColour's mode with flags & 0x200 (else 4) */
    /* 0x18 */ u32 renderModes;     /* low byte: gEffectRenderModes index, high half: combiner index */
    /* 0x1C */ s32 material;        /* math_emitPointLights's surface material, with flags & 0x4000 */
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;       /* lifetime below which alpha / rgba[3] fade */
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
} SpriteDef; /* size 0x28 */

/* matching: a 0x2C stack copy of SpriteDef (game/Particle/ShatterPieces/shatterpieces.c, game/Particle/RegionEmitter/regionemitter.c,
 * game/Beam/AreaEmitter/areaemitter.c); in shatterpieces.c the extra word keeps particle_spawnFlatMark's frame layout. */
typedef struct SpriteDef2C {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;
    /* 0x07 */ u8 handlerSet;
    /* 0x08 */ s32 frame;
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 alpha;
    /* 0x11 */ u8 colorLevel;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 primMode;
    /* 0x17 */ u8 envMode;
    /* 0x18 */ u32 renderModes;
    /* 0x1C */ s32 material;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ u8 pad28[4];
} SpriteDef2C; /* size 0x2C */

/* particle_newOwnedStreakParticle / particle_newStreakParticle (game/Particle/StreakParticle/streakparticle.c): spawn parameters of a streak particle. */
typedef struct StreakParticleDesc {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f dir;
    /* 0x18 */ f32 headSpeed;
    /* 0x1C */ f32 tailSpeed;
    /* 0x20 */ Color8 headColor;
    /* 0x24 */ Color8 tailColor;
    /* 0x28 */ u8 flags;
    /* 0x29 */ u8 pad29;
    /* 0x2A */ s16 lifetime;
    /* 0x2C */ u16 renderMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ f32 width;
    /* 0x34 */ u8 alpha;
    /* 0x35 */ u8 pad35;
    /* 0x36 */ s16 fadeStart;
    /* 0x38 */ s16 fadeRate;
    /* 0x3A */ u8 pad3A[2];
} StreakParticleDesc; /* size 0x3C */

/* Spawn parameters for each bolt of a chain (game/Beam/BoltChain/boltchain.c); "base + rand % (range + 1)" or "base + randf * range". */
typedef struct BoltChainConfig {
    /* 0x00 */ f32 widthBase;
    /* 0x04 */ f32 widthRange;
    /* 0x08 */ f32 jitter;
    /* 0x0C */ f32 bendBase;       /* distance of the inner control points from the ends */
    /* 0x10 */ f32 bendRange;
    /* 0x14 */ s16 bendSpread;     /* pitch range of the inner control points */
    /* 0x16 */ s16 rejitterBase;
    /* 0x18 */ s16 rejitterRange;
    /* 0x1A */ s16 lifeBase;
    /* 0x1C */ s16 lifeRange;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 segmentsBase;
    /* 0x24 */ s32 segmentsRange;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 alphaBase;
    /* 0x2C */ u8 alphaRange;
} BoltChainConfig; /* size 0x30 */

/* Parameters of a lightning bolt (game/Beam/BoltChain/boltchain.c): a cubic Bezier from start to end, split into jittered segments. */
typedef struct BoltDesc {
    /* 0x00 */ s16 life;
    /* 0x02 */ u8 flags;          /* bit 0: life counts down */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ Vec3f ctrl[4];     /* Bezier control points; ctrl[0] and ctrl[3] are the end points */
    /* 0x34 */ s8 updateKind;     /* index into gBoltUpdateHandlers, -1: none */
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 segments;
    /* 0x3C */ f32 jitter;        /* max sideways offset of each joint */
    /* 0x40 */ f32 width;         /* half width of the drawn strip */
    /* 0x44 */ s16 rejitterBase;  /* frames between re-jitters: base + rand % (range + 1) */
    /* 0x46 */ s16 rejitterRange;
    /* 0x48 */ Color8 color;
    /* 0x4C */ u8 kind;           /* index into gBoltCleanupHandlers / gBoltMsgHandlers */
    /* 0x4D */ u8 pad4D[3];
} BoltDesc; /* size 0x50 */

/* particle_newAnchorEmitter (game/Particle/AnchorEmitter/anchoremitter.c): emitter template (gAnchorEmitterTemplates), copied into AnchorEmitterParams after `accum`. */
typedef struct AnchorEmitterTemplate {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ f32 burstBase;
    /* 0x0C */ f32 burstRange;
    /* 0x10 */ f32 sizeX;
    /* 0x14 */ f32 sizeY;
    /* 0x18 */ f32 sizeXRange;
    /* 0x1C */ f32 sizeYRange;
    /* 0x20 */ u8 alpha;
    /* 0x21 */ u8 alphaRange;
    /* 0x22 */ s16 lifeMin;
    /* 0x24 */ s16 lifeRange;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ s32 maxCount;
    /* 0x2C */ f32 velocityScale;
    /* 0x30 */ f32 gravity;
    /* 0x34 */ f32 gravityRange;
    /* 0x38 */ f32 drag;
    /* 0x3C */ s16 fadeStart;
    /* 0x3E */ s16 fadeRate;
    /* 0x40 */ s16 launchLifeMin;
    /* 0x42 */ s16 launchLifeRange;
    /* 0x44 */ s32 anchorSet;
    /* 0x48 */ u8 flags;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 easeFactor;
} AnchorEmitterTemplate; /* size 0x50 */

typedef struct AnchorEmitterParams {
    /* 0x00 */ f32 accum;
    /* 0x04 */ AnchorEmitterTemplate tmpl;
    /* 0x54 */ LightEffect *child;
    /* 0x58 */ s8 updateFunc;
    /* 0x59 */ s8 expireFunc;
    /* 0x5A */ u8 messageFunc;
    /* 0x5B */ u8 pad5B;
} AnchorEmitterParams; /* size 0x5C */

/* Object type 0x3E (game/Effects/BoneFollower/bonefollower.c): watches an actor and owns a list of BoneFollowers. Users that need more
 * data ask effect_newFollowerGroup for extra bytes after it (AnchorEmitter). */
typedef struct ActorFollowerGroup {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ struct Actor *actor;
    /* 0x14 */ u8 actorUid;         // actor->unique_id when spawned
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 timer;
    /* 0x18 */ u8 flags;            // bit 0: timer counts down; bit 1: actor is trackable (has bone matrices, (unk74 & 0xF) != 0xF)
    /* 0x19 */ s8 updateFn;         // index into gFollowerGroupUpdateFuncs, -1 for none
    /* 0x1A */ s8 trackFn;          // index into gFollowerGroupTrackFuncs, called when bit 1 of flags sets
    /* 0x1B */ s8 untrackFn;        // index into gFollowerGroupUntrackFuncs, called when bit 1 of flags clears
    /* 0x1C */ s8 expireFn;         // index into gFollowerGroupExpireFuncs, -1 for none
    /* 0x1D */ u8 kind;             // index into gFollowerGroupDestroyFuncs and gFollowerGroupMessageFuncs
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s16 count;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ struct BoneFollower *head;
    /* 0x28 */ struct BoneFollower *tail;
    /* 0x2C */ u8 pad2C[4];
} ActorFollowerGroup; /* size 0x30 */

/* Emitter object created by particle_newAnchorEmitter (game/Particle/AnchorEmitter/anchoremitter.c): an ActorFollowerGroup (kind 1) with the emitter's data
 * after it. */
typedef struct AnchorEmitter {
    /* 0x00 */ ActorFollowerGroup group;
    /* 0x30 */ AnchorEmitterParams params;
} AnchorEmitter; /* size 0x8C */

/* trail_createMotionTrail (game/Effects/Trail/MotionTrail/motiontrail.c): motion and appearance block copied from its second argument (some
 * handlers read these words differently, e.g. game/Particle/BurstConfigs/burstconfigs.c's puffs). */
typedef struct TrailMotion {
    /* 0x00 */ f32 halfWidth;
    /* 0x04 */ Vec3f velocity;
    /* 0x10 */ f32 gravity;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 texture;          /* index into D_80091564 */
    /* 0x1A */ u8 shade;            /* passed with alpha to math_pickPrimColour / math_pickEnvColour */
    /* 0x1B */ u8 alpha;            /* faded by trail_fadeKind2Trail */
    /* 0x1C */ s16 fadeStart;       /* (flags & 0x40) once the trail's life is below this, alpha = min(alpha, life * fadeRate) */
    /* 0x1E */ s16 fadeRate;
} TrailMotion; /* size 0x20 */

/* Preset of a burst of trail_createMotionTrail particles thrown out around a point: radius and launch speed, then the
 * ranges of each particle's TrailMotion and EffectTemplate values (value = base + rand * range, or for the s16
 * pairs base + rand % (range + 1)). Tables: gScatterPresets (trail bursts, game/Effects/Trail/ScatterTrail/scattertrail.c), gJetScatterPresets (flamethrower
 * jet scatter, game/Effects/CharacterFlameThrower/characterflamethrower.c). */
typedef struct ScatterPreset {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 speed;
    /* 0x08 */ f32 speedRange;
    /* 0x0C */ f32 halfWidth;       /* TrailMotion.halfWidth */
    /* 0x10 */ f32 halfWidthRange;
    /* 0x14 */ f32 gravity;         /* TrailMotion.gravity */
    /* 0x18 */ f32 gravityRange;
    /* 0x1C */ s16 life;            /* EffectTemplate.life */
    /* 0x1E */ s16 lifeRange;
    /* 0x20 */ s16 itemCount;       /* EffectTemplate.itemCount */
    /* 0x22 */ s16 itemCountRange;
} ScatterPreset; /* size 0x24 */

/* matching: TrailMotion before fadeStart/fadeRate (0x1C bytes; trail_createMotionTrail copies 0x20); see ParticleDesc64. */
typedef struct TrailMotion1C {
    /* 0x00 */ f32 halfWidth;
    /* 0x04 */ Vec3f velocity;
    /* 0x10 */ f32 gravity;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 shade;
    /* 0x1B */ u8 alpha;
} TrailMotion1C;

/* User data of a trail_createMotionTrail trail (MotionTrail.params). */
typedef struct TrailParams {
    /* 0x00 */ TrailMotion motion;
    /* 0x20 */ u8 state;            /* handler index into gMotionTrailStateProcs */
    /* 0x21 */ u8 updateFn;         /* handler index into gMotionTrailUpdateProcs */
    /* 0x22 */ u8 finishFn;         /* handler index into gMotionTrailFinishProcs */
    /* 0x23 */ u8 hitFn;            /* handler index into gMotionTrailHitFns */
    /* 0x24 */ u8 hitKind3Fn;       /* handler index into gMotionTrailHitKind3Fns */
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ TrailRender render;
} TrailParams; /* size 0x48 */

/* matching: EffectTemplate before material (0x18 bytes; effect_newEffectObj copies 0x1C); see ParticleDesc64. */
typedef struct EffectTemplate18 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 kind;
    /* 0x14 */ s8 shapeKind;
    /* 0x15 */ u8 itemCount;
    /* 0x16 */ u8 pad16[2];
} EffectTemplate18;

/* blood_spawnTrail (game/Effects/Blood/blood.c): spawn parameters of a blood trail; copied verbatim into the effect. */
typedef struct BloodTrailDesc {
    /* 0x00 */ f32 *x;              /* followed point */
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;          /* gBloodTrailEmitters index */
    /* 0x18 */ u8 mode;             /* copied to BloodTrail.mode */
    /* 0x19 */ s8 callback;
    /* 0x1A */ u8 pad1A[2];
} BloodTrailDesc; /* size 0x1C */

/* Blood trailing behind a moving point given by three coordinate pointers (object type 0x2A, spawned by
 * blood_spawnTrail in game/Effects/Blood/blood.c); x..callback are a copy of BloodTrailDesc. Extra data may follow. */
typedef struct BloodTrail {
    /* 0x00 */ EffectHeader header; // header.kind and header.owner are passed on to the drops it emits
    /* 0x10 */ f32 *x;
    /* 0x14 */ f32 *y;
    /* 0x18 */ f32 *z;
    /* 0x1C */ f32 radius;
    /* 0x20 */ f32 rate;
    /* 0x24 */ s16 lifetime;
    /* 0x26 */ u8 flags;
    /* 0x27 */ u8 emitter;
    /* 0x28 */ u8 mode; /* 1: blood_handleTrailMessage passes messages to effect_bloodTrailOwnerMessage */
    /* 0x29 */ s8 callback;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ Vec3f previous;
    /* 0x38 */ f32 inverseDiameter;
    /* 0x3C */ f32 accumulator;
} BloodTrail; /* size 0x40 */

/* matching: BloodTrailDesc before mode/callback (0x18 bytes; blood_spawnTrail copies 0x1C); see ParticleDesc64. */
typedef struct BloodTrailDesc18 {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;
} BloodTrailDesc18;

/* particle_spawnChannelParticle (game/Particle/ChannelParticle/channelparticle.c): emitter description that it turns into a spawned particle. */
typedef struct ParticleEmitterDesc {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 maxA;
    /* 0x10 */ f32 maxB;
    /* 0x14 */ f32 minA;
    /* 0x18 */ f32 minB;
    /* 0x1C */ f32 altB;
    /* 0x20 */ s16 maxC;
    /* 0x22 */ s16 minC;
    /* 0x24 */ s16 life;
    /* 0x26 */ u8 color[4];
    /* 0x2A */ u16 flags;
    /* 0x2C */ u8 alpha;
    /* 0x2D */ u8 colorLevel;
    /* 0x2E */ u8 updateFn1;
    /* 0x2F */ u8 expireFn;
    /* 0x30 */ s32 extraSize;
    /* 0x34 */ u8 effect;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 scale;
    /* 0x3C */ f32 maxHeight;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
} ParticleEmitterDesc; /* size 0x48 */

/* trail_spawnScatterTrail (game/Effects/Trail/ScatterTrail/scattertrail.c): settings of a smoke trail; the first 0x40 bytes of its ScatterTrailState. */
typedef struct TrailStateParams {
    /* 0x00 */ Vec3f baseVel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseJitter;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedSwing;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ f32 emitRate;
    /* 0x24 */ u8 shade;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 alpha;
    /* 0x28 */ f32 widthGrowth;
    /* 0x2C */ f32 width;
    /* 0x30 */ u8 texStep;
    /* 0x31 */ u8 texStepJitter;
    /* 0x32 */ u8 fadeTailCount;
    /* 0x33 */ u8 fadeTailStep;
    /* 0x34 */ u8 fadeHeadCount;
    /* 0x35 */ u8 fadeHeadStep;
    /* 0x36 */ s16 life;
    /* 0x38 */ u8 burstPreset;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 burstRate;
} TrailStateParams; /* size 0x40 */

/* effect_newRingEffect (game/Effects/RingEffect/ringeffect.c): spawn parameters of an expanding ring effect (copied into RadialEffect.def). */
typedef struct RadialEffectDef {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 flags;            /* bit 0: expires when remaining runs out */
    /* 0x0D */ s8 checkCallback;    /* index into gRingCheckFuncs, -1 = none */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ f32 speed;           /* radius growth per tick */
    /* 0x14 */ s32 count;           /* ring segments */
    /* 0x18 */ s16 remaining;       /* ticks left */
    /* 0x1A */ s16 growUntil;
    /* 0x1C */ f32 growRate;
    /* 0x20 */ s16 shrinkFrom;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ f32 shrinkRate;
    /* 0x28 */ f32 emitRate;        /* emissions per unit of circumference per tick */
    /* 0x2C */ s8 emitCallback;     /* index into gRingEmitFuncs, -1 = none */
    /* 0x2D */ s8 updateCallback;   /* index into gRingUpdateFuncs, -1 = none */
    /* 0x2E */ u8 pad2E[2];
} RadialEffectDef; /* size 0x30 */

/* effect_newEffectActor (game/Effects/EffectActor/effectactor.c): template of an effect actor (object type 0x37 / 0x55), copied to the actor
 * at +0x10. Like EffectParams, but the actor draws one display list from gEffectActorDisplayLists. */
typedef struct EffectActorTemplate {
    /* 0x00 */ u8 flags;            /* 1: lifetime counts down, 2: attached to owner, 4: attach option */
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s8 updateCallback;   /* index into gEffectActorUpdateFuncs, -1 = none */
    /* 0x03 */ s8 drawCallback;     /* index into gEffectActorMatrixFuncs, -1 = none */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 displayList;      /* index into gEffectActorDisplayLists */
    /* 0x07 */ u8 pad07;
    /* 0x08 */ TrailRender render;
    /* 0x28 */ Color8 color0;
    /* 0x2C */ Color8 color1;
    /* 0x30 */ void *owner;
    /* 0x34 */ u8 ownerId;
    /* 0x35 */ u8 attachMode;
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ Vec3f position;
} EffectActorTemplate; /* size 0x44 */

/* EffectMotion without the flags: the motion block of the effect actors of effect_newEffectActor (game/Effects/EffectActor/effectactor.c),
 * always integrated. */
typedef struct TumbleMotion {
    /* 0x00 */ Vec3f rot;          /* degrees */
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 drag;
} TumbleMotion; /* size 0x30 */

/* Alpha fade-out read by effect_fadeEffectActor once the lifetime drops below duration. */
typedef struct EffectFade {
    /* 0x00 */ s16 duration;
    /* 0x04 */ s32 alphaPerTick;
} EffectFade; /* size 0x8 */

/* Effect actor (object type 0x37, or 0x55 when large) allocated by effect_newEffectActor (game/Effects/EffectActor/effectactor.c) through
 * effect_newObject. Bytes 0x10..0x53 are copied from its template; the type-specific data follows the
 * 0xF8-byte head. */
typedef struct EffectActor {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ EffectActorTemplate tmpl;
    /* 0x58 */ Mtx mtx[2]; /* double-buffered by gDrawBufferIndex */
    /* 0xD8 */ ModelLink attachment; /* lights drawn through math_emitPointLights */
    /* 0xF8 */ union {
        TumbleMotion motion; /* effect_moveEffectActor / effect_buildEffectActorRotScaleMtx */
        f32 size;
        EffectFade fade;     /* effect_fadeEffectActor */
        struct {
            /* 0xF8 */ EffectFade fade;
            /* 0x100 */ TumbleMotion motion;
        } piece;             /* tumbling piece of effect_spawnTumblingPiece (game/Effects/TumblingPiece/tumblingpiece.c) */
    } data;
} EffectActor; /* size 0xF8 + the type-specific data */

/* particle_spawnModelBurst (game/Particle/BurstConfigs/burstconfigs.c): spawns countBase + rand % (countRange + 1) tumbling model effects
 * (effect_newMovingModelEffect) at position, each launched at a random yaw/pitch and speed with a random spin. */
typedef struct ModelBurst {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 drag;            /* EffectMotion.drag */
    /* 0x28 */ f32 gravityBase;
    /* 0x2C */ f32 gravityRange;
    /* 0x30 */ s16 lifetimeBase;    /* EffectParams.lifetime */
    /* 0x32 */ s16 lifetimeRange;
    /* 0x34 */ s32 model;           /* EffectParams.model */
    /* 0x38 */ s32 bank;            /* EffectParams.bank */
    /* 0x3C */ s32 anim;            /* effect_newMovingModelEffect's anim (anim_play's 3rd argument) */
    /* 0x40 */ f32 animSpeedBase;   /* base and range of effect_newMovingModelEffect's speed (anim_play's playback speed) */
    /* 0x44 */ f32 animSpeedRange;
    /* 0x48 */ f32 scaleBase;
    /* 0x4C */ f32 scaleRange;
    /* 0x50 */ Vec3f rotVelBase;
    /* 0x5C */ Vec3f rotVelRange;
    /* 0x68 */ Color8 color0;
    /* 0x6C */ Color8 color1;
    /* 0x70 */ u8 flags;            /* EffectParams.flags (2 is always added) */
    /* 0x71 */ u8 motionFlags;      /* EFFECT_MOTION_* */
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
} ModelBurst; /* size 0x78 */

/* A particle object made by particle_newQuadParticle / flare_spawnGlow (game/Particle/QuadParticle/quadparticle.c), 0x110 bytes; callers keep their own
 * data after it (game/Particle/QuadParticle/quadparticle.c: DefParticleMotion). Its ParticleDef copy is at 0x18. */
typedef struct DefParticleHead {
    /* 0x000 */ EffectHeader header;    /* header.owner / kind as passed to the spawner */
    /* 0x010 */ s32 active;             /* bit 0: alive */
    /* 0x014 */ s32 unk14;
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 updateFn1;            /* gQuadParticleUpdate1Funcs index, 0 = none */
    /* 0x071 */ u8 updateFn2;            /* gQuadParticleUpdate2Funcs index, 0 = none */
    /* 0x072 */ u8 expireFn;             /* gQuadParticleExpireFuncs index, 0 = none */
    /* 0x073 */ u8 vertexFn;             /* gQuadParticleVertexFuncs index */
    /* 0x074 */ u8 growFlags;
    /* 0x075 */ u8 pad75[3];
    /* 0x078 */ CollisionHit ground;      /* cleared at spawn (height -10000); filled by a ground probe */
    /* 0x09C */ ModelLink link;
    /* 0x0BC */ u8 padBC[4];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuffers[4];  /* per viewport, refreshed from vtxTemplate */
} DefParticleHead; /* size 0x110 */

/* Burst effect (effect_spawnDelayedBurst in game/Effects/DelayedBurst/delayedburst.c, effect_spawnBoneBurst in game/Effects/FollowEffects/followeffects.c): an effect object that
 * keeps two sparks (particle_newQuadParticle beams) and two glows (flare_spawnGlow) on its owner actor. */
struct BurstSpark;
struct BurstGlow;

typedef struct BurstData {
    /* 0x00 */ struct Actor *owner;
    /* 0x04 */ u8 ownerId;            /* owner->unique_id when spawned */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct BurstSpark *sparks[2];
    /* 0x10 */ struct BurstGlow *glows[2];
} BurstData; /* size 0x18 */

typedef struct BurstEffect {
    /* 0x00 */ TimedEffect base;      /* base.header.state: age in frames; -1 frees */
    /* 0x28 */ BurstData data;
} BurstEffect; /* size 0x40 */

/* Fractions of a beam particle's width and length to draw (particle_buildBeamQuad). */
typedef struct BeamScale {
    /* 0x0 */ f32 width;
    /* 0x4 */ f32 length;
} BeamScale; /* size 0x8 */

/* Per-spark data (at spark+0x110). Each flickering value has a countdown timer; when it runs out a new
 * random target base + rand * range is picked. */
typedef struct BurstSparkData {
    /* 0x00 */ struct Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 index;              /* slot in burst->data.sparks */
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ f32 widthBase;
    /* 0x0C */ f32 heightBase;
    /* 0x10 */ f32 widthRange;
    /* 0x14 */ f32 heightRange;       /* one of the two height ranges */
    /* 0x18 */ f32 widthTarget;
    /* 0x1C */ f32 heightTarget;
    /* 0x20 */ f32 heightRange2;      /* the other height range */
    /* 0x24 */ f32 widthTimer;
    /* 0x28 */ f32 heightTimer;
    /* 0x2C */ f32 alphaBase;
    /* 0x30 */ f32 alphaRange;
    /* 0x34 */ f32 alphaTarget;
    /* 0x38 */ f32 alphaTimer;
    /* 0x3C */ f32 alpha;
    /* 0x40 */ f32 beamWidth;         /* BeamScale passed to particle_buildBeamQuad (times scale) */
    /* 0x44 */ f32 beamLength;
    /* 0x48 */ f32 beamWidthTarget;
    /* 0x4C */ f32 beamLengthTarget;
    /* 0x50 */ BurstEffect *burst;
    /* 0x54 */ f32 scale;             /* the owner's average scale */
    /* 0x58 */ u8 flags;              /* bit 0: owner visible */
    /* 0x59 */ u8 pad59[3];
} BurstSparkData; /* size 0x5C */

/* Per-glow data (at glow+0x170). */
typedef struct BurstGlowData {
    /* 0x00 */ s16 textureTimer;      /* frames until the next random texture */
    /* 0x02 */ u8 pad02[2];
    /* 0x04 */ f32 targetScale;       /* shape.scale eases towards it */
    /* 0x08 */ BurstEffect *burst;
    /* 0x0C */ u8 index;              /* slot in burst->data.glows */
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ struct Actor *owner;
    /* 0x14 */ u8 ownerId;
    /* 0x15 */ u8 pad15[3];
} BurstGlowData; /* size 0x18 */

typedef struct BurstSpark {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ BurstSparkData data;
} BurstSpark;

typedef struct BurstGlow {
    /* 0x000 */ DefParticleHead head;       /* head.def.flags bit 1: visible */
    /* 0x110 */ GlowShape shape;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ BurstGlowData data;
} BurstGlow;

/* A sprite particle made by particle_newSpriteParticleWithFloor / particle_newOwnedSpriteParticle / particle_newSpriteParticle (game/Particle/SpriteParticle/spriteparticle.c's SpriteParticle), as seen by
 * callers that keep their own data at 0xA8 (particle_newSpriteParticleWithFloor's data size). */
typedef struct DescParticleHead {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ CollisionHit floor;  /* the spawner's floor template (flags cleared), or the particle's own probe */
    /* 0xA4 */ u8 padA4[4];
} DescParticleHead; /* size 0xA8 */

/* A spark particle (particle_newOwnedSpriteParticle) that slows down: its update (particle_dampSparkXZ in game/Particle/SideDust/sidedust.c,
 * particle_dampSprayXZ in game/Particle/SideSpray/sidespray.c) multiplies desc.vel.x and desc.vel.z by damping once per frame
 * step. */
typedef struct SparkParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 damping;
} SparkParticle; /* size 0xAC */

/* Fade-out and one-shot event of particle_spawnDragFadeSprite's particles (game/Particle/SphereEmitter/sphereemitter.c), run by particle_updateDragFade. */
typedef struct FadeEvent {
    /* 0x0 */ s16 fadeOutStart;  /* desc.timer value */
    /* 0x2 */ s16 fadeOutStep;
    /* 0x4 */ s16 eventTime;     /* desc.timer value, -1 for none */
    /* 0x6 */ s8 eventFunc;      /* gDragFadeEventFuncs index, -1 for none */
} FadeEvent;


/* Description of a growing billboard particle (callback 9) spawned by particle_spawnGrowingBillboard (game/Particle/GrowingBillboard/growingbillboard.c). */
typedef struct BillboardSpawn {
    /* 0x00 */ Vec3f pos;        /* position as floats (mode 1) */
    /* 0x0C */ s16 intX;         /* position as integer + 1/256ths (other modes) */
    /* 0x0E */ s16 intY;
    /* 0x10 */ s16 intZ;
    /* 0x12 */ u8 subX;
    /* 0x13 */ u8 subZ;
    /* 0x14 */ u8 subY;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 width;
    /* 0x18 */ s16 height;
    /* 0x1A */ s16 widthStep;    /* added to the particle's width every frame */
    /* 0x1C */ s16 heightStep;   /* added to the particle's height every frame */
    /* 0x1E */ s16 velX;
    /* 0x20 */ s16 velZ;
    /* 0x22 */ s16 velY;
    /* 0x24 */ s16 accelY;
    /* 0x26 */ s16 life;
    /* 0x28 */ u8 primColor[3];
    /* 0x2B */ u8 envColor[3];
    /* 0x2E */ u8 intensity;     /* both colours are scaled by intensity / 256 */
    /* 0x2F */ u8 alpha;
    /* 0x30 */ s8 fadeStep;
    /* 0x31 */ u8 onExpire;      /* low nibble: index into gBillboardExpireFuncs, run when life reaches 0 */
} BillboardSpawn; /* size 0x34 */

/* Textured quad sprite (with optional model) made by particle_newQuadSprite (game/Particle/QuadParticle/quadparticle.c); particle_spawnFlatQuadSprite and the
 * other spawners return it, and callers keep their own data after it (extraSize). */
typedef struct QuadSprite {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 active;         /* bit 0: alive */
    /* 0x14 */ s32 unk14;
    /* 0x18 */ SpriteDef def;
    /* 0x40 */ Vtx vtx[4];
    /* 0x80 */ u8 updateFn;        /* gQuadSpriteUpdateFuncs index, 0 = none */
    /* 0x81 */ u8 expireFn;        /* gQuadSpriteExpireFuncs index, 0 = none */
    /* 0x82 */ u8 pad82[2];
    /* 0x84 */ ModelLink link;
    /* 0xA4 */ Vec3f pos;
} QuadSprite; /* size 0xB0 */

/* Head (first 0x60 bytes) of the object effect_newEffectObj (game/Effects/EffectObj/effectobj.c) makes from an EffectTemplate: trails,
 * ribbons, sprays and splashes. The rest is per handler, so each user declares its own object as
 * {EffectObjHead obj; 0x24-byte block at 0x60; u8 unk84[0x10]; items * at 0x94; user data * at 0x98}: the user
 * data (dataSize bytes) and then the items (tmpl.itemCount of itemSize bytes) follow the object in memory. */
typedef struct EffectObjHead {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ EffectTemplate tmpl;   /* tmpl.itemCount: size of the items ring buffer */
    /* 0x2C */ s8 count;              /* items in use */
    /* 0x2D */ s8 start;              /* oldest item */
    /* 0x2E */ s8 end;                /* one past the newest item */
    /* 0x2F */ u8 updateKind;         /* index into gEffectObjUpdateFuncs, 0: none */
    /* 0x30 */ u8 updateKind2;        /* index into gEffectObjUpdate2Funcs, 0: none (trails clear it to stop emitting) */
    /* 0x31 */ u8 drawKind;           /* index into gEffectObjDrawFuncs, 0: none */
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ ModelLink model;       /* drawn through math_emitPointLights */
    /* 0x54 */ Vec3f modelPos;        /* math_emitPointLights's position; trails keep their tail point here */
} EffectObjHead; /* size 0x60 */

/* The object effect_newEffectObj (game/Effects/EffectObj/effectobj.c) makes, in its generic view: EffectObjHead plus the per-handler block.
 * It is followed in memory by its user data (dataSize bytes) and then its items (a ring buffer); users with
 * typed items declare their own view (TrackerTrail, WaveTrail, MotionTrail, ...). */
typedef struct EffectObj {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ CollisionHit collision; /* a copy of effect_newEffectObj's hit (flags 0 without one) */
    /* 0x84 */ u8 buffers[0x10];  /* per-view vertex buffers (particle_getViewBuffer), released by particle_freeViewBuffers */
    /* 0x94 */ void *items;       /* ring buffer of head.tmpl.itemCount items */
    /* 0x98 */ void *userData;
    /* 0x9C */ u8 pad9C[4];
} EffectObj; /* size 0xA0 */

/* Data of the tracker effect (game/Effects/Trail/LevelStream/levelstream.c): follows one point of its owner and leaves a TrackerTrail behind it. */
typedef struct TrackerData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;        /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct TrackerTrail *trail; /* spawned by trail_spawnTrackerTrail */
    /* 0x0C */ Vec3f prevPos;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f delta;
    /* 0x30 */ f32 dist;          /* distance moved this frame */
    /* 0x34 */ u8 pointFn;        /* index into gTrackerPointProcs */
} TrackerData;

typedef struct TrackerEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrackerData data;
} TrackerEffect;

/* User data of a TrackerTrail (copied in at spawn by trail_spawnTrackerTrail, game/Effects/Trail/LevelStream/levelstream.c; run by game/Effects/Trail/TrackerTrail/trackertrail.c). */
typedef struct TrackerTrailData {
    /* 0x00 */ TrackerEffect *owner; /* NULL once the tracker lets go of the trail */
    /* 0x04 */ f32 texture;       /* running texture coordinate of the newest sample */
    /* 0x08 */ u8 alpha;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ f32 maxLength;
    /* 0x10 */ f32 widthBase;
    /* 0x14 */ f32 widthScale;
    /* 0x18 */ f32 alphaScale;
    /* 0x1C */ f32 ownerScale;    /* the tracker owner's scale_xz */
} TrackerTrailData; /* size 0x20 */

/* The ribbon trail of a tracker effect (effect_newEffectObj object, kind 19). */
typedef struct TrackerTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ struct TrackerTrailPoint *points; /* game/Effects/Trail/TrackerTrail/trackertrail.c */
    /* 0x98 */ TrackerTrailData *state;
} TrackerTrail;

/* A vector in the XZ plane: the sideways normals and drift velocities of trails. */
typedef struct TrailVec2 {
    f32 x;
    f32 z;
} TrailVec2;

/* Kind 17 trail (trail_spawnWaveTrail, game/Effects/Trail/LevelStream/levelstream.c): follows an actor or a sprite particle (trail_followWaveTrail,
 * same file) and wobbles as a sine wave that grows along its length (trail_shapeWaveTrail). */

/* WaveTrailData.flags */
#define WAVE_TRAIL_HAS_NORMAL 1 /* scaled/normal have been initialised */
#define WAVE_TRAIL_SMOOTH     2 /* recompute the previous point's normal */

/* User data of a WaveTrail, copied in at spawn. */
typedef struct WaveTrailData {
    /* 0x00 */ Actor *owner;              /* followed when anchor is NULL */
    /* 0x04 */ u8 ownerId;                /* owner->unique_id at spawn; the trail dies when it changes */
    /* 0x08 */ DescParticleHead *anchor;  /* followed instead of the owner, when not NULL */
    /* 0x0C */ TrailVec2 scaled;          /* normal * 51 */
    /* 0x14 */ TrailVec2 normal;          /* sideways unit normal of the newest segment */
    /* 0x1C */ u8 flags;                  /* WAVE_TRAIL_* */
    /* 0x20 */ f32 maxLength;             /* points beyond this summed length are trimmed */
    /* 0x24 */ f32 phase;                 /* of the sine wave, at the newest point */
} WaveTrailData; /* size 0x28 */

/* One point of a WaveTrail's ring buffer. */
typedef struct WaveTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 dist;     /* length of the segment to the next-older point */
    /* 0x10 */ f32 invLen;   /* 1 / its XZ length */
    /* 0x14 */ f32 wave;     /* sideways sine offset */
    /* 0x18 */ TrailVec2 scaled;
    /* 0x20 */ TrailVec2 normal;
} WaveTrailPoint; /* size 0x28 */

typedef struct WaveTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ WaveTrailPoint *points;
    /* 0x98 */ WaveTrailData *data;
} WaveTrail;

/* Head of the data of a particle stream (TimedEffect data at 0x28): every tick it emits `rate` particles from
 * position, aimed along math_dirFromAngles angles that wobble as sines (particle_updateWobbleStream, game/Weapon/Projectile/projectile.c;
 * particle_updateSourceEmission, game/Particle/EmissionSource/emissionsource.c). */
typedef struct ParticleStream {
    /* 0x00 */ f32 angleX;        /* centre direction */
    /* 0x04 */ f32 angleY;
    /* 0x08 */ f32 amplitudeX;    /* of the angle wobble */
    /* 0x0C */ f32 amplitudeY;
    /* 0x10 */ f32 phaseX;
    /* 0x14 */ f32 phaseY;
    /* 0x18 */ f32 speedX;        /* phase advance per tick */
    /* 0x1C */ f32 speedY;
    /* 0x20 */ f32 time;
    /* 0x24 */ f32 rate;          /* particles per tick */
    /* 0x28 */ f32 remaining;     /* particles owed (emitted while above 1) */
    /* 0x2C */ Vec3f position;
} ParticleStream; /* size 0x38 */

/* A stream of weapon_spawnProjectile projectiles (TimedEffect tick callback 0x2B, particle_updateWobbleStream, game/Weapon/Projectile/projectile.c). */
typedef struct ProjectileStream {
    /* 0x00 */ ParticleStream stream;
    /* 0x38 */ f32 speed;         /* weapon_spawnProjectile's speed */
    /* 0x3C */ f32 distScale;     /* its arg7 (weapon_setupProjectileRay's distScale) */
    /* 0x40 */ f32 defaultRemaining; /* its arg10 */
} ProjectileStream; /* size 0x44 */

/* A timed emitter of ProjectileStreams (TimedEffect tick callback 0x2A, weapon_updateStreamEmitter, game/Weapon/Projectile/projectile.c; spawned by
 * level_spawnBulletZone, game/Level/ZoneSpawn/zonespawn.c): after a random delay it starts one stream and picks a new delay. */
typedef struct StreamEmitter {
    /* 0x00 */ s16 *position;     /* x, y, z (of the spawning TriggerZone) */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s32 minDelay;
    /* 0x0C */ s32 delayRange;
    /* 0x10 */ s16 minLife;       /* of each stream */
    /* 0x12 */ s16 lifeRange;
    /* 0x14 */ f32 rateBase;      /* stream rate: rateBase + rand * rateRange */
    /* 0x18 */ f32 rateRange;
    /* 0x1C */ f32 angleX;        /* stream direction */
    /* 0x20 */ f32 angleY;
    /* 0x24 */ f32 spread;        /* max wobble amplitude */
    /* 0x28 */ f32 wobbleSpeed;   /* phase speed of the wobble */
    /* 0x2C */ f32 speed;         /* copied to ProjectileStream.speed, distScale, defaultRemaining */
    /* 0x30 */ f32 distScale;
    /* 0x34 */ f32 defaultRemaining;
} StreamEmitter; /* size 0x38 */

/* Spray description that trail_createSprayEmitter (game/Effects/Trail/SprayEmitter/sprayemitter.c) copies into a new SprayEmitter (0x50 bytes).
   Angles are in 256ths of a turn; every "...Range" is the random spread added to the value before it. */
typedef struct SprayDesc {
    /* 0x00 */ s16 yaw;
    /* 0x02 */ s16 yawSpread; /* yaw varies by +-yawSpread */
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 pitchRange;
    /* 0x08 */ Vec3f origin;
    /* 0x14 */ Vec3f extent; /* particles start on the segment origin + t * extent */
    /* 0x20 */ f32 speed;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 rate; /* particles per frame */
    /* 0x2C */ f32 rateRange;
    /* 0x30 */ f32 halfWidth;
    /* 0x34 */ f32 halfWidthRange;
    /* 0x38 */ s16 alpha; /* TrailMotion.alpha */
    /* 0x3A */ s16 alphaRange;
    /* 0x3C */ f32 gravity;
    /* 0x40 */ f32 gravityRange;
    /* 0x44 */ s16 life; /* EffectTemplate.life */
    /* 0x46 */ s16 lifeRange;
    /* 0x48 */ s32 pointCount;
    /* 0x4C */ s32 pointCountRange;
} SprayDesc; /* size 0x50 */

/* Per-behavior data of a Sprite2D (Sprite2DParams.data). */
typedef union Sprite2DData {
    struct {
        s32 frame;                /* frame (8.8) to stop scrolling at */
        s32 y;                    /* y to stop scrolling at */
        s32 stage;
    } scroll;                     /* sprite_updateFrameScroll (game/Sprite/SpriteRoutines/spriteroutines.c) */
    struct {
        s32 elapsed;
    } slide;                      /* sprite_updateSlideBlink */
    struct {
        s16 endX;
        s16 endY;
        s32 duration;
        s16 startX;
        s16 startY;
    } move;                       /* sprite_updateLinearMove: from start to end as the timer runs down */
    struct {
        s32 fadeInHold;           /* fade-in frames << 16 | hold frames */
        s32 fadeOutTotal;         /* fade-out frames << 16 | total frames */
        s32 unk10;
    } fade;                       /* game/Beam/ModelBeam/modelbeam.c, game/Sprite/FadeSprite/fadesprite.c */
    s32 words[3];                 /* move's points packed as x << 16 | y */
} Sprite2DData; /* size 0xC */

/* Spawn parameters of a 2D screen sprite (object type 0x5E, sprite_newSprite2D / sprite_newSprite2DDefault, game/Sprite/Sprite2D/sprite2d.c),
 * copied to Sprite2D.params. */
typedef struct Sprite2DParams {
    /* 0x00 */ void *texture;     /* drawn with sprite_drawImage: a TextureDesc, or a HudSprite (same layout) */
    /* 0x04 */ TextureDesc *tile1Texture; /* loaded into tile 1 when flags & 1 */
    /* 0x08 */ Sprite2DData data;
    /* 0x14 */ s16 timer;         /* frames left; 0 = lives forever */
    /* 0x16 */ s16 frame;         /* animation frame, 8.8 fixed point */
    /* 0x18 */ s16 frameSpeed;    /* added to frame every tick */
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 x;
    /* 0x20 */ f32 y;
    /* 0x24 */ s16 sOffset;
    /* 0x26 */ s16 extraWidth;
    /* 0x28 */ s16 scaleX;        /* 0x1000 = 1.0 */
    /* 0x2A */ s16 scaleY;
    /* 0x2C */ u16 width;
    /* 0x2E */ u16 height;
    /* 0x30 */ s8 behavior;       /* index into gObjectCallbacks, 0 = none */
    /* 0x31 */ u8 frameCount;
    /* 0x32 */ u8 red;
    /* 0x33 */ u8 green;
    /* 0x34 */ u8 blue;
    /* 0x35 */ u8 alpha;
    /* 0x36 */ u8 renderMode;     /* index into gEffectRenderModes */
    /* 0x37 */ u8 combiner;       /* particle_setCombineMode's combiner */
    /* 0x38 */ u8 cycleType;      /* 2 = two-cycle */
    /* 0x39 */ u8 flags;
} Sprite2DParams; /* size 0x3C */

typedef struct Sprite2D {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Sprite2DParams params;
} Sprite2D; /* size 0x4C */

/* Spawn parameters of a rumble effect object (cont_spawnRumble, kind 0x3F; copied into RumbleObj.params):
 * a timed rumble on the controllers of the players in a mask. */
typedef struct RumbleParams {
    /* 0x0 */ u8 flags;       /* bit 0: duration counts down */
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ u8 strength;    /* 0 (off) to 8 (full), see cont_startRumble */
    /* 0x5 */ u8 playerMask;  /* bit i: player i */
    /* 0x6 */ s8 updateKind;  /* index into gRumbleUpdateHandlers, -1: none */
    /* 0x7 */ u8 unk7;
} RumbleParams; /* size 0x8 */

/* Per-particle data (at 0xA8) of wobbling particles: the offset moves around an x/z ellipse
 * (particle_wobbleSpriteOffset) and the velocity is scaled by drag per tick (particle_applyDrag). */
typedef struct WobbleData {
    /* 0x00 */ u8 phase[2];      /* x and z angles of the wobble ellipse */
    /* 0x02 */ u8 phaseStep[2];  /* added to phase per tick */
    /* 0x04 */ f32 amplitude[2]; /* x and z radii */
    /* 0x0C */ f32 drag;         /* velocity scale per tick */
} WobbleData; /* size 0x10 */

/* Per-particle data for particle_updatePulse: two sizes oscillating around a base size. */
typedef struct ParticlePulse {
    /* 0x00 */ u8 phase[2];
    /* 0x02 */ s8 phaseStep[2];
    /* 0x04 */ f32 baseSize;
    /* 0x08 */ f32 amplitude[2];
} ParticlePulse; /* size 0x10 */

/* Particle data at 0xA8 of the rising-and-swaying particles (update func 4: effect_updateRiseParticle, effect_updateRiseParticleToCeiling):
 * size wobble, rise speed limit, x/z sway, and a ceiling (particle flag 0x2000 kills it above). */
typedef struct RiseMotion {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 widthPhase;
    /* 0x05 */ u8 heightPhase;
    /* 0x06 */ s8 widthPhaseStep;
    /* 0x07 */ s8 heightPhaseStep;
    /* 0x08 */ f32 widthWobble;
    /* 0x0C */ f32 heightWobble;
    /* 0x10 */ f32 maxRiseSpeed;
    /* 0x14 */ f32 swayPerSpeed;
    /* 0x18 */ u8 swayPhaseX;
    /* 0x19 */ u8 swayPhaseZ;
    /* 0x1A */ u8 swayPhaseStepX;
    /* 0x1B */ u8 swayPhaseStepZ;
    /* 0x1C */ f32 swayX;
    /* 0x20 */ f32 swayZ;
    /* 0x24 */ f32 ceiling;
} RiseMotion; /* size 0x28 */

/* A particle (DescParticleHead) with RiseMotion data. */
typedef struct RiseParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ RiseMotion motion;
} RiseParticle; /* size 0xD0 */

/* Particle data at 0x110 of a DefParticleHead particle animated by particle_updateJitter (written by particle_spawnJitterParticle,
 * game/Particle/GroundCluster/groundcluster.c): def.width, def.height and def.frameSpeed each ease toward a random target that is re-picked
 * when its countdown (in frames) runs out. */
typedef struct ParticleJitter {
    /* 0x00 */ f32 widthBase;
    /* 0x04 */ f32 heightBase;
    /* 0x08 */ f32 widthRange;
    /* 0x0C */ f32 heightRangeA;   /* one of the two height ranges is picked at random */
    /* 0x10 */ f32 widthTarget;
    /* 0x14 */ f32 heightTarget;
    /* 0x18 */ f32 heightRangeB;
    /* 0x1C */ f32 widthTimer;
    /* 0x20 */ f32 heightTimer;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ f32 speedTarget;
    /* 0x30 */ f32 speedTimer;
    /* 0x34 */ f32 speed;          /* eased value copied to def.frameSpeed */
    /* 0x38 */ Vec3f velocity;     /* ballistic motion (see particle_moveJitterBallistic, game/Particle/GroundCluster/groundcluster.c) */
    /* 0x44 */ f32 gravity;
    /* 0x48 */ f32 drag;           /* velocity scale per tick */
    /* 0x4C */ f32 easeFactor;     /* particle_easeBendTarget eases def.rot towards 100 units above def.pos by it */
} ParticleJitter; /* size 0x50 */

/* Caller data at 0x160 of a JitterParticle spawned by beam_launchJitterParticle (game/Beam/AreaEmitter/areaemitter.c): particle_relaunchJitterParticle
 * (game/Particle/GroundCluster/groundcluster.c) relaunches the particle with this life and gravity. */
typedef struct JitterRelaunch {
    /* 0x0 */ f32 gravity;
    /* 0x4 */ s16 lifetime;
} JitterRelaunch; /* size 0x8 */

/* The particle particle_spawnJitterParticle (game/Particle/GroundCluster/groundcluster.c) spawns through particle_newQuadParticle: its particle_updateJitter state, then
 * the caller's data. */
typedef struct JitterParticle {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleJitter jitter;
    /* 0x160 */ JitterRelaunch relaunch;
} JitterParticle;

/*
 * Per-particle motion state at +0x110 of a particle_spawnChannelParticle particle (game/Particle/ChannelParticle/channelparticle.c). Each channel (A, B, C)
 * picks a new random target between its min and max whenever its timer runs out, and the
 * particle field it drives eases towards that target by `rate` per frame.
 */
typedef struct ParticleChannels {
    /* 0x00 */ f32 maxA;
    /* 0x04 */ f32 minA;
    /* 0x08 */ f32 targetA;
    /* 0x0C */ s16 timerA;
    /* 0x0E */ s16 periodA;
    /* 0x10 */ f32 rateA;
    /* 0x14 */ f32 maxB;
    /* 0x18 */ f32 minB;
    /* 0x1C */ f32 altB;
    /* 0x20 */ f32 targetB;
    /* 0x24 */ s16 timerB;
    /* 0x26 */ s16 periodB;
    /* 0x28 */ f32 rateB;
    /* 0x2C */ s32 maxC;
    /* 0x30 */ s32 minC;
    /* 0x34 */ s32 targetC;
    /* 0x38 */ s16 timerC;
    /* 0x3A */ s16 periodC;
    /* 0x3C */ f32 rateC;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
    /* 0x48 */ u8 effect;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 maxHeight;
} ParticleChannels; /* size 0x50 */

/* The particle particle_spawnChannelParticle (game/Particle/ChannelParticle/channelparticle.c) spawns through particle_newQuadParticleWhite. */
typedef struct ChannelParticle {
    /* 0x000 */ DefParticleHead head; /* head.def.scale.x: accelY, head.def.scale.y: velY */
    /* 0x110 */ ParticleChannels motion;
    /* 0x160 */ f32 extra; /* the caller's emitter->extraSize bytes; game/Particle/DiscEmitter/discemitter.c keeps a yaw angle here */
} ChannelParticle;

/* Per-particle data of the age-driven fade (trail_updateBurstParticle / trail_updateBurstParticleTint for DescParticleHead particles,
 * game/Effects/Trail/BoneTrail/bonetrail.c; billboard callbacks 0x14 / 0x15, game/Effects/BloodDebris/blooddebris.c). Each *Start is a remaining-life value:
 * below it the alpha / colour follow the life (life * step) and the size grows by growRate per tick. */
typedef struct AgeFadeParams {
    /* 0x0 */ s16 tintStart;   /* then brightness (or the env colour) = life * tintStep */
    /* 0x2 */ s16 tintStep;
    /* 0x4 */ s16 fadeStart;   /* then alpha = life * fadeStep */
    /* 0x6 */ s16 fadeStep;
    /* 0x8 */ s16 growStart;
    /* 0xA */ s16 growRate;
} AgeFadeParams; /* size 0xC */

/* Per-particle data for the floor-collision update funcs (particle_checkSpriteFloor). */
typedef struct ParticleBounce {
    /* 0x0 */ u8 flags;
    /* 0x1 */ s8 hitFunc;      /* gSpriteFloorHitFuncs index, -1 for none */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ f32 restitution;
} ParticleBounce; /* size 0x8 */

/* Per-particle data for particle_updateFlash (spawned by particle_spawnFlash). */
typedef struct ParticleFlash {
    /* 0x00 */ f32 elapsed;
    /* 0x04 */ f32 duration;
    /* 0x08 */ f32 growth;
    /* 0x0C */ f32 startAlpha;
    /* 0x10 */ f32 alphaDecay;
} ParticleFlash; /* size 0x14 */

/* A sprite particle (game/Particle/SpriteParticle/spriteparticle.c): its DescParticleHead, then dataSize bytes of per-particle data (see
 * particle_newSpriteParticleWithFloor). The variants of data are those of the update funcs in game/Particle/SpriteParticle/spriteparticle.c and the ones whose data
 * type is shared; other kinds keep their own types (a DescParticleHead followed by their data). */
typedef struct SpriteParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {              /* caller-sized, see particle_newSpriteParticleWithFloor */
        f32 drag;               /* update funcs 10, 16, 26 (horizontal, whole, vertical velocity) */
        ParticleBounce bounce;
        struct {
            f32 drag;
            u8 pad4[4];
            ParticleBounce bounce;
        } dragBounce;
        struct {
            f32 drag;
            u8 pad4[4];
            ActorRef owner;     /* the particle moves with it (effect_updateBloodStripParticle, game/Effects/FollowEffects/followeffects.c) */
        } dragOwner;
        ParticlePulse pulse;
        ParticleFlash flash;
        WobbleData wobble;      /* update func 22 (particle_wobbleSmoke, game/Particle/SmokeEmitter/smokeemitter.c) and the like */
        AgeFadeParams ageFade;  /* update funcs 0 and 1 (trail_updateBurstParticle / trail_updateBurstParticleTint, game/Effects/Trail/BoneTrail/bonetrail.c) */
        struct {
            f32 drag;
            u8 pad4[4];
            FadeEvent fade;
        } dragFade;             /* update func 17 (particle_updateDragFade, game/Particle/SphereEmitter/sphereemitter.c) */
    } data;
} SpriteParticle;

/* Settings an actor-attached particle emitter (object type 0x21, particle_newActorEmitter / particle_startActorEmitter in
 * game/Particle/ActorEmitter/actoremitter.c) is created with; copied verbatim into the emitter at +0x18. */
typedef struct ActorEmitterParams {
    /* 0x0 */ Actor *owner;
    /* 0x4 */ u8 ownerId;   /* owner->unique_id at creation; the emitter dies when it changes */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ s16 lifetime; /* counts down when (flags & 1) */
    /* 0x8 */ u8 flags;
    /* 0x9 */ s8 expireCb;  /* index into gActorEmitterExpireFuncs run when lifetime runs out, -1: just die */
    /* 0xA */ u8 table;     /* index into gActorEmitterTables */
    /* 0xB */ u8 padB;
} ActorEmitterParams; /* size 0xC */

/* Size wobble of a sprite: EffectSprite.wobble (at sprite + 0x110) and ActorEmitterParticleData.wobble. */
typedef struct {
    /* 0x00 */ f32 hi0;
    /* 0x04 */ f32 lo0;
    /* 0x08 */ f32 target0;
    /* 0x0C */ f32 hi1;
    /* 0x10 */ f32 lo1;
    /* 0x14 */ f32 peak1;
    /* 0x18 */ f32 target1;
    /* 0x1C */ s16 timer0;
    /* 0x1E */ s16 timer1;
    /* 0x20 */ f32 hi2;
    /* 0x24 */ f32 lo2;
    /* 0x28 */ f32 target2;
    /* 0x2C */ s16 timer2;
} SpriteWobble;

/* Per-particle data that an actor emitter (game/Particle/ActorEmitter/actoremitter.c) copies to 0x110 of each particle it spawns. */
typedef struct ActorEmitterParticleData {
    /* 0x00 */ SpriteWobble wobble;
    /* 0x30 */ Vec3f vel;
    /* 0x3C */ f32 gravity;
    /* 0x40 */ f32 drag;
    /* 0x44 */ f32 shrinkRate;
    /* 0x48 */ s16 fadeStart;
    /* 0x4A */ s16 fadeRate;
    /* 0x4C */ f32 blend;
    /* 0x50 */ f32 stretch;   /* tilt per sqrt of sideways speed (particle_buildActorEmitterQuad, game/Particle/ActorEmitter/actoremitter.c) */
    /* 0x54 */ struct ParticleTemplate *tmpl;
} ActorEmitterParticleData; /* size 0x58 */

/* A particle of an actor emitter (game/Particle/ActorEmitter/actoremitter.c, with its callbacks), made by particle_newQuadParticleWhite. */
typedef struct ActorEmitterParticle {
    /* 0x000 */ DefParticleHead head; /* head.def.rot: the previous position */
    /* 0x110 */ ActorEmitterParticleData data;
} ActorEmitterParticle;

/* Where an actor emitter process reads its scale. */
typedef struct EmitterScaleSource {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 *unk18; // emitter scale in percent
} EmitterScaleSource;

/* One particle of an ActorEmitterProcess. */
typedef struct ActorEmitterSlot {
    /* 0x00 */ ActorEmitterParticle *particle;
    /* 0x04 */ s32 timer;
    /* 0x08 */ Vec3f offset;
    /* 0x14 */ struct ActorEmitterSlot *next; // ring of free slots while emitting
} ActorEmitterSlot; /* 0x18 */

/* Process of type 0x21: emits particles around (or from the bones of) its owner (game/Particle/ActorEmitter/actoremitter.c, with its
 * particle callbacks). */
typedef struct ActorEmitterProcess {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 flags;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ ActorEmitterParams params;
    /* 0x24 */ f32 accum; // fractional particles owed
    /* 0x28 */ s32 count; // live particles
    /* 0x2C */ u8 state;  // 1: hidden, 2: world mode, 4: attached mode, 8: new
    /* 0x2D */ u8 infoMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ EmitterScaleSource *scaleSrc;
    /* 0x34 */ f32 scale;
    /* 0x38 */ Vec3f pos;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ ActorEmitterSlot slots[100];
} ActorEmitterProcess;

/*
 * Character watcher (object type 0x2E, effect_newCharacterWatcher in game/Effects/EffectActor/effectactor.c; attached to the character as
 * attachment 0x13): tracks a character's state (five small fields packed by game/Effects/EffectActor/effectactor.c's STATE_BITS)
 * and runs the actions of gWatcherModeActions[mode] when it changes as described by gWatcherStateRules. The creator's
 * extra data (e.g. the effects the watcher keeps alive) follows at 0x58.
 */
typedef struct CharacterWatcher {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Actor *character;
    /* 0x1C */ u8 characterId;
    /* 0x1D */ u8 mode; /* from effect_pickWatcherMode */
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ Vec3f lastPosition;
    /* 0x2C */ u8 state0; /* effect_watcherIsOutOfWater */
    /* 0x2D */ u8 state1; /* effect_watcherIsDeepWater */
    /* 0x2E */ u8 state2; /* effect_watcherIsNearWaterSurface */
    /* 0x2F */ u8 moveState; /* effect_getWatcherMoveState */
    /* 0x30 */ u8 surfaceState; /* effect_getWatcherSurfaceState */
    /* 0x31 */ u8 flags; /* 1: run state-change actions */
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ CollisionHit hit; /* floor under the character, probed by the watcher's effects */
} CharacterWatcher; /* size 0x58 */

/* Payload of a FlickerSprite: the sprite's animation speed (def.frameSpeed) eases toward a target that is
 * re-picked after a random delay. The owning watcher (a CharacterWatcher with its effects after it) sets
 * keepAlive every frame; the sprite's update clears it and the sprite dies once it is not refreshed, and its
 * expiry clears the owner's pointer to it (game/Effects/WatcherEffects/watchereffects.c, game/Effects/ActorTasks/actortasks.c). */
typedef struct SpriteFlicker {
    /* 0x00 */ CharacterWatcher *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 minTarget;
    /* 0x0C */ f32 targetRange;
    /* 0x10 */ f32 target;
    /* 0x14 */ s16 timer;         /* frames until the target is re-picked */
    /* 0x16 */ s16 minDelay;
    /* 0x18 */ s16 delayRange;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 easing;        /* fraction of the distance to the target covered per frame */
} SpriteFlicker; /* size 0x20 */

/* A ground-glow QuadSprite (particle_spawnSurfaceQuadSpriteNoModel) with a SpriteFlicker payload. */
typedef struct FlickerSprite {
    /* 0x00 */ QuadSprite quad;
    /* 0xB0 */ SpriteFlicker flicker;
} FlickerSprite; /* size 0xD0 */

/* Extra data at 0x58 of a watcher's rising-particle emitter (blood_spawnDrip, game/Effects/Blood/blood.c). Like
 * SpriteFlicker, the owning watcher sets keepAlive every frame, the emitter's update clears it and the
 * emitter dies once it is not refreshed; its destructor clears the owner's pointer to it. */
typedef struct WatcherEmitterLink {
    /* 0x00 */ CharacterWatcher *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 ceiling;       /* height the particles rise to: the surface above the character */
} WatcherEmitterLink; /* size 0xC */

typedef struct WatcherEmitter {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x48];    /* the blood-drip body (game/Effects/Blood/blood.c) */
    /* 0x58 */ WatcherEmitterLink link;
} WatcherEmitter; /* size 0x64 */

#define DYNMODEL_FLAG_LIFETIME      1
#define DYNMODEL_FLAG_VIEWPORT_MASK 2

/* Spawn descriptor of a DynModel (effect_newDynModel, game/Effects/DynModel/dynmodel.c: a model whose vertices are copied into
 * per-frame, per-viewport buffers); copied verbatim into DynModel.desc. */
typedef struct DynModelDesc {
    /* 0x00 */ u8 flags;       // DYNMODEL_FLAG_*
    /* 0x01 */ s8 updateFunc;  // index into gDynModelUpdateFuncs, -1 for none
    /* 0x02 */ s8 drawFunc;    // index into gDynModelDrawFuncs, -1 for none
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 lifetime;   // counts down while DYNMODEL_FLAG_LIFETIME is set
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[0xA];
    /* 0x30 */ s32 assetTable;  // rom_loadPathTable path, first index: the asset table (9)
    /* 0x34 */ s32 assetIndex;  // second index: the model's entry in that table
    /* 0x38 */ u8 kind;        // selects the gDynModelDestroyFuncs / gDynModelFreeFuncs handlers
    /* 0x39 */ u8 viewportMask; // viewports drawn in when DYNMODEL_FLAG_VIEWPORT_MASK is set
    /* 0x3A */ u8 pad3A[2];
} DynModelDesc; /* size 0x3C */

/*
 * A model whose vertices are copied into per-frame, per-viewport buffers so they can be modified each frame
 * (effect_newDynModel, game/Effects/DynModel/dynmodel.c). The buffers follow the header, then `userDataSize` bytes of user data.
 */
typedef struct DynModel {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ DynModelDesc desc;
    /* 0x4C */ ModelAsset *asset;
    /* 0x50 */ s32 buffersSize;     /* size of all vertex buffers, i.e. the offset of the user data */
    /* 0x54 */ Gfx *displayList;
    /* 0x58 */ Vtx *vertices[2][4]; /* [frame][viewport] */
    /* 0x78 */ Mtx mtx[2];          /* [frame] */
    /* 0xF8 */ u8 buffers[1];
} DynModel;

/* The user data after a DynModel's vertex buffers. matching: s32 arithmetic (a u8 * sum schedules differently) */
#define DYNMODEL_USERDATA(model) ((void *)((s32)(model) + (model)->buffersSize + 0xF8))

/* One point of a trail_createMotionTrail trail (game/Effects/Trail/MotionTrail/motiontrail.c); the points form the object's ring buffer. */
typedef struct MotionTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velocityY;
    /* 0x10 */ u16 phase;       /* texture coordinate along the trail */
    /* 0x12 */ u8 pad12[2];
} MotionTrailPoint; /* size 0x14 */

/* A trail made by trail_createMotionTrail (game/Effects/Trail/MotionTrail/motiontrail.c): an EffectObjHead object whose points move by TrailMotion
 * and collide with the floor. */
typedef struct MotionTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ CollisionHit collision;
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ MotionTrailPoint *points;
    /* 0x98 */ TrailParams *params;
} MotionTrail; /* size 0x9C */

/* Timed-effect payloads, shared by the spawning code and the effect callbacks. */

/* Camera wobble (object type 0x39, game/Camera/CameraWobble/camerawobble.c): up to six sine waves summed into a player's camera offset. */
#define CAM_WOBBLE_TIMED    1 // stop once the timer runs out
#define CAM_WOBBLE_STOPPING 2 // each wave stops at its next zero crossing
#define CAM_WOBBLE_SCALED   4 // amplitudes are scaled by the scaleFn callback

/* What a wobble (camera_createWobble) is created from; copied into the effect at +0x10. */
typedef struct CamWobbleDef {
    /* 0x00 */ u8 flags;       // CAM_WOBBLE_*
    /* 0x01 */ s8 updateFn;    // index into gWobbleUpdateCallbacks, or -1
    /* 0x02 */ s8 scaleFn;     // index into gWobbleScaleCallbacks, or -1
    /* 0x04 */ s16 timer;      // frames left, with CAM_WOBBLE_TIMED
    /* 0x06 */ u8 playerIndex;
    /* 0x08 */ f32 amplitude[6]; // position xyz, rotation xyz
    /* 0x20 */ f32 speed[6];     // phase step per gFrameDelta of time; 0 means the wave is off
} CamWobbleDef; // size 0x38

/* Spawn parameters of a particle emitter (beam_newAreaEmitter). */
typedef struct EmitterDef {
    /* 0x00 */ u8 flags;             /* EMITTER_* */
    /* 0x04 */ struct TriggerZone *anchor; /* zone giving the centre (and area) with EMITTER_ANCHORED */
    /* 0x08 */ f32 rate;             /* particles per frame and unit area: rate + rand * rateRange */
    /* 0x0C */ f32 rateRange;
    /* 0x10 */ Vec3f pos;            /* centre without EMITTER_ANCHORED */
    /* 0x1C */ f32 bottom;           /* lowest height the ground probe searches */
    /* 0x20 */ f32 radius;
    /* 0x24 */ u8 type;              /* 1: switched on and off by messages (beam_switchEmitterMessage) */
    /* 0x25 */ s8 update;            /* index into gEmitterUpdateHooks, -1 for none */
    /* 0x26 */ s8 spawn;             /* index into gEmitterSpawnHooks, -1 for none */
} EmitterDef; /* size 0x28 */

/* EmitterDef.flags */
#define EMITTER_ON       1
#define EMITTER_ANCHORED 2
#define EMITTER_ZONED    4 /* only spawns while its zone is visible */
#define EMITTER_NO_PROBE 8 /* spawns at `bottom` without probing for the ground */

/* State of a particle emitter, at 0x28 of its Emitter. */
typedef struct EmitterState {
    /* 0x00 */ EmitterDef def;
    /* 0x28 */ f32 pending;          /* particles owed; one spawns per whole unit */
    /* 0x2C */ CollisionHit ground;
    /* 0x50 */ ZoneNode *zone;       /* collision_findZoneLeaf zone of the centre, with EMITTER_ZONED */
    /* 0x54 */ f32 area;
} EmitterState; /* size 0x58 */

/* Particle emitter: the timed effect that beam_newAreaEmitter (game/Beam/AreaEmitter/areaemitter.c) spawns from an EmitterDef. */
typedef struct Emitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterState state;
    /* 0x80 */ u8 data[1];           /* user data */
} Emitter;

/* A drip emitter's attachment to its actor (particle_newDripEmitter, game/Particle/Drips/drips.c). */
typedef struct ActorLink {
    /* 0x0 */ struct Actor *actor;
    /* 0x4 */ u8 actorId;           /* actor->unique_id when attached */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ s16 timer;
    /* 0x8 */ u8 flags;             /* bit 0: the timer counts down */
    /* 0x9 */ u8 unk9;
    /* 0xA */ u8 kind;              /* gDripEmitterKinds index */
} ActorLink; /* size 0xC */

/* Tick data of timed effect 0x46 (effect_updateBoltRing, game/Effects/ObjBurst/objburst.c): bolts around a trigger zone. */
typedef struct BoltRingData {
    /* 0x00 */ struct TriggerZone *emitter;
    /* 0x04 */ f32 spawnAccum;
    /* 0x08 */ u8 emitterState;
} BoltRingData;

/* The 12-byte record at +0x28 that effect_boltEmitterMessage also hands to effect_handleTimedEffectOwnerEvent as
 * (arg0 + 0x28, arg0 + 0x2C): the source actor (with its unique_id, to detect a reused slot)
 * plus three independent countdowns. */
typedef struct BoltSource {
    /* 0x00 */ struct Actor *obj;
    /* 0x04 */ u8  id;
    /* 0x05 */ u8  pad5;
    /* 0x06 */ s16 flashTimer;
    /* 0x08 */ s16 boltTimer;
    /* 0x0A */ s16 timer2;
} BoltSource; /* size 0xC */

/* Data of the effect run by beam_updateFlashBurst. */
typedef struct FlashBurstData {
    /* 0x00 */ struct Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer; /* frames until the next burst */
} FlashBurstData;

/* Timed effect 0x55 (effect_newTimedEffect, spawned by level_spawnFlashBurst), updated by beam_updateFlashBurst. */
typedef struct FlashBurstEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ FlashBurstData data;
} FlashBurstEffect; /* size 0x30 */

/* Data of the effect spawned by effect_spawnOwnerEffect for an actor. */
typedef struct OwnerEffectData {
    /* 0x00 */ struct Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer;  /* counts down by gFrameTicks while the owner has a camera */
} OwnerEffectData; /* size 0x8 */

/* Timed effect 0x4E (effect_newTimedEffect), spawned by effect_spawnOwnerEffect (game/Effects/OwnerEffect/ownereffect.c) and updated by effect_updateOwnerEffect. */
typedef struct OwnerEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ OwnerEffectData data;
} OwnerEffect; /* size 0x30 */

/* Data of the emitter effect (particle_updateShapeEmitter): spawns points on a shape at a random rate. */
typedef struct ShapeEmitter {
    /* 0x00 */ void *shape;
    /* 0x04 */ f32 rate;
    /* 0x08 */ ZoneNode *zone;   /* collision_findZoneLeaf at the shape's x/z */
    /* 0x0C */ f32 accumulator;
} ShapeEmitter;

/* Zone effects: timed effects (object type 35 or 95, gTimedEffectTypes) that game/Level/ZoneSpawn/zonespawn.c makes for a TriggerZone,
 * with the zone first in their data. effect_findZoneEffect (game/Particle/RegionEmitter/regionemitter.c) finds the ZoneEffect of a
 * zone (callbackSet 0x2C) and beam_findBoltArcEffect (game/Beam/BoltRain/boltrain.c) its BoltArcObj (callbackSet 0x2E, conker/beam.h). */
typedef struct ZoneEffectData {     /* filled by level_spawnZoneEffect */
    /* 0x00 */ TriggerZone *zone;
    /* 0x04 */ u8 id;               /* the zone's data2 */
    /* 0x08 */ ZoneNode *node;      /* collision_findZoneLeaf at the zone's x/z */
    /* 0x0C */ u8 flags;            /* 3 when the zone's data3 is set, else 0 */
    /* 0x10 */ struct Actor *actor; /* NULL at the start; read by func_151A6B68 */
} ZoneEffectData;

typedef struct ZoneEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ZoneEffectData data;
} ZoneEffect;

/* Message 0x35 to the timed effects (beam_onPropArrival, game/Beam/BoltRain/boltrain.c): each zone effect whose
 * ZoneEffectData.id is in ids fires once (particle_handleZoneEffectMessage). */
typedef struct ZoneTriggerMsg {
    /* 0x00 */ s32 count;
    /* 0x04 */ const u8 *ids;
    /* 0x08 */ struct Actor *player; /* stored in ZoneEffectData.actor */
} ZoneTriggerMsg;


/* Text box data (at 0x28 in its object): shows a string one line at a time. */
typedef struct TextLines {
    /* 0x00 */ u8 id; // matched against message 0x51's payload
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u8 *str;
    /* 0x08 */ u8 flags; // bit 0: a new line is being shown this frame
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ u8 *line;
    /* 0x10 */ u8 *lineEnd;
    /* 0x14 */ u8 lineLen;
    /* 0x15 */ u8 page; // which of lines[] holds the current line; the other holds the previous one
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u8 *lines[2]; // NUL-terminated copies of lines, each as long as the longest line
    /* 0x20 */ void *style; // the display style's own state, which follows this struct
    /* 0x24 */ s8 updateFn; // index into gTextBoxUpdateFuncs, or -1
    /* 0x25 */ u8 msgFn; // index into gTextBoxMessageFuncs
    /* 0x26 */ u8 pad26[2];
} TextLines; /* size 0x28 */

/* Text box object created by effect_newTextBox (game/Effects/TextLines/textlines.c); its display style's state follows. */
typedef struct TextObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TextLines text;
} TextObj;

/* Emitter state at +0x28 of effect_updateZonePairEmitter's object. */
typedef struct ZonePairEmitter {
    /* 0x00 */ s16 timer; // frames until the next particle
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ TriggerZone *source;
    /* 0x08 */ TriggerZone *target;
} ZonePairEmitter;

/* Data of timed effect expire callback 5 (particle_emitDiscParticle, game/Particle/DiscEmitter/discemitter.c): particles at a random point of a disc. */
typedef struct DiscEmitter {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
    /* 0x08 */ f32 radius;
    /* 0x0C */ f32 y;
} DiscEmitter; /* size 0x10 */

/* Timed effect with a DiscEmitter as its data: the object of expire callback 5 (particle_emitDiscParticle,
 * game/Particle/DiscEmitter/discemitter.c), made by level_setupBrontoCavern with a 0x10-byte data size. */
typedef struct DiscEmitterEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ DiscEmitter disc;
} DiscEmitterEffect; /* size 0x38 */

/* The timed effect of tick 18 (particle_updateOriginRingEmitter, game/Particle/DiscEmitter/discemitter.c), made by
 * level_setupDinoArena with a 4-byte data size: the spawn accumulator. */
typedef struct OriginRingEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ f32 accum;
} OriginRingEmitter;

/* Sight beams (laser tripwires) and sight sprites, game/Effects/Sight/SightBeam/sightbeam.c and game/Effects/Sight/effects_sight.c. They are here and
 * not in conker/sight.h because they embed DefParticleHead and ScreenSpriteInfo. */

/* Texture list copied into an emitter part by sight_spawnEmitter; its draw handler (gModelParticleDrawFuncs[2],
 * particle_bindListTextures) reads it as a ParticleTextureList. */
typedef struct {
    /* 0x00 */ u8  count;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 texture0;    /* gEffectTextures index, bound to segment0 */
    /* 0x06 */ u8  segment0;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 texture1;
    /* 0x0E */ u8  segment1;
    /* 0x10 */ s32 unk10;
} SightEmitterAnim;

/* Emitter object created by sight_spawnEmitter (a particle_newModelParticle model particle); flag 0x20000 toggles its flash. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ SightEmitterAnim anim;
} SightEmitter;

/* Sight beam state, stored at +0x110 of the beam object (see sight_updateBeam). */
typedef struct {
    f32 texS;
    f32 texT;
    f32 centerS;
    f32 centerT;
    f32 radiusS;
    f32 radiusT;
    f32 angleS;
    f32 angleT;
    f32 speedS;
    f32 speedT;
    TriggerZone *source;
    LightEffect *midEffect;
    s32 unk30;
    struct SightExtension *endEffect0;
    struct SightExtension *endEffect1;
    SightEmitter *emitter0[3];
    SightEmitter *emitter1[3];
    f32 phase;
    u8 alternate;
    CollisionTri triangle0;
    CollisionTri triangle1;
    s16 activeTime;
    s16 inactiveTime;
    s16 timer;
    s16 period;
    u8 flags;
    TriggerZone *beam;
    ZoneNode *zone0;
    ZoneNode *zone1;
    u8 breakHit;      /* index into gSightBeamBreakHits: the scripted hit dealt to the actor that breaks the beam */
    f32 pulseAngle;
    f32 pulseSpeed;
} SightBeamState;

/* Sound handle from sound_addDynamicSound; `state` is the user pointer passed to it. */
typedef struct {
    u8 pad0[0x18];
    SightBeamState *state;
} SightSoundHandle;

/* A sight beam object: spawn descriptor copy at +0x18, quad vertex buffers, beam state at +0x110. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ SightBeamState state;
} SightBeamObject;

/* Effect spawned by sight_spawnExtension; +0x170 holds the beam object that owns it. */
typedef struct SightExtension {
    EffectHeader header;
    u8 pad10[0x160];
    SightBeamObject *owner;
} SightExtension;

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} SightColor;

/* Per-sprite state copied to +0x70 of each spawned sight sprite. */
typedef struct {
    /* 0x00 */ Camera *player;
    /* 0x04 */ f32 alpha;       /* current alpha, eased toward targetAlpha (or 0) by sight_updateZTriggerFade */
    /* 0x08 */ f32 targetAlpha; /* alpha to fade to while Z_TRIG is held */
    /* 0x0C */ u8 flags; /* 1: bouncing, 2: colour flash, 4: value from gSeqTrackStates[sourceIndex] */
    /* 0x0D */ u8 kind;  /* index into the gSightSpriteMessageHandlers message handlers */
    /* 0x0E */ u8 side;
    /* 0x0F */ u8 padF;
    /* 0x10 */ f32 bouncePhase;
    /* 0x14 */ f32 fade;
    /* 0x18 */ f32 rangeMin;
    /* 0x1C */ f32 rangeMax;
    /* 0x20 */ f32 rangeScale;
    /* 0x24 */ f32 angle;
    /* 0x28 */ f32 lastValue;
    /* 0x2C */ SightColor flashColor;
    /* 0x3C */ SightColor color;
    /* 0x4C */ f32 baseHalfHeight; /* gauge half height at rangeMin (sight_updateRingGauge) */
    /* 0x50 */ f32 flashTimer;
    /* 0x54 */ s8 sourceIndex;
    /* 0x55 */ u8 pad55[3];
} SightSpritePayload;

/* A spawned sight sprite: the spawn descriptor lives on at +0x10, the payload at +0x70. */
typedef struct {
    EffectHeader header;
    ScreenSpriteInfo desc;
    u8 pad68[8];
    SightSpritePayload payload;
} SightSprite;

/*
 * Template / payload of a type 0xE QuadFx object (game/Effects/ObjectList/objectlist.c): a textured quad pair animated through an
 * TextureDesc. effect_initQuadFxTemplate fills a template's vertices, effect_spawnQuadFx copies it into a new object.
 * The bytes at 0x80-0x87 belong to the callback (gParticleCallbacks): cb has one member per callback that uses them
 * (callers without such a callback write the rain names).
 */
typedef struct QuadFxData {
    /* 0x00 */ Vtx vtx[8];
    /* 0x80 */ union {
        struct {
            /* 0x80 */ s32 dropFlags;   /* 1: hidden until it first wraps, 2: remove when it next wraps */
            /* 0x84 */ s16 slot;        /* index in gRainDrops */
            /* 0x86 */ s16 unk86;
        } rain;                         /* callback 1, effect_updateRainDrop (game/Effects/Rain/rain.c) */
        struct {
            /* 0x80 */ s32 timer;       /* ticks since the spawn */
            /* 0x84 */ s16 widthScale;  /* halfWidth = widthScale * 14 / 256 (gMaterialImpactScales[size][0]) */
            /* 0x86 */ s16 heightScale; /* halfHeight ramps up to 90/256 of it and back (gMaterialImpactScales[size][1]) */
        } impact;                       /* callback 13, effect_updateMaterialImpactQuad (game/Effects/MaterialImpact/materialimpact.c) */
        struct {
            /* 0x80 */ u8 pad80[4];
            /* 0x84 */ s16 fallSpeed;
            /* 0x86 */ s16 maxHalfHeight;
        } falling;                      /* callback 15, effect_updateFallingObject (game/Effects/FallingObject/fallingobject.c) */
    } cb;
    /* 0x88 */ s16 frame;       /* 8.8 fixed point; -1 = finished */
    /* 0x8A */ s16 frameStep;   /* 8.8 fixed point */
    /* 0x8C */ s16 x;
    /* 0x8E */ s16 y;
    /* 0x90 */ s16 z;
    /* 0x92 */ s16 halfWidth;   /* corner offsets +-halfWidth across the view (effect_drawQuadFx) */
    /* 0x94 */ s16 halfHeight;  /* corners at -h/+h; flags 0x200 / 0x4000 anchor one edge (0..+-2h) */
    /* 0x96 */ s16 floorY;      /* floor height under the spawn point */
    /* 0x98 */ u16 flags;       /* 0x4: finish at either end, 0x40: ping-pong, 0x80/0x100: flip s/t */
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ u16 heading;     /* rain: binary angle, high byte indexes the sin/cos tables */
    /* 0xA0 */ u8 animIndex;    /* index into gEffectAnimTextures */
    /* 0xA1 */ u8 speed;        /* rain: fall speed */
    /* 0xA2 */ s8 callback;     /* index into gParticleCallbacks, -1 = none */
    /* 0xA3 */ u8 unkA3;
    /* 0xA4 */ u8 alpha;        /* prim alpha with flags & 0x800 */
} QuadFxData; /* size 0xA8 */

/* Type 0xE object. */
typedef struct QuadFx {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ QuadFxData data;
} QuadFx; /* size 0xB8 */

/* An entry of the render-mode table gEffectRenderModes: the blender words of the two cycles, OR'd together for
 * math_gfxSetOtherMode. */
typedef struct RenderModePair {
    /* 0x0 */ s32 cycle1;
    /* 0x4 */ s32 cycle2;
} RenderModePair; /* size 0x8 */

/* Extra data of a pulsing particle: its size wobbles around baseSize as phase runs (to 4 pi in game/Particle/SoundPulse/soundpulse.c,
 * cycling through 2 pi in game/Particle/Pulse/pulse.c). */
typedef struct {
    /* 0x0 */ f32 phase;
    /* 0x4 */ f32 baseSize;
} Pulse; /* size 0x8 */

/* A pulsing particle (particle_spawnSoundPulse in game/Particle/SoundPulse/soundpulse.c, particle_spawnPulse in game/Particle/Pulse/pulse.c): a ModelParticle
 * carrying a Pulse as its data. */
typedef struct PulseParticle {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ Pulse pulse;
} PulseParticle;

#endif
