#include <ultra64.h>
// functions.h declares func_1513D524 as void; this file uses the effect it returns
#define func_1513D524 func_1513D524_void
#include "functions.h"
#undef func_1513D524
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

// matching: copied whole (lw/sw); two separate f32 assignments move through FPRs
typedef struct {
    f32 unk0, unk4;
} FloatPair;

/* Spawn description consumed by func_150CCEB0 (filled by func_15152874). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 drag;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ u8 color[4];
    /* 0x24 */ s32 life;
    /* 0x28 */ FloatPair unk28;
    /* 0x30 */ f32 minVy;
} DriftSpawn;

/* Spawn packet passed to func_1513D524. */
typedef struct {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 life;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ FloatPair unk14;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f dir;
    /* 0x34 */ Vec3f unk34;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 alpha;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 pad48[0x10];
} EffectSpawn; /* size 0x58 */

/* Per-effect motion state of a drifting quad (lives at Effect + 0x110). */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 drag;
    /* 0x14 */ Vec3f spin; /* added to Effect.dir every frame */
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 swayLength;
    /* 0x28 */ f32 swayLengthRate;
    /* 0x2C */ f32 swayYaw;
    /* 0x30 */ f32 phase;
    /* 0x34 */ f32 phaseSpeed;
    /* 0x38 */ f32 phaseScale;
    /* 0x3C */ f32 minVy;
} DriftMotion; /* size 0x40 */

/* Effect instance created by func_1513D524 from an EffectSpawn. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 halfWidth;
    /* 0x030 */ f32 halfHeight;
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f dir;   /* rotation angles for func_150A8050 */
    /* 0x04C */ Vec3f sway;  /* offset added to pos while falling */
    /* 0x058 */ u8 pad58[0x68];
    /* 0x0C0 */ Vtx_t template[4];
    /* 0x100 */ Vtx_t *buffers[4]; /* each holds two copies of the 4-vertex quad */
    /* 0x110 */ DriftMotion drift;
} Effect;

extern Effect *func_1513D524(EffectSpawn *spawn, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7,
                             s32 arg8);

/* Spawns a tumbling, swaying quad that drifts down from spawn->pos. */
Effect *func_150CCEB0(DriftSpawn *spawn, u8 arg1, u8 arg2) {
    Effect *effect;
    EffectSpawn packet;
    DriftMotion drift;

    packet.kind = 0x57;
    packet.flags = 0x2D03;
    packet.life = spawn->life;
    packet.unk8 = 0;
    packet.unkC = 0;
    packet.color[0] = spawn->color[0];
    packet.color[1] = spawn->color[1];
    packet.color[2] = spawn->color[2];
    packet.color[3] = 0xFF;
    packet.unk14 = spawn->unk28;
    packet.pos = spawn->pos;
    packet.dir.x = func_150ADA68() * 360.0f;
    packet.dir.y = func_150ADA68() * 360.0f;
    packet.dir.z = func_150ADA68() * 360.0f;
    packet.unk40 = 0xC03E1;
    packet.unk34.x = 0.0f;
    packet.unk34.y = 0.0f;
    packet.unk34.z = 0.0f;
    packet.alpha = spawn->color[3];
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

struct Particle150CD7F8 {
    s32 unk00;
    s32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    s16 unk32;
    s16 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
};

struct Arr5_150CD7F8 {
    f32 v[5];
};

struct Obj150CD7F8 {
    u8 pad00;
    u8 unk01;
    u8 pad02[0xA];
    u8 unk0C;
    u8 pad0D[0x1B];
    f32 unk28;
    f32 unk2C;
};

extern struct Arr5_150CD7F8 D_80088874;
extern f32 D_800BE9A4;
extern void func_1514373C(f32, f32, f32 *, f32 *);
extern void func_15152874(struct Particle150CD7F8 *, u8, u8);

void func_150CD7F8(struct Obj150CD7F8 *arg0) {
    f32 *pf = &arg0->unk28;
    struct Particle150CD7F8 p;
    struct Arr5_150CD7F8 arr;
    u8 idx;
    f32 angle;
    f32 rot;
    f32 r;
    s32 pad;

    arg0->unk28 += 0.07999999821f * D_800BE9A4 * arg0->unk2C;
    if (arg0->unk28 > 1.0f) {
        p.unk18 = -0x17;
        p.unk1A = 6;
        p.unk30 = 0;
        p.unk24 = 0.9922329783f;
        p.unk48 = -10.11759949f;
        do {
            arr = D_80088874;
            idx = func_150ADA20() % 5U;
            angle = arr.v[idx] + func_150ADA68() * 0.5999999642f;
            rot = D_800DBFF0[D_80082FA4].unk380 - 180.0f;
            while (rot > 360.0f) {
                rot -= 360.0f;
            }
            while (rot < 0.0f) {
                rot += 360.0f;
            }
            rot *= 0.01745329238f;
            if (fabsf(angle - rot) < 0.3926991224f) {
                r = func_150ADA68();
                p.unk0C = 1613.583496f * r + 2411.008545f + 500.0f;
                func_1514373C(angle, r * 2026.68000000000029 + 8400.0, &p.unk08, &p.unk10);
                p.unk00 = 0xF;
                p.unk04 = 0x14;
                p.unk14 = (angle + 3.141592741f - 0.151000008f) * 40.74366379f;
                p.unk16 = 0xC;
                p.unk32 = 0x226;
                p.unk34 = 0;
                p.unk3C = 15.0f;
                p.unk38 = 15.0f;
                p.unk44 = 25.30000114f;
                p.unk40 = 25.30000114f;
                p.unk1C = 78.59999847f;
                p.unk20 = 25.20000076f;
                p.unk28 = -1.10800004f;
                p.unk2C = 0.201000005f;
                func_15152874(&p, arg0->unk0C, arg0->unk01);
            }
            *pf -= 1.0f;
        } while (*pf > 1.0f);
    }
}

struct Inner150CDB6C {
    s32 unk0;
    f32 unk4;
};
extern u8* D_80088870;
extern f32 D_800A07A4;

void func_150CDB6C(s32 arg0) {
    struct Inner150CDB6C* q;
    if (arg0 >= 0 && arg0 < 0x100) {
        if (D_80088870 != NULL) {
            q = (struct Inner150CDB6C*)(D_80088870 + 0x28);
            q->unk4 = (f32)arg0 * D_800A07A4;
        }
    }
}
