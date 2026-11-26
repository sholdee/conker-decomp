#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A quad particle (same layout as Particle in game_169510.c), followed by its motion state at 0x110. */
struct Struct15101F50 {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 flags;
    /* 0x111 */ u8 unk111;
    /* 0x112 */ CollisionTri surface;
};

/* Fills the viewport's vertex buffer with a quad centred on pos, half-axes axisU * width and axisV * height. */
Vtx *func_15101F50(Struct15101F50 *particle, s16 viewport) {
    s32 pad; // matching: unused stack slot in the original frame
    Vtx *vtx;
    Vtx *ret;
    Vec3f axisU;
    Vec3f axisV;
    u8 isNew;

    func_151D5D60(particle->head.vtxBuffers, viewport, sizeof(particle->head.vtxTemplate), &vtx, &isNew);
    ret = vtx; // matching: the result is returned from its own stack copy, not by reloading vtx
    // matching: an early `if (vtx == NULL) return NULL;` lays out the branches differently
    if (vtx != NULL) {
        if (isNew) {
            memcpy(particle->head.vtxBuffers[viewport], particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
            memcpy(particle->head.vtxBuffers[viewport] + 4, particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
        }
    } else {
        return NULL;
    }

    if (!(particle->flags & 1)) {
        return NULL;
    }

    if (func_15144E80(&particle->surface, &axisU, &axisV, NULL) == 0) {
        return NULL;
    }

    func_15145128(&axisU, &axisU, NULL, NULL);
    func_15145128(&axisV, &axisV, NULL, NULL);

    axisU.x *= particle->head.def.width;
    axisU.y *= particle->head.def.width;
    axisU.z *= particle->head.def.width;
    axisV.x *= particle->head.def.height;
    axisV.y *= particle->head.def.height;
    axisV.z *= particle->head.def.height;

    vtx[0].v.ob[0] = (particle->head.def.pos.x - axisU.x) + axisV.x;
    vtx[0].v.ob[1] = (particle->head.def.pos.y - axisU.y) + axisV.y;
    vtx[0].v.ob[2] = (particle->head.def.pos.z - axisU.z) + axisV.z;
    vtx[0].v.flag = 0;
    vtx[1].v.ob[0] = (particle->head.def.pos.x + axisU.x) + axisV.x;
    vtx[1].v.ob[1] = (particle->head.def.pos.y + axisU.y) + axisV.y;
    vtx[1].v.ob[2] = (particle->head.def.pos.z + axisU.z) + axisV.z;
    vtx[1].v.flag = 0;
    vtx[2].v.ob[0] = (particle->head.def.pos.x + axisU.x) - axisV.x;
    vtx[2].v.ob[1] = (particle->head.def.pos.y + axisU.y) - axisV.y;
    vtx[2].v.ob[2] = (particle->head.def.pos.z + axisU.z) - axisV.z;
    vtx[2].v.flag = 0;
    vtx[3].v.ob[0] = (particle->head.def.pos.x - axisU.x) - axisV.x;
    vtx[3].v.ob[1] = (particle->head.def.pos.y - axisU.y) - axisV.y;
    vtx[3].v.ob[2] = (particle->head.def.pos.z - axisU.z) - axisV.z;
    vtx[3].v.flag = 0;

    return ret;
}

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    TimedEffect base;
    u8 rgba[4];
} Sub151022AC_24;

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 height;
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
    Sub151022AC_24 *linked24;
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

struct Owner151022AC {
    u8 pad0[0x5C];
    u8 alpha;
    u8 pad5D[0xB3];
    Flicker151022AC flicker;
};

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
        quad->width = quad->height = (func_150ADA68() * 0.2f + 0.4f) * state->scale;
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
        state->linked24->rgba[3] = obj->alpha;
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

struct Owner151025F4 {
    DescParticleHead head;
    Wobble151025F4 wobble;
};

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

extern Vec3f D_800A21A0[];
extern Vec3f D_800A21F4[];
extern u8 D_800A2248[];

/* Starts effect `kind` on obj, using the effect tables for its parameters; picks one of two styles by object id. */
EffectParticle *func_151026BC(Actor *obj, s16 arg1, u8 kind, u8 arg3, u8 arg4, s32 arg5) {
    s32 unk5;
    s32 unk6;

    if (obj == NULL) {
        return NULL;
    }
    switch (obj->character_type) {
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
            if (obj->mv->rt_context_b == 0x3B) {
                unk5 = 0xFF;
                unk6 = 0;
            } else {
                unk5 = 0x32;
                unk6 = 0xFF;
            }
            break;
    }
    // matching: the int 20 is converted separately; 20.0f would share one register with the other 20.0f
    return func_15100600(obj, &D_800A21A0[kind], &D_800A21F4[kind], D_800A2248[kind], arg1, unk5, unk6, 0,
                         20.0f, 20, 4.0f, 0.08f, arg3, arg4, arg5);
}

/* Sends message 0x1A for `actor` (stops the effect started by func_151026BC). */
void func_151027E8(Actor *actor) {
    ActorRef msg;

    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    func_151403A8((s32)&msg, 0x1A);
}


/* State block at +0x110 of the objects referenced by Owner1510281C. */
typedef struct {
    Actor *linked;
    u8 pad4[0x1E];
    u8 unk22;
} Block1510281C;

typedef struct {
    u8 pad0[0x110];
    Block1510281C block;
} Obj1510281C;

struct Owner1510281C {
    u8 pad0[0xD0];
    Obj1510281C *unkD0;
    u8 unkD4;
    u8 padD5[0x9B];
    Obj1510281C *unk170;
    u8 unk174;
};

/* Returns 0 if unkD0's block is keyed to `key` and its linked object is missing or busy, else bit 0 of unkD4. */
s32 func_1510281C(Owner1510281C *owner, s16 key) {
    Block1510281C *block = &owner->unkD0->block;
    Actor *linked;

    if (key == block->unk22) {
        linked = block->linked;
        if (linked == NULL || linked->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(owner->unkD4 & 1)) {
        return 0;
    }
    return 1;
}

struct Obj15102884 {
    u8 pad0[0xAC];
    u8 unkAC;
};

/* Returns bit 0 of obj->unkAC. */
s32 func_15102884(Obj15102884 *obj, s32 arg1) {
    if (!(obj->unkAC & 1)) {
        return 0;
    }
    return 1;
}

/* Same test as func_1510281C, on unk170 and unk174. */
s32 func_151028AC(Owner1510281C *owner, s16 key) {
    Block1510281C *block = &owner->unk170->block;
    Actor *linked;

    if (key == block->unk22) {
        linked = block->linked;
        if (linked == NULL || linked->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(owner->unk174 & 1)) {
        return 0;
    }
    return 1;
}
