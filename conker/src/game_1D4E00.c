#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A8F68;
extern f32 D_800A8F6C;
extern f32 D_800A8F58;
extern f32 D_800A8F5C;
extern f32 D_800A8F60;
extern f32 D_800A8F64;
extern f32 D_800A8DE0;
extern void (*D_8008FA60[])(f32 *, void *, s32, u8, s32);

void func_1514373C(f32, f32, f32 *, f32 *);
s32 func_15046C80(f32 *, s32, f32, void *);
typedef struct {
    struct17 unk0;
    struct17 unkC;
} Point_151A7A90;

typedef struct {
    s16 unk0;
    s16 unk2;
} Src151A7950;

void *func_151A7950(Src151A7950 *, s32, u8, s32);
s32 func_151A8B20(void *, s16, s32, u8, s32);


typedef struct {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x60 - 0x14];
    Point_151A7A90 *unk60;
    Point_151A7A90 *unk64;
    f32 unk68;
    u8 unk6C[0x10];
    s32 unk7C;
    Point_151A7A90 unk80[1];
} Obj151A7950;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

void *func_151A7950(Src151A7950 *arg0, s32 arg1, u8 arg2, s32 arg3) {
    Obj151A7950 *obj;

    obj = func_15167A68(0x2D, arg3, arg0->unk2 * sizeof(Point_151A7A90) + arg1 + 0x80, 1, arg2, 1);
    if (obj == NULL) {
        return NULL;
    }
    memcpy((u8 *)obj + 0x10, arg0, 0x50);
    obj->unk64 = obj->unk80;
    obj->unk60 = obj->unk64 + arg0->unk2;
    obj->unk68 = 0.0f;
    obj->unk64[0].unk0.unk0 = 0.0f;
    obj->unk64[0].unk0.unk4 = 0.0f;
    obj->unk64[0].unk0.unk8 = 0.0f;
    obj->unk64[arg0->unk2 - 1].unk0.unk0 = 0.0f;
    obj->unk64[arg0->unk2 - 1].unk0.unk4 = 0.0f;
    obj->unk64[arg0->unk2 - 1].unk0.unk8 = 1.0f;
    bzero(obj->unk6C, 0x10);
    obj->unk7C = (obj->unk12 << 6) + 0x140;
    return obj;
}


typedef struct {
    u8  pad0[0x10];
    s16 unk10;
    s16 unk12;
    f32 unk14;
    f32 unk18;
    u8  unk1C;
    u8  pad1D[3];
    f32 unk20;
    u8  pad24[8];
    s8  unk2C;
    s8  unk2D;
    u8  pad2E[2];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    struct17 unk48;
    u8  pad54[4];
    f32 unk58;
    u8  pad5C[8];
    Point_151A7A90 *unk64;
    f32 unk68;
} Header_151A7A90;

extern s32 (*D_8008F940[])(void *);
extern s32 (*D_8008F948[])(void *);

void func_151A8340(void *, s16, s16, f32, s16);

void func_151A7A90(Header_151A7A90 *arg0) {
    Header_151A7A90 *hdr;
    u8 flag;
    struct17 scale;
    struct17 up;
    struct17 dir;
    s32 idx1;
    s32 idx2;

    hdr = arg0;
    flag = 0;
    if (arg0->unk1C & 1) {
        arg0->unk10 -= D_800BE9E4;
        if (arg0->unk10 < 0) {
            flag = 1;
        }
    }
    idx1 = arg0->unk2C;
    if (idx1 != -1) {
        if (!flag) {
            flag = (D_8008F940[idx1](arg0) == 0);
        }
    }
    idx2 = arg0->unk2D;
    if (idx2 != -1) {
        if (!flag) {
            flag = (D_8008F948[idx2](arg0) == 0);
        }
    }

    if (!flag) {
        arg0->unk68 -= D_800BE9A4;
        if (arg0->unk68 < 0.0f) {
            func_151A8340(arg0, 0, arg0->unk12 - 1, arg0->unk20, 100);
            arg0->unk68 = func_150ADA68() * arg0->unk18 + arg0->unk14;
        }
        if (arg0->unk1C & 2) {
            Point_151A7A90 *pt;

            dir.unk0 = arg0->unk3C - arg0->unk30;
            dir.unk4 = arg0->unk40 - arg0->unk34;
            dir.unk8 = arg0->unk44 - arg0->unk38;
            up = arg0->unk48;
            func_151450B4(&dir, &up, &scale);
            {
                s32 i;

                scale.unk0 *= arg0->unk58;
                scale.unk4 *= arg0->unk58;
                scale.unk8 *= arg0->unk58;

                pt = arg0->unk64;
                for (i = 0; i < hdr->unk12; i++) {
                    pt->unkC.unk0 = arg0->unk30 + pt->unk0.unk0 * scale.unk0 +
                                    pt->unk0.unk4 * up.unk0 + pt->unk0.unk8 * dir.unk0;
                    pt->unkC.unk4 = arg0->unk34 + pt->unk0.unk0 * scale.unk4 +
                                    pt->unk0.unk4 * up.unk4 + pt->unk0.unk8 * dir.unk4;
                    pt->unkC.unk8 = arg0->unk38 + pt->unk0.unk0 * scale.unk8 +
                                    pt->unk0.unk4 * up.unk8 + pt->unk0.unk8 * dir.unk8;
                    pt++;
                }
            }
        }
    }

    if (flag) {
        func_1516972C((struct102 *)arg0);
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
s32 func_15142E24(s32, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
s32 func_15144B34(s32);
void func_15145548(void *, void *, void *, void *, f32 *);
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142C10(Gfx *, s32, s32, s32, s32, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80090D70[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

typedef struct {
    f32 x, y, z;
} Vec3f_151A7D6C;

typedef struct {
    u8 pad0[0xC];
    Vec3f_151A7D6C pos;
} Game1D4E00Point;

typedef struct {
    u8 pad0[0x12];
    s16 count;
    u8 pad14[8];
    u8 flags;
    u8 pad1D[7];
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    f32 width;
    u8 pad2C[0x38];
    Game1D4E00Point *points;
    u8 pad68[4];
    u8 buffers[0x10];
    s32 size;
} Game1D4E00Bolt;

Gfx *func_151A7D6C(Gfx *gdl, Game1D4E00Bolt *arg1, s16 arg2) {
    Game1D4E00Bolt *self = arg1;
    Vtx_t *vtx;
    Vec3f_151A7D6C dir;
    Vec3f_151A7D6C closest;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 *camera;
    f32 lenSq;
    u8 changed;
    s16 last;
    f32 scale;
    s16 i;
    f32 parameter;
    s16 j;
    Vec3f_151A7D6C *a;
    Vec3f_151A7D6C *b;

    if (arg1->flags & 2) {
        func_151D5D60(arg1->buffers, arg2, arg1->size, (void **)&vtx, NULL);
        if (vtx != NULL) {
            camera = (f32 *)func_15144B34(arg2);
            changed = 1;
            last = arg1->count - 1;
            j = 0;
            i = 1;
            gdl = (Gfx *)func_15142E24((s32)gdl, (s32)D_80090D70, 0, 0, 0, 0, 0x2C, 0, 0, &changed, 3);
            gdl = func_15142B7C(gdl, 0x200005, 0x1F0600);
            gdl = func_15142C10(gdl, arg1->r, arg1->g, arg1->b, arg1->a, &changed);
            gdl = func_1513F4E4(gdl, 0x19, &changed);
            gdl = func_15142FBC(gdl, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &changed);
            dir.x = arg1->points[0].pos.x - arg1->points[last].pos.x;
            dir.y = arg1->points[0].pos.y - arg1->points[last].pos.y;
            dir.z = arg1->points[0].pos.z - arg1->points[last].pos.z;
            func_15145548(&arg1->points[last].pos, &dir, camera, &closest, &parameter);
            dx = camera[0] - closest.x;
            dy = camera[1] - closest.y;
            dz = camera[2] - closest.z;
            nx = dir.y * dz - dy * dir.z;
            ny = dir.z * dx - dz * dir.x;
            nz = dir.x * dy - dx * dir.y;
            lenSq = nx * nx + ny * ny + nz * nz;
            if (lenSq == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = arg1->width / sqrtf(lenSq);
                nx *= scale;
                ny *= scale;
                nz *= scale;
            }
            for (; i < self->count; i++, j++) {
                b = &arg1->points[i].pos;
                a = &arg1->points[j].pos;
                vtx->ob[0] = a->x + nx;
                vtx->ob[1] = a->y + ny;
                vtx->ob[2] = a->z + nz;
                vtx->tc[0] = 0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = a->x - nx;
                vtx->ob[1] = a->y - ny;
                vtx->ob[2] = a->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = b->x - nx;
                vtx->ob[1] = b->y - ny;
                vtx->ob[2] = b->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0x3C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = b->x + nx;
                vtx->ob[1] = b->y + ny;
                vtx->ob[2] = b->z + nz;
                vtx->tc[0] = 0;
                vtx->tc[1] = 0x3C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                gSPVertex(gdl++, vtx - 4, 4, 0);
                gSP1Triangle(gdl++, 0, 1, 2, 0);
                gSP1Triangle(gdl++, 0, 2, 3, 0);
            }
        }
    }
    return gdl;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1D4E00/func_151A8340.s")

extern void func_151D5E30(void *);

void func_151A8560(void *arg0) {
    func_151D5E30((void *)((u8 *)arg0 + 0x6C));
}

typedef struct {
    u8 pad_0[0x5C];
    u8 field_5C;
} Game1D4E00State;

extern void (*D_8008F94C[])(s32);

void func_151A8584(s32 arg0) {
    Game1D4E00State *state = (Game1D4E00State *)arg0;

    if (D_8008F94C[state->field_5C] != NULL) {
        D_8008F94C[state->field_5C](arg0);
    }
    func_151A8560((void *)arg0);
    func_15169804((struct102 *)arg0);
}

typedef struct {
    u8 pad_0[0x5C];
    u8 field_5C;
} Game1D4E00State_151A85D4;

extern void (*D_8008F958[])(s32);

void func_151A85D4(s32 arg0) {
    Game1D4E00State_151A85D4 *state = (Game1D4E00State_151A85D4 *)arg0;

    if (D_8008F958[state->field_5C] != NULL) {
        D_8008F958[state->field_5C](arg0);
    }
    func_151A8560((void *)arg0);
    func_15169824((struct102 *)arg0);
}

typedef struct {
    u8 bytes[4];
} Unaligned4_151A8624;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
} Vec3f_151A8624;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    Header2 unk8;
    u8 unk14;
    u8 pad15[3];
    Header2 unk18;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
} Payload_151A8624;

typedef struct {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    Unaligned4_151A8624 unk14;
    f32 unk18;
    u8 unk1C;
    s8 unk1D;
    u8 pad1E[2];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 unk4C;
    u8 pad4D[3];
} Header_151A8624;

void *func_151A8624(u8 *arg0, Header2 *arg1, u8 arg2, s16 arg3,
                    f32 arg4, f32 arg5, Unaligned4_151A8624 *arg6,
                    f32 arg7, Vec3f_151A8624 *arg8, u8 arg9, u8 argA,
                    s8 argB, s32 argC, u8 argD, s32 argE) {
    void *temp_v0;
    Header_151A8624 header;
    Payload_151A8624 payload;
    u8 flag;

    if (arg0 == NULL) {
        return NULL;
    }
    if (arg9 >= 2) {
        arg9 = 0;
    }

    payload.unk8 = *arg1;
    payload.unk14 = arg2;
    payload.unk25 = 0;
    payload.unk0 = arg0;
    payload.unk4 = arg0[0x3B];
    payload.unk24 = argA;
    temp_v0 = &header;

    if (arg8 != NULL) {
        *(Vec3f_151A8624 *)&payload.unk18 = *arg8;
    } else {
        ((Vec3f_151A8624 *)&payload.unk18)->unk0 = 0.0f;
        ((Vec3f_151A8624 *)&payload.unk18)->unk4 = 0.0f;
        ((Vec3f_151A8624 *)&payload.unk18)->unk8 = 0.0f;
    }

    if (arg3 == -1) {
        header.unk0 = 0x12C;
    } else {
        header.unk0 = arg3;
    }
    header.unk2 = 9;
    header.unk4 = arg4;
    header.unk8 = arg5;
    if (arg3 == -1) {
        flag = 0;
    } else {
        flag = 1;
    }
    header.unkC = flag;
    header.unk10 = D_800A8DE0;
    header.unk14 = *arg6;
    header.unk18 = arg7;
    if ((arg9 != 0) && (arg9 == 1)) {
        header.unk1C = 1;
    } else {
        header.unk1C = 1;
    }
    header.unk20 = 0.0f;
    header.unk24 = 0.0f;
    header.unk28 = 0.0f;
    header.unk2C = 0.0f;
    header.unk30 = 0.0f;
    header.unk34 = 0.0f;
    header.unk38 = 0.0f;
    header.unk3C = 0.0f;
    header.unk40 = 0.0f;
    header.unk44 = 0.0f;
    header.unk1D = argB;
    header.unk4C = 2;
    header.unk48 = 1.0f;

    temp_v0 = func_151A7950(temp_v0, argC + 0x28, argD, argE);
    if (temp_v0 != NULL) {
        memcpy(*(void **)((u8 *)temp_v0 + 0x60), &payload, sizeof(payload));
    }
    return temp_v0;
}

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x14 - 0x5];
    struct17 unk14;
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    u8 pad3C[0x1D4 - 0x3C];
    s32 unk1D4;
} Owner151A87F8;

typedef struct {
    Owner151A87F8 *unk0;
    u8 unk4;
    u8 pad5[3];
    struct17 unk8;
    u8 unk14;
    u8 pad15[3];
    struct17 unk18;
    u8 pad24;
    u8 unk25;
} Link151A87F8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 pad1D[0x30 - 0x1D];
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    f32 unk58;
    u8 pad5C[4];
    Link151A87F8 *unk60;
} Obj151A87F8;

extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_15143134(void *, void *, s32);
extern f32 func_15143E64(struct17 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);

s32 func_151A87F8(Obj151A87F8 *arg0) {
    Link151A87F8 *link;
    Owner151A87F8 *owner;
    struct17 diff;
    s32 hit;
    f32 x;
    f32 y;
    f32 z;
    struct17 tmp;

    link = arg0->unk60;
    owner = link->unk0;
    if ((owner->unk0 == 0) || (owner->unk3B != link->unk4) || (owner->unk4 == 0xFF)) {
        return 0;
    }
    if (owner->unk1D4 != 0) {
        func_15143134(&link->unk8, &arg0->unk30, owner->unk1D4 + (link->unk14 << 6));
        if (!(link->unk25 & 1)) {
            x = arg0->unk30.unk0 - (owner->unk14.unk0 + link->unk18.unk0);
            y = arg0->unk30.unk4 - (owner->unk14.unk4 + link->unk18.unk4);
            z = arg0->unk30.unk8 - (owner->unk14.unk8 + link->unk18.unk8);
            if (func_150AC9C0(arg0->unk30.unk0, arg0->unk30.unk4, arg0->unk30.unk8, x, y, z, 0, 0,
                              &arg0->unk3C.unk0, &arg0->unk3C.unk4, &arg0->unk3C.unk8, 0, &hit, 0, 0.0f)) {
                if (func_15145C90(hit)) {
                    link->unk25 |= 1;
                } else {
                    arg0->unk1C &= ~2;
                    return 1;
                }
            } else {
                arg0->unk1C &= ~2;
                return 1;
            }
        }
        diff.unk0 = arg0->unk3C.unk0 - arg0->unk30.unk0;
        diff.unk4 = arg0->unk3C.unk4 - arg0->unk30.unk4;
        diff.unk8 = arg0->unk3C.unk8 - arg0->unk30.unk8;
        arg0->unk54 = func_15143E64(&diff);
        if (arg0->unk54 != 0.0f) {
        lbl0:
            arg0->unk58 = 1.0f / arg0->unk54;
            func_15146078(&diff, &tmp, &arg0->unk48);
            arg0->unk1C |= 2;
        lbl1:
            return 1;
        } else {
            arg0->unk1C &= ~2;
        lbl2:
            return 1;
        }
    } else {
        arg0->unk1C &= ~2;
    }
lbl3:
    return 1;
}

extern void (*D_8008F964[])(u8 *, s32, u8);

void func_151A8A20(u8 *arg0, s32 arg1, u8 arg2) {
    s32 idx;

    idx = arg0[0x5C];
    if (idx >= 3) {
        idx = 0;
    }
    if (D_8008F964[idx] != NULL) {
        D_8008F964[idx](arg0, arg1, arg2);
    }
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151A8A78;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151A8A78;

void func_151A8A78(void *arg0, s32 arg1, u8 arg2) {
    SubA_151A8A78 *temp_v0 = *(SubA_151A8A78 **)((u8 *)arg0 + 0x60);
    ArgB_151A8A78 *b = (ArgB_151A8A78 *)arg1;
    s32 b0;

    if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (temp_v0->unk4 == b->u4.b4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

extern f32 D_800A8F50;
extern f32 D_800A8F54;

extern void func_1510F800(s32);

typedef struct {
    u8  unk0;
    u8  pad1[3];
    struct134 *unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8  unk24;
    u8  unk25;
    u8  unk26;
    u8  pad27;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s32 unk44;
    u8  unk48;
    u8  unk49;
    u8  pad4A[2];
    s32 unk4C;
    s32 unk50;
    f32 unk54;
} Header_151A8B20;

s32 func_151A8B20(void *arg0, s16 arg1, s32 arg2, u8 arg3, s32 arg4) {
    s32 effect;
    Header_151A8B20 header;
    Header_151A8B20 *ptr;

    memcpy(&header, arg0, 0x28);

    header.unk28 = 0.0f;
    header.unk2C = D_800A8F50;
    header.unk44 = 0;
    header.unk48 = 0;
    header.unk49 = 0;
    header.unk4C = 0;

    effect = (s32)func_15149130((arg1 == -1) ? 0x12C : arg1, -1, 0x25, -1, (arg1 == -1) ? 0 : 1, 0x22, (struct37 *)(arg2 + 0x58), arg3, arg4);
    if (effect != 0) {
        ptr = (Header_151A8B20 *)(effect + 0x28);
        memcpy(ptr, &header, 0x58);

        if (ptr->unk0 & 4) {
            f32 posZ;
            f32 posX;

            if ((ptr->unk0 & 2) && (ptr->unk4 != NULL)) {
                posX = (f32)ptr->unk4->unk0;
                posZ = (f32)ptr->unk4->unk4;
            } else {
                posX = ptr->unk10;
                posZ = ptr->unk18;
            }
            func_1510F800(0);
            ptr->unk50 = func_1510FD20((s32)posX, (s32)posZ);
        } else {
            ptr->unk50 = 0;
        }

        if ((ptr->unk0 & 2) && (ptr->unk4 != NULL)) {
            ptr->unk54 = func_15144598(ptr->unk4);
        } else {
            ptr->unk54 = (ptr->unk20 * ptr->unk20) * D_800A8F54;
        }
    }
    return effect;
}

typedef struct {
    u8  unk0;
    u8  pad1[3];
    void *unk4;
    f32 unk8;
    f32 unkC;
    u8  pad10[0x15];
    s8  unk25;
    s8  unk26;
    u8  pad27;
    f32 unk28;
    f32 unk2C;
    u8  pad30[0x20];
    s32 unk50;
    f32 unk54;
} Header_151A8CEC;

extern s32 (*D_8008F980[])(void *);
extern void (*D_8008F970[])(void *, f32 *, f32, u8);

s32 func_151464B8(s32);
void func_151A8F1C(void *, f32 *, f32 *, f32 *);
void func_151A8F6C(void *, f32 *, f32 *, f32 *);

void func_151A8CEC(u8 *arg0) {
    Header_151A8CEC *ptr;
    void (*cb)(void *, f32 *, f32 *, f32 *);
    f32 pos[3];
    f32 radius;
    f32 angle;
    u8 flag;

    ptr = (Header_151A8CEC *)(arg0 + 0x28);

    if (*(s8 *)(arg0 + 0x4D) != -1) {
        if (D_8008F980[ptr->unk25](arg0) == 0) {
            *(s16 *)(arg0 + 0xE) = -1;
            return;
        }
    }

    if ((ptr->unk0 & 4) && (ptr->unk50 != 0)) {
        if (func_151464B8(ptr->unk50) != 0) {
            return;
        }
    }
    if (!(ptr->unk0 & 1)) {
        return;
    }

    ptr->unk28 += (ptr->unk8 + func_150ADA68() * ptr->unkC) * D_800BE9A4 * ptr->unk54;

    if (ptr->unk28 > 1.0f) {
        if ((ptr->unk0 & 2) && (ptr->unk4 != NULL)) {
            cb = func_151A8F1C;
        } else {
            cb = func_151A8F6C;
        }
        do {
            cb(arg0, pos, &angle, &radius);
            if (ptr->unk0 & 8) {
                pos[1] = radius;
                flag = 1;
            } else if (func_15046C80(pos, 0, radius, &ptr->unk2C) != 0) {
                pos[1] = ptr->unk2C;
            } else {
                pos[1] = radius;
                flag = 1;
            }
            if (ptr->unk26 != -1) {
                D_8008F970[ptr->unk26](arg0, pos, angle, flag);
            }
            ptr->unk28 -= 1.0f;
        } while (ptr->unk28 > 1.0f);
    }
}

extern void func_151432BC(void *, f32 *, f32 *, f32 *, f32 *);

void func_151A8F1C(void *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    func_151432BC(*(void **)((u8 *)arg0 + 0x2C), arg1, arg1 + 2, arg2, arg3);
    arg1[1] = arg2[0];
}

void func_15143874(s16, f32, f32 *, f32 *);

void func_151A8F6C(void *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    u8 *state;

    state = (u8 *)arg0 + 0x28;
    func_15143874((s16)(func_150ADA20() & 0xFF), *(f32 *)(state + 0x20) * func_150ADA68(),
                  arg1, arg1 + 2);
    arg1[0] += *(f32 *)(state + 0x10);
    arg1[2] += *(f32 *)(state + 0x18);
    arg1[1] = *(f32 *)(state + 0x14);
    *arg2 = *(f32 *)(state + 0x14);
    *arg3 = *(f32 *)(state + 0x1C);
}

void func_151A931C(u8 *, s32, u8);

void func_151A9024(u8 *arg0, s32 arg1, u8 arg2) {
    if (arg0[0x4C] == 1) {
        func_151A931C(arg0, arg1, arg2);
    }
}

extern void (*D_8008F984[])(void);
s32 func_151A9060(u8 *arg0)
{
  s32 temp;
  s32 idx;
  arg0[0x16] |= 0x4;
  temp = *((s32 *) (arg0 + 0x18));
  idx = temp & 0xFFFFFFFFu;
  temp = idx < 0;
  if (temp || (idx >= 8))
  {
    return 1;
  }
  if (D_8008F984[idx])
  {
    D_8008F984[idx]();
  }
  return 1;
}


void func_151A90C0(s32 arg0, s32 arg1) {
    struct {
        s8 unk0;
        u8 pad1[3];
        s32 unk4;
        f32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        s8 unk24;
        s8 unk25;
        s8 unk26;
    } sp30;
    s32 sp28;
    f32 temp_f0;
    s32 temp_v0;

    sp30.unk0 = 2;
    sp30.unk4 = arg0;
    temp_f0 = (f32)(arg1 & 1);
    if (temp_f0) {
        sp30.unk8 = D_800A8F58;
    } else {
        sp30.unk8 = D_800A8F5C;
    }

    if (temp_f0) {
        sp30.unkC = D_800A8F60;
    } else {
        sp30.unkC = D_800A8F64;
    }

    temp_f0 = 0.0f;
    sp30.unk10 = temp_f0;
    sp30.unk14 = temp_f0;
    sp30.unk18 = temp_f0;
    sp30.unk1C = temp_f0;
    sp30.unk20 = temp_f0;
    sp30.unk24 = 1;
    sp30.unk25 = -1;
    sp30.unk26 = 0;
    ((s8 *)&sp28)[-4] = (s8)arg1;

    temp_v0 = func_151A8B20(&sp30, -1, 1, 0xFF, 0);
    if (temp_v0 != 0) {
        memcpy((void *)(temp_v0 + 0x80), ((s8 *)&sp28) - 4, 1);
    }
}

struct S6_151A91AC { s16 unk0; s16 unk2; s16 unk4; };
extern struct S6_151A91AC D_8008F9A4;

struct Loc_151A91AC {
    s32 unk0;
    s16 unk4;
    u8  unk6;
    u8  unk7;
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

void func_151A91AC(u8 *arg0, f32 *arg1, s32 arg2, s32 arg3) {
    struct Loc_151A91AC sp64;
    f32 fval;
    struct S6_151A91AC sp58;
    u32 idx;
    s32 r3;
    s32 r2;

    sp58 = D_8008F9A4;
    fval = func_150ADA68() * 50.0f + 50.0f;
    idx = (u32)func_150ADA20() % 3U;

    sp64.unk6 = (u8)((s16 *)&sp58)[idx];
    sp64.unk7 = 0;
    sp64.unk16 = 0;
    sp64.unk17 = 7;
    sp64.unk8 = 0;
    sp64.unkC = 0;
    sp64.unk0 = 0x1701;
    sp64.unk4 = 0x3C;
    sp64.unk10 = 0xA0;
    sp64.unk11 = 0xFF;
    sp64.unk12 = 0;
    sp64.unk13 = 0;
    sp64.unk14 = 0;
    sp64.unk15 = 0xFF;
    sp64.unk18 = 0x3B0002;

    r2 = func_150ADA20();
    r3 = func_150ADA20();
    func_1513C650((s32)&sp64, 1, 0, (s32)(arg0 + 0x58), arg1[0], arg1[1], arg1[2],
                  fval, fval, r2 & 0xFF,
                  (r3 & 1) + (func_150ADA20() & 1), 3, 0xFF, 0,
                  arg0[0xC], arg0[1]);
}

void func_151A931C(u8 *arg0, s32 arg1, u8 arg2) {
    u8 *ptr;

    if (arg2 == 0x17) {
        ptr = arg0 + 0x28;
        if (*(u8 *)arg1 != arg0[0x80]) {
            goto ret;
        }
        *ptr |= 1;
    ret:
        return;
    } else if (arg2 == 0x18) {
        ptr = arg0 + 0x28;
        if (*(u8 *)arg1 == arg0[0x80]) {
            *ptr &= ~1;
        }
    }
}

typedef struct {
    f32 unk0;
    f32 unk4;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  unkB;
    u8  unkC;
    u8  padD[3];
    void *unk10;
} Def_151A9390;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151A9390;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Pos_151A9390;

typedef struct {
    u8  unk0;
    u8  pad1[3];
    void *unk4;
    f32 unk8;
    f32 unkC;
    Vec3f_151A9390 unk10;
    f32 unk1C;
    f32 unk20;
    s8  unk24;
    s8  unk25;
    s8  unk26;
    u8  pad27;
} Header_151A9390;

extern Def_151A9390 D_8008F9AC[];

void *func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);

void func_151A9390(u8 arg0, u8 arg1, Pos_151A9390 *arg2, Vec3f_151A9390 *arg3,
                   f32 arg4, f32 arg5, s16 arg6, u8 arg7, s32 arg8) {
    Header_151A9390 hdr;
    s32 effect;

    if (arg1 >= 9) {
        return;
    }

    hdr.unk0 = 1 | ((arg2 != NULL) ? 2 : 0) | ((arg0 & 1) ? 4 : 0) | ((arg0 & 2) ? 8 : 0);
    hdr.unk4 = arg2;
    hdr.unk8 = D_8008F9AC[arg1].unk0;
    hdr.unkC = D_8008F9AC[arg1].unk4;
    if (arg3 != NULL) {
        hdr.unk10 = *arg3;
    } else {
        hdr.unk10.x = 0.0f;
        hdr.unk10.y = 0.0f;
        hdr.unk10.z = 0.0f;
    }
    hdr.unk1C = arg4;
    hdr.unk20 = arg5;
    hdr.unk24 = 2;
    hdr.unk25 = -1;
    hdr.unk26 = 1;

    effect = func_151A8B20(&hdr, arg6, 0x2C, arg7, arg8);
    if (effect != 0) {
        u8 *ptr;

        ptr = (u8 *)(effect + 0x80);
        memcpy(ptr, D_8008F9AC[arg1].unk10, 0x2C);
        if (arg0 & 8) {
            ptr[0x28] |= 1;
        }
        if (arg0 & 0x10) {
            ptr[0x28] |= 2;
        }
    }

    if (arg0 & 4) {
        Header spawn;
        s32 pos[3];

        spawn.unk0 = ((arg6 == -1) ? 0 : 1) | 2;
        spawn.unk1 = 2;
        spawn.unk2 = (arg6 == -1) ? 0x12C : arg6;
        spawn.unk4 = D_8008F9AC[arg1].unkB;

        if (arg3 != NULL) {
            pos[0] = (s32)arg3->x;
            pos[1] = (s32)arg3->y;
            pos[2] = (s32)arg3->z;
        } else {
            pos[0] = arg2->unk0;
            pos[1] = arg2->unk2;
            pos[2] = arg2->unk4;
        }

        func_1516284C(&spawn, pos, D_8008F9AC[arg1].unk8, D_8008F9AC[arg1].unk9,
                      D_8008F9AC[arg1].unkA, 0xFF, 0, 0, D_8008F9AC[arg1].unkC,
                      0xFF, 1);
    }
}

typedef struct Emitter_151A9634 {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    s16 field10;
    s16 field12;
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1A;
    s16 field1C;
    s16 field1E;
    f32 field20;
    f32 field24;
    u8 flags28;
} Emitter_151A9634;

typedef struct Actor_151A9634 {
    u8 pad0;
    u8 field1;
    u8 pad2[0xA];
    u8 fieldC;
    u8 padD[0x73];
    Emitter_151A9634 emitter;
} Actor_151A9634;

typedef struct Payload_151A9634 {
    f32 field0;
    s16 field4;
} Payload_151A9634;

s32 func_1514B8E4(s32, f32 *, s16, s32, s32, f32, f32, f32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151A9634(Actor_151A9634 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 position[2];
    s32 choice;
    Payload_151A9634 payload;
    s32 kind;
    s32 flags;
    s32 result;
    Emitter_151A9634 *emitter;
    u32 random2;
    u32 random1;

    emitter = &arg0->emitter;
    position[0] = func_150ADA68() * emitter->field8 + emitter->field0;
    position[1] = func_150ADA68() * emitter->fieldC + emitter->field4;
    payload.field0 = func_150ADA68() * emitter->field24 + emitter->field20;
    payload.field4 = func_150ADA20() % (u32)(emitter->field16 + 1) + emitter->field14;
    random1 = func_150ADA20();
    random2 = func_150ADA20();
    if (emitter->flags28 & 1) {
        kind = 0x71;
    } else {
        if (func_150ADA20() & 1) {
            choice = 0x13;
        } else {
            choice = 0x14;
        }
        kind = choice;
    }
    flags = (emitter->flags28 & 2) ? 0 : 2;
    result = func_1514B8E4(arg1, position,
        (s16)(random1 % (u32)(emitter->field12 + 1) + emitter->field10),
        (random2 % (u32)(emitter->field1A + 1) + emitter->field18) & 0xFF,
        0, 0.0f, 1.0f, 1.0f, 0x21, 0x23, 2, kind, flags,
        emitter->field1C, emitter->field1E, 8, arg0->fieldC, arg0->field1);
    if (result != 0) {
        memcpy((u8 *)result + 0x160, &payload, 8);
    }
}

typedef struct {
    f32 unk0;
    u8 pad4[0x14];
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    s32 unk20;
} Struct151A9834;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f151A9834;

void func_151A9834(f32 *arg0, f32 arg1, f32 arg2, Struct151A9834 *arg3,
                   s32 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    Struct151A9834 sp8C;
    Vec3f151A9834 sp80;
    Vec3f151A9834 sp74;
    f32 temp_f22;
    f32 temp_f20;
    f32 temp_f0;

    if (arg3 == 0) {
        arg3 = &sp8C;
        sp8C.unk0 = D_800A8F68;
        sp8C.unk18 = 0;
        sp8C.unk1C = 7;
        sp8C.unk1D = 0;
        sp8C.unk20 = 0;
    }

    sp80.y = arg0[1];
    if (arg4 > 0) {
        temp_f22 = D_800A8F6C;
        do {
            temp_f20 = func_150ADA68();
            temp_f0 = func_150ADA68();
            func_1514373C((temp_f20 + temp_f20) * temp_f22, temp_f0 * arg2,
                          &sp80.x, &sp80.z);
            sp80.x += arg0[0];
            sp80.z += arg0[2];

            if (func_15046C80(&sp80.x, 0, arg1, arg3) != 0) {
                sp74.x = sp80.x;
                sp74.y = arg3->unk0;
                sp74.z = sp80.z;
                D_8008FA60[arg5](&sp74.x, arg3, arg6, arg7, arg8);
            }
            arg4--;
        } while (arg4 > 0);
    }
}
