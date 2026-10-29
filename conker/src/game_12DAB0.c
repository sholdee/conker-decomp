#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60();
extern s32 func_15144E80(struct17 *, struct17 *, struct17 *, struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;

typedef struct {
    ObjectWithState197 *field_0x0;
    char pad_0[0x1E];
    u8 field_0x22;
    char pad_1[0x1];
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
} Field124ChildBlock;

typedef struct {
    char pad_0[0x18];
    s32 field_0x18;
    char pad_1[0x8C];
    s32 field_0xA8;
    char pad_2[0x24];
    s32 field_0xD0;
    u8 field_0xD4;
    char pad_3[0x4F];
    s32 field_0x124;
    char pad_4[0x48];
    s32 field_0x170;
    u8 field_0x174;
} Field124Owner;

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[4];
    u8 unk110;
    u8 unk111;
    u8 unk112[0xC];
} Struct15101F50;


#pragma GLOBAL_ASM("asm/nonmatchings/game_12DAB0/func_15100600.s")

typedef struct {
    u8 pad0[0x24];
    struct102 *unk24;
    struct102 *unk28;
    struct102 *unk2C;
    struct102 *unk30;
    struct102 *unk34;
} Block15101148;

void func_15101090(u8 *arg0) {
    Block15101148 *temp = (Block15101148 *)(arg0 + 0x110);

    if (*(u16 *)(arg0 + 0x1A0)) {
        func_100111C8(*(s16 *)(arg0 + 0x1A0));
    }
    if (temp->unk28 != NULL) {
        func_1516972C(temp->unk28);
    }
    if (temp->unk2C != NULL) {
        func_1516972C(temp->unk2C);
    }
    if (temp->unk24 != NULL) {
        func_1516972C(temp->unk24);
    }
    if (temp->unk30 != NULL) {
        func_1516972C(temp->unk30);
    }
    if (temp->unk34 != NULL) {
        func_1516972C(temp->unk34);
    }
    func_1513CA6C((struct210 *)arg0);
}



void func_15101148(u8 *arg0) {
    Block15101148 *temp = (Block15101148 *)(arg0 + 0x110);

    if (temp->unk28 != NULL) {
        func_1516979C(temp->unk28);
    }
    if (temp->unk2C != NULL) {
        func_1516979C(temp->unk2C);
    }
    if (temp->unk24 != NULL) {
        func_1516979C(temp->unk24);
    }
    if (temp->unk30 != NULL) {
        func_1516979C(temp->unk30);
    }
    if (temp->unk34 != NULL) {
        func_1516979C(temp->unk34);
    }
    func_1513CAA0((struct210 *)arg0);
}

void func_151011E8(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x124 + 0x110);
    temp->field_0x28 = 0;
    func_1513CA6C((struct210 *)arg0);
}

void func_15101210(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x124 + 0x110);
    temp->field_0x28 = 0;
    func_1513CAA0((struct210 *)arg0);
}

void func_151617C4(struct210 *arg0);

void func_15101238(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x18 + 0x110);

    temp->field_0x2C = 0;
    func_151617C4((struct210 *)arg0);
}

void func_151617E4(struct210 *arg0);

void func_15101260(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x18 + 0x110);

    temp->field_0x2C = 0;
    func_151617E4((struct210 *)arg0);
}

void func_1513173C(struct210 *);

void func_15101288(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    temp->field_0x24 = 0;
    func_1513173C((struct210 *)arg0);
}

void func_1513175C(struct210 *);

void func_151012B0(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    temp->field_0x24 = 0;
    func_1513175C((struct210 *)arg0);
}

void func_151012D8(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    temp->field_0x30 = 0;
    func_151411A4((struct210 *)arg0);
}

void func_15101300(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    temp->field_0x30 = 0;
    func_151411C4((struct210 *)arg0);
}

void func_15101328(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xA8 + 0x110);
    temp->field_0x34 = 0;
    func_1513173C((struct210 *)arg0);
}

void func_15101350(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xA8 + 0x110);
    temp->field_0x34 = 0;
    func_1513175C((struct210 *)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_12DAB0/func_15101378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_12DAB0/func_151019C4.s")

s16 *func_15101F50(Struct15101F50 *arg0, s16 arg1) {
    s32 unused;
    s16 *var_v0;
    s16 *ret;
    struct17 sp40;
    struct17 sp34;
    u8 sp33;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp33);
    ret = var_v0;
    if (var_v0) {
        if (sp33) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    if (!(arg0->unk110 & 1)) {
        return 0;
    }

    if (func_15144E80((struct17 *)arg0->unk112, &sp40, &sp34, 0) == 0) {
        return 0;
    }

    func_15145128(&sp40, &sp40, 0, 0);
    func_15145128(&sp34, &sp34, 0, 0);

    sp40.unk0 *= arg0->unk2C;
    sp40.unk4 *= arg0->unk2C;
    sp40.unk8 *= arg0->unk2C;
    sp34.unk0 *= arg0->unk30;
    sp34.unk4 *= arg0->unk30;
    sp34.unk8 *= arg0->unk30;

    var_v0[0] = (arg0->unk34 - sp40.unk0) + sp34.unk0;
    var_v0[1] = (arg0->unk38 - sp40.unk4) + sp34.unk4;
    var_v0[2] = (arg0->unk3C - sp40.unk8) + sp34.unk8;
    var_v0[3] = 0;
    var_v0[8] = (arg0->unk34 + sp40.unk0) + sp34.unk0;
    var_v0[9] = (arg0->unk38 + sp40.unk4) + sp34.unk4;
    var_v0[10] = (arg0->unk3C + sp40.unk8) + sp34.unk8;
    var_v0[11] = 0;
    var_v0[16] = (arg0->unk34 + sp40.unk0) - sp34.unk0;
    var_v0[17] = (arg0->unk38 + sp40.unk4) - sp34.unk4;
    var_v0[18] = (arg0->unk3C + sp40.unk8) - sp34.unk8;
    var_v0[19] = 0;
    var_v0[24] = (arg0->unk34 - sp40.unk0) - sp34.unk0;
    var_v0[25] = (arg0->unk38 - sp40.unk4) - sp34.unk4;
    var_v0[26] = (arg0->unk3C - sp40.unk8) - sp34.unk8;
    var_v0[27] = 0;

    return ret;
}

extern f32 D_800BE9A4;

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

typedef struct {
    u8 pad0[0x24];
    u8 *unk24;
    Sub151022AC_28 *unk28;
    Sub151022AC_2C *unk2C;
    u8 pad30[4];
    Sub151022AC_34 *unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0xC];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
} Block151022AC;

s32 func_151022AC(u8 *arg0) {
    Block151022AC *p;
    Sub151022AC_28 *s28;
    Sub151022AC_2C *s2C;
    Sub151022AC_34 *s34;
    f32 tmp;

    *(f32 *)(arg0 + 0x160) += *(f32 *)(arg0 + 0x164) * D_800BE9A4;
    p = (Block151022AC *)(arg0 + 0x110);
    p->unk50 = func_15144B68(*(f32 *)(arg0 + 0x160));
    p->unk58 += p->unk5C * D_800BE9A4;
    while (p->unk58 > 32767.0f) {
        p->unk58 -= 65536.0f;
    }
    while (p->unk58 < -32768.0f) {
        p->unk58 += 65536.0f;
    }
    p->unk68 -= D_800BE9A4;
    if (p->unk68 < 0.0f) {
        p->unk68 = func_150ADA68() * p->unk6C;
        p->unk60 = func_150ADA68() * (p->unk64 + p->unk64) - p->unk64;
    }
    p->unk5C += (p->unk60 - p->unk5C) * p->unk70;
    p->unk38 += (p->unk3C - p->unk38) * p->unk40;
    if (p->unk28 != NULL) {
        s28 = p->unk28;
        tmp = (func_150ADA68() * 0.200000003f + 0.400000006f) * p->unk38;
        s28->unk30 = tmp;
        s28->unk2C = tmp;
    }
    p->unk84 -= D_800BE9A4;
    if (p->unk84 < 0.0f) {
        p->unk84 = func_150ADA68() * p->unk88;
        p->unk78 = func_150ADA68() * p->unk80 + p->unk7C;
    }
    p->unk74 += (p->unk78 - p->unk74) * p->unk8C;
    arg0[0x5C] = p->unk74;
    if (p->unk28 != NULL) {
        p->unk28->unk5C = arg0[0x5C];
    }
    if (p->unk24 != NULL) {
        p->unk24[0x2B] = arg0[0x5C];
    }
    if (p->unk2C != NULL) {
        s2C = p->unk2C;
        s2C->unk14[0x2F] = (func_150ADA20() & 3) + 2;
    }
    if (p->unk34 != NULL) {
        s34 = p->unk34;
        tmp = func_150ADA68() * 10.0f + 120.0f;
        s34->unk3C = tmp;
        s34->unk38 = tmp;
    }
    return 1;
}

s32 func_151025F4(u8 *arg0, s32 arg1) {
    f32 *v1 = (f32 *)(arg0 + 0xA8);
    f32 sin0[2];
    f32 sin1;

    sin0[0] = sinf(v1[6]);
    sin1 = sinf(v1[7]);
    v1[0] = v1[4] * sin0[0] + v1[2];
    v1[1] = v1[5] * sin1 + v1[3];
    v1[6] = v1[8] * D_800BE9A4 + v1[6];
    v1[7] = v1[9] * D_800BE9A4 + v1[7];
    v1[6] = func_15144B68(v1[6]);
    v1[7] = func_15144B68(v1[7]);
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_12DAB0/func_151026BC.s")

void func_151027E8(struct210 *arg0) {
    s8 sp18[5];

    *(struct210 **)&sp18[0] = arg0;
    sp18[4] = *(u8 *)((s32)arg0 + 0x3B);
    func_151403A8(&sp18, 0x1A);
}

s32 func_1510281C(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0xD4 & 1)) {
        return 0;
    }
    return 1;
}

s32 func_15102884(struct210 *arg0, s32 arg1) {
    if (!(*(u8 *)((s32)arg0 + 0xAC) & 1)) {
        return 0;
    }
    return 1;
}

s32 func_151028AC(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0x174 & 1)) {
        return 0;
    }
    return 1;
}
