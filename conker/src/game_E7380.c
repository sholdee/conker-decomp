#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* A collision triangle as func_150AC9C0 reports it: three s16 xyz vertices. */
typedef struct {
    s16 coords[9];
} SurfaceTriangle;

/* Argument of func_151511FC (layout from game_17CAF0.c): a burst of particles off a surface triangle. */
typedef struct {
    /* 0x00 */ s16 countBase;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ Vec3f normal;
    /* 0x1C */ SurfaceTriangle triangle;
    /* 0x30 */ f32 spread;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ f32 scaleBase;
    /* 0x40 */ f32 scaleRange;
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 speedBase;
    /* 0x58 */ f32 speedRange;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 unk5D;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 spin;
} SurfaceBurstConfig;

/* Spawn template of a sprite (layout from game_169510.c), copied to SpriteFx.def. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 frame;          /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 unk10;           /* faded out with the lifetime */
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 renderModes;    /* low byte: D_800A4AC8 index, high half: combiner index */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;      /* lifetime below which unk10 / rgba[3] fade */
    /* 0x24 */ s16 fadeRate;
} SpriteDef; /* size 0x28 */

/* A live sprite of game_169510.c; the update callbacks of D_80089C70 get one. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ SpriteDef def;
} SpriteFx;

/* Per-behaviour motion block stored right after a particle. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 rotVelX;
    /* 0x10 */ f32 rotVelY;
    /* 0x14 */ f32 minY;           /* the particle expires below this height */
} ParticleMotion;

/* A live particle of game_169510.c; the update callbacks of D_80089CB8 / D_80089D60 get one. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0xA0];
    /* 0x110 */ ParticleMotion motion;
} FxParticle;

extern u8 D_80084060[]; /* player -> controller channel map (game_48FD0.c) */
/* Code range checksummed by func_150B9ED0 (anti-tamper: a mismatch breaks player 1's controller mapping). */
extern s32 func_151E7EF8[];
extern s32 func_151E7F60[];
extern s32 func_150A3058(s32, s16, s16, s16);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, SurfaceTriangle *, f32 *, f32 *, f32 *, void *, s32 *, void *, f32);
extern s32 func_15145C90(s32);
extern void func_151511FC(SurfaceBurstConfig *, u8, s32);

/*
 * Called when the actor has moved from (prevX, prevZ): casts a ray from 80 units above it back towards
 * the previous position and, on a hit, spawns a burst of particles off the hit triangle (plus a
 * sprite when func_15145C90 accepts the hit object). First runs an anti-tamper checksum over the code
 * at func_151E7EF8. Nothing is spawned while func_150A3058(2 or 1, position) reports anything.
 */
void func_150B9ED0(Actor *actor, f32 prevX, f32 prevZ, u8 arg3, s32 arg4) {
    s32 pad[3]; // matching: unused locals of the original; the frame is 16 bytes larger than the named ones need
    u32 addr;
    u32 end;
    s32 sum;

    if (actor == NULL) {
        return;
    }
    sum = 0;
    end = (u32)func_151E7F60;
    for (addr = (u32)func_151E7EF8; addr < end; addr += sizeof(s32)) {
        sum += ~*(s32 *)addr;
    }
    if (sum != 0x3843095E) {
        D_80084060[0] = 0xFF;
    }
    if ((func_150A3058(2, (s16)actor->x_position, (s16)actor->y_position, (s16)actor->z_position) == 0) &&
        (func_150A3058(1, (s16)actor->x_position, (s16)actor->y_position, (s16)actor->z_position) == 0)) {
        f32 rayY = actor->y_position + 80.0f;
        f32 dz;
        f32 dy;
        f32 dx;
        Vec3f hit;
        SurfaceTriangle triangle;
        s32 hitObject;
        SpriteDef sprite;
        SurfaceBurstConfig burst;

        /* Ray back along the move, i.e. minus the displacement since (prevX, prevZ). */
        dx = -(actor->x_position - prevX);
        dy = 0.0f;
        dz = -(actor->z_position - prevZ);
        if (func_150AC9C0(actor->x_position, rayY, actor->z_position, dx, dy, dz, NULL, &triangle, &hit.x,
                          &hit.y, &hit.z, NULL, &hitObject, NULL, 0.0f) == 0) {
            return;
        }
        if (func_15145C90(hitObject) != 0) {
            sprite.texture = 6;
            sprite.unk7 = 0;
            sprite.flags = 0x9F01;
            sprite.lifetime = (func_150ADA20() % 51U) + 100;
            sprite.frame = 0;
            sprite.frameSpeed = 0;
            sprite.unk10 = (func_150ADA20() % 101U) + 155;
            sprite.unk11 = 0xFF;
            sprite.rgba[0] = 0xFF;
            sprite.rgba[1] = 0xFF;
            sprite.rgba[2] = 0xFF;
            sprite.rgba[3] = 0xFF;
            sprite.renderModes = 0x3B0002;
            sprite.unk16 = 0;
            sprite.unk17 = 7;
            sprite.fadeStart = 25;
            sprite.fadeRate = 10;
            func_1513C650(&sprite, 0, 0, (s32)&triangle, hit.x, hit.y, hit.z,
                          (func_150ADA68() * 20.0f) + 40.0f, (func_150ADA68() * 20.0f) + 40.0f, func_150ADA20() & 0xFF,
                          ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1), 3, 0xFF, 0, arg3, arg4);
        }
        burst.countBase = 15;
        burst.countRange = 10;
        burst.position = hit;
        burst.normal.x = -dx;
        burst.normal.y = -dy;
        burst.normal.z = -dz;
        burst.triangle = triangle;
        burst.spread = 675.0f;
        burst.scaleBase = 7.0f;
        burst.scaleRange = 10.0f;
        burst.unk4C = -1.022000074f;
        burst.unk50 = 0.5420000553f;
        burst.unk34 = 7;
        burst.unk36 = 0x3B01;
        burst.unk35 = 0;
        burst.unk38 = 26;
        burst.unk3A = 20;
        burst.flags = 0x5C0001;
        burst.unk48 = 100;
        burst.unk49 = 155;
        burst.unk5C = 0;
        burst.unk5D = 7;
        burst.unk60 = 3;
        burst.unk64 = 0xFF;
        burst.unk68 = 10;
        burst.unk6A = 25;
        burst.speedBase = 0.0374f;
        burst.speedRange = 0.0235f;
        burst.unk6C = 0.940404f;
        burst.spin = 1.084000111f;
        func_151511FC(&burst, arg3, arg4);
    }
}

/* Fades the sprite out over its last 64 frames (def.unk10 = 4 * lifetime). */
s32 func_150BA35C(SpriteFx *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 64) {
        sprite->def.unk10 = lifetime * 4;
    }
    return 1;
}

/* Moves the particle by its velocity and spins it, under gravity of -0.6 per frame. */
s32 func_150BA37C(FxParticle *particle) {
    particle->motion.vel.y += -0.6f * D_800BE9A4;
    particle->def.pos.x += particle->motion.vel.x * D_800BE9A4;
    particle->def.pos.y += particle->motion.vel.y * D_800BE9A4;
    particle->def.pos.z += particle->motion.vel.z * D_800BE9A4;
    particle->def.rot.x += particle->motion.rotVelX * D_800BE9A4;
    particle->def.rot.y += particle->motion.rotVelY * D_800BE9A4;
    return 1;
}

/*
 * Fades the particle with whichever is smaller of 16 * lifetime and 4 * its height above
 * motion.minY (capped at 255). Returns 0 (expire) once it falls below minY.
 */
s32 func_150BA424(FxParticle *particle) {
    f32 height = particle->def.pos.y - particle->motion.minY;
    s32 lifeAlpha;
    s32 heightAlpha;

    if (height < 0.0f) {
        return 0;
    }

    lifeAlpha = particle->def.lifetime * 16;
    if (lifeAlpha >= 256) {
        lifeAlpha = 255;
    }

    heightAlpha = (s32)height * 4;
    if (heightAlpha >= 256) {
        heightAlpha = 255;
    }

    if (heightAlpha < lifeAlpha) {
        particle->def.unk44 = heightAlpha;
    } else {
        particle->def.unk44 = lifeAlpha;
    }

    // matching: never true for a u8, but the original tests it (IDO reloads the stored byte)
    if (particle->def.unk44 < 0) {
        return 0;
    }
    return 1;
}
