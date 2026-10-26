#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad_0[0x4];
    u8 field_0x04;
    u8 pad_5[0x11];
    u8 field_0x16;
} EventPayloadFlags;


#pragma GLOBAL_ASM("asm/nonmatchings/game_F15D0/func_150C4120.s")

typedef struct {
    f32 x, y, z;
} Vec3f_C44A4;

typedef struct {
    u8 pad0[0x40];
    Vec3f_C44A4 pos; /* 0x40 */
    u8 pad4C[0x74 - 0x4C];
    s8 unk74;
} Sub28C_C44A4;

typedef struct {
    struct127 *obj;      /* 0x00 */
    u8 id;               /* 0x04 */
    u8 pad5[3];
    f32 unk8;            /* 0x08 */
    Sub28C_C44A4 *unkC;  /* 0x0C */
    f32 unk10;           /* 0x10 */
    s16 unk14;           /* 0x14 */
    u8 unk16;            /* 0x16 */
    u8 pad17;
    s32 unk18;           /* 0x18 */
} Sub28_C44A4;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    Sub28_C44A4 unk28;
} Arg_C44A4;

extern void func_1504715C(void *, struct127 *);
extern s32 func_1514654C(struct127 *, struct126 *, s32, void *, void *, s32);
extern void func_15143134(void *, void *, s32);
extern void func_15143794(s32, s32, f32, f32 *);
extern s32 func_1515C0F8(struct127 *, f32 **);
extern void func_151C36D8(void *, s32, s32);
extern void func_151D9014(void *, void *, s32, f32, s32, s32, f32, s32, f32, f32, s32, void *, s32, s32, s32, s32);
extern Vec3f_C44A4 D_800A0350[];
extern u8 D_800A0374[];
extern Vec3f_C44A4 D_800A0378[];
extern u8 D_800A03A8[];

void func_150C44A4(Arg_C44A4 *arg0) {
    struct127 *obj;
    Sub28_C44A4 *s;
    Vec3f_C44A4 spFC;
    Vec3f_C44A4 spF0;
    f32 f;
    s32 i;
    f32 t;
    u8 spC0[0x24];
    Vec3f_C44A4 *spBC;
    Vec3f_C44A4 *spB8;
    struct126 *v0;
    s32 flag;
    f32 *spAC;
    f32 spA0[3];
    s32 r1;
    s32 r0;
    f32 f20;
    f32 f22;

    obj = arg0->unk28.obj;
    s = &arg0->unk28;
    if ((obj->interaction_state == 0) || (obj->unique_id != s->id)) {
        arg0->unkE = -1;
        return;
    }
    if ((obj->unk1D4 == NULL) || (s->unk16 & 2)) {
        if (s->unkC != NULL) {
            s->unkC->unk74 = 3;
        }
        return;
    }
    func_1504715C(spC0, obj);
    if (s->unk18 == 0) {
        s->unk18 = func_10010154(0x96, obj, 0x36B0, 0x190, 0x4268);
    }
    if (s->unk16 & 1) {
        v0 = func_1503195C(obj, s->unk14, 0);
        if (v0 == NULL) {
            return;
        }
        if (*(s32 *)((u8 *)obj + 0x2EC) != 0) {
            f = *(f32 *)((u8 *)obj + 0x2D8) * *(f32 *)((u8 *)obj + 0x2DC);
        } else {
            f = 0.0f;
        }
        i = f * 4.0f;
        if (i >= 4) {
            i = 3;
        }
        t = (f - i * 0.25f) * 4.0f;
        spF0.x = D_800A0378[i].x * t;
        spF0.y = D_800A0378[i].y * t;
        spF0.z = D_800A0378[i].z * t;
        spBC = &spF0;
        spB8 = &spFC;
        if (func_1514654C(obj, v0, D_800A03A8[i], &spBC, &spB8, 1) == 0) {
            spFC.x = obj->x_position;
            spFC.y = obj->y_position + 800.0f;
            spFC.z = obj->z_position;
        }
    } else {
        if (obj->unk2E8 != 0) {
            f = (f32)obj->unk2E4 / (f32)obj->unk2E8;
        } else {
            f = 1.0f;
        }
        i = f * 3.0f;
        if (i >= 3) {
            i = 2;
        }
        t = (f - i * 0.33333334f) * 3.0f;
        spF0.x = D_800A0350[i].x * t;
        spF0.y = D_800A0350[i].y * t;
        spF0.z = D_800A0350[i].z * t;
        func_15143134(&spF0, &spFC, (D_800A0374[i] << 6) + (s32)obj->unk1D4);
    }
    if (s->unkC != NULL) {
        s->unkC->unk74 = -1;
        s->unkC->pos = spFC;
    }
    flag = 1;
    if ((D_800BE9F0 == 2) && (func_150A29C8(obj - D_800CC2D0, 0x4065) == 0)) {
        flag = 0;
    }
    if (flag) {
        s->unk8 += (0.1290000081f + func_150ADA68() * 0.1170000061f) * D_800BE9A4;
        while (s->unk8 > 1.0f) {
            func_151C36D8(&spFC, arg0->unkC, arg0->unk1);
            s->unk8 -= 1.0f;
        }
    }
    s->unk10 += (0.1300000101f + func_150ADA68() * 0.2f) * D_800BE9A4;
    if (s->unk10 > 1.0f) {
        if (func_1515C0F8(obj, &spAC) == 0) {
            spAC = &D_800A5480;
        }
        do {
            r1 = func_150ADA20();
            r0 = func_150ADA20();
            func_15143794((s16)(r1 & 0xFF), (s16)((r0 % 25U) - 0x40), func_150ADA68() * 2.597000122f + 4.870000362f, spA0);
            spA0[0] -= spAC[0] * 0.6f;
            spA0[2] -= spAC[2] * 0.6f;
            f20 = func_150ADA68();
            r1 = func_150ADA20();
            r0 = func_150ADA20();
            f22 = func_150ADA68();
            func_151D9014(&spFC, spA0, 0xC, f20 * 0.2580000162f + -0.6080000401f, (r1 % 41U) + 60, (r0 % 56U) + 200,
                          f22 * 160.0f + 121.0f, func_150ADA68() < 0.7000000477f, 1.0f, 1.0f, 0, spC0, 1, 1,
                          arg0->unkC, arg0->unk1);
            s->unk10 -= 1.0f;
        } while (s->unk10 > 1.0f);
    }
}

void func_150C4AD8(struct260 *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x34) != 0) {
        func_1516972C(*(struct102 **)((u8 *)arg0 + 0x34));
    }
}

extern void func_150C4AD8(struct260 *);

void func_150C4B08(struct260 *arg0) {
    func_150C4AD8(arg0);
    func_1514933C(arg0);
}

extern void func_15149368(struct260 *);

void func_150C4B34(struct260 *arg0) {
    func_150C4AD8(arg0);
    func_15149368(arg0);
}

void func_150C4B60(s32 arg0, s32 arg1, u8 arg2) {
    s32 *p = &arg0;
    EventPayloadFlags *q;

    if (arg2 == 0x55) {
        q = (EventPayloadFlags *)*p;
        q = (EventPayloadFlags *)((s32)q + 0x28);
        q->field_0x16 = q->field_0x16 & 0xFFFD;
    } else if (arg2 == 0x56) {
        q = (EventPayloadFlags *)*p;
        q = (EventPayloadFlags *)((s32)q + 0x28);
        q->field_0x16 = q->field_0x16 | 2;
    } else {
        q = (EventPayloadFlags *)(*p + 0x28);
        func_15149514(arg1, arg2, (s32)q, (s32)&q->field_0x04, *p);
    }
}
