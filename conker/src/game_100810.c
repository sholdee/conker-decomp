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

typedef struct {
    u8 pad0[0x48];
    f32 scale;          // eased toward ParticleTail.targetScale (func_150D4CC4)
} ParticleUnk110;

/* A particle object returned by func_151407D0. */
typedef struct {
    u8 pad0[0x18];
    u8 kind;
    u8 pad19[0x34 - 0x19];
    struct17 pos;
    struct17 end;
    u8 pad4C[0xC];
    s32 flags;
    u8 pad5C[0x110 - 0x5C];
    ParticleUnk110 unk110;
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
    struct17 end;
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
s32 func_150D4AE0(struct17 *start, struct17 *end, struct127 *owner, struct126 *arg3);
s32 func_150D5124();
Particle *func_151407D0(ParticleParams *, s32, ParticleDesc *, u8, u8, u8, u8, s8, u8, s32);
TrailActor *func_15147A80(TrailSpawn *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);

/* Spawns a trail on `owner` with its sounds and two particle emitters. */
void func_150D3360(struct127 *owner, u8 arg1, s32 arg2) {
    TrailSpawn spawn;
    TrailState init;
    struct126 *s126;
    struct17 end;
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
        if (func_150D4AE0(&spawn.pos, &end, owner, (init.unk8 = s126)) != 0) {
            spawn.flags |= 4;
        } else {
            spawn.pos.unk0 = 0.0f;
            spawn.pos.unk4 = 0.0f;
            spawn.pos.unk8 = 0.0f;
            end.unk0 = 0.0f;
            end.unk8 = 0.0f;
            end.unk4 = 500.0f;
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
                desc.end = end;
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
                desc.end = end;
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
    struct17 end;
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
        switch (func_150D4AE0(&actor->spawn.pos, &end, owner, s126)) {
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
                state->effects[0]->end = end;
                state->effects[0]->flags &= ~4;
                state->effects[0]->flags |= 2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->pos = actor->spawn.pos;
                state->effects[1]->end = end;
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

void func_151D5D60(Vtx **, s16, s32, Vtx **, u8 *);
extern struct17 *func_15144B34(s32);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern u8 D_80091100[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/*
 * Draws the trail as a camera-facing ribbon: each point gets an edge, centre and edge vertex, offset
 * along (segment x view direction) by the point's width.
 */
Gfx *func_150D4300(TrailActor *actor, Gfx *gfx, s16 view) {
    TrailPoint *points;
    struct17 p1;
    struct17 p0;
    TrailState *state;
    struct127 *owner;
    struct17 *eye;
    TrailVtx *tv;
    s32 i;
    s32 cur;
    s32 next;
    f32 viewX;
    f32 viewY;
    f32 viewZ;
    f32 segX;
    f32 segY;
    f32 segZ;
    f32 lenSq;
    u8 update;
    f32 width1;
    f32 width0;
    f32 sideX;
    Vtx *vtx;
    u8 fresh;
    f32 sideY;
    f32 sideZ;
    f32 scale;
    s32 px;
    s32 py;
    s32 pz;
    s32 ox;
    s32 oy;
    s32 oz;
    s32 pad1; // matching: stack frame size
    s32 pad2;
    s32 pad3;

    if (actor->numPoints < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    owner = state->owner;
    if (owner->unk1D4 == NULL || (owner->unk74 & 0xF) == 0xF) {
        return gfx;
    }
    func_151D5D60(actor->vtxBufs, view, (actor->spawn.maxPoints * 3 * sizeof(Vtx)) + (5 * 3 * sizeof(Vtx)), &vtx, &fresh);
    if (fresh != 0) {
        tv = (TrailVtx *)actor->vtxBufs[view];
        for (i = 0; i < actor->spawn.maxPoints * 2 + 10; i++) {
            tv->v[0].v.tc[0] = 5;
            tv->v[0].v.tc[1] = 0;
            tv->v[0].v.flag = 0;
            tv->v[1].v.tc[0] = 0xFFA;
            tv->v[1].v.tc[1] = 0;
            tv->v[1].v.flag = 0;
            tv->v[2].v.tc[0] = 0x1FA4;
            tv->v[2].v.tc[1] = 0;
            tv->v[2].v.flag = 0;
            tv++;
        }
    }
    eye = func_15144B34(view);
    update = 1;
    gfx = func_15142E24(gfx, (s32)D_80091100, 0, 0, 0, 0, 0x78, 0, 0, &update, 0x3E);
    gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x55, &update);
    gfx = func_15142FBC(gfx, G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
    cur = actor->head - 1;
    if (cur < 0) {
        cur = actor->spawn.maxPoints - 1;
    }
    next = cur - 1;
    if (next < 0) {
        next = actor->spawn.maxPoints - 1;
    }
    p0 = points[cur].pos;
    width0 = points[cur].width;
    p1 = points[next].pos;
    width1 = points[next].width;

    segX = p0.unk0 - p1.unk0;
    segY = p0.unk4 - p1.unk4;
    segZ = p0.unk8 - p1.unk8;
    viewX = p0.unk0 - eye->unk0;
    viewY = p0.unk4 - eye->unk4;
    viewZ = p0.unk8 - eye->unk8;
    sideX = segY * viewZ - viewY * segZ;
    sideY = segZ * viewX - viewZ * segX;
    sideZ = segX * viewY - viewX * segY;
    lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
    if (lenSq != 0.0f) {
        f32 s; // matching: a separate temporary keeps the original register choice

        s = width0 / sqrtf(lenSq);
        scale = s;
        sideX = sideX * scale;
        sideY = sideY * scale;
        sideZ = sideZ * scale;
    } else {
        sideX = 0.0f;
        sideY = 0.0f;
        sideZ = 0.0f;
    }
    {
        s32 px; // matching: block-local copy of px steers register allocation

        px = p0.unk0;
        py = p0.unk4;
        pz = p0.unk8;
        ox = sideX;
        oy = sideY;
        oz = sideZ;
        vtx->v.ob[0] = px + ox;
        vtx->v.ob[1] = py + oy;
        vtx->v.ob[2] = pz + oz;
        vtx++;
        vtx->v.ob[0] = px;
        vtx->v.ob[1] = py;
        vtx->v.ob[2] = pz;
        vtx++;
        vtx->v.ob[0] = px - ox;
        vtx->v.ob[1] = py - oy;
        vtx->v.ob[2] = pz - oz;
        vtx++;
    }
    // matching: a goto loop; IDO optimizes a do/while or for loop here differently
loop:
    segX = p0.unk0 - p1.unk0;
    segY = p0.unk4 - p1.unk4;
    segZ = p0.unk8 - p1.unk8;
    viewX = p1.unk0 - eye->unk0;
    viewY = p1.unk4 - eye->unk4;
    viewZ = p1.unk8 - eye->unk8;
    sideX = segY * viewZ - viewY * segZ;
    sideY = segZ * viewX - viewZ * segX;
    sideZ = segX * viewY - viewX * segY;
    lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
    if (lenSq != 0.0f) {
        s32 pad; // matching: -g3 block scope that orders the scheduling

        scale = width1 / sqrtf(lenSq);
        sideX = sideX * scale;
        sideY = sideY * scale;
        sideZ = sideZ * scale;
    } else {
        sideX = 0.0f;
        sideY = 0.0f;
        sideZ = 0.0f;
    }
    {
        px = p1.unk0;
        py = p1.unk4;
        pz = p1.unk8;
        ox = sideX;
        oy = sideY;
        oz = sideZ;
        vtx->v.ob[0] = px + ox;
        vtx->v.ob[1] = py + oy;
        vtx->v.ob[2] = pz + oz;
        vtx++;
        vtx->v.ob[0] = px;
        vtx->v.ob[1] = py;
        vtx->v.ob[2] = pz;
        vtx++;
        vtx->v.ob[0] = px - ox;
        vtx->v.ob[1] = py - oy;
        vtx->v.ob[2] = pz - oz;
        vtx++;
    }
    gSPVertex(gfx++, vtx - 6, 6, 0);
    gSP2Triangles(gfx++, 0, 1, 3, 0, 1, 4, 3, 0);
    gSP2Triangles(gfx++, 2, 1, 5, 0, 1, 4, 5, 0);
    cur = next;
    next--;
    if (next < 0) {
        next = actor->spawn.maxPoints - 1;
    }
    if (cur != actor->tail) {
        p0 = points[cur].pos;
        p1 = points[next].pos;
        width1 = points[next].width;
    }
    if (cur != actor->tail) goto loop;
    return gfx;
}

/* Message data for func_150D49C0. */
typedef union {
    struct {
        struct127 *obj;
        u8 uniqueId;
    } gone;             // messages 0, 3 and 0x2E
    struct {
        struct127 *objA;
        struct127 *objB;
        u8 uniqueIdA;
        u8 uniqueIdB;
    } swap;             // message 0x2D: objA and objB trade places
} TrailMsg;

/* Message handler: destroys the trail when its owner goes away and follows the owner through a swap. */
void func_150D49C0(TrailActor *actor, TrailMsg *msg, u8 id) {
    TrailState *state = actor->state;
    struct127 *obj;

    if (id == 0 || id == 0x2E || id == 3) {
        obj = msg->gone.obj;
        if (obj == state->owner || msg->gone.uniqueId == state->ownerId) {
            func_1516972C((struct102 *)actor);
        }
    } else if (id == 0x2D) {
        if (state->owner == msg->swap.objA) {
            state->owner = msg->swap.objB;
            state->ownerId = msg->swap.uniqueIdB;
            state->unk8 = func_1503195C(state->owner, (state->owner->id == 0x48) ? 0x7A : 0x79, 0);
        } else if (state->owner == msg->swap.objB) {
            state->owner = msg->swap.objA;
            state->ownerId = msg->swap.uniqueIdA;
            state->unk8 = func_1503195C(state->owner, (state->owner->id == 0x48) ? 0x7A : 0x79, 0);
        }
    }
}

extern s32 func_1514654C(struct127 *, struct126 *, s32, struct17 **, struct17 **, s32);
extern struct17 D_800A09D0;
extern struct17 D_800A09DC;

/*
 * Computes the trail's start and end points: the owner's model points D_800A09D0 / D_800A09DC
 * (returns 1 when found, 0 when not), or 100 and 600 units above the owner when it has no model (2).
 */
s32 func_150D4AE0(struct17 *start, struct17 *end, struct127 *owner, struct126 *arg3) {
    struct17 *modelPoints[2];
    struct17 *out[2];

    if (owner->unk1D4 != NULL) {
        modelPoints[0] = &D_800A09D0;
        modelPoints[1] = &D_800A09DC;
        out[0] = start;
        out[1] = end;
        if (func_1514654C(owner, arg3, 0, modelPoints, out, 2) != 0) {
            return 1;
        }
        return 0;
    }

    start->unk0 = owner->x_position;
    start->unk4 = owner->y_position + 100.0f;
    start->unk8 = owner->z_position;
    end->unk0 = owner->x_position;
    end->unk4 = owner->y_position + 100.0f + 500.0f;
    end->unk8 = owner->z_position;
    return 2;
}

void func_150D4C2C(TrailActor *actor);
extern void func_151478F4(TrailActor *);
extern void func_15147928(TrailActor *);

void func_150D4BD4(TrailActor *actor) {
    func_150D4C2C(actor);
    func_151478F4(actor);
}

void func_150D4C00(TrailActor *actor) {
    func_150D4C2C(actor);
    func_15147928(actor);
}

extern void func_1000FD38(void *, void *, s32);

/* Sound slot passed to func_150D5124 by the sound system. */
typedef struct {
    s16 sound;
    s16 x;
    s16 y;
    s16 z;
    u8 pad8[4];
    s32 volume;
    u8 pad10[8];
    TrailState *state;  // user data given to func_1000FA64
    s32 soundId;
    s16 pitch;
    u8 pad22[2];
    u16 unk24;
} TrailSound;

s32 func_150D5124(TrailSound *snd, s32 arg1, u32 *arg2, s32 arg3, s32 *pitchOut,
                  s32 arg5, u16 *playing);

/* Stops the trail's three sounds and destroys its particle emitters. */
void func_150D4C2C(TrailActor *actor) {
    TrailState *state;

    state = actor->state;
    if (D_8008FD8C < 5) {
        func_1000FD38(func_150D5124, state, 0x5B3);
    }
    func_1000FD38(func_150D5124, state, 0x5B4);
    func_1000FD38(func_150D5124, state, 0x5BC);
    if (state->effects[0] != NULL) {
        func_1516972C((struct102 *)state->effects[0]);
    }
    if (state->effects[1] != NULL) {
        func_1516972C((struct102 *)state->effects[1]);
    }
}

/* Update for the second emitter's particles: re-rolls the sprite every few frames and eases the scale. */
s32 func_150D4CC4(Particle *particle) {
    ParticleUnk110 *sub = &particle->unk110;

    particle->tail.timer -= D_800BE9E4;
    if (particle->tail.timer < 0) {
        particle->kind = D_800A09C0[func_150ADA20() & 3];
        particle->tail.timer = (func_150ADA20() & 7) + 3;
    }

    sub->scale = sub->scale + (particle->tail.targetScale - sub->scale) * 0.6500000358f;
    return 1;
}

/* Spark description passed to func_15130280. */
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
    u8 kind;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 flags;
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
} SparkSpawn; // size 0x70

extern void *func_15130280(SparkSpawn *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);

/* Throws 10-20 sparks from `pos`, fanned around the owner's facing `angle` (degrees) and against `vel`. */
void func_150D4D58(struct17 *pos, struct17 *vel, f32 angle, u8 arg3, s32 arg4) {
    SparkSpawn spawn;
    s32 count;
    s16 yaw;
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
    spawn.pos = *pos;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.unk1E = 10;
    spawn.unk20 = 25;
    spawn.unk22 = 1;
    spawn.flags = 0x4C207;
    spawn.unk60 = 0;
    spawn.unk61 = 7;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    yaw = (s16)(angle * 182.0444489f);
    yaw >>= 8;
    yaw += 0x40;
    spawn.unk24 = 1.0f;
    do {
        scale = func_150ADA68() * 0.2230000049f + 0.8050000668f;
        // matching: indexing D_800A09C0[...] costs IDO an extra stack slot
        spawn.kind = *(D_800A09C0 + (func_150ADA20() & 3));
        spawn.unkA = func_150ADA20() % 18U + 27;
        spawn.unk1B = func_150ADA20() % 156U + 100;
        spawn.unk28 = spawn.unk2C = func_150ADA68() * 233.0f + 199.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 % 130U + yaw, r2 % 81U - 64, func_150ADA68() * 10.0f + 5.0f, &spawn.vel.unk0);
        spawn.vel.unk0 -= vel->unk0 * scale;
        spawn.vel.unk8 -= vel->unk8 * scale;
        spawn.unk54 = func_150ADA68() * 0.6460000277f + -0.7940000296f;
        spawn.flags &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.flags |= r2 | r1;
        func_15130280(&spawn, 1, 0, 0, arg3, arg4);
        count--;
    } while (count > 0);
}

extern void func_100111C8(u16);

/*
 * Sound callback for the trail's three loops (0x5B3, 0x5B4, 0x5BC): moves volume and pitch toward
 * targets picked from the trail mode, follows the owner's position and stops when silenced.
 */
s32 func_150D5124(TrailSound *snd, s32 arg1, u32 *arg2, s32 arg3, s32 *pitchOut,
                  s32 arg5, u16 *playing) {
    TrailState *state;
    s32 volume;
    s32 pitch;
    s32 targetVolume;
    s32 targetPitch;

    targetVolume = 0;
    targetPitch = 0;
    state = snd->state;
    volume = snd->volume;
    pitch = *pitchOut;

    if (snd->soundId == 0x5B3) {
        if (state->owner != NULL) {
            targetVolume = state->owner->unk28 * 25.0f;
            if (targetVolume > 5000) {
                targetVolume = 5000;
            } else if (targetVolume < 500) {
                targetVolume = 0;
            }
            targetPitch += state->owner->unk28 * 5.0f;
            if (targetPitch > 1000) {
                targetPitch = 1000;
            }
        }
        if (state->mode == 2) {
            targetVolume += 7000;
        } else if (state->mode == 1) {
            targetPitch = 400;
            targetVolume += 7000;
        } else {
            targetPitch = 400;
            if (targetVolume != 0) {
                volume = targetVolume;
            }
        }
    } else if (snd->soundId == 0x5BC) {
        targetVolume = 24000;
        if (state->mode == 0) {
            volume = targetVolume;
        } else {
            targetVolume = 400;
        }
    } else if (snd->soundId == 0x5B4) {
        if (state->mode == 0) {
            targetVolume = 16000;
            targetPitch = 400;
            volume = targetVolume;
        } else if (state->mode == 1 || D_8008FD8C >= 5) {
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
    } else if (*playing == 0) {
        snd->sound = snd->soundId;
        *playing = snd->soundId;
    }

    snd->x = state->owner->x_position;
    snd->y = state->owner->y_position;
    snd->z = state->owner->z_position;

    if (D_800C35EA == 1) {
        snd->volume = volume >> 1;
        *arg2 = *arg2 >> 1;
    } else {
        snd->volume = volume;
    }
    *pitchOut = snd->pitch = pitch;

    if (*arg2 == 0) {
        if (snd->unk24 != 0) {
            func_100111C8(snd->unk24);
            snd->unk24 = 0;
        }
        *playing = 0;
    }
    return 0;
}
