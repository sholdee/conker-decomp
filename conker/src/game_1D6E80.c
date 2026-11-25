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
extern s32 func_10010FFC(s32, s32, s32, s32, s32, s32);
extern void func_151AA264(void *, s32);
extern void func_151ABE00(void *);
extern void *func_15190770(void *, s32, s32, s32);
extern void func_151346EC(u8 *);
extern void func_1513470C(u8 *);
extern u8 func_151D8E20(void);
extern f32 D_800A8FF0;
extern f32 D_800A8F94;
extern f32 D_800A8F98;
extern f32 D_800A8F9C;
extern f32 D_800A8FA0;
extern s32 func_1515A920(void *, s32 *);
extern void func_1515A238(f32 *, f32 *, f32, f32, s32, f32, s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
} struct_151AB2C4_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[0x3];
    void *unkC;
    u8 unk10;
    u8 pad11[0x3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    s8 unk2C;
    u8 unk2D;
} struct_151AB2C4_desc;

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    f32 unk8;
} struct_151AB2C4_payload;

extern void *func_1513418C(struct_151AB2C4_desc *, s32, u8, s32);

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xF8];
    f32 unk118;
} struct_151A9EC0_inner;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    struct_151A9EC0_inner *unk18;
    u8 pad1C[0x18];
    f32 unk34;
} struct_151A9EC0_arg;

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
} ActorTask;

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

void func_151AA264(void *arg0, s32 arg1) {
    struct_151AB2C4_desc sp18;

    if (*((u8 *)arg1 + 0x1C) & 1) {
        sp18.unk0 = 0;
        sp18.unk4 = 0;
        sp18.unk8 = *((u8 *)arg0 + 0x3B);
        sp18.unk10 = 1;
        sp18.unk14 = 0.0f;
        sp18.unk18 = 0.0f;
        sp18.unk1C = 0.0f;
        sp18.unk20 = 25.0f;
        sp18.unk24 = D_800A8F94;
        sp18.unk28 = 0x1E;
        sp18.unk2A = 0xE;
        sp18.unk2B = 2;
        sp18.unk2C = -1;
        sp18.unk2D = 0;
        sp18.unkC = arg0;
        func_1513418C(&sp18, 0, 0xFF, 0);
    }
}

void func_151AA30C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, struct_151A9EC0_arg *arg6) {
    f32 sp5C[3];
    f32 sp50[3];

    if (((struct_151A9EC0_inner *)*(void **)((u8 *)arg6 + 0x1C))->unk118 < arg1) {
        return;
    }

    if (func_1515A920(*(void **)((u8 *)arg6 + 0x1C), &((s32 *)sp50)[-2]) == 0) {
        ((s32 *)sp50)[-2] = 0;
    }

    sp50[0] = 0.0f;
    sp50[1] = -(((func_150ADA68() * 196.0f) + 199.0f) * D_800A8F98 * arg4);
    sp50[2] = 0.0f;
    sp5C[0] = arg0;
    sp5C[1] = arg1;
    sp5C[2] = arg2;

    ((f32 *)sp50)[-6] = func_150ADA68();
    ((f32 *)sp50)[-5] = func_150ADA68();
    ((s32 *)sp50)[-4] = func_150ADA20();

    func_1515A238(sp5C, sp50, ((((f32 *)sp50)[-6] * D_800A8F9C) + 454.0f) * D_800A8FA0,
                  0.972683012f, ((s32 *)sp50)[-2], (((f32 *)sp50)[-5] * 101.0f) + 101.0f,
                  (((s32 *)sp50)[-4] % 0x1FU) + 0x32, (func_150ADA20() % 0x65U) + 0x64,
                  1, arg6->unkC, arg6->unk1);
}

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
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
} struct_10E4_AA48C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  pad5[0x3];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} struct_AA48C_b;

typedef struct {
    /* 0x00 */ u8  pad0[0x58];
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5C;
    /* 0x60 */ f32 field_0x60;
    /* 0x64 */ u8  pad64[0x24];
    /* 0x88 */ s32 field_0x88;
} struct_AA48C_effect;

extern f32 D_800A8FA4;
extern f32 D_800A8FA8;
extern void *func_15134DAC(struct_10E4_AA48C *, s32);

void *func_151AA48C(void *arg0, s32 arg1) {
    struct_10E4_AA48C sp34;
    void *temp_v0;
    struct_AA48C_b sp18;

    sp18.unk0 = arg1;
    sp18.unk4 = 1;
    sp34.unk0 = *((u8 *)arg0 + 0x3B);

    sp34.unk4 = arg0;
    sp34.unk8 = 0xF;
    sp34.unkC = -11.0f;
    sp34.unk10 = -2.0f;
    sp34.unk14 = 8.0f;
    sp34.unk18 = -11.0f;
    sp34.unk1C = -2.0f;
    sp34.unk20 = 20.0f;
    sp34.unk24 = 0;
    sp34.unk26 = 0x3C;
    sp34.unk28 = 0x3C;
    sp34.unk2A = 0x12C;
    sp34.unk2E = 1;
    sp34.unk2F = 0;
    sp34.unk30 = 1;
    sp34.unk34 = 0.5f;
    sp34.unk38 = 1;
    sp34.unk39 = 0;

    sp18.unk8 = 0;
    sp18.unkC = 0x11111;
    sp18.unk10 = D_800A8FA4;
    sp18.unk14 = D_800A8FA8;

    temp_v0 = func_15134DAC(&sp34, 0x18);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp18, 0x18);
    }
    return temp_v0;
}

s32 func_151AA5A4(struct_AA48C_effect *arg0) {
    arg0->field_0x88 = 0;
    if (func_150ADA20() & 1) {
        func_10010F88(0xA, 0x55F0, (s16)((func_151EF610() % 0x4B0) - 0x258), 0, 0,
                      (s32)arg0->field_0x58, (s32)arg0->field_0x5C,
                      (s32)arg0->field_0x60, 0x1F4, 0x9C4);
        return 1;
    }
    func_10010F88(0xB, 0x55F0, (s16)((func_151EF610() % 0x4B0) - 0x258), 0, 0,
                  (s32)arg0->field_0x58, (s32)arg0->field_0x5C,
                  (s32)arg0->field_0x60, 0x1F4, 0x9C4);
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

typedef struct Sub151AAA4C {
    char pad0[0x8];
    /* 0x88 */ u32 unk8;
    /* 0x8C */ u32 unkC;
    /* 0x90 */ f32 unk10;
    /* 0x94 */ f32 unk14;
} Sub151AAA4C;
typedef struct Obj151AAA4C {
    char pad0[0x80];
    Sub151AAA4C unk80;
} Obj151AAA4C;


f32 func_151AAA4C(Obj151AAA4C *arg0) {
    Sub151AAA4C *s;
    f32 r;

    s = &arg0->unk80;
    r = (func_151423D8((s->unk8 >> 16) - 0x40) * s->unk14) + s->unk10;
    s->unk8 += s->unkC * D_800BE9E4;
    return r;
}

typedef struct Flag151AAABC {
    char pad0[0x4];
    /* 0x4 */ u8 unk4;
} Flag151AAABC;

typedef struct EffA151AAABC {
    char pad0[0x80];
    /* 0x80 */ Flag151AAABC unk80;
} EffA151AAABC;

typedef struct EffB151AAABC {
    char pad0[0x58];
    /* 0x58 */ Flag151AAABC unk58;
} EffB151AAABC;

typedef struct Sub151AAABC {
    char pad0[0x14];
    /* 0x14 */ EffA151AAABC *unk14;
    char pad18[0x4];
    /* 0x1C */ EffB151AAABC *unk1C;
} Sub151AAABC;

typedef struct Obj151AAABC {
    char pad0[0x18];
    /* 0x18 */ void *unk18;
    char pad1C[0x58 - 0x1C];
    /* 0x58 */ Sub151AAABC unk58;
} Obj151AAABC;

void *func_151AB2C4(struct_151AB2C4_arg *, s32);

void func_151AAABC(Obj151AAABC *arg0) {
    void *v;
    Sub151AAABC *p;

    v = arg0->unk18;
    p = &arg0->unk58;
    if (p->unk14 != NULL) {
        Flag151AAABC *q;

        q = &p->unk14->unk80;
        q->unk4 = 1;
    } else {
        p->unk14 = func_151AA48C(v, (s32)arg0);
    }
    if (p->unk1C != NULL) {
        Flag151AAABC *r;

        r = &p->unk1C->unk58;
        r->unk4 = 1;
    } else {
        p->unk1C = func_151AB2C4(v, (s32)arg0);
    }
}

void func_151352EC(struct102 *arg0);

void func_151AAB50(struct102 *arg0) {
    s32 *p = (s32 *)(*(s32 *)((u8 *)arg0 + 0x80) + 0x58);
    p[5] = 0;
    func_151352EC(arg0);
}

void func_1513530C(struct102 *arg0);

void func_151AAB78(struct102 *arg0) {
    s32 *p = (s32 *)(*(s32 *)((u8 *)arg0 + 0x80) + 0x58);
    p[5] = 0;
    func_1513530C(arg0);
}

s32 func_151AABA0(u8 *arg0) {
    s32 ret = 1;
    if (arg0[0x84] == 0) {
        ret = 0;
    }
    arg0[0x84] = 0;
    return ret;
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

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 pad1A[2];
    f32 unk1C;
} Payload_151AADF8;

typedef struct {
    u8 pad0[4];
    u8 unk4;
} Flag_151AADF8;

typedef struct {
    u8 pad0[0xB0];
    Flag_151AADF8 unkB0;
} Eff_151AADF8;

typedef struct {
    u8 pad0[0x18];
    Eff_151AADF8 *unk18;
} Sub_151AADF8;

typedef struct {
    u8 pad0[0x18];
    struct_151A9EC0_inner *unk18;
    u8 pad1C[0x34 - 0x1C];
    f32 unk34;
    u8 unk38[0x20];
    Sub_151AADF8 unk58;
} Obj_151AADF8;

extern u8 D_800AB414[];

void func_151AADF8(Obj_151AADF8 *arg0) {
    struct_151A9EC0_inner *v;
    Sub_151AADF8 *p;
    Flag_151AADF8 *q;
    f32 pos[3];
    Particle_151AADF8 part;
    Payload_151AADF8 payload;
    f32 size;
    u8 color;

    v = arg0->unk18;
    p = &arg0->unk58;
    if (p->unk18 != NULL) {
        q = &p->unk18->unkB0;
        q->unk4 = 1;
    dummy:
        return;
    }
    pos[0] = v->unk14;
    if (-9500.0f < *(f32 *)((u8 *)v + 0x118)) {
        pos[1] = *(f32 *)((u8 *)v + 0x118) + 100.0f;
    } else {
        pos[1] = v->unk18 + 150.0f;
    }
    pos[2] = v->unk1C;
    if (func_15045800(pos, 0, pos[1] - 300.0f, (s32)&arg0->unk34) != 0) {
        color = func_151D8E20();
        pos[1] = arg0->unk34;
        size = (func_150ADA68() * 30.0f) + 50.0f;
        payload.unk0 = arg0;
        payload.unk4 = 1;
        payload.unk16 = 10;
        payload.unk18 = 10;
        payload.unk8 = 26214.40039f;
        payload.unkC = 32768.0f;
        payload.unk10 = 0.0f;
        payload.unk14 = 0;
        payload.unk1C = 0.2310000062f;
        part.unkC = (s32)(payload.unkC * 0.5f + payload.unk8);
        part.unk6 = 0x38;
        part.unk0 = 0x60300;
        part.unk4 = 0x12C;
        part.unk10 = 0x96;
        part.unk8 = 0;
        part.unk11 = 0xFF;
        part.unk12 = D_800AB414[color * 3];
        part.unk13 = D_800AB414[color * 3 + 1];
        part.unk14 = D_800AB414[color * 3 + 2];
        part.unk15 = 0xFF;
        part.unk18 = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        p->unk18 = (Eff_151AADF8 *)func_1513C73C((s32)&part, 7, 1, (s32)arg0->unk38, pos[0], pos[1], pos[2], size, size,
                            func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), 0x20, 0xFF, 1);
        if (p->unk18 != NULL) {
            memcpy(&p->unk18->unkB0, &payload, 0x20);
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

void *func_151AB2C4(struct_151AB2C4_arg *arg0, s32 arg1) {
    struct_151AB2C4_desc sp28;
    struct_151AB2C4_payload sp1C;
    void *temp_v0;

    sp1C.unk0 = arg1;
    sp1C.unk6 = 0;
    sp1C.unk4 = 1;
    sp1C.unk8 = arg0->unk118;
    sp28.unk0 = 0;
    sp28.unk4 = 0;
    sp28.unk8 = arg0->unk3B;
    sp28.unk10 = 1;
    sp28.unk14 = 0.0f;
    sp28.unk18 = 0.0f;
    sp28.unk1C = 0.0f;
    sp28.unk28 = 0x12C;
    sp28.unk2A = 0xA;
    sp28.unk2B = 3;
    sp28.unk2C = 0;
    sp28.unk2D = 1;
    sp28.unkC = arg0;
    sp28.unk20 = 25.0f;
    sp28.unk24 = D_800A8FF0;

    temp_v0 = func_1513418C(&sp28, 0xC, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x58, &sp1C, 0xC);
    }
    return temp_v0;
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

struct tmp151ABE00 {
    s32 unk0;
    s8  unk4;
};

void func_151ABE00(void *arg0) {
    struct tmp151ABE00 tmp;

    tmp.unk0 = (s32)arg0;
    tmp.unk4 = *(u8 *)((char *)arg0 + 0x3B);

    func_1516944C(0x20, (s32)&tmp, 0xC);
}
