#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_800DF700[0xB4];
extern s32 D_800DF7B4;

typedef struct Game1B5370Fade {
    u8 color[6];
    u8 start[3];
    u8 pad9[7];
    s32 type;
    f32 rate;
    u8 work[0xC];
} Game1B5370Fade;

#define FADES ((Game1B5370Fade *)D_800DF700)

s32 func_15187EC0(s32 arg0, f32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7) {
    if (D_800DF7B4 < 5) {
        FADES[D_800DF7B4].type = arg0;
        FADES[D_800DF7B4].rate = arg1;
        FADES[D_800DF7B4].color[0] = FADES[D_800DF7B4].start[0] = arg2;
        FADES[D_800DF7B4].color[1] = FADES[D_800DF7B4].start[1] = arg3;
        FADES[D_800DF7B4].color[2] = FADES[D_800DF7B4].start[2] = arg4;
        FADES[D_800DF7B4].color[3] = arg5;
        FADES[D_800DF7B4].color[4] = arg6;
        FADES[D_800DF7B4].color[5] = arg7;
        bzero(FADES[D_800DF7B4].work, 0xC);
        D_800DF7B4++;
        return D_800DF7B4 - 1;
    }
    return -1;
}

extern u8 D_800DF700[0xB4];
extern s32 D_800DF7B4;

void func_15187F90(void) {
    bzero(D_800DF700, 0xB4);
    D_800DF7B4 = 0;
}

struct S_DF700b { u8 pad[6]; u8 unk6; u8 unk7; u8 unk8; u8 pad2[0x1B]; };

void func_15187FC0(s32 arg0, s32 *arg1) {
    struct S_DF700b *p = (struct S_DF700b *)D_800DF700;
    if (arg0 < D_800DF7B4 && arg0 >= 0) {
        arg1[0] = p[arg0].unk6;
        arg1[1] = p[arg0].unk7;
        arg1[2] = p[arg0].unk8;
    }
}

struct S_DF700 { f32 unkC; s32 pad[8]; };
extern struct S_DF700 D_800DF70C[];

void func_15188010(s32 arg0, f32 *arg1) {
    if (arg0 < D_800DF7B4 && arg0 >= 0) {
        *arg1 = D_800DF70C[arg0].unkC;
    }
}

void func_1518804C(s32 arg0, f32 arg1) {
    if (arg0 < D_800DF7B4 && arg0 >= 0) {
        if (arg1 > 1.0f) {
            arg1 = 1.0f;
        } else if (arg1 < 0.0f) {
            arg1 = 0.0f;
        }
        D_800DF70C[arg0].unkC = arg1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B5370/func_151880C0.s")
