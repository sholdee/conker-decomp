#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Brightness of a grounded swarm actor, by bits 5-6 of its surface flags. */
static const u8 D_800A7170[4] = { 0x00, 0x2A, 0x54, 0x7F };

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
    Vec3f diff;
    u32 surfaceFlags;
    s32 surface;

    height = y;
    diff.x = x - camera_global_Cameras[player].eye.x;
    diff.y = height - camera_global_Cameras[player].eye.y;
    diff.z = z - camera_global_Cameras[player].eye.z;
    angle = func_15048FC8(&diff);
    for (i = 0; i < count; i++) {
        actor = func_15167A68(10, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        onGround = flags & 1;
        actorX = (func_150ADA20() & 0x7F) + x - 63;
        actorZ = (func_150ADA20() & 0x7F) + z - 63;
        actor->group = group;
        actor->flags = 3;
        actor->y = height;
        actor->x = actorX;
        actor->z = actorZ;
        actor->speed = func_150ADA20() % 15U + 10;
        actor->speedStep = 1;
        yaw = (func_150ADA20() & 0x1F) + angle - 15;
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
        actor->vertices = D_800DDE80[model].unk8;
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
                surfaceIndex = (surface - (s32)D_800DBE3C) / 12;
                surfaceFlags = D_800DBE5C[surfaceIndex];
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
        actorX = (func_150ADA20() % spread) + spawnX - radius;
        actorZ = (func_150ADA20() % spread) + z - radius;
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
        actor->vertices = D_800DDE80[7].unk8;
        actor->modelIndex = 7;
        samples = func_150A3A70(actorX, actorZ);
        if (samples == 0) {
            func_1516979C(&actor->header);
        } else {
            lowest = D_800D3300[0].height;
            for (j = 1; j < samples; j++) {
                if (D_800D3300[j].height < lowest) {
                    lowest = D_800D3300[j].height;
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
            actor->perchPitchStep = 0;
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
    Vec3f diff;
    u32 variant;
    s32 surface;

    if ((flags & 0x40) && heightRange <= 0) {
        return;
    }
    if (flags & 0x10000) {
        diff.x = x - camera_global_Cameras[player].eye.x;
        diff.y = y - camera_global_Cameras[player].eye.y;
        diff.z = z - camera_global_Cameras[player].eye.z;
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
            actorY += func_150ADA20() % heightRange;
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
            yaw = (func_150ADA20() & 0x1F) + angle - 15;
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
        actor->vertices = D_800DDE80[variant].unk8;
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
                ringWidth = (surface - (s32)D_800DBE3C) / 12;
                onGround = D_800DBE5C[ringWidth];
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

#define D_800D35E4 ((&D_800D35E8)[-1])

/* Common per-frame flight update for a swarm actor: fading, wandering inside its home ring,
 * pitch/yaw/roll easing, wing-frame swapping and movement. Frees the actor when its model
 * is unloaded, it has faded out, or it leaves the view. */
void func_15175958(SwarmActor *actor) {
    s32 pad0; // matching: unused stack slot
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
    Vec3f vec;
    s32 surface;
    SwarmFlight *flight;
    EffectModelSlot *model;
    s32 alpha;
    f32 dx;
    f32 dz;
    u16 sound;

    if (D_800DDE80[actor->modelIndex].resource == NULL || D_800DDF78[actor->modelIndex] == NULL) {
        func_1516972C(&actor->header);
        return;
    }
    alpha = actor->alpha;
    if ((actor->modelIndex == 8 || actor->modelIndex == 9) && level == 6 && D_800C35EA[0] != 0 && D_800C35E8[0] == 0xF) {
        actor->flags &= ~8;
        if (D_800C35EA[0] == 0 || D_800C35E8[0] != 0xF) {
            func_1516972C(&actor->header);
            return;
        }
        /* flag 0x200: spiral down around (-3478, 3858); pitchStep, yawStep and rollStep then
         * hold the orbit angle, its angular speed and the orbit radius */
        if (D_800C35B0[0] >= 60 && !(actor->flags & 0x200)) {
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
            func_1516972C(&actor->header);
            return;
        }
        alpha -= D_800BE9E4 * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        actor->alpha = alpha;
    }
    model = &D_800DDE80[actor->modelIndex];
    flight = &D_8008D0B0[actor->modelIndex];
    D_800DDF68[actor->modelIndex] = 1;
    sound = actor->sound;
    if (sound != 0) {
        if (sound_isPlaying(sound) != 0) {
            func_1000F91C(actor->sound, 0x7D00, 0, 0, 0, actor->x, actor->y, actor->z, 1000, 4000);
        } else {
            actor->sound = 0;
        }
    }
    if (actor->flags & 4) {
        s32 tooClose;

        dist = func_150AD960(actor->x, actor->z, actor->originX, actor->originZ);
        tooClose = ((actor->flags & 0x10) && dist < actor->minRadius) ? TRUE : FALSE;
        if (actor->maxRadius < dist || tooClose) {
            vec.x = actor->originX - actor->x;
            vec.z = actor->z - actor->originZ;
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
            surfaceIndex = (surface - (s32)D_800DBE3C) / 12;
            surfaceFlags = D_800DBE5C[surfaceIndex];
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
            threshold = flight->u.flap.threshold;
            if (actor->vertices == model->unk4 && flight->u.flap.angle < threshold && -threshold < flight->u.flap.angle) {
                actor->vertices = model->unk8;
            }
        } else {
            threshold = flight->u.flap.threshold;
            if (actor->vertices == model->unk8 && flight->u.flap.angle < threshold && -threshold < flight->u.flap.angle) {
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
        if (actor->y < -3100.0f || D_800C35EA[0] == 0 || D_800C35E8[0] != 0xF) {
            func_1516972C(&actor->header);
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
    if (actor->header.type == 0x13 && D_800DDF60[actor->group] == 0) {
        func_10010F88(0x10, 0x7FFF, func_150ADA20() & 0x7F, 0, 0, actor->x, actor->y, actor->z, 500, 4000);
        D_800DDF60[actor->group] = (func_150ADA20() & 0x3F) + 30;
    }
    if ((actor->flags & 2) &&
        func_1510AEE0(gViewMatrices[0], actor->x, actor->y, actor->z, gDefaultNear, gDefaultFar, D_800D35E0, D_800D35E4, NULL, NULL) != 0 &&
        (gMatchState.mode != 1 ||
         func_1510AEE0(gViewMatrices[1], actor->x, actor->y, actor->z, gDefaultNear, gDefaultFar, D_800D35E0, D_800D35E4, NULL, NULL) != 0)) {
        func_1516972C(&actor->header);
    }
}

/* Update for a perching swarm actor (bat): while perched (state 0) it bobs its head and
 * yaws a little, takes off when the player comes within 400 units (state 1), and once
 * 300 units above its perch it flies normally (states 2/3). */
void func_1517685C(SwarmActor *actor) {
    s32 r;

    if (D_800DDF5C[actor->group] != 0) {
        D_800DDF68[actor->modelIndex] = 1;
        switch (actor->state) {
        case 0:
            if (func_150AD960(movement_obj->xpos, movement_obj->zpos, actor->x, actor->z) < 400) {
                actor->state = 1;
                actor->pitchStep = -1 - (s32)(func_150ADA20() & 3);
                actor->pitch = -8.0f;
                return;
            }
            {
                f32 pitch;

                if (actor->perchPitchStep != 0.0f) {
                    actor->pitch = actor->pitch + actor->perchPitchStep;
                    if (actor->perchPitchStep < 0.0f) {
                        actor->yaw = actor->yaw + actor->yawStep;
                    }
                    pitch = actor->pitch;
                    if (pitch > 45.0f) {
                        actor->perchPitchStep = -5.0f;
                    } else if ((actor->perchPitchStep < 0) && (pitch > 25.0f)) {
                        r = func_150ADA20() & 0xFF;
                        if (r < 51) {
                            actor->perchPitchStep = 5.0f;
                        }
                    }
                    if (actor->pitch <= 10.0f) {
                        actor->perchPitchStep = 0.0f;
                        return;
                    }
                } else {
                    r = func_150ADA20() & 0xFF;
                    if (r < 25) {
                        actor->perchPitchStep = 5.0f;
                        if (r < 13) {
                            actor->yawStep = (s32)(func_150ADA20() % 6U) - 3;
                            return;
                        }
                        actor->yawStep = 0.0f;
                    // matching: an unused label stops IDO from merging this return's branch
                    unmerged:
                        return;
                    }
                }
            }
            break;
        case 1:
            if (actor->y >= (300.0f + actor->groundHeight)) {
                actor->state = 2;
                actor->flags &= ~1;
                actor->flags |= 4;
                actor->targetY = (s32)(func_150ADA20() & 0x7F) + (300.0f + actor->groundHeight);
            }
            func_15175958(actor);
            return;
        default:
        case 2:
        case 3:
            if (actor->y < actor->targetY) {
                actor->pitchStep = -1.5f;
            } else if (actor->pitch < 0) {
                actor->pitchStep = 1.5f;
            }
            func_15175958(actor);
            return;
        }
    } else {
        func_1516972C(&actor->header);
    }
}

/* Update for a hovering swarm actor: fades in/out with its group, waits (state 0) until the
 * player is within 200 units, then flies off towards a random height; in state 4 it is freed
 * once it drops below its ground height. */
void func_15176B84(SwarmActor *actor) {
    s32 alpha;

    if ((D_800DDE80[actor->modelIndex].resource == NULL) || (D_800DDF78[actor->modelIndex] == NULL)) {
        func_1516972C(&actor->header);
        return;
    }

    alpha = actor->alpha;
    if (actor->fadingOut == 0) {
        if (D_800DDF5C[actor->group] == 0) {
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
            func_1516972C(&actor->header);
            return;
        }
        alpha -= D_800BE9E4 * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        actor->alpha = alpha;
    }

    D_800DDF68[actor->modelIndex] = 1;

    switch (actor->state) {
        case 0:
            if (func_150AD960(movement_obj[0].xpos, movement_obj[0].zpos, actor->x, actor->z) < 200) {
                actor->state = 2;
                actor->targetY = (f32)(func_150ADA20() % actor->heightRange) + actor->originY;
                actor->vertices = D_800DDE80[actor->modelIndex].unk8;
            } else {
                actor->vertices = D_800DDE80[actor->modelIndex].unk4;
            }
            return;
        case 2:
        case 3:
            break;
        case 4:
            if (actor->y < actor->groundHeight) {
                func_1516972C(&actor->header);
                return;
            }
            break;
    }
    func_15175958(actor);
}

/* Draws a flying swarm actor: builds its scaled model matrix, eases its brightness towards
 * the lighting of the surface below it, and draws its model with that colour and its alpha. */
Gfx *func_15176DF0(Gfx *gdl, SwarmActor *actor, s16 camera) {
    s32 pad; // matching: unused stack slot
    MtxF rot;
    MtxF mtx;
    s32 col[3];
    s32 brightness;
    s32 target;

    if ((D_800DDE80[actor->modelIndex].resource == NULL) || (D_800DDF78[actor->modelIndex] == NULL)) {
        return gdl;
    }
    func_150A8050(mtx, 0.0f, actor->yaw, 0.0f);
    func_150A8050(rot, actor->pitch, 0.0f, actor->roll);
    func_150A7A48(rot, mtx, mtx);
    mtx[3][0] = actor->x;
    mtx[3][1] = actor->y;
    mtx[3][2] = actor->z;
    mtx[0][0] *= actor->scale;
    mtx[0][1] *= actor->scale;
    mtx[0][2] *= actor->scale;
    mtx[1][0] *= actor->scale;
    mtx[1][1] *= actor->scale;
    mtx[1][2] *= actor->scale;
    mtx[2][0] *= actor->scale;
    mtx[2][1] *= actor->scale;
    mtx[2][2] *= actor->scale;
    guMtxF2L(mtx, &actor->mtx[D_800BE9C0]);
    gSPMatrix(gdl++, &actor->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gdl++, 1, actor->vertices);
    gDPPipeSync(gdl++);
    brightness = actor->brightness;
    target = D_800A7170[(actor->surfaceFlags >> 5) & 3];
    if (brightness != target) {
        if (brightness < target) {
            brightness += D_800BE9E4 * 4;
            if (target < brightness) {
                brightness = target;
            }
        } else {
            brightness -= D_800BE9E4 * 4;
            if (brightness < 0) {
                brightness = 0;
            }
        }
        actor->brightness = brightness;
    }
    col[0] = col[1] = col[2] = brightness;
    {
        u8 update;

        update = 0;
        gdl = func_15142FBC(gdl, 0x82CA0, 0x504A50, &update);
        gDPSetEnvColor(gdl++, col[0], col[1], col[2], actor->alpha);
        gSPDisplayList(gdl++, D_800DDE80[actor->modelIndex].displayList);
    }
    return gdl;
}

/* Draws a swarm actor for camera `camera`, unless it is outside that camera's view.
 * Perched actors (state 0) use model 10's resting pose; the rest go through func_15176DF0. */
Gfx *func_151770C8(Gfx *gdl, SwarmActor *actor, s16 camera) {
    MtxF mtx;
    s32 culled;

    culled = func_1510AEE0(gViewMatrices[camera], actor->x, actor->y, actor->z,
                           gDefaultNear, 4000.0f, D_800D35E0, D_800D35E4, NULL, NULL);
    if (culled == 0) {
        switch (actor->state) {
        case 0:
            func_150A8050(mtx, actor->pitch, actor->yaw, actor->roll);
            mtx[3][0] = actor->x;
            mtx[3][1] = actor->y;
            mtx[3][2] = actor->z;
            guMtxF2L(mtx, &actor->mtx[D_800BE9C0]);
            gSPMatrix(gdl++, &actor->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(gdl++, 1, D_800DDE80[10].unk4);
            gSPDisplayList(gdl++, D_800DDE80[10].displayList);
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

/* Flaps model `index`'s wings: updates the flap angle from the flap phase and rotates the inner
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
    vtx = D_800DDE80[index].unk8;
    angle = flight->u.flap.phase * 0.01745329238f;
    flight->u.flap.angle = func_150AD78C(angle) * flight->u.flap.amplitude;
    angle = flight->u.flap.angle * 0.01745329238f;
    sinAngle = func_150AD78C(angle);
    cosAngle = func_150AD780(angle);
    x = flight->u.flap.innerWingLength * cosAngle;
    y = flight->u.flap.innerWingLength * sinAngle;
    vtx[10].v.ob[0] = -x;
    vtx[11].v.ob[0] = -x;
    vtx[17].v.ob[0] = x;
    vtx[18].v.ob[0] = x;
    vtx[17].v.ob[1] = y;
    vtx[18].v.ob[1] = y;
    vtx[10].v.ob[1] = y;
    vtx[11].v.ob[1] = y;
    angle = (flight->u.flap.phase - 30.0f) * 0.01745329238f;
    flight->u.flap.angle = func_150AD78C(angle) * flight->u.flap.amplitude;
    angle = flight->u.flap.angle * 0.01745329238f;
    sinAngle = func_150AD78C(angle);
    cosAngle = func_150AD780(angle);
    x = flight->u.flap.outerWingLength * cosAngle;
    y = flight->u.flap.outerWingLength * sinAngle;
    vtx[14].v.ob[0] = -x;
    vtx[19].v.ob[0] = x;
    vtx[14].v.ob[1] = y;
    vtx[19].v.ob[1] = y;
    flight->u.flap.phase += flight->u.flap.rate;
    if (flight->u.flap.phase >= 360.0f) {
        flight->u.flap.phase -= 360.0f;
    }
}
