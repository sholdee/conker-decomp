#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8; its asm returns a full word
 * (dsra32 v0), and func_151AF388 spills the result as a word. File-local prototype. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_s32_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A9D84;
extern f32 D_800A9D88;
extern f32 D_800A9D80;
extern void func_151D5D60();
void *func_1513418C(void *, s32, s32, s32);
void func_1516962C(s32, struct102 *, s32);
void func_151AE984(f32 *, f32, f32, f32, s32, u8);
void func_151AEAB4(f32 *, s32, u8);
void func_15143134(f32 *, f32 *, s32);
void func_15135DD0(f32 *, f32 *, f32, u8, s32);
void func_151AF388(f32 *, f32, f32, f32, s32);

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 unk4[3];
    /* 0x10 */ f32 unk10[3];
} struct_AF4D0;

typedef struct {
    char pad_0[0x74];
    u8 field_0x74;
    char pad_0x75[0x15F];
    s32 field_0x1D4;
} GameObjectTransformOwner;

typedef struct {
    char pad_0[0x44];
    s32 field_0x44;
    char pad_0x48[0x4];
    f32 field_0x4C;
    s8 field_0x50;
    s8 field_0x51;
    s16 field_0x52;
} ObjectFxPayload;

typedef struct {
    char pad_0[0x98];
    s8 field_0x98;
    char pad_0x99[0x3];
    ObjectFxPayload *field_0x9C;
    char pad_0xA0[0xE];
    u8 field_0xAE;
} ObjectFxControl;

typedef struct {
    char pad_0[0x3C];
    f32 field_0x3C;
    char pad_0x40[0x36];
    u16 field_0x76;
    char pad_0x78[0x2A4];
    ObjectFxControl *field_0x31C;
} GameObjectFxOwner;

typedef struct {
    char pad_0[0x10];
    s32 field_0x10;
    s32 field_0x14;
    u8 field_0x18;
    char pad_0x19[0x3];
    s32 field_0x1C;
    u8 field_0x20;
    char pad_0x21[0x3];
    s32 field_0x24;
    f32 field_0x28;
} ObjectLinkPayload;

typedef struct {
    char pad_0[0x4];
    s16 field_0x04;
    u16 field_0x06;
    f32 field_0x08;
    f32 field_0x0C;
    s16 field_0x10;
    u16 field_0x12;
    u8 field_0x14;
    u8 field_0x15;
    char pad_0x16[0x2];
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
} ObjectSpawnParams;

typedef struct {
    s32 field_0x00;
    s32 field_0x04;
    u8 field_0x08;
} ObjectSpawnPayload;

extern struct_AF4D0 D_800A9DF0[];
extern f32 D_800AA0E4;
extern s8 D_800A9023[];
extern ObjectSpawnParams D_800A9180[];

void func_15143794(s32, s32, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

typedef struct {
    f32 unk0;
    s16 unk4;
    u16 unk6;
} FxSoundEntry;

typedef struct {
    f32 unk0;
    s16 unk4;
    f32 unk8;
} FxLightEntry;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8 unk4;
} FxEntry5;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} FxOwner;

extern FxEntry5 D_800A9020[];
extern FxSoundEntry D_800A9040[];
extern FxSoundEntry D_800A9158[];
extern FxLightEntry D_800A925C[];
void func_1514C678(f32, f32, s32, f32, s32, s32, s32, s32, s32, f32, ObjectSpawnPayload *, s32);
u8 func_151D8E20(void);
void func_151DBE80(u8, f32, f32, s16, f32 *, s32, s32, s32, s32, s32);
s32 func_151ABE40(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
} FxSprayParams;

typedef struct {
    f32 pos[3];
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} FxSpraySpawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
} FxSprayPart;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} FxSprayStyle;

extern s8 D_800A9021[];
extern FxSprayParams D_800A9068[];
extern u8 D_800AB3F4[];
extern u8 D_800AB404[];
extern s32 func_15147DA0(FxSpraySpawn *, FxSprayPart *, s32, s32, s32, s32, s32, s32, s32, s32, s32, FxSprayStyle *, s32, s32, s32);
s32 func_151AC078(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, ObjectSpawnPayload *argD, u8 argE);
s32 func_151AC3CC(s16 *arg0);

u8 func_151D8E20(void);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);

typedef struct {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
} ObjectEffectSample;

typedef struct {
    f32 field_0x00;
    char pad_0x04[0x17];
    u8 field_0x1B;
    char pad_0x1C[0x4];
    u8 field_0x20;
} ObjectEffectPayloadData;

typedef struct {
    char pad_0[0x1];
    u8 field_0x01;
    char pad_0x02[0xA];
    u8 field_0x0C;
    char pad_0x0D[0x20];
    s8 field_0x2D;
    char pad_0x2E[0x66];
    ObjectEffectSample *field_0x94;
    ObjectEffectPayloadData *field_0x98;
} ObjectEffectState;
s32 func_151AC408(ObjectEffectState *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_151AC550(ObjectEffectState *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_151AC61C(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, ObjectSpawnPayload *argD, u8 argE);

typedef struct {
    char pad_0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3C;
    char pad_0x40[0xC];
    f32 field_0x4C;
    f32 field_0x50;
    char pad_0x54[0x6C];
    char field_0xC0[0x40];
    s16 *field_0x100[1];
} ObjectGeometryState;
s16 *func_151AC810(ObjectGeometryState *arg0, s16 arg1);

extern f32 D_800BE9A4;
s32 func_151AC9EC(ObjectGeometryState *arg0);
s32 func_151ACA20(s16 *arg0);

extern u8 *func_15167A68(s32, s32, s32, s32, s32, s32);
s32 func_151ACB38(u8 *arg0, u8 *arg1);
s32 func_151ACA60(struct127 *arg0, f32 arg1, s32 arg2);
s32 func_151ACB38(u8 *arg0, u8 *arg1);

s32 func_151AE3A8(u8 *arg0);
s32 func_151ACB60(u8 *arg0);
void func_151ACB94(s32 arg0, s32 arg1, u8 arg2);

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 (*unk38)[6];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ Vtx *unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[2];
    /* 0x52 */ s16 unk52;
} Obj151ACBD4;

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u8 padA[2];
} Tex151ACBD4;

extern Tex151ACBD4 D_80090B60[];
extern void *allocate_memory(s32, s32, s32, s32);
Obj151ACBD4 *func_151ACBD4(void *arg0, s32 arg1);

void func_151AE0E4(u8 *arg0, u8 arg1);
void func_151AE264(GameObjectFxOwner *arg0);
void func_151AE06C(struct127 *arg0, struct133 *arg1);

typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[0x10];
} Key151AE2BC;

typedef struct {
    u8 pad0[0x38];
    Key151AE2BC *unk38;
    u8 pad3C[6];
    u8 unk42;
} Track151AE2BC;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 padC[0xC];
} Key151AE0E4;

typedef struct {
    u8 pad0[0x38];
    Key151AE0E4 *keys;
    f32 unk3C;
    u8 pad40[2];
    u8 unk42;
    u8 pad43;
    void *unk44;
    u8 pad48[0xA];
    s16 unk52;
} Node151AE0E4;

typedef struct {
    u8 pad0[0x98];
    u8 unk98;
    u8 pad99[3];
    Node151AE0E4 *unk9C;
    f32 unkA0;
} Ctl151AE0E4;

typedef struct {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x6A];
    u8 unk8A;
    u8 pad8B[0x79];
    u8 unk104;
    u8 pad105[0xC5];
    u8 unk1CA;
    u8 pad1CB[0x151];
    Ctl151AE0E4 *unk31C;
} Obj151AE0E4;

typedef struct Node151AE590 {
    u8 pad0[0x8];
    struct Node151AE590 *next;
    u8 padC[0x4];
    u8 unk10;
} Node151AE590;

Node151AE590 *func_151AE590(u8 arg0);
void func_151AE2BC(f32 *arg0, Track151AE2BC *arg1, f32 arg2, f32 arg3, f32 arg4);
void func_151AE0E4(u8 *arg0, u8 arg1);

extern f32 D_800A9294;
void func_151AE264(GameObjectFxOwner *arg0);
void func_151AE2BC(f32 *arg0, Track151AE2BC *arg1, f32 arg2, f32 arg3, f32 arg4);

extern Node151AE590 *D_800DCE50[][104];
extern s32 D_800A9270[];
Node151AE590 *func_151AE590(u8 arg0);
void func_151AE640(s32 *arg0, s32 *arg1, u8 arg2);


extern f32 D_800A9D70;
s32 func_15045800(f32 *arg0, s32 arg1, f32 arg2, s32 arg3);
s32 func_151ABE40(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_10010FFC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_151AE6B0(u8 *arg0);


void *func_151AE7B0(struct102 *arg0, f32 arg1, s16 arg2, u8 arg3, s32 arg4) {
    void *result;
    struct {
        s32 unk0;
        s32 unk4;
        u8 unk8;
        u8 pad9[3];
        struct102 *unkC;
        u8 unk10;
        u8 pad11[3];
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        s16 unk28;
        u8 unk2A;
        u8 unk2B;
        s8 unk2C;
        u8 unk2D;
    } sp1C;

    if (arg0 == 0) {
        return 0;
    }

    func_1516962C(0x28, arg0, 0x11);
    sp1C.unk0 = 0;
    sp1C.unk4 = 0;
    sp1C.unk8 = *((u8 *)arg0 + 0x3B);
    sp1C.unkC = arg0;
    sp1C.unk10 = 1;
    sp1C.unk14 = 0.0f;
    sp1C.unk18 = 0.0f;
    sp1C.unk1C = 0.0f;
    sp1C.unk20 = D_800A9D80;
    sp1C.unk24 = arg1;
    sp1C.unk2A = 2;
    if (arg2 != 0) {
        sp1C.unk28 = arg2;
        sp1C.unk2A = 6;
    } else {
        sp1C.unk28 = 0x12C;
    }
    sp1C.unk2B = 6;
    sp1C.unk2C = -1;
    sp1C.unk2D = 5;
    result = func_1513418C(&sp1C, 0, arg3, arg4);
    return result;
}
void func_151AE890(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 *arg6);
void func_151AE984(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, u8 arg5);

f32 func_150ADA68(void);
f32 func_151423D8(u8);
extern f32 D_800A9D98;
extern f32 D_800A9D9C;
extern f32 D_800A9DA0;
extern f32 D_800A9DA4;
void func_151AEAB4(f32 *, s32, u8);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);
void func_151AEAB4(f32 *arg0, s32 arg1, u8 arg2);


typedef struct {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    f32 position[3];
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    u8 field3C;
    u8 pad3D[3];
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Packet_151AECA0;

typedef struct {
    f32 y;
    u8 unk4[0x19];
    u8 unk1D;
    u8 pad1E[6];
} Floor_151AECA0;

void func_15153F18(s16 *, void *, s32, s32, s32);
void func_1504715C(void *, void *);
s32 func_15046C80(f32 *, s32, f32, void *);
void func_15143874(s16, f32, f32 *, f32 *);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, s32, s32);
void func_151AECA0(f32 *arg0, u8 arg1, s32 arg2);

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ struct102 *unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11[0x3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8  unk2A;
    /* 0x2B */ u8  unk2B;
    /* 0x2C */ s8  unk2C;
    /* 0x2D */ u8  unk2D;
} struct_AF270;

extern f32 D_800A9DCC;
extern f32 D_800A9DD0;
void func_1516962C(s32, struct102 *, s32);
void *func_1513418C(void *, s32, s32, s32);
void *func_151AF270(struct102 *arg0, s32 arg1, s32 arg2);
void func_151AF338(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6);

typedef u32 (*RandU32Fn_151AF388)(void);
typedef void (*SpawnFn_151AF388)(f32 *, f32 *, s32, f32, s32, s32, f32, s32,
                                 f32, f32, s32, s32, s32, s32, s32, s32);
void func_151AF388(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void func_151AF4D0(GameObjectTransformOwner *arg0, s32 arg1, s32 arg2, s32 arg3);
