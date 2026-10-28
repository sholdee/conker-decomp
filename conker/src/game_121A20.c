#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_121A20/func_150F4570.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_121A20/func_150F48D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_121A20/func_150F4A38.s")

struct sub150F4 { u8 pad[0x24]; u8 unk24; };

void func_150F4CFC(struct102 *arg0, s32 arg1, u8 arg2) {
    struct sub150F4 *p;
    p = (struct sub150F4 *)((u8 *)arg0 + 0x170);
    if (arg2 == 0x4E) {
        ((u8 *)arg0)[0x71] = 0;
        p->unk24 |= 0x5;
    } else if (arg2 == 0x4F) {
        func_1516972C(arg0);
    }
}

void func_150F4D5C(void *arg0, s8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct {
        void *unk0;
        f32 unk4;
        u8 unk8;
        u8 unk9;
        u8 padA;
        u8 padB;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = 0.0f;
    sp34.unk8 = arg1;
    sp34.unk9 = arg2;

    temp_v0 = func_15149130(0x12C, -1, 0x56, -1, 0, 0, (struct37 *)0xC, arg3, arg4);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

extern struct17 *func_15144B34(s32);
void func_150F4570(struct17 *arg0, struct17 *arg1, u8 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);

typedef struct {
    u8 count;
    s16 *ids;
    f32 *scales;
} Rec150F4;

typedef struct {
    u8 pad0[0x23D];
    u8 unk23D;
    u8 pad23E[0x142];
    f32 unk380;
} Obj150F4;

typedef struct {
    Obj150F4 *unk0;
    f32 unk4;
    s8 unk8;
    u8 unk9;
} Emit150F4;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Emit150F4 unk28;
} Emitter150F4;

extern Rec150F4 D_80088B20[];
extern s32 (*D_80088B38[])();
extern void func_15143874(s16, f32, f32 *, f32 *);

void func_150F4DEC(Emitter150F4 *arg0) {
    Emit150F4 *e;
    struct17 *pos;
    s32 angle;
    struct17 a;
    struct17 b;
    Rec150F4 *rec;
    u8 i;
    f32 speed;

    e = &arg0->unk28;
    if (e->unk8 != -1) {
        if (!D_80088B38[e->unk8]()) {
            return;
        }
    }
    e->unk4 += (0.05100000277f + func_150ADA68() * 0.153000012f) * D_800BE9A4;
    if (1.0f < e->unk4) {
        pos = func_15144B34(e->unk0->unk23D);
        angle = (e->unk0->unk380 + 180.0f) * 0.7111111283f;
        do {
            func_15143874(angle + 0x15, func_150ADA68() * 381.0f + 254.0f, &a.unk0, &a.unk8);
            a.unk4 = func_150ADA68() * 212.0f + -133.0f;
            a.unk0 += pos->unk0;
            a.unk4 += pos->unk4;
            a.unk8 += pos->unk8;
            func_15143874(angle - 0x15, func_150ADA68() * 381.0f + 254.0f, &b.unk0, &b.unk8);
            b.unk4 = func_150ADA68() * 212.0f + -133.0f;
            b.unk0 += pos->unk0;
            b.unk4 += pos->unk4;
            b.unk8 += pos->unk8;
            if (func_150ADA20() & 1) {
                struct17 tmp;

                tmp = a;
                a = b;
                b = tmp;
            }
            rec = &D_80088B20[e->unk9];
            i = func_150ADA20() % rec->count;
            speed = func_150ADA68() * 10.0f + 5.0f;
            func_150F4570(&a, &b, rec->ids[i], speed, 1.0f / speed,
                          rec->scales[i] * (func_150ADA68() * 0.07500000298f + 0.07500000298f), 0, 0, arg0->unkC,
                          arg0->unk1);
            e->unk4 -= 1.0f;
        } while (1.0f < e->unk4);
    }
}
