#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15143794(s32, s32, f32, f32 *);
s32 func_1510F8CC(s32);
void func_15152B38(void *, u8, s32);
s32 func_1513170C(s32 *, s32);
struct Obj151A2960;
extern void (*D_8008F8E0[])(struct Obj151A2960 *, s32);
extern f32 D_800A8D3C;
extern f32 D_800A8D40;
extern f32 D_800A8D44;
extern f32 D_800A8D30;
extern f32 D_800A8D34;
extern f32 D_800A8D38;
void func_151A11CC(f32, f32, f32, f32, f32, s32, s32, s32,
                   s32, s32, f32, s32, s32, s32, s32, s32,
                   s32, f32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32);


extern f32 D_800A8CE0;
extern f32 D_800A8CE4;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

f32 fabsf(f32);
#pragma intrinsic(fabsf)
s32 func_1519EF90(u8 *arg0, u8 arg1, u8 *arg2);

struct Sub1519F108 {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct Obj1519F108 {
    u8 pad0[0x20];
    s32 unk20;
    u8 pad24[0x74];
    struct Sub1519F108 **unk98;
};

void func_151478F4(struct Obj1519F108 *);
void func_1519F108(struct Obj1519F108 *arg0);

void func_15147928(struct Obj1519F108 *);
void func_1519F168(struct Obj1519F108 *arg0);

s32 *func_1519F1C8();

struct Sub1519F3B8 {
    s32 *unk0;
    s32 unk4;
    s32 *unk8;
    s32 unkC;
};

struct Obj1519F3B8 {
    u8 pad0[0x58];
    struct Sub1519F3B8 unk58;
};
void func_1519F3B8(struct Obj1519F3B8 *arg0);

struct Obj1519F400 {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
};

void func_1519F48C();
void func_151A0928();
void func_1519F400(struct Obj1519F400 *arg0);

struct Sub1519F48C {
    u8 pad0[0x58];
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct Ref1519F48C {
    struct Sub1519F48C *unk0;
    u8 unk4;
};

struct Obj1519F48C {
    u8 pad0[0x1E];
    u16 unk1E;
    s32 unk20;
    u8 pad24[0xC];
    u8 unk30;
    u8 pad31[0x67];
    struct Ref1519F48C *unk98;
};
void func_1519F48C(struct Obj1519F48C *arg0);

typedef struct Vec3f_1519F4F0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_1519F4F0;

typedef struct Config_1519F4F0 {
    void *owner;
    u8 type;
    u8 pad5[3];
    f32 field8;
    f32 fieldC;
    Vec3f_1519F4F0 position;
    f32 field1C;
    f32 field20;
    f32 field24;
    s16 field28;
    u8 color[3];
    u8 pad2D;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    u8 pad36[2];
    f32 field38;
    f32 field3C;
    u8 field40;
    u8 field41;
    u8 field42;
    u8 field43;
    u8 field44;
    u8 field45;
    s16 field46;
    s16 field48;
    s16 field4A;
    f32 field4C;
} Config_1519F4F0;

typedef struct TrailPoint_1519F4F0 {
    f32 x, z, y, fieldC;
    s16 size, delay, alpha;
    u8 pad16[2];
    s16 life;
    u8 pad1A[2];
    f32 velocityX, velocityZ;
} TrailPoint_1519F4F0;

s32 func_15045800(Vec3f_1519F4F0 *, u16, f32, void *);
extern f32 D_800BE9A4;
extern s32 D_800BE9E4;
s32 func_1519F4F0(u8 *arg0);

typedef struct Node151A084CData {
    s32 field0;
    u8 pad4[4];
    s32 field8;
} Node151A084CData;

typedef struct Node151A084C {
    u8 pad0[0x18];
    void *field18;
    u8 field1C;
    u8 pad1D[0x3B];
    Node151A084CData data58;
} Node151A084C;
void func_151A084C(u8 *arg0);

struct Obj151A0928 {
    u8 pad0[0x28];
    s32 *unk28;
    u8 unk2C;
};
void func_151A0928(struct Obj151A0928 *arg0);

struct Sub151A0950 {
    u8 pad0[0x18];
    s32 unk18;
    u8 unk1C;
};

struct Obj151A0950 {
    u8 pad0[0x98];
    struct Sub151A0950 **unk98;
};

struct Arg151A0950 {
    s32 unk0;
    u8 unk4;
};
void func_151A0950(struct Obj151A0950 *arg0, struct Arg151A0950 *arg1, u8 arg2);

struct Sub151A09B4 {
    u8 pad0[0x3B];
    u8 unk3B;
};

struct Mid151A09B4 {
    u8 pad0[0x18];
    struct Sub151A09B4 *unk18;
};

struct Obj151A09B4 {
    u8 pad0[0x28];
    struct Mid151A09B4 *unk28;
};

struct Arg151A09B4 {
    struct Sub151A09B4 *unk0;
    u8 unk4;
};
void func_151A09B4(struct Obj151A09B4 *arg0, struct Arg151A09B4 *arg1, u8 arg2);

struct Obj151A0A10 {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x1C - 0x18];
    f32 unk1C;
    u8 pad20[0x180 - 0x20];
    f32 unk180;
    s32 unk184;
};

struct Payload151A0A10 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 unkC;
    u8 padD[0x10 - 0xD];
    f32 unk10;
};
void func_151A0A10(struct Obj151A0A10 *arg0, s32 arg1, u8 arg2, s32 arg3);

extern f32 D_800A8D10;
extern f32 D_800BE9A4;
extern f32 func_150ADA68(void);
void func_1514C678(f32, f32, s32, f32, s32, s32, s32, s32, s32, f32, s32, s32);

struct Sub151A0AF8 {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    u8 unkC;
    u8 padD[0x10 - 0xD];
    f32 unk10;
};

struct Obj151A0AF8 {
    u8 pad0[0xC];
    u8 unkC;
    u8 padD[0x28 - 0xD];
    struct Sub151A0AF8 unk28;
};
void func_151A0AF8(struct Obj151A0AF8 *arg0);

void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27);

void func_151A0E40(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
    func_151A11CC(
        arg0, arg1, arg2, 163.0f, 2.019f, 0xA0, 0x4B, 0x41F, 0x4F6,
        1, 2.019f, 0x87, 0x44, 0x41F, 0x4F6, 2, 1, 2.019f, 0x46,
        0x42, 0x41F, 0x4F6, 7, 0x41, 4, 1, arg3, arg4);
}

void func_151A0F28(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
    func_151A11CC(
        arg0, arg1, arg2, 163.0f, 2.019f, 0xA0, 0x4B, 0x41F, 0x4F6,
        1, 2.019f, 0x87, 0x44, 0x41F, 0x4F6, 2, 1, 2.019f, 0x46,
        0x42, 0x41F, 0x4F6, 7, 0x41, 4, 1, arg3, arg4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1CE2F0/func_151A1010.s")

void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27) {
}

/*BEGIN151A11E4*/
typedef struct {
    void *unk0;
    s32 unk4;
    u8 pad8[4];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 pad1A[2];
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    s8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 unk2A;
    u8 pad2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 pad33;
    u16 unk34;
    u8 pad36[2];
} Packet151A11E4;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Extra151A11E4;

struct Sub151A11E4 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
};

struct Obj151A11E4 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x28 - 0xD];
    struct Sub151A11E4 unk28;
};

extern u8 D_80090B60[];
extern u8 D_80090D34;
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_151A11E4(struct Obj151A11E4 *arg0) {
    struct Sub151A11E4 *p;
    Packet151A11E4 packet;
    Extra151A11E4 extra;
    u8 r1;
    u8 r2;
    f32 cos1;
    f32 sin1;
    f32 cos2;
    f32 sin2;
    f32 speed;
    f32 horiz;
    f32 tz;
    f32 ty;
    f32 tx;
    s16 fade;
    s16 life;
    f32 inv;
    f32 vx;
    void *ret;

    arg0->unk28.unk10 += arg0->unk28.unk14 * D_800BE9A4;
    p = &arg0->unk28;
    if (1.0f < arg0->unk28.unk10) {
        packet.unk2C = 0xFF;
        packet.unk2D = 0xFF;
        packet.unk2E = 0xFF;
        packet.unk30 = 0;
        packet.unk31 = 0;
        packet.unk32 = 0;
        packet.unk1F = 0x10;
        packet.unk34 = 0x120;
        packet.unk4 = 0xC0001;
        packet.unkC = 0;
        packet.unk0 = &D_80090D34;
        packet.unk16 = 0;
        packet.unk20 = 0;
        packet.unk18 = 0;
        packet.unk22 = 0;
        packet.unk2A = 0;
        packet.unk2F = 0;
        packet.unk26 = 0x7D0;
        packet.unk24 = 0x7D0;
        do {
            r1 = func_150ADA20() & 0xFF;
            r2 = func_150ADA20() & 0xFF;
            cos1 = func_151423D8(r1 - 0x40);
            sin1 = func_151423D8(r1);
            cos2 = func_151423D8(r2 - 0x40);
            sin2 = func_151423D8(r2);
            speed = func_150ADA68() * p->unkC;
            horiz = speed * sin2;
            tx = p->unk0 + horiz * cos1;
            ty = p->unk4 - speed * cos2;
            tz = p->unk8 + horiz * sin1;
            if (func_150ADA20() & 1) {
                packet.unk34 |= 4;
            }
            if (func_150ADA20() & 1) {
                packet.unk34 |= 8;
            }
            packet.unkE = func_150ADA20() % (u32)(p->unk1A + 1) + p->unk18;
            fade = func_150ADA20() % (u32)(p->unk1E + 1) + p->unk1C;
            packet.unk28 = D_80090B60[0x1D8] * (0x100 / packet.unkE);
            life = packet.unk28;
            extra.unk8 = extra.unk2 = life;
            extra.unk4 = life - (life >> 2);
            extra.unkA = life >> 1;
            extra.unk0 = 0xFF / (extra.unk2 - extra.unk4);
            extra.unk6 = fade / (extra.unk8 - extra.unkA);
            extra.unk1A = life >> 2;
            extra.unk1C = 0xFF / extra.unk1A;
            extra.unk1E = life >> 1;
            extra.unk20 = fade / extra.unk1E;
            packet.unk10 = p->unk0;
            packet.unk12 = p->unk4;
            packet.unk14 = p->unk8;
            packet.unk1C = (s32)(p->unk0 * 256.0f) & 0xFF;
            packet.unk1E = (s32)(p->unk4 * 256.0f) & 0xFF;
            packet.unk1D = (s32)(p->unk8 * 256.0f) & 0xFF;
            extra.unkC = life >> 1;
            inv = 1.0f / extra.unkC;
            vx = (tx - p->unk0) * inv;
            do { } while (0);
            horiz = (ty - p->unk4) * inv;
            speed = (tz - p->unk8) * inv;
            extra.unkE = (s32)vx;
            extra.unk10 = (s32)horiz;
            extra.unk12 = (s32)speed;
            extra.unk14 = (u8)(s32)(vx * 256.0f);
            extra.unk16 = (u8)(s32)(horiz * 256.0f);
            extra.unk18 = (u8)(s32)(speed * 256.0f);
            ret = (void *)func_15167D84(&packet, 0, 0x22, -1, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x50, &extra, 0x22);
            }
            p->unk10 -= 1.0f;
            { s32 q; }
        } while (1.0f < p->unk10);
    }
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Sub151A175C;

typedef struct {
    u8 pad0[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0x6];
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F[0x5];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 pad40[0x10];
    Sub151A175C unk50;
} Obj151A175C;

void func_151A175C(Obj151A175C *arg0) {
    Sub151A175C *s = &arg0->unk50;

    if (arg0->unk38 > s->unk4) {
        arg0->unk3F = (s->unk2 - arg0->unk38) * s->unk0;
    }
    if (arg0->unk38 > s->unkA) {
        arg0->unk34 = arg0->unk36 = (s->unk8 - arg0->unk38) * s->unk6;
    }
    if (arg0->unk38 > s->unkC) {
        s32 px, py, pz, vx, vy, vz;

        px = arg0->unk20;
        px = (px << 8) | arg0->unk2C;
        vx = s->unkE;
        vx = (vx << 8) | s->unk14;
        px += vx * D_800BE9E4;
        arg0->unk20 = px >> 8;
        arg0->unk2C = px & 0xFF;
        py = arg0->unk22;
        py = (py << 8) | arg0->unk2E;
        vy = s->unk10;
        vy = (vy << 8) | s->unk16;
        py += vy * D_800BE9E4;
        arg0->unk22 = py >> 8;
        arg0->unk2E = py & 0xFF;
        pz = arg0->unk24;
        pz = (pz << 8) | arg0->unk2D;
        vz = s->unk12;
        vz = (vz << 8) | s->unk18;
        pz += vz * D_800BE9E4;
        arg0->unk24 = pz >> 8;
        arg0->unk2D = pz & 0xFF;

    }
    if (arg0->unk38 < s->unk1A) {
        arg0->unk3F = arg0->unk38 * s->unk1C;
    }
    if (arg0->unk38 < s->unk1E) {
        arg0->unk34 = arg0->unk36 = arg0->unk38 * s->unk20;
    }
}

struct Src151A18DC {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
};

struct Blk151A18DC {
    s32 a;
    s32 b;
    s32 c;
};

struct Dst151A18DC {
    struct Blk151A18DC unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s32 pad20;
};

void func_151A18DC(struct Src151A18DC *arg0) {
    struct Dst151A18DC sp34;
    struct260 *temp_v0;

    sp34.unk0 = *(struct Blk151A18DC *)&arg0->unk28;
    sp34.unkC = arg0->unk34;
    sp34.unk10 = 0.0f;
    sp34.unk14 = arg0->unk38;
    sp34.unk18 = arg0->unk3E;
    sp34.unk1A = arg0->unk40;
    sp34.unk1C = arg0->unk42;
    sp34.unk1E = arg0->unk44;

    temp_v0 = func_151491F4(arg0->unk3C, -1, 3, 1, 0, 0x20, arg0->unkC, arg0->unk1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x28, &sp34, 0x20);
    }
}

typedef struct {
    void *texture;
    s32 unk4;
    s32 unk8;
    s16 frame;
    s16 frame_rate;
    s16 position[3];
    s16 velocity_x;
    s16 velocity_z;
    char pad1A[2];
    u8 fraction_x;
    u8 fraction_y;
    u8 fraction_z;
    s8 callback;
    s16 velocity_y;
    s16 acceleration;
    s16 width;
    s16 height;
    s16 lifetime;
    s8 fade;
    char pad2B;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 environment[3];
    char pad33;
    u16 flags;
    char pad36[2];
} Emission151A1998;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Extra151A1998;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 unk10;
    f32 unk14;
    s16 rate;
    s16 rate_range;
    s16 unk1C;
    s16 unk1E;
} Sub151A1998;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x28 - 0xD];
    Sub151A1998 unk28;
} Obj151A1998;

extern u8 D_80090B60[];
extern u8 D_80090D34;
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_151A1998(Obj151A1998 *arg0) {
    Sub151A1998 *p;
    Emission151A1998 packet;
    Extra151A1998 extra;
    f32 cosine1;
    f32 sine1;
    f32 cosine2;
    f32 sine2;
    f32 r;
    f32 x;
    f32 y;
    f32 z;
    u8 angle1;
    u8 angle2;
    s16 t;
    s16 life;
    void *ret;

    arg0->unk28.unk10 += arg0->unk28.unk14 * D_800BE9A4;
    if (1.0f < arg0->unk28.unk10) {
        p = &arg0->unk28;
        packet.red = 0xFF;
        packet.green = 0xFF;
        packet.blue = 0xFF;
        packet.environment[0] = 0;
        packet.environment[1] = 0;
        packet.environment[2] = 0;
        packet.callback = 0x11;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.texture = &D_80090D34;
        packet.velocity_x = 0;
        packet.velocity_y = 0;
        packet.velocity_z = 0;
        packet.acceleration = 0;
        packet.fade = 0;
        packet.alpha = 0;
        packet.width = packet.height = 0x7D0;
        do {
            angle1 = func_150ADA20() & 255;
            angle2 = func_150ADA20() & 255;
            cosine1 = func_151423D8(angle1 - 64);
            sine1 = func_151423D8(angle1);
            cosine2 = func_151423D8(angle2 - 64);
            sine2 = func_151423D8(angle2);
            r = func_150ADA68() * p->radius;
            x = p->x + r * sine2 * cosine1;
            y = p->y - r * cosine2;
            z = p->z + r * sine2 * sine1;
            if (func_150ADA20() & 1) {
                packet.flags |= 4;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= 8;
            }
            packet.frame_rate = func_150ADA20() % (u32)(p->rate_range + 1) + p->rate;
            t = func_150ADA20() % (u32)(p->unk1E + 1) + p->unk1C;
            life = packet.lifetime = D_80090B60[0x1D8] * (0x100 / packet.frame_rate);
            extra.unk2 = life;
            extra.unk4 = life - (life >> 2);
            extra.unk0 = 0xFF / (extra.unk2 - extra.unk4);
            extra.unk8 = life;
            extra.unkA = life >> 1;
            extra.unk6 = t / (extra.unk8 - extra.unkA);
            extra.unkC = life >> 2;
            extra.unkE = 0xFF / extra.unkC;
            extra.unk10 = life >> 1;
            extra.unk12 = t / extra.unk10;
            packet.position[0] = x;
            packet.position[1] = y;
            packet.position[2] = z;
            packet.fraction_x = ((s32)(x * 256.0f)) & 0xFF;
            packet.fraction_z = ((s32)(y * 256.0f)) & 0xFF;
            packet.fraction_y = ((s32)(z * 256.0f)) & 0xFF;
            ret = (void *)func_15167D84(&packet, 0, 0x14, -1, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0x50, &extra, 0x14);
            }
            p->unk10 -= 1.0f;
        } while (1.0f < p->unk10);
    }
}

struct Sub151A1E34 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
};

struct Obj151A1E34 {
    u8 pad0[0x34];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x3F - 0x3A];
    u8 unk3F;
    u8 pad40[0x50 - 0x40];
    struct Sub151A1E34 unk50;
};

void func_151A1E34(struct Obj151A1E34 *arg0) {
    s16 temp_v1;
    struct Sub151A1E34 *temp_v0;

    temp_v0 = &arg0->unk50;
    temp_v1 = arg0->unk38;
    if (arg0->unk50.unk4 < temp_v1) {
        arg0->unk3F = (u32)(temp_v0->unk2 - temp_v1) * (u32)temp_v0->unk0;
    }
    if (temp_v0->unkA < temp_v1) {
        arg0->unk34 = arg0->unk36 = (u32)(temp_v0->unk8 - temp_v1) * (u32)temp_v0->unk6;
    }
    if (temp_v1 < temp_v0->unkC) {
        arg0->unk3F = (u32)temp_v1 * (u32)temp_v0->unkE;
    }
    if (temp_v1 < temp_v0->unk10) {
        arg0->unk34 = arg0->unk36 = (u32)temp_v1 * (u32)temp_v0->unk12;
    }
}

struct Src151A1EE8 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
};

struct Blk3151A1EE8 {
    s32 a;
    s32 b;
    s32 c;
};

struct Dst151A1EE8 {
    struct Blk3151A1EE8 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    union {
        struct {
            s16 unk18;
            s16 unk1A;
            s16 unk1C;
            s16 unk1E;
        } s;
        f64 _align;
    } u18;
    s16 unk20;
    s16 unk22;
};

void func_151A1EE8(struct Src151A1EE8 *arg0) {
    struct Dst151A1EE8 sp38;
    struct260 *temp_v0;

    sp38.unk0 = *(struct Blk3151A1EE8 *)&arg0->unk28;
    sp38.unkC = arg0->unk34;
    sp38.unk10 = 0.0f;
    sp38.unk14 = arg0->unk38;
    sp38.u18.s.unk18 = arg0->unk3E;
    sp38.u18.s.unk1A = arg0->unk40;
    sp38.u18.s.unk1C = arg0->unk42;
    sp38.u18.s.unk1E = arg0->unk44;
    sp38.unk20 = arg0->unk46;
    sp38.unk22 = arg0->unk48;

    temp_v0 = func_151491F4(arg0->unk3C, -1, 4, 1, 0, 0x24, arg0->unkC, arg0->unk1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x28, &sp38, 0x24);
    }
}

struct Sub151A1FB4 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
};

struct Obj151A1FB4 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    struct Sub151A1FB4 unk28;
};

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A1FB4;

typedef struct {
    void *unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    u8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u16 unk34;
    s16 unk36;
} Packet151A1FB4;

extern u8 D_80090B60[];
extern u8 D_80090D34;
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_151A1FB4(struct Obj151A1FB4 *arg0) {
    struct Sub151A1FB4 *sub;
    Packet151A1FB4 pkt;
    Fade151A1FB4 fade;
    u8 a;
    u8 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    s16 n;
    s32 ret;

    arg0->unk28.unk10 += arg0->unk28.unk14 * D_800BE9A4;
    sub = &arg0->unk28;
    if (arg0->unk28.unk10 > 1.0f) {
        pkt.unk2C = 5;
        pkt.unk2D = 0;
        pkt.unk2E = 0;
        pkt.unk30 = 0;
        pkt.unk31 = 0;
        pkt.unk32 = 0;
        pkt.unk1F = 0x12;
        pkt.unk34 = 0x120;
        pkt.unk04 = 0xC0001;
        pkt.unk0C = 0;
        pkt.unk00 = &D_80090D34;
        pkt.unk16 = 0;
        pkt.unk20 = 0;
        pkt.unk18 = 0;
        pkt.unk22 = 0;
        pkt.unk2A = 0;
        pkt.unk2F = 0;
        pkt.unk26 = 0x7D0;
        pkt.unk24 = 0x7D0;
        do {
            a = func_150ADA20() & 0xFF;
            b = func_150ADA20() & 0xFF;
            cosA = func_151423D8(a - 0x40);
            sinA = func_151423D8(a);
            cosB = func_151423D8(b - 0x40);
            sinB = func_151423D8(b);
            r = func_150ADA68() * sub->unkC;
            x = sub->unk0 + r * sinB * cosA;
            y = sub->unk4 - r * cosB;
            z = sub->unk8 + r * sinB * sinA;
            if (func_150ADA20() & 1) {
                pkt.unk34 |= 4;
            }
            if (func_150ADA20() & 1) {
                pkt.unk34 |= 8;
            }
            pkt.unk0E = (func_150ADA20() % (u32)(sub->unk1A + 1)) + sub->unk18;
            n = (func_150ADA20() % (u32)(sub->unk1E + 1)) + sub->unk1C;
            pkt.unk28 = D_80090B60[0x1D8] * (0x100 / pkt.unk0E);
            fade.unk2 = pkt.unk28;
            fade.unk8 = pkt.unk28;
            fade.unk4 = pkt.unk28 - (pkt.unk28 >> 2);
            fade.unk0 = 0xFF / (pkt.unk28 - fade.unk4);
            fade.unkA = pkt.unk28 >> 1;
            fade.unk6 = n / (pkt.unk28 - (s16)(pkt.unk28 >> 1));
            fade.unkC = pkt.unk28 >> 1;
            fade.unkE = 0xFF / (pkt.unk28 >> 1);
            fade.unk10 = pkt.unk28 >> 2;
            fade.unk12 = 0xFF / (pkt.unk28 >> 2);
            fade.unk14 = pkt.unk28 >> 1;
            fade.unk16 = (func_150ADA20() % (u32)(sub->unk22 + 1)) + sub->unk20;
            fade.unk18 = pkt.unk28 >> 1;
            fade.unk1A = (func_150ADA20() % 81U) + 0x50;
            pkt.unk10 = x;
            pkt.unk12 = y;
            pkt.unk14 = z;
            pkt.unk1C = ((s32)(x * 256.0f)) & 0xFF;
            pkt.unk1E = ((s32)(y * 256.0f)) & 0xFF;
            pkt.unk1D = ((s32)(z * 256.0f)) & 0xFF;
            ret = func_15167D84(&pkt, 0, 0x1C, -1, arg0->unkC, arg0->unk1);
            if (ret != 0) {
                memcpy((u8 *)ret + 0x50, &fade, 0x1C);
            }
            sub->unk10 -= 1.0f;
        } while (sub->unk10 > 1.0f);
    }
}

typedef struct Fade151A24A8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A24A8;

typedef struct Obj151A24A8 {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x2F - 0x18];
    u8 unk2F;
    u8 pad30[0x2];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43;
    u16 unk44;
    u8 pad46[0xA];
    Fade151A24A8 unk50;
} Obj151A24A8;

void func_151A24A8(Obj151A24A8 *arg0) {
    Fade151A24A8 *s = &arg0->unk50;

    if (arg0->unk38 > s->unk4) {
        arg0->unk3F = (s->unk2 - arg0->unk38) * s->unk0;
    }
    if (arg0->unk38 > s->unkA) {
        arg0->unk34 = arg0->unk36 = (s->unk8 - arg0->unk38) * s->unk6;
    }
    if (arg0->unk38 < s->unk10) {
        arg0->unk3F = arg0->unk38 * s->unk12;
    }
    if (arg0->unk38 < s->unk14) {
        s16 d = s->unk16 * D_800BE9E4;

        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unkC) {
        arg0->unk2F = 0x13;
        arg0->unk44 |= 0x101;
        arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unkE;
        arg0->unk14 = 0x520003;
    }
    if (arg0->unk38 < s->unk18) {
        arg0->unk32 = s->unk1A;
        s->unk18 = -9999;
    }
}

typedef struct Fade151A25E0 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A25E0;

typedef struct Obj151A25E0 {
    u8 pad0[0x32];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43[0xD];
    Fade151A25E0 unk50;
} Obj151A25E0;

void func_151A25E0(Obj151A25E0 *arg0) {
    Fade151A25E0 *s = &arg0->unk50;

    if (arg0->unk38 > s->unk4) {
        arg0->unk3F = (s->unk2 - arg0->unk38) * s->unk0;
    }
    if (arg0->unk38 > s->unkA) {
        arg0->unk34 = arg0->unk36 = (s->unk8 - arg0->unk38) * s->unk6;
    }
    if (arg0->unk38 < s->unk10) {
        arg0->unk3F = arg0->unk38 * s->unk12;
    }
    if (arg0->unk38 < s->unk14) {
        s16 d = s->unk16 * D_800BE9E4;

        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unk18) {
        arg0->unk32 = s->unk1A;
        s->unk18 = -9999;
    }
    arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unkE;
}

typedef struct {
    f32 x, y, z;
} Vec151A26EC;

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
    Vec151A26EC unk30;
    Vec151A26EC unk3C;
    Vec151A26EC unk48;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 pad66[0xA];
} Fx151A26EC;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s8 unkA;
} Extra151A26EC;

s32 func_151A26EC(Vec151A26EC *arg0, Vec151A26EC *arg1, Vec151A26EC *arg2, f32 arg3, f32 arg4, f32 arg5,
                  s16 arg6, u8 arg7, s16 arg8, s16 arg9, s16 argA, s8 argB, u8 argC, u8 argD, u8 argE,
                  u8 argF, s32 arg10) {
    Extra151A26EC sp9C;
    struct260 *ret;
    Fx151A26EC sp28;

    if (arg8 <= 0) {
        arg8 = 1;
    }
    if (arg9 <= 0) {
        arg9 = 1;
    }
    sp9C.unk0 = arg3;
    sp9C.unk4 = arg9;
    sp9C.unk6 = 0xFF / arg9;
    sp28.unk1E = arg8;
    sp28.unk20 = 0xFF / arg8;
    sp9C.unk8 = argA;
    sp9C.unkA = argB;
    sp28.unk1D = 0x27;
    sp28.unk08 = 0x5203;
    sp28.unk00 = 0x200005;
    sp28.unk04 = 0;
    sp28.unk0A = arg6;
    sp28.unk0C = 0;
    sp28.unk10 = 0;
    sp28.unk14 = argC;
    sp28.unk15 = argD;
    sp28.unk16 = argE;
    sp28.unk17 = 0xFF;
    sp28.unk18 = 0xFF;
    sp28.unk19 = 0xFF;
    sp28.unk1A = 0xFF;
    sp28.unk1B = arg7;
    sp28.unk1C = 0xFF;
    sp28.unk2C = arg5;
    sp28.unk28 = arg5;
    sp28.unk30 = *arg0;
    sp28.unk3C = *arg1;
    sp28.unk48 = *arg2;
    sp28.unk22 = 1;
    sp28.unk54 = arg4;
    sp28.unk24 = 1.0f;
    sp28.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (func_150ADA20() & 1 ? 0x80 : 0)) | 0x200 | 0x1000 | 0x4000 | 0x8000 | 0x800000;
    sp28.unk60 = 7;
    sp28.unk61 = 2;
    sp28.unk62 = 0x11;
    sp28.unk63 = -1;
    sp28.unk64 = -1;
    sp28.unk65 = 2;
    ret = func_15130374(&sp28, 1, 0x10, argF, arg10);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &sp9C.unk0, 8);
        memcpy((u8 *)ret + 0xB0, &sp9C.unk4, 8);
    }
    return 0;
}

struct Sub151A2960 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s8 unk6;
};

struct Obj151A2960 {
    u8 pad0[0x1A];
    s16 unk1A;
    u8 pad1C[0x2C - 0x1C];
    u8 unk2C;
    u8 pad2D[0xB0 - 0x2D];
    struct Sub151A2960 unkB0;
};

void func_151A2960(struct Obj151A2960 *arg0, s32 arg1) {
    struct Sub151A2960 *temp_v0;

    temp_v0 = &arg0->unkB0;
    if (arg0->unk1A < temp_v0->unk0) {
        arg0->unk2C = arg0->unk1A * temp_v0->unk2;
    }
    if (D_800BE616 == 0) {
        if (-1 != temp_v0->unk4) {
            if ((-1 != temp_v0->unk6) && (temp_v0->unk4 >= arg0->unk1A)) {
                D_8008F8E0[temp_v0->unk6](arg0, temp_v0->unk4);
                temp_v0->unk4 = -1;
            }
        }
    }
    func_1513170C((s32 *)arg0, arg1);
}

void func_151A2C24(s32, s32, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32, s32);

void func_151A2A14(s32 a0, s16 a1, s16 a2, f32 a3, f32 p4, f32 p5, s32 p6, f32 p7, f32 p8, f32 p9, f32 p10, s16 p11, s16 p12, s16 p13, s16 p14, s16 p15, s16 p16, s8 p17, u8 p18, s32 p19) {
    func_151A2C24(a0, 0, a1, a2, a3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, 1);
}

void func_151A2AD4(s32 a0, s32 a1, f32 a2, f32 a3, s32 p4, f32 p5, f32 p6, f32 p7, f32 p8, s16 p9, s16 p10, s16 p11, s16 p12, s16 p13, s16 p14, s8 p15, u8 p16, s32 p17) {
    func_151A2C24(a0, a1, 0, 0, 0.0f, a2, a3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 0);
}

void func_151A2B84(f32 *in, s32 a1, s32 a2, s32 a3, f32 t, f32 *out) {
    f32 s = 1.0f - t;
    out[0] = in[0] * s;
    out[1] = in[1] * s;
    out[2] = in[2] * s;
}

void func_151A2BD0(f32 *in, s32 a1, s32 a2, f32 a3, f32 t, f32 *out) {
    func_15143794((s16) a1, (s16) a2, (1.0f - t) * a3, out);
}

typedef struct {
    u8 c[4][3];
} Col151A2C24;
extern Col151A2C24 D_8008F8E8;

void func_151A2C24(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, s16 argC, s16 argD, s16 argE, s16 argF, s16 arg10, s16 arg11, s8 arg12, u8 arg13, s32 arg14, s32 arg15) {
    s16 minLife;
    s16 delay;
    f32 t;
    f32 step;
    void (*fn)(s32, s32, s32, f32, f32, f32 *);
    f32 pos[3];
    f32 vel[3];
    s32 c;
    Col151A2C24 colors;

    if ((u8) arg15 == 0) {
        fn = (void (*)(s32, s32, s32, f32, f32, f32 *)) func_151A2B84;
    } else {
        fn = (void (*)(s32, s32, s32, f32, f32, f32 *)) func_151A2BD0;
    }
    if (arg10 < arg11) {
        minLife = arg10;
    } else {
        minLife = arg11;
    }
    t = 0.0f;
    step = 1.0f / (f32) arg7;
    while (arg7 != 0) {
        f32 r;
        s32 a;
        s32 b;

        delay = (func_150ADA20() % (u32) (argD + 1)) + argC;
        a = func_150ADA20();
        b = func_150ADA20();
        func_15143794((s16) (a & 0xFF), (s16) (b & 0xFF), func_150ADA68() * arg8, pos);
        fn(arg1, arg2, arg3, arg4, t, vel);
        colors = D_8008F8E8;
        c = func_150ADA20() & 3;
        r = func_150ADA68();
        a = func_150ADA20();
        if (func_150ADA68() < 0.0802000016f) {
            b = 0x19;
        } else {
            b = -1;
        }
        func_151A26EC((Vec151A26EC *) arg0, (Vec151A26EC *) pos, (Vec151A26EC *) vel, arg9, (r * argB) + argA, (arg6 * t) + arg5, delay + minLife, (a % (u32) (argF + 1)) + argE, arg10, arg11, b, arg12, colors.c[c][0], colors.c[c][1], colors.c[c][2], arg13, arg14);
        t += step;
        arg7--;
    }
}

void func_151A2F0C(void *arg0) {
    struct Vec151A2F0C {
        f32 a;
        f32 b;
        f32 c;
    };
    struct Obj151A2F0C {
        u8 pad0[0x1];
        u8 unk1;
        u8 pad2[0xC - 0x2];
        u8 unkC;
        u8 padD[0x38 - 0xD];
        f32 unk38;
        f32 unk3C;
        struct Vec151A2F0C unk40;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
        u8 pad58[0x68 - 0x58];
        s32 unk68;
    };
    struct Arg151A2F0C {
        s32 unk00;
        s32 unk04;
        struct Vec151A2F0C unk08;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        s16 unk2C;
        s16 unk2E;
        s16 unk30;
        s16 unk32;
        s32 unk34;
        s32 unk38;
        s16 unk3C;
        s16 unk3E;
        s16 unk40;
        u8 unk42;
        u8 unk43;
        u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        u8 unk48;
        u8 unk49;
        u8 unk4A;
        u8 unk4B;
        u8 unk4C;
        u8 unk4D;
        u8 unk4E;
        u8 unk4F;
        u8 unk50;
        u8 unk51;
        u8 unk52;
        u8 unk53;
        u8 unk54;
        u8 unk55;
        u8 unk56;
        u8 unk57;
        u8 unk58;
        u8 pad59[3];
        s32 unk5C;
        s32 unk60;
        s16 unk64;
        s16 unk66;
        s16 unk68;
        u8 unk6A;
        u8 pad6B;
        f32 unk6C;
        s8 unk70;
        s8 unk71;
        s8 unk72;
        s8 unk73;
    };
    struct Arg151A2F0C sp1C;
    struct Obj151A2F0C *obj;

    obj = arg0;
    sp1C.unk00 = 8;
    sp1C.unk04 = 6;
    sp1C.unk08 = obj->unk40;
    if (obj->unk68 & 0x1000) {
        sp1C.unk08.a += obj->unk4C;
        sp1C.unk08.b += obj->unk50;
        sp1C.unk08.c += obj->unk54;
    }
    sp1C.unk14 = ((obj->unk38 + obj->unk3C) * 0.5f) * D_800A8D30;
    sp1C.unk18 = ((obj->unk38 + obj->unk3C) * 0.5f) * D_800A8D34;
    sp1C.unk1C = D_800A8D38;
    sp1C.unk20 = 0.0f;
    sp1C.unk24 = 8.0f;
    sp1C.unk28 = 10.0f;
    sp1C.unk2C = 0;
    sp1C.unk2E = 0xFF;
    sp1C.unk30 = -0x40;
    sp1C.unk32 = 0x56;
    sp1C.unk34 = 3;
    sp1C.unk38 = 1;
    sp1C.unk3C = 0x11;
    sp1C.unk3E = 0x12;
    sp1C.unk40 = 1;
    sp1C.unk42 = 4;
    sp1C.unk43 = 2;
    sp1C.unk44 = 3;
    sp1C.unk45 = 0xFF;
    sp1C.unk46 = 0xC8;
    sp1C.unk47 = 0xC8;
    sp1C.unk48 = 0xFF;
    sp1C.unk49 = 0;
    sp1C.unk4A = 0x37;
    sp1C.unk4B = 0x37;
    sp1C.unk4C = 0;
    sp1C.unk4D = 0xFF;
    sp1C.unk4E = 0xFF;
    sp1C.unk4F = 0xFF;
    sp1C.unk50 = 0xFF;
    sp1C.unk51 = 0;
    sp1C.unk52 = 0;
    sp1C.unk53 = 0;
    sp1C.unk54 = 0;
    sp1C.unk55 = 0xFF;
    sp1C.unk56 = 0;
    sp1C.unk57 = 1;
    sp1C.unk58 = 0x24;
    sp1C.unk5C = 0x200005;
    sp1C.unk60 = 0x60600;
    sp1C.unk64 = 7;
    sp1C.unk66 = 0x24;
    sp1C.unk68 = 1;
    sp1C.unk6A = 0;
    sp1C.unk6C = 1.0f;
    sp1C.unk70 = -1;
    sp1C.unk71 = 0;
    sp1C.unk72 = -1;
    sp1C.unk73 = -1;

    func_15152B38(&sp1C, obj->unkC, obj->unk1);
}

typedef struct {
    /* 0x0 */ f32 a;
    /* 0x4 */ f32 b;
    /* 0x8 */ f32 c;
} Struct3W151A3150;

typedef struct Obj151A3150 {
    u8 pad0[0x1];
    u8 unk1;
    u8 pad2[0xC - 0x2];
    u8 unkC;
    u8 padD[0x38 - 0xD];
    f32 unk38;
    f32 unk3C;
    Struct3W151A3150 unk40;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    u8 pad58[0x68 - 0x58];
    s32 unk68;
} Obj151A3150;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ Struct3W151A3150 unk08;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ u8 unk4B;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ u8 unk52;
    /* 0x53 */ u8 unk53;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} Struct151A3150;
void func_151A3150(Obj151A3150 *arg0);
