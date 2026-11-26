#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct PipeIds {
    s32 id[3];
} PipeIds;

/* The three exploding pipes: where each is and which way it faces (game/View/ViewOrtho/viewortho.c reads it too). */
const PipeSpot gPipeSpots[] = {
    { { 975.0f, 351.0f, 1723.0f }, 0x94 },
    { { 1005.0f, 351.0f, -1706.0f }, 0xEA },
    { { -1980.0f, 351.0f, -17.0f }, 0x3F },
};

/* Explodes pipe `pipe`: fires the four effect spawners at its spot and plays two sounds there. */
void pipeexplode_explode(u8 pipe, u8 arg1) {
    DebrisEmitter argsA;
    s16 i;
    s32 pad; // matching: stack space (an unused local)
    StreakBurst argsB;
    SimplePuffSpray argsC;
    f32 yawSin; // matching: also holds the debris size; a separate local mismatches
    f32 yawCos;
    Vec3f origin;
    TrailStateParams argsD;

    camera_startEnvelope(&camera_global_Cameras[D_800BE9E8], 7, 1);
    argsA.count = (math_random() & 1) + 3;
    argsA.pos = gPipeSpots[pipe].pos;
    argsA.radius = 170.0f;
    argsA.speedBase = 16.20000076f;
    argsA.speedRange = 17.60000038f;
    argsA.velYBase = 19.30000114f;
    argsA.velYRange = 21.30000114f;
    argsA.lifetime = 300;
    argsA.yawBase = gPipeSpots[pipe].yaw - 0x20;
    argsA.yawRange = 65;
    argsA.pitchBase = -40;
    argsA.pitchRange = 80;
    argsA.landPuffs = 1;
    argsA.landPuffFlag = 0;
    argsA.landPuffLifeBase = 50;
    argsA.landPuffLifeRange = 25;
    argsA.dir.x = 0.0f;
    argsA.dir.y = 0.0f;
    argsA.dir.z = 0.0f;
    argsA.sizeBase = 0.4670000076f;
    argsA.sizeRange = 0.5139999986f;
    argsA.gravityBase = -1.0f;
    argsA.gravityRange = -2.600000143f;
    argsA.spinBase = -5.0f;
    argsA.spinRange = 10.0f;
    blood_spawnDebris(&argsA, 0, arg1, 1);
    i = (math_random() & 1) + 1;
    while (i--) {
        yawSin = (math_randomFloat() * 73.0f) + 31.0f;
        particle_emitOnRandomArc(gPipeSpots[pipe].pos.x, gPipeSpots[pipe].pos.y, gPipeSpots[pipe].pos.z, yawSin, gPipeSpots[pipe].yaw + 0x40, 0, 0xFF, (math_random() % 3U) + 4, 9, 0, 0.009615384974f, 0, arg1);
    }
    argsB.countBase = 30;
    argsB.countRange = 15;
    argsB.pos = gPipeSpots[pipe].pos;
    argsB.yawBase = gPipeSpots[pipe].yaw - 0x40;
    argsB.yawRange = 128;
    argsB.pitchBase = -45;
    argsB.pitchRange = 90;
    argsB.headColor.red = 0xFF;
    argsB.headColor.green = 0xFF;
    argsB.headColor.blue = 0xFF;
    argsB.headColor.alpha = 0xFF;
    argsB.tailColor.red = 0xFF;
    argsB.tailColor.green = 0xFF;
    argsB.tailColor.blue = 0xFF;
    argsB.tailColor.alpha = 0xFF;
    argsB.flags = 9;
    argsB.lifetimeBase = 16;
    argsB.lifetimeRange = 22;
    argsB.renderMode = 0x1601;
    argsB.alphaBase = 0x7F;
    argsB.alphaRange = 0x7F;
    argsB.fadeStart = 10;
    argsB.fadeRate = 25;
    argsB.kind = 0;
    argsB.headSpeedBase = 6.0f;
    argsB.headSpeedRange = 2.549999952f;
    argsB.tailSpeedBase = 2.700000048f;
    argsB.tailSpeedRange = 3.419999838f;
    argsB.widthBase = 4.139999866f;
    argsB.widthRange = 4.859999657f;
    particle_spawnStreakBurst(&argsB, arg1);
    {
    // matching: a declaration here opens a -g3 lexical block, which steers as1's scheduling
    extern void particle_spawnSimplePuffSpray(SimplePuffSpray *, u8);

    argsC.pos = gPipeSpots[pipe].pos;
    argsC.countBase = 15;
    argsC.countRange = 8;
    argsC.angles.yawBase = gPipeSpots[pipe].yaw - 25;
    argsC.angles.yawRange = 50;
    argsC.angles.pitchBase = -50;
    argsC.angles.pitchRange = 60;
    argsC.itemCountBase = 3;
    argsC.itemCountRange = 2;
    argsC.lifeBase = 60;
    argsC.lifeRange = 20;
    argsC.green = 6;
    argsC.red = 8;
    argsC.handlers[0] = 1;
    argsC.handlers[1] = 7;
    argsC.handlers[2] = 0;
    argsC.handlers[3] = 0;
    argsC.handlers[4] = 0;
    argsC.handlers[5] = 0;
    argsC.blue = 0xFF;
    argsC.alpha = 0xA0;
    argsC.speedBase = 20.0f;
    argsC.speedRange = 40.0f;
    argsC.sizeBase = 10.0f;
    argsC.sizeRange = 30.0f;
    argsC.growthBase = 1.5f;
    argsC.growthRange = 1.0f;
    particle_spawnSimplePuffSpray(&argsC, arg1);
    }
    argsC.countBase = 10;
    argsC.countRange = 6;
    argsC.lifeBase = 100;
    argsC.lifeRange = 0;
    argsC.red = 9;
    argsC.handlers[3] = 5;
    particle_spawnSimplePuffSpray(&argsC, arg1);
    yawSin = math_cosBinAngleMasked(gPipeSpots[pipe].yaw);
    yawCos = math_cosBinAngleMasked(gPipeSpots[pipe].yaw - 0x40);
    origin.x = gPipeSpots[pipe].pos.x - 150.0f * yawCos;
    origin.y = gPipeSpots[pipe].pos.y;
    origin.z = gPipeSpots[pipe].pos.z - 150.0f * yawSin;
    argsD.shade = 0x80;
    argsD.baseVel.x = 10.0f * yawCos;
    argsD.baseVel.y = 3.0f;
    argsD.baseVel.z = 10.0f * yawSin;
    argsD.gravity = -3.710000038f;
    argsD.alpha = 0xFF;
    argsD.texStep = 2;
    argsD.texStepJitter = 5;
    argsD.phaseJitter = 4;
    argsD.burstPreset = 0;
    argsD.life = 30;
    argsD.flags = 6;
    argsD.fadeTailCount = 5;
    argsD.fadeTailStep = 0x33;
    argsD.fadeHeadCount = 3;
    argsD.fadeHeadStep = 0x55;
    argsD.emitRate = 0.400000006f;
    argsD.width = 47.0f;
    argsD.widthGrowth = 1.840000033f;
    argsD.speedBase = 3.299999952f;
    argsD.speedSwing = 0.6999999881f;
    argsD.burstRate = 0.3000000119f;
    {
        u8 idBuf[4];
        const PipeIds idsA = { { 0, 2, 4 } };
        const PipeIds idsB = { { 1, 3, 5 } };

        idBuf[0] = idsA.id[pipe];
        effect_sendEventToTimedEffects((s32)idBuf, 0x17);
        idBuf[0] = idsB.id[pipe];
        effect_sendEventToTimedEffects((s32)idBuf, 0x17);
    }
    trail_spawnScatterTrail(&argsD, &origin, 20);
    sound_playAt(0xE7, 0x7FFF, 0, 2, 0, gPipeSpots[pipe].pos.x, gPipeSpots[pipe].pos.y, gPipeSpots[pipe].pos.z, 1000, 3000);
    sound_playAt(0x22, 0x7FFF, 0, 0, 0, gPipeSpots[pipe].pos.x, gPipeSpots[pipe].pos.y, gPipeSpots[pipe].pos.z, 1000, 3000);
}

/* Spawns one pipe chunk at (x, y, z), flung out along yaw `arg8` with a random speed. */
s32 pipeexplode_spawnChunk(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 yawCos;
    f32 yawSin;
    s16 angle;
    f32 headingCos;
    f32 headingSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;
    ParticleSpawn spawn;

    yawCos = math_cosBinAngleMasked(arg8 - 0x40);
    yawSin = math_cosBinAngleMasked(arg8);

    spawn.flags = 0x9E8;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.elasticity = 0.200000003f;

    speed = (math_randomFloat() * 31.0f) + 18.0f;
    angle = (arg9 - 0x40) + (s16)(60.0f * yawSin * argA * argC);
    headingCos = math_cosBinAngleMasked(angle - 0x40);
    headingSin = math_cosBinAngleMasked(angle);
    angle = (s16)(48.0f * yawCos) - 20;
    pitchCos = math_cosBinAngleMasked(angle - 0x40);
    pitchSin = math_cosBinAngleMasked(angle);

    spawn.vel.x = (speed * pitchSin) * headingCos;
    spawn.vel.y = -speed * pitchCos;
    spawn.vel.z = (speed * pitchSin) * headingSin;

    spawn.rotVel.x = (math_randomFloat() * 80.0f) + -40.0f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = (math_randomFloat() * 80.0f) + -40.0f;
    spawn.lifetime = 100;

    spawn.gravity = (math_randomFloat() * -1.752000093f) + -0.8370000124f;
    spawn.radius = 1.0f;

    pitchSin = (math_randomFloat() * 72.5f) + 29.80000114f; // matching: reused as the size
    spawn.model = 0x1C;
    spawn.variant = 0;
    spawn.sizeXZ = pitchSin;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 1;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 2;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.sizeY = pitchSin;

    particle_newModelParticle(&spawn, 3, 0xFF, 0, argE, 0);
    return 1;
}

void *memcpy(void *, const void *, u32);

/*
 * Tunable variant of pipeexplode_spawnChunk: the ranges come from debug sliders (math_debugTunable), and the chunk
 * bounces until it lands slowly.
 */
s32 pipeexplode_spawnTunableChunk(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 yawCos;
    f32 yawSin;
    s16 angle;
    f32 headingCos;
    f32 headingSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;
    s16 life;
    f32 minGrav;
    f32 gravRange;
    f32 minSpeed;
    f32 speedRange;
    f32 minScale;
    f32 scaleRange;
    f32 xzAngle;
    f32 yzAngle;
    ParticleSpawn spawn;
    PipeChunk *chunk;
    f32 minBounceVelY;

    yawCos = math_cosBinAngleMasked(arg8 - 0x40);
    yawSin = math_cosBinAngleMasked(arg8);

    life = math_debugTunable(6, "life", 1, 500, 300, "../Effects/PipeExplode/pipeexplode.c", 0x742);
    minGrav = math_debugTunable(7, "min grav", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x750) * 0.001000000047f;
    gravRange = math_debugTunable(8, "grav range", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x752) * 0.001000000047f;
    minSpeed = math_debugTunable(9, "min speed", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75A);
    speedRange = math_debugTunable(10, "speeed range", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75C);
    minScale = math_debugTunable(11, "min scale", 0, 5000, 1000, "../Effects/PipeExplode/pipeexplode.c", 0x761) * 0.1000000015f;
    scaleRange = math_debugTunable(12, "scale  range", 0, 5000, 0, "../Effects/PipeExplode/pipeexplode.c", 0x763) * 0.1000000015f;
    xzAngle = math_debugTunable(13, "xz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x769);
    yzAngle = math_debugTunable(14, "yz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x76F);

    spawn.flags = 0x29E9;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    minBounceVelY = -18.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.elasticity = 0.200000003f;

    speed = (math_randomFloat() * speedRange) + minSpeed;
    angle = (arg9 - 0x40) + (s16)(xzAngle * yawSin * argA * argC);
    headingCos = math_cosBinAngleMasked(angle - 0x40);
    headingSin = math_cosBinAngleMasked(angle);
    angle = (s16)(yzAngle * yawCos) - 20;
    pitchCos = math_cosBinAngleMasked(angle - 0x40);
    pitchSin = math_cosBinAngleMasked(angle);

    spawn.vel.x = (speed * pitchSin) * headingCos;
    spawn.vel.y = -speed * pitchCos;
    spawn.vel.z = (speed * pitchSin) * headingSin;

    spawn.rotVel.x = (math_randomFloat() * 4.0f) + -2.0f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = (math_randomFloat() * 4.0f) + -2.0f;
    spawn.lifetime = life + 32;

    spawn.gravity = (math_randomFloat() * gravRange) + minGrav;

    pitchSin = (math_randomFloat() * scaleRange) + minScale; // matching: reused as the size
    spawn.sizeXZ = pitchSin;
    spawn.model = 0x1C;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 1;
    spawn.radius = pitchSin * 0.1000000015f;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 5;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 2;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 32;
    spawn.fadeRate = 7;
    spawn.sizeY = pitchSin;

    chunk = particle_newModelParticle(&spawn, 3, 0xFF, 4, argE, 0);
    if (chunk != NULL) {
        memcpy(&chunk->minBounceVelY, &minBounceVelY, sizeof(f32));
    }
    return 1;
}

/* Floor-hit callback for a pipe chunk: bounce it, or stop it once it lands slowly enough. */
s32 pipeexplode_onChunkFloorHit(PipeChunk *chunk, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    chunk->head.pos.y = chunk->head.radius + arg4;
    if (chunk->minBounceVelY < chunk->head.vel.y) {
        chunk->head.flags &= ~0x7;
        chunk->head.flags &= ~0x8;
        chunk->head.flags &= ~0x40;
        chunk->head.flags &= ~0x20;
        chunk->head.vel.x = 0.0f;
        chunk->head.vel.y = 0.0f;
        chunk->head.vel.z = 0.0f;
        if (chunk->head.lifetime > 32) {
            chunk->head.lifetime = 32;
        }
    } else {
        chunk->head.vel.x *= chunk->head.elasticity;
        chunk->head.vel.y = -chunk->head.vel.y * chunk->head.elasticity;
        chunk->head.vel.z *= chunk->head.elasticity;
        chunk->head.rotVel.x *= chunk->head.elasticity;
        chunk->head.rotVel.y *= chunk->head.elasticity;
        chunk->head.rotVel.z *= chunk->head.elasticity;
    }
    return 1;
}

/* Caps the effect's alpha at unk1C * 16 (saturating at 0xFF). */
s32 pipeexplode_fadeByLife(PipeExplodeActor *actor) {
    PipeExplodeState *state;
    s32 alpha;

    state = actor->state;
    alpha = actor->head.tmpl.life << 4;
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    if (alpha < state->alpha) {
        state->alpha = alpha;
    }
    return 1;
}

/* Particle request for particle_spawnSurfaceQuadSpriteNoModel. */
typedef struct PipeExplodeParticle {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s8 kind;
    /* 0x07 */ u8 pad07;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0xC];
    /* 0x28 */ PipeExplodePoint *points;
} PipeExplodeParticle;

typedef struct PipeKinds {
    s16 kind[3];
} PipeKinds;

/* The particle kinds pipeexplode_spawnLandParticle picks from. */
static PipeKinds D_80088700 = { { 15, 16, 17 } };

/* Spawns a random particle and a trail at the actor's current point, at the given height. */
s32 pipeexplode_spawnLandParticle(PipeExplodeActor *actor, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
    PipeExplodeState *state;
    PipeExplodeParticle particle;
    f32 size;
    PipeKinds kinds;
    EffectTemplate pos;
    TrailMotion motion;
    TrailRender style;

    {
        s32 rand2;
        s32 rand1;
        s32 rand0;
        state = actor->state;
        particle.points = actor->points; // matching: kept in the request, a separate local mismatches
        kinds = D_80088700;
        state->unk20 = 4;
        size = math_randomFloat() * 50.0f + 50.0f;
        particle.kind = *(kinds.kind + math_random() % 3U); // matching: kinds.kind[i] grows the frame by 8
        particle.unk08 = 0;
        particle.unk0C = 0;
        particle.unk00 = 0x11;
        particle.unk04 = 150;
        particle.unk10 = state->alpha;
        particle.unk11 = state->shade;
        particle.unk12 = 0;
        particle.unk13 = 0;
        particle.unk14 = 0;
        particle.unk15 = 0xFF;
        particle.unk18 = 0x30002;

        rand0 = math_random();
        rand1 = math_random();
        rand2 = math_random();

        particle_spawnSurfaceQuadSpriteNoModel(&particle, 1, 0, surface,
                      particle.points[actor->head.start].x,
                      height,
                      particle.points[actor->head.start].z,
                      size, size,
                      rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                      actor->head.header.owner, actor->head.header.kind);

    }
    style.unk0 = 0;
    style.setGeometryMode = 1;
    style.clearGeometryMode = 0x160600;
    style.renderMode = 3;
    style.unk10 = 16;
    style.unk14 = 128;
    style.unk18 = 32;
    style.primMode = 0;
    style.envMode = 9;
    pos.kind = 1;
    pos.flags = 1;
    motion.texture = state->texture;
    motion.shade = state->shade;
    motion.flags = 8;
    motion.alpha = state->alpha;
    pos.pos.x = particle.points[actor->head.start].x;
    pos.pos.y = height;
    pos.pos.z = particle.points[actor->head.start].z;
    pos.itemCount = actor->head.tmpl.itemCount;
    pos.life = 60;
    motion.halfWidth = state->unk00 * 0.75f;
    motion.gravity = state->gravity;
    motion.velocity.x = state->velocityX;
    motion.velocity.y = math_randomFloat() * 15.0f + 15.0f;
    motion.velocity.z = state->velocityZ;
    trail_createMotionTrail(&pos, &motion, 0, 1, 7, 0, 0, 0, 0, 0, 0, &style, 0, actor->head.header.owner, actor->head.header.kind);
    return 1;
}
