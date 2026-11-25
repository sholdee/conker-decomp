#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);
s32 func_150229E4(struct127 *arg0);
void func_1504BB88(struct127 *arg0);
s32 func_150535F4(struct127 *arg0);
void func_15053750(struct127 *arg0);
void func_15054F74(struct127 *arg0);
void func_150562FC(struct127 *arg0);
void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
void func_150597FC(struct127 *arg0);
void func_150AFC68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5);

/* Object update step shared by several behaviours: runs the common per-object helpers. */
void func_15052590(struct127 *obj) {
    s32 flags;

    flags = obj->unk25C;
    if (flags & 0x80) {
        obj->unk25C = flags & ~0x80;
        func_150AFC68(D_800C3E78, 3, 0x3FC, 6, 0xFF, 0);
        func_150AFC68(D_800C3E78, 2, 0x3FC, 6, 0xFF, 0);
    }

    // D_800C3654 is declared s32 in variables.h but is read here as a byte
    if ((D_800C35EA != 1) || ((*(u8 *)&D_800C3654 != 0) && (func_150229E4(obj) == 0))) {
        func_15053750(obj);
        func_1504BB88(obj);
        obj->unk80 = 10;

        if (!obj->stunned) {
            func_15056B08(obj);
        } else if (obj->unk13D == 0) {
            func_150585F0(obj, 0.05f);
        }

        // facing angle (0x10000 units per turn) rotated a quarter turn, in degrees
        obj->unk40 = (s16)(obj->unk7A + 0x4000) * (360.0f / 65536.0f);
        func_15059140(obj);

        if ((obj->unk28 == 0.0f) && ((obj->unk184 & 0x1F) == 14)) {
            obj->health = 0;
        }

        func_150597FC(obj);

        if (obj->unk13D != 0) {
            func_150562FC(obj);
        } else {
            obj->unkC4 += (0.0f - obj->unkC4) * 0.2f;
        }

        func_15054F74(obj);
        if (func_150535F4(obj) == 0) {
            func_1502178C(obj, 0, -1);
        }
    }
}
