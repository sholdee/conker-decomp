#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} Vec2_15102B38;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3_15102B38;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    Vec2_15102B38 unk14;
    Vec3_15102B38 unk1C;
    Vec3_15102B38 unk28;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} Struct15102B38;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    s16 unkC;
    u8 padE[2];
    s32 unk10;
    u8 unk14;
    u8 pad15[3];
    Vec3_15102B38 unk18;
    Vec3_15102B38 unk24;
} Payload15102B38;

typedef struct {
    Payload15102B38 payload;
    Struct15102B38 header;
    s32 temp_v0;
} Locals15102B38;

extern void func_15103254(s16 arg0, u8 arg1, f32 arg2, Vec3_15102B38 *arg3, u8 arg4, u8 arg5, s32 arg6);


typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} Struct15102920;

struct210 *func_15102920(f32 arg0, u8 arg1, s32 arg2, f32 *arg3, s16 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9) {
    s32 temp_v0;
    Struct15102920 sp64;
    s32 sp60;
    s32 sp5C;
    s32 pad;

    sp64.unk22 = 0xA;
    sp64.unk24 = 0x19;
    temp_v0 = func_1510F8CC(arg7);
    sp64.unk06 = 0x5C;
    switch (D_800BE9F0) {
    case 0x36:
        if (temp_v0 == 9) {
            return NULL;
        }
        sp64.unk06 = 0x85;
        arg0 *= 0.6000000238f;
        break;
    case 0x26:
        if (temp_v0 == 2) {
            return NULL;
        }
        break;
    }
    sp64.unk07 = 0;
    sp64.unk00 = ((arg4 == -1) ? 0 : 1) | 0x100 | 0x200 | 0x400 | 0x1000 | 0x8000 | 0x20000;
    sp64.unk04 = (arg4 == -1) ? 0x12C : arg4;
    sp64.unk08 = 0;
    sp64.unk0C = 0;
    if (D_800BE9F0 == 0x36) {
        sp64.unk10 = arg1 >> 1;
    } else {
        sp64.unk10 = arg1;
    }
    sp64.unk11 = 0xFF;
    sp64.unk14 = 0;
    sp64.unk13 = 0;
    sp64.unk12 = 0;
    sp64.unk15 = 0xFF;
    sp64.unk18 = ((arg6 != 0) ? 2 : 1) + 0x3B0000;
    sp64.unk16 = 0;
    sp64.unk17 = 7;
    if (arg5 != 0) {
        sp60 = 3;
        sp5C = 0xFF;
    } else {
        sp60 = 0;
        sp5C = 0;
    }
    return func_1513C650((s32)&sp64, 0, 0, arg2, arg3[0], arg3[1], arg3[2], arg0, arg0, func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), sp60, sp5C, 0, arg8, arg9);
}

s32 func_15102B38(struct127 *arg0, u8 arg1, Vec3_15102B38 *arg2, Vec3_15102B38 *arg3, Vec2_15102B38 *arg4, s16 arg5, u8 arg6, f32 arg7, s32 arg8, u8 arg9, s32 argA, s16 argB, u8 argC, s32 argD) {
    Locals15102B38 sp;
    s32 flag;

    if (arg0 == NULL) {
        return 0;
    }

    sp.payload.unk14 = 0;
    sp.payload.unk18 = *(Vec3_15102B38 *)&D_800A5480;
    sp.payload.unk24 = *(Vec3_15102B38 *)&D_800A5480;
    sp.payload.unk10 = arg1;
    sp.payload.unk0 = arg0;
    sp.payload.unkC = argB;
    sp.payload.unk4 = arg0->unique_id;
    sp.payload.unk8 = arg1 << 6;
    sp.header.unk0 = 0x5F;
    sp.header.unk1 = 5;
    sp.header.unk2 = 0x2203;
    sp.header.unk8 = 0;
    sp.header.unkC = 0;
    sp.header.unk4 = arg5;
    sp.header.unk10 = 0xFF;
    sp.header.unk11 = 0xFF;
    sp.header.unk12 = 0xFF;
    sp.header.unk13 = 0xFF;
    sp.header.unk14 = *arg4;
    sp.header.unk1C = *arg2;
    sp.header.unk28 = *arg3;
    sp.header.unk40 = 0x40CC0009;
    sp.header.unk44 = arg6;
    sp.header.unk45 = 0xFF;
    sp.header.unk46 = 0;
    sp.header.unk47 = 7;
    sp.header.unk34 = 0.0f;
    sp.header.unk38 = 0.0f;
    sp.header.unk3C = 0.0f;
    sp.header.unk4C = arg9;

    if (func_150ADA20() & 1) {
        flag = 2;
    } else {
        flag = 0;
    }

    sp.temp_v0 = (s32)func_1513D2F0(&sp.header, &D_800A4AA0, 0x29, 0, 0, 0x16, flag + 1, 0, 0, argA + 0x30, argC, argD);
    if (sp.temp_v0 != 0) {
        memcpy((void *)(sp.temp_v0 + 0x110), &sp.payload, 0x30);
    }

    func_15103254(arg5, arg6, arg7, arg8, arg9, argC, argD);
    return sp.temp_v0;
}

void func_15102D50(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x110, arg0 + 0x114, arg0);
}

typedef struct {
    u8 pad0[0x34];
    Vec3_15102B38 unk34;
    Vec3_15102B38 unk40;
    u8 pad4C[0xC4];
    Payload15102B38 unk110;
} Obj15102D90;

extern s32 func_1514654C(struct127 *, struct126 *, s32, void *, void *, s32);
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

s32 func_15102D90(Obj15102D90 *arg0) {
    struct127 *actor;
    Payload15102B38 *p;
    s32 src[2];
    s32 dst[2];

    p = &arg0->unk110;
    p->unk14 &= ~1;
    actor = p->unk0;
    if (actor->unique_id != p->unk4 || actor->interaction_state == 0) {
        return 0;
    }
    if (actor->unk1D4 == NULL || (actor->unk74 & 0xF) == 0xF) {
        return 1;
    }
    src[0] = (s32)&arg0->unk34;
    src[1] = (s32)&arg0->unk40;
    dst[0] = (s32)&p->unk18;
    dst[1] = (s32)&p->unk24;
    if (p->unkC != -1) {
        struct126 *temp;

        temp = func_1503195C(actor, p->unkC, 0);
        if (temp == NULL) {
            return 0;
        }
        if (func_1514654C(actor, temp, p->unk10, src, dst, 2) == 0) {
            return 0;
        }
    } else {
        func_15145EA4(src, dst, (s32)actor->unk1D4 + p->unk8, 2);
    }
    p->unk14 |= 1;
    return 1;
}

typedef struct {
    u8 pad0[0x14];
    u8 flags;
    u8 pad15[3];
    struct17 start;
    struct17 end;
} Seg_15102EB8;

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 t;
    u8 pad34[0x8C];
    u8 tmpl[0x40];
    u8 *buffers[4];
    Seg_15102EB8 seg;
} Obj_15102EB8;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    u8 pad8[8];
} Vtx_15102EB8;

extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
void func_15145548(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 *arg4);

void *func_15102EB8(Obj_15102EB8 *arg0, s16 arg1) {
    struct17 *pos;
    struct17 mid;
    struct17 dir;
    Seg_15102EB8 *seg;
    struct17 closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vtx_15102EB8 *vertices;
    void *result;
    f32 t;
    f32 lengthSquared;
    u8 fresh;
    f32 scale;

    pos = func_15144B34(arg1);
    if (!(arg0->seg.flags & 1)) {
        return NULL;
    }
    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    seg = &arg0->seg;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(arg0->buffers[arg1], arg0->tmpl, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->tmpl, 0x40);
        }
    } else {
        return NULL;
    }
    mid.unk0 = seg->start.unk0 + (seg->end.unk0 - seg->start.unk0) * arg0->t;
    mid.unk4 = seg->start.unk4 + (seg->end.unk4 - seg->start.unk4) * arg0->t;
    mid.unk8 = seg->start.unk8 + (seg->end.unk8 - seg->start.unk8) * arg0->t;
    dir.unk0 = mid.unk0 - seg->start.unk0;
    dir.unk4 = mid.unk4 - seg->start.unk4;
    dir.unk8 = mid.unk8 - seg->start.unk8;
    func_15145548(&seg->start, &dir, pos, &closest, &t);
    dx = closest.unk0 - pos->unk0;
    dy = closest.unk4 - pos->unk4;
    dz = closest.unk8 - pos->unk8;
    nx = dir.unk4 * dz - dy * dir.unk8;
    ny = dir.unk8 * dx - dz * dir.unk0;
    nz = dir.unk0 * dy - dx * dir.unk4;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = seg->start.unk0 + nx;
    vertices->y = seg->start.unk4 + ny;
    vertices->z = seg->start.unk8 + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.unk0 + nx;
    vertices->y = mid.unk4 + ny;
    vertices->z = mid.unk8 + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = mid.unk0 - nx;
    vertices->y = mid.unk4 - ny;
    vertices->z = mid.unk8 - nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = seg->start.unk0 - nx;
    vertices->y = seg->start.unk4 - ny;
    vertices->z = seg->start.unk8 - nz;
    vertices->flag = 0;
    return result;
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
    Vec3_15102B38 unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[13];
} Fx15103254;

extern s32 D_80088BD0[];
extern void *func_15130374(void *, u8, s32, u8, s32);

void func_15103254(s16 arg0, u8 arg1, f32 arg2, Vec3_15102B38 *arg3, u8 arg4, u8 arg5, s32 arg6) {
    Fx15103254 sp2C;

    sp2C.unk1D = *(D_80088BD0 + (func_150ADA20() & 3));
    sp2C.unk08 = 0x2203;
    sp2C.unk00 = 0x200005;
    sp2C.unk04 = 0x20000;
    sp2C.unk0A = arg0 + 1;
    sp2C.unk0C = 0;
    sp2C.unk10 = 0;
    sp2C.unk14 = 0xFF;
    sp2C.unk15 = 0xFF;
    sp2C.unk16 = 0xFF;
    sp2C.unk17 = 0xFF;
    sp2C.unk18 = 0xFF;
    sp2C.unk19 = 0xFF;
    sp2C.unk1A = 0xFF;
    sp2C.unk1B = arg1;
    sp2C.unk1C = 0xFF;
    sp2C.unk2C = arg2;
    sp2C.unk28 = arg2;
    sp2C.unk30 = *arg3;
    sp2C.unk1E = 3;
    sp2C.unk20 = 0x55;
    sp2C.unk22 = 1;
    sp2C.unk3C = 0.0f;
    sp2C.unk40 = 0.0f;
    sp2C.unk44 = 0.0f;
    sp2C.unk48 = 0.0f;
    sp2C.unk4C = 0.0f;
    sp2C.unk50 = 0.0f;
    sp2C.unk54 = 0.0f;
    sp2C.unk24 = 1.0f;
    sp2C.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 1) | 0x200 | 0x4000 | 0x8000 | 0x40000 | 0x800000;
    sp2C.unk60 = 6;
    sp2C.unk61 = 8;
    sp2C.unk62 = -1;
    sp2C.unk63 = -1;
    sp2C.unk64 = -1;
    sp2C.unk65 = 0;
    sp2C.unk66 = arg4;
    func_15130374(&sp2C, 1, 0, arg5, arg6);
}
