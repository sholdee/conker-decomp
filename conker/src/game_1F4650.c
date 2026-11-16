#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1513D4B8 func_1513D4B8_void_decl
#include "functions.h"
#undef func_150ADA20
#undef func_1513D4B8
s32 func_1513D4B8(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800AAED4;
extern f32 D_800AAED8;
extern f32 D_800AAEA8;
extern f32 D_800AAEAC;
extern f32 D_800AAEB0;
extern f32 D_800AAEB4;
extern f32 D_800AAEB8;
extern f32 func_151CC1D4(void *);
extern f32 sinf(f32);
extern void func_1514373C(f32, f32, f32 *, f32 *);
extern void func_15145974(struct17 *, f32 *, f32 *);
extern void func_1000FD38(void *, void *, s32);
extern s32 func_151C87E0(s32, s32);
extern void func_151C899C();
extern void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
extern void *func_1515FF74(void *, s32, u8, s32);
extern void func_15160274(s32, s32, s32);
extern void func_1515572C(s32, s32);
extern void func_151A561C(s32, s32);
extern void (*D_8008FC10[])(struct102 *, void **, u8);
typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0x160];
    f32 unk180;
} struct_func_151C9AC0_arg0;
typedef struct {
    u8 unk0[0x24];
} struct_func_151C9AC0_sp20;
typedef struct {
    u8 pad_0x00[0x28];
    s32 field_0x28;
    struct102 *field_0x2C;
    u8 pad_0x30[0x4];
    s32 field_0x34;
    s32 field_0x38;
    s32 field_0x3C;
    s32 field_0x40;
    s32 field_0x44;
    s32 field_0x48;
    s32 field_0x4C;
    s32 field_0x50;
    u8 pad_0x54[0x32];
    u8 field_0x86;
} Game1F4650LinkBlock;
typedef struct {
    u8 pad_0x00[0x13C];
    s32 field_0x13C;
    u8 pad_0x140[0x4];
    s32 field_0x144;
} Game1F4650LinkedObject;
typedef struct {
    u8 pad_0x00[0x170];
    s32 field_0x170;
} Game1F4650LinkOwner;
typedef struct {
    s32 field_0x0;
    s32 field_0x4;
} Game1F4650LinkMatchEvent;
typedef struct {
    s32 field_0x0;
    u8 field_0x4;
} Game1F4650LinkStateEvent;
typedef struct {
    s32 field_0x0;
    u8 pad_0x4[0x8];
    u8 field_0xC;
    u8 field_0xD;
    u8 field_0xE;
    u8 pad_0xF[0x1];
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    u8 pad_0x1C[0x4];
    f32 field_0x20;
} Game1F4650EffectState;
typedef struct {
    u8 pad_0x00[0x14];
    f32 field_0x14;
    u8 pad_0x18[0x58];
    Game1F4650EffectState field_0x70;
} Game1F4650EffectObject;
typedef struct {
    u8 field_0x0;
    u8 pad_0x1[0x3];
    s32 field_0x4;
} Game1F4650EffectTriggerEvent;
typedef struct {
    u8 pad_0x0[0x9];
    u8 field_0x9;
} Game1F4650ByteFlagTarget;
typedef struct {
    u8 pad_0x0[0x6F];
    u8 field_0x6F;
} Game1F4650StatusFlagBlock;
typedef struct {
    u8 pad_0x00[0x14];
    Game1F4650ByteFlagTarget *field_0x14;
    Game1F4650StatusFlagBlock *field_0x18;
} Game1F4650StatusFlagOwner;
typedef struct {
    u8 pad_0x00[0x58];
    s32 field_0x58;
    u8 pad_0x5C[0x12C];
    Game1F4650StatusFlagBlock *field_0x188;
} Game1F4650StatusFlagObject;
extern void func_1504715C(struct_func_151C9AC0_sp20 *, struct_func_151C9AC0_arg0 *);
extern s32 func_151ABE40(f32 *, struct_func_151C9AC0_sp20 *, s32, u8, s32);

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[2];
    s16 rot;
    u8 padA[2];
    f32 unkC;
    f32 unk10;
    u8 unk14;
    u8 pad15[3];
    u32 flags;
    u32 color;
    u32 unk20;
} Game1F4650Beam;
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, void *, void *, s32, f32);
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151C8FCC;
extern s32 func_151C756C(Vec3_151C8FCC *arg0, void *arg1, Vec3_151C8FCC *arg2, void *arg3, Game1F4650Beam *arg4, Game1F4650Beam *arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, s16 argA, s16 argB, s16 argC, u8 argD, u8 argE, u8 argF, u8 arg10, u8 arg11, u8 arg12, f32 arg13, f32 arg14, u8 arg15, u8 arg16, s32 arg17);

s32 func_151C71A0(Game1F4650Beam *arg0) {
    f32 z;
    f32 y;
    f32 x;
    f32 ez;
    f32 ey;
    f32 ex;
    f32 dz;
    f32 dy;
    f32 dx;
    f32 nz;
    f32 ny;
    f32 nx;
    f32 mz;
    f32 my;
    f32 mx;
    f32 mtx[4][4];
    s32 hit1;
    s32 hit2;
    u8 info1[0x14];
    u8 info2[0x14];
    f32 hitpos1[3];
    f32 hitpos2[3];
    s32 sB8;
    s32 sB4;
    s32 sB0;
    s32 sAC;
    s32 b27;
    s32 b29;
    s32 b30;
    s32 b31;

    if (arg0->unk14 == 1) {
        return 1;
    }
    x = arg0->x;
    y = arg0->y;
    z = arg0->z;
    func_150A8050(mtx, arg0->unkC, arg0->unk10, 0);
    func_150A7960(&mtx[0][0], 0.0f, arg0->rot, 0.0f, &ex, &ey, &ez);
    ex += x;
    ey += y;
    ez += z;
    dx = ex - x;
    dy = ey - y;
    dz = ez - z;
    mx = dx * 0.5f + x;
    my = dy * 0.5f + y;
    mz = dz * 0.5f + z;
    nx = -dx;
    ny = -dy;
    nz = -dz;
    hit1 = func_150AC9C0(mx, my, mz, nx, ny, nz, 0, info1, &hitpos1[0], &hitpos1[1], &hitpos1[2], &sB8, &sB0, 0, 0.0f);
    hit2 = func_150AC9C0(mx, my, mz, dx, dy, dz, 0, info2, &hitpos2[0], &hitpos2[1], &hitpos2[2], &sB4, &sAC, 0, 0.0f);
    if (hit1 == 0 || hit2 == 0) {
        return 1;
    }
    b30 = ((b27 = ((arg0->flags & 0x8000000) >> 27) == 0), (b29 = ((arg0->flags & 0x20000000) >> 29) == 0), ((arg0->flags & 0x40000000) >> 30)) ? 1 : 0;
    b31 = ((arg0->flags & 0x80000000) >> 31) == 0;
    func_151C756C(hitpos1, info1, hitpos2, info2, arg0, arg0,
                  arg0->color & 0xFF, (arg0->color & 0xFF00) >> 8, (arg0->color & 0xFF0000) >> 16,
                  (arg0->flags & 0x1000000) >> 24,
                  arg0->flags & 0xFF, (arg0->flags & 0xFF00) >> 8, (arg0->flags & 0xFF0000) >> 16,
                  (arg0->flags & 0x2000000) >> 25, (arg0->flags & 0x4000000) >> 26,
                  b27, b29, b30, (arg0->flags & 0x10000000) >> 28,
                  (f32)(arg0->unk20 & 0xFF) * 0.02463994361f,
                  (f32)((arg0->unk20 & 0xFF00) >> 8) * 0.003079992952f,
                  b31, 0xFF, 1);
    return 1;
}

typedef struct {
    s16 v[9];
} Info_151C756C;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    Game1F4650Beam *unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    struct102 *unk3C[3];
    struct102 *unk48[3];
    f32 unk54;
    u8 unk58;
    Info_151C756C unk5A;
    Info_151C756C unk6C;
    s16 unk7E;
    s16 unk80;
    s16 unk82;
    s16 unk84;
    u8 unk86;
    Game1F4650Beam *unk88;
    s32 unk8C;
    s32 unk90;
    u8 unk94;
    f32 unk98;
    f32 unk9C;
} Block_151C756C;

typedef struct {
    u8 unk00;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Vec3_151C8FCC unk1C;
    Vec3_151C8FCC unk28;
    Vec3_151C8FCC unk34;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[0xB];
} Cfg_151C756C;

extern void func_1510F800(s32);
extern s32 func_15145128(Vec3_151C8FCC *, Vec3_151C8FCC *, f32 *, f32 *);
extern s32 func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);
s32 func_151C8FCC(s32 arg0, Vec3_151C8FCC *arg1, Vec3_151C8FCC *arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, u8 arg8, s32 arg9);
s32 func_151C87AC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6);

s32 func_151C756C(Vec3_151C8FCC *arg0, void *arg1, Vec3_151C8FCC *arg2, void *arg3, Game1F4650Beam *arg4, Game1F4650Beam *arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, s16 argA, s16 argB, s16 argC, u8 argD, u8 argE, u8 argF, u8 arg10, u8 arg11, u8 arg12, f32 arg13, f32 arg14, u8 arg15, u8 arg16, s32 arg17) {
    s32 obj;
    Cfg_151C756C cfg;
    Block_151C756C blk;
    s32 pos[3];
    Vec3_151C8FCC ndir;

    func_1510F800(0);
    blk.unk8C = func_1510FD20(arg0->x, arg0->z);
    blk.unk90 = func_1510FD20(arg2->x, arg2->z);
    if (blk.unk8C == 0 || blk.unk90 == 0) {
        return 0;
    }
    blk.unk88 = arg4;
    blk.unk86 = (arg12 ? 8 : 0) | ((argF ? 4 : 0) | ((argE ? 0 : 2) | ((argD ? 1 : 0) | (arg15 ? 0x10 : 0))));
    if (!arg15) {
        arg4->unk14 = 1;
    }
    blk.unk98 = arg13;
    blk.unk9C = arg14;
    blk.unk94 = arg11;
    blk.unk5A = *(Info_151C756C *)arg1;
    blk.unk6C = *(Info_151C756C *)arg3;
    blk.unk7E = argA;
    blk.unk80 = argB;
    blk.unk82 = argC;
    blk.unk84 = argA + argB;
    blk.unk00 = blk.unk08 = func_150ADA68() * 1024.0f + -2560.0f + 2048.0f;
    blk.unk04 = blk.unk0C = func_150ADA68() * 1024.0f + 1536.0f + 2048.0f;
    blk.unk10 = func_150ADA68() * 768.0f + 768.0f;
    blk.unk14 = func_150ADA68() * 768.0f + 768.0f;
    blk.unk18 = func_150ADA68() * 6.283185482f;
    blk.unk1C = func_150ADA68() * 6.283185482f;
    blk.unk20 = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
    blk.unk24 = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
    {
        extern void func_151C9198(struct102 **, Vec3_151C8FCC *, void *, Vec3_151C8FCC *, s32, u8, u8, u8, s32);

        cfg.unk34.x = arg2->x - arg0->x;
        cfg.unk34.y = arg2->y - arg0->y;
        cfg.unk34.z = arg2->z - arg0->z;
        ndir.x = -cfg.unk34.x;
        ndir.y = -cfg.unk34.y;
        ndir.z = -cfg.unk34.z;
        func_151C9198(blk.unk3C, arg0, arg1, &ndir, blk.unk8C, arg6 < arg8, argF, arg16, 0);
        func_151C9198(blk.unk48, arg2, arg3, &cfg.unk34, blk.unk90, arg6 < arg8, argF, arg16, 0);
        if (blk.unk3C[0] == NULL || blk.unk3C[1] == NULL || blk.unk3C[2] == NULL ||
            blk.unk48[0] == NULL || blk.unk48[1] == NULL || blk.unk48[2] == NULL) {
            if (blk.unk3C[0] != NULL) {
                func_1516972C(blk.unk3C[0]);
            }
            if (blk.unk3C[1] != NULL) {
                func_1516972C(blk.unk3C[1]);
            }
            if (blk.unk3C[2] != NULL) {
                func_1516972C(blk.unk3C[2]);
            }
            if (blk.unk48[0] != NULL) {
                func_1516972C(blk.unk48[0]);
            }
            if (blk.unk48[1] != NULL) {
                func_1516972C(blk.unk48[1]);
            }
            if (blk.unk48[2] != NULL) {
                func_1516972C(blk.unk48[2]);
            }
            return 0;
        }
    }
    blk.unk58 = 0;
    blk.unk54 = 0.0f;
    blk.unk28 = arg5;
    if (arg9) {
        Header hdr;

        pos[0] = arg0->x + cfg.unk34.x * 0.5f;
        pos[1] = arg0->y + cfg.unk34.y * 0.5f;
        pos[2] = arg0->z + cfg.unk34.z * 0.5f;
        hdr.unk0 = 2;
        hdr.unk1 = 2;
        hdr.unk2 = 0x12C;
        hdr.unk4 = 0;
        blk.unk2C = func_1516284C(&hdr, pos, arg6, arg7, arg8, 0xFF, 0, 0, 0x10, arg16, arg17);
    } else {
        blk.unk2C = 0;
    }
    cfg.unk00 = 0x59;
    cfg.unk01 = 6;
    cfg.unk02 = 0x5A03;
    cfg.unk04 = 0x12C;
    cfg.unk08 = 0;
    cfg.unk0C = 0;
    cfg.unk10 = arg6;
    cfg.unk11 = arg7;
    cfg.unk12 = arg8;
    cfg.unk13 = 0xFF;
    cfg.unk14 = 0.0f;
    cfg.unk18 = 20.0f;
    cfg.unk1C = *arg0;
    cfg.unk28 = *arg2;
    cfg.unk40 = (D_800BE9F0 == 0x36 ? 0 : 0x02000000) | 0x6C0000 | 0x40000000;
    cfg.unk44 = 0xFF;
    cfg.unk45 = 0xFF;
    cfg.unk46 = 0;
    cfg.unk47 = 6;
    obj = func_1513D4B8((s32)&cfg, (s32)&D_800A4AA0, 0x1A, 0, 0, 0x18, 0, 0xA0, arg16, arg17);
    if (obj != 0) {
        Block_151C756C *bp;
        Vec3_151C8FCC dir;
        f32 len;
        f32 inv;
        Vec3_151C8FCC p0;
        Vec3_151C8FCC p1;
        Vec3_151C8FCC q0;
        Vec3_151C8FCC q1;

        {
            extern s32 D_800BE9F0;

            bp = (Block_151C756C *)(obj + 0x110);
            memcpy(bp, &blk, sizeof(Block_151C756C));
        }
        dir = cfg.unk34;
        if (func_15145128(&dir, &dir, &len, &inv) != 0 && arg10) {
            p0.x = arg0->x + dir.x * 20.0f;
            p0.y = arg0->y + dir.y * 20.0f;
            p0.z = arg0->z + dir.z * 20.0f;
            p1.x = p0.x + dir.x * 500.0f;
            p1.y = p0.y + dir.y * 500.0f;
            p1.z = p0.z + dir.z * 500.0f;
            q0.x = arg2->x - dir.x * 20.0f;
            q0.y = arg2->y - dir.y * 20.0f;
            q0.z = arg2->z - dir.z * 20.0f;
            q1.x = q0.x - dir.x * 500.0f;
            q1.y = q0.y - dir.y * 500.0f;
            q1.z = q0.z - dir.z * 500.0f;
            bp->unk34 = func_151C8FCC(obj, &p0, &p1, arg6, arg7, arg8, blk.unk8C, argF, arg16, arg17);
            bp->unk38 = func_151C8FCC(obj, &q0, &q1, arg6, arg7, arg8, blk.unk90, argF, arg16, arg17);
        } else {
            bp->unk34 = 0;
            bp->unk38 = 0;
        }
        if (bp->unk2C != 0) {
            func_1000FA64(0x4D0, pos[0], pos[1], pos[2], 0x1F40, 0xFA, 0x64, (s32)func_151C87AC, bp, 0, 8, (u8)func_150ADA20());
        }
    }
    return obj;
}

typedef struct {
    u8 pad0[2];
    s16 mask;
} Mask_151C7E98;

typedef struct {
    f32 tu;
    f32 tv;
    u8 pad8[0x7E];
    u8 flags;
    u8 pad87[5];
    Mask_151C7E98 *m1;
    Mask_151C7E98 *m2;
} Sub_151C7E98;

typedef struct {
    u8 pad0[0x30];
    f32 width;
    struct17 start;
    struct17 end;
    u8 pad4C[0x74];
    u8 tmpl[0x40];
    u8 *buffers[4];
    Sub_151C7E98 sub;
} Obj_151C7E98;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    s16 tc0;
    u8 padA[6];
} Vtx_151C7E98;

extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
void func_15145548(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 *arg4);

void *func_151C7E98(Obj_151C7E98 *arg0, s16 arg1) {
    Vtx_151C7E98 *vertices;
    void *result;
    struct17 *pos;
    struct17 dir;
    struct17 closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    f32 scale;
    u8 fresh;
    Sub_151C7E98 *sub;

    sub = &arg0->sub;
    if (!(*((u8 *)arg0 + 0x196) & 2)) {
        return NULL;
    }
    if (sub->flags & 4) {
        if (!(sub->m1->mask & (1 << arg1)) && !(sub->m2->mask & (1 << arg1))) {
            return NULL;
        }
    }
    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(arg0->buffers[arg1], arg0->tmpl, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->tmpl, 0x40);
        }
    } else {
        return NULL;
    }
    pos = func_15144B34(arg1);
    dir.unk0 = arg0->end.unk0 - arg0->start.unk0;
    dir.unk4 = arg0->end.unk4 - arg0->start.unk4;
    dir.unk8 = arg0->end.unk8 - arg0->start.unk8;
    func_15145548(&arg0->start, &dir, pos, &closest, 0);
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
    vertices->x = arg0->end.unk0 - nx;
    vertices->y = arg0->end.unk4 - ny;
    vertices->z = arg0->end.unk8 - nz;
    vertices->tc0 = sub->tu;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.unk0 - nx;
    vertices->y = arg0->start.unk4 - ny;
    vertices->z = arg0->start.unk8 - nz;
    vertices->tc0 = sub->tv;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.unk0 + nx;
    vertices->y = arg0->start.unk4 + ny;
    vertices->z = arg0->start.unk8 + nz;
    vertices->tc0 = sub->tv;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->end.unk0 + nx;
    vertices->y = arg0->end.unk4 + ny;
    vertices->z = arg0->end.unk8 + nz;
    vertices->tc0 = sub->tu;
    vertices->flag = 0;
    return result;
}

typedef struct SightOscillation {
    f32 x, y, centerX, centerY, radiusX, radiusY;
    f32 angleX, angleY, speedX, speedY;
    u8 pad28[4];
    void *animation;
    u8 pad30[0x10];
    void *objects[5];
    f32 phase;
    u8 alternate;
    u8 pad59[0x25];
    s16 activeTime;
    u8 pad80[2];
    s16 timer, period;
    u8 flags;
    u8 pad87;
    void *secondary;
    u8 pad8C[0xC];
    f32 brightnessAngle, brightnessSpeed;
} SightOscillation;

extern f32 D_800AAC50, D_800AAC54;

s32 func_151C82D0(void *arg0) {
    SightOscillation *state = (SightOscillation *)((u8 *)arg0 + 0x110);
    s16 period;
    void *object;
    void *object1;
    void *object2;
    void *object3;

    if (state->flags & 1) {
        period = state->period;
        state->timer += D_800BE9E4;
        while (state->timer >= period) {
            state->timer -= period;
        }
        if (state->timer < state->activeTime) {
            state->flags |= 2;
        } else {
            state->flags &= 0xFFFD;
        }
    }
    if (state->flags & 2) {
        state->angleX += state->speedX * D_800BE9A4;
        state->angleY += state->speedY * D_800BE9A4;
        state->angleX = func_15144B68(state->angleX);
        state->angleY = func_15144B68(state->angleY);
        state->x = func_150AD78C(state->angleX) * state->radiusX + state->centerX;
        state->y = func_150AD78C(state->angleY) * state->radiusY + state->centerY;
        object = state->animation;
        if (object != NULL) {
            *((u8 *)*(void **)((u8 *)object + 0x14) + 9) = 0;
        }
        object = state->secondary;
        if (object != NULL && (state->flags & 0x10)) {
            *((u8 *)object + 0x14) = 0;
        }
    } else {
        object = state->animation;
        if (object != NULL) {
            *((u8 *)*(void **)((u8 *)object + 0x14) + 9) = 1;
        }
        object = state->secondary;
        if (object != NULL) {
            *((u8 *)object + 0x14) = 1;
        }
    }
    state->phase += D_800AAC50 * D_800BE9A4;
    while (state->phase > 1.0f) {
        if (state->flags & 2) {
            state->alternate ^= 1;
        }
        state->phase -= 1.0f;
    }
    if (state->alternate != 0) {
        object = state->objects[0];
        *(u32 *)((u8 *)object + 0x60) &= 0xFFFDFFFF;
        object1 = state->objects[1];
        *(u32 *)((u8 *)object1 + 0x60) |= 0x20000;
        object2 = state->objects[3];
        *(u32 *)((u8 *)object2 + 0x60) &= 0xFFFDFFFF;
        object3 = state->objects[4];
        *(u32 *)((u8 *)object3 + 0x60) |= 0x20000;
    } else {
        object = state->objects[0];
        *(u32 *)((u8 *)object + 0x60) |= 0x20000;
        object1 = state->objects[1];
        *(u32 *)((u8 *)object1 + 0x60) &= 0xFFFDFFFF;
        object2 = state->objects[3];
        *(u32 *)((u8 *)object2 + 0x60) |= 0x20000;
        object3 = state->objects[4];
        *(u32 *)((u8 *)object3 + 0x60) &= 0xFFFDFFFF;
    }
    if (state->flags & 8) {
        *((u8 *)arg0 + 0x5C) = (u32)(func_150AD78C(state->brightnessAngle) * 112.5f + D_800AAC54);
        state->brightnessAngle = func_15144B68(state->brightnessAngle + state->brightnessSpeed * D_800BE9A4);
    }
    return 1;
}

void func_151C8674(s32 arg0, s32 arg1) {
    struct {
        s32 unk0;
        s32 unk4;
    } sp18;
    if (arg0 != 0) {
        sp18.unk0 = arg0;
        sp18.unk4 = arg1;
        func_151403A8((s32)&sp18, 0x20);
    }
}

void func_151C86AC(s32 arg0, s32 arg1, u8 arg2) {
    s32 *arg0p;
    Game1F4650LinkBlock *temp_v0;
    s32 temp_v1;

    arg0p = &arg0;
    switch (arg2) {
    case 0x20:
        temp_v0 = (Game1F4650LinkBlock *)(*arg0p + 0x110);
        if ((temp_v0->field_0x86 & 0x10) && (((Game1F4650LinkMatchEvent *)arg1)->field_0x4 == temp_v0->field_0x28)) {
            if (func_151C87E0(((Game1F4650LinkMatchEvent *)arg1)->field_0x0, *arg0p) != 0) {
                func_151C899C(((Game1F4650LinkMatchEvent *)arg1)->field_0x0, *arg0p);
            }
        }
        break;
    case 0x3A:
        temp_v0 = (Game1F4650LinkBlock *)*arg0p;
        temp_v0 = (Game1F4650LinkBlock *)((s32)temp_v0 + 0x110);
        if (((Game1F4650LinkStateEvent *)arg1)->field_0x0 == temp_v0->field_0x28) {
            temp_v1 = ((Game1F4650LinkStateEvent *)arg1)->field_0x4;
            switch (temp_v1) {
            case 0:
                temp_v0->field_0x86 |= 2;
                break;
            case 1:
                temp_v0->field_0x86 &= ~2;
                break;
            case 2:
                func_151C899C(0, *arg0p);
                break;
            }
        }
        break;
    }
}

s32 func_151C87AC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    void *temp = *(void **)((s32)arg0 + 0x18);
    if (!(*(u8 *)((s32)temp + 0x86) & 0x2)) {
        *arg6 = 0;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1F4650/func_151C87E0.s")

typedef struct { f32 x, y, z; } V3_899C;
typedef struct { u8 b[0x10]; u16 h; } B18_899C;
typedef struct {
    u8 pad0[0x5A];
    B18_899C c5A;
    B18_899C c6C;
    u8 pad7E[0x94 - 0x7E];
    u8 unk94;
} Sub110_899C;
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x34 - 0xD];
    V3_899C p34;
    V3_899C p40;
    V3_899C p4C;
    u8 pad58[0x110 - 0x58];
    Sub110_899C s110;
} Ent_899C;
typedef struct {
    V3_899C pos;
    B18_899C copy;
    f32 f20;
    s32 i24, i28;
    V3_899C v2C;
    f32 f38, f3C, f40, f44, f48, f4C;
    s32 i50, i54;
    f32 f58, f5C, f60, f64;
    s16 s68, s6A, s6C, s6E, s70, s72;
    s8 s74;
} Cfg_899C;
typedef struct { s8 a, b; s16 c; s8 d; } S60_899C;
typedef struct { s8 a; s16 b; s8 c, d, e; } S58_899C;
extern u8 *D_800AAC30[];
extern void func_1514FBFC(void *, u8, s32);
extern void func_151602C0(void *, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15164F0C(s32, u8, s32, u8, s32);
extern void func_151D8868(void *, s32, s32, s32);
extern void func_1505D024(s32, u8 *, s32, s32);
extern void func_1000EC24();

#define E arg1
#define PC sub
typedef struct { u8 pad[0x318]; u8 *unk318; } A0_899C;
void func_151C899C(A0_899C *arg0, Ent_899C *arg1) {
    Sub110_899C *sub;
    Cfg_899C cfg;
    s32 v68[3];
    S60_899C s60;
    S58_899C s58;

    sub = &E->s110;
    cfg.f20 = 491.0f;
    cfg.f38 = 0.06049999967f;
    cfg.f3C = 0.0522999987f;
    cfg.f40 = 30.0f;
    cfg.f44 = 83.0f;
    cfg.i24 = 5;
    cfg.i28 = 3;
    cfg.i50 = 5;
    cfg.i54 = 4;
    cfg.s68 = 0x12;
    cfg.s6A = 9;
    cfg.s6C = 0x64;
    cfg.s6E = 0x64;
    cfg.s70 = 0xC;
    cfg.s72 = 0xF;
    cfg.s74 = 0;
    cfg.f48 = 211.0f;
    cfg.f4C = 150.0f;
    cfg.f58 = 21.0f;
    cfg.f5C = 0.6980000138f;
    cfg.f60 = -0.4450000226f;
    cfg.f64 = 0.4410000145f;
    cfg.v2C = E->p34;
    cfg.pos = E->p4C;
    cfg.copy = sub->c5A;
    func_1514FBFC(&cfg, E->unkC, E->unk1);
    cfg.v2C = E->p40;
    cfg.pos.x = -E->p4C.x;
    cfg.pos.y = -E->p4C.y;
    cfg.pos.z = -E->p4C.z;
    cfg.copy = sub->c6C;
    func_1514FBFC(&cfg, E->unkC, E->unk1);
    v68[0] = E->p34.x;
    v68[1] = E->p34.y;
    v68[2] = E->p34.z;
    s60.a = 3;
    s60.b = -1;
    s60.c = func_150ADA20() % 9U + 0x11;
    s60.d = 0;
    func_151602C0(&s60, v68, func_150ADA20() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, E->unkC, E->unk1);
    v68[0] = E->p40.x;
    v68[1] = E->p40.y;
    v68[2] = E->p40.z;
    func_151602C0(&s60, v68, func_150ADA20() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, E->unkC, E->unk1);
    func_150E7FEC(func_150ADA68() * 101.0f + 55.0f, (u8)(func_150ADA20() % 66U + 0xBE), &PC->c5A, &E->p34, -1, 1, 1, 0, 0, 0, E->unkC, 0);
    func_150E7FEC(func_150ADA68() * 101.0f + 55.0f, (u8)(func_150ADA20() % 66U + 0xBE), &sub->c6C, &E->p40, -1, 1, 1, 0, 0, 0, E->unkC, 0);
    if (arg0 != 0) {
        func_15164F0C(2, arg0->unk318[0x23D], 0, E->unkC, E->unk1);
        s58.a = 1;
        s58.b = 0x28;
        s58.d = 1;
        s58.c = 8;
        s58.e = -1;
        func_151D8868(&s58, 0, 0xFF, 0);
    }
    if (arg0 != 0) {
        func_1505D024((s32)arg0, D_800AAC30[PC->unk94], 0, -1);
    }
    func_10010F88(0x2F5, 0x7FFF, func_150ADA20() % 500U, 0, 0, E->p34.x, E->p34.y, E->p34.z, 0x7D0, 0x898);
    func_1000FA64(0x2F6, E->p40.x, E->p40.y, E->p40.z, 0x7FFF, 0x898, 0x7D0, (s32)&func_1000EC24, (void *)5, 0, 0, func_150ADA20() % 500U);
    func_1516972C((struct102 *)arg1);
}


typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    s8 unk59;
    u8 pad5A[2];
} Struct90_151C8FCC;

typedef struct {
    u8 unk00;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Vec3_151C8FCC unk1C;
    Vec3_151C8FCC unk28;
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
    u8 pad4D[0xB];
} Struct38_151C8FCC;

void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);

s32 func_151C8FCC(s32 arg0, Vec3_151C8FCC *arg1, Vec3_151C8FCC *arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, u8 arg8, s32 arg9) {
    void *temp_v0;
    Struct90_151C8FCC sp90;
    Struct38_151C8FCC sp38;
    s32 sp34;

    sp38.unk1C = *arg1;
    sp38.unk28 = *arg2;
    sp34 = arg0;
    sp90.unk00 = sp90.unk04 = 30.10000038f;
    sp90.unk08 = sp90.unk0C = 40.0f;
    sp90.unk18 = 1.0f;
    sp90.unk10 = 0.0f;
    sp90.unk14 = 0.0f;
    sp38.unk00 = 0x15;
    sp38.unk02 = 0x3403;
    sp38.unk04 = 0x12C;
    sp38.unk08 = 0;
    sp38.unk0C = 0;
    sp38.unk10 = arg3;
    sp90.unk1C = 0.3000000119f;
    sp38.unk11 = arg4;
    sp38.unk12 = arg5;
    sp38.unk13 = 0xFF;
    sp38.unk14 = 0.0f;
    sp38.unk18 = 0.0f;
    sp38.unk34 = 1.0f;
    sp38.unk38 = 1.0f;
    sp38.unk3C = 1.0f;
    sp38.unk40 = (arg7 ? 0x01000000 : 0) | 0x024D2006 | 0x40000000;
    sp38.unk45 = 0xFF;
    sp38.unk44 = 0xFF;
    sp38.unk46 = 0;
    sp38.unk47 = 6;
    sp38.unk01 = 3;
    sp90.unk54 = 0;
    sp90.unk55 = 0;
    sp90.unk56 = 0;
    sp90.unk57 = 0;
    sp90.unk48 = 1.0f;
    sp90.unk58 = 0;
    sp38.unk48 = arg6;
    sp90.unk20 = 9.999999975e-07f;
    temp_v0 = func_151407D0(&sp90, 0x64, &sp38, 0, 0, 0, 0, 1, arg8, arg9);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x170, &sp34, 4);
    }
    return (s32)temp_v0;
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8  unk60;
    /* 0x61 */ u8  unk61;
    /* 0x62 */ u8  unk62;
    /* 0x63 */ u8  unk63;
    /* 0x64 */ u8  unk64;
    /* 0x65 */ u8  unk65;
    /* 0x66 */ u8  unk66;
    /* 0x67 */ u8  unk67;
    /* 0x68 */ u8  unk68;
    /* 0x69 */ s8  unk69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8  pad76[2];
} S151C9198;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8  unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8  unkE;
    /* 0x10 */ s32 unk10;
} S151C9198_b;

extern s32 func_15144E80(void *, struct17 *, struct17 *, struct17 *);
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_15133760(void *, void *);

void func_151C9198(struct102 **arg0, Vec3_151C8FCC *arg1, void *arg2, Vec3_151C8FCC *arg3, s32 arg4, u8 arg5, u8 arg6, u8 arg7, s32 arg8) {
    struct17 spEC;
    struct17 spE0;
    struct17 spD4;
    s32 i;
    S151C9198 desc;
    void *ret;
    S151C9198_b extra;

    if (func_15144E80(arg2, &spEC, &spE0, &spD4) != 0) {
        func_15145974(&spD4, &desc.unk14, &desc.unk10);
        desc.unk0 = 1.0f;
        desc.unk4 = 1.0f;
        desc.unk1C = 1.0f;
        desc.unk20 = 1.0f;
        desc.unk10 += 90.0f;
        desc.unk24 = 1.0f;
        desc.unkC = 0.4000000059604645f;
        desc.unk8 = 0.4000000059604645f;
        desc.unk18 = 0.0f;
        desc.unk28 = *(struct17 *)arg1;
        desc.unk34.unk0 = 0.0f;
        desc.unk34.unk4 = 0.0f;
        desc.unk34.unk8 = 0.0f;
        desc.unk40.unk0 = 0.0f;
        desc.unk40.unk4 = 0.0f;
        desc.unk40.unk8 = 0.0f;
        desc.unk4C = 0.0f;
        desc.unk50 = (arg6 ? 0x400 : 0) | 0x100 | 0x800 | 0x10000 | 0x4000 | 0x40000 | 0x80000;
        desc.unk54 = 0x12C;
        desc.unk56 = 0x36;
        desc.unk58 = 0;
        desc.unk5C = arg4;
        desc.unk60 = 0xFF;
        desc.unk61 = 0;
        desc.unk62 = 0;
        desc.unk63 = 0;
        desc.unk64 = 0;
        desc.unk65 = 0;
        desc.unk66 = 0;
        desc.unk67 = 0;
        desc.unk68 = 2;
        desc.unk6A = 0;
        desc.unk6C = 0;
        desc.unk70 = 0;
        desc.unk72 = 1;
        desc.unk74 = 0xFF;
        desc.unk69 = -1;
        arg0[0] = func_15132A4C(&desc, 3, 0xFF, 0, arg7, arg8);
        if (arg0[0] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760((u8 *)arg0[0] + 0x90 + i * 0x40, arg0[0]);
            }
        }
        desc.unk69 = 2;
        extra.unk0 = 2;
        extra.unk6 = 4;
        extra.unkE = 6;
        extra.unk8 = 0x3E;
        extra.unk10 = 0x3E;
        if (arg5) {
            extra.unk4 = 0x87;
            extra.unkC = 0x88;
        } else {
            extra.unk4 = 0x8B;
            extra.unkC = 0x8C;
        }
        desc.unk56 = 0x37;
        desc.unk68 = 0;
        arg0[1] = func_15132A4C(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (arg0[1] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760((u8 *)arg0[1] + 0x90 + i * 0x40, arg0[1]);
            }
            memcpy((u8 *)arg0[1] + 0x170, &extra, 0x14);
        }
        desc.unk56 = 0x38;
        if (arg5) {
            extra.unk4 = 0x89;
            extra.unkC = 0x8A;
        } else {
            extra.unk4 = 0x8D;
            extra.unkC = 0x8E;
        }
        arg0[2] = func_15132A4C(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (arg0[2] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760((u8 *)arg0[2] + 0x90 + i * 0x40, arg0[2]);
            }
            memcpy((u8 *)arg0[2] + 0x170, &extra, 0x14);
        }
    }
}

void func_151C94D4(Game1F4650LinkedObject *arg0) {
    register Game1F4650LinkBlock *temp_s0;
    s32 temp_a0;

    temp_s0 = (Game1F4650LinkBlock *)((s32)arg0 + 0x110);
    if (arg0->field_0x13C != 0) {
        func_1516972C(temp_s0->field_0x2C);
        func_1000FD38(func_151C87AC, (void *)temp_s0, 0);
    }

    temp_s0 = (Game1F4650LinkBlock *)*(s32 *)&arg0;
    temp_a0 = ((Game1F4650LinkedObject *)temp_s0)->field_0x144;
    temp_s0 = (Game1F4650LinkBlock *)((s32)temp_s0 + 0x110);
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x38;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x3C;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x40;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x44;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x48;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x4C;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x50;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    func_1513CA6C(arg0);
}

void func_151C95D8(Game1F4650LinkedObject *arg0) {
    register Game1F4650LinkBlock *temp_s0;
    s32 temp_a0;

    temp_s0 = (Game1F4650LinkBlock *)((s32)arg0 + 0x110);
    if (arg0->field_0x13C != 0) {
        func_1516972C(temp_s0->field_0x2C);
        func_1000FD38(func_151C87AC, (void *)temp_s0, 0);
    }

    temp_s0 = (Game1F4650LinkBlock *)*(s32 *)&arg0;
    temp_a0 = ((Game1F4650LinkedObject *)temp_s0)->field_0x144;
    temp_s0 = (Game1F4650LinkBlock *)((s32)temp_s0 + 0x110);
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x38;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x3C;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x40;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x44;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x48;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x4C;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    temp_a0 = temp_s0->field_0x50;
    if (temp_a0 != 0) {
        func_1516972C((struct102 *)temp_a0);
    }

    func_1513CAA0(arg0);
}

s32 func_151C96DC(Game1F4650LinkOwner *arg0, s32 arg1) {
    Game1F4650LinkBlock *temp;

    temp = (Game1F4650LinkBlock *)(arg0->field_0x170 + 0x110);
    if (temp->field_0x86 & 0x2) {
        return 1;
    }
    return 0;
}

void func_151C970C(s32 arg0, s32 arg1) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;
    sp18.unk0 = arg1;
    sp18.unk4 = arg0;
    func_151403A8((s32)&sp18, 0x3A);
}

extern s32 func_15046C80(f32 *, u16, f32, void *);
extern void func_150E83AC(void *, s16, u8, s32);
extern void func_15136C3C(void *, u8, u8, u8, u8, u8, u8, s32);
extern void *func_151541B8(void *, f32, f32, f32, f32, u8, s32);
extern void func_151D5334(void *, f32, f32, f32, u8, u8, s32);
extern void func_151D5514(void *, u8, s32);
extern void func_151D3FF4(f32 *, u8, s32);
void func_151C9AC0(struct_func_151C9AC0_arg0 *arg0, u8 arg1, s32 arg2);

void func_151C9740(struct_func_151C9AC0_arg0 *arg0, u8 arg1, s32 arg2) {
    struct17 origin;
    struct17 impact;
    u8 hit;
    u8 airborne;
    struct17 query;
    union {
        struct_func_151C9AC0_sp20 transform;
        f32 values[9];
    } collision;

    if (arg0 == NULL) {
        return;
    }
    origin.unk0 = arg0->unk14;
    origin.unk4 = *(f32 *)((u8 *)arg0 + 0x18) + 135.0f;
    origin.unk8 = arg0->unk1C;
    hit = 0;
    airborne = *(f32 *)((u8 *)arg0 + 0x2D8) != 0.0f;
    if (!airborne) {
        query.unk0 = origin.unk0;
        query.unk4 = origin.unk4;
        query.unk8 = origin.unk8;
        func_1504715C(&collision.transform, arg0);
        if (func_15046C80(&query.unk0, 0, origin.unk4 - 200.0f, &collision.transform) != 0 &&
            (collision.transform.unk0[0x1C] & 1)) {
            impact.unk0 = query.unk0;
            hit = 1;
            impact.unk4 = collision.values[0];
            impact.unk8 = query.unk8;
            {
            func_150E7FEC(func_150ADA68() * 125.0f + 204.0f,
                (func_150ADA20() % 101U + 0x9B) & 0xFF,
                &collision.values[1], &impact,
                func_150ADA20() % 302U + 0x1F4, 0, 1, 0, 0, 0, arg1, 0);
            }
        }
    }
    func_151D5404(&origin, 506.0f, 1013.0f, 0.000987166888f, 0xF, 0x14, arg1, arg2);
    func_151D5334(&origin, 506.0f, 1013.0f, 0.000987166888f, 5, arg1, arg2);
    func_151D5514(&origin, arg1, arg2);
    func_151D3FF4(&origin.unk0, arg1, arg2);
    func_150E83AC(hit ? &impact.unk0 : &origin.unk0, (s16)(func_150ADA20() % 62U + 0x78), arg1, arg2);
    func_151541B8(&origin, 2.0f * func_150ADA68() + 8.0f, 2.4641099f,
        (f32)(func_150ADA20() % 56U + 0xC8), 0.0f, arg1, arg2);
    if (!airborne) {
        func_15136C3C(arg0, 0, 0, 1, 0, 0, arg1, arg2);
    }
    func_10010F88(0x60D, 0x7D00, 0, 0, 0, (s32)origin.unk0,
        (s32)origin.unk4, (s32)origin.unk8, 0x1F4, 0x3E8);
    if (airborne) {
        func_151C9AC0(arg0, arg1, arg2);
    }
}

void func_151C9AC0(struct_func_151C9AC0_arg0 *arg0, u8 arg1, s32 arg2) {
    f32 sp44[3];
    struct_func_151C9AC0_sp20 sp20;

    sp44[0] = arg0->unk14;
    sp44[1] = arg0->unk180 + 2.0f;
    sp44[2] = arg0->unk1C;
    func_1504715C(&sp20, arg0);
    func_151ABE40(sp44, &sp20, 2, arg1, arg2);
}

s32 func_151C9B30(Game1F4650StatusFlagOwner *arg0) {
    Game1F4650StatusFlagBlock *temp = arg0->field_0x18;
    if (temp->field_0x6F == 0) {
        arg0->field_0x14->field_0x9 = 0;
    } else {
        arg0->field_0x14->field_0x9 = 1;
    }
    return 1;
}

s32 func_151C9B64(Game1F4650StatusFlagObject *arg0, u8 *arg1) {
    Game1F4650StatusFlagBlock *temp = arg0->field_0x188;
    if (temp->field_0x6F == 0) {
        *arg1 = 1;
    } else {
        arg0->field_0x58 &= ~0x2;
        *arg1 = 0;
    }
    return 1;
}

extern u8 D_800CBFA8[][0x32C];
void func_151C9ED4(s32 arg0);
void func_151C9F38(void *, u8, u8, s8, u8);
void func_151CA6A0(void *, u8);
void func_151CAD28(void *, u8);
void func_151CAB78(void *arg0, u8 arg1);
void func_151CCE94(void *arg0);
void func_151C9DE8(void *arg0, u8 arg1, s8 arg2, u8 arg3, s32 arg4);
void func_151CC524(void *, u8, u8, u8, u8, u8);
void func_151CC840(void *, u8, u8, u8, u8, u8);
void func_151CCD1C(void *arg0, u8 arg1, u8 arg2, u8 arg3);
void func_151CB5FC(void *, u8, u8, s8, u8);

void func_151C9BA0(void *arg0, s32 arg1) {
    u8 idx;

    func_151C9ED4((s32)arg0);
    idx = *(u8 *)(*(s32 *)((u8 *)arg0 + 0x3D0) + 0x65);
    if (idx != 0) {
        if (D_800CBFA8[idx][0] == 0x23 || D_800CBFA8[idx][0] == 0x8A) {
            arg1 = 0x1B;
        }
    }
    switch (arg1) {
        case 35:
            func_151C9F38(arg0, 1, 1, -1, 0xFF);
            func_151CA6A0(arg0, 0);
            break;
        case 31:
        case 41:
            func_151C9F38(arg0, 2, 1, -1, 0xFF);
            func_151CAD28(arg0, 0);
            break;
        case 12:
        case 22:
            func_151CAD28(arg0, 0);
            func_151CCE94(arg0);
            break;
        case 27:
            if (D_800BE9F0 == 0x27) {
                func_151C9DE8(arg0, 1, -1, 0xFF, 1);
            } else {
                if (D_80082FA0 == 0) {
                    func_151CC524(arg0, 0xFF, 0, 0, 1, 0xFF);
                    func_151CC840(arg0, 0xFF, 0, 0, 1, 0xFF);
                } else {
                    func_151CCD1C(arg0, 0xFF, 0, 0);
                }
                func_151C9F38(arg0, 3, 1, -1, 0xFF);
                func_151CB5FC(arg0, 1, 1, -1, 0xFF);
            }
            break;
        case 36:
            func_151C9F38(arg0, 0, 1, -1, 0xFF);
            func_151CA6A0(arg0, 0);
            break;
        case 18:
        case 24:
        case 56:
        case 59:
            break;
        case 9:
        default:
            func_151CAB78(arg0, 0);
            break;
    }
}

void func_150BDE90(void *, u8, s32);
void func_151C9F38(void *, u8, u8, s8, u8);
void func_151CC524(void *, u8, u8, u8, u8, u8);
void func_151CC840(void *, u8, u8, u8, u8, u8);
void func_151CCF08(void *, u8, s8, u8);
void func_151CB5FC(void *, u8, u8, s8, u8);

void func_151C9DE8(void *arg0, u8 arg1, s8 arg2, u8 arg3, s32 arg4) {
    func_151C9F38(arg0, 4, arg1, arg2, arg3);
    func_151CC524(arg0, 0, 0x32, 0xFF, arg1, arg3);
    func_151CC840(arg0, 0, 0x32, 0xFF, arg1, arg3);
    func_151CCF08(arg0, arg1, arg2, arg3);
    func_150BDE90(arg0, arg3, arg4);
    func_151CB5FC(arg0, 2, arg1, arg2, arg3);
    if (D_800BE9F0 == 0x27) {
        D_8008CD00 = 1;
    }
}

void func_151C9ED4(s32 arg0) {
    s32 sp24;

    sp24 = arg0;
    func_15160274((s32)&sp24, 0x21, arg0);
    func_1515572C((s32)&sp24, 0x21);
    func_151A561C((s32)&sp24, 0x21);
    func_151494E0((s32)&sp24, 0x21);
    D_8008CD00 = 0;
}

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} Col151C9F38;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} Emitter151C9F38;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[0xA];
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ f32 unk28;
    /* 0x2C */ Col151C9F38 unk2C;
    /* 0x3C */ Col151C9F38 unk3C;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ s8 unk54;
    /* 0x55 */ u8 pad55[3];
} Payload151C9F38;

extern Col151C9F38 D_800AAC80;
extern Col151C9F38 D_800AAC90[];
extern Col151C9F38 D_800AACE0[];
extern f32 D_800AAD30[];
extern f32 D_800AAD44[];
extern f32 D_800AAD58[];
extern f32 D_800AAD6C[][2];
extern f32 D_800AAD94[][2];

void func_151C9F38(void *arg0, u8 arg1, u8 arg2, s8 arg3, u8 arg4) {
    Emitter151C9F38 spA0;
    void *temp;
    Payload151C9F38 sp44;

    sp44.unk2C = D_800AAC80;
    sp44.unk3C = D_800AAC90[arg1];
    sp44.unk4C = D_800AAD44[arg1] - 1.0f;
    sp44.unk4 = 0.0f;
    sp44.unkD = 0;
    sp44.unk8 = 130.0f;
    sp44.unk0 = arg0;
    sp44.unkC = (s32)((arg3 == -1) ? 0 : 4);
    sp44.unk54 = arg3;
    spA0.unk12 = 0x12C;
    spA0.unk16 = 1;
    spA0.unk18 = 0xFF;
    spA0.unk1A = 7;
    spA0.unk1F = 0xFF;
    spA0.unk20 = 0xFF;
    spA0.unk21 = 0xFF;
    spA0.unk22 = 0xFF;
    spA0.unk23 = 0xFF;
    spA0.unk24 = 0;
    spA0.unk28 = 0x200004;
    spA0.unk2C = 0x1F0601;
    spA0.unk38 = 0x80;
    spA0.unk3C = 0x20;
    spA0.unk40 = 0;
    spA0.unk41 = 0xA;
    spA0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA0.unk0 = D_800AAD58[arg1];
    spA0.unk4 = 0.0f;
    spA0.unk8 = D_800AAD30[arg1];
    spA0.unkC = D_800AAD44[arg1];
    spA0.unk10 = 0xFF;
    spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg2 ? 0x40 : 0) | 0x10);
    spA0.unk30 = 8;
    spA0.unk34 = 0x54;
    sp44.unk1C = 0.0f;
    sp44.unk28 = 99999.0f;
    sp44.unk18 = D_800AAD6C[arg1][D_80082FA0 == 1];
    sp44.unk20 = D_800AAD94[arg1][D_80082FA0 == 1];
    sp44.unkD = 3;

    spA0.unk1B = D_800AAC90[arg1].r;
    spA0.unk1C = D_800AAC90[arg1].g;
    spA0.unk1D = D_800AAC90[arg1].b;
    spA0.unk1E = D_800AAC90[arg1].a;
    temp = func_1515548C(&spA0, 8, 0, 0, 0x58, arg4, 1);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp44, 0x58);
    }

    sp44.unkD = 0;
    spA0.unk8 = D_800AAD30[arg1] + 2.0f;
    spA0.unkC = D_800AAD44[arg1] + 2.0f;
    spA0.unk1B = D_800AACE0[arg1].r;
    spA0.unk1C = D_800AACE0[arg1].g;
    spA0.unk1D = D_800AACE0[arg1].b;
    spA0.unk1E = D_800AACE0[arg1].a;
    temp = func_1515548C(&spA0, 0, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp44, 0x58);
    }
}

void func_151CA6A0(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CA6A0_spA0;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0xA];
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        u8 pad2C[0x2C];
    } struct_151CA6A0_sp44;
    struct_151CA6A0_spA0 spA0;
    void *temp_v0;
    struct_151CA6A0_sp44 sp44;
    s32 pad1;
    s32 pad2;

    sp44.unkD = 0;
    sp44.unkC = 0;
    spA0.unkC = 25.0f;
    spA0.unk8 = 25.0f;
    sp44.unk0 = arg0;
    sp44.unk4 = 0.0f;
    sp44.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        spA0.unk10 = 0x73;
    } else {
        spA0.unk10 = 0x5D;
    }
    spA0.unk12 = 0x12C;
    spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    spA0.unk16 = 1;
    spA0.unk18 = 0xFF;
    spA0.unk1A = 7;
    spA0.unk1B = 0;
    spA0.unk1C = 0xFF;
    spA0.unk1D = 0;
    spA0.unk1E = 0x82;
    spA0.unk1F = 0xFF;
    spA0.unk20 = 0xFF;
    spA0.unk21 = 0xFF;
    spA0.unk22 = 0xFF;
    spA0.unk23 = 0xFF;
    spA0.unk24 = 0;
    spA0.unk28 = 0x200004;
    spA0.unk2C = 0x1F0601;
    spA0.unk30 = 8;
    spA0.unk34 = 0x44;
    spA0.unk38 = 0x80;
    spA0.unk3C = 0x20;
    spA0.unk40 = 0;
    spA0.unk41 = 0xA;
    spA0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA0.unk14 &= ~6;
    spA0.unk14 |= 6;
    spA0.unk0 = 25.0f;
    spA0.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 4;
    spA0.unk0 = -25.0f;
    spA0.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 0;
    spA0.unk0 = -25.0f;
    spA0.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 2;
    spA0.unk0 = 25.0f;
    spA0.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    {
        s32 i;

        spA0.unk0 = 0.0f;
        spA0.unk4 = 0.0f;
        spA0.unkC = 3.0f;
        spA0.unk8 = 3.0f;
        spA0.unk10 = 0x99;
        spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
        spA0.unk30 = 8;
        spA0.unk34 = 0x44;
        sp44.unk28 = 99999.0f;
        if (D_80082FA0 == 1) {
            sp44.unk18 = -62.0f;
        } else {
            sp44.unk18 = -50.0f;
        }
        sp44.unk1C = 0.0f;
        if (D_80082FA0 == 1) {
            sp44.unk20 = 0.01612903178f;
        } else {
            sp44.unk20 = 0.01999999955f;
        }
        i = 0;
        sp44.unk24 = 0.0f;
        for (; i < 12; i++) {
            temp_v0 = func_1515548C(&spA0, 7, 0, 0, 0x58, 0xFF, 1);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
            }
            sp44.unk24 += 0.5235987902f;
        }
    }
}

void func_151CAACC(struct102 *arg0, void **arg1, u8 arg2) {
    void (*fn)(struct102 *, void **, u8);
    void *temp;

    if (arg2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x70) == *(s32 *)arg1) {
            func_1516972C(arg0);
        label_151CAACC:
            ;
        }
    } else if (arg2 == 0) {
        temp = *arg1;
        if (*(s32 *)((s32)arg0 + 0x70) == *(s32 *)((s32)temp + 0x318)) {
            func_1516972C(arg0);
        }
    }
    fn = D_8008FC10[*(u8 *)((s32)arg0 + 0x7D)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

void func_151CAB78(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        s16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CAB78_sp90;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4E];
    } struct_151CAB78_sp34;
    struct_151CAB78_sp90 sp90;
    struct_151CAB78_sp34 sp34;
    void *temp_v0;
    s32 temp_type;
    f32 temp_f0;

    sp34.unk0 = arg0;
    sp34.unk4 = 0.0f;
    sp34.unkD = 0;
    sp34.unkC = 0;
    sp34.unk8 = 130.0f;

    if (D_80082FA0 > 0) {
        temp_f0 = 2.0f;
    } else {
        temp_f0 = 1.0f;
    }

    sp90.unk10 = 0x64;
    sp90.unk12 = 0x12C;
    sp90.unkC = temp_f0 * 12.0f;
    sp90.unk8 = sp90.unkC;
    sp90.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    sp90.unk16 = 1;
    sp90.unk18 = 0xFF;
    sp90.unk1A = 7;
    sp90.unk1B = 0xFF;
    sp90.unk1C = 0xC8;
    sp90.unk1D = 0;
    sp90.unk1E = 0x82;
    sp90.unk1F = 0xFF;
    sp90.unk20 = 0xFF;
    sp90.unk21 = 0xFF;
    sp90.unk22 = 0xFF;
    sp90.unk23 = 0xFF;
    sp90.unk24 = 0;
    sp90.unk28 = 0x200004;
    sp90.unk2C = 0x1F0601;
    sp90.unk30 = 8;
    sp90.unk34 = 0x44;
    sp90.unk38 = 0x80;
    sp90.unk3C = 0x20;
    sp90.unk40 = 0;
    sp90.unk41 = 0xA;
    sp90.unk0 = 0.0f;
    sp90.unk4 = 0.0f;
    sp90.unk44 = *(u8 *)((s32)arg0 + 0x23D);

    if (arg1 != 0) {
        temp_type = 4;
    } else {
        temp_type = 0;
    }

    temp_v0 = func_1515548C(&sp90, temp_type, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp34, 0x58);
    }
}

void func_151CAD28(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CAD28_spA8;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CAD28_sp4C;
    typedef struct {
        s8 unk0;
        s8 unk1;
        s8 unk2;
        s16 unk4;
        s8 unk6;
    } struct_151CAD28_sp44;
    typedef struct {
        void *unk0;
        void *unk4;
        void *unk8;
        s32 unkC;
        u32 unk10;
    } struct_151CAD28_sp30;
    struct_151CAD28_spA8 spA8;
    void *temp_v0;
    struct_151CAD28_sp4C sp4C;

    sp4C.unkD = 0;
    sp4C.unkC = 0;
    spA8.unkC = 25.0f;
    spA8.unk8 = 25.0f;
    sp4C.unk0 = arg0;
    sp4C.unk4 = 0.0f;
    sp4C.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        spA8.unk10 = 0x72;
    } else {
        spA8.unk10 = 0x65;
    }
    spA8.unk12 = 0x12C;
    spA8.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    spA8.unk16 = 1;
    spA8.unk18 = 0xFF;
    spA8.unk1A = 7;
    spA8.unk1B = 0;
    spA8.unk1C = 0xFF;
    spA8.unk1D = 0;
    spA8.unk1E = 0x82;
    spA8.unk1F = 0xFF;
    spA8.unk20 = 0xFF;
    spA8.unk21 = 0xFF;
    spA8.unk22 = 0xFF;
    spA8.unk23 = 0xFF;
    spA8.unk24 = 0;
    spA8.unk28 = 0x200004;
    spA8.unk2C = 0x1F0601;
    spA8.unk30 = 8;
    spA8.unk34 = 0x44;
    spA8.unk38 = 0x80;
    spA8.unk3C = 0x20;
    spA8.unk40 = 0;
    spA8.unk41 = 0xA;
    spA8.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA8.unk14 &= ~6;
    spA8.unk14 |= 6;
    spA8.unk0 = 25.0f;
    spA8.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 4;
    spA8.unk0 = -25.0f;
    spA8.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 0;
    spA8.unk0 = -25.0f;
    spA8.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 2;
    spA8.unk0 = 25.0f;
    spA8.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    if (D_800BE616 == 0) {
    struct_151CAD28_sp44 sp44;
    struct_151CAD28_sp30 sp30;

        sp30.unk0 = arg0;
        sp30.unk4 = (void *)(*(u8 *)((s32)arg0 + 0x23D) * 0x180 + D_800BE628);
        sp30.unkC = *(f32 *)((s32)sp30.unk4 + 0x34) - 12.0f;
        sp30.unk10 = (u32)(s32)(*(f32 *)((s32)sp30.unk4 + 0x38) - 12.0f) * D_800BE620;
        sp44.unk0 = 0xE;
        sp44.unk1 = -1;
        sp44.unk2 = 1;
        sp44.unk4 = 0x12C;
        sp44.unk6 = 1;
        temp_v0 = func_1515FF74(&sp44, 0x498, 0xFF, 1);
        if (temp_v0 != NULL) {
            sp30.unk8 = (u8 *)temp_v0 + 0x30;
            memcpy((u8 *)temp_v0 + 0x18, &sp30, 0x14);
        }
    }
}

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
} struct_func_151CB110_s2;

typedef struct {
    s32 unk0;
    struct_func_151CB110_s2 *unk4;
    u16 *unk8;
    s32 unkC;
    s32 unk10;
} struct_func_151CB110_s0;

typedef struct {
    u8 pad0[0x18];
    struct_func_151CB110_s0 sub;
} struct_func_151CB110_arg0;

extern u8 D_800BEAC2;

void func_151CB110(struct_func_151CB110_arg0 *arg0, u16 *arg1) {
    struct_func_151CB110_s2 *s2;
    s32 i;
    s32 row;
    s32 src;
    f32 x;
    f32 y;
    f32 w;
    f32 invw;
    f32 k;
    s32 dst[2][2];
    s32 pad;
    s32 srcp[2][2];

    if (D_800BEAC0 != 0 || D_800BEAC1 != 0) {
        return;
    }
    if (D_800BEAC2 != 0) {
        return;
    }
    i = 0;
    s2 = arg0->sub.unk4;
    {
    struct_func_151CB110_s0 *s0 = &arg0->sub;
    src = arg0->sub.unkC + arg0->sub.unk10;
    row = 0;
    do {
        memcpy(s0->unk8 + row, (u8 *)arg1 + src * 2, 0x30);
        i++;
        row += 0x18;
        src += D_800BE620;
    } while (i != 0x18);

    {
    s32 q;
    for (y = -1.0f; y < 0.0f; y += 0.08333333582f) {
        w = func_15145A0C(fabsf(y), 1.0f, 1.0f);
        if (w) {
        dst[0][1] = (s32)(s2->unk38 + y * 12.0f) * D_800BE620;
        dst[1][1] = (s32)(s2->unk38 - y * 12.0f) * D_800BE620;
        invw = 1.0f / w;
        k = 1.0f - (1.0f - w * 0.6693121791f);
        x = -w;
        while (x < 0.0f) {
            f32 t;
            t = func_15145A0C(fabsf(x), w, invw);
            dst[0][0] = (s32)(s2->unk34 + x * 12.0f);
            dst[1][0] = (s32)(s2->unk34 - x * 12.0f);
            t = 1.0f - t * k;
            srcp[0][0] = (s32)(x * t * 12.0f + 12.0f);
            srcp[0][1] = (s32)(y * t * 12.0f + 12.0f) * 0x18;
            srcp[1][0] = (s32)(12.0f - x * t * 12.0f);
            srcp[1][1] = (s32)(12.0f - y * t * 12.0f) * 0x18;
            *(arg1 + dst[0][1] + dst[0][0]) = *(s0->unk8 + srcp[0][0] + srcp[0][1]);
            *(arg1 + dst[0][1] + dst[1][0]) = *(s0->unk8 + srcp[1][0] + srcp[0][1]);
            *(arg1 + dst[1][1] + dst[1][0]) = *(s0->unk8 + srcp[1][0] + srcp[1][1]);
            *(arg1 + dst[1][1] + dst[0][0]) = *(s0->unk8 + srcp[0][0] + srcp[1][1]);
            x += 0.08333333582f;
        }
        }
    }
    }
    }
}

void func_151CB49C(struct102 *arg0, void **arg1, u8 arg2) {
    void *temp_v0;
    s32 temp_a2;

    temp_a2 = arg2;
    if (temp_a2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x18) == *(s32 *)arg1) {
            func_1516972C(arg0);
        }
    } else if (temp_a2 == 0) {
        temp_v0 = *arg1;
        if (*(s32 *)((s32)arg0 + 0x18) == *(s32 *)((s32)temp_v0 + 0x318)) {
            func_1516972C(arg0);
        }
    }
}

void func_151CB510(void *arg0) {
    void *temp_v0;
    s32 base;
    f32 target;

    temp_v0 = *(void **)((s32)arg0 + 0x70);
    if ((*(u16 *)*(s32 *)((s32)temp_v0 + 0x36C) & 0x2000) && (base = (s32)arg0 + 0x70, *(u16 *)(*(s32 *)((s32)temp_v0 + 0x3D4) + 0x19E) == 0)) {
        target = *(f32 *)(base + 8);
    } else {
        target = 0.0f;
        base = (s32)arg0 + 0x70;
    }
    *(f32 *)(base + 4) += (target - *(f32 *)(base + 4)) * D_800AAEA8;
    *(u8 *)((s32)arg0 + 0x2E) = (u32)*(f32 *)(base + 4);
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} Emitter151CB5FC;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 padF;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x30];
    /* 0x54 */ s8 unk54;
    /* 0x55 */ u8 pad55[3];
} Payload151CB5FC;

extern f32 D_800AAD84[];
extern f32 D_800AADAC[];

void func_151CB5FC(void *arg0, u8 arg1, u8 arg2, s8 arg3, u8 arg4) {
    Emitter151CB5FC sp90;
    void *temp;
    Payload151CB5FC sp34;

    sp34.unk4 = 255.0f;
    sp34.unk8 = 255.0f;
    sp34.unkD = arg1;
    sp34.unk0 = arg0;
    sp34.unkC = (s32)((arg3 == -1) ? 0 : 4);
    sp34.unk54 = arg3;
    sp34.unk14 = 255.0f;
    sp90.unk12 = 0x12C;
    sp90.unk8 = 49.0f;
    sp34.unk10 = 0.0f;
    sp90.unkC = 18.0f;
    sp90.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg2 ? 0x40 : 0) | 0x32);
    sp90.unk16 = 1;
    sp90.unk18 = 0xFF;
    sp90.unk1A = 7;
    sp90.unk1B = 0xFF;
    sp90.unk1C = 0xFF;
    sp90.unk1D = 0xFF;
    sp90.unk1F = 0xFF;
    sp90.unk20 = 0xFF;
    sp90.unk21 = 0xFF;
    sp90.unk22 = 0xFF;
    sp90.unk23 = 0xFF;
    sp90.unk24 = 0;
    sp90.unk28 = 0x200004;
    sp90.unk2C = 0x1F0601;
    sp90.unk30 = 7;
    sp90.unk34 = 0x22;
    sp90.unk38 = 0x80;
    sp90.unk3C = 0x20;
    sp90.unk40 = 0;
    sp90.unk41 = 7;
    sp90.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp34.unkE = 0;
    sp90.unk1E = 0xFF;
    sp90.unk10 = 0x7B;
    sp34.unk1C = 0.0f;
    sp90.unk0 = 88.0f;
    sp90.unk4 = 91.0f;
    sp34.unk18 = D_800AAD84[D_80082FA0 == 1];
    sp34.unk20 = D_800AADAC[D_80082FA0 == 1];
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk1E = 0;
    sp90.unk10 = 0x7C;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk14 &= 0xFFFD;
    sp90.unk1E = 0xFF;
    sp90.unk10 = 0x7B;
    sp34.unkE = 1;
    sp90.unk0 = -88.0f;
    sp90.unk4 = 91.0f;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk1E = 0;
    sp90.unk10 = 0x7C;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
}

void func_151CB918(Game1F4650EffectObject *arg0, Game1F4650EffectTriggerEvent *arg1, u8 arg2) {
    Game1F4650EffectState *base = &arg0->field_0x70;

    if (arg2 == 0x37) {
        if (base->field_0xE == arg1->field_0x0) {
            if (base->field_0x0 == arg1->field_0x4) {
                base->field_0x10 = 0.0f;
                base->field_0xC |= 0x1;
                base->field_0x14 = 0.0f;
            }
        }
    }
}

s32 func_151CB970(void *arg0) {
    typedef struct {
        u8 pad0[0xC];
        u8 unkC;
        u8 unkD;
        u8 padE[2];
        f32 unk10;
        f32 unk14;
        f32 unk18;
        u8 pad1C[4];
        f32 unk20;
    } Struct151CB970;
    register Struct151CB970 *base;
    f32 temp_f0;
    f32 pad0;
    f32 temp_f2;
    f32 temp_f12;
    u8 temp_v0;

    base = (Struct151CB970 *)((s32)arg0 + 0x70);
    temp_f0 = func_151CC1D4(arg0);
    temp_f2 = (1.0f - ((temp_f0 - base->unk18) * base->unk20)) * 75.0f;

    if (base->unkC & 1) {
        temp_f0 = sinf(base->unk10);
        temp_f12 = 91.0f + temp_f2;
        pad0 = (112.0f + temp_f2) - temp_f12;
        *(f32 *)((s32)arg0 + 0x14) = (temp_f0 * pad0) + temp_f12;
        base->unk10 += D_800AAEAC * D_800BE9A4;
        if (D_800AAEB0 <= base->unk10) {
            *(f32 *)((s32)arg0 + 0x14) = temp_f12;
            base->unkC &= ~1;
        }
    } else {
        *(f32 *)((s32)arg0 + 0x14) = 91.0f + temp_f2;
    }

    if (base->unkD == 2) {
        *(f32 *)((s32)arg0 + 0x14) -= 20.0f;
    }

    temp_v0 = (u32)base->unk14;
    if (*(u8 *)((s32)arg0 + 0x20) == 0x7C) {
        *(u8 *)((s32)arg0 + 0x2E) = 0xFF - temp_v0;
    } else {
        *(u8 *)((s32)arg0 + 0x2E) = temp_v0;
    }

    base->unk14 += (255.0f - base->unk14) * D_800AAEB4;
    return 1;
}

void func_151CBB6C(struct102 *arg0, void **arg1, u8 arg2) {
    void *temp_v0;
    s32 temp_a2;

    temp_a2 = arg2;
    if (temp_a2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x20) == *(s32 *)arg1) {
            func_1516972C(arg0);
        }
    } else if (temp_a2 == 0) {
        temp_v0 = *arg1;
        if (*(s32 *)((s32)arg0 + 0x20) == *(s32 *)((s32)temp_v0 + 0x318)) {
            func_1516972C(arg0);
        }
    }
}

s32 func_151CBBE0(void *arg0) {
    typedef struct {
        u8 pad0[0x18];
        f32 unk18;
        u8 pad1C[4];
        f32 unk20;
        f32 unk24;
        f32 unk28;
    } Struct151CBBE0;
    register Struct151CBBE0 *base;
    f32 scaled;
    f32 temp_f0;

    base = (Struct151CBBE0 *)((s32)arg0 + 0x70);
    temp_f0 = func_151CC1D4(arg0);
    if (temp_f0 != base->unk28) {
        base->unk28 = temp_f0;
        scaled = ((temp_f0 - base->unk18) * base->unk20) * D_800AAEB8;
        func_1514373C(base->unk24 + scaled,
            55.0f, (f32 *)((s32)arg0 + 0x10), (f32 *)((s32)arg0 + 0x14));
    }
    return 1;
}

s32 func_151CBC60(u8 *arg0) {
    u8 *state = arg0 + 0x70;
    f32 value;
    f32 base;

    value = func_151CC1D4(arg0);
    if (value != *(f32 *)(state + 0x28)) {
        base = *(f32 *)(state + 0x4C);
        *(f32 *)(state + 0x28) = value;
        *(f32 *)(arg0 + 0x1C) = base - *(f32 *)(state + 0x20) * (value - *(f32 *)(state + 0x18)) * base + 1.0f;
    }
    if (*(state + 0xC) & 2) {
        *(f32 *)(state + 0x50) -= D_800BE9A4;
        value = *(f32 *)(state + 0x50);
        if (value <= 0.0f) {
            arg0[0x2B] = *(f32 *)(state + 0x3C);
            arg0[0x2C] = *(f32 *)(state + 0x40);
            arg0[0x2D] = *(f32 *)(state + 0x44);
            arg0[0x2E] = *(f32 *)(state + 0x48);
            *(state + 0xC) &= ~2;
        } else {
            arg0[0x2B] = (*(f32 *)(state + 0x2C) - *(f32 *)(state + 0x3C)) * (value * 0.028571429f) + *(f32 *)(state + 0x3C);
            arg0[0x2C] = (*(f32 *)(state + 0x30) - *(f32 *)(state + 0x40)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x40);
            arg0[0x2D] = (*(f32 *)(state + 0x34) - *(f32 *)(state + 0x44)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x44);
            arg0[0x2E] = (*(f32 *)(state + 0x38) - *(f32 *)(state + 0x48)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x48);
        }
    }
    return 1;
}

extern f32 D_800AAEC0;

f32 func_151CC1D4(void *arg0) {
    typedef struct {
        u8 *unk0;
        u8 pad4[0x8];
        u8 unkC;
        u8 padD[0xB];
        f32 unk18;
        f32 unk1C;
        u8 pad20[0x34];
        s8 unk54;
    } Struct151CC1D4;
    Struct151CC1D4 *base;
    f32 v;

    base = (Struct151CC1D4 *)((s32)arg0 + 0x70);
    if (base->unkC & 4) {
        f32 t = D_800C3958[base->unk54].unk28 * D_800AAEC0;

        v = base->unk18 + (base->unk1C - base->unk18) * t;
    } else {
        v = (*(f32 *)(base->unk0 + 0x1A0) + *(f32 *)(base->unk0 + 0x19C)) * 0.5f;
    }
    if (v < base->unk18) {
        v = base->unk18;
    } else if (base->unk1C < v) {
        v = base->unk1C;
    }
    return v;
}


extern void func_1515572C(s32, s32);

void func_151CC290(s32 arg0) {
    s32 sp1C;

    sp1C = arg0;
    func_1515572C((s32)&sp1C, 0x46);
}

void func_151CC2BC(void *arg0, s32 *arg1, u8 arg2) {
    typedef struct {
        s32 field_0x0;
        u8 pad_0x4[0x8];
        u8 field_0xC;
        u8 pad_0xD[0x1F];
        f32 field_0x2C;
        f32 field_0x30;
        f32 field_0x34;
        f32 field_0x38;
        u8 pad_0x3C[0x14];
        f32 field_0x50;
    } State;
    State *base;

    base = (State *)((s32)arg0 + 0x70);
    if ((arg2 == 0x46) && (*arg1 == *(s32 *)((u8 *)base + 0))) {
        base->field_0xC |= 2;
        base->field_0x50 = 35.0f;
        *(u8 *)((s32)arg0 + 0x2B) = (u32)base->field_0x2C;
        *(u8 *)((s32)arg0 + 0x2C) = (u32)base->field_0x30;
        *(u8 *)((s32)arg0 + 0x2D) = (u32)base->field_0x34;
        *(u8 *)((s32)arg0 + 0x2E) = (u32)base->field_0x38;
    }
}

void func_151CC524(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CC524_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CC524_sp2C;
    struct_151CC524_sp88 sp88;
    void *temp_v0;
    struct_151CC524_sp2C sp2C;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk50 = 0.0f;
    sp88.unk54 = 0.0f;
    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10 | 0x80 | 0x100 | 0x200);
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0x82;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk48 = 0.200000003f;
    sp88.unk4C = 1.0f;
    sp88.unk8 = 28.0f;
    sp88.unkC = 4.199999809f;
    sp88.unk4 = -37.0f;
    sp88.unk10 = 0x9A;
    sp88.unk0 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 9, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9B;
    sp88.unk4 = 0.0f;
    sp88.unk48 = 1.0f;
    sp88.unk4C = 0.200000003f;
    sp88.unk8 = 4.199999809f;
    sp88.unkC = 28.0f;
    sp88.unk0 = -37.0f;
    temp_v0 = func_1515548C(&sp88, 9, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

s32 func_151CC77C(void *arg0) {
    register f32 *temp_a1;
    register f32 *temp_a2;
    struct {
        f32 sp20;
        f32 sp24;
        s32 pad28;
        s32 pad2C;
    } sp20;
    void *temp_v0;
    void *temp_v1;

    temp_v0 = *(void **)((s32)arg0 + 0x70);
    temp_v1 = *(void **)((s32)temp_v0 + 0x3D0);
    if (temp_v1 == NULL) {
        return 0;
    }
    temp_v0 = *(void **)((s32)temp_v1 + 0x31C);
    temp_a1 = &sp20.sp24;
    temp_a2 = &sp20.sp20;
    if (temp_v0 == NULL) {
        return 0;
    }
    func_15145974((struct17 *)((s32)temp_v0 + 0x130), temp_a1, temp_a2);
    if (*(u8 *)((s32)arg0 + 0x20) == 0x9A) {
        *(f32 *)((s32)arg0 + 0x60) = -((sp20.sp24 * D_800AAED4) * 2560.0f) * 2.0f;
    } else {
        *(f32 *)((s32)arg0 + 0x64) = ((sp20.sp20 * D_800AAED8) * 2560.0f) * 2.0f;
    }
    return 1;
}

void func_151CC840(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CC840_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CC840_sp2C;
    struct_151CC840_sp88 sp88;
    void *temp_v0;
    struct_151CC840_sp2C sp2C;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10);
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0xB4;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 0.0f;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk10 = 0x64;
    sp88.unkC = 12.0f;
    sp88.unk8 = 12.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9D;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unkC = 7.0f;
    sp88.unk8 = 7.0f;
    sp88.unk0 = 55.0f;
    sp88.unk4 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 2;
    sp88.unk0 = -55.0f;
    sp88.unk4 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9C;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 4;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 55.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unk0 = 0.0f;
    sp88.unk4 = -55.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9E;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 6;
    sp88.unkC = 6.0f;
    sp88.unk8 = 6.0f;
    sp88.unk0 = 44.0f;
    sp88.unk4 = 44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 4;
    sp88.unk0 = -44.0f;
    sp88.unk4 = 44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unk0 = -44.0f;
    sp88.unk4 = -44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 2;
    sp88.unk0 = 44.0f;
    sp88.unk4 = -44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

void func_151CCD1C(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        s16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CCD1C_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4E];
    } struct_151CCD1C_sp2C;
    struct_151CCD1C_sp88 sp88;
    struct_151CCD1C_sp2C sp2C;
    void *temp_v0;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0xB4;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 0.0f;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk10 = 0xB4;
    sp88.unk8 = 65.0f;
    sp88.unkC = 65.0f;

    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

void func_151CCE94(void *arg0) {
    void *temp_v0;
    void *sp28;

    if (D_800BE616 != 0) {
        sp28 = arg0;
        temp_v0 = func_151A4FD0(0, 0, 0, 0xFF, 0, *(u8 *)((s32)arg0 + 0x23D), 1, 4);
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x20), &sp28, 4);
        }
    }
}

extern f32 D_800AAD8C[];
extern f32 D_800AADB4[];
typedef struct { u8 b[4]; } Colors151CCF08;
extern Colors151CCF08 D_8008FC28;

void func_151CCF08(void *arg0, u8 arg1, s8 arg2, u8 arg3) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CCF08_spE0;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0xA];
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        u8 pad24[0x30];
        s8 unk54;
        u8 pad55[3];
    } struct_151CCF08_sp84;
    struct_151CCF08_spE0 spE0;
    void *temp_v0;
    struct_151CCF08_sp84 sp84;
    u8 i;
    Colors151CCF08 sp7C;
    u8 j;

    sp84.unk1C = 0.0f;
    sp84.unk0 = arg0;
    sp84.unk4 = 0.0f;
    sp84.unk18 = D_800AAD8C[D_80082FA0 == 1];
    sp84.unk20 = D_800AADB4[D_80082FA0 == 1];
    sp84.unk8 = 130.0f;
    sp84.unkC = (s32)((arg2 == -1) ? 0 : 4);
    sp84.unk54 = arg2;

    spE0.unk12 = 0x12C;
    spE0.unk14 = (arg1 ? 0x40 : 0) | 0x20 | 0x10;
    spE0.unk16 = 1;
    spE0.unk18 = 0xFF;
    spE0.unk1A = 7;
    spE0.unk1B = 0xFF;
    spE0.unk1C = 0xFF;
    spE0.unk1D = 0xFF;
    spE0.unk1E = 0xFF;
    spE0.unk1F = 0xFF;
    spE0.unk20 = 0xFF;
    spE0.unk21 = 0xFF;
    spE0.unk22 = 0xFF;
    spE0.unk23 = 0xFF;
    spE0.unk24 = 0;
    spE0.unk28 = 0x200004;
    spE0.unk2C = 0x1F0601;
    spE0.unk30 = 8;
    spE0.unk34 = 0x13;
    spE0.unk38 = 0x80;
    spE0.unk3C = 0x20;
    spE0.unk40 = 0;
    spE0.unk41 = 0;
    spE0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spE0.unk50 = 0.0f;
    spE0.unk54 = 0.0f;
    spE0.unk48 = 1.0f;
    spE0.unk4C = 1.0f;
    spE0.unk8 = 35.75f;
    spE0.unkC = 16.0f;

    for (j = 0; j < 2; j++) {
        sp7C = D_8008FC28;
        spE0.unk4 = j ? 92.0f : -90.0f;
        spE0.unk14 |= j ? 4 : 0;
        sp84.unkD = j ? 5 : 4;
        for (i = 0; i < 4; i++) {
            spE0.unk0 = ((i * 71.5f) + 35.75f) - 143.0f;
            spE0.unk10 = sp7C.b[i];
            temp_v0 = func_1515548C(&spE0, 0xB, 0, 0, 0x58, arg3, 1);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0x70, &sp84, 0x58);
            }
        }
    }
}

extern f32 func_151CC1D4(void *);

void func_151CD224(Game1F4650EffectObject *arg0) {
    Game1F4650EffectState *base = &arg0->field_0x70;
    f32 ret = func_151CC1D4(arg0);
    f32 temp = (1.0f - (ret - base->field_0x18) * base->field_0x20) * 75.0f;
    if (base->field_0xD == 5) {
        arg0->field_0x14 = 92.0f + temp;
    } else if (base->field_0xD == 4) {
        arg0->field_0x14 = -92.0f - temp;
    }
}
