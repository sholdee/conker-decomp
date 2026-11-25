#include <ultra64.h>
#define func_1513D2F0 func_1513D2F0_hdr
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);

s32 func_151BC580(s32 *arg0) {
    s32 *p = (s32 *)arg0[0x10];
    if (*p == 0) {
        return 0;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BC5A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151BC5A4;

void func_151BC5A4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151BC5A4 *temp_v0 = (SubA_151BC5A4 *)((u8 *)arg0 + 0x40);
    ArgB_151BC5A4 *b = (ArgB_151BC5A4 *)arg1;
    s32 b0 = temp_v0->unk0;

    if (arg2 == 0) {
        if ((b0 == b->unk0) || (temp_v0->unk4 == b->u4.b4)) {
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

typedef struct Game1E9A30Trail {
    s32 x;
    s32 y;
    s32 z;
    s16 lifetime;
    u8 alpha;
    u8 pad_F[5];
} Game1E9A30Trail;

typedef struct Vec3i_151BC64C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BC64C;

s32 func_151BC64C(u8 *actor) {
    Game1E9A30Trail *entries;
    Game1E9A30Trail *entry;
    s32 cursor;
    s16 lifetime;
    s32 opacity;

    entries = *(Game1E9A30Trail **)(actor + 0x94);
    if ((*(s8 *)(actor + 0x2C) < 2) && (*(u16 *)(actor + 0x1E) & 8)) {
        return 0;
    }
    cursor = *(s8 *)(actor + 0x2E);
    if (cursor != *(s8 *)(actor + 0x2D)) {
        do {
            cursor--;
            if (cursor < 0) {
                cursor = actor[0x25] - 1;
            }
            entry = &entries[cursor];
            entry->lifetime -= D_800BE9E4;
            lifetime = entry->lifetime;
            if (lifetime < 0) {
                if (cursor != *(s8 *)(actor + 0x2D)) {
                    do {
                        *(s8 *)(actor + 0x2D) += 1;
                        if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                            *(s8 *)(actor + 0x2D) = 0;
                        }
                        *(s8 *)(actor + 0x2C) -= 1;
                    } while (cursor != *(s8 *)(actor + 0x2D));
                }
            } else if (lifetime < 0xA) {
                opacity = lifetime * 0x19;
                if (opacity < entry->alpha) {
                    entry->alpha = opacity;
                }
            }
        } while (cursor != *(s8 *)(actor + 0x2D));
    }
    if (*(s8 *)(actor + 0x2C) > 0) {
        *(Vec3i_151BC64C *)(actor + 0x54) = *(Vec3i_151BC64C *)((u8 (*)[sizeof(Game1E9A30Trail)])entries)[*(s8 *)(actor + 0x2D)];
    } else {
        *(f32 *)(actor + 0x54) = 0.0f;
        *(f32 *)(actor + 0x58) = 0.0f;
        *(f32 *)(actor + 0x5C) = 0.0f;
    }
    return 1;
}

typedef struct Trail151BC794 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BC794;

typedef struct Src151BC794 {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Src151BC794;

typedef struct Data151BC794 {
    Src151BC794 *src;
    u8 unk4;
    u8 pad5[3];
    struct17 pos;
    f32 count;
    f32 unk18;
    f32 unk1C;
    f32 rot;
} Data151BC794;

typedef struct Actor151BC794 {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    Trail151BC794 *entries;
    Data151BC794 *data;
} Actor151BC794;

extern f32 func_15143E64(struct17 *);

s32 func_151BC794(Actor151BC794 *arg0) {
    Data151BC794 *data;
    Trail151BC794 *entries;
    Src151BC794 *src;
    struct17 diff;
    Trail151BC794 *entry;
    f32 dist;
    f32 inv;
    f32 rot0;

    data = arg0->data;
    entries = arg0->entries;
    src = data->src;
    if (src->unk0 == 0 || src->unk3B != data->unk4) {
        arg0->unk1E |= 8;
        return 1;
    }
    arg0->unk10 = src->unk14;
    arg0->unk14 = src->unk18;
    arg0->unk18 = src->unk1C;
    diff.unk0 = arg0->unk10 - data->pos.unk0;
    diff.unk4 = arg0->unk14 - data->pos.unk4;
    diff.unk8 = arg0->unk18 - data->pos.unk8;
    dist = func_15143E64(&diff);
    data->count += dist * 0.04870799929f * D_800BE9A4;
    data->unk1C += dist * 9.708835602f;
    if (data->count > 1.0f) {
        struct17 pos;
        f32 rot;
        f32 y;
        f32 yStep;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 delta;

        inv = 1.0f / data->count;
        y = data->unk18 + D_800BE9A4;
        pos = data->pos;
        rot0 = data->rot;
        rot = rot0;
        yStep = -(y * inv);
        dx = diff.unk0 * inv;
        dy = diff.unk4 * inv;
        dz = diff.unk8 * inv;
        delta = data->unk1C - rot0;
        do {
            entry = &entries[arg0->unk2E];
            entry->pos = pos;
            entry->lifetime = 12;
            entry->alpha = 100;
            entry->rot = rot;
            while (entry->rot > 16384.0f) {
                entry->rot -= 32768.0f;
            }
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
            rot += delta * inv;
            pos.unk0 += dx;
            pos.unk4 += dy;
            pos.unk8 += dz;
            y += yStep;
            data->count -= 1.0f;
        } while (data->count > 1.0f);
        data->pos = pos;
        data->rot = rot;
        data->unk18 = y;
    }
    return 1;
}

typedef struct Trail151BCA90 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BCA90;

typedef struct Actor151BCA90 {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Trail151BCA90 *entries;
} Actor151BCA90;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_151BCA90(Actor151BCA90 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Trail151BCA90 *entries;
    struct17 *cam;
    u8 update;
    struct17 b;
    struct17 a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 cx;
    f32 cy;
    f32 cz;
    s32 i;
    s32 j;
    f32 len[1];
    u8 alphaA;
    f32 rotB;
    f32 rotA;
    u8 alphaB;
    f32 scale;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->unk25 + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            entries = arg0->entries;
            cam = func_15144B34(arg2);
            update = 1;
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
            gfx = func_1513F4E4(gfx, 0x4E, &update);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
            if (arg0->unk1E & 2) {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = arg0->pos;
                alphaA = 0x80;
                rotA = 0.0f;
            } else {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                i = j - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = entries[j].pos;
                alphaA = entries[j].alpha;
                rotA = entries[j].rot;
            }
            b = entries[i].pos;
            alphaB = entries[i].alpha;
            rotB = entries[i].rot;

            ex = a.unk0 - b.unk0;
            ey = a.unk4 - b.unk4;
            ez = a.unk8 - b.unk8;
            dx = a.unk0 - cam->unk0;
            dy = a.unk4 - cam->unk4;
            dz = a.unk8 - cam->unk8;
            cx = (ey * dz) - (dy * ez);
            cy = (ez * dx) - (dz * ex);
            cz = (ex * dy) - (dx * ey);
            len[0] = (cx * cx) + (cy * cy) + (cz * cz);
            if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                cz = cy = cx = 0.0f;
            } else {
                scale = 35.0f / sqrtf(len[0]);
                cx = cx * scale;
                cy = cy * scale;
                cz = cz * scale;
            }
            vtx->v.ob[0] = a.unk0 + cx;
            vtx->v.ob[1] = a.unk4 + cy;
            vtx->v.ob[2] = a.unk8 + cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.unk0 - cx;
            vtx->v.ob[1] = a.unk4 - cy;
            vtx->v.ob[2] = a.unk8 - cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            do {
                j = i;
                ex = a.unk0 - b.unk0;
                ey = a.unk4 - b.unk4;
                ez = a.unk8 - b.unk8;
                dx = b.unk0 - cam->unk0;
                dy = b.unk4 - cam->unk4;
                dz = b.unk8 - cam->unk8;
                cx = (ey * dz) - (dy * ez);
                cy = (ez * dx) - (dz * ex);
                cz = (ex * dy) - (dx * ey);
                len[0] = (cx * cx) + (cy * cy) + (cz * cz);
                if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                    cz = cy = cx = 0.0f;
                } else {
                    scale = 35.0f / sqrtf(len[0]);
                    cx = cx * scale;
                    cy = cy * scale;
                    cz = cz * scale;
                }
                vtx->v.ob[0] = b.unk0 + cx;
                vtx->v.ob[1] = b.unk4 + cy;
                vtx->v.ob[2] = b.unk8 + cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.unk0 - cx;
                vtx->v.ob[1] = b.unk4 - cy;
                vtx->v.ob[2] = b.unk8 - cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                if (j != arg0->unk2D) {
                    a = b;
                    b = entries[i].pos;
                    alphaB = entries[i].alpha;
                    rotB = entries[i].rot;
                }
            } while (j != arg0->unk2D);
        }
    }
    return gfx;
}

void func_151BD21C(void *arg0, void *arg1, u8 arg2) {
    void *target;
    s32 current;
    s32 value;

    target = *(void **)((u8 *)arg0 + 0x98);
    current = *(s32 *)target;
    if (arg2 == 0) {
        if (current == *(s32 *)arg1 || *(u8 *)((u8 *)target + 4) == *(u8 *)((u8 *)arg1 + 4)) {
            u16 *flags = (u16 *)((u8 *)arg0 + 0x1E);

            *(s8 *)((u8 *)arg0 + 0x30) = 0;
            *flags |= 8;
        }
    } else if (arg2 == 0x2D) {
        value = *(s32 *)arg1;
        if (value == *(s32 *)target) {
            *(s32 *)target = *(s32 *)((u8 *)arg1 + 4);
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 9);
            return;
        }
        if (*(s32 *)((u8 *)arg1 + 4) == *(s32 *)target) {
            *(s32 *)target = value;
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 8);
        }
    }
}

s32 func_151BD2BC(s32 *arg0) {
    s32 *p = (s32 *)arg0[0xA];
    if (*p == 0) {
        return 0;
    }
    if (*((u8 *)arg0 + 0x2C) != *((u8 *)p + 0x3B)) {
        return 0;
    }
    return 1;
}

extern f32 D_800AA850;
extern f32 D_800AA860;
extern f32 D_800AA864;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25[0x1];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_E9FE0_2F8;

void *func_15134DAC(struct_E9FE0_2F8 *, s32);

void func_151BD2F8(void *arg0, s32 arg1, s32 arg2) {
    struct_E9FE0_2F8 sp24;
    struct { s16 unk0; u8 pad[2]; } sp20;
    void *temp_v0;

    if (arg0 == NULL) {
        return;
    }

    sp24.unk0 = *((u8 *)arg0 + 0x3B);
    sp24.unk4 = arg0;
    sp24.unk8 = 1;
    sp24.unkC = 31.0f;
    sp24.unk10 = -34.0f;
    sp24.unk14 = -117.0f;
    sp24.unk18 = 35.0f;
    sp24.unk1C = -24.0f;
    sp24.unk20 = -182.0f;
    sp24.unk24 = 0;
    sp24.unk26 = 0x32;
    sp24.unk28 = 0x64;
    sp24.unk2A = 0x12C;
    sp24.unk2E = 4;
    sp24.unk2F = 4;
    sp24.unk30 = 3;
    sp24.unk34 = D_800AA850;
    sp24.unk38 = 5;
    sp24.unk39 = -1;

    sp20.unk0 = 0;

    temp_v0 = func_15134DAC(&sp24, 2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp20, 2);
    }

    sp24.unkC = -sp24.unkC;
    sp24.unk18 = -sp24.unk18;

    temp_v0 = func_15134DAC(&sp24, 2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp20, 2);
    }
}

s32 func_151BD42C(u8 *arg0) {
    *(s16 *)(arg0 + 0x80) = 0;
    return 1;
}

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30[3];
    f32 unk3C[3];
    f32 unk48[3];
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
} Fx151BD43C;

typedef struct {
    u8 pad0[0x14];
    f32 unk14[3];
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
} Owner151BD43C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1C - 0xD];
    Owner151BD43C *unk1C;
    u8 pad20[0x74 - 0x20];
    f32 unk74;
} Actor151BD43C;

typedef struct {
    Owner151BD43C *owner;
    u8 ownerId;
} Extra151BD43C;

struct260 *func_15130374(void *, u8, s32, u8, s32);

void func_151BD43C(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3, f32 arg4, Actor151BD43C *arg5) {
    struct260 *ret;
    Owner151BD43C *owner;
    Fx151BD43C fx;
    f32 val;
    s32 pad;
    Extra151BD43C extra;
    s32 pad2;

    owner = arg5->unk1C;
    if ((owner->unk74 & 0xF) != 0xF) {
        fx.unk1D = 0x29;
        fx.unk08 = 0xE03;
        fx.unk00 = 0x200005;
        fx.unk04 = 0;
        fx.unk0C = 0;
        fx.unk10 = 0;
        fx.unk54 = ((func_150ADA68() * 251.0f) + 44.0f) * 0.001000000047f;
        fx.unk1E = 0x14;
        fx.unk20 = 0xC;
        fx.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (func_150ADA20() & 1 ? 0x80 : 0)) | 0x200 | 0x400 | 0x800 | 0x1000 | 0x4000 | 0x8000;
        fx.unk60 = 3;
        fx.unk61 = 3;
        fx.unk63 = -1;
        fx.unk64 = -1;
        fx.unk65 = 7;
        fx.unk22 = 0x22;
        fx.unk14 = 0xDD;
        fx.unk15 = 0xD3;
        fx.unk16 = 0xCD;
        fx.unk17 = 0xFF;
        fx.unk18 = 0x57;
        fx.unk19 = 0x55;
        fx.unk1A = 0x5A;
        val = 0.9347190261f;
        fx.unk24 = 1.034358025f;
        fx.unk1B = (func_150ADA20() % 156U) + 0x64;
        fx.unk1C = 0xFF;
        fx.unk0A = (func_150ADA20() % 18U) + 0x19;
        fx.unk28 = fx.unk2C = (func_150ADA68() * 53.0f) + 49.0f;
        fx.unk48[0] = (arg1[0] - arg0[0]) * arg5->unk74;
        fx.unk48[1] = (arg1[1] - arg0[1]) * arg5->unk74;
        fx.unk48[2] = (arg1[2] - arg0[2]) * arg5->unk74;
        fx.unk30[0] = arg0[0] - arg5->unk1C->unk14[0];
        fx.unk30[1] = arg0[1] - arg5->unk1C->unk14[1];
        fx.unk30[2] = arg0[2] - arg5->unk1C->unk14[2];
        fx.unk30[0] += fx.unk48[0] * arg4;
        fx.unk30[1] += fx.unk48[1] * arg4;
        fx.unk30[2] += fx.unk48[2] * arg4;
        extra.owner = arg5->unk1C;
        extra.ownerId = arg5->unk1C->unk3B;
        fx.unk62 = 0x13;
        fx.unk3C[0] = arg5->unk1C->unk14[0];
        fx.unk3C[1] = arg5->unk1C->unk14[1];
        fx.unk3C[2] = arg5->unk1C->unk14[2];
        ret = func_15130374(&fx, 1, 0x10, arg5->unkC, arg5->unk1);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0xA8, &val, 4);
            memcpy((u8 *)ret + 0xB0, &extra, 8);
        }
    }
}

f32 func_151BD750(s16 *arg0) {
    s16 temp;
    f32 ret;

    temp = arg0[0x40];
    ret = (1.0f + 1.0f) * (f32) temp * D_800AA860 + D_800AA864;
    arg0[0x40] = temp + D_800BE9E4;
    return ret;
}

extern void func_1513170C(void);

struct S151BD79C_sub {
    s32 unk0;
    u8 unk4;
    u8 pad5[0xF];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct S151BD79C {
    u8 pad0[0x4C];
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 padB0[0xB0 - 0x58];
    struct S151BD79C_sub *unkB0;
};

void func_151BD79C(struct S151BD79C *arg0) {
    struct S151BD79C_sub *v0 = arg0->unkB0;
    if (v0->unk0 != 0 && v0->unk4 != 0xFF) {
        arg0->unk4C = v0->unk14;
        arg0->unk50 = v0->unk18;
        arg0->unk54 = v0->unk1C;
    }
    func_1513170C();
}

void func_151BD7F4(s32 arg0) {
    s32 sp18[2];
    sp18[0] = arg0;
    *((u8 *)&sp18 + 4) = *((u8 *)arg0 + 0x3B);
    func_151494E0((s32)&sp18, 0x3B);
}

s32 func_151407D0(void *, u32, void *, u8, u8, u8, u8, s8, u8, s32);

typedef struct Game1E9A30BurstData {
    void *owner;
    u8 id;
    u8 pad05[3];
    void *sparks[2];
    void *glows[2];
} Game1E9A30BurstData;

typedef struct Game1E9A30BurstSpark {
    void *owner;
    u8 id;
    u8 index;
    u8 pad06[2];
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    void *object;
    f32 field54;
    s8 field58;
    u8 pad59[3];
} Game1E9A30BurstSpark;

typedef struct Game1E9A30BurstEffect {
    u8 kind;
    u8 mode;
    s16 flags;
    s16 lifetime;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 color[4];
    f32 field14;
    f32 field18;
    struct17 field1C;
    struct17 field28;
    f32 field34;
    f32 field38;
    f32 field3C;
    s32 field40;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    s32 field48;
    u8 field4C;
    u8 pad4D[0xB];
} Game1E9A30BurstEffect;

typedef struct Game1E9A30BurstGlowShape {
    f32 field00;
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3C;
    s32 field40;
    s32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    u8 field54;
    u8 field55;
    u8 field56;
    u8 field57;
    u8 field58;
    s8 field59;
    u8 pad5A[2];
} Game1E9A30BurstGlowShape;

typedef struct Game1E9A30BurstGlow {
    s16 field00;
    u8 pad02[2];
    f32 field04;
    void *object;
    u8 index;
    u8 pad0D[3];
    void *owner;
    u8 id;
    u8 pad15[3];
} Game1E9A30BurstGlow;

void func_151BD828(u8 *arg0, u8 arg1, s32 arg2) {
    void *object;
    Game1E9A30BurstData burst;
    Game1E9A30BurstData *data;

    burst.owner = arg0;
    burst.id = arg0[0x3B];
    burst.sparks[0] = 0;
    burst.sparks[1] = 0;
    burst.glows[0] = 0;
    burst.glows[1] = 0;
    object = func_15149130(0x12C, -1, 0x40, -1, 0, 0x33, (struct37 *)0x18, arg1, arg2);
    if (object != NULL) {
        data = (Game1E9A30BurstData *)((u8 *)object + 0x28);
        memcpy(data, &burst, sizeof(burst));
        {
            Game1E9A30BurstSpark spark;
            Game1E9A30BurstEffect sparkEffect;

            spark.field58 = 0;
            spark.field54 = (*(f32 *)(arg0 + 0x150) + *(f32 *)(arg0 + 0x14C)) * 0.5f;
            spark.owner = arg0;
            spark.id = arg0[0x3B];
            spark.field08 = 15.045272f;
            spark.field10 = 5.8676558f;
            spark.field18 = 12.111444f;
            spark.field0C = 0.396174f;
            spark.field24 = 0.0f;
            spark.field28 = 0.0f;
            spark.field38 = 0.0f;
            sparkEffect.field14 = 0.0f;
            sparkEffect.field18 = 0.0f;
            spark.field3C = 0.0f;
            spark.field48 = 1.0f;
            spark.field40 = 1.0f;
            spark.field4C = 1.0f;
            spark.field44 = 1.0f;
            spark.object = object;
            sparkEffect.kind = 0x5F;
            sparkEffect.mode = 8;
            sparkEffect.flags = 0x2203;
            sparkEffect.lifetime = 0x12C;
            sparkEffect.field08 = 0;
            sparkEffect.field0C = 0;
            sparkEffect.color[0] = 0xFF;
            sparkEffect.color[1] = 0xFF;
            sparkEffect.color[2] = 0xFF;
            sparkEffect.color[3] = 0xFF;
            spark.field14 = 0.1042563f;
            spark.field20 = 0.5197371f;
            spark.field1C = 0.34404588f;
            spark.field2C = 160.0f;
            spark.field30 = 95.0f;
            spark.field34 = 207.5f;
            sparkEffect.field1C = *(struct17 *)&D_800A5480;
            sparkEffect.field28 = *(struct17 *)&D_800A5480;
            sparkEffect.field34 = 1.0f;
            sparkEffect.field38 = 1.0f;
            sparkEffect.field3C = 1.0f;
            sparkEffect.field40 = 0x40EC0000;
            sparkEffect.field44 = 0;
            sparkEffect.field45 = 0xFF;
            sparkEffect.field46 = 0;
            sparkEffect.field47 = 7;
            sparkEffect.field48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                data->sparks[spark.index] = func_1513D2F0(&sparkEffect, &D_800A4AA0, 0, 0x25, 0, 0x1D,
                    ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, 0x5C, arg1, arg2);
                if (data->sparks[spark.index] != 0) {
                    memcpy((u8 *)data->sparks[spark.index] + 0x110, &spark, sizeof(spark));
                }
            }
        }
        {
            Game1E9A30BurstGlowShape shape;
            Game1E9A30BurstEffect glowEffect;
            Game1E9A30BurstGlow glow;

            glow.field00 = 0;
            glow.field04 = 1.0f;
            glow.owner = arg0;
            glow.object = object;
            glow.id = arg0[0x3B];
            shape.field00 = 50.3f;
            shape.field04 = 50.3f;
            shape.field08 = 30.9f;
            shape.field0C = 30.9f;
            shape.field10 = 0.1f;
            shape.field14 = 0.1f;
            shape.field1C = 0.7f;
            shape.field20 = 0.000001f;
            shape.field24 = -1;
            shape.field34 = -1;
            shape.field28 = -1;
            shape.field38 = -1;
            shape.field2C = -1;
            shape.field3C = -1;
            shape.field30 = -1;
            shape.field40 = -1;
            shape.field18 = 1.0f;
            shape.field44 = 0;
            shape.field48 = 1.0f;
            shape.field4C = 0;
            shape.field4E = 0;
            shape.field50 = 0;
            shape.field52 = 0;
            shape.field54 = 0;
            shape.field55 = 0;
            shape.field56 = 0;
            shape.field57 = 0;
            shape.field58 = 0xF;
            shape.field59 = -1;
            glowEffect.kind = 0x60;
            glowEffect.mode = 3;
            glowEffect.flags = 0x2203;
            glowEffect.lifetime = 0x12C;
            glowEffect.field08 = 0;
            glowEffect.field0C = 0;
            glowEffect.color[0] = 0xFF;
            glowEffect.color[1] = 0xFF;
            glowEffect.color[2] = 0xFF;
            glowEffect.color[3] = 0xFF;
            glowEffect.field14 = 100.0f;
            glowEffect.field18 = 100.0f;
            glowEffect.field1C = *(struct17 *)&D_800A5480;
            glowEffect.field28 = *(struct17 *)&D_800A5480;
            glowEffect.field34 = 1.0f;
            glowEffect.field38 = 1.0f;
            glowEffect.field3C = 1.0f;
            glowEffect.field40 = 0xCD2002;
            glowEffect.field44 = 0xFF;
            glowEffect.field45 = 0xFF;
            glowEffect.field46 = 0;
            glowEffect.field47 = 7;
            glowEffect.field48 = 0;
            glowEffect.field4C = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                data->glows[glow.index] = (void *)func_151407D0(&shape, 0x78, &glowEffect, 0, 0x2A, 0, 0, -1, arg1, arg2);
                if (data->glows[glow.index] != 0) {
                    memcpy((u8 *)data->glows[glow.index] + 0x170, &glow, sizeof(glow));
                }
            }
        }
    }
}

typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[8];
    u8 flags;
} Part151BDD8C;

typedef struct {
    u8 pad0[4];
    f32 unk4;
} Sub151BDD8C;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x110 - 0x4C];
    Part151BDD8C part;
} Obj151BDD8C_A;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0xC];
    s32 flags58;
    u8 pad5C[0x170 - 0x5C];
    Sub151BDD8C sub;
} Obj151BDD8C_B;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    Obj151BDD8C_A *a[2];
    Obj151BDD8C_B *b[2];
} Data151BDD8C;

typedef struct {
    u8 pad0[0xE];
    s16 state;
    u8 pad10[0x18];
    Data151BDD8C data;
} Effect151BDD8C;

extern struct17 D_800AA778;
extern struct17 D_800AA784;
extern struct17 D_800AA790;
extern struct17 D_800AA79C;
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_151BDD8C(Effect151BDD8C *effect) {
    Data151BDD8C *data;
    struct127 *owner;
    struct17 a[2];
    struct17 b[2];
    u8 i;
    u8 j;
    u8 k;
    s32 srcs[4];
    s32 dsts[4];
    s32 idx;

    data = &effect->data;
    owner = data->owner;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->unk1D4 != 0 && (owner->unk74 & 0xF) != 0xF) {
        Obj151BDD8C_A *oa;
        Obj151BDD8C_B *ob;
        s32 pad1;
        s32 pad2;

        srcs[0] = (s32)&D_800AA778;
        srcs[1] = (s32)&D_800AA784;
        srcs[2] = (s32)&D_800AA790;
        srcs[3] = (s32)&D_800AA79C;
        dsts[0] = (s32)&a[0];
        dsts[1] = (s32)&a[1];
        dsts[2] = (s32)&b[0];
        dsts[3] = (s32)&b[1];
        func_15145EA4(srcs, dsts, (s32)((Mtx *)owner->unk1D4 + 1), 4);
        for (i = 0; i < 2; i++) {
            if (data->a[i] != NULL) {
                oa = data->a[i];
                oa->part.flags |= 1;
                oa->start = a[i];
                oa->end = b[i];
            }
            if (data->b[i] != NULL) {
                ob = data->b[i];
                ob->flags58 |= 2;
                ob->flags58 &= ~4;
                ob->start = a[i];
                ob->end = b[i];
            }
        }
    } else {
        for (j = 0; j < 2; j++) {
            if (data->a[j] != NULL) {
                Part151BDD8C *part = &data->a[j]->part;

                part->flags &= ~1;
            }
            if (data->b[j] != NULL) {
                data->b[j]->flags58 &= ~2;
            }
        }
    }
    idx = owner - D_800CC2D0;
    if (idx < 4) {
        for (k = 0; k < 2; k++) {
            if (data->a[k] != NULL) {
                Part151BDD8C *part = &data->a[k]->part;

                if (*((u16 **)D_800BE728[0])[(u8)idx] & 0x8000) {
                    part->unk48 = 6.0f;
                    part->unk4C = 21.0f;
                } else {
                    part->unk48 = 3.0f;
                    part->unk4C = 8.0f;
                }
            }
            if (data->b[k] != NULL) {
                Sub151BDD8C *sub = &data->b[k]->sub;

                if (*((u16 **)D_800BE728[0])[(u8)idx] & 0x8000) {
                    sub->unk4 = 1.5f;
                } else {
                    sub->unk4 = 1.0f;
                }
            }
        }
    }
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BE0AC;

typedef struct {
    s32 unk0;
    u8  b4;
} ArgB_151BE0AC;

void func_151BE0AC(s32 arg0, s32 arg1, u8 arg2) {
    s32 *pp = &arg0;
    s32 t;
    if (arg2 == 0x3B) {
        t = *pp + 0x28;
        if ((*(s32 *)t == *(s32 *)arg1) ||
            (*(u8 *)(t + 4) == *(u8 *)(pp[1] + 4))) {
            func_1516972C((struct102 *)*pp);
        }
    } else {
        t = *pp + 0x28;
        func_15169850(arg1, arg2, t, t + 4, *pp);
    }
}

void func_151BE138(struct260 *arg0) {
    struct102 **p;

    p = (struct102 **)((u8 *)arg0 + 0x28);
    if (p[2] != 0) {
        func_1516972C(p[2]);
    }
    if (p[3] != 0) {
        func_1516972C(p[3]);
    }
    if (p[4] != 0) {
        func_1516972C(p[4]);
    }
    if (p[5] != 0) {
        func_1516972C(p[5]);
    }
}

extern void func_151BE138(struct260 *arg0);

void func_151BE1B8(struct260 *arg0) {
    func_151BE138(arg0);
    func_1514933C(arg0);
}

extern void func_15149368(struct260 *arg0);

void func_151BE1E4(struct260 *arg0) {
    func_151BE138(arg0);
    func_15149368(arg0);
}
