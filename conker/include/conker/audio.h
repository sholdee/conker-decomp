#ifndef CONKER_AUDIO_H
#define CONKER_AUDIO_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "audio": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Audio/SeqPlayer/seqplayer.c
 *   init/Audio/AudioMgr/audiomgr.c
 *   init/Audio/MixSelector/mixselector.c
 *   init/Audio/OutputMode/outputmode.c
 *   libultra/audio/n_seqplayer.c
 *   libultra/audio/n_event.c
 *   libultra/audio/n_syndelete.c
 *   libultra/audio/n_synallocfx.c
 *   libultra/audio/n_reverb.c
 *   libultra/audio/n_mainbus.c
 *   libultra/audio/n_env.c
 *   libultra/audio/n_auxbus.c
 *   init/Audio/AdpcmLoad/adpcmload.c
 *   init/Audio/VoiceLowPass/voicelowpass.c
 *   libultra/audio/n_resample.c
 */

/* Types that these files define; the declarations below use them. */
/* Sound bank loader (init/Audio/AudioMgr/audiomgr.c): a bank fetch proc takes an owner slot (a ROM reference, or a record
 * once loading has started) and returns the bank's data, loading it on demand. */
typedef struct BankRecord BankRecord;
typedef void *(*BankFetchProc)(u32 *slot, s32 mode);
/* Audio output settings for audio_createAudioMgr (init/Audio/SeqPlayer/seqplayer.c fills them in). */
typedef struct AudioMgrConfig {
    u32 frequency;
    u32 framesPerField;
    u32 maxACMDSize;
} AudioMgrConfig;
/* Synthesis driver settings for audio_createAudioMgr, with Conker's bank hooks. */
typedef struct SynDriverConfig {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    ALDMANew dma;
    BankFetchProc (*bankFetchNew)(void);
    void (*bankDecRef)(BankRecord *record);
    void (*bankAddRef)(BankRecord *record);
    void (*bankRelease)(BankRecord *record);
    void *waveRom;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType[4];
    s32 *params[2];
} SynDriverConfig;
typedef struct AudioInfo_s AudioInfo;

/* init/Audio/SeqPlayer/seqplayer.c */
void audio_init(void);
void audio_playSeqIfStopped(u8 player);
s32 audio_getSeqState(u8 player);
void audio_setSeqMsgQueue(u8 player, OSMesgQueue *queue);
void func_100085A4(s32 arg0, s32 arg1, s32 arg2);
void audio_setChannelNoteMesg(u8 player, s32 chan, u8 arg2);
void audio_muteChannel(u8 player, s32 chan);
void audio_unmuteChannel(u8 player, s32 chan);
void audio_fadeChannelVolume(u8 player, u8 chan, u8 volume, s32 time);
void func_100086FC(u8 player, u8 chan, u8 arg2);
void audio_setChannelPan(u8 player, u8 chan, u8 pan);
void audio_fadeChannelsVolume(u8 player, s32 mask, u8 volume, s32 time);
void audio_setChannelFade(u8 player, u8 chan, u8 arg2);
void audio_setChannelsFade(u8 player, s32 mask, u8 arg2);
void audio_setChannelsEnabled(u8 player, s32 mask, s32 enable);
void audio_setChannelMask(u8 player, s32 mask, s32 enable);
u8 audio_getChannelFade(u8 player, u8 chan);
void audio_setChannelsFxBus(u8 player, s32 mask, s32 arg2);
void func_10008B2C(u8 player);
void audio_setSeqFxParam(u8 player, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void audio_setSeqFxMix(u8 player, f32 arg1, f32 arg2);
void audio_findSeqLoopMarkers(u8 player, u8 count, s32 base);
void audio_jumpToSeqMarker(u8 player, u8 marker);
s32 audio_playSequence(u8 player, s32 sequence);
void audio_setSeqVolume(u8 player, s32 vol);
void audio_stopSeq(u8 player);
void audio_pauseSeq(u8 player);
extern ALBank *gSoundBank; // sound bank
extern ALHeap gAudioHeap;  // audio heap
extern u8 D_80044B20[];    // the audio heap's memory, 0x3E000 bytes from 0x80044B20 (linker-defined, past the init .bss)

/* init/Audio/AudioMgr/audiomgr.c */
void audio_createAudioMgr(SynDriverConfig *config, OSPri priority, AudioMgrConfig *amConfig);
void audio_stopThread(void);
void audio_threadMain(void *arg);
s32 audio_handleFrame(AudioInfo *info, AudioInfo *lastInfo);
s32 audio_dmaSample(s32 addr, s32 len, void *state);
ALDMAproc audio_dmaNew(void *state);
void audio_clearDma(void);
void audio_bankDecRef(BankRecord *record);
void audio_bankRelease(BankRecord *record);
void audio_bankAddRef(BankRecord *record);
void audio_retireBank(BankRecord *record);
void *audio_fetchBank(u32 *slot, s32 mode);
BankFetchProc audio_bankFetchNew(void);
void audio_updateBanks(void);
void audio_releaseRawBanks(void);
extern s32 gReleaseRawBanksPending;

/* The .rodata slice at 0x2C960 of conker.us.yaml (no C file); audiomgr.c points the audio tasks at it. */
extern u8 D_8002C960[]; /* data of the audio microcode */

/* libultra/audio/n_seqplayer.c to libultra/audio/n_resample.c are Rare's copies of the n_audio library (sequence player, event queue,
 * synthesizer, reverb, envelope mixer, resampler, loader). Like libultra, they are declared by the SDK headers
 * (n_libaudio.h, n_synthInternals.h) and by their own files. */

/* init/Audio/MixSelector/mixselector.c */
void audio_requestMixSelector(s32 selector);
void audio_setMixOverride(s32 selector);
void audio_setMixFallback(s32 selector);
void audio_clearMixSelectors(void);
void audio_updateMix(void);

/* init/Audio/OutputMode/outputmode.c */
void audio_setOutputMode(u8 arg0);
void audio_setBusReverbMode(s32 arg0, u32 arg1);
/* Output-mode flags (libultra/audio/n_env.c reads them): C0 = stereo/headphone, C1 = mono, C2 = headphone. */
extern u8 D_800428C0;
extern u8 D_800428C1;
extern u8 D_800428C2;
extern u8 D_800428C4[2];
extern u8 D_800428C6[2];

/* libultra/audio/n_env.c */
extern s16 n_eqpower[128]; /* n_eqpower: equal-power pan curve */

/* libultra/audio/n_sndplayer.c */
s32  audio_getSoundPlayState(SndState **arg0);
SndState *audio_playSound(ALBank *bank, s16 arg1, u16 arg2, u8 arg3, f32 arg4, u8 arg5, u8 arg6, SndState **arg7);
void audio_stopSound(SndState *arg0);
void audio_stopAllSounds(void);
void audio_stopLoopingSounds(void);
void audio_postSoundEvent(SndState *state, s16 type, s32 arg);
void audio_setSoundGroupVolume(u8 arg0, u16 arg1);


#endif
