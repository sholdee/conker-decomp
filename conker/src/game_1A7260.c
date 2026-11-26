#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Indices of the wave vertices in each buffer. */
static const u8 gWaveVertIndices[7] = { 4, 9, 13, 14, 15, 17, 19 };

/* Animates the wave vertices of slot `slot`: both buffers bob with a sine of their own phase. */
void func_15179DB0(u8 slot) {
    s32 i;
    s32 j;
    s16 height;
    Vtx *vtx[2];
    f32 *angle[2];
    f32 *rate[2];

    vtx[1] = gEffectModelSlots[slot].unk4;
    vtx[0] = gEffectModelSlots[slot].unk8;
    angle[0] = &gSwarmFlights[slot].u.bob.angle[0];
    angle[1] = &gSwarmFlights[slot].u.bob.angle[1];
    rate[0] = &gSwarmFlights[slot].u.bob.rate[0];
    rate[1] = &gSwarmFlights[slot].u.bob.rate[1];
    for (i = 0; i < 2; i++) {
        height = gSwarmFlights[slot].u.bob.amplitude * sinf(*angle[i] * 0.01745329238f);
        vtx[i][2].v.ob[0] = height;
        vtx[i][3].v.ob[0] = height;
        height = -(height >> 1);
        for (j = 0; j < 7; j++) {
            vtx[i][gWaveVertIndices[j]].v.ob[0] = height;
        }
        vtx[i][6].v.ob[0] = height + 18;
        vtx[i][8].v.ob[0] = height - 18;
        *angle[i] += *rate[i];
        if (*angle[i] >= 360.0f) {
            *angle[i] -= 360.0f;
        }
    }
}
