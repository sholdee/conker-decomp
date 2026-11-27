#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void level_initTintWaves(arg0)
    s32 arg0;
{
    s32 i;
    s32 j;
    s32 idx;

    idx = gLevelHeader->tintPreset;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            gTintWavePhases[i][j] = 0;
            gTintWaveSteps[i][j] = gTintPresetSteps[gTintPresets[idx][0]][j];
            gTintWaveAmps[i][j] = gTintPresetAmps[gTintPresets[idx][1]][j];
            gTintDefaultTargets[j] = gTintPresetTargets[gTintPresets[idx][2]][j];
            gTintLevels[i][j] = gTintDefaultTargets[j];
            gTintTargetOverrides[i][j] = 0;
        }
    }
}
