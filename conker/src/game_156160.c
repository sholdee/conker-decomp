#include <ultra64.h>
#include "functions.h"
// variables.h declares these as arrays/ints; the code uses them as pointers.
#define D_800D3098 D_800D3098_header
#define D_800BE728 D_800BE728_header
#define D_800DC010 D_800DC010_header
#define D_800C35EA D_800C35EA_header
#include "variables.h"
#undef D_800D3098
#undef D_800BE728
#undef D_800DC010
#undef D_800C35EA
extern struct178 *D_800D3098;
extern u16 **D_800BE728;
extern Camera *D_800DC010;
extern u8 D_800C35EA[];

struct Entry151298C0 {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 padC[0x18];
};

extern struct Entry151298C0 D_800DC028[];

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
    union {
        s32 index;          /* types 5, 6 */
        Cfg15128CB0 *cfg;   /* type 1 */
        Data15128CB0 *data; /* types 7, 8, 10 */
        f32 *value;         /* type 9 */
    } arg;
} Msg15128CB0;

/*
 * CameraL state: the same 0x9A0-byte object as Camera (include/structs.h), which is why it is
 * cast to Camera * for the functions.h prototypes. Fields here follow this file's usage.
 */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x114];
    /* 0x19C */ f32 unk19C;
    /* 0x1A0 */ f32 unk1A0;
    /* 0x1A4 */ f32 unk1A4;
    /* 0x1A8 */ f32 unk1A8;
    /* 0x1AC */ u8 pad1AC[0x90];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 index; /* this camera's slot in the per-camera tables */
    /* 0x23E */ u8 unk23E;
    /* 0x23F */ u8 pad23F[0x39];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ struct17 unk27C;
    /* 0x288 */ struct17 unk288;
    /* 0x294 */ u8 pad294[0x10];
    /* 0x2A4 */ struct17 unk2A4;
    /* 0x2B0 */ u8 pad2B0[0xC];
    /* 0x2BC */ struct17 unk2BC;
    /* 0x2C8 */ struct17 unk2C8;
    /* 0x2D4 */ u8 pad2D4[0xC];
    /* 0x2E0 */ struct17 unk2E0;
    /* 0x2EC */ struct17 unk2EC;
    /* 0x2F8 */ struct17 unk2F8;
    /* 0x304 */ struct17 unk304;
    /* 0x310 */ u8 pad310[0x4C];
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
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
    /* 0x3AC */ u8 pad3AC[0x14];
    /* 0x3C0 */ struct17 unk3C0;
    /* 0x3CC */ u8 pad3CC[4];
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
    /* 0x778 */ u8 pad778[0x3C];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x3C];
    /* 0x7F4 */ s16 unk7F4;
    /* 0x7F6 */ u8 pad7F6[0x2A];
    /* 0x820 */ f32 unk820;
    /* 0x824 */ u8 pad824[0x10];
    /* 0x834 */ u16 unk834;
    /* 0x836 */ u8 pad836[0x16];
    /* 0x84C */ u8 unk84C;
    /* 0x84D */ u8 pad84D[0xA0];
    /* 0x8ED */ u8 unk8ED;
    /* 0x8EE */ u8 pad8EE[0x76];
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ f32 unk96C;
    /* 0x970 */ f32 unk970;
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ f32 unk97C;
    /* 0x980 */ f32 unk980;
    /* 0x984 */ u8 pad984[0x1C];
} CameraL; /* size 0x9A0 */

s32 func_1509BE40(s32, s32, s32, s32);
void func_1512D560(CameraL *, s32, struct178 *);
Msg15128CB0 *func_1512D604(CameraL *);
s32 func_1512D6B0(CameraL *);
void func_1512D6F0(CameraL *);
void func_151C9ED4(CameraL *);
void func_1512E8E0(CameraL *);
void func_151C9BA0(CameraL *, s32);
void func_150A09D0(s32);
void func_150A11C4(struct127 *);
void func_151287E0(CameraL *, s32, s32);
void func_151283B8(CameraL *);
void func_1510B128(s32, f32, f32, f32, f32);
void func_1501B320(s32);
void func_1510E7A4(s32 *, s32 *, f32 *, f32 *, s32 *, s32, f32, f32, f32, f32, u16, s32, f32, f32);
void func_151236D0(CameraL *);
void func_150495B0(f32 *value, f32 target, f32 *velocity, f32, f32, f32);
s32 func_150A3FC4(f32, f32, s32, s32 *, f32 *);
void func_1512C150(CameraL *);
extern u8 D_800C3672;
extern s32 D_800C367C;
extern s32 D_800C3678;
extern struct17 D_80089544;
extern f32 D_800C3648;

/*
 * Per-frame camera message pump: handles a pending unk734 request, then drains the camera's
 * message queue (func_1512D604) and applies each message by type.
 * matching: in the braced cases, `break` follows the closing brace (as1 orders the case-end
 * branch by source line).
 */
void func_15128CB0(CameraL *cam) {
    Msg15128CB0 *msg;

    cam->unk6CC = 0;
    if (cam->unk734 != 0 && func_1512D6B0(cam)) {
        if (func_1509BE40(1, cam->unk734, 6, 0x9000) && cam->unk738) {
            func_1512D560(cam, 1, &D_800D3098[cam->unk734 & 0xFFF]);
            cam->unk700 = cam->unk734 & 0xFFF;
        } else {
            cam->unk734 = 0;
            cam->unk738 = 0;
        }
    }
    if (func_1512D6B0(cam)) {
        if (cam->unk2C & 1) {
            func_151239CC((Camera *)cam, 12);
        }
        return;
    }
    while (!func_1512D6B0(cam)) {
        msg = func_1512D604(cam);
        switch (msg->type) {
        case 5:
            cam->unk23E = cam->unk3D4->unk78;
            if (cam->unk23E == 0) {
                func_151239CC((Camera *)cam, 1);
                cam->unk3D4->unk198 = 0;
                cam->unk3D4->unk197 = 0;
                cam->unk5F8 = 1;
                cam->unk5FC = 2;
                cam->unk3D4->unk198 = 0;
                cam->unk73C = 0;
                func_15127EB8((Camera *)cam);
                D_80089524[cam->index] = 0;
                D_80089534[cam->index] = 0;
            } else {
                D_80089524[cam->index] = cam->unk3D4->unk197;
                D_80089534[cam->index] = cam->unk3D4->unk198;
            }
            func_1512D6F0(cam);
            if (cam->unk84C) {
                func_151C9ED4(cam);
                cam->unk84C = 0;
            }
            D_800DC010[msg->arg.index] = D_800DBFF0[cam->index];
            func_15123934((Camera *)cam, 0x40000, 0, 0, 10);
            cam->unk5F0 |= 4;
            cam->unk3A4 = -1.0f;
            break;
        case 6:
        {
            f32 angle;

            cam->unk23E = cam->unk3D4->unk78;
            if (D_80089520 != 0) {
                D_80089520--;
            } else if (cam->unk2C == 0x40000) {
                angle = cam->unk3A4;
                if (D_800D2DB0 != 999.0f) {
                    angle = D_800D2DB0;
                    D_800D2DB0 = 999.0f;
                }
                func_151239CC((Camera *)cam, 10);
                if (D_800C3672) {
                    cam->unk820 = 999.0f;
                    cam->unk834 = 0;
                    cam->unk7F4 = 0;
                }
                if (cam->unk834 == 0) {
                    D_800DBFF0[cam->index] = D_800DC010[msg->arg.index];
                    cam->unk36C = D_800BE728[cam->index];
                    cam->unk36A = D_800BE710[cam->index];
                    cam->unk23E = cam->unk3D4->unk78;
                    cam->unk23C = 1;
                    func_1512E8E0(cam);
                    if (D_80089534[cam->index] != 0) {
                        cam->unk36A |= 0x10;
                        *cam->unk36C |= 0x10;
                        angle = cam->unk774;
                    } else if (D_80089524[cam->index] != 0) {
                        if ((cam->unk23E == 12 && cam->unk3D0->unk84 != 0x76) || cam->unk23E == 27 || (*cam->unk36C & 0x10)) {
                            cam->unk36A |= 0x10;
                            *cam->unk36C |= 0x10;
                            cam->unk3D4->unk197 = D_80089524[cam->index];
                            func_151C9BA0(cam, cam->unk23E);
                            angle = cam->unk774;
                        } else {
                            cam->unk3D4->unk198 = 0;
                            cam->unk73C = 0;
                            D_80089524[cam->index] = 0;
                            D_80089534[cam->index] = 0;
                            func_15127EB8((Camera *)cam);
                        }
                    } else {
                        cam->unk3D4->unk198 = 0;
                        cam->unk73C = 0;
                        func_15127EB8((Camera *)cam);
                    }
                    func_15126138((Camera *)cam);
                    if (cam->unk5F0 & 2) {
                        angle = cam->unk3D0->unk40 - 180.0f;
                    }
                    cam->unk37C = angle;
                    cam->unk39C = angle * 0.01745329238f;
                    cam->unk2E0 = cam->unk2BC;
                    cam->unk2EC = cam->unk2F8;
                    cam->unk2C8 = cam->unk2BC;
                    cam->unk304 = cam->unk2F8;
                    func_150A09D0(D_800BE9E4);
                    func_150A11C4(&D_800CC2D0[cam->index]);
                    func_1512E8E0(cam);
                    func_151287E0(cam, 0, 0);
                    func_151283B8(cam);
                    func_15126138((Camera *)cam);
                    cam->unk5F0 |= 4;
                    cam->unk670 = 0.0f;
                    cam->unk678 = 0.0f;
                    cam->unk8ED = 1;
                    cam->unk23C = 1;
                    cam->unk674 = 1.0f;
                } else {
                    cam->unk36C = D_800BE728[cam->index];
                    cam->unk36A = D_800BE710[cam->index];
                    func_15127FEC((Camera *)cam, D_800C367C, D_800C3678);
                    cam->unk23C = 0;
                }
            }
        }
            break;
        case 8:
        {
            Data15128CB0 *data = msg->arg.data;
            cam->unk3A4 = data->unkC;
        }
            break;
        case 9:
            cam->unk364 = *msg->arg.value;
            break;
        case 10:
        {
            Data15128CB0 *data;

            data = msg->arg.data;
            cam->unk834 = data->unk10;
            if (data->unkC == 999.0f) {
                cam->unk820 = cam->unk3D0->unk40 - 180.0f;
            } else {
                cam->unk820 = data->unkC;
            }
        }
            break;
        case 7:
        {
            Data15128CB0 *data;
            f32 surface;
            struct17 pos;
            f32 angle;
            f32 x; // matching: stack-slot pads that place pos and angle in the frame
            f32 y;

            data = msg->arg.data;
            cam->unk2F8.unk0 = data->unk14;
            cam->unk2F8.unk4 = data->unk18;
            cam->unk2F8.unk8 = data->unk1C;
            if (data->unk0 == 0 || data->unk0 == 2) {
                cam->unk2A4.unk0 = data->unk20;
                cam->unk2A4.unk4 = data->unk24;
                cam->unk2A4.unk8 = data->unk28;
                cam->unk19C = data->unk30;
                cam->unk1A0 = data->unk34;
                cam->unk1A4 = data->unk30;
                cam->unk1A8 = data->unk34;
                cam->unk2BC = cam->unk2A4;
                cam->unk5EC = data->unk2C;
                cam->unk3A8 = 0.0f;
                cam->unk5E8 = 0.0f;
                cam->unk38C = 0.0f;
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
                cam->unk5EC = data->unk20;
                angle = data->unk24 * 0.01745329238f;
                pos.unk4 = sinf(angle) * -pos.unk8;
                pos.unk8 = cosf(angle) * pos.unk8;
                if (D_800DBFF4[0]) {
                    cam->unk37C = data->unk28;
                    cam->unk380 = data->unk28;
                    cam->unk39C = cam->unk37C * 0.01745329238f;
                    cam->unk3A0 = cam->unk380 * 0.01745329238f;
                } else {
                    x = cam->unk37C; // matching: dead store steers register allocation
                }
                angle = cam->unk37C;
                angle += func_15048A70(cam->unk37C, data->unk28) / 1.0f;
                angle *= 0.01745329238f;
                pos.unk0 = sinf(angle) * pos.unk8;
                pos.unk8 = cosf(angle) * pos.unk8;
                pos.unk0 += data->unk14;
                pos.unk4 += data->unk18;
                pos.unk8 += data->unk1C;
                cam->unk19C = data->unk2C;
                cam->unk1A0 = data->unk30;
                cam->unk1A4 = data->unk2C;
                cam->unk1A8 = data->unk30;
                cam->unk2A4.unk0 = pos.unk0;
                cam->unk2A4.unk4 = pos.unk4;
                cam->unk2A4.unk8 = pos.unk8;
                cam->unk2BC = cam->unk2A4;
                cam->unk3A8 = 0.0f;
                cam->unk5E8 = 0.0f;
                cam->unk38C = 0.0f;
            }
            if (D_800C35EA[data->unk4] == 1) {
                D_800C3648 = data->unk8;
                func_1510B128(cam->index, cam->unk19C, cam->unk1A0, 1.0f, 0.0f);
            }
            if (D_800DBFF4[0]) {
                func_1501B320(0);
            }
            if (!D_800BEAC0) {
                func_1510E7A4(&cam->unk644, &cam->unk648, &surface, &cam->unk360, &cam->unk640, 0,
                              cam->unk2F8.unk0, cam->unk2F8.unk4, cam->unk2F8.unk8, cam->unk2F8.unk4, 0, 0, -10000.0f, cam->unk2F8.unk4);
                func_151236D0(cam);
            }
        }
            break;
        case 1:
            cam->unk6FC = msg->arg.cfg->unk17;
            cam->unk718 = msg->arg.cfg->unk18;
            cam->unk71C = msg->arg.cfg->unk1C;
            cam->unk704 = msg->arg.cfg->unk20;
            cam->unk70C = msg->arg.cfg->unk0;
            cam->unk70E = msg->arg.cfg->unk2;
            cam->unk710 = msg->arg.cfg->unk4;
            cam->unk712 = msg->arg.cfg->unk6;
            cam->unk714 = msg->arg.cfg->unk8;
            cam->unk716 = msg->arg.cfg->unkA;
            cam->unk720 = msg->arg.cfg->unk28;
            cam->unk724 = msg->arg.cfg->unk24;
            cam->unk728 = msg->arg.cfg->unk30;
            cam->unk72C = msg->arg.cfg->unk2C;
            cam->unk6CC = 1;
            break;
        }
    }
}

/* Resets this camera's D_800DC028 entry when D_80089550 is set. */
void func_151298C0(CameraL *cam, s32 arg1) {
    if (D_80089550 != 0) {
        D_800DC028[cam->index].unk2 = 0;
        D_800DC028[cam->index].unk4 = -1.0f;
        D_800DC028[cam->index].unk8 = 1.0f / 90.0f;
    }
}


/*
 * Eases the camera point unk2F8 toward the point unk278 short of unk2BC on the line from
 * unk288 (snapping when unk23C is set), then re-queries func_1510E7A4 for it when needed.
 */
void func_15129934(CameraL *cam) {
    f32 x;
    struct17 dir;
    f32 z;
    f32 y;
    f32 dist;
    f32 surface;
    f32 dx;
    f32 dy;
    f32 dz;

    if (D_80089550 != 0 && cam->unk73C == 0) {
        cam->unk27C.unk0 = cam->unk288.unk0;
        cam->unk27C.unk4 = cam->unk288.unk4;
        cam->unk27C.unk8 = cam->unk288.unk8;
        func_15048F90(&cam->unk27C, &cam->unk2BC, &dir);
        func_1504917C(&dir, &dir);
        dx = cam->unk2BC.unk0 - cam->unk27C.unk0;
        dy = cam->unk2BC.unk4 - cam->unk27C.unk4;
        dz = cam->unk2BC.unk8 - cam->unk27C.unk8;
        dist = sqrtf(dx * dx + dy * dy + dz * dz) - cam->unk278;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        func_15049148(&dir, dist, &dir);
        x = cam->unk27C.unk0 + dir.unk0;
        if (cam->unk704 & 0xF) {
            y = cam->unk27C.unk4;
        } else {
            y = cam->unk27C.unk4 + dir.unk4 * 1.2f;
        }
        z = cam->unk27C.unk8 + dir.unk8;
        if (cam->unk23C) {
            cam->unk2F8.unk0 = x;
            cam->unk304.unk0 = x;
            cam->unk3C0.unk0 = 0; // matching: int 0, not 0.0f (a float zero gets promoted to a register)
        } else {
            func_150495B0(&cam->unk2F8.unk0, x, &cam->unk3C0.unk0, 1.0f, 3.0f, cam->unk7B4);
        }
        if (cam->unk23C) {
            cam->unk2F8.unk4 = y;
            cam->unk304.unk4 = y;
            cam->unk3C0.unk4 = 0;
        } else {
            func_150495B0(&cam->unk2F8.unk4, y, &cam->unk3C0.unk4, 1.0f, 2.0f, cam->unk7B4);
        }
        if (cam->unk23C) {
            cam->unk2F8.unk8 = z;
            cam->unk304.unk8 = z;
            cam->unk3C0.unk8 = 0;
        } else {
            func_150495B0(&cam->unk2F8.unk8, z, &cam->unk3C0.unk8, 1.0f, 3.0f, cam->unk7B4);
        }
        func_1512C150(cam);
        if (cam->unk23C || (cam->unk84 & 0x400000) ||
            !func_150A3FC4(cam->unk2F8.unk0, cam->unk2F8.unk8, cam->unk644, &cam->unk648, &cam->unk35C)) {
            func_1510E7A4(NULL, NULL, &surface, &cam->unk360, &cam->unk640, 0, cam->unk2F8.unk0, cam->unk2F8.unk4,
                          cam->unk2F8.unk8, cam->unk2F8.unk4, 0, 0, -10000.0f, cam->unk2F8.unk4);
            if (cam->unk364 != -10000.0f) {
                cam->unk360 = cam->unk364;
            }
            func_151236D0(cam);
        }
        cam->unk974 = 1.0f;
        cam->unk964 = 1.0f;
        cam->unk97C = 1.0f;
        cam->unk96C = 1.0f;
        cam->unk978 = 2.0f;
        cam->unk968 = 2.0f;
        cam->unk980 = 2.0f;
        cam->unk970 = 2.0f;
    }
}
