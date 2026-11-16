#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8proto
#define func_151D9B8C func_151D9B8C_s32proto
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

extern u8 D_800A3FE6[];
extern u8 D_800A4058[];
extern u8 D_800A4068[];
extern f32 D_800A45B0;
extern f32 D_800A482C;
extern f32 D_800A4830;
extern f32 D_800A4834;
extern f32 D_800A4838;
extern f32 D_800A483C;
extern f32 D_800A4840;
extern void func_15153F18(void *, void *, s32, u8, s32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_15143134(void *, s32, s32);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);
extern void func_151BC5A4(struct260 *, s32, u8);

typedef struct {
    s32 unk0[4];
} Struct15136698Table;

extern Struct15136698Table D_80089BAC;

// requires jump table
s32 func_15134070(void *arg0) {
    switch (*((u8 *)arg0 + 4)) {
    case 135:
    case 176:
        return 0xE;
    case 177:
        return 0xF;
    case 180:
        return 0x10;
    case 112:
    case 178:
        return 0x11;
    case 0xAB:
        return 0x12;
    case 16:
    case 56:
    case 145:
        return 0xD;
    case 152:
        return 0xC;
    case 52:
        return 0xB;
    case 71:
        return 0xA;
    case 17:
    case 20:
    case 59:
        return 1;
    case 90:
    case 95:
    case 116:
    case 117:
    case 122:
        return 2;
    case 88:
    case 91:
    case 128:
        return 3;
    case 22:
    case 136:
    case 144:
        return 4;
    case 156:
        return 5;
    case 157:
        return 6;
    case 150:
        return 7;
    case 160:
        return 8;
    case 159:
        return 9;
    case 0x9A:
        return 0x13;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return 0;
    default:
        return 0x63;
    }
}

s32 func_1513416C(struct102 *arg0) {
    s16 temp_v0 = arg0->unk1C;
    if (temp_v0 < 32) {
        arg0->unk28 = temp_v0 * 8;
    }
    return 1;
}

typedef struct {
    u8 pad0[0x10];
    u8 pad10[0x8];
    u8 unk18;
    u8 pad19[0x3];
    struct127 *unk1C;
    u8 unk20;
    u8 pad21[0x3];
    u8 unk24[0xC];
    f32 unk30;
    u8 pad34[0x6];
    u8 unk3A;
    u8 pad3B[0x5];
    u8 unk40[0xC];
    f32 unk4C;
    f32 unk50;
} Struct1513418C;

void *func_1513418C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct1513418C *temp_v0;
    struct127 *temp_v0_2;
    s32 temp_v1;
    s32 temp_a2;
    u8 temp_a0;
    Struct1513418C *sp24;

    temp_v0 = func_15167A68(0x28, arg3, arg1 + 0x58, 1, (u8)arg2, 1);
    if (temp_v0 == NULL) {
        return NULL;
    }
    sp24 = temp_v0;
    memcpy(&temp_v0->pad10, arg0, 0x30);
    temp_a0 = temp_v0->unk3A;
    if (temp_a0 & 2) {
        temp_v0_2 = temp_v0->unk1C;
        if ((temp_v0_2->interaction_state == 0) || (temp_v0_2->unique_id != temp_v0->unk18)) {
            func_1516972C((struct102 *)temp_v0);
            return NULL;
        }
        temp_v1 = (s32)temp_v0_2->unk1D4;
        if ((temp_v1 != 0) && ((temp_v0_2->unk74 & 0xF) != 0xF)) {
            sp24 = temp_v0;
            temp_a2 = temp_v1;
            temp_a2 += temp_v0->unk20 << 6;
            func_15143134(temp_v0->unk24, (s32)temp_v0->unk40, temp_a2);
        } else {
            temp_v0->unk3A = temp_a0 | 8;
        }
    } else {
        temp_v0->unk3A = temp_a0 | 0x18;
    }
    temp_v0->unk4C = 1.0f / (temp_v0->unk30 + temp_v0->unk30);
    temp_v0->unk50 = 0.0f;
    return temp_v0;
}

typedef struct {
    f32 x, y, z;
} BloodDripVec3f;

typedef struct {
    u8 pad0[0x10];
    s32 target[2];
    u8 field18;
    u8 pad19[3];
    void *field1C;
    u8 field20;
    u8 pad21[3];
    f32 offset[3];
    f32 radius;
    f32 rate;
    s16 lifetime;
    u8 flags;
    u8 emitter;
    s8 callback;
    u8 pad3D[3];
    BloodDripVec3f previous;
    f32 inverseDiameter;
    f32 accumulator;
} BloodDrip;

extern void (*D_80089A70[])(f32, f32, f32, f32, f32, f32, void *);
extern s32 (*D_80089AA0[])(void *);

void func_151342BC(BloodDrip *drip) {
    f32 *source;
    u8 dead;
    u8 flags;
    f32 position[3];
    s32 target;
    u8 *bones;
    f32 deltaZ;
    f32 deltaY;
    f32 deltaX;
    f32 randomX;
    f32 randomY;
    s32 index;
    f32 scale;
    f32 distance;

    dead = 0;
    if (drip->flags & 4) {
        drip->lifetime -= D_800BE9E4;
        if (drip->lifetime < 0) {
            dead = 1;
        }
    }
    flags = drip->flags;
    source = (flags & 8) ? position : &drip->previous.x;
    if (flags & 1) {
        position[0] = *(f32 *)((u8 *)drip->field1C + 0x14);
        position[1] = *(f32 *)((u8 *)drip->field1C + 0x18);
        position[2] = *(f32 *)((u8 *)drip->field1C + 0x1C);
    } else {
        if (flags & 2) {
            if ((*(s32 *)drip->field1C == 0) || (*(s32 *)drip->field1C == 8) || (drip->field18 != *((u8 *)drip->field1C + 0x3B))) {
                func_1516972C((struct102 *)drip);
                return;
            }
            bones = *(u8 **)((u8 *)drip->field1C + 0x1D4);
            if ((bones == NULL) || ((*((u8 *)drip->field1C + 0x74) & 0xF) == 0xF)) {
                drip->flags = flags | 8;
                return;
            }
            target = (s32)((u8 (*)[0x40])bones)[drip->field20];
        } else if (!(flags & 0x10)) {
            index = D_800BE9C0 ? 0 : 1;
            target = drip->target[index];
        } else {
            return;
        }
        func_15143134(drip->offset, (s32)position, target);
    }
    deltaX = source[0] - position[0];
    deltaY = source[1] - position[1];
    deltaZ = source[2] - position[2];
    distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    scale = drip->inverseDiameter * distance + 1.0f;
    drip->accumulator += scale * D_800BE9A4 * drip->rate;
    if (drip->accumulator > 7.0f) {
        drip->accumulator = 7.0f;
    }
    if ((drip->flags & 0x20) && (D_800C35EA == 1)) {
        while (drip->accumulator > 1.0f) {
            drip->accumulator -= 1.0f;
        }
    } else {
        while (drip->accumulator > 1.0f) {
            f32 diameter;
            f32 fraction;

            diameter = drip->radius + drip->radius;
            fraction = func_150ADA68();
            if (D_80089A70[drip->emitter] != NULL) {
                randomX = func_150ADA68();
                randomY = func_150ADA68();
                D_80089A70[drip->emitter](
                    (position[0] + deltaX * fraction + drip->radius) - randomX * diameter,
                    (position[1] + deltaY * fraction + drip->radius) - randomY * diameter,
                    (position[2] + deltaZ * fraction + drip->radius) - func_150ADA68() * diameter,
                    deltaX, deltaY, deltaZ, drip);
            }
            drip->accumulator -= 1.0f;
        }
    }
    drip->previous = *(BloodDripVec3f *)position;
    drip->flags &= ~8;
    if (!dead) {
        if ((drip->callback != -1) && (D_80089AA0[drip->callback](drip) == 0)) {
            dead = 1;
        }
    }
    if (dead) {
        func_1516972C((struct102 *)drip);
    }
}

s32 func_151346D0(s32 arg0, struct102 *arg1, s32 arg2) {
    arg1->unk3A &= 0xFFEF;
    return arg0;
}

void func_151346EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513470C(struct102 *arg0) {
    func_15169824(arg0);
}

void func_1513472C(struct102 *arg0) {
    s32 idx = arg0->unk3D;
    if (idx < 0) {
        idx = 0;
    }
    if (idx >= 10) {
        idx = 0;
    }
    D_80089AAC[idx]();
}

void func_1513477C(struct102 *arg0) {
  s32 idx = arg0->unk3D;
  if (idx < 0) {
      idx = 0;
  }
  if (idx >= 10) {
      idx = 0;
  }
    D_80089AD4[idx]();
}

typedef struct {
    u8  pad0[0x18];
    u8  unk18;
    u8  pad19[0x3];
    s32 unk1C;
    u8  pad20[0x1D];
    u8  unk3D;
} SubA_151347CC;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151347CC;

void func_151347CC(void *arg0, s32 arg1, u8 arg2) {
    SubA_151347CC *a = (SubA_151347CC *)arg0;
    ArgB_151347CC *b = (ArgB_151347CC *)arg1;
    s32 t;

    if ((arg2 == 0) || (arg2 == 0x3)) {
        if ((b->unk0 == a->unk1C) || (b->u4.b4 == a->unk18)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x11) {
        if (a->unk3D == 0x5) {
            if ((b->unk0 == a->unk1C) || (b->u4.b4 == a->unk18)) {
                func_1516972C((struct102 *)arg0);
            }
        }
    } else if (arg2 == 0x16) {
        if (arg1 == a->unk1C) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        t = b->unk0;
        if (a->unk1C == t) {
            a->unk1C = b->u4.w4;
            a->unk18 = b->unk9;
        } else {
            if (a->unk1C == b->u4.w4) {
                a->unk1C = t;
                a->unk18 = b->unk8;
            }
        dummy_label_151347CC: ;
        }
    }
}

void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Struct15134908 {
    u8 pad0[0x10];
    f32 *unk10;
    f32 *unk14;
    f32 *unk18;
    f32 unk1C;
    u8 pad20[0xC];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
};

void *func_15134908(void *arg0, s32 arg1, u8 arg2, s32 arg3) {
    struct Struct15134908 *temp_v0;
    struct Struct15134908 *sp24;

    temp_v0 = (struct Struct15134908 *)func_15167A68(0x2A, arg3, arg1 + 0x40, 1, arg2, 1);
    if (temp_v0 == 0) {
        return NULL;
    }
    *(u8 *)((s32)arg0 + 0x16) |= 2;
    sp24 = temp_v0;
    memcpy((s32)temp_v0 + 0x10, arg0, 0x1C);
    sp24->unk2C = *sp24->unk10;
    sp24->unk30 = *sp24->unk14;
    sp24->unk34 = *sp24->unk18;
    sp24->unk38 = (f32)(1.0f / (sp24->unk1C + sp24->unk1C));
    sp24->unk3C = 0.0f;
    return sp24;
}

typedef struct {
    u8 pad0[0x10];
    f32 *x;
    f32 *y;
    f32 *z;
    f32 radius;
    f32 rate;
    s16 lifetime;
    u8 flags;
    u8 emitter;
    u8 field28;
    s8 callback;
    u8 pad2A[2];
    BloodDripVec3f previous;
    f32 inverseDiameter;
    f32 accumulator;
} BloodTrail;

extern s32 (*D_80089B18[])(void *);
extern void (*D_80089AFC[])(f32, f32, f32, f32, f32, f32, void *);

void func_151349D0(BloodTrail *trail) {
    u8 flags;
    f32 position[3];
    f32 deltaZ;
    f32 deltaY;
    f32 deltaX;
    f32 randomX;
    f32 randomY;

    position[0] = *trail->x;
    position[1] = *trail->y;
    position[2] = *trail->z;
    if (trail->flags & 4) {
        if ((trail->callback != -1) && (D_80089B18[trail->callback](trail) == 0)) {
            func_1516972C((struct102 *)trail);
            return;
        }
    }
    flags = trail->flags;
    if (flags & 2) {
        trail->flags = flags & ~2;
    } else {
        f32 scale;
        f32 distance;

        deltaX = trail->previous.x - position[0];
        deltaY = trail->previous.y - position[1];
        deltaZ = trail->previous.z - position[2];
        distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
        scale = trail->inverseDiameter * distance + 1.0f;
        trail->accumulator += scale * D_800BE9A4 * trail->rate;
        if (trail->accumulator > 7.0f) {
            trail->accumulator = 7.0f;
        }
        while (trail->accumulator > 1.0f) {
            f32 diameter;
            f32 fraction;

            diameter = trail->radius + trail->radius;
            fraction = func_150ADA68();
            if (D_80089AFC[trail->emitter] != NULL) {
                randomX = func_150ADA68();
                randomY = func_150ADA68();
                D_80089AFC[trail->emitter](
                    (position[0] + deltaX * fraction + trail->radius) - randomX * diameter,
                    (position[1] + deltaY * fraction + trail->radius) - randomY * diameter,
                    (position[2] + deltaZ * fraction + trail->radius) - func_150ADA68() * diameter,
                    deltaX, deltaY, deltaZ, trail);
            }
            trail->accumulator -= 1.0f;
        }
    }
    trail->previous = *(BloodDripVec3f *)position;
    if (trail->flags & 1) {
        trail->lifetime -= D_800BE9E4;
        if (trail->lifetime < 0) {
            func_1516972C((struct102 *)trail);
        }
    }
}

void func_15134C98(struct260 *arg0, s32 arg1, u8 arg2) {
    if (((struct102 *)arg0)->unk28 == 1) {
        func_151BC5A4(arg0, arg1, arg2);
    }
}

void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0xE];
    u8 unk2E;
    u8 pad2F[0x41];
    f32 unk70;
    f32 unk74;
} Struct15134CEC;

s32 func_15134CEC(Struct15134CEC *arg0) {
    s32 temp_v1;

    arg0->unk70 += 0.125f * D_800BE9A4;
    temp_v1 = arg0->unk2E;
    arg0->unk74 += D_800A45B0 * D_800BE9A4;
    arg0->unk14 += arg0->unk70 * D_800BE9A4;
    arg0->unk1C += arg0->unk74 * D_800BE9A4;
    if (arg0->unk14 > 130.0f) {
        return 0;
    }
    temp_v1 -= D_800BE9E4 * 2;
    if (temp_v1 < 0) {
        return 0;
    }
    arg0->unk2E = temp_v1;
    return 1;
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

void *func_15134DAC(void *arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = func_15167A68(0x29, 0, arg1 + 0x80, 1, 0xFF, 1);
    if (temp_v1 == 0) {
        return NULL;
    }
    memcpy((s32)temp_v1 + 0x18, arg0, 0x3C);
    *(s16 *)((s32)temp_v1 + 0x54) = -(*(s16 *)((s32)arg0 + 0x28));
    *(s32 *)((s32)temp_v1 + 0x10) = 1;
    *(s32 *)((s32)temp_v1 + 0x14) = 0;
    *(f32 *)((s32)temp_v1 + 0x70) = 0.0f;
    *(f32 *)((s32)temp_v1 + 0x74) = 0.0f;
    *(f32 *)((s32)temp_v1 + 0x78) = 0.0f;
    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134E48.s")

void func_151352EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513530C(struct102 *arg0) {
    func_15169824(arg0);
}

extern void (*D_80089B70[])(void *);

typedef struct {
    u8 pad0[0x44];
    u16 unk44;
    u8 pad46[0xA];
    u8 unk50;
} struct1513532C;

void func_1513532C(struct1513532C *arg0) {
    struct1513532C *p = arg0;
    s32 idx;

    idx = p->unk50;
    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    if (p->unk44 != 0) {
        func_100111C8(p->unk44);
        p->unk44 = 0;
    }
    D_80089B70[idx](p);
}

extern void (*D_80089B88[])(void *);

void func_151353A8(struct1513532C *arg0) {
    struct1513532C *p = arg0;
    s32 idx;

    idx = p->unk50;
    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    if (p->unk44 != 0) {
        func_100111C8(p->unk44);
        p->unk44 = 0;
    }
    D_80089B88[idx](p);
}

extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_15135424(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp20[2];
    s32 sp18[2];

    sp20[0] = arg1;
    sp20[1] = arg2;
    sp18[0] = arg3;
    sp18[1] = arg4;
    func_15145EA4(sp20, sp18, arg0, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135480.s")

struct arg1_1513555C {
    s32 unk0;
    u8 unk4;
};

void func_1513555C(struct102 *arg0, struct arg1_1513555C *arg1, u8 arg2) {
    s32 a = arg2;
    if (a == 0 || a == 0x12) {
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            func_1516972C(arg0);
        }
    }
}

void func_151355B8(struct102 *arg0, struct arg1_1513555C *arg1, u8 arg2) {
    switch (arg2) {
    case 0:
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            func_1516972C(arg0);
        }
        break;
    case 3:
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            s32 *p = (s32 *)((u8 *)arg0 + 0x10);
            *p &= ~1;
            *p = *p;
        }
        break;
    }
}

s32 func_15135658(void *arg0) {
    *(f32 *)((u8 *)arg0 + 0x74) = 1.0f;
    return 1;
}

f32 func_15135670(s32 arg0) {
    // "power", "../Effects/Blood/blood.c"
    return func_151422DC(0, &D_800A3FB4, 0, 2000, 1000, &D_800A3FBC, 2938) * D_800A45B4;
}

void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
typedef struct {
    f32 x, y;
} Blood356D4Position;

typedef struct {
    Blood356D4Position position;
    f32 width, height;
    u8 texture;
    u8 pad11;
    s16 lifetime;
    u16 flags;
    s16 duration, period;
    u8 mode, red, green, blue, alpha, maximum;
    u8 endRed, endGreen, endBlue, endAlpha;
    s32 zero24, zero28, zero2C;
    s32 field30, field34, field38, field3C;
    u8 field40, field41;
    u8 pad42[0x16];
} Blood356D4Particle;

typedef struct {
    s16 owner;
    f32 growth, velocityX, velocityY, field10, field14;
} Blood356D4Motion;

typedef struct {
    u8 value[2];
} Blood356D4Kinds;

extern Blood356D4Kinds D_80089BA0;
extern s32 D_80089BA4;
extern Blood356D4Kinds D_80089BA8;

void *func_151356D4(Blood356D4Position *arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9, u8 arg10, s32 arg11) {
    void *result;
    Blood356D4Particle particle;
    Blood356D4Motion motion;
    Blood356D4Kinds kinds;
    s32 data;
    f32 scale;
    f32 count;
    Blood356D4Particle spark;
    Blood356D4Kinds sparkKinds;
    s32 flip;

    kinds = D_80089BA0;
    data = D_80089BA4;
    motion.owner = arg6;
    motion.growth = (func_150ADA68() * 100.0f + 100.0f) * arg1 * 0.001f;
    motion.velocityX = func_150ADA68() * 6.0f * arg0->x * 0.001f;
    motion.velocityY = func_150ADA68() * 6.0f * arg0->y * 0.001f;
    motion.field10 = arg2;
    motion.field14 = arg3;
    particle.position = *arg0;
    particle.width = arg1;
    particle.height = arg1;
    particle.texture = kinds.value[func_150ADA20() & 1];
    particle.lifetime = arg7;
    flip = (func_150ADA20() & 1) ? 2 : 0;
    particle.flags = ((particle.texture == 0x3A) ? ((func_150ADA20() & 1) ? 4 : 0) : 0) | 9 | flip;
    particle.duration = arg8;
    particle.period = arg9;
    particle.mode = 2;
    particle.red = 0xFF;
    particle.green = 0xFF;
    particle.blue = 0xFF;
    particle.maximum = 0xFF;
    particle.endRed = 0xFF;
    particle.endGreen = 0xFF;
    particle.endBlue = 0xFF;
    particle.zero24 = 0;
    particle.zero28 = 0;
    particle.zero2C = 0;
    particle.field40 = 0;
    particle.field41 = 9;
    particle.field30 = 7;
    particle.field34 = 0x3C;
    particle.field38 = 0x80;
    particle.field3C = 0x20;
    particle.alpha = arg4;
    particle.endAlpha = arg5;
    result = func_1515548C(&particle, 3, (s32)&data, 1, 0x18, arg10, arg11);
    if (result != NULL) {
        memcpy((u8 *)result + 0x70, &motion, 0x18);
        scale = (particle.width + particle.height) * 0.5f;
        count = (func_150ADA68() * 300.0f + 100.0f) * arg1 * 0.001f;
        spark.lifetime = 0x32;
        spark.flags = 9;
        spark.duration = 0x10;
        spark.period = 0xF;
        spark.mode = 3;
        spark.red = 0xFF;
        spark.green = 0xFF;
        spark.blue = 0xFF;
        spark.alpha = arg4;
        spark.maximum = 0xFF;
        spark.endRed = 0xFF;
        spark.endGreen = 0xFF;
        spark.endBlue = 0xFF;
        spark.endAlpha = arg5;
        spark.zero24 = 0;
        spark.zero28 = 0;
        spark.zero2C = 0;
        spark.field40 = 0;
        spark.field41 = 9;
        spark.field30 = 7;
        spark.field34 = 0x3C;
        spark.field38 = 0x40;
        spark.field3C = 0;
        spark.width = (func_150ADA68() * 120.0f + 80.0f) * (arg1 * scale) * 0.0001f;
        spark.height = (func_150ADA68() * 120.0f + 80.0f) * (arg1 * scale) * 0.0001f;
        while (count > 1.0f) {
            sparkKinds = D_80089BA8;
            {
                s16 angle;
                f32 cosine;
                f32 sine;
                f32 distance;

                angle = func_150ADA20() & 0xFF;
                cosine = func_151423D8(angle - 0x40);
                sine = func_151423D8(angle);
                distance = (func_150ADA68() * 300.0f + 1300.0f) * scale * 0.001f;
                spark.position.x = arg0->x + distance * sine;
                spark.position.y = arg0->y + distance * cosine;
            }
            spark.texture = sparkKinds.value[func_150ADA20() & 1];
            spark.flags &= ~6;
            if (func_150ADA20() & 1) {
                spark.flags |= 2;
            }
            if (func_150ADA20() & 1) {
                spark.flags |= 4;
            }
            func_1515548C(&spark, 0, 0, 0, 0, arg10, arg11);
            count -= 1.0f;
        }
    }
    return result;
}


typedef struct {
    f32 x;
    f32 y;
} Vec2_15135BF8;

typedef struct {
    s16 timer;
    f32 unk4;
    Vec2_15135BF8 vel;
    f32 unk10;
    f32 unk14;
} Sub_15135BF8;

typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x50];
    s16 unk70;
} Obj_15135BF8;

s32 func_15135BF8(Obj_15135BF8 *arg0) {
    Sub_15135BF8 *sub;
    s32 ret;

    sub = (Sub_15135BF8 *)&arg0->unk70;
    ret = 1;
    if (arg0->unk70 > 0) {
        sub->timer -= D_800BE9E4;
        arg0->unk10 += sub->vel.x * D_800BE9A4;
        arg0->unk14 += sub->vel.y * D_800BE9A4;
        arg0->unk18 += sub->unk4 * D_800BE9A4;
        arg0->unk1C += sub->unk4 * D_800BE9A4;
    } else {
        Vec2_15135BF8 tmp;

        tmp = sub->vel;
        arg0->unk14 += sub->vel.y * D_800BE9A4 + sub->unk10 * D_800BE9A4 * D_800BE9A4 * 0.5f;
        sub->vel.y += sub->unk10 * D_800BE9A4;
        tmp.x += sub->vel.x;
        tmp.y += sub->vel.y;
        tmp.x *= 0.5f;
        tmp.y *= 0.5f;
        {
            f32 f = fabsf(tmp.x) * sub->unk14;

            arg0->unk18 += f * D_800BE9A4;
            arg0->unk1C += f * D_800BE9A4;
        }
    }
    if (arg0->unk10 > 200.0f || arg0->unk10 < -200.0f || arg0->unk14 > 200.0f || arg0->unk14 < -200.0f) {
        ret = 0;
    }
    return ret;
}

typedef struct { f32 x, y, z; } V15135DD0;
typedef struct { f32 a; s32 pad[5]; s32 g; u8 h; u8 i; s32 j; } S15135DD0;
extern s32 func_15046460(V15135DD0 *, s32, s32, S15135DD0 *);
extern void func_151DB5D0(u8, void *, V15135DD0 *, f32, f32, f32, f32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151DA6F8(void *, V15135DD0 *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s32, s32, s32, u8, s32);
extern void func_151D9014(void *, void *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
extern void func_15143794(s16, s16, f32, f32 *);

void func_15135DD0(V15135DD0 *arg0, V15135DD0 *arg1, s32 arg2, u8 arg3, s32 arg4) {
    V15135DD0 sp13C;
    f32 x0;
    V15135DD0 sp12C;
    S15135DD0 sp108;
    u32 fp;
    V15135DD0 spF8;
    s32 i;
    f32 a, b;
    V15135DD0 spE0;
    f32 c;
    s32 s0;
    V15135DD0 spCC;
    s32 i1;

    fp = arg3;
    sp108.g = 0;
    sp108.h = 0;
    sp108.i = 0;
    sp108.j = 0;
    sp108.a = -10000.0f;
    sp13C.x = arg1->x - (x0 = arg0->x);
    sp13C.y = arg1->y - arg0->y;
    sp13C.z = arg1->z - arg0->z;
    sp12C.x = x0;
    sp12C.y = 10000.0f;
    sp12C.z = arg0->z;
    if (func_15046460(&sp12C, 0, *(s32 *)&arg0->y, &sp108) != 0 && sp108.i == 3) {
        i1 = func_150ADA20() % 11U + 10;
        while (i1 > 0) {
            a = (func_150ADA68() * 67.0f + 86.0f) * 0.001000000047f;
            spF8.x = sp13C.x * a;
            spF8.y = sp13C.y * a;
            spF8.z = sp13C.z * a;
            s0 = (s16)(func_150ADA20() % 52U + 99);
            c = func_150ADA68();
            a = func_150ADA68();
            b = func_150ADA68();
            func_151DB5D0(0, arg0, &spF8, c * 50.0f + 30.0f, a * 0 + 1.00999999f, 1.0f, b * 0.04100000113f, s0, func_150ADA20() % 156U + 100, s0, 255 / s0, 1, arg3, arg4);
            i1--;
        }
    } else {
        i = func_150ADA20() % 5U + 3;
        while (i > 0) {
            b = (func_150ADA68() * 618.0f + 201.0f) * 0.001000000047f;
            func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, *(f32 *)&arg2, &spE0.x);
            spE0.x += sp13C.x * b;
            spE0.y += sp13C.y * b;
            spE0.z += sp13C.z * b;
    {
    u32 x;
            a = func_150ADA68();
            x = func_150ADA20();
            s0 = func_150ADA20();
            b = func_150ADA68();
            func_151DA6F8(arg0, &spE0, a * 1.013000011f, (u32)x % 21U + 20, (u32)s0 % 101U + 100, b * 4.200000286f + 1.899999976f, func_150ADA20() % 3U + 3, 0, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0, arg3, arg4);
    }
            i--;
        }
    {
    u32 y;
        i = func_150ADA20() % 11U + 10;
        while (i > 0) {
            b = func_150ADA68() * 200.0f * 0.001000000047f;
            func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, *(f32 *)&arg2, &spCC.x);
            spCC.x += sp13C.x * b;
            spCC.y += sp13C.y * b;
            spCC.z += sp13C.z * b;
    {
    u32 x;
            a = func_150ADA68();
            x = func_150ADA20();
            y = func_150ADA20();
            func_151D9014(arg0, &spCC, 0, a * 0.1469999999f + -0.302000016f, x % 52U + 31, y % 101U + 155, func_150ADA68() * 50.0f + 50.0f, 0, 1.0f, 1.0f, 0, 0, 1, 0, fp, arg4);
    }
            i--;
        }
    }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136404.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Struct15136698Vec;

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Struct15136698Vec unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 pad46[0x12];
} Struct15136698Local;

s32 func_15136698(f32 arg0, f32 arg1, u8 arg2, u8 arg3, s16 arg4, s32 arg5, Struct15136698Vec *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10) {
    s32 ret;
    Struct15136698Local sp7C;
    f32 sp78;
    Struct15136698Table sp68;
    s32 sp64;
    s32 sp60;
    s32 temp_v0;
    s32 sp54;
    s32 sp50;

    sp68 = D_80089BAC;
    sp78 = arg1;
    sp7C.unk0 = sp68.unk0[func_150ADA20() & 3];
    if (arg7 != 0) {
        temp_v0 = 2;
    } else {
        temp_v0 = 1;
    }
    sp7C.unk2 = temp_v0 + 0x300;
    sp7C.unk8 = 0;
    sp7C.unkC = 0;
    sp7C.unk10 = 0;
    sp7C.unk11 = 0;
    sp7C.unk12 = 0;
    sp7C.unk13 = 0xFF;
    sp7C.unk18 = arg0;
    sp7C.unk14 = arg0;
    sp7C.unk1C = *arg6;
    sp7C.unk40 = 0;
    sp7C.unk28 = 0.0f;
    sp7C.unk2C = 0.0f;
    sp7C.unk30 = 0.0f;
    sp7C.unk34 = 1.0f;
    sp7C.unk38 = 1.0f;
    sp7C.unk3C = 1.0f;
    if (arg4 == -1) {
        sp7C.unk4 = 0x12C;
    } else {
        sp7C.unk4 = arg4 + 0x20;
        sp7C.unk40 = 1;
    }
    sp7C.unk44 = arg2;
    sp7C.unk45 = arg3;
    if (arg8 != 0) {
        sp64 = 3;
        sp60 = 0xFF;
    } else {
        sp64 = 0;
        sp60 = 0;
    }

    sp50 = func_150ADA20();
    sp54 = func_150ADA20();
    temp_v0 = func_150ADA20();
    ret = func_1513D594((s32)&sp7C, 0, 0, 0x1A, 0, (sp54 & 1) + (sp50 & 1), temp_v0 & 0xFF, 500.0f, 500.0f, 0, arg5, sp64, sp60, 0, 4, arg9, arg10);
    if (ret != 0) {
        memcpy(ret + 0x128, &sp78, sizeof(sp78));
    }
    return ret;
}

extern f32 D_800BE9A4;

s32 func_151368A8(struct102 *arg0) {
    s32 v0 = *(s16 *)((u8 *)arg0 + 0x1C);
    f32 *v1 = (f32 *)((u8 *)arg0 + 0x128);

    if (v0 < 0x20) {
        s32 sv = v0 << 3;
        if (sv < *(u8 *)((u8 *)arg0 + 0x5C)) {
            *(u8 *)((u8 *)arg0 + 0x5C) = sv;
        }
    }
    *(f32 *)((u8 *)arg0 + 0x2C) += *v1 * D_800BE9A4;
    *(f32 *)((u8 *)arg0 + 0x30) += *v1 * D_800BE9A4;
    return 1;
}

struct LocalDef15136918 {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    s8  unk7;
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    s8  unk16;
    s8  unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

void func_15136918(f32 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4, s32 arg5, f32 *arg6, u8 arg7, s32 arg8) {
    struct LocalDef15136918 tmp;
    f64 spPad;

    spPad = arg0;
    tmp.unk6 = 0x55;
    tmp.unk0 = 0x300;
    tmp.unk8 = 0;
    tmp.unkC = 0;
    tmp.unk10 = arg1;
    tmp.unk11 = arg2;
    tmp.unk12 = 0;
    tmp.unk13 = 0;
    tmp.unk14 = 0;
    tmp.unk15 = 0;
    tmp.unk18 = 0x280001;
    tmp.unk16 = 1;
    tmp.unk17 = 1;
    if (arg3 == -1) {
        tmp.unk4 = 0x12C;
    } else {
        tmp.unk0 = 0x301;
        tmp.unk4 = arg3 + 0x20;
    }
    func_1513C73C((s32)&tmp, 0xD, 0, arg5, arg6[0], arg6[1], arg6[2], arg0, arg0, arg4, 0, 0, arg7, arg8);
}

s32 func_15136A1C(struct102 *arg0) {
    s16 temp_v0 = arg0->unk1C;
    s32 temp_v1;

    if (temp_v0 < 0x20) {
        temp_v1 = temp_v0 << 3;
        if (temp_v1 < arg0->unk28) {
            arg0->unk28 = temp_v1;
        }
    }
    return 1;
}

extern f32 D_800A461C;
extern f32 D_800A4620;
extern void *func_15134908(void *, s32, u8, s32);

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 unk19;
} Struct15136A50;

void func_15136A50(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, s32 arg5) {
    Struct15136A50 sp1C;

    sp1C.unk0C = D_800A461C;
    sp1C.unk00 = arg0;
    sp1C.unk04 = arg1;
    sp1C.unk08 = arg2;
    sp1C.unk10 = D_800A4620;
    sp1C.unk14 = arg3;
    sp1C.unk16 = 5;
    sp1C.unk17 = 5;
    sp1C.unk18 = 2;
    sp1C.unk19 = -1;
    func_15134908(&sp1C, 0, arg4, arg5);
}

extern f32 D_800A4624;
extern f32 D_800A4628;
extern f32 D_800A462C;
void func_151D9014(void *a0, void *a1, s32 a2, f32 a3, s32 sp10, s32 sp14,
                     f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C,
                     s32 sp30, s32 sp34, s32 sp38, s32 sp3C);

void func_15136AE4(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, u8 *a6) {
    f32 sp6C[3];
    f32 sp60[3];
    f32 r;
    struct { f32 c; s32 a; u32 b; s32 pad; } sp4C;

    sp6C[0] = a0;
    sp6C[1] = a1;
    sp6C[2] = a2;
    r = (func_150ADA68() * 112.0f + 247.0f) * D_800A4624;
    sp60[0] = -a3 * r;
    sp60[1] = -a4 * r;
    sp60[2] = -a5 * r;
    sp4C.c = func_150ADA68();
    sp4C.a = func_150ADA20();
    sp4C.b = func_150ADA20();
    func_151D9014(sp6C, sp60, 0, sp4C.c * D_800A4628 + D_800A462C,
                    (sp4C.a & 0xF) + 25, (sp4C.b % 101) + 155,
                    func_150ADA68() * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    a6[0xC], a6[1]);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_36C3C;

typedef struct Blood36C3CActor {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x147];
    f32 unk14C;
    f32 unk150;
} Blood36C3CActor;

typedef struct {
    f32 values[4];
} Blood1380B4Vector;

typedef struct Blood36C3CHit {
    f32 height;
    u8 geometry[0x20];
} Blood36C3CHit;

typedef struct Blood36C3CSpray {
    s32 countBase;
    s32 countRange;
    Vec3f_36C3C origin;
    s16 angleBase;
    s16 angleRange;
    s16 pitchBase;
    s16 pitchRange;
    f32 maxDistance;
    f32 sizeBase;
    f32 sizeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 kind;
} Blood36C3CSpray;

s32 func_15134070();
void func_1504715C();
void func_1515C244(void *, void *, f32 *, f32 *);
void func_1513A24C(Vec3f_36C3C *, s32 *, f32 *, s32, f32, u8, u8, u8 *, void *,
                   u8, s32);
void func_15150D1C(Blood36C3CSpray *, u8, s32);
void func_151A9834(Vec3f_36C3C *, f32, f32, void *, s32, u8, s8 *, u8, s32);
void func_151C61A0(s32, s32, u8, s32);
void func_1513A48C(void *, u8, s32);
extern s32 *D_80089A20[];
extern f32 D_800A3C30;
extern s32 D_800A3F14[];
extern s32 D_800A3F64[];
extern u8 D_800A3FD8[];
extern Blood1380B4Vector D_800A3FF8;
extern Blood1380B4Vector D_800A40C8;
extern Blood1380B4Vector D_800A40F8;
extern f32 D_800A4630;

#define BLOOD_TBL ((Blood1380B4Vector *)D_800A3FD8)

void func_15136C3C(Blood36C3CActor *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4,
                   u8 arg5, u8 arg6, s32 arg7) {
    Vec3f_36C3C position;
    f32 unused0;
    f32 unused1;
    f32 halfHeight;
    Blood36C3CHit hit;
    s32 kind;
    Blood36C3CSpray spray;
    s8 heavy;

    if (arg0 == NULL) {
        return;
    }
    halfHeight = (arg0->unk14C + arg0->unk150) * 0.5f;
    func_1515C244(arg0, &position, &unused0, &unused1);
    kind = func_15134070(arg0);
    if (kind == 0x63) {
        return;
    }
    func_1504715C(&hit, arg0);
    if (arg3 != 0) {
        func_1513A24C(&position, D_80089A20[kind], &D_800A3C30,
                      ((arg5 != 0) || ((kind == 4) && (arg0->unk4 != 0x16)))
                          ? D_800A3F14[kind] - 2
                          : D_800A3F14[kind],
                      halfHeight, kind, D_800A3F64[kind], (u8 *)arg0, &hit, arg6,
                      arg7);
    }
    if (((u8 *)&BLOOD_TBL[kind])[0xE] == 2) {
        return;
    }
    if (arg1 != 0) {
        spray.countBase = 0xC;
        spray.countRange = 3;
        spray.origin = position;
        spray.angleBase = 0;
        spray.angleRange = 0xFF;
        spray.pitchBase = -0x40;
        spray.pitchRange = 0x45;
        spray.lifeBase = 0x96;
        spray.lifeRange = 0x64;
        spray.maxDistance = D_800A4630;
        spray.sizeBase = 25.0f;
        spray.sizeRange = 10.0f;
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind]) ||
            (&D_800A40F8 == &BLOOD_TBL[kind])) {
            spray.kind = 1;
        } else {
            spray.kind = 0;
        }
        func_15150D1C(&spray, arg6, arg7);
    }
    if (arg2 != 0) {
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind]) ||
            (&D_800A40F8 == &BLOOD_TBL[kind])) {
            heavy = 1;
        } else {
            heavy = 0;
        }
        func_151A9834(&position, position.y - 300.0f, 300.0f, &hit,
                      (func_150ADA20() % 6U) + 0x12, 0, &heavy, arg6, arg7);
    }
    if (arg4 != 0) {
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind])) {
            func_151C61A0((s32)&position, (s32)&hit, arg6, arg7);
        } else {
            func_1513A48C(&position, arg6, arg7);
        }
    }
}

extern void func_15143874(s16, f32, f32 *, f32 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, f32 *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_151DC034(struct17 *, f32, s32, s16, u8, u8, s32);
s32 func_15137C64(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3,
                  struct17 *arg4, struct17 *arg5, struct17 *arg6, struct17 *arg7);

void func_15136F50(void *arg0, void *arg1, void *arg2, u8 arg3, u8 arg4, s32 arg5) {
    struct17 v0;
    struct17 v1;
    struct17 v2;
    struct17 v3;
    struct17 v4;
    u8 pad0;
    u8 doSparks;
    u8 doDecals;
    s32 n;
    f32 sin1;
    f32 cos1;
    V15135DD0 vel1;
    f32 fa;
    f32 fb;
    f32 sin2;
    f32 cos2;
    V15135DD0 vel2;
    s32 ret;
    u32 r;
    s16 cnt;
    s32 j;
    f32 sin3;
    f32 cos3;
    s32 pad1;
    s32 pad2;
    f32 normal[3];
    struct17 hit;
    f32 dist;
    s32 surface;
    s32 pad3;

    doSparks = 1;
    doDecals = 1;
    ret = func_15137C64(&v0, &v1, &v2, &v3, &v4, arg0, arg1, arg2);
    if (ret != 0) {
        if (ret == 2) {
            doSparks = 0;
            doDecals = 0;
        }
        fa = func_150ADA68();
        r = func_150ADA20();
        func_151DC034(&v0, fa * 192.0f + 192.0f, (r % 101U + 0x9B) & 0xFF, (func_150ADA20() & 7) + 0xF, arg3, arg4, arg5);
        {
        s32 pad9;
        n = (func_150ADA20() & 3) + 4;
        for (; n > 0; n--) {
            fa = func_150ADA68() * 0.09290000051f + 0.06430000067f;
            r = func_150ADA20();
            func_15143874(r & 0xFF, func_150ADA68() * 50.0f, &cos1, &sin1);
            vel1.x = (v2.unk0 * 100.0f + v3.unk0 * cos1 + v4.unk0 * sin1) * fa;
            vel1.y = (v2.unk4 * 100.0f + v3.unk4 * cos1 + v4.unk4 * sin1) * fa;
            vel1.z = (v2.unk8 * 100.0f + v3.unk8 * cos1 + v4.unk8 * sin1) * fa;
            fb = func_150ADA68();
            fa = func_150ADA68();
            r = func_150ADA20();
            func_151DB5D0(arg3, &v1, &vel1, fb * 80.0f + 120.0f, 1.033617973f, 0.959544003f, fa * 0,
                          r % 31U + 0x1E, func_150ADA20() % 156U + 0x64, 0x2E, 5, 0, arg4, arg5);
        }
        }
        if (doSparks) {
            cnt = (func_150ADA20() & 3) + 4;
            if (cnt != 0) {
                do {
                    r = func_150ADA20();
                    func_15143874(r & 0xFF, func_150ADA68() * 2 + 6.0f, &cos2, &sin2);
                    vel2.x = v3.unk0 * cos2 + v4.unk0 * sin2;
                    vel2.y = v3.unk4 * cos2 + v4.unk4 * sin2;
                    vel2.z = v3.unk8 * cos2 + v4.unk8 * sin2;
                    r = func_150ADA20();
                    fa = func_150ADA68();
                    func_151DA6F8(&v0, &vel2, 0.0f, r % 6U + 8, 0xFF, fa * 1.300000072f + 1.899999976f,
                                  func_150ADA20() % 3U + 4, 0, 1.0f, 1.0f, 1, arg3, 0, 0x10, 0xF, 0, arg4, arg5);
                    cnt--;
                } while (cnt != 0);
            }
        }
        if (doDecals && D_800BE616) {
            n = (func_150ADA20() & 3) + 3;
            if (n != 0) {
                do {
                    r = func_150ADA20();
                    func_15143874(r & 0xFF, func_150ADA68() * 140.0f, &cos3, &sin3);
                    if (func_150AC9C0(v1.unk0, v1.unk4, v1.unk8,
                                      v2.unk0 * 100.0f + v3.unk0 * cos3 + v4.unk0 * sin3,
                                      v2.unk4 * 100.0f + v3.unk4 * cos3 + v4.unk4 * sin3,
                                      v2.unk8 * 100.0f + v3.unk8 * cos3 + v4.unk8 * sin3,
                                      0, normal, &hit.unk0, &hit.unk4, &hit.unk8, &dist, &surface, 0, 0.0f) != 0 &&
                        func_15145C90(surface) != 0 && dist < 1999.0f) {
                        fa = func_150ADA68();
                        r = func_150ADA20();
                        func_151D9B8C(arg3, fa * 16.0f + 25.0f, r % 156U + 0x64, normal, &hit,
                                      func_150ADA20() % 101U + 0x64, 1, 1, 1, arg4, arg5);
                    }
                    n--;
                } while (n != 0);
            }
        }
    }
}


typedef struct BloodImpactActor {
    u8 pad0[4];
    u8 kind;
    u8 pad5[0x6F];
    u8 flags74;
    u8 pad75[0x8F];
    u8 state104;
    u8 pad105[0x20];
    u8 state125;
    u8 pad126[0xA4];
    u8 count;
    u8 pad1CB[9];
    void *transforms;
} BloodImpactActor;

void func_15136F50(void *, void *, void *, u8, u8, s32);
void func_151C329C(void *, u8, s32);
void func_151C577C(void *, void *, void *, u8, u8, u8, u8, s32);
void func_151036B4(void *, u8, s32);

void func_15137610(BloodImpactActor *arg0, void *arg1, void *arg2,
                   void *arg3, u8 arg4, s32 arg5) {
    s32 mode;

    if (arg0 != 0) {
        if (arg0->transforms == 0 || (arg0->flags74 & 0xF) == 0xF ||
            (s32)arg0->count <= 0) {
            return;
        }
        mode = arg0->kind;
        switch (mode) {
            case 5:
            case 0xAD:
            case 0xAE:
            case 0xAF:
                mode = 3;
                break;
            case 0x5B:
            case 0x70:
            case 0xA6:
            case 0xB2:
            case 0xB4:
                func_151036B4(arg1, arg4, arg5);
                mode = 2;
                break;
            case 0x28:
            case 0x77:
            case 0x8A:
            case 0x8C:
                if ((func_150ADA20() & 1) == 0) {
                    mode = 2;
                    break;
                }
                return;
            case 0x42:
                if ((func_150ADA20() & 3) == 0) {
                    mode = 2;
                    break;
                }
                return;
            case 0x5A:
            case 0x5F:
            case 0x74:
            case 0x75:
            case 0x7A:
            case 0x8D:
            case 0xB1:
                mode = arg0->state125;
                if (mode == 0xFF) {
                    if ((func_150ADA20() & 0x1F) == 0) {
                        mode = 1;
                        break;
                    }
                } else if (mode == 0 && arg0->state104 == 0) {
                    mode = 1;
                    break;
                }
                return;
            default:
                if (arg0->state125 == 0 && arg0->state104 == 0) {
                    mode = 0;
                } else {
                    return;
                }
                break;
        }
    } else {
        mode = 0;
    }
    switch (mode) {
        case 1:
            func_151C577C(arg1, arg2, arg3, 1, 1, 1, arg4, arg5);
            return;
        case 2:
            func_151C329C(arg1, arg4, arg5);
            return;
        case 3:
            func_15136F50(arg1, arg2, arg3, 1, arg4, arg5);
            return;
        default:
        case 0:
            func_15136F50(arg1, arg2, arg3, 0, arg4, arg5);
            return;
    }
}

typedef struct Blood3783CHit {
    f32 height;
    u8 geometry[0x18];
    u8 flags;
    u8 pad1D[7];
} Blood3783CHit;

typedef void *(*Blood3783CSpawn)(Vec3f_36C3C *, Vec3f_36C3C *, f32, Vec3f_36C3C *, Vec3f_36C3C *, f32,
                                 s32, s32, s32, f32, s32, s32, s32, s32);

s32 func_15046C80(f32 *, u16, f32, void *);
s32 func_1514672C(f32 *);
void *func_151B5BF0(Vec3f_36C3C *, Vec3f_36C3C *, f32, Vec3f_36C3C *, Vec3f_36C3C *, f32, s32, s32,
                    s32, f32, s32, s32, s32, s32);
void *func_151B6010(Vec3f_36C3C *, Vec3f_36C3C *, f32, Vec3f_36C3C *, Vec3f_36C3C *, f32, s32, s32,
                    s32, f32, s32, s32, s32, s32);
extern f32 D_800A481C;
extern f32 D_800A4820;
extern f32 D_800A4824;

void func_1513783C(Blood36C3CActor *arg0, u8 arg1, s32 arg2) {
    Vec3f_36C3C position;
    f32 unused0;
    f32 unused1;
    f32 halfHeight;
    Vec3f_36C3C probe;
    Blood3783CHit hit;
    s32 kind;
    u8 heavy;
    Vec3f_36C3C origin;
    Vec3f_36C3C velocity;
    Vec3f_36C3C rotation;
    Vec3f_36C3C spin;
    Blood3783CSpawn spawn;

    if (arg0 == NULL) {
        return;
    }
    kind = func_15134070(arg0);
    if (kind == 0x63) {
        return;
    }
    halfHeight = (arg0->unk14C + arg0->unk150) * 0.5f;
    func_1515C244(arg0, &position, &unused0, &unused1);
    func_1504715C(&hit, arg0);
    probe.x = position.x;
    probe.y = position.y + 100.0f;
    probe.z = position.z;
    if (func_1514672C(&probe.x) == 0) {
        return;
    }
    if (func_15046C80(&probe.x, 0, probe.y - 500.0f, &hit) != 0) {
        if (hit.flags & 1) {
            heavy = 1;
        } else {
            heavy = 0;
        }
        probe.y = hit.height;
    } else {
        heavy = 0;
        probe = position;
    }
    func_1513A24C(&position, D_80089A20[kind] + 2, &D_800A3C30,
                  D_800A3F14[kind] - 2, halfHeight, kind, D_800A3F64[kind],
                  (u8 *)arg0, &hit, arg1, arg2);
    if (D_800A3FE6[kind * 16] != 2) {
        func_1513A48C(&position, arg1, arg2);
        if (heavy != 0) {
            func_15136698(((func_150ADA68() * 25.0f) + 75.0f) * D_800A481C,
                          ((func_150ADA68() * 106.0f) + 75.0f) * D_800A4820,
                          (func_150ADA20() % 101U) + 0x9B,
                          (func_150ADA20() % 101U) + 0x64,
                          (func_150ADA20() % 56U) + 0xCB, (s32)hit.geometry,
                          (Struct15136698Vec *)&probe, 1, 1, arg1, arg2);
            if (func_150ADA20() & 1) {
                spawn = func_151B5BF0;
            } else {
                spawn = func_151B6010;
            }
            origin.x = probe.x;
            velocity.x = 0.0f;
            origin.y = probe.y + 10.0f;
            velocity.y = 0.0f;
            velocity.z = 0.0f;
            origin.z = probe.z;
            rotation.x = func_150ADA68() * 360.0f;
            rotation.y = func_150ADA68() * 360.0f;
            rotation.z = func_150ADA68() * 360.0f;
            spin.x = 0.0f;
            spin.y = 0.0f;
            spin.z = 0.0f;
            spawn(&origin, &velocity,
                  ((func_150ADA68() * 101.0f) + 600.0f) * D_800A4824, &rotation,
                  &spin, 0.0f, 1000, 0xFF, 0xFF, 1.0f, 3, 0, arg1, arg2);
        }
    }
    func_15145A50((struct127 *)arg0);
}

s32 func_15137C64(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3,
                  struct17 *arg4, struct17 *arg5, struct17 *arg6, struct17 *arg7) {
    f32 sp2C;
    f32 sp28;

    if ((arg5 != NULL) && (arg6 != NULL)) {
        *arg0 = *arg5;
        *arg1 = *arg6;
    } else if (arg5 != NULL) {
        *arg0 = *arg5;
        *arg1 = *arg5;
    } else if (arg6 != NULL) {
        *arg0 = *arg6;
        *arg1 = *arg6;
    } else {
        return 0;
    }

    if (arg7 == NULL) {
        arg2->unk0 = arg1->unk0 - arg0->unk0;
        arg2->unk4 = arg1->unk4 - arg0->unk4;
        arg2->unk8 = arg1->unk8 - arg0->unk8;
        if (func_15145128(arg2, arg2, &sp2C, &sp28) == 0) {
            return 0;
        }
    } else {
        *arg2 = *arg7;
    }

    if (func_15146078(arg2, arg3, arg4) == 0) {
        return 2;
    }
    return 1;
}

extern f32 D_800A4828;

s32 func_15137E10(s32 *arg0) {
    f32 temp = func_150ADA68();
    *(f32 *)((u8 *)arg0 + 0x74) = (temp * 50.0f + 580.0f) * D_800A4828;
    return 1;
}

typedef struct { f32 x, y, z; } Vec15137;
typedef struct { u8 pad0[0x74]; f32 unk74; } Struct15137F30Scale;

extern void func_15137F30(Vec15137 *a0, Vec15137 *a1, Vec15137 *a2, Vec15137 *a3,
                          f32 sp10, Struct15137F30Scale *sp14, Vec15137 *out6C,
                          Vec15137 *out60, Vec15137 *out54, f32 *out50,
                          s16 *out4E, u8 *out4D, f32 *out48);
void func_15137E60(Vec15137 *arg0, Vec15137 *arg1, Vec15137 *arg2,
                   Vec15137 *arg3, f32 arg88, s32 *arg8C) {
    Vec15137 sp6C;
    Vec15137 sp60;
    Vec15137 sp54;
    f32 sp50;
    s16 sp4E;
    u8 sp4D;
    f32 sp48;

    func_15137F30(arg0, arg1, arg2, arg3, arg88, (Struct15137F30Scale *)arg8C,
                  &sp6C, &sp60, &sp54, &sp50, &sp4E, &sp4D, &sp48);
    func_151D9014(&sp6C, &sp54, 0, sp50, sp4E, sp4D, sp48, 0, 1.0f, 1.0f,
                  1, 0, 1, 0, *((u8 *)arg8C + 0xC), *((u8 *)arg8C + 1));
}

void func_15137F30(Vec15137 *a0, Vec15137 *a1, Vec15137 *a2, Vec15137 *a3,
                   f32 sp10, Struct15137F30Scale *sp14, Vec15137 *out6C, Vec15137 *out60,
                   Vec15137 *out54, f32 *out50, s16 *out4E, u8 *out4D,
                   f32 *out48) {
    out6C->x = a0->x + a2->x * sp10;
    out6C->y = a0->y + a2->y * sp10;
    out6C->z = a0->z + a2->z * sp10;
    out60->x = a1->x + a3->x * sp10;
    out60->y = a1->y + a3->y * sp10;
    out60->z = a1->z + a3->z * sp10;
    out54->x = (out60->x - out6C->x) * sp14->unk74;
    out54->y = (out60->y - out6C->y) * sp14->unk74;
    out54->z = (out60->z - out6C->z) * sp14->unk74;
    *out50 = (func_150ADA68() * 217.0f + -456.0f) * D_800A482C;
    *out4E = func_150ADA20() % 31U + 30;
    *out4D = func_150ADA20() % 156U + 100;
    *out48 = func_150ADA68() * 35.0f + 40.0f;
}

extern u8 D_800A3FD8[];
void func_15143134(void *, s32, s32);

s32 func_151380B4(s32 *arg0, s32 arg1, f32 *arg2) {
    s32 temp_v0 = *(s32 *)((u8 *)arg0 + 0x1D4);
    if (temp_v0 == 0) {
        return 0;
    }
    if ((*(u8 *)((u8 *)arg0 + 0x74) & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[arg1 << 4], (s32)arg2, temp_v0 + 0x300);
    return 1;
}

typedef struct {
    u8 unk00;
    u8 pad01[3];
    struct127 *unk04;
    u8 unk08;
    u8 pad09[3];
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 pad25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    u8 unk2E;
    u8 unk2F;
    s8 unk30;
    u8 pad31[3];
    f32 unk34;
    u8 unk38;
    s8 unk39;
    u8 pad3A[2];
} Local15138120;

void func_15138120(struct127 *arg0, s32 arg1, u8 arg2) {
    Local15138120 tmp;

    if (D_800A3FD8[(arg1 << 4) + 0xE] != 2) {
        tmp.unk00 = arg0->unique_id;
        tmp.unk04 = arg0;
        if (arg2) {
            tmp.unk08 = 0xC;
        } else {
            tmp.unk08 = 1;
        }
        tmp.unk24 = 2;
        tmp.unk26 = 0x28;
        tmp.unk28 = 0x10;
        tmp.unk0C = 0.0f;
        tmp.unk10 = 0.0f;
        tmp.unk14 = 0.0f;
        tmp.unk18 = 0.0f;
        tmp.unk20 = 0.0f;
        tmp.unk1C = 20.0f;
        if ((D_800A4058 == &D_800A3FD8[arg1 << 4]) || (D_800A4068 == &D_800A3FD8[arg1 << 4])) {
            tmp.unk2A = (arg0->unk94 & 0xE) ? 0x78 : 0xF0;
        } else {
            tmp.unk2A = 0x258;
        }
        tmp.unk2E = 5;
        if (D_800A3FD8[(arg1 << 4) + 0xE] == 0) {
            tmp.unk2F = 5;
        } else {
            tmp.unk2F = 6;
        }
        tmp.unk30 = -1;
        tmp.unk38 = 0;
        tmp.unk39 = -1;
        tmp.unk34 = 1.0f;
        tmp.unk2C = func_1000FA64(0x4FE, (s16)(s32)arg0->x_position,
                                  (s16)(s32)arg0->y_position,
                                  (s16)(s32)arg0->z_position, 0x5DC0,
                                  0x258, 0x12C, (s32)func_1000EBC4,
                                  (void *)0x78, 0, 0, 0);
        func_15134DAC(&tmp, 0);
    }
}

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
} Vec151382E0;

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    Vec151382E0 unk08;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} LocalDef151382E0;

void func_151382E0(Vec151382E0 *arg0, s32 arg1, void *arg2, u8 arg3, s32 arg4) {
    LocalDef151382E0 tmp;
    s32 temp_v0;

    temp_v0 = D_800A3FE6[arg1 << 4];
    if (temp_v0 != 2) {
        tmp.unk08 = *arg0;
        tmp.unk14 = D_800A4830;
        tmp.unk18 = D_800A4834;
        tmp.unk1C = D_800A4838;
        tmp.unk20 = D_800A483C;
        tmp.unk24 = 4.0f;
        tmp.unk28 = 9.0f;
        tmp.unk2C = 0x12;
        tmp.unk2E = 7;
        tmp.unk02 = 0xFF;
        tmp.unk00 = 0;
        tmp.unk04 = -0x3F;
        tmp.unk06 = 0x4E;
        tmp.unk30 = 3;
        tmp.unk32 = 3;
        tmp.unk34 = 0x14;
        tmp.unk36 = 0x1E;
        tmp.unk38 = 0x9B;
        tmp.unk3A = 0x64;
        tmp.unk44 = 0x10;
        tmp.unk46 = 0xF;
        tmp.unk48 = 0;
        tmp.unk40 = D_800A4840;
        if (temp_v0 == 1) {
            tmp.unk3C = 1;
        } else {
            tmp.unk3C = 0;
        }
        func_15153F18(&tmp, &tmp.unk08, (s32)arg2, arg3, arg4);
    }
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    u16 unk56;
    s8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    s8 unk68;
    u8 unk69;
    s8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 unk78;
} Spawn138424;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x94 - 0x3C];
    s32 unk94;
    u8 pad98[0x14C - 0x98];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} Obj138424;

typedef struct {
    u8 pad0[0xC];
    u16 unkC;
    u8 unkE;
    u8 padF;
} Entry138424;

extern Entry138424 D_800A4018;
extern Entry138424 D_800A4028;
extern Entry138424 D_800A4038;
extern void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
f32 func_150ADA68(void);

#define ENTRY ((Entry138424 *)&D_800A3FD8[arg2 << 4])
void func_15138424(Obj138424 *arg0, struct17 *arg1, s32 arg2, void *arg3, u8 arg4, s32 arg5) {
    struct17 v;
    Spawn138424 spawn;
    struct17 a;
    struct17 b;

    func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
    spawn.unk1C = 1.0f;
    spawn.unk00 = 20.0f;
    spawn.unk04 = 0.5610000491f;
    if (ENTRY->unkE == 1) {
        spawn.unk20 = 1.0f;
    } else {
        spawn.unk20 = 0.0f;
    }
    if (ENTRY->unkE == 2) {
        spawn.unk20 = 2.0f;
    }
    spawn.unk24 = 1.0f;
    spawn.unk50 = 0x39E9;
    spawn.unk44 = 0.0f;
    spawn.unk54 = func_150ADA20() % 51U + 250;
    spawn.unk56 = ENTRY->unkC;
    spawn.unk0C = (((Entry138424 *)&D_800A40C8 == ENTRY) ? 0.6000000238f : 1.0f) * ((arg0->unk14C + arg0->unk150) * 0.5f);
    spawn.unk08 = spawn.unk0C;
    spawn.unk4C = (func_150ADA68() * 292.0f + -1099.0f) * 0.001000000047f;
    spawn.unk28 = *arg1;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 21U - 83), func_150ADA68() * 8.0f + 9.0f, &spawn.unk34.unk0);
    spawn.unk10 = v;
    spawn.unk40 = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.unk48 = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0xD;
    spawn.unk62 = 0;
    spawn.unk63 = 8;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 1;
    spawn.unk6C = arg0;
    spawn.unk70 = arg0->unk3B;
    spawn.unk72 = 0xC;
    spawn.unk74 = 0x15;
    func_1513264C(&spawn, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
    if (&D_800A4018 == ENTRY) {
        const s32 ids[3] = { 0x76, 0x77, 0x78 };
        u8 i;
        u8 found;

        i = 0;
        found = 0;
        do {
            if (func_1503195C((struct127 *)arg0, ids[i], 0) != NULL) {
                found = 1;
            } else {
                i++;
            }
        } while (i < 3 && !found);
        if (found) {
            const s32 ids2[3] = { 0x84, 0x83, 0x82 };

            spawn.unk56 = ids2[i];
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
        }
    } else if (&D_800A4028 == ENTRY) {
        const s32 masks[4] = { 2, 8, 0x10, 0x20 };
        u8 i;
        const s32 ids[4] = { 0x87, 0x88, 0x89, 0x8A };

        for (i = 0; i < 4; i++) {
            if (!(masks[i] & arg0->unk94)) {
                spawn.unk56 = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    } else if (&D_800A4038 == ENTRY) {
        const s32 masks[2] = { 2, 8 };
        u8 i;
        const s32 ids[2] = { 0x8D, 0x8B };

        for (i = 0; i < 2; i++) {
            if (!(masks[i] & arg0->unk94)) {
                spawn.unk56 = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    }
}

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xC];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    s32 unk60;
} Struct151389A8;

s32 func_151389A8(Struct151389A8 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    struct17 pos;
    f32 damp;
    u32 r;
    f32 rf;
    s32 flag;

    damp = arg0->unk14;
    arg0->unk3C = arg0->unk10 + arg4;
    arg0->unk44 = arg0->unk44 * damp;
    arg0->unk48 = arg0->unk48 * -damp;
    arg0->unk4C = arg0->unk4C * damp;
    arg0->unk50 = arg0->unk50 * damp;
    arg0->unk54 = arg0->unk54 * damp;
    arg0->unk58 = arg0->unk58 * damp;
    if (fabsf(arg0->unk48) < 4.0f) {
        arg0->unk44 = 0.0f;
        arg0->unk60 &= ~0x69;
        arg0->unk48 = 0.0f;
        arg0->unk4C = 0.0f;
        arg0->unk50 = 0.0f;
        arg0->unk54 = 0.0f;
        arg0->unk58 = 0.0f;
        arg0->unk5C = 0.0f;
    }
    if (arg0->unk30 != 2.0f) {
        pos.unk0 = arg0->unk38;
        pos.unk4 = arg4;
        pos.unk8 = arg0->unk40;
        if (arg0->unk30) {
            flag = 1;
        } else {
            flag = 0;
        }
        rf = func_150ADA68();
        r = func_150ADA20();
        func_151D9B8C(flag, (rf * 25.39999962f + 39.79999924f) * ((arg0->unk18 + arg0->unk1C) * 0.5f),
                      (r % 101U) + 100, arg5, &pos, (func_150ADA20() % 101U) + 0x50, 1, 1, 0,
                      arg0->unkC, arg0->unk1);
        arg0->unk2C = 0.0f;
    }
    return 1;
}

void func_15138BC0(struct102 *arg0, u8 arg1, s32 arg2) {
    u8 sp50[8];
    u8 sp28[0x27];
    s32 temp_s1;

    temp_s1 = func_15134070(arg0);
    if (temp_s1 != 0x63) {
        sp28[0x1F] = func_151380B4(arg0, temp_s1, sp50 - 8);
        func_15138120(arg0, temp_s1, 1);
        if (sp28[0x1F] != 0) {
            func_1504715C(sp28 - 8, arg0);
            func_151382E0(sp50 - 8, temp_s1, sp28 - 8, arg1, arg2);
            func_15138424(arg0, sp50 - 8, temp_s1, sp28 - 8, arg1, arg2);
        }
    }
}

typedef struct {
    u8 pad_0[0x3B];
    u8 field_3B;
    u8 pad_3C[0x38];
    u8 field_74;
    u8 pad_75[0xD7];
    f32 field_14C;
    f32 field_150;
    u8 pad_154[0x80];
    u8 *field_1D4;
} Blood1380B4State;

typedef struct {
    f32 height;
    u8 geometry[0x20];
} BloodHitRecord;

void func_15138E98(Blood1380B4State *, f32 *, u8, void *, u8, s32);
struct Obj15139768;
struct ObjA6E0;
void func_15139768(struct Obj15139768 *, f32 *, f32, void *, u8, s32);
void func_15139D74(Blood1380B4State *, f32 *, void *, u8, s32);
void func_1513A6E0(struct ObjA6E0 *, f32 *, void *, u8, s32);
void func_1513ABB8(Blood1380B4State *, f32 *, u8, void *, u8, s32);
void func_1513B0F8(Blood1380B4State *, f32 *, void *, u8, s32);
void func_1513A594(void *, s32, s32, u8, s32);
void func_151036B4(void *, u8, s32);

void func_15138C80(Blood1380B4State *arg0, u8 arg1, s32 arg2) {
    s32 kind;
    Vec3f_36C3C position;
    u8 valid;
    BloodHitRecord hit;

    kind = func_15134070(arg0);
    if (kind != 0x63) {
        valid = func_151380B4(arg0, kind, &position.x);
        func_15138120(arg0, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, arg0);
            func_151382E0(&position.x, kind, &hit, arg1, arg2);
            switch (kind) {
            case 16:
                func_1513B0F8(arg0, &position.x, &hit, arg1, arg2);
                return;
            case 17:
                func_151036B4(&position.x, arg1, arg2);
                return;
            case 0:
            case 7:
                func_1513A6E0(arg0, &position.x, &hit, arg1, arg2);
                return;
            case 3:
                func_15138E98(arg0, &position.x, 1, &hit, arg1, arg2);
                return;
            case 5:
            case 6:
                func_15138E98(arg0, &position.x, 0, &hit, arg1, arg2);
                return;
            case 15:
                func_15139768(arg0, &position.x, 0.6f, &hit, arg1, arg2);
                return;
            case 2:
                func_15139768(arg0, &position.x, 1.0f, &hit, arg1, arg2);
                return;
            case 4:
                func_1513A594(arg0, (s32)&position.x, (s32)&hit, arg1, arg2);
                return;
            case 8:
            case 9:
                func_1513ABB8(arg0, &position.x, kind == 9, &hit, arg1, arg2);
                return;
            case 10:
            case 11:
            case 14:
            case 18:
            case 19:
                return;
            case 1:
            case 12:
            case 13:
            default:
                func_15139D74(arg0, &position.x, &hit, arg1, arg2);
                break;
            }
        }
    }
}

typedef struct BloodParticlePacket {
    f32 field0, field4, field8, fieldC;
    Vec3f_36C3C angles, scale, position, velocity, field40;
    f32 field4C;
    s32 field50;
    s16 field54, field56;
    u8 field58, pad59[3];
    s32 field5C;
    u8 field60, field61, field62, field63;
    u8 field64, field65, field66, field67;
    u8 field68, pad69, field6A, pad6B;
    void *owner;
    u8 field70, pad71;
    s16 field72, field74;
    u8 pad76[2];
    s32 field78;
} BloodParticlePacket;

extern u8 D_800A4160[];
extern s32 D_800A4168[];
extern s32 D_800A4178[];
extern Vec3f_36C3C D_800A4188[];
extern Vec3f_36C3C D_800A41B8;
void func_1513A5E0(s32 arg0, s32 arg1, s32 arg2);

void func_15138E98(Blood1380B4State *arg0, f32 *arg1, u8 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f_36C3C position;

    index = func_150ADA20() % 6U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 100;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->field_3B;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (!(D_800A4160[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A4168[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->field_1D4 == NULL) {
        return;
    }
    if ((arg0->field_74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u8 j;
        u8 bit;
        BloodParticlePacket spray;
        u32 yaw;
        u32 pitch;

        func_1503F404((s32)(arg0->field_1D4 + 0x300), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->field_3B;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 4; j++, bit <<= 1) {
            if (!(D_800A4160[index] & bit)) {
                packet.field56 = D_800A4178[j];
                func_15143134(&D_800A4188[j].x, (s32)&packet.position.x, (s32)(arg0->field_1D4 + 0x300));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
        if (arg2 == 0) {
            return;
        }
        spray.field0 = 1.0f;
        spray.field4 = 0.64900005f;
        spray.field8 = spray.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        spray.angles = position;
        spray.scale.x = 1.0f;
        spray.scale.y = 1.0f;
        spray.scale.z = 1.0f;
        spray.field40.y = 0.0f;
        spray.field58 = 0;
        spray.field5C = 0;
        spray.field60 = 0xFF;
        spray.field62 = 0;
        spray.field64 = 0;
        spray.field65 = 0;
        spray.field66 = 0;
        spray.field67 = 0;
        spray.field6A = 1;
        spray.owner = arg0;
        spray.field70 = arg0->field_3B;
        spray.field72 = 0xC;
        spray.field74 = 0x15;
        spray.field50 = 0x1039E9;
        spray.field61 = 0xD;
        spray.field68 = 2;
        spray.field63 = 0xB;
        spray.field56 = 0x48;
        func_15143134(&D_800A41B8.x, (s32)&spray.position.x, (s32)(arg0->field_1D4 + 0x300));
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794((u8)yaw, (s16)(pitch % 16U - 0x3C), func_150ADA68() * 7.1499996f + 10.889999f,
                      &spray.velocity.x);
        spray.field40.x = func_150ADA68() * 48.158f + -24.079f;
        spray.field40.z = func_150ADA68() * 48.158f + -24.079f;
        spray.field4C = func_150ADA68() * 0.323f + -1.4210001f;
        spray.field54 = func_150ADA20() % 61U + 100;
        func_1513264C(&spray, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
    }
}

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x10];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    s32 unk60;
} Struct15139578;

s32 func_15139578(Struct15139578 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    struct17 pos;
    f32 damp;
    u32 r;
    f32 rf;
    s32 flag;

    damp = arg0->unk14;
    arg0->unk3C = arg0->unk10 + arg4;
    arg0->unk44 = arg0->unk44 * damp;
    arg0->unk48 = arg0->unk48 * -damp;
    arg0->unk4C = arg0->unk4C * damp;
    arg0->unk50 = arg0->unk50 * damp;
    arg0->unk54 = arg0->unk54 * damp;
    arg0->unk58 = arg0->unk58 * damp;
    if (fabsf(arg0->unk48) < 4.0f) {
        arg0->unk44 = 0.0f;
        arg0->unk60 &= ~0x69;
        arg0->unk48 = 0.0f;
        arg0->unk4C = 0.0f;
        arg0->unk50 = 0.0f;
        arg0->unk54 = 0.0f;
        arg0->unk58 = 0.0f;
        arg0->unk5C = 0.0f;
    }
    if (arg0->unk30 != 2.0f) {
        pos.unk0 = arg0->unk38;
        pos.unk4 = arg4;
        pos.unk8 = arg0->unk40;
        if (arg0->unk30) {
            flag = 1;
        } else {
            flag = 0;
        }
        rf = func_150ADA68();
        r = func_150ADA20();
        func_151D9B8C(flag, (rf * 27.60000038f + 19.70000076f) * ((arg0->unk18 + arg0->unk1C) * 0.5f),
                      (r % 101U) + 100, arg5, &pos, (func_150ADA20() % 144U) + 0x50, 1, 1, 0,
                      arg0->unkC, arg0->unk1);
    }
    return 1;
}

typedef struct Obj15139768 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x14C - 0x75];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} Obj15139768;

extern u8 D_800A41C4[];
extern s32 D_800A41C8[];
extern s32 D_800A41D8[];
extern struct17 D_800A41E8[];
extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);

void func_15139768(Obj15139768 *arg0, f32 *arg1, f32 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 idx;
    u8 i;
    u8 mask;
    struct17 v;

    idx = func_150ADA20() & 3;
    {
        Spawn138424 spawn;
        u8 kind[4];
        kind[0] = 0xC;
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk08 = spawn.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f * arg2;
        spawn.unk10.unk0 = 0.0f;
        spawn.unk10.unk4 = 0.0f;
        spawn.unk10.unk8 = 0.0f;
        spawn.unk28.unk0 = 0.0f;
        spawn.unk28.unk4 = 0.0f;
        spawn.unk28.unk8 = 0.0f;
        spawn.unk34.unk0 = 0.0f;
        spawn.unk34.unk4 = 0.0f;
        spawn.unk34.unk8 = 0.0f;
        spawn.unk40 = 0.0f;
        spawn.unk44 = 0.0f;
        spawn.unk48 = 0.0f;
        spawn.unk4C = 0.0f;
        spawn.unk50 = 0x101900;
        spawn.unk54 = 0x64;
        spawn.unk58 = 5;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 0;
        spawn.unk62 = 0;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 5;
        spawn.unk6A = 1;
        spawn.unk6C = arg0;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk70 = arg0->unk3B;
        spawn.unk72 = 0xC;
        spawn.unk74 = 0x15;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (D_800A41C4[idx] & mask) {
                s32 ret;
                spawn.unk56 = D_800A41C8[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg4, arg5);
                if (ret != 0) {
                    memcpy((void *)(ret + 0x170), kind, 1);
                }
            }
        }
    }
    if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
        s32 count;
        u8 mask2;
        u8 j;
        struct17 dir;
        struct17 a;
        struct17 b;
        Spawn138424 spawn2;
        for (count = (func_150ADA20() & 3) + 5; count > 0; count--) {
            func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 71U - 60), func_150ADA68() * 6.0f + 6.0f, &dir.unk0);
            func_151C5F44((s32)arg1, &dir.unk0, func_150ADA68() * 0.1170000061f + -0.1970000118f, 0.9508649707f,
                          func_150ADA20() % 66U + 0xBE, func_150ADA20() % 45U + 0x2C, func_150ADA68() * 20.0f + 50.0f,
                          0, (s32)arg3, arg4, arg5);
        }
        {
            s32 q;
            func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
        }
        spawn2.unk00 = 1.0f;
        spawn2.unk04 = 0.6490000486f;
        spawn2.unk08 = spawn2.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f * arg2;
        spawn2.unk10 = v;
        spawn2.unk44 = 0.0f;
        spawn2.unk58 = 0;
        spawn2.unk5C = 0;
        spawn2.unk60 = 0xFF;
        spawn2.unk62 = 0;
        spawn2.unk64 = 0;
        spawn2.unk65 = 0;
        spawn2.unk66 = 0;
        spawn2.unk67 = 0;
        spawn2.unk6A = 1;
        spawn2.unk6C = arg0;
        spawn2.unk1C = 1.0f;
        spawn2.unk20 = 1.0f;
        spawn2.unk24 = 1.0f;
        spawn2.unk70 = arg0->unk3B;
        spawn2.unk72 = 0xC;
        spawn2.unk74 = 0x15;
        spawn2.unk50 = 0x1039E9;
        spawn2.unk61 = 0xD;
        spawn2.unk68 = 2;
        spawn2.unk63 = 9;
        for (j = 0, mask2 = 1; j < 4; j++, mask2 <<= 1) {
            if (!(D_800A41C4[idx] & mask2)) {
                spawn2.unk56 = D_800A41D8[j];
                func_15143134(&D_800A41E8[j], (s32)&spawn2.unk28, arg0->unk1D4 + 0x300);
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 35U - 0x54), func_150ADA68() * 6.119999886f + 3.0f, &spawn2.unk34.unk0);
                spawn2.unk40 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk48 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk4C = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.unk54 = func_150ADA20() % 101U + 0x3C;
                func_1513264C(&spawn2, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
    }
}

extern u8 D_800A4218[];
extern s32 D_800A421C[];
extern s32 D_800A4228[];
extern Vec3f_36C3C D_800A4234[];

void func_15139D74(Blood1380B4State *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f_36C3C position;

    index = func_150ADA20() % 3U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 0x190;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->field_3B;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 3; i++, mask <<= 1) {
            if (!(D_800A4218[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A421C[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg3, arg4);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->field_1D4 == NULL) {
        return;
    }
    if ((arg0->field_74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg3 & 0xFF, arg4);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404((s32)(arg0->field_1D4 + 0x300), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->field_3B;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 3; j++, bit <<= 1) {
            if (!(D_800A4218[index] & bit)) {
                packet.field56 = D_800A4228[j];
                func_15143134(&D_800A4234[j].x, (s32)&packet.position.x, (s32)(arg0->field_1D4 + 0x300));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
            }
        }
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f_36C3C pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 unk40;
} Cfg080C_1513A24C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x08 */ s32 unk8;
} Extra080C_1513A24C;

void func_1515080C(Cfg080C_1513A24C *, s32 *, f32 *, s32, Extra080C_1513A24C *, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15133E3C(s32, u8);

void func_1513A24C(Vec3f_36C3C *arg0, s32 *arg1, f32 *arg2, s32 arg3, f32 arg4, u8 arg5, u8 arg6, u8 *arg7, void *arg8, u8 arg9, s32 argA) {
    Cfg080C_1513A24C a;
    Extra080C_1513A24C b;
    s32 flag;

    a.unk0 = 0x28;
    a.unk4 = 0;
    a.pos = *arg0;
    a.unk14 = 0;
    a.unk16 = 0xFF;
    a.unk2C = 0x1E0;
    a.unk2E = 0xF0;
    a.unk30 = 1 * arg4;
    a.unk34 = 0 * arg4;
    a.unk40 = arg6;
    b.unk0 = 4;
    flag = -1;
    a.unk18 = -0x40;
    a.unk1A = 0xC;
    a.unk20 = 5.0f;
    a.unk3C = 1.0f;
    a.unk1C = 13.0f;
    a.unk24 = -1.007000089f;
    a.unk28 = 0.2520000041f;
    a.unk38 = 16.10200119f;
    switch (arg5) {
    case 0x13:
        if (D_800BE9F0 == 0x3C) {
            a.unk3C = 0.0f;
        }
        break;
    case 0xD:
        a.unk18 = -0x2E;
        a.unk1A = 0x14;
        a.unk1C = 15.0f;
        a.unk20 = 8.0f;
        a.unk24 = -1.262000084f;
        a.unk28 = 0.4900000095f;
        a.unk38 = 12.91100025f;
        break;
    case 1: {
        const u16 tbl[4] = { 0x83, 0x81, 0x82, 0x84 };
        s32 idx;
        idx = arg7[0x128];
        if (idx >= 4) {
            idx = 3;
        }
        b.unk4 = tbl[idx];
        b.unk6 = 6;
        b.unk8 = 3;
        flag = 1;
        break;
    }
    }
    func_1515080C(&a, arg1, arg2, arg3, &b, 0xC, 6, 1, flag, arg8, 5.0f, 1, D_800BE616 ? 1 : 0, arg9, argA);
    func_15133E3C(0, 0x45);
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_A48C;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_A48C position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA48CConfig;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4260[];
extern u8 D_800A4264[];

void func_1513A48C(void *arg0, u8 arg1, s32 arg2) {
    BloodA48CConfig config;

    config.field_0 = 8;
    config.field_4 = 4;
    config.position = *(Vec3i_A48C *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x37;
    config.field_1A = 0x20;
    config.field_2C = 0x28;
    config.field_2E = 0x14;
    config.field_30 = 0.07f;
    config.field_34 = 0.07f;
    config.field_1C = 10.0f;
    config.field_20 = 9.0f;
    config.field_24 = -1.0040001f;
    config.field_28 = 0.254f;
    config.field_38 = 23.446001f;
    func_15152190(&config, D_800A4260, D_800A4264, 1, 0.0f, 1,
                  (s32)arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A594.s")

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_1513A5E0;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_1513A5E0 position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA5E0Config;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4268[];
extern u8 D_800A4270[];

void func_1513A5E0(s32 arg0, s32 arg1, s32 arg2) {
    BloodA5E0Config config;

    config.field_0 = 7;
    config.field_4 = 7;
    config.position = *(Vec3i_1513A5E0 *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x32;
    config.field_1A = 0x1B;
    config.field_1C = 4.0f;
    config.field_20 = 4.0f;
    config.field_2C = 0x19;
    config.field_2E = 0x28;
    config.field_30 = 0.0201f;
    config.field_34 = 0.0201f;
    config.field_24 = -0.43f;
    config.field_28 = 0.272f;
    config.field_38 = 20.187f;
    func_15152190(&config, D_800A4268, D_800A4270, 2, 0.0f, 1,
                  (u8)arg1, arg2);
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    s8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    s8 unk68;
    u8 unk69;
    s8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 unk78;
} SpawnA6E0;

typedef struct ObjA6E0 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x14C - 0x75];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} ObjA6E0;

extern u8 D_800A4278[];
extern s32 D_800A4280[];
extern s32 D_800A4290[];
extern struct17 D_800A42A0[];
extern void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
f32 func_150ADA68(void);

void func_1513A6E0(ObjA6E0 *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 idx;
    u8 i;
    u8 mask;
    struct17 v;

    idx = func_150ADA20() % 6U;
    {
    SpawnA6E0 spawn;
    u8 extra[4];
    s32 pad1;
    extra[0] = 0xC;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk08 = spawn.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
    spawn.unk10.unk0 = 0.0f;
    spawn.unk10.unk4 = 0.0f;
    spawn.unk10.unk8 = 0.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk28.unk0 = 0.0f;
    spawn.unk28.unk4 = 0.0f;
    spawn.unk28.unk8 = 0.0f;
    spawn.unk34.unk0 = 0.0f;
    spawn.unk34.unk4 = 0.0f;
    spawn.unk34.unk8 = 0.0f;
    spawn.unk40 = 0.0f;
    spawn.unk44 = 0.0f;
    spawn.unk48 = 0.0f;
    spawn.unk4C = 0.0f;
    spawn.unk50 = 0x101900;
    spawn.unk54 = 0x64;
    spawn.unk58 = 5;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 5;
    spawn.unk6A = 1;
    spawn.unk6C = arg0;
    spawn.unk70 = arg0->unk3B;
    spawn.unk72 = 0xC;
    spawn.unk74 = 0x15;
    for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
        if (D_800A4278[idx] & mask) {
            s32 ret;
            spawn.unk56 = D_800A4280[i];
            ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (ret != 0) {
                memcpy((void *)(ret + 0x170), extra, 1);
            }
        }
    }
    }
    if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
        struct17 a;
        struct17 b;
        SpawnA6E0 spawn2;
        u8 mask2;
        u32 r1;
        u32 r2;
        func_1513A5E0((s32)arg1, arg3 & 0xFF, arg4);
        {
        s32 q;
        func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
        }
        spawn2.unk00 = 1.0f;
        spawn2.unk04 = 0.6490000486f;
        spawn2.unk08 = spawn2.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
        spawn2.unk10 = v;
        spawn2.unk1C = 1.0f;
        spawn2.unk20 = 0.0f;
        spawn2.unk24 = 1.0f;
        spawn2.unk44 = 0.0f;
        spawn2.unk58 = 0;
        spawn2.unk5C = 0;
        spawn2.unk60 = 0xFF;
        spawn2.unk62 = 0;
        spawn2.unk64 = 0;
        spawn2.unk65 = 0;
        spawn2.unk66 = 0;
        spawn2.unk67 = 0;
        spawn2.unk6A = 1;
        spawn2.unk6C = arg0;
        spawn2.unk70 = arg0->unk3B;
        spawn2.unk72 = 0xC;
        spawn2.unk74 = 0x15;
        spawn2.unk50 = 0x1039E9;
        spawn2.unk61 = 0xD;
        spawn2.unk68 = 2;
        spawn2.unk63 = 9;
        for (i = 0, mask2 = 1; i < 4; i++, mask2 <<= 1) {
            if (!(D_800A4278[idx] & mask2)) {
                spawn2.unk56 = D_800A4290[i];
                func_15143134(&D_800A42A0[i], (s32)&spawn2.unk28, arg0->unk1D4 + 0x300);
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_15143794(r1 & 0xFF, r2 % 35U - 84, func_150ADA68() * 6.119999886f + 3.0f, &spawn2.unk34.unk0);
                spawn2.unk40 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk48 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk4C = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.unk54 = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
            }
        }
    }
}

extern u8 D_800A42D0[];
extern s32 D_800A42D4[][2];
extern s32 D_800A42E4[][2];
extern Vec3f_36C3C D_800A42F4[][2];

void func_1513ABB8(Blood1380B4State *arg0, f32 *arg1, u8 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f_36C3C position;

    index = func_150ADA20() % 3U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 0x64;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->field_3B;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
            if (!(D_800A42D0[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A42D4[arg2][i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->field_1D4 == NULL) {
        return;
    }
    if ((arg0->field_74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404((s32)(arg0->field_1D4 + 0x300), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->field_3B;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 2; j++, bit <<= 1) {
            if (!(D_800A42D0[index] & bit)) {
                packet.field56 = D_800A42E4[arg2][j];
                func_15143134(&D_800A42F4[arg2][j].x, (s32)&packet.position.x, (s32)(arg0->field_1D4 + 0x300));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
    }
}

void func_1513B0B8(void *arg0, s32 arg1, u8 arg2) {
    s32 *temp_v0 = (s32 *)((s32)arg0 + 0x170);

    if (arg2 == 0x45) {
        if (--(*temp_v0) < 0) {
            *(s32 *)((s32)arg0 + 0x60) |= 0x80;
        }
    }
}

void func_151036B4(void *, u8, s32);
extern u8 D_800A4324[];
extern s32 D_800A4328[];
extern s32 D_800A4330[];
extern f32 D_800A4338[][3];
void func_1513B0F8(Blood1380B4State *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f_36C3C rotation;
    BloodParticlePacket spawn;
    u8 i;
    u8 mask;
    u8 bit;
    u8 tag;
    u32 yaw;
    u32 pitch;
    f32 first[3];
    f32 second[3];
    BloodParticlePacket drop;

    func_151036B4(arg1, arg3, arg4);
    {
        u32 random = func_150ADA20();

        index = random % 3;
    }
    tag = 0xC;
    spawn.field0 = 1.0f;
    spawn.field4 = 1.0f;
    spawn.field8 = spawn.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
    spawn.angles.x = 0.0f;
    spawn.angles.y = 0.0f;
    spawn.angles.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.position.x = 0.0f;
    spawn.position.y = 0.0f;
    spawn.position.z = 0.0f;
    spawn.velocity.x = 0.0f;
    spawn.velocity.y = 0.0f;
    spawn.velocity.z = 0.0f;
    spawn.field40.x = 0.0f;
    spawn.field40.y = 0.0f;
    spawn.field40.z = 0.0f;
    spawn.field4C = 0.0f;
    spawn.field50 = 0x101900;
    spawn.field54 = 100;
    spawn.field58 = 5;
    spawn.field5C = 0;
    spawn.field60 = 0xFF;
    spawn.field61 = 0;
    spawn.field62 = 0;
    spawn.field63 = 0;
    spawn.field64 = 0;
    spawn.field65 = 0;
    spawn.field66 = 0;
    spawn.field67 = 0;
    spawn.field68 = 5;
    spawn.field6A = 1;
    spawn.owner = arg0;
    spawn.field70 = arg0->field_3B;
    spawn.field72 = 0xC;
    spawn.field74 = 0x15;
    for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
        if (D_800A4324[index] & mask) {
            void *particle;

            spawn.field56 = D_800A4328[i];
            particle = (void *)func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0x170, &tag, 1);
            }
        }
    }
    if (arg0->field_1D4 == NULL) {
        return;
    }
    if ((arg0->field_74 & 0xF) == 0xF) {
        return;
    }
    func_1503F404((s32)(arg0->field_1D4 + 0x300), &first[0], &first[1], &first[2],
                  &rotation.x, &rotation.y, &rotation.z,
                  &second[0], &second[1], &second[2]);
    drop.field0 = 1.0f;
    drop.field4 = 0.64900005f;
    drop.field8 = drop.fieldC = (arg0->field_14C + arg0->field_150) * 0.5f;
    drop.angles = rotation;
    drop.scale.x = 1.0f;
    drop.scale.y = 2.0f;
    drop.scale.z = 1.0f;
    drop.field40.y = 0.0f;
    drop.field58 = 0;
    drop.field5C = 0;
    drop.field60 = 0xFF;
    drop.field62 = 0;
    drop.field64 = 0;
    drop.field65 = 0;
    drop.field66 = 0;
    drop.field67 = 0;
    drop.field6A = 1;
    drop.owner = arg0;
    drop.field70 = arg0->field_3B;
    drop.field72 = 0xC;
    drop.field74 = 0x15;
    drop.field50 = 0x1039E9;
    drop.field61 = 0xD;
    drop.field68 = 2;
    drop.field63 = 9;
    for (i = 0, bit = 1; i < 2; i++, bit <<= 1) {
        if (!(D_800A4324[index] & bit)) {
            u32 random;

            drop.field56 = D_800A4330[i];
            func_15143134(D_800A4338[i], (s32)&drop.position.x, (s32)(arg0->field_1D4 + 0x300));
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794((u8)yaw, (pitch % 35U) - 0x54, (func_150ADA68() * 6.12f) + 3.0f, &drop.velocity.x);
            drop.field40.x = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.field40.z = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.field4C = (func_150ADA68() * 0.208f) + -0.708f;
            random = func_150ADA20();
            drop.field54 = (random % 101U) + 0x3C;
            func_1513264C(&drop, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
        }
    }
}
