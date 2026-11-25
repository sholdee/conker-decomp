#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Spawn header that func_15147A80 copies to 0x10 of a new trail. */
typedef struct TrailHeader {
    /* 0x00 */ vertex pos;
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;
    /* 0x10 */ s32 kind;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 capacity; /* number of points in the ring buffer */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void *unk18;
} TrailHeader; /* size 0x1C */

/* One point of a trail. */
typedef struct TrailPoint {
    /* 0x00 */ vertex pos;
    /* 0x0C */ f32 velY;
} TrailPoint; /* size 0x10 */

/* Per-trail settings, copied from the spawner by func_1515C2F0. */
typedef struct TrailData {
    /* 0x00 */ f32 width;     /* half-width of the ribbon */
    /* 0x04 */ vertex vel;    /* initial point velocity; only vel.y is kept per point */
    /* 0x10 */ f32 gravity;
    /* 0x14 */ u8 flags;      /* 1: two-cycle mode; 4: fade out; 8: shrink */
    /* 0x15 */ u8 primMode;
    /* 0x16 */ u8 envMode;
    /* 0x17 */ u8 color[4];
    /* 0x1B */ u8 color2[4];
    /* 0x1F */ u8 colorLevel;
    /* 0x20 */ u8 renderMode; /* index into D_800A4AC8 */
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ s32 setGeometryMode;
    /* 0x28 */ s32 clearGeometryMode;
    /* 0x2C */ s16 fadeStart;  /* life below which the alpha fades */
    /* 0x2E */ s16 fadeRate;   /* alpha per unit of life */
    /* 0x30 */ s16 shrinkStart; /* life below which the width shrinks */
    /* 0x32 */ u8 shrinkPaused;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ f32 shrinkFactor; /* width multiplier per frame */
    /* 0x38 */ s8 unk38;      /* index into D_8008B080, -1: none */
    /* 0x39 */ s8 unk39;      /* index into D_8008B084, -1: none */
    /* 0x3A */ u8 pad3A[2];
} TrailData; /* size 0x3C */

/* A trail object made by func_15147A80: a ring buffer of points falling under gravity. */
typedef struct Trail {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ TrailHeader header;
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next point to write */
    /* 0x2F */ u8 pad2F[0x25];
    /* 0x54 */ vertex tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailData *data;
} Trail;

extern Trail *func_15147A80(TrailHeader *, s32, s32, s32, s32, s32, s32, s32, void *, s32, s32);

/* Spawns a trail with extraSize + 0x40 bytes of data and copies *data into it. */
Trail *func_1515C2F0(TrailHeader *header, s32 extraSize, TrailData *data, void *arg3, u8 arg4, s32 arg5) {
    Trail *trail;

    header->kind = 10;
    trail = func_15147A80(header, extraSize + 0x40, sizeof(TrailPoint), 8, 8, 8, 0, 0, arg3, arg4, arg5);
    if (trail == NULL) {
        return NULL;
    }
    memcpy(trail->data, data, sizeof(TrailData));
    return trail;
}

typedef s32 (*TrailHandler)(Trail *, TrailData *);
extern TrailHandler D_8008B080[];
extern TrailHandler D_8008B084[];

/* Moves every point by the trail's velocity under gravity, runs the unk38 handler and tracks the tail position. */
s32 func_1515C388(Trail *trail) {
    TrailData *data;
    u8 failed;
    TrailPoint *points;
    vertex oldest; // matching: copied but never read (the target keeps the copy)
    s32 i;
    TrailHandler unused; // matching: unused local; without it IDO allocates registers differently

    data = trail->data;
    failed = FALSE;
    points = trail->points;
    if (trail->count) {
        oldest = points[trail->tail].pos;
        i = trail->tail;
        while (i != trail->head) {
            points[i].pos.x += data->vel.x * D_800BE9A4;
            points[i].pos.y += points[i].velY * D_800BE9A4 + data->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
            points[i].pos.z += data->vel.z * D_800BE9A4;
            points[i].velY += data->gravity * D_800BE9A4;
            i++;
            if (i == trail->header.capacity) {
                i = 0;
            }
        }
    }
    if (data->unk38 != -1) {
        failed = D_8008B080[data->unk38](trail, data) == 0;
    }
    if (trail->count > 0) {
        trail->tailPos = points[trail->tail].pos;
    } else {
        trail->tailPos.x = 0.0f;
        trail->tailPos.y = 0.0f;
        trail->tailPos.z = 0.0f;
    }
    if (failed) {
        return 0;
    }
    return 1;
}

/* Runs the unk39 handler, then fades the alpha and shrinks the width as the trail's life runs out. */
s32 func_1515C534(Trail *trail) {
    TrailData *data;
    u8 failed;
    u32 frames;

    data = trail->data;
    failed = FALSE;
    if (data->unk39 != -1) {
        failed = D_8008B084[data->unk39](trail, data) == 0;
    }
    if (!failed) {
        if ((data->flags & 4) && trail->header.life < data->fadeStart) {
            s32 alpha = trail->header.life * data->fadeRate;

            if (alpha < data->color[3]) {
                data->color[3] = alpha;
            }
        }
        if ((data->flags & 8) && trail->header.life < data->shrinkStart && !data->shrinkPaused) {
            for (frames = D_800BE9E4; frames != 0; frames--) {
                data->width *= data->shrinkFactor;
            }
        }
    }
    if (failed) {
        return 0;
    }
    return 1;
}

extern u8 D_80090E78[];
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];
extern s32 D_800D2C9C;

extern void func_151D5D60(void *, s16, s32, Vtx **, u8 *);
extern void func_151441A4(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_151442FC(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Gfx *func_15142E24(Gfx *gfx, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8,
                          u8 *update, s32 argA);
extern Gfx *func_1513F4E4(Gfx *, s32, u8 *);
extern Gfx *func_15142B7C(Gfx *gfx, s32 setGeometryMode, s32 clearGeometryMode);
extern Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *update);
extern Gfx *func_15142CF0(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *update);
extern Gfx *func_15142FBC(Gfx *gfx, s32 otherModeH, s32 otherModeL, u8 *update);

/* Draws the trail as a camera-facing ribbon, newest point first, fading in alpha towards the tail. */
Gfx *func_1515C6F4(Trail *trail, Gfx *gfx, s16 view) {
    Vtx *vtx;
    s32 i;
    u8 update;
    vertex next;
    vertex cur;
    TrailData *data;
    TrailPoint *points;
    f32 *cam;
    s32 j;
    f32 headScale;
    s32 texStep;
    s32 alphaStep;
    s32 tex;
    s32 alphaScale;
    s32 alpha;
    s32 cycleMode;
    f32 toCamX;
    f32 toCamY;
    f32 toCamZ;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    f32 segX, segY, segZ;
    f32 sideX, sideY, sideZ;
    f32 lenSq;
    f32 scale;

    if (trail->count >= 2) {
        func_151D5D60(trail->unk84, view, (trail->header.capacity * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            data = trail->data;
            update = 1;
            cam = &D_800DBFF0[view].unk2F8;
            points = trail->points;
            func_151441A4(&primR, &primG, &primB, &primA, data->color2[0], data->color2[1], data->color2[2], data->color2[3],
                          data->color[0], data->color[1], data->color[2], data->color[3], data->colorLevel, data->primMode);
            func_151442FC(&envR, &envG, &envB, &envA, data->color2[0], data->color2[1], data->color2[2], data->color2[3],
                          data->color[0], data->color[1], data->color[2], data->color[3], data->colorLevel, data->envMode);
            gfx = func_15142E24(gfx, D_80090E78, 0, 0, 0, 0, 0x42, 0, 0, &update, 0x3E);
            gfx = func_1513F4E4(gfx, data->unk21, &update);
            gfx = func_15142B7C(gfx, data->setGeometryMode, data->clearGeometryMode);
            gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
            gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
            if (data->flags & 1) {
                cycleMode = G_CYC_2CYCLE;
            } else {
                cycleMode = G_CYC_1CYCLE;
            }
            gfx = func_15142FBC(gfx, cycleMode | G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CD_NOISE | G_AD_NOISE,
                                D_800A4AC8[data->renderMode].unk0 | D_800A4AC8[data->renderMode].unk4, &update);
            i = trail->head - 1;
            i = (i < 0) ? trail->header.capacity - 1 : i;
            j = i - 1;
            j = (j < 0) ? trail->header.capacity - 1 : j;
            cur = points[i].pos;
            next = points[j].pos;
            texStep = 0x800 / (trail->count - 1);
            alphaStep = 0xFF / (trail->count - 1);

            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            toCamX = cur.x - cam[0];
            toCamY = cur.y - cam[1];
            toCamZ = cur.z - cam[2];
            sideX = segY * toCamZ - toCamY * segZ;
            sideY = segZ * toCamX - toCamZ * segX;
            sideZ = segX * toCamY - toCamX * segY;
            lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
            if (lenSq == 0.0f) {
                sideX = 0.0f;
                sideY = 0.0f;
                sideZ = 0.0f;
            } else {
                headScale = data->width / sqrtf(lenSq);
                sideX *= headScale;
                sideY *= headScale;
                sideZ *= headScale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = 0x3FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            tex = texStep;
            alphaScale = alphaStep;
            do {
                i = j;
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                toCamX = next.x - cam[0];
                toCamY = next.y - cam[1];
                toCamZ = next.z - cam[2];
                sideX = segY * toCamZ - toCamY * segZ;
                sideY = segZ * toCamX - toCamZ * segX;
                sideZ = segX * toCamY - toCamX * segY;
                lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
                if (lenSq == 0.0f) {
                    sideX = 0.0f;
                    sideY = 0.0f;
                    sideZ = 0.0f;
                } else {
                    scale = data->width / sqrtf(lenSq);
                    sideX *= scale;
                    sideY *= scale;
                    sideZ *= scale;
                }
                alpha = (data->color[3] * alphaScale) >> 8;
                alphaScale += alphaStep;
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[0] = tex;
                vtx->v.tc[1] = 0x3FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[0] = tex;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                tex += texStep;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                j--;
                if (j < 0) {
                    j = trail->header.capacity - 1;
                }
                cur = next;
                next = points[j].pos;
            } while (i != trail->tail);
        }
    }
    return gfx;
}

/* Handler: adds a point at the trail's position, or stops the unk39 handler once the buffer is full. */
s32 func_1515CF9C(Trail *trail, TrailData *data) {
    if (trail->count < trail->header.capacity - 1) {
        TrailPoint *points;

        points = trail->points;
        trail->count++;
        points[trail->head].pos = trail->header.pos;
        points[trail->head].velY = data->vel.y;
        trail->head++;
        if (trail->head == trail->header.capacity) {
            trail->head = 0;
        }
    } else {
        data->unk39 = -1;
    }
    return 1;
}

/* Handler: drops the newest point while more than two remain; returns 0 once the trail is down to two. */
s32 func_1515D030(Trail *trail, TrailData *data) {
    s8 count;
    s32 result;

    count = trail->count;
    result = TRUE;
    if (count >= 3) {
        trail->count = count - 1;
        trail->head--;
        if (trail->head < 0) {
            trail->head = trail->header.capacity - 1;
        }
    } else {
        result = FALSE;
    }
    return result;
}

/* A level object placement (see game_40490.c). */
typedef struct LevelObject {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 sizeX;
    /* 0x08 */ s16 sizeY;
    /* 0x0A */ s16 sizeZ;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8 pad24[0x10];
} LevelObject;

/* Data block at 0x28 of the effect func_1515D088 spawns. */
typedef struct {
    /* 0x0 */ LevelObject *owner;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ s8 kind;
} LevelEffectData; /* size 0xC */

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ LevelEffectData data;
} LevelEffect;

/* Spawns an effect for the level object when its unk18 kind is 0 or 1; returns it, or NULL. */
LevelEffect *func_1515D088(LevelObject *obj) {
    s32 value; // matching: one variable holds the kind and then the effect (IDO's register and spill slot)
    LevelEffectData data;

    value = obj->unk18;
    data.kind = (u16)value; // matching: the u16 step keeps the loaded word apart from value & 0xFF
    if ((value & 0xFF) < 0 || (value & 0xFF) >= 2) {
        return NULL;
    }
    data.owner = obj;
    data.unk4 = 0.0f;
    value = (s32)func_151491F4(300, -1, 17, 0, 13, 12, 255, 1);
    if (value != 0) {
        memcpy(&((LevelEffect *)value)->data, &data, sizeof(data));
    }
    return (LevelEffect *)value;
}

/* Emitter origin: a position and the radius particles spawn at around it. */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 radius;
} EmitterOrigin;

/* Emitter state at 0x28 of its owner. */
typedef struct {
    /* 0x0 */ EmitterOrigin *origin;
    /* 0x4 */ f32 accum;  /* particles owed; one is emitted per whole unit */
    /* 0x8 */ u8 preset;  /* index into D_800A64A0 */
} EmitterState;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterState emitter;
} Emitter;

/* Emission preset in D_800A64A0. */
typedef struct {
    /* 0x00 */ f32 rateBase; /* particles per frame */
    /* 0x04 */ f32 rateRange;
    /* 0x08 */ s32 countBase;
    /* 0x0C */ s32 countRange;
    /* 0x10 */ f32 sizeBase;
    /* 0x14 */ f32 sizeRange;
    /* 0x18 */ f32 gravityBase;
    /* 0x1C */ f32 gravityRange;
    /* 0x20 */ f32 speedBase;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ s32 capacityBase;
    /* 0x2C */ s32 capacityRange;
    /* 0x30 */ s16 lifeBase;
    /* 0x32 */ s16 lifeRange;
    /* 0x34 */ s16 fadeStart;
    /* 0x36 */ s16 fadeRate;
    /* 0x38 */ s16 shrinkStart;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ f32 shrinkFactor;
} EmitterPreset; /* size 0x40 */

/* Burst description for func_15152B38, which spawns a randomised batch of trails with func_1515C2F0. */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ vertex pos;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 gravityBase;
    /* 0x20 */ f32 gravityRange;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ s16 yawBase;
    /* 0x2E */ s16 yawRange;
    /* 0x30 */ s16 pitchBase;
    /* 0x32 */ s16 pitchRange;
    /* 0x34 */ s32 capacityBase;
    /* 0x38 */ s32 capacityRange;
    /* 0x3C */ s16 lifeBase;
    /* 0x3E */ s16 lifeRange;
    /* 0x40 */ u16 headerFlags;
    /* 0x42 */ u8 flags;
    /* 0x43 */ u8 primMode;
    /* 0x44 */ u8 envMode;
    /* 0x45 */ u8 colorBase[4];
    /* 0x49 */ u8 colorRange[4];
    /* 0x4D */ u8 color2Base[4];
    /* 0x51 */ u8 color2Range[4];
    /* 0x55 */ u8 colorLevelBase;
    /* 0x56 */ u8 colorLevelRange;
    /* 0x57 */ u8 renderMode;
    /* 0x58 */ u8 unk21;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 setGeometryMode;
    /* 0x60 */ s32 clearGeometryMode;
    /* 0x64 */ s16 fadeStart;
    /* 0x66 */ s16 fadeRate;
    /* 0x68 */ s16 shrinkStart;
    /* 0x6A */ u8 shrinkPaused;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 shrinkFactor;
    /* 0x70 */ s8 unk38;
    /* 0x71 */ s8 unk39;
} TrailBurst;

extern EmitterPreset D_800A64A0[];
extern void func_15143794(s16 yaw, s16 pitch, f32 length, vertex *out);
extern void func_15152B38(TrailBurst *, u8, s32);

/* Emits trail bursts at random points on a sphere around the origin, at the preset's random rate. */
void func_1515D130(Emitter *owner) {
    EmitterPreset *preset;
    EmitterState *emitter;
    TrailBurst burst;
    s32 yaw;
    s32 pitch;

    preset = &D_800A64A0[owner->emitter.preset];
    emitter = &owner->emitter;
    emitter->accum += preset->rateBase + func_150ADA68() * preset->rateRange;

    while (emitter->accum > 1.0f) {
        burst.countBase = preset->countBase;
        burst.countRange = preset->countRange;
        burst.sizeBase = preset->sizeBase;
        burst.sizeRange = preset->sizeRange;
        burst.gravityBase = preset->gravityBase;
        burst.gravityRange = preset->gravityRange;
        burst.speedBase = preset->speedBase;
        burst.speedRange = preset->speedRange;
        burst.yawBase = 0;
        burst.yawRange = 255;
        burst.pitchBase = -63;
        burst.pitchRange = 80;
        burst.capacityBase = preset->capacityBase;
        burst.capacityRange = preset->capacityRange;
        burst.lifeBase = preset->lifeBase;
        burst.lifeRange = preset->lifeRange;
        burst.headerFlags = 1;
        burst.flags = 4 | 8;
        burst.primMode = 2;
        burst.envMode = 3;
        burst.colorBase[0] = 180;
        burst.colorBase[1] = 0;
        burst.colorBase[2] = 0;
        burst.colorBase[3] = 155;
        burst.colorRange[0] = 50;
        burst.colorRange[1] = 100;
        burst.colorRange[2] = 0;
        burst.colorRange[3] = 100;
        burst.color2Base[0] = 255;
        burst.color2Base[1] = 255;
        burst.color2Base[2] = 255;
        burst.color2Base[3] = 255;
        burst.color2Range[0] = 0;
        burst.color2Range[1] = 0;
        burst.color2Range[2] = 0;
        burst.color2Range[3] = 0;
        burst.colorLevelBase = 255;
        burst.colorLevelRange = 0;
        burst.renderMode = 1;
        burst.unk21 = 0x24;
        burst.setGeometryMode = 0x200005;
        burst.clearGeometryMode = 0x60600;
        burst.fadeStart = preset->fadeStart;
        burst.fadeRate = preset->fadeRate;
        burst.shrinkStart = preset->shrinkStart;
        burst.shrinkPaused = FALSE;
        burst.shrinkFactor = preset->shrinkFactor;
        burst.unk38 = -1;
        burst.unk39 = 0;
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794(yaw & 0xFF, 64 - (pitch & 0x7F), emitter->origin->radius, &burst.pos);
        burst.pos.x += emitter->origin->x;
        burst.pos.y += emitter->origin->y;
        burst.pos.z += emitter->origin->z;
        func_15152B38(&burst, owner->unkC, owner->unk1);
        emitter->accum -= 1.0f;
    }
}
