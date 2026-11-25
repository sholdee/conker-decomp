/* functions.h declares func_150ADA20 as returning u8; the code here needs the s32 version. */
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

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 object;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 active;
    /* 0x20 */ s32 unk20;
} Ground_150BABE0;

/* Spawn descriptor passed to func_1513C73C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
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
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21[7];
} Particle_150BABE0;

/* matching: the joint owner is passed as s32; a pointer parameter changes register allocation */
void func_15142314(s32, s32, Vec3F_150BABE0 *);
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);
void func_1504715C(Ground_150BABE0 *, struct127 *);
s32 func_15046C80(Vec3F_150BABE0 *, s32, f32, Ground_150BABE0 *);
s32 func_1518ABD0(s32, struct210 *, u8);

/*
 * Spawns the effects for one of obj's joints (side 0: joint 0x11, side 1: joint 0x1A): a sound,
 * two particle bursts and, when the ground below is found, a ground decal.
 */
// matching: each `*(u8 *)&arg2` reloads arg2 from its stack slot; plain or K&R arg2 is kept in a register
void func_150BABE0(struct127 *obj, u8 side, u8 arg2) {
    Vec3F_150BABE0 pos;
    s32 joint;
    s32 yaw;
    Ground_150BABE0 ground;
    Vec3F_150BABE0 probe;
    Particle_150BABE0 part;
    struct210 *decal;
    f32 size;

    /* camera angle in degrees -> 256-step angle */
    yaw = D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f;
    if (obj == NULL) {
        return;
    }
    if (obj->interaction_state == 0 || obj->interaction_state == 8) {
        return;
    }
    if (obj->unk1D4 == NULL) {
        return;
    }
    switch (side) {
    case 0:
        joint = 0x11;
        break;
    case 1:
        joint = 0x1A;
        break;
    }
    func_15142314((s32)obj->unk1D4, joint, &pos);
    pos.y = obj->unk180;
    func_15143E94(5, 0x4022);
    func_15165F80(-1, pos.x, pos.y + 6.0f, pos.z, 25, 18, 0, *(u8 *)&arg2, 1);
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 50.0f + 40.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 11U + 30, 5, 0, 0.0f, 0, *(u8 *)&arg2);
    func_1504715C(&ground, obj);
    probe.x = pos.x;
    probe.y = pos.y + 100.0f;
    probe.z = pos.z;
    if (func_15046C80(&probe, 0, pos.y - 100.0f, &ground) != 0) {
        size = func_150ADA68() * 200.0f + 200.0f;
        part.unk6 = 0x2A;
        part.unk7 = 0;
        part.unk0 = 0x7340;
        part.unk4 = 300;
        part.unk8 = 0;
        part.unkC = func_150ADA20() % 2001U + 2000;
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
        decal = func_1513C73C((s32)&part, 0, 0, (s32)ground.points, pos.x, ground.height, pos.z, size, size,
                              func_150ADA20() & 0xFF, (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), 4, *(u8 *)&arg2, 0);
        if (decal != NULL) {
            if (func_1518ABD0(D_80088750, decal, 1) == 0) {
                func_1516972C((struct102 *)decal);
            }
        }
    }
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 40.0f + 60.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 7U + 6, 6, 0, 0.0f, 0, *(u8 *)&arg2);
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
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    u8 pad64[4];
} Effect150BAFEC;
extern f32 D_8009FE74;
extern f32 D_8009FE78;
extern void *func_15130374(void *, u8, s32, u8, s32);

s32 func_150BAFEC(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 speed;
    void *ret;
    Effect150BAFEC fx;
    s32 pad2;
    f32 val;
    s16 ang;
    f32 c1;
    f32 s1;
    f32 c2;
    f32 s2;

    c1 = func_151423D8(arg8 - 0x40);
    s1 = func_151423D8(arg8);
    fx.unk1D = 0x29;
    fx.unk08 = 0xE03;
    fx.unk00 = 0x200005;
    fx.unk04 = 0;
    fx.unk0A = (func_150ADA20() % 36U) + 0x46;
    fx.unk0C = 0;
    fx.unk10 = 0;
    fx.unk18 = 0xB0;
    fx.unk19 = 0xA0;
    fx.unk1A = 0x2A;
    fx.unk14 = 0x40;
    fx.unk15 = 0xB;
    fx.unk16 = 0x6A;
    fx.unk17 = 0xFF;
    fx.unk1B = (func_150ADA20() % 52U) + 0x5A;
    fx.unk1C = 0xFF;
    fx.unk60 = 3;
    fx.unk61 = 3;
    fx.unk28 = fx.unk2C = (func_150ADA68() * 165.0f) + 800.0f;
    fx.unk30[0] = arg2;
    fx.unk30[1] = arg3;
    fx.unk30[2] = arg4;
    ang = (func_150ADA20() % 11U) - 0xC;
    c2 = func_151423D8(ang - 0x40);
    s2 = func_151423D8(ang);
    speed = (func_150ADA68() * 11.0f) + 15.0f;
    fx.unk58 = 0xE05;
    fx.unk48 = speed * s2 * c1;
    fx.unk4C = -speed * c2;
    fx.unk50 = speed * s2 * s1;
    fx.unk54 = 0.0f;
    if (func_150ADA20() & 1) {
        fx.unk58 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        fx.unk58 |= 0x80;
    }
    fx.unk62 = 2;
    fx.unk63 = -1;
    fx.unk1E = 0x16;
    fx.unk20 = 0xB;
    fx.unk22 = 1;
    fx.unk24 = D_8009FE74;
    val = D_8009FE78;
    ret = func_15130374(&fx, 0, 4, argE, 1);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &val, 4);
    }
    return 1;
}

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
