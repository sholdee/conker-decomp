#include <ultra64.h>
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_1513D524
#include "variables.h"

extern void func_1510F800(s32);
extern s32 func_1513D524(s32, u8, u8, u8, u8, u8, s32, u8, s32);
extern void func_1513F680();
extern void func_151429E0(u8, u8 *, u8 *, u8 *);
extern void func_1518CA80(void *, u8);
extern void func_151D5D60();

typedef s32 (*Func1514C2F0Callback)(s32, s16, f32, f32, f32, f32, f32, f32,
                                    s32, s32, f32, s32, f32, s32, u8);
extern Func1514C2F0Callback D_8008AA00[];

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg;

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 pad46[2];
    s32 unk48;
    s32 pad4C;
    s32 pad50;
    s32 pad54;
} Struct15149550Local;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    s16 unkC;
    s16 unkE;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    f32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s16 unk38;
    s16 unk3A;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    u8 unk48;
    u8 pad49[3];
    f32 unk4C;
} Struct15149550Extra;
s32 func_15149550(Struct15149550Arg *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);

typedef struct {
    char pad_0x0[0x1C];
    s16 unk1C;
    char pad_0x1E[0x6];
    s32 unk24;
    char pad_0x28[0x4];
    f32 unk2C;
    f32 unk30;
    char pad_0x34[0x24];
    s32 unk58;
    char pad_0x5C[0x14];
    u8 unk70;
    char pad_0x71[0x1];
    u8 unk72;
    u8 unk73;
    char pad_0x74[0x9C];
    Struct15149550Extra unk110;
} Struct15149838Obj;
s32 func_15149838(Struct15149838Obj *arg0);

typedef struct {
    char pad_0x0[0x8];
    f32 field_0x8;
    char pad_0xC[0x14];
    f32 field_0x20;
    char pad_0x24[0x20];
    f32 field_0x44;
} Struct15149550ObjExtra;

typedef struct {
    char pad_0x0[0x1C];
    s16 field_0x1C;
    char pad_0x1E[0xE];
    f32 field_0x2C;
    f32 field_0x30;
    char pad_0x34[0x24];
    s32 field_0x58;
    char pad_0x5C[0x14];
    u8 field_0x70;
    char pad_0x71[0x1];
    u8 field_0x72;
    u8 field_0x73;
    u8 field_0x74;
    char pad_0x75[0x9B];
    Struct15149550ObjExtra field_0x110;
    u8 field_0x158;
} Struct15149550Obj;
s32 func_15149A94(Struct15149550Obj *arg0);

struct Foo15149BF4 {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    char pad34[0x11C];
    f32 unk150;
};
s32 func_15149BF4(struct Foo15149BF4 *arg0);

extern f32 D_800BE9A4;
s32 func_15149C58(f32 *arg0);

struct Foo15149D18 {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[1];
};
s16 *func_15149D18(struct Foo15149D18 *arg0, s16 arg1);

typedef struct {
    char pad0[0x34];
    struct17 unk34;
} Struct15149EC4Arg;

typedef struct {
    struct17 unk0;
    char padC[0xA];
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
} Struct15149EC4Stack;
s32 func_15149EC4(Struct15149EC4Arg *arg0);
s16 *func_15149FD0(struct Foo15149D18 *arg0, s16 arg1);

struct Foo1514A19CObj {
    char pad0[0x24];
    s32 unk24;
    char pad28[0x4];
    f32 unk2C;
    f32 unk30;
};

struct Foo1514A19CSub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s16 unk1C;
    s16 unk1E;
    char pad20[0x18];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
};
s32 func_1514A19C(struct Foo1514A19CObj *arg0);

typedef struct {
    char pad_0x0[0x8];
    f32 field_0x8;
    char pad_0xC[0xC];
    f32 field_0x18;
} Struct1514A380ObjExtra;

typedef struct {
    char pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    char pad_0x34[0x3C];
    u8 field_0x70;
    char pad_0x71[0x1];
    u8 field_0x72;
    u8 field_0x73;
    u8 field_0x74;
    char pad_0x75[0x9B];
    Struct1514A380ObjExtra field_0x110;
} Struct1514A380Obj;
s32 func_1514A380(Struct1514A380Obj *arg0);

struct Foo1514A498 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0xE];
    f32 unk2C;
    f32 unk30;
    char pad34[0x28];
    u8 unk5C;
    char pad5D[0xE7];
    f32 unk144;
    char pad148[0xE];
    s16 unk156;
    s16 unk158;
};
s32 func_1514A498(struct Foo1514A498 *arg0);

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
} Struct1514A4ECVec;

struct Foo1514A4EC {
    char pad0[0x34];
    Struct1514A4ECVec unk34;
    Struct1514A4ECVec unk40;
    char pad4C[0xE4];
    f32 unk130;
    f32 unk134;
    f32 unk138;
    f32 unk13C;
    f32 unk140;
};
s32 func_1514A4EC(struct Foo1514A4EC *arg0);
void func_1514AB5C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, s16 arg9, s16 arg10, u8 arg11);

u8 func_150ADA20(void);
f32 func_150ADA68(void);
s32 func_15149550(Struct15149550Arg *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);
void func_1514AD9C(struct17 *arg0, u8 arg1, s32 arg2);


extern f32 D_800BE9A4;
s32 func_1514AF74(f32 *arg0);

extern void func_15143874(s16, f32, f32 *, f32 *);
void func_1514B034(struct17 *arg0, u8 arg1, s32 arg2);

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
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
    s32 pad4C;
    s32 pad50;
    s16 unk54;
    s16 unk56;
} Struct1514B8E4Desc;

typedef struct {
    s32 unk0;
    s32 unk4;
    struct17 unk8;
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
    u16 unk40;
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
    u8 pad6B[1];
    f32 unk6C;
    s8 unk70;
    s8 unk71;
    s8 unk72;
    s8 unk73;
} Struct1514B364Spawn;

typedef struct {
    struct17 unk0;
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
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
} Struct1514B364Emit;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[0x7];
} Struct1514B364Hit;

extern s32 func_15046C80(struct17 *, s32, f32, Struct1514B364Hit *);
extern void func_15152B38(Struct1514B364Spawn *, u8, s32);
extern void func_15152F70(Struct1514B364Emit *, u8);
void func_1514B364(struct17 *arg0, Struct1514B364Hit *arg1, u8 arg2, s32 arg3);

struct Foo1514B844 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x7A];
    u8 *unk98;
};
s32 func_1514B844(struct Foo1514B844 *arg0);

extern f32 D_800BE9A4;

struct Foo1514B87C {
    char pad2C[0x2C];
    f32 unk2C;
    f32 unk30;
    char pad34[0x18];
    f32 unk4C;
};
s32 func_1514B87C(struct Foo1514B87C *arg0);

struct Foo1514B8B0 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x3E];
    u8 unk5C;
};
s32 func_1514B8B0(struct Foo1514B8B0 *arg0);

typedef struct {
    f32 unk0;
    f32 unk4;
} Struct1514B8E4Size;

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
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct17 unk38;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
} Struct1514B8E4Extra;
void *func_1514B8E4(struct17 *arg0, Struct1514B8E4Size *arg1, s16 arg2, u8 arg3, struct17 *arg4, f32 arg5, f32 arg6, f32 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB, u8 argC, s16 argD, s16 argE, s32 argF, u8 arg10, s32 arg11);

struct Foo1514BC08Obj {
    char pad0[0x24];
    s32 unk24;
    char pad28[0x4];
    f32 unk2C;
    f32 unk30;
};

struct Foo1514BC08Sub {
    f32 unk0;
    f32 unk4;
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
};
s32 func_1514BC08(void *arg0, void *arg1);
void func_1514BE00(u8 *arg0);

typedef struct {
    char pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    char pad4C[0x15C - 0x4C];
    f32 unk15C;
} Struct1514BE20;

extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

void func_1514BE20(void *arg) {
    struct17 target;
    struct17 lerp;
    Struct1514BE20 *obj;
    struct17 diff;
    f32 len;
    f32 inv;

    obj = arg;
    target.unk0 = obj->unk34.unk0;
    target.unk4 = obj->unk34.unk4 + 100.0f;
    target.unk8 = obj->unk34.unk8;
    lerp.unk0 = obj->unk40.unk0 + (target.unk0 - obj->unk40.unk0) * obj->unk15C;
    lerp.unk4 = obj->unk40.unk4 + (target.unk4 - obj->unk40.unk4) * obj->unk15C;
    lerp.unk8 = obj->unk40.unk8 + (target.unk8 - obj->unk40.unk8) * obj->unk15C;
    {
        s32 pad;

        diff.unk0 = lerp.unk0 - obj->unk34.unk0;
        diff.unk4 = lerp.unk4 - obj->unk34.unk4;
        diff.unk8 = lerp.unk8 - obj->unk34.unk8;
        if (func_15145128(&diff, &diff, &len, &inv)) {
            obj->unk40.unk0 = obj->unk34.unk0 + diff.unk0 * 100.0f;
            obj->unk40.unk4 = obj->unk34.unk4 + diff.unk4 * 100.0f;
            obj->unk40.unk8 = obj->unk34.unk8 + diff.unk8 * 100.0f;
        } else {
            obj->unk40 = obj->unk34;
        }
    }
}
void func_1514BF50(f32 *arg0);
void func_1514BF7C(u8 *arg0);

typedef struct {
    char pad0[0x38];
    struct17 unk38;
    f32 unk44;
    f32 unk48;
} Struct1514BF9CSub;

typedef struct {
    char pad0[0x34];
    struct17 unk34;
    char pad40[0xD0];
    Struct1514BF9CSub unk110;
} Struct1514BF9CObj;
void func_1514BF9C(Struct1514BF9CObj *arg0);

extern void func_1514BE20(void *);
s32 func_1514C258(void *arg0);
s32 func_1514C288(void *arg0);

struct Foo1514C2B8 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x3A];
    s32 unk58;
    char pad5C[0x15];
    u8 unk71;
    u8 unk72;
    char pad73[0xE1];
    f32 unk154;
    char pad158[0x8];
    f32 unk160;
    s16 unk164;
};
s32 func_1514C2B8(struct Foo1514C2B8 *arg0);
void func_1514C2F0(f32 arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, s8 arg5, s16 arg6, u8 arg7, s32 arg8, f32 arg9, s32 arg10, u8 arg11);
void func_1514C470(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, u8 arg7, s32 arg8, f32 arg9, s32 arg10, u8 arg11);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 arg8, f32 arg9, s32 argA, u8 argB);
void func_1514C858(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7, u8 arg8, s32 arg9, f32 argA, s32 argB, u8 argC);
