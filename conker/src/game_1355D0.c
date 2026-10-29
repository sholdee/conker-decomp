#include <ultra64.h>
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ struct17 unk08;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Arg15108120;

extern s32 D_800A2430;
extern f32 D_800A2434;
void func_150E83AC(struct17 *, s16, u8, s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, u8, s32);
void func_15152190(Arg15108120 *, s32 *, f32 *, s32, f32, s32, s32, s32);

void func_15108120(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 sp7C;
    struct17 sp70;

    sp7C.unk0 = arg0->x_position;
    sp7C.unk4 = arg0->y_position;
    sp7C.unk8 = arg0->z_position;
    sp70.unk0 = arg0->x_position;
    sp70.unk4 = arg0->y_position + 30.0f;
    sp70.unk8 = arg0->z_position;
    func_150E83AC(&sp7C, (s16)((func_150ADA20() % 201U) + 500), arg1, arg2);
    {
        Arg15108120 sp34;

        sp34.unk00 = 0xF;
        sp34.unk04 = 8;
        sp34.unk08 = sp70;
        sp34.unk14 = 0;
        sp34.unk16 = 0xFF;
        sp34.unk18 = -0x40;
        sp34.unk1A = 0x29;
        sp34.unk2C = 0x27;
        sp34.unk2E = 0x14;
        sp34.unk30 = 0.3050000072f;
        sp34.unk34 = 0.3050000072f;
        sp34.unk1C = 8.0f;
        sp34.unk20 = 10.0f;
        sp34.unk24 = -0.8000000119f;
        sp34.unk28 = 0.3440000117f;
        sp34.unk38 = 26.41100121f;
        func_15152190(&sp34, &D_800A2430, &D_800A2434, 1, 40.0f, 0, arg1, arg2);
    }
    func_15136C3C(arg0, 1, 1, 1, 0, 0, arg1, arg2);
}
