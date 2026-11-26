#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The render setup of the tracer effect type (the effect type table entry of beam_updateTracer points to it). */
Gfx gTracerRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};

/* Aim-impact slots: where a shot from an owner at a given angle lands (effect_spawnMaterialImpact fills them). */
AimImpact gAimImpacts[4];

/* Spawns a tracer effect from (x0, y0, z0) towards (x1, y1, z1). */
void beam_spawnTracer(s32 arg0, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 speed, f32 stopDist,
                   f32 halfWidth, f32 length, s16 lifetime, u16 texture, s8 mode, s16 onTime, s16 offTime,
                   Actor *owner, u8 ownerId, u8 bone, s32 ownerTag, u8 impactEffect, u8 unkFE,
                   f32 startX, f32 startY, f32 startZ, f32 endX, f32 endY, f32 endZ) {
    Game10EF60Effect *effect;
    s32 i;
    s32 sMax;
    s32 tMax;

    effect = effect_newObject(0xB, 0, 0x100, 1, 0xFF, 1);
    if (effect != NULL) {
        effect->lifetime = lifetime;
        effect->x = x0;
        effect->y = y0;
        effect->z = z0;
        effect->yaw = math_atan2(-(x1 - x0), -(z1 - z0));
        effect->pitch = math_atan2(y1 - y0, sqrtf((x1 - x0) * (x1 - x0) + (z1 - z0) * (z1 - z0)));
        effect->dx = 0.0f;
        effect->dy = 0.0f;
        effect->dz = 0.0f;
        effect->target_x = x1;
        effect->target_y = y1;
        effect->target_z = z1;
        effect->stopDist = stopDist;
        effect->speed = speed;
        effect->halfWidth = halfWidth;
        effect->length = length;
        effect->attachDelay = 0;
        effect->impactEffect = impactEffect;
        effect->fireCounter = 0;
        effect->unkFE = unkFE;
        effect->owner = owner;
        effect->texture = texture;
        effect->mode = mode;
        if (effect->mode != 0) {
            if (owner == NULL) {
                effect_freeObject(&effect->header);
                return;
            }
            effect->onTime = onTime;
            effect->timer = -offTime;
            effect->offTime = offTime;
            effect->ownerId = ownerId;
            effect->bone = bone;
            effect->ownerTag = ownerTag;
            effect->startOffset[0] = startX;
            effect->startOffset[1] = startY;
            effect->startOffset[2] = startZ;
            effect->endOffset[0] = endX;
            effect->endOffset[1] = endY;
            effect->endOffset[2] = endZ;
        } else {
            effect->timer = 1;
            effect->ownerTag = -1;
        }
        for (i = 0; i < 2; i++) {
            sMax = gEffectAnimTextures[effect->texture]->width - 1;
            tMax = gEffectAnimTextures[effect->texture]->height - 1;
            effect->quads[i][2].tc[0] = 0x2000;
            effect->quads[i][2].tc[1] = 0x2000;
            effect->quads[i][2].flag = 0;
            effect->quads[i][0].tc[0] = (u16)(sMax << 5) + 0x2000;
            effect->quads[i][0].tc[1] = 0x2000;
            effect->quads[i][0].flag = 0;
            effect->quads[i][3].tc[0] = 0x2000;
            effect->quads[i][3].tc[1] = (u16)(tMax << 5) + 0x2000;
            effect->quads[i][3].flag = 0;
            effect->quads[i][1].tc[0] = (u16)(sMax << 5) + 0x2000;
            effect->quads[i][1].tc[1] = (u16)(tMax << 5) + 0x2000;
            effect->quads[i][1].flag = 0;
        }
        effect->drawn = 0;
    }
}

/* Per-frame update of a tracer: re-fires attached ones from the owner's bone, then moves it. */
void beam_updateTracer(Game10EF60Effect *effect) {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 sinA;
    f32 cosA;
    f32 stopDist;
    f32 dist;
    f32 speed;
    f32 (*bone)[4];
    s32 aim;
    s32 texture;
    s32 fire;
    s32 impact;
    s16 angle;
    f32 incoming[3];
    Vec3f reflected;
    f32 fwdY;
    f32 fwdZ;
    f32 rayX;
    f32 rayY;
    f32 rayZ;
    f32 len;
    u32 hitTri;
    s32 unused; // matching: frame size
    s32 i;

    // matching: dead stores; first mentions order uopt'len CSE temp slots
    impact = (s32)gAimImpacts[aim].normal;
    fire = effect->owner->angle - effect->owner->mv->pssd_waist_tilt;
    i = (s32)&effect->target_x;
    bone = (void *)&effect->target_z;
    texture = 0x23;
    dist = 0.0f;
    effect->lifetime -= D_800BE9E4;
    if (effect->lifetime < 0 || (effect->owner != NULL && effect->ownerTag != -1 && effect->owner->animseq.uh != effect->ownerTag)) {
        effect_destroyObject(&effect->header);
        return;
    }
    if (effect->mode != 0) {
        if (effect->attachDelay == 0) {
            if (effect->owner != NULL && effect->owner->ob_matrix_ptr != NULL && effect->owner->unique_id == effect->ownerId && (aim = effect->owner->playernumber) != 0xFF) {
                bone = effect->owner->ob_matrix_ptr[effect->bone];
                math_transformPoint(bone, effect->startOffset[0], effect->startOffset[1], effect->startOffset[2], &effect->x, &effect->y, &effect->z);
                math_transformPoint(bone, effect->endOffset[0], effect->endOffset[1], effect->endOffset[2], &effect->target_x, &effect->target_y, &effect->target_z);
                effect->attachDelay = 3;
            }
        } else {
            effect->attachDelay--;
        }
        effect->timer -= D_800BE9E4;
        if (effect->timer < -effect->offTime) {
            effect->timer = effect->onTime;
            if (effect->owner != NULL && effect->owner->ob_matrix_ptr != NULL && effect->owner->unique_id == effect->ownerId) {
                bone = effect->owner->ob_matrix_ptr[effect->bone];
                math_transformPoint(bone, effect->startOffset[0], effect->startOffset[1], effect->startOffset[2], &effect->x, &effect->y, &effect->z);
                aim = effect->owner->playernumber;
                math_transformPoint(bone, effect->endOffset[0], effect->endOffset[1], effect->endOffset[2], &effect->target_x, &effect->target_y, &effect->target_z);
                stopDist = 0.0f;
                if (effect->fireCounter-- == 0) {
                    effect->fireCounter = 2;
                    fire = 1;
                } else {
                    fire = 0;
                }
                if (effect->speed == 0.0f) {
                    speed = math_rand() % 70 + 40.0f;
                } else {
                    speed = effect->speed;
                }
                impact = 0;
                if (aim != 0xFF) {
                    if (effect->owner->angle - effect->owner->mv->pssd_waist_tilt == gAimImpacts[aim].unk0 && effect->owner->unk1D1 == gAimImpacts[aim].unk2) {
                        effect->target_x = gAimImpacts[aim].x;
                        effect->target_y = gAimImpacts[aim].y;
                        effect->target_z = gAimImpacts[aim].z;
                        if (gIsMultiplayer == 0) {
                            if (math_rand() % 100 < gAimImpacts[aim].ricochetChance) {
                                incoming[0] = effect->target_x - effect->x;
                                incoming[1] = effect->target_y - effect->y;
                                incoming[2] = effect->target_z - effect->z;
                                math_vec3Scale((Vec3f *)gAimImpacts[aim].normal, 2.0f * math_dot3(gAimImpacts[aim].normal, incoming), &reflected);
                                math_vec3Sub((Vec3f *)incoming, &reflected, &reflected);
                                reflected.x += effect->target_x;
                                reflected.y += effect->target_y;
                                reflected.z += effect->target_z;
                                beam_spawnTracer(0, effect->target_x, effect->target_y, effect->target_z, reflected.x, reflected.y, reflected.z, math_rand() % 35 + 20.0f, 0.0f, 8.0f, 320.0f, 30, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                                sound_playAt(math_rand() % 6 + 0x2B, 10000, 0, 0, aim, effect->target_x, effect->target_y, effect->target_z, 500, 2000);
                            }
                        }
                        stopDist = 1.0f;
                    } else {
                        angle = effect->owner->unk1D1 * 200;
                        actor_splitVelocity(effect->owner->angle - effect->owner->mv->pssd_waist_tilt, 2000.0f, angle * 0.005493164f, &effect->target_x, &effect->target_z, &effect->target_y);
                        effect->target_x += effect->x;
                        effect->target_y += effect->y;
                        effect->target_z += effect->z;
                    }
                    effect->length = math_rand() % 3 * 80.0f + 80.0f;
                } else {
                    if (fire) {
                        rayX = effect->target_x - effect->x;
                        rayY = effect->target_y - effect->y;
                        rayZ = effect->target_z - effect->z;
                        len = (75.0f * speed) / sqrtf(rayX * rayX + rayY * rayY + rayZ * rayZ);
                        if (collision_castRayWorld(effect->x, effect->y, effect->z, rayX * len, rayY * len, rayZ * len, &hitTri, 0, &rayX, &rayY, &rayZ, 0, 0, 0, 0.0f)) {
                            effect->target_x = rayX;
                            effect->target_y = rayY;
                            effect->target_z = rayZ;
                            stopDist = 1.0f;
                            i = (hitTri - (u32)gCurMapTris) / 12;
                            if (i >= 0 && i < gCurMapTriCount && gMeshTriMaterials != NULL) {
                                impact = collision_getSurfaceType(gMeshTriMaterials[i]);
                            }
                        }
                    }
                    texture = 0x43;
                }
                effect->yaw = math_atan2(-(effect->target_x - effect->x), -(effect->target_z - effect->z));
                effect->pitch = math_atan2(effect->target_y - effect->y, sqrtf((effect->target_x - effect->x) * (effect->target_x - effect->x) + (effect->target_z - effect->z) * (effect->target_z - effect->z)));
                effect->dx = 0.0f;
                effect->dy = 0.0f;
                effect->dz = 0.0f;
                if (fire) {
                    beam_spawnTracer(0, effect->x, effect->y, effect->z, effect->target_x, effect->target_y, effect->target_z, speed, stopDist, 15.0f, 820.0f, 75, texture, 0, 0, 0, effect->owner, 0, 0, 0, impact, effect->unkFE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                }
            } else {
                effect_destroyObject(&effect->header);
                return;
            }
        }
    } else { // matching: empty else changes IDO'len branch layout
    }
    if (effect->timer > 0) {
        fwdY = 0.0f;
        fwdZ = -1.0f;
        sinA = math_sin(effect->pitch);
        cosA = math_cos(effect->pitch);
        rx = fwdY * cosA;
        ry = fwdY * cosA - sinA * fwdZ;
        rz = fwdY * sinA + cosA * fwdZ;
        sinA = math_sin(effect->yaw);
        cosA = math_cos(effect->yaw);
        vx = cosA * rx + sinA * rz;
        vy = ry;
        vz = -sinA * rx + cosA * rz;
        if (effect->stopDist != 0.0f) {
            dist = sqrtf((effect->target_x - effect->x) * (effect->target_x - effect->x) + (effect->target_y - effect->y) * (effect->target_y - effect->y) + (effect->target_z - effect->z) * (effect->target_z - effect->z));
        }
        if (effect->drawn == 0) {
            vx *= 0.5f;
            vy *= 0.5f;
            vz *= 0.5f;
        }
        effect->dx = vx;
        effect->dy = vy;
        effect->dz = vz;
        len = effect->speed * D_800BE9E4;
        if (dist < len && effect->stopDist != 0.0f) {
            vx *= dist;
            vy *= dist;
            vz *= dist;
            effect->length = dist;
        } else {
            vx *= len;
            vy *= len;
            vz *= len;
        }
        if (effect->unkFE == 1) {
            i = effect->owner != NULL ? effect->owner - movement_obj : 0;
            collision_castRayAtObjects(effect->owner, effect->x, effect->y, effect->z, effect->x + vx, effect->y + vy, effect->z + vz, i, 0);
        }
        effect->x += vx;
        effect->y += vy;
        effect->z += vz;
        if (effect->stopDist != 0.0f) {
            len = sqrtf((effect->target_x - effect->x) * (effect->target_x - effect->x) + (effect->target_y - effect->y) * (effect->target_y - effect->y) + (effect->target_z - effect->z) * (effect->target_z - effect->z));
            if (len < effect->stopDist || dist < len) {
                if (effect->impactEffect != 0) {
                    effect_spawnMaterialImpact(effect->target_x, effect->target_y, effect->target_z, 1, effect->impactEffect, 0, 0xFF);
                }
                effect_destroyObject(&effect->header);
                return;
            }
            if (len < effect->length) {
                effect->length = len;
            }
        }
    }
}


/* Draws a tracer as a camera-facing quad from its position along its direction. */
Gfx *beam_drawTracer(Gfx *gfx, Game10EF60Effect *effect, s16 view) {
    Vec3f points[2];
    f32 camera[3];
    f32 normal[3];
    f32 delta[3];
    s32 i;
    s32 j;
    f32 corners[4][3];
    f32 length;
    f32 scale;
    f32 temp;
    f32 distances[2]; // written but never read
    f32 halfWidth;
    SpriteParams texState;
    u8 flag;

    if (effect->timer > 0) {
        points[0].x = effect->x;
        points[0].y = effect->y;
        points[0].z = effect->z;
        points[1].x = effect->x + effect->length * effect->dx;
        points[1].y = effect->y + effect->length * effect->dy;
        points[1].z = effect->z + effect->length * effect->dz;
        camera[0] = camera_global_Cameras[view].eye.x;
        camera[1] = camera_global_Cameras[view].eye.y;
        camera[2] = camera_global_Cameras[view].eye.z;
        halfWidth = effect->halfWidth;
        for (i = 0; i < 2; i++) {
            distances[i] = (points[i].x - camera[0]) * (points[i].x - camera[0]) + (points[i].y - camera[1]) * (points[i].y - camera[1]) + (points[i].z - camera[2]) * (points[i].z - camera[2]);
        }
        temp = points[1].x;
        points[1].x = points[0].x;
        points[0].x = temp;
        temp = points[1].y;
        points[1].y = points[0].y;
        points[0].y = temp;
        temp = points[1].z;
        points[1].z = points[0].z;
        points[0].z = temp;
        delta[0] = camera[0] - effect->x;
        delta[1] = camera[1] - effect->y;
        delta[2] = camera[2] - effect->z;
        normal[0] = delta[1] * effect->dz - delta[2] * effect->dy;
        normal[1] = delta[2] * effect->dx - delta[0] * effect->dz;
        normal[2] = delta[0] * effect->dy - delta[1] * effect->dx;
        for (i = 0; i < 2; i++) {
            delta[0] = points[i].x - camera[0];
            delta[1] = points[i].y - camera[1];
            delta[2] = points[i].z - camera[2];
            normal[0] = delta[1] * effect->dz - delta[2] * effect->dy;
            normal[1] = delta[2] * effect->dx - delta[0] * effect->dz;
            normal[2] = delta[0] * effect->dy - delta[1] * effect->dx;
            length = sqrtf(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
            if (length == 0.0f) {
                return gfx;
            }
            scale = halfWidth / length;
            normal[0] *= scale;
            normal[1] *= scale;
            normal[2] *= scale;
            corners[i * 2][0] = points[i].x + normal[0];
            corners[i * 2][1] = points[i].y + normal[1];
            corners[i * 2][2] = points[i].z + normal[2];
            corners[i * 2 + 1][0] = points[i].x - normal[0];
            corners[i * 2 + 1][1] = points[i].y - normal[1];
            corners[i * 2 + 1][2] = points[i].z - normal[2];
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 3; j++) {
                effect->quads[D_800BE9C0][i].ob[j] = corners[i][j];
            }
        }
        gfx = sprite_loadDescFrame(gfx, gEffectAnimTextures[effect->texture], 0, &texState, 0, 0, 0, 2, 3);
        flag = 0;
        gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, 0x504A50, &flag);
        gSPVertex(gfx++, effect->quads[D_800BE9C0], 4, 0);
        gSP1Triangle(gfx++, 0, 1, 3, 0);
        gSP1Triangle(gfx++, 0, 3, 2, 0);
    }
    effect->drawn = 1;
    return gfx;
}

s32 func_150E2DA4(s32 arg0, s32 arg1) {
    return arg0;
}

/* Spawns a tracer attached to a bone of owner, refiring every onTime + offTime frames. */
void beam_spawnBoneTracer(Actor *owner, u8 ownerId, s16 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ,
                   f32 endX, f32 endY, f32 endZ, s16 onTime, s16 offTime, u16 lifetime, u8 unkFE) {
    beam_spawnTracer(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 400.0f, lifetime, 39, 1, onTime, offTime,
                  owner, ownerId, bone, ownerTag, 0, unkFE, startX, startY, startZ, endX, endY, endZ);
}

/* Like beam_spawnBoneTracer with a caller-chosen width, length and speed. */
// matching: functions.h declares the narrow params as s32, but the original took u8/s16 (lbu/lh of the arg slot)
void beam_spawnBoneTracerEx(Actor *owner, s32 ownerId, s32 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ,
                   f32 endX, f32 endY, f32 endZ, s32 onTime, s32 offTime, s32 lifetime, f32 halfWidth, f32 length,
                   s32 unkFE, f32 speed) {
    beam_spawnTracer(0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, speed, 0.0f, halfWidth, length, *(u16 *)((u8 *)&lifetime + 2),
                  39, 1, *(s16 *)((u8 *)&onTime + 2), *(s16 *)((u8 *)&offTime + 2), owner, *((u8 *)&ownerId + 3),
                  *(s16 *)((u8 *)&bone + 2), ownerTag, 0, *((u8 *)&unkFE + 3), startX, startY, startZ, endX, endY, endZ);
}

s32 func_150E2F90(s32 a0, s32 a1, s16 a2) {
    return func_150E2DA4(a0, a2);
}

/* Message 0x2D: if the effect's owner is one object of the pair, retarget it to the other. */
void beam_tracerSwapMessage(Game10EF60Effect *effect, ActorSwapMsg *swap, u8 msg) {
    if (msg == 0x2D) {
        if (effect->owner == swap->actorA) {
            effect->owner = swap->actorB;
            effect->ownerId = swap->idB;
        } else if (effect->owner == swap->actorB) {
            effect->owner = swap->actorA;
            effect->ownerId = swap->idA;
        }
    }
}
