#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A8F68;
extern f32 D_800A8F6C;
extern f32 D_800A8F58;
extern f32 D_800A8F5C;
extern f32 D_800A8F60;
extern f32 D_800A8F64;
extern f32 D_800A8DE0;
extern void (*D_8008FA60[])(f32 *, void *, s32, u8, s32);

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
    if (update1 != -1) {
        if (!done) {
            done = (D_8008F940[update1](bolt) == 0);
        }
    }
    update2 = bolt->def.update2;
    if (update2 != -1) {
        if (!done) {
            done = (D_8008F948[update2](bolt) == 0);
        }
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
    Vec3f *a;
    Vec3f *b;

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
                b = &bolt->points[i].world;
                a = &bolt->points[j].world;
                vtx->ob[0] = a->x + nx;
                vtx->ob[1] = a->y + ny;
                vtx->ob[2] = a->z + nz;
                vtx->tc[0] = 0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = a->x - nx;
                vtx->ob[1] = a->y - ny;
                vtx->ob[2] = a->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = b->x - nx;
                vtx->ob[1] = b->y - ny;
                vtx->ob[2] = b->z - nz;
                vtx->tc[0] = 0x3C0;
                vtx->tc[1] = 0x3C0;
                vtx->cn[0] = 0xFF;
                vtx->cn[1] = 0xFF;
                vtx->cn[2] = 0xFF;
                vtx->cn[3] = 0xFF;
                vtx->flag = 0;
                vtx++;
                vtx->ob[0] = b->x + nx;
                vtx->ob[1] = b->y + ny;
                vtx->ob[2] = b->z + nz;
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
void *func_151A8624(struct127 *owner, Vec3f *boneOffset, u8 bone, s16 lifetime,
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
    def.jitter = D_800A8DE0;
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
                pos.y = state->ground.height;
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

void func_151A931C(u8 *, s32, u8);

void func_151A9024(u8 *arg0, s32 arg1, u8 arg2) {
    if (arg0[0x4C] == 1) {
        func_151A931C(arg0, arg1, arg2);
    }
}

extern void (*D_8008F984[])(void);
s32 func_151A9060(u8 *arg0)
{
  s32 temp;
  s32 idx;
  arg0[0x16] |= 0x4;
  temp = *((s32 *) (arg0 + 0x18));
  idx = temp & 0xFFFFFFFFu;
  temp = idx < 0;
  if (temp || (idx >= 8))
  {
    return 1;
  }
  if (D_8008F984[idx])
  {
    D_8008F984[idx]();
  }
  return 1;
}


void func_151A90C0(s32 arg0, s32 arg1) {
    struct {
        s8 unk0;
        u8 pad1[3];
        s32 unk4;
        f32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        s8 unk24;
        s8 unk25;
        s8 unk26;
    } sp30;
    s32 sp28;
    f32 temp_f0;
    s32 temp_v0;

    sp30.unk0 = 2;
    sp30.unk4 = arg0;
    temp_f0 = (f32)(arg1 & 1);
    if (temp_f0) {
        sp30.unk8 = D_800A8F58;
    } else {
        sp30.unk8 = D_800A8F5C;
    }

    if (temp_f0) {
        sp30.unkC = D_800A8F60;
    } else {
        sp30.unkC = D_800A8F64;
    }

    temp_f0 = 0.0f;
    sp30.unk10 = temp_f0;
    sp30.unk14 = temp_f0;
    sp30.unk18 = temp_f0;
    sp30.unk1C = temp_f0;
    sp30.unk20 = temp_f0;
    sp30.unk24 = 1;
    sp30.unk25 = -1;
    sp30.unk26 = 0;
    ((s8 *)&sp28)[-4] = (s8)arg1;

    temp_v0 = func_151A8B20(&sp30, -1, 1, 0xFF, 0);
    if (temp_v0 != 0) {
        memcpy((void *)(temp_v0 + 0x80), ((s8 *)&sp28) - 4, 1);
    }
}

struct S6_151A91AC { s16 unk0; s16 unk2; s16 unk4; };
extern struct S6_151A91AC D_8008F9A4;

struct Loc_151A91AC {
    s32 unk0;
    s16 unk4;
    u8  unk6;
    u8  unk7;
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

void func_151A91AC(u8 *arg0, f32 *arg1, s32 arg2, s32 arg3) {
    struct Loc_151A91AC sp64;
    f32 fval;
    struct S6_151A91AC sp58;
    u32 idx;
    s32 r3;
    s32 r2;

    sp58 = D_8008F9A4;
    fval = func_150ADA68() * 50.0f + 50.0f;
    idx = (u32)func_150ADA20() % 3U;

    sp64.unk6 = (u8)((s16 *)&sp58)[idx];
    sp64.unk7 = 0;
    sp64.unk16 = 0;
    sp64.unk17 = 7;
    sp64.unk8 = 0;
    sp64.unkC = 0;
    sp64.unk0 = 0x1701;
    sp64.unk4 = 0x3C;
    sp64.unk10 = 0xA0;
    sp64.unk11 = 0xFF;
    sp64.unk12 = 0;
    sp64.unk13 = 0;
    sp64.unk14 = 0;
    sp64.unk15 = 0xFF;
    sp64.unk18 = 0x3B0002;

    r2 = func_150ADA20();
    r3 = func_150ADA20();
    func_1513C650((s32)&sp64, 1, 0, (s32)(arg0 + 0x58), arg1[0], arg1[1], arg1[2],
                  fval, fval, r2 & 0xFF,
                  (r3 & 1) + (func_150ADA20() & 1), 3, 0xFF, 0,
                  arg0[0xC], arg0[1]);
}

void func_151A931C(u8 *arg0, s32 arg1, u8 arg2) {
    u8 *ptr;

    if (arg2 == 0x17) {
        ptr = arg0 + 0x28;
        if (*(u8 *)arg1 != arg0[0x80]) {
            goto ret;
        }
        *ptr |= 1;
    ret:
        return;
    } else if (arg2 == 0x18) {
        ptr = arg0 + 0x28;
        if (*(u8 *)arg1 == arg0[0x80]) {
            *ptr &= ~1;
        }
    }
}

typedef struct {
    f32 unk0;
    f32 unk4;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  unkB;
    u8  unkC;
    u8  padD[3];
    void *unk10;
} Def_151A9390;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_151A9390;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} Pos_151A9390;

typedef struct {
    u8  unk0;
    u8  pad1[3];
    void *unk4;
    f32 unk8;
    f32 unkC;
    Vec3f_151A9390 unk10;
    f32 unk1C;
    f32 unk20;
    s8  unk24;
    s8  unk25;
    s8  unk26;
    u8  pad27;
} Header_151A9390;

extern Def_151A9390 D_8008F9AC[];

void *func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);

void func_151A9390(u8 arg0, u8 arg1, Pos_151A9390 *arg2, Vec3f_151A9390 *arg3,
                   f32 arg4, f32 arg5, s16 arg6, u8 arg7, s32 arg8) {
    Header_151A9390 hdr;
    s32 effect;

    if (arg1 >= 9) {
        return;
    }

    hdr.unk0 = 1 | ((arg2 != NULL) ? 2 : 0) | ((arg0 & 1) ? 4 : 0) | ((arg0 & 2) ? 8 : 0);
    hdr.unk4 = arg2;
    hdr.unk8 = D_8008F9AC[arg1].unk0;
    hdr.unkC = D_8008F9AC[arg1].unk4;
    if (arg3 != NULL) {
        hdr.unk10 = *arg3;
    } else {
        hdr.unk10.x = 0.0f;
        hdr.unk10.y = 0.0f;
        hdr.unk10.z = 0.0f;
    }
    hdr.unk1C = arg4;
    hdr.unk20 = arg5;
    hdr.unk24 = 2;
    hdr.unk25 = -1;
    hdr.unk26 = 1;

    effect = func_151A8B20(&hdr, arg6, 0x2C, arg7, arg8);
    if (effect != 0) {
        u8 *ptr;

        ptr = (u8 *)(effect + 0x80);
        memcpy(ptr, D_8008F9AC[arg1].unk10, 0x2C);
        if (arg0 & 8) {
            ptr[0x28] |= 1;
        }
        if (arg0 & 0x10) {
            ptr[0x28] |= 2;
        }
    }

    if (arg0 & 4) {
        Header spawn;
        s32 pos[3];

        spawn.unk0 = ((arg6 == -1) ? 0 : 1) | 2;
        spawn.unk1 = 2;
        spawn.unk2 = (arg6 == -1) ? 0x12C : arg6;
        spawn.unk4 = D_8008F9AC[arg1].unkB;

        if (arg3 != NULL) {
            pos[0] = (s32)arg3->x;
            pos[1] = (s32)arg3->y;
            pos[2] = (s32)arg3->z;
        } else {
            pos[0] = arg2->unk0;
            pos[1] = arg2->unk2;
            pos[2] = arg2->unk4;
        }

        func_1516284C(&spawn, pos, D_8008F9AC[arg1].unk8, D_8008F9AC[arg1].unk9,
                      D_8008F9AC[arg1].unkA, 0xFF, 0, 0, D_8008F9AC[arg1].unkC,
                      0xFF, 1);
    }
}

typedef struct Emitter_151A9634 {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    s16 field10;
    s16 field12;
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1A;
    s16 field1C;
    s16 field1E;
    f32 field20;
    f32 field24;
    u8 flags28;
} Emitter_151A9634;

typedef struct Actor_151A9634 {
    u8 pad0;
    u8 field1;
    u8 pad2[0xA];
    u8 fieldC;
    u8 padD[0x73];
    Emitter_151A9634 emitter;
} Actor_151A9634;

typedef struct Payload_151A9634 {
    f32 field0;
    s16 field4;
} Payload_151A9634;

s32 func_1514B8E4(s32, f32 *, s16, s32, s32, f32, f32, f32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_151A9634(Actor_151A9634 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    f32 position[2];
    s32 choice;
    Payload_151A9634 payload;
    s32 kind;
    s32 flags;
    s32 result;
    Emitter_151A9634 *emitter;
    u32 random2;
    u32 random1;

    emitter = &arg0->emitter;
    position[0] = func_150ADA68() * emitter->field8 + emitter->field0;
    position[1] = func_150ADA68() * emitter->fieldC + emitter->field4;
    payload.field0 = func_150ADA68() * emitter->field24 + emitter->field20;
    payload.field4 = func_150ADA20() % (u32)(emitter->field16 + 1) + emitter->field14;
    random1 = func_150ADA20();
    random2 = func_150ADA20();
    if (emitter->flags28 & 1) {
        kind = 0x71;
    } else {
        if (func_150ADA20() & 1) {
            choice = 0x13;
        } else {
            choice = 0x14;
        }
        kind = choice;
    }
    flags = (emitter->flags28 & 2) ? 0 : 2;
    result = func_1514B8E4(arg1, position,
        (s16)(random1 % (u32)(emitter->field12 + 1) + emitter->field10),
        (random2 % (u32)(emitter->field1A + 1) + emitter->field18) & 0xFF,
        0, 0.0f, 1.0f, 1.0f, 0x21, 0x23, 2, kind, flags,
        emitter->field1C, emitter->field1E, 8, arg0->fieldC, arg0->field1);
    if (result != 0) {
        memcpy((u8 *)result + 0x160, &payload, 8);
    }
}

typedef struct {
    f32 unk0;
    u8 pad4[0x14];
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
    s32 unk20;
} Struct151A9834;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f151A9834;

void func_151A9834(f32 *arg0, f32 arg1, f32 arg2, Struct151A9834 *arg3,
                   s32 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8) {
    Struct151A9834 sp8C;
    Vec3f151A9834 sp80;
    Vec3f151A9834 sp74;
    f32 temp_f22;
    f32 temp_f20;
    f32 temp_f0;

    if (arg3 == 0) {
        arg3 = &sp8C;
        sp8C.unk0 = D_800A8F68;
        sp8C.unk18 = 0;
        sp8C.unk1C = 7;
        sp8C.unk1D = 0;
        sp8C.unk20 = 0;
    }

    sp80.y = arg0[1];
    if (arg4 > 0) {
        temp_f22 = D_800A8F6C;
        do {
            temp_f20 = func_150ADA68();
            temp_f0 = func_150ADA68();
            func_1514373C((temp_f20 + temp_f20) * temp_f22, temp_f0 * arg2,
                          &sp80.x, &sp80.z);
            sp80.x += arg0[0];
            sp80.z += arg0[2];

            if (func_15046C80(&sp80.x, 0, arg1, arg3) != 0) {
                sp74.x = sp80.x;
                sp74.y = arg3->unk0;
                sp74.z = sp80.z;
                D_8008FA60[arg5](&sp74.x, arg3, arg6, arg7, arg8);
            }
            arg4--;
        } while (arg4 > 0);
    }
}
