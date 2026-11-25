#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Spawn description consumed by func_150CCEB0 (filled by func_15152874). */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ vertex vel;
    /* 0x18 */ f32 drag;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ u8 color[4];
    /* 0x24 */ s32 life;
    /* 0x28 */ ParticleSize size;
    /* 0x30 */ f32 minVy;
} DriftSpawn;

/* Per-effect motion state of a drifting quad (lives at Effect + 0x110). */
typedef struct {
    /* 0x00 */ vertex vel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 drag;
    /* 0x14 */ vertex spin; /* added to Effect.dir every frame */
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 swayLength;
    /* 0x28 */ f32 swayLengthRate;
    /* 0x2C */ f32 swayYaw;
    /* 0x30 */ f32 phase;
    /* 0x34 */ f32 phaseSpeed;
    /* 0x38 */ f32 phaseScale;
    /* 0x3C */ f32 minVy;
} DriftMotion; /* size 0x40 */

/* Effect instance created by func_1513D524 from an ParticleDef. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 halfWidth;
    /* 0x030 */ f32 halfHeight;
    /* 0x034 */ vertex pos;
    /* 0x040 */ vertex dir;   /* rotation angles for func_150A8050 */
    /* 0x04C */ vertex sway;  /* offset added to pos while falling */
    /* 0x058 */ u8 pad58[0x68];
    /* 0x0C0 */ Vtx_t template[4];
    /* 0x100 */ Vtx_t *buffers[4]; /* each holds two copies of the 4-vertex quad */
    /* 0x110 */ DriftMotion drift;
} Effect;


/* Spawns a tumbling, swaying quad that drifts down from spawn->pos. */
Effect *func_150CCEB0(DriftSpawn *spawn, u8 arg1, u8 arg2) {
    Effect *effect;
    ParticleDef packet;
    DriftMotion drift;

    packet.texture = 0x57;
    packet.renderModes = 0x2D03;
    packet.lifetime = spawn->life;
    packet.frame = 0;
    packet.frameSpeed = 0;
    packet.rgba[0] = spawn->color[0];
    packet.rgba[1] = spawn->color[1];
    packet.rgba[2] = spawn->color[2];
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
    packet.unk44 = spawn->color[3];
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

    effect = func_1513D524(&packet, 0x13, 0x1E, 0, 0x11, 0, 0x40, arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->drift, &drift, sizeof(drift));
    }
    return effect;
}

void func_151436B4(f32 yaw, f32 pitch, f32 length, f32 *out);

/* Per-frame update of a drifting quad: integrates velocity with gravity and drag, sways it while falling. */
s32 func_150CD17C(Effect *effect) {
    DriftMotion *drift;
    s32 i;
    f32 wave;
    f32 pitch;
    f32 period;

    drift = &effect->drift;
    effect->pos.x += drift->vel.x * D_800BE9A4;
    effect->pos.y += drift->vel.y * D_800BE9A4 + 0.5f * drift->gravity * D_800BE9A4 * D_800BE9A4;
    effect->pos.z += drift->vel.z * D_800BE9A4;
    drift->vel.y += drift->gravity * D_800BE9A4;
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
        func_151436B4(drift->swayYaw, pitch, drift->swayLength, &effect->sway.x);
        period = 6.2831855f;
        drift->swayLength += drift->swayLengthRate * D_800BE9A4;
        drift->phase += drift->phaseSpeed * D_800BE9A4;
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

extern void func_151D5D60(Vtx_t **buffers, s16 index, s32 size, Vtx_t **vertices, u8 *fresh);
void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Draw: fills this frame's vertex buffer with the quad, rotated by dir and placed at pos (plus sway while falling). */
Vtx_t *func_150CD59C(Effect *effect, s16 bufferIndex) {
    s32 pad; // matching: unused stack slot
    Vtx_t *vertices;
    Vtx_t *result;
    f32 mtx[4][4];
    f32 corners[4][3];
    u8 i;
    u8 fresh;

    func_151D5D60(effect->buffers, bufferIndex, sizeof(effect->template), &vertices, &fresh);
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

/* Burst description passed to func_15152874, which spawns countBase..+countRange func_150CCEB0 quads. */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ vertex pos;
    /* 0x14 */ s16 yawBase; /* binary angles (256 per turn) */
    /* 0x16 */ s16 yawRange;
    /* 0x18 */ s16 pitchBase;
    /* 0x1A */ s16 pitchRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 drag;
    /* 0x28 */ f32 gravityBase;
    /* 0x2C */ f32 gravityRange;
    /* 0x30 */ s8 colorFunc; /* index into D_8008AC60, or -1 for opaque white */
    /* 0x32 */ s16 lifeBase;
    /* 0x34 */ s16 lifeRange;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 minVy;
} DriftBurst;

/* Five yaw angles (radians) around which the bursts appear. */
typedef struct {
    f32 yaw[5];
} BurstYaws;

typedef struct {
    f32 pending; /* bursts owed; one is spawned per whole unit */
    f32 rate;    /* 0..1, set by func_150CDB6C */
} BurstTimer;

/* Emitter object driven by func_150CD7F8; D_80088870 points at one. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ BurstTimer timer;
} BurstEmitter;

extern BurstYaws D_80088874;
extern void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z);
extern void func_15152874(DriftBurst *burst, u8 arg1, s32 arg2);

/*
 * Emitter update: accumulates rate over time and, for each whole unit, spawns a burst at a random distance
 * along one of the D_80088874 yaws, but only if that yaw is within pi/8 of the current camera's heading.
 */
void func_150CD7F8(BurstEmitter *emitter) {
    BurstTimer *timer = &emitter->timer;
    DriftBurst burst;
    BurstYaws yaws;
    u8 index;
    f32 yaw;
    f32 heading;
    f32 diff;
    f32 r;

    timer->pending += 0.08f * D_800BE9A4 * timer->rate;
    if (timer->pending > 1.0f) {
        burst.pitchBase = -23;
        burst.pitchRange = 6;
        burst.colorFunc = 0;
        burst.drag = 0.992233f;
        burst.minVy = -10.1175995f;
        do {
            yaws = D_80088874;
            index = func_150ADA20() % 5U;
            yaw = yaws.yaw[index] + func_150ADA68() * 0.59999996f;
            heading = D_800DBFF0[D_80082FA4].unk380 - 180.0f;
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
                burst.yawBase = (yaw + 3.1415927f - 0.15100001f) * 40.743664f;
                burst.yawRange = 12;
                burst.lifeBase = 550;
                burst.lifeRange = 0;
                burst.unk3C = 15.0f;
                burst.unk38 = 15.0f;
                burst.unk44 = 25.300001f;
                burst.unk40 = 25.300001f;
                burst.speedBase = 78.6f;
                burst.speedRange = 25.2f;
                burst.gravityBase = -1.108f;
                burst.gravityRange = 0.201f;
                func_15152874(&burst, emitter->unk0C, emitter->unk01);
            }
            timer->pending -= 1.0f;
        } while (timer->pending > 1.0f);
    }
}

extern BurstEmitter *D_80088870;

/* Sets the burst emitter's rate from a 0..255 intensity. */
void func_150CDB6C(s32 intensity) {
    BurstTimer *timer;

    if (intensity >= 0 && intensity < 256 && D_80088870 != NULL) {
        timer = &D_80088870->timer;
        timer->rate = intensity * (1.0f / 255.0f);
    }
}
