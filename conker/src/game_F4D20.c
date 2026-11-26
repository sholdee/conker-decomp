#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* An 8-byte entry of the list at WorldObject.displayList; the list ends with an entry whose kind is -14. */
typedef struct {
    union {
        s8  kind;
        s32 word;
    } head;
    s32 unk4;
} F4D20Entry;

/* Whatever an object's work word points to in func_150C7968. */
typedef struct {
    /* 0x00 */ u8 pad00[0x13];
    /* 0x13 */ s8 unk13;
} LinkedObj;

/* Behaviour scratch state of the objects in this file (WorldObject.touch). */
typedef union {
    s32 count;        // func_150C7C90
    LinkedObj *link;  // func_150C7968
} F4D20Work;

#define WORK(obj) ((F4D20Work *)&(obj)->touch)

/* This file's .data. */
static s32 D_800887F0 = 0; /* not referenced */
static s32 D_800887F4 = 0; /* set while the camera is inside the zone of func_150C79BC */

/* Runs func_1511650C with sound 0x353 (or 0x43 at a lower pitch once object 0's flag bit 2 is set), unless
 * bit 3 of byte 0xA of the 27-byte flag record at gTaskDoneBits is set. */
void func_150C7870(WorldObject *obj) {
    if (!(gTaskDoneBits[0xA] & 8)) {
        if (!(db_Object_array[0].task_flag & TASK_FLAG_LOCKED)) {
            func_1511650C(obj, 1, 0x353, 1000.0f);
        } else {
            func_1511650C(obj, 1, 0x43, 400.0f);
        }
    }
}

/* Spins the object at the opposite of object 3's spin speed. */
void func_150C78E0(WorldObject *obj) {
    WorldObject *obj3;

    if (!(obj->task_flag & TASK_FLAG_LOCKED)) {
        obj3 = &db_Object_array[3];
        obj->moveData.word = -(obj3->moveData.word & 0xFFFF0000) & 0xFFFF0000;
        func_151150BC(obj);
    }
}

/* Spins the object at object 3's spin speed. */
void func_150C7930(WorldObject *obj) {
    WorldObject *obj3;

    obj3 = &db_Object_array[3];
    obj->moveData.word = obj3->moveData.word & 0xFFFF0000;
    func_151150BC(obj);
}

/* After func_15116110, copies object 3's spin speed / 16 into the linked object's unk13. */
void func_150C7968(WorldObject *obj) {
    WorldObject *obj3;
    LinkedObj *link;
    s32 speed;

    func_15116110(obj);
    if (!(obj->task_flag & TASK_FLAG_LOCKED)) {
        obj3 = &db_Object_array[3];
        link = WORK(obj)->link;
        speed = obj3->moveData.half.hi >> 4;
        if (link != NULL) {
            link->unk13 = speed;
        }
    }
}


/* For camera `view`: switches music_playTune ids 0xF <-> 0x5F as the camera enters or leaves a cylinder of
 * radius 1200 around (-2418, 1775) above y 2300, and while it is below y 1583 within 900 of that axis,
 * sometimes spawns a particle at a random point within 900 of it at y 1583. */
void func_150C79BC(s32 view) {
    s32 texture;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 radius;
    u8 angle;

    dx = (s32)camera_global_Cameras[view].eye.x + 2418;
    dy = (s32)camera_global_Cameras[view].eye.y - 2500;
    dz = (s32)camera_global_Cameras[view].eye.z - 1775;
    if (dy > -200 && dx * dx + dz * dz < 1200 * 1200) {
        if (D_800887F4 == 0) {
            music_playTune(0x5F, 0xF, 6);
            music_stopTune(0x10, 4);
            D_800887F4 = 1;
        }
    } else if (D_800887F4 != 0) {
        music_playTune(0xF, 0x5F, 4);
        music_playTune(0x10, 0, 3);
        D_800887F4 = 0;
    }
    dy = (s32)camera_global_Cameras[view].eye.y - 1583;
    if (dy < 0 && dx * dx + dz * dz < 900 * 900 && (func_150ADA20() & 0xFFFF) < 0x2000) {
        radius = func_150ADA20() % 900U;
        angle = func_150ADA20();
        // dx and dz now hold the spawn point itself
        dx = func_150489B0(angle) * radius + -2418.0f; // matching: "- 2418.0f" emits sub.s instead of add.s
        dz = func_15048A40(angle) * radius + 1775.0f;
        texture = ((func_150ADA20() & 0xFFFF) * 2) * (1.0f / 65536.0f);
        func_15179FE0(texture, dx, 1583, dz, 7, 1200, 37, 10, 30, 3, 8);
    }
}

/* Stores an angle (0x400 per turn) in the low 12 bits of the list's terminating entry (its index is found once
 * and cached in the work word). The angle comes from the unk12 of object (params & 0xFF), mirrored when params bit 15
 * is set. */
void func_150C7C90(WorldObject *obj) {
    F4D20Entry *pad; // matching: unused, but -g3 gives it the stack slot that puts last at sp+0x18
    F4D20Entry *last;
    s32 index;
    s32 count;
    s32 angle;

    index = WORK(obj)->count;
    if (index == 0) {
        last = (F4D20Entry *)obj->displayList;
        // matching: reading kind through a cast keeps IDO from strength-reducing last[count] to a cursor
        for (count = 0; *(s8 *)&last[count] != -14; count++) {
        }
        WORK(obj)->count = count;
        index = count;
    }
    last = &((F4D20Entry *)obj->displayList)[index];
    angle = -669 - gfx_findObjectById(obj->moveData.word & 0xFF)->y;
    if (obj->moveData.word & 0x8000) {
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

/* Places the object at (-30, +50, +30) from the Actor that func_15083E90(0xC) returns. */
void func_150C7D7C(WorldObject *obj) {
    Actor *target;

    target = func_15083E90(0xC);
    obj->x = target->xpos - 30.0f;
    obj->y = target->ypos + 50.0f;
    obj->z = target->zpos + 30.0f;
}
