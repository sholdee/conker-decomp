#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    s32 field_0x0;
    s32 field_0x4;
    u8 field_0x8;
    u8 field_0x9;
    u8 pad_0xA[0xD0];
    u8 field_0xDA;
    u8 pad_0xDB;
    s32 field_0xDC[1];
} CollisionPairRecord;

typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx150E1AB0;

typedef struct {
    u8 pad0[0x10];
    Vtx150E1AB0 v[2][4];
    s16 unk90;
    u8 pad92[2];
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    f32 unkB0;
    f32 unkB4;
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    s8 unkD0;
    u8 padD1;
    s16 unkD2;
    s16 unkD4;
    s16 unkD6;
    s16 unkD8;
    u8 unkDA;
    u8 unkDB;
    s32 unkDC;
    s16 unkE0;
    u8 unkE2;
    u8 unkE3;
    f32 unkE4;
    f32 unkE8;
    f32 unkEC;
    f32 unkF0;
    f32 unkF4;
    f32 unkF8;
    u8 unkFC;
    u8 unkFD;
    u8 unkFE;
} Obj150E1AB0;

struct Tex150E1AB0 {
    u8 pad0[6];
    u16 unk6;
    u16 unk8;
};

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
typedef struct {
    u8 pad0[6];
    u16 width;
    u16 height;
} Game10EF60Texture;

extern Game10EF60Texture *D_8008CA4C[];

void func_150E1AB0(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 argA, s16 argB, u16 argC, s8 argD, s16 argE, s16 argF, s32 arg10, u8 arg11, u8 arg12, s32 arg13, u8 arg14, u8 arg15, f32 arg16, f32 arg17, f32 arg18, f32 arg19, f32 arg1A, f32 arg1B) {
    Obj150E1AB0 *obj;
    s32 i;
    s32 s;
    s32 t;

    obj = func_15167A68(0xB, 0, 0x100, 1, 0xFF, 1);
    if (obj != NULL) {
        obj->unk90 = argB;
        obj->unk94 = arg1;
        obj->unk98 = arg2;
        obj->unk9C = arg3;
        obj->unkA4 = func_150484A0(-(arg4 - arg1), -(arg6 - arg3));
        obj->unkA0 = func_150484A0(arg5 - arg2, sqrtf((arg4 - arg1) * (arg4 - arg1) + (arg6 - arg3) * (arg6 - arg3)));
        obj->unkC4 = 0.0f;
        obj->unkC8 = 0.0f;
        obj->unkCC = 0.0f;
        obj->unkB0 = arg4;
        obj->unkB4 = arg5;
        obj->unkB8 = arg6;
        obj->unkAC = arg8;
        obj->unkA8 = arg7;
        obj->unkBC = arg9;
        obj->unkC0 = argA;
        obj->unkE3 = 0;
        obj->unkFC = arg14;
        obj->unkFD = 0;
        obj->unkFE = arg15;
        obj->unkDC = arg10;
        obj->unkD8 = argC;
        obj->unkD0 = argD;
        if (obj->unkD0 != 0) {
            if (arg10 == 0) {
                func_1516979C((struct102 *)obj);
                return;
            }
            obj->unkD2 = argE;
            obj->unkD6 = -argF;
            obj->unkD4 = argF;
            obj->unkDA = arg11;
            obj->unkE2 = arg12;
            obj->unkE0 = arg13;
            obj->unkE4 = arg16;
            obj->unkE8 = arg17;
            obj->unkEC = arg18;
            obj->unkF0 = arg19;
            obj->unkF4 = arg1A;
            obj->unkF8 = arg1B;
        } else {
            obj->unkD6 = 1;
            obj->unkE0 = -1;
        }
        for (i = 0; i < 2; i++) {
            s = D_8008CA4C[obj->unkD8]->width - 1;
            t = D_8008CA4C[obj->unkD8]->height - 1;
            obj->v[i][2].tc[0] = 0x2000;
            obj->v[i][2].tc[1] = 0x2000;
            obj->v[i][2].flag = 0;
            obj->v[i][0].tc[0] = (u16)(s << 5) + 0x2000;
            obj->v[i][0].tc[1] = 0x2000;
            obj->v[i][0].flag = 0;
            obj->v[i][3].tc[0] = 0x2000;
            obj->v[i][3].tc[1] = (u16)(t << 5) + 0x2000;
            obj->v[i][3].flag = 0;
            obj->v[i][1].tc[0] = (u16)(s << 5) + 0x2000;
            obj->v[i][1].tc[1] = (u16)(t << 5) + 0x2000;
            obj->v[i][1].flag = 0;
        }
        obj->unkDB = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_10EF60/func_150E1D14.s")

typedef struct {
    s16 pos[3];
    s16 zero;
    s16 u;
    s16 v;
    u8 padC[4];
} Game10EF60Vertex;

typedef struct {
    u8 pad0[0x10];
    Game10EF60Vertex quads[2][4];
    s16 lifetime;
    u8 pad92[2];
    f32 x, y, z;
    f32 pitch, yaw;
    f32 scale, speed;
    f32 target_x, target_y, target_z;
    f32 range, rate;
    f32 dx, dy, dz;
    volatile s8 mode;
    u8 padD1;
    s16 fieldD2, fieldD4, fieldD6;
    volatile s16 material;
    u8 fieldDA, fieldDB;
    s32 owner;
    s16 timeout;
    u8 fieldE2, fieldE3;
    f32 fieldE4, fieldE8, fieldEC, fieldF0, fieldF4, fieldF8;
    u8 fieldFC, fieldFD, fieldFE, padFF;
} Game10EF60Effect;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;
typedef struct Game10EF60Owner {
    u8 pad0[0x10];
    void *output;
} Game10EF60Owner;
s32 func_15094F70(s32, void *, s32, void *, s32, s32, s32, s32, s32);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern s32 D_800D2C9C;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

Gfx *func_150E28DC(Gfx *arg0, Game10EF60Effect *arg1, s16 arg2) {
    Vec3f points[2];
    f32 camera[3];
    f32 normal[3];
    f32 delta[3];
    s32 i;
    s32 j;
    f32 corners[4][3];
    f32 length;
    f32 scale;
    f32 temp;
    f32 distances[2];
    f32 range;
    Game10EF60Owner owner;
    u8 flag;

    if (arg1->fieldD6 > 0) {
        points[0].x = arg1->x;
        points[0].y = arg1->y;
        points[0].z = arg1->z;
        points[1].x = arg1->x + arg1->rate * arg1->dx;
        points[1].y = arg1->y + arg1->rate * arg1->dy;
        points[1].z = arg1->z + arg1->rate * arg1->dz;
        camera[0] = D_800DBFF0[arg2].unk2F8;
        camera[1] = D_800DBFF0[arg2].unk2FC;
        camera[2] = D_800DBFF0[arg2].unk300;
        range = arg1->range;
        for (i = 0; i < 2; i++) {
            distances[i] = (points[i].x - camera[0]) * (points[i].x - camera[0]) + (points[i].y - camera[1]) * (points[i].y - camera[1]) + (points[i].z - camera[2]) * (points[i].z - camera[2]);
        }
        temp = points[1].x;
        points[1].x = points[0].x;
        points[0].x = temp;
        temp = points[1].y;
        points[1].y = points[0].y;
        points[0].y = temp;
        temp = points[1].z;
        points[1].z = points[0].z;
        points[0].z = temp;
        delta[0] = camera[0] - arg1->x;
        delta[1] = camera[1] - arg1->y;
        delta[2] = camera[2] - arg1->z;
        normal[0] = delta[1] * arg1->dz - delta[2] * arg1->dy;
        normal[1] = delta[2] * arg1->dx - delta[0] * arg1->dz;
        normal[2] = delta[0] * arg1->dy - delta[1] * arg1->dx;
        for (i = 0; i < 2; i++) {
            delta[0] = points[i].x - camera[0];
            delta[1] = points[i].y - camera[1];
            delta[2] = points[i].z - camera[2];
            normal[0] = delta[1] * arg1->dz - delta[2] * arg1->dy;
            normal[1] = delta[2] * arg1->dx - delta[0] * arg1->dz;
            normal[2] = delta[0] * arg1->dy - delta[1] * arg1->dx;
            length = sqrtf(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
            if (length == 0.0f) {
                return arg0;
            }
            scale = range / length;
            normal[0] *= scale;
            normal[1] *= scale;
            normal[2] *= scale;
            corners[i * 2][0] = points[i].x + normal[0];
            corners[i * 2][1] = points[i].y + normal[1];
            corners[i * 2][2] = points[i].z + normal[2];
            corners[i * 2 + 1][0] = points[i].x - normal[0];
            corners[i * 2 + 1][1] = points[i].y - normal[1];
            corners[i * 2 + 1][2] = points[i].z - normal[2];
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 3; j++) {
                arg1->quads[D_800BE9C0][i].pos[j] = corners[i][j];
            }
        }
        arg0 = (Gfx *)func_15094F70((s32)arg0, D_8008CA4C[arg1->material], 0, &owner, 0, 0, 0, 2, 3);
        flag = 0;
        arg0 = func_15142FBC(arg0, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x504A50, &flag);
        gSPVertex(arg0++, arg1->quads[D_800BE9C0], 4, 0);
        gSP1Triangle(arg0++, 0, 1, 3, 0);
        gSP1Triangle(arg0++, 0, 3, 2, 0);
    }
    arg1->fieldDB = 1;
    return arg0;
}

s32 func_150E2DA4(s32 arg0, s32 arg1) {
    return arg0;
}

void func_150E1AB0(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, s16, u16, s8, s16, s16, s32, u8, u8, s32, u8, u8, f32, f32, f32, f32, f32, f32);

void func_150E2DB4(s32 a0, u8 a1, s16 a2, s32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, s16 a10, s16 a11, u16 a12, u8 a13) {
    func_150E1AB0(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 400.0f, a12, 39, 1, a10, a11, a0, a1, a2, a3, 0, a13, a4, a5, a6, a7, a8, a9);
}

void func_150E2EA4(struct127 *a0, s32 a1, s32 a2, s32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, s32 a10, s32 a11, s32 a12, f32 a13, f32 a14, s32 a15, f32 a16) {
    func_150E1AB0(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, a16, 0.0f, a13, a14, *(u16 *)((u8 *)&a12 + 2), 39, 1, *(s16 *)((u8 *)&a10 + 2), *(s16 *)((u8 *)&a11 + 2), (s32)a0, *((u8 *)&a1 + 3), *(s16 *)((u8 *)&a2 + 2), a3, 0, *((u8 *)&a15 + 3), a4, a5, a6, a7, a8, a9);
}

s32 func_150E2DA4(s32, s32);

s32 func_150E2F90(s32 a0, s32 a1, s16 a2) {
    return func_150E2DA4(a0, a2);
}

void func_150E2FC0(CollisionPairRecord *a0, CollisionPairRecord *a1, u8 a2) {
    s32 temp_v0;

    if (a2 == 0x2D) {
        if ((temp_v0 = a1->field_0x0) != a0->field_0xDC[0]) {
            goto check_second;
        }
        a0->field_0xDC[0] = a1->field_0x4;
        a0->field_0xDA = a1->field_0x9;
        return;
check_second:
        if ((u32)a1->field_0x4 != (u32)a0->field_0xDC[0]) {
            return;
        }
        a0->field_0xDC[0] = temp_v0;
        a0->field_0xDA = a1->field_0x8;
    }
}
