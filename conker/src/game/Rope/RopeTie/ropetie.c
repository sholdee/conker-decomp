#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/rope.h"
#include "conker/sprite.h"
#include "conker/texture.h"

/* Rope end offsets in bone space. */
static const Vec3f D_800AA320 = { 77.0f, 20.0f, 67.0f };
static const Vec3f D_800AA32C = { -77.0f, 20.0f, 67.0f };
static const Vec3f D_800AA338 = { -6.0f, 0.0f, 0.0f };
static const Vec3f D_800AA344 = { 6.0f, 0.0f, 0.0f };
static const Vec3f D_800AA350 = { -12.0f, 0.0f, 0.0f };
static const Vec3f D_800AA35C = { 12.0f, 0.0f, 0.0f };
static const Vec3f D_800AA368 = { 27.0f, 42.0f, 0.0f };
static const Vec3f D_800AA374 = { -27.0f, 42.0f, 0.0f };

/* Spawns a RopeTie between `obj` and the object from collision_findObjectById(1). */
void rope_spawnTie(Actor *obj) {
    RopeTie *effect;
    RopeTieData tie;

    if (obj != NULL) {
        tie.obj0 = obj;
        tie.obj0Id = obj->unique_id;
        tie.obj1 = collision_findObjectById(1);
        tie.obj1Id = 1;
        tie.state = 0;
        bzero(tie.ropes, sizeof(tie.ropes));
        tie.line = NULL;

        effect = effect_newTimedEffectNoDraw(300, -1, 0x16, 0, 0x12, 0x20, 0xFF, 1);
        if (effect != NULL) {
            memcpy(&effect->tie, &tie, sizeof(tie));
        }
    }
}

/* Per-frame update: dies when either object is gone, and respawns the ropes when the state changes. */
void rope_updateTie(RopeTie *self) {
    RopeTieData *tie;
    Actor *obj0;
    Actor *obj1;
    u8 oldState;

    obj0 = self->tie.obj0;
    obj1 = self->tie.obj1;
    tie = &self->tie;
    if ((obj0->type == 0) || (obj0->character_type == 0xFF) || (obj0->unique_id != tie->obj0Id) ||
        (obj1->type == 0) || (obj1->character_type == 0xFF) || (obj1->unique_id != tie->obj1Id)) {
        self->base.header.state = -1;
        return;
    }

    oldState = tie->state;
    tie->state = rope_getTieState(self);
    if (tie->state != oldState) {
        rope_killTieParts(self);
        if (tie->state == 1) {
            rope_tieBetweenActors(self);
        }

        if ((tie->state == 2) || (tie->state == 0)) {
            rope_tieFromActor(self);
        }
    }
}

void rope_cleanupTie(RopeTie *self) {
    rope_killTieParts(self);
}

/* Kills the ropes and the line this RopeTie owns. */
void rope_killTieParts(RopeTie *self) {
    u8 i;
    void *child;
    RopeTieData *tie = &self->tie;

    for (i = 0; i < 3; i++) {
        child = tie->ropes[i];
        if (child != NULL) {
            effect_destroyObject(child);
        }
    }
    child = tie->line;
    if (child != NULL) {
        effect_destroyObject(child);
    }
}

void rope_destroyTie(RopeTie *self) {
    rope_cleanupTie(self);
    effect_defaultDestroyTimedEffect(&self->base);
}

void rope_freeTie(RopeTie *self) {
    rope_cleanupTie(self);
    effect_defaultFreeTimedEffect(&self->base);
}

/* Returns 1 when obj1->unk65 holds obj0's 1-based index in movement_obj and obj1->unk5C is 1, else 2. */
s32 rope_getTieState(RopeTie *self) {
    Actor *obj0 = self->tie.obj0;

    if (self->tie.obj1->attach_char == (obj0 - movement_obj) + 1) {
        if (self->tie.obj1->basemtx == 1) {
            return 1;
        }
    }
    return 2;
}

/* State 1: hangs three ropes between bones of obj0 and obj1. */
void rope_tieBetweenActors(RopeTie *self) {
    Actor *obj1;
    Actor *obj0;
    RopeTieData *tie;
    HoltenRopeDesc desc;
    RopeTieLink link;

    obj1 = self->tie.obj1;
    obj0 = self->tie.obj0;
    link.owner = self;
    link.slot = 2;
    link.start.target = obj1;
    link.start.targetId = obj1->unique_id;
    link.start.bone = 5;
    link.start.offset = D_800AA350;
    link.end.target = obj1;
    link.end.targetId = obj1->unique_id;
    link.end.bone = 10;
    link.end.offset = D_800AA35C;

    desc.type = 0;
    desc.flags = 0;
    desc.timer = 1000;
    desc.start.x = obj1->xpos;
    desc.start.y = obj1->ypos;
    desc.start.z = obj1->zpos;
    desc.end.x = obj1->xpos;
    desc.end.y = obj1->ypos;
    desc.end.z = obj1->zpos;
    desc.startCallback = 1;
    desc.endCallback = 1;
    desc.drawStyle = 0;
    desc.updateCallback = 3;
    desc.halfWidth = 5.0f;
    desc.length = 80.0f;
    desc.accelScale = -0.01999999955f;
    desc.lagScale = 0.7000000477f;
    tie = &self->tie;
    tie->ropes[2] = (TiedRope *)rope_createEffect(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[2] != NULL) {
        memcpy(&tie->ropes[2]->link, &link, sizeof(link));
    }

    link.slot = 1;
    link.start.target = obj0;
    link.start.targetId = obj0->unique_id;
    link.start.bone = 5;
    link.start.offset = D_800AA320;
    link.end.target = obj1;
    link.end.targetId = obj1->unique_id;
    link.end.bone = 5;
    link.end.offset = D_800AA338;
    desc.start.x = obj0->xpos;
    desc.start.y = obj0->ypos;
    desc.start.z = obj0->zpos;
    desc.end.x = obj1->xpos;
    desc.end.y = obj1->ypos;
    desc.end.z = obj1->zpos;
    desc.drawStyle = 0;
    desc.length = 170.0f;
    tie->ropes[1] = (TiedRope *)rope_createEffect(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[1] != NULL) {
        memcpy(&tie->ropes[1]->link, &link, sizeof(link));
    }

    link.slot = 0;
    link.start.target = obj0;
    link.start.targetId = obj0->unique_id;
    link.start.bone = 5;
    link.start.offset = D_800AA32C;
    link.end.target = obj1;
    link.end.targetId = obj1->unique_id;
    link.end.bone = 10;
    link.end.offset = D_800AA344;
    desc.start.x = obj0->xpos;
    desc.start.y = obj0->ypos;
    desc.start.z = obj0->zpos;
    desc.end.x = obj1->xpos;
    desc.end.y = obj1->ypos;
    desc.end.z = obj1->zpos;
    desc.drawStyle = 0;
    tie->ropes[0] = (TiedRope *)rope_createEffect(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[0] != NULL) {
        memcpy(&tie->ropes[0]->link, &link, sizeof(link));
    }
}

/* States 0 and 2: hangs two ropes and a line from obj0's bones. */
void rope_tieFromActor(RopeTie *self) {
    Actor *obj;
    RopeTieData *tie;
    HoltenRopeDesc desc;
    RopeTieLink link;
    RopeLineData lineData;

    obj = self->tie.obj0;
    link.owner = self;
    link.slot = 1;
    link.start.target = obj;
    link.start.targetId = obj->unique_id;
    link.start.bone = 5;
    link.start.offset = D_800AA320;
    link.end.target = obj;
    link.end.targetId = obj->unique_id;
    link.end.bone = 2;
    link.end.offset = D_800AA368;

    desc.type = 0;
    desc.flags = 0;
    desc.timer = 100;
    desc.start.x = obj->xpos;
    desc.start.y = obj->ypos;
    desc.start.z = obj->zpos;
    desc.end.x = obj->xpos;
    desc.end.y = obj->ypos;
    desc.end.z = obj->zpos;
    desc.startCallback = 1;
    desc.endCallback = 1;
    desc.drawStyle = 1;
    desc.updateCallback = 3;
    desc.halfWidth = 5.0f;
    desc.length = 180.0f;
    desc.accelScale = -0.01185000036f;
    desc.lagScale = 0.7130000591f;

    tie = &self->tie;
    tie->ropes[1] = (TiedRope *)rope_createEffect(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[1] != NULL) {
        memcpy(&tie->ropes[1]->link, &link, sizeof(link));
    }

    link.slot = 0;
    link.start.offset = D_800AA32C;
    link.end.offset = D_800AA374;
    desc.drawStyle = 1;
    tie->ropes[0] = (TiedRope *)rope_createEffect(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[0] != NULL) {
        memcpy(&tie->ropes[0]->link, &link, sizeof(link));
    }

    lineData.anchor.target = obj;
    lineData.anchor.targetId = obj->unique_id;
    lineData.anchor.bone = 2;
    lineData.anchor.offset = D_800AA368;
    lineData.endOffset = D_800AA374;
    lineData.halfWidth = 5.0f;
    lineData.owner = self;
    tie->line = effect_newTimedEffect(300, -1, -1, 0, 0, 0x13, sizeof(RopeLineData), 0xFF, 1);
    if (tie->line != NULL) {
        memcpy(&tie->line->line, &lineData, sizeof(lineData));
    }
}

/* Rope teardown: clears the owner's reference to this rope. */
void rope_unlinkTiedRope(TiedRope *self) {
    RopeTie *owner;

    owner = self->link.owner;
    if (owner != NULL) {
        owner->tie.ropes[self->link.slot] = NULL;
    }
}

/* Draws the line as a camera-facing textured quad between its two bone-relative points. */
Gfx *rope_drawLine(Gfx *gfx, RopeLine *self, s16 view) {
    RopeLineData *line;
    volatile u8 dead; // matching: IDO keeps the flag in its stack slot only when volatile
    u8 update;
    Vtx *vtx;
    f32 *camera;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 lenSq;
    Vec3f start;
    Vec3f end;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    MtxF *boneMtx;
    f32 scale;

    dead = 0;
    line = &self->line;
    if ((line->anchor.target->type == 0) || (line->anchor.target->unique_id != line->anchor.targetId)) {
        dead = 1;
    }
    if ((dead == 0) && (line->anchor.target->ob_matrix_ptr != NULL)) {
        particle_getViewBuffer(self->base.buffers, view, 4 * sizeof(Vtx), &vtx, NULL);
        if (vtx == NULL) {
            return gfx;
        }
        boneMtx = &line->anchor.target->ob_matrix_ptr[line->anchor.bone];
        math_transformOffset(&line->anchor.offset, &start, boneMtx);
        math_transformOffset(&line->endOffset, &end, boneMtx);
        update = 1;
        camera = &camera_global_Cameras[view].eye.x;
        gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x60600);
        gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[54], 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
        gfx = math_gfxSetEnvColor(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
        gfx = math_gfxSetPrimColor(gfx, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, &update);
        gfx = particle_setCombineMode(gfx, 0x2B, &update);
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[5].cycle1 | gEffectRenderModes[5].cycle2, &update);
        // side = (end - start) x (midpoint - camera), scaled to halfWidth; texture is 30x30 texels (S10.5)
        dx = end.x - start.x;
        toCamX = ((dx * 0.5f) + start.x) - camera[0];
        dy = end.y - start.y;
        toCamY = ((dy * 0.5f) + start.y) - camera[1];
        dz = end.z - start.z;
        toCamZ = ((dz * 0.5f) + start.z) - camera[2];
        sideX = (dy * toCamZ) - (toCamY * dz);
        sideY = (dz * toCamX) - (toCamZ * dx);
        sideZ = (dx * toCamY) - (toCamX * dy);
        lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
        if (lenSq == 0.0f) {
            sideX = 0.0f;
            sideY = 0.0f;
            sideZ = 0.0f;
        } else {
            scale = line->halfWidth / sqrtf(lenSq);
            sideX = sideX * scale;
            sideY = sideY * scale;
            sideZ = sideZ * scale;
        }
        vtx->v.ob[0] = end.x + sideX;
        vtx->v.ob[1] = end.y + sideY;
        vtx->v.ob[2] = end.z + sideZ;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = end.x - sideX;
        vtx->v.ob[1] = end.y - sideY;
        vtx->v.ob[2] = end.z - sideZ;
        vtx->v.tc[0] = 30 << 5;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = start.x - sideX;
        vtx->v.ob[1] = start.y - sideY;
        vtx->v.ob[2] = start.z - sideZ;
        vtx->v.tc[0] = 30 << 5;
        vtx->v.tc[1] = 30 << 5;
        vtx++;
        vtx->v.ob[0] = start.x + sideX;
        vtx->v.ob[1] = start.y + sideY;
        vtx->v.ob[2] = start.z + sideZ;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = 30 << 5;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    if (dead != 0) {
        self->base.header.state = -1;
    }
    return gfx;
}

/* Line event handler: lets effect_handleOwnerActorEvent retarget the anchor object. */
void rope_lineMessage(RopeLine *self, s32 arg1, u8 event) {
    effect_handleOwnerActorEvent(arg1, event, &self->line.anchor.target, &self->line.anchor.targetId, self);
}

/* RopeTie event handler: on event 0x2D, swaps obj0/obj1 for the other object of the pair. */
void rope_tieMessage(RopeTie *self, ActorSwapMsg *swap, u8 event) {
    RopeTieData *tie = &self->tie;

    if (event == 0x2D) {
        if (tie->obj0 == swap->actorA) {
            tie->obj0 = swap->actorB;
            tie->obj0Id = swap->idB;
        } else if (tie->obj0 == swap->actorB) {
            tie->obj0 = swap->actorA;
            tie->obj0Id = swap->idA;
        }
        if (tie->obj1 == swap->actorA) {
            tie->obj1 = swap->actorB;
            tie->obj1Id = swap->idB;
        } else if (tie->obj1 == swap->actorB) {
            tie->obj1 = swap->actorA;
            tie->obj1Id = swap->idA;
        }
    }
}

/* Rope event handler: passes the link to rope_placeTiedEnd. */
s32 rope_tiedRopeEndpoint(TiedRope *self, Vec3f *pos, u8 isStart) {
    return rope_placeTiedEnd(&self->effect, &self->link, pos, isStart);
}

/* Line teardown: clears the owner's reference to this line. */
void rope_unlinkLine(RopeLine *self) {
    RopeTie *owner = self->line.owner;

    if (owner != NULL) {
        owner->tie.line = NULL;
    }
}

void rope_destroyLine(RopeLine *self) {
    rope_unlinkLine(self);
    effect_defaultDestroyTimedEffect(&self->base);
}

void rope_freeLine(RopeLine *self) {
    rope_unlinkLine(self);
    effect_defaultFreeTimedEffect(&self->base);
}

/* Rope event handler: lets effect_handleOwnerActorEvent retarget both anchor objects. */
void rope_tiedRopeMessage(TiedRope *self, s32 arg1, u8 event) {
    RopeTieLink *link;

    link = &self->link;
    effect_handleOwnerActorEvent(arg1, event, &link->start.target, &link->start.targetId, self);
    effect_handleOwnerActorEvent(arg1, event, &link->end.target, &link->end.targetId, self);
}
