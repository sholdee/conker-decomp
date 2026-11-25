#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_151BB0E0();
void func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/* Effect objects keep a per-type extra-data block at 0xF8 + extraOffset. */
typedef struct {
    u8 pad0[0x50];
    s32 extraOffset;
} EffectObject;

#define EFFECT_EXTRA(effect) ((void *)((s32)(effect) + ((EffectObject *)(effect))->extraOffset + 0xF8))

/* Extra data of effects that follow an object: the owner and its unique_id at spawn time. */
typedef struct {
    struct127 *owner;
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
} Spawn151B9F00;

extern s8 D_800E0BCE[];
extern s8 D_8008FD90;
void *func_1513B5E0(void *, s32, s32, u8, s32);

/* Spawns the effect tied to `owner` and records the owner in the effect's extra data. */
void *func_151B9F00(struct127 *owner, u8 arg1, s32 arg2) {
    void *effect;
    Spawn151B9F00 desc;
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

typedef struct {
    u8 pad0[0x197];
    u8 unk197;
} Obj31C_151BA084;

extern u8 D_800C3E90;
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);

/*
 * Per-frame update of an effect that follows its owner's model: builds the effect's matrix from the
 * owner's and sets each vertex's texture coordinates from its projected position in `viewport`.
 * Removes the effect when the owner is gone; returns 0 when it should not be drawn.
 */
s32 func_151BA084(Obj151BA084 *effect, s16 viewport) {
    EffectOwnerRef *ref;
    struct127 *owner;
    u8 *model;
    f32 modelMtx[4][4];
    struct127 *player;
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
            ((Obj31C_151BA084 *)owner->unk31C)->unk197 != 0 && viewport == owner->camera->unk23D) {
            return 0;
        }
    } else {
        player = &D_800CC2D0[0];
        if (player->camera != NULL && player->unk31C != NULL &&
            ((Obj31C_151BA084 *)player->unk31C)->unk197 != 0 && viewport == player->camera->unk23D) {
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
        struct127 *obj;
        u8 objId;
    } removed; /* type 0 */
    struct {
        struct127 *a;
        struct127 *b;
        u8 aId;
        u8 bId;
    } swap; /* type 0x2D */
} EffectMsg;

/* Message handler: removes the effect with its owner, and follows the owner when two objects swap. */
void func_151BA468(struct260 *effect, EffectMsg *msg, u8 msgType) {
    EffectOwnerRef *ref = EFFECT_EXTRA(effect);
    struct127 *owner = ref->owner;

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

typedef struct { f32 x, y, z; } V3_151BA518;
typedef struct {
    s16 a0, a2, a4, a6;
    s32 a8, aC;
    f32 f10, f14, f18;
    f32 f1c, f20, f24, f28, f2c, f30;
    s32 i34, i38;
    f32 f3c, f40, f44, f48;
    s16 s4c[6];
    s8 b58;
} Cfg151BA518;
typedef struct {
    f32 f0, f4, f8, fC;
    V3_151BA518 pos;
    f32 f1c, f20, f24;
    V3_151BA518 t;
    V3_151BA518 vel;
    f32 f40, f44, f48, f4c;
    s32 i50;
    s16 s54, s56;
    s8 b58;
    s32 i5c;
    u8 b60, b61, b62, b63, b64, b65, b66, b67, b68, b69;
    s8 b6a;
    void *p6c;
    u8 b70;
    s16 s72, s74;
    s32 pad78, pad7c;
} Part151BA518;
extern V3_151BA518 D_800AA590, D_800AA59C, D_800AA5A8, D_800AA5B4[], D_800AA5CC[], D_800AA5FC[];
extern f32 D_800AA62C[];
extern V3_151BA518 D_800AA63C, D_800AA648[], D_800AA660[], D_800AA678[], D_800AA6A8[], D_800AA6D8[];
extern f32 D_800AA708[];
void func_1514FCE8(s16 *, u8, s32);
extern void func_1504715C(void *, void *);
extern void func_1503F404(s32, f32 *, f32 *, f32 *, V3_151BA518 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void func_1513264C(Part151BA518 *, s32, s32, void *, s32, u8, s32);
extern void func_15143134(V3_151BA518 *, V3_151BA518 *, s32);

void func_151BA518(void *arg0, u8 arg1, s32 arg2) {
    Cfg151BA518 cfg;
    u8 sp168[0x24];
    V3_151BA518 sp15C;
    Part151BA518 p;
    f32 spD8, spD4, spD0, spCC, spC8, spC4;
    s32 s3;
    s32 i1, i2, i3;
    s32 v0;
    f32 f20, f22, f28;

    cfg.a0 = 0;
    cfg.a2 = 0xFF;
    cfg.a4 = -0x3F;
    cfg.a6 = 0x34;
    cfg.a8 = 6;
    if (D_80082FA0 >= 2) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    cfg.aC = 5 >> v0;
    cfg.f10 = ((f32 *)arg0)[5];
    cfg.f14 = ((f32 *)arg0)[6] + 25.0f;
    cfg.f18 = ((f32 *)arg0)[7];
    cfg.i34 = 6;
    cfg.f1c = 50.0f;
    cfg.f20 = 42.0f;
    cfg.f24 = 98.0f;
    cfg.f28 = 68.0f;
    cfg.f2c = 609.0f;
    cfg.f30 = 271.0f;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    cfg.i38 = 3 >> v0;
    cfg.s4c[0] = 0x19;
    cfg.s4c[1] = 0xF;
    cfg.s4c[2] = 0x64;
    cfg.s4c[3] = 0x64;
    cfg.s4c[4] = 0xC;
    cfg.s4c[5] = 0x14;
    cfg.f3c = 18.0f;
    cfg.f40 = 0.7690000534f;
    cfg.f44 = -0.1690000147f;
    cfg.f48 = 0;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        cfg.b58 = -1;
    } else {
        cfg.b58 = 0;
    }
    func_1514FCE8((s16 *)&cfg, arg1, arg2);
    if ((*(s32 *)((u8 *)arg0 + 0x1D4) != 0) && ((*((u8 *)arg0 + 0x74) & 0xF) != 0xF)) {
        func_1504715C(sp168, arg0);
        s3 = *(s32 *)((u8 *)arg0 + 0x1D4) + 0x40;
        {
            typedef s32 T151;
            func_1503F404(s3, &spD0, &spD4, &spD8, &sp15C.x, &sp15C.y, &sp15C.z, &spC4, &spC8, &spCC);
        }
        {
            typedef s32 T152;
            p.f0 = 1.0f;
            p.f1c = 1.0f;
            p.f20 = 1.0f;
            p.f24 = 1.0f;
            p.f4 = 0.6999999881f;
            p.f44 = 0.0f;
            p.i50 = 0x39E9;
            p.b58 = 0;
            p.i5c = 0;
            p.b60 = 0xFF;
            p.b61 = 0xD;
            p.b62 = 0;
            p.b63 = 0xC;
            p.b64 = 0;
            p.b65 = 0;
            p.b66 = 0;
            p.b67 = 0;
            p.b68 = 2;
            p.b6a = 0;
            p.p6c = arg0;
            p.b70 = *((u8 *)arg0 + 0x3B);
            p.s72 = 0xC;
            p.s74 = 0x15;
        }
        f22 = 0.001000000047f;
        f28 = -488.0f * f22;
        f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
        p.s54 = (func_150ADA20() % 36U) + 0x1E;
        p.s56 = 0x27;
        p.f8 = 0.75f;
        p.fC = 0.75f;
        func_150ADA68();
        p.f4c = f28;
        func_15143134(&D_800AA590, &p.t, s3);
        func_15143134(&D_800AA59C, &p.vel, s3);
        p.vel.x = p.vel.x - p.t.x;
        p.vel.y = p.vel.y - p.t.y;
        p.vel.z = p.vel.z - p.t.z;
        p.vel.x = p.vel.x * f20;
        p.vel.y = p.vel.y * f20;
        p.vel.z = p.vel.z * f20;
        p.pos = sp15C;
        p.pos.x += 109.0f;
        p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);

        f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
        p.s54 = (func_150ADA20() % 36U) + 0x1E;
        p.s56 = 0x28;
        p.f8 = 0.5f;
        p.fC = 0.5f;
        func_150ADA68();
        p.f4c = f28;
        func_15143134(&D_800AA5A8, &p.t, s3);
        func_15143134(&D_800AA5B4[func_150ADA20() & 1], &p.vel, s3);
        p.vel.x = p.vel.x - p.t.x;
        p.vel.y = p.vel.y - p.t.y;
        p.vel.z = p.vel.z - p.t.z;
        p.vel.x = p.vel.x * f20;
        p.vel.y = p.vel.y * f20;
        p.vel.z = p.vel.z * f20;
        p.pos = sp15C;
        p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);

        p.s56 = 0x29;
        for (i1 = 0; i1 != 4; i1++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                p.f8 = p.fC = D_800AA62C[i1];
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA5CC[i1], &p.t, s3);
                func_15143134(&D_800AA5FC[i1], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
        p.s56 = 0x2B;
        func_15143134(&D_800AA63C, &p.t, s3);
        p.f8 = p.fC = 0.550999999f;
        for (i2 = 0; i2 != 2; i2++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA648[i2], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.pos.x += D_800AA660[i2].x;
                p.pos.y += D_800AA660[i2].y;
                p.pos.z += D_800AA660[i2].z;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
        p.s56 = 0x2C;
        for (i3 = 0; i3 != 4; i3++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                p.f8 = p.fC = D_800AA708[i3];
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA678[i3], &p.t, s3);
                func_15143134(&D_800AA6A8[i3], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.pos.x += D_800AA6D8[i3].x;
                p.pos.y += D_800AA6D8[i3].y;
                p.pos.z += D_800AA6D8[i3].z;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
    }
}


void func_151BB044(struct127 *arg0) {
    func_1000FA64(0x4A7, (s16)(s32)arg0->x_position,
                  (s16)(s32)arg0->y_position,
                  (s16)(s32)arg0->z_position, 0x2000, 0x320, 0xC8,
                  (s32)func_151BB0E0, arg0, 0, 8, 0);
}
s32 func_1000F568(s32, s32);

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ struct127 *unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ u16 unk24;
} Snd151BB0E0;

s32 func_151BB0E0(Snd151BB0E0 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 *arg4,
                  s32 arg5, u16 *arg6) {
    struct127 *obj;
    s32 state;
    s32 prev;
    s32 sound;
    s32 param;
    s32 vol;
    s32 timer;
    s32 flag;

    obj = arg0->unk18;
    if ((obj != NULL) && (obj->health != 0)) {
        state = 0;
        sound = 0;
        prev = arg0->unk1C & 0x7F;
        vol = (arg0->unk1C >> 8) & 0xFF;
        timer = (arg0->unk1C >> 16) & 0xFFF;
        flag = (arg0->unk1C >> 7) & 1;
        arg0->unk2 = obj->x_position;
        arg0->unk4 = obj->y_position;
        arg0->unk6 = obj->z_position;
        if (((obj->xz_velocity > 1.0f) && (obj->unk44 > 0.0f)) ||
            ((obj->xz_velocity < -1.0f) && (obj->unk44 < 0.0f))) {
            state = 1;
        } else if (((obj->xz_velocity > 5.0f) && (obj->unk44 <= 0.0f)) ||
                   ((obj->xz_velocity < -5.0f) && (obj->unk44 >= 0.0f))) {
            state = 2;
        }
        /* Emits no code. The four extra reads of state raise its uopt register
           priority above sound's, which puts state in v1 and sound in t0. */
        if (state + state + state + state) {}
        if ((state ^ prev) & 0x7F) {
            if ((((state ^ prev) & 0x7F) & 2) && (state & 2)) {
                sound = 0x4A5;
            } else {
                sound = func_1000F568(0x4A3, 4);
            }
            if (!func_10010894(obj)) {
                func_10010154(sound, obj, 0x6D60, 0x12C, 0x708);
            }
            sound = 0;
        }
        if ((state != 0) && (*arg6 == 0x4A7)) {
            sound = 0x4A8;
            param = 0x2EE0;
        } else if ((state == 0) && ((*arg6 == 0x4A9) || (*arg6 == 0x4A8))) {
            if (*arg6 == 0x4A9) {
                return 1;
            }
            sound = 0x4A7;
            param = 0x2000;
        }
        if (sound != 0) {
            func_1000FA64(sound, obj->x_position, obj->y_position, obj->z_position, param, 0x320, 0xC8,
                          (s32)func_151BB0E0, obj, state | 0x12CFF00, 8, 0);
            goto end;
        }
        if (*arg6 != 0x4A7) {
            if (timer == 0) {
                *arg2 = (*arg2 * vol) >> 8;
                if (flag) {
                    vol += 8;
                    if (vol >= 0x100) {
                        vol = 0xFF;
                        timer = (func_150ADA20() & 0x200) + 0x12C;
                        flag = 0;
                    }
                } else {
                    vol -= 8;
                    if (vol <= 0) {
                        return 1;
                    }
                }
            } else {
                timer -= D_800BE9E4;
                if (timer <= 0) {
                    timer = 0;
                    if (*arg6 == 0x4A8) {
                        func_1000FA64(0x4A9, obj->x_position, obj->y_position, obj->z_position, 0x2EE0, 0x320,
                                      0xC8, (s32)func_151BB0E0, obj, state | 0x880, 8, 0);
                        timer = (func_150ADA20() & 0x200) + 0x384;
                    }
                }
            }
            *arg4 = fabsf(obj->xz_velocity) * obj->unkB8 * 0.5f;
        }
        arg0->unk1C = (flag << 7) | state | (vol << 8) | (timer << 16);
        if ((*arg2 == 0) || (arg0->unk10 & 0x80)) {
            if (arg0->unk24 != 0) {
                func_100111C8(arg0->unk24);
                arg0->unk24 = 0;
            }
            *arg6 = 0;
            arg0->unk10 &= ~0x80;
            *arg2 = 0;
        }
        return 0;
    }
end:
    return 1;
}

typedef struct { f32 x, y, z; } Vec3_151BB61C;
typedef struct { s16 v[9]; } Surf_151BB61C;
typedef struct {
    Vec3_151BB61C dir;      /* 0x00 */
    Surf_151BB61C surf;     /* 0x0C */
    u8 pad1E[2];
    f32 unk20;
    s32 unk24;
    s32 unk28;
    Vec3_151BB61C pos;      /* 0x2C */
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
} Fbfc_151BB61C;
typedef struct {
    s32 count;
    s32 spread;
    Vec3_151BB61C pos;      /* 0x08 */
    Vec3_151BB61C vel;      /* 0x14 */
    Surf_151BB61C surf;     /* 0x20 */
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
} Desc_151BB61C;

typedef struct { s32 v[9]; } Colors_151BB61C;
typedef struct { f32 v[9]; } Scales_151BB61C;
extern Colors_151BB61C D_800AA718;
extern Scales_151BB61C D_800AA73C;
void func_15055A2C(s32, f32, f32, f32, s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
s32 func_15145C90(s32);
s32 func_15144E80(void *, Vec3_151BB61C *, Vec3_151BB61C *, Vec3_151BB61C *);
void func_1514FBFC(void *, u8, s32);
void func_15151D6C(void *, s32 *, f32 *, s32, u8, s32);
void func_151C04F8(f32 *, u8, s32);
void func_151C05A4(void *, u8, s32);
void func_151C05F0(void *, u8, s32);
void func_151C0644(void *, u8, s32);
void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2);
void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_151BB61C(s32 arg0, Vec3_151BB61C *arg1, Vec3_151BB61C *arg2, u8 arg3, s32 arg4) {
    Surf_151BB61C surf;
    Vec3_151BB61C hit;
    Vec3_151BB61C delta;
    s32 sp1A0;
    Vec3_151BB61C sp194;
    Vec3_151BB61C sp188;
    Vec3_151BB61C sp17C;
    Fbfc_151BB61C fx;

    if (arg0 != 0) {
        func_15055A2C(0, arg1->x, arg1->y, arg1->z, 0);
        delta.x = arg2->x - arg1->x;
        delta.y = arg2->y - arg1->y;
        delta.z = arg2->z - arg1->z;
        func_151C05A4(arg1, arg3, arg4);
        func_151C05F0(arg1, arg3, arg4);
        func_151C04F8((f32 *)arg1, arg3, arg4);
        func_151BBEE4((f32 *)arg1, arg3, arg4);
        if (func_150AC9C0(arg1->x, arg1->y, arg1->z, delta.x, delta.y, delta.z, 0, &surf, &hit.x, &hit.y, &hit.z, 0, &sp1A0, 0, 0.0f) != 0) {
            if (func_15145C90(sp1A0) != 0) {
                func_151BBFBC((s32)&surf, (s32)&hit, arg3 & 0xFF, arg4);
            }
            if (func_15144E80(&surf, &sp194, &sp188, &sp17C) != 0 && sp17C.y < 0.0f) {
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
            fx.unk38 = 0.06089999899f;
            fx.unk3C = 0.02520000003f;
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
            Colors_151BB61C colors;
            Scales_151BB61C scales;
            Desc_151BB61C desc;
            colors = D_800AA718;
            scales = D_800AA73C;
            desc.count = 6;
            desc.spread = 6;
            desc.pos = hit;
            desc.vel.x = -delta.x;
            desc.vel.y = -delta.y;
            desc.vel.z = -delta.z;
            desc.surf = surf;
            desc.unk34 = 456.0f;
            desc.unk38 = 0.02000000142f;
            desc.unk3C = 0.02000000142f;
            desc.unk40 = -1.710000038f;
            desc.unk44 = 0.4480000138f;
            desc.unk48 = 50;
            desc.unk4A = 20;
            desc.unk4C = 0.4990000129f;
            desc.unk50 = 0.7010000348f;
            desc.unk54 = 21.50200081f;
            func_15151D6C(&desc, colors.v, scales.v, 9, arg3, arg4);
            }
        }
    }
}

typedef struct Vec3i_151BBA9C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BBA9C;

typedef struct Effect_151BBA9C {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    s32 field08;
    s32 field0C;
    Vec3i_151BBA9C position;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    s32 field34;
    s32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    s16 field54;
    s16 field56;
    s8 field58;
    u8 pad59[3];
} Effect_151BBA9C;

typedef struct Config_151BBA9C {
    s32 countBase, countRange;
    Vec3_151BB61C position;
    s16 yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange, durationBase, durationRange;
    s16 lifeBase, lifeRange;
    f32 sizeBase, sizeRange, jitter;
} Config_151BBA9C;

void func_1514FCE8(s16 *, u8, s32);
void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
s32 func_15046C80(f32 *, u16, f32, void *);
void func_1504715C(void *, void *);
extern Vec3_151BB61C D_8008FBA0;
extern Vec3_151BB61C D_8008FBAC;
extern f32 D_800AA80C;
extern f32 D_800AA810;
extern f32 D_800AA814;
extern f32 D_800AA818;
extern f32 D_800AA81C;
extern f32 D_800AA820;
extern f32 D_800AA824;
extern f32 D_800AA828;

void func_151BBA9C(s32 arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4) {
    Vec3_151BB61C center;
    Effect_151BBA9C effect;
    f32 contact[8];
    f32 ground;
    Vec3_151BB61C probe;
    Vec3_151BB61C hit;
    Vec3_151BB61C normal;
    Vec3_151BB61C tangent;
    Vec3_151BB61C facing;

    if (arg0 != 0) {
        center.x = (arg2[0] + arg1[0]) * 0.5f;
        center.y = (arg2[1] + arg1[1]) * 0.5f;
        center.z = (arg2[2] + arg1[2]) * 0.5f;
        func_151C04F8(&center.x, arg3, arg4);
        func_151C05A4(&center.x, arg3, arg4);
        func_151C05F0(&center.x, arg3, arg4);
        func_15055A2C(1, center.x, center.y, center.z, 0);
        func_151BBEE4(&center.x, arg3, arg4);
        effect.field00 = 0;
        effect.field02 = 0xFF;
        effect.field04 = -0x40;
        effect.field06 = 0x47;
        effect.field08 = 6;
        effect.field0C = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
        effect.position = *(Vec3i_151BBA9C *)&center;
        effect.field34 = 7;
        effect.field1C = 23.0f;
        effect.field20 = 30.0f;
        effect.field24 = 45.0f;
        effect.field28 = 53.0f;
        effect.field2C = 203.0f;
        effect.field30 = 414.0f;
        effect.field38 = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
        effect.field4C = 0x19;
        effect.field4E = 0xF;
        effect.field50 = 0x64;
        effect.field52 = 0x64;
        effect.field54 = 0xC;
        effect.field56 = 0x14;
        effect.field3C = 15.0f;
        effect.field40 = D_800AA80C;
        effect.field44 = D_800AA810;
        effect.field48 = D_800AA814;
        if (D_80082FA0 >= 2 || D_8008FD8C >= 8) {
            effect.field58 = -1;
        } else {
            effect.field58 = 0;
        }
        func_1514FCE8(&effect.field00, arg3, arg4);
        func_1504715C(&ground, (void *)arg0);
        probe.x = center.x;
        probe.y = center.y + 100.0f;
        probe.z = center.z;
        if (func_15046C80(&probe.x, 0, center.y - 1000.0f, &ground) != 0) {
            hit.x = probe.x;
            hit.y = ground;
            hit.z = probe.z;
            func_151BBFBC((s32)contact, (s32)&hit, arg3, arg4);
            if (func_15144E80(contact, &normal, &tangent, &facing) != 0 && facing.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
        }
        {
            Config_151BBA9C config;
            Vec3_151BB61C first = D_8008FBA0;
            Vec3_151BB61C second = D_8008FBAC;

            config.countBase = 4;
            config.countRange = 8;
            config.position = center;
            config.speedBase = 12.0f;
            config.speedRange = 6.0f;
            config.yawBase = 0;
            config.yawRange = 0xFF;
            config.pitchBase = -0x3F;
            config.pitchRange = 0x57;
            config.lifeBase = 0x32;
            config.lifeRange = 0xA;
            config.durationBase = D_800AA818;
            config.durationRange = D_800AA81C;
            config.sizeBase = D_800AA820;
            config.sizeRange = D_800AA824;
            config.jitter = D_800AA828;
            func_15152190(&config, &first, &second, 3, 0.0f, 1, arg3, arg4);
        }
    }
}

void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() & 0xF) + 5;
    header.unk4 = 0;

    header2.unk0 = (s32) arg0[0];
    header2.unk4 = (s32) arg0[1];
    header2.unk8 = (s32) arg0[2];

    func_151602C0(&header, &header2, (func_150ADA20() % 0x15U) + 0x1E, 0xFF, 0x61, 0x36, 0xFF, 0, 0, arg1, arg2);
}

void func_150E7FEC(f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    union { u32 w; f64 _a; } r1;
    f32 temp_f;
    u32 r2;

    temp_f = func_150ADA68();
    r1.w = func_150ADA20();
    r2 = func_150ADA20();
    func_150E7FEC(temp_f * 75.0f + 75.0f, ((r1.w % 0x38) + 0xC8) & 0xFF, arg0, arg1,
                  (r2 % 0xCD) + 0x12B, 1, 1, 0, 0, 0, (u8)arg2, 0);
}

extern f32 D_800AA82C;
extern u8 D_800AA760;
extern u8 D_800AA76C;
void func_15160CDC(s32, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151BC074(s32 arg0) {
    if (arg0 != 0) {
        func_15160CDC(arg0, 1, &D_800AA760, &D_800AA76C, D_800AA82C, 2, 0x12C,
                      0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
    }
}

extern f32 D_800AA830;
extern f32 D_800DCA24;
void *func_15134908(void *, s32, u8, s32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void *func_15163414(Header *, f32 *, f32 *, f32 *, s8, u8, u8, u8, u8, u8, u8, u8, s32, u8, s32);

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 pad1A[2];
} Spawn151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
} Extra151BC104;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec151BC104;

typedef struct {
    Vec151BC104 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Header151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    u8 pad5[3];
    Vec151BC104 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Payload151BC104;

void func_151BC104(struct127 *arg0, u8 arg1, s32 arg2) {
    void *temp_v0;

    if ((D_80082FA0 < 2) && (D_8008FD8C < 5)) {
        Spawn151BC104 spawn;
        Extra151BC104 extra;
        spawn.unkC = 15.0f;
        spawn.unk0 = &arg0->x_position;
        spawn.unk4 = &arg0->y_position;
        spawn.unk8 = &arg0->z_position;
        spawn.unk14 = 0x12C;
        spawn.unk10 = D_800AA830 * D_800DCA24;
        spawn.unk16 = 4;
        spawn.unk17 = 4;
        spawn.unk18 = 1;
        spawn.unk19 = 0;
        extra.owner = arg0;
        extra.ownerId = arg0->unique_id;
        temp_v0 = func_15134908(&spawn, 8, arg1, arg2);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x40, &extra, 8);
        }
    }
    {
        Header151BC104 header;
        void *temp_v0_2;
        Payload151BC104 payload;
        header.unk15 = 0x28;
        header.unk0.x = arg0->x_position;
        header.unk0.y = arg0->y_position;
        header.unk0.z = arg0->z_position;
        header.unkC = 0x12C;
        header.unkE = 6;
        header.unk10 = 0xC;
        payload.owner = arg0;
        payload.ownerId = arg0->unique_id;
        payload.unk8 = header.unk0;
        payload.unk14 = 0.0f;
        payload.unk18 = 0.0f;
        payload.unk20 = -16384.0f;
        payload.unk1C = -16384.0f;
        temp_v0_2 = func_15147A80(&header, 0x28, 0x14, 0xA, 0xA, 0xA, 0, 0, 0, arg1, arg2);
        if (temp_v0_2 != NULL) {
            memcpy(*(void **)((u8 *)temp_v0_2 + 0x98), &payload, 0x24);
        }
    }
    {
        Extra151BC104 extra2;
        void *temp_v0_3;
        Header header2;
        extra2.owner = arg0;
        extra2.ownerId = arg0->unique_id;
        header2.unk0 = 2;
        header2.unk1 = 0xE;
        header2.unk2 = 0x12C;
        header2.unk4 = 0x1F;
        temp_v0_3 = func_15163414(&header2, &arg0->x_position, &arg0->y_position, &arg0->z_position,
                                0, 0, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0, 8, arg1, arg2);
        if (temp_v0_3 != NULL) {
            memcpy((u8 *)temp_v0_3 + 0x28, &extra2, 8);
        }
    }
}
