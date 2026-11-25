/* matching: functions.h types func_1513D2F0's arg1 as s32; this file passes a pointer (an int-typed
 * address changes which loop invariants IDO keeps in saved registers). */
#define func_1513D2F0 func_1513D2F0_s32_decl_in_functions_h
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_1513D2F0

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

typedef struct Game1FBB80Packet {
    u8 kind;
    u8 unk01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 colour[4];
    f32 unk14;
    f32 unk18;
    f32 x, y, z;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u32 flags40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    Game1FBB80Owner *owner;
    u8 pad4C[0xC]; /* func_1513D2F0 copies the complete 0x58-byte packet. */
} Game1FBB80Packet;

s32 func_1504530C(void *, f32, void *);
void *func_1513D2F0(void *, void *, u8, u8, u8, u8, u8, s32, s32, s32, u8, s32);
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
            Game1FBB80Packet packet;
            f32 upperHeight;

            packet.kind = 0x6A;
            packet.unk01 = 0;
            packet.unk02 = 0x2203;
            packet.unk04 = 100;
            packet.unk08 = 0;
            packet.unk0C = 0;
            packet.colour[0] = 0xFF;
            packet.colour[1] = 0xFF;
            packet.colour[2] = 0xFF;
            packet.colour[3] = 0xFF;
            packet.unk2C = 0.0f;
            packet.unk30 = 0.0f;
            packet.unk38 = 0.0f;
            packet.unk3C = 1.0f;
            packet.flags40 = 0x01CC0061;
            packet.unk45 = 0xFF;
            packet.unk46 = 0;
            packet.unk47 = 7;
            packet.owner = effect->owner;
            do {
                // matching: `+ -1308.0f` (add.s); `- 1308.0f` emits sub.s
                packet.unk28 = (func_150ADA68() * 409.0f + -1308.0f) * 0.001f;
                packet.unk34 = (func_150ADA68() * 121.0f + 23.0f) * 0.001f;
                packet.unk14 = func_150ADA68() * 25.0f + 10.0f;
                packet.unk18 = func_150ADA68() * 60.0f + 60.0f;
                func_151432BC(effect->shape, &packet.x, &packet.z, &upperHeight, &packet.y);
                if (func_1504530C(&packet.x, upperHeight, &effect->height) != 0) {
                    packet.y = effect->height;
                    packet.unk44 = func_150ADA20() % 101U + 155;
                    func_1513D2F0(&packet, &D_800A4AA0, 0x1C, 0, 0, 0x22, 0, 0, 0, 0, object->flags0C, object->flags01);
                }
                effect->accumulator -= 1.0f;
            } while (effect->accumulator > 1.0f);
        }
    }
}
