#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
    /* 0x48 */ u8 unk48;
    /* 0x49 */ s8 camState;
    /* 0x4A */ u8 camNode;
    /* 0x4B */ u8 camNextNode;
    /* 0x4C */ CamParams cam;
} Racer;

extern s8 D_800D239A;
extern s8 D_800D2398;
extern s8 D_800D2399;
extern s8 *D_800D23B0;
s32 func_1508802C();
extern s16 D_80087290;
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
extern s8 D_800D23A8;
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
extern f32 D_800D239C;
extern f32 D_800D23A0;
extern f32 D_800D23A4;
extern s8 D_800D2390;
void func_15088F30(Racer *racer, f32 *outX, f32 *outY, f32 *outZ);
f32 func_150497E0(f32 *, s32, f32);
f32 func_150498A4(f32 *, s32, f32, f32 *);
s32 func_15088A08(Racer *racer, f32 step);

/*
 * Per-frame update of racer `index`: steers it along the path to follow its actor, tracks its
 * speed and race time and, for racer 0, recomputes everyone's rank.
 */
void func_15087350(s32 index, s32 arg1, f32 lookahead) {
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
    struct127 *obj;
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
                if (arg1 == 0) {
                    f = D_800CC310 * 0.01745;
                    D_800D239C = sinf(f) * 300.0f + obj->x_position;
                    D_800D23A0 = obj->y_position;
                    D_800D23A4 = cosf(f) * 300.0f + obj->z_position;
                }
                ((s16 **)D_800D2104)[obj->unk13F][4] = D_800D239C;
                ((s16 **)D_800D2104)[obj->unk13F][5] = D_800D23A0;
                ((s16 **)D_800D2104)[obj->unk13F][6] = D_800D23A4;
                obj->unk44 = arg1;
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
        racer->unk48 = arg1;
        D_800D23A9 = 1;
        if (racer->state == 0 && racer->unk48 > 0 && !(D_800D18A0 & (1 << racer->actorIndex))) {
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

void func_150891E8(Racer *racer, struct127 *obj);
void func_150896EC(Racer *racer, struct127 *obj, s32 leave);

/* Runs the race cameras of every racer whose actor has a camera. */
void func_15087CC0(void) {
    Racer *racer;
    struct127 *obj;
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
            } else if (racer->unk48 == 0) {
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
void func_15087E54(s32 index, struct127 *obj) {
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
void func_15087EF0(s32 index, struct127 *obj) {
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

/* Ends the race for `racer` (actor obj); returns 1 if it can't finish now. */
s32 func_1508802C(Racer *racer, struct127 *obj, s32 arg2) {
    struct126 *temp_v0;

    temp_v0 = obj->unk31C;
    if (temp_v0->unk120 != 0) {
        return 1;
    }

    if ((D_800D18A0 & (1 << (obj - D_800CC2D0))) != 0) {
        return 1;
    }

    temp_v0->unk84 = 1;
    obj->interaction_state = 12;
    obj->unk232 = arg2;
    D_800D154C = obj;
    D_800C3E78 = racer->actorIndex;
    obj->unk218 = func_1507BB28(0, arg2);
    obj->unk21C = 0;
    obj->immune = 0xFF;
    racer->state = 2;
    return 0;
}

/* Racer `index` crosses the finish line: record its finish order. */
s32 func_150880F8(s32 index, s32 arg1) {
    Racer *racer;
    struct127 *obj;
    s32 ret;

    ret = 0;
    if (D_800872A0 == NULL) {
        return 0;
    }

    racer = &D_800872A0[index];
    obj = &D_800CC2D0[racer->actorIndex];
    if (racer->state == 0) {
        ret = func_1508802C(racer, obj, arg1);
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

s32 func_15088270(s32 index) {
    Racer *racer;
    if (D_800872A0 == NULL) {
        return 0;
    }
    racer = &D_800872A0[index];
    return (s32)racer->speed;
}

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
struct127 *func_150883B0(s32 index, s32 dir, f32 *lateral, s32 *route) {
    Racer *racer;
    struct127 *ret;
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
s32 func_1508855C(struct127 *obj) {
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
s32 func_1508868C(struct127 *obj) {
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

extern s8 D_800E0BE7;
extern u8 D_800BE616;
extern s8 D_8008FD90;
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];

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
            if ((idx < D_8008FD90) && (racer->state == 0) && (racer->raceTime > 0)) {
                if (lim >= racer->maxLap) {
                    t = racer->raceTime - racer->lapStartTime;
                    if (t > 32000) {
                        t = 32000;
                    }
                    if ((D_800E0C10[idx] <= 0) || (t < D_800E0C10[idx])) {
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
void func_15088D58(struct127 *obj) {
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
void func_150891E8(Racer *racer, struct127 *obj) {
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
    struct108 *cam;

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


void func_1512D560();
extern s32 D_800BE9E4;

/*
 * Pre-race fly-through camera along the type-6 camera nodes; `leave` hands the camera back
 * to the actor.
 */
void func_150896EC(Racer *racer, struct127 *obj, s32 leave) {
    struct108 *cam;
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
        func_1512D560(obj->camera, 6, cam->unk23D);
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
        func_1512D560(obj->camera, 5, cam->unk23D);
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
    D_800D23B0 = 0;
}

void *func_1509B570(s32);
void func_1508BC20(void);
extern f32 D_800872B0[], D_800872F8[];
extern s8 D_800872F0[], D_800E0BD0;
extern s32 D_800D2E48;

s32 func_15089BC0(void) {
    f32 *scales;
    s32 i;
    s32 id;
    void *resource;

    id = D_800D2E48;
    resource = func_1509B570(id);
    if (resource == NULL) return 0;
    switch (id) {
    case 0xB9:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xB8:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBB:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBA:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xB7:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBD:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBE:
        D_800D23B0 = (s8 *)resource;
        break;
    default:
        D_800D23B0 = 0;
        return 0;
    }
    func_1508BC20();
    *(s8 *)((u8 *)D_800D23B0 + 0x1700) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1701) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1703) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1702) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1704) = 0;
    *(s16 *)((u8 *)D_800D23B0 + 0x16BC) = id;
    *(f32 *)((u8 *)D_800D23B0 + 0x16B8) = D_800872B0[D_800E0BD0];
    *(s8 *)((u8 *)D_800D23B0 + 0x1745) = 0;
    scales = D_800872F8;
    for (i = 0; i < 16; i++) {
        *(s32 *)((u8 *)D_800D23B0 + i * 4 + 0x16C0) = 0;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x1868) = scales[D_800E0BD0];
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x17A8) = 0.0f;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x17E8) = 0.0f;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x1828) = 0.0f;
    }
    *(s32 *)((u8 *)D_800D23B0 + 0xC) = D_800872F0[D_800E0BD0];
    return 0;
}

s32 func_15089F9C(s32 arg0) {
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
    struct127 *other;
    struct127 *self;
    s32 *states;
    s32 off;

    states = (s32 *)(D_800D23B0 + 0xE64);
    mask = *(s32 *)(D_800D23B0 + 0x14);
    count = *(s32 *)(D_800D23B0 + 0x10);
    off = arg0 * 4;
    bestIdx = -1;
    if (*(s32 *)(D_800D23B0 + off + 0x15C) == 0) {
        minDist = 750.0f;
        maxDist = 3500.0f;
    } else {
        minDist = 1500.0f;
        maxDist = 8000.0f;
    }
    best = -1000000.0f;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if ((states[i] != 3) && (mask & bit) &&
            (*(s32 *)(D_800D23B0 + off + 0x16C0) & (bit << 16))) {
            other = &D_800CC2D0[i];
            if (other != NULL) {
                self = &D_800CC2D0[arg0];
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
    s32 *teams;
    s32 *states;
    u32 row;
    s32 count;
    s32 rawDist;
    s32 minDist;
    s32 *kinds;
    s32 *ids;
    s32 *dists;
    f32 dx;
    struct127 *other;
    struct127 *self;

    states = (s32 *)(D_800D23B0 + 0xE64);
    mask = *(s32 *)(D_800D23B0 + 0x14);
    count = *(s32 *)(D_800D23B0 + 0x10);
    kinds = (s32 *)(D_800D23B0 + 0x15C);
    active = (s32 *)(D_800D23B0 + 0x39C);
    teams = (s32 *)(D_800D23B0 + 0x9C);
    ids = (s32 *)(D_800D23B0 + 0x5DC);
    dists = (s32 *)(D_800D23B0 + 0x9DC);
    D_800D23B0[0x1704]++;
    if (*(s32 *)(D_800D23B0 + 0x10) == D_800D23B0[0x1704]) {
        D_800D23B0[0x1704] = 0;
    }
    cur = D_800D23B0[0x1704];
    row = cur * 16;
    if (!((1 << cur) & mask)) {
        for (i = 0; i < 16; i++) ids[row + i] = -1;
        return 0;
    }
    n = 0;
    minDist = 32000;
    myTeam = states[cur];
    myMask = ((s32 *)(D_800D23B0 + 0x16C0))[cur];
    for (i = 0; i < count; i++) {
        if (active[i] > 0 && myTeam != states[i]) {
            other = &D_800CC2D0[i];
            self = &D_800CC2D0[cur];
            if (other != NULL) {
                dx = self->x_position - other->x_position;
                dist = sqrtf((dx * dx) + ((self->z_position - other->z_position) * (self->z_position - other->z_position)));
            } else {
                dist = 32000;
            }
            rawDist = dist;
            if (teams[cur] != teams[i]) {
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
                if (*(s16 *)(D_800D23B0 + 0x16BC) != 0xB8 && (kinds[i] == 2 || kinds[i] == 3)) {
                    dist = -1;
                }
            }
            if (*(s16 *)(D_800D23B0 + 0x16BC) == 0xB7) {
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
                dist *= *(f32 *)(D_800D23B0 + 0x16B8);
            }
            if (dist >= 0) {
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
    for (i = n; i < 16; i++) ids[row + i] = -1;
    if (D_800D23B0 + 0x51C) {
        ((s32 *)(D_800D23B0 + 0x51C))[cur] = minDist;
    }
    return 0;
}

extern s8 D_800E0BD0;
extern u8 D_800E0BB8[];
extern s8 D_800872D0[];
extern s16 D_800872C4[];
extern s8 D_800872E0[];
extern s8 D_800872E8[];
extern f32 D_800872F8[];
void func_15042D94(s32, s32, s32, char *, ...);
s32 func_1508CAD8(s32, s32, s32, s32, s32, s32);
void func_1508EC5C(s32, s32);
void func_1508EBF8(s32, s32);
s32 func_1508DAEC(s32, s32);
void func_1508B3F8(void);

#define MGR_W(off) ((s32 *)(D_800D23B0 + (off)))
#define MGR_B(off) ((s8 *)(D_800D23B0 + (off)))
#define MGR_F(off) ((f32 *)(D_800D23B0 + (off)))

s32 func_1508A6FC(void)
{
  s32 i;
  s32 bit;
  s32 isNew;
  s32 team;
  struct127 *obj;
  s32 other;
  s32 type;
  s32 result;
  s32 respawn;
  s32 old;
  s32 reset;
  struct127 *tgt;
  s32 mask14;
  s32 mask18;
  s32 count;
  s32 *arr1C;
  s32 *teams;
  s32 *arr35C;
  s32 *arr31C;
  s32 *arr39C;
  s32 *arr19C;
  s32 *arr21C;
  s32 *arr29C;
  s32 *arr25C;
  s32 *arr41C;
  s32 *arr3DC;
  s32 *arr9C;
  s32 *arrDDC;
  s32 *arr1574;
  u8 flag;
  s32 half;
  s32 j;
  s32 pad;
  s32 flags;
  u8 *vp;
  result = 0;
  i = 0;
  mask14 = ((s32 *) (D_800D23B0 + 0x14))[0];
  mask18 = ((s32 *) (D_800D23B0 + 0x18))[0];
  count = ((s32 *) (D_800D23B0 + 0x10))[0];
  teams = (s32 *) (D_800D23B0 + 0xE64);
  arr35C = (s32 *) (D_800D23B0 + 0x35C);
  arr31C = (s32 *) (D_800D23B0 + 0x31C);
  arr19C = (s32 *) (D_800D23B0 + 0x19C);
  arr21C = (s32 *) (D_800D23B0 + 0x21C);
  arr29C = (s32 *) (D_800D23B0 + 0x29C);
  arr25C = (s32 *) (D_800D23B0 + 0x25C);
  arr41C = (s32 *) (D_800D23B0 + 0x41C);
  arr3DC = (s32 *) (D_800D23B0 + 0x3DC);
  arrDDC = (s32 *) (D_800D23B0 + 0xDDC);
  arr1574 = (s32 *) (D_800D23B0 + 0x1574);
  arr1C = (s32 *) (D_800D23B0 + 0x1C);
  arr39C = (s32 *) (D_800D23B0 + 0x39C);
  arr9C = (s32 *) (D_800D23B0 + 0x9C);
  for (i = 0; i < count; i++)
  {
    bit = 1 << i;
    arr1C[i] = 0;
    isNew = 0;
    if (!(mask14 & bit))
    {
      if (!(mask18 & bit))
      {
        isNew = 1;
      }
      mask18 |= bit;
    }
    if ((mask14 & bit) || isNew)
    {
      obj = &D_800CC2D0[i];
      team = teams[i];
      if ((team != 0) && ((*((s16 *) (D_800D23B0 + 0x16BC))) == 0xB9))
      {
        obj->unk1E4 = 1;
      }
      old = arr29C[i];
      arr21C[i] += D_800BE9E4;
      arr29C[i] += D_800BE9E4;
      arr25C[i] -= D_800BE9E4;
      if (arr25C[i] <= 0)
      {
        arr25C[i] = 0;
      }
      arr41C[i] -= D_800BE9E4;
      if (arr41C[i] <= 0)
      {
        arr41C[i] = 0;
      }
      arr1574[i] -= D_800BE9E4;
      if (arr1574[i] < 0)
      {
        arr1574[i] = 0;
      }
      if ((arr19C[i] == 3) && ((arr25C[i] & 0x1F) >= 0x1D))
      {
        func_1508EC5C(i, 0x8000);
      }
      arr3DC[i] = D_800E0BB8[i];
      D_800E0BB8[i] &= 0x7F;
      if (((s32 *) (D_800D23B0 + 0x4DC)) != 0)
      {
        ((s32 *) (D_800D23B0 + 0x4DC))[i] = obj->unk31C->grenade_count;
      }
      arr39C[i] = func_150859AC(i, 4);
      arr9C[i] = func_15085DA8(obj->y_position);
      if (((arr39C[i] == 0) || isNew) || (obj->pad2FC[1] != 0))
      {
        if (arr35C[i] == 0)
        {
          if (obj->unk20F != 0xFF)
          {
            ((s32 *) (D_800D23B0 + 0x59C))[i] = obj->unk20F;
          }
          else
          {
            ((s32 *) (D_800D23B0 + 0x59C))[i] = -1;
            if (!arr3DC)
            {
            }
          }
          arr35C[i] = 1;
          arr25C[i] = 0;
          arr19C[i] = 0;
          ((s32 *) (D_800D23B0 + 0x5C))[i] = 1;
          ((s32 *) (D_800D23B0 + 0x11C))[i] = -1;
          arrDDC[team]--;
          if ((arr3DC[i] & 0x7F) < count)
          {
            other = teams[arr3DC[i] & 0x7F];
            if (team != other)
            {
              arrDDC[other]++;
            }
          }
          if ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB9)
          {
            ((s32 *) (D_800D23B0 + 0xFF0))[i] = 0;
            func_1508DAEC(i, 1);
          }
          result |= bit;
        }
      }
      else
      {
        arr35C[i] = 0;
      }
      flag = 1;
      if ((arr31C[i] != 0) && ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB8))
      {
        reset = 0;
        if ((arr19C[i] != 1) && (arr21C[i] < 30))
        {
          type = obj->unk232;
          obj->unk222 = arr31C[i] - 2;
          if ((type != 4) && (type != 5))
          {
            func_1508EBF8(i, 4);
          }
          if (type == 4)
          {
            arr21C[i] = 0;
          }
          if (type == 5)
          {
            arr21C[i] = 30;
            ((f32 *) (D_800D23B0 + 0x1868))[i] = D_800872F8[D_800E0BD0];
            ((f32 *) (D_800D23B0 + 0x17A8))[i] = (func_150ADA68() * 50.0f) + obj->x_position;
            ((f32 *) (D_800D23B0 + 0x17E8))[i] = obj->y_position + 40.0f;
            ((f32 *) (D_800D23B0 + 0x1828))[i] = (func_150ADA68() * 50.0f) + obj->z_position;
            if ((arr31C[i] - 2) >= 2)
            {
              tgt = &D_800CC2D0[arr31C[i] - 2];
              ((f32 *) (D_800D23B0 + 0x17A8))[i] += (tgt->x_position - obj->x_position) * 0.5f;
              ((f32 *) (D_800D23B0 + 0x1828))[i] += (tgt->z_position - obj->z_position) * 0.5f;
            }
          }
        }
        flag = 0;
        bit = 0;
        if (D_800872D0[D_800E0BD0] < arr29C[i])
        {
          arr29C[i] = 0;
          old = -1;
        }
        if ((arr29C[i] >= 0) && (old < 0))
        {
          bit = 1;
        }
        if ((D_800872C4[D_800E0BD0] != (-1)) && (arr21C[i] >= D_800872C4[D_800E0BD0]))
        {
          reset = 1;
        }
        ((s8 *) (D_800D23B0 + 0x1725))[i] += (((s8 *) (D_800D23B0 + 0x1705))[i] - ((s8 *) (D_800D23B0 + 0x1725))[i]) >> 2;
        ((s8 *) (D_800D23B0 + 0x1735))[i] += (((s8 *) (D_800D23B0 + 0x1715))[i] - ((s8 *) (D_800D23B0 + 0x1735))[i]) >> 2;
        if (func_1508CAD8(i, arr31C[i] - 2, bit, reset, ((s8 *) (D_800D23B0 + 0x1725))[i], ((s8 *) (D_800D23B0 + 0x1735))[i]) && (arr19C[i] != 1))
        {
          arr21C[i] = 0;
        }
        if (arr29C[i] == 0)
        {
          ((s8 *) (D_800D23B0 + 0x1705))[i] = 0;
          ((s8 *) (D_800D23B0 + 0x1715))[i] = 0;
          if (D_800872E8[D_800E0BD0] < ((s32) (func_150ADA68() * 100.0f)))
          {
            if (func_150ADA68() > 0.5f)
            {
              ((s8 *) (D_800D23B0 + 0x1705))[i] = -50;
            }
            else
            {
              ((s8 *) (D_800D23B0 + 0x1705))[i] = 50;
            }
            if (func_150ADA68() > 0.5f)
            {
              ((s8 *) (D_800D23B0 + 0x1715))[i] = 50;
            }
            else
            {
              ((s8 *) (D_800D23B0 + 0x1715))[i] = 0;
            }
          }
        }
      }
      if (flag && ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB8))
      {
        arr29C[i] = -D_800872E0[D_800E0BD0];
      }
      flags = obj->unk22C & 0x200;
      obj->unk22C &= ~0x200;
      if (flags || (arr3DC[i] & 0x80))
      {
        arr19C[i] = 0;
        arr1C[i] = 1;
      }
      if ((*(((u8 *) obj->unk31C) + 0x1AC)) != 0)
      {
        arr25C[i] = 20;
      }
    }
    else
    {
      arr39C[i] = 0;
      arr9C[i] = -1;
    }
  }

  ((s32 *) (D_800D23B0 + 0x18))[0] = mask18;
  if (D_800BE9E4 != 0)
  {
    ((f32 *) (D_800D23B0 + 0x16B4))[0] = D_800BE9E4;
  }
  else
  {
    ((f32 *) (D_800D23B0 + 0x16B4))[0] = 1.0f;
  }
  if (((s32 *) (D_800D23B0 + 0x55C)) != 0)
  {
    func_1508B3F8();
  }
  if ((*((s16 *) (D_800D23B0 + 0x16BC))) == 0xB9)
  {
    half = ((s32) (((f32 *) D_800BE628)[0x30 / 4] - ((f32 *) D_800BE628)[0x2C / 4])) >> 1;
    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    for (j = 0; j <= D_80082FA0; j++)
    {
      if ((*((u8 *) (&D_800CC2D0[j].pad128))) == 0)
      {
        vp = ((u8 *) D_800BE628) + (j * 0x180);
        func_15042D94((s32) ((*((f32 *) (vp + 0x2C))) + ((f32) half)), (s32) ((*((f32 *) (vp + 0x24))) + 6.0f), 1, "%d", arr1574[j] / 60);
      }
    }

  }
  return result;
}

extern s8 D_8008FD90;
extern s16 *D_8008FDD4;

s32 func_1508B194(s32 arg0) {
    if (arg0 >= D_8008FD90) {
        return 0;
    }

    return *(s16 *)((u8 *)D_8008FDD4 + arg0 * 12 + 0x70);
}

void func_1508B1D4(s32 arg0) {
    if (arg0 < D_8008FD90) {
        *(s16 *)((u8 *)D_8008FDD4 + arg0 * 12 + 0x70) = 0;
    }
}

typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B20C;

typedef struct {
    u8 pad0[0x1745];
    s8 count;
    u8 pad1746[2];
    Entry1508B20C entries[8];
} Mgr1508B20C;

#define MGR ((Mgr1508B20C *)D_800D23B0)

void func_1508B20C(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 n;

    if (MGR != NULL && MGR->count < 8) {
        n = MGR->count++;
        MGR->entries[n].x = arg0;
        MGR->entries[n].y = arg1;
        MGR->entries[n].z = arg2;
        MGR->entries[n].radiusSq = arg3 * arg3;
    }
}

#undef MGR

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[1];
} Search1508B2A8;

void func_1508B2A8(u8 id, Search1508B2A8 *s) {
    PathNode *node;
    f32 dx;
    f32 dist;
    s32 j;
    u8 link;

    /* BUG (original game): the bit index should be id & 7, so ids 8 apart share a visited bit */
    s->visited[id >> 3] |= 1 << (id & 3);
    node = id + D_800D2350;
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


typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B3F8;

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[0x20];
} Search1508B3F8;

void func_1508B3F8(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 k;
    u8 best;
    s32 r;
    s32 *zone;
    struct127 *obj;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    s32 t;
    f32 d;
    Entry1508B3F8 *e;
    Search1508B3F8 s;
    f32 bestDist;
    PathNode *node;

    zone = (s32 *)(D_800D23B0 + 0x55C);
    e = (Entry1508B3F8 *)(D_800D23B0 + 0x1748);
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] != -1) {
            zone[i] = -2;
        }
    }
    for (i = 0; i < *(s8 *)(D_800D23B0 + 0x1745); i++) {
        found = 0;
        s.radiusSq = e[i].radiusSq;
        x = e[i].x;
        y = e[i].y;
        z = e[i].z;
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
                        t = func_15085DA8(y);
                        found = 1;
                        r = func_15085DF8(x, y, z, 0, t);
                        if (r != -1) {
                            s.x = x;
                            s.z = z;
                            s.count = 0;
                            bzero(s.visited, 0x20);
                            func_1508B2A8(r, (Search1508B2A8 *)&s);
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
                            ((s32 *)(D_800D23B0 + 0x5C))[j] = 1;
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
    *(s8 *)(D_800D23B0 + 0x1745) = 0;
}

s32 func_1508C5B8(s32, s32);
s32 func_1508A1BC(void);

typedef struct B9BCActor {
    u8 pad0[0x14];
    f32 field14;
    f32 field18;
    f32 field1C;
} B9BCActor;

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
    count = *(s32 *)((u8 *)D_800D23B0 + 0x10);
    current = *(s32 *)((u8 *)D_800D23B0 + 4);
    active = (s32 *)((u8 *)D_800D23B0 + 0x39C);
    neighbors = (s32 *)((u8 *)D_800D23B0 + 0x5DC);
    costs = (s32 *)((u8 *)D_800D23B0 + 0x9DC);
    targets = (s32 *)((u8 *)D_800D23B0 + 0x11C);
    if (count >= 9) {
        mode = 4;
    }
    func_1508C5B8(0, mode);
    func_1508A1BC();
    weights = (u8 *)D_800D23B0 + current * 16 + 0x15B4;
    winner = -1;
    minimum = 10000000;
    index = 0;
    if (count > 0) {
        do {
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
                        value = (f32)(u32)weights[index] * 0.003921569f;
                        if (index == targets[current]) {
                            value *= 0.8f;
                        }
                        cost = (s32)((f32)cost * value);
                        if (cost < minimum) {
                            minimum = cost;
                            winner = index;
                        }
                    }
                }
            }
            index++;
        } while (index != count);
    }
    *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x49C) = winner;
    if (winner != -1) {
        f32 dx;
        f32 dz;
        B9BCActor *actor;
        B9BCActor *other;

        actor = (B9BCActor *)((u8 *)D_800CC2D0 + current * 0x32C);
        other = (B9BCActor *)((u8 *)D_800CC2D0 + winner * 0x32C);
        dx = actor->field14 - other->field14;
        dz = actor->field1C - other->field1C;
        distance = sqrtf(dx * dx + dz * dz);
        *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x45C) = (s32)distance;
    } else {
        *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x45C) = minimum;
    }
    return 0;
}

void func_1508BC20(void) {
    s32 i;
    f32 fn;
    s32 pos;
    s32 val;
    s32 dir;
    s32 delta;
    s32 step;
    s32 h;
    u8 *base;
    s8 n;
    s8 used[16];

    n = D_8008FD8C;
    base = (u8 *)D_800D23B0 + 0x15B4;
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
                base[pos] = val;
            } else {
                base[pos] = 0xFF;
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
        base += 0x10;
    }
}

s32 func_1508BF14(void) {
    s32 numSlots;
    s32 owner;
    s32 pass;
    s32 myIdx;
    s32 i;
    s32 j;
    s32 end;
    s32 cost;
    s32 bestCost;
    s8 counts[16];
    s32 *owners;
    s32 *links;
    s32 *costs;
    s32 *statuses;
    s32 *weights;
    s32 *bestCosts;
    s32 *bestSlots;
    s32 bestSlot;
    s32 n;

    myIdx = ((s32 *)D_800D23B0)[1];
    numSlots = ((s32 *)D_800D23B0)[4];
    statuses = (s32 *)(D_800D23B0 + 0x15C);
    weights = (s32 *)(D_800D23B0 + 0x39C);
    owners = (s32 *)(D_800D23B0 + 0xE64);
    bestCosts = (s32 *)(D_800D23B0 + 0x45C);
    bestSlots = (s32 *)(D_800D23B0 + 0x49C);
    links = (s32 *)(D_800D23B0 + 0x5DC);
    costs = (s32 *)(D_800D23B0 + 0x9DC);
    owner = owners[myIdx];

    for (i = 0; i < numSlots; i++) counts[i] = 0;

    for (pass = 0; pass != 2; pass++) {
        for (i = 0; i < numSlots; i++) {
            n = 0;
            if ((statuses[i] == 2) || (statuses[i] == 3)) {
                n = 1;
            }
            if ((n && (pass == 0)) || (!n && (pass != 0))) {
                if (owners[i] == owner) {
                    bestSlot = -1;
                    bestCost = 10000000;
                    for (j = 0; j < numSlots; j++) {
                        if (weights[j] > 0) {
                            if (owners[j] != owner) {
                                cost = -1;
                                n = j * 16;
                                end = n + 16;
                                while ((n < end) && (links[n] != -1)) {
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

s32 func_1508C194(s32 arg0) {
    return 0;
}

typedef struct GameB4080SightInner {
    u8 pad0[0x75];
    u8 kind75;
    u8 pad76[0xE];
    u8 flag84;
    u8 pad85[0x8F];
    s16 height114;
} GameB4080SightInner;

typedef struct GameB4080SightActor {
    s32 type;
    u8 pad4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x184];
    s16 pointX;
    s16 pointY;
    s16 pointZ;
    u8 pad1AA[0x172];
    GameB4080SightInner *inner;
} GameB4080SightActor;

void *func_1505EEF4(s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *,
                  f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
extern s16 D_800872C4[];
extern s8 D_800E0BD0;

void func_1508C1A4(s32 arg0, s32 arg1) {
    s32 blocked;
    s32 seen;
    GameB4080SightActor *from;
    GameB4080SightActor *to;
    s32 fromFlag;
    s32 toFlag;
    GameB4080SightInner *fromInner;
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
    from = func_1505EEF4(arg0 & 0xFFFF0FFF);
    to = func_1505EEF4(arg1 & 0xFFFF0FFF);
    if (from != NULL && to != NULL) {
        x0 = from->x;
        z0 = from->z;
        fromInner = from->inner;
        fromFlag = fromInner->flag84;
        if (fromFlag) {
            y0 = from->y + fromInner->height114 * 0.75f;
            if ((fromInner->kind75 & 0x7F) == 9) {
                y0 -= 50.0f;
            }
        } else if (from->type == 0x1F) {
            y0 = from->y + 80.0f;
        } else if (to->inner->flag84 && D_800872C4[D_800E0BD0] != -1) {
            x0 = from->pointX;
            y0 = from->pointY;
            z0 = from->pointZ;
        } else {
            y0 = from->y + 40.0f;
        }
        x1 = to->x;
        z1 = to->z;
        toFlag = to->inner->flag84;
        if (toFlag) {
            y1 = to->y + to->inner->height114 * 0.75f;
            if ((to->inner->kind75 & 0x7F) == 9) {
                y1 -= 50.0f;
            }
        } else if (to->type == 0x1F) {
            y1 = to->y + 80.0f;
        } else if (fromFlag && D_800872C4[D_800E0BD0] != -1) {
            x1 = to->pointX;
            y1 = to->pointY;
            z1 = to->pointZ;
        } else {
            y1 = to->y + 40.0f;
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
        *(s32 *)((u8 *)D_800D23B0 + arg0 * 4 + 0x16C0) |= 1 << arg1;
        *(s32 *)((u8 *)D_800D23B0 + arg1 * 4 + 0x16C0) |= 1 << arg0;
    }
    if (seen) {
        *(s32 *)((u8 *)D_800D23B0 + arg0 * 4 + 0x16C0) |= 0x10000 << arg1;
        *(s32 *)((u8 *)D_800D23B0 + arg1 * 4 + 0x16C0) |= 0x10000 << arg0;
    }
}

s32 func_1508C5B8(s32 arg0, s32 arg1) {
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
    struct127 *obj;
    s8 idx[4];
    f32 dist[4];
    s32 *list;
    s32 mask;

    cur = D_800D23B0[0x1700];
    other = D_800D23B0[0x1701];
    list = (s32 *)(D_800D23B0 + 0xE64);
    if (arg1 != 0) {
        for (i = 0; i < D_8008FD90; i++) {
            x0 = D_800CC2D0[i].x_position;
            x1 = D_800CC2D0[i].x_position;
            for (k = 0; &dist[k] < &dist[3]; k++) {
                idx[k] = -1;
                dist[k] = 900000000.0f;
            }
            for (k = D_8008FD90; k < D_8008FD8C; k++) {
                if (list[i] != list[k] &&
                    !(((s32 *)(D_800D23B0 + 0x16C0))[i] & (0x10001 << k))) {
                    dx = x0 - D_800CC2D0[i].x_position;
                    dz = x1 - D_800CC2D0[i].z_position;
                    if (i == ((s32 *)(D_800D23B0 + 0x11C))[k]) {
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
                    ((s32 *)(D_800D23B0 + 0x16C0))[i] &= ~(0x10001 << j);
                    ((s32 *)(D_800D23B0 + 0x16C0))[j] &= ~(0x10001 << i);
                    func_1508C1A4(i, j);
                }
            }
        }
    }
    if (arg1 != 0) {
        obj = &D_800CC2D0[cur];
        do {
            team = *(u8 *)&obj->pad128;
            m = 0;
            do {
                other++;
                if (other >= D_8008FD8C) {
                    cur++;
                    if (cur >= D_8008FD8C - 1) {
                        cur = 0;
                    }
                    obj = &D_800CC2D0[cur];
                    team = *(u8 *)&obj->pad128;
                    other = cur + 1;
                }
                ((s32 *)(D_800D23B0 + 0x16C0))[cur] &= ~(0x10001 << other);
                ((s32 *)(D_800D23B0 + 0x16C0))[other] &= ~(0x10001 << cur);
                mask = (1 << cur) | (1 << other);
                m++;
            } while (m < 64 && ((D_800D18A0 & mask) ||
                                    team == *(u8 *)&D_800CC2D0[other].pad128));
            func_1508C1A4(cur, other);
        } while (--arg1 != 0);
    }
    D_800D23B0[0x1700] = cur;
    D_800D23B0[0x1701] = other;
    return ((s32 *)(D_800D23B0 + 0x16C0))[arg0];
}

s32 func_1508C9CC(void) {
    s32 count;
    s32 ret;
    s8 cur;
    struct127 *obj;
    struct126 *unk31C;

    count = 0;
    ret = -1;
    cur = D_800D23B0[0x1702];
    do {
        cur = (s8)(cur + 1);
        if (cur >= D_8008FD8C) {
            cur = 0;
        }

        obj = &D_800CC2D0[cur];
        if (obj->interaction_state != 0) {
            unk31C = obj->unk31C;
            if ((unk31C != 0) && (unk31C->unk84 != 0) && ((D_800D18A0 & (1 << cur)) == 0)) {
                count = D_8008FD8C;
                ret = cur;
            }
        }
        count++;
    } while (count < D_8008FD8C);

    D_800D23B0[0x1702] = cur;
    return ret;
}

s32 func_1508CA88(void) {
    s8 **p2;
    s8 temp;

    p2 = &D_800D23B0;
    D_800D23B0[0x1703]++;
    temp = D_800D23B0[0x1703];
    if (temp >= D_8008FD90) {
        D_800D23B0[0x1703] = 0;
        temp = (*p2)[0x1703];
    }
    return temp;
}

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
    u8 pad79[0xC];
    u8 unk85;
    u8 unk86;
    u8 pad87[3];
    u16 unk8A;
    u16 unk8C;
    s8 unk8E;
    s8 unk8F;
    f32 unk90;
    u8 pad94[0xAC];
    f32 unk140;
    u8 pad144[0x28];
    f32 unk16C;
    f32 unk170;
    f32 unk174;
    f32 unk178;
} CAD8Inner;

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0xC];
    f32 oldX;
    f32 oldY;
    f32 oldZ;
    u8 pad38[0x16C];
    s16 unk1A4;
    s16 unk1A6;
    s16 unk1A8;
} CAD8Def;

extern s32 D_800BE9AC;
extern s8 D_800872D8[];
void func_150DA5EC(struct127 *, f32);

#define INNER(o) ((CAD8Inner *)(o)->unk31C)
#define OBJ obj


s32 func_1508CAD8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    CAD8Def *def;
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
    struct127 *obj;
    s32 type;
    s32 hasCam;
    s32 result;
    struct108 *cam;
    s32 pad;

    doInner = 1;
    result = 0;
    obj = &D_800CC2D0[arg0];
    ratio = ((f32 *)D_800BE628)[0x70 / 4] / ((f32 *)D_800BE628)[0x6C / 4];
    if (arg1 != -1) {
        cam = OBJ->camera;
        if (cam != NULL) {
            hasCam = 1;
        } else {
            hasCam = 0;
        }
        type = INNER(OBJ)->unk78;
        if ((type == 9) || (type == 0x38) || (type == 0x15)) {
            hasCam = 0;
        }
        def = func_1505EEF4(arg1 & 0xFFFF0FFF);
        if (def == NULL) {
            return 1;
        }
        flagA = 0;
        mode = 0;
        if ((type == 0x24) || (type == 0x18) || (type == 0x23) || (type == 0x41)) {
            flagA = 1;
        } else if (type == 9) {
            mode = 1;
        } else if ((type == 0x15) || (type == 0x1B) || (type == 3)) {
            mode = 1;
            if (type != 0x1B) {
                mode = 3;
            }
            doInner = 0;
        } else if (type == 0x38) {
            mode = 2;
        }
        if (type == 3) {
            arg3 = 0;
        }
        height = 40.0f;
        if (def->unk0 == 0x1F) {
            arg3 = 0;
            height = 80.0f;
        }
        dx = 0.0f;
        dz = 0.0f;
        if ((*(s16 *)(D_800D23B0 + 0x16BC) == 0xB9) || (D_800E0BD0 >= 3)) {
            switch (INNER(OBJ)->unk78) {
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
                dx = def->x - OBJ->x_position;
                dz = def->z - OBJ->z_position;
                dy = def->y - OBJ->y_position;
                dy = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
                V = (dy * V) / *(f32 *)(D_800D23B0 + 0x16B4);
                dx = (def->x - def->oldX) * V;
                dz = (def->z - def->oldZ) * V;
            }
        }
        if (arg3 == 0) {
            dx = (def->x + dx) - ((f32 *)(D_800D23B0 + 0x17A8))[arg0];
            dz = (def->z + dz) - ((f32 *)(D_800D23B0 + 0x1828))[arg0];
        } else {
            dx = ((f32)def->unk1A4 + dx) - ((f32 *)(D_800D23B0 + 0x17A8))[arg0];
            dz = ((f32)def->unk1A8 + dz) - ((f32 *)(D_800D23B0 + 0x1828))[arg0];
        }
        scale = ((f32 *)(D_800D23B0 + 0x1868))[arg0] * (f32)D_800BE9E4;
        if (type == 3) {
            scale = 1.0f;
            arg5 = 0;
            arg4 = 0;
        }
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        ((f32 *)(D_800D23B0 + 0x17A8))[arg0] += dx * scale;
        ((f32 *)(D_800D23B0 + 0x1828))[arg0] += dz * scale;
        dx = ((f32 *)(D_800D23B0 + 0x17A8))[arg0] - OBJ->x_position;
        dz = ((f32 *)(D_800D23B0 + 0x1828))[arg0] - OBJ->z_position;
        dist = sqrtf((dx * dx) + (dz * dz));
        if (dist < 1.0f) {
            dist = 1.0f;
        } else {
            f32 tx;

            if (((dist < 100.0f) || (type == 3)) && (arg5 != 0)) {
                arg5 = 0;
                if (arg4 == 0) {
                    arg4 = 50;
                }
            }
            mag = arg4;
            if (type == 9) {
                mag = arg4 * 1.25f;
            }
            mag = mag / dist;
            tx = dx;
            dx -= dz * mag;
            dz += tx * mag;
        }
        ang = (f32)func_1505A630(dx, -dz, 0) * 0.005493164f;
        if (type != 3) {
            curYaw = INNER(OBJ)->unk16C;
        } else {
            curYaw = OBJ->unk40 - 90.0f;
        }
        ang = func_15144BC8(ang);
        curYaw = func_15144BC8(curYaw);
        diff = func_15144BC8(curYaw - ang);
        if (diff > 180.0f) {
            diff = -360.0f + diff;
        }
        if ((diff > 45.0f) || (diff < -45.0f)) {
            result = 1;
        }
        if (arg3 == 0) {
            cur = ((def->y + height) - (f32)arg5) - ((f32 *)(D_800D23B0 + 0x17E8))[arg0];
        } else {
            cur = ((f32)def->unk1A6 - (f32)arg5) - ((f32 *)(D_800D23B0 + 0x17E8))[arg0];
        }
        ((f32 *)(D_800D23B0 + 0x17E8))[arg0] += cur * scale;
        cur = INNER(OBJ)->unk140 - ((f32 *)(D_800D23B0 + 0x17E8))[arg0];
        ang = (f32)func_1505A630(dist, cur, 0) * 0.005493164f;
        curYaw = INNER(OBJ)->unk170;
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
            ptr = &INNER(OBJ)->unk90;
            INNER(OBJ)->unk16C = INNER(OBJ)->unk16C - diff;
            INNER(OBJ)->unk170 = ang;
            func_15048758(&INNER(OBJ)->unk16C);
            INNER(OBJ)->unk174 = INNER(OBJ)->unk16C;
            INNER(OBJ)->unk178 = INNER(OBJ)->unk170;
            INNER(OBJ)->unk85 = 0;
            INNER(OBJ)->unk86 = 0;
        } else if (hasCam != 0) {
            ptr = &cam->unk1A4;
            ((s8 *)cam->unk36C)[2] = (s32)yaw;
            ((s8 *)cam->unk36C)[3] = (s32)-roll;
        } else {
            ptr = &INNER(OBJ)->unk90;
            INNER(OBJ)->unk85 = (s32)yaw;
            INNER(OBJ)->unk86 = (s32)-roll;
        }
        if (mode != 0) {
            if (type == 0x1B) {
                if ((h < -50.0f) && (((s32 *)(D_800D23B0 + 0x16C0))[arg0] & (0x10001 << arg1))) {
                    INNER(OBJ)->unk8A |= 0x2000;
                    INNER(OBJ)->unk8E = 2;
                }
            } else if (type == 3) {
                f32 f;

                f = (dist - 136.0f) / 664.0f;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                f = (sinf((f32)(D_800BE9AC & 0x3F) * 0.09815625101f) * 0.174999997f) + f;
                if (f > 1.0f) {
                    f = 1.0f;
                }
                f = (f * 0.02569000237f) + 0.01724099927f;
                INNER(OBJ)->unk86 = 0;
                INNER(OBJ)->unk85 = 0;
                OBJ->unk76 -= (s32)(yaw * 16.0f);
                OBJ->unk7A = OBJ->unk76;
                OBJ->unk40 = (f32)(s16)(OBJ->unk7A + 0x4000) * 0.005493164f;
                func_150DA5EC(OBJ, f);
            } else if (((s32 *)(D_800D23B0 + 0x29C))[arg0] >= 0) {
                INNER(OBJ)->unk8A = 0x2000;
                if (mode == 1) {
                    INNER(OBJ)->unk8E = 2;
                    if (D_800872D8[D_800E0BD0] < ((s32 *)(D_800D23B0 + 0x29C))[arg0]) {
                        INNER(OBJ)->unk8A &= 0xDFFF;
                        INNER(OBJ)->unk8E = 0;
                    }
                } else if (mode == 2) {
                    if (arg2 != 0) {
                        INNER(OBJ)->unk8E = 0xA;
                    } else {
                        INNER(OBJ)->unk8A &= 0xDFFF;
                    }
                } else {
                    INNER(OBJ)->unk8E = 2;
                }
            }
        } else if (arg2 != 0) {
            if (flagA != 0) {
                INNER(OBJ)->unk8C = 0;
                INNER(OBJ)->unk8A = 0;
            } else {
                INNER(OBJ)->unk8C = 0x2000;
                INNER(OBJ)->unk8F = 1;
            }
        } else if (flagA != 0) {
            if (cam != NULL) {
                *cam->unk36C |= 0x2000;
            }
            INNER(OBJ)->unk8A = 0x2000;
            INNER(OBJ)->unk8E = 2;
        }
        INNER(OBJ)->unk8A |= 0x10;
        INNER(OBJ)->unk8E = 1;
        *ptr += (h - *ptr) * 0.125f;
        if (hasCam != 0) {
            cam->unk1A8 = cam->unk1A4 * ratio;
        }
        if (cam != NULL) {
            *cam->unk36C |= 0x10;
        }
    }
    return result;
}
#undef OBJ
#undef INNER

extern s16 D_80087320[][3];
void func_1508EB90(s32 arg0, s32 arg1, s32 arg2);
extern f32 D_8009DA8C;

s32 func_1508D850(s32 arg0) {
    s8 *timers;
    s8 *targets;
    s32 *target;
    f32 dx;
    struct127 *obj;
    f32 dz;
    f32 dist;
    s32 range;
    s32 limit;
    s32 off;

    targets = (s8 *)(D_800D23B0 + 0x11C);
    timers = (s8 *)(D_800D23B0 + 0x29C);
    obj = &D_800CC2D0[arg0];
    off = arg0 * 4;
    target = (s32 *)(targets + off);
    obj->unk31C->pad129[0] = *target;
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
    limit = ((s16 *)D_80087320)[D_800E0BD0 * 3 + range];
    if (limit < *(s32 *)(timers + off)) {
        *(s32 *)(timers + off) = 0;
        if (*(s32 *)(D_800D23B0 + off + 0x16C0) & (0x10000 << *target)) {
            func_1508EB90(arg0, 0x61, 0x2000);
        }
    }
    if (*(s32 *)(timers + off) == 0) {
        obj->unk31C->pad129[1] = 0;
        if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
            obj->unk31C->pad129[1] = 1;
        }
    }
}

s32 func_1508DA1C(void) {
    s32 i;
    s32 *a = (s32 *)((u8 *)D_800D23B0 + 0xEB0);
    s32 *b = (s32 *)((u8 *)D_800D23B0 + 0xF70);
    s32 *c = (s32 *)((u8 *)D_800D23B0 + 0xFF0);
    s32 *d = (s32 *)((u8 *)D_800D23B0 + 0x1070);
    s32 *e = (s32 *)((u8 *)D_800D23B0 + 0x10F0);
    s32 *f = (s32 *)((u8 *)D_800D23B0 + 0x12F4);

    for (i = 0; i < *(s32 *)((u8 *)D_800D23B0 + 0x10); i++) {
        a[i] = -1;
    }
    for (i = 0; i < *(s32 *)((u8 *)D_800D23B0 + 0xEA4); i++) {
        b[i] = -1;
        c[i] = 0;
        e[i] = -1;
        d[i] = 1;
        f[i] = 0;
    }
}

extern struct127 D_800CC2D0[26];
extern s8 *D_800D23B0;
s32 func_1508802C();
s32 func_1508DAEC(s32 arg0, s32 arg1)
{
  s8 *base;
  s32 value;
  s32 *entry;
  s8 *slot;
  s32 slotOffset;
  int mode;
  struct127 *obj;
  base = D_800D23B0;
  value = arg0 * 4;
  entry = (s32 *) (((s32) base) + value);
  if (base == 0)
  {
    return 0;
  }
  value = entry[0x3AC];
  if (value >= 0)
  {
    if (arg1 < 0)
    {
      return 1;
    }
    entry[0x3AC] = -1;
    slotOffset = value * 4;
    obj = &D_800CC2D0[arg0];
    slot = D_800D23B0 + slotOffset;
    if (((*((s32 *) (slot + 0x11F4))) & 2) == 0)
    {
      *((s32 *) (slot + 0xF70)) = -1;
      *((s32 *) ((D_800D23B0 + slotOffset) + 0x10F0)) = -1;
      mode = *((s32 *) (D_800D23B0 + 0xEAC));
      if (arg1 == 1)
      {
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x12F4)) = 0x12C;
        obj = &D_800CC2D0[arg0];
        mode = 4;
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x1374)) = (s32) obj->x_position;
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x13F4)) = (s32) (obj->y_position + 50.0f);
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x1474)) = (s32) obj->z_position;
      }
      *((s32 *) ((D_800D23B0 + slotOffset) + 0x1070)) = mode;
    }
  }
  return 0;
}


void func_1502EA0C(struct127 *, s32, s32, s32, s32, s32);
s32 func_1508E780(s32 arg0);
void func_1508EB90(s32 arg0, s32 arg1, s32 arg2);
void func_1502EA50(struct127 *);
void func_15030158(void *, s32);
s32 func_15033E28(struct127 *, void **);
void func_15080620(s32, s32, s32, s32);
void func_151027E8(struct127 *);
u8 *func_151149AC(u8);

extern s32 D_800D121C;

void func_1508DC24(void) {
    static s8 D_80087340; s8 *base;
    s32 i;
    struct127 *obj;
    s32 count;
    s32 cur;
    s32 prev;
    s32 anim;
    s32 n;
    s32 j;
    f32 x;
    f32 y;
    f32 z;
    s32 found;
    u16 pp;
    s32 kind;
    s32 *arrEB0;
    s32 *arrEF0;
    s32 *arrF70;
    s32 *arrFF0;
    s32 *arr1070;
    s32 *arr10F0;
    s32 *arr1174;
    s32 *arr11F4;
    s32 *arr1274;
    s32 *arr12F4;
    s32 *arr1374;
    s32 *arr13F4;
    s32 *arr1474;
    u8 *buf[10];
    struct127 *other;
    u8 *ent;
    s32 idx;
    s32 r;
    s32 k;

    base = D_800D23B0;
    i = 0;
    arrEB0 = (s32 *)(base + 0xEB0);
    arrEF0 = (s32 *)(base + 0xEF0);
    arrF70 = (s32 *)(base + 0xF70);
    arrFF0 = (s32 *)(base + 0xFF0);
    arr1070 = (s32 *)(base + 0x1070);
    arr10F0 = (s32 *)(base + 0x10F0);
    arr1174 = (s32 *)(base + 0x1174);
    arr11F4 = (s32 *)(base + 0x11F4);
    arr1274 = (s32 *)(base + 0x1274);
    arr12F4 = (s32 *)(base + 0x12F4);
    arr1374 = (s32 *)(base + 0x1374);
    arr13F4 = (s32 *)(base + 0x13F4);
    arr1474 = (s32 *)(base + 0x1474);
    count = *(s32 *)(base + 0x10);
    for (; i < count; i++) {
        kind = *(s16 *)(base + 0x16BC);
        if ((kind == 0xB9)) {
            if ((*(u8 *)((u8 *)&D_800CC2D0[i] + 0x128) == 0)) {
            } else {
                continue;
            }
        }
        {
            prev = D_800CC2D0[i].unk31C->unk75 & 0x7F;
            cur = arrFF0[i];
            if (D_800CC2D0[i].unk31C->unk19B >= 5) {
                cur = 0x3A;
            } else if (D_800CC2D0[i].unk31C->unk19B > 0) {
                cur = 0x26;
            }
            if (cur == 0) {
                if (arrEB0[i] != -1) {
                    cur = arr1174[arrEB0[i]];
                } else {
                    cur = *(s32 *)(base + 0x1170);
                }
            } else if (cur == 0x457) {
                cur = 0;
            }
            if (kind == 0xB8) {
                prev = D_800CC2D0[i].unk31C->unk128 & ~0x20;
            }
            if (cur != prev) {
                anim = func_1508E780(prev);
                if (anim != -1) {
                    anim &= ~0x1000;
                    func_1508EB90(i, 0x4C, anim);
                    if (anim == 0x1D) {
                        func_1508EB90(i, 0x4C, 0x1E);
                    }
                    D_800CC2D0[i].unk31C->unk11A = 0;
                }
                anim = func_1508E780(cur);
                if (anim > 0x1000) {
                    anim &= ~0x1000;
                    func_1508EB90(i, 0x4B, anim);
                }

                if (*(s16 *)(D_800D23B0 + 0x16BC) == 0xB8) {
                    if (prev == 0x80) {
                        func_1502EA50(&D_800CC2D0[i]);
                        D_800CC2D0[i].immune = 0;
                    }
                    D_800CC2D0[i].unk31C->unk128 = cur;
                    if (cur == 1) {
                        *(s16 *)((u8 *)D_800CC2D0[i].unk31C + 0x12C) = 0x258;
                    } else if (cur == 0x80) {
                        *(s16 *)((u8 *)D_800CC2D0[i].unk31C + 0x12C) = 0x708;
                        func_1502EA0C(&D_800CC2D0[i], 0, 0, 0xFF, 0xBF, 4);
                    } else if (cur == 0x10) {
                        *(s16 *)((u8 *)D_800CC2D0[i].unk31C + 0x12C) = 3;
                    }
                } else {
                    func_15080620(i, 1, cur, 0);
                    if ((cur == 0) && (prev != 3)) {
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
            if ((D_80087340 == i) && (*(s16 *)(base + 0x16BC) != 0xB8)) {
                anim = func_1508E780(cur);
                found = 0;
                n = func_15033E28(&D_800CC2D0[i], (void **)buf);
                if ((n == 0) && (D_800CC2D0[i].unk31C->unk11A == 2) && (anim != 0x55) && (anim != 0x2E)) {
                    func_15083568(&D_800CC2D0[i], anim, 1.0f, 0);
                    if (anim == 0x1D) {
                        func_15083568(&D_800CC2D0[i], 0x1E, 1.0f, 0);
                    }
                }
                if ((n != 0) && (cur != 0x20)) {
                    if (anim != -1) {
                        anim &= ~0x1000;
                        for (j = 0; j < n; j++) {
                            if (((buf[j][6] != 0x1E) || (anim != 0x1D)) && (anim != buf[j][6]) && (buf[j][6] != 0x2A) &&
                                (buf[j][6] != 0x22) && (buf[j][6] != 0x2D) && (buf[j][6] != 0x72) && (buf[j][6] != 0x73) &&
                                (buf[j][6] != 0x76) && (buf[j][6] != 0x77) && (buf[j][6] != 0x78) && (buf[j][6] != 7)) {
                                func_15030158(buf[j], 0);
                                D_800CC2D0[i].unk31C->unk11A = 0;
                            } else if (anim == buf[j][6]) {
                                found = 1;
                            }
}
                        if ((found == 0) && (D_800CC2D0[i].unk31C->unk11A == 2) && (anim != 0x55) && (anim != 0x2E)) {
                            func_15083568(&D_800CC2D0[i], anim, 1.0f, 0);
                            if (anim == 0x1D) {
                                func_15083568(&D_800CC2D0[i], 0x1E, 1.0f, 0);
                            }
                        }
                    } else {
                        for (j = 0; j < n; j++) {
                            if ((buf[j][6] != 0x2A) && (buf[j][6] != 0x22) && (buf[j][6] != 0x2C) && (buf[j][6] != 0x64) &&
                                (buf[j][6] != 0x2D) && (buf[j][6] != 0x72) && (buf[j][6] != 0x73) && (buf[j][6] != 0x76) &&
                                (buf[j][6] != 0x77) && (buf[j][6] != 0x78) && (buf[j][6] != 7)) {
                                func_15030158(buf[j], 0);
                                D_800CC2D0[i].unk31C->unk11A = 0;
                            }
}
                    }
                }
                if (anim != 0x55) {
                    s32 m;
                    for (m = *(s32 *)(D_800D23B0 + 0x10); m < 25; m++) {
                        other = &D_800CC2D0[m];
                        if (other->interaction_state == 0) continue;
                        if (other->unk65 != i + 1) continue;
                        if (other->id != 0x6E) continue;
                        func_151027E8(other);
                            func_15060F28(other, 0);
                            break;
                    }
                    if (anim == 0x7B) {
                    }
                    if (n == 0x7C) {
                    }
                    if (anim == 0x55) {
                    }
                }
                base = D_800D23B0;
            }
            count = *(s32 *)(base + 0x10);
        }
    }
    D_80087340++;
    if (D_80087340 >= count) {
        D_80087340 = 0;
    }
    for (i = 0; i < *(s32 *)(base + 0xEA4); i++) {
        ent = func_151149AC(arrEF0[i]);
        if (ent == NULL) {
            base = D_800D23B0;
            continue;
        }
        if (arr12F4[i] != 0) {
            arr12F4[i] -= D_800BE9E4;
            if (arr12F4[i] <= 0) {
                arr1070[i] = 4;
                arr12F4[i] = 0;
            }
        }
        if (arr1070[i] > 0) {
            arr1070[i] -= D_800BE9E4;
            if (arr1070[i] <= 0) {
                arr1070[i] = 0;
                ent[0x6E] = 0;
                if (arr12F4[i] != 0) {
                    *(s16 *)(ent + 0x10) = arr1374[i];
                    *(s16 *)(ent + 0x12) = arr13F4[i];
                    *(s16 *)(ent + 0x14) = arr1474[i];
                } else if (!(arr11F4[i] & 4)) {
                    if (arr11F4[i] & 1) {
                        x = *(s32 *)(D_800D23B0 + 0xEA8);
                        do {
                            r = func_150ADA68() * 0.9999899864f * x;
                            for (k = 0; k < *(s32 *)(D_800D23B0 + 0xEA8); k++) {
                                if (r == arr10F0[k]) {
                                    r = -1;
                                    k = 1000;
                                }
                            }
                        } while (r == -1);
                    } else {
                        r = arr1274[i];
                    }
                    arr10F0[i] = r;
                    idx = func_15086D48(*(s32 *)(D_800D23B0 + 0x14F4 + r * 4));
                    if (idx != -1) {
                        func_15086CBC(idx, &x, &y, &z);
                        *(s16 *)(ent + 0x10) = x;
                        *(s16 *)(ent + 0x12) = (s16)y + 0x3C;
                        *(s16 *)(ent + 0x14) = z;
                    }
                }
            }
        } else if (arr1070[i] == 0) {
            j = *(s32 *)(ent + 0x80) - 1;
            if (j != -1) {
                *(s32 *)(ent + 0x80) = 0;
                if (arr1174[i] == 0x8AE) {
                    D_800CC2D0[j].unk31C->grenade_count += 3;
                    if (D_800CC2D0[j].unk31C->grenade_count >= 7) {
                        D_800CC2D0[j].unk31C->grenade_count = 6;

                    }
                    arr1070[i] = *(s32 *)(D_800D23B0 + 0xEAC);
                } else {
                    func_1508DAEC(j, 0);
                    if (arr11F4[i] & 2) {
                        arr1070[i] = *(s32 *)(D_800D23B0 + 0xEAC);
                    } else {
                        arr1070[i] = -1;
                        arrF70[i] = j | 0x2000;
                        arr10F0[i] = -1;
                    }
                    arr12F4[i] = 0;
                    arrEB0[j] = i;
                    *(s32 *)(D_800D23B0 + j * 4 + 0x5C) = 1;
                }
                if ((kind = *(s16 *)(D_800D23B0 + 0x16BC)) == 0xB9) {
                    *(s16 *)((u8 *)D_800CC2D0[j].unk31C + 0x12C) = 0x258;
                }
                if ((kind = *(s16 *)(D_800D23B0 + 0x16BC)) == 0xB8) {
                    if (arr1174[i] == (pp = *(u8 *)((u8 *)D_800CC2D0[j].unk31C + 0x128))) {
                        if (arr1174[i] == 1) {
                            *(s16 *)((u8 *)D_800CC2D0[j].unk31C + 0x12C) = 0x258;
                        } else if (arr1174[i] == 0x80) {
                            *(s16 *)((u8 *)D_800CC2D0[j].unk31C + 0x12C) = 0x708;
                        } else if (arr1174[i] == 0x10) {
                            *(s16 *)((u8 *)D_800CC2D0[j].unk31C + 0x12C) = 3;
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

s32 func_1508E6D0(s32 arg0) {
    switch (arg0) {
    case 0x3F:
        return 0x708;
    case 0x14:
        return 0xC80;
    case 0x22:
        return 0x1F4;
    case 0x23:
        return 0x2710;
    case 0x16:
        return 0x186A0;
    case 0x15:
        return 0x1F4;
    case 0x21:
        return 0x1F4;
    case 0x24:
        return 0x186A0;
    case 0x19:
    case 0x40:
        return 0x1F4;
    case 0x9:
        return 0x1388;
    case 0x38:
        return 0x1388;
    case 0x18:
    case 0x41:
        return 0x186A0;
    case 0x3:
        return 0x2EE;
    case 0x37:
        return 0x5DC;
    default:
        return 0x186A0;
    }
}

s32 func_1508E780(s32 arg0) {
    if (*(s16 *)((u8 *)D_800D23B0 + 0x16BC) != 0xB8) {
        switch (arg0) {                             /* switch 1 */
        case 0x14:                                  /* switch 1 */
            return 0x28;
        case 0x3F:                                  /* switch 1 */
            return 0x2F;
        case 0x22:                                  /* switch 1 */
            return 0x56;
        case 0x23:                                  /* switch 1 */
            return 0x16;
        case 0x16:                                  /* switch 1 */
            return 0x24;
        case 0x15:                                  /* switch 1 */
            return 0x4B;
        case 0x21:                                  /* switch 1 */
            return 0x58;
        case 0x24:                                  /* switch 1 */
            return 0x63;
        case 0x19:                                  /* switch 1 */
            return 0x57;
        case 0x40:                                  /* switch 1 */
            return 0x21;
        case 0x9:                                   /* switch 1 */
            return 0x1D;
        case 0x38:                                  /* switch 1 */
            return 0x7B;
        case 0x39:                                  /* switch 1 */
            return 0x82;
        case 0x37:                                  /* switch 1 */
            return 0x85;
        case 0x18:                                  /* switch 1 */
            return 0x55;
        case 0x41:                                  /* switch 1 */
            return 0x2E;
        case 0x25:                                  /* switch 1 */
            return 0x1060;
        default:                                    /* switch 1 */
            return -1;
        }
    } else {
        switch (arg0) {                             /* switch 2; irregular */
        case 8:                                     /* switch 2 */
            return -1;
        case 1:                                     /* switch 2 */
            return 0x105C;
        case 16:                                    /* switch 2 */
            return 0x1019;
        case 2:                                     /* switch 2 */
            return 0x1059;
        default:                                    /* switch 2 */
            return -1;
        }
    }
}

void func_15062800(struct127 *, struct127 *);

s32 func_1508E89C(s32 arg0, s32 arg1) {
    struct127 *obj;
    s32 savedRot;
    s32 bit0;
    s32 tmp;
    u8 slot;
    s8 *res;

    obj = &D_800CC2D0[arg0];
    if (((u8 *)&D_800D18A4)[arg0] > 50) {
        if (!(D_800D18A2 & (1 << arg0))) {
            bit0 = 1 << arg0;
            D_800D18A2 |= bit0;
            D_800D18A0 &= ~bit0;
            D_800D18A0 |= (1 << arg1);
            tmp = func_150859AC(arg1, 3);
            func_15085710(arg0, 2, tmp);
            func_15085710(arg1, 2, 0);
            if (*(s32 *)(D_800D23B0 + 0x18) != 0) {
                *(s32 *)(D_800D23B0 + 0x18) &= ~bit0;
                *(s32 *)(D_800D23B0 + 0x18) |= (1 << arg1);
            }
            if (D_800D23B0 + 0x9C) {
                ((s32 *)(D_800D23B0 + 0x9C))[arg0] = ((s32 *)(D_800D23B0 + 0x9C))[arg1];
            }
            if (D_800D23B0 + 0x39C) {
                ((s32 *)(D_800D23B0 + 0x39C))[arg0] = ((s32 *)(D_800D23B0 + 0x39C))[arg1];
            }
            if (D_800D23B0 + 0x3DC) {
                ((s32 *)(D_800D23B0 + 0x3DC))[arg0] = ((s32 *)(D_800D23B0 + 0x3DC))[arg1];
            }
            if (D_800D23B0 + 0xEB0) {
                ((s32 *)(D_800D23B0 + 0xEB0))[arg0] = ((s32 *)(D_800D23B0 + 0xEB0))[arg1];
                if (((s32 *)(D_800D23B0 + 0xEB0))[arg0] >= 0) {
                    ((s32 *)(D_800D23B0 + 0xF70))[((s32 *)(D_800D23B0 + 0xEB0))[arg0]] = arg0;
                }
            }
            ((s32 *)(D_800D23B0 + 0x16C0))[arg0] = ((s32 *)(D_800D23B0 + 0x16C0))[arg1];
            savedRot = obj->unk31C->unk1AA;
            func_15062800(&D_800CC2D0[arg1], obj);
            if (D_800BE9F0 == 0x3F) {
                if (obj->unk13C >= 100) {
                    slot = obj->unk13C - 100;
                    res = func_1509B570(D_800D2E48);
                    if (res != NULL) {
                        s32 idx;
                        s32 id;

                        idx = obj - D_800CC2D0;
                        id = D_800CC2D0[slot].unique_id - 20;
                        *(s32 *)(res + id * 4 + 0x24B8) = idx | 0x2000;
                        *(s32 *)(res + id * 4 + 0x24DC) = idx;
                    }
                }
            }
            obj->unk31C->unk84 = 0;
            obj->immune = 90;
            obj->unk31C->unk1AA = savedRot;
            *((u8 *)obj->unk31C + 0x1AF) = 0x40;
            func_15128774(obj->camera, obj);
            (*(s8 *)&D_8008FD94)++;
        }
    }
    return 0;
}

extern struct127 D_800CC40F[];

void func_1508EB90(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, arg1, arg2);
}

void func_1508EBF8(s32 arg0, s32 arg1) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, 0x14, arg1);
}

void func_1508EC5C(s32 arg0, s32 arg1) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, 0x61, arg1);
}

typedef struct {
    u16 unk0;
    u16 count;
    s32 unk4;
    u16 entries[8];
} GameB4080Group;

s32 func_1508ECC0(u16 arg0, u16 arg1, u16 arg2) {
    u32 i;
    u32 j;
    u32 k;
    u16 key;

    key = (arg0 << 12) + arg1;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
            if (key == (*(GameB4080Group **)&D_800D23C0)[i].entries[j]) {
                for (k = 0; k < (*(GameB4080Group **)&D_800D23C0)[i].count; k++) {
                    if (arg2 == ((*(GameB4080Group **)&D_800D23C0)[i].entries[k] >> 12) && k != j) {
                        return (*(GameB4080Group **)&D_800D23C0)[i].entries[k] & 0xFFF;
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8[4];
} Struct1508EDBC;

void func_1508EDBC(s32 arg0) {
    if (arg0 < D_80087380) {
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk2 = 0;
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk4 = 0;
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk0 = 0;
    }
}

void func_1503DDD0(s32);
void func_15114B94(u32);

void func_1508EE0C(u16 arg0, u16 arg1) {
    u32 i;
    u32 j;
    s32 operand;
    u16 key;

    key = (arg0 << 12) + arg1;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
            if ((*(GameB4080Group **)&D_800D23C0)[i].entries[j] == key) {
                for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
                    operand = (*(GameB4080Group **)&D_800D23C0)[i].entries[j] & 0xFFF;
                    switch ((*(GameB4080Group **)&D_800D23C0)[i].entries[j] >> 12) {
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
