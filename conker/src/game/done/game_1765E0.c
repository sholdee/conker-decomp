#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Allocates a timed callback object (0x28 header bytes plus dataSize) and initializes its header. */
void *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags,
                    u8 callbackSet, s32 dataSize, u8 owner, s32 list) {
    TimedEffect *object;
    u8 type;
    s32 size;

    size = dataSize + sizeof(TimedEffect);
    type = (flags & 2) ? 95 : 35;

    object = func_15167A68(type, list, size, 1, owner, 1);
    if (object == NULL) {
        return NULL;
    }
    object->header.state = timer;
    object->expireCallback = expireCallback;
    object->tickCallback = tickCallback;
    object->drawCallback = drawCallback;
    object->header.flags = flags;
    object->callbackSet = callbackSet;
    bzero(object->buffers, sizeof(object->buffers));

    return object;
}

/* func_15149130 without a draw callback. */
void *func_151491F4(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                    u8 owner, s32 list) {
    return func_15149130(timer, expireCallback, tickCallback, -1, flags, callbackSet, dataSize, owner, list);
}

/* Per-frame update: counts the timer down (flag 1), runs the tick callback, then the expire callback and
 * removal once the timer goes negative. */
void func_15149264(TimedEffect *object) {
    if (object->header.flags & 1) {
        object->header.state -= D_800BE9E4;
    }
    if (object->tickCallback != -1) {
        D_8008A4E8[object->tickCallback](object);
    }
    if (object->header.state < 0) {
        if (object->expireCallback != -1) {
            D_8008A4C0[object->expireCallback](object);
        }
    }
    if (object->header.state < 0) {
        func_1516972C(&object->header);
    }
}

void func_15149318(TimedEffect *object) {
    func_151D5E30(object->buffers);
}

void func_1514933C(TimedEffect *object) {
    func_15149318(object);
    func_15169804(&object->header);
}

void func_15149368(TimedEffect *object) {
    func_15149318(object);
    func_15169824(&object->header);
}

/* Dispatches through the object's callback set (out-of-range sets fall back to set 0). */
void func_15149394(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A688[set](object);
}

void func_151493E4(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A7B0[set](object);
}

void func_15149434(TimedEffect *object, s32 arg1, u8 arg2) {
    s32 set = object->callbackSet;

    if (set < 0 || set >= 74) {
        set = 0;
    }
    if (D_8008A8D8[set] != NULL) {
        D_8008A8D8[set](object, arg1, arg2);
    }
}

/* Runs the object's draw callback, if any, which may replace arg0 (returned). */
s32 func_15149490(s32 arg0, TimedEffect *object, s16 arg2) {
    s32 drawCallback = object->drawCallback;

    if (drawCallback != -1) {
        arg0 = D_8008A670[drawCallback](arg0, object, arg2);
    }
    return arg0;
}

void func_151494E0(s32 arg0, u8 arg1) {
    func_15169260(D_800A5770, 2, arg0, arg1);
}

/* func_15169850 for effects: tracks or retargets the effect's owner on actor events. */
void func_15149514(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    func_15169850(event, eventType, owner, ownerId, obj);
}
