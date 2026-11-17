#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 *unk0;
    s32 unk4;
} struct_game_197C20_0;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct_game_197C20_1;

typedef struct {
    s32 field_0x0;
    char pad_0[0x4];
    u8 field_0x8;
} struct_game_197C20_2;

typedef struct {
    u8 field_0x0;
    char pad_0[0xF];
    struct_game_197C20_2 *field_0x10;
    char pad_1[0x1];
    u8 field_0x15;
    char pad_2[0xA];
    u8 field_0x20;
} struct_game_197C20_3;

extern struct_game_197C20_0 *D_800DD254;
extern struct_game_197C20_0 *D_800DD258;
s32 func_1516A770(u8 *arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    char padB[0x26 - 0xB];
    u16 unk26;
    char pad28[4];
    s32 unk2C;
} Struct1516A7B0_Data;

typedef struct {
    char pad0[0xD];
    u8 unkD;
    u8 unkE;
    u8 unkF;
    Struct1516A7B0_Data *unk10;
    u8 unk14;
    u8 unk15;
    u16 unk16;
    u16 unk18;
    u8 unk1A;
    char pad1B[4];
    u8 unk1F;
    u8 unk20;
    char pad21[3];
    u8 *unk24;
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
} Struct1516A7B0;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
u8 *func_1516C878(struct_game_197C20_1 *arg0, s32 arg1, s16 arg2);
Struct1516A7B0 *func_1516A7B0(u32 arg0, s32 arg1, s32 arg2, s32 arg3, Struct1516A7B0_Data *arg4, s32 arg5);

#pragma GLOBAL_ASM("asm/nonmatchings/game_197F20/func_1516AA70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_197F20/func_1516B6BC.s")

u8 *func_1516C878(struct_game_197C20_1 *arg0, s32 arg1, s16 arg2) {
    u8 *temp_v1;
    u8 *sp0;
    s32 temp_v0;
    s32 temp_a0;

    switch (arg2) {
    case 0:
        temp_v1 = D_800DD254[arg0->unk4].unk0;
        break;
    case 1:
        temp_v1 = D_800DD258[arg0->unk4].unk0;
        break;
    case 2:
        temp_v1 = (u8 *)arg0->unk4;
        break;
    default:
        temp_v1 = sp0;
        break;
    }

    temp_v0 = 0;
    if (arg1 > 0) {
        temp_a0 = temp_v1[0];
        do {
            temp_v0++;
            if (temp_a0 != 0) {
                do {
                    temp_a0 = temp_v1[1];
                    temp_v1++;
                } while (temp_a0 != 0);
            }
            if (temp_a0 == 0) {
                do {
                    temp_a0 = temp_v1[1];
                    temp_v1++;
                } while (temp_a0 == 0);
            }
        } while (temp_v0 != arg1);
    }

    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_197F20/func_1516C934.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    char padA[2];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 *unk18;
    f32 *unk1C;
    f32 *unk20;
} Struct1516CC58_10;

typedef struct {
    char pad0[0x10];
    Struct1516CC58_10 *unk10;
    char pad14[0xB];
    u8 unk1F;
    char pad20[8];
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    char pad48[8];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
} Struct1516CC58;

typedef struct {
    char pad0[4];
    f32 unk4;
    f32 unk8;
} Struct1516CC58_BE628;

typedef struct {
    f32 unk0;
    char pad4[0x328];
} Struct1516CC58_CC310;

typedef struct {
    char pad0[6];
    u16 unk6;
    u16 unk8;
} Struct1516CC58_Vp;

extern Struct1516CC58_Vp D_8009055C;
extern Struct1516CC58_Vp D_8009057C;
extern Struct1516CC58_CC310 D_800CC310[];
extern f32 cosf(f32);
extern u16 D_80090562;
extern u16 D_80090582;
extern f32 sinf(f32);
void func_1501A764(s16 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);
void func_1516D0CC(Struct1516CC58 *arg0, f32 *arg1, f32 *arg2, f32 *arg3);

void func_1516CC58(Struct1516CC58 *arg0) {
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    Struct1516CC58_10 *temp_v0;
    s32 sp44;
    s32 sp40;
    f32 temp_f0;
    f32 temp_f2;
    f32 var_f0_2;

    temp_v0 = arg0->unk10;
    if (temp_v0->unk0 & 0x8000) {
        temp_f2 = *temp_v0->unk18 * 0.00999999977648f;
        arg0->unk5C = (*(Struct1516CC58_BE628 **)&D_800BE628)->unk4 * temp_f2;
        temp_f2 = *temp_v0->unk1C * 0.00999999977648f;
        temp_f2 = 1.0f - temp_f2;
        arg0->unk60 = (*(Struct1516CC58_BE628 **)&D_800BE628)->unk8 * temp_f2;
        var_f0_2 = *temp_v0->unk20 * 0.00999999977648f;
        var_f0_2 = (var_f0_2 < 0.0f) ? 0.0f : ((var_f0_2 > 1.0f) ? 1.0f : var_f0_2);
        var_f0_2 = (70.0f * var_f0_2) + 55.0f;
        var_f0_2 = var_f0_2 * 0.0174532923847f;
        arg0->unk58 = var_f0_2;
        if (temp_v0->unk0 & 0x10000) {
            func_1516D0CC(arg0, &arg0->unk5C, &arg0->unk60, &arg0->unk58);
        }
    } else {
        func_1501A764(0, temp_v0->unkC, temp_v0->unk10, temp_v0->unk14, &sp54, &sp50, &sp4C);
        if (arg0->unk1F == 0) {
            arg0->unk64 = sp54;
            arg0->unk68 = sp50;
            arg0->unk6C = (D_800CC310[arg0->unk10->unk9].unk0 - D_800DBFF0->unk37C) - 180.0f;
            arg0->unk1F = 1;
        }
        var_f0_2 = arg0->unk6C;
        arg0->unk5C = arg0->unk64;
        arg0->unk60 = arg0->unk68;
        if (var_f0_2 < 0.0f) {
            var_f0_2 += 360.0f;
        }
        if (var_f0_2 > 180.0f) {
            var_f0_2 -= 360.0f;
        }
        if (var_f0_2 > 90.0f) {
            var_f0_2 = 180.0f - var_f0_2;
        } else if (var_f0_2 < -90.0) {
            var_f0_2 = -180.0f - var_f0_2;
        }
        var_f0_2 = (var_f0_2 / -180.0f) + 0.5f;
        var_f0_2 = (70.0f * var_f0_2) + 55.0f;
        var_f0_2 = var_f0_2 * 0.0174532923847f;
        arg0->unk58 = var_f0_2;
        func_1516D0CC(arg0, &arg0->unk5C, &arg0->unk60, &arg0->unk58);
    }
    arg0->unk40 = arg0->unk5C;
    arg0->unk44 = arg0->unk60;
    sp44 = (arg0->unk38 / (f32)(u32)D_8009055C.unk6) * (f32)(u32)D_8009057C.unk6 * 0.25f;
    sp40 = (arg0->unk3C / (f32)(u32)D_8009055C.unk8) * (f32)(u32)D_8009057C.unk8 * 0.5f;
    arg0->unk50 = (s32)(((cosf(arg0->unk58 + 3.14159274101f) * (arg0->unk38 * 0.5f) * 1.14999997616f) + arg0->unk5C) - (f32)sp44);
    arg0->unk54 = ((sinf(arg0->unk58 + 3.14159274101f) * (arg0->unk3C * 0.5f) * 1.03999996185f) + arg0->unk60) - (f32)sp40;
}

typedef struct {
    char pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct1516D0CC_BE628;

void func_1516D0CC(Struct1516CC58 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    f32 top;
    f32 y;
    f32 left;
    f32 right;
    f32 xoff;
    f32 halfW;
    f32 h;
    f32 x;

    xoff = (arg0->unk38 / (f32)(u32)D_80090562) * (f32)(u32)D_80090582 * 0.25f * arg0->unk28;
    x = (*arg1 + (cosf(arg0->unk58 + 3.14159274101f) * (arg0->unk38 * 0.5f) * 1.14999997616f * arg0->unk28)) - xoff;
    halfW = arg0->unk38 * 0.5f;
    h = arg0->unk3C;
    y = *arg2;
    top = *arg2 - h;
    left = x - halfW;
    right = halfW + x;
    if (arg0 != NULL) {
        if (top < (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk24) {
            *arg2 = (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk24 + h;
        } else if ((*(Struct1516D0CC_BE628 **)&D_800BE628)->unk28 < y) {
            *arg2 = (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk28;
        }
        if (left < (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk2C) {
            *arg1 -= left - (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk2C;
        } else if ((*(Struct1516D0CC_BE628 **)&D_800BE628)->unk30 < right) {
            *arg1 += (*(Struct1516D0CC_BE628 **)&D_800BE628)->unk30 - right;
        }
        *arg1 = (s32)*arg1;
        *arg2 = (s32)*arg2;
    }
}

void func_1516D2D8(void) {
}

void func_1516D2E0(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x8 = 0;
        func_1516972C((struct102 *)arg0);
        arg0->field_0x15 = 0;
    }
}

void func_1516D328(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x0 |= 0x100;
        arg0->field_0x10->field_0x0 &= ~0x400;
        arg0->field_0x20 = arg0->field_0x10->field_0x8;
    }
}

u8 *func_1516D378(struct_game_197C20_3 *arg0) {
    if ((arg0 != 0) && (arg0->field_0x0 == 0x51)) {
        arg0->field_0x10->field_0x0 |= 0x100;
        arg0->field_0x10->field_0x0 &= ~0x400;
        return (u8 *)arg0;
    }
    return 0;
}

s32 func_1516D3C4(u8 *arg0) {
    s32 *temp;

    if ((arg0 != 0) && (*arg0 == 0x51)) {
        temp = *(s32 **)(arg0 + 0x10);
        *temp |= 0x500;
        return (s32)arg0;
    }
    return 0;
}
