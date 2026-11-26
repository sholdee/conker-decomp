#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct Game36680ResourceInfo {
    u8 flags;
    u8 frames;
    u16 length;
    s16 x;
    s16 y;
    u8 width;
    u8 pad9;
} Game36680ResourceInfo; // size 0xA

extern Game36680ResourceInfo D_80095CE0[];
extern u16 D_800BE478[15];
extern s16 D_800DDB80[15][2];

/** Clears the particle slot tables and picks this level's resources. */
void func_1500ABA0(s32 arg0) {
    bzero(D_800BE4A0, sizeof(D_800BE4A0));
    func_1502B8E0(D_800BE4A0, sizeof(D_800BE4A0), 3, 12, arg0, 10);
    bzero(D_800DDA90, sizeof(D_800DDA90));
    bzero(D_800DD478, sizeof(D_800DD478));
}

/** Looks up the texture file of every used particle slot and computes its texture extents. */
void func_1500AC14(void) {
    s32 i;
    s32 id;
    s32 size;
    u32 length;

    for (i = 0; i < 15; i++) {
        D_800DDA90[i].devAddr = 0;
        D_800DDA90[i].buf = 0;
        D_800DDA90[i].size = 0;
        D_800DDA90[i].used = 0;
        id = D_800BE4A0[i].id;
        if (id != 0xFF && (D_800BE478[i] != 0 || D_800BE4A0[i].id == 0x3B)) {
            D_800DDA90[i].devAddr = (s32)func_1502B020(&size, 2, 0, id);
            D_800DDA90[i].size = (size + 15) & ~15;
            length = D_80095CE0[id].length;
            if (size == 0 || length == 0) {
                length = 0;
                D_800DDA90[i].devAddr = 0;
            }
            D_800DDB80[i][0] = (D_80095CE0[id].width << 5) - 1;
            D_800DDB80[i][1] = (((length / D_80095CE0[id].width) << 6) / 5) - 1;
        }
    }
}

extern u8 D_80095CC4[];
extern s16 D_800BE438[15][2];
extern u8 D_800DDC20[][2];

/** Sets up particle slot `slot` for resource `id` (0xFF: unused). */
void func_1500AD84(s32 slot, s32 id, s32 flags) {
    Game36680ResourceInfo *info;
    u8 *frames;
    s32 infoFlags;

    if (id != 255) {
        frames = &D_800DDBD0[slot];
        info = &D_80095CE0[id];
        *frames = info->frames;
        infoFlags = info->flags;
        if (infoFlags & 0x10) {
            *frames -= 1;
            D_800DDBE0[slot] = *frames;
        } else {
            D_800DDBE0[slot] = 0;
        }
        if (infoFlags & 0x80) {
            D_800DDBF0[slot] = 2;
        } else {
            D_800DDBF0[slot] = 1;
        }
        D_800BE438[slot][0] = info->x;
        D_800BE438[slot][1] = info->y;
        D_800DDBC0[slot] = id;
        D_800DDC80[slot] = flags & 0xFF7F;
        if (flags & 0x80) {
            D_800DDC00 |= 1 << slot;
        }
        if (infoFlags & 1) {
            D_800DDC04 |= 1 << slot;
        }
        if (infoFlags & 8) {
            D_800DDC08 |= 1 << slot;
        }
    } else {
        D_800DDBD0[slot] = 0;
    }
    D_800BE478[slot] = 0;
    D_800DDC20[slot][0] = D_80095CC4[slot];
    D_800DDC20[slot][1] = D_80095CC4[slot];
}

/** One particle placed on a level triangle; D_800DDD18 holds D_800B0DF0->maxParticles of them. */
typedef struct Game36680Particle {
    s16 x;
    s16 y;
    s16 z;
    u8 unk6; // (subtype << 5) | slot
    u8 animIndex; // slot * 4 + phase into D_800DDC90, 60: not animated
    u8 resource;
    u8 unk9;
    u8 kind;
    u8 unkB;
    u8 scaleX;
    u8 scaleY;
} Game36680Particle; // size 0xE

extern Game36680Particle *D_800DDD18;

/** Fills particle `index` for particle slot `slot`; returns the next free index. */
s32 func_1500AF08(s32 index, s32 slot, s32 subtype, s16 x, s16 y, s16 z, s32 jitter, u8 kind, s32 parity) {
    s16 scale;

    if (index >= (u32)(D_800B0DF0->maxParticles - 1)) {
        return D_800B0DF0->maxParticles - 1;
    }

    D_800DDD18[index].x = x;
    D_800DDD18[index].y = y;
    D_800DDD18[index].z = z;

    if (D_800DDBD0[slot] >= 2) {
        if ((1 << slot) & D_800DDC00) {
            D_800DDD18[index].animIndex = (slot << 2) + (parity & 1);
        } else {
            D_800DDD18[index].animIndex = (index & 3) + (slot << 2);
        }
    } else {
        D_800DDD18[index].animIndex = 60;
    }

    D_800DDD18[index].unk6 = (subtype << 5) | slot;
    D_800DDD18[index].kind = kind;
    D_800DDD18[index].resource = D_800DDBC0[slot];

    if (jitter != 0) {
        // matching: the "+ jitter - jitter" pair is needed for the original instruction sequence
        scale = (func_151EF610() % jitter + jitter) - jitter + 0x100;
        D_800DDD18[index].scaleX = (D_800BE438[slot][0] * scale) >> 10;
        if (D_80095CE0[D_800DDBC0[slot]].flags & 4) {
            scale = func_151EF610() % jitter * 4 + scale;
        }
        D_800DDD18[index].scaleY = (D_800BE438[slot][1] * scale) >> 10;
    } else {
        D_800DDD18[index].scaleX = D_800BE438[slot][0] >> 2;
        D_800DDD18[index].scaleY = D_800BE438[slot][1] >> 2;
    }
    return index + 1;
}

extern f32 D_80095C64[]; // pairs of interpolation weights

/** Linear interpolation from a to b by t, with the difference computed in the integer type of a and b. */
#define LERP_INT(a, b, t) ((f32)(a) + (f32)((b) - (a)) * (t))
/** Linear interpolation from a to b by t. */
#define LERP(a, b, t) ((a) + ((b) - (a)) * (t))

/** Scatters `count` particles over the triangle `tri` at the barycentric weights of D_80095C64. */
s32 func_1500B1F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0x80, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    return index;
}

extern f32 *D_80082D80[];
extern u16 D_800BE430;

/**
 * Places the particles of pattern `pattern` (D_80082D80: a pair count, then x/z offsets) around the
 * centre of triangle `tri`, projecting each onto the triangle's plane (computed by func_15049350).
 */
s32 func_1500B3B0(s16 pattern, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    u32 i;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    f32 x;
    f32 y;
    f32 z;
    Tri3f triangle;

    centerX = 0.0f;
    centerY = 0.0f;
    centerZ = 0.0f;

    for (i = 0; i < 3; i++) {
        triangle.pts[i].x = tri[i][0];
        centerX += triangle.pts[i].x / 3.0f;
        triangle.pts[i].y = tri[i][1];
        centerY += triangle.pts[i].y / 3.0f;
        triangle.pts[i].z = tri[i][2];
        centerZ += triangle.pts[i].z / 3.0f;
    }

    func_15049350(triangle);

    for (i = 1; i <= D_80082D80[pattern][0] * 2;) {
        x = D_80082D80[pattern][i++] + centerX;
        z = D_80082D80[pattern][i++] + centerZ;
        if (D_800CC214 != 0.0f) {
            y = ((D_800CC21C - D_800CC210 * x) - D_800CC218 * z) / D_800CC214;
        } else {
            y = centerY;
        }
        index = func_1500AF08(index, slot, subtype, (s16)(s32)x, (s32)y, (s32)z, 0x80, kind, D_800BE430 >> slot);
    }

    D_800BE430 ^= 1 << slot;
    return index;
}

/** Places one particle at the centre of triangle `tri` and starts positional sound 0x4D there. */
s32 func_1500B714(s16 amount, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    u16 clamped;
    s32 x;
    s32 y;
    s32 z;

    clamped = amount;

    x = (tri[0][0] + tri[1][0] + tri[2][0]) / 3;
    y = (tri[0][1] + tri[1][1] + tri[2][1]) / 3;
    z = (tri[0][2] + tri[1][2] + tri[2][2]) / 3;

    index = func_1500AF08(index, slot, subtype, (s16)x, (s16)y, (s16)z, 0x80, kind, index);

    if ((f32)(u16)amount > 62.5f) {
        clamped = 62;
    }

    func_1000FA64(0x4D, (s16)x, (s16)y, (s16)z, 0x8000, clamped, clamped >> 3, 0, 0, 0, 0, 0);
    return index;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 87. */
void func_1500B8F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 87;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 8. */
void func_1500BAB8(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 8;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 33. */
void func_1500BC7C(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 33;
}

void func_1500BE40(s32 unused) {
    D_800DDD10[0] = NULL;
    D_800DDD10[1] = NULL;
    D_800DDD0C = 110;
}

/** Resets the particle counters and slot bitmasks. */
void func_1500BE68(void) {
    D_800DDD1C = 0;
    D_8008CEB0 = 0;
    D_800DDC04 = D_800DDC00 = D_800DDC08 = 0;
    D_800DDD20 = 0;
    func_1500BE40(0);
}

typedef s32 (*ParticleEmitFunc)(s16, u8, s32, s32, s32, s16 **);

typedef struct Game36680Emitter {
    ParticleEmitFunc emit;
    s32 unk4;
    s32 unk8;
} Game36680Emitter;

typedef struct Game36680EmitterChoice {
    s16 emitter; // index into D_8008CEB8
    s16 count;   // first argument of the emit function
} Game36680EmitterChoice;

extern Game36680EmitterChoice D_8008CF00[][8];
extern Game36680Emitter D_8008CEB8[];

/**
 * Builds the level's particles: sets up the 15 particle slots, then for every level triangle whose
 * packed word names a slot, runs that slot's emitter for the triangle's kind.
 */
void func_1500BEC0(void) {
    s32 i;
    s32 slot;
    u32 packed;
    s32 kind;

    func_1500BE68();
    if (D_800B0DF0->maxParticles == 0) {
        return;
    }
    if ((level == 6) && (D_80038080 == 0)) {
        return;
    }
    if (level == 0x1D) {
        return;
    }

    for (i = 0; i < 15; i++) {
        func_1500AD84(i, D_800BE4A0[i].id, D_800BE4A0[i].flags);
    }

    D_800DDD18 = allocate_memory(D_800B0DF0->maxParticles * sizeof(Game36680Particle), 1, 2, 0);
    func_1510F800(0);

    if (D_800DBE5C != NULL) {
        for (i = 0; i < D_800DBE4C; i++) {
            packed = D_800DBE5C[i];
            if (packed & 0xF0000) {
                slot = ((packed >> 16) & 0xF) - 1;
                kind = (packed >> 20) & 7;
                D_800DDD20 = D_8008CEB8[D_8008CF00[D_800DDC80[slot]][kind].emitter].emit(
                    D_8008CF00[D_800DDC80[slot]][kind].count, kind, slot,
                    ((packed >> 5) & 3) ^ 3, D_800DDD20, (s16 **)&D_800DBE3C[i]);
                D_800BE478[slot]++;
            }
        }
    }

    for (i = 0; i < D_800DDD20; i++) {
        D_800DDD18[i].unkB = 0;
    }

    D_8008CEB4 = 0;
    func_1517B89C(0, D_800DDD20 - 1);

    for (i = 0; i < 60; i++) {
        slot = i >> 2;
        if (D_800DDBD0[slot] != 0) {
            D_800DDC90[i] = (func_151EF610() % D_800DDBD0[slot]) << 8;
        } else {
            D_800DDC90[i] = 0;
        }
        D_800DDC10[slot] = func_151EF610();
    }
}
