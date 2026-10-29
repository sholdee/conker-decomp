#include <ultra64.h>
#include "functions.h"
#include "variables.h"



typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 unk56;
    /* 0x58 */ s8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ s8 unk68;
    /* 0x69 */ u8 unk69;
    /* 0x6A */ s8 unk6A;
    /* 0x6C */ void *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} Spawn150DECC0;

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ struct17 pos;
    /* 0x10 */ s16 unk10;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 velBase;
    /* 0x24 */ f32 velRange;
    /* 0x28 */ f32 velYBase;
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ struct17 vel;
    /* 0x3C */ f32 scaleBase;
    /* 0x40 */ f32 scaleRange;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s16 unk58;
} Params150DECC0;

extern struct17 D_800A0D60[15];
extern f32 D_800A0E14[15];
extern f32 D_800A0E50[15];
extern f32 D_800A0E8C[15];
extern f32 D_80088960[3];
extern f32 D_8008896C[3];

void *memcpy(void *, const void *, u32);
s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
void func_151D5514(struct17 *, u8, s32);
void func_151D3FF4(struct17 *, u8, s32);
void func_151541B8(struct17 *, f32, s32, f32, f32, u8, s32);
void func_151A6F00(void *, s32, u8, s32);

void func_150DECC0(struct17 *arg0, u8 arg1, s32 arg2) {
    s32 yaw;
    s32 i;
    Spawn150DECC0 spawn;
    u8 idx;
    Params150DECC0 params;

    yaw = (s32)(D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f);
    {
        extern f32 D_800A0EC8[15];

        spawn.unk00 = 15.0f;
        spawn.unk0C = 1.0f;
        spawn.unk08 = 1.0f;
        spawn.unk04 = 0.200000003f;
        spawn.unk10 = *(struct17 *)&D_800A5480;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk44 = 0.0f;
        spawn.unk50 = 0x29E9;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 0xD;
        spawn.unk62 = 0;
        spawn.unk63 = 0xA;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 2;
        spawn.unk6C = NULL;
        spawn.unk70 = 0;
        spawn.unk72 = 0x23;
        spawn.unk74 = 7;
        spawn.unk78 = 0;
        spawn.unk34.unk0 = 0.0f;
        spawn.unk34.unk8 = 0.0f;
        for (i = 0; i < 15; i++) {
            idx = D_800A0EC8[i];
            spawn.unk28.unk0 = D_800A0D60[i].unk0 + arg0->unk0;
            spawn.unk28.unk4 = D_800A0D60[i].unk4 + arg0->unk4;
            spawn.unk28.unk8 = D_800A0D60[i].unk8 + arg0->unk8;
            spawn.unk34.unk4 = D_80088960[idx] + func_150ADA68() * D_8008896C[idx];
            spawn.unk40 = D_800A0E8C[i] * D_800A0E50[i] * 0.01900000125f;
            spawn.unk48 = D_800A0E8C[i] * D_800A0E14[i] * 0.01900000125f;
            spawn.unk4C = func_150ADA68() * 0.2080000043f + -1.032000065f;
            spawn.unk54 = func_150ADA20() % 111U + 130;
            if (func_150ADA20() & 1) {
                spawn.unk56 = 0xC4;
            } else {
                spawn.unk56 = 0xC5;
            }
            func_1513264C(&spawn, 3, 0xFF, 0, 0, *(u8 *)&arg1, arg2);
        }
    }
    func_15165F80(-1, (s32)arg0->unk0, (s32)arg0->unk4, (s32)arg0->unk8, 0x1E, 0x32, 0, *(u8 *)&arg1, arg2);
    func_151D5404(arg0, 506.0f, 1013.0f, 0.0009871668881f, 0xF, 0x14, *(u8 *)&arg1, arg2);
    func_151D5334(arg0, 506.0f, 1013.0f, 0.0009871668881f, 5, *(u8 *)&arg1, arg2);
    func_151D5514(arg0, *(u8 *)&arg1, arg2);
    func_151D3FF4(arg0, *(u8 *)&arg1, arg2);
    func_151541B8(arg0, func_150ADA68() * 4.0f + 12.0f, 0x3FD20C49,
                  (f32)(func_150ADA20() % 56U + 200), 0.0f, *(u8 *)&arg1, arg2);
    params.count = (func_150ADA20() & 3) + 3;
    params.unk10 = 0xBE;
    params.yawBase = yaw - 0x40;
    params.yawRange = 0x80;
    params.pitchBase = -0x3F;
    params.pitchRange = 0x5A;
    params.speed = 250.0f;
    params.pos = *arg0;
    params.velBase = 5.0f;
    params.velYBase = 15.0f;
    params.velYRange = 15.0f;
    params.vel.unk0 = 0.0f;
    params.vel.unk4 = 0.0f;
    params.unk54 = 1;
    params.unk55 = 1;
    params.unk56 = 0x64;
    params.unk58 = 0x32;
    params.velRange = 10.0f;
    params.unk50 = 10.0f;
    params.vel.unk8 = 0.0f;
    params.scaleBase = 0.25f;
    params.scaleRange = 0.5f;
    params.unk44 = -0.6999999881f;
    params.unk48 = -0.6200000048f;
    params.unk4C = -5.0f;
    func_151A6F00(&params, 0, *(u8 *)&arg1, arg2);
    {
        struct260 *ret;
        struct17 pos;

        pos = *arg0;
        ret = func_15149130(0xF, 8, -1, -1, 1, 0, (struct37 *)0xC, *(u8 *)&arg1, arg2);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x28, &pos, sizeof(pos));
        }
    }
}

typedef struct {
    u8 pad0[0x24];
} Floor150DF334;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    struct17 unk28;
} Actor150DF334;

s32 func_1504697C(struct17 *, s32, f32, Floor150DF334 *);
void func_150E83AC(struct17 *, s16, u8, s32);
void func_151A9834(struct17 *, f32, f32, Floor150DF334 *, s32, s32, s32, s32, s32);

void func_150DF334(Actor150DF334 *arg0) {
    struct17 *pos;
    Floor150DF334 floor;
    u8 onGround;
    struct17 sp60;
    struct17 sp54;
    struct17 sp48;

    pos = &arg0->unk28;
    onGround = 0;
    sp54.unk0 = arg0->unk28.unk0;
    sp54.unk4 = arg0->unk28.unk4 + 500.0f;
    sp54.unk8 = arg0->unk28.unk8;
    if (func_1504697C(&sp54, 0, sp54.unk4 - 1000.0f, &floor) != 0) {
        sp60.unk0 = pos->unk0;
        sp60.unk4 = *(f32 *)&floor;
        sp60.unk8 = pos->unk8;
        onGround = 1;
    }
    func_150E83AC(onGround ? &sp60 : pos, (s16)(func_150ADA20() % 62U + 120), arg0->unkC, arg0->unk1);
    pos = onGround ? &sp60 : &arg0->unk28;
    sp48 = *pos;
    sp48.unk4 += 500.0f;
    func_151A9834(&sp48, sp48.unk4 - 1000.0f, 250.0f, &floor, func_150ADA20() % 3U + 3, 1, 0, arg0->unkC, arg0->unk1);
}

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg;

s32 func_15149550(Struct15149550Arg *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);
extern void func_15143874(s16, f32, f32 *, f32 *);

void func_150DF4B8(struct17 *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    Struct15149550Arg sp88;
    f32 t;
    f32 s;
    s32 count;

    sp88.unk0 = *arg0;
    t = (func_150ADA68() * 0.5f) + 1.0f;
    s = (func_150ADA68() * 0.5f) + 1.0f;
    sp88.unkC = 22.6f * t;
    sp88.unk14 = 13.786f * t;
    sp88.unk20 = 0x6231; sp88.unk22 = 0x1A4D;
    sp88.unk10 = 57.969f * s;
    sp88.unk18 = 42.714f * s;
    sp88.unk1C = 118.763f * s;
    sp88.unk24 = (func_150ADA20() % 151U) + 150;
    sp88.unk26 = 0;
    sp88.unk27 = 0;
    sp88.unk28 = 0;
    sp88.unk29 = 0xFF;
    sp88.unk2A = 1;
    sp88.unk2C = (func_150ADA20() % 156U) + 100;
    sp88.unk2D = 0xFF;
    sp88.unk2E = 0;
    sp88.unk2F = 1;
    sp88.unk30 = 0;
    sp88.unk34 = 0xF;
    sp88.unk38 = 0.0f;
    sp88.unk3C = 10000.0f;
    sp88.unk40 = 0.0450000018f;
    sp88.unk44 = 0.699999988f;
    func_15149550(&sp88, 0xA, 0, 0, arg3, arg4);
    count = (func_150ADA20() % 3U) + 1;
    while (count > 0) {
        func_15143874(func_150ADA20() & 0xFF, 30.0f, &sp88.unk0.unk0, &sp88.unk0.unk8);
        sp88.unk0.unk0 += arg0->unk0;
        sp88.unk0.unk8 += arg0->unk8;
        t = (func_150ADA68() * 0.5f) + 0.75f;
        s = (func_150ADA68() * 0.5f) + 0.75f;
        sp88.unkC = 22.6f * t;
        sp88.unk14 = 13.786f * t;
        sp88.unk10 = 57.969f * s;
        sp88.unk18 = 42.714f * s;
        sp88.unk1C = 118.763f * s;
        sp88.unk24 = (func_150ADA20() % 151U) + 150;
        sp88.unk2C = (func_150ADA20() % 156U) + 100;
        sp88.unk2F = 0;
        func_15149550(&sp88, 0xA, 0, 0, arg3, arg4);
        count--;
    }
}
