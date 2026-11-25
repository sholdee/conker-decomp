#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    s16 x, y, z;
} Vec3s;

/* Link fields shared by every object (see game_1944C0.c). */
typedef struct ObjLink {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 list;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ struct ObjLink *prev;
    /* 0x8 */ struct ObjLink *next;
    /* 0xC */ u8 owner;
} ObjLink;

/* Object lists: D_800DCE50[list][type] is the head of a linked list of objects of that type. */
extern ObjLink *D_800DCE50[2][104];

/* While a list is walked, the next node is saved in D_800DD198[depth] (depth D_800DD190). */
#define OBJ_ITER_NEXT ((ObjLink **)D_800DD198)

#define OBJ_TYPE_TRAIL_EMITTER 0x1F

/* Emitter settings, copied into the object from a template by func_1518BCD0. */
typedef struct {
    /* 0x00 */ f32 speedMin;
    /* 0x04 */ f32 speedRange;
    /* 0x08 */ f32 halfWidthMin;
    /* 0x0C */ f32 halfWidthRange;
    /* 0x10 */ s16 pointRange; /* each trail gets 3 + rand % (pointRange + 1) points */
    /* 0x12 */ s16 angleRange;
    /* 0x14 */ struct127 *owner;
    /* 0x18 */ u8 ownerId;     /* owner->unique_id when attached */
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 flags;
} TrailEmitterParams; /* size 0x1C */

/* Type 0x1F object: emits trails from two points of its owner actor. */
typedef struct {
    /* 0x00 */ ObjLink link;
    /* 0x10 */ TrailEmitterParams params;
    /* 0x2C */ s32 timers[2]; /* frames until the next trail from each point */
    /* 0x34 */ f32 prevX[2];  /* each point's position on the previous update */
    /* 0x3C */ f32 prevZ[2];
} TrailEmitter; /* size 0x44 */

void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list);

/* Returns the actor's trail emitter, creating one if its kind has emitter settings. */
TrailEmitter *func_1518BA90(struct127 *actor, u8 owner, s32 list) {
    ObjLink **heads;
    TrailEmitterParams params;
    TrailEmitter *emitter;
    s32 i;

    if (actor == NULL) {
        return NULL;
    }
    for (i = 0; i < 2; i++) {
        heads = D_800DCE50[i];
        for (emitter = (TrailEmitter *)heads[OBJ_TYPE_TRAIL_EMITTER]; emitter != NULL;
             emitter = (TrailEmitter *)emitter->link.next) {
            if (emitter->params.owner == actor || emitter->params.ownerId == actor->unique_id) {
                return emitter;
            }
        }
    }

    switch (actor->id + 1) {
    case 0: /* id 0xFF */
        return NULL;
    case 8:
        params.angleRange = 55;
        params.pointRange = 4;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 2.75999999f;
        params.speedRange = 5.960000038f;
        params.halfWidthMin = 3.210000038f;
        params.halfWidthRange = 5.0f;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        params.angleRange = 0;
        params.pointRange = 5;
        params.texture = 5;
        params.flags = 0x13;
        params.speedMin = 10.0f;
        params.speedRange = 10.0f;
        params.halfWidthMin = 10.0f;
        params.halfWidthRange = 10.0f;
        break;
    default:
        return NULL;
    }
    params.owner = actor;
    params.ownerId = actor->unique_id;
    return func_1518BCD0(&params, owner, list);
}

/* Detaches every trail emitter onto `actor` and destroys it. */
void func_1518BBF4(struct127 *actor) {
    s32 i;
    TrailEmitter *emitter;

    if (actor != NULL) {
        for (i = 0; i < 2; i++) {
            D_800DD190++;
            emitter = (TrailEmitter *)D_800DCE50[i][OBJ_TYPE_TRAIL_EMITTER];
            while (emitter != NULL) {
                OBJ_ITER_NEXT[D_800DD190] = emitter->link.next;
                emitter->params.owner = actor;
                if (actor != NULL || emitter->params.ownerId == actor->unique_id) {
                    func_1516972C((struct102 *)emitter);
                }
                emitter = (TrailEmitter *)OBJ_ITER_NEXT[D_800DD190];
            }
            {
                // matching: a slotless block-scope decl moves the -g3 line so the decrement fills the bnel slot
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
}

/* Spawns a trail emitter object from `params` with random initial timers. */
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list) {
    TrailEmitter *emitter;

    emitter = func_15167A68(OBJ_TYPE_TRAIL_EMITTER, list, sizeof(TrailEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->params, params, sizeof(TrailEmitterParams));
    emitter->timers[0] = func_150ADA20() & 0x1F;
    emitter->timers[1] = func_150ADA20() & 0x1F;
    return emitter;
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
