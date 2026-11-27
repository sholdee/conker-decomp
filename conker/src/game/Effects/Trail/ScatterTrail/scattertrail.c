#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/trail.h"

/* Particle trails: a ring buffer of points emitted from a moving source and drawn as a ribbon. */

#define TRAIL_STOPPED   1 /* state->flags: no more points are emitted */
#define TRAIL_FADE_TAIL 2 /* state->flags: ramp alpha up from the oldest point */
#define TRAIL_FADE_HEAD 4 /* state->flags: ramp alpha up from the newest point */

#define TRAIL_ATTACHED  2 /* trail->flags: the ribbon starts at the emitter position */

void func_150B3DD0(void) {
    u8 msg[4];

    effect_sendEventToEffectObjs(0, 5);
    msg[0] = 0;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
    msg[0] = 2;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
    msg[0] = 4;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
    msg[0] = 1;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
    msg[0] = 3;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
    msg[0] = 5;
    effect_sendEventToTimedEffects((s32)msg, 0x18);
}

void trail_destroyScatterTrail(ScatterTrail *trail) {
    sound_stopDynamicSoundAt(0x221, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, 4000);
    effect_defaultDestroyEffectObj(trail);
}

void trail_freeScatterTrail(ScatterTrail *trail) {
    sound_stopDynamicSoundAt(0x221, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, 4000);
    effect_defaultFreeEffectObj(trail);
}

/* Spawns a trail at pos with the settings in params. Returns the new trail, or NULL. */
ScatterTrail *trail_spawnScatterTrail(TrailStateParams *params, Vec3f *pos, s16 capacity) {
    EffectTemplate spawn;
    ScatterTrailState state;
    ScatterTrail *trail;

    memcpy(&state, params, sizeof(TrailStateParams)); /* the settings, up to state.speed */
    state.emitAccum = 0.0f;
    spawn.itemCount = capacity;
    state.texS = 0;
    spawn.flags = TRAIL_ATTACHED;
    spawn.life = 500;
    state.speedTarget = params->speedBase + params->speedSwing;
    state.phase = 0;
    state.burstAccum = 0.0f;
    state.speed = state.speedTarget;
    spawn.pos = *pos;
    spawn.kind = 9;

    trail = effect_newEffectObj(&spawn, sizeof(ScatterTrailState), sizeof(ScatterTrailPoint), 7, 7, 7, 0, 0, 0, 0xFF, 1);
    if (trail != NULL) {
        sound_addDynamicSound(0x221, pos->x, pos->y, pos->z, 25000, 4000, 600, sound_stopWhenSilentCallback, 0, 0, 8,
                      math_random() & 0x40);
        memcpy(trail->state, &state, sizeof(ScatterTrailState));
    }
    return trail;
}

/* Moves and ages the trail's points, dropping expired ones from the tail. Returns 0 once a stopped trail is empty. */
s32 trail_ageScatterTrail(ScatterTrail *trail) {
    ScatterTrailState *state;
    ScatterTrailPoint *points;
    s32 i;
    s32 expired;

    state = trail->state;
    points = trail->points;

    if ((trail->head.count < 2) && (state->flags & TRAIL_STOPPED)) {
        return 0;
    }

    i = trail->head.end;
    if (i != trail->head.start) {
        do {
            i--;
            expired = FALSE;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }

            points[i].alpha = 0xFF;
            points[i].life -= gFrameTicks;
            if (points[i].life < 0) {
                expired = TRUE;
            }

            points[i].vel.y += state->gravity * gFrameDelta;
            points[i].pos.x += points[i].vel.x * gFrameDelta;
            points[i].pos.y += points[i].vel.y * gFrameDelta;
            points[i].pos.z += points[i].vel.z * gFrameDelta;
            points[i].width += state->widthGrowth * gFrameDelta;

            if (expired && i != trail->head.start) {
                do {
                    trail->head.start++;
                    if (trail->head.start == trail->head.tmpl.itemCount) {
                        trail->head.start = 0;
                    }
                    trail->head.count--;
                } while (i != trail->head.start);
            }
        } while (i != trail->head.start);
    }

    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }

    return 1;
}

/* Emits new trail points for this frame, interpolating speed across the frame's step. */
s32 trail_emitScatterTrail(ScatterTrail *trail) {
    f32 inv;
    f32 dt;
    f32 speed;
    ScatterTrailState *state;
    ScatterTrailPoint *points;
    Vec3f baseVel;
    f32 diff;
    f32 dtStep;
    f32 speedStep;

    state = trail->state;
    points = trail->points;
    state->phase += (math_random() % (state->phaseJitter + 1)) * gFrameTicks;
    state->speedTarget = math_cosBinAngleMasked(state->phase - 0x40) * state->speedSwing + state->speedBase;
    state->emitAccum += state->emitRate * gFrameDelta;
    if (state->emitAccum > 1.0f) {
        dt = gFrameDelta;
        inv = 1.0f / state->emitAccum;
        baseVel = state->baseVel;
        dtStep = gFrameDelta * inv;
        speed = state->speed;
        diff = state->speedTarget - state->speed;
        speedStep = diff * inv;
        do {
            state->burstAccum += state->burstRate;
            if (state->burstAccum > 1.0f) {
                trail_burstScatterTrail(trail);
            }
            points[trail->head.end].pos = trail->head.tmpl.pos;
            points[trail->head.end].vel.x = baseVel.x * speed;
            points[trail->head.end].vel.y = baseVel.y * speed;
            points[trail->head.end].vel.z = baseVel.z * speed;
            points[trail->head.end].width = state->width;
            points[trail->head.end].alpha = 0xFF;
            points[trail->head.end].life = state->life;
            points[trail->head.end].texS = state->texS;
            state->texS += state->texStep + math_randomFloat() * state->texStepJitter;
            points[trail->head.end].vel.y += state->gravity * dt;
            points[trail->head.end].pos.x += points[trail->head.end].vel.x * dt;
            points[trail->head.end].pos.y += points[trail->head.end].vel.y * dt;
            points[trail->head.end].pos.z += points[trail->head.end].vel.z * dt;
            trail->head.end++;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                /* buffer full: drop the oldest point */
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            speed += speedStep;
            dt -= dtStep;
            state->emitAccum -= 1.0f;
        } while (state->emitAccum > 1.0f);
        state->speed = speed;
    }
    return 1;
}

/*
 * Draws the trail as a camera-facing ribbon, newest point first: each pair of points becomes a quad
 * whose sides are offset by the point's width along (segment x to-camera).
 */
Gfx *trail_drawScatterTrail(ScatterTrail *trail, Gfx *gfx, s16 camIdx) {
    Vtx *vtx;
    ScatterTrailState *state;
    Vec3f next;
    Vec3f cur;
    s32 j;
    s32 i;
    ScatterTrailPoint *points;
    f32 *cam;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 lenSq;
    u8 texS;
    u8 prevTexS;
    s16 alpha;
    s16 prevAlpha;
    u8 update;
    u8 fade;
    s16 count;
    s32 k;
    f32 scale;
    f32 nextScale;

    if (trail->head.count >= 2) {
        particle_getViewBuffer(trail->vtxBuf, camIdx, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            cam = &camera_global_Cameras[camIdx].eye.x;
            state = trail->state;
            points = trail->points;
            update = 1;
// matching: the label starts a new basic block, keeping the state loads below it
lbl1:
            if (state->flags & TRAIL_FADE_TAIL) {
                u8 fade1;
                s16 count1;

                fade1 = 0;
                count1 = state->fadeTailCount;
                k = trail->head.start;
                do {
                    points[k].alpha = fade1;
                    fade1 += state->fadeTailStep;
                    if (++k == trail->head.tmpl.itemCount) k = 0;
                } while (--count1 != 0 && k != trail->head.end);
            }
            if (state->flags & TRAIL_FADE_HEAD) {
                fade = 0;
                count = state->fadeHeadCount;
                k = trail->head.end - 1;
                if (k < 0) {
                    k = trail->head.tmpl.itemCount - 1;
                }
                do {
                    points[k].alpha = fade;
                    fade += state->fadeHeadStep;
                    count--;
                    k--;
                    if (k < 0) {
                        k = trail->head.tmpl.itemCount - 1;
                    }
                } while (count != 0 && k != trail->head.end);
            }
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[146], 0, 0, 0, 0, 0x92, 0, 0, &update, 3);
            gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
            gfx = particle_setCombineMode(gfx, 0x4D, &update);
            gfx = math_gfxSetEnvColor(gfx, state->shade, state->shade, state->shade, 0xFF, &update);
            gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (trail->head.tmpl.flags & TRAIL_ATTACHED) {
                j = trail->head.end - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = trail->head.tmpl.pos;
                prevTexS = points[j].texS;
                prevAlpha = (points[j].alpha * state->alpha) >> 8;
            } else {
                i = trail->head.end - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                cur = points[i].pos;
                prevTexS = points[i].texS;
                prevAlpha = (points[i].alpha * state->alpha) >> 8;
            }
            next = points[j].pos;
            texS = points[j].texS;
            alpha = (points[j].alpha * state->alpha) >> 8;
            dx = cur.x - next.x;
            dy = cur.y - next.y;
            dz = cur.z - next.z;
            cx = cur.x - cam[0];
            cy = cur.y - cam[1];
            cz = cur.z - cam[2];
            nx = dy * cz - cy * dz;
            ny = dz * cx - cz * dx;
            nz = dx * cy - cx * dy;
            lenSq = nx * nx + ny * ny + nz * nz;
            if (lenSq == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = points[j].width / sqrtf(lenSq);
                nx = nx * scale;
                ny = ny * scale;
                nz = nz * scale;
            }
            vtx->v.ob[0] = cur.x + nx;
            vtx->v.ob[1] = cur.y + ny;
            vtx->v.ob[2] = cur.z + nz;
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - nx;
            vtx->v.ob[1] = cur.y - ny;
            vtx->v.ob[2] = cur.z - nz;
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                dx = cur.x - next.x;
                dy = cur.y - next.y;
                dz = cur.z - next.z;
                cx = next.x - cam[0];
                cy = next.y - cam[1];
                cz = next.z - cam[2];
                nx = dy * cz - cy * dz;
                ny = dz * cx - cz * dx;
                nz = dx * cy - cx * dy;
                lenSq = nx * nx + ny * ny + nz * nz;
                if (lenSq == 0.0f) {
                    nx = 0.0f;
                    ny = 0.0f;
                    nz = 0.0f;
                } else {
                    nextScale = points[j].width / sqrtf(lenSq);
                    nx = nx * nextScale;
                    ny = ny * nextScale;
                    nz = nz * nextScale;
                }
                vtx->v.ob[0] = next.x + nx;
                vtx->v.ob[1] = next.y + ny;
                vtx->v.ob[2] = next.z + nz;
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - nx;
                vtx->v.ob[1] = next.y - ny;
                vtx->v.ob[2] = next.z - nz;
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prevTexS < texS) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                prevTexS = texS;
                cur = points[i].pos;
                next = points[j].pos;
                alpha = (points[j].alpha * state->alpha) >> 8;
                texS = points[j].texS;
            } while (i != trail->head.start);
        }
    }
    return gfx;
}

/* Stops the trail: no more points are emitted and the ribbon detaches from the emitter. */
void trail_stopScatterTrail(ScatterTrail *trail) {
    ScatterTrailState *state = trail->state;

    trail->head.updateKind2 = 0;
    trail->head.tmpl.flags &= ~TRAIL_ATTACHED;
    state->flags |= TRAIL_STOPPED;
}

/* The burst presets of ScatterTrail (indexed by its state's burstPreset). */
static const ScatterPreset gScatterPresets[] = {
    { 5.0f, 2.0f, 3.0f, 20.0f, 20.0f, 1.7f, 0.605f, 70, 0, 4, 3 },
};

/* Throws out a burst of particles around the trail's emitter, one per whole unit of burstAccum. */
void trail_burstScatterTrail(ScatterTrail *trail) {
    TrailRender style;
    ScatterTrailState *state;
    EffectTemplate spawn;
    TrailMotion particle;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 cosA;
    f32 sinA;
    f32 cosB;
    f32 sinB;
    u8 angA;
    u8 angB;
    const ScatterPreset *preset;

    state = trail->state;
    preset = &gScatterPresets[state->burstPreset];

    style.unk0 = 0;
    style.setGeometryMode = 1;
    style.clearGeometryMode = 0x160600;
    style.renderMode = 3;
    style.unk10 = 16;
    style.unk14 = 128;
    style.unk18 = 32;
    style.primMode = 0;
    style.envMode = 9;
    spawn.flags = 1;
    particle.texture = 6;
    particle.shade = state->shade;
    particle.flags = 8;
    particle.alpha = state->alpha;
    spawn.pos = trail->head.tmpl.pos;

    do {
        speed = math_randomFloat() * preset->speedRange + preset->speed;
        angA = math_random() & 0xFF;
        angB = math_random() & 0xFF;
        cosA = math_cosBinAngleMasked(angA - 0x40);
        sinA = math_cosBinAngleMasked(angA);
        cosB = math_cosBinAngleMasked(angB - 0x40);
        sinB = math_cosBinAngleMasked(angB);
        x = state->baseVel.x + preset->radius * sinB * cosA;
        y = state->baseVel.y - preset->radius * cosB;
        z = state->baseVel.z + preset->radius * sinB * sinA;
        spawn.itemCount = (math_random() % (preset->itemCountRange + 1)) + preset->itemCount;
        spawn.life = (math_random() % (preset->lifeRange + 1)) + preset->life;
        particle.halfWidth = math_randomFloat() * preset->halfWidthRange + preset->halfWidth;
        particle.gravity = math_randomFloat() * preset->gravityRange + preset->gravity;
        particle.velocity.x = speed * x;
        particle.velocity.y = speed * y;
        particle.velocity.z = speed * z;
        trail_createMotionTrail(&spawn, &particle, 0, 1, 7, 0, 0, 0, 0, 0, 0, &style, 0, trail->head.header.owner, trail->head.header.kind);
        state->burstAccum -= 1.0f;
    } while (state->burstAccum > 1.0f);
}

/* Message handler: message 5 stops the trail. */
void trail_onScatterTrailMessage(ScatterTrail *trail, s32 event, u8 msg) {
    if (msg == 5) {
        trail_stopScatterTrail(trail);
    }
}
