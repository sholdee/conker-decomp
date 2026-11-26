#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* The level's rope (higher end first). */
static const RopeEnds D_80096430 = { { -2495.0f, 729.0f, -906.0f }, { -2495.0f, 129.0f, -906.0f } };

void func_15010780(void) {
    prop_createWaveDeformer(1, 255,    0, 108, 0, 0.0f, 15000, 80.0f, 77, 15, 127, 127, 127, 127, 127, 127);
    prop_createWaveDeformer(0, 255, 6628, 108, 0, 0.0f, 15000, 80.0f, 77, 15, 127, 127, 127, 127, 127, 127);
}

void func_15010880(void) {
    ZonePairEmitter tmp;
    TimedEffect *temp_v0;
    TriggerZone *base;

    prop_createWaveDeformer(0x10, 0xE9, -0x1B5, 0x463, -0xCBF, 90.0f, 0x3A98, 36.0f, 0x4D, 0xF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F);
    D_800E0934 = func_150DE32C;
dummy_label_15010880:
    base = zones;

    tmp.timer = 0;
    tmp.source = &base[72];
    tmp.target = &base[73];

    temp_v0 = effect_newTimedEffect(0x12C, -1, 0x5E, -1, 0, 0,  0xC, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy(temp_v0 + 1, &tmp, sizeof(tmp));
    }
    if (gTaskDoneBits[0x12] & 0x40) {
        effect_newTimedEffect(5, 9, -1, -1, 1, 0, NULL, 0xFF, 1);
    }
}
// NON-MATCHING: ops in wrong order
// void func_15010880(void) {
//     ZonePairEmitter tmp;
//     TimedEffect *temp_v0;

//     prop_createWaveDeformer(0x10, 0xE9, -437, 0x463, -0xCBF, 90.0f, 15000, 36.0f, 0x4D, 15, 127, 127, 127, 127, 127, 127);
//     D_800E0934 = func_150DE32C;
    
//     tmp.unk0 = 0;
//     tmp.unk4 = *((s32*) &zones) + 0xEA0;
//     tmp.unk8 = *((s32*) &zones) + 0xED4;

//     temp_v0 = effect_newTimedEffect(300, -1, 94, -1, 0, 0, 12, 255, 1);
//     if (temp_v0 != NULL) {
//         memcpy(&temp_v0->unk28, &tmp, 12); // memcpy
//     }
//     if ((gTaskDoneBits[0x12] & 0x40) != 0) {
//         effect_newTimedEffect(5, 9, -1, -1, 1, 0, 0, 255, 1);
//     }
// }

void func_150109D0(void) {
    RopeSpawn tmp;

    tmp.unk20 = 0;  
    tmp.ends = D_80096430;

    tmp.id = 1;
    tmp.length = fabsf(tmp.ends.top.y - tmp.ends.bottom.y); 
    func_151ACBD4(&tmp, 0);
    func_150E8854();
}
