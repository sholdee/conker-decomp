#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);

/* D_800A9D40: lifetime and wobble of each kind of falling drop. */
typedef struct {
    /* 0x00 */ s16 lifeMin;
    /* 0x02 */ s16 lifeRange;
    /* 0x04 */ Size2f baseSize;
    /* 0x0C */ Size2f wobble;
} DropKind; /* size 0x14 */

extern DropKind D_800A9D40[];

/* Destroys the emitter already on link->actor, then creates one with extraSize bytes of extra data. */
DripEmitter *func_15190770(ActorLink *link, s32 extraSize, u8 owner, s32 list) {
    DripEmitter *emitter;
    ActorRef ref;

    ref.actor = link->actor;
    ref.uniqueId = link->actor->unique_id;
    func_15191B8C(&ref, 0xE);
    emitter = func_15167A68(0x20, list, extraSize + sizeof(DripEmitter), 1, owner, 1);
    if (emitter == NULL) {
        return NULL;
    }
    memcpy(&emitter->link, link, sizeof(ActorLink));
    emitter->slots = emitter->slotStorage;
    emitter->extra = emitter + 1;
    emitter->dropCount = 0;
    emitter->spawnAccum = 0.0f;
    bzero(emitter->slotStorage, sizeof(emitter->slotStorage));
    emitter->kind = func_151D8E20();
    return emitter;
}

/* Type 0x20 handler: releases the drops, then func_15169804. */
void func_15190840(DripEmitter *emitter) {
    func_15191400(emitter);
    func_15169804(&emitter->header);
}

/* Type 0x20 handler: releases the drops, then func_15169824. */
void func_1519086C(DripEmitter *emitter) {
    func_15191400(emitter);
    func_15169824(&emitter->header);
}

/* A point on an actor's bone where a drop hangs. */
typedef struct {
    /* 0x0 */ Vec3f offset;
    /* 0xC */ u8 bone;
} BoneMount; /* size 0x10 */

/* D_8008FA70: one entry per emitter kind (ActorLink.kind). */
typedef struct {
    /* 0x00 */ u8 dropKind;         /* D_800A9D40 index */
    /* 0x04 */ s32 numMounts;
    /* 0x08 */ BoneMount *mounts;
    /* 0x0C */ f32 rate;            /* new drops per unit of D_800BE9A4 */
    /* 0x10 */ s32 maxDrops;
    /* 0x14 */ s16 holdMin;         /* D_800BE9E4 ticks a drop hangs before it falls */
    /* 0x16 */ s16 holdRange;
} EmitterKind; /* size 0x18 */

extern EmitterKind D_8008FA70[];

/*
 * Update of the emitter: keeps its hanging drops on their bones and lets them fall when their time is up,
 * then hangs new drops on random free mounts at the kind's rate. Destroys itself when its actor is gone or
 * its timer runs out; when the actor has no bone matrices it hands its link to a retry task instead.
 */
void func_15190898(DripEmitter *emitter) {
    DripEmitter *self; // matching: stack copy of emitter, read back by the spawn loop test
    volatile u8 done; // matching: done is re-read from its stack slot after every store
    EmitterKind *kind;
    DripSlot *slot;
    DripSlot *head;
    DripSlot *tail;
    s32 freeCount;
    s32 i;
    s32 isFree;
    DropKind *dropKind;
    f32 dx;
    f32 dz;
    s32 skip;
    s32 index;
    DripParticle *drop;
    ParticleDef def;
    DripMotion motion;
    s16 timer;
    u8 hasTimer;

    self = emitter;
    done = 0;
    if (emitter->link.flags & 1) {
        emitter->link.timer -= D_800BE9E4;
        if (emitter->link.timer < 0) {
            done = 1;
        }
    }
    if (!done && (emitter->link.actor->type == 0 || emitter->link.actor->unique_id != emitter->link.actorId)) {
        done = 1;
    }
    if (!done) {
        if (emitter->link.actor->ob_matrix_ptr != NULL) {
            kind = &D_8008FA70[emitter->link.kind];
            head = NULL;
            tail = NULL;
            freeCount = 0;
            for (i = 0, slot = emitter->slots; i < kind->numMounts; i++, slot++) {
                if (slot->drop != NULL) {
                    slot->drop->head.def.rot = slot->drop->head.def.pos;
                    func_15143134(&kind->mounts[i].offset, &slot->drop->head.def.pos, (Mtx *)emitter->link.actor->ob_matrix_ptr + kind->mounts[i].bone);
                    slot->timer -= D_800BE9E4;
                    if (slot->timer < 0) {
                        DripParticle *falling = slot->drop;
                        DripMotion *fall = &slot->drop->motion;

                        dropKind = &D_800A9D40[fall->dropKind];
                        dx = (falling->head.def.pos.x - falling->head.def.rot.x) * D_800BE9A8;
                        dz = (falling->head.def.pos.z - falling->head.def.rot.z) * D_800BE9A8;
                        fall->vel.y = 0.0f;
                        fall->vel.x = dx;
                        fall->vel.z = dz;
                        func_1513F680(&slot->drop->head, 13, 18, 0, 13);
                        isFree = 1;
                        slot->drop->head.def.lifetime = func_150ADA20() % (u32)(dropKind->lifeRange + 1) + dropKind->lifeMin;
                        slot->drop->head.def.flags |= 1;
                        slot->drop = NULL;
                        emitter->dropCount--;
                    } else {
                        isFree = 0;
                    }
                } else {
                    isFree = 1;
                }
                if (isFree) {
                    freeCount++;
                    if (tail == NULL) {
                        tail = slot;
                    }
                    slot->next = head;
                    head = slot;
                }
            }
            if (freeCount != 0) {
                emitter->spawnAccum += kind->rate * D_800BE9A4;
                if (emitter->spawnAccum > 1.0f) {
                    DropKind *newKind;

                    newKind = &D_800A9D40[kind->dropKind];
                    tail->next = head;
                    motion.vel.x = 0.0f;
                    motion.vel.y = 0.0f;
                    motion.vel.z = 0.0f;
                    motion.drag = 0.2f;
                    motion.fadeStart = 15;
                    motion.fadeRate = 17;
                    motion.trail = 0.0f;
                    motion.stretch = 0.2f;
                    motion.gravity = -1.0f;
                    motion.dropKind = kind->dropKind;
                    func_1504715C(&motion.ground, emitter->link.actor);
                    motion.kind = emitter->kind;
                    def.texture = D_800AB310[emitter->kind];
                    def.renderModes = 0x1001;
                    def.lifetime = 300;
                    def.frame = 0;
                    def.frameSpeed = 0;
                    def.rgba[0] = 0;
                    def.rgba[1] = 0;
                    def.rgba[2] = 0;
                    def.rgba[3] = 0xFF;
                    def.rot.x = 0.0f;
                    def.rot.y = 0.0f;
                    def.rot.z = 0.0f;
                    def.flags = 0;
                    def.unk45 = 0xFF;
                    def.scale.x = 1.0f;
                    def.scale.y = 1.0f;
                    def.scale.z = 1.0f;
                    motion.phase[0] = 0;
                    motion.phase[1] = 0;
                    do {
                        skip = func_150ADA20() % (u32)freeCount;
                        while (skip--) {
                            tail = head;
                            head = head->next;
                        }
                        index = head - emitter->slots;
                        def.unk44 = func_150ADA20() % 101U + 100;
                        def.width = 0.0f;
                        def.height = 0.0f;
                        motion.baseSize = newKind->baseSize;
                        motion.wobble = newKind->wobble;
                        motion.phaseSpeed[0] = func_150ADA20() % 5U + 4;
                        motion.phaseSpeed[1] = func_150ADA20() % 5U + 4;
                        func_15143134(&kind->mounts[index].offset, &def.pos, (Mtx *)emitter->link.actor->ob_matrix_ptr + kind->mounts[index].bone);
                        def.rot = def.pos;
                        if (emitter->link.actor->unk118 < def.pos.y - motion.baseSize.height - motion.wobble.height) {
                            /* the random flip bit is always 0 (% 1), but still advances the RNG */
                            drop = func_1513D524(&def, 12, 0, 0, 13,
                                                 ((func_150ADA20() % 1) ? 1 : 0) | (D_800AB404[emitter->kind] ? 0 : 2),
                                                 sizeof(DripMotion), 0xFF, 0);
                            head->drop = drop;
                            if (drop != NULL) {
                                memcpy(&drop->motion, &motion, sizeof(motion));
                                head->timer = func_150ADA20() % (u32)(kind->holdRange + 1) + kind->holdMin;
                                freeCount--;
                                head = head->next;
                                tail->next = head;
                                emitter->dropCount++;
                            }
                        }
                        emitter->spawnAccum -= 1.0f;
                    } while (emitter->spawnAccum > 1.0f && freeCount != 0 && self->dropCount < kind->maxDrops);
                }
            }
        } else {
            DripRetryTask *task;

            done = 1;
            if (emitter->link.flags & 1) {
                hasTimer = 1;
                timer = emitter->link.timer;
            } else {
                hasTimer = 0;
                timer = 300;
            }
            task = func_151491F4(timer, -1, 13, hasTimer, 8, sizeof(ActorLink), 0xFF, 0);
            if (task != NULL) {
                memcpy(&task->link, &emitter->link, sizeof(ActorLink));
            }
        }
    }
    if (done) {
        func_1516972C(&emitter->header);
    }
}

/* Wobbles a drop's width and height around its base size. */
s32 func_15190F9C(DripParticle *drop) {
    DripParticle *p = drop; // matching: the param copy steers register allocation
    DripMotion *motion = &drop->motion;
    f32 widthWave;
    f32 heightWave;

    motion->phase[0] += motion->phaseSpeed[0] * D_800BE9E4;
    motion->phase[1] += motion->phaseSpeed[1] * D_800BE9E4;
    widthWave = func_151423D8((u8)(motion->phase[0] - 0x40));
    heightWave = func_151423D8((u8)(motion->phase[1] - 0x40));
    p->head.def.width += (motion->baseSize.width + motion->wobble.width * widthWave - p->head.def.width) * 0.5f;
    p->head.def.height += (motion->baseSize.height + motion->wobble.height * heightWave - p->head.def.height) * 0.5f;
    return 1;
}

/*
 * Vertex function: builds the drop's quad in its vertex buffer for viewport vp, tilted by how fast it
 * moves sideways on screen (D_800DD1E8 / D_800DD1D8 are the viewport's view direction).
 */
Vtx *func_1519108C(DripParticle *drop, s16 vp) {
    Vtx *vtx;
    Vtx *ret;
    f32 halfWidthCos;
    f32 sinAngle;
    f32 halfWidthSin;
    f32 heightCos;
    f32 heightSin;
    f32 speed;
    f32 sign;
    f32 *viewX;
    f32 *viewZ;
    f32 angle;
    u8 fresh;

    func_151D5D60(drop->head.vtxBuffers, vp, 0x40, &vtx, &fresh);
    ret = vtx;
    if (vtx) {
        if (fresh) {
            memcpy(drop->head.vtxBuffers[vp], drop->head.vtxTemplate, sizeof(drop->head.vtxTemplate));
            memcpy(drop->head.vtxBuffers[vp] + 4, drop->head.vtxTemplate, sizeof(drop->head.vtxTemplate));
        }
    } else {
        return NULL;
    }

    viewX = &D_800DD1E8[vp];
    viewZ = &D_800DD1D8[vp];
    sign = -1.0f;
    speed = ((*viewX * (drop->head.def.rot.x - drop->head.def.pos.x)) - (*viewZ * (drop->head.def.rot.z - drop->head.def.pos.z))) * D_800BE9A8;
    if (speed < 0.0f) {
        sign = 1.0f;
        speed = -speed;
    }

    angle = sqrtf(speed) * drop->motion.stretch;
    if (angle > 1.570796371f) {
        angle = 1.570796371f;
    }

    angle = angle * sign;
    sinAngle = sinf(angle);
    angle = cosf(angle);
    halfWidthCos = drop->head.def.width * angle;
    halfWidthSin = drop->head.def.width * sinAngle;
    heightCos = drop->head.def.height * angle;
    heightSin = drop->head.def.height * sinAngle;

    vtx[0].v.ob[0] = (*viewX * -halfWidthCos) + drop->head.def.pos.x;
    vtx[0].v.ob[1] = drop->head.def.pos.y + halfWidthSin;
    vtx[0].v.ob[2] = drop->head.def.pos.z - (*viewZ * -halfWidthCos);
    vtx[0].v.flag = 0;

    vtx[1].v.ob[0] = (*viewX * halfWidthCos) + drop->head.def.pos.x;
    vtx[1].v.ob[1] = drop->head.def.pos.y - halfWidthSin;
    vtx[1].v.ob[2] = drop->head.def.pos.z - (*viewZ * halfWidthCos);
    vtx[1].v.flag = 0;

    angle = halfWidthCos - heightSin;
    vtx[2].v.ob[0] = (*viewX * angle) + drop->head.def.pos.x;
    vtx[2].v.ob[1] = (drop->head.def.pos.y - halfWidthSin) - heightCos;
    vtx[2].v.ob[2] = drop->head.def.pos.z - (*viewZ * angle);
    vtx[2].v.flag = 0;

    angle = -(halfWidthCos + heightSin);
    vtx[3].v.ob[0] = (*viewX * angle) + drop->head.def.pos.x;
    vtx[3].v.ob[1] = (drop->head.def.pos.y + halfWidthSin) - heightCos;
    vtx[3].v.ob[2] = drop->head.def.pos.z - (*viewZ * angle);
    vtx[3].v.flag = 0;

    return ret;
}

/* Lets go of every drop the emitter still holds and clears its slots. */
void func_15191400(DripEmitter *emitter) {
    s32 i;
    DripSlot *slot;
    DripMotion *motion;
    DropKind *dropKind;

    emitter->spawnAccum = 0.0f;
    emitter->dropCount = 0;
    slot = emitter->slots;
    for (i = 0; i < DRIP_SLOT_COUNT; i++, slot++) {
        if (slot->drop != NULL) {
            motion = &slot->drop->motion;
            dropKind = &D_800A9D40[motion->dropKind];
            motion->vel.x = 0.0f;
            motion->vel.y = 0.0f;
            motion->vel.z = 0.0f;
            func_1513F680(&slot->drop->head, 13, 18, 0, 13);
            slot->drop->head.def.lifetime = func_150ADA20() % (u32)(dropKind->lifeRange + 1) + dropKind->lifeMin;
            slot->drop->head.def.flags |= 1;
        }
    }
    bzero(emitter->slots, DRIP_SLOT_COUNT * sizeof(DripSlot));
}

/* Update of a falling drop: moves it under drag and gravity; on reaching the ground it splashes and dies. */
s8 func_15191520(DripParticle *drop) {
    f32 dx;
    f32 dz;
    f32 dy;
    DripMotion *motion;
    s8 alive;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;

    oldY = drop->head.def.pos.y;
    motion = &drop->motion;
    alive = 1;
    oldVx = motion->vel.x;
    oldVz = motion->vel.z;
    motion->vel.x *= motion->drag * D_800BE9A4;
    motion->vel.z *= motion->drag * D_800BE9A4;
    dx = (motion->vel.x + oldVx) * 0.5f * D_800BE9A4;
    dy = motion->vel.y * D_800BE9A4 + motion->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (motion->vel.z + oldVz) * 0.5f * D_800BE9A4;
    motion->vel.y += motion->gravity * D_800BE9A4;
    drop->head.def.pos.x += dx;
    drop->head.def.pos.y += dy;
    drop->head.def.pos.z += dz;
    drop->head.def.rot.x += dx;
    drop->head.def.rot.y += dy;
    drop->head.def.rot.z += dz;
    drop->head.def.rot.x = drop->head.def.pos.x + (drop->head.def.rot.x - drop->head.def.pos.x) * motion->trail;
    drop->head.def.rot.y = drop->head.def.pos.y + (drop->head.def.rot.y - drop->head.def.pos.y) * motion->trail;
    drop->head.def.rot.z = drop->head.def.pos.z + (drop->head.def.rot.z - drop->head.def.pos.z) * motion->trail;
    {
        Vec3f probe;

        probe.x = drop->head.def.pos.x;
        probe.y = oldY - drop->head.def.height;
        probe.z = drop->head.def.pos.z;
        if (func_1514672C(&probe) == 0) {
            return 0;
        }
        if (func_15046C80(&probe, 0, drop->head.def.pos.y - drop->head.def.height, &motion->ground) != 0) {
            probe.y = motion->ground.height;
            if (motion->ground.source == 3) {
                func_151DBCBC(motion->kind, drop->head.def.width * drop->head.def.height * 0.25f, drop->head.def.unk44,
                              &motion->ground.tri, &probe, drop->head.header.owner, drop->head.header.kind);
            } else if (func_150ADA20() & 1) {
                func_151D9B8C(motion->kind, drop->head.def.width * drop->head.def.height * 0.150000006f, drop->head.def.unk44,
                              &motion->ground.tri, &probe, func_150ADA20() % 41U + 80, 1, 1, 0,
                              drop->head.header.owner, drop->head.header.kind);
            } else {
                func_151DAB58(motion->kind, drop->head.def.width * drop->head.def.height * 0.05000000075f, drop->head.def.unk44,
                              &probe, 1, drop->head.header.owner, drop->head.header.kind);
            }
            alive = 0;
        }
    }
    return alive;
}

/* Fades a drop's alpha out over the last fadeStart frames of its lifetime. */
s32 func_1519187C(DripParticle *drop) {
    s16 lifetime;
    s32 alpha;

    lifetime = drop->head.def.lifetime;
    if (lifetime < drop->motion.fadeStart) {
        alpha = lifetime * drop->motion.fadeRate;
        if (alpha < drop->head.def.unk44) {
            drop->head.def.unk44 = alpha;
        }
    }
    return 1;
}

/* Retries creating the emitter until its actor has bone matrices; gives up when the actor is gone. */
void func_151918BC(DripRetryTask *task) {
    Actor *actor;
    ActorLink *link;
    u8 done;

    link = &task->link;
    actor = link->actor;
    done = 0;
    if (actor->type == 0) {
        done = 1;
    } else if (actor->unique_id != link->actorId) {
        done = 1;
    }
    if (!done && actor->ob_matrix_ptr != NULL) {
        if (task->base.header.flags & 1) {
            link->timer = task->base.header.state;
            link->flags |= 1;
        } else {
            link->timer = 300;
        }
        done = 1;
        func_15190770(link, 0, task->base.header.owner, task->base.header.kind);
    }
    if (done) {
        func_1516972C(&task->base.header);
    }
}

/* Message handler of the emitter: 0xD starts its timer, 0xC / 0xE / 0x50 / 0x4E destroy it. */
/* Message handler of the emitter: 0xD starts its timer, 0xC / 0xE / 0x50 / 0x4E destroy it. */
void func_15191980(DripEmitter *emitter, ActorRef *ref, u8 msg) {
    if (msg == 0xD) {
        // matching: each test is its own if so IDO reloads ref for the second one (|| loads it once)
        if (ref->actor == emitter->link.actor) {
            goto hit2;
        }
        if (ref->uniqueId != emitter->link.actorId) {
            goto skip;
        }
    hit2:
        emitter->link.flags |= 1;
        emitter->link.timer = 100;
    skip:;
    } else if (msg == 0xC || msg == 0x50 || msg == 0xE) {
        if (ref->actor == emitter->link.actor) {
            goto hit;
        }
        if (ref->uniqueId == emitter->link.actorId) {
        hit:
            func_1516972C(&emitter->header);
        }
    } else if (msg == 0x4E) {
        func_1516972C(&emitter->header);
    } else {
        func_15169850((s32)ref, msg, &emitter->link.actor, &emitter->link.actorId, emitter);
    }
}

/* Message handler of the retry task, as func_15191980. */
void func_15191A84(DripRetryTask *task, ActorRef *ref, u8 msg) {
    ActorLink *link;

    link = &task->link;
    if (msg == 0xC || msg == 0xE || msg == 0x50) {
        // matching: separate ifs, as in func_15191980
        if (ref->actor == link->actor) {
            goto hit;
        }
        if (ref->uniqueId == link->actorId) {
        hit:
            func_1516972C(&task->base.header);
        }
    } else if (msg == 0x4E) {
        func_1516972C(&task->base.header);
    } else if (msg == 0xD) {
        if (ref->actor == link->actor) {
            goto hit2;
        }
        if (ref->uniqueId != link->actorId) {
            goto skip;
        }
    hit2:
        task->base.header.flags |= 1;
        task->base.header.state = 100;
    skip:;
    } else {
        func_15169850((s32)ref, msg, &link->actor, &link->actorId, task);
    }
}


/* Sends msg about ref's actor through func_151494E0 and to every object of type 0x20. */
void func_15191B8C(ref, msg)
    ActorRef *ref;
    u8 msg;
{
    const s32 types[1] = { 0x20 };

    func_151494E0((s32)ref, msg);
    func_15169260(types, 1, (s32)ref, msg);
}
