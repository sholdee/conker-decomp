#include <ultra64.h>
#include "conker/heap.h"
#include "conker/prop.h"



void prop_clearWaveChains(void) {
    gNumWaveChains = 0;
}

void prop_startWaveChain(s32 presetId) {
    s32 i;
    f32 zero;
    WavePreset *entry;

    gWaveChainPresetIds[gNumWaveChains] = presetId;
    gWaveChainSamples[gNumWaveChains] = allocate_memory(40 * sizeof(WaveSample), 1, 0, 0);

    // matching: the clear loop is unrolled by hand, four samples (0x30 bytes) per step of the byte offset i
#define SAMPLES ((WaveSample *)((u8 *)gWaveChainSamples[gNumWaveChains] + i))
    for (i = 0; i != 40 * sizeof(WaveSample); i += 4 * sizeof(WaveSample)) {
        SAMPLES[0].accel = 0.0f;
        zero = SAMPLES[0].accel;
        SAMPLES[0].vel = zero;
        SAMPLES[0].pos = zero;
        SAMPLES[1].accel = 0.0f;
        zero = SAMPLES[1].accel;
        SAMPLES[1].vel = zero;
        SAMPLES[1].pos = zero;
        SAMPLES[2].accel = 0.0f;
        zero = SAMPLES[2].accel;
        SAMPLES[2].vel = zero;
        SAMPLES[2].pos = zero;
        SAMPLES[3].accel = 0.0f;
        zero = SAMPLES[3].accel;
        SAMPLES[3].vel = zero;
        SAMPLES[3].pos = zero;
    }
#undef SAMPLES

    entry = &gWaveChainPresets[presetId];
    gWaveChainSamples[gNumWaveChains][39].pos = entry->initialPos;
    entry->soundTimer = 0;
    gNumWaveChains++;
}
