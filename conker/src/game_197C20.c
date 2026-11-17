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


s32 func_1516A770(u8 *arg0) {
    s32 count = 1;

    while (*arg0 != 0) {
        if (*arg0 == 0xBD) {
            *arg0 = 0;
            count++;
        }
        arg0++;
    }
    return count;
}

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

Struct1516A7B0 *func_1516A7B0(u32 arg0, s32 arg1, s32 arg2, s32 arg3, Struct1516A7B0_Data *arg4, s32 arg5) {
    Struct1516A7B0 *obj;
    Struct1516A7B0_Data *data;
    s32 i;
    u8 *str;
    f32 h;
    s32 w;
    s32 hh;
    s32 sp38;

    obj = func_15167A68(0x51, 0, 0x74, 0, arg5, 2);
    if (obj != NULL) {
        if (arg4 == NULL) {
        /* unused label: the extra block keeps IDO from hoisting &w/&hh/&sp38 into s-regs */
        lbl4:
            if (arg0 >= 0x100) {
                data = (Struct1516A7B0_Data *)D_800DD258[arg0 - 0x100].unk0;
            } else {
                data = (Struct1516A7B0_Data *)D_800DD254[arg0].unk0;
            }
        } else {
            data = arg4;
        }
        if (arg4 != NULL) {
            obj->unkD = 2;
        } else if (arg0 >= 0x100) {
            obj->unkD = 1;
        } else {
            obj->unkD = 0;
        }
        data->unk9 = arg1;
        data->unkA = arg2;
        data->unk2C = arg3;
        obj->unkF = data->unk0 & 0xF;
        obj->unk10 = data;
        obj->unk15 = 1;
        obj->unk1F = 0;
        if (arg4 != NULL) {
            obj->unk14 = 1;
            obj->unk16 = arg4->unk26;
        } else {
            obj->unk14 = 0;
            obj->unk16 = data->unk26 >> 1;
        }
        obj->unk18 = 0;
        obj->unk1A = 0;
        obj->unk28 = 0.0f;
        obj->unk38 = 0.0f;
        obj->unk3C = 0.0f;
        if (obj->unkD == 2) {
            data->unk8 = func_1516A770((u8 *)data->unk4);
        }
        for (i = 0; i < data->unk8; i++) {
            str = func_1516C878((struct_game_197C20_1 *)data, i, obj->unkD);
            func_150428D4(str, &w, &hh, &sp38);
            if (obj->unk38 < (f32)w) {
                obj->unk38 = w;
            }
            if (obj->unk3C < (f32)hh) {
                obj->unk3C = hh;
            }
        }
        if (data->unk0 & 2) {
            h = 25.0f;
        } else {
            h = 18.0f;
        }
        obj->unk38 = (obj->unk38 + h * 0.5f) * 1.149999976f;
        obj->unk3C = (obj->unk3C + h) * 1.039999962f;
        obj->unk24 = func_1516C878((struct_game_197C20_1 *)data, 0, obj->unkD);
        obj->unk20 = 0;
        data->unk0 &= ~0x7000;
        ((u8 (*)[0x32C])&D_800CC2D0)[data->unk9][0x1E3] = 1;
        obj->unkE = 0;
    }
    return obj;
}
u8 *func_1516C878(struct_game_197C20_1 *arg0, s32 arg1, s16 arg2);

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
void func_1516CC58(Struct1516CC58 *arg0);

typedef struct {
    char pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct1516D0CC_BE628;
void func_1516D0CC(Struct1516CC58 *arg0, f32 *arg1, f32 *arg2, f32 *arg3);
void func_1516D2D8(void);
void func_1516D2E0(struct_game_197C20_3 *arg0);
void func_1516D328(struct_game_197C20_3 *arg0);
u8 *func_1516D378(struct_game_197C20_3 *arg0);
s32 func_1516D3C4(u8 *arg0);
