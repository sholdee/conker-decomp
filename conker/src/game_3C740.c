#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 func_151EF610(void);

struct LocalDef1500F290 {
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

void *func_1500F290(f32 arg0, f32 arg1, f32 arg2) {
    struct LocalDef1500F290 tmp;
    s32 temp;

    tmp.unk6 = 0x38;
    tmp.unk8 = 0;
    temp = func_151EF610();
    tmp.unkC = (temp % 0x1000) + 0x4000;
    tmp.unk0 = 0x20014;
    tmp.unk4 = 1;
    tmp.unk10 = 0xFF;
    tmp.unk11 = 0xFF;
    tmp.unk12 = 0;
    tmp.unk13 = 0;
    tmp.unk14 = 0;
    tmp.unk15 = 0xFF;
    tmp.unk18 = 0x30001;
    return func_1513C5B0(&tmp, 0, 0, 0, arg0, arg1, arg2, 110.0f, 110.0f, 0, 0, 0, 0xFF, 0);
}

typedef struct {
    u8  pad0[0x28];
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s8  unk30;
} Struct1500F378;

s32 func_1500F378(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct1500F378 *temp;

    temp = (Struct1500F378 *)func_151491F4((s16)((func_150ADA20() & 0x7F) + 0xA), 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (temp != 0) {
        temp->unk28 = arg0;
        temp->unk2A = arg1;
        temp->unk2C = arg2;
        temp->unk2E = arg3;
        temp->unk30 = 1;
    }
}

typedef struct Game3C740Descriptor {
    s16 angle, field2, field4, field6;
    f32 x, y, z;
    f32 width, field18, depth;
    f32 field20, field24, field28, field2C, field30, field34;
    s16 field38, field3A;
    f32 field3C, field40;
    s16 field44, field46;
    s32 field48, field4C;
} Game3C740Descriptor;

void func_15189900(void *, u8);

void func_1500F40C(void) {
    Game3C740Descriptor descriptor;

    {
        extern void *D_800D98D0[];

        func_1500F378(0x1612, -0x553, -0x2DC, -0x69F);
        func_1500F378(0x1633, -0x553, -0x3C2, -0x69F);
        D_800D98D0[0] = func_1500F290(4054.0f, -1692.0f, 0.0f);
        D_800D98D0[1] = func_1500F290(3890.0f, -1692.0f, -78.0f);
        D_800D98D0[2] = func_1500F290(4380.0f, -1692.0f, -664.0f);
        D_800D98D0[3] = func_1500F290(4218.0f, -1692.0f, -740.0f);
    }
    descriptor.field28 = 0.0f;
    descriptor.field2C = 0.2400000244f;
    descriptor.field30 = 2.0f;
    descriptor.field34 = 6.0f;
    descriptor.field20 = 7.0f;
    descriptor.field24 = 10.0f;
    descriptor.field3C = 0.4970000088f;
    descriptor.field40 = 0.3150000274f;
    descriptor.field48 = 3;
    descriptor.field4C = 2;
    descriptor.field2 = 0x12;
    descriptor.field4 = -0x15;
    descriptor.field6 = 0xF;
    descriptor.field38 = 0x9B;
    descriptor.field3A = 0x64;
    descriptor.field44 = 0x29;
    descriptor.field46 = 0x29;
    descriptor.x = 2686.0f;
    descriptor.y = -1667.0f;
    descriptor.z = 2063.0f;
    descriptor.width = 2810.0f - descriptor.x;
    descriptor.field18 = 0;
    descriptor.depth = 1637.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 2810.0f;
    descriptor.y = -1667.0f;
    descriptor.z = 1637.0f;
    descriptor.width = 2667.0f - descriptor.x;
    descriptor.field18 = -1668.0f - descriptor.y;
    descriptor.depth = 1236.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 2667.0f;
    descriptor.y = -1668.0f;
    descriptor.z = 1236.0f;
    descriptor.width = 2220.0f - descriptor.x;
    descriptor.field18 = -1676.0f - descriptor.y;
    descriptor.depth = 1025.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 1894.0f;
    descriptor.y = -1674.0f;
    descriptor.z = 788.0f;
    descriptor.width = 1841.0f - descriptor.x;
    descriptor.field18 = -1677.0f - descriptor.y;
    descriptor.depth = 557.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 1841.0f;
    descriptor.y = -1677.0f;
    descriptor.z = 557.0f;
    descriptor.width = 1594.0f - descriptor.x;
    descriptor.field18 = -1678.0f - descriptor.y;
    descriptor.depth = 455.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 1594.0f;
    descriptor.y = -1678.0f;
    descriptor.z = 455.0f;
    descriptor.width = 1400.0f - descriptor.x;
    descriptor.field18 = 0.0f;
    descriptor.depth = 613.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 1253.0f;
    descriptor.y = -1673.0f;
    descriptor.z = 668.0f;
    descriptor.width = 1075.0f - descriptor.x;
    descriptor.field18 = -1682.0f - descriptor.y;
    descriptor.depth = 572.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 1075.0f;
    descriptor.y = -1682.0f;
    descriptor.z = 572.0f;
    descriptor.width = 872.0f - descriptor.x;
    descriptor.field18 = -1680.0f - descriptor.y;
    descriptor.depth = 687.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.x = 872.0f;
    descriptor.y = -1680.0f;
    descriptor.z = 687.0f;
    descriptor.width = 772.0f - descriptor.x;
    descriptor.field18 = -1676.0f - descriptor.y;
    descriptor.depth = 931.0f - descriptor.z;
    descriptor.angle = (s16)(func_150484A0(descriptor.width, descriptor.depth) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
}
