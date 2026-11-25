#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* One ring-buffer entry; distance is the segment length to the next-older entry. */
typedef struct {
    f32 x, y, z, distance;
    u8 pad10[4];
    f32 wave;
    u8 pad18[0x10];
} TrailPoint;

typedef struct {
    u8 pad00[0x20];
    f32 limit, phase;
} TrailConfig;

typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailConfig *config;
} TrailActor;

/*
 * Updates a ring buffer of trail points: trims the oldest points so the summed segment
 * distance stays within config->limit, then gives each point a sine wave offset that
 * grows in amplitude along the trail.
 */
s32 func_151CF120(TrailActor *actor) {
    TrailConfig *config;
    TrailPoint *points;
    s32 count;

    count = actor->head.count;
    config = actor->config;
    points = actor->points;
    if (count >= 2) {
        f32 total;
        f32 limit;
        s32 index;
        s32 current;
        TrailPoint *point;

        total = 0.0f;
        index = actor->head.end - 1;
        if (index < 0) index = actor->head.tmpl.itemCount - 1;
        if (index != actor->head.start) {
            limit = config->limit;
            do {
                current = index;
                index--;
                if (index < 0) index = actor->head.tmpl.itemCount - 1;
                point = &points[current];
                total += point->distance;
                if (limit < total) {
                    // matching: comparing with 0.0f instead of int 0 changes the emitted code
                    if (point->distance != 0) {
                        TrailPoint *previous;
                        f32 ratio;
                        f32 px, py, pz;
                        f32 dx, dy, dz;

                        ratio = (total - limit) / point->distance;
                        previous = &points[index];
                        px = previous->x;
                        py = previous->y;
                        pz = previous->z;
                        dx = px - point->x;
                        dy = py - point->y;
                        dz = pz - point->z;
                        previous->x = px - dx * ratio;
                        previous->y = py - dy * ratio;
                        previous->z = pz - dz * ratio;
                        point->distance *= 1.0f - ratio;
                    }
                    while (index != actor->head.start) {
                        actor->head.start++;
                        if (actor->head.start == actor->head.tmpl.itemCount) {
                            actor->head.start = 0;
                        }
                        actor->head.count--;
                    }
                    limit = config->limit;
                    total = limit;
                }
            } while (index != actor->head.start);
            count = actor->head.count;
        }
    }
    if (count >= 2) {
        f32 amplitude;
        f32 phase;
        s32 index;

        amplitude = 0.0f;
        index = actor->head.end;
        phase = config->phase;
        do {
            TrailPoint *point;
            f32 distance;

            index--;
            if (index < 0) index = actor->head.tmpl.itemCount - 1;
            point = &points[index];
            point->wave = sinf(phase) * amplitude;
            distance = point->distance;
            amplitude += distance * 0.037499998f;
            phase += distance * 0.0122f;
        } while (index != actor->head.start);
    }
    config->phase += -0.70900005f * D_800BE9A4;
    return 1;
}
