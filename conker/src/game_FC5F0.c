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
    f32 x;
    f32 y;
} Vec2f;

/* Parameters for func_1515548C, which spawns a 2D particle. */
typedef struct {
    /* 0x00 */ Vec2f pos;
    /* 0x08 */ f32 scaleX;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 flags;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} ParticleSpawn;

/* Particle motion: while flags & 1, hop from startX to startX + distance along an arc. */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 phase; // 0 to 1 over the hop
    /* 0x08 */ f32 startX;
    /* 0x0C */ f32 distance;
} HopMotion;

/* Particle motion: fall under gravity and bounce on a floor line (y grows downwards). */
typedef struct {
    /* 0x00 */ Vec2f vel;
    /* 0x08 */ f32 gravity;
    /* 0x0C */ f32 floorY;
} BounceMotion;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec2f pos;
    /* 0x18 */ u8 pad18[0x70 - 0x18];
    /* 0x70 */ union {
        HopMotion hop;
        BounceMotion bounce;
    } motion;
} Particle;

Particle *func_1515548C(ParticleSpawn *, s32, s32, s32, s32, u8, s32);

/* Spawns the hopping particle, waiting off screen at (-300, 72). */
void func_150CF680(s16 unk12, u8 unkC, s32 unk1) {
    ParticleSpawn spawn;
    HopMotion hop;
    Particle *particle;

    spawn.pos.y = 72.0f;
    spawn.unk10 = 0xC1;
    spawn.unk12 = unk12;
    spawn.flags = 0x31;
    spawn.unk16 = 1;
    spawn.unk18 = 0xFF;
    spawn.unk1A = 8;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk1D = 0xFF;
    spawn.unk1E = 0xFF;
    spawn.unk1F = 0xFF;
    spawn.unk20 = 0xFF;
    spawn.unk21 = 0xFF;
    spawn.unk22 = 0xFF;
    spawn.unk23 = 0xFF;
    spawn.unk28 = 0x200004;

    hop.flags = 0;
    hop.phase = 0.0f;
    spawn.pos.x = -300.0f;
    hop.startX = -300.0f;
    hop.distance = 0.0f;
    spawn.scaleY = 7.5f;
    spawn.scaleX = 7.5f;
    spawn.unk24 = 0;
    spawn.unk2C = 0x9F0601;
    spawn.unk40 = 0;
    spawn.unk41 = 10;
    spawn.unk30 = 0x17;
    spawn.unk34 = 0x44;
    spawn.unk38 = 0x80;
    spawn.unk3C = 0x20;
    spawn.unk4C = 1.0f;
    spawn.unk48 = 1.0f;
    spawn.unk54 = 0.0f;
    spawn.unk50 = 0.0f;
    spawn.unk44 = D_80082FA0;

    particle = func_1515548C(&spawn, 12, 0, 0, 0x10, unkC, unk1);
    if (particle != NULL) {
        memcpy(&particle->motion.hop, &hop, sizeof(hop));
    }
}

extern s8 (*D_8008FCD0[])(void);
void func_15143874(s16 angle, f32 distance, f32 *x, f32 *y);

/* Hopping particle update: follows the hop arc and, on landing, kicks up 2 to 5 bouncing
   dust particles. */
s32 func_150CF800(Particle *hopper) {
    f32 phase;
    ParticleSpawn spawn;
    BounceMotion dust;
    s8 count;
    s32 rand;
    s32 bit2;
    Particle *particle;
    HopMotion *hop;

    hop = &hopper->motion.hop;
    if (hop->flags & 1) {
        hop->phase += 0.19000001f * D_800BE9A4;
        phase = hop->phase;
        if (phase >= 1.0f) {
            hop->phase = 1.0f;
            hop->flags &= ~1;
            count = (func_150ADA20() & 3) + 2;
            dust.floorY = hopper->pos.y + 15.0f;
            spawn.pos = hopper->pos;
            spawn.unk16 = 12;
            spawn.unk18 = 0x15;
            spawn.unk1A = 0;
            spawn.unk1B = 0xFF;
            spawn.unk1C = 0xFF;
            spawn.unk1D = 0xFF;
            spawn.unk1F = 0xFF;
            spawn.unk20 = 0xFF;
            spawn.unk21 = 0xFF;
            spawn.unk22 = 0xFF;
            spawn.unk23 = 0xFF;
            spawn.unk24 = 0;
            spawn.unk28 = 0x200004;
            spawn.unk2C = 0x9F0601;
            spawn.unk40 = 0;
            spawn.unk41 = 10;
            spawn.unk30 = 0x17;
            spawn.unk34 = 0x44;
            spawn.unk38 = 0x80;
            spawn.unk3C = 0x20;
            spawn.unk48 = 1.0f;
            spawn.unk4C = 1.0f;
            spawn.unk54 = 0.0f;
            spawn.unk50 = 0.0f;
            spawn.unk44 = D_80082FA0;
            for (; count > 0; count--) {
                spawn.unk1E = (func_150ADA20() % 101U) + 155;
                spawn.unk10 = D_8008FCD0[2]();
                rand = func_150ADA20();
                phase = (func_150ADA68() * 2) + 2.5f; // matching: the hop phase's register holds the dust speed
                func_15143874((s16)((rand % 65U) - 160), phase, &dust.vel.x, &dust.vel.y);
                dust.gravity = (func_150ADA68() * 0.333f) + 0.404f;
                spawn.scaleX = spawn.scaleY = (func_150ADA68() * 1.79f) + 1.6999999f;
                spawn.unk12 = (func_150ADA20() % 26U) + 30;
                if (func_150ADA20() & 1) {
                    bit2 = 2;
                } else {
                    bit2 = 0;
                }
                if (func_150ADA20() & 1) {
                    rand = 4; // matching: reusing rand's register for this flag bit
                } else {
                    rand = 0;
                }
                spawn.flags = rand | 1 | bit2 | 8 | 0x10 | 0x20; // matching: folding the constants into 0x39 changes the code
                particle = func_1515548C(&spawn, 13, 0, 0, 0x10, hopper->unkC, hopper->unk1);
                if (particle != NULL) {
                    memcpy(&particle->motion.bounce, &dust, sizeof(dust));
                }
            }
            phase = hop->phase;
        }
        hopper->pos.x = hop->startX + (phase * hop->distance);
        hopper->pos.y = (sinf(hop->phase * 3.1415927f) * -17.5f) + 72.0f;
    }
    return 1;
}

/* Hopping particle message handler: message 0x52 starts a hop to the x given in the payload. */
void func_150CFBEC(Particle *hopper, f32 *targetX, u8 type) {
    HopMotion *hop = &hopper->motion.hop;

    if (type == 0x52) {
        hop->flags |= 1;
        hop->phase = 0.0f;
        hop->startX = hopper->pos.x;
        hop->distance = targetX[0] - hopper->pos.x;
    }
}

void func_1515572C(f32 *, s32);

/* Sends message 0x52: hop to targetX. */
void func_150CFC38(f32 targetX) {
    f32 payload = targetX;

    func_1515572C(&payload, 0x52);
}

/* Bouncing particle update: integrates the motion and bounces off the floor, damped. */
s32 func_150CFC60(Particle *particle) {
    BounceMotion *m = &particle->motion.bounce;

    particle->pos.x += m->vel.x * D_800BE9A4;
    particle->pos.y += (m->vel.y * D_800BE9A4) + (((0.5f * m->gravity) * D_800BE9A4) * D_800BE9A4);
    m->vel.y += m->gravity * D_800BE9A4;
    if (m->floorY < particle->pos.y) {
        particle->pos.y = m->floorY;
        m->vel.x *= 0.3f;
        m->vel.y = fabsf(m->vel.y) * -0.15f;
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
