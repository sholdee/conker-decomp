#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define TRAIL_DETACHED  1 /* owner gone: die once the trail has emptied */
#define TRAIL_FADE_TAIL 2
#define TRAIL_FADE_HEAD 4

#define TRAIL_EFFECT_ATTACHED 2     /* the head follows the owner's bone */
#define TRAIL_EFFECT_STARTED  4     /* state.lastPos is valid */

/* Spawns the effects attached to an actor: a trail and two more effects. */
void trail_spawnActorBoneTrail(Actor *owner, u8 arg1) {
    BoneTrailParams params;
    Vec3f origin;

    params.owner = owner;
    params.ownerId = owner->unique_id;
    params.bone = 1;
    params.offset.x = 0.0f;
    params.offset.y = 0.0f;
    params.offset.z = 0.0f;
    params.tailFadeCount = 1;
    params.tailFadeStep = 0xFF;
    params.headFadeCount = 8;
    params.headFadeStep = 0x1F;
    params.intensity = 170;
    params.holdTime = 40;
    params.fadeRate = 7;
    params.texStep = 2;
    params.texStepRandom = 4;
    params.texScroll = 1;
    params.pointsPerUnit = 0.01800000109f;
    params.width = 30.0f;
    params.widthGrowth = 0.9810000658f;
    trail_createBoneTrailEffect(&params, arg1);
    trail_startBloodDrip(owner, arg1);
    trail_startBoneEmitter(owner, arg1);
    sound_playActorSound2(0x1AA, owner, 0x55F0, 1000, 4000);
    origin.x = 0.0f;
    origin.y = 0.0f;
    origin.z = 0.0f;
    light_createAttached(owner, 1, &origin, 2, 300, 80, 0xFF, 0xFF, 0x75, 0xFF, 0, -1, 0, 0, arg1, 1);
}

/* Creates a trail effect object from params. Returns NULL if there is no owner. */
BoneTrailEffect *trail_createBoneTrailEffect(BoneTrailParams *params, u8 arg1) {
    EffectTemplate info;
    BoneTrailState state;
    BoneTrailEffect *effect;

    if (params->owner == NULL) {
        return NULL;
    }

    memcpy(&state, params, sizeof(BoneTrailParams));
    state.texCoord = 0;
    info.itemCount = 50;
    info.flags = TRAIL_EFFECT_ATTACHED;
    info.life = 1000;
    state.flags = TRAIL_FADE_TAIL | TRAIL_FADE_HEAD;
    state.texOffset = 0;
    state.pending = 0.0f;
    state.age = 0.0f;

    if (trail_getBoneAttachPoint(&state.params, &info.pos) != 0) {
        state.lastPos = info.pos;
        info.flags |= TRAIL_EFFECT_STARTED;
    }

    info.kind = 8;
    effect = effect_newEffectObj(&info, sizeof(BoneTrailState), 24, 6, 6, 6, 0, 0, 0, arg1, 0);
    if (effect != NULL) {
        memcpy(effect->state, &state, sizeof(BoneTrailState));
    }
    return effect;
}

/*
 * Per-tick update of a trail's points: ages and fades them, drops dead ones from
 * the tail and publishes the tail position. Returns 0 to kill a detached trail.
 */
s32 trail_ageBoneTrail(BoneTrailEffect *effect) {
    BoneTrailState *state = effect->state;
    BoneTrailPoint *points = effect->points;
    BoneTrailPoint *point;
    s32 i;

    if (effect->head.count < 2 && (state->flags & TRAIL_DETACHED)) {
        return 0;
    }
    state->texOffset += state->params.texScroll * gFrameTicks;
    i = effect->head.end;
    while (i != effect->head.start) {
        i--;
        if (i < 0) {
            i = effect->head.tmpl.itemCount - 1;
        }
        point = &points[i];
        if (point->holdTime > 0) {
            point->holdTime -= gFrameTicks;
        } else {
            point->intensity -= gFrameTicks * state->params.fadeRate;
        }
        point->alpha = 0xFF;
        point->width += gFrameDelta * state->params.widthGrowth;
        if (point->intensity < 0) {
            state->flags &= ~TRAIL_FADE_TAIL;
            while (i != effect->head.start) {
                effect->head.start++;
                if (effect->head.start == effect->head.tmpl.itemCount) {
                    effect->head.start = 0;
                }
                effect->head.count--;
            }
            points[effect->head.start].intensity = 0;
        }
    }
    if (effect->head.count > 0) {
        effect->head.modelPos = points[effect->head.start].pos;
    } else {
        effect->head.modelPos.x = 0.0f;
        effect->head.modelPos.y = 0.0f;
        effect->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Per-tick update of a trail's head: follows the owner's bone and emits points
 * along the path travelled. If the bone can no longer be found, the trail is
 * detached and a TrailRetryTask is started to re-create it later.
 */
s32 trail_followBoneTrail(BoneTrailEffect *effect) {
    BoneTrailState *state = effect->state;
    BoneTrailPoint *points = effect->points;
    Actor *owner;
    Vec3f delta;
    s32 started;
    BoneTrailParams retry;

    owner = state->params.owner;
    if (owner->type == 0) {
        return 0;
    }
    if (owner->unique_id != state->params.ownerId) {
        return 0;
    }
    started = 0;
    if (!(effect->head.tmpl.flags & TRAIL_EFFECT_STARTED)) {
        if (trail_getBoneAttachPoint(&state->params, &effect->head.tmpl.pos) != 0) {
            started = 1;
            state->lastPos = effect->head.tmpl.pos;
            effect->head.tmpl.flags |= TRAIL_EFFECT_STARTED;
        } else {
            return 1;
        }
    }
    if (!started && trail_getBoneAttachPoint(&state->params, &effect->head.tmpl.pos) == 0) {
        trail_detachBoneTrail(effect);
        memcpy(&retry, state, sizeof(BoneTrailParams));
        {
            TrailRetryTask *task = effect_newTimedEffectNoDraw(300, -1, 10, 0, 5, sizeof(BoneTrailParams), effect->head.header.owner, 0);
            if (task != NULL) {
                memcpy(&task->trail, &retry, sizeof(BoneTrailParams));
            }
        }
        return 1;
    }
    delta.x = effect->head.tmpl.pos.x - state->lastPos.x;
    delta.y = effect->head.tmpl.pos.y - state->lastPos.y;
    delta.z = effect->head.tmpl.pos.z - state->lastPos.z;
    if (fabsf(delta.x) > 0.0001f || fabsf(delta.y) > 0.0001f || fabsf(delta.z) > 0.0001f) {
        state->pending += sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) * state->params.pointsPerUnit;
    }
    if (state->pending > 1.0f) {
        f32 invPending;
        f32 age;
        f32 ageStep;
        f32 width;
        Vec3f pos;
        f32 stepX;
        f32 stepY;
        f32 widthLeft;
        f32 stepZ;
        f32 widthStep;

        invPending = 1.0f / state->pending;
        age = state->age + gFrameDelta;
        pos = state->lastPos;
        ageStep = age * invPending;
        width = state->params.width + state->params.widthGrowth * age;
        stepX = delta.x * invPending;
        stepY = delta.y * invPending;
        stepZ = delta.z * invPending;
        widthLeft = state->params.width - width;
        widthStep = widthLeft * invPending;
        do {
            BoneTrailPoint *point = &points[effect->head.end];
            point->pos = pos;
            point->width = width;
            point->intensity = state->params.intensity;
            point->holdTime = state->params.holdTime;
            point->texCoord = state->texCoord;
            // matching: reusing "started" for the random value (on one line) sets the frame layout
            started = math_random(); state->texCoord += state->params.texStep + (u32)started % (state->params.texStepRandom + 1);
            point->alpha = 0xFF;
            effect->head.end++;
            if (effect->head.end == effect->head.tmpl.itemCount) {
                effect->head.end = 0;
            }
            effect->head.count++;
            if (effect->head.end == effect->head.start) {
                effect->head.start++;
                if (effect->head.start == effect->head.tmpl.itemCount) {
                    effect->head.start = 0;
                }
                effect->head.count--;
            }
            width += widthStep;
            pos.x += stepX;
            pos.y += stepY;
            pos.z += stepZ;
            age -= ageStep;
            state->pending -= 1.0f;
        } while (state->pending > 1.0f);
        state->lastPos = pos;
        state->age = age;
    }
    /*
     * matching: nine scopes holding a kept dead load each give as1 enough extra
     * basic blocks to stop it hoisting an addiu into the 'pending > 1.0f' branch
     * (found by search).
     */
    { typedef s32 T0; state = *(BoneTrailState **)((u8 *)effect + 0x40); state = state; }
    { typedef s32 T1; state = *(BoneTrailState **)((u8 *)effect + 0x44); state = state; }
    { typedef s32 T2; state = *(BoneTrailState **)((u8 *)effect + 0x48); state = state; }
    { typedef s32 T3; state = *(BoneTrailState **)((u8 *)effect + 0x4C); state = state; }
    { typedef s32 T4; state = *(BoneTrailState **)((u8 *)effect + 0x50); state = state; }
    { typedef s32 T5; state = *(BoneTrailState **)((u8 *)effect + 0x54); state = state; }
    { typedef s32 T6; state = *(BoneTrailState **)((u8 *)effect + 0x58); state = state; }
    { typedef s32 T7; state = *(BoneTrailState **)((u8 *)effect + 0x5C); state = state; }
    { typedef s32 T8; state = *(BoneTrailState **)((u8 *)effect + 0x60); state = state; }
    return 1;
}

/*
 * Draws a trail as a camera-facing ribbon: one quad per pair of neighbouring
 * points, from the head back to the tail. Needs at least two points.
 */
Gfx *trail_drawBoneTrail(BoneTrailEffect *effect, Gfx *gfx, s16 view) {
    Vtx *vtx;
    f32 camX, camY, camZ, segX, segY, segZ;
    f32 sideX, sideY, sideZ, lenSq, scale;
    u8 pad; // matching: unused local keeps the frame layout
    s32 idx;
    Vec3f next;
    Vec3f cur;
    s32 nextIdx;
    s32 curIdx;
    u8 nextTex;
    u8 curTex;
    u8 nextAlpha;
    u8 curAlpha;
    u8 stateFlag;
    f32 *camPos;
    BoneTrailPoint *points;
    BoneTrailState *state;

    if (effect->head.count >= 2) {
        particle_getViewBuffer(effect->vtxBuf, view, (effect->head.tmpl.itemCount + 5) * 2 * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            state = effect->state;
            camPos = &camera_global_Cameras[view].eye.x;
            points = effect->points;
            stateFlag = 1;
        fence:; // matching: a label here is a uopt barrier before the fade loops
            if (state->flags & TRAIL_FADE_TAIL) {
                u8 fade;
                s16 remaining;

                fade = 0;
                remaining = state->params.tailFadeCount;
                idx = effect->head.start;
                do {
                    points[idx].alpha = fade;
                    idx++;
                    fade += state->params.tailFadeStep;
                    if (idx == effect->head.tmpl.itemCount) {
                        idx = 0;
                    }
                    remaining--;
                } while (remaining != 0 && idx != effect->head.end);
            }
            if (state->flags & TRAIL_FADE_HEAD) {
                u8 fade;
                s16 remaining;

                fade = 0;
                remaining = state->params.headFadeCount;
                idx = effect->head.end - 1;
                if (idx < 0) {
                    idx = effect->head.tmpl.itemCount - 1;
                }
                do {
                    points[idx].alpha = (points[idx].alpha * fade) >> 8;
                    idx--;
                    fade += state->params.headFadeStep;
                    remaining--;
                    if (idx < 0) {
                        idx = effect->head.tmpl.itemCount - 1;
                    }
                } while (remaining != 0 && idx != effect->head.end);
            }
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[31], 0, 0, 0, 0, 0x1F, 0, 0, &stateFlag, 3);
            gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x160600);
            gfx = math_gfxSetEnvColor(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &stateFlag);
            gfx = math_gfxSetPrimColor(gfx, 0, 0, 0, 0, 0, 0, &stateFlag);
            gfx = particle_setCombineMode(gfx, 6, &stateFlag);
            gfx = math_gfxSetOtherMode(gfx, gSpriteTlutMode | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &stateFlag);
            if (effect->head.tmpl.flags & TRAIL_EFFECT_ATTACHED) {
                nextIdx = effect->head.end - 1;
                if (nextIdx < 0) {
                    nextIdx = effect->head.tmpl.itemCount - 1;
                }
                cur = effect->head.tmpl.pos;
                curTex = points[nextIdx].texCoord + state->texOffset;
                curAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
            } else {
                curIdx = effect->head.end - 1;
                if (curIdx < 0) {
                    curIdx = effect->head.tmpl.itemCount - 1;
                }
                nextIdx = curIdx - 1;
                if (nextIdx < 0) {
                    nextIdx = effect->head.tmpl.itemCount - 1;
                }
                cur = points[curIdx].pos;
                curTex = points[curIdx].texCoord + state->texOffset;
                curAlpha = (points[curIdx].intensity * points[curIdx].alpha) >> 8;
            }
            next = points[nextIdx].pos;
            nextTex = points[nextIdx].texCoord + state->texOffset;
            nextAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
            segX = cur.x - next.x;
            segY = cur.y - next.y;
            segZ = cur.z - next.z;
            camX = cur.x - camPos[0];
            camY = cur.y - camPos[1];
            camZ = cur.z - camPos[2];
            sideX = segY * camZ - camY * segZ;
            sideY = segZ * camX - camZ * segX;
            sideZ = segX * camY - camX * segY;
            lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
            if (lenSq == 0.0f) {
                sideZ = sideY = sideX = 0.0f;
            } else {
                scale = points[nextIdx].width / sqrtf(lenSq);
                sideX *= scale;
                sideY *= scale;
                sideZ *= scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = curTex << 6;
            vtx->v.tc[1] = 0x7C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[0] = curTex << 6;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                segX = cur.x - next.x;
                segY = cur.y - next.y;
                segZ = cur.z - next.z;
                camX = next.x - camPos[0];
                camY = next.y - camPos[1];
                camZ = next.z - camPos[2];
                sideX = segY * camZ - camY * segZ;
                sideY = segZ * camX - camZ * segX;
                sideZ = segX * camY - camX * segY;
                lenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
                if (lenSq == 0.0f) {
                    sideZ = sideY = sideX = 0.0f;
                } else {
                    scale = points[nextIdx].width / sqrtf(lenSq);
                    sideX *= scale;
                    sideY *= scale;
                    sideZ *= scale;
                }
                vtx->v.ob[0] = next.x + sideX;
                vtx->v.ob[1] = next.y + sideY;
                vtx->v.ob[2] = next.z + sideZ;
                vtx->v.tc[0] = nextTex << 6;
                vtx->v.tc[1] = 0x7C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = next.x - sideX;
                vtx->v.ob[1] = next.y - sideY;
                vtx->v.ob[2] = next.z - sideZ;
                vtx->v.tc[0] = nextTex << 6;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = nextAlpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (curTex < nextTex) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                curIdx = nextIdx;
                nextIdx--;
                if (nextIdx < 0) {
                    nextIdx = effect->head.tmpl.itemCount - 1;
                }
                if (curIdx != effect->head.start) {
                    curTex = nextTex;
                    cur = next;
                    next = points[nextIdx].pos;
                    nextTex = points[nextIdx].texCoord + state->texOffset;
                    nextAlpha = (points[nextIdx].intensity * points[nextIdx].alpha) >> 8;
                }
            } while (curIdx != effect->head.start);
        }
    }
    return gfx;
}

void trail_startBloodDrip(Actor *owner, u8 arg1) {
    BloodDripDesc params;

    // matching: an integer test makes IDO reload owner from its arg slot
    if ((s32)owner != 0) {
        params.target[0] = 0;
        params.target[1] = 0;
        params.ownerId = owner->unique_id;
        params.bone = 1;
        params.offset.x = 0.0f;
        params.offset.y = 0.0f;
        params.offset.z = 0.0f;
        params.radius = 14.69999981f;
        params.rate = 3.0f;
        params.lifetime = 100;
        params.flags = 10;
        params.emitter = 1;
        params.callback = -1;
        params.unk2D = 0;
        params.owner = owner;
        blood_spawnDrip(&params, 0, arg1, 0);
    }
}

/* Emits a particle burst at (x, y, z), moving against the given direction. */
void trail_emitBurst(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *parent) {
    ParticleDesc spawn;
    f32 scale;
    s16 params[6];
    SpriteParticle *particles;

    spawn.texture = 0x27;
    spawn.renderMode = 0x1401;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.timer = (math_random() % 5U) + 15;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0x8A;
    spawn.g = 0;
    spawn.b = 0;
    spawn.a = 0xFF;
    spawn.r2 = 0;
    spawn.g2 = 0;
    spawn.b2 = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.scaleStart = 1;
    spawn.scaleRate = 1.0f;

    spawn.width = spawn.height = math_randomFloat() * 50.0f + 500.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    scale = 0.0869999975f * gFrameDeltaInv;
    spawn.vel.x = -dirX * scale;
    spawn.vel.y = -dirY * scale;
    spawn.vel.z = -dirZ * scale;
    spawn.flags = 0xD;
    spawn.primColorMode = 1;
    spawn.envColorMode = 1;
    spawn.gravity = 0.0f;

    if (math_random() & 1) {
        spawn.flags |= 0x40;
    }

    if (math_random() & 1) {
        spawn.flags |= 0x80;
    }

    spawn.alpha = 0xFF;
    spawn.brightness = 0xFF;
    spawn.updateFunc = 0;
    spawn.deathFunc = -1;
    params[0] = 16;
    params[1] = 15;
    params[2] = 13;
    params[3] = 19;
    params[4] = 17;
    params[5] = -24;

    particles = particle_newOwnedSpriteParticle(&spawn, 0, 12, parent->owner, 1);
    if (particles != NULL) {
        memcpy(&particles->data.ageFade, params, sizeof(params));
    }
}

/* Particle update for trail_emitBurst's bursts: fades and resizes by age. */
s32 trail_updateBurstParticle(SpriteParticle *particle, u8 arg1) {
    s16 age;
    AgeFadeParams *params;

    params = &particle->data.ageFade;
    age = particle->head.desc.timer;
    if (age < params->fadeStart) {
        particle->head.desc.alpha = age * params->fadeStep;
    }
    if (age < params->growStart) {
        {
            f32 sizeStep = params->growRate * gFrameTicks;

            particle->head.desc.width += sizeStep;
            particle->head.desc.height += sizeStep;
        }
        age = particle->head.desc.timer;
    }
    if (age < params->tintStart) {
        particle->head.desc.updateFunc = 1;
        particle->head.desc.primColorMode = 2;
        particle->head.desc.envColorMode = 2;
        particle->head.desc.brightness = age * params->tintStep;
        particle->head.desc.renderMode = 0x5203;
    }
    return 1;
}

/* Like trail_updateBurstParticle, but always sets unk2C from the age. */
s32 func_151A4900(SpriteParticle *particle, u8 arg1) {
    s16 age;
    AgeFadeParams *params;

    params = &particle->data.ageFade;
    age = particle->head.desc.timer;
    if (age < params->fadeStart) {
        particle->head.desc.alpha = age * params->fadeStep;
    }
    if (age < params->growStart) {
        {
            f32 sizeStep = params->growRate * gFrameTicks;

            particle->head.desc.width += sizeStep;
            particle->head.desc.height += sizeStep;
        }
        age = particle->head.desc.timer;
    }
    particle->head.desc.brightness = age * params->tintStep;
    return 1;
}

/* Starts a BoneEmitterTask on the owner's bone 1. */
void trail_startBoneEmitter(Actor *owner, u8 arg1) {
    BoneEmitterTask *task;
    BoneEmitter emitter;

    emitter.owner = owner;
    emitter.ownerId = owner->unique_id;
    emitter.pending = 0.0f;
    emitter.bone = 1;
    emitter.offset.x = 0.0f;
    emitter.offset.y = 0.0f;
    emitter.offset.z = 0.0f;
    emitter.rate = 0.7220000029f;
    task = effect_newTimedEffectNoDraw(300, -1, 5, 0, 1, sizeof(BoneEmitter), arg1, 0);
    if (task != NULL) {
        memcpy(&task->emitter, &emitter, sizeof(BoneEmitter));
    }
}

/*
 * Per-tick update of a BoneEmitterTask: ends when the owner is gone, otherwise
 * spawns the particles that have accumulated at the bone's current position.
 */
void trail_updateBoneEmitter(BoneEmitterTask *task) {
    BoneEmitter *emitter;
    Vec3f pos;
    SpriteBurst spawn;
    s16 angle;

    emitter = &task->emitter;
    if ((emitter->owner->type == 0) || (emitter->owner->type == 8)
        || (emitter->owner->unique_id != emitter->ownerId)) {
        task->base.header.state = -1;
        task->base.header.flags |= 1;
        return;
    }

    if (emitter->owner->ob_matrix_ptr == NULL) {
        return;
    }
    if ((emitter->owner->cullMask & 0xF) == 0xF) {
        return;
    }

    emitter->pending += emitter->rate * gFrameDelta;
    if (emitter->pending > 1.0f) {
        angle = (emitter->owner->angle >> 8) - 0x40;
        math_transformOffset(&emitter->offset, &pos, &emitter->owner->ob_matrix_ptr[emitter->bone]);

        spawn.countBase = emitter->pending;
        spawn.countRange = 0;
        emitter->pending -= spawn.countBase;
        spawn.texture = 0x28;
        spawn.renderMode = 0xC01;
        spawn.unk8 = 0x200005;
        spawn.unkC = 0;
        spawn.timerBase = 23;
        spawn.timerRange = 13;
        spawn.frameRateBase = 0;
        spawn.frameRateRange = 0;
        spawn.r = 0;
        spawn.g = 0;
        spawn.b = 0;
        spawn.a = 0xFF;
        spawn.r2 = 0;
        spawn.g2 = 0;
        spawn.b2 = 0;
        spawn.sizeBase = 300.0f;
        spawn.sizeRange = 400.0f;
        spawn.pos = pos;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0;
        spawn.scaleStart = 1;
        spawn.scaleRate = 1.0f;
        spawn.unk64 = 0;
        spawn.yawBase = angle - 25;
        spawn.pitchBase = -44;
        spawn.yawRange = 50;
        spawn.pitchRange = 50;
        spawn.speedBase = 0.0f;
        spawn.speedRange = 15.0f;
        spawn.gravityBase = -0.5f;
        spawn.gravityRange = -0.5f;
        spawn.flags = 7;
        spawn.primColorMode = 1;
        spawn.envColorMode = 0;

        if (math_random() & 1) {
            spawn.flags |= 0x40;
        }
        if (math_random() & 1) {
            spawn.flags |= 0x80;
        }

        spawn.alphaBase = 0xFF;
        spawn.alphaRange = 0;
        spawn.brightness = 0xFF;
        spawn.updateFunc = -1;
        spawn.deathFunc = -1;
        particle_spawnSpriteBurst(&spawn, 0xFF, task->base.header.owner, task->base.header.kind);
    }
}

/* Message handler of a trail: detach on owner removal, follow an owner swap. */
void trail_onBoneTrailMessage(BoneTrailEffect *effect, ActorMsg *msg, u8 type) {
    BoneTrailParams *trail = &effect->state->params;

    if (type == 0) {
        if ((msg->ref.actor == trail->owner) || (msg->ref.uniqueId == trail->ownerId)) {
            trail_detachBoneTrail(effect);
        }
    } else if (type == 0x2D) {
        if (trail->owner == msg->swap.actorA) {
            trail->owner = msg->swap.actorB;
            trail->ownerId = msg->swap.idB;
        } else if (trail->owner == msg->swap.actorB) {
            trail->owner = msg->swap.actorA;
            trail->ownerId = msg->swap.idA;
        }
    }
}

/* Message handler of a BoneEmitterTask: end on owner removal, follow an owner swap. */
void trail_onBoneEmitterMessage(BoneEmitterTask *task, ActorMsg *msg, u8 type) {
    BoneEmitter *emitter = &task->emitter;

    if (type == 0) {
        if ((msg->ref.actor == emitter->owner) || (msg->ref.uniqueId == emitter->ownerId)) {
            effect_destroyObject(&task->base.header);
        }
    } else if (type == 0x2D) {
        if (emitter->owner == msg->swap.actorA) {
            emitter->owner = msg->swap.actorB;
            emitter->ownerId = msg->swap.idB;
        } else if (emitter->owner == msg->swap.actorB) {
            emitter->owner = msg->swap.actorA;
            emitter->ownerId = msg->swap.idA;
        }
    }
}

/*
 * Transforms the trail's attachment offset by its owner's bone matrix into out.
 * Returns 0 if the owner has no bone matrices or is not being drawn.
 */
s32 trail_getBoneAttachPoint(BoneTrailParams *params, Vec3f *out) {
    Actor *owner = params->owner;
    MtxF *bones = owner->ob_matrix_ptr;

    if (bones == NULL) {
        return 0;
    }
    if ((owner->cullMask & 0xF) == 0xF) {
        return 0;
    }
    // matching: a byte offset; &bones[params->bone] scales the index differently
    math_transformOffset(&params->offset, out, (u8 *)bones + params->bone * sizeof(Mtx));
    return 1;
}

/* Detaches a trail from its owner: the head stops following and fades out. */
void trail_detachBoneTrail(BoneTrailEffect *effect) {
    BoneTrailState *state = effect->state;

    effect->head.updateKind2 = 0;
    effect->head.tmpl.flags &= ~TRAIL_EFFECT_ATTACHED;
    state->flags |= TRAIL_DETACHED;
    state->flags |= TRAIL_FADE_HEAD;
}

/*
 * Per-tick update of a TrailRetryTask: the task ends when the owner is gone or
 * once the trail has been re-created on the owner's bone.
 */
void trail_updateRetryTask(TrailRetryTask *task) {
    BoneTrailParams *trail;
    u8 done;

    done = 0;
    trail = &task->trail;
    if (trail->owner->type == 0) {
        done = 1;
    }
    if (trail->owner->unique_id != trail->ownerId) {
        done = 1;
    }
    if (!done && trail->owner->ob_matrix_ptr != NULL && (trail->owner->cullMask & 0xF) != 0xF) {
        trail_createBoneTrailEffect(trail, task->base.header.owner);
        done = 1;
    }
    if (done) {
        task->base.header.state = -1;
        task->base.header.flags |= 1;
    }
}

/* Message handler of a TrailRetryTask: end when the owner is removed. */
void trail_onRetryTaskMessage(TrailRetryTask *task, ActorMsg *msg, u8 type) {
    BoneTrailParams *trail = &task->trail;

    if (type == 0) {
        if (msg->ref.actor == trail->owner || msg->ref.uniqueId == trail->ownerId) {
            effect_destroyObject(&task->base.header);
        }
    }
}
