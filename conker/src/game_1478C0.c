#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_1511A410(s32 arg0, s32 *arg1) {
    s32 count;
    s32 found[2];
    s32 i;

    found[0] = 0;
    found[1] = 0;
    count = 0;
    for (i = 0; *(s8 *)((i << 3) + arg0) != -0x21 && count < 2; i++) {
        switch (*(s8 *)((i << 3) + arg0)) {
        case -3:
            found[count++] = i;
            break;
        }
    }
    *arg1 = found[1];
    return found[0];
}

typedef struct {
    s32 *unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
} Struct1511A494Entry;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x34];
    u16 unk54;
} Struct1511A494Arg;

extern Struct1511A494Entry D_80089324[];
extern Struct1511A494Entry D_80089444[];
extern s32 func_1511A410(s32, s32 *);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(void *, s32, s32, s32, s32);

void func_1511A494(Struct1511A494Arg *arg0, s32 *arg1, s32 *arg2) {
    s32 pad64;
    s32 sp60;
    s32 i;
    Struct1511A494Entry *found;
    s32 id;
    s32 frame;
    s32 dir;
    s32 sp48;
    s32 type;
    s32 width;
    s32 offset;
    s32 x;

    found = NULL;
    for (i = 0; i < 24; i++) {
        id = D_80089324[i].unk5;
        if (D_80089324[i].unk6 != -1) {
            if (D_800BE9F0 != D_80089324[i].unk6) {
                continue;
            }
            if (id != 0xFF) {
                id |= 0x8000;
            }
        }
        if (id == 0xFF || id == arg0->unk54) {
            found = &D_80089324[i];
            break;
        }
    }
    if (found != NULL) {
        if (*arg1 == 0) {
            x = func_1511A410(arg0->unk1C, &sp48);
            *arg1 = (sp48 << 16) | x;
        } else {
            sp48 = (u16)(*arg1 >> 16);
        }
        dir = (*arg2 >> 16) & 0xFFFF;
        dir = (s16)dir;
        frame = *arg2 & 0xFFFF;
        if (dir == 0) {
            dir = 1;
        }
        frame += dir;
        if (found->unk8 != 0) {
            if (frame < 0) {
                dir = 1;
                frame = found->unk7;
            } else if (frame >= found->unk4 * found->unk7) {
                dir = -1;
                frame = (found->unk4 - 1) * found->unk7;
            }
        } else {
            if (frame >= found->unk4 * found->unk7) {
                frame -= found->unk4 * found->unk7;
                dir = 1;
            }
        }
        x = found->unk0[frame / found->unk7];
        offset = 0;
        if (D_800BE9F0 == 6 || D_800BE9F0 == 0x3B) {
            type = 3;
        } else {
            type = 0x3E;
        }
        width = func_1510D0EC(x, &sp60, type, 0);
        if (sp48 != 0) {
            offset = sp60 + width - 0x20;
        }
        func_1510D874(arg0, width, offset, 4, 5);
        *arg2 = (dir << 16) | frame;
    }
}

extern void func_15116110(void *);

void func_1511A6FC(s32 *arg0) {
    if (arg0[0xF] != 0) {
        func_15116110(arg0);
    }
    func_1511A494(arg0, (u8 *)arg0 + 0x80, (u8 *)arg0 + 0x84);
}

extern u8 D_800C35EA;
extern struct131 *D_800DBEF4;
extern s32 func_15022B08(s32, s32);
extern void func_151162D4(void *);

void func_1511A738(struct131 *arg0) {
    if (D_800C35EA == 1) {
        if (func_15022B08(arg0 - D_800DBEF4, 0) != 0) {
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            *(f32 *)((u8 *)arg0 + 0x18) = 0.0f;
        }
    }
    func_151162D4(arg0);
    func_1511A494(arg0, (u8 *)arg0 + 0x80, (u8 *)arg0 + 0x84);
}
