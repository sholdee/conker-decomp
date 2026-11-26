#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A trail effect attached to a character (Actor): a ring buffer of points that is drawn as a
 * ribbon, two particle emitters that follow the owner and three looping sounds driven by its state.
 */

// matching: defined below func_150D4300; with the initializer in view, IDO compiles func_150D4300 differently
static const s32 D_800A09C0[4];

/* Spawns a trail on `owner` with its sounds and two particle emitters. */
void func_150D3360(Actor *owner, u8 arg1, s32 arg2) {
    EffectTemplate spawn;
    CharacterTrailState init;
    ModelAttachment *attachment;
    Vec3f end;
    CharacterTrailState *state;

    spawn.itemCount = 25;
    spawn.life = 300;
    spawn.flags = 0x12;
    spawn.kind = 15;
    spawn.unk14 = 1;
    init.targetWidth = 20.0f;
    init.width = 20.0f;
    init.mode = 1;
    init.effects[0] = NULL;
    init.effects[1] = NULL;
    init.owner = owner;
    init.ownerId = owner->unique_id;
    attachment = func_1503195C(owner, (owner->character_type == 0x48) ? 0x7A : 0x79, 0);
    if (attachment == NULL) {
        return;
    }
    // matching: the store to init.attachment must be scheduled inside the call's argument setup
    if (func_150D4AE0(&spawn.pos, &end, owner, (init.attachment = attachment)) != 0) {
        spawn.flags |= 4;
    } else {
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
        end.x = 0.0f;
        end.z = 0.0f;
        end.y = 500.0f;
    }
    // matching: one local holds the actor and then its state (a separate actor local adds a stack slot)
    state = func_15147A80(&spawn, 0x20, 0x14, 0, 0xD, 0xD, 0, 0, 0, arg1, arg2);
    if (state == NULL) {
        return;
    }
    state = ((CharacterTrail *)state)->state;
    memcpy(state, &init, sizeof(CharacterTrailState));
    if (D_8008FD8C < 5) {
        func_1000FA64(0x5B3, state->owner->xpos, state->owner->ypos, state->owner->zpos, 1000, 1000, 200, (SoundRecordCallback)func_150D5124, state, 0x5B3, 12, 0);
    }
    func_1000FA64(0x5B4, state->owner->xpos, state->owner->ypos, state->owner->zpos, 1000, 1000, 200, (SoundRecordCallback)func_150D5124, state, 0x5B4, 12, 0);
    if (D_8008FD8C < 7 || owner->CameraPtr != NULL) {
        func_1000FA64(0x5BC, state->owner->xpos, state->owner->ypos, state->owner->zpos, 1000, 1000, 200, (SoundRecordCallback)func_150D5124, state, 0x5BC, 12, 0);
    }
    {
        GlowShape params;
        ParticleDef desc;

        params.width1 = 200.0f;
        params.height1 = 40.0f;
        params.width0 = 40.0f;
        params.height0 = 40.0f;
        params.offset1 = 0.05000000075f;
        params.offset0 = 0.05000000075f;
        params.alpha1 = 0.4499999881f;
        params.alpha0 = 0.3000000119f;
        params.fadeRate = 9.999999975e-07f;
        params.screenX[0] = -1;
        params.screenY[0] = -1;
        params.screenX[1] = -1;
        params.screenY[1] = -1;
        params.screenX[2] = -1;
        params.screenY[2] = -1;
        params.screenX[3] = -1;
        params.screenY[3] = -1;
        params.attach = 0;
        params.scale = 1.0f;
        params.zbufSample[0] = 0;
        params.zbufSample[1] = 0;
        params.zbufSample[2] = 0;
        params.zbufSample[3] = 0;
        params.visibility[0] = 0;
        params.visibility[1] = 0;
        params.visibility[2] = 0;
        params.visibility[3] = 0;
        params.type = 0;
        params.handler = -1;
        desc.texture = 0x15;
        desc.type = 3;
        desc.renderModes = 0x5503;
        desc.lifetime = 300;
        desc.frame = 0;
        desc.frameSpeed = 0;
        desc.rgba[0] = 0xFF;
        desc.rgba[1] = 0xFF;
        desc.rgba[2] = 0xFF;
        desc.rgba[3] = 0xFF;
        desc.width = 100.0f;
        desc.height = 100.0f;
        desc.pos = spawn.pos;
        desc.rot = end;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.flags = 0xCD2006;
        desc.unk44 = 0xFF;
        desc.unk45 = 0xFF;
        desc.unk46 = 0;
        desc.unk47 = 6;
        desc.unk48 = 0;
        desc.viewportMask = 0xFF;
        state->effects[0] = func_151407D0(&params, 0x60, &desc, 0, 0, 0, 0, -1, arg1, arg2);
    }
    {
        GlowShape params;
        ParticleDef desc;
        TrailGlowData tail;

        tail.timer = 0;
        tail.targetScale = 1.0f;
        params.width1 = params.height1 = 45.0f;
        params.width0 = params.height0 = 25.5f;
        params.offset0 = params.offset1 = 0.1000000015f;
        params.alpha1 = 1.0f;
        params.screenX[0] = -1;
        params.screenY[0] = -1;
        params.screenX[1] = -1;
        params.screenY[1] = -1;
        params.screenX[2] = -1;
        params.screenY[2] = -1;
        params.screenX[3] = -1;
        params.screenY[3] = -1;
        params.attach = 0;
        params.scale = 1.0f;
        params.zbufSample[0] = 0;
        params.zbufSample[1] = 0;
        params.zbufSample[2] = 0;
        params.zbufSample[3] = 0;
        params.visibility[0] = 0;
        params.visibility[1] = 0;
        params.visibility[2] = 0;
        params.visibility[3] = 0;
        params.type = 0;
        params.handler = -1;
        params.alpha0 = 0.8000000119f;
        params.fadeRate = 9.999999975e-07f;
        // matching: indexing D_800A09C0[...] costs IDO an extra 8-byte stack slot
        desc.texture = *(D_800A09C0 + (func_150ADA20() & 3));
        desc.type = 3;
        desc.renderModes = 0x2203;
        desc.lifetime = 300;
        desc.frame = 0;
        desc.frameSpeed = 0;
        desc.rgba[0] = 0xFF;
        desc.rgba[1] = 0xFF;
        desc.rgba[2] = 0xFF;
        desc.rgba[3] = 0xFF;
        desc.width = 100.0f;
        desc.height = 100.0f;
        desc.pos = spawn.pos;
        desc.rot = end;
        desc.flags = 0xCD2006;
        desc.unk44 = 0xFF;
        desc.unk45 = 0xFF;
        desc.unk46 = 0;
        desc.unk47 = 7;
        desc.unk48 = 0;
        desc.viewportMask = 0xFF;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        state->effects[1] = func_151407D0(&params, 0x68, &desc, 0, 0x27, 0, 0, -1, arg1, arg2);
        if (state->effects[1] != NULL) {
            memcpy(&state->effects[1]->tail, &tail, sizeof(TrailGlowData));
        }
    }
}

/*
 * Per-frame update: appends the owner's current model point to the ring buffer, aims the emitters
 * at it and picks the mode from the owner's speed (or a player's stick tilt).
 */
s32 func_150D3A68(CharacterTrail *actor) {
    CharacterTrailState *state;
    CharacterTrailPoint *points;
    Actor *owner;
    ModelAttachment *attachment;
    Vec3f prevPos;
    Vec3f delta;
    f32 dist;
    Vec3f end;
    TrailGlowData *tail;
    u8 prevMode;
    s32 player;

    state = actor->state;
    points = actor->points;
    owner = state->owner;
    attachment = state->attachment;
    if (owner->type == 0 || owner->unique_id != state->ownerId) {
        return 0;
    }
    if (D_800BE9E4 > 0) {
        prevPos = actor->head.tmpl.pos;
        switch (func_150D4AE0(&actor->head.tmpl.pos, &end, owner, attachment)) {
        case 0:
            if (state->effects[0] != NULL) {
                state->effects[0]->head.def.flags &= ~2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->head.def.flags &= ~2;
            }
            actor->head.tmpl.flags &= ~4;
            return 1;
        case 1:
            if (!(actor->head.tmpl.flags & 4)) {
                prevPos = actor->head.tmpl.pos;
            }
            actor->head.tmpl.flags |= 4;
            if (state->effects[0] != NULL) {
                state->effects[0]->head.def.pos = actor->head.tmpl.pos;
                state->effects[0]->head.def.rot = end;
                state->effects[0]->head.def.flags &= ~4;
                state->effects[0]->head.def.flags |= 2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->head.def.pos = actor->head.tmpl.pos;
                state->effects[1]->head.def.rot = end;
                state->effects[1]->head.def.flags &= ~4;
                state->effects[1]->head.def.flags |= 2;
            }
            break;
        case 2:
            if (!(actor->head.tmpl.flags & 4)) {
                prevPos = actor->head.tmpl.pos;
            }
            actor->head.tmpl.flags |= 4;
            if (state->effects[0] != NULL) {
                state->effects[0]->head.def.flags &= ~2;
            }
            if (state->effects[1] != NULL) {
                state->effects[1]->head.def.flags &= ~2;
            }
            break;
        }
        delta.x = actor->head.tmpl.pos.x - prevPos.x;
        delta.y = actor->head.tmpl.pos.y - prevPos.y;
        delta.z = actor->head.tmpl.pos.z - prevPos.z;
        dist = func_15143E64(&delta);
        {
            CharacterTrailPoint *pt = &points[actor->head.end];

            pt->pos = actor->head.tmpl.pos;
            pt->len = dist;
            pt->width = 100.0f;
            actor->head.end++;
            if (actor->head.end == actor->head.tmpl.itemCount) {
                actor->head.end = 0;
            }
            actor->head.count++;
            if (actor->head.end == actor->head.start) {
                actor->head.start++;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count--;
            }
        }
        if (owner->mv != NULL) {
            tail = (state->effects[1] != NULL) ? &state->effects[1]->tail : NULL;
            prevMode = state->mode;
            if (owner->mv->bot != 0) {
                if (owner->stickspeed > 6.0f) {
                    state->mode = 0;
                } else if (owner->stickspeed > 3.0f) {
                    state->mode = 1;
                } else {
                    state->mode = 2;
                }
            } else {
                player = (u8)(owner - movement_obj);
                if (player >= 4) {
                    state->mode = 1;
                } else {
                    if (D_800BE728[player]->stick_y > 20) {
                        state->mode = 0;
                    } else if (D_800BE728[player]->stick_y < -20) {
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
                    const Vec3f *vel;

                    if (func_1515C0F8(owner, &vel) == 0) {
                        vel = &D_800A5480;
                    }
                    func_150D4D58(&actor->head.tmpl.pos, vel, owner->theta, actor->head.header.owner, actor->head.header.kind);
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
s32 func_150D3FD4(CharacterTrail *actor) {
    CharacterTrailState *state;
    CharacterTrailPoint *points;
    f32 total;
    f32 invTotal;
    const Vec3f *vel;
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
    if (D_800C35EA[0] != 1) {
        if (func_1515C0F8(state->owner, &vel) != 0) {
            speed = func_15143E64(vel);
        } else {
            speed = 0.0f;
        }
    } else if (camera_global_Cameras->SpecialFlags & 4) {
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
    if (actor->head.count >= 2) {
        total = 0.0f;
        i = actor->head.end - 1;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        while (i != actor->head.start) {
            prev = i;
            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            // matching: `prev + points` (not points[prev]) keeps IDO's index-add operand order
            total += (prev + points)->len;
            if (maxLen < total) {
                if ((prev + points)->len) {
                    CharacterTrailPoint *q;

                    q = points + i;
                    rem = q->pos.x;
                    t = (total - maxLen) / (prev + points)->len;
                    dx = rem - (prev + points)->pos.x;
                    dy = q->pos.y - (prev + points)->pos.y;
                    dz = q->pos.z - (prev + points)->pos.z;
                    q->pos.x = rem - dx * t;
                    q->pos.y = q->pos.y - dy * t;
                    q->pos.z = q->pos.z - dz * t;
                    (prev + points)->len = (prev + points)->len * (1.0f - t);
                }
                while (i != actor->head.start) {
                    actor->head.start++;
                    if (actor->head.start == actor->head.tmpl.itemCount) {
                        actor->head.start = 0;
                    }
                    actor->head.count--;
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
    if (actor->head.count >= 2) {
        scale = state->width * invTotal;
        j = actor->head.end;
        rem = total;
        do {
            CharacterTrailPoint *pt;
            j--;
            if (j < 0) {
                j = actor->head.tmpl.itemCount - 1;
            }
            pt = &points[j];
            pt->width = rem * scale;
            rem -= pt->len;
        } while (j != actor->head.start);
    }
    return 1;
}

/* A ribbon segment: three vertices (edge, centre, edge) per trail point. */
typedef struct {
    Vtx v[3];
} TrailVtx;

extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/*
 * Draws the trail as a camera-facing ribbon: each point gets an edge, centre and edge vertex, offset
 * along (segment x view direction) by the point's width.
 */
Gfx *func_150D4300(CharacterTrail *actor, Gfx *gfx, s16 view) {
    CharacterTrailPoint *points;
    Vec3f p1;
    Vec3f p0;
    CharacterTrailState *state;
    Actor *owner;
    Vec3f *eye;
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

    if (actor->head.count < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    owner = state->owner;
    if (owner->ob_matrix_ptr == NULL || (owner->unk74 & 0xF) == 0xF) {
        return gfx;
    }
    func_151D5D60(actor->vtxBufs, view, (actor->head.tmpl.itemCount * 3 * sizeof(Vtx)) + (5 * 3 * sizeof(Vtx)), &vtx, &fresh);
    if (fresh != 0) {
        tv = (TrailVtx *)actor->vtxBufs[view];
        for (i = 0; i < actor->head.tmpl.itemCount * 2 + 10; i++) {
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
    gfx = func_15142E24(gfx, &D_80090B60[120], 0, 0, 0, 0, 0x78, 0, 0, &update, 0x3E);
    gfx = func_15142C10(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &update);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x55, &update);
    gfx = func_15142FBC(gfx, G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[3].unk0 | D_800A4AC8[3].unk4, &update);
    cur = actor->head.end - 1;
    if (cur < 0) {
        cur = actor->head.tmpl.itemCount - 1;
    }
    next = cur - 1;
    if (next < 0) {
        next = actor->head.tmpl.itemCount - 1;
    }
    p0 = points[cur].pos;
    width0 = points[cur].width;
    p1 = points[next].pos;
    width1 = points[next].width;

    segX = p0.x - p1.x;
    segY = p0.y - p1.y;
    segZ = p0.z - p1.z;
    viewX = p0.x - eye->x;
    viewY = p0.y - eye->y;
    viewZ = p0.z - eye->z;
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

        px = p0.x;
        py = p0.y;
        pz = p0.z;
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
    segX = p0.x - p1.x;
    segY = p0.y - p1.y;
    segZ = p0.z - p1.z;
    viewX = p1.x - eye->x;
    viewY = p1.y - eye->y;
    viewZ = p1.z - eye->z;
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
        px = p1.x;
        py = p1.y;
        pz = p1.z;
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
        next = actor->head.tmpl.itemCount - 1;
    }
    if (cur != actor->head.start) {
        p0 = points[cur].pos;
        p1 = points[next].pos;
        width1 = points[next].width;
    }
    if (cur != actor->head.start) goto loop;
    return gfx;
}

/* Message handler: destroys the trail when its owner goes away and follows the owner through a swap. */
void func_150D49C0(CharacterTrail *actor, ActorMsg *msg, u8 id) {
    CharacterTrailState *state = actor->state;
    Actor *obj;

    if (id == 0 || id == 0x2E || id == 3) {
        obj = msg->ref.actor;
        if (obj == state->owner || msg->ref.uniqueId == state->ownerId) {
            func_1516972C(&actor->head.header);
        }
    } else if (id == 0x2D) {
        if (state->owner == msg->swap.actorA) {
            state->owner = msg->swap.actorB;
            state->ownerId = msg->swap.idB;
            state->attachment = func_1503195C(state->owner, (state->owner->character_type == 0x48) ? 0x7A : 0x79, 0);
        } else if (state->owner == msg->swap.actorB) {
            state->owner = msg->swap.actorA;
            state->ownerId = msg->swap.idA;
            state->attachment = func_1503195C(state->owner, (state->owner->character_type == 0x48) ? 0x7A : 0x79, 0);
        }
    }
}

/* The four trail particle textures, one picked at random. */
static const s32 D_800A09C0[4] = { 0x60, 0x61, 0x62, 0x63 };

/* The trail's start and end points in the attachment's model space. */
static const Vec3f D_800A09D0 = { -151.7874f, -23.267801f, -20.8778f };
static const Vec3f D_800A09DC = { -283.44f, -23.267801f, -20.8778f };

/*
 * Computes the trail's start and end points: the points D_800A09D0 / D_800A09DC of `attachment`
 * (returns 1 when found, 0 when not), or 100 and 600 units above the owner when it has no model (2).
 */
s32 func_150D4AE0(Vec3f *start, Vec3f *end, Actor *owner, ModelAttachment *attachment) {
    const Vec3f *modelPoints[2];
    Vec3f *out[2];

    if (owner->ob_matrix_ptr != NULL) {
        modelPoints[0] = &D_800A09D0;
        modelPoints[1] = &D_800A09DC;
        out[0] = start;
        out[1] = end;
        if (func_1514654C(owner, attachment, 0, modelPoints, out, 2) != 0) {
            return 1;
        }
        return 0;
    }

    start->x = owner->xpos;
    start->y = owner->ypos + 100.0f;
    start->z = owner->zpos;
    end->x = owner->xpos;
    end->y = owner->ypos + 100.0f + 500.0f;
    end->z = owner->zpos;
    return 2;
}

void func_150D4BD4(CharacterTrail *actor) {
    func_150D4C2C(actor);
    func_151478F4(actor);
}

void func_150D4C00(CharacterTrail *actor) {
    func_150D4C2C(actor);
    func_15147928(actor);
}

/* Stops the trail's three sounds and destroys its particle emitters. */
void func_150D4C2C(CharacterTrail *actor) {
    CharacterTrailState *state;

    state = actor->state;
    if (D_8008FD8C < 5) {
        func_1000FD38((SoundRecordCallback)func_150D5124, state, 0x5B3);
    }
    func_1000FD38((SoundRecordCallback)func_150D5124, state, 0x5B4);
    func_1000FD38((SoundRecordCallback)func_150D5124, state, 0x5BC);
    if (state->effects[0] != NULL) {
        func_1516972C(&state->effects[0]->head.header);
    }
    if (state->effects[1] != NULL) {
        func_1516972C(&state->effects[1]->head.header);
    }
}

/* Update for the second emitter's particles: re-rolls the sprite every few frames and eases the scale. */
s32 func_150D4CC4(TrailGlow *particle) {
    GlowShape *sub = &particle->shape;

    particle->tail.timer -= D_800BE9E4;
    if (particle->tail.timer < 0) {
        particle->head.def.texture = D_800A09C0[func_150ADA20() & 3];
        particle->tail.timer = (func_150ADA20() & 7) + 3;
    }

    sub->scale += (particle->tail.targetScale - sub->scale) * 0.6500000358f;
    return 1;
}

/* Throws 10-20 sparks from `pos`, fanned around the owner's facing `angle` (degrees) and against `vel`. */
void func_150D4D58(Vec3f *pos, const Vec3f *vel, f32 angle, u8 arg3, s32 arg4) {
    ParticleDesc spawn;
    s32 count;
    s16 yaw;
    f32 scale;
    s32 r1;
    s32 r2;

    count = func_150ADA20() % 11U + 10;
    spawn.renderMode = 0x2203;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.brightness = 0xFF;
    spawn.pos = *pos;
    spawn.offset = D_800A5480;
    spawn.fadeStart = 10;
    spawn.fadeRate = 25;
    spawn.scaleStart = 1;
    spawn.flags = 0x4C207;
    spawn.primColorMode = 0;
    spawn.envColorMode = 7;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    yaw = angle * 182.0444489f;
    yaw >>= 8;
    yaw += 0x40;
    spawn.scaleRate = 1.0f;
    do {
        scale = func_150ADA68() * 0.2230000049f + 0.8050000668f;
        spawn.texture = D_800A09C0[func_150ADA20() & 3];
        spawn.timer = func_150ADA20() % 18U + 27;
        spawn.alpha = func_150ADA20() % 156U + 100;
        spawn.width = spawn.height = func_150ADA68() * 233.0f + 199.0f;
        r1 = func_150ADA20();
        r2 = func_150ADA20();
        func_15143794(r1 % 130U + yaw, r2 % 81U - 64, func_150ADA68() * 10.0f + 5.0f, &spawn.vel);
        spawn.vel.x -= vel->x * scale;
        spawn.vel.z -= vel->z * scale;
        // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
        spawn.gravity = func_150ADA68() * 0.6460000277f + -0.7940000296f;
        spawn.flags &= ~0xC0;
        r1 = (func_150ADA20() & 1) ? 0x80 : 0;
        r2 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.flags |= r2 | r1;
        func_15130280(&spawn, 1, 0, 0, arg3, arg4);
        count--;
    } while (count > 0);
}

/*
 * Sound callback for the trail's three loops (0x5B3, 0x5B4, 0x5BC): moves volume and pitch toward
 * targets picked from the trail mode, follows the owner's position and stops when silenced.
 */
s32 func_150D5124(TrailSound *snd, s32 arg1, u32 *arg2, s32 arg3, s32 *pitchOut,
                  s32 arg5, u16 *playing) {
    CharacterTrailState *state;
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
            targetVolume = state->owner->shad * 25.0f;
            if (targetVolume > 5000) {
                targetVolume = 5000;
            } else if (targetVolume < 500) {
                targetVolume = 0;
            }
            targetPitch += state->owner->shad * 5.0f;
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

    snd->x = state->owner->xpos;
    snd->y = state->owner->ypos;
    snd->z = state->owner->zpos;

    if (D_800C35EA[0] == 1) {
        snd->volume = volume >> 1;
        *arg2 >>= 1;
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
