#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#include "variables.h"


extern f32 func_15048A40(u8 arg0);

/* An object func_150C4D20 swings like a pendulum about angle 0. */
typedef struct {
    /* 0x00 */ f32 angle; // degrees, wrapped into [0, 360)
    /* 0x04 */ u8  pad4[0xC];
    /* 0x10 */ s16 position[3];
    /* 0x16 */ u8  pad16[0x39];
    /* 0x4F */ u8  flags;
    /* 0x50 */ u8  pad50[0x10];
    /* 0x60 */ f32 angularVelocity; // degrees per frame
    /* 0x64 */ u8  pad64[0x18];
    /* 0x7C */ s32 phase; // drives the push of func_15048A40; plays sound 0xF each time it wraps 256
} Pendulum;

/* Swings the pendulum: a spring pulls it back toward angle 0, a periodic push from func_15048A40(phase)
 * (plus a constant one while flags bit 2 is set) drives it, and 20% damping is applied every frame. */
void func_150C4D20(Pendulum *pendulum) {
    f32 velocity;
    f32 angle;
    f32 push;
    s32 phase;

    angle = pendulum->angle;
    velocity = pendulum->angularVelocity;
    if (angle > 180.0f) {
        angle -= 360.0f;
    }
    phase = pendulum->phase;
    phase += D_800BE9E4;
    velocity -= angle * 0.009f;
    if (phase >= 256.0f) {
        phase -= 256.0f;
        func_10010F88(0xF, 0x55F0, 0, 0, 0, pendulum->position[0], pendulum->position[1], pendulum->position[2],
                      500, 1000);
    }
    pendulum->phase = phase;
    push = func_15048A40(phase);
    velocity += push * 0.1f;
    if ((pendulum->flags & 4) == 4) {
        velocity += 0.1f;
    }
    velocity *= 0.8f;
    pendulum->angularVelocity = velocity;
    pendulum->angle += velocity;
    if (pendulum->angle < 0.0f) {
        pendulum->angle += 360.0f;
    } else if (pendulum->angle >= 360.0f) {
        pendulum->angle -= 360.0f;
    }
}

extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1511A410(s32, void *);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(void *, s32, s32, s32, s32);
extern u8 D_2C1;
extern u8 D_1C0;

typedef struct {
    u8 pad0[0xA];
    s16 unkA;
    u8 padC[4];
} Struct150C4E9CKey;

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 pad6[2];
    f32 unk8;
    f32 unkC;
} Struct150C4E9CState;

typedef struct {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[4];
    s32 unk1C;
    Struct150C4E9CKey *unk20[2];
    Struct150C4E9CKey *unk28;
    u8 pad2C[0x47];
    u8 unk73;
    u8 pad74[8];
    Struct150C4E9CState *unk7C;
} Struct150C4E9C;

void func_150C4E9C(Struct150C4E9C *arg0) {
    s32 anim;
    s32 sp50;
    s32 r;
    s32 x;
    Struct150C4E9CState *p;
    s32 mode;
    s32 i;

    mode = arg0->unk73 & 3;
    if (arg0->unk7C == NULL) {
        p = arg0->unk7C = allocate_memory(0x10, 1, 0, 0);
        func_1511A410(arg0->unk1C, p);
        p->unk4 = arg0->unk28->unkA;
        p->unk8 = 0.0f;
        p->unkC = 0.0f;
    } else {
        p = arg0->unk7C;
    }
    if (mode == 0 || mode == 2) {
        anim = (s32)&D_2C1;
    } else {
        anim = (s32)&D_1C0;
    }
    if (mode == 0 || mode == 3) {
        p->unk8 = 0.0f;
        p->unkC = 0.0f;
    } else if (mode == 2) {
        if (p->unk8 < 300.0f) {
            p->unk8 += 25.0f * D_800BE9E4;
        } else {
            p->unk8 = 300.0f;
        }
    } else if (mode == 1) {
        s32 a;
        s32 b;

        if (100.0f < p->unk8) {
            p->unk8 -= 25.0f * D_800BE9E4;
        } else {
            p->unk8 = 100.0f;
            a = arg0->unk20[D_800BE9C0]->unkA % 0x400;
            while (a < 0) {
                a += 0x400;
            }
            b = p->unk4 % 0x400;
            while (b < 0) {
                b += 0x400;
            }
            if (a >= b && (a - p->unk8 <= b)) {
                p->unk8 = a - b;
                arg0->unk73 &= ~3;
                arg0->unk73 |= 3;
            }
        }
    }
    if (p->unk8) {
        for (i = 0; i < arg0->unk16; i++) {
            arg0->unk20[D_800BE9C0][i].unkA -= p->unk8;
        }
    }
    if (arg0->unk20[D_800BE9C0]->unkA < -0x2800) {
        for (i = 0; i < arg0->unk16; i++) {
            arg0->unk20[D_800BE9C0][i].unkA += 0x2800;
        }
    }
    x = 0;
    r = func_1510D0EC(anim, &sp50, 3, 0);
    if (p->unk0 != 0) {
        x = sp50 + r - 0x200;
    }
    func_1510D874(arg0, r, x, 4, 5);
}

extern s32 *D_800D98D0[];

void func_150C522C(void) {
    s32 i;
    s32 v;

    i = 0;
    do {
        v = (s32)D_800D98D0[i];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
        i++;
        D_800D98D0[i - 1] = 0;
    } while ((s32 **)&D_800D98E0 != &D_800D98D0[i]);
}

extern u8 D_800C35E8;

s32 func_150C5280(void) {
    u8 v0;
    if (D_800C35EA == 1) {
        v0 = D_800C35E8;
        if (v0 == 0xB || v0 == 0xC || v0 == 0xD) {
            return 1;
        }
    }
    return 0;
}

extern s32 func_150C5280(void);
extern s32 (*D_8008ADA8)(s32);

s32 func_150C52CC(s32 arg0) {
    if (func_150C5280()) {
        return 0;
    }
    return D_8008ADA8(arg0);
}

s32 func_150C5310(s32 *arg0) {
    if (func_150C5280()) {
        arg0[0x18] |= 0x20000;
    } else {
        arg0[0x18] &= ~0x20000;
    }
    return 1;
}
