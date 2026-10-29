#include <ultra64.h>
#define func_15178E14 func_15178E14_header_mismatch
#define func_150ADA20 func_150ADA20_hdr
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_150ADA20
#undef func_15048A40
#undef func_15178E14
#include "variables.h"

typedef struct Func15178750Inner {
    u8 pad0[0x36];
    u8 unk36;
} Func15178750Inner;

typedef struct Func15178750Arg1 {
    u8 pad0[0x14];
    Func15178750Inner *unk14;
} Func15178750Arg1;

extern s32 func_15168118();


u32 func_150ADA20(void);
f32 func_15048A40(u8);
f32 func_150489B0(u8);
void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 D_8008CA64;

typedef struct Func15177F90Obj {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0xA];
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    u8 unk34;
    u8 unk35;
    u8 pad36;
    u8 unk37;
    u8 pad38[0x10];
    s8 unk48;
} Func15177F90Obj;

typedef struct Func15177F90Spawn {
    s32 unk0;
    Func15177F90Obj *unk4;
    u8 pad8[4];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 pad1C[3];
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
    u8 pad30[4];
    s16 unk34;
    u8 pad36[2];
} Func15177F90Spawn;

void func_15177F90(u8 count, s32 arg1, u8 arg2, s32 x, s32 y, s32 z, s32 arg6, s32 arg7, u8 arg8, s32 arg9, s32 argA) {
    Func15177F90Spawn spawn;
    Func15177F90Obj *obj;
    Func15177F90Obj *part;
    s32 i;
    s32 angle;
    f32 c;
    f32 s;
    f32 r;
    s32 px;
    s32 pz;
    s32 py;
    s32 pad1;

    obj = func_15167A68(0x3A, 0, 0x38, 0, arg8, 1);
    if (obj == NULL) {
        return;
    }
    if (argA == 0) {
        argA = 0x118;
    } else {
        argA = 0x8C;
    }
    if (arg1 != 0) {
        obj->unk10 = arg1;
        obj->unk14 = 0x80000000;
    } else {
        obj->unk10 = (x << 16) | (y & 0xFFFF);
        obj->unk14 = z << 16;
    }
    obj->unk18 = x;
    obj->unk1C = y;
    obj->unk20 = z;
    obj->unk30 = 0;
    obj->unk32 = 0x320;
    obj->unk35 = count;
    obj->unk34 = arg2;
    obj->unk37 = arg9;
    obj->unk2E = 0;
    if (0) {}
    spawn.unk0 = D_8008CA64;
    spawn.unk4 = obj;
    spawn.unk16 = 0;
    spawn.unk18 = 0;
    spawn.unk1A = 0;
    spawn.unk1F = 6;
    spawn.unk20 = 0;
    spawn.unk22 = 0;
    spawn.unk24 = argA;
    spawn.unk26 = argA;
    spawn.unk28 = arg7;
    spawn.unk2A = 0;
    spawn.unk2C = 0xFF;
    spawn.unk2D = 0xFF;
    spawn.unk2E = 0xFF;
    spawn.unk2F = 0xFF;
    spawn.unk34 = 0;
    for (i = 0; i < count; i++) {
        angle = func_150ADA20() & 0xFF;
        c = func_15048A40(angle);
        s = func_150489B0(angle);
        r = (func_150ADA20() & 0xFFFF) * 50.0f * (1.0f / 65536.0f);
        px = x + r * c;
        pz = z + r * s;
        py = (func_150ADA20() & 0xFF) + y - 0x4E;
        spawn.unk2B = (func_150ADA20() & 7) + 7;
        spawn.unkC = 0;
        spawn.unkE = 0;
        spawn.unk10 = px;
        spawn.unk12 = py;
        spawn.unk14 = pz;
        part = func_15167A68(0x3B, 0, 0x4C, 0, arg8, 1);
        if (part == NULL) {
            break;
        }
        bcopy(&spawn, &part->unk10, 0x38);
        part->unk48 = -1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A5440/func_15178268.s")

s32 func_15178750(s32 arg0, Func15178750Arg1 *arg1, s16 arg2) {
    Func15178750Inner *temp;

    temp = arg1->unk14;
    if (temp->unk36 & (1 << arg2)) {
        return func_15168118();
    }
    return arg0;
}

static void func_15178750_pad(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A5440/func_151787AC.s")

typedef struct Node15178B98 {
    char pad0[0x8];
    struct Node15178B98 *unk8;
    char padC[0x4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    char pad24[0xC];
    s16 unk30;
    char pad32[0x2];
    u8 unk34;
} Node15178B98;

extern Node15178B98 *D_800DCF38;

Node15178B98 *func_15178B98(u8 arg0) {
    Node15178B98 *node;

    for (node = D_800DCF38; node != NULL; node = node->unk8) {
        if (node->unk34 == arg0) {
            return node;
        }
    }
    return NULL;
}

Node15178B98 *func_15178BE4(u8 arg0, s32 arg1, s16 arg2) {
    Node15178B98 *node;

    node = func_15178B98(arg0);
    if (node != NULL) {
        node->unk10 = arg1;
        node->unk14 = 0x80000000;
        node->unk30 = arg2;
    }
}

Node15178B98 *func_15178C34(u8 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    Node15178B98 *node;

    node = func_15178B98(arg0);
    if (node != NULL) {
        node->unk10 = (arg1 << 16) | (arg2 & 0xFFFF);
        node->unk14 = arg3 << 16;
        node->unk30 = arg4;
    }
}

extern f32 sqrtf(f32);

s32 func_15178C9C(u8 arg0, s32 arg1) {
    Node15178B98 *node;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 sum;

    node = func_15178B98(arg0);
    if (node != NULL) {
        dx = (s32)(D_800CC2D0[arg1].x_position - (f32)node->unk18);
        dy = (s32)(D_800CC2D0[arg1].y_position - (f32)node->unk1C);
        dz = (s32)(D_800CC2D0[arg1].z_position - (f32)node->unk20);
        dx = dx * dx;
        dy = dy * dy;
        dz = dz * dz;
        sum = dx + dy + dz;
        return (s32)sqrtf((f32)sum);
    }
    return 1;
}

typedef struct Node15178DA4 {
    char pad0[0x8];
    struct Node15178DA4 *unk8;
    char padC[0x8];
    struct Node15178DA4 *unk14;
    char pad18[0x16];
    u16 unk2E;
} Node15178DA4;

extern Node15178DA4 *D_800DCF3C;

void func_15178DA4(Node15178DA4 *arg0) {
    Node15178DA4 *next;
    Node15178DA4 *node;

    node = D_800DCF3C;
    func_100111C8(arg0->unk2E);
    if (node != NULL) {
        do {
            next = node->unk8;
            if (arg0 == node->unk14) {
                func_1516972C((struct102 *)node);
            }
            node = next;
        } while (next != NULL);
    }
    func_15169824((struct102 *)arg0);
}

void func_15178E14(u8 arg0) {
    func_15178DA4(func_15178B98(arg0));
}
