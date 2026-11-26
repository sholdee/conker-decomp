#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Spawns a trail with extraSize + 0x40 bytes of data and copies *data into it. */
FallingTrail *trail_spawnFallingTrail(EffectTemplate *header, s32 extraSize, TrailBurstParams *data, CollisionHit *hit, u8 arg4, s32 arg5) {
    FallingTrail *trail;

    header->kind = 10;
    trail = func_15147A80(header, extraSize + 0x40, sizeof(FallingTrailPoint), 8, 8, 8, 0, 0, hit, arg4, arg5);
    if (trail == NULL) {
        return NULL;
    }
    memcpy(trail->data, data, sizeof(TrailBurstParams));
    return trail;
}

typedef s32 (*TrailHandler)(FallingTrail *, TrailBurstParams *);
/* Handlers per TrailBurstParams.unk38 and unk39; each returns 0 when the trail fails. */
static TrailHandler gFallingTrailMoveProcs[] = { NULL };
static TrailHandler gFallingTrailShapeProcs[] = { trail_growFallingTrail, trail_shrinkFallingTrail };

/* Moves every point by the trail's velocity under gravity, runs the unk38 handler and tracks the tail position. */
s32 trail_moveFallingTrail(FallingTrail *trail) {
    TrailBurstParams *data;
    u8 failed;
    FallingTrailPoint *points;
    Vec3f oldest; // matching: copied but never read (the target keeps the copy)
    s32 i;
    TrailHandler unused; // matching: unused local; without it IDO allocates registers differently

    data = trail->data;
    failed = FALSE;
    points = trail->points;
    if (trail->head.count) {
        oldest = points[trail->head.start].pos;
        i = trail->head.start;
        while (i != trail->head.end) {
            points[i].pos.x += data->vel.x * gFrameDelta;
            points[i].pos.y += points[i].velY * gFrameDelta + data->gravity * gFrameDelta * gFrameDelta * 0.5f;
            points[i].pos.z += data->vel.z * gFrameDelta;
            points[i].velY += data->gravity * gFrameDelta;
            i++;
            if (i == trail->head.tmpl.itemCount) {
                i = 0;
            }
        }
    }
    if (data->unk38 != -1) {
        failed = gFallingTrailMoveProcs[data->unk38](trail, data) == 0;
    }
    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    if (failed) {
        return 0;
    }
    return 1;
}

/* Runs the unk39 handler, then fades the alpha and shrinks the width as the trail's life runs out. */
s32 trail_fadeFallingTrail(FallingTrail *trail) {
    TrailBurstParams *data;
    u8 failed;
    u32 frames;

    data = trail->data;
    failed = FALSE;
    if (data->unk39 != -1) {
        failed = gFallingTrailShapeProcs[data->unk39](trail, data) == 0;
    }
    if (!failed) {
        if ((data->flags & TRAIL_BURST_FADE) && trail->head.tmpl.life < data->fadeStart) {
            s32 alpha = trail->head.tmpl.life * data->fadeRate;

            if (alpha < data->color.alpha) {
                data->color.alpha = alpha;
            }
        }
        if ((data->flags & TRAIL_BURST_SHRINK) && trail->head.tmpl.life < data->shrinkStart && !data->shrinkPaused) {
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

extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

/*
 * Draws the trail as a camera-facing ribbon from the newest point (i) back to the tail, one quad per segment;
 * alpha and the texture s coordinate ramp up along the ribbon.
 */
Gfx *trail_drawFallingTrail(FallingTrail *trail, Gfx *gfx, s16 view) {
    Vtx *vtx;
    s32 i;
    u8 update;
    Vec3f next;
    Vec3f cur;
    TrailBurstParams *data;
    FallingTrailPoint *points;
    f32 *camPos;
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

    if (trail->head.count >= 2) {
        func_151D5D60(trail->vtxBuf, view, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            data = trail->data;
            update = 1;
            camPos = &camera_global_Cameras[view].eye.x;
            points = trail->points;
            func_151441A4(&primR, &primG, &primB, &primA, data->color2.red, data->color2.green, data->color2.blue, data->color2.alpha,
                          data->color.red, data->color.green, data->color.blue, data->color.alpha, data->colorLevel, data->primMode);
            func_151442FC(&envR, &envG, &envB, &envA, data->color2.red, data->color2.green, data->color2.blue, data->color2.alpha,
                          data->color.red, data->color.green, data->color.blue, data->color.alpha, data->colorLevel, data->envMode);
            gfx = func_15142E24(gfx, &gEffectTextures[66], 0, 0, 0, 0, 0x42, 0, 0, &update, 0x3E);
            gfx = func_1513F4E4(gfx, data->unk21, &update);
            gfx = func_15142B7C(gfx, data->setGeometryMode, data->clearGeometryMode);
            gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
            gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
            if (data->flags & TRAIL_BURST_TWO_CYCLE) {
                cycleMode = G_CYC_2CYCLE;
            } else {
                cycleMode = G_CYC_1CYCLE;
            }
            gfx = func_15142FBC(gfx, cycleMode | G_TP_PERSP | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CD_NOISE | G_AD_NOISE,
                                D_800A4AC8[data->renderMode].unk0 | D_800A4AC8[data->renderMode].unk4, &update);
            i = trail->head.end - 1;
            i = (i < 0) ? trail->head.tmpl.itemCount - 1 : i;
            j = i - 1;
            j = (j < 0) ? trail->head.tmpl.itemCount - 1 : j;
            cur = points[i].pos;
            next = points[j].pos;
            texStep = 0x800 / (trail->head.count - 1);
            alphaStep = 0xFF / (trail->head.count - 1);

            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            toCamX = cur.x - camPos[0];
            toCamY = cur.y - camPos[1];
            toCamZ = cur.z - camPos[2];
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
                toCamX = next.x - camPos[0];
                toCamY = next.y - camPos[1];
                toCamZ = next.z - camPos[2];
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
                alpha = (data->color.alpha * alphaScale) >> 8;
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
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = next;
                next = points[j].pos;
            } while (i != trail->head.start);
        }
    }
    return gfx;
}

/* Handler: adds a point at the trail's position, or stops the unk39 handler once the buffer is full. */
s32 trail_growFallingTrail(FallingTrail *trail, TrailBurstParams *data) {
    if (trail->head.count < trail->head.tmpl.itemCount - 1) {
        FallingTrailPoint *points;

        points = trail->points;
        trail->head.count++;
        points[trail->head.end].pos = trail->head.tmpl.pos;
        points[trail->head.end].velY = data->vel.y;
        trail->head.end++;
        if (trail->head.end == trail->head.tmpl.itemCount) {
            trail->head.end = 0;
        }
    } else {
        data->unk39 = -1;
    }
    return 1;
}

/* Handler: drops the newest point while more than two remain; returns 0 once the trail is down to two. */
s32 trail_shrinkFallingTrail(FallingTrail *trail, TrailBurstParams *data) {
    s8 count;
    s32 result;

    count = trail->head.count;
    result = TRUE;
    if (count >= 3) {
        trail->head.count = count - 1;
        trail->head.end--;
        if (trail->head.end < 0) {
            trail->head.end = trail->head.tmpl.itemCount - 1;
        }
    } else {
        result = FALSE;
    }
    return result;
}

/* Spawns an effect for the level object when its data1 kind is 0 or 1; returns it, or NULL. */
LevelEffect *trail_spawnLevelEffect(TriggerZone *obj) {
    s32 value; // matching: one variable holds the kind and then the effect (IDO's register and spill slot)
    LevelEffectData data;

    value = obj->data1;
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

/* Emission preset in gTrailBurstPresets. */
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
} TrailBurstPreset; /* size 0x40 */

/* Burst description for func_15152B38, which spawns a randomised batch of trails with trail_spawnFallingTrail. */

/* The emission presets (TrailBurstEmitter.emitter.preset); preset 0 emits nothing. */
static const TrailBurstPreset gTrailBurstPresets[] = {
    { 0 },
    { 0.0f, 0.03299999982f, 5, 15, 6.0f, 9.0f, -2.205000162f, 0.753000021f, 20.10000038f, 10.10000038f, 3, 4, 16, 9,
      6, 42, 6, { 0 }, 0.9900000691f },
};

/* Emits trail bursts at random points on a sphere around the origin, at the preset's random rate. */
void trail_updateBurstEmitter(TrailBurstEmitter *owner) {
    const TrailBurstPreset *preset;
    TrailBurstState *emitter;
    TrailBurst burst;
    s32 yaw;
    s32 pitch;

    preset = &gTrailBurstPresets[owner->emitter.preset];
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
        burst.angles.yawBase = 0;
        burst.angles.yawRange = 255;
        burst.angles.pitchBase = -63;
        burst.angles.pitchRange = 80;
        burst.capacityBase = preset->capacityBase;
        burst.capacityRange = preset->capacityRange;
        burst.lifeBase = preset->lifeBase;
        burst.lifeRange = preset->lifeRange;
        burst.headerFlags = 1;
        burst.flags = TRAIL_BURST_FADE | TRAIL_BURST_SHRINK;
        burst.primMode = 2;
        burst.envMode = 3;
        burst.colorBase.red = 180;
        burst.colorBase.green = 0;
        burst.colorBase.blue = 0;
        burst.colorBase.alpha = 155;
        burst.colorRange.red = 50;
        burst.colorRange.green = 100;
        burst.colorRange.blue = 0;
        burst.colorRange.alpha = 100;
        burst.color2Base.red = 255;
        burst.color2Base.green = 255;
        burst.color2Base.blue = 255;
        burst.color2Base.alpha = 255;
        burst.color2Range.red = 0;
        burst.color2Range.green = 0;
        burst.color2Range.blue = 0;
        burst.color2Range.alpha = 0;
        burst.colorLevelBase = 255;
        burst.colorLevelRange = 0;
        burst.renderMode = 1;
        burst.unk21 = 0x24;
        burst.setGeometryMode = G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH;
        burst.clearGeometryMode = G_CULL_BOTH | G_LIGHTING | G_TEXTURE_GEN;
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
        func_15152B38(&burst, owner->base.header.owner, owner->base.header.kind);
        emitter->accum -= 1.0f;
    }
}
