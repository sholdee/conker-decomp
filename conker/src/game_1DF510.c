#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f_1DF510;

/* Template copied into a new rope (HoltenRopeEffect 0x10-0x47, see game_1E0560.c). */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 timer;
    /* 0x04 */ Vec3f_1DF510 start;
    /* 0x10 */ Vec3f_1DF510 end;
    /* 0x1C */ s8 startCallback;
    /* 0x1D */ s8 endCallback;
    /* 0x1E */ u8 drawStyle;
    /* 0x1F */ u8 pad1F;
    /* 0x20 */ f32 halfWidth;
    /* 0x24 */ s8 updateCallback;
    /* 0x25 */ u8 pad25[3];
    /* 0x28 */ f32 length;
    /* 0x2C */ f32 accelScale;
    /* 0x30 */ f32 lagScale;
    /* 0x34 */ u8 type;
} HoltenRopeDesc;

/* A point fixed relative to one of an object's bones. */
typedef struct {
    /* 0x00 */ struct127 *target;
    /* 0x04 */ u8 targetId; // target->unique_id when attached
    /* 0x05 */ u8 bone;     // index into the Mtx array at target->unk1D4
    /* 0x08 */ Vec3f_1DF510 offset;
} RopeAnchor;

typedef struct TiedRope TiedRope;
typedef struct RopeLine RopeLine;

/* Data at 0x28 of the effect that ties obj0 to obj1 with ropes. */
typedef struct {
    /* 0x00 */ struct127 *obj0;
    /* 0x04 */ u8 obj0Id;   // obj0->unique_id when attached
    /* 0x08 */ struct127 *obj1;
    /* 0x0C */ u8 obj1Id;
    /* 0x0D */ u8 state;    // last func_151B22F4 result, 0 before the first update
    /* 0x10 */ TiedRope *ropes[3];
    /* 0x1C */ RopeLine *line;
} RopeTieData;

typedef struct {
    /* 0x00 */ struct260 base;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ RopeTieData tie;
} RopeTie;

/* Data at 0x150 of each rope a RopeTie owns. */
typedef struct {
    /* 0x00 */ RopeAnchor start;
    /* 0x14 */ RopeAnchor end;
    /* 0x28 */ RopeTie *owner;
    /* 0x2C */ u8 slot; // index in owner->tie.ropes
} RopeTieLink;

struct TiedRope {
    /* 0x000 */ u8 pad0[0x150]; // HoltenRopeEffect
    /* 0x150 */ RopeTieLink link;
};

/* Data at 0x28 of the straight line effect a RopeTie owns. */
typedef struct {
    /* 0x00 */ RopeAnchor anchor; // anchor.offset is the start point
    /* 0x14 */ Vec3f_1DF510 endOffset;
    /* 0x20 */ f32 halfWidth;
    /* 0x24 */ RopeTie *owner;
} RopeLineData;

struct RopeLine {
    /* 0x00 */ struct260 base;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ RopeLineData line;
};

TiedRope *func_151B30B0(HoltenRopeDesc *desc, f32 texScale, s32 extraSize, u8 arg3, s32 arg4);
extern Vec3f_1DF510 D_800AA320;
extern Vec3f_1DF510 D_800AA32C;
extern Vec3f_1DF510 D_800AA338;
extern Vec3f_1DF510 D_800AA344;
extern Vec3f_1DF510 D_800AA350;
extern Vec3f_1DF510 D_800AA35C;
extern Vec3f_1DF510 D_800AA368;
extern Vec3f_1DF510 D_800AA374;

s32 func_151B22F4(RopeTie *self);
void func_151B222C(RopeTie *self);
void func_151B2348(RopeTie *self);
void func_151B2690(RopeTie *self);
void func_151B47D8(TiedRope *rope, RopeTieLink *link, s32 arg2, u8 event);

/* Spawns a RopeTie between `obj` and the object from func_15083E90(1). */
void func_151B2060(struct127 *obj) {
    RopeTie *effect;
    RopeTieData tie;

    if (obj != NULL) {
        tie.obj0 = obj;
        tie.obj0Id = obj->unique_id;
        tie.obj1 = func_15083E90(1);
        tie.obj1Id = 1;
        tie.state = 0;
        bzero(tie.ropes, sizeof(tie.ropes));
        tie.line = NULL;

        effect = (RopeTie *)func_151491F4(300, -1, 0x16, 0, 0x12, 0x20, 0xFF, 1);
        if (effect != NULL) {
            memcpy(&effect->tie, &tie, sizeof(tie));
        }
    }
}

/* Per-frame update: dies when either object is gone, and respawns the ropes when the state changes. */
void func_151B2100(RopeTie *self) {
    RopeTieData *tie;
    struct127 *obj0;
    struct127 *obj1;
    u8 oldState;

    obj0 = self->tie.obj0;
    obj1 = self->tie.obj1;
    tie = &self->tie;
    if ((obj0->interaction_state == 0) || (obj0->id == 0xFF) || (obj0->unique_id != tie->obj0Id) ||
        (obj1->interaction_state == 0) || (obj1->id == 0xFF) || (obj1->unique_id != tie->obj1Id)) {
        self->base.unkE = -1;
        return;
    }

    oldState = tie->state;
    tie->state = func_151B22F4(self);
    if (tie->state != oldState) {
        func_151B222C(self);
        if (tie->state == 1) {
            func_151B2348(self);
        }

        if ((tie->state == 2) || (tie->state == 0)) {
            func_151B2690(self);
        }
    }
}

void func_151B220C(RopeTie *self) {
    func_151B222C(self);
}

/* Kills the ropes and the line this RopeTie owns. */
void func_151B222C(RopeTie *self) {
    u8 i;
    void *child;
    RopeTieData *tie = &self->tie;

    for (i = 0; i < 3; i++) {
        child = tie->ropes[i];
        if (child != NULL) {
            func_1516972C(child);
        }
    }
    child = tie->line;
    if (child != NULL) {
        func_1516972C(child);
    }
}

void func_151B229C(RopeTie *self) {
    func_151B220C(self);
    func_1514933C(&self->base);
}

void func_151B22C8(RopeTie *self) {
    func_151B220C(self);
    func_15149368(&self->base);
}

/* Returns 1 when obj1->unk65 holds obj0's 1-based index in D_800CC2D0 and obj1->unk5C is 1, else 2. */
s32 func_151B22F4(RopeTie *self) {
    struct127 *obj0 = self->tie.obj0;

    if (self->tie.obj1->unk65 == (obj0 - D_800CC2D0) + 1) {
        if (self->tie.obj1->unk5C == 1) {
            return 1;
        }
    }
    return 2;
}

/* State 1: hangs three ropes between bones of obj0 and obj1. */
void func_151B2348(RopeTie *self) {
    struct127 *obj1;
    struct127 *obj0;
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
    desc.start.x = obj1->x_position;
    desc.start.y = obj1->y_position;
    desc.start.z = obj1->z_position;
    desc.end.x = obj1->x_position;
    desc.end.y = obj1->y_position;
    desc.end.z = obj1->z_position;
    desc.startCallback = 1;
    desc.endCallback = 1;
    desc.drawStyle = 0;
    desc.updateCallback = 3;
    desc.halfWidth = 5.0f;
    desc.length = 80.0f;
    desc.accelScale = -0.01999999955f;
    desc.lagScale = 0.7000000477f;
    tie = &self->tie;
    tie->ropes[2] = func_151B30B0(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
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
    desc.start.x = obj0->x_position;
    desc.start.y = obj0->y_position;
    desc.start.z = obj0->z_position;
    desc.end.x = obj1->x_position;
    desc.end.y = obj1->y_position;
    desc.end.z = obj1->z_position;
    desc.drawStyle = 0;
    desc.length = 170.0f;
    tie->ropes[1] = func_151B30B0(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
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
    desc.start.x = obj0->x_position;
    desc.start.y = obj0->y_position;
    desc.start.z = obj0->z_position;
    desc.end.x = obj1->x_position;
    desc.end.y = obj1->y_position;
    desc.end.z = obj1->z_position;
    desc.drawStyle = 0;
    tie->ropes[0] = func_151B30B0(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[0] != NULL) {
        memcpy(&tie->ropes[0]->link, &link, sizeof(link));
    }
}

/* States 0 and 2: hangs two ropes and a line from obj0's bones. */
void func_151B2690(RopeTie *self) {
    struct127 *obj;
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
    desc.start.x = obj->x_position;
    desc.start.y = obj->y_position;
    desc.start.z = obj->z_position;
    desc.end.x = obj->x_position;
    desc.end.y = obj->y_position;
    desc.end.z = obj->z_position;
    desc.startCallback = 1;
    desc.endCallback = 1;
    desc.drawStyle = 1;
    desc.updateCallback = 3;
    desc.halfWidth = 5.0f;
    desc.length = 180.0f;
    desc.accelScale = -0.01185000036f;
    desc.lagScale = 0.7130000591f;

    tie = &self->tie;
    tie->ropes[1] = func_151B30B0(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
    if (tie->ropes[1] != NULL) {
        memcpy(&tie->ropes[1]->link, &link, sizeof(link));
    }

    link.slot = 0;
    link.start.offset = D_800AA32C;
    link.end.offset = D_800AA374;
    desc.drawStyle = 1;
    tie->ropes[0] = func_151B30B0(&desc, 0.0015f, sizeof(RopeTieLink), 0xFF, 0);
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
    tie->line = (RopeLine *)func_15149130(300, -1, -1, 0, 0, 0x13, (struct37 *)sizeof(RopeLineData), 0xFF, 1);
    if (tie->line != NULL) {
        memcpy(&tie->line->line, &lineData, sizeof(lineData));
    }
}

/* Rope teardown: clears the owner's reference to this rope. */
void func_151B2950(TiedRope *self) {
    RopeTie *owner;

    owner = self->link.owner;
    if (owner != NULL) {
        owner->tie.ropes[self->link.slot] = NULL;
    }
}

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern void func_15143134(Vec3f_1DF510 *in, Vec3f_1DF510 *out, Mtx *mtx);
extern void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090DE8[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/* Draws the line as a camera-facing textured quad between its two bone-relative points. */
Gfx *func_151B2974(Gfx *gfx, RopeLine *self, s16 view) {
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
    Vec3f_1DF510 start;
    Vec3f_1DF510 end;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    Mtx *boneMtx;
    f32 scale;

    dead = 0;
    line = &self->line;
    if ((line->anchor.target->interaction_state == 0) || (line->anchor.target->unique_id != line->anchor.targetId)) {
        dead = 1;
    }
    if ((dead == 0) && (line->anchor.target->unk1D4 != NULL)) {
        func_151D5D60(&self->base.unk14, view, 4 * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx == NULL) {
            return gfx;
        }
        boneMtx = &((Mtx *)line->anchor.target->unk1D4)[line->anchor.bone];
        func_15143134(&line->anchor.offset, &start, boneMtx);
        func_15143134(&line->endOffset, &end, boneMtx);
        update = 1;
        camera = &D_800DBFF0[view].unk2F8;
        gfx = func_15142B7C(gfx, 0x200005, 0x60600);
        gfx = func_15142E24(gfx, (s32)D_80090DE8, 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
        gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
        gfx = func_15142CF0(gfx, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, &update);
        gfx = func_1513F4E4(gfx, 0x2B, &update);
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[5].unk0 | D_800A4AC8[5].unk4, &update);
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
        self->base.unkE = -1;
    }
    return gfx;
}

/* Line event handler: lets func_15169850 retarget the anchor object. */
void func_151B2EC4(RopeLine *self, s32 arg1, u8 event) {
    func_15169850(arg1, event, (s32)&self->line.anchor.target, (s32)&self->line.anchor.targetId, (s32)self);
}

/* Event 0x2D payload: two objects that swap places. */
typedef struct {
    /* 0x00 */ struct127 *a;
    /* 0x04 */ struct127 *b;
    /* 0x08 */ u8 aId;
    /* 0x09 */ u8 bId;
} ObjectSwap;

/* RopeTie event handler: on event 0x2D, swaps obj0/obj1 for the other object of the pair. */
void func_151B2F04(RopeTie *self, ObjectSwap *swap, u8 event) {
    RopeTieData *tie = &self->tie;

    if (event == 0x2D) {
        if (tie->obj0 == swap->a) {
            tie->obj0 = swap->b;
            tie->obj0Id = swap->bId;
        } else if (tie->obj0 == swap->b) {
            tie->obj0 = swap->a;
            tie->obj0Id = swap->aId;
        }
        if (tie->obj1 == swap->a) {
            tie->obj1 = swap->b;
            tie->obj1Id = swap->bId;
        } else if (tie->obj1 == swap->b) {
            tie->obj1 = swap->a;
            tie->obj1Id = swap->aId;
        }
    }
}

/* Rope event handler: passes the link to func_151B47D8. */
void func_151B2FA0(TiedRope *self, s32 arg1, u8 event) {
    func_151B47D8(self, &self->link, arg1, event);
}

/* Line teardown: clears the owner's reference to this line. */
void func_151B2FD0(RopeLine *self) {
    RopeTie *owner = self->line.owner;

    if (owner != NULL) {
        owner->tie.line = NULL;
    }
}

void func_151B2FE8(RopeLine *self) {
    func_151B2FD0(self);
    func_1514933C(&self->base);
}

void func_151B3014(RopeLine *self) {
    func_151B2FD0(self);
    func_15149368(&self->base);
}

/* Rope event handler: lets func_15169850 retarget both anchor objects. */
void func_151B3040(TiedRope *self, s32 arg1, u8 event) {
    RopeTieLink *link;

    link = &self->link;
    func_15169850(arg1, event, (s32)&link->start.target, (s32)&link->start.targetId, (s32)self);
    func_15169850(arg1, event, (s32)&link->end.target, (s32)&link->end.targetId, (s32)self);
}
