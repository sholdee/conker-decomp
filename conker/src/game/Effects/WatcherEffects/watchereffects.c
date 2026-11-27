#include <ultra64.h>
#include "effects.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/*
 * Two effects that character-watcher actions (gWatcherActions) keep alive around a character:
 * a flickering sprite on the floor below it, and an emitter of particles that rise from it
 * to the surface above. Each frame the action sets the effect's keepAlive flag; the effect's
 * own update clears it and dies once a frame passes without it.
 */


/* The effects a watcher keeps alive, stored in the watcher's extra data. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ FlickerSprite *volatile sprite; // matching: volatile keeps IDO from reusing the NULL test's load
    /* 0x18 */ WatcherEmitter *volatile emitter;
} WatcherEffects;

/* A character watcher with WatcherEffects as extra data. */
struct Watcher {
    /* 0x00 */ CharacterWatcher watcher;
    /* 0x58 */ WatcherEffects effects;
};

static Vec3f gWatcherRootOffset = { 0.0f, 0.0f, 0.0f }; /* the point below the character's root */
/*
 * Heights above the character: where the ceiling ray-cast starts, and the ceiling used when it hits
 * nothing. They are loaded from .rodata (a 5000.0f literal would be built with lui); IDO 5.3 puts a
 * const aggregate there (a const scalar goes to .data).
 */
const struct CeilingProbe {
    f32 rayStart;
    f32 defaultCeiling;
} gWatcherCeilingProbe = { 5000.0f, 5000.0f };

/* Watcher action: keeps the floor sprite alive, creating it on first use. */
void effect_keepWatcherFloorSprite(Watcher *watcher) {
    if (watcher->effects.sprite != NULL) {
        SpriteFlicker *flicker = &watcher->effects.sprite->flicker;

        flicker->keepAlive = 1;
    } else {
        watcher->effects.sprite = effect_createFloorFlickerSprite(watcher);
    }
}

/*
 * Sprite update: eases the animation speed toward the flicker target, re-picking the target
 * at random intervals. Returns whether the watcher refreshed keepAlive since the last call.
 */
s32 effect_updateFlickerSprite(FlickerSprite *sprite) {
    s8 alive;
    SpriteFlicker *flicker;

    alive = 1;
    flicker = &sprite->flicker;
    if (!flicker->keepAlive) {
        alive = 0;
    }
    flicker->keepAlive = 0;
    flicker->timer -= gFrameTicks;
    if (flicker->timer < 0) {
        flicker->timer = (math_random() % (flicker->delayRange + 1)) + flicker->minDelay;
        flicker->target = math_randomFloat() * flicker->targetRange + flicker->minTarget;
    }
    sprite->quad.def.frameSpeed += (s32)((flicker->target - sprite->quad.def.frameSpeed) * flicker->easing);
    return alive;
}

/* Sprite expiry: detaches the sprite from its watcher and clears its handlers. */
s32 effect_expireFlickerSprite(FlickerSprite *sprite) {
    WatcherEffects *effects = &((Watcher *)sprite->flicker.owner)->effects;

    effects->sprite = NULL;
    sprite->quad.def.flags |= 2;
    sprite->flicker.owner = NULL;
    particle_setQuadSpriteFuncs(&sprite->quad, 0, 0);
    return 0;
}

/* Watcher action: keeps the rising-particle emitter alive, creating it on first use. */
void effect_keepWatcherEmitter(Watcher *watcher) {
    Actor *character = watcher->watcher.character;

    if (watcher->effects.emitter != NULL) {
        WatcherEmitterLink *link = &watcher->effects.emitter->link;

        link->keepAlive = 1;
    } else {
        watcher->effects.emitter = effect_attachRisingEmitter(character, watcher);
    }
}

/*
 * Attaches an emitter to the character; its particles rise to the surface found above the
 * character (or 5000 units up when there is none).
 */
WatcherEmitter *effect_attachRisingEmitter(Actor *character, Watcher *watcher) {
    BloodDripDesc desc;
    WatcherEmitterLink link;
    WatcherEmitter *emitter;

    {
        Vec3f pos;

        link.owner = &watcher->watcher;
        link.keepAlive = 1;
        pos.x = character->xpos;
        pos.y = character->ypos + gWatcherCeilingProbe.rayStart;
        pos.z = character->zpos;
        if (collision_probeFloorSurfBCached(&pos, 0, character->ypos - 100.0f, &watcher->watcher.hit) != 0) {
            link.ceiling = watcher->watcher.hit.height;
        } else {
            link.ceiling = character->ypos + gWatcherCeilingProbe.defaultCeiling;
        }
    }
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = character->unique_id;
    desc.owner = character;
    desc.bone = 0;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 80.0f;
    desc.rate = 0.6049699783f;
    desc.lifetime = 300;
    desc.flags = 10;        /* on a bone (2), no previous position yet (8) */
    desc.emitter = 5;
    desc.callback = 2;
    desc.handlerSet = 3;
    emitter = blood_spawnDrip(&desc, sizeof(WatcherEmitterLink), 0xFF, 0);
    if (emitter != NULL) {
        memcpy(&emitter->link, &link, sizeof(WatcherEmitterLink));
    }
    return emitter;
}

/* WatcherEmitter func 5: spawns one rising particle at (x, y, z), drifting against the emitter's motion. */
void effect_spawnRisingParticle(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, WatcherEmitter *emitter) {
    ParticleDesc desc;
    RiseParticle *particle;
    RiseMotion motion;

    desc.texture = 0x2F;
    desc.renderMode = 0xC01;
    desc.setGeometryMode = 0x200005;
    desc.clearGeometryMode = 0;
    desc.r = desc.g = desc.b = desc.a = desc.r2 = desc.g2 = desc.b2 = 0;
    desc.frame = desc.frameRate = 0;
    desc.brightness = 0xFF;
    desc.scaleStart = 0;
    desc.scaleRate = 0.0f;
    desc.flags = 0x3207;
    desc.primColorMode = 5;
    desc.envColorMode = 5;
    desc.updateFunc = 4;
    desc.deathFunc = -1;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.fadeStart = 30;
    desc.fadeRate = 8;
    motion.ceiling = emitter->link.ceiling;
    motion.maxRiseSpeed = 4.0f;
    motion.swayPerSpeed = 0.25f;
    motion.widthPhase = 0;
    motion.heightPhase = 0;
    motion.swayPhaseX = 0;
    motion.swayPhaseZ = 0;
    desc.vel.x = -dx * 0.2f;
    desc.vel.y = 0.0f;
    desc.vel.z = -dz * 0.2f;
    if (math_random() & 1) {
        desc.flags |= 0x40;
    }
    if (math_random() & 1) {
        desc.flags |= 0x80;
    }
    desc.timer = math_random() % 51U + 106;
    desc.alpha = math_random() % 101U + 100;
    motion.baseSize = desc.width = desc.height = math_randomFloat() * 62.0f + 41.0f;
    motion.widthPhaseStep = math_random() % 5U + 4;
    motion.heightPhaseStep = math_random() % 5U + 4;
    motion.widthWobble = (math_randomFloat() * 0.25f + 0.15f) * motion.baseSize;
    motion.heightWobble = (math_randomFloat() * 0.25f + 0.15f) * motion.baseSize;
    motion.swayPhaseStepX = math_random() % 5U + 4;
    motion.swayPhaseStepZ = math_random() % 5U + 4;
    motion.swayX = (math_randomFloat() * 0.1f + 0.08f) * motion.baseSize;
    motion.swayZ = (math_randomFloat() * 0.1f + 0.08f) * motion.baseSize;
    desc.gravity = (math_randomFloat() * 60.0f + 21.0f) * 0.001f;
    particle = particle_newSpriteParticle(&desc, 2, sizeof(RiseMotion));
    if (particle != NULL) {
        memcpy(&particle->motion, &motion, sizeof(RiseMotion));
    }
}

/* WatcherEmitter callback 2: returns whether the watcher refreshed keepAlive since the last call. */
s32 effect_isWatcherEmitterAlive(WatcherEmitter *emitter) {
    s32 alive = 1;

    if (!emitter->link.keepAlive) {
        alive = 0;
    }
    emitter->link.keepAlive = 0;
    return alive;
}

/* WatcherEmitter teardown (gBloodDripDestroyHandlers[3]): detaches the emitter from its watcher. */
void effect_destroyWatcherEmitter(WatcherEmitter *emitter) {
    WatcherEffects *effects = &((Watcher *)emitter->link.owner)->effects;

    effects->emitter = NULL;
    blood_destroyDripDefault(&emitter->header);
}

/* WatcherEmitter teardown (gBloodDripFreeHandlers[3]): detaches the emitter from its watcher. */
void effect_freeWatcherEmitter(WatcherEmitter *emitter) {
    WatcherEffects *effects = &((Watcher *)emitter->link.owner)->effects;

    effects->emitter = NULL;
    blood_freeDripDefault(&emitter->header);
}

/*
 * Creates the flickering sprite on the floor below the character (at its first bone when it
 * has a skeleton), colored by particle_getContextEffectKind(). Returns NULL when there is no floor within 200 units.
 */
FlickerSprite *effect_createFloorFlickerSprite(Watcher *watcher) {
    FlickerSprite *sprite;
    Vec3f pos;
    Actor *character;

    sprite = NULL;
    character = watcher->watcher.character;
    if (character->ob_matrix_ptr) {
        Vec3f origin = gWatcherRootOffset;

        math_transformOffset(&origin, &pos, character->ob_matrix_ptr);
    } else {
        pos.x = character->xpos;
        pos.y = character->ypos;
        pos.z = character->zpos;
    }
    {
        Vec3f probe;

        probe.x = pos.x;
        probe.y = pos.y + 200.0f;
        probe.z = pos.z;
        if (collision_probeFloorSurfBCached(&probe, 0, pos.y - 200.0f, &watcher->watcher.hit) != 0) {
            SpriteDef def;
            SpriteFlicker flicker;
            f32 size;
            u8 color;
            s32 rand2;
            s32 rand1;
            s32 rand0;

            color = particle_getContextEffectKind();
            probe.y = watcher->watcher.hit.height;
            size = math_randomFloat() * 100.0f + 200.0f;
            flicker.owner = &watcher->watcher;
            flicker.keepAlive = 1;
            flicker.minTarget = 26214.4f;
            flicker.targetRange = 32768.0f;
            flicker.target = 0.0f;
            flicker.timer = 0;
            flicker.minDelay = 10;
            flicker.delayRange = 10;
            flicker.easing = 0.231f;
            def.frameSpeed = flicker.targetRange * 0.5f + flicker.minTarget;
            def.texture = 0x38;
            def.flags = 0x20300;
            def.lifetime = 300;
            def.alpha = 150;
            def.frame = 0;
            def.colorLevel = 0xFF;
            def.rgba[0] = gKindColors[color].r;
            def.rgba[1] = gKindColors[color].g;
            def.rgba[2] = gKindColors[color].b;
            def.rgba[3] = 0xFF;
            def.renderModes = 0x440001;
            def.primMode = 0;
            def.envMode = 6;
            rand0 = math_random();
            rand1 = math_random();
            rand2 = math_random();
            // matching: IDO needs the redundant `& 0xFF` on the u8 argument (the bytes change without it)
            sprite = (FlickerSprite *)particle_spawnSurfaceQuadSpriteNoModel(&def, 9, 3, &watcher->watcher.hit.tri,
                                                    probe.x, probe.y, probe.z, size, size, rand0 & 0xFF,
                                                    (rand2 & 1) * 2 + (rand1 & 1), 0x20, watcher->watcher.header.owner,
                                                    watcher->watcher.header.kind);
            if (sprite != NULL) {
                memcpy(&sprite->flicker, &flicker, sizeof(SpriteFlicker));
            }
        }
    }
    return sprite;
}
