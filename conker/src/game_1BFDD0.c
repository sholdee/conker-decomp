#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_15192920(void *arg0) {
    struct260 *temp_v0;
    struct {
        void *unk0;
        u8 unk4;
        f32 unk8;
    } sp28;

    if (*(s32 *)&arg0 != 0) {
        sp28.unk0 = arg0;
        sp28.unk4 = *(u8 *)((s32)arg0 + 0x3B);
        sp28.unk8 = 0.0f;

        temp_v0 = func_151491F4(0x23, -1, 0x14, 1, 0x10, 0xC, 0xFF, 1);
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x28), &sp28, 0xC);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1BFDD0/func_151929A4.s")

struct260 *func_15192D48(void *arg0, s16 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        void *unk0;
        u8 unk4;
        f32 unk8;
    } sp28;

    if (arg0 == NULL) {
        return NULL;
    }

    sp28.unk0 = arg0;
    sp28.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    sp28.unk8 = 0.0f;

    temp_v0 = func_151491F4(arg1, -1, 0x18, 1, 0x16, 0xC, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp28, 0xC);
    }
    return temp_v0;
}

typedef struct {
    u8 pad0[0x1D4];
    s32 unk1D4;
} Obj15192DF0;

typedef struct {
    u8 pad0[0x7A];
    u16 unk7A;
    u8 pad7C[0x1D4 - 0x7C];
    s32 unk1D4;
} ObjB15192DF0;

typedef struct {
    ObjB15192DF0 *unk0;
    u8 unk4;
    f32 unk8;
} State15192DF0;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    State15192DF0 unk28;
} Actor15192DF0;

extern void func_15143134(const void *, void *, s32);
extern void func_15143794(s16, s16, f32, void *);
extern struct260 *func_15130374(void *, u8, s32, u8, s32);

void func_15192DF0(Actor15192DF0 *arg0) {
    State15192DF0 *state;
    s16 angle;
    s16 a;
    s32 r;
    s32 pad1;
    struct {
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
        f32 unk30[3];
        f32 unk3C;
        f32 unk40;
        f32 unk44;
        f32 unk48[3];
        f32 unk54;
        s32 unk58;
        u8 pad5C[4];
        u8 unk60;
        u8 unk61;
        u8 unk62;
        s8 unk63;
    } fx;
    s32 pad2;
    f32 val;
    f32 f;
    f32 vec[3];
    struct260 *ret;

    state = &arg0->unk28;
    state->unk8 += (792.0f + (func_150ADA68() * 1202.0f)) * 0.001000000047f * D_800BE9A4;
    if (state->unk8 > 1.0f) {
        if (state->unk0->unk1D4 == 0) {
            while (state->unk8 > 1.0f) {
            }
        } else {
            angle = (state->unk0->unk7A >> 8) + 0x40;
            fx.unk1D = 0x29;
            fx.unk08 = 0xE03;
            fx.unk00 = 0x200005;
            fx.unk18 = 0xB0;
            fx.unk19 = 0xA0;
            fx.unk1A = 0x2A;
            fx.unk14 = 0x40;
            fx.unk15 = 0xB;
            fx.unk16 = 0x6A;
            fx.unk04 = 0;
            fx.unk0C = 0;
            fx.unk10 = 0;
            fx.unk17 = 0xFF;
            fx.unk1C = 0xFF;
            fx.unk60 = 3;
            fx.unk61 = 3;
            fx.unk58 = 0xE05;
            fx.unk62 = 0xA;
            fx.unk63 = -1;
            fx.unk1E = 0x19;
            fx.unk20 = 0xA;
            fx.unk22 = 0x23;
            fx.unk54 = 0.0f;
            fx.unk24 = 1.019999981f;
            val = 0.9708999991f;
            do {
                a = (func_150ADA20() % 51U) - 0x82;
                vec[0] = (func_150ADA68() * 20.0f) + 30.0f;
                vec[1] = (func_150ADA68() * 60.0f) + -20.0f;
                vec[2] = 0.0f;
                if (func_150ADA20() & 1) {
                    vec[0] = -vec[0];
                    a = a + angle;
                } else {
                    a = angle - a;
                }
                fx.unk0A = (func_150ADA20() % 11U) + 0x1E;
                fx.unk1B = (func_150ADA20() % 101U) + 0x64;
                fx.unk28 = fx.unk2C = (func_150ADA68() * 150.0f) + 100.0f;
                func_15143134(vec, fx.unk30, state->unk0->unk1D4 + 0x40);
                r = func_150ADA20();
                func_15143794(a, (r % 28U) - 0x19, func_150ADA68() * 3998.0f * 0.001000000047f, fx.unk48);
                fx.unk58 &= ~0xC0;
                if (func_150ADA20() & 1) {
                    fx.unk58 |= 0x40;
                }
                if (func_150ADA20() & 1) {
                    fx.unk58 |= 0x80;
                }
                ret = func_15130374(&fx, 1, 4, arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &val, 4);
                }
                state->unk8 -= 1.0f;
            } while (state->unk8 > 1.0f);
        }
    }
}

































typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_15193234;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_15193234;

void func_15193234(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_15193234 *temp_v0 = (SubA_15193234 *)((u8 *)arg0 + 0x28);
    ArgB_15193234 *b = (ArgB_15193234 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA_15193234 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_15193234:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_151932E0(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_15193234 *temp_v0 = (SubA_15193234 *)((u8 *)arg0 + 0x28);
    ArgB_15193234 *b = (ArgB_15193234 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA_15193234 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_151932E0:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
