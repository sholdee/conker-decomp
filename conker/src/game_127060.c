#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


/* Per-spark data of a burst spark (func_150F9BB0), stored at 0x110 of the spawned object. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 index;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 width;
    /* 0x4C */ f32 height;
    /* 0x50 */ struct260 *effect;
    /* 0x54 */ f32 ownerScale;
    /* 0x58 */ u8 flags;
} BurstSparkData; /* 0x5C */

/* A spark object (func_1513D2F0) of the burst. */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f dir;
    /* 0x04C */ u8 pad4C[0x110 - 0x4C];
    /* 0x110 */ BurstSparkData data;
} BurstSpark;

/* Per-glow data of a burst glow (func_150F9BB0), stored at 0x170 of the spawned object. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 size;
    /* 0x08 */ struct260 *effect;
    /* 0x0C */ u8 index;
    /* 0x10 */ Actor *owner;
    /* 0x14 */ u8 ownerId;
} BurstGlowData; /* 0x18 */

/* A glow object (func_151407D0) of the burst. */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f dir;
    /* 0x04C */ u8 pad4C[0x58 - 0x4C];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0x170 - 0x5C];
    /* 0x170 */ BurstGlowData data;
} BurstGlow;

/* Effect data of the burst effect (updated by func_150FA1B8). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ BurstSpark *sparks[2];
    /* 0x10 */ BurstGlow *glows[2];
} BurstData; /* 0x18 */

typedef struct {
    /* 0x00 */ struct260 header; /* header.unkE: age in frames */
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ BurstData data;
} BurstEffect;

/* Event argument of func_151D8868. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x02 */ s16 duration;
    /* 0x04 */ u8 field4;
    /* 0x05 */ u8 field5;
    /* 0x06 */ s8 field6;
} Event151D8868;

extern s32 D_800A1CC0;
void *func_151407D0(void *, s32, void *, u8, u8, u8, u8, s8, u8, s32);
void func_151D8868(void *, s32, u8, s32);
s32 func_15160A58(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns the burst effect on actor: two sparks and two glows that follow its second matrix. */
void func_150F9BB0(Actor *actor, u8 owner, s32 kind) {
    BurstEffect *effect;
    BurstData data;
    BurstData *burst;

    data.owner = actor;
    data.ownerId = actor->unique_id;
    data.sparks[0] = NULL;
    data.sparks[1] = NULL;
    data.glows[0] = NULL;
    data.glows[1] = NULL;
    effect = (BurstEffect *)func_15149130(70, -1, 0x4D, -1, 1, 0x33, sizeof(data), owner, kind);
    if (effect != NULL) {
        burst = &effect->data;
        memcpy(burst, &data, sizeof(data));
        {
            BurstSparkData spark;
            ParticleDef spawn;

            spark.flags = 0;
            spark.ownerScale = (actor->xz_scale + actor->y_scale) * 0.5f;
            spark.owner = actor;
            spark.ownerId = actor->unique_id;
            spark.unk8 = 15.045272f;
            spark.unk10 = 5.8676558f;
            spark.unk18 = 12.111444f;
            spark.unkC = 0.396174f;
            spark.unk24 = 0.0f;
            spark.unk28 = 0.0f;
            spark.unk38 = 0.0f;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            spark.unk3C = 0.0f;
            spark.width = 1.0f;
            spark.unk40 = 1.0f;
            spark.height = 1.0f;
            spark.unk44 = 1.0f;
            spark.unk14 = 0.1042563f;
            spark.unk20 = 0.5197371f;
            spark.unk1C = 0.34404588f;
            spark.unk2C = 160.0f;
            spark.unk30 = 95.0f;
            spark.unk34 = 207.5f;
            spark.effect = &effect->header;
            spawn.texture = 0x5F;
            spawn.type = 8;
            spawn.renderModes = 0x2203;
            spawn.lifetime = 300;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.pos = D_800A5480;
            spawn.rot = D_800A5480;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0x40EC0000;
            spawn.unk44 = 0;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                burst->sparks[spark.index] = func_1513D2F0(&spawn, &D_800A4AA0, 0, 0x25, 0, 0x1D, ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, sizeof(spark), owner, kind);
                if (burst->sparks[spark.index] != NULL) {
                    memcpy(&burst->sparks[spark.index]->data, &spark, sizeof(spark));
                }
            }
        }
        {
            GlowShape shape;
            ParticleDef spawn;
            BurstGlowData glow;

            glow.unk0 = 0;
            glow.size = 1.0f;
            glow.owner = actor;
            glow.effect = &effect->header;
            glow.ownerId = actor->unique_id;
            shape.alpha0 = 0.7f;
            shape.fadeRate = 1e-06f;
            shape.screenX[0] = -1;
            shape.screenY[0] = -1;
            shape.screenX[1] = -1;
            shape.screenY[1] = -1;
            shape.screenX[2] = -1;
            shape.screenY[2] = -1;
            shape.screenX[3] = -1;
            shape.screenY[3] = -1;
            shape.width1 = 50.3f;
            shape.height1 = 50.3f;
            shape.width0 = 30.9f;
            shape.height0 = 30.9f;
            shape.offset1 = 0.1f;
            shape.offset0 = 0.1f;
            shape.alpha1 = 1.0f;
            shape.attach = 0;
            shape.scale = 1.0f;
            shape.zbufSample[0] = 0;
            shape.zbufSample[1] = 0;
            shape.zbufSample[2] = 0;
            shape.zbufSample[3] = 0;
            shape.visibility[0] = 0;
            shape.visibility[1] = 0;
            shape.visibility[2] = 0;
            shape.visibility[3] = 0;
            shape.type = 15;
            shape.handler = -1;
            spawn.texture = 0x60;
            spawn.type = 3;
            spawn.renderModes = 0x2203;
            spawn.lifetime = 300;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.width = 100.0f;
            spawn.height = 100.0f;
            spawn.pos = D_800A5480;
            spawn.rot = D_800A5480;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0xCD2002;
            spawn.unk44 = 0xFF;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            spawn.viewportMask = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                burst->glows[glow.index] = func_151407D0(&shape, 0x78, &spawn, 0, 0x2A, 0, 0, -1, owner, kind);
                if (burst->glows[glow.index] != NULL) {
                    memcpy(&burst->glows[glow.index]->data, &glow, sizeof(glow));
                }
            }
        }
    }
    {
        Event151D8868 event;

        event.type = 1;
        event.duration = 70;
        event.field5 = 1;
        event.field4 = 8;
        event.field6 = -1;
        func_151D8868(&event, 0, owner, kind);
        func_15160A58(actor, 1, &D_800A1CC0, 3, 70, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, owner, kind);
    }
}

extern s32 func_15145EA4(void *offsets, void *results, Mtx *mtx, s32 count);
extern Vec3f D_800A1CCC;
extern Vec3f D_800A1CD8;
extern Vec3f D_800A1CE4;
extern Vec3f D_800A1CF0;

/*
 * Burst effect update: ends when the owner is gone, keeps the sparks and glows on the owner's
 * second matrix (hiding them while it has none), and shows them once the effect is 10 frames old.
 */
void func_150FA1B8(BurstEffect *effect) {
    Actor *actor;
    BurstData *burst;

    actor = effect->data.owner;
    burst = &effect->data;
    if ((actor->interaction_state == 0) || (actor->unique_id != burst->ownerId)) {
        effect->header.unkE = -1;
        return;
    }
    if (actor->unk1D4 != NULL) {
        Vec3f positions[2];
        Vec3f directions[2];
        s32 pad; // matching: unused stack slot
        Vec3f *offsets[4];
        Vec3f *results[4];
        BurstSpark *spark;
        BurstGlow *glow;
        u8 i;

        offsets[0] = &D_800A1CCC;
        offsets[1] = &D_800A1CD8;
        offsets[2] = &D_800A1CE4;
        offsets[3] = &D_800A1CF0;
        results[0] = &positions[0];
        results[1] = &positions[1];
        results[2] = &directions[0];
        results[3] = &directions[1];
        func_15145EA4(offsets, results, (Mtx *)actor->unk1D4 + 1, 4);
        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                spark = burst->sparks[i];
                spark->data.flags |= 1;
                spark->pos = positions[i];
                spark->dir = directions[i];
            }
            if (burst->glows[i] != NULL) {
                glow = burst->glows[i];
                glow->flags |= 2;
                glow->flags &= ~4;
                glow->pos = positions[i];
                glow->dir = directions[i];
            }
        }
    } else {
        BurstSparkData *sparkData;
        u8 i;

        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                sparkData = &burst->sparks[i]->data;
                sparkData->flags &= ~1;
            }
            if (burst->glows[i] != NULL) {
                burst->glows[i]->flags &= ~2;
            }
        }
    }
    {
        u8 i;
        s32 pad; // matching: unused local; removing it reschedules the loop

        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                BurstSparkData *sparkData = &burst->sparks[i]->data;

                if (effect->header.unkE < 10) {
                    sparkData->width = 0.0f;
                    sparkData->height = 0.0f;
                } else {
                    sparkData->width = 6.0f;
                    sparkData->height = 15.0f;
                }
            }
            if (burst->glows[i] != NULL) {
                BurstGlowData *glowData = &burst->glows[i]->data;

                if (effect->header.unkE < 10) {
                    glowData->size = 0.0f;
                } else {
                    glowData->size = 1.306000113f;
                }
            }
        }
    }
}

/* Event handler: events 0x4C-0x4F set effect->unk11; 0x4C and 0x4F also call func_1515D4D4 (all 0 / all 0xFF). */
void func_150FA468(struct260 *effect, s32 arg1, u8 event) {
    if (event == 0x4C) {
        effect->unk11 = -1;
        func_1515D4D4(0, 0, 0, 0xFF);
    } else if (event == 0x4D) {
        effect->unk11 = 30;
    } else if (event == 0x4E) {
        effect->unk11 = -1;
    } else if (event == 0x4F) {
        effect->unk11 = -1;
        func_1515D4D4(0xFF, 0xFF, 0xFF, 0xFF);
    }
}

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    Vec3f pos;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    u8 unk3C;
    u8 pad3D[3];
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} Spawn15153F18;

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    Vec3f pos;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    u8 unk2C;
    u8 unk2D;
    u8 pad2E[2];
    f32 unk30;
    f32 unk34;
    u8 unk38;
    u8 unk39;
    u8 pad3A[2];
    f32 unk3C;
    u8 unk40;
    u8 pad41[3];
    f32 unk44;
} Spawn15150178;

/* A particle created by func_15130280; this caller's data lives at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 data[5];
} ImpactParticle;

extern Vec3f D_800A1CFC[];
extern void func_15143134(Vec3f *offset, Vec3f *out, Mtx *mtx);
extern void func_1504715C(void *ground, Actor *actor);
extern void func_15153F18(s16 *, void *, void *, u8, s32);
extern void func_15150178(s16 *, void *, void *, u8, s32);
extern ImpactParticle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
extern void func_151C329C(Vec3f *, u8, s32);

/*
 * Spawns an impact at offset D_800A1CFC[point] in actor's sixth matrix: two func_15153F18 /
 * func_15150178 bursts, a particle with a random rise, and func_151C329C at the point.
 */
void func_150FA520(Actor *actor, u8 point, u8 owner, s32 kind) {
    Vec3f pos;
    u8 ground[0x24];

    if (actor->unk1D4) {
        func_15143134(&D_800A1CFC[point], &pos, (Mtx *)actor->unk1D4 + 5);
        func_1504715C(ground, actor);
        {
            Spawn15153F18 first;

            first.pos = pos;
            first.unk14 = 4.0f;
            first.unk2C = 9;
            first.unk2E = 6;
            first.unk02 = 0xFF;
            first.unk04 = -40;
            first.unk00 = 0;
            first.unk06 = 0x32;
            first.unk30 = 3;
            first.unk32 = 1;
            first.unk34 = 0x1E;
            first.unk36 = 0x32;
            first.unk38 = 0x64;
            first.unk3A = 0x64;
            first.unk44 = 0x10;
            first.unk46 = 0xF;
            first.unk48 = 0;
            first.unk3C = 1;
            first.unk18 = 3.879999876f;
            first.unk1C = 0.694f;
            first.unk20 = 0.4500000179f;
            first.unk24 = 12.0f;
            first.unk28 = 13.90000057f;
            first.unk40 = 1.0f;
            func_15153F18(&first.unk00, &first.pos, ground, owner, kind);
        }
        {
            Spawn15150178 second;

            second.pos = pos;
            second.unk14 = 7;
            second.unk16 = 6;
            second.unk00 = 0;
            second.unk02 = 0xFF;
            second.unk04 = -63;
            second.unk06 = 0x18;
            second.unk20 = 0x23;
            second.unk22 = 0xB;
            second.unk2C = 0x64;
            second.unk2D = 0x64;
            second.unk38 = 1;
            second.unk39 = 1;
            second.unk40 = 1;
            second.unk18 = 8.0f;
            second.unk1C = 10.0f;
            second.unk24 = -1.266f;
            second.unk28 = 0.4660000205f;
            second.unk30 = 123.0f;
            second.unk34 = 104.0f;
            second.unk3C = 1.0f;
            second.unk44 = 0.3f;
            func_15150178(&second.unk00, &second.pos, ground, owner, kind);
        }
        {
            ParticleDesc spawn;
            f32 data[5];
            ImpactParticle *particle;

            data[0] = 0.0f;
            data[2] = 1369.429932f;
            data[1] = func_150ADA68() * 3.0f + 10.0f;
            data[3] = func_150ADA68() * 100.0f + 100.0f;
            data[4] = data[3] / (data[1] * data[1]);
            spawn.alpha = data[3];
            spawn.texture = 0x79;
            spawn.renderMode = 0x4403;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r2 = 0xDC;
            spawn.g2 = 0xDC;
            spawn.b2 = 0xFF;
            spawn.a = 0xFF;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.brightness = 0xFF;
            spawn.pos = pos;
            spawn.offset = D_800A5480;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0x4C000 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.primColorMode = 6;
            spawn.envColorMode = 6;
            spawn.updateFunc = 0x27;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.vel = D_800A5480;
            spawn.timer = 300;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            spawn.gravity = 0.0f;
            particle = func_15130280(&spawn, 1, 0, sizeof(data), owner, kind);
            if (particle != NULL) {
                memcpy(particle->data, data, sizeof(data));
            }
        }
        func_151C329C(&pos, owner, kind);
    }
}

/* Data of the emitter effect started by func_150FAA40 and updated by func_150FAAEC. */
typedef struct {
    /* 0x00 */ s32 from; /* endpoints, resolved with func_1514470C */
    /* 0x04 */ s32 to;
    /* 0x08 */ f32 pending; /* fractional count of spawns due */
} EmitterData;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x28 - 0xD];
    /* 0x28 */ EmitterData data;
} EmitterEffect;

/* Starts the (single) emitter effect between D_800D3098 records 35 and 34. */
void func_150FAA40(u8 owner, s32 kind) {
    EmitterData data;
    EmitterEffect *effect;

    if (D_80088B60 == 0) {
        data.from = (s32)&D_800D3098[35];
        data.to = (s32)&D_800D3098[34];
        data.pending = 0.0f;

        effect = (EmitterEffect *)func_15149130(300, -1, 0x57, -1, 0, 0x46, sizeof(data), owner, kind);
        if (effect != NULL) {
            D_80088B60 = 1;
        }
        if (effect != NULL) { // matching: a second test; one merged if mismatches
            memcpy(&effect->data, &data, sizeof(data));
        }
    }
}

/* A list of spawn variants: an id and a scale for each. */
typedef struct {
    /* 0x00 */ u8 count;
    /* 0x04 */ s16 *ids;
    /* 0x08 */ f32 *scales;
} VariantList;

extern VariantList D_80088B20[];
void func_1514470C(s32, Vec3f *);
void func_150F4570(Vec3f *from, Vec3f *to, u8 id, f32 duration, f32 rate, f32 scale, u8 arg6, s32 arg7, u8 owner,
                   s32 kind);

/* Emitter update: spawns func_150F4570 effects between the two endpoints, 0.08-0.26 per frame (times D_800BE9A4). */
void func_150FAAEC(EmitterEffect *effect) {
    EmitterData *data;
    Vec3f from;
    Vec3f to;
    u8 i;
    f32 duration;
    VariantList *variants;

    data = &effect->data;
    data->pending += (0.08000000566f + func_150ADA68() * 0.18f) * D_800BE9A4;
    while (data->pending > 1.0f) {
        func_1514470C(data->from, &from);
        func_1514470C(data->to, &to);
        variants = &D_80088B20[1];
        i = func_150ADA20() % variants->count;
        duration = func_150ADA68() * 25.0f + 30.0f;
        // matching: the "* 1" is kept; a plain random + 1.0f schedules differently
        func_150F4570(&from, &to, variants->ids[i], duration, 1.0f / duration,
                      variants->scales[i] * (func_150ADA68() * 1 + 1), 1, func_150ADA20() % 3 + 750,
                      effect->unkC, effect->unk1);
        data->pending -= 1.0f;
    }
}

/* Event handler: events 0x4E and 0x4F call func_1516972C on the object. */
void func_150FACE4(struct102 *obj, s32 arg1, u8 event) {
    if ((event == 0x4E) || (event == 0x4F)) {
        func_1516972C(obj);
    }
}

extern void func_1515F170(s32 arg0, u8 arg1);

/* Calls func_1515F170 for 8 and 0xB, then func_151494E0 with D_800D3098 record 25 (0x30) and 0x4D. */
void func_150FAD28(void) {
    func_1515F170(8, 0);
    func_1515F170(0xB, 1);
    func_151494E0((s32)&D_800D3098[25], 0x30);
    func_151494E0(0, 0x4D);
}

/* Counterpart of func_150FAD28: func_1515F170 for 8 and 7, func_151494E0 with record 25 (0x31) and 0x4C. */
void func_150FAD78(void) {
    func_1515F170(8, 1);
    func_1515F170(7, 0);
    func_151494E0((s32)&D_800D3098[25], 0x31);
    func_151494E0(0, 0x4C);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 flags;
} Obj150FADC8;

/* Event handler: event 0x53 sets flag 2 of the object, event 0x54 clears it. */
void func_150FADC8(Obj150FADC8 *obj, s32 arg1, u8 event) {
    if (event == 0x53) {
        obj->flags |= 2;
    } else if (event == 0x54) {
        obj->flags &= ~2;
    }
}

/* Objects created by func_15157010 / func_1515548C; this caller stores a tag in each. */
typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s16 tag;
} TaggedModel;

typedef struct {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ s16 tag;
} TaggedSprite;

extern TaggedModel *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
extern TaggedSprite *func_1515548C(void *, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} Spawn15157010;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} Spawn1515548C;

/*
 * Spawns a func_15157010 object and a row of 8 func_1515548C sprites (the first with kind 0xCE,
 * the rest 0xC5/0xC4), each tagged with duration in its data.
 */
void func_150FAE18(s16 duration, u8 owner, s32 kind) {
    s16 pad; // matching: unused stack slot
    s16 tag;

    tag = duration;
    {
        Spawn15157010 model;
        void *spawned; // matching: one variable for both spawned objects (separate ones reallocate)

        model.unk38 = 0;
        model.unk00 = 0x61;
        model.unk01 = 3;
        model.unk02 = 4;
        model.unk03 = 2;
        model.unk04 = 0;
        model.unk06 = duration;
        model.unk08 = 0xA5;
        model.unk0C = 0x17;
        model.unk10 = 0;
        model.unk14 = 0x200405;
        model.unk18 = 0x60200;
        model.unk2D = 8;
        model.unk1C = 1;
        model.unk20 = 0x38;
        model.unk2C = 0;
        model.unk24 = 0x80;
        model.unk28 = 0x20;
        model.unk30 = 0xFF;
        model.unk31 = 0xFF;
        model.unk32 = 0xFF;
        model.unk33 = 0xFF;
        model.unk34 = 0xFF;
        model.unk35 = 0xFF;
        model.unk36 = 0xFF;
        model.unk37 = 0xFF;
        model.unk3C = 0;
        model.unk40 = 0;
        model.unk41 = 1;
        model.unk44 = D_800A5480;
        model.unk54 = 10;
        model.unk56 = 25;
        spawned = func_15157010(&model, 0, 1.0, 0, 0, 2, owner, kind);
        if (spawned != NULL) {
            memcpy(&((TaggedModel *)spawned)->tag, &tag, sizeof(tag));
        }
        {
            Spawn1515548C sprite;
            u8 i;
            const s16 kinds[8] = { 0xCE, 0xC5, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4 };

            sprite.unk04 = 62.0f;
            sprite.unk12 = duration;
            sprite.unk14 = 0x29;
            sprite.unk16 = 10;
            sprite.unk18 = 25;
            sprite.unk08 = 16.0f;
            sprite.unk0C = 16.0f;
            sprite.unk1A = 0;
            sprite.unk1B = 0xFF;
            sprite.unk1C = 0xFF;
            sprite.unk1D = 0xFF;
            sprite.unk1E = 0xFF;
            sprite.unk1F = 0xFF;
            sprite.unk20 = 0xFF;
            sprite.unk21 = 0xFF;
            sprite.unk22 = 0xFF;
            sprite.unk23 = 0xFF;
            sprite.unk24 = 0;
            sprite.unk28 = 0x200405;
            sprite.unk2C = 0x60200;
            sprite.unk40 = 0;
            sprite.unk41 = 7;
            sprite.unk30 = 25;
            sprite.unk34 = 0x22;
            sprite.unk38 = 0x80;
            sprite.unk3C = 0x20;
            sprite.unk44 = 0;
            sprite.unk48 = 1.0f;
            sprite.unk4C = 1.0f;
            sprite.unk50 = 0.0f;
            sprite.unk54 = 0.0f;
            for (i = 0; i < 8; i++) {
                sprite.unk00 = -65.0f + i * 24.75f;
                sprite.unk10 = kinds[i];
                spawned = func_1515548C(&sprite, 14, 0, 0, 2, owner, kind);
                if (spawned != NULL) {
                    memcpy(&((TaggedSprite *)spawned)->tag, &tag, sizeof(tag));
                }
            }
        }
    }
}

typedef struct Func150FB188State {
    u8 pad0[0x54];
    f32 field54;
    f32 field58;
    f32 field5C;
} Func150FB188State;

typedef struct Func150FB188Transform {
    f32 field0;
    f32 field4;
    f32 field8;
    f32 fieldC;
    f32 field10;
} Func150FB188Transform;

void func_15157DEC(Func150FB188State *, Func150FB188Transform *);

/* Sets obj->field54..5C to (-95, -80, 0) and calls func_15157DEC with {0, 0, 0, 0.3, 0.3}. */
s32 func_150FB188(Func150FB188State *obj) {
    Func150FB188Transform transform;

    obj->field5C = 0.0f;
    obj->field54 = -95.0f;
    obj->field58 = -80.0f;
    transform.field0 = 0.0f;
    transform.field4 = 0.0f;
    transform.field8 = 0.0f;
    transform.fieldC = 0.3f;
    transform.field10 = 0.3f;
    func_15157DEC(obj, &transform);
    return 1;
}

extern s32 func_151D710C(s32, s32, s32, s32, s32);
extern void func_15157F80(s32, s32, s32, s32, s32);

/* Passes func_151D710C's result, with the same remaining arguments, to func_15157F80. */
void func_150FB1E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_15157F80(func_151D710C(arg0, arg1, arg2, arg3, arg4), arg1, arg2, arg3, arg4);
}
