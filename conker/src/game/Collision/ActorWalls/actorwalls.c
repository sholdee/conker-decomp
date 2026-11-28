#include <ultra64.h>

/*
 * This file's .bss, in the original order. IDO lays out .bss in an order that depends on the declarations
 * before it, so it is defined before the subsystem headers.
 */
f32 gLedgeZ; // ledge probe results (collision_resolveMapWalls family in game/Collision/WallCollide/wallcollide.s)
f32 gLedgeAngle1;
f32 gLedgeAngle2;
s32 D_800CBD9C; // cleared by func_15044370
u16 gActorTouchedObject[25]; // per movement_obj: 1 + the world object it touches, 0 if none
u8 gWallProbeIsCamera;
u8 D_800CBDD3;
u8 D_800CBDD4;
f32 gProbeHeight; // collision probe height (collision_setProbeHeights)
f32 gProbeMidHeight; // collision probe mid-height
f32 D_800CBDE0[5]; /* not referenced */
f32 gLedgeHeight1; // ledge heights, -32768.0f if none
f32 gLedgeHeight2;
f32 gLedgeX;

#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/save.h"

/* A Latin-1 accented letter: its string, its code and the index of its upper/lower case pair. */
typedef struct {
    char *str;
    u8 code;
    u8 index;
} AccentedLetter;

/* Nothing reads this table. The link order puts it here: its .data and its 52 strings are the first data of
 * this object, before the data of game/Collision/Probe/probe. No code in this file uses it, so it can also
 * have been a data-only file between the two. */
static AccentedLetter gAccentedLetters[52] = { /* not referenced */
    { "\337", 0xDF, 0 }, { "\340", 0xE0, 1 }, { "\300", 0xC0, 1 }, { "\341", 0xE1, 2 },
    { "\301", 0xC1, 2 }, { "\342", 0xE2, 3 }, { "\302", 0xC2, 3 }, { "\343", 0xE3, 4 },
    { "\303", 0xC3, 4 }, { "\344", 0xE4, 5 }, { "\304", 0xC4, 5 }, { "\347", 0xE7, 6 },
    { "\307", 0xC7, 6 }, { "\350", 0xE8, 7 }, { "\310", 0xC8, 7 }, { "\351", 0xE9, 8 },
    { "\311", 0xC9, 8 }, { "\352", 0xEA, 9 }, { "\312", 0xCA, 9 }, { "\353", 0xEB, 10 },
    { "\313", 0xCB, 10 }, { "\354", 0xEC, 11 }, { "\314", 0xCC, 11 }, { "\355", 0xED, 12 },
    { "\315", 0xCD, 12 }, { "\356", 0xEE, 13 }, { "\316", 0xCE, 13 }, { "\357", 0xEF, 14 },
    { "\317", 0xCF, 14 }, { "\361", 0xF1, 15 }, { "\321", 0xD1, 15 }, { "\362", 0xF2, 16 },
    { "\322", 0xD2, 16 }, { "\363", 0xF3, 17 }, { "\323", 0xD3, 17 }, { "\364", 0xF4, 18 },
    { "\324", 0xD4, 18 }, { "\365", 0xF5, 19 }, { "\325", 0xD5, 19 }, { "\366", 0xF6, 20 },
    { "\326", 0xD6, 20 }, { "\371", 0xF9, 21 }, { "\331", 0xD9, 21 }, { "\372", 0xFA, 22 },
    { "\332", 0xDA, 22 }, { "\373", 0xFB, 23 }, { "\333", 0xDB, 23 }, { "\374", 0xFC, 24 },
    { "\334", 0xDC, 24 }, { "\375", 0xFD, 25 }, { "\335", 0xDD, 25 }, { "\377", 0xFF, 26 }
};

/* Clears D_800CBD9C. */
void func_15044370(void) {
    D_800CBD9C = 0;
}

/*
 * Runs collision_resolveMapWalls for obj against each enabled collision set (3 down to 0; set 3 is skipped when obj
 * flag 0x200 is set) and returns the summed results. With secondPass, also runs collision_findLedge on sets 0-2.
 */
s32 collision_resolveActorWalls(f32 x, f32 y, f32 z, Actor *obj, s32 arg4, s32 secondPass) {
    s32 i;
    s32 total;
    s32 saved_mode;

    gLedgeHeight1 = -32768.0f;
    gLedgeHeight2 = -32768.0f;
    total = 0;
    obj->touching_char = 0;
    collision_setProbeHeights(obj, x, y, z);
    saved_mode = D_800CBDD3;
    for (i = 3; i >= 0; i--) {
        if (gFloorProbeTypeEnabled[i] == 1 && (i != 3 || !(obj->noshove & 0x200))) {
            collision_selectMap(i);
            if (gMeshBuilt == 0) continue;
            total += collision_resolveMapWalls(x, y, z, obj, arg4);
        }
    }
    if (secondPass) {
        for (i = 0; i < 3; i++) {
            if (gFloorProbeTypeEnabled[i] == 1) {
                collision_selectMap(i);
                if (gMeshBuilt != 0) {
                    collision_findLedge(x, y, z, obj, 0);
                }
            }
        }
    }
    collision_selectMap(0);
    D_800CBDD3 = saved_mode;
    return total;
}

/* Copies a triangle's three s16 vertices into out, relative to (ox, oy, oz) and rotated about Y. */
void collision_rotateTriVerts(s16 **vertices, Vec3f *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset) {
    s16 *vtx;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    for (i = 0; i < 3; i++) {
        vtx = (s16 *)((u8 *)vertices[i] + offset);
        x = vtx[0] - ox;
        y = vtx[1] - oy;
        z = vtx[2] - oz;
        out[i].x = (x * cosY) + (z * sinY);
        out[i].y = y;
        out[i].z = (z * cosY) - (x * sinY);
    }
}

void func_15044658(void) {
}

/* Sets gProbeHeight/gProbeMidHeight (a height and a mid-height) for obj's collision probe. */
void collision_setProbeHeights(Actor *obj, f32 x, f32 y, f32 z) {
    s16 height;
    s16 unk2C;
    s16 unk2A;
    s32 half;
    s32 index;

    actor_computeSize(obj, &height, &unk2C, &unk2A);
    if (obj->type == 0x2D || obj->type == 0x2E || obj->type == 0x2C) {
        gProbeMidHeight = 0.0f;
        gProbeHeight = height;
    } else {
        height += fabsf(y - obj->ypos);
        index = obj - movement_obj;
    }
    if (obj->drawtype == 5) {
        gProbeMidHeight = 0.0f;
        gProbeHeight = height;
        return;
    }
    if (obj->inwater != 0) {
        gProbeHeight = height;
        gProbeMidHeight = height >> 1;
        return;
    }
    if (obj->character_type == 0x53) {
        gProbeHeight = height;
        gProbeMidHeight = height >> 1;
        return;
    }
    if (obj->character_type == 0x28) {
        half = 20;
        gProbeHeight = height - half;
        gProbeMidHeight = half + gProbeHeight * 0.5f;
        return;
    }
    if (index >= 0 && index < gPlayerCount && obj->shad != 0.0f) {
        gProbeHeight = height;
        gProbeMidHeight = height >> 1;
        return;
    }
    if (obj->character_type == 0x25) {
        half = height / 2;
        gProbeHeight = height - half;
        gProbeMidHeight = half + gProbeHeight * 0.5f;
        return;
    }
    gProbeHeight = height;
    gProbeMidHeight = height >> 1;
}
