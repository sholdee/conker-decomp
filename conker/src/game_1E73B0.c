#include <ultra64.h>
#define func_1513D2F0 func_1513D2F0_hdr
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);

s32 func_1513FAB4(void *arg0, s32 arg1, f32 *arg2, s16 arg3);
s32 func_151BB0E0();
extern f32 D_800AA8E0;
extern f32 D_800AA8E8;
extern f32 D_800AA8EC;
extern f32 D_800AA8F0;
extern f32 D_800AA8F4;
extern f32 D_800AA8F8;
extern f32 D_800AA8FC;
extern f32 D_800AA900;
extern f32 D_800AA904;
extern f32 D_800AA908;
extern f32 D_800AA90C;
void func_15150F90(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15153634(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_151602C0(Header *, Header2 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);


typedef struct {
    u8 unk0;
    s8 unk1;
    u8 unk2;
    u8 pad3;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[0xA];
    s32 unk30;
    s32 unk34;
    u8 unk38;
    u8 unk39;
} Spawn151B9F00;

typedef struct {
    struct127 *owner;
    u8 ownerId;
} Extra151B9F00;

typedef struct {
    u8 pad0[0x50];
    s32 unk50; /* offset of the spawned object's extra block, from 0xF8 */
} Obj151B9F00;

extern s8 D_800E0BCE[];
extern s8 D_8008FD90;
void *func_1513B5E0(void *, s32, s32, u8, s32);

void *func_151B9F00(struct127 *arg0, u8 arg1, s32 arg2) {
    void *obj;
    Spawn151B9F00 desc;
    Extra151B9F00 extra;

    if (arg0 == NULL) {
        return NULL;
    }
    extra.owner = arg0;
    extra.ownerId = ((u8 *)arg0)[0x3B];
    desc.unk0 = 2;
    desc.unk1 = -1;
    desc.unk2 = 3;
    desc.unk4 = 0x12C;
    desc.unk30 = 9;
    desc.unk34 = 0x1AD;
    desc.unk39 = 0xFF;
    {
        s32 idx = arg0 - D_800CC2D0;

        if ((idx < 2) && (D_800E0BCE[idx] != 0)) {
            if (D_8008FD90 == 2) {
                desc.unk39 = 0xFD;
            } else if (D_8008FD90 == 4) {
                if (idx == 0) {
                    desc.unk39 = 0xFB;
                } else {
                    desc.unk39 = 0xF7;
                }
            }
        }
    }
    desc.unk8 = 1;
    desc.unkC = 0x200205;
    desc.unk10 = 0x60600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.unk38 = 2;
    obj = func_1513B5E0(&desc, 1, 8, arg1, arg2);
    if (obj != NULL) {
        memcpy((u8 *)obj + ((Obj151B9F00 *)obj)->unk50 + 0xF8, &extra, 8);
    }
    return obj;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1E73B0/func_151BA084.s")

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BA468;

typedef struct {
    u8  pad[0x3B];
    u8  unk3B;
} SomeObj_151BA468;

typedef struct {
    u8  pad[0x50];
    s32 unk50;
} Base_151BA468;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151BA468;

void func_151BA468(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151BA468 *temp_v0 = (SubA_151BA468 *)((u8 *)arg0 + ((Base_151BA468 *)arg0)->unk50 + 0xF8);
    ArgB_151BA468 *b = (ArgB_151BA468 *)arg1;
    s32 b0 = temp_v0->unk0;
    SomeObj_151BA468 *o = (SomeObj_151BA468 *)b0;

    if (arg2 == 0) {
        if ((b0 == b->unk0) || (b->u4.b4 == o->unk3B)) {
            func_1516972C((struct102 *)arg0);
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

typedef struct { f32 x, y, z; } V3_151BA518;
typedef struct {
    s16 a0, a2, a4, a6;
    s32 a8, aC;
    f32 f10, f14, f18;
    f32 f1c, f20, f24, f28, f2c, f30;
    s32 i34, i38;
    f32 f3c, f40, f44, f48;
    s16 s4c[6];
    s8 b58;
} Cfg151BA518;
typedef struct {
    f32 f0, f4, f8, fC;
    V3_151BA518 pos;
    f32 f1c, f20, f24;
    V3_151BA518 t;
    V3_151BA518 vel;
    f32 f40, f44, f48, f4c;
    s32 i50;
    s16 s54, s56;
    s8 b58;
    s32 i5c;
    u8 b60, b61, b62, b63, b64, b65, b66, b67, b68, b69;
    s8 b6a;
    void *p6c;
    u8 b70;
    s16 s72, s74;
    s32 pad78, pad7c;
} Part151BA518;
extern V3_151BA518 D_800AA590, D_800AA59C, D_800AA5A8, D_800AA5B4[], D_800AA5CC[], D_800AA5FC[];
extern f32 D_800AA62C[];
extern V3_151BA518 D_800AA63C, D_800AA648[], D_800AA660[], D_800AA678[], D_800AA6A8[], D_800AA6D8[];
extern f32 D_800AA708[];
extern f32 D_800AA7BC, D_800AA7C0, D_800AA7C4, D_800AA7C8, D_800AA7CC, D_800AA7D0, D_800AA7D4, D_800AA7D8, D_800AA7DC;
void func_1514FCE8(s16 *, u8, s32);
extern void func_1504715C(void *, void *);
extern void func_1503F404(s32, f32 *, f32 *, f32 *, V3_151BA518 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern void func_1513264C(Part151BA518 *, s32, s32, void *, s32, u8, s32);
extern void func_15143134(V3_151BA518 *, V3_151BA518 *, s32);

void func_151BA518(void *arg0, u8 arg1, s32 arg2) {
    Cfg151BA518 cfg;
    u8 sp168[0x24];
    V3_151BA518 sp15C;
    Part151BA518 p;
    f32 spD8, spD4, spD0, spCC, spC8, spC4;
    s32 s3;
    s32 i1, i2, i3;
    s32 v0;
    f32 f20, f22, f28;

    cfg.a0 = 0;
    cfg.a2 = 0xFF;
    cfg.a4 = -0x3F;
    cfg.a6 = 0x34;
    cfg.a8 = 6;
    if (D_80082FA0 >= 2) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    cfg.aC = 5 >> v0;
    cfg.f10 = ((f32 *)arg0)[5];
    cfg.f14 = ((f32 *)arg0)[6] + 25.0f;
    cfg.f18 = ((f32 *)arg0)[7];
    cfg.i34 = 6;
    cfg.f1c = 50.0f;
    cfg.f20 = 42.0f;
    cfg.f24 = 98.0f;
    cfg.f28 = 68.0f;
    cfg.f2c = 609.0f;
    cfg.f30 = 271.0f;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        v0 = 1;
    } else {
        v0 = 0;
    }
    cfg.i38 = 3 >> v0;
    cfg.s4c[0] = 0x19;
    cfg.s4c[1] = 0xF;
    cfg.s4c[2] = 0x64;
    cfg.s4c[3] = 0x64;
    cfg.s4c[4] = 0xC;
    cfg.s4c[5] = 0x14;
    cfg.f3c = 18.0f;
    cfg.f40 = 0.7690000534f;
    cfg.f44 = -0.1690000147f;
    cfg.f48 = 0;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        cfg.b58 = -1;
    } else {
        cfg.b58 = 0;
    }
    func_1514FCE8((s16 *)&cfg, arg1, arg2);
    if ((*(s32 *)((u8 *)arg0 + 0x1D4) != 0) && ((*((u8 *)arg0 + 0x74) & 0xF) != 0xF)) {
        func_1504715C(sp168, arg0);
        s3 = *(s32 *)((u8 *)arg0 + 0x1D4) + 0x40;
        {
            typedef s32 T151;
            func_1503F404(s3, &spD0, &spD4, &spD8, &sp15C.x, &sp15C.y, &sp15C.z, &spC4, &spC8, &spCC);
        }
        {
            typedef s32 T152;
            p.f0 = 1.0f;
            p.f1c = 1.0f;
            p.f20 = 1.0f;
            p.f24 = 1.0f;
            p.f4 = 0.6999999881f;
            p.f44 = 0.0f;
            p.i50 = 0x39E9;
            p.b58 = 0;
            p.i5c = 0;
            p.b60 = 0xFF;
            p.b61 = 0xD;
            p.b62 = 0;
            p.b63 = 0xC;
            p.b64 = 0;
            p.b65 = 0;
            p.b66 = 0;
            p.b67 = 0;
            p.b68 = 2;
            p.b6a = 0;
            p.p6c = arg0;
            p.b70 = *((u8 *)arg0 + 0x3B);
            p.s72 = 0xC;
            p.s74 = 0x15;
        }
        f22 = 0.001000000047f;
        f28 = -488.0f * f22;
        f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
        p.s54 = (func_150ADA20() % 36U) + 0x1E;
        p.s56 = 0x27;
        p.f8 = 0.75f;
        p.fC = 0.75f;
        func_150ADA68();
        p.f4c = f28;
        func_15143134(&D_800AA590, &p.t, s3);
        func_15143134(&D_800AA59C, &p.vel, s3);
        p.vel.x = p.vel.x - p.t.x;
        p.vel.y = p.vel.y - p.t.y;
        p.vel.z = p.vel.z - p.t.z;
        p.vel.x = p.vel.x * f20;
        p.vel.y = p.vel.y * f20;
        p.vel.z = p.vel.z * f20;
        p.pos = sp15C;
        p.pos.x += 109.0f;
        p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);

        f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
        p.s54 = (func_150ADA20() % 36U) + 0x1E;
        p.s56 = 0x28;
        p.f8 = 0.5f;
        p.fC = 0.5f;
        func_150ADA68();
        p.f4c = f28;
        func_15143134(&D_800AA5A8, &p.t, s3);
        func_15143134(&D_800AA5B4[func_150ADA20() & 1], &p.vel, s3);
        p.vel.x = p.vel.x - p.t.x;
        p.vel.y = p.vel.y - p.t.y;
        p.vel.z = p.vel.z - p.t.z;
        p.vel.x = p.vel.x * f20;
        p.vel.y = p.vel.y * f20;
        p.vel.z = p.vel.z * f20;
        p.pos = sp15C;
        p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
        func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);

        p.s56 = 0x29;
        for (i1 = 0; i1 != 4; i1++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                p.f8 = p.fC = D_800AA62C[i1];
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA5CC[i1], &p.t, s3);
                func_15143134(&D_800AA5FC[i1], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
        p.s56 = 0x2B;
        func_15143134(&D_800AA63C, &p.t, s3);
        p.f8 = p.fC = 0.550999999f;
        for (i2 = 0; i2 != 2; i2++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA648[i2], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.pos.x += D_800AA660[i2].x;
                p.pos.y += D_800AA660[i2].y;
                p.pos.z += D_800AA660[i2].z;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
        p.s56 = 0x2C;
        for (i3 = 0; i3 != 4; i3++) {
            if (func_150ADA20() & 1) {
                f20 = ((func_150ADA68() * 200.0f) + 83.0f) * f22;
                p.s54 = (func_150ADA20() % 36U) + 0x1E;
                p.f8 = p.fC = D_800AA708[i3];
                func_150ADA68();
                p.f4c = f28;
                func_15143134(&D_800AA678[i3], &p.t, s3);
                func_15143134(&D_800AA6A8[i3], &p.vel, s3);
                p.vel.x = p.vel.x - p.t.x;
                p.vel.y = p.vel.y - p.t.y;
                p.vel.z = p.vel.z - p.t.z;
                p.vel.x = p.vel.x * f20;
                p.vel.y = p.vel.y * f20;
                p.vel.z = p.vel.z * f20;
                p.pos = sp15C;
                p.pos.x += D_800AA6D8[i3].x;
                p.pos.y += D_800AA6D8[i3].y;
                p.pos.z += D_800AA6D8[i3].z;
                p.f40 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                p.f48 = ((func_150ADA68() * 28864.0f) + -14432.0f) * f22;
                func_1513264C(&p, 3, 0xFF, sp168, 0, arg1, arg2);
            }
        }
    }
}


void func_151BB044(struct127 *arg0) {
    func_1000FA64(0x4A7, (s16)(s32)arg0->x_position,
                  (s16)(s32)arg0->y_position,
                  (s16)(s32)arg0->z_position, 0x2000, 0x320, 0xC8,
                  (s32)func_151BB0E0, arg0, 0, 8, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E73B0/func_151BB0E0.s")

typedef struct { f32 x, y, z; } Vec3_151BB61C;
typedef struct { s16 v[9]; } Surf_151BB61C;
typedef struct {
    Vec3_151BB61C dir;      /* 0x00 */
    Surf_151BB61C surf;     /* 0x0C */
    u8 pad1E[2];
    f32 unk20;
    s32 unk24;
    s32 unk28;
    Vec3_151BB61C pos;      /* 0x2C */
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s16 unk72;
    s8 unk74;
} Fbfc_151BB61C;
typedef struct {
    s32 count;
    s32 spread;
    Vec3_151BB61C pos;      /* 0x08 */
    Vec3_151BB61C vel;      /* 0x14 */
    Surf_151BB61C surf;     /* 0x20 */
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
} Desc_151BB61C;

typedef struct { s32 v[9]; } Colors_151BB61C;
typedef struct { f32 v[9]; } Scales_151BB61C;
extern Colors_151BB61C D_800AA718;
extern Scales_151BB61C D_800AA73C;
void func_15055A2C(s32, f32, f32, f32, s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
s32 func_15145C90(s32);
s32 func_15144E80(void *, Vec3_151BB61C *, Vec3_151BB61C *, Vec3_151BB61C *);
void func_1514FBFC(void *, u8, s32);
void func_15151D6C(void *, s32 *, f32 *, s32, u8, s32);
void func_151C04F8(f32 *, u8, s32);
void func_151C05A4(void *, u8, s32);
void func_151C05F0(void *, u8, s32);
void func_151C0644(void *, u8, s32);
void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2);
void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_151BB61C(s32 arg0, Vec3_151BB61C *arg1, Vec3_151BB61C *arg2, u8 arg3, s32 arg4) {
    Surf_151BB61C surf;
    Vec3_151BB61C hit;
    Vec3_151BB61C delta;
    s32 sp1A0;
    Vec3_151BB61C sp194;
    Vec3_151BB61C sp188;
    Vec3_151BB61C sp17C;
    Fbfc_151BB61C fx;

    if (arg0 != 0) {
        func_15055A2C(0, arg1->x, arg1->y, arg1->z, 0);
        delta.x = arg2->x - arg1->x;
        delta.y = arg2->y - arg1->y;
        delta.z = arg2->z - arg1->z;
        func_151C05A4(arg1, arg3, arg4);
        func_151C05F0(arg1, arg3, arg4);
        func_151C04F8((f32 *)arg1, arg3, arg4);
        func_151BBEE4((f32 *)arg1, arg3, arg4);
        if (func_150AC9C0(arg1->x, arg1->y, arg1->z, delta.x, delta.y, delta.z, 0, &surf, &hit.x, &hit.y, &hit.z, 0, &sp1A0, 0, 0.0f) != 0) {
            if (func_15145C90(sp1A0) != 0) {
                func_151BBFBC((s32)&surf, (s32)&hit, arg3 & 0xFF, arg4);
            }
            if (func_15144E80(&surf, &sp194, &sp188, &sp17C) != 0 && sp17C.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
            fx.dir.x = -delta.x;
            fx.dir.y = -delta.y;
            fx.dir.z = -delta.z;
            fx.surf = surf;
            fx.unk20 = 494.0f;
            fx.pos = hit;
            fx.unk24 = 3;
            fx.unk28 = 4;
            fx.unk38 = 0.06089999899f;
            fx.unk3C = 0.02520000003f;
            fx.unk40 = 45.0f;
            fx.unk44 = 53.0f;
            fx.unk48 = 203.0f;
            fx.unk4C = 414.0f;
            fx.unk50 = 7;
            fx.unk54 = 3;
            fx.unk58 = 15.0f;
            fx.unk5C = 0.8800000548f;
            fx.unk60 = -0.3980000317f;
            fx.unk64 = 0.3910000324f;
            fx.unk68 = 25;
            fx.unk6A = 15;
            fx.unk6C = 100;
            fx.unk6E = 100;
            fx.unk70 = 12;
            fx.unk72 = 20;
            fx.unk74 = 0;
            func_1514FBFC(&fx, arg3, arg4);
            {
            Colors_151BB61C colors;
            Scales_151BB61C scales;
            Desc_151BB61C desc;
            colors = D_800AA718;
            scales = D_800AA73C;
            desc.count = 6;
            desc.spread = 6;
            desc.pos = hit;
            desc.vel.x = -delta.x;
            desc.vel.y = -delta.y;
            desc.vel.z = -delta.z;
            desc.surf = surf;
            desc.unk34 = 456.0f;
            desc.unk38 = 0.02000000142f;
            desc.unk3C = 0.02000000142f;
            desc.unk40 = -1.710000038f;
            desc.unk44 = 0.4480000138f;
            desc.unk48 = 50;
            desc.unk4A = 20;
            desc.unk4C = 0.4990000129f;
            desc.unk50 = 0.7010000348f;
            desc.unk54 = 21.50200081f;
            func_15151D6C(&desc, colors.v, scales.v, 9, arg3, arg4);
            }
        }
    }
}

typedef struct Vec3i_151BBA9C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BBA9C;

typedef struct Effect_151BBA9C {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    s32 field08;
    s32 field0C;
    Vec3i_151BBA9C position;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    s32 field34;
    s32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    s16 field54;
    s16 field56;
    s8 field58;
    u8 pad59[3];
} Effect_151BBA9C;

typedef struct Config_151BBA9C {
    s32 countBase, countRange;
    Vec3_151BB61C position;
    s16 yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange, durationBase, durationRange;
    s16 lifeBase, lifeRange;
    f32 sizeBase, sizeRange, jitter;
} Config_151BBA9C;

void func_1514FCE8(s16 *, u8, s32);
void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
s32 func_15046C80(f32 *, u16, f32, void *);
void func_1504715C(void *, void *);
extern Vec3_151BB61C D_8008FBA0;
extern Vec3_151BB61C D_8008FBAC;
extern f32 D_800AA80C;
extern f32 D_800AA810;
extern f32 D_800AA814;
extern f32 D_800AA818;
extern f32 D_800AA81C;
extern f32 D_800AA820;
extern f32 D_800AA824;
extern f32 D_800AA828;

void func_151BBA9C(s32 arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4) {
    Vec3_151BB61C center;
    Effect_151BBA9C effect;
    f32 contact[8];
    f32 ground;
    Vec3_151BB61C probe;
    Vec3_151BB61C hit;
    Vec3_151BB61C normal;
    Vec3_151BB61C tangent;
    Vec3_151BB61C facing;

    if (arg0 != 0) {
        center.x = (arg2[0] + arg1[0]) * 0.5f;
        center.y = (arg2[1] + arg1[1]) * 0.5f;
        center.z = (arg2[2] + arg1[2]) * 0.5f;
        func_151C04F8(&center.x, arg3, arg4);
        func_151C05A4(&center.x, arg3, arg4);
        func_151C05F0(&center.x, arg3, arg4);
        func_15055A2C(1, center.x, center.y, center.z, 0);
        func_151BBEE4(&center.x, arg3, arg4);
        effect.field00 = 0;
        effect.field02 = 0xFF;
        effect.field04 = -0x40;
        effect.field06 = 0x47;
        effect.field08 = 6;
        effect.field0C = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
        effect.position = *(Vec3i_151BBA9C *)&center;
        effect.field34 = 7;
        effect.field1C = 23.0f;
        effect.field20 = 30.0f;
        effect.field24 = 45.0f;
        effect.field28 = 53.0f;
        effect.field2C = 203.0f;
        effect.field30 = 414.0f;
        effect.field38 = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
        effect.field4C = 0x19;
        effect.field4E = 0xF;
        effect.field50 = 0x64;
        effect.field52 = 0x64;
        effect.field54 = 0xC;
        effect.field56 = 0x14;
        effect.field3C = 15.0f;
        effect.field40 = D_800AA80C;
        effect.field44 = D_800AA810;
        effect.field48 = D_800AA814;
        if (D_80082FA0 >= 2 || D_8008FD8C >= 8) {
            effect.field58 = -1;
        } else {
            effect.field58 = 0;
        }
        func_1514FCE8(&effect.field00, arg3, arg4);
        func_1504715C(&ground, (void *)arg0);
        probe.x = center.x;
        probe.y = center.y + 100.0f;
        probe.z = center.z;
        if (func_15046C80(&probe.x, 0, center.y - 1000.0f, &ground) != 0) {
            hit.x = probe.x;
            hit.y = ground;
            hit.z = probe.z;
            func_151BBFBC((s32)contact, (s32)&hit, arg3, arg4);
            if (func_15144E80(contact, &normal, &tangent, &facing) != 0 && facing.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
        }
        {
            Config_151BBA9C config;
            Vec3_151BB61C first = D_8008FBA0;
            Vec3_151BB61C second = D_8008FBAC;

            config.countBase = 4;
            config.countRange = 8;
            config.position = center;
            config.speedBase = 12.0f;
            config.speedRange = 6.0f;
            config.yawBase = 0;
            config.yawRange = 0xFF;
            config.pitchBase = -0x3F;
            config.pitchRange = 0x57;
            config.lifeBase = 0x32;
            config.lifeRange = 0xA;
            config.durationBase = D_800AA818;
            config.durationRange = D_800AA81C;
            config.sizeBase = D_800AA820;
            config.sizeRange = D_800AA824;
            config.jitter = D_800AA828;
            func_15152190(&config, &first, &second, 3, 0.0f, 1, arg3, arg4);
        }
    }
}

void func_151BBEE4(f32 *arg0, u8 arg1, s32 arg2) {
    Header header;
    Header2 header2;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() & 0xF) + 5;
    header.unk4 = 0;

    header2.unk0 = (s32) arg0[0];
    header2.unk4 = (s32) arg0[1];
    header2.unk8 = (s32) arg0[2];

    func_151602C0(&header, &header2, (func_150ADA20() % 0x15U) + 0x1E, 0xFF, 0x61, 0x36, 0xFF, 0, 0, arg1, arg2);
}

void func_150E7FEC(f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151BBFBC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    union { u32 w; f64 _a; } r1;
    f32 temp_f;
    u32 r2;

    temp_f = func_150ADA68();
    r1.w = func_150ADA20();
    r2 = func_150ADA20();
    func_150E7FEC(temp_f * 75.0f + 75.0f, ((r1.w % 0x38) + 0xC8) & 0xFF, arg0, arg1,
                  (r2 % 0xCD) + 0x12B, 1, 1, 0, 0, 0, (u8)arg2, 0);
}

extern f32 D_800AA82C;
extern u8 D_800AA760;
extern u8 D_800AA76C;
void func_15160CDC(s32, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151BC074(s32 arg0) {
    if (arg0 != 0) {
        func_15160CDC(arg0, 1, &D_800AA760, &D_800AA76C, D_800AA82C, 2, 0x12C,
                      0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
    }
}

extern f32 D_800AA830;
extern f32 D_800DCA24;
void *func_15134908(void *, s32, u8, s32);
void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void *func_15163414(Header *, f32 *, f32 *, f32 *, s8, u8, u8, u8, u8, u8, u8, u8, s32, u8, s32);

typedef struct {
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 pad1A[2];
} Spawn151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
} Extra151BC104;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec151BC104;

typedef struct {
    Vec151BC104 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
} Header151BC104;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    u8 pad5[3];
    Vec151BC104 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
} Payload151BC104;

void func_151BC104(struct127 *arg0, u8 arg1, s32 arg2) {
    void *temp_v0;

    if ((D_80082FA0 < 2) && (D_8008FD8C < 5)) {
        Spawn151BC104 spawn;
        Extra151BC104 extra;
        spawn.unkC = 15.0f;
        spawn.unk0 = &arg0->x_position;
        spawn.unk4 = &arg0->y_position;
        spawn.unk8 = &arg0->z_position;
        spawn.unk14 = 0x12C;
        spawn.unk10 = D_800AA830 * D_800DCA24;
        spawn.unk16 = 4;
        spawn.unk17 = 4;
        spawn.unk18 = 1;
        spawn.unk19 = 0;
        extra.owner = arg0;
        extra.ownerId = arg0->unique_id;
        temp_v0 = func_15134908(&spawn, 8, arg1, arg2);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x40, &extra, 8);
        }
    }
    {
        Header151BC104 header;
        void *temp_v0_2;
        Payload151BC104 payload;
        header.unk15 = 0x28;
        header.unk0.x = arg0->x_position;
        header.unk0.y = arg0->y_position;
        header.unk0.z = arg0->z_position;
        header.unkC = 0x12C;
        header.unkE = 6;
        header.unk10 = 0xC;
        payload.owner = arg0;
        payload.ownerId = arg0->unique_id;
        payload.unk8 = header.unk0;
        payload.unk14 = 0.0f;
        payload.unk18 = 0.0f;
        payload.unk20 = -16384.0f;
        payload.unk1C = -16384.0f;
        temp_v0_2 = func_15147A80(&header, 0x28, 0x14, 0xA, 0xA, 0xA, 0, 0, 0, arg1, arg2);
        if (temp_v0_2 != NULL) {
            memcpy(*(void **)((u8 *)temp_v0_2 + 0x98), &payload, 0x24);
        }
    }
    {
        Extra151BC104 extra2;
        void *temp_v0_3;
        Header header2;
        extra2.owner = arg0;
        extra2.ownerId = arg0->unique_id;
        header2.unk0 = 2;
        header2.unk1 = 0xE;
        header2.unk2 = 0x12C;
        header2.unk4 = 0x1F;
        temp_v0_3 = func_15163414(&header2, &arg0->x_position, &arg0->y_position, &arg0->z_position,
                                0, 0, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0, 8, arg1, arg2);
        if (temp_v0_3 != NULL) {
            memcpy((u8 *)temp_v0_3 + 0x28, &extra2, 8);
        }
    }
}

void func_15143794(s16, s16, f32, void *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32,
                    s32, s32, s32, s32, s32, s32, s32, s32);

void func_151BC370(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4,
                   f32 arg5, void *arg6) {
    f32 position[3];
    f32 velocity[3];
    f32 zero[3];
    f32 velocityScale;
    f32 randomScale1;
    u32 random1;
    u32 random0;
    f32 randomScale0;

    position[0] = arg0;
    position[1] = arg1;
    position[2] = arg2;
    random0 = func_150ADA20();
    random1 = func_150ADA20();
    randomScale0 = func_150ADA68();
    func_15143794((s16)(random0 & 0xFF), (s16)((random1 % 65U) - 0x20),
                   randomScale0 * 1680.0f * 0.01f, velocity);
    velocityScale = ((func_150ADA68() * 202.0f) + 208.0f) * 0.001f;
    zero[0] = 0.0f;
    zero[1] = 0.0f;
    zero[2] = 0.0f;
    velocity[0] += -arg3 * D_800BE9A8 * velocityScale;
    velocity[1] += -arg4 * D_800BE9A8 * velocityScale;
    velocity[2] += -arg5 * D_800BE9A8 * velocityScale;
    randomScale0 = func_150ADA68();
    randomScale1 = func_150ADA68();
    random0 = func_150ADA20();
    random1 = func_150ADA20();
    func_151A26EC(position, zero, velocity, 1.0f,
                   ((randomScale0 * 1049.0f) + -1022.0f) * 0.001f,
                   (randomScale1 * 124.0f) + 202.0f,
                   (random0 & 0xF) + 0x14, (random1 % 201U) + 0x37,
                   0xF, 0xF, 0, -1, 0xCB, 0, 0,
                   *(u8 *)((u8 *)arg6 + 0xC), *(u8 *)((u8 *)arg6 + 1));
}

s32 func_151BC580(s32 *arg0) {
    s32 *p = (s32 *)arg0[0x10];
    if (*p == 0) {
        return 0;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BC5A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151BC5A4;

void func_151BC5A4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151BC5A4 *temp_v0 = (SubA_151BC5A4 *)((u8 *)arg0 + 0x40);
    ArgB_151BC5A4 *b = (ArgB_151BC5A4 *)arg1;
    s32 b0 = temp_v0->unk0;

    if (arg2 == 0) {
        if ((b0 == b->unk0) || (temp_v0->unk4 == b->u4.b4)) {
            func_1516972C((struct102 *)arg0);
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

typedef struct Game1E9A30Trail {
    s32 x;
    s32 y;
    s32 z;
    s16 lifetime;
    u8 alpha;
    u8 pad_F[5];
} Game1E9A30Trail;

typedef struct Vec3i_151BC64C {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_151BC64C;

s32 func_151BC64C(u8 *actor) {
    Game1E9A30Trail *entries;
    Game1E9A30Trail *entry;
    s32 cursor;
    s16 lifetime;
    s32 opacity;

    entries = *(Game1E9A30Trail **)(actor + 0x94);
    if ((*(s8 *)(actor + 0x2C) < 2) && (*(u16 *)(actor + 0x1E) & 8)) {
        return 0;
    }
    cursor = *(s8 *)(actor + 0x2E);
    if (cursor != *(s8 *)(actor + 0x2D)) {
        do {
            cursor--;
            if (cursor < 0) {
                cursor = actor[0x25] - 1;
            }
            entry = &entries[cursor];
            entry->lifetime -= D_800BE9E4;
            lifetime = entry->lifetime;
            if (lifetime < 0) {
                if (cursor != *(s8 *)(actor + 0x2D)) {
                    do {
                        *(s8 *)(actor + 0x2D) += 1;
                        if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                            *(s8 *)(actor + 0x2D) = 0;
                        }
                        *(s8 *)(actor + 0x2C) -= 1;
                    } while (cursor != *(s8 *)(actor + 0x2D));
                }
            } else if (lifetime < 0xA) {
                opacity = lifetime * 0x19;
                if (opacity < entry->alpha) {
                    entry->alpha = opacity;
                }
            }
        } while (cursor != *(s8 *)(actor + 0x2D));
    }
    if (*(s8 *)(actor + 0x2C) > 0) {
        *(Vec3i_151BC64C *)(actor + 0x54) = *(Vec3i_151BC64C *)((u8 (*)[sizeof(Game1E9A30Trail)])entries)[*(s8 *)(actor + 0x2D)];
    } else {
        *(f32 *)(actor + 0x54) = 0.0f;
        *(f32 *)(actor + 0x58) = 0.0f;
        *(f32 *)(actor + 0x5C) = 0.0f;
    }
    return 1;
}

typedef struct Trail151BC794 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BC794;

typedef struct Src151BC794 {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Src151BC794;

typedef struct Data151BC794 {
    Src151BC794 *src;
    u8 unk4;
    u8 pad5[3];
    struct17 pos;
    f32 count;
    f32 unk18;
    f32 unk1C;
    f32 rot;
} Data151BC794;

typedef struct Actor151BC794 {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x65];
    Trail151BC794 *entries;
    Data151BC794 *data;
} Actor151BC794;

extern f32 func_15143E64(struct17 *);

s32 func_151BC794(Actor151BC794 *arg0) {
    Data151BC794 *data;
    Trail151BC794 *entries;
    Src151BC794 *src;
    struct17 diff;
    Trail151BC794 *entry;
    f32 dist;
    f32 inv;
    f32 rot0;

    data = arg0->data;
    entries = arg0->entries;
    src = data->src;
    if (src->unk0 == 0 || src->unk3B != data->unk4) {
        arg0->unk1E |= 8;
        return 1;
    }
    arg0->unk10 = src->unk14;
    arg0->unk14 = src->unk18;
    arg0->unk18 = src->unk1C;
    diff.unk0 = arg0->unk10 - data->pos.unk0;
    diff.unk4 = arg0->unk14 - data->pos.unk4;
    diff.unk8 = arg0->unk18 - data->pos.unk8;
    dist = func_15143E64(&diff);
    data->count += dist * 0.04870799929f * D_800BE9A4;
    data->unk1C += dist * 9.708835602f;
    if (data->count > 1.0f) {
        struct17 pos;
        f32 rot;
        f32 y;
        f32 yStep;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 delta;

        inv = 1.0f / data->count;
        y = data->unk18 + D_800BE9A4;
        pos = data->pos;
        rot0 = data->rot;
        rot = rot0;
        yStep = -(y * inv);
        dx = diff.unk0 * inv;
        dy = diff.unk4 * inv;
        dz = diff.unk8 * inv;
        delta = data->unk1C - rot0;
        do {
            entry = &entries[arg0->unk2E];
            entry->pos = pos;
            entry->lifetime = 12;
            entry->alpha = 100;
            entry->rot = rot;
            while (entry->rot > 16384.0f) {
                entry->rot -= 32768.0f;
            }
            arg0->unk2E++;
            if (arg0->unk2E == arg0->unk25) {
                arg0->unk2E = 0;
            }
            arg0->unk2C++;
            if (arg0->unk2E == arg0->unk2D) {
                arg0->unk2D++;
                if (arg0->unk2D == arg0->unk25) {
                    arg0->unk2D = 0;
                }
                arg0->unk2C--;
            }
            rot += delta * inv;
            pos.unk0 += dx;
            pos.unk4 += dy;
            pos.unk8 += dz;
            y += yStep;
            data->count -= 1.0f;
        } while (data->count > 1.0f);
        data->pos = pos;
        data->rot = rot;
        data->unk18 = y;
    }
    return 1;
}

typedef struct Trail151BCA90 {
    struct17 pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 rot;
} Trail151BCA90;

typedef struct Actor151BCA90 {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Trail151BCA90 *entries;
} Actor151BCA90;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_151BCA90(Actor151BCA90 *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    Trail151BCA90 *entries;
    struct17 *cam;
    u8 update;
    struct17 b;
    struct17 a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 cx;
    f32 cy;
    f32 cz;
    s32 i;
    s32 j;
    f32 len[1];
    u8 alphaA;
    f32 rotB;
    f32 rotA;
    u8 alphaB;
    f32 scale;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->buffers, arg2, (arg0->unk25 + 5) * 2 * sizeof(Vtx), (void **)&vtx, 0);
        if (vtx != NULL) {
            entries = arg0->entries;
            cam = func_15144B34(arg2);
            update = 1;
            gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &update, 3);
            gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
            gfx = func_1513F4E4(gfx, 0x4E, &update);
            gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
            gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
            if (arg0->unk1E & 2) {
                i = arg0->unk2E - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = arg0->pos;
                alphaA = 0x80;
                rotA = 0.0f;
            } else {
                j = arg0->unk2E - 1;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                i = j - 1;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                a = entries[j].pos;
                alphaA = entries[j].alpha;
                rotA = entries[j].rot;
            }
            b = entries[i].pos;
            alphaB = entries[i].alpha;
            rotB = entries[i].rot;

            ex = a.unk0 - b.unk0;
            ey = a.unk4 - b.unk4;
            ez = a.unk8 - b.unk8;
            dx = a.unk0 - cam->unk0;
            dy = a.unk4 - cam->unk4;
            dz = a.unk8 - cam->unk8;
            cx = (ey * dz) - (dy * ez);
            cy = (ez * dx) - (dz * ex);
            cz = (ex * dy) - (dx * ey);
            len[0] = (cx * cx) + (cy * cy) + (cz * cz);
            if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                cz = cy = cx = 0.0f;
            } else {
                scale = 35.0f / sqrtf(len[0]);
                cx = cx * scale;
                cy = cy * scale;
                cz = cz * scale;
            }
            vtx->v.ob[0] = a.unk0 + cx;
            vtx->v.ob[1] = a.unk4 + cy;
            vtx->v.ob[2] = a.unk8 + cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.unk0 - cx;
            vtx->v.ob[1] = a.unk4 - cy;
            vtx->v.ob[2] = a.unk8 - cz;
            vtx->v.tc[0] = rotA;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = alphaA;
            vtx->v.flag = 0;
            vtx++;
            do {
                j = i;
                ex = a.unk0 - b.unk0;
                ey = a.unk4 - b.unk4;
                ez = a.unk8 - b.unk8;
                dx = b.unk0 - cam->unk0;
                dy = b.unk4 - cam->unk4;
                dz = b.unk8 - cam->unk8;
                cx = (ey * dz) - (dy * ez);
                cy = (ez * dx) - (dz * ex);
                cz = (ex * dy) - (dx * ey);
                len[0] = (cx * cx) + (cy * cy) + (cz * cz);
                if ((cx * cx) + (cy * cy) + (cz * cz) == 0) {
                    cz = cy = cx = 0.0f;
                } else {
                    scale = 35.0f / sqrtf(len[0]);
                    cx = cx * scale;
                    cy = cy * scale;
                    cz = cz * scale;
                }
                vtx->v.ob[0] = b.unk0 + cx;
                vtx->v.ob[1] = b.unk4 + cy;
                vtx->v.ob[2] = b.unk8 + cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.unk0 - cx;
                vtx->v.ob[1] = b.unk4 - cy;
                vtx->v.ob[2] = b.unk8 - cz;
                vtx->v.tc[0] = rotB;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alphaB;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i--;
                if (i < 0) {
                    i = arg0->unk25 - 1;
                }
                if (j != arg0->unk2D) {
                    a = b;
                    b = entries[i].pos;
                    alphaB = entries[i].alpha;
                    rotB = entries[i].rot;
                }
            } while (j != arg0->unk2D);
        }
    }
    return gfx;
}

void func_151BD21C(void *arg0, void *arg1, u8 arg2) {
    void *target;
    s32 current;
    s32 value;

    target = *(void **)((u8 *)arg0 + 0x98);
    current = *(s32 *)target;
    if (arg2 == 0) {
        if (current == *(s32 *)arg1 || *(u8 *)((u8 *)target + 4) == *(u8 *)((u8 *)arg1 + 4)) {
            u16 *flags = (u16 *)((u8 *)arg0 + 0x1E);

            *(s8 *)((u8 *)arg0 + 0x30) = 0;
            *flags |= 8;
        }
    } else if (arg2 == 0x2D) {
        value = *(s32 *)arg1;
        if (value == *(s32 *)target) {
            *(s32 *)target = *(s32 *)((u8 *)arg1 + 4);
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 9);
            return;
        }
        if (*(s32 *)((u8 *)arg1 + 4) == *(s32 *)target) {
            *(s32 *)target = value;
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 8);
        }
    }
}

s32 func_151BD2BC(s32 *arg0) {
    s32 *p = (s32 *)arg0[0xA];
    if (*p == 0) {
        return 0;
    }
    if (*((u8 *)arg0 + 0x2C) != *((u8 *)p + 0x3B)) {
        return 0;
    }
    return 1;
}

extern f32 D_800AA850;
extern f32 D_800AA860;
extern f32 D_800AA864;

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25[0x1];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_E9FE0_2F8;

void *func_15134DAC(struct_E9FE0_2F8 *, s32);

void func_151BD2F8(void *arg0, s32 arg1, s32 arg2) {
    struct_E9FE0_2F8 sp24;
    struct { s16 unk0; u8 pad[2]; } sp20;
    void *temp_v0;

    if (arg0 == NULL) {
        return;
    }

    sp24.unk0 = *((u8 *)arg0 + 0x3B);
    sp24.unk4 = arg0;
    sp24.unk8 = 1;
    sp24.unkC = 31.0f;
    sp24.unk10 = -34.0f;
    sp24.unk14 = -117.0f;
    sp24.unk18 = 35.0f;
    sp24.unk1C = -24.0f;
    sp24.unk20 = -182.0f;
    sp24.unk24 = 0;
    sp24.unk26 = 0x32;
    sp24.unk28 = 0x64;
    sp24.unk2A = 0x12C;
    sp24.unk2E = 4;
    sp24.unk2F = 4;
    sp24.unk30 = 3;
    sp24.unk34 = D_800AA850;
    sp24.unk38 = 5;
    sp24.unk39 = -1;

    sp20.unk0 = 0;

    temp_v0 = func_15134DAC(&sp24, 2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp20, 2);
    }

    sp24.unkC = -sp24.unkC;
    sp24.unk18 = -sp24.unk18;

    temp_v0 = func_15134DAC(&sp24, 2);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp20, 2);
    }
}

s32 func_151BD42C(u8 *arg0) {
    *(s16 *)(arg0 + 0x80) = 0;
    return 1;
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
    f32 unk3C[3];
    f32 unk48[3];
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
} Fx151BD43C;

typedef struct {
    u8 pad0[0x14];
    f32 unk14[3];
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
} Owner151BD43C;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1C - 0xD];
    Owner151BD43C *unk1C;
    u8 pad20[0x74 - 0x20];
    f32 unk74;
} Actor151BD43C;

typedef struct {
    Owner151BD43C *owner;
    u8 ownerId;
} Extra151BD43C;

struct260 *func_15130374(void *, u8, s32, u8, s32);

void func_151BD43C(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3, f32 arg4, Actor151BD43C *arg5) {
    struct260 *ret;
    Owner151BD43C *owner;
    Fx151BD43C fx;
    f32 val;
    s32 pad;
    Extra151BD43C extra;
    s32 pad2;

    owner = arg5->unk1C;
    if ((owner->unk74 & 0xF) != 0xF) {
        fx.unk1D = 0x29;
        fx.unk08 = 0xE03;
        fx.unk00 = 0x200005;
        fx.unk04 = 0;
        fx.unk0C = 0;
        fx.unk10 = 0;
        fx.unk54 = ((func_150ADA68() * 251.0f) + 44.0f) * 0.001000000047f;
        fx.unk1E = 0x14;
        fx.unk20 = 0xC;
        fx.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (func_150ADA20() & 1 ? 0x80 : 0)) | 0x200 | 0x400 | 0x800 | 0x1000 | 0x4000 | 0x8000;
        fx.unk60 = 3;
        fx.unk61 = 3;
        fx.unk63 = -1;
        fx.unk64 = -1;
        fx.unk65 = 7;
        fx.unk22 = 0x22;
        fx.unk14 = 0xDD;
        fx.unk15 = 0xD3;
        fx.unk16 = 0xCD;
        fx.unk17 = 0xFF;
        fx.unk18 = 0x57;
        fx.unk19 = 0x55;
        fx.unk1A = 0x5A;
        val = 0.9347190261f;
        fx.unk24 = 1.034358025f;
        fx.unk1B = (func_150ADA20() % 156U) + 0x64;
        fx.unk1C = 0xFF;
        fx.unk0A = (func_150ADA20() % 18U) + 0x19;
        fx.unk28 = fx.unk2C = (func_150ADA68() * 53.0f) + 49.0f;
        fx.unk48[0] = (arg1[0] - arg0[0]) * arg5->unk74;
        fx.unk48[1] = (arg1[1] - arg0[1]) * arg5->unk74;
        fx.unk48[2] = (arg1[2] - arg0[2]) * arg5->unk74;
        fx.unk30[0] = arg0[0] - arg5->unk1C->unk14[0];
        fx.unk30[1] = arg0[1] - arg5->unk1C->unk14[1];
        fx.unk30[2] = arg0[2] - arg5->unk1C->unk14[2];
        fx.unk30[0] += fx.unk48[0] * arg4;
        fx.unk30[1] += fx.unk48[1] * arg4;
        fx.unk30[2] += fx.unk48[2] * arg4;
        extra.owner = arg5->unk1C;
        extra.ownerId = arg5->unk1C->unk3B;
        fx.unk62 = 0x13;
        fx.unk3C[0] = arg5->unk1C->unk14[0];
        fx.unk3C[1] = arg5->unk1C->unk14[1];
        fx.unk3C[2] = arg5->unk1C->unk14[2];
        ret = func_15130374(&fx, 1, 0x10, arg5->unkC, arg5->unk1);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0xA8, &val, 4);
            memcpy((u8 *)ret + 0xB0, &extra, 8);
        }
    }
}

f32 func_151BD750(s16 *arg0) {
    s16 temp;
    f32 ret;

    temp = arg0[0x40];
    ret = (1.0f + 1.0f) * (f32) temp * D_800AA860 + D_800AA864;
    arg0[0x40] = temp + D_800BE9E4;
    return ret;
}

extern void func_1513170C(void);

struct S151BD79C_sub {
    s32 unk0;
    u8 unk4;
    u8 pad5[0xF];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct S151BD79C {
    u8 pad0[0x4C];
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 padB0[0xB0 - 0x58];
    struct S151BD79C_sub *unkB0;
};

void func_151BD79C(struct S151BD79C *arg0) {
    struct S151BD79C_sub *v0 = arg0->unkB0;
    if (v0->unk0 != 0 && v0->unk4 != 0xFF) {
        arg0->unk4C = v0->unk14;
        arg0->unk50 = v0->unk18;
        arg0->unk54 = v0->unk1C;
    }
    func_1513170C();
}

void func_151BD7F4(s32 arg0) {
    s32 sp18[2];
    sp18[0] = arg0;
    *((u8 *)&sp18 + 4) = *((u8 *)arg0 + 0x3B);
    func_151494E0((s32)&sp18, 0x3B);
}

s32 func_151407D0(void *, u32, void *, u8, u8, u8, u8, s8, u8, s32);

typedef struct Game1E9A30BurstData {
    void *owner;
    u8 id;
    u8 pad05[3];
    void *sparks[2];
    void *glows[2];
} Game1E9A30BurstData;

typedef struct Game1E9A30BurstSpark {
    void *owner;
    u8 id;
    u8 index;
    u8 pad06[2];
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    f32 field44;
    f32 field48;
    f32 field4C;
    void *object;
    f32 field54;
    s8 field58;
    u8 pad59[3];
} Game1E9A30BurstSpark;

typedef struct Game1E9A30BurstEffect {
    u8 kind;
    u8 mode;
    s16 flags;
    s16 lifetime;
    u8 pad06[2];
    s32 field08;
    s32 field0C;
    u8 color[4];
    f32 field14;
    f32 field18;
    struct17 field1C;
    struct17 field28;
    f32 field34;
    f32 field38;
    f32 field3C;
    s32 field40;
    u8 field44;
    u8 field45;
    u8 field46;
    u8 field47;
    s32 field48;
    u8 field4C;
    u8 pad4D[0xB];
} Game1E9A30BurstEffect;

typedef struct Game1E9A30BurstGlowShape {
    f32 field00;
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    s32 field24;
    s32 field28;
    s32 field2C;
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3C;
    s32 field40;
    s32 field44;
    f32 field48;
    s16 field4C;
    s16 field4E;
    s16 field50;
    s16 field52;
    u8 field54;
    u8 field55;
    u8 field56;
    u8 field57;
    u8 field58;
    s8 field59;
    u8 pad5A[2];
} Game1E9A30BurstGlowShape;

typedef struct Game1E9A30BurstGlow {
    s16 field00;
    u8 pad02[2];
    f32 field04;
    void *object;
    u8 index;
    u8 pad0D[3];
    void *owner;
    u8 id;
    u8 pad15[3];
} Game1E9A30BurstGlow;

void func_151BD828(u8 *arg0, u8 arg1, s32 arg2) {
    void *object;
    Game1E9A30BurstData burst;
    Game1E9A30BurstData *data;

    burst.owner = arg0;
    burst.id = arg0[0x3B];
    burst.sparks[0] = 0;
    burst.sparks[1] = 0;
    burst.glows[0] = 0;
    burst.glows[1] = 0;
    object = func_15149130(0x12C, -1, 0x40, -1, 0, 0x33, (struct37 *)0x18, arg1, arg2);
    if (object != NULL) {
        data = (Game1E9A30BurstData *)((u8 *)object + 0x28);
        memcpy(data, &burst, sizeof(burst));
        {
            Game1E9A30BurstSpark spark;
            Game1E9A30BurstEffect sparkEffect;

            spark.field58 = 0;
            spark.field54 = (*(f32 *)(arg0 + 0x150) + *(f32 *)(arg0 + 0x14C)) * 0.5f;
            spark.owner = arg0;
            spark.id = arg0[0x3B];
            spark.field08 = 15.045272f;
            spark.field10 = 5.8676558f;
            spark.field18 = 12.111444f;
            spark.field0C = 0.396174f;
            spark.field24 = 0.0f;
            spark.field28 = 0.0f;
            spark.field38 = 0.0f;
            sparkEffect.field14 = 0.0f;
            sparkEffect.field18 = 0.0f;
            spark.field3C = 0.0f;
            spark.field48 = 1.0f;
            spark.field40 = 1.0f;
            spark.field4C = 1.0f;
            spark.field44 = 1.0f;
            spark.object = object;
            sparkEffect.kind = 0x5F;
            sparkEffect.mode = 8;
            sparkEffect.flags = 0x2203;
            sparkEffect.lifetime = 0x12C;
            sparkEffect.field08 = 0;
            sparkEffect.field0C = 0;
            sparkEffect.color[0] = 0xFF;
            sparkEffect.color[1] = 0xFF;
            sparkEffect.color[2] = 0xFF;
            sparkEffect.color[3] = 0xFF;
            spark.field14 = 0.1042563f;
            spark.field20 = 0.5197371f;
            spark.field1C = 0.34404588f;
            spark.field2C = 160.0f;
            spark.field30 = 95.0f;
            spark.field34 = 207.5f;
            sparkEffect.field1C = *(struct17 *)&D_800A5480;
            sparkEffect.field28 = *(struct17 *)&D_800A5480;
            sparkEffect.field34 = 1.0f;
            sparkEffect.field38 = 1.0f;
            sparkEffect.field3C = 1.0f;
            sparkEffect.field40 = 0x40EC0000;
            sparkEffect.field44 = 0;
            sparkEffect.field45 = 0xFF;
            sparkEffect.field46 = 0;
            sparkEffect.field47 = 7;
            sparkEffect.field48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                data->sparks[spark.index] = func_1513D2F0(&sparkEffect, &D_800A4AA0, 0, 0x25, 0, 0x1D,
                    ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, 0x5C, arg1, arg2);
                if (data->sparks[spark.index] != 0) {
                    memcpy((u8 *)data->sparks[spark.index] + 0x110, &spark, sizeof(spark));
                }
            }
        }
        {
            Game1E9A30BurstGlowShape shape;
            Game1E9A30BurstEffect glowEffect;
            Game1E9A30BurstGlow glow;

            glow.field00 = 0;
            glow.field04 = 1.0f;
            glow.owner = arg0;
            glow.object = object;
            glow.id = arg0[0x3B];
            shape.field00 = 50.3f;
            shape.field04 = 50.3f;
            shape.field08 = 30.9f;
            shape.field0C = 30.9f;
            shape.field10 = 0.1f;
            shape.field14 = 0.1f;
            shape.field1C = 0.7f;
            shape.field20 = 0.000001f;
            shape.field24 = -1;
            shape.field34 = -1;
            shape.field28 = -1;
            shape.field38 = -1;
            shape.field2C = -1;
            shape.field3C = -1;
            shape.field30 = -1;
            shape.field40 = -1;
            shape.field18 = 1.0f;
            shape.field44 = 0;
            shape.field48 = 1.0f;
            shape.field4C = 0;
            shape.field4E = 0;
            shape.field50 = 0;
            shape.field52 = 0;
            shape.field54 = 0;
            shape.field55 = 0;
            shape.field56 = 0;
            shape.field57 = 0;
            shape.field58 = 0xF;
            shape.field59 = -1;
            glowEffect.kind = 0x60;
            glowEffect.mode = 3;
            glowEffect.flags = 0x2203;
            glowEffect.lifetime = 0x12C;
            glowEffect.field08 = 0;
            glowEffect.field0C = 0;
            glowEffect.color[0] = 0xFF;
            glowEffect.color[1] = 0xFF;
            glowEffect.color[2] = 0xFF;
            glowEffect.color[3] = 0xFF;
            glowEffect.field14 = 100.0f;
            glowEffect.field18 = 100.0f;
            glowEffect.field1C = *(struct17 *)&D_800A5480;
            glowEffect.field28 = *(struct17 *)&D_800A5480;
            glowEffect.field34 = 1.0f;
            glowEffect.field38 = 1.0f;
            glowEffect.field3C = 1.0f;
            glowEffect.field40 = 0xCD2002;
            glowEffect.field44 = 0xFF;
            glowEffect.field45 = 0xFF;
            glowEffect.field46 = 0;
            glowEffect.field47 = 7;
            glowEffect.field48 = 0;
            glowEffect.field4C = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                data->glows[glow.index] = (void *)func_151407D0(&shape, 0x78, &glowEffect, 0, 0x2A, 0, 0, -1, arg1, arg2);
                if (data->glows[glow.index] != 0) {
                    memcpy((u8 *)data->glows[glow.index] + 0x170, &glow, sizeof(glow));
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E73B0/func_151BDD8C.s")

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BE0AC;

typedef struct {
    s32 unk0;
    u8  b4;
} ArgB_151BE0AC;

void func_151BE0AC(s32 arg0, s32 arg1, u8 arg2) {
    s32 *pp = &arg0;
    s32 t;
    if (arg2 == 0x3B) {
        t = *pp + 0x28;
        if ((*(s32 *)t == *(s32 *)arg1) ||
            (*(u8 *)(t + 4) == *(u8 *)(pp[1] + 4))) {
            func_1516972C((struct102 *)*pp);
        }
    } else {
        t = *pp + 0x28;
        func_15169850(arg1, arg2, t, t + 4, *pp);
    }
}

void func_151BE138(struct260 *arg0) {
    struct102 **p;

    p = (struct102 **)((u8 *)arg0 + 0x28);
    if (p[2] != 0) {
        func_1516972C(p[2]);
    }
    if (p[3] != 0) {
        func_1516972C(p[3]);
    }
    if (p[4] != 0) {
        func_1516972C(p[4]);
    }
    if (p[5] != 0) {
        func_1516972C(p[5]);
    }
}

extern void func_151BE138(struct260 *arg0);

void func_151BE1B8(struct260 *arg0) {
    func_151BE138(arg0);
    func_1514933C(arg0);
}

extern void func_15149368(struct260 *arg0);

void func_151BE1E4(struct260 *arg0) {
    func_151BE138(arg0);
    func_15149368(arg0);
}

typedef struct {
    u8 pad0[0x8];
    f32 unk8;
    f32 unkC;
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
} Sub151BE210;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
    u8 pad5D[0xB3];
    union { Sub151BE210 unk110; struct { u8 pad[0x24]; f32 unk134; } s; } u;
} Obj151BE210;

s32 func_151BE210(Obj151BE210 *obj) {
    Sub151BE210 *sub;

    sub = &obj->u.unk110;
    obj->u.s.unk134 -= D_800BE9A4;
    if (obj->u.s.unk134 < 0.0f) {
        sub->unk24 = func_150ADA68() * 4.0f;
        sub->unk18 = func_150ADA68() * sub->unk10 + sub->unk8;
    }
    obj->unk2C = obj->unk2C + ((sub->unk18 - obj->unk2C) * 0.3499999940395355f);
    sub->unk28 -= D_800BE9A4;
    if (sub->unk28 < 0.0f) {
        sub->unk28 = func_150ADA68() * 9.0f;
        if (func_150ADA20() & 1) {
            sub->unk1C = func_150ADA68() * sub->unk14 + sub->unkC;
        } else {
            sub->unk1C = func_150ADA68() * sub->unk20 + sub->unkC;
        }
    }
    obj->unk30 = obj->unk30 + ((sub->unk1C - obj->unk30) * 0.4000000059604645f);
    sub->unk38 -= D_800BE9A4;
    if (sub->unk38 < 0.0f) {
        sub->unk38 = func_150ADA68() * 10.0f;
        sub->unk34 = func_150ADA68() * sub->unk30 + sub->unk2C;
    }
    sub->unk3C = sub->unk3C + ((sub->unk34 - sub->unk3C) * 0.4f);
    obj->unk5C = sub->unk3C;
    sub->unk40 = sub->unk40 + ((sub->unk48 - sub->unk40) * 0.6500000357627869f);
    sub->unk44 = sub->unk44 + ((sub->unk4C - sub->unk44) * 0.6500000357627869f);
    return 1;
}

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
} Obj151BE4B8;

typedef struct {
    Obj151BE4B8 *unk0;
    u8 unk4;
    u8 pad5[0x3B];
    f32 unk40;
    f32 unk44;
    u8 pad48[0xC];
    f32 unk54;
    u8 unk58;
} Sub151BE4B8;

s32 func_151BE4B8(void *arg0, s16 arg1) {
    Obj151BE4B8 *temp_v0;
    Sub151BE4B8 *sub;
    f32 sp20[2];

    sub = (Sub151BE4B8 *)((u8 *)arg0 + 0x110);
    temp_v0 = sub->unk0;
    if ((temp_v0->unk3B != sub->unk4) || (temp_v0->unk0 == 0)) {
        func_1516972C((struct102 *)arg0);
        return 0;
    }

    if (!(sub->unk58 & 1)) {
        return 0;
    }

    sp20[0] = sub->unk40 * sub->unk54;
    sp20[1] = sub->unk44 * sub->unk54;
    return func_1513FAB4(arg0, 0, sp20, arg1);
}

typedef struct { s32 unk0; u8 unk4; } SubA_151BE558;
typedef struct { s32 unk0; union { s32 w4; u8 b4; } u4; u8 unk8; u8 unk9; } ArgB_151BE558;

void func_151BE558(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151BE558 *temp_v0 = (SubA_151BE558 *)((u8 *)arg0 + 0xB0);
    ArgB_151BE558 *b = (ArgB_151BE558 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
            goto dummy_label_442082; dummy_label_442082: ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_151BE604(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x110, arg0 + 0x114, arg0);
}

struct Sub151BE644 {
    u8  pad0[0x5];
    u8  unk5;
    u8  pad6[0x4A];
    s32 unk50;
};

void func_151BE644(struct210 *arg0) {
    struct Sub151BE644 *v0 = (struct Sub151BE644 *)((u8 *)arg0 + 0x110);
    if (*(s32 *)((u8 *)arg0 + 0x160) != 0) {
        s32 v1 = v0->unk50 + 0x28;
        *(s32 *)(v1 + (v0->unk5 << 2) + 8) = 0;
    }
}

void func_151BE674(struct210 *arg0) {
    func_151BE644(arg0);
    func_1513CA6C(arg0);
}

void func_151BE6A0(struct210 *arg0) {
    func_151BE644(arg0);
    func_1513CAA0(arg0);
}

struct S4151BE6CC { s32 w[4]; };
extern struct S4151BE6CC D_800AA7A8;
extern f32 D_800AA8AC;

struct Bar151BE6CC {
    char pad48[0x48];
    f32 unk48;
};

struct Obj151BE6CC {
    char pad18[0x18];
    u8  unk18;
    char pad19[0x170 - 0x19];
    s16 unk170;
    char pad172[0x2];
    f32 unk174;
};

s32 func_151BE6CC(struct Obj151BE6CC *arg0) {
    s32 dummy;
    struct S4151BE6CC sp24;
    struct Bar151BE6CC *v1 = (struct Bar151BE6CC *)((char *)arg0 + 0x110);

    sp24 = D_800AA7A8;

    arg0->unk170 -= D_800BE9E4;
    if (arg0->unk170 < 0) {
        arg0->unk18 = sp24.w[func_150ADA20() & 3];
        arg0->unk170 = (func_150ADA20() & 7) + 3;
    }

    v1->unk48 = v1->unk48 + (arg0->unk174 - v1->unk48) * D_800AA8AC;
    return 1;
}

void func_151BE788(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x180, arg0 + 0x184, arg0);
}

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} BE7C8_elem;

typedef struct {
    u8 pad0[0x8];
    s32 *unk8;
    u8 unkC;
} BE7C8_sub;

void func_151BE7C8(struct210 *arg0) {
    BE7C8_sub *sub = (BE7C8_sub *)((u8 *)arg0 + 0x170);
    if (sub->unk8 != NULL) {
        s32 *arr = (s32 *)((u8 *)sub->unk8 + 0x28);
        ((BE7C8_elem *)&arr[sub->unkC])->unk10 = 0;
    }
}

extern void func_151BE7C8(struct210 *);
extern void func_151411A4(struct210 *);

void func_151BE7F8(struct210 *arg0) {
    func_151BE7C8(arg0);
    func_151411A4(arg0);
}

void func_151BE824(struct210 *arg0) {
    func_151BE7C8(arg0);
    func_151411C4(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E73B0/func_151BE850.s")

struct S151BEB20 {
    u8 pad0[0x54];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    u8 pad60[0x7C - 0x60];
    f32 mat[1][4][4];
    u8 padBC[0x120 - 0xBC];
    f32 unk120;
};

s32 func_151BEB20(struct S151BEB20 *arg0) {
    f32 m;

    func_150A8050(arg0->mat[D_800BE9C0], 0.0f, arg0->unk120, 0.0f);

    m = D_800AA8E0;
    arg0->mat[D_800BE9C0][3][0] = arg0->unk54;
    arg0->mat[D_800BE9C0][3][1] = arg0->unk58;
    arg0->mat[D_800BE9C0][3][2] = arg0->unk5C;

    arg0->mat[D_800BE9C0][0][0] *= m;
    arg0->mat[D_800BE9C0][0][1] *= m;
    arg0->mat[D_800BE9C0][0][2] *= m;
    arg0->mat[D_800BE9C0][1][0] *= m;
    arg0->mat[D_800BE9C0][1][1] *= m;
    arg0->mat[D_800BE9C0][1][2] *= m;
    arg0->mat[D_800BE9C0][2][0] *= m;
    arg0->mat[D_800BE9C0][2][1] *= m;
    arg0->mat[D_800BE9C0][2][2] *= m;

    return 1;
}

s32 func_151BEC94(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    *arg4 = 1;
    return arg0;
}

typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ u8 unk10;
    /* 0x011 */ u8 pad11[0x37];
    /* 0x048 */ u8 unk48;
    /* 0x049 */ u8 pad49[0xB];
    /* 0x054 */ struct17 unk54;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 pad64[0x98];
    /* 0x0FC */ s32 unkFC;
    /* 0x100 */ u8 pad100[0x1B];
    /* 0x11B */ u8 unk11B;
    /* 0x11C */ u8 pad11C[4];
} S151BECB8;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ struct17 unk4;
    /* 0x10 */ u8 unk10;
} S151BECB8Params;

extern void func_1515C1A0(struct127 *, f32 *, f32 *, f32 *);
extern f32 func_15143E64(struct17 *);
void func_151BF340(u8 arg0, s32 arg1);
void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
void func_151BF0C8();
s32 func_151BEE94(struct127 *arg0);

s32 func_151BECB8(S151BECB8 *arg0) {
    s32 ret = 1;

    {
        struct127 *p;
        S151BECB8Params *params;
        f32 range = 43.0f;
        f32 radius;
        f32 height;
        f32 pos[3];
        struct17 diff;

        p = D_800CC2D0;
        do {
            if (func_151BEE94(p) != 0) {
                f32 dist;

                params = (S151BECB8Params *)((u8 *)arg0 + 0x120);
                func_1515C1A0(p, pos, &radius, &height);
                diff.unk0 = pos[0] - params->unk4.unk0;
                diff.unk4 = pos[1] - params->unk4.unk4;
                diff.unk8 = pos[2] - params->unk4.unk8;
                dist = func_15143E64(&diff);
                if (dist - radius < range) {
                    func_15085710(p->unk127, 3, params->unk10);
                    ret = 0;
                    func_10010F30(0x511, 0x7D00, 0x40, 0, 0);
                    func_151BF340(arg0->unkC, arg0->unk1);
                    func_151BF0C8(&params->unk4);
                    {
                        s32 v;
                        s32 kind;

                        kind = arg0->unk48;
                        switch (kind) {
                            case 1:
                                v = 1;
                                break;
                            case 2:
                                v = 2;
                                break;
                            case 3:
                                v = 0;
                                break;
                            case 0:
                            default:
                                v = 0;
                                break;
                        }
                        func_151BEEE0(params->unk0, &arg0->unk54, arg0->unkFC, arg0->unk11B, v, arg0->unk10 & 0x10, arg0->unk60, arg0->unkC, arg0->unk1);
                    }
                }
            }
            p++;
        } while (p != (struct127 *)&D_800D1548);
    }
    return ret;
}

s32 func_151BEE94(struct127 *arg0) {
    if (arg0->unk127 == 0xFF) {
        return 0;
    }
    if (arg0->interaction_state == 0) {
        return 0;
    }
    if (arg0->id == 0xFF) {
        return 0;
    }
    return 1;
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ struct17 unk10;
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
    /* 0x69 */ u8  pad69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 pad78;
} S151BEEE0;

typedef struct { u8 b[3]; } U8x3_151BEEE0;
extern U8x3_151BEEE0 D_8008FBC0;
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_15133760(void *, void *);

void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    void *ret;
    U8x3_151BEEE0 tbl = D_8008FBC0;

    if (arg4 >= 3) {
        return NULL;
    }
    {
    S151BEEE0 sp44;
    s32 i;

    sp44.unk0 = 1.0f;
    sp44.unk4 = 1.0f;
    sp44.unkC = 0.9230000377f;
    sp44.unk8 = 0.9230000377f;
    sp44.unk10.unk0 = 0.0f;
    sp44.unk10.unk4 = arg0;
    sp44.unk10.unk8 = 0.0f;
    sp44.unk1C = 1.0f;
    sp44.unk20 = 1.0f;
    sp44.unk24 = 1.0f;
    sp44.unk28 = *arg1;
    sp44.unk34.unk0 = 0.0f;
    sp44.unk34.unk4 = 0.0f;
    sp44.unk34.unk8 = 0.0f;
    sp44.unk40.unk0 = 0.0f;
    sp44.unk40.unk4 = 0.0f;
    sp44.unk40.unk8 = 0.0f;
    sp44.unk4C = 0.0f;
    sp44.unk50 = (arg5 ? 0x4000 : 0) | 0x1D00 | 0x80000 | 0x40000;
    sp44.unk54 = 0x12C;
    sp44.unk56 = 0x26;
    sp44.unk58 = tbl.b[arg4];
    sp44.unk5C = arg6;
    sp44.unk60 = 0xFF;
    if (D_800BE9F0 == 7) {
        sp44.unk61 = 0x18;
    } else {
        sp44.unk61 = 0;
    }
    sp44.unk62 = 0;
    sp44.unk63 = 0;
    sp44.unk64 = 0;
    sp44.unk65 = 0;
    sp44.unk66 = 0;
    sp44.unk67 = 0;
    sp44.unk68 = 2;
    sp44.unk6A = 0;
    sp44.unk6C = 0;
    sp44.unk70 = 0;
    sp44.unk72 = 1;
    sp44.unk74 = 0xFF;
    ret = func_15132A4C(&sp44, arg2, arg3, 0, arg7, arg8);
    if (ret != NULL) {
        for (i = 0; i < 2; i++) {
            func_15133760((u8 *)ret + 0x90 + i * 0x40, ret);
        }
    }
    return ret;
    }
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_151BF0C8;

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3F_151BF0C8 unk8;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 unk3C;
    u8 pad3D;
    s16 unk3E;
    s16 unk40;
} Struct151BF0C8_A;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    Vec3F_151BF0C8 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    u8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} Struct151BF0C8_B;

void func_151BF0C8(Vec3F_151BF0C8 *arg0) {
    Struct151BF0C8_A sp84;
    Struct151BF0C8_B sp18;

    sp84.unk0 = 0x18;
    sp84.unk4 = 0xA;
    sp84.unk8 = *arg0;
    sp84.unk14 = D_800AA8E8;
    sp84.unk18 = D_800AA8EC;
    sp84.unk1C = 0;
    sp84.unk1E = 0xFF;
    sp84.unk20 = -0x32;
    sp84.unk22 = 0x3C;
    sp84.unk24 = 13.0f;
    sp84.unk28 = 13.0f;
    sp84.unk2C = D_800AA8F0;
    sp84.unk30 = D_800AA8F4;
    sp84.unk34 = D_800AA8F8;
    sp84.unk38 = D_800AA8FC;
    sp84.unk3C = 3;
    sp84.unk3E = 0x41;
    sp84.unk40 = 0x1E;
    func_15150F90(&sp84, 1, 0xFF, 1);

    sp18.unk0 = 0x14;
    sp18.unk2 = 8;
    sp18.unk4 = 0x6C;
    sp18.unk6 = 0x5103;
    sp18.unk8 = 0x200005;
    sp18.unkC = 0;
    sp18.unk10 = 0x28;
    sp18.unk12 = 0x28;
    sp18.unk14 = 0;
    sp18.unk18 = 0;
    sp18.unk1F = 0xFF;
    sp18.unk1C = 0xFF;
    sp18.unk1D = 0x91;
    sp18.unk1E = 6;
    sp18.unk20 = 0xFF;
    sp18.unk21 = 0xFF;
    sp18.unk22 = 0;
    sp18.unk23 = 0x14;
    sp18.unk24 = 0xC8;
    sp18.unk25 = 0xFF;
    sp18.unk26 = 0x32;
    sp18.unk28 = 5;
    sp18.unk2A = 0x32;
    sp18.unk2C = D_800AA900;
    sp18.unk30 = 175.0f;
    sp18.unk34 = 160.0f;
    sp18.unk38 = *arg0;
    sp18.unk44 = 0;
    sp18.unk46 = -0x33;
    sp18.unk48 = 0xFF;
    sp18.unk4A = 0x50;
    sp18.unk4C = 4.0f;
    sp18.unk50 = 13.0f;
    sp18.unk54 = D_800AA904;
    sp18.unk58 = D_800AA908;
    sp18.unk5C = 0x840E07;
    sp18.unk60 = 0x10;
    sp18.unk61 = -1;
    sp18.unk62 = 8;
    sp18.unk63 = 6;
    sp18.unk64 = 1;
    sp18.unk68 = D_800AA90C;
    func_15153634(&sp18, 0xFF, 0xFF, 1);
}

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S151BE850;
typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} S151BF81CPart;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} S151BF81CDigit;

/* The 0x18-byte block func_151BF340 copies to object+0x28. */
typedef struct {
    /* 0x00 */ S151BF81CPart *unk0;
    /* 0x04 */ S151BF81CDigit *unk4[2];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 unk14;
} S151BF81CState;

typedef struct {
    /* 0x00 */ u8 pad0[0xD];
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ S151BF81CState unk28;
} S151BF81CObj;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
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
    /* 0x58 */ f32 unk58;
} S151BF340Digit;

typedef struct {
    /* 0x00 */ struct17 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} S151BF340Payload;

extern void *func_1515548C(S151BF340Digit *, s32, s32, s32, s32, u8, s32);

void func_151BF340(u8 arg0, s32 arg1) {
    S151BF81CState sp160;
    struct260 *ret;
    s32 count;
    s32 digits[2];
    S151BF81CState *s;

    func_151494E0(0, 0x57);
    count = func_150859AC(0, 3);
    if (count != 0) {
        count--;
    }
    if (count >= 100) {
        count = 99;
    }
    digits[0] = count / 10;
    digits[1] = count % 10;
    sp160.unk0 = NULL;
    bzero(sp160.unk4, 8);
    sp160.unkC = 110.0f;
    sp160.unk10 = sinf(1.905000091f);
    sp160.unk14 = 0;
    ret = func_15149130(0x12C, -1, 0x60, -1, 0, 0x49, (struct37 *)0x18, arg0, arg1);
    if (ret != NULL) {
        s = &((S151BF81CObj *)ret)->unk28;
        memcpy(s, &sp160, 0x18);
        {
        S151BE850 spF4;
        S151BF340Payload spE0;
        S151BF340Digit sp84;

        spF4.unk38 = 0;
        spF4.unk00 = 0xC0;
        spF4.unk01 = -1;
        spF4.unk02 = 5;
        spF4.unk03 = 3;
        spF4.unk04 = 0;
        spF4.unk06 = 0x12C;
        spF4.unk08 = 0xA3;
        spF4.unk0C = 8;
        spF4.unk10 = 0x100000;
        spF4.unk14 = 0x200004;
        spF4.unk18 = 0x60601;
        spF4.unk2D = 8;
        spF4.unk1C = 1;
        spF4.unk20 = 0x38;
        spF4.unk2C = 0;
        spF4.unk24 = 0x80;
        spF4.unk28 = 0x20;
        spF4.unk30 = 0xFF;
        spF4.unk31 = 0xFF;
        spF4.unk32 = 0xFF;
        spF4.unk33 = 0xFF;
        spF4.unk34 = 0xFF;
        spF4.unk35 = 0xFF;
        spF4.unk36 = 0xFF;
        spF4.unk37 = 0xFF;
        spF4.unk3C = 0;
        spF4.unk40 = 0;
        spF4.unk41 = 1;
        spF4.unk54 = 1;
        spF4.unk56 = 0xFF;
        spF4.unk44.unk0 = 300.0f;
        if (D_800D2458) {
            spF4.unk44.unk4 = 32.0f;
        } else {
            spF4.unk44.unk4 = 79.0f;
        }
        spF4.unk44.unk8 = 0.0f;
        spE0.unk0 = *(struct17 *)&D_800A5480;
        spE0.unkC = 0.3010000288f;
        spE0.unk10 = 0.3010000288f;
        s->unk0 = func_15157010(&spF4, 0, 1.0f, 0, 0, 0x14, arg0, arg1);
        if (s->unk0 != NULL) {
            memcpy((u8 *)s->unk0 + 0x120, &spE0, 0x14);
        }
        {
            const s32 tbl[10] = { 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD };
            u8 i;

            sp84.unk00 = -500.0f;
            if (D_800D2458) {
                sp84.unk04 = -15.0f;
            } else {
                sp84.unk04 = -62.0f;
            }
            sp84.unk12 = 0x12C;
            sp84.unk14 = 0x20;
            sp84.unk16 = 1;
            sp84.unk18 = 0xFF;
            sp84.unk1A = 0;
            sp84.unk1B = 0xFF;
            sp84.unk1C = 0xFF;
            sp84.unk1D = 0xFF;
            sp84.unk1E = 0xFF;
            sp84.unk1F = 0xFF;
            sp84.unk20 = 0xFF;
            sp84.unk21 = 0xFF;
            sp84.unk22 = 0xFF;
            sp84.unk23 = 0xFF;
            sp84.unk24 = 0;
            sp84.unk28 = 0x200004;
            sp84.unk2C = 0x9F0601;
            sp84.unk40 = 0;
            sp84.unk41 = 0;
            sp84.unk30 = 0x19;
            sp84.unk34 = 0x13;
            sp84.unk38 = 0x80;
            sp84.unk3C = 0x20;
            sp84.unk44 = 0;
            sp84.unk50 = 0.0f;
            sp84.unk54 = 0.0f;
            sp84.unk0C = 16.0f;
            sp84.unk08 = 16.0f;
            sp84.unk48 = 1.f;
            sp84.unk4C = 1.f;
            if (digits[0] == 0) {
                sp84.unk10 = tbl[digits[1]];
                s->unk4[0] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                s->unk4[1] = NULL;
            } else {
                for (i = 0; i < 2; i++) {
                    sp84.unk10 = tbl[digits[i]];
                    s->unk4[i] = func_1515548C(&sp84, 0, 0, 0, 0, arg0, arg1);
                }
            }
        }
        }
    }
}

typedef struct Game1EC390Animation {
    void *primary;
    void *secondary[2];
    f32 timer;
    f32 amount;
    u8 flags;
} Game1EC390Animation;

s32 func_1517EF00(s32);
s32 func_15181CC8(s32);
extern f32 D_800AA918, D_800AA91C, D_800AA920;
extern f32 D_800AA924, D_800AA928, D_800AA92C;

void func_151BF81C(void *arg0) {
    f32 amount;
    Game1EC390Animation *animation;

    if (D_800BEAC1 != 0 || (animation = (Game1EC390Animation *)((u8 *)arg0 + 0x28), D_800BEAC0 != 0)) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
        return;
    }
    if (!(animation->flags & 1)) {
        if (D_800C35EA == 1) return;
        if (func_15181CC8(0) == 0) return;
        if (func_1517EF00(0) != 0 || D_800BE9B4 != 0) return;
        animation->flags |= 1;
    }
    if (animation->timer < D_800AA918) {
        amount = sinf(animation->timer * D_800AA91C * D_800AA920);
    } else if (D_800AA924 < animation->timer) {
        amount = sinf((110.0f - animation->timer) * D_800AA928 * D_800AA92C);
    } else {
        amount = animation->amount;
    }
    if (animation->primary != NULL) {
        *(f32 *)((u8 *)animation->primary + 0x54) = amount * -85.0f + 150.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->primary + 0x58) = 32.0f;
        else *(f32 *)((u8 *)animation->primary + 0x58) = 79.0f;
    }
    if (animation->secondary[0] != NULL) {
        *(f32 *)((u8 *)animation->secondary[0] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) - 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[0] + 0x14) = -62.0f;
    }
    if (animation->secondary[1] != NULL) {
        *(f32 *)((u8 *)animation->secondary[1] + 0x10) = ((amount * -85.0f + 150.0f) / 1.0f + 48.0f) + 12.0f;
        if (D_800D2458 != 0) *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -15.0f;
        else *(f32 *)((u8 *)animation->secondary[1] + 0x14) = -62.0f;
    }
    animation->timer -= D_800BE9A4;
    if (animation->timer < 0.0f) {
        *(s16 *)((u8 *)arg0 + 0xE) = -1;
        *((u8 *)arg0 + 0xD) |= 1;
    }
}


void func_151BFB2C(struct260 *arg0) {
    s32 i;
    s32 v;
    s32 *p = (s32 *)((u8 *)arg0 + 0x28);

    if (p[0] != 0) {
        func_1516972C((struct102 *)p[0]);
    }
    for (i = 0; i < 2; i = (u8)(i + 1)) {
        v = p[i + 1];
        if (v != 0) {
            func_1516972C((struct102 *)v);
        }
    }
}

void func_151BFBA4(struct260 *arg0) {
    func_151BFB2C(arg0);
    func_1514933C(arg0);
}

void func_151BFBD0(struct260 *arg0) {
    func_151BFB2C(arg0);
    func_15149368(arg0);
}

void func_151BFBFC(struct102 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x57) {
        func_1516972C(arg0);
    }
}
