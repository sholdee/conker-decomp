#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800AA418;
struct127 *func_15083E90(s32);
struct260 *func_15149130(s16 arg0, s8 arg1, s8 arg2, s8 arg3, u8 arg4, u8 arg5, struct37 *arg6, u8 arg7, s32 arg8);

typedef struct {
    u8 pad0[0x580];
    s32 unk580;
    u8 unk584;
    s32 unk588;
    u8 unk58C;
} SubA151B5A9C;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB151B5A9C;

s32 func_151B5240(s16 arg0, u8 arg1, s32 arg2) {
    struct260 *temp_v0;
    struct {
        f64 pad0[0xB0];
        s32 unk580;
        u8 unk584;
        s32 unk588;
        u8 unk58C;
        f32 unk590;
        f32 unk594;
        f32 unk598;
        f32 unk59C;
        f32 unk5A0;
        u8 unk5A4[4];
    } tmp;

    tmp.unk584 = 0x17;
    tmp.unk58C = 0x16;
    tmp.unk580 = (s32)func_15083E90(0x17);
    tmp.unk588 = (s32)func_15083E90(tmp.unk58C);
    tmp.unk598 = 250.0f;
    tmp.unk590 = 18.0f;
    tmp.unk594 = 0.0f;
    tmp.unk5A0 = D_800AA418;
    tmp.unk59C = 1.0f / tmp.unk598;

    temp_v0 = func_15149130(arg0, -1, -1, 1, 1, 0x15, (struct37 *)0x5A8, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x28, &tmp, 0x5A8);
    }
    return (s32)temp_v0;
}

typedef struct {
    s32 first;
    s32 second;
    u8 firstFlag;
    u8 secondFlag;
} Game1E26F0Pair;

typedef struct {
    Vtx_t vertices[2][44];
    s32 first;
    u8 firstFlag;
    u8 pad585[3];
    s32 second;
    u8 secondFlag;
    u8 pad58D[3];
    f32 width;
    f32 fadeStart;
    f32 fadeEnd;
    f32 fadeScale;
    f32 fadeMax;
} Game1E26F0State;

void func_15143134(f32 *, f32 *, s32);
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142C10(Gfx *, s32, s32, s32, s32, u8 *);
Gfx *func_15142CF0(Gfx *, s32, s32, s32, s32, s32, s32, u8 *);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_15143E64(void *);
typedef struct { f32 x; f32 y; f32 z; } Vec3f;
extern u8 D_8008FB80;
extern u8 D_8008FB84;
extern u8 D_80090F68[];
extern s32 D_800A4AC8[][2];
extern f32 D_800AA400[];
extern f32 D_800AA40C[];
extern s32 D_800D2C9C;

Gfx *func_151B5328(Gfx *gdl, u8 *arg1, s16 arg2) {
    Game1E26F0State *state;
    u8 failed;
    u8 changed;
    s32 i;
    Vtx_t *vtx;
    Vec3f *pos;
    f32 lenSq;
    f32 inv;
    Vec3f perp;
    f32 w;
    Vec3f start;
    Vec3f end;
    Vec3f dir;
    f32 dx, dy, dz;
    Vec3f cur;
    f32 t;
    f32 count;
    f32 length;
    f32 fade;
    f32 stepX, stepY, stepZ;
    f32 angle;
    f32 mag;
    f32 wave;

    failed = 0;
    state = (Game1E26F0State *)(arg1 + 0x28);
    if (*(s32 *)state->first == 0 || state->firstFlag != *(u8 *)(state->first + 0x3B)) {
        failed = 1;
    }
    if (*(s32 *)state->second == 0 || state->secondFlag != *(u8 *)(state->second + 0x3B)) {
        failed = 1;
    }
    if (!failed && *(s32 *)(state->first + 0x1D4) != 0 && *(s32 *)(state->second + 0x1D4) != 0) {
        func_15143134(D_800AA400, &start.x, *(s32 *)(state->first + 0x1D4) + (D_8008FB80 << 6));
        func_15143134(D_800AA40C, &end.x, *(s32 *)(state->second + 0x1D4) + (D_8008FB84 << 6));
        changed = 1;
        vtx = state->vertices[D_800BE9C0];
        pos = (Vec3f *)&D_800DBFF0[arg2].unk2F8;
        gdl = func_15142B7C(gdl, 0x200005, 0x60600);
        gdl = (Gfx *)func_15142E24((s32)gdl, (s32)D_80090F68, 0, 0, 0, 0, 0x56, 0, 0, &changed, 3);
        gdl = func_15142C10(gdl, 0xFF, 0xFF, 0xFF, 0xFF, &changed);
        gdl = func_15142CF0(gdl, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, &changed);
        gdl = func_1513F4E4(gdl, 0x2E, &changed);
        gdl = func_15142FBC(gdl, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[5][0] | D_800A4AC8[5][1], &changed);
        dir.x = end.x - start.x;
        dir.y = end.y - start.y;
        dir.z = end.z - start.z;
        length = func_15143E64(&dir);
        dx = dir.x * 0.5f + start.x - pos->x;
        dy = dir.y * 0.5f + start.y - pos->y;
        dz = dir.z * 0.5f + start.z - pos->z;
        perp.x = dir.y * dz - dy * dir.z;
        perp.y = dir.z * dx - dz * dir.x;
        perp.z = dir.x * dy - dx * dir.y;
        lenSq = perp.x * perp.x + perp.y * perp.y + perp.z * perp.z;
        if (lenSq == 0.0f) {
            perp.x = 0.0f;
            perp.y = 0.0f;
            perp.z = 0.0f;
        } else {
            mag = sqrtf(lenSq);
            inv = state->width / mag;
            perp.x *= inv;
            perp.y *= inv;
            perp.z *= inv;
        }
        stepX = dir.x * 0.1f;
        stepY = dir.y * 0.1f;
        stepZ = dir.z * 0.1f;
        if (length < state->fadeStart) {
            fade = 0.0f;
        } else if (length < state->fadeEnd) {
            fade = (length - state->fadeStart) * state->fadeScale * state->fadeMax;
        } else {
            fade = state->fadeMax;
        }
        i = 0;
        cur = start;
        angle = 0.0f;
        t = 0.0f;
        count = 0.0f;
        do {
            wave = sinf(angle);
            w = 1.0f - wave * fade;
            vtx[0].ob[0] = perp.x * w + cur.x;
            vtx[0].ob[1] = perp.y * w + cur.y;
            vtx[0].ob[2] = perp.z * w + cur.z;
            vtx[0].cn[0] = 0xFF;
            vtx[0].cn[1] = 0;
            vtx[0].cn[2] = 0;
            vtx[0].cn[3] = 0xFF;
            vtx[0].tc[0] = 0;
            vtx[0].tc[1] = t;
            vtx[1].ob[0] = cur.x - perp.x * w;
            vtx[1].ob[1] = cur.y - perp.y * w;
            vtx[1].ob[2] = cur.z - perp.z * w;
            vtx[1].cn[0] = 0xFF;
            vtx[1].cn[1] = 0;
            vtx[1].cn[2] = 0;
            vtx[1].cn[3] = 0xFF;
            vtx[1].tc[0] = 0x7C0;
            vtx[1].tc[1] = t;
            vtx += 2;
            wave = sinf(angle + 0.31415927f);
            w = 1.0f - wave * fade;
            vtx[0].ob[0] = cur.x + stepX - perp.x * w;
            vtx[0].ob[1] = cur.y + stepY - perp.y * w;
            vtx[0].ob[2] = cur.z + stepZ - perp.z * w;
            vtx[0].cn[0] = 0xFF;
            vtx[0].cn[1] = 0;
            vtx[0].cn[2] = 0;
            vtx[0].cn[3] = 0xFF;
            vtx[0].tc[0] = 0x7C0;
            vtx[0].tc[1] = t + 204.8f;
            vtx[1].ob[0] = cur.x + stepX + perp.x * w;
            vtx[1].ob[1] = cur.y + stepY + perp.y * w;
            vtx[1].ob[2] = cur.z + stepZ + perp.z * w;
            vtx[1].cn[0] = 0xFF;
            vtx[1].cn[1] = 0;
            vtx[1].cn[2] = 0;
            vtx[1].cn[3] = 0xFF;
            vtx[1].tc[0] = 0;
            vtx[1].tc[1] = t + 204.8f;
            vtx += 2;
            gSPVertex(gdl++, vtx - 4, 4, 0);
            gSP1Triangle(gdl++, 0, 1, 2, 0);
            gSP1Triangle(gdl++, 0, 2, 3, 0);
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            angle += 0.31415927f;
            t += 204.8f;
            count += 0.1f;
            i++;
        } while (i != 10);
    }
    if (failed) {
        *(s16 *)(arg1 + 0xE) = -1;
    }
    return gdl;
}

void func_151B5A9C(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151B5A9C *temp_v0 = (SubA151B5A9C *)((u8 *)arg0 + 0x28);
    ArgB151B5A9C *b = (ArgB151B5A9C *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk580 == b->unk0) {
            temp_v0->unk580 = b->u4.w4;
            temp_v0->unk584 = b->unk9;
        } else {
            if (temp_v0->unk580 == b->u4.w4) {
                temp_v0->unk580 = b->unk0;
                temp_v0->unk584 = b->unk8;
            }
        }

        if (temp_v0->unk588 == b->unk0) {
            temp_v0->unk588 = b->u4.w4;
            temp_v0->unk58C = b->unk9;
        } else {
            if (temp_v0->unk588 == b->u4.w4) {
                temp_v0->unk588 = b->unk0;
                ((volatile SubA151B5A9C *)temp_v0)->unk58C = b->unk8;
            }
trailing_label_151B5A9C:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk580) || (b0 == temp_v0->unk588) || (b->u4.b4 == temp_v0->unk584) || (b->u4.b4 == temp_v0->unk58C)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
