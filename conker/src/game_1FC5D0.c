#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Updates a ring buffer of trail points: trims the oldest points so the summed segment
 * distance stays within data->maxLength, then gives each point a sine wave offset that
 * grows in amplitude along the trail.
 */
s32 func_151CF120(WaveTrail *actor) {
    WaveTrailData *config;
    WaveTrailPoint *points;
    s32 count;

    count = actor->head.count;
    config = actor->data;
    points = actor->points;
    if (count >= 2) {
        f32 total;
        f32 limit;
        s32 index;
        s32 current;
        WaveTrailPoint *point;

        total = 0.0f;
        index = actor->head.end - 1;
        if (index < 0) index = actor->head.tmpl.itemCount - 1;
        if (index != actor->head.start) {
            limit = config->maxLength;
            do {
                current = index;
                index--;
                if (index < 0) index = actor->head.tmpl.itemCount - 1;
                point = &points[current];
                total += point->dist;
                if (limit < total) {
                    // matching: comparing with 0.0f instead of int 0 changes the emitted code
                    if (point->dist != 0) {
                        WaveTrailPoint *previous;
                        f32 ratio;
                        f32 px, py, pz;
                        f32 dx, dy, dz;

                        ratio = (total - limit) / point->dist;
                        previous = &points[index];
                        px = previous->pos.x;
                        py = previous->pos.y;
                        pz = previous->pos.z;
                        dx = px - point->pos.x;
                        dy = py - point->pos.y;
                        dz = pz - point->pos.z;
                        previous->pos.x = px - dx * ratio;
                        previous->pos.y = py - dy * ratio;
                        previous->pos.z = pz - dz * ratio;
                        point->dist *= 1.0f - ratio;
                    }
                    while (index != actor->head.start) {
                        actor->head.start++;
                        if (actor->head.start == actor->head.tmpl.itemCount) {
                            actor->head.start = 0;
                        }
                        actor->head.count--;
                    }
                    limit = config->maxLength;
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
            WaveTrailPoint *point;
            f32 distance;

            index--;
            if (index < 0) index = actor->head.tmpl.itemCount - 1;
            point = &points[index];
            point->wave = sinf(phase) * amplitude;
            distance = point->dist;
            amplitude += distance * 0.037499998f;
            phase += distance * 0.0122f;
        } while (index != actor->head.start);
    }
    config->phase += -0.70900005f * D_800BE9A4;
    return 1;
}
