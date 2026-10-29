#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern f32 D_8009FDD4;
extern f32 D_8009FDD8;
extern f32 D_8009FDDC;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    u8 pad18[8];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8 pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    u8 pad5C[4];
    s32 unk60;
    u8 pad64[0xE];
    u8 unk72;
    u8 pad73[5];
    u8 unk78;
    u8 pad79[0xAF];
    s32 unk128;
    u8 pad12C[0x14];
    f32 unk140;
    u8 pad144[8];
    s32 unk14C;
    u8 pad150[0x18];
    s32 unk168;
} Obj150B89E0;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
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
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
} Spawn150B89E0;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 unk7C;
    u8 unk7D;
    u8 pad7E[2];
    s16 unk80;
    s16 unk82;
    s32 unk84;
} Burst150B89E0;

extern s32 D_80088734[];
extern s32 D_8008873C;
void *memcpy(void *, const void *, u32);
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_15151A38(Burst150B89E0 *, u8, s32);

s32 func_150B89E0(Obj150B89E0 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    Spawn150B89E0 spawn;
    f32 extra[2];

    arg0->unk3C = arg0->unk10 + arg4;
    if (arg0->unk140 > 15.0f) {
        if (arg0->unk140 > 30.0f) {
            spawn.unk54 = 0x19;
            spawn.unk56 = 0xA;
            spawn.unk0 = 4;
            spawn.unk1 = 0;
            spawn.unk2 = 0x3B02;
            spawn.unk4 = (func_150ADA20() % 31U) + 0x28;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0xFF;
            spawn.unk11 = 0xFF;
            spawn.unk12 = 0xFF;
            spawn.unk13 = 0xFF;
            spawn.unk14 = spawn.unk18 = ((func_150ADA68() * 25.0f) + 25.0f) * 0.001f;
            spawn.unk40 = 0x0C5C0001;
            spawn.unk44 = 0xFF;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk1C = arg1;
            spawn.unk28 = 0.0f;
            spawn.unk2C = 0.0f;
            spawn.unk30 = 0.0f;
            spawn.unk34 = 0.0f;
            spawn.unk38 = 0.0f;
            spawn.unk3C = 0.0f;
            spawn.unk20 = arg4;
            spawn.unk24 = arg3;
            extra[0] = ((func_150ADA68() * 80.0f) + 50.0f) * 1e-05f;
            spawn.unk50 = arg0->unk128;
            arg5 = func_1513D594((s32)&spawn, (s32)&D_800A4AA0, 0x18, 0, 0,
                                (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), func_150ADA20() & 0xFF,
                                500.0f, 500.0f, 0, arg5, arg0->unk14C, arg0->unk168, 0, 8, arg0->unkC, arg0->unk1);
            if (arg5 != 0) {
                memcpy((void *)(arg5 + 0x128), extra, 8);
            }
            {
            Burst150B89E0 burst;
            burst.unk0 = arg1;
            burst.unk24 = 6;
            burst.unk26 = 6;
            burst.unk2A = 0xFF;
            burst.unk2C = -0x3F;
            burst.unk2E = 0x1D;
            burst.unkC = 3.0f;
            burst.unk10 = 3.0f;
            burst.unk1C = 10.0f;
            burst.unk20 = 10.0f;
            burst.unk28 = 0;
            burst.unk30 = 3;
            burst.unk32 = 3;
            burst.unk34 = 0x32;
            burst.unk36 = 0x14;
            burst.unk38 = 1;
            burst.unk39 = 0x21;
            burst.unk3A = 0x48;
            burst.unk3C = 1;
            burst.unk40 = 0;
            burst.unk44 = 0;
            burst.unk48 = 0;
            burst.unk4C = 0;
            burst.unk50 = 0;
            burst.unk54 = 0;
            burst.unk55 = 0x9B;
            burst.unk56 = 0x64;
            burst.unk4 = arg4;
            burst.unk8 = arg3;
            burst.unk14 = 0.6610000134f;
            burst.unk18 = 0;
            burst.unk58 = arg0->unk14C;
            burst.unk5C = arg0->unk168;
            burst.unk60 = 0;
            burst.unk64 = 0x220005;
            burst.unk68 = 0x1D0600;
            burst.unk6C = 1;
            burst.unk70 = 0x3B;
            burst.unk74 = 0x80;
            burst.unk78 = 0x20;
            burst.unk7C = 0;
            burst.unk7D = 7;
            burst.unk80 = 0xA;
            burst.unk82 = 0x19;
            burst.unk84 = arg0->unk128;
            func_15151A38(&burst, arg0->unkC, arg0->unk1);
            }
            func_10010F88(D_80088734[func_150ADA20() & 1], 0x7D00, 0, 0, 0, arg0->unk38, arg4 + 25.0f, arg0->unk40, 1000, 4000);
            return 0;
        }
        arg0->unk44 *= arg0->unk14;
        arg0->unk48 = -arg0->unk48 * arg0->unk14;
        arg0->unk4C *= arg0->unk14;
        arg0->unk50 *= arg0->unk14;
        arg0->unk54 *= arg0->unk14;
        arg0->unk58 *= arg0->unk14;
        func_10010F88(D_8008873C, 0x7D00, 0, 0, 0, arg0->unk38, arg0->unk3C, arg0->unk40, 1000, 4000);
    } else {
        f32 mtx[4][4];

        arg0->unk72 = 1;
        arg0->unk78 = 1;
        arg0->unk60 &= ~0xFF;
        func_150A8050(mtx, arg0->unk20, arg0->unk24, arg0->unk28);
        func_150A7960(mtx, 0.0f, 0.0f, 200.0f, &arg0->unk20, &arg0->unk24, &arg0->unk28);
        arg0->unk20 += arg0->unk38;
        arg0->unk24 += arg0->unk3C;
        arg0->unk28 += arg0->unk40;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E5E90/func_150B8F44.s")

s32 func_150B9560(void *arg0) {
    f32 *temp_v0;
    f32 temp_f0;
    s16 v1;

    temp_v0 = (f32 *)((u8 *)arg0 + 0x110);
    v1 = *(volatile s16 *)((u8 *)arg0 + 0x1C);
    if (v1 < 0x50) {
        *temp_v0 += D_8009FDD4 * D_800BE9A4;
        *(f32 *)((u8 *)arg0 + 0x38) = *(volatile f32 *)((u8 *)arg0 + 0x38) - *temp_v0;
        v1 = *(volatile s16 *)((u8 *)arg0 + 0x1C);
    }

    if (v1 >= 0x73) {
        temp_f0 = D_8009FDD8;
        *(f32 *)((u8 *)arg0 + 0x2C) *= temp_f0;
        *(f32 *)((u8 *)arg0 + 0x30) *= temp_f0;
    } else if (v1 < 0x41) {
        *(f32 *)((u8 *)arg0 + 0x30) += D_8009FDDC;
    }

    return 1;
}

s32 func_150B95FC(void *arg0) {
    s16 v0 = *(s16*)((u8*)arg0 + 0x1C);
    if (v0 < 0x20) {
        *(u8*)((u8*)arg0 + 0x5C) = v0 << 3;
    }
    return 1;
}

s32 func_150B961C(void *arg0) {
    s16 v0 = *(s16*)((u8*)arg0 + 0x1C);
    if (v0 < 0x40) {
        *(u8*)((u8*)arg0 + 0x28) = v0 << 2;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E5E90/func_150B963C.s")

struct Struct150B9D14 {
    char pad0[0x18];
    s32 unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
};

extern void func_15142600(s32, s32, s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 func_150B9D14(s32 arg0, struct Struct150B9D14 *arg1) {
    func_15142600(arg0, arg1->unk18, arg1->unk1C, arg1->unk2C,
                  arg1->unk30, arg1->unk34, arg1->unk38, arg1->unk3C,
                  arg1->unk40, arg1->unk20, arg1->unk24, arg1->unk28);
    return 1;
}

struct Struct150B9D8C {
    char pad0[0x1];
    u8  unk1;
    char pad2[0xA];
    u8  unkC;
    char padD[0x83];
    s32 unk90;
    char pad94[0x4];
    s16 unk98;
    char pad9A[0x2];
    s16 unk9C;
    char pad9E[0x2];
    s16 unkA0;
    s16 unkA2;
    char padA4[0x2];
    s16 unkA6;
};

struct Struct150B9D8CTmp {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    char pad7[0x1];
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    char pad16[0x2];
    s32 unk18;
    char pad1C[0x10];
};

void func_150B9D8C(struct Struct150B9D8C *arg0) {
    struct Struct150B9D8CTmp tmp;
    s32 c;
    s32 b;
    s32 a;

    if (arg0->unk98 == -1) {
        tmp.unk6 = 4;
        tmp.unk8 = 0;
        tmp.unkC = 0;
        tmp.unk0 = 0x11;
        tmp.unk4 = 0x64;
        tmp.unk10 = 0xFF;
        tmp.unk11 = arg0->unk90;
        tmp.unk12 = 0;
        tmp.unk13 = 0;
        tmp.unk14 = 0;
        tmp.unk15 = 0xFF;
        tmp.unk18 = 0x30001;

        a = func_150ADA20();
        b = func_150ADA20();
        c = func_150ADA20();

        func_1513C4EC((s32)&tmp, 0, 2, 0,
                      (f32)arg0->unk9C,
                      (f32)(arg0->unkA6 + 2),
                      (f32)arg0->unkA0,
                      (f32)arg0->unkA2,
                      (f32)arg0->unkA2,
                      a & 0xFF,
                      ((c & 1) << 1) + (b & 1),
                      3, 0xFF, 0,
                      arg0->unkC,
                      arg0->unk1);
    }
}
