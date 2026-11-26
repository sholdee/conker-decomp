#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name (see init_50A0.c). */
static s8 D_800D2390;  // camera target mode of the racers (1..3)
static s32 D_800D2394; // bit per spare racer slot in use
static s8 D_800D2398;  // number of racing racers
static s8 D_800D2399;  // number of spare racer slots
static s8 D_800D239A;
f32 D_800D239C; // camera target circling the actor until the start
f32 D_800D23A0;
f32 D_800D23A4;
s8 D_800D23A8;  // race direction: 0 = towards higher node params
s8 D_800D23A9;

#include "functions.h"
#include "variables.h"

/* This file's .data. */
Racer *D_800872A0 = NULL; // per-racer state of the race mode

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)gViews)


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

/* Calls zone_findAt(3, 11, ...) at path node `index`. */
TriggerZone *func_15086C70(s32 index) {
    s16 z = D_800D2350[index].z;

    return zone_findAt(3, 11, D_800D2350[index].x, D_800D2350[index].y, z);
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
s32 func_15086D48(id)
    s32 id;
{
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
        text_setFlags(0x80);
        racer->prevTicks = racer->ticks;
        racer->ticks = D_800BE9E4;
        adist = (obj->old_xpos * dx + dz * obj->old_zpos + d) - dist;
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
                    f = movement_obj[0].theta * 0.01745;
                    D_800D239C = sinf(f) * 300.0f + obj->xpos;
                    D_800D23A0 = obj->ypos;
                    D_800D23A4 = cosf(f) * 300.0f + obj->zpos;
                }
                D_800D2104[obj->listpos][1].x = D_800D239C;
                D_800D2104[obj->listpos][1].y = D_800D23A0;
                D_800D2104[obj->listpos][1].z = D_800D23A4;
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
        if (racer->state >= 2 && obj->mv->bot == 0) {
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
        obj = &movement_obj[racer->actorIndex];
        if (obj->CameraPtr != NULL) {
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
void func_15087EF0(s32 index, Actor *obj) {
    Racer *racer;
    u16 r;
    if (D_800872A0 == NULL) {
        return;
    }
    racer = &D_800872A0[index];
    r = actor_getDirectionAngle(racer->dirZ, racer->dirX, 0);
    if (D_800D23A8 == 0) {
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
    racer->lateralTarget = lateral * (1.0f / 256.0f);
}

/* Ends the race for `racer` (actor obj), entering state 12 with `mode` (unk232); returns 1 if it can't finish now. */
s32 func_1508802C(Racer *racer, Actor *obj, s32 mode) {
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
s32 func_150880F8(s32 index, s32 mode) {
    Racer *racer;
    Actor *obj;
    s32 ret;

    ret = 0;
    if (D_800872A0 == NULL) {
        return 0;
    }

    racer = &D_800872A0[index];
    obj = &movement_obj[racer->actorIndex];
    if (racer->state == 0) {
        ret = func_1508802C(racer, obj, mode);
    } else if (racer->state == 1) {
        racer->state = 2;
        obj->imune = 0xFF;
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
    return racer->lateral * 256.0f;
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
    return racer->speed;
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
    x = movement_obj[racer->actorIndex].xpos;
    z = movement_obj[racer->actorIndex].zpos;
    dx = racer->dirX * sign;
    dz = racer->dirZ * sign;
    self = movement_obj[racer->actorIndex].owner - 1;
    base = -((dx * x) + (dz * z));
    ret = NULL;
    best = 10000.0f;
    for (i = 0; i < D_800D2398; i++) {
        racer = &D_800872A0[i];
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
s32 func_1508855C(Actor *obj) {
    Racer *racers;
    s32 actorIndex;
    s32 i;
    s32 limit;
    Racer *racer;

    actorIndex = obj - movement_obj;
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
                D_800872A0[i].actorIndex = obj - movement_obj;
                return i;
            }
            i++;
            bit <<= 1;
        } while (i < D_800D2399);
    }
    return -1;
}

/* Releases the spare racer slot of the calling actor. */
void func_15088780(Actor *obj) {
    s32 index;

    if (D_800872A0 != NULL) {
        index = func_1508855C(obj);
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
        obj->xpos = func_150498A4(xs, 0, racer->segmentT, &x_derivative);
        obj->ypos = func_150497E0(ys, 0, racer->segmentT);
        obj->zpos = func_150498A4(zs, 0, racer->segmentT, &z_derivative);
        angle = func_15144BC8((actor_getDirectionAngle(racer->dirZ, racer->dirX, 0) + 0x8000) * 0.005493164f);
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
            dx = obj->xpos - entry->x;
            dz = obj->zpos - entry->z;
            distSq = dx * dx + dz * dz;
            if (distSq < best) {
                best = distSq;
                racer->camNode = prevNode;
                racer->camNextNode = nextNode;
            }
        } while (curNode != start);
        func_1512D560(obj->CameraPtr, 5, 0);
        racer->state = 3;
    }
    p = &racer->cam;
    camNext = racer->camNextNode;
    nextNode = racer->camNode;
    entry = &D_800D2350[nextNode];
    other = &D_800D2350[camNext];
    dx = obj->xpos - other->x;
    dz = obj->zpos - other->z;
    distSq = dx * dx + dz * dz;
    dx = obj->xpos - entry->x;
    dz = obj->zpos - entry->z;
    best = dx * dx + dz * dz;
    if (distSq < best) {
        racer->camNode = camNext;
        racer->camNextNode = func_1508907C(racer->camNode, 0xFF, nextNode, param);
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
    func_1512D560(obj->CameraPtr, 7, p);
    cam->FOVX = 30.0f;
    cam->TgetFOVX = 30.0f;
    cam->FOVY = 30.0f;
    cam->TgetFOVY = 30.0f;
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

    if (obj->CameraPtr == NULL) {
        return;
    }
    cam = obj->CameraPtr;
    p = &racer->cam;
    if (leave != 0) {
        p->unk10 = 120;
        p->unkC = 999.0f;
        func_1512D560(obj->CameraPtr, 10, p);
        func_1512D560(obj->CameraPtr, 6, (void *)cam->CNumber);
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
        racer->camX = obj->xpos;
        racer->camY = obj->ypos;
        racer->camZ = obj->zpos;
        racer->camT = 0.0f;
        func_1512D560(obj->CameraPtr, 5, (void *)cam->CNumber);
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
    func_1512D560(obj->CameraPtr, 7, p);
}




