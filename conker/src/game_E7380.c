#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"



typedef struct {
    s16 unk0[9];
} E7380HitInfo;

typedef struct {
    s16 unk0;
    s16 unk2;
    struct17 pos;
    struct17 vel;
    E7380HitInfo info;
    f32 unk30;
    u8 unk34;
    u8 unk35;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    f32 unk3C;
    f32 unk40;
    s32 unk44;
    u8 unk48;
    u8 unk49;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    u8 unk5C;
    u8 unk5D;
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    f32 unk6C;
    f32 unk70;
} E7380Spark;

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s32 unk18;
    u8 pad1C[6];
    s16 unk22;
    s16 unk24;
} E7380Debris;

typedef struct {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
} E7380Obj;

extern u8 D_80084060;
extern s32 func_151E7EF8[];
extern s32 func_151E7F60[];
extern s32 func_150A3058(s32, s16, s16, s16);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_151511FC(void *, u8, s32);

void func_150B9ED0(E7380Obj *arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
    s32 pad[3];
    u32 p;
    u32 end;
    s32 sum;

    if (arg0 != NULL) {
        sum = 0;
        end = (u32)func_151E7F60;
        for (p = (u32)func_151E7EF8; p < end; p += 4) {
            sum += ~*(s32 *)p;
        }
        if (sum != 0x3843095E) {
            D_80084060 = 0xFF;
        }
        if ((func_150A3058(2, (s16)(s32)arg0->x, (s16)(s32)arg0->y, (s16)(s32)arg0->z) == 0) &&
            (func_150A3058(1, (s16)(s32)arg0->x, (s16)(s32)arg0->y, (s16)(s32)arg0->z) == 0)) {
            f32 ty = arg0->y + 80.0f;
            f32 dz;
            f32 dy;
            f32 dx;
            struct17 hit;
            E7380HitInfo info;
            s32 hitId;
            E7380Debris debris;
            E7380Spark spark;

            dy = 0.0f;
            if (func_150AC9C0(arg0->x, ty, arg0->z, dx = -(arg0->x - arg1), dy, dz = -(arg0->z - arg2), 0, &info, &hit.unk0, &hit.unk4, &hit.unk8, 0, &hitId, 0, 0.0f) != 0) {
                if (func_15145C90(hitId) != 0) {
                    debris.unk6 = 6;
                    debris.unk7 = 0;
                    debris.unk0 = 0x9F01;
                    debris.unk4 = (func_150ADA20() % 51U) + 100;
                    debris.unk8 = 0;
                    debris.unkC = 0;
                    debris.unk10 = (func_150ADA20() % 101U) + 155;
                    debris.unk11 = 0xFF;
                    debris.unk12 = 0xFF;
                    debris.unk13 = 0xFF;
                    debris.unk14 = 0xFF;
                    debris.unk15 = 0xFF;
                    debris.unk18 = 0x3B0002;
                    debris.unk16 = 0;
                    debris.unk17 = 7;
                    debris.unk22 = 0x19;
                    debris.unk24 = 0xA;
                    func_1513C650((s32)&debris, 0, 0, (s32)&info, hit.unk0, hit.unk4, hit.unk8,
                                  (func_150ADA68() * 20.0f) + 40.0f, (func_150ADA68() * 20.0f) + 40.0f, func_150ADA20() & 0xFF,
                                  ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1), 3, 0xFF, 0, arg3, arg4);
                }
                spark.unk0 = 0xF;
                spark.unk2 = 0xA;
                spark.pos = hit;
                spark.vel.unk0 = -dx;
                spark.vel.unk4 = -dy;
                spark.vel.unk8 = -dz;
                spark.info = info;
                spark.unk30 = 675.0f;
                spark.unk3C = 7.0f;
                spark.unk40 = 10.0f;
                spark.unk4C = -1.022000074f;
                spark.unk50 = 0.5420000553f;
                spark.unk34 = 7;
                spark.unk36 = 0x3B01;
                spark.unk35 = 0;
                spark.unk38 = 0x1A;
                spark.unk3A = 0x14;
                spark.unk44 = 0x5C0001;
                spark.unk48 = 0x64;
                spark.unk49 = 0x9B;
                spark.unk5C = 0;
                spark.unk5D = 7;
                spark.unk60 = 3;
                spark.unk64 = 0xFF;
                spark.unk68 = 0xA;
                spark.unk6A = 0x19;
                spark.unk54 = 0.0373999998f;
                spark.unk58 = 0.0234999992f;
                spark.unk6C = 0.9404039979f;
                spark.unk70 = 1.084000111f;
                func_151511FC(&spark, arg3, arg4);
            }
        }
    }
}

s32 func_150BA35C(s16 *arg0) {
    s16 temp = arg0[0xE];
    if (temp < 0x40) {
        *((s8 *)arg0 + 0x28) = temp << 2;
    }
    return 1;
}

extern f32 D_800BE9A4;
extern f32 D_8009FE2C;

s32 func_150BA37C(s32 *arg0) {
    *(f32 *)((s8 *)arg0 + 0x114) = *(f32 *)((s8 *)arg0 + 0x114) + D_8009FE2C * D_800BE9A4;
    *(f32 *)((s8 *)arg0 + 0x34) = *(f32 *)((s8 *)arg0 + 0x110) * D_800BE9A4 + *(f32 *)((s8 *)arg0 + 0x34);
    *(f32 *)((s8 *)arg0 + 0x38) = *(f32 *)((s8 *)arg0 + 0x114) * D_800BE9A4 + *(f32 *)((s8 *)arg0 + 0x38);
    *(f32 *)((s8 *)arg0 + 0x3C) = *(f32 *)((s8 *)arg0 + 0x118) * D_800BE9A4 + *(f32 *)((s8 *)arg0 + 0x3C);
    *(f32 *)((s8 *)arg0 + 0x40) = *(f32 *)((s8 *)arg0 + 0x11C) * D_800BE9A4 + *(f32 *)((s8 *)arg0 + 0x40);
    *(f32 *)((s8 *)arg0 + 0x44) = *(f32 *)((s8 *)arg0 + 0x120) * D_800BE9A4 + *(f32 *)((s8 *)arg0 + 0x44);
    return 1;
}

s32 func_150BA424(s32 *arg0) {
    f32 diff = *(f32 *)((s8 *)arg0 + 0x38) - *(f32 *)((s8 *)arg0 + 0x124);
    s32 a;
    s32 b;

    if (diff < 0.0f) {
        return 0;
    }

    a = *(s16 *)((s8 *)arg0 + 0x1C) << 4;
    if (a >= 0x100) {
        a = 0xFF;
    }

    b = (s32)diff << 2;
    if (b >= 0x100) {
        b = 0xFF;
    }

    if (b < a) {
        *((s8 *)arg0 + 0x5C) = b;
    } else {
        *((s8 *)arg0 + 0x5C) = a;
    }

    if (*((u8 *)arg0 + 0x5C) < 0) {
        return 0;
    }
    return 1;
}
