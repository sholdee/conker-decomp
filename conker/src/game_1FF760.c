#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Parameter block for the particle emitter func_1515080C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
} Emitter080CParams;



extern s32 D_800AB0B0[];
extern f32 D_800AB0E0[];
extern s32 D_800AB110[];
extern f32 D_800AB114[];

void func_1515C244(Actor *, Vec3f *, f32 *, f32 *);
void func_1504715C(void *, Actor *);
void func_1515080C(Emitter080CParams *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(DebrisBurst *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15151A38(PuffSpray *, u8, s32);

/* Plays sound 0xAA at the actor and spawns three particle effects (via func_1515080C, func_15152190
 * and func_15151A38) at its origin. */
void func_151D22B0(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f origin;
    f32 extent1;
    f32 extent2;
    f32 scale;
    u8 surface[0x24];

    if (actor != NULL) {
        func_10010154(0xAA, actor, 0x7FFF, 1000, 3000);
        func_10010A3C(actor);
        scale = (actor->xz_scale + actor->y_scale) * 0.5f;
        func_1515C244(actor, &origin, &extent1, &extent2);
        func_1504715C(surface, actor);
        {
            Emitter080CParams params;
            params.unk0 = 5;
            params.unk4 = 5;
            params.pos = origin;
            params.unk14 = 0;
            params.unk16 = 0xFF;
            params.unk18 = -60;
            params.unk1A = 52;
            params.unk1C = 15.0f;
            params.unk20 = 20.0f;
            params.unk24 = -1.447000027f;
            params.unk28 = 0.5790000558f;
            params.unk2C = 80;
            params.unk2E = 30;
            params.unk30 = 1 * scale; // matching: int factors; 1.0f/0.0f literals or plain scale/0.0f schedule differently
            params.unk34 = 0 * scale;
            params.unk38 = 20.24800110f;
            params.unk3C = 1.0f;
            params.unk40 = 11;
            func_1515080C(&params, D_800AB0B0, D_800AB0E0, 12, 0, 0, 0, 1, -1, surface, 20.0f, 0, 0, arg1, arg2);
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
            params.render.envMode = 0;
            params.render.primMode = 7;
            params.unk80 = 12;
            params.unk82 = 21;
            func_15151A38(&params, arg1, arg2);
        }
    }
}
