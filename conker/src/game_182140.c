#include <ultra64.h>
#define func_15169260 func_15169260_s32_decl
#define func_150ADA20 func_150ADA20_hdr_decl
#include "functions.h"
#undef func_15169260
#undef func_150ADA20
#include "variables.h"

extern s32 func_150ADA20(void);

struct Vec3F { f32 x; f32 y; f32 z; };
struct Local1514F6E8 { u8 unk0; u8 pad1[3]; struct Vec3F unk4; struct Vec3F unk10; struct Vec3F unk1C; f32 unk28; };
struct Arg1514FB98 { u8 pad[0xC]; f32 unkC; u8 unk10[0x4]; };
struct Arg1514FBFC { struct Vec3F unk0; struct Vec3F unkC; f32 unk18; f32 unk1C; f32 unk20; u8 unk24[0x4]; };
extern s32 func_15146078(struct Arg1514FB98 *, struct Vec3F *, struct Vec3F *);
extern s32 func_15144E80(struct Vec3F *, struct Vec3F *, struct Vec3F *, struct Vec3F *);
extern f32 func_15144A74(struct Vec3F *, struct Vec3F *);
extern s32 func_15145128(struct Vec3F *, struct Vec3F *, f32 *, f32 *);
struct Local1514F8F8;
extern void func_1514F8F8(struct Local1514F8F8 *, struct Vec3F *, struct Vec3F *, struct Vec3F *, f32, u8, s32);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern f32 D_800A601C;
extern f32 D_800A6020;
extern f32 D_800A6024;
extern f32 D_800A6028;
extern f32 D_800A602C;
extern f32 D_800A6004;
extern f32 D_800A6008;
extern f32 D_800A600C;
extern f32 D_800A6010;
extern f32 D_800A6014;
extern f32 D_800A6018;
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s32, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);

struct Local1515572CData {
    s32 unk0;
    s32 unk4;
};

extern struct Local1515572CData D_800A6038;
extern void func_15169260(struct Local1515572CData *, s32, s32, u8);

struct Local15152874Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15152874Arg {
    s32 unk0;
    s32 unk4;
    struct Local15152874Header unk8;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u8 pad36[2];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
};

struct Local15152874Spawn {
    struct Local15152874Header unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    s32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 pad34;
};

struct Local15150D1CArg {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    s16 unk2A;
    u8 unk2C;
};

struct Local151539B4Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local151539B4Arg {
    s16 unk0;
    s16 unk2;
    struct Local151539B4Header unk4;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
};

struct Local151539B4Spawn {
    struct Local151539B4Header unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 pad29;
    s16 unk2A;
    u16 unk2C;
    u8 pad2E[2];
    f32 unk30;
    u8 unk34;
    u8 pad35;
    s16 unk36;
    s16 unk38;
    u8 pad3A[2];
};

struct Local15153CCCArg {
    struct Local151539B4Header unk0;
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
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
};

extern void func_15143794(s16, s16, f32, f32 *);
extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);
extern void func_150CCEB0(struct Local15152874Spawn *, u8, u8);
extern void (*D_8008AC60[])(u8 *);
extern s32 (*D_8008ACC8[])(void *);
extern void func_15156190(void *, u8, s32, u8, s32);
extern f32 D_800A5FF0;
extern f32 D_800A5FFC;
extern f32 D_800A6000;
extern void func_1514F808(struct Local1514F6E8 *, f32, struct Vec3F *);
extern void func_151DA6F8(struct Local15153CCCArg *, struct Vec3F *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);

struct Local15152520Bytes {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
};

struct Local15152520Arg {
    s32 unk0;
    s32 unk4;
    struct Local15152874Header unk8;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
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
    struct Local15152520Bytes unk68;
    struct Local15152520Bytes unk6C;
    u8 unk70;
    u8 unk71;
    s16 unk72;
    s16 unk74;
};

struct Local15152520Motion {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 pad31[3];
};

struct Local15152520Params {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    s8 unk03;
    s8 unk04;
    u8 pad05;
    s16 unk06;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    struct Local15152520Bytes unk30;
    struct Local15152520Bytes unk34;
    u8 unk38;
    u8 pad39[3];
    s32 unk3C;
    u8 unk40;
    u8 unk41;
    u8 pad42[2];
    struct Local15152874Header unk44;
    u8 pad50[4];
    s16 unk54;
    s16 unk56;
};

struct Local15152520ParamBlock {
    struct Local15152520Params params;
    s32 pad58;
};

extern void func_15157898(struct Local15152520Params *, struct Local15152520Motion *, s32, f32, s32, s32, s32, u8, s32);
extern void func_1515C2F0(void *, s32, void *, s32, u8, s32);

struct Local15152ABCOut {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
};

struct Local15152ABCColor {
    u8 unk0;
    u8 unk1;
    u8 unk2;
};

struct Local15152ABCStack {
    struct Local15152ABCColor *color;
    s32 pad;
};

extern struct Local15152ABCColor D_800A5FE0[];
extern s32 func_15147DA0(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32, u8, s32);

struct Local15152B38Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15152B38Arg {
    s32 unk0;
    s32 unk4;
    struct Local15152B38Header unk8;
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
    u8 pad6B;
    f32 unk6C;
    s8 unk70;
    s8 unk71;
};

struct Local15152B38Param {
    struct Local15152B38Header unk0;
    s16 unkC;
    u16 unkE;
    s32 unk10;
    u8 pad14;
    s8 unk15;
    u8 pad16[2];
    u8 pad18[4];
};

struct Local15152B38Spawn {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    s8 unk17;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    s8 unk1B;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    s8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    u8 pad33;
    f32 unk34;
    s8 unk38;
    s8 unk39;
    u8 pad3A[2];
};

struct Local15153634Block {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15153634Arg {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct Local15153634Block unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
};

struct Local15153634Spawn {
    s32 unk00;
    s32 unk04;
    u16 unk08;
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
    struct Local15153634Block unk30;
    u8 pad3C[0xC];
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
    u8 pad64[2];
    u8 unk66;
    u8 pad67[9];
};

struct Local15151A38Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15151A38Arg {
    struct Local15151A38Header unk0;
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
    u8 pad3B;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 pad57;
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
};

struct Local15151A38Pos {
    struct Local15151A38Header unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
};

struct Local15151A38Spawn {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[4];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    s16 unk1E;
};


struct Local1514FEFC;
struct Local1514FF44Arg;
void func_1514F640(void *arg0, struct Local1514FEFC *arg1);
s32 func_1514F6E8(struct Local1514F6E8 *arg0);

extern void func_15143874(s32, f32, f32 *, f32 *);
void func_1514F808(struct Local1514F6E8 *arg0, f32 arg1, struct Vec3F *arg2);

struct Local1514F8F8 {
    s32 unk0;
    s32 unk4;
    u8 unk8[0xC];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s32 unk2C;
    s32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
};

extern void func_15143874(s32, f32, f32 *, f32 *);
extern void func_151A2AD4(void *, f32 *, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);
void func_1514F8F8(struct Local1514F8F8 *arg0, struct Vec3F *arg1, struct Vec3F *arg2, struct Vec3F *arg3, f32 arg4, u8 arg5, s32 arg6);
void func_1514FB98(struct Arg1514FB98 *arg0, u8 arg1, s32 arg2);
void func_1514FBFC(struct Arg1514FBFC *arg0, u8 arg1, s32 arg2);

struct Local1514FCE8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
    struct Vec3F unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s8 unk58;
};

extern void func_151A2A14(struct Vec3F *, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32);
void func_1514FCE8(struct Local1514FCE8 *arg0, u8 arg1, s32 arg2);

struct Local1514FF44Arg {
    u8 pad0[0xC];
    s16 unkC;
    s16 unkE;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 pad30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34;
    u8 unk38;
    u8 pad39[3];
    f32 unk3C;
};

struct Local1514FEFC { u8 unk0[0x2C]; };
extern void func_1514F640(void *, struct Local1514FEFC *);
extern void func_1514FF44(struct Local1514FEFC *, struct Local1514FF44Arg *, s32, u8, s32);
extern void func_151D9014(struct Local1514FF44Arg *, struct Vec3F *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, u8, s32);
extern f32 D_800A5FF4;
void func_1514FEFC(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
void func_1514FF44(struct Local1514FEFC *arg0, struct Local1514FF44Arg *arg1, s32 arg2, u8 arg3, s32 arg4);

struct Local15150178Angles {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
};
void func_15150178(struct Local15150178Angles *arg0, struct Local1514FF44Arg *arg1, s32 arg2, u8 arg3, s32 arg4);

struct Arg15150400 {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s32 *unk48;
    f32 *unk4C;
    u32 unk50;
    f32 unk54;
    u8 unk58;
    u8 unk59;
    u8 pad5A[2];
    s32 unk5C;
    void *unk60;
};

struct Angles15150400 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
};

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    volatile f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F unk28;
    struct Vec3F unk34;
    struct Vec3F unk40;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Spawn15150400;

extern s32 func_1513264C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
void func_15150400(struct Arg15150400 *arg0, struct Angles15150400 *arg1, u8 arg2, s32 arg3);

struct Arg1515080C {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
};

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    volatile f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F unk28;
    struct Vec3F unk34;
    struct Vec3F unk40;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Spawn1515080C;

extern void func_15143794(s16, s16, f32, f32 *);
extern s32 func_1513264C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
void func_1515080C(struct Arg1515080C *arg0, s32 *arg1, f32 *arg2, s32 arg3, void *arg4, s32 arg5, u8 arg6, u8 arg7, s8 arg8, s32 arg9, f32 argA, u8 argB, u8 argC, u8 argD, s32 argE);
void func_15150D1C(struct Local15150D1CArg *arg0, u8 arg1, s32 arg2);

typedef struct {
    s32 countBase, countRange;
    f32 position[3];
    f32 speedBase, speedRange;
    s16 angleBase, angleRange, pitchBase, pitchRange;
    f32 directionBase, directionRange, jitter;
    f32 sizeBase, sizeRange, scale;
    u8 kind, pad3D;
    s16 lifeBase, lifeRange;
} Game150F90Config;

void *func_1518A3C0(f32 *, f32 *, f32, f32 *, f32 *, f32, f32, u8, s16, u8, s32, u8, s32);
void func_15150F90(Game150F90Config *arg0, u8 arg1, u8 arg2, s32 arg3);

typedef struct {
    s16 countBase;
    s16 countRange;
    struct Vec3F position;
    struct Vec3F normal;
    s16 triangle[9];
    u8 pad2E[2];
    f32 spread;
    u8 unk34;
    u8 unk35;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
    f32 unk3C;
    f32 unk40;
    s32 unk44;
    u8 unk48;
    u8 unk49;
    u8 pad4A[2];
    f32 unk4C;
    f32 unk50;
    f32 speedBase;
    f32 speedRange;
    u8 unk5C;
    u8 unk5D;
    u8 pad5E[2];
    s32 unk60;
    s32 unk64;
    s16 unk68;
    s16 unk6A;
    f32 unk6C;
    f32 unk70;
} Local151511FCArg;

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
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
    struct Vec3F unk1C;
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
} Local151511FCDesc;

typedef struct {
    struct Vec3F velocity;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Local151511FCExtra;
void func_151511FC(Local151511FCArg *arg0, u8 arg1, s32 arg2);

typedef struct {
    struct Vec3F position;
    s16 life, enabled;
    s32 active;
    u8 field14, fade, pad16[2];
    s32 field18;
} Game151670Spawn;

typedef struct {
    f32 size;
    struct Vec3F velocity;
    f32 growth;
    u8 pad14[4];
    u8 red, green, blue, alpha;
    s16 field1C, field1E;
} Game151670Motion;

typedef struct {
    struct Vec3F position;
    struct Vec3F normal;
    s16 triangle[9];
    u8 pad2A[2];
    f32 spread;
    f32 sizeBase, sizeRange, growthBase, growthRange, speedBase, speedRange;
    s16 countBase, countRange, fadeBase, fadeRange, lifeBase, lifeRange;
    u8 green, enabled, red, pad57;
    s32 callbacks[6];
    u8 blue, alphaBase, alphaRange, pad73;
    s32 field74, field78;
    s32 flags[8];
    s16 field9C, field9E;
    s32 fieldA0;
} Game151670Config;
void func_15151670(Game151670Config *arg0, u8 arg1, s32 arg2);
void func_15151A38(struct Local15151A38Arg *arg0, u8 arg1, s32 arg2);

struct Arg15151D6C {
    s32 unk0;
    s32 unk4;
    struct Vec3F unk8;
    struct Vec3F unk14;
    struct Vec3F unk20;
    u8 pad2C[0x8];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
};

struct Spawn15151D6C {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    volatile f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F unk28;
    struct Vec3F unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    s16 pad76;
};
void func_15151D6C(struct Arg15151D6C *arg0, s32 *arg1, f32 *arg2, u32 arg3, u8 arg4, s32 arg5);

struct Arg15152190 {
    s32 unk0;
    s32 unk4;
    struct Vec3F unk8;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
};

struct Spawn15152190 {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    volatile f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Vec3F unk28;
    struct Vec3F unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    s16 pad76;
};
void func_15152190(struct Arg15152190 *arg0, s32 *arg1, f32 *arg2, u32 arg3, f32 arg4, u8 arg5, u8 arg6, s32 arg7);
void func_15152520(struct Local15152520Arg *arg0, u8 arg1, s32 arg2);
void func_15152874(struct Local15152874Arg *arg0, u8 arg1, s32 arg2);
void func_15152ABC(struct Local15152ABCOut *arg0);
void func_15152B38(struct Local15152B38Arg *arg0, u8 arg1, s32 arg2);

struct Local15152F70Header {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Local15152F70Arg {
    struct Local15152F70Header unk0;
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
};

struct Local15152F70Pos {
    struct Local15152F70Header unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
};

struct Local15152F70Spawn {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8 pad14[4];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
};

struct Local15152F70Style {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
};
void func_15152F70(struct Local15152F70Arg *arg0, u8 arg1);


typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color153298;

typedef struct {
    struct Vec3F position;
    s16 countBase, countRange, yawBase, yawRange, pitchBase, pitchRange;
    f32 speedBase, speedRange;
    s16 lifeBase, lifeRange, sizeBase, sizeRange;
    s8 mode;
    u8 kind;
    s16 fadeBase, fadeRange, field2E;
    s16 field30, field32, field34, field36;
    s32 flags;
    u8 color, alpha;
} Game153298Config;

typedef struct {
    s32 flags;
    u8 pad04[8];
    s16 field0C, field0E;
    s16 x, y, z, velocityX, velocityZ;
    u8 pad1A[2];
    s8 fractionX, fractionY, fractionZ, mode;
    s16 velocityY, size, field24, field26, life;
    s8 fade;
    u8 pad2B;
    Color153298 color;
    s8 field30, field31, field32;
    u8 pad33;
    s16 kind;
} Game153298Packet;

extern void *func_15167D84(void *, s32, s32, s8, u8, s32);
void func_15153298(Game153298Config *arg0, u8 arg1, s32 arg2);
void func_15153634(struct Local15153634Arg *arg0, u8 arg1, u8 arg2, s32 arg3);
void func_151539B4(struct Local151539B4Arg *arg0, u8 arg1);

extern void func_15153CCC(struct Local1514FEFC *, struct Local15153CCCArg *, s32, u8, s32);
void func_15153C84(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
void func_15153CCC(struct Local1514FEFC *arg0, struct Local15153CCCArg *arg1Struct, s32 arg2, u8 arg3, s32 arg4);
void func_15153F18(struct Local15150178Angles *arg0, struct Local15153CCCArg *arg1, register s32 arg2, u8 arg3, register s32 arg4);
void *func_151541B8(struct Vec3F *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6);

f32 sqrtf(f32);
s32 func_1515452C(u8 *arg0);

struct Local15154684Arg {
    s16 unk00;
    s16 unk02;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
};
void func_15154684(struct Local15154684Arg *arg0, u8 arg1, s32 arg2);
void *func_15154884(struct Vec3F *arg0, f32 arg1, f32 arg2, f32 arg3, u8 arg4, s32 arg5);
s32 func_15154A88(void *arg0);

typedef struct {
    u8 pad0[0x22];
    s16 unk22;
    u16 unk24;
    s16 unk26;
    s16 unk28;
    u8 pad2A[0x4];
    u8 unk2E;
    u8 pad2F[0x39];
    u8 unk68;
} Struct15154C90;

void func_15154C90(Struct15154C90 *arg0) {
    s32 temp;
    s8 failed;

    failed = 0;
    if (arg0->unk24 & 1) {
        arg0->unk22 -= D_800BE9E4;
        if (arg0->unk22 < 0) {
            failed = 1;
        }
    }
    temp = arg0->unk68 & 0xF;
    if (temp != 0) {
        if (failed == 0) {
            s32 ret;

            ret = D_8008ACC8[temp](arg0);
            if (ret == 0) {
                failed = 1;
            }
        }
    }
    if (arg0->unk24 & 8) {
        s16 temp_v1;

        temp_v1 = arg0->unk22;
        if (temp_v1 < arg0->unk26) {
            temp = (s16)(temp_v1 * (u32)arg0->unk28);
            if (temp < arg0->unk2E) {
                arg0->unk2E = temp;
            }
        }
    }
    if (failed) {
        func_1516972C((struct102 *) arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_182140/func_15154D80.s")

struct Arg1515548C {
    u8 pad0[8];
    f32 unk8;
    f32 unkC;
    u8 pad10[4];
    u16 unk14;
    u8 pad16[0x42];
};

extern s32 func_151555AC();
extern u8 *func_15167A68(s32, s32, s32, s32, u8, s32);

u8 *func_1515548C(struct Arg1515548C *arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    u8 *ret;
    f32 pos[2];

    if ((arg2 != 0) && (arg3 > 0)) {
        pos[0] = arg0->unk8;
        pos[1] = arg0->unkC;
        if (func_151555AC(arg0, pos) != 0) {
            return 0;
        }
    }
    ret = func_15167A68((arg0->unk14 & 0x20) ? 0x57 : 0x5D, arg6, arg4 + 0x70, 1, arg5, 1);
    if (ret == 0) {
        return 0;
    }
    memcpy(ret + 0x10, arg0, 0x58);
    ret[0x68] = 0;
    ret[0x68] |= arg1;
    return ret;
}

struct Local15155564 { u8 pad[0x2A]; u8 unk2A; };
extern void (*D_8008AD04[])(struct Local15155564 *, s32, u8);

void func_15155564(struct Local15155564 *arg0, s32 arg1, u8 arg2) {
    if (D_8008AD04[arg0->unk2A] != NULL) {
        D_8008AD04[arg0->unk2A](arg0, arg1, arg2);
    }
}

struct Node151555AC {
    u8 pad0[8];
    struct Node151555AC *next;
    u8 padC[4];
    f32 pos[2];
    f32 size[2];
    u8 pad20[0xA];
    u8 unk2A;
};
struct Ids151555AC { s32 id[2]; };
extern struct Ids151555AC D_800A6030;
extern u8 D_800DCE50[];

s32 func_151555AC(f32 *arg0, f32 *arg1, s32 *arg2, s32 arg3) {
    s32 j;
    struct Ids151555AC ids;
    u8 i;
    struct Node151555AC *node;
    struct Node151555AC *next;
    s32 found;
    s32 k;
    f32 w;
    f32 h;
    i = 0;
    for (j = 0; j < 2; j++) {
        ids = D_800A6030;
        i &= 0;
        do {
            node = ((struct Node151555AC *(*)[0x68])D_800DCE50)[j][ids.id[i]];
            while (node != NULL) {
                next = node->next;
                k = arg3;
                found = 0;
                while (k > 0 && !found) {
                    if (node->unk2A == arg2[k - 1]) {
                        found = 1;
                    } else {
                        k--;
                    }
                }
                if (found) {
                    w = arg1[0] + node->size[0];
                    h = arg1[1] + node->size[1];
                    if (arg0[0] - w <= node->pos[0] && node->pos[0] <= arg0[0] + w &&
                        arg0[1] - h <= node->pos[1] && node->pos[1] <= arg0[1] + h) {
                        return 1;
                    }
                }
                node = next;
            }
lbl:
            i++;
        } while (i < 2);
        i = 0;
    }
}
void func_1515572C(s32 arg0, u8 arg1) {
    struct Local1515572CData sp18;

    sp18 = D_800A6038;
    func_15169260(&sp18, 2, arg0, arg1);
}
