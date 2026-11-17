#include <ultra64.h>
#define func_1513D2F0 func_1513D2F0_hdr
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"
#undef func_1513D2F0
void *func_1513D2F0(void *arg0, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, u8 argA, s32 argB);


extern s32 D_800BE628;
extern s32 D_80082FA4;
void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4);
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
void func_150FB4C0(Gfx *gfx, f32 mtx[4][4]);

typedef struct {
    u8  pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct150F9AB0;
void func_150F9AB0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

typedef struct {
    /* 0x000 */ u8 pad0[0x3B];
    /* 0x03B */ u8 unk3B;
    /* 0x03C */ u8 pad3C[0x110];
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ f32 unk150;
} Actor150F9BB0;

typedef struct {
    /* 0x00 */ Actor150F9BB0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} Data150F9BB0; /* 0x18 */

typedef struct {
    /* 0x00 */ Actor150F9BB0 *unk0;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
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
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ struct260 *unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s8 unk58;
} Ext150F9BB0; /* 0x5C */

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Spawn150F9BB0; /* 0x58 */

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ s8 unk59;
} Emit150F9BB0; /* 0x5C */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ struct260 *unk8;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ Actor150F9BB0 *unk10;
    /* 0x14 */ u8 unk14;
} Sub150F9BB0; /* 0x18 */

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 duration;
    /* 0x04 */ u8 field4;
    /* 0x05 */ u8 field5;
    /* 0x06 */ s8 field6;
} Event150F9BB0;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void *unk8[2];
    /* 0x10 */ void *unk10[2];
} Slots150F9BB0;

extern s32 D_800A1CC0;
void *func_151407D0(void *, s32, void *, u8, u8, u8, u8, s8, u8, s32);
void func_151D8868(void *, s32, u8, s32);
s32 func_15160A58(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150F9BB0(Actor150F9BB0 *arg0, u8 arg1, s32 arg2);

extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
extern struct17 D_800A1CCC;
extern struct17 D_800A1CD8;
extern struct17 D_800A1CE4;
extern struct17 D_800A1CF0;

typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[0x8];
    u8 unk58;
} SubA_150FA1B8;

typedef struct {
    u8 pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    u8 pad4C[0x110 - 0x4C];
    SubA_150FA1B8 unk110;
} Struct150FA1B8_A;

typedef struct {
    u8 pad0[0x4];
    f32 unk4;
} SubB_150FA1B8;

typedef struct {
    u8 pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x170 - 0x5C];
    SubB_150FA1B8 unk170;
} Struct150FA1B8_B;

typedef struct {
    s32 unk0;
    u8 pad4[0x3B - 4];
    u8 unk3B;
    u8 pad3C[0x1D4 - 0x3C];
    s32 unk1D4;
} Struct150FA1B8_C;

typedef struct {
    Struct150FA1B8_C *unk0;
    u8 unk4;
    u8 pad5[3];
    Struct150FA1B8_A *unk8[2];
    Struct150FA1B8_B *unk10[2];
} Struct150FA1B8_D;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x18];
    Struct150FA1B8_D unk28;
} Struct150FA1B8;
void func_150FA1B8(Struct150FA1B8 *arg0);
void func_150FA468(struct102 *arg0, s32 arg1, u8 arg2);
void func_150FAA40(u8 arg0, s32 arg1);

typedef struct {
    u8 count;
    s16 *ids;
    f32 *scales;
} Rec150FA;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
} Emit150FA;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Emit150FA unk28;
} Emitter150FA;

extern Rec150FA D_80088B20[];
void func_1514470C(s32, struct17 *);
void func_150F4570(struct17 *arg0, struct17 *arg1, u8 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
void func_150FAAEC(Emitter150FA *arg0);
void func_150FACE4(struct102 *arg0, s32 arg1, u8 arg2);

extern void func_1515F170(s32 arg0, u8 arg1);
void func_150FAD28(void);
void func_150FAD78(void);
void func_150FADC8(s32 *arg0, s32 arg1, u8 arg2);

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
extern void *func_1515548C(void *, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S150FAE18Main;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} S150FAE18Part;
void func_150FAE18(s16 arg0, u8 arg1, s32 arg2);

typedef struct Func150FB188State {
    u8 pad0[0x54];
    f32 field54;
    f32 field58;
    f32 field5C;
} Func150FB188State;

typedef struct Func150FB188Transform {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    f32 field10;
} Func150FB188Transform;

void func_15157DEC(u8 *, u8 *);
s32 func_150FB188(Func150FB188State *arg0);

extern s32 func_151D710C(s32, s32, s32, s32, s32);
extern void func_15157F80(s32, s32, s32, s32, s32);
void func_150FB1E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_150FB240(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    if ((arg2 - arg3) < arg1) {
        *arg0 = (arg2 - arg1) * arg4;
    } else {
        *arg0 = 0xFF;
    }
}

s32 func_150FB29C(void *arg0) {
    func_150FB240((u8 *)arg0 + 0x43, *(s16 *)((u8 *)arg0 + 0x16), *(s16 *)((u8 *)arg0 + 0x120), *(s16 *)((u8 *)arg0 + 0x64), *(s16 *)((u8 *)arg0 + 0x66));
    return 1;
}

extern void func_150FB240(u8 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
s32 func_150FB2E0(void *arg0);

extern s32 func_15145EA4(void *arg0, void *arg1, s32 arg2, s32 arg3);
void func_150FEC28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
void func_151D4408(struct17 *, struct17 *, s32, struct127 *, f32, s32, s32);
void func_150FB324(struct127 *arg0, s32 arg1, s32 arg2);
