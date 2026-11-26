#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
f32 (*gPointGroupMtx)[4]; // the current float matrix
s32 gSegmentBases[16];   // segment base addresses
u8 D_800D2C68;

#include "functions.h"
#include "variables.h"

/* Transforms the group's points by the matrix gPointGroupMtx into out[slot] (16 slots; empty slots are skipped). */
void sprite_transformPointGroup(PointGroup *group, f32 (*out)[3]) {
    GroupPoint *slots[16];
    s32 i;

    sprite_spreadGroupSlots(group, slots);
    for (i = 0; i < 16; i++) {
        if (slots[i] != NULL) {
            func_150A7960(gPointGroupMtx, slots[i]->x, slots[i]->y, slots[i]->z, &out[i][0], &out[i][1], &out[i][2]);
        }
    }
}

/* Spreads a group's points over 16 slots: slots first..last (inclusive) get consecutive points, the rest NULL. */
void sprite_spreadGroupSlots(PointGroup *group, GroupPoint **slots) {
    GroupPoint *point = group->points;
    s16 end = group->slots; // matching: the packed byte and `end` share one register
    s16 first = end & 0xF;
    s32 i;

    end = (end >> 4) + 1;

    for (i = 0; i < first; i++) {
        slots[i] = NULL;
    }
    for (i = first; i < end; i++) {
        slots[i] = point;
        point++;
    }
    for (; i < 16; i++) {
        slots[i] = NULL;
    }
}

/*
 * Writes texcoords[i] rotated by `angle` degrees about the centre of a width x height texture
 * into verts[i].v.tc (S10.5 fixed point).
 */
void sprite_rotateTexcoords(Vtx *verts, s16 (*texcoords)[2], s32 count, f32 angle, s32 width, s32 height) {
    f32 centerS;
    f32 centerT;
    f32 sine;
    f32 cosine;
    s32 i;

    sine = sinf(angle * 0.01745329238f); /* degrees to radians */
    cosine = cosf(angle * 0.01745329238f);
    centerS = (width / 2) << 5;
    centerT = (height / 2) << 5;
    for (i = 0; i < count; i++) {
        verts[i].v.tc[0] = ((texcoords[i][0] - centerS) * cosine) - ((texcoords[i][1] - centerT) * sine) + centerS;
        verts[i].v.tc[1] = ((texcoords[i][0] - centerS) * sine) + ((texcoords[i][1] - centerT) * cosine) + centerT;
    }
}

/* Empty stub. */
void func_15094E98(void) {
}
