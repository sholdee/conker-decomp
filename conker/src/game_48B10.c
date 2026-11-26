#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct Game48B10Vertex {
    s16 position[3];
    u16 flags;
    s16 uv[2];
    u8 r, g, b, a;
} Game48B10Vertex;
typedef struct Game48B10Color { u8 r, g, b; } Game48B10Color;

/* The number of colours in the ramp of each vertex group (groups 1..5). */
static const u8 D_80096920[] = { 4, 4, 3, 3, 2 };
/* The colour ramps. */
static const Game48B10Color D_80096928[4] = {
    { 0xFF, 0x64, 0x00 }, { 0xFF, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF },
};
static const Game48B10Color D_80096934[4] = {
    { 0xFF, 0x5A, 0x00 }, { 0xFF, 0x1E, 0x00 }, { 0xFF, 0xFF, 0xFF },
};
static const Game48B10Color D_80096940[4] = {
    { 0xB4, 0x3C, 0x00 }, { 0xB4, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF },
};
static const Game48B10Color D_8009694C[4] = {
    { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 },
};
/* The colour ramp of each vertex group. */
static const Game48B10Color *D_80084040[] = { D_80096928, D_80096928, D_80096934, D_80096940, D_8009694C };

void func_1501B660(Actor *arg0, f32 *arg1, s32 arg2) {
    Game48B10Vertex *vertices;
    const Game48B10Color *first;
    const Game48B10Color *second;
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

    vertices = (Game48B10Vertex *)arg0->modelData[arg0->current_lod][D_800BE9C0]; // the model's vertex copy
    if (vertices == NULL) {
        return;
    }
    previous = -1;
    for (i = 0; i < D_800C57A0[arg0->character_type]; i++) {
        if (vertices[i].flags & 0x7F00) {
            group = (vertices[i].flags & 0x7F00) >> 8;
            if (group != previous) {
                if (group >= 6) {
                    continue;
                }
                previous = group;
                group--;
                steps = D_80096920[group];
                interval = 1.0f / (f32) (steps - 1);
                if (arg2 != 0) {
                    value = arg1[group];
                } else {
                    value = *arg1;
                }
                sample = value / interval;
                first = &D_80084040[group][sample];
                second = &D_80084040[group][sample + 1];
                fraction = (value - (f32) (u32) sample * interval) / interval;
                r = first->r;
                outR = (f32) (s32) (second->r - r) * fraction + (f32) r;
                g = first->g;
                outG = (f32) (s32) (second->g - g) * fraction + (f32) g;
                b = first->b;
                outB = (f32) (s32) (second->b - b) * fraction + (f32) b;
            }
            vertices[i].r = outR;
            vertices[i].g = outG;
            vertices[i].b = outB;
        }
    }
}
