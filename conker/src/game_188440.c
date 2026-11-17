#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1515AF90;

typedef struct {
    Vec3f_1515AF90 position;
    f32 scale;
    f32 zero;
    f32 ground;
} GameAF90Packet;

typedef struct {
    f32 height;
    s16 points[9];
    s32 object;
    u8 flags;
    u8 active;
    u8 pad1E[2];
    s32 field20;
} Game71820Hit_1515AF90;

s32 func_1504697C(Vec3f_1515AF90 *, u16, f32, Game71820Hit_1515AF90 *);
extern f32 D_800A6420;
extern f32 D_800A6424;
extern f32 D_800A6428;
extern f32 D_800A642C;
extern f32 D_800A6430;
extern f32 D_800A6434;

void func_1515AF90(s16 arg0) {
    if (!(func_150ADA20() & 0xF)) {
        void *result;
        GameAF90Packet packet;
        Vec3f_1515AF90 *base;
        s32 offset;
        u8 valid;
        s16 angle;
        f32 radius;
        f32 cosine;
        f32 sine;
        Game71820Hit_1515AF90 hit;
        Vec3f_1515AF90 query;
        Game71820Hit_1515AF90 lowerHit;
        Vec3f_1515AF90 lowerQuery;
        f32 lowerHeight;

        valid = 1;
        base = (Vec3f_1515AF90 *)&D_800DBFF0[arg0].unk2F8;
        offset = D_800DBFF0[arg0].unk380 * D_800A6420;
        {
            u32 roll = func_150ADA20();

            angle = (roll % 41U) + offset - 0x14;
            radius = func_150ADA68() * D_800A6424 + 107.0f;
            cosine = func_151423D8(angle);
            sine = func_151423D8(angle - 0x40);
            packet.position.x = base->x - radius * sine;
            packet.position.z = base->z - radius * cosine;
        }
        query.x = packet.position.x;
        hit.object = 0;
        hit.flags = 0;
        hit.active = 0;
        hit.field20 = 0;
        query.y = D_800A6428;
        query.z = packet.position.z;
        hit.height = D_800A642C;
        if (func_1504697C(&query, 0, base->y, &hit) != 0) {
            packet.ground = hit.height;
        } else {
            valid = 0;
        }
        if (valid) {
            lowerHeight = base->y + -250.0f;
            lowerHit.field20 = 0;
            lowerHit.active = 0;
            lowerHit.flags = 0;
            lowerHit.object = 0;
            lowerHit.height = D_800A6430;
            lowerQuery.x = packet.position.x;
            lowerQuery.y = base->y;
            lowerQuery.z = packet.position.z;
            if (func_1504697C(&lowerQuery, 0, lowerHeight, &lowerHit) != 0) {
                packet.position.y = lowerHit.height;
            } else {
                packet.position.y = lowerHeight;
            }
        }
        func_150ADA68();
        packet.scale = 195.0f * D_800A6434;
        packet.zero = 0.0f;
        if (valid) {
            angle = (func_150ADA20() % 61U) + 5;
            result = func_151491F4(angle, -1, 0xC, 1, 7, 0x18, 0xFF, 0);
            if (result != NULL) {
                memcpy((u8 *)result + 0x28, &packet, sizeof(packet));
            }
        }
    }
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} Vec1515B21C;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec1515B21C unk30;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ Vec1515B21C unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[0x4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0x8];
} Params1515B21C;

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    s8 unk7;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} Extra1515B21C;

typedef struct {
    /* 0x00 */ Vec1515B21C pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} Sub1515B21C;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ Sub1515B21C sub;
    /* 0x40 */ u8 pad40[0x4];
} Arg1515B21C;

extern f32 D_800BE9A4;
void *func_151303BC(Params1515B21C *, s32, s32);

void func_1515B21C(Arg1515B21C *arg0) {
    Sub1515B21C *sub;
    void *temp_v0;
    Params1515B21C params;
    Extra1515B21C extra;
    f32 zero;

    arg0->sub.unk10 += arg0->sub.unkC * D_800BE9A4;
    sub = &arg0->sub;
    if (arg0->sub.unk10 > 1.0f) {
        zero = 0.0f;
        {
        f32 temp_f2;

        params.unk1D = 0x2F;
        params.unk08 = 0xC01;
        params.unk00 = 0x200005;
        params.unk04 = 0;
        params.unk1A = 0;
        params.unk19 = 0;
        params.unk18 = 0;
        params.unk17 = 0;
        params.unk16 = 0;
        params.unk15 = 0;
        params.unk14 = 0;
        params.unk10 = 0;
        params.unk0C = 0;
        params.unk1C = 0xFF;
        params.unk22 = 0;
        params.unk24 = zero;
        params.unk58 = 0x3207;
        params.unk60 = 5;
        params.unk61 = 5;
        params.unk62 = 4;
        params.unk63 = -1;
        params.unk30 = sub->pos;
        params.unk3C = zero;
        params.unk40 = zero;
        params.unk44 = zero;
        params.unk1E = 0x32;
        params.unk20 = 5;
        extra.unk4 = 0;
        extra.unk5 = 0;
        extra.unk18 = 0;
        extra.unk19 = 0;
        params.unk48.x = zero;
        params.unk48.z = zero;
        extra.unk10 = 1.200000048f;
        extra.unk14 = 0.8333333135f;
        extra.unk24 = sub->unk14;

        if (func_150ADA20() & 1) {
            params.unk58 |= 0x40;
        }
        if (func_150ADA20() & 1) {
            params.unk58 |= 0x80;
        }

        do {
            params.unk48.y = func_150ADA68() * 12.0f * 0.1000000015f;
            params.unk0A = (func_150ADA20() % 152U) + 0x96;
            params.unk1B = (func_150ADA20() % 151U) + 0x37;
            temp_f2 = (func_150ADA68() * 200.0f) + 100.0f;
            params.unk28 = temp_f2;
            extra.unk0 = temp_f2;
            params.unk2C = temp_f2;
            extra.unk6 = (func_150ADA20() % 5U) + 4;
            extra.unk7 = (func_150ADA20() % 5U) + 4;
            extra.unk8 = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
            extra.unkC = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.unk0;
            extra.unk1A = (func_150ADA20() & 3) + 2;
            extra.unk1B = (func_150ADA20() & 3) + 2;
            extra.unk1C = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.unk0;
            extra.unk20 = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.unk0;
            params.unk54 = ((func_150ADA68() * 1005.0f) + 600.0f) * 0.001000000047f;

            temp_v0 = func_151303BC(&params, 2, 0x28);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0xA8, &extra, 0x28);
            }
            sub->unk10 -= 1.0f;
        } while (sub->unk10 > 1.0f);
        }
    }
}

extern void func_1515572C(s16 *, s32);

void func_1515B5F4(s16 arg0) {
    s16 sp1C[2];

    sp1C[0] = arg0;
    func_1515572C(sp1C, 0xB);
}

void func_1515B62C(struct102 *arg0, s16 *arg1, u8 arg2) {
    if (arg2 == 0xB) {
        if (*(s16 *)((u8 *)arg0 + 0x70) == arg1[0]) {
            func_1516972C(arg0);
        }
    }
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
    /* 0x45 */ u8  pad45[0x17];
} struct_1515B674;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  pad14[8];
} struct_1515B674b;

extern u8 (*D_8008FD04[])(void);
extern s32 func_151D8E20(void);
extern void *func_1515548C(struct_1515B674 *, s32, s32, s32, s32, u8, s32);

void func_1515B674(s16 arg0) {
    struct_1515B674 sp9C;
    struct_1515B674b sp80;
    void *temp_v0;
    s16 i;

    i = (func_150ADA20() % 1U) + 10;
    sp9C.unk14 = 0x19;
    sp9C.unk1A = 1;
    sp9C.unk16 = 0x14;
    sp9C.unk18 = 0xC;
    sp9C.unk1B = 0xFF;
    sp9C.unk1C = 0xFF;
    sp9C.unk1D = 0xFF;
    sp9C.unk1F = 0xFF;
    sp80.unk0 = arg0;
    sp9C.unk44 = arg0;
    sp9C.unk20 = 0xFF;
    sp9C.unk21 = 0xFF;
    sp9C.unk22 = 0xFF;
    sp9C.unk23 = 0xFF;
    sp9C.unk24 = 0;
    sp9C.unk28 = 0;
    sp9C.unk2C = 0;
    sp9C.unk40 = 0;
    sp9C.unk41 = 9;
    sp9C.unk30 = 7;
    sp9C.unk34 = 0x3C;
    sp9C.unk38 = 0x80;
    sp9C.unk3C = 0x20;
    while (i--) {
        sp80.unk4 = ((func_150ADA68() * 105.0f) + 105.0f) * 0.001000000047f;
        sp80.unk8 = func_150ADA68() * 3008.0f * 0.001000000047f;
        sp9C.unkC = sp9C.unk8 = sp80.unkC = ((func_150ADA68() * 1579.0f) + 489.0f) * 0.009999999776f;
        sp80.unk10 = ((func_150ADA68() * 301.0f) + 94.0f) * 0.009999999776f;
        sp9C.unk0 = (func_150ADA68() * 200.0f) + -100.0f;
        sp9C.unk4 = (func_150ADA68() * 148.0f) + -100.0f;
        sp9C.unk10 = D_8008FD04[func_151D8E20()]();
        sp9C.unk12 = (func_150ADA20() % 51U) + 0x32;
        sp9C.unk1E = (func_150ADA20() % 101U) + 0x64;
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 2;
        }
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 4;
        }
        temp_v0 = func_1515548C(&sp9C, 2, 0, 0, 0x18, 0xFF, 1);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x70, &sp80, 0x18);
        }
    }
}

typedef struct Obj1515B994 {
    char pad0[0x14];
    /* 0x14 */ f32 unk14;
    char pad18[4];
    /* 0x1C */ f32 unk1C;
    char pad20[0x74 - 0x20];
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
} Obj1515B994;

s32 func_1515B994(void *arg0) {
    typedef struct {
        u8 pad0[0x14];
        f32 x;
        u8 pad18[4];
        f32 z;
        u8 pad20[0x54];
        f32 acceleration;
        f32 velocity;
        f32 base;
        f32 scale;
    } Motion;
    Motion *motion = arg0;
    f32 previous;

    previous = motion->velocity;
    motion->x += previous * D_800BE9A4 + 0.5f * motion->acceleration * D_800BE9A4;
    motion->velocity = motion->velocity + motion->acceleration * D_800BE9A4;
    motion->z = motion->base + motion->scale * (motion->velocity + previous) * 0.5f;
    return 1;
}

void func_1515BA10(s32 arg0) {

}

extern void func_1515AF90(s16);

void func_1515BA1C(s16 arg0) {
    func_1515AF90(arg0);
}

void func_1515BA48(s32 arg0) {
}

extern void func_1515B674(s16);

void func_1515BA54(s16 arg0) {
    func_1515B674(arg0);
}

extern void func_1515B5F4(s16);

void func_1515BA80(s16 arg0) {
    func_1515B5F4(arg0);
}

void func_1515BAAC(s16 arg0) {
    func_1515B5F4(arg0);
}
