#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

s32 func_151BB0E0(); /* sound update callback, defined below */
void func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/* Collision surface record, from the func_150AC9C0 ray cast or a floor query. */
typedef struct {
    s16 unk[9];
} Surface;

/* Floor below a point, from func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ Surface surf;
    /* 0x16 */ u8 pad16[0x24 - 0x16];
} FloorInfo;

void func_1504715C(FloorInfo *, Actor *);

/* Effect objects keep a per-type extra-data block at 0xF8 + extraOffset. */
typedef struct {
    u8 pad0[0x50];
    s32 extraOffset;
} EffectObject;

#define EFFECT_EXTRA(effect) ((void *)((s32)(effect) + ((EffectObject *)(effect))->extraOffset + 0xF8))  // matching: s32 arithmetic; a u8 * sum schedules func_151BA084 differently

/* Extra data of effects that follow an object: the owner and its unique_id at spawn time. */
typedef struct {
    Actor *owner;
    u8 ownerId;
} EffectOwnerRef;


typedef struct {
    u8 unk0;
    s8 unk1;
    u8 unk2;
    u8 pad3;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[0xA];
    s32 unk30;
    s32 unk34;
    u8 unk38;
    u8 unk39;
} Desc1513B5E0;

extern s8 D_800E0BCE[];
extern s8 D_8008FD90;
void *func_1513B5E0(void *, s32, s32, u8, s32);

/* Spawns the effect tied to `owner` and records the owner in the effect's extra data. */
void *func_151B9F00(Actor *owner, u8 arg1, s32 arg2) {
    void *effect;
    Desc1513B5E0 desc;
    EffectOwnerRef ref;

    if (owner == NULL) {
        return NULL;
    }
    ref.owner = owner;
    ref.ownerId = owner->unique_id;
    desc.unk0 = 2;
    desc.unk1 = -1;
    desc.unk2 = 3;
    desc.unk4 = 300;
    desc.unk30 = 9;
    desc.unk34 = 0x1AD;
    desc.unk39 = 0xFF;
    {
        s32 objIndex = owner - D_800CC2D0;

        if ((objIndex < 2) && (D_800E0BCE[objIndex] != 0)) {
            if (D_8008FD90 == 2) {
                desc.unk39 = 0xFD;
            } else if (D_8008FD90 == 4) {
                if (objIndex == 0) {
                    desc.unk39 = 0xFB;
                } else {
                    desc.unk39 = 0xF7;
                }
            }
        }
    }
    desc.unk8 = 1;
    desc.unkC = 0x200205;
    desc.unk10 = 0x60600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.unk38 = 2;
    effect = func_1513B5E0(&desc, 1, 8, arg1, arg2);
    if (effect != NULL) {
        memcpy(EFFECT_EXTRA(effect), &ref, sizeof(ref));
    }
    return effect;
}


typedef struct {
    u8 pad0[0x58];
    Vtx *verts[2][4]; /* [frame][viewport], 20 vertices each */
    Mtx mtx[2];       /* [frame] */
} Obj151BA084;

extern u8 D_800C3E90;
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);

/*
 * Per-frame update of an effect that follows its owner's model: builds the effect's matrix from the
 * owner's and sets each vertex's texture coordinates from its projected position in `viewport`.
 * Removes the effect when the owner is gone; returns 0 when it should not be drawn.
 */
s32 func_151BA084(Obj151BA084 *effect, s16 viewport) {
    EffectOwnerRef *ref;
    Actor *owner;
    u8 *model;
    f32 modelMtx[4][4];
    Actor *player;
    f32 viewMtx[4][4];
    s32 i;
    f32 originS;
    f32 origin[3];
    f32 originWorld[3];
    f32 originT;
    f32 x;
    f32 y;
    f32 z;
    f32 world[3];
    f32 proj[3];

    ref = EFFECT_EXTRA(effect);
    owner = ref->owner;
    if (owner->interaction_state == 0 || owner->unique_id != ref->ownerId) {
        func_1516972C((struct102 *)effect);
    }
    model = (u8 *)owner->unk1D4;
    if (model == NULL) {
        return 0;
    }
    if ((owner->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800BE616 != 0) {
        if (owner->camera != NULL && owner->unk31C != NULL &&
            owner->unk31C->aimState != 0 && viewport == owner->camera->playerIndex) {
            return 0;
        }
    } else {
        player = &D_800CC2D0[0];
        if (player->camera != NULL && player->unk31C != NULL &&
            player->unk31C->aimState != 0 && viewport == player->camera->playerIndex) {
            return 0;
        }
    }
    // matching: the two unused labels stop IDO from hoisting the shared call arguments out of the branches
    if (D_800C3E90 != 0) {
    lbl2:
        guMtxL2F(modelMtx, (Mtx *)(model + 0x40));
    } else {
    lbl0:
        memcpy(modelMtx, model + 0x40, sizeof(modelMtx));
    }
    modelMtx[0][3] = 0.0f;
    modelMtx[1][3] = 0.0f;
    modelMtx[2][3] = 0.0f;
    modelMtx[3][3] = 1.0f;
    guMtxF2L(modelMtx, &effect->mtx[D_800BE9C0]);
    memcpy(viewMtx, &((f32 (*)[4][4])D_800D9D10)[viewport], sizeof(viewMtx));
    viewMtx[0][3] = 0.0f;
    viewMtx[1][3] = 0.0f;
    viewMtx[2][3] = 0.0f;
    viewMtx[3][3] = 1.0f;
label: // matching: unused label keeps the viewMtx stores ahead of the argument setup
    func_150A7960(modelMtx, 0.0f, 67.0f, 159.0f, &originWorld[0], &originWorld[1], &originWorld[2]);
    func_150A7960(viewMtx, originWorld[0], originWorld[1], originWorld[2], &origin[0], &origin[1], &origin[2]);
    origin[0] *= 2.002f;
    origin[1] *= 2.002f;
    /* keep texture coordinates near zero by removing whole 1024-unit tiles */
    originS = (s32)(origin[0] / 1024.0f) * 1024.0f;
    originT = (s32)(origin[1] / 1024.0f) * 1024.0f;
    {
        s32 pad; // matching: -g3 block marker keeps the origin stores after the tile math

        for (i = 0; i < 20; i++) {
            Vtx_t *vtx = &effect->verts[D_800BE9C0][viewport][i].v;

            x = vtx->ob[0];
            y = vtx->ob[1];
            z = vtx->ob[2];
            func_150A7960(modelMtx, x, y, z, &world[0], &world[1], &world[2]);
            func_150A7960(viewMtx, world[0], world[1], world[2], &proj[0], &proj[1], &proj[2]);
            proj[0] *= 2.002f;
            proj[1] *= 2.002f;
            vtx->tc[0] = proj[0] - originS;
            vtx->tc[1] = proj[1] - originT;
        }
    }
    return 1;
}

/* Messages delivered to effects through their message handler. */
typedef union {
    struct {
        Actor *obj;
        u8 objId;
    } removed; /* type 0 */
    struct {
        Actor *a;
        Actor *b;
        u8 aId;
        u8 bId;
    } swap; /* type 0x2D */
} EffectMsg;

/* Message handler: removes the effect with its owner, and follows the owner when two objects swap. */
void func_151BA468(struct260 *effect, EffectMsg *msg, u8 msgType) {
    EffectOwnerRef *ref = EFFECT_EXTRA(effect);
    Actor *owner = ref->owner;

    if (msgType == 0) {
        if ((owner == msg->removed.obj) || (msg->removed.objId == owner->unique_id)) {
            func_1516972C((struct102 *)effect);
        }
    } else if (msgType == 0x2D) {
        if (ref->owner == msg->swap.a) {
            ref->owner = msg->swap.b;
            ref->ownerId = msg->swap.bId;
        } else if (ref->owner == msg->swap.b) {
            ref->owner = msg->swap.a;
            ref->ownerId = msg->swap.aId;
        }
    }
}

/* Spawn descriptor read by func_1514FCE8. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ vertex pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C[6];
    /* 0x58 */ s8 unk58;
} Desc1514FCE8;

/* matching: ParticleSpawn plus 0x4 bytes; the stack frame needs the larger size. */
typedef struct ParticleSpawn80 {
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
    /* 0x7C */ u8 pad7C[0x4];
} ParticleSpawn80;

extern vertex D_800AA590, D_800AA59C, D_800AA5A8, D_800AA5B4[], D_800AA5CC[], D_800AA5FC[];
extern f32 D_800AA62C[];
extern vertex D_800AA63C, D_800AA648[], D_800AA660[], D_800AA678[], D_800AA6A8[], D_800AA6D8[];
extern f32 D_800AA708[];
void func_1514FCE8(Desc1514FCE8 *, u8, s32);
extern void func_1503F404(Mtx *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void func_1513264C(ParticleSpawn80 *, s32, s32, FloorInfo *, s32, u8, s32);
extern void func_15143134(vertex *in, vertex *out, Mtx *mtx);

/*
 * Spawns a burst effect at `obj` plus up to twelve particles launched along fixed paths in the space of
 * the object's model matrix (several of them picked at random).
 */
void func_151BA518(Actor *obj, u8 arg1, s32 arg2) {
    Desc1514FCE8 burst;
    FloorInfo floor;
    vertex objPos;
    ParticleSpawn80 part;
    f32 outA2, outA1, outA0, outB2, outB1, outB0;
    Mtx *mtx;
    s32 i, j, k;
    s32 shift;
    f32 speed, scale, accel;

    burst.unk0 = 0;
    burst.unk2 = 0xFF;
    burst.unk4 = -0x3F;
    burst.unk6 = 0x34;
    burst.unk8 = 6;
    if (D_80082FA0 >= 2) {
        shift = 1;
    } else {
        shift = 0;
    }
    burst.unkC = 5 >> shift;
    burst.pos.x = obj->x_position;
    burst.pos.y = obj->y_position + 25.0f;
    burst.pos.z = obj->z_position;
    burst.unk34 = 6;
    burst.unk1C = 50.0f;
    burst.unk20 = 42.0f;
    burst.unk24 = 98.0f;
    burst.unk28 = 68.0f;
    burst.unk2C = 609.0f;
    burst.unk30 = 271.0f;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        shift = 1;
    } else {
        shift = 0;
    }
    burst.unk38 = 3 >> shift;
    burst.unk4C[0] = 25;
    burst.unk4C[1] = 15;
    burst.unk4C[2] = 100;
    burst.unk4C[3] = 100;
    burst.unk4C[4] = 12;
    burst.unk4C[5] = 20;
    burst.unk3C = 18.0f;
    burst.unk40 = 0.7690000534f;
    burst.unk44 = -0.1690000147f;
    burst.unk48 = 0;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        burst.unk58 = -1;
    } else {
        burst.unk58 = 0;
    }
    func_1514FCE8(&burst, arg1, arg2);
    if ((obj->unk1D4 != NULL) && ((obj->unk74 & 0xF) != 0xF)) {
        func_1504715C(&floor, obj);
        mtx = (Mtx *)((u8 *)obj->unk1D4 + 0x40);
        {
            typedef s32 T151; // matching: a declaration makes -g3 open a scope here, which steers the scheduler
            func_1503F404(mtx, &outA0, &outA1, &outA2, &objPos.x, &objPos.y, &objPos.z, &outB0, &outB1, &outB2);
        }
        {
            typedef s32 T152; // matching: as above
            part.radius = 1.0f;
            part.scale.x = 1.0f;
            part.scale.y = 1.0f;
            part.scale.z = 1.0f;
            part.elasticity = 0.7f;
            part.rotVel.y = 0.0f;
            part.flags = 0x39E9;
            part.variant = 0;
            part.unk5C = 0;
            part.alpha = 0xFF;
            part.updateFn = 0xD;
            part.update2Fn = 0;
            part.floorHitFn = 0xC;
            part.floorType3Fn = 0;
            part.wallHitFn = 0;
            part.destroyFn = 0;
            part.squashFn = 0;
            part.matrixFn = 2;
            part.unk6A = 0;
            part.owner = obj;
            part.ownerId = obj->unique_id;
            part.fadeStart = 0xC;
            part.fadeRate = 0x15;
        }
        scale = 0.001f;
        accel = -488.0f * scale;

        speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
        part.lifetime = (func_150ADA20() % 36U) + 30;
        part.model = 0x27;
        part.sizeXZ = 0.75f;
        part.sizeY = 0.75f;
        func_150ADA68();
        part.gravity = accel;
        func_15143134(&D_800AA590, &part.pos, mtx);
        func_15143134(&D_800AA59C, &part.vel, mtx);
        part.vel.x = part.vel.x - part.pos.x;
        part.vel.y = part.vel.y - part.pos.y;
        part.vel.z = part.vel.z - part.pos.z;
        part.vel.x = part.vel.x * speed;
        part.vel.y = part.vel.y * speed;
        part.vel.z = part.vel.z * speed;
        part.rot = objPos;
        part.rot.x += 109.0f;
        part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);

        speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
        part.lifetime = (func_150ADA20() % 36U) + 30;
        part.model = 0x28;
        part.sizeXZ = 0.5f;
        part.sizeY = 0.5f;
        func_150ADA68();
        part.gravity = accel;
        func_15143134(&D_800AA5A8, &part.pos, mtx);
        func_15143134(&D_800AA5B4[func_150ADA20() & 1], &part.vel, mtx);
        part.vel.x = part.vel.x - part.pos.x;
        part.vel.y = part.vel.y - part.pos.y;
        part.vel.z = part.vel.z - part.pos.z;
        part.vel.x = part.vel.x * speed;
        part.vel.y = part.vel.y * speed;
        part.vel.z = part.vel.z * speed;
        part.rot = objPos;
        part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);

        part.model = 0x29;
        for (i = 0; i != 4; i++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                part.sizeXZ = part.sizeY = D_800AA62C[i];
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA5CC[i], &part.pos, mtx);
                func_15143134(&D_800AA5FC[i], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }

        part.model = 0x2B;
        func_15143134(&D_800AA63C, &part.pos, mtx);
        part.sizeXZ = part.sizeY = 0.551f;
        for (j = 0; j != 2; j++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA648[j], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += D_800AA660[j].x;
                part.rot.y += D_800AA660[j].y;
                part.rot.z += D_800AA660[j].z;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }

        part.model = 0x2C;
        for (k = 0; k != 4; k++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                part.sizeXZ = part.sizeY = D_800AA708[k];
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA678[k], &part.pos, mtx);
                func_15143134(&D_800AA6A8[k], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += D_800AA6D8[k].x;
                part.rot.y += D_800AA6D8[k].y;
                part.rot.z += D_800AA6D8[k].z;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }
    }
}


/* Starts sound 0x4A7 at `obj`; func_151BB0E0 keeps it updated. */
void func_151BB044(Actor *obj) {
    func_1000FA64(0x4A7, obj->x_position, obj->y_position, obj->z_position, 0x2000, 0x320, 0xC8,
                  (s32)func_151BB0E0, obj, 0, 8, 0);
}
s32 func_1000F568(s32, s32);

/* A playing positional sound, as seen by its update callback. */
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ Actor *obj;
    /* 0x1C */ u32 userData; /* bits 0-6 state, 7 fading in, 8-15 fade volume, 16-27 timer */
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ u16 unk24;
} SoundChannel;

/*
 * Update callback of the sounds started by func_151BB044: follows the object, switches between the
 * 0x4A7/0x4A8/0x4A9 sounds by how it moves, and fades the volume. Returns 1 when the sound should stop.
 */
s32 func_151BB0E0(SoundChannel *chan, s32 arg1, u32 *volume, s32 arg3, s32 *speedOut,
                  s32 arg5, u16 *soundId) {
    Actor *obj;
    s32 state;
    s32 prevState;
    s32 sound;
    s32 param;
    s32 fade;
    s32 timer;
    s32 fadingIn;

    obj = chan->obj;
    if ((obj != NULL) && (obj->health != 0)) {
        state = 0;
        sound = 0;
        prevState = chan->userData & 0x7F;
        fade = (chan->userData >> 8) & 0xFF;
        timer = (chan->userData >> 16) & 0xFFF;
        fadingIn = (chan->userData >> 7) & 1;
        chan->x = obj->x_position;
        chan->y = obj->y_position;
        chan->z = obj->z_position;
        if (((obj->xz_velocity > 1.0f) && (obj->unk44 > 0.0f)) ||
            ((obj->xz_velocity < -1.0f) && (obj->unk44 < 0.0f))) {
            state = 1;
        } else if (((obj->xz_velocity > 5.0f) && (obj->unk44 <= 0.0f)) ||
                   ((obj->xz_velocity < -5.0f) && (obj->unk44 >= 0.0f))) {
            state = 2;
        }
        // matching: emits no code; the extra reads of state raise its uopt register priority
        // above sound's, which puts state in v1 and sound in t0
        if (state + state + state + state) {}
        if ((state ^ prevState) & 0x7F) {
            if ((((state ^ prevState) & 0x7F) & 2) && (state & 2)) {
                sound = 0x4A5;
            } else {
                sound = func_1000F568(0x4A3, 4);
            }
            if (!func_10010894(obj)) {
                func_10010154(sound, obj, 0x6D60, 300, 1800);
            }
            sound = 0;
        }
        if ((state != 0) && (*soundId == 0x4A7)) {
            sound = 0x4A8;
            param = 0x2EE0;
        } else if ((state == 0) && ((*soundId == 0x4A9) || (*soundId == 0x4A8))) {
            if (*soundId == 0x4A9) {
                return 1;
            }
            sound = 0x4A7;
            param = 0x2000;
        }
        if (sound != 0) {
            func_1000FA64(sound, obj->x_position, obj->y_position, obj->z_position, param, 0x320, 0xC8,
                          (s32)func_151BB0E0, obj, state | 0x12CFF00, 8, 0);
            goto end; // matching: a plain return 1 here is scheduled differently
        }
        if (*soundId != 0x4A7) {
            if (timer == 0) {
                *volume = (*volume * fade) >> 8;
                if (fadingIn) {
                    fade += 8;
                    if (fade >= 0x100) {
                        fade = 0xFF;
                        timer = (func_150ADA20() & 0x200) + 300;
                        fadingIn = FALSE;
                    }
                } else {
                    fade -= 8;
                    if (fade <= 0) {
                        return 1;
                    }
                }
            } else {
                timer -= D_800BE9E4;
                if (timer <= 0) {
                    timer = 0;
                    if (*soundId == 0x4A8) {
                        func_1000FA64(0x4A9, obj->x_position, obj->y_position, obj->z_position, 0x2EE0, 0x320,
                                      0xC8, (s32)func_151BB0E0, obj, state | 0x880, 8, 0);
                        timer = (func_150ADA20() & 0x200) + 900;
                    }
                }
            }
            *speedOut = fabsf(obj->xz_velocity) * obj->unkB8 * 0.5f;
        }
        chan->userData = (fadingIn << 7) | state | (fade << 8) | (timer << 16);
        if ((*volume == 0) || (chan->flags & 0x80)) {
            if (chan->unk24 != 0) {
                func_100111C8(chan->unk24);
                chan->unk24 = 0;
            }
            *soundId = 0;
            chan->flags &= ~0x80;
            *volume = 0;
        }
        return 0;
    }
end:
    return 1;
}

/* Effect descriptor read by func_1514FBFC. */
typedef struct {
    vertex dir;      /* 0x00 */
    Surface surf;     /* 0x0C */
    u8 pad1E[2];
    f32 unk20;
    s32 unk24;
    s32 unk28;
    vertex pos;      /* 0x2C */
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s16 unk72;
    s8 unk74;
} Desc1514FBFC;

/* Effect descriptor read by func_15151D6C. */
typedef struct {
    s32 unk0;
    s32 unk4;
    vertex pos;      /* 0x08 */
    vertex vel;      /* 0x14 */
    Surface surf;     /* 0x20 */
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
} Desc15151D6C;

/* wrapped in structs so the tables are copied by struct assignment */
typedef struct { s32 v[9]; } S32x9;
typedef struct { f32 v[9]; } F32x9;
extern S32x9 D_800AA718;
extern F32x9 D_800AA73C;
void func_15055A2C(s32, f32, f32, f32, s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, Surface *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
s32 func_15145C90(s32 hitInfo);
s32 func_15144E80(Surface *, vertex *, vertex *, vertex *);
void func_1514FBFC(Desc1514FBFC *, u8, s32);
void func_15151D6C(Desc15151D6C *, s32 *, f32 *, s32, u8, s32);
void func_151C04F8(vertex *, u8, s32);
void func_151C05A4(vertex *, u8, s32);
void func_151C05F0(vertex *, u8, s32);
void func_151C0644(vertex *, u8, s32);
void func_151BBEE4(vertex *pos, u8 arg1, s32 arg2);
void func_151BBFBC(Surface *surf, vertex *pos, s32 arg2, s32 arg3);

/*
 * Impact effect for a shot from `from` towards `to`: effects at the origin, then a ray cast along the
 * shot and, where it hits a surface, impact effects there.
 */
void func_151BB61C(Actor *obj, vertex *from, vertex *to, u8 arg3, s32 arg4) {
    Surface surf;
    vertex hit;
    vertex delta;
    s32 hitInfo;
    vertex edgeA;
    vertex edgeB;
    vertex normal;
    Desc1514FBFC fx;

    if (obj != NULL) {
        func_15055A2C(0, from->x, from->y, from->z, 0);
        delta.x = to->x - from->x;
        delta.y = to->y - from->y;
        delta.z = to->z - from->z;
        func_151C05A4(from, arg3, arg4);
        func_151C05F0(from, arg3, arg4);
        func_151C04F8(from, arg3, arg4);
        func_151BBEE4(from, arg3, arg4);
        if (func_150AC9C0(from->x, from->y, from->z, delta.x, delta.y, delta.z, 0, &surf, &hit.x, &hit.y, &hit.z, NULL,
                          &hitInfo, 0, 0.0f) != 0) {
            if (func_15145C90(hitInfo) != 0) {
                func_151BBFBC(&surf, &hit, arg3 & 0xFF, arg4);
            }
            if (func_15144E80(&surf, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
            fx.dir.x = -delta.x;
            fx.dir.y = -delta.y;
            fx.dir.z = -delta.z;
            fx.surf = surf;
            fx.unk20 = 494.0f;
            fx.pos = hit;
            fx.unk24 = 3;
            fx.unk28 = 4;
            fx.unk38 = 0.0609f;
            fx.unk3C = 0.0252f;
            fx.unk40 = 45.0f;
            fx.unk44 = 53.0f;
            fx.unk48 = 203.0f;
            fx.unk4C = 414.0f;
            fx.unk50 = 7;
            fx.unk54 = 3;
            fx.unk58 = 15.0f;
            fx.unk5C = 0.8800000548f;
            fx.unk60 = -0.3980000317f;
            fx.unk64 = 0.3910000324f;
            fx.unk68 = 25;
            fx.unk6A = 15;
            fx.unk6C = 100;
            fx.unk6E = 100;
            fx.unk70 = 12;
            fx.unk72 = 20;
            fx.unk74 = 0;
            func_1514FBFC(&fx, arg3, arg4);
            {
                S32x9 colors;
                F32x9 scales;
                Desc15151D6C desc;
                colors = D_800AA718;
                scales = D_800AA73C;
                desc.unk0 = 6;
                desc.unk4 = 6;
                desc.pos = hit;
                desc.vel.x = -delta.x;
                desc.vel.y = -delta.y;
                desc.vel.z = -delta.z;
                desc.surf = surf;
                desc.unk34 = 456.0f;
                desc.unk38 = 0.02000000142f;
                desc.unk3C = 0.02000000142f;
                desc.unk40 = -1.71f;
                desc.unk44 = 0.448f;
                desc.unk48 = 50;
                desc.unk4A = 20;
                desc.unk4C = 0.499f;
                desc.unk50 = 0.7010000348f;
                desc.unk54 = 21.502f;
                func_15151D6C(&desc, colors.v, scales.v, 9, arg3, arg4);
            }
        }
    }
}

/* Spawn descriptor read by func_15152190. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ vertex pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Desc15152190;

void func_15152190(Desc15152190 *, vertex *, vertex *, s32, f32, s32, s32, s32);
s32 func_15046C80(vertex *, u16, f32, FloorInfo *);
extern vertex D_8008FBA0;
extern vertex D_8008FBAC;

/*
 * Impact effect halfway between `a` and `b`: a burst there, a ground impact on the floor below it,
 * and a shower of debris.
 */
void func_151BBA9C(Actor *obj, f32 *a, f32 *b, u8 arg3, s32 arg4) {
    vertex center;
    Desc1514FCE8 burst;
    FloorInfo floor;
    vertex probe;
    vertex hit;
    vertex edgeA;
    vertex edgeB;
    vertex normal;

    if (obj != NULL) {
        center.x = (b[0] + a[0]) * 0.5f;
        center.y = (b[1] + a[1]) * 0.5f;
        center.z = (b[2] + a[2]) * 0.5f;
        func_151C04F8(&center, arg3, arg4);
        func_151C05A4(&center, arg3, arg4);
        func_151C05F0(&center, arg3, arg4);
        func_15055A2C(1, center.x, center.y, center.z, 0);
        func_151BBEE4(&center, arg3, arg4);
        burst.unk0 = 0;
        burst.unk2 = 0xFF;
        burst.unk4 = -0x40;
        burst.unk6 = 0x47;
        burst.unk8 = 6;
        burst.unkC = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
        burst.pos = center;
        burst.unk34 = 7;
        burst.unk1C = 23.0f;
        burst.unk20 = 30.0f;
        burst.unk24 = 45.0f;
        burst.unk28 = 53.0f;
        burst.unk2C = 203.0f;
        burst.unk30 = 414.0f;
        burst.unk38 = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
        burst.unk4C[0] = 25;
        burst.unk4C[1] = 15;
        burst.unk4C[2] = 100;
        burst.unk4C[3] = 100;
        burst.unk4C[4] = 12;
        burst.unk4C[5] = 20;
        burst.unk3C = 15.0f;
        burst.unk40 = 0.8720000386f;
        burst.unk44 = -0.3980000317f;
        burst.unk48 = 0.3640000224f;
        if (D_80082FA0 >= 2 || D_8008FD8C >= 8) {
            burst.unk58 = -1;
        } else {
            burst.unk58 = 0;
        }
        func_1514FCE8(&burst, arg3, arg4);
        func_1504715C(&floor, obj);
        probe.x = center.x;
        probe.y = center.y + 100.0f;
        probe.z = center.z;
        if (func_15046C80(&probe, 0, center.y - 1000.0f, &floor) != 0) {
            hit.x = probe.x;
            hit.y = floor.height;
            hit.z = probe.z;
            func_151BBFBC(&floor.surf, &hit, arg3, arg4);
            if (func_15144E80(&floor.surf, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
        }
        {
            Desc15152190 debris;
            vertex first = D_8008FBA0;
            vertex second = D_8008FBAC;

            debris.unk0 = 4;
            debris.unk4 = 8;
            debris.pos = center;
            debris.unk1C = 12.0f;
            debris.unk20 = 6.0f;
            debris.unk14 = 0;
            debris.unk16 = 0xFF;
            debris.unk18 = -0x3F;
            debris.unk1A = 0x57;
            debris.unk2C = 50;
            debris.unk2E = 10;
            debris.unk24 = -1.438000083f;
            debris.unk28 = 0.4490000308f;
            debris.unk30 = 0.2480000108f;
            debris.unk34 = 0.09900000691f;
            debris.unk38 = 22.85900116f;
            func_15152190(&debris, &first, &second, 3, 0.0f, 1, arg3, arg4);
        }
    }
}

/* Spawns a short-lived effect at `pos` with a random size and lifetime. */
void func_151BBEE4(vertex *pos, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() & 0xF) + 5;
    header.unk4 = 0;

    header2.unk0 = pos->x;
    header2.unk4 = pos->y;
    header2.unk8 = pos->z;

    func_151602C0(&header, &header2, (func_150ADA20() % 21U) + 30, 0xFF, 0x61, 0x36, 0xFF, 0, 0, arg1, arg2);
}

void func_150E7FEC(f32, s32, Surface *, vertex *, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns a randomised func_150E7FEC effect on surface `surf` at `pos`. */
void func_151BBFBC(Surface *surf, vertex *pos, s32 arg2, s32 arg3) {
    s32 pad; // matching: unused stack slot
    u32 rnd1;
    f32 rnd0;
    u32 rnd2;

    rnd0 = func_150ADA68();
    rnd1 = func_150ADA20();
    rnd2 = func_150ADA20();
    func_150E7FEC(rnd0 * 75.0f + 75.0f, ((rnd1 % 56) + 200) & 0xFF, surf, pos,
                  (rnd2 % 205) + 299, 1, 1, 0, 0, 0, (u8)arg2, 0);
}

extern u8 D_800AA760;
extern u8 D_800AA76C;
void func_15160CDC(Actor *, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns the func_15160CDC effect (tables D_800AA760/D_800AA76C) on `obj`. */
void func_151BC074(Actor *obj) {
    if (obj != NULL) {
        func_15160CDC(obj, 1, &D_800AA760, &D_800AA76C, 0.01769899949f, 2, 300,
                      0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
    }
}

extern f32 D_800DCA24;

typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 pad1A[2];
} Desc15134908;

/* Objects returned by func_15134908 and func_15163414 hold the owner reference at these offsets. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ EffectOwnerRef ref;
} Fx15134908;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ EffectOwnerRef ref;
} Fx15163414;

typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} Desc15147A80;

/* Data block of the object returned by func_15147A80. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
} Data15147A80;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ Data15147A80 *data;
} Fx15147A80;

Fx15134908 *func_15134908(Desc15134908 *, s32, u8, s32);
Fx15147A80 *func_15147A80(Desc15147A80 *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
Fx15163414 *func_15163414(Header *, f32 *, f32 *, f32 *, s8, u8, u8, u8, u8, u8, u8, u8, s32, u8, s32);

/*
 * Spawns three effects that follow `obj` (the first only while D_80082FA0 < 2 and D_8008FD8C < 5),
 * each with a reference back to its owner.
 */
void func_151BC104(Actor *obj, u8 arg1, s32 arg2) {
    Fx15134908 *fx1;

    if ((D_80082FA0 < 2) && (D_8008FD8C < 5)) {
        Desc15134908 desc;
        EffectOwnerRef ref;

        desc.unkC = 15.0f;
        desc.x = &obj->x_position;
        desc.y = &obj->y_position;
        desc.z = &obj->z_position;
        desc.unk14 = 300;
        desc.unk10 = 0.3033999801f * D_800DCA24;
        desc.unk16 = 4;
        desc.unk17 = 4;
        desc.unk18 = 1;
        desc.unk19 = 0;
        ref.owner = obj;
        ref.ownerId = obj->unique_id;
        fx1 = func_15134908(&desc, 8, arg1, arg2);
        if (fx1 != NULL) {
            memcpy(&fx1->ref, &ref, sizeof(ref));
        }
    }
    {
        Desc15147A80 desc;
        Fx15147A80 *fx2;
        Data15147A80 data;

        desc.unk15 = 40;
        desc.pos.x = obj->x_position;
        desc.pos.y = obj->y_position;
        desc.pos.z = obj->z_position;
        desc.unkC = 300;
        desc.unkE = 6;
        desc.unk10 = 12;
        data.owner = obj;
        data.ownerId = obj->unique_id;
        data.pos = desc.pos;
        data.unk14 = 0.0f;
        data.unk18 = 0.0f;
        data.unk20 = -16384.0f;
        data.unk1C = -16384.0f;
        fx2 = func_15147A80(&desc, 40, 20, 10, 10, 10, 0, 0, 0, arg1, arg2);
        if (fx2 != NULL) {
            memcpy(fx2->data, &data, sizeof(data));
        }
    }
    {
        EffectOwnerRef ref;
        Fx15163414 *fx3;
        Header header;

        ref.owner = obj;
        ref.ownerId = obj->unique_id;
        header.unk0 = 2;
        header.unk1 = 14;
        header.unk2 = 300;
        header.unk4 = 31;
        fx3 = func_15163414(&header, &obj->x_position, &obj->y_position, &obj->z_position,
                           0, 0, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 8, arg1, arg2);
        if (fx3 != NULL) {
            memcpy(&fx3->ref, &ref, sizeof(ref));
        }
    }
}
