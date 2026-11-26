#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800AB550;
extern f32 D_800AB554;

void func_151DCDE0(void *arg0, u8 arg1, s32 arg2) {
    DebrisBurst config;
    f32 temp_fv0 = 0.2f;

    config.countBase = 0xC;
    config.countRange = 5;
    config.pos = *(Vec3f *)arg0;
    config.angles.yawBase = 0;
    config.angles.yawRange = 0xFF;
    config.angles.pitchBase = -0x40;
    config.angles.pitchRange = 0x31;
    config.lifeBase = 0x19;
    config.lifeRange = 0x14;
    config.scaleBase = temp_fv0;
    config.scaleRange = temp_fv0;
    config.speedBase = 3.0f;
    config.speedRange = 9.0f;
    config.gravityBase = -0.693f;
    config.gravityRange = 0.30400002f;
    config.spin = 25.714f;
    func_15152190(&config, &D_800AB550, &D_800AB554, 1, 26.0f, 0,
                  (s32)arg1, arg2);
}
