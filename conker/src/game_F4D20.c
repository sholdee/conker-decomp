#include <ultra64.h>
// functions.h declares these two as returning void / u8; hide those prototypes and redeclare them below.
#include "functions.h"
#include "variables.h"

/* An 8-byte entry of the list at F4D20Obj.unk1C; the list ends with an entry whose kind is -14. */
typedef struct {
    union {
        s8  kind;
        s32 word;
    } head;
    s32 unk4;
} F4D20Entry;

/* Whatever an object's unk7C points to in func_150C7968. */
typedef struct {
    /* 0x00 */ u8 pad00[0x13];
    /* 0x13 */ s8 unk13;
} LinkedObj;

/* The 0xA0-byte level objects (D_800DBEF4 entries) driven by the callbacks in this file. */
typedef struct {
    /* 0x00 */ f32 angleX;
    /* 0x04 */ f32 angleY;
    /* 0x08 */ f32 angleZ;
    /* 0x0C */ u8  pad0C[0x4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8  pad16[0x6];
    /* 0x1C */ F4D20Entry *unk1C;
    /* 0x20 */ u8  pad20[0x1C];
    /* 0x3C */ union {
        s32 word;
        struct {
            s16 hi; // spin speed for func_151150BC
            u16 lo;
        } half;
    } params;
    /* 0x40 */ u8  pad40[0x33];
    /* 0x73 */ u8  flags;
    /* 0x74 */ u8  pad74[0x8];
    /* 0x7C */ union {
        s32 count;        // func_150C7C90
        LinkedObj *link;  // func_150C7968
    } unk7C;
    /* 0x80 */ u8  pad80[0x20];
} F4D20Obj; // size 0xA0

#define sObjs ((F4D20Obj *)D_800DBEF4)

extern void func_1511650C(F4D20Obj *obj, s32 arg1, s32 soundId, f32 pitchScale);
extern WorldObject *func_151149AC(u8);
extern void func_151150BC(F4D20Obj *obj);
extern s32 func_15116110(F4D20Obj *obj);

/* Runs func_1511650C with sound 0x353 (or 0x43 at a lower pitch once object 0's flag bit 2 is set), unless
 * bit 3 of byte 0xA of the 27-byte flag record at D_800D2E4C is set. */
void func_150C7870(F4D20Obj *obj) {
    if (!(((u8 *)D_800D2E4C)[0xA] & 8)) { // struct102 has no field at byte 0xA (inside unk8)
        if (!(sObjs[0].flags & 4)) {
            func_1511650C(obj, 1, 0x353, 1000.0f);
        } else {
            func_1511650C(obj, 1, 0x43, 400.0f);
        }
    }
}

/* Spins the object at the opposite of object 3's spin speed. */
void func_150C78E0(F4D20Obj *obj) {
    F4D20Obj *obj3;

    if (!(obj->flags & 4)) {
        obj3 = &sObjs[3];
        obj->params.word = -(obj3->params.word & 0xFFFF0000) & 0xFFFF0000;
        func_151150BC(obj);
    }
}

/* Spins the object at object 3's spin speed. */
void func_150C7930(F4D20Obj *obj) {
    F4D20Obj *obj3;

    obj3 = &sObjs[3];
    obj->params.word = obj3->params.word & 0xFFFF0000;
    func_151150BC(obj);
}

/* After func_15116110, copies object 3's spin speed / 16 into the linked object's unk13. */
void func_150C7968(F4D20Obj *obj) {
    F4D20Obj *obj3;
    LinkedObj *link;
    s32 speed;

    func_15116110(obj);
    if (!(obj->flags & 4)) {
        obj3 = &sObjs[3];
        link = obj->unk7C.link;
        speed = obj3->params.half.hi >> 4;
        if (link != NULL) {
            link->unk13 = speed;
        }
    }
}

extern void func_1000D96C(s32 newId, s32 oldId, s32 transition);
extern void func_1000DE1C(s32, s32);
extern f32 func_150489B0(u8 angle); /* cos */
extern void func_15179FE0(u8 texture, s16 x, s16 y, s16 z, s8 velY, u16 timer, u16 unkAC, u8 speedRange,
                          u8 spinRange, u8 dragRange, s32 type);
extern s32 D_800887F4; /* set while the camera is inside the zone below */

/* For camera `view`: switches func_1000D96C ids 0xF <-> 0x5F as the camera enters or leaves a cylinder of
 * radius 1200 around (-2418, 1775) above y 2300, and while it is below y 1583 within 900 of that axis,
 * sometimes spawns a particle at a random point within 900 of it at y 1583. */
void func_150C79BC(s32 view) {
    s32 texture;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 radius;
    u8 angle;

    dx = (s32)D_800DBFF0[view].unk2F8 + 2418;
    dy = (s32)D_800DBFF0[view].unk2FC - 2500;
    dz = (s32)D_800DBFF0[view].unk300 - 1775;
    if (dy > -200 && dx * dx + dz * dz < 1200 * 1200) {
        if (D_800887F4 == 0) {
            func_1000D96C(0x5F, 0xF, 6);
            func_1000DE1C(0x10, 4);
            D_800887F4 = 1;
        }
    } else if (D_800887F4 != 0) {
        func_1000D96C(0xF, 0x5F, 4);
        func_1000D96C(0x10, 0, 3);
        D_800887F4 = 0;
    }
    dy = (s32)D_800DBFF0[view].unk2FC - 1583;
    if (dy < 0 && dx * dx + dz * dz < 900 * 900 && (func_150ADA20() & 0xFFFF) < 0x2000) {
        radius = func_150ADA20() % 900U;
        angle = func_150ADA20();
        // dx and dz now hold the spawn point itself
        dx = func_150489B0(angle) * radius + -2418.0f; // matching: "- 2418.0f" emits sub.s instead of add.s
        dz = func_15048A40(angle) * radius + 1775.0f;
        texture = (f32)((func_150ADA20() & 0xFFFF) * 2) * (1.0f / 65536.0f);
        func_15179FE0(texture, dx, 1583, dz, 7, 1200, 37, 10, 30, 3, 8);
    }
}

/* Stores an angle (0x400 per turn) in the low 12 bits of the list's terminating entry (its index is found once
 * and cached in unk7C). The angle comes from the unk12 of object (params & 0xFF), mirrored when params bit 15
 * is set. */
void func_150C7C90(F4D20Obj *obj) {
    F4D20Entry *pad; // matching: unused, but -g3 gives it the stack slot that puts last at sp+0x18
    F4D20Entry *last;
    s32 index;
    s32 count;
    s32 angle;

    index = obj->unk7C.count;
    if (index == 0) {
        last = obj->unk1C;
        // matching: reading kind through a cast keeps IDO from strength-reducing last[count] to a cursor
        for (count = 0; *(s8 *)&last[count] != -14; count++) {
        }
        obj->unk7C.count = count;
        index = count;
    }
    last = &obj->unk1C[index];
    angle = -669 - func_151149AC(obj->params.word & 0xFF)->y;
    if (obj->params.word & 0x8000) {
        angle = 836 - angle;
    }
    while (angle < 0) {
        angle += 0x400;
    }
    while (angle >= 0x400) {
        angle -= 0x400;
    }
    last->head.word &= ~0xFFF;
    last->head.word |= angle;
}

/* Places the object at (-30, +50, +30) from the struct127 that func_15083E90(0xC) returns. */
void func_150C7D7C(F4D20Obj *obj) {
    struct127 *target;

    target = func_15083E90(0xC);
    obj->x = target->x_position - 30.0f;
    obj->y = target->y_position + 50.0f;
    obj->z = target->z_position + 30.0f;
}
