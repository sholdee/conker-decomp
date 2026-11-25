#include <ultra64.h>
/* functions.h declares these two with the wrong return types; hide its prototypes. */
#define func_150ADA20 func_150ADA20_u8_proto
#define func_15048A40 func_15048A40_void_proto
#include "functions.h"
#undef func_150ADA20
#undef func_15048A40
#include "variables.h"

/* Template for a type 0x10 model effect (same layout as ModelFxData in game_1944C0.c); func_15168BE4 copies it. */
typedef struct {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ f32 rotation[3];
    /* 0x18 */ f32 velocity[3];
    /* 0x24 */ f32 spin[3];
    /* 0x30 */ f32 scale[3];
    /* 0x3C */ s32 life;
    /* 0x40 */ void *displayList;
    /* 0x44 */ u32 pad44;
    /* 0x48 */ f32 unk48[3];
    /* 0x54 */ u8 callback;
    /* 0x55 */ u8 primA;
    /* 0x56 */ u8 tileUlt;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 modelId;
    /* 0x5B */ u8 unk5B;
    /* 0x5C */ u8 renderMode;
    /* 0x5D */ u8 combineMode;
    /* 0x5E */ u8 pad5E[2];
} ModelFxTemplate;

/* The actor an effect is spawned at (struct131 / Obj1511C638 layout); only the fields used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x6E - 0x16];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 pad6F[0x72 - 0x6F];
    /* 0x72 */ u8 unk72;
} EffectActor;

/* One piece of the func_15170F4C burst. */
typedef struct {
    /* 0x00 */ u16 modelId;
    /* 0x04 */ f32 offset[3];
    /* 0x10 */ f32 rotation[3];
} BurstPiece;

extern f32 D_800A6E90[6][3];
extern BurstPiece D_800A6ED8[6];
extern s32 D_800BE9F0;

u32 func_150ADA20(void);
f32 func_15048A40(u8);
f32 func_150489B0(u8);
void *func_1518C900(s32);
void func_15168BE4(ModelFxTemplate *, u8, s32);
void func_15170B90(EffectActor *, s32, s32, s32, u8, s32);
void func_151717FC(EffectActor *, f32, f32, f32, f32, f32, s32, s32, s32, s32, f32, u8, s32);
void func_151718F0(f32, f32, f32, f32, s32, f32, s32, s32, s32, s32);
void func_151700D8(f32 (*)[3], f32, f32, s32, f32, s32, s32, s32, s32, s32);
void func_1516D4E8(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns six models bursting up and out of the actor, plus a ring of dust particles around its feet. */
void func_15170B90(EffectActor *actor, s32 modelId, s32 arg2, s32 mode, u8 owner, s32 list) {
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
    fx.unk48[0] = 0.3f;
    fx.unk48[1] = -0.7f;
    fx.callback = 2;
    fx.unk48[2] = -0.2f;
    fx.primA = 255;
    fx.tileUlt = 0;
    fx.unk57 = 0;
    fx.unk58 = 0;
    fx.unk59 = 60;
    fx.unk5B = arg2;
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
    actor->unk6E = 1;
}

/* Plays the scene-specific burst (scenes 2 and 16) at the actor. */
void func_15170EC4(EffectActor *actor, u8 owner, s32 list) {
    switch (D_800BE9F0) {
    case 2:
        func_15170B90(actor, 1, 1, 1, owner, list);
        break;
    case 16:
        func_15170B90(actor, 0xA9, 8, 0, owner, list);
        break;
    }
}

/* Spawns the six D_800A6ED8 pieces flying out of the actor. */
void func_15170F4C(EffectActor *actor, u8 owner, s32 list) {
    s32 i;
    ModelFxTemplate fx;
    BurstPiece *piece;
    f32 x;
    f32 y;
    f32 z;
    s32 count;
    void *displayList;

    count = 6;
    piece = D_800A6ED8;
    fx.renderMode = 2;
    fx.combineMode = 1;
    fx.unk5B = 6;
    fx.unk59 = 0;
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
    fx.unk57 = 0;
    fx.unk58 = 0;
    fx.unk48[0] = 0.3f;
    fx.unk48[1] = -0.7f;
    fx.unk48[2] = -0.2f;
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
    actor->unk6E = 1;
}

void func_150C3D5C(struct127 *);

/* Effect hook for objects: object 0x33 gets func_150C3D5C first. */
void func_151711C4(struct127 *obj) {
    if (obj->id == 0x33) {
        func_150C3D5C(obj);
    }
    func_15060F28(obj, 1);
}

/*
 * Spawns `count` models scattered within +-radius/2 of (x, y, z), rising at 0.8-1.2 x `speed`
 * with a random drift and spin and a random scale between minScale and maxScale.
 */
void func_15171200(s32 count, s32 modelId, f32 x, f32 y, f32 z, f32 speed, f32 minScale, f32 maxScale,
                   f32 radius, s32 arg9, s32 life, f32 arg11, u8 owner, s32 list) {
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
    fx.unk48[0] = arg11 * -0.75f;
    fx.unk48[1] = arg11;
    fx.callback = 2;
    fx.primA = 255;
    fx.tileUlt = 0;
    fx.unk57 = 0;
    fx.unk58 = 0;
    fx.modelId = modelId;
    fx.unk5B = arg9;
    fx.renderMode = 1;
    fx.combineMode = 1;
    radius *= 1.0f / 65536;
    fx.unk48[2] = -0.2f;
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
        fx.unk59 = (fx.scale[1] + fx.scale[0] + scaleZ) * 6.666667f + 20.0f;
        fx.scale[2] = scaleZ;
        func_15168BE4(&fx, owner, list);
    }
}

/* Spawns the scene-specific square ring of particles under the actor, chosen by its unk72. */
void func_15171600(EffectActor *actor, u8 owner, s32 list) {
    switch (actor->unk72) {
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
void func_151717FC(EffectActor *actor, f32 halfX, f32 halfZ, f32 offsetX, f32 offsetY, f32 offsetZ, s32 type,
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

f32 func_15048A40(u8);
void func_151718F0(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, f32 arg5, s32 arg6, s32 arg7,
                   s32 arg8, s32 arg9) {
    s32 i;
    s32 angle;
    u8 a;
    f32 sn;
    f32 cs;
    f32 z;
    s32 c;
    u8 chi;
    u8 clo;
    f32 scale;
    s32 spd;
    s32 s;
    u32 r;
    f32 x;
    u8 shi;
    u8 slo;
    s32 step;
    u32 r1;
    u32 r2;
    s32 pad;
    const u8 colors[1][3] = { { 0x45, 0x53, 0x68 } };

    if (arg7 != 0) {
        angle = 0;
        step = 0xFFFF / arg7;
        for (i = 0; i < arg7; i++) {
            a = angle >> 8;
            angle |= 0;
            sn = func_15048A40(a);
            a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0; a |= 0;
            cs = func_150489B0(a);
            z = arg3 * cs + arg2;
            c = 200.0f * cs;
            chi = c >> 8;
            clo = c;
            r1 = func_150ADA20();
            scale = (f32)(r1 & 0xFFFF) * 0.0000152587890625f + 100.0f;
            r2 = func_150ADA20();
            spd = (f32)(r2 & 0xFFFF) * arg5 + (f32)arg4;
            r = func_150ADA20();
            s = 200.0f * sn;
            x = arg3 * sn + arg0;
            shi = s >> 8;
            slo = s;
            func_1516D4E8(x, arg1, z, 0xD, 0, colors[arg6][0], colors[arg6][1],
                          colors[arg6][2], 0, 0, 0, 0, 2, shi, slo, chi, clo, 0, 0, 5, 0, 0,
                          0, (s16)spd, (s16)spd, (r & 0xF) + 0xA, scale, 0, 0, (u8)arg8, arg9);
            angle += step;
        }
    }
}

void func_150AEEB0(struct127 *);
void func_150BD740(struct127 *, u8, s32);
void func_150CDBB0(struct127 *, u8, s32);

/* Effect hook for objects, by object id. */
void func_15171BF4(struct127 *obj, u8 owner) {
    switch (obj->id) {
    case 0x13:
    case 0x23:
        func_150AEEB0(obj);
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
