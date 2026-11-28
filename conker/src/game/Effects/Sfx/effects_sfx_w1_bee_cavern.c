#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/sprite.h"

/*
 * Rare's ../Effects/Sfx/effects_sfx_w1_bee_cavern.c. The ECTS build (August 2000) passes this path
 * to its debug tunables from effect_updateEmitterSpawner (12 calls, lines 87 to 171) and from
 * effect_updateFallingScreenSprite. The US build has no tunables: their values are literals.
 * The file is these four functions: the unit starts after alignment padding, and the next unit's
 * const tables follow this unit's float literals in .rodata.
 * The ECTS tunables have Rare's names for the constants: MINIMUM_FRAMES_TIL_SCREENSPLAT,
 * FRAMES_TIL_SCREENSPLAT_RANGE, POSITION_S_RANGE, POSITION_T, MINIMUM_WIDTH, WIDTH_RANGE, MINIMUM_HEIGHT,
 * HEIGHT_RANGE, MINIMUM_ALPHA, ALPHA_RANGE, MINIMUM_SPEED, SPEED_RANGE (effect_updateEmitterSpawner) and
 * BOTTOM_OF_SCREEN (effect_updateFallingScreenSprite).
 */

/* Creates the spawner object for `owner`, with its spawn timer at 0. */
void effect_newEmitterSpawner(Camera *owner, u8 fxOwner, s32 fxList) {
    BeeCavernSpawner spawn;
    BeeCavernSpawnerEffect *obj;

    spawn.owner = owner;
    spawn.timer = 0;

    obj = effect_newTimedEffect(300, -1, 0x4F, -1, 0, 60, 8, fxOwner, fxList);
    if (obj != NULL) {
        memcpy(&obj->spawn, &spawn, sizeof(spawn));
    }
}

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void effect_updateEmitterSpawner(BeeCavernSpawnerEffect *obj) {
    BeeCavernSpawner *spawn;
    ScreenSpriteInfo emitter;
    void *effect;
    BeeCavernSpritePayload payload;

    obj->spawn.timer -= gFrameTicks;
    spawn = &obj->spawn;
    if (obj->spawn.timer < 0) {
        emitter.x = (math_randomFloat() * 270.0f) + -135.0f;
        emitter.y = -120.0f;
        emitter.halfWidth = (math_randomFloat() * 10.0f) + 3.0f;
        emitter.halfHeight = (math_randomFloat() * 17.0f) + 9.0f;
        emitter.texture = 0xAB;
        emitter.timer = 1000;
        emitter.flags = 0x31;
        emitter.fadeStart = 1;
        emitter.fadeRate = 0xFF;
        emitter.kind = 7;
        emitter.color1.red = 0xFF;
        emitter.color1.green = 0xFF;
        emitter.color1.blue = 0xFF;
        emitter.color1.alpha = (math_random() % 156U) + 100;
        emitter.color2.red = 0xFF;
        emitter.color2.green = 0xFF;
        emitter.color2.blue = 0xFF;
        emitter.color2.alpha = 0xFF;
        emitter.colorLevel = 0xFF;
        emitter.render.otherModeH = 0;
        emitter.render.setGeometryMode = 0x200004;
        emitter.render.clearGeometryMode = 0x1F0601;
        emitter.render.renderMode = 3;
        emitter.render.combiner = 0x22;
        emitter.render.colorDither = 0x80;
        emitter.render.alphaDither = 0x20;
        emitter.render.primMode = 0;
        emitter.render.envMode = 7;
        emitter.viewport = spawn->owner->CNumber;
        emitter.scaleX = 1.0f;
        emitter.scaleY = 1.0f;
        emitter.offsetS = 0.0f;
        emitter.offsetT = 0.0f;
        payload.owner = spawn->owner;
        payload.kind = 0;
        payload.flags = 0;
        payload.ySpeed = (math_randomFloat() * 4.06400013f) + 2.513000011f;
        effect = sprite_newScreenSprite(&emitter, 10, 0, 0, sizeof(emitter), obj->base.header.owner, 0);
        if (effect != NULL) {
            memcpy(&((BeeCavernSprite *)effect)->payload, &payload, sizeof(payload));
        }
        spawn->timer = (math_random() % 151U) + 25;
    }
}

/* Spawner message handler: destroys the spawner when its owner (event 0x21), or the
 * object whose +0x318 points at its owner (event 0), sends the message. */
void effect_emitterSpawnerMessage(BeeCavernSpawnerEffect *obj, void **msg, u8 event) {
    if (event == 0x21) {
        if (obj->spawn.owner == msg[0]) {
            effect_destroyObject(&obj->base.header);
        }
    } else if (event == 0) {
        Actor *sender = msg[0];

        if (sender->CameraPtr == obj->spawn.owner) {
            effect_destroyObject(&obj->base.header);
        }
    }
}

/* Advances y by ySpeed per frame (down the screen); returns 0 once it passes 120 (Rare's BOTTOM_OF_SCREEN). */
s32 effect_updateFallingScreenSprite(BeeCavernSprite *sprite) {
    sprite->sprite.info.y += sprite->payload.ySpeed * gFrameDelta;
    if (sprite->sprite.info.y > 120.0f) {
        return 0;
    }
    return 1;
}
