#ifndef CONKER_AUDIO_H
#define CONKER_AUDIO_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "audio": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_8180.c
 *   init_8F90.c
 *   init_11FA0.c
 *   libultra/audio/init_17870.c
 *   init_1AAE0.c
 *   init_1C060.c
 *   init_1E480.c
 *   init_1E4A0.c
 *   init_1E530.c
 *   init_1FB40.c
 *   init_20000.c
 *   init_210C0.c
 *   init_214F0.c
 *   init_22040.c
 *   init_22460.c
 */

/* Types that these files define; the declarations below use them. */
/* Sound bank loader (init_8F90.c): a bank fetch proc takes an owner slot (a ROM reference, or a record
 * once loading has started) and returns the bank's data, loading it on demand. */
typedef struct BankRecord BankRecord;
typedef void *(*BankFetchProc)(u32 *slot, s32 mode);
typedef struct AudioMgrConfig AudioMgrConfig;
typedef struct SynDriverConfig SynDriverConfig;
typedef struct AudioInfo_s AudioInfo;

/* init_8180.c */
void audio_init(void);
void func_100084D8(u8 player);
s32 audio_getSeqState(u8 player);
void audio_setSeqMsgQueue(u8 player, OSMesgQueue *queue);
void func_100085A4(s32 arg0, s32 arg1, s32 arg2);
void func_100085B8(u8 player, s32 chan, u8 arg2);
void audio_muteChannel(u8 player, s32 chan);
void audio_unmuteChannel(u8 player, s32 chan);
void audio_fadeChannelVolume(u8 player, u8 chan, u8 volume, s32 time);
void func_100086FC(u8 player, u8 chan, u8 arg2);
void audio_setChannelPan(u8 player, u8 chan, u8 pan);
void audio_fadeChannelsVolume(u8 player, s32 mask, u8 volume, s32 time);
void func_10008824(u8 player, u8 chan, u8 arg2);
void func_1000886C(u8 player, s32 mask, u8 arg2);
void audio_setChannelsEnabled(u8 player, s32 mask, s32 enable);
void audio_setChannelMask(u8 player, s32 mask, s32 enable);
u8 func_10008A4C(u8 player, u8 chan);
void func_10008A94(u8 player, s32 mask, s32 arg2);
void func_10008B2C(u8 player);
void func_10008B60(u8 player, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_10008BC0(u8 player, f32 arg1, f32 arg2);
void audio_findSeqLoopMarkers(u8 player, u8 count, s32 base);
void audio_jumpToSeqMarker(u8 player, u8 marker);
s32 audio_playSequence(u8 player, s32 sequence);
void audio_setSeqVolume(u8 player, s32 vol);
void func_10008F24(u8 player);
void audio_stopSeq(u8 player);
extern ALBank *gSoundBank; // sound bank
extern ALHeap gAudioHeap;  // audio heap

/* init_8F90.c */
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

/* init_1AAE0.c to init_22460.c are Rare's copies of the n_audio library (sequence player, event queue,
 * synthesizer, reverb, envelope mixer, resampler, loader). Like libultra, they are declared by the SDK headers
 * (n_libaudio.h, n_synthInternals.h) and by their own files, which do not include functions.h. */

/* init_11FA0.c */
void audio_requestMixSelector(s32 selector);
void audio_setMixOverride(s32 selector);
void audio_setMixFallback(s32 selector);
void audio_clearMixSelectors(void);
void audio_updateMix(void);

/* libultra/audio/init_17870.c */
void func_10017870(u8 arg0);
void func_10017944(s32 arg0, u32 arg1);
extern u8 D_800428C0;
extern u8 D_800428C1;
extern u8 D_800428C2;
extern u8 D_800428C4[2];
extern u8 D_800428C6[2];

/* init_20000.c */
extern s16 D_8002BC10[128]; /* n_eqpower: equal-power pan curve */

#endif
