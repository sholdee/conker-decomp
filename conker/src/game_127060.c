#include <ultra64.h>
// matching: redeclared below with the types this file was built against (functions.h has a u8 return
// for func_150ADA20, which adds masks, and an s32 arg1 for func_1513D2F0, whose cast swaps registers)
#define func_1513D2F0 func_1513D2F0_hdr
#define func_150ADA20 func_150ADA20_hdr
#include "functions.h"
/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the table. */
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098
#undef func_1513D2F0
#undef func_150ADA20
extern struct178 *D_800D3098;
u32 func_150ADA20(void);
void *func_1513D2F0(void *spawn, void *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, s32 arg8,
                    s32 dataSize, u8 owner, s32 kind);


extern s32 D_800BE628;
extern s32 D_80082FA4;
void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4);
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
void func_150FB4C0(Gfx *gfx, f32 mtx[4][4]);

typedef struct {
    u8  pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct150F9AB0;
void func_150F9AB0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

/* Per-spark data of a burst spark (func_150F9BB0), stored at 0x110 of the spawned object. */
typedef struct {
    /* 0x00 */ struct127 *owner;
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
    /* 0x034 */ struct17 pos;
    /* 0x040 */ struct17 dir;
    /* 0x04C */ u8 pad4C[0x110 - 0x4C];
    /* 0x110 */ BurstSparkData data;
} BurstSpark;

/* Per-glow data of a burst glow (func_150F9BB0), stored at 0x170 of the spawned object. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 size;
    /* 0x08 */ struct260 *effect;
    /* 0x0C */ u8 index;
    /* 0x10 */ struct127 *owner;
    /* 0x14 */ u8 ownerId;
} BurstGlowData; /* 0x18 */

/* A glow object (func_151407D0) of the burst. */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ struct17 pos;
    /* 0x040 */ struct17 dir;
    /* 0x04C */ u8 pad4C[0x58 - 0x4C];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0x170 - 0x5C];
    /* 0x170 */ BurstGlowData data;
} BurstGlow;

/* Effect data of the burst effect (updated by func_150FA1B8). */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ BurstSpark *sparks[2];
    /* 0x10 */ BurstGlow *glows[2];
} BurstData; /* 0x18 */

typedef struct {
    /* 0x00 */ struct260 header; /* header.unkE: age in frames */
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ BurstData data;
} BurstEffect;

/* Spawn parameters shared by func_1513D2F0 and func_151407D0. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 lifetime;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ struct17 unk1C;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} SpawnParams; /* 0x58 */

/* Shape parameters of a glow (func_151407D0). */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ s8 unk59;
} GlowShape; /* 0x5C */

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
void func_150F9BB0(struct127 *actor, u8 owner, s32 kind) {
    BurstEffect *effect;
    BurstData data;
    BurstData *burst;

    data.owner = actor;
    data.ownerId = actor->unique_id;
    data.sparks[0] = NULL;
    data.sparks[1] = NULL;
    data.glows[0] = NULL;
    data.glows[1] = NULL;
    effect = (BurstEffect *)func_15149130(70, -1, 0x4D, -1, 1, 0x33, (struct37 *)sizeof(data), owner, kind);
    if (effect != NULL) {
        burst = &effect->data;
        memcpy(burst, &data, sizeof(data));
        {
            BurstSparkData spark;
            SpawnParams spawn;

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
            spawn.unk14 = 0.0f;
            spawn.unk18 = 0.0f;
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
            spawn.unk0 = 0x5F;
            spawn.unk1 = 8;
            spawn.unk2 = 0x2203;
            spawn.lifetime = 300;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.color[0] = 0xFF;
            spawn.color[1] = 0xFF;
            spawn.color[2] = 0xFF;
            spawn.color[3] = 0xFF;
            spawn.unk1C = *(struct17 *)&D_800A5480;
            spawn.unk28 = *(struct17 *)&D_800A5480;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 1.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0x40EC0000;
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
            SpawnParams spawn;
            BurstGlowData glow;

            glow.unk0 = 0;
            glow.size = 1.0f;
            glow.owner = actor;
            glow.effect = &effect->header;
            glow.ownerId = actor->unique_id;
            shape.unk1C = 0.7f;
            shape.unk20 = 1e-06f;
            shape.unk24 = -1;
            shape.unk34 = -1;
            shape.unk28 = -1;
            shape.unk38 = -1;
            shape.unk2C = -1;
            shape.unk3C = -1;
            shape.unk30 = -1;
            shape.unk40 = -1;
            shape.unk0 = 50.3f;
            shape.unk4 = 50.3f;
            shape.unk8 = 30.9f;
            shape.unkC = 30.9f;
            shape.unk10 = 0.1f;
            shape.unk14 = 0.1f;
            shape.unk18 = 1.0f;
            shape.unk44 = 0;
            shape.unk48 = 1.0f;
            shape.unk4C = 0;
            shape.unk4E = 0;
            shape.unk50 = 0;
            shape.unk52 = 0;
            shape.unk54 = 0;
            shape.unk55 = 0;
            shape.unk56 = 0;
            shape.unk57 = 0;
            shape.unk58 = 15;
            shape.unk59 = -1;
            spawn.unk0 = 0x60;
            spawn.unk1 = 3;
            spawn.unk2 = 0x2203;
            spawn.lifetime = 300;
            spawn.unk8 = 0;
            spawn.unkC = 0;
            spawn.color[0] = 0xFF;
            spawn.color[1] = 0xFF;
            spawn.color[2] = 0xFF;
            spawn.color[3] = 0xFF;
            spawn.unk14 = 100.0f;
            spawn.unk18 = 100.0f;
            spawn.unk1C = *(struct17 *)&D_800A5480;
            spawn.unk28 = *(struct17 *)&D_800A5480;
            spawn.unk34 = 1.0f;
            spawn.unk38 = 1.0f;
            spawn.unk3C = 1.0f;
            spawn.unk40 = 0xCD2002;
            spawn.unk44 = 0xFF;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            spawn.unk4C = 0xFF;
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
extern struct17 D_800A1CCC;
extern struct17 D_800A1CD8;
extern struct17 D_800A1CE4;
extern struct17 D_800A1CF0;

/*
 * Burst effect update: ends when the owner is gone, keeps the sparks and glows on the owner's
 * second matrix (hiding them while it has none), and shows them once the effect is 10 frames old.
 */
void func_150FA1B8(BurstEffect *effect) {
    struct127 *actor;
    BurstData *burst;

    actor = effect->data.owner;
    burst = &effect->data;
    if ((actor->interaction_state == 0) || (actor->unique_id != burst->ownerId)) {
        effect->header.unkE = -1;
        return;
    }
    if (actor->unk1D4 != NULL) {
        struct17 positions[2];
        struct17 directions[2];
        s32 pad; // matching: unused stack slot
        struct17 *offsets[4];
        struct17 *results[4];
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
    struct17 pos;
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
    struct17 pos;
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

/* Particle spawn template passed to func_15130280. */
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
    u8 unk1D;
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
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} ParticleSpawn; /* 0x70 */

extern struct17 D_800A1CFC[];
extern void func_15143134(struct17 *offset, struct17 *out, Mtx *mtx);
extern void func_1504715C(void *ground, struct127 *actor);
extern void func_15153F18(s16 *, void *, void *, u8, s32);
extern void func_15150178(s16 *, void *, void *, u8, s32);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_151C329C(struct17 *, u8, s32);

/*
 * Spawns an impact at offset D_800A1CFC[point] in actor's sixth matrix: two func_15153F18 /
 * func_15150178 bursts, a particle with a random rise, and func_151C329C at the point.
 */
void func_150FA520(struct127 *actor, u8 point, u8 owner, s32 kind) {
    struct17 pos;
    u8 ground[0x24];

    if (actor->unk1D4) {
        func_15143134(&D_800A1CFC[point], &pos, (Mtx *)actor->unk1D4 + 5);
        func_1504715C(ground, actor);
        {
            Spawn15153F18 a;

        a.pos = pos;
        a.unk14 = 4.0f;
        a.unk2C = 9;
        a.unk2E = 6;
        a.unk02 = 0xFF;
        a.unk04 = -40;
        a.unk00 = 0;
        a.unk06 = 0x32;
        a.unk30 = 3;
        a.unk32 = 1;
        a.unk34 = 0x1E;
        a.unk36 = 0x32;
        a.unk38 = 0x64;
        a.unk3A = 0x64;
        a.unk44 = 0x10;
        a.unk46 = 0xF;
        a.unk48 = 0;
        a.unk3C = 1;
        a.unk18 = 3.879999876f;
        a.unk1C = 0.694f;
        a.unk20 = 0.4500000179f;
        a.unk24 = 12.0f;
        a.unk28 = 13.90000057f;
        a.unk40 = 1.0f;
        func_15153F18(&a.unk00, &a.pos, ground, owner, kind);
        }
        {
            Spawn15150178 b;

        b.pos = pos;
        b.unk14 = 7;
        b.unk16 = 6;
        b.unk00 = 0;
        b.unk02 = 0xFF;
        b.unk04 = -63;
        b.unk06 = 0x18;
        b.unk20 = 0x23;
        b.unk22 = 0xB;
        b.unk2C = 0x64;
        b.unk2D = 0x64;
        b.unk38 = 1;
        b.unk39 = 1;
        b.unk40 = 1;
        b.unk18 = 8.0f;
        b.unk1C = 10.0f;
        b.unk24 = -1.266f;
        b.unk28 = 0.4660000205f;
        b.unk30 = 123.0f;
        b.unk34 = 104.0f;
        b.unk3C = 1.0f;
        b.unk44 = 0.3f;
        func_15150178(&b.unk00, &b.pos, ground, owner, kind);
        }
        {
            ParticleSpawn spawn;
            f32 data[5];
            void *particle;

        data[0] = 0.0f;
        data[2] = 1369.429932f;
        data[1] = func_150ADA68() * 3.0f + 10.0f;
        data[3] = func_150ADA68() * 100.0f + 100.0f;
        data[4] = data[3] / (data[1] * data[1]);
        spawn.unk1B = data[3];
        spawn.unk1D = 0x79;
        spawn.unk8 = 0x4403;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk18 = 0xDC;
        spawn.unk19 = 0xDC;
        spawn.unk1A = 0xFF;
        spawn.unk17 = 0xFF;
        spawn.unk14 = 0xFF;
        spawn.unk15 = 0xFF;
        spawn.unk16 = 0xFF;
        spawn.unk1C = 0xFF;
        spawn.pos = pos;
        spawn.unk3C = *(struct17 *)&D_800A5480;
        spawn.unk1E = 1;
        spawn.unk20 = 0xFF;
        spawn.unk22 = 1;
        spawn.unk24 = 1.0f;
        spawn.flags = 0x4C000 | ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
        spawn.unk60 = 6;
        spawn.unk61 = 6;
        spawn.unk62 = 0x27;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk5C = 0;
        spawn.unk66 = 0xFF;
        spawn.vel = *(struct17 *)&D_800A5480;
        spawn.unkA = 300;
        spawn.unk28 = 0.0f;
        spawn.unk2C = 0.0f;
        spawn.unk54 = 0.0f;
        particle = func_15130280(&spawn, 1, 0, sizeof(data), owner, kind);
        if (particle != NULL) {
            memcpy((u8 *)particle + 0xA8, data, sizeof(data));
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

        effect = (EmitterEffect *)func_15149130(300, -1, 0x57, -1, 0, 0x46, (struct37 *)sizeof(data), owner, kind);
        if (effect != NULL) {
            D_80088B60 = 1;
        }
        if (effect != NULL) {
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
void func_1514470C(s32, struct17 *);
void func_150F4570(struct17 *from, struct17 *to, u8 id, f32 duration, f32 rate, f32 scale, u8 arg6, s32 arg7, u8 owner,
                   s32 kind);

/* Emitter update: spawns func_150F4570 effects between the two endpoints, 0.08-0.26 per frame (times D_800BE9A4). */
void func_150FAAEC(EmitterEffect *effect) {
    EmitterData *data;
    struct17 from;
    struct17 to;
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

void func_150FACE4(struct102 *arg0, s32 arg1, u8 arg2) {
    switch (arg2) {
        case 0x4E:
        case 0x4F:
            func_1516972C(arg0);
            break;
    }
}

extern void func_1515F170(s32 arg0, u8 arg1);

void func_150FAD28(void) {
    func_1515F170(8, 0);
    func_1515F170(0xB, 1);
    func_151494E0((s32)&D_800D3098[25], 0x30);
    func_151494E0(0, 0x4D);
}

void func_150FAD78(void) {
    func_1515F170(8, 1);
    func_1515F170(7, 0);
    func_151494E0((s32)&D_800D3098[25], 0x31);
    func_151494E0(0, 0x4C);
}

void func_150FADC8(s32 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x53) {
        arg0[0x16] |= 2;
    } else if (arg2 == 0x54) {
        arg0[0x16] &= ~2;
    }
}

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
extern void *func_1515548C(void *, s32, s32, s32, s32, u8, s32);

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
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S150FAE18Main;

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
} S150FAE18Part;

void func_150FAE18(s16 arg0, u8 arg1, s32 arg2) {
    s16 pad;
    s16 id;

    id = arg0;
    {
    S150FAE18Main spC4;
    void *ret;

    spC4.unk38 = 0;
    spC4.unk00 = 0x61;
    spC4.unk01 = 3;
    spC4.unk02 = 4;
    spC4.unk03 = 2;
    spC4.unk04 = 0;
    spC4.unk06 = arg0;
    spC4.unk08 = 0xA5;
    spC4.unk0C = 0x17;
    spC4.unk10 = 0;
    spC4.unk14 = 0x200405;
    spC4.unk18 = 0x60200;
    spC4.unk2D = 8;
    spC4.unk1C = 1;
    spC4.unk20 = 0x38;
    spC4.unk2C = 0;
    spC4.unk24 = 0x80;
    spC4.unk28 = 0x20;
    spC4.unk30 = 0xFF;
    spC4.unk31 = 0xFF;
    spC4.unk32 = 0xFF;
    spC4.unk33 = 0xFF;
    spC4.unk34 = 0xFF;
    spC4.unk35 = 0xFF;
    spC4.unk36 = 0xFF;
    spC4.unk37 = 0xFF;
    spC4.unk3C = 0;
    spC4.unk40 = 0;
    spC4.unk41 = 1;
    spC4.unk44 = *(struct17 *)&D_800A5480;
    spC4.unk54 = 0xA;
    spC4.unk56 = 0x19;
    ret = func_15157010(&spC4, 0, 1.0, 0, 0, 2, arg1, arg2);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x120, &id, 2);
    }
    {
    S150FAE18Part sp68;
    u8 i;
    const s16 tbl[8] = { 0xCE, 0xC5, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4 };

    sp68.unk04 = 62.0f;
    sp68.unk12 = arg0;
    sp68.unk14 = 0x29;
    sp68.unk16 = 0xA;
    sp68.unk18 = 0x19;
    sp68.unk08 = 16.0f;
    sp68.unk0C = 16.0f;
    sp68.unk1A = 0;
    sp68.unk1B = 0xFF;
    sp68.unk1C = 0xFF;
    sp68.unk1D = 0xFF;
    sp68.unk1E = 0xFF;
    sp68.unk1F = 0xFF;
    sp68.unk20 = 0xFF;
    sp68.unk21 = 0xFF;
    sp68.unk22 = 0xFF;
    sp68.unk23 = 0xFF;
    sp68.unk24 = 0;
    sp68.unk28 = 0x200405;
    sp68.unk2C = 0x60200;
    sp68.unk40 = 0;
    sp68.unk41 = 7;
    sp68.unk30 = 0x19;
    sp68.unk34 = 0x22;
    sp68.unk38 = 0x80;
    sp68.unk3C = 0x20;
    sp68.unk44 = 0;
    sp68.unk48 = 1.0f;
    sp68.unk4C = 1.0f;
    sp68.unk50 = 0.0f;
    sp68.unk54 = 0.0f;
    for (i = 0; i < 8; i++) {
        sp68.unk00 = i * 24.75f + -65.0f;
        sp68.unk10 = tbl[i];
        ret = func_1515548C(&sp68, 0xE, 0, 0, 2, arg1, arg2);
        if (ret != NULL) {
            memcpy((u8 *)ret + 0x70, &id, 2);
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

void func_15157DEC(u8 *, u8 *);

s32 func_150FB188(Func150FB188State *arg0) {
    Func150FB188Transform transform;

    arg0->field5C = 0.0f;
    arg0->field54 = -95.0f;
    arg0->field58 = -80.0f;
    transform.field0 = 0.0f;
    transform.field4 = 0.0f;
    transform.field8 = 0.0f;
    transform.fieldC = 0.3f;
    transform.field10 = 0.3f;
    func_15157DEC((u8 *)arg0, (u8 *)&transform);
    return 1;
}

extern s32 func_151D710C(s32, s32, s32, s32, s32);
extern void func_15157F80(s32, s32, s32, s32, s32);

void func_150FB1E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_15157F80(func_151D710C(arg0, arg1, arg2, arg3, arg4), arg1, arg2, arg3, arg4);
}
