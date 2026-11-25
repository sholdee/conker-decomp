#include <ultra64.h>
/* functions.h declares these three with stale signatures; hide them and declare the real ones. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_decl_in_functions_h
#define func_1513D524 func_1513D524_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
#undef func_1513D524
void *func_1513D524(void *, u8, u8, u8, u8, u8, s32, u8, s32);
void func_151D9B8C(u8, f32, u8, void *, f32 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

typedef struct { f32 x, y, z; } Vec3f;

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
    /* 0x08 */ Vec3f position;
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
void func_15143134(Vec3f *, Vec3f *, Mtx *);
void func_15153F18(Game1DD500Impact *, Vec3f *, Game1DD500Ground *, s32, s32);
extern Game1DD500Profile D_800A3FD8[];
extern Vec3f D_800AA120;

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
    func_151DA6F8(pos, dir, func_150ADA68() * (0.68f - 0.68f) + 0.68f, (s16)(func_150ADA20() % 20 + 31),
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

/* TEMP compat */
typedef struct {
    u8 pad_0x0[0x10];
    u8 field_0x10;
    u8 field_0x11;
    s8 field_0x12;
    s8 field_0x13;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
} CompactOscillatorMotionState;
typedef struct {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    CompactOscillatorMotionState field_0x110;
} CompactOscillatorObject;
void func_15149368(struct260 *arg0);
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
    /* 0x00 */ Vec3f velocity;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phase[2];      /* oscillator angles for width, height */
    /* 0x12 */ s8 phaseSpeed[2];
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
    /* 0x034 */ Vec3f position;
    /* 0x040 */ Vec3f previous;
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

/* Spawn description passed to func_1513D524; laid out like the particle from 0x18 on. */
typedef struct Game1DD500ParticleDesc {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 life;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f position;
    /* 0x28 */ Vec3f previous;
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 alpha;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 pad46[0x12];
} Game1DD500ParticleDesc; // size 0x58

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
    /* 0x4 */ Vec3f offset;
} Game1DD500Mount;

void func_1513F680(void *, u8, u8, u8, u8);
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
                    Game1DD500ParticleDesc desc;
                    Game1DD500Motion motion;

                    last->next = head;
                    desc.unk0 = D_800AB310[emitter->profileFlag];
                    desc.flags = 0x1001;
                    desc.life = func_150ADA20() % 51 + 52;
                    desc.unk8 = 0;
                    desc.unkC = 0;
                    desc.color[0] = 0;
                    desc.color[1] = 0;
                    desc.color[2] = 0;
                    desc.color[3] = 0xFF;
                    desc.unk40 = 0;
                    desc.scale.x = 1.0f;
                    desc.scale.y = 1.0f;
                    desc.scale.z = 1.0f;
                    motion.phase[0] = 0;
                    motion.phase[1] = 0;
                    desc.previous.x = 0.0f;
                    desc.previous.y = 0.0f;
                    desc.previous.z = 0.0f;
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
                        desc.alpha = func_150ADA20() % 101 + 155;
                        motion.phaseSpeed[0] = func_150ADA20() % 5 + 4;
                        motion.phaseSpeed[1] = func_150ADA20() % 5 + 4;
                        // matching: "- 703.0f" emits sub.s; the original adds a negative constant
                        motion.gravity = (func_150ADA68() * 506.0f + -703.0f) * 0.001f;
                        func_15143134(&D_800AA12C[index].offset, &desc.position,
                                      JOINT_MTX(emitter->owner, D_800AA12C[index].joint));
                        desc.previous = desc.position;
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

typedef struct {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 ay;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
} Game1DD500Proj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x23];
    f32 unk30;
    f32 pos[3];
    f32 lag[3];
    u8 pad4C[0x10];
    u8 unk5C;
    u8 pad5D[0x1B];
    f32 unk78;
    f32 unk7C;
    u8 pad80[0x90];
    Game1DD500Proj proj;
} Game1DD500ProjObj;

s32 func_151B118C(Game1DD500ProjObj *arg0) {
    f32 dx;
    f32 dz;
    f32 dy;
    Game1DD500Proj *p;
    s8 ret;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;
    f32 probe[3];
    f32 pos[3];
    f32 size;

    p = &arg0->proj;
    oldY = arg0->pos[1];
    oldVx = p->vx;
    oldVz = p->vz;
    ret = 1;
    p->vx *= 0.25f * D_800BE9A4;
    p->vz *= 0.25f * D_800BE9A4;
    dx = (p->vx + oldVx) * 0.5f * D_800BE9A4;
    dy = p->vy * D_800BE9A4 + p->ay * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (p->vz + oldVz) * 0.5f * D_800BE9A4;
    p->vy += p->ay * D_800BE9A4;
    arg0->pos[0] += dx;
    arg0->pos[1] += dy;
    arg0->pos[2] += dz;
    arg0->lag[0] += dx;
    arg0->lag[1] += dy;
    arg0->lag[2] += dz;
    arg0->lag[0] = (arg0->lag[0] - arg0->pos[0]) * 0.706000030f + arg0->pos[0];
    arg0->lag[1] = (arg0->lag[1] - arg0->pos[1]) * 0.706000030f + arg0->pos[1];
    arg0->lag[2] = (arg0->lag[2] - arg0->pos[2]) * 0.706000030f + arg0->pos[2];
    if (p->unk24) {
        probe[0] = arg0->pos[0];
        probe[1] = oldY - arg0->unk30;
        probe[2] = arg0->pos[2];
        if (func_15046C80(probe, 0, arg0->pos[1] - arg0->unk30, &arg0->unk78)) {
            pos[0] = probe[0];
            pos[1] = arg0->unk78 + 5.0f;
            pos[2] = probe[2];
            size = ((p->unk14 + p->unk1C * 0.5f) + (p->unk18 + p->unk20 * 0.5f)) * 0.5f;
            if (func_150ADA20() & 1) {
                func_151D9B8C(p->unk25, size * 1.70000005f, arg0->unk5C, (void *)&arg0->unk7C, pos, 100, 0, 1, 0, arg0->unkC, arg0->unk1);
            } else {
                func_151DAB58(p->unk25, size * 0.680000007f, arg0->unk5C, pos, 1, arg0->unkC, arg0->unk1);
            }
            ret = 0;
        }
    }
    return ret;
}

s32 func_151B1478(struct260 *arg0) {
    s32 v1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) < 0x20) {
        v1 = *(s16 *)((u8 *)arg0 + 0x1C) << 3;
        if (v1 < *(u8 *)((u8 *)arg0 + 0x5C)) {
            *(u8 *)((u8 *)arg0 + 0x5C) = v1;
        }
    }
    return 1;
}

struct Struct151B14AC {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
};

extern void func_151D5D60();

s16 *func_151B14AC(struct Struct151B14AC *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 sinval;
    f32 temp_f18;
    f32 temp_y_cos;
    f32 temp_y_sin;
    f32 temp_f2;
    f32 var_f14;
    f32 var_f2;
    f32 *temp_v1;
    f32 temp_f0;
    u8 sp3B;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp3B);
    ret = var_v0;
    if (var_v0) {
        if (sp3B) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_v1 = &D_800DD1E8[arg1];
    var_f14 = -1.0f;
    var_f2 = ((*temp_v1 * (arg0->unk40 - arg0->unk34)) - (D_800DD1D8[arg1] * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f2 < 0.0f) {
        var_f14 = 1.0f;
        var_f2 = -var_f2;
    }

    temp_f0 = sqrtf(var_f2) * 0.25f;
    if (1.570796371f < temp_f0) {
        temp_f0 = 1.570796371f;
    }

    temp_f0 = temp_f0 * var_f14;
    sinval = sinf(temp_f0);
    temp_f0 = cosf(temp_f0);
    temp_f2 = arg0->unk2C * temp_f0;
    temp_f18 = arg0->unk2C * sinval;
    temp_y_cos = arg0->unk30 * temp_f0;
    temp_y_sin = arg0->unk30 * sinval;

    var_v0[0] = (*temp_v1 * -temp_f2) + arg0->unk34;
    var_v0[1] = arg0->unk38 + temp_f18;
    var_v0[2] = arg0->unk3C - (D_800DD1D8[arg1] * -temp_f2);
    var_v0[3] = 0;

    var_v0[8] = (*temp_v1 * temp_f2) + arg0->unk34;
    var_v0[9] = arg0->unk38 - temp_f18;
    var_v0[10] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f2);
    var_v0[11] = 0;

    temp_f0 = temp_f2 - temp_y_sin;
    var_v0[16] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[17] = (arg0->unk38 - temp_f18) - temp_y_cos;
    var_v0[18] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f0);
    var_v0[19] = 0;

    temp_f0 = -(temp_f2 + temp_y_sin);
    var_v0[24] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[25] = (arg0->unk38 + temp_f18) - temp_y_cos;
    var_v0[26] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f0);
    var_v0[27] = 0;

    return ret;
}


s32 func_151B1828(CompactOscillatorObject *arg0) {
    CompactOscillatorObject *p = arg0;
    CompactOscillatorMotionState *f = &arg0->field_0x110;
    f32 r1;
    f32 r2;
    f->field_0x10 = f->field_0x10 + f->field_0x12 * D_800BE9E4;
    f->field_0x11 = f->field_0x11 + f->field_0x13 * D_800BE9E4;
    r1 = func_151423D8((u8)(f->field_0x10 - 0x40));
    r2 = func_151423D8((u8)(f->field_0x11 - 0x40));
    p->field_0x2C += (f->field_0x14 + f->field_0x1C * r1 - p->field_0x2C) * 0.5f;
    p->field_0x30 += (f->field_0x18 + f->field_0x20 * r2 - p->field_0x30) * 0.5f;
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

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151B19A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151B19A4;

void func_151B19A4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151B19A4 *temp_v0 = (SubA151B19A4 *)((u8 *)arg0 + 0x28);
    ArgB151B19A4 *b = (ArgB151B19A4 *)arg1;
    s32 v1 = temp_v0->unk0;

    if (arg2 == 0 || arg2 == 0x13) {
        if ((v1 == b->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

void func_151B1A58(struct260 *arg0) {
    func_151B1918(arg0);
    func_1514933C(arg0);
}

void func_151B1A84(struct260 *arg0) {
    func_151B1918(arg0);
    func_15149368(arg0);
}
