#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/math.h"

void func_150E5AE0(void);

/* A sine oscillator: value = base + amplitude * sin(phase); phase advances by phaseStep per frame. */
typedef struct {
    /* 0x00 */ u32 phase;
    /* 0x04 */ s32 phaseStep;
    /* 0x08 */ f32 base;
    /* 0x0C */ f32 amplitude;
} Oscillator;

/* An object created by func_15134DAC with an Oscillator in its user data. */
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ Oscillator oscillator;
} OscillatorObject;

/* Extra data func_151303BC's caller copies into the particle at +0xA8. */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ u8  unk4[4];
    /* 0x08 */ f32 unk8[2];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8  unk18[4];
    /* 0x1C */ f32 unk1C[2];
    /* 0x24 */ f32 unk24;
} ParticleExtra;

/* The owner whose +0x74 scales the spawned particle's velocity. */
typedef struct {
    /* 0x00 */ u8  pad0[0x74];
    /* 0x74 */ f32 unk74;
} struct_EA11C_arg5;

void *func_151303BC(ParticleDesc *, s32, s32);

void *func_15134DAC(BloodStripDesc *, s32);

/* Creates a func_15134DAC object attached to `owner`, with a slowly pulsing oscillator. */
OscillatorObject *func_150E9FE0(Actor *owner, s16 arg1, s32 arg2, s32 arg3) {
    BloodStripDesc desc;
    OscillatorObject *obj;
    Oscillator oscillator;

    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.bone = 8;
    desc.offsetA.x = -9.0f;
    desc.offsetA.y = -14.0f;
    desc.offsetA.z = 27.0f;
    desc.offsetB.x = -21.0f;
    desc.offsetB.y = -14.0f;
    desc.offsetB.z = 41.0f;
    desc.flags = 2;
    desc.duration = 60;
    desc.pause = 60;
    desc.lifetime = arg1;
    desc.sampleCallback = 2;
    desc.emitCallback = 2;
    desc.rateCallback = 2;
    desc.rate = 0.5f;
    desc.mode = 3;
    desc.updateCallback = -1;

    oscillator.phase = 0;
    oscillator.phaseStep = 0x11111;
    oscillator.base = 0.2f;
    oscillator.amplitude = 0.060000002f;

    obj = func_15134DAC(&desc, 0x10);
    if (obj != NULL) {
        memcpy(&obj->oscillator, &oscillator, sizeof(oscillator));
    }
    return obj;
}

/* Restarts the object's oscillator from phase 0. */
s32 func_150EA10C(OscillatorObject *obj) {
    obj->oscillator.phase = 0;
    return 1;
}

/* Spawns a particle on the segment `from`->`to`, `t` of the way along, drifting that way. */
void func_150EA11C(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, struct_EA11C_arg5 *arg5) {
    ParticleDesc desc;
    u8 *particle;
    ParticleExtra extra;

    desc.texture = 0x2F;
    desc.renderMode = 0xC01;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.b2 = 0;
    desc.g2 = 0;
    desc.r2 = 0;
    desc.a = 0;
    desc.b = 0;
    desc.g = 0;
    desc.r = 0;
    desc.frameRate = 0;
    desc.frame = 0;
    desc.brightness = 0xFF;
    desc.scaleStart = 0;
    desc.scaleRate = 0.0f;
    desc.flags = 0x1207;
    desc.primColorMode = 5;
    desc.envColorMode = 5;
    desc.updateFunc = 4;
    desc.deathFunc = -1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.fadeStart = 20;
    desc.fadeRate = 12;
    extra.unk4[0] = 0;
    extra.unk4[1] = 0;
    extra.unk18[0] = 0;
    extra.unk18[1] = 0;
    extra.unk10 = 3.5f;
    extra.unk14 = 0.2857143f;

    desc.vel.x = (to->x - from->x) * arg5->unk74;
    desc.vel.y = (to->y - from->y) * arg5->unk74;
    desc.vel.z = (to->z - from->z) * arg5->unk74;
    desc.pos = *from;
    desc.pos.x += desc.vel.x * t;
    desc.pos.y += desc.vel.y * t;
    desc.pos.z += desc.vel.z * t;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.timer = func_150ADA20() % 51U + 50;
    desc.alpha = func_150ADA20() % 101U + 100;
    extra.size = desc.width = desc.height = func_150ADA68() * 39.0f + 31.0f;
    extra.unk4[2] = func_150ADA20() % 5U + 4;
    extra.unk4[3] = func_150ADA20() % 5U + 4;
    extra.unk8[0] = (func_150ADA68() * 0.25f + 0.15f) * extra.size;
    extra.unk8[1] = (func_150ADA68() * 0.25f + 0.15f) * extra.size;
    extra.unk18[2] = func_150ADA20() % 5U + 4;
    extra.unk18[3] = func_150ADA20() % 5U + 4;
    extra.unk1C[0] = (func_150ADA68() * 0.1f + 0.08f) * extra.size;
    extra.unk1C[1] = (func_150ADA68() * 0.1f + 0.08f) * extra.size;
    desc.gravity = (func_150ADA68() * 70.0f + 73.0f) * 0.001f;

    particle = func_151303BC(&desc, 2, 0x28);
    if (particle != NULL) {
        memcpy(particle + 0xA8, &extra, sizeof(extra));
    }
}

/* Returns the oscillator's current value and advances its phase by this frame's step. */
f32 func_150EA490(OscillatorObject *obj) {
    Oscillator *oscillator = &obj->oscillator;
    f32 value;

    value = func_151423D8((oscillator->phase >> 16) - 0x40) * oscillator->amplitude + oscillator->base;
    oscillator->phase += oscillator->phaseStep * D_800BE9E4;
    return value;
}

void *func_1513418C(BloodDripDesc *, s32, u8, s32);

/* Attaches an emitter to bone 1 of `owner` for `lifetime` frames (-1: 300 frames). */
void *func_150EA500(Actor *owner, s16 lifetime, s32 arg2, s32 arg3) {
    BloodDripDesc desc;

    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 25.0f;
    desc.rate = 0.26497f;
    if (lifetime == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = lifetime;
    }
    desc.flags = ((lifetime == -1) ? 0 : 4) | 0xA;
    desc.emitter = 7;
    desc.callback = -1;
    desc.unk2D = 6;
    return func_1513418C(&desc, 0, 0xFF, 0);
}

/* Spawns a particle at (x, y, z) drifting against the velocity (vx, vy, vz). */
void func_150EA5CC(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz) {
    ParticleDesc desc;
    u8 *particle;
    ParticleExtra extra;

    desc.pos.z = z;
    extra.unk24 = 1000.0f;
    extra.unk10 = 4.0f;
    extra.unk14 = 0.25f;
    desc.texture = 0x2F;
    desc.renderMode = 0xC01;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.b2 = 0;
    desc.g2 = 0;
    desc.r2 = 0;
    desc.a = 0;
    desc.b = 0;
    desc.g = 0;
    desc.r = 0;
    desc.frameRate = 0;
    desc.frame = 0;
    desc.brightness = 0xFF;
    desc.scaleStart = 0;
    desc.scaleRate = 0.0f;
    desc.flags = 0x3207;
    desc.primColorMode = 5;
    desc.envColorMode = 5;
    desc.updateFunc = 4;
    desc.deathFunc = -1;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.fadeStart = 30;
    desc.fadeRate = 8;
    extra.unk4[0] = 0;
    extra.unk4[1] = 0;
    extra.unk18[0] = 0;
    extra.unk18[1] = 0;
    desc.vel.x = -vx * 0.2f;
    desc.vel.y = 0.0f;
    desc.vel.z = -vz * 0.2f;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.timer = func_150ADA20() % 51U + 106;
    desc.alpha = func_150ADA20() % 101U + 100;
    extra.size = desc.width = desc.height = func_150ADA68() * 62.0f + 41.0f;
    extra.unk4[2] = func_150ADA20() % 5U + 4;
    extra.unk4[3] = func_150ADA20() % 5U + 4;
    extra.unk8[0] = (func_150ADA68() * 0.25f + 0.15f) * extra.size;
    extra.unk8[1] = (func_150ADA68() * 0.25f + 0.15f) * extra.size;
    extra.unk18[2] = func_150ADA20() % 5U + 4;
    extra.unk18[3] = func_150ADA20() % 5U + 4;
    extra.unk1C[0] = (func_150ADA68() * 0.1f + 0.08f) * extra.size;
    extra.unk1C[1] = (func_150ADA68() * 0.1f + 0.08f) * extra.size;
    desc.gravity = (func_150ADA68() * 60.0f + 21.0f) * 0.001f;

    particle = func_151303BC(&desc, 2, 0x28);
    if (particle != NULL) {
        memcpy(particle + 0xA8, &extra, sizeof(extra));
    }
}

/* Callback that ignores its argument and calls func_150E5AE0. */
void func_150EA8E0(s32 arg0) {
    func_150E5AE0();
}

/* Sets behaviorState bits 0 and 1 on D_800DBEF4 entry `index` if its id is 0xE0. */
void func_150EA904(s32 arg0, s32 index) {
    // matching: the no-op cast makes IDO load the array base before scaling index
    WorldObject *entry = (WorldObject *)D_800DBEF4 + index;

    if (entry->id == 0xE0) {
        entry->behaviorState = (entry->behaviorState & 0xFF) | 3; // matching: the no-op mask costs IDO one extra temp register
    }
}

/* While behaviorState bits 0-1 are set: spins and moves the entry, applies gravity (param, y kept in 1/8
 * units in work.words[0]) and fades its alpha out, then marks it inactive. */
void func_150EA944(WorldObject *entry) {
    f32 frames;
    f32 angle;
    s32 timer;

    if ((entry->behaviorState & 3) == 3) {
        frames = D_800BE9E4;

        entry->rotX += entry->spinX * frames;
        angle = entry->rotX;
        if (angle < 0.0f) {
            entry->rotX = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->rotX = angle - 360.0f;
        }

        entry->rotY += entry->spinY * frames;
        angle = entry->rotY;
        if (angle < 0.0f) {
            entry->rotY = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->rotY = angle - 360.0f;
        }

        entry->rotZ += entry->spinZ * frames;
        angle = entry->rotZ;
        if (angle < 0.0f) {
            entry->rotZ = angle + 360.0f;
        } else if (angle >= 360.0f) {
            entry->rotZ = angle - 360.0f;
        }

        timer = entry->alpha;
        entry->moveY -= entry->param.word * D_800BE9E4;
        entry->x += entry->moveX * D_800BE9E4;
        entry->work.words[0] += entry->moveY * D_800BE9E4;
        entry->y = entry->work.words[0] >> 3;
        entry->z += entry->moveZ * D_800BE9E4;

        timer -= D_800BE9E4 * 4;
        if (timer > 0) {
            entry->alpha = timer;
        } else {
            entry->inactive = 1;
        }
    }
}

/* A point on the ground plane (world x and z). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
} Vec2f;

/* Motion state of a ground-hugging flash, kept at +0x18 of the object func_151602C0 returns. */
typedef struct {
    /* 0x00 */ Vec2f position;
    /* 0x08 */ Vec2f velocity;
    /* 0x10 */ f32 height;      /* used when the ground probe finds nothing */
    /* 0x14 */ f32 probeHeight; /* height the ground probe starts from */
    /* 0x18 */ f32 lifetime;
    /* 0x1C */ CollisionHit ground;
} GroundFlash; // size 0x40

/* Spawner state: flashes run between a random point on the start line and one on the end line. */
typedef struct {
    /* 0x00 */ Vec2f startOrigin;
    /* 0x08 */ Vec2f endOrigin;
    /* 0x10 */ Vec2f startExtent;
    /* 0x18 */ Vec2f endExtent;
    /* 0x20 */ f32 height;
    /* 0x24 */ f32 probeHeight;
    /* 0x28 */ f32 rate;        /* flashes per frame */
    /* 0x2C */ f32 rateRange;
    /* 0x30 */ f32 accumulator;
    /* 0x34 */ f32 speed;
    /* 0x38 */ f32 speedRange;
    /* 0x3C */ u8  colour[4];
} FlashSpawner;

typedef struct {
    /* 0x00 */ u8  pad0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ u8  pad2[0xA];
    /* 0x0C */ u8  unkC;
    /* 0x0D */ u8  padD[0x1B];
    /* 0x28 */ FlashSpawner spawner;
} struct_EAB10;

/* The LightEffect func_151602C0 creates, with this file's GroundFlash payload at LightEffect.data;
 * func_150EAE24 is its per-frame update. */
typedef struct {
    /* 0x00 */ u8  pad0[0x14];
    /* 0x14 */ PointLight *light;
    /* 0x18 */ GroundFlash flash;
} FlashObject;

FlashObject *func_151602C0(LightEffectHeader *, Vec3i *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns this frame's share of flashes, each sliding from one line to the other along the ground. */
void func_150EAB10(struct_EAB10 *actor) {
    s32 pad; // matching: unused stack slots that keep the frame layout
    GroundFlash flash;
    LightEffectHeader header;
    s32 padB0; // matching: see pad
    Vec2f end;
    Vec2f start;
    f32 speed;
    f32 length;
    f32 random;
    Vec3i origin;
    FlashObject *obj;
    FlashSpawner *spawner;

    spawner = &actor->spawner;
    spawner->accumulator += (spawner->rate + func_150ADA68() * spawner->rateRange) * D_800BE9A4;
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
                        memcpy(&obj->flash, &flash, sizeof(flash));
                    }
                }
            }
            spawner->accumulator -= 1.0f;
        } while (spawner->accumulator > 1.0f);
    }
}

s32 func_15046C80(f32 *, s32, f32, CollisionHit *);

/* Slides the flash along the ground and keeps its light on the ground; returns 0 once it expires. */
s32 func_150EAE24(FlashObject *obj) {
    GroundFlash *flash;
    f32 probe[3];

    obj->flash.position.x += obj->flash.velocity.x * D_800BE9A4;
    obj->flash.position.z += obj->flash.velocity.z * D_800BE9A4;
    probe[0] = obj->flash.position.x;
    probe[1] = obj->flash.height;
    probe[2] = obj->flash.position.z;
    obj->light->x = obj->flash.position.x;
    obj->light->z = obj->flash.position.z;
    flash = &obj->flash;
    if (func_15046C80(probe, 0, obj->flash.probeHeight, &flash->ground) != 0) {
        obj->light->y = flash->ground.height;
    } else {
        obj->light->y = flash->height;
    }
    flash->lifetime -= D_800BE9A4;
    if (flash->lifetime <= 0.0f) {
        return 0;
    }
    return 1;
}
