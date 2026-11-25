#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)D_800BE628)

/* A node of the level's path graph (loaded by func_15085B70), 0x10 bytes. */
typedef struct PathNode {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 layer; /* height band, see func_15085DA8 */
    /* 0x7 */ u8 unk7;  /* id looked up by func_15086D48 */
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 links[5]; /* neighbouring nodes, 0xFF = none */
    /* 0xE */ u8 type;
    /* 0xF */ u8 param;
} PathNode;

/* Camera request passed to func_1512D560 (mode 7). */
typedef struct CamParams {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 eyeX;
    /* 0x18 */ f32 eyeY;
    /* 0x1C */ f32 eyeZ;
    /* 0x20 */ f32 atX;
    /* 0x24 */ f32 atY;
    /* 0x28 */ f32 atZ;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} CamParams;

/* Per-racer state of the race mode (D_800872A0[]), 0x84 bytes. */
typedef struct Racer {
    /* 0x00 */ f32 lateral;       /* 0..1 across the track, see func_15088F30 */
    /* 0x04 */ f32 lateralTarget;
    /* 0x08 */ f32 segmentT;      /* spline parameter between nodes[1] and nodes[2] */
    /* 0x0C */ f32 dirX;
    /* 0x10 */ f32 dirZ;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s32 lapStartTime;
    /* 0x1C */ s32 raceTime;
    /* 0x20 */ s32 lastLapTime;
    /* 0x24 */ s16 nodesPassed;
    /* 0x26 */ s8 lapLength;      /* nodes per lap */
    /* 0x27 */ s8 lap;
    /* 0x28 */ s8 maxLap;
    /* 0x29 */ s8 rank;
    /* 0x2A */ s8 finishOrder;    /* 0x7F while still racing */
    /* 0x2B */ u8 nodes[4];       /* spline control nodes */
    /* 0x2F */ s8 route;
    /* 0x30 */ s8 state;
    /* 0x31 */ s8 actorIndex;     /* index into D_800CC2D0 */
    /* 0x32 */ s8 prevTicks;
    /* 0x33 */ s8 ticks;
    /* 0x34 */ f32 camT;
    /* 0x38 */ f32 camX;
    /* 0x3C */ f32 camY;
    /* 0x40 */ f32 camZ;
    /* 0x44 */ u8 camNodes[4];
    /* 0x48 */ u8 started; /* 0 during the pre-race fly-through, then nonzero */
    /* 0x49 */ s8 camState;
    /* 0x4A */ u8 camNode;
    /* 0x4B */ u8 camNextNode;
    /* 0x4C */ CamParams cam;
} Racer;

/* A circular area registered with func_1508B20C. */
typedef struct MpZone {
    /* 0x0 */ f32 radiusSq;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 z;
    /* 0xA */ s16 padA;
} MpZone;

/*
 * Multiplayer game state (D_800D23B0), loaded from resources 0xB7..0xBE. Arrays are per player or
 * per slot. Some functions null-test arrays (grenades, unk55C, attacker, ...) as if they were
 * pointers; the tests are always true but are kept, in the exact form that matches.
 */
typedef struct MpGame {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ s32 unk4;
    /* 0x0008 */ s32 unk8;
    /* 0x000C */ s32 unkC;
    /* 0x0010 */ s32 numPlayers;
    /* 0x0014 */ s32 activeMask;
    /* 0x0018 */ s32 seenMask;
    /* 0x001C */ s32 unk1C[16];
    /* 0x005C */ s32 unk5C[16];
    /* 0x009C */ s32 layer[16]; /* height band, func_15085DA8 */
    /* 0x00DC */ s32 unkDC[16];
    /* 0x011C */ s32 target[16];              /* player each one is after, -1 = none */
    /* 0x015C */ s32 unk15C[16];
    /* 0x019C */ s32 unk19C[16];
    /* 0x01DC */ s32 unk1DC[16];
    /* 0x021C */ s32 aimTime[16];             /* frames spent on the current aim */
    /* 0x025C */ s32 unk25C[16];
    /* 0x029C */ s32 attackTimer[16];         /* fires when it turns non-negative */
    /* 0x02DC */ s32 unk2DC[16];
    /* 0x031C */ s32 unk31C[16];
    /* 0x035C */ s32 unk35C[16];
    /* 0x039C */ s32 unk39C[16];
    /* 0x03DC */ s32 attacker[16];            /* last hit by (| 0x80 when fresh), from D_800E0BB8 */
    /* 0x041C */ s32 unk41C[16];
    /* 0x045C */ s32 opponentDist[16];
    /* 0x049C */ s32 opponent[16];            /* chosen by func_1508B9BC */
    /* 0x04DC */ s32 grenades[16];
    /* 0x051C */ s32 nearestEnemyDist[16];
    /* 0x055C */ s32 unk55C[16];
    /* 0x059C */ s32 unk59C[16];
    /* 0x05DC */ s32 targets[16][16];     /* per player: other players sorted by targetDists, -1 ends */
    /* 0x09DC */ s32 targetDists[16][16];
    /* 0x0DDC */ s32 teamScore[16];
    /* 0x0E1C */ u8 padE1C[0x48];
    /* 0x0E64 */ s32 team[16];
    /* 0x0EA4 */ s32 numSlots;
    /* 0x0EA8 */ s32 numSpawns;               /* entries in spawnNodes */
    /* 0x0EAC */ s32 respawnDelay;            /* slotTimer after a pickup */
    /* 0x0EB0 */ s32 slot[16];                /* slot held by each player, -1 = none */
    /* 0x0EF0 */ s32 slotEntity[32];          /* func_151149AC id of each slot's pickup */
    /* 0x0F70 */ s32 slotOwner[32];           /* player holding each slot (| 0x2000 once taken), -1 = none */
    /* 0x0FF0 */ s32 itemOverride[32];        /* per player: 0 = none, 0x457 = no item */
    /* 0x1070 */ s32 slotTimer[32];           /* >0 counting down to respawn, 0 = available, -1 = held */
    /* 0x10F0 */ s32 slotSpawn[32];           /* spawnNodes index it sits at, -1 = none */
    /* 0x1170 */ s32 defaultItem;             /* item of a player without a slot */
    /* 0x1174 */ s32 slotItem[32];
    /* 0x11F4 */ s32 slotFlags[32];           /* 1 random spawn, 2 respawns instead of being held, 4 never moves */
    /* 0x1274 */ s32 slotHomeSpawn[32];
    /* 0x12F4 */ s32 slotDropTimer[32];       /* frames before a dropped pickup returns */
    /* 0x1374 */ s32 slotDropX[32];           /* where it was dropped */
    /* 0x13F4 */ s32 slotDropY[32];
    /* 0x1474 */ s32 slotDropZ[32];
    /* 0x14F4 */ s32 spawnNodes[32];          /* path node ids (func_15086D48) */
    /* 0x1574 */ s32 countdown[16];           /* frames, drawn in seconds in game type 0xB9 */
    /* 0x15B4 */ u8 weights[16][16];
    /* 0x16B4 */ f32 ticks;
    /* 0x16B8 */ f32 unk16B8;
    /* 0x16BC */ s16 gameType; /* resource id */
    /* 0x16BE */ s16 pad16BE;
    /* 0x16C0 */ s32 unk16C0[16];
    /* 0x1700 */ s8 unk1700;
    /* 0x1701 */ s8 unk1701;
    /* 0x1702 */ s8 unk1702;
    /* 0x1703 */ s8 unk1703;
    /* 0x1704 */ s8 unk1704;
    /* 0x1705 */ s8 sideGoal[16];             /* aim offset goals (func_1508CAD8 sideOffset/dropOffset) */
    /* 0x1715 */ s8 dropGoal[16];
    /* 0x1725 */ s8 side[16];                 /* eased toward the goals */
    /* 0x1735 */ s8 drop[16];
    /* 0x1745 */ s8 numZones;
    /* 0x1748 */ MpZone zones[8];
    /* 0x17A8 */ f32 aimX[16];                /* smoothed aim point */
    /* 0x17E8 */ f32 aimY[16];
    /* 0x1828 */ f32 aimZ[16];
    /* 0x1868 */ f32 aimRate[16];             /* aim easing per frame */
    /* 0x18A8 */ u8 pad18A8[0x24B8 - 0x18A8];
    /* 0x24B8 */ s32 unk24B8[9];              /* by unique_id - 20 (game type 0x3F), see func_1508E89C */
    /* 0x24DC */ s32 unk24DC[9];
} MpGame;



extern s8 D_800D239A;
extern s8 D_800D2398;
extern s8 D_800D2399;
extern MpGame *D_800D23B0;
s32 func_1508802C(Racer *racer, Actor *obj, s32 mode);
extern PathNode *D_800D2350;
extern f32 sqrtf(f32);
u8 func_15085DA8(f32 arg0);
s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s8 arg3, s8 arg4);

#pragma intrinsic(sqrtf)

/* Distance between two path nodes, 0 if either is 0xFF. */
f32 func_15086BD0(s32 a, s32 b) {
    f32 dx;
    f32 dy;
    f32 dz;
    PathNode *nodes;

    if (a == 0xFF || b == 0xFF) {
        return 0.0f;
    }
    nodes = D_800D2350;
    dx = nodes[a].x - nodes[b].x;
    dy = nodes[a].y - nodes[b].y;
    dz = nodes[a].z - nodes[b].z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void func_15086C68(void) {
}

extern void func_150A3194(s32, s32, s32, s32, s32);

/* Calls func_150A3194(3, 11, ...) at path node `index`. */
void func_15086C70(s32 index) {
    s16 z = D_800D2350[index].z;

    func_150A3194(3, 11, D_800D2350[index].x, D_800D2350[index].y, z);
}

/* Writes the position of path node `index` to *x, *y, *z; returns 0 if out of range. */
s32 func_15086CBC(s32 index, f32 *x, f32 *y, f32 *z) {
    if (index < 0 || index >= D_80087290) {
        return 0;
    }
    *x = D_800D2350[index].x;
    *y = D_800D2350[index].y;
    *z = D_800D2350[index].z;
    return 1;
}

/* Returns the index of the path node whose unk7 equals `id`, or 0xFF. */
s32 func_15086D48(s32 id) {
    s32 i;

    for (i = 0; i < D_80087290; i++) {
        if (id == D_800D2350[i].unk7) {
            return i;
        }
    }
    return 0xFF;
}

/*
 * Casts a ray from (x, z) along (dx, dz) against the wall edges (type 1 node pairs) of the
 * height band containing y. Returns the distance to the hit, or -1 if no edge is crossed.
 */
f32 func_15086D94(f32 x, f32 y, f32 z, f32 dx, f32 dz) {
    s32 pad[4]; // matching: unused stack space in the original frame
    u8 id;
    s32 i;
    s32 j;
    PathNode *n1;
    PathNode *n2;
    f32 a;
    f32 b;
    f32 c;
    f32 d1;
    f32 d2;
    f32 t;
    f32 best;
    u8 link;

    id = func_15085DA8(y);
    best = 100.0f;
    for (i = 0; i < D_80087290; i++) {
        n1 = &D_800D2350[i];
        if (n1->type == 1 && n1->layer == id) {
            for (j = 0; j < 5; j++) {
                link = n1->links[j];
                if (link != 0xFF && i < link) {
                    n2 = &D_800D2350[link];
                    if (n2->type == 1) {
                        a = n2->z - n1->z;
                        b = -(f32)(n2->x - n1->x);
                        c = -((f32)n1->x * a + b * (f32)n1->z);
                        d1 = x * a + z * b + c;
                        d2 = (x + dx) * a + (z + dz) * b + c;
                        if ((d2 < 0.0f && d1 >= 0.0f) || (d1 < 0.0f && d2 >= 0.0f)) {
                            if (d2 < 0.0f) {
                                d2 = -d2;
                            }
                            if (d1 < 0.0f) {
                                d1 = -d1;
                            }
                            t = d1 / (d1 + d2);
                            c = a;
                            a = -b;
                            b = c;
                            c = -((f32)n1->x * a + b * (f32)n1->z);
                            d1 = ((t * dx) + x) * a + ((t * dz) + z) * b + c;
                            d2 = (f32)n2->x * a + b * (f32)n2->z + c;
                            if (((d2 > 0.0f && d1 > 0.0f && d1 <= d2) || (d2 < 0.0f && d1 < 0.0f && d2 <= d1)) && t < best) {
                                best = t;
                            }
                        }
                    }
                }
            }
        }
    }
    if (best <= 1.0f) {
        return sqrtf(dx * dx + dz * dz) * t;
    }
    return -1.0f;
}

extern Racer *D_800872A0;
extern s32 D_800D2394;
extern void *allocate_memory(s32, s32, s32, s32);
u8 func_150888A8(u8, u8, s8);
void func_15088824(Racer *racer);

/*
 * Allocates the racers (numRacers racing plus numSpare free slots) and places them on the
 * path at the node with id `startId`, measuring the lap length on the way.
 */
void func_150870D0(s32 startId, s32 numRacers, s32 numSpare) {
    Racer *racer;
    s32 lapLength;
    s32 i;
    PathNode *node;
    s32 pad; // matching: unused stack slot in the original frame
    u8 start;
    u8 best;
    u8 fwd;
    u8 back;
    u8 sel;
    u8 path[3];
    PathNode *startNode;
    u8 idx;

    D_800872A0 = NULL;
    start = func_15086D48(startId);
    if (start == 0xFF) {
        return;
    }
    startNode = &D_800D2350[start];
    best = D_800D23A8 * 0xFF;
    sel = 0xFF;
    for (i = 0; i < 5; i++) {
        idx = startNode->links[i];
        if (idx != 0xFF) {
            node = &D_800D2350[idx];
            if (node->type == 0) {
                if ((D_800D23A8 == 0 && node->param >= best) || (D_800D23A8 != 0 && node->param <= best)) {
                    best = node->param;
                    sel = idx;
                }
            }
        }
    }

    if (sel != 0xFF) {
        fwd = func_150888A8(start, sel, 0);
        back = func_150888A8(sel, start, 0);
    }
    if (sel == 0xFF || back == 0xFF || fwd == 0xFF) {
        return;
    }
    path[1] = sel;
    path[2] = fwd;
    lapLength = 0;
    do {
        path[0] = path[1];
        path[1] = path[2];
        path[2] = func_150888A8(path[0], path[1], 0);
        lapLength++;
    } while (start != path[0]);
    D_800D2399 = numSpare;
    D_800D2398 = numRacers;
    numRacers += numSpare;
    D_800D2394 = 0;
    D_800872A0 = allocate_memory(numRacers * sizeof(Racer), 1, 0, 0);
    for (i = 0; i < numRacers; i++) {
        racer = &D_800872A0[i];
        func_15088824(racer);
        racer->nodes[0] = back;
        racer->nodes[1] = start;
        racer->nodes[2] = sel;
        racer->nodes[3] = fwd;
        if (i < D_800D2398) {
            racer->actorIndex = i;
        }
        racer->state = i >= D_8008FD90;
        racer->lapLength = lapLength;
    }

    D_800D239A = 1;
}

f32 sinf(f32);
f32 cosf(f32);
extern f32 D_800CC310;
extern s8 D_800D2390;
void func_15088F30(Racer *racer, f32 *outX, f32 *outY, f32 *outZ);
f32 func_150497E0(f32 *, s32, f32);
f32 func_150498A4(f32 *, s32, f32, f32 *);
s32 func_15088A08(Racer *racer, f32 step);

/*
 * Per-frame update of racer `index`: steers it along the path to follow its actor, tracks its
 * speed and race time and, for racer 0, recomputes everyone's rank. Until `started`, the
 * camera target circles the actor.
 */
void func_15087350(s32 index, s32 started, f32 lookahead) {
    f32 f;
    f32 g;
    f32 posX;
    f32 posZ;
    f32 dx;
    f32 dz;
    f32 d;
    f32 dist;
    f32 r;
    f32 prev;
    f32 len;
    f32 adist;
    s32 j;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    Racer *racer;
    Actor *obj;
    s8 iter;
    s8 ok;
    Racer copy;
    f32 scale;
    s32 pad; // matching: unused stack slot in the original frame
    f32 inv;
    s32 again;
    s32 i;
    s32 sorted;
    f32 ftmp;
    s8 order[4];
    s32 itmp;
    f32 vals[16];

    iter = 0;
    if (D_800872A0 != NULL) {
        racer = &D_800872A0[index];
        obj = &D_800CC2D0[racer->actorIndex];
        posX = obj->x_position;
        posZ = obj->z_position;
        D_800D2390 = 0;
        if (racer->state == 0) {
            racer->lateral = 0.5f;
        } else {
            f = (racer->lateralTarget - racer->lateral) * 0.125f;
            if (f > 0.05999999866f) {
                f = 0.05999999866f;
            }
            if (f < -0.05999999866f) {
                f = -0.05999999866f;
            }
            racer->lateral += f;
        }
        prev = 10000.0f;
        scale = 0.75f;
        ok = 1;
        do {
            again = 0;
            if (ok) {
                func_15088F30(racer, xs, ys, zs);
                dx = xs[2] - xs[1];
                dz = zs[2] - zs[1];
                len = sqrtf(dx * dx + dz * dz);
            }
            f = func_150498A4(xs, 0, racer->segmentT, &dx);
            g = func_150498A4(zs, 0, racer->segmentT, &dz);
            inv = -1.0f / sqrtf(dx * dx + dz * dz);
            dx *= inv;
            dz *= inv;
            racer->dirX = dx;
            racer->dirZ = dz;
            d = -(f * dx + g * dz);
            dist = dx * posX + dz * posZ + d;
            r = dist;
            if (r < -50.0f || r > 50.0f) {
                adist = r;
                if (r < 0.0f) {
                    adist = -r;
                }
                if (prev < adist) {
                    scale *= 0.5f;
                }
                prev = adist;
                again = 1;
                inv = (r * scale) / len;
                if (inv < -0.5f) {
                    inv = -0.5f;
                }
                if (inv > 0.5f) {
                    inv = 0.5f;
                }
                ok = func_15088A08(racer, -inv);
            }
            iter++;
            if (iter >= 11) {
                again = 0;
            }
        } while (again);
        func_15042D78(0x80);
        racer->prevTicks = racer->ticks;
        racer->ticks = D_800BE9E4;
        adist = (obj->old_x_position * dx + dz * obj->old_z_position + d) - dist;
        racer->speed = (adist * 4.0f) / racer->prevTicks;
        if (racer->state != 0) {
            bcopy(racer, &copy, sizeof(Racer));
            func_15088A08(&copy, lookahead + lookahead);
            func_15088F30(&copy, xs, ys, zs);
            D_800D239C = func_150497E0(xs, 0, copy.segmentT);
            D_800D23A0 = func_150497E0(ys, 0, copy.segmentT);
            D_800D23A4 = func_150497E0(zs, 0, copy.segmentT);
            if (index < D_800D2398) {
                if (started == 0) {
                    f = D_800CC310 * 0.01745;
                    D_800D239C = sinf(f) * 300.0f + obj->x_position;
                    D_800D23A0 = obj->y_position;
                    D_800D23A4 = cosf(f) * 300.0f + obj->z_position;
                }
                D_800D2104[obj->unk13F][1].x = D_800D239C;
                D_800D2104[obj->unk13F][1].y = D_800D23A0;
                D_800D2104[obj->unk13F][1].z = D_800D23A4;
                obj->unk44 = started;
                if (D_800D2390 == 1) {
                    obj->y_velocity = 75.0f;
                } else if (D_800D2390 == 2) {
                    obj->y_velocity = 55.0f;
                } else if (D_800D2390 == 3) {
                    obj->y_velocity = 65.0f;
                }
                obj->unk21E = 0;
            lbl:; // matching: the empty labelled statement splits the block for IDO's scheduler
            }
        } else {
            racer->lateral = 0.0f;
            func_15088F30(racer, xs, ys, zs);
            f = func_150497E0(xs, 0, racer->segmentT);
            g = func_150497E0(zs, 0, racer->segmentT);
            racer->lateral = 1.0f;
            func_15088F30(racer, xs, ys, zs);
            dx = func_150497E0(xs, 0, racer->segmentT) - f;
            dz = func_150497E0(zs, 0, racer->segmentT) - g;
            dist = sqrtf(dx * dx + dz * dz);
            if (dist > 1.0f) {
                dist = 1.0f / dist;
                dx *= dist;
                dz *= dist;
                racer->lateral = ((dx * posX + dz * posZ) - (dx * f + dz * g)) * dist;
            } else {
                racer->lateral = 0.0f;
            }
        }
        racer->started = started;
        D_800D23A9 = 1;
        if (racer->state == 0 && racer->started > 0 && !(D_800D18A0 & (1 << racer->actorIndex))) {
            racer->raceTime += D_800BE9E4;
        }
        if (racer->state >= 2 && obj->unk31C->unk84 == 0) {
            func_1508802C(racer, obj, 13);
        }
        if (index == 0) {
            for (i = 0; i < D_800D2398; i++) {
                order[i] = i;
                // matching: (D_800872A0 + i)-> rather than D_800872A0[i] keeps IDO's FP register order
                vals[i] = (D_800872A0 + i)->segmentT + (D_800872A0 + i)->nodesPassed;
                if ((D_800872A0 + i)->finishOrder != 0x7F) {
                    vals[i] += 30000.0f - (D_800872A0 + i)->finishOrder * 1000.0f;
                }
                j = (D_800872A0 + i)->actorIndex;
                if (D_800D18A0 & (1 << j)) {
                    vals[i] -= 10000.0f;
                }
            }
            do {
                sorted = 1;
                for (i = 0; i < D_800D2398 - 1; i++) {
                    j = i + 1;
                    if (vals[i] < vals[j]) {
                        ftmp = vals[i];
                        itmp = order[i];
                        order[i] = order[j]; vals[i] = vals[j];
                        sorted = 0;
                        vals[j] = ftmp;
                        order[j] = itmp;
                    }
                }
            } while (!sorted);
            for (i = 0; i < D_800D2398; i++) {
                D_800872A0[order[i]].rank = i + 1;
            }
        }
    }
}

void func_150891E8(Racer *racer, Actor *obj);
void func_150896EC(Racer *racer, Actor *obj, s32 leave);

/* Runs the race cameras of every racer whose actor has a camera. */
void func_15087CC0(void) {
    Racer *racer;
    Actor *obj;
    s32 i;

    D_800D23A9 = 0;
    if (D_800872A0 == NULL) {
        return;
    }
    for (i = 0; i < D_800D2398; i++) {
        racer = &D_800872A0[i];
        obj = &D_800CC2D0[racer->actorIndex];
        if (obj->camera != NULL) {
            if (racer->state >= 2) {
                func_150891E8(racer, obj);
            } else if (racer->started == 0) {
                func_150896EC(racer, obj, 0);
            } else if (racer->camState != 0) {
                func_150896EC(racer, obj, 1);
            }
        }
    }
}

/* Sets racer `index`'s route, re-picking the nodes ahead when it is nonzero. */
void func_15087DCC(s32 index, s32 route) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    if (route == racer->route) {
        return;
    }
    if (route != 0) {
        racer->nodes[2] = func_150888A8(racer->nodes[0], racer->nodes[1], 1);
        racer->nodes[3] = func_150888A8(racer->nodes[1], racer->nodes[2], 1);
    }
    racer->route = route;
}

/* Keeps obj's movement angle within 0x2400 of racer `index`'s track direction. */
void func_15087E54(s32 index, Actor *obj) {
    Racer *racer;
    u16 d;
    u16 r;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    r = func_1505A630(racer->dirZ, racer->dirX, 0) + 0x4000;
    d = (u16)(r - obj->unk76);
    if (d & 0x8000) {
        if (d < 0xDBFF) {
            obj->unk76 = r + 0x2400;
        }
    } else {
        if (d >= 0x2401) {
            obj->unk76 = r - 0x2400;
        }
    }
}

/* Points obj's movement angle along the track, angled toward its centre line. */
void func_15087EF0(s32 index, Actor *obj) {
    Racer *racer;
    u16 r;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    r = func_1505A630(racer->dirZ, racer->dirX, 0);
    if (D_800D23A8 == 0) {
        if (racer->lateral < 0.5f) {
            r = (u16)(r + 0x4A00);
        } else {
            r = (u16)(r + 0x3600);
        }
    } else {
        if (0.5f <= racer->lateral) {
            r = (u16)(r + 0x4A00);
        } else {
            r = (u16)(r + 0x3600);
        }
    }
    obj->unk76 = r;
}

/* Binds racer `index` to actor `actorIndex`. */
void func_15087FC4(s32 index, s32 actorIndex) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    racer->actorIndex = actorIndex;
}

/* Sets racer `index`'s lateral target, in 1/256ths of the track width. */
void func_15087FEC(s32 index, s32 lateral) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    racer->lateralTarget = (f32)lateral * (1.0f / 256.0f);
}

/* Ends the race for `racer` (actor obj), entering state 12 with `mode` (unk232); returns 1 if it can't finish now. */
s32 func_1508802C(Racer *racer, Actor *obj, s32 mode) {
    ActorCtl *state;

    state = obj->unk31C;
    if (state->unk120 != 0) {
        return 1;
    }

    if ((D_800D18A0 & (1 << (obj - D_800CC2D0))) != 0) {
        return 1;
    }

    state->unk84 = 1;
    obj->interaction_state = 12;
    obj->unk232 = mode;
    D_800D154C = obj;
    D_800C3E78 = racer->actorIndex;
    obj->unk218 = func_1507BB28(0, mode);
    obj->unk21C = 0;
    obj->immune = 0xFF;
    racer->state = 2;
    return 0;
}

/* Racer `index` crosses the finish line: record its finish order. */
s32 func_150880F8(s32 index, s32 mode) {
    Racer *racer;
    Actor *obj;
    s32 ret;

    ret = 0;
    if (D_800872A0 == NULL) {
        return 0;
    }

    racer = &D_800872A0[index];
    obj = &D_800CC2D0[racer->actorIndex];
    if (racer->state == 0) {
        ret = func_1508802C(racer, obj, mode);
    } else if (racer->state == 1) {
        racer->state = 2;
        obj->immune = 0xFF;
    }

    racer->finishOrder = D_800D239A;
    D_800D239A++;
    return ret;
}

/* Racer `index`'s lateral position in 1/256ths of the track width. */
s32 func_150881CC(s32 index) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return 0;
    }
    racer = &D_800872A0[index];
    return (s32)(racer->lateral * 256.0f);
}

/* Racer `index`'s distance along the path, in 1/16ths of a node. */
s32 func_15088218(s32 index) {
    Racer *racer;
    s32 v;
    if (D_800872A0 == NULL) {
        return 0;
    }
    racer = &D_800872A0[index];
    v = racer->nodesPassed;
    return (v << 4) + (s32)(racer->segmentT * 16.0f);
}

/* Returns racer `index`'s speed, truncated (0 without racers). */
s32 func_15088270(s32 index) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return 0;
    }
    racer = &D_800872A0[index];
    return (s32)racer->speed;
}

/* Returns racer `index`'s lap (0 without racers). */
s32 func_150882B0(s32 index) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return 0;
    }
    racer = &D_800872A0[index];
    return racer->lap;
}

/*
 * Finds the racer ranked `offset` places from racer `index`. Returns
 * (path distance between them << 8) | that racer's index, or 0x10 if there is none.
 */
s32 func_150882E4(s32 index, s32 offset) {
    Racer *racer;
    Racer *other;
    s32 i;
    s32 wanted;

    if (D_800872A0 == NULL) {
        return 0x10;
    }

    racer = &D_800872A0[index];
    wanted = racer->rank + offset;
    i = 0;
    if (D_800D2398 > 0) {
        other = D_800872A0;
        do {
            if (wanted == other->rank) {
                if (i != index) {
                    index = (racer->nodesPassed << 4) + (s32)(racer->segmentT * 16.0f);
                    index = index - (other->nodesPassed << 4) - (s32)(other->segmentT * 16.0f);
                    if (index < 0) {
                        index = -index;
                    }
                    return (index << 8) | i;
                }
            }
            i++;
            other++;
        } while (i < D_800D2398);
    }

    return 0x10;
}

extern f32 D_8009DA00;

/*
 * Finds the nearest racer ahead of (dir >= 0) or behind racer `index` on the same route.
 * Stores its lateral position and route in *lateral and *route.
 */
Actor *func_150883B0(s32 index, s32 dir, f32 *lateral, s32 *route) {
    Racer *racer;
    Actor *ret;
    f32 sign;
    f32 dx;
    f32 dz;
    f32 x;
    f32 z;
    f32 base;
    f32 best;
    f32 d;
    s32 i;
    s8 self;

    if (D_800872A0 == NULL) {
        return NULL;
    }
    racer = &D_800872A0[index];
    if (dir >= 0) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    dir = racer->route;
    x = D_800CC2D0[racer->actorIndex].x_position;
    z = D_800CC2D0[racer->actorIndex].z_position;
    dx = racer->dirX * sign;
    dz = racer->dirZ * sign;
    self = D_800CC2D0[racer->actorIndex].unk124 - 1;
    base = -((dx * x) + (dz * z));
    ret = NULL;
    best = D_8009DA00;
    for (i = 0; i < D_800D2398; i++) {
        racer = &D_800872A0[i];
        if (self != racer->actorIndex && (dir == 0 || dir == racer->route)) {
            x = D_800CC2D0[racer->actorIndex].x_position;
            z = D_800CC2D0[racer->actorIndex].z_position;
            d = (dx * x) + (dz * z) + base;
            if (d > 0.0f && d < best) {
                best = d;
                *lateral = racer->lateral;
                *route = racer->route;
                ret = &D_800CC2D0[racer->actorIndex];
            }
        }
    }
    return ret;
}

/* Returns the index of the racer driven by obj, or -1. */
s32 func_1508855C(Actor *obj) {
    Racer *racers;
    s32 actorIndex;
    s32 i;
    s32 limit;
    Racer *racer;

    actorIndex = obj - D_800CC2D0;
    racers = D_800872A0;
    if (racers == NULL) {
        return -1;
    }

    if (actorIndex == 0) {
        return 0;
    }

    i = 1;
    limit = D_800D2398 + D_800D2399;
    racer = &racers[1];
    // matching: the goto keeps IDO from unrolling this loop
    if (limit < 2) {
        goto fail;
    }

    do {
        if (actorIndex == racer->actorIndex) {
            return i;
        }
        i++;
        racer++;
    } while (i < limit);

fail:
    return -1;
}

void bcopy(const void *, void *, int);

/* Copies racer `src` over racer `dst`, keeping dst's actor and state. */
void func_150885EC(s32 src, s32 dst) {
    s32 actorIndex;
    s32 state;

    if (D_800872A0 != NULL) {
        actorIndex = D_800872A0[dst].actorIndex;
        state = D_800872A0[dst].state;
        bcopy(&D_800872A0[src], &D_800872A0[dst], sizeof(Racer));
        D_800872A0[dst].actorIndex = actorIndex;
        D_800872A0[dst].state = state;
    }
}

/* Claims a free spare racer slot for actor obj; returns its index or -1. */
s32 func_1508868C(Actor *obj) {
    s32 i;
    s32 bit;
    Racer *racers;

    racers = D_800872A0;
    if (racers == NULL) {
        return -1;
    }

    bit = 1;
    i = 0;
    if (D_800D2399 > 0) {
        do {
            if ((D_800D2394 & bit) == 0) {
                D_800D2394 |= bit;
                i += D_800D2398;
                func_15088824(&racers[i]);
                D_800872A0[i].state = 1;
                D_800872A0[i].actorIndex = obj - D_800CC2D0;
                return i;
            }
            i++;
            bit <<= 1;
        } while (i < D_800D2399);
    }
    return -1;
}

/* Releases the spare racer slot of the calling actor. */
void func_15088780(void) {
    s32 func_1508855C(); /* called without its actor argument: a0 passes through */
    s32 index;

    if (D_800872A0 != NULL) {
        index = func_1508855C();
        D_800872A0[index].actorIndex = 0;
        D_800D2394 &= ~(1 << (index - D_800D2398));
    }
}

/* Returns whether racer 0's camNodes[2] is 0xFF (no node). */
s32 func_150887F8(void) {
    Racer *racers = D_800872A0;
    if (racers == NULL) {
        return 0;
    }
    return racers->camNodes[2] == 0xFF;
}

/* Resets a racer to the start of the race. */
void func_15088824(Racer *racer) {
    racer->nodes[0] = 0;
    racer->nodes[1] = 0;
    racer->nodes[2] = 0;
    racer->nodes[3] = 0;
    racer->maxLap = 0;
    racer->raceTime = 0;
    racer->lapStartTime = 0;
    racer->lastLapTime = -1;
    racer->route = 0;
    racer->segmentT = 0.0f;
    racer->lateral = 0.5f;
    racer->lateralTarget = 0.5f;
    racer->nodesPassed = 0;
    racer->lapLength = 0;
    racer->lap = 0;
    racer->actorIndex = 0;
    racer->dirX = 0.0f;
    racer->speed = 0.0f;
    racer->ticks = 2;
    racer->state = 0;
    racer->finishOrder = 0x7F;
    racer->camState = 0;
    racer->dirZ = 1.0f;
}

/*
 * Picks the next node after `cur` coming from `prev`: prefers a type-0 neighbour whose param
 * differs from the route mode (1 when `route` is set, else 2), else one that matches it.
 */
u8 func_150888A8(u8 prev, u8 cur, s8 route) {
    s32 i;
    PathNode *node;
    PathNode *e;
    u8 best;
    u8 back;
    s32 mode;

    best = 0xFF;
    back = 0xFF;
    if (route != 0) {
        mode = 1;
    } else {
        mode = 2;
    }
    node = &D_800D2350[cur];
    for (i = 0; i < 5; i++) {
        if (node->links[i] != 0xFF && node->links[i] != prev) {
            e = &D_800D2350[node->links[i]];
            if (e->type == 0) {
                if (mode != e->param) {
                    if (best == 0xFF || e->param != 0) {
                        best = node->links[i];
                    }
                } else {
                    back = node->links[i];
                }
            }
        }
    }
    if (best == 0xFF) {
        best = back;
    }
    return best;
}


/*
 * Advances `racer` by `step` along the path, shifting its spline nodes when it crosses one.
 * Counts laps and records lap times. Returns the node step taken (-1, 0 or 1).
 */
s32 func_15088A08(Racer *racer, f32 step) {
    s32 dir;
    s32 idx;
    s32 lim;
    s32 t;
    s32 v;

    dir = 0;
    racer->segmentT += step;
    if (racer->segmentT < 0.0f) {
        racer->segmentT += 1.0f;
        racer->nodes[3] = racer->nodes[2];
        racer->nodes[2] = racer->nodes[1];
        racer->nodes[1] = racer->nodes[0];
        racer->nodes[0] = func_150888A8(racer->nodes[2], racer->nodes[1], racer->route);
        dir = -1;
    } else if (racer->segmentT >= 1.0f) {
        racer->segmentT -= 1.0f;
        racer->nodes[0] = racer->nodes[1];
        racer->nodes[1] = racer->nodes[2];
        racer->nodes[2] = racer->nodes[3];
        racer->nodes[3] = func_150888A8(racer->nodes[1], racer->nodes[2], racer->route);
        dir = 1;
        if (D_800D23A8 == 0) {
            if (D_800D2350[racer->nodes[1]].param == 5) {
                D_800D2390 = 1;
            }
            if (D_800D2350[racer->nodes[1]].param == 6) {
                D_800D2390 = 2;
            }
        } else {
            if (D_800D2350[racer->nodes[1]].param == 7) {
                D_800D2390 = 1;
            }
            if (D_800D2350[racer->nodes[1]].param == 8) {
                D_800D2390 = 2;
            }
            if (D_800D2350[racer->nodes[1]].param == 9) {
                D_800D2390 = 3;
            }
        }
    }

    if (dir != 0) {
        lim = D_800E0BE7;
        idx = racer->actorIndex;
        racer->nodesPassed += dir;
        racer->lap = racer->nodesPassed / racer->lapLength;
        if (D_800BE616 != 0) {
            if (lim < racer->lap) {
                racer->lap = lim + 1;
            }
        }
        if (racer->maxLap < racer->lap) {
            racer->maxLap = racer->lap;
            if (idx < D_8008FD90 && racer->state == 0 && racer->raceTime > 0) {
                if (lim >= racer->maxLap) {
                    t = racer->raceTime - racer->lapStartTime;
                    if (t > 32000) {
                        t = 32000;
                    }
                    if (D_800E0C10[idx] <= 0 || t < D_800E0C10[idx]) {
                        D_800E0C10[idx] = t;
                    }
                    racer->lastLapTime = t;
                }
                if (lim == racer->maxLap) {
                    v = racer->raceTime;
                    if (v > 360000) {
                        v = 360000;
                    }
                    D_800E0C18[idx] = v;
                }
            }
            if (lim == racer->maxLap) {
                racer->lapStartTime = -racer->raceTime;
            } else if (racer->maxLap < lim) {
                racer->lapStartTime = racer->raceTime;
            }
        }
    }

    return dir;
}

f32 func_15144BC8(f32);
extern f32 D_8009DA04;

/* Places actor obj at the start of its racer's path, facing along it. */
void func_15088D58(Actor *obj) {
    Racer *racer;
    f32 angle;
    f32 x_derivative;
    f32 z_derivative;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    s32 rotation;
    s32 index;
    s32 i;
    s32 link;
    s32 again;
    PathNode *target;
    u8 *cursor;
    s32 none;

    none = 0xFF;
    index = func_1508855C(obj);
    if (index >= 0) {
        racer = &D_800872A0[index];
        racer->lateral = 0.5f;
        racer->lateralTarget = 0.5f;
        racer->segmentT = 0.5f;
        do {
            /* skip forward while the current node's edge partner (type 4) has a nonzero param */
            again = 0;
            i = 0;
            // matching: a byte cursor over the node (links at +9) and 0xFF in a variable keep the frame size
            cursor = (u8 *)&D_800D2350[racer->nodes[1]];
            do {
                link = cursor[9];
                i++;
                if (none != link) {
                    target = &D_800D2350[link];
                    if (target->type == 4) {
                        again = target->param;
                    }
                }
                cursor++;
            } while (i != 5);
            if (again != 0) {
                func_15088A08(racer, 1.0f);
            }
        } while (again != 0);
        racer->segmentT = 0.0f;
        func_15088F30(racer, xs, ys, zs);
        obj->x_position = func_150498A4(xs, 0, racer->segmentT, &x_derivative);
        obj->y_position = func_150497E0(ys, 0, racer->segmentT);
        obj->z_position = func_150498A4(zs, 0, racer->segmentT, &z_derivative);
        angle = func_15144BC8((f32)(func_1505A630(racer->dirZ, racer->dirX, 0) + 0x8000) * 0.005493164f);
        obj->unk40 = angle;
        rotation = (s32)(short)(s32)(angle * D_8009DA04) - 0x4000;
        obj->unk7A = rotation;
        obj->unk78 = rotation;
        obj->unk76 = rotation;
    }
}

/*
 * Fills the four spline control points of `racer`: each node moved toward its edge partner
 * (a type-4 neighbour) by the racer's lateral position.
 */
void func_15088F30(Racer *racer, f32 *outX, f32 *outY, f32 *outZ) {
    s32 i;
    s32 j;
    u8 id;
    PathNode *node;
    PathNode *next;
    f32 dx;
    f32 dz;

    for (i = 0; i < 4; i++) {
        if (racer->nodes[i] != 0xFF) {
            node = &D_800D2350[racer->nodes[i]];
            next = node;
            for (j = 0; j < 5; j++) {
                id = node->links[j];
                if (id != 0xFF && D_800D2350[id].type == 4) {
                    next = &D_800D2350[id];
                    j = 5;
                }
            }
            dx = next->x - node->x;
            dz = next->z - node->z;
            outX[i] = node->x + (dx * racer->lateral);
            outY[i] = node->y;
            outZ[i] = node->z + (dz * racer->lateral);
        } else {
            outX[i] = i * 64;
            outY[i] = 0.0f;
            outZ[i] = 0.0f;
        }
    }
}

/*
 * Returns a neighbour of `node` (other than `exclude`) of the given type (0xFF = any),
 * preferring one whose param equals `param`; 0xFF if none.
 */
u8 func_1508907C(u8 node, u8 type, u8 exclude, u8 param) {
    s32 i;
    u8 result;
    PathNode *n;
    PathNode *t;

    result = 0xFF;
    if (node >= D_80087290) {
        return 0xFF;
    }
    n = &D_800D2350[node];
    for (i = 0; i < 5; i++) {
        if (n->links[i] != 0xFF && n->links[i] != exclude) {
            t = &D_800D2350[n->links[i]];
            if (t->type == type || type == 0xFF) {
                if (t->param == param) {
                    return n->links[i];
                }
                result = n->links[i];
            }
        }
    }
    return result;
}


void func_1512D560();

/* Race-over camera: follows the actor along the type-5 camera nodes, looking at it. */
void func_150891E8(Racer *racer, Actor *obj) {
    f32 dx;
    f32 dy;
    f32 dz;
    s32 nextNode;
    s32 camNext;
    s32 i;
    f32 distSq;
    u32 j;
    u8 pad; // matching: unused stack slot in the original frame
    f32 best;
    PathNode *entry;
    PathNode *other;
    u8 prevNode;
    u8 curNode;
    u8 start;
    u8 param;
    CamParams *p;
    Camera *cam;

    if (obj->camera == NULL) {
        return;
    }
    if (D_8008FDBC & 0x40) {
        param = 1;
    } else {
        param = 0;
    }
    cam = obj->camera;
    if (racer->state == 2) {
        for (i = 0; i < D_80087290; i++) {
            entry = &D_800D2350[i];
            if (entry->type == 5 && entry->param == 2) {
                racer->camNode = i;
                racer->camNextNode = i;
                for (j = 0; j < 5; j++) {
                    if (entry->links[j] != 0xFF) {
                        other = &D_800D2350[entry->links[j]];
                        if ((!D_800D23A8 && other->param == 3) || (D_800D23A8 && other->param != 3)) {
                            racer->camNextNode = entry->links[j];
                        }
                    }
                }
                i = D_80087290;
            }
        }
        curNode = racer->camNextNode;
        best = 1000000000.0f;
        prevNode = racer->camNode;
        start = curNode;
        do {
            entry = &D_800D2350[curNode];
            nextNode = func_1508907C(curNode, 0xFF, prevNode, param);
            prevNode = curNode;
            curNode = nextNode;
            dx = obj->x_position - entry->x;
            dz = obj->z_position - entry->z;
            distSq = dx * dx + dz * dz;
            if (distSq < best) {
                best = distSq;
                racer->camNode = prevNode;
                racer->camNextNode = nextNode;
            }
        } while (curNode != start);
        func_1512D560(obj->camera, 5, 0);
        racer->state = 3;
    }
    p = &racer->cam;
    camNext = racer->camNextNode;
    nextNode = racer->camNode;
    entry = &D_800D2350[nextNode];
    other = &D_800D2350[camNext];
    dx = obj->x_position - other->x;
    dz = obj->z_position - other->z;
    distSq = dx * dx + dz * dz;
    dx = obj->x_position - entry->x;
    dz = obj->z_position - entry->z;
    best = dx * dx + dz * dz;
    if (distSq < best) {
        racer->camNode = camNext;
        racer->camNextNode = func_1508907C(racer->camNode, 0xFF, nextNode, param);
        entry = other;
        dx = obj->x_position - entry->x;
        dz = obj->z_position - entry->z;
        best = distSq;
    }
    dy = obj->y_position - entry->y;
    best = sqrtf(dy * dy + best);
    dx /= best;
    dy /= best;
    dz /= best;
    if (best > 1000.0f) {
        best = 1000.0f;
    }
    p->unk0 = 0;
    p->atX = obj->x_position;
    p->atY = obj->y_position;
    p->atZ = obj->z_position;
    p->eyeX = obj->x_position - dx * best;
    p->eyeY = obj->y_position - dy * best;
    p->eyeZ = obj->z_position - dz * best;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    func_1512D560(obj->camera, 7, p);
    cam->unk19C = 30.0f;
    cam->unk1A4 = 30.0f;
    cam->unk1A0 = 30.0f;
    cam->unk1A8 = 30.0f;
}



/*
 * Pre-race fly-through camera along the type-6 camera nodes; `leave` hands the camera back
 * to the actor.
 */
void func_150896EC(Racer *racer, Actor *obj, s32 leave) {
    Camera *cam;
    s32 i;
    s32 active;
    f32 angle;
    u8 next;
    PathNode *entry;
    CamParams *p;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 lxs[4];
    f32 lys[4];
    f32 lzs[4];

    if (obj->camera == NULL) {
        return;
    }
    cam = obj->camera;
    p = &racer->cam;
    if (leave != 0) {
        p->unk10 = 120;
        p->unkC = 999.0f;
        func_1512D560(obj->camera, 10, p);
        func_1512D560(obj->camera, 6, cam->playerIndex);
        racer->camState = 0;
        return;
    }
    if (racer->camState == 0) {
        next = D_800D23A8 + 1;
        for (i = 0; i < D_80087290; i++) {
            entry = &D_800D2350[i];
            if (entry->type == 6 && entry->param == next) {
                racer->camNodes[0] = i;
                i = D_80087290;
            }
        }
        racer->camNodes[1] = func_1508907C(racer->camNodes[0], 6, 0xFF, 0);
        racer->camNodes[2] = func_1508907C(racer->camNodes[1], 6, racer->camNodes[0], 0);
        racer->camNodes[3] = func_1508907C(racer->camNodes[2], 6, racer->camNodes[1], 0);
        racer->camX = obj->x_position;
        racer->camY = obj->y_position;
        racer->camZ = obj->z_position;
        racer->camT = 0.0f;
        func_1512D560(obj->camera, 5, cam->playerIndex);
        racer->camState = 1;
    }
    racer->camT += 0.01700000092f * D_800BE9E4;
    if (racer->camT >= 1.0f) {
        if (racer->camNodes[2] == 0xFF) {
            racer->camT = 1.0f;
        } else {
            racer->camT -= 1.0f;
            racer->camNodes[0] = racer->camNodes[1];
            racer->camNodes[1] = racer->camNodes[2];
            racer->camNodes[2] = racer->camNodes[3];
            racer->camNodes[3] = func_1508907C(racer->camNodes[2], 6, racer->camNodes[1], 0);
        }
    }
    active = 0;
    for (i = 0; i < 4; i++) {
        if (racer->camNodes[i] != 0xFF) {
            func_15086CBC(racer->camNodes[i], &xs[i], &ys[i], &zs[i]);
            active = 1;
            func_15086CBC(func_1508907C(racer->camNodes[i], 4, 0xFF, 0), &lxs[i], &lys[i], &lzs[i]);
        } else if (active == 1) {
            active = 0;
            angle = obj->unk40 * 0.0174500000000000002;
            xs[i] = racer->camX - sinf(angle) * 500.0f;
            ys[i] = racer->camY + 180.0f;
            zs[i] = racer->camZ - cosf(angle) * 500.0f;
            lxs[i] = racer->camX;
            lys[i] = racer->camY + 120.0f;
            lzs[i] = racer->camZ;
        } else {
            xs[i] = xs[i - 1] + (xs[i - 1] - xs[i - 2]);
            ys[i] = ys[i - 1] + (ys[i - 1] - ys[i - 2]);
            zs[i] = zs[i - 1] + (zs[i - 1] - zs[i - 2]);
            lxs[i] = lxs[i - 1] + (lxs[i - 1] - lxs[i - 2]);
            lys[i] = lys[i - 1] + (lys[i - 1] - lys[i - 2]);
            lzs[i] = lzs[i - 1] + (lzs[i - 1] - lzs[i - 2]);
        }
    }
    angle = racer->camT;
    p = &racer->cam;
    p->eyeX = func_150497E0(xs, 0, angle);
    p->eyeY = func_150497E0(ys, 0, angle);
    p->eyeZ = func_150497E0(zs, 0, angle);
    p->atX = func_150497E0(lxs, 0, angle);
    p->atY = func_150497E0(lys, 0, angle);
    p->atZ = func_150497E0(lzs, 0, angle);
    p->unk0 = 0;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    func_1512D560(obj->camera, 7, p);
}

void func_15089BB0(void) {
    D_800D23B0 = NULL;
}

void *func_1509B570(s32);
void func_1508BC20(void);
extern f32 D_800872B0[], D_800872F8[];
extern s8 D_800872F0[];

/* Loads the multiplayer game resource D_800D2E48 and resets its per-player state. */
s32 func_15089BC0(void) {
    f32 *scales;
    s32 i;
    s32 id;
    void *resource;

    id = D_800D2E48;
    resource = func_1509B570(id);
    if (resource == NULL) {
        return 0;
    }
    // matching: identical separate case bodies, as in the original jump table
    switch (id) {
    case 0xB9:
        D_800D23B0 = resource;
        break;
    case 0xB8:
        D_800D23B0 = resource;
        break;
    case 0xBB:
        D_800D23B0 = resource;
        break;
    case 0xBA:
        D_800D23B0 = resource;
        break;
    case 0xB7:
        D_800D23B0 = resource;
        break;
    case 0xBD:
        D_800D23B0 = resource;
        break;
    case 0xBE:
        D_800D23B0 = resource;
        break;
    default:
        D_800D23B0 = NULL;
        return 0;
    }
    func_1508BC20();
    D_800D23B0->unk1700 = 0;
    D_800D23B0->unk1701 = 0;
    D_800D23B0->unk1703 = 0;
    D_800D23B0->unk1702 = 0;
    D_800D23B0->unk1704 = 0;
    D_800D23B0->gameType = id;
    D_800D23B0->unk16B8 = D_800872B0[D_800E0BD0];
    D_800D23B0->numZones = 0;
    scales = D_800872F8;
    for (i = 0; i < 16; i++) {
        D_800D23B0->unk16C0[i] = 0;
        D_800D23B0->aimRate[i] = scales[D_800E0BD0];
        D_800D23B0->aimX[i] = 0.0f;
        D_800D23B0->aimY[i] = 0.0f;
        D_800D23B0->aimZ[i] = 0.0f;
    }
    D_800D23B0->unkC = D_800872F0[D_800E0BD0];
    return 0;
}

/*
 * Picks the opponent `player` should go for: the one whose distance scores best on a sine
 * curve between a minimum and maximum range. Returns its index or -1.
 */
s32 func_15089F9C(s32 player) {
    f32 dx;
    f32 dz;
    f32 val;
    f32 dist;
    f32 minDist;
    f32 maxDist;
    f32 best;
    s32 mask;
    s32 count;
    s32 bit;
    s32 i;
    s32 bestIdx;
    Actor *other;
    Actor *self;
    s32 *states;
    s32 off;

    states = D_800D23B0->team;
    mask = D_800D23B0->activeMask;
    count = D_800D23B0->numPlayers;
    off = player * 4; // matching: dead store that IDO still schedules
    bestIdx = -1;
    if (D_800D23B0->unk15C[player] == 0) {
        minDist = 750.0f;
        maxDist = 3500.0f;
    } else {
        minDist = 1500.0f;
        maxDist = 8000.0f;
    }
    best = -1000000.0f;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if (states[i] != 3 && (mask & bit) &&
            (D_800D23B0->unk16C0[player] & (bit << 16))) {
            other = &D_800CC2D0[i];
            if (other != NULL) {
                self = &D_800CC2D0[player];
                dx = self->x_position - other->x_position;
                dz = self->z_position - other->z_position;
                dist = sqrtf((dx * dx) + (dz * dz));
            } else {
                dist = 32000.0f;
            }
            if (maxDist < dist) {
                dist = maxDist;
            }
            if (dist < minDist) {
                dist = minDist;
            }
            val = sinf(((dist - minDist) * (6.283185482f / (maxDist - minDist))) + -1.570796371f) + 1.0f;
            if (best < val) {
                best = val;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

/*
 * Advances to the next player (unk1704) and rebuilds its target list: the opposing players
 * sorted by a weighted distance (targets / targetDists rows).
 */
s32 func_1508A1BC(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 mask;
    s32 n;
    s32 dist;
    s32 myTeam;
    s32 cur;
    s32 myMask;
    s32 *active;
    s32 *layers;
    s32 *teams;
    u32 row;
    s32 count;
    s32 rawDist;
    s32 minDist;
    s32 *kinds;
    s32 *ids;
    s32 *dists;
    f32 dx;
    Actor *other;
    Actor *self;

    teams = D_800D23B0->team;
    mask = D_800D23B0->activeMask;
    count = D_800D23B0->numPlayers;
    kinds = D_800D23B0->unk15C;
    active = D_800D23B0->unk39C;
    layers = D_800D23B0->layer;
    ids = D_800D23B0->targets[0];
    dists = D_800D23B0->targetDists[0];
    D_800D23B0->unk1704++;
    if (D_800D23B0->unk1704 == D_800D23B0->numPlayers) {
        D_800D23B0->unk1704 = 0;
    }
    cur = D_800D23B0->unk1704;
    row = cur * 16;
    if (!((1 << cur) & mask)) {
        // matching: one-line loop (the -g3 line numbers steer the unrolled stores' order)
        for (i = 0; i < 16; i++) ids[row + i] = -1;
        return 0;
    }
    n = 0;
    minDist = 32000;
    myTeam = teams[cur];
    myMask = D_800D23B0->unk16C0[cur];
    for (i = 0; i < count; i++) {
        if (active[i] > 0 && myTeam != teams[i]) {
            other = &D_800CC2D0[i];
            self = &D_800CC2D0[cur];
            if (other != NULL) {
                dx = self->x_position - other->x_position;
                dist = sqrtf((dx * dx) + ((self->z_position - other->z_position) * (self->z_position - other->z_position)));
            } else {
                dist = 32000;
            }
            rawDist = dist;
            if (layers[cur] != layers[i]) {
                dist += 500;
            }
            if ((0x10000 << i) & myMask) {
                if (dist > 3000 && other->unk65 != 0) {
                    dist = -1;
                }
            } else {
                if (i >= D_8008FD90) {
                    dist *= 2;
                }
                if (D_800D23B0->gameType != 0xB8 && (kinds[i] == 2 || kinds[i] == 3)) {
                    dist = -1;
                }
            }
            if (D_800D23B0->gameType == 0xB7) {
                if (dist == -1) {
                    dist = rawDist;
                }
                if (other->unk13C != 0) {
                    if (i >= D_8008FD90) {
                        dist *= 2;
                    }
                    dist = dist / (func_150859AC(i, 6) + 2);
                }
            }
            if (i < D_8008FD90 && dist != -1) {
                dist *= D_800D23B0->unk16B8;
            }
            if (dist >= 0) {
                /* insertion sort into this player's row; matching: the row + index forms fix addu operand order */
                j = 0;
                while (j < n && (dists + row)[j] < dist) {
                    j++;
                }
                for (k = n; k > j; k--) {
                    ids[(s32)row + k] = ids[(s32)row + k - 1];
                    dists[(s32)row + k] = dists[(s32)row + k - 1];
                }
                ids[row + j] = i;
                n++;
                dists[row + j] = dist;
            }
            if (rawDist < minDist) {
                minDist = rawDist;
            }
        }
    }
    for (i = n; i < 16; i++) ids[row + i] = -1; // matching: one-line loop, as above
    if (D_800D23B0->nearestEnemyDist) {
        D_800D23B0->nearestEnemyDist[cur] = minDist;
    }
    return 0;
}

extern u8 D_800E0BB8[];
extern s8 D_800872D0[];
extern s16 D_800872C4[];
extern s8 D_800872E0[];
extern s8 D_800872E8[];
void func_15042D94(s32, s32, s32, char *, ...);
s32 func_1508CAD8(s32, s32, s32, s32, s32, s32);
void func_1508EC5C(s32, s32);
void func_1508EBF8(s32, s32);
s32 func_1508DAEC(s32, s32);
void func_1508B3F8(void);

/* Per-frame update of every player's multiplayer state; returns a mask of players that respawned. */
s32 func_1508A6FC(void) {
    s32 i;
    s32 bit;
    s32 isNew;
    s32 team;
    Actor *obj;
    s32 other;
    s32 type;
    s32 result;
    s32 pad1; // matching: unused stack slot in the original frame
    s32 old;
    s32 reset;
    Actor *tgt;
    s32 activeMask;
    s32 seenMask;
    s32 count;
    s32 *arr1C;
    s32 *teams;
    s32 *arr35C;
    s32 *arr31C;
    s32 *arr39C;
    s32 *arr19C;
    s32 *aimTimes;
    s32 *attackTimers;
    s32 *arr25C;
    s32 *arr41C;
    s32 *attackers;
    s32 *layers;
    s32 *teamScores;
    s32 *countdowns;
    u8 flag;
    s32 half;
    s32 j;
    s32 pad2; // matching: unused stack slot in the original frame
    s32 flags;
    RenderView *vp;

    result = 0;
    i = 0;
    activeMask = D_800D23B0->activeMask;
    seenMask = D_800D23B0->seenMask;
    count = D_800D23B0->numPlayers;
    teams = D_800D23B0->team;
    arr35C = D_800D23B0->unk35C;
    arr31C = D_800D23B0->unk31C;
    arr19C = D_800D23B0->unk19C;
    aimTimes = D_800D23B0->aimTime;
    attackTimers = D_800D23B0->attackTimer;
    arr25C = D_800D23B0->unk25C;
    arr41C = D_800D23B0->unk41C;
    attackers = D_800D23B0->attacker;
    teamScores = D_800D23B0->teamScore;
    countdowns = D_800D23B0->countdown;
    arr1C = D_800D23B0->unk1C;
    arr39C = D_800D23B0->unk39C;
    layers = D_800D23B0->layer;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        arr1C[i] = 0;
        isNew = 0;
        if (!(activeMask & bit)) {
            if (!(seenMask & bit)) {
                isNew = 1;
            }
            seenMask |= bit;
        }
        if ((activeMask & bit) || isNew) {
            obj = &D_800CC2D0[i];
            team = teams[i];
            if (team != 0 && D_800D23B0->gameType == 0xB9) {
                obj->unk1E4 = 1;
            }
            old = attackTimers[i];
            aimTimes[i] += D_800BE9E4;
            attackTimers[i] += D_800BE9E4;
            arr25C[i] -= D_800BE9E4;
            if (arr25C[i] <= 0) {
                arr25C[i] = 0;
            }
            arr41C[i] -= D_800BE9E4;
            if (arr41C[i] <= 0) {
                arr41C[i] = 0;
            }
            countdowns[i] -= D_800BE9E4;
            if (countdowns[i] < 0) {
                countdowns[i] = 0;
            }
            if (arr19C[i] == 3 && (arr25C[i] & 0x1F) >= 0x1D) {
                func_1508EC5C(i, 0x8000);
            }
            attackers[i] = D_800E0BB8[i];
            D_800E0BB8[i] &= 0x7F;
            if (D_800D23B0->grenades != NULL) {
                D_800D23B0->grenades[i] = obj->unk31C->grenade_count;
            }
            arr39C[i] = func_150859AC(i, 4);
            layers[i] = func_15085DA8(obj->y_position);
            if (arr39C[i] == 0 || isNew || obj->unk2FD != 0) {
                if (arr35C[i] == 0) {
                    if (obj->unk20F != 0xFF) {
                        D_800D23B0->unk59C[i] = obj->unk20F;
                    } else {
                        D_800D23B0->unk59C[i] = -1;
                        if (!attackers) { // matching: an extra use of attackers steers IDO's register allocation
                        }
                    }
                    arr35C[i] = 1;
                    arr25C[i] = 0;
                    arr19C[i] = 0;
                    D_800D23B0->unk5C[i] = 1;
                    D_800D23B0->target[i] = -1;
                    teamScores[team]--;
                    if ((attackers[i] & 0x7F) < count) {
                        other = teams[attackers[i] & 0x7F];
                        if (team != other) {
                            teamScores[other]++;
                        }
                    }
                    if (D_800D23B0->gameType != 0xB9) {
                        D_800D23B0->itemOverride[i] = 0;
                        func_1508DAEC(i, 1);
                    }
                    result |= bit;
                }
            } else {
                arr35C[i] = 0;
            }
            flag = 1;
            if (arr31C[i] != 0 && D_800D23B0->gameType != 0xB8) {
                reset = 0;
                if (arr19C[i] != 1 && aimTimes[i] < 30) {
                    type = obj->unk232;
                    obj->unk222 = arr31C[i] - 2;
                    if (type != 4 && type != 5) {
                        func_1508EBF8(i, 4);
                    }
                    if (type == 4) {
                        aimTimes[i] = 0;
                    }
                    if (type == 5) {
                        aimTimes[i] = 30;
                        D_800D23B0->aimRate[i] = D_800872F8[D_800E0BD0];
                        D_800D23B0->aimX[i] = (func_150ADA68() * 50.0f) + obj->x_position;
                        D_800D23B0->aimY[i] = obj->y_position + 40.0f;
                        D_800D23B0->aimZ[i] = (func_150ADA68() * 50.0f) + obj->z_position;
                        if ((arr31C[i] - 2) >= 2) {
                            tgt = &D_800CC2D0[arr31C[i] - 2];
                            D_800D23B0->aimX[i] += (tgt->x_position - obj->x_position) * 0.5f;
                            D_800D23B0->aimZ[i] += (tgt->z_position - obj->z_position) * 0.5f;
                        }
                    }
                }
                flag = 0;
                bit = 0;
                if (D_800872D0[D_800E0BD0] < attackTimers[i]) {
                    attackTimers[i] = 0;
                    old = -1;
                }
                if (attackTimers[i] >= 0 && old < 0) {
                    bit = 1;
                }
                if (D_800872C4[D_800E0BD0] != -1 && aimTimes[i] >= D_800872C4[D_800E0BD0]) {
                    reset = 1;
                }
                D_800D23B0->side[i] += (D_800D23B0->sideGoal[i] - D_800D23B0->side[i]) >> 2;
                D_800D23B0->drop[i] += (D_800D23B0->dropGoal[i] - D_800D23B0->drop[i]) >> 2;
                if (func_1508CAD8(i, arr31C[i] - 2, bit, reset, D_800D23B0->side[i], D_800D23B0->drop[i]) && arr19C[i] != 1) {
                    aimTimes[i] = 0;
                }
                if (attackTimers[i] == 0) {
                    D_800D23B0->sideGoal[i] = 0;
                    D_800D23B0->dropGoal[i] = 0;
                    if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
                        if (func_150ADA68() > 0.5f) {
                            D_800D23B0->sideGoal[i] = -50;
                        } else {
                            D_800D23B0->sideGoal[i] = 50;
                        }
                        if (func_150ADA68() > 0.5f) {
                            D_800D23B0->dropGoal[i] = 50;
                        } else {
                            D_800D23B0->dropGoal[i] = 0;
                        }
                    }
                }
            }
            if (flag && D_800D23B0->gameType != 0xB8) {
                attackTimers[i] = -D_800872E0[D_800E0BD0];
            }
            flags = obj->unk22C & 0x200;
            obj->unk22C &= ~0x200;
            if (flags || (attackers[i] & 0x80)) {
                arr19C[i] = 0;
                arr1C[i] = 1;
            }
            if (obj->unk31C->unk1AC != 0) {
                arr25C[i] = 20;
            }
        } else {
            arr39C[i] = 0;
            layers[i] = -1;
        }
    }

    D_800D23B0->seenMask = seenMask;
    if (D_800BE9E4 != 0) {
        D_800D23B0->ticks = D_800BE9E4;
    } else {
        D_800D23B0->ticks = 1.0f;
    }
    if (D_800D23B0->unk55C != NULL) {
        func_1508B3F8();
    }
    if (D_800D23B0->gameType == 0xB9) {
        half = (s32)(VIEWS[0].right - VIEWS[0].left) >> 1;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        for (j = 0; j <= D_80082FA0; j++) {
            if (D_800CC2D0[j].team == 0) {
                vp = &VIEWS[j];
                func_15042D94(vp->left + half, vp->top + 6.0f, 1, "%d", countdowns[j] / 60);
            }
        }

    }
    return result;
}

/* 12-byte per-player records at 0x70 of D_8008FDD4. */
typedef struct ScoreRecord {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ u8 pad2[0xA];
} ScoreRecord;

typedef struct ScoreTable {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ ScoreRecord records[4];
} ScoreTable;

extern ScoreTable *D_8008FDD4;

/* Returns player's score record value (0 for a non-human player). */
s32 func_1508B194(s32 player) {
    if (player >= D_8008FD90) {
        return 0;
    }

    return D_8008FDD4->records[player].unk0;
}

/* Clears player's score record value. */
void func_1508B1D4(s32 player) {
    if (player < D_8008FD90) {
        D_8008FDD4->records[player].unk0 = 0;
    }
}

/* Registers a zone of radius r around (x, y, z), up to 8. */
void func_1508B20C(f32 x, f32 y, f32 z, f32 r) {
    s32 n;

    if (D_800D23B0 != NULL && D_800D23B0->numZones < 8) {
        n = D_800D23B0->numZones++;
        D_800D23B0->zones[n].x = x;
        D_800D23B0->zones[n].y = y;
        D_800D23B0->zones[n].z = z;
        D_800D23B0->zones[n].radiusSq = r * r;
    }
}

/* Flood fill over the path graph from a point, collecting the nodes just outside a radius. */
typedef struct PathSearch {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
    /* 0x08 */ f32 radiusSq;
    /* 0x0C */ f32 dist[8];
    /* 0x2C */ s16 count;
    /* 0x2E */ u8 ids[8];
    /* 0x36 */ u8 visited[0x20]; /* bitset of visited node ids */
} PathSearch;

/*
 * Flood-fills the path graph from node `id`, marking visited nodes; nodes farther than
 * s->radiusSq from (s->x, s->z) end the walk and are collected (up to 8) with their squared distance.
 */
void func_1508B2A8(u8 id, PathSearch *s) {
    PathNode *node;
    f32 dx;
    f32 dist;
    s32 j;
    u8 link;

    /* BUG (original game): the bit index should be id & 7, so ids 8 apart share a visited bit */
    s->visited[id >> 3] |= 1 << (id & 3);
    node = &D_800D2350[id];
    dx = node->x - s->x;
    dist = node->z - s->z;
    dist = (dx * dx) + (dist * dist);
    if (s->radiusSq < dist) {
        if (s->count < 8) {
            s->ids[s->count] = id;
            s->dist[s->count] = dist;
            s->count++;
        }
    } else {
        for (j = 0; j < 5; j++) {
            if (node->links[j] != 0xFF) {
                link = node->links[j];
                if (!(s->visited[link >> 3] & (1 << (link & 3)))) {
                    func_1508B2A8(node->links[j], s);
                }
            }
        }
    }
}

/*
 * For each registered zone, assigns every computer player standing in it the nearest
 * path node reachable from the zone centre (unk55C), then clears the zones.
 */
void func_1508B3F8(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 k;
    u8 best;
    s32 startNode;
    s32 *zone;
    Actor *obj;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    s32 layer;
    f32 d;
    MpZone *zones;
    PathSearch s;
    f32 bestDist;
    PathNode *node;

    zone = D_800D23B0->unk55C;
    zones = D_800D23B0->zones;
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] != -1) {
            zone[i] = -2;
        }
    }
    for (i = 0; i < D_800D23B0->numZones; i++) {
        found = 0;
        s.radiusSq = zones[i].radiusSq;
        x = zones[i].x;
        y = zones[i].y;
        z = zones[i].z;
        for (j = D_8008FD90; j < D_8008FD8C; j++) {
            obj = &D_800CC2D0[j];
            d = obj->y_position - y;
            if (d < 200.0f && d > -100.0f) {
                dx = obj->x_position - x;
                d = obj->z_position - z;
                if (dx * dx + d * d < s.radiusSq + 100.0f) {
                    bestDist = 100000000.0f;
                    best = 0xFF;
                    if (!found) {
                        s.count = 0;
                        layer = func_15085DA8(y);
                        found = 1;
                        startNode = func_15085DF8(x, y, z, 0, layer);
                        if (startNode != -1) {
                            s.x = x;
                            s.z = z;
                            s.count = 0;
                            bzero(s.visited, 0x20);
                            func_1508B2A8(startNode, &s);
                        }
                    }
                    for (k = 0; k < s.count; k++) {
                        node = &D_800D2350[s.ids[k]];
                        dx = node->x - obj->x_position;
                        d = node->z - obj->z_position;
                        d = dx * dx + d * d;
                        if (d < s.dist[k] && d < bestDist) {
                            best = s.ids[k];
                            bestDist = d;
                        }
                    }
                    if (best != 0xFF) {
                        node = &D_800D2350[best];
                        if (zone[j] != -2) {
                            D_800D23B0->unk5C[j] = 1;
                        }
                        zone[j] = node->unk7;
                    }
                }
            }
        }
    }
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] < 0) {
            zone[i] = -1;
        }
    }
    D_800D23B0->numZones = 0;
}

s32 func_1508C5B8(s32, s32);
s32 func_1508A1BC(void);

/*
 * Picks the best opponent for player unk4: the active player with the lowest path cost to it,
 * scaled by weights[unk4][] / 255 (and 0.8 for its current target). Stores it in opponent and
 * the distance in opponentDist. Returns 0.
 */
s32 func_1508B9BC(void) {
    s32 mode;
    s32 count;
    s32 *targets;
    u8 *weights;
    s32 winner;
    s32 minimum;
    s32 index;
    s32 cost;
    f32 distance;
    s32 current;
    s32 end;
    s32 *active;
    s32 *neighbors;
    s32 *costs;
    s32 cursor;
    f32 value;

    mode = 1;
    count = D_800D23B0->numPlayers;
    current = D_800D23B0->unk4;
    active = D_800D23B0->unk39C;
    neighbors = D_800D23B0->targets[0];
    costs = D_800D23B0->targetDists[0];
    targets = D_800D23B0->target;
    if (count >= 9) {
        mode = 4;
    }
    func_1508C5B8(0, mode);
    func_1508A1BC();
    weights = D_800D23B0->weights[current];
    winner = -1;
    minimum = 10000000;
    for (index = 0; index < count; index++) {
        if (active[index] > 0) {
            cost = -1;
            if (index != current) {
                cursor = index * 16;
                end = cursor + 16;
                while (cursor < end && neighbors[cursor] != -1) {
                    if (neighbors[cursor] == current) {
                        cost = costs[cursor];
                        cursor = end;
                    } else {
                        cursor++;
                    }
                }
                if (cost != -1) {
                    value = weights[index] * (1.0f / 255.0f);
                    if (index == targets[current]) {
                        value *= 0.8f;
                    }
                    cost = cost * value;
                    if (cost < minimum) {
                        minimum = cost;
                        winner = index;
                    }
                }
            }
        }
    }
    D_800D23B0->opponent[current] = winner;
    if (winner != -1) {
        f32 dx;
        f32 dz;
        Actor *actor;
        Actor *other;

        actor = &D_800CC2D0[current];
        other = &D_800CC2D0[winner];
        dx = actor->x_position - other->x_position;
        dz = actor->z_position - other->z_position;
        distance = sqrtf(dx * dx + dz * dz);
        D_800D23B0->opponentDist[current] = (s32)distance;
    } else {
        D_800D23B0->opponentDist[current] = minimum;
    }
    return 0;
}

/*
 * Fills each player's row of weights with a random triangle wave between 0x55 and 0xFF,
 * starting from a random unused column.
 */
void func_1508BC20(void) {
    s32 i;
    f32 fn;
    s32 pos;
    s32 val;
    s32 dir;
    s32 delta;
    s32 step;
    s32 h;
    u8 *row;
    s8 n;
    s8 used[16];

    n = D_8008FD8C;
    row = D_800D23B0->weights[0];
    h = n >> 1;
    if (h < 2) {
        h = 2;
    }
    step = 0xAA / h;
    fn = n;
    for (i = 0; i < n; i++) {
        for (h = 0; h < 16; h++) used[h] = 0;
        dir = -1;
        val = 0x55;
        delta = step;
        pos = func_150ADA68() * fn;
        if (func_150ADA68() > 0.5f) {
            dir = 1;
        }
        while (used[pos] != 0) {
            pos += dir;
            if (pos < 0) {
                pos = n - 1;
            }
            if (pos >= n) {
                pos = 0;
            }
        }
        used[pos] = 1;
        for (h = 0; h < n; h++) {
            if (val < 0x100) {
                row[pos] = val;
            } else {
                row[pos] = 0xFF;
            }
            pos++;
            if (pos >= n) {
                pos = 0;
            }
            val += delta;
            if (val >= 0x100) {
                delta = -delta;
            }
        }
        row += 0x10;
    }
}

/*
 * For every player on unk4's team, picks the reachable opponent with the lowest target
 * distance, doubling the cost per teammate already on it (kinds 2/3 choose first).
 */
s32 func_1508BF14(void) {
    s32 numPlayers;
    s32 myTeam;
    s32 pass;
    s32 myIdx;
    s32 i;
    s32 j;
    s32 end;
    s32 cost;
    s32 bestCost;
    s8 counts[16];
    s32 *teams;
    s32 *links;
    s32 *costs;
    s32 *kinds;
    s32 *active;
    s32 *bestCosts;
    s32 *bestSlots;
    s32 bestSlot;
    s32 n;

    myIdx = D_800D23B0->unk4;
    numPlayers = D_800D23B0->numPlayers;
    kinds = D_800D23B0->unk15C;
    active = D_800D23B0->unk39C;
    teams = D_800D23B0->team;
    bestCosts = D_800D23B0->opponentDist;
    bestSlots = D_800D23B0->opponent;
    links = D_800D23B0->targets[0];
    costs = D_800D23B0->targetDists[0];
    myTeam = teams[myIdx];

    for (i = 0; i < numPlayers; i++) counts[i] = 0;

    for (pass = 0; pass != 2; pass++) {
        for (i = 0; i < numPlayers; i++) {
            n = 0;
            if (kinds[i] == 2 || kinds[i] == 3) {
                n = 1;
            }
            if ((n && pass == 0) || (!n && pass != 0)) {
                if (teams[i] == myTeam) {
                    bestSlot = -1;
                    bestCost = 10000000;
                    for (j = 0; j < numPlayers; j++) {
                        if (active[j] > 0) {
                            if (teams[j] != myTeam) {
                                cost = -1;
                                n = j * 16;
                                end = n + 16;
                                while (n < end && links[n] != -1) {
                                    if (links[n] == i) {
                                        cost = costs[n];
                                        n = end;
                                    } else {
                                        n++;
                                    }
                                }
                                if (cost != -1) {
                                    cost <<= counts[j];
                                    if (cost < bestCost) {
                                        bestCost = cost;
                                        bestSlot = j;
                                    }
                                }
                            }
                        }
                    }
                    counts[bestSlot]++;
                    bestCosts[i] = bestCost;
                    bestSlots[i] = bestSlot;
                }
            }
        }
    }
    return 0;
}

s32 func_1508C194(s32 player) {
    return 0;
}

Actor *func_1505EEF4(s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *,
                  f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);

/* Line-of-sight test between players a and b; sets their visibility bits in unk16C0. */
void func_1508C1A4(s32 a, s32 b) {
    s32 blocked;
    s32 seen;
    Actor *from;
    Actor *to;
    s32 fromFlag;
    s32 toFlag;
    ActorCtl *fromInner;
    f32 distance;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 hitDistance;
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    f32 z0;
    f32 z1;

    seen = 0;
    blocked = 0;
    from = func_1505EEF4(a & 0xFFFF0FFF);
    to = func_1505EEF4(b & 0xFFFF0FFF);
    if (from != NULL && to != NULL) {
        x0 = from->x_position;
        z0 = from->z_position;
        fromInner = from->unk31C;
        fromFlag = fromInner->unk84;
        if (fromFlag) {
            y0 = from->y_position + fromInner->height * 0.75f;
            if ((fromInner->unk75 & 0x7F) == 9) {
                y0 -= 50.0f;
            }
        } else if (from->interaction_state == 0x1F) {
            y0 = from->y_position + 80.0f;
        } else if (to->unk31C->unk84 && D_800872C4[D_800E0BD0] != -1) {
            /* 0x1A4..0x1A9 is an s16 point; structs.h types it as u16 pad1A4/s16 unk1A6/u32 pad1A8 */
            x0 = from->aim.x;
            y0 = from->aim.y;
            z0 = from->aim.z;
        } else {
            y0 = from->y_position + 40.0f;
        }
        x1 = to->x_position;
        z1 = to->z_position;
        toFlag = to->unk31C->unk84;
        if (toFlag) {
            y1 = to->y_position + to->unk31C->height * 0.75f;
            if ((to->unk31C->unk75 & 0x7F) == 9) {
                y1 -= 50.0f;
            }
        } else if (to->interaction_state == 0x1F) {
            y1 = to->y_position + 80.0f;
        } else if (fromFlag && D_800872C4[D_800E0BD0] != -1) {
            x1 = to->aim.x;
            y1 = to->aim.y;
            z1 = to->aim.z;
        } else {
            y1 = to->y_position + 40.0f;
        }
        distance = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) +
                         (z1 - z0) * (z1 - z0));
        if (func_150AC9C0(x0, y0, z0, x1 - x0, y1 - y0, z1 - z0, NULL, NULL, &hitX,
                          &hitY, &hitZ, &hitDistance, NULL, NULL, 0.0f) == 0) {
            blocked = 1;
        } else if (distance < hitDistance) {
            blocked = 1;
        }
        seen = blocked;
        if (blocked && func_15086D94(x0, y0, z0, x1 - x0, z1 - z0) > 0.0f) {
            blocked = 0;
        }
    }
    if (blocked) {
        D_800D23B0->unk16C0[a] |= 1 << b;
        D_800D23B0->unk16C0[b] |= 1 << a;
    }
    if (seen) {
        D_800D23B0->unk16C0[a] |= 0x10000 << b;
        D_800D23B0->unk16C0[b] |= 0x10000 << a;
    }
}

/*
 * Refreshes the line-of-sight bits (unk16C0) between players: if `checks` is nonzero,
 * each human against its three nearest opponents, then `checks` round-robin pairs.
 * Returns player's bits.
 */
s32 func_1508C5B8(s32 player, s32 checks) {
    u8 cur;
    u8 other;
    s32 i;
    s32 m;
    s32 j;
    s32 k;
    f32 x0;
    f32 x1;
    f32 dx;
    f32 dz;
    u8 team;
    Actor *obj;
    s8 idx[4];
    f32 dist[4];
    s32 *list;
    s32 mask;

    cur = D_800D23B0->unk1700;
    other = D_800D23B0->unk1701;
    list = D_800D23B0->team;
    if (checks != 0) {
        for (i = 0; i < D_8008FD90; i++) {
            x0 = D_800CC2D0[i].x_position;
            x1 = D_800CC2D0[i].x_position;
            for (k = 0; k < 3; k++) {
                idx[k] = -1;
                dist[k] = 900000000.0f;
            }
            for (k = D_8008FD90; k < D_8008FD8C; k++) {
                if (list[i] != list[k] &&
                    !(D_800D23B0->unk16C0[i] & (0x10001 << k))) {
                    /* BUG (original game): measures player i against itself, not k */
                    dx = x0 - D_800CC2D0[i].x_position;
                    dz = x1 - D_800CC2D0[i].z_position;
                    if (i == D_800D23B0->target[k]) {
                        dx *= 0.5f;
                        dz *= 0.5f;
                    }
                    dx = dx * dx + dz * dz;
                    j = 2;
                    while (j >= 0 && dx < dist[j]) {
                        dist[j + 1] = dist[j];
                        idx[j + 1] = idx[j];
                        idx[j] = k;
                        dist[j] = dx;
                        j--;
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                if (idx[k] != -1) {
                    j = idx[k];
                    D_800D23B0->unk16C0[i] &= ~(0x10001 << j);
                    D_800D23B0->unk16C0[j] &= ~(0x10001 << i);
                    func_1508C1A4(i, j);
                }
            }
        }
    }
    if (checks != 0) {
        obj = &D_800CC2D0[cur];
        do {
            team = obj->team;
            m = 0;
            do {
                other++;
                if (other >= D_8008FD8C) {
                    cur++;
                    if (cur >= D_8008FD8C - 1) {
                        cur = 0;
                    }
                    obj = &D_800CC2D0[cur];
                    team = obj->team;
                    other = cur + 1;
                }
                D_800D23B0->unk16C0[cur] &= ~(0x10001 << other);
                D_800D23B0->unk16C0[other] &= ~(0x10001 << cur);
                mask = (1 << cur) | (1 << other);
                m++;
            } while (m < 64 && ((D_800D18A0 & mask) ||
                                    team == D_800CC2D0[other].team));
            func_1508C1A4(cur, other);
        } while (--checks != 0);
    }
    D_800D23B0->unk1700 = cur;
    D_800D23B0->unk1701 = other;
    return D_800D23B0->unk16C0[player];
}

/*
 * Advances unk1702 round-robin over the D_8008FD8C actors; returns the next active one with
 * unk31C->unk84 set that isn't in D_800D18A0, or -1.
 */
s32 func_1508C9CC(void) {
    s32 count;
    s32 ret;
    s8 cur;
    Actor *obj;
    ActorCtl *state;

    count = 0;
    ret = -1;
    cur = D_800D23B0->unk1702;
    do {
        cur++;
        if (cur >= D_8008FD8C) {
            cur = 0;
        }
        obj = &D_800CC2D0[cur];
        if (obj->interaction_state != 0) {
            state = obj->unk31C;
            if (state != NULL && state->unk84 != 0 && !(D_800D18A0 & (1 << cur))) {
                count = D_8008FD8C;
                ret = cur;
            }
        }
        count++;
    } while (count < D_8008FD8C);

    D_800D23B0->unk1702 = cur;
    return ret;
}

/* Advances unk1703 to the next human player and returns it. */
s32 func_1508CA88(void) {
    MpGame **p2; // matching: reading back through a pointer to D_800D23B0
    s8 temp;

    p2 = &D_800D23B0;
    D_800D23B0->unk1703++;
    temp = D_800D23B0->unk1703;
    if (temp >= D_8008FD90) {
        D_800D23B0->unk1703 = 0;
        temp = (*p2)->unk1703;
    }
    return temp;
}


extern s8 D_800872D8[];
void func_150DA5EC(Actor *, f32);



/*
 * Steers bot `player` toward player `target`: eases the smoothed aim point (aimX/aimY/aimZ)
 * toward the target (its position, or its 0x1A4 point if aimAtPoint), offset sideways by
 * sideOffset and down by dropOffset, turns the actor's yaw/pitch (or the camera pad's stick)
 * to it and presses Z_TRIG/R_TRIG when it should fire. Returns 1 while the yaw error exceeds 45 degrees.
 */
s32 func_1508CAD8(s32 player, s32 target, s32 fire, s32 aimAtPoint, s32 sideOffset, s32 dropOffset) {
    Actor *victim;
    f32 dz;
    f32 dx;
    f32 dy;
    f32 V;
    f32 scale;
    f32 ang;
    f32 curYaw;
    f32 diff;
    f32 roll;
    f32 h;
    f32 dist;
    f32 mag;
    f32 yaw;
    f32 *ptr;
    f32 ratio;
    f32 height;
    s32 flagA;
    s32 doInner;
    s32 mode;
    f32 cur;
    Actor *obj;
    s32 type;
    s32 hasCam;
    s32 result;
    Camera *cam;
    s32 pad; // matching: unused stack slot in the original frame

    doInner = 1;
    result = 0;
    obj = &D_800CC2D0[player];
    ratio = VIEWS[0].fovY / VIEWS[0].fovX;
    if (target != -1) {
        cam = obj->camera;
        if (cam != NULL) {
            hasCam = 1;
        } else {
            hasCam = 0;
        }
        type = obj->unk31C->unk78;
        if (type == 9 || type == 0x38 || type == 0x15) {
            hasCam = 0;
        }
        victim = func_1505EEF4(target & 0xFFFF0FFF);
        if (victim == NULL) {
            return 1;
        }
        flagA = 0;
        mode = 0;
        if (type == 0x24 || type == 0x18 || type == 0x23 || type == 0x41) {
            flagA = 1;
        } else if (type == 9) {
            mode = 1;
        } else if (type == 0x15 || type == 0x1B || type == 3) {
            mode = 1;
            if (type != 0x1B) {
                mode = 3;
            }
            doInner = 0;
        } else if (type == 0x38) {
            mode = 2;
        }
        if (type == 3) {
            aimAtPoint = 0;
        }
        height = 40.0f;
        if (victim->interaction_state == 0x1F) {
            aimAtPoint = 0;
            height = 80.0f;
        }
        dx = 0.0f;
        dz = 0.0f;
        if (D_800D23B0->gameType == 0xB9 || D_800E0BD0 >= 3) {
            switch (obj->unk31C->unk78) {
                case 0x18:
                case 0x41:
                    V = 0.01600000076f;
                    break;
                case 0x14:
                case 0x3F:
                    V = 0.02476780117f;
                    break;
                case 0x1B:
                    V = 0.01600000076f;
                    break;
                case 0x16:
                    V = 0.03200000152f;
                    break;
                default:
                    V = 0.0f;
                    break;
            }
            if (V != 0.0f) {
                dx = victim->x_position - obj->x_position;
                dz = victim->z_position - obj->z_position;
                dy = victim->y_position - obj->y_position;
                dy = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
                V = (dy * V) / D_800D23B0->ticks;
                dx = (victim->x_position - victim->old_x_position) * V;
                dz = (victim->z_position - victim->old_z_position) * V;
            }
        }
        if (aimAtPoint == 0) {
            dx = (victim->x_position + dx) - D_800D23B0->aimX[player];
            dz = (victim->z_position + dz) - D_800D23B0->aimZ[player];
        } else {
            dx = ((f32)victim->aim.x + dx) - D_800D23B0->aimX[player];
            dz = ((f32)victim->aim.z + dz) - D_800D23B0->aimZ[player];
        }
        scale = D_800D23B0->aimRate[player] * D_800BE9E4;
        if (type == 3) {
            scale = 1.0f;
            dropOffset = 0;
            sideOffset = 0;
        }
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        D_800D23B0->aimX[player] += dx * scale;
        D_800D23B0->aimZ[player] += dz * scale;
        dx = D_800D23B0->aimX[player] - obj->x_position;
        dz = D_800D23B0->aimZ[player] - obj->z_position;
        dist = sqrtf((dx * dx) + (dz * dz));
        if (dist < 1.0f) {
            dist = 1.0f;
        } else {
            f32 tx;

            if ((dist < 100.0f || type == 3) && dropOffset != 0) {
                dropOffset = 0;
                if (sideOffset == 0) {
                    sideOffset = 50;
                }
            }
            mag = sideOffset;
            if (type == 9) {
                mag = sideOffset * 1.25f;
            }
            mag = mag / dist;
            tx = dx;
            dx -= dz * mag;
            dz += tx * mag;
        }
        ang = func_1505A630(dx, -dz, 0) * 0.005493164f;
        if (type != 3) {
            curYaw = obj->unk31C->aimYaw;
        } else {
            curYaw = obj->unk40 - 90.0f;
        }
        ang = func_15144BC8(ang);
        curYaw = func_15144BC8(curYaw);
        diff = func_15144BC8(curYaw - ang);
        if (diff > 180.0f) {
            diff = -360.0f + diff;
        }
        if (diff > 45.0f || diff < -45.0f) {
            result = 1;
        }
        if (aimAtPoint == 0) {
            cur = ((victim->y_position + height) - dropOffset) - D_800D23B0->aimY[player];
        } else {
            cur = ((f32)victim->aim.y - (f32)dropOffset) - D_800D23B0->aimY[player];
        }
        D_800D23B0->aimY[player] += cur * scale;
        cur = obj->unk31C->aimEye.y - D_800D23B0->aimY[player];
        ang = func_1505A630(dist, cur, 0) * 0.005493164f;
        curYaw = obj->unk31C->aimPitch;
        while (ang > 180.0f) {
            ang -= 360.0f;
        }
        while (curYaw > 180.0f) {
            curYaw -= 360.0f;
        }
        V = func_15144BC8(curYaw - ang);
        while (V > 180.0f) {
            V -= 360.0f;
        }
        yaw = diff * 8.0f;
        roll = V * 8.0f;
        if (yaw < -90.0f) {
            yaw = -90.0f;
        }
        if (yaw > 90.0f) {
            yaw = 90.0f;
        }
        if (roll < -30.0f) {
            roll = -30.0f;
        }
        if (roll > 30.0f) {
            roll = 30.0f;
        }
        V = sqrtf((yaw * yaw) + (roll * roll)) * 1.5f;
        h = V - 20.0f;
        if (V < 20.0f) {
            h = 0.0f;
        }
        h = -80.0f + h;
        if (type == 3) {
            h = 35.0f;
            roll = 0.0f;
        }
        if (doInner != 0) {
            ptr = &obj->unk31C->unk90;
            obj->unk31C->aimYaw -= diff;
            obj->unk31C->aimPitch = ang;
            func_15048758(&obj->unk31C->aimYaw);
            obj->unk31C->aimYawTarget = obj->unk31C->aimYaw;
            obj->unk31C->aimPitchTarget = obj->unk31C->aimPitch;
            obj->unk31C->stick_x = 0;
            obj->unk31C->stick_y = 0;
        } else if (hasCam != 0) {
            ptr = &cam->unk1A4;
            ((OSContPad *)cam->contPad)->stick_x = (s32)yaw;
            ((OSContPad *)cam->contPad)->stick_y = (s32)-roll;
        } else {
            ptr = &obj->unk31C->unk90;
            obj->unk31C->stick_x = (s32)yaw;
            obj->unk31C->stick_y = (s32)-roll;
        }
        if (mode != 0) {
            if (type == 0x1B) {
                if (h < -50.0f && (D_800D23B0->unk16C0[player] & (0x10001 << target))) {
                    obj->unk31C->buttons |= Z_TRIG;
                    obj->unk31C->unk8E = 2;
                }
            } else if (type == 3) {
                f32 f;

                f = (dist - 136.0f) / 664.0f;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                f = (sinf((D_800BE9AC & 0x3F) * 0.09815625101f) * 0.174999997f) + f;
                if (f > 1.0f) {
                    f = 1.0f;
                }
                f = (f * 0.02569000237f) + 0.01724099927f;
                obj->unk31C->stick_y = 0;
                obj->unk31C->stick_x = 0;
                obj->unk76 -= (s32)(yaw * 16.0f);
                obj->unk7A = obj->unk76;
                obj->unk40 = (f32)(s16)(obj->unk7A + 0x4000) * 0.005493164f;
                func_150DA5EC(obj, f);
            } else if (D_800D23B0->attackTimer[player] >= 0) {
                obj->unk31C->buttons = Z_TRIG;
                if (mode == 1) {
                    obj->unk31C->unk8E = 2;
                    if (D_800872D8[D_800E0BD0] < D_800D23B0->attackTimer[player]) {
                        obj->unk31C->buttons &= ~Z_TRIG;
                        obj->unk31C->unk8E = 0;
                    }
                } else if (mode == 2) {
                    if (fire != 0) {
                        obj->unk31C->unk8E = 0xA;
                    } else {
                        obj->unk31C->buttons &= ~Z_TRIG;
                    }
                } else {
                    obj->unk31C->unk8E = 2;
                }
            }
        } else if (fire != 0) {
            if (flagA != 0) {
                obj->unk31C->unk8C = 0;
                obj->unk31C->buttons = 0;
            } else {
                obj->unk31C->unk8C = Z_TRIG;
                obj->unk31C->unk8F = 1;
            }
        } else if (flagA != 0) {
            if (cam != NULL) {
                cam->contPad->button |= Z_TRIG;
            }
            obj->unk31C->buttons = Z_TRIG;
            obj->unk31C->unk8E = 2;
        }
        obj->unk31C->buttons |= R_TRIG;
        obj->unk31C->unk8E = 1;
        *ptr += (h - *ptr) * 0.125f;
        if (hasCam != 0) {
            cam->unk1A8 = cam->unk1A4 * ratio;
        }
        if (cam != NULL) {
            cam->contPad->button |= R_TRIG;
        }
    }
    return result;
}

extern s16 D_80087320[][3];
void func_1508EB90(s32 arg0, s32 arg1, s32 arg2);
extern f32 D_8009DA8C;

/*
 * Copies player's target to its state (unk129). Once its attackTimer passes the limit for
 * this level and distance it is reset, with func_1508EB90(player, 0x61, 0x2000) if the target
 * is in sight; while the timer is 0, unk12A is set at random.
 */
s32 func_1508D850(s32 player) {
    s8 *timers;
    s8 *targets;
    s32 *target;
    f32 dx;
    Actor *obj;
    f32 dz;
    f32 dist;
    s32 range;
    s32 limit;
    s32 off;

    // matching: byte pointers plus a shared byte offset, as IDO keeps player * 4 in one register
    targets = (s8 *)D_800D23B0->target;
    timers = (s8 *)D_800D23B0->attackTimer;
    obj = &D_800CC2D0[player];
    off = player * 4;
    target = (s32 *)(targets + off);
    obj->unk31C->unk129 = *target;
    if (*target < 0) {
        return 0;
    }
    dx = D_800CC2D0[*target].x_position - obj->x_position;
    dz = D_800CC2D0[*target].z_position - obj->z_position;
    dist = sqrtf((dx * dx) + (dz * dz));
    if (5000.0f < dist) {
        range = 0;
    } else if (dist > 2000.0f) {
        range = 1;
    } else {
        range = 2;
    }
    limit = ((s16 *)D_80087320)[D_800E0BD0 * 3 + range]; // matching: the [level][range] form computes the address differently
    if (limit < *(s32 *)(timers + off)) {
        *(s32 *)(timers + off) = 0;
        if (*(s32 *)((s8 *)D_800D23B0->unk16C0 + off) & (0x10000 << *target)) {
            func_1508EB90(player, 0x61, 0x2000);
        }
    }
    if (*(s32 *)(timers + off) == 0) {
        obj->unk31C->unk12A = 0;
        if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
            obj->unk31C->unk12A = 1;
        }
    }
}

/* Resets the pickup slots: nobody holds one and each respawns on the next update. */
s32 func_1508DA1C(void) {
    s32 i;
    s32 *slots = D_800D23B0->slot;
    s32 *owners = D_800D23B0->slotOwner;
    s32 *overrides = D_800D23B0->itemOverride;
    s32 *timers = D_800D23B0->slotTimer;
    s32 *spawns = D_800D23B0->slotSpawn;
    s32 *dropTimers = D_800D23B0->slotDropTimer;

    for (i = 0; i < D_800D23B0->numPlayers; i++) {
        slots[i] = -1;
    }
    for (i = 0; i < D_800D23B0->numSlots; i++) {
        owners[i] = -1;
        overrides[i] = 0;
        spawns[i] = -1;
        timers[i] = 1;
        dropTimers[i] = 0;
    }
}

/*
 * Releases the slot held by `player` (returns 1 without doing so if `mode` < 0). Unless the
 * slot is flagged 2, it is reset; mode 1 also drops it at the player's position.
 */
s32 func_1508DAEC(s32 player, s32 mode) {
    MpGame *game;
    s32 slot;
    s32 next;
    Actor *obj;

    game = D_800D23B0;
    if (game == NULL) {
        return 0;
    }
    slot = game->slot[player];
    if (slot >= 0) {
        if (mode < 0) {
            return 1;
        }
        game->slot[player] = -1;
        obj = &D_800CC2D0[player];
        if ((D_800D23B0->slotFlags[slot] & 2) == 0) {
            D_800D23B0->slotOwner[slot] = -1;
            D_800D23B0->slotSpawn[slot] = -1;
            next = D_800D23B0->respawnDelay;
            if (mode == 1) {
                D_800D23B0->slotDropTimer[slot] = 300;
                obj = &D_800CC2D0[player];
                next = 4;
                D_800D23B0->slotDropX[slot] = obj->x_position;
                D_800D23B0->slotDropY[slot] = obj->y_position + 50.0f;
                D_800D23B0->slotDropZ[slot] = obj->z_position;
            }
            D_800D23B0->slotTimer[slot] = next;
        }
    }
    return 0;
}


void func_1502EA0C(Actor *, s32, s32, s32, s32, s32);
s32 func_1508E780(s32 arg0);
void func_1502EA50(Actor *);
void func_15080620(s32, s32, s32, s32);
void func_151027E8(Actor *);
WorldObject *func_151149AC(u8);
void func_15030158(ModelAttachment *, s32);
s32 func_15033E28(Actor *, ModelAttachment **);


/*
 * Per-frame pickup update. Gives each player the item of its override, its slot or the default
 * (swapping attachments when it changes), then moves the slot pickups: drop timers, respawns at
 * a spawn node, and handing a touched pickup to its player (0x8AE adds 3 grenades, up to 6).
 */
void func_1508DC24(void) {
    static s8 D_80087340;
    MpGame *base;
    s32 i;
    Actor *obj; // matching: unused stack slot in the original frame
    s32 count;
    s32 cur;
    s32 prev;
    s32 setId;
    s32 numAttached;
    s32 j;
    f32 x;
    f32 y;
    f32 z;
    s32 found;
    u16 pp;
    s32 kind;
    s32 *slots;
    s32 *entities;
    s32 *owners;
    s32 *overrides;
    s32 *timers;
    s32 *spawns;
    s32 *items;
    s32 *flags;
    s32 *homeSpawns;
    s32 *dropTimers;
    s32 *dropX;
    s32 *dropY;
    s32 *dropZ;
    ModelAttachment *attached[10];
    Actor *other;
    WorldObject *pickup;
    s32 idx;
    s32 r;
    s32 k;

    base = D_800D23B0;
    i = 0; // matching: zeroed here, before the field loads, not in the for
    slots = base->slot;
    entities = base->slotEntity;
    owners = base->slotOwner;
    overrides = base->itemOverride;
    timers = base->slotTimer;
    spawns = base->slotSpawn;
    items = base->slotItem;
    flags = base->slotFlags;
    homeSpawns = base->slotHomeSpawn;
    dropTimers = base->slotDropTimer;
    dropX = base->slotDropX;
    dropY = base->slotDropY;
    dropZ = base->slotDropZ;
    count = base->numPlayers;
    for (; i < count; i++) {
        kind = base->gameType;
        if (kind == 0xB9 && D_800CC2D0[i].team != 0) {
            continue;
        }
        prev = D_800CC2D0[i].unk31C->unk75 & 0x7F;
        cur = overrides[i];
        if (D_800CC2D0[i].unk31C->unk19B >= 5) {
            cur = 0x3A;
        } else if (D_800CC2D0[i].unk31C->unk19B > 0) {
            cur = 0x26;
        }
        if (cur == 0) {
            if (slots[i] != -1) {
                cur = items[slots[i]];
            } else {
                cur = base->defaultItem;
            }
        } else if (cur == 0x457) {
            cur = 0;
        }
        if (kind == 0xB8) {
            prev = D_800CC2D0[i].unk31C->unk128 & ~0x20;
        }
        if (cur != prev) {
            setId = func_1508E780(prev);
            if (setId != -1) {
                setId &= ~0x1000;
                func_1508EB90(i, 0x4C, setId);
                if (setId == 0x1D) {
                    func_1508EB90(i, 0x4C, 0x1E);
                }
                D_800CC2D0[i].unk31C->unk11A = 0;
            }
            setId = func_1508E780(cur);
            if (setId > 0x1000) {
                setId &= ~0x1000;
                func_1508EB90(i, 0x4B, setId);
            }

            if (D_800D23B0->gameType == 0xB8) {
                if (prev == 0x80) {
                    func_1502EA50(&D_800CC2D0[i]);
                    D_800CC2D0[i].immune = 0;
                }
                D_800CC2D0[i].unk31C->unk128 = cur;
                if (cur == 1) {
                    D_800CC2D0[i].unk31C->unk12C = 600;
                } else if (cur == 0x80) {
                    D_800CC2D0[i].unk31C->unk12C = 1800;
                    func_1502EA0C(&D_800CC2D0[i], 0, 0, 0xFF, 0xBF, 4);
                } else if (cur == 0x10) {
                    D_800CC2D0[i].unk31C->unk12C = 3;
                }
            } else {
                func_15080620(i, 1, cur, 0);
                if (cur == 0 && prev != 3) {
                    D_800CC2D0[i].unk31C->unk78 = 0;
                }
                if (prev == 0x20) {
                    func_1508EB90(i, 0x45, 4);
                }
                if (cur == 0x20) {
                    func_1508EB90(i, 0x45, 3);
                }
            }
        }
        base = D_800D23B0;
        if (D_80087340 == i && base->gameType != 0xB8) {
            setId = func_1508E780(cur);
            found = 0;
            numAttached = func_15033E28(&D_800CC2D0[i], attached);
            if (numAttached == 0 && D_800CC2D0[i].unk31C->unk11A == 2 && setId != 0x55 && setId != 0x2E) {
                func_15083568(&D_800CC2D0[i], setId, 1.0f, 0);
                if (setId == 0x1D) {
                    func_15083568(&D_800CC2D0[i], 0x1E, 1.0f, 0);
                }
            }
            if (numAttached != 0 && cur != 0x20) {
                if (setId != -1) {
                    setId &= ~0x1000;
                    for (j = 0; j < numAttached; j++) {
                        if ((attached[j]->setId != 0x1E || setId != 0x1D) && setId != attached[j]->setId &&
                            attached[j]->setId != 0x2A && attached[j]->setId != 0x22 &&
                            attached[j]->setId != 0x2D && attached[j]->setId != 0x72 &&
                            attached[j]->setId != 0x73 && attached[j]->setId != 0x76 &&
                            attached[j]->setId != 0x77 && attached[j]->setId != 0x78 && attached[j]->setId != 7) {
                            func_15030158(attached[j], 0);
                            D_800CC2D0[i].unk31C->unk11A = 0;
                        } else if (setId == attached[j]->setId) {
                            found = 1;
                        }
                    }
                    if (found == 0 && D_800CC2D0[i].unk31C->unk11A == 2 && setId != 0x55 && setId != 0x2E) {
                        func_15083568(&D_800CC2D0[i], setId, 1.0f, 0);
                        if (setId == 0x1D) {
                            func_15083568(&D_800CC2D0[i], 0x1E, 1.0f, 0);
                        }
                    }
                } else {
                    for (j = 0; j < numAttached; j++) {
                        if (attached[j]->setId != 0x2A && attached[j]->setId != 0x22 &&
                            attached[j]->setId != 0x2C && attached[j]->setId != 0x64 &&
                            attached[j]->setId != 0x2D && attached[j]->setId != 0x72 &&
                            attached[j]->setId != 0x73 && attached[j]->setId != 0x76 &&
                            attached[j]->setId != 0x77 && attached[j]->setId != 0x78 && attached[j]->setId != 7) {
                            func_15030158(attached[j], 0);
                            D_800CC2D0[i].unk31C->unk11A = 0;
                        }
                    }
                }
            }
            if (setId != 0x55) {
                s32 m;
                for (m = D_800D23B0->numPlayers; m < 25; m++) {
                    other = &D_800CC2D0[m];
                    if (other->interaction_state == 0) {
                        continue;
                    }
                    if (other->unk65 != i + 1) {
                        continue;
                    }
                    if (other->id != 0x6E) {
                        continue;
                    }
                    func_151027E8(other);
                    func_15060F28(other, 0);
                    break;
                }
                // matching: empty tests of the original source; without them IDO allocates registers differently
                if (setId == 0x7B) {}
                if (numAttached == 0x7C) {}
                if (setId == 0x55) {}
            }
            base = D_800D23B0;
        }
        count = base->numPlayers;
    }
    D_80087340++;
    if (D_80087340 >= count) {
        D_80087340 = 0;
    }
    for (i = 0; i < base->numSlots; i++) {
        pickup = func_151149AC(entities[i]);
        if (pickup == NULL) {
            base = D_800D23B0;
            continue;
        }
        if (dropTimers[i] != 0) {
            dropTimers[i] -= D_800BE9E4;
            if (dropTimers[i] <= 0) {
                timers[i] = 4;
                dropTimers[i] = 0;
            }
        }
        if (timers[i] > 0) {
            timers[i] -= D_800BE9E4;
            if (timers[i] <= 0) {
                timers[i] = 0;
                pickup->inactive = 0;
                if (dropTimers[i] != 0) {
                    pickup->x = dropX[i];
                    pickup->y = dropY[i];
                    pickup->z = dropZ[i];
                } else if (!(flags[i] & 4)) {
                    if (flags[i] & 1) {
                        x = D_800D23B0->numSpawns;
                        do {
                            r = func_150ADA68() * 0.9999899864f * x;
                            for (k = 0; k < D_800D23B0->numSpawns; k++) {
                                if (r == spawns[k]) {
                                    r = -1;
                                    k = 1000;
                                }
                            }
                        } while (r == -1);
                    } else {
                        r = homeSpawns[i];
                    }
                    spawns[i] = r;
                    idx = func_15086D48(D_800D23B0->spawnNodes[r]);
                    if (idx != -1) {
                        func_15086CBC(idx, &x, &y, &z);
                        pickup->x = x;
                        pickup->y = (s16)y + 60;
                        pickup->z = z;
                    }
                }
            }
        } else if (timers[i] == 0) {
            j = pickup->work.words[1] - 1; // work word 1: player + 1 that touched the pickup, 0 = none
            if (j != -1) {
                pickup->work.words[1] = 0;
                if (items[i] == 0x8AE) {
                    D_800CC2D0[j].unk31C->grenade_count += 3;
                    if (D_800CC2D0[j].unk31C->grenade_count >= 7) {
                        D_800CC2D0[j].unk31C->grenade_count = 6;
                    }
                    timers[i] = D_800D23B0->respawnDelay;
                } else {
                    func_1508DAEC(j, 0);
                    if (flags[i] & 2) {
                        timers[i] = D_800D23B0->respawnDelay;
                    } else {
                        timers[i] = -1;
                        owners[i] = j | 0x2000;
                        spawns[i] = -1;
                    }
                    dropTimers[i] = 0;
                    slots[j] = i;
                    D_800D23B0->unk5C[j] = 1;
                }
                kind = D_800D23B0->gameType;
                if (kind == 0xB9) {
                    D_800CC2D0[j].unk31C->unk12C = 600;
                }
                kind = D_800D23B0->gameType;
                if (kind == 0xB8) {
                    if (items[i] == (pp = D_800CC2D0[j].unk31C->unk128)) { // matching: the u16 temporary
                        if (items[i] == 1) {
                            D_800CC2D0[j].unk31C->unk12C = 600;
                        } else if (items[i] == 0x80) {
                            D_800CC2D0[j].unk31C->unk12C = 1800;
                        } else if (items[i] == 0x10) {
                            D_800CC2D0[j].unk31C->unk12C = 3;
                        }
                    }
                }
            }
        }
        base = D_800D23B0;
    }
}

void func_1508E6C8(void) {
}

/* Per item `type` value (100000 by default). */
s32 func_1508E6D0(s32 type) {
    switch (type) {
    case 0x3F:
        return 1800;
    case 0x14:
        return 3200;
    case 0x22:
        return 500;
    case 0x23:
        return 10000;
    case 0x16:
        return 100000;
    case 0x15:
        return 500;
    case 0x21:
        return 500;
    case 0x24:
        return 100000;
    case 0x19:
    case 0x40:
        return 500;
    case 9:
        return 5000;
    case 0x38:
        return 5000;
    case 0x18:
    case 0x41:
        return 100000;
    case 3:
        return 750;
    case 0x37:
        return 1500;
    default:
        return 100000;
    }
}

/* Returns the attachment set id for item `type`, or -1; bit 0x1000 is a flag on it. */
s32 func_1508E780(s32 type) {
    if (D_800D23B0->gameType != 0xB8) {
        switch (type) {
        case 0x14:
            return 0x28;
        case 0x3F:
            return 0x2F;
        case 0x22:
            return 0x56;
        case 0x23:
            return 0x16;
        case 0x16:
            return 0x24;
        case 0x15:
            return 0x4B;
        case 0x21:
            return 0x58;
        case 0x24:
            return 0x63;
        case 0x19:
            return 0x57;
        case 0x40:
            return 0x21;
        case 9:
            return 0x1D;
        case 0x38:
            return 0x7B;
        case 0x39:
            return 0x82;
        case 0x37:
            return 0x85;
        case 0x18:
            return 0x55;
        case 0x41:
            return 0x2E;
        case 0x25:
            return 0x1060;
        default:
            return -1;
        }
    } else {
        switch (type) {
        case 8:
            return -1;
        case 1:
            return 0x105C;
        case 16:
            return 0x1019;
        case 2:
            return 0x1059;
        default:
            return -1;
        }
    }
}

void func_15062800(Actor *, Actor *);

/* Player `player` takes over the multiplayer state of player `source`. */
s32 func_1508E89C(s32 player, s32 source) {
    Actor *obj;
    s32 savedRot;
    s32 bit0;
    s32 tmp;
    u8 slot;
    MpGame *res;

    obj = &D_800CC2D0[player];
    if (D_800D18A4[player] > 50) {
        if (!(D_800D18A2 & (1 << player))) {
            bit0 = 1 << player;
            D_800D18A2 |= bit0;
            D_800D18A0 &= ~bit0;
            D_800D18A0 |= (1 << source);
            tmp = func_150859AC(source, 3);
            func_15085710(player, 2, tmp);
            func_15085710(source, 2, 0);
            if (D_800D23B0->seenMask != 0) {
                D_800D23B0->seenMask &= ~bit0;
                D_800D23B0->seenMask |= (1 << source);
            }
            if (D_800D23B0->layer) {
                D_800D23B0->layer[player] = D_800D23B0->layer[source];
            }
            if (D_800D23B0->unk39C) {
                D_800D23B0->unk39C[player] = D_800D23B0->unk39C[source];
            }
            if (D_800D23B0->attacker) {
                D_800D23B0->attacker[player] = D_800D23B0->attacker[source];
            }
            if (D_800D23B0->slot) {
                D_800D23B0->slot[player] = D_800D23B0->slot[source];
                if (D_800D23B0->slot[player] >= 0) {
                    D_800D23B0->slotOwner[D_800D23B0->slot[player]] = player;
                }
            }
            D_800D23B0->unk16C0[player] = D_800D23B0->unk16C0[source];
            savedRot = obj->unk31C->unk1AA;
            func_15062800(&D_800CC2D0[source], obj);
            if (D_800BE9F0 == 0x3F) {
                if (obj->unk13C >= 100) {
                    slot = obj->unk13C - 100;
                    res = func_1509B570(D_800D2E48);
                    if (res != NULL) {
                        s32 idx;
                        s32 id;

                        idx = obj - D_800CC2D0;
                        id = D_800CC2D0[slot].unique_id - 20;
                        res->unk24B8[id] = idx | 0x2000;
                        res->unk24DC[id] = idx;
                    }
                }
            }
            obj->unk31C->unk84 = 0;
            obj->immune = 90;
            obj->unk31C->unk1AA = savedRot;
            obj->unk31C->unk1AF = 0x40;
            func_15128774(obj->camera, obj);
            D_8008FD94++;
        }
    }
    return 0;
}

/* Calls func_1509BFB0(1, actor | 0x2000, id, param) for player's actor (unk13F). */
void func_1508EB90(s32 player, s32 id, s32 param) {
    s32 temp = D_800CC2D0[player].unk13F;
    func_1509BFB0(1, temp | 0x2000, id, param);
}

/* func_1508EB90(player, 0x14, param). */
void func_1508EBF8(s32 player, s32 param) {
    s32 temp = D_800CC2D0[player].unk13F;
    func_1509BFB0(1, temp | 0x2000, 0x14, param);
}

/* func_1508EB90(player, 0x61, param). */
void func_1508EC5C(s32 player, s32 param) {
    s32 temp = D_800CC2D0[player].unk13F;
    func_1509BFB0(1, temp | 0x2000, 0x61, param);
}


/* Finds the entry of kind `kind` linked to (type, id); returns its id or -1. */
s32 func_1508ECC0(u16 type, u16 id, u16 kind) {
    u32 i;
    u32 j;
    u32 k;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < D_800D23C0[i].count; j++) {
            if (key == D_800D23C0[i].members[j]) {
                for (k = 0; k < D_800D23C0[i].count; k++) {
                    if (kind == (D_800D23C0[i].members[k] >> 12) && k != j) {
                        return D_800D23C0[i].members[k] & 0xFFF;
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

/* Clears link group `index`. */
void func_1508EDBC(s32 index) {
    if (index < D_80087380) {
        D_800D23C0[index].count = 0;
        D_800D23C0[index].unk4 = 0;
        D_800D23C0[index].unk0 = 0;
    }
}

void func_1503DDD0(s32);
void func_15114B94(u32);

/* Fires every entry of the link group containing (type, id), then clears the group. */
void func_1508EE0C(u16 type, u16 id) {
    u32 i;
    u32 j;
    s32 operand;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < D_800D23C0[i].count; j++) {
            if (D_800D23C0[i].members[j] == key) {
                for (j = 0; j < D_800D23C0[i].count; j++) {
                    operand = D_800D23C0[i].members[j] & 0xFFF;
                    switch (D_800D23C0[i].members[j] >> 12) {
                    case 2:
                        func_15114B94(operand);
                        break;
                    case 3:
                        func_1503DDD0(operand);
                        break;
                    }
                }
                func_1508EDBC(i);
                return;
            }
        }
    }
}
