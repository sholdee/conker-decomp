#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Extra data of the swinging lamp particle: a sine swing whose amplitude and speed ramp at their rates
 * while `timer` runs (started by func_151B9660), then ease back toward their targets. */
typedef struct {
    /* 0x00 */ f32 out;         // current swing angle, used as the particle's roll
    /* 0x04 */ f32 amp;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ f32 targetAmp;
    /* 0x14 */ f32 targetSpeed;
    /* 0x18 */ f32 timer;
    /* 0x1C */ f32 ampRate;
    /* 0x20 */ f32 speedRate;
} LampSwing; // size 0x24

/* Head of a pooled physics particle (full layout in game_15F680.c), carrying a LampSwing as its extra data. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ u8 pad20[0x18];
    /* 0x038 */ vertex pos;
    /* 0x044 */ u8 pad44[0x170 - 0x44];
    /* 0x170 */ LampSwing swing;
} Particle;

/* Beam particle made by func_151407D0 (same layout as Particle in game_100810.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ vertex pos;
    /* 0x40 */ vertex end;
} Beam;

/* Data of the swinging-light effect, stored at struct225.unk18 by func_151B8DB0. */
typedef struct {
    /* 0x0 */ Particle *lamp;
    /* 0x4 */ Beam *beam;      // NULL if func_151407D0 failed
    /* 0x8 */ f32 invScale;    // 1 / D_800AA528[kind]
} SwingingLight;

/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c). */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s32 zone;        // func_1510FD20 leaf zone at the spawn point
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x78 */ s32 unk78;
} ParticleSpawn; // size 0x7C

/* First argument of func_151407D0 (see game_100810.c). */
typedef struct {
    f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
    s32 unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44;
    f32 unk48;
    s16 unk4C, unk4E, unk50, unk52;
    u8 unk54, unk55, unk56, unk57, unk58;
    s8 unk59;
    u8 pad5A[6];
} ParticleParams; // size 0x60

/* Third argument of func_151407D0 (see game_100810.c). */
typedef struct {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ vertex pos;
    /* 0x28 */ vertex end;
    /* 0x34 */ f32 scale[3];
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44, unk45, unk46, unk47;
    /* 0x48 */ s32 zone;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[0xB];
} ParticleDesc; // size 0x58

extern vertex D_800AA4E0[];     // lamp position, by kind
extern f32 D_800AA528[];        // lamp size, by kind
Particle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
void func_1510F800(s32 type);
Beam *func_151407D0(ParticleParams *, s32, ParticleDesc *, u8, u8, u8, u8, s8, u8, s32);
struct225 *func_1516037C(Header *src, struct226 *arg1, s32 size, u8 arg3, s32 arg4);

/* Light callback: spawns a swinging lamp of the given kind with a light effect (D_8008B0F0 slot 7) that
 * follows it, and a beam shining from it. */
void func_151B8DB0(s32 light, u8 kind, u8 arg2, s32 arg3) {
    void *obj; // matching: one local holds the lamp, then the effect (two locals shift the stack)
    ParticleSpawn spawn;
    LampSwing swing;

    swing.out = 0.0f;
    swing.targetAmp = swing.amp = 17.31599998f;
    swing.phase = func_150ADA68() * 6.283185482f;
    swing.targetSpeed = swing.speed = 0.08799999952f;
    swing.timer = 0.0f;
    swing.ampRate = 0.0f;
    swing.speedRate = 0.0f;

    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeXZ = spawn.sizeY = D_800AA528[kind];
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = D_800AA4E0[kind];
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.rotVel.x = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = 0.0f;
    spawn.lifetime = 300;
    spawn.model = 0x22;
    func_1510F800(0);
    spawn.zone = func_1510FD20(spawn.pos.x, spawn.pos.z);
    spawn.flags = (spawn.zone ? 0x400 : 0) | 0x100 | 0x800;
    spawn.variant = 0;
    spawn.alpha = 255;
    spawn.updateFn = 9;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 4;
    spawn.unk6A = 2;
    spawn.owner = NULL;
    spawn.ownerId = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 255;
    obj = func_15132A4C(&spawn, 3, 255, sizeof(LampSwing), arg2, arg3);
    if (obj != NULL) {
        Header header;

        memcpy(&((Particle *)obj)->swing, &swing, sizeof(swing));
        {
            SwingingLight data;
            SwingingLight *state;

            data.lamp = obj;
            data.beam = NULL;
            data.invScale = 1.0f / D_800AA528[kind];
            header.unk0 = 0;
            header.unk1 = 7;
            header.unk2 = 300;
            header.unk4 = 0x12;
            obj = func_1516037C(&header, (struct226 *)light, sizeof(data), arg2, arg3);
            if (obj != NULL) {
                state = (SwingingLight *)&((struct225 *)obj)->unk18;
                memcpy(state, &data, sizeof(data));
                {
                    ParticleParams params;
                    ParticleDesc desc;

                    params.unk0 = params.unk4 = D_800AA528[kind] * 43.29999924f;
                    params.unk8 = params.unkC = D_800AA528[kind] * 30.20000076f;
                    params.unk10 = 0.200000003f;
                    params.unk14 = 0.25f;
                    params.unk18 = 0.8000000119f;
                    params.unk1C = 0.6000000238f;
                    params.unk20 = 9.999999975e-07f;
                    params.unk24 = -1;
                    params.unk34 = -1;
                    params.unk28 = -1;
                    params.unk38 = -1;
                    params.unk2C = -1;
                    params.unk3C = -1;
                    params.unk30 = -1;
                    params.unk40 = -1;
                    params.unk44 = 0;
                    params.unk48 = 1.0f;
                    params.unk4C = 0;
                    params.unk4E = 0;
                    params.unk50 = 0;
                    params.unk52 = 0;
                    params.unk54 = 0;
                    params.unk55 = 0;
                    params.unk56 = 0;
                    params.unk57 = 0;
                    params.unk58 = 0;
                    params.unk59 = -1;

                    desc.kind = 0x69;
                    desc.unk1 = 3;
                    desc.unk2 = 0x3403;
                    desc.unk4 = 300;
                    desc.unk8 = 0;
                    desc.unkC = 0;
                    desc.color[0] = 255;
                    desc.color[1] = 255;
                    desc.color[2] = 255;
                    desc.color[3] = 255;
                    desc.unk14 = 100.0f;
                    desc.unk18 = 100.0f;
                    desc.pos = spawn.pos;
                    desc.end.x = spawn.pos.x;
                    desc.end.y = spawn.pos.y - 500.0f;
                    desc.end.z = spawn.pos.z;
                    desc.scale[0] = 1.0f;
                    desc.scale[1] = 1.0f;
                    desc.scale[2] = 1.0f;
                    desc.flags = 0x1CD2006;
                    desc.unk44 = 255;
                    desc.unk45 = 255;
                    desc.unk46 = 0;
                    desc.unk47 = 6;
                    desc.zone = spawn.zone;
                    desc.unk4C = 255;
                    state->beam = func_151407D0(&params, sizeof(params), &desc, 0, 0, 0, 0, -1, arg2, arg3);
                }
            }
        }
    }
}

/* Particle update (D_800898B0 slot 9): advances the swing and its amplitude/speed ramps. */
s32 func_151B9214(Particle *particle) {
    LampSwing *swing = &particle->swing;

    swing->phase += swing->speed * D_800BE9A4;
    swing->phase = func_15144B68(swing->phase);
    swing->out = sinf(swing->phase) * swing->amp;
    if (swing->timer > 0.0f) {
        swing->timer -= D_800BE9A4;
        swing->speed += swing->speedRate * D_800BE9A4;
        swing->amp += swing->ampRate * D_800BE9A4;
    } else {
        swing->speed += (swing->targetSpeed - swing->speed) * 0.0234f;
        swing->amp += (swing->targetAmp - swing->amp) * 0.0234f;
    }
    return 1;
}

/* Matrix callback (D_8008998C slot 4): the particle rolled by its swing angle, scaled by its size and
 * placed at its position. */
s32 func_151B9310(Mtx *dest, Particle *particle) {
    f32 mtx[4][4];
    s32 pad[2]; // matching: unused stack space below the matrix

    func_150A8050(mtx, 0.0f, 0.0f, particle->swing.out);

    mtx[3][0] = particle->pos.x;
    mtx[3][1] = particle->pos.y;
    mtx[3][2] = particle->pos.z;

    mtx[0][0] *= particle->sizeXZ;
    mtx[0][1] *= particle->sizeXZ;
    mtx[0][2] *= particle->sizeXZ;

    mtx[1][0] *= particle->sizeY;
    mtx[1][1] *= particle->sizeY;
    mtx[1][2] *= particle->sizeY;

    mtx[2][0] *= particle->sizeXZ;
    mtx[2][1] *= particle->sizeXZ;
    mtx[2][2] *= particle->sizeXZ;

    guMtxF2L(mtx, dest);
    return 1;
}

void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Light effect update (D_8008B0F0 slot 7): moves the light to a point 150 units below the swinging lamp
 * and aims the beam from the lamp along the swing. */
s32 func_151B9408(struct225 *effect) {
    f32 extend; // matching: the 500.0f literal needs its own local (inline changes the frame)
    Particle *lamp;
    vertex point;
    f32 mtx[4][4];
    f32 reach;
    f32 rate;
    f32 dx, dy, dz; // unit direction from the lamp down to the light point
    SwingingLight *state;

    state = (SwingingLight *)&effect->unk18;
    lamp = state->lamp;
    func_150A8050(mtx, 0.0f, 0.0f, lamp->swing.out);
    mtx[3][0] = lamp->pos.x;
    mtx[3][1] = lamp->pos.y;
    mtx[3][2] = lamp->pos.z;
    mtx[0][0] *= lamp->sizeXZ;
    mtx[0][1] *= lamp->sizeXZ;
    mtx[0][2] *= lamp->sizeXZ;
    mtx[1][0] *= lamp->sizeY;
    mtx[1][1] *= lamp->sizeY;
    mtx[1][2] *= lamp->sizeY;
    mtx[2][0] *= lamp->sizeXZ;
    mtx[2][1] *= lamp->sizeXZ;
    mtx[2][2] *= lamp->sizeXZ;
    func_150A7960(mtx, 0.0f, -150.0f, 0.0f, &point.x, &point.y, &point.z);
    effect->unk14->unkE = point.x;
    effect->unk14->unk10 = point.y;
    effect->unk14->unk12 = point.z;
    if (state->beam != NULL) {
        extend = 500.0f;
        reach = lamp->sizeY * 65.0f;
        rate = state->invScale * (-1.0f / 150.0f);
        dx = -(point.x - lamp->pos.x) * rate;
        dy = -(point.y - lamp->pos.y) * rate;
        dz = -(point.z - lamp->pos.z) * rate;
        state->beam->pos.x = lamp->pos.x + dx * reach;
        state->beam->pos.y = lamp->pos.y + dy * reach;
        state->beam->pos.z = lamp->pos.z + dz * reach;
        state->beam->end.x = state->beam->pos.x + dx * extend;
        state->beam->end.y = state->beam->pos.y + dy * extend;
        state->beam->end.z = state->beam->pos.z + dz * extend;
    }
    return 1;
}

/* Event 0x1B handler for model 0x22 (see func_15133588): starts a 5-unit ramp of the swing. */
void func_151B9660(Particle *particle) {
    particle->swing.timer = 5.0f;
    particle->swing.ampRate = 9.256800652f;
    particle->swing.speedRate = 0.005399999209f;
}
