#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct CB3D0_arg1 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

extern f32 D_800D2FC0[];
extern f32 D_800D2FD8[];
extern u8 D_800D2FEC[];
extern s32 D_800D3840;

void func_1509DF20(s32 arg0, struct CB3D0_arg1 *arg1) {
    if (arg1->unk4 == 1) {
        if (D_800D3840 == 3) {
            D_800D2FC0[arg1->unk8] = (f32)arg1->unkC * 0.0000152587890625f;
            D_800D2FD8[arg1->unk8] = (f32)arg1->unkC * 0.0000152587890625f;
            D_800D2FEC[arg1->unk8] = 1;
        }
    }
}

void func_1509DFB4(s32 arg0, s32 arg1) {
}

struct CB3D0_arg2 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

extern void func_15181E18(s32);
extern void func_15042D94(s32, s32, s32, char *, ...);
extern s32 func_15089BC0(void);
extern s32 func_1508A1BC(void);
extern s32 func_1508A6FC(void);
extern s32 func_1508BF14(void);
extern s32 func_1508C194(s32);
extern s32 func_1508CAD8(s32, s32, s32, s32, s32, s32);
extern s32 func_1508DA1C(void);
extern s32 func_1508DC24(void);
extern s32 func_1508DAEC(s32, s32);
extern s32 func_1508B9BC(void);
extern s32 func_1508D850(s32);
extern s32 func_1508E89C(s32, s32);
extern s32 func_15086D48(s32);
extern void func_15086CBC(s32, f32 *, f32 *, f32 *);
extern void func_150870D0(s32, s32, s32);
extern void func_15087350(s32, s32, f32);
extern void func_15087DCC(s32, s32);
extern void func_15087FEC(s32, s32);
extern void func_15087FC4(s32, s32);
extern s32 func_150880F8(s32, s32);
extern void func_1508B1D4(s32);
extern void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
extern s32 D_800D2450;

s32 func_1509DFC4(s32 arg0, s32 arg1, struct CB3D0_arg2 *arg2) {
    s32 id;
    f32 x;
    f32 y;
    f32 z;
    struct127 *obj;
    f32 scale;

    switch (arg1) {
    case 0:
        func_15181E18(0);
        return 0;
    case 1:
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        func_15042D94(0x96, 0x56, 1, "%d", (arg2->unk8 + 0x3B) / 60);
        D_8008FDBC |= 0x80;
        break;
    case 2:
        D_800D2450 = arg2->unk8;
        break;
    case 3:
        func_15093818(arg2->unk8);
        return 0;
    case 4:
        return func_15089BC0();
    case 5:
        return func_1508A1BC();
    case 6:
        return func_1508A6FC();
    case 7:
        return func_1508BF14();
    case 8:
        return func_1508C194(arg2->unk8);
    case 9:
        return func_1508CAD8(arg2->unk8, arg2->unkC, arg2->unk10, arg2->unk14, 0, 0);
    case 10:
        return func_1508DA1C();
    case 11:
        return func_1508DC24();
    case 12:
        return func_1508DAEC(arg2->unk8, arg2->unkC);
    case 13:
        return func_1508B9BC();
    case 14:
        return func_1508D850(arg2->unk8);
    case 15:
        return func_1508E89C(arg2->unk8, arg2->unkC);
    case 16:
        id = func_15086D48(arg2->unk8);
        if (id != 0xFF) {
            func_15086CBC(id, &x, &y, &z);
            func_1508B20C(x, y, z, arg2->unkC);
            return 0;
        }
        break;
    case 17:
        func_150870D0(arg2->unk8, arg2->unkC, arg2->unk10);
        return 0;
    case 18:
        func_15087350(arg2->unk8, arg2->unkC, 0.1f);
        return 0;
    case 19:
        func_15087DCC(arg2->unk8, arg2->unkC);
        return 0;
    case 20:
        func_15087FEC(arg2->unk8, arg2->unkC);
        return 0;
    case 21:
        func_15087FC4(arg2->unk8, arg2->unkC);
        return 0;
    case 22:
        return func_150880F8(arg2->unk8, arg2->unkC);
    case 23:
        func_1508B1D4(arg2->unk8);
        return 0;
    case 25:
        scale = 0.0000152587890625f;
        {
        struct108 *cam = D_800DBFF0;
        func_150495B0(&D_800D2FD8[arg2->unk8], arg2->unkC * scale, &D_800D2FC0[arg2->unk8],
                      arg2->unk10 * scale, arg2->unk14 * scale, cam->unk7B4);
        return D_800D2FD8[arg2->unk8] * 65536.0f;
        }
    case 26:
        scale = 0.0000152587890625f;
        {
        struct108 *cam = D_800DBFF0;
        func_15049688(&D_800D2FD8[arg2->unk8], arg2->unkC * scale, &D_800D2FC0[arg2->unk8],
                      arg2->unk10 * scale, arg2->unk14 * scale, cam->unk7B4);
        return D_800D2FD8[arg2->unk8] * 65536.0f;
        }
    case 24:
        obj = func_15083E90((u8)arg2->unk8);
        if (obj != NULL) {
            func_15060F28(obj, 0);
        }
        break;
    }
    return 0;
}


extern f32 func_150CF040(s32 arg0, s32 arg1);
extern s32 func_1508CA88(void);
extern s32 func_1508C9CC(void);
extern s32 func_1508C5B8(s32 arg0, s32 arg1);
extern s32 func_15089F9C(s32 arg0);
extern s32 func_1508E6D0(s32 arg0);
extern s32 func_150881CC(s32 arg0);
extern s32 func_15088218(s32 arg0);
extern s32 func_15088270(s32 arg0);
extern s32 func_150882B0(s32 arg0);
extern s32 func_150882E4(s32 arg0, s32 arg1);
extern s32 func_150887F8(void);
extern s32 func_1508B194(s32 arg0);
extern s8 D_800E0BD0;
extern s8 D_800E0BE0[];

s32 func_1509E3DC(s32 arg0, s32 arg1, struct CB3D0_arg2 *arg2) {
    struct127 *obj;
    s32 x;
    s32 z;

    switch (arg1) {
    case 0:
        return func_150CF040(arg2->unk8, arg2->unkC);
    case 1:
        x = arg2->unk8 - arg2->unk10;
        z = arg2->unkC - arg2->unk14;
        return sqrtf(x * x + z * z);
    case 2:
        return func_1508CA88();
    case 3:
        return func_1508C9CC();
    case 4:
        return func_1508C5B8(arg2->unk8, arg2->unkC);
    case 5:
        return D_800E0BD0;
    case 6:
        return func_15089F9C(arg2->unk8);
    case 7:
        return func_1508E6D0(arg2->unk8);
    case 8:
        return 0;
    case 9:
        return func_150881CC(arg2->unk8);
    case 10:
        return func_15088218(arg2->unk8);
    case 11:
        return func_15088270(arg2->unk8);
    case 12:
        return func_150882B0(arg2->unk8);
    case 13:
        return func_150882E4(arg2->unk8, arg2->unkC);
    case 14:
        return func_150887F8();
    case 15:
        return D_8008FDBC & 0x40;
    case 16:
        return D_800E0BE0[arg2->unk8];
    case 17:
        return func_1508B194(arg2->unk8);
    case 20:
    case 21:
        return D_800D2FD8[arg2->unk8] * 65536.0f;
    case 18:
        obj = func_15083E90((u8)arg2->unk8);
        if (obj != NULL) {
            x = obj->x_position;
            z = obj->z_position;
            return (x << 16) | (z & 0xFFFF);
        }
        return 0;
    case 19:
        obj = func_15083E90((u8)arg2->unk8);
        if (obj != NULL) {
            return (s16)obj->y_position;
        }
        return 0;
    }
end:
    return 0;
}
