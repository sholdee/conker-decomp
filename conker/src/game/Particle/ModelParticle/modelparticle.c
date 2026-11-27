#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
ParticleNode *gParticleModelsHead; // loaded particle models: list head
ParticleNode *gParticleModelsTail; // list tail
s16 gParticleModelUseCounts[233];      // per model: live particles that use it
s32 gModelParticleCount;           // live particles (at most 300)
s16 *gParticleModelTextures[233];     // per model: the textures its display list uses (texture_resolveDl)

#include "functions.h"
#include "variables.h"
#include "libc/string.h"

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))


/* Collision handlers; each gets the particle's position from before the move. Returning 0 stops the update. */
typedef s32 (*ParticleFloorHitFn)(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight,
                                  CollisionTri *surface);
typedef s32 (*ParticleFloorType3Fn)(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
typedef s32 (*ParticleWallHitFn)(ModelParticle *particle, Actor *probe, f32 x, f32 y, f32 z);



typedef s32 (*ParticleUpdateFn)(ModelParticle *);
typedef void (*ParticleDestroyFn)(ModelParticle *);
typedef s32 (*ParticleMatrixFn)(Mtx *mtx, ModelParticle *particle);
typedef void (*ParticleEventFn)(ModelParticle *particle, void *data, u8 event);

/* This file's .data: the particle handler tables. */
static s32 (*gModelParticleUpdateFuncs[])() = {
    NULL, particle_movePhysics, particle_moveBallisticConstAccel, particle_moveTumbler,
    effect_moveTumblingBurst, particle_updateSoundPulse, particle_updatePulseWobble, effect_moveBurstParticle,
    particle_moveBallistic, light_updateLampParticle, weapon_updateBullet, collision_updateWobble,
    particle_settleWobble, particle_moveBallisticFloor, particle_bobModelParticle, particle_updateGrowFade,
    effect_updateSway, particle_updateFallingDebris, particle_updateSwell, effect_updateProjectile,
    effect_updatePuffFollower, cutscene_updateTrackEffect, particle_moveSpiral, func_151C7038,
    func_150C5310,
};
static s32 (*gModelParticleUpdate2Funcs[])() = {
    NULL, particle_updateKickable, particle_splashOnFloor, particle_dieBelowFloor,
    particle_fadeInAboveFloor, blood_dropDebrisTrail, func_150C7670, particle_updateSpiralFadeSound,
};
static ParticleFloorHitFn gModelParticleFloorFuncs[] = {
    NULL, particle_floorShatter, effect_bounceOnGround, effect_bounceOffTriangle,
    blood_onDebrisFloorHit, pipeexplode_onChunkFloorHit, particle_floorBounce, particle_floorStop,
    blood_onChunkLand, blood_onGorePartLand, particle_floorBounceSettle, func_15133D20,
    func_15133C58, func_150C01DC, prop_onPieceFloorHit,
};
static ParticleFloorType3Fn gModelParticleFloorType3Funcs[] = { NULL };
static ParticleWallHitFn gModelParticleWallFuncs[] = { NULL, particle_wallShatter };
static void (*gModelParticleDestroyFuncs[])() = { NULL, blood_destroyDebris, particle_stopPulseSound };
static s32 (*gModelParticleSquashFuncs[])() = { particle_updateSquash };
static s32 (*gModelParticleMatrixFuncs[])() = {
    particle_buildScaledMatrix, particle_buildLookAtMatrix, particle_buildUniformMatrix, effect_setParticleIdentityMtx,
    light_getLampParticleMatrix, particle_buildOwnerMatrix,
};
static Gfx *(*gModelParticleDrawFuncs[])(Gfx *gfx, ModelParticle *particle, s16 arg2) = { particle_bindDataTexture, particle_bindFirstListTexture, particle_bindListTextures };
static void (*gModelParticleDestroyVariants[])() = {
    particle_destroyModelParticleDefault, weapon_destroyBullet, particle_destroyModelParticleDefault, particle_destroyModelParticleDefault,
    particle_destroyModelParticleDefault, particle_destroyModelParticleDefault, particle_destroyModelParticleDefault, particle_destroyTrailProjectile,
    particle_destroyModelParticleDefault,
};
static void (*gModelParticleFreeVariants[])() = {
    particle_freeModelParticleDefault, weapon_freeBullet, particle_freeModelParticleDefault, particle_freeModelParticleDefault,
    particle_freeModelParticleDefault, particle_freeModelParticleDefault, particle_freeModelParticleDefault, particle_freeTrailProjectile,
    particle_freeModelParticleDefault,
};
static ParticleEventFn gModelParticleEventVariants[] = {
    NULL, weapon_onBulletPairEvent, NULL, NULL,
    NULL, particle_onOwnerGone, blood_handleChunkMessage, NULL,
    particle_onSpiralEvent, NULL,
};

/* Per-frame update: run the particle's update callbacks, age and fade it, and destroy it when any step says so. */
void particle_updateModelParticle(ModelParticle *particle) {
    s32 result;
    u8 dead;

    dead = FALSE;
    particle->flags &= ~0x200000;
    if ((particle->flags & 0x10) && (particle->hitFlags & 0x18)) {
        result = gModelParticleSquashFuncs[particle->squashFn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    } else if (particle->updateFn != 0) {
        result = gModelParticleUpdateFuncs[particle->updateFn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    }

    if (particle->update2Fn != 0 && !dead) {
        result = gModelParticleUpdate2Funcs[particle->update2Fn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    }

    if ((particle->flags & 0x80) && !dead) {
        particle->lifetime -= gFrameTicks;
        if (particle->lifetime < 0) {
            dead = TRUE;
        }
    }

    if ((particle->flags & 0x2000) && !dead) {
        if (particle->lifetime < particle->fadeStart) {
            result = particle->lifetime * particle->fadeRate;
            if (result < particle->alpha) {
                particle->alpha = result;
            }
        }
    }

    if (!dead && !(particle->flags & 0x80000)) {
        dead = math_isInWorldBounds(&particle->pos) == 0;
    }

    if (dead) {
        if (particle->destroyFn != 0) {
            gModelParticleDestroyFuncs[particle->destroyFn](particle);
        }
        effect_destroyObject(&particle->header);
    }
}

/* Dispatch to the variant's handler in gModelParticleDestroyVariants (variant 0 unless flag 0x100 is set). */
void particle_destroyModelParticle(ModelParticle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    gModelParticleDestroyVariants[variant](particle);
}

/* Dispatch to the variant's handler in gModelParticleFreeVariants (variant 0 unless flag 0x100 is set). */
void particle_freeModelParticle(ModelParticle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    gModelParticleFreeVariants[variant](particle);
}

/* Release a particle's model reference: drop the per-model count, and once no particle uses the model
 * (and it is not marked to stay resident) unlink its node and free the model. */
void particle_releaseModel(ModelParticle *particle) {
    ParticleNode *node;

    gModelParticleCount--;
    gParticleModelUseCounts[particle->model]--;
    if (gParticleModelUseCounts[particle->model] == 0) {
        if (particle->node->keepLoaded) {
            gParticleModelUseCounts[particle->model] = 1;
        } else {
            texture_releaseList(gParticleModelTextures[particle->model]);
            node = particle->node;
            if (node->prev == NULL) {
                gParticleModelsHead = node->next;
                if (gParticleModelsHead != NULL) {
                    gParticleModelsHead->prev = NULL;
                } else {
                    gParticleModelsTail = NULL;
                }
            } else {
                node->prev->next = node->next;
                node = particle->node;
                if (node->next != NULL) {
                    node->next->prev = node->prev;
                } else {
                    gParticleModelsTail = node->prev;
                }
            }
            heap_setTag(particle->node->resource, 4);
            heap_setTag((s32 *)particle->node, 4);
        }
    }
    particle_freeModelParticleLights(particle);
}

void particle_destroyModelParticleDefault(ModelParticle *particle) {
    particle_releaseModel(particle);
    effect_defaultDestroy(&particle->header);
}

void particle_freeModelParticleDefault(ModelParticle *particle) {
    particle_releaseModel(particle);
    effect_defaultFree(&particle->header);
}

/* Free the light buffers particle_newModelParticleWithFloor allocated into link.handles[0..gScreenLayout] and link.sharedHandle. */
void particle_freeModelParticleLights(ModelParticle *particle) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= gScreenLayout; i++) {
        if (particle->link.handles[i] != NULL) {
            heap_setTag((void *)(s32)particle->link.handles[i], 4);
        }
    }

    if (particle->link.sharedHandle != NULL) {
        heap_setTag((void *)(s32)particle->link.sharedHandle, 4);
    }
}

/* Spawn a particle from a template: allocate it (failing past 300 live), find or load the shared node for
 * its model, copy the template, reset collision state (or copy `floor`) and allocate its buffers. */
void *particle_newModelParticleWithFloor(ParticleSpawn *spawn, s32 arg1, s32 arg2, CollisionHit *floor, s32 extraSize, u8 arg5,
                    s32 arg6) {
    ModelParticle *particle;
    ParticleNode *node;
    s32 i;

    if (gModelParticleCount > 300) {
        return NULL;
    }
    particle = effect_newObject((spawn->flags & 0x4000) ? 0x48 : 0x19, arg6, extraSize + 0x170, 1, arg5,
                             (spawn->flags & 0x400000) ? 2 : 1);
    if (particle == NULL) {
        return NULL;
    }
    if (gParticleModelUseCounts[spawn->model] == 0) {
        node = allocate_memory(sizeof(ParticleNode), 1, 2, (spawn->flags & 0x400000) ? 2 : 1);
        if (node == NULL) {
            effect_unlinkObject(&particle->header);
            heap_free(particle);
            return NULL;
        }
        if (particle_loadModel(spawn->model, node, particle)) {
            node->next = gParticleModelsHead;
            if (node->next != NULL) {
                node->next->prev = node;
            } else {
                gParticleModelsTail = node;
            }
            gParticleModelsHead = node;
            node->prev = NULL;
            node->model = spawn->model;
            if (spawn->flags & 0x100000) {
                if (level == LEVEL_W1_WINDMILL_EXPLODED || level == LEVEL_THE_WINDMILL || level == LEVEL_W2_HARBOUR ||
                    gIsMultiplayer != 0 || level == LEVEL_W1_BRONTO_CAVERN) {
                    node->keepLoaded = FALSE;
                } else {
                    node->keepLoaded = TRUE;
                }
            } else {
                node->keepLoaded = FALSE;
            }
        } else {
            effect_unlinkObject(&particle->header);
            heap_free(particle);
            heap_free(node);
            return NULL;
        }
    } else {
        s32 tries;

        node = gParticleModelsHead;
        tries = 100;
        while (node->model != spawn->model) {
            tries--;
            node = node->next;
            if (tries <= 0) {
                break;
            }
        }
        if (tries <= 0) {
            effect_unlinkObject(&particle->header);
            heap_free(particle);
            return NULL;
        }
    }
    gParticleModelUseCounts[spawn->model]++;
    particle->node = node;
    memcpy(&particle->radius, spawn, sizeof(ParticleSpawn));
    particle->squashPhase = 0;
    particle->squashOrigin.x = 0.0f;
    particle->squashOrigin.y = 0.0f;
    particle->squashOrigin.z = 0.0f;
    particle->speed = sqrtf(spawn->vel.x * spawn->vel.x + spawn->vel.y * spawn->vel.y +
                            spawn->vel.z * spawn->vel.z);
    particle->hitFlags = 0;
    particle->invSpeed = 1.0f;
    if (floor != NULL) {
        particle->floor = *floor;
    } else {
        particle->floor.height = -10000.0f;
        particle->floor.surface = NULL;
        particle->floor.source = 0;
        particle->floor.flags = 0;
        particle->floor.material = 0;
    }
    gModelParticleCount++;
    particle->link.unk4 = 0;
    particle->link.model = arg1;
    particle->link.unk1C = arg2;
    for (i = 0; i < ARRAY_COUNT(particle->link.handles); i++) {
        particle->link.handles[i] = NULL;
    }
    particle->link.sharedHandle = NULL;
    if (arg1 != 0) {
        for (i = 0; i <= gScreenLayout; i++) {
            particle->link.handles[i] = light_allocGfxLights(arg1);
        }
        particle->link.sharedHandle = light_allocAmbients();
    }
    particle->flags &= ~0x200000;
    return particle;
}

void *particle_newModelParticle(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5) {
    return particle_newModelParticleWithFloor(spawn, arg1, arg2, NULL, extraSize, arg4, arg5);
}

/* Rebuild this frame's matrix through the matrixFn handler; flag 0x40000 says the particle is drawable.
 * A handler result of -1 destroys the particle. */
void particle_buildModelParticleMatrix(ModelParticle *particle) {
    ZoneNode *zone;
    s32 result;

    particle->flags &= ~0x200000;
    if (particle->flags & 0x20000) {
        return;
    }

    if (particle->flags & 0x400) {
        zone = particle->unk6C;
        if (zone != NULL && math_isZoneHiddenFromViews(zone) != 0) {
            return;
        }
    }

    if (particle->flags & 0x80000) {
        particle->flags |= 0x200000;
        return;
    }

    result = gModelParticleMatrixFuncs[particle->matrixFn](&particle->mtx[gDrawBufferIndex], particle);
    if (result == -1) {
        effect_destroyObject(&particle->header);
        return;
    }

    if (result == 0) {
        particle->flags &= ~0x40000;
    } else {
        particle->flags |= 0x240000;
    }
}

/* Draw a particle whose matrix particle_buildModelParticleMatrix built this frame: segment 8, render and geometry mode (lit when
 * flag 0x800), optional extra commands from drawFn, then the matrix and the model's display list. */
Gfx *particle_drawModelParticle(Gfx *gfx, ModelParticle *arg1, s16 arg2) {
    ModelParticle *particle;
    u8 state;

    particle = arg1; // matching: IDO allocates s0/s1 and the frame from this param copy

    if (!(particle->flags & 0x200000)) {
        return gfx;
    }
    if (particle->flags & 0x20000) {
        return gfx;
    }
    if (!(particle->flags & 0x40000)) {
        return gfx;
    }
    state = 1;
    if (particle->alpha < 0xFF) {
        gSPSegment(gfx++, 8, gEffectXluRenderModes);
    } else {
        gSPSegment(gfx++, 8, gEffectOpaRenderModes);
    }
    if (particle->flags & 0x10000) {
        if (particle->drawFn != -1) {
            gfx = gModelParticleDrawFuncs[particle->drawFn](gfx, particle, arg2);
        }
    }
    gfx = math_gfxSetEnvColor(gfx, 0, 0, 0, particle->alpha, &state);
    gfx = math_gfxUpdateGeometryMode(gfx,
                        ((particle->flags & 0x800) ? G_LIGHTING : 0) | ((particle->flags & 0x1000) ? 0x400000 : 0) |
                            G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH,
                        ((particle->flags & 0x800) ? 0 : G_LIGHTING) | ((particle->flags & 0x1000) ? 0 : 0x400000) |
                            G_CULL_BOTH | G_FOG | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    if (particle->flags & 0x800) {
        gfx = math_emitPointLights(gfx, &particle->link, particle->unk7A, particle->owner, particle->ownerId, arg2,
                            &particle->pos, (particle->flags & 0x1000) ? 1 : 0, particle->floor.material);
    }
    gSPMatrix(gfx++, &particle->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, *particle->node->resource);
    return gfx;
}

/* Move a physics particle (gravity, velocity, spin) and collide it with the floor and walls, recording
 * hits in hitFlags and calling the hit handlers. Returns 0 if a handler ended the update. */
s32 particle_movePhysics(ModelParticle *particle) {
    Vec3f prev;
    Vec3f test;
    s32 probePad; // matching: stack slot next to the probe
    Actor probe; // a stand-in actor for the wall probe
    f32 speed;

    if (particle->flags & 7) {
        memcpy(&prev, &particle->pos, sizeof(Vec3f));
    }

    if (particle->flags & 8) {
        particle->vel.y += particle->gravity * gFrameDelta;
    }

    if (particle->flags & 0x20) {
        particle->pos.x += particle->vel.x * gFrameDelta;
        particle->pos.y += particle->vel.y * gFrameDelta;
        particle->pos.z += particle->vel.z * gFrameDelta;
    }

    if (particle->flags & 0x40) {
        particle->rot.x += particle->rotVel.x * gFrameDelta;
        particle->rot.y += particle->rotVel.y * gFrameDelta;
        particle->rot.z += particle->rotVel.z * gFrameDelta;
    }

    if (particle->flags & 7) {
        test.x = particle->pos.x;
        test.y = prev.y;
        test.z = particle->pos.z;

        if (collision_probeFloorSurfABSet0Cached(&test, 0, particle->pos.y - particle->radius, &particle->floor) != 0) {
            if (particle->floor.source == 1 || (particle->floor.source == 2 && (particle->floor.flags & 1))) {
                particle->hitFlags |= 1;
                if (particle->flags & 0x10) {
                    speed = sqrtf(particle->vel.x * particle->vel.x + particle->vel.y * particle->vel.y +
                                  particle->vel.z * particle->vel.z);
                    if (speed > 1.0f) {
                        particle->hitFlags |= 8;
                        particle->speed = speed;
                        particle->invSpeed = 1.0f / speed;
                    }
                }

                if (particle->floorHitFn != 0) {
                    if (gModelParticleFloorFuncs[particle->floorHitFn](particle, prev.x, prev.y, prev.z, particle->floor.height,
                                                         &particle->floor.tri) == 0) {
                        return 0;
                    }
                }

                if (particle->flags & 0x10) {
                    memcpy(&particle->squashOrigin, &particle->pos, sizeof(Vec3f));
                }
            }

            if (particle->floor.source == 3) {
                particle->hitFlags |= 2;
                if (particle->floorType3Fn != 0) {
                    if (gModelParticleFloorType3Funcs[particle->floorType3Fn](particle, prev.x, prev.y, prev.z,
                                                           particle->floor.height) == 0) {
                        return 0;
                    }
                }
            }
        }

        if (particle->flags & 4) {
            probe.type = 4;
            probe.xpos = particle->pos.x;
            probe.ypos = particle->pos.y;
            probe.zpos = particle->pos.z;
            probe.floorTriVerts = 0;
            probe.floorTri = particle->floor.tri;
            probe.shad = 1.0f;
            probe.scale_xz = 1.0f;
            probe.bodyRadius = particle->radius;
            probe.scale_y = 1.0f;
            probe.bodyHeight = particle->radius;
            if (particle->floor.flags & 2) {
                probe.floorY = particle->floor.height;
            } else {
                probe.floorY = -10000.0f;
            }
            probe.noshove = 0;

            if (collision_resolveActorWalls(prev.x, prev.y, prev.z, &probe, 0, 0) != 0) {
                particle->hitFlags |= 4;
                if (particle->flags & 0x10) {
                    speed = sqrtf(particle->vel.x * particle->vel.x + particle->vel.y * particle->vel.y +
                                  particle->vel.z * particle->vel.z);
                    if (speed > 1.0f) {
                        particle->hitFlags |= 0x10;
                        particle->speed = speed;
                        particle->invSpeed = 1.0f / speed;
                        particle->squashAngle = math_atan2(probe.xpos - particle->pos.x,
                                                              probe.zpos - particle->pos.z) *
                                                40.74366379f; // 256 / (2 * pi): radians to a byte angle
                    }
                }

                if (particle->wallHitFn != 0) {
                    if (gModelParticleWallFuncs[particle->wallHitFn](particle, &probe, prev.x, prev.y, prev.z) == 0) {
                        return 0;
                    }
                }

                if (particle->flags & 0x10) {
                    memcpy(&particle->squashOrigin, &particle->pos, sizeof(Vec3f));
                }
            }
        }
    }

    return 1;
}

/* Squash-and-stretch after a fast floor (hitFlags 8) or wall (hitFlags 0x10) hit: oscillate the scale and
 * offset the position from squashOrigin over half a sine period, then restore it. */
s32 particle_updateSquash(ModelParticle *particle) {
    s32 pad; // matching: unused stack slot
    f32 amplitude;
    f32 cosine;
    f32 sine;

    particle->scale.x = 1.0f;
    particle->scale.y = 1.0f;
    particle->scale.z = 1.0f;
    if (particle->flags & 0x40) {
        particle->rot.x += particle->rotVel.x * gFrameDelta;
        particle->rot.y += particle->rotVel.y * gFrameDelta;
        particle->rot.z += particle->rotVel.z * gFrameDelta;
    }
    particle->squashPhase += (s32)(576.0f * particle->invSpeed) * gFrameTicks;
    if (particle->squashPhase < 0x80) {
        amplitude = math_cosBinAngleMasked(particle->squashPhase - 0x40);
        amplitude = (1.0f - particle->elasticity) * (1.0f - particle->invSpeed) * amplitude;
        if (particle->hitFlags & 8) {
            particle->scale.x *= 1.0f + amplitude;
            particle->scale.y *= 1.0f - amplitude;
            particle->scale.z *= 1.0f + amplitude;
            particle->pos.y = particle->squashOrigin.y - amplitude * particle->radius;
        }
        if (particle->hitFlags & 0x10) {
            cosine = math_cosBinAngleMasked(particle->squashAngle - 0x40);
            sine = math_cosBinAngleMasked(particle->squashAngle);
            particle->scale.x *= 1.0f - amplitude;
            particle->scale.y *= 1.0f + amplitude;
            particle->scale.z *= 1.0f - amplitude;
            particle->pos.x = particle->squashOrigin.x - (amplitude * particle->radius) * sine;
            particle->pos.z = particle->squashOrigin.z + (amplitude * particle->radius) * cosine;
        // matching: an (unused) label on the early return keeps as1 from filling its delay slot
        early_return:
            return 1;
        }
    } else {
        memcpy(&particle->pos, &particle->squashOrigin, sizeof(Vec3f));
        particle->squashPhase = 0;
        particle->hitFlags &= ~0x18;
    }
    return 1;
}

/* Matrix handler: rotation, per-axis scale times size, and translation. */
s32 particle_buildScaledMatrix(Mtx *mtx, ModelParticle *particle) {
    math_mtxRotScaleAxes(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->scale.x, particle->scale.y, particle->scale.z, particle->pos.x, particle->pos.y,
                  particle->pos.z);
    return 1;
}

/* Event handler: follow an owner swap (0x2D), forget a removed owner (0), then pass the event on to the
 * variant's handler in gModelParticleEventVariants. */
void particle_onModelParticleEvent(ModelParticle *particle, void *data, u8 event) {
    s32 variant;
    ParticleEventFn handler;
    Actor *obj;

    if (event == 0x1B && particle->model == 0x22) {
        light_startLampSwing((LampParticle *)particle);
    }
    if (event == 0x2D) {
        ActorSwapMsg *swap = data;

        obj = swap->actorA;
        if (obj == particle->owner) {
            particle->owner = swap->actorB;
            particle->ownerId = swap->idB;
        } else if (particle->owner == swap->actorB) {
            particle->owner = obj;
            particle->ownerId = swap->idA;
        }
    } else if (event == 0) {
        ActorRef *removed = data;

        obj = removed->actor;
        if (obj == particle->owner || removed->uniqueId == particle->ownerId) {
            particle->owner = NULL;
            particle->ownerId = 0;
        }
    }
    variant = (particle->flags & 0x100) ? particle->variant : 0;
    handler = gModelParticleEventVariants[variant];
    if (handler != NULL) {
        handler(particle, data, event);
    }
}

/* Load model `model` (D_800A3880 table, bank 9) into the node and prepare it; returns FALSE if the load fails. */
s32 particle_loadModel(s32 model, ParticleNode *node, ModelParticle *particle) {
    u32 out1;
    s32 out2;

    node->resource = rom_loadPathTable(&out1, 0, &out2, 2, 9, D_800A3880[model]);
    if (node->resource == NULL) {
        return FALSE;
    }
    texture_resolveDl(*node->resource, 0, 1, 0x3E, &gParticleModelTextures[model]);
    effect_relocateDisplayList(*node->resource, (s32)node->resource);
    return TRUE;
}

/* Matrix handler: rotation scaled by size only, and translation. */
s32 particle_buildUniformMatrix(Mtx *mtx, ModelParticle *particle) {
    math_mtxRotScaleXZ(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->pos.x, particle->pos.y, particle->pos.z);
    return 1;
}

/* Ballistic step: integrate position under gravity (y gains vel.y*dt + gravity*dt*dt/2), then velocity and spin. */
s32 particle_moveBallistic(ModelParticle *particle) {
    particle->pos.x += particle->vel.x * gFrameDelta;
    particle->pos.y += (particle->vel.y * gFrameDelta) + (((particle->gravity * gFrameDelta) * gFrameDelta) * 0.5f);
    particle->pos.z += particle->vel.z * gFrameDelta;
    particle->vel.y += particle->gravity * gFrameDelta;
    particle->rot.x += particle->rotVel.x * gFrameDelta;
    particle->rot.y += particle->rotVel.y * gFrameDelta;
    particle->rot.z += particle->rotVel.z * gFrameDelta;
    return 1;
}

/* Ballistic update with floor collision only while falling; the floor handler runs unless the floor is an
 * object surface flagged (flags & 0x60) == 0x40. */
s32 particle_moveBallisticFloor(ModelParticle *particle) {
    s8 result;
    Vec3f prev;
    Vec3f test;
    WorldObject *surface;
    s32 skip;

    result = 1;
    prev = particle->pos;
    particle_moveBallistic(particle);
    if ((particle->flags & 1) && particle->floorHitFn != 0 && particle->pos.y < prev.y) {
        test.x = particle->pos.x;
        test.y = particle->radius + prev.y;
        test.z = particle->pos.z;
        if (collision_probeFloorSurfABSet0Cached(&test, 0, particle->pos.y - particle->radius, &particle->floor) != 0) {
            skip = FALSE;
            if (particle->floor.surface != NULL) {
                surface = particle->floor.surface;
                if ((surface->object_status & 0x60) == 0x40) {
                    skip = TRUE;
                }
            }
            if (!skip) {
                result = gModelParticleFloorFuncs[particle->floorHitFn](particle, prev.x, prev.y, prev.z, particle->floor.height,
                                                          &particle->floor.tri);
            }
        }
    }
    return result;
}

/* Floor handler: rest on the floor and bounce, scaling velocity and spin by elasticity. */
s32 particle_floorBounce(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
    f32 elasticity;

    particle->pos.y = particle->radius + floorHeight;
    elasticity = particle->elasticity;
    particle->vel.x = particle->vel.x * elasticity;
    particle->vel.y = particle->vel.y * -elasticity;
    particle->vel.z = particle->vel.z * elasticity;
    particle->rotVel.x = particle->rotVel.x * elasticity;
    particle->rotVel.y = particle->rotVel.y * elasticity;
    particle->rotVel.z = particle->rotVel.z * elasticity;
    return 1;
}

/* Floor handler: stop dead on the floor. */
s32 particle_floorStop(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
    particle->pos.y = particle->radius + floorHeight;
    particle->vel.x = 0.0f;
    particle->vel.y = 0.0f;
    particle->vel.z = 0.0f;
    particle->rotVel.x = 0.0f;
    particle->rotVel.y = 0.0f;
    particle->rotVel.z = 0.0f;
    return 1;
}

/* Matrix handler for particles attached to their owner: use the owner's float matrix number data[0]
 * (as is when gBoneMatricesFlushed is set, else converted to fixed point). Returns -1 if the owner is gone. */
s32 particle_buildOwnerMatrix(Mtx *mtx, ModelParticle *particle) {
    Actor *owner;
    MtxF *ownerMtxs;
    MtxF mf;
    s32 pad; // matching: unused stack slot

    owner = particle->owner;
    if (owner == NULL || owner->type == 0 || owner->character_type == 0xFF ||
        owner->unique_id != particle->ownerId) {
        return -1;
    }
    ownerMtxs = owner->ob_matrix_ptr;
    if (ownerMtxs == NULL) {
        return 0;
    }
    if ((owner->cullMask & 0xF) == 0xF) {
        return 0;
    }
    if (gBoneMatricesFlushed) {
        memcpy(mtx, ownerMtxs[MODEL_PARTICLE_DATA(particle)[0]], sizeof(mf));
        return 1;
    }
    memcpy(mf, ownerMtxs[MODEL_PARTICLE_DATA(particle)[0]], sizeof(mf));
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
    guMtxF2L(mf, mtx);
    return 1;
}

/* Floor handler: bounce like particle_floorBounce, and come to rest (clearing the motion flags) once the
 * rebound is slower than 4. */
s32 particle_floorBounceSettle(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    f32 elasticity;

    elasticity = particle->elasticity;
    particle->pos.y = particle->radius + floorHeight;
    particle->vel.x = particle->vel.x * elasticity;
    particle->vel.y = particle->vel.y * -elasticity;
    particle->vel.z = particle->vel.z * elasticity;
    particle->rotVel.x = particle->rotVel.x * elasticity;
    particle->rotVel.y = particle->rotVel.y * elasticity;
    particle->rotVel.z = particle->rotVel.z * elasticity;
    if (fabsf(particle->vel.y) < 4.0f) {
        particle->flags &= ~0x69;
        particle->vel.x = 0.0f;
        particle->vel.y = 0.0f;
        particle->vel.z = 0.0f;
        particle->rotVel.x = 0.0f;
        particle->rotVel.y = 0.0f;
        particle->rotVel.z = 0.0f;
        particle->gravity = 0.0f;
    }
    return 1;
}

/* Floor handler: particle_floorBounceSettle, plus (half the time) a random sound 0x2DE..0x2E6 at the hit position. */
s32 func_15133C58(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    particle_floorBounceSettle(particle, x, y, z, floorHeight, surface);
    if (math_random() & 1) {
        sound_playAt((math_random() % 9U) + 0x2DE, 12000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Floor handler: particle_floorBounceSettle, plus (half the time) a random sound 0x1B8..0x1C1 at the hit position. */
s32 func_15133D20(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    particle_floorBounceSettle(particle, x, y, z, floorHeight, surface);
    if (math_random() & 1) {
        sound_playAt((math_random() % 10U) + 0x1B8, 24000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Event handler: destroy the particle when its owner goes away (event 0). */
void particle_onOwnerGone(ModelParticle *particle, ActorRef *data, u8 event) {
    Actor *obj;

    if (event == 0) {
        obj = data->actor;
        if (obj == particle->owner || particle->ownerId == data->uniqueId) {
            effect_destroyObject(&particle->header);
        }
    }
}

/* Call effect_sendEventToTypes for both particle object kinds (0x19 and 0x48, see particle_newModelParticleWithFloor). */

void particle_broadcastToModelParticles(s32 arg0, u8 arg1) {
    const s32 kinds[2] = { 0x19, 0x48 };

    effect_sendEventToTypes(kinds, ARRAY_COUNT(kinds), arg0, arg1);
}

/* A texture to bind to a segment: gEffectTextures index, segment number, and texture_getAddress's third argument. */
typedef struct {
    /* 0x0 */ u16 index;
    /* 0x2 */ u8 segment;
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s32 arg;
} SegmentTexture;

/* Variable-length texture list kept in a ModelParticle's data. */
typedef struct {
    /* 0x0 */ u8 count;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ SegmentTexture textures[1];
} ParticleTextureList;

/* Draw handler: bind the texture stored in the particle's data. */
Gfx *particle_bindDataTexture(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    SegmentTexture *tex = (SegmentTexture *)MODEL_PARTICLE_DATA(particle);

    return particle_bindSegmentTexture(gfx, tex->index, tex->segment, tex->arg);
}

/* Draw handler: bind the first texture of the list stored in the particle's data. */
Gfx *particle_bindFirstListTexture(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    SegmentTexture *tex = ((ParticleTextureList *)MODEL_PARTICLE_DATA(particle))->textures;

    return particle_bindSegmentTexture(gfx, tex->index, tex->segment, tex->arg);
}


/* Bind texture gEffectTextures[index] to `segment` (and its palette, at the end of the data, to segment + 1). */
Gfx *particle_bindSegmentTexture(Gfx *gfx, u16 index, u8 segment, s32 arg3) {
    s32 data;
    u8 format;
    u8 unkB;
    TextureDesc *entry;
    s32 size;

    entry = &gEffectTextures[index];
    data = *(s32 *)entry->addr;
    format = entry->fmt;
    unkB = entry->siz;
    data = texture_getAddress(data, &size, arg3, 0);
    gSPSegment(gfx++, segment, data);
    if (format == 2) {
        gSPSegment(gfx++, segment + 1, data + size - ((unkB == 1) ? 0x200 : 0x20));
    }
    return gfx;
}

/* Draw handler: bind every texture of the list stored in the particle's data. */
Gfx *particle_bindListTextures(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    u8 i;
    ParticleTextureList *list;
    SegmentTexture *tex;

    list = (ParticleTextureList *)MODEL_PARTICLE_DATA(particle);
    for (i = 0; i < list->count; i++) {
        tex = &list->textures[i];
        gfx = particle_bindSegmentTexture(gfx, tex->index, tex->segment, tex->arg);
    }
    return gfx;
}
