/* game/Level/LevelSetupLab/levelsetuplab.c -- level_setupLab: the level's six floating models, three of them with a light. */
#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"

static const Vec3f gSetup23ModelPositions[6] = {
    { 3432.0f, 593.0f, -255.0f }, { 3627.0f, 593.0f, 291.0f }, { 3065.0f, 593.0f, 290.0f },
    { 2984.0f, 593.0f, -274.0f }, { 2489.0f, 593.0f, 298.0f }, { 2519.0f, 593.0f, -261.0f },
};
static const u8 gSetup23ModelLights[6] = { 1, 0, 1, 0, 0, 1 }; /* which of them get a light */

void level_setupLab(void) {
    ModelParticle *result;
    ParticleSpawn78 part;
    s32 i;
    DynModelDesc desc;
    SwayBlock block;
    Sway sway;
    f32 rnd;
    DynModel *ent;
    LightEffectHeader header;
    Vec3i header2;
    void *payload;
    LightEffect *snd;
    Vec3f pos;

    particle_startShakeTimer();

    desc.updateFunc = 3;
    desc.drawFunc = 6;
    desc.lifetime = 0x12C;
    desc.assetTable = 9;
    desc.flags = 0;
    desc.assetIndex = 0x1AF;
    desc.kind = 0;

    part.flags = 0xD00;
    part.lifetime = 0x12C;
    part.model = 0x56;
    part.variant = 0;
    part.alpha = 0xFF;
    part.updateFn = 0x10;
    part.update2Fn = 0;
    part.floorHitFn = 0;
    part.floorType3Fn = 0;
    part.wallHitFn = 0;
    part.destroyFn = 0;
    part.squashFn = 0;
    part.matrixFn = 2;
    part.lightMode = 2;
    part.owner = 0;
    part.ownerId = 0;
    part.fadeStart = 1;
    part.fadeRate = 0xFF;

    part.radius = 1.0f;
    part.elasticity = 1.0f;
    part.sizeY = 1.0f;
    part.sizeXZ = 1.0f;
    part.rot.x = 0.0f;
    part.rot.y = 0.0f;
    part.rot.z = 0.0f;
    part.scale.x = 1.0f;
    part.scale.y = 1.0f;
    part.scale.z = 1.0f;
    part.vel.x = 0.0f;
    part.vel.y = 0.0f;
    part.vel.z = 0.0f;
    part.rotVel.x = 0.0f;
    part.rotVel.y = 0.0f;
    part.rotVel.z = 0.0f;
    part.gravity = 0.0f;
    block.rotX = 0.0f;
    block.rotZ = 0.0f;

    for (i = 0; i < 6; i++) {
        pos = gSetup23ModelPositions[i];
        part.pos = pos;
        block.pos = pos;

        collision_selectMap(0);
        part.zone = collision_findZoneLeaf(part.pos.x, part.pos.z);

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.phaseX = (rnd + rnd) * 3.141592741f;
        block.sway.phaseX = sway.phaseX;

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.phaseZ = (rnd + rnd) * 3.141592741f;
        block.sway.phaseZ = sway.phaseZ;

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.amplitudeX = rnd * 3.800000191f + 2.0f;
        block.sway.amplitudeX = sway.amplitudeX;

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.amplitudeZ = rnd * 3.800000191f + 2.0f;
        block.sway.amplitudeZ = sway.amplitudeZ;

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.speedX = rnd * 0.1100000069f + 0.05000000075f;
        block.sway.speedX = sway.speedX;

        rnd = (osGetCount() * math_random()) & 0xFFFF;
        rnd *= 1.525902189e-05f;
        sway.speedZ = rnd * 0.1100000069f + 0.05000000075f;
        block.sway.speedZ = sway.speedZ;

        result = particle_newModelParticle(&part, 3, 0xFF, 0x18, 0xFF, 1);
        if (result != NULL) {
            memcpy(result + 1, &sway, sizeof(sway));
        }

        if (gSetup23ModelLights[i] != 0) {
            ent = effect_newDynModel(&desc, 0, 0x2C, 0xFF, 1);
            if (ent != NULL) {
                memcpy(DYNMODEL_USERDATA(ent), &block, sizeof(block));
            }
            header2.x = gSetup23ModelPositions[i].x;
            header2.y = gSetup23ModelPositions[i].y;
            header2.z = gSetup23ModelPositions[i].z;
            payload = result;
            header.flags = 2;
            header.update = 0x16;
            header.lifetime = 0x12C;
            header.lightType = 0;
            snd = light_createWithEffect(&header, &header2, 0xC, 0xFF, 0xFF, 0xFF, 0xFF, 0, 4, 0xFF, 1);
            if (snd != NULL) {
                memcpy(&snd->data.object, &payload, sizeof(payload));
            }
        }
    }
}
