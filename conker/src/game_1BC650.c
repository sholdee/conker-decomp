#include <ultra64.h>
// functions.h declares these pointer parameters as s32; this file uses the real pointer types.
#define func_15169260 func_15169260_hdr
#define func_15169850 func_15169850_hdr
#include "functions.h"
#undef func_15169260
#undef func_15169850
#include "variables.h"

/* Per-type state of a sprite effect, at sprite + 0x110. */
typedef struct {
    /* 0x00 */ f32 hi0;
    /* 0x04 */ f32 lo0;
    /* 0x08 */ f32 target0;
    /* 0x0C */ f32 hi1;
    /* 0x10 */ f32 lo1;
    /* 0x14 */ f32 peak1;
    /* 0x18 */ f32 target1;
    /* 0x1C */ s16 timer0;
    /* 0x1E */ s16 timer1;
    /* 0x20 */ f32 hi2;
    /* 0x24 */ f32 lo2;
    /* 0x28 */ f32 target2;
    /* 0x2C */ s16 timer2;
} SpriteWobble;

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ struct17 velocity;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
} SpriteMotion;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[6];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[4];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ struct17 target;
    /* 0x040 */ u8 pad40[0x18];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0x10];
    /* 0x06C */ s16 unk6C;
    /* 0x06E */ s16 unk6E;
    /* 0x070 */ u8 unk70;
    /* 0x071 */ u8 unk71;
    /* 0x072 */ u8 pad72[0x9E];
    /* 0x110 */ union {
        SpriteWobble wobble;
        SpriteMotion motion;
    } data;
} EffectSprite;

/* 0x50-byte emitter template (D_8008D630), copied into EmitterParams after `accum`. */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ f32 burstBase;
    /* 0x0C */ f32 burstRange;
    /* 0x10 */ f32 sizeX;
    /* 0x14 */ f32 sizeY;
    /* 0x18 */ f32 sizeXRange;
    /* 0x1C */ f32 sizeYRange;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21Range;
    /* 0x22 */ s16 lifeMin;
    /* 0x24 */ s16 lifeRange;
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ s32 maxCount;
    /* 0x2C */ f32 velocityScale;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34Range;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42Range;
    /* 0x44 */ s32 anchorSet;
    /* 0x48 */ u8 flags;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 unk4C;
} EmitterTemplate; /* 0x50 */

typedef struct {
    /* 0x00 */ f32 accum;
    /* 0x04 */ EmitterTemplate tmpl;
    /* 0x54 */ struct225 *child;
    /* 0x58 */ s8 updateFunc;
    /* 0x59 */ s8 unk59Func;
    /* 0x5A */ u8 unk5A;
    /* 0x5B */ u8 pad5B;
} EmitterParams; /* 0x5C */

/* Object returned by func_151D2F00. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ struct127 *owner;
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ s16 count;
    /* 0x22 */ u8 pad22[0xE];
    /* 0x30 */ EmitterParams params;
} Emitter;

typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 duration;
    /* 0x08 */ s8 hasDuration;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 unkD;
    /* 0x0E */ u8 padE[2];
} EmitterDesc;

/* One entry of a bone anchor set (D_800E0990). */
typedef struct {
    /* 0x00 */ u8 bone;
    /* 0x04 */ struct17 offset;
} BoneAnchor; /* 0x10 */

typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ struct17 offset;
    /* 0x14 */ u8 bone;
    /* 0x16 */ s16 duration;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1C */ void *sprite;
    /* 0x20 */ Emitter *parent;
} EmitterParticleDesc;

/* Particle created by func_151D2BA4 from an EmitterParticleDesc. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ EffectSprite *sprite;
    /* 0x30 */ Emitter *parent;
    /* 0x34 */ struct17 pos;
} EmitterParticle;

typedef struct {
    f32 x;
    f32 y;
} SpriteSize;

typedef struct {
    s32 unk0;
    u8 unk4;
} ContactKey;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ ContactKey key;
} ContactObj;

typedef struct {
    s32 unk0;
} SoundDesc;

typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
} ChildOwner;

typedef struct {
    /* 0x00 */ s32 emitter;
    /* 0x04 */ f32 params[8];
    /* 0x24 */ ChildOwner owner;
    /* 0x2C */ Header header;
    /* 0x34 */ s32 pad34;
} ChildDesc;

void func_15169260(SoundDesc *, s32, s32, u8);
void func_15169850(ContactKey *, u8, s32 *, u8 *, ContactObj *);
void *memcpy(void *, const void *, u32);
s32 func_1518E298();
void func_15143134(struct17 *offset, struct17 *out, Mtx *mtx);
void *func_1514B8E4(struct17 *, SpriteSize *, s16, u8, struct17 *, f32, f32, f32, u8, u8, u8, s32, u8, s16,
                    s16, s32, u8, s32);
void func_1514BE20(EffectSprite *);
void func_1514BF50(EffectSprite *);
struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
void func_151D2AB0(s32);
void func_151D2BA4(EmitterParticleDesc *, struct17 *, s32, u8, s32);
Emitter *func_151D2F00(EmitterDesc *, s32, u8, s32);

s32 func_1518FDC4(struct127 *obj, u8 *kind, u8 variant);
s32 func_1518F5D0(struct127 *owner, EmitterTemplate *tmpl, s16 duration, s8 updateFunc, s8 unk59Func,
                  s8 childKind, u8 unk5A, s32 arg7, u8 arg8, s32 arg9);
void func_1518F8E0(Emitter *emitter);
struct225 *func_1519021C(s32 emitter, struct127 *owner, u8 kind, s16 duration, u8 arg4, s32 arg5);

extern EmitterTemplate *D_8008D630[];
extern s32 (*D_8008D67C[])(Emitter *);
extern void (*D_8008D680[])(void);

/* Moves three sprite values toward random targets that are re-rolled on timers. */
s32 func_1518F1A0(EffectSprite *sprite) {
    SpriteWobble *state = &sprite->data.wobble;

    state->timer1 -= D_800BE9E4;
    if (state->timer1 < 0) {
        state->timer1 = func_150ADA20() % 6U;
        if (func_150ADA20() & 3) {
            state->target1 = func_150ADA68() * (state->hi1 - state->lo1) + state->lo1;
        } else {
            state->target1 = func_150ADA68() * (state->peak1 - state->hi1) + state->hi1;
        }
    }
    sprite->unk30 += (state->target1 - sprite->unk30) * 0.359f;
    state->timer0 -= D_800BE9E4;
    if (state->timer0 < 0) {
        state->timer0 = func_150ADA20() % 17U;
        state->target0 = func_150ADA68() * (state->hi0 - state->lo0) + state->lo0;
    }
    sprite->unk2C += (state->target0 - sprite->unk2C) * 0.186f;
    state->timer2 -= D_800BE9E4;
    if (state->timer2 < 0) {
        state->timer2 = func_150ADA20() % 15U;
        state->target2 = func_150ADA68() * (state->hi2 - state->lo2) + state->lo2;
    }
    sprite->unk24 += (s32)((state->target2 - sprite->unk24) * 0.231f);
    return 1;
}

/* Like func_1518F1A0, but each target is rolled only once. */
s32 func_1518F384(EffectSprite *sprite) {
    SpriteWobble *state = &sprite->data.wobble;

    if (sprite->data.wobble.timer0 == 0) {
        state->target0 = func_150ADA68() * (state->hi0 - state->lo0) + state->lo0;
        state->timer0 = 1;
    }
    if (state->timer1 == 0) {
        state->target1 = func_150ADA68() * (state->hi1 - state->lo1) + state->lo1;
        state->timer1 = 1;
    }
    sprite->unk30 += (state->target1 - sprite->unk30) * 0.359f;
    sprite->unk2C += (state->target0 - sprite->unk2C) * 0.186f;
    return 1;
}

void func_1518F45C(s32 arg0, u8 arg1) {
    SoundDesc desc = { 0x21 };

    func_15169260(&desc, 1, arg0, arg1);
}

void func_1518F49C(ContactObj *self, ContactKey *other, u8 arg2) {
    func_15169850(other, arg2, &self->key.unk0, &self->key.unk4, self);
    if (arg2 == 0x49) {
        s32 selfId = self->key.unk0;
        s32 otherId = other->unk0;
        u8 selfTag = self->key.unk4;
        u8 otherTag = other->unk4;
        if (selfId == otherId || selfTag == otherTag) {
            func_1516972C((struct102 *)self);
        }
    }
}

/* Creates emitter `index` of the kind matching `owner`'s object id. */
s32 func_1518F51C(struct127 *owner, u8 index, s16 duration, s8 updateFunc, s8 unk59Func, s8 childKind,
                  u8 unk5A, s32 arg7, u8 arg8, s32 arg9) {
    u8 pad[4]; // matching: unused stack slot
    u8 kind;

    if (owner == NULL) {
        return 0;
    }
    if (func_1518FDC4(owner, &kind, index) == 0) {
        return 0;
    }
    return func_1518F5D0(owner, &D_8008D630[kind][index], duration, updateFunc, unk59Func,
                         childKind, unk5A, arg7, arg8, arg9);
}

/* Creates an emitter attached to `owner` from `tmpl`; duration -1 means none (uses 300). */
s32 func_1518F5D0(struct127 *owner, EmitterTemplate *tmpl, s16 duration, s8 updateFunc, s8 unk59Func,
                  s8 childKind, u8 unk5A, s32 arg7, u8 arg8, s32 arg9) {
    EmitterDesc desc;
    u8 pad[4]; // matching: unused stack slot
    EmitterParams params;
    EmitterParams *dest;
    Emitter *emitter;
    s8 hasDuration;

    params.accum = 0.0f;
    params.tmpl = *tmpl;
    params.updateFunc = updateFunc;
    desc.owner = owner;
    params.unk59Func = unk59Func;
    params.unk5A = unk5A;
    desc.ownerId = owner->unique_id;
    if (duration == -1) {
        desc.duration = 300;
    } else {
        desc.duration = duration;
    }
    hasDuration = (duration == -1) ? 0 : 1;
    desc.hasDuration = hasDuration;
    desc.unk9 = 0;
    desc.unkA = 0;
    desc.unkB = 0;
    desc.unkC = 0;
    desc.unkD = 1;

    emitter = func_151D2F00(&desc, arg7 + 0x60, arg8, arg9);
    if (emitter != NULL) {
        dest = &emitter->params;
        memcpy(dest, &params, sizeof(EmitterParams));
        if (childKind != -1) {
            dest->child = func_1519021C((s32)emitter, owner, childKind, duration, arg8, arg9);
        } else {
            dest->child = NULL;
        }
        func_151D2AB0(dest->tmpl.anchorSet);
        func_1000FA64(0x4D, owner->x_position, owner->y_position, owner->z_position, 15000, 2000, 800,
                      (s32)func_1518E298, emitter, (s32)owner, 0, 0);
    }
    return (s32)emitter;
}

/* Accumulates the emission rate, spawns particles, then runs the emitter's update callback. */
s32 func_1518F7C4(Emitter *emitter) {
    EmitterParams *params;

    params = &emitter->params;
    params->accum += (params->tmpl.rate + func_150ADA68() * params->tmpl.rateRange) * D_800BE9A4;
    func_1518F8E0(emitter);
    if (params->updateFunc != -1) {
        return D_8008D67C[params->updateFunc](emitter);
    }
    return 1;
}

void func_1518F858(Emitter *emitter) {
    EmitterParams *params = &emitter->params;

    if (emitter->params.unk59Func != -1) {
        D_8008D680[params->unk59Func]();
    }
}

/* Emits a random-sized burst of particles. */
void func_1518F89C(Emitter *emitter) {
    EmitterParams *params;

    params = &emitter->params;
    params->accum = func_150ADA68() * params->tmpl.burstRange + params->tmpl.burstBase;
    func_1518F8E0(emitter);
}

/* Spawns one particle at a random bone anchor of the owner per whole unit in `accum`. */
void func_1518F8E0(Emitter *emitter) {
    EmitterParams *params;
    struct127 *owner;
    SpriteSize size;
    struct17 pos;
    BoneAnchor *anchor;
    s32 set;
    s32 kind;
    EmitterParticleDesc desc;
    s32 frameRand;
    void *sprite;

    owner = emitter->owner;
    params = &emitter->params;
    if (params->accum > 1.0f) {
        if ((owner->unk1D4 != NULL) && ((owner->unk74 & 0xF) != 0xF)) {
            do {
                if (emitter->count < params->tmpl.maxCount) {
                    set = params->tmpl.anchorSet;
                    anchor = &((BoneAnchor *)D_800E0990[set])[(u32)func_150ADA20() % ((u16 *)D_800E0968)[set]];
                    size.x = func_150ADA68() * params->tmpl.sizeXRange + params->tmpl.sizeX;
                    size.y = func_150ADA68() * params->tmpl.sizeYRange + params->tmpl.sizeY;
                    func_15143134(&anchor->offset, &pos, (Mtx *)owner->unk1D4 + anchor->bone);
                    frameRand = func_150ADA20();
                    if (func_150ADA20() & 1) {
                        kind = 0x13;
                    } else {
                        kind = 0x14;
                    }
                    sprite = func_1514B8E4(&pos, &size, -1,
                                           (u32)frameRand % (params->tmpl.unk21Range + 1) + params->tmpl.unk20,
                                           (struct17 *)&D_800A5480, 0.0f, 1.0f, params->tmpl.unk4C, 0, 0x23,
                                           0, kind, 1, 1, 0xFF, 0, emitter->unkC, emitter->unk1);
                    if (sprite != NULL) {
                        desc.owner = owner;
                        desc.ownerId = owner->unique_id;
                        desc.offset = anchor->offset;
                        desc.bone = anchor->bone;
                        desc.duration = func_150ADA20() % (u32)(params->tmpl.lifeRange + 1) + params->tmpl.lifeMin;
                        desc.unk18 = 1;
                        if (params->tmpl.flags & 1) {
                            desc.unk19 = 1;
                        } else {
                            desc.unk19 = 0;
                        }
                        desc.unk1A = 0;
                        desc.parent = emitter;
                        desc.sprite = sprite;
                        func_151D2BA4(&desc, &pos, 0, emitter->unkC, emitter->unk1);
                    }
                }
                params->accum -= 1.0f;
            } while (params->accum > 1.0f);
        } else {
            do {
                params->accum -= 1.0f;
            } while (params->accum > 1.0f);
        }
    }
}

s32 func_1518FC04(EmitterParticle *particle, struct17 *target) {
    EffectSprite *sprite;

    sprite = particle->sprite;
    sprite->target = *target;
    func_1514BE20(sprite);
    return 1;
}

s32 func_1518FC44(EmitterParticle *particle, struct17 *target) {
    EffectSprite *sprite;

    sprite = particle->sprite;
    sprite->target = *target;
    func_1514BF50(sprite);
    return 1;
}

/* Launches the particle's sprite from its position toward `dest`, or frees it if it has no emitter. */
void func_1518FC84(EmitterParticle *particle, struct17 *dest) {
    EffectSprite *sprite;
    SpriteMotion *motion;
    EmitterParams *params;
    Emitter *parent;

    sprite = particle->sprite;
    if (particle->parent == NULL) {
        func_1516972C((struct102 *)sprite);
        return;
    }
    parent = particle->parent;
    sprite->data.motion.velocity.unk0 = (dest->unk0 - particle->pos.unk0) * parent->params.tmpl.velocityScale;
    sprite->data.motion.velocity.unk4 = (dest->unk4 - particle->pos.unk4) * parent->params.tmpl.velocityScale;
    sprite->data.motion.velocity.unk8 = (dest->unk8 - particle->pos.unk8) * parent->params.tmpl.velocityScale;
    params = &parent->params;
    motion = &sprite->data.motion;
    motion->unk44 = func_150ADA68() * params->tmpl.unk34Range + params->tmpl.unk30;
    motion->unk48 = params->tmpl.unk38;
    sprite->unk6C = params->tmpl.unk3C;
    sprite->unk6E = params->tmpl.unk3E;
    sprite->unk1C = func_150ADA20() % (u32)(params->tmpl.unk42Range + 1) + params->tmpl.unk40;
    if (params->tmpl.flags & 1) {
        sprite->unk70 = 0x21;
    } else {
        sprite->unk70 = 0x20;
    }
    sprite->unk71 = 0x24;
    sprite->flags |= 0x08000001;
}

/* Maps an object id to its emitter kind; only variant 0 exists. Returns 0 for unknown ids. */
s32 func_1518FDC4(struct127 *obj, u8 *kind, u8 variant) {
    switch (obj->id) {
    case 0x87:
    case 0xB0:
        *kind = 13;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xB1:
        *kind = 14;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xB4:
        *kind = 15;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x70:
    case 0xB2:
        *kind = 16;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x53:
        *kind = 17;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x96:
        *kind = 18;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
        *kind = 0;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x49:
        *kind = 1;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x58:
        *kind = 2;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x50:
        *kind = 3;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x5A:
    case 0x5F:
    case 0x74:
    case 0x75:
    case 0x7A:
        *kind = 4;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x80:
        *kind = 5;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        *kind = 6;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x16:
        *kind = 7;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x88:
    case 0x90:
        *kind = 8;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9C:
        *kind = 9;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9D:
        *kind = 10;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9F:
        *kind = 11;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xA0:
        *kind = 12;
        if (variant > 0) {
            return 0;
        }
        return 1;
    default:
        return 0;
    }
}

extern u8 D_800A7AC8;
extern u8 D_800A7ACC;
extern u8 D_800A7AD0;
extern u8 D_800A7AD4;
extern u8 D_800A7AD8;
extern u8 D_800A7ADC;
extern u8 D_800A7AE0;
extern u8 D_800A7AE4;
extern u8 D_800A7AE8;
extern u8 D_800A7AEC;
extern u8 D_800A7AF0;
extern u8 D_800A7AF4;
extern u8 D_800A7AF8;
extern u8 D_800A7AFC;
extern u8 D_800A7B00;
extern u8 D_800A7B04;
extern u8 D_800A7B08;
extern u8 D_800A7B0C;
extern u8 D_800A7B10;

/* Returns the per-object-id data block (all empty in this build), setting *found. */
u8 *func_1519003C(struct127 *obj, s32 *found) {
    u8 *data;

    switch (obj->id) {
    case 0x87:
    case 0xB0:
        data = &D_800A7AFC;
        *found = 1;
        break;
    case 0xB1:
        data = &D_800A7B00;
        *found = 1;
        break;
    case 0xB4:
        data = &D_800A7B04;
        *found = 1;
        break;
    case 0x70:
    case 0xB2:
        data = &D_800A7B08;
        *found = 1;
        break;
    case 0x53:
        data = &D_800A7B0C;
        *found = 1;
        break;
    case 0x96:
        data = &D_800A7B10;
        *found = 1;
        break;
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
        data = &D_800A7AC8;
        *found = 1;
        break;
    case 0x49:
        data = &D_800A7ACC;
        *found = 1;
        break;
    case 0x58:
        data = &D_800A7AD0;
        *found = 1;
        break;
    case 0x50:
        data = &D_800A7AD4;
        *found = 1;
        break;
    case 0x75:
        data = &D_800A7AD8;
        *found = 1;
        break;
    case 0x80:
        data = &D_800A7ADC;
        *found = 1;
        break;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        data = &D_800A7AE0;
        *found = 1;
        break;
    case 0x16:
        data = &D_800A7AE4;
        *found = 1;
        break;
    case 0x88:
    case 0x90:
        data = &D_800A7AE8;
        *found = 1;
        break;
    case 0x9C:
        data = &D_800A7AEC;
        *found = 1;
        break;
    case 0x9D:
        data = &D_800A7AF0;
        *found = 1;
        break;
    case 0x9F:
        data = &D_800A7AF4;
        *found = 1;
        break;
    case 0xA0:
        data = &D_800A7AF8;
        *found = 1;
        break;
    default:
        data = NULL;
        *found = 0;
        break;
    }
    return data;
}

/* Creates the child sprite of an emitter, colored from the D_800A67C0/6760/6820 tables. */
struct225 *func_1519021C(s32 emitter, struct127 *owner, u8 kind, s16 duration, u8 arg4, s32 arg5) {
    ChildDesc desc;
    Header2 pos;
    struct225 *child;

    if (kind >= 24) {
        return NULL;
    }

    desc.params[0] = D_800A67C0[kind];
    desc.params[1] = D_800A6760[kind];
    desc.params[2] = D_800A6820[kind];
    desc.params[3] = 1.0f;
    desc.params[5] = 5.0f;
    desc.params[4] = 0.0f;
    desc.params[7] = 0.0f;
    desc.params[6] = 0.2489f;
    desc.emitter = emitter;

    desc.header.unk0 = ((duration == -1) ? 0 : 1) | 2;
    desc.header.unk1 = 0x12;
    if (duration == -1) {
        desc.header.unk2 = 300;
    } else {
        desc.header.unk2 = duration;
    }
    desc.header.unk4 = 0x25;

    desc.owner.owner = owner;
    desc.owner.ownerId = owner->unique_id;

    pos.unk0 = owner->x_position;
    pos.unk4 = owner->y_position;
    pos.unk8 = owner->z_position;

    child = func_151602C0(&desc.header, &pos, 0, 0xFF, 0xD1, 0, 0xFF, 0, 0x30, arg4, arg5);
    if (child != NULL) {
        memcpy((u8 *)child + 0x18, &desc.owner, sizeof(desc.owner));
        memcpy((u8 *)child + 0x20, desc.params, sizeof(desc.params));
        memcpy((u8 *)child + 0x40, &desc.emitter, sizeof(desc.emitter));
    }
    return child;
}
