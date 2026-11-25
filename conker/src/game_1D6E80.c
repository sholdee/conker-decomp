/* 08 14 18 1C 20 24 28 2C 2E 02 04 06 00 30 32 34 36 38 3A 3C 44 46 48 40 */
#include <ultra64.h>
#define func_10010F30 func_10010F30_void
#define func_150ADA20 func_150ADA20_orig
#include "functions.h"
#undef func_10010F30
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 func_151EF610(void);
extern f32 D_800A9014;
extern s32 func_10010FFC(s32, s32, s32, s32, s32, struct127 *);
extern void *func_15190770(void *, s32, s32, s32);
extern void func_151346EC(u8 *);
extern void func_1513470C(u8 *);
extern u8 func_151D8E20(void);
extern f32 D_800A8FA0;

/* Spawn descriptor for func_1513418C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[0x3];
    /* 0x0C */ struct127 *owner;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ f32 offset[3];
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ u8 unk2D;
} SpawnDesc1513418C;

extern void *func_1513418C(SpawnDesc1513418C *desc, s32 payloadSize, u8 arg2, s32 arg3);

/* Ground probe result filled in by func_15045800 (same layout as game_71820.c's Game71820Hit). */
typedef struct GroundHit {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 object;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 active;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

struct ActorTask;

/*
 * The three effects an ActorTask keeps alive. Each effect object carries a payload (copied in at spawn)
 * that points back at its owner task; the owner sets payload.keepAlive every frame, the effect's update
 * clears it and dies once it is no longer refreshed, and its destructor clears the owner's pointer.
 */

/* Payload at +0x80 of the effect spawned by func_151AA48C: a value oscillating around `base`. */
typedef struct {
    /* 0x00 */ struct ActorTask *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5[0x3];
    /* 0x08 */ u32 phase;      // angle in the top byte (16.16)
    /* 0x0C */ u32 phaseStep;  // per frame unit
    /* 0x10 */ f32 base;
    /* 0x14 */ f32 amplitude;
} PulseFxPayload;

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ u8 pad64[0x1C];
    /* 0x80 */ PulseFxPayload payload;
} PulseFx;

/* Payload at +0xB0 of the effect spawned by func_151AADF8: a value eased toward random targets. */
typedef struct {
    /* 0x00 */ struct ActorTask *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5[0x3];
    /* 0x08 */ f32 minTarget;
    /* 0x0C */ f32 targetRange;
    /* 0x10 */ f32 target;
    /* 0x14 */ s16 timer;
    /* 0x16 */ s16 minDelay;
    /* 0x18 */ s16 delayRange;
    /* 0x1A */ u8 pad1A[0x2];
    /* 0x1C */ f32 easing;
} FlickerFxPayload;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 value;
    /* 0x28 */ u8 pad28[0x88];
    /* 0xB0 */ FlickerFxPayload payload;
} FlickerFx;

/* Payload at +0x58 of the effect spawned by func_151AB2C4: holds a looping sound. */
typedef struct {
    /* 0x00 */ struct ActorTask *owner;
    /* 0x04 */ u8 keepAlive;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 soundHandle;
    /* 0x08 */ f32 unk8;
} LoopSoundFxPayload;

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ LoopSoundFxPayload payload;
} LoopSoundFx;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PulseFx *pulse;
    /* 0x18 */ FlickerFx *flicker;
    /* 0x1C */ LoopSoundFx *sound;
} ActorTaskFx;

/* Callback block attached to an actor; the callbacks below drop effects onto the ground under it. */
typedef struct ActorTask {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ struct127 *actor;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ GroundHit hit;
    /* 0x58 */ ActorTaskFx fx;
} ActorTask;

void func_151AA264(struct127 *actor, GroundHit *hit);
void func_151ABE00(struct127 *actor);
extern s32 func_15045800(f32 *pos, s32 arg1, f32 minY, GroundHit *hit);
extern s32 func_151ABE40(f32 *pos, GroundHit *hit, s32 kind, s32 arg3, s32 arg4);

/* Probes the ground under the task's actor and spawns ground effect 3 there. */
void func_151A99D0(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        if (func_15045800(pos, 0, pos[1] - 300.0f, &task->hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, &task->hit, 3, task->unkC, task->unk1);
        }
    }
}

void func_151A9AA4(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 3, task->unkC, task->unk1);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    func_151ABE00(actor);
}

void func_151A9BA0(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 1, task->unkC, task->unk1);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}


void func_151A9CA0(ActorTask *task) {
    struct127 *actor = task->actor;
    f32 pos[3];

    if ((actor->unk31C != NULL) && ((u8)actor->unk31C->unk95 != 0)) {
        return;
    }
    pos[0] = actor->x_position;
    if (-9500.0f < actor->unk118) {
        pos[1] = actor->unk118 + 100.0f;
    } else {
        pos[1] = actor->y_position + 150.0f;
    }
    pos[2] = actor->z_position;
    if (func_15045800(pos, 0, pos[1] - 300.0f, &task->hit) != 0) {
        pos[1] = task->hit.height;
        func_151ABE40(pos, &task->hit, 1, task->unkC, task->unk1);
        func_10010FFC(0, 8, 0x6978, 0, 0, actor);
    }
    func_151AA264(actor, &task->hit);
    func_151ABE00(actor);
}

void func_151A9DC0(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 2, task->unkC, task->unk1);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}

void func_151A9EC0(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 2, task->unkC, task->unk1);
            func_10010FFC(0, 8, 0x6978, 0, 0, actor);
        }
    }
    func_151AA264(actor, &task->hit);
    func_151ABE00(actor);
}

void func_151A9FC8(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        if (func_15045800(pos, 0, pos[1] - 300.0f, &task->hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, &task->hit, 4, task->unkC, task->unk1);
        }
    }
}

void func_151AA09C(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        if (func_15045800(pos, 0, pos[1] - 300.0f, &task->hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, &task->hit, 4, task->unkC, task->unk1);
        }
    }
}

void func_151AA170(s32 arg0) {
}

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
} Task151AA17C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
} Msg151AA17C;

extern void func_15147D64(s32, s32);
extern void func_1519F3B8(Task151AA17C *);
extern void func_1519F400(void);

void func_151AA17C(Task151AA17C *task) {
    s32 word;
    Msg151AA17C msg;

    word = task->unk18;
    msg.unk0 = word;
    msg.unk4 = task->unk1C;
    word = (s32)&msg; // matching: one int local reused for &msg; a pointer local is rematerialized instead
    func_15147D64(word, 10);
    func_151494E0(word, 10);
    func_1519F3B8(task);
}

void func_151AA1D0(void) {
    func_1519F400();
}

void func_151AA1F0(void) {
    func_1519F400();
}

void func_151AA210(Task151AA17C *task) {
    s32 word;
    Msg151AA17C msg;

    word = task->unk18;
    msg.unk0 = word;
    msg.unk4 = task->unk1C;
    word = (s32)&msg; // matching: one int local reused for &msg; a pointer local is rematerialized instead
    func_15147D64(word, 10);
    func_151494E0(word, 10);
    func_1519F3B8(task);
}

void func_151AA264(struct127 *actor, GroundHit *hit) {
    SpawnDesc1513418C desc;

    if (hit->flags & 1) {
        desc.unk0 = 0;
        desc.unk4 = 0;
        desc.ownerId = actor->unique_id;
        desc.unk10 = 1;
        desc.offset[0] = 0.0f;
        desc.offset[1] = 0.0f;
        desc.offset[2] = 0.0f;
        desc.unk20 = 25.0f;
        desc.unk24 = 1.212440014f;
        desc.unk28 = 30;
        desc.unk2A = 14;
        desc.unk2B = 2;
        desc.unk2C = -1;
        desc.unk2D = 0;
        desc.owner = actor;
        func_1513418C(&desc, 0, 0xFF, 0);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0xF];
    /* 0x1C */ struct127 *actor;
} Task151AA30C;

extern s32 func_1515A920(struct127 *actor, s32 *out);
extern void func_1515A238(f32 *pos, f32 *vel, f32, f32, s32, f32, s32, s32, s32, s32, s32);

/* Spawns one falling particle at (x, y, z) when that point is not above the task actor's unk118. */
void func_151AA30C(f32 x, f32 y, f32 z, f32 arg3, f32 scale, f32 arg5, Task151AA30C *task) {
    f32 pos[3];
    f32 vel[3];
    struct127 *actor;
    s32 surface;

    actor = task->actor;
    if (actor->unk118 < y) {
        return;
    }
    if (func_1515A920(actor, &surface) == 0) {
        surface = 0;
    }
    vel[0] = 0.0f;
    vel[1] = -((func_150ADA68() * 196.0f + 199.0f) * 0.001000000047f * scale);
    vel[2] = 0.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    func_1515A238(pos, vel, (func_150ADA68() * 311.0f + 454.0f) * D_800A8FA0, 0.972683012f, surface,
                  func_150ADA68() * 101.0f + 101.0f, func_150ADA20() % 31U + 50, func_150ADA20() % 101U + 100,
                  1, task->unkC, task->unk1);
}

/* Spawn descriptor for func_15134DAC. */
typedef struct {
    /* 0x00 */ u8  ownerId;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ struct127 *owner;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25[0x1];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} SpawnDesc15134DAC;

extern void *func_15134DAC(SpawnDesc15134DAC *desc, s32 payloadSize);

/* Spawns the pulse effect owned by `task`. */
PulseFx *func_151AA48C(struct127 *actor, ActorTask *task) {
    SpawnDesc15134DAC desc;
    PulseFx *fx;
    PulseFxPayload payload;

    payload.owner = task;
    payload.keepAlive = 1;
    desc.ownerId = actor->unique_id;

    desc.owner = actor;
    desc.unk8 = 15;
    desc.unkC = -11.0f;
    desc.unk10 = -2.0f;
    desc.unk14 = 8.0f;
    desc.unk18 = -11.0f;
    desc.unk1C = -2.0f;
    desc.unk20 = 20.0f;
    desc.unk24 = 0;
    desc.unk26 = 60;
    desc.unk28 = 60;
    desc.unk2A = 300;
    desc.unk2E = 1;
    desc.unk2F = 0;
    desc.unk30 = 1;
    desc.unk34 = 0.5f;
    desc.unk38 = 1;
    desc.unk39 = 0;

    payload.phase = 0;
    payload.phaseStep = 0x11111;
    payload.base = 0.200000003f;
    payload.amplitude = 0.3000000119f;

    fx = func_15134DAC(&desc, sizeof(payload));
    if (fx != NULL) {
        memcpy(&fx->payload, &payload, sizeof(payload));
    }
    return fx;
}

/* PulseFx callback: restarts the pulse and plays sound 10 or 11 (at random) at the effect. */
s32 func_151AA5A4(PulseFx *fx) {
    fx->payload.phase = 0;
    if (func_150ADA20() & 1) {
        func_10010F88(10, 0x55F0, (func_151EF610() % 1200) - 600, 0, 0, fx->unk58, fx->unk5C, fx->unk60, 500, 2500);
        return 1;
    }
    func_10010F88(11, 0x55F0, (func_151EF610() % 1200) - 600, 0, 0, fx->unk58, fx->unk5C, fx->unk60, 500, 2500);
    return 1;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} Vec3F_AA6D8;

typedef struct {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ f32 unk74;
} struct_AA6D8_arg5;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec3F_AA6D8 unk30;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
} struct_AA6D8_params;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ u8 unk04;
    /* 0x05 */ u8 unk05;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x4];
} struct_AA6D8_extra;

void *func_151303BC(s32, u8, s32);

void func_151AA6D8(Vec3F_AA6D8 *arg0, Vec3F_AA6D8 *arg1, s32 arg2, s32 arg3, f32 arg4, struct_AA6D8_arg5 *arg5) {
    void *temp_v0;
    f32 temp_f2;
    s32 padAC;
    struct_AA6D8_params params;
    s32 pad44;
    struct_AA6D8_extra extra;

    params.unk1D = 0x2F;
    params.unk08 = 0xC01;
    params.unk00 = 0x200005;
    params.unk04 = 0;
    params.unk1A = 0;
    params.unk19 = 0;
    params.unk18 = 0;
    params.unk17 = 0;
    params.unk16 = 0;
    params.unk15 = 0;
    params.unk14 = 0;
    params.unk10 = 0;
    params.unk0C = 0;
    params.unk1C = 0xFF;
    params.unk22 = 0;
    params.unk24 = 0.0f;
    params.unk58 = 0x1207;
    params.unk60 = 5;
    params.unk61 = 5;
    params.unk62 = 4;
    params.unk63 = -1;
    params.unk3C = 0.0f;
    params.unk40 = 0.0f;
    params.unk44 = 0.0f;
    params.unk1E = 0x14;
    params.unk20 = 0xC;
    extra.unk04 = 0;
    extra.unk05 = 0;
    extra.unk18 = 0;
    extra.unk19 = 0;
    extra.unk10 = 3.5f;
    extra.unk14 = 0.2857142985f;

    params.unk48 = (arg1->x - arg0->x) * arg5->unk74;
    params.unk4C = (arg1->y - arg0->y) * arg5->unk74;
    params.unk50 = (arg1->z - arg0->z) * arg5->unk74;
    params.unk30 = *arg0;
    params.unk30.x += params.unk48 * arg4;
    params.unk30.y += params.unk4C * arg4;
    params.unk30.z += params.unk50 * arg4;

    if (func_150ADA20() & 1) {
        params.unk58 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        params.unk58 |= 0x80;
    }

    params.unk0A = (func_150ADA20() % 51U) + 0x32;
    params.unk1B = (func_150ADA20() % 101U) + 0x64;
    temp_f2 = (func_150ADA68() * 39.0f) + 31.0f;
    params.unk28 = temp_f2;
    extra.unk00 = temp_f2;
    params.unk2C = temp_f2;
    extra.unk06 = (func_150ADA20() % 5U) + 4;
    extra.unk07 = (func_150ADA20() % 5U) + 4;
    extra.unk08 = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk00;
    extra.unk0C = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk00;
    extra.unk1A = (func_150ADA20() % 5U) + 4;
    extra.unk1B = (func_150ADA20() % 5U) + 4;
    extra.unk1C = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk00;
    extra.unk20 = ((func_150ADA68() * 0.1000000015f) + 0.07999999821f) * extra.unk00;
    params.unk54 = ((func_150ADA68() * 70.0f) + 73.0f) * 0.001000000047f;

    temp_v0 = func_151303BC((s32)&params, 2, 0x28);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0xA8, &extra, 0x28);
    }
}

/* PulseFx update: returns the current oscillation value and advances the phase. */
f32 func_151AAA4C(PulseFx *fx) {
    PulseFxPayload *pulse;
    f32 value;

    pulse = &fx->payload;
    value = (func_151423D8((pulse->phase >> 16) - 0x40) * pulse->amplitude) + pulse->base;
    pulse->phase += pulse->phaseStep * D_800BE9E4;
    return value;
}

LoopSoundFx *func_151AB2C4(struct127 *actor, ActorTask *task);

/* Refreshes the task's pulse and loop-sound effects, spawning whichever is missing. */
void func_151AAABC(ActorTask *task) {
    struct127 *actor;
    ActorTaskFx *fx;

    actor = task->actor;
    fx = &task->fx;
    if (fx->pulse != NULL) {
        PulseFxPayload *pulse;

        pulse = &fx->pulse->payload;
        pulse->keepAlive = 1;
    } else {
        fx->pulse = func_151AA48C(actor, task);
    }
    if (fx->sound != NULL) {
        LoopSoundFxPayload *sound;

        sound = &fx->sound->payload;
        sound->keepAlive = 1;
    } else {
        fx->sound = func_151AB2C4(actor, task);
    }
}

void func_151352EC(PulseFx *fx);

/* PulseFx destructor (variant 1): detaches from the owner task. */
void func_151AAB50(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    func_151352EC(fx);
}

void func_1513530C(PulseFx *fx);

/* PulseFx destructor (variant 2): detaches from the owner task. */
void func_151AAB78(PulseFx *fx) {
    ActorTaskFx *ownerFx = &fx->payload.owner->fx;

    ownerFx->pulse = NULL;
    func_1513530C(fx);
}

/* PulseFx keep-alive poll: TRUE if the owner refreshed it since the last call. */
s32 func_151AABA0(PulseFx *fx) {
    s32 alive = TRUE;

    if (fx->payload.keepAlive == 0) {
        alive = FALSE;
    }
    fx->payload.keepAlive = 0;
    return alive;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec_151AABC4;

typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ Vec_151AABC4 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
} Cfg_151AABC4;

typedef struct {
    f32 y;
    u8 pad4[0x20];
} Floor_151AABC4;

extern u8 D_800A8F70[];
extern void func_15142314(s32, u8, Vec_151AABC4 *, void *);
extern void func_1504715C();
extern s32 func_1504697C(Vec_151AABC4 *, s32, f32, Floor_151AABC4 *);
extern void func_15153F18(Cfg_151AABC4 *, Vec_151AABC4 *, Floor_151AABC4 *, u8, s32);

void func_151AABC4(u8 *arg0, u8 arg1) {
    Vec_151AABC4 pos;
    Floor_151AABC4 floor;
    f32 y;
    Vec_151AABC4 probe;
    u8 color;
    Vec_151AABC4 hit;

    if ((arg0 != NULL) && (*(s32 *)(arg0 + 0x1D4) != 0)) {
        color = func_151D8E20();
        func_15142314(*(s32 *)(arg0 + 0x1D4), D_800A8F70[arg1], &pos, arg0);
        func_1504715C(&floor, arg0);
        y = pos.y + 50.0f;
        probe.x = pos.x;
        probe.y = y;
        probe.z = pos.z;
        if (func_1504697C(&probe, 0, y - 100.0f, &floor) != 0) {
            hit.x = pos.x;
            hit.y = floor.y;
            hit.z = pos.z;
            func_151DBCBC(color, 40.0f, 0x96, (s32)&floor.pad4, &hit, 0xFF, 1);
            {
                Cfg_151AABC4 c;

                c.unk08 = hit;
                c.unk14 = 2.5f;
                c.unk18 = 1.549999952f;
                c.unk1C = 0.7910000086f;
                c.unk20 = 0.2030000091f;
                c.unk24 = 8.039999962f;
                c.unk28 = 3.449999809f;
                c.unk2C = 3;
                c.unk2E = 3;
                c.unk02 = 0xFF;
                c.unk04 = -0x40;
                c.unk06 = 0x1A;
                c.unk00 = 0;
                c.unk30 = 3;
                c.unk32 = 1;
                c.unk34 = 0x1E;
                c.unk36 = 0x14;
                c.unk38 = 0x9B;
                c.unk3A = 0x64;
                c.unk3C = color;
                c.unk44 = 0x10;
                c.unk46 = 0xF;
                c.unk48 = 0;
                c.unk40 = 0.0f;
                func_15153F18(&c, &c.unk08, &floor, 0xFF, 1);
            }
        }
    }
}

typedef struct {
    u8 pad_0x0[0x1B];
    u8 field_0x1B;
} AlphaFadePayload;

typedef struct {
    u8 pad_0x0[0x1C];
    s16 field_0x1C;
    u8 pad_0x1E[0x7A];
    AlphaFadePayload *field_0x98;
} AlphaFadeOwner;

s32 func_151AADBC(AlphaFadeOwner *arg0) {
    AlphaFadePayload *p;
    s32 v1;

    p = arg0->field_0x98;
    v1 = arg0->field_0x1C << 4;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    if (v1 < p->field_0x1B) {
        p->field_0x1B = v1;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 pad21[7];
} Particle_151AADF8;

extern u8 D_800AB414[];

/* Keeps the task's flicker effect alive, or spawns it on the ground under the actor. */
void func_151AADF8(ActorTask *task) {
    struct127 *actor;
    ActorTaskFx *fx;
    FlickerFxPayload *keep;
    f32 pos[3];
    Particle_151AADF8 part;
    FlickerFxPayload payload;
    f32 size;
    u8 color;

    actor = task->actor;
    fx = &task->fx;
    if (fx->flicker != NULL) {
        keep = &fx->flicker->payload;
        keep->keepAlive = 1;
    // matching: an unused label before the return makes IDO branch past the epilogue's first load
    refreshed:
        return;
    }
    pos[0] = actor->x_position;
    if (-9500.0f < actor->unk118) {
        pos[1] = actor->unk118 + 100.0f;
    } else {
        pos[1] = actor->y_position + 150.0f;
    }
    pos[2] = actor->z_position;
    if (func_15045800(pos, 0, pos[1] - 300.0f, &task->hit) != 0) {
        color = func_151D8E20();
        pos[1] = task->hit.height;
        size = (func_150ADA68() * 30.0f) + 50.0f;
        payload.owner = task;
        payload.keepAlive = 1;
        payload.minDelay = 10;
        payload.delayRange = 10;
        payload.minTarget = 26214.40039f;
        payload.targetRange = 32768.0f;
        payload.target = 0.0f;
        payload.timer = 0;
        payload.easing = 0.2310000062f;
        part.unkC = (s32)(payload.targetRange * 0.5f + payload.minTarget);
        part.unk6 = 0x38;
        part.unk0 = 0x60300;
        part.unk4 = 300;
        part.unk10 = 150;
        part.unk8 = 0;
        part.unk11 = 0xFF;
        part.unk12 = D_800AB414[color * 3];
        part.unk13 = D_800AB414[color * 3 + 1];
        part.unk14 = D_800AB414[color * 3 + 2];
        part.unk15 = 0xFF;
        part.unk18 = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        fx->flicker = (FlickerFx *)func_1513C73C((s32)&part, 7, 1, (s32)task->hit.points, pos[0], pos[1], pos[2],
                                                 size, size, func_150ADA20() & 0xFF,
                                                 (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), 0x20, 0xFF, 1);
        if (fx->flicker != NULL) {
            memcpy(&fx->flicker->payload, &payload, sizeof(payload));
        }
    }
}

extern f32  func_150ADA68(void);

typedef struct {
    /* 0x00 */ u8  pad0[0x4];
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  pad5[0x3];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
} struct_AB090;

typedef struct {
    /* 0x00 */ u8  pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8  pad28[0x88];
    /* 0xB0 */ struct_AB090 unkB0;
} struct_AB090_root;

s32 func_151AB090(struct_AB090_root *arg0) {
    s8 ret = 1;
    struct_AB090 *v1 = &arg0->unkB0;

    if (*((u8 *)arg0 + 0xB4) == 0) {
        ret = 0;
    }
    v1->unk4 = 0;
    v1->unk14 -= D_800BE9E4;
    if (v1->unk14 < 0) {
        v1->unk14 = (func_150ADA20() % (u32)(v1->unk18 + 1)) + v1->unk16;
        v1->unk10 = func_150ADA68() * v1->unkC + v1->unk8;
    }
    arg0->unk24 += (s32)((v1->unk10 - (f32)arg0->unk24) * v1->unk1C);
    return ret;
}

extern s32 func_1513F6C0(void *arg0, s32 arg1, s32 arg2);

s32 func_151AB180(u8 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0xB0)) + 0x58);
    temp[6] = 0;
    *(s32 *)((u8 *)arg0 + 0x18) |= 2;
    *(u8 **)((u8 *)arg0 + 0xB0) = 0;
    func_1513F6C0(arg0, 0, 0);
    return 0;
}

void func_151AB1C4(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 3, task->unkC, task->unk1);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
    func_151AA264(actor, &task->hit);
}

/* Spawns the looping-sound effect owned by `task`. */
LoopSoundFx *func_151AB2C4(struct127 *actor, ActorTask *task) {
    SpawnDesc1513418C desc;
    LoopSoundFxPayload payload;
    LoopSoundFx *fx;

    payload.owner = task;
    payload.soundHandle = 0;
    payload.keepAlive = 1;
    payload.unk8 = actor->unk118;
    desc.unk0 = 0;
    desc.unk4 = 0;
    desc.ownerId = actor->unique_id;
    desc.unk10 = 1;
    desc.offset[0] = 0.0f;
    desc.offset[1] = 0.0f;
    desc.offset[2] = 0.0f;
    desc.unk28 = 300;
    desc.unk2A = 10;
    desc.unk2B = 3;
    desc.unk2C = 0;
    desc.unk2D = 1;
    desc.owner = actor;
    desc.unk20 = 25.0f;
    desc.unk24 = 0.2649700046f;

    fx = func_1513418C(&desc, sizeof(payload), 0xFF, 0);
    if (fx != NULL) {
        memcpy(&fx->payload, &payload, sizeof(payload));
    }
    return fx;
}

typedef struct Game1D6E80Burst {
    s32 flags, zero4;
    s16 texture, lifetime;
    s32 zeroC, zero10;
    u8 zero14, zero15, zero16, zero17, zero18, zero19, zero1A;
    u8 alpha, maximum, kind;
    s16 duration, period, zero22;
    f32 zero24, width, height;
    f32 x, y, z;
    f32 zero3C, zero40, zero44;
    f32 vx, vy, vz, rotation;
    s32 mode, unused5C;
    s8 field60, field61, field62, field63;
    u8 pad64[0xC];
} Game1D6E80Burst;

typedef struct Game1D6E80BurstMotion {
    f32 size;
    u8 mode0, mode1, count0, count1;
    f32 amount0, amount1, speed, damping;
    u8 mode2, mode3, count2, count3;
    f32 amount2, amount3, sourceScale;
} Game1D6E80BurstMotion;

void *func_151303BC(s32, u8, s32);
extern f32 D_800A8FF4, D_800A8FF8, D_800A8FFC, D_800A9000;
extern f32 D_800A9004, D_800A9008, D_800A900C, D_800A9010;

void func_151AB3A4(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, f32 arg5, void *arg6) {
    Game1D6E80Burst burst;
    void *spawned;
    Game1D6E80BurstMotion motion;
    burst.kind = 0x2F;
    burst.texture = 0xC01;
    burst.flags = 0x200005;
    burst.zero4 = 0;
    burst.zero1A = 0;
    burst.zero19 = 0;
    burst.zero18 = 0;
    burst.zero17 = 0;
    burst.zero16 = 0;
    burst.zero15 = 0;
    burst.zero14 = 0;
    burst.zero10 = 0;
    burst.zeroC = 0;
    burst.maximum = 0xFF;
    burst.zero22 = 0;
    burst.zero24 = 0.0f;
    burst.mode = 0x3207;
    burst.field60 = 5;
    burst.field61 = 5;
    burst.field62 = 4;
    burst.field63 = -1;
    burst.x = arg0;
    burst.y = arg1;
    burst.zero3C = 0.0f;
    burst.zero40 = 0.0f;
    burst.zero44 = 0.0f;
    burst.duration = 30;
    burst.period = 8;
    burst.z = arg2;
    motion.sourceScale = *(f32 *)((u8 *)arg6 + 0x60);
    motion.speed = 4.0f;
    motion.damping = 0.25f;
    motion.mode0 = 0;
    motion.mode1 = 0;
    motion.mode2 = 0;
    motion.mode3 = 0;
    burst.vx = -arg3 * 0.2f;
    burst.vy = 0.0f;
    burst.vz = -arg5 * 0.2f;
    if (func_150ADA20() & 1) burst.mode |= 0x40;
    if (func_150ADA20() & 1) burst.mode |= 0x80;
    burst.lifetime = func_150ADA20() % 51U + 106;
    burst.alpha = func_150ADA20() % 101U + 100;
    motion.size = burst.width = burst.height = func_150ADA68() * 62.0f + 41.0f;
    motion.count0 = func_150ADA20() % 5U + 4;
    motion.count1 = func_150ADA20() % 5U + 4;
    motion.amount0 = (func_150ADA68() * 0.25f + D_800A8FF8) * motion.size;
    motion.amount1 = (func_150ADA68() * 0.25f + D_800A8FFC) * motion.size;
    motion.count2 = func_150ADA20() % 5U + 4;
    motion.count3 = func_150ADA20() % 5U + 4;
    motion.amount2 = (func_150ADA68() * D_800A9000 + D_800A9004) * motion.size;
    motion.amount3 = (func_150ADA68() * D_800A9008 + D_800A900C) * motion.size;
    burst.rotation = (func_150ADA68() * 60.0f + 21.0f) * D_800A9010;
    spawned = func_151303BC((s32)&burst, 2, 0x28);
    if (spawned != NULL) memcpy((u8 *)spawned + 0xA8, &motion, 0x28);
}

typedef struct {
    u8 pad_0x0[0x4];
    u8 field_0x4;
    u8 pad_0x5[0x1];
    u16 field_0x6;
} SoundHandlePlayback;

typedef struct {
    u8 pad_0x0[0x1C];
    s32 field_0x1C;
} SoundHandleState;

typedef struct {
    u8 pad_0x0[0x58];
    s32 field_0x58;
    u8 pad_0x5C[0x2];
    volatile u16 field_0x5E;
} SoundHandleOwner;

s32 func_151AB6B8(SoundHandleOwner *arg0) {
    u8 ret;
    SoundHandlePlayback *temp_v1;

    ret = 1;
    temp_v1 = (SoundHandlePlayback *)&arg0->field_0x58;
    if (arg0->field_0x5E != 0) {
        if (D_800DBFF0->unk5F0 & 1) {
        } else {
            func_100111C8(temp_v1->field_0x6);
            temp_v1->field_0x6 = 0;
            goto done_sound;
        }
    }
    if (temp_v1->field_0x6 == 0) {
        if (D_800DBFF0->unk5F0 & 1) {
            temp_v1->field_0x6 = func_10010F30(0x355, 0x7D00, 0x40, 0, 0);
        }
    }

done_sound:
    if (temp_v1->field_0x4 == 0) {
        ret = 0;
    }
    temp_v1->field_0x4 = 0;
    return ret;
}

void func_151AB788(SoundHandleOwner *arg0) {
    union {
        SoundHandleState *v0;
        f64 align;
    } sp18;

    sp18.v0 = (SoundHandleState *)(arg0->field_0x58 + 0x58);
    if (arg0->field_0x5E != 0) {
        func_100111C8(arg0->field_0x5E);
    }
    sp18.v0->field_0x1C = 0;
    func_151346EC((u8 *)arg0);
}

void func_151AB7D8(SoundHandleOwner *arg0) {
    union {
        SoundHandleState *v0;
        f64 align;
    } sp18;

    sp18.v0 = (SoundHandleState *)(arg0->field_0x58 + 0x58);
    if (arg0->field_0x5E != 0) {
        func_100111C8(arg0->field_0x5E);
    }
    sp18.v0->field_0x1C = 0;
    func_1513470C((u8 *)arg0);
}

void func_151AB828(u8 *arg0) {
    u8 *a3;

    a3 = arg0;
    func_15141DA4(*(void **)(a3 + 0x18), 0, 4);
}

void func_151AB854(u8 *arg0) {
    u8 *v1;
    struct185 tmp;

    v1 = *(u8 **)(arg0 + 0x18);
    if ((v1[4] == 0) || (v1[4] == 1) || (v1[4] == 2) || (v1[4] == 3) || (v1[4] == 4) || (v1[4] == 0x96)) {
        tmp.unk0 = (struct127 *)v1;
        tmp.unk4 = v1[0x3B];
        tmp.unk6 = 0x12C;
        tmp.unk8 = 0;
        tmp.unk9 = 0;
        if (v1[4] == 0x96) {
            tmp.unkA = 3;
        } else {
            tmp.unkA = 0;
        }
        if (v1[0x127] != 0xFF) {
            *(s16 *)(*(u8 **)(v1 + 0x31C) + 0x66) = 0x1F4;
        }
        func_15190770(&tmp, 0, arg0[0xC], arg0[1]);
    }
}

void func_151AB920(s32 arg0, s32 arg1) {
}

void func_151AB930(struct127 *arg0) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        u8 unk18;
    } sp28;

    sp28.unk0 = arg0;
    sp28.unk4 = ((struct127 *)arg0)->unique_id;
    sp28.unk8 = arg0->unk84.uh;
    sp28.unkC = 0.0f;
    sp28.unk10 = D_800A9014;
    sp28.unk14 = 3.0f;
    sp28.unk18 = func_151D8E20();
    temp_v0 = func_151491F4(0x3C, -1, 0xE, 1, 9, 0x1C, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp28, 0x1C);
    }
}

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
    u8 pad3C[0x48];
    u16 unk84;
} Ent_151AB9C8;

typedef struct {
    Ent_151AB9C8 *unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
} Sub_151AB9C8;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    Sub_151AB9C8 unk28;
} Obj_151AB9C8;

typedef struct {
    u8 pad[0x24];
} Floor_151AB9C8;

extern void func_1504715C(Floor_151AB9C8 *, Ent_151AB9C8 *);
extern void func_151D9014(f32 *, f32 *, u8, f32, s32, s32, f32, s32, f32, f32, s32, Floor_151AB9C8 *, s32, s32, u8, u8);

void func_151AB9C8(Obj_151AB9C8 *arg0) {
    Sub_151AB9C8 *sub;
    u8 dead;
    Floor_151AB9C8 floor;
    f32 pos[3];
    f32 vel[3];
    s16 yaw;
    s16 pitch;
    f32 sinP;
    f32 cosP;
    f32 sinY;
    f32 cosY;
    f32 speed;
    s32 r1;
    s32 r2;

    dead = 0;
    sub = &arg0->unk28;
    if (arg0->unk28.unk0->unk0 == 0) {
        dead = 1;
    } else if (sub->unk0->unk3B != sub->unk4) {
        dead = 1;
    } else if (sub->unk0->unk84 != sub->unk8) {
        dead = 1;
    }
    if (!dead) {
        sub = &arg0->unk28;
        sub->unkC += (sub->unk10 + func_150ADA68() * sub->unk14) * D_800BE9A4;
        if (sub->unkC > 1.0f) {
            func_1504715C(&floor, sub->unk0);
            do {
                yaw = func_150ADA20() & 0xFF;
                pitch = (func_150ADA20() % 46U) - 0x3F;
                sinP = func_151423D8(pitch);
                cosP = func_151423D8(pitch - 0x40);
                sinY = func_151423D8(yaw);
                cosY = func_151423D8(yaw - 0x40);
                speed = (func_150ADA68() * 713.0f + 500.0f) * 0.009999999776f;
                pos[0] = sub->unk0->unk14 + 30.0f * sinP * cosY;
                pos[1] = sub->unk0->unk18 - 75.0f * cosP;
                pos[2] = sub->unk0->unk1C + 30.0f * sinP * sinY;
                vel[0] = speed * sinP * cosY;
                vel[1] = -speed * cosP;
                vel[2] = speed * sinP * sinY;
                {
                    f32 t;
                t = func_150ADA68();
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_151D9014(pos, vel, sub->unk18, t * 0.5f + -1.0f, (u32)r1 % 26U + 0x20, (u32)r2 % 101U + 100,
                              func_150ADA68() * 34.0f + 52.0f, 1, 1.0f, 1.0f, 1, &floor, 1, 0, arg0->unkC, arg0->unk1);
                }
                sub->unkC -= 1.0f;
            } while (sub->unkC > 1.0f);
        }
    }
    if (dead) {
        arg0->unkE = -1;
    }
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151ABD54;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151ABD54;

void func_151ABD54(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151ABD54 *temp_v0 = (SubA_151ABD54 *)((u8 *)arg0 + 0x28);
    ArgB_151ABD54 *b = (ArgB_151ABD54 *)arg1;

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

typedef struct {
    /* 0x00 */ struct127 *actor;
    /* 0x04 */ s8 actorId;
} Msg151ABE00;

void func_151ABE00(struct127 *actor) {
    Msg151ABE00 msg;

    msg.actor = actor;
    msg.actorId = actor->unique_id;

    func_1516944C(0x20, (s32)&msg, 12);
}
