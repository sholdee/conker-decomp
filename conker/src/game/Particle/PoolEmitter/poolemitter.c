#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Effect callback: hands effect_handleOwnerActorEvent the emitter's target reference. */
void particle_onPoolEmitterEvent(PoolEmitter *emitter, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &emitter->desc.target, &emitter->desc.targetId, emitter);
}

/* Emitter destructors: free the live particles, then the emitter. */
void particle_destroyPoolEmitter(PoolEmitter *emitter) {
    particle_freePoolParticles(emitter);
    effect_defaultDestroy(&emitter->header);
}


void particle_freePoolEmitter(PoolEmitter *emitter) {
    particle_freePoolParticles(emitter);
    effect_defaultFree(&emitter->header);
}

/* Frees every live particle of the emitter. */
void particle_freePoolParticles(PoolEmitter *emitter) {
    s16 i;

    for (i = emitter->activeHead; i != -1; i = emitter->slots[i].next) {
        effect_destroyObject(&emitter->slots[i].particle->head.header);
    }
}

/* Allocates an emitter from desc with desc->slotCount particle slots, all initially free. */
PoolEmitter *particle_newPoolEmitter(PoolEmitterDesc *desc, s32 arg1, s32 arg2) {
    PoolEmitter *emitter;
    s32 size;
    s16 i;

    size = desc->slotCount * sizeof(PoolEmitterSlot) + sizeof(PoolEmitter);
    if (!(desc->flags & 2)) {
        if (desc->target == NULL) {
            return NULL;
        }
        if (desc->target->type == 0) {
            return NULL;
        }
    }

    emitter = effect_newObject(0x25, arg2, size, 1, (u8)arg1, 1);
    if (emitter == NULL) {
        return NULL;
    }

    memcpy(&emitter->desc, desc, sizeof(PoolEmitterDesc));
    emitter->slots = (PoolEmitterSlot *)(emitter + 1);
    if (desc->flags & 2) {
        emitter->lastX = emitter->desc.source->x;
        emitter->lastZ = emitter->desc.source->z;
    } else {
        emitter->lastX = emitter->desc.target->xpos;
        emitter->lastZ = emitter->desc.target->zpos;
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

/* Emitter update: follows its target and, while the target moves, spawns particles around it. */
void particle_updatePoolEmitter(PoolEmitter *emitter) {
    PoolEmitter *self; // matching: IDO's register/stack allocation needs this copy of the parameter
    u8 kill;
    Vec3f pos;
    Actor *target;
    s16 i;
    s32 bit;
    ParticleDesc68 spawn;
    PoolEmitter *owner;

    self = emitter;
    kill = FALSE;
    if (!(emitter->desc.flags & 2)) {
        target = emitter->desc.target;
        if (target->type == 0) {
            kill = TRUE;
        }
        if (target->unique_id != emitter->desc.targetId) {
            kill = TRUE;
        }
        pos.x = target->xpos;
        pos.y = emitter->desc.target->floorY;
        pos.z = emitter->desc.target->zpos;
    } else {
        memcpy(&pos, emitter->desc.source, sizeof(Vec3f));
        pos.y -= 65.0f;
    }
    if (!kill) {
        s32 active;

        active = TRUE;
        for (i = emitter->activeHead; i != -1; i = emitter->slots[i].next) {
            emitter->slots[i].particle->head.desc.pos = pos;
        }
        if (emitter->desc.flags & 4) {
            if (emitter->desc.target->animseq.uh != 0x1D && emitter->desc.target->animseq.uh != 0x1E) {
                active = FALSE;
            }
        }
        if (emitter->desc.target->speed < 3.0f) {
            active = FALSE;
        }
        if ((pos.x != emitter->lastX || pos.z != emitter->lastZ) && active) {
            emitter->spawnAccum += emitter->desc.rate * gFrameDelta;
            if (emitter->spawnAccum > 1.0f) {
                s16 slot;
                u8 angle;
                f32 sinA;
                f32 cosA;
                f32 speed;

                spawn.texture = 0x6C;
                spawn.renderMode = 0x5103;
                spawn.unk0 = 0x200005;
                spawn.unk4 = 0x1F0600;
                spawn.frame = 0;
                spawn.frameRate = 0;
                spawn.a = 0xFF;
                spawn.brightness = 0xFF;
                spawn.vel.x = 0.0f;
                spawn.vel.y = 0.0f;
                spawn.vel.z = 0.0f;
                spawn.gravity = 0.0f;
                spawn.fadeStart = 30;
                spawn.fadeRate = 8;
                spawn.flags = 0x80DE01;
                spawn.primColorMode = 8;
                spawn.envColorMode = 6;
                spawn.updateFunc = -1;
                spawn.deathFunc = 0;
                spawn.drawFunc = -1;
                spawn.handlerSet = 0;
                owner = emitter;
                while (emitter->spawnAccum > 1.0f && self->freeHead != -1) {
                    slot = emitter->freeHead;
                    angle = math_random() & 0xFF; // matching: the redundant mask changes IDO's code for the call result
                    sinA = math_cosBinAngleMasked(angle - 0x40);
                    cosA = math_cosBinAngleMasked(angle);
                    speed = math_randomFloat() * 33.0f + 40.0f;
                    spawn.r2 = 0x52;
                    spawn.g2 = 0x6B;
                    spawn.b2 = 0x97;
                    spawn.r = 0x87;
                    spawn.g = 0x6B;
                    spawn.b = 0x97;
                    spawn.alpha = math_random() % 121U + 80;
                    spawn.width = spawn.height = math_randomFloat() * 17.0f + 67.0f;
                    spawn.pos = pos;
                    spawn.offset.x = speed * sinA;
                    spawn.offset.y = 0.0f;
                    spawn.offset.z = speed * cosA;
                    spawn.flags &= ~0xC0;
                    bit = (math_random() & 1) ? 0x80 : 0;
                    spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | bit;
                    spawn.timer = spawn.scaleStart = math_random() % 22U + 70;
                    spawn.scaleRate = 1.035226941f;
                    emitter->slots[slot].particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 4, emitter->header.owner, emitter->header.kind);
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
        emitter->desc.timer -= gFrameTicks;
        if (emitter->desc.timer < 0) {
            kill = TRUE;
        }
    }
    if (kill) {
        effect_destroyObject(&emitter->header);
    }
}

/* Particle destructor hook: moves the particle's slot from its emitter's active chain to the free chain. */
void particle_releasePoolSlot(PoolParticle *particle) {
    PoolEmitter *emitter;
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
