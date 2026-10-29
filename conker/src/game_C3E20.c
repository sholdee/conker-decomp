#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_800D2DC0[];
extern s32 D_800D2E30[];

void func_15096970(void) {
    bzero(D_800D2DC0, 0x6C);
    D_800D2DB4 = 0;
}

s32 func_150969A0(s32 arg0) {
    s32 i;

    for (i = 0; i < arg0; i++) {
        if (D_800D2DC0[i * 0x24] != 0) {
            return 1;
        }
    }

    return 0;
}

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 unk20;
} Struct_D_800D2DC0;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} Struct_D_800DC020;

extern f32 D_800D2DB8;
void func_1512D560(struct108 *, s32, struct168 *);

s32 func_15096A68(s32 arg0) {
    u8 idx;
    Struct_D_800DC020 *pos;
    struct108 *obj;
    struct108 *obj2;

    switch (((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk0) {
    case 1:
        idx = ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk1;
        obj = &D_800DBFF0[idx];
        pos = *(Struct_D_800DC020 **)&D_800DC020;
        pos = &pos[((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk2 & 0xFF];
        D_800D2DB8 = obj->unk2C0;
        D_800D2DB4 = 1;
        D_800DBFF4[idx] = 3;
        ((struct168 *)&D_800C3600)->unk0 = 1;
        ((struct168 *)&D_800C3600)->unk4 = 0;
        ((struct168 *)&D_800C3600)->unk14 = pos->unk0;
        ((struct168 *)&D_800C3600)->unk18 = pos->unk2;
        ((struct168 *)&D_800C3600)->unk1C = pos->unk4;
        ((struct168 *)&D_800C3600)->unk20 = pos->unk14;
        ((struct168 *)&D_800C3600)->unk24 = pos->unkC;
        ((struct168 *)&D_800C3600)->unk28 = pos->unk10;
        ((struct168 *)&D_800C3600)->unk8 = 0.0f;
        func_1512D560(obj, 5, 0);
        func_1512D560(obj, 7, (struct168 *)&D_800C3600);
        if (!(((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unkC & 1)) {
            obj->unk3D0->unk7 = 0;
            obj->unk3D0->unk25C |= 0x200;
        }
        D_800D2E30[arg0] = ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk10;
        ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk0 = 2;
    lbl1:
        break;
    case 2:
        obj2 = &D_800DBFF0[((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk1];
        ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk4 -= D_800BE9A0;
        if (obj2->unk3D0->health == 0) {
            ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk4 = 0;
        }
        if (((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk4 <= 0) {
            obj2->pad3A4 = ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk20;
            if (!(((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unkC & 2)) {
                func_1512D560(obj2, 6, 0);
            }
            D_800D2DB4 = 0;
            if (!(((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unkC & 1)) {
                obj2->unk3D0->unk25C &= ~0x200;
                obj2->unk3D0->unk7 = 0xFF;
            }
            ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk4 = 0;
            ((Struct_D_800D2DC0 *)D_800D2DC0)[arg0].unk0 = 0;
            obj2->unk2C0 = D_800D2DB8;
            return 0;
        }
        break;
    }
    return 1;
}

extern u8 D_800C35EA;
extern s32 func_15096A68(s32);

void func_15096D08(void) {
    s32 i;
    u8 *p;

    i = 0;
    if (D_800C35EA != 1) {
        p = D_800D2DC0;
        while (1) {
            if (*p != 0) {
                if (func_15096A68(i) != 0) {
                    break;
                }
            }
            i++;
            p += 0x24;
            if (i == 3) {
                break;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_C3E20/func_15096D78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C3E20/func_1509759C.s")

void func_15097798(s32 arg0) {
    struct108 *temp_v0;
    struct17 sp20;
    s32 *temp_v1;

    temp_v0 = &D_800DBFF0[arg0];
    if (D_800D2DB4 != 0) {
        func_15048F90((struct17 *)&temp_v0->unk2A4, (struct17 *)&temp_v0->unk2F8, &sp20);
        func_1504917C(&sp20, &sp20);

        temp_v1 = &D_800D2E30[arg0];
        ((struct17 *)&temp_v0->unk2A4)->unk0 += (sp20.unk0 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2A4)->unk4 += (sp20.unk4 * -2.0f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2A4)->unk8 += (sp20.unk8 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk0 += (sp20.unk0 * -2.5f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk4 += (sp20.unk4 * -2.0f) * (f32)*temp_v1;
        ((struct17 *)&temp_v0->unk2F8)->unk8 += (sp20.unk8 * -2.5f) * (f32)*temp_v1;
    }
}
