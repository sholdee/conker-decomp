#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

typedef struct Game1FBB80Owner {
    s16 field00;
    s16 mask;
} Game1FBB80Owner;

typedef struct Game1FBB80Effect {
    void *shape;
    f32 rate;
    f32 accumulator;
    Game1FBB80Owner *owner;
    f32 height;
} Game1FBB80Effect;

typedef struct Game1FBB80Object {
    u8 pad00, flags01;
    u8 pad02[0xA];
    u8 flags0C;
    u8 pad0D[0x1B];
    Game1FBB80Effect effect;
} Game1FBB80Object;

s32 func_1504530C(void *, f32, void *);
void func_151432BC(void *, f32 *, f32 *, f32 *, f32 *);

/* Emitter update: accumulates a randomized spawn rate and, for each whole unit, spawns one
 * 0x6A effect packet at a random point of the shape (only when the owner's mask has one of its
 * low D_80082FA0 + 1 bits set; otherwise the accumulated units are discarded). */
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
        if (!(effect->owner->mask & mask)) {
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
            packet.unk48 = effect->owner;
            do {
                // matching: `+ -1308.0f` (add.s); `- 1308.0f` emits sub.s
                packet.rot.x = (func_150ADA68() * 409.0f + -1308.0f) * 0.001f;
                packet.scale.x = (func_150ADA68() * 121.0f + 23.0f) * 0.001f;
                packet.width = func_150ADA68() * 25.0f + 10.0f;
                packet.height = func_150ADA68() * 60.0f + 60.0f;
                func_151432BC(effect->shape, &packet.pos.x, &packet.pos.z, &upperHeight, &packet.pos.y);
                if (func_1504530C(&packet.pos.x, upperHeight, &effect->height) != 0) {
                    packet.pos.y = effect->height;
                    packet.unk44 = func_150ADA20() % 101U + 155;
                    func_1513D2F0(&packet, &D_800A4AA0, 0x1C, 0, 0, 0x22, 0, 0, 0, 0, object->flags0C, object->flags01);
                }
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
        }
    }
}
