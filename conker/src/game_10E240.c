#include <ultra64.h>
/* functions.h declares func_1509BFB0 void; it ends by tail-dispatching through D_800884D4
 * via jalr and leaves that result in v0. Declaring it s32 matches the caller's regalloc. */
#define func_1509BFB0 func_1509BFB0_hdr
#include "functions.h"
#undef func_1509BFB0
s32 func_1509BFB0(s32 num, s32, s32, ...);
#include "variables.h"

extern u8 D_800A0FE0[];
extern void *func_151149AC(u8);
extern void func_1510F800(s32);


s32 func_1509BE40(s32, s32, s32, s32);

void func_150E0D90(struct108 *arg0) {
    if (arg0->unk23E == 9) {
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 1);
        }
        if ((arg0->unk2C != 0x100) && (arg0->unk6C8 == 0)) {
            if (func_15123934(arg0, 8, 0, 0, 0) != 0) {
                arg0->unk84 |= 0x300000;
                arg0->unk84 &= ~4;
                arg0->unk1B4 = 1;
                arg0->unk1E0 = 3;
                func_15124B18(arg0);
            }
            arg0->unk348 = 125.0f;
            arg0->unk34C = 125.0f;
            arg0->unk374 = 220.0f;
            arg0->unk190 = 30.0f;
        } else {
            arg0->unk190 = 0.0f;
        }
    } else {
        if ((arg0->unk2C == 8) && (arg0->unk6C8 == 0) && (func_151239CC(arg0, 0) != 0)) {
            arg0->unk34C = 110.0f;
            arg0->unk348 = 110.0f;
            arg0->unk374 = 300.0f;
        }
        if (D_800BE9F0 == 0x1E) {
            func_1509BFB0(0, 0x4044, 0);
        }
    }
    if (D_800BE9F0 == 0x2F) {
        arg0->unk84 &= 0x7FFFFFFF;
    } else if (D_800BE9F0 == 0x1E) {
        if (func_1509BE40(1, 0x4042, 6, 0x9000) != 0) {
            arg0->unk84 &= 0x7FFFFFFF;
        } else {
            arg0->unk84 |= 0x80000000;
        }
        if (func_1509BE40(1, 0x4044, 6, 0x2000) != 0) {
            arg0->unk84 |= 0x20001010;
            arg0->unk84 &= ~8;
        } else {
            arg0->unk84 &= 0xDFFFEFEF;
            arg0->unk84 |= 8;
        }
    }
    if (D_800BE9F0 == 0x1B) {
        if (func_1509BE40(1, 0x406E, 6, 0x9000) != 0) {
            arg0->unk84 |= 0x200;
        } else {
            arg0->unk84 &= ~0x200;
        }
        if (func_1509BE40(1, 0x406F, 6, 0x9000) != 0) {
            arg0->unk84 |= 0x80000000;
        } else {
            arg0->unk84 &= 0x7FFFFFFF;
        }
    }
}

struct260 *func_150E1060(u8 arg0, u8 arg1, s32 arg2) {
    struct260 *temp_v0;
    struct {
        u8 unk0;
        u8 pad1;
        u8 pad2;
        u8 pad3;
        void *unk4;
        s32 unk8;
        f32 unkC;
    } sp34;

    if ((s32)arg0 < 0) {
        return NULL;
    }

    if (arg0 >= 2) {
        return NULL;
    }

    sp34.unk0 = arg0;
    sp34.unk4 = func_151149AC(D_800A0FE0[arg0]);
    if (sp34.unk4 == NULL) {
        return NULL;
    }

    func_1510F800(0);
    sp34.unk8 = func_1510FD20(*(s16 *)((s32)sp34.unk4 + 0x10), *(s16 *)((s32)sp34.unk4 + 0x14));
    sp34.unkC = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x1F, -1, 0, 0x1F, (struct37 *)0x10, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x10);
    }
    return temp_v0;
}

typedef struct {
    f32 x, y, z;
} Vec3F150E114C;
typedef struct {
    u8 pad0[2];
    s16 unk2;
} Obj150E114C;
typedef struct {
    u8 unk0;
    s32 unk4;
    Obj150E114C *unk8;
    f32 unkC;
} State150E114C;
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    State150E114C unk28;
} Actor150E114C;
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
    f32 unk30[3];
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 pad66[2];
} Effect150E114C;
extern Vec3F150E114C D_800A0FE4[];
extern Vec3F150E114C D_800A0FFC[];
extern struct260 *func_15130374(void *, u8, s32, u8, s32);
extern void func_15145CD0(s32, Vec3F150E114C **, f32 **, s32);

void func_150E114C(Actor150E114C *arg0) {
    State150E114C *state;
    s32 i;
    s16 mask;
    s32 pad[2]; /* frame: two unused words above fx */
    Effect150E114C fx;
    f32 val;
    f32 vec[3];
    Vec3F150E114C *in[2];
    f32 *out[2];
    s32 hi;
    s32 lo;
    f32 scale;
    struct260 *ret;

    state = &arg0->unk28;
    state->unkC += (204.0f + (func_150ADA68() * 159.0f)) * 0.001000000047f * D_800BE9A4;
    if (state->unkC > 1.0f) {
        i = 0;
        mask = 0;
        for (; i <= D_80082FA0; i++) {
            mask |= 1 << i;
        }
        if (!(state->unk8->unk2 & mask)) {
            do {
                state->unkC -= 1.0f;
            } while (state->unkC > 1.0f);
            return;
        }
        {
            f32 *d;
            fx.unk1D = 0x29;
            fx.unk08 = 0x4403;
            fx.unk00 = 0x200005;
            fx.unk1E = 0x21;
            fx.unk20 = 7;
            fx.unk58 = 0x1CE05;
            fx.unk60 = 6;
            fx.unk61 = 8;
            fx.unk62 = 0x10;
            fx.unk63 = -1;
            fx.unk04 = 0;
            fx.unk0C = 0;
            fx.unk10 = 0;
            fx.unk54 = 0.0f;
            fx.unk64 = -1;
            fx.unk65 = 0;
            fx.unk22 = 0x27;
            fx.unk14 = 0xDD;
            fx.unk15 = 0xD3;
            fx.unk16 = 0xCD;
            fx.unk17 = 0xFF;
            fx.unk18 = 0x57;
            fx.unk19 = 0x55;
            fx.unk1A = 0x5A;
            fx.unk1C = 0xFF;
            val = 0.962338984f;
            fx.unk24 = 1.011461973f;
            in[0] = &D_800A0FE4[state->unk0];
            in[1] = &D_800A0FFC[state->unk0];
            out[0] = fx.unk30;
            out[1] = vec;
            func_15145CD0(state->unk4, in, out, 2);
            d = vec;
            d[0] -= fx.unk30[0];
            d[1] -= fx.unk30[1];
            d[2] -= fx.unk30[2];
        }
        do {
            fx.unk58 &= ~0xC0;
            hi = (func_150ADA20() & 1) ? 0x80 : 0;
            lo = (func_150ADA20() & 1) ? 0x40 : 0;
            fx.unk58 |= lo | hi;
            fx.unk3C = 0.0f;
            fx.unk40 = 0.0f;
            fx.unk44 = 0.0f;
            fx.unk1B = 0xFF;
            fx.unk0A = (func_150ADA20() % 17U) + 50;
            fx.unk28 = fx.unk2C = (func_150ADA68() * 165.0f) + 357.0f;
            scale = ((func_150ADA68() * 195.0f) + 356.0f) * 0.001000000047f;
            fx.unk48 = vec[0] * scale;
            fx.unk4C = vec[1] * scale;
            fx.unk50 = vec[2] * scale;
            ret = func_15130374(&fx, 1, 4, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &val, 4);
            }
            state->unkC -= 1.0f;
        } while (state->unkC > 1.0f);
    }
}
