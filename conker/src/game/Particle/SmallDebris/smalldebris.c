#include <ultra64.h>
#include "effects.h"
#include "conker/particle.h"

/* The debris variant: model id and scale. */
static const s32 gSmallDebrisVariants[1] = { 0x10 };
static const f32 gSmallDebrisScales[1] = { 1.0f };

void particle_spawnSmallDebrisBurst(Vec3f *pos, u8 owner, s32 list) {
    DebrisBurst config;
    f32 scale = 0.2f;

    config.countBase = 0xC;
    config.countRange = 5;
    config.pos = *pos;
    config.angles.yawBase = 0;
    config.angles.yawRange = 0xFF;
    config.angles.pitchBase = -0x40;
    config.angles.pitchRange = 0x31;
    config.lifeBase = 0x19;
    config.lifeRange = 0x14;
    config.scaleBase = scale;
    config.scaleRange = scale;
    config.speedBase = 3.0f;
    config.speedRange = 9.0f;
    config.gravityBase = -0.693f;
    config.gravityRange = 0.30400002f;
    config.spin = 25.714f;
    particle_spawnDebrisBurst(&config, gSmallDebrisVariants, gSmallDebrisScales, 1, 26.0f, 0, owner, list);
}
