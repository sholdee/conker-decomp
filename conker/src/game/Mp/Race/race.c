#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name (see init/Sched/SchedInit/schedinit.c). */
static s8 D_800D2390;  // camera target mode of the racers (1..3)
static s32 gSpareRacerMask; // bit per spare racer slot in use
static s8 gNumRacers;  // number of racing racers
static s8 gNumSpareRacers;  // number of spare racer slots
static s8 gNextFinishOrder;
f32 gRaceTargetX; // camera target circling the actor until the start
f32 gRaceTargetY;
f32 gRaceTargetZ;
s8 gRaceDirection;  // race direction: 0 = towards higher node params
s8 D_800D23A9;

#include "functions.h"
#include "variables.h"

/* This file's .data. */
Racer *gRacers = NULL; // per-racer state of the race mode

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)gViews)


/* Distance between two path nodes, 0 if either is 0xFF. */
f32 mp_nodeDistance(s32 a, s32 b) {
    f32 dx;
    f32 dy;
    f32 dz;
    PathNode *nodes;

    if (a == 0xFF || b == 0xFF) {
        return 0.0f;
    }
    nodes = gPathNodes;
    dx = nodes[a].x - nodes[b].x;
    dy = nodes[a].y - nodes[b].y;
    dz = nodes[a].z - nodes[b].z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void func_15086C68(void) {
}

/* Calls zone_findAt(3, 11, ...) at path node `index`. */
TriggerZone *mp_findNodeZone(s32 index) {
    s16 z = gPathNodes[index].z;

    return zone_findAt(3, 11, gPathNodes[index].x, gPathNodes[index].y, z);
}

/* Writes the position of path node `index` to *x, *y, *z; returns 0 if out of range. */
s32 mp_getNodePos(s32 index, f32 *x, f32 *y, f32 *z) {
    if (index < 0 || index >= gPathNodeCount) {
        return 0;
    }
    *x = gPathNodes[index].x;
    *y = gPathNodes[index].y;
    *z = gPathNodes[index].z;
    return 1;
}

/* Returns the index of the path node whose unk7 equals `id`, or 0xFF. */
s32 mp_findNodeById(id)
    s32 id;
{
    s32 i;

    for (i = 0; i < gPathNodeCount; i++) {
        if (id == gPathNodes[i].unk7) {
            return i;
        }
    }
    return 0xFF;
}

/*
 * Casts a ray from (x, z) along (dx, dz) against the wall edges (type 1 node pairs) of the
 * height band containing y. Returns the distance to the hit, or -1 if no edge is crossed.
 */
f32 mp_castWallRay(f32 x, f32 y, f32 z, f32 dx, f32 dz) {
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

    id = path_getLayerAt(y);
    best = 100.0f;
    for (i = 0; i < gPathNodeCount; i++) {
        n1 = &gPathNodes[i];
        if (n1->type == 1 && n1->layer == id) {
            for (j = 0; j < 5; j++) {
                link = n1->links[j];
                if (link != 0xFF && i < link) {
                    n2 = &gPathNodes[link];
                    if (n2->type == 1) {
                        a = n2->z - n1->z;
                        b = -(f32)(n2->x - n1->x);
                        c = -(n1->x * a + b * n1->z);
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
                            c = -(n1->x * a + b * n1->z);
                            d1 = ((t * dx) + x) * a + ((t * dz) + z) * b + c;
                            d2 = n2->x * a + b * n2->z + c;
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

/*
 * Allocates the racers (numRacers racing plus numSpare free slots) and places them on the
 * path at the node with id `startId`, measuring the lap length on the way.
 */
void mp_initRacers(s32 startId, s32 numRacers, s32 numSpare) {
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

    gRacers = NULL;
    start = mp_findNodeById(startId);
    if (start == 0xFF) {
        return;
    }
    startNode = &gPathNodes[start];
    best = gRaceDirection * 0xFF;
    sel = 0xFF;
    for (i = 0; i < 5; i++) {
        idx = startNode->links[i];
        if (idx != 0xFF) {
            node = &gPathNodes[idx];
            if (node->type == 0) {
                if ((gRaceDirection == 0 && node->param >= best) || (gRaceDirection != 0 && node->param <= best)) {
                    best = node->param;
                    sel = idx;
                }
            }
        }
    }

    if (sel != 0xFF) {
        fwd = mp_pickNextNode(start, sel, 0);
        back = mp_pickNextNode(sel, start, 0);
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
        path[2] = mp_pickNextNode(path[0], path[1], 0);
        lapLength++;
    } while (start != path[0]);
    gNumSpareRacers = numSpare;
    gNumRacers = numRacers;
    numRacers += numSpare;
    gSpareRacerMask = 0;
    gRacers = allocate_memory(numRacers * sizeof(Racer), 1, 0, 0);
    for (i = 0; i < numRacers; i++) {
        racer = &gRacers[i];
        mp_resetRacer(racer);
        racer->nodes[0] = back;
        racer->nodes[1] = start;
        racer->nodes[2] = sel;
        racer->nodes[3] = fwd;
        if (i < gNumRacers) {
            racer->actorIndex = i;
        }
        racer->state = i >= D_8008FD90;
        racer->lapLength = lapLength;
    }

    gNextFinishOrder = 1;
}


/*
 * Per-frame update of racer `index`: steers it along the path to follow its actor, tracks its
 * speed and race time and, for racer 0, recomputes everyone's rank. Until `started`, the
 * camera target circles the actor.
 */
void mp_updateRacer(s32 index, s32 started, f32 lookahead) {
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
    if (gRacers != NULL) {
        racer = &gRacers[index];
        obj = &movement_obj[racer->actorIndex];
        posX = obj->xpos;
        posZ = obj->zpos;
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
                mp_getSplinePoints(racer, xs, ys, zs);
                dx = xs[2] - xs[1];
                dz = zs[2] - zs[1];
                len = sqrtf(dx * dx + dz * dz);
            }
            f = math_catmullRomDeriv(xs, 0, racer->segmentT, &dx);
            g = math_catmullRomDeriv(zs, 0, racer->segmentT, &dz);
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
                ok = mp_advanceRacer(racer, -inv);
            }
            iter++;
            if (iter >= 11) {
                again = 0;
            }
        } while (again);
        text_setFlags(0x80);
        racer->prevTicks = racer->ticks;
        racer->ticks = D_800BE9E4;
        adist = (obj->old_xpos * dx + dz * obj->old_zpos + d) - dist;
        racer->speed = (adist * 4.0f) / racer->prevTicks;
        if (racer->state != 0) {
            bcopy(racer, &copy, sizeof(Racer));
            mp_advanceRacer(&copy, lookahead + lookahead);
            mp_getSplinePoints(&copy, xs, ys, zs);
            gRaceTargetX = math_catmullRom(xs, 0, copy.segmentT);
            gRaceTargetY = math_catmullRom(ys, 0, copy.segmentT);
            gRaceTargetZ = math_catmullRom(zs, 0, copy.segmentT);
            if (index < gNumRacers) {
                if (started == 0) {
                    f = movement_obj[0].theta * 0.01745;
                    gRaceTargetX = sinf(f) * 300.0f + obj->xpos;
                    gRaceTargetY = obj->ypos;
                    gRaceTargetZ = cosf(f) * 300.0f + obj->zpos;
                }
                gSpawnPaths[obj->listpos][1].x = gRaceTargetX;
                gSpawnPaths[obj->listpos][1].y = gRaceTargetY;
                gSpawnPaths[obj->listpos][1].z = gRaceTargetZ;
                obj->stickspeed = started;
                if (D_800D2390 == 1) {
                    obj->yspd = 75.0f;
                } else if (D_800D2390 == 2) {
                    obj->yspd = 55.0f;
                } else if (D_800D2390 == 3) {
                    obj->yspd = 65.0f;
                }
                obj->unk21E = 0;
            lbl:; // matching: the empty labelled statement splits the block for IDO's scheduler
            }
        } else {
            racer->lateral = 0.0f;
            mp_getSplinePoints(racer, xs, ys, zs);
            f = math_catmullRom(xs, 0, racer->segmentT);
            g = math_catmullRom(zs, 0, racer->segmentT);
            racer->lateral = 1.0f;
            mp_getSplinePoints(racer, xs, ys, zs);
            dx = math_catmullRom(xs, 0, racer->segmentT) - f;
            dz = math_catmullRom(zs, 0, racer->segmentT) - g;
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
        if (racer->state >= 2 && obj->mv->bot == 0) {
            mp_endRace(racer, obj, 13);
        }
        if (index == 0) {
            for (i = 0; i < gNumRacers; i++) {
                order[i] = i;
                // matching: (gRacers + i)-> rather than gRacers[i] keeps IDO's FP register order
                vals[i] = (gRacers + i)->segmentT + (gRacers + i)->nodesPassed;
                if ((gRacers + i)->finishOrder != 0x7F) {
                    vals[i] += 30000.0f - (gRacers + i)->finishOrder * 1000.0f;
                }
                j = (gRacers + i)->actorIndex;
                if (D_800D18A0 & (1 << j)) {
                    vals[i] -= 10000.0f;
                }
            }
            do {
                sorted = 1;
                for (i = 0; i < gNumRacers - 1; i++) {
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
            for (i = 0; i < gNumRacers; i++) {
                gRacers[order[i]].rank = i + 1;
            }
        }
    }
}

/* Runs the race cameras of every racer whose actor has a camera. */
void mp_updateRaceCameras(void) {
    Racer *racer;
    Actor *obj;
    s32 i;

    D_800D23A9 = 0;
    if (gRacers == NULL) {
        return;
    }
    for (i = 0; i < gNumRacers; i++) {
        racer = &gRacers[i];
        obj = &movement_obj[racer->actorIndex];
        if (obj->CameraPtr != NULL) {
            if (racer->state >= 2) {
                mp_updateRaceOverCamera(racer, obj);
            } else if (racer->started == 0) {
                mp_updateFlyThroughCamera(racer, obj, 0);
            } else if (racer->camState != 0) {
                mp_updateFlyThroughCamera(racer, obj, 1);
            }
        }
    }
}

/* Sets racer `index`'s route, re-picking the nodes ahead when it is nonzero. */
void mp_setRacerRoute(s32 index, s32 route) {
    Racer *racer;
    if (gRacers == NULL) {
        return;
    }
    racer = &gRacers[index];
    if (route == racer->route) {
        return;
    }
    if (route != 0) {
        racer->nodes[2] = mp_pickNextNode(racer->nodes[0], racer->nodes[1], 1);
        racer->nodes[3] = mp_pickNextNode(racer->nodes[1], racer->nodes[2], 1);
    }
    racer->route = route;
}

/* Keeps obj's movement angle within 0x2400 of racer `index`'s track direction. */
void mp_clampRacerAngle(s32 index, Actor *obj) {
    Racer *racer;
    u16 d;
    u16 r;
    if (gRacers == NULL) {
        return;
    }
    racer = &gRacers[index];
    r = actor_getDirectionAngle(racer->dirZ, racer->dirX, 0) + 0x4000;
    d = r - obj->angle;
    if (d & 0x8000) {
        if (d < 0xDBFF) {
            obj->angle = r + 0x2400;
        }
    } else {
        if (d >= 0x2401) {
            obj->angle = r - 0x2400;
        }
    }
}

/* Points obj's movement angle along the track, angled toward its centre line. */
void mp_alignRacerToTrack(s32 index, Actor *obj) {
    Racer *racer;
    u16 r;
    if (gRacers == NULL) {
        return;
    }
    racer = &gRacers[index];
    r = actor_getDirectionAngle(racer->dirZ, racer->dirX, 0);
    if (gRaceDirection == 0) {
        if (racer->lateral < 0.5f) {
            r += 0x4A00;
        } else {
            r += 0x3600;
        }
    } else {
        if (0.5f <= racer->lateral) {
            r += 0x4A00;
        } else {
            r += 0x3600;
        }
    }
    obj->angle = r;
}

/* Binds racer `index` to actor `actorIndex`. */
void mp_bindRacer(s32 index, s32 actorIndex) {
    Racer *racer;
    if (gRacers == NULL) {
        return;
    }
    racer = &gRacers[index];
    racer->actorIndex = actorIndex;
}

/* Sets racer `index`'s lateral target, in 1/256ths of the track width. */
void mp_setRacerLateral(s32 index, s32 lateral) {
    Racer *racer;
    if (gRacers == NULL) {
        return;
    }
    racer = &gRacers[index];
    racer->lateralTarget = lateral * (1.0f / 256.0f);
}

/* Ends the race for `racer` (actor obj), entering state 12 with `mode` (unk232); returns 1 if it can't finish now. */
s32 mp_endRace(Racer *racer, Actor *obj, s32 mode) {
    ActorCtl *state;

    state = obj->mv;
    if (state->death_State != 0) {
        return 1;
    }

    if ((D_800D18A0 & (1 << (obj - movement_obj))) != 0) {
        return 1;
    }

    state->bot = 1;
    obj->type = 12;
    obj->intelwalk = mode;
    gCurrentActor = obj;
    dc_glbl_CharNum = racer->actorIndex;
    obj->scriptCmd = script_getScript(0, mode);
    obj->unk21C = 0;
    obj->imune = 0xFF;
    racer->state = 2;
    return 0;
}

/* Racer `index` crosses the finish line: record its finish order. */
s32 mp_crossFinishLine(s32 index, s32 mode) {
    Racer *racer;
    Actor *obj;
    s32 ret;

    ret = 0;
    if (gRacers == NULL) {
        return 0;
    }

    racer = &gRacers[index];
    obj = &movement_obj[racer->actorIndex];
    if (racer->state == 0) {
        ret = mp_endRace(racer, obj, mode);
    } else if (racer->state == 1) {
        racer->state = 2;
        obj->imune = 0xFF;
    }

    racer->finishOrder = gNextFinishOrder;
    gNextFinishOrder++;
    return ret;
}

/* Racer `index`'s lateral position in 1/256ths of the track width. */
s32 mp_getRacerLateral(s32 index) {
    Racer *racer;
    if (gRacers == NULL) {
        return 0;
    }
    racer = &gRacers[index];
    return racer->lateral * 256.0f;
}

/* Racer `index`'s distance along the path, in 1/16ths of a node. */
s32 mp_getRacerDistance(s32 index) {
    Racer *racer;
    s32 v;
    if (gRacers == NULL) {
        return 0;
    }
    racer = &gRacers[index];
    v = racer->nodesPassed;
    return (v << 4) + (s32)(racer->segmentT * 16.0f);
}

/* Returns racer `index`'s speed, truncated (0 without racers). */
s32 mp_getRacerSpeed(s32 index) {
    Racer *racer;
    if (gRacers == NULL) {
        return 0;
    }
    racer = &gRacers[index];
    return racer->speed;
}

/* Returns racer `index`'s lap (0 without racers). */
s32 mp_getRacerLap(s32 index) {
    Racer *racer;
    if (gRacers == NULL) {
        return 0;
    }
    racer = &gRacers[index];
    return racer->lap;
}

/*
 * Finds the racer ranked `offset` places from racer `index`. Returns
 * (path distance between them << 8) | that racer's index, or 0x10 if there is none.
 */
s32 mp_findRacerByRank(s32 index, s32 offset) {
    Racer *racer;
    Racer *other;
    s32 i;
    s32 wanted;

    if (gRacers == NULL) {
        return 0x10;
    }

    racer = &gRacers[index];
    wanted = racer->rank + offset;
    i = 0;
    if (gNumRacers > 0) {
        other = gRacers;
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
        } while (i < gNumRacers);
    }

    return 0x10;
}


/*
 * Finds the nearest racer ahead of (dir >= 0) or behind racer `index` on the same route.
 * Stores its lateral position and route in *lateral and *route.
 */
Actor *mp_findAdjacentRacer(s32 index, s32 dir, f32 *lateral, s32 *route) {
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

    if (gRacers == NULL) {
        return NULL;
    }
    racer = &gRacers[index];
    if (dir >= 0) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    dir = racer->route;
    x = movement_obj[racer->actorIndex].xpos;
    z = movement_obj[racer->actorIndex].zpos;
    dx = racer->dirX * sign;
    dz = racer->dirZ * sign;
    self = movement_obj[racer->actorIndex].owner - 1;
    base = -((dx * x) + (dz * z));
    ret = NULL;
    best = 10000.0f;
    for (i = 0; i < gNumRacers; i++) {
        racer = &gRacers[i];
        if (self != racer->actorIndex && (dir == 0 || dir == racer->route)) {
            x = movement_obj[racer->actorIndex].xpos;
            z = movement_obj[racer->actorIndex].zpos;
            d = (dx * x) + (dz * z) + base;
            if (d > 0.0f && d < best) {
                best = d;
                *lateral = racer->lateral;
                *route = racer->route;
                ret = &movement_obj[racer->actorIndex];
            }
        }
    }
    return ret;
}

/* Returns the index of the racer driven by obj, or -1. */
s32 mp_findRacer(Actor *obj) {
    Racer *racers;
    s32 actorIndex;
    s32 i;
    s32 limit;
    Racer *racer;

    actorIndex = obj - movement_obj;
    racers = gRacers;
    if (racers == NULL) {
        return -1;
    }

    if (actorIndex == 0) {
        return 0;
    }

    i = 1;
    limit = gNumRacers + gNumSpareRacers;
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


/* Copies racer `src` over racer `dst`, keeping dst's actor and state. */
void mp_copyRacer(s32 src, s32 dst) {
    s32 actorIndex;
    s32 state;

    if (gRacers != NULL) {
        actorIndex = gRacers[dst].actorIndex;
        state = gRacers[dst].state;
        bcopy(&gRacers[src], &gRacers[dst], sizeof(Racer));
        gRacers[dst].actorIndex = actorIndex;
        gRacers[dst].state = state;
    }
}

/* Claims a free spare racer slot for actor obj; returns its index or -1. */
s32 mp_claimSpareRacer(Actor *obj) {
    s32 i;
    s32 bit;
    Racer *racers;

    racers = gRacers;
    if (racers == NULL) {
        return -1;
    }

    bit = 1;
    i = 0;
    if (gNumSpareRacers > 0) {
        do {
            if ((gSpareRacerMask & bit) == 0) {
                gSpareRacerMask |= bit;
                i += gNumRacers;
                mp_resetRacer(&racers[i]);
                gRacers[i].state = 1;
                gRacers[i].actorIndex = obj - movement_obj;
                return i;
            }
            i++;
            bit <<= 1;
        } while (i < gNumSpareRacers);
    }
    return -1;
}

/* Releases the spare racer slot of the calling actor. */
void mp_releaseSpareRacer(Actor *obj) {
    s32 index;

    if (gRacers != NULL) {
        index = mp_findRacer(obj);
        gRacers[index].actorIndex = 0;
        gSpareRacerMask &= ~(1 << (index - gNumRacers));
    }
}

/* Returns whether racer 0's camNodes[2] is 0xFF (no node). */
s32 func_150887F8(void) {
    Racer *racers = gRacers;
    if (racers == NULL) {
        return 0;
    }
    return racers->camNodes[2] == 0xFF;
}

/* Resets a racer to the start of the race. */
void mp_resetRacer(Racer *racer) {
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
u8 mp_pickNextNode(u8 prev, u8 cur, s8 route) {
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
    node = &gPathNodes[cur];
    for (i = 0; i < 5; i++) {
        if (node->links[i] != 0xFF && node->links[i] != prev) {
            e = &gPathNodes[node->links[i]];
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
s32 mp_advanceRacer(Racer *racer, f32 step) {
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
        racer->nodes[0] = mp_pickNextNode(racer->nodes[2], racer->nodes[1], racer->route);
        dir = -1;
    } else if (racer->segmentT >= 1.0f) {
        racer->segmentT -= 1.0f;
        racer->nodes[0] = racer->nodes[1];
        racer->nodes[1] = racer->nodes[2];
        racer->nodes[2] = racer->nodes[3];
        racer->nodes[3] = mp_pickNextNode(racer->nodes[1], racer->nodes[2], racer->route);
        dir = 1;
        if (gRaceDirection == 0) {
            if (gPathNodes[racer->nodes[1]].param == 5) {
                D_800D2390 = 1;
            }
            if (gPathNodes[racer->nodes[1]].param == 6) {
                D_800D2390 = 2;
            }
        } else {
            if (gPathNodes[racer->nodes[1]].param == 7) {
                D_800D2390 = 1;
            }
            if (gPathNodes[racer->nodes[1]].param == 8) {
                D_800D2390 = 2;
            }
            if (gPathNodes[racer->nodes[1]].param == 9) {
                D_800D2390 = 3;
            }
        }
    }

    if (dir != 0) {
        lim = gSettings[7];
        idx = racer->actorIndex;
        racer->nodesPassed += dir;
        racer->lap = racer->nodesPassed / racer->lapLength;
        if (gIsMultiplayer != 0) {
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

/* Places actor obj at the start of its racer's path, facing along it. */
void mp_placeAtStart(Actor *obj) {
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
    index = mp_findRacer(obj);
    if (index >= 0) {
        racer = &gRacers[index];
        racer->lateral = 0.5f;
        racer->lateralTarget = 0.5f;
        racer->segmentT = 0.5f;
        do {
            /* skip forward while the current node's edge partner (type 4) has a nonzero param */
            again = 0;
            i = 0;
            // matching: a byte cursor over the node (links at +9) and 0xFF in a variable keep the frame size
            cursor = (u8 *)&gPathNodes[racer->nodes[1]];
            do {
                link = cursor[9];
                i++;
                if (none != link) {
                    target = &gPathNodes[link];
                    if (target->type == 4) {
                        again = target->param;
                    }
                }
                cursor++;
            } while (i != 5);
            if (again != 0) {
                mp_advanceRacer(racer, 1.0f);
            }
        } while (again != 0);
        racer->segmentT = 0.0f;
        mp_getSplinePoints(racer, xs, ys, zs);
        obj->xpos = math_catmullRomDeriv(xs, 0, racer->segmentT, &x_derivative);
        obj->ypos = math_catmullRom(ys, 0, racer->segmentT);
        obj->zpos = math_catmullRomDeriv(zs, 0, racer->segmentT, &z_derivative);
        angle = math_wrapDegrees((actor_getDirectionAngle(racer->dirZ, racer->dirX, 0) + 0x8000) * 0.005493164f);
        obj->theta = angle;
        rotation = (s16)(angle * 182.0444489f) - 0x4000;
        obj->langle = rotation;
        obj->fangle = rotation;
        obj->angle = rotation;
    }
}

/*
 * Fills the four spline control points of `racer`: each node moved toward its edge partner
 * (a type-4 neighbour) by the racer's lateral position.
 */
void mp_getSplinePoints(Racer *racer, f32 *outX, f32 *outY, f32 *outZ) {
    s32 i;
    s32 j;
    u8 id;
    PathNode *node;
    PathNode *next;
    f32 dx;
    f32 dz;

    for (i = 0; i < 4; i++) {
        if (racer->nodes[i] != 0xFF) {
            node = &gPathNodes[racer->nodes[i]];
            next = node;
            for (j = 0; j < 5; j++) {
                id = node->links[j];
                if (id != 0xFF && gPathNodes[id].type == 4) {
                    next = &gPathNodes[id];
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
u8 mp_findNeighbour(u8 node, u8 type, u8 exclude, u8 param) {
    s32 i;
    u8 result;
    PathNode *n;
    PathNode *t;

    result = 0xFF;
    if (node >= gPathNodeCount) {
        return 0xFF;
    }
    n = &gPathNodes[node];
    for (i = 0; i < 5; i++) {
        if (n->links[i] != 0xFF && n->links[i] != exclude) {
            t = &gPathNodes[n->links[i]];
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


/* Race-over camera: follows the actor along the type-5 camera nodes, looking at it. */
void mp_updateRaceOverCamera(Racer *racer, Actor *obj) {
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

    if (obj->CameraPtr == NULL) {
        return;
    }
    if (D_8008FDBC & 0x40) {
        param = 1;
    } else {
        param = 0;
    }
    cam = obj->CameraPtr;
    if (racer->state == 2) {
        for (i = 0; i < gPathNodeCount; i++) {
            entry = &gPathNodes[i];
            if (entry->type == 5 && entry->param == 2) {
                racer->camNode = i;
                racer->camNextNode = i;
                for (j = 0; j < 5; j++) {
                    if (entry->links[j] != 0xFF) {
                        other = &gPathNodes[entry->links[j]];
                        if ((!gRaceDirection && other->param == 3) || (gRaceDirection && other->param != 3)) {
                            racer->camNextNode = entry->links[j];
                        }
                    }
                }
                i = gPathNodeCount;
            }
        }
        curNode = racer->camNextNode;
        best = 1000000000.0f;
        prevNode = racer->camNode;
        start = curNode;
        do {
            entry = &gPathNodes[curNode];
            nextNode = mp_findNeighbour(curNode, 0xFF, prevNode, param);
            prevNode = curNode;
            curNode = nextNode;
            dx = obj->xpos - entry->x;
            dz = obj->zpos - entry->z;
            distSq = dx * dx + dz * dz;
            if (distSq < best) {
                best = distSq;
                racer->camNode = prevNode;
                racer->camNextNode = nextNode;
            }
        } while (curNode != start);
        camera_queueMessage(obj->CameraPtr, 5, 0);
        racer->state = 3;
    }
    p = &racer->cam;
    camNext = racer->camNextNode;
    nextNode = racer->camNode;
    entry = &gPathNodes[nextNode];
    other = &gPathNodes[camNext];
    dx = obj->xpos - other->x;
    dz = obj->zpos - other->z;
    distSq = dx * dx + dz * dz;
    dx = obj->xpos - entry->x;
    dz = obj->zpos - entry->z;
    best = dx * dx + dz * dz;
    if (distSq < best) {
        racer->camNode = camNext;
        racer->camNextNode = mp_findNeighbour(racer->camNode, 0xFF, nextNode, param);
        entry = other;
        dx = obj->xpos - entry->x;
        dz = obj->zpos - entry->z;
        best = distSq;
    }
    dy = obj->ypos - entry->y;
    best = sqrtf(dy * dy + best);
    dx /= best;
    dy /= best;
    dz /= best;
    if (best > 1000.0f) {
        best = 1000.0f;
    }
    p->unk0 = 0;
    p->atX = obj->xpos;
    p->atY = obj->ypos;
    p->atZ = obj->zpos;
    p->eyeX = obj->xpos - dx * best;
    p->eyeY = obj->ypos - dy * best;
    p->eyeZ = obj->zpos - dz * best;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    camera_queueMessage(obj->CameraPtr, 7, p);
    cam->FOVX = 30.0f;
    cam->TgetFOVX = 30.0f;
    cam->FOVY = 30.0f;
    cam->TgetFOVY = 30.0f;
}



/*
 * Pre-race fly-through camera along the type-6 camera nodes; `leave` hands the camera back
 * to the actor.
 */
void mp_updateFlyThroughCamera(Racer *racer, Actor *obj, s32 leave) {
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

    if (obj->CameraPtr == NULL) {
        return;
    }
    cam = obj->CameraPtr;
    p = &racer->cam;
    if (leave != 0) {
        p->unk10 = 120;
        p->unkC = 999.0f;
        camera_queueMessage(obj->CameraPtr, 10, p);
        camera_queueMessage(obj->CameraPtr, 6, (void *)cam->CNumber);
        racer->camState = 0;
        return;
    }
    if (racer->camState == 0) {
        next = gRaceDirection + 1;
        for (i = 0; i < gPathNodeCount; i++) {
            entry = &gPathNodes[i];
            if (entry->type == 6 && entry->param == next) {
                racer->camNodes[0] = i;
                i = gPathNodeCount;
            }
        }
        racer->camNodes[1] = mp_findNeighbour(racer->camNodes[0], 6, 0xFF, 0);
        racer->camNodes[2] = mp_findNeighbour(racer->camNodes[1], 6, racer->camNodes[0], 0);
        racer->camNodes[3] = mp_findNeighbour(racer->camNodes[2], 6, racer->camNodes[1], 0);
        racer->camX = obj->xpos;
        racer->camY = obj->ypos;
        racer->camZ = obj->zpos;
        racer->camT = 0.0f;
        camera_queueMessage(obj->CameraPtr, 5, (void *)cam->CNumber);
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
            racer->camNodes[3] = mp_findNeighbour(racer->camNodes[2], 6, racer->camNodes[1], 0);
        }
    }
    active = 0;
    for (i = 0; i < 4; i++) {
        if (racer->camNodes[i] != 0xFF) {
            mp_getNodePos(racer->camNodes[i], &xs[i], &ys[i], &zs[i]);
            active = 1;
            mp_getNodePos(mp_findNeighbour(racer->camNodes[i], 4, 0xFF, 0), &lxs[i], &lys[i], &lzs[i]);
        } else if (active == 1) {
            active = 0;
            angle = obj->theta * 0.0174500000000000002;
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
    p->eyeX = math_catmullRom(xs, 0, angle);
    p->eyeY = math_catmullRom(ys, 0, angle);
    p->eyeZ = math_catmullRom(zs, 0, angle);
    p->atX = math_catmullRom(lxs, 0, angle);
    p->atY = math_catmullRom(lys, 0, angle);
    p->atZ = math_catmullRom(lzs, 0, angle);
    p->unk0 = 0;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    camera_queueMessage(obj->CameraPtr, 7, p);
}




