#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15150F90(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15153634(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern f32 D_800AA8E8;
extern f32 D_800AA8EC;
extern f32 D_800AA8F0;
extern f32 D_800AA8F4;
extern f32 D_800AA8F8;
extern f32 D_800AA8FC;
extern f32 D_800AA900;
extern f32 D_800AA904;
extern f32 D_800AA908;
extern f32 D_800AA90C;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8  unk60;
    /* 0x61 */ u8  unk61;
    /* 0x62 */ u8  unk62;
    /* 0x63 */ u8  unk63;
    /* 0x64 */ u8  unk64;
    /* 0x65 */ u8  unk65;
    /* 0x66 */ u8  unk66;
    /* 0x67 */ u8  unk67;
    /* 0x68 */ u8  unk68;
    /* 0x69 */ u8  pad69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 pad78;
} S151BEEE0;

typedef struct { u8 b[3]; } U8x3_151BEEE0;
extern U8x3_151BEEE0 D_8008FBC0;
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_15133760(void *, void *);

void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    void *ret;
    U8x3_151BEEE0 tbl = D_8008FBC0;

    if (arg4 >= 3) {
        return NULL;
    }
    {
    S151BEEE0 sp44;
    s32 i;

    sp44.unk0 = 1.0f;
    sp44.unk4 = 1.0f;
    sp44.unkC = 0.9230000377f;
    sp44.unk8 = 0.9230000377f;
    sp44.unk10.unk0 = 0.0f;
    sp44.unk10.unk4 = arg0;
    sp44.unk10.unk8 = 0.0f;
    sp44.unk1C = 1.0f;
    sp44.unk20 = 1.0f;
    sp44.unk24 = 1.0f;
    sp44.unk28 = *arg1;
    sp44.unk34.unk0 = 0.0f;
    sp44.unk34.unk4 = 0.0f;
    sp44.unk34.unk8 = 0.0f;
    sp44.unk40.unk0 = 0.0f;
    sp44.unk40.unk4 = 0.0f;
    sp44.unk40.unk8 = 0.0f;
    sp44.unk4C = 0.0f;
    sp44.unk50 = (arg5 ? 0x4000 : 0) | 0x1D00 | 0x80000 | 0x40000;
    sp44.unk54 = 0x12C;
    sp44.unk56 = 0x26;
    sp44.unk58 = tbl.b[arg4];
    sp44.unk5C = arg6;
    sp44.unk60 = 0xFF;
    if (D_800BE9F0 == 7) {
        sp44.unk61 = 0x18;
    } else {
        sp44.unk61 = 0;
    }
    sp44.unk62 = 0;
    sp44.unk63 = 0;
    sp44.unk64 = 0;
    sp44.unk65 = 0;
    sp44.unk66 = 0;
    sp44.unk67 = 0;
    sp44.unk68 = 2;
    sp44.unk6A = 0;
    sp44.unk6C = 0;
    sp44.unk70 = 0;
    sp44.unk72 = 1;
    sp44.unk74 = 0xFF;
    ret = func_15132A4C(&sp44, arg2, arg3, 0, arg7, arg8);
    if (ret != NULL) {
        for (i = 0; i < 2; i++) {
            func_15133760((u8 *)ret + 0x90 + i * 0x40, ret);
        }
    }
    return ret;
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_151BF0C8;

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3F_151BF0C8 unk8;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 unk3C;
    u8 pad3D;
    s16 unk3E;
    s16 unk40;
} Struct151BF0C8_A;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3F_151BF0C8 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    u8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} Struct151BF0C8_B;

void func_151BF0C8(Vec3F_151BF0C8 *arg0) {
    Struct151BF0C8_A sp84;
    Struct151BF0C8_B sp18;

    sp84.unk0 = 0x18;
    sp84.unk4 = 0xA;
    sp84.unk8 = *arg0;
    sp84.unk14 = D_800AA8E8;
    sp84.unk18 = D_800AA8EC;
    sp84.unk1C = 0;
    sp84.unk1E = 0xFF;
    sp84.unk20 = -0x32;
    sp84.unk22 = 0x3C;
    sp84.unk24 = 13.0f;
    sp84.unk28 = 13.0f;
    sp84.unk2C = D_800AA8F0;
    sp84.unk30 = D_800AA8F4;
    sp84.unk34 = D_800AA8F8;
    sp84.unk38 = D_800AA8FC;
    sp84.unk3C = 3;
    sp84.unk3E = 0x41;
    sp84.unk40 = 0x1E;
    func_15150F90(&sp84, 1, 0xFF, 1);

    sp18.unk0 = 0x14;
    sp18.unk2 = 8;
    sp18.unk4 = 0x6C;
    sp18.unk6 = 0x5103;
    sp18.unk8 = 0x200005;
    sp18.unkC = 0;
    sp18.unk10 = 0x28;
    sp18.unk12 = 0x28;
    sp18.unk14 = 0;
    sp18.unk18 = 0;
    sp18.unk1F = 0xFF;
    sp18.unk1C = 0xFF;
    sp18.unk1D = 0x91;
    sp18.unk1E = 6;
    sp18.unk20 = 0xFF;
    sp18.unk21 = 0xFF;
    sp18.unk22 = 0;
    sp18.unk23 = 0x14;
    sp18.unk24 = 0xC8;
    sp18.unk25 = 0xFF;
    sp18.unk26 = 0x32;
    sp18.unk28 = 5;
    sp18.unk2A = 0x32;
    sp18.unk2C = D_800AA900;
    sp18.unk30 = 175.0f;
    sp18.unk34 = 160.0f;
    sp18.unk38 = *arg0;
    sp18.unk44 = 0;
    sp18.unk46 = -0x33;
    sp18.unk48 = 0xFF;
    sp18.unk4A = 0x50;
    sp18.unk4C = 4.0f;
    sp18.unk50 = 13.0f;
    sp18.unk54 = D_800AA904;
    sp18.unk58 = D_800AA908;
    sp18.unk5C = 0x840E07;
    sp18.unk60 = 0x10;
    sp18.unk61 = -1;
    sp18.unk62 = 8;
    sp18.unk63 = 6;
    sp18.unk64 = 1;
    sp18.unk68 = D_800AA90C;
    func_15153634(&sp18, 0xFF, 0xFF, 1);
}

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S151BE850;
typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} S151BF81CPart;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} S151BF81CDigit;

/* The 0x18-byte block func_151BF340 copies to object+0x28. */
typedef struct {
    /* 0x00 */ S151BF81CPart *unk0;
    /* 0x04 */ S151BF81CDigit *unk4[2];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
} S151BF81CState;

typedef struct {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ S151BF81CState unk28;
} S151BF81CObj;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} S151BF340Digit;

typedef struct {
    /* 0x00 */ struct17 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} S151BF340Payload;

extern void *func_1515548C(S151BF340Digit *, s32, s32, s32, s32, u8, s32);

void func_151BF340(u8 arg0, s32 arg1) {
    S151BF81CState sp160;
    struct260 *ret;
    s32 count;
    s32 digits[2];
    S151BF81CState *s;

    func_151494E0(0, 0x57);
    count = func_150859AC(0, 3);
    if (count != 0) {
        count--;
    }
    if (count >= 100) {
        count = 99;
    }
    digits[0] = count / 10;
    digits[1] = count % 10;
    sp160.unk0 = NULL;
    bzero(sp160.unk4, 8);
    sp160.unkC = 110.0f;
    sp160.unk10 = sinf(1.905000091f);
    sp160.unk14 = 0;
    ret = func_15149130(0x12C, -1, 0x60, -1, 0, 0x49, (struct37 *)0x18, arg0, arg1);
    if (ret != NULL) {
        s = &((S151BF81CObj *)ret)->unk28;
        memcpy(s, &sp160, 0x18);
        {
        S151BE850 spF4;
        S151BF340Payload spE0;
        S151BF340Digit sp84;

        spF4.unk38 = 0;
        spF4.unk00 = 0xC0;
        spF4.unk01 = -1;
        spF4.unk02 = 5;
        spF4.unk03 = 3;
        spF4.unk04 = 0;
        spF4.unk06 = 0x12C;
        spF4.unk08 = 0xA3;
        spF4.unk0C = 8;
        spF4.unk10 = 0x100000;
        spF4.unk14 = 0x200004;
        spF4.unk18 = 0x60601;
        spF4.unk2D = 8;
        spF4.unk1C = 1;
        spF4.unk20 = 0x38;
        spF4.unk2C = 0;
        spF4.unk24 = 0x80;
        spF4.unk28 = 0x20;
        spF4.unk30 = 0xFF;
        spF4.unk31 = 0xFF;
        spF4.unk32 = 0xFF;
        spF4.unk33 = 0xFF;
        spF4.unk34 = 0xFF;
        spF4.unk35 = 0xFF;
        spF4.unk36 = 0xFF;
        spF4.unk37 = 0xFF;
        spF4.unk3C = 0;
        spF4.unk40 = 0;
        spF4.unk41 = 1;
        spF4.unk54 = 1;
        spF4.unk56 = 0xFF;
        spF4.unk44.unk0 = 300.0f;
        if (D_800D2458) {
            spF4.unk44.unk4 = 32.0f;
        } else {
            spF4.unk44.unk4 = 79.0f;
        }
        spF4.unk44.unk8 = 0.0f;
        spE0.unk0 = *(struct17 *)&D_800A5480;
        spE0.unkC = 0.3010000288f;
        spE0.unk10 = 0.3010000288f;
        s->unk0 = func_15157010(&spF4, 0, 1.0f, 0, 0, 0x14, arg0, arg1);
        if (s->unk0 != NULL) {
            memcpy((u8 *)s->unk0 + 0x120, &spE0, 0x14);
        }
        {
            const s32 tbl[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
            u8 i;

            sp84.unk00 = -500.0f;
            if (D_800D2458) {
                sp84.unk04 = -15.0f;
            } else {
                sp84.unk04 = -62.0f;
            }
            sp84.unk12 = 0x12C;
            sp84.unk14 = 0x20;
            sp84.unk16 = 1;
            sp84.unk18 = 0xFF;
            sp84.unk1A = 0;
            sp84.unk1B = 0xFF;
            sp84.unk1C = 0xFF;
            sp84.unk1D = 0xFF;
            sp84.unk1E = 0xFF;
            sp84.unk1F = 0xFF;
            sp84.unk20 = 0xFF;
            sp84.unk21 = 0xFF;
            sp84.unk22 = 0xFF;
            sp84.unk23 = 0xFF;
            sp84.unk24 = 0;
            sp84.unk28 = 0x200004;
            sp84.unk2C = 0x9F0601;
            sp84.unk40 = 0;
            sp84.unk41 = 0;
            sp84.unk30 = 0x19;
            sp84.unk34 = 0x13;
            sp84.unk38 = 0x80;
            sp84.unk3C = 0x20;
            sp84.unk44 = 0;
            sp84.unk50 = 0.0f;
            sp84.unk54 = 0.0f;
            sp84.unk0C = 16.0f;
            sp84.unk08 = 16.0f;
            sp84.unk48 = 1.f;
            sp84.unk4C = 1.f;
            if (digits[0] == 0) {
                sp84.unk10 = tbl[digits[1]];
                s->unk4[0] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                s->unk4[1] = NULL;
            } else {
                for (i = 0; i < 2; i++) {
                    sp84.unk10 = tbl[digits[i]];
                    s->unk4[i] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                }
            }
        }
        }
    }
}

typedef struct Game1EC390Animation {
    void *primary;
    void *secondary[2];
    f32 timer;
    f32 amount;
    u8 flags;
} Game1EC390Animation;

s32 func_1517EF00(s32);
s32 func_15181CC8(s32);
extern f32 D_800AA918, D_800AA91C, D_800AA920;
extern f32 D_800AA924, D_800AA928, D_800AA92C;

void func_151BF81C(void *arg0) {
    f32 amount;
    Game1EC390Animation *animation;

    if (D_800BEAC1 != 0 || (animation = (Game1EC390Animation *)((u8 *)arg0 + 0x28), D_800BEAC0 != 0)) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
        return;
    }
    if (!(animation->flags & 1)) {
        if (D_800C35EA == 1) return;
        if (func_15181CC8(0) == 0) return;
        if (func_1517EF00(0) != 0 || D_800BE9B4 != 0) return;
        animation->flags |= 1;
    }
    if (animation->timer < D_800AA918) {
        amount = sinf(animation->timer * D_800AA91C * D_800AA920);
    } else if (D_800AA924 < animation->timer) {
        amount = sinf((110.0f - animation->timer) * D_800AA928 * D_800AA92C);
    } else {
        amount = animation->amount;
    }
    if (animation->primary != NULL) {
        *(f32 *)((u8 *)animation->primary + 0x54) = amount * -85.0f + 150.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->primary + 0x58) = 32.0f;
        else *(f32 *)((u8 *)animation->primary + 0x58) = 79.0f;
    }
    if (animation->secondary[0] != NULL) {
        *(f32 *)((u8 *)animation->secondary[0] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) - 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -62.0f;
    }
    if (animation->secondary[1] != NULL) {
        *(f32 *)((u8 *)animation->secondary[1] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) + 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -62.0f;
    }
    animation->timer -= D_800BE9A4;
    if (animation->timer < 0.0f) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
    }
}


void func_151BFB2C(struct260 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)((u8 *)arg0 + 0x28);

    if (p[0] != 0) {
        func_1516972C((struct102 *)p[0]);
    }
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        v = p[i + 1];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
}

void func_151BFBA4(struct260 *arg0) {
    func_151BFB2C(arg0);
    func_1514933C(arg0);
}
