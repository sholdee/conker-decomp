#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* D_8008F8E0[1]: a func_15152B38 burst at the particle's position, sized by the particle. */
void func_151A3150(FadeEventParticle *particle) {
    TrailBurst config;

    config.countBase = 10;
    config.countRange = 15;
    config.pos = particle->head.desc.pos;
    if (particle->head.desc.flags & 0x1000) {
        config.pos.x += particle->head.desc.offset.x;
        config.pos.y += particle->head.desc.offset.y;
        config.pos.z += particle->head.desc.offset.z;
    }
    config.sizeBase = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.02119999938f;
    config.sizeRange = (particle->head.desc.width + particle->head.desc.height) * 0.5f * 0.05319999903f;
    config.gravityBase = -1.031000018f;
    config.gravityRange = 0.0f;
    config.speedBase = 8.0f;
    config.speedRange = 8.0f;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.angles.pitchBase = -64;
    config.angles.pitchRange = 86;
    config.capacityBase = 3;
    config.capacityRange = 2;
    config.lifeBase = 15;
    config.lifeRange = 30;
    config.headerFlags = 1;
    config.flags = 4;
    config.primMode = 2;
    config.envMode = 3;
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
    config.colorLevelBase = 255;
    config.colorLevelRange = 0;
    config.renderMode = 1;
    config.unk21 = 0x24;
    config.setGeometryMode = 0x200005;
    config.clearGeometryMode = 0x60600;
    config.fadeStart = 15;
    config.fadeRate = 17;
    config.shrinkStart = 1;
    config.shrinkPaused = 0;
    config.shrinkFactor = 1.0f;
    config.unk38 = -1;
    config.unk39 = 0;
    config.unk72 = -1;
    config.unk73 = -1;

    func_15152B38(&config, particle->head.header.owner, particle->head.header.kind);
}
