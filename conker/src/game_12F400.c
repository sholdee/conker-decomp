#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60(Vtx **buffers, s16 index, s32 size, Vtx **out, u8 *isNew);
extern s32 func_15144E80(void *, struct17 *, struct17 *, struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;

typedef struct {
    ObjectWithState197 *field_0x0;
    char pad_0[0x1E];
    u8 field_0x22;
    char pad_1[0x1];
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
} Field124ChildBlock;

typedef struct {
    char pad_0[0x18];
    s32 field_0x18;
    char pad_1[0x8C];
    s32 field_0xA8;
    char pad_2[0x24];
    s32 field_0xD0;
    u8 field_0xD4;
    char pad_3[0x4F];
    s32 field_0x124;
    char pad_4[0x48];
    s32 field_0x170;
    u8 field_0x174;
} Field124Owner;

/* A camera-facing quad: four vertices around `center`, spanned by two axes scaled by scaleU/scaleV. */
typedef struct {
    char pad0[0x2C];
    f32 scaleU;
    f32 scaleV;
    struct17 center;
    char pad40[0x80];
    Vtx templateVtx[4];
    Vtx *vtxBuffers[4];
    u8 flags;
    u8 unk111;
    u8 unk112[0xC];
} Struct15101F50;

/* Fetches vertex buffer `index` (initialised from templateVtx when new) and fills in the quad's corners. */
Vtx *func_15101F50(Struct15101F50 *quad, s16 index) {
    s32 pad; // matching: unused stack slot in the original frame
    Vtx *vtx;
    Vtx *ret;
    struct17 axisU;
    struct17 axisV;
    u8 isNew;

    func_151D5D60(quad->vtxBuffers, index, sizeof(quad->templateVtx), &vtx, &isNew);
    ret = vtx;
    // matching: an early `if (vtx == NULL) return NULL;` lays out the branches differently
    if (vtx != NULL) {
        if (isNew) {
            memcpy(quad->vtxBuffers[index], quad->templateVtx, sizeof(quad->templateVtx));
            memcpy(quad->vtxBuffers[index] + 4, quad->templateVtx, sizeof(quad->templateVtx));
        }
    } else {
        return NULL;
    }

    if (!(quad->flags & 1)) {
        return NULL;
    }

    if (func_15144E80(quad->unk112, &axisU, &axisV, NULL) == 0) {
        return NULL;
    }

    func_15145128(&axisU, &axisU, NULL, NULL);
    func_15145128(&axisV, &axisV, NULL, NULL);

    axisU.unk0 *= quad->scaleU;
    axisU.unk4 *= quad->scaleU;
    axisU.unk8 *= quad->scaleU;
    axisV.unk0 *= quad->scaleV;
    axisV.unk4 *= quad->scaleV;
    axisV.unk8 *= quad->scaleV;

    vtx[0].v.ob[0] = (quad->center.unk0 - axisU.unk0) + axisV.unk0;
    vtx[0].v.ob[1] = (quad->center.unk4 - axisU.unk4) + axisV.unk4;
    vtx[0].v.ob[2] = (quad->center.unk8 - axisU.unk8) + axisV.unk8;
    vtx[0].v.flag = 0;
    vtx[1].v.ob[0] = (quad->center.unk0 + axisU.unk0) + axisV.unk0;
    vtx[1].v.ob[1] = (quad->center.unk4 + axisU.unk4) + axisV.unk4;
    vtx[1].v.ob[2] = (quad->center.unk8 + axisU.unk8) + axisV.unk8;
    vtx[1].v.flag = 0;
    vtx[2].v.ob[0] = (quad->center.unk0 + axisU.unk0) - axisV.unk0;
    vtx[2].v.ob[1] = (quad->center.unk4 + axisU.unk4) - axisV.unk4;
    vtx[2].v.ob[2] = (quad->center.unk8 + axisU.unk8) - axisV.unk8;
    vtx[2].v.flag = 0;
    vtx[3].v.ob[0] = (quad->center.unk0 - axisU.unk0) - axisV.unk0;
    vtx[3].v.ob[1] = (quad->center.unk4 - axisU.unk4) - axisV.unk4;
    vtx[3].v.ob[2] = (quad->center.unk8 - axisU.unk8) - axisV.unk8;
    vtx[3].v.flag = 0;

    return ret;
}

extern f32 D_800BE9A4;

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

typedef struct {
    u8 pad0[0x24];
    u8 *unk24;
    Sub151022AC_28 *unk28;
    Sub151022AC_2C *unk2C;
    u8 pad30[4];
    Sub151022AC_34 *unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0xC];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
} Block151022AC;

s32 func_151022AC(u8 *arg0) {
    Block151022AC *p;
    Sub151022AC_28 *s28;
    Sub151022AC_2C *s2C;
    Sub151022AC_34 *s34;
    f32 tmp;

    *(f32 *)(arg0 + 0x160) += *(f32 *)(arg0 + 0x164) * D_800BE9A4;
    p = (Block151022AC *)(arg0 + 0x110);
    p->unk50 = func_15144B68(*(f32 *)(arg0 + 0x160));
    p->unk58 += p->unk5C * D_800BE9A4;
    while (p->unk58 > 32767.0f) {
        p->unk58 -= 65536.0f;
    }
    while (p->unk58 < -32768.0f) {
        p->unk58 += 65536.0f;
    }
    p->unk68 -= D_800BE9A4;
    if (p->unk68 < 0.0f) {
        p->unk68 = func_150ADA68() * p->unk6C;
        p->unk60 = func_150ADA68() * (p->unk64 + p->unk64) - p->unk64;
    }
    p->unk5C += (p->unk60 - p->unk5C) * p->unk70;
    p->unk38 += (p->unk3C - p->unk38) * p->unk40;
    if (p->unk28 != NULL) {
        s28 = p->unk28;
        tmp = (func_150ADA68() * 0.200000003f + 0.400000006f) * p->unk38;
        s28->unk30 = tmp;
        s28->unk2C = tmp;
    }
    p->unk84 -= D_800BE9A4;
    if (p->unk84 < 0.0f) {
        p->unk84 = func_150ADA68() * p->unk88;
        p->unk78 = func_150ADA68() * p->unk80 + p->unk7C;
    }
    p->unk74 += (p->unk78 - p->unk74) * p->unk8C;
    arg0[0x5C] = p->unk74;
    if (p->unk28 != NULL) {
        p->unk28->unk5C = arg0[0x5C];
    }
    if (p->unk24 != NULL) {
        p->unk24[0x2B] = arg0[0x5C];
    }
    if (p->unk2C != NULL) {
        s2C = p->unk2C;
        s2C->unk14[0x2F] = (func_150ADA20() & 3) + 2;
    }
    if (p->unk34 != NULL) {
        s34 = p->unk34;
        tmp = func_150ADA68() * 10.0f + 120.0f;
        s34->unk3C = tmp;
        s34->unk38 = tmp;
    }
    return 1;
}

s32 func_151025F4(u8 *arg0, s32 arg1) {
    f32 *v1 = (f32 *)(arg0 + 0xA8);
    f32 sin0[2];
    f32 sin1;

    sin0[0] = sinf(v1[6]);
    sin1 = sinf(v1[7]);
    v1[0] = v1[4] * sin0[0] + v1[2];
    v1[1] = v1[5] * sin1 + v1[3];
    v1[6] = v1[8] * D_800BE9A4 + v1[6];
    v1[7] = v1[9] * D_800BE9A4 + v1[7];
    v1[6] = func_15144B68(v1[6]);
    v1[7] = func_15144B68(v1[7]);
    return 1;
}

extern f32 D_800A21A0[][3];
extern f32 D_800A21F4[][3];
extern u8 D_800A2248[];
s32 func_15100600(u8 *, f32 *, f32 *, u8, s16, u8, u8, u8, f32, f32, f32, f32, u8, u8, s32);

s32 func_151026BC(u8 *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5) {
    s32 a;
    s32 b;

    if (arg0 == NULL) {
        return 0;
    }
    switch (arg0[4]) {
        case 0x58:
        case 0x5B:
        case 0x80:
            a = 0x32;
            b = 0xFF;
            break;
        case 0x5A:
        case 0x5F:
        case 0x74:
        case 0x75:
        case 0x7A:
            a = 0xFF;
            b = 0;
            break;
        default:
            if ((*(u8 **)(arg0 + 0x31C))[0x78] == 0x3B) {
                a = 0xFF;
                b = 0;
            } else {
                a = 0x32;
                b = 0xFF;
            }
            break;
    }
    return func_15100600(arg0, D_800A21A0[arg2], D_800A21F4[arg2], D_800A2248[arg2], arg1, a, b, 0, 20.0f, 20, 4.0f, 0.07999999821f, arg3, arg4, arg5);
}

void func_151027E8(struct210 *arg0) {
    s8 sp18[5];

    *(struct210 **)&sp18[0] = arg0;
    sp18[4] = *(u8 *)((s32)arg0 + 0x3B);
    func_151403A8(&sp18, 0x1A);
}

s32 func_1510281C(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0xD4 & 1)) {
        return 0;
    }
    return 1;
}

s32 func_15102884(struct210 *arg0, s32 arg1) {
    if (!(*(u8 *)((s32)arg0 + 0xAC) & 1)) {
        return 0;
    }
    return 1;
}

s32 func_151028AC(Field124Owner *arg0, s16 arg1) {
    Field124ChildBlock *p = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    ObjectWithState197 *v1;

    if (arg1 != p->field_0x22) {
        goto tail;
    }
    v1 = p->field_0x0;
    if (v1 != 0) {
        if (v1->field_0x31C->field_0x197 == 0) {
            goto tail;
        }
    }
    return 0;
tail:
    if (!(arg0->field_0x174 & 1)) {
        return 0;
    }
    return 1;
}
