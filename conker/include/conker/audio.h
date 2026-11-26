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
void func_10008180(void);
void func_100084D8(u8 player);
s32 func_1000853C(u8 player);
void func_10008570(u8 player, OSMesgQueue *queue);
void func_100085A4(s32 arg0, s32 arg1, s32 arg2);
void func_100085B8(u8 player, s32 chan, u8 arg2);
void func_100085F8(u8 player, s32 chan);
void func_1000862C(u8 player, s32 chan);
void func_10008660(u8 player, u8 chan, u8 volume, s32 time);
void func_100086FC(u8 player, u8 chan, u8 arg2);
void func_10008744(u8 player, u8 chan, u8 pan);
void func_10008790(u8 player, s32 mask, u8 volume, s32 time);
void func_10008824(u8 player, u8 chan, u8 arg2);
void func_1000886C(u8 player, s32 mask, u8 arg2);
void func_100088F0(u8 player, s32 mask, s32 enable);
void func_10008988(u8 player, s32 mask, s32 enable);
u8 func_10008A4C(u8 player, u8 chan);
void func_10008A94(u8 player, s32 mask, s32 arg2);
void func_10008B2C(u8 player);
void func_10008B60(u8 player, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_10008BC0(u8 player, f32 arg1, f32 arg2);
void func_10008C04(u8 player, u8 count, s32 base);
void func_10008C6C(u8 player, u8 marker);
s32 func_10008CE8(u8 player, s32 sequence);
void func_10008EE0(u8 player, s32 vol);
void func_10008F24(u8 player);
void func_10008F58(u8 player);
extern ALBank *D_8003E368; // sound bank
extern ALHeap D_8003E370;  // audio heap

/* init_8F90.c */
void func_10008F90(SynDriverConfig *config, OSPri priority, AudioMgrConfig *amConfig);
void func_100093CC(void);
void func_10009400(void *arg);
s32 func_100095A0(AudioInfo *info, AudioInfo *lastInfo);
s32 func_100097CC(s32 addr, s32 len, void *state);
ALDMAproc func_10009980(void *state);
void func_100099BC(void);
void func_10009B2C(BankRecord *record);
void func_10009B4C(BankRecord *record);
void func_10009B90(BankRecord *record);
void func_10009BE4(BankRecord *record);
void *func_10009CBC(u32 *slot, s32 mode);
BankFetchProc func_10009FFC(void);
void func_1000A03C(void);
void func_1000A348(void);
extern s32 D_8003E384;

/* init_1AAE0.c to init_22460.c are Rare's copies of the n_audio library (sequence player, event queue,
 * synthesizer, reverb, envelope mixer, resampler, loader). Like libultra, they are declared by the SDK headers
 * (n_libaudio.h, n_synthInternals.h) and by their own files, which do not include functions.h. */

/* init_11FA0.c */
void func_10011FA0(s32 selector);
void func_10011FB0(s32 selector);
void func_10011FDC(s32 selector);
void func_10011FEC(void);
void func_10012020(void);

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
