#include <ultra64.h>
#include "functions.h"
#include "variables.h"


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
    TimedEffect base;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s8  unk30;
} Struct1500F378;

s32 func_1500F378(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct1500F378 *temp;

    temp = func_151491F4((func_150ADA20() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (temp != 0) {
        temp->unk28 = arg0;
        temp->unk2A = arg1;
        temp->unk2C = arg2;
        temp->unk2E = arg3;
        temp->unk30 = 1;
    }
}

void func_1500F40C(void) {
    SprayDesc descriptor;

    {
        extern EffectHeader *D_800D98D0[4]; // matching: the block-scope redeclaration changes IDO's store scheduling

        func_1500F378(0x1612, -0x553, -0x2DC, -0x69F);
        func_1500F378(0x1633, -0x553, -0x3C2, -0x69F);
        D_800D98D0[0] = func_1500F290(4054.0f, -1692.0f, 0.0f);
        D_800D98D0[1] = func_1500F290(3890.0f, -1692.0f, -78.0f);
        D_800D98D0[2] = func_1500F290(4380.0f, -1692.0f, -664.0f);
        D_800D98D0[3] = func_1500F290(4218.0f, -1692.0f, -740.0f);
    }
    descriptor.rate = 0.0f;
    descriptor.rateRange = 0.2400000244f;
    descriptor.halfWidth = 2.0f;
    descriptor.halfWidthRange = 6.0f;
    descriptor.speed = 7.0f;
    descriptor.speedRange = 10.0f;
    descriptor.gravity = 0.4970000088f;
    descriptor.gravityRange = 0.3150000274f;
    descriptor.pointCount = 3;
    descriptor.pointCountRange = 2;
    descriptor.yawSpread = 0x12;
    descriptor.pitch = -0x15;
    descriptor.pitchRange = 0xF;
    descriptor.unk1B = 0x9B;
    descriptor.unk1BRange = 0x64;
    descriptor.unkC = 0x29;
    descriptor.unkCRange = 0x29;
    descriptor.origin.x = 2686.0f;
    descriptor.origin.y = -1667.0f;
    descriptor.origin.z = 2063.0f;
    descriptor.extent.x = 2810.0f - descriptor.origin.x;
    descriptor.extent.y = 0;
    descriptor.extent.z = 1637.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 2810.0f;
    descriptor.origin.y = -1667.0f;
    descriptor.origin.z = 1637.0f;
    descriptor.extent.x = 2667.0f - descriptor.origin.x;
    descriptor.extent.y = -1668.0f - descriptor.origin.y;
    descriptor.extent.z = 1236.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 2667.0f;
    descriptor.origin.y = -1668.0f;
    descriptor.origin.z = 1236.0f;
    descriptor.extent.x = 2220.0f - descriptor.origin.x;
    descriptor.extent.y = -1676.0f - descriptor.origin.y;
    descriptor.extent.z = 1025.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 1894.0f;
    descriptor.origin.y = -1674.0f;
    descriptor.origin.z = 788.0f;
    descriptor.extent.x = 1841.0f - descriptor.origin.x;
    descriptor.extent.y = -1677.0f - descriptor.origin.y;
    descriptor.extent.z = 557.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 1841.0f;
    descriptor.origin.y = -1677.0f;
    descriptor.origin.z = 557.0f;
    descriptor.extent.x = 1594.0f - descriptor.origin.x;
    descriptor.extent.y = -1678.0f - descriptor.origin.y;
    descriptor.extent.z = 455.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 1594.0f;
    descriptor.origin.y = -1678.0f;
    descriptor.origin.z = 455.0f;
    descriptor.extent.x = 1400.0f - descriptor.origin.x;
    descriptor.extent.y = 0.0f;
    descriptor.extent.z = 613.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 1253.0f;
    descriptor.origin.y = -1673.0f;
    descriptor.origin.z = 668.0f;
    descriptor.extent.x = 1075.0f - descriptor.origin.x;
    descriptor.extent.y = -1682.0f - descriptor.origin.y;
    descriptor.extent.z = 572.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 1075.0f;
    descriptor.origin.y = -1682.0f;
    descriptor.origin.z = 572.0f;
    descriptor.extent.x = 872.0f - descriptor.origin.x;
    descriptor.extent.y = -1680.0f - descriptor.origin.y;
    descriptor.extent.z = 687.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
    descriptor.origin.x = 872.0f;
    descriptor.origin.y = -1680.0f;
    descriptor.origin.z = 687.0f;
    descriptor.extent.x = 772.0f - descriptor.origin.x;
    descriptor.extent.y = -1676.0f - descriptor.origin.y;
    descriptor.extent.z = 931.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    func_15189900(&descriptor, 1);
}
