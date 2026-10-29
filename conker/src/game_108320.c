#include <ultra64.h>
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_1513D524
#include "variables.h"

extern void func_151D5D60();

struct Foo150DB2D8 {
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
    s16 *unk100[1];
};


typedef struct {
    f32 x, y, z;
} Vec3_150DAE70;

typedef struct {
    u8  unk0;
    u8  pad1;
    s16 unk2;
    s16 unk4;
    u8  pad6[2];
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  pad14[8];
    Vec3_150DAE70 pos;
    Vec3_150DAE70 dir;
    f32 scale[3];
    s32 unk40;
    u8  unk44;
    u8  unk45;
    u8  pad46[0x12];
} Spawn150DAE70;

typedef struct {
    u8  pad0[0x110];
    Vec3_150DAE70 vel;
    f32 unk11C;
    f32 unk120;
    f32 unk124;
    f32 unk128;
    f32 unk12C;
    f32 unk130;
    f32 unk134;
    u8  unk138;
} Obj150DAE70;

extern void func_15143134(void *, Vec3_150DAE70 *, s32);
typedef struct {
    u8  pad0[0x110];
    s32 unk110;
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    s32 unk120;
} Struct150DB518Obj;

extern Struct150DB518Obj *func_1513D524(s32 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern Vec3_150DAE70 D_80088910[];
extern Vec3_150DAE70 D_8008891C[];
extern s32 D_80088928[];
extern f32 D_8008892C[];
extern s16 D_80088930[];
extern f32 D_80088934[];
extern f32 D_80088938[];
extern f32 D_8008893C[];
extern u8 D_80088940[];
extern f32 D_80088944[];
extern f32 D_80088948[];

void func_150DAE70(void *arg0, s32 arg1, u8 arg2, s32 arg3) {
    s32 base;
    Vec3_150DAE70 from;
    Vec3_150DAE70 to;
    Vec3_150DAE70 vel;
    Spawn150DAE70 spawn;
    f32 distSq;
    f32 inv;
    Obj150DAE70 *obj;
    s32 bones;

    if (arg0 != NULL) {
        bones = *(s32 *)((u8 *)arg0 + 0x1D4);
        if (bones != 0) {
            base = (D_80088928[arg1] << 6) + bones;
            func_15143134(&D_80088910[arg1], &from, base);
            func_15143134(&D_8008891C[arg1], &to, base);
            vel.x = D_8008892C[arg1] * (to.x - from.x);
            vel.y = D_8008892C[arg1] * (to.y - from.y);
            vel.z = D_8008892C[arg1] * (to.z - from.z);
            distSq = (vel.x * vel.x) + (vel.z * vel.z);
            if (!(fabsf(distSq) < 9.999999747e-05f)) {
spawn.unk0 = 0x17;
spawn.unkC = 0x243A;
spawn.unk8 = 0;
spawn.unk40 = 0x401;
spawn.unk4 = D_80088930[arg1];
spawn.unk44 = 0xFF;
spawn.unk45 = 0xFF;
spawn.unk12 = 0xC8; spawn.unk11 = 0xC8; spawn.unk10 = 0xC8;
spawn.unk13 = 0xC8;
spawn.unk2 = 0x401;
                spawn.pos = from;
                spawn.scale[0] = 1.0f;
                spawn.scale[1] = 1.0f;
                inv = 1.0f / sqrtf(distSq);
                spawn.scale[2] = 1.0f;
                spawn.dir.x = vel.z * inv;
                spawn.dir.y = 0.0f;
                spawn.dir.z = vel.x * inv;
                obj = (Obj150DAE70 *)func_1513D524((s32)&spawn, 9, 0, 0, 7, 0, 0x2C, arg2, arg3);
                if (obj != NULL) {
                    obj->vel = vel;
                    obj->unk11C = D_80088934[arg1];
                    obj->unk120 = D_80088938[arg1];
                    obj->unk12C = D_80088944[arg1];
                    obj->unk130 = D_80088948[arg1];
                    obj->unk124 = obj->unk128 = 1.0f;
                    obj->unk134 = D_8008893C[arg1];
                    obj->unk138 = D_80088940[arg1];
                }
            }
        }
    }
}

typedef struct {
    f32 vel[3];   /* 0x110 */
    f32 unkC;     /* 0x11C */
    f32 unk10;    /* 0x120 */
    f32 unk14;    /* 0x124 */
    f32 unk18;    /* 0x128 */
    f32 unk1C;    /* 0x12C */
    f32 unk20;    /* 0x130 */
    f32 unk24;    /* 0x134 */
    u8 unk28;     /* 0x138 */
} Sub150DB114;

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 pos[3];
    char pad40[0x1C];
    u8 unk5C;
    char pad5D[0xB3];
    Sub150DB114 sub;
} Obj150DB114;

s32 func_150DB114(Obj150DB114 *arg0) {
    Sub150DB114 *s;
    f32 damp;
    s16 t;
    f32 max;
    f32 min;

    max = 1.2f;
    min = 0.8f;
    damp = arg0->sub.unk24;
    t = arg0->unk5C;
    s = &arg0->sub;
    s->vel[0] *= damp;
    s->vel[1] *= damp;
    s->vel[2] *= damp;
    arg0->pos[0] += s->vel[0] * D_800BE9A4;
    arg0->pos[1] += s->vel[1] * D_800BE9A4;
    arg0->pos[2] += s->vel[2] * D_800BE9A4;
    arg0->sub.unk14 += arg0->sub.unk1C * D_800BE9A4;
    if (arg0->sub.unk14 > max) {
        s->unk14 = max;
        s->unk1C = -s->unk1C;
    } else if (s->unk14 < min) {
        s->unk14 = min;
        s->unk1C = -s->unk1C;
    }
    s->unk18 += s->unk20 * D_800BE9A4;
    if (s->unk18 > max) {
        s->unk18 = max;
        s->unk20 = -s->unk20;
    } else if (s->unk18 < min) {
        s->unk18 = min;
        s->unk20 = -s->unk20;
    }
    s->unkC += s->unk10 * D_800BE9A4;
    arg0->unk2C = s->unkC * s->unk14;
    arg0->unk30 = s->unkC * s->unk18;
    t -= s->unk28;
    if (t < 0) {
        arg0->unk5C = 0;
        return 0;
    }
    arg0->unk5C = t;
    return 1;
}

s16 *func_150DB2D8(struct Foo150DB2D8 *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f2;
    f32 temp_f12;
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

    temp_f2 = arg0->unk2C * arg0->unk40;
    temp_f12 = -arg0->unk2C * arg0->unk48;

    var_v0[0] = arg0->unk34 + temp_f2;
    var_v0[1] = arg0->unk38 + arg0->unk30;
    var_v0[2] = arg0->unk3C + temp_f12;
    var_v0[3] = 0;
    var_v0[8] = arg0->unk34 + temp_f2;
    var_v0[9] = arg0->unk38 - arg0->unk30;
    var_v0[10] = arg0->unk3C + temp_f12;
    var_v0[11] = 0;
    var_v0[16] = arg0->unk34 - temp_f2;
    var_v0[17] = arg0->unk38 - arg0->unk30;
    var_v0[18] = arg0->unk3C - temp_f12;
    var_v0[19] = 0;
    var_v0[24] = arg0->unk34 - temp_f2;
    var_v0[25] = arg0->unk38 + arg0->unk30;
    var_v0[26] = arg0->unk3C - temp_f12;
    var_v0[27] = 0;

    return ret;
}

typedef struct {
    u8  unk0;
    u8  unk1;
    s16 unk2;
    s16 unk4;
    u8  pad6[2];
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  pad14[0x2C];
    s32 unk40;
    u8  unk44;
    u8  unk45;
    u8  pad46[0x12];
} Struct150DB518;


void func_150DB518(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, u8 arg6, s32 arg7) {
    Struct150DB518 sp38;
    Struct150DB518Obj *temp_v0;

    if (arg0 != 0 && arg1 != 0 && arg2 != 0 && arg3 != 0 && arg4 != 0 && arg5 != 0) {
        sp38.unk0 = 0x17;
        sp38.unkC = 0x243A;
        sp38.unk40 = 0x401;
        sp38.unk44 = 0xFF;
        sp38.unk8 = 0;
        sp38.unk4 = arg5;
        sp38.unk45 = 0xFF;
        *((u8 *)&sp38 + 0x12) = 0xC8; *((u8 *)&sp38 + 0x11) = 0xC8; *((u8 *)&sp38 + 0x10) = 0xC8;
        *((u8 *)&sp38 + 0x13) = 0xC8;
        sp38.unk2 = 0x401;

        temp_v0 = func_1513D524((s32)&sp38, 0, 0xA, 0, 8, 0, 0x14, arg6, arg7);
        if (temp_v0 != 0) {
            temp_v0->unk110 = arg0;
            temp_v0->unk114 = arg1;
            temp_v0->unk118 = arg2;
            temp_v0->unk11C = arg4;
            temp_v0->unk120 = arg3;
        }
    }
}

s32 func_150DB630(void *arg0) {
    f32 temp_f0;
    s32 ret;

    ret = 1;
    if (255.0f < **(f32 * volatile *)((u8 *)arg0 + 0x120)) {
        *(u8 *)((u8 *)arg0 + 0x5C) = 0xFF;
    } else {
        temp_f0 = **(f32 * volatile *)((u8 *)arg0 + 0x120);
        if (temp_f0 < 0.0f) {
            *(u8 *)((u8 *)arg0 + 0x5C) = 0;
        } else {
            *(u8 *)((u8 *)arg0 + 0x5C) = (u32)temp_f0;
        }
    }
    return ret;
}

typedef struct Vtx150DB714 {
    s16 x, y, z, flag;
    u8 attributes[8];
} Vtx150DB714;

typedef struct Values150DB714 {
    f32 *data[4];
} Values150DB714;

typedef struct Owner150DB714 {
    u8 pad0[0xC0];
    Vtx150DB714 template[4];
    u8 *buffers[4];
    Values150DB714 values;
} Owner150DB714;

extern f32 D_800A0BF0;

void *func_150DB714(Owner150DB714 *owner, s16 bufferIndex) {
    void *vertices;
    void *result;
    f32 cosine;
    s32 angle;
    f32 sine;
    f32 offsetX;
    f32 offsetZ;
    Values150DB714 *values;
    u8 fresh;

    func_151D5D60(owner->buffers, bufferIndex, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(owner->buffers[bufferIndex], owner->template, 0x40U);
            memcpy(owner->buffers[bufferIndex] + 0x40, owner->template, 0x40U);
        }
    } else {
        return 0;
    }
    angle = (s32)(*(owner->values.data[1]) * D_800A0BF0);
    values = (Values150DB714 *)((u8 *)owner + 0x110);
    sine = func_151423D8((u8)angle);
    cosine = func_151423D8((u8)(angle - 0x40));
    offsetX = *values->data[2] * sine;
    offsetZ = -*values->data[2] * cosine;
    ((Vtx150DB714 *)vertices)[0].x = (s32)(values->data[0][0] + offsetX);
    ((Vtx150DB714 *)vertices)[0].y = (s32)(values->data[0][1] + *values->data[3]);
    ((Vtx150DB714 *)vertices)[0].z = (s32)(values->data[0][2] + offsetZ);
    ((Vtx150DB714 *)vertices)[0].flag = 0;
    ((Vtx150DB714 *)vertices)[1].x = (s32)(values->data[0][0] + offsetX);
    ((Vtx150DB714 *)vertices)[1].y = (s32)(values->data[0][1] - *values->data[3]);
    ((Vtx150DB714 *)vertices)[1].z = (s32)(values->data[0][2] + offsetZ);
    ((Vtx150DB714 *)vertices)[1].flag = 0;
    ((Vtx150DB714 *)vertices)[2].x = (s32)(values->data[0][0] - offsetX);
    ((Vtx150DB714 *)vertices)[2].y = (s32)(values->data[0][1] - *values->data[3]);
    ((Vtx150DB714 *)vertices)[2].z = (s32)(values->data[0][2] - offsetZ);
    ((Vtx150DB714 *)vertices)[2].flag = 0;
    ((Vtx150DB714 *)vertices)[3].x = (s32)(values->data[0][0] - offsetX);
    ((Vtx150DB714 *)vertices)[3].y = (s32)(values->data[0][1] + *values->data[3]);
    ((Vtx150DB714 *)vertices)[3].z = (s32)(values->data[0][2] - offsetZ);
    ((Vtx150DB714 *)vertices)[3].flag = 0;
    return result;
}
