#include <ultra64.h>

#include "functions.h"
#include "variables.h"



void mp3_initVoice(ALHeap *arg0) {
    bzero(&D_800E0D80, 0xA0);
    D_800E0D80.unk8 = alHeapDBAlloc(NULL, 0, arg0, 1, 0x50);
    D_800E0D80.unk74[0] = alHeapDBAlloc(NULL, 0, arg0, 1, 0x440);
    D_800E0D80.unk5C = alHeapDBAlloc(NULL, 0, arg0, 1, 8);
    D_800E0D80.em_pan = 0x40;
    D_800E0D80.unk96 = 0x40;
    D_800E0D80.unk84 = 0;
    *(u32 *)&D_800E0D80.unk88 = 0x7FFF; // matching: a plain store schedules differently
    D_800E0D80.unk9A = 0;
    D_800E0D80.unk32 = 0;
    D_800E0D80.unk30 = 0;
    D_800E0D80.unk94 = D_800E0D80.unk96;
    D_800E0D80.em_volume = 0x7FFF;
    D_800E0D80.em_cvolL = (n_eqpower[D_800E0D80.em_pan] * D_800E0D80.em_volume) >> 15;
    D_800E0D80.em_cvolR = (n_eqpower[127 - D_800E0D80.em_pan] * D_800E0D80.em_volume) >> 15;
    D_800E0D80.unk24 = 1;
    D_800E0D80.unk14 = 0x7FFC;
    D_800E0D80.unk16 = 0;
    func_151F3C34(func_151F3C4C, D_800E0D80.em_volume, 0x40);
}

