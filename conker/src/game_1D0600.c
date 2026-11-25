#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Fade-out and one-shot event of func_151A26EC's particles, run by func_151A2960. */
typedef struct {
    /* 0x0 */ s16 fadeOutStart;  /* desc.timer value */
    /* 0x2 */ s16 fadeOutStep;
    /* 0x4 */ s16 eventTime;     /* desc.timer value, -1 for none */
    /* 0x6 */ s8 eventFunc;      /* D_8008F8E0 index, -1 for none */
} FadeEvent;

/* The particle func_15130374 returns (layout as DescParticle in game_1CE2F0.c). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 unk80[0x28];
    /* 0xA8 */ f32 drag;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ FadeEvent fade;
} DescParticle;

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color8;

/* Settings of a func_15152B38 burst (layout as BurstConfig in game_1CE2F0.c). */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ struct17 position;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ s16 yawBase;
    /* 0x2E */ s16 yawRange;
    /* 0x30 */ s16 pitchBase;
    /* 0x32 */ s16 pitchRange;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 lifeBase;
    /* 0x3E */ s16 lifeRange;
    /* 0x40 */ u16 kind;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ Color8 colorBase;
    /* 0x49 */ Color8 colorRange;
    /* 0x4D */ Color8 color2Base;
    /* 0x51 */ Color8 color2Range;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} BurstConfig;

void func_15152B38(BurstConfig *, u8, s32);

/* D_8008F8E0[1]: a func_15152B38 burst at the particle's position, sized by the particle. */
void func_151A3150(DescParticle *particle) {
    BurstConfig config;

    config.countBase = 10;
    config.countRange = 15;
    config.position = *(struct17 *)&particle->desc.pos; // TODO(vectors): struct17/vertex mix
    if (particle->desc.flags & 0x1000) {
        config.position.unk0 += particle->desc.offset.x;
        config.position.unk4 += particle->desc.offset.y;
        config.position.unk8 += particle->desc.offset.z;
    }
    config.sizeBase = (particle->desc.width + particle->desc.height) * 0.5f * 0.02119999938f;
    config.sizeRange = (particle->desc.width + particle->desc.height) * 0.5f * 0.05319999903f;
    config.unk1C = -1.031000018f;
    config.unk20 = 0.0f;
    config.speedBase = 8.0f;
    config.speedRange = 8.0f;
    config.yawBase = 0;
    config.yawRange = 255;
    config.pitchBase = -64;
    config.pitchRange = 86;
    config.unk34 = 3;
    config.unk38 = 2;
    config.lifeBase = 15;
    config.lifeRange = 30;
    config.kind = 1;
    config.unk42 = 4;
    config.unk43 = 2;
    config.unk44 = 3;
    config.colorBase.red = 255;
    config.colorBase.green = 200;
    config.colorBase.blue = 200;
    config.colorBase.alpha = 255;
    config.colorRange.red = 0;
    config.colorRange.green = 55;
    config.colorRange.blue = 55;
    config.colorRange.alpha = 0;
    config.color2Base.red = 255;
    config.color2Base.green = 255;
    config.color2Base.blue = 255;
    config.color2Base.alpha = 255;
    config.color2Range.red = 0;
    config.color2Range.green = 0;
    config.color2Range.blue = 0;
    config.color2Range.alpha = 0;
    config.unk55 = 255;
    config.unk56 = 0;
    config.unk57 = 1;
    config.unk58 = 0x24;
    config.unk5C = 0x200005;
    config.unk60 = 0x60600;
    config.unk64 = 15;
    config.unk66 = 17;
    config.unk68 = 1;
    config.unk6A = 0;
    config.unk6C = 1.0f;
    config.unk70 = -1;
    config.unk71 = 0;
    config.unk72 = -1;
    config.unk73 = -1;

    func_15152B38(&config, particle->unkC, particle->unk1);
}
