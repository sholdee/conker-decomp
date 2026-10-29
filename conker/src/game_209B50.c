#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E[2];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A[2];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ u8 pad41[3];
    /* 0x44 */ f32 unk44;
} Struct151DC6A0;

typedef struct {
    f32 v[3];
} Vec3Copy_151DC6A0;

void func_15103254(s16, u8, f32, struct127 *, s32, u8, s32);
void func_15150178(Struct151DC6A0 *arg0, f32 *arg1, s32 arg2, u8 arg3, s32 arg4);
void func_151D3F14(struct127 *, u8, s32);
void func_15182670(u8, u8, u8, u8, s32, s32, u8, s32);

void func_151DC6A0(struct127 *arg0, u8 arg1, s32 arg2) {
    func_15103254(func_150ADA20() % 5 + 12, func_150ADA20() % 56 + 200, func_150ADA68() * 600.0f + 1700.0f, arg0, 0xFF, arg1, arg2);
    {
    Struct151DC6A0 sp38;
    *(Vec3Copy_151DC6A0 *)sp38.unk8 = *(Vec3Copy_151DC6A0 *)arg0;
    sp38.unk14 = 0xC;
    sp38.unk16 = 7;
    sp38.unk0 = 0;
    sp38.unk2 = 0xFF;
    sp38.unk4 = -0x40;
    sp38.unk6 = 0x3C;
    sp38.unk20 = 0x24;
    sp38.unk22 = 0x3C;
    sp38.unk2C = 0xC8;
    sp38.unk2D = 0x37;
    sp38.unk38 = 0;
    sp38.unk39 = 0xC;
    sp38.unk40 = 1;
    sp38.unk18 = 8.0f;
    sp38.unk1C = 10.0f;
    sp38.unk24 = -1.097000003f;
    sp38.unk28 = 0.3700000048f;
    sp38.unk30 = 280.0f;
    sp38.unk34 = 390.0f;
    sp38.unk3C = 0.6000000238f;
    sp38.unk44 = 1.0f;
    func_15150178(&sp38, sp38.unk8, 0, arg1, arg2);
    }
    func_151D3F14(arg0, arg1, arg2);
    func_15182670(0xFF, 0xFF, 0xFF, func_150ADA20() % 51 + 150, func_150ADA20() % 5 + 8, 0, arg1, arg2);
}

void func_151DC6A0(struct127 *arg0, u8 arg1, s32 arg2);

struct dc8bc_a { s32 unk0[3]; };

void func_151DC8BC(struct127 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5) {
    struct {
        struct dc8bc_a unk0;
        f32 unkC;
        u8 unk10;
    } sp34;
    struct260 *temp_v0;

    if (arg2 != 0) {
        func_151DC6A0(arg0, arg4, arg5);
    }

    sp34.unk0 = *(struct dc8bc_a *)arg0;
    sp34.unkC = 0.0f;
    sp34.unk10 = arg3;

    temp_v0 = func_15149130((s16)arg1, -1, 0x5B, -1, 1, 0, (struct37 *)0x14, arg4, arg5);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x14);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
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
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn151DC97C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra151DC97C;

typedef struct {
    struct17 pos;
    f32 accum;
    u8 unk10;
} Sub151DC97C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub151DC97C unk28;
} Obj151DC97C;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151DC97C(Obj151DC97C *arg0) {
    Sub151DC97C *sub;

    sub = &arg0->unk28;
    sub->accum += (0.1360000074f + func_150ADA68() * 0.1210000068f) * D_800BE9A4;
    if (sub->accum > 1.0f) {
        Spawn151DC97C spawn;
        Extra151DC97C extra;
        void *ret;
        u8 i;

        extra.unkC = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk1E = 0x28;
        spawn.unk20 = 6;
        spawn.unk58 = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x78;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.006579041f;
        spawn.unk30 = sub->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        if (sub->unk10 == 0) {
            spawn.unk14 = 0xEF;
            spawn.unk15 = 0xE0;
            spawn.unk16 = 0xCD;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0x30;
            spawn.unk19 = 0x30;
            spawn.unk1A = 0x30;
        }
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 50.0f;
            extra.unk8 = func_150ADA68() * 50.0f;
            spawn.unk54 = func_150ADA68() * 0.151000008f + 0.1910000145f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 56U + 200;
            spawn.unkA = func_150ADA20() % 36U + 110;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 370.0f + 403.0f;
            if (sub->unk10 != 0) {
                const u8 colA[3][3] = { { 0xEF, 0xE0, 0xCD }, { 0x9E, 0x9C, 0xA0 }, { 0x9E, 0x9C, 0xA0 } };
                const u8 colB[3][3] = { { 0x30, 0x30, 0x30 }, { 0x9A, 0x69, 0xD2 }, { 0xD5, 0x83, 0x91 } };
                i = func_150ADA20() % 3U;
                spawn.unk14 = colA[i][0];
                spawn.unk15 = colA[i][1];
                spawn.unk16 = colA[i][2];
                spawn.unk17 = 0xFF;
                spawn.unk18 = colB[i][0];
                spawn.unk19 = colB[i][1];
                spawn.unk1A = colB[i][2];
            }
            ret = func_15130280(&spawn, 0, 0, 0x10, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
            }
            sub->accum -= 1.0f;
        } while (sub->accum > 1.0f);
    }
}
