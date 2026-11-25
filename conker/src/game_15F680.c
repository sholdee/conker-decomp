#include <ultra64.h>
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_15169260
/* variables.h declares the D_800DC460/D_800DC464 list head/tail as s32 and D_800DC468 as u16;
 * func_15132444 walks them as node pointers and reads D_800DC468 with lh, so the real types
 * are made file-local here (the shared header is left alone). */
#define D_800DC460 D_800DC460_s32_decl_in_variables_h
#define D_800DC464 D_800DC464_s32_decl_in_variables_h
#define D_800DC468 D_800DC468_u16_decl_in_variables_h
#include "variables.h"
#undef D_800DC460
#undef D_800DC464
#undef D_800DC468
/* One loaded particle model, shared by every live particle that uses it (D_800DC468 counts them). */
typedef struct ParticleNode {
    /* 0x0 */ s32 *resource;
    /* 0x4 */ struct ParticleNode *next;
    /* 0x8 */ struct ParticleNode *prev;
    /* 0xC */ u16 model;
    /* 0xE */ u8 keepLoaded;
} ParticleNode;

extern ParticleNode *D_800DC460;
extern ParticleNode *D_800DC464;
extern s16 D_800DC468[];

s32 func_151464B8(s16 *arg0);
extern u8 D_800C3E90;
extern s32 D_800A3880[];
extern s32 D_800DC640[];
s32 *func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4, s32 arg5);
s32 func_1510CE60(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4);
void func_15168E54(s32 arg0, s32 arg1);

/* Surface record of the floor under a particle; only copied around in this file. */
typedef struct {
    u8 unk0[0x10];
    u16 unk10;
} FloorSurface; // size 0x12

/* Ground contact written by func_15046C80 (copied whole from the spawner's optional template). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ FloorSurface surface;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ u8 *unk20;
} ParticleFloor; // size 0x24

/* A pooled physics particle (at most 300 live, see D_800DC63C). Bytes 0x10..0x8B are copied from the
 * spawn template passed to func_1513264C; the u8 callback fields index the D_8008xxxx handler tables. */
typedef struct Particle {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ vertex rot;
    /* 0x02C */ vertex scale;
    /* 0x038 */ vertex pos;
    /* 0x044 */ vertex vel;
    /* 0x050 */ vertex rotVel;
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
    /* 0x07C */ struct127 *owner;
    /* 0x080 */ u8 ownerId;
    /* 0x081 */ u8 pad81;
    /* 0x082 */ s16 fadeStart;
    /* 0x084 */ s16 fadeRate;
    /* 0x086 */ u8 pad86[6];
    /* 0x08C */ struct ParticleNode *node;
    /* 0x090 */ Mtx mtx[2];
    /* 0x110 */ ParticleFloor floor;
    /* 0x134 */ f32 squashOrigin[3];
    /* 0x140 */ f32 speed;
    /* 0x144 */ f32 invSpeed;
    /* 0x148 */ u8 hitFlags;
    /* 0x149 */ u8 squashPhase;
    /* 0x14A */ s8 squashAngle;
    /* 0x14B */ u8 pad14B;
    /* 0x14C */ s32 unk14C;
    /* 0x150 */ u8 unk150;
    /* 0x151 */ u8 pad151[3];
    /* 0x154 */ s32 unk154[4];
    /* 0x164 */ s32 unk164;
    /* 0x168 */ s32 unk168;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ u8 extra[1]; // per-kind data, size passed to func_1513264C
} Particle;

/* Spawn template: the same layout as Particle 0x10..0x8B, copied into the particle by func_1513264C. */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 unk58[0x7C - 0x58];
} ParticleSpawn; // size 0x7C

/* Wall-collision query filled in by the caller and updated by func_15044380. */
typedef struct {
    /* 0x000 */ s32 type;
    /* 0x004 */ u8 pad4[0x10];
    /* 0x014 */ vertex pos;
    /* 0x020 */ u8 pad20[0x8];
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0xB8];
    /* 0x0E4 */ s16 unkE4;
    /* 0x0E6 */ s16 unkE6;
    /* 0x0E8 */ u8 padE8[0x10];
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[0x50];
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ f32 unk150;
    /* 0x154 */ u8 pad154[0x2C];
    /* 0x180 */ f32 floorHeight;
    /* 0x184 */ u8 pad184[0x4];
    /* 0x188 */ s32 unk188;
    /* 0x18C */ FloorSurface surface;
    /* 0x19E */ u8 pad19E[0x192];
} CollisionProbe; // size 0x330

s32 func_15046C80(f32 *pos, s32 arg1, f32 bottom, ParticleFloor *floor);
s32 func_15044380(f32 x, f32 y, f32 z, CollisionProbe *probe, s32 arg4, s32 arg5);

/* Collision handlers; each gets the particle's position from before the move. Returning 0 stops the update. */
typedef s32 (*ParticleFloorHitFn)(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight,
                                  FloorSurface *surface);
typedef s32 (*ParticleFloorType3Fn)(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
typedef s32 (*ParticleWallHitFn)(Particle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z);

extern ParticleFloorHitFn D_80089934[];
extern ParticleFloorType3Fn D_80089970[];
extern ParticleWallHitFn D_80089974[];
extern f32 D_800A386C;
extern f32 D_800A3870;

typedef struct {
    u8 pad[4];
    f32 m[4][4];
} PaddedMtxF15133A94;

typedef struct {
    s32 unk0;
    s32 unk4;
} Data15133E3C;

typedef struct {
    u8 pad_0[0x7C];
    s32 field_0x7C;
    u8 field_0x80;
} InteractionTargetFields;

typedef struct {
    s32 field_0x0;
    u8 field_0x4;
} InteractionKeyFields;

extern Data15133E3C D_800A3860;
void func_15169260(Data15133E3C *, s32, s32, u8);

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
        func_1516972C((struct102 *)particle);
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
    func_15169804((struct102 *)particle);
}

void func_1513259C(Particle *particle) {
    func_15132444(particle);
    func_15169824((struct102 *)particle);
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
    particle->squashOrigin[0] = 0.0f;
    particle->squashOrigin[1] = 0.0f;
    particle->squashOrigin[2] = 0.0f;
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
    for (i = 0; i < 4; i++) {
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
        func_1516972C((struct102 *)particle);
        return;
    }

    if (result == 0) {
        particle->flags &= ~0x40000;
    } else {
        particle->flags |= 0x240000;
    }
}

extern u8 D_80083740[];
extern u8 D_800838C0[];
extern Gfx *(*D_800899A4[])(Gfx *gfx, Particle *particle, s16 arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 setFlags, s32 clearFlags);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 alpha, u8 *state);
Gfx *func_151462C8(Gfx *gfx, s32 *arg1, u8 arg2, struct127 *owner, u8 ownerId, s16 arg5, f32 *pos, u8 arg7,
                   s32 arg8);

/* Draw a particle whose matrix func_15132A88 built this frame: segment 8, render state, optional extra
 * commands from drawFn, then the matrix and the model's display list. */
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
                        ((particle->flags & 0x800) ? 0x20000 : 0) | ((particle->flags & 0x1000) ? 0x400000 : 0) | 1 |
                            4 | 0x200000,
                        ((particle->flags & 0x800) ? 0 : 0x20000) | ((particle->flags & 0x1000) ? 0 : 0x400000) |
                            0x600 | 0x10000 | 0x40000 | 0x80000);
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
    vertex prev;
    vertex test;
    CollisionProbe probe;
    f32 speed;

    if (particle->flags & 7) {
        memcpy(&prev, &particle->pos, sizeof(vertex));
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
                    memcpy(particle->squashOrigin, &particle->pos, sizeof(vertex));
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
            probe.type = 4;
            probe.pos.x = particle->pos.x;
            probe.pos.y = particle->pos.y;
            probe.pos.z = particle->pos.z;
            probe.unk188 = 0;
            probe.surface = particle->floor.surface;
            probe.unk28 = 1.0f;
            probe.unk14C = 1.0f;
            probe.unkE4 = particle->radius;
            probe.unk150 = 1.0f;
            probe.unkE6 = particle->radius;
            if (particle->floor.unk1C & 2) {
                probe.floorHeight = particle->floor.height;
            } else {
                probe.floorHeight = D_800A386C;
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
                        particle->squashAngle = func_150484A0(probe.pos.x - particle->pos.x,
                                                              probe.pos.z - particle->pos.z) *
                                                D_800A3870;
                    }
                }

                if (particle->wallHitFn != 0) {
                    if (D_80089974[particle->wallHitFn](particle, &probe, prev.x, prev.y, prev.z) == 0) {
                        return 0;
                    }
                }

                if (particle->flags & 0x10) {
                    memcpy(particle->squashOrigin, &particle->pos, sizeof(vertex));
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
            particle->pos.y = particle->squashOrigin[1] - amplitude * particle->radius;
        }
        if (particle->hitFlags & 0x10) {
            cosine = func_151423D8(particle->squashAngle - 0x40);
            sine = func_151423D8(particle->squashAngle);
            particle->scale.x *= 1.0f - amplitude;
            particle->scale.y *= 1.0f + amplitude;
            particle->scale.z *= 1.0f - amplitude;
            particle->pos.x = particle->squashOrigin[0] - (amplitude * particle->radius) * sine;
            particle->pos.z = particle->squashOrigin[2] + (amplitude * particle->radius) * cosine;
        // matching: an (unused) label on the early return keeps as1 from filling its delay slot
        early_return:
            return 1;
        }
    } else {
        memcpy(&particle->pos, particle->squashOrigin, sizeof(vertex));
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

/* Event 0 payload: an object going away (also handled by func_15133DE8). */
typedef struct {
    struct127 *obj;
    u8 id;
} ParticleEventRemove;

/* Event 0x2D payload: two objects trading places. */
typedef struct {
    struct127 *a;
    struct127 *b;
    u8 aId;
    u8 bId;
} ParticleEventSwap;

typedef void (*ParticleEventFn)(Particle *particle, void *data, u8 event);
extern ParticleEventFn D_800899F8[];
void func_151B9660(Particle *particle);

/* Event handler: follow an owner swap (0x2D), forget a removed owner (0), then pass the event on to the
 * variant's handler in D_800899F8. */
void func_15133588(Particle *particle, void *data, u8 event) {
    s32 pad[2]; // matching: unused stack slots
    s32 variant;
    ParticleEventFn handler;
    struct127 *obj;

    if (event == 0x1B && particle->model == 0x22) {
        func_151B9660(particle);
    }
    if (event == 0x2D) {
        obj = ((ParticleEventSwap *)data)->a;
        if (obj == particle->owner) {
            particle->owner = ((ParticleEventSwap *)data)->b;
            particle->ownerId = ((ParticleEventSwap *)data)->bId;
        } else if (particle->owner == ((ParticleEventSwap *)data)->b) {
            particle->owner = obj;
            particle->ownerId = ((ParticleEventSwap *)data)->aId;
        // matching: an (unused) label here keeps as1 from filling this branch's delay slot
        swapped:;
        }
    } else if (event == 0) {
        obj = ((ParticleEventRemove *)data)->obj;
        if (obj == particle->owner || ((ParticleEventRemove *)data)->id == particle->ownerId) {
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
    vertex prev;
    vertex test;
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

s32 func_15133A94(Mtx *arg0, u8 *arg1) {
    struct127 *obj;
    struct255 *mtx;
    PaddedMtxF15133A94 sp18;
    u8 *src;

    obj = *(struct127 **)(arg1 + 0x7C);
    if ((obj == NULL) || (obj->interaction_state == 0) || (obj->id == 0xFF) ||
        (arg1[0x80] != *(u8 *)((u8 *)obj + 0x3B))) {
        return -1;
    }
    mtx = obj->unk1D4;
    if (mtx == NULL) {
        return 0;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800C3E90) {
        src = (u8 *)mtx;
        src += arg1[0x170] << 6;
        memcpy(arg0, src, 0x40);
        return 1;
    }
    src = (u8 *)mtx;
    src += arg1[0x170] << 6;
    memcpy(sp18.m, src, 0x40);
    sp18.m[0][3] = 0.0f;
    sp18.m[1][3] = 0.0f;
    sp18.m[2][3] = 0.0f;
    sp18.m[3][3] = 1.0f;
    guMtxF2L(sp18.m, arg0);
    return 1;
}

/* Floor handler: bounce like func_151339D4, and come to rest (clearing the motion flags) once the
 * rebound is slower than 4. */
s32 func_15133B98(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, FloorSurface *surface) {
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
s32 func_15133C58(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, FloorSurface *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 9U) + 0x2DE, 12000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

/* Floor handler: func_15133B98, plus (half the time) a random sound 0x1B8..0x1C1 at the hit position. */
s32 func_15133D20(Particle *particle, f32 x, f32 y, f32 z, f32 floorHeight, FloorSurface *surface) {
    func_15133B98(particle, x, y, z, floorHeight, surface);
    if (func_150ADA20() & 1) {
        func_10010F88((func_150ADA20() % 10U) + 0x1B8, 24000, 0, 0, 0, x, y, z, 500, 1000);
    }
    return 1;
}

void func_15133DE8(InteractionTargetFields *arg0, InteractionKeyFields *arg1, u8 arg2) {
    s32 temp;

    if (arg2 == 0) {
        temp = arg1->field_0x0;
        if ((temp == arg0->field_0x7C) ||
            (arg0->field_0x80 == arg1->field_0x4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

void func_15133E3C(s32 arg0, u8 arg1) {
    Data15133E3C sp18;

    sp18 = D_800A3860;
    func_15169260(&sp18, 2, arg0, arg1);
}

Gfx *func_15133EEC(Gfx *gfx, u16 arg1, u8 arg2, s32 arg3);

void func_15133E84(s32 arg0, void *arg1, s32 arg2) {
    func_15133EEC(arg0, *(u16 *)((u8 *)arg1 + 0x170), *(u8 *)((u8 *)arg1 + 0x172), *(s32 *)((u8 *)arg1 + 0x174));
}

void func_15133EB8(s32 arg0, void *arg1, s32 arg2) {
    func_15133EEC(arg0, *(u16 *)((u8 *)arg1 + 0x174), *(u8 *)((u8 *)arg1 + 0x176), *(s32 *)((u8 *)arg1 + 0x178));
}

typedef struct {
    s32 *unk0;
    u8 pad4[6];
    u8 unkA;
    u8 unkB;
} Entry15133EEC;

extern Entry15133EEC D_80090B60[];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

Gfx *func_15133EEC(Gfx *gfx, u16 arg1, u8 arg2, s32 arg3) {
    s32 data;
    u8 kind;
    u8 fmt;
    Entry15133EEC *e;
    s32 size;

    e = &D_80090B60[arg1];
    data = *e->unk0;
    kind = e->unkA;
    fmt = e->unkB;
    data = func_1510D0EC(data, &size, arg3, 0);
    gSPSegment(gfx++, arg2, data);
    if (kind == 2) {
        gSPSegment(gfx++, arg2 + 1, data + size - ((fmt == 1) ? 0x200 : 0x20));
    }
    return gfx;
}

typedef struct {
    u8 unk0[4];
    u16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
} Elem15133FD8B;

typedef u8 Entry15133FD8[8];

s32 func_15133FD8(s32 arg0, void *arg1, s32 arg2) {
    u8 i;
    Entry15133FD8 *base;
    Elem15133FD8B *e;

    base = (Entry15133FD8 *)((u8 *)arg1 + 0x170);
    for (i = 0; i < (*base)[0]; i++) {
        e = (Elem15133FD8B *)base[i];
        arg0 = func_15133EEC(arg0, e->unk4, e->unk6, e->unk8);
    }
    return arg0;
}
