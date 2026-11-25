#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))

s32 func_151464B8(s16 *arg0);
extern s32 D_800A3880[];
extern s32 D_800DC640[];
s32 *func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5);
s32 func_1510CE60(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4);
void func_15168E54(s32 arg0, s32 arg1);

/* Ground contact written by func_15046C80 (copied whole from the spawner's optional template). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ CollisionTri surface;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ u8 *unk20;
} ParticleFloor; // size 0x24

/* A pooled physics particle (at most 300 live, see D_800DC63C). Bytes 0x10..0x8B are copied from the
 * spawn template passed to func_1513264C; the u8 callback fields index the D_8008xxxx handler tables. */
typedef struct Particle {
    /* 0x00 */ EffectHeader header;
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ Vec3f rot;
    /* 0x02C */ Vec3f scale;
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u16 model;
    /* 0x068 */ u8 variant;
    /* 0x069 */ u8 pad69[3];
    /* 0x06C */ s16 *unk6C;
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 updateFn;
    /* 0x072 */ u8 update2Fn;
    /* 0x073 */ u8 floorHitFn;
    /* 0x074 */ u8 floorType3Fn;
    /* 0x075 */ u8 wallHitFn;
    /* 0x076 */ u8 destroyFn;
    /* 0x077 */ u8 squashFn;
    /* 0x078 */ u8 matrixFn;
    /* 0x079 */ s8 drawFn;
    /* 0x07A */ u8 unk7A;
    /* 0x07B */ u8 pad7B;
    /* 0x07C */ Actor *owner;
    /* 0x080 */ u8 ownerId;
    /* 0x081 */ u8 pad81;
    /* 0x082 */ s16 fadeStart;
    /* 0x084 */ s16 fadeRate;
    /* 0x086 */ u8 pad86[6];
    /* 0x08C */ struct ParticleNode *node;
    /* 0x090 */ Mtx mtx[2];
    /* 0x110 */ ParticleFloor floor;
    /* 0x134 */ Vec3f squashOrigin;
    /* 0x140 */ f32 speed;
    /* 0x144 */ f32 invSpeed;
    /* 0x148 */ u8 hitFlags;
    /* 0x149 */ u8 squashPhase;
    /* 0x14A */ s8 squashAngle;
    /* 0x14B */ u8 pad14B;
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8 unk150;
    /* 0x151 */ u8 pad151[3];
    /* 0x154 */ s32 unk154[4]; // allocations held as s32 (a pointer type changes func_151325C8's code)
    /* 0x164 */ s32 unk164;
    /* 0x168 */ s32 unk168;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ u8 extra[1]; // per-kind data, size passed to func_1513264C
} Particle;

s32 func_15046C80(f32 *pos, s32 arg1, f32 bottom, ParticleFloor *floor);
s32 func_15044380(f32 x, f32 y, f32 z, Actor *probe, s32 arg4, s32 arg5);

/* Collision handlers; each gets the particle's position from before the move. Returning 0 stops the update. */
typedef s32 (*ParticleFloorHitFn)(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight,
                                  CollisionTri *surface);
typedef s32 (*ParticleFloorType3Fn)(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
typedef s32 (*ParticleWallHitFn)(Particle *particle, Actor *probe, f32 x, f32 y, f32 z);

extern ParticleFloorHitFn D_80089934[];
extern ParticleFloorType3Fn D_80089970[];
extern ParticleWallHitFn D_80089974[];


typedef s32 (*ParticleUpdateFn)(Particle *);
typedef void (*ParticleDestroyFn)(Particle *);

extern ParticleUpdateFn D_800898B0[];
extern ParticleUpdateFn D_80089914[];
extern ParticleDestroyFn D_8008997C[];
extern ParticleUpdateFn D_80089988[];
s32 func_1514672C(f32 *pos);

/* Per-frame update: run the particle's update callbacks, age and fade it, and destroy it when any step says so. */
void func_151321D0(Particle *particle) {
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
void func_151323AC(Particle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    D_800899B0[variant](particle);
}

extern ParticleDestroyFn D_800899D4[];

/* Dispatch to the variant's handler in D_800899D4 (variant 0 unless flag 0x100 is set). */
void func_151323F8(Particle *particle) {
    s32 variant;

    variant = (particle->flags & 0x100) ? particle->variant : 0;
    D_800899D4[variant](particle);
}

void func_1510D630(s32);
void func_151325C8(Particle *particle);

/* Release a particle's model reference: drop the per-model count, and once no particle uses the model
 * (and it is not marked to stay resident) unlink its node and free the model. */
void func_15132444(Particle *particle) {
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

void func_15132570(Particle *particle) {
    func_15132444(particle);
    func_15169804(&particle->header);
}

void func_1513259C(Particle *particle) {
    func_15132444(particle);
    func_15169824(&particle->header);
}

/* Free the buffers func_1513264C allocated into unk154[0..D_80082FA0] and unk164. */
void func_151325C8(Particle *particle) {
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

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_15168A9C(void *);
extern void func_10004074(void *);
extern s32 func_1515D480(s32);
extern s32 func_1515D440(void);
s32 func_151336A8(s32 model, ParticleNode *node, Particle *particle);

/* Spawn a particle from a template: allocate it (failing past 300 live), find or load the shared node for
 * its model, copy the template, reset collision state (or copy `floor`) and allocate its buffers. */
Particle *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, ParticleFloor *floor, s32 extraSize, u8 arg5,
                        s32 arg6) {
    Particle *particle;
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
                if (D_800BE9F0 == 0x3B || D_800BE9F0 == 6 || D_800BE9F0 == 0x13 || D_800BE616 != 0 ||
                    D_800BE9F0 == 2) {
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
        particle->floor.unk20 = NULL;
        particle->floor.type = 0;
        particle->floor.unk1C = 0;
        particle->floor.unk18 = 0;
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

Particle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5) {
    return func_1513264C(spawn, arg1, arg2, NULL, extraSize, arg4, arg5);
}

typedef s32 (*ParticleMatrixFn)(Mtx *mtx, Particle *particle);
extern ParticleMatrixFn D_8008998C[];

/* Rebuild this frame's matrix through the matrixFn handler; flag 0x40000 says the particle is drawable.
 * A handler result of -1 destroys the particle. */
void func_15132A88(Particle *particle) {
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

extern Gfx *(*D_800899A4[])(Gfx *gfx, Particle *particle, s16 arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 setGeometryMode, s32 clearGeometryMode);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 alpha, u8 *state);
Gfx *func_151462C8(Gfx *gfx, s32 *arg1, u8 arg2, Actor *owner, u8 ownerId, s16 arg5, f32 *pos, u8 arg7,
                   s32 arg8);

/* Draw a particle whose matrix func_15132A88 built this frame: segment 8, render and geometry mode (lit when
 * flag 0x800), optional extra commands from drawFn, then the matrix and the model's display list. */
Gfx *func_15132B80(Gfx *gfx, Particle *arg1, s16 arg2) {
    Particle *particle;
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
                            &particle->pos.x, (particle->flags & 0x1000) ? 1 : 0, particle->floor.unk18);
    }
    gSPMatrix(gfx++, &particle->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gfx++, *particle->node->resource);
    return gfx;
}

/* Move a physics particle (gravity, velocity, spin) and collide it with the floor and walls, recording
 * hits in hitFlags and calling the hit handlers. Returns 0 if a handler ended the update. */
s32 func_15132DDC(Particle *particle) {
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
            if (particle->floor.type == 1 || (particle->floor.type == 2 && (particle->floor.unk1C & 1))) {
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
                                                         &particle->floor.surface) == 0) {
                        return 0;
                    }
                }

                if (particle->flags & 0x10) {
                    memcpy(&particle->squashOrigin, &particle->pos, sizeof(Vec3f));
                }
            }

            if (particle->floor.type == 3) {
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
            probe.interaction_state = 4;
            probe.x_position = particle->pos.x;
            probe.y_position = particle->pos.y;
            probe.z_position = particle->pos.z;
            probe.unk188 = 0;
            probe.floorTri = particle->floor.surface;
            probe.unk28 = 1.0f;
            probe.xz_scale = 1.0f;
            probe.bodyRadius = particle->radius;
            probe.y_scale = 1.0f;
            probe.bodyHeight = particle->radius;
            if (particle->floor.unk1C & 2) {
                probe.floorY = particle->floor.height;
            } else {
                probe.floorY = -10000.0f;
            }
            probe.unkF8 = 0;

            if (func_15044380(prev.x, prev.y, prev.z, &probe, 0, 0) != 0) {
                particle->hitFlags |= 4;
                if (particle->flags & 0x10) {
                    speed = sqrtf(particle->vel.x * particle->vel.x + particle->vel.y * particle->vel.y +
                                  particle->vel.z * particle->vel.z);
                    if (speed > 1.0f) {
                        particle->hitFlags |= 0x10;
                        particle->speed = speed;
                        particle->invSpeed = 1.0f / speed;
                        particle->squashAngle = func_150484A0(probe.x_position - particle->pos.x,
                                                              probe.z_position - particle->pos.z) *
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
s32 func_151332DC(Particle *particle) {
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

void func_151424F4(Mtx *mtx, f32 sizeXZ, f32 sizeY, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY,
                   f32 scaleZ, f32 posX, f32 posY, f32 posZ);

/* Matrix handler: rotation, per-axis scale times size, and translation. */
s32 func_15133510(Mtx *mtx, Particle *particle) {
    func_151424F4(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->scale.x, particle->scale.y, particle->scale.z, particle->pos.x, particle->pos.y,
                  particle->pos.z);
    return 1;
}

typedef void (*ParticleEventFn)(Particle *particle, void *data, u8 event);
extern ParticleEventFn D_800899F8[];
void func_151B9660(Particle *particle);

/* Event handler: follow an owner swap (0x2D), forget a removed owner (0), then pass the event on to the
 * variant's handler in D_800899F8. */
void func_15133588(Particle *particle, void *data, u8 event) {
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
s32 func_151336A8(s32 model, ParticleNode *node, Particle *particle) {
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

void func_15142838(Mtx *mtx, f32 sizeXZ, f32 sizeY, f32 rotX, f32 rotY, f32 rotZ, f32 posX, f32 posY, f32 posZ);

/* Matrix handler: rotation scaled by size only, and translation. */
s32 func_15133760(Mtx *mtx, Particle *particle) {
    func_15142838(mtx, particle->sizeXZ, particle->sizeY, particle->rot.x, particle->rot.y, particle->rot.z,
                  particle->pos.x, particle->pos.y, particle->pos.z);
    return 1;
}

/* Ballistic step: integrate position under gravity (y gains vel.y*dt + gravity*dt*dt/2), then velocity and spin. */
s32 func_151337C0(Particle *particle) {
    particle->pos.x += particle->vel.x * D_800BE9A4;
    particle->pos.y += (particle->vel.y * D_800BE9A4) + (((particle->gravity * D_800BE9A4) * D_800BE9A4) * 0.5f);
    particle->pos.z += particle->vel.z * D_800BE9A4;
    particle->vel.y += particle->gravity * D_800BE9A4;
    particle->rot.x += particle->rotVel.x * D_800BE9A4;
    particle->rot.y += particle->rotVel.y * D_800BE9A4;
    particle->rot.z += particle->rotVel.z * D_800BE9A4;
    return 1;
}

/* Ballistic update with floor collision only while falling; the floor handler runs unless the floor's
 * unk20 record is flagged (bits 0x60 of byte 0x4F == 0x40). */
s32 func_15133894(Particle *particle) {
    s8 result;
    Vec3f prev;
    Vec3f test;
    u8 *unk20;
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
            if (particle->floor.unk20 != NULL) {
                unk20 = particle->floor.unk20;
                if ((unk20[0x4F] & 0x60) == 0x40) {
                    skip = TRUE;
                }
            }
            if (!skip) {
                result = D_80089934[particle->floorHitFn](particle, prev.x, prev.y, prev.z, particle->floor.height,
                                                          &particle->floor.surface);
            }
        }
    }
    return result;
}

/* Floor handler: rest on the floor and bounce, scaling velocity and spin by elasticity. */
s32 func_151339D4(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
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
s32 func_15133A50(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight) {
    particle->pos.y = particle->radius + floorHeight;
    particle->vel.x = 0.0f;
    particle->vel.y = 0.0f;
    particle->vel.z = 0.0f;
    particle->rotVel.x = 0.0f;
    particle->rotVel.y = 0.0f;
    particle->rotVel.z = 0.0f;
    return 1;
}

/* Matrix handler for particles attached to their owner: use the owner's float matrix number extra[0]
 * (as is when D_800C3E90 is set, else converted to fixed point). Returns -1 if the owner is gone. */
s32 func_15133A94(Mtx *mtx, Particle *particle) {
    Actor *owner;
    MtxF *ownerMtxs;
    MtxF mf;
    s32 pad; // matching: unused stack slot

    owner = particle->owner;
    if (owner == NULL || owner->interaction_state == 0 || owner->id == 0xFF ||
        owner->unique_id != particle->ownerId) {
        return -1;
    }
    ownerMtxs = owner->unk1D4;
    if (ownerMtxs == NULL) {
        return 0;
    }
    if ((owner->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800C3E90) {
        memcpy(mtx, ownerMtxs[particle->extra[0]], sizeof(mf));
        return 1;
    }
    memcpy(mf, ownerMtxs[particle->extra[0]], sizeof(mf));
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
    guMtxF2L(mf, mtx);
    return 1;
}

/* Floor handler: bounce like func_151339D4, and come to rest (clearing the motion flags) once the
 * rebound is slower than 4. */
s32 func_15133B98(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
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
s32 func_15133C58(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 9U) + 0x2DE, 12000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Floor handler: func_15133B98, plus (half the time) a random sound 0x1B8..0x1C1 at the hit position. */
s32 func_15133D20(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 10U) + 0x1B8, 24000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Event handler: destroy the particle when its owner goes away (event 0). */
void func_15133DE8(Particle *particle, ActorRef *data, u8 event) {
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
} ParticleTexture;

/* Variable-length texture list kept in Particle.extra. */
typedef struct {
    /* 0x0 */ u8 count;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ ParticleTexture textures[1];
} ParticleTextureList;

Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3);

/* Draw handler: bind the texture stored at extra. */
Gfx *func_15133E84(Gfx *gfx, Particle *particle, s16 arg2) {
    ParticleTexture *tex = (ParticleTexture *)particle->extra;

    return func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
}

/* Draw handler: bind the first texture of the list stored at extra. */
Gfx *func_15133EB8(Gfx *gfx, Particle *particle, s16 arg2) {
    ParticleTexture *tex = ((ParticleTextureList *)particle->extra)->textures;

    return func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
}

typedef struct {
    /* 0x0 */ s32 *unk0;
    /* 0x4 */ u8 pad4[6];
    /* 0xA */ u8 unkA; // 2: a palette follows the texels
    /* 0xB */ u8 unkB; // palette size: 1 -> 0x200 bytes, else 0x20
} TextureEntry; // size 0xC

extern TextureEntry D_80090B60[];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

/* Bind texture D_80090B60[index] to `segment` (and its palette, at the end of the data, to segment + 1). */
Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3) {
    s32 data;
    u8 unkA;
    u8 unkB;
    TextureEntry *entry;
    s32 size;

    entry = &D_80090B60[index];
    data = *entry->unk0;
    unkA = entry->unkA;
    unkB = entry->unkB;
    data = func_1510D0EC(data, &size, arg3, 0);
    gSPSegment(gfx++, segment, data);
    if (unkA == 2) {
        gSPSegment(gfx++, segment + 1, data + size - ((unkB == 1) ? 0x200 : 0x20));
    }
    return gfx;
}

/* Draw handler: bind every texture of the list stored at extra. */
Gfx *func_15133FD8(Gfx *gfx, Particle *particle, s16 arg2) {
    u8 i;
    ParticleTextureList *list;
    ParticleTexture *tex;

    list = (ParticleTextureList *)particle->extra;
    for (i = 0; i < list->count; i++) {
        tex = &list->textures[i];
        gfx = func_15133EEC(gfx, tex->index, tex->segment, tex->arg);
    }
    return gfx;
}
