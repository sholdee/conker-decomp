/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#ifndef __N_LIBAUDIO__
#define __N_LIBAUDIO__

#include <PR/libaudio.h>

#ifdef _LANGUAGE_C_PLUS_PLUS
extern "C" {
#endif

#include <PR/ultratypes.h>
#include <PR/mbi.h>


/*
 * Synthesis driver stuff
 */
typedef struct N_ALVoice_s {
    /* 0x00 */  ALLink              node;
    /* 0x08 */  struct N_PVoice_s   *pvoice;
    /* 0x0C */  ALWaveTable         *table;
    /* 0x10 */  void                *unk10;
    /* 0x14 */  void                *unk14;
    /* 0x18 */  s16                 state;
    /* 0x1A */  s16                 priority;
    // /* 0x1C */  void                *clientPrivate;
    /* 0x20 */  s16                 fxBus;
    /* 0x22 */  s16                 unityPitch;
} N_ALVoice; // size 0x20

/* Conker: the compact sequence player's initOsc takes a 7th argument. */
typedef ALMicroTime (*N_ALCSPOscInit)(void **oscState, f32 *initVal, u8 oscType,
                                      u8 oscRate, u8 oscDepth, u8 oscDelay, u8 arg6);

/* Conker: the driver's bank loader resolves an instrument slot (isInst) or a sound slot to the loaded object. */
typedef void *(*BankLoadProc)(void *slot, s32 isInst);

typedef struct {
    /* 0x00 */  ALPlayer    *head;          /* client list head                     */
    // ALPlayer    *n_seqp1;    /* for fade in / fade out */
    // ALPlayer    *n_seqp2;
    // ALPlayer    *n_sndp;
    ALLink      pFreeList;      /* list of free physical voices         */
    ALLink      pAllocList;     /* list of allocated physical voices    */
    ALLink      pLameList;      /* list of voices ready to be freed     */
    /* 0x1C */  s32         paramSamples;
    /* 0x20 */  s32         curSamples;     /* samples from start of game           */
    /* 0x24 */  ALDMANew    dma;
    /* Conker: the bank hooks of the driver config (SynDriverConfig in init_8F90.c) */
    /* 0x28 */  BankLoadProc (*getLoadProc)(void);
    /* 0x2C */  void        (*bankDecRef)(void *record);
    /* 0x30 */  void        (*bankAddRef)(void *record);
    /* 0x34 */  void        (*bankRelease)(void *record);
    /* 0x38 */  s32         waveTableBase;  /* handed to func_10012C5C */
    /* 0x3C */  ALHeap      *heap;
    /* 0x40 */  struct ALParam_s      *paramList;
    /* 0x44 */  struct N_ALMainBus_s  *mainBus;
    /* 0x48 */  struct N_ALAuxBus_s   *auxBus;
    s32         numPVoices;
    s32         maxAuxBusses;
    /* 0x54 */ s32         outputRate;
    s32         maxOutSamples;
    s32         sv_dramout;
    s32         sv_first;
} N_ALSynth;

/*
 * Conker: ALWaveTable as Rare's bank loader (func_10012CFC, init_128D0.c) and _pullSubFrame
 * (func_10020ABC, init_20000.c) see it: the ADPCM wave info, then a word the loader clears.
 */
typedef struct WaveTable {
    /* 0x00 */  u8                  *base;
    /* 0x04 */  s32                 len;
    /* 0x08 */  u8                  type;       /* AL_ADPCM_WAVE / AL_RAW16_WAVE */
    /* 0x09 */  u8                  flags;      /* 1 once relocated */
    /* 0x0A */  u8                  inUse;      /* Rare's "wave is in use" marker, in the byte ALWaveTable's union alignment leaves free */
    /* 0x0B */  u8                  padB;
    /* 0x0C */  void                *loop;      /* ALADPCMloop or ALRawLoop */
    /* 0x10 */  ALADPCMBook         *book;
    /* 0x14 */  s32                 unk14;
} WaveTable;

/*
 * Conker: Rare's low-pass (pole) filter, its version of synthInternals.h's ALLowPass. `first` and
 * the filter state are swapped, and there is one A_POLEF state per main-out channel (L at DMEM
 * 0x4E0, R at 0x650). Each aux bus has one (n_synthesizer.c allocates it, init_1FB40.c runs it on
 * the main outs), and so has each delay section of the reverb (init_1CBF0.c, init_1E530.c).
 */
typedef struct LowPass {
    /* 0x00 */  s16                 fc;
    /* 0x02 */  s16                 fgain;
    /* 0x04 */  u8                  pad4[0x4];
    /* 0x08 */  union {
                    s16             fccoef[16];
                    s64             force_aligned;
                } fcvec;
    /* 0x28 */  s32                 first;      /* set when fc/fgain change, cleared once recomputed */
    /* 0x2C */  POLEF_STATE         *fstate[2];
} LowPass; /* size 0x38 */

/* Conker: recomputes the coefficients of lp for the output rate (init_1CBF0.c). */
void    func_1001CF38(LowPass *lp, f32 outputRate);


void    n_alSynAddPlayer(ALPlayer *client);
void    n_alSynAddSndPlayer(ALPlayer *client);
void    n_alSynAddSeqPlayer(ALPlayer *client);

ALFxRef n_alSynAllocFX( s16 bus,ALSynConfig *c, ALHeap *hp);
s32     n_alSynAllocVoice( N_ALVoice *voice, ALVoiceConfig *vc);


void    n_alSynFreeVoice(N_ALVoice *voice);
ALFxRef n_alSynGetFXRef( s16 bus, s16 index);
s16     n_alSynGetPriority( N_ALVoice *voice);
void    n_alSynRemovePlayer( ALPlayer *client);
void    n_alSynSetFXMix(N_ALVoice *v, u8 fxmix);
void    n_alSynSetFXParam(ALFxRef fx, s16 paramID, void *param);
void    n_alSynFreeFX(ALFxRef *fx);
void    n_alSynSetPan(N_ALVoice *v, u8 pan);
void    n_alSynSetPitch( N_ALVoice *v, f32 pitch);
void    n_alSynSetPriority( N_ALVoice *voice, s16 priority);
void    n_alSynSetVol( N_ALVoice *v, s16 volume, ALMicroTime t);
void    n_alSynStartVoice(N_ALVoice *v, ALWaveTable *table);
void    n_alSynStartVoiceParams(N_ALVoice *v, ALWaveTable *w,f32 pitch, s16 vol,
                                ALPan pan, u8 fxmix, u8 arg6, f32 arg7, u8 arg8,
                                ALMicroTime t);
void    n_alSynStopVoice( N_ALVoice *v);

void    n_alSynNew(ALSynConfig *c);
void    n_alSynDelete(void);


/*
 * Audio Library (AL) stuff
 */
typedef struct {
    N_ALSynth     drvr;
} N_ALGlobals;

extern N_ALGlobals *n_alGlobals;
extern N_ALSynth *n_syn;

void n_alInit(N_ALGlobals *g, ALSynConfig *c);
void n_alClose(N_ALGlobals *glob);
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen,
            s16 *outBuf, s32 outLen);


/*
 * Sequence Player stuff
 */
typedef struct {
    struct N_ALVoice_s    *voice;
} N_ALNoteEvent;


typedef struct {
    struct N_ALVoice_s  *voice;
    ALMicroTime         delta;
    u8                  vol;
} N_ALVolumeEvent;


typedef struct {
    struct N_ALVoiceState_s    *vs;
    void                       *oscState;
    u8                         chan;
} N_ALOscEvent;

typedef struct {
    f32 unk0;
    f32 unk4;
    u8  pad8[0x4];
} N_ALUnknownEvent1;

/* Rare's sound player event: the sound-state record (SndState, src/libultra/audio/init_15550.c). */
typedef struct {
    struct SndState *unk0;
    u8  pad4[0x8];
} N_ALUnknownEvent2;

typedef struct {
    u8  unk0;
    u8  unk1;
    u8  unk2;
    u8  pad3;
    s32 unk4;
    u8  pad[0x4];
} N_ALUnknownEvent3;

typedef struct {
    s16                     type;
    union {
        ALMIDIEvent         midi;
        ALTempoEvent        tempo;
        ALEndEvent          end;
        N_ALNoteEvent       note;
        N_ALVolumeEvent     vol;
        ALSeqpLoopEvent     loop;
        ALSeqpVolEvent      spvol;
        ALSeqpPriorityEvent sppriority;
        ALSeqpSeqEvent      spseq;
        ALSeqpBankEvent     spbank;
        N_ALOscEvent        osc;
        N_ALUnknownEvent1   unknown0;
        N_ALUnknownEvent2   unknown1;
        N_ALUnknownEvent3   unknown2;
    } msg;
} N_ALEvent;


typedef struct {
    ALLink         node;
    ALMicroTime    delta;
    N_ALEvent      evt;
} N_ALEventListItem;

void            n_alEvtqNew(ALEventQueue *evtq, N_ALEventListItem *items, s32 itemCount);
ALMicroTime     n_alEvtqNextEvent(ALEventQueue *evtq, N_ALEvent *evt);
void            n_alEvtqPostEvent(ALEventQueue *evtq, N_ALEvent *evt, ALMicroTime delta, s32 foo);
void            n_alEvtqFlushType(ALEventQueue *evtq, s16 type);
/* Conker: Rare's n_alEvtqFlushType that returns the delta of the flushed events (src/init_1C060.c, subsystem audio). */
s32             func_1001C4F0(ALEventQueue *evtq, s16 type);


typedef struct N_ALVoiceState_s {
    /* 0x00 */  struct N_ALVoiceState_s *next;     /* MUST be first                */
    /* 0x04 */  N_ALVoice          voice;
    /* 0x24 */  ALSound            *sound;
    /* 0x28 */  ALMicroTime        envEndTime;     /* time of envelope segment end */
    /* 0x2C */  f32                pitch;          /* currect pitch ratio          */
    /* 0x30 */  f32                vibrato;        /* current value of the vibrato */
    /* 0x34 */  u8                 envGain;        /* current envelope gain        */
    /* 0x35 */  u8                 channel;        /* channel assignment           */
    /* 0x36 */  u8                 key;            /* note on key number           */
    /* 0x37 */  u8                 velocity;       /* note on velocity             */
    /* 0x38 */  u8                 envPhase;       /* what envelope phase          */
    /* 0x39 */  u8                 phase;
    /* 0x3A */  u8                 tremelo;        /* current value of the tremelo */
    /* 0x3B */  u8                 flags;          /* bit 0 tremelo flag
                                                      bit 1 vibrato flag           */
    /* 0x3C */  void               *tremOscState;  /* Conker: the oscillator states __n_seqpStopOsc stops */
    /* 0x40 */  void               *vibOscState;
} N_ALVoiceState;

typedef struct {
    /* 0x00 */  ALPlayer            node;          /* note: must be first in structure */
    /* 0x14 */  N_ALSynth          *drvr;          /* reference to the client driver   */
    /* 0x18 */  ALSeq              *target;        /* current sequence                 */
    /* 0x1C */  ALMicroTime         curTime;
    /* 0x20 */  ALBank             *bank;           /* current ALBank                   */
    /* 0x24 */  s32                 uspt;           /* microseconds per tick            */
    /* 0x28 */  s32                 nextDelta;      /* microseconds to next callback    */
    /* 0x2C */  s32                 state;
    /* 0x30 */  u16                 chanMask;       /* active channels                  */
    /* 0x32 */  s16                 vol;            /* overall sequence volume          */
    /* 0x34 */  u8                  maxChannels;    /* number of MIDI channels          */
    /* 0x35 */  u8                  debugFlags;     /* control which error get reported */
    /* 0x38 */  N_ALEvent           nextEvent;
    /* 0x48 */  ALEventQueue        evtq;
    /* 0x5C */  ALMicroTime         frameTime;
    /* 0x60 */  ALChanState        *chanState;      /* 16 channels for MIDI             */
    /* 0x64 */  N_ALVoiceState     *vAllocHead;     /* list head for allocated voices   */
    /* 0x68 */  N_ALVoiceState     *vAllocTail;     /* list tail for allocated voices   */
    /* 0x6C */  N_ALVoiceState     *vFreeList;      /* list of free voice state structs */
    /* 0x70 */  ALOscInit           initOsc;
    /* 0x74 */  ALOscUpdate         updateOsc;
    /* 0x78 */  ALOscStop           stopOsc;
    /* 0x7C */  ALSeqMarker        *loopStart;
    /* 0x80 */  ALSeqMarker        *loopEnd;
    /* 0x84 */  s32                 loopCount;      /* -1 = loop forever, 0 = no loop   */
    /* 0x88 */  u8 pad88[0x4];
    /* 0x8C */  u8 voiceLimit;     /* same bytes as N_ALCSPlayer.voiceLimit */
    /* 0x8D */  u8 voiceCount;
} N_ALSeqPlayer;

typedef struct {
    /* 0x00 */  ALPlayer            node;           /* note: must be first in structure */
    /* 0x14 */  N_ALSynth          *drvr;           /* reference to the client driver   */
    /* 0x18 */  ALCSeq             *target;         /* current sequence                 */
    /* 0x1C */  ALMicroTime         curTime;
    /* 0x20 */  ALBank             *bank;           /* current ALBank                   */
    /* 0x24 */  s32                 uspt;           /* microseconds per tick            */
    /* 0x28 */  s32                 nextDelta;      /* microseconds to next callback    */
    /* 0x2C */  s32                 state;
    /* 0x30 */  u16                 chanMask;       /* active channels                  */
    /* 0x32 */  s16                 vol;            /* overall sequence volume          */
    /* 0x34 */  u8                  maxChannels;    /* number of MIDI channels          */
    /* 0x35 */  u8                  debugFlags;     /* control which error get reported */
    /* 0x36 */  u8                  mpegBank;       /* Rare: the Oct 2000 debugger's name; set by func_1001A3E0 */
    /* 0x37 */  u8                  pad37;
    /* 0x38 */  N_ALEvent           nextEvent;
    /* 0x48 */  ALEventQueue        evtq;
    /* 0x5C */  ALMicroTime         frameTime;
    /* 0x60 */  ALChanState        *chanState;      /* 16 channels for MIDI             */
    /* 0x64 */  N_ALVoiceState     *vAllocHead;     /* list head for allocated voices   */
    /* 0x68 */  N_ALVoiceState     *vAllocTail;     /* list tail for allocated voices   */
    /* 0x6C */  N_ALVoiceState     *vFreeList;      /* list of free voice state structs */
    /* 0x70 */  N_ALCSPOscInit      initOsc;
    /* 0x74 */  ALOscUpdate         updateOsc;
    /* 0x78 */  ALOscStop           stopOsc;
    /* RARE CUSTOM (mpegBank, voiceLimit and voiceCount are the Oct 2000 debugger's names) */
    /* 0x7C */  f32          fxMixOffset;     /* fxmix = ((chan fxmix & 0x7F) + fxMixOffset * 127) * fxMixScale */
    /* 0x80 */  f32          fxMixScale;      /* both set by event 25 (func_10017DF0) */
    /* 0x84 */  struct OSMesgQueue_s *msgQueue; /* receives marker and note messages (func_10017AF0) */
    /* 0x88 */  s32          pausedDelta;     /* ticks to the next sequence event, saved on pause (state 3) */
    /* 0x8C */  u8           voiceLimit;      /* Oct 2000 had these two at 0x88/0x89, before pausedDelta */
    /* 0x8D */  u8           voiceCount;
} N_ALCSPlayer;


/*
 * Sequence data representation routines
 */
void    n_alSeqNextEvent(ALSeq *seq, N_ALEvent *event);
void    n_alSeqNewMarker(ALSeq *seq, ALSeqMarker *m, u32 ticks);

void    n_alCSeqNew(ALCSeq *seq, u8 *ptr);
/* Conker: Rare's n_alCSeqNextEvent (n_csq.c) takes a third argument. */
void    n_alCSeqNextEvent(ALCSeq *seq, N_ALEvent *evt, s32 arg2);
void    n_alCSeqNewMarker(ALCSeq *seq, ALCSeqMarker *m, u32 ticks);


/*
 * Sequence Player routines
 */
void    n_alSeqpNew(N_ALSeqPlayer *seqp, ALSeqpConfig *config);
void    n_alSeqpDelete(N_ALSeqPlayer *seqp);
u8      n_alSeqpGetChlVol(N_ALSeqPlayer *seqp, u8 chan);
u8      n_alSeqpGetChlFXMix(N_ALSeqPlayer *seqp, u8 chan);
ALPan   n_alSeqpGetChlPan(N_ALSeqPlayer *seqp, u8 chan);
u8      n_alSeqpGetChlPriority(N_ALSeqPlayer *seqp, u8 chan);
s32     n_alSeqpGetChlProgram(N_ALSeqPlayer *seqp, u8 chan);
ALSeq  *n_alSeqpGetSeq(N_ALSeqPlayer *seqp);
s32     n_alSeqpGetState(N_ALSeqPlayer *seqp);
s32     n_alSeqpGetTempo(N_ALSeqPlayer *seqp);
s16     n_alSeqpGetVol(N_ALSeqPlayer *seqp);        /* Master volume control */
void    n_alSeqpPlay(N_ALSeqPlayer *seqp);
void    n_alSeqpSendMidi(N_ALSeqPlayer *seqp, s32 ticks, u8 status, u8 byte1, u8 byte2);
void    n_alSeqpSetBank(N_ALSeqPlayer *seqp, ALBank *b);
void    n_alSeqpSetChlVol(N_ALSeqPlayer *seqp, u8 chan, u8 vol);
void    n_alSeqpSetChlFXMix(N_ALSeqPlayer *seqp, u8 chan, u8 fxmix);
void    n_alSeqpSetChlPan(N_ALSeqPlayer *seqp, u8 chan, ALPan pan);
void    n_alSeqpSetChlPriority(N_ALSeqPlayer *seqp, u8 chan, u8 priority);
void    n_alSeqpSetChlProgram(N_ALSeqPlayer *seqp, u8 chan, u8 prog);
void    n_alSeqpSetSeq(N_ALSeqPlayer *seqp, ALSeq *seq);
void    n_alSeqpSetTempo(N_ALSeqPlayer *seqp, s32 tempo);
void    n_alSeqpSetVol(N_ALSeqPlayer *seqp, s16 vol);
void    n_alSeqpStop(N_ALSeqPlayer *seqp);
void    n_alSeqpLoop(N_ALSeqPlayer *seqp, ALSeqMarker *start, ALSeqMarker *end, s32 count);


/*
 * Compressed Sequence Player routines
 */
void    n_alCSPNew(N_ALCSPlayer *seqp, ALSeqpConfig *config);
void    n_alCSPDelete(N_ALCSPlayer *seqp);
u8      n_alCSPGetChlVol(N_ALCSPlayer *seqp, u8 chan);
u8      n_alCSPGetChlFXMix(N_ALCSPlayer *seqp, u8 chan);
ALPan   n_alCSPGetChlPan(N_ALCSPlayer *seqp, u8 chan);
u8      n_alCSPGetChlPriority(N_ALCSPlayer *seqp, u8 chan);
s32     n_alCSPGetChlProgram(N_ALCSPlayer *seqp, u8 chan);
ALCSeq *n_alCSPGetSeq(N_ALCSPlayer *seqp);
s32     n_alCSPGetState(N_ALCSPlayer *seqp);
s32     n_alCSPGetTempo(N_ALCSPlayer *seqp);
s16     n_alCSPGetVol(N_ALCSPlayer *seqp);
void    n_alCSPPlay(N_ALCSPlayer *seqp);
void    n_alCSPSendMidi(N_ALCSPlayer *seqp, s32 ticks, u8 status, u8 byte1, u8 byte2);
void    n_alCSPSetBank(N_ALCSPlayer *seqp, ALBank *b);
void    n_alCSPSetChlVol(N_ALCSPlayer *seqp, u8 chan, u8 vol);
void    n_alCSPSetChlFXMix(N_ALCSPlayer *seqp, u8 chan, u8 fxmix);
void    n_alCSPSetChlPan(N_ALCSPlayer *seqp, u8 chan, ALPan pan);
void    n_alCSPSetChlPriority(N_ALCSPlayer *seqp, u8 chan, u8 priority);
void    n_alCSPSetChlProgram(N_ALCSPlayer *seqp, u8 chan, u8 prog);
void    n_alCSPSetSeq(N_ALCSPlayer *seqp, ALCSeq *seq);
void    n_alCSPSetTempo(N_ALCSPlayer *seqp, s32 tempo);
void    n_alCSPSetVol(N_ALCSPlayer *seqp, s16 vol);
void    n_alCSPStop(N_ALCSPlayer *seqp);


/*
 * Sound Player stuff
 */
typedef struct {
    /* 0x00 */  ALPlayer            node;           /* note: must be first in structure */
    /* 0x14 */  ALEventQueue        evtq;
    /* 0x28 */  N_ALEvent           nextEvent;
    /* 0x38 */  N_ALSynth           *drvr;          /* reference to the client driver   */
    /* 0x3C */  s32                 target;
    /* 0x40 */  void                *sndState;
    /* 0x44 */  s32                 maxSounds;
    /* 0x48 */  ALMicroTime         frameTime;
    /* 0x4C */  ALMicroTime         nextDelta;      /* microseconds to next callback    */
    /* 0x50 */  ALMicroTime         curTime;
} N_ALSndPlayer;

void     n_alSndpNew(N_ALSndPlayer *sndp, ALSndpConfig *c);
void     n_alSndpDelete(void);
ALSndId  n_alSndpAllocate(ALSound *sound);
void     n_alSndpDeallocate(ALSndId id);
s32      n_alSndpGetState(void);
void     n_alSndpPlay(void);
void     n_alSndpPlayAt(ALMicroTime delta);
void     n_alSndpSetFXMix(u8 mix);
void     n_alSndpSetPan(ALPan pan);
void     n_alSndpSetPitch(f32 pitch);
void     n_alSndpSetPriority(ALSndId id, u8 priority);
void     n_alSndpSetVol(s16 vol);
void     n_alSndpStop(void);
ALSndId  n_alSndpGetSound(void);
void     n_alSndpSetSound(ALSndId id);


/*
 * Conker: Rare's additions to the audio library, defined in src/libultra/audio, in address order.
 */
struct OscState; /* the sequence players' oscillator state, defined in init_128D0.c */
void    func_10012934(ALBankFile *file, u8 *table, s32 arg2);           /* init_128D0.c: alBnkfNew */
void    func_10012A28(ALBank *bank, s32 offset, s32 table, s32 arg3);   /* init_128D0.c: relocates a bank */
void    func_10012B84(ALInstrument *inst, s32 arg1);
void    func_10012BD0(ALInstrument *inst, s32 arg1);
void    func_10012C5C(ALSound *sound, s32 offset, s32 table);
void    func_10012CFC(WaveTable *wave, s32 offset, s32 table);
f32     func_10012D80(u8 arg0);
s32     func_10012E04(s32 *arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5); /* the oscillators */
s32     func_10012F94(struct OscState *arg0, f32 *arg1);
void    func_100131D8(s32 *arg0);
void    func_100131FC(ALSeqpConfig *config, s32 arg1);                  /* installs the oscillators */
void    func_10017AA0(N_ALCSPlayer *seqp);                              /* code_17AA0.c: play */
void    func_10017AF0(N_ALCSPlayer *seqp, struct OSMesgQueue_s *queue); /* init_17AF0.c */
void    func_10017B04(N_ALCSPlayer *seqp, s32 chan, u8 arg2);
void    func_10017B30(N_ALCSPlayer *seqp);
void    func_10017BB8(N_ALCSPlayer *seqp, s32 arg1);
void    func_10017C00(N_ALCSPlayer *seqp, s32 arg1);                    /* init_17C00.c */
void    func_10017C68(N_ALCSPlayer *seqp, s32 arg1, u8 arg2, u8 arg3);
void    func_10017CE0(N_ALCSPlayer *seqp, s32 arg1, u8 arg2);
void    func_10017D30(N_ALCSPlayer *seqp, s32 arg1, u8 arg2);
void    func_10017D80(N_ALCSPlayer *seqp, u8 chan, u8 prog);            /* code_17D80.c */
void    func_10017DF0(N_ALCSPlayer *csp, f32 arg1, f32 arg2);           /* init_17DF0.c */
void    func_10017E4C(N_ALCSPlayer *csp, u8 chan, u8 arg2);
void    func_10017F10(N_ALCSPlayer *seqp, u8 arg1, u8 arg2, u8 arg3, s32 arg4); /* init_17F10.c */
void    func_100186DC(ALCSeq *seq, ALCSeqMarker *marker);               /* n_csq.c */
void    func_10018790(ALCSeq *seq, ALCSeqMarker *markers, u32 count, u32 base);
void    func_10018C60(N_ALCSPlayer *seqp);                              /* code_18C60.c: posts AL_TRACK_END */
void    func_10018CB0(N_ALCSPlayer *seqp, ALCSeq *seq);                 /* code_18CB0.c */
void    func_10018D00(N_ALCSPlayer *seqp, s16 vol);                     /* init_18D00.c */
void    func_10018D50(N_ALCSPlayer *seqp);                              /* init_18D50.c */
void    func_1001A704(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 value); /* init_19B50.c */
void    func_1001CA90(N_ALVoice *v, f32 pitch);                         /* n_synsetpitch2.c */
f32     func_1001CEA4(s32 arg0);                                        /* init_1CBF0.c */
void   *func_1001D9B0(s16 bus);                                         /* init_1D900.c: the bus's effect */
LowPass *func_1001DA28(s16 bus);                                        /* init_1D900.c: the bus's low-pass */
void    func_1001DAA0(void *fx, s16 paramID, void *value);
void    func_1001DAE4(LowPass *lp, s16 paramID, s32 *value);
void    func_1001E2A0(N_ALVoice *v, u8 pan);                            /* init_1E2A0.c */
void    func_1001E350(N_ALVoice *v, u8 fxmix);                          /* init_1E350.c */
void    func_1001E400(N_ALCSPlayer *seqp, s32 deltaTime, u8 status, u8 byte1, u8 byte2); /* init_1E400.c: send MIDI */

/* Conker: Rare's debug printf in src/init_2070.c, used by init_15550.c (the game's declaration is in include/conker/libc.h). */
void    func_10002088(const char *fmt, ...);

/* Conker: Rare's instrument loading in src/init_1AAE0.c (subsystem audio), used by init_15550.c. */
void   *func_1001BD34(N_ALSynth *drvr, ALInstrument **instSlot, s32 soundIdx);
void    func_1001BE1C(N_ALSynth *drvr, ALInstrument *inst, s32 soundIdx);


/*
 *  for n_audio micro code
 */
extern long long int    n_aspMainTextStart[], n_aspMainTextEnd[];
extern long long int    n_aspMainDataStart[], n_aspMainDataEnd[];


#ifdef _LANGUAGE_C_PLUS_PLUS
}
#endif

#endif /* __N_LIBAUDIO__ */
