#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A3720;
extern f32 D_800DC004;
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

typedef struct Func1512E140Sub0 {
    u8 pad0[0x18];
    f32 unk18;
    u8 pad1C[0x84 - 0x1C];
    u16 unk84;
    u8 pad86[0xAD - 0x86];
    u8 unkAD;
    u8 padAE[0x102 - 0xAE];
    u8 unk102;
    u8 pad103[0x137 - 0x103];
    u8 unk137;
    u8 pad138[0x17C - 0x138];
    f32 unk17C;
    f32 unk180;
} Func1512E140Sub0;

typedef struct Func1512E140Sub1 {
    u8 pad0[0x95];
    u8 unk95;
    u8 pad96[0x9C - 0x96];
    s32 unk9C;
} Func1512E140Sub1;

typedef struct Func1512E140Object {
    u8 pad0[0x2C];
    s32 unk2C;
    u8 pad30[0x84 - 0x30];
    s32 unk84;
    u8 pad88[0x23C - 0x88];
    u8 unk23C;
    u8 pad23D[0x25C - 0x23D];
    f32 unk25C;
    u8 pad260[0x2FC - 0x260];
    f32 unk2FC;
    u8 pad300[0x344 - 0x300];
    f32 unk344;
    f32 unk348;
    u8 pad34C[0x354 - 0x34C];
    f32 unk354;
    u8 pad358[0x360 - 0x358];
    f32 unk360;
    u8 pad364[0x36C - 0x364];
    u16 *unk36C;
    u8 pad370[0x3B4 - 0x370];
    f32 unk3B4;
    f32 unk3B8;
    u8 pad3BC[0x3D0 - 0x3BC];
    Func1512E140Sub0 *unk3D0;
    Func1512E140Sub1 *unk3D4;
    u8 pad3D8[0x5F0 - 0x3D8];
    s32 unk5F0;
    u8 pad5F4[0x6F0 - 0x5F4];
    f32 unk6F0;
    f32 unk6F4;
    u8 pad6F8[0x73C - 0x6F8];
    s16 unk73C;
    u8 pad73E[0x7B4 - 0x73E];
    f32 unk7B4;
    u8 pad7B8[0x7F4 - 0x7B8];
    u16 unk7F4;
    u8 pad7F6[0x920 - 0x7F6];
    u16 unk920;
    u8 pad922[0x92C - 0x922];
    u8 unk92C;
} Func1512E140Object;

void func_1512E140(Func1512E140Object *arg0) {
    f32 var_f16;
    f32 sp48;
    s32 sp44;
    f32 var_f14;

    if (D_800C3671 != 0) {
        return;
    }
    if (!(arg0->unk2C & 0x100) && (arg0->unk92C != 0) && !(arg0->unk920 & 8)) {
        return;
    }
    sp48 = arg0->unk2FC - arg0->unk354;
    if ((arg0->unk5F0 & 0x10) && (*arg0->unk36C & 0x10)) {
        var_f16 = 95.0f;
    } else {
        var_f16 = arg0->unk348;
    }
    if (arg0->unk5F0 & 0x400) {
        var_f16 *= 1.5f;
    }
    if (!(arg0->unk84 & 0x10)) {
        if (arg0->unk3D0->unk180 < arg0->unk3D0->unk17C) {
            var_f14 = arg0->unk3D0->unk17C - 40.0f;
            if ((var_f14 < (arg0->unk354 + var_f16)) && (arg0->unk354 < (arg0->unk3D0->unk17C - 40.0f))) {
                var_f16 = var_f14 - arg0->unk354;
                sp44 = 1;
            } else {
                sp44 = 0;
            }
        }
    }
    if ((arg0->unk6F0 != 0.0f) && (arg0->unk6F4 != 0.0f)) {
        var_f16 = (var_f16 < arg0->unk6F0) ? arg0->unk6F0 : ((arg0->unk6F4 < var_f16) ? arg0->unk6F4 : var_f16);
    }
    if (arg0->unk7F4 != 0) {
        var_f16 = D_800DC004;
    }
    if (arg0->unk23C != 0) {
        arg0->unk25C = 0;
        sp48 = var_f16;
    } else {
        if (((arg0->unk5F0 & 0x40) && (arg0->unk2C != 0x100)) || (arg0->unk2C & 0x01000000)) {
            var_f16 = 0;
        } else if (((arg0->unk3D0->unk18 - arg0->unk3D0->unk180) > 600.0f) && !(arg0->unk5F0 & 0x400) &&
                   (arg0->unk3D0->unkAD == 0) && (arg0->unk3D0->unk102 == 0) &&
                   (arg0->unk3D4->unk9C == 0) && (arg0->unk3D4->unk95 == 0) &&
                   (arg0->unk3D0->unk137 == 0) && (arg0->unk3D0->unk84 != 0x194) && !(arg0->unk5F0 & 0x80)) {
            return;
        }
        if ((arg0->unk2C & 0x100) || sp44) {
            func_150495B0(&sp48, var_f16, &arg0->unk25C, 5.0f, 9.0f, arg0->unk7B4);
        } else {
            func_150495B0(&sp48, var_f16, &arg0->unk25C, arg0->unk3B4, arg0->unk3B8, arg0->unk7B4);
        }
    }
    arg0->unk2FC = arg0->unk354 + sp48;
    arg0->unk344 = arg0->unk2FC - arg0->unk354;
    if ((arg0->unk3D0->unkAD == 1) && (arg0->unk73C != 0) && (D_800A3720 != arg0->unk360)) {
        if (arg0->unk2FC < arg0->unk360 + 5.0f) {
            arg0->unk2FC = arg0->unk360 + 5.0f;
            arg0->unk344 = 5.0f;
        }
    }
}
