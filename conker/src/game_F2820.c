#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 pad30[0xC0 - 0x30];
    f32 unkC0;
} Obj150C5370;

Obj150C5370 *func_150C5370(void *arg0, s32 arg1) {
    Obj150C5370 *obj;
    struct17 diff;

    obj = func_15167A68(0x46, 0, arg1 + 0xC8, 1, 0xFF, 1);
    if (obj == NULL) {
        return NULL;
    }
    memcpy(&obj->unk18, arg0, 0x24);
    {
        extern f32 func_15143E64(struct17 *);

        diff.unk0 = obj->unk18 - obj->unk24;
        diff.unk4 = obj->unk1C - obj->unk28;
        diff.unk8 = obj->unk20 - obj->unk2C;
        obj->unkC0 = func_15143E64(&diff);
    }
    obj->unk10 = 1;
    obj->unk14 = 0;
    return obj;
}


void func_150C5430(struct102 *arg0) {
    func_15169804(arg0);
}

void func_150C5450(struct102 *arg0) {
    func_15169824(arg0);
}

extern void (*D_800887B8[])(void);

void func_150C5470(struct102 *arg0) {
    s32 temp = *(u8 *)((u8 *)arg0 + 0x38);

    if (temp < 0) {
        temp = 0;
    } else if (temp >= 2) {
        temp = 0;
    }
    D_800887B8[temp]();
}

extern void (*D_800887C0[])(void);

void func_150C54C0(struct102 *arg0) {
    s32 temp = *(u8 *)((u8 *)arg0 + 0x38);

    if (temp < 0) {
        temp = 0;
    } else if (temp >= 2) {
        temp = 0;
    }
    D_800887C0[temp]();
}

extern void func_1514EDF0(struct102 *arg0, s32 arg1);

void func_150C5510(struct102 *arg0) {
    func_1514EDF0(arg0, *(s32 *)((u8 *)arg0 + 0xC8));
    func_150C5430(arg0);
}

void func_150C553C(struct102 *arg0) {
    func_1514EDF0(arg0, *(s32 *)((u8 *)arg0 + 0xC8));
    func_150C5450(arg0);
}

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s8 unk30;
    u8 pad31[0xC0 - 0x31];
    f32 unkC0;
} Obj150C5568;

extern s32 (*D_80088760[])(Obj150C5568 *);

void func_150C5568(Obj150C5568 *arg0) {
    s32 idx;
    u8 done;
    struct17 prev;

    if (arg0->unk10 & 1) {
        done = 0;
        idx = arg0->unk30;
        prev.unk0 = arg0->unk18 - arg0->unk24;
        prev.unk4 = arg0->unk1C - arg0->unk28;
        prev.unk8 = arg0->unk20 - arg0->unk2C;

        if (idx != -1) {
            if (D_80088760[idx](arg0) == 0) {
                done = 1;
            }
        }
        if (!done) {
            struct17 diff;
            s32 pad;
            extern f32 func_15143E64(struct17 *);

            diff.unk0 = arg0->unk18 - arg0->unk24;
            diff.unk4 = arg0->unk1C - arg0->unk28;
            diff.unk8 = arg0->unk20 - arg0->unk2C;
            if (diff.unk0 != prev.unk0 || diff.unk4 != prev.unk4 || diff.unk8 != prev.unk8) {
                arg0->unkC0 = func_15143E64(&diff);
            }
        }
        if (done) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 pad30[0x4];
    f32 unk34;
    u8 pad38[0x8];
    Vtx unk40[2][4];
    f32 unkC0;
} Obj150C56A4;

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct150C56A4_mode;

typedef struct {
    f32 x, y, z;
} Vec3f_150C56A4;

extern u8 D_80090DE8[];
extern Struct150C56A4_mode D_800A4AC8[];
extern s32 D_800D2C9C;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);

Gfx *func_150C56A4(Gfx *gfx, Obj150C56A4 *arg1, s16 arg2) {
    Gfx *vtxCmd;
    u8 update;
    Vtx *vtx;
    f32 *cam;
    Vec3f_150C56A4 a;
    Vec3f_150C56A4 b;
    Vec3f_150C56A4 n;
    f32 lensq;
    f32 scale;
    s32 t;

    if (arg1->unk10 & 1) {
        n.z = a.z;
        scale = a.y * b.z - b.y * a.z;
        lensq = a.z * b.x - b.z * a.x;
        b.z = a.z = 0.0f;
        update = 1;
        vtx = arg1->unk40[D_800BE9C0];
        cam = &D_800DBFF0[arg2].unk2F8;
        gfx = func_15142B7C(gfx, 1, 0x600);
        gfx = func_15142E24(gfx, (s32)D_80090DE8, 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
        if (D_800DBFF0[arg2].unk5F0 & 1) {
            gfx = func_15142C10(gfx, D_800D9B78[arg2][0], D_800D9B78[arg2][1], D_800D9B78[arg2][2], 0xFF, &update);
            gfx = func_15142CF0(gfx, 0xF2, 0, D_800D9B68[arg2][0], D_800D9B68[arg2][1], D_800D9B68[arg2][2], 0xFF, &update);
            gfx = func_1513F4E4(gfx, 0x1E, &update);
        } else {
            gfx = func_15142C10(gfx, 0, 0, 0, 0, &update);
            gfx = func_15142CF0(gfx, 0, 0, 0, 0, 0, 0, &update);
            gfx = func_1513F4E4(gfx, 0x13, &update);
        }
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].unk0 | D_800A4AC8[1].unk4, &update);

        scale = arg1->unk2C;
        a.x = arg1->unk24 - arg1->unk18;
        a.y = arg1->unk28 - arg1->unk1C;
        a.z = arg1->unk2C - arg1->unk20;
        b.x = arg1->unk24 - cam[0];
        b.y = arg1->unk28 - cam[1];
        b.z = arg1->unk2C - cam[2];
        n.x = a.y * b.z - b.y * a.z;
        n.y = a.z * b.x - b.z * a.x;
        n.z = a.x * b.y - b.x * a.y;
        lensq = n.x * n.x + n.y * n.y + n.z * n.z;
        if (lensq == 0.0f) {
            n.x = 0.0f;
            n.y = 0.0f;
            n.z = 0.0f;
        } else {
            scale = 8.399999619f / sqrtf(lensq);
            n.x *= scale;
            n.y *= scale;
            n.z *= scale;
        }
        t = arg1->unkC0 * arg1->unk34 * 64.0f;
        vtx->v.ob[0] = arg1->unk24 + n.x;
        vtx->v.ob[1] = arg1->unk28 + n.y;
        vtx->v.ob[2] = arg1->unk2C + n.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = arg1->unk24 - n.x;
        vtx->v.ob[1] = arg1->unk28 - n.y;
        vtx->v.ob[2] = arg1->unk2C - n.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = arg1->unk18 - n.x;
        vtx->v.ob[1] = arg1->unk1C - n.y;
        vtx->v.ob[2] = arg1->unk20 - n.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = t;
        vtx++;
        vtx->v.ob[0] = arg1->unk18 + n.x;
        vtx->v.ob[1] = arg1->unk1C + n.y;
        vtx->v.ob[2] = arg1->unk20 + n.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = t;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Sub150C5B88;

s32 func_150C5B88(struct102 *arg0) {
    Sub150C5B88 **temp_v1;

    temp_v1 = (Sub150C5B88 **)((u8 *)arg0 + 0xC8);
    if ((*temp_v1)->unk0 == 0) {
        return 0;
    }
    *(f32 *)((u8 *)arg0 + 0x24) = (*temp_v1)->unk14;
    *(f32 *)((u8 *)arg0 + 0x28) = (*temp_v1)->unk18;
    *(f32 *)((u8 *)arg0 + 0x2C) = (*temp_v1)->unk1C;
    return 1;
}

extern void func_15143134(void *, s32, s32);
s32 func_150C5BD4(struct102 *arg0)
{
  u8 *p = ((u8 *) arg0) + 0xC8;
  u32 offset;
  s32 t0;
  if ((*((s32 *) (*((s32 *) p)))) == 0)
  {
    return 0;
  }
  if (p[4] != (*(((u8 *) (*((s32 *) p))) + 0x3B)))
  {
    return 0;
  }
  t0 = *((s32 *) (((u8 *) (*((s32 *) p))) + 0x1D4));
  if (t0 == 0)
  {
    *((f32 *) (((u8 *) arg0) + 0x24)) = *((f32 *) (((u8 *) (*((s32 *) p))) + 0x14));
    *((f32 *) (((u8 *) arg0) + 0x28)) = *((f32 *) (((u8 *) (*((s32 *) p))) + 0x18));
    *((f32 *) (((u8 *) arg0) + 0x2C)) = *((f32 *) (((u8 *) (*((s32 *) p))) + 0x1C));
  }
  else
  {
    offset = (p[5] << 4) << 2;
    func_15143134(p + 0x8, (s32) (((u8 *) arg0) + 0x24), t0 + offset);
  }
  return 1;
}


void func_150C5C74(s32 arg0) {
    func_1514D3B0(arg0, 0x15, 1, 0);
}

void func_150C5C9C(s32 arg0) {
    func_1514D3B0(arg0, 0x15, 2, 0);
}

extern void (*D_800887C8[])(struct102 *, s32, u8);

void func_150C5CC4(struct102 *arg0, s32 arg1, u8 arg2) {
    void (*temp)(struct102 *, s32, u8) = D_800887C8[*(u8 *)((u8 *)arg0 + 0x38)];

    if (temp != NULL) {
        temp(arg0, arg1, arg2);
    }
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA150C5D0C;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB150C5D0C;

void func_150C5D0C(struct102 *arg0, s32 arg1, u8 arg2) {
    SubA150C5D0C *temp_v0 = (SubA150C5D0C *)((u8 *)arg0 + 0xC8);
    ArgB150C5D0C *b = (ArgB150C5D0C *)arg1;

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C(arg0);
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
