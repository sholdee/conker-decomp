#ifndef EFFECTS_H
#define EFFECTS_H

/* Particle / effect spawn descriptors shared by the spawn functions and their callers. */

/* Common head of the effect / particle objects (the first 0x10 bytes of each): the link of the object lists
 * D_800DCE50[kind][type] (func_15168A4C in game_1944C0.c pushes an object onto its list). */
typedef struct EffectHeader {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 kind;    /* also the object list it is linked into */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ struct EffectHeader *prev;
    /* 0x08 */ struct EffectHeader *next;
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 state;   /* -1 frees the object */
} EffectHeader; /* size 0x10 */

struct Actor;
struct ParticleViewMask;

/* ParticleDesc.flags (sprite particles, func_15130280) */
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
#define PARTICLE_LIT               0x10000  /* env color comes from func_1515E888 */
#define PARTICLE_VIEW_MASK         0x20000
#define PARTICLE_VIEWS             0x40000
#define PARTICLE_SCALE_WIDTH_ONLY  0x80000
#define PARTICLE_SCALE_HEIGHT_ONLY 0x100000
#define PARTICLE_MAX_SIZE          0x200000
#define PARTICLE_MIN_SIZE          0x400000

/* Spawn parameters of a sprite particle; func_15130280 / func_15130374 / func_151303BC
 * (game_15D730.c) copy them to Particle.desc. */
typedef struct ParticleDesc {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
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
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
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
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 maxSize;
} ParticleDesc; /* size 0x70 */

/* matching: the first 0x64 bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc64 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
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
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
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
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
} ParticleDesc64;

/* matching: the first 0x68 bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc68 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
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
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
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
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
} ParticleDesc68;

/* matching: the first 0x6C bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc6C {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
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
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
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
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
} ParticleDesc6C;

/* Spawn template of a model (debris) particle passed to func_15132A4C (game_15F680.c):
 * the layout of that Particle's bytes 0x10..0x8B. */
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
    /* 0x5C */ s16 *unk5C;
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
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
} ParticleSpawn; /* size 0x7C */

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
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
    /* 0x5C */ s16 *unk5C;
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
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

/* matching: ParticleSpawn plus 0x4 bytes; the stack frame needs the larger size. */
typedef struct ParticleSpawn80 {
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
    /* 0x5C */ s16 *unk5C;
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
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
    /* 0x7C */ u8 pad7C[0x4];
} ParticleSpawn80;

/* A particle's width and height, copied as one struct by some callers. */
typedef struct ParticleSize {
    /* 0x0 */ f32 width;
    /* 0x4 */ f32 height;
} ParticleSize;

/* Spawn template of a particle, func_1513D2F0 / func_1513D4B8 / func_1513D524 / func_1513D594
 * (game_169510.c) copy it to Particle.def. */
typedef struct ParticleDef {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 fadeStart;   /* lifetime below which unk44 / rgba[3] fade */
    /* 0x56 */ s16 fadeRate;
} ParticleDef; /* size 0x58 */

/* matching: the first 0x48 bytes of ParticleDef; a full ParticleDef on the stack changes the frame. */
typedef struct ParticleDef48 {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
} ParticleDef48;

/* matching: the first 0x4C bytes of ParticleDef; a full ParticleDef on the stack changes the frame. */
typedef struct ParticleDef4C {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
} ParticleDef4C;

/* matching: the first 0x54 bytes of ParticleDef; a full ParticleDef on the stack changes the frame. */
typedef struct ParticleDef54 {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
} ParticleDef54;

/* Debris emitter settings read by func_151A6F00 (game_1D43B0.c). Angles are 0..255 binary angles. */
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
    /* 0x54 */ u8 landPuffs;   /* see DebrisData in game_1D43B0.c */
    /* 0x55 */ u8 landPuffFlag;
    /* 0x56 */ s16 landPuffLifeBase;
    /* 0x58 */ s16 landPuffLifeRange;
} DebrisEmitter; /* size 0x5C */

/* Spawn header of an effect object, copied into EffectObj.header by func_15147A80 (game_174BF0.c);
 * the template of trails, streams and other func_15147A80 effects. */
typedef struct EffectTemplate {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;     /* bit 0: life counts down; 0x10: run unk14's handler; 0x20: pass unk18 to the draw */
    /* 0x10 */ s32 kind;      /* index into D_8008A2F0 / D_8008A340 / D_8008A390 */
    /* 0x14 */ s8 unk14;      /* index into D_8008A284, -1: none */
    /* 0x15 */ u8 itemCount;  /* number of items allocated after the user data */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void *unk18;
} EffectTemplate; /* size 0x1C */

/* Spawn parameters of a screen effect, copied to ScreenEffect.desc by func_1515FF74 (game_18D250.c). */
typedef struct EffectDesc {
    /* 0x0 */ u8 flags;        /* 1: ends when lifetime runs out, 2/8: hidden in some states, 4: skip one draw */
    /* 0x1 */ s8 updateIndex;  /* D_8008B0D0 handler, -1 for none */
    /* 0x2 */ s8 drawIndex;    /* D_8008B0D8 handler, -1 for none */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 lifetime;
    /* 0x6 */ u8 messageIndex; /* D_8008B0E4 handler */
    /* 0x7 */ u8 pad7;
} EffectDesc; /* size 0x8 */

/* Spawn parameters of a blood drip, copied to BloodDrip.target by func_1513418C (game_161520.c). */
typedef struct BloodDripDesc {
    /* 0x00 */ s32 target[2];  /* matrices to transform offset by, double-buffered */
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
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
} BloodDripDesc; /* size 0x30 */

/* BloodDripDesc.flags */
#define BLOOD_DRIP_FOLLOW_OWNER  0x01
#define BLOOD_DRIP_ON_BONE       0x02   /* offset is in the space of owner's bone `bone` */
#define BLOOD_DRIP_LIFETIME      0x04   /* lifetime counts down */
#define BLOOD_DRIP_NO_PREVIOUS   0x08
#define BLOOD_DRIP_STATIC        0x10

/* Object attached to a glow (freed with func_1517E134): follows the glow's position and takes its alpha. */
typedef struct GlowAttach {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ u8 padC[0x24];
    /* 0x30 */ u8 alpha;
} GlowAttach;

/* Glow animations (GlowParams.anim); GlowParams.type selects the handler (game_16DC80.c). */

/* func_15141478: random flicker */
typedef struct GlowFlicker {
    /* 0x00 */ f32 base;
    /* 0x04 */ f32 low;
    /* 0x08 */ f32 high;
    /* 0x0C */ f32 target;
    /* 0x10 */ f32 timer;
    /* 0x14 */ f32 period;
    /* 0x18 */ f32 rate;
} GlowFlicker; /* size 0x1C */

/* func_15141564: sine pulse */
typedef struct GlowPulse {
    /* 0x00 */ f32 base;
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
} GlowPulse; /* size 0x10 */

/* func_151415D4: periodic rise / hold / fall */
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

/* func_15141928: placed around a pivot */
typedef struct GlowOrbit {
    /* 0x00 */ f32 distance;
    /* 0x04 */ f32 spread;
    /* 0x08 */ f32 *angles;
    /* 0x0C */ f32 pivot[3];
} GlowOrbit; /* size 0x18 */

/* Shape of a glow sprite (game_16DC80.c): the part of GlowParams every caller of func_151407D0 fills in. */
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
    /* 0x44 */ GlowAttach *attach;
    /* 0x48 */ f32 scale;
    /* 0x4C */ u16 zbufSample[4]; /* z-buffer value at the glow's screen position, per view */
    /* 0x54 */ u8 visibility[4];  /* per view, eases towards 0xFF (visible) or 0 */
    /* 0x58 */ u8 type;           /* index into the D_80089F9C/D_80089FE4/D_8008A02C handler tables */
    /* 0x59 */ s8 handler;        /* index into D_80089F90, or -1 */
    /* 0x5A */ u8 pad5A[2];
} GlowShape; /* size 0x5C */

/* Glow parameters: func_151407D0 (game_16DC80.c) copies paramSize bytes of them into the glow at +0x110. */
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

/* Attachment of a type 0xB glow (func_15189FF0, game_1B74A0.c), stored in GlowParams.anim's place at
 * glow+0x170: two points (p0, p1) given in the space of bone `bone` of `target`, which func_1518A094
 * transforms every frame. */
typedef struct EmitterAttach {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ struct Actor *target;
    /* 0x1C */ u8 bone;
    /* 0x1D */ u8 targetId;   /* the target's unique_id */
    /* 0x1E */ s8 handler;    /* index into D_8008D5A0, or -1 */
    /* 0x1F */ u8 pad1F;
} EmitterAttach; /* size 0x20 */

/* An RGBA color. */
typedef struct Color8 {
    /* 0x0 */ u8 red;
    /* 0x1 */ u8 green;
    /* 0x2 */ u8 blue;
    /* 0x3 */ u8 alpha;
} Color8;

/* Spawn parameters of a screen sprite, copied to ScreenSprite.info by func_1515548C (game_182140.c). */
typedef struct ScreenSpriteInfo {
    /* 0x00 */ f32 x;           /* centre, in viewport units */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 halfWidth;
    /* 0x0C */ f32 halfHeight;
    /* 0x10 */ u8 texture;    /* index into D_80090B60, 0xFF = none */
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 timer;     /* counts down by D_800BE9E4 when (flags & 1) */
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 fadeStart; /* (flags & 8): below this timer, alpha = timer * fadeRate */
    /* 0x18 */ s16 fadeRate;
    /* 0x1A */ u8 kind;       /* index into D_8008AD04 */
    /* 0x1B */ Color8 color1;
    /* 0x1F */ Color8 color2;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 otherModeH;  /* ORed with unk38, unk3C into the othermode-H word */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 combineMode; /* index into D_800A4AC8 */
    /* 0x34 */ s32 unk34;       /* its low byte (0x37) is func_1513F4E4's combiner argument */
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ u8 viewport;   /* the only viewport drawn in when (flags & 0x10) */
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 scaleX;    /* (flags & 0x80) */
    /* 0x4C */ f32 scaleY;
    /* 0x50 */ f32 offsetS;   /* (flags & 0x100) */
    /* 0x54 */ f32 offsetT;
} ScreenSpriteInfo; /* size 0x58 */

/* matching: ScreenSpriteInfo plus 0x4 bytes; the stack frame needs the larger size. */
typedef struct ScreenSpriteInfo5C {
    /* 0x00 */ f32 x;           /* centre, in viewport units */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 halfWidth;
    /* 0x0C */ f32 halfHeight;
    /* 0x10 */ u8 texture;    /* index into D_80090B60, 0xFF = none */
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 timer;     /* counts down by D_800BE9E4 when (flags & 1) */
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 fadeStart; /* (flags & 8): below this timer, alpha = timer * fadeRate */
    /* 0x18 */ s16 fadeRate;
    /* 0x1A */ u8 kind;       /* index into D_8008AD04 */
    /* 0x1B */ Color8 color1;
    /* 0x1F */ Color8 color2;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 otherModeH;  /* ORed with unk38, unk3C into the othermode-H word */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 combineMode; /* index into D_800A4AC8 */
    /* 0x34 */ s32 unk34;       /* its low byte (0x37) is func_1513F4E4's combiner argument */
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ u8 viewport;   /* the only viewport drawn in when (flags & 0x10) */
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 scaleX;    /* (flags & 0x80) */
    /* 0x4C */ f32 scaleY;
    /* 0x50 */ f32 offsetS;   /* (flags & 0x100) */
    /* 0x54 */ f32 offsetT;
    /* 0x58 */ u8 pad58[0x4];
} ScreenSpriteInfo5C;

/* Sprite animation of a billboard particle (game_1944C0.c): `frames` is one segmented texture address
 * (< 0x10000000) or a table of `frameCount` texture addresses. */
typedef struct SpriteAnim {
    /* 0x0 */ s32 *frames;
    /* 0x4 */ u8 frameCount;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 width;
    /* 0x8 */ u16 height;
    /* 0xA */ u8 unkA;       /* 5: two-texture (two-cycle) animation */
} SpriteAnim;

/* BillboardTemplate.flags (billboard particles, game_1944C0.c) */
#define BILLBOARD_ENV_COLOR     0x1    /* use envR/G/B instead of the per-texture default */
#define BILLBOARD_PINGPONG      0x2    /* reverse the animation at either end instead of wrapping */
#define BILLBOARD_FLIP_S        0x4
#define BILLBOARD_FLIP_T        0x8
#define BILLBOARD_DIE_AT_END    0x20
#define BILLBOARD_DIE_AT_START  0x40
#define BILLBOARD_LOOPED        0x80   /* set once the animation has passed its last frame */
#define BILLBOARD_CUSTOM_MODE   0x100  /* render/combine modes come from unk4 */
#define BILLBOARD_2CYCLE        0x200

/* Spawn template of a billboard particle (type 5 / 0x42): func_15167D84 (game_1944C0.c) copies it to
 * the particle at +0x10. */
typedef struct BillboardTemplate {
    /* 0x00 */ SpriteAnim *anim;
    /* 0x04 */ s32 unk4;       /* BILLBOARD_CUSTOM_MODE: bits 0-7 render mode, bits 16-31 combine mode;
                                  else free for the particle's callback */
    /* 0x08 */ s32 unk8;       /* low byte: angle offset in 1/256ths of a turn */
    /* 0x0C */ s16 frame;      /* 8.8 fixed point index into anim->frames */
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
    /* 0x1F */ s8 callback;    /* index into D_8008C9C8, -1 = none */
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

/* Type 5 / 0x42 object: a moving, animated billboard particle (created by func_15167D84 in game_1944C0.c).
 * A spawner's extra data follows it (particle + 1). */
typedef struct BillboardParticle {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ BillboardTemplate tmpl;
    /* 0x48 */ s8 onDeath;      /* index into D_8008CA20, -1 = none */
    /* 0x49 */ u8 pad49[7];
} BillboardParticle; /* size 0x50 */

/* Random spawn angles: yaw = yawBase + rand % (yawRange + 1), likewise pitch (256-step binary angles).
 * Most spawn configs of game_17CAF0.c store random ranges as base/range pairs like this. */
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

/* Per-trail settings, copied into the trail (Trail.data) by func_1515C2F0 (game_1897A0.c). */
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
    /* 0x20 */ u8 renderMode; /* index into D_800A4AC8 */
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s32 setGeometryMode;
    /* 0x28 */ s32 clearGeometryMode;
    /* 0x2C */ s16 fadeStart;  /* life below which the alpha fades */
    /* 0x2E */ s16 fadeRate;   /* alpha per unit of life */
    /* 0x30 */ s16 shrinkStart; /* life below which the width shrinks */
    /* 0x32 */ u8 shrinkPaused;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ f32 shrinkFactor; /* width multiplier per frame */
    /* 0x38 */ s8 unk38;      /* index into D_8008B080, -1: none */
    /* 0x39 */ s8 unk39;      /* index into D_8008B084, -1: none */
    /* 0x3A */ u8 pad3A[2];
} TrailBurstParams; /* size 0x3C */

/* func_15152B38 (game_17CAF0.c): spawns countBase + rand % (countRange + 1) trails (func_1515C2F0) at pos,
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
    /* 0x58 */ u8 unk21;           /* TrailBurstParams.unk21 */
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 setGeometryMode;
    /* 0x60 */ s32 clearGeometryMode;
    /* 0x64 */ s16 fadeStart;
    /* 0x66 */ s16 fadeRate;
    /* 0x68 */ s16 shrinkStart;
    /* 0x6A */ u8 shrinkPaused;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 shrinkFactor;
    /* 0x70 */ s8 unk38;           /* TrailBurstParams.unk38 */
    /* 0x71 */ s8 unk39;           /* TrailBurstParams.unk39 */
    /* 0x72 */ s8 unk72;           /* not read by func_15152B38; callers store -1 */
    /* 0x73 */ s8 unk73;
} TrailBurst; /* size 0x74 */

/* func_15153F18 / func_15153CCC / func_15153C84 (game_17CAF0.c): throws countBase + rand % (countRange + 1)
 * splashes (func_151DA6F8, game_2062D0.c) from pos, each value drawn from its base/range pair. */
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
    /* 0x30 */ s16 unk30;           /* base of func_151DA6F8's arg4 */
    /* 0x32 */ s16 unk32;           /* its range */
    /* 0x34 */ u8 kind;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 unk38;           /* probability of func_151DA6F8's arg7 */
    /* 0x3C */ s16 unk3C;           /* func_151DA6F8's argD */
    /* 0x3E */ s16 unk3E;           /* func_151DA6F8's argE */
    /* 0x40 */ s32 unk40;           /* func_151DA6F8's argF (EffectTemplate.unk18) */
} SplashSpray; /* size 0x44 */

/* What the callers of func_15153F18 build: the direction ranges followed by the spray. */
typedef struct SplashSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ SplashSpray spray;
} SplashSpraySpec; /* size 0x4C */

/* func_15150178 / func_1514FF44 / func_1514FEFC (game_17CAF0.c): throws countBase + rand % (countRange + 1)
 * wobbling drops (func_151D9014, game_2062D0.c) from pos, each value drawn from its base/range pair. */
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
    /* 0x24 */ u8 unk24;            /* base of func_151D9014's arg5 */
    /* 0x25 */ u8 unk25;            /* its range */
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ f32 sizeBase;
    /* 0x2C */ f32 sizeRange;
    /* 0x30 */ s8 unk30;            /* not read by the spray functions */
    /* 0x31 */ u8 kind;
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ f32 unk34;           /* probability of func_151D9014's arg7 */
    /* 0x38 */ u8 wobbles;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 bounceChance;
} DropSpray; /* size 0x40 */

/* What the callers of func_15150178 build: the direction ranges followed by the spray. */
typedef struct DropSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ DropSpray spray;
} DropSpraySpec; /* size 0x48 */

/* func_15152190 (game_17CAF0.c): throws countBase + rand % (countRange + 1) model pieces (func_15132A4C) out of a
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

/* A random direction inside a cone around `direction`, built lazily by func_1514F6E8 (game_17CAF0.c);
 * the first argument of func_1514FF44 / func_15153CCC. */
typedef struct ConeBasis {
    /* 0x00 */ u8 flags;         /* 1: direction normalized and scaled, 2: axes built, 4: axes normalized */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f direction;
    /* 0x10 */ Vec3f axis0;
    /* 0x1C */ Vec3f axis1;
    /* 0x28 */ f32 spread;
} ConeBasis; /* size 0x2C */

/* Three s16 vertices of a collision triangle, as stored in a ray-cast result. */
typedef struct SurfaceTriangle {
    /* 0x00 */ s16 v[9];
} SurfaceTriangle; /* size 0x12 */

/* A cone described by a surface triangle (three s16 vertices) and the side its normal faces
 * (func_1514F640); the first argument of func_1514FEFC / func_15153C84. */
typedef struct TriangleSource {
    /* 0x00 */ Vec3f normal;
    /* 0x0C */ SurfaceTriangle triangle;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ f32 spread;
} TriangleSource; /* size 0x24 */

/* func_1514F8F8 (game_17CAF0.c): countBase + rand % (countRange + 1) jets of particles from pos
 * (func_151A2AD4 / func_151A2A14, game_1CE2F0.c), each with particleCount particles whose speed falls off
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
    /* 0x50 */ s8 eventFunc;      /* index into D_8008F8E0, -1: none */
    /* 0x51 */ u8 pad51[3];
} JetSpray; /* size 0x54 */

/* func_1514FCE8: jets in random yaw/pitch directions from `angles`. */
typedef struct JetSpraySpec {
    /* 0x00 */ AngleRange angles;
    /* 0x08 */ JetSpray spray;
} JetSpraySpec; /* size 0x5C */

/* func_1514FB98: jets inside a cone of the given spread around `direction`. */
typedef struct DirectedJetSpray {
    /* 0x00 */ Vec3f direction;
    /* 0x0C */ f32 spread;
    /* 0x10 */ JetSpray spray;
} DirectedJetSpray; /* size 0x64 */

/* func_1514FBFC: jets inside a cone around the normal of a surface triangle. */
typedef struct SurfaceJetSpray {
    /* 0x00 */ TriangleSource source;
    /* 0x24 */ JetSpray spray;
} SurfaceJetSpray; /* size 0x78 */

/* Render settings of a func_15147DA0 effect (game_175250.c), its 12th argument, copied into the effect. */
typedef struct TrailRender {
    /* 0x00 */ s32 unk0;              /* ORed into the other-mode high word */
    /* 0x04 */ s32 setGeometryMode;
    /* 0x08 */ s32 clearGeometryMode;
    /* 0x0C */ s32 renderMode;        /* index into D_800A4AC8 */
    /* 0x10 */ s32 unk10;             /* its low byte is func_1513F4E4's argument */
    /* 0x14 */ s32 unk14;             /* ORed into the other-mode high word */
    /* 0x18 */ s32 unk18;             /* ORed into the other-mode high word */
    /* 0x1C */ s8 primMode;           /* mode of func_151441A4 (prim color) */
    /* 0x1D */ s8 envMode;            /* mode of func_151442FC (env color) */
    /* 0x1E */ u8 pad1E[2];
} TrailRender; /* size 0x20 */

/* func_15151A38 (game_17CAF0.c): countBase + rand % (countRange + 1) puffs (func_15147DA0 effects) at pos,
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
    /* 0x3C */ s32 handlers[6];     /* func_15147DA0's arguments 3..8 (state and handler indices) */
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x57 */ u8 pad57;
    /* 0x58 */ s32 unk58;           /* func_15147DA0's arg9 */
    /* 0x5C */ s32 unk5C;           /* func_15147DA0's arg10 */
    /* 0x60 */ TrailRender render;
    /* 0x80 */ s16 unk80;           /* copied to the motion block's 0x1C */
    /* 0x82 */ s16 unk82;           /* copied to the motion block's 0x1E */
    /* 0x84 */ s32 unk84;           /* EffectTemplate.unk18 */
} PuffSpray; /* size 0x88 */

/* func_15151670 (game_17CAF0.c): like PuffSpray, but each puff moves in a random direction inside the cone
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
    /* 0x58 */ s32 handlers[6];     /* func_15147DA0's arguments 3..8 (state and handler indices) */
    /* 0x70 */ u8 blue;
    /* 0x71 */ u8 alphaBase;
    /* 0x72 */ u8 alphaRange;
    /* 0x73 */ u8 pad73;
    /* 0x74 */ s32 unk74;           /* func_15147DA0's arg9 */
    /* 0x78 */ s32 unk78;           /* func_15147DA0's arg10 */
    /* 0x7C */ TrailRender render;
    /* 0x9C */ s16 unk9C;           /* copied to the motion block's 0x1C */
    /* 0x9E */ s16 unk9E;           /* copied to the motion block's 0x1E */
    /* 0xA0 */ s32 unkA0;           /* EffectTemplate.unk18 */
} SurfacePuffSpray; /* size 0xA4 */

/* func_15153634 (game_17CAF0.c): countBase + rand % (countRange + 1) sprite particles (func_15130280) at pos,
 * each moving at a random yaw/pitch; the ParticleDesc fields are copied, the base/range pairs drawn per
 * particle, and `extra` is copied into each particle at +0xA8. Note the yaw/pitch order differs from
 * AngleRange. */
typedef struct SpriteBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ u8 texture;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 renderMode;
    /* 0x08 */ s32 unk8;            /* ParticleDesc.unk0 */
    /* 0x0C */ s32 unkC;            /* ParticleDesc.unk4 */
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
    /* 0x64 */ u8 unk64;            /* func_15130280's second argument */
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 extra;           /* copied into each particle at +0xA8 */
} SpriteBurst; /* size 0x6C */

/* func_15152F70 (game_17CAF0.c): PuffSpray with a fixed alpha, no per-puff alpha range and fixed render
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
    /* 0x3C */ s32 handlers[6];     /* func_15147DA0's arguments 3..8 (state and handler indices) */
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alpha;
    /* 0x56 */ u8 pad56[2];
} SimplePuffSpray; /* size 0x58 */

/* func_1515080C (game_17CAF0.c): DebrisBurst whose pieces each use a different entry of the variant tables,
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

/* func_15150D1C (game_17CAF0.c): casts countBase + rand % (countRange + 1) rays from pos in random directions
 * and spawns a func_151D9B8C emitter on each suitable surface hit closer than maxDistance. */
typedef struct DecalSpray {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ AngleRange angles;
    /* 0x1C */ f32 maxDistance;
    /* 0x20 */ f32 sizeBase;
    /* 0x24 */ f32 sizeRange;
    /* 0x28 */ s16 unk28;           /* base of func_151D9B8C's arg2 */
    /* 0x2A */ s16 unk2A;           /* its range */
    /* 0x2C */ u8 kind;
    /* 0x2D */ u8 pad2D[3];
} DecalSpray; /* size 0x30 */

/* func_15150F90 (game_17CAF0.c): countBase + rand % (countRange + 1) randomly rotated pieces (func_1518A3C0)
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

/* func_15151D6C (game_17CAF0.c): DebrisBurst thrown off a surface triangle, inside the cone around its
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

/* func_151539B4 (game_17CAF0.c): countBase + rand % (countRange + 1) streaks (func_15156190) at pos, each
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

struct EffectMask;

/* func_15157010 (game_1844C0.c): spawn parameters of a model effect, copied into the effect. */
typedef struct EffectParams {
    /* 0x00 */ u8 flags;
    /* 0x01 */ s8 updateFunc;      /* index into D_8008AD90, -1 = none */
    /* 0x02 */ s8 updateFunc2;     /* index into D_8008ADA0, -1 = none */
    /* 0x03 */ s8 preDrawFunc;     /* index into D_8008ADBC, -1 = none */
    /* 0x04 */ s8 postDrawFunc;    /* index into D_8008ADCC, -1 = none */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 lifetime;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ TrailRender render;
    /* 0x30 */ Color8 color0;
    /* 0x34 */ Color8 color1;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ struct Actor *owner; /* see func_15169850 */
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f pos;
    /* 0x50 */ struct EffectMask *mask;
    /* 0x54 */ s16 fadeStart;      /* alpha fades once lifetime drops below this */
    /* 0x56 */ s16 fadeRate;
} EffectParams; /* size 0x58 */

/* func_15153298 (game_17CAF0.c): spawns countBase + rand % (countRange + 1) billboards (func_15167D84) at
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
    /* 0x38 */ SpriteAnim *anim;
    /* 0x3C */ u8 color;           /* prim r = g = b */
    /* 0x3D */ u8 alpha;
    /* 0x3E */ u8 pad3E[2];
} BillboardBurst; /* size 0x40 */

/* Motion extension of a model effect: func_15157898 (game_1844C0.c) copies it to Effect.extra, and the
 * D_8008AD90 / D_8008ADA0 update callbacks integrate it. */
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

/* func_15134DAC (game_161520.c): spawn parameters of a blood strip (object type 0x29), copied to the strip
 * at +0x10. Every duration frames (after pause) offsetA/offsetB are sampled in bone space. */
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
    /* 0x2E */ s8 sampleCallback;   /* index into D_80089B1C, -1 = none */
    /* 0x2F */ s8 emitCallback;     /* index into D_80089B38 */
    /* 0x30 */ s8 rateCallback;     /* index into D_80089B5C, -1 = none */
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ f32 rate;
    /* 0x38 */ u8 mode;
    /* 0x39 */ s8 updateCallback;   /* index into D_80089B6C, -1 = none */
    /* 0x3A */ u8 pad3A[2];
} BloodStripDesc; /* size 0x3C */

/* func_15154684 (game_17CAF0.c): countBase + rand % (countRange + 1) func_151C5F44 particles (game_1F2730.c)
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
    /* 0x20 */ f32 unk20;           /* copied to the particle's data */
    /* 0x24 */ u8 alphaBase;        /* ParticleDesc.alpha */
    /* 0x25 */ u8 alphaRange;
    /* 0x26 */ s16 timerBase;       /* ParticleDesc.timer; -1 = 300 */
    /* 0x28 */ s16 timerRange;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 sizeBase;
    /* 0x30 */ f32 sizeRange;
    /* 0x34 */ f32 altChance;       /* probability of the alternative update function */
    /* 0x38 */ void *ground;        /* func_15130280's ground argument */
} ParticleBurst; /* size 0x3C */

/* func_151511FC (game_17CAF0.c): countBase + rand % (countRange + 1) func_1513D2F0 particles at position,
 * each launched inside a cone of half-width `spread` around the normal of `triangle` (flipped to face
 * `normal`). */
typedef struct SurfaceBurst {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ Vec3f normal;
    /* 0x1C */ SurfaceTriangle triangle;
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
    /* 0x48 */ u8 unk44Base;        /* ParticleDef.unk44 */
    /* 0x49 */ u8 unk44Range;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ f32 unk4C;           /* base and range of the particle's motion word at +0x11C */
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 speedBase;
    /* 0x58 */ f32 speedRange;
    /* 0x5C */ u8 unk46;            /* ParticleDef.unk46 */
    /* 0x5D */ u8 unk47;            /* ParticleDef.unk47 */
    /* 0x5E */ u8 pad5E[2];
    /* 0x60 */ s32 unk60;           /* func_1513D2F0's model argument */
    /* 0x64 */ s32 unk64;           /* func_1513D2F0's arg8 */
    /* 0x68 */ s16 fadeStart;
    /* 0x6A */ s16 fadeRate;
    /* 0x6C */ f32 unk6C;           /* the particle's motion word at +0x120 */
    /* 0x70 */ f32 spin;            /* spin.x / spin.z are random in [-spin, spin] */
} SurfaceBurst; /* size 0x74 */

/* func_150CCEB0 (game_FA360.c): spawn description of a drifting quad (filled by func_15152874). */
typedef struct DriftSpawn {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 drag;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ Color8 color;        /* alpha goes to ParticleDef.unk44 */
    /* 0x24 */ s32 life;
    /* 0x28 */ ParticleSize size;
    /* 0x30 */ f32 minVy;
    /* 0x34 */ u8 pad34[4];
} DriftSpawn; /* size 0x38 */

/* func_15152874 (game_17CAF0.c): countBase + rand % (countRange + 1) drifting quads (func_150CCEB0) at pos,
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
    /* 0x30 */ s8 colorFunc;        /* index into D_8008AC60, or -1 for opaque white */
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

/* func_15150400 (game_17CAF0.c; the direction ranges are a separate argument): throws countBase + rand %
 * (countRange + 1) model pieces (func_1513264C) from a sphere of the given radius around origin, each a random
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
    /* 0x34 */ void *floor;         /* func_1513264C's floor argument */
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

/* Spawn template of a sprite: func_1513C350 / func_1513C4EC (game_169510.c) copy it to the sprite at +0x18. */
typedef struct SpriteDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;          /* D_80090B60 index, 0xFF = untextured */
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 frame;           /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 unk10;            /* faded out with the lifetime (flag 0x8000) */
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 renderModes;     /* low byte: D_800A4AC8 index, high half: combiner index */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;       /* lifetime below which unk10 / rgba[3] fade */
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
} SpriteDef; /* size 0x28 */

/* matching: a 0x2C stack copy of SpriteDef (game_E6A10.c); the extra word keeps func_150B9D8C's frame layout. */
typedef struct SpriteDef2C {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 frame;
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 renderModes;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ u8 pad28[4];
} SpriteDef2C; /* size 0x2C */

/* func_15156190 / func_15156388 (game_183640.c): spawn parameters of a streak particle. */
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

/* Spawn parameters for each bolt of a chain (game_133190.c); "base + rand % (range + 1)" or "base + randf * range". */
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

/* Parameters of a lightning bolt (game_133190.c): a cubic Bezier from start to end, split into jittered segments. */
typedef struct BoltDesc {
    /* 0x00 */ s16 life;
    /* 0x02 */ u8 flags;          /* bit 0: life counts down */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ Vec3f ctrl[4];     /* Bezier control points; ctrl[0] and ctrl[3] are the end points */
    /* 0x34 */ s8 updateKind;     /* index into D_80088C10, -1: none */
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 segments;
    /* 0x3C */ f32 jitter;        /* max sideways offset of each joint */
    /* 0x40 */ f32 width;         /* half width of the drawn strip */
    /* 0x44 */ s16 rejitterBase;  /* frames between re-jitters: base + rand % (range + 1) */
    /* 0x46 */ s16 rejitterRange;
    /* 0x48 */ Color8 color;
    /* 0x4C */ u8 kind;           /* index into D_80088C18 / D_80088C28 */
    /* 0x4D */ u8 pad4D[3];
} BoltDesc; /* size 0x50 */

/* func_1518F5D0 (game_1BC650.c): emitter template (D_8008D630), copied into AnchorEmitterParams after `accum`. */
typedef struct AnchorEmitterTemplate {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ f32 burstBase;
    /* 0x0C */ f32 burstRange;
    /* 0x10 */ f32 sizeX;
    /* 0x14 */ f32 sizeY;
    /* 0x18 */ f32 sizeXRange;
    /* 0x1C */ f32 sizeYRange;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21Range;
    /* 0x22 */ s16 lifeMin;
    /* 0x24 */ s16 lifeRange;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ s32 maxCount;
    /* 0x2C */ f32 velocityScale;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34Range;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42Range;
    /* 0x44 */ s32 anchorSet;
    /* 0x48 */ u8 flags;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 unk4C;
} AnchorEmitterTemplate; /* size 0x50 */

typedef struct AnchorEmitterParams {
    /* 0x00 */ f32 accum;
    /* 0x04 */ AnchorEmitterTemplate tmpl;
    /* 0x54 */ struct225 *child;
    /* 0x58 */ s8 updateFunc;
    /* 0x59 */ s8 unk59Func;
    /* 0x5A */ u8 unk5A;
    /* 0x5B */ u8 pad5B;
} AnchorEmitterParams; /* size 0x5C */

/* Emitter object created by func_1518F5D0 (game_1BC650.c; an object of func_151D2F00). */
typedef struct AnchorEmitter {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ struct Actor *owner;
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ s16 count;
    /* 0x22 */ u8 pad22[0xE];
    /* 0x30 */ AnchorEmitterParams params;
} AnchorEmitter; /* size 0x8C */

/* func_15147DA0 (game_175250.c): motion and appearance block copied from its second argument (some
 * handlers read these words differently, e.g. game_17CAF0.c's puffs). */
typedef struct TrailMotion {
    /* 0x00 */ f32 halfWidth;
    /* 0x04 */ Vec3f velocity;
    /* 0x10 */ f32 gravity;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 texture;          /* index into D_80091564 */
    /* 0x1A */ u8 shade;            /* passed with alpha to func_151441A4 / func_151442FC */
    /* 0x1B */ u8 alpha;            /* faded by func_150C1978 */
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
} TrailMotion; /* size 0x20 */

/* matching: the first 0x1C bytes of TrailMotion (callers that never set unk1C/unk1E; func_15147DA0 copies
 * past its end); a full TrailMotion on the stack changes the frame. */
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

/* User data of a func_15147DA0 trail (Trail.params). */
typedef struct TrailParams {
    /* 0x00 */ TrailMotion motion;
    /* 0x20 */ u8 state;            /* handler index into D_8008A3E0 */
    /* 0x21 */ u8 unk21;            /* handler index into D_8008A3F8 */
    /* 0x22 */ u8 finishFn;         /* handler index into D_8008A42C */
    /* 0x23 */ u8 hitFn;            /* handler index into D_8008A430 */
    /* 0x24 */ u8 hitKind3Fn;       /* handler index into D_8008A450 */
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ TrailRender render;
} TrailParams; /* size 0x48 */

/* matching: the first 0x18 bytes of EffectTemplate (func_15147DA0's first argument); a full EffectTemplate
 * on the stack changes the frame. */
typedef struct EffectTemplate18 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 kind;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 itemCount;
    /* 0x16 */ u8 pad16[2];
} EffectTemplate18;

/* matching: EffectTemplate plus 4 bytes; the stack frame needs the larger size. */
typedef struct EffectTemplate20 {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 kind;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 itemCount;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void *unk18;
    /* 0x1C */ u8 pad1C[4];
} EffectTemplate20;

/* func_15134908 (game_161520.c): spawn parameters of a blood trail; copied verbatim into the effect. */
typedef struct BloodTrailDesc {
    /* 0x00 */ f32 *x;              /* followed point */
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;          /* D_80089AFC index */
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 callback;
    /* 0x1A */ u8 pad1A[2];
} BloodTrailDesc; /* size 0x1C */

/* matching: the first 0x18 bytes of BloodTrailDesc; a full BloodTrailDesc on the stack changes the frame. */
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

/* matching: BloodTrailDesc plus 4 bytes; the stack frame needs the larger size. */
typedef struct BloodTrailDesc20 {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 callback;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ u8 pad1C[4];
} BloodTrailDesc20;

/* func_15149550 (game_176A00.c): emitter description that it turns into a spawned particle. */
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
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ s32 extraSize;
    /* 0x34 */ u8 effect;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ f32 scale;
    /* 0x3C */ f32 maxHeight;
    /* 0x40 */ f32 shrinkRate;
    /* 0x44 */ f32 growRate;
} ParticleEmitterDesc; /* size 0x48 */

/* func_150B3F5C (game_E1280.c): settings of a smoke trail; the first 0x40 bytes of its TrailState. */
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

/* func_150C88D0 (game_F5D80.c): spawn parameters of an expanding ring effect (copied into RadialEffect.def). */
typedef struct RadialEffectDef {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 flags;            /* bit 0: expires when remaining runs out */
    /* 0x0D */ s8 checkCallback;    /* index into D_80088860, -1 = none */
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
    /* 0x2C */ s8 emitCallback;     /* index into D_80088858, -1 = none */
    /* 0x2D */ s8 updateCallback;   /* index into D_80088864, -1 = none */
    /* 0x2E */ u8 pad2E[2];
} RadialEffectDef; /* size 0x30 */

/* func_151580B0 (game_185560.c): template of an effect actor (object type 0x37 / 0x55), copied to the actor
 * at +0x10. Like EffectParams, but the actor draws one display list from D_8008AFB8. */
typedef struct EffectActorTemplate {
    /* 0x00 */ u8 flags;            /* 1: lifetime counts down, 2: attached to owner, 4: attach option */
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s8 updateCallback;   /* index into D_8008AE00, -1 = none */
    /* 0x03 */ s8 drawCallback;     /* index into D_8008AE0C, -1 = none */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 displayList;      /* index into D_8008AFB8 */
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

/* EffectMotion without the flags: the motion block of the effect actors of func_151580B0 (game_185560.c),
 * always integrated. */
typedef struct TumbleMotion {
    /* 0x00 */ Vec3f rot;          /* degrees */
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 drag;
} TumbleMotion; /* size 0x30 */

/* func_15152520 (game_17CAF0.c): spawns countBase + rand % (countRange + 1) tumbling model effects
 * (func_15157898) at position, each launched at a random yaw/pitch and speed with a random spin. */
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
    /* 0x38 */ s32 unk38;           /* EffectParams.unkC */
    /* 0x3C */ s32 unk3C;           /* func_15157898's 3rd argument (func_1503F5B8's 3rd) */
    /* 0x40 */ f32 unk40;           /* base and range of func_15157898's 4th argument */
    /* 0x44 */ f32 unk44;
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

/* A particle object made by func_1513D2F0 / func_151407D0 (game_169510.c's Particle, 0x110 bytes), as seen
 * by callers that keep their own data after it. Its ParticleDef copy is at 0x18. */
typedef struct DefParticleHead {
    /* 0x000 */ EffectHeader header;    /* header.owner / kind as passed to the spawner */
    /* 0x010 */ s32 active;             /* bit 0: alive */
    /* 0x014 */ s32 unk14;
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0x50];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuffers[4];  /* per viewport, refreshed from vtxTemplate */
} DefParticleHead; /* size 0x110 */

/* Burst effect (func_150F9BB0 in game_127060.c, func_151BD828 in game_1E9A30.c): an effect object that
 * keeps two sparks (func_1513D2F0 beams) and two glows (func_151407D0) on its owner actor. */
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
    /* 0x00 */ EffectHeader header;   /* header.state: age in frames; -1 frees */
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ BurstData data;
} BurstEffect; /* size 0x40 */

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
    /* 0x40 */ f32 beamWidth;         /* BeamScale passed to func_1513FAB4 (times scale) */
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

#endif
