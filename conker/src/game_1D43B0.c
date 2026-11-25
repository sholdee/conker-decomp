#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value; the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

/* Debris emitter settings read by func_151A6F00. Angles are 0..255 binary angles. */
typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ vertex pos;
    /* 0x10 */ s16 lifetime;
    /* 0x14 */ f32 radius;     /* spawn points lie within this distance of pos */
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 speedBase;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 velYBase;   /* mode 0 only */
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ vertex dir;     /* modes 2 and 3 */
    /* 0x3C */ f32 sizeBase;
    /* 0x40 */ f32 sizeRange;
    /* 0x44 */ f32 gravityBase;
    /* 0x48 */ f32 gravityRange;
    /* 0x4C */ f32 spinBase;
    /* 0x50 */ f32 spinRange;
    /* 0x54 */ u8 landPuffs;   /* see DebrisData */
    /* 0x55 */ u8 landPuffFlag;
    /* 0x56 */ s16 landPuffLifeBase;
    /* 0x58 */ s16 landPuffLifeRange;
} DebrisEmitter;

/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c). */
typedef struct {
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
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[6];
} ParticleSpawn; // size 0x7C

/* Spawn parameters of a BloodTrail effect (see game_161520.c). */
typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;     /* D_80089AFC index */
} BloodTrailDesc; // matching: the callee's copy is 0x1C bytes, but this frame only holds 0x18

/* A BloodTrail effect; the effect header's bytes 1 and 0xC are passed on to the drops it emits. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x40 - 0xD];
    /* 0x40 */ f32 size;       /* extra data: the size of the debris piece it follows */
} BloodTrail;

/* Per-particle data of the debris pieces, at 0x170 of the particle. */
typedef struct {
    /* 0x0 */ f32 stopSpeed;   /* landing slower than this (vel.y above it) stops the piece */
    /* 0x4 */ BloodTrail *trail;
    /* 0x8 */ u8 landPuffs;    /* spawn func_1514AB5C particles on landing */
    /* 0x9 */ u8 landPuffFlag;
    /* 0xA */ s16 landPuffLifeBase;
    /* 0xC */ s16 landPuffLifeRange;
} DebrisData;

/* A live func_15132A4C particle (layout as Particle in game_15F680.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ vertex rot;
    /* 0x02C */ vertex scale;
    /* 0x038 */ vertex pos;
    /* 0x044 */ vertex vel;
    /* 0x050 */ vertex rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0x170 - 0x66];
    /* 0x170 */ DebrisData debris;
} Particle;

extern Particle *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);
extern BloodTrail *func_15134908(BloodTrailDesc *, s32, u8, s32);

/*
 * Spawns emitter->count debris particles at random points within emitter->radius of emitter->pos,
 * each followed by a BloodTrail. mode picks the launch velocity: 0 = horizontal speed along the yaw
 * plus a separate vertical speed, 1 = speed along the spawn direction, 2 = dir, 3 = dir * speed.
 */
void func_151A6F00(DebrisEmitter *emitter, u8 mode, u8 arg2, s32 arg3) {
    s16 count;
    ParticleSpawn spawn;
    s32 pad; // matching: unused stack slot
    DebrisData debris;
    f32 size;
    u8 yaw;
    u8 pitch;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 dist;
    Particle *particle;

    count = emitter->count;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.flags = 0x21E9;
    spawn.variant = 0;
    spawn.lifetime = emitter->lifetime;
    spawn.model = 0x1B;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.elasticity = 0.200000003f;
    debris.stopSpeed = -18.0f;
    debris.landPuffs = emitter->landPuffs;
    debris.landPuffFlag = emitter->landPuffFlag;
    debris.landPuffLifeBase = emitter->landPuffLifeBase;
    debris.landPuffLifeRange = emitter->landPuffLifeRange;
    spawn.unk5C = NULL;
    spawn.alpha = 0xFF;
    spawn.updateFn = 1;
    spawn.update2Fn = 5;
    spawn.floorHitFn = 4;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 1;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 0;
    spawn.owner = NULL;
    spawn.ownerId = 0;
    spawn.fadeStart = 32;
    spawn.fadeRate = 7;
    while (count--) {
        yaw = (func_150ADA20() % (emitter->yawRange + 1)) + emitter->yawBase;
        pitch = (func_150ADA20() % (emitter->pitchRange + 1)) + emitter->pitchBase;
        /* func_151423D8 is a cosine, so cos(a - 64) is sin(a) */
        sinYaw = func_151423D8((u8)(yaw - 64));
        cosYaw = func_151423D8(yaw);
        sinPitch = func_151423D8((u8)(pitch - 64));
        cosPitch = func_151423D8(pitch);
        dist = func_150ADA68() * emitter->radius;
        spawn.pos.x = emitter->pos.x + dist * cosPitch * sinYaw;
        spawn.pos.y = emitter->pos.y - dist * sinPitch;
        spawn.pos.z = emitter->pos.z + dist * cosPitch * cosYaw;
        switch (mode) {
        case 0: {
            f32 speed;
            f32 velY;
            speed = (func_150ADA68() * emitter->speedRange) + emitter->speedBase;
            velY = (func_150ADA68() * emitter->velYRange) + emitter->velYBase;
            spawn.vel.x = speed * sinYaw;
            spawn.vel.y = velY;
            spawn.vel.z = speed * cosYaw;
        }
            break;
        case 1: {
            f32 speed;
            speed = (func_150ADA68() * emitter->speedRange) + emitter->speedBase;
            spawn.vel.x = speed * cosPitch * sinYaw;
            spawn.vel.y = -speed * sinPitch;
            spawn.vel.z = speed * cosPitch * cosYaw;
        }
            break;
        case 2:
            spawn.vel = emitter->dir;
            break;
        case 3: {
            f32 speed;
            speed = (func_150ADA68() * emitter->speedRange) + emitter->speedBase;
            spawn.vel.x = emitter->dir.x * speed;
            spawn.vel.y = emitter->dir.y * speed;
            spawn.vel.z = emitter->dir.z * speed;
            break;
        }
        }
        size = spawn.sizeXZ = spawn.sizeY = (func_150ADA68() * emitter->sizeRange) + emitter->sizeBase;
        spawn.radius = size * 30.0f;
        spawn.gravity = (func_150ADA68() * emitter->gravityRange) + emitter->gravityBase;
        spawn.rotVel.x = (func_150ADA68() * emitter->spinRange) + emitter->spinBase;
        spawn.rotVel.z = (func_150ADA68() * emitter->spinRange) + emitter->spinBase;
        particle = func_15132A4C(&spawn, 0, 0, sizeof(DebrisData), arg2, arg3);
        if (particle != NULL) {
            BloodTrailDesc trailDesc;
            DebrisData *data;
            BloodTrail *trail;
            data = &particle->debris;
            memcpy(data, &debris, sizeof(DebrisData));
            trailDesc.x = &particle->pos.x;
            trailDesc.y = &particle->pos.y;
            trailDesc.z = &particle->pos.z;
            trailDesc.radius = 15.0f * spawn.sizeY;
            trailDesc.lifetime = 300;
            trailDesc.flags = 0;
            trailDesc.emitter = 1;
            trailDesc.rate = 0.3580000103f;
            data->trail = NULL;
            trail = func_15134908(&trailDesc, sizeof(f32), arg2, arg3);
            data->trail = trail;
            if (trail != NULL) {
                memcpy(&trail->size, &size, sizeof(f32));
            }
        }
    }
}

struct S151A73EC {
    s32 unk0;
    void *unk4;
};
s32 func_151A73EC(void *arg0) {
    struct S151A73EC *p = (struct S151A73EC *)((u8 *)arg0 + 0x170);
    if (*(s16 *)((u8 *)arg0 + 0x64) < 0x20) {
        if (p->unk4 != 0) {
            func_1516972C((struct102 *)p->unk4);
            p->unk4 = 0;
        }
    }
    return 1;
}

typedef struct {
    f32 field_0;
    void *field_4;
    u8 spawn;
    u8 alternate;
    s16 first;
    s16 last;
} Game1D4140Entry;

typedef struct {
    u8 pad_0[0x10];
    f32 base_height;
    f32 bounce;
    f32 scale;
    u8 pad1C[0x20];
    f32 height;
    u8 pad40[4];
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    f32 rotation_x;
    f32 rotation_y;
    f32 rotation_z;
    u8 pad5C[4];
    s32 flags;
    s16 field_64;
    u8 pad_66[0x10A];
    Game1D4140Entry field_170;
} Game1D4140State;

void func_1514AB5C(f32, f32, f32, f32, s32, f32, f32, f32, f32, s32, s32, s32);

s32 func_151A743C(Game1D4140State *arg0, f32 arg1, s32 arg2, f32 arg3, f32 arg4) {
    Game1D4140Entry *state;
    u8 alternate;

    state = &arg0->field_170;
    arg0->height = arg0->base_height + arg4;
    if (arg0->field_170.field_4 != 0) {
        func_1516972C(state->field_4);
        state->field_4 = 0;
    }
    if (state->field_0 < arg0->velocity_y) {
        arg0->flags &= ~7;
        arg0->flags &= ~8;
        arg0->flags &= ~0x40;
        arg0->flags &= ~0x20;
        arg0->velocity_x = 0.0f;
        arg0->velocity_y = 0.0f;
        arg0->velocity_z = 0.0f;
        if (arg0->field_64 >= 0x21) {
            arg0->field_64 = 0x20;
        }
        if (state->spawn != 0) {
            alternate = 0;
            if (state->alternate != 0) {
                alternate = 1;
            }
            func_1514AB5C(arg1, arg4, arg3, arg0->scale * 35.0f,
                         func_150ADA20() % 3U + 2,
                         arg0->scale, arg0->scale * 0.5f,
                         arg0->scale, arg0->scale * 0.5f,
                         state->first, state->last, alternate);
        }
    } else {
        arg0->velocity_x *= arg0->bounce;
        arg0->velocity_y = -arg0->velocity_y * arg0->bounce;
        arg0->velocity_z *= arg0->bounce;
        arg0->rotation_x *= arg0->bounce;
        arg0->rotation_y *= arg0->bounce;
        arg0->rotation_z *= arg0->bounce;
    }
    return 1;
}

f32 func_150ADA68(void);
extern f32 D_800BE9A8;
extern f32 D_800A8DC8;
extern f32 D_800A8DCC;
extern f32 D_800A8DD0;
void func_151A26EC(f32 *arg0, f32 *arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE, s32 argF, s32 arg10);
void func_151A7610(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 *arg6)
{
  f32 sp8C[3];
  f32 sp80[3];
  f32 sp74[3];
  f32 r;
  f32 sp6C;
  struct 
  {
    f32 a;
    f32 b;
    u32 c;
    u32 pad0;
    u32 pad1;
  } sp58;
  f32 dt;
  sp6C = *((f32 *) (arg6 + 0x40));
  sp8C[0] = arg0;
  sp8C[1] = arg1;
  sp8C[2] = arg2;
 do { } while (0);
  if (dt && dt)
  {
  }
  r = func_150ADA68();
  dt = D_800BE9A8;
  r = (r * 166.0f) + 160.0f;
  sp74[0] = 0.0f;
  sp74[1] = 0.0f;
  sp74[2] = 0.0f;
  r *= D_800A8DC8;
  sp80[0] = ((-arg3) * dt) * r;
  sp80[1] = ((-arg4) * dt) * r;
  sp80[2] = ((-arg5) * dt) * r;
  sp58.a = func_150ADA68();
  sp58.b = func_150ADA68();
  sp58.c = func_150ADA20();
  func_151A26EC(sp8C, sp74, sp80, 1.0f, ((sp58.a * D_800A8DCC) + (-600.0f)) * D_800A8DD0, ((sp58.b * 500.0f) + 400.0f) * sp6C, (sp58.c % 21U) + 15, (func_150ADA20() % 101U) + 100, 15, 20, 0, -1, 86, 39, 0, arg6[0xC], arg6[1]);
}


typedef struct Obj151A77C0 {
    char pad0[0x14];
    /* 0x14 */ s32 unk14;
    char pad18[0x2F - 0x18];
    /* 0x2F */ u8 unk2F;
    char pad30[0x34 - 0x30];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    char pad3A[0x3F - 0x3A];
    /* 0x3F */ u8 unk3F;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    char pad43;
    /* 0x44 */ u16 unk44;
    char pad46[0x50 - 0x46];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} Obj151A77C0;

typedef struct Sub151A77C0 {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} Sub151A77C0;

void func_151A77C0(Obj151A77C0 *arg0) {
    Sub151A77C0 *s;

    s = (Sub151A77C0 *)&arg0->unk50;
    if (arg0->unk38 < arg0->unk54) {
        arg0->unk3F = arg0->unk38 * arg0->unk56;
    }
    if (arg0->unk38 < s->unk8) {
        s16 d = s->unkA * D_800BE9E4;
        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unk0) {
        arg0->unk2F = 0x15;
        arg0->unk44 |= 0x201;
        arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unk2;
        arg0->unk14 = 0xC000F;
    }
}


typedef struct Obj151A787C {
    char pad0[0x34];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    char pad3A[0x3F - 0x3A];
    /* 0x3F */ u8 unk3F;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
    char pad43[0x50 - 0x43];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
    /* 0x5A */ s16 unk5A;
} Obj151A787C;

typedef struct Sub151A787C {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} Sub151A787C;

void func_151A787C(Obj151A787C *arg0) {
    Sub151A787C *s;

    s = (Sub151A787C *)&arg0->unk50;
    if (arg0->unk38 < arg0->unk54) {
        arg0->unk3F = arg0->unk38 * arg0->unk56;
    }
    if (arg0->unk38 < s->unk8) {
        s16 d = s->unkA * D_800BE9E4;
        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unk2;
}

struct S151A7908 {
    s32 unk0;
    void *unk4;
};
void func_151A7908(void *arg0) {
    struct S151A7908 *p = (struct S151A7908 *)((u8 *)arg0 + 0x170);
    if (p->unk4 != 0) {
        func_1516972C((struct102 *)p->unk4);
        p->unk4 = 0;
    }
}
