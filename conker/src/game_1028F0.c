#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A0AB8;
extern f32 D_800A0ABC;
extern f32 D_800A0AC0;
extern f32 D_800A0AC4;
extern f32 D_800A0AC8;
extern f32 D_800A0AB0;
extern f32 D_800A0AB4;

extern void func_15182670(s32, s32, s32, s32, s32, s32, s32, s32);
extern void *func_15154884(struct Vec3F *, f32, f32, f32, u8, s32);

void func_150D5440(void *arg0, u8 arg1, s32 arg2) {
    struct {
        void *unk0;
        u8 unk4;
        u8 pad5;
        u8 pad6;
        u8 pad7;
        f32 unk8;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    sp34.unk8 = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x38, -1, 0, 0x28, (struct37 *)0xC, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

typedef struct {
    s32 interaction_state;
    u8 pad4[0x14 - 0x4];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x28 - 0x20];
    f32 unk28;
    u8 pad2C[0x3B - 0x2C];
    u8 unique_id;
    u8 pad3C[0x40 - 0x3C];
    f32 unk40;
    u8 pad44[0x180 - 0x44];
    f32 unk180;
    s32 unk184;
} Owner54C8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
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
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Particle54C8; /* 0x70 */

typedef struct {
    Owner54C8 *owner;
    u8 ownerId;
    u8 pad5[3];
    f32 accum;
} Data54C8;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 0x2];
    u8 unkC;
    u8 padD;
    s16 state;
    u8 pad10[0x28 - 0x10];
    Data54C8 data;
} Effect54C8;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern s32 func_1515C0F8(Owner54C8 *arg0, f32 **arg1);

void func_150D54C8(Effect54C8 *effect) {
    Owner54C8 *owner;
    Data54C8 *data;
    Particle54C8 spawn;
    f32 c;
    f32 s;
    f32 cx;
    f32 nc;
    f32 *ref;
    u8 flip;
    f32 r1;
    f32 r2;
    f32 side;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner) {}
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->unk28 < 8.0f && (owner->unk184 & 0x1F) == 0xE) {
        data->accum += (3.000000238f + func_150ADA68() * (f32)1) * D_800BE9A4;
        if (data->accum > 1.0f) {
            if (func_1515C0F8(owner, &ref) == 0) {
                ref = &D_800A5480;
            }
            func_1514373C(owner->unk40 * 0.01745329238f, 1.0f, &s, &c);
            cx = c;
            nc = -s;
            spawn.unk1D = 0x28;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0xFF;
            spawn.unk19 = 0xFF;
            spawn.unk1A = 0xFF;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk1C = 0xFF;
            spawn.unk1E = 5;
            spawn.unk20 = 0x33;
            spawn.unk22 = 1;
            spawn.unk58 = 0x84C207;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk24 = 1.0f;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            do {
                flip = func_150ADA20() & 1;
                r1 = func_150ADA68();
                r2 = func_150ADA68();
                if (flip) {
                    side = 35.0f;
                } else {
                    side = -35.0f;
                }
                spawn.pos.unk0 = ((owner->unk14 + (cx * side)) - ((s * 180.0f) * 0.5f)) + ((s * 180.0f) * r1);
                spawn.pos.unk4 = owner->unk180;
                spawn.pos.unk8 = ((owner->unk1C + (nc * side)) - ((c * 180.0f) * 0.5f)) + ((c * 180.0f) * r1);
                spawn.vel.unk0 = ((flip ? (r2 = (r2 * 2.907000065f) + 6.000000477f) : -(r2 = (r2 * 2.907000065f) + 6.0000005f)) * cx) - (ref[0] * 0.5640000105f);
                spawn.vel.unk4 = 3.156000137f * r2;
                spawn.vel.unk8 = ((flip ? r2 : -r2) * nc) - (ref[2] * 0.564f);
                spawn.unkA = func_150ADA20() % 10U + 0x1F;
                spawn.unk1B = func_150ADA20() % 76U + 0xB4;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 364.0f + 111.0f;
                spawn.unk54 = func_150ADA68() * 0.1570000052f + -2.111000061f;
                spawn.unk58 &= ~0xC0;
                flip = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | flip;
                func_15130280(&spawn, 1, 0, 0, effect->unkC, effect->unk1);
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        }
    }
}





typedef struct {
    s32 unk0;
    u8  unk4;
} SubA150D596C;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB150D596C;

void func_150D596C(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA150D596C *temp_v0 = (SubA150D596C *)((u8 *)arg0 + 0x28);
    ArgB150D596C *b = (ArgB150D596C *)arg1;
    s32 b0;

    if (arg2 == 0 || arg2 == 0x2F || arg2 == 0x3) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
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

void func_150D5A2C(struct260 *arg0) {
    func_1514933C(arg0);
}

void func_150D5A4C(struct260 *arg0) {
    func_15149368(arg0);
}

struct Vec3F;

typedef struct {
    f32 x, y, z;
} Vec3f_150D5A6C;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 scaleXZ;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ Vec3f_150D5A6C pos;
    /* 0x34 */ f32 vel[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
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
    /* 0x69 */ u8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 unk78;
} Particle150D5A6C;

typedef struct {
    s32 v[3];
} Arr3_150D5A6C;

extern Arr3_150D5A6C D_800A0A30;
extern s32 *D_80089A20[];
extern s32 D_800A3F14[];

s32 func_15134070(struct127 *);
s32 func_15145128(void *, void *, void *, void *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, void *, void *, void *, void *, f32 *, s32, s32 *, f32);
s32 func_15144E80(void *, Vec3f_150D5A6C *, Vec3f_150D5A6C *, Vec3f_150D5A6C *);
f32 func_15144A74(Vec3f_150D5A6C *, Vec3f_150D5A6C *);
void func_15146078(Vec3f_150D5A6C *, Vec3f_150D5A6C *, Vec3f_150D5A6C *);
void func_15143874(s16, f32, f32 *, f32 *);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_151C05A4(void *, u8, s32);
void func_151C05F0(void *, u8, s32);
void func_151C0360(void *, void *, u8, s32);
void func_150D6388(void *arg0, u8 arg1, s32 arg2);
void func_150D6434(struct Vec3F *arg0, u8 arg1, s32 arg2);
void func_150D65F0(s32 arg0, s32 arg1, u8 arg2, s32 arg3);
void func_150D66A4(u8 arg0, u8 arg1, s32 arg2);

void func_150D5A6C(struct127 *arg0, u8 arg1, s32 arg2) {
    Vec3f_150D5A6C pos;
    Vec3f_150D5A6C vel;
    s32 sp1A4;
    s32 sp1A0;
    s32 sp19C;
    u8 hit;
    s32 needDefault;
    f32 sp188[3];
    s32 sp184;
    s32 sp180;
    s32 sp17C;
    s32 type;
    f32 dist;
    Vec3f_150D5A6C n;
    Vec3f_150D5A6C b;
    Vec3f_150D5A6C c;
    s32 found;
    Particle150D5A6C p;

    hit = 0;
    pos.x = arg0->x_position;
    pos.y = arg0->y_position + 50.0f;
    pos.z = arg0->z_position;
    vel.x = arg0->x_position - arg0->old_x_position;
    vel.y = arg0->y_position - arg0->old_y_position;
    vel.z = arg0->z_position - arg0->old_z_position;
    type = func_15134070(arg0);
    if (type == 0x63) {
        return;
    }
    if ((arg0->unk184 & 0x1F) == 0xE) {
        func_10010630(0x434, arg0, 0x7D00, 0x2BC, 0x7D0);
    } else {
        func_10010630(0x4C8, arg0, -0x7D00, 0x2BC, 0x7D0);
    }
    func_10010630((func_150ADA20() & 1) + 0x2B6, arg0, 0x7D00, 0x3E8, 0xBB8);
    func_10010630((func_150ADA20() & 3) + 0x2B1, arg0, 0x7D00, 0x2BC, 0x7D0);
    func_151C05A4(&pos, arg1, arg2);
    func_151C05F0(&pos, arg1, arg2);
    func_15145A50(arg0);
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    found = func_15145128(&vel, &vel, &sp184, &sp180) ? 1 : 0;
    if (found) {
        typedef s32 ZQ;
        if (func_150AC9C0(arg0->old_x_position, arg0->old_y_position, arg0->old_z_position,
                               vel.x, vel.y, vel.z, 0, sp188, &sp19C, &sp1A0, &sp1A4, &dist, 0, &sp17C, 0.0f) && dist < 397.0f) {
            hit = 1;
        }
    }
    { typedef s32 ZQ2;
    needDefault = 1;
    if (hit) {
        needDefault = 0;
        if (func_15144E80(sp188, &b, &c, &n)) {
            s32 spCC;
            s32 spC8;
            s32 spC4;
            s32 spC0;
            s32 spBC;
            s32 spB8;

            func_15145128(&b, &b, &spCC, &spC8);
            func_15145128(&c, &c, &spC4, &spC0);
            func_15145128(&n, &n, &spBC, &spB8);
            if (func_15144A74(&n, &vel) > 0.0f) {
                n.x = -n.x;
                n.y = -n.y;
                n.z = -n.z;
            }
        } else {
            needDefault = 1;
        }
    }
    if (needDefault) {
        if (found) {
            n.x = -vel.x;
            n.y = -vel.y;
            n.z = -vel.z;
            func_15146078(&n, &b, &c);
        } else {
            n.x = 0.0f;
            n.y = 1.0f;
            n.z = 0.0f;
            b.x = 1.0f;
            b.y = 0.0f;
            b.z = 0.0f;
            c.x = 0.0f;
            c.y = 0.0f;
            c.z = 1.0f;
        }
    }
    p.unk0 = 1.0f;
    p.unk4 = 1.0f;
    n.x *= 1000.0f;
    n.y *= 1000.0f;
    n.z *= 1000.0f;
    p.scaleXZ = arg0->xz_scale;
    p.scaleY = arg0->y_scale;
    p.unk1C[0] = 1.0f;
    p.unk1C[1] = 1.0f;
    p.unk1C[2] = 1.0f;
    p.pos = pos;
    p.unk50 = 0x21E8;
    p.unk58 = 0;
    p.unk5C = 0;
    p.unk60 = 0xFF;
    p.unk61 = 8;
    p.unk62 = 0;
    p.unk63 = 0;
    p.unk64 = 0;
    p.unk65 = 0;
    p.unk66 = 0;
    p.unk67 = 0;
    p.unk68 = 2;
    p.unk6A = 1;
    p.unk6C = arg0;
    p.unk70 = arg0->unique_id;
    p.unk78 = 0;
    p.unk44 = 0.0f;
    {
        const s32 arr[3] = { 0x6C, 0x6D, 0x6E };
        s32 i;
        f32 spA4;
        f32 spA0;
        f32 scale;
        s32 r;
        f32 sp94;
        f32 sp90;
        s32 j;

    p.unk72 = 0xC;
    p.unk74 = 0x15;
    for (i = 0; i != 3; i++) {
        scale = func_150ADA68() * 0.01319999993f + 0.02979999967f;
        r = func_150ADA20();
        func_15143874((u8)r, func_150ADA68() * 759.0f, &spA0, &spA4);
        p.vel[0] = (b.x * spA0 + c.x * spA4 + n.x) * scale;
        p.vel[1] = (b.y * spA0 + c.y * spA4 + n.y) * scale;
        p.vel[2] = (b.z * spA0 + c.z * spA4 + n.z) * scale;
        p.rot[0] = func_150ADA68() * 360.0f;
        p.rot[1] = func_150ADA68() * 360.0f;
        p.rot[2] = func_150ADA68() * 360.0f;
        p.unk40 = func_150ADA68() * 38.44400024f + -19.22200012f;
        p.unk48 = func_150ADA68() * 38.44400024f + -19.22200012f;
        p.unk4C = func_150ADA68() * 0.6480000019f + -1.397000074f;
        p.unk54 = (func_150ADA20() % 21U) + 30;
        p.unk56 = arr[i];
        func_15132A4C(&p, 0, 0, 0, arg1, arg2);
    }
    p.unk72 = 0xC;
    p.unk74 = 0x15;
    for (j = 2; j < D_800A3F14[type]; j++) {
        scale = func_150ADA68() * 0.01999999955f + 0.005200000014f;
        r = func_150ADA20();
        func_15143874((u8)r, func_150ADA68() * 723.0f, &sp90, &sp94);
        p.vel[0] = (b.x * sp90 + c.x * sp94 + n.x) * scale;
        p.vel[1] = (b.y * sp90 + c.y * sp94 + n.y) * scale;
        p.vel[2] = (b.z * sp90 + c.z * sp94 + n.z) * scale;
        p.rot[0] = func_150ADA68() * 360.0f;
        p.rot[1] = func_150ADA68() * 360.0f;
        p.rot[2] = func_150ADA68() * 360.0f;
        p.unk40 = func_150ADA68() * 40.45000076f + -20.22500038f;
        p.unk48 = func_150ADA68() * 40.45000076f + -20.22500038f;
        p.unk4C = func_150ADA68() * 0.6260000467f + -1.089000106f;
        p.unk54 = (func_150ADA20() % 41U) + 40;
        p.unk56 = D_80089A20[type][j];
        func_15132A4C(&p, 0, 0, 0, arg1, arg2);
    }
    }
    if (arg0->camera != NULL && D_80082FA0 <= 0) {
        func_150D66A4(arg0->camera->unk23D, arg1, arg2);
    }
    if (hit && arg0->camera != NULL) {
        if ((sp17C & 0x1F) == 0xE) {
            func_150D65F0((s32)sp188, (s32)&sp19C, arg1, arg2);
        } else {
            func_151C0360(sp188, &sp19C, arg1, arg2);
        }
    }
    func_150D6434((struct Vec3F *)&pos, arg1, arg2);
    func_150D6388(&pos, arg1, arg2);
    }
}

void func_151541B8(void *, f32, s32, f32, f32, u8, s32);

void func_150D6388(void *arg0, u8 arg1, s32 arg2) {
    union { f32 f; f64 _a; } temp_f0;
    u32 temp_a;

    temp_f0.f = func_150ADA68();
    temp_a = func_150ADA20();
    func_151541B8(arg0, temp_f0.f * 4.0f + 15.0f, 0x3FAFF1E9,
                  (f32)(u32)((temp_a % 0x38U) + 0xC8), 0.0f, arg1, arg2);
}

void func_150D6434(struct Vec3F *arg0, u8 arg1, s32 arg2) {
    union { f32 f; f64 _a; } temp1;
    f32 temp0;

    temp0 = func_150ADA68();
    temp1.f = func_150ADA68();
    func_15154884(arg0, temp0 * 3.0f + 8.0f, temp1.f * D_800A0AB0 + D_800A0AB4,
                  func_150ADA68() * 50.0f + 100.0f, arg1, arg2);
}

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u8  pad08[0xC];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} Struct150D64E8;

void func_150D64E8(Struct150D64E8 *arg0, f32 *arg1) {
    arg0->unk00 = 6;
    arg0->unk04 = 3;
    arg1[0] = D_800A0AB8;
    arg0->unk14 = D_800A0ABC;
    arg0->unk18 = D_800A0AC0;
    arg0->unk1C = 150.0f;
    arg0->unk20 = 105.0f;
    arg0->unk24 = 396.0f;
    arg0->unk28 = 612.0f;
    arg0->unk2C = 4;
    arg0->unk30 = 3;
    arg0->unk34 = 25.0f;
    arg0->unk38 = D_800A0AC4;
    arg0->unk3C = -2.0f;
    arg0->unk40 = D_800A0AC8;
    arg0->unk44 = 20;
    arg0->unk46 = 30;
    arg0->unk48 = 100;
    arg0->unk4A = 100;
    arg0->unk4C = 12;
    arg0->unk4E = 20;

    if (D_80082FA0 < 2) {
        if (D_8008FD8C < 8) {
            goto zero;
        }
    }

    arg0->unk50 = -1;
    return;

zero:
    arg0->unk50 = 0;
}

void func_151A5D58(f32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150D65F0(s32 arg0, s32 arg1, u8 arg2, s32 arg3) {
    union { u32 w; f64 _a; } temp_a;
    f32 temp_f0;
    u32 temp_b;

    temp_f0 = func_150ADA68();
    temp_a.w = func_150ADA20();
    temp_b = func_150ADA20();
    func_151A5D58(temp_f0 * 100.0f + 150.0f, ((temp_a.w % 71) + 0x82) & 0xFF, arg0, arg1,
                  (temp_b % 31) + 0x32, 0, 1, arg2, arg3);
}

void func_150D66A4(u8 arg0, u8 arg1, s32 arg2) {
    u32 temp1;
    u32 temp0;

    temp0 = func_150ADA20();
    temp1 = func_150ADA20();
    func_15182670(0xFF, 0xFF, 0xFF, (u8)(temp0 % 0x38 + 0xC8), temp1 % 6 + 0x19, arg0, arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1028F0/func_150D6730.s")

void func_150D6C98(f32 *arg0, f32 *arg1) {
    arg1[0] = arg0[5];
    arg1[1] = arg0[6] + 60.0f;
    arg1[2] = arg0[7];
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_150D6CC4;

void func_15131958(void *, f32);

s32 func_150D6CC4(u8 *arg0, s32 arg1) {
    u8 *state;
    u8 *object;

    state = arg0 + 0xA8;
    object = *(u8 **)state;
    if (*(s32 *)object == 0 || state[4] != object[0x3B]) {
        return 0;
    }
    func_150D6C98((f32 *)object, (f32 *)(arg0 + 0x40));
    {
        Vec3f_150D6CC4 previous = *(Vec3f_150D6CC4 *)(arg0 + 0x58);
        f32 changeX;
        f32 changeY;
        f32 changeZ;

        *(f32 *)(arg0 + 0x5C) += *(f32 *)(arg0 + 0x64) * D_800BE9A4;
        func_15131958(arg0 + 0x58, *(f32 *)(state + 8));
        changeX = (*(f32 *)(arg0 + 0x58) - previous.x) * D_800BE9A8;
        changeY = (*(f32 *)(arg0 + 0x5C) - previous.y) * D_800BE9A8;
        changeZ = (*(f32 *)(arg0 + 0x60) - previous.z) * D_800BE9A8;
        *(f32 *)(arg0 + 0x4C) += previous.x * D_800BE9A4 +
            changeX * D_800BE9A4 * D_800BE9A4 * 0.5f;
        *(f32 *)(arg0 + 0x50) += previous.y * D_800BE9A4 +
            changeY * D_800BE9A4 * D_800BE9A4 * 0.5f;
        *(f32 *)(arg0 + 0x54) += previous.z * D_800BE9A4 +
            changeZ * D_800BE9A4 * D_800BE9A4 * 0.5f;
    }
    return 1;
}

void func_150D6E60(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA150D596C *temp_v0 = (SubA150D596C *)((u8 *)arg0 + 0xA8);
    ArgB150D596C *b = (ArgB150D596C *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                temp_v0->unk4 = b->unk8;
            }
trailing_150D6E60:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

typedef struct {
    /* 0x00 */ struct127 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
} Struct150D6F0C_A8;

typedef struct {
    /* 0x00 */ u8 pad0[0x2B];
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40[3];
    /* 0x4C */ u8 pad4C[0x5C];
    /* 0xA8 */ Struct150D6F0C_A8 unkA8;
} Struct150D6F0C;

s32 func_150D6F0C(Struct150D6F0C *arg0, s32 arg1) {
    Struct150D6F0C_A8 *temp_v0;
    struct127 *temp_a3;
    f32 temp_f2;

    temp_v0 = &arg0->unkA8;
    temp_a3 = temp_v0->unk0;

    if (temp_a3->interaction_state == 0) {
        goto return_zero;
    }

    do {
        if (temp_a3->unique_id == temp_v0->unk4) {
            break;
        }
return_zero:
        return 0;
    } while (0);

    func_150D6C98((f32 *)temp_a3, arg0->unk40);

    temp_f2 = sqrtf(temp_v0->unk8) * temp_v0->unk10;
    arg0->unk3C = temp_f2;
    arg0->unk38 = temp_f2;

    arg0->unk2B = (u32)(temp_v0->unk14 - ((temp_v0->unk18 * temp_v0->unk8) * temp_v0->unk8));
    temp_v0->unk8 = temp_v0->unk8 + D_800BE9A4;

    if (temp_v0->unkC < temp_v0->unk8) {
        return 0;
    }
    return 1;
}

void func_150D7068(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA150D596C *temp_v0 = (SubA150D596C *)((u8 *)arg0 + 0xA8);
    ArgB150D596C *b = (ArgB150D596C *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                temp_v0->unk4 = b->unk8;
            }
trailing_150D7068:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
