#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0__proto
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1511FC2C func_1511FC2C__proto
#define func_1511FC44 func_1511FC44__proto
#include "functions.h"
#undef func_1502B7F0
#undef func_150ADA20
#undef func_1511FC2C
#undef func_1511FC44
#include "variables.h"

u32 func_150ADA20(void);

extern u8 D_800DBEA8[];
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);

typedef struct {
    u8  pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct151103C8;

typedef struct Game13D350ViewAngles {
    u8 pad0[0xC];
    f32 pitch;
    f32 yaw;
    f32 roll;
} Game13D350ViewAngles;

typedef Gfx *(*Game13D350ViewDraw)(Gfx *, f32, f32, f32, f32, f32, f32, s32);

Gfx *func_151106A8(Gfx *);
s32 func_151108C4(f32, f32, f32, f32, f32);
extern s16 D_80082FA6;
extern u8 D_800D9B69;
extern u8 D_800D9B6A;
extern u8 D_800D9B6B;
extern u8 D_800D9B6C;
extern u8 D_800D9B6D;
extern u8 D_800D9B6E;
extern u8 D_800D9B6F;
extern u8 D_800D9B70;
extern u8 D_800D9B71;
extern u8 D_800D9B72;
extern u8 D_800D9B73;
extern u8 D_800D9B79;
extern u8 D_800D9B7A;
extern u8 D_800D9B7B;
extern u8 D_800D9B7C;
extern u8 D_800D9B7D;
extern u8 D_800D9B7E;
extern u8 D_800D9B7F;
extern u8 D_800D9B80;
extern u8 D_800D9B81;
extern u8 D_800D9B82;
extern u8 D_800D9B83;
extern Game13D350ViewAngles D_800DCDE0[];
extern Game13D350ViewDraw D_800891D8[];

Gfx *func_1510FEA0(Gfx *arg0, s32 arg1) {
    u8 *record;
    f32 pitch;
    f32 yaw;
    f32 roll;
    s32 visible;
    f32 x;
    f32 y;
    f32 z;

    D_800D9B68[0][0] = 0;
    D_800D9B69 = 0;
    D_800D9B6A = 0;
    D_800D9B78[0][0] = 0;
    D_800D9B79 = 0;
    D_800D9B7A = 0;
    D_800D9B6B = 0;
    D_800D9B6C = 0;
    D_800D9B6D = 0;
    D_800D9B7B = 0;
    D_800D9B7C = 0;
    D_800D9B7D = 0;
    D_800D9B6E = 0;
    D_800D9B6F = 0;
    D_800D9B70 = 0;
    D_800D9B7E = 0;
    D_800D9B7F = 0;
    D_800D9B80 = 0;
    D_800D9B71 = 0;
    D_800D9B72 = 0;
    D_800D9B73 = 0;
    D_800D9B81 = 0;
    D_800D9B82 = 0;
    D_800D9B83 = 0;
    { Gfx *command = arg0++; command->words.w0 = 0xD9FFF9FF; command->words.w1 = 0; }
    arg0 = func_151106A8(arg0);
    { Gfx *command = arg0++; command->words.w0 = 0xE7000000; command->words.w1 = 0; }
    if (D_800BE635 != 0) {
        arg0 = func_15110544(arg0, 0, 0, D_800BE620, D_800BE624, 0, 0, 0);
    }
    if (D_800C35EA == 1 && D_800C3681 != 0) {
        return arg0;
    }
    pitch = 360.0f - (*(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x388) + D_800DCDE0[D_80082FA4].pitch);
    while (pitch < -180.0f) {
        pitch += 360.0f;
    }
    while (pitch >= 180.0f) {
        pitch -= 360.0f;
    }
    yaw = 360.0f - (*(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x380) + D_800DCDE0[D_80082FA4].yaw);
    while (yaw < 0.0f) {
        yaw += 360.0f;
    }
    while (yaw >= 360.0f) {
        yaw -= 360.0f;
    }
    roll = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x5EC) + D_800DCDE0[D_80082FA4].roll;
    while (roll < -180.0f) {
        roll += 360.0f;
    }
    while (roll >= 180.0f) {
        roll -= 360.0f;
    }
    x = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x2F8);
    y = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x2FC);
    z = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x300);
    if (D_800C3682 != 0) {
        record = (u8 *)D_800BE628 + D_80082FA4 * 0x180;
        return func_15110544(arg0,
                             (s32)*(f32 *)(record + 0x2C), (s32)*(f32 *)(record + 0x24),
                             (s32)(*(f32 *)(record + 0x30) - 1.0f),
                             (s32)*(f32 *)(record + 0x28), 0x16, 0x1C, 0x1B);
    }
    visible = func_151108C4(x, y, z, pitch, yaw);
    if (visible != 0) {
        arg0 = D_800891D8[((u8 *)D_800B0DF0)[8]](arg0, x, y, z, pitch, yaw, roll, visible);
    }
    arg0 = (Gfx *)func_1501A490((s32)arg0, D_80082FA6, 0, 0, 0, 0);
    { Gfx *command = arg0++; command->words.w0 = 0xD9FFFFFF; command->words.w1 = 0x400; }
    return arg0;
}

extern s32 D_800BE628;
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
void func_151102CC(f32 a0[4][4], f32 a1, f32 a2, f32 a3);

void func_151102CC(f32 arg0[4][4], f32 arg1, f32 arg2, f32 arg3) {
    f32 sp28[4][4];

    func_150A8050(arg0, 0.0f, arg2, 0.0f);
    func_150A8050(sp28, arg1, 0.0f, 0.0f);
    func_150A7A48(arg0, sp28, arg0);
    func_150A8050(sp28, 0.0f, 0.0f, arg3);
    func_150A7A48(arg0, sp28, arg0);
}

void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4) {
    func_151102CC(arg1, arg2, arg3, arg4);
    func_150A7A48(arg1, (f32 (*)[4])((u8 *)((struct259 *)D_800BE628 + arg0) + 0xBC), arg1);
}

Gfx *func_151103C8(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    Struct151103C8 *temp;

    temp = (Struct151103C8 *)((struct259 *)D_800BE628 + D_80082FA4);
    gfx = func_15110544(gfx, (s32)temp->unk2C, (s32)temp->unk24,
                        (s32)(temp->unk30 - 1.0f), (s32)temp->unk28,
                        D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    if (D_80082FA0 != 0) {
        if (D_80082FA4 == 0) {
            gDPPipeSync(gfx++);
            gfx = (Gfx *)func_1501A490((s32)gfx, 0xFF, 0, 0, 0, 0);
            gDPSetFillColor(gfx++, 0x10001);
            gfx = func_1501A6CC(gfx, 0, (D_800BE624 >> 1) - 6, D_800BE620,
                                (D_800BE624 >> 1) + 6);
            if (D_80082FA0 != 1) {
                gfx = func_1501A6CC(gfx, (D_800BE620 >> 1) - 1, 0,
                                    (D_800BE620 >> 1) + 1, D_800BE624);
            }
        }
    }
    return gfx;
}

Gfx *func_1501A680(Gfx *arg0);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);

Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl) {
    s32 color;

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x302C0F, 4);
    gfx = func_1501A680(gfx);
    color = GPACK_RGBA5551(r, g, bl, 1);
    gDPSetFillColor(gfx++, (color << 16) | color);
    gfx = func_1501A6CC(gfx, a, b, c, d);
}

Gfx *func_15110600(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;

    temp_v0 = D_800BE9F0;
    if ((temp_v0 == 0x1B) || (temp_v0 == 0x1E) ||
        ((temp_v0 == 0x31) && (*(u8 *)&D_800DBFF0->unk3D4->unk78 == 3))) {
        gfx = func_15110544(gfx, 2, 0, D_800BE620 - 2, D_800BE624,
                            D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    }
    return gfx;
}

extern u8 D_800BEAC2;

Gfx *func_151106A8(Gfx *arg0) {
    if (D_80082FA4 == 0) {
        { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xE7, 24, 8); command->words.w1 = 0; }
        { Gfx *command = arg0++; command->words.w0 = 0xEF302C0F; command->words.w1 = 4; }
        { Gfx *command = arg0++; command->words.w0 = 0xFCFFFFFF; command->words.w1 = 0xFFFE793C; }
        { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xFF, 24, 8) | _SHIFTL(0, 21, 3) | _SHIFTL(2, 19, 2) | _SHIFTL(D_800BE620 - 1, 0, 12); command->words.w1 = D_800BE9C4; }
        { Gfx *command = arg0++; command->words.w0 = 0xF7000000; command->words.w1 = 0xFFFCFFFC; }
        arg0 = (Gfx *)func_1501A490((s32)arg0, 0xFF, 0, 0, 0, 0);
        { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xF6, 24, 8) | _SHIFTL(D_800BE620 - 2, 14, 10) | _SHIFTL(D_800BE624, 2, 10); command->words.w1 = _SHIFTL(2, 14, 10) | _SHIFTL(0, 2, 10); }
        { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xE7, 24, 8); command->words.w1 = 0; }
        arg0 = func_1501A680(arg0);
        if (D_80082FA0 != 0) {
            { Gfx *command = arg0++; command->words.w0 = _SHIFTL(0xE7, 24, 8); command->words.w1 = 0; }
            arg0 = (Gfx *)func_1501A490((s32)arg0, 0xFF, 0, 0, 0, 0);
            { Gfx *command = arg0++; command->words.w0 = 0xF7000000; command->words.w1 = 0x10001; }
            arg0 = func_1501A6CC(arg0, 0, (D_800BE624 >> 1) - 6, D_800BE620, (D_800BE624 >> 1) + 6);
            if (D_80082FA0 != 1) {
                arg0 = func_1501A6CC(arg0, (D_800BE620 >> 1) - 1, 0, (D_800BE620 >> 1) + 1, D_800BE624);
            }
            if (D_800BEAC2 != 0) {
                arg0 = func_1501A6CC(arg0, 0, 0, D_800BE620, D_800BE624);
            }
        }
    }
    return arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_13D350/func_151108C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_13D350/func_15110CFC.s")

extern s32 *D_800DBE80;
extern s32 *D_800891BC[];
s32 func_1502B7F0();
void func_10004074();
void func_1510D694(s32);
void func_15111858(void);

void func_1511172C(s32 arg0) {
    s32 i;
    struct104 *var_v0;
    struct104 * volatile *var_s3;
    s32 **var_s0;

    var_s3 = (struct104 * volatile *)&D_800B0DF0;
    if (arg0 == 1) {
        var_s3 = (struct104 * volatile *)&D_800B0DF0;
        (*var_s3)->unk8 = 1;
        var_s0 = &D_800DBE80;
        if (D_800DBE80 != 0) {
            s32 *temp_v0;

            temp_v0 = D_800DBE80;
            func_10004074(temp_v0);
        }
        func_15111858();
        return;
    }

    var_v0 = *var_s3;
    if (arg0 != var_v0->unk9) {
        if (var_v0->unk8 == 4) {
            if (D_80038080 != 0) {
                for (i = 0; i != 0x168; i++) {
                    func_1510D694(D_800891BC[(*var_s3)->unk9][0] + i);
                }
            }
            var_s0 = &D_800DBE80;
            func_10004074(*var_s0);
            (*var_s3)->unk9 = arg0;
            func_1502B7F0(var_s0, 2, 0xD, (*var_s3)->unk9);
        }
    }
}

extern void *allocate_memory(s32, s32, s32, s32);

typedef struct Star15111858 {
    u16 x;
    u16 y;
    u16 z;
    u8 unk6;
    u8 unk7;
} Star15111858;

void func_15111858(void) {
    Star15111858 *p;
    s32 i;

    D_800DBE80 = allocate_memory(0xFA00, 1, 0, 0);
    p = (Star15111858 *)D_800DBE80;
    for (i = 0; i != 8000; i++) {
        f32 r;
        f32 ang;
        f32 v;
        f32 lat;
        s32 h;
        f32 tmp;

        ang = (f32)(func_150ADA20() % 36000U) * 0.0001745329209f;
        v = (f32)(s32)((func_150ADA20() % 1584400U) - 792100) * 0.009999999776f;
        if (v >= 0.0f) {
            lat = 89.0f - sqrtf(v);
        } else {
            lat = sqrtf(-v) + -89.0f;
        }
        lat *= 0.01745329238f;
        h = func_150AD78C(lat) * 256.0f;
        tmp = h;
        p[i].y = *(u16 *)&tmp;
        r = sqrtf(0x10000 - h * h);
        tmp = (s32)(func_150AD78C(ang) * r);
        p[i].x = *(u16 *)&tmp;
        tmp = (s32)(func_150AD780(ang) * r);
        p[i].z = *(u16 *)&tmp;
        p[i].unk7 = (func_150ADA20() % 191U) + 0x40;
        p[i].unk6 = func_150ADA20() % 5U;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_13D350/func_15111AF4.s")
void func_15113180(void);

typedef struct Game13D350Entry {
    u16 index;
    u16 value;
} Game13D350Entry;

typedef struct Struct15113218Obj {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x16];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x16];
    u8 unk4E;
    u8 pad4F[0x21];
    u8 unk70;
    u8 pad71[0x2F];
} Struct15113218Obj;

typedef struct Struct15113218Sub {
    u8 pad0[0x30];
    f32 x;
    f32 y;
    f32 z;
} Struct15113218Sub;

typedef struct Struct15113218Actor {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x70];
    s16 unk90;
    u8 pad92[0x142];
    Struct15113218Sub *unk1D4;
    u8 pad1D8[0x154];
} Struct15113218Actor;

struct Struct1511490C;
void func_1511490C(f32 arg0[4][4], struct Struct1511490C *arg1);
void func_150A7B80(void *);
extern u8 **D_80089240;
extern u8 **D_80089250[];
extern u16 D_800DBEE8[];
extern f32 D_800DBF08[][4][4];
void func_15113218(void);

typedef struct Game13D350Cmd {
    u32 word0;
    u32 word1;
} Game13D350Cmd;

typedef struct Game13D350DrawEntry {
    u16 index;
    u16 value;
} Game13D350DrawEntry;

void *func_151733D8(void *, s32);
struct Game13D350Model;
s32 func_151137D4(s32, struct Game13D350Model *, s32, s32, s32, s32);
s32 func_1515E544(s32, s32, s32, s32, void *);
extern u8 **D_80089240;
extern u8 **D_80089250[];
extern u8 D_80089470;
extern u8 D_800D9E21;
extern u16 D_800DBEE8[];
Game13D350Cmd *func_151135C4(Game13D350Cmd *arg0, s32 arg1, s16 arg2);

typedef struct Game13D350Model {
    u8 pad0[4];
    f32 scale;
    u8 pad8[8];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[2];
    f32 yOffset;
    s32 displayList;
    s32 matrices[2];
    u8 pad28[0x27];
    u8 renderFlags;
    u8 pad50[0x1C];
    u16 effectFlags;
    u8 pad6E[2];
    u8 displayFlags;
    u8 pad71[0xB];
    s32 effectScale;
    u8 pad80[0xA];
    u8 alpha;
    u8 alphas[1];
} Game13D350Model;

s32 func_1511FC2C(s32, f32, f32, f32, f32, s32);
s32 func_1511FC44(s32, f32, f32, f32, s32);
s32 func_1510CDB8(s32, s32, s32, s32);
s32 func_1510D8C0(s32, Game13D350Model *);
s32 func_150CF5E8(s32);
s32 func_150A50C0(s32, s32, u8 *, s32, s32, s32);
extern u8 D_80083D40[];
extern u8 D_80083EC0[];
extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;
extern u8 D_800D9C10[];
s32 func_151137D4(s32 arg0, Game13D350Model *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

typedef struct Struct15113C88 {
    u8 pad0[0x4C];
    u8 unk4C;
    u8 pad4D[2];
    u8 unk4F;
    u8 pad50[0x20];
    u8 unk70;
    u8 pad71[0x1A];
    u8 unk8B[9];
    struct102 *unk94;
    u8 pad98[8];
} Struct15113C88;

extern s32 func_15022B08(s32, s32);
extern struct102 *func_1510D970(s32, void *, s32, s32, s32);
extern u8 D_800C3658;
void func_15113C88(void);


extern s32 D_800DBF98;

/* struct131 with the fields this file needs spelled out (the shared header
   stops at unk6F). */
typedef struct Struct15113E54 {
    u8  pad0[0x38];
    /* 0x38 */ void (*unk38)(struct Struct15113E54 *);
    u8  pad3C[0x1E];
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ s16 unk5E;
    u8  pad60[0xE];
    /* 0x6E */ u8  unk6E;
    /* 0x6F */ u8  unk6F;
    u8  pad70[0x10];
    /* 0x80 */ s32 unk80;
    u8  pad84[0x1C];
} Struct15113E54; /* size 0xA0 */

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 unk8[8];
} Struct150174C0; /* size 0x18 */

#define RECORDS (*(Struct150174C0 **)&D_800D23C0)
void func_15113E54(s32 arg0);

extern s32 *D_800DBF94;
s32 func_15114050(struct131 *arg0, s32 arg1);
s32 func_151140C4(struct131 *arg0);

typedef struct Struct15114188 {
    u8  pad0[0x4F];
    /* 0x4F */ u8  unk4F;
    u8  pad50[0x28];
    /* 0x78 */ void (*unk78)(struct Struct15114188 *, struct127 *);
    u8  pad7C[0x16];
    /* 0x92 */ u8  unk92;
    u8  pad93[0xD];
} Struct15114188; /* size 0xA0 */

struct Game13F9D0MovingActor;
void func_1511473C(struct Game13F9D0MovingActor *arg0, s32 arg1);

#define OBJS ((Struct15114188 *)D_800DBEF4)
void func_15114188(void);

typedef struct {
    /* 0x00 */ f32 rotX;
    /* 0x04 */ f32 rotY;
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x44];
    /* 0x5A */ s16 ox;
    /* 0x5C */ s16 oy;
    /* 0x5E */ s16 oz;
    /* 0x60 */ f32 a;
    /* 0x64 */ f32 b;
    /* 0x68 */ f32 c;
    /* 0x6C */ u8 pad6C[0x34];
} Struct15114348;

void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_15114348(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3);

typedef struct Game13F9D0MovingActor {
    u8 pad0[0x14];
    f32 xyz[3];
    u8 pad20[0x56];
    u16 angle;
    u8 pad78[2];
    s16 angleCopy;
    u8 pad7C[0xBB];
    u8 active;
} Game13F9D0MovingActor;

extern f32 D_800A2F60;
extern u16 D_800CBDA0[];
void func_1511473C(Game13F9D0MovingActor *arg0, s32 arg1);

void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

struct Struct151148A8 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};
void func_151148A8(f32 arg0[4][4], struct Struct151148A8 *arg1);

struct Struct1511490C {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[2];
    f32 unk18;
    char pad1C[0x10];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
};

void func_150A7CB0(f32 mtx[4][4], s32 x, s32 y, s32 z);
void func_1511490C(f32 arg0[4][4], struct Struct1511490C *arg1);
struct131 *func_151149AC(u8 id);

typedef struct Struct15114B94 {
    u8 pad0[0x4C];
    u8 enabled;
    u8 pad4D[2];
    u8 flags;
    u8 pad50[0x1E];
    u8 unk6E;
    u8 pad6F;
    u8 state;
    u8 pad71[0x1A];
    u8 players[9];
    struct102 *callback;
    u8 pad98[8];
} Struct15114B94;
void func_15114B94(u32 arg0);
s32 func_15114CC4(void *a0, s32 a1, s32 *a2, s32 a3);

typedef struct {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[0x5E];
    u16 unk74;
} Struct15114D24;

s32 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s16 arg3, u8 arg4, s32 arg5,
                  s16 arg6, s16 arg7, s16 arg8, s16 arg9, s16 argA);
void func_15114D24(Struct15114D24 *arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s32 arg5);
void func_15114F04(s32 arg0, s32 arg1, s32 arg2);

extern s32 D_800DBF9C;
extern s32 D_800DBFA0;
extern s32 D_800DBFA4;
void func_15114F44(s32 arg0, s32 arg1, s32 arg2);
