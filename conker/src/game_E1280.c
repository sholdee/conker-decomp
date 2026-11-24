#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void func_15147D64(s32, s32);

void func_150B3DD0(void) {
    u8 sp24[4];

    func_15147D64(0, 5);
    sp24[0] = 0;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 2;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 4;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 1;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 3;
    func_151494E0((s32)&sp24[0], 0x18);
    sp24[0] = 5;
    func_151494E0((s32)&sp24[0], 0x18);
}

extern void func_1000FC18(s32, s16, s16, s16, s32);
extern void func_151478F4(void *);

void func_150B3E74(void *arg0) {
    func_1000FC18(0x221, (s16)(s32)*(f32 *)((u8 *)arg0 + 0x10),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x14),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x18), 0xFA0);
    func_151478F4(arg0);
}

extern void func_15147928(void *);

void func_150B3EE8(void *arg0) {
    func_1000FC18(0x221, (s16)(s32)*(f32 *)((u8 *)arg0 + 0x10),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x14),
                  (s16)(s32)*(f32 *)((u8 *)arg0 + 0x18), 0xFA0);
    func_15147928(arg0);
}

extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150B3F5C(void *arg0, void *arg1, s16 arg2) {
    struct vec150B3F5C {
        s32 unk0;
        s32 unk4;
        s32 unk8;
    };
    struct {
        struct vec150B3F5C unk0;
        s16 unkC;
        s16 unkE;
        s32 unk10;
        u8 unk14;
        u8 unk15;
        u8 pad16[2];
        s32 unk18;
    } sp9C;
    struct {
        u8 pad0[0x40];
        f32 unk40;
        f32 unk44;
        u8 unk48;
        u8 pad49[3];
        f32 unk4C;
        u8 unk50;
        u8 pad51[3];
        f32 unk54;
    } sp44;
    s32 temp_v0;
    s32 rnd;

    memcpy(&sp44, arg0, 0x40);
    sp44.unk44 = 0.0f;
    sp9C.unk15 = arg2;
    sp44.unk48 = 0;
    sp9C.unkE = 2;
    sp9C.unkC = 0x1F4;
    sp44.unk4C = *(f32 *)((u8 *)arg0 + 0x18) + *(f32 *)((u8 *)arg0 + 0x14);
    sp44.unk50 = 0;
    sp44.unk54 = 0.0f;
    sp44.unk40 = sp44.unk4C;
    sp9C.unk0 = *(struct vec150B3F5C *)arg1;
    sp9C.unk10 = 9;

    temp_v0 = func_15147A80(&sp9C, 0x58, 0x24, 7, 7, 7, 0, 0, 0, 0xFF, 1);
    if (temp_v0 != 0) {
        rnd = func_150ADA20();
        func_1000FA64(0x221, (s16)(s32)*(f32 *)((u8 *)arg1 + 0),
                      (s16)(s32)*(f32 *)((u8 *)arg1 + 4),
                      (s16)(s32)*(f32 *)((u8 *)arg1 + 8), 0x61A8, 0xFA0,
                      0x258, (s32)func_1000EF40, 0, 0, 8, rnd & 0x40);
        memcpy((void *)*(s32 *)((u8 *)temp_v0 + 0x98), &sp44, 0x58);
    }
    return temp_v0;
}

s32 func_150B40E8(void *arg0, s32 var_a1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
    } Vec3f;
    void *temp_v0;
    void *temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    void *temp_a3;

    var_a1 = *(s8 *)((u8 *)arg0 + 0x2C);
    temp_v0 = *(void **)((u8 *)arg0 + 0x98);
    temp_v1 = *(void **)((u8 *)arg0 + 0x94);

    if ((var_a1 < 2) && ((*(u8 *)((u8 *)temp_v0 + 0x1C) & 1) != 0)) {
        return 0;
    }

    var_a2 = *(s8 *)((u8 *)arg0 + 0x2E);
    var_a3 = *(s8 *)((u8 *)arg0 + 0x2D);

    if (var_a2 != var_a3) {
        var_t0 = 0x24;

        do {
            var_a2--;
            var_a1 = 0;
            if (var_a2 < 0) {
                var_a2 = *(u8 *)((u8 *)arg0 + 0x25) - 1;
            }

            temp_a3 = (u8 *)temp_v1 + (var_a2 * var_t0);
            *(s16 *)((u8 *)temp_a3 + 0x1C) = 0xFF;
            *(s16 *)((u8 *)temp_a3 + 0x1E) = *(s16 *)((u8 *)temp_a3 + 0x1E) - D_800BE9E4;
            if (*(s16 *)((u8 *)temp_a3 + 0x1E) < 0) {
                var_a1 = 1;
            }

            *(f32 *)((u8 *)temp_a3 + 0x10) += *(f32 *)((u8 *)temp_v0 + 0xC) * (&D_800BE9A4)[0];
            *(f32 *)((u8 *)temp_a3 + 0x0) += *(f32 *)((u8 *)temp_a3 + 0xC) * (&D_800BE9A4)[0];
            *(f32 *)((u8 *)temp_a3 + 0x4) += *(f32 *)((u8 *)temp_a3 + 0x10) * (&D_800BE9A4)[0];
            *(f32 *)((u8 *)temp_a3 + 0x8) += *(f32 *)((u8 *)temp_a3 + 0x14) * (&D_800BE9A4)[0];
            *(f32 *)((u8 *)temp_a3 + 0x18) += *(f32 *)((u8 *)temp_v0 + 0x28) * (&D_800BE9A4)[0];

            if (var_a1 != 0) {
                var_a3 = *(s8 *)((u8 *)arg0 + 0x2D);
                if (var_a2 != var_a3) {
                    do {
                        *(s8 *)((u8 *)arg0 + 0x2D) = var_a3 + 1;
                        var_a3 = *(s8 *)((u8 *)arg0 + 0x2D);
                        if (*(u8 *)((u8 *)arg0 + 0x25) != *(s8 *)((u8 *)arg0 + 0x2D)) {
                        } else {
                            *(s8 *)((u8 *)arg0 + 0x2D) = 0;
                            var_a3 = *(s8 *)((u8 *)arg0 + 0x2D);
                        }
                        *(s8 *)((u8 *)arg0 + 0x2C) = *(s8 *)((u8 *)arg0 + 0x2C) - 1;
                    } while (var_a2 != var_a3);
                }
            }

            var_a3 = *(s8 *)((u8 *)arg0 + 0x2D);
        } while (var_a2 != var_a3);

        var_a1 = *(s8 *)((u8 *)arg0 + 0x2C);
    }

    if (var_a1 > 0) {
        var_t0 = 0x24;
        temp_a3 = (u8 *)temp_v1 + (var_a3 * var_t0);
        *(Vec3f *)((u8 *)arg0 + 0x54) = *(Vec3f *)temp_a3;
    } else {
        *(f32 *)((u8 *)arg0 + 0x54) = 0.0f;
        *(f32 *)((u8 *)arg0 + 0x58) = 0.0f;
        *(f32 *)((u8 *)arg0 + 0x5C) = 0.0f;
    }

    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_B4294;

typedef struct {
    Vec3f_B4294 unk0;
    f32 unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
    u8 pad1C[4];
    f32 unk20;
    u8 pad24[8];
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 pad32[4];
    s16 unk36;
    u8 pad38[4];
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    u8 unk48;
    u8 pad49[3];
    f32 unk4C;
    u8 unk50;
    u8 pad51[3];
    f32 unk54;
} SplatOwner_B4294;

typedef struct {
    Vec3f_B4294 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    u8 unk20;
    u8 pad21[3];
} SplatTrail_B4294;

typedef struct {
    u8 pad0[0x10];
    Vec3f_B4294 unk10;
    u8 pad1C[9];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    SplatTrail_B4294 *unk94;
    SplatOwner_B4294 *unk98;
} SplatActor_B4294;

void func_150B5088();

s32 func_150B4294(SplatActor_B4294 *arg0) {
    f32 inv;
    f32 dt;
    f32 scale;
    SplatOwner_B4294 *owner;
    SplatTrail_B4294 *trail;
    Vec3f_B4294 sp80;
    f32 diff;
    f32 dtStep;
    f32 scaleStep;

    owner = arg0->unk98;
    trail = arg0->unk94;
    owner->unk50 += (func_150ADA20() % (u32)(owner->unk10 + 1)) * D_800BE9E4;
    owner->unk4C = func_151423D8(owner->unk50 - 0x40) * owner->unk18 + owner->unk14;
    owner->unk44 += owner->unk20 * D_800BE9A4;
    if (owner->unk44 > 1.0f) {
        dt = D_800BE9A4;
        inv = 1.0f / owner->unk44;
        sp80 = owner->unk0;
        dtStep = D_800BE9A4 * inv;
        scale = owner->unk40;
        diff = owner->unk4C - owner->unk40;
        scaleStep = diff * inv;
        do {
            owner->unk54 += owner->unk3C;
            if (owner->unk54 > 1.0f) {
                func_150B5088(arg0);
            }
            trail[arg0->unk2E].unk0 = arg0->unk10;
            trail[arg0->unk2E].unkC = sp80.x * scale;
            trail[arg0->unk2E].unk10 = sp80.y * scale;
            trail[arg0->unk2E].unk14 = sp80.z * scale;
            trail[arg0->unk2E].unk18 = owner->unk2C;
            trail[arg0->unk2E].unk1C = 0xFF;
            trail[arg0->unk2E].unk1E = owner->unk36;
            trail[arg0->unk2E].unk20 = owner->unk48;
            owner->unk48 += owner->unk30 + func_150ADA68() * owner->unk31;
            trail[arg0->unk2E].unk10 += owner->unkC * dt;
            trail[arg0->unk2E].unk0.x += trail[arg0->unk2E].unkC * dt;
            trail[arg0->unk2E].unk0.y += trail[arg0->unk2E].unk10 * dt;
            trail[arg0->unk2E].unk0.z += trail[arg0->unk2E].unk14 * dt;
            arg0->unk2E++;
            if (arg0->unk2E == arg0->unk25) {
                arg0->unk2E = 0;
            }
            arg0->unk2C++;
            if (arg0->unk2E == arg0->unk2D) {
                arg0->unk2D++;
                if (arg0->unk2D == arg0->unk25) {
                    arg0->unk2D = 0;
                }
                arg0->unk2C--;
            }
            scale += scaleStep;
            dt -= dtStep;
            owner->unk44 -= 1.0f;
        } while (owner->unk44 > 1.0f);
        owner->unk40 = scale;
    }
    return 1;
}

typedef struct {
    f32 x, y, z;
} Vec3f_150B4710;

typedef struct {
    Vec3f_150B4710 pos;
    u8 pad0C[0xC];
    f32 width;
    u16 alpha;
    u8 pad1E[2];
    u8 level;
    u8 pad21[3];
} Point150B4710;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 pad1D[7];
    u8 unk24;
    u8 pad25;
    s16 unk26;
    u8 pad28[0xA];
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
} Style150B4710;

typedef struct {
    u8 pad0[0x10];
    Vec3f_150B4710 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x84 - 0x2F];
    u8 unk84[0x10];
    Point150B4710 *points;
    Style150B4710 *style;
} Owner150B4710;

extern u8 D_80091238[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_150B4710(Owner150B4710 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Style150B4710 *style;
    Vec3f_150B4710 b;
    Vec3f_150B4710 a;
    s32 j;
    s32 i;
    Point150B4710 *points;
    f32 *cam;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 len;
    u8 level;
    u8 prev;
    s16 alpha;
    s16 prevAlpha;
    u8 update;
    u8 fade;
    s16 count;
    s32 k;
    f32 scale;
    f32 dist;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            cam = &D_800DBFF0[arg2].unk2F8;
            style = arg0->style;
            points = arg0->points;
            update = 1;
lbl1:
            if (style->unk1C & 2) {
                u8 fade1;
                s16 count1;

                fade1 = 0;
                count1 = style->unk32;
                k = arg0->unk2D;
                do {
                    points[k].alpha = fade1;
                    fade1 += style->unk33;
                    if (++k == arg0->unk25) k = 0;
                } while (--count1 != 0 && k != arg0->unk2E);
            }
            if (style->unk1C & 4) {
                fade = 0;
                count = style->unk34;
                k = arg0->unk2E - 1;
                if (k < 0) {
                    k = arg0->unk25 - 1;
                }
                do {
                    points[k].alpha = fade;
                    fade += style->unk35;
                    count--;
                    k--;
                    if (k < 0) {
                        k = arg0->unk25 - 1;
                    }
                } while (count != 0 && k != arg0->unk2E);
            }
            gfx = func_15142E24(gfx, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &update, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_1513F4E4(gfx, 0x4D, &update);
            gfx = func_15142C10(gfx, style->unk24, style->unk24, style->unk24, 0xFF, &update);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (arg0->unk1E & 2) {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = arg0->pos;
                prev = points[j].level;
                prevAlpha = (points[j].alpha * style->unk26) >> 8;
            } else {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = points[i].pos;
                prev = points[i].level;
                prevAlpha = (points[i].alpha * style->unk26) >> 8;
            }
            b = points[j].pos;
            level = points[j].level;
            alpha = (points[j].alpha * style->unk26) >> 8;
            dx = a.x - b.x;
            dy = a.y - b.y;
            dz = a.z - b.z;
            cx = a.x - cam[0];
            cy = a.y - cam[1];
            cz = a.z - cam[2];
            nx = dy * cz - cy * dz;
            ny = dz * cx - cz * dx;
            nz = dx * cy - cx * dy;
            len = nx * nx + ny * ny + nz * nz;
            if (len == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = points[j].width / sqrtf(len);
                nx = nx * scale;
                ny = ny * scale;
                nz = nz * scale;
            }
            vtx->v.ob[0] = (s16)(s32)(a.x + nx);
            vtx->v.ob[1] = (s16)(s32)(a.y + ny);
            vtx->v.ob[2] = (s16)(s32)(a.z + nz);
            vtx->v.tc[0] = points[j].level << 6;
            vtx->v.tc[1] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = (s16)(s32)(a.x - nx);
            vtx->v.ob[1] = (s16)(s32)(a.y - ny);
            vtx->v.ob[2] = (s16)(s32)(a.z - nz);
            vtx->v.tc[0] = points[j].level << 6;
            vtx->v.tc[1] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                dx = a.x - b.x;
                dy = a.y - b.y;
                dz = a.z - b.z;
                cx = b.x - cam[0];
                cy = b.y - cam[1];
                cz = b.z - cam[2];
                nx = dy * cz - cy * dz;
                ny = dz * cx - cz * dx;
                nz = dx * cy - cx * dy;
                len = nx * nx + ny * ny + nz * nz;
                if (len == 0.0f) {
                    nx = 0.0f;
                    ny = 0.0f;
                    nz = 0.0f;
                } else {
                    dist = points[j].width / sqrtf(len);
                    nx = nx * dist;
                    ny = ny * dist;
                    nz = nz * dist;
                }
                vtx->v.ob[0] = (s16)(s32)(b.x + nx);
                vtx->v.ob[1] = (s16)(s32)(b.y + ny);
                vtx->v.ob[2] = (s16)(s32)(b.z + nz);
                vtx->v.tc[0] = points[j].level << 6;
                vtx->v.tc[1] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = (s16)(s32)(b.x - nx);
                vtx->v.ob[1] = (s16)(s32)(b.y - ny);
                vtx->v.ob[2] = (s16)(s32)(b.z - nz);
                vtx->v.tc[0] = points[j].level << 6;
                vtx->v.tc[1] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prev < level) {
                    memcpy(vtx, vtx - 2, 0x20);
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                prev = level;
                a = points[i].pos;
                b = points[j].pos;
                alpha = (points[j].alpha * style->unk26) >> 8;
                level = points[j].level;
            } while (i != arg0->unk2D);
        }
    }
    return gfx;
}

void func_150B5060(void *arg0) {
    void *temp_v0 = *(void **)((u8 *)arg0 + 0x98);

    *(u8 *)((u8 *)arg0 + 0x30) = 0;
    *(u16 *)((u8 *)arg0 + 0x1E) = *(u16 *)((u8 *)arg0 + 0x1E) & 0xFFFD;
    *(u8 *)((u8 *)temp_v0 + 0x1C) = *(u8 *)((u8 *)temp_v0 + 0x1C) | 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} SplatParams;

typedef struct {
    struct17 unk0;
    u8 padC[0x18];
    u8 unk24;
    u8 pad25;
    s16 unk26;
    u8 pad28[0x10];
    u8 unk38;
    u8 pad39[0x1B];
    f32 unk54;
} SplatOwner;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    struct17 unk10;
    u8 pad1C[0x7C];
    SplatOwner *unk98;
} SplatActor;

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} SplatSpawn;

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
} SplatPart;

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
} SplatStyle;

extern SplatParams D_8009FBF0[];
extern s32 func_15147DA0(SplatSpawn *, SplatPart *, s32, s32, s32, s32, s32, s32, s32, s32, s32, SplatStyle *, s32, s32, s32);

void func_150B5088(SplatActor *arg0) {
    SplatStyle sp100;
    SplatOwner *owner;
    SplatParams *par;
    SplatSpawn spE0;
    SplatPart spC0;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 cosA;
    f32 sinA;
    f32 cosB;
    f32 sinB;
    u8 angA;
    u8 angB;

    owner = arg0->unk98;
    par = &D_8009FBF0[owner->unk38];

    sp100.unk0 = 0;
    sp100.unk4 = 1;
    sp100.unk8 = 0x160600;
    sp100.unkC = 3;
    sp100.unk10 = 0x10;
    sp100.unk14 = 0x80;
    sp100.unk18 = 0x20;
    sp100.unk1C = 0;
    sp100.unk1D = 9;
    spE0.unkE = 1;
    spC0.unk19 = 6;
    spC0.unk1A = owner->unk24;
    spC0.unk18 = 8;
    spC0.unk1B = owner->unk26;
    spE0.unk0 = arg0->unk10;

    do {
        speed = func_150ADA68() * par->unk8 + par->unk4;
        angA = func_150ADA20() & 0xFF;
        angB = func_150ADA20() & 0xFF;
        cosA = func_151423D8(angA - 0x40);
        sinA = func_151423D8(angA);
        cosB = func_151423D8(angB - 0x40);
        sinB = func_151423D8(angB);
        x = owner->unk0.unk0 + par->unk0 * sinB * cosA;
        y = owner->unk0.unk4 - par->unk0 * cosB;
        z = owner->unk0.unk8 + par->unk0 * sinB * sinA;
        spE0.unk15 = ((u32)func_150ADA20() % (par->unk22 + 1)) + par->unk20;
        spE0.unkC = ((u32)func_150ADA20() % (par->unk1E + 1)) + par->unk1C;
        spC0.unk0 = func_150ADA68() * par->unk10 + par->unkC;
        spC0.unk10 = func_150ADA68() * par->unk18 + par->unk14;
        spC0.unk4 = speed * x;
        spC0.unk8 = speed * y;
        spC0.unkC = speed * z;
        func_15147DA0(&spE0, &spC0, 0, 1, 7, 0, 0, 0, 0, 0, 0, &sp100, 0, arg0->unkC, arg0->unk1);
        owner->unk54 -= 1.0f;
    } while (owner->unk54 > 1.0f);
}

void func_150B538C(void *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 5) {
        func_150B5060(arg0);
    }
}
