#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A1EB0;
extern f32 D_800A1EB4;
extern f32 D_800A1EB8;
extern f32 D_800A1EBC;
extern f32 D_800A1EC0;
extern f32 D_800A1EC4;
extern f32 D_800A1EC8;
extern f32 D_800A1ECC;
extern f32 D_800A1ED0;
extern struct260 *func_15164780(void *, s32, u8, s32);

typedef struct {
    u8 pad0[0x23D];
    u8 unk23D;
} Sub318_150FC438;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x2DC];
    Sub318_150FC438 *unk318;
} Arg1_150FC438;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
} Arg0_150FC438;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[3];
    f32 unk8;
    u8  unkC;
    u8  unkD;
    u8  padE[2];
    s32 unk10;
    s16 unk14;
    s8  unk16;
    u8  pad17;
    Arg1_150FC438 *unk18;
    u8  unk1C;
    u8  unk1D;
    u8  pad1E[2];
} Data_150FC438;

typedef union {
    s64 align;
    struct {
        u8  unk0;
        s8  unk1;
        s8  unk2;
        u8  pad3;
        s16 unk4;
        u8  unk6;
        u8  pad7;
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
    } f;
} Child_150FC438;

void func_151C3B0C(void *, f32, f32, f32, f32, s32, s32, s32);
void func_150FC368(void *arg0);
void func_150FC438(Arg0_150FC438 *arg0, Arg1_150FC438 *arg1, u8 arg2, u8 arg3);

typedef struct {
    u8  pad0[0x1AA];
    s16 unk1AA;
} Sub31C_150FC614;

typedef struct {
    u8  pad0[0x31C];
    Sub31C_150FC614 *unk31C;
} Holder_150FC614;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[0x36];
    u8  unk3B;
    u8  pad3C[0x48];
    u16 unk84;
} Owner_150FC614;

typedef struct {
    Owner_150FC614 *unk0;
    u8  unk4;
    u8  pad5[3];
    f32 unk8;
    u8  unkC;
    u8  unkD;
    u8  padE[2];
    s32 unk10;
    s16 unk14;
    s8  unk16;
    u8  pad17;
    Holder_150FC614 *unk18;
    u8  unk1C;
    u8  unk1D;
    u8  pad1E[2];
} Data_150FC614;

typedef struct {
    u8  pad0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD;
    s16 unkE;
    u8  pad10[0x18];
    Data_150FC614 unk28;
} Entity_150FC614;

typedef struct {
    u8  unk0;
    u8  unk1;
    s16 unk2;
    u8  unk4;
    u8  unk5;
    s8  unk6;
    u8  unk7;
} Spark_150FC614;

extern void func_150FB8C0(Owner_150FC614 *arg0, u8 arg1, f32 arg2, u8 arg3, u8 arg4, u8 arg5);
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_150FC614(Entity_150FC614 *arg0);

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[0x13];
    s32 unk18;
    u8  unk1C;
} SubA_150FC818;

typedef struct {
    s32 unk0;
    union { s32 w4; u8 b4; } u4;
    u8  unk8;
    u8  unk9;
} ArgB_150FC818;
void func_150FC818(void *arg0, s32 arg1, u8 arg2);

s32 func_150FC930(struct260 *arg0, s32 arg1, s32 arg2) {
    u8 v = *((u8 *)arg0 + 4);
    if (v == 0x12 || v == 0x23 || v == 0x73 || v == 0x8A) {
        return 0;
    }
    return 1;
}

void func_150FC974(struct260 *arg0) {
    s32 *p = (s32 *)((u8 *)arg0 + 0x38);
    if (*p != 0) {
        func_1516972C((struct102 *)*p);
    }
}

extern void func_150FC974(struct260 *);

void func_150FC9A4(struct260 *arg0) {
    func_150FC974(arg0);
    func_1514933C(arg0);
}

extern void func_15149368(struct260 *);
void func_150FC9D0(struct260 *arg0);
