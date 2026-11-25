#include <ultra64.h>
// matching: functions.h declares func_150ADA20 as returning u8, but the callers here use its full s32 result
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

/* Header shared by the objects func_15149130 creates; each object type's own data starts at 0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE; // set to -1 to remove the object
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} ObjHeader;

/* Ground probe result filled by func_1504715C. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
} Ground;

/* Data of an object that emits particles around an actor while that actor has bones. */
typedef struct {
    /* 0x00 */ struct127 *actor;
    /* 0x04 */ u8 actorId; // the actor's unique_id when the emitter was attached
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnCredit; // one particle is emitted per whole unit
} ActorEmitterState;

typedef struct {
    /* 0x00 */ ObjHeader header;
    /* 0x28 */ ActorEmitterState state;
} ActorEmitter;

void func_1504715C(Ground *, struct127 *);
void func_15143794(s16 yaw, s16 pitch, f32 distance, f32 *out);
void func_1515C244(struct127 *, f32 *center, f32 *, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32,
                   s32, Ground *, s32, s32, s32, s32);

/* Emitter update: removes itself once its actor is gone or replaced, otherwise emits particles
   from a sphere around the actor's center at a random rate. */
void func_150CF140(ActorEmitter *emitter) {
    ActorEmitterState *state;
    struct127 *actor;
    f32 center[3];
    f32 radius;
    Ground ground;
    f32 extentA;
    f32 extentB;
    f32 pos[3];
    f32 vel[3];
    u8 yaw;

    actor = emitter->state.actor;
    state = &emitter->state;
    if ((actor->interaction_state == 0) || (actor->unique_id != state->actorId)) {
        emitter->header.unkE = -1;
        return;
    }
    if (actor->unk1D4 != NULL) {
        state->spawnCredit += (0.06f + (func_150ADA68() * 0.0704f)) * D_800BE9A4;
        func_1515C244(actor, center, &extentA, &extentB);
        radius = (extentA + extentB) * 0.5f;
        func_1504715C(&ground, actor);
        while (state->spawnCredit > 1.0f) {
            yaw = func_150ADA20() & 0xFF;
            func_15143794(yaw, (s16)((func_150ADA20() % 76U) - 64), radius, pos);
            pos[0] += center[0];
            pos[1] += center[1];
            pos[2] += center[2];
            func_15143794(yaw, (s16)((func_150ADA20() % 54U) - 56), (func_150ADA68() * 1.4430001f) + 2.2680001f, vel);
            func_151D9014(pos, vel, 5, (func_150ADA68() * 0.286f) + -0.78700006f, (func_150ADA20() % 21U) + 30,
                          (func_150ADA20() % 156U) + 100, (func_150ADA68() * 50.0f) + 48.0f, 1, 1.0f, 1.0f, 0,
                          &ground, 1, 0, emitter->header.unkC, emitter->header.unk1);
            state->spawnCredit -= 1.0f;
        }
    }
}

/* Message payload for func_150CF484: message 0 names an actor that went away, message 0x2D two
   actors that swapped places. */
typedef struct {
    /* 0x00 */ struct127 *actorA;
    /* 0x04 */ union {
        struct127 *actorB;
        u8 id;
    } u4;
    /* 0x08 */ u8 actorAId;
    /* 0x09 */ u8 actorBId;
} ActorMsg;

/* Emitter message handler: dies with its actor and follows it through a swap. */
void func_150CF484(ActorEmitter *emitter, ActorMsg *msg, u8 type) {
    ActorEmitterState *state = &emitter->state;
    struct127 *actorA;

    if (type == 0) {
        actorA = msg->actorA;
        if ((actorA == state->actor) || (msg->u4.id == state->actorId)) {
            func_1516972C((struct102 *)emitter);
        }
    } else if (type == 0x2D) {
        if (state->actor == msg->actorA) {
            state->actor = msg->u4.actorB;
            state->actorId = msg->actorBId;
        } else if (state->actor == msg->u4.actorB) {
            state->actor = msg->actorA;
            state->actorId = msg->actorAId;
        }
    }
}

void func_151CF898(struct127 *, f32, f32);

void func_150CF530(s32 arg0) {
    func_151CF898(&D_800CC2D0[0], D_800CC2D0[0].y_position + 300.0f, -10000.0f);
}

/* Texture scroll offsets (10.2 fixed point, in units of 1/32 texel). */
extern s32 D_80088890;
extern s32 D_80088894;
extern s32 D_80088898;
extern s32 D_8008889C;

/* Advances the texture scroll offsets by the frames elapsed. */
void func_150CF578(s32 arg0) {
    D_80088890 += D_800BE9E4 * 26;
    D_80088894 -= D_800BE9E4 * 28;
    D_80088898 -= D_800BE9E4 * 28;
}

/* Sets the scrolled tile sizes of tiles 0 and 1. */
Gfx *func_150CF5E8(Gfx *gfx) {
    gDPSetTileSize(gfx++, 0, D_80088890 >> 7, D_80088894 >> 7, 0x47E, 0x47E);
    gDPSetTileSize(gfx++, 1, D_80088898 >> 7, D_8008889C >> 7, 0x47E, 0x47E);
    return gfx;
}
extern s32 (*D_800888A0[])(struct260 *);

struct struct_150D032C_prefix {
    u8 bytes[5];
};

extern struct struct_150D032C_prefix D_800A0880;
extern f32 D_800A0888;
void func_15042D94(s32, s32, s32, u8 *);
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} struct_150CF680;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[3];
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} struct_150CF680b;

void *func_1515548C(struct_150CF680 *, s32, s32, s32, s32, u8, s32);

void func_150CF680(s16 arg0, u8 arg1, s32 arg2) {
    struct_150CF680 sp40;
    struct_150CF680b sp30;
    void *temp_v0;

    sp40.unk4 = 72.0f;
    sp40.unk10 = 0xC1;
    sp40.unk12 = arg0;
    sp40.unk14 = 0x31;
    sp40.unk16 = 1;
    sp40.unk18 = 0xFF;
    sp40.unk1A = 8;
    sp40.unk1B = 0xFF;
    sp40.unk1C = 0xFF;
    sp40.unk1D = 0xFF;
    sp40.unk1E = 0xFF;
    sp40.unk1F = 0xFF;
    sp40.unk20 = 0xFF;
    sp40.unk21 = 0xFF;
    sp40.unk22 = 0xFF;
    sp40.unk23 = 0xFF;
    sp40.unk28 = 0x200004;

    sp30.unk0 = 0;
    sp30.unk4 = 0.0f;
    sp40.unk0 = -300.0f;
    sp30.unk8 = -300.0f;
    sp30.unkC = 0.0f;
    sp40.unkC = 7.5f;
    sp40.unk8 = 7.5f;
    sp40.unk24 = 0;
    sp40.unk2C = 0x9F0601;
    sp40.unk40 = 0;
    sp40.unk41 = 0xA;
    sp40.unk30 = 0x17;
    sp40.unk34 = 0x44;
    sp40.unk38 = 0x80;
    sp40.unk3C = 0x20;
    sp40.unk4C = 1.0f;
    sp40.unk48 = 1.0f;
    sp40.unk54 = 0.0f;
    sp40.unk50 = 0.0f;
    sp40.unk44 = (u8)D_80082FA0;

    temp_v0 = func_1515548C(&sp40, 0xC, 0, 0, 0x10, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp30, 0x10);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 pad18[0x70 - 0x18];
    /* 0x70 */ struct_150CF680b unk70;
} Obj150CF800;

typedef struct {
    f32 x;
    f32 y;
} Vec2_150CF800;

typedef struct {
    u8 pad[8];
    s8 (*unk8)(void);
} Struct8008FCD0;

extern Struct8008FCD0 D_8008FCD0;
void func_15143874(s16, f32, f32 *, f32 *);

s32 func_150CF800(Obj150CF800 *arg0) {
    f32 t;
    struct_150CF680 sp;
    f32 vel[4];
    s8 count;
    s32 r;
    s32 b1;
    void *obj;
    struct_150CF680b *sub;

    sub = &arg0->unk70;
    if (sub->unk0 & 1) {
        sub->unk4 += 0.1900000125f * D_800BE9A4;
        t = sub->unk4;
        if (t >= 1.0f) {
            sub->unk4 = 1.0f;
            sub->unk0 &= ~1;
            count = (func_150ADA20() & 3) + 2;
            vel[3] = arg0->unk14 + 15.0f;
            *(Vec2_150CF800 *)&sp.unk0 = *(Vec2_150CF800 *)&arg0->unk10;
            sp.unk16 = 0xC;
            sp.unk18 = 0x15;
            sp.unk1A = 0;
            sp.unk1B = 0xFF;
            sp.unk1C = 0xFF;
            sp.unk1D = 0xFF;
            sp.unk1F = 0xFF;
            sp.unk20 = 0xFF;
            sp.unk21 = 0xFF;
            sp.unk22 = 0xFF;
            sp.unk23 = 0xFF;
            sp.unk24 = 0;
            sp.unk28 = 0x200004;
            sp.unk2C = 0x9F0601;
            sp.unk40 = 0;
            sp.unk41 = 0xA;
            sp.unk30 = 0x17;
            sp.unk34 = 0x44;
            sp.unk38 = 0x80;
            sp.unk3C = 0x20;
            sp.unk48 = 1.0f;
            sp.unk4C = 1.0f;
            sp.unk54 = 0.0f;
            sp.unk50 = 0.0f;
            sp.unk44 = (u8)D_80082FA0;
            for (; count > 0; count--) {
                sp.unk1E = (func_150ADA20() % 101U) + 0x9B;
                sp.unk10 = D_8008FCD0.unk8();
                r = func_150ADA20();
                t = (func_150ADA68() * 2) + 2.5f;
                func_15143874((s16)((r % 65U) - 0xA0), t, &vel[0], &vel[1]);
                vel[2] = (func_150ADA68() * 0.3330000043f) + 0.4040000141f;
                sp.unk8 = sp.unkC = (func_150ADA68() * 1.789999962f) + 1.699999928f;
                sp.unk12 = (func_150ADA20() % 26U) + 0x1E;
                if (func_150ADA20() & 1) {
                    b1 = 2;
                } else {
                    b1 = 0;
                }
                if (func_150ADA20() & 1) {
                    r = 4;
                } else {
                    r = 0;
                }
                sp.unk14 = r | 1 | b1 | 8 | 0x10 | 0x20;
                obj = func_1515548C(&sp, 0xD, 0, 0, 0x10, arg0->unkC, arg0->unk1);
                if (obj != NULL) {
                    memcpy((u8 *)obj + 0x70, vel, 0x10);
                }
            }
            t = sub->unk4;
        }
        arg0->unk10 = sub->unk8 + (t * sub->unkC);
        arg0->unk14 = (sinf(sub->unk4 * 3.141592741f) * -17.5f) + 72.0f;
    }
    return 1;
}

struct sub150CFBEC {
    u8 unk0;
    u8 pad[3];
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct par150CFBEC {
    char pad10[0x10];
    f32 unk10;
    char pad14[0x70 - 0x14];
    struct sub150CFBEC unk70;
};

void func_150CFBEC(struct par150CFBEC *arg0, f32 *arg1, u8 arg2) {
    struct sub150CFBEC *temp = &arg0->unk70;

    if (arg2 == 0x52) {
        temp->unk0 |= 1;
        temp->unk4 = 0.0f;
        temp->unk8 = arg0->unk10;
        temp->unkC = arg1[0] - arg0->unk10;
    }
}

extern void func_1515572C(f32 *, s32);

void func_150CFC38(f32 arg0) {
    f32 sp1C;

    sp1C = arg0;
    func_1515572C(&sp1C, 0x52);
}

extern f32 D_800A0878;
extern f32 D_800A087C;

s32 func_150CFC60(f32 *arg0) {
    f32 *p = &arg0[28];
    arg0[4] += p[0] * D_800BE9A4;
    arg0[5] += (p[1] * D_800BE9A4) + (((0.5f * p[2]) * D_800BE9A4) * D_800BE9A4);
    p[1] += p[2] * D_800BE9A4;
    if (p[3] < arg0[5]) {
        arg0[5] = p[3];
        p[0] = p[0] * D_800A0878;
        p[1] = fabsf(p[1]) * D_800A087C;
    }
    return 1;
}

unsigned char *func_150CFD20(unsigned char *arg0) {
    if (*arg0 != 0xBD && *arg0 != 0) {
        do {
            arg0++;
        } while (*arg0 != 0xBD && *arg0 != 0);
    }
    return arg0;
}

unsigned char *func_150CFD5C(unsigned char *arg0) {
    if (*arg0 != 0) {
        do {
            arg0++;
        } while (*arg0 != 0);
    }
    return arg0;
}

extern unsigned char *func_150CFD20(unsigned char *);

s32 func_150CFD84(unsigned char *arg0, unsigned char **arg1) {
    unsigned char *temp;

    temp = func_150CFD20(arg0);
    *arg1 = temp;
    return temp - arg0;
}

s32 func_150CFDB8(unsigned char *arg0) {
    s32 best;
    unsigned char *end;
    unsigned char *sp2C;

    best = 0;
    end = func_150CFD5C(arg0);
    while (arg0 < end) {
        s32 v = func_150CFD84(arg0, &sp2C);
        if (best < v) {
            best = v;
        }
        arg0 = sp2C + 1;
    }
    return best;
}

struct inner150CFE3C {
    char pad0[0x14];
    u8 unk14;
    u8 unk15;
    char pad16[0x18 - 0x16];
    u8 *unk18[1];
};

struct flat150CFE3C {
    char pad0[0x34];
    u8 *unk34;
    char pad38[0x3C - 0x38];
    u8 unk3C;
    u8 unk3D;
    char pad3E[0x40 - 0x3E];
    u8 *unk40[1];
};

union par150CFE3C {
    struct flat150CFE3C flat;
    struct {
        char pad0[0x28];
        struct inner150CFE3C inner;
    } sub;
};

void func_150CFE3C(union par150CFE3C *arg0) {
    struct inner150CFE3C *inner;

    inner = &arg0->sub.inner;
    memcpy(arg0->flat.unk40[arg0->flat.unk3D], arg0->flat.unk34, arg0->flat.unk3C);
    inner->unk18[inner->unk15][inner->unk14] = 0;
}

struct inner150CFE98 {
    char pad0[0x8];
    u8 unk8;
    char pad9[0xC - 0x9];
    u8 *unkC;
    u8 *unk10;
    u8 unk14;
    u8 unk15;
};

void func_150CFE98(arg0)
union par150CFE3C *arg0;
{
    struct inner150CFE98 *inner = (struct inner150CFE98 *)((char *)arg0 + 0x28);

    if (*inner->unk10 != 0) {
        inner->unkC = inner->unk10 + 1;
        inner->unk14 = func_150CFD84(inner->unk10 + 1, &inner->unk10);
        inner->unk15 ^= 1;
        func_150CFE3C(arg0);
        inner->unk8 |= 1;
    }
}

struct frame150CFF10 {
    u8  unk0;
    s32 unk4;
    u8  unk8;
    s32 unkC;
    u8 *unk10;
    u8  unk14;
    u8  unk15;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8  unk24;
    u8  unk25;
};

extern s32 func_150CFDB8(unsigned char *);

struct out150CFF10 {
    char pad0[0x18];
    u8 *unk18;
    u8 *unk1C;
    u8 *unk20;
};

void *func_150CFF10(u8 arg0, s32 arg1, s16 arg2, s32 arg3, s8 arg4, u8 arg5, u8 arg6, void *arg7) {
    struct frame150CFF10 tmp;
    struct260 *temp_v0;
    s32 temp;
    struct out150CFF10 *base;

    tmp.unk25 = arg5;
    tmp.unk24 = arg4;
    tmp.unk15 = 0;
    tmp.unk8 = 1;
    tmp.unk4 = arg1;
    tmp.unkC = arg1;
    tmp.unk0 = arg0;
    tmp.unk14 = func_150CFD84((unsigned char *)arg1, &tmp.unk10);
    tmp.unk18 = 0;
    tmp.unk1C = 0;
    tmp.unk20 = 0;

    temp = func_150CFDB8((unsigned char *)arg1) + 1;

    temp_v0 = func_15149130(arg2, -1, -1, 5, 3, 0x47, (struct37 *)(arg3 + 0x2A + (temp - 1) * 2), arg6, (s32)arg7);
    if (temp_v0 != NULL) {
        base = (struct out150CFF10 *)((u8 *)temp_v0 + 0x28);
        memcpy(base, &tmp, 0x28);
        base->unk20 = (u8 *)base + 0x28;
        base->unk18 = base->unk20 + arg3;
        base->unk1C = base->unk18 + temp;
        *base->unk18 = 0;
        *base->unk1C = 0;
        func_150CFE3C((union par150CFE3C *)temp_v0);
    }
    return temp_v0;
}

s32 func_150D0034(s32 arg0, struct260 *arg1, s32 arg2) {
    u8 *temp_v1;
    s32 ret;

    if (*(volatile s8 *)((u8 *)arg1 + 0x4C) == -1) {
        temp_v1 = (u8 *)arg1 + 0x28;
    } else {
        ret = D_800888A0[*(s8 *)((u8 *)arg1 + 0x4C)](arg1);
        if (ret == 0) {
            arg1->unkE = -1;
            return arg0;
        }
        temp_v1 = (u8 *)arg1 + 0x28;
    }

    temp_v1[8] &= ~1;
    return arg0;
}

extern s32 (*D_800888B0[])(s32 *, s32, u8);

s32 func_150D00C0(s32 *a0, s32 a1, u8 a2) {
    s32 (*fn)(s32 *, s32, u8);

    if (a2 == 0x51) {
        if (*((u8 *)a0 + 0x28) == *((u8 *)a1)) {
            func_150CFE98();
        }
    } else {
        fn = D_800888B0[*((u8 *)a0 + 0x4D)];
        if (fn != 0) {
            fn(a0, a1, a2);
        }
    }
}

extern void *func_150CFF10(u8, s32, s16, s32, s8, u8, u8, void *);

struct frame150D0134 {
    u8 unk0;
    u8 pad[7];
};

void func_150D0134(u8 arg0, s32 arg1, s16 arg2, u8 arg3, void *arg4) {
    struct frame150D0134 sp30;
    void *ret;

    sp30.unk0 = 0;
    ret = func_150CFF10(arg0, arg1, arg2, 8, 0, 0, arg3, arg4);
    if (ret != 0) {
        memcpy(*(void **)((u8 *)ret + 0x48), &sp30.unk0, 1);
    }
}

s32 func_150D01A0(struct260 *arg0) {
    s8 *state;
    u8 alpha;
    u8 fade;
    struct inner150CFE3C *inner;

    state = *(s8 **)((u8 *)arg0 + 0x48);
    inner = (struct inner150CFE3C *)((u8 *)arg0 + 0x28);
    if (((u8 *)arg0)[0x30] & 1) {
        *state = 8;
    }
    if (*state > 0) {
        fade = *state * 31;
        alpha = 0xFF - fade;
        *state -= D_800BE9E4;
    } else {
        alpha = 0xFF;
        fade = 0;
    }
    if (alpha) {
        func_1504332C(0xCE, 0xC4, 0x61, alpha);
        func_15042D94(0x92, 0xBE, 0x81, inner->unk18[inner->unk15]);
    }
    if (fade) {
        func_1504332C(0xCE, 0xC4, 0x61, fade);
        func_15042D94(0x92, 0xBE, 0x81, inner->unk18[inner->unk15 ^ 1]);
    }
    return 1;
}

struct frame150D02B4 {
    f32 unk0;
    s16 unk4;
    u8 pad[2];
};

void func_150D02B4(u8 arg0, s32 arg1, s16 arg2, u8 arg3, void *arg4) {
    void *ret;
    struct frame150D02B4 sp2C;

    sp2C.unk4 = 0;
    sp2C.unk0 = 0.0f;
    ret = func_150CFF10(arg0, arg1, arg2, 8, 1, 0, arg3, arg4);
    if (ret != 0) {
        memcpy(*(void **)((u8 *)ret + 0x48), &sp2C.unk0, 8);
    }
}

struct frame150D032C {
    f32 unk0;
    s16 unk4;
    u8 pad[2];
};

s32 func_150D032C(struct260 *arg0) {
    struct inner150CFE3C *inner;
    struct frame150D032C *state;
    u8 sp28[0x50];
    struct struct_150D032C_prefix sp20;

    inner = (struct inner150CFE3C *)((u8 *)arg0 + 0x28);
    sp20 = D_800A0880;
    state = *(struct frame150D032C **)((u8 *)arg0 + 0x48);

    if (((u8 *)arg0)[0x30] & 1) {
        state->unk4 = 0;
        state->unk0 = 0.0f;
    }

    if (state->unk0 < (f32)inner->unk14) {
        state->unk0 = state->unk0 + (D_800A0888 * D_800BE9A4);
        if (state->unk0 > 1.0f) {
            do {
                state->unk4 = state->unk4 + 1;
                if (inner->unk14 < state->unk4) {
                    state->unk4 = inner->unk14;
                }
                state->unk0 = state->unk0 - 1.0f;
            } while (state->unk0 > 1.0f);
        }
    }

    memcpy(sp28, &sp20, 4);
    memcpy(&sp28[4], inner->unk18[inner->unk15], state->unk4);
    sp28[state->unk4 + 4] = 0x20;
    sp28[state->unk4 + 5] = 0xBB;
    sp28[state->unk4 + 6] = 0;
    func_1504332C(0, 0xFF, 0, 0x96);
    func_15042D94(0xF, 0xBE, 0x80, sp28);
    return 1;
}

void func_150D04C4(u8 arg0, s32 arg1, s16 arg2, u8 arg3, void *arg4) {
    struct frame150D0134 sp30;
    void *ret;

    sp30.unk0 = 0;
    ret = func_150CFF10(arg0, arg1, arg2, 8, 2, 0, arg3, arg4);
    if (ret != 0) {
        memcpy(*(void **)((u8 *)ret + 0x48), &sp30.unk0, 1);
    }
}

s32 func_150D0534(struct260 *arg0) {
    s8 *state;
    u8 alpha;
    u8 fade;
    struct inner150CFE3C *inner;

    state = *(s8 **)((u8 *)arg0 + 0x48);
    inner = (struct inner150CFE3C *)((u8 *)arg0 + 0x28);
    if (((u8 *)arg0)[0x30] & 1) {
        *state = 0x14;
    }
    if (*state > 0) {
        fade = *state * 12;
        alpha = 0xFF - fade;
        *state -= D_800BE9E4;
    } else {
        alpha = 0xFF;
        fade = 0;
    }
    if (alpha) {
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(0x14, 0x14, 0x80, inner->unk18[inner->unk15]);
    }
    if (fade) {
        func_1504332C(0xFF, 0xFF, 0xFF, fade);
        func_15042D94(0x14, 0x14, 0x80, inner->unk18[inner->unk15 ^ 1]);
    }
    return 1;
}
