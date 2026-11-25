#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_15036310(s32, s32, s32, s32);
extern void func_15034EB4(struct127 *, s32, s32);
extern void func_150F1A00(struct127 *);
extern void func_150DFDA4(struct127 *);
extern s32 func_1502DB20(u8);
void func_1503B9BC(void);

typedef struct GameCommand {
    s16 opcode;
    s16 value;
} GameCommand;

/* An actor's bone matrix: struct127.unk1D4 points at an array of these. */
typedef f32 MtxF[4][4];

/*
 * Appends two commands built from the object's unk2E4 angle pair (two s16s):
 * opcode 0x12 with the second angle, opcode 0xE with the negated first one,
 * both scaled by 65536 / 18000.
 */
GameCommand *func_150344A0(GameCommand *cmd, s32 index) {
    s32 angle;

    cmd[0].opcode = 0x12;
    angle = ((s16 *)&D_800CC2D0[index].unk2E4)[1];
    cmd[0].value = angle * 3.640889f;
    cmd[1].opcode = 0xE;
    angle = -((s16 *)&D_800CC2D0[index].unk2E4)[0];
    cmd[1].value = angle * 3.640889f;
    return cmd + 2;
}

/* Calls func_1503B9BC for objects with id 0-4, 0x80 or 0xB0. */
void func_1503453C(s32 index) {
    switch (D_800CC2D0[index].id) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x80:
    case 0xB0:
        func_1503B9BC();
        break;
    }
}

/* Per-object update hook for objects that have bone matrices. */
void func_150345E4(s32 index) {
    if (D_800CC2D0[index].unk1D4 != NULL) {
        if (D_800CC2D0[index].interaction_state == 1 || D_800CC2D0[index].unk5 == 7) {
            func_1503453C(index);
        }

        if (D_800CC2D0[index].interaction_state == 0x22) {
            func_1503453C(index);
        }

        switch (D_800CC2D0[index].id) {
        case 4:
            if (D_800BE616 == 0) {
                func_15036310(0, index, 9, 0);
                func_15036310(0, index, 4, 0);
            }
            break;
        case 0x70:
            func_15034EB4(&D_800CC2D0[index], 10, 11);
            break;
        case 0x7B:
            func_150F1A00(&D_800CC2D0[index]);
            break;
        case 0x60:
            func_150DFDA4(&D_800CC2D0[index]);
            break;
        }
    }
}

/* Zeroes the 3x3 rotation part of every bone matrix whose bit is set in unk9C (bones 0-31). */
void func_15034728(struct127 *obj) {
    s32 bone;
    s32 row;
    s32 col;
    s32 lastBone;
    MtxF *bones;

    if (obj->unk1D4 != NULL && obj->unk9C != 0) {
        lastBone = func_1502DB20(obj->id) - 1;
        bones = (MtxF *)obj->unk1D4;
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
    struct127 *obj;

    if (D_800BEAC0 != 0) {
        return;
    }

    obj = D_800CC2D0;
    do {
        if (obj->interaction_state != 0 && obj->unk9C != 0) {
            func_15034728(obj);
        }
        obj++;
        // matching: the end is its own symbol (D_800D121C == &D_800CC2D0[25]), and the loop is bottom-tested
    } while (obj != (struct127 *)&D_800D121C);
}
