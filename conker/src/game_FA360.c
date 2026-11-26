#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Five yaw angles (radians) around which the bursts appear. */
typedef struct {
    f32 yaw[5];
} BurstYaws;

/* This file's .data. */
DriftBurstEmitter *gDriftBurstEmitter = NULL; /* the burst emitter whose rate particle_setDriftBurstRate sets */
static BurstYaws gDriftBurstYaws = { { 0.19999999f, 1.3f, 2.3f, 3.3999999f, 5.4f } };

/* Spawns a tumbling, swaying quad that drifts down from spawn->pos. */
DriftQuad *particle_spawnDriftQuad(DriftSpawn *spawn, u8 arg1, u8 arg2) {
    DriftQuad *effect;
    ParticleDef packet;
    QuadDriftMotion drift;

    packet.texture = 0x57;
    packet.renderModes = 0x2D03;
    packet.lifetime = spawn->life;
    packet.frame = 0;
    packet.frameSpeed = 0;
    packet.rgba[0] = spawn->color.red;
    packet.rgba[1] = spawn->color.green;
    packet.rgba[2] = spawn->color.blue;
    packet.rgba[3] = 0xFF;
    *(ParticleSize *)&packet.width = spawn->size; // matching: copied whole (lw/sw); two f32 assignments move through FPRs
    packet.pos = spawn->pos;
    packet.rot.x = func_150ADA68() * 360.0f;
    packet.rot.y = func_150ADA68() * 360.0f;
    packet.rot.z = func_150ADA68() * 360.0f;
    packet.flags = 0xC03E1;
    packet.scale.x = 0.0f;
    packet.scale.y = 0.0f;
    packet.scale.z = 0.0f;
    packet.unk44 = spawn->color.alpha;
    packet.unk45 = 0xFF;
    packet.unk46 = 0;
    packet.unk47 = 6;

    drift.vel = spawn->vel;
    drift.minVy = spawn->minVy;
    drift.drag = spawn->drag;
    drift.gravity = spawn->gravity;
    drift.spin.x = (func_150ADA68() * 20000.0f + -10000.0f) * 0.001f;
    drift.spin.y = (func_150ADA68() * 20000.0f + -10000.0f) * 0.001f;
    drift.spin.z = (func_150ADA68() * 20000.0f + -10000.0f) * 0.001f;
    drift.swayLength = 0.0f;
    drift.swayLengthRate = (func_150ADA68() * 206.0f + 58.0f) * 0.001f;
    drift.swayYaw = func_150ADA68() * 360.0f;
    drift.phase = 0.0f;
    drift.phaseSpeed = func_150ADA68() * 1210.0f * 0.0001f;
    drift.phaseScale = (func_150ADA68() * 12960.0f + 15241.0f) * 0.0001f;

    effect = particle_newQuadParticleWhite(&packet, 0x13, 0x1E, 0, 0x11, 0, 0x40, arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->drift, &drift, sizeof(drift));
    }
    return effect;
}

/* Per-frame update of a drifting quad: integrates velocity with gravity and drag, sways it while falling. */
s32 particle_updateDriftQuad(DriftQuad *effect) {
    QuadDriftMotion *drift;
    s32 i;
    f32 wave;
    f32 pitch;
    f32 period;

    drift = &effect->drift;
    effect->pos.x += drift->vel.x * gFrameDelta;
    effect->pos.y += drift->vel.y * gFrameDelta + 0.5f * drift->gravity * gFrameDelta * gFrameDelta;
    effect->pos.z += drift->vel.z * gFrameDelta;
    drift->vel.y += drift->gravity * gFrameDelta;
    for (i = D_800BE9E4; i != 0; i--) {
        drift->vel.x *= drift->drag;
        drift->vel.z *= drift->drag;
    }
    if (drift->vel.y > 0.0f) {
        for (i = D_800BE9E4; i != 0; i--) {
            drift->vel.y *= drift->drag;
        }
    }
    if (drift->vel.y < drift->minVy) {
        drift->vel.y = drift->minVy;
    }
    if (drift->vel.y < 0.0f) {
        wave = sinf(drift->phase);
        pitch = drift->phaseScale * wave + 1.5707964f;
        func_151436B4(drift->swayYaw, pitch, drift->swayLength, &effect->sway);
        period = 6.2831855f;
        drift->swayLength += drift->swayLengthRate * gFrameDelta;
        drift->phase += drift->phaseSpeed * gFrameDelta;
        while (drift->phase > period) {
            drift->phase -= period;
        }
        while (drift->phase < 0.0f) {
            drift->phase += period;
        }
    }
    effect->dir.x += drift->spin.x;
    effect->dir.y += drift->spin.y;
    effect->dir.z += drift->spin.z;
    return 1;
}

/* Draw: fills this frame's vertex buffer with the quad, rotated by dir and placed at pos (plus sway while falling). */
Vtx_t *particle_buildDriftQuad(DriftQuad *effect, s16 bufferIndex) {
    s32 pad; // matching: unused stack slot
    Vtx_t *vertices;
    Vtx_t *result;
    MtxF mtx;
    f32 corners[4][3];
    u8 i;
    u8 fresh;

    particle_getViewBuffer(effect->buffers, bufferIndex, sizeof(effect->template), &vertices, &fresh);
    result = vertices;
    // matching: an early `return NULL` reorders the blocks; the else form keeps IDO's layout
    if (vertices != NULL) {
        if (fresh) {
            memcpy(effect->buffers[bufferIndex], effect->template, sizeof(effect->template));
            memcpy(effect->buffers[bufferIndex] + 4, effect->template, sizeof(effect->template));
        }
    } else {
        return NULL;
    }
    {
        f32 *translation = mtx[3];

        corners[0][0] = effect->halfWidth;
        corners[0][1] = effect->halfHeight;
        corners[0][2] = 0.0f;
        corners[1][0] = -effect->halfWidth;
        corners[1][1] = effect->halfHeight;
        corners[1][2] = 0.0f;
        corners[2][0] = -effect->halfWidth;
        corners[2][1] = -effect->halfHeight;
        corners[2][2] = 0.0f;
        corners[3][0] = effect->halfWidth;
        corners[3][1] = -effect->halfHeight;
        corners[3][2] = 0.0f;
        func_150A8050(mtx, effect->dir.x, effect->dir.y, effect->dir.z);
        translation[0] = effect->pos.x;
        translation[1] = effect->pos.y;
        translation[2] = effect->pos.z;
        if (effect->drift.vel.y < 0.0f) {
            translation[0] += effect->sway.x;
            translation[1] += effect->sway.y;
            translation[2] += effect->sway.z;
        }
        for (i = 0; i < 4; i++) {
            f32 *corner = corners[i];

            func_150A7960(mtx, corner[0], corner[1], 0.0f, &corner[0], &corner[1], &corner[2]);
            vertices->ob[0] = corner[0];
            vertices->ob[1] = corner[1];
            vertices->ob[2] = corner[2];
            vertices->flag = 0;
            vertices++;
        }
    }
    return result;
}

/*
 * Emitter update: accumulates rate over time and, for each whole unit, spawns a burst at a random distance
 * along one of the gDriftBurstYaws yaws, but only if that yaw is within pi/8 of the current camera's heading.
 */
void particle_updateDriftBurstEmitter(DriftBurstEmitter *emitter) {
    BurstTimer *timer = &emitter->timer;
    DriftBurst burst;
    BurstYaws yaws;
    u8 index;
    f32 yaw;
    f32 heading;
    f32 diff;
    f32 r;

    timer->pending += 0.08f * gFrameDelta * timer->rate;
    if (timer->pending > 1.0f) {
        burst.angles.pitchBase = -23;
        burst.angles.pitchRange = 6;
        burst.colorFunc = 0;
        burst.drag = 0.992233f;
        burst.minVy = -10.1175995f;
        do {
            yaws = gDriftBurstYaws;
            index = func_150ADA20() % 5U;
            yaw = yaws.yaw[index] + func_150ADA68() * 0.59999996f;
            heading = camera_global_Cameras[gCurrentView].TrueCameraAngle - 180.0f;
            while (heading > 360.0f) {
                heading -= 360.0f;
            }
            while (heading < 0.0f) {
                heading += 360.0f;
            }
            heading *= 0.017453292f; /* degrees to radians */
            diff = yaw - heading;
            if (fabsf(diff) < 0.39269912f) {
                r = func_150ADA68();
                burst.pos.y = 1613.5835f * r + 2411.0085f + 500.0f;
                func_1514373C(yaw, r * (10426.68 - 8400.0) + 8400.0, &burst.pos.x, &burst.pos.z);
                burst.countBase = 15;
                burst.countRange = 20;
                /* 40.743664 = 256 / (2 * pi): radians to binary angle */
                burst.angles.yawBase = (yaw + 3.1415927f - 0.15100001f) * 40.743664f;
                burst.angles.yawRange = 12;
                burst.lifeBase = 550;
                burst.lifeRange = 0;
                burst.heightBase = 15.0f;
                burst.widthBase = 15.0f;
                burst.heightRange = 25.300001f;
                burst.widthRange = 25.300001f;
                burst.speedBase = 78.6f;
                burst.speedRange = 25.2f;
                burst.gravityBase = -1.108f;
                burst.gravityRange = 0.201f;
                particle_spawnDriftBurst(&burst, emitter->base.header.owner, emitter->base.header.kind);
            }
            timer->pending -= 1.0f;
        } while (timer->pending > 1.0f);
    }
}

/* Sets the burst emitter's rate from a 0..255 intensity. */
void particle_setDriftBurstRate(s32 intensity) {
    BurstTimer *timer;

    if (intensity >= 0 && intensity < 256 && gDriftBurstEmitter != NULL) {
        timer = &gDriftBurstEmitter->timer;
        timer->rate = intensity * (1.0f / 255.0f);
    }
}
