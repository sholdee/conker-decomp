#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A05AC;
extern f32 D_800A05B0;
extern f32 D_800A05B4;
extern f32 D_800A05B8;
extern f32 D_800A05BC;
extern f32 D_800A05C0;
extern f32 D_800A05C4;
extern f32 D_800A05C8;
extern f32 D_800A05CC;
extern f32 D_800A05D0;
extern f32 D_800A05D4;
extern f32 D_800A05D8;
extern f32 D_800A0570;
extern f32 D_800A0574;
extern f32 D_800A0578;
extern f32 D_800A057C;
extern f32 D_800A04F0;
extern f32 D_800A04F4;
extern f32 D_800A04F8;
extern f32 D_800A04FC;

void func_15143794(s32, s32, f32, f32 *);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_1503E5F8(f32 (*)[4], f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern struct260 *func_15073118(struct108 *, s32, s32, f32, f32, f32, f32, f32, f32, f32, s32);

typedef struct {
    u8 pad0[2];
    s8 unk2;
    u8 pad3;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Struct150C8350;
void func_150C8350(void);

extern void func_150C8350(void);
void func_150C84F4(s32 arg0);

void func_150CDB6C(s32);
void func_150C851C(s32 arg0);


typedef struct {
    u8 unk0;
    u8 pad1[3];
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Struct150C8600;

extern s32 D_80090230[5];
extern s32 D_80090244[5];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

s32 func_1510D0EC(s32, s32 *, s32, s32);
extern s32 *D_80088810[2];

typedef struct GameF5800Command {
    u32 w0;
    u32 w1;
} GameF5800Command;
GameF5800Command *func_150C8600(GameF5800Command *arg0);

typedef struct {
    u8 pad0[0x34];
    Mtx *unk34;
} Struct150C8730Arg0;

typedef struct {
    f32 sp48;
    f32 sp4C;
    f32 sp50;
    f32 sp54;
    f32 sp58;
    f32 sp5C;
    f32 sp60;
    f32 sp64[4][4];
    u8 padA4[0x14];
} Struct150C8730Stack;

s32 func_150C8730(Struct150C8730Arg0 *arg0, struct108 *arg1) {
    Struct150C8730Stack sp48;
    f32 sp44;
    f32 sp40;
    s32 temp_v0_2;
    s32 temp_v1;
    Mtx *temp_a1;
    register s16 temp_t7;

    temp_v0_2 = arg1->unk2D0;
    if (temp_v0_2 == 0) {
        return 0;
    }

    temp_v1 = *(u16 *)((u8 *)arg1 + 0x84);
    if (((temp_v1 == 0x14) && (10.0f < *(f32 *)((u8 *)temp_v0_2 + 8))) || (temp_v1 == 0x23)) {
        temp_v0_2 = D_800BE9C0;
        temp_a1 = arg0->unk34;
        temp_a1 = (Mtx *)((u8 *)temp_a1 + ((temp_v0_2 == 0) << 6));
        guMtxL2F((f32 (*)[4])((u8 *)sp48.sp64 - 8), temp_a1);
        ((f32 (*)[4])((u8 *)sp48.sp64 - 8))[0][3] = 0.0f;
        ((f32 (*)[4])((u8 *)sp48.sp64 - 8))[1][3] = 0.0f;
        ((f32 (*)[4])((u8 *)sp48.sp64 - 8))[2][3] = 0.0f;
        ((f32 (*)[4])((u8 *)sp48.sp64 - 8))[3][3] = 1.0f;
        func_1503E5F8((f32 (*)[4])((u8 *)sp48.sp64 - 8), (f32 *)((u8 *)&sp48.sp60 - 8), (f32 *)((u8 *)&sp48.sp5C - 8), (f32 *)((u8 *)&sp48.sp58 - 8), (f32 *)((u8 *)&sp48.sp48 - 8), (f32 *)((u8 *)&sp44 - 8), (f32 *)((u8 *)&sp40 - 8), (f32 *)((u8 *)&sp48.sp54 - 8), (f32 *)((u8 *)&sp48.sp50 - 8), (f32 *)((u8 *)&sp48.sp4C - 8));

        arg0 = (Struct150C8730Arg0 *) func_15073118(arg1, -1, 0x4E, 0.0f, 0.0f, 0.0f, 0.0f, D_800A04F0, D_800A04F4, D_800A04F8, 0xC8);
        *(f32 *)((u8 *)arg0 + 0x14) = *(f32 *)((u8 *)&sp48.sp60 - 8);
        *(f32 *)((u8 *)arg0 + 0x18) = *(f32 *)((u8 *)&sp48.sp5C - 8);
        *(f32 *)((u8 *)arg0 + 0x1C) = *(f32 *)((u8 *)&sp48.sp58 - 8);
        *(f32 *)((u8 *)arg0 + 0xB8) = *(f32 *)((u8 *)&sp48.sp48 - 8);
        *(f32 *)((u8 *)arg0 + 0x40) = *(f32 *)((u8 *)&sp44 - 8);
        *(f32 *)((u8 *)arg0 + 0xC4) = *(f32 *)((u8 *)&sp40 - 8);
        temp_t7 = (s32)((*(f32 *)((u8 *)arg0 + 0x40) - 90.0f) * D_800A04FC);
        *(s16 *)((u8 *)arg0 + 0x76) = temp_t7;
        *(s16 *)((u8 *)arg0 + 0x7A) = temp_t7;
        return 1;
    }

    return 0;
}

extern void *func_15167A68(s32, s32, s32, s32, u8, s32);

typedef struct {
    u8 pad0[0x14];
    s32 count;
    s16 unk18;
    u8 pad1A[0x16];
} Struct150C88D0Def; /* size 0x30 */

typedef struct {
    u8 pad0[0x10];
    Struct150C88D0Def def;
    f32 unk40;
    f32 unk44;
    f32 step;
    s16 unk4C;
    f32 unk50;
    u8 *unk54;
    u8 *unk58;
    u8 pad5C[0x360 - 0x5C];
    f32 (*sincos)[2];
    u8 *unk364;
    u8 data[1];
} Struct150C88D0;
Struct150C88D0 *func_150C88D0(Struct150C88D0Def *arg0, s32 arg1, u8 arg2, s32 arg3);

typedef struct {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 unk1C;
    s8 unk1D;
    u8 pad1E[2];
    f32 unk20;
    u8 pad24[4];
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    s16 unk30;
    u8 pad32[2];
    f32 unk34;
    f32 unk38;
    s8 unk3C;
    s8 unk3D;
    u8 pad3E[2];
    f32 unk40;
    f32 unk44;
    u8 pad48[4];
    s16 unk4C;
    u8 pad4E[2];
    f32 unk50;
} Struct150C8A68;

extern void (*D_80088858[])(Struct150C8A68 *, f32 *, u8, f32, f32);
extern s32 (*D_80088860[])(Struct150C8A68 *);
extern void (*D_80088864[])(Struct150C8A68 *, f32);
void func_150C8A68(Struct150C8A68 *arg0);

typedef struct {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x13];
    f32 unk20;
} Struct150C99B4;
void func_150C99B4(Struct150C99B4 *arg0, struct17 *arg1, s32 arg2, s32 arg3);

typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x28];
    f32 unk44;
} Struct150C9BDC;

extern s32 func_1505D024(struct127 *, s32, u16, s32);
void func_150C9BDC(Struct150C9BDC *arg0, s32 arg1);

void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_150C9DC4(Struct150C99B4 *arg0, struct17 *arg1, s32 arg2, s32 arg3);

extern f32 D_800BE9A4;
s32 func_150CA07C(f32 *arg0);

typedef struct {
    f32 unk0;
    f32 unk4;
    s32 unk8;
} Struct150CA930;

extern void func_1514C678(f32, f32, s32, f32, s32, s32, s32, s32, s32, f32, s32, s32);
void func_150CA930(Struct150CA930 *arg0);

extern f32 D_800A05A0;
extern f32 D_800A05A4;
extern f32 D_800A05A8;
extern s32 func_15193CA0[];
extern s32 func_151942B0[];
void func_1504715C(void *, void *);
s32 func_15046C80(void *, s32, f32, void *);
void func_150CCD90(f32, f32 *, f32 *, f32 *);

typedef struct {
    f32 x, y, z;
} V3_150CA150;

typedef struct {
    V3_150CA150 pos;
    u8 unkF0;
    s8 unkF1;
    f32 unkF4;
    s32 unkF8;
    s16 unkFC;
    s16 unkFE;
    f32 unk100;
    s16 unk104;
    f32 unk108;
    f32 unk10C;
    u8 unk110;
    u8 unk111;
} Def_150CA150;

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 pad7;
    s32 unk8;
    s32 unkC;
    u8 unk10, unk11, unk12, unk13;
    u8 unk14, unk15, unk16, unk17;
    s32 unk18;
} Fx_150CA150;
void func_150CA150(u8 *arg0);
void func_150CA930(Struct150CA930 *arg0);

typedef struct {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x28 - 0x1E];
    u8 unk28;
} Struct150CA9D0;
s32 func_150CA9D0(Struct150CA9D0 *arg0);
s32 func_150CAA04(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CAC28(f32 *arg0, s32 arg1);
s32 func_150CADD0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11, s32 arg12, f32 *arg13, u8 arg14);
s32 func_150CB008(f32 *arg0);
