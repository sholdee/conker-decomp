#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Creates the spawner object for `owner`, with its spawn timer at 0. */
void func_150BDE90(Camera *owner, u8 arg1, s32 arg2) {
    EB340Spawn spawn;
    EB340Obj *obj;

    spawn.owner = owner;
    spawn.timer = 0;

    obj = (EB340Obj *)func_15149130(300, -1, 0x4F, -1, 0, 60, 8, arg1, arg2);
    if (obj != NULL) {
        memcpy(&obj->spawn, &spawn, sizeof(spawn));
    }
}

/* Spawner update: when the timer runs out, spawns one emitter near the owner and
 * restarts the timer at a random 25..175. */
void func_150BDF0C(EB340Obj *obj) {
    EB340Spawn *spawn;
    ScreenSpriteInfo emitter;
    void *effect;
    EB340Payload payload;

    obj->spawn.timer -= D_800BE9E4;
    spawn = &obj->spawn;
    if (obj->spawn.timer < 0) {
        emitter.x = (func_150ADA68() * 270.0f) + -135.0f;
        emitter.y = -120.0f;
        emitter.halfWidth = (func_150ADA68() * 10.0f) + 3.0f;
        emitter.halfHeight = (func_150ADA68() * 17.0f) + 9.0f;
        emitter.texture = 0xAB;
        emitter.timer = 1000;
        emitter.flags = 0x31;
        emitter.fadeStart = 1;
        emitter.fadeRate = 0xFF;
        emitter.kind = 7;
        emitter.color1.red = 0xFF;
        emitter.color1.green = 0xFF;
        emitter.color1.blue = 0xFF;
        emitter.color1.alpha = (func_150ADA20() % 156U) + 100;
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
        payload.unk10 = (func_150ADA68() * 4.06400013f) + 2.513000011f;
        effect = func_1515548C(&emitter, 10, 0, 0, sizeof(emitter), obj->base.header.owner, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x70, &payload, sizeof(payload));
        }
        spawn->timer = (func_150ADA20() % 151U) + 25;
    }
}
