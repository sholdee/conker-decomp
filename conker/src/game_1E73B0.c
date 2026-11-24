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


typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad6;
    s16 unk8;
    s16 unkA;
    s32 padC;
} Pt151BA084;

typedef struct {
    u8 pad0[0x50];
    s32 unk50;
    u8 pad54[4];
    Pt151BA084 *unk58[1][4];
    u8 pad68[0x78 - 0x68];
    Mtx mtx[1];
} Obj151BA084;

typedef struct {
    u8 pad0[0x197];
    u8 unk197;
    u8 pad198[0x23D - 0x198];
    u8 unk23D;
} Ent151BA084;

typedef struct {
    s32 unk0;
    u8 pad4[0x3B - 4];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x1D4 - 0x75];
    u8 *unk1D4;
    u8 pad1D8[0x318 - 0x1D8];
    Ent151BA084 *unk318;
    Ent151BA084 *unk31C;
} Player151BA084;

typedef struct {
    Player151BA084 *unk0;
    u8 unk4;
} Sub151BA084;

extern u8 D_800C3E90;
extern Ent151BA084 *D_800CC5E8;
extern Ent151BA084 *D_800CC5EC;
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);


s32 func_151BA084(Obj151BA084 *arg0, s16 arg1) {
    Sub151BA084 *sub;
    Player151BA084 *pl;
    u8 *src;
    f32 mtx[4][4];
    Player151BA084 *p0;
    f32 m2[4][4];
    s32 i;
    f32 offx;
    f32 o[3];
    f32 b[3];
    f32 offy;
    f32 x;
    f32 y;
    f32 z;
    f32 t[3];
    f32 r[3];

    sub = (Sub151BA084 *)((s32)arg0 + arg0->unk50 + 0xF8);
    pl = sub->unk0;
    if (pl->unk0 == 0 || pl->unk3B != sub->unk4) {
        func_1516972C((struct102 *)arg0);
    }
    src = pl->unk1D4;
    if (src == NULL) {
        return 0;
    }
    if ((pl->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800BE616 != 0) {
        if (pl->unk318 != NULL && pl->unk31C != NULL && pl->unk31C->unk197 != 0 && arg1 == pl->unk318->unk23D) {
            return 0;
        }
    } else {
        p0 = (Player151BA084 *)D_800CC2D0;
        if (p0->unk318 != NULL && p0->unk31C != NULL && p0->unk31C->unk197 != 0 && arg1 == p0->unk318->unk23D) {
            return 0;
        }
    }
    if (D_800C3E90 != 0) {
    lbl2:
        guMtxL2F(mtx, (Mtx *)(src + 0x40));
    } else {
    lbl0:
        memcpy(mtx, src + 0x40, 0x40);
    }
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
    guMtxF2L(mtx, &arg0->mtx[D_800BE9C0]);
    memcpy(m2, &((f32 (*)[4][4])D_800D9D10)[arg1], 0x40);
    m2[0][3] = 0.0f;
    m2[1][3] = 0.0f;
    m2[2][3] = 0.0f;
    m2[3][3] = 1.0f;
label:
    func_150A7960(mtx, 0.0f, 67.0f, 159.0f, &b[0], &b[1], &b[2]);
    func_150A7960(m2, b[0], b[1], b[2], &o[0], &o[1], &o[2]);
    o[0] *= 2.002000093f;
    o[1] *= 2.002000093f;
    offx = (s32)(o[0] / 1024.0f) * 1024.0f;
    offy = (s32)(o[1] / 1024.0f) * 1024.0f;
    { s32 pad2;
    for (i = 0; i < 20; i++) {
        Pt151BA084 *q;
        q = &arg0->unk58[D_800BE9C0][arg1][i];
        x = q->x;
        y = q->y;
        z = q->z;
        func_150A7960(mtx, x, y, z, &t[0], &t[1], &t[2]);
        func_150A7960(m2, t[0], t[1], t[2], &r[0], &r[1], &r[2]);
        r[0] *= 2.002000093f;
        r[1] *= 2.002000093f;
        q->unk8 = r[0] - offx;
        q->unkA = r[1] - offy;
    }
    }
    return 1;
}

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
void func_151BC370(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6);
s32 func_151BC580(s32 *arg0);

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
void func_151BC5A4(struct260 *arg0, s32 arg1, u8 arg2);

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
s32 func_151BC64C(u8 *actor);

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
s32 func_151BC794(Actor151BC794 *arg0);

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
Gfx *func_151BCA90(Actor151BCA90 *arg0, Gfx *gfx, s16 arg2);
void func_151BD21C(void *arg0, void *arg1, u8 arg2);
s32 func_151BD2BC(s32 *arg0);

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
void func_151BD2F8(void *arg0, s32 arg1, s32 arg2);
s32 func_151BD42C(u8 *arg0);

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
void func_151BD43C(f32 *arg0, f32 *arg1, s32 arg2, s32 arg3, f32 arg4, Actor151BD43C *arg5);
f32 func_151BD750(s16 *arg0);

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
void func_151BD79C(struct S151BD79C *arg0);
void func_151BD7F4(s32 arg0);

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
void func_151BD828(u8 *arg0, u8 arg1, s32 arg2);

typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[8];
    u8 flags;
} Part151BDD8C;

typedef struct {
    u8 pad0[4];
    f32 unk4;
} Sub151BDD8C;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x110 - 0x4C];
    Part151BDD8C part;
} Obj151BDD8C_A;

typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0xC];
    s32 flags58;
    u8 pad5C[0x170 - 0x5C];
    Sub151BDD8C sub;
} Obj151BDD8C_B;

typedef struct {
    struct127 *owner;
    u8 ownerId;
    Obj151BDD8C_A *a[2];
    Obj151BDD8C_B *b[2];
} Data151BDD8C;

typedef struct {
    u8 pad0[0xE];
    s16 state;
    u8 pad10[0x18];
    Data151BDD8C data;
} Effect151BDD8C;

extern struct17 D_800AA778;
extern struct17 D_800AA784;
extern struct17 D_800AA790;
extern struct17 D_800AA79C;
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
void func_151BDD8C(Effect151BDD8C *effect);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151BE0AC;

typedef struct {
    s32 unk0;
    u8  b4;
} ArgB_151BE0AC;
void func_151BE0AC(s32 arg0, s32 arg1, u8 arg2);
void func_151BE138(struct260 *arg0);

extern void func_151BE138(struct260 *arg0);
void func_151BE1B8(struct260 *arg0);

extern void func_15149368(struct260 *arg0);
void func_151BE1E4(struct260 *arg0);

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
s32 func_151BE210(Obj151BE210 *obj);

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
s32 func_151BE4B8(void *arg0, s16 arg1);

typedef struct { s32 unk0; u8 unk4; } SubA_151BE558;
typedef struct { s32 unk0; union { s32 w4; u8 b4; } u4; u8 unk8; u8 unk9; } ArgB_151BE558;
void func_151BE558(struct260 *arg0, s32 arg1, u8 arg2);
void func_151BE604(s32 arg0, s32 arg1, u8 arg2);

struct Sub151BE644 {
    u8  pad0[0x5];
    u8  unk5;
    u8  pad6[0x4A];
    s32 unk50;
};
void func_151BE644(struct210 *arg0);
void func_151BE674(struct210 *arg0);
void func_151BE6A0(struct210 *arg0);

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
s32 func_151BE6CC(struct Obj151BE6CC *arg0);
void func_151BE788(s32 arg0, s32 arg1, u8 arg2);

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} BE7C8_elem;

typedef struct {
    u8 pad0[0x8];
    s32 *unk8;
    u8 unkC;
} BE7C8_sub;
void func_151BE7C8(struct210 *arg0);

extern void func_151BE7C8(struct210 *);
extern void func_151411A4(struct210 *);
void func_151BE7F8(struct210 *arg0);
void func_151BE824(struct210 *arg0);

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
s32 func_151BEB20(struct S151BEB20 *arg0);
s32 func_151BEC94(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4);

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
s32 func_151BECB8(S151BECB8 *arg0);
s32 func_151BEE94(struct127 *arg0);

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
void *func_151BEEE0(f32 arg0, struct17 *arg1, s32 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);

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
void func_151BF0C8(Vec3F_151BF0C8 *arg0);

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
void func_151BF340(u8 arg0, s32 arg1);

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
void func_151BF81C(void *arg0);
void func_151BFB2C(struct260 *arg0);
void func_151BFBA4(struct260 *arg0);
void func_151BFBD0(struct260 *arg0);
void func_151BFBFC(struct102 *arg0, s32 arg1, u8 arg2);
