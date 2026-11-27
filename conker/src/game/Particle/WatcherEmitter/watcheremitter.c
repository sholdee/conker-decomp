#include <ultra64.h>
#include "effects.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/* Keeps the emitter's particle alive, spawning it if it doesn't exist yet. */
void particle_keepFlickerSprite(GameF3270Emitter *emitter) {
    if (emitter->children.particle != NULL) {
        SpriteFlicker *flicker = &emitter->children.particle->flicker;

        flicker->keepAlive = TRUE;
    } else {
        emitter->children.particle = particle_spawnFlickerSprite(emitter);
    }
}

/* Particle update: eases unk24 toward a target re-rolled at random intervals. Returns whether it stays alive. */
s8 particle_updateFlickerSprite(FlickerSprite *particle) {
    s8 alive;
    SpriteFlicker *flicker;

    alive = TRUE;
    flicker = &particle->flicker;
    if (!flicker->keepAlive) {
        alive = FALSE;
    }
    flicker->keepAlive = FALSE;
    flicker->timer -= gFrameTicks;
    if (flicker->timer < 0) {
        flicker->timer = math_random() % (flicker->delayRange + 1) + flicker->minDelay;
        flicker->target = math_randomFloat() * flicker->targetRange + flicker->minTarget;
    }
    particle->quad.def.frameSpeed += (s32)((flicker->target - particle->quad.def.frameSpeed) * flicker->easing);
    return alive;
}

/* Particle destructor: detaches it from its emitter. */
s32 particle_detachFlickerSprite(FlickerSprite *particle) {
    GameF3270Children *children = &((GameF3270Emitter *)particle->flicker.owner)->children;

    children->particle = NULL;
    particle->quad.def.flags |= 2;
    particle->flicker.owner = NULL;
    particle_setQuadSpriteFuncs(&particle->quad, 0, 0);
    return 0;
}

/* Keeps the emitter's linked object alive, spawning it if it doesn't exist yet. */
void particle_keepWatcherEmitter(GameF3270Emitter *emitter) {
    Actor *actor = emitter->watcher.character;

    if (emitter->children.linked != NULL) {
        WatcherEmitterLink *link = &emitter->children.linked->link;

        link->keepAlive = TRUE;
    } else {
        emitter->children.linked = particle_spawnWatcherEmitter(actor, emitter);
    }
}


/* Spawns the emitter's linked object, recording the surface height above the actor. */
WatcherEmitter *particle_spawnWatcherEmitter(Actor *actor, GameF3270Emitter *emitter) {
    BloodDripDesc desc;
    WatcherEmitterLink link;
    WatcherEmitter *obj;

    link.owner = &emitter->watcher;
    link.keepAlive = TRUE;
    {
        Vec3f from;

        from.x = actor->xpos;
        from.y = actor->ypos + 5000.0f;
        from.z = actor->zpos;
        if (collision_probeFloorSurfBCached(&from, 0, actor->ypos - 100.0f, &emitter->watcher.hit) != 0) {
            link.ceiling = emitter->watcher.hit.height;
        } else {
            link.ceiling = actor->ypos + 5000.0f;
        }
    }
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 0;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 25.0f;
    desc.rate = 0.132485f;
    desc.lifetime = 300;
    desc.flags = 10;
    desc.emitter = 4;
    desc.callback = 1;
    desc.handlerSet = 2;
    obj = blood_spawnDrip(&desc, 12, 0xFF, 0);
    if (obj != NULL) {
        memcpy(&obj->link, &link, sizeof(link));
    }
    return obj;
}

/* Spawns one randomised particle at (x, y, z), moving against (vx, vz) at a fifth of its speed. */
void particle_spawnWatcherParticle(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, WatcherEmitter *source) {
    ParticleDesc burst;
    RiseParticle *spawned;
    RiseMotion motion;

    burst.texture = 47;
    burst.renderMode = 0xC01;
    burst.setGeometryMode = 0x200005;
    burst.clearGeometryMode = 0;
    burst.b2 = 0;
    burst.g2 = 0;
    burst.r2 = 0;
    burst.a = 0;
    burst.b = 0;
    burst.g = 0;
    burst.r = 0;
    burst.frameRate = 0;
    burst.frame = 0;
    burst.brightness = 0xFF;
    burst.scaleStart = 0;
    burst.scaleRate = 0.0f;
    burst.flags = 0x3207;
    burst.primColorMode = 5;
    burst.envColorMode = 5;
    burst.updateFunc = 4;
    burst.deathFunc = -1;
    burst.pos.x = x;
    burst.pos.y = y;
    burst.offset.x = 0.0f;
    burst.offset.y = 0.0f;
    burst.offset.z = 0.0f;
    burst.fadeStart = 30;
    burst.fadeRate = 8;
    burst.pos.z = z;
    motion.ceiling = source->link.ceiling;
    motion.maxRiseSpeed = 4.0f;
    motion.swayPerSpeed = 0.25f;
    motion.widthPhase = 0;
    motion.heightPhase = 0;
    motion.swayPhaseX = 0;
    motion.swayPhaseZ = 0;
    burst.vel.x = -vx * 0.2f;
    burst.vel.y = 0.0f;
    burst.vel.z = -vz * 0.2f;
    if (math_random() & 1) {
        burst.flags |= 0x40;
    }
    if (math_random() & 1) {
        burst.flags |= 0x80;
    }
    burst.timer = math_random() % 51U + 106;
    burst.alpha = math_random() % 101U + 100;
    motion.baseSize = burst.width = burst.height = math_randomFloat() * 62.0f + 41.0f;
    motion.widthPhaseStep = math_random() % 5U + 4;
    motion.heightPhaseStep = math_random() % 5U + 4;
    motion.widthWobble = (math_randomFloat() * 0.25f + 0.15f) * motion.baseSize;
    motion.heightWobble = (math_randomFloat() * 0.25f + 0.15f) * motion.baseSize;
    motion.swayPhaseStepX = math_random() % 5U + 4;
    motion.swayPhaseStepZ = math_random() % 5U + 4;
    motion.swayX = (math_randomFloat() * 0.1f + 0.08f) * motion.baseSize;
    motion.swayZ = (math_randomFloat() * 0.1f + 0.08f) * motion.baseSize;
    burst.gravity = (math_randomFloat() * 60.0f + 21.0f) * 0.001f;
    spawned = particle_newSpriteParticle(&burst, 2, 40);
    if (spawned != NULL) {
        memcpy(&spawned->motion, &motion, sizeof(motion));
    }
}

/* Linked-object update: returns whether its owner kept it alive this frame. */
s32 particle_updateWatcherLink(WatcherEmitter *obj) {
    s32 alive = TRUE;

    if (!obj->link.keepAlive) {
        alive = FALSE;
    }
    obj->link.keepAlive = FALSE;
    return alive;
}

/* Linked-object destructor: detaches it from its emitter. */
void particle_destroyWatcherLink(WatcherEmitter *obj) {
    GameF3270Children *children = &((GameF3270Emitter *)obj->link.owner)->children;

    children->linked = NULL;
    blood_destroyDripDefault(&obj->header);
}

/* Linked-object destructor: detaches it from its emitter. */
void particle_freeWatcherLink(WatcherEmitter *obj) {
    GameF3270Children *children = &((GameF3270Emitter *)obj->link.owner)->children;

    children->linked = NULL;
    blood_freeDripDefault(&obj->header);
}

static Vec3f gFlickerSpriteOrigin = { 0.0f, 0.0f, 0.0f }; /* the point below the actor's root */

/* Spawns the emitter's particle on the surface below the actor, if there is one within 200 units. */
FlickerSprite *particle_spawnFlickerSprite(GameF3270Emitter *emitter) {
    FlickerSprite *particle;
    Vec3f pos;
    Actor *actor;

    particle = NULL;
    actor = emitter->watcher.character;
    if (actor->ob_matrix_ptr) {
        Vec3f origin;

        origin = gFlickerSpriteOrigin;
        math_transformOffset(&origin, &pos, actor->ob_matrix_ptr);
    } else {
        pos.x = actor->xpos;
        pos.y = actor->ypos;
        pos.z = actor->zpos;
    }
    {
        Vec3f probe;

        probe.x = pos.x;
        probe.y = pos.y + 200.0f;
        probe.z = pos.z;
        if (collision_probeFloorSurfBCached(&probe, 0, pos.y - 200.0f, &emitter->watcher.hit) != 0) {
            SpriteDef desc;
            SpriteFlicker flicker;
            f32 size;
            u8 colorIndex;
            s32 rand2;
            s32 rand1;
            s32 rand0;

            colorIndex = particle_getContextEffectKind();
            probe.y = emitter->watcher.hit.height;
            size = math_randomFloat() * 50.0f + 100.0f;
            flicker.owner = &emitter->watcher;
            flicker.keepAlive = TRUE;
            flicker.minTarget = 26214.4f;
            flicker.targetRange = 32768.0f;
            flicker.target = 0.0f;
            flicker.timer = 0;
            flicker.minDelay = 10;
            flicker.delayRange = 10;
            flicker.easing = 0.231f;
            desc.frameSpeed = flicker.targetRange * 0.5f + flicker.minTarget;
            desc.texture = 56;
            desc.flags = 0x20300;
            desc.lifetime = 300;
            desc.alpha = 150;
            desc.frame = 0;
            desc.colorLevel = 0xFF;
            desc.rgba[0] = gKindColors[colorIndex].r;
            desc.rgba[1] = gKindColors[colorIndex].g;
            desc.rgba[2] = gKindColors[colorIndex].b;
            desc.rgba[3] = 0xFF;
            desc.renderModes = 0x440001;
            desc.primMode = 0;
            desc.envMode = 6;
            rand0 = math_random();
            rand1 = math_random();
            rand2 = math_random();
            // matching: the explicit & 0xFF on rand0 (a (u8) cast emits no andi)
            particle = (FlickerSprite *)particle_spawnSurfaceQuadSpriteNoModel(
                &desc, 8, 2, &emitter->watcher.hit.tri, probe.x, probe.y, probe.z, size, size, rand0 & 0xFF,
                (rand2 & 1) * 2 + (rand1 & 1), 32, emitter->watcher.header.owner, emitter->watcher.header.kind);
            if (particle != NULL) {
                memcpy(&particle->flicker, &flicker, sizeof(flicker));
            }
        }
    }
    return particle;
}
