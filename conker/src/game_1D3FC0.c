#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A node of the object lists D_800DCE50[list][type] (see game_1944C0.c). */
typedef struct ListObj {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct ListObj *next;
    /* 0x0C */ u8 padC[7];
    /* 0x13 */ u8 type;
    /* 0x14 */ u8 pad14[0x14];
    /* 0x28 */ s32 owner;
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ struct127 *actor;
} ListObj;

extern ListObj *D_800DCE50[2][104];

extern void func_151A6AB8(struct210 *);
extern void func_1505D024(struct127 *, s32, u16, s32);
ListObj *func_151A6BD8(s32 owner);

void func_151A6B10(struct210 *arg0) {
    func_151A6AB8(arg0);
    func_1513CA6C(arg0);
}

void func_151A6B3C(struct210 *arg0) {
    func_151A6AB8(arg0);
    func_1513CAA0(arg0);
}

/* Sends message 0x60034 to `target` with its facing angle and the index of the actor owned by `owner`
 * (-1 if none). */
void func_151A6B68(struct127 *target, s32 owner) {
    ListObj *obj;
    struct127 *actor;
    s32 actorIndex;

    actor = NULL;
    obj = func_151A6BD8(owner);
    if (obj != NULL) {
        actor = obj->actor;
    }
    if (actor != NULL) {
        actorIndex = actor - D_800CC2D0;
    } else {
        actorIndex = -1;
    }
    func_1505D024(target, 0x60034, target->unk7A, actorIndex);
}

/* Returns the first type-0x2C object in either object list, among the object types listed in D_800A5770,
 * whose owner is `owner`, or NULL. */
ListObj *func_151A6BD8(s32 owner) {
    u8 i;
    u8 list;
    ListObj *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = D_800DCE50[list][(&D_800A5770)[i]]; obj != NULL; obj = obj->next) {
                if (obj->type == 0x2C && owner == obj->owner) {
                    return obj;
                }
            }
        // matching: the label starts a new basic block, which keeps IDO from hoisting list++
        next_list:;
        }
    }
    return NULL;
}
