#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_15008E00(void) {
    D_800DDE50 = 0;
}

void func_15008E10(s32 arg0) {
    s32 i;
    f32 temp;
    WavePreset *entry;

    D_800DDE54[D_800DDE50] = arg0;
    D_800DDE60[D_800DDE50] = allocate_memory(40 * sizeof(WaveSample), 1, 0, 0);

    // matching: the clear loop is unrolled by hand, four samples (0x30 bytes) per step of the byte offset i
#define SAMPLES ((WaveSample *)((u8 *)D_800DDE60[D_800DDE50] + i))
    for (i = 0; i != 40 * sizeof(WaveSample); i += 4 * sizeof(WaveSample)) {
        SAMPLES[0].accel = 0.0f;
        temp = SAMPLES[0].accel;
        SAMPLES[0].vel = temp;
        SAMPLES[0].pos = temp;
        SAMPLES[1].accel = 0.0f;
        temp = SAMPLES[1].accel;
        SAMPLES[1].vel = temp;
        SAMPLES[1].pos = temp;
        SAMPLES[2].accel = 0.0f;
        temp = SAMPLES[2].accel;
        SAMPLES[2].vel = temp;
        SAMPLES[2].pos = temp;
        SAMPLES[3].accel = 0.0f;
        temp = SAMPLES[3].accel;
        SAMPLES[3].vel = temp;
        SAMPLES[3].pos = temp;
    }
#undef SAMPLES

    entry = &D_8008D050[arg0];
    D_800DDE60[D_800DDE50][39].pos = entry->initialPos;
    entry->soundTimer = 0;
    D_800DDE50++;
}
