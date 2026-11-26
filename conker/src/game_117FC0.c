#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/math.h"

/* Spawner state: flashes run between a random point on the start line and one on the end line. */
typedef struct {
    /* 0x00 */ Vec2fXZ startOrigin;
    /* 0x08 */ Vec2fXZ endOrigin;
    /* 0x10 */ Vec2fXZ startExtent;
    /* 0x18 */ Vec2fXZ endExtent;
    /* 0x20 */ f32 height;
    /* 0x24 */ f32 probeHeight;
    /* 0x28 */ f32 rate;        /* flashes per frame */
    /* 0x2C */ f32 rateRange;
    /* 0x30 */ f32 accumulator;
    /* 0x34 */ f32 speed;
    /* 0x38 */ f32 speedRange;
    /* 0x3C */ u8  colour[4];
} FlashSpawner;

struct struct_EAB10 {
    /* 0x00 */ u8  pad0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ u8  pad2[0xA];
    /* 0x0C */ u8  unkC;
    /* 0x0D */ u8  padD[0x1B];
    /* 0x28 */ FlashSpawner spawner;
};

/* Spawns this frame's share of flashes, each sliding from one line to the other along the ground. */
void func_150EAB10(struct_EAB10 *actor) {
    s32 pad; // matching: unused stack slots that keep the frame layout
    struct GroundFlash flash;
    LightEffectHeader header;
    s32 padB0; // matching: see pad
    Vec2fXZ end;
    Vec2fXZ start;
    f32 speed;
    f32 length;
    f32 random;
    Vec3i origin;
    LightEffect *obj;
    FlashSpawner *spawner;

    spawner = &actor->spawner;
    spawner->accumulator += (spawner->rate + func_150ADA68() * spawner->rateRange) * gFrameDelta;
    if (spawner->accumulator > 1.0f) {
        flash.ground.material = 0;
        flash.ground.flags = 0;
        flash.ground.source = 0;
        flash.ground.surface = 0;
        flash.ground.height = -10000.0f;
        flash.height = spawner->height;
        flash.probeHeight = spawner->probeHeight;
        header.flags = 2;
        header.update = 19;
        header.lifetime = 300;
        header.lightType = 0;
        do {
            speed = spawner->speed + spawner->speedRange * func_150ADA68();
            if (speed != 0.0f) {
                random = func_150ADA68();
                start.x = spawner->startExtent.x * random + spawner->startOrigin.x;
                start.z = spawner->startExtent.z * random + spawner->startOrigin.z;
                random = func_150ADA68();
                end.x = spawner->endExtent.x * random + spawner->endOrigin.x;
                end.z = spawner->endExtent.z * random + spawner->endOrigin.z;
                flash.velocity.x = end.x - start.x;
                flash.velocity.z = end.z - start.z;
                length = sqrtf(flash.velocity.x * flash.velocity.x + flash.velocity.z * flash.velocity.z);
                if (length != 0.0f) {
                    random = 1.0f / length;
                    flash.velocity.x *= random;
                    flash.velocity.z *= random;
                    if (func_150ADA20() & 1) {
                        flash.position = start;
                    } else {
                        flash.position = end;
                        flash.velocity.x = -flash.velocity.x;
                        flash.velocity.z = -flash.velocity.z;
                    }
                    flash.velocity.x *= speed;
                    flash.velocity.z *= speed;
                    flash.lifetime = length / speed;
                    origin.x = flash.position.x;
                    origin.y = spawner->height;
                    origin.z = flash.position.z;
                    obj = func_151602C0(&header, &origin, spawner->colour[3], spawner->colour[0],
                                        spawner->colour[1], spawner->colour[2], 0xFF, 0, 0x40, actor->unkC,
                                        actor->unk1);
                    if (obj != NULL) {
                        memcpy(&obj->data.groundFlash, &flash, sizeof(flash));
                    }
                }
            }
            spawner->accumulator -= 1.0f;
        } while (spawner->accumulator > 1.0f);
    }
}


/* Slides the flash along the ground and keeps its light on the ground; returns 0 once it expires. */
s32 func_150EAE24(LightEffect *obj) {
    struct GroundFlash *flash;
    Vec3f probe;

    obj->data.groundFlash.position.x += obj->data.groundFlash.velocity.x * gFrameDelta;
    obj->data.groundFlash.position.z += obj->data.groundFlash.velocity.z * gFrameDelta;
    probe.x = obj->data.groundFlash.position.x;
    probe.y = obj->data.groundFlash.height;
    probe.z = obj->data.groundFlash.position.z;
    obj->light->x = obj->data.groundFlash.position.x;
    obj->light->z = obj->data.groundFlash.position.z;
    flash = &obj->data.groundFlash;
    if (func_15046C80(&probe, 0, obj->data.groundFlash.probeHeight, &flash->ground) != 0) {
        obj->light->y = flash->ground.height;
    } else {
        obj->light->y = flash->height;
    }
    flash->lifetime -= gFrameDelta;
    if (flash->lifetime <= 0.0f) {
        return 0;
    }
    return 1;
}
