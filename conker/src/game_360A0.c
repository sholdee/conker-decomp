#include <ultra64.h>

#include "functions.h"
#include "variables.h"

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

/* The 0x50-byte emitter descriptor handed to func_15189900 (struct145 in structs.h,
   but that copy types unk0/unk2/unk6 as u16 and unk28 as s32). */
typedef struct Game360A0Descriptor {
    s16 angle;      /* 256-step binary angle */
    s16 field2;
    s16 field4;
    s16 field6;
    f32 x;
    f32 y;
    f32 z;
    f32 width;
    f32 field18;
    f32 depth;
    f32 field20;
    f32 field24;
    f32 field28;
    f32 field2C;
    f32 field30;
    f32 field34;
    s16 field38;
    s16 field3A;
    f32 field3C;
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
    s32 field4C;
} Game360A0Descriptor;

extern void func_15143874(s16 angle, f32 dist, f32 *x, f32 *z);
extern void func_15189900(Game360A0Descriptor *descriptor, u8 style);

/* Flags the owner and spawns an emitter spanning its radius, oriented by its angles. */
s32 func_15008BF0(Game360A0Owner *owner) {
    Game360A0Descriptor descriptor;
    f32 offsetZ;
    f32 offsetX;

    owner->flags |= 4;
    descriptor.field28 = owner->radius * 0.19600001f * (1.0f / 457.0f);
    descriptor.field2C = 0.0f;
    descriptor.field48 = 4;
    descriptor.field4C = 2;
    descriptor.field38 = 100;
    descriptor.field3A = 100;
    descriptor.field44 = 40;
    descriptor.field46 = 20;
    descriptor.field30 = 7.0f;
    descriptor.field34 = 6.0f;
    descriptor.field20 = 20.1f;
    descriptor.field24 = 7.5f;
    descriptor.field3C = 0.4f;
    descriptor.field40 = 0.60700005f;
    descriptor.angle = owner->angle10 * (256.0f / 360.0f);
    descriptor.field2 = 20;
    descriptor.field4 = owner->angleC * (256.0f / 360.0f) - 64.0f;
    descriptor.field6 = 8;
    func_15143874(descriptor.angle - 64, owner->radius, &offsetX, &offsetZ);
    descriptor.x = owner->x - offsetX;
    descriptor.y = owner->y;
    descriptor.z = owner->z - offsetZ;
    descriptor.width = 2.0f * offsetX;
    descriptor.field18 = 0.0f;
    descriptor.depth = 2.0f * offsetZ;
    func_15189900(&descriptor, owner->style);
    return 1;
}
