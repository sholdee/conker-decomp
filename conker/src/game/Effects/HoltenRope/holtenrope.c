#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define ROPE_FLAG_TIMED    1 // removed when `timer` runs out
#define ROPE_FLAG_RESET    2 // points are re-laid at rest by the next update callback
#define ROPE_FLAG_NO_START 4 // start position not known
#define ROPE_FLAG_NO_END   8 // end position not known

/* Endpoint callbacks (HoltenRopeEffect.startCallback / endCallback): s32 (rope, Vec3f *pos, u8 isStart),
 * each taking the rope as its own type's view; 0 ends the rope. */
static s32 (*gRopeEndpointCallbacks[])() = { rope_actorEndpoint, rope_tiedRopeEndpoint };

/* Update callbacks (HoltenRopeEffect.updateCallback); 0 ends the rope. */
static s32 (*gRopeUpdateCallbacks[])(HoltenRopeEffect *) = {
    rope_layStraight, rope_layHalfCircle, rope_layHalfEllipse, rope_swingHalfEllipse, rope_spaceAlongX, rope_spaceFixedX,
};

/* Draw styles (HoltenRopeEffect.drawStyle): s32 (rope, s32 *mode0, s32 *mode1, s32 *primR, s32 *primG, s32 *primB,
 * s32 *primA, s32 *envR, s32 *envG, s32 *envB, s32 *envA, s32 *cycleType, u8 *renderMode, u8 *unk13) returns the
 * draw settings of the rope's display list, nonzero if the rope should be drawn. */
static s32 (*gRopeDrawStyles[])() = { rope_drawStyleWhite, rope_drawStyleTinted };

/* Render setup and reset display lists of the ropes; the rope type's entry in the object type table
 * gObjectTypes points to them. */
Gfx gRopeRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx gRopeRenderResetDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5),
    gsSPEndDisplayList(),
};

/* Message and cleanup handlers per rope type (HoltenRopeEffect.type); the cleanup handlers take the rope as
 * their type's view. */
static void (*gRopeTypeMsgHandlers[])(HoltenRopeEffect *, s32, u8) = { NULL, NULL };
static void (*gRopeTypeCleanups[])() = { rope_unlinkTiedRope, NULL };

/* Allocates a rope effect with `extraSize` bytes of type data and initializes it from `desc`. */
HoltenRopeEffect *rope_createEffect(HoltenRopeDesc *desc, f32 texScale, s32 extraSize, u8 arg3, s32 arg4) {
    HoltenRopeEffect *effect;
    f32 length;
    f32 halfAxesSq;

    effect = effect_newObject(0x33, arg4, extraSize + sizeof(HoltenRopeEffect), 1, arg3, 1);
    if (effect == NULL) {
        return NULL;
    }

    memcpy(&effect->flags, desc, sizeof(HoltenRopeDesc));
    length = effect->length;
    effect->flags |= ROPE_FLAG_RESET | ROPE_FLAG_NO_START | ROPE_FLAG_NO_END;
    halfAxesSq = (length * length) / 9.869605064f; // pi^2
    effect->axesSq = halfAxesSq + halfAxesSq;
    effect->texStep = length * texScale * 4096.0f;
    bzero(effect->vtxBuffer, sizeof(effect->vtxBuffer));
    return effect;
}

/* Per-frame update: counts down the lifetime, refreshes both endpoints and moves the points. */
void rope_updateEffect(HoltenRopeEffect *effect) {
    s32 callback;
    u8 finished;

    finished = FALSE;
    if (effect->flags & ROPE_FLAG_TIMED) {
        effect->timer -= D_800BE9E4;
        if (effect->timer < 0) {
            finished = TRUE;
        }
    }
    if (!finished) {
        callback = effect->startCallback;
        if (callback != -1) {
            if (!gRopeEndpointCallbacks[callback](effect, &effect->start, TRUE)) {
                finished = TRUE;
            }
        }
        callback = effect->endCallback;
        if (callback != -1) {
            if (!gRopeEndpointCallbacks[callback](effect, &effect->end, FALSE)) {
                finished = TRUE;
            }
        }
        if ((effect->flags & ROPE_FLAG_NO_START) || (effect->flags & ROPE_FLAG_NO_END)) {
            effect->flags |= ROPE_FLAG_RESET;
        } else {
            callback = effect->updateCallback;
            if (callback != -1) {
                if (!gRopeUpdateCallbacks[callback](effect)) {
                    finished = TRUE;
                }
            }
        }
    }
    if (finished) {
        effect_destroyObject(&effect->header);
    }
}

extern struct {
    s32 unk0;
    s32 unk4;
} gEffectRenderModes[];

/*
 * Draws the rope for view `view` as a camera-facing strip: each point gets a vertex
 * pair offset by `halfWidth` along the side vector (segment x eye-to-point).
 */
Gfx *rope_drawEffect(Gfx *gfx, HoltenRopeEffect *effect, s16 view) {
    s32 i;
    Vtx *vtx;
    Vec3f a;
    Vec3f b;
    s32 texT;
    f32 *eye;
    f32 toEyeX;
    f32 toEyeY;
    f32 toEyeZ;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    f32 lenSq;
    f32 pad[2]; // matching: stack slots

    if (effect->flags & ROPE_FLAG_NO_START) {
        return gfx;
    }
    if (effect->flags & ROPE_FLAG_NO_END) {
        return gfx;
    }
    if (effect->flags & ROPE_FLAG_RESET) {
        return gfx;
    }
    particle_getViewBuffer(effect->vtxBuffer, view, 25 * sizeof(Vtx), &vtx, NULL);
    if (vtx != NULL) {
        u8 needSync;
        s32 mode0;
        s32 mode1;
        s32 primR;
        s32 primG;
        s32 primB;
        s32 primA;
        s32 envR;
        s32 envG;
        s32 envB;
        s32 envA;
        s32 cycleType;
        u8 renderMode;
        u8 unk13;

        eye = &camera_global_Cameras[view].eye.x;
        needSync = TRUE;
        {
            HoltenRopePoint *points;

            if (!gRopeDrawStyles[effect->drawStyle](effect, &mode0, &mode1, &primR, &primG, &primB, &primA, &envR, &envG, &envB, &envA, &cycleType, &renderMode, &unk13)) {
                return gfx;
            }
            points = effect->points;
            gfx = math_gfxUpdateGeometryMode(gfx, mode0, mode1);
            gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &needSync);
            gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &needSync);
            gfx = particle_setCombineMode(gfx, unk13, &needSync);
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[54], 0, 0, 0, 0, 0x36, 0, 0, &needSync, 3);
            gfx = math_gfxSetOtherMode(gfx, cycleType | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[renderMode].unk0 | gEffectRenderModes[renderMode].unk4, &needSync);
            {
                f32 pad3; // matching: a local here opens a -g3 scope that IDO will not schedule across

                /* first pair: side vector of the first segment, placed at its start */
                a = points[0].pos;
                b = points[1].pos;
                dx = b.x - a.x;
                dy = b.y - a.y;
                dz = b.z - a.z;
                toEyeX = a.x - eye[0];
                toEyeY = a.y - eye[1];
                toEyeZ = a.z - eye[2];
                sideX = (dy * toEyeZ) - (toEyeY * dz);
                sideY = (dz * toEyeX) - (toEyeZ * dx);
                sideZ = (dx * toEyeY) - (toEyeX * dy);
                lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                if (lenSq == 0.0f) {
                    sideX = 0.0f;
                    sideY = 0.0f;
                    sideZ = 0.0f;
                } else {
                    dz = effect->halfWidth / sqrtf(lenSq);
                    sideX *= dz;
                    sideY *= dz;
                    sideZ *= dz;
                }
                vtx->v.ob[0] = a.x + sideX;
                vtx->v.ob[1] = a.y + sideY;
                vtx->v.ob[2] = a.z + sideZ;
                vtx->v.tc[0] = 0;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 255;
                vtx->v.cn[1] = 100;
                vtx->v.cn[2] = 100;
                vtx->v.cn[3] = 255;
                vtx++;
                vtx->v.ob[0] = a.x - sideX;
                vtx->v.ob[1] = a.y - sideY;
                vtx->v.ob[2] = a.z - sideZ;
                vtx->v.tc[0] = 0x3C0;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 255;
                vtx->v.cn[1] = 100;
                vtx->v.cn[2] = 100;
                vtx->v.cn[3] = 255;
                vtx++;
                /* then one pair per point at the end of each segment, and two triangles joining it to the previous pair */
                texT = effect->texStep;
                for (i = 1; i < 10; i++) {
                    a = points[i - 1].pos;
                    b = points[i].pos;
                    dx = b.x - a.x;
                    dy = b.y - a.y;
                    dz = b.z - a.z;
                    toEyeX = b.x - eye[0];
                    toEyeY = b.y - eye[1];
                    toEyeZ = b.z - eye[2];
                    sideX = (dy * toEyeZ) - (toEyeY * dz);
                    sideY = (dz * toEyeX) - (toEyeZ * dx);
                    sideZ = (dx * toEyeY) - (toEyeX * dy);
                    lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                    if (lenSq == 0.0f) {
                        sideX = 0.0f;
                        sideY = 0.0f;
                        sideZ = 0.0f;
                    } else {
                        dz = effect->halfWidth / sqrtf(lenSq);
                        sideX *= dz;
                        sideY *= dz;
                        sideZ *= dz;
                    }
                    vtx->v.ob[0] = b.x + sideX;
                    vtx->v.ob[1] = b.y + sideY;
                    vtx->v.ob[2] = b.z + sideZ;
                    vtx->v.tc[0] = 0;
                    vtx->v.tc[1] = texT;
                    vtx->v.cn[0] = 255;
                    vtx->v.cn[1] = 100;
                    vtx->v.cn[2] = 100;
                    vtx->v.cn[3] = 255;
                    vtx++;
                    vtx->v.ob[0] = b.x - sideX;
                    vtx->v.ob[1] = b.y - sideY;
                    vtx->v.ob[2] = b.z - sideZ;
                    vtx->v.tc[0] = 0x3C0;
                    vtx->v.tc[1] = texT;
                    vtx->v.cn[0] = 255;
                    vtx->v.cn[1] = 100;
                    vtx->v.cn[2] = 100;
                    vtx->v.cn[3] = 255;
                    vtx++;
                    texT += effect->texStep;
                    gSPVertex(gfx++, vtx - 4, 4, 0);
                    gSP1Triangle(gfx++, 0, 1, 2, 0);
                    gSP1Triangle(gfx++, 1, 3, 2, 0);
                }
            }
        }
    }
    return gfx;
}

/* Calls the rope type's handler from gRopeTypeMsgHandlers, if it has one. */
void rope_effectMessage(HoltenRopeEffect *effect, s32 arg1, u8 arg2) {
    void (*handler)(HoltenRopeEffect *, s32, u8) = gRopeTypeMsgHandlers[effect->type];
    if (handler != NULL) {
        handler(effect, arg1, arg2);
    }
}

/* Update callback: lays the points out in a straight line from start to end, at rest. */
s32 rope_layStraight(HoltenRopeEffect *effect) {
    s32 pad[4]; // matching: stack slots
    Vec3f pos;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 i;

    dx = effect->end.x - effect->start.x;
    dy = effect->end.y - effect->start.y;
    dz = effect->end.z - effect->start.z;
    pos = effect->start;
    for (i = 0; i < 10; i++) {
        HoltenRopePoint *point = &effect->points[i];
        point->pos = pos;
        point->vel.x = 0.0f;
        point->vel.y = 0.0f;
        point->vel.z = 0.0f;
        pos.x += dx * (1.0f / 9.0f);
        pos.y += dy * (1.0f / 9.0f);
        pos.z += dz * (1.0f / 9.0f);
    }
    effect->flags &= ~ROPE_FLAG_RESET;
    return TRUE;
}

/*
 * Update callback: lays the points on a half circle whose diameter is the start-end segment,
 * in the vertical plane through both ends.
 */
s32 rope_layHalfCircle(HoltenRopeEffect *effect) {
    Vec3f delta;
    Vec3f mid;
    Vec3f relStart;
    Vec3f relEnd;
    f32 invDist;
    f32 dirZ;
    f32 dirX;
    f32 across;
    f32 up;
    f32 heading;
    f32 angle;
    f32 radius;
    s32 i;

    delta.x = effect->end.x - effect->start.x;
    delta.y = effect->end.y - effect->start.y;
    delta.z = effect->end.z - effect->start.z;
    if ((0.0001f < fabsf(delta.x)) || (0.0001f < fabsf(delta.z))) {
        mid.x = effect->start.x + (delta.x * 0.5f);
        mid.y = effect->start.y + (delta.y * 0.5f);
        mid.z = effect->start.z + (delta.z * 0.5f);
        relStart.x = effect->start.x - mid.x;
        relStart.y = effect->start.y - mid.y;
        relStart.z = effect->start.z - mid.z;
        relEnd.x = effect->end.x - mid.x;
        relEnd.y = effect->end.y - mid.y;
        relEnd.z = effect->end.z - mid.z;
        invDist = 1.0f / sqrtf((delta.x * delta.x) + (delta.z * delta.z));
        dirZ = delta.z * invDist;
        dirX = delta.x * invDist;
        across = ((relEnd.z * dirZ) + (relEnd.x * dirX)) - ((relStart.z * dirZ) + (relStart.x * dirX));
        up = relEnd.y - relStart.y;
        heading = math_atan2(up, across);
        radius = sqrtf((across * across) + (up * up)) * 0.5f;
        angle = heading - 3.1415927f;
        for (i = 0; i < 10; i++, angle += 0.34906587f) { // 20 degrees
            HoltenRopePoint *point = &effect->points[i];
            f32 s = sinf(angle);
            f32 c = cosf(angle);

            point->pos.x = (radius * c * dirX) + mid.x;
            point->pos.y = (radius * s) + mid.y;
            point->pos.z = (radius * c * dirZ) + mid.z;
        }
    }
    return TRUE;
}

/*
 * Endpoint callback for a rope hung from an actor: the start follows the actor
 * while it is still the one recorded at creation, the end hangs at a fixed point.
 */
s32 rope_actorEndpoint(HoltenRopeAttached *rope, Vec3f *pos, u8 isStart) {
    s32 ret;

    ret = TRUE;
    if (isStart) {
        HoltenRopeAnchor *anchor = &rope->anchor;

        if (anchor->target->type != 0 && anchor->target->unique_id == anchor->targetId) {
            pos->x = anchor->target->xpos;
            pos->y = anchor->target->ypos;
            pos->z = anchor->target->zpos;
            rope->effect.flags &= ~ROPE_FLAG_NO_START;
        } else {
            ret = FALSE;
            rope->effect.flags |= ROPE_FLAG_NO_START | ROPE_FLAG_NO_END;
        }
    } else {
        pos->x = 0.0f;
        pos->y = 3000.0f;
        pos->z = 0.0f;
        rope->effect.flags &= ~ROPE_FLAG_NO_END;
    }
    return ret;
}

/*
 * Update callback: hangs the points on a half ellipse below the start-end chord whose arc
 * length is about `length`; falls back to a straight line when the ends are too far apart.
 */
s32 rope_layHalfEllipse(HoltenRopeEffect *effect) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f mid;
    Vec3f relStart;
    Vec3f relEnd;
    f32 horizDist;
    f32 inv;
    f32 dirZ;
    f32 dirX;
    f32 pad1;    // matching: stack slot
    f32 across;
    f32 up;
    f32 pad2[7]; // matching: stack slots
    f32 semiMajor;
    f32 chord;
    f32 chordSq;
    f32 chordX;
    f32 chordY;
    f32 semiMinor;
    f32 angle;
    f32 sinA;
    f32 cosA;
    f32 ellipseX;
    f32 ellipseY;
    f32 acrossOffset;
    s32 i;
    HoltenRopePoint *points;

    dx = effect->end.x - effect->start.x;
    dy = effect->end.y - effect->start.y;
    dz = effect->end.z - effect->start.z;
    if ((fabsf(dx) > 0.0001f) || (fabsf(dz) > 0.0001f)) {
        f32 halfChordSq;

        mid.x = effect->start.x + (dx * 0.5f);
        mid.y = effect->start.y + (dy * 0.5f);
        mid.z = effect->start.z + (dz * 0.5f);
        relStart.x = effect->start.x - mid.x;
        relStart.y = effect->start.y - mid.y;
        relStart.z = effect->start.z - mid.z;
        relEnd.x = effect->end.x - mid.x;
        relEnd.y = effect->end.y - mid.y;
        relEnd.z = effect->end.z - mid.z;
        horizDist = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / horizDist;
        dirZ = dz * inv;
        dirX = dx * inv;
        across = ((relEnd.z * dirZ) + (relEnd.x * dirX)) - ((relStart.z * dirZ) + (relStart.x * dirX));
        up = relEnd.y - relStart.y;
        chordSq = (across * across) + (up * up);
        chord = sqrtf(chordSq);
        inv = 1.0f / chord;
        semiMajor = chord * 0.5f;
        chordX = across * inv;
        chordY = up * inv;
        halfChordSq = chordSq * 0.25f;
        if (effect->axesSq < halfChordSq) {
            return rope_layStraight(effect);
        }
        semiMinor = sqrtf(effect->axesSq - halfChordSq);
        angle = 0.0f;
        points = effect->points;
        for (i = 0; i < 10; i++, angle += 0.34906587f) { // 20 degrees
            HoltenRopePoint *point = &points[i];

            sinA = sinf(angle);
            cosA = cosf(angle);
            ellipseX = semiMajor * cosA;
            ellipseY = semiMinor * sinA;
            acrossOffset = (ellipseX * chordX) - (ellipseY * chordY);
            point->pos.x = (acrossOffset * dirX) + mid.x;
            point->pos.y = ((ellipseX * chordY) - (ellipseY * chordX)) + mid.y;
            point->pos.z = (acrossOffset * dirZ) + mid.z;
        }
    }
    return TRUE;
}

/*
 * Update callback: the half ellipse of rope_layHalfEllipse, but each point keeps part of its offset
 * from last frame (most in the middle of the rope) and is integrated with a velocity, so the
 * rope swings. Points snap to the curve on a reset (flag 2) or while gCameraSnapTimers is set.
 */
s32 rope_swingHalfEllipse(HoltenRopeEffect *effect) {
    u8 ret;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f mid;
    Vec3f relStart;
    Vec3f relEnd;
    f32 horizDist;
    f32 inv;
    f32 pad3[5];  // matching: stack slots
    f32 dirZ;
    f32 dirX;
    f32 accelZ;
    f32 chordX;
    f32 chordY;
    f32 across;
    f32 up;
    f32 semiMajor;
    f32 chord;
    f32 chordSq;
    f32 pad4[2];  // matching: stack slots
    f32 semiMinor;
    f32 weightAngle;
    f32 pad5;     // matching: stack slot
    f32 sinA;
    f32 cosA;
    f32 weight;
    f32 ellipseX;
    f32 ellipseY;
    f32 acrossOffset;
    f32 pad6;     // matching: stack slot
    Vec3f prev;
    Vec3f target;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 pad7[2];  // matching: stack slots
    f32 accelX;
    f32 accelY;
    s32 i;
    HoltenRopePoint *points;

    ret = TRUE;
    dx = -effect->start.x + effect->end.x;
    dy = -effect->start.y + effect->end.y;
    dz = effect->end.z - effect->start.z;
    if ((fabsf(dx) > 0.0001f) || (fabsf(dz) > 0.0001f)) {
        f32 halfChordSq;

        mid.x = effect->start.x + (dx * 0.5f);
        mid.y = effect->start.y + (dy * 0.5f);
        mid.z = effect->start.z + (dz * 0.5f);
        relStart.x = effect->start.x - mid.x;
        relStart.y = effect->start.y - mid.y;
        relStart.z = effect->start.z - mid.z;
        relEnd.x = effect->end.x - mid.x;
        relEnd.y = effect->end.y - mid.y;
        relEnd.z = effect->end.z - mid.z;
        horizDist = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / horizDist;
        dirZ = dz * inv;
        dirX = dx * inv;
        across = ((relEnd.z * dirZ) + (relEnd.x * dirX)) - ((relStart.z * dirZ) + (relStart.x * dirX));
        up = relEnd.y - relStart.y;
        chordSq = (across * across) + (up * up);
        chord = sqrtf(chordSq);
        inv = 1.0f / chord;
        semiMajor = chord * 0.5f;
        chordX = across * inv;
        chordY = up * inv;
        halfChordSq = chordSq * 0.25f;
        if (effect->axesSq < halfChordSq) {
            ret = rope_layStraight(effect);
        } else {
            f32 angle;

            semiMinor = sqrtf(effect->axesSq - halfChordSq);
            points = effect->points;
            for (i = 0, angle = 0.0f, weightAngle = 0.0f; i < 10; i++, angle += 0.34906587f, weightAngle += 0.34906587f) {
                HoltenRopePoint *point = &points[i];
                prev = point->pos;
                sinA = sinf(angle);
                cosA = cosf(angle);
                weight = sinf(weightAngle);
                ellipseX = semiMajor * cosA;
                ellipseY = semiMinor * sinA;
                acrossOffset = (ellipseX * chordX) - (ellipseY * chordY);
                target.x = (acrossOffset * dirX) + mid.x;
                target.y = ((ellipseX * chordY) - (ellipseY * chordX)) + mid.y;
                target.z = (acrossOffset * dirZ) + mid.z;
                if (effect->flags & ROPE_FLAG_RESET) {
                    point->pos = target;
                    point->vel.x = 0.0f;
                    point->vel.y = 0.0f;
                    point->vel.z = 0.0f;
                } else if (*(s32 *)gCameraSnapTimers != 0) {
                    point->pos = target;
                    point->vel = gZeroVec3f;
                } else {
                    offsetX = (prev.x - target.x) * weight * effect->lagScale;
                    offsetY = (prev.y - target.y) * weight * effect->lagScale;
                    offsetZ = (prev.z - target.z) * weight * effect->lagScale;
                    point->pos.x = target.x + offsetX;
                    point->pos.y = target.y + offsetY;
                    point->pos.z = target.z + offsetZ;
                    accelX = effect->accelScale * offsetX;
                    accelY = effect->accelScale * offsetY;
                    accelZ = effect->accelScale * offsetZ;
                    point->pos.x += (point->vel.x * gFrameDelta) + (0.5f * accelX * gFrameDelta * gFrameDelta);
                    point->pos.y += (point->vel.y * gFrameDelta) + (0.5f * accelY * gFrameDelta * gFrameDelta);
                    point->pos.z += (point->vel.z * gFrameDelta) + (0.5f * accelZ * gFrameDelta * gFrameDelta);
                    point->vel.x += accelX * gFrameDelta;
                    point->vel.y += accelY * gFrameDelta;
                    point->vel.z += accelZ * gFrameDelta;
                }
            }
        }
    }
    effect->flags &= ~ROPE_FLAG_RESET;
    return ret;
}

/*
 * Places one end of a rope tied between two bones (called through rope_tiedRopeEndpoint). Returns 0 once
 * either actor is gone; while either has no bone matrices yet, both ends are marked lost.
 */
s32 rope_placeTiedEnd(HoltenRopeEffect *effect, RopeTieLink *anchors, Vec3f *pos, u8 isStart) {
    Actor *startTarget = anchors->start.target;
    Actor *endTarget = anchors->end.target;

    if (startTarget->ob_matrix_ptr == NULL || endTarget->ob_matrix_ptr == NULL) {
        effect->flags |= ROPE_FLAG_NO_START | ROPE_FLAG_NO_END;
        return TRUE;
    }
    if (startTarget->type == 0 || startTarget->unique_id != anchors->start.targetId || endTarget->type == 0 || endTarget->unique_id != anchors->end.targetId) {
        return FALSE;
    }
    effect->flags &= ~(isStart ? ROPE_FLAG_NO_START : ROPE_FLAG_NO_END);
    math_transformOffset(isStart ? &anchors->start.offset : &anchors->end.offset, pos,
                  isStart ? &startTarget->ob_matrix_ptr[anchors->start.bone] : &endTarget->ob_matrix_ptr[anchors->end.bone]);
    return TRUE;
}

/* Update callback: spaces the points along x from start.x towards end.x, in tenths. */
s32 rope_spaceAlongX(HoltenRopeEffect *effect) {
    f32 startX;
    f32 endX;
    f32 step;
    f32 x;
    s32 i;

    startX = effect->start.x;
    endX = effect->end.x;
    step = -(startX - endX) * 0.1f;
    x = startX;
    for (i = 0; i < 10; i++, x += step) {
        HoltenRopePoint *point = &effect->points[i];

        point->pos.x = x;
        point->pos.y = 0.0f;
        point->pos.z = 0.0f;
    }
    effect->flags &= ~ROPE_FLAG_RESET;
    return TRUE;
}

/* Draw style 0: white, one-cycle. */
s32 rope_drawStyleWhite(HoltenRopeEffect *effect, s32 *mode0, s32 *mode1,
                   s32 *primR, s32 *primG, s32 *primB, s32 *primA,
                   s32 *envR, s32 *envG, s32 *envB, s32 *envA,
                   s32 *cycleType, u8 *renderMode, u8 *unk13) {
    *mode0 = 0x220005;
    *mode1 = 0x40600;
    *primR = 255;
    *primG = 255;
    *primB = 255;
    *primA = 255;
    *envR = 255;
    *envG = 255;
    *envB = 255;
    *envA = 255;
    *cycleType = G_CYC_1CYCLE;
    *renderMode = 5;
    *unk13 = 0x2B;
    return TRUE;
}

/* Draw style 1: two-cycle, tinted with the color source's color while bit 0 of its unkA4 is set; style 0 otherwise. */
s32 rope_drawStyleTinted(HoltenRopeTinted *rope, s32 *mode0, s32 *mode1,
                 s32 *primR, s32 *primG, s32 *primB, s32 *primA,
                 s32 *envR, s32 *envG, s32 *envB, s32 *envA,
                 s32 *cycleType, u8 *renderMode, u8 *unk13) {
    u8 ret;
    s32 pad[2]; // matching: stack slots
    Actor *source;
    s32 r;
    s32 g;
    s32 b;
    u32 a; // matching: actor_getFogColour writes an s32; as an s32 here alpha below takes another register
    u32 alpha;

    ret = TRUE;
    source = rope->colorSource;
    actor_getFogColour(source, &r, &g, &b, &a);
    if (source->fogMode & 1) {
        *mode0 = 0x200005;
        *mode1 = 0x60600;
        *primR = r;
        *primG = g;
        *primB = b;
        *primA = 255;
        *envB = alpha = a;
        *envG = alpha;
        *envR = alpha;
        *envA = 255;
        *cycleType = G_CYC_2CYCLE;
        *renderMode = 5;
        *unk13 = 0x2F;
    } else {
        ret = rope_drawStyleWhite(&rope->effect, mode0, mode1, primR, primG, primB, primA, envR, envG, envB, envA, cycleType, renderMode, unk13);
    }
    return ret;
}

/* Update callback: spaces the points 2000 units along x, from x = -1000. */
s32 rope_spaceFixedX(HoltenRopeEffect *effect) {
    f32 x;
    s32 i;

    x = -1000.0f;
    for (i = 0; i < 10; i++, x += 2000.0f / 9.0f) {
        HoltenRopePoint *point = &effect->points[i];

        point->pos.x = x;
        point->pos.y = 0.0f;
        point->pos.z = 0.0f;
    }
    effect->flags &= ~ROPE_FLAG_RESET;
    return TRUE;
}

/* Frees the rope's vertex buffers and runs the rope type's cleanup handler, if any. */
void rope_cleanupEffect(HoltenRopeEffect *effect) {
    void (*handler)(HoltenRopeEffect *);

    particle_freeViewBuffers(effect->vtxBuffer);
    handler = gRopeTypeCleanups[effect->type];
    if (handler != NULL) {
        handler(effect);
    }
}

void rope_destroyEffect(HoltenRopeEffect *effect) {
    rope_cleanupEffect(effect);
    effect_defaultFree(&effect->header);
}

void func_151B4C98(HoltenRopeEffect *effect) {
    rope_cleanupEffect(effect);
    effect_defaultFree(&effect->header);
}
