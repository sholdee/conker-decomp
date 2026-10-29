#define func_150ADA20 func_150ADA20_orig
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_150ADA20
s32 func_150ADA20(void);

extern f32 D_8009FE7C;
extern f32 D_8009FE80;
extern f32 D_8009FE84;
extern f32 D_8009FE88;
extern f32 D_8009FE8C;
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_150BABE0;

typedef struct {
    f32 unk0;
    u8 unk4[0x18];
    u8 unk1C;
    u8 unk1D;
    s32 unk20;
} Ground_150BABE0;

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
    s32 unk1C;
    u8 unk20;
    u8 pad21[7];
} Particle_150BABE0;

void func_15142314(s32, s32, Vec3F_150BABE0 *);
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);
void func_1504715C(Ground_150BABE0 *, struct127 *);
s32 func_15046C80(Vec3F_150BABE0 *, s32, f32, Ground_150BABE0 *);
s32 func_1518ABD0(s32, struct210 *, u8);

void func_150BABE0(struct127 *arg0, u8 arg1, u8 arg2) {
    Vec3F_150BABE0 pos;
    s32 type;
    s32 spBC;
    Ground_150BABE0 ground;
    Vec3F_150BABE0 probe;
    Particle_150BABE0 part;
    struct210 *ret;
    f32 size;

    spBC = (s32)(D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f);
    if (arg0 == NULL) {
        return;
    }
    if (arg0->interaction_state == 0 || arg0->interaction_state == 8) {
        return;
    }
    if (arg0->unk1D4 == 0) {
        return;
    }
    switch (arg1) {
    case 0:
        type = 0x11;
        break;
    case 1:
        type = 0x1A;
        break;
    }
    func_15142314(arg0->unk1D4, type, &pos);
    pos.y = arg0->unk180;
    func_15143E94(5, 0x4022);
    func_15165F80(-1, (s32)pos.x, (s32)(pos.y + 6.0f), (s32)pos.z, 0x19, 0x12, 0, *(u8 *)&arg2, 1);
    func_1514C678(pos.x, pos.y, pos.z, (func_150ADA68() * 50.0f) + 40.0f, spBC + 0x3C, spBC - 0x3C,
                  (func_150ADA20() % 11U) + 0x1E, 5, 0, 0.0f, 0, *(u8 *)&arg2);
    func_1504715C(&ground, arg0);
    probe.x = pos.x;
    probe.y = pos.y + 100.0f;
    probe.z = pos.z;
    if (func_15046C80(&probe, 0, pos.y - 100.0f, &ground) != 0) {
        size = (func_150ADA68() * 200.0f) + 200.0f;
        part.unk6 = 0x2A;
        part.unk7 = 0;
        part.unk0 = 0x7340;
        part.unk4 = 0x12C;
        part.unk8 = 0;
        part.unkC = (func_150ADA20() % 2001U) + 0x7D0;
        part.unk10 = 0xFF;
        part.unk11 = 0xFF;
        part.unk12 = 0;
        part.unk13 = 0;
        part.unk14 = 0;
        part.unk15 = 0xFF;
        part.unk16 = 0;
        part.unk17 = 0xD;
        part.unk20 = 0xFF;
        part.unk1C = 0;
        part.unk18 = 0x170002;
        ret = func_1513C73C((s32)&part, 0, 0, (s32)ground.unk4, pos.x, ground.unk0, pos.z, size, size,
                            func_150ADA20() & 0xFF, ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1), 4, *(u8 *)&arg2, 0);
        if (ret != NULL) {
            if (func_1518ABD0(D_80088750, ret, 1) == 0) {
                func_1516972C((struct102 *)ret);
            }
        }
    }
    func_1514C678(pos.x, pos.y, pos.z, (func_150ADA68() * 40.0f) + 60.0f, spBC + 0x3C, spBC - 0x3C,
                  (func_150ADA20() % 7U) + 6, 6, 0, 0.0f, 0, *(u8 *)&arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E8090/func_150BAFEC.s")

s32 func_150BB260(void *arg0, s32 arg1) {
    s32 i;
    f32 *scale;

    scale = (f32 *)((u8 *)arg0 + 0xA8);
    for (i = D_800BE9E4; i != 0; i--) {
        *(f32 *)((u8 *)arg0 + 0x58) *= *scale;
        *(f32 *)((u8 *)arg0 + 0x60) *= *scale;
    }
    return 1;
}

extern void func_1513F6C0(void *arg0, s32 arg1, s32 arg2);

void func_150BB408(void *arg0) {
    *(s32 *)((u8 *)arg0 + 0x18) |= 1;
    *(s16 *)((u8 *)arg0 + 0xB0) = (*(s16 *)((u8 *)arg0 + 0x1C) = 0x32);
    *(s16 *)((u8 *)arg0 + 0xB2) = 5;
    func_1513F6C0(arg0, 6, *(u8 *)((u8 *)arg0 + 0x81));
}

s32 func_150BB450(void *arg0) {
    s32 temp;
    if (*(s16 *)((u8 *)arg0 + 0x1C) < *(s16 *)((u8 *)arg0 + 0xB0)) {
        temp = (s16)(*(s16 *)((u8 *)arg0 + 0xB2) * *(s16 *)((u8 *)arg0 + 0x1C));
        if (temp < *(u8 *)((u8 *)arg0 + 0x28)) {
            *(u8 *)((u8 *)arg0 + 0x28) = temp;
        }
    }
    return 1;
}

s32 func_150BB498(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    s16 angle;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    f32 spAC;
    f32 temp_f2;
    struct {
        s32 pad00;
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
        s16 unk56;
        u8 unk58;
        u8 pad59[3];
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
        u8 pad6B;
        s32 unk6C;
        u8 unk70;
        u8 pad71;
        s16 unk72;
        s16 unk74;
        u8 pad76[6];
    } sp2C;

    arg0 = arg8 - 0x40;
    spB8 = func_151423D8(arg0);
    spB4 = func_151423D8((u8)arg8);

    sp2C.unk50 = 0x9E8;
    sp2C.unk10 = 0.0f;
    sp2C.unk14 = 0.0f;
    sp2C.unk18 = 0.0f;
    sp2C.unk1C = 1.0f;
    sp2C.unk20 = 1.0f;
    sp2C.unk24 = 1.0f;
    sp2C.unk28 = arg2;
    sp2C.unk2C = arg3;
    sp2C.unk30 = arg4;
    sp2C.unk04 = D_8009FE7C;

    angle = (func_150ADA20() % 13U) - 0x34;
    arg0 = angle - 0x40;
    spB0 = func_151423D8(arg0);
    spAC = func_151423D8((u8)angle);

    temp_f2 = (func_150ADA68() * 9.0f) + 3.0f;
    sp2C.unk34 = (temp_f2 * spAC) * spB8;
    sp2C.unk38 = -temp_f2 * spB0;
    sp2C.unk3C = (temp_f2 * spAC) * spB4;

    sp2C.unk40 = (func_150ADA68() * 50.0f) + -25.0f;
    sp2C.unk44 = 0.0f;
    sp2C.unk48 = (func_150ADA68() * 50.0f) + -25.0f;
    sp2C.unk54 = (func_150ADA20() % 68U) + 0x26;

    sp2C.unk4C = (func_150ADA68() * D_8009FE80) + D_8009FE84;
    sp2C.unk00 = 1.0f;

    temp_f2 = (func_150ADA68() * D_8009FE88) + D_8009FE8C;
    sp2C.unk56 = 0x1F;
    sp2C.unk58 = 0;
    sp2C.unk5C = 0;
    sp2C.unk08 = temp_f2;
    sp2C.unk60 = 0xFF;
    sp2C.unk61 = 1;
    sp2C.unk62 = 0;
    sp2C.unk63 = 0;
    sp2C.unk64 = 0;
    sp2C.unk65 = 0;
    sp2C.unk66 = 0;
    sp2C.unk67 = 0;
    sp2C.unk68 = 0;
    sp2C.unk6A = 0;
    sp2C.unk6C = 0;
    sp2C.unk70 = 0;
    sp2C.unk72 = 1;
    sp2C.unk74 = 0xFF;
    sp2C.unk0C = temp_f2;

    func_15132A4C(&sp2C.unk00, 3, 0xFF, 0, arg14, 0);
    return 1;
}
