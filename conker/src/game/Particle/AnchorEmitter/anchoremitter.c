#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "libc/string.h"

/* matching: no header declares particle_spawnJitterParticle (groundcluster.c): areaemitter.c was compiled with another view */
void *particle_spawnJitterParticle(Vec3f *pos, ParticleSize *scale, s16 life, u8 alpha, const Vec3f *velocity, f32 gravity, f32 drag,
                    f32 easeFactor, u8 updateFn1, u8 updateFn2, u8 expireFn, s32 texture, u8 flags, s16 fadeStart, s16 fadeRate, s32 extraSize, u8 owner,
                    s32 list);

/* What effect_newFollowerGroup (game/Effects/BoneFollower/bonefollower.c) copies to ActorFollowerGroup.actor: the group's fields 0x10-0x1F. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 duration;
    /* 0x08 */ s8 hasDuration; /* the group's flags: bit 0 = timer counts down */
    /* 0x09 */ s8 updateFn;
    /* 0x0A */ s8 trackFn;
    /* 0x0B */ s8 untrackFn;
    /* 0x0C */ s8 expireFn;
    /* 0x0D */ s8 kind;
    /* 0x0E */ u8 padE[2];
} FollowerGroupDesc;

/* One entry of a bone anchor set (gAnchorSets). */
typedef struct BoneAnchor {
    /* 0x00 */ u8 bone;
    /* 0x04 */ Vec3f offset;
} BoneAnchor; /* 0x10 */

typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ Vec3f offset;
    /* 0x14 */ u8 bone;
    /* 0x16 */ s16 duration;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 updateFn;
    /* 0x1A */ u8 expireFn;
    /* 0x1C */ void *sprite;
    /* 0x20 */ AnchorEmitter *parent;
} EmitterParticleDesc;



typedef struct {
    /* 0x00 */ AnchorEmitter *emitter;
    /* 0x04 */ LightJitter jitter;
    /* 0x24 */ ActorRef owner;
    /* 0x2C */ LightEffectHeader header;
    /* 0x34 */ s32 pad34;
} ChildDesc;

/* This file's .data. The emitter templates by kind (particle_getEmitterKindForObj), then variant. */
static const AnchorEmitterTemplate *gAnchorEmitterTemplates[] = {
    D_800A74D8, D_800A7528, D_800A7578, D_800A75C8, D_800A7618, D_800A7668, D_800A76B8,
    D_800A7708, D_800A7758, D_800A77A8, D_800A77F8, D_800A7848, D_800A7898, D_800A78E8,
    D_800A7938, D_800A7988, D_800A79D8, D_800A7A28, D_800A7A78,
};
/* Emitter update and finish callbacks (AnchorEmitterParams.updateFunc, expireFunc); the callbacks take the emitter as
   their own view (effect_updateBurnDamage and effect_clearOnFire, game/Effects/BurnDamage/burndamage.c), so the tables have no prototype. */
static s32 (*gAnchorEmitterUpdateFuncs[])() = { effect_updateBurnDamage };
static void (*gAnchorEmitterExpireFuncs[])() = { effect_clearOnFire };

/* Moves three sprite values toward random targets that are re-rolled on timers. */
s32 particle_wobbleSpriteSize(EffectSprite *sprite) {
    SpriteWobble *state = &sprite->wobble;

    state->timer1 -= gFrameTicks;
    if (state->timer1 < 0) {
        state->timer1 = math_random() % 6U;
        if (math_random() & 3) {
            state->target1 = math_randomFloat() * (state->hi1 - state->lo1) + state->lo1;
        } else {
            state->target1 = math_randomFloat() * (state->peak1 - state->hi1) + state->hi1;
        }
    }
    sprite->head.def.height += (state->target1 - sprite->head.def.height) * 0.359f;
    state->timer0 -= gFrameTicks;
    if (state->timer0 < 0) {
        state->timer0 = math_random() % 17U;
        state->target0 = math_randomFloat() * (state->hi0 - state->lo0) + state->lo0;
    }
    sprite->head.def.width += (state->target0 - sprite->head.def.width) * 0.186f;
    state->timer2 -= gFrameTicks;
    if (state->timer2 < 0) {
        state->timer2 = math_random() % 15U;
        state->target2 = math_randomFloat() * (state->hi2 - state->lo2) + state->lo2;
    }
    sprite->head.def.frameSpeed += (s32)((state->target2 - sprite->head.def.frameSpeed) * 0.231f);
    return 1;
}

/* Like particle_wobbleSpriteSize, but each target is rolled only once. */
s32 particle_easeSpriteSizeOnce(EffectSprite *sprite) {
    SpriteWobble *state = &sprite->wobble;

    if (sprite->wobble.timer0 == 0) {
        state->target0 = math_randomFloat() * (state->hi0 - state->lo0) + state->lo0;
        state->timer0 = 1;
    }
    if (state->timer1 == 0) {
        state->target1 = math_randomFloat() * (state->hi1 - state->lo1) + state->lo1;
        state->timer1 = 1;
    }
    sprite->head.def.height += (state->target1 - sprite->head.def.height) * 0.359f;
    sprite->head.def.width += (state->target0 - sprite->head.def.width) * 0.186f;
    return 1;
}

void particle_broadcastToActorEmitters(s32 event, u8 eventType) {
    IdList1 desc = gActorEmitterTypes;

    effect_sendEventToTypes(desc.ids, 1, event, eventType);
}

/* Message handler of the actor emitter process (type 0x21): message 0x49 names an actor; the emitter dies with it. */
void particle_onActorEmitterMsg(ActorEmitterProcess *self, ActorRef *other, u8 eventType) {
    effect_handleOwnerActorEvent((s32)other, eventType, &self->params.owner, &self->params.ownerId, self);
    if (eventType == 0x49) {
        Actor *selfActor = self->params.owner;
        Actor *otherActor = other->actor;
        u8 selfTag = self->params.ownerId;
        u8 otherTag = other->uniqueId;
        if (selfActor == otherActor || selfTag == otherTag) {
            effect_destroyObject(&self->header);
        }
    }
}

/* Creates emitter `index` of the kind matching `owner`'s object id, with extraSize bytes of the caller's data after it. */
void *particle_startAnchorEmitter(Actor *owner, u8 index, s16 duration, s8 updateFunc, s8 expireFunc, s8 childKind, u8 messageFunc, s32 extraSize,
                    u8 fxOwner, s32 fxList) {
    u8 pad[4]; // matching: unused stack slot
    u8 kind;

    if (owner == NULL) {
        return NULL;
    }
    if (particle_getEmitterKindForObj(owner, &kind, index) == 0) {
        return NULL;
    }
    return particle_newAnchorEmitter(owner, &gAnchorEmitterTemplates[kind][index], duration, updateFunc, expireFunc,
                         childKind, messageFunc, extraSize, fxOwner, fxList);
}

/* Creates an emitter attached to `owner` from `tmpl`; duration -1 means none (uses 300). */
AnchorEmitter *particle_newAnchorEmitter(Actor *owner, const AnchorEmitterTemplate *tmpl, s16 duration, s8 updateFunc, s8 expireFunc,
                       s8 childKind, u8 messageFunc, s32 extraSize, u8 fxOwner, s32 fxList) {
    FollowerGroupDesc desc;
    u8 pad[4]; // matching: unused stack slot
    AnchorEmitterParams params;
    AnchorEmitterParams *dest;
    AnchorEmitter *emitter;
    s8 hasDuration;

    params.accum = 0.0f;
    params.tmpl = *tmpl;
    params.updateFunc = updateFunc;
    desc.owner = owner;
    params.expireFunc = expireFunc;
    params.messageFunc = messageFunc;
    desc.ownerId = owner->unique_id;
    if (duration == -1) {
        desc.duration = 300;
    } else {
        desc.duration = duration;
    }
    hasDuration = (duration == -1) ? 0 : 1;
    desc.hasDuration = hasDuration;
    desc.updateFn = 0;
    desc.trackFn = 0;
    desc.untrackFn = 0;
    desc.expireFn = 0;
    desc.kind = 1;

    emitter = effect_newFollowerGroup(&desc, extraSize + 0x60, fxOwner, fxList);
    if (emitter != NULL) {
        dest = &emitter->params;
        memcpy(dest, &params, sizeof(AnchorEmitterParams));
        if (childKind != -1) {
            dest->child = particle_spawnAnchorEmitterSprite(emitter, owner, childKind, duration, fxOwner, fxList);
        } else {
            dest->child = NULL;
        }
        effect_acquireAnchorSet(dest->tmpl.anchorSet);
        sound_addDynamicSound(0x4D, owner->xpos, owner->ypos, owner->zpos, 15000, 2000, 800,
                      particle_followEmitterSound, emitter, (s32)owner, 0, 0);
    }
    return emitter;
}

/* Accumulates the emission rate, spawns particles, then runs the emitter's update callback. */
s32 particle_updateAnchorEmitter(AnchorEmitter *emitter) {
    AnchorEmitterParams *params;

    params = &emitter->params;
    params->accum += (params->tmpl.rate + math_randomFloat() * params->tmpl.rateRange) * gFrameDelta;
    particle_emitAtRandomAnchor(emitter);
    if (params->updateFunc != -1) {
        return gAnchorEmitterUpdateFuncs[params->updateFunc](emitter);
    }
    return 1;
}

void particle_runAnchorEmitterExpire(AnchorEmitter *emitter) {
    AnchorEmitterParams *params = &emitter->params;

    if (emitter->params.expireFunc != -1) {
        gAnchorEmitterExpireFuncs[params->expireFunc](emitter);
    }
}

/* Emits a random-sized burst of particles. */
void particle_emitAnchorBurst(AnchorEmitter *emitter) {
    AnchorEmitterParams *params;

    params = &emitter->params;
    params->accum = math_randomFloat() * params->tmpl.burstRange + params->tmpl.burstBase;
    particle_emitAtRandomAnchor(emitter);
}

/* Spawns one particle at a random bone anchor of the owner per whole unit in `accum`. */
void particle_emitAtRandomAnchor(AnchorEmitter *emitter) {
    AnchorEmitterParams *params;
    Actor *owner;
    ParticleSize size;
    Vec3f pos;
    BoneAnchor *anchor;
    s32 set;
    s32 kind;
    EmitterParticleDesc desc;
    u32 frameRand;
    void *sprite;

    owner = emitter->group.actor;
    params = &emitter->params;
    if (params->accum > 1.0f) {
        if ((owner->ob_matrix_ptr != NULL) && ((owner->cullMask & 0xF) != 0xF)) {
            do {
                if (emitter->group.count < params->tmpl.maxCount) {
                    set = params->tmpl.anchorSet;
                    anchor = &gAnchorSets[set][math_random() % gAnchorCounts[set]];
                    size.width = math_randomFloat() * params->tmpl.sizeXRange + params->tmpl.sizeX;
                    size.height = math_randomFloat() * params->tmpl.sizeYRange + params->tmpl.sizeY;
                    math_transformOffset(&anchor->offset, &pos, owner->ob_matrix_ptr + anchor->bone);
                    frameRand = math_random();
                    if (math_random() & 1) {
                        kind = 0x13;
                    } else {
                        kind = 0x14;
                    }
                    sprite = particle_spawnJitterParticle(&pos, &size, -1,
                                           frameRand % (params->tmpl.alphaRange + 1) + params->tmpl.alpha,
                                           &gZeroVec3f, 0.0f, 1.0f, params->tmpl.easeFactor, 0, 0x23,
                                           0, kind, 1, 1, 0xFF, 0, emitter->group.header.owner, emitter->group.header.kind);
                    if (sprite != NULL) {
                        desc.owner = owner;
                        desc.ownerId = owner->unique_id;
                        desc.offset = anchor->offset;
                        desc.bone = anchor->bone;
                        desc.duration = math_random() % (params->tmpl.lifeRange + 1) + params->tmpl.lifeMin;
                        desc.flags = 1;
                        if (params->tmpl.flags & 1) {
                            desc.updateFn = 1;
                        } else {
                            desc.updateFn = 0;
                        }
                        desc.expireFn = 0;
                        desc.parent = emitter;
                        desc.sprite = sprite;
                        effect_spawnBoneFollower(&desc, &pos, 0, emitter->group.header.owner, emitter->group.header.kind);
                    }
                }
                params->accum -= 1.0f;
            } while (params->accum > 1.0f);
        } else {
            do {
                params->accum -= 1.0f;
            } while (params->accum > 1.0f);
        }
    }
}

s32 particle_followAnchorEased(EmitterParticle *particle, Vec3f *target) {
    JitterParticle *sprite;

    sprite = particle->sprite;
    sprite->head.def.pos = *target;
    particle_easeBendTarget(sprite);
    return 1;
}

s32 particle_followAnchor(EmitterParticle *particle, Vec3f *target) {
    JitterParticle *sprite;

    sprite = particle->sprite;
    sprite->head.def.pos = *target;
    particle_resetBendTarget(sprite);
    return 1;
}

/* Launches the particle's sprite from its position toward `dest`, or frees it if it has no emitter. */
void particle_launchAnchorParticle(EmitterParticle *particle, Vec3f *dest) {
    JitterParticle *sprite;
    ParticleJitter *motion;
    AnchorEmitterParams *params;
    AnchorEmitter *parent;

    sprite = particle->sprite;
    if (particle->parent == NULL) {
        effect_destroyObject(&sprite->head.header);
        return;
    }
    parent = particle->parent;
    sprite->jitter.velocity.x = (dest->x - particle->pos.x) * parent->params.tmpl.velocityScale;
    sprite->jitter.velocity.y = (dest->y - particle->pos.y) * parent->params.tmpl.velocityScale;
    sprite->jitter.velocity.z = (dest->z - particle->pos.z) * parent->params.tmpl.velocityScale;
    params = &parent->params;
    motion = &sprite->jitter;
    motion->gravity = math_randomFloat() * params->tmpl.gravityRange + params->tmpl.gravity;
    motion->drag = params->tmpl.drag;
    sprite->head.def.fadeStart = params->tmpl.fadeStart;
    sprite->head.def.fadeRate = params->tmpl.fadeRate;
    sprite->head.def.lifetime = math_random() % (params->tmpl.launchLifeRange + 1) + params->tmpl.launchLifeMin;
    if (params->tmpl.flags & 1) {
        sprite->head.updateFn1 = 0x21;
    } else {
        sprite->head.updateFn1 = 0x20;
    }
    sprite->head.updateFn2 = 0x24;
    sprite->head.def.flags |= 0x08000001;
}

/* Maps an object id to its emitter kind; only variant 0 exists. Returns 0 for unknown ids. */
s32 particle_getEmitterKindForObj(Actor *obj, u8 *kind, u8 variant) {
    switch (obj->character_type) {
    case 0x87:
    case 0xB0:
        *kind = 13;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xB1:
        *kind = 14;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xB4:
        *kind = 15;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x70:
    case 0xB2:
        *kind = 16;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x53:
        *kind = 17;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x96:
        *kind = 18;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
        *kind = 0;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x49:
        *kind = 1;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x58:
        *kind = 2;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x50:
        *kind = 3;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x5A:
    case 0x5F:
    case 0x74:
    case 0x75:
    case 0x7A:
        *kind = 4;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x80:
        *kind = 5;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        *kind = 6;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x16:
        *kind = 7;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x88:
    case 0x90:
        *kind = 8;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9C:
        *kind = 9;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9D:
        *kind = 10;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0x9F:
        *kind = 11;
        if (variant > 0) {
            return 0;
        }
        return 1;
    case 0xA0:
        *kind = 12;
        if (variant > 0) {
            return 0;
        }
        return 1;
    default:
        return 0;
    }
}

/* Returns the per-object-id data block (all empty in this build), setting *found. */
const u8 *particle_getObjEmitterData(Actor *obj, s32 *found) {
    const u8 *data;

    switch (obj->character_type) {
    case 0x87:
    case 0xB0:
        data = D_800A7AFC;
        *found = 1;
        break;
    case 0xB1:
        data = D_800A7B00;
        *found = 1;
        break;
    case 0xB4:
        data = D_800A7B04;
        *found = 1;
        break;
    case 0x70:
    case 0xB2:
        data = D_800A7B08;
        *found = 1;
        break;
    case 0x53:
        data = D_800A7B0C;
        *found = 1;
        break;
    case 0x96:
        data = D_800A7B10;
        *found = 1;
        break;
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
        data = D_800A7AC8;
        *found = 1;
        break;
    case 0x49:
        data = D_800A7ACC;
        *found = 1;
        break;
    case 0x58:
        data = D_800A7AD0;
        *found = 1;
        break;
    case 0x50:
        data = D_800A7AD4;
        *found = 1;
        break;
    case 0x75:
        data = D_800A7AD8;
        *found = 1;
        break;
    case 0x80:
        data = D_800A7ADC;
        *found = 1;
        break;
    case 0x11:
    case 0x14:
    case 0x3B:
    case 0x98:
    case 0x99:
        data = D_800A7AE0;
        *found = 1;
        break;
    case 0x16:
        data = D_800A7AE4;
        *found = 1;
        break;
    case 0x88:
    case 0x90:
        data = D_800A7AE8;
        *found = 1;
        break;
    case 0x9C:
        data = D_800A7AEC;
        *found = 1;
        break;
    case 0x9D:
        data = D_800A7AF0;
        *found = 1;
        break;
    case 0x9F:
        data = D_800A7AF4;
        *found = 1;
        break;
    case 0xA0:
        data = D_800A7AF8;
        *found = 1;
        break;
    default:
        data = NULL;
        *found = 0;
        break;
    }
    return data;
}

/* Creates the child sprite of an emitter, colored from the gLightJitterMids/6760/6820 tables. */
LightEffect *particle_spawnAnchorEmitterSprite(AnchorEmitter *emitter, Actor *owner, u8 kind, s16 duration, u8 fxOwner, s32 fxList) {
    ChildDesc desc;
    Vec3i pos;
    LightEffect *child;

    if (kind >= 24) {
        return NULL;
    }

    desc.jitter.mid = gLightJitterMids[kind];
    desc.jitter.low = gLightJitterLows[kind];
    desc.jitter.high = gLightJitterHighs[kind];
    desc.jitter.target = 1.0f;
    desc.jitter.interval = 5.0f;
    desc.jitter.timer = 0.0f;
    desc.jitter.level = 0.0f;
    desc.jitter.smoothing = 0.2489f;
    desc.emitter = emitter;

    desc.header.flags = ((duration == -1) ? 0 : 1) | 2;
    desc.header.update = 0x12;
    if (duration == -1) {
        desc.header.lifetime = 300;
    } else {
        desc.header.lifetime = duration;
    }
    desc.header.lightType = 0x25;

    desc.owner.actor = owner;
    desc.owner.uniqueId = owner->unique_id;

    pos.x = owner->xpos;
    pos.y = owner->ypos;
    pos.z = owner->zpos;

    child = light_createWithEffect(&desc.header, &pos, 0, 0xFF, 0xD1, 0, 0xFF, 0, 0x30, fxOwner, fxList);
    if (child != NULL) {
        memcpy(&child->data.actorJitter.ref, &desc.owner, sizeof(desc.owner));
        memcpy(&child->data.actorJitter.jitter, &desc.jitter, sizeof(desc.jitter));
        memcpy(&child->data.actorJitter.user, &desc.emitter, sizeof(desc.emitter));
    }
    return child;
}

/* Update for a flickering light tied to an actor (payload: actor reference, then jitter state). */
void light_updateActorFlicker(LightEffect *effect) {
    light_applyJitter(effect, &effect->data.actorJitter.jitter);
    light_trackActorRef(effect, &effect->data.actorJitter.ref);
}

/* An effect bound to an actor. */
struct BoundEffect {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ ActorRef ref;
};

/* Message handler: type 0 removes the effect when the message names its actor. */
void effect_handleAnchorLightEvent(BoundEffect *effect, ActorRef *msg, u8 type) {
    Actor *actor;
    Actor *msgActor;
    u8 actorId;
    u8 msgActorId;

    if (type == 0) {
        actor = effect->ref.actor;
        msgActor = msg->actor;
        actorId = effect->ref.uniqueId;
        msgActorId = msg->uniqueId;
        if (actor == msgActor || actorId == msgActorId) {
            effect_destroyObject(&effect->header);
        }
    }
}

/* Detaches the light from the AnchorEmitter that owns it. */
void effect_unlinkAnchorLight(LightEffect *obj) {
    AnchorEmitter *emitter = obj->data.actorJitter.user;

    emitter->params.child = NULL;
}

void effect_destroyAnchorLight(LightEffect *obj) {
    effect_unlinkAnchorLight(obj);
    light_defaultDestroy(obj);
}

void particle_freeAnchorEmitterSprite(LightEffect *obj) {
    effect_unlinkAnchorLight(obj);
    light_defaultFree(obj);
}

/* AnchorEmitter destroy: frees its child effect, unregisters it and releases its anchor set. */
void particle_destroyAnchorEmitter(AnchorEmitter *emitter) {
    AnchorEmitterParams *params = &emitter->params;

    if (params->child != NULL) {
        effect_destroyObject(params->child);
    }
    sound_stopByCallback(particle_followEmitterSound, emitter, (s32)emitter->group.actor);
    effect_releaseAnchorSet(params->tmpl.anchorSet);
}

/* Sends event 0x2A about `actor` to every object of type 0x3E. */
void particle_sendActorGoneToEmitters(Actor *actor) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    effect_sendEventToType(0x3E, (s32)&event, 0x2A);
}
