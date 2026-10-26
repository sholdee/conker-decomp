#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1B81D0/func_1518AD20.s")

s32 func_1518B1AC(void *arg0) {
    if (*(f32 *)((u8 *)arg0 + 0x3C) < *(f32 *)((u8 *)arg0 + 0x170)) {
        return 0;
    }
    return 1;
}

s32 func_1518B1D8(void *arg0) {
    f32 temp_f0;
    s32 var_v0;
    s32 var_a1;
    s32 temp_v1;

    temp_f0 = *(f32 *)((u8 *)arg0 + 0x3C) - *(f32 *)((u8 *)arg0 + 0x170);
    if (temp_f0 < 0.0f) {
        return 0;
    }

    var_v0 = *(s16 *)((u8 *)arg0 + 0x64);
    var_v0 <<= 2;
    if (var_v0 >= 0x100) {
        var_v0 = 0xFF;
    }

    temp_v1 = (s32)temp_f0 >> 1;
    var_a1 = var_v0;
    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }

    var_a1 = (temp_v1 < var_v0) ? temp_v1 : var_v0;

    if (var_a1 < 0) {
        return 0;
    }

    *(u8 *)((u8 *)arg0 + 0x70) = var_a1;
    return 1;
}

void func_1518B2A8(f32 *, f32, f32, s32, u16, s32, u8);

void func_1518B264(s32 arg0, f32 arg1, f32 arg2, s32 arg3, u8 arg4) {
    func_1518B2A8(arg0, arg1, arg2, arg3, 4, 0xFF, arg4);
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
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
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    u16 unk56;
    u8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 pad78;
} S1518B2A8;

typedef struct {
    f32 unk00;
    u8 pad04[0x14];
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    s32 unk20;
} S1518B2A8Ground;

extern s32 func_15046C80(f32 *, s32, f32, S1518B2A8Ground *);
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);

void func_1518B2A8(f32 *arg0, f32 arg1, f32 arg2, s32 arg3, u16 arg4, s32 arg5, u8 arg6) {
    S1518B2A8 p;
    S1518B2A8Ground ground;
    u8 angle;
    f32 dist;
    void *v;
    s32 n;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    if (arg0 != NULL) {
        p.unk56 = arg4;
        ground.unk18 = 0;
        ground.unk1C = 0;
        ground.unk1D = 0;
        ground.unk20 = 0;
        p.unk54 = 0x12C;
        p.unk50 = 0x9E8;
        p.unk58 = 0;
        p.unk5C = 0;
        p.unk60 = 0xFF;
        p.unk61 = 1;
        p.unk63 = 0;
        p.unk64 = 0;
        p.unk65 = 0;
        p.unk66 = 0;
        p.unk67 = 0;
        p.unk68 = 0;
        p.unk6A = 2;
        p.unk6C = NULL;
        p.unk70 = 0;
        p.unk72 = 1;
        p.unk74 = 0xFF;
        p.unk1C = 1.0f;
        p.unk20 = 1.0f;
        p.unk24 = 1.0f;
        ground.unk00 = 99999.8984375f;
        p.unk48 = 0.0f;
        p.unk34 = 0.0f;
        p.unk38 = 0.0f;
        p.unk3C = 0.0f;
        p.unk04 = 0.4000000060f;
        p.unk4C = -1.299999952f;
        for (; arg3 > 0; arg3--) {
            angle = func_150ADA20();
            angle &= 0xFF;
            dist = func_150ADA68() * arg1;
            p.unk08 = arg2;
            p.unk0C = arg2;
            p.unk00 = arg2;
            p.unk10 = func_150ADA68() * 360.0f;
            p.unk14 = func_150ADA68() * 360.0f;
            p.unk18 = func_150ADA68() * 360.0f;
            p.unk28 = arg0[0] + func_151423D8(angle) * dist;
            p.unk2C = (arg0[1] + arg1) - (func_150ADA68() * 2.0f * arg1);
            p.unk30 = arg0[2] - func_151423D8(angle - 0x40) * dist;
            p.unk40 = 25.0f - func_150ADA68() * 50.0f;
            p.unk44 = 25.0f - func_150ADA68() * 50.0f;
            if (func_15046C80(&p.unk28, 0, -10000.0f, &ground) != 0) {
                p.unk62 = 2;
                v = func_15132A4C(&p, 3, 0xFF, 4, arg6, 0);
                if (v != NULL) {
                    *(f32 *)((u8 *)v + 0x170) = ground.unk00;
                }
                n = (func_150ADA20() & 3) + 1;
                x = p.unk28;
                y = arg2 * 16.0f + p.unk2C;
                z = p.unk30;
                for (; n > 0; n--) {
                    angle = func_150ADA20();
            angle &= 0xFF;
                    dist = func_150ADA68() * arg2 * 16.0f;
                    scale = func_150ADA68() * 0.3000000119f + 0.1000000015f;
                    p.unk0C = scale;
                    p.unk08 = scale;
                    p.unk00 = scale;
                    p.unk10 = func_150ADA68() * 360.0f;
                    p.unk14 = func_150ADA68() * 360.0f;
                    p.unk18 = func_150ADA68() * 360.0f;
                    p.unk28 = func_151423D8(angle) * dist + x;
                    p.unk2C = func_150ADA68() * 25.0f + y;
                    p.unk30 = z - func_151423D8(angle - 0x40) * dist;
                    p.unk40 = 25.0f - func_150ADA68() * 50.0f;
                    p.unk44 = 25.0f - func_150ADA68() * 50.0f;
                    p.unk62 = 3;
                    v = func_15132A4C(&p, 3, 0xFF, 4, arg6, 0);
                    if (v != NULL) {
                        *(f32 *)((u8 *)v + 0x170) = ground.unk00;
                    }
                }
            }
        }
    }
}
