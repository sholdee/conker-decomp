#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A trail effect attached to a character (struct127): a ring buffer of points that is drawn as a
 * ribbon, two particle emitters that follow the owner and three looping sounds driven by its state.
 */

/* variables.h declares D_800BE728 as s32[]; it holds a pointer to the per-controller OSContPad pointers. */
#define CONT_PADS (*(OSContPad ***)&D_800BE728)

/* Spawn header passed to func_15147A80; the actor keeps a copy of it at 0x10. */
typedef struct {
    struct17 pos;
    s16 unkC;
    u16 flags;      // 4: pos was found on the owner's model
    s32 unk10;
    u8 unk14;
    u8 maxPoints;   // ring buffer size
    u8 pad16[2];
    s32 unk18;
} TrailSpawn; // size 0x1C

/* Written by func_15147A80's caller into a particle object at 0x170, read by func_150D4CC4. */
typedef struct {
    s16 timer;
    f32 targetScale;
} ParticleTail;

/* A particle object returned by func_151407D0. */
typedef struct {
    u8 pad0[0x18];
    u8 kind;
    u8 pad19[0x34 - 0x19];
    struct17 pos;
    struct17 dir;
    u8 pad4C[0xC];
    s32 flags;
    u8 pad5C[0x158 - 0x5C];
    f32 scale;          // eased toward tail.targetScale (func_150D4CC4)
    u8 pad15C[0x170 - 0x15C];
    ParticleTail tail;
} Particle;

/* Per-trail state, at TrailActor.state; also the sound callbacks' user data. */
typedef struct {
    struct127 *owner;
    u8 ownerId;         // owner->unique_id, to notice the owner slot being reused
    u8 mode;            // 0, 1 or 2 (from owner speed or stick tilt); selects sounds and sizes
    u8 pad6[2];
    struct126 *unk8;    // func_1503195C(owner, 0x79 / 0x7A)
    Particle *effects[2];
    f32 width;          // trail width at the head, eased toward targetWidth each frame
    f32 targetWidth;
} TrailState; // size 0x1C

typedef struct {
    struct17 pos;
    f32 len;            // distance from the previous point
    f32 width;
} TrailPoint;

/* The actor returned by func_15147A80. */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    TrailSpawn spawn;
    s8 numPoints;       // points in the ring buffer
    s8 tail;            // oldest point
    s8 head;            // next point to write
    u8 pad2F[0x84 - 0x2F];
    Vtx *vtxBufs[4];
    TrailPoint *points;
    TrailState *state;
} TrailActor;

/* First argument of func_151407D0. */
typedef struct {
    f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
    s32 unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44;
    f32 unk48;
    s16 unk4C, unk4E, unk50, unk52;
    u8 unk54, unk55, unk56, unk57, unk58;
    s8 unk59;
} ParticleParams;

/* Third argument of func_151407D0. */
typedef struct {
    u8 kind;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 color[4];
    f32 unk14;
    f32 unk18;
    struct17 pos;
    struct17 dir;
    f32 scale[3];
    s32 flags;
    u8 unk44, unk45, unk46, unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} ParticleDesc;

extern s32 D_800A09C0[];
s32 func_150D4AE0(struct17 *pos, struct17 *dir, struct127 *owner, struct126 *arg3);
s32 func_150D5124();
Particle *func_151407D0(ParticleParams *, s32, ParticleDesc *, u8, u8, u8, u8, s8, u8, s32);
TrailActor *func_15147A80(TrailSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/* Spawns a trail on `owner` with its sounds and two particle emitters. */
void func_150D3360(struct127 *owner, u8 arg1, s32 arg2) {
    TrailSpawn spawn;
    TrailState init;
    struct126 *s126;
    struct17 dir;
    TrailState *state;

    spawn.maxPoints = 25;
    spawn.unkC = 300;
    spawn.flags = 0x12;
    spawn.unk10 = 15;
    spawn.unk14 = 1;
    init.targetWidth = 20.0f;
    init.width = 20.0f;
    init.mode = 1;
    init.effects[0] = NULL;
    init.effects[1] = NULL;
    init.owner = owner;
    init.ownerId = owner->unique_id;
    s126 = func_1503195C(owner, (owner->id == 0x48) ? 0x7A : 0x79, 0);
    if (s126 != NULL) {
        // matching: the store to init.unk8 must be scheduled inside the call's argument setup
        if (func_150D4AE0(&spawn.pos, &dir, owner, (init.unk8 = s126)) != 0) {
            spawn.flags |= 4;
        } else {
            spawn.pos.unk0 = 0.0f;
            spawn.pos.unk4 = 0.0f;
            spawn.pos.unk8 = 0.0f;
            dir.unk0 = 0.0f;
            dir.unk8 = 0.0f;
            dir.unk4 = 500.0f;
        }
        // matching: one local holds the actor and then its state (a separate actor local adds a stack slot)
        state = (TrailState *)func_15147A80(&spawn, 0x20, 0x14, 0, 0xD, 0xD, 0, 0, 0, arg1, arg2);
        if (state != NULL) {
            state = ((TrailActor *)state)->state;
            memcpy(state, &init, sizeof(TrailState));
            if (D_8008FD8C < 5) {
                func_1000FA64(0x5B3, state->owner->x_position, state->owner->y_position, state->owner->z_position, 1000, 1000, 200, (s32)func_150D5124, state, 0x5B3, 12, 0);
            }
            func_1000FA64(0x5B4, state->owner->x_position, state->owner->y_position, state->owner->z_position, 1000, 1000, 200, (s32)func_150D5124, state, 0x5B4, 12, 0);
            if (D_8008FD8C < 7 || owner->camera != NULL) {
                func_1000FA64(0x5BC, state->owner->x_position, state->owner->y_position, state->owner->z_position, 1000, 1000, 200, (s32)func_150D5124, state, 0x5BC, 12, 0);
            }
            {
                ParticleParams params;
                ParticleDesc desc;

                params.unk0 = 200.0f;
                params.unk4 = 40.0f;
                params.unk8 = 40.0f;
                params.unkC = 40.0f;
                params.unk10 = 0.05000000075f;
                params.unk14 = 0.05000000075f;
                params.unk18 = 0.4499999881f;
                params.unk1C = 0.3000000119f;
                params.unk20 = 9.999999975e-07f;
                params.unk24 = -1;
                params.unk34 = -1;
                params.unk28 = -1;
                params.unk38 = -1;
                params.unk2C = -1;
                params.unk3C = -1;
                params.unk30 = -1;
                params.unk40 = -1;
                params.unk44 = 0;
                params.unk48 = 1.0f;
                params.unk4C = 0;
                params.unk4E = 0;
                params.unk50 = 0;
                params.unk52 = 0;
                params.unk54 = 0;
                params.unk55 = 0;
                params.unk56 = 0;
                params.unk57 = 0;
                params.unk58 = 0;
                params.unk59 = -1;
                desc.kind = 0x15;
                desc.unk1 = 3;
                desc.unk2 = 0x5503;
                desc.unk4 = 300;
                desc.unk8 = 0;
                desc.unkC = 0;
                desc.color[0] = 0xFF;
                desc.color[1] = 0xFF;
                desc.color[2] = 0xFF;
                desc.color[3] = 0xFF;
                desc.unk14 = 100.0f;
                desc.unk18 = 100.0f;
                desc.pos = spawn.pos;
                desc.dir = dir;
                desc.scale[0] = 1.0f;
                desc.scale[1] = 1.0f;
                desc.scale[2] = 1.0f;
                desc.flags = 0xCD2006;
                desc.unk44 = 0xFF;
                desc.unk45 = 0xFF;
                desc.unk46 = 0;
                desc.unk47 = 6;
                desc.unk48 = 0;
                desc.unk4C = 0xFF;
                state->effects[0] = func_151407D0(&params, 0x60, &desc, 0, 0, 0, 0, -1, arg1, arg2);
            }
            {
                ParticleParams params;
                ParticleDesc desc;
                ParticleTail tail;

                tail.timer = 0;
                tail.targetScale = 1.0f;
                params.unk0 = params.unk4 = 45.0f;
                params.unk8 = params.unkC = 25.5f;
                params.unk14 = params.unk10 = 0.1000000015f;
                params.unk18 = 1.0f;
                params.unk24 = -1;
                params.unk34 = -1;
                params.unk28 = -1;
                params.unk38 = -1;
                params.unk2C = -1;
                params.unk3C = -1;
                params.unk30 = -1;
                params.unk40 = -1;
                params.unk44 = 0;
                params.unk48 = 1.0f;
                params.unk4C = 0;
                params.unk4E = 0;
                params.unk50 = 0;
                params.unk52 = 0;
                params.unk54 = 0;
                params.unk55 = 0;
                params.unk56 = 0;
                params.unk57 = 0;
                params.unk58 = 0;
                params.unk59 = -1;
                params.unk1C = 0.8000000119f;
                params.unk20 = 9.999999975e-07f;
                // matching: indexing D_800A09C0[...] costs IDO an extra 8-byte stack slot
                desc.kind = *(D_800A09C0 + (func_150ADA20() & 3));
                desc.unk1 = 3;
                desc.unk2 = 0x2203;
                desc.unk4 = 300;
                desc.unk8 = 0;
                desc.unkC = 0;
                desc.color[0] = 0xFF;
                desc.color[1] = 0xFF;
                desc.color[2] = 0xFF;
                desc.color[3] = 0xFF;
                desc.unk14 = 100.0f;
                desc.unk18 = 100.0f;
                desc.pos = spawn.pos;
                desc.dir = dir;
                desc.flags = 0xCD2006;
                desc.unk44 = 0xFF;
                desc.unk45 = 0xFF;
                desc.unk46 = 0;
                desc.unk47 = 7;
                desc.unk48 = 0;
                desc.unk4C = 0xFF;
                desc.scale[0] = 1.0f;
                desc.scale[1] = 1.0f;
                desc.scale[2] = 1.0f;
                state->effects[1] = func_151407D0(&params, 0x68, &desc, 0, 0x27, 0, 0, -1, arg1, arg2);
                if (state->effects[1] != NULL) {
                    memcpy(&state->effects[1]->tail, &tail, sizeof(ParticleTail));
                }
            }
        }
    }
}

void func_150D4D58(struct17 *pos, struct17 *vel, f32 angle, u8 arg3, s32 arg4);
extern f32 func_15143E64(struct17 *);
extern s32 func_1515C0F8(struct127 *, struct17 **);

/*
 * Per-frame update: appends the owner's current model point to the ring buffer, aims the emitters
 * at it and picks the mode from the owner's speed (or a player's stick tilt).
 */
s32 func_150D3A68(TrailActor *actor) {
    TrailState *state;
    TrailPoint *points;
    struct127 *owner;
    struct126 *s126;
    struct17 prevPos;
    struct17 delta;
    f32 dist;
    struct17 dir;
    ParticleTail *tail;
    u8 prevMode;
    s32 player;

    state = actor->state;
    points = actor->points;
    owner = state->owner;
    s126 = state->unk8;
    if (owner->interaction_state == 0 || owner->unique_id != state->ownerId) {
        return 0;
    }
    if (D_800BE9E4 > 0) {
        prevPos = actor->spawn.pos;
        switch (func_150D4AE0(&actor->spawn.pos, &dir, owner, s126)) {
        case 0:
            if (state->effects[0] != NULL) {
                state->effects[0]->flags &= ~2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->flags &= ~2;
            }
            actor->spawn.flags &= ~4;
            return 1;
        case 1:
            if (!(actor->spawn.flags & 4)) {
                prevPos = actor->spawn.pos;
            }
            actor->spawn.flags |= 4;
            if (state->effects[0] != NULL) {
                state->effects[0]->pos = actor->spawn.pos;
                state->effects[0]->dir = dir;
                state->effects[0]->flags &= ~4;
                state->effects[0]->flags |= 2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->pos = actor->spawn.pos;
                state->effects[1]->dir = dir;
                state->effects[1]->flags &= ~4;
                state->effects[1]->flags |= 2;
            }
            break;
        case 2:
            if (!(actor->spawn.flags & 4)) {
                prevPos = actor->spawn.pos;
            }
            actor->spawn.flags |= 4;
            if (state->effects[0] != NULL) {
                state->effects[0]->flags &= ~2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->flags &= ~2;
            }
            break;
        }
        delta.unk0 = actor->spawn.pos.unk0 - prevPos.unk0;
        delta.unk4 = actor->spawn.pos.unk4 - prevPos.unk4;
        delta.unk8 = actor->spawn.pos.unk8 - prevPos.unk8;
        dist = func_15143E64(&delta);
        {
            TrailPoint *pt = &points[actor->head];

            pt->pos = actor->spawn.pos;
            pt->len = dist;
            pt->width = 100.0f;
            actor->head++;
            if (actor->head == actor->spawn.maxPoints) {
                actor->head = 0;
            }
            actor->numPoints++;
            if (actor->head == actor->tail) {
                actor->tail++;
                if (actor->tail == actor->spawn.maxPoints) {
                    actor->tail = 0;
                }
                actor->numPoints--;
            }
        }
        if (owner->unk31C != NULL) {
            tail = (state->effects[1] != NULL) ? &state->effects[1]->tail : NULL;
            prevMode = state->mode;
            if (owner->unk31C->unk84 != 0) {
                if (owner->unk44 > 6.0f) {
                    state->mode = 0;
                } else if (owner->unk44 > 3.0f) {
                    state->mode = 1;
                } else {
                    state->mode = 2;
                }
            } else {
                player = (u8)(owner - D_800CC2D0);
                if (player >= 4) {
                    state->mode = 1;
                } else {
                    if (CONT_PADS[player]->stick_y > 20) {
                        state->mode = 0;
                    } else if (CONT_PADS[player]->stick_y < -20) {
                        state->mode = 2;
                    } else {
                        state->mode = 1;
                    }
                }
            }
            if (state->mode == 0) {
                if (tail != NULL) {
                    tail->targetScale = 2.099999905f;
                }
                state->targetWidth = 50.0f;
                if (prevMode != state->mode) {
                    struct17 *vel;

                    if (func_1515C0F8(owner, &vel) == 0) {
                        vel = (struct17 *)&D_800A5480;
                    }
                    func_150D4D58(&actor->spawn.pos, vel, owner->unk40, actor->unkC, actor->unk1);
                }
            } else if (state->mode == 2) {
                if (tail != NULL) {
                    tail->targetScale = 0.200000003f;
                }
                state->targetWidth = 10.0f;
                // matching: an empty mode-change check; removing it changes the code layout
                if (prevMode != state->mode) {
                }
            } else {
                state->targetWidth = 20.0f;
                if (tail != NULL) {
                    tail->targetScale = 1.0f;
                }
            }
        }
    }
    state->width += (state->targetWidth - state->width) * 0.3500000238f;
    return 1;
}

/*
 * Trims the trail to a length that grows with the owner's speed, then tapers the point widths from
 * state->width at the head to 0 at the tail.
 */
s32 func_150D3FD4(TrailActor *actor) {
    TrailState *state;
    TrailPoint *points;
    f32 total;
    f32 invTotal;
    struct17 *vel;
    f32 speed;
    f32 maxLen;
    f32 t;
    s32 i;
    s32 prev;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 scale;
    f32 rem;
    s32 j;

    state = actor->state;
    points = actor->points;
    if (D_800C35EA != 1) {
        if (func_1515C0F8(state->owner, &vel) != 0) {
            speed = func_15143E64(vel);
        } else {
            speed = 0.0f;
        }
    } else if (D_800DBFF0->unk5F0 & 4) {
        speed = 0.0f;
    } else {
        speed = 128.0f;
    }
    if (speed < 10.0f) {
        maxLen = 0.0f;
    } else if (speed > 70.0f) {
        maxLen = 600.0f;
    } else {
        maxLen = (speed - 10.0f) * 0.01666666754f * 600.0f;
    }
    if (actor->numPoints >= 2) {
        total = 0.0f;
        i = actor->head - 1;
        if (i < 0) {
            i = actor->spawn.maxPoints - 1;
        }
        while (i != actor->tail) {
            prev = i;
            i--;
            if (i < 0) {
                i = actor->spawn.maxPoints - 1;
            }
            // matching: `prev + points` (not points[prev]) keeps IDO's index-add operand order
            total += (prev + points)->len;
            if (maxLen < total) {
                if ((prev + points)->len) {
                    TrailPoint *q;

                    q = points + i;
                    rem = q->pos.unk0;
                    t = (total - maxLen) / (prev + points)->len;
                    dx = rem - (prev + points)->pos.unk0;
                    dy = q->pos.unk4 - (prev + points)->pos.unk4;
                    dz = q->pos.unk8 - (prev + points)->pos.unk8;
                    q->pos.unk0 = rem - dx * t;
                    q->pos.unk4 = q->pos.unk4 - dy * t;
                    q->pos.unk8 = q->pos.unk8 - dz * t;
                    (prev + points)->len = (prev + points)->len * (1.0f - t);
                }
                while (i != actor->tail) {
                    actor->tail++;
                    if (actor->tail == actor->spawn.maxPoints) {
                        actor->tail = 0;
                    }
                    actor->numPoints--;
                }
                total = maxLen;
            }
        }
        if (total != 0.0f) {
            invTotal = 1.0f / total;
        } else {
            invTotal = 0.0f;
        }
        // matching: a dead early copy that steers register allocation (rem is set again below)
        rem = total;
    }
    if (actor->numPoints >= 2) {
        scale = state->width * invTotal;
        j = actor->head;
        rem = total;
        do {
            TrailPoint *pt;
            j--;
            if (j < 0) {
                j = actor->spawn.maxPoints - 1;
            }
            pt = &points[j];
            pt->width = rem * scale;
            rem -= pt->len;
        } while (j != actor->tail);
    }
    return 1;
}

/* A ribbon segment: three vertices (edge, centre, edge) per trail point. */
typedef struct {
    Vtx v[3];
} TrailVtx;

void func_151D5D60(void *, s16, s32, void **, u8 *);
extern struct17 *func_15144B34(s32);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern u8 D_80091100[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_150D4300(TrailActor *actor, Gfx *gfx, s16 frame) {
    TrailPoint *table;
    struct17 b;
    struct17 a;
    TrailState *state;
    struct127 *obj;
    struct17 *pos;
    TrailVtx *v;
    s32 i;
    s32 idxA;
    s32 idxB;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ey;
    f32 ez;
    f32 len;
    u8 update;
    f32 wB;
    f32 wA;
    f32 cx;
    Vtx *vtx;
    u8 fresh;
    f32 cy;
    f32 cz;
    f32 scale;
    s32 ix;
    s32 iy;
    s32 iz;
    s32 nx;
    s32 ny;
    s32 nz;
    s32 pad1;
    s32 pad2;
    s32 pad3;

    if (actor->numPoints < 2) {
        return gfx;
    }
    state = actor->state;
    table = actor->points;
    obj = state->owner;
    if (*(s32 *)((u8 *)obj + 0x1D4) == 0 || (obj->unk74 & 0xF) == 0xF) {
        return gfx;
    }
    func_151D5D60(actor->vtxBufs, frame, (actor->spawn.maxPoints * 3 * sizeof(Vtx)) + (5 * 3 * sizeof(Vtx)), (void **)&vtx, &fresh);
    if (fresh != 0) {
        v = (TrailVtx *)actor->vtxBufs[frame];
        for (i = 0; i < actor->spawn.maxPoints * 2 + 10; i++) {
            v->v[0].v.tc[0] = 5;
            v->v[0].v.tc[1] = 0;
            v->v[0].v.flag = 0;
            v->v[1].v.tc[0] = 0xFFA;
            v->v[1].v.tc[1] = 0;
            v->v[1].v.flag = 0;
            v->v[2].v.tc[0] = 0x1FA4;
            v->v[2].v.tc[1] = 0;
            v->v[2].v.flag = 0;
            v++;
        }
    }
    pos = func_15144B34(frame);
    update = 1;
    gfx = func_15142E24(gfx, (s32)D_80091100, 0, 0, 0, 0, 0x78, 0, 0, &update, 0x3E);
    gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x55, &update);
    gfx = func_15142FBC(gfx, G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
    idxA = actor->head - 1;
    if (idxA < 0) {
        idxA = actor->spawn.maxPoints - 1;
    }
    idxB = idxA - 1;
    if (idxB < 0) {
        idxB = actor->spawn.maxPoints - 1;
    }
    a = table[idxA].pos;
    wA = table[idxA].width;
    b = table[idxB].pos;
    wB = table[idxB].width;

    ex = a.unk0 - b.unk0;
    ey = a.unk4 - b.unk4;
    ez = a.unk8 - b.unk8;
    dx = a.unk0 - pos->unk0;
    dy = a.unk4 - pos->unk4;
    dz = a.unk8 - pos->unk8;
    cx = ey * dz - dy * ez;
    cy = ez * dx - dz * ex;
    cz = ex * dy - dx * ey;
    len = cx * cx + cy * cy + cz * cz;
    if (len != 0.0f) {
        f32 sc;

        sc = wA / sqrtf(len);
        scale = sc;
        cx = cx * scale;
        cy = cy * scale;
        cz = cz * scale;
    } else {
        cx = 0.0f; cy = 0.0f; cz = 0.0f;
    }
    {
        s32 ix;

        ix = a.unk0;
        iy = a.unk4;
        iz = a.unk8;
        nx = cx;
        ny = cy;
        nz = cz;
        vtx->v.ob[0] = ix + nx;
        vtx->v.ob[1] = iy + ny;
        vtx->v.ob[2] = iz + nz;
        vtx++;
        vtx->v.ob[0] = ix;
        vtx->v.ob[1] = iy;
        vtx->v.ob[2] = iz;
        vtx++;
        vtx->v.ob[0] = ix - nx;
        vtx->v.ob[1] = iy - ny;
        vtx->v.ob[2] = iz - nz;
        vtx++;
    }
loop:
        ex = a.unk0 - b.unk0;
        ey = a.unk4 - b.unk4;
        ez = a.unk8 - b.unk8;
        dx = b.unk0 - pos->unk0;
        dy = b.unk4 - pos->unk4;
        dz = b.unk8 - pos->unk8;
        cx = ey * dz - dy * ez;
        cy = ez * dx - dz * ex;
        cz = ex * dy - dx * ey;
        len = cx * cx + cy * cy + cz * cz;
        if (len != 0.0f) {
            s32 pad;

            scale = wB / sqrtf(len);
            cx = cx * scale;
            cy = cy * scale;
            cz = cz * scale;
        } else {
            cx = 0.0f; cy = 0.0f; cz = 0.0f;
        }
        {
            ix = b.unk0;
            iy = b.unk4;
            iz = b.unk8;
            nx = cx;
            ny = cy;
            nz = cz;
            vtx->v.ob[0] = ix + nx;
            vtx->v.ob[1] = iy + ny;
            vtx->v.ob[2] = iz + nz;
            vtx++;
            vtx->v.ob[0] = ix;
            vtx->v.ob[1] = iy;
            vtx->v.ob[2] = iz;
            vtx++;
            vtx->v.ob[0] = ix - nx;
            vtx->v.ob[1] = iy - ny;
            vtx->v.ob[2] = iz - nz;
            vtx++;
        }
        gSPVertex(gfx++, vtx - 6, 6, 0);
        gSP2Triangles(gfx++, 0, 1, 3, 0, 1, 4, 3, 0);
        gSP2Triangles(gfx++, 2, 1, 5, 0, 1, 4, 5, 0);
        idxA = idxB;
        idxB--;
        if (idxB < 0) {
            idxB = actor->spawn.maxPoints - 1;
        }
        if (idxA != actor->tail) {
            a = table[idxA].pos;
            b = table[idxB].pos;
            wB = table[idxB].width;
        }
        if (idxA != actor->tail) goto loop;
    return gfx;
}

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    struct126 *unk8;
} SubA_150D49C0;

typedef struct {
    struct127 *unk0;
    union {
        struct127 *w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150D49C0;

void func_150D49C0(void *arg0, s32 arg1, u8 arg2) {
    SubA_150D49C0 *temp_v1 = *(SubA_150D49C0 **)((u8 *)arg0 + 0x98);
    ArgB_150D49C0 *b = (ArgB_150D49C0 *)arg1;
    struct127 *temp;

    if (arg2 == 0 || arg2 == 0x2E || arg2 == 3) {
        temp = b->unk0;
        if ((temp == temp_v1->unk0) || (b->u4.b4 == temp_v1->unk4)) {
            func_1516972C(arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v1->unk0 == b->unk0) {
            temp_v1->unk0 = b->u4.w4;
            temp_v1->unk4 = b->unk9;
            temp_v1->unk8 = func_1503195C(temp_v1->unk0, (temp_v1->unk0->id == 0x48) ? 0x7A : 0x79, 0);
        } else if (temp_v1->unk0 == b->u4.w4) {
            temp_v1->unk0 = b->unk0;
            temp_v1->unk4 = b->unk8;
            temp_v1->unk8 = func_1503195C(temp_v1->unk0, (temp_v1->unk0->id == 0x48) ? 0x7A : 0x79, 0);
        }
    }
}

extern s32 func_1514654C(struct127 *, struct126 *, s32, void *, void *, s32);
extern f32 D_800A09D0;
extern f32 D_800A09DC;

s32 func_150D4AE0(struct17 *arg0, struct17 *arg1, struct127 *arg2, struct126 *arg3) {
    void *sp30[2];
    void *sp28[2];

    if (arg2->unk1D4 != 0) {
        sp30[0] = &D_800A09D0;
        sp30[1] = &D_800A09DC;
        sp28[0] = arg0;
        sp28[1] = (void *)arg1;
        if (func_1514654C(arg2, arg3, 0, sp30, sp28, 2) != 0) {
            return 1;
        }
        return 0;
    }

    arg0->unk0 = arg2->x_position;
    arg0->unk4 = arg2->y_position + 100.0f;
    arg0->unk8 = arg2->z_position;
    arg1->unk0 = arg2->x_position;
    arg1->unk4 = arg2->y_position + 100.0f + 500.0f;
    arg1->unk8 = arg2->z_position;
    return 2;
}

extern void func_150D4C2C(void *);
extern void func_151478F4(void *);
extern void func_15147928(void *);

void func_150D4BD4(void *arg0) {
    func_150D4C2C(arg0);
    func_151478F4(arg0);
}

void func_150D4C00(void *arg0) {
    func_150D4C2C(arg0);
    func_15147928(arg0);
}

extern void func_1000FD38(void *, void *, s32);
typedef struct {
    struct127 *unk0;
    u8 pad4;
    u8 unk5;
} Src150D5124;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[8];
    Src150D5124 *unk18;
    s32 unk1C;
    s16 unk20;
    u8 pad22[2];
    u16 unk24;
} Snd150D5124;

s32 func_150D5124(Snd150D5124 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 *arg4,
                  s32 arg5, u16 *arg6);
extern s8 D_8008FD8C;

void func_150D4C2C(void *arg0) {
    struct102 **s0;

    s0 = *(struct102 ***)((u8 *)arg0 + 0x98);
    if (D_8008FD8C < 5) {
        func_1000FD38(func_150D5124, s0, 0x5B3);
    }
    func_1000FD38(func_150D5124, s0, 0x5B4);
    func_1000FD38(func_150D5124, s0, 0x5BC);
    if (s0[3] != 0) {
        func_1516972C(s0[3]);
    }
    if (s0[4] != 0) {
        func_1516972C(s0[4]);
    }
}

struct Bar150D4CC4 {
    char pad48[0x48];
    f32 unk48;
};

struct Obj150D4CC4 {
    char pad18[0x18];
    u8 unk18;
    char pad19[0x170 - 0x19];
    s16 unk170;
    char pad172[0x2];
    f32 unk174;
};

extern s32 D_800A09C0[];
extern f32 D_800A0A14;

s32 func_150D4CC4(struct Obj150D4CC4 *arg0) {
    struct Bar150D4CC4 *v1 = (struct Bar150D4CC4 *)((char *)arg0 + 0x110);

    arg0->unk170 -= D_800BE9E4;
    if (arg0->unk170 < 0) {
        arg0->unk18 = D_800A09C0[func_150ADA20() & 3];
        arg0->unk170 = (func_150ADA20() & 7) + 3;
    }

    v1->unk48 = v1->unk48 + (arg0->unk174 - v1->unk48) * D_800A0A14;
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Func150D4D58Spawn;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);

void func_150D4D58(struct17 *arg0, struct17 *arg1, f32 arg2, u8 arg3, s32 arg4) {
    Func150D4D58Spawn spawn;
    s32 count;
    s16 angle;
    f32 scale;
    s32 r1;
    s32 r2;

    count = func_150ADA20() % 11U + 10;
    spawn.unk8 = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk30 = *arg0;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.unk1E = 0xA;
    spawn.unk20 = 0x19;
    spawn.unk22 = 1;
    spawn.unk58 = 0x4C207;
    spawn.unk60 = 0;
    spawn.unk61 = 7;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    angle = (s16)(arg2 * 182.0444489f);
    angle >>= 8;
    angle += 0x40;
    spawn.unk24 = 1.0f;
    do {
        scale = func_150ADA68() * 0.2230000049f + 0.8050000668f;
        spawn.unk1D = *(D_800A09C0 + (func_150ADA20() & 3));
        spawn.unkA = func_150ADA20() % 18U + 27;
        spawn.unk1B = func_150ADA20() % 156U + 100;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 233.0f + 199.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 % 130U + angle, r2 % 81U - 64, func_150ADA68() * 10.0f + 5.0f, &spawn.unk48.unk0);
        spawn.unk48.unk0 -= arg1->unk0 * scale;
        spawn.unk48.unk8 -= arg1->unk8 * scale;
        spawn.unk54 = func_150ADA68() * 0.6460000277f + -0.7940000296f;
        spawn.unk58 &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.unk58 |= r2 | r1;
        func_15130280(&spawn, 1, 0, 0, arg3, arg4);
        count--;
    } while (count > 0);
}

extern void func_100111C8(u16);

s32 func_150D5124(Snd150D5124 *arg0, s32 arg1, u32 *arg2, s32 arg3, s32 *arg4,
                  s32 arg5, u16 *arg6) {
    Src150D5124 *src;
    s32 volume;
    s32 pitch;
    s32 targetVolume;
    s32 targetPitch;

    targetVolume = 0;
    targetPitch = 0;
    src = arg0->unk18;
    volume = arg0->unkC;
    pitch = *arg4;

    if (arg0->unk1C == 0x5B3) {
        if (src->unk0 != 0) {
            targetVolume = src->unk0->unk28 * 25.0f;
            if (targetVolume > 5000) {
                targetVolume = 5000;
            } else if (targetVolume < 500) {
                targetVolume = 0;
            }
            targetPitch += src->unk0->unk28 * 5.0f;
            if (targetPitch > 1000) {
                targetPitch = 1000;
            }
        }
        if (src->unk5 == 2) {
            targetVolume += 7000;
        } else if (src->unk5 == 1) {
            targetPitch = 400;
            targetVolume += 7000;
        } else {
            targetPitch = 400;
            if (targetVolume != 0) {
                volume = targetVolume;
            }
        }
    } else if (arg0->unk1C == 0x5BC) {
        targetVolume = 24000;
        if (src->unk5 == 0) {
            volume = targetVolume;
        } else {
            targetVolume = 400;
        }
    } else if (arg0->unk1C == 0x5B4) {
        if (src->unk5 == 0) {
            targetVolume = 16000;
            targetPitch = 400;
            volume = targetVolume;
        } else if ((src->unk5 == 1) || (D_8008FD8C >= 5)) {
            targetVolume = 12000;
        } else {
            targetPitch = 400;
        }
    }

    if (volume != targetVolume) {
        if (volume < targetVolume) {
            volume += D_800BE9E4 * 256;
            if (volume > targetVolume) {
                volume = targetVolume;
            }
        } else {
            volume -= D_800BE9E4 * 256;
            if (volume < targetVolume) {
                volume = targetVolume;
            }
        }
    }
    if (pitch != targetPitch) {
        if (pitch < targetPitch) {
            pitch += D_800BE9E4 * 256;
            if (pitch > targetPitch) {
                pitch = targetPitch;
            }
        } else {
            pitch -= D_800BE9E4 * 8;
            if (pitch < targetPitch) {
                pitch = targetPitch;
            }
        }
    }

    if (volume < 500) {
        volume = 0;
    } else if (*arg6 == 0) {
        arg0->unk0 = arg0->unk1C;
        *arg6 = arg0->unk1C;
    }

    arg0->unk2 = src->unk0->x_position;
    arg0->unk4 = src->unk0->y_position;
    arg0->unk6 = src->unk0->z_position;

    if (D_800C35EA == 1) {
        arg0->unkC = volume >> 1;
        *arg2 = *arg2 >> 1;
    } else {
        arg0->unkC = volume;
    }
    *arg4 = arg0->unk20 = pitch;

    if (*arg2 == 0) {
        if (arg0->unk24 != 0) {
            func_100111C8(arg0->unk24);
            arg0->unk24 = 0;
        }
        *arg6 = 0;
    }
    return 0;
}
