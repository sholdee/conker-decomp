#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A34A0;
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
void func_15123A54(struct108 *arg0);
void func_1512E140(struct108 *arg0);
void func_1512A390(struct108 *arg0);


extern f32 D_800894C0;
extern struct17 D_800894C8;
void func_15123070(struct108 *arg0);

#define SQ(x) ((x) * (x))
#define FLD(o) (*(f32 *)((u8 *)arg0 + (o)))

void func_15122440(struct108 *arg0) {
    struct17 v = D_800894C8;
    f32 tmp;
    f32 yaw;
    f32 pitch;
    f32 s;
    f32 x;
    f32 z;
    f32 y;

    yaw = (arg0->unk3D0->unk40 + 90.0f) * 0.01745329238f;
    func_1512A390(arg0);
    if (!(arg0->unk5F0 & 1) && (arg0->unk3D0->unk102 == 0)) {
        yaw = arg0->unk39C - 1.570796371f;
    }
    if (arg0->unk86C != 999.0f) {
        pitch = arg0->unk86C;
    } else if (arg0->unk3D0->health == 0) {
        pitch = 0.0f;
    } else {
        pitch = (arg0->unk3D0->unk102 ? 10.0f : 0.0f) + arg0->unk3D0->unkB8;
    }
    if (arg0->unk3D0->unk102) {
        pitch *= 0.5f;
    }
    pitch = (pitch < -88.0f) ? -88.0f : ((pitch > 88.0f) ? 88.0f : pitch);
    if (fabsf(D_800894C0 - pitch) < 10.0f) {
        pitch = D_800894C0;
    } else {
        D_800894C0 = pitch;
    }
    pitch *= 0.01745329238f;
    func_15123070(arg0);
    if (*arg0->unk36C & 0x10) {
        v.unk0 = 150.0f;
    } else {
        v.unk0 = arg0->unk374;
    }
    if ((arg0->unk3D0->unk102 != 0) && (arg0->unk3E8 == 0)) {
        if (FLD(0x370) < arg0->unk374) {
            v.unk0 = FLD(0x370);
        }
    }
    tmp = cosf(pitch) * v.unk0;
    v.unk4 = sinf(pitch) * v.unk0;
    v.unk0 = tmp;
lbl:
    {
    f32 c;
    s = sinf(yaw);
    c = cosf(yaw);
    FLD(0x3DC) = yaw;
    do {
    v.unk8 = -v.unk0 * s;
    v.unk0 = v.unk0 * c;
    v.unk4 = (v.unk4 < -200.0f) ? -200.0f : ((v.unk4 > 200.0f) ? 200.0f : v.unk4);
    } while (0);
    v.unk0 += arg0->unk2BC;
    v.unk4 += arg0->unk2C0;
    v.unk8 += arg0->unk2C4;
    }
    {
    f32 accel = 1.0f;
    f32 maxv = 2.200000048f;
    x = v.unk0;
    y = v.unk4;
    z = v.unk8;
    if (arg0->unk23C) {
        arg0->unk2F8 = x;
        arg0->unk2FC = y;
        arg0->unk300 = z;
        FLD(0x3C0) = 0.0f;
        FLD(0x3C4) = 0.0f;
        FLD(0x3C8) = 0.0f;
    } else {
        accel += accel;
        maxv += maxv;
        if (arg0->unk3D0->unk102) {
            accel += accel;
            maxv += maxv;
        }
        func_150495B0(&arg0->unk2F8, x, &FLD(0x3C0), accel, maxv, arg0->unk7B4);
        func_150495B0(&arg0->unk2FC, y, &FLD(0x3C4), accel, maxv, arg0->unk7B4);
        func_150495B0(&arg0->unk300, z, &FLD(0x3C8), accel, maxv, arg0->unk7B4);
    }
    }
    if ((arg0->unk3D0->unk102 == 0) && !(arg0->unk36A & 0x10)) {
        if (sqrtf(SQ(arg0->unk2F8 - arg0->unk2BC) + SQ(arg0->unk2FC - arg0->unk2C0) + SQ(arg0->unk300 - arg0->unk2C4)) < 120.0f) {
            struct17 dir;
            func_150491EC((struct17 *)&arg0->unk2BC, (struct17 *)&arg0->unk2F8, &dir);
            arg0->unk2F8 = arg0->unk2BC + 120.0f * dir.unk0;
            arg0->unk2FC = arg0->unk2C0 + 120.0f * dir.unk4;
            arg0->unk300 = arg0->unk2C4 + 120.0f * dir.unk8;
        }
    }
    if (arg0->unk2FC < arg0->unk35C + 30.0f) {
        arg0->unk2FC = arg0->unk35C + 30.0f;
    }
    arg0->unk5E8 += (FLD(0x5EC) - arg0->unk5E8) * 0.150000006f;
    FLD(0x370) = sqrtf(SQ(arg0->unk2F8 - arg0->unk2BC) + SQ(arg0->unk300 - arg0->unk2C4));
    arg0->unk348 = FLD(0x344) = arg0->unk2FC - arg0->unk354;
}

void func_15122980(struct108 *arg0) {
    struct17 sp44;
    struct17 sp38;
    f32 temp_f12;
    f32 temp_f14;

    if (arg0->unkDC != 4) {
        func_15048F90((struct17 *)&arg0->unk2F8, (struct17 *)&arg0->unk2C8, &sp44);
        func_15048F90((struct17 *)&arg0->unk2F8, (struct17 *)&arg0->unk2BC, &sp38);
        temp_f12 = func_15048FC8(&sp44);
        temp_f14 = func_15048FC8(&sp38);
        if (*(s16 *)&arg0->unk298 == 0) {
            *(f32 *)((u8 *)arg0 + 0x8D4) = 0.0f;
        } else {
            if (((arg0->unk84 << 11) < 0) || ((arg0->unk390 > 180.0f) && (arg0->unk390 < 270.0f))) {
                *(f32 *)((u8 *)arg0 + 0x8D4) = 0.75f;
            } else {
                *(f32 *)((u8 *)arg0 + 0x8D4) = 0.5f;
            }
        }

        func_150495B0((f32 *)((u8 *)arg0 + 0x8DC), *(f32 *)((u8 *)arg0 + 0x8D4), (f32 *)((u8 *)arg0 + 0x8D8), 1.0f, 2.0f, arg0->unk7B4);
        arg0->unk37C += func_15048A70(temp_f12, temp_f14) * *(f32 *)((u8 *)arg0 + 0x8DC);
        arg0->unk39C = arg0->unk37C * D_800A34A0;
    }

    func_15123A54(arg0);
    func_1512E140(arg0);
    func_1512A390(arg0);
}
