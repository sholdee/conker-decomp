#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    char pad_0[0x28];
    u8 field_0x28;
    u8 field_0x29;
    u8 field_0x2A;
    char pad_0x2B[0x2D];
    s32 field_0x58;
} GameObjectColorFlags;

void func_150D7420(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 2:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

void func_150D74DC(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 &= ~2;
                return;
            case 2:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

void func_150D758C(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 &= ~2;
                return;
            case 2:
                arg0->field_0x58 &= ~2;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

extern u16 D_800D9910;

void func_150D7630(s32 arg0) {
    if (arg0 == 0) {
        D_800D9910 += D_800BE9E4 << 6;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1048D0/func_150D765C.s")

s32 func_1508855C(s32);
void func_150885EC(s32, s32);
s32 func_1508868C(u8 *);

void func_150D7790(u8 *arg0, s32 arg1) {
    s32 new_id;
    s32 old_id;
    s32 variant;
    u8 *created;

    variant = 0;
    if ((*(u8 **)(arg0 + 0x31C)) != 0 && *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) <= 0) {
        return;
    }
    switch (arg1) {
    case 0x30:
    case 0x31:
        if ((*(u8 **)(arg0 + 0x31C)) != 0) {
            *(s16 *)((*(u8 **)(arg0 + 0x31C)) + 0x12C) = 0x78;
            *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) -= 1;
            func_10010154(0x628, arg0, 0x7FFF, 0x1F4, 0x3E8);
        }
        return;
    case 0x33:
        variant = 1;
    case 0x32:
        break;
    default:
        return;
    }
    created = func_1506C460(*(f32 *)(arg0 + 0x40), 310.0f, 0.0f, 0.0f,
                             ((variant * 0x1E) + 0x1E), 0xD, 0.0f, 1.0f, 0, 1, variant == 0);
    if (created != 0) {
        *(s8 *)(created + 0x124) = D_800C3E78 + 1;
    }
    if (created != 0 && ((variant * 0x1E) + 0x1E) != 0x1E) {
        *(s8 *)(created + 0x221) = -1;
        old_id = func_1508855C((s32)arg0);
        if (old_id != -1) {
            new_id = func_1508868C(created);
            if (new_id != -1) {
                func_150885EC(old_id, new_id);
            }
        }
    }
    if ((*(u8 **)(arg0 + 0x31C)) != 0) {
        *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) -= 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1048D0/func_150D7928.s")
