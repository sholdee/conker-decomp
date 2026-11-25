#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15142314(struct255 *bones, s32 bone, vertex *out);

/* Gets the position of the actor's bone 0x18 (side 1) or 0x15 (side 2). */
s32 func_150C19C0(vertex *pos, struct127 *actor, u8 side) {
    s32 bone;

    switch (side) {
    case 1:
        bone = 0x18;
        break;
    case 2:
        bone = 0x15;
        break;
    }

    func_15142314(actor->unk1D4, bone, pos);
    return 1;
}

s32 func_150C1A2C(s32 arg0, s32 arg1) {
    return 7;
}

/* Spawn template for func_15132A4C (layout as ParticleSpawn in game_1D43B0.c). */
typedef struct {
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
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[6];
} ParticleSpawn; // size 0x7C

/* A live func_15132A4C particle; this file's particles keep 4 bytes of extra data at 0x170. */
typedef struct {
    /* 0x000 */ u8 pad0[0x170];
    /* 0x170 */ f32 unk170;
} Particle;

Particle *func_15132A4C(ParticleSpawn *spawn, s32, s32, s32 extraSize, u8, s32);
void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, s32 arg10, u8 arg11);
void func_15165F80(s32 direction, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg6, s32 arg7, s32 arg8);

/*
 * Spawns an effect at the actor's bone for `side` (see func_150C19C0): a ring of particles and a
 * flat quad at the actor's unk180 height, and 5 to 12 debris particles flying outwards from it.
 */
void func_150C1A40(struct127 *actor, s32 side, s32 arg2) {
    vertex pos;
    f32 scale;
    ParticleSpawn spawn;
    f32 radius;
    u8 angle;
    f32 speed;
    f32 cosine;
    f32 sine;
    Particle *particle;
    u32 count;
    u8 startAngle;

    if (actor->unk1D4 == NULL) {
        return;
    }

    scale = (actor->xz_scale + actor->y_scale) * 0.5f;
    func_150C19C0(&pos, actor, side);
    startAngle = (actor->unk76 >> 8) + 0x40;
    radius = scale * 70.0f;
    func_1514C2F0(pos.x, actor->unk180, pos.z, radius, startAngle,
                  (side == 1) ? 16 : -16, 8, 0, 0, radius * 4.04f, 0, 255);
    func_15165F80(-1, (s32)pos.x, (s32)((s32)actor->unk180 + 2.0f), (s32)pos.z, 8, 21, 0, 255, 0);
    spawn.model = 5;
    spawn.flags = 0x29E9;
    spawn.variant = 0;
    spawn.unk5C = NULL;
    spawn.alpha = 255;
    spawn.updateFn = 1;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 1;
    spawn.owner = actor;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.elasticity = 0.4f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = -0.9f;
    spawn.ownerId = actor->unique_id;
    spawn.fadeStart = 16;
    spawn.fadeRate = 15;
    count = (func_150ADA20() & 7) + 5;
    while (count--) {
        // matching: masking the already-u8 random byte keeps IDO's andi/move pair
        angle = func_150ADA20() & 0xFF;
        speed = (0.1f + func_150ADA68() * 0.9f) * 9.0f * scale;
        cosine = func_151423D8(angle - 0x40);
        sine = func_151423D8(angle);
        spawn.lifetime = (func_150ADA20() & 15) + 30;
        spawn.pos.x = radius * cosine + pos.x;
        spawn.pos.y = actor->unk180 + 2.0f;
        spawn.pos.z = radius * sine + pos.z;
        spawn.vel.x = speed * cosine;
        spawn.vel.y = 2.0f * speed;
        spawn.vel.z = speed * sine;
        spawn.sizeY = spawn.sizeXZ = spawn.radius = (0.1f + func_150ADA68() * 0.9f) * scale * 1.5f;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
        spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
        particle = func_15132A4C(&spawn, 3, 255, sizeof(f32), 255, 0);
        if (particle != NULL) {
            particle->unk170 = actor->unk180;
        }
    }
}

/* Effect description passed to func_1518CA80 (same layout as ParticleEffect in game_1B9F30.c). */
typedef struct {
    /* 0x00 */ f32 x;            /* position as floats (mode 1) */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
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
    /* 0x31 */ u8 onExpire;      /* low nibble: index into D_8008D5D0, run when life reaches 0 */
} ParticleEffect;

void func_151429E0(u8 palette, u8 *r, u8 *g, u8 *b);
void func_1518CA80(ParticleEffect *effect, u8 mode);

/*
 * Spawns a growing billboard particle of size `size` at (x, y, z), moving horizontally away from
 * (fromX, fromZ), with random colours from palette 0.
 */
s32 func_150C1E34(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, f32 fromX, f32 fromY, f32 fromZ,
                  s32 arg8, s32 arg9, s32 argA, s32 argB, f32 size) {
    ParticleEffect effect;

    effect.x = x;
    effect.y = y;
    effect.z = z;
    effect.velX = (x - fromX) * 10.0f;
    effect.velZ = (z - fromZ) * 10.0f;
    effect.width = effect.height = size;
    effect.heightStep = effect.widthStep = size * 0.1757;
    effect.velY = 150;
    effect.accelY = 20;
    effect.life = 600;
    func_151429E0(0, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
    func_151429E0(0, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
    effect.intensity = 255;
    effect.alpha = 255;
    effect.fadeStep = (func_150ADA20() & 15) + 12;
    effect.onExpire = 1;
    func_1518CA80(&effect, 1);
    return 1;
}
