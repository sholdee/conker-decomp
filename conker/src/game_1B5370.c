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


#define FADES_ ((FadeB *)D_800DF700)
typedef struct FadeB {
    u8 cur[3];
    u8 end[3];
    u8 p6[3];
    u8 pad[3];
    f32 val;
    s32 type;
    f32 rate;
    s32 pad18;
    union { s32 i; f32 f; } st;
    union { s32 i; f32 f; } t;
} FadeB;
extern u8 D_800A7390[];
extern f32 sinf(f32);
extern s32 func_151EF610(void);
void func_151880C0(void) {
    s32 i;
    for (i = 0; i < D_800DF7B4; i++) {
        switch (FADES_[i].type) {
        case 0:
            break;
        case 1:
            FADES_[i].val = (sinf(FADES_[i].st.f) + 1.0f) * 0.5f;
            FADES_[i].st.f = FADES_[i].rate * D_800BE9A4 + FADES_[i].st.f;
            if (FADES_[i].st.f >= 6.283185482f) {
                FADES_[i].st.f = FADES_[i].st.f - 6.283185482f;
            }
            break;
        case 2:
            if (FADES_[i].t.i == 0) {
                FADES_[i].t.i = -8;
                FADES_[i].val = 1.0f;
            } else if (FADES_[i].t.i < 0) {
                FADES_[i].t.i += D_800BE9E4;
                if (FADES_[i].t.i >= 0) {
                    FADES_[i].val = 0.0f;
                    FADES_[i].st.i++;
                    if ((u32)FADES_[i].st.i >= 8) {
                        FADES_[i].st.i = 0;
                    }
                    FADES_[i].t.i = D_800A7390[FADES_[i].st.i];
                }
            } else {
                FADES_[i].t.i -= D_800BE9E4;
                if (FADES_[i].t.i < 0) {
                    FADES_[i].t.i = 0;
                }
            }
            break;
        case 3:
            switch (FADES_[i].st.i) {
            case 0:
                FADES_[i].val += FADES_[i].rate * D_800BE9E4;
                if (FADES_[i].t.f <= FADES_[i].val) {
                    FADES_[i].val = FADES_[i].t.f;
                    if (FADES_[i].t.f == 1.0f) {
                        if (!(func_151EF610() & 1)) {
                            FADES_[i].t.f = 0.0f;
                        } else {
                            FADES_[i].t.f = 0.5f;
                        }
                        FADES_[i].st.i = 2;
                    } else {
                        FADES_[i].st.i = 1;
                        FADES_[i].t.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 1:
                FADES_[i].t.i -= D_800BE9E4;
                if (FADES_[i].t.i <= 0) {
                    FADES_[i].st.i = (func_151EF610() % 2) * 2;
                    if (FADES_[i].st.i == 0) {
                        FADES_[i].t.f = 1.0f;
                    } else {
                        FADES_[i].t.f = 0.0f;
                    }
                }
                break;
            case 2:
                FADES_[i].val -= FADES_[i].rate * D_800BE9E4;
                if (FADES_[i].val <= FADES_[i].t.f) {
                    FADES_[i].val = FADES_[i].t.f;
                    if (FADES_[i].t.f == 0.0f) {
                        FADES_[i].st.i = 3;
                        FADES_[i].t.i = func_151EF610() % 30 + 30;
                    } else {
                        FADES_[i].st.i = 1;
                        FADES_[i].t.i = func_151EF610() % 30 + 30;
                    }
                }
                break;
            case 3:
                FADES_[i].t.i -= D_800BE9E4;
                if (FADES_[i].t.i <= 0) {
                    FADES_[i].st.i = 0;
                    if (!(func_151EF610() & 1)) {
                        FADES_[i].t.f = 0.5f;
                    } else {
                        FADES_[i].t.f = 1.0f;
                    }
                }
                break;
            }
            break;
        }
        FADES_[i].p6[0] = FADES_[i].cur[0] + (FADES_[i].end[0] - FADES_[i].cur[0]) * FADES_[i].val;
        FADES_[i].p6[1] = FADES_[i].cur[1] + (FADES_[i].end[1] - FADES_[i].cur[1]) * FADES_[i].val;
        FADES_[i].p6[2] = FADES_[i].cur[2] + (FADES_[i].end[2] - FADES_[i].cur[2]) * FADES_[i].val;
    }
}

