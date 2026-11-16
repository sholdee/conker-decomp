#include <ultra64.h>
#define func_10010F30 func_10010F30_void
#define func_1513D4B8 func_1513D4B8_void
#include "functions.h"
#undef func_10010F30
#undef func_1513D4B8
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern void *func_1513D4B8(void *, void *, u8, u8, u8, u8, u8, s32, u8, s32);
#include "variables.h"

extern void func_151D5D60();
extern s32 func_15144E80(struct17 *, struct17 *, struct17 *, struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;

typedef struct {
    ObjectWithState197 *field_0x0;
    char pad_0[0x1E];
    u8 field_0x22;
    char pad_1[0x1];
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
} Field124ChildBlock;

typedef struct {
    char pad_0[0x18];
    s32 field_0x18;
    char pad_1[0x8C];
    s32 field_0xA8;
    char pad_2[0x24];
    s32 field_0xD0;
    u8 field_0xD4;
    char pad_3[0x4F];
    s32 field_0x124;
    char pad_4[0x48];
    s32 field_0x170;
    u8 field_0x174;
} Field124Owner;

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[4];
    u8 unk110;
    u8 unk111;
    u8 unk112[0xC];
} Struct15101F50;


typedef struct {
    u8 *obj;
    u8 unk4;
    u8 pad5[3];
    struct17 unk8;
    struct17 unk14;
    u8 unk20;
    s8 unk21;
    u8 unk22;
    u8 unk23;
    u8 pad24[0x14];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    s16 unk90;
    u8 pad92[2];
} Data15100600;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10, unk11, unk12, unk13;
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
    u8 unk44, unk45, unk46, unk47;
    u8 pad48[0x10];
} Spawn15100600;

typedef struct {
    u8 unk0;
    u8 pad1[0x13];
    void *unk14;
} Data2_15100600;

typedef struct {
    u8 unk0;
    s8 unk1;
    s16 unk2;
    u8 unk4;
    u8 pad5[3];
} Hdr15100600;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Hdr2_15100600;

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14, unk15, unk16, unk17;
    u8 unk18, unk19, unk1A, unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 unk64;
    u8 unk65;
    u8 pad66[0xA];
} Effect15100600;

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
    void *unk28;
    u8 unk2C;
    u8 pad2D[3];
} Data3_15100600;

typedef struct {
    f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
    s32 unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44;
    f32 unk48;
    s16 unk4C, unk4E, unk50, unk52;
    u8 unk54, unk55, unk56, unk57, unk58;
    u8 pad59[3];
} Emit15100600;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
} Data4_15100600;

extern void *func_15130374(void *, u8, s32, u8, s32);
extern void *func_151407D0(void *, s32, void *, u8, u8, u8, u8, s8, u8, s32);
extern void *func_151602C0(void *, void *, s32, u8, u8, u8, u8, u8, s32, u8, s32);

s32 func_15100600(u8 *arg0, f32 *arg1, f32 *arg2, u8 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, u8 argC, u8 argD, s32 argE) {
    u8 *ret;
    Spawn15100600 spawn;
    Data15100600 data;
    u8 flag;
    Field124ChildBlock *p;

    if (arg4 == -1) {
        flag = 0;
        arg4 = 0x12C;
    } else {
        flag = 1;
    }
    data.unk21 = (struct127 *)arg0 - D_800CC2D0;
    if (*(void **)(arg0 + 0x318) != NULL) {
        data.unk22 = (*(u8 **)(arg0 + 0x318))[0x23D];
    } else {
        data.unk22 = 0xFF;
    }
    data.obj = arg0;
    data.unk4 = arg0[0x3B];
    data.unk20 = arg3;
    data.unk23 = (s32)(argC ? 2 : 0);
    data.unk8 = *(struct17 *)arg1;
    data.unk14 = *(struct17 *)arg2;
    data.unk3C = arg8;
    data.unk40 = 0.1019999981f;
    data.unk48 = arg9;
    data.unk4C = argA;
    data.unk54 = argB;
    data.unk58 = -32768.0f;
    data.unk64 = 250.0f;
    data.unk6C = 25.0f;
    data.unk38 = 0.0f;
    data.unk50 = 0.0f;
    data.unk5C = 0.0f;
    data.unk60 = 0.0f;
    data.unk68 = 0.0f;
    data.unk78 = 0.0f;
    data.unk84 = 0.0f;
    data.unk70 = 0.4499999881f;
    data.unk74 = 255.0f;
    data.unk7C = 128.0f;
    data.unk80 = 127.0f;
    data.unk88 = 20.0f;
    data.unk8C = 0.400000006f;
    if (*(void **)(arg0 + 0x318) != NULL) {
        data.unk90 = func_10010F30(0x4D0, 0x7D0, 0x40, 0, 0);
    } else {
        data.unk90 = 0;
    }
    spawn.unk0 = 0x59;
    spawn.unk1 = 1;
    spawn.unk2 = 0x3103;
    spawn.unk4 = arg4;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.unk10 = arg5;
    spawn.unk11 = arg6;
    spawn.unk12 = arg7;
    spawn.unk13 = 0xFF;
    spawn.unk14 = 100.0f;
    spawn.unk18 = 100.0f;
    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;
    spawn.unk34 = 1.0f;
    spawn.unk38 = 1.0f;
    spawn.unk3C = 1.0f;
    spawn.unk40 = (flag ? 1 : 0) | 0x40000 | 0x80000 | 0x200000 | 0x400000 | 0x80000000;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    ret = func_1513D4B8(&spawn, &D_800A4AA0, 0x15, 0x1F, 0, 0x12, 0, 0x94, argD, argE);
    if (ret != NULL) {
        p = (Field124ChildBlock *)(ret + 0x110);
        memcpy(p, &data, 0x94);
        {
            Spawn15100600 spawn2;
            Data2_15100600 data2;

        data2.unk14 = ret;
        {
            Hdr15100600 hdr;
            Hdr2_15100600 hdr2;
            void *data3;
            Effect15100600 fx;
            Data3_15100600 data4;

        data2.unk0 = 0;
        spawn2.unk0 = 3;
        spawn2.unk1 = 2;
        spawn2.unk2 = 0x3201;
        spawn2.unk4 = arg4;
        spawn2.unk8 = 0;
        spawn2.unkC = 0;
        spawn2.unk10 = arg5;
        spawn2.unk11 = arg6;
        spawn2.unk12 = arg7;
        spawn2.unk13 = 0xFF;
        spawn2.unk18 = 0.0f;
        spawn2.unk14 = 0.0f;
        spawn2.unk1C = 0.0f;
        spawn2.unk20 = 0.0f;
        spawn2.unk24 = 0.0f;
        spawn2.unk28 = 0.0f;
        spawn2.unk2C = 0.0f;
        spawn2.unk30 = 0.0f;
        spawn2.unk34 = 1.0f;
        spawn2.unk38 = 1.0f;
        spawn2.unk3C = 1.0f;
        spawn2.unk40 = (flag ? 1 : 0) | 0x40000 | 0x80000 | 0x400000;
        spawn2.unk44 = 0xFF;
        spawn2.unk45 = 0xFF;
        spawn2.unk46 = 0;
        spawn2.unk47 = 6;
        if ((p->field_0x28 = (s32)func_1513D4B8(&spawn2, &D_800A4AA0, 0, 0, 0, 0x13, 0, 0x18, argD, argE)) != 0) {
            memcpy((u8 *)p->field_0x28 + 0x110, &data2, 0x18);
        }
        hdr2.unk0 = 0;
        hdr2.unk4 = 0;
        hdr2.unk8 = 0;
        data3 = ret;
        hdr.unk0 = (flag ? 1 : 0) | 2;
        hdr.unk1 = -1;
        hdr.unk2 = arg4;
        hdr.unk4 = 7;
        if ((p->field_0x2C = (s32)func_151602C0(&hdr, &hdr2, 5, arg5, arg6, arg7, 0xFF, 0, 4, argD, argE)) != 0) {
            memcpy((u8 *)p->field_0x2C + 0x18, &data3, 4);
        }
        data4.unk2C = 0;
        data4.unk0 = 0.0f;
        data4.unk4 = 0.0f;
        data4.unkC = 10.25000095f;
        data4.unk8 = 10.25000095f;
        data4.unk14 = 2.750000238f;
        data4.unk10 = 2.750000238f;
        data4.unk28 = ret;
        data4.unk18 = func_150ADA68() * 6.283185482f;
        data4.unk1C = func_150ADA68() * 6.283185482f;
        data4.unk20 = (func_150ADA68() * 0.5999999642f) + -0.2999999821f;
        data4.unk24 = (func_150ADA68() * 0.5999999642f) + -0.2999999821f;
        fx.unk1D = 0x2B;
        fx.unk08 = 0x3303;
        fx.unk00 = 0x200005;
        fx.unk04 = 0;
        fx.unk0A = arg4;
        fx.unk0C = 0;
        fx.unk10 = 0;
        fx.unk14 = 0xFF;
        fx.unk15 = 0xFF;
        fx.unk16 = 0xFF;
        fx.unk17 = 0xFF;
        fx.unk18 = arg5;
        fx.unk19 = arg6;
        fx.unk1A = arg7;
        fx.unk1B = 0xFF;
        fx.unk1C = 0xFF;
        fx.unk2C = 0.0f;
        fx.unk28 = 0.0f;
        fx.unk30 = 0.0f;
        fx.unk34 = 0.0f;
        fx.unk38 = 0.0f;
        fx.unk3C = 0.0f;
        fx.unk40 = 0.0f;
        fx.unk44 = 0.0f;
        fx.unk48 = 0.0f;
        fx.unk4C = 0.0f;
        fx.unk50 = 0.0f;
        fx.unk54 = 0.0f;
        fx.unk1E = 1;
        fx.unk20 = 0xFF;
        fx.unk22 = 1;
        fx.unk24 = 1.0f;
        fx.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((flag ? 1 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0)) | 0x4000 | 0x8000;
        fx.unk60 = 6;
        fx.unk61 = 6;
        fx.unk62 = 0xB;
        fx.unk63 = -1;
        fx.unk64 = 0;
        fx.unk65 = 1;
        if ((p->field_0x24 = (s32)func_15130374(&fx, 1, 0x30, argD, argE)) != 0) {
            memcpy((u8 *)p->field_0x24 + 0xA8, &data4, 0x30);
        }
        {
            Emit15100600 emit;
            Spawn15100600 spawn3;
            Data4_15100600 data5;

        data5.unk0 = ret;
        data5.unk4 = 0;
        spawn3.unk0 = 3;
        spawn3.unk1 = 3;
        spawn3.unk2 = 0x3403;
        spawn3.unk4 = arg4;
        spawn3.unk8 = 0;
        spawn3.unkC = 0;
        spawn3.unk10 = arg5;
        spawn3.unk11 = arg6;
        spawn3.unk12 = arg7;
        spawn3.unk13 = 0xFF;
        spawn3.unk14 = 100.0f;
        spawn3.unk18 = 100.0f;
        spawn3.unk1C = 0.0f;
        spawn3.unk20 = 0.0f;
        spawn3.unk24 = 0.0f;
        spawn3.unk28 = 0.0f;
        spawn3.unk2C = 0.0f;
        spawn3.unk30 = 500.0f;
        spawn3.unk34 = 1.0f;
        spawn3.unk38 = 1.0f;
        spawn3.unk3C = 1.0f;
        spawn3.unk40 = (flag ? 1 : 0) | 0x2 | 0x10 | 0x2000 | 0x20000 | 0x40000 | 0x80000 | 0x400000;
        spawn3.unk45 = 0xFF;
        spawn3.unk44 = 0xFF;
        spawn3.unk46 = 0;
        spawn3.unk47 = 6;
        emit.unkC = 0.0f;
        emit.unk8 = 0.0f;
        emit.unk1C = 0.0f;
        emit.unk54 = 0;
        emit.unk55 = 0;
        emit.unk56 = 0;
        emit.unk57 = 0;
        emit.unk48 = 1.0f;
        emit.unk58 = 1;
        emit.unk4 = 60.00000381f;
        emit.unk0 = 60.00000381f;
        emit.unk10 = 0.5f;
        emit.unk14 = 0.5f;
        emit.unk18 = 0.3000000119f;
        emit.unk20 = 9.999999975e-07f;
        if ((p->field_0x30 = (s32)func_151407D0(&emit, 0x68, &spawn3, 0, 0, 0, 0, 0, argD, argE)) != 0) {
            memcpy((u8 *)p->field_0x30 + 0x170, &data5, 8);
        }
        {
            Effect15100600 fx2;
            Data4_15100600 data6;

        data6.unk4 = 0;
        data6.unk0 = ret;
        fx2.unk1D = 0x15;
        fx2.unk08 = 0x3203;
        fx2.unk00 = 0x200005;
        fx2.unk04 = 0;
        fx2.unk0A = arg4;
        fx2.unk0C = 0;
        fx2.unk10 = 0;
        fx2.unk14 = 0xFF;
        fx2.unk15 = 0xFF;
        fx2.unk16 = 0xFF;
        fx2.unk17 = 0xFF;
        fx2.unk18 = arg5;
        fx2.unk19 = arg6;
        fx2.unk1A = arg7;
        fx2.unk1B = 0xFF;
        fx2.unk1C = 0xFF;
        fx2.unk2C = 125.0f;
        fx2.unk28 = 125.0f;
        fx2.unk30 = 0.0f;
        fx2.unk34 = 0.0f;
        fx2.unk38 = 0.0f;
        fx2.unk3C = 0.0f;
        fx2.unk40 = 0.0f;
        fx2.unk44 = 0.0f;
        fx2.unk48 = 0.0f;
        fx2.unk4C = 0.0f;
        fx2.unk50 = 0.0f;
        fx2.unk54 = 0.0f;
        fx2.unk1E = 1;
        fx2.unk20 = 0xFF;
        fx2.unk22 = 1;
        fx2.unk24 = 1.0f;
        fx2.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((flag ? 1 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0)) | 0x4000 | 0x8000;
        fx2.unk60 = 6;
        fx2.unk61 = 6;
        fx2.unk62 = -1;
        fx2.unk63 = -1;
        fx2.unk64 = 1;
        fx2.unk65 = 5;
        if ((p->field_0x34 = (s32)func_15130374(&fx2, 1, 8, argD, argE)) != 0) {
            memcpy((u8 *)p->field_0x34 + 0xA8, &data6, 8);
        }
        }
        }
        }
        }
    }
    return (s32)ret;
}

typedef struct {
    u8 pad0[0x24];
    struct102 *unk24;
    struct102 *unk28;
    struct102 *unk2C;
    struct102 *unk30;
    struct102 *unk34;
} Block15101148;
void func_15101090(u8 *arg0);
void func_15101148(u8 *arg0);
void func_151011E8(Field124Owner *arg0);
void func_15101210(Field124Owner *arg0);

void func_151617C4(struct210 *arg0);
void func_15101238(Field124Owner *arg0);

void func_151617E4(struct210 *arg0);
void func_15101260(Field124Owner *arg0);

void func_1513173C(struct210 *);
void func_15101288(Field124Owner *arg0);

void func_1513175C(struct210 *);
void func_151012B0(Field124Owner *arg0);
void func_151012D8(Field124Owner *arg0);
void func_15101300(Field124Owner *arg0);
void func_15101328(Field124Owner *arg0);
void func_15101350(Field124Owner *arg0);
s16 *func_15101F50(Struct15101F50 *arg0, s16 arg1);

extern f32 D_800BE9A4;

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

typedef struct {
    u8 pad0[0x24];
    u8 *unk24;
    Sub151022AC_28 *unk28;
    Sub151022AC_2C *unk2C;
    u8 pad30[4];
    Sub151022AC_34 *unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0xC];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
} Block151022AC;
s32 func_151022AC(u8 *arg0);
s32 func_151025F4(u8 *arg0, s32 arg1);

extern f32 D_800A21A0[][3];
extern f32 D_800A21F4[][3];
extern u8 D_800A2248[];
s32 func_15100600(u8 *, f32 *, f32 *, u8, s16, u8, u8, u8, f32, f32, f32, f32, u8, u8, s32);
s32 func_151026BC(u8 *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);
void func_151027E8(struct210 *arg0);
s32 func_1510281C(Field124Owner *arg0, s16 arg1);
s32 func_15102884(struct210 *arg0, s32 arg1);
s32 func_151028AC(Field124Owner *arg0, s16 arg1);
