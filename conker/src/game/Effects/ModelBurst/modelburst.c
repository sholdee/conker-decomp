#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/attach.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/trail.h"

/* One piece of the effect_throwBurstPieces burst. */
struct ModelBurstPiece {
    /* 0x00 */ u16 modelId;
    /* 0x04 */ f32 offset[3];
    /* 0x10 */ f32 rotation[3];
};

/* Start offsets of the six burst models of effect_spawnModelBurst. */
const f32 gModelBurstOffsets[6][3] = {
    { 37.2f, -15.0f, -46.8f },
    { -19.6f, 49.8f, -47.1f },
    { 13.8f, 59.7f, 30.0f },
    { 27.8f, -30.3f, 44.0f },
    { -37.4f, -47.6f, -32.9f },
    { -48.2f, -16.0f, 30.4f },
};

/* The six pieces effect_throwBurstPieces throws out of the actor (model, start offset, rotation). */
const ModelBurstPiece gBurstPieces[6] = {
    { 338, { -6.0f, 84.0f, 89.0f }, { -40.0f, 0.0f, 0.0f } },
    { 338, { 89.0f, 75.0f, -57.0f }, { -40.0f, 120.0f, 0.0f } },
    { 338, { -79.0f, 59.0f, -57.0f }, { -40.0f, 240.0f, 0.0f } },
    { 338, { 92.0f, -60.0f, -59.0f }, { 30.0f, 60.0f, 0.0f } },
    { 338, { 19.0f, -59.0f, -89.0f }, { 37.0f, 180.0f, 0.0f } },
    { 338, { -92.0f, -66.0f, 55.0f }, { 36.0f, 300.0f, 0.0f } },
};

/* Defined in game/Particle/StyleParticle/styleparticle.c with a u8 style. This file was compiled with a view that takes an s32 style (no
   narrowing at the call). */
void particle_spawnQuadEdgeStyleParticles(f32 (*verts)[3], f32 x, f32 z, s32 style, f32 scale, s32 type, s32 countA, s32 countB, u8 arg8,
                   s32 arg9);

/* Spawns six models bursting up and out of the actor, plus a ring of dust particles around its feet. */
void effect_spawnModelBurst(WorldObject *actor, s32 modelId, s32 kind, s32 mode, u8 owner, s32 list) {
    s32 i;
    ModelFxData fx;
    f32 x;
    f32 y;
    f32 z;
    s32 id;
    void *displayList;
    s32 r;

    x = actor->x;
    y = actor->y;
    z = actor->z;
    fx.rotation[0] = 0.0f;
    fx.rotation[1] = 0.0f;
    fx.rotation[2] = 0.0f;
    fx.scale[0] = 1.0f;
    fx.scale[1] = 1.0f;
    fx.scale[2] = 1.0f;
    fx.life = 60;
    fx.friction = 0.3f;
    fx.bounce = -0.7f;
    fx.callback = 2;
    fx.spinDamp = -0.2f;
    fx.primA = 255;
    fx.tileUlt = 0;
    fx.phase = 0;
    fx.flags = 0;
    fx.floorOffset = 60;
    fx.kind = kind;
    fx.renderMode = 0;
    fx.combineMode = 0;
    for (i = 0; i < 6; i++) {
        if (mode & 2) {
            const u8 ids[6] = { 7, 2, 3, 8, 9, 10 };
            id = ids[i];
        } else if (mode & 1) {
            id = modelId + i;
        } else {
            id = modelId;
        }
        displayList = attach_loadModelDl(id);
        fx.position[0] = gModelBurstOffsets[i][0] + x;
        fx.position[1] = gModelBurstOffsets[i][1] + y;
        fx.position[2] = gModelBurstOffsets[i][2] + z;
        fx.modelId = id;
        fx.velocity[0] = gModelBurstOffsets[i][0] * 0.08f;
        fx.velocity[1] = fabsf(gModelBurstOffsets[i][1]) * 0.08f + 7.0f;
        fx.velocity[2] = gModelBurstOffsets[i][2] * 0.08f;
        r = math_random();
        fx.velocity[0] += (r >> 16) * (1.0f / 16384);
        r = math_random();
        fx.velocity[1] += (r >> 16) * (1.0f / 16384);
        r = math_random();
        fx.velocity[2] += (r >> 16) * (1.0f / 16384);
        fx.spin[0] = ((s32)math_random() >> 16) * (1.25f / 16384);
        fx.spin[1] = ((s32)math_random() >> 16) * (1.25f / 16384);
        fx.spin[2] = ((s32)math_random() >> 16) * (1.25f / 16384);
        fx.displayList = displayList;
        effect_spawnModelFx(&fx, owner, list);
    }
    effect_spawnDustRing(x, y - 80.0f, z, 70.0f, 500, 300.0f / 65536, 0, 10, owner, list);
    actor->inactive = 1;
}

/* Plays the map-specific burst (LEVEL_W1_BRONTO_CAVERN and LEVEL_W1_BARN_ARENA) at the actor. */
void effect_spawnLevelModelBurst(WorldObject *actor, u8 owner, s32 list) {
    switch (level) {
    case LEVEL_W1_BRONTO_CAVERN:
        effect_spawnModelBurst(actor, 1, 1, 1, owner, list);
        break;
    case LEVEL_W1_BARN_ARENA:
        effect_spawnModelBurst(actor, 0xA9, 8, 0, owner, list);
        break;
    }
}

/* Spawns the six gBurstPieces pieces flying out of the actor. */
void effect_throwBurstPieces(WorldObject *actor, u8 owner, s32 list) {
    s32 i;
    ModelFxData fx;
    const ModelBurstPiece *piece;
    f32 x;
    f32 y;
    f32 z;
    s32 count;
    void *displayList;

    count = 6; // matching: IDO emits the target's loop only with the bound in a variable
    piece = gBurstPieces;
    fx.renderMode = 2;
    fx.combineMode = 1;
    fx.kind = 6;
    fx.floorOffset = 0;
    x = actor->x;
    y = actor->y;
    z = actor->z;
    fx.rotation[0] = 0.0f;
    fx.rotation[1] = 0.0f;
    fx.rotation[2] = 0.0f;
    fx.scale[0] = 2.0f;
    fx.scale[1] = 2.0f;
    fx.scale[2] = 2.0f;
    fx.life = 60;
    fx.callback = 2;
    fx.primA = 255;
    fx.tileUlt = 0;
    fx.phase = 0;
    fx.flags = 0;
    fx.friction = 0.3f;
    fx.bounce = -0.7f;
    fx.spinDamp = -0.2f;
    for (i = 0; i < count; i++) {
        displayList = attach_loadModelDl(piece->modelId);
        fx.modelId = piece->modelId;
        fx.position[0] = piece->offset[0] + x;
        fx.position[1] = piece->offset[1] + y;
        fx.position[2] = piece->offset[2] + z;
        fx.velocity[0] = piece->offset[0] * 0.09f;
        fx.velocity[1] = fabsf(piece->offset[1]) * 0.23f;
        fx.velocity[2] = piece->offset[2] * 0.09f;
        fx.rotation[0] = piece->rotation[0];
        fx.rotation[1] = piece->rotation[1];
        fx.rotation[2] = piece->rotation[2];
        fx.spin[0] = fx.rotation[0] * 0.1f;
        fx.spin[1] = math_randomFloat() * 4.0f - 2.0f;
        fx.spin[2] = math_randomFloat() * 10.0f - 5.0f;
        fx.displayList = displayList;
        effect_spawnModelFx(&fx, owner, list);
        piece++;
    }
    actor->inactive = 1;
}

/* Effect hook for objects: object 0x33 gets func_150C3D5C first. */
void func_151711C4(Actor *obj) {
    if (obj->character_type == 0x33) {
        func_150C3D5C(obj);
    }
    actor_remove(obj, 1);
}

/*
 * Spawns `count` models scattered within +-radius/2 of (x, y, z), rising at 0.8-1.2 x `speed`
 * with a random drift and spin and a random scale between minScale and maxScale; they bounce
 * off the floor with `bounce` (friction is -0.75 x bounce).
 */
void effect_scatterModels(s32 count, s32 modelId, f32 x, f32 y, f32 z, f32 speed, f32 minScale, f32 maxScale,
                   f32 radius, s32 kind, s32 life, f32 bounce, u8 owner, s32 list) {
    s32 i;
    ModelFxData fx;
    f32 scaleRange;
    void *displayList;
    f32 scaleZ;
    f32 driftX;
    f32 lift;
    f32 driftZ;
    f32 liftRange;

    scaleRange = maxScale - minScale;
    fx.life = life;
    fx.rotation[0] = 0.0f;
    fx.rotation[1] = 0.0f;
    fx.rotation[2] = 0.0f;
    fx.friction = bounce * -0.75f;
    fx.bounce = bounce;
    fx.callback = 2;
    fx.primA = 255;
    fx.tileUlt = 0;
    fx.phase = 0;
    fx.flags = 0;
    fx.modelId = modelId;
    fx.kind = kind;
    fx.renderMode = 1;
    fx.combineMode = 1;
    radius *= 1.0f / 65536;
    fx.spinDamp = -0.2f;
    liftRange = speed * 6.103515716e-06f;
    speed *= 0.8f;
    for (i = 0; i < count; i++) {
        fx.position[0] = ((s32)math_random() >> 16) * radius + x;
        fx.position[1] = ((s32)math_random() >> 16) * radius + y;
        fx.position[2] = ((s32)math_random() >> 16) * radius + z;
        displayList = attach_loadModelDl(modelId);
        fx.displayList = displayList;
        driftX = ((s32)math_random() >> 16) * (15.0f / 65536);
        fx.velocity[0] = driftX;
        lift = (math_random() & 0xFFFF) * liftRange + speed;
        fx.velocity[1] = lift;
        driftZ = ((s32)math_random() >> 16) * (15.0f / 65536);
        fx.velocity[2] = driftZ;
        fx.spin[0] = ((s32)math_random() >> 16) * (10.0f / 65536);
        fx.spin[1] = ((s32)math_random() >> 16) * (10.0f / 65536);
        fx.spin[2] = ((s32)math_random() >> 16) * (10.0f / 65536);
        fx.scale[0] = (math_random() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        fx.scale[1] = (math_random() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        scaleZ = (math_random() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        fx.floorOffset = (fx.scale[1] + fx.scale[0] + scaleZ) * 6.666667f + 20.0f;
        fx.scale[2] = scaleZ;
        effect_spawnModelFx(&fx, owner, list);
    }
}

/* Spawns the scene-specific square ring of particles under the actor, chosen by its unk72. */
void func_15171600(WorldObject *actor, u8 owner, s32 list) {
    switch (actor->unique_id) {
    case 0xEB:
        effect_spawnRectangleParticles(actor, 400.0f, 500.0f, 0.0f, -1270.0f, 0.0f, 2, 5, 20, 20, 1.0f, owner, list);
        break;
    case 0xF6:
        effect_spawnRectangleParticles(actor, 200.0f, 400.0f, -1800.0f, -1000.0f, -1200.0f, 2, 5, 4, 10, 1.0f, owner, list);
        break;
    case 0xF8:
        effect_spawnRectangleParticles(actor, 100.0f, 160.0f, -2500.0f, -650.0f, 0.0f, 2, 5, 4, 4, 0.5f, owner, list);
        break;
    case 0xF9:
        effect_spawnRectangleParticles(actor, 200.0f, 100.0f, 100.0f, -300.0f, 0.0f, 1, 5, 4, 4, 1.0f, owner, list);
        break;
    }
}

/*
 * Passes particle_spawnQuadEdgeStyleParticles the corners of a (2 * halfX) x (2 * halfZ) rectangle centred at the actor's
 * position plus (offsetX, offsetY, offsetZ).
 */
void effect_spawnRectangleParticles(WorldObject *actor, f32 halfX, f32 halfZ, f32 offsetX, f32 offsetY, f32 offsetZ, s32 type,
                   s32 style, s32 countA, s32 countB, f32 scale, u8 owner, s32 list) {
    f32 corners[4][3];
    f32 centerX;
    f32 centerY;
    f32 centerZ;

    centerX = actor->x + offsetX;
    centerY = actor->y + offsetY;
    centerZ = actor->z + offsetZ;
    corners[0][0] = halfX + centerX;
    corners[0][1] = centerY;
    corners[0][2] = halfZ + centerZ;
    corners[1][0] = centerX - halfX;
    corners[1][1] = centerY;
    corners[1][2] = halfZ + centerZ;
    corners[2][0] = centerX - halfX;
    corners[2][1] = centerY;
    corners[2][2] = centerZ - halfZ;
    corners[3][0] = halfX + centerX;
    corners[3][1] = centerY;
    corners[3][2] = centerZ - halfZ;
    particle_spawnQuadEdgeStyleParticles(corners, centerX, centerZ, style, scale, type, countA, countB, owner, list);
}

/*
 * Spawns `count` dust particles evenly spaced on a circle of `radius` around (x, y, z), each moving
 * outward at 200 units (8.8 fixed point) with a random size of baseSize + rand * sizeScale.
 */
void effect_spawnDustRing(f32 x, f32 y, f32 z, f32 radius, s32 baseSize, f32 sizeScale, s32 color, s32 count,
                   u8 owner, s32 list) {
    s32 i;
    s32 angle;
    u8 a;
    f32 sn;
    f32 cs;
    f32 pz;
    s32 velZ;
    u8 velZHigh;
    u8 velZLow;
    f32 velY;
    s32 size;
    s32 velX;
    u32 r;
    f32 px;
    u8 velXHigh;
    u8 velXLow;
    s32 step;
    u32 r1;
    u32 r2;
    s32 pad; // matching: moves colors to the target's stack slot
    u8 colors[1][3] = { { 0x45, 0x53, 0x68 } };

    if (count != 0) {
        angle = 0;
        step = 0xFFFF / count;
        for (i = 0; i < count; i++) {
            a = angle >> 8;
            angle |= 0; // matching: makes `a` a real variable instead of a CSE temp (one fewer uopt temp slot)
            sn = math_sinBinAngle(a);
            // matching: these no-ops swap s0/s1 to the target's allocation
            a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0;
            cs = math_cosBinAngle(a);
            pz = radius * cs + z;
            velZ = 200.0f * cs;
            velZHigh = velZ >> 8;
            velZLow = velZ;
            r1 = math_random();
            velY = (r1 & 0xFFFF) * (1.0f / 65536) + 100.0f;
            r2 = math_random();
            size = (r2 & 0xFFFF) * sizeScale + baseSize;
            r = math_random();
            velX = 200.0f * sn;
            px = radius * sn + x;
            velXHigh = velX >> 8;
            velXLow = velX;
            particle_newStyleParticle(px, y, pz, 0xD, 0, colors[color][0], colors[color][1], colors[color][2], 0, 0, 0, 0,
                          2, velXHigh, velXLow, velZHigh, velZLow, 0, 0, 5, 0, 0, 0, (s16)size, (s16)size,
                          (r & 0xF) + 10, velY, 0, 0, owner, list);
            angle += step;
        }
    }
}

/* Effect hook for objects, by object id. */
void func_15171BF4(Actor *obj, u8 owner) {
    switch (obj->character_type) {
    case 0x13:
    case 0x23:
        trail_sprayFromRing(obj, owner);
        sound_playActorSound2(0x69, obj, 32000, 200, 2000);
        break;
    case 0x1E:
        func_150BD740(obj, owner, 1);
        break;
    case 0x54:
        particle_spawnGroundEffects(obj, owner, 1);
        break;
    }
}
