#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Object update step shared by several behaviours: runs the common per-object helpers. */
void func_15052590(Actor *obj) {
    s32 flags;

    flags = obj->task_attrib;
    if (flags & 0x80) {
        obj->task_attrib = flags & ~0x80;
        func_150AFC68(dc_glbl_CharNum, 3, 0x3FC, 6, 0xFF, 0);
        func_150AFC68(dc_glbl_CharNum, 2, 0x3FC, 6, 0xFF, 0);
    }

    if ((D_800C35EA[0] != 1) || ((D_800C3654[0] != 0) && (func_150229E4(obj) == 0))) {
        func_15053750(obj);
        func_1504BB88(obj);
        obj->count2 = 10;

        if (!obj->stunned) {
            func_15056B08(obj);
        } else if (obj->carried == 0) {
            func_150585F0(obj, 0.05f);
        }

        // facing angle (0x10000 units per turn) rotated a quarter turn, in degrees
        obj->theta = (s16)(obj->langle + 0x4000) * (360.0f / 65536.0f);
        func_15059140(obj);

        if ((obj->shad == 0.0f) && ((obj->floorMaterial & 0x1F) == 14)) {
            obj->energy = 0;
        }

        func_150597FC(obj);

        if (obj->carried != 0) {
            func_150562FC(obj);
        } else {
            obj->lean_angle += (0.0f - obj->lean_angle) * 0.2f;
        }

        func_15054F74(obj);
        if (func_150535F4(obj) == 0) {
            func_1502178C(obj, 0, -1);
        }
    }
}
