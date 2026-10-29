#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_150AE5B0(struct108 *arg0) {
    if ((D_800D2E4C->unk4 & 0x80) == 0) {
        if (func_1509BE40(0, 0x2000, 0xBB) != -1) {
            if (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 8) != 0) {
                arg0->unk84 |= 0x1000000;
                func_151254F4(arg0, D_800CC335 - 1);
            }
        } else if (func_151239CC(arg0, 8) != 0) {
            func_151254F4(arg0, 0);
        }
    }
    arg0->unk84 &= -0x4001;
    if ((D_800D2E4C->unk1 & 4) == 0) {
        if (func_1509BE40(1, 0x2000, 0x95, func_1509BE40(0, 0x2014, 0xB7) | 0x2000) != 0) {
            arg0->unk84 |= 0x1000000;
            if (((arg0->unk2C & 1) != 0) && (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 0) != 0)) {
                arg0->unk1B4 = 3;
                arg0->unk84 &= -5;
                func_15124B18(arg0);
            }
        } else if (func_151239CC(arg0, 0) != 0) {
            func_15124B18(arg0);
            arg0->unk84 &= 0xFEFFFFFF;
        }
    }
    if (func_1509BE40(1, 0x4082, 6, 0x9000) != 0) {
        arg0->unk84 |= 0x10000;
    } else {
        arg0->unk84 &= 0xFFFEFFFF;
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_DBA60/func_150AE790.s")
// some funky xor going on
typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
} Ent150AEB9C;
typedef struct {
    s32 count;
    Ent150AEB9C *ents;
} EntList150AEB9C;
typedef struct {
    u8 pad0[0x28];
    void *unk28;
    u8 unk2C;
    u8 unk2D;
} Spawn150AEB9C;
extern EntList150AEB9C D_800886E0[];
extern Ent150AEB9C *D_800886E4;
extern u32 D_15001B08[];
extern u8 D_800DCE50[];
void func_15001A08(void);

void func_150AEB9C(u8 *arg0) {
    u32 *p;
    u32 sum;
    s32 i;
    s32 idx;
    u32 end;

    idx = 0;
    if (arg0 != NULL) {
        sum = 0;
        end = (u32)D_15001B08;
        for (p = (u32 *)func_15001A08; (u32)p < end; p++) {
            sum ^= *p;
            sum <<= 1;
        }
        if (sum != 0xB4E42D60) {
            *(s32 *)(D_800DCE50 + 0x8C) = 0;
            *(s32 *)(D_800DCE50 + 0x22C) = 0;
        }
        for (i = 0; i < D_800886E0[idx].count; i++) {
            Spawn150AEB9C *obj;
            obj = (Spawn150AEB9C *)func_151491F4(D_800886E0[idx].ents[i].unk2, 0, -1, 1, 0x27, 8, 0xFF, 0);
            if (obj == NULL) {
                break;
            } else {
                EntList150AEB9C *list;
                list = &D_800886E0[idx];
                obj->unk28 = arg0;
                obj->unk2C = arg0[0x3B];
                obj->unk2D = list->ents[i].unk0;
            }
        }
    }
}

void func_150AECCC(struct42 *arg0) {
    arg0->unk96 = arg0->unk96 + (arg0->unk94 * D_800BE9E4);
    if (arg0->unk96 >= 0x1401) {
        arg0->unk96 = 0x1400;
    }
    arg0->unk9E = arg0->unk9E - ((s32) arg0->unk96 >> 8);
    arg0->unkA4 = arg0->unkA4 + D_800BE9E4;
    if (arg0->unkA4 >= 0x1A) {
        arg0->unkA4 = 0x19;
    }
}

void func_150AED4C(struct114 *arg0) {
    arg0->unk34 += arg0->unk14 * D_800BE9E4;
    if (arg0->unk2A < arg0->unk34) {
        arg0->unk3A = 70;
        arg0->unk34 = arg0->unk2A;
    }
    arg0->unk36 = arg0->unk34;
}

s32 func_150AED9C(void *arg0) {
    void *temp_v0 = *(void **)((s32)arg0 + 0x98);
    s32 temp_v1 = *(s16 *)((s32)arg0 + 0x1C) << 3;
    s32 temp_t7;

    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }
    temp_t7 = temp_v1 & 0xFF;
    *(u8 *)((s32)temp_v0 + 0x1B) = temp_v1;
    if (temp_t7 < 0) {
        return 0;
    }
    return 1;
}

s32 func_150AEDD8(struct202 *arg0) {
    if (arg0->unk1C < 0x20) {
        arg0->unk28 = arg0->unk1C * 8;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_150AEDF8;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150AEDF8;

void func_150AEDF8(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_150AEDF8 *temp_v0 = (SubA_150AEDF8 *)((u8 *)arg0 + 0x28);
    ArgB_150AEDF8 *b = (ArgB_150AEDF8 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA_150AEDF8 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_150AEDF8:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
