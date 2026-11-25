#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/**
 * Death handling for obj (called once its health is 0): picks the outcome from the
 * flag byte of its D_800D1C90 entry. Returns 0 only when unk10E is in 1..0xEF.
 */
s32 func_15053430(Actor *obj) {
    u8 flags;

    if (obj->character_type == 0xFF) {
        func_15060F28(obj, 1);
        return 1;
    }

    flags = D_800D1C90[obj->character_type]->flags;
    if ((obj->unk10E != 0) && (obj->unk10E < 0xF0)) {
        func_1505D2B8(obj, 9);
    } else {
        s32 result = 0;

        if ((obj->unk10E < 0xF0) && (flags & 0x20)) {
            func_151669A0(obj->xpos, obj->ypos + 50.0f, obj->zpos, 0.5f, 0xFF, 0);
        }
        if (flags & 4) {
            result = func_1507D754(obj);
        }
        if (flags & 2) {
            obj->type = 42;
        } else if (flags & 0x80) {
            func_15055B0C(obj, 1200);
        } else if (flags & 1) {
            func_15053694(obj);
        } else if (flags & 0x40) {
            D_800D2110[obj->listpos] = 1250;
            func_15060F28(obj, 1);
        } else if (result == 1) {
            func_150536D0(obj);
        } else if (result == 0) {
            func_15060F28(obj, 1);
        }
        return 1;
    }
    return 0;
}

/**
 * Runs func_15053430 for obj once its health is 0 and its D_800D1C90 flags (or
 * unk10F == 0) allow it. Returns 0 when nothing was done.
 */
s32 func_150535F4(Actor *obj) {
    u8 flags;

    if (obj->energy == 0) {
        if (obj->character_type == 0xFF) {
            func_15060F28(obj, 1);
            return 1;
        }
        flags = D_800D1C90[obj->character_type]->flags;
        if ((flags & 0x10) ||
            ((flags & 8) && (obj->shad == 0.0f)) ||
            (obj->unk10F == 0)) {
            return func_15053430(obj);
        }
    }
    return 0;
}

/** Sets obj's interaction state to 8 and starts func_1506160C mode 6. */
void func_15053694(Actor *obj) {
    obj->type = 8;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    func_1506160C(obj, 6, 0, 0, 0);
}
