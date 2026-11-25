#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#define D_800CBE00 D_800CBE00_s32
#define D_800D3300 D_800D3300_hdr
#define D_800DBE3C D_800DBE3C_hdr
#define D_800DBE5C D_800DBE5C_hdr
#define D_800DBEF4 D_800DBEF4_hdr
#include "variables.h"
#undef D_800CBE00
#undef D_800D3300
#undef D_800DBE3C
#undef D_800DBE5C
#undef D_800DBEF4


/* A timed node in the D_800CBE00 list, allocated by func_15044964. */
typedef struct TriggerNode {
    /* 0x00 */ struct TriggerNode *next;
    /* 0x04 */ s16 lifetime; // frames left; -1 = never expires
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8 check;     // index into D_80085E80
    /* 0x0D */ u8 action;    // index into D_80085E8C, run when check passes
    /* 0x0E */ u8 delay;     // frames before check starts running
    /* 0x0F */ u8 padF;
} TriggerNode;

/* A TriggerNode with a box volume (func_150448D0). */
typedef struct TriggerVolume {
    /* 0x00 */ TriggerNode node;
    /* 0x10 */ s16 depth;
    /* 0x12 */ s16 width;
    /* 0x14 */ s16 height;
    /* 0x16 */ u8 angle;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 *position;
    /* 0x1C */ s16 *size;
} TriggerVolume;

extern TriggerNode *D_800CBE00;
extern s32 (*D_80085E80[])(TriggerNode *);
extern void (*D_80085E8C[])(void);
extern f32 D_80098D60;
extern f32 D_80098D64;
extern f32 D_80098D68;
extern f32 sqrtf(f32);
extern s32 D_800CBD9C;
extern s32 func_15145C90(s32);
extern void *allocate_memory(s32, s32, s32, s32);
TriggerNode *func_15044964(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z);

#pragma function sqrtf

void func_15044370(void) {
    D_800CBD9C = 0;
}

extern u8 D_80089120[4];
extern u8 D_800CBDD3;
extern f32 D_800CBDF4;
extern f32 D_800CBDF8;
void func_1510F800(s32);
void func_15044660(struct127 *, f32, f32, f32);
s32 func_150AB1F0(f32, f32, f32, struct127 *, s32);
void func_150AC3E4(f32, f32, f32, struct127 *, s32);

s32 func_15044380(f32 x, f32 y, f32 z, struct127 *obj, s32 arg4, s32 arg5) {
    s32 i;
    s32 total;
    s32 saved_mode;

    D_800CBDF4 = -32768.0f;
    D_800CBDF8 = -32768.0f;
    total = 0;
    obj->pad275 = 0;
    func_15044660(obj, x, y, z);
    saved_mode = D_800CBDD3;
    for (i = 3; i >= 0; i--) {
        if (D_80089120[i] == 1 && (i != 3 || !(obj->unkF8 & 0x200))) {
            func_1510F800(i);
            if (D_800DBE62 == 0) continue;
            total += func_150AB1F0(x, y, z, obj, arg4);
        }
    }
    if (arg5 != 0) {
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
void func_1504452C(s16 **vertices, struct17 *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset) {
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
        out[i].unk0 = (x * cosY) + (z * sinY);
        out[i].unk4 = y;
        out[i].unk8 = (z * cosY) - (x * sinY);
    }
}

void func_15044658(void) {
}

extern f32 D_800CBDD8;
extern f32 D_800CBDDC;
void func_1507C3E0(struct127 *obj, s16 *height, s16 *arg2, s16 *arg3);

/* Sets D_800CBDD8/D_800CBDDC (a height and a mid-height) for obj's collision probe. */
void func_15044660(struct127 *obj, f32 x, f32 y, f32 z) {
    s16 height;
    s16 unk2C;
    s16 unk2A;
    s32 half;
    s32 index;

    func_1507C3E0(obj, &height, &unk2C, &unk2A);
    if (obj->interaction_state == 0x2D || obj->interaction_state == 0x2E || obj->interaction_state == 0x2C) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
    } else {
        height += fabsf(y - obj->y_position);
        index = obj - D_800CC2D0;
    }
    if (obj->unk5 == 5) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
        return;
    }
    if (obj->in_water != 0) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x53) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x28) {
        half = 20;
        D_800CBDD8 = height - half;
        D_800CBDDC = half + D_800CBDD8 * 0.5f;
        return;
    }
    if (index >= 0 && index < D_8008FD8C && obj->unk28 != 0.0f) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x25) {
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

extern f32 func_15048A40(u8); // sine of a 256-step angle (header declares it void)
extern f32 func_150489B0(u8); // cosine of a 256-step angle

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

    playerHalfHeight = (s16)D_800CC2D0[0].unk31C->unk114 >> 1;
    playerWidth = (s16)D_800CC2D0[0].unk31C->unk116;
    playerDepth = (s16)D_800CC2D0[0].unk31C->unk118;
    dx = D_800CC2D0[0].x_position - volume->node.x;
    dy = (D_800CC2D0[0].y_position - volume->node.y) + playerHalfHeight;
    dz = D_800CC2D0[0].z_position - volume->node.z;
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

extern s32 func_1505D1C4(f32, f32, f32, s32, s32, u16, s32, s32);

/* Check 2: calls func_1505D1C4 at the node's position with its depth; never fires. */
s32 func_15044D40(TriggerVolume *volume) {
    func_1505D1C4(volume->node.x, volume->node.y, volume->node.z, volume->depth, 0xFF, 0, 0, 0);
    return FALSE;
}

s32 func_1505D024(struct127 *obj, s32 type, u16 angle, s32 arg3);

/* Trigger actions 1-4: hit the player with a func_1505D024 type unless stunned or immune. */

void func_15044DA0(void) {
    if (D_800CC2D0[0].stunned || D_800CC2D0[0].immune) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 5, D_800CC2D0[0].unk7A, -1);
}

void func_15044DE8(void) {
    if (D_800CC2D0[0].stunned || D_800CC2D0[0].immune) {
        return;
    }
    if (D_800C35EA != 1) {
        func_1505D024(&D_800CC2D0[0], 4, D_800CC2D0[0].unk7A, -1);
    }
}

void func_15044E40(void) {
    if (D_800CC2D0[0].stunned || D_800CC2D0[0].immune) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 0x40, D_800CC2D0[0].unk7A, -1);
}

void func_15044E88(void) {
    if (D_800CC2D0[0].stunned || D_800CC2D0[0].immune) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 1, D_800CC2D0[0].unk7A, -1);
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Vec3s_71820;

typedef struct {
    Vec3s_71820 v[3];
} CollisionTri;

/* Surface record in the D_800DBEF4 array (same 0xA0 layout as struct131, whose header copy lacks 0x40/0x44). */
typedef struct Game71820Surface {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ s32 default_material;
    /* 0x44 */ s32 *materials;     // per-triangle, indexed from first_triangle
    /* 0x48 */ u8 pad48[0x10];
    /* 0x58 */ u16 first_triangle;
    /* 0x5A */ u8 pad5A[0x15];
    /* 0x6F */ u8 flags;
    /* 0x70 */ u8 pad70[0x30];
} Game71820Surface;

/* Result of a floor/ceiling probe. */
typedef struct CollisionHit {
    /* 0x00 */ f32 height;
    /* 0x04 */ CollisionTri tri;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 material;
    /* 0x1C */ u8 flags;           // 2: height within the probe's limit, 4: tri is valid (fast path re-tests it)
    /* 0x1D */ u8 source;          // which collision set produced the hit (1-4)
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ Game71820Surface *surface;
} CollisionHit;

/* One candidate triangle returned by the func_150A3A70/func_150A4FA0/func_150A43E0 grid queries. */
typedef struct Game71820Candidate {
    /* 0x00 */ s32 height;         // 24.8 fixed point
    /* 0x04 */ s32 *vertices;      // entry in the D_800DBE3C triangle table: three vertex-array bases
    /* 0x08 */ s32 index;          // vertex index (or byte offset) into each base
    /* 0x0C */ s32 surface;        // index into D_800DBEF4
} Game71820Candidate;

void func_1510F800(s32);
s32 func_150A3A70(s32, s32);
s32 func_150A4FA0(s32, s32);
s32 func_150A43E0(s32, s32, s32, void *);
void func_150A44F0(s32, void *, s32);
s32 func_150A6500(s16, s16, s32 *, u16);
s32 func_150A3FC4(f32, f32, s32, CollisionTri *, f32 *);
extern u8 *D_800DBE3C;
extern s32 *D_800DBE5C;
extern Game71820Candidate D_800D3300[];
extern Game71820Surface *D_800DBEF4;
extern u8 D_800D37E0[];
extern u8 D_800D3830[];
extern f32 D_800DBE68;
extern f32 D_800DBE6C;
extern f32 D_800DBE70;
extern f32 D_800DBE74;

/* Ceiling probe against collision set 0: lowest triangle between pos.y and top. */
s32 func_15044ED0(struct17 *pos, f32 top, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (top < pos->unk4) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = 99999.8984375f;
    best = -1;
    func_1510F800(0);
    count = func_150A3A70(pos->unk0, pos->unk8);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (pos->unk4 <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        u32 j;
        s32 index;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        index = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + index * 16);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        {
            s32 triangle = ((u8 *)candidate->vertices - D_800DBE3C) / 12;
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
s32 func_150450CC(struct17 *pos, f32 bottom, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (pos->unk4 < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = -10000.0f;
    best = -1;
    func_1510F800(0);
    D_800DBE68 = pos->unk0;
    D_800DBE6C = pos->unk4;
    D_800DBE70 = pos->unk8;
    D_800DBE74 = bottom;
    count = func_150A3A70(pos->unk0, pos->unk8);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (height <= pos->unk4 && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        u32 j;
        s32 index;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        index = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + index * 16);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        if (D_800DBE5C != NULL) {
            s32 triangle = ((u8 *)candidate->vertices - D_800DBE3C) / 12;
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

s32 func_150470B0(struct17 *pos, f32 top, CollisionHit *hit);

/* Ceiling probe: re-tests the cached triangle first, then searches collision set 0. */
s32 func_1504530C(struct17 *pos, f32 top, CollisionHit *hit) {
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
s32 func_15045384(struct17 *pos, f32 top, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (top < pos->unk4) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = 99999.8984375f;
    best = -1;
    func_1510F800(3);
    count = func_150A4FA0(pos->unk0, pos->unk8);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (pos->unk4 <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        u32 j;
        s32 offset;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
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
s32 func_1504554C(struct17 *pos, f32 bottom, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    if (pos->unk4 < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    hit->height = -10000.0f;
    best = -1;
    func_1510F800(3);
    count = func_150A4FA0(pos->unk0, pos->unk8);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (height <= pos->unk4 && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        u32 j;
        s32 offset;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
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
void func_15045714(struct17 *pos, u16 mask, s32 *setA, s32 *setB) {
    func_1510F800(2);
    *setA = func_150A6500(pos->unk0, pos->unk8, setB, mask);
}

s32 func_15045F8C(struct17 *pos, f32 bottom, s32 *set, CollisionHit *hit);

/* Floor probe against surface set B only. */
s32 func_15045780(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    s32 setA;
    s32 setB;

    if (pos->unk4 < bottom) {
        hit->flags &= ~2;
        return FALSE;
    }
    func_15045714(pos, mask, &setA, &setB);
    return func_15045F8C(pos, bottom, &setB, hit);
}

s32 func_15047004(struct17 *pos, f32 bottom, CollisionHit *hit);

/* Floor probe: re-tests the cached triangle first, then func_15045780. */
s32 func_15045800(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
s32 func_15045880(struct17 *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    func_150A44F0(*set, D_800D37E0, 0);
    count = func_150A43E0(pos->unk0, pos->unk8, *set, D_800D37E0);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (pos->unk4 <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        u32 surface;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->surface;
        hit->surface = &D_800DBEF4[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((u8 *)vertices - D_800DBE3C) / 12 - hit->surface->first_triangle;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->default_material;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: a scoped alias makes IDO reload flags here

            if ((D_800DBEF4[surface = candidate->surface].flags & 0x80) == 0x80) {
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
s32 func_15045AE4(struct17 *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    func_150A44F0(*set, D_800D37E0, 0);
    count = func_150A43E0(pos->unk0, pos->unk8, *set, D_800D37E0);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (height <= pos->unk4 && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        u32 surface;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->surface;
        hit->surface = &D_800DBEF4[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((u8 *)vertices - D_800DBE3C) / 12 - hit->surface->first_triangle;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->default_material;
        }
        hit->flags |= 6;
        {
            CollisionHit *alias; // matching: a scoped alias makes IDO reload flags here

            if ((D_800DBEF4[surface = candidate->surface].flags & 0x80) == 0x80) {
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
s32 func_15045D48(struct17 *pos, f32 top, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = 99999.8984375f;
    best = -1;
    func_150A44F0(*set, D_800D3830, 0);
    count = func_150A43E0(pos->unk0, pos->unk8, *set, D_800D3830);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (pos->unk4 <= height && height < hit->height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        s32 surface;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->surface;
        hit->surface = &D_800DBEF4[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((u8 *)vertices - D_800DBE3C) / 12 - hit->surface->first_triangle;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->default_material;
        }
        hit->flags |= 6;
        if ((hit->surface->flags & 0x80) == 0x80) {
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
s32 func_15045F8C(struct17 *pos, f32 bottom, s32 *set, CollisionHit *hit) {
    s32 count;
    s32 best;
    s32 i;
    f32 height;

    hit->height = -10000.0f;
    best = -1;
    func_150A44F0(*set, D_800D3830, 0);
    count = func_150A43E0(pos->unk0, pos->unk8, *set, D_800D3830);
    for (i = 0; i < count; i++) {
        height = D_800D3300[i].height * 0.00390625f;
        if (height <= pos->unk4 && hit->height < height) {
            best = i;
            hit->height = height;
        }
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *vertices;
        s32 j;
        s32 offset;
        s32 *materials;
        s32 surface;

        candidate = &D_800D3300[best];
        vertices = candidate->vertices;
        offset = candidate->index;
        for (j = 0; j != 3; j++) {
            s16 *point = (s16 *)(vertices[j] + offset);
            hit->tri.v[j].x = point[0];
            hit->tri.v[j].y = point[1];
            hit->tri.v[j].z = point[2];
        }
        surface = candidate->surface;
        hit->surface = &D_800DBEF4[surface];
        materials = hit->surface->materials;
        if (materials != NULL) {
            s32 triangle = ((u8 *)vertices - D_800DBE3C) / 12 - hit->surface->first_triangle;
            hit->material = materials[triangle];
        } else {
            hit->material = hit->surface->default_material;
        }
        hit->flags |= 6;
        if ((hit->surface->flags & 0x80) == 0x80) {
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
s32 func_150461D0(struct17 *pos, u16 mask, f32 top, CollisionHit *hit) {
    s32 setA;
    s32 setB;
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (top < pos->unk4) {
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
s32 func_15046460(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    s32 setA;
    s32 setB;
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->unk4 < bottom) {
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
s32 func_150466F8(struct17 *pos, u16 mask, f32 top, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (top < pos->unk4) {
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
s32 func_1504697C(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->unk4 < bottom) {
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
s32 func_15046C00(struct17 *pos, u16 mask, f32 top, CollisionHit *hit) {
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
s32 func_15046C80(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
s32 func_15046D00(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
    CollisionHit hitA;
    CollisionHit hitB;
    u8 foundA;
    u8 foundB;

    if (pos->unk4 < bottom) {
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
s32 func_15046F84(struct17 *pos, u16 mask, f32 bottom, CollisionHit *hit) {
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
s32 func_15047004(struct17 *pos, f32 bottom, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (func_150A3FC4(pos->unk0, pos->unk8, 0, &hit->tri, &height)) {
            if (bottom <= height) {
                if (height <= pos->unk4) {
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
s32 func_150470B0(struct17 *pos, f32 top, CollisionHit *hit) {
    f32 height;

    if (hit->flags & 4) {
        if (func_150A3FC4(pos->unk0, pos->unk8, 0, &hit->tri, &height)) {
            if (height <= top) {
                if (pos->unk4 <= height) {
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

/* Floor state an object keeps (overlays struct127, whose header copy lacks these fields). */
typedef struct ObjFloorState {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ f32 x;
    /* 0x018 */ f32 y;
    /* 0x01C */ f32 z;
    /* 0x020 */ u8 pad20[0xD8];
    /* 0x0F8 */ u32 flags;
    /* 0x0FC */ u8 padFC[0x84];
    /* 0x180 */ f32 floor_height;
    /* 0x184 */ s32 floor_material;
    /* 0x188 */ u8 pad188[4];
    /* 0x18C */ CollisionTri floor_tri;
    /* 0x19E */ u8 pad19E[2];
    /* 0x1A0 */ u16 floor_surface; // 1-based index into D_800DBEF4; 0 = none
} ObjFloorState;

/* Seeds a CollisionHit from the floor an object last stood on (a flat triangle if flag 0x200000 is set). */
void func_1504715C(CollisionHit *hit, ObjFloorState *obj) {
    s32 surface;
    s32 flag;

    hit->height = obj->floor_height;
    if (obj->flags & 0x200000) {
        s16 x;
        s16 y;
        s16 z;

        x = obj->x;
        y = obj->floor_height;
        z = obj->z;
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
        hit->tri = obj->floor_tri;
    }
    hit->material = obj->floor_material;
    hit->flags = 6;
    if (obj->floor_surface != 0) {
        surface = obj->floor_surface - 1;
        hit->source = 2;
        hit->surface = &D_800DBEF4[surface];
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

/* Source of func_150472C0 (layout only as far as used). */
typedef struct Game71820FloorSrc {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 height;
    /* 0x10 */ u8 pad10[0x34];
    /* 0x44 */ CollisionTri tri;
    /* 0x56 */ u8 pad56[3];
    /* 0x59 */ u8 unk59;
    /* 0x5A */ u8 pad5A[2];
    /* 0x5C */ Game71820Surface *surface;
    /* 0x60 */ s32 material;
} Game71820FloorSrc;

/* Fills a CollisionHit from a Game71820FloorSrc; unk59 == 1 marks it valid (flags 7, source 1). */
void func_150472C0(CollisionHit *hit, Game71820FloorSrc *src) {
    s32 one;
    s32 valid;
    s32 inRange;
    s32 flag1;
    u8 source1;

    one = 1;
    hit->height = src->height;
    hit->tri = (0, src->tri); // matching: comma expression steers the struct copy's scheduling
    hit->material = src->material;
    valid = src->unk59;
    inRange = (1 == (valid ^ 0)) ? 2 : 0;
    source1 = 1;
    flag1 = (one == valid) ? one : 0;
    valid = (one == valid) ? 4 : 0;
    hit->flags = (valid | flag1) | inRange;
    if (src->unk59 == one) {
        hit->source = source1;
    } else {
        hit->source = 0;
    }
    hit->surface = src->surface;
}


void func_15047390(f32 mf[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 temp_f0;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f30;
    f32 temp_f18;

    guMtxIdentF(mf);

    temp_f26 = arg4 - arg1;
    temp_f28 = arg5 - arg2;
    temp_f30 = arg6 - arg3;
    temp_f18 = sqrtf((temp_f26 * temp_f26) + (temp_f28 * temp_f28) + (temp_f30 * temp_f30));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D60;
    }
    temp_f0 = -1.0f / temp_f18;
    temp_f26 *= temp_f0;
    temp_f28 *= temp_f0;
    temp_f30 *= temp_f0;

    temp_f20 = (arg8 * temp_f30) - (arg9 * temp_f28);
    temp_f22 = (arg9 * temp_f26) - (arg7 * temp_f30);
    temp_f24 = (arg7 * temp_f28) - (arg8 * temp_f26);
    temp_f18 = sqrtf((temp_f20 * temp_f20) + (temp_f22 * temp_f22) + (temp_f24 * temp_f24));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D64;
    }
    temp_f0 = 1.0f / temp_f18;
    temp_f20 *= temp_f0;
    temp_f22 *= temp_f0;
    temp_f24 *= temp_f0;

    arg7 = (temp_f28 * temp_f24) - (temp_f30 * temp_f22);
    arg8 = (temp_f30 * temp_f20) - (temp_f26 * temp_f24);
    arg9 = (temp_f26 * temp_f22) - (temp_f28 * temp_f20);
    temp_f18 = sqrtf((arg7 * arg7) + (arg8 * arg8) + (arg9 * arg9));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D68;
    }
    temp_f0 = 1.0f / temp_f18;
    arg7 *= temp_f0;
    arg8 *= temp_f0;
    arg9 *= temp_f0;

    mf[0][0] = temp_f20;
    mf[1][0] = temp_f22;
    mf[2][0] = temp_f24;
    mf[3][0] = -((arg1 * temp_f20) + (arg2 * temp_f22) + (arg3 * temp_f24));
    mf[0][1] = arg7;
    mf[1][1] = arg8;
    mf[2][1] = arg9;
    mf[3][1] = -((arg1 * arg7) + (arg2 * arg8) + (arg3 * arg9));
    mf[0][2] = temp_f26;
    mf[1][2] = temp_f28;
    mf[2][2] = temp_f30;
    mf[3][2] = -((arg1 * temp_f26) + (arg2 * temp_f28) + (arg3 * temp_f30));
    mf[3][3] = 1.0f;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
}

void func_15047688(Mtx *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 sp30[4][4];

    func_15047390(&sp30, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    guMtxF2L(&sp30, arg0);
}

#pragma intrinsic(sqrtf)

void func_15047700(f32 mf[4][4], s32 arg1, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
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

    ((LookAt *)arg1)->l[0].l.dir[0] = (s32)((xRight * 128.0f) < 127.0f ? (xRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[0].l.dir[1] = (s32)((yRight * 128.0f) < 127.0f ? (yRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[0].l.dir[2] = (s32)((zRight * 128.0f) < 127.0f ? (zRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[0] = (s32)((xUp * 128.0f) < 127.0f ? (xUp * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[1] = (s32)((yUp * 128.0f) < 127.0f ? (yUp * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[2] = (s32)((zUp * 128.0f) < 127.0f ? (zUp * 128.0f) : 127.0f) & 0xFF;

    ((LookAt *)arg1)->l[0].l.col[0] = 0;
    ((LookAt *)arg1)->l[0].l.col[1] = 0;
    ((LookAt *)arg1)->l[0].l.col[2] = 0;
    ((LookAt *)arg1)->l[0].l.pad1 = 0;
    ((LookAt *)arg1)->l[0].l.colc[0] = 0;
    ((LookAt *)arg1)->l[0].l.colc[1] = 0;
    ((LookAt *)arg1)->l[0].l.colc[2] = 0;
    ((LookAt *)arg1)->l[0].l.pad2 = 0;
    ((LookAt *)arg1)->l[1].l.col[0] = 0;
    ((LookAt *)arg1)->l[1].l.col[1] = 0x80;
    ((LookAt *)arg1)->l[1].l.col[2] = 0;
    ((LookAt *)arg1)->l[1].l.pad1 = 0;
    ((LookAt *)arg1)->l[1].l.colc[0] = 0;
    ((LookAt *)arg1)->l[1].l.colc[1] = 0x80;
    ((LookAt *)arg1)->l[1].l.colc[2] = 0;
    ((LookAt *)arg1)->l[1].l.pad2 = 0;

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

void func_15047700(f32 mf[4][4], s32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

void func_15047B80(Mtx *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10) {
    f32 sp38[4][4];

    func_15047700(&sp38, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    guMtxF2L(&sp38, arg0);
}
