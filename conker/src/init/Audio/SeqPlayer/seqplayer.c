#include <ultra64.h>
#include <n_libaudio.h>

#define NUM_SEQ_PLAYERS 3
#define NUM_SEQUENCES   150

/* This file's .bss, defined before conker/audio.h declares it (see init/Sched/SchedInit/schedinit.c). */
static N_ALCSPlayer *gSeqPlayers[NUM_SEQ_PLAYERS]; // sequence players
static u16           gSeqLengths[NUM_SEQUENCES];   // sequence lengths, rounded up to even
static u16           gSeqPlayerSeqIds[NUM_SEQ_PLAYERS]; // sequence loaded in each player (0xFFFF: none)
static u8           *gSeqPlayerBuffers[NUM_SEQ_PLAYERS]; // each player's sequence data buffer
static ALCSeq        gSeqPlayerSeqs[NUM_SEQ_PLAYERS];
static ALSeqFile    *gSeqFile;                  // sequence table
static ALCSeqMarker  gSeqMarkers[NUM_SEQ_PLAYERS][8];
ALBank              *gSoundBank;                  // sound bank
ALHeap               gAudioHeap;                  // audio heap

#include "conker/audio.h"
#include "conker/heap.h"
#include "conker/rom.h"
#include "macros.h"

/* Initialise the audio heap, synthesis driver, sound bank, sequence table, sequence players and sound player. */
void audio_init(void) {
    SndPlayerConfig soundConfig;
    s32 bankSize;
    u32 bankAddress;
    SynDriverConfig driverConfig;
    s32 sequenceSize;
    ALSeqpConfig playerConfig;
    AudioMgrConfig deviceConfig;
    ALBankFile *bankFile;
    ALSeqFile *seqHeader;
    u32 sequenceAddress;
    u8 *waveBase;
    s32 i;

    alHeapInit(&gAudioHeap, D_80044B20, 0x3E000);
    driverConfig.maxVVoices = 44;
    driverConfig.maxPVoices = 40;
    driverConfig.maxUpdates = 64;
    driverConfig.maxFXbusses = 2;
    driverConfig.dma = NULL;
    driverConfig.fxType[0] = 6;
    driverConfig.fxType[1] = 6;
    driverConfig.outputRate = 0;
    driverConfig.heap = &gAudioHeap;
    deviceConfig.frequency = 22020;
    deviceConfig.framesPerField = 1;
    deviceConfig.maxACMDSize = 0xC00;
    driverConfig.waveRom = rom_findPath(0, 2, 0x17, 2);
    audio_createAudioMgr(&driverConfig, 12, &deviceConfig);

    // load the sound bank file
    bankSize = rom_getPathSize(2, 0x17, 0);
    bankFile = allocate_memory(bankSize, 0xFF, 2, 0);
    rom_loadPathInto(bankFile, bankSize, 2, 0x17, 0);
    bankAddress = (u32)rom_findPath(0, 2, 0x17, 1);
    waveBase = rom_findPath(0, 2, 0x17, 2);
    func_10012934(bankFile, waveBase, bankAddress);
    gSoundBank = bankFile->bankArray[0];

    // read the sequence table header for its count, then the whole table
    sequenceAddress = (u32)rom_findPath(0, 2, 0x17, 3);
    seqHeader = allocate_memory(0x10, 1, 2, 0);
    rom_readLarge(sequenceAddress, seqHeader, 0x10);
    sequenceSize = seqHeader->seqCount * sizeof(ALSeqData) + 4;
    heap_free(seqHeader);
    gSeqFile = allocate_memory(sequenceSize, 0xFF, 2, 0);
    rom_readLarge(sequenceAddress, gSeqFile, ALIGN16(sequenceSize));
    alSeqFileNew(gSeqFile, (u8 *)sequenceAddress);
    for (i = 0; i < NUM_SEQUENCES; i++) {
        gSeqLengths[i] = gSeqFile->seqArray[i].len;
        if (gSeqLengths[i] & 1) {
            gSeqLengths[i]++;
        }
    }

    playerConfig.maxVoices = 44;
    playerConfig.maxEvents = 104;
    playerConfig.debugFlags = 0;
    playerConfig.maxChannels = 16;
    playerConfig.heap = &gAudioHeap;
    func_100131FC(&playerConfig, 88);
    for (i = 0; i < NUM_SEQ_PLAYERS; i++) {
        gSeqPlayerSeqIds[i] = 0xFFFF;
        gSeqPlayerBuffers[i] = NULL;
        gSeqPlayers[i] = alHeapAlloc(&gAudioHeap, 1, sizeof(N_ALCSPlayer));
        n_alCSPNew(gSeqPlayers[i], &playerConfig);
        func_10015550(gSeqPlayers[i], bankFile->bankArray[0]);
    }

    soundConfig.maxEvents = 64;
    soundConfig.maxStates = 64;
    soundConfig.maxSounds = 20;
    soundConfig.maxVolumes = 8;
    soundConfig.heap = &gAudioHeap;
    soundConfig.waveRom = waveBase;
    func_100155A0(&soundConfig);
    func_10017870(4);
    func_10017944(0, 2);
    func_10017944(1, 2);
}

/* Stop the player's sequence unless it is already stopped or stopping. */
void func_100084D8(u8 player) {
    if (n_alCSPGetState(gSeqPlayers[player]) == AL_STOPPED ||
        n_alCSPGetState(gSeqPlayers[player]) == 3) {
        func_10017AA0(gSeqPlayers[player]);
    }
}

s32 audio_getSeqState(u8 player) {
    return n_alCSPGetState(gSeqPlayers[player]);
}

void audio_setSeqMsgQueue(u8 player, OSMesgQueue *queue) {
    func_10017AF0(gSeqPlayers[player], queue);
}

void func_100085A4(s32 arg0, s32 arg1, s32 arg2) {
}

void func_100085B8(u8 player, s32 chan, u8 arg2) {
    func_10017B04(gSeqPlayers[player], chan, arg2);
}

void audio_muteChannel(u8 player, s32 chan) {
    func_10017BB8(gSeqPlayers[player], chan);
}

void audio_unmuteChannel(u8 player, s32 chan) {
    func_10017C00(gSeqPlayers[player], chan);
}

/* Fade a channel to `volume`; `time` is scaled by 10/60 and clamped to 1..127 (0 if not positive). */
void audio_fadeChannelVolume(u8 player, u8 chan, u8 volume, s32 time) {
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
    func_10017C68(gSeqPlayers[player], chan, volume, time);
}

void func_100086FC(u8 player, u8 chan, u8 arg2) {
    func_10017CE0(gSeqPlayers[player], chan, arg2);
}

void audio_setChannelPan(u8 player, u8 chan, u8 pan) {
    func_10017D80(gSeqPlayers[player], chan, pan);
}

/* audio_fadeChannelVolume for every channel in `mask`. */
void audio_fadeChannelsVolume(u8 player, s32 mask, u8 volume, s32 time) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            audio_fadeChannelVolume(player, chan, volume, time);
        }
    }
}

void func_10008824(u8 player, u8 chan, u8 arg2) {
    func_10017D30(gSeqPlayers[player], chan, arg2);
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

/* Unmute (audio_unmuteChannel) or mute (audio_muteChannel) every channel in `mask`. */
void audio_setChannelsEnabled(u8 player, s32 mask, s32 enable) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            if (enable) {
                audio_unmuteChannel(player, chan);
            } else {
                audio_muteChannel(player, chan);
            }
        }
    }
}

/* Set or clear the channels in `mask` in the player's active channel mask. */
void audio_setChannelMask(u8 player, s32 mask, s32 enable) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            if (enable) {
                gSeqPlayers[player]->chanMask |= mask;
            } else {
                gSeqPlayers[player]->chanMask &= mask ^ 0xFFFF;
            }
        }
    }
}

u8 func_10008A4C(u8 player, u8 chan) {
    return gSeqPlayers[player]->chanState[chan].unkD;
}

/* func_10017E4C for every channel in `mask`. */
void func_10008A94(u8 player, s32 mask, s32 arg2) {
    s32 chan;

    for (chan = 0; chan < 16; chan++) {
        if ((1 << chan) & mask) {
            func_10017E4C(gSeqPlayers[player], chan, arg2);
        }
    }
}

void func_10008B2C(u8 player) {
    n_alCSPGetTempo(gSeqPlayers[player]);
}

void func_10008B60(u8 player, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    func_10017F10(gSeqPlayers[player], arg1, arg2, arg3, arg4);
}

void func_10008BC0(u8 player, f32 arg1, f32 arg2) {
    func_10017DF0(gSeqPlayers[player], arg1, arg2);
}

/* Scan the player's sequence for `count` loop markers starting at number `base`. */
void audio_findSeqLoopMarkers(u8 player, u8 count, s32 base) {
    func_10018790(&gSeqPlayerSeqs[player], gSeqMarkers[player], count, base);
}

/* Jump the player's sequence to one of its saved markers. */
void audio_jumpToSeqMarker(u8 player, u8 marker) {
    func_100186DC(&gSeqPlayerSeqs[player], &gSeqMarkers[player][marker]);
}

/* Stop the player, load `sequence` into its buffer if it isn't loaded already, and start playing it. */
s32 audio_playSequence(u8 player, s32 sequence) {
    u8 *romAddress;
    u32 count;

    // wait (bounded) for the player to stop
    count = 0;
    func_10018C60(gSeqPlayers[player]);
    while (n_alCSPGetState(gSeqPlayers[player]) != AL_STOPPED && count < 2000000) {
        count++;
    }
    if (count >= 2000000) {
        func_10018C60(gSeqPlayers[player]);
        while (n_alCSPGetState(gSeqPlayers[player]) != AL_STOPPED && count < 4000000) {
            count++;
        }
    }

    if (sequence != gSeqPlayerSeqIds[player]) {
        if (gSeqPlayerBuffers[player] != NULL) {
            // matching: the integer round-trip makes IDO load the pointer into v0 and copy it to a0
            heap_free((void *)(s32)gSeqPlayerBuffers[player]);
            gSeqPlayerBuffers[player] = NULL;
        }
        romAddress = gSeqFile->seqArray[sequence].offset;
        gSeqPlayerBuffers[player] = allocate_memory(gSeqLengths[sequence], 0xFF, 2, 2);
        if (gSeqPlayerBuffers[player] == NULL) {
            return -1;
        }
        rom_read((u32)romAddress, gSeqPlayerBuffers[player], ALIGN16(gSeqLengths[sequence]), 1);
        gSeqPlayerSeqIds[player] = sequence;
    }
    n_alCSeqNew(&gSeqPlayerSeqs[player], gSeqPlayerBuffers[player]);
    func_10018CB0(gSeqPlayers[player], &gSeqPlayerSeqs[player]);
    func_10017B30(gSeqPlayers[player]);
    return 0;
}

void audio_setSeqVolume(u8 player, s32 vol) {
    func_10018D00(gSeqPlayers[player], vol);
}

void func_10008F24(u8 player) {
    // AL_TRACK_END
    func_10018C60(gSeqPlayers[player]);
}

void audio_stopSeq(u8 player) {
    func_10018D50(gSeqPlayers[player]);
}
