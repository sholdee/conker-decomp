#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A1050;
extern f32 D_800A1054;
extern void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);
void func_151C3B0C(s32, f32, f32, f32, f32, s32, s32, s32);
extern s32 func_150AD9A0(s32, s32, s32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* 18 bytes copied verbatim from an effect into each particle it spawns. */
typedef struct {
    s16 unk0[9];
} EffectParams;

/*
 * Object type 0x27: moves from pos toward target (a fixed point, a struct127 or a followed f32[3]) at
 * speed units per frame, spawning particles that run from origin to the moving point. Live effects are
 * registered in the 8 handle slots D_800D99D0 (handle = slot + 1).
 */
typedef struct Effect {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ struct Effect *prev;
    /* 0x08 */ struct Effect *next;
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f target;
    /* 0x28 */ Vec3f origin;
    /* 0x34 */ f32 speed;
    /* 0x38 */ f32 startY;
    /* 0x3C */ struct127 *targetObj;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 lingerTimer;
    /* 0x48 */ u8 slot;
    /* 0x49 */ u8 pad49;
    /* 0x4A */ u16 sound;
    /* 0x4C */ f32 particleAccum;
    /* 0x50 */ f32 sparkAccum;
    /* 0x54 */ f32 groundY;
    /* 0x58 */ EffectParams params;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 unk71;
    /* 0x72 */ u8 pad72[2];
    /* 0x74 */ s32 unk74;
    /* 0x78 */ f32 *follow;
    /* 0x7C */ s16 lifetime; /* frames left, or -99 for unlimited */
} Effect;

#define EFFECT_SLOTS ((Effect **)D_800D99D0)

/* Object lists (see game_1944C0.c); list D_800DCE50[n][0x27] holds the effects. */
extern Effect *D_800DCE50[2][104];
#define OBJ_ITER_NEXT ((Effect **)D_800DD198)

/* Creates an effect from (x, y, z) toward (targetX, targetY, targetZ) and registers it in a free slot. */
Effect *func_150E3020(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, f32 speed,
                      struct127 *targetObj, f32 originX, f32 originY, f32 originZ, f32 *follow, s16 lifetime) {
    Effect *effect;
    s32 i;

    effect = func_15167A68(0x27, 0, 0x80, 1, 0xFF, 1);
    if (effect == NULL) {
        return NULL;
    }
    effect->particleAccum = 0.0f;
    effect->sparkAccum = 0.0f;
    effect->groundY = -10000.0f;
    effect->unk6C = 0;
    effect->unk70 = 0;
    effect->unk71 = 0;
    effect->unk74 = 0;
    effect->lifetime = lifetime;
    effect->follow = follow;
    effect->pos.x = x;
    effect->pos.y = y;
    effect->pos.z = z;
    effect->target.x = targetX;
    effect->target.y = targetY;
    effect->target.z = targetZ;
    effect->origin.x = originX;
    effect->origin.y = originY;
    effect->origin.z = originZ;
    effect->speed = speed;
    effect->startY = y;
    effect->targetObj = targetObj;
    effect->unk40 = arg6;
    effect->lingerTimer = 0;
    effect->sound = 0;
    for (i = 0; i < 8; i++) {
        if (EFFECT_SLOTS[i] == NULL) {
            EFFECT_SLOTS[i] = effect;
            effect->slot = i;
            return effect;
        }
    }
    effect->slot = 0;
    return effect;
}

/* Starts an effect that reaches the target in `frames` frames; returns its handle, or 0. */
s32 func_150E3208(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, s32 frames) {
    Effect *effect;

    effect = func_150E3020(x, y, z, targetX, targetY, targetZ, arg6,
                           (f32)func_150AD9A0(x - targetX, y - targetY, z - targetZ) / (f32)frames,
                           NULL, 0.0f, 0.0f, 0.0f, NULL, -99);
    if (effect != NULL) {
        return effect->slot + 1;
    }
    return 0;
}

/* Starts an effect that homes in on targetObj; returns its handle, or 0. */
s32 func_150E32D0(s32 x, s32 y, s32 z, struct127 *targetObj, s32 arg4, f32 speed) {
    Effect *effect;

    effect = func_150E3020(x, y, z, 0, 0, 0, arg4, speed, targetObj, 0.0f, 0.0f, 0.0f, NULL, -99);
    if (effect != NULL) {
        return effect->slot + 1;
    }
    return 0;
}

/* Starts an effect at `to` that follows the f32[3] at `follow` for `lifetime` frames. */
void func_150E3340(f32 *origin, s32 *to, f32 *follow, s16 lifetime) {
    s32 x;
    s32 y;
    s32 z;

    x = to[0];
    y = to[1];
    z = to[2];
    func_150E3020(x, y, z, x, y, z, 0x1A, 10.0f, NULL, origin[0], origin[1], origin[2], follow, lifetime);
}

void func_1000E7A0(s32, s32);

/* Sound callback of the effect started by func_150E3414. */
s32 func_150E33CC(s32 arg0, s32 arg1, void **arg2, s32 arg3) {
    if (*arg2 == NULL) {
        return 1;
    }
    func_1000E7A0(2, (s32)*arg2);
    return 0;
}

/* Starts an effect with a looping sound (0x2D0) at its origin; returns its handle, or 0. */
s32 func_150E3414(s32 originX, s32 originY, s32 originZ, s32 x, s32 y, s32 z, struct127 *targetObj, s32 arg7,
                  f32 speed) {
    Effect *effect;

    effect = func_150E3020(x, y, z, 0, 0, 0, arg7, speed, targetObj, originX, originY, originZ, NULL, -99);
    if (effect != NULL) {
        effect->sound = func_1000FA64(0x2D0, originX, originY, originZ, 0x5DC0, 0x1770, 0x3E8, (s32)func_150E33CC,
                                      0, 0, 0, 0);
        return effect->slot + 1;
    }
    return 0;
}

/* Destroys an effect: frees its slot, stops its sound (playing 0x2D7 at the origin) and frees the object. */
void func_150E3514(Effect *effect) {
    s32 slot;
    Effect **entry;
    u16 sound;

    slot = effect->slot;
    if (slot >= 0) {
        entry = &EFFECT_SLOTS[slot];
        if (*entry == effect) {
            *entry = NULL;
        }
    }

    sound = effect->sound;
    if (sound != 0) {
        func_100111C8(sound);
        effect->sound = 0;
        func_10010F88(0x2D7, 0x5DC0, 0, 0, 0, effect->origin.x, effect->origin.y, effect->origin.z, 0x3E8, 0x1770);
    }

    func_1516972C((struct102 *)effect);
}

/* Destroys the effect with the given handle, or every effect for handle 0. */
void func_150E35DC(s32 handle) {
    s32 list;
    Effect *effect;

    handle--;
    for (list = 0; list != 2; list++) {
        D_800DD190++;
        effect = D_800DCE50[list][0x27];
        while (effect != NULL) {
            OBJ_ITER_NEXT[D_800DD190] = effect->next;
            if (handle == -1 || handle == effect->slot) {
                func_150E3514(effect);
            }
            effect = OBJ_ITER_NEXT[D_800DD190];
        }
        {
            extern s8 D_800DD190;

            D_800DD190--;
        }
    }
}

/* Reads the position of the effect with the given handle, truncated to integers. */
void func_150E36BC(s32 handle, s32 *x, s32 *y, s32 *z) {
    Effect *effect;

    handle -= 1;
    if ((handle >= 0) && (handle < 8)) {
        effect = EFFECT_SLOTS[handle];
        if (effect != NULL) {
            if (effect->type == 0x27) {
                *x = effect->pos.x;
                *y = effect->pos.y;
                *z = effect->pos.z;
            }
        }
    }
}

/* Spawn description handed to func_151C229C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ Vec3f unk8;
    /* 0x14 */ Vec3f unk14;
    /* 0x20 */ Vec3f unk20;
    /* 0x2C */ Vec3f unk2C;
    /* 0x38 */ Vec3f unk38;
    /* 0x44 */ EffectParams params;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 unk5A;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
} ParticleDesc;

extern s32 func_15046C80(Vec3f *, s32, f32, f32 *);
extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern s32 func_151C229C(Vec3f *, void *, s32, s32, s32, ParticleDesc *, f32, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32);
extern void func_15103254(s16, u8, f32, Vec3f *, s32, s32, s32);

void func_150E3738(Effect *arg0) {
    Effect *effect;
    Vec3f start;
    Vec3f next;
    Vec3f delta;
    Vec3f step;
    f32 dist;
    f32 move;
    f32 scale;
    s32 spawned;
    Vec3f p;
    Vec3f hit;
    f32 pad178;
    Vec3f pos;
    Vec3f dir;
    f32 len;
    f32 pad158;
    f32 offz;
    f32 offx;
    f32 unused;
    f32 r2;
    s32 c;
    ParticleDesc particle;
    f32 r;
    s32 ang;

    effect = arg0;
    spawned = 0;
    if (effect->lifetime != -99) {
        effect->lifetime -= D_800BE9E4;
        if (effect->lifetime < 0) {
            func_150E3514(effect);
        }
    }
    start = effect->pos;
    if (effect->targetObj != NULL) {
        if (effect->targetObj->interaction_state == 0) {
            func_150E3514(effect);
            return;
        }
        effect->target.x = effect->targetObj->x_position;
        effect->target.y = effect->targetObj->y_position;
        effect->target.z = effect->targetObj->z_position;
    } else if (effect->follow != NULL) {
        effect->target.x = effect->follow[0];
        effect->target.y = effect->follow[1];
        effect->target.z = effect->follow[2];
    }
    delta.x = effect->target.x - effect->pos.x;
    delta.y = effect->target.y - effect->pos.y;
    delta.z = effect->target.z - effect->pos.z;
    dist = sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    move = effect->speed * D_800BE9E4;
    if (effect->follow == NULL) {
        if (effect->lingerTimer == 0) {
            if (dist <= move) {
                if (effect->targetObj != NULL) {
                    effect->lingerTimer = 30;
                    effect->targetObj = NULL;
                    effect->target.x += delta.x * 500.0f;
                    effect->target.y += delta.y * 500.0f;
                    effect->target.z += delta.z * 500.0f;
                } else {
                    func_150E3514(effect);
                    return;
                }
            }
        } else {
            effect->lingerTimer -= D_800BE9E4;
            if (effect->lingerTimer <= 0) {
                func_150E3514(effect);
                return;
            }
        }
    }
    if (dist != 0.0f) {
        extern f32 D_800BE9A4;

        scale = move / dist;
        step.x = scale * delta.x;
        step.y = scale * delta.y;
        step.z = scale * delta.z;
        next.x = start.x + step.x;
        next.y = start.y + step.y;
        next.z = start.z + step.z;
    } else {
        next = effect->target;
        step = delta;
    }
    effect->pos = next;
    effect->particleAccum += (526.0f + func_150ADA68() * 1228.0f) * 0.001000000047f * D_800BE9A4;
    while (effect->particleAccum > 1.0f) {
        pos = effect->origin;
        {
            f32 rnd;

            rnd = func_150ADA68();
            ang = func_150ADA20();
            func_15143874(ang & 0xFF, func_150ADA68() * 129.0f, &offx, &offz);
            p.x = rnd * step.x + start.x + offx;
            p.y = rnd * step.y + start.y + 200.0f;
            p.z = rnd * step.z + start.z + offz;
        }
        hit = p;
        if (func_15046C80(&p, 0, p.y - 1000.0f, &effect->groundY) != 0) {
            hit.y = effect->groundY;
        }
        {
            extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);

            dir.x = hit.x - pos.x;
            dir.y = hit.y - pos.y;
            dir.z = hit.z - pos.z;
        }
        if (func_15145128(&dir, &dir, &len, &unused) != 0) {
            move = func_150ADA68() * D_800BE9A4;
            scale = 190.0f * move;
            pos.x += dir.x * scale;
            pos.y += dir.y * scale;
            pos.z += dir.z * scale;
            len -= scale;
        }
        particle.unk0 = 0;
        particle.unk4 = len;
        particle.unk8 = hit;
        particle.unk14 = hit;
        particle.unk20 = hit;
        particle.unk2C = pos;
        particle.unk38 = dir;
        particle.params = effect->params;
        particle.unk56 = 0;
        particle.unk58 = 0;
        particle.unk59 = 1;
        particle.unk5A = 0;
        if (effect->unk71 == 2 || effect->unk71 == 3) {
            particle.unk5C = effect->unk74;
        } else {
            particle.unk5C = 0;
        }
        particle.unk60 = effect->unk6C;
        r2 = func_150ADA68();
        scale = func_150ADA68();
        c = func_150ADA20();
        if (spawned != 0 || effect->follow != NULL) {
            ang = 2;
        } else {
            ang = 1;
        }
        if (func_151C229C(&pos, NULL, 0, 0, 0, &particle, 190.0f, 0.00526315812f, r2 * 50.0f + 10.0f,
                          scale * 191.0f + 160.0f, 80.0f, (c % 176U) + 80, 0, 0, 0, 0, 0, 0, 1, ang,
                          effect->unk40, 0.0f, 0xFF, -1, 0, effect->owner, effect->list) != 0) {
            spawned = 1;
        }
        effect->particleAccum -= 1.0f;
    }
    if (effect->follow == NULL) {
        effect->sparkAccum += (56.0f + func_150ADA68() * 235.0f) * 0.001000000047f * D_800BE9A4;
        while (effect->sparkAccum > 1.0f) {
            c = func_150ADA20();
            ang = func_150ADA20();
            func_15103254((c & 3) + 6, (ang % 156U) + 100,
                          func_150ADA68() * 1548.0f + 1093.0f, &effect->origin, 0xFF, effect->owner, effect->list);
            effect->sparkAccum -= 1.0f;
        }
    }
}

/* The part of an object that func_150E4010 reads, at +0x110 of it. */
typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ f32 x;
    /* 0x34 */ f32 y;
    /* 0x38 */ f32 z;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ u8 pad68[0x3C];
    /* 0xA4 */ s32 unkA4;
} Obj4010State;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ Obj4010State state;
} Obj4010;

void func_150E4010(Obj4010 *obj) {
    Obj4010State *state;

    state = &obj->state;
    if (obj->state.unkA4 != 0) {
        func_1505D1C4(state->x, state->y, state->z, state->unkA4 | 0x60000, -1,
                      ((u16)(u32)(func_150484A0(state->unk60, state->unk64) * D_800A1050) - 0x4000) | 1, 0, 0);
    }
}

void func_150E411C(s32 arg0) {
    func_151C3B0C(arg0, 0.352000028f, 0.701000035f, 0.566000044f, D_800A1054, 0xFF, 0xFF, 0xFF);
}

extern f32 D_800BE9A4;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x18];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ f32 unk54;
} Obj4174;

/* Advances unk2C and unk30 by their per-frame rates unk4C and unk54. */
s32 func_150E4174(Obj4174 *obj) {
    obj->unk2C += obj->unk4C * D_800BE9A4;
    obj->unk30 += obj->unk54 * D_800BE9A4;
    return 1;
}
