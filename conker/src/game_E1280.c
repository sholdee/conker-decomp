#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle trails: a ring buffer of points emitted from a moving source and drawn as a ribbon. */

#define TRAIL_STOPPED   1 /* state->flags: no more points are emitted */
#define TRAIL_FADE_TAIL 2 /* state->flags: ramp alpha up from the oldest point */
#define TRAIL_FADE_HEAD 4 /* state->flags: ramp alpha up from the newest point */

#define TRAIL_ATTACHED  2 /* trail->flags: the ribbon starts at the emitter position */

void func_150B3DD0(void) {
    u8 msg[4];

    func_15147D64(0, 5);
    msg[0] = 0;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 2;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 4;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 1;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 3;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 5;
    func_151494E0((s32)msg, 0x18);
}

void func_150B3E74(ScatterTrail *trail) {
    func_1000FC18(0x221, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, 4000);
    func_151478F4(trail);
}

void func_150B3EE8(ScatterTrail *trail) {
    func_1000FC18(0x221, trail->head.tmpl.pos.x, trail->head.tmpl.pos.y, trail->head.tmpl.pos.z, 4000);
    func_15147928(trail);
}

/* Spawns a trail at pos with the settings in params. Returns the new trail, or NULL. */
ScatterTrail *func_150B3F5C(TrailStateParams *params, Vec3f *pos, s16 capacity) {
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

    trail = func_15147A80(&spawn, sizeof(ScatterTrailState), sizeof(ScatterTrailPoint), 7, 7, 7, 0, 0, 0, 0xFF, 1);
    if (trail != NULL) {
        func_1000FA64(0x221, pos->x, pos->y, pos->z, 25000, 4000, 600, func_1000EF40, 0, 0, 8,
                      func_150ADA20() & 0x40);
        memcpy(trail->state, &state, sizeof(ScatterTrailState));
    }
    return trail;
}

/* Moves and ages the trail's points, dropping expired ones from the tail. Returns 0 once a stopped trail is empty. */
s32 func_150B40E8(ScatterTrail *trail) {
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
            points[i].life -= D_800BE9E4;
            if (points[i].life < 0) {
                expired = TRUE;
            }

            points[i].vel.y += state->gravity * D_800BE9A4;
            points[i].pos.x += points[i].vel.x * D_800BE9A4;
            points[i].pos.y += points[i].vel.y * D_800BE9A4;
            points[i].pos.z += points[i].vel.z * D_800BE9A4;
            points[i].width += state->widthGrowth * D_800BE9A4;

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
s32 func_150B4294(ScatterTrail *trail) {
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
    state->phase += (func_150ADA20() % (u32)(state->phaseJitter + 1)) * D_800BE9E4;
    state->speedTarget = func_151423D8(state->phase - 0x40) * state->speedSwing + state->speedBase;
    state->emitAccum += state->emitRate * D_800BE9A4;
    if (state->emitAccum > 1.0f) {
        dt = D_800BE9A4;
        inv = 1.0f / state->emitAccum;
        baseVel = state->baseVel;
        dtStep = D_800BE9A4 * inv;
        speed = state->speed;
        diff = state->speedTarget - state->speed;
        speedStep = diff * inv;
        do {
            state->burstAccum += state->burstRate;
            if (state->burstAccum > 1.0f) {
                func_150B5088(trail);
            }
            points[trail->head.end].pos = trail->head.tmpl.pos;
            points[trail->head.end].vel.x = baseVel.x * speed;
            points[trail->head.end].vel.y = baseVel.y * speed;
            points[trail->head.end].vel.z = baseVel.z * speed;
            points[trail->head.end].width = state->width;
            points[trail->head.end].alpha = 0xFF;
            points[trail->head.end].life = state->life;
            points[trail->head.end].texS = state->texS;
            state->texS += state->texStep + func_150ADA68() * state->texStepJitter;
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
Gfx *func_150B4710(ScatterTrail *trail, Gfx *gfx, s16 camIdx) {
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
        func_151D5D60(trail->unk84, camIdx, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
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
            gfx = func_15142E24(gfx, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &update, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_1513F4E4(gfx, 0x4D, &update);
            gfx = func_15142C10(gfx, state->shade, state->shade, state->shade, 0xFF, &update);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
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
            vtx->v.ob[0] = (s16)(cur.x + nx);
            vtx->v.ob[1] = (s16)(cur.y + ny);
            vtx->v.ob[2] = (s16)(cur.z + nz);
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = (s16)(cur.x - nx);
            vtx->v.ob[1] = (s16)(cur.y - ny);
            vtx->v.ob[2] = (s16)(cur.z - nz);
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
                vtx->v.ob[0] = (s16)(next.x + nx);
                vtx->v.ob[1] = (s16)(next.y + ny);
                vtx->v.ob[2] = (s16)(next.z + nz);
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = (s16)(next.x - nx);
                vtx->v.ob[1] = (s16)(next.y - ny);
                vtx->v.ob[2] = (s16)(next.z - nz);
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
void func_150B5060(ScatterTrail *trail) {
    ScatterTrailState *state = trail->state;

    trail->head.updateKind2 = 0;
    trail->head.tmpl.flags &= ~TRAIL_ATTACHED;
    state->flags |= TRAIL_STOPPED;
}

extern ScatterPreset D_8009FBF0[];

/* Throws out a burst of particles around the trail's emitter, one per whole unit of burstAccum. */
void func_150B5088(ScatterTrail *trail) {
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
    ScatterPreset *preset;

    state = trail->state;
    preset = &D_8009FBF0[state->burstPreset];

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
        speed = func_150ADA68() * preset->speedRange + preset->speed;
        angA = func_150ADA20() & 0xFF;
        angB = func_150ADA20() & 0xFF;
        cosA = func_151423D8(angA - 0x40);
        sinA = func_151423D8(angA);
        cosB = func_151423D8(angB - 0x40);
        sinB = func_151423D8(angB);
        x = state->baseVel.x + preset->radius * sinB * cosA;
        y = state->baseVel.y - preset->radius * cosB;
        z = state->baseVel.z + preset->radius * sinB * sinA;
        spawn.itemCount = (func_150ADA20() % (preset->itemCountRange + 1)) + preset->itemCount;
        spawn.life = (func_150ADA20() % (preset->lifeRange + 1)) + preset->life;
        particle.halfWidth = func_150ADA68() * preset->halfWidthRange + preset->halfWidth;
        particle.gravity = func_150ADA68() * preset->gravityRange + preset->gravity;
        particle.velocity.x = speed * x;
        particle.velocity.y = speed * y;
        particle.velocity.z = speed * z;
        func_15147DA0(&spawn, &particle, 0, 1, 7, 0, 0, 0, 0, 0, 0, &style, 0, trail->head.header.owner, trail->head.header.kind);
        state->burstAccum -= 1.0f;
    } while (state->burstAccum > 1.0f);
}

/* Message handler: message 5 stops the trail. */
void func_150B538C(ScatterTrail *trail, s32 arg1, u8 msg) {
    if (msg == 5) {
        func_150B5060(trail);
    }
}
