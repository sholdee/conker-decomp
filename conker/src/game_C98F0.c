#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern struct127 *func_1505EEF4(s32);
extern void func_15022190(s16, s16, s16, f32);
extern void func_150221E8(s16, s16, s16, f32);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 args[6];
} Struct_1509C440;

extern void func_1501D348(s32, s32, s32, s32, s32);
extern s32 D_800D3840;
extern u8 D_800C35C2;
extern u8 D_800C35C3;
extern u8 D_800C3C99;
extern u8 D_800C3C9C;
void func_1509C8FC(s32 ref, s32 alt);

/* Fixed-point 16.16 to float scale. */
#define FIX16_TO_F32 (1.0f / 65536.0f)

/*
 * Event handler (address-taken, see the table at 800884D4). `event` selects the action; for
 * D_800D3840 mode 6 the params hold a position (args[0..2]) and a 16.16 angle (args[3]).
 */
s32 func_1509C440(s32 id, s32 event, Struct_1509C440 *params) {
    s32 value;
    s32 savedValue;
    s32 value2;
    s32 extra0;
    s32 extra1;

    switch (event) {
    case 1:
        if (D_800C35EA == 0) {
            value = D_800BE9F0;
            if (D_800D3840 == 6) {
                func_15022190(params->args[0], params->args[1], params->args[2], params->args[3] * FIX16_TO_F32);
            } else if (D_800D3840 == 3) {
                // Type 2 in bits 12-15: the low 12 bits are an object id.
                if ((params->args[0] & 0xF000) == 0x2000) {
                    func_1509C8FC(params->args[0], 0);
                } else {
                    value = params->args[0];
                }
            }
            func_1501D348(value, id, 0, 0, 0);
            return 1;
        }
        return 0;
    case 4:
        if (D_800C35EA == 0) {
            if (D_800D3840 == 6) {
                func_15022190(params->args[0], params->args[1], params->args[2], params->args[3] * FIX16_TO_F32);
            }
            savedValue = D_800BE9F0;
            D_800BE9F0 = 0x25;
            // matching: passing 0x25 directly reschedules the switch
            func_1501D348(D_800BE9F0, id, 0, 0, 0);
            D_800BE9F0 = savedValue;
            return 1;
        }
        return 0;
    case 3:
        if (D_800C35EA == 0) {
            extra0 = params->args[0];
            extra1 = params->args[1];
            value2 = D_800BE9F0;
            if (D_800D3840 == 8) {
                func_15022190(params->args[2], params->args[3], params->args[4], params->args[5] * FIX16_TO_F32);
            } else if (D_800D3840 == 5) {
                value2 = params->args[2];
            }
            func_1501D348(value2, id, 0, extra0, extra1);
            return 1;
        }
        return 0;
    case 2: {
        s32 current = D_800BE9F0;

        D_800C35C2 = id + 1;
        D_800C35C3 = current;
        D_800C3C99 = 1;
        if (D_800D3840 == 6) {
            func_150221E8(params->args[0], params->args[1], params->args[2], params->args[3] * FIX16_TO_F32);
        } else if (D_800D3840 == 3) {
            if ((params->args[0] & 0xF000) == 0x2000) {
                func_1509C8FC(params->args[0], 1);
            } else {
                D_800C35C3 = params->args[0];
            }
        }
    } break; // matching: the block must close before the break (line steers as1)
    case 5:
        D_800C35C2 = id + 1;
        D_800C35C3 = 0x25;
        if (D_800D3840 == 6) {
            func_150221E8(params->args[0], params->args[1], params->args[2], params->args[3] * FIX16_TO_F32);
        }
        break;
    case 6:
        D_800C3C9C = 1;
        break;
    }
    // Events 2, 5 and 6 (and unknown ones) fall off the end with no return value.
}

extern u8 D_800C35E8;
extern u8 D_800C3C98;
extern u8 D_800C3672;

/* Condition check per event; its table entry (80088498) sits near func_1509C440's (800884D4). */
s32 func_1509C7C8(s32 id, s32 event, s32 unused) {
    switch (event) {
    case 1:
        if (D_800C35C4 != 0) {
            return 0;
        }
        if (D_800C35EA == 0) {
            return 1;
        }
        if (id == D_800C3C98) {
            return 0;
        }
        return 1;
    case 2:
        if ((D_800C35EA == 1) && (id == D_800C3C98)) {
            return 1;
        }
        return 0;
    case 3:
        if ((D_800C35EA == 1) && (id == D_800C35E8)) {
            return 1;
        }
        return 0;
    case 4:
        return D_800C35EA != 1;
    case 5:
        return D_800C3672;
    case 6:
        if ((D_800C35EA == 1) && (id == D_800C35E8)) {
            return 1;
        }
        return 0;
    }
}

/*
 * Looks up the object whose id is the low 12 bits of `ref` and passes its position, with the
 * player's (D_800DBFF0) angle turned by 180 degrees, to func_150221E8 if `alt` else func_15022190.
 */
void func_1509C8FC(s32 ref, s32 alt) {
    struct127 *obj;
    struct108 *player;
    f32 angle;

    obj = func_1505EEF4(ref & 0xFFF);
    if (obj != NULL) {
        player = D_800DBFF0;
        if (player->unk2C & 0x100) {
            // struct108 (include/structs.h) has no field at 0x760; it lies inside pad73E.
            angle = *(f32 *)((u8 *)player + 0x760);
        } else {
            angle = player->unk37C;
        }

        if (alt) {
            func_150221E8(obj->x_position, obj->y_position, obj->z_position, angle + 180.0f);
        } else {
            func_15022190(obj->x_position, obj->y_position, obj->z_position, angle + 180.0f);
        }
    }
}
