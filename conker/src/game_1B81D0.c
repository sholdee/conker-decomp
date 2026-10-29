#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    s32 pad78;
} S1518AD20;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    u8 pad0C[0xA];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
} S1518AD20b;

typedef struct {
    u8 pad00;
    u8 unk01;
    u8 pad02[0xA];
    u8 unk0C;
    u8 pad0D[3];
    f32 unk10;
    u8 pad14[0x24];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x22];
    u16 unk66;
    u8 pad68[0x108];
    f32 unk170;
} S1518AD20Obj;

extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_1518CA80(void *, u8);
extern void func_151429E0(u8, u8 *, u8 *, u8 *);

s32 func_1518AD20(S1518AD20Obj *arg0) {
    f32 *pos;

    pos = &arg0->unk170;
    if (arg0->unk3C < arg0->unk170) {
        S1518AD20 sp;
        S1518AD20b sp2;
        s16 i;
        f32 scale;
        u8 yaw;
        u8 pitch;
        f32 speed;
        f32 sinYaw;
        f32 cosYaw;
        f32 sinPitch;
        f32 cosPitch;
        void *v;
        u8 color;
        s32 temp;
        s32 sel;

        sp.unk56 = arg0->unk66;
        sp.unk54 = 0x32;
        sp.unk1C = 1.0f;
        sp.unk20 = 1.0f;
        sp.unk24 = 1.0f;
        sp.unk50 = 0x9E8;
        sp.unk04 = 0.400000006f;
        sp.unk48 = 0.0f;
        sp.unk4C = -1.299999952f;
        sp.unk28 = arg0->unk38;
        sp.unk2C = *pos;
        sp.unk30 = arg0->unk40;
        sp.unk58 = 0;
        sp.unk5C = 0;
        sp.unk60 = 0xFF;
        sp.unk61 = 1;
        sp.unk63 = 0;
        sp.unk64 = 0;
        sp.unk65 = 0;
        sp.unk66 = 0;
        sp.unk67 = 0;
        sp.unk68 = 0;
        sp.unk6A = 2;
        sp.unk6C = 0;
        sp.unk70 = 0;
        sp.unk72 = 1;
        sp.unk74 = 0xFF;
        i = (func_150ADA20() & 3) + 4;
        for (; i > 0; i--) {
            scale = (func_150ADA68() * (arg0->unk10 * 0.5f)) + 0.05000000075f;
            yaw = func_150ADA20() & 0xFF;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            speed = (func_150ADA68() * 10.0f) + 5.0f;
            sinYaw = func_151423D8((u8)(yaw - 0x40));
            cosYaw = func_151423D8(yaw);
            sinPitch = func_151423D8((u8)(pitch - 0x40));
            cosPitch = func_151423D8(pitch);
            sp.unk00 = scale;
            sp.unk08 = scale;
            sp.unk0C = scale;
            sp.unk34 = speed * cosPitch * sinYaw;
            sp.unk38 = -speed * sinPitch;
            sp.unk3C = speed * cosPitch * cosYaw;
            sp.unk10 = func_150ADA68() * 360.0f;
            sp.unk14 = func_150ADA68() * 360.0f;
            sp.unk18 = func_150ADA68() * 360.0f;
            sp.unk40 = 25.0f - (func_150ADA68() * 50.0f);
            sp.unk44 = 25.0f - (func_150ADA68() * 50.0f);
            if (scale > 0.25f) {
                sp.unk62 = 2;
            } else {
                sp.unk62 = 4;
            }
            v = func_15132A4C(&sp, 3, 0xFF, 4, arg0->unk0C, arg0->unk01);
            if (v != NULL) {
                *(f32 *)((u8 *)v + 0x170) = *pos;
            }
        }
        switch (arg0->unk66) {
            case 4:
                sel = 1;
                break;
            case 5:
                sel = 0;
                break;
            default:
                sel = 0;
                break;
        }
        sp2.unk00 = arg0->unk38;
        sp2.unk04 = *pos;
        sp2.unk08 = arg0->unk40;
        sp2.unk18 = sp2.unk16 = arg0->unk10 * 200.0f;
        sp2.unk1C = sp2.unk1A = ((func_150ADA68() * 10.0f) + 20.0f) * arg0->unk10;
        sp2.unk1E = 0;
        sp2.unk20 = 0;
        sp2.unk22 = (func_150ADA20() % 201U) + 0x12C;
        sp2.unk24 = 0;
        sp2.unk26 = 0x258;
        color = sel;
        func_151429E0(color, &sp2.unk28, &sp2.unk29, &sp2.unk2A);
        func_151429E0(color, &sp2.unk2B, &sp2.unk2C, &sp2.unk2D);
        sp2.unk2E = 0xFF;
        temp = arg0->unk10 * 511.0f;
        if (temp > 255.0f) {
            sp2.unk2F = 0xFF;
        } else {
            sp2.unk2F = temp;
        }
        sp2.unk30 = 0xA;
        sp2.unk31 = 0;
        func_1518CA80(&sp2, 1);
        return 0;
    }
    return 1;
}

s32 func_1518B1AC(void *arg0) {
    if (*(f32 *)((u8 *)arg0 + 0x3C) < *(f32 *)((u8 *)arg0 + 0x170)) {
        return 0;
    }
    return 1;
}

s32 func_1518B1D8(void *arg0) {
    f32 temp_f0;
    s32 var_v0;
    s32 var_a1;
    s32 temp_v1;

    temp_f0 = *(f32 *)((u8 *)arg0 + 0x3C) - *(f32 *)((u8 *)arg0 + 0x170);
    if (temp_f0 < 0.0f) {
        return 0;
    }

    var_v0 = *(s16 *)((u8 *)arg0 + 0x64);
    var_v0 <<= 2;
    if (var_v0 >= 0x100) {
        var_v0 = 0xFF;
    }

    temp_v1 = (s32)temp_f0 >> 1;
    var_a1 = var_v0;
    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }

    var_a1 = (temp_v1 < var_v0) ? temp_v1 : var_v0;

    if (var_a1 < 0) {
        return 0;
    }

    *(u8 *)((u8 *)arg0 + 0x70) = var_a1;
    return 1;
}

void func_1518B2A8(f32 *, f32, f32, s32, u16, s32, u8);

void func_1518B264(s32 arg0, f32 arg1, f32 arg2, s32 arg3, u8 arg4) {
    func_1518B2A8(arg0, arg1, arg2, arg3, 4, 0xFF, arg4);
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
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    u16 unk56;
    u8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 pad78;
} S1518B2A8;

typedef struct {
    f32 unk00;
    u8 pad04[0x14];
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    s32 unk20;
} S1518B2A8Ground;

extern s32 func_15046C80(f32 *, s32, f32, S1518B2A8Ground *);
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);

void func_1518B2A8(f32 *arg0, f32 arg1, f32 arg2, s32 arg3, u16 arg4, s32 arg5, u8 arg6) {
    S1518B2A8 p;
    S1518B2A8Ground ground;
    u8 angle;
    f32 dist;
    void *v;
    s32 n;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    if (arg0 != NULL) {
        p.unk56 = arg4;
        ground.unk18 = 0;
        ground.unk1C = 0;
        ground.unk1D = 0;
        ground.unk20 = 0;
        p.unk54 = 0x12C;
        p.unk50 = 0x9E8;
        p.unk58 = 0;
        p.unk5C = 0;
        p.unk60 = 0xFF;
        p.unk61 = 1;
        p.unk63 = 0;
        p.unk64 = 0;
        p.unk65 = 0;
        p.unk66 = 0;
        p.unk67 = 0;
        p.unk68 = 0;
        p.unk6A = 2;
        p.unk6C = NULL;
        p.unk70 = 0;
        p.unk72 = 1;
        p.unk74 = 0xFF;
        p.unk1C = 1.0f;
        p.unk20 = 1.0f;
        p.unk24 = 1.0f;
        ground.unk00 = 99999.8984375f;
        p.unk48 = 0.0f;
        p.unk34 = 0.0f;
        p.unk38 = 0.0f;
        p.unk3C = 0.0f;
        p.unk04 = 0.4000000060f;
        p.unk4C = -1.299999952f;
        for (; arg3 > 0; arg3--) {
            angle = func_150ADA20();
            angle &= 0xFF;
            dist = func_150ADA68() * arg1;
            p.unk08 = arg2;
            p.unk0C = arg2;
            p.unk00 = arg2;
            p.unk10 = func_150ADA68() * 360.0f;
            p.unk14 = func_150ADA68() * 360.0f;
            p.unk18 = func_150ADA68() * 360.0f;
            p.unk28 = arg0[0] + func_151423D8(angle) * dist;
            p.unk2C = (arg0[1] + arg1) - (func_150ADA68() * 2.0f * arg1);
            p.unk30 = arg0[2] - func_151423D8(angle - 0x40) * dist;
            p.unk40 = 25.0f - func_150ADA68() * 50.0f;
            p.unk44 = 25.0f - func_150ADA68() * 50.0f;
            if (func_15046C80(&p.unk28, 0, -10000.0f, &ground) != 0) {
                p.unk62 = 2;
                v = func_15132A4C(&p, 3, 0xFF, 4, arg6, 0);
                if (v != NULL) {
                    *(f32 *)((u8 *)v + 0x170) = ground.unk00;
                }
                n = (func_150ADA20() & 3) + 1;
                x = p.unk28;
                y = arg2 * 16.0f + p.unk2C;
                z = p.unk30;
                for (; n > 0; n--) {
                    angle = func_150ADA20();
            angle &= 0xFF;
                    dist = func_150ADA68() * arg2 * 16.0f;
                    scale = func_150ADA68() * 0.3000000119f + 0.1000000015f;
                    p.unk0C = scale;
                    p.unk08 = scale;
                    p.unk00 = scale;
                    p.unk10 = func_150ADA68() * 360.0f;
                    p.unk14 = func_150ADA68() * 360.0f;
                    p.unk18 = func_150ADA68() * 360.0f;
                    p.unk28 = func_151423D8(angle) * dist + x;
                    p.unk2C = func_150ADA68() * 25.0f + y;
                    p.unk30 = z - func_151423D8(angle - 0x40) * dist;
                    p.unk40 = 25.0f - func_150ADA68() * 50.0f;
                    p.unk44 = 25.0f - func_150ADA68() * 50.0f;
                    p.unk62 = 3;
                    v = func_15132A4C(&p, 3, 0xFF, 4, arg6, 0);
                    if (v != NULL) {
                        *(f32 *)((u8 *)v + 0x170) = ground.unk00;
                    }
                }
            }
        }
    }
}
