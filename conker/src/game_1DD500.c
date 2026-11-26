#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One row of the per-actor-type effect profile table. */
typedef struct Game1DD500Profile {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u8 unkE; /* 1: flagged effect, 2: no effect */
    /* 0x0F */ u8 padF;
} Game1DD500Profile; // size 0x10

extern Vec3f D_800AA120;

/* Joint matrix of an actor, as an Mtx. */
#define JOINT_MTX(actor, joint) (&(actor)->ob_matrix_ptr[joint])

/* Effect profile index for an actor; actor types 0x53 and 0xA5 (and no actor) use profile 4. */
#define EFFECT_PROFILE(actor) \
    ((actor) != NULL ? (((actor)->character_type == 0x53 || (actor)->character_type == 0xA5) ? 4 : func_15134070(actor)) : 4)

/* Spawns one randomized particle at (x, y, z) launched in direction (yaw, pitch). */
s32 func_151B0050(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7,
                  s16 pitch, s16 yaw, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    Vec3f pos;
    Vec3f dir;
    f32 speed;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    speed = func_150ADA68() * 5.3f + 4.4f;
    func_15143794(yaw, pitch, speed, &dir);
    // matching: a literal 0.0f scale is folded away; IDO keeps the multiply for (0.68f - 0.68f)
    func_151DA6F8(&pos, &dir, func_150ADA68() * (0.68f - 0.68f) + 0.68f, func_150ADA20() % 20 + 31,
                  func_150ADA20() % 101 + 155, func_150ADA68() * 2.33f + 6.7f,
                  (func_150ADA20() & 1) + 3, 1, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0, argE, 1);
    return 1;
}

/* Spawns an impact effect at joint 6 of actor, styled by the profile of other. */
void func_151B01B8(Actor *actor, Actor *other) {
    SplashSpraySpec impact;
    u8 shade;
    CollisionHit ground;
    s32 profile;

    if (actor != NULL && actor->ob_matrix_ptr != NULL && (actor->unk74 & 0xF) != 0xF) {
        func_1504715C(&ground, actor);
        profile = EFFECT_PROFILE(other);
        if (profile != 99) {
            if (D_800A3FD8[profile].unkE != 2) {
                shade = (actor->langle >> 8) + 0x40;
                func_15143134(&D_800AA120, &impact.spray.pos, JOINT_MTX(actor, 6));
                impact.spray.countBase = 10;
                impact.spray.countRange = 20;
                impact.angles.yawBase = shade - 0x68;
                impact.spray.scaleBase = 7.22f;
                impact.angles.yawRange = 0xD0;
                impact.angles.pitchBase = -0x1B;
                impact.angles.pitchRange = 0x36;
                impact.spray.itemCountBase = 3;
                impact.spray.itemCountRange = 2;
                impact.spray.lifeBase = 30;
                impact.spray.lifeRange = 40;
                impact.spray.unk30 = 155;
                impact.spray.unk32 = 100;
                impact.spray.scaleRange = 3.04f;
                impact.spray.sizeBase = 0.810000062f;
                impact.spray.sizeRange = 0.405000031f;
                impact.spray.speedBase = 6.0f;
                impact.spray.speedRange = 13.90000057f;
                impact.spray.unk38 = 0.5f;
                if (D_800A3FD8[profile].unkE == 1) {
                    impact.spray.kind = 1;
                } else {
                    impact.spray.kind = 0;
                }
                impact.spray.unk3C = 16;
                impact.spray.unk3E = 15;
                impact.spray.unk40 = 0;
                func_15153F18(&impact.angles, &impact.spray, &ground, 0xFF, 1);
            }
        }
    }
}

/* Big landing burst at actor: an impact effect, 12-17 splats scattered on the ground around it, and a mark under it. */
void func_151B03B8(Actor *actor, u8 arg1, s32 arg2) {
    SplashSpraySpec impact;
    CollisionHit ground;
    s32 count;
    CollisionHit scatterGround;
    Vec3f probe;
    f32 spot[3];   // matching: spot and center stay f32[3] (a Vec3f changes IDO's schedule); passed as Vec3f below
    f32 center[3];
    CollisionHit centerGround;

    if (actor != NULL) {
        func_1504715C(&ground, actor);
        impact.spray.pos.x = actor->xpos;
        impact.spray.pos.y = actor->ypos;
        impact.spray.pos.z = actor->zpos;
        impact.spray.scaleBase = 5.08f;
        impact.spray.countBase = 10;
        impact.angles.yawRange = 0xFF;
        impact.angles.pitchBase = -0x3F;
        impact.spray.countRange = 0;
        impact.angles.yawBase = 0;
        impact.angles.pitchRange = 0x2D;
        impact.spray.itemCountBase = 3;
        impact.spray.itemCountRange = 2;
        impact.spray.lifeBase = 40;
        impact.spray.lifeRange = 20;
        impact.spray.unk30 = 155;
        impact.spray.unk32 = 100;
        impact.spray.kind = 0;
        impact.spray.unk3C = 16;
        impact.spray.unk3E = 15;
        impact.spray.unk40 = 0;
        impact.spray.scaleRange = 3.02f;
        impact.spray.sizeBase = 0.807000041f;
        impact.spray.sizeRange = 0.701000035f;
        impact.spray.speedBase = 18.0f;
        impact.spray.speedRange = 11.8f;
        impact.spray.unk38 = 0.5f;
        func_15153F18(&impact.angles, &impact.spray, &ground, 0xFF, 1);

        count = func_150ADA20() % 6 + 12;
        scatterGround.material = 0;
        scatterGround.flags = 0;
        scatterGround.source = 0;
        scatterGround.surface = 0;
        scatterGround.height = -10000.0f;
        probe.y = actor->ypos + 100.0f;
        if (count != 0) {
            // matching: a do/while (--count != 0) loop schedules differently from this break form
            for (;;) {
                func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 100.0f + 110.0f, &spot[0], &spot[2]);
                spot[0] += actor->xpos;
                spot[2] += actor->zpos;
                probe.x = spot[0];
                probe.z = spot[2];
                if (!func_15046C80(&probe, 0, actor->ypos - 500.0f, &scatterGround) || scatterGround.source == 3) {
                    return;
                }
                spot[1] = scatterGround.height;
                spot[1] += 10.0f;
                // matching: (f32)100 is not CSE'd with the 100.0f above, which would tie up an FP register
                if (func_150ADA20() & 1) {
                    func_151D9B8C(0, func_150ADA68() * 4.5f + 15.0f, func_150ADA68() * (f32)100 + 155.0f,
                                  &scatterGround.tri, (Vec3f *)spot, 100, 0, 1, 0, arg1, arg2);
                } else {
                    func_151DAB58(0, func_150ADA68() * 5.9f + 6.1f, func_150ADA68() * (f32)100 + 155.0f,
                                  (Vec3f *)spot, 1, arg1, arg2);
                }
                if (--count == 0) {
                    break;
                }
            }
        }

        center[0] = actor->xpos;
        center[1] = actor->ypos;
        center[2] = actor->zpos;
        centerGround.surface = 0;
        centerGround.source = 0;
        centerGround.flags = 0;
        centerGround.material = 0;
        centerGround.height = -10000.0f;
        {
            Vec3f above;

            above.x = center[0];
            above.y = center[1] + 100.0f;
            above.z = center[2];
            if (!func_1504697C(&above, 0, center[1] - 500.0f, &centerGround) || centerGround.source == 3) {
                return;
            }
        }
        center[1] = centerGround.height;
        center[1] += 10.0f;
        func_15136698((func_150ADA68() * 40.0f + 120.0f) * 0.001f, (func_150ADA68() * 123.0f + 123.0f) * 0.00001f,
                      func_150ADA20() % 101 + 155, func_150ADA20() % 101 + 100, func_150ADA20() % 56 + 70,
                      &centerGround.tri, (Vec3f *)center, 0, 1, arg1, arg2);
    }
}

/* Per-particle motion block at 0x110 of a particle object. */
typedef struct Game1DD500Motion {
    /* 0x00 */ Vec3f velocity;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseWidth;     /* oscillator angles */
    /* 0x11 */ u8 phaseHeight;
    /* 0x12 */ s8 phaseSpeedWidth;
    /* 0x13 */ s8 phaseSpeedHeight;
    /* 0x14 */ f32 baseWidth;
    /* 0x18 */ f32 baseHeight;
    /* 0x1C */ f32 swingWidth;
    /* 0x20 */ f32 swingHeight;
    /* 0x24 */ u8 checkGround;   /* splat when it reaches the ground */
    /* 0x25 */ u8 unk25;
    /* 0x26 */ u8 pad26[2];
} Game1DD500Motion; // size 0x28

/* Particle object created by func_1513D524. */
struct Game1DD500Particle {
    /* 0x000 */ EffectHeader header;
    /* 0x010 */ u8 pad10[0xC];
    /* 0x01C */ s16 life;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 height;
    /* 0x034 */ Vec3f position;
    /* 0x040 */ Vec3f previous;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x1B];
    /* 0x078 */ CollisionHit ground;
    /* 0x09C */ u8 pad9C[0x24];
    /* 0x0C0 */ Vtx quadTemplate[4];
    /* 0x100 */ Vtx *quads[4];
    /* 0x110 */ Game1DD500Motion motion;
};

/* A particle held at one of the owner's joints until its timer runs out. */
typedef struct Game1DD500Slot {
    /* 0x0 */ Game1DD500Particle *particle;
    /* 0x4 */ s32 timer;
    /* 0x8 */ struct Game1DD500Slot *next; /* ring of free slots while spawning */
} Game1DD500Slot;

/* Effect that keeps particles stuck to an actor's joints and drops them over time. */
typedef struct Game1DD500Emitter {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;   /* owner->unique_id at creation */
    /* 0x08 */ s32 count;    /* attached particles */
    /* 0x0C */ Game1DD500Slot slots[11];
    /* 0x90 */ f32 spawnTime;
    /* 0x94 */ u8 profileFlag;
} Game1DD500Emitter; // size 0x98

/* Effect task: a TimedEffect header followed by the effect's data. */
struct Game1DD500Task {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game1DD500Emitter emitter;
};

void *memcpy(void *, const void *, u32);

/* Starts an emitter task on actor (lifetime 300 when -1), first telling older ones on it to end. */
Game1DD500Task *func_151B09BC(Actor *actor, Actor *other, s16 lifetime, u8 arg3, s32 arg4) {
    Game1DD500Task *task;
    Game1DD500Emitter emitter;
    s32 profile;
    s16 life;
    u8 timed;
    ActorRef msg;

    if (actor == NULL) {
        return NULL;
    }
    profile = EFFECT_PROFILE(other);
    if (profile == 99) {
        return NULL;
    }
    if (D_800A3FD8[profile].unkE == 2) {
        return NULL;
    }
    if (D_800A3FD8[profile].unkE == 1) {
        emitter.profileFlag = 1;
    } else {
        emitter.profileFlag = 0;
    }
    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    func_151494E0((s32)&msg, 0x13);
    timed = FALSE;
    if (lifetime == -1) {
        life = 300;
    } else {
        life = lifetime;
        timed = TRUE;
    }
    emitter.owner = actor;
    emitter.ownerId = actor->unique_id;
    emitter.count = 0;
    emitter.spawnTime = 0.0f;
    {
        s32 i;

        for (i = 0; i < 11; i++) {
            emitter.slots[i].particle = NULL;
            emitter.slots[i].timer = 0;
            emitter.slots[i].next = NULL;
        }
    }
    task = func_151491F4(life, -1, 0x13, timed, 0xF, sizeof(emitter), arg3, arg4);
    if (task != NULL) {
        memcpy(&task->emitter, &emitter, sizeof(emitter));
    }
    return task;
}

/* A joint and an offset in that joint's space where an emitter particle sits. */
typedef struct Game1DD500Mount {
    /* 0x0 */ u8 joint;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ Vec3f offset;
} Game1DD500Mount;

extern Game1DD500Mount D_800AA12C[11];
extern Game1DD500Mount D_800AA1DC[11]; /* actor type 0x36 */

/* Emitter task update: carries attached particles with the owner's joints, releases expired ones and spawns new ones. */
void func_151B0B88(Game1DD500Task *task) {
    Game1DD500Emitter *emitter = &task->emitter;
    u8 failed; // matching: ending the task directly in the check reorders the whole function

    failed = FALSE;
    if (emitter->owner->type == 0 || emitter->owner->unique_id != emitter->ownerId) {
        failed = TRUE;
    }
    if (!failed) {
        Game1DD500Slot *head;
        Game1DD500Slot *last;
        s32 i;
        u32 freeCount;

        if (emitter->owner->ob_matrix_ptr != NULL && (emitter->owner->unk74 & 0xF) != 0xF) {
            head = NULL;
            last = NULL;
            freeCount = 0;
            for (i = 0; i < 11; i++) {
                Game1DD500Slot *slot = &emitter->slots[i];
                s32 isFree;

                if (slot->particle != NULL) {
                    slot->particle->previous = slot->particle->position;
                    func_15143134(emitter->owner->character_type == 0x36 ? &D_800AA1DC[i].offset : &D_800AA12C[i].offset,
                                  &slot->particle->position,
                                  emitter->owner->character_type == 0x36 ? JOINT_MTX(emitter->owner, D_800AA1DC[i].joint)
                                                             : JOINT_MTX(emitter->owner, D_800AA12C[i].joint));
                    slot->timer -= D_800BE9E4;
                    if (slot->timer < 0) {
                        Vec3f *velocity = &slot->particle->motion.velocity;

                        velocity->x = (slot->particle->position.x - slot->particle->previous.x) * D_800BE9A8;
                        velocity->y = 0.0f;
                        velocity->z = (slot->particle->position.z - slot->particle->previous.z) * D_800BE9A8;
                        func_1513F680(slot->particle, 0x10, 0x1B, 0, 0x10);
                        slot->particle->flags |= 1;
                        slot->particle = NULL;
                        emitter->count--;
                        isFree = TRUE;
                    } else {
                        isFree = FALSE;
                    }
                } else {
                    isFree = TRUE;
                }
                if (isFree) {
                    if (last == NULL) {
                        last = slot;
                    }
                    slot->next = head;
                    head = slot;
                    freeCount++;
                }
            }
            if (freeCount != 0) {
                emitter->spawnTime += func_150ADA68() * 307.0f * 0.001f * D_800BE9A4;
                if (emitter->spawnTime > 1.0f) {
                    ParticleDef desc;
                    Game1DD500Motion motion;

                    last->next = head;
                    desc.texture = D_800AB310[emitter->profileFlag];
                    desc.renderModes = 0x1001;
                    desc.lifetime = func_150ADA20() % 51 + 52;
                    desc.frame = 0;
                    desc.frameSpeed = 0;
                    desc.rgba[0] = 0;
                    desc.rgba[1] = 0;
                    desc.rgba[2] = 0;
                    desc.rgba[3] = 0xFF;
                    desc.flags = 0;
                    desc.scale.x = 1.0f;
                    desc.scale.y = 1.0f;
                    desc.scale.z = 1.0f;
                    motion.phaseWidth = 0;
                    motion.phaseHeight = 0;
                    desc.rot.x = 0.0f;
                    desc.rot.y = 0.0f;
                    desc.rot.z = 0.0f;
                    motion.unk25 = emitter->profileFlag;
                    motion.checkGround = func_150ADA20() & 1;
                    desc.width = 0.0f;
                    desc.height = 0.0f;
                    motion.baseWidth = 5.0f;
                    motion.baseHeight = 25.0f;
                    motion.swingWidth = 2.0f;
                    motion.swingHeight = 8.0f;
                    do {
                        u32 skip;
                        s32 index;
                        Game1DD500Particle *particle;

                        skip = func_150ADA20() % freeCount;
                        while (skip--) {
                            last = head;
                            head = head->next;
                        }
                        index = head - emitter->slots;
                        desc.unk45 = func_150ADA20() % 101 + 100;
                        desc.unk44 = func_150ADA20() % 101 + 155;
                        motion.phaseSpeedWidth = func_150ADA20() % 5 + 4;
                        motion.phaseSpeedHeight = func_150ADA20() % 5 + 4;
                        // matching: "- 703.0f" emits sub.s; the original adds a negative constant
                        motion.gravity = (func_150ADA68() * 506.0f + -703.0f) * 0.001f;
                        func_15143134(&D_800AA12C[index].offset, &desc.pos,
                                      JOINT_MTX(emitter->owner, D_800AA12C[index].joint));
                        desc.rot = desc.pos;
                        particle = func_1513D524(&desc, 0x11, 0, 0, 0x10, ((func_150ADA20() % 1) ? 1 : 0) | 2, 0x28,
                                                 task->base.header.owner, task->base.header.kind);
                        head->particle = particle;
                        if (particle != NULL) {
                            memcpy(&particle->motion, &motion, sizeof(motion));
                            head->timer = func_150ADA20() % 31 + 20;
                            head = head->next;
                            freeCount--;
                            last->next = head;
                            emitter->count++;
                        }
                        emitter->spawnTime -= 1.0f;
                    } while (emitter->spawnTime > 1.0f && freeCount != 0);
                }
            }
        } else if (emitter->count != 0) {
            func_151B1918(task);
        }
    }
    if (failed) {
        task->base.header.state = -1;
    }
}

/* Moves a released particle under drag and gravity; with checkGround set, a particle that reaches
 * the ground leaves a splat there and the function returns 0 (otherwise 1). */
s32 func_151B118C(Game1DD500Particle *particle) {
    f32 dx;
    f32 dz;
    f32 dy;
    Game1DD500Motion *motion;
    s8 alive;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;
    Vec3f probe;
    Vec3f splat;
    f32 size;

    motion = &particle->motion;
    oldY = particle->position.y;
    oldVx = motion->velocity.x;
    oldVz = motion->velocity.z;
    alive = TRUE;
    motion->velocity.x *= 0.25f * D_800BE9A4;
    motion->velocity.z *= 0.25f * D_800BE9A4;
    dx = (motion->velocity.x + oldVx) * 0.5f * D_800BE9A4;
    dy = motion->velocity.y * D_800BE9A4 + motion->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (motion->velocity.z + oldVz) * 0.5f * D_800BE9A4;
    motion->velocity.y += motion->gravity * D_800BE9A4;
    particle->position.x += dx;
    particle->position.y += dy;
    particle->position.z += dz;
    particle->previous.x += dx;
    particle->previous.y += dy;
    particle->previous.z += dz;
    particle->previous.x = particle->position.x + (particle->previous.x - particle->position.x) * 0.706000030f;
    particle->previous.y = particle->position.y + (particle->previous.y - particle->position.y) * 0.706000030f;
    particle->previous.z = particle->position.z + (particle->previous.z - particle->position.z) * 0.706000030f;
    if (motion->checkGround) {
        probe.x = particle->position.x;
        probe.y = oldY - particle->height;
        probe.z = particle->position.z;
        if (func_15046C80(&probe, 0, particle->position.y - particle->height, &particle->ground)) {
            splat.x = probe.x;
            splat.y = particle->ground.height + 5.0f;
            splat.z = probe.z;
            size = ((motion->baseWidth + motion->swingWidth * 0.5f) + (motion->baseHeight + motion->swingHeight * 0.5f)) * 0.5f;
            if (func_150ADA20() & 1) {
                func_151D9B8C(motion->unk25, size * 1.7f, particle->alpha, &particle->ground.tri, &splat, 100, 0, 1, 0,
                              particle->header.owner, particle->header.kind);
            } else {
                func_151DAB58(motion->unk25, size * 0.68f, particle->alpha, &splat, 1, particle->header.owner, particle->header.kind);
            }
            alive = FALSE;
        }
    }
    return alive;
}

/* Fades a particle out over the last 32 frames of its life. */
s32 func_151B1478(Game1DD500Particle *particle) {
    s32 fade;

    if (particle->life < 32) {
        fade = particle->life * 8;
        if (fade < particle->alpha) {
            particle->alpha = fade;
        }
    }
    return 1;
}

/* Writes a particle's quad for view into that view's vertex buffer, facing the camera and tilted
 * by the particle's sideways motion. Returns the vertices, or NULL when there is no buffer. */
Vtx *func_151B14AC(Game1DD500Particle *particle, s16 view) {
    Vtx *quad;
    Vtx *ret;
    f32 sine;
    f32 widthSin;
    f32 heightCos;
    f32 heightSin;
    f32 widthCos;
    f32 sign;
    f32 lateral;
    f32 *camX;
    f32 t;
    u8 fresh;

    func_151D5D60(particle->quads, view, 0x40, &quad, &fresh);
    ret = quad; // matching: quad is address-taken; returning it directly reloads it
    if (quad != NULL) {
        if (fresh) {
            memcpy(particle->quads[view], particle->quadTemplate, sizeof(particle->quadTemplate));
            memcpy(&particle->quads[view][4], particle->quadTemplate, sizeof(particle->quadTemplate));
        }
    } else {
        return NULL;
    }

    camX = &D_800DD1E8[view];
    sign = -1.0f;
    lateral = ((*camX * (particle->previous.x - particle->position.x)) -
               (D_800DD1D8[view] * (particle->previous.z - particle->position.z))) * D_800BE9A8;
    if (lateral < 0.0f) {
        sign = 1.0f;
        lateral = -lateral;
    }

    t = sqrtf(lateral) * 0.25f;
    if (t > 1.570796371f) {
        t = 1.570796371f;
    }

    t *= sign;
    sine = sinf(t);
    t = cosf(t);
    widthCos = particle->width * t;
    widthSin = particle->width * sine;
    heightCos = particle->height * t;
    heightSin = particle->height * sine;

    quad[0].v.ob[0] = (*camX * -widthCos) + particle->position.x;
    quad[0].v.ob[1] = particle->position.y + widthSin;
    quad[0].v.ob[2] = particle->position.z - (D_800DD1D8[view] * -widthCos);
    quad[0].v.flag = 0;

    quad[1].v.ob[0] = (*camX * widthCos) + particle->position.x;
    quad[1].v.ob[1] = particle->position.y - widthSin;
    quad[1].v.ob[2] = particle->position.z - (D_800DD1D8[view] * widthCos);
    quad[1].v.flag = 0;

    t = widthCos - heightSin;
    quad[2].v.ob[0] = (*camX * t) + particle->position.x;
    quad[2].v.ob[1] = (particle->position.y - widthSin) - heightCos;
    quad[2].v.ob[2] = particle->position.z - (D_800DD1D8[view] * t);
    quad[2].v.flag = 0;

    t = -(widthCos + heightSin);
    quad[3].v.ob[0] = (*camX * t) + particle->position.x;
    quad[3].v.ob[1] = (particle->position.y + widthSin) - heightCos;
    quad[3].v.ob[2] = particle->position.z - (D_800DD1D8[view] * t);
    quad[3].v.flag = 0;

    return ret;
}

/* Swings a particle's width and height around their base sizes, each on its own phase. */
s32 func_151B1828(Game1DD500Particle *arg0) {
    Game1DD500Particle *particle = arg0; // matching: the copy of the argument sets the stack frame size
    Game1DD500Motion *motion = &arg0->motion;
    f32 swingW;
    f32 swingH;

    motion->phaseWidth += motion->phaseSpeedWidth * D_800BE9E4;
    motion->phaseHeight += motion->phaseSpeedHeight * D_800BE9E4;
    swingW = func_151423D8(motion->phaseWidth - 0x40);
    swingH = func_151423D8(motion->phaseHeight - 0x40);
    particle->width += (motion->baseWidth + motion->swingWidth * swingW - particle->width) * 0.5f;
    particle->height += (motion->baseHeight + motion->swingHeight * swingH - particle->height) * 0.5f;
    return 1;
}

/* Frees every attached particle and resets the emitter. */
void func_151B1918(Game1DD500Task *task) {
    Game1DD500Emitter *emitter;
    s32 i;

    emitter = &task->emitter;
    emitter->count = 0;
    emitter->spawnTime = 0.0f;
    for (i = 0; i < 11; i++) {
        Game1DD500Slot *slot = &emitter->slots[i];

        if (slot->particle != NULL) {
            func_1516972C(&slot->particle->header);
        }
        slot->particle = NULL;
        slot->timer = 0;
        slot->next = NULL;
    }
}

/* Emitter message handler: ends the task when its owner goes away (messages 0 and 0x13) and
 * follows the owner through an identity swap (0x2D). */
void func_151B19A4(Game1DD500Task *task, void *msg, u8 type) {
    Game1DD500Emitter *emitter = &task->emitter;
    ActorRef *actorMsg = msg;
    ActorSwapMsg *swap = msg;
    Actor *owner = emitter->owner;

    if (type == 0 || type == 0x13) {
        if (owner == actorMsg->actor || actorMsg->uniqueId == emitter->ownerId) {
            func_1516972C(&task->base.header);
        }
    } else if (type == 0x2D) {
        if (emitter->owner == swap->actorA) {
            emitter->owner = swap->actorB;
            emitter->ownerId = swap->idB;
        } else if (emitter->owner == swap->actorB) {
            emitter->owner = swap->actorA;
            emitter->ownerId = swap->idA;
        }
    }
}

/* Task handler: frees the attached particles, then passes the task on to func_1514933C. */
void func_151B1A58(Game1DD500Task *task) {
    func_151B1918(task);
    func_1514933C(&task->base);
}

/* Task handler: frees the attached particles, then passes the task on to func_15149368. */
void func_151B1A84(Game1DD500Task *task) {
    func_151B1918(task);
    func_15149368(&task->base);
}
