#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A8DB0;

extern void func_151A6F00(void *, s32, u8, s32);
extern void func_151D5334(void *, f32, f32, f32, s32, u8, s32);
extern void func_151D3FF4(void *, u8, s32);
extern void func_151D5514(void *, u8, s32);
extern void func_151541B8(void *, f32, s32, f32, f32, u8, s32);

typedef struct { s16 x; s16 y2; s16 z; s16 r6; s16 h8; s16 rA; u8 pad[4]; f32 ang; u8 b14; u8 b15; } F5660Src;
typedef struct {
    F5660Src *src; f32 f4; f32 f8; f32 fC; f32 f10; f32 f14; f32 f18; f32 f1c; f32 f20;
    s16 s24; s16 s26; f32 f28; f32 f2c; f32 f30; f32 f34; f32 f38; f32 f3c;
    u8 b40; u8 b41; u8 b42; u8 pad43; u8 pad44[4]; f32 f48; u8 pad4c[0x20]; s32 h6c;
} F5660Obj;
typedef struct { f32 x, y, z, f8c, fcc, fd0, fd4, fd8; s16 sdc, sde, se0; u8 be2, be3, be4, be5; s16 se6; u8 be8, be9, bea, beb; s32 sec; u8 bf0; f32 ff4, ff8, ffc, f100; } F5660P;
extern s32 func_151464B8(s32);
extern s32 func_15046C80(f32 *, s32, f32, f32 *);
extern void func_15149550(void *, s32, s32, s32, s32, s32);
void func_151A5660(u8 *arg0);

void func_151A5D2C(u16 arg0);
void func_151A5CAC(struct260 *arg0);
void func_151A5CEC(struct260 *arg0);
void func_151A5D2C(u16 arg0);

typedef struct {
    s32 field_0x00;
    s16 field_0x04;
    u8 field_0x06;
    u8 field_0x07;
    s32 field_0x08;
    s32 field_0x0C;
    u8 field_0x10;
    u8 field_0x11;
    u8 field_0x12;
    u8 field_0x13;
    u8 field_0x14;
    u8 field_0x15;
    u8 field_0x16;
    u8 field_0x17;
    s32 field_0x18;
    s32 field_0x1C;
    u8 field_0x20;
    u8 pad_0x21;
    s16 field_0x22;
    s16 field_0x24;
    u8 pad_0x26[0x6];
} SparkSpawnParams;
struct210 *func_151A5D58(f32 arg0, u8 arg1, s32 arg2, f32 *arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7, s32 arg8);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Obj151A6068;

void func_151A6068(Obj151A6068 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);

typedef struct Game1D3420Message {
    s32 count;
    u8 *ids;
    s32 value;
} Game1D3420Message;

typedef struct Game1D3420Target {
    u8 *object;
    u8 id;
    u8 pad05[3];
    s32 argument;
    u8 flags;
    u8 pad0D[3];
    s32 value;
} Game1D3420Target;
void func_151A5F70(u8 *arg0, void *arg1, u8 arg2);

typedef struct {
    Obj151A6068 *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    f32 unk18;
} Ext151A6068;

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
    u8 unk4C;
    u8 pad4D[0xB];
} Spawn151A6068;

extern s32 func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);
void func_151A6068(Obj151A6068 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);

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
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    s8 unk65;
    u8 pad66[2];
} Spawn151A6600;

typedef struct {
    s16 *unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[8];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 pad17;
    f32 unk18;
} Sub151A6600;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x27];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x20];
    s32 unk60;
    u8 pad64[0xAC];
    Sub151A6600 unk110;
    u8 pad12C[0x127 - 0x12C + 0x10];
} Obj151A6600;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, struct17 *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern f32 func_15144528(f32, f32, f32);
extern s32 func_151464B8(s32);

s32 func_151A6600(Obj151A6600 *arg0) {
    Sub151A6600 *sub;
    s32 pad[2];

    sub = &arg0->unk110;
    if (*((u8 *)arg0 + 0x127) != 0) {
        func_1508B20C(arg0->unk34, arg0->unk38, arg0->unk3C, 500.0f);
    }
    sub->unk4 += sub->unk8 * D_800BE9A4;
    sub->unk4 = func_15144528(sub->unk4, 2048.0f, 0.0f);
    sub->unk14 += sub->unk15 * D_800BE9E4;
    sub->unk16 = func_151423D8(sub->unk14 - 0x40) * 63.0f + 192.0f;
    if (func_151464B8(arg0->unk60) == 0) {
        sub->unk18 += (0.3930000067f + func_150ADA68() * 0.4370000064f) * D_800BE9A4;
        if (sub->unk18 > 1.0f) {
            Spawn151A6600 spawn;

            spawn.unk1D = 0x28;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x1F0600;
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0xFF;
            spawn.unk19 = 0xFF;
            spawn.unk3C.unk0 = 0.0f;
            spawn.unk3C.unk4 = 0.0f;
            spawn.unk3C.unk8 = 0.0f;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk1A = 0xFF;
            spawn.unk1E = 0xC;
            spawn.unk20 = 0x15;
            spawn.unk22 = 1;
            spawn.unk24 = 1.0f;
            spawn.unk58 = 0xC207;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            do {
                func_15143874((u8)func_150ADA20(), sub->unk0[3] * func_150ADA68(), &spawn.unk30.unk0, &spawn.unk30.unk8);
                spawn.unk30.unk0 += sub->unk0[0];
                spawn.unk30.unk4 = sub->unk0[1];
                spawn.unk30.unk8 += sub->unk0[2];
                spawn.unkA = (func_150ADA20() & 0xF) + 0x19;
                spawn.unk1B = func_150ADA20() % 101U + 100;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 205.0f + 98.0f;
                func_15143794((u8)func_150ADA20(), func_150ADA20() % 35U - 0x40, func_150ADA68() * 10.0f + 10.0f, &spawn.unk48);
                spawn.unk54 = func_150ADA68() * 0.2840000093f + -0.9100000262f;
                func_15130280(&spawn, 1, 0, 0, arg0->unkC, arg0->unk1);
                sub->unk18 -= 1.0f;
            } while (sub->unk18 > 1.0f);
        }
    }
    return 1;
}

extern void func_1000FD38(void *, void *, s32);
extern s32 func_1000EF40();

void func_151A6AB8(struct210 *arg0) {
    struct102 *temp;
    *(u8 *)(*(s32 *)((u8 *)arg0 + 0x110) + 0x14) = 1;
    func_1000FD38(func_1000EF40, *(void **)((u8 *)arg0 + 0x110), 0);
    temp = *(struct102 **)((u8 *)arg0 + 0x120);
    if (temp != 0) {
        func_1516972C(temp);
    }
}

extern void func_151A6AB8(struct210 *);
void func_151A6B10(struct210 *arg0);
void func_151A6B3C(struct210 *arg0);

struct Func151A6BD8Obj;
extern struct Func151A6BD8Obj *func_151A6BD8(s32 arg0);

typedef struct Obj151A6B68 {
    char pad0[0x7A];
    /* 0x7A */ u16 unk7A;
} Obj151A6B68;

extern void func_1505D024(Obj151A6B68 *, s32, u16, s32);
void func_151A6B68(Obj151A6B68 *arg0, s32 arg1);

extern s32 D_800A5770;
extern s32 D_800DCE50[][0x68];
typedef struct Func151A6BD8Obj
{
  u8 pad0[8];
  struct Func151A6BD8Obj *next;
  u8 padC[7];
  u8 type;
  u8 pad14[0x14];
  s32 owner;
} Func151A6BD8Obj;
Func151A6BD8Obj *func_151A6BD8(s32 arg0);
void func_151A6C90(void *arg0, u8 arg1, s32 arg2);
