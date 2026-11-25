#include <ultra64.h>
/* include/functions.h types func_15149130's seventh parameter as a struct37 pointer, but every caller
 * passes the byte size of the effect's extra data; the header declaration is renamed out of the way
 * (same approach as game_113D60.c). */
#define func_15149130 func_15149130_struct37_decl_in_functions_h
#include "functions.h"
#undef func_15149130
#include "variables.h"

void *func_15167A68(s32, s32, s32, s32, s32, s32);
void func_151D5E30(void *);

struct260 *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags,
                         u8 callbackSet, s32 dataSize, u8 arg7, s32 arg8);

/* Allocates a timed callback object (0x28 header bytes plus dataSize) and initializes its header. */
struct260 *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags,
                         u8 callbackSet, s32 dataSize, u8 arg7, s32 arg8) {
    struct260 *object;
    u8 kind;
    s32 size;

    size = dataSize + 0x28;
    kind = (flags & 2) ? 95 : 35;

    object = func_15167A68(kind, arg8, size, 1, arg7, 1);
    if (object == NULL) {
        return NULL;
    }
    object->unkE  = timer;
    object->unk10 = expireCallback;
    object->unk11 = tickCallback;
    object->unk12 = drawCallback;
    object->unkD  = flags;
    object->unk13 = callbackSet;
    bzero(&object->unk14, 16);

    return object;
}

/* func_15149130 without a draw callback. */
struct260 *func_151491F4(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                         u8 arg6, s32 arg7) {
    return func_15149130(timer, expireCallback, tickCallback, -1, flags, callbackSet, dataSize, arg6, arg7);
}

/* Per-frame update: counts the timer down (flag 1), runs the tick callback, then the expire callback and
 * removal once the timer goes negative. */
void func_15149264(struct260 *object) {
    if (object->unkD & 1) {
        object->unkE -= D_800BE9E4;
    }
    if (object->unk11 != -1) {
        D_8008A4E8[object->unk11](object);
    }
    if (object->unkE < 0) {
        if (object->unk10 != -1) {
            D_8008A4C0[object->unk10](object);
        }
    }
    if (object->unkE < 0) {
        func_1516972C(object);
    }
}

void func_15149318(struct260 *object) {
    func_151D5E30(&object->unk14);
}

void func_1514933C(struct260 *object) {
    func_15149318(object);
    func_15169804(object);
}

void func_15149368(struct260 *object) {
    func_15149318(object);
    func_15169824(object);
}

/* Dispatches through the object's callback set (out-of-range sets fall back to set 0). */
void func_15149394(struct260 *object) {
    s32 set = object->unk13;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A688[set](object);
}

void func_151493E4(struct260 *object) {
    s32 set = object->unk13;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A7B0[set](object);
}

void func_15149434(struct260 *object, s32 arg1, u8 arg2) {
    s32 set = object->unk13;

    if (set < 0 || set >= 74) {
        set = 0;
    }
    if (D_8008A8D8[set] != NULL) {
        D_8008A8D8[set](object, arg1, arg2);
    }
}

/* Runs the object's draw callback, if any, which may replace arg0 (returned). */
s32 func_15149490(s32 arg0, struct260 *object, s16 arg2) {
    s32 drawCallback = object->unk12;

    if (drawCallback != -1) {
        arg0 = D_8008A670[drawCallback](arg0, object, arg2);
    }
    return arg0;
}

void func_151494E0(s32 arg0, u8 arg1) {
    func_15169260(&D_800A5770, 2, arg0, arg1);
}

void func_15149514(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_15169850(arg0, arg1, arg2, arg3, arg4);
}
