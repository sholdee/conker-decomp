#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/sprite.h"

/*
 * Rare's ../Effects/Sfx/effects_sfx_w1_bee_cavern.c. The ECTS build (August 2000) passes this path
 * to its debug tunables from effect_updateEmitterSpawner (12 calls, lines 87 to 171) and from
 * effect_updateRisingScreenSprite. The US build has no tunables: their values are literals.
 * The file is these four functions: the unit starts after alignment padding, and the next unit's
 * const tables follow this unit's float literals in .rodata.
 * The ECTS tunables have Rare's names for the constants: MINIMUM_FRAMES_TIL_SCREENSPLAT,
 * FRAMES_TIL_SCREENSPLAT_RANGE, POSITION_S_RANGE, POSITION_T, MINIMUM_WIDTH, WIDTH_RANGE, MINIMUM_HEIGHT,
 * HEIGHT_RANGE, MINIMUM_ALPHA, ALPHA_RANGE, MINIMUM_SPEED, SPEED_RANGE (effect_updateEmitterSpawner) and
 * BOTTOM_OF_SCREEN (effect_updateRisingScreenSprite).
 */

/* Creates the spawner object for `owner`, with its spawn timer at 0. */
void effect_newEmitterSpawner(Camera *owner, u8 fxOwner, s32 fxList) {
    EB340Spawn spawn;
    EB340Obj *obj;

    spawn.owner = owner;
    spawn.timer = 0;

    obj = effect_newTimedEffect(300, -1, 0x4F, -1, 0, 60, 8, fxOwner, fxList);
    if (obj != NULL) {
        memcpy(&obj->spawn, &spawn, sizeof(spawn));
    }
}

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void effect_updateEmitterSpawner(EB340Obj *obj) {
    EB340Spawn *spawn;
    ScreenSpriteInfo emitter;
    void *effect;
    EB340Payload payload;

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
        emitter.unk23 = 0xFF;
        emitter.otherModeH = 0;
        emitter.unk28 = 0x200004;
        emitter.unk2C = 0x1F0601;
        emitter.combineMode = 3;
        emitter.unk34 = 0x22;
        emitter.unk38 = 0x80;
        emitter.unk3C = 0x20;
        emitter.unk40 = 0;
        emitter.unk41 = 7;
        emitter.viewport = spawn->owner->CNumber;
        emitter.scaleX = 1.0f;
        emitter.scaleY = 1.0f;
        emitter.offsetS = 0.0f;
        emitter.offsetT = 0.0f;
        payload.owner = spawn->owner;
        payload.unkD = 0;
        payload.unkC = 0;
        payload.unk10 = (math_randomFloat() * 4.06400013f) + 2.513000011f;
        effect = sprite_newScreenSprite(&emitter, 10, 0, 0, sizeof(emitter), obj->base.header.owner, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x70, &payload, sizeof(payload));
        }
        spawn->timer = (math_random() % 151U) + 25;
    }
}

/* Spawner message handler: destroys the spawner when its owner (event 0x21), or the
 * object whose +0x318 points at its owner (event 0), sends the message. */
void effect_emitterSpawnerMessage(EB340Obj *obj, void **msg, u8 event) {
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

/* Advances y by ySpeed per frame; returns 0 once it passes 120. */
s32 effect_updateRisingScreenSprite(EB340Rising *sprite) {
    sprite->y += sprite->ySpeed * gFrameDelta;
    if (sprite->y > 120.0f) {
        return 0;
    }
    return 1;
}
