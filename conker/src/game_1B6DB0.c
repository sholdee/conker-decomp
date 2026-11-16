#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad0[0x8];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x30];
} Struct15189900Arg;

typedef struct {
    u8 pad0[0x10];
    Struct15189900Arg unk10;
    s16 unk60;
    u8 pad62[0x2];
    f32 unk64;
    f32 unk68;
    s32 unk6C;
    u8 unk70;
} Struct15189900Obj;

extern Struct15189900Obj *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_1510F800(s32);


void func_15189900(Struct15189900Arg *arg0, s32 arg1) {
    Struct15189900Obj *temp_v0;
    s32 var_v0;

    temp_v0 = func_15167A68(0x1A, 1, 0x78, 1, 0xFF, 1);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->unk10, arg0, 0x50);
        func_1510F800(0);
        temp_v0->unk6C = func_1510FD20((s32)(temp_v0->unk10.unk8 + (temp_v0->unk10.unk14 * 0.5f)), (s32)(temp_v0->unk10.unk10 + (temp_v0->unk10.unk1C * 0.5f)));
        if ((u8)arg1 != 0) {
            var_v0 = 1;
        } else {
            var_v0 = 0;
        }
        temp_v0->unk70 = var_v0;
        temp_v0->unk60 = 0;
        temp_v0->unk64 = 0.0f;
        if (temp_v0->unk10.unk18 < 0.0f) {
            temp_v0->unk68 = temp_v0->unk10.unkC + temp_v0->unk10.unk18;
        } else {
            temp_v0->unk68 = temp_v0->unk10.unkC;
        }
    }
}

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x3];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
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
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    s16 unk54;
    s16 unk56;
    s32 unk58;
    s32 unk5C;
    s16 unk60;
    u8 pad62[0x2];
    f32 unk64;
    f32 unk68;
    s32 unk6C;
    u8 unk70;
} Struct15189A00;

typedef struct {
    u8 pad0[0x98];
    u8 *unk98;
} Struct15189A00_ret;

extern s32 func_151464B8(s32);
extern Struct15189A00_ret *func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, s32 arg13, s32 arg14);
extern u8 D_800BE630[];

void func_15189A00(Struct15189A00 *arg0) {
    s32 i;
    s32 found;
    f32 *pos;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 idx;
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; s32 pad18; s32 pad1C; } sp10C;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; f32 unk14; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s16 unk1C; s16 unk1E; } spEC;
    f32 spE8;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } spC8;
    Struct15189A00_ret *ret;
    u8 *dst;

    if ((arg0->unk6C != 0) && (func_151464B8(arg0->unk6C) != 0)) {
        return;
    }
    i = D_80082FA0;
    arg0->unk60 -= D_800BE9E4;
    if (arg0->unk60 < 0) {
        arg0->unk60 += 50;
        found = 0;
        for (i = 0; (i <= D_80082FA0) && (found == 0); i++) {
            idx = D_800BE630[i];
            pos = &D_800DBFF0[idx].unk2F8;
            dx = pos[0] - arg0->unk18;
            dy = pos[1] - arg0->unk1C;
            dz = pos[2] - arg0->unk20;
            if (((dx * dx) + (dy * dy) + (dz * dz)) < 2500000.0f) {
                found = 1;
            }
        }
        if (found == 0) {
            return;
        }
    }
    arg0->unk64 += (arg0->unk38 + (func_150ADA68() * arg0->unk3C)) * D_800BE9A4;
    if (arg0->unk64 > 1.0f) {
        spE8 = arg0->unk68;
        sp10C.unk10 = 1;
        sp10C.unk0E = 5;
        spEC.unk1C = 0x10;
        spEC.unk1E = 0xF;
        spEC.unk14 = -10000.0f;
        spEC.unk18 = 0x48;
        spEC.unk19 = 3;
        spEC.unk1A = 0xFF;
        spC8.unk00 = 0;
        spC8.unk04 = 1;
        spC8.unk08 = 0x160600;
        spC8.unk0C = 3;
        spC8.unk10 = 0x22;
        spC8.unk14 = 0x80;
        spC8.unk18 = 0x20;
        spC8.unk1C = 0;
        spC8.unk1D = 7;
        do {
            sp10C.unk15 = (func_150ADA20() % (u32)(arg0->unk5C + 1)) + arg0->unk58;
            {
                f32 r;
                r = func_150ADA68();
                sp10C.x = arg0->unk18 + (r * arg0->unk24);
                sp10C.y = arg0->unk1C + (r * arg0->unk28);
                sp10C.z = arg0->unk20 + (r * arg0->unk2C);
            }
            spEC.unk00 = (func_150ADA68() * arg0->unk44) + arg0->unk40;
            {
            f32 mag;
            s16 ang1;
            s16 ang2;
            f32 sa1;
            f32 ca1;
            f32 sa2;
            f32 ca2;
            ang1 = arg0->unk10 - (func_150ADA20() % (u32)((arg0->unk12 * 2) + 1)) - -arg0->unk12;
            ang2 = (func_150ADA20() % (u32)(arg0->unk16 + 1)) + arg0->unk14;
            mag = (func_150ADA68() * arg0->unk34) + arg0->unk30;
            sa1 = func_151423D8((u8)ang1);
            ca1 = func_151423D8((u8)(ang1 - 0x40));
            sa2 = func_151423D8((u8)ang2);
            ca2 = func_151423D8((u8)(ang2 - 0x40));
            spEC.unk04 = (mag * sa2) * ca1;
            spEC.unk08 = -mag * ca2;
            spEC.unk0C = (mag * sa2) * sa1;
            }
            spEC.unk10 = (func_150ADA68() * arg0->unk50) + arg0->unk4C;
            spEC.unk1B = (func_150ADA20() % (u32)(arg0->unk4A + 1)) + arg0->unk48;
            sp10C.unk0C = (func_150ADA20() % (u32)(arg0->unk56 + 1)) + arg0->unk54;
            ret = func_15147DA0(&sp10C, &spEC, 8, 1, (arg0->unk70 & 1) ? 10 : 0, 0, 0, 0, 0, 0, 0, (s32 *)&spC8, 0, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                dst = ret->unk98;
                memcpy(dst + 0x48, &spE8, 4);
            }
            arg0->unk64 -= 1.0f;
        } while (arg0->unk64 > 1.0f);
    }
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Struct15189EBC_elem;

typedef struct {
    u8 unk0[0x1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x20];
    s8 unk2D;
    u8 pad2E[0x66];
    Struct15189EBC_elem *unk94;
    u8 *unk98;
} Struct15189EBC;

typedef struct {
    f32 unk0;
    u8 pad4[0x4];
    u8 unk8;
} Struct15189EBC_sub;

typedef struct {
    f32 unk0;
    u8 pad4[0x17];
    u8 unk1B;
    u8 pad1C[0x4];
    u8 unk20;
    u8 pad21[0x27];
    Struct15189EBC_sub unk48;
} Struct15189EBC_v1;

extern f64 D_800A73A8;
extern f32 D_800A73B0;
extern s32 func_151D8E20(void);
void func_151DBCBC(u8 arg0, f32 arg1, s16 arg2, s32 arg3, void *arg4, u8 arg5, s32 arg6);
f32 func_150ADA68(void);

s32 func_15189EBC(Struct15189EBC *arg0) {
    f64 tmp;
    Struct15189EBC_v1 *v1 = (Struct15189EBC_v1 *)arg0->unk98;
    Struct15189EBC_elem *base = arg0->unk94;
    struct17 sp34;

    if (base[arg0->unk2D].unk4 < v1->unk48.unk0) {
        tmp = func_150ADA68();
        if (tmp < D_800A73A8) {
            sp34.unk0 = base[arg0->unk2D].unk0;
            sp34.unk4 = v1->unk48.unk0;
            sp34.unk8 = base[arg0->unk2D].unk8;
            func_151DBCBC((u8)func_151D8E20(), v1->unk0 * 6.0f, v1->unk1B, 0, &sp34, arg0->unkC, arg0->unk1);
        }
        v1->unk20 = 4;
        v1->unk48.unk0 = D_800A73B0;
    }
    return 1;
}

s32 func_15189FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 *p = &arg0;
    return 1;
}
