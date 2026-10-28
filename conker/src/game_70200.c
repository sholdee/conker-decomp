#include <ultra64.h>
#include "libc/stdarg.h"

#define func_15043BB8 func_15043BB8_header_mismatch
#include "functions.h"
#undef func_15043BB8
#include "variables.h"

void func_15042ECC();

typedef struct Game70200Anchor {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
} Game70200Anchor;

typedef struct Game70200TextNode {
    Game70200Anchor *attachedObject;
    f32 scale;
    s16 x;
    s16 yOrVerticalOffset;
    u8 flagsRaw;
    u8 kindSelector;
    u8 primaryRed;
    u8 primaryGreen;
    u8 primaryBlue;
    u8 primaryAlpha;
    u8 field_12;
    u8 field_13;
    u8 field_14;
    u8 field_15;
    u8 pad_16[2];
    struct Game70200TextNode *next;
    u8 text[0x40];
} Game70200TextNode;

void *allocate_memory(s32, s32, s32, s32);
s32 func_151EFF94(u8 *, const u8 *, ...);
extern u8 D_80085CC0[];
extern u8 D_80085CC4;
extern s32 D_80098B90;
extern s32 D_80098B94;
extern Game70200TextNode *D_800CBD68;
extern u8 D_800CBD6C;
extern u8 D_800CBD6D;
extern u8 D_800CBD6E;
extern u8 D_800CBD6F;



void func_15042D50(void) {
    D_800CBD64 = 0;
    func_15043384(0);
}

void func_15042D78(u8 arg0) {
    D_800CBD74 = arg0;
}

void func_15042D94(s32 arg0, s32 arg1, u8 arg2, s32 arg3, ...) {
    va_list args;
    s32 sp24[16];
    s32 i;

    D_800CBD74 = arg2;
    D_800CBD70 = arg0;
    D_800CBD72 = arg1;
    va_start(args, arg3);
    for (i = 0; i < 16; i++) {
        sp24[i] = va_arg(args, s32);
    }
    func_15042ECC(arg3, sp24);
}
void func_15042E3C(s32 arg0, ...) {
    va_list args;
    s32 sp24[16];
    s32 i;

    va_start(args, arg0);
    for (i = 0; i < 16; i++) {
        sp24[i] = va_arg(args, s32);
    }
    func_15042ECC(arg0, sp24);
}
void func_15042ECC(u8 *format, s32 *argumentWords) {
    struct {
        Game70200TextNode *node;
        s32 pad;
        s32 finished;
        s32 pad2;
    } local;
    Game70200TextNode *node;
    u8 *out;
    s32 argumentIndex;
    s32 conversionIndex;
    u8 ch;
    u8 flags;
    u8 *conversion;

    argumentIndex = 0;
    conversion = &D_80085CC4;
    while (*format != 0) {
        local.node = allocate_memory(sizeof(Game70200TextNode), 1, 0, 1);
        if (local.node == 0) {
            return;
        }
        local.node->next = 0;
        local.finished = 0;
        if (D_800CBD68 == 0) {
            D_800CBD64 = (s32)local.node;
        } else {
            D_800CBD68->next = local.node;
        }
        out = local.node->text;
        local.node->kindSelector = 0;
        do {
            ch = *format;
            if (ch == '%') {
                format++;
                *conversion = '%';
                for (conversionIndex = 1; conversionIndex != 0; conversionIndex++, format++) {
                    u8 *destination = &(&D_80085CC4)[conversionIndex];

                    destination[0] = *format;
                    destination[1] = 0;
                    switch (destination[0]) {
                    case 'X':
                    case 'd':
                    case 'x':
                        func_151EFF94(out, &D_80085CC4,
                                     argumentWords[argumentIndex++]);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    case 'F':
                    case 'f': {
                        f32 *number = (f32 *)argumentWords[argumentIndex++];
                        f32 value = *number;

                        func_151EFF94(out, D_80085CC0, &D_80098B90,
                                     &D_80098B94,
                                     (f64)value);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    }
                    case 's':
                        func_151EFF94(out, &D_80085CC4,
                                     argumentWords[argumentIndex++]);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    case '%':
                        *out++ = *format;
                        *out = 0;
                        conversionIndex = -1;
                        break;
                    default:
                        break;
                    }
                }
            } else if (ch == '#') {
                local.node->kindSelector = argumentWords[argumentIndex++];
                format += 2;
            } else if (ch == '\n') {
                format++;
                local.finished = 1;
            } else if (ch == 0) {
                local.finished = 1;
            } else {
                *out = ch;
                format++;
                out++;
                *out = 0;
            }
        } while (local.finished == 0);

        node = local.node;
        node->flagsRaw = D_800CBD74;
        node->attachedObject = (Game70200Anchor *)D_800CBD78;
        if (node->attachedObject != 0) {
            node->yOrVerticalOffset = D_800CBD7C;
            flags = node->flagsRaw | 1;
            node->flagsRaw = (node->flagsRaw = flags);
            if (D_8008FD90 >= 2) {
                node->scale = D_800CBD80 + D_800CBD80;
            } else {
                node->scale = D_800CBD80;
            }
        } else {
            node->x = D_800CBD70;
            node->yOrVerticalOffset = D_800CBD72;
            node->scale = D_800CBD80;
        }
        node->primaryRed = D_800CBD60;
        node->primaryGreen = D_800CBD61;
        node->primaryBlue = D_800CBD62;
        node->primaryAlpha = D_800CBD63;
        node->field_12 = D_800CBD6C;
        node->field_13 = D_800CBD6D;
        node->field_14 = D_800CBD6E;
        node->field_15 = D_800CBD6F;
        D_800CBD68 = node;
        if (node->kindSelector != 0) {
            node->primaryRed = 0xFF;
            node->primaryGreen = 0xFF;
            node->primaryBlue = 0xFF;
        } else {
            D_800CBD72 += 0xB;
        }
    }
    D_800CBD78 = 0;
}

void func_150432BC(f32 arg0) {
    D_800CBD80 = arg0;
}

void func_150432CC(s32 arg0, s32 arg1) {
    D_800CBD74 = D_800CBD74 | 1;
    D_800CBD74 = D_800CBD74;
    D_800CBD7C = arg1;
    D_800CBD78 = arg0;
}

void func_150432FC(s16 arg0, s16 arg1) {
    D_800CBD70 = arg0;
    D_800CBD72 = arg1;
}

void func_1504332C(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_800CBD60 = arg0;
    D_800CBD61 = arg1;
    D_800CBD62 = arg2;
    D_800CBD63 = arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15043384.s")

void func_15043A00(struct105 *arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        arg0->unk0 = arg1; // are these structs?
        arg0->unk4 = arg2;
        arg0->unkC = 0;
        arg0->unk8 = 0;
    }
}

// something with memcpy
s32 func_15043A20(u8 *arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    s32 n;

    while (arg4 != 0) {
        if (arg1 < arg2 + arg4) {
            n = arg1 - arg2;
        } else {
            n = arg4;
        }
        memcpy(arg0 + arg2, arg3, n);
        arg2 += n;
        arg3 += n;
        arg4 -= n;
        if (arg2 < arg1) {
        } else {
            arg2 = 0;
        }
    }
    return arg2;
}
s32 func_15043AC8(u8 *arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    s32 n;

    while (arg4 != 0) {
        if (arg1 < arg2 + arg4) {
            n = arg1 - arg2;
        } else {
            n = arg4;
        }
        memcpy(arg3, arg0 + arg2, n);
        arg2 += n;
        arg3 += n;
        arg4 -= n;
        if (arg2 < arg1) {
        } else {
            arg2 = 0;
        }
    }
    return arg2;
}

s32 func_15043B70(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 n;

    while (arg3 != 0) {
        if (arg1 < arg2 + arg3) {
            n = arg1 - arg2;
        } else {
            n = arg3;
        }
        arg2 += n;
        arg3 -= n;
        if (arg2 < arg1) {
        } else {
            arg2 = 0;
        }
    }
    return arg2;
}
s32 func_15043BB8(struct105 *arg0, u8 *arg1, s32 arg2) {
    s32 head;
    s32 tail;

    if (arg2 != 0) {
        if (arg1 != 0) {
            arg2 += 4;
            arg2 = (arg2 + 3) & ~3;
            tail = arg0->unkC;
            head = arg0->unk8;
            if (tail < head) {
                if (tail + arg2 >= head) {
                    return 1;
                }
            } else if ((tail + arg2) - arg0->unk4 >= head) {
                return 1;
            }
            arg2 -= 4;
            arg0->unkC = func_15043A20((u8 *)arg0->unk0, arg0->unk4, func_15043A20((u8 *)arg0->unk0, arg0->unk4, tail, (u8 *)&arg2, 4), arg1, arg2);
        }
    }
    return 0;
}

s32 func_15043B70(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

s32 func_15043CA4(struct105 *arg0, u8 *arg1, s32 arg2) {
    s32 v1;
    s32 sp30;
    s32 head;
    s32 ret;

    sp30 = 0;
    head = arg0->unk8;
    if (head == arg0->unkC) {
        return 0;
    }
    ret = func_15043AC8((u8 *)arg0->unk0, arg0->unk4, head, (u8 *)&sp30, 4);
    v1 = sp30;
    if (arg2 < v1) {
        arg2 -= 1;
        ret = func_15043AC8((u8 *)arg0->unk0, arg0->unk4, ret, arg1, arg2);
        arg1[arg2] = 0;
        ret = func_15043B70((u8 *)arg0->unk0, arg0->unk4, ret, sp30 - arg2);
    } else if (v1 != 0) {
        ret = func_15043AC8((u8 *)arg0->unk0, arg0->unk4, ret, arg1, sp30);
    }
    arg0->unk8 = ret;
    return sp30;
}
