#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Emitter update: accumulates a randomized spawn rate and, for each whole unit, spawns one
 * 0x6A effect packet at a random point of the shape (only when its zone's view mask has one of its
 * low D_80082FA0 + 1 bits set, i.e. a view can see it; otherwise the accumulated units are discarded). */
void func_151CE6D0(Game1FBB80Object *object) {
    Game1FBB80Effect *effect;
    s32 index;
    s16 mask;

    effect = &object->effect;
    effect->accumulator += (5624.0f + func_150ADA68() * 3973.0f) * 1.0e-10f * D_800BE9A4 * effect->rate;
    if (effect->accumulator > 1.0f) {
        for (index = 0, mask = 0; index <= D_80082FA0; index++) {
            mask |= 1 << index;
        }
        if (!(effect->zone->views & mask)) {
            do {
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
            return;
        }
        {
            ParticleDef packet;
            f32 upperHeight;

            packet.texture = 0x6A;
            packet.type = 0;
            packet.renderModes = 0x2203;
            packet.lifetime = 100;
            packet.frame = 0;
            packet.frameSpeed = 0;
            packet.rgba[0] = 0xFF;
            packet.rgba[1] = 0xFF;
            packet.rgba[2] = 0xFF;
            packet.rgba[3] = 0xFF;
            packet.rot.y = 0.0f;
            packet.rot.z = 0.0f;
            packet.scale.y = 0.0f;
            packet.scale.z = 1.0f;
            packet.flags = 0x01CC0061;
            packet.unk45 = 0xFF;
            packet.unk46 = 0;
            packet.unk47 = 7;
            packet.unk48 = effect->zone;
            do {
                // matching: `+ -1308.0f` (add.s); `- 1308.0f` emits sub.s
                packet.rot.x = (func_150ADA68() * 409.0f + -1308.0f) * 0.001f;
                packet.scale.x = (func_150ADA68() * 121.0f + 23.0f) * 0.001f;
                packet.width = func_150ADA68() * 25.0f + 10.0f;
                packet.height = func_150ADA68() * 60.0f + 60.0f;
                func_151432BC(effect->shape, &packet.pos.x, &packet.pos.z, &upperHeight, &packet.pos.y);
                if (func_1504530C(&packet.pos, upperHeight, &effect->floor) != 0) {
                    packet.pos.y = effect->floor.height;
                    packet.unk44 = func_150ADA20() % 101U + 155;
                    func_1513D2F0(&packet, &D_800A4AA0, 0x1C, 0, 0, 0x22, 0, 0, 0, 0, object->base.header.owner, object->base.header.kind);
                }
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
        }
    }
}
