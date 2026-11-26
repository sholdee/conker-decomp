#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A TriggerNode with a box volume (func_150448D0). */
struct TriggerVolume {
    /* 0x00 */ TriggerNode node;
    /* 0x10 */ s16 depth;
    /* 0x12 */ s16 width;
    /* 0x14 */ s16 height;
    /* 0x16 */ u8 angle;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 *position;
    /* 0x1C */ s16 *size;
};

extern s32 (*D_80085E80[])(TriggerNode *);
extern void (*D_80085E8C[])(void);
extern s32 D_800CBD9C;

/* Up to func_15047390, sqrtf is a real call (jal sqrtf), not the inline sqrt.s of gu.h's #pragma intrinsic. */
#pragma function sqrtf

/* Clears D_800CBD9C. */
void func_15044370(void) {
    D_800CBD9C = 0;
}

/*
 * Runs func_150AB1F0 for obj against each enabled collision set (3 down to 0; set 3 is skipped when obj
 * flag 0x200 is set) and returns the summed results. With secondPass, also runs func_150AC3E4 on sets 0-2.
 */
s32 func_15044380(f32 x, f32 y, f32 z, Actor *obj, s32 arg4, s32 secondPass) {
    s32 i;
    s32 total;
    s32 saved_mode;

    D_800CBDF4 = -32768.0f;
    D_800CBDF8 = -32768.0f;
    total = 0;
    obj->touching_char = 0;
    func_15044660(obj, x, y, z);
    saved_mode = D_800CBDD3;
    for (i = 3; i >= 0; i--) {
        if (D_80089120[i] == 1 && (i != 3 || !(obj->noshove & 0x200))) {
            func_1510F800(i);
            if (D_800DBE62 == 0) continue;
            total += func_150AB1F0(x, y, z, obj, arg4);
        }
    }
    if (secondPass) {
        for (i = 0; i < 3; i++) {
            if (D_80089120[i] == 1) {
                func_1510F800(i);
                if (D_800DBE62 != 0) {
                    func_150AC3E4(x, y, z, obj, 0);
                }
            }
        }
    }
    func_1510F800(0);
    D_800CBDD3 = saved_mode;
    return total;
}

/* Copies a triangle's three s16 vertices into out, relative to (ox, oy, oz) and rotated about Y. */
void func_1504452C(s16 **vertices, Vec3f *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset) {
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

extern f32 D_800CBDD8;
extern f32 D_800CBDDC;

/* Sets D_800CBDD8/D_800CBDDC (a height and a mid-height) for obj's collision probe. */
void func_15044660(Actor *obj, f32 x, f32 y, f32 z) {
    s16 height;
    s16 unk2C;
    s16 unk2A;
    s32 half;
    s32 index;

    func_1507C3E0(obj, &height, &unk2C, &unk2A);
    if (obj->type == 0x2D || obj->type == 0x2E || obj->type == 0x2C) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
    } else {
        height += fabsf(y - obj->ypos);
        index = obj - movement_obj;
    }
    if (obj->drawtype == 5) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
        return;
    }
    if (obj->inwater != 0) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->character_type == 0x53) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->character_type == 0x28) {
        half = 20;
        D_800CBDD8 = height - half;
        D_800CBDDC = half + D_800CBDD8 * 0.5f;
        return;
    }
    if (index >= 0 && index < D_8008FD8C && obj->shad != 0.0f) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->character_type == 0x25) {
        half = height / 2;
        D_800CBDD8 = height - half;
        D_800CBDDC = half + D_800CBDD8 * 0.5f;
        return;
    }
    D_800CBDD8 = height;
    D_800CBDDC = height >> 1;
}

/* Adds a box trigger volume whose position and size are read from the given s16 pointers. */
TriggerVolume *func_150448D0(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size) {
    TriggerVolume *volume;

    volume = (TriggerVolume *)func_15044964(sizeof(TriggerVolume), 1, lifetime, delay, action, 0, 0, 0);
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

/* Allocates a trigger node and appends it to the D_800CBE00 list. */
TriggerNode *func_15044964(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z) {
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

    if (D_800CBE00 == NULL) {
        D_800CBE00 = node;
    } else {
        for (tail = D_800CBE00; tail->next != NULL; tail = tail->next) {
        }
        tail->next = node;
    }

    return node;
}

/* Per frame: runs each node's check (and its action when the check passes), then ages and frees expired nodes. */
void func_15044A28(void) {
    TriggerNode *node;
    TriggerNode *prev;
    TriggerNode *next;
    s32 check;
    s32 countdown;
    s32 lifetime;
    s32 delay;

    prev = NULL;
    for (node = D_800CBE00; node != NULL; node = next) {
        countdown = node->delay;
        check = node->check;
        next = node->next;
        if (countdown == 0) {
            if (D_80085E80[check](node) != 0) {
                D_80085E8C[node->action]();
            }
        } else {
            delay = countdown - D_800BE9E4;
            if (delay < 0) {
                delay = 0;
            }
            node->delay = delay;
        }
        lifetime = node->lifetime;
        if (lifetime != -1) {
            lifetime -= D_800BE9E4;
            if (lifetime <= 0) {
                if (prev == NULL) {
                    D_800CBE00 = node->next;
                } else {
                    prev->next = node->next;
                }
                func_100043B4((s32 *)node, 2);
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
s32 func_15044B78(TriggerVolume *volume) {
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
    sinA = func_15048A40(volume->angle);
    cosA = func_150489B0(volume->angle);
    dzRot = dz * cosA + dx * sinA;
    dx = dx * cosA - dz * sinA;
    if (fabsf(dy) < volume->height + playerHalfHeight && fabsf(dx) < volume->width + playerWidth &&
        fabsf(dzRot) < volume->depth + playerDepth) {
        return TRUE;
    }
    return FALSE;
}

/* Check 1: refreshes the volume's position and size from its source pointers, then runs check 0. */
s32 func_15044CE4(TriggerVolume *volume) {
    s16 *position = volume->position;
    s32 extent;

    volume->node.x = position[0];
    volume->node.y = position[1];
    volume->node.z = position[2];
    extent = volume->size[0] / 32;
    volume->depth = extent;
    volume->width = extent;
    volume->height = extent;
    return func_15044B78(volume);
}

/* Check 2: calls func_1505D1C4 at the node's position with its depth; never fires. */
s32 func_15044D40(TriggerVolume *volume) {
    func_1505D1C4(volume->node.x, volume->node.y, volume->node.z, volume->depth, 0xFF, 0, 0, 0);
    return FALSE;
}

/* Trigger actions 1-4: hit the player with a func_1505D024 type unless stunned or immune. */

void func_15044DA0(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    func_1505D024(&movement_obj[0], 5, movement_obj[0].langle, -1);
}

void func_15044DE8(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    if (D_800C35EA[0] != 1) {
        func_1505D024(&movement_obj[0], 4, movement_obj[0].langle, -1);
    }
}

void func_15044E40(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    func_1505D024(&movement_obj[0], 0x40, movement_obj[0].langle, -1);
}

void func_15044E88(void) {
    if (movement_obj[0].stunned || movement_obj[0].imune) {
        return;
    }
    func_1505D024(&movement_obj[0], 1, movement_obj[0].langle, -1);
}

/* One candidate triangle returned by the func_150A3A70/func_150A4FA0/func_150A43E0 grid queries. */

extern f32 D_800DBE74;

/* Ceiling probe against collision set 0: lowest triangle between pos.y and top. */
s32 func_15044ED0(Vec3f *pos, f32 top, CollisionHit *hit) {
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
    func_1510F800(0);
    count = func_150A3A70(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        index = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            Vtx *vtx = (Vtx *)vertices[j] + index;
            hit->tri.v[j].x = vtx->v.ob[0];
            hit->tri.v[j].y = vtx->v.ob[1];
            hit->tri.v[j].z = vtx->v.ob[2];
        }
        {
            s32 triangle = (MeshTri *)candidate->tri - D_800DBE3C;
            hit->material = D_800DBE5C[triangle];
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
s32 func_150450CC(Vec3f *pos, f32 bottom, CollisionHit *hit) {
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
    func_1510F800(0);
    D_800DBE68 = pos->x;
    D_800DBE6C = pos->y;
    D_800DBE70 = pos->z;
    D_800DBE74 = bottom;
    count = func_150A3A70(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        index = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            Vtx *vtx = (Vtx *)vertices[j] + index;
            hit->tri.v[j].x = vtx->v.ob[0];
            hit->tri.v[j].y = vtx->v.ob[1];
            hit->tri.v[j].z = vtx->v.ob[2];
        }
        if (D_800DBE5C != NULL) {
            s32 triangle = (MeshTri *)candidate->tri - D_800DBE3C;
            hit->material = D_800DBE5C[triangle];
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
s32 func_1504530C(Vec3f *pos, f32 top, CollisionHit *hit) {
    switch (func_150470B0(pos, top, hit)) {
    case 0:
        return func_15044ED0(pos, top, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Ceiling probe against collision set 3. */
s32 func_15045384(Vec3f *pos, f32 top, CollisionHit *hit) {
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
    func_1510F800(3);
    count = func_150A4FA0(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
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
s32 func_1504554C(Vec3f *pos, f32 bottom, CollisionHit *hit) {
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
    func_1510F800(3);
    count = func_150A4FA0(pos->x, pos->z);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
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

/* Selects the two surface sets (collision set 2) under pos for the func_15045880-style probes. */
void func_15045714(Vec3f *pos, u16 mask, s32 *setA, s32 *setB) {
    func_1510F800(2);
    *setA = func_150A6500(pos->x, pos->z, setB, mask);
}


/* Floor probe against surface set B only. */
s32 func_15045780(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    s32 setA;
    s32 setB;

    if (pos->y < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    func_15045714(pos, mask, &setA, &setB);
    return func_15045F8C(pos, bottom, &setB, hit);
}


/* Floor probe: re-tests the cached triangle first, then func_15045780. */
s32 func_15045800(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (func_15047004(pos, bottom, hit)) {
    case 0:
        return func_15045780(pos, mask, bottom, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Ceiling probe against surface set A. */
s32 func_15045880(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    func_150A44F0(*set, D_800D37E0, 0);
    count = func_150A43E0(pos->x, pos->z, *set, D_800D37E0);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->unkC;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - D_800DBE3C) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: this alias and re-reading candidate->unkC make IDO reload both

            if ((db_Object_array[surface = candidate->unkC].viewFlags & 0x80) == 0x80) {
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
s32 func_15045AE4(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    func_150A44F0(*set, D_800D37E0, 0);
    count = func_150A43E0(pos->x, pos->z, *set, D_800D37E0);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->unkC;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - D_800DBE3C) - hit->surface->firstTri;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->floorHitAttrib;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: this alias and re-reading candidate->unkC make IDO reload both

            if ((db_Object_array[surface = candidate->unkC].viewFlags & 0x80) == 0x80) {
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
s32 func_15045D48(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    func_150A44F0(*set, D_800D3830, 0);
    count = func_150A43E0(pos->x, pos->z, *set, D_800D3830);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->unkC;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - D_800DBE3C) - hit->surface->firstTri;
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
s32 func_15045F8C(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    func_150A44F0(*set, D_800D3830, 0);
    count = func_150A43E0(pos->x, pos->z, *set, D_800D3830);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
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

        candidate = &D_800D3300[best];
        vertices = candidate->tri;
        offset = candidate->vtxBase;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->unkC;
        hit->surface = &db_Object_array[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((MeshTri *)vertices - D_800DBE3C) - hit->surface->firstTri;
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
s32 func_150461D0(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
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
    func_15045714(pos, mask, &setA, &setB);
    foundA = func_15045880(pos, top, &setA, &hitA);
    foundB = func_15045D48(pos, top, &setB, &hitB);
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
s32 func_15046460(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
    func_15045714(pos, mask, &setA, &setB);
    foundA = func_15045AE4(pos, bottom, &setA, &hitA);
    foundB = func_15045F8C(pos, bottom, &setB, &hitB);
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
s32 func_150466F8(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
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
    foundA = func_150461D0(pos, mask, top, &hitA);
    foundB = func_15044ED0(pos, top, &hitB);

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
s32 func_1504697C(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
    foundA = func_15046460(pos, mask, bottom, &hitA);
    foundB = func_150450CC(pos, bottom, &hitB);

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

/* Ceiling probe with the cached-triangle fast path (func_150466F8). */
s32 func_15046C00(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit) {
    switch (func_150470B0(pos, top, hit)) {
    case 0:
        return func_150466F8(pos, mask, top, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Floor probe with the cached-triangle fast path (func_1504697C). */
s32 func_15046C80(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (func_15047004(pos, bottom, hit)) {
    case 0:
        return func_1504697C(pos, mask, bottom, hit);
    case 1:
        return FALSE;
    case 2:
        return TRUE;
    }
}

/* Floor probe: func_1504697C or collision set 3, whichever is nearer. */
s32 func_15046D00(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
    foundA = func_1504697C(pos, mask, bottom, &hitA);
    foundB = func_1504554C(pos, bottom, &hitB);

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

/* Floor probe with the cached-triangle fast path (func_15046D00). */
s32 func_15046F84(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    switch (func_15047004(pos, bottom, hit)) {
    case 0:
        return func_15046D00(pos, mask, bottom, hit);
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
s32 func_15047004(Vec3f *pos, f32 bottom, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (func_150A3FC4(pos->x, pos->z, 0, hit->tri.v, &height)) {
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

/* Ceiling counterpart of func_15047004: a hit lies in [pos.y, top]. */
s32 func_150470B0(Vec3f *pos, f32 top, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (func_150A3FC4(pos->x, pos->z, 0, hit->tri.v, &height)) {
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
void func_1504715C(CollisionHit *hit, Actor *obj) {
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
        if (func_15145C90(surface) != 0) {
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
void func_150472C0(CollisionHit *hit, RayHit *src) {
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

/* guLookAtF equivalent: view matrix from eye, target and up vector (sqrtf called out of line). */
void func_15047390(MtxF mf, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
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

/* guLookAt equivalent: func_15047390 converted to a fixed-point Mtx. */
void func_15047688(Mtx *m, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    MtxF mf;

    func_15047390(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}

/* From here on, sqrtf is inline (sqrt.s) again. */
#pragma intrinsic(sqrtf)

/* guLookAtReflectF equivalent: view matrix plus the reflection LookAt for highlights. */
void func_15047700(MtxF mf, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
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

/* guLookAtReflect equivalent: func_15047700 converted to a fixed-point Mtx. */
void func_15047B80(Mtx *m, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp) {
    MtxF mf;

    func_15047700(mf, l, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}
