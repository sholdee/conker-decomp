#include <ultra64.h>
#include "structs.h"
#include "conker/game.h"
#include "conker/model.h"


/* The number of colours in the ramp of each vertex group (groups 1..5). */
static const u8 gVtxRampLengths[] = { 4, 4, 3, 3, 2 };
/* The colour ramps. */
static const Color24 gVtxRamp0[4] = {
    { 0xFF, 0x64, 0x00 }, { 0xFF, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF },
};
static const Color24 gVtxRamp2[4] = {
    { 0xFF, 0x5A, 0x00 }, { 0xFF, 0x1E, 0x00 }, { 0xFF, 0xFF, 0xFF },
};
static const Color24 gVtxRamp3[4] = {
    { 0xB4, 0x3C, 0x00 }, { 0xB4, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF },
};
static const Color24 gVtxRamp4[4] = {
    { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 },
};
/* The colour ramp of each vertex group. */
static const Color24 *gVtxRamps[] = { gVtxRamp0, gVtxRamp0, gVtxRamp2, gVtxRamp3, gVtxRamp4 };

void model_rampVertexGroupColours(Actor *arg0, f32 *arg1, s32 arg2) {
    Vtx *vertices;
    const Color24 *first;
    const Color24 *second;
    f32 value;
    f32 interval;
    f32 fraction;
    u8 sample;
    u32 r, g, b;
    u8 outR, outG, outB;
    s32 previous;
    s32 group;
    s32 i;
    u8 steps;

    vertices = (Vtx *)arg0->modelData[arg0->current_lod][gDrawBufferIndex]; // the model's vertex copy
    if (vertices == NULL) {
        return;
    }
    previous = -1;
    for (i = 0; i < gModelVertexCounts[arg0->character_type]; i++) {
        if (vertices[i].v.flag & 0x7F00) {
            group = (vertices[i].v.flag & 0x7F00) >> 8;
            if (group != previous) {
                if (group >= 6) {
                    continue;
                }
                previous = group;
                group--;
                steps = gVtxRampLengths[group];
                interval = 1.0f / (steps - 1);
                if (arg2 != 0) {
                    value = arg1[group];
                } else {
                    value = *arg1;
                }
                sample = value / interval;
                first = &gVtxRamps[group][sample];
                second = &gVtxRamps[group][sample + 1];
                fraction = (value - (u32)sample * interval) / interval;
                r = first->r;
                outR = (s32)(second->r - r) * fraction + r;
                g = first->g;
                outG = (s32)(second->g - g) * fraction + g;
                b = first->b;
                outB = (s32)(second->b - b) * fraction + b;
            }
            vertices[i].v.cn[0] = outR;
            vertices[i].v.cn[1] = outG;
            vertices[i].v.cn[2] = outB;
        }
    }
}
