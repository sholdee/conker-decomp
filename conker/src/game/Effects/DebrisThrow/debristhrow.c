#include <ultra64.h>
#include "effects.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/sound.h"

void effect_levelUpdateSoundCooldowns(s32 unused) {
    sound_updateCooldowns();
}

/* Throws a burst of debris (one variant, id 226) from origin. */
void effect_throwDebrisBurst(Vec3f *origin, u8 owner, s32 list) {
    DebrisBurst debris;
    const s32 variantIds[] = { 226 };
    const f32 variantScales[] = { 1.0f };

    debris.countBase = 8;
    debris.countRange = 4;
    debris.pos = *origin;
    debris.angles.yawBase = 0;
    debris.angles.yawRange = 0xFF;
    debris.angles.pitchBase = -64;
    debris.angles.pitchRange = 93;
    debris.speedBase = 8.0f;
    debris.speedRange = 4.0f;
    debris.gravityBase = -0.7060000300f;
    debris.gravityRange = 0.3220000267f;
    debris.lifeBase = 20;
    debris.lifeRange = 10;
    debris.scaleBase = 0.2510000169f;
    debris.scaleRange = 0.1990000159f;
    debris.spin = 30.69600105f;
    particle_spawnDebrisBurst(&debris, variantIds, variantScales, 1, 0.0f, 1, owner, list);
}

s32 effect_nullSoundCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound) {
    return 0;
}
