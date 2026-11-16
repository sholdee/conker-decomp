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

void func_150C8350(void) {
    Struct150C8350 *record;
    register s32 offset;
    s32 clamp;
    s32 max;
    s32 current;
    s32 target;
    s32 adjustedTarget;
    s32 diff;
    s32 absDiff;
    s32 step;
    s32 value;

    clamp = 0x4FF;
    max = 0x500;
    record = (Struct150C8350 *)0;

    for (offset = (s32)record; offset != 0x64; offset += sizeof(Struct150C8350)) {
        record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
        current = record->unk6;
        target = record->unk8;
        if (current != target) {
            adjustedTarget = target;
            if (current < 0) {
                adjustedTarget = -target;
            }

            diff = adjustedTarget - current;
            step = D_800BE9E4 << 4;
            absDiff = (diff < 0) ? -diff : diff;

            if (absDiff < step) {
                record->unk6 = adjustedTarget;
                record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
                current = record->unk6;
            } else {
                if (diff < 0) {
                    diff = -1;
                } else {
                    diff = 1;
                }
                value = step * diff;
                record->unk6 = current + value;
                record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
                current = record->unk6;
            }
        }

        record->unk4 += (current * record->unk2) * D_800BE9A0;
        record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
        value = record->unk4;
        if (value >= max) {
            record->unk4 = value - max;
            record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
            record->unk4 = max - record->unk4;
            record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
            record->unk2 = -record->unk2;
            record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
            if (record->unk4 >= max) {
                record->unk4 = clamp;
            }
        } else if (value < 0) {
            record->unk4 = -value;
            record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
            record->unk2 = -record->unk2;
            record = (Struct150C8350 *)((u8 *)D_800BE4E0 + offset);
            if (record->unk4 < 0) {
                record->unk4 = 0;
            }
        }

    }
}

extern void func_150C8350(void);

void func_150C84F4(s32 arg0) {
    if (arg0 == 0) {
        func_150C8350();
    }
}

void func_150CDB6C(s32);

void func_150C851C(s32 arg0) {
    s32 i;
    s32 val;
    s32 lo;
    s32 range;

    if (D_800BE4E0 != 0) {
        func_150CDB6C(arg0);
        range = ((arg0 * 50) >> 8) + 50;
        lo = (arg0 * 300) >> 8;
        for (i = 0; i < 10; i++) {
            val = lo;
            val += (u32)func_150ADA20() % range;
            ((Struct150C8350 *)D_800BE4E0)[i].unk8 = val;
        }
    }
}


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

GameF5800Command *func_150C8600(GameF5800Command *arg0) {
    typedef struct { s32 *entries[2]; } PointerPair;
    s32 record_offset;
    s32 command_offset;
    s32 workspace[2];
    s32 variant;
    PointerPair tables;
    s32 result;

    tables = *(PointerPair *)D_80088810;
    for (record_offset = 0, command_offset = 4; command_offset != 0x2C; command_offset += 4, record_offset += 0xA) {
        variant = *(s16 *)((u8 *)D_800BE4E0 + record_offset + 4) >> 8;
        if (variant < 0) {
            variant = 0;
        } else if (variant >= 5) {
            variant = 4;
        }
        if (*((u8 *)D_800BE4E0 + record_offset) >= 2) {
            *((u8 *)D_800BE4E0 + record_offset) = 0;
        }
        result = func_1510D0EC(tables.entries[*((u8 *)D_800BE4E0 + record_offset)][variant], workspace, 3, 0);
        { GameF5800Command *command = arg0++; command->w0 = (command_offset & 0xFFFF) | 0xDB060000; command->w1 = result; }
    }
    return arg0;
}

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
s32 func_150C8730(Struct150C8730Arg0 *arg0, struct108 *arg1);

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
