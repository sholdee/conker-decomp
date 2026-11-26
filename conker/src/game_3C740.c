#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns a flat sprite (texture 0x38) of 110 x 110 at (x, y, z). */
void *func_1500F290(f32 x, f32 y, f32 z) {
    SpriteDef def;
    s32 rand;

    def.texture = 0x38;
    def.frame = 0;
    rand = func_151EF610();
    def.frameSpeed = (rand % 0x1000) + 0x4000;
    def.flags = 0x20014;
    def.lifetime = 1;
    def.unk10 = 0xFF;
    def.unk11 = 0xFF;
    def.rgba[0] = 0;
    def.rgba[1] = 0;
    def.rgba[2] = 0;
    def.rgba[3] = 0xFF;
    def.renderModes = 0x30001;
    return particle_spawnFlatQuadSpriteNoModel(&def, 0, 0, 0, x, y, z, 110.0f, 110.0f, 0, 0, 0, 0xFF, 0);
}

/* Starts an object emitter at (x, y, z) over the floor at floorY. */
void func_1500F378(s32 x, s32 y, s32 z, s32 floorY) {
    ObjectEmitter *emitter;

    emitter = effect_newTimedEffectNoDraw((func_150ADA20() & 0x7F) + 0xA, 1, -1, 1, 0, 0xA, 0xFF, 0);
    if (emitter != NULL) {
        emitter->pos.x = x;
        emitter->pos.y = y;
        emitter->pos.z = z;
        emitter->pos.floorY = floorY;
        emitter->pos.unk8 = 1;
    }
}

void func_1500F40C(void) {
    SprayDesc descriptor;

    {
        extern EffectHeader *gLevelSprayEffects[4]; // matching: the block-scope redeclaration changes IDO's store scheduling

        func_1500F378(0x1612, -0x553, -0x2DC, -0x69F);
        func_1500F378(0x1633, -0x553, -0x3C2, -0x69F);
        gLevelSprayEffects[0] = func_1500F290(4054.0f, -1692.0f, 0.0f);
        gLevelSprayEffects[1] = func_1500F290(3890.0f, -1692.0f, -78.0f);
        gLevelSprayEffects[2] = func_1500F290(4380.0f, -1692.0f, -664.0f);
        gLevelSprayEffects[3] = func_1500F290(4218.0f, -1692.0f, -740.0f);
    }
    descriptor.rate = 0.0f;
    descriptor.rateRange = 0.2400000244f;
    descriptor.halfWidth = 2.0f;
    descriptor.halfWidthRange = 6.0f;
    descriptor.speed = 7.0f;
    descriptor.speedRange = 10.0f;
    descriptor.gravity = 0.4970000088f;
    descriptor.gravityRange = 0.3150000274f;
    descriptor.pointCount = 3;
    descriptor.pointCountRange = 2;
    descriptor.yawSpread = 0x12;
    descriptor.pitch = -0x15;
    descriptor.pitchRange = 0xF;
    descriptor.unk1B = 0x9B;
    descriptor.unk1BRange = 0x64;
    descriptor.unkC = 0x29;
    descriptor.unkCRange = 0x29;
    descriptor.origin.x = 2686.0f;
    descriptor.origin.y = -1667.0f;
    descriptor.origin.z = 2063.0f;
    descriptor.extent.x = 2810.0f - descriptor.origin.x;
    descriptor.extent.y = 0;
    descriptor.extent.z = 1637.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 2810.0f;
    descriptor.origin.y = -1667.0f;
    descriptor.origin.z = 1637.0f;
    descriptor.extent.x = 2667.0f - descriptor.origin.x;
    descriptor.extent.y = -1668.0f - descriptor.origin.y;
    descriptor.extent.z = 1236.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 2667.0f;
    descriptor.origin.y = -1668.0f;
    descriptor.origin.z = 1236.0f;
    descriptor.extent.x = 2220.0f - descriptor.origin.x;
    descriptor.extent.y = -1676.0f - descriptor.origin.y;
    descriptor.extent.z = 1025.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 1894.0f;
    descriptor.origin.y = -1674.0f;
    descriptor.origin.z = 788.0f;
    descriptor.extent.x = 1841.0f - descriptor.origin.x;
    descriptor.extent.y = -1677.0f - descriptor.origin.y;
    descriptor.extent.z = 557.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 1841.0f;
    descriptor.origin.y = -1677.0f;
    descriptor.origin.z = 557.0f;
    descriptor.extent.x = 1594.0f - descriptor.origin.x;
    descriptor.extent.y = -1678.0f - descriptor.origin.y;
    descriptor.extent.z = 455.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 1594.0f;
    descriptor.origin.y = -1678.0f;
    descriptor.origin.z = 455.0f;
    descriptor.extent.x = 1400.0f - descriptor.origin.x;
    descriptor.extent.y = 0.0f;
    descriptor.extent.z = 613.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 1253.0f;
    descriptor.origin.y = -1673.0f;
    descriptor.origin.z = 668.0f;
    descriptor.extent.x = 1075.0f - descriptor.origin.x;
    descriptor.extent.y = -1682.0f - descriptor.origin.y;
    descriptor.extent.z = 572.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 1075.0f;
    descriptor.origin.y = -1682.0f;
    descriptor.origin.z = 572.0f;
    descriptor.extent.x = 872.0f - descriptor.origin.x;
    descriptor.extent.y = -1680.0f - descriptor.origin.y;
    descriptor.extent.z = 687.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
    descriptor.origin.x = 872.0f;
    descriptor.origin.y = -1680.0f;
    descriptor.origin.z = 687.0f;
    descriptor.extent.x = 772.0f - descriptor.origin.x;
    descriptor.extent.y = -1676.0f - descriptor.origin.y;
    descriptor.extent.z = 931.0f - descriptor.origin.z;
    descriptor.yaw = (s16)(func_150484A0(descriptor.extent.x, descriptor.extent.z) * 40.74366379f) - 64;
    trail_createSprayEmitter(&descriptor, 1);
}
