#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value (asm: dsra32 v0); the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

struct Vec15107A20 { f32 x; f32 y; f32 z; };
struct Vec15106F98 { f32 x; f32 y; f32 z; };
struct S15106F98Points { struct Vec15106F98 unk0[4]; };
struct S15106F98Payload {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    struct Vec15106F98 *unkC;
    struct S15106F98Points unk10;
    u8 unk40;
    u8 pad41[3];
};

struct S15107C1CWord {
    s32 unk0;
};

struct S15107C1CWords3 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct S15107C1CBytes4 {
    u8 bytes[4];
};

struct S15107C1CPayload {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    struct S15107C1CWords3 unk8;
    u8 unk14;
    u8 pad15[3];
};

struct S15107C1CTrace {
    u8 pad0[8];
    struct Vec15107A20 unk8;
    u8 pad14[0x59 - 0x14];
    u8 unk59;
    u8 pad5A[0x64 - 0x5A];
};

struct S15107C1CInit {
    s16 unk0;
    s8 unk2;
    u8 pad3;
    struct Vec15107A20 unk4;
    struct Vec15107A20 unk10;
    struct Vec15107A20 unk1C;
    struct Vec15107A20 unk28;
    s8 unk34;
    u8 pad35[3];
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    struct S15107C1CWord unk48;
    s8 unk4C;
    u8 pad4D[3];
};

struct S15107C1CCreated {
    u8 pad0[0x60];
    void *unk60;
};

void func_151070F8(struct260 *arg0, s16 arg1, s16 arg2, f32 arg3);
void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4);
void func_15107AE0(struct Vec15107A20 *arg0, struct Vec15107A20 *arg1, struct Vec15107A20 *arg2, struct Vec15107A20 *arg3);
void func_15107F54(s32 *arg0, u8 arg1, void *arg2, s32 arg3);
void func_15081690(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, void *arg7, f32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
struct S15107C1CCreated *func_15105CE0(void *arg0, s32 arg1, u8 arg2, s32 arg3);
void func_15106214(struct102 *arg0);
void func_15106610(struct102 *arg0);

struct S1510608CVec {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct S1510608CChild {
    u8 pad0[0x40];
    struct S1510608CVec unk40;
};

struct S1510608C {
    u8 pad0[0x10];
    s16 unk10;
    u8 unk12;
    u8 pad13;
    struct S1510608CVec unk14;
    u8 pad20[0x38 - 0x20];
    struct S1510608CVec unk38;
    s8 unk44;
    u8 pad45[0x54 - 0x45];
    s16 unk54;
    s16 unk56;
    u8 pad58[0x68 - 0x58];
    s16 unk68;
    u8 pad6A[0x6C - 0x6A];
    struct S1510608CChild *unk6C;
    struct S1510608CChild *unk70;
};

extern s32 (*D_80088C10[])(struct S1510608C *arg0, u8 *arg1);

#pragma GLOBAL_ASM("asm/nonmatchings/game_133190/func_15105CE0.s")

void func_1510608C(struct S1510608C *arg0) {
    s32 (*fn)(struct S1510608C *, u8 *);
    u8 sp2B;
    struct S1510608CChild *child;

    sp2B = 0;
    if (arg0->unk44 != -1) {
        fn = D_80088C10[arg0->unk44];
        if (fn != NULL) {
            if (fn(arg0, &sp2B) == 0) {
                func_1516972C((struct102 *)arg0);
            }
        }
    }

    if (arg0->unk12 & 1) {
        arg0->unk10 -= D_800BE9E4;
        if (arg0->unk10 < 0) {
            func_1516972C((struct102 *)arg0);
        }
    }

    if (sp2B != 0) {
        func_15106214((struct102 *)arg0);
    }

    arg0->unk68 -= D_800BE9E4;
    if (arg0->unk68 < 0) {
        func_15106610((struct102 *)arg0);
        arg0->unk68 = (func_150ADA20() % (u32)(arg0->unk56 + 1)) + arg0->unk54;
    }

    child = arg0->unk6C;
    if (child != NULL) {
        child->unk40 = arg0->unk14;
    }

    child = arg0->unk70;
    if (child != NULL) {
        child->unk40 = arg0->unk38;
    }
}

struct S15106214Record;
struct S15106214Record *func_151061E0(struct102 *arg0) {
    return (struct S15106214Record *)((s32)arg0 + 0x88);
}

struct S151061EC {
    u8 pad0[0x48];
    s32 idx;
    u8 pad4C[0x88 - 0x4C];
    u8 elems[1][0x34];
};

void *func_151061EC(struct S151061EC *arg0) {
    s32 i = arg0->idx;
    return &arg0->elems[i];
}

struct Vec15106214 { f32 x; f32 y; f32 z; };
struct S15106214Record {
    f32 unk0;
    struct Vec15106214 unk4;
    struct Vec15106214 unk10;
    struct Vec15106214 unk1C;
    struct Vec15106214 unk28;
};
struct S15106214Arg {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x48 - 0x44];
    s32 unk48;
    u8 pad4C[0x64 - 0x4C];
    f32 unk64;
};

struct S15106214Record *func_151061E0(struct102 *arg0);
f32 func_151064B4(f32 arg0);
f32 func_151064DC(f32 arg0);
f32 func_15106510(f32 arg0);
f32 func_15106540(f32 arg0);
f32 func_15106558(f32 arg0);
f32 func_15106584(f32 arg0);
f32 func_151065BC(f32 arg0);
f32 func_151065EC(f32 arg0);
s32 func_15146078(struct Vec15106214 *arg0, struct Vec15106214 *arg1, struct Vec15106214 *arg2);

#define A ((struct S15106214Arg *)arg0)
void func_15106214(struct102 *arg0) {
    struct S15106214Record *rec;
    f32 step;
    struct S15106214Record *out;
    s32 i;
    f32 t;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 b3;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 d3;
    rec = func_151061E0(arg0);
    step = A->unk64;
    i = 0;
    t = step;
    if (A->unk48 > 0) {
        do {

        out = &rec[i];
        out->unk0 = t;
        b0 = func_151064B4(t);
        b1 = func_151064DC(t);
        b2 = func_15106510(t);
        b3 = func_15106540(t);
        d0 = func_15106558(t);
        d1 = func_15106584(t);
        d2 = func_151065BC(t);
        d3 = func_151065EC(t);
        out->unk4.x = (A->unk38 * b3) + ((A->unk14 * b0) + (A->unk20 * b1) + (A->unk2C * b2));
        out->unk4.y = (A->unk3C * b3) + ((A->unk18 * b0) + (A->unk24 * b1) + (A->unk30 * b2));
        out->unk4.z = (A->unk40 * b3) + ((A->unk1C * b0) + (A->unk28 * b1) + (A->unk34 * b2));
        out->unk10.x = (A->unk38 * d3) + ((A->unk14 * d0) + (A->unk20 * d1) + (A->unk2C * d2));
        out->unk10.y = (A->unk3C * d3) + ((A->unk18 * d0) + (A->unk24 * d1) + (A->unk30 * d2));
        out->unk10.z = (A->unk40 * d3) + ((A->unk1C * d0) + (A->unk28 * d1) + (A->unk34 * d2));
        if (func_15146078(&out->unk10, &out->unk1C, &out->unk28) == 0) {
            out->unk1C.x = 0.0f;
            out->unk1C.y = 0.0f;
            out->unk1C.z = 0.0f;
            out->unk28.x = 0.0f;
            out->unk28.y = 0.0f;
            out->unk28.z = 0.0f;
        }
        i += 1;
        t += step;
        } while (i < A->unk48);
    }
}
#undef A

f32 func_151064B4(f32 arg0) {
    f32 t = 1.0f - arg0;
    return t * t * t;
}

f32 func_151064DC(f32 arg0) {
    f32 t = 1.0f - arg0;
    return 3.0f * arg0 * t * t;
}

f32 func_15106510(f32 arg0) {
    return 3.0f * arg0 * arg0 * (1.0f - arg0);
}

f32 func_15106540(f32 arg0) {
    return arg0 * arg0 * arg0;
}

f32 func_15106558(f32 arg0) {
    f32 t = 1.0f - arg0;
    return -3.0f * t * t;
}

f32 func_15106584(f32 arg0) {
    return 9.0f * arg0 * arg0 - 12.0f * arg0 + 3.0f;
}

f32 func_151065BC(f32 arg0) {
    return -9.0f * arg0 * arg0 + 6.0f * arg0;
}

f32 func_151065EC(f32 arg0) {
    return 3.0f * arg0 * arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_133190/func_15106610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_133190/func_151067B8.s")

extern void (*D_80088C18[])(struct102 *);
void func_151D5E30(void *arg0);

void func_15106E78(struct102 *arg0) {
    void (*fn)(struct102 *);
    struct102 *p6C;

    fn = D_80088C18[*(u8 *)((u8 *)arg0 + 0x5C)];
    if (fn != NULL) {
        fn(arg0);
    }
    p6C = *(struct102 **)((u8 *)arg0 + 0x6C);
    if (p6C != NULL) {
        func_1516972C(p6C);
    }
    p6C = *(struct102 **)((u8 *)arg0 + 0x70);
    if (p6C != NULL) {
        func_1516972C(p6C);
    }
    func_151D5E30((u8 *)arg0 + 0x74);
}

void func_15106EF8(struct102 *arg0) {
    func_15106E78(arg0);
    func_15169804(arg0);
}

void func_15169824(struct102 *arg0);

void func_15106F24(struct102 *arg0) {
    func_15106E78(arg0);
    func_15169824(arg0);
}

extern void (*D_80088C28[])(void *, s32, u8);
extern void (*D_80088C38[])(struct102 *, s32, u8);

void func_15106F50(struct102 *arg0, s32 arg1, u8 arg2) {
    void (*fn)(void *, s32, u8) = D_80088C28[*(u8 *)((u8 *)arg0 + 0x5C)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

struct260 *func_15106F98(struct Vec15106F98 *arg0, struct Vec15106F98 *arg1, s32 arg2, struct S15106F98Points *arg3, f32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    struct260 *temp_v0;
    struct S15106F98Payload sp40;
    struct S15106F98Payload *data;

    sp40.unk0 = (1 << arg2) + 1;
    sp40.unk8 = 0;
    sp40.unk4 = 0;
    sp40.unk10 = *arg3;
    sp40.unk40 = arg5;

    temp_v0 = func_15149130(0, -1, 0x3E, -1, 0, 0x30, (struct37 *)(sp40.unk0 * 12 + 0x48), arg6, arg7);
    if (temp_v0 != NULL) {
        data = (struct S15106F98Payload *)((u8 *)temp_v0 + 0x28);
        memcpy(data, &sp40, 0x44);
        data->unkC = (struct Vec15106F98 *)((u8 *)data + 0x48);
        data->unkC[0] = *arg0;
        data->unkC[sp40.unk0 - 1] = *arg1;
        func_151070F8(temp_v0, 0, (s16)(sp40.unk0 - 1), arg4);
    }
    return temp_v0;
}

void func_151070F8(struct260 *arg0, s16 arg1, s16 arg2, f32 arg3) {
    struct S15106F98Payload *data = (struct S15106F98Payload *)((u8 *)arg0 + 0x28);
    struct Vec15106F98 *a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;
    s16 mid;

    if (arg2 - arg1 < 2) {
        return;
    }
    mid = arg1 + ((arg2 - arg1) >> 1);
    a = &data->unkC[arg1];
    dx = data->unkC[arg2].x - data->unkC[arg1].x;
    dy = data->unkC[arg2].y - data->unkC[arg1].y;
    dz = data->unkC[arg2].z - data->unkC[arg1].z;
    t = func_150ADA68() * (arg3 + arg3) + (0.5f - arg3);
    data->unkC[mid].x = a->x + dx * t;
    data->unkC[mid].y = a->y + dy * t;
    data->unkC[mid].z = a->z + dz * t;
    func_151070F8(arg0, arg1, mid, arg3);
    func_151070F8(arg0, mid, arg2, arg3);
}

void func_15143794(s32, s32, f32, f32 *);

typedef struct S151072BCColor {
    u8 bytes[4];
} S151072BCColor;

typedef struct S151072BCBeam {
    s16 life;
    s8 enabled;
    u8 pad3;
    struct Vec15107A20 position;
    struct Vec15107A20 axis0;
    struct Vec15107A20 axis1;
    struct Vec15107A20 contact;
    s8 flag;
    u8 pad35[3];
    s32 count;
    f32 width;
    f32 scale;
    s16 lower;
    s16 upper;
    S151072BCColor color;
    s8 mode;
    u8 pad4D[3];
} S151072BCBeam;

typedef struct S151072BCConfig {
    f32 scaleBase, scaleRange, width, speedBase, speedRange;
    s16 spread, lower, upper, lifeBase, lifeRange;
    u8 pad1E[2];
    s32 countBase, countRange;
    u8 red, green, blue, alphaBase, alphaRange;
} S151072BCConfig;

typedef struct S151072BCChain {
    s32 count, index;
    struct S15107C1CCreated *child;
    struct Vec15107A20 *points;
    S151072BCConfig config;
} S151072BCChain;

void func_151072BC(u8 *arg0) {
    S151072BCChain *chain;
    S151072BCBeam beam;
    u8 *owner;
    struct Vec15107A20 direction;
    S151072BCConfig *config;

    chain = (S151072BCChain *)(arg0 + 0x28);
    if (*(s32 *)(arg0 + 0x30) == 0) {
        if (chain->index >= chain->count - 1) {
            *(s16 *)(arg0 + 0xE) = -1;
            arg0[0xD] |= 1;
            return;
        }
        owner = arg0;
        beam.enabled = 1;
        beam.flag = -1;
        config = &chain->config;
        beam.scale = func_150ADA68() * config->scaleRange + config->scaleBase;
        beam.width = config->width;
        beam.lower = config->lower;
        beam.upper = config->upper;
        beam.position = chain->points[chain->index];
        beam.contact = chain->points[chain->index + 1];
        func_15143794((s16)(func_150ADA20() & 0xFF),
            (s16)(func_150ADA20() % (u32)(config->spread + 1) - (config->spread >> 1) - 0x40),
            func_150ADA68() * config->speedRange + config->speedBase, &direction.x);
        beam.axis0.x = beam.position.x + direction.x;
        beam.axis0.y = beam.position.y + direction.y;
        beam.axis0.z = beam.position.z + direction.z;
        func_15143794((s16)(func_150ADA20() & 0xFF),
            (s16)(func_150ADA20() % (u32)(config->spread + 1) - (config->spread >> 1) - 0x40),
            func_150ADA68() * config->speedRange + config->speedBase, &direction.x);
        beam.axis1.x = beam.contact.x + direction.x;
        beam.axis1.y = beam.contact.y + direction.y;
        beam.axis1.z = beam.contact.z + direction.z;
        beam.color.bytes[0] = config->red;
        beam.color.bytes[1] = config->green;
        beam.color.bytes[2] = config->blue;
        beam.life = func_150ADA20() % (u32)(config->lifeRange + 1) + config->lifeBase;
        beam.color.bytes[3] = func_150ADA20() % (u32)(config->alphaRange + 1) + config->alphaBase;
        beam.mode = 1;
        beam.count = func_150ADA20() % (u32)(config->countRange + 1) + config->countBase;
        chain->child = func_15105CE0(&beam, 4, arg0[0xC], arg0[1]);
        if (chain->child != NULL) {
            memcpy(chain->child->unk60, &owner, 4U);
        }
        chain->index++;
    }
}

struct S15107604 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

void func_15107604(s32 **arg0) {
    s32 **q = (s32 **)arg0[0x18];
    struct S15107604 *p = (struct S15107604 *)((u8 *)q[0] + 0x28);
    p->unk8 = 0;
}

void func_1510761C(struct260 *arg0) {
    if (*(struct102 *volatile *)((u8 *)arg0 + 0x30) != 0) {
        func_1516972C(*(struct102 *volatile *)((u8 *)arg0 + 0x30));
    }
}

void func_1510764C(struct260 *arg0) {
    func_1510761C(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_15107678(struct260 *arg0) {
    func_1510761C(arg0);
    func_15149368(arg0);
}

void func_151076A4(struct102 *arg0, s32 arg1, u8 arg2) {
    if (D_80088C38[*(u8 *)((s32)arg0 + 0x68)] != 0) {
        D_80088C38[*(volatile u8 *)((s32)arg0 + 0x68)](arg0, arg1, arg2);
    }
}

struct S15107700Payload {
    struct127 *unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s16 unk8;
    u8 padA[2];
};

struct S15107C1CCreated *func_15107700(struct127 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, f32 arg5, f32 arg6, s16 arg7, s16 arg8, u8 *arg9, u8 argA, s32 argB) {
    struct S15107700Payload payload;
    struct S15107C1CCreated *created;
    struct S15107C1CInit init;
    struct S15107C1CTrace trace;
    struct Vec15107A20 vec;
    s32 flag;
    if (arg0->unk1D4 == NULL) {
        return NULL;
    }

    payload.unk0 = arg0;
    payload.unk4 = arg0->unique_id;
    payload.unk6 = arg1;
    payload.unk8 = arg2;

    if (arg3 == -1) {
        init.unk0 = 0x12C;
    } else {
        init.unk0 = arg3;
    }
    if (arg3 == -1) {
        flag = 0;
    } else {
        flag = 1;
    }
    init.unk2 = flag;
    init.unk34 = 0;
    init.unk38 = arg4;
    init.unk3C = arg5;
    init.unk40 = arg6;
    init.unk44 = arg7;
    init.unk46 = arg8;
    *(struct S15107C1CBytes4 *)&init.unk48 = *(struct S15107C1CBytes4 *)arg9;
    init.unk4C = 2;

    /* block-scoped declaration: its debug scope marker keeps IDO from hoisting &init.unk48 out of this block */
    {
    void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4);
    func_15107A20(arg0, arg1, arg2, &init.unk4, &vec);
    func_15081690(arg0, init.unk4.x, init.unk4.y, init.unk4.z, init.unk4.x - vec.x, init.unk4.y - vec.y, init.unk4.z - vec.z, &trace, 0.0f, 0, 1, 1, -1, 0, 0);

    }
    if (trace.unk59 == 0) {
        return NULL;
    }
    init.unk28 = trace.unk8;

    func_15107AE0(&init.unk4, &init.unk28, &init.unk10, &init.unk1C);
    created = func_15105CE0(&init, 0xC, argA, argB);
    if (created != NULL) {
        memcpy(created->unk60, &payload, 0xC);
    }
    return created;
}


void func_151078E4(s32 *arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0[0x18], arg0[0x18] + 4, (s32)arg0);
}

struct S15107924 {
    struct127 *unk0;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    s16 unk8;
};

s32 func_15107924(struct127 *arg0, u8 *arg1) {
    struct S15107924 *data;
    struct127 *obj;
    struct Vec15107A20 oldPos;

    data = (struct S15107924 *)arg0->unk60;
    obj = data->unk0;
    if ((obj->interaction_state == 0) || (obj->unique_id != data->unk4)) {
        return 0;
    }
    *arg1 = 0;
    oldPos = *(struct Vec15107A20 *)&arg0->x_position;
    func_15107A20(obj, data->unk6, data->unk8, (struct Vec15107A20 *)&arg0->x_position, NULL);
    if ((oldPos.x == arg0->x_position) && (oldPos.y == arg0->y_position) && (oldPos.z == arg0->z_position)) {
        goto ret1;
    }
    *arg1 = 1;
    func_15107AE0((struct Vec15107A20 *)&arg0->x_position, (struct Vec15107A20 *)&arg0->unk38, (struct Vec15107A20 *)&arg0->y_velocity, (struct Vec15107A20 *)&arg0->old_x_position);
ret1:
    return 1;
}

void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_15143794(s32, s32, f32, f32 *);

void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4) {
    struct Vec15107A20 sp2C;
    f32 sp28;
    f32 sp24;

    func_1515C244(arg0, &sp2C.x, &sp28, &sp24);
    func_15143794((s16)arg1, (s16)arg2, sp28, &arg3->x);
    arg3->y *= arg0->unkF0;
    arg3->x += sp2C.x;
    arg3->y += sp2C.y;
    arg3->z += sp2C.z;
    if (arg4 != NULL) {
        *arg4 = sp2C;
    }
}

void func_15107AE0(struct Vec15107A20 *arg0, struct Vec15107A20 *arg1, struct Vec15107A20 *arg2, struct Vec15107A20 *arg3) {
    f32 *start = (f32 *)arg0;
    f32 *end = (f32 *)arg1;
    f32 *first = (f32 *)arg2;
    f32 *second = (f32 *)arg3;
    f32 dx;
    f32 dy;
    f32 dz;

    dx = end[0] - start[0];
    dy = end[1] - start[1];
    dz = end[2] - start[2];
    first[0] = start[0] + dx * 0.333f;
    first[1] = start[1] + dy * 0.333f;
    first[2] = start[2] + dz * 0.333f;
    second[0] = start[0] + dx * 0.6667f;
    second[1] = start[1] + dy * 0.6667f;
    second[2] = start[2] + dz * 0.6667f;
}

extern void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
extern void func_15143794(s32, s32, f32, f32 *);
extern void func_151C329C(void *, s32, s32);

void func_15107B78(struct127 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct { f32 x; f32 y; f32 z; } sp2C;
    struct { f32 x; f32 y; f32 z; } sp20;
    f32 sp1C;
    f32 sp18;

    if (arg0->unk1D4 != NULL) {
        func_1515C244(arg0, &sp20.x, &sp1C, &sp18);
        func_15143794((s16)arg1, (s16)arg2, sp1C, &sp2C.x);
        sp2C.y *= arg0->unkF0;
        sp2C.x += sp20.x;
        sp2C.y += sp20.y;
        sp2C.z += sp20.z;
        func_151C329C(&sp2C, (u8)arg3, arg4);
    }
}

struct S15107C1CCreated *func_15107C1C(struct127 *arg0, u8 arg1, void *arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, f32 arg7, f32 arg8, s16 arg9, s16 argA, u8 *argB, u8 argC, s32 argD) {
    struct S15107C1CPayload payload;
    struct S15107C1CCreated *created;
    struct S15107C1CInit init;
    struct S15107C1CTrace trace;
    struct Vec15107A20 vec;
    s32 flag;
    if (arg0->unk1D4 == NULL) {
        return NULL;
    }

    payload.unk0 = arg0;
    payload.unk4 = arg0->unique_id;
    payload.unk8 = *(struct S15107C1CWords3 *)arg2;
    payload.unk14 = arg1;

    if (arg5 == -1) {
        init.unk0 = 0x12C;
    } else {
        init.unk0 = arg5;
    }
    if (arg5 == -1) {
        flag = 0;
    } else {
        flag = 1;
    }
    init.unk2 = flag;
    init.unk34 = 1;
    init.unk38 = arg6;
    init.unk3C = arg7;
    init.unk40 = arg8;
    init.unk44 = arg9;
    init.unk46 = argA;
    *(struct S15107C1CBytes4 *)&init.unk48 = *(struct S15107C1CBytes4 *)argB;
    init.unk4C = 3;

    func_15107F54((s32 *)arg0, arg1, arg2, (s32)&init.unk4);
    func_15143794(arg3, arg4, 1.0f, &vec.x);
    func_15081690(arg0, init.unk4.x, init.unk4.y, init.unk4.z, vec.x, vec.y, vec.z, &trace, 300.0f, 0, 1, 1, -1, 0, 0);

    if (trace.unk59 == 0) {
        init.unk28.x = init.unk4.x;
        init.unk28.x += vec.x * 300.0f;
        init.unk28.y = init.unk4.y;
        init.unk28.y += vec.y * 300.0f;
        init.unk28.z = init.unk4.z;
        init.unk28.z += vec.z * 300.0f;
    } else {
        init.unk28 = trace.unk8;
    }

    func_15107AE0(&init.unk4, &init.unk28, &init.unk10, &init.unk1C);
    created = func_15105CE0(&init, 0x18, argC, argD);
    if (created != NULL) {
        memcpy(created->unk60, &payload, 0x18);
    }
    return created;
}

struct S15107E48 {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    u8 unk8[0xC];
    u8 unk14;
};

s32 func_15107E48(struct127 *arg0, u8 *arg1) {
    struct S15107E48 *data;
    struct127 *obj;
    struct Vec15107A20 oldPos;

    data = (struct S15107E48 *)arg0->unk60;
    obj = data->unk0;
    if ((obj->interaction_state == 0) || (obj->unique_id != data->unk4)) {
        return 0;
    }
    if (obj->unk1D4 == NULL) {
        return 0;
    }
    *arg1 = 0;
    oldPos = *(struct Vec15107A20 *)&arg0->x_position;
    func_15107F54((s32 *)obj, data->unk14, data->unk8, (s32)(struct Vec15107A20 *)&arg0->x_position);
    if ((oldPos.x == arg0->x_position) && (oldPos.y == arg0->y_position) && (oldPos.z == arg0->z_position)) {
        goto ret1;
    }
    *arg1 = 1;
    func_15107AE0((struct Vec15107A20 *)&arg0->x_position, (struct Vec15107A20 *)&arg0->unk38, (struct Vec15107A20 *)&arg0->y_velocity, (struct Vec15107A20 *)&arg0->old_x_position);
ret1:
    return 1;
}

void func_15143134(void *, s32, s32);

void func_15107F54(s32 *arg0, u8 arg1, void *arg2, s32 arg3) {
    func_15143134(arg2, arg3, arg0[0x1D4 / 4] + (arg1 << 6));
}

void func_15169850(s32, u8, s32, s32, s32);

void func_15107F98(s32 *arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0[0x18], arg0[0x18] + 4, (s32)arg0);
}
