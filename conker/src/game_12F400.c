#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60(Vtx **buffers, s16 index, s32 size, Vtx **out, u8 *isNew);
extern s32 func_15144E80(void *, struct17 *, struct17 *, struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;

typedef struct {
    ObjectWithState197 *field_0x0;
    char pad_0[0x1E];
    u8 field_0x22;
    char pad_1[0x1];
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
} Field124ChildBlock;

typedef struct {
    char pad_0[0x18];
    s32 field_0x18;
    char pad_1[0x8C];
    s32 field_0xA8;
    char pad_2[0x24];
    s32 field_0xD0;
    u8 field_0xD4;
    char pad_3[0x4F];
    s32 field_0x124;
    char pad_4[0x48];
    s32 field_0x170;
    u8 field_0x174;
} Field124Owner;

/* A camera-facing quad: four vertices around `center`, spanned by two axes scaled by scaleU/scaleV. */
typedef struct {
    char pad0[0x2C];
    f32 scaleU;
    f32 scaleV;
    struct17 center;
    char pad40[0x80];
    Vtx templateVtx[4];
    Vtx *vtxBuffers[4];
    u8 flags;
    u8 unk111;
    u8 unk112[0xC];
} Struct15101F50;

/* Fetches vertex buffer `index` (initialised from templateVtx when new) and fills in the quad's corners. */
Vtx *func_15101F50(Struct15101F50 *quad, s16 index) {
    s32 pad; // matching: unused stack slot in the original frame
    Vtx *vtx;
    Vtx *ret;
    struct17 axisU;
    struct17 axisV;
    u8 isNew;

    func_151D5D60(quad->vtxBuffers, index, sizeof(quad->templateVtx), &vtx, &isNew);
    ret = vtx;
    // matching: an early `if (vtx == NULL) return NULL;` lays out the branches differently
    if (vtx != NULL) {
        if (isNew) {
            memcpy(quad->vtxBuffers[index], quad->templateVtx, sizeof(quad->templateVtx));
            memcpy(quad->vtxBuffers[index] + 4, quad->templateVtx, sizeof(quad->templateVtx));
        }
    } else {
        return NULL;
    }

    if (!(quad->flags & 1)) {
        return NULL;
    }

    if (func_15144E80(quad->unk112, &axisU, &axisV, NULL) == 0) {
        return NULL;
    }

    func_15145128(&axisU, &axisU, NULL, NULL);
    func_15145128(&axisV, &axisV, NULL, NULL);

    axisU.unk0 *= quad->scaleU;
    axisU.unk4 *= quad->scaleU;
    axisU.unk8 *= quad->scaleU;
    axisV.unk0 *= quad->scaleV;
    axisV.unk4 *= quad->scaleV;
    axisV.unk8 *= quad->scaleV;

    vtx[0].v.ob[0] = (quad->center.unk0 - axisU.unk0) + axisV.unk0;
    vtx[0].v.ob[1] = (quad->center.unk4 - axisU.unk4) + axisV.unk4;
    vtx[0].v.ob[2] = (quad->center.unk8 - axisU.unk8) + axisV.unk8;
    vtx[0].v.flag = 0;
    vtx[1].v.ob[0] = (quad->center.unk0 + axisU.unk0) + axisV.unk0;
    vtx[1].v.ob[1] = (quad->center.unk4 + axisU.unk4) + axisV.unk4;
    vtx[1].v.ob[2] = (quad->center.unk8 + axisU.unk8) + axisV.unk8;
    vtx[1].v.flag = 0;
    vtx[2].v.ob[0] = (quad->center.unk0 + axisU.unk0) - axisV.unk0;
    vtx[2].v.ob[1] = (quad->center.unk4 + axisU.unk4) - axisV.unk4;
    vtx[2].v.ob[2] = (quad->center.unk8 + axisU.unk8) - axisV.unk8;
    vtx[2].v.flag = 0;
    vtx[3].v.ob[0] = (quad->center.unk0 - axisU.unk0) - axisV.unk0;
    vtx[3].v.ob[1] = (quad->center.unk4 - axisU.unk4) - axisV.unk4;
    vtx[3].v.ob[2] = (quad->center.unk8 - axisU.unk8) - axisV.unk8;
    vtx[3].v.flag = 0;

    return ret;
}

extern f32 D_800BE9A4;

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    u8 pad0[0x2C];
    f32 scaleU;
    f32 scaleV;
    u8 pad34[0x28];
    u8 alpha;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

/* Randomly wandering spin, scale and alpha, pushed out to the linked objects each frame. */
typedef struct {
    u8 pad0[0x24];
    u8 *linked24;
    Sub151022AC_28 *linked28;
    Sub151022AC_2C *linked2C;
    u8 pad30[4];
    Sub151022AC_34 *linked34;
    f32 scale;
    f32 targetScale;
    f32 scaleBlend;
    u8 pad44[0xC];
    f32 angle;
    f32 rate;
    f32 rotation;
    f32 velocity;
    f32 targetVelocity;
    f32 velocityRange;
    f32 timer;
    f32 timerRange;
    f32 velocityBlend;
    f32 alpha;
    f32 targetAlpha;
    f32 alphaBase;
    f32 alphaRange;
    f32 alphaTimer;
    f32 alphaTimerRange;
    f32 alphaBlend;
} Flicker151022AC;

typedef struct {
    u8 pad0[0x5C];
    u8 alpha;
    u8 pad5D[0xB3];
    Flicker151022AC flicker;
} Owner151022AC;

/* Per-frame update: advances the angle and spin, and eases scale and alpha toward random targets. */
s32 func_151022AC(Owner151022AC *obj) {
    Flicker151022AC *state;
    Sub151022AC_28 *quad;
    Sub151022AC_2C *particles;
    Sub151022AC_34 *child;

    state = &obj->flicker;
    state->angle += state->rate * D_800BE9A4;
    state->angle = func_15144B68(state->angle);
    state->rotation += state->velocity * D_800BE9A4;
    while (state->rotation > 32767.0f) {
        state->rotation -= 65536.0f;
    }
    while (state->rotation < -32768.0f) {
        state->rotation += 65536.0f;
    }
    state->timer -= D_800BE9A4;
    if (state->timer < 0.0f) {
        state->timer = func_150ADA68() * state->timerRange;
        state->targetVelocity = func_150ADA68() * (state->velocityRange + state->velocityRange) - state->velocityRange;
    }
    state->velocity += (state->targetVelocity - state->velocity) * state->velocityBlend;
    state->scale += (state->targetScale - state->scale) * state->scaleBlend;
    if (state->linked28 != NULL) {
        quad = state->linked28;
        quad->scaleU = quad->scaleV = (func_150ADA68() * 0.2f + 0.4f) * state->scale;
    }
    state->alphaTimer -= D_800BE9A4;
    if (state->alphaTimer < 0.0f) {
        state->alphaTimer = func_150ADA68() * state->alphaTimerRange;
        state->targetAlpha = func_150ADA68() * state->alphaRange + state->alphaBase;
    }
    state->alpha += (state->targetAlpha - state->alpha) * state->alphaBlend;
    obj->alpha = state->alpha;
    if (state->linked28 != NULL) {
        state->linked28->alpha = obj->alpha;
    }
    if (state->linked24 != NULL) {
        state->linked24[0x2B] = obj->alpha;
    }
    if (state->linked2C != NULL) {
        particles = state->linked2C;
        particles->unk14[0x2F] = (func_150ADA20() & 3) + 2;
    }
    if (state->linked34 != NULL) {
        child = state->linked34;
        child->unk38 = child->unk3C = func_150ADA68() * 10.0f + 120.0f;
    }
    return 1;
}

/* A 2D sine wobble: offset = base + amplitude * sin(phase), with phase advancing at speed. */
typedef struct {
    f32 offset[2];
    f32 base[2];
    f32 amplitude[2];
    f32 phase[2];
    f32 speed[2];
} Wobble151025F4;

typedef struct {
    u8 pad0[0xA8];
    Wobble151025F4 wobble;
} Owner151025F4;

/* Per-frame update of the owner's wobble offset. */
s32 func_151025F4(Owner151025F4 *obj, s32 arg1) {
    Wobble151025F4 *wobble = &obj->wobble;
    s32 pad; // matching: unused stack slot in the original frame
    f32 sinX;
    f32 sinY;

    sinX = sinf(wobble->phase[0]);
    sinY = sinf(wobble->phase[1]);
    wobble->offset[0] = wobble->amplitude[0] * sinX + wobble->base[0];
    wobble->offset[1] = wobble->amplitude[1] * sinY + wobble->base[1];
    wobble->phase[0] += wobble->speed[0] * D_800BE9A4;
    wobble->phase[1] += wobble->speed[1] * D_800BE9A4;
    wobble->phase[0] = func_15144B68(wobble->phase[0]);
    wobble->phase[1] = func_15144B68(wobble->phase[1]);
    return 1;
}

extern f32 D_800A21A0[][3];
extern f32 D_800A21F4[][3];
extern u8 D_800A2248[];
s32 func_15100600(u8 *, f32 *, f32 *, u8, s16, u8, u8, u8, f32, f32, f32, f32, u8, u8, s32);

/* Starts effect `kind` on obj, using the effect tables for its parameters; picks one of two styles by object id. */
s32 func_151026BC(struct127 *obj, s16 arg1, u8 kind, u8 arg3, u8 arg4, s32 arg5) {
    s32 unk5;
    s32 unk6;

    if (obj == NULL) {
        return 0;
    }
    switch (obj->id) {
        case 0x58:
        case 0x5B:
        case 0x80:
            unk5 = 0x32;
            unk6 = 0xFF;
            break;
        case 0x5A:
        case 0x5F:
        case 0x74:
        case 0x75:
        case 0x7A:
            unk5 = 0xFF;
            unk6 = 0;
            break;
        default:
            if (obj->unk31C->unk78 == 0x3B) {
                unk5 = 0xFF;
                unk6 = 0;
            } else {
                unk5 = 0x32;
                unk6 = 0xFF;
            }
            break;
    }
    // matching: the int 20 is converted separately; 20.0f would share one register with the other 20.0f
    return func_15100600((u8 *)obj, D_800A21A0[kind], D_800A21F4[kind], D_800A2248[kind], arg1, unk5, unk6, 0,
                         20.0f, 20, 4.0f, 0.08f, arg3, arg4, arg5);
}

void func_151027E8(struct210 *arg0) {
    s8 sp18[5];

    *(struct210 **)&sp18[0] = arg0;
    sp18[4] = *(u8 *)((s32)arg0 + 0x3B);
    func_151403A8(&sp18, 0x1A);
}

s32 func_1510281C(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0xD4 & 1)) {
        return 0;
    }
    return 1;
}

s32 func_15102884(struct210 *arg0, s32 arg1) {
    if (!(*(u8 *)((s32)arg0 + 0xAC) & 1)) {
        return 0;
    }
    return 1;
}

s32 func_151028AC(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0x174 & 1)) {
        return 0;
    }
    return 1;
}
