#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[0x6];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
} Cam1512E8E0;

typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x6];
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8 pad24[0x10];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} Cmd1512E8E0;

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 pad6[0x3];
    /* 0x9 */ u8 links[5];
    /* 0xE */ u8 padE[0x2];
} Node1512E8E0;

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 pad6[0x12];
} Point1512E8E0;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0xAC];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ u8 pad138[0x7C];
    /* 0x1B4 */ s16 unk1B4;
    /* 0x1B6 */ u8 pad1B6[0xE];
    /* 0x1C4 */ s16 unk1C4;
    /* 0x1C6 */ u8 pad1C6[0x1A];
    /* 0x1E0 */ s16 unk1E0;
    /* 0x1E2 */ u8 pad1E2[0xE];
    /* 0x1F0 */ s16 unk1F0;
    /* 0x1F2 */ u8 pad1F2[0x4A];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 pad23E[0x34];
    /* 0x272 */ s16 unk272;
    /* 0x274 */ s16 unk274;
    /* 0x276 */ u8 pad276[0x2];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ Vec3f unk27C;
    /* 0x288 */ f32 unk288;
    /* 0x28C */ f32 unk28C;
    /* 0x290 */ f32 unk290;
    /* 0x294 */ u8 pad294[0x10];
    /* 0x2A4 */ f32 unk2A4;
    /* 0x2A8 */ f32 unk2A8;
    /* 0x2AC */ f32 unk2AC;
    /* 0x2B0 */ u8 pad2B0[0x48];
    /* 0x2F8 */ Vec3f pos;
    /* 0x304 */ u8 pad304[0x44];
    /* 0x348 */ f32 unk348;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x1C];
    /* 0x36C */ u16 * unk36C;
    /* 0x370 */ f32 unk370;
    /* 0x374 */ f32 unk374;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 unk37C;
    /* 0x380 */ u8 pad380[0x4];
    /* 0x384 */ f32 unk384;
    /* 0x388 */ u8 pad388[0x14];
    /* 0x39C */ f32 unk39C;
    /* 0x3A0 */ u8 pad3A0[0x30];
    /* 0x3D0 */ struct127 * unk3D0;
    /* 0x3D4 */ struct127 * unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x204];
    /* 0x5DC */ f32 unk5DC;
    /* 0x5E0 */ u8 pad5E0[0x10];
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x20];
    /* 0x614 */ Vec3f * unk614;
    /* 0x618 */ Vec3f unk618;
    /* 0x624 */ u8 pad624[0x4];
    /* 0x628 */ s32 unk628;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ u8 pad630[0x40];
    /* 0x670 */ f32 unk670;
    /* 0x674 */ u8 pad674[0x54];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ Cam1512E8E0 cam;
    /* 0x6FC */ Cmd1512E8E0 cmd;
    /* 0x734 */ s32 unk734;
    /* 0x738 */ s32 unk738;
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x138];
    /* 0x8F0 */ f32 unk8F0;
    /* 0x8F4 */ f32 unk8F4;
    /* 0x8F8 */ f32 unk8F8;
    /* 0x8FC */ f32 unk8FC;
    /* 0x900 */ f32 unk900;
    /* 0x904 */ f32 unk904;
    /* 0x908 */ f32 unk908;
    /* 0x90C */ f32 unk90C;
    /* 0x910 */ f32 unk910;
    /* 0x914 */ f32 unk914;
    /* 0x918 */ f32 unk918;
    /* 0x91C */ f32 unk91C;
    /* 0x920 */ u16 unk920;
    /* 0x922 */ u8 pad922[0x2];
    /* 0x924 */ f32 unk924;
    /* 0x928 */ f32 unk928;
    /* 0x92C */ u8 unk92C;
    /* 0x92D */ u8 pad92D[0x1B];
    /* 0x948 */ u8 unk948;
} Obj1512E8E0;

extern Node1512E8E0 *D_800D2350;
extern Point1512E8E0 *D_800DC024;

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
struct127 *func_1505EEF4(s32 arg0);
s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15124C38(struct108 *arg0, s32 arg1);
void func_151254F4(struct108 *arg0, s32 arg1);
void func_151287E0(struct108 *arg0, f32 *arg1, f32 *arg2);
void func_1512DD50(struct108 *arg0);
void func_1512E140(struct108 *arg0);
void func_1512FE00(struct108 *arg0, s32 arg1);

void func_1512E8E0(Obj1512E8E0 *arg0) {
    Cmd1512E8E0 *cmd;
    Cam1512E8E0 *cam;

    if (arg0->unk2C & 0x100) {
        return;
    }
    if (arg0->unk73C != 0) {
        return;
    }
    func_151239CC((struct108 *)arg0, 0xD);
    cmd = &arg0->cmd;
    cam = &arg0->cam;
    if (arg0->unk2C & 0x40000) {
        return;
    }
    if (cam->unk2 != cmd->unk4) {
        arg0->unk92C = 0;
        cam->unk2 = cmd->unk4;
        cam->unk6 = 0;
        func_151239CC((struct108 *)arg0, 0xC);
        func_151239CC((struct108 *)arg0, 0xD);
        func_151239CC((struct108 *)arg0, 2);
        func_151239CC((struct108 *)arg0, 7);
    }
    if ((cmd->type != 4) && (arg0->unk3D4->unk120 != 0) && (arg0->unk92C == 0)) {
        cam->unk6 = 1;
    }
    if (cam->unk6 != 0) {
        func_151239CC((struct108 *)arg0, 0xC);
    }
    if ((arg0->cam.unk0 != 0) && (cam->unk6 == 0) && (cam->unk8 == 0)) {
        cam->unk4 = cmd->type;
        if (cmd->type != 0xA) {
            func_1512FE00((struct108 *)arg0, cmd->unk8);
        }
        if (cmd->type == 4) {
            arg0->unk272 = -1;
            arg0->unk6C8 = 1;
            arg0->unk1C4 = arg0->unk1B4;
            arg0->unk1F0 = arg0->unk1E0;
            return;
        }
        if (cmd->type == 7) {
            f32 dx;
            s32 i;
            f32 minDist;
            struct17 spB8;
            struct17 spAC;
            struct17 spA0;
            struct17 sp94;
            struct17 sp88;
            struct17 best;
            struct17 sp70;
            f32 dy;
            Node1512E8E0 *node;
            f32 dz;

            node = &D_800D2350[func_15085DF8(arg0->pos.x, arg0->pos.y, arg0->pos.z, 7, 0)];
            minDist = 1.000000004e+14f;
            if (arg0->unk2C & 0x100) {
                return;
            }
            sp94.unk0 = node->x;
            sp94.unk4 = node->y;
            sp94.unk8 = node->z;
            func_15048F90(&sp94, (struct17 *)&arg0->pos, &spB8);
            for (i = 0; i < 5; i++) {
                s32 link;

                link = node->links[i];
                if ((u8)link != 0xFF) {
                    sp88.unk0 = D_800D2350[(u8)link].x;
                    sp88.unk4 = D_800D2350[(u8)link].y;
                    sp88.unk8 = D_800D2350[(u8)link].z;
                    func_150491EC(&sp94, &sp88, &spAC);
                    func_15049148(&spAC, func_150AD900(&spAC.unk0, &spB8.unk0), &spA0);
                    func_15048F20(&sp94, &spA0, &sp70);
                    dx = arg0->pos.x - sp70.unk0;
                    dy = arg0->pos.y - sp70.unk4;
                    dz = arg0->pos.z - sp70.unk8;
                    if (((dx * dx) + ((arg0->pos.y - sp70.unk4) * (arg0->pos.y - sp70.unk4)) + ((arg0->pos.z - sp70.unk8) * (arg0->pos.z - sp70.unk8))) < minDist) {
                        minDist = (dx * dx) + ((arg0->pos.y - sp70.unk4) * (arg0->pos.y - sp70.unk4)) + ((arg0->pos.z - sp70.unk8) * (arg0->pos.z - sp70.unk8));
                        best = sp70;
                    }
                }
            }
            if (func_15123934((struct108 *)arg0, arg0->unk2C, 0, arg0->unk134, 0xC) != 0) {
                arg0->unk6C8 = 1;
                arg0->unk84 &= 0x7FFFFFF9;
                arg0->unk920 = cmd->unk8;
                arg0->unk924 = cmd->unk1C;
                arg0->unk928 = cmd->unk20;
                if (arg0->unk920 & 1) {
                    arg0->unk84 |= 4;
                }
                if (arg0->unk920 & 2) {
                    arg0->unk84 |= 2;
                }
                if (arg0->unk920 & 7) {
                    arg0->unk84 |= 0x20000;
                }
                arg0->unk92C = 1;
            }
            if (arg0->unk920 & 0x20) {
                arg0->unk84 |= 8;
            } else {
                arg0->unk84 &= ~8;
            }
            arg0->unk910 = 1.5f;
            arg0->unk914 = 2.5f;
            if ((arg0->unk920 & 2) && (*arg0->unk36C & 7)) {
                arg0->unk84 |= 8;
                arg0->unk910 *= 0.25f;
                arg0->unk914 *= 0.25f;
            } else if (arg0->unk920 & 0x40) {
                if (arg0->unk3D0->xz_velocity > 30.0f) {
                    arg0->unk910 *= 7.0f;
                    arg0->unk914 *= 7.0f;
                } else {
                    arg0->unk910 *= 4.0f;
                    arg0->unk914 *= 4.0f;
                }
                arg0->unk84 |= 0x20000;
            }
            if (arg0->unk23C != 0) {
                arg0->unk918 = 0.0f;
                arg0->unk908 = arg0->unk910;
                arg0->unk91C = 0.0f;
                arg0->unk90C = arg0->unk914;
            } else {
                func_150495B0(&arg0->unk908, arg0->unk910, &arg0->unk918, 1.5f, 2.0f, arg0->unk7B4);
                func_150495B0(&arg0->unk90C, arg0->unk914, &arg0->unk91C, 1.5f, 2.0f, arg0->unk7B4);
            }
            if (!(arg0->unk920 & 0x10)) {
                if (arg0->unk23C != 0) {
                    arg0->unk8F4 = 0.0f;
                    arg0->unk904 = 0.0f;
                    arg0->pos.x = best.unk0;
                    arg0->pos.z = best.unk8;
                } else {
                    func_150495B0(&arg0->pos.x, best.unk0, &arg0->unk8F4, arg0->unk908, arg0->unk90C, arg0->unk7B4);
                    func_150495B0(&arg0->pos.z, best.unk8, &arg0->unk904, arg0->unk908, arg0->unk90C, arg0->unk7B4);
                }
            }
            if (!(arg0->unk920 & 8)) {
                if (arg0->unk23C != 0) {
                    arg0->pos.y = best.unk4;
                    arg0->unk8FC = 0.0f;
                } else {
                    func_150495B0(&arg0->pos.y, best.unk4, &arg0->unk8FC, arg0->unk908, arg0->unk90C, arg0->unk7B4);
                }
            }
            func_15125594((struct108 *)arg0);
            arg0->unk924 = cmd->unk1C;
            arg0->unk928 = cmd->unk20;
            arg0->unk374 = arg0->unk924;
            arg0->unk348 = arg0->unk928;
            if (arg0->unk920 & 4) {
                arg0->unk734 = cmd->unk4 | 0x4000;
                arg0->unk738 = 1;
            }
            return;
        }
        if (cmd->type == 5) {
            if (func_15123934((struct108 *)arg0, 0x40, 0, 0, 0xC) != 0) {
                arg0->unk84 &= ~0xE;
                arg0->unk84 |= 0x200;
                arg0->unk27C = arg0->pos;
            }
            if ((cmd->unk8 == 5) || (cmd->unk8 == 0x11)) {
                if (arg0->unk272 != cmd->unk1C) {
                    arg0->unk272 = cmd->unk1C;
                    D_800DC024 = (Point1512E8E0 *)D_800DC020;
                    D_800DC024 = &D_800DC024[arg0->unk272];
                    arg0->unk278 = cmd->unk20;
                    arg0->unk288 = D_800DC024->x;
                    arg0->unk28C = D_800DC024->y;
                    arg0->unk290 = D_800DC024->z;
                    arg0->unk5F0 |= 4;
                    arg0->pos.x = arg0->unk288;
                    arg0->unk27C.x = arg0->unk288;
                    arg0->pos.y = arg0->unk28C;
                    arg0->unk27C.y = arg0->unk28C;
                    arg0->pos.z = arg0->unk290;
                    arg0->unk27C.z = arg0->unk290;
                    func_15128774((struct108 *)arg0, arg0->unk3D0);
                    arg0->unk23C = 1;
                }
            } else {
                arg0->unk272 = cmd->unk1C;
                D_800DC024 = (Point1512E8E0 *)D_800DC020;
                D_800DC024 = &D_800DC024[arg0->unk272];
                arg0->unk288 = D_800DC024->x;
                arg0->unk28C = D_800DC024->y;
                arg0->unk290 = D_800DC024->z;
                arg0->unk278 = cmd->unk20;
            }
            arg0->unk274 = cmd->unk8;
            arg0->unk6C8 = 1;
            return;
        }
        if (cmd->type == 0xB) {
            cam->unk24 = cmd->unk1C;
            cam->unk28 = cmd->unk20;
            cam->unk2C = cmd->unk8;
            return;
        }
        if ((cmd->type == 0xA) || (cmd->type == 0xE)) {
            struct127 *obj;

            obj = (struct127 *)func_15083E0C(cmd->unk1C);
            obj = func_1505EEF4((s32)obj);
            if (obj != NULL) {
                f32 dist;

                if (func_15123934((struct108 *)arg0, 0x400, 0, arg0->unk134, 2) != 0) {
                    if (cmd->type == 0xE) {
                        func_151254F4((struct108 *)arg0, obj - D_800CC2D0);
                    }
                    arg0->unk84 &= ~8;
                    arg0->unk628 = 0;
                    arg0->unk614 = &arg0->unk618;
                    arg0->unk6C8 = 1;
                    arg0->unk62C = 0.0f;
                    arg0->unk5DC = 0.0f;
                    dist = sqrtf(((arg0->pos.x - obj->x_position) * (arg0->pos.x - obj->x_position)) + ((arg0->pos.z - obj->z_position) * (arg0->pos.z - obj->z_position)));
                    arg0->unk618.x = -sinf(arg0->unk37C * 0.01745329238f) * dist + arg0->unk2A4;
                    arg0->unk618.y = arg0->unk2A8;
                    arg0->unk618.z = -cosf(arg0->unk37C * 0.01745329238f) * dist + arg0->unk2AC;
                    arg0->unk384 = arg0->unk37C;
                }
                if (cmd->type == 0xE) {
                    arg0->unk618.x = D_800CC2D0[arg0->unk23D].x_position;
                    arg0->unk618.y = D_800CC2D0[arg0->unk23D].y_position;
                    arg0->unk618.z = D_800CC2D0[arg0->unk23D].z_position;
                } else if (arg0->unk23C != 0) {
                    arg0->unk618.x = obj->x_position;
                    arg0->unk618.y = obj->y_position;
                    arg0->unk618.z = obj->z_position;
                } else {
                    arg0->unk618.x += (obj->x_position - arg0->unk618.x) * 0.03999999911f;
                    arg0->unk618.y += (obj->y_position - arg0->unk618.y) * 0.03999999911f;
                    arg0->unk618.z += (obj->z_position - arg0->unk618.z) * 0.03999999911f;
                }
                if (cmd->unk8 != 0) {
                    arg0->unk374 = cmd->unk20;
                }
                if (cmd->unk8 != 0) {
                    f32 h;

                    h = cmd->unk8 - (arg0->unk2A8 - arg0->unk618.y);
                    arg0->unk34C = h;
                    cmd->unk34 = cmd->unk8;
                }
            }
            return;
        }
        if (cmd->type == 0xF) {
            if (func_15123934((struct108 *)arg0, 0x800000, 0, arg0->unk134, 2) != 0) {
                arg0->unk134 = 0;
                arg0->unk84 |= 0x200;
                arg0->unk84 &= ~8;
                func_1512DD50((struct108 *)arg0);
            }
            return;
        }
        if (cmd->type == 3) {
            if (func_15123934((struct108 *)arg0, 0x400, 0, arg0->unk134, 2) != 0) {
                arg0->unk628 = 0;
                arg0->unk84 |= 8;
                arg0->unk614 = &arg0->unk618;
                arg0->unk62C = 0.0f;
                arg0->unk5DC = 0.0f;
                arg0->unk374 = cmd->unk1C;
                arg0->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
                arg0->unk6C8 = 1;
            }
            arg0->unk618.x = cmd->x;
            arg0->unk618.y = cmd->y;
            arg0->unk618.z = cmd->z;
            arg0->unk374 = cmd->unk1C;
            return;
        }
        if (cmd->type == 2) {
            f32 d;

            if (func_15123934((struct108 *)arg0, 0x2000, 0, arg0->unk134, 2) != 0) {
                arg0->unk374 = cmd->unk1C;
                arg0->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
            }
            arg0->unk6C8 = 1;
            func_1512E140((struct108 *)arg0);
            arg0->unk370 = sqrtf(((arg0->pos.x - arg0->unk2A4) * (arg0->pos.x - arg0->unk2A4)) + ((arg0->pos.z - arg0->unk2AC) * (arg0->pos.z - arg0->unk2AC)));
            if (arg0->unk370 < arg0->unk374) {
                d = arg0->unk374 - arg0->unk370;
                arg0->pos.x += d * cam->unk14 * func_150AD78C(arg0->unk39C);
                arg0->pos.z += d * cam->unk14 * func_150AD780(arg0->unk39C);
                cam->unk14 += 0.004999999888f;
                cam->unk14 = (cam->unk14 > 0.03999999911f) ? 0.03999999911f : cam->unk14;
            }
            func_151287E0((struct108 *)arg0, NULL, NULL);
            return;
        }
        if ((cmd->type == 1) || (cmd->type == 0)) {
            f32 dist;
            f32 d;

            dist = sqrtf(((arg0->pos.x - cmd->x) * (arg0->pos.x - cmd->x)) + ((arg0->pos.z - cmd->z) * (arg0->pos.z - cmd->z)));
            if ((arg0->unk6C8 == 0) || (cmd->type == 0)) {
                if (func_15123934((struct108 *)arg0, arg0->unk2C, 0, arg0->unk134, 2) != 0) {
                    arg0->unk6C8 = 1;
                    arg0->unk374 = cmd->unk1C;
                    arg0->unk34C = cmd->unk20;
                    cmd->unk34 = cmd->unk20;
                    arg0->unk84 &= ~4;
                    if (cmd->type == 0) {
                        arg0->unk734 = cmd->unk4 | 0x4000;
                        arg0->unk738 = 1;
                    }
                }
            }
            arg0->unk374 = cmd->unk1C;
            arg0->unk34C = arg0->unk348 = cmd->unk20;
            cmd->unk34 = cmd->unk20;
            if ((dist < arg0->unk374) && (cmd->type == 1)) {
                d = arg0->unk374 - dist;
                arg0->pos.x += d * cam->unk14 * func_150AD78C(arg0->unk39C);
                arg0->pos.z += d * cam->unk14 * func_150AD780(arg0->unk39C);
                cam->unk14 += 0.002499999944f;
                cam->unk14 = (cam->unk14 > 0.03999999911f) ? 0.03999999911f : cam->unk14;
            }
            if (cmd->type == 1) {
                cam->unk18 += (cmd->x - cam->unk18) * 0.03999999911f;
                cam->unk20 += (cmd->z - cam->unk20) * 0.03999999911f;
                arg0->unk2A4 += (cam->unk18 - arg0->unk2A4) * 0.5f;
                arg0->unk2AC += (cam->unk20 - arg0->unk2AC) * 0.5f;
            }
            if (cmd->type != 0) {
                func_1512E140((struct108 *)arg0);
                func_15124C38((struct108 *)arg0, 0);
                cam->unk10 = 0.0f;
            }
            return;
        }
        if (cmd->type == 0xD) {
            func_15123934((struct108 *)arg0, arg0->unk2C, 0, arg0->unk134, 2);
            return;
        }
    }
    cam->unk24 = 0.0f;
    cam->unk28 = 0.0f;
    cam->unk2C = 0.0f;
    if (arg0->cam.unk0 == 0) {
        cam->unk6 = 0;
    }
    if ((arg0->unk6C8 == 1) && (cam->unk4 == 1)) {
        cam->unk18 = arg0->unk2A4 + (cam->unk18 - arg0->unk2A4) * 0.5f;
        cam->unk20 = arg0->unk2AC + (cam->unk20 - arg0->unk2AC) * 0.5f;
        cam->unk14 = 0.0f;
    }
    if (arg0->unk2C == 0x40) {
        arg0->unk272 = -1;
        func_151239CC((struct108 *)arg0, 0xC);
    }
    if (arg0->unk6C8 == 1) {
        if (cam->unk4 == 0) {
            cam->unk18 = arg0->unk2A4;
            cam->unk20 = arg0->unk2AC;
        }
        if (cam->unk4 == 0xE) {
            arg0->unk670 = 0.009999999776f;
            func_151254F4((struct108 *)arg0, arg0->unk23D);
        }
    }
    if (arg0->unk23C != 0) {
        cam->unk18 = arg0->unk2A4;
        cam->unk20 = arg0->unk2AC;
        cam->unk10 = 1.0f;
    } else {
        cam->unk18 += (arg0->unk2A4 - cam->unk18) * cam->unk10;
        cam->unk20 += (arg0->unk2AC - cam->unk20) * cam->unk10;
    }
    arg0->unk2A4 = cam->unk18;
    arg0->unk2AC = cam->unk20;
    cam->unk10 += 0.01499999966f;
    cam->unk10 = (cam->unk10 > 1.0f) ? 1.0f : cam->unk10;
    if ((cam->unk10 != 1.0f) && (cam->unk4 == 1)) {
        cam->unk8 = 1;
    } else {
        cam->unk8 = 0;
    }
    if (arg0->unk6C8 == 1) {
        arg0->unk272 = -1;
        arg0->unk614 = NULL;
        arg0->unk6C8 = 0;
        cmd->unk34 = 0;
        cmd->type = 0xD;
        arg0->unk8F0 = 0.0f;
        arg0->unk8F4 = 0.0f;
        arg0->unk8F8 = 0.0f;
        arg0->unk8FC = 0.0f;
        arg0->unk900 = 0.0f;
        arg0->unk904 = 0.0f;
        arg0->unk910 = 0.0f;
        arg0->unk908 = 0.0f;
        arg0->unk918 = 0.0f;
        arg0->unk914 = 0.0f;
        arg0->unk90C = 0.0f;
        arg0->unk91C = 0.0f;
        arg0->unk920 = 0;
        arg0->unk92C = 0;
        arg0->unk948 = 0;
        arg0->unk384 = arg0->unk37C;
        arg0->unk924 = 0.0;
        arg0->unk928 = 0.0;
        func_151239CC((struct108 *)arg0, 1);
        func_151239CC((struct108 *)arg0, 0xC);
        func_151239CC((struct108 *)arg0, 0xD);
        func_151239CC((struct108 *)arg0, 2);
        func_151239CC((struct108 *)arg0, 7);
    }
}

void func_1512FE00(struct108 *arg0, s32 arg1) {
    switch (arg1) {
    case 1:
        func_15123934(arg0, arg0->unk2C, 0, 0, 7);
        break;
    case 2:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 &= ~8;
        break;
    case 3:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10000000;
        break;
    case 8:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x80;
        break;
    case 9:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 &= ~8;
        arg0->unk84 |= 0x80;
        break;
    case 11:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x4000;
        break;
    case 12:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x8000;
        break;
    case 13:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x20000;
        break;
    case 16:
        func_15123934(arg0, 8, 0, arg0->unk134, 7);
        break;
    case 14:
        func_15123934(arg0, 2, 0, arg0->unk134, 7);
        break;
    case 18:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x100000;
        break;
    case 19:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x400000;
        break;
    case 21:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk190 = 0.0f;
        break;
    case 22:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x800;
        arg0->unk190 = -30.0f;
        break;
    case 25:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x40;
        break;
    case 23:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x1000000;
        break;
    case 24:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x80000000;
        break;
    case 26:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10;
        break;
    case 27:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x20;
        break;
    case 28:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10;
        arg0->unk84 &= ~8;
        break;
    case 29:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x1000010;
        arg0->unk84 &= ~8;
        break;
    case 30:
        func_15128774(arg0, arg0->unk3D0);
        break;
    }
}
