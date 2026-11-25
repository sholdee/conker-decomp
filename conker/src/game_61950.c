#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Appends two commands built from the object's taskint[0] angle pair (two s16s):
 * opcode 0x12 with the second angle, opcode 0xE with the negated first one,
 * both scaled by 65536 / 18000.
 */
GameCommand *func_150344A0(GameCommand *cmd, s32 index) {
    s32 angle;

    cmd[0].opcode = 0x12;
    angle = ((s16 *)&movement_obj[index].taskint[0])[1];
    cmd[0].value = angle * 3.640889f;
    cmd[1].opcode = 0xE;
    angle = -((s16 *)&movement_obj[index].taskint[0])[0];
    cmd[1].value = angle * 3.640889f;
    return cmd + 2;
}

/* Calls func_1503B9BC for objects with id 0-4, 0x80 or 0xB0. */
void func_1503453C(s32 index) {
    switch (movement_obj[index].character_type) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x80:
    case 0xB0:
        func_1503B9BC(index);
        break;
    }
}

/* Per-object update hook for objects that have bone matrices. */
void func_150345E4(s32 index) {
    if (movement_obj[index].ob_matrix_ptr != NULL) {
        if (movement_obj[index].type == 1 || movement_obj[index].drawtype == 7) {
            func_1503453C(index);
        }

        if (movement_obj[index].type == 0x22) {
            func_1503453C(index);
        }

        switch (movement_obj[index].character_type) {
        case 4:
            if (D_800BE616 == 0) {
                func_15036310(0, index, 9, 0);
                func_15036310(0, index, 4, 0);
            }
            break;
        case 0x70:
            func_15034EB4(&movement_obj[index], 10, 11);
            break;
        case 0x7B:
            func_150F1A00(&movement_obj[index]);
            break;
        case 0x60:
            func_150DFDA4(&movement_obj[index]);
            break;
        }
    }
}

/* Zeroes the 3x3 rotation part of every bone matrix whose bit is set in unk9C (bones 0-31). */
void func_15034728(Actor *obj) {
    s32 bone;
    s32 row;
    s32 col;
    s32 lastBone;
    MtxF *bones;

    if (obj->ob_matrix_ptr != NULL && obj->unk9C != 0) {
        lastBone = func_1502DB20(obj->character_type) - 1;
        bones = obj->ob_matrix_ptr;
        bone = 31;
        if (lastBone < 31) {
            bone = lastBone;
        }
        for (; bone >= 0; bone--) {
            if (obj->unk9C & (1 << bone)) {
                for (row = 0; row < 3; row++) {
                    for (col = 0; col < 3; col++) {
                        // matching: bones[bone][row][col] lets IDO strength-reduce the row loop; the
                        // explicit (row << 4) byte offset keeps the per-row address recompute
                        ((f32 *)((u8 *)(bone + bones) + (row << 4)))[col] = 0.0f;
                    }
                }
            }
        }
    }
}

/* Runs func_15034728 on every active object with a non-zero unk9C mask. */
void func_150347E8(void) {
    Actor *obj;

    if (D_800BEAC0 != 0) {
        return;
    }

    obj = movement_obj;
    do {
        if (obj->type != 0 && obj->unk9C != 0) {
            func_15034728(obj);
        }
        obj++;
        // matching: the end is its own symbol (D_800D121C == &movement_obj[25]), and the loop is bottom-tested
    } while (obj != &D_800D121C);
}
