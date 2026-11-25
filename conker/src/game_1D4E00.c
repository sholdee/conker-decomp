#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_1514373C(f32, f32, f32 *, f32 *);

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    u8 r, g, b, a;
} BoltColor;

/* Spawn parameters of a lightning bolt; func_151A7950 copies them into Bolt.def. */
typedef struct {
    /* 0x00 */ s16 lifetime;         /* frames, counted down while BOLT_TIMED */
    /* 0x02 */ s16 numPoints;
    /* 0x04 */ f32 jitterDelay;      /* re-jitter after jitterDelay + rand * jitterDelayRange */
    /* 0x08 */ f32 jitterDelayRange;
    /* 0x0C */ u8 flags;             /* BOLT_TIMED | BOLT_ACTIVE */
    /* 0x10 */ f32 jitter;           /* largest sideways displacement of a midpoint */
    /* 0x14 */ BoltColor color;
    /* 0x18 */ f32 width;
    /* 0x1C */ s8 update1;           /* index into D_8008F940, -1 for none */
    /* 0x1D */ s8 update2;           /* index into D_8008F948, -1 for none */
    /* 0x20 */ Vec3f start;
    /* 0x2C */ Vec3f end;
    /* 0x38 */ Vec3f up;
    /* 0x44 */ f32 length;
    /* 0x48 */ f32 invLength;
    /* 0x4C */ u8 type;              /* index into the D_8008F94C/D_8008F958/D_8008F964 handlers */
} BoltDef; /* size 0x50 */

#define BOLT_TIMED  1
#define BOLT_ACTIVE 2

typedef struct {
    Vec3f local; /* x: sideways, y: along up, z: fraction of the way from start to end */
    Vec3f world;
} BoltPoint;

/* User data of a bolt that runs from an actor bone to a point near the actor (func_151A8624). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;           /* owner->unique_id at spawn; the bolt stops when it changes */
    /* 0x08 */ Vec3f boneOffset;
    /* 0x14 */ u8 bone;
    /* 0x18 */ Vec3f targetOffset;   /* end point, relative to the owner's position */
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 linkFlags;         /* 1: the end point has been ray-cast */
} BoltLink; /* size 0x28 */

typedef struct {
    /* 0x00 */ u8 unk0[0x10];        /* object header */
    /* 0x10 */ BoltDef def;
    /* 0x60 */ BoltLink *link;       /* user data, stored after the points */
    /* 0x64 */ BoltPoint *points;
    /* 0x68 */ f32 jitterTimer;
    /* 0x6C */ u8 vtxBuf[0x10];
    /* 0x7C */ s32 vtxBufSize;
    /* 0x80 */ BoltPoint pointData[1];
} Bolt;

Bolt *func_151A7950(BoltDef *, s32, u8, s32);

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

/*
 * Allocates a bolt for def with extraSize bytes of user data after its points. The local points run
 * from (0, 0, 0) to (0, 0, 1); func_151A8340 fills in the ones between.
 */
Bolt *func_151A7950(BoltDef *def, s32 extraSize, u8 arg2, s32 arg3) {
    Bolt *bolt;

    bolt = func_15167A68(0x2D, arg3, def->numPoints * sizeof(BoltPoint) + extraSize + sizeof(Bolt) - sizeof(BoltPoint), 1, arg2, 1);
    if (bolt == NULL) {
        return NULL;
    }
    memcpy(&bolt->def, def, sizeof(BoltDef));
    bolt->points = bolt->pointData;
    bolt->link = (BoltLink *)(bolt->points + def->numPoints);
    bolt->jitterTimer = 0.0f;
    bolt->points[0].local.x = 0.0f;
    bolt->points[0].local.y = 0.0f;
    bolt->points[0].local.z = 0.0f;
    bolt->points[def->numPoints - 1].local.x = 0.0f;
    bolt->points[def->numPoints - 1].local.y = 0.0f;
    bolt->points[def->numPoints - 1].local.z = 1.0f;
    bzero(bolt->vtxBuf, sizeof(bolt->vtxBuf));
    /* four vertices per point, plus slack */
    bolt->vtxBufSize = bolt->def.numPoints * 4 * (s32)sizeof(Vtx) + 0x140;
    return bolt;
}

extern s32 (*D_8008F940[])(Bolt *);
extern s32 (*D_8008F948[])(Bolt *);

void func_151A8340(Bolt *, s16, s16, f32, s16);

/*
 * Per-frame update: counts down the lifetime, runs the update hooks, re-jitters the points when the
 * timer runs out and places them in the world along start->end. The bolt is destroyed once a hook
 * returns 0 or its lifetime runs out.
 */
void func_151A7A90(Bolt *bolt) {
    Bolt *self; // matching: the loop bound is read through a second copy of bolt
    u8 done;
    Vec3f side;
    Vec3f up;
    Vec3f dir;
    s32 update1;
    s32 update2;

    self = bolt;
    done = FALSE;
    if (bolt->def.flags & BOLT_TIMED) {
        bolt->def.lifetime -= D_800BE9E4;
        if (bolt->def.lifetime < 0) {
            done = TRUE;
        }
    }
    update1 = bolt->def.update1;
    if (update1 != -1 && !done) {
        done = (D_8008F940[update1](bolt) == 0);
    }
    update2 = bolt->def.update2;
    if (update2 != -1 && !done) {
        done = (D_8008F948[update2](bolt) == 0);
    }

    if (!done) {
        bolt->jitterTimer -= D_800BE9A4;
        if (bolt->jitterTimer < 0.0f) {
            func_151A8340(bolt, 0, bolt->def.numPoints - 1, bolt->def.jitter, 100);
            bolt->jitterTimer = func_150ADA68() * bolt->def.jitterDelayRange + bolt->def.jitterDelay;
        }
        if (bolt->def.flags & BOLT_ACTIVE) {
            BoltPoint *pt;

            dir.x = bolt->def.end.x - bolt->def.start.x;
            dir.y = bolt->def.end.y - bolt->def.start.y;
            dir.z = bolt->def.end.z - bolt->def.start.z;
            up = bolt->def.up;
            func_151450B4((struct17 *)&dir, (struct17 *)&up, (struct17 *)&side);
            {
                s32 i;

                side.x *= bolt->def.invLength;
                side.y *= bolt->def.invLength;
                side.z *= bolt->def.invLength;

                pt = bolt->points;
                for (i = 0; i < self->def.numPoints; i++) {
                    pt->world.x = bolt->def.start.x + pt->local.x * side.x +
                                  pt->local.y * up.x + pt->local.z * dir.x;
                    pt->world.y = bolt->def.start.y + pt->local.x * side.y +
                                  pt->local.y * up.y + pt->local.z * dir.y;
                    pt->world.z = bolt->def.start.z + pt->local.x * side.z +
                                  pt->local.y * up.z + pt->local.z * dir.z;
                    pt++;
                }
            }
        }
    }

    if (done) {
        func_1516972C((struct102 *)bolt);
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
Gfx *func_15142E24(Gfx *, u8 *, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Vec3f *func_15144B34(s32);
void func_15145548(Vec3f *, Vec3f *, Vec3f *, Vec3f *, f32 *);
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142C10(Gfx *, s32, s32, s32, s32, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80090D70[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

/*
 * Draws the bolt as one camera-facing quad (two triangles) per segment, def.width wide, with
 * vertices from the bolt's vertex buffer for this view.
 */
Gfx *func_151A7D6C(Gfx *gdl, Bolt *bolt, s16 view) {
    Bolt *self = bolt; // matching: the loop bound is read through a second copy of bolt
    Vtx_t *vtx;
    Vec3f dir;
    Vec3f closest;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    Vec3f *camera;
    f32 lenSq;
    u8 changed;
    s16 last;
    f32 scale;
    s16 i;
    f32 parameter;
    s16 j;
    Vec3f *from;
    Vec3f *to;

    if (bolt->def.flags & BOLT_ACTIVE) {
        func_151D5D60(bolt->vtxBuf, view, bolt->vtxBufSize, (void **)&vtx, NULL);
        if (vtx != NULL) {
            camera = func_15144B34(view);
            changed = 1;
            last = bolt->def.numPoints - 1;
            j = 0;
            i = 1;
            gdl = func_15142E24(gdl, D_80090D70, 0, 0, 0, 0, 0x2C, 0, 0, &changed, 3);
            gdl = func_15142B7C(gdl, 0x200005, 0x1F0600);
            gdl = func_15142C10(gdl, bolt->def.color.r, bolt->def.color.g, bolt->def.color.b, bolt->def.color.a, &changed);
            gdl = func_1513F4E4(gdl, 0x19, &changed);
            gdl = func_15142FBC(gdl, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[3][0] | D_800A4AC8[3][1], &changed);
            dir.x = bolt->points[0].world.x - bolt->points[last].world.x;
            dir.y = bolt->points[0].world.y - bolt->points[last].world.y;
            dir.z = bolt->points[0].world.z - bolt->points[last].world.z;
            func_15145548(&bolt->points[last].world, &dir, camera, &closest, &parameter);
            dx = camera->x - closest.x;
            dy = camera->y - closest.y;
            dz = camera->z - closest.z;
            nx = dir.y * dz - dy * dir.z;
            ny = dir.z * dx - dz * dir.x;
            nz = dir.x * dy - dx * dir.y;
            lenSq = nx * nx + ny * ny + nz * nz;
            if (lenSq == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = bolt->def.width / sqrtf(lenSq);
                nx *= scale;
                ny *= scale;
                nz *= scale;
            }
            for (; i < self->def.numPoints; i++, j++) {
                to = &bolt->points[i].world;
                from = &bolt->points[j].world;
                vtx->ob[0] = from->x + nx;
                vtx->ob[1] = from->y + ny;
                vtx->ob[2] = from->z + nz;
                vtx->tc[0] = 0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = from->x - nx;
                vtx->ob[1] = from->y - ny;
                vtx->ob[2] = from->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = to->x - nx;
                vtx->ob[1] = to->y - ny;
                vtx->ob[2] = to->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0x3C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = to->x + nx;
                vtx->ob[1] = to->y + ny;
                vtx->ob[2] = to->z + nz;
                vtx->tc[0] = 0;
                vtx->tc[1] = 0x3C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                gSPVertex(gdl++, vtx - 4, 4, 0);
                gSP1Triangle(gdl++, 0, 1, 2, 0);
                gSP1Triangle(gdl++, 0, 2, 3, 0);
            }
        }
    }
    return gdl;
}

/*
 * Midpoint displacement: sets the point halfway between first and last to their average, moved up
 * to amp sideways and along up, then recurses into both halves until depth runs out.
 */
void func_151A8340(Bolt *bolt, s16 first, s16 last, f32 amp, s16 depth) {
    s16 middle;

    if (last - first < 2) {
        return;
    }
    if (depth <= 0) {
        return;
    }
    {
        f32 dx;
        f32 dy;
        f32 dz;
        Vec3f *a;

        {
            Vec3f *b;

            a = &bolt->points[first].local;
            middle = first + ((last - first) >> 1);
            b = &bolt->points[last].local;
            dx = b->x - bolt->points[first].local.x;
            dy = b->y - bolt->points[first].local.y;
            dz = b->z - bolt->points[first].local.z;
        }
        bolt->points[middle].local.x = (a->x + dx * 0.5f) + (func_150ADA68() * (amp + amp) + -amp);
        bolt->points[middle].local.y = (a->y + dy * 0.5f) + (func_150ADA68() * (amp + amp) + -amp);
        bolt->points[middle].local.z = a->z + dz * 0.5f;
    }
    func_151A8340(bolt, first, middle, amp, depth - 1);
    func_151A8340(bolt, middle, last, amp, depth - 1);
}

extern void func_151D5E30(void *);

/* Frees the bolt's vertex buffers. */
void func_151A8560(Bolt *bolt) {
    func_151D5E30(bolt->vtxBuf);
}

extern void (*D_8008F94C[])(Bolt *);

/* Destroy handler: runs the type's hook, then frees the vertex buffers and the object. */
void func_151A8584(Bolt *bolt) {
    Bolt *self = bolt; // matching: the copy lets IDO read type from a0 before homing bolt

    if (D_8008F94C[self->def.type] != NULL) {
        D_8008F94C[self->def.type](bolt);
    }
    func_151A8560(bolt);
    func_15169804((struct102 *)bolt);
}

extern void (*D_8008F958[])(Bolt *);

/* Like func_151A8584, through the D_8008F958 hooks and func_15169824. */
void func_151A85D4(Bolt *bolt) {
    Bolt *self = bolt; // matching: the copy lets IDO read type from a0 before homing bolt

    if (D_8008F958[self->def.type] != NULL) {
        D_8008F958[self->def.type](bolt);
    }
    func_151A8560(bolt);
    func_15169824((struct102 *)bolt);
}

/*
 * Spawns a bolt from bone `bone` of owner (at boneOffset in bone space) to targetOffset from the
 * owner's position; func_151A87F8 (update1) keeps both ends attached. lifetime -1 means untimed.
 */
Bolt *func_151A8624(struct127 *owner, Vec3f *boneOffset, u8 bone, s16 lifetime,
                    f32 jitterDelay, f32 jitterDelayRange, BoltColor *color,
                    f32 width, Vec3f *targetOffset, u8 arg9, u8 argA,
                    s8 update2, s32 extraSize, u8 argD, s32 argE) {
    Bolt *bolt;
    BoltDef def;
    BoltLink link;
    u8 timed;

    if (owner == NULL) {
        return NULL;
    }
    if (arg9 >= 2) {
        arg9 = 0;
    }

    link.boneOffset = *boneOffset;
    link.bone = bone;
    link.linkFlags = 0;
    link.owner = owner;
    link.ownerId = owner->unique_id;
    link.unk24 = argA;

    if (targetOffset != NULL) {
        link.targetOffset = *targetOffset;
    } else {
        link.targetOffset.x = 0.0f;
        link.targetOffset.y = 0.0f;
        link.targetOffset.z = 0.0f;
    }

    if (lifetime == -1) {
        def.lifetime = 300;
    } else {
        def.lifetime = lifetime;
    }
    def.numPoints = 9;
    def.jitterDelay = jitterDelay;
    def.jitterDelayRange = jitterDelayRange;
    if (lifetime == -1) {
        timed = 0;
    } else {
        timed = BOLT_TIMED;
    }
    def.flags = timed;
    def.jitter = 79.75f;
    def.color = *color;
    def.width = width;
    // matching: both arms pick update1 1 (func_151A87F8), but IDO must see the arg9 test
    if ((arg9 != 0) && (arg9 == 1)) {
        def.update1 = 1;
    } else {
        def.update1 = 1;
    }
    def.start.x = 0.0f;
    def.start.y = 0.0f;
    def.start.z = 0.0f;
    def.end.x = 0.0f;
    def.end.y = 0.0f;
    def.end.z = 0.0f;
    def.up.x = 0.0f;
    def.up.y = 0.0f;
    def.up.z = 0.0f;
    def.length = 0.0f;
    def.update2 = update2;
    def.type = 2;
    def.invLength = 1.0f;

    bolt = func_151A7950(&def, extraSize + sizeof(BoltLink), argD, argE);
    if (bolt != NULL) {
        memcpy(bolt->link, &link, sizeof(link));
    }
    return bolt;
}

extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, s32, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_15143134(Vec3f *, Vec3f *, s32);
extern f32 func_15143E64(Vec3f *);
extern s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);

/*
 * Update hook 1 of a bolt spawned by func_151A8624: puts the start on the owner's bone and, until
 * a ray cast succeeds, casts from there towards the target point to place the end. Deactivates
 * the bolt when it has no length or the cast fails; returns 0 (destroy) once the owner is gone.
 */
s32 func_151A87F8(Bolt *bolt) {
    BoltLink *link;
    struct127 *owner;
    Vec3f dir;
    s32 hit;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f unused;

    link = bolt->link;
    owner = link->owner;
    if ((owner->interaction_state == 0) || (owner->unique_id != link->ownerId) || (owner->id == 0xFF)) {
        return 0;
    }
    if (owner->unk1D4 != NULL) {
        func_15143134(&link->boneOffset, &bolt->def.start, (s32)owner->unk1D4 + (link->bone << 6)); /* 0x40-byte bone matrices */
        if (!(link->linkFlags & 1)) {
            dx = bolt->def.start.x - (owner->x_position + link->targetOffset.x);
            dy = bolt->def.start.y - (owner->y_position + link->targetOffset.y);
            dz = bolt->def.start.z - (owner->z_position + link->targetOffset.z);
            if (func_150AC9C0(bolt->def.start.x, bolt->def.start.y, bolt->def.start.z, dx, dy, dz, 0, 0,
                              &bolt->def.end.x, &bolt->def.end.y, &bolt->def.end.z, 0, &hit, 0, 0.0f)) {
                if (func_15145C90(hit)) {
                    link->linkFlags |= 1;
                } else {
                    bolt->def.flags &= ~BOLT_ACTIVE;
                    return 1;
                }
            } else {
                bolt->def.flags &= ~BOLT_ACTIVE;
                return 1;
            }
        }
        dir.x = bolt->def.end.x - bolt->def.start.x;
        dir.y = bolt->def.end.y - bolt->def.start.y;
        dir.z = bolt->def.end.z - bolt->def.start.z;
        bolt->def.length = func_15143E64(&dir);
        // matching: the unused labels split IDO's basic blocks (length is reloaded; removing any one mismatches)
        if (bolt->def.length != 0.0f) {
        lbl0:
            bolt->def.invLength = 1.0f / bolt->def.length;
            func_15146078(&dir, &unused, &bolt->def.up);
            bolt->def.flags |= BOLT_ACTIVE;
        lbl1:
            return 1;
        } else {
            bolt->def.flags &= ~BOLT_ACTIVE;
        lbl2:
            return 1;
        }
    } else {
        bolt->def.flags &= ~BOLT_ACTIVE;
    }
lbl3:
    return 1;
}

extern void (*D_8008F964[])(Bolt *, void *, u8);

/* Message handler: forwards to the type's handler in D_8008F964 (types 3 and up use type 0). */
void func_151A8A20(Bolt *bolt, void *msg, u8 msgType) {
    s32 type;

    type = bolt->def.type;
    if (type >= 3) {
        type = 0;
    }
    if (D_8008F964[type] != NULL) {
        D_8008F964[type](bolt, msg, msgType);
    }
}

/* Message 0x2D: two actors traded places (each with its unique_id). */
typedef struct {
    struct127 *actorA;
    union {
        struct127 *actorB;
        u8 id;              /* message 0: the unique_id of the dying actor */
    } u4;
    u8 idA;
    u8 idB;
} BoltOwnerMsg;

/*
 * Type 2 message handler (bolts from func_151A8624): message 0 destroys the bolt when its owner goes
 * away; message 0x2D re-attaches it when its owner is swapped for another actor.
 */
void func_151A8A78(Bolt *bolt, void *arg, u8 msgType) {
    BoltLink *link = bolt->link;
    BoltOwnerMsg *msg = arg;
    struct127 *actor;

    if (msgType == 0) {
        actor = msg->actorA;
        if ((actor == link->owner) || (link->ownerId == msg->u4.id)) {
            func_1516972C((struct102 *)bolt);
        }
    } else if (msgType == 0x2D) {
        if (link->owner == msg->actorA) {
            link->owner = msg->u4.actorB;
            link->ownerId = msg->idB;
        } else if (link->owner == msg->u4.actorB) {
            link->owner = msg->actorA;
            link->ownerId = msg->idA;
        }
    }
}

/* A placed level object; the shared headers type it as struct134. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u8 pad6[0x10];
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s32 unk18;
} LevelObject;

/* Result of a ground probe (func_15046C80). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 unk4[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 surfaceType;
    /* 0x20 */ s32 unk20;
} GroundHit; /* size 0x24 */

/* Spawn parameters of a particle emitter (func_151A8B20). */
typedef struct {
    /* 0x00 */ u8 flags;             /* EMITTER_* */
    /* 0x04 */ LevelObject *anchor;  /* centre (and area) with EMITTER_ANCHORED */
    /* 0x08 */ f32 rate;             /* particles per frame and unit area: rate + rand * rateRange */
    /* 0x0C */ f32 rateRange;
    /* 0x10 */ Vec3f pos;            /* centre without EMITTER_ANCHORED */
    /* 0x1C */ f32 bottom;           /* lowest height the ground probe searches */
    /* 0x20 */ f32 radius;
    /* 0x24 */ u8 type;              /* 1: switched on and off by messages (func_151A931C) */
    /* 0x25 */ s8 update;            /* index into D_8008F980, -1 for none */
    /* 0x26 */ s8 spawn;             /* index into D_8008F970, -1 for none */
} EmitterDef; /* size 0x28 */

#define EMITTER_ON       1
#define EMITTER_ANCHORED 2
#define EMITTER_ZONED    4 /* only spawns while its zone is visible */
#define EMITTER_NO_PROBE 8 /* spawns at `bottom` without probing for the ground */

typedef struct {
    /* 0x00 */ EmitterDef def;
    /* 0x28 */ f32 pending;          /* particles owed; one spawns per whole unit */
    /* 0x2C */ GroundHit ground;
    /* 0x50 */ s32 zone;             /* func_1510FD20 zone of the centre, with EMITTER_ZONED */
    /* 0x54 */ f32 area;
} EmitterState; /* size 0x58 */

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EmitterState state;
    /* 0x80 */ u8 data[1];           /* user data */
} Emitter;

extern void func_1510F800(s32);

/*
 * Spawns an emitter object for def with extraSize bytes of user data. lifetime -1 means untimed.
 * The spawn rate is scaled by the emitter's area: the anchor's, or that of a circle of def->radius.
 */
Emitter *func_151A8B20(EmitterDef *def, s16 lifetime, s32 extraSize, u8 arg3, s32 arg4) {
    Emitter *emitter;
    EmitterState init;
    EmitterState *state;

    memcpy(&init, def, sizeof(EmitterDef));

    init.pending = 0.0f;
    init.ground.height = 99999.9f;
    init.ground.unk18 = 0;
    init.ground.flags = 0;
    init.ground.surfaceType = 0;
    init.ground.unk20 = 0;

    /* func_15149130 takes the data size (after its 0x28-byte header) in its struct37 * parameter */
    emitter = (Emitter *)func_15149130((lifetime == -1) ? 300 : lifetime, -1, 0x25, -1, (lifetime == -1) ? 0 : 1, 0x22, (struct37 *)(extraSize + sizeof(EmitterState)), arg3, arg4);
    if (emitter != NULL) {
        state = &emitter->state;
        memcpy(state, &init, sizeof(EmitterState));

        if (state->def.flags & EMITTER_ZONED) {
            f32 z;
            f32 x;

            if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
                x = state->def.anchor->x;
                z = state->def.anchor->z;
            } else {
                x = state->def.pos.x;
                z = state->def.pos.z;
            }
            func_1510F800(0);
            state->zone = func_1510FD20(x, z);
        } else {
            state->zone = 0;
        }

        if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
            state->area = func_15144598((struct134 *)state->def.anchor);
        } else {
            state->area = (state->def.radius * state->def.radius) * 3.14159265f;
        }
    }
    return emitter;
}

extern s32 (*D_8008F980[])(Emitter *);
extern void (*D_8008F970[])(Emitter *, Vec3f *, f32, u8);

s32 func_151464B8(s32);
s32 func_15046C80(Vec3f *, s32, f32, GroundHit *);
void func_151A8F1C(Emitter *, Vec3f *, f32 *, f32 *);
void func_151A8F6C(Emitter *, Vec3f *, f32 *, f32 *);

/*
 * Per-frame update: runs the update hook (0 kills the object), then while switched on (and its zone
 * is visible) accumulates the spawn rate and calls the spawn hook once per whole particle, at a
 * random point of the area dropped to the ground.
 */
void func_151A8CEC(Emitter *emitter) {
    EmitterState *state;
    void (*pick)(Emitter *, Vec3f *, f32 *, f32 *);
    Vec3f pos;
    f32 bottom;
    f32 top;
    u8 noGround;

    state = &emitter->state;

    if (state->def.update != -1) {
        if (D_8008F980[state->def.update](emitter) == 0) {
            emitter->unkE = -1;
            return;
        }
    }

    if ((state->def.flags & EMITTER_ZONED) && (state->zone != 0)) {
        if (func_151464B8(state->zone) != 0) {
            return;
        }
    }
    if (!(state->def.flags & EMITTER_ON)) {
        return;
    }

    state->pending += (state->def.rate + func_150ADA68() * state->def.rateRange) * D_800BE9A4 * state->area;

    if (state->pending > 1.0f) {
        if ((state->def.flags & EMITTER_ANCHORED) && (state->def.anchor != NULL)) {
            pick = func_151A8F1C;
        } else {
            pick = func_151A8F6C;
        }
        do {
            pick(emitter, &pos, &top, &bottom);
            if (state->def.flags & EMITTER_NO_PROBE) {
                pos.y = bottom;
                noGround = TRUE;
            } else if (func_15046C80(&pos, 0, bottom, &state->ground) != 0) {
                pos.y = state->ground.height; /* noGround keeps its previous (at first unset) value */
            } else {
                pos.y = bottom;
                noGround = TRUE;
            }
            if (state->def.spawn != -1) {
                D_8008F970[state->def.spawn](emitter, &pos, top, noGround);
            }
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
    }
}

extern void func_151432BC(LevelObject *, f32 *, f32 *, f32 *, f32 *);

/* Picks a random point of the anchor object's area; *top is its height. */
void func_151A8F1C(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom) {
    func_151432BC(emitter->state.def.anchor, &pos->x, &pos->z, top, bottom);
    pos->y = *top;
}

void func_15143874(s16, f32, f32 *, f32 *);

/* Picks a random point within def.radius of def.pos; *top is def.pos.y, *bottom def.bottom. */
void func_151A8F6C(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom) {
    EmitterDef *def;

    def = &emitter->state.def;
    func_15143874((s16)(func_150ADA20() & 0xFF), def->radius * func_150ADA68(), &pos->x, &pos->z);
    pos->x += def->pos.x;
    pos->z += def->pos.z;
    pos->y = def->pos.y;
    *top = def->pos.y;
    *bottom = def->bottom;
}

void func_151A931C(Emitter *, u8 *, u8);

/* Message handler: type 1 emitters are switched on and off by messages. */
void func_151A9024(Emitter *emitter, u8 *msg, u8 msgType) {
    if (emitter->state.def.type == 1) {
        func_151A931C(emitter, msg, msgType);
    }
}

extern void (*D_8008F984[])(LevelObject *, s32);

/* Level object setup: marks the object and runs the handler for its kind (unk18, 0 to 7). */
s32 func_151A9060(LevelObject *obj) {
    s32 kind;

    obj->flags |= 4;
    kind = obj->unk18;
    if (kind < 0 || kind >= 8) {
        return 1;
    }
    if (D_8008F984[kind] != NULL) {
        D_8008F984[kind](obj, kind);
    }
    return 1;
}

/*
 * Kind 0-5 level object handler: an emitter anchored to obj, with message id `kind`; odd kinds
 * spawn faster.
 */
void func_151A90C0(LevelObject *obj, s32 kind) {
    EmitterDef def;
    // matching: the id byte sits 4 bytes below this unused slot, in frame padding no declared
    // local reaches (a u8 local lands at sp+0x2F or grows the frame)
    s32 idSlot;
    f32 odd;
    Emitter *emitter;

    def.flags = EMITTER_ANCHORED;
    def.anchor = obj;
    odd = kind & 1;
    if (odd) {
        def.rate = 1.4e-6f;
    } else {
        def.rate = 4e-7f;
    }

    if (odd) {
        def.rateRange = 8e-7f;
    } else {
        def.rateRange = 6e-7f;
    }

    def.pos.x = 0.0f;
    def.pos.y = 0.0f;
    def.pos.z = 0.0f;
    def.bottom = 0.0f;
    def.radius = 0.0f;
    def.type = 1;
    def.update = -1;
    def.spawn = 0;
    ((u8 *)&idSlot)[-4] = kind;

    emitter = func_151A8B20(&def, -1, 1, 0xFF, 0);
    if (emitter != NULL) {
        memcpy(emitter->data, (u8 *)&idSlot - 4, 1);
    }
}

typedef struct {
    s16 ids[3];
} SpriteChoices;

extern SpriteChoices D_8008F9A4;

/* Spawn descriptor passed to func_1513C650. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
} SpriteSpawn;

/* Spawn hook 0: a sprite of random size (50 to 100) using one of three D_8008F9A4 images, at pos. */
void func_151A91AC(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround) {
    SpriteSpawn spawn;
    f32 size;
    SpriteChoices choices;
    u32 pick;
    s32 rand2;
    s32 rand1;

    choices = D_8008F9A4;
    size = func_150ADA68() * 50.0f + 50.0f;
    pick = func_150ADA20() % 3;

    spawn.unk6 = choices.ids[pick];
    spawn.unk7 = 0;
    spawn.unk16 = 0;
    spawn.unk17 = 7;
    spawn.unk8 = 0;
    spawn.unkC = 0;
    spawn.flags = 0x1701;
    spawn.lifetime = 60;
    spawn.unk10 = 0xA0;
    spawn.unk11 = 0xFF;
    spawn.unk12 = 0;
    spawn.unk13 = 0;
    spawn.unk14 = 0;
    spawn.unk15 = 0xFF;
    spawn.unk18 = 0x3B0002;

    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    func_1513C650((s32)&spawn, 1, 0, (s32)emitter->state.ground.unk4, pos->x, pos->y, pos->z,
                  size, size, rand1 & 0xFF,
                  (rand2 & 1) + (func_150ADA20() & 1), 3, 0xFF, 0,
                  emitter->unkC, emitter->unk1);
}

/* Type 1 message handler: 0x17 switches the emitter on, 0x18 off, when the message carries its id. */
void func_151A931C(Emitter *emitter, u8 *msg, u8 msgType) {
    EmitterState *state;

    if (msgType == 0x17) {
        state = &emitter->state;
        // matching: the goto keeps IDO from sharing the 0x18 branch's return
        if (*msg != emitter->data[0]) {
            goto ret;
        }
        state->def.flags |= EMITTER_ON;
    ret:
        return;
    } else if (msgType == 0x18) {
        state = &emitter->state;
        if (*msg == emitter->data[0]) {
            state->def.flags &= ~EMITTER_ON;
        }
    }
}

typedef struct {
    s16 base;
    s16 range;
} RangeS16; /* base + rand % (range + 1) */

/* User data of a type 2 emitter: the particles spawn hook 1 (func_151A9634) launches. */
typedef struct {
    /* 0x00 */ f32 vec0Base;         /* the two floats passed to func_1514B8E4: base + rand * range */
    /* 0x04 */ f32 vec1Base;
    /* 0x08 */ f32 vec0Range;
    /* 0x0C */ f32 vec1Range;
    /* 0x10 */ RangeS16 unk10;
    /* 0x14 */ RangeS16 unk14;       /* goes into the particle's extra data */
    /* 0x18 */ RangeS16 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20Base;        /* goes into the particle's extra data */
    /* 0x24 */ f32 unk20Range;
    /* 0x28 */ u8 flags;             /* 1: particle kind 0x71 (else 0x13 or 0x14); 2: clear the 2 flag */
} ParticleSettings; /* size 0x2C */

/* One of the nine emitter presets of func_151A9390. */
typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ u8 unk8;              /* unk8..unkC go to func_1516284C */
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 unkC;
    /* 0x10 */ ParticleSettings *settings;
} EmitterPreset;

extern EmitterPreset D_8008F9AC[];

void *func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);

/*
 * Spawns particle emitter preset `preset` (0-8) on obj, or at pos when given. options:
 * 1 EMITTER_ZONED, 2 EMITTER_NO_PROBE, 4 also call func_1516284C there, 8 and 0x10 set settings
 * flags 1 and 2. lifetime -1 means untimed.
 */
void func_151A9390(u8 options, u8 preset, LevelObject *obj, Vec3f *pos,
                   f32 bottom, f32 radius, s16 lifetime, u8 arg7, s32 arg8) {
    EmitterDef def;
    Emitter *emitter;

    if (preset >= 9) {
        return;
    }

    def.flags = EMITTER_ON | ((obj != NULL) ? EMITTER_ANCHORED : 0) | ((options & 1) ? EMITTER_ZONED : 0) |
                ((options & 2) ? EMITTER_NO_PROBE : 0);
    def.anchor = obj;
    def.rate = D_8008F9AC[preset].rate;
    def.rateRange = D_8008F9AC[preset].rateRange;
    if (pos != NULL) {
        def.pos = *pos;
    } else {
        def.pos.x = 0.0f;
        def.pos.y = 0.0f;
        def.pos.z = 0.0f;
    }
    def.bottom = bottom;
    def.radius = radius;
    def.type = 2;
    def.update = -1;
    def.spawn = 1;

    emitter = func_151A8B20(&def, lifetime, sizeof(ParticleSettings), arg7, arg8);
    if (emitter != NULL) {
        ParticleSettings *settings;

        settings = (ParticleSettings *)emitter->data;
        memcpy(settings, D_8008F9AC[preset].settings, sizeof(ParticleSettings));
        if (options & 8) {
            settings->flags |= 1;
        }
        if (options & 0x10) {
            settings->flags |= 2;
        }
    }

    if (options & 4) {
        Header spawn;
        s32 spawnPos[3];

        spawn.unk0 = ((lifetime == -1) ? 0 : 1) | 2;
        spawn.unk1 = 2;
        spawn.unk2 = (lifetime == -1) ? 300 : lifetime;
        spawn.unk4 = D_8008F9AC[preset].unkB;

        if (pos != NULL) {
            spawnPos[0] = pos->x;
            spawnPos[1] = pos->y;
            spawnPos[2] = pos->z;
        } else {
            spawnPos[0] = obj->x;
            spawnPos[1] = obj->y;
            spawnPos[2] = obj->z;
        }

        func_1516284C(&spawn, spawnPos, D_8008F9AC[preset].unk8, D_8008F9AC[preset].unk9,
                      D_8008F9AC[preset].unkA, 0xFF, 0, 0, D_8008F9AC[preset].unkC,
                      0xFF, 1);
    }
}

/* Extra data stored at 0x160 of each particle (the particle object is not typed here). */
typedef struct {
    f32 unk0;
    s16 unk4;
} ParticleExtra;

u8 *func_1514B8E4(Vec3f *, f32 *, s16, s32, s32, f32, f32, f32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawn hook 1: launches one particle at pos with values drawn from the emitter's ParticleSettings. */
void func_151A9634(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround) {
    f32 vec[2];
    s32 choice;
    ParticleExtra extra;
    s32 kind;
    s32 flags;
    u8 *particle;
    ParticleSettings *settings;
    u32 rand2;
    u32 rand1;

    settings = (ParticleSettings *)emitter->data;
    vec[0] = func_150ADA68() * settings->vec0Range + settings->vec0Base;
    vec[1] = func_150ADA68() * settings->vec1Range + settings->vec1Base;
    extra.unk0 = func_150ADA68() * settings->unk20Range + settings->unk20Base;
    extra.unk4 = func_150ADA20() % (u32)(settings->unk14.range + 1) + settings->unk14.base;
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    if (settings->flags & 1) {
        kind = 0x71;
    } else {
        if (func_150ADA20() & 1) {
            choice = 0x13;
        } else {
            choice = 0x14;
        }
        kind = choice; // matching: the extra copy shapes IDO's register use
    }
    flags = (settings->flags & 2) ? 0 : 2;
    particle = func_1514B8E4(pos, vec,
        (s16)(rand1 % (u32)(settings->unk10.range + 1) + settings->unk10.base),
        (rand2 % (u32)(settings->unk18.range + 1) + settings->unk18.base) & 0xFF,
        0, 0.0f, 1.0f, 1.0f, 0x21, 0x23, 2, kind, flags,
        settings->unk1C, settings->unk1E, 8, emitter->unkC, emitter->unk1);
    if (particle != NULL) {
        memcpy(particle + 0x160, &extra, sizeof(extra));
    }
}

extern void (*D_8008FA60[])(Vec3f *, GroundHit *, s32, u8, s32);

/*
 * Calls spawn function D_8008FA60[spawnFunc] at `count` random points within radius of center
 * that have ground above minY (the spawn point is on the ground). hit is the probe result buffer;
 * NULL uses a local one.
 */
void func_151A9834(Vec3f *center, f32 minY, f32 radius, GroundHit *hit,
                   s32 count, u8 spawnFunc, s32 arg6, u8 arg7, s32 arg8) {
    GroundHit localHit;
    Vec3f probe;
    Vec3f spawnPos;
    f32 angle;
    f32 dist;

    if (hit == NULL) {
        hit = &localHit;
        localHit.height = 99999.9f;
        localHit.unk18 = 0;
        localHit.flags = 7;
        localHit.surfaceType = 0;
        localHit.unk20 = 0;
    }

    probe.y = center->y;
    for (; count > 0; count--) {
        angle = func_150ADA68();
        dist = func_150ADA68();
        func_1514373C((angle + angle) * 3.14159265f, dist * radius, &probe.x, &probe.z);
        probe.x += center->x;
        probe.z += center->z;

        if (func_15046C80(&probe, 0, minY, hit) != 0) {
            spawnPos.x = probe.x;
            spawnPos.y = hit->height;
            spawnPos.z = probe.z;
            D_8008FA60[spawnFunc](&spawnPos, hit, arg6, arg7, arg8);
        }
    }
}
