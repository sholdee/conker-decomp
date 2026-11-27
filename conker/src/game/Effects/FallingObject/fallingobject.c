#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/*
 * Spawns a QuadFx (animation 0x45, callback 15) at the emitter's position, resting on its floor height (or 200 units
 * below it, clearing pos.markOnLanding, when it has none), then rearms the emitter's cooldown to 100..227.
 */
// matching: volatile keeps `emitter` in its stack slot (reloaded after each call) instead of $s0
void effect_dropFallingObject(ObjectEmitter *volatile emitter) {
    u8 pad[8]; // matching: the frame needs 8 more bytes
    QuadFxData spawn;
    EmitterPos *pos;
    s16 floorY;

    bzero(&spawn, sizeof(spawn));
    pos = &emitter->pos;
    floorY = pos->floorY;
    if (floorY != -10000.0f) {
        spawn.floorY = floorY;
    } else {
        spawn.floorY = pos->y - 200.0f;
        pos->markOnLanding = 0;
    }

    spawn.x = pos->x;
    spawn.y = pos->y;
    spawn.z = pos->z;
    spawn.halfWidth = 0;
    spawn.halfHeight = 0;
    spawn.callback = 15;
    spawn.animIndex = 0x45;
    spawn.unk86 = 10;
    spawn.unkA3 = pos->markOnLanding;
    spawn.flags = 0x4880;
    spawn.alpha = 0x80;

    effect_initQuadFxTemplate(&spawn, 0xFF, 0xFF, 0xFF, 0x80);
    // matching: one load of the volatile `emitter` serves both arguments
    pos = (EmitterPos *)emitter;
    effect_spawnQuadFx(&spawn, ((ObjectEmitter *)pos)->base.header.owner, ((ObjectEmitter *)pos)->base.header.kind);
    emitter->base.header.state = (math_random() & 0x7F) + 100;
}

struct FallingObject {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x84];
    /* 0x94 */ s16 fallSpeed;
    /* 0x96 */ s16 maxHalfHeight;
    /* 0x98 */ s16 frame;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 x;
    /* 0x9E */ s16 y;
    /* 0xA0 */ s16 z;
    /* 0xA2 */ s16 halfWidth;
    /* 0xA4 */ s16 halfHeight;
    /* 0xA6 */ s16 floorY;
    /* 0xA8 */ u8 padA8[2];
    /* 0xAA */ s16 timer;
    /* 0xAC */ u8 padAC[7];
    /* 0xB3 */ s8 markOnLanding;
};

/*
 * While halfHeight is below maxHalfHeight, grows halfHeight/halfWidth with the timer. After that the object falls with
 * a speed that ramps up to 20; once it drops below floorY it sets frame to -1, first spawning a
 * particle_spawnKindMark effect at the landing point when markOnLanding is 1.
 */
void effect_updateFallingObject(FallingObject *obj) {
    f32 pos[4]; // matching: a 4th (unused) slot keeps pos at its original stack offset

    if (obj->halfHeight < obj->maxHalfHeight) {
        obj->halfHeight = ((obj->timer >> 2) * 24) / 3;
        obj->halfWidth = ((obj->timer >> 2) * 6) / 3;
        obj->timer += gFrameTicks;
        return;
    }

    obj->fallSpeed++;
    if (obj->fallSpeed > 20) {
        obj->fallSpeed = 20;
    }

    obj->y -= (obj->fallSpeed * gFrameTicks) >> 1;
    if (obj->y < obj->floorY) {
        if (obj->markOnLanding == 1) {
            pos[0] = obj->x;
            pos[1] = obj->floorY;
            pos[2] = obj->z;
            particle_spawnKindMark(5, 30.0f, 0xFF, 0, pos, obj->header.owner, obj->header.kind);
        }
        obj->frame = -1;
    }
}
