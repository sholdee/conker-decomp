#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/effect.h"

/* Maps a model id to the hit-sphere table (gHitSphereTables / gHitSphereCounts index) it uses. */
typedef struct {
    /* 0x0 */ u16 model;
    /* 0x2 */ u8  sphereTable;
    /* 0x3 */ u8  pad3;
} ModelHitEntry; /* size 0x4 */

/* The models that have hit spheres. */
static const ModelHitEntry gModelHitTables[23] = {
    { 0x53, 0, 0 }, { 0x00, 1, 0 }, { 0x01, 1, 0 }, { 0x02, 1, 0 },
    { 0x03, 1, 0 }, { 0x04, 1, 0 }, { 0x3B, 2, 0 }, { 0x5A, 3, 0 },
    { 0x74, 3, 0 }, { 0x7A, 3, 0 }, { 0x75, 4, 0 }, { 0x80, 5, 0 },
    { 0x88, 6, 0 }, { 0x90, 7, 0 }, { 0x96, 8, 0 }, { 0x9F, 10, 0 },
    { 0xA0, 10, 0 }, { 0x9C, 11, 0 }, { 0x9D, 11, 0 }, { 0xB2, 11, 0 },
    { 0xB4, 11, 0 }, { 0xB0, 11, 0 }, { 0xB1, 11, 0 },
};

/* One hit sphere bound to a bone of a model. */
typedef struct {
    /* 0x0 */ u8  bone;
    /* 0x1 */ u8  part;
    /* 0x2 */ u16 radius;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 z;
} HitSphere; /* size 0xA */

/* The hit spheres of each table: bone, part, radius, x, y, z. */
static const HitSphere D_8009BD8C[7] = {
    { 1, 0, 63, 0, 0, 0 },
    { 2, 0, 65, 0, 0, 0 },
    { 3, 0, 35, 0, 0, 0 },
    { 4, 0, 25, 0, 0, 0 },
    { 5, 0, 58, 0, 42, 115 },
    { 6, 0, 44, 0, -34, 67 },
    { 7, 0, 69, 0, -37, 0 },
};
static const HitSphere D_8009BDD4[4] = {
    { 14, 10, 50, 0, 12, -1 },
    { 2, 0, 31, 0, 0, 0 },
    { 13, 0, 19, 0, 0, 0 },
    { 1, 0, 29, 0, 0, 0 },
};
static const HitSphere D_8009BDFC[42] = {
    { 12, 10, 38, 0, 33, 11 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 0, 19, 0, 0, 4 },
    { 0, 0, 24, 0, 12, 6 },
    { 1, 0, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 19, 0, 20, 3, -3, 11 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 23, 0, 20, -4, -3, 7 },
    { 12, 10, 22, 20, 21, 9 },
    { 12, 10, 22, -22, 21, 6 },
    { 25, 0, 27, 0, 0, 0 },
    { 26, 0, 43, 0, 0, 0 },
    { 27, 0, 34, 0, 0, 0 },
    { 27, 0, 41, 0, 0, 32 },
    { 25, 0, 34, 0, 6, -24 },
};
static const HitSphere D_8009BFA0[37] = {
    { 12, 10, 38, 0, 33, 11 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 0, 19, 0, 0, 4 },
    { 0, 0, 24, 0, 12, 6 },
    { 1, 0, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 19, 0, 20, 3, -3, 11 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 23, 0, 20, -4, -3, 7 },
    { 12, 10, 22, 20, 21, 9 },
    { 12, 10, 22, -22, 21, 6 },
};
static const HitSphere D_8009C114[37] = {
    { 12, 10, 38, 0, 33, 11 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 0, 19, 0, 0, 4 },
    { 0, 0, 24, 0, 12, 6 },
    { 1, 0, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 19, 0, 20, 3, -3, 11 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 23, 0, 20, -4, -3, 7 },
    { 12, 10, 22, 20, 21, 9 },
    { 12, 10, 22, -22, 21, 6 },
};
static const HitSphere D_8009C288[35] = {
    { 12, 10, 38, 0, 33, 5 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 0, 21, 0, 0, 4 },
    { 0, 0, 24, 0, 12, 6 },
    { 1, 0, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 12, 10, 22, 20, 21, 9 },
    { 12, 10, 22, -22, 21, 6 },
};
static const HitSphere D_8009C3E8[43] = {
    { 12, 10, 25, 0, 19, 17 },
    { 12, 10, 17, 0, 15, 38 },
    { 12, 0, 20, 0, 8, 56 },
    { 12, 0, 17, 0, 6, 74 },
    { 12, 0, 10, 0, 12, 89 },
    { 12, 0, 9, 0, 14, 101 },
    { 12, 0, 9, 0, 22, 117 },
    { 12, 0, 13, 0, -9, 0 },
    { 0, 0, 19, 0, 0, 4 },
    { 0, 0, 17, 0, 12, 6 },
    { 1, 0, 16, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 19, 0, 27, 5, -10, 19 },
    { 19, 0, 17, 0, -12, 42 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 23, 0, 25, -10, -9, 22 },
    { 23, 0, 14, -7, -9, 43 },
};
static const HitSphere D_8009C598[21] = {
    { 1, 0, 31, 0, 12, 8 },
    { 1, 0, 24, 0, 26, 8 },
    { 3, 10, 33, 0, 24, 2 },
    { 3, 10, 17, 0, 16, 26 },
    { 3, 10, 17, 25, 11, 0 },
    { 3, 10, 17, -23, 14, 0 },
    { 13, 0, 12, 0, 0, 0 },
    { 14, 0, 12, 14, -9, 0 },
    { 14, 0, 14, 0, -6, 0 },
    { 15, 0, 25, 0, -9, 0 },
    { 18, 0, 9, -15, 1, 0 },
    { 18, 0, 9, 0, 0, 0 },
    { 19, 0, 11, -9, -7, -3 },
    { 19, 0, 10, 0, 0, 0 },
    { 20, 0, 22, 6, -7, -1 },
    { 22, 0, 15, 0, 0, 0 },
    { 23, 0, 16, 0, 0, 0 },
    { 24, 0, 19, 0, 8, 0 },
    { 25, 0, 16, 0, 0, 4 },
    { 26, 0, 15, 0, 0, 0 },
    { 27, 0, 19, -6, 4, 3 },
};
static const HitSphere D_8009C66C[42] = {
    { 12, 10, 38, 0, 33, 11 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 0, 19, 0, 0, 4 },
    { 0, 0, 24, 0, 12, 6 },
    { 1, 0, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 0, 6, 16, -22, -2 },
    { 2, 0, 7, 10, -15, -1 },
    { 2, 0, 6, 4, -9, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 8, 5, -13, 0 },
    { 3, 0, 8, 9, -22, 0 },
    { 4, 0, 12, 0, -11, 0 },
    { 4, 0, 11, 6, -22, 0 },
    { 7, 0, 7, -6, -13, -2 },
    { 7, 0, 6, -12, -18, -3 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 7, -9, -10, 0 },
    { 9, 0, 9, 3, 1, 0 },
    { 9, 0, 11, 0, -12, 2 },
    { 9, 0, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 0, 14, -2, -9, -1 },
    { 17, 0, 10, 0, 0, 0 },
    { 17, 0, 10, 3, -13, 0 },
    { 18, 0, 11, 0, -11, 0 },
    { 19, 0, 19, 0, -8, 0 },
    { 19, 0, 20, 3, -3, 11 },
    { 20, 0, 12, 0, -10, 0 },
    { 20, 0, 10, 0, -20, 0 },
    { 21, 0, 9, -5, -14, 0 },
    { 22, 0, 13, -4, -16, 4 },
    { 22, 0, 20, -5, -21, 20 },
    { 23, 0, 20, -4, -3, 7 },
    { 12, 10, 22, 20, 21, 9 },
    { 12, 10, 22, -22, 21, 6 },
    { 25, 0, 27, 0, 0, 0 },
    { 26, 0, 43, 0, 0, 0 },
    { 27, 0, 34, 0, 0, 0 },
    { 27, 0, 41, 0, 0, 32 },
    { 25, 0, 34, 0, 6, -24 },
};
static const HitSphere D_8009C810[31] = {
    { 0, 0, 39, 0, 0, 0 },
    { 1, 0, 29, 0, 0, 3 },
    { 12, 10, 30, 0, 30, 7 },
    { 12, 10, 27, 0, 39, 8 },
    { 12, 10, 32, 0, 12, 23 },
    { 2, 0, 10, 0, -12, 0 },
    { 2, 0, 9, 9, -27, -2 },
    { 3, 0, 9, 0, 0, 0 },
    { 3, 0, 9, 0, -11, 0 },
    { 3, 0, 11, 4, -18, 0 },
    { 4, 0, 12, 0, 0, 0 },
    { 4, 0, 17, 0, -16, 0 },
    { 5, 0, 17, -4, 0, -3 },
    { 7, 0, 9, 0, 0, 0 },
    { 7, 0, 8, 0, -11, 0 },
    { 7, 0, 8, 0, -22, 0 },
    { 8, 0, 9, 0, 0, 0 },
    { 8, 0, 10, 0, -14, 0 },
    { 9, 0, 14, 0, 0, 0 },
    { 9, 0, 16, 0, -16, 0 },
    { 10, 0, 16, 3, 0, -2 },
    { 17, 0, 13, 0, 0, 0 },
    { 18, 0, 13, 0, 0, 0 },
    { 18, 0, 17, 0, -1, 21 },
    { 19, 0, 24, 0, 6, 0 },
    { 19, 0, 20, 0, 0, 12 },
    { 21, 0, 11, 0, 0, 0 },
    { 22, 0, 17, 0, 0, 0 },
    { 22, 0, 16, -3, 0, 23 },
    { 23, 0, 25, 2, 0, 0 },
    { 23, 0, 18, 0, -4, 11 },
};
static const HitSphere D_8009C948[27] = {
    { 0, 0, 40, 0, 0, -8 },
    { 1, 0, 30, 0, 0, 0 },
    { 12, 10, 36, 0, 21, 14 },
    { 12, 10, 31, 0, 49, 8 },
    { 15, 10, 39, 0, 6, 27 },
    { 2, 0, 11, 0, 0, 0 },
    { 2, 0, 8, 6, -16, 0 },
    { 3, 0, 11, 0, 0, 0 },
    { 3, 0, 10, 5, -16, 0 },
    { 4, 0, 11, -2, 0, 0 },
    { 4, 0, 16, 3, -14, 1 },
    { 5, 0, 20, -9, 4, 0 },
    { 7, 0, 9, 0, 0, 0 },
    { 7, 0, 7, -3, -12, 0 },
    { 8, 0, 11, 0, 0, 0 },
    { 8, 0, 10, -6, -15, 0 },
    { 9, 0, 11, 2, 0, 0 },
    { 9, 0, 16, -2, -17, 0 },
    { 10, 0, 15, 3, 0, 0 },
    { 18, 0, 17, 0, 0, 0 },
    { 18, 0, 17, 9, 0, 23 },
    { 19, 0, 20, 18, 8, -5 },
    { 19, 0, 22, 20, 0, 12 },
    { 22, 0, 17, 0, 0, 0 },
    { 22, 0, 24, -10, 0, 24 },
    { 23, 0, 30, -16, -6, 0 },
    { 23, 0, 19, -22, 0, 17 },
};
static const HitSphere D_8009CA58[37] = {
    { 12, 10, 38, 0, 33, 11 },
    { 12, 0, 14, 0, -9, 0 },
    { 0, 4, 19, 0, 0, 4 },
    { 0, 4, 24, 0, 12, 6 },
    { 1, 5, 20, 0, 14, 7 },
    { 2, 0, 13, -6, -5, 0 },
    { 7, 0, 12, 3, -8, 0 },
    { 2, 9, 6, 16, -22, -2 },
    { 2, 9, 7, 10, -15, -1 },
    { 2, 9, 6, 4, -9, -2 },
    { 3, 9, 9, 0, 0, 0 },
    { 3, 9, 8, 5, -13, 0 },
    { 3, 9, 8, 9, -22, 0 },
    { 4, 9, 12, 0, -11, 0 },
    { 4, 9, 11, 6, -22, 0 },
    { 7, 8, 7, -6, -13, -2 },
    { 7, 8, 6, -12, -18, -3 },
    { 8, 8, 9, 0, 0, 0 },
    { 8, 8, 7, -9, -10, 0 },
    { 9, 8, 9, 3, 1, 0 },
    { 9, 8, 11, 0, -12, 2 },
    { 9, 8, 11, -5, -24, 0 },
    { 1, 0, 9, 0, 49, 0 },
    { 16, 7, 14, -2, -9, -1 },
    { 17, 7, 10, 0, 0, 0 },
    { 17, 7, 10, 3, -13, 0 },
    { 18, 7, 11, 0, -11, 0 },
    { 19, 7, 19, 0, -8, 0 },
    { 19, 7, 20, 3, -3, 11 },
    { 20, 6, 12, 0, -10, 0 },
    { 20, 6, 10, 0, -20, 0 },
    { 21, 6, 9, -5, -14, 0 },
    { 22, 6, 13, -4, -16, 4 },
    { 22, 6, 20, -5, -21, 20 },
    { 23, 6, 20, -4, -3, 7 },
    { 12, 10, 15, 22, 17, -4 },
    { 12, 10, 15, -30, 17, -3 },
};

/* The number of spheres of each table. */
static const u8 gHitSphereCounts[12] = { 7, 42, 43, 21, 35, 42, 31, 27, 37, 4, 37, 37 };

/* This file's .data: the sphere tables. */
static const HitSphere *gHitSphereTables[12] = {
    D_8009BD8C, D_8009BDFC, D_8009C3E8, D_8009C598, D_8009C288, D_8009C66C,
    D_8009C810, D_8009C948, D_8009BFA0, D_8009BDD4, D_8009CA58, D_8009C114,
};

/* Point vs. hit-sphere test: takes the point `dist` ahead of `self` (rotated by its pitch unkB8
 * and yaw unk40), rejects it against obj's whole-body sphere, then tests it against each of obj's
 * hit spheres (table `type`) grown by `radius`.  On a hit, calls actor_runAttackAt at obj's position
 * (unless obj is immune) and returns bone + 1; returns 0 for a miss. */
s32 collision_testPointHitSpheres(Actor *self, Actor *obj, s32 type, f32 dist, f32 radius, s32 attackId) {
    f32 scale;
    f32 angle;
    f32 s;
    f32 c;
    f32 x;
    f32 height;
    f32 r;
    f32 dx;
    f32 dy;
    f32 pos[3];
    f32 dz;
    ModelDef *bounds;
    s32 count;
    MtxF *bones;
    Vec3f local;
    Vec3f world;
    const HitSphere *p;
    s32 i;
    const HitSphere *spheres;
    s32 id;

    scale = self->scale_xz;
    dist *= scale;
    radius *= scale;
    angle = self->vtilt * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    pos[0] = 0.0f;
    pos[1] = -dist * s;
    pos[2] = dist * c;
    angle = self->theta * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    x = pos[0] * c + pos[2] * s;
    pos[2] = -pos[0] * s + pos[2] * c;
    pos[0] = x;
    pos[0] += self->xpos;
    pos[1] += self->ypos;
    pos[2] += self->zpos;
    id = obj->character_type;
    bounds = gModelDefs[id];
    // matching: the body radius reuses `angle`; a separate local shifts IDO's register allocation
    angle = bounds->bodyRadius * obj->scale_xz;
    height = bounds->bodyHeight * obj->scale_y;
    dx = pos[0] - obj->xpos;
    dy = pos[1] - (obj->ypos + height);
    dz = pos[2] - obj->zpos;
    if (angle * angle < dx * dx + dy * dy + dz * dz) {
        return 0;
    }
    spheres = gHitSphereTables[type];
    count = gHitSphereCounts[type];
    i = 0;
    bones = obj->ob_matrix_ptr;
    if (count > 0) {
        // matching: p = spheres in its own block; a plain assignment changes IDO's s-register allocation
        do { p = spheres; } while (0);
        do {
            local.x = p->x;
            local.y = p->y;
            local.z = p->z;
            // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
            math_transformOffset(&local, &world, (MtxF *)((u8 *)bones + p->bone * sizeof(MtxF)));
            dx = pos[0] - world.x;
            dy = pos[1] - world.y;
            dz = pos[2] - world.z;
            i++;
            r = p->radius * obj->scale_xz + radius;
            if (!(r * r < dx * dx + dy * dy + dz * dz)) {
                if (obj->imune == 0) {
                    actor_runAttackAt(obj->xpos, obj->ypos, obj->zpos, attackId, self - movement_obj, 0, 0, 0);
                }
                return p->bone + 1;
            }
            p++;
        } while (i != count);
    }
    return 0;
}

/* Returns the index of obj's model in the gModelHitTables hit table, or -1 if it has none. */
s32 collision_getHitTableIndex(Actor *obj) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if (obj->character_type == gModelHitTables[i].model) {
            return i;
        }
    }
    return -1;
}

/* Ray vs. per-bone hit-sphere test for one actor.  Broad phase: build the whole-object
 * bounding sphere from gModelDefs[obj->id] and ray-test it; if that hits, walk the model's
 * hit-sphere table, skipping bones masked off in obj->hiddenBones, transform each sphere's local
 * offset by its bone matrix and keep the nearest / highest-priority hit.
 *
 * ORIGINAL-GAME BUG, reproduced deliberately: the tail does `*outDist = tNear;` after the
 * loop, overwriting the tracked best distance with the LAST call's near-t rather than the
 * best one.  Golden does exactly this (0x15081460..0x15081484); it is not to be "fixed".
 */
s32 collision_rayHitSpheres(Actor *obj, Vec3f *rayOrigin, Vec3f *rayDir, Vec3f *outNear, Vec3f *outFar, Vec3f *outCentre, f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset) {
    s32 idx;
    const HitSphere *spheres;
    s32 count;
    s32 type;
    s32 bestIndex;
    f32 tNear;
    f32 tFar;
    s32 i;
    Vec3f centre;
    Vec3f bestCentre;
    Vec3f near;
    Vec3f far;
    Vec3f local;
    Vec3f bestNear;
    Vec3f bestFar;
    s32 bestPart;
    const HitSphere *p;
    MtxF *bones;
    f32 radius;

    if (obj->ob_matrix_ptr == NULL) {
        return -1;
    }
    idx = collision_getHitTableIndex(obj);
    if (idx == -1) {
        return -1;
    }
    type = gModelHitTables[idx].sphereTable;
    spheres = gHitSphereTables[type];
    count = gHitSphereCounts[type];
    *outDist = -1.0f;
    centre.x = obj->xpos;
    centre.y = gModelDefs[obj->character_type]->bodyHeight * obj->scale_y + obj->ypos;
    centre.z = obj->zpos;
    radius = gModelDefs[obj->character_type]->bodyRadius * obj->scale_y;
    if (math_raySphere(rayOrigin, rayDir, &centre, radius, &near, &far, &tNear, &tFar) == 0) {
        return 0;
    }
    bestPart = -1;
    i = 0;
    if (count > 0) {
        p = spheres;
        do {
            // matching: empty then-branch; `if (!(mask))` makes IDO rotate the s-registers
            if ((1 << p->bone) & obj->hiddenBones) {
                /* bone masked off in obj->hiddenBones */
            } else {
                bones = obj->ob_matrix_ptr;
                local.x = p->x;
                local.y = p->y;
                local.z = p->z;
                // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
                math_transformOffset(&local, &centre, (MtxF *)((u8 *)bones + p->bone * sizeof(MtxF)));
                if (math_raySphere(rayOrigin, rayDir, &centre, p->radius * obj->scale_xz, &near, &far, &tNear, &tFar) != 0) {
                    if (*outDist == -1.0f || (tNear < *outDist && p->part >= bestPart)) {
                        *outDist = tNear;
                        bestNear = near;
                        bestFar = far;
                        bestCentre = centre;
                        bestIndex = i;
                        bestPart = p->part;
                    }
                }
            }
            i++;
            p++;
        } while (i != count);
    }
    if (*outDist == -1.0f) {
        return 0;
    }
    *outDist = tNear;
    *outIndex = bestIndex;
    *outPart = spheres[bestIndex].part;
    *outBone = spheres[bestIndex].bone;
    outOffset[0] = spheres[bestIndex].x;
    outOffset[1] = spheres[bestIndex].y;
    outOffset[2] = spheres[bestIndex].z;
    *outNear = bestNear;
    *outFar = bestFar;
    if (outCentre != NULL) {
        *outCentre = bestCentre;
    }
    return 1;
}


/* Finds the first live object of the given model whose hit spheres the point `dist` ahead of
 * `self` (with reach `radius`) touches.  Stores it in *outTarget and returns bone + 1, or 0. */
s32 collision_findHitSphereTarget(Actor *self, f32 dist, f32 radius, Actor **outTarget, s32 attackId, s32 model) {
    s32 i;
    Actor *obj;
    s32 idx;
    s32 bone;

    *outTarget = NULL;
    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0 || obj->ob_matrix_ptr == NULL || model != obj->character_type) {
            continue;
        }
        idx = collision_getHitTableIndex(obj);
        if (idx == -1) {
            continue;
        }
        bone = collision_testPointHitSpheres(self, obj, gModelHitTables[idx].sphereTable, dist, radius, attackId);
        if (bone != 0) {
            *outTarget = obj;
            return bone;
        }
    }
    return 0;
}
