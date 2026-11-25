#ifndef EFFECTS_H
#define EFFECTS_H

/* Particle / effect spawn descriptors shared by the spawn functions and their callers. */

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
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
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

/* Spawn template of a model (debris) particle passed to func_15132A4C (game_15F680.c):
 * the layout of that Particle's bytes 0x10..0x8B. */
typedef struct ParticleSpawn {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
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
    /* 0x1C */ vertex pos;
    /* 0x28 */ vertex rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ vertex scale;
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

/* Debris emitter settings read by func_151A6F00 (game_1D43B0.c). Angles are 0..255 binary angles. */
typedef struct DebrisEmitter {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ vertex pos;
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
    /* 0x30 */ vertex dir;     /* modes 2 and 3 */
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

#endif
