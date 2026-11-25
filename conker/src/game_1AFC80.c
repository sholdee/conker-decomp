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
    f32 scale;    /* samples per 1/40 world unit along the chain */
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

void func_151827D0(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 v;
    s32 found;
    s32 fall;
    f32 a;
    f32 b;
    s32 sums[38];
    s32 *ps;
    s32 d0;
    s32 d1;

    for (i = 0; i < D_800DDE50; i++) {
        found = 0;
        fall = 0;
        a = D_8008D050[D_800DDE54[i]].force;
        b = D_8008D050[D_800DDE54[i]].damping;
        for (k = 1, ps = sums; k < 39; k++, ps++) {
            *ps = 0;
            for (j = 0; j < 4; j++) {
                if (D_800CC2D0[j].interaction_state != 0) {
                    v = func_15182FDC(&D_800CC2D0[j], k, i);
                    if (D_800CC2D0[j].unk84.uh == 0x4B) {
                        v *= 2;
                    }
                    *ps += v;
                    if (v != 0) {
                        found = j + 1;
                    }
                }
            }
        }
        for (n = 0; n < 3; n++) {
            ps = sums;
            for (k = 1; k < 39; k++, ps++) {
                d0 = D_800DDE60[i][k - 1].pos - D_800DDE60[i][k].pos;
                d1 = D_800DDE60[i][k + 1].pos - D_800DDE60[i][k].pos;
                D_800DDE60[i][k].accel = (f32)(d0 + d1) + a;
                D_800DDE60[i][k].accel += (f32)*ps;
                D_800DDE60[i][k].vel += D_800DDE60[i][k].accel;
                D_800DDE60[i][k].vel *= b;
                if (fabsf(D_800DDE60[i][k].vel) < 1.0f) {
                    D_800DDE60[i][k].vel = 0.0f;
                }
            }
            for (k = 1; k < 39; k++) {
                D_800DDE60[i][k].pos += D_800DDE60[i][k].vel;
                if (found != 0 && D_800DDE60[i][k].vel < -30.0f) {
                    fall = 1;
                }
            }
            if (found == 0) {
                D_8008D050[D_800DDE54[i]].soundTimer = 0;
            }
            if (fall != 0 && D_8008D050[D_800DDE54[i]].soundTimer == 0) {
                D_8008D050[D_800DDE54[i]].soundTimer = (func_150ADA20() & 0x3F) + 0xB4;
                if (D_800DDE54[i] == 2 || D_800DDE54[i] == 3) {
                    func_10010F88((func_150ADA20() & 8) + 0x507, 0x5DC0, 0, 0, 0, D_800CC2D0[found - 1].x_position, D_800CC2D0[found - 1].y_position, D_800CC2D0[found - 1].z_position, 300, 800);
                } else {
                    func_10010F88(0xF, 0x5DC0, 0, 0, 0, D_800CC2D0[found - 1].x_position, D_800CC2D0[found - 1].y_position, D_800CC2D0[found - 1].z_position, 300, 800);
                }
            } else if (fall == 0) {
                if (D_8008D050[D_800DDE54[i]].soundTimer != 0 && D_8008D050[D_800DDE54[i]].soundTimer != 0) {
                    D_8008D050[D_800DDE54[i]].soundTimer -= D_800BE9E4;
                }
            }
        }
    }
}


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[0xA];
} Vtx15182C5C;

typedef struct {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    Vtx15182C5C *unk20[2];
    Vtx15182C5C *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
} Obj15182C5C;

typedef struct {
    s16 unk0;
    u8 pad2[0x16];
} Tbl15182C5C;

extern Tbl15182C5C D_8008D060[];
extern Tbl15182C5C D_8008D062[];
f32 func_15182F58(s32 arg0, s32 arg1);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

typedef struct { u8 pad0[0x16]; u16 unk16; u8 pad18[0x88]; } Big15182C5C;



void func_15182C5C(Obj15182C5C *arg0) {
    s32 idx;
    Vtx15182C5C *out;
    s32 i;
    f32 f;
    f32 w[2];
    s32 lo;
    s32 hi;
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    f32 frac;
    s32 ofs;

    idx = arg0->unk3C;
    out = arg0->unk20[D_800BE9C0];
    func_150A8050(mtx, 0.0f, -D_8008D060[D_800DDE54[idx]].unk0, 0.0f);
    for (i = 0; i < ((Big15182C5C *)D_800DBEF4)[D_8008D062[D_800DDE54[idx]].unk0].unk16; i++) {
        f = func_15182F58(arg0->unk28[i].unk4, idx);
        lo = f;
        hi = lo + 1;
        if (hi >= 40) {
            hi = 39;
        }
        frac = f - lo;
        w[0] = D_800DDE60[idx][lo].pos * (1.0f - frac);
        w[1] = D_800DDE60[idx][hi].pos * frac;
        ofs = (s32)(w[0] + w[1]);
        ofs >>= 4;
        func_150A7960(&mtx[0][0], arg0->unk28[i].unk0, arg0->unk28[i].unk2 + ofs, arg0->unk28[i].unk4, &x, &y, &z);
        out[i].unk0 = arg0->unk10 + x;
        out[i].unk2 = arg0->unk12 + y;
        out[i].unk4 = arg0->unk14 + z;
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
    originX = D_800DBEF4[objIndex].unk10;
    originY = D_800DBEF4[objIndex].unk12;
    originZ = D_800DBEF4[objIndex].unk14;
    if (actor->y_position < D_800DDE60[chain][sample].pos * 0.0625f + originY - 300.0f ||
        D_800DDE60[chain][sample].pos * 0.0625f + originY + 300.0f < actor->y_position) {
        return 0;
    }

    dx = actor->x_position - originX;
    dy = 0.0f;
    dz = actor->z_position - originZ;
    func_150A8050(mtx, 0.0f, D_8008D050[D_800DDE54[chain]].angle, 0.0f);
    func_150A7960(&mtx[0][0], dx, dy, dz, &localX, &localY, &localZ);

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
