#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Live particle spawned by an emitter (see func_15130280). */
typedef struct Particle Particle;

/* One entry of an emitter's slot pool; slots are chained by index, -1 ends a chain. */
typedef struct {
    /* 0x00 */ Particle *particle;
    /* 0x04 */ s16 next;
    /* 0x06 */ s16 pad6;
} EmitterSlot;

/* Describes an emitter; func_150C0AC0 copies it into the emitter. */
typedef struct {
    /* 0x00 */ vertex *source;     /* followed when flags & 2 */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct127 *target;  /* followed otherwise */
    /* 0x0C */ u8 targetId;        /* target->unique_id; a mismatch means the object was reused */
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ f32 rate;           /* particles per frame step while the target moves */
    /* 0x14 */ s16 slotCount;
    /* 0x16 */ s16 timer;          /* counts down when flags & 1 */
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 pad19[3];
} EmitterDesc; /* size 0x1C */

/* Emitter object that keeps spawning particles behind a moving object. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ EmitterDesc desc;
    /* 0x34 */ f32 lastX;
    /* 0x38 */ f32 lastZ;
    /* 0x3C */ f32 spawnAccum;
    /* 0x40 */ EmitterSlot *slots; /* desc.slotCount slots, stored right after the emitter */
    /* 0x44 */ s16 activeHead;     /* chain of slots holding a live particle */
    /* 0x46 */ s16 freeHead;       /* chain of unused slots */
} Emitter; /* size 0x48 */

struct Particle {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ vertex anchor;
    /* 0x4C */ u8 pad4C[0xA8 - 0x4C];
    /* 0xA8 */ Emitter *owner;
};

/* Effect callback: hands func_15169850 the emitter's target reference. */
void func_150C09B0(Emitter *emitter, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&emitter->desc.target, (s32)&emitter->desc.targetId, (s32)emitter);
}

void func_150C0A48(Emitter *emitter);

/* Emitter destructors: free the live particles, then the emitter. */
void func_150C09F0(Emitter *emitter) {
    func_150C0A48(emitter);
    func_15169804((struct102 *)emitter);
}

extern void func_15169824(struct102 *);

void func_150C0A1C(Emitter *emitter) {
    func_150C0A48(emitter);
    func_15169824((struct102 *)emitter);
}

/* Frees every live particle of the emitter. */
void func_150C0A48(Emitter *emitter) {
    s16 i;

    for (i = emitter->activeHead; i != -1; i = emitter->slots[i].next) {
        func_1516972C((struct102 *)emitter->slots[i].particle);
    }
}

/* Allocates an emitter from desc with desc->slotCount particle slots, all initially free. */
Emitter *func_150C0AC0(EmitterDesc *desc, s32 arg1, s32 arg2) {
    Emitter *emitter;
    s32 size;
    s16 i;

    size = desc->slotCount * sizeof(EmitterSlot) + sizeof(Emitter);
    if (!(desc->flags & 2)) {
        if (desc->target == NULL) {
            return NULL;
        }
        if (desc->target->interaction_state == 0) {
            return NULL;
        }
    }

    emitter = func_15167A68(0x25, arg2, size, 1, (u8)arg1, 1);
    if (emitter == NULL) {
        return NULL;
    }

    memcpy(&emitter->desc, desc, sizeof(EmitterDesc));
    emitter->slots = (EmitterSlot *)(emitter + 1);
    if (desc->flags & 2) {
        emitter->lastX = emitter->desc.source->x;
        emitter->lastZ = emitter->desc.source->z;
    } else {
        emitter->lastX = emitter->desc.target->x_position;
        emitter->lastZ = emitter->desc.target->z_position;
    }
    emitter->spawnAccum = 0.0f;
    emitter->activeHead = -1;
    emitter->freeHead = 0;
    for (i = 0; i < desc->slotCount - 1; i++) {
        emitter->slots[i].next = i + 1;
    }
    emitter->slots[desc->slotCount - 1].next = -1;
    emitter->unk10 = 1;
    emitter->unk14 = 0;
    return emitter;
}

/* Particle spawn template passed to func_15130280. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 scaleX;
    /* 0x2C */ f32 scaleY;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex vel;
    /* 0x48 */ vertex unk48;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ s8 unk65;
    /* 0x66 */ u8 pad66[2];
} ParticleSpawn; /* size 0x68 */

extern Particle *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);

/* Emitter update: follows its target and, while the target moves, spawns particles around it. */
void func_150C0C38(Emitter *emitter) {
    Emitter *self; // matching: IDO's register/stack allocation needs this copy of the parameter
    u8 kill;
    vertex pos;
    struct127 *target;
    s16 i;
    s32 bit;
    ParticleSpawn spawn;
    Emitter *owner;

    self = emitter;
    kill = FALSE;
    if (!(emitter->desc.flags & 2)) {
        target = emitter->desc.target;
        if (target->interaction_state == 0) {
            kill = TRUE;
        }
        if (target->unique_id != emitter->desc.targetId) {
            kill = TRUE;
        }
        pos.x = target->x_position;
        pos.y = emitter->desc.target->unk180;
        pos.z = emitter->desc.target->z_position;
    } else {
        memcpy(&pos, emitter->desc.source, sizeof(vertex));
        pos.y -= 65.0f;
    }
    if (!kill) {
        s32 active;

        active = TRUE;
        for (i = emitter->activeHead; i != -1; i = emitter->slots[i].next) {
            emitter->slots[i].particle->anchor = pos;
        }
        if (emitter->desc.flags & 4) {
            if (emitter->desc.target->unk84.uh != 0x1D && emitter->desc.target->unk84.uh != 0x1E) {
                active = FALSE;
            }
        }
        if (emitter->desc.target->xz_velocity < 3.0f) {
            active = FALSE;
        }
        if ((pos.x != emitter->lastX || pos.z != emitter->lastZ) && active) {
            emitter->spawnAccum += emitter->desc.rate * D_800BE9A4;
            if (emitter->spawnAccum > 1.0f) {
                s16 slot;
                u8 angle;
                f32 sinA;
                f32 cosA;
                f32 speed;

                spawn.unk1D = 0x6C;
                spawn.unk8 = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.unk4 = 0x1F0600;
                spawn.unkC = 0;
                spawn.unk10 = 0;
                spawn.unk17 = 0xFF;
                spawn.unk1C = 0xFF;
                spawn.unk48.x = 0.0f;
                spawn.unk48.y = 0.0f;
                spawn.unk48.z = 0.0f;
                spawn.unk54 = 0.0f;
                spawn.unk1E = 30;
                spawn.unk20 = 8;
                spawn.flags = 0x80DE01;
                spawn.unk60 = 8;
                spawn.unk61 = 6;
                spawn.unk62 = -1;
                spawn.unk63 = 0;
                spawn.unk64 = -1;
                spawn.unk65 = 0;
                owner = emitter;
                while (emitter->spawnAccum > 1.0f && self->freeHead != -1) {
                    slot = emitter->freeHead;
                    angle = func_150ADA20() & 0xFF; // matching: the redundant mask changes IDO's code for the call result
                    sinA = func_151423D8(angle - 0x40);
                    cosA = func_151423D8(angle);
                    speed = func_150ADA68() * 33.0f + 40.0f;
                    spawn.unk18 = 0x52;
                    spawn.unk19 = 0x6B;
                    spawn.unk1A = 0x97;
                    spawn.unk14 = 0x87;
                    spawn.unk15 = 0x6B;
                    spawn.unk16 = 0x97;
                    spawn.unk1B = func_150ADA20() % 121U + 80;
                    spawn.scaleX = spawn.scaleY = func_150ADA68() * 17.0f + 67.0f;
                    spawn.pos = pos;
                    spawn.vel.x = speed * sinA;
                    spawn.vel.y = 0.0f;
                    spawn.vel.z = speed * cosA;
                    spawn.flags &= ~0xC0;
                    bit = (func_150ADA20() & 1) ? 0x80 : 0;
                    spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | bit;
                    spawn.unkA = spawn.unk22 = func_150ADA20() % 22U + 70;
                    spawn.unk24 = 1.035226941f;
                    emitter->slots[slot].particle = func_15130280(&spawn, 1, 0, 4, emitter->unkC, emitter->unk1);
                    if (emitter->slots[slot].particle != NULL) {
                        memcpy(&emitter->slots[slot].particle->owner, &owner, sizeof(owner));
                        emitter->freeHead = emitter->slots[slot].next;
                        emitter->slots[slot].next = emitter->activeHead;
                        emitter->activeHead = slot;
                    }
                    emitter->spawnAccum -= 1.0f;
                }
                while (emitter->spawnAccum > 1.0f) {
                    emitter->spawnAccum -= 1.0f;
                }
            }
        }
        emitter->lastX = pos.x;
        emitter->lastZ = pos.z;
    }
    if (emitter->desc.flags & 1) {
        emitter->desc.timer -= D_800BE9E4;
        if (emitter->desc.timer < 0) {
            kill = TRUE;
        }
    }
    if (kill) {
        func_1516972C((struct102 *)emitter);
    }
}

/* Particle destructor hook: moves the particle's slot from its emitter's active chain to the free chain. */
void func_150C1198(Particle *particle) {
    Emitter *emitter;
    s32 found;
    s16 prev;
    s16 i;

    emitter = particle->owner;
    found = FALSE;
    prev = -1;
    i = emitter->activeHead;
    while (i != -1 && !found) {
        if (particle == emitter->slots[i].particle) {
            found = TRUE;
        } else {
            prev = i;
            i = emitter->slots[i].next;
        }
    }

    if (found) {
        if (i == emitter->activeHead) {
            emitter->activeHead = emitter->slots[i].next;
        } else {
            emitter->slots[prev].next = emitter->slots[i].next;
        }
        emitter->slots[i].next = emitter->freeHead;
        emitter->freeHead = i;
    }
}
