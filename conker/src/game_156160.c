#include <ultra64.h>
#include "functions.h"
#define D_800D3098 D_800D3098_header
#include "variables.h"
#undef D_800D3098
extern struct178 *D_800D3098;

struct Entry151298C0 {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 padC[0x18];
};

extern f32 D_800A3610;
extern struct Entry151298C0 D_800DC028[];

typedef struct {
    f32 x, y, z;
} Vec3_15128CB0;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 pad44[0x34];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[0xB];
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[0x111];
    /* 0x197 */ u8 unk197;
    /* 0x198 */ u8 unk198;
} Obj15128CB0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} Data15128CB0;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 padC[0xB];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Cfg15128CB0;

typedef struct {
    s32 type;
    void *data;
} Msg15128CB0;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x16C];
    /* 0x19C */ f32 unk19C;
    /* 0x1A0 */ f32 unk1A0;
    /* 0x1A4 */ f32 unk1A4;
    /* 0x1A8 */ f32 unk1A8;
    /* 0x1AC */ u8 pad1AC[0x90];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 unk23E;
    /* 0x23F */ u8 pad23F[0x65];
    /* 0x2A4 */ Vec3_15128CB0 unk2A4;
    /* 0x2B0 */ u8 pad2B0[0xC];
    /* 0x2BC */ Vec3_15128CB0 unk2BC;
    /* 0x2C8 */ Vec3_15128CB0 unk2C8;
    /* 0x2D4 */ u8 pad2D4[0xC];
    /* 0x2E0 */ Vec3_15128CB0 unk2E0;
    /* 0x2EC */ Vec3_15128CB0 unk2EC;
    /* 0x2F8 */ Vec3_15128CB0 unk2F8;
    /* 0x304 */ Vec3_15128CB0 unk304;
    /* 0x310 */ u8 pad310[0x50];
    /* 0x360 */ s32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ u8 pad368[2];
    /* 0x36A */ u16 unk36A;
    /* 0x36C */ u16 *unk36C;
    /* 0x370 */ u8 pad370[0xC];
    /* 0x37C */ f32 unk37C;
    /* 0x380 */ f32 unk380;
    /* 0x384 */ u8 pad384[8];
    /* 0x38C */ f32 unk38C;
    /* 0x390 */ u8 pad390[0xC];
    /* 0x39C */ f32 unk39C;
    /* 0x3A0 */ f32 unk3A0;
    /* 0x3A4 */ f32 unk3A4;
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x24];
    /* 0x3D0 */ Obj15128CB0 *unk3D0;
    /* 0x3D4 */ Obj15128CB0 *unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x210];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 unk5EC;
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[4];
    /* 0x5F8 */ s16 unk5F8;
    /* 0x5FA */ u8 pad5FA[2];
    /* 0x5FC */ s16 unk5FC;
    /* 0x5FE */ u8 pad5FE[0x42];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
    /* 0x64C */ u8 pad64C[0x24];
    /* 0x670 */ f32 unk670;
    /* 0x674 */ f32 unk674;
    /* 0x678 */ f32 unk678;
    /* 0x67C */ u8 pad67C[0x50];
    /* 0x6CC */ s16 unk6CC;
    /* 0x6CE */ u8 pad6CE[0x2E];
    /* 0x6FC */ s32 unk6FC;
    /* 0x700 */ s16 unk700;
    /* 0x702 */ u8 pad702[2];
    /* 0x704 */ s32 unk704;
    /* 0x708 */ u8 pad708[4];
    /* 0x70C */ s16 unk70C;
    /* 0x70E */ s16 unk70E;
    /* 0x710 */ s16 unk710;
    /* 0x712 */ s16 unk712;
    /* 0x714 */ s16 unk714;
    /* 0x716 */ s16 unk716;
    /* 0x718 */ s32 unk718;
    /* 0x71C */ s32 unk71C;
    /* 0x720 */ f32 unk720;
    /* 0x724 */ f32 unk724;
    /* 0x728 */ f32 unk728;
    /* 0x72C */ f32 unk72C;
    /* 0x730 */ u8 pad730[4];
    /* 0x734 */ s32 unk734;
    /* 0x738 */ s32 unk738;
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x36];
    /* 0x774 */ f32 unk774;
    /* 0x778 */ u8 pad778[0x7C];
    /* 0x7F4 */ s16 unk7F4;
    /* 0x7F6 */ u8 pad7F6[0x2A];
    /* 0x820 */ f32 unk820;
    /* 0x824 */ u8 pad824[0x10];
    /* 0x834 */ u16 unk834;
    /* 0x836 */ u8 pad836[0x16];
    /* 0x84C */ u8 unk84C;
    /* 0x84D */ u8 pad84D[0xA0];
    /* 0x8ED */ u8 unk8ED;
    /* 0x8EE */ u8 pad8EE[0xB2];
} Player15128CB0;

s32 func_1509BE40(s32, s32, s32, s32);
void func_1512D560(Player15128CB0 *, s32, struct178 *);
Msg15128CB0 *func_1512D604(Player15128CB0 *);
void func_1512D6F0(Player15128CB0 *);
void func_151C9ED4(Player15128CB0 *);
void func_1512E8E0(Player15128CB0 *);
void func_151C9BA0(Player15128CB0 *, s32);
void func_150A09D0(s32);
void func_150A11C4(struct127 *);
void func_151287E0(Player15128CB0 *, s32, s32);
void func_151283B8(Player15128CB0 *);
void func_1510B128(s32, f32, f32, f32, f32);
void func_1501B320(s32);
void func_1510E7A4(s32 a0, s32 a1, void *a2, void *a3, void *arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, u16 arg10, s32 arg11, f32 arg12, f32 arg13);
void func_151236D0(void *);
extern u8 D_800C3672;
extern s32 D_800C367C;
extern s32 D_800C3678;
extern Vec3_15128CB0 D_80089544;
extern f32 D_800C3648;

void func_15128CB0(Player15128CB0 *arg0) {
    Msg15128CB0 *msg;

    arg0->unk6CC = 0;
    if (arg0->unk734 != 0) {
        if (func_1512D6B0((u8 *)arg0)) {
            if (func_1509BE40(1, arg0->unk734, 6, 0x9000) && arg0->unk738) {
                func_1512D560(arg0, 1, &D_800D3098[arg0->unk734 & 0xFFF]);
                arg0->unk700 = arg0->unk734 & 0xFFF;
            } else {
                arg0->unk734 = 0;
                arg0->unk738 = 0;
            }
        }
    }
    if (func_1512D6B0((u8 *)arg0)) {
        if (arg0->unk2C & 1) {
            func_151239CC((struct108 *)arg0, 0xC);
        }
        return;
    }
    while (!func_1512D6B0((u8 *)arg0)) {
        msg = func_1512D604(arg0);
        switch (msg->type) {
        case 5:
            arg0->unk23E = arg0->unk3D4->unk78;
            if (arg0->unk23E == 0) {
                func_151239CC((struct108 *)arg0, 1);
                arg0->unk3D4->unk198 = 0;
                arg0->unk3D4->unk197 = 0;
                arg0->unk5F8 = 1;
                arg0->unk5FC = 2;
                arg0->unk3D4->unk198 = 0;
                arg0->unk73C = 0;
                func_15127EB8((struct108 *)arg0);
                D_80089524[arg0->unk23D] = 0;
                D_80089534[arg0->unk23D] = 0;
            } else {
                D_80089524[arg0->unk23D] = arg0->unk3D4->unk197;
                D_80089534[arg0->unk23D] = arg0->unk3D4->unk198;
            }
            func_1512D6F0(arg0);
            if (arg0->unk84C) {
                func_151C9ED4(arg0);
                arg0->unk84C = 0;
            }
            ((struct108 *)D_800DC010)[(s32)msg->data] = D_800DBFF0[arg0->unk23D];
            func_15123934((struct108 *)arg0, 0x40000, 0, 0, 10);
            arg0->unk5F0 |= 4;
            arg0->unk3A4 = -1.0f;
            break;
        case 6:
        {
            f32 angle;

            arg0->unk23E = arg0->unk3D4->unk78;
            if (D_80089520 != 0) {
                D_80089520--;
            } else if (arg0->unk2C == 0x40000) {
                angle = arg0->unk3A4;
                if (D_800D2DB0 != 999.0f) {
                    angle = D_800D2DB0;
                    D_800D2DB0 = 999.0f;
                }
                func_151239CC((struct108 *)arg0, 10);
                if (D_800C3672) {
                    arg0->unk820 = 999.0f;
                    arg0->unk834 = 0;
                    arg0->unk7F4 = 0;
                }
                if (arg0->unk834 == 0) {
                    D_800DBFF0[arg0->unk23D] = ((struct108 *)D_800DC010)[(s32)msg->data];
                    arg0->unk36C = (*(u16 ***)&D_800BE728)[arg0->unk23D];
                    arg0->unk36A = D_800BE710[arg0->unk23D];
                    arg0->unk23E = arg0->unk3D4->unk78;
                    arg0->unk23C = 1;
                    func_1512E8E0(arg0);
                    if (D_80089534[arg0->unk23D] != 0) {
                        arg0->unk36A |= 0x10;
                        *arg0->unk36C |= 0x10;
                        angle = arg0->unk774;
                    } else if (D_80089524[arg0->unk23D] != 0) {
                        if ((arg0->unk23E == 0xC && arg0->unk3D0->unk84 != 0x76) || arg0->unk23E == 0x1B || (*arg0->unk36C & 0x10)) {
                            arg0->unk36A |= 0x10;
                            *arg0->unk36C |= 0x10;
                            arg0->unk3D4->unk197 = D_80089524[arg0->unk23D];
                            func_151C9BA0(arg0, arg0->unk23E);
                            angle = arg0->unk774;
                        } else {
                            arg0->unk3D4->unk198 = 0;
                            arg0->unk73C = 0;
                            D_80089524[arg0->unk23D] = 0;
                            D_80089534[arg0->unk23D] = 0;
                            func_15127EB8((struct108 *)arg0);
                        }
                    } else {
                        arg0->unk3D4->unk198 = 0;
                        arg0->unk73C = 0;
                        func_15127EB8((struct108 *)arg0);
                    }
                    func_15126138((struct108 *)arg0);
                    if (arg0->unk5F0 & 2) {
                        angle = arg0->unk3D0->unk40 - 180.0f;
                    }
                    arg0->unk37C = angle;
                    arg0->unk39C = angle * 0.01745329238f;
                    arg0->unk2E0 = arg0->unk2BC;
                    arg0->unk2EC = arg0->unk2F8;
                    arg0->unk2C8 = arg0->unk2BC;
                    arg0->unk304 = arg0->unk2F8;
                    func_150A09D0(D_800BE9E4);
                    func_150A11C4(&D_800CC2D0[arg0->unk23D]);
                    func_1512E8E0(arg0);
                    func_151287E0(arg0, 0, 0);
                    func_151283B8(arg0);
                    func_15126138((struct108 *)arg0);
                    arg0->unk5F0 |= 4;
                    arg0->unk670 = 0.0f;
                    arg0->unk678 = 0.0f;
                    arg0->unk8ED = 1;
                    arg0->unk23C = 1;
                    arg0->unk674 = 1.0f;
                } else {
                    arg0->unk36C = (*(u16 ***)&D_800BE728)[arg0->unk23D];
                    arg0->unk36A = D_800BE710[arg0->unk23D];
                    func_15127FEC((struct108 *)arg0, D_800C367C, D_800C3678);
                    arg0->unk23C = 0;
                }
            }
        }
            break;
        case 8:
        {
            Data15128CB0 *data = (Data15128CB0 *)msg->data;
            arg0->unk3A4 = data->unkC;
        }
            break;
        case 9:
            arg0->unk364 = *(f32 *)msg->data;
            break;
        case 10:
        {
            Data15128CB0 *data;

            data = msg->data;
            arg0->unk834 = data->unk10;
            if (data->unkC == 999.0f) {
                arg0->unk820 = arg0->unk3D0->unk40 - 180.0f;
            } else {
                arg0->unk820 = data->unkC;
            }
        }
            break;
        case 7:
        {
            Data15128CB0 *data;
            s32 sp;
            Vec3_15128CB0 pos;
            f32 angle;
            f32 x;
            f32 y;

            data = msg->data;
            arg0->unk2F8.x = data->unk14;
            arg0->unk2F8.y = data->unk18;
            arg0->unk2F8.z = data->unk1C;
            if (data->unk0 == 0 || data->unk0 == 2) {
                arg0->unk2A4.x = data->unk20;
                arg0->unk2A4.y = data->unk24;
                arg0->unk2A4.z = data->unk28;
                arg0->unk19C = data->unk30;
                arg0->unk1A0 = data->unk34;
                arg0->unk1A4 = data->unk30;
                arg0->unk1A8 = data->unk34;
                arg0->unk2BC = arg0->unk2A4;
                arg0->unk5EC = data->unk2C;
                arg0->unk3A8 = 0.0f;
                arg0->unk5E8 = 0.0f;
                arg0->unk38C = 0.0f;
            } else {
                pos = D_80089544;
                while (data->unk20 > 180.0f) {
                    data->unk20 -= 360.0f;
                }
                while (data->unk24 > 180.0f) {
                    data->unk24 -= 360.0f;
                }
                while (data->unk28 > 180.0f) {
                    data->unk28 -= 360.0f;
                }
                arg0->unk5EC = data->unk20;
                angle = data->unk24 * 0.01745329238f;
                pos.y = sinf(angle) * -pos.z;
                pos.z = cosf(angle) * pos.z;
                if (D_800DBFF4[0]) {
                    arg0->unk37C = data->unk28;
                    arg0->unk380 = data->unk28;
                    arg0->unk39C = arg0->unk37C * 0.01745329238f;
                    arg0->unk3A0 = arg0->unk380 * 0.01745329238f;
                } else {
                    x = arg0->unk37C;
                }
                angle = arg0->unk37C; angle += func_15048A70(arg0->unk37C, data->unk28) / 1.0f; angle *= 0.01745329238f;
                pos.x = sinf(angle) * pos.z;
                pos.z = cosf(angle) * pos.z;
                pos.x += data->unk14;
                pos.y += data->unk18;
                pos.z += data->unk1C;
                arg0->unk19C = data->unk2C;
                arg0->unk1A0 = data->unk30;
                arg0->unk1A4 = data->unk2C;
                arg0->unk1A8 = data->unk30;
                arg0->unk2A4.x = pos.x;
                arg0->unk2A4.y = pos.y;
                arg0->unk2A4.z = pos.z;
                arg0->unk2BC = arg0->unk2A4;
                arg0->unk3A8 = 0.0f;
                arg0->unk5E8 = 0.0f;
                arg0->unk38C = 0.0f;
            }
            if ((&D_800C35EA)[data->unk4] == 1) {
                D_800C3648 = data->unk8;
                func_1510B128(arg0->unk23D, arg0->unk19C, arg0->unk1A0, 1.0f, 0.0f);
            }
            if (D_800DBFF4[0]) {
                func_1501B320(0);
            }
            if (!D_800BEAC0) {
                func_1510E7A4((s32)&arg0->unk644, (s32)&arg0->unk648, &sp, &arg0->unk360, &arg0->unk640, 0,
                              arg0->unk2F8.x, arg0->unk2F8.y, arg0->unk2F8.z, arg0->unk2F8.y, 0, 0, -10000.0f, arg0->unk2F8.y);
                func_151236D0(arg0);
            }
        }
            break;
        case 1:
            arg0->unk6FC = ((Cfg15128CB0 *)msg->data)->unk17;
            arg0->unk718 = ((Cfg15128CB0 *)msg->data)->unk18;
            arg0->unk71C = ((Cfg15128CB0 *)msg->data)->unk1C;
            arg0->unk704 = ((Cfg15128CB0 *)msg->data)->unk20;
            arg0->unk70C = ((Cfg15128CB0 *)msg->data)->unk0;
            arg0->unk70E = ((Cfg15128CB0 *)msg->data)->unk2;
            arg0->unk710 = ((Cfg15128CB0 *)msg->data)->unk4;
            arg0->unk712 = ((Cfg15128CB0 *)msg->data)->unk6;
            arg0->unk714 = ((Cfg15128CB0 *)msg->data)->unk8;
            arg0->unk716 = ((Cfg15128CB0 *)msg->data)->unkA;
            arg0->unk720 = ((Cfg15128CB0 *)msg->data)->unk28;
            arg0->unk724 = ((Cfg15128CB0 *)msg->data)->unk24;
            arg0->unk728 = ((Cfg15128CB0 *)msg->data)->unk30;
            arg0->unk72C = ((Cfg15128CB0 *)msg->data)->unk2C;
            arg0->unk6CC = 1;
            break;
        }
    }
}

void func_151298C0(void *arg0, s32 arg1)
{
    struct Entry151298C0 *entries;

    if (D_80089550 != 0) {
        entries = D_800DC028;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk2 = 0;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk4 = -1.0f;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk8 = D_800A3610;
    }
}

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
s32 func_150A3FC4(f32, f32, s32, void *, void *);
void func_1510E7A4(s32 a0, s32 a1, void *a2, void *a3, void *arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, u16 arg10, s32 arg11, f32 arg12, f32 arg13);
void func_151236D0(void *);
void func_1512C150(void *);

typedef struct {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x1B4];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D[0x3B];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ f32 unk27C;
    /* 0x280 */ f32 unk280;
    /* 0x284 */ f32 unk284;
    /* 0x288 */ f32 unk288;
    /* 0x28C */ f32 unk28C;
    /* 0x290 */ f32 unk290;
    /* 0x294 */ u8 pad294[0x28];
    /* 0x2BC */ f32 unk2BC;
    /* 0x2C0 */ f32 unk2C0;
    /* 0x2C4 */ f32 unk2C4;
    /* 0x2C8 */ u8 pad2C8[0x30];
    /* 0x2F8 */ f32 unk2F8;
    /* 0x2FC */ f32 unk2FC;
    /* 0x300 */ f32 unk300;
    /* 0x304 */ f32 unk304;
    /* 0x308 */ f32 unk308;
    /* 0x30C */ f32 unk30C;
    /* 0x310 */ u8 pad310[0x4C];
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ u8 pad368[0x58];
    /* 0x3C0 */ f32 unk3C0;
    /* 0x3C4 */ f32 unk3C4;
    /* 0x3C8 */ f32 unk3C8;
    /* 0x3CC */ u8 pad3CC[0x274];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
    /* 0x64C */ u8 pad64C[0xB8];
    /* 0x704 */ s32 unk704;
    /* 0x708 */ u8 pad708[0x34];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x1AC];
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ f32 unk96C;
    /* 0x970 */ f32 unk970;
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ f32 unk97C;
    /* 0x980 */ f32 unk980;
} Obj15129934;

void func_15129934(Obj15129934 *arg0) {
    f32 x;
    struct17 sp68;
    f32 z;
    f32 y;
    f32 dist;
    f32 sp58;
    f32 dx;
    f32 dy;
    f32 dz;

    if ((D_80089550 != 0) && (arg0->unk73C == 0)) {
        arg0->unk27C = arg0->unk288;
        arg0->unk280 = arg0->unk28C;
        arg0->unk284 = arg0->unk290;
        func_15048F90((struct17 *)&arg0->unk27C, (struct17 *)&arg0->unk2BC, &sp68);
        func_1504917C(&sp68, &sp68);
        dx = arg0->unk2BC - arg0->unk27C;
        dy = arg0->unk2C0 - arg0->unk280;
        dz = arg0->unk2C4 - arg0->unk284;
        dist = sqrtf((dx * dx) + (dy * dy) + (dz * dz)) - arg0->unk278;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        func_15049148(&sp68, dist, &sp68);
        x = arg0->unk27C + sp68.unk0;
        if (arg0->unk704 & 0xF) {
            y = arg0->unk280;
        } else {
            y = arg0->unk280 + (sp68.unk4 * 1.200000048f);
        }
        z = arg0->unk284 + sp68.unk8;
        if (arg0->unk23C != 0) {
            arg0->unk2F8 = x;
            arg0->unk304 = x;
            arg0->unk3C0 = 0;
        } else {
            func_150495B0(&arg0->unk2F8, x, &arg0->unk3C0, 1.0f, 3.0f, arg0->unk7B4);
        }
        if (arg0->unk23C != 0) {
            arg0->unk2FC = y;
            arg0->unk308 = y;
            arg0->unk3C4 = 0;
        } else {
            func_150495B0(&arg0->unk2FC, y, &arg0->unk3C4, 1.0f, 2.0f, arg0->unk7B4);
        }
        if (arg0->unk23C != 0) {
            arg0->unk300 = z;
            arg0->unk30C = z;
            arg0->unk3C8 = 0;
        } else {
            func_150495B0(&arg0->unk300, z, &arg0->unk3C8, 1.0f, 3.0f, arg0->unk7B4);
        }
        func_1512C150(arg0);
        if ((arg0->unk23C != 0) || (arg0->unk84 & 0x400000) ||
            (func_150A3FC4(arg0->unk2F8, arg0->unk300, arg0->unk644, &arg0->unk648, &arg0->unk35C) == 0)) {
            func_1510E7A4(0, 0, &sp58, &arg0->unk360, &arg0->unk640, 0, arg0->unk2F8, arg0->unk2FC, arg0->unk300, arg0->unk2FC, 0, 0, -10000.0f, arg0->unk2FC);
            if (-10000.0f != arg0->unk364) {
                arg0->unk360 = arg0->unk364;
            }
            func_151236D0(arg0);
        }
        arg0->unk974 = 1.0f;
        arg0->unk964 = 1.0f;
        arg0->unk97C = 1.0f;
        arg0->unk96C = 1.0f;
        arg0->unk978 = 2.0f;
        arg0->unk968 = 2.0f;
        arg0->unk980 = 2.0f;
        arg0->unk970 = 2.0f;
    }
}
