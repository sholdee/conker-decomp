#include <ultra64.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/music.h"
#include "conker/particle.h"
#include "conker/prop.h"
#include "conker/script.h"

/* Whatever an object's work word points to in prop_scrollTextureByObject3. */
typedef struct {
    /* 0x00 */ u8 pad00[0x13];
    /* 0x13 */ s8 speedT;
} LinkedObj;

/* Behaviour scratch state of the objects in this file (WorldObject.touch). */
typedef union {
    s32 count;        // func_150C7C90
    LinkedObj *link;  // prop_scrollTextureByObject3
} LinkedSpinnerWork;

#define WORK(obj) ((LinkedSpinnerWork *)&(obj)->touch)

/* This file's .data. */
static s32 D_800887F0 = 0; /* not referenced */
static s32 gInMusicZone = 0; /* set while the camera is inside the zone of prop_levelUpdateMusicAndDebris */

/* Runs prop_updatePush with sound 0x353 (or 0x43 at a lower pitch once object 0's flag bit 2 is set), unless
 * bit 3 of byte 0xA of the 27-byte flag record at gTaskDoneBits is set. */
void func_150C7870(WorldObject *obj) {
    if (!(gTaskDoneBits[0xA] & 8)) {
        if (!(db_Object_array[0].task_flag & TASK_FLAG_LOCKED)) {
            prop_updatePush(obj, 1, 0x353, 1000.0f);
        } else {
            prop_updatePush(obj, 1, 0x43, 400.0f);
        }
    }
}

/* Spins the object at the opposite of object 3's spin speed. */
void prop_spinOppositeObject3(WorldObject *obj) {
    WorldObject *obj3;

    if (!(obj->task_flag & TASK_FLAG_LOCKED)) {
        obj3 = &db_Object_array[3];
        obj->moveData.word = -(obj3->moveData.word & 0xFFFF0000) & 0xFFFF0000;
        prop_spinZ(obj);
    }
}

/* Spins the object at object 3's spin speed. */
void prop_spinWithObject3(WorldObject *obj) {
    WorldObject *obj3;

    obj3 = &db_Object_array[3];
    obj->moveData.word = obj3->moveData.word & 0xFFFF0000;
    prop_spinZ(obj);
}

/* After prop_registerTextureScroll, copies object 3's spin speed / 16 into the linked object's speedT. */
void prop_scrollTextureByObject3(WorldObject *obj) {
    WorldObject *obj3;
    LinkedObj *link;
    s32 speed;

    prop_registerTextureScroll(obj);
    if (!(obj->task_flag & TASK_FLAG_LOCKED)) {
        obj3 = &db_Object_array[3];
        link = WORK(obj)->link;
        speed = obj3->moveData.half.hi >> 4;
        if (link != NULL) {
            link->speedT = speed;
        }
    }
}


/* For camera `view`: switches music_playTune ids 0xF <-> 0x5F as the camera enters or leaves a cylinder of
 * radius 1200 around (-2418, 1775) above y 2300, and while it is below y 1583 within 900 of that axis,
 * sometimes spawns a particle at a random point within 900 of it at y 1583. */
void prop_levelUpdateMusicAndDebris(s32 view) {
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
        if (gInMusicZone == 0) {
            music_playTune(0x5F, 0xF, 6);
            music_stopTune(0x10, 4);
            gInMusicZone = 1;
        }
    } else if (gInMusicZone != 0) {
        music_playTune(0xF, 0x5F, 4);
        music_playTune(0x10, 0, 3);
        gInMusicZone = 0;
    }
    dy = (s32)camera_global_Cameras[view].eye.y - 1583;
    if (dy < 0 && dx * dx + dz * dz < 900 * 900 && (math_random() & 0xFFFF) < 0x2000) {
        radius = math_random() % 900U;
        angle = math_random();
        // dx and dz now hold the spawn point itself
        dx = math_cosBinAngle(angle) * radius + -2418.0f; // matching: "- 2418.0f" emits sub.s instead of add.s
        dz = math_sinBinAngle(angle) * radius + 1775.0f;
        texture = ((math_random() & 0xFFFF) * 2) * (1.0f / 65536.0f);
        particle_spawnSpinDebris(texture, dx, 1583, dz, 7, 1200, 37, 10, 30, 3, 8);
    }
}

/* Stores an angle (0x400 per turn) in the low 12 bits of w0 of the first G_SETTILESIZE (0xF2) command of the object's
 * display list (its index is found once and cached in the work word). The angle comes from the unk12 of object (params & 0xFF), mirrored when params bit 15
 * is set. */
void func_150C7C90(WorldObject *obj) {
    Gfx *pad; // matching: unused, but -g3 gives it the stack slot that puts last at sp+0x18
    Gfx *last;
    s32 index;
    s32 count;
    s32 angle;

    index = WORK(obj)->count;
    if (index == 0) {
        last = obj->displayList;
        // matching: reading the opcode byte through a cast keeps IDO from strength-reducing last[count] to a cursor
        for (count = 0; *(s8 *)&last[count] != -14; count++) {
        }
        WORK(obj)->count = count;
        index = count;
    }
    last = &obj->displayList[index];
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
    last->words.w0 &= ~0xFFF;
    last->words.w0 |= angle;
}

/* Places the object at (-30, +50, +30) from the Actor that collision_findObjectById(0xC) returns. */
void func_150C7D7C(WorldObject *obj) {
    Actor *target;

    target = collision_findObjectById(0xC);
    obj->x = target->xpos - 30.0f;
    obj->y = target->ypos + 50.0f;
    obj->z = target->zpos + 30.0f;
}
