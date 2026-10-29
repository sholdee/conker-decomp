#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A3408;
extern f32 D_800A340C;
extern f32 D_800A3410;
extern f32 D_800A3414;
extern f32 D_800A3418;

typedef struct {
    u8 pad0[0x20];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 unk4C;
    u8 unk4D;
    s8 unk4E;
    u8 pad4F;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    u8 pad5C[0x8];
    s32 unk64;
} CamSub740;

void func_151C9BA0(struct108 *arg0, s32 arg1);
void func_151216F8(struct108 *arg0);

#define FLD(type, base, off) (*(type *)((u8 *)(base) + (off)))

void func_1511FC60(struct108 *arg0) {
    CamSub740 *s;
    s32 state;

    s = (CamSub740 *)((u8 *)arg0 + 0x740);
    state = arg0->unk23E;
    FLD(s16, arg0, 0x84A) = 0x3C;
    if ((arg0->unk5F0 & 2) || (arg0->unk23E == 9) || (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) || (arg0->unk23E == 0x3B)) {
        *(volatile u8 *)&s->unk4D = 0;
        s->unk4D = 1;
    } else {
        s->unk4D = 1;
    }
    FLD(u8, arg0, 0x84C) = 0;
    if (FLD(s32, arg0, 0x34) == 0x40) {
        s->unk34 = arg0->unk37C;
    } else if (s->unk4D) {
        if ((arg0->unk23E == 9) || (arg0->unk23E == 0x38) || (arg0->unk23E == 0x39) || (arg0->unk23E == 0x37) ||
            (arg0->unk23E == 0x3B) || (arg0->unk23E == 0x15) || (arg0->unk23E == 0x26) || (arg0->unk23E == 0x3A)) {
            s->unk34 = (f32)((arg0->unk3D0->unk7A - FLD(s16, arg0->unk3D4, 0x12)) + 0xC000) * 0.005493164f;
        } else if ((D_800D1940 == 0x42) && (state == 0x1A)) {
            s->unk34 = arg0->unk3D0->unk40;
        } else {
            s->unk34 = arg0->unk3D0->unk40 - 180.0f;
        }
    } else {
        s->unk34 = arg0->unk37C;
        arg0->unk84 &= ~0x280;
    }
    if ((state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) || (state == 0x3B) ||
        (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
        func_151C9BA0(arg0, state);
        FLD(u8, arg0, 0x84C) = 1;
    } else if ((state == 2) || (state == 0x13)) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    } else if (state == 0x1C) {
    } else if (state == 0x34) {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    } else if (state == 0xF) {
        s->unk38 = 27.0f;
        s->unk3C = 27.0f;
        s->unk34 = arg0->unk3D0->unk40 + 225.0f;
    } else {
        s->unk38 = 0.0f;
        s->unk3C = 0.0f;
    }
    func_151216F8(arg0);
    func_15048758(&s->unk34);
    if (s->unk4C) {
        s->unk38 = s->unk50;
        s->unk3C = s->unk50;
        s->unk44 = s->unk58;
        s->unk34 += s->unk54;
    }
    s->unk20 = arg0->unk37C;
    s->unk2C = arg0->unk134;
    if (FLD(u8, arg0, 0x92C)) {
        s->unk24 = sqrtf((arg0->unk2BC - arg0->unk2F8) * (arg0->unk2BC - arg0->unk2F8) + (arg0->unk2C4 - arg0->unk300) * (arg0->unk2C4 - arg0->unk300));
        s->unk28 = FLD(f32, arg0, 0x344);
        s->unk30 = arg0->unk354;
    } else {
        s->unk24 = arg0->unk374;
        s->unk28 = arg0->unk348;
        s->unk30 = arg0->unk354;
    }
    arg0->unk35C = arg0->unk2C0;
    arg0->unk354 = arg0->unk2C0;
    s->unk44 = 110.0f;
    arg0->unk348 = 30.0f;
    FLD(f32, arg0->unk3D4, 0x16C) = s->unk34;
    if ((state == 0) || (state == 0xC) || (state == 9) || (state == 0x38) || (state == 0x39) || (state == 0x37) ||
        (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
        arg0->unk348 += 30.0f;
    }
    s->unk64 = 0;
    arg0->unk73C = 1;
    if (arg0->unk23C) {
        s->unk34 = arg0->unk3D0->unk40 - 180.0f;
        arg0->unk37C = s->unk34;
        arg0->unk39C = arg0->unk37C * 0.01745329238f;
        arg0->unk194 = arg0->unk198;
        arg0->unk18C = arg0->unk190;
    }
    if (s->unk44 < FLD(f32, arg0, 0x370)) {
        s->unk48 = s->unk44 * 1.200000048f;
        s->unk4E = -1;
    } else {
        s->unk48 = s->unk44 * 0.6666659713f;
        s->unk4E = 1;
    }
    if ((state == 0xA) || (state == 2) || (state == 0x12) || (state == 3) || (state == 0x34)) {
        s->unk48 = arg0->unk374;
        s->unk4E = -1;
    }
    arg0->unk84 &= ~0x280;
    arg0->unk374 = s->unk44;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_14D110/func_15120158.s")

void func_15121490(struct127 *arg0, u16 *arg1) {
    struct126 *stats;
    s32 state;
    vertex endPoint;
    vertex origin;
    f32 angle;
    vertex dir;
    f32 temp;

    arg0->unk31C->unk198 = 0;
    if (*arg1 & 0x10) {
        state = arg0->unk31C->unk78;
        if ((state == 0x9) || (state == 0x38) || (state == 0x39) || (state == 0x37) ||
            (state == 0x3B) || (state == 0x15) || (state == 0x26) || (state == 0x3A)) {
            stats = arg0->unk31C;
            angle = (arg0->unk40 - 90.0f) * D_800A3408;
            endPoint.x = arg0->x_position;
            endPoint.y = arg0->y_position + ((f32)(s16)stats->unk114 * 0.75f);
            endPoint.z = arg0->z_position;
            origin = endPoint;
            endPoint.x = (sinf(angle) * 100.0f) + origin.x;
            endPoint.y = origin.y;
            endPoint.z = (cosf(angle) * 100.0f) + origin.z;
            origin.y += arg0->y_position - arg0->old_y_position;
            *(vertex *)((u8 *)stats + 0x13C) = origin;
            *(vertex *)((u8 *)stats + 0x148) = endPoint;
            stats->unk198 = 1;
            dir.x = 1.0f;
            temp = dir.x * cosf(*(f32 *)((u8 *)stats + 0x170) * D_800A340C);
            dir.y = dir.x * sinf(*(f32 *)((u8 *)stats + 0x170) * D_800A3410);
            dir.x = temp;
            temp = dir.x * cosf(*(f32 *)((u8 *)stats + 0x16C) * D_800A3414);
            dir.z = dir.x * sinf(*(f32 *)((u8 *)stats + 0x16C) * D_800A3418);
            dir.x = temp;
            *(vertex *)((u8 *)stats + 0x130) = dir;
        }
    }
}

void func_151216F8(struct108 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->unk23E;
    if (temp_v0) {
        if (temp_v0 == 0x34) {
            arg0->unk190 = 80.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 350.0f;
            return;
        }
        if ((temp_v0 == 0x3B) && ((D_800BE9F0 == 0x41) || (D_800BE9F0 == 0x3C))) {
            arg0->unk190 = -12.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = -28.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 106.0f;
            return;
        }
        if ((temp_v0 == 2) || (temp_v0 == 0xA) || (temp_v0 == 0x13)) {
            arg0->unk190 = 10.0f;
            arg0->unk84 |= 0x80;
            *(f32 *)((u8 *)arg0 + 0x784) = 135.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if ((temp_v0 == 0x12) && (D_800BE9F0 == 0x3C)) {
            arg0->unk190 = 10.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 180.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0xF) {
            arg0->unk190 = 10.0f;
            *(f32 *)((u8 *)arg0 + 0x784) = 480.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0x38) {
            *(f32 *)((u8 *)arg0 + 0x784) = 110.0f;
            arg0->unk190 = 5.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        if (temp_v0 == 0x15) {
            *(f32 *)((u8 *)arg0 + 0x784) = 110.0f;
            arg0->unk190 = 25.0f;
            *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
            return;
        }
        arg0->unk190 = 5.0f;
        *(f32 *)((u8 *)arg0 + 0x784) = 75.0f;
        *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
        return;
    }
    *(f32 *)((u8 *)arg0 + 0x784) = 90.0f;
    arg0->unk190 = 25.0f;
    *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
}

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    u8 pad30[0x1C];
    u8 unk4C;
    u8 unk4D;
} Sub740; /* sub-block at actor+0x740 (func_1511FC60 uses +0x34/+0x4D too) */

void func_151C9ED4(struct108 *);

void func_151218C4(struct108 *arg0) {
    Sub740 *s = (Sub740 *)((u8 *)arg0 + 0x740);

    if (*arg0->unk36C & 0x10) {
        return;
    }
    if (*(s32 *)((u8 *)arg0 + 0x34) == 0x40) {
        if (arg0->unk6FC == 0xF) {
            *(f32 *)((u8 *)arg0 + 0x898) = 0.0f;
            *(f32 *)((u8 *)arg0 + 0x89C) = 0.0f;
        }
        arg0->unk3D4->unk198 = 0;
        arg0->unk73C = 0;
        *(s16 *)((u8 *)arg0 + 0x84A) = 0x3C;
    } else {
        *(s16 *)((u8 *)arg0 + 0x84A) = 0x3C;
        arg0->unk73C = 3;
    }
    arg0->unk374 = 200.0f;
    arg0->unk348 = 100.0f;
    arg0->unk134 = s->unk2C;
    s->unk4C = 0;
    arg0->unk190 = 0.0f;
    *(f32 *)((u8 *)arg0 + 0x99C) = 0.0f;
    arg0->unk198 = 0.0f;
    func_1506160C(arg0->unk3D0, 2, 0xFF, 0x20, arg0->unk23D);
    if (*((u8 *)arg0 + 0x84C) != 0) {
        func_151C9ED4(arg0);
        *((u8 *)arg0 + 0x84C) = 0;
    }
}
