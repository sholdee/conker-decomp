#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad_0[0x4];
    u8 field_0x04;
    u8 pad_5[0x11];
    u8 field_0x16;
} EventPayloadFlags;


typedef struct {
    void *unk0;
    u8 unk4;
    f32 unk8;
    s32 unkC;
    f32 unk10;
    s16 unk14;
    s8 unk16;
    s32 unk18;
} Data150C4120;

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
} Spawn150C4120;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} Extra150C4120;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150C4120(u8 *arg0, s16 arg1, u8 arg2, s32 arg3) {
    Data150C4120 data;
    struct260 *temp_v0;
    void *ret;
    Spawn150C4120 spawn;
    Extra150C4120 extra;

    data.unk0 = arg0;
    data.unk4 = arg0[0x3B];
    data.unk8 = 0.0f;
    data.unkC = 0;
    data.unk10 = 0.0f;
    data.unk14 = -1;
    data.unk16 = 0;
    data.unk18 = 0;
    if (arg0[4] == 0x34) {
        data.unk14 = 0x54;
        data.unk16 = 1;
    }
    temp_v0 = func_15149130((arg1 == -1) ? 0x12C : arg1, -1, 0x53, -1, (arg1 == -1) ? 0 : 1, 0x40, (struct37 *)0x1C, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &data, 0x1C);
        arg0 = (u8 *)((s32)temp_v0 + 0x28);
        {
        const s32 tbl[4] = { 0x60, 0x61, 0x62, 0x63 };

        extra.unk0 = func_150ADA20();
        extra.unk1 = (s8)func_150ADA20();
        extra.unk4 = spawn.unk28 = spawn.unk2C = 700.0f;
        extra.unk2 = (s8)(func_150ADA20() % 6U) + 5;
        extra.unk3 = func_150ADA20() % 6U + 5;
        extra.unk8 = (func_150ADA68() * 0.25f + 0.200000003f) * extra.unk4;
        extra.unkC = (func_150ADA68() * 0.25f + 0.200000003f) * extra.unk4;
        spawn.unk1D = *(tbl + (func_150ADA20() & 3));
        spawn.unk8 = 0x2203;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk14 = 0;
        spawn.unk15 = 0;
        spawn.unk16 = 0;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0;
        spawn.unk19 = 0;
        spawn.unk1A = 0;
        spawn.unk1B = 0xFF;
        spawn.unk1C = 0xFF;
        spawn.unk30 = *(struct17 *)&D_800A5480;
        spawn.unk3C = *(struct17 *)&D_800A5480;
        spawn.unk48 = *(struct17 *)&D_800A5480;
        spawn.unk22 = 1;
        spawn.unk54 = 0.0f;
        spawn.unk24 = 1.0f;
        spawn.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000;
        spawn.unkA = 0x12C;
        spawn.unk6C = 10000.0f;
        spawn.unk1E = 1;
        spawn.unk20 = 0xFF;
        spawn.unk60 = 6;
        spawn.unk61 = 8;
        spawn.unk64 = -1;
        spawn.unk62 = 0x26;
        spawn.unk63 = -1;
        spawn.unk65 = 0;
        spawn.unk5C = 0;
        spawn.unk66 = 0xFF;
        spawn.unk68 = 0;
        ((Data150C4120 *)arg0)->unkC = (s32)func_15130280(&spawn, 1, 0, 0x10, arg2, arg3);
        if (((Data150C4120 *)arg0)->unkC != 0) {
            memcpy((u8 *)((Data150C4120 *)arg0)->unkC + 0xA8, &extra, sizeof(extra));
        }
        }
    }
}

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
