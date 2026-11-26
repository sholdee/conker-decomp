#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spins the emitter and, while func_151464B8 allows it, throws particles from random points of its region. */
s32 func_151A6600(ZoneEmitter *obj) {
    ZoneEmitterState *state;

    state = &obj->state;
    if (state->hasZone) {
        func_1508B20C(obj->head.def.pos.x, obj->head.def.pos.y, obj->head.def.pos.z, 500.0f);
    }
    state->angle += state->angleSpeed * D_800BE9A4;
    state->angle = func_15144528(state->angle, 2048.0f, 0.0f);
    state->phase += state->phaseSpeed * D_800BE9E4;
    state->alpha = func_151423D8(state->phase - 0x40) * 63.0f + 192.0f;
    if (func_151464B8(obj->head.def.unk48) == 0) {
        state->pending += (0.3930000067f + func_150ADA68() * 0.4370000064f) * D_800BE9A4;
        if (state->pending > 1.0f) {
            ParticleDesc spawn;

            spawn.texture = 40;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x1F0600;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.r2 = 0xFF;
            spawn.g2 = 0xFF;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.b2 = 0xFF;
            spawn.fadeStart = 12;
            spawn.fadeRate = 21;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0xC207;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            do {
                func_15143874((u8)func_150ADA20(), state->region->xSize * func_150ADA68(), &spawn.pos.x,
                              &spawn.pos.z);
                spawn.pos.x += state->region->x;
                spawn.pos.y = state->region->y;
                spawn.pos.z += state->region->z;
                spawn.timer = (func_150ADA20() & 0xF) + 25;
                spawn.alpha = func_150ADA20() % 101U + 100;
                spawn.width = spawn.height = func_150ADA68() * 205.0f + 98.0f;
                func_15143794((u8)func_150ADA20(), func_150ADA20() % 35U - 64, func_150ADA68() * 10.0f + 10.0f,
                              &spawn.vel);
                // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
                spawn.gravity = func_150ADA68() * 0.2840000093f + -0.9100000262f;
                func_15130280(&spawn, 1, 0, 0, obj->head.header.owner, obj->head.header.kind);
                state->pending -= 1.0f;
            } while (state->pending > 1.0f);
        }
    }
    return 1;
}

/* Shuts the emitter down: marks its region off, calls func_1000FD38 with func_1000EF40 on it, and frees unk10. */
void func_151A6AB8(ZoneEmitter *obj) {
    obj->state.region->inactive = 1;
    func_1000FD38(func_1000EF40, obj->state.region, 0);
    if (obj->state.unk10 != NULL) {
        func_1516972C(obj->state.unk10);
    }
}
