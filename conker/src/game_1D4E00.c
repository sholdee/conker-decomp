#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The bolt object type's render setup and teardown display lists (object type table D_8008B4A8). */
Gfx D_8008F910[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

Gfx D_8008F930[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

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
    bolt->link = bolt->points + def->numPoints;
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

/* Update hooks (BoltDef.update1, BoltDef.update2); a hook that returns 0 ends the bolt. */
static s32 (*D_8008F940[])(Bolt *) = { func_150EFA4C, func_151A87F8 };
static s32 (*D_8008F948[])(Bolt *) = { NULL };

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
            func_151450B4(&dir, &up, &side);
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
        func_1516972C(&bolt->header);
    }
}

extern s32 D_800A4AC8[][2];

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
        func_151D5D60(bolt->vtxBuf, view, bolt->vtxBufSize, &vtx, NULL);
        if (vtx != NULL) {
            camera = func_15144B34(view);
            changed = 1;
            last = bolt->def.numPoints - 1;
            j = 0;
            i = 1;
            gdl = func_15142E24(gdl, &D_80090B60[44], 0, 0, 0, 0, 0x2C, 0, 0, &changed, 3);
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

/* Frees the bolt's vertex buffers. */
void func_151A8560(Bolt *bolt) {
    func_151D5E30(bolt->vtxBuf);
}

/* Destroy and free hooks, by bolt type. */
static void (*D_8008F94C[])(Bolt *) = { NULL, func_150F0260, NULL };

/* Destroy handler: runs the type's hook, then frees the vertex buffers and the object. */
void func_151A8584(Bolt *bolt) {
    Bolt *self = bolt; // matching: the copy lets IDO read type from a0 before homing bolt

    if (D_8008F94C[self->def.type] != NULL) {
        D_8008F94C[self->def.type](bolt);
    }
    func_151A8560(bolt);
    func_15169804(&bolt->header);
}

static void (*D_8008F958[])(Bolt *) = { NULL, func_150F0280, NULL };

/* Like func_151A8584, through the D_8008F958 hooks and func_15169824. */
void func_151A85D4(Bolt *bolt) {
    Bolt *self = bolt; // matching: the copy lets IDO read type from a0 before homing bolt

    if (D_8008F958[self->def.type] != NULL) {
        D_8008F958[self->def.type](bolt);
    }
    func_151A8560(bolt);
    func_15169824(&bolt->header);
}

/*
 * Spawns a bolt from bone `bone` of owner (at boneOffset in bone space) to targetOffset from the
 * owner's position; func_151A87F8 (update1) keeps both ends attached. lifetime -1 means untimed.
 */
Bolt *func_151A8624(Actor *owner, Vec3f *boneOffset, u8 bone, s16 lifetime,
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

/*
 * Update hook 1 of a bolt spawned by func_151A8624: puts the start on the owner's bone and, until
 * a ray cast succeeds, casts from there towards the target point to place the end. Deactivates
 * the bolt when it has no length or the cast fails; returns 0 (destroy) once the owner is gone.
 */
s32 func_151A87F8(Bolt *bolt) {
    BoltLink *link;
    Actor *owner;
    Vec3f dir;
    s32 hit;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f unused;

    link = bolt->link;
    owner = link->owner;
    if ((owner->type == 0) || (owner->unique_id != link->ownerId) || (owner->character_type == 0xFF)) {
        return 0;
    }
    if (owner->ob_matrix_ptr != NULL) {
        func_15143134(&link->boneOffset, &bolt->def.start, &owner->ob_matrix_ptr[link->bone]);
        if (!(link->linkFlags & 1)) {
            dx = bolt->def.start.x - (owner->xpos + link->targetOffset.x);
            dy = bolt->def.start.y - (owner->ypos + link->targetOffset.y);
            dz = bolt->def.start.z - (owner->zpos + link->targetOffset.z);
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

/* Message handlers, by bolt type. */
static void (*D_8008F964[])(Bolt *, s32, u8) = { NULL, func_150F07E4, func_151A8A78 };

/* Message handler: forwards to the type's handler in D_8008F964 (types 3 and up use type 0). */
void func_151A8A20(Bolt *bolt, s32 msg, u8 msgType) {
    s32 type;

    type = bolt->def.type;
    if (type >= 3) {
        type = 0;
    }
    if (D_8008F964[type] != NULL) {
        D_8008F964[type](bolt, msg, msgType);
    }
}

/*
 * Type 2 message handler (bolts from func_151A8624): message 0 destroys the bolt when its owner goes
 * away; message 0x2D re-attaches it when its owner is swapped for another actor.
 */
void func_151A8A78(Bolt *bolt, s32 arg, u8 msgType) {
    BoltLink *link = bolt->link;
    ActorMsg *msg = (ActorMsg *)arg;
    Actor *actor;

    if (msgType == 0) {
        actor = msg->ref.actor;
        if ((actor == link->owner) || (link->ownerId == msg->ref.uniqueId)) {
            func_1516972C(&bolt->header);
        }
    } else if (msgType == 0x2D) {
        if (link->owner == msg->swap.actorA) {
            link->owner = msg->swap.actorB;
            link->ownerId = msg->swap.idB;
        } else if (link->owner == msg->swap.actorB) {
            link->owner = msg->swap.actorA;
            link->ownerId = msg->swap.idA;
        }
    }
}

