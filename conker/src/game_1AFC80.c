#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/prop.h describes it. */
u8 D_800DDE50;
u8 D_800DDE54[4];
s32 D_800DDE58; /* not referenced */
WaveSample *D_800DDE60[8];

#include "functions.h"
#include "variables.h"

/* The static parameters of each wave chain (game_362B0 starts a chain with one of them); scale is 1 / minZ. */
WavePreset D_8008D050[4] = {
    { -5.0f, 0.9200000167f, -0.001488095266f, -672, 209, 29, 0, 3136, -50, 0 },
    { -5.0f, 0.9200000167f, -0.001488095266f, -672, 209, 118, 1, 3136, -50, 0 },
    { -23.70000076f, 0.9700000286f, -0.0004222973075f, -2368, 375, 270, 0, 0, -64, 0 },
    { -23.70000076f, 0.9700000286f, -0.0004222973075f, -2368, 375, 270, 1, 0, -64, 0 },
};


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
                if (movement_obj[actor].type != 0) {
                    push = func_15182FDC(&movement_obj[actor], sample, chain);
                    if (movement_obj[actor].animseq.uh == 75) {
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
                D_800DDE60[chain][sample].accel = (left + right) + force;
                D_800DDE60[chain][sample].accel += pushes[sample];
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
                                  movement_obj[lastActor - 1].xpos, movement_obj[lastActor - 1].ypos,
                                  movement_obj[lastActor - 1].zpos, 300, 800);
                } else {
                    func_10010F88(15, 0x5DC0, 0, 0, 0,
                                  movement_obj[lastActor - 1].xpos, movement_obj[lastActor - 1].ypos,
                                  movement_obj[lastActor - 1].zpos, 300, 800);
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


/*
 * Bends a wave-chain prop's mesh: each rest vertex is lifted by the chain height interpolated
 * at its z, rotated into world space and written to this frame's vertex buffer.
 */
void func_15182C5C(WorldObject *prop) {
    s32 chain;
    Vtx *out;
    s32 i;
    f32 samplePos;
    f32 weighted[2]; // matching: two scalars give a different stack frame
    s32 sample;
    s32 next;
    MtxF mtx;
    f32 x;
    f32 y;
    f32 z;
    f32 frac;
    s32 lift;

    chain = prop->moveData.word;
    out = prop->vertices[D_800BE9C0];
    func_150A8050(mtx, 0.0f, -D_8008D050[D_800DDE54[chain]].angle, 0.0f);
    for (i = 0; i < db_Object_array[D_8008D050[D_800DDE54[chain]].objIndex].numVertices; i++) {
        samplePos = func_15182F58(prop->srcVertices[i].v.ob[2], chain);
        sample = samplePos;
        next = sample + 1;
        if (next >= 40) {
            next = 39;
        }
        frac = samplePos - sample;
        weighted[0] = D_800DDE60[chain][sample].pos * (1.0f - frac);
        weighted[1] = D_800DDE60[chain][next].pos * frac;
        lift = (weighted[0] + weighted[1]);
        lift >>= 4; // matching: folding the shift into the line above reschedules it
        func_150A7960(mtx, prop->srcVertices[i].v.ob[0], prop->srcVertices[i].v.ob[1] + lift, prop->srcVertices[i].v.ob[2],
                      &x, &y, &z);
        out[i].v.ob[0] = prop->x + x;
        out[i].v.ob[1] = prop->y + y;
        out[i].v.ob[2] = prop->z + z;
    }
}

/* Maps a chain-local distance to a (fractional) sample index, clamped to [0, 39]. */
f32 func_15182F58(s32 dist, s32 chain) {
    f32 pos;

    pos = D_8008D050[D_800DDE54[chain]].scale * (dist * 40);
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
s32 func_15182FDC(Actor *actor, s32 sample, s32 chain) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 localX;
    f32 localY;
    f32 localZ;
    f32 originX;
    f32 originY;
    f32 originZ;
    MtxF mtx;
    s32 objIndex;
    s32 under;
    s8 push;

    if (actor->shad > 3.0f) {
        return 0;
    }

    objIndex = D_8008D050[D_800DDE54[chain]].objIndex;
    originX = db_Object_array[objIndex].x;
    originY = db_Object_array[objIndex].y;
    originZ = db_Object_array[objIndex].z;
    if (actor->ypos < D_800DDE60[chain][sample].pos * 0.0625f + originY - 300.0f ||
        D_800DDE60[chain][sample].pos * 0.0625f + originY + 300.0f < actor->ypos) {
        return 0;
    }

    dx = actor->xpos - originX;
    dy = 0.0f;
    dz = actor->zpos - originZ;
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
