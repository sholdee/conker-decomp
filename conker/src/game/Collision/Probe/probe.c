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
struct TriggerNode *gTriggerNodes; // timed trigger nodes (collision_addTriggerNode)

#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/heap.h"
#include "conker/math.h"
#include "conker/save.h"

/* A Latin-1 accented letter: its string, its code and the index of its upper/lower case pair. */
typedef struct {
    char *str;
    u8 code;
    u8 index;
} AccentedLetter;

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

/* Trigger checks (TriggerNode.check) and actions (TriggerNode.action). */
static s32 (*gTriggerChecks[])() = { collision_checkPlayerInVolume, collision_checkTrackedVolume, collision_runVolumeAttack };
static void (*gTriggerActions[])(void) = { NULL, func_15044DA0, func_15044DE8, func_15044E40, func_15044E88 };

/* Up to guLookAtF, sqrtf is a real call (jal sqrtf), not the inline sqrt.s of gu.h's #pragma intrinsic. */
#pragma function sqrtf

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

/* Adds a box trigger volume whose position and size are read from the given s16 pointers. */
TriggerVolume *collision_addTriggerVolume(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size) {
    TriggerVolume *volume;

    volume = (TriggerVolume *)collision_addTriggerNode(sizeof(TriggerVolume), 1, lifetime, delay, action, 0, 0, 0);
    if (volume == NULL) {
        return NULL;
    }
    volume->depth = depth;
    volume->width = width;
    volume->height = height;
    volume->angle = angle;
    volume->position = position;
    volume->size = size;
    return volume;
}

/* Allocates a trigger node and appends it to the gTriggerNodes list. */
TriggerNode *collision_addTriggerNode(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z) {
    TriggerNode *node;
    TriggerNode *tail;

    node = allocate_memory(size, 1, 0, 0);
    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->lifetime = lifetime;
    node->check = check;
    node->action = action;
    node->delay = delay;
    node->x = x;
    node->y = y;
    node->z = z;

    if (gTriggerNodes == NULL) {
        gTriggerNodes = node;
    } else {
        for (tail = gTriggerNodes; tail->next != NULL; tail = tail->next) {
        }
        tail->next = node;
    }

    return node;
}

/* Per frame: runs each node's check (and its action when the check passes), then ages and frees expired nodes. */
void collision_updateTriggers(void) {
    TriggerNode *node;
    TriggerNode *prev;
    TriggerNode *next;
    s32 check;
    s32 countdown;
    s32 lifetime;
    s32 delay;

    prev = NULL;
    for (node = gTriggerNodes; node != NULL; node = next) {
        countdown = node->delay;
        check = node->check;
        next = node->next;
        if (countdown == 0) {
            if (gTriggerChecks[check](node) != 0) {
                gTriggerActions[node->action]();
            }
        } else {
            delay = countdown - gFrameTicks;
            if (delay < 0) {
                delay = 0;
            }
            node->delay = delay;
        }
        lifetime = node->lifetime;
        if (lifetime != -1) {
            lifetime -= gFrameTicks;
            if (lifetime <= 0) {
                if (prev == NULL) {
                    gTriggerNodes = node->next;
                } else {
                    prev->next = node->next;
                }
                heap_setTag(node, 2);
            } else {
                node->lifetime = lifetime;
                prev = node;
            }
        } else {
            prev = node;
        }
    }
}


/* Check 0: whether the player's bounds overlap the volume's box, rotated about Y by its angle. */
s32 collision_checkPlayerInVolume(TriggerVolume *volume) {
    f32 cosA;
    f32 sinA;
    f32 dzRot;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 pad[3]; // matching: unused stack slots
    s32 playerHalfHeight;
    s32 playerWidth;
    s32 playerDepth;

    playerHalfHeight = movement_obj[0].mv->height >> 1;
    playerWidth = movement_obj[0].mv->width;
    playerDepth = movement_obj[0].mv->depth;
    dx = movement_obj[0].xpos - volume->node.x;
    dy = (movement_obj[0].ypos - volume->node.y) + playerHalfHeight;
    dz = movement_obj[0].zpos - volume->node.z;
    sinA = math_sinBinAngle(volume->angle);
    cosA = math_cosBinAngle(volume->angle);
    dzRot = dz * cosA + dx * sinA;
    dx = dx * cosA - dz * sinA;
    if (fabsf(dy) < volume->height + playerHalfHeight && fabsf(dx) < volume->width + playerWidth &&
        fabsf(dzRot) < volume->depth + playerDepth) {
        return TRUE;
    }
    return FALSE;
}

/* Check 1: refreshes the volume's position and size from its source pointers, then runs check 0. */
s32 collision_checkTrackedVolume(TriggerVolume *volume) {
    s16 *position = volume->position;
    s32 extent;

    volume->node.x = position[0];
    volume->node.y = position[1];
    volume->node.z = position[2];
    extent = volume->size[0] / 32;
    volume->depth = extent;
    volume->width = extent;
    volume->height = extent;
    return collision_checkPlayerInVolume(volume);
}

/* Check 2: calls actor_runAttackAt at the node's position with its depth; never fires. */
s32 collision_runVolumeAttack(TriggerVolume *volume) {
    actor_runAttackAt(volume->node.x, volume->node.y, volume->node.z, volume->depth, 0xFF, 0, 0, 0);
    return FALSE;
}

/* Trigger actions 1-4: hit the player with a actor_hitScripted type unless stunned or immune. */

void func_15044DA0(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    actor_hitScripted(&movement_obj[0], 5, movement_obj[0].langle, -1);
}

void func_15044DE8(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    if (gCutsceneStates[0] != 1) {
        actor_hitScripted(&movement_obj[0], 4, movement_obj[0].langle, -1);
    }
}

void func_15044E40(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    actor_hitScripted(&movement_obj[0], 0x40, movement_obj[0].langle, -1);
}

void func_15044E88(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    actor_hitScripted(&movement_obj[0], 1, movement_obj[0].langle, -1);
}

/* One candidate triangle returned by the collision_gatherGroundTris/collision_gatherActorGroundTris/collision_gatherPropGroundTris grid queries. */


/* Ceiling probe against collision set 0: lowest triangle between pos.y and top. */
s32 collision_probeCeilingSet0(Vec3f *pos, f32 top, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (top < pos->y) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = 99999.8984375f;
    best = -1;
    collision_selectMap(0);
    count = collision_gatherGroundTris(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (pos->y <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        u32 j;
        s32 index;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        index = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            Vtx *vtx = (Vtx *)vertices[j] + index;
            hit->tri.v[j].x = vtx->v.ob[0];
            hit->tri.v[j].y = vtx->v.ob[1];
            hit->tri.v[j].z = vtx->v.ob[2];
        }
        {
            s32 triangle = (MeshTri *)candidate->tri - gCurMapTris;
            hit->material = gMeshTriMaterials[triangle];
        }
        hit->flags |= 5;
        hit->source = 1;
        hit->surface = NULL;
        if (hit->height <= top) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe against collision set 0: highest triangle between bottom and pos.y. */
s32 collision_probeFloorSet0(Vec3f *pos, f32 bottom, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = -10000.0f;
    best = -1;
    collision_selectMap(0);
    gFloorQueryX = pos->x;
    gFloorQueryY = pos->y;
    gFloorQueryZ = pos->z;
    gRayProbeBottom = bottom;
    count = collision_gatherGroundTris(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (height <= pos->y && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        u32 j;
        s32 index;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        index = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            Vtx *vtx = (Vtx *)vertices[j] + index;
            hit->tri.v[j].x = vtx->v.ob[0];
            hit->tri.v[j].y = vtx->v.ob[1];
            hit->tri.v[j].z = vtx->v.ob[2];
        }
        if (gMeshTriMaterials != NULL) {
            s32 triangle = (MeshTri *)candidate->tri - gCurMapTris;
            hit->material = gMeshTriMaterials[triangle];
        } else {
            hit->material = 0;
        }
        hit->flags |= 7;
        hit->source = 1;
        hit->surface = NULL;
        if (bottom <= hit->height) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}


/* Ceiling probe: re-tests the cached triangle first, then searches collision set 0. */
s32 collision_probeCeilingSet0Cached(Vec3f *pos, f32 top, CollisionHit *hit) {
    switch (collision_retestCachedCeiling(pos, top, hit)) {
    case 0:
        return collision_probeCeilingSet0(pos, top, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Ceiling probe against collision set 3. */
s32 collision_probeCeilingSet3(Vec3f *pos, f32 top, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (top < pos->y) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = 99999.8984375f;
    best = -1;
    collision_selectMap(3);
    count = collision_gatherActorGroundTris(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (pos->y <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        u32 j;
        s32 offset;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        hit->material = 0;
        hit->flags |= 6;
        hit->source = 4;
        hit->surface = NULL;
        if (hit->height <= top) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe against collision set 3. */
s32 collision_probeFloorSet3(Vec3f *pos, f32 bottom, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = -10000.0f;
    best = -1;
    collision_selectMap(3);
    count = collision_gatherActorGroundTris(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (height <= pos->y && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        u32 j;
        s32 offset;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        hit->material = 0;
        hit->flags |= 6;
        hit->source = 4;
        hit->surface = NULL;
        if (bottom <= hit->height) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Selects the two surface sets (collision set 2) under pos for the collision_probeCeilingInSurfA-style probes. */
void collision_selectSurfaceSets(Vec3f *pos, u16 mask, s32 *setA, s32 *setB) {
    collision_selectMap(2);
    *setA = collision_findPropsAt(pos->x, pos->z, setB, mask);
}


/* Floor probe against surface set B only. */
s32 collision_probeFloorSurfB(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    s32 setA;
    s32 setB;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    collision_selectSurfaceSets(pos, mask, &setA, &setB);
    return collision_probeFloorInSurfB(pos, bottom, &setB, hit);
}


/* Floor probe: re-tests the cached triangle first, then collision_probeFloorSurfB. */
s32 collision_probeFloorSurfBCached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (collision_retestCachedFloor(pos, bottom, hit)) {
    case 0:
        return collision_probeFloorSurfB(pos, mask, bottom, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Ceiling probe against surface set A. */
s32 collision_probeCeilingInSurfA(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    collision_transformPropTris(*set, gSurfSetA, 0);
    count = collision_gatherPropGroundTris(pos->x, pos->z, *set, gSurfSetA);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (pos->y <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        u32 surface;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->owner;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - gCurMapTris) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: this alias and re-reading candidate->owner make IDO reload both

            if ((db_Object_array[surface = candidate->owner].viewFlags & 0x80) == 0x80) {
                alias = hit;
                alias->flags |= 1;
            }
        }
        hit->source = 2;
        if (hit->height <= top) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe against surface set A. */
s32 collision_probeFloorInSurfA(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    collision_transformPropTris(*set, gSurfSetA, 0);
    count = collision_gatherPropGroundTris(pos->x, pos->z, *set, gSurfSetA);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (height <= pos->y && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        u32 surface;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->owner;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - gCurMapTris) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: this alias and re-reading candidate->owner make IDO reload both

            if ((db_Object_array[surface = candidate->owner].viewFlags & 0x80) == 0x80) {
                alias = hit;
                alias->flags |= 1;
            }
        }
        hit->source = 2;
        if (bottom <= hit->height) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Ceiling probe against surface set B. */
s32 collision_probeCeilingInSurfB(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    collision_transformPropTris(*set, gSurfSetB, 0);
    count = collision_gatherPropGroundTris(pos->x, pos->z, *set, gSurfSetB);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (pos->y <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        s32 surface;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->owner;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - gCurMapTris) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        if ((hit->surface->viewFlags & 0x80) == 0x80) {
            hit->flags |= 1;
        }
        hit->source = 3;
        if (hit->height <= top) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe against surface set B. */
s32 collision_probeFloorInSurfB(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    collision_transformPropTris(*set, gSurfSetB, 0);
    count = collision_gatherPropGroundTris(pos->x, pos->z, *set, gSurfSetB);
    for (i = 0; i < count; i++) {
        height = gGroundCandidates[i].height * 0.00390625f;
        if (height <= pos->y && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        GroundCandidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        s32 surface;

        candidate = &gGroundCandidates[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->owner;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - gCurMapTris) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        if ((hit->surface->viewFlags & 0x80) == 0x80) {
            hit->flags |= 1;
        }
        hit->source = 3;
        if (bottom <= hit->height) {
            hit->flags |= 2;
            return TRUE;
        }
        return FALSE;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Ceiling probe: the nearer of surface sets A and B. */
s32 collision_probeCeilingSurfAB(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
    s32 setA;
    s32 setB;
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (top < pos->y) {
        hit->flags &= ~2;
        return FALSE;
    }
    hitA = *hit;
    hitB = *hit;
    collision_selectSurfaceSets(pos, mask, &setA, &setB);
    foundA = collision_probeCeilingInSurfA(pos, top, &setA, &hitA);
    foundB = collision_probeCeilingInSurfB(pos, top, &setB, &hitB);
    if (foundA && foundB) {
        if (hitA.height < hitB.height) {
            *hit = hitA;
        } else {
            *hit = hitB;
        }
        return TRUE;
    }
    if (foundA) {
        *hit = hitA;
        return TRUE;
    }
    if (foundB) {
        *hit = hitB;
        return TRUE;
    }
    if (hitA.height < hitB.height) {
        *hit = hitA;
    } else {
        *hit = hitB;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe: the nearer of surface sets A and B. */
s32 collision_probeFloorSurfAB(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    s32 setA;
    s32 setB;
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->y < bottom) {
        hit->source = 0;
        hit->surface = NULL;
        hit->flags &= ~2;
        return FALSE;
    }
    hitA = *hit;
    hitB = *hit;
    collision_selectSurfaceSets(pos, mask, &setA, &setB);
    foundA = collision_probeFloorInSurfA(pos, bottom, &setA, &hitA);
    foundB = collision_probeFloorInSurfB(pos, bottom, &setB, &hitB);
    if (foundA && foundB) {
        if (hitB.height < hitA.height) {
            *hit = hitA;
            return TRUE;
        }
        *hit = hitB;
        return TRUE;
    }
    if (foundA) {
        *hit = hitA;
        return TRUE;
    }
    if (foundB) {
        *hit = hitB;
        return TRUE;
    }
    if (hitB.height < hitA.height) {
        *hit = hitA;
    } else {
        *hit = hitB;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Ceiling probe: the nearer of surface sets A/B and collision set 0. */
s32 collision_probeCeilingSurfABSet0(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (top < pos->y) {
        hit->flags &= ~2;
        return FALSE;
    }

    hitA = *hit;
    hitB = *hit;
    foundA = collision_probeCeilingSurfAB(pos, mask, top, &hitA);
    foundB = collision_probeCeilingSet0(pos, top, &hitB);

    if (foundA && foundB) {
        if (hitA.height < hitB.height) {
            *hit = hitA;
        } else {
            *hit = hitB;
        }
        return TRUE;
    }

    if (foundA) {
        *hit = hitA;
        return TRUE;
    }

    if (foundB) {
        *hit = hitB;
        return TRUE;
    }

    if (hitA.height < hitB.height) {
        *hit = hitA;
    } else {
        *hit = hitB;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe: the nearer of surface sets A/B and collision set 0. */
s32 collision_probeFloorSurfABSet0(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }

    hitA = *hit;
    hitB = *hit;
    foundA = collision_probeFloorSurfAB(pos, mask, bottom, &hitA);
    foundB = collision_probeFloorSet0(pos, bottom, &hitB);

    if (foundA && foundB) {
        if (hitB.height < hitA.height) {
            *hit = hitA;
        } else {
            *hit = hitB;
        }
        return TRUE;
    }

    if (foundA) {
        *hit = hitA;
        return TRUE;
    }

    if (foundB) {
        *hit = hitB;
        return TRUE;
    }

    if (hitB.height < hitA.height) {
        *hit = hitA;
    } else {
        *hit = hitB;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Ceiling probe with the cached-triangle fast path (collision_probeCeilingSurfABSet0). */
s32 collision_probeCeilingSurfABSet0Cached(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
    switch (collision_retestCachedCeiling(pos, top, hit)) {
    case 0:
        return collision_probeCeilingSurfABSet0(pos, mask, top, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Floor probe with the cached-triangle fast path (collision_probeFloorSurfABSet0). */
s32 collision_probeFloorSurfABSet0Cached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (collision_retestCachedFloor(pos, bottom, hit)) {
    case 0:
        return collision_probeFloorSurfABSet0(pos, mask, bottom, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Floor probe: collision_probeFloorSurfABSet0 or collision set 3, whichever is nearer. */
s32 collision_probeFloorWithSet3(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }

    hitA = *hit;
    hitB = *hit;
    foundA = collision_probeFloorSurfABSet0(pos, mask, bottom, &hitA);
    foundB = collision_probeFloorSet3(pos, bottom, &hitB);

    if (foundA && foundB) {
        if (hitB.height < hitA.height) {
            *hit = hitA;
        } else {
            *hit = hitB;
        }
        return TRUE;
    }

    if (foundA) {
        *hit = hitA;
        return TRUE;
    }

    if (foundB) {
        *hit = hitB;
        return TRUE;
    }

    if (hitB.height < hitA.height) {
        *hit = hitA;
    } else {
        *hit = hitB;
    }
    hit->flags &= ~2;
    return FALSE;
}

/* Floor probe with the cached-triangle fast path (collision_probeFloorWithSet3). */
s32 collision_probeFloorWithSet3Cached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (collision_retestCachedFloor(pos, bottom, hit)) {
    case 0:
        return collision_probeFloorWithSet3(pos, mask, bottom, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/*
 * Fast path for floor probes: if hit->tri is still valid and pos lies over it, re-measure its height.
 * Returns 0 when not over the cached triangle, 1 when over it but outside [bottom, pos.y], 2 on a hit.
 */
s32 collision_retestCachedFloor(Vec3f *pos, f32 bottom, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (collision_getTriHeightAt(pos->x, pos->z, 0, hit->tri.v, &height)) {
            if (bottom <= height) {
                if (height <= pos->y) {
                    hit->height = height;
                    hit->flags |= 2;
                    return 2;
                }
            }
            return 1;
        }
    }
    return 0;
}

/* Ceiling counterpart of collision_retestCachedFloor: a hit lies in [pos.y, top]. */
s32 collision_retestCachedCeiling(Vec3f *pos, f32 top, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (collision_getTriHeightAt(pos->x, pos->z, 0, hit->tri.v, &height)) {
            if (height <= top) {
                if (pos->y <= height) {
                    hit->height = height;
                    hit->flags |= 2;
                    return 2;
                }
            }
            return 1;
        }
    }
    return 0;
}

/* Floor state an object keeps (overlays Actor, whose header copy lacks these fields). */
/* Seeds a CollisionHit from the floor an object last stood on (a flat triangle if flag 0x200000 is set). */
void collision_initHitFromObject(CollisionHit *hit, Actor *obj) {
    s32 surface;
    s32 flag;

    hit->height = obj->floorY;
    if (obj->noshove & 0x200000) {
        s16 x;
        s16 y;
        s16 z;

        x = obj->xpos;
        y = obj->floorY;
        z = obj->zpos;
        hit->tri.v[0].x = x + 1000;
        hit->tri.v[0].y = y;
        hit->tri.v[0].z = z + 1000;
        hit->tri.v[1].x = x - 1000;
        hit->tri.v[1].y = y;
        hit->tri.v[1].z = z;
        hit->tri.v[2].x = x + 1000;
        hit->tri.v[2].y = y;
        hit->tri.v[2].z = z - 1000;
    } else {
        hit->tri = obj->floorTri;
    }
    hit->material = obj->floorMaterial;
    hit->flags = 6;
    if (obj->propNum != 0) {
        surface = obj->propNum - 1;
        hit->source = 2;
        hit->surface = &db_Object_array[surface];
        if (math_isObjectHidden(surface) != 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        hit->flags |= flag;
    lab: ; // matching: an empty labelled statement keeps the flags store out of the branch delay slot
    } else {
        hit->source = 1;
        hit->surface = NULL;
        hit->flags |= 1;
    }
}

/* Fills a CollisionHit from a ray hit; a level-mesh hit (hitType 1) is valid (flags 7, source 1). */
void collision_initHitFromRay(CollisionHit *hit, RayHit *src) {
    s32 one;
    s32 inRange;
    s32 flag1;
    s32 valid;
    s32 hasTri;

    one = 1; // matching: keeps the constant 1 in a register for every compare
    hit->height = src->pos.y;
    hit->tri = src->tri;
    hit->material = src->material;
    valid = src->hitType;
    inRange = (1 == (valid ^ 0)) ? 2 : 0; // matching: the ^ 0 keeps IDO from swapping the compare operands
    flag1 = (one == valid) ? one : 0;
    hasTri = (one == valid) ? 4 : 0;
    hit->flags = hasTri | flag1 | inRange;
    if (src->hitType == one) {
        hit->source = 1;
    } else {
        hit->source = 0;
    }
    hit->surface = src->surface;
}

/* guLookAtF: view matrix from eye, target and up vector. Rare's copy: a zero-length vector gets length
 * 0.001 instead of a divide by zero, and sqrtf is called out of line. */
void guLookAtF(MtxF mf, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 scale;
    f32 xRight;
    f32 yRight;
    f32 zRight;
    f32 xLook;
    f32 yLook;
    f32 zLook;
    f32 len;

    guMtxIdentF(mf);

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    len = sqrtf((xLook * xLook) + (yLook * yLook) + (zLook * zLook));
    if (len == 0.0f) {
        len = 0.001f;
    }
    scale = -1.0f / len;
    xLook *= scale;
    yLook *= scale;
    zLook *= scale;

    xRight = (yUp * zLook) - (zUp * yLook);
    yRight = (zUp * xLook) - (xUp * zLook);
    zRight = (xUp * yLook) - (yUp * xLook);
    len = sqrtf((xRight * xRight) + (yRight * yRight) + (zRight * zRight));
    if (len == 0.0f) {
        len = 0.001f;
    }
    scale = 1.0f / len;
    xRight *= scale;
    yRight *= scale;
    zRight *= scale;

    xUp = (yLook * zRight) - (zLook * yRight);
    yUp = (zLook * xRight) - (xLook * zRight);
    zUp = (xLook * yRight) - (yLook * xRight);
    len = sqrtf((xUp * xUp) + (yUp * yUp) + (zUp * zUp));
    if (len == 0.0f) {
        len = 0.001f;
    }
    scale = 1.0f / len;
    xUp *= scale;
    yUp *= scale;
    zUp *= scale;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -((xEye * xRight) + (yEye * yRight) + (zEye * zRight));
    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -((xEye * xUp) + (yEye * yUp) + (zEye * zUp));
    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -((xEye * xLook) + (yEye * yLook) + (zEye * zLook));
    mf[3][3] = 1.0f;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
}

/* guLookAt: guLookAtF converted to a fixed-point Mtx. */
void guLookAt(Mtx *m, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    MtxF mf;

    guLookAtF(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}

/* From here on, sqrtf is inline (sqrt.s) again. */
#pragma intrinsic(sqrtf)

/* guLookAtReflectF: view matrix plus the reflection LookAt for highlights. Rare's copy replaces a
 * zero-length look, right or up vector with a fixed axis. */
void guLookAtReflectF(MtxF mf, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 len;
    f32 xLook;
    f32 yLook;
    f32 zLook;
    f32 xRight;
    f32 yRight;
    f32 zRight;

    guMtxIdentF(mf);

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    if (xLook == 0.0f && yLook == 0.0f && zLook == 0.0f) {
        zLook = 1.0f;
        xLook = 0.0f;
        yLook = 0.0f;
    }

    len = -1.0f / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = xRight * xRight + yRight * yRight + zRight * zRight;
    if (len != 0.0f) {
        len = 1.0f / sqrtf(len);
        xRight *= len;
        yRight *= len;
        zRight *= len;
    } else {
        xRight = 1.0f;
        yRight = 0.0f;
        zRight = 0.0f;
    }

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = xUp * xUp + yUp * yUp + zUp * zUp;
    if (len != 0.0f) {
        len = 1.0f / sqrtf(len);
        xUp *= len;
        yUp *= len;
        zUp *= len;
    } else {
        xUp = 0.0f;
        yUp = 1.0f;
        zUp = 0.0f;
    }

    l->l[0].l.dir[0] = FTOFRAC8(xRight);
    l->l[0].l.dir[1] = FTOFRAC8(yRight);
    l->l[0].l.dir[2] = FTOFRAC8(zRight);
    l->l[1].l.dir[0] = FTOFRAC8(xUp);
    l->l[1].l.dir[1] = FTOFRAC8(yUp);
    l->l[1].l.dir[2] = FTOFRAC8(zUp);

    l->l[0].l.col[0] = 0;
    l->l[0].l.col[1] = 0;
    l->l[0].l.col[2] = 0;
    l->l[0].l.pad1 = 0;
    l->l[0].l.colc[0] = 0;
    l->l[0].l.colc[1] = 0;
    l->l[0].l.colc[2] = 0;
    l->l[0].l.pad2 = 0;
    l->l[1].l.col[0] = 0;
    l->l[1].l.col[1] = 0x80;
    l->l[1].l.col[2] = 0;
    l->l[1].l.pad1 = 0;
    l->l[1].l.colc[0] = 0;
    l->l[1].l.colc[1] = 0x80;
    l->l[1].l.colc[2] = 0;
    l->l[1].l.pad2 = 0;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);

    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);

    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);

    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
}

/* guLookAtReflect: guLookAtReflectF converted to a fixed-point Mtx. */
void guLookAtReflect(Mtx *m, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp) {
    MtxF mf;

    guLookAtReflectF(mf, l, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}
