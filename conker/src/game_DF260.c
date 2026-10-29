#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_DF260/func_150B1DB0.s")

void func_150B1E20(struct108 *arg0) {
    struct127 *temp;

    if (func_1509BE40(1, 0x4054, 6, 0x9000)) {
        arg0->unk84 |= 0x10000;
    } else {
        arg0->unk84 &= ~0x10000;
    }
    if (!func_1509BE40(1, 0x405C, 6, 0x9000)) {
        if (func_1509BE40(1, 0x405B, 6, 0x2000) && (arg0->unk2C != 0x40)) {
            temp = arg0->unk3D0;
            *(f32 *)((u8 *)temp + 0x17C) = temp->unk180 + 10.0f;
        }
    }
}

struct df260a { s32 unk0[3]; };

void func_150B1EE0(struct127 *arg0, struct df260a *arg1, s32 arg2, s32 arg3) {
    struct {
        struct df260a unk0;
        f32 unkC;
    } sp38;
    struct260 *temp_v0;

    sp38.unk0 = *arg1;
    sp38.unkC = 0.0f;

    temp_v0 = func_15149130((func_150ADA20() % 0x85U) + 0x45, -1, 0x49, -1, 1, 0, (struct37 *)0x10, *(u8 *)((s32)arg0 + 0xC), *(u8 *)((s32)arg0 + 0x1));
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp38, 0x10);
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
    s16 unk68;
    f32 unk6C;
} SpawnDF260;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} ExtraDF260;

typedef struct {
    struct17 pos;
    f32 timer;
} TimerDF260;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150B1F90(u8 *arg0) {
    TimerDF260 *t;

    t = (TimerDF260 *)(arg0 + 0x28);
    t->timer += (0.07859999686f + func_150ADA68() * 0.03460000083f) * D_800BE9A4;
    if (t->timer > 1.0f) {
        SpawnDF260 spawn;
        ExtraDF260 extra;
        void *temp_v0;

        extra.unkC = 0.9794009924f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x50;
        spawn.unk20 = 3;
        spawn.unk58 = 0x90DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x50;
        spawn.unk14 = 0x67;
        spawn.unk15 = 0x6C;
        spawn.unk16 = 0x6C;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x16;
        spawn.unk19 = 0xB;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.016479015f;
        spawn.unk30 = t->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        do {
            extra.unk2 = (func_150ADA20() % 5U) + 4;
            extra.unk3 = (func_150ADA20() % 5U) + 4;
            extra.unk4 = func_150ADA68() * 27.0f;
            extra.unk8 = func_150ADA68() * 27.0f;
            spawn.unk54 = func_150ADA68() * 0.2090000063f + 0.1350000054f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = (func_150ADA20() % 101U) + 100;
            spawn.unkA = (func_150ADA20() % 78U) + 0x7E;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 503.0f + 405.0f;
            temp_v0 = func_15130280(&spawn, 1, 0, 0x10, arg0[0xC], arg0[1]);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0xA8, &extra, 0x10);
            }
            t->timer -= 1.0f;
        } while (t->timer > 1.0f);
    }
}

void func_150B2340(struct108 *arg0) {
    s32 temp1;
    s32 temp2;

    temp1 = func_1509BE40(0, func_1509BE40(0, 0x2007, 0xB7) | 0x2000, 0xBC);
    temp2 = func_1509BE40(0, 0x2000, 0xBB);

    if (func_1509BE40(0, 0x5043, 0x1A) != 0) {
        if (func_1509BE40(0, 0x5045, 0x1A) != 0) {
            arg0->unk84 |= 0x4000;
        }
        arg0->unk84 |= 0x400000;
        arg0->unk190 = 0.0f;
    } else {
        if (func_1509BE40(1, 0x4030, 6, 0x2000) != 0) {
            arg0->unk190 = 235.0f;
        } else {
            arg0->unk348 = 114.0f;
            arg0->unk34C = 114.0f;
            arg0->unk374 = 384.0f;
            arg0->unk190 = 60.0f;
        }
    }

    if ((temp1 != 0) && (temp2 != -1)) {
        arg0->unk5F0 |= 0x100;
    } else {
        arg0->unk5F0 &= ~0x100;
    }
}
