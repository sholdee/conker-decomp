#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/*
 * Spawns a QuadFx (animation 0x45, callback 15) at the emitter's position, resting on its floor height (or 200 units
 * below it, clearing pos.unk8, when it has none), then rearms the emitter's cooldown to 100..227.
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
        pos->unk8 = 0;
    }

    spawn.x = pos->x;
    spawn.y = pos->y;
    spawn.z = pos->z;
    spawn.unk92 = 0;
    spawn.unk94 = 0;
    spawn.callback = 15;
    spawn.animIndex = 0x45;
    spawn.unk86 = 10;
    spawn.unkA3 = pos->unk8;
    spawn.flags = 0x4880;
    spawn.unkA4 = 0x80;

    effect_initQuadFxTemplate(&spawn, 0xFF, 0xFF, 0xFF, 0x80);
    // matching: one load of the volatile `emitter` serves both arguments
    pos = (EmitterPos *)emitter;
    effect_spawnQuadFx(&spawn, ((ObjectEmitter *)pos)->base.header.owner, ((ObjectEmitter *)pos)->base.header.kind);
    emitter->base.header.state = (math_random() & 0x7F) + 100;
}

struct FallingObject {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x87];
    /* 0x94 */ s16 fallSpeed;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 x;
    /* 0x9E */ s16 y;
    /* 0xA0 */ s16 z;
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ s16 floorY;
    /* 0xA8 */ u8 padA8[2];
    /* 0xAA */ s16 timer;
    /* 0xAC */ u8 padAC[7];
    /* 0xB3 */ s8 unkB3;
};

/*
 * While unkA4 is below unk96, grows unkA4/unkA2 with the timer. After that the object falls with
 * a speed that ramps up to 20; once it drops below floorY it sets unk98 to -1, first spawning a
 * particle_spawnKindMark effect at the landing point when unkB3 is 1.
 */
void effect_updateFallingObject(FallingObject *obj) {
    f32 pos[4]; // matching: a 4th (unused) slot keeps pos at its original stack offset

    if (obj->unkA4 < obj->unk96) {
        obj->unkA4 = ((obj->timer >> 2) * 24) / 3;
        obj->unkA2 = ((obj->timer >> 2) * 6) / 3;
        obj->timer += gFrameTicks;
        return;
    }

    obj->fallSpeed++;
    if (obj->fallSpeed > 20) {
        obj->fallSpeed = 20;
    }

    obj->y -= (obj->fallSpeed * gFrameTicks) >> 1;
    if (obj->y < obj->floorY) {
        if (obj->unkB3 == 1) {
            pos[0] = obj->x;
            pos[1] = obj->floorY;
            pos[2] = obj->z;
            particle_spawnKindMark(5, 30.0f, 0xFF, 0, pos, obj->unkC, obj->unk1);
        }
        obj->unk98 = -1;
    }
}
