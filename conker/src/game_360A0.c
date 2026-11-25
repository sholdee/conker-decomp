#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "effects.h"

/* structs.h's struct110 types offsets 0/2/4 as u8 and 0xC/0x10 as u8 as well; the
   golden code reads them with lh / lwc1.  File-local typedef per the usual policy. */
typedef struct Game360A0Owner {
    s16 x;
    s16 y;
    s16 z;
    s16 radius;
    u8  pad8[0x4];
    f32 angleC;     /* degrees */
    f32 angle10;    /* degrees */
    u8  pad14[0x2];
    u8  flags;
    u8  pad17[0x4];
    u8  style;
} Game360A0Owner;

extern void func_15143874(s16 angle, f32 dist, f32 *x, f32 *z);
extern void func_15189900(SprayDesc *descriptor, u8 style);

/* Flags the owner and spawns an emitter spanning its radius, oriented by its angles. */
s32 func_15008BF0(Game360A0Owner *owner) {
    SprayDesc descriptor;
    f32 offsetZ;
    f32 offsetX;

    owner->flags |= 4;
    descriptor.rate = owner->radius * 0.19600001f * (1.0f / 457.0f);
    descriptor.rateRange = 0.0f;
    descriptor.pointCount = 4;
    descriptor.pointCountRange = 2;
    descriptor.unk1B = 100;
    descriptor.unk1BRange = 100;
    descriptor.unkC = 40;
    descriptor.unkCRange = 20;
    descriptor.halfWidth = 7.0f;
    descriptor.halfWidthRange = 6.0f;
    descriptor.speed = 20.1f;
    descriptor.speedRange = 7.5f;
    descriptor.gravity = 0.4f;
    descriptor.gravityRange = 0.60700005f;
    descriptor.yaw = owner->angle10 * (256.0f / 360.0f);
    descriptor.yawSpread = 20;
    descriptor.pitch = owner->angleC * (256.0f / 360.0f) - 64.0f;
    descriptor.pitchRange = 8;
    func_15143874(descriptor.yaw - 64, owner->radius, &offsetX, &offsetZ);
    descriptor.origin.x = owner->x - offsetX;
    descriptor.origin.y = owner->y;
    descriptor.origin.z = owner->z - offsetZ;
    descriptor.extent.x = 2.0f * offsetX;
    descriptor.extent.y = 0.0f;
    descriptor.extent.z = 2.0f * offsetZ;
    func_15189900(&descriptor, owner->style);
    return 1;
}
