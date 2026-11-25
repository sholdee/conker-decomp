#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Parameter block for the particle emitter func_1515080C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ vertex pos;
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

/* Parameter block for the particle emitter func_15152190. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ vertex pos;
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
} Emitter2190Params;

/* Parameter block for the particle emitter func_15151A38. */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s8 unk7C;
    /* 0x7D */ s8 unk7D;
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 pad84;
} Emitter1A38Params;

extern s32 D_800AB0B0[];
extern f32 D_800AB0E0[];
extern s32 D_800AB110[];
extern f32 D_800AB114[];

void func_1515C244(struct127 *, vertex *, f32 *, f32 *);
void func_1504715C(void *, struct127 *);
void func_1515080C(Emitter080CParams *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(Emitter2190Params *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15151A38(Emitter1A38Params *, u8, s32);

/* Plays sound 0xAA at the actor and spawns three particle effects (via func_1515080C, func_15152190
 * and func_15151A38) at its origin. */
void func_151D22B0(struct127 *actor, u8 arg1, s32 arg2) {
    vertex origin;
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
            Emitter2190Params params;
            params.unk0 = 15;
            params.unk4 = 7;
            params.pos = origin;
            params.unk1C = 30.0f;
            params.unk20 = 10.0f;
            params.unk14 = 0;
            params.unk16 = 0xFF;
            params.unk18 = -29;
            params.unk1A = 20;
            params.unk2C = 32;
            params.unk2E = 18;
            params.unk24 = -1.309000015f;
            params.unk28 = 0.4200000167f;
            params.unk30 = 0.1070000082f;
            params.unk34 = 0.3000000119f;
            params.unk38 = 19.23800087f;
            func_15152190(&params, D_800AB110, D_800AB114, 1, 0.0f, 0, arg1, arg2);
        }
        {
            Emitter1A38Params params;
            params.pos = origin;
            params.unk0C = 5.0f;
            params.unk10 = 10.0f;
            params.unk14 = 0.4990000129f;
            params.unk18 = 0.7040000558f;
            params.unk1C = 20.0f;
            params.unk20 = 25.0f;
            params.unk24 = 10;
            params.unk26 = 7;
            params.unk2A = 0xFF;
            params.unk2C = -50;
            params.unk2E = 30;
            params.unk30 = 3;
            params.unk32 = 3;
            params.unk34 = 35;
            params.unk36 = 15;
            params.unk39 = 1;
            params.unk3A = 72;
            params.unk3C = 1;
            params.unk55 = 200;
            params.unk56 = 55;
            params.unk58 = 3;
            params.unk28 = 0;
            params.unk38 = 0;
            params.unk40 = 0;
            params.unk44 = 0;
            params.unk48 = 0;
            params.unk4C = 0;
            params.unk50 = 0;
            params.unk54 = 0;
            params.unk5C = 0xFF;
            params.unk60 = 0;
            params.unk64 = 0x220005;
            params.unk68 = 0x1D0600;
            params.unk6C = 3;
            params.unk70 = 59;
            params.unk74 = 0x80;
            params.unk78 = 32;
            params.unk7C = 0;
            params.unk7D = 7;
            params.unk80 = 12;
            params.unk82 = 21;
            func_15151A38(&params, arg1, arg2);
        }
    }
}
