#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u8 padC[0x10 - 0xC];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Sub1512DD50A;

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ f32 start[3];
    /* 0x10 */ f32 dir[3];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 speed;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
} Sub1512DD50B;

typedef struct {
    /* 0x000 */ u8 pad0[0x6FC];
    /* 0x6FC */ Sub1512DD50A a;
    /* 0x730 */ u8 pad730[0x870 - 0x730];
    /* 0x870 */ Sub1512DD50B b;
} Obj1512DD50;

void func_1512DD50(Obj1512DD50 *arg0) {
    Sub1512DD50B *p0;
    f32 z;
    Sub1512DD50A *p1;
    f32 x;
    f32 y;
    f32 sc;
    f32 b;
    struct17 tmp;
    f32 c;
    f32 d;

    p0 = &arg0->b;
    p1 = &arg0->a;
    x = p1->unk10;
    y = p1->unk12;
    z = p1->unk14;
    sc = p1->unk16;
    c = p1->unk24 * sc;
    b = p1->unk28 * sc;
    sc = c;
    c = p1->unk2C * c;
    d = -sc * p1->unk30;
    p0->start[0] = x - c;
    p0->start[1] = y - b;
    p0->start[2] = z - d;
    tmp.unk0 = 2.0f * c;
    tmp.unk4 = 2.0f * b;
    tmp.unk8 = 2.0f * d;

    sc = 1.0f / func_150AD930(&tmp.unk0);
    p0->unk1C = sc;
    p0->dir[0] = sc * tmp.unk0;
    p0->dir[1] = sc * tmp.unk4;
    p0->dir[2] = sc * tmp.unk8;
    p0->speed = p1->unk20;
    p0->unk24 = p1->unk8;
    p0->type = p1->unk1C;
    p0->unk28 = 0.0f;
    p0->unk2C = 0.0f;
}
// void func_1512DD50(struct108 *arg0) {
//     s32 pad;
//     struct17 tmp;
//
//     f32 temp_f0;
//     f32 temp_f0_2;
//     f32 temp_f12;
//     f32 temp_f14;
//     f32 temp_f2;
//
//     struct127 *temp_v0;
//     struct164 *temp_v1;
//
//     temp_f2 = arg0->unk712;
//     temp_f0 = arg0->unk720 * temp_f2;
//     temp_f12 = arg0->unk724 * temp_f2;
//     temp_f0_2 = arg0->unk728 * temp_f0;
//     temp_f14 = -temp_f0 * arg0->unk72C;
//
//     arg0->unk874 = arg0->unk70C - temp_f0_2;
//     arg0->unk878 = arg0->unk70E - temp_f12;
//     arg0->unk87C = arg0->unk710 - temp_f14;
//
//     tmp.unk0 = 2.0f * temp_f0_2;
//     tmp.unk4 = 2.0f * temp_f12;
//     tmp.unk8 = 2.0f * temp_f14;
//
//     temp_f2 = 1.0f / func_150AD930(&tmp);
//     temp_v0 = arg0->unk870;
//     temp_v0->unk1C = temp_f2;
//     temp_v0->unk10 = temp_f2 * tmp.unk0;
//     temp_v0->unk14 = temp_f2 * tmp.unk4;
//     temp_v0->unk18 = temp_f2 * tmp.unk8;
//     temp_v1 = arg0->unk6FC;
//     temp_v0->unk20 = temp_v1->unk20;
//     temp_v0->unk24 = temp_v1->unk8;
//     temp_v0->unk28 = 0.0f;
//     temp_v0->unk2C = 0.0f;
//     temp_v0->unk0 = (s8) temp_v1->unk1C;
// }

typedef struct {
    f32 x, y, z;
} Vec3f1512DEA4;

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ Vec3f1512DEA4 start;
    /* 0x10 */ Vec3f1512DEA4 dir;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 speed;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ Vec3f1512DEA4 pos;
} Sub1512DEA4;

typedef struct {
    /* 0x000 */ u8 pad0[0x23C];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D[0x2BC - 0x23D];
    /* 0x2BC */ f32 unk2BC[3];
    /* 0x2C8 */ u8 pad2C8[0x2F8 - 0x2C8];
    /* 0x2F8 */ f32 unk2F8[3];
    /* 0x304 */ u8 pad304[0x3C0 - 0x304];
    /* 0x3C0 */ f32 unk3C0[3];
    /* 0x3CC */ u8 pad3CC[0x7B4 - 0x3CC];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x870 - 0x7B8];
    /* 0x870 */ Sub1512DEA4 sub;
} Obj1512DEA4;

#define SQ1512(x) ((x) * (x))
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

void func_1512DEA4(Obj1512DEA4 *arg0) {
    Sub1512DEA4 *s;
    f32 dist;
    f32 vel[3];
    f32 x, y, z;
    f32 dx;
    f32 dy;
    f32 speed;
    f32 pa, pb, pc, pd;
    f32 dz;

    x = arg0->sub.start.x;
    dx = x - arg0->unk2BC[0];
    y = arg0->sub.start.y;
    dy = y - arg0->unk2BC[1];
    z = arg0->sub.start.z;
    dz = z - arg0->unk2BC[2];
    s = &arg0->sub;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    s->pos.x = (f32)x + s->dir.x * dist;
    s->pos.y = (f32)y + s->dir.y * dist;
    s->pos.z = (f32)z + s->dir.z * dist;
    vel[1] = arg0->sub.unk24;
    switch (arg0->sub.type) {
        case 0: {
            speed = s->speed;
            vel[0] = (f32)speed * s->dir.z;
            vel[2] = (f32)speed * -s->dir.x;
            break;
        }
        case 1: {
            speed = s->speed;
            vel[0] = (f32)speed * -s->dir.z;
            vel[2] = (f32)speed * s->dir.x;
            break;
        }
        case 2: {
            speed = s->speed;
            vel[0] = (f32)speed * s->dir.x;
            vel[2] = (f32)speed * s->dir.z;
            break;
        }
        case 3: {
            speed = s->speed;
            vel[0] = (f32)speed * -s->dir.x;
            vel[2] = (f32)speed * -s->dir.z;
            break;
        }
    }
    s->pos.x += vel[0];
    s->pos.y += vel[1];
    s->pos.z += vel[2];
    if (arg0->unk23C) {
        arg0->unk2F8[0] = s->pos.x;
        arg0->unk2F8[1] = s->pos.y;
        arg0->unk2F8[2] = s->pos.z;
        s->unk28 = 40.0f;
        s->unk2C = 40.0f;
        return;
    }
    func_150495B0(&s->unk28, 10.0f, &s->unk30, 0.6000000238f, 1.399999976f, arg0->unk7B4);
    func_150495B0(&s->unk2C, 10.0f, &s->unk34, 0.6000000238f, 1.399999976f, arg0->unk7B4);
    func_150495B0(&arg0->unk2F8[0], s->pos.x, &arg0->unk3C0[0], s->unk28, s->unk2C, arg0->unk7B4);
    func_150495B0(&arg0->unk2F8[1], s->pos.y, &arg0->unk3C0[1], s->unk28, s->unk2C, arg0->unk7B4);
    func_150495B0(&arg0->unk2F8[2], s->pos.z, &arg0->unk3C0[2], s->unk28, s->unk2C, arg0->unk7B4);
}
