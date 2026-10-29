#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_8009D954[];
extern u8 *D_80087240[];

/* Looks arg0->id up in the 7 rows of D_80087240 (row lengths in D_8009D954) and returns
 * the first entry of the row that contains it, or the id itself when no row does.
 * Same search as func_1503D5F0 (game_6A3D0). */
s32 func_15084D00(struct127 *arg0) {
    s32 key = arg0->id;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < D_8009D954[i]; j++) {
            if (key == D_80087240[i][j]) {
                return D_80087240[i][0];
            }
        }
    }
    return key;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B21B0/func_15084D70.s")

void func_15085410(struct127 *arg0, s32 arg1) {
    arg0->unk31C->pad11B[0] = arg1;
}

u8 func_15085420(struct127 *arg0) {
    return arg0->unk31C->pad11B[0];
}

extern s32  func_15084D70(s32, s32, s32, s16*, f32*, u8*, u8*, s32*, s32*, s32, s32);
extern f32  func_1506AD30(struct127 *, f32, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1505D6F0(struct127 *, s32);
extern void func_15005818(struct108 *, struct127 *, f32 *);
extern void func_15169040(s32, u8);

void func_15085430(struct127 *arg0, s32 arg1, s32 arg2) {
    s16 sp78[3];
    f32 sp6C[3];
    u8 sp6B;
    u8 sp6A;
    s32 sp64;
    s32 sp60;
    struct127 *oldObject;
    s32 oldIndex;
    struct { s32 unk0; u8 unk4; } sp50;

    func_15084D70(arg0->unk127, arg1, D_800D18A8, sp78, sp6C, &sp6B, &sp6A, &sp60, &sp64, arg2, 0);
    oldObject = D_800D154C;
    oldIndex = D_800C3E78;
    D_800D154C = arg0;
    D_800C3E78 = arg0 - D_800CC2D0;
    func_1506AD30(arg0, 800.0f, 1);
    D_800D154C = oldObject;
    D_800C3E78 = oldIndex;
    arg0->x_position = sp78[0];
    arg0->y_position = sp78[1];
    arg0->z_position = sp78[2];
    arg0->old_x_position = arg0->x_position;
    arg0->old_y_position = *(volatile f32 *)&arg0->y_position;
    arg0->old_z_position = arg0->z_position;
    arg0->unk76 = arg0->unk78 = arg0->unk7A = sp6A << 8;
    arg0->unk40 = (s16)(arg0->unk7A + 0x4000) * 0.0054931640625f;
    arg0->unkB8 = (sp6B * 360) >> 8;
    while (arg0->unkB8 > 180.0f) {
        arg0->unkB8 -= 360.0f;
    }
    arg0->unk28 = 0.0f;
    arg0->unk180 = arg0->y_position;
    if (arg0->unk31C != NULL) {
        func_1507C3E0(arg0, (s16 *)&arg0->unk31C->unk114, (s16 *)&arg0->unk31C->unk116, (s16 *)&arg0->unk31C->unk118);
    }
    arg0->unk1CC = arg0->y_position;
    arg0->unk2F8 &= ~0x10;
    arg0->unk22C |= 0x200;
    arg0->unk164 = 0.0f;
    arg0->unk168 = 0.0f;
    arg0->unk16C = 0.0f;
    arg0->unk170 = 0.0f;
    if (arg0->unk127 != 0xFF) {
        *((u8 *)arg0->unk31C + 0x23) = sp64;
        if (D_800BE616 && D_800D18A8) {
            func_1505D6F0(arg0, arg0 - D_800CC2D0);
        }
        if (arg0->unk31C->unk78 == 0 && arg0->unk31C->unk19B != 4 && arg0->unk31C->unk19B != 7) {
            arg0->unk83 = 0;
        }
    } else {
        arg0->unk83 = 0;
    }
    if (arg0->camera != NULL) {
        func_15005818(arg0->camera, arg0, sp6C);
    }
    sp50.unk0 = (s32)arg0;
    sp50.unk4 = arg0->unique_id;
    func_15169040((s32)&sp50, 4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B21B0/func_15085710.s")

/* Fields recovered from the shared 0x1C-byte player records. */
typedef struct GameB21B0PlayerRecord {
    s32 field_0;
    s32 field_4;
    s8 field_8;
    s8 field_9;
    u8 field_A;
    u8 padB;
    s32 field_C;
    u8 pad10[0xC];
} GameB21B0PlayerRecord;

extern GameB21B0PlayerRecord D_800D213C[];

s32 func_150859AC(s16 arg0, s16 arg1) {
    if (arg0 >= 0xFF) {
        return 0;
    }
    switch (arg1) {
    case 0:
        return *(u16 *)&D_800D2340 & (1 << arg0);
    case 1:
        return D_800D213C[arg0].field_0;
    case 2:
        return D_800D213C[arg0].field_4;
    case 3:
        return D_800D213C[arg0].field_8;
    case 4:
        return D_800D213C[arg0].field_9;
    case 5:
        return D_800D213C[arg0].field_A;
    case 6:
        return D_800D213C[arg0].field_C;
    }
}

void func_15085ABC(s16 arg0) {
    s32 i;

    i = 0;
    if (D_800D18A8 == 0) {
        for (i = 0; i != 4; i++) {
            if ((1 << i) & arg0) {
                func_15085710((s16)i, 5, D_800CC2D0[i].health);
            }
        }
    }
}
