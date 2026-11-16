#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct Node1518BA90 {
    u8 pad0[8];
    struct Node1518BA90 *next;
    u8 padC[0x18];
    void *owner;
    u8 type;
} Node1518BA90;

typedef struct Spawn1518BA90 {
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s16 field10;
    s16 field12;
    void *owner;
    u8 type;
    s8 field19;
    s8 field1A;
} Spawn1518BA90;

typedef struct Owner1518BA90 {
    u8 pad0[4];
    u8 kind;
    u8 pad5[0x36];
    u8 type;
} Owner1518BA90;

extern u8 D_800DCE50[];
extern f32 D_800A7434;
extern f32 D_800A7438;
extern f32 D_800A743C;
s32 func_1518BCD0(void *arg0, u8 arg1, s32 arg2);

s32 func_1518BA90(void *arg0, u8 arg1, s32 arg2) {
    u8 *block;
    Spawn1518BA90 spawn;
    Node1518BA90 *node;
    s32 i;

    if (arg0 == 0) {
        return 0;
    }
    i = 0;
    do {
        block = D_800DCE50 + i * 0x1A0;
        for (node = *(Node1518BA90 **)(block + 0x7C); node != 0; node = node->next) {
            if (node->owner == arg0 ||
                node->type == ((Owner1518BA90 *)arg0)->type) {
                return (s32)node;
            }
        }
        i++;
    } while ((u8 *)&D_800DD190 != D_800DCE50 + i * 0x1A0);

    switch (*(u8 *)((u8 *)arg0 + 4) + 1) {
    case 0:
        return 0;
    case 8:
        spawn.field12 = 0x37;
        spawn.field10 = 4;
        spawn.field19 = 5;
        spawn.field1A = 0x13;
        spawn.x = D_800A7434;
        spawn.y = D_800A7438;
        spawn.z = D_800A743C;
        spawn.scale = 5.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        spawn.field12 = 0;
        spawn.field10 = 5;
        spawn.field19 = 5;
        spawn.field1A = 0x13;
        spawn.x = 10.0f;
        spawn.y = 10.0f;
        spawn.z = 10.0f;
        spawn.scale = 10.0f;
        break;
    default:
        return 0;
    }
    spawn.owner = arg0;
    spawn.type = *(u8 *)((u8 *)arg0 + 0x3B);
    return func_1518BCD0(&spawn, arg1, arg2);
}

extern u8 D_800DCE50[];

typedef struct Node1518BBF4 {
    u8 pad0[0x8];
    struct Node1518BBF4 *unk8;
    u8 padC[0x18];
    struct Node1518BBF4 *unk24;
    u8 unk28;
    u8 pad29[0x12];
    u8 unk3B;
} Node1518BBF4;

void func_1518BBF4(Node1518BBF4 *arg0) {
    s32 i;
    Node1518BBF4 *node;

    if (arg0 != NULL) {
        for (i = 0; i != 2; i++) {
            D_800DD190++;
            node = ((Node1518BBF4 *(*)[104])D_800DCE50)[i][31];
            while (node != NULL) {
                ((Node1518BBF4 **)D_800DD198)[D_800DD190] = node->unk8;
                node->unk24 = arg0;
                if (arg0 != NULL || node->unk28 == arg0->unk3B) {
                    func_1516972C((struct102 *)node);
                }
                node = ((Node1518BBF4 **)D_800DD198)[D_800DD190];
            }
            {
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
}


s32 func_15167A68(s32, s32, s32, s32, s32, s32);

typedef struct {
    u8 pad_0x0[0x10];
    u8 field_0x10[0x1C];
    s32 field_0x2C;
    s32 field_0x30;
} EffectObject1F;

s32 func_1518BCD0(void *arg0, u8 arg1, s32 arg2) {
    EffectObject1F *temp_v0;

    temp_v0 = (EffectObject1F *)func_15167A68(0x1F, arg2, 0x44, 1, (u8)arg1, 1);
    if (temp_v0 == 0) {
        return 0;
    }
    memcpy(temp_v0->field_0x10, (void *)arg0, 0x1C);
    temp_v0->field_0x2C = func_150ADA20() & 0x1F;
    temp_v0->field_0x30 = func_150ADA20() & 0x1F;
    return (s32)temp_v0;
}

typedef struct {
    f32 pos[3];
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Spawn1518BD60;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
} Part1518BD60;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} Style1518BD60;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x184 - 0x3C];
    u32 unk184;
    u8 pad188[0x1AC - 0x188];
    s16 unk1AC[2][3];
    u8 pad1B8[0x1D4 - 0x1B8];
    s32 unk1D4;
} Target1518BD60;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    Target1518BD60 *unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 pad2B;
    s32 unk2C[2];
    f32 unk34[2];
    f32 unk3C[2];
} Emitter1518BD60;

typedef struct {
    Emitter1518BD60 *owner;
    u8 index;
} Link1518BD60;

extern void *func_15147DA0(Spawn1518BD60 *, Part1518BD60 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, Style1518BD60 *, s32, s32, s32);

void func_1518BD60(Emitter1518BD60 *arg0) {
    u8 i;
    s32 idx;
    void *ret;
    Spawn1518BD60 spawn;
    Part1518BD60 part;
    s32 pad;
    Style1518BD60 style;
    f32 speed;
    f32 sinA;
    f32 cosA;
    s32 angle;
    Target1518BD60 *obj;
    Link1518BD60 *link;

    for (i = 0; i < 2; i++) {
        arg0->unk2C[i] -= D_800BE9E4;
        obj = arg0->unk24;
        if (obj != NULL && obj->unk0 != 0 && obj->unk1D4 != 0 && obj->unk3B == arg0->unk28) {
            if (arg0->unk2C[i] < 0) {
                idx = i;
                if (idx) {}
                angle = func_150ADA20() % (u32)(arg0->unk22 + 1);
                sinA = func_151423D8(angle);
                cosA = func_151423D8(angle - 0x40);
                speed = func_150ADA68() * arg0->unk14 + arg0->unk10;
                arg0->unk2C[i] = func_150ADA20() % 23U + 5;
                style.unk0 = 0;
                style.unk4 = 1;
                style.unk8 = 0x160600;
                style.unkC = 3;
                style.unk10 = 0x10;
                style.unk14 = 0x80;
                style.unk18 = 0x20;
                style.unk1C = 0;
                style.unk1D = 9;
                spawn.unk15 = func_150ADA20() % (u32)(arg0->unk20 + 1) + 3;
                spawn.unk10 = 1;
                spawn.unkE = 1;
                spawn.unkC = func_150ADA20() % 21U + 0x50;
                part.unk19 = arg0->unk29;
                part.unk0 = func_150ADA68() * arg0->unk1C + arg0->unk18;
                part.unk8 = 0.0f;
                part.unk10 = 0.6539999843f;
                part.unk4 = speed * cosA;
                part.unkC = speed * sinA;
                part.unk18 = arg0->unk2A | 8;
                part.unk1A = 0xFF - ((u8)(arg0->unk24->unk184 >> 5) << 6);
                part.unk1B = -1;
                ret = func_15147DA0(&spawn, &part, spawn.unk15 * 0xC + 8, 5, 1, 0, 2, 1, 0, 0, 0, &style, 0, arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    link = (Link1518BD60 *)((u8 *)*(void **)((u8 *)ret + 0x98) + 0x48);
                    link->owner = arg0;
                    link->index = idx;
                }
                obj = arg0->unk24;
            }
            arg0->unk34[i] = obj->unk1AC[i][0];
            arg0->unk3C[i] = arg0->unk24->unk1AC[i][2];
        }
    }
}

typedef struct { f32 x, y, z; } Vec3f_C0B8;
typedef struct { s16 x, y, z; } Vec3s_C0B8;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x1AC - 0x3C];
    Vec3s_C0B8 points[2];
    s8 unk1B8;
    u8 pad1B9;
    s8 unk1BA;
    u8 pad1BB[0x1D4 - 0x1BB];
    s32 unk1D4;
} Model_C0B8;

typedef struct {
    u8 pad0[0x24];
    Model_C0B8 *model;
    u8 unk28;
    u8 pad29[0x34 - 0x29];
    f32 unk34[2];
    f32 unk3C[2];
} Obj_C0B8;

typedef struct {
    Obj_C0B8 *obj;
    s8 idx;
} Sub_C0B8;

typedef struct {
    u8 pad0[4];
    f32 vx;
    f32 vy;
    f32 vz;
    f32 grav;
    u8 pad14[4];
    u8 flags;
    u8 pad19[7];
    u8 state;
    u8 pad21[0x48 - 0x21];
    Sub_C0B8 sub;
    Vec3f_C0B8 offs[1];
} Ctrl_C0B8;

typedef struct {
    Vec3f_C0B8 pos;
    f32 w;
    u16 ang;
} Elem_C0B8;

typedef struct {
    u8 pad0[0x25];
    u8 max;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Elem_C0B8 *elems;
    Ctrl_C0B8 *ctrl;
} Trail_C0B8;

s32 func_1518C0B8(Trail_C0B8 *arg0) {
    Ctrl_C0B8 *ctrl;
    Elem_C0B8 *elems;
    s32 i;
    Sub_C0B8 *sub;
    Vec3f_C0B8 *offs;
    Vec3f_C0B8 pos;
    f32 dx;
    f32 dz;
    f32 len2;
    f32 c;
    f32 s;
    f32 inv;
    u16 ang;
    u16 step;
    f32 ox;
    f32 oz;
    f32 invdt;
    f32 rx;
    f32 rz;

    elems = arg0->elems;
    ctrl = arg0->ctrl;
    sub = &ctrl->sub;
    offs = ctrl->offs;
    i = arg0->tail;
    if (sub->obj->model == NULL || sub->obj->model->unk0 == 0 || sub->obj->model->unk1D4 == 0 ||
        sub->obj->model->unk3B != sub->obj->unk28) {
        return 0;
    }
    pos.x = sub->obj->model->points[sub->idx].x;
    pos.y = sub->obj->model->points[sub->idx].y;
    pos.z = sub->obj->model->points[sub->idx].z;
    dx = sub->obj->model->unk1B8 * 0.00390625f;
    dz = sub->obj->model->unk1BA * 0.00390625f;
    len2 = dx * dx + dz * dz;
    if (len2) {
        extern f32 sqrtf(f32);

        inv = 1.0f / sqrtf(len2);
        c = dx * inv;
        s = dz * inv;
    } else {
        s = c = 0.0f;
    }
    while (i != arg0->head) {
        elems[i].w -= ctrl->grav * D_800BE9A4;
        offs[i].x += ctrl->vx * D_800BE9A4;
        offs[i].y += elems[i].w;
        offs[i].z += ctrl->vz * D_800BE9A4;
        elems[i].pos.x = (pos.x + offs[i].x * s) + offs[i].z * c;
        elems[i].pos.y = offs[i].y + pos.y;
        elems[i].pos.z = (pos.z + offs[i].z * s) - offs[i].x * c;
        i++;
        if (i == arg0->max) {
            i = 0;
        }
    }
    if (arg0->n < arg0->max - 1) {
        ang = 0;
        step = 0;
        if (arg0->n != 0) {
            step = 0x1000 / arg0->n;
        }
        elems[arg0->head].pos = pos;
        elems[arg0->head].w = 0.0f;
        offs[arg0->head].x = 0.0f;
        offs[arg0->head].y = 0.0f;
        offs[arg0->head].z = 0.0f;
        arg0->n++;
        arg0->head++;
        if (arg0->head == arg0->max) {
            arg0->head = 0;
        }
        for (i = arg0->tail; i != arg0->head;) {
            elems[i].ang = ang;
            i++;
            ang += step;
            if (i == arg0->max) {
                i = 0;
            }
        }
    } else {
        invdt = 1.0f / D_800BE9A4;
        rx = ctrl->vx * s + ctrl->vz * c;
        rz = ctrl->vz * s - ctrl->vx * c;
        ox = (pos.x - sub->obj->unk34[sub->idx]) * invdt;
        oz = (pos.z - sub->obj->unk3C[sub->idx]) * invdt;
        ctrl->vx = rx + ox;
        ctrl->vz = rz + oz;
        if (ctrl->flags & 0x17) {
            ctrl->state = 3;
        } else {
            ctrl->state = 2;
        }
    }
    return 1;
}

s32 func_1518C540(void* arg0) {
    s32 v1;
    void* p;

    p = *(void**)((u8*)arg0 + 0x98);
    v1 = *(s16*)((u8*)arg0 + 0x1C) << 3;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    *(s8*)((u8*)p + 0x1B) = v1;
    if ((v1 & 0xFF) < 0) {
        return 0;
    }
    return 1;
}

void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(s32 arg0, s32 arg1, s32 arg2);

typedef struct {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    s32 field_0xC;
    s32 field_0x10;
} EffectPoint;

typedef struct {
    u8 pad_0x0[0x20];
    s8 field_0x20;
} EffectState;

typedef struct {
    u8 pad_0x0[0x1];
    u8 field_0x1;
    u8 pad_0x2[0xA];
    u8 field_0xC;
    u8 pad_0xD[0x20];
    s8 field_0x2D;
    u8 pad_0x2E[0x66];
    EffectPoint *field_0x94;
    EffectState *field_0x98;
} EffectEmitter;

typedef struct {
    u8 pad_0x0[0x80];
    s32 field_0x80;
    u8 pad_0x84[0x6];
    s16 field_0x8A;
    s16 field_0x8C;
    s16 field_0x8E;
    s16 field_0x90;
    s16 field_0x92;
    s16 field_0x94;
    s16 field_0x96;
    s16 field_0x98;
    u8 pad_0x9A[0x6];
    s8 field_0xA0;
    s8 field_0xA1;
    s8 field_0xA2;
    u8 pad_0xA3[0x5];
} EffectSpawnPacket;

s32 func_1518C57C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    EffectSpawnPacket sp38;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp38.field_0x80 = 0;
    sp38.field_0x8A = 0x100;
    sp38.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp38.field_0x8E = (s32)arg4;
    sp38.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp38.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp38.field_0x92 = sp38.field_0x94;
    sp38.field_0xA2 = -1;
    sp38.field_0xA0 = 0x12;
    sp38.field_0x98 = 0xC;
    sp38.field_0xA1 = 0;
    sp38.field_0x96 = (s32)arg4;
    func_1516865C(&sp38, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp38, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    return 1;
}

u8 func_151D8E20(void);

s32 func_1518C69C(EffectEmitter *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    EffectSpawnPacket sp50;
    EffectState *temp;
    EffectPoint *base;

    temp = arg0->field_0x98;
    base = arg0->field_0x94;
    sp50.field_0x80 = 0;
    sp50.field_0x8A = 0x100;
    sp50.field_0x8C = (s32)base[arg0->field_0x2D].field_0x0;
    sp50.field_0x8E = (s32)arg4;
    sp50.field_0x90 = (s32)base[arg0->field_0x2D].field_0x8;
    sp50.field_0x94 = (func_150ADA20() % 0x15U) + 0x23;
    sp50.field_0x92 = sp50.field_0x94;
    sp50.field_0xA2 = -1;
    sp50.field_0xA0 = 0x12;
    sp50.field_0x98 = 0xC;
    sp50.field_0xA1 = 0;
    sp50.field_0x96 = (s32)arg4;
    func_1516865C(&sp50, 0xFF, 0xFF, 0xFF, 0xFF);
    func_15168800((s32)&sp50, arg0->field_0xC, arg0->field_0x1);
    temp->field_0x20 = 4;
    {
    u8 id;
    f32 pos[3];
    pos[0] = base[arg0->field_0x2D].field_0x0;
    pos[1] = arg4;
    pos[2] = base[arg0->field_0x2D].field_0x8;
    id = func_151D8E20();
    func_151DBCBC(id, func_150ADA68() * 40.0f + 100.0f, 0xFF, arg5, pos, arg0->field_0xC, arg0->field_0x1);
    }
    return 1;
}

typedef struct {
    u8 pad0[0x24];
    s32 unk24;
    u8 unk28;
} SubA1518C850;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB1518C850;

void func_1518C850(struct102 *arg0, s32 arg1, u8 arg2) {
    SubA1518C850 *dst = (SubA1518C850 *)arg0;
    ArgB1518C850 *b = (ArgB1518C850 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        b0 = b->unk0;
        if (b0 == dst->unk24) {
            dst->unk24 = b->u4.w4;
            dst->unk28 = b->unk9;
        } else {
            if (dst->unk24 == b->u4.w4) {
                dst->unk24 = b0;
                dst->unk28 = b->unk8;
            }
trailing_label_1518C850:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == dst->unk24) || (b->u4.b4 == dst->unk28)) {
            func_1516972C(arg0);
        }
    }
}
