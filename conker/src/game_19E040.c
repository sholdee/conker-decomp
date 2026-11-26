#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Template for a type 0x10 model effect; func_15168BE4 copies it into the object's data (ModelFxData in
 * game_1944C0.c; the callback-2 update, func_15169C70, reads it as Debris in game_197120.c).
 */
typedef struct {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ f32 rotation[3];
    /* 0x18 */ f32 velocity[3];
    /* 0x24 */ f32 spin[3];
    /* 0x30 */ f32 scale[3];
    /* 0x3C */ s32 life;
    /* 0x40 */ void *displayList;
    /* 0x44 */ u32 pad44;
    /* 0x48 */ f32 friction;
    /* 0x4C */ f32 bounce;
    /* 0x50 */ f32 spinDamp;
    /* 0x54 */ u8 callback;     /* index into D_8008CA20 */
    /* 0x55 */ u8 primA;
    /* 0x56 */ u8 tileUlt;
    /* 0x57 */ u8 phase;
    /* 0x58 */ u8 flags;
    /* 0x59 */ u8 floorOffset;
    /* 0x5A */ u8 modelId;      /* the func_1518C900 resource displayList came from */
    /* 0x5B */ u8 kind;
    /* 0x5C */ u8 renderMode;
    /* 0x5D */ u8 combineMode;
    /* 0x5E */ u8 pad5E[2];
} ModelFxTemplate;

/* One piece of the func_15170F4C burst. */
struct ModelBurstPiece {
    /* 0x00 */ u16 modelId;
    /* 0x04 */ f32 offset[3];
    /* 0x10 */ f32 rotation[3];
};

/* Start offsets of the six burst models of func_15170B90. */
const f32 D_800A6E90[6][3] = {
    { 37.2f, -15.0f, -46.8f },
    { -19.6f, 49.8f, -47.1f },
    { 13.8f, 59.7f, 30.0f },
    { 27.8f, -30.3f, 44.0f },
    { -37.4f, -47.6f, -32.9f },
    { -48.2f, -16.0f, 30.4f },
};

/* The six pieces func_15170F4C throws out of the actor (model, start offset, rotation). */
const ModelBurstPiece D_800A6ED8[6] = {
    { 338, { -6.0f, 84.0f, 89.0f }, { -40.0f, 0.0f, 0.0f } },
    { 338, { 89.0f, 75.0f, -57.0f }, { -40.0f, 120.0f, 0.0f } },
    { 338, { -79.0f, 59.0f, -57.0f }, { -40.0f, 240.0f, 0.0f } },
    { 338, { 92.0f, -60.0f, -59.0f }, { 30.0f, 60.0f, 0.0f } },
    { 338, { 19.0f, -59.0f, -89.0f }, { 37.0f, 180.0f, 0.0f } },
    { 338, { -92.0f, -66.0f, 55.0f }, { 36.0f, 300.0f, 0.0f } },
};

/* Defined in game_19A8B0.c with a u8 style. This file was compiled with a view that takes an s32 style (no
   narrowing at the call). */
void func_151700D8(f32 (*verts)[3], f32 x, f32 z, s32 style, f32 scale, s32 type, s32 countA, s32 countB, u8 arg8,
                   s32 arg9);

/* Spawns six models bursting up and out of the actor, plus a ring of dust particles around its feet. */
void func_15170B90(WorldObject *actor, s32 modelId, s32 kind, s32 mode, u8 owner, s32 list) {
    s32 i;
    ModelFxTemplate fx;
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
        displayList = func_1518C900(id);
        fx.position[0] = D_800A6E90[i][0] + x;
        fx.position[1] = D_800A6E90[i][1] + y;
        fx.position[2] = D_800A6E90[i][2] + z;
        fx.modelId = id;
        fx.velocity[0] = D_800A6E90[i][0] * 0.08f;
        fx.velocity[1] = fabsf(D_800A6E90[i][1]) * 0.08f + 7.0f;
        fx.velocity[2] = D_800A6E90[i][2] * 0.08f;
        r = func_150ADA20();
        fx.velocity[0] += (f32)(r >> 16) * (1.0f / 16384);
        r = func_150ADA20();
        fx.velocity[1] += (f32)(r >> 16) * (1.0f / 16384);
        r = func_150ADA20();
        fx.velocity[2] += (f32)(r >> 16) * (1.0f / 16384);
        fx.spin[0] = (f32)((s32)func_150ADA20() >> 16) * (1.25f / 16384);
        fx.spin[1] = (f32)((s32)func_150ADA20() >> 16) * (1.25f / 16384);
        fx.spin[2] = (f32)((s32)func_150ADA20() >> 16) * (1.25f / 16384);
        fx.displayList = displayList;
        func_15168BE4(&fx, owner, list);
    }
    func_151718F0(x, y - 80.0f, z, 70.0f, 500, 300.0f / 65536, 0, 10, owner, list);
    actor->inactive = 1;
}

/* Plays the scene-specific burst (scenes 2 and 16) at the actor. */
void func_15170EC4(WorldObject *actor, u8 owner, s32 list) {
    switch (level) {
    case 2:
        func_15170B90(actor, 1, 1, 1, owner, list);
        break;
    case 16:
        func_15170B90(actor, 0xA9, 8, 0, owner, list);
        break;
    }
}

/* Spawns the six D_800A6ED8 pieces flying out of the actor. */
void func_15170F4C(WorldObject *actor, u8 owner, s32 list) {
    s32 i;
    ModelFxTemplate fx;
    ModelBurstPiece *piece;
    f32 x;
    f32 y;
    f32 z;
    s32 count;
    void *displayList;

    count = 6; // matching: IDO emits the target's loop only with the bound in a variable
    piece = D_800A6ED8;
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
        displayList = func_1518C900(piece->modelId);
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
        fx.spin[1] = func_150ADA68() * 4.0f - 2.0f;
        fx.spin[2] = func_150ADA68() * 10.0f - 5.0f;
        fx.displayList = displayList;
        func_15168BE4(&fx, owner, list);
        piece++;
    }
    actor->inactive = 1;
}

/* Effect hook for objects: object 0x33 gets func_150C3D5C first. */
void func_151711C4(Actor *obj) {
    if (obj->character_type == 0x33) {
        func_150C3D5C(obj);
    }
    func_15060F28(obj, 1);
}

/*
 * Spawns `count` models scattered within +-radius/2 of (x, y, z), rising at 0.8-1.2 x `speed`
 * with a random drift and spin and a random scale between minScale and maxScale; they bounce
 * off the floor with `bounce` (friction is -0.75 x bounce).
 */
void func_15171200(s32 count, s32 modelId, f32 x, f32 y, f32 z, f32 speed, f32 minScale, f32 maxScale,
                   f32 radius, s32 kind, s32 life, f32 bounce, u8 owner, s32 list) {
    s32 i;
    ModelFxTemplate fx;
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
        fx.position[0] = (f32)((s32)func_150ADA20() >> 16) * radius + x;
        fx.position[1] = (f32)((s32)func_150ADA20() >> 16) * radius + y;
        fx.position[2] = (f32)((s32)func_150ADA20() >> 16) * radius + z;
        displayList = func_1518C900(modelId);
        fx.displayList = displayList;
        driftX = (f32)((s32)func_150ADA20() >> 16) * (15.0f / 65536);
        fx.velocity[0] = driftX;
        lift = (f32)(func_150ADA20() & 0xFFFF) * liftRange + speed;
        fx.velocity[1] = lift;
        driftZ = (f32)((s32)func_150ADA20() >> 16) * (15.0f / 65536);
        fx.velocity[2] = driftZ;
        fx.spin[0] = (f32)((s32)func_150ADA20() >> 16) * (10.0f / 65536);
        fx.spin[1] = (f32)((s32)func_150ADA20() >> 16) * (10.0f / 65536);
        fx.spin[2] = (f32)((s32)func_150ADA20() >> 16) * (10.0f / 65536);
        fx.scale[0] = (f32)(func_150ADA20() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        fx.scale[1] = (f32)(func_150ADA20() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        scaleZ = (f32)(func_150ADA20() & 0xFFFF) * (1.0f / 65536) * scaleRange + minScale;
        fx.floorOffset = (fx.scale[1] + fx.scale[0] + scaleZ) * 6.666667f + 20.0f;
        fx.scale[2] = scaleZ;
        func_15168BE4(&fx, owner, list);
    }
}

/* Spawns the scene-specific square ring of particles under the actor, chosen by its unk72. */
void func_15171600(WorldObject *actor, u8 owner, s32 list) {
    switch (actor->unique_id) {
    case 0xEB:
        func_151717FC(actor, 400.0f, 500.0f, 0.0f, -1270.0f, 0.0f, 2, 5, 20, 20, 1.0f, owner, list);
        break;
    case 0xF6:
        func_151717FC(actor, 200.0f, 400.0f, -1800.0f, -1000.0f, -1200.0f, 2, 5, 4, 10, 1.0f, owner, list);
        break;
    case 0xF8:
        func_151717FC(actor, 100.0f, 160.0f, -2500.0f, -650.0f, 0.0f, 2, 5, 4, 4, 0.5f, owner, list);
        break;
    case 0xF9:
        func_151717FC(actor, 200.0f, 100.0f, 100.0f, -300.0f, 0.0f, 1, 5, 4, 4, 1.0f, owner, list);
        break;
    }
}

/*
 * Passes func_151700D8 the corners of a (2 * halfX) x (2 * halfZ) rectangle centred at the actor's
 * position plus (offsetX, offsetY, offsetZ).
 */
void func_151717FC(WorldObject *actor, f32 halfX, f32 halfZ, f32 offsetX, f32 offsetY, f32 offsetZ, s32 type,
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
    func_151700D8(corners, centerX, centerZ, style, scale, type, countA, countB, owner, list);
}

/*
 * Spawns `count` dust particles evenly spaced on a circle of `radius` around (x, y, z), each moving
 * outward at 200 units (8.8 fixed point) with a random size of baseSize + rand * sizeScale.
 */
void func_151718F0(f32 x, f32 y, f32 z, f32 radius, s32 baseSize, f32 sizeScale, s32 color, s32 count,
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
            sn = func_15048A40(a);
            // matching: these no-ops swap s0/s1 to the target's allocation
            a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0;
            cs = func_150489B0(a);
            pz = radius * cs + z;
            velZ = 200.0f * cs;
            velZHigh = velZ >> 8;
            velZLow = velZ;
            r1 = func_150ADA20();
            velY = (f32)(r1 & 0xFFFF) * (1.0f / 65536) + 100.0f;
            r2 = func_150ADA20();
            size = (f32)(r2 & 0xFFFF) * sizeScale + (f32)baseSize;
            r = func_150ADA20();
            velX = 200.0f * sn;
            px = radius * sn + x;
            velXHigh = velX >> 8;
            velXLow = velX;
            func_1516D4E8(px, y, pz, 0xD, 0, colors[color][0], colors[color][1], colors[color][2], 0, 0, 0, 0,
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
        func_150AEEB0(obj, owner);
        func_10010154(0x69, obj, 32000, 200, 2000);
        break;
    case 0x1E:
        func_150BD740(obj, owner, 1);
        break;
    case 0x54:
        func_150CDBB0(obj, owner, 1);
        break;
    }
}
