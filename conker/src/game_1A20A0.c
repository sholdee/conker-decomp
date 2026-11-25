#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


/* A flying swarm member (bat/bird): spawned by func_15174BF0 / func_15174FA4 / func_15175390,
 * updated by func_15175958 and friends, drawn by func_15176DF0. */
typedef struct SwarmActor {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0xF];
    /* 0x10 */ Mtx mtx[2];           /* double-buffered by D_800BE9C0 */
    /* 0x90 */ u8 group;             /* index into D_800DDF5C / D_800DDF60 */
    /* 0x91 */ u8 state;
    /* 0x92 */ s16 flags;
    /* 0x94 */ Vtx *vertices;        /* bound to segment 1 when drawn */
    /* 0x98 */ f32 x, y, z;
    /* 0xA4 */ s16 speed;
    /* 0xA6 */ s16 speedStep;
    /* 0xA8 */ f32 pitch, yaw, roll;
    /* 0xB4 */ f32 pitchStep, yawStep, rollStep;
    /* 0xC0 */ f32 scale;
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ f32 groundHeight;
    /* 0xCC */ f32 unkCC;
    /* 0xD0 */ s16 originX, originY, originZ;
    /* 0xD6 */ s16 maxRadius, minRadius, heightRange;
    /* 0xDC */ s16 turnTimer, hoverTimer;
    /* 0xE0 */ f32 targetYaw;
    /* 0xE4 */ f32 targetY;
    /* 0xE8 */ u32 surfaceFlags;
    /* 0xEC */ u8 brightness;
    /* 0xED */ u8 modelIndex;        /* index into gSwarmModels / D_8008D0B0 */
    /* 0xEE */ u16 sound;
    /* 0xF0 */ u8 alpha;
    /* 0xF1 */ u8 fadingOut;
} SwarmActor;

/* Per-model data in D_800DDE80 (the shared header only names fields 0x0, 0xC and 0x10). */
typedef struct SwarmModel {
    /* 0x00 */ void *loaded;
    /* 0x04 */ Vtx *unk4;
    /* 0x08 */ Vtx *unk8;
    /* 0x0C */ Gfx *displayList;
    /* 0x10 */ void *unk10;
} SwarmModel;
#define gSwarmModels ((SwarmModel *)D_800DDE80)

typedef struct HeightSample {
    s32 height;
    u8 pad04[0xC];
} HeightSample;
#define gHeightSamples ((HeightSample *)&D_800D3300) /* filled by func_150A3A70 */

// matching: functions.h declares func_150ADA20 as returning u8, but callers here use the full u32
u32 func_150ADA20(void);
void *func_15167A68(s32, s32, s32, s32, u8, u8);
u16 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA);
s32 func_1510F8D8(s32, s32, s32, s32 *);
extern u8 D_800A7170[];

/* Spawns `count` swarm actors of type 10 around (x, y, z), heading roughly away from player `player`. */
void func_15174BF0(s32 count, s32 group, s32 x, s32 y, s32 z, s32 player,
                   f32 minScale, f32 scaleRange, s32 flags) {
    SwarmActor *actor;
    f32 height;
    s32 actorX;
    s32 actorZ;
    s32 angle;
    s32 yaw;
    s32 i;
    s32 onGround;
    s32 model;
    s32 surfaceIndex;
    struct17 diff;
    u32 surfaceFlags;
    s32 surface;

    height = (f32)y;
    diff.unk0 = (f32)x - D_800DBFF0[player].unk2F8;
    diff.unk4 = height - D_800DBFF0[player].unk2FC;
    diff.unk8 = (f32)z - D_800DBFF0[player].unk300;
    angle = func_15048FC8(&diff);
    for (i = 0; i < count; i++) {
        actor = func_15167A68(10, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        onGround = flags & 1;
        actorX = (s32)(func_150ADA20() & 0x7F) + x - 63;
        actorZ = (s32)(func_150ADA20() & 0x7F) + z - 63;
        actor->group = group;
        actor->flags = 3;
        actor->y = height;
        actor->x = actorX;
        actor->z = actorZ;
        actor->speed = func_150ADA20() % 15U + 10;
        actor->speedStep = 1;
        yaw = (s32)(func_150ADA20() & 0x1F) + angle - 15;
        if (yaw < 0) {
            yaw += 360;
        } else if (yaw >= 360) {
            yaw -= 360;
        }
        actor->pitch = (s32)(func_150ADA20() & 0x1F) - 15;
        actor->yaw = yaw;
        actor->roll = 0.0f;
        actor->pitchStep = -1.0f;
        actor->yawStep = ((s32)(func_150ADA20() & 0xFF) - 128) * 0.009999999776f;
        actor->rollStep = 0.0f;
        actor->scale = (s32)(func_150ADA20() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        model = func_150ADA20() & 1;
        actor->vertices = gSwarmModels[model].unk8;
        actor->modelIndex = model;
        if (i == 0) {
            actor->sound = func_10010E78(0, 0xA2, 0x7D00, 0, 0, 0, actorX, y, actorZ, 1000, 4000);
        } else {
            actor->sound = 0;
        }
        actor->surfaceFlags = 0;
        if (onGround) {
            actor->flags |= 0x80;
            actor->groundHeight = func_1510F8D8(actorX, y, actorZ, &surface);
            if (surface != 0) {
                surfaceIndex = (surface - D_800DBE3C) / 12;
                surfaceFlags = ((u32 *)D_800DBE5C)[surfaceIndex];
            } else {
                surfaceFlags = 0;
            }
            actor->surfaceFlags = surfaceFlags;
            actor->brightness = D_800A7170[(surfaceFlags >> 5) & 3];
        } else {
            actor->brightness = 0;
        }
        actor->alpha = 0;
        actor->fadingOut = 0;
    }
}

s32 func_150A3A70(s32, s32);
void func_1510F800(s32);

/* Spawns `count` swarm actors of type 0x13 scattered over a `spread`-wide square around (x, z).
 * With `airborne` clear they start just above the ground; otherwise they start 300+ units up. */
void func_15174FA4(s32 count, s32 group, s32 x, s32 y, s32 z, s32 spread, s32 arg6, s32 airborne,
                   f32 minScale, f32 scaleRange) {
    f32 height;
    s32 spawnX;
    s32 spawnY;
    s32 actorX;
    s32 actorZ;
    s32 radius;
    s32 lowest;
    s32 samples;
    s32 j;
    s32 i;
    SwarmActor *actor;

    // matching: IDO assigns x and y their callee-saved registers in copy order
    spawnX = x;
    spawnY = y;
    if (airborne == 0) func_1510F800(0);
    for (i = 0; i < count; i++) {
        actor = func_15167A68(0x13, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        radius = spread >> 1;
        height = spawnY;
        actorX = (func_150ADA20() % (u32)spread) + spawnX - radius;
        actorZ = (func_150ADA20() % (u32)spread) + z - radius;
        actor->group = group;
        actor->x = actorX;
        actor->y = height;
        actor->z = actorZ;
        actor->speed = func_150ADA20() % 3U + 4;
        actor->speedStep = 0;
        actor->pitch = (s32)(func_150ADA20() & 0x1F) - 15;
        actor->yaw = func_150ADA20() % 360U;
        actor->roll = (s32)(func_150ADA20() & 0x1F) - 15;
        actor->pitchStep = 0.0f;
        actor->rollStep = 0.0f;
        actor->yawStep = 3.0f;
        actor->scale = (s32)(func_150ADA20() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        actor->vertices = gSwarmModels[7].unk8;
        actor->modelIndex = 7;
        samples = func_150A3A70(actorX, actorZ);
        if (samples == 0) {
            func_1516979C((struct102 *)actor);
        } else {
            lowest = gHeightSamples[0].height;
            for (j = 1; j < samples; j++) {
                if (gHeightSamples[j].height < lowest) {
                    lowest = gHeightSamples[j].height;
                }
            }
            lowest >>= 8;
            actor->groundHeight = lowest;
            if (airborne == 0) {
                actor->state = 0;
                actor->y = lowest + 10;
                actor->flags = 1;
            } else {
                actor->state = 2;
                actor->flags = 4;
                actor->targetY = (s32)(func_150ADA20() & 0x7F) + lowest + 300;
                actor->y = (s32)(func_150ADA20() & 0x7F) + lowest + 300;
            }
            actor->unkCC = 0;
            actor->originX = spawnX;
            actor->originZ = z;
            actor->maxRadius = radius;
            actor->targetYaw = 0;
            actor->turnTimer = 0;
            actor->surfaceFlags = 0;
            actor->brightness = 0;
            actor->alpha = 0;
            actor->fadingOut = 0;
        }
    }
}

/* Spawns `count` swarm actors in a ring between minRadius and maxRadius around (x, z).
 * flags becomes the actors' flags; 0x10000 aims them away from player `player`,
 * 0x20000 gives them a random initial pitch. */
void func_15175390(s32 count, s32 group, s32 x, s32 y, s32 z, s32 maxRadius,
                   s32 player, s32 heightRange, s32 minRadius, s32 flags, s32 baseType, u32 typeCount,
                   s32 baseModel, s32 modelMask, s32 state, f32 minScale, f32 scaleRange, s32 spawnFlags) {
    SwarmActor *actor;
    s32 actorX;
    s32 actorY;
    s32 actorZ;
    s32 i;
    s32 yaw;
    s32 angle;
    s32 ringWidth;
    s32 onGround;
    struct17 diff;
    u32 variant;
    s32 surface;

    if ((flags & 0x40) && heightRange <= 0) {
        return;
    }
    if (flags & 0x10000) {
        diff.unk0 = (f32)x - D_800DBFF0[player].unk2F8;
        diff.unk4 = (f32)y - D_800DBFF0[player].unk2FC;
        diff.unk8 = (f32)z - D_800DBFF0[player].unk300;
        angle = func_15048FC8(&diff);
    }
    for (i = 0; i < count; i++) {
        if (typeCount != 0) {
            variant = func_150ADA20() % typeCount;
        } else {
            variant = 0;
        }
        actor = func_15167A68(baseType + variant, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        ringWidth = maxRadius - minRadius;
        onGround = spawnFlags & 1;
        actorX = (s32)func_150ADA20() % ringWidth;
        actorX = (actorX < 0) ? actorX - minRadius : actorX + minRadius;
        actorX += x;
        actorY = y;
        actorZ = (s32)func_150ADA20() % ringWidth;
        actorZ = (actorZ < 0) ? actorZ - minRadius : actorZ + minRadius;
        actorZ += z;
        if (state == 0 || onGround) {
            actor->groundHeight = func_1510F8D8(actorX, y, actorZ, &surface);
        }
        if (state == 0) {
            actorY = actor->groundHeight;
        } else if (heightRange != 0) {
            actorY += func_150ADA20() % (u32)heightRange;
        }
        actor->group = group;
        actor->flags = flags;
        actor->state = state;
        actor->x = actorX;
        actor->y = actorY;
        actor->z = actorZ;
        actor->speed = func_150ADA20() % 15U + 10;
        actor->speedStep = 0;
        if (flags & 0x10000) {
            yaw = (s32)(func_150ADA20() & 0x1F) + angle - 15;
            if (yaw < 0) {
                yaw += 360;
            } else if (yaw >= 360) {
                yaw -= 360;
            }
        } else {
            yaw = func_150ADA20() % 360U;
        }
        if (flags & 0x20000) {
            actor->pitch = (s32)(func_150ADA20() & 0x1F) - 15;
            actor->pitchStep = -1.0f;
        } else {
            actor->pitch = 0.0f;
            actor->pitchStep = 0.0f;
        }
        actor->roll = 0.0f;
        actor->yaw = yaw;
        actor->yawStep = ((s32)(func_150ADA20() & 0xFF) - 128) * 0.009999999776f;
        actor->rollStep = 0.0f;
        actor->scale = (s32)(func_150ADA20() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        if (modelMask != -1) {
            if (modelMask != 0) {
                variant = func_150ADA20() & modelMask;
            } else {
                variant = 0;
            }
        }
        variant += baseModel;
        actor->vertices = gSwarmModels[variant].unk8;
        actor->modelIndex = variant;
        actor->originX = x;
        actor->originY = y;
        actor->originZ = z;
        actor->maxRadius = maxRadius;
        actor->minRadius = minRadius;
        actor->heightRange = heightRange;
        actor->turnTimer = 0;
        actor->hoverTimer = 0;
        actor->surfaceFlags = 0;
        actor->targetYaw = 0;
        if (i == 0) {
            if (baseType == 10) {
                actor->sound = func_10010E78(0, 0xA2, 0x7D00, 0, 0, 0, actorX, actorY, actorZ, 1000, 4000);
            }
        } else {
            actor->sound = 0;
        }
        if (onGround) {
            actor->flags |= 0x80;
            // matching: reusing ringWidth/onGround here keeps the original stack frame size
            if (surface != 0) {
                ringWidth = (surface - D_800DBE3C) / 12;
                onGround = ((u32 *)D_800DBE5C)[ringWidth];
            } else {
                onGround = 0;
            }
            actor->surfaceFlags = onGround;
            actor->brightness = D_800A7170[((u32)onGround >> 5) & 3];
        } else {
            actor->brightness = 0;
        }
        actor->alpha = 0;
        actor->fadingOut = 0;
    }
}

/* Per-model flight parameters, indexed like gSwarmModels. */
typedef struct SwarmFlight {
    /* 0x00 */ u8 maxSpeed;
    /* 0x01 */ u8 minSpeed;
    /* 0x02 */ u8 wanderTime;
    /* 0x03 */ u8 turnTime;
    /* 0x04 */ f32 maxRoll;
    /* 0x08 */ f32 maxPitch;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 maxYawStep;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ f32 flapAngle;        /* flapAmplitude * sin(flapPhase) */
    /* 0x1C */ f32 flapRate;
    /* 0x20 */ f32 flapThreshold;
    /* 0x24 */ f32 flapPhase;
    /* 0x28 */ f32 flapAmplitude;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ f32 innerWingLength;
    /* 0x34 */ f32 outerWingLength;
} SwarmFlight;

extern SwarmFlight D_8008D0B0[];
extern s32 D_800C35B0;
extern u8 D_800C35E8;
extern f32 D_800D9C10[][4][4]; /* one view matrix per camera */
extern f32 D_800D35E0;
extern f32 D_800D35E8; /* 0x800D35E4 == (&D_800D35E8)[-1]; it has no symbol of its own */
#define D_800D35E4 ((&D_800D35E8)[-1])
s32 func_150490A8(struct17 *arg0);
s32 func_150AD960(s32, s32, s32, s32);
s32 func_1510AEE0(f32 mtx[4][4], f32, f32, f32, f32, f32, f32, f32, f32 *, f32 *);

/* Common per-frame flight update for a swarm actor: fading, wandering inside its home ring,
 * pitch/yaw/roll easing, wing-frame swapping and movement. Frees the actor when its model
 * is unloaded, it has faded out, or it leaves the view. */
void func_15175958(SwarmActor *actor) {
    s32 pad0;
    f32 dir[3];
    f32 rad[3];
    f32 sinPitch;
    f32 sinYaw;
    f32 cosPitch;
    f32 cosYaw;
    s32 surfaceIndex;
    f32 step;
    f32 threshold;
    s32 surfaceFlags;
    s32 angle;
    s32 dist;
    struct17 vec;
    s32 surface;
    SwarmFlight *flight;
    SwarmModel *model;
    s32 alpha;
    f32 dx;
    f32 dz;
    u16 sound;

    if (gSwarmModels[actor->modelIndex].loaded == NULL || D_800DDF78[actor->modelIndex] == 0) {
        func_1516972C((struct102 *)actor);
        return;
    }
    alpha = actor->alpha;
    if ((actor->modelIndex == 8 || actor->modelIndex == 9) && D_800BE9F0 == 6 && D_800C35EA != 0 && D_800C35E8 == 0xF) {
        actor->flags &= ~8;
        if (D_800C35EA == 0 || D_800C35E8 != 0xF) {
            func_1516972C((struct102 *)actor);
            return;
        }
        if (D_800C35B0 >= 60 && !(actor->flags & 0x200)) {
            actor->flags = (actor->flags & ~0x4C) | 0x200;
            dx = actor->x - -3478.0f;
            dz = actor->z - 3858.0f;
            actor->pitchStep = 1.570796371f - func_150484A0(dx, dz);
            actor->yawStep = 0.01150000002f;
            actor->rollStep = sqrtf(dx * dx + dz * dz);
        }
    }
    if (actor->fadingOut == 0) {
        if ((actor->flags & 8) && D_800DDF5C[actor->group] == 0) {
            actor->fadingOut = 1;
        } else if (alpha != 255) {
            alpha += D_800BE9E4 * 4;
            if (alpha >= 256) {
                alpha = 255;
            }
            actor->alpha = alpha;
        }
    } else {
        if (alpha == 0) {
            func_1516972C((struct102 *)actor);
            return;
        }
        alpha -= D_800BE9E4 * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        actor->alpha = alpha;
    }
    // matching: index D_800DDE80 before the cast; &gSwarmModels[i] schedules differently
    model = (SwarmModel *)&D_800DDE80[actor->modelIndex];
    flight = &D_8008D0B0[actor->modelIndex];
    D_800DDF68[actor->modelIndex] = 1;
    sound = actor->sound;
    if (sound != 0) {
        if (func_1000F3D0(sound) != 0) {
            func_1000F91C(actor->sound, 0x7D00, 0, 0, 0, actor->x, actor->y, actor->z, 1000, 4000);
        } else {
            actor->sound = 0;
        }
    }
    if (actor->flags & 4) {
        s32 tooClose;

        dist = func_150AD960((s32)actor->x, (s32)actor->z, actor->originX, actor->originZ);
        tooClose = ((actor->flags & 0x10) && dist < actor->minRadius) ? TRUE : FALSE;
        if (actor->maxRadius < dist || tooClose) {
            vec.unk0 = actor->originX - actor->x;
            vec.unk8 = actor->z - actor->originZ;
            angle = func_150490A8(&vec);
            if (tooClose) {
                angle += 0x80;
            }
            if (angle >= 0x100) {
                angle -= 0x100;
            }
            actor->targetYaw = angle * 1.40625f;
            actor->turnTimer = flight->turnTime;
            actor->state = 3;
        }
        if (actor->state == 3) {
            step = D_800BE9E4 * 0.5f;
            if (actor->targetYaw < actor->yaw) {
                step = -step;
            }
            if (fabsf(actor->targetYaw - actor->yaw) >= 180.0f) {
                step = -step;
            }
            if ((step > 0.0f && actor->yawStep < 0.0f) || (step < 0.0f && actor->yawStep > 0.0f)) {
                actor->state = 2;
            } else {
                actor->yawStep += step;
            }
        }
        if (actor->state == 2) {
            if (actor->yawStep != 0.0f) {
                if (actor->yawStep > 0.0f) {
                    actor->yawStep -= D_800BE9E4 * 0.5f;
                    if (actor->yawStep < 0.0f) {
                        actor->yawStep = 0.0f;
                    }
                } else {
                    actor->yawStep += D_800BE9E4 * 0.5f;
                    if (actor->yawStep > 0.0f) {
                        actor->yawStep = 0.0f;
                    }
                }
            }
            actor->turnTimer -= D_800BE9E4;
            if (actor->turnTimer <= 0) {
                actor->targetYaw = func_150ADA20() % 360;
                actor->turnTimer = flight->wanderTime;
                actor->state = 3;
            }
        }
        if (flight->maxYawStep < fabsf(actor->yawStep)) {
            if (actor->yawStep > 0.0f) {
                actor->yawStep = flight->maxYawStep;
            } else {
                actor->yawStep = -flight->maxYawStep;
            }
        }
    }
    if (actor->flags & 0x80) {
        actor->groundHeight = func_1510F8D8(actor->x, actor->y, actor->z, &surface);
        if (surface != 0) {
            surfaceIndex = (surface - D_800DBE3C) / 12;
            surfaceFlags = ((u32 *)D_800DBE5C)[surfaceIndex];
        } else {
            surfaceFlags = 0;
        }
        actor->surfaceFlags = surfaceFlags;
    }
    if (actor->flags & 0x40) {
        if (actor->hoverTimer >= 0) {
            actor->hoverTimer -= D_800BE9E4;
            if (actor->hoverTimer <= 0) {
                actor->hoverTimer = -1;
                actor->targetY = (f32)(func_150ADA20() % actor->heightRange) + actor->originY;
                if (actor->y < actor->targetY) {
                    actor->pitchStep = -1.0f;
                } else {
                    actor->pitchStep = 1.0f;
                }
            } else if (actor->pitch > 0.0f) {
                actor->pitchStep = -1.0f;
            } else if (actor->pitch < 0.0f) {
                actor->pitchStep = 1.0f;
            }
        } else if ((actor->pitchStep > 0.0f && actor->y < actor->targetY) ||
                   (actor->pitchStep < 0.0f && actor->targetY < actor->y)) {
            actor->hoverTimer = 120;
            actor->pitchStep = -actor->pitchStep;
        }
    }
    if (!(actor->flags & 0x200)) {
        actor->pitch += actor->pitchStep;
        actor->pitch = (actor->pitch < -flight->maxPitch) ? -flight->maxPitch : ((flight->maxPitch < actor->pitch) ? flight->maxPitch : actor->pitch);
        if (0.200000003f < fabsf(actor->yawStep)) {
            actor->yaw += actor->yawStep;
            if (actor->yaw < 0.0f) {
                actor->yaw += 360.0f;
            } else if (actor->yaw >= 360.0f) {
                actor->yaw -= 360.0f;
            }
            if (!(actor->flags & 0x20)) {
                actor->rollStep = -actor->yawStep;
                actor->roll += actor->rollStep;
                actor->roll = (actor->roll < -flight->maxRoll) ? -flight->maxRoll : ((flight->maxRoll < actor->roll) ? flight->maxRoll : actor->roll);
            }
        } else {
            if (actor->roll > 0) {
                actor->rollStep = -1.0f;
            } else if (actor->roll < 0) {
                actor->rollStep = 1.0f;
            }
            actor->roll += actor->rollStep;
            if (fabsf(actor->roll) < 0.200000003f) {
                actor->roll = 0;
            }
        }
        actor->speed += actor->speedStep;
        actor->speed = (actor->speed < flight->minSpeed) ? flight->minSpeed : ((flight->maxSpeed < actor->speed) ? flight->maxSpeed : actor->speed);
    }
    if (!(actor->flags & 1)) {
        if (actor->pitch >= 0) {
            threshold = flight->flapThreshold;
            if (actor->vertices == model->unk4 && flight->flapAngle < threshold && -threshold < flight->flapAngle) {
                actor->vertices = model->unk8;
            }
        } else {
            threshold = flight->flapThreshold;
            if (actor->vertices == model->unk8 && flight->flapAngle < threshold && -threshold < flight->flapAngle) {
                actor->vertices = model->unk4;
            }
        }
    } else {
        // matching: an empty else block keeps the original branch layout
    }
    if (actor->flags & 0x200) {
        f32 speed;
        speed = D_800BE9E4;
        actor->pitchStep += actor->yawStep * speed;
        actor->yawStep += 0.005189999938f;
        if (0.1499999911f < actor->yawStep) {
            actor->yawStep = 0.1499999911f;
        }
        actor->rollStep -= 3.200000048f * speed;
        if (actor->rollStep < 60.0f) {
            actor->rollStep = 60.0f;
        }
        sinPitch = func_150AD78C(actor->pitchStep);
        cosPitch = func_150AD780(actor->pitchStep);
        actor->x = actor->rollStep * cosPitch + -3478.0f;
        actor->z = actor->rollStep * sinPitch + 3858.0f;
        actor->y -= 100.0f / (actor->rollStep - 60.0f + 4.0f);
        if (actor->y < -3100.0f || D_800C35EA == 0 || D_800C35E8 != 0xF) {
            func_1516972C((struct102 *)actor);
            return;
        }
    } else {
        s32 speed;
        rad[0] = actor->pitch * 0.01745329238f;
        rad[1] = actor->yaw * 0.01745329238f;
        rad[2] = actor->roll * 0.01745329238f;
        sinPitch = func_150AD78C(rad[0]);
        sinYaw = func_150AD78C(rad[1]);
        cosPitch = func_150AD780(rad[0]);
        cosYaw = func_150AD780(rad[1]);
        dir[0] = cosPitch * sinYaw;
        dir[1] = -sinPitch;
        dir[2] = cosPitch * cosYaw;
        speed = actor->speed * D_800BE9E4;
        actor->x += dir[0] * speed;
        actor->y += dir[1] * speed;
        actor->z += dir[2] * speed;
    }
    if (actor->type == 0x13 && D_800DDF60[actor->group] == 0) {
        func_10010F88(0x10, 0x7FFF, func_150ADA20() & 0x7F, 0, 0, actor->x, actor->y, actor->z, 500, 4000);
        D_800DDF60[actor->group] = (func_150ADA20() & 0x3F) + 30;
    }
    if ((actor->flags & 2) &&
        func_1510AEE0(D_800D9C10[0], actor->x, actor->y, actor->z, D_800D9B20, D_800D9B1C, D_800D35E0, D_800D35E4, NULL, NULL) != 0 &&
        (D_800D2138 != 1 ||
         func_1510AEE0(D_800D9C10[1], actor->x, actor->y, actor->z, D_800D9B20, D_800D9B1C, D_800D35E0, D_800D35E4, NULL, NULL) != 0)) {
        func_1516972C((struct102 *)actor);
    }
}

typedef struct {
    u8 pad0[0x90];
    u8 unk90;
    u8 unk91;
    s16 unk92;
    u8 pad94[4];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    u8 padA4[4];
    f32 unkA8;
    f32 unkAC;
    u8 padB0[4];
    f32 unkB4;
    f32 unkB8;
    u8 padBC[0xC];
    f32 unkC8;
    f32 unkCC;
    u8 padD0[0x14];
    f32 unkE4;
    u8 padE8[5];
    u8 unkED;
} struct1517685C;

s32 func_150AD960(s32, s32, s32, s32);
void func_15175958();
u32 func_150ADA20(void);

void func_1517685C(struct1517685C *arg0) {
    s32 r;
    f32 cc;
    s32 t;

    if (D_800DDF5C[arg0->unk90] != 0) {
        D_800DDF68[arg0->unkED] = 1;
        switch (arg0->unk91) {
        case 0:
            if (func_150AD960((s32)D_800CC2D0->x_position, (s32)D_800CC2D0->z_position, (s32)arg0->unk98, (s32)arg0->unkA0) < 400) {
                arg0->unk91 = 1;
                arg0->unkB4 = -1 - (s32)(func_150ADA20() & 3); arg0->unkA8 = -8.0f;
                return;
            }
            {
                f32 a8;

                if (arg0->unkCC != 0.0f) {
                    arg0->unkA8 = arg0->unkA8 + arg0->unkCC;
                    if (arg0->unkCC < 0.0f) {
                        arg0->unkAC = arg0->unkAC + arg0->unkB8;
                    }
                    a8 = arg0->unkA8;
                    if (a8 > 45.0f) {
                        arg0->unkCC = -5.0f;
                    } else if ((arg0->unkCC < 0) && (a8 > 25.0f)) {
                        r = func_150ADA20() & 0xFF;
                        if (r < 0x33) {
                            arg0->unkCC = 5.0f;
                        }
                    }
                    if (arg0->unkA8 <= 10.0f) {
                        arg0->unkCC = 0.0f;
                        return;
                    }
                } else {
                    r = func_150ADA20() & 0xFF;
                    if (r < 0x19) {
                        arg0->unkCC = 5.0f;
                        if (r < 0xD) {
                            arg0->unkB8 = (s32)(func_150ADA20() % 6U) - 3;
                            return;
                        }
                        arg0->unkB8 = 0.0f;
                    lbl1:
                        return;
                    }
                }
            }
            break;
        case 1:
        lbl2:
            if (arg0->unk9C >= (300.0f + arg0->unkC8)) {
                arg0->unk91 = 2;
                arg0->unk92 &= ~1;
                arg0->unk92 |= 4;
                arg0->unkE4 = (s32)(func_150ADA20() & 0x7F) + (300.0f + arg0->unkC8);
            }
            func_15175958(arg0);
            return;
        default:
        case 2:
        case 3:
            if (arg0->unk9C < arg0->unkE4) {
                arg0->unkB4 = -1.5f;
            } else if (arg0->unkA8 < 0) {
                arg0->unkB4 = 1.5f;
            }
            func_15175958(arg0);
            return;
        }
    } else {
        func_1516972C((struct102 *)arg0);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct15176B84;

typedef struct {
    u8  pad0[0x90];
    u8  unk90;
    u8  unk91;
    u8  pad92[0x2];
    s32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    u8  padA4[0x24];
    f32 unkC8;
    u8  padCC[0x6];
    s16 unkD2;
    u8  padD4[0x6];
    s16 unkDA;
    u8  padDC[0x8];
    f32 unkE4;
    u8  padE8[0x5];
    u8  unkED;
    u8  padEE[0x2];
    u8  unkF0;
    u8  unkF1;
} Obj15176B84;

s32 func_150AD960(s32, s32, s32, s32);
void func_15175958();

void func_15176B84(Obj15176B84 *obj) {
    s32 fade;

    if ((((Struct15176B84 *)D_800DDE80)[obj->unkED].unk0 == 0) || (D_800DDF78[obj->unkED] == 0)) {
        func_1516972C((struct102 *)obj);
        return;
    }

    fade = obj->unkF0;
    if (obj->unkF1 == 0) {
        if (D_800DDF5C[obj->unk90] == 0) {
            obj->unkF1 = 1;
        } else if (fade != 0xFF) {
            fade += D_800BE9E4 * 4;
            if (fade >= 0x100) {
                fade = 0xFF;
            }
            obj->unkF0 = fade;
        }
    } else {
        if (fade == 0) {
            func_1516972C((struct102 *)obj);
            return;
        }
        fade -= D_800BE9E4 * 4;
        if (fade < 0) {
            fade = 0;
        }
        obj->unkF0 = fade;
    }

    D_800DDF68[obj->unkED] = 1;

    switch (obj->unk91) {
        case 0:
            if (func_150AD960((s32)D_800CC2D0[0].x_position, (s32)D_800CC2D0[0].z_position, (s32)obj->unk98,
                              (s32)obj->unkA0) < 0xC8) {
                obj->unk91 = 2;
                obj->unkE4 = (f32)((u32)func_150ADA20() % obj->unkDA) + obj->unkD2;
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk8;
            } else {
                obj->unk94 = ((Struct15176B84 *)D_800DDE80)[obj->unkED].unk4;
            }
            return;
        case 2:
        case 3:
            break;
        case 4:
            if (obj->unk9C < obj->unkC8) {
                func_1516972C((struct102 *)obj);
                return;
            }
            break;
    }
    func_15175958(obj);
}

void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

void *func_15176DF0(Gfx *gdl, u8 *arg1, s16 arg2) {
    s32 pad;
    f32 rot[4][4];
    f32 mtx[4][4];
    s32 col[3];
    s32 fade;
    s32 target;

    if ((D_800DDE80[arg1[0xED]].field0 == NULL) || (D_800DDF78[arg1[0xED]] == 0)) {
        return gdl;
    }
    func_150A8050(mtx, 0.0f, *(f32 *)(arg1 + 0xAC), 0.0f);
    func_150A8050(rot, *(f32 *)(arg1 + 0xA8), 0.0f, *(f32 *)(arg1 + 0xB0));
    func_150A7A48(rot, mtx, mtx);
    mtx[3][0] = *(f32 *)(arg1 + 0x98);
    mtx[3][1] = *(f32 *)(arg1 + 0x9C);
    mtx[3][2] = *(f32 *)(arg1 + 0xA0);
    mtx[0][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[0][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[0][2] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[1][2] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][0] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][1] *= *(f32 *)(arg1 + 0xC0);
    mtx[2][2] *= *(f32 *)(arg1 + 0xC0);
    guMtxF2L(mtx, (Mtx *)((arg1 + 0x10) + (D_800BE9C0 << 6)));
    gSPMatrix(gdl++, (Mtx *)((arg1 + 0x10) + (D_800BE9C0 << 6)), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gdl++, 1, *(u32 *)(arg1 + 0x94));
    gDPPipeSync(gdl++);
    fade = arg1[0xEC];
    target = D_800A7170[(*(u32 *)(arg1 + 0xE8) >> 5) & 3];
    if (fade != target) {
        if (fade < target) {
            fade += D_800BE9E4 * 4;
            if (target < fade) {
                fade = target;
            }
        } else {
            fade -= D_800BE9E4 * 4;
            if (fade < 0) {
                fade = 0;
            }
        }
        arg1[0xEC] = fade;
    }
    col[0] = col[1] = col[2] = fade;
    {
    u8 update;
    update = 0;
    gdl = func_15142FBC(gdl, 0x82CA0, 0x504A50, &update);
    gdl->words.w0 = 0xFB000000; gdl->words.w1 = ((col[0] & 0xFF) << 24) | ((col[1] & 0xFF) << 16) | (arg1[0xF0] & 0xFF) | ((col[2] & 0xFF) << 8); gdl++;
    gSPDisplayList(gdl++, D_800DDE80[arg1[0xED]].fieldC);
    }
    return gdl;
}

/* Draws a swarm actor for camera `camera`, unless it is outside that camera's view.
 * Perched actors (state 0) use model 10's resting pose; the rest go through func_15176DF0. */
Gfx *func_151770C8(Gfx *gdl, SwarmActor *actor, s16 camera) {
    f32 mtx[4][4];
    s32 culled;

    culled = func_1510AEE0(D_800D9C10[camera], actor->x, actor->y, actor->z,
                           D_800D9B20, 4000.0f, D_800D35E0, D_800D35E4, NULL, NULL);
    if (culled == 0) {
        switch (actor->state) {
        case 0:
            func_150A8050(mtx, actor->pitch, actor->yaw, actor->roll);
            mtx[3][0] = actor->x;
            mtx[3][1] = actor->y;
            mtx[3][2] = actor->z;
            guMtxF2L(mtx, &actor->mtx[D_800BE9C0]);
            gSPMatrix(gdl++, &actor->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(gdl++, 1, gSwarmModels[10].unk4);
            gSPDisplayList(gdl++, gSwarmModels[10].displayList);
            break;
        case 1:
        case 2:
        case 3:
        default:
            gdl = func_15176DF0(gdl, actor, camera);
            break;
        }
    }
    return gdl;
}

/* Flaps model `index`'s wings: updates flapAngle from the flap phase and rotates the inner
 * wing vertices by it and the outer ones by the angle 30 degrees of phase earlier. */
void func_1517725C(u8 index) {
    f32 angle;
    f32 sinAngle;
    s16 x;
    s16 y;
    SwarmFlight *flight;
    Vtx *vtx;
    f32 cosAngle;

    flight = &D_8008D0B0[index];
    vtx = gSwarmModels[index].unk8;
    angle = flight->flapPhase * 0.01745329238f;
    flight->flapAngle = func_150AD78C(angle) * flight->flapAmplitude;
    angle = flight->flapAngle * 0.01745329238f;
    sinAngle = func_150AD78C(angle);
    cosAngle = func_150AD780(angle);
    x = flight->innerWingLength * cosAngle;
    y = flight->innerWingLength * sinAngle;
    vtx[10].v.ob[0] = -x;
    vtx[11].v.ob[0] = -x;
    vtx[17].v.ob[0] = x;
    vtx[18].v.ob[0] = x;
    vtx[17].v.ob[1] = y;
    vtx[18].v.ob[1] = y;
    vtx[10].v.ob[1] = y;
    vtx[11].v.ob[1] = y;
    angle = (flight->flapPhase - 30.0f) * 0.01745329238f;
    flight->flapAngle = func_150AD78C(angle) * flight->flapAmplitude;
    angle = flight->flapAngle * 0.01745329238f;
    sinAngle = func_150AD78C(angle);
    cosAngle = func_150AD780(angle);
    x = flight->outerWingLength * cosAngle;
    y = flight->outerWingLength * sinAngle;
    vtx[14].v.ob[0] = -x;
    vtx[19].v.ob[0] = x;
    vtx[14].v.ob[1] = y;
    vtx[19].v.ob[1] = y;
    flight->flapPhase += flight->flapRate;
    if (flight->flapPhase >= 360.0f) {
        flight->flapPhase -= 360.0f;
    }
}
