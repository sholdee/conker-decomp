#include <ultra64.h>
#define func_15169260 func_15169260_s32
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_15169260
#undef func_1513D524
#include "variables.h"

void *memcpy(void *, const void *, u32);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15169260(void *, s32, s32, u8);
extern f32 D_800A8018;
extern void func_151D5D60();
extern void func_1513F680();
extern void func_15143134(void *, void *, void *);
extern void *func_1513D524(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_15191B8C(s32, u8);
extern u8 func_151D8E20(void);

struct Struct15190770Local {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x3];
};

void *func_15190770(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_s0;
    struct Struct15190770Local sp2C;
    void *sp38;

    sp2C.unk0 = *(s32 *)arg0;
    sp2C.unk4 = *(u8 *)(*(s32 *)arg0 + 0x3B);
    sp38 = arg0;
    func_15191B8C((s32)&sp2C, 0xE);
    temp_s0 = func_15167A68(0x20, arg3, arg1 + 0x300, 1, (u8)arg2, 1);
    if (temp_s0 == 0) {
        return NULL;
    }
    memcpy((s32)temp_s0 + 0x10, sp38, 0xC);
    *(void **)((s32)temp_s0 + 0x24) = (void *)((s32)temp_s0 + 0x30);
    *(void **)((s32)temp_s0 + 0x28) = (void *)((s32)temp_s0 + 0x300);
    *(s32 *)((s32)temp_s0 + 0x20) = 0;
    *(f32 *)((s32)temp_s0 + 0x1C) = 0.0f;
    bzero((s32)temp_s0 + 0x30, 0x2D0);
    *(u8 *)((s32)temp_s0 + 0x2C) = func_151D8E20();
    return temp_s0;
}

struct Struct15191400Arg;
extern void func_15191400(struct Struct15191400Arg *);

void func_15190840(void *arg0) {
    func_15191400(arg0);
    func_15169804(arg0);
}

void func_1519086C(void *arg0) {
    func_15191400(arg0);
    func_15169824(arg0);
}

typedef struct F898Owner {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
    u8 pad11C[0xB8];
    u8 (*unk1D4)[0x40];
} F898Owner;

typedef struct F898V2 {
    s32 a, b;
} F898V2;

typedef struct F898V3 {
    s32 a, b, c;
} F898V3;

typedef struct F898Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 pad25[3];
    s32 unk28[2];
    s32 unk30[2];
    u8 unk38[0x24];
    u8 unk5C;
    u8 pad5D[3];
} F898Sub;

typedef struct F898Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x16];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xC];
    u32 unk58;
    u8 pad5C[0xB4];
    F898Sub sub;
} F898Obj;

typedef struct F898Slot {
    F898Obj *obj;
    s32 timer;
    struct F898Slot *next;
} F898Slot;

typedef struct F898Sk {
    u8 pad0[0xC];
    u8 unkC;
    u8 padD[3];
} F898Sk;

typedef struct F898Entry {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    F898Sk *unk8;
    f32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
} F898Entry;

struct Struct15191400Range {
    s16 unk0;
    s16 unk2;
    s32 unk4[2];
    s32 unkC[2];
};
typedef struct Struct15191400Range F898Range;
extern struct Struct15191400Range D_800A9D40[];

typedef struct F898Arg {
    u8 pad0[0x10];
    F898Owner *unk10;
    u8 unk14;
    u8 pad15;
    s16 unk16;
    u8 unk18;
    u8 pad19;
    u8 unk1A;
    u8 pad1B;
    f32 unk1C;
    s32 unk20;
    F898Slot *unk24;
    u8 pad28[4];
    u8 unk2C;
} F898Arg;

typedef struct F898Spawn {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C[3];
    f32 unk28[3];
    f32 unk34[3];
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 pad46[2];
} F898Spawn;

extern F898Entry D_8008FA70[];
extern u8 D_800AB310[];
extern u8 D_800AB404[];
extern f32 D_800A8014;
extern void func_1504715C(void *, void *);

void func_15190898(F898Arg *arg0) {
    F898Arg *self;
    volatile u8 done;
    F898Entry *ent;
    F898Slot *slot;
    F898Slot *head;
    F898Slot *tail;
    s32 count;
    s32 i;
    s32 keep;
    F898Obj *o;
    F898Sub *sub;
    F898Range *rng;
    f32 dx;
    f32 dz;
    s32 idx;
    s32 n;
    F898Obj *res;
    s32 kz;
    s32 flag;
    F898Spawn b;
    F898Sub a;
    s16 va0;
    u8 va3;
    s32 pad5;

    self = arg0;
    done = 0;
    if (arg0->unk18 & 1) {
        arg0->unk16 = arg0->unk16 - D_800BE9E4;
        if (arg0->unk16 < 0) {
            done = 1;
        }
    }
    if (done == 0 && (arg0->unk10->unk0 == 0 || arg0->unk10->unk3B != arg0->unk14)) {
        done = 1;
    }
    if (done == 0) {
        if (arg0->unk10->unk1D4 != NULL) {
            ent = &D_8008FA70[arg0->unk1A];
            head = NULL;
            tail = NULL;
            count = 0;
            i = 0;
            slot = arg0->unk24;
            for (; i < ent->unk4; i++, slot++) {
                if (slot->obj != NULL) {
                    *(F898V3 *)&slot->obj->unk40 = *(F898V3 *)&slot->obj->unk34;
                    func_15143134(ent->unk8 + i, &slot->obj->unk34, arg0->unk10->unk1D4 + ent->unk8[i].unkC);
                    slot->timer = slot->timer - D_800BE9E4;
                    if (slot->timer < 0) {
                        F898Obj *o2 = slot->obj;
                        F898Sub *sub2 = &slot->obj->sub;

                        rng = &D_800A9D40[sub2->unk20];
                        dx = (o2->unk34 - o2->unk40) * D_800BE9A8;
                        dz = (o2->unk3C - o2->unk48) * D_800BE9A8;
                        sub2->unk4 = 0.0f;
                        sub2->unk0 = dx;
                        sub2->unk8 = dz;
                        func_1513F680(slot->obj, 0xD, 0x12, 0, 0xD);
                        keep = 1;
                        slot->obj->unk1C = func_150ADA20() % (u32)(rng->unk2 + 1) + rng->unk0;
                        slot->obj->unk58 |= 1;
                        slot->obj = NULL;
                        arg0->unk20 = arg0->unk20 - 1;
                    } else {
                        keep = 0;
                    }
                } else {
                    keep = 1;
                }
                if (keep != 0) {
                    count += 1;
                    if (tail == NULL) {
                        tail = slot;
                    }
                    slot->next = head;
                    head = slot;
                }
            }
            if (count != 0) {
                arg0->unk1C = arg0->unk1C + ent->unkC * D_800BE9A4;
                if (arg0->unk1C > 1.0f) {
                    F898Range *rng2;

                    rng2 = &D_800A9D40[ent->unk0];
                    tail->next = head;
                    a.unk0 = 0.0f;
                    a.unk4 = 0.0f;
                    a.unk8 = 0.0f;
                    a.unk10 = 0.2f;
                    a.unk14 = 0xF;
                    a.unk16 = 0x11;
                    a.unk18 = 0.0f;
                    a.unk1C = 0.2f;
                    a.unkC = -1.0f;
                    a.unk20 = ent->unk0;
                    func_1504715C(a.unk38, arg0->unk10);
                    a.unk5C = arg0->unk2C;
                    b.unk0 = D_800AB310[arg0->unk2C];
                    b.unk2 = 0x1001;
                    b.unk4 = 0x12C;
                    b.unk8 = 0;
                    b.unkC = 0;
                    b.unk10 = 0;
                    b.unk11 = 0;
                    b.unk12 = 0;
                    b.unk13 = 0xFF;
                    b.unk28[0] = 0.0f;
                    b.unk28[1] = 0.0f;
                    b.unk28[2] = 0.0f;
                    b.unk40 = 0;
                    b.unk45 = 0xFF;
                    b.unk34[0] = 1.0f;
                    b.unk34[1] = 1.0f;
                    b.unk34[2] = 1.0f;
                    a.unk21 = 0;
                    a.unk22 = 0;
                    do {
                        idx = func_150ADA20() % (u32)count;
                        while (idx--) {
                            tail = head;
                            head = head->next;
                        }
                        n = head - arg0->unk24;
                        b.unk44 = func_150ADA20() % 101U + 100;
                        b.unk14 = 0.0f;
                        b.unk18 = 0.0f;
                        *(F898V2 *)a.unk28 = *(F898V2 *)rng2->unk4;
                        *(F898V2 *)a.unk30 = *(F898V2 *)rng2->unkC;
                        a.unk23 = func_150ADA20() % 5U + 4;
                        a.unk24 = func_150ADA20() % 5U + 4;
                        func_15143134(ent->unk8 + n, b.unk1C, arg0->unk10->unk1D4 + ent->unk8[n].unkC);
                        *(F898V3 *)b.unk28 = *(F898V3 *)b.unk1C;
                        if (arg0->unk10->unk118 < b.unk1C[1] - *(f32 *)&a.unk28[1] - *(f32 *)&a.unk30[1]) {
                            res = func_1513D524(&b, 12, 0, 0, 0xD, ((func_150ADA20() % 1) ? 1 : 0) | (D_800AB404[arg0->unk2C] ? 0 : 2), 0x60, 0xFF, 0);
                            head->obj = res;
                            if (res != NULL) {
                                memcpy(&res->sub, &a, sizeof(a));
                                head->timer = func_150ADA20() % (u32)(ent->unk16 + 1) + ent->unk14;
                                count -= 1;
                                head = head->next;
                                tail->next = head;
                                arg0->unk20 = arg0->unk20 + 1;
                            }
                        }
                        arg0->unk1C = arg0->unk1C - 1.0f;
                    } while (arg0->unk1C > 1.0f && count != 0 && self->unk20 < ent->unk10);
                }
            }
        } else {
            done = 1;
            if (arg0->unk18 & 1) {
                va3 = 1;
                va0 = arg0->unk16;
            } else {
                va3 = 0;
                va0 = 0x12C;
            }
            res = (F898Obj *)func_151491F4(va0, -1, 0xD, va3, 8, 0xC, 0xFF, 0);
            if (res != NULL) {
                memcpy((u8 *)res + 0x28, &arg0->unk10, 0xC);
            }
        }
    }
    if (done != 0) {
        func_1516972C((void *)arg0);
    }
}

struct OscillatorMotionState {
    u8 pad_0x0[0x21];
    u8 field_0x21;
    u8 field_0x22;
    s8 field_0x23;
    s8 field_0x24;
    u8 pad_0x25[0x3];
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
};

struct OscillatorObject {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    struct OscillatorMotionState field_0x110;
};

s32 func_15190F9C(struct OscillatorObject *arg0) {
    struct OscillatorObject *p = arg0;
    struct OscillatorMotionState *f = &arg0->field_0x110;
    f32 r1;
    f32 r2;

    f->field_0x21 = f->field_0x21 + f->field_0x23 * D_800BE9E4;
    f->field_0x22 = f->field_0x22 + f->field_0x24 * D_800BE9E4;
    r1 = func_151423D8((u8)(f->field_0x21 - 0x40));
    r2 = func_151423D8((u8)(f->field_0x22 - 0x40));
    p->field_0x2C += (f->field_0x28 + f->field_0x30 * r1 - p->field_0x2C) * 0.5f;
    p->field_0x30 += (f->field_0x2C + f->field_0x34 * r2 - p->field_0x30) * 0.5f;
    return 1;
}

struct Struct1519108C {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
    f32 unk12C;
};

s16 *func_1519108C(struct Struct1519108C *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f2;
    f32 sinval;
    f32 temp_f18;
    f32 temp_y_cos;
    f32 temp_y_sin;
    f32 var_f2;
    f32 var_f14;
    f32 *temp_v1;
    f32 *temp_a0;
    f32 temp_f0;
    u8 sp37;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp37);
    ret = var_v0;
    if (var_v0) {
        if (sp37) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_v1 = &D_800DD1E8[arg1];
    temp_a0 = &D_800DD1D8[arg1];
    var_f14 = -1.0f;
    var_f2 = ((*temp_v1 * (arg0->unk40 - arg0->unk34)) - (*temp_a0 * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f2 < 0.0f) {
        var_f14 = 1.0f;
        var_f2 = -var_f2;
    }

    temp_f0 = sqrtf(var_f2) * arg0->unk12C;
    if (D_800A8018 < temp_f0) {
        temp_f0 = D_800A8018;
    }

    temp_f0 = temp_f0 * var_f14;
    sinval = sinf(temp_f0);
    temp_f0 = cosf(temp_f0);
    temp_f2 = arg0->unk2C * temp_f0;
    temp_f18 = arg0->unk2C * sinval;
    temp_y_cos = arg0->unk30 * temp_f0;
    temp_y_sin = arg0->unk30 * sinval;

    var_v0[0] = (*temp_v1 * -temp_f2) + arg0->unk34;
    var_v0[1] = arg0->unk38 + temp_f18;
    var_v0[2] = arg0->unk3C - (*temp_a0 * -temp_f2);
    var_v0[3] = 0;

    var_v0[8] = (*temp_v1 * temp_f2) + arg0->unk34;
    var_v0[9] = arg0->unk38 - temp_f18;
    var_v0[10] = arg0->unk3C - (*temp_a0 * temp_f2);
    var_v0[11] = 0;

    temp_f0 = temp_f2 - temp_y_sin;
    var_v0[16] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[17] = (arg0->unk38 - temp_f18) - temp_y_cos;
    var_v0[18] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[19] = 0;

    temp_f0 = -(temp_f2 + temp_y_sin);
    var_v0[24] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[25] = (arg0->unk38 + temp_f18) - temp_y_cos;
    var_v0[26] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[27] = 0;

    return ret;
}

struct Struct15191400Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x14];
    u8 unk20;
};
struct Struct15191400Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x3A];
    s32 unk58;
    u8 pad5C[0xB4];
    struct Struct15191400Sub unk110;
};
struct Struct15191400Slot {
    struct Struct15191400Obj *unk0;
    s32 unk4;
    s32 unk8;
};
struct Struct15191400Arg {
    u8 pad0[0x1C];
    f32 unk1C;
    s32 unk20;
    struct Struct15191400Slot *unk24;
};
extern struct Struct15191400Range D_800A9D40[];
extern void func_1513F680();

void func_15191400(struct Struct15191400Arg *arg0) {
    s32 i;
    struct Struct15191400Slot *slot;
    struct Struct15191400Sub *sub;
    struct Struct15191400Range *range;

    arg0->unk1C = 0.0f;
    arg0->unk20 = 0;
    slot = arg0->unk24;
    for (i = 0; i < 60; i++, slot++) {
        if (slot->unk0 != NULL) {
            sub = &slot->unk0->unk110;
            range = &D_800A9D40[sub->unk20];
            sub->unk0 = 0.0f;
            sub->unk4 = 0.0f;
            sub->unk8 = 0.0f;
            func_1513F680(slot->unk0, 13, 18, 0, 13);
            slot->unk0->unk1C = (u32)func_150ADA20() % (range->unk2 + 1) + range->unk0;
            slot->unk0->unk58 |= 1;
        }
    }
    bzero(arg0->unk24, 0x2D0);
}

struct Sub15191520 {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 grav;
    f32 drag;
    u8 pad_14[4];
    f32 damp;
    u8 pad_1C[0x1C];
    f32 floor;
    u8 unk3C[0x19];
    u8 type;
    u8 pad_56[6];
    u8 color;
};

struct Obj15191520 {
    u8 pad_0;
    u8 unk1;
    u8 pad_2[0xA];
    u8 unkC;
    u8 pad_D[0x1F];
    f32 unk2C;
    f32 unk30;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    u8 pad_4C[0x10];
    u8 unk5C;
    u8 pad_5D[0xB3];
    struct Sub15191520 sub;
};

s32 func_1514672C(struct17 *arg0);
s32 func_15046C80(f32 *arg0, s32 arg1, f32 arg2, void *arg3);
void func_151DAB58(u8, f32, u8, struct17 *, s32, u8, u8);

s8 func_15191520(struct Obj15191520 *arg0) {
    f32 dx;
    f32 dz;
    f32 dy;
    struct Sub15191520 *sub;
    s8 ret;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;

    oldY = arg0->y;
    sub = &arg0->sub;
    ret = 1;
    oldVx = sub->vx;
    oldVz = sub->vz;
    sub->vx *= sub->drag * D_800BE9A4;
    sub->vz *= sub->drag * D_800BE9A4;
    dx = (sub->vx + oldVx) * 0.5f * D_800BE9A4;
    dy = sub->vy * D_800BE9A4 + sub->grav * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (sub->vz + oldVz) * 0.5f * D_800BE9A4;
    sub->vy += sub->grav * D_800BE9A4;
    arg0->x += dx;
    arg0->y += dy;
    arg0->z += dz;
    arg0->x2 += dx;
    arg0->y2 += dy;
    arg0->z2 += dz;
    arg0->x2 = arg0->x + (arg0->x2 - arg0->x) * sub->damp;
    arg0->y2 = arg0->y + (arg0->y2 - arg0->y) * sub->damp;
    arg0->z2 = arg0->z + (arg0->z2 - arg0->z) * sub->damp;
    {
    struct17 pos;
    pos.unk0 = arg0->x;
    pos.unk4 = oldY - arg0->unk30;
    pos.unk8 = arg0->z;

    if (func_1514672C(&pos) == 0) {
        return 0;
    }
    if (func_15046C80((f32 *)&pos, 0, arg0->y - arg0->unk30, &sub->floor) != 0) {
        pos.unk4 = sub->floor;
        if (sub->type == 3) {
            func_151DBCBC(sub->color, arg0->unk2C * arg0->unk30 * 0.25f, arg0->unk5C, (s32)&sub->unk3C, &pos, arg0->unkC, arg0->unk1);
        } else if (func_150ADA20() & 1) {
            func_151D9B8C(sub->color, arg0->unk2C * arg0->unk30 * 0.150000006f, arg0->unk5C, (s32)&sub->unk3C, &pos, (func_150ADA20() % 41U) + 0x50, 1, 1, 0, arg0->unkC, arg0->unk1);
        } else {
            func_151DAB58(sub->color, arg0->unk2C * arg0->unk30 * 0.05000000075f, arg0->unk5C, &pos, 1, arg0->unkC, arg0->unk1);
        }
        ret = 0;
    }
    }
    return ret;
}

s32 func_1519187C(void *arg0)
{
  s16 v0;
  s32 v1;
  v0 = *((s16 *) (((u8 *) arg0) + 0x1C));
  if (v0 < (*((s16 *) (((0, (u8 *) arg0)) + 0x124))))
  {
    v1 = v0;
    v1 *= *((s16 *) (((u8 *) arg0) + 0x126));
    if (v1 < (*((u8 *) (((u8 *) arg0) + 0x5C))))
    {
      *((u8 *) (((u8 *) arg0) + 0x5C)) = v1;
    }
  }
  return 1;
}


void func_151918BC(void *arg0) {
    u8 *temp_v1;
    u8 *temp_a0;
    u8 var_v0;

    temp_a0 = (u8 *)arg0 + 0x28;
    temp_v1 = *(u8 **)temp_a0;
    var_v0 = 0;
    if (*(s32 *)temp_v1 == 0) {
        var_v0 = 1;
    } else if (temp_a0[4] != temp_v1[0x3B]) {
        var_v0 = 1;
    }
    if (!var_v0 && (*(s32 *)(temp_v1 + 0x1D4) != 0)) {
        if ((*(u8 *)((u8 *)arg0 + 0xD) & 1) != 0) {
            *(s16 *)(temp_a0 + 6) = *(s16 *)((u8 *)arg0 + 0xE);
            temp_a0[8] |= 1;
        } else {
            *(s16 *)(temp_a0 + 6) = 0x12C;
        }
        var_v0 = 1;
        func_15190770(temp_a0, 0, *(u8 *)((u8 *)arg0 + 0xC), *(u8 *)((u8 *)arg0 + 1));
    }
    if (var_v0) {
        func_1516972C(arg0);
    }
}

void func_15191980(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;

  arg1p = (s32 *) &arg1;
  if (arg2 == 0xD)
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      u8 *base;
      u8 old;
      u8 *flagsPtr;

      old = *((u8 *) (((u8 *) arg0) + 0x18));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0x18);
      base = (u8 *) arg0;
      *flagsPtr = *((u8 *) (((u8 *) arg0) + 0x18)) | 1;
      *((s16 *) (base + 0x16)) = 0x64;
      ;
    }
  }
  else
    if (((arg2 == 0xC) || (arg2 == 0x50)) || (arg2 == 0xE))
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
  {
    func_15169850(arg1, arg2, (s32) arg0 + 0x10, (s32) arg0 + 0x14, (s32) arg0);
  }
}

struct Struct1518F49C {
    s32 unk0;
    u8 unk4;
};

void func_1516972C(struct102 *arg0);
void func_15169850(s32, u8, s32, s32, s32);
void func_15191A84(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;
  struct Struct1518F49C *p;
  u8 *base;
  u8 old;
  u8 *flagsPtr;
  p = (struct Struct1518F49C *) (((u8 *) arg0) + 0x28);
  arg1p = (s32 *) (&arg1);
  if (((arg2 == 0xC) || (arg2 == 0xE)) || (arg2 == 0x50))
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*arg1p) + 4))) == p->unk4))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
    if (arg2 == 0xD)
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*((s32 *) (&arg1))) + 4))) == p->unk4))
    {
      old = *((u8 *) (((u8 *) arg0) + 0xD));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0xD);
      base = (u8 *) arg0;
      *flagsPtr = (*((u8 *) (((u8 *) arg0) + 0xD))) | 1;
      *((s16 *) (base + 0xE)) = 0x64;
      dummy_label_15191A84_1:
      ;

      ;
      ;
    }
  }
  else
  {
    func_15169850(arg1, arg2, (s32) p, (s32) (&p->unk4), (s32) arg0);
  }
}


extern s32 D_800A8010;

void func_15191B8C(s32 arg0, u8 arg1) {
    s32 sp1C;

    sp1C = D_800A8010;
    func_151494E0(arg0, arg1);
    func_15169260(&sp1C, 1, arg0, arg1);
}
