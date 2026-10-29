#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr0
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_15048A40
extern f32 func_15048A40(u8);
#undef func_15169260
#include "variables.h"

void func_1518E308(void *);
void func_15169260(void *, s32, s32, u8);
extern f32 D_800A8004;
extern f32 D_800A7B68;
extern f32 D_800A7B6C;
extern f32 D_800A7B70;
extern f32 D_800A8018;
extern void func_151D5D60();
extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
struct Struct1519021CArg1;
extern void func_151D2AB0(s32);
extern s32 func_151D2F00(s32, s32, s32, s32);
extern struct225 *func_1519021C(s32, struct Struct1519021CArg1 *, u8, s16, u8, s32);

typedef struct Game1BBFC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game1BBFC0Vec3f;

typedef struct Game1BBFC0Vertex {
    s16 x, y, z, flag;
    u8 other[8];
} Game1BBFC0Vertex;

typedef struct Game1BBFC0Beam {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Game1BBFC0Vec3f start;
    Game1BBFC0Vec3f end;
    u8 unknown4C[0x74];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} Game1BBFC0Beam;

typedef struct Game1BBFC0Camera {
    u8 unknown0[0x2F8];
    Game1BBFC0Vec3f position;
    u8 unknown304[0x69C];
} Game1BBFC0Camera;

void *memcpy(void *, const void *, u32);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

struct SoundLinkedPayload {
    u8 pad_0x0[0x46];
    s16 field_0x46;
    u8 pad_0x48[0x3C];
    u8 field_0x84;
};

struct SoundLinkedObject {
    u8 pad_0x0[0x31C];
    struct SoundLinkedPayload *field_0x31C;
};

struct TimedSoundTrigger {
    s32 field_0x0;
    s32 field_0x4;
    struct127 *field_0x8;
};

struct TimedSoundObject {
    u8 pad_0x0[0x10];
    struct SoundLinkedObject *field_0x10;
    u8 pad_0x14[0x7C];
    struct TimedSoundTrigger field_0x90;
};

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
} Particle1518CD20; /* 0x70 */

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
extern void func_15145DB4(s32, struct17 *, struct17 *, s32);

void func_1518CD20(s32 arg0, u8 arg1, s32 arg2) {
    s16 count;
    Particle1518CD20 spawn;
    s16 remaining;
    struct17 *b;
    s16 i;
    f32 scale;
    struct17 *a;

    count = remaining = func_150ADA20() % 11U + 25;
    if (count > 0) {
        a = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        b = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        if (a == NULL || b == NULL) {
            return;
        }
        {
            for (i = 0; i < count; i++) {
                func_1514373C(func_150ADA68() * 3.141592741f + 3.141592741f, 114.0f, &a[i].unk0, &a[i].unk8);
                a[i].unk0 += 111.0f;
                a[i].unk4 = -90.0f;
            }
            i = 0;
            a[count].unk0 = 111.0f;
            a[count].unk4 = -90.0f;
            a[count].unk8 = 0.0f;
            func_15145DB4(arg0, a, b, count + 1);
            spawn.unk1D = 0x6C;
            spawn.unk8 = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk14 = 0xE7;
            spawn.unk15 = 0xE7;
            spawn.unk16 = 0xB6;
            spawn.unk18 = 0x9E;
            spawn.unk19 = 0x9E;
            spawn.unk1A = 0x62;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk1C = 0xFF;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            spawn.unk1E = 0x19;
            spawn.unk20 = 0xA;
            spawn.unk58 = 0x80CE07;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x10;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk24 = 1.040568948f;
            scale = 0.8533809781f;
            do {
                f32 speed;
                s32 flags;
                void *ret;

                spawn.unk58 &= ~0xC0;
                flags = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | flags;
                speed = (func_150ADA68() * 202.0f + 250.0f) * 0.001000000047f;
                spawn.unk22 = spawn.unkA = func_150ADA20() % 26U + 25;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 105.0f + 150.0f;
                spawn.pos = b[i];
                spawn.vel.unk0 = (b[i].unk0 - b[count].unk0) * speed;
                spawn.vel.unk4 = (b[i].unk4 - b[count].unk4) * speed;
                spawn.vel.unk8 = (b[i].unk8 - b[count].unk8) * speed;
                spawn.unk54 = (func_150ADA68() * 880.0f + 13.0f) * 0.001000000047f;
                spawn.unk1B = func_150ADA20() % 176U + 80;
                ret = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &scale, 4);
                }
                remaining--;
                i++;
            } while (remaining > 0);
        }
    }
}

void *func_1518E3C4(void *arg0, s32 arg1);

typedef struct Node1518D1C0 {
    u8 pad0[0x8];
    struct Node1518D1C0 *unk8;
    u8 padC[0xC];
    void *unk18;
    u8 unk1C;
} Node1518D1C0;

typedef struct Owner1518D1C0 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x18E];
    u8 unk1CA;
} Owner1518D1C0;

typedef struct Spawn1518D1C0 {
    void *owner;
    u8 unk4;
    s16 unk6;
    u8 unk8;
    s8 unk9;
    u8 unkA;
} Spawn1518D1C0;

extern u8 D_800DCE50[];

void *func_1518D1C0(Owner1518D1C0 *arg0, s16 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5, void *arg6) {
    Spawn1518D1C0 spawn;
    s32 i;
    Node1518D1C0 *node;

    if (arg0 == NULL) {
        return NULL;
    }
    if (arg3) {
        for (i = 0; i != 2; i++) {
            D_800DD190++;
            node = ((Node1518D1C0 *(*)[104])D_800DCE50)[i][33];
            while (node != NULL) {
                ((Node1518D1C0 **)D_800DD198)[D_800DD190] = node->unk8;
                if (arg0 == node->unk18 || node->unk1C == arg0->unk3B) {
                    func_1516972C((struct102 *)node);
                }
                node = ((Node1518D1C0 **)D_800DD198)[D_800DD190];
            }
            {
                extern s8 D_800DD190;

                D_800DD190--;
            }
        }
    }
    spawn.owner = arg0;
    spawn.unk4 = arg0->unk3B;
    switch (arg1) {
    case 10:
        spawn.unk6 = 75;
        spawn.unk8 = arg0->unk1CA ? 1 : 0;
        spawn.unk9 = 1;
        spawn.unkA = 7;
        return func_1518E3C4(&spawn, arg4);
    case 11:
        spawn.unk6 = 75;
        spawn.unk8 = arg0->unk1CA ? 1 : 0;
        spawn.unk9 = 0;
        spawn.unkA = 1;
        return func_1518E3C4(&spawn, arg4);
    case 12:
        spawn.unk6 = 75;
        spawn.unk8 = arg0->unk1CA ? 1 : 0;
        spawn.unk9 = 1;
        spawn.unkA = 7;
        return func_1518E3C4(&spawn, arg4);
    case 1:
        spawn.unk6 = 500;
        spawn.unk8 = 1;
        spawn.unk9 = 0;
        spawn.unkA = 1;
        return func_1518E3C4(&spawn, arg4);
    case 2:
        spawn.unk6 = 3000;
        spawn.unk8 = 1;
        spawn.unk9 = 1;
        spawn.unkA = 7;
        return func_1518E3C4(&spawn, arg4);
    case 3:
        spawn.unk6 = 200;
        spawn.unk8 = 1;
        spawn.unk9 = -1;
        spawn.unkA = 8;
        return func_1518E3C4(&spawn, arg4);
    case 4:
        spawn.unk6 = 200;
        spawn.unk8 = 1;
        spawn.unk9 = -1;
        spawn.unkA = 9;
        return func_1518E3C4(&spawn, arg4);
    case 5:
        spawn.unk6 = 3000;
        spawn.unk8 = 0;
        spawn.unk9 = -1;
        spawn.unkA = 2;
        return func_1518E3C4(&spawn, arg4);
    case 6:
        spawn.unk6 = 3000;
        spawn.unk8 = 0;
        spawn.unk9 = -1;
        spawn.unkA = 3;
        return func_1518E3C4(&spawn, arg4);
    case 7:
        spawn.unk6 = 3000;
        spawn.unk8 = 0;
        spawn.unk9 = -1;
        spawn.unkA = 4;
        return func_1518E3C4(&spawn, arg4);
    case 8:
        spawn.unk6 = 3000;
        spawn.unk8 = 0;
        spawn.unk9 = -1;
        spawn.unkA = 5;
        return func_1518E3C4(&spawn, arg4);
    case 9:
        spawn.unk6 = 3000;
        spawn.unk8 = 0;
        spawn.unk9 = -1;
        spawn.unkA = 6;
        return func_1518E3C4(&spawn, arg4);
    case 13:
        spawn.unk6 = arg2;
        spawn.unk8 = (arg2 == 999) ? 0 : 1;
        spawn.unk9 = -1;
        spawn.unkA = 10;
        return func_1518E3C4(&spawn, arg4);
    case 14:
        spawn.unk6 = arg2;
        spawn.unk8 = 1;
        spawn.unk9 = -1;
        spawn.unkA = 11;
        return func_1518E3C4(&spawn, arg4);
    case 15:
        spawn.unk6 = arg2;
        spawn.unk8 = (arg2 == 999) ? 0 : 1;
        spawn.unk9 = -1;
        spawn.unkA = 12;
        return func_1518E3C4(&spawn, arg4);
    case 16:
        spawn.unk6 = arg2;
        spawn.unk8 = (arg2 == 999) ? 0 : 1;
        spawn.unk9 = -1;
        spawn.unkA = 13;
        return func_1518E3C4(&spawn, arg4);
    case 17:
        spawn.unk6 = 300;
        spawn.unk8 = 1;
        spawn.unk9 = -1;
        spawn.unkA = 14;
        return func_1518E3C4(&spawn, arg4);
    case 18:
        spawn.unk6 = arg2;
        spawn.unk8 = 1;
        spawn.unk9 = -1;
        spawn.unkA = 15;
        return func_1518E3C4(&spawn, arg4);
    }
    return NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1BA1D0/func_1518D6E0.s")

struct Struct1518E298Inner {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Struct1518E298 {
    u8 pad0[2];
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[0x14];
    struct Struct1518E298Inner *unk1C;
};

s32 func_1518E298(struct Struct1518E298 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct Struct1518E298Inner *temp_v1;

    temp_v1 = arg0->unk1C;
    if (temp_v1 != 0) {
        if (temp_v1->unk0 != 0) {
            arg0->unk2 = temp_v1->unk14;
            arg0->unk4 = temp_v1->unk18;
            arg0->unk6 = temp_v1->unk1C;
            return 0;
        }
    }

    return 1;
}

struct Struct1518E308Entry {
    struct102 *unk0;
    u8 pad4[0x14];
};

void func_1518E308(void *arg0) {
    struct Struct1518E308Entry *var_s0;
    s32 var_s1;
    s32 sp28[2];

    var_s0 = (struct Struct1518E308Entry *)((char *)arg0 + 0x48);
    sp28[1] = (s32)var_s0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    var_s1 = 0;
    *(f32 *)((char *)arg0 + 0x24) = 0.0f;
    do {
        if (var_s0->unk0 != NULL) {
            func_1516972C(var_s0->unk0);
        }
        var_s1++;
        var_s0++;
    } while (var_s1 != 0x64);
    bzero((void *)sp28[1], 0x960);
}

void func_1518E37C(void *arg0, u8 arg1) {
    if (arg1 != *(u8 *)((char *)arg0 + 0x22)) {
        func_1518E308(arg0);
        *(u8 *)((char *)arg0 + 0x22) = arg1;
    }
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Struct1518E3C4 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0xC];
    f32 unk24;
    s32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[0x2];
    s32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
};

void *func_1518E3C4(void *arg0, s32 arg1) {
    struct Struct1518E3C4 *temp_v1;
    struct Struct1518E3C4 *sp24;

    if (*(u8 *)(*(s32 *)arg0 + 4) == 0x96) {
        return NULL;
    }
    temp_v1 = (struct Struct1518E3C4 *)func_15167A68(0x21, 1, 0x9EC, 1, (u8)arg1, 1);
    if (temp_v1 == 0) {
        return NULL;
    }
    sp24 = temp_v1;
    memcpy((s32)temp_v1 + 0x18, arg0, 0xC);
    sp24->unk28 = 0;
    sp24->unk2C = 8;
    sp24->unk2D = 4;
    sp24->unk30 = 0;
    sp24->unk10 = 1;
    sp24->unk14 = 0;
    sp24->unk24 = 0.0f;
    sp24->unk38 = 0.0f;
    sp24->unk3C = 0.0f;
    sp24->unk40 = 0.0f;
    sp24->unk34 = 1.0f;
    bzero((s32)sp24 + 0x48, 0x960);
    return sp24;
}

extern void func_1518E308(void *);
extern void func_1514EDF0(void *, s32);

void func_1518E4A0(void *arg0) {
    func_1518E308(arg0);
    func_1514EDF0(arg0, *(s32 *)((char *)arg0 + 0x18));
}

void func_1518E4CC(void *arg0) {
    func_1518E4A0(arg0);
    func_15169804(arg0);
}

void func_1518E4F8(void *arg0) {
    func_1518E4A0(arg0);
    func_15169824(arg0);
}

void func_1518E524(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3, u8 *arg4, u8 *arg5, s16 *arg6) {
    s32 temp;

    *arg1 = 2;
    if (func_150ADA20() & 1) {
        *arg1 |= 1;
    }

    temp = func_150ADA20() & 1;
    switch (temp) {
    case 0:
        *arg0 = 0x13;
        break;
    case 1:
        *arg0 = 0x14;
        break;
    }

    *arg2 = 0;
    *arg3 = 0;
    *arg4 = 0;
    *arg5 = 0xFF;
    *arg6 = 0x301;
}

extern u8 func_150ADA20(void);
extern void func_151429E0(u8, s32, s32, s32);

void func_1518E5D8(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, s16 *arg6) {
    s32 temp;

    if (func_150ADA20() & 1) {
        *arg1 |= 1;
    }
    *arg0 = 0x16;
    if (func_150ADA20() & 1) {
        temp = 3;
    } else {
        temp = 4;
    }
    func_151429E0(temp, arg2, arg3, arg4);
    *arg5 = 0xC8;
    *arg6 = 0x401;
}

extern void *func_1518D1C0(Owner1518D1C0 *, s16, s16, u8, u8, u8, void *);
extern u8 D_800A7460[];

s32 func_1518E66C(void *arg0) {
    func_1518D1C0(*(Owner1518D1C0 **)((char *)arg0 + 0x18), 3, 0, 0, *(u8 *)((char *)arg0 + 0xC), *(u8 *)((char *)arg0 + 1), D_800A7460);
    *(s16 *)((char *)arg0 + 0x1E) = 0x80;
    *(s8 *)((char *)arg0 + 0x21) = -1;
    return 0;
}

extern u8 D_800A749C[];

s32 func_1518E6D4(void *arg0) {
    func_1518D1C0(*(Owner1518D1C0 **)((char *)arg0 + 0x18), 4, 0, 0, *(u8 *)((char *)arg0 + 0xC), *(u8 *)((char *)arg0 + 1), D_800A749C);
    *(s16 *)((char *)arg0 + 0x1E) = 0x80;
    *(s8 *)((char *)arg0 + 0x21) = -1;
    return 0;
}

struct Struct1518E73C {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x4];
    f32 unk48;
    u8 pad4C[0x74];
    u8 unkC0[0x40];
    u8 *unk100[0x18];
    f32 unk160;
};

typedef struct Vtx1518E73C {
    s16 x, y, z, flag;
    u8 other[8];
} Vtx1518E73C;

extern f32 func_150489B0(u8);

void *func_1518E73C(struct Struct1518E73C *arg0, s16 arg1) {
    Vtx1518E73C *vertices;
    void *result;
    f32 cx;
    f32 rz;
    f32 rx;
    f32 sn;
    f32 cs;
    f32 sx;
    f32 t;
    f32 sy;
    f32 cy;
    f32 var_f12;
    f32 var_f2;
    s32 sign;
    u8 fresh;

    func_151D5D60(arg0->unk100, arg1, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x40, arg0->unkC0, 0x40);
        }
    } else {
        return NULL;
    }
    rz = D_800DD1D8[arg1];
    rx = D_800DD1E8[arg1];
    sign = -1;
    var_f12 = (((arg0->unk40 - arg0->unk34) * rx) - (rz * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f12 < 0.0f) {
        var_f12 = -var_f12;
        sign = 0;
    }
    var_f2 = sqrtf(var_f12) * arg0->unk160;
    if (1.570796371f < var_f2) {
        var_f2 = 1.570796371f;
    }
    if (sign != 0) {
        var_f2 = -var_f2;
    }
    {
        u16 angle;
        angle = (u8)(var_f2 * 40.74366379f);
        sn = func_15048A40(angle);
        cs = func_150489B0(angle);
    }
    cx = arg0->unk2C * cs;
    sx = arg0->unk2C * sn;
    sy = arg0->unk30 * sn;
    cy = arg0->unk30 * cs;
    vertices[0].x = arg0->unk34 + cx * rx;
    vertices[0].y = arg0->unk38 + sx;
    vertices[0].z = arg0->unk3C - cx * rz;
    t = -cx;
    vertices[1].x = arg0->unk34 + t * rx;
    vertices[1].y = arg0->unk38 - sx;
    vertices[1].z = arg0->unk3C - t * rz;
    t = -(cx + sy);
    vertices[2].x = arg0->unk34 + t * rx;
    vertices[2].y = (arg0->unk38 + cy) - sx;
    vertices[2].z = arg0->unk3C - t * rz;
    t = cx - sy;
    vertices[3].x = arg0->unk34 + t * rx;
    vertices[3].y = arg0->unk38 + cy + sx;
    vertices[3].z = arg0->unk3C - t * rz;
    return result;
}

void *func_1518EB10(Game1BBFC0Beam *arg0, s16 arg1) {
    Game1BBFC0Vertex *vertices;
    void *result;
    f32 progress;
    Game1BBFC0Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Game1BBFC0Vec3f *eye;
    f32 scale;

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(arg0->buffers[arg1], arg0->vertexTemplate, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    progress = arg0->progress;
    dx = arg0->end.x - arg0->start.x;
    dy = arg0->end.y - arg0->start.y;
    dz = arg0->end.z - arg0->start.z;
    tip.x = arg0->start.x + dx * progress;
    tip.y = arg0->start.y + dy * progress;
    tip.z = arg0->start.z + dz * progress;
    eye = (Game1BBFC0Vec3f *)&D_800DBFF0[arg1].unk2F8;
    dx = tip.x - arg0->start.x;
    dy = tip.y - arg0->start.y;
    dz = tip.z - arg0->start.z;
    vx = arg0->start.x - eye->x;
    vy = arg0->start.y - eye->y;
    vz = arg0->start.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = arg0->start.x + nx;
    vertices->y = arg0->start.y + ny;
    vertices->z = arg0->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x - nx;
    vertices->y = arg0->start.y - ny;
    vertices->z = arg0->start.z - nz;
    vertices->flag = 0;
    vertices++;
    dx = -dx;
    dy = -dy;
    dz = -dz;
    vx = tip.x - eye->x;
    vy = tip.y - eye->y;
    vz = tip.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = tip.x + nx;
    vertices->y = tip.y + ny;
    vertices->z = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = tip.x - nx;
    vertices->y = tip.y - ny;
    vertices->z = tip.z - nz;
    vertices->flag = 0;
    return result;
}

struct Struct1518EF54Sub {
    u8 pad0[0x4C];
    f32 unk4C;
};

struct Struct1518EF54 {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xF4];
    f32 unk140;
    f32 unk144;
    f32 unk148;
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x8];
    f32 unk15C;
};

s32 func_1518EF54(struct Struct1518EF54 *arg0) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f0;
    f32 temp_f2;
    struct Struct1518EF54Sub *temp_v1;

    temp_f0 = arg0->unk150;
    temp_v1 = (struct Struct1518EF54Sub *)((u8 *)arg0 + 0x110);
    arg0->unk140 = arg0->unk140 * temp_f0;
    arg0->unk144 += arg0->unk14C * D_800BE9A4;
    arg0->unk148 = arg0->unk148 * temp_f0;
    temp_f12 = arg0->unk140 * D_800BE9A4;
    temp_f14 = arg0->unk144 * D_800BE9A4;
    temp_f16 = arg0->unk148 * D_800BE9A4;
    arg0->unk34 += temp_f12;
    arg0->unk38 += temp_f14;
    arg0->unk3C += temp_f16;
    arg0->unk40 += temp_f12;
    arg0->unk44 += temp_f14;
    arg0->unk48 += temp_f16;
    temp_f0 = arg0->unk38;
    temp_f2 = arg0->unk3C;
    arg0->unk40 = (temp_f18 = arg0->unk34) + ((arg0->unk40 - temp_f18) * arg0->unk15C);
    arg0->unk44 = (temp_f0 = temp_f0) + ((arg0->unk44 - temp_f0) * temp_v1->unk4C);
    arg0->unk48 = (temp_f2 = temp_f2) + ((arg0->unk48 - temp_f2) * temp_v1->unk4C);
    return 1;
}

extern f32 D_800BE9A4;
s32 func_1518F058(f32 *arg0)
{
  f32 t0;
  f32 t1;
  f32 t2;
  int dummy;
  arg0[0x50] = arg0[0x54] * arg0[0x50];
  arg0[0x51] += arg0[0x53] * D_800BE9A4;
  dummy = arg0 && arg0;
  arg0[0x52] = arg0[0x54] * arg0[0x52];
  t0 = arg0[0x50] * D_800BE9A4;
  t1 = arg0[0x51] * D_800BE9A4;
  t2 = arg0[0x52] * D_800BE9A4;
  arg0[0xD] = arg0[0xD] + t0;
  arg0[0xE] = arg0[0xE] + t1;
  arg0[0xF] = arg0[0xF] + t2;
  arg0[0x10] = arg0[0x10] + t0;
  if (dummy && arg0)
  {
  }
  arg0[0x11] = arg0[0x11] + t1;
  arg0[0x12] = arg0[0x12] + t2;
  return 1;
}


struct Struct1518F108 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0xE];
    f32 unk2C;
    f32 unk30;
    char pad34[0x28];
    u8 unk5C;
    char pad5D[0xF7];
    f32 unk154;
    s16 unk158;
    s16 unk15A;
};

s32 func_1518F108(struct Struct1518F108 *arg0) {
    arg0->unk30 = arg0->unk30 - arg0->unk30 * arg0->unk154;
    arg0->unk2C = arg0->unk2C - arg0->unk2C * arg0->unk154;
    if (arg0->unk1C < arg0->unk158) {
        arg0->unk5C = arg0->unk1C * arg0->unk15A;
    }
    return 1;
}

s32 func_1518F15C(void *arg0) {
    f32 temp;

    temp = *(f32 *)((char *)arg0 + 0x30);
    *(f32 *)((char *)arg0 + 0x30) = temp - *(f32 *)((char *)arg0 + 0x154) * *(f32 *)((char *)arg0 + 0x30);
    if (*(s16 *)((char *)arg0 + 0x1C) < *(s16 *)((char *)arg0 + 0x158)) {
        *(u8 *)((char *)arg0 + 0x5C) = *(s16 *)((char *)arg0 + 0x15A) * *(s16 *)((char *)arg0 + 0x1C);
    }
    return 1;
}

struct Struct1518F1A0Obj {
    char pad0[0x24];
    s32 unk24;
    char pad28[0x4];
    f32 unk2C;
    f32 unk30;
};

struct Struct1518F1A0Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
};

s32 func_1518F1A0(struct Struct1518F1A0Obj *arg0) {
    struct Struct1518F1A0Sub *sub;
    s32 temp_v1;

    sub = (struct Struct1518F1A0Sub *)((u8 *)arg0 + 0x110);
    sub->unk1E -= D_800BE9E4;
    if (sub->unk1E < 0) {
        sub->unk1E = (u32) func_150ADA20() % 6;
        if (func_150ADA20() & 3) {
            sub->unk18 = (func_150ADA68() * (sub->unkC - sub->unk10)) + sub->unk10;
        } else {
            sub->unk18 = (func_150ADA68() * (sub->unk14 - sub->unkC)) + sub->unkC;
        }
    }
    arg0->unk30 = arg0->unk30 + ((sub->unk18 - arg0->unk30) * D_800A7B68);
    sub->unk1C -= D_800BE9E4;
    if (sub->unk1C < 0) {
        sub->unk1C = (u32) func_150ADA20() % 0x11;
        sub->unk8 = (func_150ADA68() * (sub->unk0 - sub->unk4)) + sub->unk4;
    }
    arg0->unk2C = arg0->unk2C + ((sub->unk8 - arg0->unk2C) * D_800A7B6C);
    sub->unk2C -= D_800BE9E4;
    if (sub->unk2C < 0) {
        sub->unk2C = (u32) func_150ADA20() % 0xF;
        sub->unk28 = (func_150ADA68() * (sub->unk20 - sub->unk24)) + sub->unk24;
    }
    temp_v1 = arg0->unk24;
    arg0->unk24 += (s32)((sub->unk28 - (f32) temp_v1) * D_800A7B70);
    return 1;
}

extern f32 func_150ADA68(void);
extern f32 D_800A7B74;
extern f32 D_800A7B78;

struct Struct1518F384 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    char pad14[0x4];
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
};

s32 func_1518F384(void *arg0) {
    struct Struct1518F384 *p;

    p = (struct Struct1518F384 *)((char *)arg0 + 0x110);
    if (p->unk1C == 0) {
        p->unk8 = func_150ADA68() * (p->unk0 - p->unk4) + p->unk4;
        p->unk1C = 1;
        p = (struct Struct1518F384 *)((char *)arg0 + 0x110);
    }
    if (p->unk1E == 0) {
        p->unk18 = func_150ADA68() * (p->unkC - p->unk10) + p->unk10;
        p->unk1E = 1;
    }
    *(f32 *)((char *)arg0 + 0x30) += (p->unk18 - *(f32 *)((char *)arg0 + 0x30)) * D_800A7B74;
    *(f32 *)((char *)arg0 + 0x2C) += (p->unk8 - *(f32 *)((char *)arg0 + 0x2C)) * D_800A7B78;
    return 1;
}

struct Struct1518F45CData {
    s32 unk0;
};

extern struct Struct1518F45CData D_800A74D4;

void func_1518F45C(s32 arg0, u8 arg1) {
    struct Struct1518F45CData sp1C;

    sp1C = D_800A74D4;
    func_15169260(&sp1C, 1, arg0, arg1);
}

struct Struct1518F49C {
    s32 unk0;
    u8 unk4;
};

void func_1518F49C(void *arg0, struct Struct1518F49C *arg1, u8 arg2) {
    func_15169850((s32)arg1, arg2, (s32)arg0 + 0x18, (s32)arg0 + 0x1C, (s32)arg0);
    if (arg2 == 0x49) {
        s32 a = *(s32 *)((char *)arg0 + 0x18);
        s32 b = arg1->unk0;
        u8 c = *(u8 *)((char *)arg0 + 0x1C);
        u8 d = arg1->unk4;
        if (a == b || c == d) {
            func_1516972C(arg0);
        }
    }
}

extern s32 func_1518FDC4(void *, u8 *, u8);
extern s32 func_1518F5D0(void *, void *, s16, s8, s8, s8, u8, s32, u8, s32);
extern s32 D_8008D630[];

s32 func_1518F51C(void *arg0, u8 arg1, s32 arg2, s32 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9) {
    u8 dummy[4];
    u8 sp33;

    if (arg0 == 0) {
        return 0;
    }
    if (func_1518FDC4(arg0, &sp33, arg1) == 0) {
        return 0;
    }
    return func_1518F5D0(arg0, (void *)(D_8008D630[sp33] + arg1 * 0x50), (s16)arg2, (s8)arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

struct Struct1518F5D0Data {
    u32 unk0[0x11];
    s32 unk44;
    u32 unk48[2];
};

struct Struct1518F5D0Payload {
    f32 unk0;
    struct Struct1518F5D0Data unk4;
    struct225 *unk54;
    s8 unk58;
    s8 unk59;
    u8 unk5A;
    u8 unk5B;
    u32 pad5C;
};

struct Struct1518F5D0Init {
    void *unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    s8 unkB;
    s8 unkC;
    s8 unkD;
    u8 padE[2];
};

s32 func_1518F5D0(void *arg0, void *arg1, s16 arg2, s8 arg3, s8 arg4, s8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9) {
    struct Struct1518F5D0Init init;
    struct Struct1518F5D0Payload payload;
    struct Struct1518F5D0Payload *temp_s0;
    s32 temp_v0;
    s8 flag;

    payload.unk0 = 0.0f;
    payload.unk4 = *(struct Struct1518F5D0Data *)arg1;
    payload.unk58 = arg3;
    init.unk0 = arg0;
    payload.unk59 = arg4;
    payload.unk5A = arg6;
    init.unk4 = ((struct127 *)arg0)->unique_id;
    if (arg2 == -1) {
        init.unk6 = 0x12C;
    } else {
        init.unk6 = arg2;
    }
    flag = (arg2 == -1) ? 0 : 1;
    init.unk8 = flag;
    init.unk9 = 0;
    init.unkA = 0;
    init.unkB = 0;
    init.unkC = 0;
    init.unkD = 1;

    temp_v0 = func_151D2F00((s32)&init, arg7 + 0x60, arg8, arg9);
    if (temp_v0 != 0) {
        temp_s0 = (struct Struct1518F5D0Payload *)(temp_v0 + 0x30);
        memcpy(temp_s0, &payload, 0x5C);
        if (arg5 != -1) {
            temp_s0->unk54 = func_1519021C(temp_v0, arg0, arg5, arg2, arg8, arg9);
        } else {
            temp_s0->unk54 = 0;
        }
        func_151D2AB0(temp_s0->unk4.unk44);
        func_1000FA64(0x4D, (s16)(s32)((struct127 *)arg0)->x_position, (s16)(s32)((struct127 *)arg0)->y_position, (s16)(s32)((struct127 *)arg0)->z_position, 0x3A98, 0x7D0, 0x320, (s32)func_1518E298, temp_v0, (s32)arg0, 0, 0);
    }
    return temp_v0;
}

extern f32 func_150ADA68(void);
extern f32 D_800BE9A4;
extern s32 (*D_8008D67C[])(void *);
extern void func_1518F8E0(void *);

struct Obj7C4 { f32 unk0; f32 unk4; f32 unk8; char pad[0x4C]; s8 unk58; };

s32 func_1518F7C4(void *arg0) {
    struct Obj7C4 *p;

    p = (struct Obj7C4 *)((char *)arg0 + 0x30);
    p->unk0 += (p->unk4 + func_150ADA68() * p->unk8) * D_800BE9A4;
    func_1518F8E0(arg0);
    if (p->unk58 != -1) {
        return D_8008D67C[p->unk58](arg0);
    }
    return 1;
}

extern void (*D_8008D680[])(void);

void func_1518F858(void *arg0) {
    if (*(volatile s8 *)((char *)arg0 + 0x89) == -1) {
        return;
    }
    D_8008D680[*(s8 *)((char *)arg0 + 0x89)]();
}

extern f32 func_150ADA68(void);
extern void func_1518F8E0(void *);

struct Vec898 { f32 unk0; f32 unk4; f32 unk8; f32 unkC; f32 unk10; };
struct Obj898 { char pad[0x30]; struct Vec898 vec; };

void func_1518F89C(struct Obj898 *arg0) {
    struct Vec898 *p;

    p = &arg0->vec;
    p->unk0 = func_150ADA68() * p->unk10 + p->unkC;
    func_1518F8E0(arg0);
}

struct Struct1518F8E0Frame {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ struct17 unk4;
};

struct Struct1518F8E0Sub {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ f32 unk50;
};

struct Struct1518F8E0Spawn {
    /* 0x00 */ struct127 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct17 unk8;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 pad1B;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ void *unk20;
};

struct Struct1518F8E0Size {
    f32 unk0;
    f32 unk4;
};

extern void func_15143134(void *, void *, void *);
extern s32 func_1514B8E4(struct17 *, struct Struct1518F8E0Size *, s16, u8, f32 *, f32, f32, f32, u8,
                         u8, u8, s32, u8, s16, s16, s32, u8, s32);
extern void func_151D2BA4(struct Struct1518F8E0Spawn *, struct17 *, s32, u8, s32);
extern f32 D_800A5480;

void func_1518F8E0(void *arg0) {
    struct Struct1518F8E0Sub *p;
    struct127 *obj;
    struct Struct1518F8E0Size size;
    struct17 pos;
    struct Struct1518F8E0Frame *frame;
    s32 idx;
    s32 kind;
    struct Struct1518F8E0Spawn spawn;
    s32 temp;

    obj = *(struct127 **)((char *)arg0 + 0x10);
    p = (struct Struct1518F8E0Sub *)((char *)arg0 + 0x30);
    if (p->unk0 > 1.0f) {
        if ((obj->unk1D4 != 0) && ((obj->unk74 & 0xF) != 0xF)) {
            do {
                if (*(s16 *)((char *)arg0 + 0x20) < p->unk2C) {
                    idx = p->unk48;
                    frame = &((struct Struct1518F8E0Frame *)D_800E0990[idx])[(u32)func_150ADA20() % ((u16 *)D_800E0968)[idx]];
                    size.unk0 = func_150ADA68() * p->unk1C + p->unk14;
                    size.unk4 = func_150ADA68() * p->unk20 + p->unk18;
                    func_15143134(&frame->unk4, &pos, (u8 *)obj->unk1D4 + frame->unk0 * 0x40);
                    temp = func_150ADA20();
                    if (func_150ADA20() & 1) {
                        kind = 0x13;
                    } else {
                        kind = 0x14;
                    }
                    temp = func_1514B8E4(&pos, &size, -1, (u32)temp % (p->unk25 + 1) + p->unk24,
                                         &D_800A5480, 0.0f, 1.0f, p->unk50, 0, 0x23, 0, kind, 1, 1,
                                         0xFF, 0, *(u8 *)((char *)arg0 + 0xC),
                                         *(u8 *)((char *)arg0 + 1));
                    if (temp != 0) {
                        spawn.unk0 = obj;
                        spawn.unk4 = obj->unique_id;
                        spawn.unk8 = frame->unk4;
                        spawn.unk14 = frame->unk0;
                        spawn.unk16 = (u32)func_150ADA20() % (p->unk28 + 1) + p->unk26;
                        spawn.unk18 = 1;
                        if (p->unk4C & 1) {
                            spawn.unk19 = 1;
                        } else {
                            spawn.unk19 = 0;
                        }
                        spawn.unk1A = 0;
                        spawn.unk20 = arg0;
                        spawn.unk1C = temp;
                        func_151D2BA4(&spawn, &pos, 0, *(u8 *)((char *)arg0 + 0xC),
                                      *(u8 *)((char *)arg0 + 1));
                    }
                }
                p->unk0 -= 1.0f;
            } while (p->unk0 > 1.0f);
        } else {
            do {
                p->unk0 -= 1.0f;
            } while (p->unk0 > 1.0f);
        }
    }
}

extern void func_1514BE20(void *);

struct Vec3i { s32 x, y, z; };

s32 func_1518FC04(void *arg0, struct Vec3i *arg1) {
    struct { s32 pad[13]; struct Vec3i v; } *temp;
    temp = ((void **)arg0)[0xB];
    temp->v = *arg1;
    func_1514BE20(temp);
    return 1;
}

extern void func_1514BF50(void *);

struct Vec3i2 { s32 x, y, z; };

s32 func_1518FC44(void *arg0, struct Vec3i2 *arg1) {
    struct { s32 pad[13]; struct Vec3i2 v; } *temp;
    temp = *(void **)((char *)arg0 + 0x2C);
    temp->v = *arg1;
    func_1514BF50(temp);
    return 1;
}

typedef struct Fc84Params {
    u8 pad0[0x30];
    f32 scale;      /* 0x30 */
    f32 base;       /* 0x34 */
    f32 range;      /* 0x38 */
    f32 unk3C;      /* 0x3C */
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    u8 pad48[4];
    u8 unk4C;
} Fc84Params;

typedef struct Fc84Info {
    u8 pad0[0x30];
    Fc84Params p;
} Fc84Info;

typedef struct Fc84Sub {
    u8 pad0[0x44];
    f32 unk44;
    f32 unk48;
} Fc84Sub;

typedef struct Fc84Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x58 - 0x1E];
    s32 unk58;
    u8 pad5C[0x6C - 0x5C];
    s16 unk6C;
    s16 unk6E;
    u8 unk70;
    u8 unk71;
    u8 pad72[0x110 - 0x72];
    Fc84Sub sub;
    u8 pad15C[0x148 - 0x15C];
} Fc84Obj;

typedef struct Fc84Arg0 {
    u8 pad0[0x2C];
    struct102 *obj;
    Fc84Info *info;
    f32 x, y, z;
} Fc84Arg0;

void func_1518FC84(Fc84Arg0 *arg0, f32 *arg1) {
    struct102 *obj;
    Fc84Sub *sub;
    Fc84Params *p;
    Fc84Info *info;

    obj = arg0->obj;
    if (arg0->info == NULL) {
        func_1516972C(obj);
        return;
    }
    info = arg0->info;
    ((f32 *)((u8 *)obj + 0x148))[0] = (arg1[0] - arg0->x) * info->p.scale;
    ((f32 *)((u8 *)obj + 0x148))[1] = (arg1[1] - arg0->y) * info->p.scale;
    ((f32 *)((u8 *)obj + 0x148))[2] = (arg1[2] - arg0->z) * info->p.scale;
    p = &info->p;
    sub = &((Fc84Obj *)obj)->sub;
    sub->unk44 = func_150ADA68() * p->range + p->base;
    sub->unk48 = p->unk3C;
    ((Fc84Obj *)obj)->unk6C = p->unk40;
    ((Fc84Obj *)obj)->unk6E = p->unk42;
    ((Fc84Obj *)obj)->unk1C = func_150ADA20() % (u32)(p->unk46 + 1) + p->unk44;
    if (p->unk4C & 1) {
        ((Fc84Obj *)obj)->unk70 = 0x21;
    } else {
        ((Fc84Obj *)obj)->unk70 = 0x20;
    }
    ((Fc84Obj *)obj)->unk71 = 0x24;
    ((Fc84Obj *)obj)->unk58 |= 0x08000001;
}

s32 func_1518FDC4(void *arg0, u8 *arg1, u8 arg2) {

    switch ((s32)*((u8 *)arg0 + 4)) {
    case 0x87:
    case 0xB0:
        *arg1 = 0xD;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0xB1:
        *arg1 = 0xE;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0xB4:
        *arg1 = 0xF;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x70:
    case 0xB2:
        *arg1 = 0x10;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x53:
        *arg1 = 0x11;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x96:
        *arg1 = 0x12;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x4:
        *arg1 = 0;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x49:
        *arg1 = 1;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x58:
        *arg1 = 2;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x50:
        *arg1 = 3;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x5A:
    case 0x5F:
    case 0x74:
    case 0x75:
    case 0x7A:
        *arg1 = 4;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x80:
        *arg1 = 5;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        *arg1 = 6;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x16:
        *arg1 = 7;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x88:
    case 0x90:
        *arg1 = 8;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x9C:
        *arg1 = 9;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x9D:
        *arg1 = 0xA;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0x9F:
        *arg1 = 0xB;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    case 0xA0:
        *arg1 = 0xC;
        if (arg2 > 0) {
            return 0;
        }
        return 1;
    default:
        return 0;
    }
}

extern u8 D_800A7AC8;
extern u8 D_800A7ACC;
extern u8 D_800A7AD0;
extern u8 D_800A7AD4;
extern u8 D_800A7AD8;
extern u8 D_800A7ADC;
extern u8 D_800A7AE0;
extern u8 D_800A7AE4;
extern u8 D_800A7AE8;
extern u8 D_800A7AEC;
extern u8 D_800A7AF0;
extern u8 D_800A7AF4;
extern u8 D_800A7AF8;
extern u8 D_800A7AFC;
extern u8 D_800A7B00;
extern u8 D_800A7B04;
extern u8 D_800A7B08;
extern u8 D_800A7B0C;
extern u8 D_800A7B10;

u8 *func_1519003C(u8 *arg0, s32 *arg1) {
    u8 *var_v1;

    switch (arg0[4]) {
    case 0x87:
    case 0xB0:
        var_v1 = &D_800A7AFC;
        *arg1 = 1;
        break;
    case 0xB1:
        var_v1 = &D_800A7B00;
        *arg1 = 1;
        break;
    case 0xB4:
        var_v1 = &D_800A7B04;
        *arg1 = 1;
        break;
    case 0x70:
    case 0xB2:
        var_v1 = &D_800A7B08;
        *arg1 = 1;
        break;
    case 0x53:
        var_v1 = &D_800A7B0C;
        *arg1 = 1;
        break;
    case 0x96:
        var_v1 = &D_800A7B10;
        *arg1 = 1;
        break;
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x4:
        var_v1 = &D_800A7AC8;
        *arg1 = 1;
        break;
    case 0x49:
        var_v1 = &D_800A7ACC;
        *arg1 = 1;
        break;
    case 0x58:
        var_v1 = &D_800A7AD0;
        *arg1 = 1;
        break;
    case 0x50:
        var_v1 = &D_800A7AD4;
        *arg1 = 1;
        break;
    case 0x75:
        var_v1 = &D_800A7AD8;
        *arg1 = 1;
        break;
    case 0x80:
        var_v1 = &D_800A7ADC;
        *arg1 = 1;
        break;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        var_v1 = &D_800A7AE0;
        *arg1 = 1;
        break;
    case 0x16:
        var_v1 = &D_800A7AE4;
        *arg1 = 1;
        break;
    case 0x88:
    case 0x90:
        var_v1 = &D_800A7AE8;
        *arg1 = 1;
        break;
    case 0x9C:
        var_v1 = &D_800A7AEC;
        *arg1 = 1;
        break;
    case 0x9D:
        var_v1 = &D_800A7AF0;
        *arg1 = 1;
        break;
    case 0x9F:
        var_v1 = &D_800A7AF4;
        *arg1 = 1;
        break;
    case 0xA0:
        var_v1 = &D_800A7AF8;
        *arg1 = 1;
        break;
    default:
        var_v1 = 0;
        *arg1 = 0;
        break;
    }
    return var_v1;
}

struct Struct1519021CArg1 {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
};

struct Struct1519021CData {
    struct Struct1519021CArg1 *unk0;
    u8 unk4;
    u8 pad5[3];
};

struct Struct1519021CLocal {
    s32 unk0;
    f32 unk4[8];
    struct Struct1519021CData unk24;
    Header unk2C;
    s32 pad34;
};

struct225 *func_1519021C(s32 arg0, struct Struct1519021CArg1 *arg1, u8 arg2, s16 arg3, u8 arg4, s32 arg5) {
    struct Struct1519021CLocal sp58;
    Header2 header2;
    struct225 *temp_v0;

    if (arg2 >= 0x18) {
        return 0;
    }

    sp58.unk4[0] = D_800A67C0[arg2];
    sp58.unk4[1] = D_800A6760[arg2];
    sp58.unk4[2] = D_800A6820[arg2];
    sp58.unk4[3] = 1.0f;
    sp58.unk4[5] = 5.0f;
    sp58.unk4[4] = 0.0f;
    sp58.unk4[7] = 0.0f;
    sp58.unk4[6] = D_800A8004;
    sp58.unk0 = arg0;

    sp58.unk2C.unk0 = ((arg3 == -1) ? 0 : 1) | 2;
    sp58.unk2C.unk1 = 0x12;
    if (arg3 == -1) {
        sp58.unk2C.unk2 = 0x12C;
    } else {
        sp58.unk2C.unk2 = arg3;
    }
    sp58.unk2C.unk4 = 0x25;

    sp58.unk24.unk0 = arg1;
    sp58.unk24.unk4 = arg1->unk3B;

    header2.unk0 = arg1->unk14;
    header2.unk4 = arg1->unk18;
    header2.unk8 = arg1->unk1C;

    temp_v0 = func_151602C0(&sp58.unk2C, &header2, 0, 0xFF, 0xD1, 0, 0xFF, 0, 0x30, arg4, arg5);
    if (temp_v0 != 0) {
        memcpy((s32)temp_v0 + 0x18, &sp58.unk24, sizeof(sp58.unk24));
        memcpy((s32)temp_v0 + 0x20, sp58.unk4, sizeof(sp58.unk4));
        memcpy((s32)temp_v0 + 0x40, &sp58.unk0, sizeof(sp58.unk0));
    }
    return temp_v0;
}

extern void func_15163DEC(void *, void *);
extern s32 func_15163F50(void *, void *);

void func_151903D0(void *arg0) {
    func_15163DEC(arg0, (char *)arg0 + 0x20);
    func_15163F50(arg0, (char *)arg0 + 0x18);
}

void func_15190400(struct102 *arg0, void *arg1, u8 arg2) {
    s32 a, b;
    u8 c, d;
    if (arg2 == 0) {
        a = *(s32 *)((u8 *)arg0 + 0x18);
        b = *(s32 *)arg1;
        c = *(u8 *)((u8 *)arg0 + 0x1C);
        d = *(u8 *)((u8 *)arg1 + 0x4);
        if (a == b || c == d) {
            func_1516972C(arg0);
        }
    }
}

void func_15190454(void *arg0) {
    s32 *t = *(s32 **)((u8 *)arg0 + 0x40);

    t[0x21] = 0;
}

extern void func_15190454(void *);
extern void func_151617C4(void *);

void func_15190464(void *arg0) {
    func_15190454(arg0);
    func_151617C4(arg0);
}

extern void func_151617E4(void *);

void func_15190490(void *arg0) {
    func_15190454(arg0);
    func_151617E4(arg0);
}

extern void func_1000FD38(void *, void *, s32);
extern void func_151D2B4C(s32);

void func_151904BC(void *arg0) {
    struct Struct1518F5D0Payload *p;

    p = (struct Struct1518F5D0Payload *)((u8 *)arg0 + 0x30);
    if (p->unk54 != 0) {
        func_1516972C((struct102 *)p->unk54);
    }
    func_1000FD38(func_1518E298, arg0, *(s32 *)((u8 *)arg0 + 0x10));
    func_151D2B4C(p->unk4.unk44);
}

struct tmp15190518 {
    s32 unk0;
    s8  unk4;
};

void func_15190518(void *arg0) {
    struct tmp15190518 tmp;

    tmp.unk0 = (s32)arg0;
    tmp.unk4 = *(u8 *)((char *)arg0 + 0x3B);

    func_1516944C(0x3E, (s32)&tmp, 0x2A);
}

extern void func_151D33FC(void *, s32);
extern void (*D_8008D684[])(void *, s32, u8);

void func_15190550(void *arg0, s32 arg1, u8 arg2) {
    void (*fn)(void *, s32, u8);

    if (arg2 == 0x2A) {
        func_151D33FC(arg0, arg1);
    }
    fn = D_8008D684[*(u8 *)((char *)arg0 + 0x8A)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

extern s32 func_1505D024(void *, s32, u16, s32);

s32 func_151905BC(struct TimedSoundObject *arg0) {
    struct TimedSoundTrigger *p;
    struct SoundLinkedPayload *temp;

    arg0->field_0x90.field_0x4 += D_800BE9E4;
    p = &arg0->field_0x90;
    while (p->field_0x4 >= p->field_0x0) {
        func_1505D024(arg0->field_0x10, 0x60021, 0, p->field_0x8 - D_800CC2D0);
        p->field_0x4 -= p->field_0x0;
    }
    temp = arg0->field_0x10->field_0x31C;
    if (temp != NULL) {
        if (temp->field_0x84 == 0) {
            temp->field_0x46 = 0x3E8;
        }
    }
    return 1;
}

void func_151906C4(void *arg0) {
    u8 *temp;
    temp = *(u8 **)(*(u8 **)((u8 *)arg0 + 0x10) + 0x31C);
    if (temp != NULL) {
        temp[0x1AC] = 0;
    }
}

void func_151906E0(void *arg0, s32 *arg1, u8 arg2) {
    s32 *ptr;
    s32 temp;

    ptr = (s32 *)((char *)arg0 + 0x90);
    if (arg2 == 0x2D) {
        temp = arg1[0];
        if (temp == ptr[2]) {
            ptr[2] = arg1[1];
        } else if (arg1[1] == ptr[2]) {
            ptr[2] = temp;
        }
    }
}

extern void func_151D343C(void *, s32);
extern void func_1518F45C(s32, u8);

struct tmp1519072C {
    s32 unk0;
    s8  unk4;
};

void func_1519072C(void *arg0) {
    struct tmp1519072C tmp;
    struct tmp1519072C *p;
    struct tmp1519072C **pp;

    tmp.unk0 = (s32)arg0;
    tmp.unk4 = *(u8 *)((char *)arg0 + 0x3B);
    pp = &p;
    p = &tmp;

    func_151D343C(*pp, 0x2A);
    func_1518F45C(*pp, 0x49);
}

extern void func_15191B8C(s32, u8);
extern u8 func_151D8E20(void);

struct Struct15190770Local {
    s32 unk0;
    u8 unk4;
    u8 pad5[0x3];
};

void *func_15190770(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_s0;
    struct Struct15190770Local sp2C;
    void *sp38;

    sp2C.unk0 = *(s32 *)arg0;
    sp2C.unk4 = *(u8 *)(*(s32 *)arg0 + 0x3B);
    sp38 = arg0;
    func_15191B8C((s32)&sp2C, 0xE);
    temp_s0 = func_15167A68(0x20, arg3, arg1 + 0x300, 1, (u8)arg2, 1);
    if (temp_s0 == 0) {
        return NULL;
    }
    memcpy((s32)temp_s0 + 0x10, sp38, 0xC);
    *(void **)((s32)temp_s0 + 0x24) = (void *)((s32)temp_s0 + 0x30);
    *(void **)((s32)temp_s0 + 0x28) = (void *)((s32)temp_s0 + 0x300);
    *(s32 *)((s32)temp_s0 + 0x20) = 0;
    *(f32 *)((s32)temp_s0 + 0x1C) = 0.0f;
    bzero((s32)temp_s0 + 0x30, 0x2D0);
    *(u8 *)((s32)temp_s0 + 0x2C) = func_151D8E20();
    return temp_s0;
}

struct Struct15191400Arg;
extern void func_15191400(struct Struct15191400Arg *);

void func_15190840(void *arg0) {
    func_15191400(arg0);
    func_15169804(arg0);
}

void func_1519086C(void *arg0) {
    func_15191400(arg0);
    func_15169824(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1BA1D0/func_15190898.s")

struct OscillatorMotionState {
    u8 pad_0x0[0x21];
    u8 field_0x21;
    u8 field_0x22;
    s8 field_0x23;
    s8 field_0x24;
    u8 pad_0x25[0x3];
    f32 field_0x28;
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
};

struct OscillatorObject {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    struct OscillatorMotionState field_0x110;
};

s32 func_15190F9C(struct OscillatorObject *arg0) {
    struct OscillatorObject *p = arg0;
    struct OscillatorMotionState *f = &arg0->field_0x110;
    f32 r1;
    f32 r2;

    f->field_0x21 = f->field_0x21 + f->field_0x23 * D_800BE9E4;
    f->field_0x22 = f->field_0x22 + f->field_0x24 * D_800BE9E4;
    r1 = func_151423D8((u8)(f->field_0x21 - 0x40));
    r2 = func_151423D8((u8)(f->field_0x22 - 0x40));
    p->field_0x2C += (f->field_0x28 + f->field_0x30 * r1 - p->field_0x2C) * 0.5f;
    p->field_0x30 += (f->field_0x2C + f->field_0x34 * r2 - p->field_0x30) * 0.5f;
    return 1;
}

struct Struct1519108C {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
    f32 unk12C;
};

s16 *func_1519108C(struct Struct1519108C *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 temp_f2;
    f32 sinval;
    f32 temp_f18;
    f32 temp_y_cos;
    f32 temp_y_sin;
    f32 var_f2;
    f32 var_f14;
    f32 *temp_v1;
    f32 *temp_a0;
    f32 temp_f0;
    u8 sp37;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp37);
    ret = var_v0;
    if (var_v0) {
        if (sp37) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_v1 = &D_800DD1E8[arg1];
    temp_a0 = &D_800DD1D8[arg1];
    var_f14 = -1.0f;
    var_f2 = ((*temp_v1 * (arg0->unk40 - arg0->unk34)) - (*temp_a0 * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f2 < 0.0f) {
        var_f14 = 1.0f;
        var_f2 = -var_f2;
    }

    temp_f0 = sqrtf(var_f2) * arg0->unk12C;
    if (D_800A8018 < temp_f0) {
        temp_f0 = D_800A8018;
    }

    temp_f0 = temp_f0 * var_f14;
    sinval = sinf(temp_f0);
    temp_f0 = cosf(temp_f0);
    temp_f2 = arg0->unk2C * temp_f0;
    temp_f18 = arg0->unk2C * sinval;
    temp_y_cos = arg0->unk30 * temp_f0;
    temp_y_sin = arg0->unk30 * sinval;

    var_v0[0] = (*temp_v1 * -temp_f2) + arg0->unk34;
    var_v0[1] = arg0->unk38 + temp_f18;
    var_v0[2] = arg0->unk3C - (*temp_a0 * -temp_f2);
    var_v0[3] = 0;

    var_v0[8] = (*temp_v1 * temp_f2) + arg0->unk34;
    var_v0[9] = arg0->unk38 - temp_f18;
    var_v0[10] = arg0->unk3C - (*temp_a0 * temp_f2);
    var_v0[11] = 0;

    temp_f0 = temp_f2 - temp_y_sin;
    var_v0[16] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[17] = (arg0->unk38 - temp_f18) - temp_y_cos;
    var_v0[18] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[19] = 0;

    temp_f0 = -(temp_f2 + temp_y_sin);
    var_v0[24] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[25] = (arg0->unk38 + temp_f18) - temp_y_cos;
    var_v0[26] = arg0->unk3C - (*temp_a0 * temp_f0);
    var_v0[27] = 0;

    return ret;
}

struct Struct15191400Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x14];
    u8 unk20;
};
struct Struct15191400Obj {
    u8 pad0[0x1C];
    s16 unk1C;
    u8 pad1E[0x3A];
    s32 unk58;
    u8 pad5C[0xB4];
    struct Struct15191400Sub unk110;
};
struct Struct15191400Slot {
    struct Struct15191400Obj *unk0;
    s32 unk4;
    s32 unk8;
};
struct Struct15191400Range {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x10];
};
struct Struct15191400Arg {
    u8 pad0[0x1C];
    f32 unk1C;
    s32 unk20;
    struct Struct15191400Slot *unk24;
};
extern struct Struct15191400Range D_800A9D40[];
extern void func_1513F680();

void func_15191400(struct Struct15191400Arg *arg0) {
    s32 i;
    struct Struct15191400Slot *slot;
    struct Struct15191400Sub *sub;
    struct Struct15191400Range *range;

    arg0->unk1C = 0.0f;
    arg0->unk20 = 0;
    slot = arg0->unk24;
    for (i = 0; i < 60; i++, slot++) {
        if (slot->unk0 != NULL) {
            sub = &slot->unk0->unk110;
            range = &D_800A9D40[sub->unk20];
            sub->unk0 = 0.0f;
            sub->unk4 = 0.0f;
            sub->unk8 = 0.0f;
            func_1513F680(slot->unk0, 13, 18, 0, 13);
            slot->unk0->unk1C = (u32)func_150ADA20() % (range->unk2 + 1) + range->unk0;
            slot->unk0->unk58 |= 1;
        }
    }
    bzero(arg0->unk24, 0x2D0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1BA1D0/func_15191520.s")

s32 func_1519187C(void *arg0)
{
  s16 v0;
  s32 v1;
  v0 = *((s16 *) (((u8 *) arg0) + 0x1C));
  if (v0 < (*((s16 *) (((0, (u8 *) arg0)) + 0x124))))
  {
    v1 = v0;
    v1 *= *((s16 *) (((u8 *) arg0) + 0x126));
    if (v1 < (*((u8 *) (((u8 *) arg0) + 0x5C))))
    {
      *((u8 *) (((u8 *) arg0) + 0x5C)) = v1;
    }
  }
  return 1;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1BA1D0/func_151918BC.s")

void func_15191980(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;

  arg1p = (s32 *) &arg1;
  if (arg2 == 0xD)
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      u8 *base;
      u8 old;
      u8 *flagsPtr;

      old = *((u8 *) (((u8 *) arg0) + 0x18));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0x18);
      base = (u8 *) arg0;
      *flagsPtr = *((u8 *) (((u8 *) arg0) + 0x18)) | 1;
      *((s16 *) (base + 0x16)) = 0x64;
      ;
    }
  }
  else
    if (((arg2 == 0xC) || (arg2 == 0x50)) || (arg2 == 0xE))
  {
    if ((*((s32 *) (((u8 *) arg0) + 0x10)) == *((s32 *) arg1)) || ((*((u8 *) (((u8 *) arg0) + 0x14))) == *((u8 *) ((*arg1p) + 4))))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
  {
    func_15169850(arg1, arg2, (s32) arg0 + 0x10, (s32) arg0 + 0x14, (s32) arg0);
  }
}

void func_1516972C(struct102 *arg0);
void func_15169850(s32, u8, s32, s32, s32);
void func_15191A84(struct102 *arg0, s32 arg1, u8 arg2)
{
  s32 *arg1p;
  struct Struct1518F49C *p;
  u8 *base;
  u8 old;
  u8 *flagsPtr;
  p = (struct Struct1518F49C *) (((u8 *) arg0) + 0x28);
  arg1p = (s32 *) (&arg1);
  if (((arg2 == 0xC) || (arg2 == 0xE)) || (arg2 == 0x50))
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*arg1p) + 4))) == p->unk4))
    {
      func_1516972C(arg0);
    }
  }
  else
    if (arg2 == 0x4E)
  {
    func_1516972C(arg0);
  }
  else
    if (arg2 == 0xD)
  {
    if ((((struct Struct1518F49C *) arg1)->unk0 == p->unk0) || ((*((u8 *) ((*((s32 *) (&arg1))) + 4))) == p->unk4))
    {
      old = *((u8 *) (((u8 *) arg0) + 0xD));
      flagsPtr = (u8 *) (((u8 *) arg0) + 0xD);
      base = (u8 *) arg0;
      *flagsPtr = (*((u8 *) (((u8 *) arg0) + 0xD))) | 1;
      *((s16 *) (base + 0xE)) = 0x64;
      dummy_label_15191A84_1:
      ;

      ;
      ;
    }
  }
  else
  {
    func_15169850(arg1, arg2, (s32) p, (s32) (&p->unk4), (s32) arg0);
  }
}


extern s32 D_800A8010;

void func_15191B8C(s32 arg0, u8 arg1) {
    s32 sp1C;

    sp1C = D_800A8010;
    func_151494E0(arg0, arg1);
    func_15169260(&sp1C, 1, arg0, arg1);
}
