#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct Node1518BA90 {
    u8 pad0[8];
    struct Node1518BA90 *next;
    u8 padC[0x18];
    void *owner;
    u8 type;
} Node1518BA90;

typedef struct Spawn1518BA90 {
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s16 field10;
    s16 field12;
    void *owner;
    u8 type;
    s8 field19;
    s8 field1A;
} Spawn1518BA90;

typedef struct Owner1518BA90 {
    u8 pad0[4];
    u8 kind;
    u8 pad5[0x36];
    u8 type;
} Owner1518BA90;

extern u8 D_800DCE50[];
extern f32 D_800A7434;
extern f32 D_800A7438;
extern f32 D_800A743C;
s32 func_1518BCD0(void *arg0, u8 arg1, s32 arg2);

s32 func_1518BA90(void *arg0, u8 arg1, s32 arg2) {
    u8 *block;
    Spawn1518BA90 spawn;
    Node1518BA90 *node;
    s32 i;

    if (arg0 == 0) {
        return 0;
    }
    i = 0;
    do {
        block = D_800DCE50 + i * 0x1A0;
        for (node = *(Node1518BA90 **)(block + 0x7C); node != 0; node = node->next) {
            if (node->owner == arg0 ||
                node->type == ((Owner1518BA90 *)arg0)->type) {
                return (s32)node;
            }
        }
        i++;
    } while ((u8 *)&D_800DD190 != D_800DCE50 + i * 0x1A0);

    switch (*(u8 *)((u8 *)arg0 + 4) + 1) {
    case 0:
        return 0;
    case 8:
        spawn.field12 = 0x37;
        spawn.field10 = 4;
        spawn.field19 = 5;
        spawn.field1A = 0x13;
        spawn.x = D_800A7434;
        spawn.y = D_800A7438;
        spawn.z = D_800A743C;
        spawn.scale = 5.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        spawn.field12 = 0;
        spawn.field10 = 5;
        spawn.field19 = 5;
        spawn.field1A = 0x13;
        spawn.x = 10.0f;
        spawn.y = 10.0f;
        spawn.z = 10.0f;
        spawn.scale = 10.0f;
        break;
    default:
        return 0;
    }
    spawn.owner = arg0;
    spawn.type = *(u8 *)((u8 *)arg0 + 0x3B);
    return func_1518BCD0(&spawn, arg1, arg2);
}

extern u8 D_800DCE50[];

typedef struct Node1518BBF4 {
    u8 pad0[0x8];
    struct Node1518BBF4 *unk8;
    u8 padC[0x18];
    struct Node1518BBF4 *unk24;
    u8 unk28;
    u8 pad29[0x12];
    u8 unk3B;
} Node1518BBF4;

void func_1518BBF4(Node1518BBF4 *arg0) {
    s32 i;
    Node1518BBF4 *node;

    if (arg0 != NULL) {
        for (i = 0; i != 2; i++) {
            D_800DD190++;
            node = ((Node1518BBF4 *(*)[104])D_800DCE50)[i][31];
            while (node != NULL) {
                ((Node1518BBF4 **)D_800DD198)[D_800DD190] = node->unk8;
                node->unk24 = arg0;
                if (arg0 != NULL || node->unk28 == arg0->unk3B) {
                    func_1516972C((struct102 *)node);
                }
                node = ((Node1518BBF4 **)D_800DD198)[D_800DD190];
            }
            {
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
}


s32 func_15167A68(s32, s32, s32, s32, s32, s32);

typedef struct {
    u8 pad_0x0[0x10];
    u8 field_0x10[0x1C];
    s32 field_0x2C;
    s32 field_0x30;
} EffectObject1F;

s32 func_1518BCD0(void *arg0, u8 arg1, s32 arg2) {
    EffectObject1F *temp_v0;

    temp_v0 = (EffectObject1F *)func_15167A68(0x1F, arg2, 0x44, 1, (u8)arg1, 1);
    if (temp_v0 == 0) {
        return 0;
    }
    memcpy(temp_v0->field_0x10, (void *)arg0, 0x1C);
    temp_v0->field_0x2C = func_150ADA20() & 0x1F;
    temp_v0->field_0x30 = func_150ADA20() & 0x1F;
    return (s32)temp_v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B8F40/func_1518BD60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B8F40/func_1518C0B8.s")

s32 func_1518C540(void* arg0) {
    s32 v1;
    void* p;

    p = *(void**)((u8*)arg0 + 0x98);
    v1 = *(s16*)((u8*)arg0 + 0x1C) << 3;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    *(s8*)((u8*)p + 0x1B) = v1;
    if ((v1 & 0xFF) < 0) {
        return 0;
    }
    return 1;
}

void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(s32 arg0, s32 arg1, s32 arg2);

typedef struct {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    s32 field_0xC;
    s32 field_0x10;
} EffectPoint;

typedef struct {
    u8 pad_0x0[0x20];
    s8 field_0x20;
} EffectState;

typedef struct {
    u8 pad_0x0[0x1];
    u8 field_0x1;
    u8 pad_0x2[0xA];
    u8 field_0xC;
    u8 pad_0xD[0x20];
    s8 field_0x2D;
    u8 pad_0x2E[0x66];
    EffectPoint *field_0x94;
    EffectState *field_0x98;
} EffectEmitter;

typedef struct {
    u8 pad_0x0[0x80];
    s32 field_0x80;
    u8 pad_0x84[0x6];
    s16 field_0x8A;
    s16 field_0x8C;
    s16 field_0x8E;
    s16 field_0x90;
    s16 field_0x92;
    s16 field_0x94;
    s16 field_0x96;
    s16 field_0x98;
    u8 pad_0x9A[0x6];
    s8 field_0xA0;
    s8 field_0xA1;
    s8 field_0xA2;
    u8 pad_0xA3[0x5];
} EffectSpawnPacket;

s32 func_1518C57C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    EffectSpawnPacket sp38;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp38.field_0x80 = 0;
    sp38.field_0x8A = 0x100;
    sp38.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp38.field_0x8E = (s32)arg4;
    sp38.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp38.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp38.field_0x92 = sp38.field_0x94;
    sp38.field_0xA2 = -1;
    sp38.field_0xA0 = 0x12;
    sp38.field_0x98 = 0xC;
    sp38.field_0xA1 = 0;
    sp38.field_0x96 = (s32)arg4;
    func_1516865C(&sp38, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp38, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    return 1;
}

u8 func_151D8E20(void);

s32 func_1518C69C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnPacket sp50;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp50.field_0x80 = 0;
    sp50.field_0x8A = 0x100;
    sp50.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp50.field_0x8E = (s32)arg4;
    sp50.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp50.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp50.field_0x92 = sp50.field_0x94;
    sp50.field_0xA2 = -1;
    sp50.field_0xA0 = 0x12;
    sp50.field_0x98 = 0xC;
    sp50.field_0xA1 = 0;
    sp50.field_0x96 = (s32)arg4;
    func_1516865C(&sp50, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp50, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    {
    u8 id;
    f32 pos[3];
    pos[0] = base[arg0->field_0x2D].field_0x0;
    pos[1] = arg4;
    pos[2] = base[arg0->field_0x2D].field_0x8;
    id = func_151D8E20();
    func_151DBCBC(id, func_150ADA68() * 40.0f + 100.0f, 0xFF, arg5, pos, arg0->field_0xC, arg0->field_0x1);
    }
    return 1;
}

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    u8 unk28;
} SubA1518C850;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB1518C850;

void func_1518C850(struct102 *arg0, s32 arg1, u8 arg2) {
    SubA1518C850 *dst = (SubA1518C850 *)arg0;
    ArgB1518C850 *b = (ArgB1518C850 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        b0 = b->unk0;
        if (b0 == dst->unk24) {
            dst->unk24 = b->u4.w4;
            dst->unk28 = b->unk9;
        } else {
            if (dst->unk24 == b->u4.w4) {
                dst->unk24 = b0;
                dst->unk28 = b->unk8;
            }
trailing_label_1518C850:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == dst->unk24) || (b->u4.b4 == dst->unk28)) {
            func_1516972C(arg0);
        }
    }
}
