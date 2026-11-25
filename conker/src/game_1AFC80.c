#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One point of a 40-point spring chain; neighbours pull each other level. */
typedef struct {
    f32 accel;
    f32 vel;
    f32 pos; /* height, in 1/16 world units */
} WaveSample; /* size 0xC */

/* Static per-chain parameters (D_8008D050). */
typedef struct {
    f32 force;    /* constant added to every sample's acceleration */
    f32 damping;  /* velocity multiplier per step */
    f32 scale;    /* sample index = scale * 40 * chain-local distance */
    s16 minZ;     /* chain-local z extent (negative) */
    s16 width;    /* chain-local x extent */
    s16 angle;    /* yaw of the chain */
    s16 objIndex; /* index into D_800DBEF4 for the chain's origin */
    s16 initialPos;
    s8  push;     /* displacement an actor applies to the sample under it */
    u8  soundTimer;
} WavePreset; /* size 0x18 */

extern u8 D_800DDE54[];
extern WavePreset D_8008D050[];
extern WaveSample *D_800DDE60[];

s32 func_15182FDC(struct127 *actor, s32 sample, s32 chain);

/*
 * Steps every active wave chain: actors on it push the samples under them, each sample is pulled
 * towards its neighbours, and a fast downward swing plays a sound at the pushing actor.
 */
void func_151827D0(void) {
    s32 chain;
    s32 sample;
    s32 actor;
    s32 pass;
    s32 push;
    s32 lastActor;
    s32 falling;
    f32 force;
    f32 damping;
    s32 pushes[39];
    s32 left;
    s32 right;

    for (chain = 0; chain < D_800DDE50; chain++) {
        lastActor = 0;
        falling = 0;
        force = D_8008D050[D_800DDE54[chain]].force;
        damping = D_8008D050[D_800DDE54[chain]].damping;
        for (sample = 1; sample < 39; sample++) {
            pushes[sample] = 0;
            for (actor = 0; actor < 4; actor++) {
                if (D_800CC2D0[actor].interaction_state != 0) {
                    push = func_15182FDC(&D_800CC2D0[actor], sample, chain);
                    if (D_800CC2D0[actor].unk84.uh == 75) {
                        push *= 2;
                    }
                    pushes[sample] += push;
                    if (push != 0) {
                        lastActor = actor + 1;
                    }
                }
            }
        }
        for (pass = 0; pass < 3; pass++) {
            for (sample = 1; sample < 39; sample++) {
                left = D_800DDE60[chain][sample - 1].pos - D_800DDE60[chain][sample].pos;
                right = D_800DDE60[chain][sample + 1].pos - D_800DDE60[chain][sample].pos;
                D_800DDE60[chain][sample].accel = (f32)(left + right) + force;
                D_800DDE60[chain][sample].accel += (f32)pushes[sample];
                D_800DDE60[chain][sample].vel += D_800DDE60[chain][sample].accel;
                D_800DDE60[chain][sample].vel *= damping;
                if (fabsf(D_800DDE60[chain][sample].vel) < 1.0f) {
                    D_800DDE60[chain][sample].vel = 0.0f;
                }
            }
            for (sample = 1; sample < 39; sample++) {
                D_800DDE60[chain][sample].pos += D_800DDE60[chain][sample].vel;
                if (lastActor != 0 && D_800DDE60[chain][sample].vel < -30.0f) {
                    falling = TRUE;
                }
            }
            if (lastActor == 0) {
                D_8008D050[D_800DDE54[chain]].soundTimer = 0;
            }
            if (falling && D_8008D050[D_800DDE54[chain]].soundTimer == 0) {
                D_8008D050[D_800DDE54[chain]].soundTimer = (func_150ADA20() & 0x3F) + 180;
                if (D_800DDE54[chain] == 2 || D_800DDE54[chain] == 3) {
                    func_10010F88((func_150ADA20() & 8) + 0x507, 0x5DC0, 0, 0, 0,
                                  D_800CC2D0[lastActor - 1].x_position, D_800CC2D0[lastActor - 1].y_position,
                                  D_800CC2D0[lastActor - 1].z_position, 300, 800);
                } else {
                    func_10010F88(15, 0x5DC0, 0, 0, 0,
                                  D_800CC2D0[lastActor - 1].x_position, D_800CC2D0[lastActor - 1].y_position,
                                  D_800CC2D0[lastActor - 1].z_position, 300, 800);
                }
            } else if (!falling) {
                // matching: the duplicated test is in the original (it emits a second lbu/beqz)
                if (D_8008D050[D_800DDE54[chain]].soundTimer != 0 && D_8008D050[D_800DDE54[chain]].soundTimer != 0) {
                    D_8008D050[D_800DDE54[chain]].soundTimer -= D_800BE9E4;
                }
            }
        }
    }
}


/* A placed level prop: one 0xA0-byte entry of the D_800DBEF4 table (see game_14BC30.c). */
typedef struct Prop {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u16 vtxCount;
    /* 0x18 */ u8 pad18[0x8];
    /* 0x20 */ Vtx *vtx[2]; /* output vertices, double-buffered by D_800BE9C0 */
    /* 0x28 */ Vtx *baseVtx; /* rest-pose vertices, chain-local */
    /* 0x2C */ u8 pad2C[0x10];
    /* 0x3C */ s32 chain;
    /* 0x40 */ u8 pad40[0x60];
} Prop; /* size 0xA0 */

f32 func_15182F58(s32 dist, s32 chain);
void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/*
 * Bends a wave-chain prop's mesh: each rest vertex is lifted by the chain height interpolated
 * at its z, rotated into world space and written to this frame's vertex buffer.
 */
void func_15182C5C(Prop *prop) {
    s32 chain;
    Vtx *out;
    s32 i;
    f32 samplePos;
    f32 weighted[2]; // matching: two scalars give a different stack frame
    s32 sample;
    s32 next;
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    f32 frac;
    s32 lift;

    chain = prop->chain;
    out = prop->vtx[D_800BE9C0];
    func_150A8050(mtx, 0.0f, -D_8008D050[D_800DDE54[chain]].angle, 0.0f);
    // D_800DBEF4's WorldObject does not declare the vertex count at 0x16
    for (i = 0; i < ((Prop *)D_800DBEF4)[D_8008D050[D_800DDE54[chain]].objIndex].vtxCount; i++) {
        samplePos = func_15182F58(prop->baseVtx[i].v.ob[2], chain);
        sample = samplePos;
        next = sample + 1;
        if (next >= 40) {
            next = 39;
        }
        frac = samplePos - sample;
        weighted[0] = D_800DDE60[chain][sample].pos * (1.0f - frac);
        weighted[1] = D_800DDE60[chain][next].pos * frac;
        lift = (s32)(weighted[0] + weighted[1]);
        lift >>= 4; // matching: folding the shift into the line above reschedules it
        func_150A7960(mtx, prop->baseVtx[i].v.ob[0], prop->baseVtx[i].v.ob[1] + lift, prop->baseVtx[i].v.ob[2],
                      &x, &y, &z);
        out[i].v.ob[0] = prop->x + x;
        out[i].v.ob[1] = prop->y + y;
        out[i].v.ob[2] = prop->z + z;
    }
}

/* Maps a chain-local distance to a (fractional) sample index, clamped to [0, 39]. */
f32 func_15182F58(s32 dist, s32 chain) {
    f32 pos;

    pos = D_8008D050[D_800DDE54[chain]].scale * (f32)(dist * 40);
    if (pos < 0.0f) {
        pos = 0.0f;
    } else if (pos > 39.0f) {
        pos = 39.0f;
    }
    return pos;
}




/*
 * Returns how far `actor` pushes sample `sample` of chain `chain`: the preset's push value
 * on the sample under it, 4/7 of that on its neighbours, 0 if the actor is off the chain.
 */
s32 func_15182FDC(struct127 *actor, s32 sample, s32 chain) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 localX;
    f32 localY;
    f32 localZ;
    f32 originX;
    f32 originY;
    f32 originZ;
    f32 mtx[4][4];
    s32 objIndex;
    s32 under;
    s8 push;

    if (actor->unk28 > 3.0f) {
        return 0;
    }

    objIndex = D_8008D050[D_800DDE54[chain]].objIndex;
    originX = D_800DBEF4[objIndex].x;
    originY = D_800DBEF4[objIndex].y;
    originZ = D_800DBEF4[objIndex].z;
    if (actor->y_position < D_800DDE60[chain][sample].pos * 0.0625f + originY - 300.0f ||
        D_800DDE60[chain][sample].pos * 0.0625f + originY + 300.0f < actor->y_position) {
        return 0;
    }

    dx = actor->x_position - originX;
    dy = 0.0f;
    dz = actor->z_position - originZ;
    func_150A8050(mtx, 0.0f, D_8008D050[D_800DDE54[chain]].angle, 0.0f);
    func_150A7960(mtx, dx, dy, dz, &localX, &localY, &localZ);

    if (localZ > 0.0f || localZ < D_8008D050[D_800DDE54[chain]].minZ || localX < 0.0f ||
        D_8008D050[D_800DDE54[chain]].width < localX) {
        return 0;
    }

    under = func_15182F58(localZ, chain);
    push = D_8008D050[D_800DDE54[chain]].push;
    objIndex = under + 1; // matching: the neighbour index must reuse objIndex's register
    if (sample == under) {
        return push;
    }
    if (sample == objIndex || sample + 1 == under) {
        return push * 4 / 7;
    }
    return 0;
}
