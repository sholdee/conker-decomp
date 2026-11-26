#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The variant ids and scales of the debris that func_151D22B0 sprays (func_1515080C). */
static const s32 D_800AB0B0[12] = { 0x3F, 0x40, 0x40, 0x41, 0x42, 0x42, 0x42, 0x42, 0x43, 0x43, 0x02, 0x03 };
static const f32 D_800AB0E0[12] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
/* The variant id and scale of its debris burst (func_15152190). */
static const s32 D_800AB110[1] = { 0x39 };
static const f32 D_800AB114[1] = { 1.0f };

/* Plays sound 0xAA at the actor and spawns three particle effects (via func_1515080C, func_15152190
 * and func_15151A38) at its origin. */
void func_151D22B0(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f origin;
    f32 extent1;
    f32 extent2;
    f32 scale;
    CollisionHit surface;

    if (actor != NULL) {
        sound_playActorSound2(0xAA, actor, 0x7FFF, 1000, 3000);
        sound_stopActorSound1(actor);
        scale = (actor->scale_xz + actor->scale_y) * 0.5f;
        func_1515C244(actor, &origin, &extent1, &extent2);
        func_1504715C(&surface, actor);
        {
            DebrisSpray params;
            params.countBase = 5;
            params.countRange = 5;
            params.pos = origin;
            params.angles.yawBase = 0;
            params.angles.yawRange = 0xFF;
            params.angles.pitchBase = -60;
            params.angles.pitchRange = 52;
            params.speedBase = 15.0f;
            params.speedRange = 20.0f;
            params.gravityBase = -1.447000027f;
            params.gravityRange = 0.5790000558f;
            params.lifeBase = 80;
            params.lifeRange = 30;
            params.scaleBase = 1 * scale; // matching: int factors; 1.0f/0.0f literals or plain scale/0.0f schedule differently
            params.scaleRange = 0 * scale;
            params.spin = 20.24800110f;
            params.floorHitChance = 1.0f;
            params.floorHitFn = 11;
            func_1515080C(&params, D_800AB0B0, D_800AB0E0, 12, 0, 0, 0, 1, -1, &surface, 20.0f, 0, 0, arg1, arg2);
        }
        {
            DebrisBurst params;
            params.countBase = 15;
            params.countRange = 7;
            params.pos = origin;
            params.speedBase = 30.0f;
            params.speedRange = 10.0f;
            params.angles.yawBase = 0;
            params.angles.yawRange = 0xFF;
            params.angles.pitchBase = -29;
            params.angles.pitchRange = 20;
            params.lifeBase = 32;
            params.lifeRange = 18;
            params.gravityBase = -1.309000015f;
            params.gravityRange = 0.4200000167f;
            params.scaleBase = 0.1070000082f;
            params.scaleRange = 0.3000000119f;
            params.spin = 19.23800087f;
            func_15152190(&params, D_800AB110, D_800AB114, 1, 0.0f, 0, arg1, arg2);
        }
        {
            PuffSpray params;
            params.pos = origin;
            params.sizeBase = 5.0f;
            params.sizeRange = 10.0f;
            params.growthBase = 0.4990000129f;
            params.growthRange = 0.7040000558f;
            params.speedBase = 20.0f;
            params.speedRange = 25.0f;
            params.countBase = 10;
            params.countRange = 7;
            params.angles.yawRange = 0xFF;
            params.angles.pitchBase = -50;
            params.angles.pitchRange = 30;
            params.itemCountBase = 3;
            params.itemCountRange = 3;
            params.lifeBase = 35;
            params.lifeRange = 15;
            params.headerFlags = 1;
            params.red = 72;
            params.handlers[0] = 1;
            params.alphaBase = 200;
            params.alphaRange = 55;
            params.unk58 = 3;
            params.angles.yawBase = 0;
            params.green = 0;
            params.handlers[1] = 0;
            params.handlers[2] = 0;
            params.handlers[3] = 0;
            params.handlers[4] = 0;
            params.handlers[5] = 0;
            params.blue = 0;
            params.unk5C = 0xFF;
            params.render.unk0 = 0;
            params.render.setGeometryMode = 0x220005;
            params.render.clearGeometryMode = 0x1D0600;
            params.render.renderMode = 3;
            params.render.unk10 = 59;
            params.render.unk14 = 0x80;
            params.render.unk18 = 32;
            params.render.primMode = 0;
            params.render.envMode = 7;
            params.unk80 = 12;
            params.unk82 = 21;
            func_15151A38(&params, arg1, arg2);
        }
    }
}
