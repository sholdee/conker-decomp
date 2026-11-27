#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Trails left behind an actor, following the surface below it. A SurfaceTrailOwner keeps one trail per
 * side (side 6 and side 7). Each SurfaceTrail is a ring buffer of SurfaceTrailPoints that age, drift outwards
 * and fade.
 * When a trail loses its attach point it is detached, and a TrailSpawner waits to start a new one.
 */

#define TRAIL_SIDE_6 6
#define TRAIL_SIDE_7 7

struct SurfaceTrail;
struct TrailSpawner;

/* Computes where side `side`'s trail attaches to the owner's actor; returns 0 if the actor is gone. */
s32 trail_getSurfaceAttachPoint(config, side, out)
    SurfaceTrailConfig *config;
    u8 side;
    Vec3f *out;
{
    Actor *actor;
    TrailSet *trails;
    f32 dx;
    f32 dz;
    f32 dirX;
    f32 dirZ;
    f32 dist;

    trails = &config->owner->trails;
    actor = config->owner->watcher.character;
    if (actor->ob_matrix_ptr == NULL) {
        return 0;
    }
    dx = actor->xpos - config->actorX;
    dz = actor->zpos - config->actorZ;
    if ((0.0001f < fabsf(dx)) || (0.0001f < fabsf(dz))) {
        f32 inv;

        inv = 1.0f / sqrtf((dx * dx) + (dz * dz));
        dirX = dz * inv;
        dirZ = dx * inv;
    } else {
        u8 angle;

        angle = math_getActorFacingBin(actor);
        dirX = math_cosBinAngleMasked(angle);
        dirZ = math_cosBinAngleMasked(angle - 0x40);
    }
    if (side == TRAIL_SIDE_6) {
        dist = trails->sideOffset;
    } else {
        dist = -trails->sideOffset;
    }
    out->x = actor->xpos + (dist * dirX);
    out->z = actor->zpos - (dist * dirZ);
    if (actor->waterHeight < -9500.0f) {
        out->y = actor->ypos;
    } else {
        out->y = actor->waterHeight;
    }
    return 1;
}

/* Clears the owner's reference to this trail, then hands it to effect_defaultDestroyEffectObj. */
void trail_destroySurfaceTrail(SurfaceTrail *trail) {
    SurfaceTrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->head.tmpl.kind == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->head.tmpl.kind == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
    }
    effect_defaultDestroyEffectObj(trail);
}

/* Clears the owner's reference to this trail, then hands it to effect_defaultFreeEffectObj. */
void trail_freeSurfaceTrail(SurfaceTrail *trail) {
    SurfaceTrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->head.tmpl.kind == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->head.tmpl.kind == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
    }
    effect_defaultFreeEffectObj(trail);
}

/* Creates the trail for `side` behind the owner's actor. */
SurfaceTrail *trail_createSurfaceTrail(owner, side)
    SurfaceTrailOwner *owner;
    u8 side;
{
    Actor *actor;
    EffectTemplate header;
    SurfaceTrailConfig config;
    SurfaceTrail *trail;
    u8 colorIndex;
    u32 sideWord; // matching: a widened copy of side, narrowed again for trail_getSurfaceAttachPoint (andi)

    actor = owner->watcher.character;
    colorIndex = particle_getContextEffectKind();
    config.owner = owner;
    header.flags = 0x42;
    header.life = 300;
    header.itemCount = 10;
    config.pointLife = 200;
    config.flags = 2 | 4;
    config.tailFadeCount = 3;
    config.tailFadeStep = 0x55;
    config.headFadeCount = 3;
    config.headFadeStep = 0x55;
    config.texSStep = 0x88;
    config.texSStepRandom = 0xC4;
    config.texS = 0;
    config.texT = 0;
    config.pointAlpha = 255;
    config.pointFadeDelay = 40;
    config.alphaDecay = 25;
    config.texTSpeed = 6;
    config.texTSpan = 0x325;
    config.pointsPerUnit = 0.0150000006f;
    config.startWidth = 5.800000191f;
    config.widthGrowth = 1.0f;
    config.pendingPoints = 0.0f;
    config.emitTime = 0.0f;
    sideWord = side;
    header.kind = sideWord;
    config.driftSpeed = 1.5f;
    config.actorX = actor->xpos;
    config.actorZ = actor->zpos;
    if (trail_getSurfaceAttachPoint(&config, (u8)sideWord, &header.pos) != 0) {
        config.emitPos = header.pos;
        header.flags |= 4;
    }
    config.color[0] = gKindColors[colorIndex].r;
    config.color[1] = gKindColors[colorIndex].g;
    config.color[2] = gKindColors[colorIndex].b;
    trail = effect_newEffectObj(&header, sizeof(SurfaceTrailConfig), sizeof(SurfaceTrailPoint), 5, 5, 5, 0, 0, &owner->watcher.hit, 0xFF, 1);
    if (trail != NULL) {
        memcpy(trail->config, &config, sizeof(SurfaceTrailConfig));
    }
    return trail;
}

/* Gives the owner a new trail on both sides. */
void trail_startSurfaceTrails(SurfaceTrailOwner *owner) {
    TrailSide *sides = owner->trails.sides;

    sides[0].trail = trail_createSurfaceTrail(owner, TRAIL_SIDE_6);
    sides[0].spawner = NULL;
    sides[1].trail = trail_createSurfaceTrail(owner, TRAIL_SIDE_7);
    sides[1].spawner = NULL;
}

/* Lets go of both trails and deletes any pending spawners. */
void trail_releaseSurfaceTrails(owner)
    SurfaceTrailOwner *owner;
{
    TrailSide *sides = owner->trails.sides;

    if (sides[0].trail != NULL) {
        trail_detachSurfaceTrail(sides[0].trail);
    }
    if (sides[1].trail != NULL) {
        trail_detachSurfaceTrail(sides[1].trail);
    }
    if (sides[0].spawner != NULL) {
        trail_unlinkSurfaceSpawner(sides[0].spawner);
        effect_destroyObject(&sides[0].spawner->base.header);
    }
    if (sides[1].spawner != NULL) {
        trail_unlinkSurfaceSpawner(sides[1].spawner);
        effect_destroyObject(&sides[1].spawner->base.header);
    }
}

/* Detaches the trail from its owner; the points already laid stay and fade out. */
void trail_detachSurfaceTrail(SurfaceTrail *trail) {
    SurfaceTrailConfig *config = trail->config;
    TrailSide *sides;

    if (config->owner != NULL) {
        sides = config->owner->trails.sides;
        if (trail->head.tmpl.kind == TRAIL_SIDE_6) {
            sides[0].trail = NULL;
        }
        if (trail->head.tmpl.kind == TRAIL_SIDE_7) {
            sides[1].trail = NULL;
        }
        config->owner = NULL;
    }
    trail->head.updateKind2 = 0;
    trail->head.tmpl.flags &= ~2;
    config->flags |= 1;
}

/* gFrameDelta: frame time, gFrameTicks: frames elapsed (both used as per-update scale factors). */

/* Ages, drifts and re-grounds every point; returns 0 once a detached trail is used up. */
s32 trail_ageSurfaceTrail(SurfaceTrail *trail) {
    SurfaceTrailPoint *points;
    SurfaceTrailPoint *point;
    SurfaceTrailConfig *config;
    s32 index, expired;

    config = trail->config;
    points = trail->points;
    if (trail->head.count < 2 && (config->flags & 1)) {
        return 0;
    }
    index = trail->head.end;
    while (index != trail->head.start) {
        index--;
        expired = 0;
        if (index < 0) {
            index = trail->head.tmpl.itemCount - 1;
        }
        point = &points[index];
        point->life -= gFrameTicks;
        if (point->life < 0) {
            expired = 1;
        }
        point->fade = 255;
        if (point->fadeDelay > 0) {
            point->fadeDelay -= gFrameTicks;
        } else {
            point->alpha -= config->alphaDecay * gFrameTicks;
        }
        point->width += config->widthGrowth * gFrameDelta;
        point->x += point->vel.x * gFrameDelta;
        point->z += point->vel.z * gFrameDelta;
        {
            Vec3f probe;

            probe.x = point->x;
            probe.y = point->y + 30.0f;
            probe.z = point->z;
            if (20000.0f < fabsf(probe.x) || 20000.0f < fabsf(probe.z)) {
                expired = 1;
            } else if (collision_probeFloorSurfBCached(&probe, 0, point->y - 30.0f, &trail->collision) != 0) {
                point->y = trail->collision.height;
            } else {
                expired = 1;
            }
        }
        if (point->alpha < 0) {
            expired = 1;
        }
        if (expired) {
            /* drop this point and every older one */
            while (index != trail->head.start) {
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            points[trail->head.start].alpha = 0;
        }
    }
    config->texT += config->texTSpeed * gFrameTicks;
    if (trail->head.count > 0) {
        trail->head.modelPos.x = points[trail->head.start].x;
        trail->head.modelPos.y = points[trail->head.start].y;
        trail->head.modelPos.z = points[trail->head.start].z;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Trail update while attached: follows the owner's actor and lays new points along the path it moved.
 * If the attach point is lost, detaches the trail and leaves a spawner to start a new one.
 */
s32 trail_followSurfaceTrail(SurfaceTrail *trail) {
    SurfaceTrailConfig *config;
    SurfaceTrailPoint *points;
    Actor *actor;
    s32 pad; // matching: unused stack slot in the original frame
    s32 found;
    f32 widthStep;
    s32 i;
    s32 j;
    f32 invDist;
    SurfaceTrailLink link;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 ageStep;
    f32 inv;
    Vec3f cur;
    f32 age;
    f32 width;
    f32 widthDelta;
    TrailSide *sides;
    TrailSpawner *spawner;
    f32 stepX;
    TrailVec2 vel;
    f32 stepY;
    f32 stepZ;

    config = trail->config;
    points = trail->points;
    actor = config->owner->watcher.character;
    if (actor->type == 0) {
        return 0;
    }
    if (actor->ob_matrix_ptr == NULL) {
        return 0;
    }
    if (actor->unique_id != config->owner->watcher.characterId) {
        return 0;
    }
    found = 0;
    if (!(trail->head.tmpl.flags & 4)) {
        if (trail_getSurfaceAttachPoint(config, (u8)trail->head.tmpl.kind, &trail->head.tmpl.pos) != 0) {
            found = 1;
            config->emitPos = trail->head.tmpl.pos;
            trail->head.tmpl.flags |= 4;
        } else {
            return 1;
        }
    }
    if (!found && trail_getSurfaceAttachPoint(config, (u8)trail->head.tmpl.kind, &trail->head.tmpl.pos) == 0) {
        trail_detachSurfaceTrail(trail);
        sides = config->owner->trails.sides;
        link.owner = config->owner;
        link.side = trail->head.tmpl.kind;
        spawner = effect_newTimedEffectNoDraw(300, -1, 11, 0, 6, 8, 0xFF, 0);
        if (spawner != NULL) {
            memcpy(&spawner->link, &link, sizeof(SurfaceTrailLink));
        }
        if (trail->head.tmpl.kind == TRAIL_SIDE_6) {
            sides[0].spawner = spawner;
        } else {
            sides[1].spawner = spawner;
        }
        return 1;
    }
    config->actorX = actor->xpos;
    config->actorZ = actor->zpos;
    dx = trail->head.tmpl.pos.x - config->emitPos.x;
    dy = trail->head.tmpl.pos.y - config->emitPos.y;
    dz = trail->head.tmpl.pos.z - config->emitPos.z;
    if (0.0001f < fabsf(dx) || 0.0001f < fabsf(dy) || 0.0001f < fabsf(dz)) {
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
        config->pendingPoints += dist * config->pointsPerUnit;
        invDist = 1.0f / dist;
    }
    if (config->pendingPoints > 1.0f) {
        inv = 1.0f / config->pendingPoints;
        age = config->emitTime + gFrameDelta;
        ageStep = age * inv;
        cur = config->emitPos;
        width = config->startWidth + config->widthGrowth * age;
        widthDelta = config->startWidth - width;
        /* points drift outwards, away from the actor */
        if (trail->head.tmpl.kind == TRAIL_SIDE_7) {
            vel.x = -dz * invDist * config->driftSpeed;
            vel.z = dx * invDist * config->driftSpeed;
        } else {
            vel.x = dz * invDist * config->driftSpeed;
            vel.z = -dx * invDist * config->driftSpeed;
        }
        i = trail->head.end - 1;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        stepX = dx * inv;
        stepY = dy * inv;
        stepZ = dz * inv;
        widthStep = widthDelta * inv;
        if (i != trail->head.start && trail->head.count != 0) {
            /* blend the previous point's drift towards the new direction */
            j = i - 1;
            if (j < 0) {
                j = trail->head.tmpl.itemCount - 1;
            }
            // matching: `points + j` rather than points[j] keeps IDO's addu operand order
            (points + j)->vel.x = ((points + j)->vel.x + vel.x) * 0.5f;
            (points + j)->vel.z = ((points + j)->vel.z + vel.z) * 0.5f;
        }
        do {
            s32 ok;
            SurfaceTrailPoint *point;

            ok = 1;
            point = &points[trail->head.end];
            point->vel = vel;
            point->x = cur.x;
            point->z = cur.z;
            point->life = config->pointLife;
            point->width = width;
            point->alpha = config->pointAlpha;
            point->fadeDelay = config->pointFadeDelay;
            point->fade = 255;
            point->texS = config->texS;
            config->texS += config->texSStep + math_random() % (config->texSStepRandom + 1);
            point->x += point->vel.x * age;
            point->z += point->vel.z * age;
            {
                Vec3f probe;

                probe.x = point->x;
                if (actor->waterHeight < -9500.0f) {
                    probe.y = actor->ypos + 100.0f;
                } else {
                    probe.y = actor->waterHeight + 100.0f;
                }
                probe.z = point->z;
                if (fabsf(probe.x) > 20000.0f || fabsf(probe.z) > 20000.0f) {
                    ok = 0;
                } else if (collision_probeFloorSurfBCached(&probe, 0, probe.y - 200.0f, &trail->collision) != 0) {
                    point->y = trail->collision.height;
                } else {
                    ok = 0;
                }
            }
            if (ok) {
                trail->head.end++;
                if (trail->head.end == trail->head.tmpl.itemCount) {
                    trail->head.end = 0;
                }
                trail->head.count++;
                if (trail->head.end == trail->head.start) {
                    /* full: drop the oldest point */
                    trail->head.start++;
                    if (trail->head.start == trail->head.tmpl.itemCount) {
                        trail->head.start = 0;
                    }
                    trail->head.count--;
                }
            }
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            width += widthStep;
            age -= ageStep;
            config->pendingPoints -= 1.0f;
        } while (config->pendingPoints > 1.0f);
        config->emitPos = cur;
        config->emitTime = age;
    }
    return 1;
}


/*
 * Draws the trail as a strip three vertices wide (outer edge, centre line, outer edge), from the newest
 * point (or the live attach point) back to the oldest. Only the centre line is opaque.
 */
Gfx *trail_drawSurfaceTrail(SurfaceTrail *trail, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    SurfaceTrailConfig *config;
    SurfaceTrailPoint *points;
    Vec3f cur;
    Vec3f prev;
    s32 i;
    s32 j;
    f32 nx;
    s16 tailFadeLeft;
    s16 headFadeLeft;
    s16 prevTexS;
    s16 curTexS;
    s32 prevAlpha;
    s32 curAlpha;
    f32 dx;
    f32 dz;
    f32 prevWidth;
    s32 edgeA;
    s32 edgeB;
    s16 texT[3]; /* t at the first edge, the centre line and the second edge */
    u8 tailFade;
    u8 headFade;
    u8 update;
    f32 curWidth;
    f32 invLen;
    f32 nz;
    s32 k;

    if (trail->head.count >= 2) {
        particle_getViewBuffer(trail->vtxBuf, arg2, (trail->head.tmpl.itemCount * 3 + 18) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            config = trail->config;
            points = trail->points;
            update = 1;
            if (config->flags & 2) {
                /* fade in from the tail */
                tailFade = 0;
                tailFadeLeft = config->tailFadeCount;
                j = trail->head.start;
                do {
                    points[j].fade = tailFade;
                    j++;
                    tailFade += config->tailFadeStep;
                    if (j == trail->head.tmpl.itemCount) {
                        j = 0;
                    }
                    tailFadeLeft--;
                } while (tailFadeLeft != 0 && j != trail->head.end);
                do { } while (0); // matching: empty statement; IDO allocates registers differently without it
            }
            if (config->flags & 4) {
                /* fade out towards the head */
                headFade = 0;
                headFadeLeft = config->headFadeCount;
                j = trail->head.end - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                do {
                    points[j].fade = (points[j].fade * headFade) >> 8;
                    j--;
                    headFade += config->headFadeStep;
                    headFadeLeft--;
                    if (j < 0) {
                        j = trail->head.tmpl.itemCount - 1;
                    }
                } while (headFadeLeft != 0 && j != trail->head.end);
            }
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[57], 0, 0, 0, 0, 0x39, 0, 0, &update, 0x3E);
            gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
            gfx = math_gfxSetEnvColor(gfx, config->color[0], config->color[1], config->color[2], 0xFF, &update);
            gfx = math_gfxSetPrimColor(gfx, 0, 0, 0xFF, 0xFF, 0xFF, 0, &update);
            gfx = particle_setCombineMode(gfx, 7, &update);
            gfx = math_gfxSetOtherMode(gfx, G_TP_PERSP | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (trail->head.tmpl.flags & 2) {
                /* still attached: start the strip at the live attach point */
                i = trail->head.end - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                prev = trail->head.tmpl.pos;
                prevAlpha = (points[i].alpha * points[i].fade) >> 8;
                prevTexS = points[i].texS;
                prevWidth = config->startWidth;
            } else {
                k = trail->head.end - 1;
                if (k < 0) {
                    k = trail->head.tmpl.itemCount - 1;
                }
                i = k - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                prev.x = points[k].x;
                prev.y = points[k].y;
                prev.z = points[k].z;
                prevAlpha = (points[k].alpha * points[k].fade) >> 8;
                prevWidth = points[k].width;
                prevTexS = points[k].texS;
            }
            cur.x = points[i].x;
            cur.y = points[i].y;
            cur.z = points[i].z;
            edgeB = 2;
            curTexS = points[i].texS;
            curWidth = points[i].width;
            curAlpha = (points[i].alpha * points[i].fade) >> 8;
            dx = cur.x - prev.x;
            dz = cur.z - prev.z;
            if (0.0001f < fabsf(dx) || 0.0001f < fabsf(dz)) {
                invLen = 1.0f / sqrtf(dx * dx + dz * dz);
            } else {
                invLen = 1.0f;
            }
            texT[0] = config->texT;
            texT[2] = config->texT + config->texTSpan;
            if (texT[2] < texT[0]) {
                /* the scroll overflowed s16: wrap it */
                texT[0] = texT[0] % 1024;
                config->texT = texT[0];
                texT[2] = config->texT + config->texTSpan;
            }
            texT[1] = (texT[0] + texT[2]) >> 1;
            if (trail->head.tmpl.kind == TRAIL_SIDE_6) {
                edgeA = 0;
            } else if (trail->head.tmpl.kind == TRAIL_SIDE_7) {
                edgeA = 2;
                edgeB = 0;
            } else {
                edgeB = 0;
                edgeA = 2;
            }
            nx = -dz * invLen * prevWidth;
            nz = dx * invLen * prevWidth;
            vtx->v.ob[0] = prev.x + nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z + nz;
            vtx->v.tc[0] = prevTexS;
            vtx->v.tc[1] = texT[edgeA];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z;
            vtx->v.tc[0] = prevTexS;
            vtx->v.tc[1] = texT[1];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = prev.x - nx;
            vtx->v.ob[1] = prev.y;
            vtx->v.ob[2] = prev.z - nz;
            vtx->v.tc[0] = prevTexS;
            vtx->v.tc[1] = texT[edgeB];
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            do {
                nx = -dz * invLen * curWidth;
                nz = dx * invLen * curWidth;
                vtx->v.ob[0] = cur.x + nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z + nz;
                vtx->v.tc[0] = curTexS;
                vtx->v.tc[1] = texT[edgeA];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z;
                vtx->v.tc[0] = curTexS;
                vtx->v.tc[1] = texT[1];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = curAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = cur.x - nx;
                vtx->v.ob[1] = cur.y;
                vtx->v.ob[2] = cur.z - nz;
                vtx->v.tc[0] = curTexS;
                vtx->v.tc[1] = texT[edgeB];
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = 0;
                vtx->v.flag = 0;
                vtx++;
                /* quad strip between the previous row (0-2) and this row (3-5) */
                gSPVertex(gfx++, vtx - 6, 6, 0);
                gSP1Triangle(gfx++, 0, 3, 4, 0);
                gSP1Triangle(gfx++, 0, 4, 1, 0);
                gSP1Triangle(gfx++, 1, 4, 5, 0);
                gSP1Triangle(gfx++, 1, 5, 2, 0);
                // matching: an empty test of prevTexS; IDO allocates registers differently without it
                if (prevTexS != curTexS) {
                }
                k = i--;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                if (k != trail->head.start) {
                    prev = cur;
                    cur.x = points[i].x;
                    cur.y = points[i].y;
                    cur.z = points[i].z;
                    dx = cur.x - prev.x;
                    dz = cur.z - prev.z;
                    prevTexS = curTexS;
                    curTexS = points[i].texS;
                    curAlpha = (points[i].alpha * points[i].fade) >> 8;
                    if (dx != 0.0f || dz != 0.0f) {
                        invLen = 1.0f / sqrtf(dx * dx + dz * dz);
                    } else {
                        invLen = 1.0f;
                    }
                    curWidth = points[i].width;
                }
            } while (k != trail->head.start);
        }
    }
    return gfx;
}

/* Spawner update: once the owner's actor is back, creates the trail for its side and removes itself. */
void trail_updateSurfaceSpawner(TrailSpawner *spawner) {
    SurfaceTrail *trail;
    TrailSet *trails;
    Actor *actor;
    u8 done;
    SurfaceTrailLink *link;

    done = 0;
    link = &spawner->link;
    actor = link->owner->watcher.character;
    trails = &link->owner->trails;
    if (actor->type == 0) {
        done = 1;
    }
    if (actor->unique_id != link->owner->watcher.characterId) {
        done = 1;
    }
    if (!done && actor->ob_matrix_ptr != NULL) {
        done = 1;
        // matching: the unused extra arguments pin link and spawner to a2/a3
        trail = trail_createSurfaceTrail(link->owner, link->side, link, spawner);
        if (link->side == TRAIL_SIDE_6) {
            trails->sides[0].trail = trail;
        } else {
            trails->sides[1].trail = trail;
        }
    }
    if (done) {
        trail_unlinkSurfaceSpawner(spawner);
        spawner->base.header.state = -1;
        spawner->base.header.flags |= 1;
    }
}

/* Clears the owner's reference to this spawner. */
void trail_unlinkSurfaceSpawner(TrailSpawner *spawner) {
    TrailSide *sides = spawner->link.owner->trails.sides;

    if (spawner->link.side == TRAIL_SIDE_6) {
        sides[0].spawner = NULL;
    } else {
        sides[1].spawner = NULL;
    }
}

/* Event handler for a trail: event 10 about its owner's actor detaches the trail. */
void trail_onSurfaceTrailEvent(SurfaceTrail *trail, ActorRef *event, u8 type) {
    SurfaceTrailOwner *owner;
    SurfaceTrailConfig *config = trail->config;
    Actor *actor;

    if (type == 10) {
        owner = config->owner;
        if (owner != NULL) {
            actor = owner->watcher.character;
            if (event->actor == actor || event->uniqueId == owner->watcher.characterId) {
                trail_detachSurfaceTrail(trail);
            }
        }
    }
}

/* Event handler for a spawner: event 0 about its owner's actor deletes the spawner. */
void trail_onSurfaceSpawnerEvent(TrailSpawner *spawner, ActorRef *event, u8 type) {
    Actor *actor = spawner->link.owner->watcher.character;

    if (type == 0) {
        if (actor == event->actor || event->uniqueId == actor->unique_id) {
            trail_unlinkSurfaceSpawner(spawner);
            effect_destroyObject(&spawner->base.header);
        }
    }
}
