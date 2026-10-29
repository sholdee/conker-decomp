#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern f32 D_800A34B0[4][2];
extern f32 D_800A34D0;
extern f32 D_800A34D4;
extern f32 D_800A34D8;
extern f32 D_800A34DC;
extern f32 D_800A34E0;
extern f32 D_800A34E4;
extern f32 D_800A34E8;
extern f32 D_800A34EC;
extern f32 D_800A34F0;
extern f32 D_800A34F4;
extern f32 D_800A34F8;
extern f32 D_800A34FC;
extern f32 D_800A3500;
extern f32 D_800A3504;
extern f32 D_800A3508;
extern f32 D_800A350C;
extern f32 D_800A3510;
extern f32 D_800A3514;
extern f32 D_800A3518;
extern f32 D_800A351C;
extern f32 D_800A3520;
extern f32 D_800A3524;
extern f32 D_800A3528;
extern f32 D_800A352C;
extern f32 D_800A3530;
extern f32 D_800A3534;
extern f32 D_800A3538;
extern f32 D_800A353C;
extern f32 D_800A3540;
extern f32 D_800A3544;
extern f32 D_800A3548;
extern f32 D_800A354C;
extern f32 D_800A3550;
extern f32 D_800A3554;
extern f32 D_800A3558;
extern f32 D_800A355C;
extern f32 D_800A3560;
extern f32 D_800A3564;
extern f32 D_800A3568;
extern f32 D_800A356C;
extern f32 D_800A3570;
extern f32 D_800A3574;
extern f32 D_800A3578;
extern f32 D_800A357C;
extern f32 D_800A3580;
extern f32 D_800A3584;
extern f32 D_800A3588;
extern f32 D_800A358C;
extern f32 D_800A3590;
extern f32 D_800A3594;
extern f32 D_800A3598;
extern f32 D_800A359C;
extern f32 D_800A35A0;
extern f32 D_800A35A4;
extern f32 D_800A35A8;
extern f32 D_800A35AC;

void func_15120158(struct108 *arg0);
void func_15121C80(struct108 *arg0, f32 arg1);
void func_151220D0(struct108 *arg0);
void func_15122170(struct108 *arg0);
void func_15122440(struct108 *arg0);
void func_15122980(struct108 *arg0);
void func_151219D0(struct108 *arg0);
void func_151236D0(struct108 *arg0);
void func_151256BC(struct108 *arg0);
s32 func_15126378(struct108 *arg0);
s32 func_15128030(struct108 *arg0);
void func_15129934(struct108 *arg0);
void func_1512A360(u8 *arg0);
void func_1512E4B0(struct108 *arg0);
void func_1512DEA4(struct108 *arg0);
void func_15143134(void *arg0, void *arg1, s32 arg2);
void func_1508EF80(struct17 *arg0, struct17 *arg1, f32 arg2, struct17 *arg3);
void func_151CC290(s32 arg0);
void func_1515BA10(s32 arg0);
void func_1515BA1C(s16 arg0);
void func_1515BA48(s32 arg0);
void func_1515BA54(s16 arg0);
void func_1515BA80(s16 arg0);
void func_1515BAAC(s16 arg0);


#if 0
f32 D_800A34B0[4][2] = {
    { 267.0f, 100.0f },
    { 247.0f, 100.0f },
    { 370.0f, 185.0f },
    { 530.0f, 400.0f }
};

f32 D_800A34D0 = 0.01666666753590107f;
f32 D_800A34D4 = 0.10000000149011612f;
f32 D_800A34D8 = 999.0f;
f32 D_800A34DC = 0.01745329238474369f;
f32 D_800A34E0 = 0.01745329238474369f;
f32 D_800A34E4 = 0.01745329238474369f;
f32 D_800A34E8 = 0.01745329238474369f;
f32 D_800A34EC = 530.0f;
f32 D_800A34F0 = 3.1415927410125732f;
f32 D_800A34F4 = 3.1415927410125732f;
f32 D_800A34F8 = 0.3253220021724701f;
f32 D_800A34FC = 1.5707963705062866f;
f32 D_800A3500 = 1.5707963705062866f;
f32 D_800A3504 = 0.6499999761581421f;
f32 D_800A3508 = 0.550000011920929f;
f32 D_800A350C = 0.8999999761581421f;
f32 D_800A3510 = 0.8500000238418579f;
f32 D_800A3514 = -10000.0f;
f32 D_800A3518 = 0.0033333334140479565f;
f32 D_800A351C = 57.2957763671875f;
f32 D_800A3520 = 0.01745329238474369f;
f32 D_800A3524 = 0.01745329238474369f;
f32 D_800A3528 = 0.01745329238474369f;
f32 D_800A352C = 0.01745329238474369f;
f32 D_800A3530 = 0.01745329238474369f;
f32 D_800A3534 = 6.2831854820251465f;
f32 D_800A3538 = 0.01745329238474369f;
f32 D_800A353C = 0.03999999910593033f;
f32 D_800A3540 = 0.05000000074505806f;
f32 D_800A3544 = 0.07999999821186066f;
f32 D_800A3548 = 0.03999999910593033f;
f32 D_800A354C = 0.05000000074505806f;
f32 D_800A3550 = 0.01745329238474369f;
f32 D_800A3554 = 0.01745329238474369f;
f32 D_800A3558 = 0.019999999552965164f;
f32 D_800A355C = 0.01745329238474369f;
f32 D_800A3560 = 0.6000000238418579f;
f32 D_800A3564 = 10000.0f;
f32 D_800A3568 = 10000.0f;
f32 D_800A356C = -0.01745329238474369f;
f32 D_800A3570 = -0.01745329238474369f;
f32 D_800A3574 = -0.01745329238474369f;
f32 D_800A3578 = -0.01745329238474369f;
f32 D_800A357C = -0.9399999976158142f;
f32 D_800A3580 = -10000.0f;
f32 D_800A3584 = 0.01666666753590107f;
f32 D_800A3588 = 0.01745329238474369f;
f32 D_800A358C = 0.02500000037252903f;
f32 D_800A3590 = -0.01745329238474369f;
f32 D_800A3594 = -0.01745329238474369f;
f32 D_800A3598 = -0.01745329238474369f;
f32 D_800A359C = -0.01745329238474369f;
f32 D_800A35A0 = 999.0f;
f32 D_800A35A4 = 0.01745329238474369f;
f32 D_800A35A8 = 999.0f;
f32 D_800A35AC = 999.0f;
#endif

void func_15122AE0(void) {
    struct108 *temp_s0;
    s16 i;
    f32 temp_f20;

    temp_f20 = D_800BEA08 * D_800A34D0;

    for (i = 0; i <= D_80082FA0; i++) {
        temp_s0 = &D_800DBFF0[i]; // ???
        if (func_150859AC(i, 0) || (i == 0)) {
            temp_s0->unk7B4 = temp_f20;
            if ((D_800BEAC0 == 0) || (D_800C35EA != 0) || (D_800D2DB4 != 0)) {
                func_151239CC(temp_s0, 5);
                func_15122C5C(temp_s0);
            } else {
                func_15123934(temp_s0, 8192, 0, temp_s0->unk134, 5);
                func_1512C490(temp_s0);
            }
        }
    }
    D_800894B0 += 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15122C5C.s")
// NON-MATCHING: 80% there
// void func_15122C5C(struct108 *arg0) {
//     f32 blend;
//     u8 temp_v0_2;
//     struct167 *temp_a1;
//     struct167 *temp_a0;
//
//     if (arg0->unk23C) {
//         arg0->unk2E0 = arg0->unk2BC;
//         arg0->unk2E4 = arg0->unk2C0;
//         arg0->unk2F0 = arg0->unk2FC;
//
//         arg0->unk2EC = arg0->unk2F8;
//         arg0->unk2E8 = arg0->unk2C4;
//         arg0->unk2F4 = arg0->unk300;
//     }
//
//     temp_a0 = &arg0->unk2BC;
//     temp_a1 = &arg0->unk2F8;
//
//     if ((arg0->unk3D4 != NULL) && (arg0->unk73C != 3) && ((arg0->unk2C != 0x100) || (arg0->unk73C != 0))) {
//         arg0->unk23E = arg0->unk3D4->unk78;
//     }
//
//     temp_v0_2 = arg0->unk23D;
//
//     if (D_800DBFF4[temp_v0_2] != 4) {
//         arg0->unk5F0 &= -5;
//     }
//
//     if ((arg0->unk2C << 9) >= 0) {
//         temp_a0->unk0 = arg0->unk2E0;
//         temp_a0->unk4 = arg0->unk2E4;
//         temp_a0->unk8 = arg0->unk2E8;
//         temp_a1->unk0 = arg0->unk2EC;
//         temp_a1->unk4 = arg0->unk2F0;
//         temp_a1->unk8 = arg0->unk2F4;
//     }
//     // temp_v0_2 = arg0->unk23D;
//     arg0->unk36C = D_800BE728[temp_v0_2];
//     arg0->unk36A = D_800BE710[temp_v0_2];
//     arg0->unk304 = temp_a1->unk0;
//     arg0->unk308 = temp_a1->unk4;
//     arg0->unk358 = arg0->unk35C;
//     arg0->unk30C = temp_a1->unk8;
//     arg0->unk2C8 = temp_a0->unk0;
//     arg0->unk2CC = temp_a0->unk4;
//     arg0->unk2D0 = temp_a0->unk8;
//     if (arg0->unk23C != 0) {
//         arg0->unk19C = arg0->unk1A4;
//         arg0->unk1A0 = arg0->unk1A8;
//         // blend = *(s32*)&arg0; // unk0?
//     } else {
//         arg0->unk19C += (arg0->unk1A4 - arg0->unk19C) * D_800A34D4;
//         arg0->unk1A0 += (arg0->unk1A8 - arg0->unk1A0) * D_800A34D4;
//         blend = D_800A34D4;
//     }
//
//     func_1510B128(blend, temp_v0_2, arg0->unk19C, arg0->unk1A0, D_800BE628[temp_v0_2].unk84, (arg0->unk2C & 0x100) ? 16.0f : 0.0f);
//     func_15097798(arg0->unk23D);
//     func_15125A6C(arg0);
//     func_15128CB0(arg0);
//     func_15125DB4(arg0);
//     if ((arg0->unk3D4->unk120 != 0) && (arg0->unk3D4->unk197 != 0)) {
//         func_15127EB8(arg0);
//     }
//     if (func_15128540(arg0) == 0) {
//         if (((arg0->unk84 * 0x10) < 0) && ((arg0->unk84 & 0x400) == 0)) {
//             func_1512E8E0(arg0);
//         }
//         func_15130230(arg0, D_800BE9F0);
//         func_15126138(arg0);
//         func_15125C40(arg0);
//         func_15128680(arg0);
//         func_1512317C(arg0);
//         func_15125394(arg0);
//         if (((arg0->unk84 * 16) >= 0) && ((arg0->unk84 & 0x400) == 0)) {
//             func_1512E8E0(arg0);
//         }
//         func_15123568(arg0);
//         func_1512C200(arg0);
//         func_15125924(arg0);
//         func_151283B8(arg0);
//         if (arg0->unk23C != 0) {
//             func_15125594(arg0);
//             func_15123070(arg0);
//             func_151283B8(arg0);
//             func_151283B8(arg0);
//         }
//         func_15123508(arg0);
//         if (((arg0->unk84 & 2) != 0) && (arg0->unk3D4->unk120 == 0)) {
//             func_1512D390(arg0);
//         }
//         func_15125594(arg0);
//         func_151256BC(arg0);
//         func_1512D980(arg0);
//         func_1512C068(arg0);
//         func_1512B100(arg0);
//         func_1512C20C(arg0);
//         func_1512BB10(arg0);
//         if (arg0->unk23C != 0) {
//             func_1512C068(arg0);
//             func_1512B100(arg0);
//             func_1512C20C(arg0);
//             func_1512BB10(arg0);
//         }
//         func_15125594(arg0);
//         func_15123070(arg0);
//         if (arg0->unk7F4 == 0) {
//             func_151287E0(arg0, 0, 0);
//         }
//         func_1512C150(arg0);
//         func_151284C4(arg0);
//     }
// }

void func_15123070(struct108 *arg0) {
    f32 temp_f0;

    if ((arg0->unk6C8 != 0) && ((arg0->unk6FC == 10) || (arg0->unk6FC == 14))) {
        struct17 tmp;

        func_15048F90((struct17 *)&arg0->unk618, (struct17 *)&arg0->unk2A4, &tmp);
        arg0->unk390 = arg0->unk37C - func_15048FC8(&tmp);
    } else {
        temp_f0 = arg0->unk3D0->unk40 - arg0->unk37C - 180.0f;

        while (temp_f0 < 0.0f) {
            temp_f0 += 360.0f;
        }

        arg0->unk390 = temp_f0;
    }

    while (arg0->unk390 < -360.0f) {
        arg0->unk390 += 360.0f;
    }
}

void func_1511FC60(struct108 *arg0);
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
s32 func_15125490(struct108 *arg0);

void func_1512317C(struct108 *arg0) {
    s32 cond1;
    s32 bit100;
    s32 bit200;
    s32 flag;

    bit100 = arg0->unk5F0 & 0x100;
    bit200 = arg0->unk5F0 & 0x200;
    cond1 = (arg0->unk3D0->stunned != 0) && (arg0->unk23E != 9) && (arg0->unk23E != 0x38) &&
            (arg0->unk23E != 0x39) && (arg0->unk23E != 0x3B) && (arg0->unk23E != 0x37) &&
            (arg0->unk23E != 0x15) && (arg0->unk23E != 0x26) && (arg0->unk23E != 0x3A);

    flag = ((arg0->unk2C & 0x80) != 0) ||
           ((arg0->unk84 & 0x1000000) != 0) ||
           ((arg0->unk3D0->unk28 != 0.0f) && (arg0->unk3D0->in_water == 0) &&
            (arg0->unk3D4->unk9C == 0) && (bit100 == 0) && (bit200 == 0) &&
            (arg0->unk23E != 3)) ||
           ((arg0->unk3D0->unk65 != 0) && (bit100 == 0) && (bit200 == 0)) ||
           (cond1 != 0) ||
           (func_15125490(arg0) != 0) ||
           (arg0->unk3D0->health == 0) ||
           (((*(u8 *)((u8 *)arg0->unk3D4 + 0x4E) & 0xF) == 1) &&
            (arg0->unk3D0->xz_velocity > 15.0f)) ||
           (*(u8 *)((u8 *)arg0->unk3D4 + 0x1AC) != 0);

    if (((*arg0->unk36C & 0x10) != 0) && (flag == 0)) {
        if ((arg0->unk36A & 0x10) == 0) {
            return;
        }
        if (func_15123934(arg0, 0x100, 4, arg0->unk134, 1) != 0) {
            func_1511FC60(arg0);
        }
        return;
    }

    if ((arg0->unk2C == 0x100) && (arg0->unk73C == 0)) {
        func_151239CC(arg0, 1);
        arg0->unk3D4->unk198 = 0;
        arg0->unk3D4->unk197 = 0;
        *(s16 *)((u8 *)arg0 + 0x5F8) = 1;
        arg0->unk5FC = 2;
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((arg0->unk2C == 0x100) || (arg0->unk73C != 0)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }
    arg0->unk3D4->unk198 = arg0->unk73C;

    if ((arg0->unk73C != 0) || (arg0->unk73C == 3)) {
        func_150495B0((f32 *)((u8 *)arg0 + 0x74C), 0.0f, (f32 *)((u8 *)arg0 + 0x79C), 4.0f, 9.0f,
                      arg0->unk7B4);
        func_150495B0((f32 *)((u8 *)arg0 + 0x754), 0.0f, (f32 *)((u8 *)arg0 + 0x7A0), 4.0f, 9.0f,
                      arg0->unk7B4);
    }

    if ((arg0->unk2C != 0x100) && (arg0->unk73C != 0)) {
        arg0->unk73C = 0;
    }
}

void func_15123508(struct108 *arg0) {
    if ((arg0->unk84 & 2) != 0) {
        if (((arg0->unk36A & 2) != 0) && (arg0->unk698 == 0)) {
            arg0->unk6B0 = -1;
        }
        if (((arg0->unk36A & 1) != 0) && (arg0->unk698 == 0)) {
            arg0->unk6B0 = 1;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15123568.s")
void func_151236D0(struct108 *arg0) {
    s32 oldState;

    oldState = arg0->unk5F0 & 1;
    if (arg0->unk3D4 != NULL) {
        if ((arg0->unk3D4->unk9C != 0) ||
            ((*((u8 *)arg0->unk3D4 + 0x95) != 0) && (*((u8 *)arg0->unk3D0 + 0x137) == 0))) {
            arg0->unk5F0 |= 0x40;
        } else {
            arg0->unk5F0 &= ~0x40;
        }
        if (arg0->unk3D0->unk28 == 0.0f) {
            arg0->unk5F0 &= ~0x400;
        }
        if ((*(s16 *)((u8 *)arg0->unk3D4 + 0x8) != 0) && (*((u8 *)arg0->unk3D4 + 0x16) == 0)) {
            arg0->unk5F0 |= 8;
        } else {
            arg0->unk5F0 &= ~8;
        }
        if (*((u8 *)arg0->unk3D4 + 0x4E) == 2) {
            arg0->unk5F0 |= 0x80;
        } else {
            arg0->unk5F0 &= ~0x80;
        }
    }
    if ((arg0->unk2FC <= *(f32 *)((u8 *)arg0 + 0x360)) && (arg0->unk2C != 0x100)) {
        arg0->unk5F0 |= 1;
    } else {
        arg0->unk5F0 &= ~1;
    }
    if (arg0->unk23C != 0) {
        if ((arg0->unk5F0 & 1) != 0) {
            func_1515BA80(arg0->unk23D);
        } else {
            func_1515BA48(arg0->unk23D);
            *(f32 *)((u8 *)arg0 + 0x7B0) = 0.0f;
        }
    } else if ((oldState == 0) && ((arg0->unk5F0 & 1) != 0)) {
        if (((arg0->unk5F0 & 4) != 0) || ((arg0->unk2C & 0x40000) != 0)) {
            func_1515BA80(arg0->unk23D);
        } else {
            func_1515BAAC(arg0->unk23D);
        }
    } else if ((oldState != 0) && ((arg0->unk5F0 & 1) == 0)) {
        if (((arg0->unk5F0 & 4) != 0) || ((arg0->unk2C & 0x40000) != 0)) {
            func_1515BA48(arg0->unk23D);
            *(f32 *)((u8 *)arg0 + 0x7B0) = 0.0f;
        } else {
            func_1515BA54(arg0->unk23D);
            func_15124B18(arg0);
        }
    } else if ((arg0->unk5F0 & 1) != 0) {
        func_1515BA1C(arg0->unk23D);
    } else {
        func_1515BA10(arg0->unk23D);
    }
}
s32 func_15123934(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 v0;

    v0 = 0;
    if (((s16 *)((u8 *)arg0 + arg4 * 2))[0x106] == 0) {
        *(u16 *)((u8 *)arg0 + arg4 * 2 + 0x2) = arg0->unk0;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x30) = arg0->unk2C;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x88) = arg0->unk84;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0xE0) = arg0->unkDC;
        *(s32 *)((u8 *)arg0 + arg4 * 4 + 0x138) = arg0->unk134;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x1B6) = arg0->unk1B4;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x1E2) = arg0->unk1E0;
        arg0->unk2C = arg1;
        arg0->unkDC = arg2;
        arg0->unk134 = arg3;
        *(s16 *)((u8 *)arg0 + arg4 * 2 + 0x20C) = 1;
        func_15125394(arg0);
        return 1;
    }
    return v0;
}

#if 0
// NON-MATCHING: not sure what is up with arg0
// s32 func_15123934(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     struct108 *temp_v0;
//     struct108 *temp_v1;
//
//     temp_v1 = &arg0[arg4]; // ???
//     if (temp_v1->unk20C == 0) {
//         temp_v1->unk2 = arg0->unk0;
//         temp_v0 = &arg0[arg4 * 4]; // ???
//         temp_v0->unk30 = arg0->unk2C;
//         temp_v0->unk88 = arg0->unk84;
//         temp_v0->unkE0 = arg0->unkDC;
//         temp_v0->unk138 = arg0->unk134;
//         temp_v1->unk1B6[0] = arg0->unk1B4;
//         temp_v1->unk1E2 = arg0->unk1E0;
//         arg0->unk2C = arg1;
//         arg0->unkDC = arg2;
//         arg0->unk134 = arg3;
//         temp_v1->unk20C = 1;
//         func_15125394();
//         return 1;
//     }
//     return 0;
// }
#endif

s32 func_151239CC(struct108 *arg0, s32 arg1) {
    if (arg0->unk20C[arg1] != 0) {
        arg0->unk0 = arg0->unk2[arg1];
        arg0->unk2C  = *(s32*)((u8*)arg0 + arg1 * 4 + 0x30);
        arg0->unkDC  = *(s32*)((u8*)arg0 + arg1 * 4 + 0xE0);
        arg0->unk84  = *(s32*)((u8*)arg0 + arg1 * 4 + 0x88);
        arg0->unk134 = *(s32*)((u8*)arg0 + arg1 * 4 + 0x138);
        arg0->unk1B4 = ((s16*)arg0)[arg1 + 0xDB];
        arg0->unk1E0 = ((s16*)arg0)[arg1 + 0xF1];
        func_15124B18(arg0);
        arg0->unk20C[arg1] = 0;
        return 1;
    }
    return 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15123A54.s")

void func_15124770(struct108 *arg0, s32 arg1) {
    if (arg1 != 0) {
        arg0->unk244 = arg1;
        arg0->unk248 = &D_800CC2D0[arg1];
    } else {
        arg0->unk244 = 0;
        arg0->unk248 = NULL;
    }
}

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

#define F(p, off) (*(f32 *)((u8 *)(p) + (off)))

void func_151247C0(struct108 *arg0) {
    f32 padA;
    f32 padB;
    f32 padC;
    f32 sp28;
    struct127 *obj;
    struct127 *other;
    u8 flag;
    f32 t;
    f32 t2;

    if (arg0->unk7F4 == 0) {
        obj = arg0->unk3D4;
        flag = (obj != NULL) ? (*((u8 *)obj + 0x95) | *(s32 *)((u8 *)obj + 0x9C)) : 0;
        if (flag) {
            arg0->unk674 = 0.25f;
        } else {
            arg0->unk674 = 1.0f;
        }
        if (arg0->unk23C) {
            arg0->unk670 = arg0->unk674;
            F(arg0, 0x678) = 0;
        } else {
            func_150495B0(&arg0->unk670, arg0->unk674, &F(arg0, 0x678), F(arg0, 0x3B4) * 0.25f, F(arg0, 0x3B8) * 0.25f, arg0->unk7B4);
        }
        if (arg0->unk244) {
            if (arg0->unk23C) {
                obj = arg0->unk3D0;
                other = (struct127 *)arg0->unk248;
                arg0->unk2A4 = (obj->x_position + other->x_position) * 0.5f;
                arg0->unk2A8 = (obj->y_position + other->y_position) * 0.5f;
                F(arg0, 0x2AC) = (obj->z_position + other->z_position) * 0.5f;
            } else {
                obj = arg0->unk3D0;
                other = (struct127 *)arg0->unk248;
                arg0->unk2A4 += (((obj->x_position + other->x_position) * 0.5f) - arg0->unk2A4) * arg0->unk670;
                arg0->unk2A8 = (obj->y_position + other->y_position) * 0.5f;
                F(arg0, 0x2AC) += (((obj->z_position + other->z_position) * 0.5f) - F(arg0, 0x2AC)) * arg0->unk670;
            }
        } else {
            if (arg0->unk23C) {
                obj = arg0->unk3D0;
                arg0->unk2A4 = obj->x_position;
                F(arg0, 0x2AC) = obj->z_position;
            } else {
                obj = arg0->unk3D0;
                arg0->unk2A4 += (obj->x_position - arg0->unk2A4) * arg0->unk670;
                F(arg0, 0x2AC) += (obj->z_position - F(arg0, 0x2AC)) * arg0->unk670;
            }
            arg0->unk2A8 = obj->y_position;
        }
        if (F(arg0, 0x994) != 0.0f || F(arg0, 0x99C) != 0.0f) {
            if (arg0->unk23C) {
                F(arg0, 0x994) = F(arg0, 0x99C);
                F(arg0, 0x998) = 0;
            } else {
                func_150495B0(&F(arg0, 0x994), F(arg0, 0x99C), &F(arg0, 0x998), 4.0f, 6.0f, arg0->unk7B4);
            }
            sp28 = F(arg0, 0x994) * sinf(arg0->unk39C - D_800A34FC);
            t = F(arg0, 0x994) * cosf(arg0->unk39C - D_800A3500);
            arg0->unk2A4 += sp28;
            F(arg0, 0x2AC) += t;
        }
        arg0->unk2BC = arg0->unk2A4;
        arg0->unk2C4 = F(arg0, 0x2AC);
        func_15125594(arg0);
    }
}

void func_15124AB4(struct108 *arg0) {
    arg0->unk668 = -sinf(arg0->unk398);
    arg0->unk66C = cosf(arg0->unk398);
    arg0->unk664 = sinf(arg0->unk39C) * arg0->unk66C;
    arg0->unk66C = cosf(arg0->unk39C) * arg0->unk66C;
}

void func_15124B18(struct108 *arg0) {
    s32 temp_t8;
    f32 *temp_v0;

    if (arg0->unk1B4 == 0) {
        arg0->unk1B4 = 4;
    }
    temp_t8 = arg0->unk2C & 0x80;
    if ((temp_t8 != 0) && ((*arg0->unk36C & 0x10) != 0)) {
        arg0->unk348 = 40.0f;
        arg0->unk34C = 40.0f;
        arg0->unk374 = 150.0f;
    } else if (temp_t8 != 0) {
        arg0->unk348 = (f32) (arg0->unk2FC - arg0->unk354);
    } else if ((temp_t8 != 0) && (arg0->unk3D0->unk102 == 0) && (D_800BE9F0 != 0x17)) {
        arg0->unk348 = 40.0f;
        arg0->unk34C = 40.0f;
        arg0->unk374 = 150.0f;
    } else if (arg0->unk3D0->unk102 != 0) {
        arg0->unk348 = 40.0f;
        arg0->unk34C = 40.0f;
        arg0->unk374 = 194.0f;
    } else {
        temp_v0 = D_800A34B0[arg0->unk1B4];
        arg0->unk374 = temp_v0[0]; //->unk0;
        arg0->unk348 = temp_v0[1]; //->unk4;
        arg0->unk34C = arg0->unk348;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15124C38.s")

void func_1512523C(struct108 *arg0) {
    f32 temp_f12;
    f32 temp_f2;
    f32 dist;

    temp_f2 = arg0->unk2BC - arg0->unk2F8;
    temp_f12 = arg0->unk2C4 - arg0->unk300;
    temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f12 * temp_f12));
    if (temp_f12 < 0.0f) {
        dist = -temp_f12;
    } else {
        dist = temp_f12;
    }
    temp_f2 = func_150484A0(arg0->unk2FC - arg0->unk2C0, dist) * D_800A351C;
    arg0->unk388 = temp_f2;
    if (temp_f2 > 180.0f) {
        arg0->unk388 -= 360.0f;
    }
    arg0->unk388 = -arg0->unk388;
    arg0->unk388 -= arg0->unk3A8;
    arg0->unk388 += arg0->unk38C;
    arg0->unk398 = arg0->unk388 * D_800A3520;
}

void func_15125330(struct108 *arg0) {
    struct17 tmp;

    tmp.unk0 = arg0->unk2BC - arg0->unk2F8;
    tmp.unk4 = 0.0f;
    tmp.unk8 = arg0->unk2C4 - arg0->unk300;

    arg0->unk37C = func_15048FC8(&tmp);
    arg0->unk39C = arg0->unk37C * D_800A3524;
}

void func_15125394(struct108 *arg0) {
    s32 i = 0;

    while ((arg0->unk2C & (1 << i)) == 0) {
        i += 1;
    }
    arg0->unk0 = i;
}

s32 func_151253CC(struct108 *arg0) {
    f32 temp_f0;
    f32 temp_f2;

    if (arg0->unk2C & 0x40000) {
        arg0->unk84 &= ~0x4F;
        arg0->unk84 |= 0x2680;
        arg0->unk3A8 = 0.0f;
        arg0->unk5E8 = 0.0f;
        arg0->unk38C = 0.0f;
        func_15125330(arg0);
        func_1512523C(arg0);
        temp_f0 = arg0->unk37C;
        temp_f2 = D_800A3528;
        arg0->unk380 = arg0->unk37C;
        arg0->unk3A0 = (f32) (temp_f0 * temp_f2);
        arg0->unk398 = (f32) (arg0->unk388 * temp_f2);
        func_15124AB4(arg0);
        if (arg0->unk3D4->unk197 != 0) {
            func_15127EB8(arg0);
        }
        func_1512C490(arg0);
        return 1;
    }
    return 0;
}

// no idea what going on here
s32 func_15125490(struct108 *arg0) {
    struct127 *temp = arg0->unk3D0;
    s32 v1;

    if (temp->in_water == 1) {
        v1 = (s32) fabsf(temp->y_position - temp->unk118);
        if (v1 < 0x64) {
            return 0;
        }
        if (v1 >= 0x12D) {
            return 1;
        }
    } else {
        return 0;
    }
}

// PERMUTER CANDIDATE (best 558): logic + operand/store order all match; only two IDO
// scheduling diffs remain (D_800A352C load placed after `sw ra`; a1-reload vs 0x670 store).
// void func_151254F4(struct108 *arg0, s32 arg1) {
//     f32 tmp = D_800A352C;
//     arg0->unk3A0 = arg0->unk380 * tmp;
//     arg0->unk398 = arg0->unk388 * tmp;
//     func_15124AB4(arg0);
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk198 = 0;
//     arg0->unk73C = 0;
//     arg0->unk670 = 0.0f;
//     arg0->unk3D0 = &D_800CC2D0[arg1];
// }
void func_151254F4(void *arg0, s32 arg1) {
    f32 scale;

    scale = 0.01745329238474369f;
    *(f32 *)((u8 *)arg0 + 0x3A0) = *(f32 *)((u8 *)arg0 + 0x380) * scale;
    *(f32 *)((u8 *)arg0 + 0x398) = *(f32 *)((u8 *)arg0 + 0x388) * scale;
    func_15124AB4(arg0);
    func_151239CC(arg0, 1);
    *((u8 *)*(void **)((u8 *)arg0 + 0x3D4) + 0x198) = 0;
    *(s16 *)((u8 *)arg0 + 0x73C) = 0;
    *(void **)((u8 *)arg0 + 0x3D0) = (void *)((u32)D_800CC2D0 + (u32)arg1 * 0x32CU);
    *(f32 *)((u8 *)arg0 + 0x670) = 0.0f;
}
// NON-MATCHING: first statements in wrong order
// void func_151254F4(struct108 *arg0, s32 arg1) {
//     f32 tmp = D_800A352C;
//     arg0->unk3A0 = tmp * arg0->unk380;
//     arg0->unk398 = tmp * arg0->unk388;
//     func_15124AB4(arg0);
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk198 = 0;
//     arg0->unk73C = 0;
//     arg0->unk3D0 = &D_800CC2D0[arg1];
//     arg0->unk670 = 0.0f;
// }

void func_15125594(struct108 *arg0) {
    f32 temp_f0;
    f32 temp_f2;

    func_1512523C(arg0);
    func_15125330(arg0);

    temp_f0 = arg0->unk37C;
    temp_f2 = arg0->unk5E8;
    arg0->unk380 = temp_f0 - temp_f2;
    if (temp_f0 < temp_f2) {
        arg0->unk380 += 360.0f;
    }
    arg0->unk3A0 = arg0->unk380 * D_800A3530;
    func_15124AB4(arg0);
}


void func_15125608(struct108 *arg0) {
    arg0->unk24C = 3.0f;
    arg0->unk250 = 2.5f;
}

// PERMUTER/CODEGEN CANDIDATE: logic correct, but IDO materializes each timer's address
// (lui+addiu, load/store via one reg) where target folds %lo separately per access.
// void func_15125628(void) {
//     if (D_800DBFF4[0] != 0) D_800DBFF4[0]--;
//     if (D_800DBFF5 != 0) D_800DBFF5--;
//     if (D_800DBFF6 != 0) D_800DBFF6--;
//     if ((&D_800DBFF6)[1] != 0) (&D_800DBFF6)[1]--;
// }
void func_15125628(void) {
    u8 v0;
    u8 *p4 = D_800DBFF4;
    u8 *p5 = &D_800DBFF5;
    u8 *p6 = &D_800DBFF6;
    u8 *p7 = &(&D_800DBFF6)[1];

    v0 = *p4;
    if (v0 != 0) {
        *p4 = v0 - 1;
    }
    v0 = *p5;
    if (v0 != 0) {
        *p5 = v0 - 1;
    }
    v0 = *p6;
    if (v0 != 0) {
        *p6 = v0 - 1;
    }
    v0 = *p7;
    if (v0 != 0) {
        *p7 = v0 - 1;
    }
}

void func_15125690(struct108 *arg0, s32 arg1) {
    u8 *temp_v0 = &D_800DBFF4[arg0->unk23D];

    if (*temp_v0 < arg1) {
        *temp_v0 = arg1;
    }
}

/* Camera "sway"/bob: +0x29C is the sway phase angle and +0x5EC the current sway
 * amount (structs.h mis-labels both offsets as padding). While swaying is armed
 * the phase advances by a random 0/2/4 degrees per frame and wraps at 2*PI;
 * otherwise the phase decays away. The resulting amount is fed to the camera
 * eye/target orbit routine. */
void func_151256BC(struct108 *arg0) {
    f32 twoPi;
    f32 speed;
    f32 scale;
    f32 mult;

    if (((arg0->unk2C & 0x80000) != 0) ||
        (((arg0->unk5F0 & 8) != 0) && ((arg0->unk2C & 0x40000) == 0))) {
        speed = (f32) ((u32) func_150ADA20() % 3);
        twoPi = D_800A3534;
        *(f32 *) ((u8 *) arg0 + 0x29C) += ((speed + speed) * D_800A3538) * D_800BE9A4;
        while (twoPi < *(f32 *) ((u8 *) arg0 + 0x29C)) {
            *(f32 *) ((u8 *) arg0 + 0x29C) -= twoPi;
        }
        *(f32 *) ((u8 *) arg0 + 0x5EC) += ((sinf(*(f32 *) ((u8 *) arg0 + 0x29C)) * 4.0f) -
                                           *(f32 *) ((u8 *) arg0 + 0x5EC)) * D_800A353C;
        speed = *(f32 *) ((u8 *) arg0 + 0x5EC) * D_800A3540;
        if ((arg0->unk2C & 0x80000) != 0) {
            scale = 4.0f;
            mult = 4.0f;
        } else {
            scale = 1.0f;
            mult = 20.0f;
        }
        func_1508EF80((struct17 *) &arg0->unk2F8, (struct17 *) &arg0->unk2BC, speed * scale,
                      (struct17 *) &arg0->unk2F8);
        func_1508EF80((struct17 *) &arg0->unk2BC, (struct17 *) &arg0->unk2F8, speed * mult,
                      (struct17 *) &arg0->unk2BC);
    } else if (*(f32 *) ((u8 *) arg0 + 0x29C) != 0.0f) {
        *(f32 *) ((u8 *) arg0 + 0x29C) -= *(f32 *) ((u8 *) arg0 + 0x29C) * D_800A3544;
        *(f32 *) ((u8 *) arg0 + 0x5EC) += ((sinf(*(f32 *) ((u8 *) arg0 + 0x29C)) * 4.0f) -
                                           *(f32 *) ((u8 *) arg0 + 0x5EC)) * D_800A3548;
        speed = *(f32 *) ((u8 *) arg0 + 0x5EC) * D_800A354C;
        func_1508EF80((struct17 *) &arg0->unk2F8, (struct17 *) &arg0->unk2BC, speed,
                      (struct17 *) &arg0->unk2F8);
        func_1508EF80((struct17 *) &arg0->unk2BC, (struct17 *) &arg0->unk2F8, speed * 20.0f,
                      (struct17 *) &arg0->unk2BC);
    }
}

void func_15125924(struct108 *arg0) {
    s32 temp_v1;
    s32 temp_a1;

    if (arg0->unk3D4 != NULL) {
        temp_a1 = (s32) arg0->unk3D4->z_position;
        temp_v1 = *(u8 *)((u8 *)arg0->unk3D4 + 0x4E);
    } else {
        temp_a1 = 0;
        temp_v1 = 0;
    }

    if ((arg0->unk2C & 0x40) == 0) {
        if (((arg0->unk84 & 0x4000) != 0) || (arg0->unk3D0->unk102 != 0)) {
            if (func_15125490(arg0) != 0) {
                if ((temp_v1 == 0) && (temp_a1 == 0)) {
                    if (func_15123934(arg0, 0x80, 1, 1, 0xD) != 0) {
                        if (arg0->unk6C8 == 0) {
                            func_15124B18(arg0);
                            arg0->unk5F0 |= 0x1000;
                        }
                    }
                    return;
                }
            }
            if ((arg0->unk2C & 0x80) != 0) {
                func_151239CC(arg0, 0xD);
                arg0->unk190 = 0.0f;
            }
            if ((arg0->unk5F0 & 0x1000) != 0) {
                arg0->unk5F0 &= ~0x1000;
                arg0->unk348 = (*(f32 *)((u8 *)arg0 + 0x344) = arg0->unk2FC - arg0->unk354);
            }
        }
    }
}
void func_1512D560(void *, s32, s32);
extern f32 D_800C3614;
extern f32 D_800C3618;
extern f32 D_800C361C;
extern f32 D_800C3624;
extern s8 D_800C365C;

void func_15125A6C(struct108 *arg0) {
    f32 offsetCos;
    f32 offsetSin;
    f32 radius;
    f32 yaw;
    f32 offsetRadius;
    f32 height;
    f32 trig;
    f32 angle;
    f32 offsetAngle;
    u8 mode;

    mode = arg0->unk23E;
    if (mode == 0x2A) {
        yaw = arg0->unk3D0->unk40;
        if (arg0->unk2C != 0x40000) {
            func_1512D560(arg0, 5, 0);
            arg0->unk7E4 = 1U;
        }
        *D_800DBFF4 = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        radius = 138.0f;
        offsetRadius = 46.0f;
        height = 96.0f;
        angle = yaw * D_800A3550;
        trig = sinf(angle);
        offsetAngle = (yaw - 90.0f) * D_800A3554;
        offsetSin = sinf(offsetAngle);
        D_800C3614 = (offsetSin * offsetRadius) + (arg0->unk3D0->x_position + (radius * trig));
        D_800C3618 = arg0->unk3D0->y_position + height;
        trig = cosf(angle);
        offsetCos = cosf(offsetAngle);
        D_800C361C = (offsetCos * offsetRadius) + (arg0->unk3D0->z_position + (radius * trig));
        ((struct168 *)&D_800C3600)->unk20 = 0.0f;
        D_800C3624 = -180.0f;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        ((struct168 *)&D_800C3600)->unk28 = yaw - 180.0f;
        D_800C365C = 0;
        func_1512D560(arg0, 7, (s32)&D_800C3600);
        return;
    }
    if (arg0->unk7E4 != 0) {
        func_1512D560(arg0, 6, 0);
        arg0->unk7E4 = 0U;
    }
}
// NON-MATCHING: miles away
// void func_15125A6C(struct108 *arg0) {
//     f32 pad3C;
//     f32 sp38;
//     f32 pad34;
//     f32 pad30;
//     f32 sp2C;
//     f32 sp28;
//     f32 sp24;
//
//     f32 temp_f12;
//     f32 temp_f12_2;
//
//     if (arg0->unk23E == 0x2A) {
//         sp38 = arg0->unk3D0->unk40;
//         if (arg0->unk2C != 0x40000) {
//             func_1512D560(arg0, 5, 0);
//             arg0->unk7E4 = 1U;
//         }
//         D_800DBFF4[0] = 3;
//         D_800C3600->unk0 = 1;
//         D_800C3600->unk4 = 0;
//         temp_f12 = sp38 * D_800A3550;
//         sp28 = temp_f12;
//         sp2C = sinf(temp_f12);
//         temp_f12_2 = (sp38 - 90.0f) * D_800A3554;
//         sp24 = temp_f12_2;
//         D_800C3600->unk14 = (sinf(temp_f12_2) * 46.0f) + (arg0->unk3D0->unk14 + (138.0f * sp2C));
//         D_800C3600->unk18 = arg0->unk3D0->unk18 + 96.0f;
//         sp2C = cosf(temp_f12);
//         D_800C3600->unk1C = (cosf(temp_f12_2) * 46.0f) + (arg0->unk3D0->unk1C + (138.0f * sp2C));
//         D_800C3600->unk20 = 0.0f;
//         D_800C3600->unk24 = -180.0f;
//         D_800C3600->unk8 = 0.0f;
//         D_800C3600->unk28 = sp38 - 180.0f;
//         D_800C3600->unk5C = 0;
//         func_1512D560(arg0, 7, D_800C3600);
//         return;
//     }
//
//     if (arg0->unk7E4 != 0) {
//         func_1512D560(arg0, 6, 0);
//         arg0->unk7E4 = 0U;
//     }
// }

void func_1509BFB0(s32 num, s32, s32, ...);
extern u8 D_800D1940;
void func_15125C40(struct108 *arg0)
{
  s32 temp_v0;
  s32 temp_a0;
  s32 temp_a1;
  s32 temp_a2;
  u16 *temp_v0_2;
  temp_v0 = D_800D1940 == 0x42;
  temp_a1 = arg0->unk23E;
  temp_a2 = 0x1A;
  if (temp_v0 != 0)
  {
    temp_v0 = temp_a1 == temp_a2;
  }
  temp_a0 = temp_v0;
  temp_v0 = temp_a1 == 3;
  if (temp_v0 == 0)
  {
    temp_a2 = 0x1A;
    temp_v0 = temp_a1 == temp_a2;
    if (temp_v0 == 0)
    {
      temp_v0 = temp_a0 != 0;
    }
  }
  arg0->unk7CC = arg0->unk7CC - 1;
  if (temp_v0 != 0)
  {
    if (arg0->unk7CC == 0)
    {
      if (temp_a0 != 0)
      {
        func_1509BFB0(3, 0x9000, 0x18, (s32) arg0->unk3D0->unk40, 0, 0xFA);
      }
      else
        if (temp_a1 == temp_a2)
      {
        func_1509BFB0(3, 0x9000, 0x18, 0, 0, 0xFA);
      }
      temp_v0_2 = arg0->unk36C;
      arg0->unk5F0 = arg0->unk5F0 | 2;
      *temp_v0_2 |= 0x10;
      arg0->unk36A = arg0->unk36A | 0x10;
      arg0->unk7CC = 1;
    }
  }
  else
  {
    if ((arg0->unk5F0 & 2) && (temp_v0 == 0))
    {
      func_1509BFB0(1, 0x9000, 0x10, 0);
      func_1509BFB0(1, 0x9000, 0xF, 0);
      arg0->unk5F0 = arg0->unk5F0 & (~2);
    }
    arg0->unk7CC = 2;
  }
}

/* structs.h declares unk5FC as u16 (golden loads it signed) and has no member at
   0x84A at all -- it falls inside pad814[0x68].  Shadow both here rather than
   touching structs.h, which 106 other translation units include. */
typedef struct {
    /* 0x000 */ u8  pad0[0x5FC];
    /* 0x5FC */ s16 unk5FC;
    /* 0x5FE */ u8  pad5FE[0x24C];
    /* 0x84A */ s16 unk84A;
} struct108_shadow;

void func_15125DB4(struct108 *arg0) {
    s32 state;

    state = arg0->unk23E;
    if ((arg0->unk3D4->unk120 != 0) || (D_80084480 != 0)) {
        arg0->unk36A = 0;
        *arg0->unk36C &= ~0x1F;
        return;
    }

    if (state != 0) {
        if ((state == 2) || (state == 0x12) || (state == 0x13) || (state == 0xA) ||
            (state == 0x34) || (state == 0x10) || (state == 0xF)) {
            if (state == 0xF) {
                func_1509BFB0(3, 0x9000, 0x18, 0x25, 0x13, 0x1E0);
            }
            *arg0->unk36C |= 0x10;
            arg0->unk36A |= 0x10;
            *arg0->unk36C &= ~0xF;
        } else {
            arg0->unk36A = D_800BE710[arg0->unk368];
        }
        if ((state == 3) || (state == 0xD) || (state == 0x1A) || (state == 0x2A)) {
            arg0->unk36A &= ~0xF;
            *arg0->unk36C &= ~0xF;
        }
        if (state == 0x29) {
            *arg0->unk36C |= 0x10;
            arg0->unk36A |= 0x10;
        }
    } else {
        if (arg0->unk2C & 0x100) {
            arg0->unk36A &= ~0xF;
            *arg0->unk36C &= ~0xF;
        }
    }

    if ((arg0->unk3D0->in_water == 1) && (arg0->unk2C & 0x80)) {
        arg0->unk36A &= ~0x1C;
        *arg0->unk36C &= ~0xC;
    }

    if (arg0->unk84 & 0x200000) {
        arg0->unk36A &= ~0xF;
    }

    if ((D_800C3671 != 0) || (arg0->unk5F0 & 0x80)) {
        arg0->unk36A &= ~0x1F;
        *arg0->unk36C &= ~0x1F;
    }

    if ((((u8 *) arg0->unk3D4)[0x7D] != 0) || (((u8 *) arg0->unk3D4)[0x1AC] != 0) ||
        (((u8 *) arg0->unk3D4)[0x27] != 0)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((((u8 *) arg0->unk3D4)[0x95] != 0) ||
        ((((struct108_shadow *) arg0)->unk5FC != 0) && (((u8 *) arg0->unk3D4)[0x1B3] != 0))) {
        arg0->unk36A &= ~0x1F;
        *arg0->unk36C &= ~0x1F;
    }

    if (((arg0->unk2C & 0x100) && (arg0->unk73C == 0)) || (arg0->unk73C == 3)) {
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
    }

    if ((arg0->unk2C & 0x40) && (((struct108_shadow *) arg0)->unk84A != 0)) {
        ((struct108_shadow *) arg0)->unk84A -= D_800BE9E4;
        arg0->unk36A &= ~0x10;
        *arg0->unk36C &= ~0x10;
        if (((struct108_shadow *) arg0)->unk84A <= 0) {
            ((struct108_shadow *) arg0)->unk84A = 0;
        }
    }
}

void func_15126138(struct108 *arg0) {
    struct127 *temp_v0_2;
    struct108 *unused;

    func_151247C0(arg0);

    temp_v0_2 = arg0->unk3D0;

    if (((temp_v0_2->x_position != arg0->unk2B0) ||
         (temp_v0_2->y_position != arg0->unk2B4) ||
         (temp_v0_2->z_position != arg0->unk2B8)) && ((arg0->unk2C & ~0x100) != 0)) {
        arg0->unk298 = 1;
        arg0->unk7E6 = 60;
        arg0->unk8ED = 0;
    } else {
        arg0->unk298 = 0;
    }

    if (arg0->unk84 & 0x200000) {
        temp_v0_2 = arg0->unk3D0; // again?!
        if (temp_v0_2->interaction_state != 0x1E) {
            if (temp_v0_2->unk102 == 0) {
                if ((*(arg0->unk36C) & 0xF) != 0) {
                    if ((arg0->unk2C & 0x40) == 0) {
                        func_151220D0(arg0);
                    }
                }
            }
        }
    }
    func_15124C38(arg0, 0);
}

typedef struct {
    u8 pad0[0x114];
    s16 unk114;
} struct_1512623C_arg1;

void func_1512623C(struct127 *arg0, struct_1512623C_arg1 *arg1, s32 arg2, f32 *arg3, f32 *arg4, f32 *arg5, s32 arg6) {
    s32 useObjectPos;
    f32 localPos[3];
    f32 worldPos[3];
    s32 boneIndex;
    s32 boneMtx;
    f32 offsetY;

    useObjectPos = 1;
    if (arg0->unk1D4 != NULL) {
        useObjectPos = 0;
        if (arg6 != 0) {
            boneIndex = 3;
            offsetY = 20.0f;
            localPos[0] = 0.0f;
            localPos[1] = offsetY;
            localPos[2] = 0.0f;
        } else if (arg2 == 0x1B) {
            boneIndex = 4;
            offsetY = 116.0f;
            localPos[1] = offsetY;
            localPos[0] = 0.0f;
            localPos[2] = 130.0f;
        } else {
            useObjectPos = 1;
        }

        if (useObjectPos == 0) {
            boneMtx = (s32)arg0->unk1D4;
            boneMtx += boneIndex << 6;
            func_15143134(localPos, worldPos, boneMtx);
            *arg3 = worldPos[0];
            *arg4 = worldPos[1];
            *arg5 = worldPos[2];
        }
    }

    if (useObjectPos != 0) {
        *arg3 = arg0->x_position;
        *arg4 = arg0->y_position + (arg1->unk114 * 0.75f);
        *arg5 = arg0->z_position;
    }
}
// ooh mama
#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15126378.s")
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  pad8[0x8];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ struct108 *unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u16 unk24;
} struct_15127520_arg0;

extern u16 D_800894E0[2][3];

s32 func_15127520(struct_15127520_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u16 *arg6) {
    struct108 *obj;
    u32 index;
    u32 oldTime;
    u32 newTime;
    f32 time;
    s32 changed;

    obj = arg0->unk18;
    index = arg0->unk1C >> 31;
    oldTime = arg0->unk1C & 0xFFFFFF;
    time = obj->unk1A4 * D_800A3580;
    changed = 0;

    if (*arg6 == D_800894E0[index][2]) {
        *arg6 = 0;
    }

    arg0->unk2 = obj->unk3D0->x_position;
    arg0->unk4 = obj->unk3D0->y_position;
    arg0->unk6 = obj->unk3D0->z_position;

    if (obj->unk3D4->unk197 == 2) {
        newTime = (u32)time;
        if (oldTime != newTime) {
            changed = 1;
            arg0->unk1C = (index << 31) | newTime;
        }

        if (changed) {
            if (*arg6 != D_800894E0[index][0]) {
                if (arg0->unk24 != 0) {
                    func_100111C8(arg0->unk24);
                }
                arg0->unk24 = 0;
                arg0->unk10 &= ~0x80;
                *arg6 = D_800894E0[index][0];
            }
        } else if (*arg6 == D_800894E0[index][0]) {
            if (arg0->unk24 != 0) {
                func_100111C8(arg0->unk24);
            }
            arg0->unk24 = 0;
            *arg6 = D_800894E0[index][1];
            func_10010F30(D_800894E0[index][1], 0x4E20, 0x40, 0, 0);
            arg0->unk10 &= ~0x80;
        } else if (arg0->unk10 & 0x80) {
            func_151CC290((s32)obj);
            func_10010F30(D_800894E0[index][2], 0x4E20, 0x40, 0, 0);
        }
    } else {
        return 1;
    }

    arg0->unk10 &= ~0x80;
    return 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_151277B0.s")
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151279A0;

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
    u8 pad79[0x114 - 0x79];
    s16 unk114;
    u8 pad116[0x130 - 0x116];
    Vec3_151279A0 unk130;
    Vec3_151279A0 unk13C;
    Vec3_151279A0 unk148;
    u8 pad154[0x16C - 0x154];
    f32 unk16C;
    f32 unk170;
    f32 unk174;
    f32 unk178;
    f32 unk17C;
    f32 unk180;
    u8 pad184[0x197 - 0x184];
    u8 unk197;
} struct_151279A0;

s32 func_151279A0(struct127 *arg0, OSContPad *arg1) {
    struct_151279A0 *s0;
    s32 type;
    Vec3_151279A0 pos;
    Vec3_151279A0 base;
    f32 speed;
    Vec3_151279A0 dir;
    f32 t;
    f32 t2;
    s32 buttons;
    f32 angle;

    s0 = (struct_151279A0 *) arg0->unk31C;
    type = s0->unk78;
    speed = D_800BE9E4 * 0.01666666753590107f;
    if ((type == 0xC) || (type == 0x1B)) {
        arg1->button |= 0x10;
    }
    if (((type == 0x23) || (type == 0x24) || (type == 0x16) || (type == 0x12) || (type == 0x18) ||
         (type == 0x41) || (type == 0xC) || (type == 0x14) || (type == 0x3F) || (type == 0x1B) ||
         (type == 0x1F) || (type == 0x29)) &&
        (arg1->button & 0x10) && (s0->unk197 == 0)) {
        s0->unk197 = 2;
        s0->unk13C = base;
        s0->unk174 = arg0->unk40 - 90.0f;
        s0->unk178 = 0.0f;
        s0->unk170 = 0.0f;
        s0->unk17C = 0.0f;
        s0->unk180 = 0.0f;
        s0->unk16C = s0->unk174;
        func_150627D4(arg0);
    }
    if (s0->unk197 == 0) {
        return 0;
    }
    if (s0->unk197 == 2) {
        buttons = arg1->button;
        angle = (arg0->unk40 - 90.0f) * 0.01745329238474369f;
        pos.x = arg0->x_position;
        pos.y = arg0->y_position + (s0->unk114 * 0.75f);
        pos.z = arg0->z_position;
        base = pos;
        pos.x = (sinf(angle) * 100.0f) + base.x;
        pos.z = (cosf(angle) * 100.0f) + base.z;
        base.y += arg0->y_position - arg0->old_y_position;
        if (!(buttons & 0x10) || (type == 0)) {
            s0->unk197 = 0;
            func_150627D4(arg0);
            return 0;
        }
        s0->unk174 -= arg1->stick_x * 0.02500000037252903f;
        s0->unk178 += arg1->stick_y * 0.02500000037252903f;
        func_15048758(&s0->unk174);
        if ((type == 0x1F) || (type == 0x29)) {
            s0->unk178 = (s0->unk178 < -20.0f) ? -20.0f : ((s0->unk178 > 80.0f) ? 80.0f : s0->unk178);
        } else {
            s0->unk178 = (s0->unk178 < -80.0f) ? -80.0f : ((s0->unk178 > 80.0f) ? 80.0f : s0->unk178);
        }
        func_15049688(&s0->unk16C, s0->unk174, &s0->unk17C, 6.0f, 8.0f, speed);
        func_15049688(&s0->unk170, s0->unk178, &s0->unk180, 6.0f, 8.0f, speed);
        dir.x = 1.0f;
        t = cosf(s0->unk170 * -0.01745329238474369f) * dir.x;
        dir.y = sinf(s0->unk170 * -0.01745329238474369f) * dir.x;
        dir.x = t;
        t = cosf(s0->unk16C * -0.01745329238474369f) * dir.x;
        dir.z = sinf(s0->unk16C * -0.01745329238474369f) * dir.x;
        dir.x = t;
        pos.x = base.x + dir.x;
        pos.y = base.y + dir.y;
        pos.z = base.z + dir.z;
        s0->unk148 = pos;
        s0->unk13C = base;
        s0->unk130 = dir;
        return 1;
    }

}

typedef struct {
    u8 pad0[0x84];
    f32 unk84;
    u8 pad88[0xF8];
} struct_15127EB8_D_800BE628;

void func_15127EB8(struct108 *arg0) {
    struct127 *temp_v1;
    struct127 *target;
    u8 temp_a0;

    func_151239CC(arg0, 1);
    arg0->unk3D4->unk197 = 0;
    func_151C9ED4(arg0);
    arg0->unk19C = 0.0f;
    arg0->unk1A0 = 0.0f;
    arg0->unk1A4 = 0.0f;
    arg0->unk1A8 = 0.0f;
    func_1510B32C(arg0->unk23D, 0.0f, 0.0f, 1.0f);
    ((struct_15127EB8_D_800BE628 *)D_800BE628)[arg0->unk23D].unk84 = 1.0f;
    func_150627D4(arg0->unk3D0);
    D_800DBFF4[arg0->unk23D] = 2;

    temp_v1 = arg0->unk3D0;
    temp_a0 = temp_v1->unk65;
    target = (temp_a0 != 0) ? &D_800CC2D0[temp_a0] - 1 : temp_v1;

    *(u8 *)((u8 *)target + 0x2FC) &= ~(1 << arg0->unk23D);
    target->unk74 &= ~(1 << arg0->unk23D);
    arg0->unk23C = 1;
}
// NON-MATCHING: not too far away
// void func_15127EB8(struct108 *arg0) {
//     struct127 *target;
//
//     func_151239CC(arg0, 1);
//     arg0->unk3D4->unk197 = (u8)0;
//     func_151C9ED4(arg0);
//     arg0->unk19C = 0.0f;
//     arg0->unk1A0 = 0.0f;
//     arg0->unk1A4 = 0.0f;
//     arg0->unk1A8 = 0.0f;
//     func_1510B32C(arg0->unk23D, 0.0f, 0.0f, 1.0f);
//     D_800BE628[arg0->unk23D].unk84 = 1.0f;
//     func_150627D4(arg0->unk3D0);
//     D_800DBFF4[arg0->unk23D] = (u8)2;
//
//     target = &arg0->unk3D0;
//     if (target->unk65 != 0) {
//         target = &D_800CC2D0[target->unk65];
//     }
//
//     target->unk2FC &= ~(1 << arg0->unk23D);
//     target->unk74 &= ~(1 << arg0->unk23D);
//     arg0->unk23C = (u8)1;
// }

void func_15127FEC(struct108 *arg0, s32 arg1, s32 arg2) {
    arg0->unk7F4 = 1;
    arg0->unk7F8 = arg0->unk2A4;
    arg0->unk7FC = arg0->unk2A8;
    arg0->unk800 = *(f32*)&arg0->unk2AC;
    func_1512A390();
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_14FF90/func_15128030.s")
void func_151283B8(struct108 *arg0) {
    switch (arg0->unk2C) {
    case 0x80:
        func_15122440(arg0);
        break;
    case 0x40:
        func_15129934(arg0);
        break;
    case 1:
        func_15122980(arg0);
        break;
    case 8:
        func_15121C80(arg0, 0.0f);
        break;
    case 2:
        func_151220D0(arg0);
        break;
    case 0x400:
        func_15122170(arg0);
        break;
    case 0x800:
        func_15122980(arg0);
        break;
    case 0x100:
        func_15120158(arg0);
        break;
    case 0x800000:
        func_1512DEA4(arg0);
        break;
    case 0x2000:
        break;
    }
}

void func_151284C4(struct108 *arg0) {
    struct127 *temp_v0;

    func_1512C490(arg0);
    temp_v0 = arg0->unk3D0;
    arg0->unk2B0 = temp_v0->x_position;
    arg0->unk2B4 = temp_v0->y_position;
    arg0->unk2B8 = temp_v0->z_position;
    if (arg0->unk23C != 0) {
        arg0->unk23C = arg0->unk23C - 1;
    }
    if ((arg0->unk84 & 8) != 0) {
        if ((s16)arg0->unk5FE <= 0) {
            arg0->unk5FE = 0x3C;
            func_15128774(arg0, arg0->unk3D0);
        }
    }
}
s32 func_15128540(struct108 *arg0) {
    s32 temp_v0;

    if (func_151253CC(arg0) != 0) {
        return 1;
    }

    if (func_15128030(arg0) != 0) {
        return 0;
    }

    temp_v0 = arg0->unk2C;
    if ((temp_v0 & 0x80000) != 0) {
        func_1512A360((u8 *)arg0);
        *(struct167 *)&arg0->unk2BC = *(struct167 *)&arg0->unk2A4;
        func_151256BC(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x100000) != 0) {
        func_1512E4B0(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x200000) != 0) {
        func_151219D0(arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if ((temp_v0 & 0x400000) != 0) {
        func_1512D380((s32)arg0);
        func_151236D0(arg0);
        func_151284C4(arg0);
        return 1;
    }

    if (func_15126378(arg0) != 0) {
        func_151284C4(arg0);
        return 1;
    }
    return 0;
}

void func_15128680(struct108 *arg0) {
}

void func_1512868C(struct108 *arg0) {
    struct127 *temp_v1_2;
    u8 idx;
    s32 i;

    for (i = 0; i < 21; i++)
    {
        arg0->unk20C[i] = 0;
    }

    arg0->unk2C = 1;
    arg0->unkDC = 0;
    arg0->unk134 = 1;
    arg0->unk84 = 14;
    arg0->unk1B4 = 3;
    func_15124B18(arg0);
    idx = arg0->unk23D;
    arg0->unk3D0 = temp_v1_2 = &D_800CC2D0[idx];
    arg0->unk368 = arg0->unk3CC = idx;
    arg0->unk3D4 = temp_v1_2->unk31C;
    arg0->unk190 = 0.0f;
    arg0->unk198 = 0.0f;
    arg0->unk18C = 0.0f;
    arg0->unk194 = 0.0f;
    arg0->unk674 = 1.0f;
    arg0->unk3D4->unk198 = 0;
    arg0->unk73C = 0;
}

void func_15128774(struct108 *arg0, struct127 *arg1) {
    arg0->unk35C = arg1->unk180;
    arg0->unk304 = arg0->unk2F8 = arg1->x_position;
    arg0->unk308 = arg0->unk2FC = arg1->y_position;
    arg0->unk30C = arg0->unk300 = arg1->z_position;
    arg0->unk37C = arg0->unk3D0->unk40 - 180.0f;
    arg0->unk3D4->unk18C = D_800A35AC;
    arg0->unk23C = 1;
}
