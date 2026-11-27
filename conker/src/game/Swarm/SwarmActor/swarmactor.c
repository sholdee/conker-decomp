#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/player.h"
#include "conker/prop.h"
#include "conker/sound.h"
#include "conker/swarm.h"
#include "conker/view.h"

/* Brightness of a grounded swarm actor, by bits 5-6 of its surface flags. */
static const u8 gSwarmGroundBrightness[4] = { 0x00, 0x2A, 0x54, 0x7F };

/* Spawns `count` swarm actors of type 10 around (x, y, z), heading roughly away from player `player`. */
void swarm_spawnAwayFromPlayer(s32 count, s32 group, s32 x, s32 y, s32 z, s32 player,
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
    angle = math_vec3Heading(&diff);
    for (i = 0; i < count; i++) {
        actor = effect_newObject(10, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        onGround = flags & 1;
        actorX = (math_random() & 0x7F) + x - 63;
        actorZ = (math_random() & 0x7F) + z - 63;
        actor->group = group;
        actor->flags = 3;
        actor->y = height;
        actor->x = actorX;
        actor->z = actorZ;
        actor->speed = math_random() % 15U + 10;
        actor->speedStep = 1;
        yaw = (math_random() & 0x1F) + angle - 15;
        if (yaw < 0) {
            yaw += 360;
        } else if (yaw >= 360) {
            yaw -= 360;
        }
        actor->pitch = (s32)(math_random() & 0x1F) - 15;
        actor->yaw = yaw;
        actor->roll = 0.0f;
        actor->pitchStep = -1.0f;
        actor->yawStep = ((s32)(math_random() & 0xFF) - 128) * 0.009999999776f;
        actor->rollStep = 0.0f;
        actor->scale = (s32)(math_random() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        model = math_random() & 1;
        actor->vertices = gEffectModelSlots[model].unk8;
        actor->modelIndex = model;
        if (i == 0) {
            actor->sound = sound_startVoiceAt(0, 0xA2, 0x7D00, 0, 0, 0, actorX, y, actorZ, 1000, 4000);
        } else {
            actor->sound = 0;
        }
        actor->surfaceFlags = 0;
        if (onGround) {
            actor->flags |= 0x80;
            actor->groundHeight = collision_findMeshFloorNear(actorX, y, actorZ, &surface);
            if (surface != 0) {
                surfaceIndex = (surface - (s32)gCurMapTris) / 12;
                surfaceFlags = gMeshTriMaterials[surfaceIndex];
            } else {
                surfaceFlags = 0;
            }
            actor->surfaceFlags = surfaceFlags;
            actor->brightness = gSwarmGroundBrightness[(surfaceFlags >> 5) & 3];
        } else {
            actor->brightness = 0;
        }
        actor->alpha = 0;
        actor->fadingOut = 0;
    }
}

/* Spawns `count` swarm actors of type 0x13 scattered over a `spread`-wide square around (x, z).
 * With `airborne` clear they start just above the ground; otherwise they start 300+ units up. */
void swarm_spawnScattered(s32 count, s32 group, s32 x, s32 y, s32 z, s32 spread, s32 player, s32 airborne,
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
    if (airborne == 0) collision_selectMap(0);
    for (i = 0; i < count; i++) {
        actor = effect_newObject(0x13, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        radius = spread >> 1;
        height = spawnY;
        actorX = (math_random() % spread) + spawnX - radius;
        actorZ = (math_random() % spread) + z - radius;
        actor->group = group;
        actor->x = actorX;
        actor->y = height;
        actor->z = actorZ;
        actor->speed = math_random() % 3U + 4;
        actor->speedStep = 0;
        actor->pitch = (s32)(math_random() & 0x1F) - 15;
        actor->yaw = math_random() % 360U;
        actor->roll = (s32)(math_random() & 0x1F) - 15;
        actor->pitchStep = 0.0f;
        actor->rollStep = 0.0f;
        actor->yawStep = 3.0f;
        actor->scale = (s32)(math_random() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        actor->vertices = gEffectModelSlots[7].unk8;
        actor->modelIndex = 7;
        samples = collision_gatherGroundTris(actorX, actorZ);
        if (samples == 0) {
            effect_freeObject(&actor->header);
        } else {
            lowest = gGroundCandidates[0].height;
            for (j = 1; j < samples; j++) {
                if (gGroundCandidates[j].height < lowest) {
                    lowest = gGroundCandidates[j].height;
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
                actor->targetY = (s32)(math_random() & 0x7F) + lowest + 300;
                actor->y = (s32)(math_random() & 0x7F) + lowest + 300;
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
void swarm_spawnRing(s32 count, s32 group, s32 x, s32 y, s32 z, s32 maxRadius,
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
        angle = math_vec3Heading(&diff);
    }
    for (i = 0; i < count; i++) {
        if (typeCount != 0) {
            variant = math_random() % typeCount;
        } else {
            variant = 0;
        }
        actor = effect_newObject(baseType + variant, 0, 0xF8, 1, 0xFF, 1);
        if (actor == NULL) return;
        ringWidth = maxRadius - minRadius;
        onGround = spawnFlags & 1;
        actorX = (s32)math_random() % ringWidth;
        actorX = (actorX < 0) ? actorX - minRadius : actorX + minRadius;
        actorX += x;
        actorY = y;
        actorZ = (s32)math_random() % ringWidth;
        actorZ = (actorZ < 0) ? actorZ - minRadius : actorZ + minRadius;
        actorZ += z;
        if (state == 0 || onGround) {
            actor->groundHeight = collision_findMeshFloorNear(actorX, y, actorZ, &surface);
        }
        if (state == 0) {
            actorY = actor->groundHeight;
        } else if (heightRange != 0) {
            actorY += math_random() % heightRange;
        }
        actor->group = group;
        actor->flags = flags;
        actor->state = state;
        actor->x = actorX;
        actor->y = actorY;
        actor->z = actorZ;
        actor->speed = math_random() % 15U + 10;
        actor->speedStep = 0;
        if (flags & 0x10000) {
            yaw = (math_random() & 0x1F) + angle - 15;
            if (yaw < 0) {
                yaw += 360;
            } else if (yaw >= 360) {
                yaw -= 360;
            }
        } else {
            yaw = math_random() % 360U;
        }
        if (flags & 0x20000) {
            actor->pitch = (s32)(math_random() & 0x1F) - 15;
            actor->pitchStep = -1.0f;
        } else {
            actor->pitch = 0.0f;
            actor->pitchStep = 0.0f;
        }
        actor->roll = 0.0f;
        actor->yaw = yaw;
        actor->yawStep = ((s32)(math_random() & 0xFF) - 128) * 0.009999999776f;
        actor->rollStep = 0.0f;
        actor->scale = (s32)(math_random() & 0xFFFF) * (1.0f / 32768.0f) * scaleRange + minScale;
        if (modelMask != -1) {
            if (modelMask != 0) {
                variant = math_random() & modelMask;
            } else {
                variant = 0;
            }
        }
        variant += baseModel;
        actor->vertices = gEffectModelSlots[variant].unk8;
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
                actor->sound = sound_startVoiceAt(0, 0xA2, 0x7D00, 0, 0, 0, actorX, actorY, actorZ, 1000, 4000);
            }
        } else {
            actor->sound = 0;
        }
        if (onGround) {
            actor->flags |= 0x80;
            // matching: reusing ringWidth/onGround here keeps the original stack frame size
            if (surface != 0) {
                ringWidth = (surface - (s32)gCurMapTris) / 12;
                onGround = gMeshTriMaterials[ringWidth];
            } else {
                onGround = 0;
            }
            actor->surfaceFlags = onGround;
            actor->brightness = gSwarmGroundBrightness[((u32)onGround >> 5) & 3];
        } else {
            actor->brightness = 0;
        }
        actor->alpha = 0;
        actor->fadingOut = 0;
    }
}

#define D_800D35E4 ((&gCullVtxFlags)[-1])

/* Common per-frame flight update for a swarm actor: fading, wandering inside its home ring,
 * pitch/yaw/roll easing, wing-frame swapping and movement. Frees the actor when its model
 * is unloaded, it has faded out, or it leaves the view. */
void swarm_updateFlight(SwarmActor *actor) {
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

    if (gEffectModelSlots[actor->modelIndex].resource == NULL || gEffectModelSecondDls[actor->modelIndex] == NULL) {
        effect_destroyObject(&actor->header);
        return;
    }
    alpha = actor->alpha;
    if ((actor->modelIndex == 8 || actor->modelIndex == 9) && level == LEVEL_THE_WINDMILL && gCutsceneStates[0] != 0 && gCutsceneIds[0] == 0xF) {
        actor->flags &= ~8;
        if (gCutsceneStates[0] == 0 || gCutsceneIds[0] != 0xF) {
            effect_destroyObject(&actor->header);
            return;
        }
        /* flag 0x200: spiral down around (-3478, 3858); pitchStep, yawStep and rollStep then
         * hold the orbit angle, its angular speed and the orbit radius */
        if (gCutsceneTimes[0] >= 60 && !(actor->flags & 0x200)) {
            actor->flags = (actor->flags & ~0x4C) | 0x200;
            dx = actor->x - -3478.0f;
            dz = actor->z - 3858.0f;
            actor->pitchStep = 1.570796371f - math_atan2(dx, dz);
            actor->yawStep = 0.01150000002f;
            actor->rollStep = sqrtf(dx * dx + dz * dz);
        }
    }
    if (actor->fadingOut == 0) {
        if ((actor->flags & 8) && gSwarmGroupsInUse[actor->group] == 0) {
            actor->fadingOut = 1;
        } else if (alpha != 255) {
            alpha += gFrameTicks * 4;
            if (alpha >= 256) {
                alpha = 255;
            }
            actor->alpha = alpha;
        }
    } else {
        if (alpha == 0) {
            effect_destroyObject(&actor->header);
            return;
        }
        alpha -= gFrameTicks * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        actor->alpha = alpha;
    }
    model = &gEffectModelSlots[actor->modelIndex];
    flight = &gSwarmFlights[actor->modelIndex];
    gEffectModelSlotStates[actor->modelIndex] = 1;
    sound = actor->sound;
    if (sound != 0) {
        if (sound_isPlaying(sound) != 0) {
            sound_setVoiceSpatial(actor->sound, 0x7D00, 0, 0, 0, actor->x, actor->y, actor->z, 1000, 4000);
        } else {
            actor->sound = 0;
        }
    }
    if (actor->flags & 4) {
        s32 tooClose;

        dist = math_distance2D(actor->x, actor->z, actor->originX, actor->originZ);
        tooClose = ((actor->flags & 0x10) && dist < actor->minRadius) ? TRUE : FALSE;
        if (actor->maxRadius < dist || tooClose) {
            vec.x = actor->originX - actor->x;
            vec.z = actor->z - actor->originZ;
            angle = math_vec3HeadingBin(&vec);
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
            step = gFrameTicks * 0.5f;
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
                    actor->yawStep -= gFrameTicks * 0.5f;
                    if (actor->yawStep < 0.0f) {
                        actor->yawStep = 0.0f;
                    }
                } else {
                    actor->yawStep += gFrameTicks * 0.5f;
                    if (actor->yawStep > 0.0f) {
                        actor->yawStep = 0.0f;
                    }
                }
            }
            actor->turnTimer -= gFrameTicks;
            if (actor->turnTimer <= 0) {
                actor->targetYaw = math_random() % 360;
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
        actor->groundHeight = collision_findMeshFloorNear(actor->x, actor->y, actor->z, &surface);
        if (surface != 0) {
            surfaceIndex = (surface - (s32)gCurMapTris) / 12;
            surfaceFlags = gMeshTriMaterials[surfaceIndex];
        } else {
            surfaceFlags = 0;
        }
        actor->surfaceFlags = surfaceFlags;
    }
    if (actor->flags & 0x40) {
        if (actor->hoverTimer >= 0) {
            actor->hoverTimer -= gFrameTicks;
            if (actor->hoverTimer <= 0) {
                actor->hoverTimer = -1;
                actor->targetY = (f32)(math_random() % actor->heightRange) + actor->originY;
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
        speed = gFrameTicks;
        actor->pitchStep += actor->yawStep * speed;
        actor->yawStep += 0.005189999938f;
        if (0.1499999911f < actor->yawStep) {
            actor->yawStep = 0.1499999911f;
        }
        actor->rollStep -= 3.200000048f * speed;
        if (actor->rollStep < 60.0f) {
            actor->rollStep = 60.0f;
        }
        sinPitch = math_sin(actor->pitchStep);
        cosPitch = math_cos(actor->pitchStep);
        actor->x = actor->rollStep * cosPitch + -3478.0f;
        actor->z = actor->rollStep * sinPitch + 3858.0f;
        actor->y -= 100.0f / (actor->rollStep - 60.0f + 4.0f);
        if (actor->y < -3100.0f || gCutsceneStates[0] == 0 || gCutsceneIds[0] != 0xF) {
            effect_destroyObject(&actor->header);
            return;
        }
    } else {
        s32 speed;
        rad[0] = actor->pitch * 0.01745329238f;
        rad[1] = actor->yaw * 0.01745329238f;
        rad[2] = actor->roll * 0.01745329238f;
        sinPitch = math_sin(rad[0]);
        sinYaw = math_sin(rad[1]);
        cosPitch = math_cos(rad[0]);
        cosYaw = math_cos(rad[1]);
        dir[0] = cosPitch * sinYaw;
        dir[1] = -sinPitch;
        dir[2] = cosPitch * cosYaw;
        speed = actor->speed * gFrameTicks;
        actor->x += dir[0] * speed;
        actor->y += dir[1] * speed;
        actor->z += dir[2] * speed;
    }
    if (actor->header.type == 0x13 && gSwarmGroupSoundTimers[actor->group] == 0) {
        sound_playAt(0x10, 0x7FFF, math_random() & 0x7F, 0, 0, actor->x, actor->y, actor->z, 500, 4000);
        gSwarmGroupSoundTimers[actor->group] = (math_random() & 0x3F) + 30;
    }
    if ((actor->flags & 2) &&
        prop_isOutsideFrustum(gViewMatrices[0], actor->x, actor->y, actor->z, gDefaultNear, gDefaultFar, gFrustumSlopeX, D_800D35E4, NULL, NULL) != 0 &&
        (gMatchState.mode != 1 ||
         prop_isOutsideFrustum(gViewMatrices[1], actor->x, actor->y, actor->z, gDefaultNear, gDefaultFar, gFrustumSlopeX, D_800D35E4, NULL, NULL) != 0)) {
        effect_destroyObject(&actor->header);
    }
}

/* Update for a perching swarm actor (bat): while perched (state 0) it bobs its head and
 * yaws a little, takes off when the player comes within 400 units (state 1), and once
 * 300 units above its perch it flies normally (states 2/3). */
void swarm_updatePercher(SwarmActor *actor) {
    s32 r;

    if (gSwarmGroupsInUse[actor->group] != 0) {
        gEffectModelSlotStates[actor->modelIndex] = 1;
        switch (actor->state) {
        case 0:
            if (math_distance2D(movement_obj->xpos, movement_obj->zpos, actor->x, actor->z) < 400) {
                actor->state = 1;
                actor->pitchStep = -1 - (s32)(math_random() & 3);
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
                        r = math_random() & 0xFF;
                        if (r < 51) {
                            actor->perchPitchStep = 5.0f;
                        }
                    }
                    if (actor->pitch <= 10.0f) {
                        actor->perchPitchStep = 0.0f;
                        return;
                    }
                } else {
                    r = math_random() & 0xFF;
                    if (r < 25) {
                        actor->perchPitchStep = 5.0f;
                        if (r < 13) {
                            actor->yawStep = (s32)(math_random() % 6U) - 3;
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
                actor->targetY = (s32)(math_random() & 0x7F) + (300.0f + actor->groundHeight);
            }
            swarm_updateFlight(actor);
            return;
        default:
        case 2:
        case 3:
            if (actor->y < actor->targetY) {
                actor->pitchStep = -1.5f;
            } else if (actor->pitch < 0) {
                actor->pitchStep = 1.5f;
            }
            swarm_updateFlight(actor);
            return;
        }
    } else {
        effect_destroyObject(&actor->header);
    }
}

/* Update for a hovering swarm actor: fades in/out with its group, waits (state 0) until the
 * player is within 200 units, then flies off towards a random height; in state 4 it is freed
 * once it drops below its ground height. */
void swarm_updateHoverer(SwarmActor *actor) {
    s32 alpha;

    if ((gEffectModelSlots[actor->modelIndex].resource == NULL) || (gEffectModelSecondDls[actor->modelIndex] == NULL)) {
        effect_destroyObject(&actor->header);
        return;
    }

    alpha = actor->alpha;
    if (actor->fadingOut == 0) {
        if (gSwarmGroupsInUse[actor->group] == 0) {
            actor->fadingOut = 1;
        } else if (alpha != 255) {
            alpha += gFrameTicks * 4;
            if (alpha >= 256) {
                alpha = 255;
            }
            actor->alpha = alpha;
        }
    } else {
        if (alpha == 0) {
            effect_destroyObject(&actor->header);
            return;
        }
        alpha -= gFrameTicks * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        actor->alpha = alpha;
    }

    gEffectModelSlotStates[actor->modelIndex] = 1;

    switch (actor->state) {
        case 0:
            if (math_distance2D(movement_obj[0].xpos, movement_obj[0].zpos, actor->x, actor->z) < 200) {
                actor->state = 2;
                actor->targetY = (f32)(math_random() % actor->heightRange) + actor->originY;
                actor->vertices = gEffectModelSlots[actor->modelIndex].unk8;
            } else {
                actor->vertices = gEffectModelSlots[actor->modelIndex].unk4;
            }
            return;
        case 2:
        case 3:
            break;
        case 4:
            if (actor->y < actor->groundHeight) {
                effect_destroyObject(&actor->header);
                return;
            }
            break;
    }
    swarm_updateFlight(actor);
}

/* Draws a flying swarm actor: builds its scaled model matrix, eases its brightness towards
 * the lighting of the surface below it, and draws its model with that colour and its alpha. */
Gfx *swarm_drawFlying(Gfx *gdl, SwarmActor *actor, s16 camera) {
    s32 pad; // matching: unused stack slot
    MtxF rot;
    MtxF mtx;
    s32 col[3];
    s32 brightness;
    s32 target;

    if ((gEffectModelSlots[actor->modelIndex].resource == NULL) || (gEffectModelSecondDls[actor->modelIndex] == NULL)) {
        return gdl;
    }
    math_mtxRotateRPYF(mtx, 0.0f, actor->yaw, 0.0f);
    math_mtxRotateRPYF(rot, actor->pitch, 0.0f, actor->roll);
    math_mtxMulF(rot, mtx, mtx);
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
    guMtxF2L(mtx, &actor->mtx[gDrawBufferIndex]);
    gSPMatrix(gdl++, &actor->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gdl++, 1, actor->vertices);
    gDPPipeSync(gdl++);
    brightness = actor->brightness;
    target = gSwarmGroundBrightness[(actor->surfaceFlags >> 5) & 3];
    if (brightness != target) {
        if (brightness < target) {
            brightness += gFrameTicks * 4;
            if (target < brightness) {
                brightness = target;
            }
        } else {
            brightness -= gFrameTicks * 4;
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
        gdl = math_gfxSetOtherMode(gdl, 0x82CA0, 0x504A50, &update);
        gDPSetEnvColor(gdl++, col[0], col[1], col[2], actor->alpha);
        gSPDisplayList(gdl++, gEffectModelSlots[actor->modelIndex].displayList);
    }
    return gdl;
}

/* Draws a swarm actor for camera `camera`, unless it is outside that camera's view.
 * Perched actors (state 0) use model 10's resting pose; the rest go through swarm_drawFlying. */
Gfx *swarm_drawActor(Gfx *gdl, SwarmActor *actor, s16 camera) {
    MtxF mtx;
    s32 culled;

    culled = prop_isOutsideFrustum(gViewMatrices[camera], actor->x, actor->y, actor->z,
                           gDefaultNear, 4000.0f, gFrustumSlopeX, D_800D35E4, NULL, NULL);
    if (culled == 0) {
        switch (actor->state) {
        case 0:
            math_mtxRotateRPYF(mtx, actor->pitch, actor->yaw, actor->roll);
            mtx[3][0] = actor->x;
            mtx[3][1] = actor->y;
            mtx[3][2] = actor->z;
            guMtxF2L(mtx, &actor->mtx[gDrawBufferIndex]);
            gSPMatrix(gdl++, &actor->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPSegment(gdl++, 1, gEffectModelSlots[10].unk4);
            gSPDisplayList(gdl++, gEffectModelSlots[10].displayList);
            break;
        case 1:
        case 2:
        case 3:
        default:
            gdl = swarm_drawFlying(gdl, actor, camera);
            break;
        }
    }
    return gdl;
}

/* Flaps model `index`'s wings: updates the flap angle from the flap phase and rotates the inner
 * wing vertices by it and the outer ones by the angle 30 degrees of phase earlier. */
void swarm_flapWings(u8 index) {
    f32 angle;
    f32 sinAngle;
    s16 x;
    s16 y;
    SwarmFlight *flight;
    Vtx *vtx;
    f32 cosAngle;

    flight = &gSwarmFlights[index];
    vtx = gEffectModelSlots[index].unk8;
    angle = flight->u.flap.phase * 0.01745329238f;
    flight->u.flap.angle = math_sin(angle) * flight->u.flap.amplitude;
    angle = flight->u.flap.angle * 0.01745329238f;
    sinAngle = math_sin(angle);
    cosAngle = math_cos(angle);
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
    flight->u.flap.angle = math_sin(angle) * flight->u.flap.amplitude;
    angle = flight->u.flap.angle * 0.01745329238f;
    sinAngle = math_sin(angle);
    cosAngle = math_cos(angle);
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
