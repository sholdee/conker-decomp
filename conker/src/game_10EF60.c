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

/* A tracer/beam effect: a textured quad stretched from its position along its direction. */
typedef struct {
    u8 pad0[0x10];
    Vtx_t quads[2][4]; // double-buffered by D_800BE9C0
    s16 lifetime;
    u8 pad92[2];
    f32 x, y, z;
    f32 pitch, yaw;
    f32 speed;    // distance per frame; 0 means random
    f32 stopDist; // nonzero: stop on reaching the target
    f32 target_x, target_y, target_z;
    f32 halfWidth;
    f32 length;
    f32 dx, dy, dz; // unit direction
    s8 mode;        // nonzero: re-fired from a bone of the owner
    u8 padD1;
    s16 onTime;
    s16 offTime;
    s16 timer;      // drawn while positive
    s16 texture;    // index into D_8008CA4C
    u8 ownerId;     // owner's unique_id when attached
    u8 drawn;
    struct127 *owner;
    s16 ownerTag;   // compared with owner->unk84.uh; -1 for none
    u8 bone;
    u8 attachDelay;
    f32 startOffset[3]; // in bone space
    f32 endOffset[3];
    u8 impactEffect;
    u8 fireCounter;
    u8 unkFE;
    u8 padFF;
} Game10EF60Effect;

typedef struct {
    u8 pad0[6];
    u16 width;
    u16 height;
} Game10EF60Texture;

extern Game10EF60Texture *D_8008CA4C[];
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Spawns a tracer effect from (x0, y0, z0) towards (x1, y1, z1). */
void func_150E1AB0(s32 arg0, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 speed, f32 stopDist,
                   f32 halfWidth, f32 length, s16 lifetime, u16 texture, s8 mode, s16 onTime, s16 offTime,
                   struct127 *owner, u8 ownerId, u8 bone, s32 ownerTag, u8 impactEffect, u8 unkFE,
                   f32 startX, f32 startY, f32 startZ, f32 endX, f32 endY, f32 endZ) {
    Game10EF60Effect *effect;
    s32 i;
    s32 sMax;
    s32 tMax;

    effect = func_15167A68(0xB, 0, 0x100, 1, 0xFF, 1);
    if (effect != NULL) {
        effect->lifetime = lifetime;
        effect->x = x0;
        effect->y = y0;
        effect->z = z0;
        effect->yaw = func_150484A0(-(x1 - x0), -(z1 - z0));
        effect->pitch = func_150484A0(y1 - y0, sqrtf((x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0)));
        effect->dx = 0.0f;
        effect->dy = 0.0f;
        effect->dz = 0.0f;
        effect->target_x = x1;
        effect->target_y = y1;
        effect->target_z = z1;
        effect->stopDist = stopDist;
        effect->speed = speed;
        effect->halfWidth = halfWidth;
        effect->length = length;
        effect->attachDelay = 0;
        effect->impactEffect = impactEffect;
        effect->fireCounter = 0;
        effect->unkFE = unkFE;
        effect->owner = owner;
        effect->texture = texture;
        effect->mode = mode;
        if (effect->mode != 0) {
            if (owner == NULL) {
                func_1516979C((struct102 *)effect);
                return;
            }
            effect->onTime = onTime;
            effect->timer = -offTime;
            effect->offTime = offTime;
            effect->ownerId = ownerId;
            effect->bone = bone;
            effect->ownerTag = ownerTag;
            effect->startOffset[0] = startX;
            effect->startOffset[1] = startY;
            effect->startOffset[2] = startZ;
            effect->endOffset[0] = endX;
            effect->endOffset[1] = endY;
            effect->endOffset[2] = endZ;
        } else {
            effect->timer = 1;
            effect->ownerTag = -1;
        }
        for (i = 0; i < 2; i++) {
            sMax = D_8008CA4C[effect->texture]->width - 1;
            tMax = D_8008CA4C[effect->texture]->height - 1;
            effect->quads[i][2].tc[0] = 0x2000;
            effect->quads[i][2].tc[1] = 0x2000;
            effect->quads[i][2].flag = 0;
            effect->quads[i][0].tc[0] = (u16)(sMax << 5) + 0x2000;
            effect->quads[i][0].tc[1] = 0x2000;
            effect->quads[i][0].flag = 0;
            effect->quads[i][3].tc[0] = 0x2000;
            effect->quads[i][3].tc[1] = (u16)(tMax << 5) + 0x2000;
            effect->quads[i][3].flag = 0;
            effect->quads[i][1].tc[0] = (u16)(sMax << 5) + 0x2000;
            effect->quads[i][1].tc[1] = (u16)(tMax << 5) + 0x2000;
            effect->quads[i][1].flag = 0;
        }
        effect->drawn = 0;
    }
}

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 unk3;
    f32 x, y, z;
    f32 dir[3];
} Game10EF60Impact;

extern Game10EF60Impact D_800D9960[];
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, u32 *, s32, f32 *, f32 *, f32 *, s32, s32, s32, f32);
s32 func_150E1570(struct127 *, f32, f32, f32, f32, f32, f32, s32, s32);
void func_150E4550(f32, f32, f32, s32, s32, s32, s32);
s32 func_1510F8CC(s32);
s32 func_151EF610(void);


void func_150E1D14(Game10EF60Effect *arg0) {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 a;
    f32 b;
    f32 c;
    f32 sn;
    f32 cs;
    f32 hit;
    f32 dist;
    f32 speed;
    f32 *bone;
    s32 idx;
    s32 type;
    s32 fire;
    s32 surf;
    s16 p;
    f32 dir[3];
    struct17 out;
    f32 y0;
    f32 z0;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 s;
    u32 tri;
    s32 ang;
    s32 i;

    surf = (s32)D_800D9960[idx].dir;
    fire = arg0->owner->unk76 - arg0->owner->unk31C->unk12;
    i = (s32)&arg0->target_x;
    bone = &arg0->target_z;
    type = 0x23;
    dist = 0.0f;
    arg0->lifetime -= D_800BE9E4;
    if (arg0->lifetime < 0 || (arg0->owner != NULL && arg0->ownerTag != -1 && arg0->owner->unk84.uh != arg0->ownerTag)) {
        func_1516972C((struct102 *)arg0);
        return;
    }
    if (arg0->mode != 0) {
        if (arg0->attachDelay == 0) {
            if (arg0->owner != NULL && arg0->owner->unk1D4 != NULL && arg0->owner->unique_id == arg0->ownerId && (idx = arg0->owner->unk127) != 0xFF) {
                bone = (f32 *)((u8 *)arg0->owner->unk1D4 + (arg0->bone << 6));
                func_150A7960(bone, arg0->startOffset[0], arg0->startOffset[1], arg0->startOffset[2], &arg0->x, &arg0->y, &arg0->z);
                func_150A7960(bone, arg0->endOffset[0], arg0->endOffset[1], arg0->endOffset[2], &arg0->target_x, &arg0->target_y, &arg0->target_z);
                arg0->attachDelay = 3;
            }
        } else {
            arg0->attachDelay--;
        }
        arg0->timer -= D_800BE9E4;
        if (arg0->timer < -arg0->offTime) {
            arg0->timer = arg0->onTime;
            if (arg0->owner != NULL && arg0->owner->unk1D4 != NULL && arg0->owner->unique_id == arg0->ownerId) {
                bone = (f32 *)((u8 *)arg0->owner->unk1D4 + (arg0->bone << 6));
                func_150A7960(bone, arg0->startOffset[0], arg0->startOffset[1], arg0->startOffset[2], &arg0->x, &arg0->y, &arg0->z);
                idx = arg0->owner->unk127;
                func_150A7960(bone, arg0->endOffset[0], arg0->endOffset[1], arg0->endOffset[2], &arg0->target_x, &arg0->target_y, &arg0->target_z);
                hit = 0.0f;
                if (arg0->fireCounter-- == 0) {
                    arg0->fireCounter = 2;
                    fire = 1;
                } else {
                    fire = 0;
                }
                if (arg0->speed == 0.0f) {
                    speed = (f32)(func_151EF610() % 70) + 40.0f;
                } else {
                    speed = arg0->speed;
                }
                surf = 0;
                if (idx != 0xFF) {
                    if ((arg0->owner->unk76 - arg0->owner->unk31C->unk12) == D_800D9960[idx].unk0 && arg0->owner->unk1D1 == D_800D9960[idx].unk2) {
                        arg0->target_x = D_800D9960[idx].x;
                        arg0->target_y = D_800D9960[idx].y;
                        arg0->target_z = D_800D9960[idx].z;
                        if (D_800BE616 == 0) {
                            if (func_151EF610() % 100 < D_800D9960[idx].unk3) {
                                dir[0] = arg0->target_x - arg0->x;
                                dir[1] = arg0->target_y - arg0->y;
                                dir[2] = arg0->target_z - arg0->z;
                                func_15049148((struct17 *)D_800D9960[idx].dir, 2.0f * func_150AD900(D_800D9960[idx].dir, dir), &out);
                                func_15048F58((struct17 *)dir, &out, &out);
                                out.unk0 += arg0->target_x;
                                out.unk4 += arg0->target_y;
                                out.unk8 += arg0->target_z;
                                func_150E1AB0(0, arg0->target_x, arg0->target_y, arg0->target_z, out.unk0, out.unk4, out.unk8, (f32)(func_151EF610() % 35) + 20.0f, 0.0f, 8.0f, 320.0f, 30, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                                func_10010F88(func_151EF610() % 6 + 0x2B, 10000, 0, 0, idx, arg0->target_x, arg0->target_y, arg0->target_z, 500, 2000);
                            }
                        }
                        hit = 1.0f;
                    } else {
                        p = arg0->owner->unk1D1 * 200;
                        func_1505A184((arg0->owner->unk76 - arg0->owner->unk31C->unk12), 2000.0f, (f32)p * 0.005493164f, &arg0->target_x, &arg0->target_z, &arg0->target_y);
                        arg0->target_x += arg0->x;
                        arg0->target_y += arg0->y;
                        arg0->target_z += arg0->z;
                    }
                    arg0->length = (f32)(func_151EF610() % 3) * 80.0f + 80.0f;
                } else {
                    if (fire) {
                        hx = arg0->target_x - arg0->x;
                        hy = arg0->target_y - arg0->y;
                        hz = arg0->target_z - arg0->z;
                        s = (75.0f * speed) / sqrtf(hx * hx + hy * hy + hz * hz);
                        if (func_150AC9C0(arg0->x, arg0->y, arg0->z, hx * s, hy * s, hz * s, &tri, 0, &hx, &hy, &hz, 0, 0, 0, 0.0f)) {
                            arg0->target_x = hx;
                            arg0->target_y = hy;
                            arg0->target_z = hz;
                            hit = 1.0f;
                            i = (tri - D_800DBE3C) / 12;
                            if (i >= 0 && i < D_800DBE4C && D_800DBE5C != 0) {
                                surf = func_1510F8CC(((s32 *)D_800DBE5C)[i]);
                            }
                        }
                    }
                    type = 0x43;
                }
                arg0->yaw = func_150484A0(-(arg0->target_x - arg0->x), -(arg0->target_z - arg0->z));
                arg0->pitch = func_150484A0(arg0->target_y - arg0->y, sqrtf((arg0->target_x - arg0->x) * (arg0->target_x - arg0->x) + (arg0->target_z - arg0->z) * (arg0->target_z - arg0->z)));
                arg0->dx = 0.0f;
                arg0->dy = 0.0f;
                arg0->dz = 0.0f;
                if (fire) {
                    func_150E1AB0(0, arg0->x, arg0->y, arg0->z, arg0->target_x, arg0->target_y, arg0->target_z, speed, hit, 15.0f, 820.0f, 75, type, 0, 0, 0, arg0->owner, 0, 0, 0, surf, arg0->unkFE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                }
            } else {
                func_1516972C((struct102 *)arg0);
                return;
            }
        }
    } else {
    }
    if (arg0->timer > 0) {
        y0 = 0.0f;
        z0 = -1.0f;
        sn = func_150AD78C(arg0->pitch);
        cs = func_150AD780(arg0->pitch);
        a = y0 * cs;
        b = y0 * cs - sn * z0;
        c = y0 * sn + cs * z0;
        sn = func_150AD78C(arg0->yaw);
        cs = func_150AD780(arg0->yaw);
        vx = cs * a + sn * c;
        vy = b;
        vz = -sn * a + cs * c;
        if (arg0->stopDist != 0.0f) {
            dist = sqrtf((arg0->target_x - arg0->x) * (arg0->target_x - arg0->x) + (arg0->target_y - arg0->y) * (arg0->target_y - arg0->y) + (arg0->target_z - arg0->z) * (arg0->target_z - arg0->z));
        }
        if (arg0->drawn == 0) {
            vx *= 0.5f;
            vy *= 0.5f;
            vz *= 0.5f;
        }
        arg0->dx = vx;
        arg0->dy = vy;
        arg0->dz = vz;
        s = arg0->speed * D_800BE9E4;
        if (dist < s && arg0->stopDist != 0.0f) {
            vx *= dist;
            vy *= dist;
            vz *= dist;
            arg0->length = dist;
        } else {
            vx *= s;
            vy *= s;
            vz *= s;
        }
        if (arg0->unkFE == 1) {
            i = arg0->owner != NULL ? arg0->owner - D_800CC2D0 : 0;
            func_150E1570(arg0->owner, arg0->x, arg0->y, arg0->z, arg0->x + vx, arg0->y + vy, arg0->z + vz, i, 0);
        }
        arg0->x += vx;
        arg0->y += vy;
        arg0->z += vz;
        if (arg0->stopDist != 0.0f) {
            s = sqrtf((arg0->target_x - arg0->x) * (arg0->target_x - arg0->x) + (arg0->target_y - arg0->y) * (arg0->target_y - arg0->y) + (arg0->target_z - arg0->z) * (arg0->target_z - arg0->z));
            if (s < arg0->stopDist || dist < s) {
                if (arg0->impactEffect != 0) {
                    func_150E4550(arg0->target_x, arg0->target_y, arg0->target_z, 1, arg0->impactEffect, 0, 0xFF);
                }
                func_1516972C((struct102 *)arg0);
                return;
            }
            if (s < arg0->length) {
                arg0->length = s;
            }
        }
    }
}


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

    if (arg1->timer > 0) {
        points[0].x = arg1->x;
        points[0].y = arg1->y;
        points[0].z = arg1->z;
        points[1].x = arg1->x + arg1->length * arg1->dx;
        points[1].y = arg1->y + arg1->length * arg1->dy;
        points[1].z = arg1->z + arg1->length * arg1->dz;
        camera[0] = D_800DBFF0[arg2].unk2F8;
        camera[1] = D_800DBFF0[arg2].unk2FC;
        camera[2] = D_800DBFF0[arg2].unk300;
        range = arg1->halfWidth;
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
                arg1->quads[D_800BE9C0][i].ob[j] = corners[i][j];
            }
        }
        arg0 = (Gfx *)func_15094F70((s32)arg0, D_8008CA4C[arg1->texture], 0, &owner, 0, 0, 0, 2, 3);
        flag = 0;
        arg0 = func_15142FBC(arg0, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x504A50, &flag);
        gSPVertex(arg0++, arg1->quads[D_800BE9C0], 4, 0);
        gSP1Triangle(arg0++, 0, 1, 3, 0);
        gSP1Triangle(arg0++, 0, 3, 2, 0);
    }
    arg1->drawn = 1;
    return arg0;
}

s32 func_150E2DA4(s32 arg0, s32 arg1) {
    return arg0;
}

void func_150E2DB4(struct127 *a0, u8 a1, s16 a2, s32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, s16 a10, s16 a11, u16 a12, u8 a13) {
    func_150E1AB0(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 400.0f, a12, 39, 1, a10, a11, a0, a1, a2, a3, 0, a13, a4, a5, a6, a7, a8, a9);
}

void func_150E2EA4(struct127 *a0, s32 a1, s32 a2, s32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, s32 a10, s32 a11, s32 a12, f32 a13, f32 a14, s32 a15, f32 a16) {
    func_150E1AB0(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, a16, 0.0f, a13, a14, *(u16 *)((u8 *)&a12 + 2), 39, 1, *(s16 *)((u8 *)&a10 + 2), *(s16 *)((u8 *)&a11 + 2), a0, *((u8 *)&a1 + 3), *(s16 *)((u8 *)&a2 + 2), a3, 0, *((u8 *)&a15 + 3), a4, a5, a6, a7, a8, a9);
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
