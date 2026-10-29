#include <n_libaudio.h>

#include "macros.h"

typedef struct {
  u8 pad0[0x760];
} struct247; // something naudio related?

extern N_ALCSPlayer *D_8003C900[];
extern ALCSeq        D_8003CA58[];
extern struct247     D_8003CD48[];

// FIXME: create header file for audio related functions
s32  func_10017A80(N_ALCSPlayer *csp);
void func_10017AF0(N_ALCSPlayer *csp, s32 arg1);
void func_10017B04(N_ALCSPlayer *arg0, s32 arg1, u8 arg2);
void func_10017B30(N_ALCSPlayer *csp);
void func_10017BB8(N_ALCSPlayer *csp, s32 arg1);
void func_10017C00(N_ALCSPlayer *csp, s32 arg1);
void func_10017C68(N_ALCSPlayer *arg0, s32 arg1, u8 arg2, u8 arg3);
void func_10017CE0(N_ALCSPlayer *arg0, s32 arg1, u8 arg2);
void func_10017D30(N_ALCSPlayer *arg0, s32 arg1, u8 arg2);
void func_10017D80(N_ALCSPlayer *arg0, u8 arg1, u8 arg2);
void func_10017DF0(N_ALCSPlayer *csp, f32 arg1, f32 arg2);
void func_10017E4C(N_ALCSPlayer *csp, u8 chan, u8 arg2);
void func_10017F10(N_ALCSPlayer *arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_100186DC(void *arg0, void *arg1);
void func_10018790(N_ALCSPlayer *arg0, s32 arg1, u32 arg2, u32 arg3);
void func_10018D00(N_ALCSPlayer *arg0, s16 arg1);
void func_10018D50(N_ALCSPlayer *seqp);

// this one is a monster
typedef struct {
    u8 *address;
    s32 length;
} SequenceFileEntry_8CE8;

typedef struct {
    s16 revision;
    s16 count;
    SequenceFileEntry_8CE8 entries[1];
} SequenceFile_8CE8;

typedef struct AudioBank_8180 AudioBank_8180;

typedef struct {
    s16 revision;
    s16 count;
    AudioBank_8180 *banks[1];
} AudioBankFile_8180;

typedef struct {
    s32 maxVoices;
    s32 maxPhysicalVoices;
    s32 maxUpdates;
    s32 maxFxBuses;
    void *dma;
    void *fetch;
    void *release;
    void *retain;
    void *releaseNow;
    void *waveBase;
    ALHeap *heap;
    s32 outputRate;
    u8 fxTypes[2];
    u8 pad32[2];
    s32 *params[2];
} AudioDriverConfig_8180;

typedef struct {
    u32 frequency;
    u32 frames;
    s32 maxCommands;
} AudioDeviceConfig_8180;

typedef struct {
    s32 maxStates;
    s32 maxEvents;
    s32 maxSounds;
    ALHeap *heap;
    void *waveBase;
    u16 maxVolumes;
} AudioSoundConfig_8180;

void func_100046E4(u32, void *, u32);
void func_10008F90(AudioDriverConfig_8180 *, s32, AudioDeviceConfig_8180 *);
void func_10012934(AudioBankFile_8180 *, u8 *, s32);
void func_100131FC(ALSeqpConfig *, s32);
void func_10015550(N_ALCSPlayer *, AudioBank_8180 *);
void func_100155A0(AudioSoundConfig_8180 *);
void func_10017870(u8);
void func_10017944(s32, s32);
void *func_1502B020(s32 *, s32, ...);
s32 func_1502B8E0(void *, s32, s32, ...);
s32 func_1502B9B4(s32, ...);
extern AudioBank_8180 *D_8003E368;
extern ALHeap D_8003E370;
extern u8 D_80044B20[];
s32 allocate_memory(s32, s32, s32, s32);
void func_10004074(void *);
extern u8 D_8003C90C[];
extern u16 D_8003C910[];
extern u16 D_8003CA3C[];
extern s32 D_8003CA48[];
extern SequenceFile_8CE8 *D_8003CD40;

void func_10008180(void) {
    AudioSoundConfig_8180 soundConfig;
    s32 bankSize;
    u32 bankAddress;
    AudioDriverConfig_8180 driverConfig;
    s32 sequenceSize;
    ALSeqpConfig playerConfig;
    AudioDeviceConfig_8180 deviceConfig;
    AudioBankFile_8180 *bankFile;
    SequenceFile_8CE8 *header;
    u32 sequenceAddress;
    u8 *waveBase;
    s32 i;

    alHeapInit(&D_8003E370, D_80044B20, 0x3E000);
    driverConfig.maxVoices = 0x2C;
    driverConfig.maxPhysicalVoices = 0x28;
    driverConfig.maxUpdates = 0x40;
    driverConfig.maxFxBuses = 2;
    driverConfig.dma = 0;
    driverConfig.fxTypes[0] = 6;
    driverConfig.fxTypes[1] = 6;
    driverConfig.outputRate = 0;
    driverConfig.heap = &D_8003E370;
    deviceConfig.frequency = 0x5604;
    deviceConfig.frames = 1;
    deviceConfig.maxCommands = 0xC00;
    driverConfig.waveBase = func_1502B020(0, 2, 0x17, 2);
    func_10008F90(&driverConfig, 0xC, &deviceConfig);
    bankSize = func_1502B9B4(2, 0x17, 0);
    bankFile = (AudioBankFile_8180 *)allocate_memory(bankSize, 0xFF, 2, 0);
    func_1502B8E0(bankFile, bankSize, 2, 0x17, 0);
    bankAddress = (u32)func_1502B020(0, 2, 0x17, 1);
    waveBase = func_1502B020(0, 2, 0x17, 2);
    func_10012934(bankFile, waveBase, bankAddress);
    D_8003E368 = bankFile->banks[0];
    sequenceAddress = (u32)func_1502B020(0, 2, 0x17, 3);
    header = (SequenceFile_8CE8 *)allocate_memory(0x10, 1, 2, 0);
    func_100046E4(sequenceAddress, header, 0x10);
    sequenceSize = header->count * 8 + 4;
    func_10004074(header);
    D_8003CD40 = (SequenceFile_8CE8 *)allocate_memory(sequenceSize, 0xFF, 2, 0);
    func_100046E4(sequenceAddress, D_8003CD40, (sequenceSize + 0xF) & ~0xF);
    alSeqFileNew((ALSeqFile *)D_8003CD40, (u8 *)sequenceAddress);
    for (i = 0; i < 150; i++) {
        D_8003C910[i] = D_8003CD40->entries[i].length;
        if (D_8003C910[i] & 1) {
            D_8003C910[i]++;
        }
    }
    playerConfig.maxVoices = 0x2C;
    playerConfig.maxEvents = 0x68;
    playerConfig.debugFlags = 0;
    playerConfig.maxChannels = 0x10;
    playerConfig.heap = &D_8003E370;
    func_100131FC(&playerConfig, 0x58);
    i = 0;
    do {
        D_8003CA3C[i] = 0xFFFF;
        D_8003CA48[i] = 0;
        D_8003C900[i] = alHeapDBAlloc(0, 0, &D_8003E370, 1, 0x90);
        n_alCSPNew(D_8003C900[i], &playerConfig);
        func_10015550(D_8003C900[i], bankFile->banks[0]);
        i++;
    } while ((N_ALCSPlayer **)D_8003C90C != &D_8003C900[i]);
    soundConfig.maxEvents = 0x40;
    soundConfig.maxStates = 0x40;
    soundConfig.maxSounds = 0x14;
    soundConfig.maxVolumes = 8;
    soundConfig.heap = &D_8003E370;
    soundConfig.waveBase = waveBase;
    func_100155A0(&soundConfig);
    func_10017870(4);
    func_10017944(0, 2);
    func_10017944(1, 2);
}

void func_100084D8(u8 idx) {
    if ((n_alCSPGetState(D_8003C900[idx]) == 0) || (n_alCSPGetState(D_8003C900[idx]) == 3)) {
        func_10017AA0(D_8003C900[idx]);
    }
}

s32 func_1000853C(u8 idx) {
    return n_alCSPGetState(D_8003C900[idx]);
}

void func_10008570(u8 idx, s32 arg1) { // arg1 is OSMesgQueue ?
    func_10017AF0(D_8003C900[idx], arg1);
}

void func_100085A4(s32 arg0, s32 arg1, s32 arg2) {
}

void func_100085B8(u8 idx, s32 arg1, u8 arg2) {
    func_10017B04(D_8003C900[idx], arg1, arg2);
}

void func_100085F8(u8 idx, s32 arg1) {
    func_10017BB8(D_8003C900[idx], arg1);
}

void func_1000862C(u8 idx, s32 arg1) {
    func_10017C00(D_8003C900[idx], arg1);
}

void func_10008660(u8 idx, u8 chan, u8 arg2, s32 arg3) {
    if (arg3 > 0) {
        arg3 = (arg3 * 10) / 60;
        if (arg3 == 0) {
            arg3 = 1; // final?
        } else if (arg3 >= 128) {
            arg3 = 127; // more to come?
        }
    } else {
        arg3 = 0; // empty?
    }
    func_10017C68(D_8003C900[idx], chan, arg2, arg3);
}

void func_100086FC(u8 idx, u8 arg1, u8 arg2) {
    func_10017CE0(D_8003C900[idx], arg1, arg2);
}

void func_10008744(u8 idx, u8 arg1, u8 arg2) {
    func_10017D80(D_8003C900[idx], arg1, arg2);
}

void func_10008790(u8 idx, s32 mask, u8 arg2, s32 arg3) {
    s32 chan;

    for (chan = 0; chan < 16; chan++)
    {
        if ((1 << chan) & mask) {
            func_10008660(idx, chan, arg2, arg3);
        }
    }
}

void func_10008824(u8 idx, u8 arg1, u8 arg2) {
    func_10017D30(D_8003C900[idx], arg1, arg2);
}

void func_1000886C(u8 idx, s32 mask, u8 arg2) {
    s32 chan;

    for (chan = 0; chan < 16; chan++)
    {
        if ((1 << chan) & mask) {
            func_10008824(idx, chan, arg2);
        }
    }
}

void func_100088F0(u8 idx, s32 mask, s32 enable) {
    s32 chan;

    for (chan = 0; chan < 16; chan++)
    {
        if ((1 << chan) & mask) {
            if (enable) {
                func_1000862C(idx, chan);
            } else {
                func_100085F8(idx, chan);
            }
        }
    }
}

void func_10008988(u8 idx, s32 mask, s32 enable) {
    s32 chan;

    for(chan = 0; chan < 16; chan++) // 16 channels
    {
        if ((1 << chan) & mask) {
            if (enable != 0) {
                D_8003C900[idx]->chanMask |= mask; // enable
            } else {
                D_8003C900[idx]->chanMask &= (mask ^ 0xFFFF); // disable
            }
        }
    }
}

// is this n_alCSPGetChlVol ?
u8 func_10008A4C(u8 idx, u8 chan) {
    return D_8003C900[idx]->chanState[chan].unkD; // do we assume this is volume?
}

void func_10008A94(u8 idx, s32 mask, s32 arg2) {
    s32 chan;

    for(chan = 0; chan < 16; chan++)
    {
        if (((1 << chan) & mask) != 0) {
            func_10017E4C(D_8003C900[idx], chan, arg2);
        }
    }
}

void func_10008B2C(u8 idx) {
      n_alCSPGetTempo(D_8003C900[idx]);
}

void func_10008B60(u8 idx, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    func_10017F10(D_8003C900[idx], arg1, arg2, arg3, arg4);
}

void func_10008BC0(u8 idx, f32 arg1, f32 arg2) {
    func_10017DF0(D_8003C900[idx], arg1, arg2);
}

void func_10008C04(u8 idx, u8 arg1, s32 arg2) {
    func_10018790(&D_8003CA58[idx], &D_8003CD48[idx], arg1, arg2);
}

void func_10008C6C(u8 idx, u8 arg1) {
    func_100186DC(&D_8003CA58[idx], &D_8003CD48[idx].pad0[arg1 * 0xEC]);
}

s32 allocate_memory(s32, s32, s32, s32);
void func_10004074(void *);
void func_10004514(u32, void *, u32, s32);
void func_10018C60(N_ALCSPlayer *);
void func_10018CB0(N_ALCSPlayer *, ALCSeq *);
extern u16 D_8003C910[];
extern u16 D_8003CA3C[];
extern s32 D_8003CA48[];
extern SequenceFile_8CE8 *D_8003CD40;

s32 func_10008CE8(u8 arg0, s32 sequence) {
    s32 address;
    u32 count;

    count = 0;
    func_10018C60(D_8003C900[arg0]);
    while (n_alCSPGetState(D_8003C900[arg0]) != 0 && count != 2000000) {
        count++;
    }
    if (count >= 2000000U) {
        func_10018C60(D_8003C900[arg0]);
        while (n_alCSPGetState(D_8003C900[arg0]) != 0 && count < 4000000U) {
            count++;
        }
    }
    if (sequence != D_8003CA3C[arg0]) {
        if (D_8003CA48[arg0] != 0) {
            func_10004074((void *)D_8003CA48[arg0]);
            D_8003CA48[arg0] = 0;
        }
        address = (s32)D_8003CD40->entries[sequence].address;
        D_8003CA48[arg0] = allocate_memory(D_8003C910[sequence], 0xFF, 2, 2);
        if (D_8003CA48[arg0] == 0) {
            return -1;
        }
        func_10004514(address, (void *)D_8003CA48[arg0],
                      (D_8003C910[sequence] + 0xF) & ~0xF, 1);
        D_8003CA3C[arg0] = sequence;
    }
    n_alCSeqNew(&D_8003CA58[arg0], (u8 *)D_8003CA48[arg0]);
    func_10018CB0(D_8003C900[arg0], &D_8003CA58[arg0]);
    func_10017B30(D_8003C900[arg0]);
    return 0;
}
// NON-MATCHING: 80% of the way there
// s32 func_10008CE8(u8 idx, u16 arg1) {
//     s32 sp3C;
//     ALCSeq *temp_s0_3;
//     u32 i;
//
//     i = 0;
//     func_10018C60(&D_8003C900[idx]);
//     while ((n_alCSPGetState(&D_8003C900[idx]) != 0) && (i < 2000000)) {
//         i++;
//     };
//
//     if (i >= 2000000) {
//         func_10018C60(&D_8003C900[idx]);
//         while ((n_alCSPGetState(&D_8003C900[idx]) != 0) && (i < 4000000)) {
//             i++;
//         }
//     }
//
//     if (arg1 != D_8003CA3C[idx]) {
//         if (D_8003CA48[idx] != NULL) {
//             func_10004074(&D_8003CA48[idx]); // de-init?
//             D_8003CA48[idx] = NULL;
//         }
//
//         sp3C = D_8003CD40[arg1].unk4;
//         temp_s0_3 = allocate_memory(&D_8003C910[arg1], 0xFF, 2, 2);
//         if (temp_s0_3 == NULL) {
//             return -1;
//         }
//         func_10004514(sp3C, temp_s0_3, ALIGN16(D_8003C910[arg1]), 1);
//         D_8003CA3C[idx] = arg1;
//       }
//
//     n_alCSeqNew(&D_8003CA58[idx], &D_8003CA48[idx]);
//     func_10018CB0(&D_8003C900[idx], &D_8003CA58[idx]);
//     func_10017B30(&D_8003C900[idx]);
//
//     return 0;
// }

void func_10008EE0(u8 idx, s32 arg1) {
    func_10018D00(D_8003C900[idx], arg1);
}

void func_10008F24(u8 idx) {
    // AL_TRACK_END
    func_10018C60(D_8003C900[idx]);
}

void func_10008F58(u8 idx) {
    func_10018D50(D_8003C900[idx]);
}
