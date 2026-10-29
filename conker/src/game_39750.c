#define func_1513D2F0 func_1513D2F0_hdr
#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#undef func_1513D2F0


void func_1500C2A0(void) {
    s32 i;

    for (i = 0; i < 233; i++) {
        D_800DC468[i] = 0;
    }

    D_800DC460 = 0;
    D_800DC464 = 0;
    D_800DC63C = 0;
}


typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58;
    u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    u8 pad69;
    /* 0x6A */ u8 unk6A;
    u8 pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    u8 pad76[2];
} Part_1500D3AC; /* size 0x78 */

void func_15008340(s32, f32 *, f32 *, f32 *, f32 *);
void func_1510F800(s32);
void *func_15132A4C(void *, s32, s32, s32, u8, s32);

void func_1500C2F0(s32 arg0, f32 arg1) {
    f32 phi_f30;
    s32 pad;

    phi_f30 = func_1514462C(arg0) * arg1 * 1.999999995e-06f;
    if (phi_f30 > 1.0f) {
        f32 temp_f22;
        Part_1500D3AC part;
        f32 sp90;
        f32 sp8C;

        part.unk00 = 1.0f;
        part.unk04 = 1.0f;
        part.unk1C = 1.0f;
        part.unk20 = 1.0f;
        part.unk24 = 1.0f;
        part.unk50 = 0x5D40;
        part.unk54 = 0x12C;
        part.unk58 = 0;
        part.unk60 = 0xFF;
        part.unk61 = 0xE;
        part.unk62 = 0;
        part.unk63 = 0;
        part.unk64 = 0;
        part.unk65 = 0;
        part.unk66 = 0;
        part.unk67 = 0;
        part.unk68 = 2;
        part.unk6A = 0;
        part.unk6C = 0;
        part.unk70 = 0;
        part.unk72 = 1;
        part.unk74 = 0xFF;
        temp_f22 = 1.525902189e-05f;
        part.unk44 = 0.0f;
        do {
            s32 pad2;
            const u16 types[9] = { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F, 0x50, 0x51 };
            f32 phi_f20;
            f32 temp_f2;
            f32 temp_f0;

            func_15008340(arg0, &part.unk28, &part.unk30, &sp90, &sp8C);
            phi_f20 = sp90 - sp8C;
            temp_f2 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22) * (phi_f20 * 0.5f)) + (sp8C + (phi_f20 * 0.25f));
            part.unk38 = temp_f2;
            {
            f32 temp_f12;
            temp_f12 = fabsf(temp_f2 - sp90);
            part.unk2C = temp_f2;
            temp_f0 = fabsf(temp_f2 - sp8C);
            if (temp_f0 < temp_f12) {
                phi_f20 = temp_f0;
            } else {
                phi_f20 = temp_f12;
            }
            part.unk4C = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.75f) + 0.25f) * phi_f20;
            }
            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            part.unk34 = (temp_f0 + temp_f0) * 3.141592741f;
            part.unk3C = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.01300000027f) + 0.005000000354f;
            part.unk10 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk14 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk18 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk40 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 1.600000024f);
            part.unk48 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 1.600000024f);
            part.unk56 = types[(osGetCount() * func_150ADA20()) % 9U];
            if (part.unk56 == 0x4B || part.unk56 == 0x48 || part.unk56 == 0x4E || part.unk56 == 0x4F || part.unk56 == 0x50 || part.unk56 == 0x51) {
                temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.25f) + 0.5f;
                part.unk08 = temp_f0;
                part.unk0C = temp_f0;
            } else {
                temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.25f) + 1.0f;
                part.unk08 = temp_f0;
                part.unk0C = temp_f0;
            }
            func_1510F800(0);
            part.unk5C = func_1510FD20(part.unk28, part.unk30);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            phi_f30 -= 1.0f;
        } while (phi_f30 > 1.0f);
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_39750/func_1500C8D0.s")
typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
} Spawn_1500CE4C; /* size 0x4C */

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
} Extra_1500CE4C; /* size 0x1C */

void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);
void *memcpy(void *, const void *, u32);

void func_1500CE4C(s32 arg0, f32 arg1) {
    f32 phi_f26;
    s32 pad;

    phi_f26 = func_1514462C(arg0) * arg1 * 2.000000023e-07f;
    if (phi_f26 > 1.0f) {
        f32 temp_f22;
        f32 phi_f20;
        f32 temp_f2;
        Spawn_1500CE4C spawn;
        Extra_1500CE4C extra;
        f32 sp98;
        f32 sp94;
        f32 temp_f0;
        void *ret;
        s32 pad2;
        s32 pad3;

        spawn.unk00 = 0x57;
        spawn.unk01 = 0;
        spawn.unk02 = 0x3B03;
        spawn.unk04 = 0x12C;
        spawn.unk08 = 0;
        spawn.unk0C = 0;
        spawn.unk13 = 0xFF;
        spawn.unk34 = 0.0f;
        spawn.unk38 = 0.0f;
        spawn.unk3C = 0.0f;
        spawn.unk40 = 0x037C0000;
        spawn.unk45 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        extra.unk14 = 0.0f;
        spawn.unk10 = 0xFF;
        spawn.unk11 = 0xFF;
        spawn.unk12 = 0xFF;
        temp_f22 = 1.525902189e-05f;
        do {
            extern s32 D_800A4AA0;

            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 12.0f) + 10.0f;
            spawn.unk14 = temp_f0;
            spawn.unk18 = temp_f0;
            func_15008340(arg0, &spawn.unk1C, &spawn.unk24, &sp98, &sp94);
            phi_f20 = sp98 - sp94;
            temp_f2 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22) * (phi_f20 * 0.5f)) + (sp94 + (phi_f20 * 0.25f));
            extra.unk00 = temp_f2;
            {
            f32 temp_f12;
            temp_f12 = fabsf(temp_f2 - sp98);
            spawn.unk20 = temp_f2;
            temp_f0 = fabsf(temp_f2 - sp94);
            if (temp_f0 < temp_f12) {
                phi_f20 = temp_f0;
            } else {
                phi_f20 = temp_f12;
            }
            extra.unk04 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.75f) + 0.25f) * phi_f20;
            }
            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            extra.unk08 = (temp_f0 + temp_f0) * 3.141592741f;
            extra.unk0C = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.01300000027f) + 0.005000000354f;
            extra.unk10 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            extra.unk18 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            spawn.unk28 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            spawn.unk2C = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            spawn.unk30 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            spawn.unk44 = (osGetCount() * func_150ADA20()) % 156U + 100;
            func_1510F800(0);
            spawn.unk48 = func_1510FD20(spawn.unk1C, spawn.unk24);
            ret = func_1513D2F0(&spawn, &D_800A4AA0, 0x23, 0, 0, 0x1E, 0, 3, 0xFF, 0x1C, 0xFF, 1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x110, &extra, sizeof(extra));
            }
            phi_f26 -= 1.0f;
        } while (phi_f26 > 1.0f);
    }
}

void func_1500D3AC(s32 arg0, f32 arg1) {
    f32 phi_f30;
    s32 pad;

    phi_f30 = func_1514462C(arg0) * arg1 * 2.000000023e-07f;
    if (phi_f30 > 1.0f) {
        f32 temp_f22;
        Part_1500D3AC part;
        f32 sp90;
        f32 sp8C;
        f32 phi_f20;
        f32 temp_f2;
        f32 temp_f0;

        part.unk00 = 1.0f;
        part.unk04 = 1.0f;
        part.unk1C = 1.0f;
        part.unk20 = 1.0f;
        part.unk24 = 1.0f;
        part.unk50 = 0x4D40;
        part.unk54 = 0x12C;
        part.unk58 = 0;
        part.unk60 = 0xFF;
        part.unk61 = 0xE;
        part.unk62 = 0;
        part.unk63 = 0;
        part.unk64 = 0;
        part.unk65 = 0;
        part.unk66 = 0;
        part.unk67 = 0;
        part.unk68 = 2;
        part.unk6A = 0;
        part.unk6C = 0;
        part.unk70 = 0;
        part.unk72 = 1;
        part.unk74 = 0xFF;
        part.unk56 = 0x53;
        temp_f22 = 1.525902189e-05f;
        part.unk44 = 0.0f;
        do {
            func_15008340(arg0, &part.unk28, &part.unk30, &sp90, &sp8C);
            phi_f20 = sp90 - sp8C;
            temp_f2 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22) * (phi_f20 * 0.5f)) + (sp8C + (phi_f20 * 0.25f));
            part.unk38 = temp_f2;
            {
            f32 temp_f12;
            temp_f12 = fabsf(temp_f2 - sp90);
            part.unk2C = temp_f2;
            temp_f0 = fabsf(temp_f2 - sp8C);
            if (temp_f0 < temp_f12) {
                phi_f20 = temp_f0;
            } else {
                phi_f20 = temp_f12;
            }
            part.unk4C = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.75f) + 0.25f) * phi_f20;
            }
            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            part.unk34 = (temp_f0 + temp_f0) * 3.141592741f;
            part.unk3C = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.01300000027f) + 0.005000000354f;
            part.unk10 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk14 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk18 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 360.0f);
            part.unk40 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 1.600000024f);
            part.unk48 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 1.600000024f);
            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.25f) + 1.0f;
            part.unk08 = temp_f0;
            part.unk0C = temp_f0;
            func_1510F800(0);
            part.unk5C = func_1510FD20(part.unk28, part.unk30);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            phi_f30 -= 1.0f;
        } while (phi_f30 > 1.0f);
    }
}
// void func_1500D3AC(s32 arg0, f32 arg1) {
//
//     f32 temp_f0;
//     struct171 tmp;
//     f32 temp_f12;
//     f32 phi_f18;
//     f32 phi_f30; // main tmp
//
//
//     phi_f30 = func_1514462C(arg0) * arg1 * D_8009616C;
//
//     if (phi_f30 > 1.0f) {
//         tmp.unk8 = 1.0f;
//         tmp.unkC = 1.0f;
//         tmp.unk24 = 1.0f;
//         tmp.unk28 = 1.0f;
//         tmp.unk2C = 1.0f;
//         tmp.unk58 = 19776;
//         tmp.unk5C = 300;
//         tmp.unk60 = 0;
//         tmp.unk68 = 255;
//         tmp.unk69 = 14;
//         tmp.unk6A = 0;
//         tmp.unk6B = 0;
//         tmp.unk6C = 0;
//         tmp.unk6D = 0;
//         tmp.unk6E = 0;
//         tmp.unk6F = 0;
//         tmp.unk70 = 2;
//         tmp.unk72 = 0;
//         tmp.unk74 = 0;
//         tmp.unk78 = 0;
//         tmp.unk7A = 1;
//         tmp.unk7C = 0xFF;
//         tmp.unk5E = 0x53;
//         tmp.unk4C = 0.0f;
//
//         do {
//             func_15008340(arg0, &tmp.unk30, &tmp.unk38, &tmp.unk4, &tmp.unk0);
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk34 = tmp.unk40 = (phi_f18 * D_80096170 * ((tmp.unk4 - tmp.unk0) * 0.5f)) + (tmp.unk0 + ((tmp.unk4 - tmp.unk0) * 0.25f));
//
//             temp_f12 = fabsf(tmp.unk40 - tmp.unk4);
//             temp_f0 = fabsf(tmp.unk40 - tmp.unk0);
//
//             if (temp_f0 < temp_f12) {
//                 temp_f0 = temp_f0;
//             } else {
//                 temp_f0 = temp_f12;
//             }
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk54 = ((phi_f18 * D_80096170 * 0.75f) + 0.25f) * temp_f0;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk3C = ((phi_f18 * D_80096170) + (phi_f18 * D_80096170)) * D_80096174;
//
//             phi_f18 = (u16)(osGetCount() * func_150ADA20());
//             tmp.unk44 = (phi_f18 * D_80096170 * D_80096178) + D_8009617C;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk18 = phi_f18 * D_80096170 * 360.0f;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk1C = phi_f18 * D_80096170 * 360.0f;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk20 = phi_f18 * D_80096170 * 360.0f;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk48 = phi_f18 * D_80096170 * D_80096180;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk50 = phi_f18 * D_80096170 * D_80096184;
//
//             phi_f18 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk10 = tmp.unk14 = (phi_f18 * D_80096170 * 0.25f) + 1.0f;
//
//             func_1510F800(0);
//             tmp.unk64 = func_1510FD20(tmp.unk30, tmp.unk38);
//             func_15132A4C(&tmp, 3, 0xFF, 0, 0xFF, 1);
//
//         } while ((phi_f30 -= 1.0f) > 1.0f);
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_39750/func_1500D8C8.s")

typedef struct {
    s32 unk0;
    u8  pad4[0x14];
    f32 unk18;
    f32 unk1C;
    u8  pad20[0x4];
    f32 unk24;
    s8  unk28;
    s8  unk29;
    s8  unk2A;
    s8  unk2B;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8  pad3C[0x4];
    s32 unk40;
    s32 unk44;
    s16 unk48;
    s16 unk4A;
    s32 unk4C;
    s32 unk50;
    u8  unk54;
    u8  unk55;
    u8  unk56;
    u8  unk57;
    u8  unk58;
    u8  unk59;
    u8  unk5A;
    u8  unk5B;
    u8  unk5C;
    u8  unk5D;
    s16 unk5E;
    s16 unk60;
    s16 unk62;
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
    f32 unk90;
    f32 unk94;
    s32 unk98;
    s32 unk9C;
    s8  unkA0;
    s8  unkA1;
    s8  unkA2;
    s8  unkA3;
    s8  unkA4;
    s8  unkA5;
} struct_1500DE88;

void func_15008340(s32, f32 *, f32 *, f32 *, f32 *);
void func_1510F800(s32);
u8 *func_15130280(s32 *, s32, s32, s32, s32, s32);

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
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
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    s8 unk65;
} Params_1500DE88;

typedef struct {
    f32 unk00;
    s8 unk04;
    s8 unk05;
    s8 unk06;
    s8 unk07;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    u8 pad18[4];
} Extra_1500DE88;

void func_1500DE88(s32 arg0, f32 arg1) {
    f32 phi_f24;

    phi_f24 = func_1514462C(arg0) * arg1 * 1.999999995e-06f;
    if (phi_f24 > 1.0f) {
        f32 temp_f30;
        f32 temp_f28;
        f32 temp_f22;
        Params_1500DE88 params;
        Extra_1500DE88 extra;
        void *temp_v0;
        f32 sp7C;
        f32 sp78;
        f32 phi_f20;
        f32 temp_f2;
        f32 temp_f0;


        temp_f30 = 0.3000000119f;
        params.unk08 = 0x5103;
        params.unk00 = 0x200005;
        params.unk17 = 0xFF;
        params.unk1C = 0xFF;
        temp_f28 = 0.150000006f;
        params.unk04 = 0;
        params.unk0C = 0;
        params.unk10 = 0;
        params.unk15 = 0;
        params.unk16 = 0;
        params.unk19 = 0;
        params.unk1A = 0;
        params.unk3C = 0.0f;
        params.unk40 = 0.0f;
        params.unk44 = 0.0f;
        params.unk22 = 1;
        params.unk24 = 1.0f;
        params.unk0A = 300;
        params.unk1E = 1;
        params.unk20 = 0xFF;
        params.unk60 = 8;
        params.unk61 = 6;
        params.unk62 = 23;
        params.unk63 = -1;
        params.unk64 = -1;
        params.unk65 = 0;
        extra.unk04 = 0;
        extra.unk05 = 0;
        extra.unk10 = 1.0f;
        extra.unk14 = 1.0f;
        params.unk1D = 108;
        temp_f22 = 1.525902189e-05f;
        do {
            temp_f0 = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
            temp_f0 = (temp_f0 * temp_f22 * 600.0f) + 200.0f;
            params.unk2C = temp_f0;
            params.unk28 = temp_f0;
            extra.unk00 = temp_f0;
            params.unk1B = ((u32)(osGetCount() * func_150ADA20()) % 71U) + 50;
            extra.unk06 = (u32)(osGetCount() * func_150ADA20()) % 3U;
            extra.unk07 = (u32)(osGetCount() * func_150ADA20()) % 3U;
            extra.unk08 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * temp_f28) + temp_f30) * extra.unk00;
            extra.unk0C = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * temp_f28) + temp_f30) * extra.unk00;
            params.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x4000 | 0x8000 | 0x20000 | 0x800000;

            params.unk18 = ((u32)(osGetCount() * func_150ADA20()) % 56U) + 80;
            params.unk14 = ((u32)(osGetCount() * func_150ADA20()) % 46U) + 180;
            func_15008340(arg0, &params.unk30, &params.unk38, &sp7C, &sp78);
            phi_f20 = sp7C - sp78;
            temp_f2 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22) * (phi_f20 * 0.5f)) + (sp78 + (phi_f20 * 0.25f));
            params.unk4C = temp_f2;
            {
                f32 temp_f12;

                temp_f12 = fabsf(temp_f2 - sp7C);
                params.unk34 = temp_f2;
                temp_f0 = fabsf(temp_f2 - sp78);
                if (temp_f0 < temp_f12) {
                    phi_f20 = temp_f0;
                } else {
                    phi_f20 = temp_f12;
                }
                params.unk54 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.75f) + 0.25f) * phi_f20;

            }
            temp_f0 = (((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22);
            params.unk48 = (temp_f0 + temp_f0) * 3.141592741f;
            params.unk50 = ((((u32)(osGetCount() * func_150ADA20()) & 0xFFFF) * temp_f22 * 0.01000000071f) + 0.002000000095f);
            func_1510F800(0);
            params.unk5C = func_1510FD20(params.unk30, params.unk38);
            temp_v0 = func_15130280(&params.unk00, 2, 0, 28, 255, 1);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0xA8, &extra, 28);
            }
            phi_f24 -= 1.0f;
        } while (phi_f24 > 1.0f);
    }
}
// NON-MATCHING: not a bad start..
// void func_1500DE88(s32 arg0, f32 arg1) {
//
//     f32 temp_f0;    // tmp
//     f32 phi_f10;    // tmp
//     f32 temp_f12;   // another tmp
//     f32 temp_f22;   // D_800961B4
//     f32 temp_f28;   // D_800961B0
//     f32 temp_f30;   // D_800961AC
//     struct21 *temp_v0;    // dummy struct
//     s32 phi_v1;
//     struct170 tmp;
//
//
//     f32 phi_f24 = func_1514462C(arg0) * arg1 * D_800961A8;
//
//     if (phi_f24 > 1.0f) {
//         tmp.unk48 = 20739;
//         tmp.unk40 = 0x200005;
//         tmp.unk57 = 0xFF;
//         tmp.unk5C = 0xFF;
//         tmp.unk44 = 0.0f;
//         tmp.unk4C = 0.0f;
//         tmp.unk50 = 0.0f;
//         tmp.unk55 = 0;
//         tmp.unk56 = 0;
//         tmp.unk59 = 0;
//         tmp.unk5A = 0;
//         tmp.unk7C = 0.0f;
//         tmp.unk80 = 0.0f;
//         tmp.unk84 = 0.0f;
//         tmp.unk62 = 1;
//         tmp.unk64 = 1.0f;
//         tmp.unk4A = 300;
//         tmp.unk5E = 1;
//         tmp.unk60 = 0xFF;
//         tmp.unkA0 = 8;
//         tmp.unkA1 = 6;
//         tmp.unkA2 = 23;
//         tmp.unkA3 = -1;
//         tmp.unkA4 = -1;
//         tmp.unkA5 = 0;
//         tmp.unk28 = 0;
//         tmp.unk29 = 0;
//         tmp.unk34 = 1.0f;
//         tmp.unk38 = 1.0f;
//         tmp.unk5D = 108;
//
//         temp_f30 = D_800961AC;
//         temp_f28 = D_800961B0;
//         temp_f22 = D_800961B4;
//
//         do {
//             phi_f10 = (u16) (osGetCount() * func_150ADA20()) ;
//
//             tmp.unk68 = tmp.unk6C = tmp.unk24 = (phi_f10 * temp_f22 * 600.0f) + 200.0f;
//
//             tmp.unk5B = (osGetCount() * func_150ADA20()) % 0x47U + 0x32;
//             tmp.unk2A = (osGetCount() * func_150ADA20()) % 3U;
//             tmp.unk2B = (osGetCount() * func_150ADA20()) % 3U;
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk2C = ((phi_f10 * temp_f22 * temp_f28) + temp_f30) * tmp.unk24;
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk30 = ((phi_f10 * temp_f22 * temp_f28) + temp_f30) * tmp.unk24;
//
//             if ((func_150ADA20() & 1) != 0) {
//                 tmp.unk0 = 0x40;
//             } else {
//                 tmp.unk0 = 0;
//             }
//
//             if ((func_150ADA20() & 1) != 0) {
//                 phi_v1 = 0x80;
//             } else {
//                 phi_v1 = 0;
//             }
//             tmp.unk98 = phi_v1 | tmp.unk0 | 0xC000 | 0x20000 | 0x800000;
//             tmp.unk58 = ((osGetCount() * func_150ADA20()) % 0x38U) + 0x50;
//             tmp.unk54 = ((osGetCount() * func_150ADA20()) % 0x2EU) + 0xB4;
//             func_15008340(arg0, &tmp.unk70, &tmp.unk78, &tmp.unk1C, &tmp.unk18);
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//
//             temp_f0 = tmp.unk1C - tmp.unk18;
//             tmp.unk8C = (phi_f10 * temp_f22 * (temp_f0 * 0.5f)) + (tmp.unk18 + (temp_f0 * 0.25f));
//             tmp.unk74 = fabsf(tmp.unk8C - tmp.unk1C);
//             temp_f0 = fabsf(tmp.unk74 - tmp.unk18);
//             if (tmp.unk74 < temp_f0) {
//                 temp_f0 = tmp.unk74;
//             }
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk94 = ((phi_f10 * temp_f22 * 0.75f) + 0.25f) * temp_f0;
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//             temp_f0 = phi_f10 * temp_f22;
//             tmp.unk88 = (temp_f0 + temp_f0) * D_800961B8;
//
//             phi_f10 = (u16) (osGetCount() * func_150ADA20());
//             tmp.unk90 = (phi_f10 * temp_f22 * D_800961BC) + D_800961C0;
//             func_1510F800(0);
//             tmp.unk9C = func_1510FD20(tmp.unk70, tmp.unk78);
//             temp_v0 = func_15130280(&tmp.unk40, 2, 0, 28, 255, 1);
//             if (temp_v0 != NULL) {
//                 memcpy(&temp_v0->unkA8, &tmp.unk24, 28); // memcpy  temp_v0->A8 ?
//             }
//             phi_f24 -= 1.0f;
//         }
//         while (phi_f24 > 1.0f);
//     }
// }
