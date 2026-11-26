#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

extern s32 D_800A3880[];
extern s32 D_800DC640[];


/* Collision handlers; each gets the particle's position from before the move. Returning 0 stops the update. */
typedef s32 (*ParticleFloorHitFn)(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight,
                                  CollisionTri *surface);
typedef s32 (*ParticleFloorType3Fn)(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
typedef s32 (*ParticleWallHitFn)(ModelParticle *particle, Actor *probe, f32 x, f32 y, f32 z);

extern ParticleFloorHitFn D_80089934[];
extern ParticleFloorType3Fn D_80089970[];
extern ParticleWallHitFn D_80089974[];


typedef s32 (*ParticleUpdateFn)(ModelParticle *);
typedef void (*ParticleDestroyFn)(ModelParticle *);

extern ParticleUpdateFn D_800898B0[];
extern ParticleUpdateFn D_80089914[];
extern ParticleDestroyFn D_8008997C[];
extern ParticleUpdateFn D_80089988[];

/* Per-frame update: run the particle's update callbacks, age and fade it, and destroy it when any step says so. */
void func_151321D0(ModelParticle *particle) {
    s32 result;
    u8 dead;

    dead = FALSE;
    particle->flags &= ~0x200000;
    if ((particle->flags & 0x10) && (particle->hitFlags & 0x18)) {
        result = D_80089988[particle->squashFn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    } else if (particle->updateFn != 0) {
        result = D_800898B0[particle->updateFn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    }

    if (particle->update2Fn != 0 && !dead) {
        result = D_80089914[particle->update2Fn](particle);
        if (result == 0) {
            dead = TRUE;
        }
    }

    if ((particle->flags & 0x80) && !dead) {
        particle->lifetime -= D_800BE9E4;
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
        dead = func_1514672C(&particle->pos.x) == 0;
    }

    if (dead) {
        if (particle->destroyFn != 0) {
            D_8008997C[particle->destroyFn](particle);
        }
        func_1516972C(&particle->header);
    }
}

extern ParticleDestroyFn D_800899B0[];

/* Dispatch to the variant's handler in D_800899B0 (variant 0 unless flag 0x100 is set). */
void func_151323AC(ModelParticle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    D_800899B0[variant](particle);
}

extern ParticleDestroyFn D_800899D4[];

/* Dispatch to the variant's handler in D_800899D4 (variant 0 unless flag 0x100 is set). */
void func_151323F8(ModelParticle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    D_800899D4[variant](particle);
}

/* Release a particle's model reference: drop the per-model count, and once no particle uses the model
 * (and it is not marked to stay resident) unlink its node and free the model. */
void func_15132444(ModelParticle *particle) {
    ParticleNode *node;

    D_800DC63C--;
    D_800DC468[particle->model]--;
    if (D_800DC468[particle->model] == 0) {
        if (particle->node->keepLoaded) {
            D_800DC468[particle->model] = 1;
        } else {
            func_1510D630(D_800DC640[particle->model]);
            node = particle->node;
            if (node->prev == NULL) {
                D_800DC460 = node->next;
                if (D_800DC460 != NULL) {
                    D_800DC460->prev = NULL;
                } else {
                    D_800DC464 = NULL;
                }
            } else {
                node->prev->next = node->next;
                node = particle->node;
                if (node->next != NULL) {
                    node->next->prev = node->prev;
                } else {
                    D_800DC464 = node->prev;
                }
            }
            func_100043B4(particle->node->resource, 4);
            func_100043B4((s32 *)particle->node, 4);
        }
    }
    func_151325C8(particle);
}

void func_15132570(ModelParticle *particle) {
    func_15132444(particle);
    func_15169804(&particle->header);
}

void func_1513259C(ModelParticle *particle) {
    func_15132444(particle);
    func_15169824(&particle->header);
}

/* Free the buffers func_1513264C allocated into unk154[0..D_80082FA0] and unk164. */
void func_151325C8(ModelParticle *particle) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (particle->unk154[i] != 0) {
            func_100043B4((s32 *)particle->unk154[i], 4);
        }
    }

    if (particle->unk164 != 0) {
        func_100043B4((s32 *)particle->unk164, 4);
    }
}

/* Spawn a particle from a template: allocate it (failing past 300 live), find or load the shared node for
 * its model, copy the template, reset collision state (or copy `floor`) and allocate its buffers. */
void *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, CollisionHit *floor, s32 extraSize, u8 arg5,
                    s32 arg6) {
    ModelParticle *particle;
    ParticleNode *node;
    s32 i;

    if (D_800DC63C > 300) {
        return NULL;
    }
    particle = func_15167A68((spawn->flags & 0x4000) ? 0x48 : 0x19, arg6, extraSize + 0x170, 1, arg5,
                             (spawn->flags & 0x400000) ? 2 : 1);
    if (particle == NULL) {
        return NULL;
    }
    if (D_800DC468[spawn->model] == 0) {
        node = allocate_memory(sizeof(ParticleNode), 1, 2, (spawn->flags & 0x400000) ? 2 : 1);
        if (node == NULL) {
            func_15168A9C(particle);
            func_10004074(particle);
            return NULL;
        }
        if (func_151336A8(spawn->model, node, particle)) {
            node->next = D_800DC460;
            if (node->next != NULL) {
                node->next->prev = node;
            } else {
                D_800DC464 = node;
            }
            D_800DC460 = node;
            node->prev = NULL;
            node->model = spawn->model;
            if (spawn->flags & 0x100000) {
                if (level == 0x3B || level == 6 || level == 0x13 || D_800BE616 != 0 ||
                    level == 2) {
                    node->keepLoaded = FALSE;
                } else {
                    node->keepLoaded = TRUE;
                }
            } else {
                node->keepLoaded = FALSE;
            }
        } else {
            func_15168A9C(particle);
            func_10004074(particle);
            func_10004074(node);
            return NULL;
        }
    } else {
        s32 tries;

        node = D_800DC460;
        tries = 100;
        while (node->model != spawn->model) {
            tries--;
            node = node->next;
            if (tries <= 0) {
                break;
            }
        }
        if (tries <= 0) {
            func_15168A9C(particle);
            func_10004074(particle);
            return NULL;
        }
    }
    D_800DC468[spawn->model]++;
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
    D_800DC63C++;
    particle->unk150 = 0;
    particle->unk14C = arg1;
    particle->unk168 = arg2;
    for (i = 0; i < ARRAY_COUNT(particle->unk154); i++) {
        particle->unk154[i] = 0;
    }
    particle->unk164 = 0;
    if (arg1 != 0) {
        for (i = 0; i <= D_80082FA0; i++) {
            particle->unk154[i] = func_1515D480(arg1);
        }
        particle->unk164 = func_1515D440();
    }
    particle->flags &= ~0x200000;
    return particle;
}

void *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5) {
    return func_1513264C(spawn, arg1, arg2, NULL, extraSize, arg4, arg5);
}

typedef s32 (*ParticleMatrixFn)(Mtx *mtx, ModelParticle *particle);
extern ParticleMatrixFn D_8008998C[];

/* Rebuild this frame's matrix through the matrixFn handler; flag 0x40000 says the particle is drawable.
 * A handler result of -1 destroys the particle. */
void func_15132A88(ModelParticle *particle) {
    s16 *unk6C;
    s32 result;

    particle->flags &= ~0x200000;
    if (particle->flags & 0x20000) {
        return;
    }

    if (particle->flags & 0x400) {
        unk6C = particle->unk6C;
        if (unk6C != NULL && func_151464B8(unk6C) != 0) {
            return;
        }
    }

    if (particle->flags & 0x80000) {
        particle->flags |= 0x200000;
        return;
    }

    result = D_8008998C[particle->matrixFn](&particle->mtx[D_800BE9C0], particle);
    if (result == -1) {
        func_1516972C(&particle->header);
        return;
    }

    if (result == 0) {
        particle->flags &= ~0x40000;
    } else {
        particle->flags |= 0x240000;
    }
}

extern Gfx *(*D_800899A4[])(Gfx *gfx, ModelParticle *particle, s16 arg2);

/* Draw a particle whose matrix func_15132A88 built this frame: segment 8, render and geometry mode (lit when
 * flag 0x800), optional extra commands from drawFn, then the matrix and the model's display list. */
Gfx *func_15132B80(Gfx *gfx, ModelParticle *arg1, s16 arg2) {
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
        gSPSegment(gfx++, 8, D_80083740);
    } else {
        gSPSegment(gfx++, 8, D_800838C0);
    }
    if (particle->flags & 0x10000) {
        if (particle->drawFn != -1) {
            gfx = D_800899A4[particle->drawFn](gfx, particle, arg2);
        }
    }
    gfx = func_15142C10(gfx, 0, 0, 0, particle->alpha, &state);
    gfx = func_15142B7C(gfx,
                        ((particle->flags & 0x800) ? G_LIGHTING : 0) | ((particle->flags & 0x1000) ? 0x400000 : 0) |
                            G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH,
                        ((particle->flags & 0x800) ? 0 : G_LIGHTING) | ((particle->flags & 0x1000) ? 0 : 0x400000) |
                            G_CULL_BOTH | G_FOG | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR);
    if (particle->flags & 0x800) {
        gfx = func_151462C8(gfx, &particle->unk14C, particle->unk7A, particle->owner, particle->ownerId, arg2,
                            &particle->pos.x, (particle->flags & 0x1000) ? 1 : 0, particle->floor.material);
    }
    gSPMatrix(gfx++, &particle->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, *particle->node->resource);
    return gfx;
}

/* Move a physics particle (gravity, velocity, spin) and collide it with the floor and walls, recording
 * hits in hitFlags and calling the hit handlers. Returns 0 if a handler ended the update. */
s32 func_15132DDC(ModelParticle *particle) {
    Vec3f prev;
    Vec3f test;
    s32 probePad; // matching: stack slot next to the probe
    Actor probe; // a stand-in actor for the wall probe
    f32 speed;

    if (particle->flags & 7) {
        memcpy(&prev, &particle->pos, sizeof(Vec3f));
    }

    if (particle->flags & 8) {
        particle->vel.y += particle->gravity * D_800BE9A4;
    }

    if (particle->flags & 0x20) {
        particle->pos.x += particle->vel.x * D_800BE9A4;
        particle->pos.y += particle->vel.y * D_800BE9A4;
        particle->pos.z += particle->vel.z * D_800BE9A4;
    }

    if (particle->flags & 0x40) {
        particle->rot.x += particle->rotVel.x * D_800BE9A4;
        particle->rot.y += particle->rotVel.y * D_800BE9A4;
        particle->rot.z += particle->rotVel.z * D_800BE9A4;
    }

    if (particle->flags & 7) {
        test.x = particle->pos.x;
        test.y = prev.y;
        test.z = particle->pos.z;

        if (func_15046C80(&test.x, 0, particle->pos.y - particle->radius, &particle->floor) != 0) {
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
                    if (D_80089934[particle->floorHitFn](particle, prev.x, prev.y, prev.z, particle->floor.height,
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
                    if (D_80089970[particle->floorType3Fn](particle, prev.x, prev.y, prev.z,
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

            if (func_15044380(prev.x, prev.y, prev.z, &probe, 0, 0) != 0) {
                particle->hitFlags |= 4;
                if (particle->flags & 0x10) {
                    speed = sqrtf(particle->vel.x * particle->vel.x + particle->vel.y * particle->vel.y +
                                  particle->vel.z * particle->vel.z);
                    if (speed > 1.0f) {
                        particle->hitFlags |= 0x10;
                        particle->speed = speed;
                        particle->invSpeed = 1.0f / speed;
                        particle->squashAngle = func_150484A0(probe.xpos - particle->pos.x,
                                                              probe.zpos - particle->pos.z) *
                                                40.74366379f; // 256 / (2 * pi): radians to a byte angle
                    }
                }

                if (particle->wallHitFn != 0) {
                    if (D_80089974[particle->wallHitFn](particle, &probe, prev.x, prev.y, prev.z) == 0) {
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
s32 func_151332DC(ModelParticle *particle) {
    s32 pad; // matching: unused stack slot
    f32 amplitude;
    f32 cosine;
    f32 sine;

    particle->scale.x = 1.0f;
    particle->scale.y = 1.0f;
    particle->scale.z = 1.0f;
    if (particle->flags & 0x40) {
        particle->rot.x += particle->rotVel.x * D_800BE9A4;
        particle->rot.y += particle->rotVel.y * D_800BE9A4;
        particle->rot.z += particle->rotVel.z * D_800BE9A4;
    }
    particle->squashPhase += (s32)(576.0f * particle->invSpeed) * D_800BE9E4;
    if (particle->squashPhase < 0x80) {
        amplitude = func_151423D8(particle->squashPhase - 0x40);
        amplitude = (1.0f - particle->elasticity) * (1.0f - particle->invSpeed) * amplitude;
        if (particle->hitFlags & 8) {
            particle->scale.x *= 1.0f + amplitude;
            particle->scale.y *= 1.0f - amplitude;
            particle->scale.z *= 1.0f + amplitude;
            particle->pos.y = particle->squashOrigin.y - amplitude * particle->radius;
        }
        if (particle->hitFlags & 0x10) {
            cosine = func_151423D8(particle->squashAngle - 0x40);
            sine = func_151423D8(particle->squashAngle);
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
s32 func_15133510(Mtx *mtx, ModelParticle *particle) {
    func_151424F4(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->scale.x, particle->scale.y, particle->scale.z, particle->pos.x, particle->pos.y,
                  particle->pos.z);
    return 1;
}

typedef void (*ParticleEventFn)(ModelParticle *particle, void *data, u8 event);
extern ParticleEventFn D_800899F8[];

/* Event handler: follow an owner swap (0x2D), forget a removed owner (0), then pass the event on to the
 * variant's handler in D_800899F8. */
void func_15133588(ModelParticle *particle, void *data, u8 event) {
    s32 variant;
    ParticleEventFn handler;
    Actor *obj;

    if (event == 0x1B && particle->model == 0x22) {
        func_151B9660(particle);
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
    handler = D_800899F8[variant];
    if (handler != NULL) {
        handler(particle, data, event);
    }
}

/* Load model `model` (D_800A3880 table, bank 9) into the node and prepare it; returns FALSE if the load fails. */
s32 func_151336A8(s32 model, ParticleNode *node, ModelParticle *particle) {
    s32 out1;
    s32 out2;

    node->resource = func_1502B6BC(&out1, 0, &out2, 2, 9, D_800A3880[model]);
    if (node->resource == NULL) {
        return FALSE;
    }
    func_1510CE60(*node->resource, 0, 1, 0x3E, &D_800DC640[model]);
    func_15168E54(*node->resource, (s32)node->resource);
    return TRUE;
}

/* Matrix handler: rotation scaled by size only, and translation. */
s32 func_15133760(Mtx *mtx, ModelParticle *particle) {
    func_15142838(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->pos.x, particle->pos.y, particle->pos.z);
    return 1;
}

/* Ballistic step: integrate position under gravity (y gains vel.y*dt + gravity*dt*dt/2), then velocity and spin. */
s32 func_151337C0(ModelParticle *particle) {
    particle->pos.x += particle->vel.x * D_800BE9A4;
    particle->pos.y += (particle->vel.y * D_800BE9A4) + (((particle->gravity * D_800BE9A4) * D_800BE9A4) * 0.5f);
    particle->pos.z += particle->vel.z * D_800BE9A4;
    particle->vel.y += particle->gravity * D_800BE9A4;
    particle->rot.x += particle->rotVel.x * D_800BE9A4;
    particle->rot.y += particle->rotVel.y * D_800BE9A4;
    particle->rot.z += particle->rotVel.z * D_800BE9A4;
    return 1;
}

/* Ballistic update with floor collision only while falling; the floor handler runs unless the floor is an
 * object surface flagged (flags & 0x60) == 0x40. */
s32 func_15133894(ModelParticle *particle) {
    s8 result;
    Vec3f prev;
    Vec3f test;
    WorldObject *surface;
    s32 skip;

    result = 1;
    prev = particle->pos;
    func_151337C0(particle);
    if ((particle->flags & 1) && particle->floorHitFn != 0 && particle->pos.y < prev.y) {
        test.x = particle->pos.x;
        test.y = particle->radius + prev.y;
        test.z = particle->pos.z;
        if (func_15046C80(&test.x, 0, particle->pos.y - particle->radius, &particle->floor) != 0) {
            skip = FALSE;
            if (particle->floor.surface != NULL) {
                surface = particle->floor.surface;
                if ((surface->object_status & 0x60) == 0x40) {
                    skip = TRUE;
                }
            }
            if (!skip) {
                result = D_80089934[particle->floorHitFn](particle, prev.x, prev.y, prev.z, particle->floor.height,
                                                          &particle->floor.tri);
            }
        }
    }
    return result;
}

/* Floor handler: rest on the floor and bounce, scaling velocity and spin by elasticity. */
s32 func_151339D4(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
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
s32 func_15133A50(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
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
 * (as is when D_800C3E90 is set, else converted to fixed point). Returns -1 if the owner is gone. */
s32 func_15133A94(Mtx *mtx, ModelParticle *particle) {
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
    if ((owner->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800C3E90) {
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

/* Floor handler: bounce like func_151339D4, and come to rest (clearing the motion flags) once the
 * rebound is slower than 4. */
s32 func_15133B98(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
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

/* Floor handler: func_15133B98, plus (half the time) a random sound 0x2DE..0x2E6 at the hit position. */
s32 func_15133C58(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 9U) + 0x2DE, 12000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Floor handler: func_15133B98, plus (half the time) a random sound 0x1B8..0x1C1 at the hit position. */
s32 func_15133D20(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 10U) + 0x1B8, 24000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Event handler: destroy the particle when its owner goes away (event 0). */
void func_15133DE8(ModelParticle *particle, ActorRef *data, u8 event) {
    Actor *obj;

    if (event == 0) {
        obj = data->actor;
        if (obj == particle->owner || particle->ownerId == data->uniqueId) {
            func_1516972C(&particle->header);
        }
    }
}

/* Call func_15169260 for both particle object kinds (0x19 and 0x48, see func_1513264C). */

void func_15133E3C(s32 arg0, u8 arg1) {
    const s32 kinds[2] = { 0x19, 0x48 };

    func_15169260(kinds, ARRAY_COUNT(kinds), arg0, arg1);
}

/* A texture to bind to a segment: D_80090B60 index, segment number, and func_1510D0EC's third argument. */
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
Gfx *func_15133E84(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    SegmentTexture *tex = (SegmentTexture *)MODEL_PARTICLE_DATA(particle);

    return func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
}

/* Draw handler: bind the first texture of the list stored in the particle's data. */
Gfx *func_15133EB8(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    SegmentTexture *tex = ((ParticleTextureList *)MODEL_PARTICLE_DATA(particle))->textures;

    return func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
}

extern EffectTexture D_80090B60[];

/* Bind texture D_80090B60[index] to `segment` (and its palette, at the end of the data, to segment + 1). */
Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3) {
    s32 data;
    u8 format;
    u8 unkB;
    EffectTexture *entry;
    s32 size;

    entry = &D_80090B60[index];
    data = *entry->frames;
    format = entry->format;
    unkB = entry->paletteSize;
    data = func_1510D0EC(data, &size, arg3, 0);
    gSPSegment(gfx++, segment, data);
    if (format == 2) {
        gSPSegment(gfx++, segment + 1, data + size - ((unkB == 1) ? 0x200 : 0x20));
    }
    return gfx;
}

/* Draw handler: bind every texture of the list stored in the particle's data. */
Gfx *func_15133FD8(Gfx *gfx, ModelParticle *particle, s16 arg2) {
    u8 i;
    ParticleTextureList *list;
    SegmentTexture *tex;

    list = (ParticleTextureList *)MODEL_PARTICLE_DATA(particle);
    for (i = 0; i < list->count; i++) {
        tex = &list->textures[i];
        gfx = func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
    }
    return gfx;
}
