#include <ultra64.h>
#include <n_libaudio.h>

#include "macros.h"

#define NUM_SEQ_PLAYERS 3
#define NUM_SEQUENCES   150

/* Synthesis driver settings for func_10008F90 (SynDriverConfig in init_8F90.c). */
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

/* Audio output settings for func_10008F90 (AudioSpec in init_8F90.c). */
typedef struct {
    u32 frequency;
    u32 frames;
    s32 maxCommands;
} AudioDeviceConfig_8180;

/* Sound player settings for func_100155A0 (SndPlayerConfig in init_15550.c). */
typedef struct {
    s32 maxStates;
    s32 maxEvents;
    s32 maxSounds;
    ALHeap *heap;
    void *waveBase;
    u16 maxVolumes;
} AudioSoundConfig_8180;

/* This file's .bss. */
static N_ALCSPlayer *D_8003C900[NUM_SEQ_PLAYERS]; // sequence players
static u16           D_8003C910[NUM_SEQUENCES];   // sequence lengths, rounded up to even
static u16           D_8003CA3C[NUM_SEQ_PLAYERS]; // sequence loaded in each player (0xFFFF: none)
static u8           *D_8003CA48[NUM_SEQ_PLAYERS]; // each player's sequence data buffer
static ALCSeq        D_8003CA58[NUM_SEQ_PLAYERS];
static ALSeqFile    *D_8003CD40;                  // sequence table
static ALCSeqMarker  D_8003CD48[NUM_SEQ_PLAYERS][8];
ALBank              *D_8003E368;                  // sound bank
ALHeap               D_8003E370;                  // audio heap
extern u8            D_80044B20[];                 // audio heap memory

void *allocate_memory(s32, s32, s32, s32);
void func_10004074(void *);
void func_10004514(u32, void *, u32, s32);
void func_100046E4(u32, void *, u32);
void func_10008F90(AudioDriverConfig_8180 *, s32, AudioDeviceConfig_8180 *);
void func_10015550(N_ALCSPlayer *, ALBank *);
void func_100155A0(AudioSoundConfig_8180 *);
void func_10017870(u8);
void func_10017944(s32, s32);
void *func_1502B020(s32 *, s32, ...);
s32 func_1502B8E0(void *, s32, s32, ...);
s32 func_1502B9B4(s32, ...);


/* Initialise the audio heap, synthesis driver, sound bank, sequence table, sequence players and sound player. */
void func_10008180(void) {
    AudioSoundConfig_8180 soundConfig;
    s32 bankSize;
    u32 bankAddress;
    AudioDriverConfig_8180 driverConfig;
    s32 sequenceSize;
    ALSeqpConfig playerConfig;
    AudioDeviceConfig_8180 deviceConfig;
    ALBankFile *bankFile;
    ALSeqFile *seqHeader;
    u32 sequenceAddress;
    u8 *waveBase;
    s32 i;

    alHeapInit(&D_8003E370, D_80044B20, 0x3E000);
    driverConfig.maxVoices = 44;
    driverConfig.maxPhysicalVoices = 40;
    driverConfig.maxUpdates = 64;
    driverConfig.maxFxBuses = 2;
    driverConfig.dma = NULL;
    driverConfig.fxTypes[0] = 6;
    driverConfig.fxTypes[1] = 6;
    driverConfig.outputRate = 0;
    driverConfig.heap = &D_8003E370;
    deviceConfig.frequency = 22020;
    deviceConfig.frames = 1;
    deviceConfig.maxCommands = 0xC00;
    driverConfig.waveBase = func_1502B020(0, 2, 0x17, 2);
    func_10008F90(&driverConfig, 12, &deviceConfig);

    // load the sound bank file
    bankSize = func_1502B9B4(2, 0x17, 0);
    bankFile = allocate_memory(bankSize, 0xFF, 2, 0);
    func_1502B8E0(bankFile, bankSize, 2, 0x17, 0);
    bankAddress = (u32)func_1502B020(0, 2, 0x17, 1);
    waveBase = func_1502B020(0, 2, 0x17, 2);
    func_10012934(bankFile, waveBase, bankAddress);
    D_8003E368 = bankFile->bankArray[0];

    // read the sequence table header for its count, then the whole table
    sequenceAddress = (u32)func_1502B020(0, 2, 0x17, 3);
    seqHeader = allocate_memory(0x10, 1, 2, 0);
    func_100046E4(sequenceAddress, seqHeader, 0x10);
    sequenceSize = seqHeader->seqCount * sizeof(ALSeqData) + 4;
    func_10004074(seqHeader);
    D_8003CD40 = allocate_memory(sequenceSize, 0xFF, 2, 0);
    func_100046E4(sequenceAddress, D_8003CD40, ALIGN16(sequenceSize));
    alSeqFileNew(D_8003CD40, (u8 *)sequenceAddress);
    for (i = 0; i < NUM_SEQUENCES; i++) {
        D_8003C910[i] = D_8003CD40->seqArray[i].len;
        if (D_8003C910[i] & 1) {
            D_8003C910[i]++;
        }
    }

    playerConfig.maxVoices = 44;
    playerConfig.maxEvents = 104;
    playerConfig.debugFlags = 0;
    playerConfig.maxChannels = 16;
    playerConfig.heap = &D_8003E370;
    func_100131FC(&playerConfig, 88);
    for (i = 0; i < NUM_SEQ_PLAYERS; i++) {
        D_8003CA3C[i] = 0xFFFF;
        D_8003CA48[i] = NULL;
        D_8003C900[i] = alHeapAlloc(&D_8003E370, 1, sizeof(N_ALCSPlayer));
        n_alCSPNew(D_8003C900[i], &playerConfig);
        func_10015550(D_8003C900[i], bankFile->bankArray[0]);
    }

    soundConfig.maxEvents = 64;
    soundConfig.maxStates = 64;
    soundConfig.maxSounds = 20;
    soundConfig.maxVolumes = 8;
    soundConfig.heap = &D_8003E370;
    soundConfig.waveBase = waveBase;
    func_100155A0(&soundConfig);
    func_10017870(4);
    func_10017944(0, 2);
    func_10017944(1, 2);
}

/* Stop the player's sequence unless it is already stopped or stopping. */
void func_100084D8(u8 player) {
    if (n_alCSPGetState(D_8003C900[player]) == AL_STOPPED ||
        n_alCSPGetState(D_8003C900[player]) == 3) {
        func_10017AA0(D_8003C900[player]);
    }
}

s32 func_1000853C(u8 player) {
    return n_alCSPGetState(D_8003C900[player]);
}

void func_10008570(u8 player, OSMesgQueue *queue) {
    func_10017AF0(D_8003C900[player], queue);
}

void func_100085A4(s32 arg0, s32 arg1, s32 arg2) {
}

void func_100085B8(u8 player, s32 chan, u8 arg2) {
    func_10017B04(D_8003C900[player], chan, arg2);
}

void func_100085F8(u8 player, s32 chan) {
    func_10017BB8(D_8003C900[player], chan);
}

void func_1000862C(u8 player, s32 chan) {
    func_10017C00(D_8003C900[player], chan);
}

/* Fade a channel to `volume`; `time` is scaled by 10/60 and clamped to 1..127 (0 if not positive). */
void func_10008660(u8 player, u8 chan, u8 volume, s32 time) {
    if (time > 0) {
        time = (time * 10) / 60;
        if (time == 0) {
            time = 1;
        } else if (time >= 128) {
            time = 127;
        }
    } else {
        time = 0;
    }
    func_10017C68(D_8003C900[player], chan, volume, time);
}

void func_100086FC(u8 player, u8 chan, u8 arg2) {
    func_10017CE0(D_8003C900[player], chan, arg2);
}

void func_10008744(u8 player, u8 chan, u8 pan) {
    func_10017D80(D_8003C900[player], chan, pan);
}

/* func_10008660 for every channel in `mask`. */
void func_10008790(u8 player, s32 mask, u8 volume, s32 time) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            func_10008660(player, chan, volume, time);
        }
    }
}

void func_10008824(u8 player, u8 chan, u8 arg2) {
    func_10017D30(D_8003C900[player], chan, arg2);
}

/* func_10008824 for every channel in `mask`. */
void func_1000886C(u8 player, s32 mask, u8 arg2) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            func_10008824(player, chan, arg2);
        }
    }
}

/* Unmute (func_1000862C) or mute (func_100085F8) every channel in `mask`. */
void func_100088F0(u8 player, s32 mask, s32 enable) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            if (enable) {
                func_1000862C(player, chan);
            } else {
                func_100085F8(player, chan);
            }
        }
    }
}

/* Set or clear the channels in `mask` in the player's active channel mask. */
void func_10008988(u8 player, s32 mask, s32 enable) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            if (enable) {
                D_8003C900[player]->chanMask |= mask;
            } else {
                D_8003C900[player]->chanMask &= mask ^ 0xFFFF;
            }
        }
    }
}

u8 func_10008A4C(u8 player, u8 chan) {
    return D_8003C900[player]->chanState[chan].unkD;
}

/* func_10017E4C for every channel in `mask`. */
void func_10008A94(u8 player, s32 mask, s32 arg2) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            func_10017E4C(D_8003C900[player], chan, arg2);
        }
    }
}

void func_10008B2C(u8 player) {
    n_alCSPGetTempo(D_8003C900[player]);
}

void func_10008B60(u8 player, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    func_10017F10(D_8003C900[player], arg1, arg2, arg3, arg4);
}

void func_10008BC0(u8 player, f32 arg1, f32 arg2) {
    func_10017DF0(D_8003C900[player], arg1, arg2);
}

/* Scan the player's sequence for `count` loop markers starting at number `base`. */
void func_10008C04(u8 player, u8 count, s32 base) {
    func_10018790(&D_8003CA58[player], D_8003CD48[player], count, base);
}

/* Jump the player's sequence to one of its saved markers. */
void func_10008C6C(u8 player, u8 marker) {
    func_100186DC(&D_8003CA58[player], &D_8003CD48[player][marker]);
}

/* Stop the player, load `sequence` into its buffer if it isn't loaded already, and start playing it. */
s32 func_10008CE8(u8 player, s32 sequence) {
    u8 *romAddress;
    u32 count;

    // wait (bounded) for the player to stop
    count = 0;
    func_10018C60(D_8003C900[player]);
    while (n_alCSPGetState(D_8003C900[player]) != AL_STOPPED && count < 2000000) {
        count++;
    }
    if (count >= 2000000) {
        func_10018C60(D_8003C900[player]);
        while (n_alCSPGetState(D_8003C900[player]) != AL_STOPPED && count < 4000000) {
            count++;
        }
    }

    if (sequence != D_8003CA3C[player]) {
        if (D_8003CA48[player] != NULL) {
            // matching: the integer round-trip makes IDO load the pointer into v0 and copy it to a0
            func_10004074((void *)(s32)D_8003CA48[player]);
            D_8003CA48[player] = NULL;
        }
        romAddress = D_8003CD40->seqArray[sequence].offset;
        D_8003CA48[player] = allocate_memory(D_8003C910[sequence], 0xFF, 2, 2);
        if (D_8003CA48[player] == NULL) {
            return -1;
        }
        func_10004514((u32)romAddress, D_8003CA48[player], ALIGN16(D_8003C910[sequence]), 1);
        D_8003CA3C[player] = sequence;
    }
    n_alCSeqNew(&D_8003CA58[player], D_8003CA48[player]);
    func_10018CB0(D_8003C900[player], &D_8003CA58[player]);
    func_10017B30(D_8003C900[player]);
    return 0;
}

void func_10008EE0(u8 player, s32 vol) {
    func_10018D00(D_8003C900[player], vol);
}

void func_10008F24(u8 player) {
    // AL_TRACK_END
    func_10018C60(D_8003C900[player]);
}

void func_10008F58(u8 player) {
    func_10018D50(D_8003C900[player]);
}
