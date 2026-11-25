#include <ultra64.h>
/* functions.h declares these three with stale signatures; hide them there and declare the real ones here. */
#define func_151D9B8C func_151D9B8C_decl_in_functions_h
#define func_1513D524 func_1513D524_decl_in_functions_h
#include "functions.h"
#undef func_151D9B8C
#undef func_1513D524
void func_151D9B8C(u8, f32, u8, void *, f32 *, s16, u8, u8, u8, u8, s32);
#include "variables.h"
#include "effects.h"

/* Ground probe result filled by func_1504715C / func_15046C80 / func_1504697C. */
typedef struct Game1DD500Ground {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
} Game1DD500Ground; // size 0x24

/* One row of the per-actor-type effect profile table. */
typedef struct Game1DD500Profile {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u8 unkE; /* 1: flagged effect, 2: no effect */
    /* 0x0F */ u8 padF;
} Game1DD500Profile; // size 0x10

/* Parameters for the impact effect spawned by func_15153F18. */
typedef struct Game1DD500Impact {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ vertex position;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s8 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
} Game1DD500Impact;

void func_15143794(s16 yaw, s16 pitch, f32 speed, f32 *out);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                   s32, s32, s32, s32, s32, s32, s32, s32);
void func_1504715C(Game1DD500Ground *, struct127 *);
s32 func_15134070(struct127 *);
void func_15143134(vertex *, vertex *, Mtx *);
void func_15153F18(Game1DD500Impact *, vertex *, Game1DD500Ground *, s32, s32);
extern Game1DD500Profile D_800A3FD8[];
extern vertex D_800AA120;

/* Joint matrix of an actor; include/ types the array as struct255 *. */
#define JOINT_MTX(actor, joint) (&((Mtx *)(actor)->unk1D4)[joint])

/* Effect profile index for an actor; actor types 0x53 and 0xA5 (and no actor) use profile 4. */
#define EFFECT_PROFILE(actor) \
    ((actor) != NULL ? (((actor)->id == 0x53 || (actor)->id == 0xA5) ? 4 : func_15134070(actor)) : 4)

/* Spawns one randomized particle at (x, y, z) launched in direction (yaw, pitch). */
s32 func_151B0050(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7,
                  s16 pitch, s16 yaw, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 pos[3];
    f32 dir[3];
    f32 speed;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    speed = func_150ADA68() * 5.3f + 4.4f;
    func_15143794(yaw, pitch, speed, dir);
    // matching: a literal 0.0f scale is folded away; IDO keeps the multiply for (0.68f - 0.68f)
    func_151DA6F8(pos, dir, func_150ADA68() * (0.68f - 0.68f) + 0.68f, func_150ADA20() % 20 + 31,
                  func_150ADA20() % 101 + 155, func_150ADA68() * 2.33f + 6.7f,
                  (func_150ADA20() & 1) + 3, 1, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0, argE, 1);
    return 1;
}

/* Spawns an impact effect at joint 6 of actor, styled by the profile of other. */
void func_151B01B8(struct127 *actor, struct127 *other) {
    Game1DD500Impact impact;
    u8 shade;
    Game1DD500Ground ground;
    s32 profile;

    if (actor != NULL && actor->unk1D4 != NULL && (actor->unk74 & 0xF) != 0xF) {
        func_1504715C(&ground, actor);
        profile = EFFECT_PROFILE(other);
        if (profile != 99) {
            if (D_800A3FD8[profile].unkE != 2) {
                shade = (actor->unk7A >> 8) + 0x40;
                func_15143134(&D_800AA120, &impact.position, JOINT_MTX(actor, 6));
                impact.unk2C = 10;
                impact.unk2E = 20;
                impact.unk00 = shade - 0x68;
                impact.unk14 = 7.22f;
                impact.unk02 = 0xD0;
                impact.unk04 = -0x1B;
                impact.unk06 = 0x36;
                impact.unk30 = 3;
                impact.unk32 = 2;
                impact.unk34 = 30;
                impact.unk36 = 40;
                impact.unk38 = 155;
                impact.unk3A = 100;
                impact.unk18 = 3.04f;
                impact.unk1C = 0.810000062f;
                impact.unk20 = 0.405000031f;
                impact.unk24 = 6.0f;
                impact.unk28 = 13.90000057f;
                impact.unk40 = 0.5f;
                if (D_800A3FD8[profile].unkE == 1) {
                    impact.unk3C = 1;
                } else {
                    impact.unk3C = 0;
                }
                impact.unk44 = 16;
                impact.unk46 = 15;
                impact.unk48 = 0;
                func_15153F18(&impact, &impact.position, &ground, 0xFF, 1);
            }
        }
    }
}

void func_15143874(s16 angle, f32 radius, f32 *outX, f32 *outZ);
s32 func_15046C80(f32 *, s32, f32, Game1DD500Ground *);
s32 func_1504697C(f32 *, s32, f32, Game1DD500Ground *);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, u8, s32);

/* Big landing burst at actor: an impact effect, 12-17 splats scattered on the ground around it, and a mark under it. */
void func_151B03B8(struct127 *actor, u8 arg1, s32 arg2) {
    Game1DD500Impact impact;
    Game1DD500Ground ground;
    s32 count;
    Game1DD500Ground scatterGround;
    f32 probe[3];
    f32 spot[3];
    f32 center[3];
    Game1DD500Ground centerGround;

    if (actor != NULL) {
        func_1504715C(&ground, actor);
        impact.position.x = actor->x_position;
        impact.position.y = actor->y_position;
        impact.position.z = actor->z_position;
        impact.unk14 = 5.08f;
        impact.unk2C = 10;
        impact.unk02 = 0xFF;
        impact.unk04 = -0x3F;
        impact.unk2E = 0;
        impact.unk00 = 0;
        impact.unk06 = 0x2D;
        impact.unk30 = 3;
        impact.unk32 = 2;
        impact.unk34 = 40;
        impact.unk36 = 20;
        impact.unk38 = 155;
        impact.unk3A = 100;
        impact.unk3C = 0;
        impact.unk44 = 16;
        impact.unk46 = 15;
        impact.unk48 = 0;
        impact.unk18 = 3.02f;
        impact.unk1C = 0.807000041f;
        impact.unk20 = 0.701000035f;
        impact.unk24 = 18.0f;
        impact.unk28 = 11.8f;
        impact.unk40 = 0.5f;
        func_15153F18(&impact, &impact.position, &ground, 0xFF, 1);

        count = func_150ADA20() % 6 + 12;
        scatterGround.unk18 = 0;
        scatterGround.unk1C = 0;
        scatterGround.unk1D = 0;
        scatterGround.unk20 = 0;
        scatterGround.height = -10000.0f;
        probe[1] = actor->y_position + 100.0f;
        if (count != 0) {
            // matching: a do/while (--count != 0) loop schedules differently from this break form
            for (;;) {
                func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 100.0f + 110.0f, &spot[0], &spot[2]);
                spot[0] += actor->x_position;
                spot[2] += actor->z_position;
                probe[0] = spot[0];
                probe[2] = spot[2];
                if (!func_15046C80(probe, 0, actor->y_position - 500.0f, &scatterGround) || scatterGround.unk1D == 3) {
                    return;
                }
                spot[1] = scatterGround.height;
                spot[1] += 10.0f;
                // matching: (f32)100 is not CSE'd with the 100.0f above, which would tie up an FP register
                if (func_150ADA20() & 1) {
                    func_151D9B8C(0, func_150ADA68() * 4.5f + 15.0f, (u32)(func_150ADA68() * (f32)100 + 155.0f),
                                  scatterGround.surface, spot, 100, 0, 1, 0, arg1, arg2);
                } else {
                    func_151DAB58(0, func_150ADA68() * 5.9f + 6.1f, (u32)(func_150ADA68() * (f32)100 + 155.0f),
                                  spot, 1, arg1, arg2);
                }
                if (--count == 0) {
                    break;
                }
            }
        }

        center[0] = actor->x_position;
        center[1] = actor->y_position;
        center[2] = actor->z_position;
        centerGround.unk20 = 0;
        centerGround.unk1D = 0;
        centerGround.unk1C = 0;
        centerGround.unk18 = 0;
        centerGround.height = -10000.0f;
        {
            f32 above[3];

            above[0] = center[0];
            above[1] = center[1] + 100.0f;
            above[2] = center[2];
            if (!func_1504697C(above, 0, center[1] - 500.0f, &centerGround) || centerGround.unk1D == 3) {
                return;
            }
        }
        center[1] = centerGround.height;
        center[1] += 10.0f;
        func_15136698((func_150ADA68() * 40.0f + 120.0f) * 0.001f, (func_150ADA68() * 123.0f + 123.0f) * 0.00001f,
                      func_150ADA20() % 101 + 155, func_150ADA20() % 101 + 100, func_150ADA20() % 56 + 70,
                      centerGround.surface, center, 0, 1, arg1, arg2);
    }
}

/* Per-particle motion block at 0x110 of a particle object. */
typedef struct Game1DD500Motion {
    /* 0x00 */ vertex velocity;
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
typedef struct Game1DD500Particle {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xF];
    /* 0x01C */ s16 life;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 height;
    /* 0x034 */ vertex position;
    /* 0x040 */ vertex previous;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x1B];
    /* 0x078 */ Game1DD500Ground ground;
    /* 0x09C */ u8 pad9C[0x24];
    /* 0x0C0 */ Vtx quadTemplate[4];
    /* 0x100 */ Vtx *quads[4];
    /* 0x110 */ Game1DD500Motion motion;
} Game1DD500Particle;

/* A particle held at one of the owner's joints until its timer runs out. */
typedef struct Game1DD500Slot {
    /* 0x0 */ Game1DD500Particle *particle;
    /* 0x4 */ s32 timer;
    /* 0x8 */ struct Game1DD500Slot *next; /* ring of free slots while spawning */
} Game1DD500Slot;

/* Effect that keeps particles stuck to an actor's joints and drops them over time. */
typedef struct Game1DD500Emitter {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;   /* owner->unique_id at creation */
    /* 0x08 */ s32 count;    /* attached particles */
    /* 0x0C */ Game1DD500Slot slots[11];
    /* 0x90 */ f32 spawnTime;
    /* 0x94 */ u8 profileFlag;
} Game1DD500Emitter; // size 0x98

/* Effect task: a struct260 header followed by the effect's data. */
typedef struct Game1DD500Task {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;   /* -1 ends the task */
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Game1DD500Emitter emitter;
} Game1DD500Task;

/* Message 0x13 (and 0): this actor's effects should end. */
typedef struct Game1DD500ActorMsg {
    /* 0x0 */ struct127 *actor;
    /* 0x4 */ u8 uniqueId;
} Game1DD500ActorMsg;

/* Message 0x2D: two actors swapped identities. */
typedef struct Game1DD500SwapMsg {
    /* 0x0 */ struct127 *a;
    /* 0x4 */ struct127 *b;
    /* 0x8 */ u8 uniqueIdA;
    /* 0x9 */ u8 uniqueIdB;
} Game1DD500SwapMsg;

void func_151B1918(Game1DD500Task *task);

void *memcpy(void *, const void *, u32);

/* Starts an emitter task on actor (lifetime 300 when -1), first telling older ones on it to end. */
Game1DD500Task *func_151B09BC(struct127 *actor, struct127 *other, s16 lifetime, u8 arg3, s32 arg4) {
    Game1DD500Task *task;
    Game1DD500Emitter emitter;
    s32 profile;
    s16 life;
    u8 timed;
    Game1DD500ActorMsg msg;

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
    task = (Game1DD500Task *)func_151491F4(life, -1, 0x13, timed, 0xF, sizeof(emitter), arg3, arg4);
    if (task != NULL) {
        memcpy(&task->emitter, &emitter, sizeof(emitter));
    }
    return task;
}

/* A joint and an offset in that joint's space where an emitter particle sits. */
typedef struct Game1DD500Mount {
    /* 0x0 */ u8 joint;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ vertex offset;
} Game1DD500Mount;

Game1DD500Particle *func_1513D524(ParticleDef *, u8, u8, u8, u8, u8, s32, u8, s32);
void func_1513F680(Game1DD500Particle *, u8, u8, u8, u8);
extern Game1DD500Mount D_800AA12C[11];
extern Game1DD500Mount D_800AA1DC[11]; /* actor type 0x36 */
extern u8 D_800AB310[];

/* Emitter task update: carries attached particles with the owner's joints, releases expired ones and spawns new ones. */
void func_151B0B88(Game1DD500Task *task) {
    Game1DD500Emitter *emitter = &task->emitter;
    u8 failed; // matching: ending the task directly in the check reorders the whole function

    failed = FALSE;
    if (emitter->owner->interaction_state == 0 || emitter->owner->unique_id != emitter->ownerId) {
        failed = TRUE;
    }
    if (!failed) {
        Game1DD500Slot *head;
        Game1DD500Slot *last;
        s32 i;
        u32 freeCount;

        if (emitter->owner->unk1D4 != NULL && (emitter->owner->unk74 & 0xF) != 0xF) {
            head = NULL;
            last = NULL;
            freeCount = 0;
            for (i = 0; i < 11; i++) {
                Game1DD500Slot *slot = &emitter->slots[i];
                s32 isFree;

                if (slot->particle != NULL) {
                    slot->particle->previous = slot->particle->position;
                    func_15143134(emitter->owner->id == 0x36 ? &D_800AA1DC[i].offset : &D_800AA12C[i].offset,
                                  &slot->particle->position,
                                  emitter->owner->id == 0x36 ? JOINT_MTX(emitter->owner, D_800AA1DC[i].joint)
                                                             : JOINT_MTX(emitter->owner, D_800AA12C[i].joint));
                    slot->timer -= D_800BE9E4;
                    if (slot->timer < 0) {
                        vertex *velocity = &slot->particle->motion.velocity;

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
                                                 task->unkC, task->unk1);
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
        task->unkE = -1;
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
    f32 probe[3];
    f32 splat[3];
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
        probe[0] = particle->position.x;
        probe[1] = oldY - particle->height;
        probe[2] = particle->position.z;
        if (func_15046C80(probe, 0, particle->position.y - particle->height, &particle->ground)) {
            splat[0] = probe[0];
            splat[1] = particle->ground.height + 5.0f;
            splat[2] = probe[2];
            size = ((motion->baseWidth + motion->swingWidth * 0.5f) + (motion->baseHeight + motion->swingHeight * 0.5f)) * 0.5f;
            if (func_150ADA20() & 1) {
                func_151D9B8C(motion->unk25, size * 1.7f, particle->alpha, particle->ground.surface, splat, 100, 0, 1, 0,
                              particle->unkC, particle->unk1);
            } else {
                func_151DAB58(motion->unk25, size * 0.68f, particle->alpha, splat, 1, particle->unkC, particle->unk1);
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

void func_151D5D60(Vtx **buffers, s16 view, s32 size, Vtx **out, u8 *fresh);

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
            func_1516972C((struct102 *)slot->particle);
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
    Game1DD500ActorMsg *actorMsg = msg;
    Game1DD500SwapMsg *swap = msg;
    struct127 *owner = emitter->owner;

    if (type == 0 || type == 0x13) {
        if (owner == actorMsg->actor || actorMsg->uniqueId == emitter->ownerId) {
            func_1516972C((struct102 *)task);
        }
    } else if (type == 0x2D) {
        if (emitter->owner == swap->a) {
            emitter->owner = swap->b;
            emitter->ownerId = swap->uniqueIdB;
        } else if (emitter->owner == swap->b) {
            emitter->owner = swap->a;
            emitter->ownerId = swap->uniqueIdA;
        }
    }
}

/* Task handler: frees the attached particles, then passes the task on to func_1514933C. */
void func_151B1A58(Game1DD500Task *task) {
    func_151B1918(task);
    func_1514933C((struct260 *)task);
}

/* Task handler: frees the attached particles, then passes the task on to func_15149368. */
void func_151B1A84(Game1DD500Task *task) {
    func_151B1918(task);
    func_15149368((struct260 *)task);
}
