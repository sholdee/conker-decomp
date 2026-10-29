#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    char pad_0x0[0x4];
    u8 field_0x4;
} GameF3270Child58State;

typedef struct {
    char pad_0x0[0x58];
    GameF3270Child58State field_0x58;
} GameF3270SpawnedObject;

typedef struct {
    char pad_0x0[0x18];
    s32 field_0x18;
    char pad_0x1C[0x18];
    f32 field_0x34;
    char pad_0x38[0x24];
    GameF3270SpawnedObject *volatile field_0x5C;
} GameF3270SpawnOwner;

typedef struct {
    char pad_0x0[0x4];
    u8 field_0x4;
    char pad_0x5[0x3];
    f32 field_0x8;
    f32 field_0xC;
    f32 field_0x10;
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    char pad_0x1A[0x2];
    f32 field_0x1C;
} GameF3270B0State;

typedef struct {
    char pad_0x0[0x24];
    s32 field_0x24;
    char pad_0x28[0x88];
    GameF3270B0State field_0xB0;
} GameF3270Object;

typedef struct {
    char pad_0x0[0x58];
    GameF3270Object *volatile field_0x58;
} GameF3270ObjectOwner58;

GameF3270Object *func_150C6460(GameF3270ObjectOwner58 *arg0);
typedef struct {
    char pad_0x0[0x14];
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    char pad_0x20[0x1B];
    u8 field_0x3B;
} GameF3270SpawnSource;

GameF3270SpawnedObject *func_150C5F94(GameF3270SpawnSource *arg0, GameF3270SpawnOwner *arg1);

void func_150C5DC0(GameF3270ObjectOwner58 *arg0) {
    GameF3270B0State *temp;

    if (arg0->field_0x58 != 0) {
        temp = &arg0->field_0x58->field_0xB0;
        temp->field_0x4 = 1;
        done:
            ;
    } else {
        arg0->field_0x58 = func_150C6460(arg0);
    }
}

s32 func_150C5E0C(GameF3270Object *arg0) {
    s8 ret;
    GameF3270B0State *sub;

    ret = 1;
    sub = &arg0->field_0xB0;
    if (sub->field_0x4 == 0) {
        ret = 0;
    }
    sub->field_0x4 = 0;
    sub->field_0x14 -= D_800BE9E4;
    if (sub->field_0x14 < 0) {
        sub->field_0x14 = (func_150ADA20() % (u32)(sub->field_0x18 + 1)) + sub->field_0x16;
        sub->field_0x10 = func_150ADA68() * sub->field_0xC + sub->field_0x8;
    }
    arg0->field_0x24 += (s32)((sub->field_0x10 - (f32)arg0->field_0x24) * sub->field_0x1C);
    return ret;
}

extern s32 func_1513F6C0(void *arg0, s32 arg1, s32 arg2);

s32 func_150C5EFC(u8 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0xB0)) + 0x58);
    temp[0] = 0;
    *(s32 *)((u8 *)arg0 + 0x18) |= 2;
    *(u8 **)((u8 *)arg0 + 0xB0) = 0;
    func_1513F6C0(arg0, 0, 0);
    return 0;
}

void func_150C5F40(GameF3270SpawnOwner *arg0) {
    s32 temp_a2;
    GameF3270Child58State *temp;

    temp_a2 = arg0->field_0x18;
    if (arg0->field_0x5C != 0) {
        temp = &arg0->field_0x5C->field_0x58;
        temp->field_0x4 = 1;
        done:
            ;
    } else {
        arg0->field_0x5C = func_150C5F94((GameF3270SpawnSource *)temp_a2, arg0);
    }
}

void *memcpy(void *, const void *, u32);
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[0x3];
    /* 0x0C */ GameF3270SpawnSource *unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ u8 unk2D;
} GameF3270SpawnDesc;

typedef struct {
    /* 0x00 */ GameF3270SpawnOwner *owner;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ f32 unk8;
} GameF3270SpawnLink;

extern f32 D_800A0410;
extern f32 D_800A0414;
extern f32 D_800A0418;
extern s32 func_15045800(f32 *, s32, f32, f32 *);
extern GameF3270SpawnedObject *func_1513418C(GameF3270SpawnDesc *, s32, u8, s32);

GameF3270SpawnedObject *func_150C5F94(GameF3270SpawnSource *arg0, GameF3270SpawnOwner *arg1) {
    GameF3270SpawnDesc desc;
    GameF3270SpawnLink link;
    GameF3270SpawnedObject *result;

    link.owner = arg1;
    link.unk4 = 1;
    {
        f32 pos[3];

        pos[0] = arg0->field_0x14;
        pos[1] = arg0->field_0x18 + D_800A0410;
        pos[2] = arg0->field_0x1C;
        if (func_15045800(pos, 0, arg0->field_0x18 - 100.0f, &arg1->field_0x34) != 0) {
            link.unk8 = arg1->field_0x34;
        } else {
            link.unk8 = arg0->field_0x18 + D_800A0414;
        }
    }
    desc.unk0 = 0;
    desc.unk4 = 0;
    desc.unk8 = arg0->field_0x3B;
    desc.unkC = arg0;
    desc.unk10 = 0;
    desc.unk14 = 0.0f;
    desc.unk18 = 0.0f;
    desc.unk1C = 0.0f;
    desc.unk20 = 25.0f;
    desc.unk24 = D_800A0418;
    desc.unk28 = 300;
    desc.unk2A = 10;
    desc.unk2B = 4;
    desc.unk2C = 1;
    desc.unk2D = 2;
    result = func_1513418C(&desc, 0xC, 0xFF, 0);
    if (result != NULL) {
        memcpy(&result->field_0x58, &link, sizeof(link));
    }
    return result;
}


typedef struct GameF3270Burst {
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
} GameF3270Burst;

typedef struct GameF3270BurstMotion {
    f32 size;
    u8 mode0, mode1, count0, count1;
    f32 amount0, amount1, speed, damping;
    u8 mode2, mode3, count2, count3;
    f32 amount2, amount3, sourceScale;
} GameF3270BurstMotion;

void *func_151303BC(s32, u8, s32);
void *memcpy(void *, const void *, u32);
extern f32 D_800A041C, D_800A0420, D_800A0424, D_800A0428;
extern f32 D_800A042C, D_800A0430, D_800A0434, D_800A0438;

void func_150C60D8(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, f32 arg5, void *arg6) {
    GameF3270Burst burst;
    void *spawned;
    GameF3270BurstMotion motion;
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
    motion.amount0 = (func_150ADA68() * 0.25f + D_800A0420) * motion.size;
    motion.amount1 = (func_150ADA68() * 0.25f + D_800A0424) * motion.size;
    motion.count2 = func_150ADA20() % 5U + 4;
    motion.count3 = func_150ADA20() % 5U + 4;
    motion.amount2 = (func_150ADA68() * D_800A0428 + D_800A042C) * motion.size;
    motion.amount3 = (func_150ADA68() * D_800A0430 + D_800A0434) * motion.size;
    burst.rotation = (func_150ADA68() * 60.0f + 21.0f) * D_800A0438;
    spawned = func_151303BC((s32)&burst, 2, 0x28);
    if (spawned != NULL) memcpy((u8 *)spawned + 0xA8, &motion, 0x28);
}

s32 func_150C63EC(u8 *arg0) {
    s32 ret = 1;

    if (arg0[0x5C] == 0) {
        ret = 0;
    }
    arg0[0x5C] = 0;
    return ret;
}

void func_150C6410(struct102 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0x58)) + 0x58);
    temp[1] = 0;
    func_151346EC(arg0);
}

void func_150C6438(struct102 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0x58)) + 0x58);
    temp[1] = 0;
    func_1513470C(arg0);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_150C6460;

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
} Particle_150C6460;

typedef struct {
    u8 pad0[0x14];
    Vec3F_150C6460 pos;
    u8 pad20[0x1B4];
    s32 unk1D4;
} Actor_150C6460;

typedef struct {
    s32 field_0x00;
    u8 field_0x04;
    u8 pad_0x05[0x3];
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    s16 field_0x1A;
    f32 field_0x1C;
} State_150C6460;

extern Vec3F_150C6460 D_800887D0;
extern u8 D_800AB414[][3];
void func_15143134(Vec3F_150C6460 *, Vec3F_150C6460 *, s32);
u8 func_151D8E20(void);

GameF3270Object *func_150C6460(GameF3270ObjectOwner58 *arg0) {
    GameF3270Object *ret;
    Vec3F_150C6460 pos;
    Actor_150C6460 *actor;

    ret = NULL;
    actor = *(Actor_150C6460 **)((u8 *)arg0 + 0x18);
    if (actor->unk1D4) {
        Vec3F_150C6460 zero;
        zero = D_800887D0;
        func_15143134(&zero, &pos, actor->unk1D4);
    } else {
        pos.x = actor->pos.x;
        pos.y = actor->pos.y;
        pos.z = actor->pos.z;
    }
    {
    Vec3F_150C6460 probe;
    probe.x = pos.x;
    probe.y = pos.y + 200.0f;
    probe.z = pos.z;
    if (func_15045800((f32 *)&probe, 0, pos.y - 200.0f, (f32 *)((u8 *)arg0 + 0x34)) != 0) {
    Particle_150C6460 part;
    State_150C6460 state;
    f32 size;
    u8 color;
    s32 rand2;
    s32 rand1;
    s32 rand0;
        color = func_151D8E20();
        probe.y = *(f32 *)((u8 *)arg0 + 0x34);
        size = func_150ADA68() * 50.0f + 100.0f;
        state.field_0x00 = (s32)arg0;
        state.field_0x04 = 1;
        state.field_0x08 = 26214.40039f;
        state.field_0x0C = 32768.0f;
        state.field_0x10 = 0.0f;
        state.field_0x14 = 0;
        state.field_0x16 = 10;
        state.field_0x18 = 10;
        state.field_0x1C = 0.2310000062f;
        part.unkC = (s32)(state.field_0x0C * 0.5f + state.field_0x08);
        part.unk6 = 0x38;
        part.unk0 = 0x20300;
        part.unk4 = 0x12C;
        part.unk10 = 0x96;
        part.unk8 = 0;
        part.unk11 = 0xFF;
        part.unk12 = D_800AB414[color][0];
        part.unk13 = D_800AB414[color][1];
        part.unk14 = D_800AB414[color][2];
        part.unk15 = 0xFF;
        part.unk18 = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        rand0 = func_150ADA20();
        rand1 = func_150ADA20();
        rand2 = func_150ADA20();
        ret = (GameF3270Object *)func_1513C73C((s32)&part, 8, 2, (s32)((u8 *)arg0 + 0x38), probe.x, probe.y, probe.z, size, size,
                            rand0 & 0xFF, ((rand2 & 1) * 2) + (rand1 & 1), 0x20,
                            *((u8 *)arg0 + 0xC), *((u8 *)arg0 + 1));
        if (ret != NULL) {
            memcpy(&ret->field_0xB0, &state, sizeof(state));
        }
    }
    }
    return ret;
}
