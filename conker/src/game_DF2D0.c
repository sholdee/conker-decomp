#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Common head of an effect object (struct260 is a shorter view). Each effect keeps its own data
 * from +0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} EffectHead;

/* Data of the effect spawned by func_150B1EE0 and run by func_150B1F90. */
typedef struct {
    struct17 pos;
    f32 accum; /* particles owed; one is spawned per whole unit */
} EmitterData;

typedef struct {
    EffectHead head;
    EmitterData data;
} EmitterEffect;

/* Mirrors game flag 0x4054 into camera->flags bit 0x10000; when 0x405B is set but 0x405C is not (and
 * camera->mode != 0x40), sets the camera target's unk17C to its unk180 + 10. */
void func_150B1E20(Camera *camera) {
    struct127 *target;

    if (func_1509BE40(1, 0x4054, 6, 0x9000)) {
        camera->flags |= 0x10000;
    } else {
        camera->flags &= ~0x10000;
    }
    if (!func_1509BE40(1, 0x405C, 6, 0x9000)) {
        if (func_1509BE40(1, 0x405B, 6, 0x2000) && camera->mode != 0x40) {
            target = camera->player;
            // struct127 has only pad17C at 0x17C (include/structs.h)
            *(f32 *)((u8 *)target + 0x17C) = target->unk180 + 10.0f;
        }
    }
}

/* Spawns an emitter effect (func_150B1F90) at `pos` with a random duration of 69-201. */
void func_150B1EE0(EffectHead *spawner, struct17 *pos, s32 arg2, s32 arg3) {
    EmitterData data;
    struct260 *effect;

    data.pos = *pos;
    data.accum = 0.0f;

    // the header's struct37 * parameter is really the size of the effect's data
    effect = func_15149130(func_150ADA20() % 133U + 69, -1, 0x49, -1, 1, 0, (struct37 *)sizeof(data),
                           spawner->unkC, spawner->unk1);
    if (effect != NULL) {
        memcpy(&((EmitterEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Particle descriptor passed to func_15130280 (0x70 bytes). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 kind;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 flags;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} ParticleSpawn; // size 0x70

/* Extra data copied to +0xA8 of each spawned particle. */
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} ParticleExtra;

extern void *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);

/* Emitter effect update: accumulates a random rate per frame and spawns one particle at the
 * emitter's position per whole unit owed. */
void func_150B1F90(EmitterEffect *effect) {
    EmitterData *data;

    data = &effect->data;
    data->accum += (0.07859999686f + func_150ADA68() * 0.03460000083f) * D_800BE9A4;
    if (data->accum > 1.0f) {
        ParticleSpawn spawn;
        ParticleExtra extra;
        void *particle;

        extra.unkC = 0.9794009924f;
        spawn.kind = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 80;
        spawn.unk20 = 3;
        spawn.flags = 0x90DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 80;
        spawn.unk14 = 0x67;
        spawn.unk15 = 0x6C;
        spawn.unk16 = 0x6C;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x16;
        spawn.unk19 = 0xB;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.016479015f;
        spawn.pos = data->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.vel.unk0 = 0.0f;
        spawn.vel.unk4 = 0.0f;
        spawn.vel.unk8 = 0.0f;
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 27.0f;
            extra.unk8 = func_150ADA68() * 27.0f;
            spawn.unk54 = func_150ADA68() * 0.2090000063f + 0.1350000054f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 101U + 100;
            spawn.unkA = func_150ADA20() % 78U + 126;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 503.0f + 405.0f;
            particle = func_15130280(&spawn, 1, 0, 0x10, effect->head.unkC, effect->head.unk1);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            data->accum -= 1.0f;
        } while (data->accum > 1.0f);
    }
}

/* Sets camera distances (unk190, unk348/unk34C/unk374) and flag bits in unk84/unk5F0 from game flags. */
void func_150B2340(Camera *camera) {
    s32 flagBC;
    s32 flagBB;

    flagBC = func_1509BE40(0, func_1509BE40(0, 0x2007, 0xB7) | 0x2000, 0xBC);
    flagBB = func_1509BE40(0, 0x2000, 0xBB);

    if (func_1509BE40(0, 0x5043, 0x1A)) {
        if (func_1509BE40(0, 0x5045, 0x1A)) {
            camera->flags |= 0x4000;
        }
        camera->flags |= 0x400000;
        camera->liftTarget = 0.0f;
    } else if (func_1509BE40(1, 0x4030, 6, 0x2000)) {
        camera->liftTarget = 235.0f;
    } else {
        camera->height = 114.0f;
        camera->unk34C = 114.0f;
        camera->targetDistance = 384.0f;
        camera->liftTarget = 60.0f;
    }

    if (flagBC != 0 && flagBB != -1) {
        camera->status |= 0x100;
    } else {
        camera->status &= ~0x100;
    }
}
