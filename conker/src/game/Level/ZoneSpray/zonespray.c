#include <ultra64.h>

#include "conker/effect.h"
#include "conker/level.h"
#include "conker/trail.h"

/* Zone handler gNeutralZoneSpawnHandlers[...]: flags the zone and spawns a spray spanning its xSize, oriented by its pitch/yaw;
   data1 selects the spray style. */
s32 level_spawnZoneSpray(TriggerZone *zone) {
    SprayDesc descriptor;
    f32 offsetZ;
    f32 offsetX;

    zone->flags |= 4;
    descriptor.rate = zone->xSize * 0.19600001f * (1.0f / 457.0f);
    descriptor.rateRange = 0.0f;
    descriptor.pointCount = 4;
    descriptor.pointCountRange = 2;
    descriptor.alpha = 100;
    descriptor.alphaRange = 100;
    descriptor.life = 40;
    descriptor.lifeRange = 20;
    descriptor.halfWidth = 7.0f;
    descriptor.halfWidthRange = 6.0f;
    descriptor.speed = 20.1f;
    descriptor.speedRange = 7.5f;
    descriptor.gravity = 0.4f;
    descriptor.gravityRange = 0.60700005f;
    descriptor.yaw = zone->yaw * (256.0f / 360.0f);
    descriptor.yawSpread = 20;
    descriptor.pitch = zone->pitch * (256.0f / 360.0f) - 64.0f;
    descriptor.pitchRange = 8;
    math_dirFromBinAngleXZ(descriptor.yaw - 64, zone->xSize, &offsetX, &offsetZ);
    descriptor.origin.x = zone->x - offsetX;
    descriptor.origin.y = zone->y;
    descriptor.origin.z = zone->z - offsetZ;
    descriptor.extent.x = 2.0f * offsetX;
    descriptor.extent.y = 0.0f;
    descriptor.extent.z = 2.0f * offsetZ;
    trail_createSprayEmitter(&descriptor, zone->data1);
    return 1;
}
