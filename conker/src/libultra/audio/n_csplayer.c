#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>
#include <assert.h>
#include "n_libaudio.h"
#include "n_seqp.h"
#include "cseq.h"
#include "n_cseqp.h"

static ALMicroTime    D_80042810[16];   /* per-channel note-off delta */



       ALMicroTime      __n_CSPVoiceHandler(void *node);
static void              __n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp);
static void             __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
static void             __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
       void             __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item);
       void              __n_setUsptFromTempo(N_ALCSPlayer *seqp, f32 tempo);


void n_alCSPNew(N_ALCSPlayer *seqp, ALSeqpConfig *c)
{
    s32                 i;
    N_ALEventListItem  *items;
    N_ALVoiceState     *vs;
    N_ALVoiceState     *voices;

    ALHeap *hp = c->heap;

    /*
     * initialize member variables
     */
    seqp->bank          = 0;
    seqp->target        = NULL;
    seqp->drvr          = n_syn;
    seqp->chanMask      = 0xffff;
    func_10017B30(seqp);
    seqp->uspt          = 488;
    seqp->nextDelta     = 0;
    seqp->state         = AL_STOPPED;
    seqp->vol           = 0x7FFF;              /* full volume  */
    seqp->debugFlags    = c->debugFlags;
    seqp->frameTime     = AL_USEC_PER_FRAME;   /* should get this from driver */
    seqp->curTime       = 0;
    seqp->initOsc       = c->initOsc;
    seqp->updateOsc     = c->updateOsc;
    seqp->stopOsc       = c->stopOsc;

#if 1
    seqp->fxMixOffset = 0.0f;
    seqp->fxMixScale = 1.0f;
    seqp->msgQueue = 0;
    seqp->voiceCount = 0;
    seqp->voiceLimit = c->maxVoices;
#endif

    seqp->nextEvent.type = AL_SEQP_API_EVT;  /* this will start the voice handler "spinning" */

    /*
     * init the channel state
     */
    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapAlloc(hp, c->maxChannels, sizeof(ALChanState) );
    __n_initChanState((N_ALSeqPlayer*)seqp);  /* sct 11/6/95 */

    /*
     * init the voice state array
     */
    voices = alHeapAlloc(hp, c->maxVoices, sizeof(N_ALVoiceState));
    seqp->vFreeList = 0;
    for (i = 0; i < c->maxVoices; i++) {
      vs = &voices[i];
      vs->next = seqp->vFreeList;
      seqp->vFreeList = vs;
    }

    seqp->vAllocHead = 0;
    seqp->vAllocTail = 0;

    /*
     * init the event queue
     */
    items = alHeapAlloc(hp, c->maxEvents, sizeof(N_ALEventListItem));
    n_alEvtqNew(&seqp->evtq, items, c->maxEvents);


    /*
     * add ourselves to the driver
     */
    seqp->node.next       = NULL;
    seqp->node.handler    = __n_CSPVoiceHandler;
    seqp->node.clientData = seqp;
#if 1
    n_alSynAddSndPlayer (&seqp->node);
#endif
#if 0
    n_alSynAddSeqPlayer( &seqp->node);
#endif
}

ALMicroTime __n_CSPVoiceHandler(void *node)
{
    N_ALCSPlayer    *seqp = (N_ALCSPlayer *) node;
    N_ALEvent       evt;
    N_ALVoice       *voice;
    ALMicroTime     delta;
    N_ALVoiceState  *vs;
    void            *oscState;
    f32             oscValue;
    u8              chan;

    do {
        switch (seqp->nextEvent.type)
        {
            case 0: /* AL_SEQ_REF_EVT */
                __n_CSPHandleNextSeqEvent(seqp);
                break;

            case 9: /* AL_SEQP_API_EVT */
                evt.type = 9;
                n_alEvtqPostEvent(&seqp->evtq, &evt, seqp->frameTime, 1);
                break;

            case 5: /* AL_NOTE_END_EVT */
                voice = seqp->nextEvent.msg.note.voice;

                n_alSynStopVoice(voice);
                n_alSynFreeVoice(voice);
                vs = (N_ALVoiceState *)voice->unk10;
                if (vs->flags)
                    __n_seqpStopOsc((N_ALSeqPlayer*)seqp, vs);
                __n_unmapVoice((N_ALSeqPlayer *)seqp, voice);
                break;

            case 6: /* AL_SEQP_ENV_EVT */
                voice = seqp->nextEvent.msg.vol.voice;
                vs = (N_ALVoiceState *)voice->unk10;

                if (vs->envPhase == AL_PHASE_ATTACK)
                    vs->envPhase = AL_PHASE_DECAY;

                delta = seqp->nextEvent.msg.vol.delta;
                vs->envEndTime = seqp->curTime + delta;
                vs->envGain = seqp->nextEvent.msg.vol.vol;
                n_alSynSetVol(voice, __n_vsVol(vs, (N_ALSeqPlayer*)seqp), delta);
                break;

            case 23: /* AL_TREM_OSC_EVT */
                vs = seqp->nextEvent.msg.osc.vs;
                oscState = seqp->nextEvent.msg.osc.oscState;
                delta = (*seqp->updateOsc)(oscState, &oscValue);
                vs->tremelo = (u8)oscValue;
                n_alSynSetVol(&vs->voice, __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
                              __n_vsDelta(vs, seqp->curTime));
                evt.type = 23;
                evt.msg.osc.vs = vs;
                evt.msg.osc.oscState = oscState;
                n_alEvtqPostEvent(&seqp->evtq, &evt, delta, 0);
                break;

            case 24: /* AL_VIB_OSC_EVT */
                vs = seqp->nextEvent.msg.osc.vs;
                oscState = seqp->nextEvent.msg.osc.oscState;
                chan = seqp->nextEvent.msg.osc.chan;
                delta = (*seqp->updateOsc)(oscState, &oscValue);
                vs->vibrato = oscValue;
                n_alSynSetPitch(&vs->voice, vs->pitch * vs->vibrato
                                * seqp->chanState[chan].pitchBend);
                if (seqp->chanState[chan].unk14) {
                    func_1001CA90(&vs->voice,
                        440.0f * func_1001CEA4(seqp->chanState[chan].unk15 + (vs->key - vs->sound->keyMap->keyBase) - 0x40)
                        * seqp->chanState[chan].pitchBend * vs->vibrato);
                }
                evt.type = 24;
                evt.msg.osc.vs = vs;
                evt.msg.osc.oscState = oscState;
                evt.msg.osc.chan = chan;
                n_alEvtqPostEvent(&seqp->evtq, &evt, delta, 0);
                break;

            case 2:  /* AL_SEQP_MIDI_EVT */
            case 22: /* AL_CSP_NOTEOFF_EVT */
                __n_CSPHandleMIDIMsg(seqp, &seqp->nextEvent);
                break;

            case 7: /* AL_SEQP_META_EVT */
                __n_CSPHandleMetaMsg(seqp, &seqp->nextEvent);
                break;

            case 10: /* AL_SEQP_VOL_EVT */
                seqp->vol = seqp->nextEvent.msg.spvol.vol;
                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    n_alSynSetVol(&vs->voice,
                                  __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
                                  __n_vsDelta(vs, seqp->curTime));
                }
                break;

            case 25:
                seqp->fxMixOffset = seqp->nextEvent.msg.unknown0.unk0;
                seqp->fxMixScale = seqp->nextEvent.msg.unknown0.unk4;
                for (vs = seqp->vAllocHead; vs != 0; ) {
                    if (vs->envPhase != 3) {
                        n_alSynSetFXMix(&vs->voice, audio_getVoiceFxMix(vs, seqp));
                    }
                    vs = vs->next;
                }
                break;

            case 26:
                if (seqp->nextEvent.msg.unknown2.unk1 < 8) {
                    void *a = func_1001D9B0(seqp->nextEvent.msg.unknown2.unk0);
                    if (a) {
                        func_1001DAA0(a, (seqp->nextEvent.msg.unknown2.unk2 << 3) | (seqp->nextEvent.msg.unknown2.unk1 & 7),
                                      &seqp->nextEvent.msg.unknown2.unk4);
                    }
                } else {
                    LowPass *b = func_1001DA28(seqp->nextEvent.msg.unknown2.unk0);
                    if (b) {
                        func_1001DAE4(b, seqp->nextEvent.msg.unknown2.unk1, &seqp->nextEvent.msg.unknown2.unk4);
                    }
                }
                break;

            case 15: /* AL_SEQP_PLAY_EVT */
                if (seqp->state != AL_PLAYING) {
                    s32 oldState = seqp->state;
                    if (seqp->target) {
                        N_ALEvent evt;
                        s32 deltaTicks;

                        seqp->state = AL_PLAYING;
                        if (__alCSeqNextDelta(seqp->target, &deltaTicks)) {
                            evt.type = 0; /* AL_SEQ_REF_EVT */
                            if (oldState == 3)
                                deltaTicks = seqp->pausedDelta;
                            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks, 0);
                        }
                    }
                }
                break;

            case 16:
                if (seqp->state == AL_PLAYING) {
                    seqp->state = 3;
                    seqp->pausedDelta = n_alEvtqFlushType(&seqp->evtq, 0);
                }
                break;

            case 17: /* AL_SEQP_STOP_EVT */
                if (seqp->state == AL_STOPPING) {
                    for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead) {
                        n_alSynStopVoice(&vs->voice);
                        n_alSynFreeVoice(&vs->voice);
                        if (vs->flags)
                            __n_seqpStopOsc((N_ALSeqPlayer*)seqp, vs);
                        __n_unmapVoice((N_ALSeqPlayer *)seqp, &vs->voice);
                    }
                    seqp->state = AL_STOPPED;
                    for (chan = 0; chan < 16; chan++) {
                        if (seqp->chanState[chan].instMissing) {
                            /* debug output compiled out */
                        }
                        if (seqp->chanState[chan].instrument) {
                            seqp->drvr->bankRelease(seqp->bank->instArray[seqp->chanState[chan].instNum]);
                            seqp->chanState[chan].instrument = 0;
                        }
                    }
                }
                break;

            case 18: /* AL_SEQP_STOPPING_EVT */
                if (seqp->state == AL_PLAYING || seqp->state == 3) {
                    n_alEvtqFlushType(&seqp->evtq, 0);
                    n_alEvtqFlushType(&seqp->evtq, 22);
                    n_alEvtqFlushType(&seqp->evtq, 2);
                    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                        if (__n_voiceNeedsNoteKill((N_ALSeqPlayer *)seqp, &vs->voice, KILL_TIME))
                            __n_seqpReleaseVoice((N_ALSeqPlayer*)seqp, &vs->voice, KILL_TIME);
                    }
                    for (chan = 0; chan < 16; chan++) {
                        seqp->chanState[chan].unkD = seqp->chanState[chan].unkE;
                        if (seqp->chanState[chan].unkD == 0) {
                            seqp->chanMask &= (0xFFFF ^ (1 << chan));
                        } else {
                            seqp->chanMask |= (1 << chan);
                        }
                    }
                    seqp->state = AL_STOPPING;
                    evt.type = 17; /* AL_SEQP_STOP_EVT */
                    n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END, 0);
                }
                break;

            case 12: /* AL_SEQP_PRIORITY_EVT */
                chan = seqp->nextEvent.msg.sppriority.chan;
                seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
                break;

            case 13: /* AL_SEQP_SEQ_EVT */
                seqp->target = seqp->nextEvent.msg.spseq.seq;
                seqp->chanMask = 0xFFFF;
                if (seqp->bank)
                    __n_initFromBank((N_ALSeqPlayer *)seqp, seqp->bank);
                break;

            case 14: /* AL_SEQP_BANK_EVT */
                seqp->bank = seqp->nextEvent.msg.spbank.bank;
                __n_initFromBank((N_ALSeqPlayer *)seqp, seqp->bank);
                break;

            /* sct 11/6/95 - these events should now be handled by __n_CSPHandleNextSeqEvent */
            case 4: /* AL_SEQ_END_EVT */
            case 3: /* AL_TEMPO_EVT */
            case 1: /* AL_SEQ_MIDI_EVT */
                break;
        }
        seqp->nextDelta = n_alEvtqNextEvent(&seqp->evtq, &seqp->nextEvent);

    } while (seqp->nextDelta == 0);

    /*
     * adjust the curTime to account for the next delay
     */
    seqp->curTime += seqp->nextDelta;

    return seqp->nextDelta;
}

static void __n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp)
{
    N_ALEvent   evt;

    /* sct 1/5/96 - Do nothing if we don't have a target sequence. */
    if (seqp->target == NULL || seqp->state == 3)
        return;

    n_alCSeqNextEvent(seqp->target, &evt, 1);

    switch (evt.type)
    {
      case 1: /* AL_SEQ_MIDI_EVT */
        __n_CSPHandleMIDIMsg(seqp, &evt);
        __n_CSPPostNextSeqEvent(seqp);
        break;

      case 3: /* AL_TEMPO_EVT */
        __n_CSPHandleMetaMsg(seqp, &evt);
        __n_CSPPostNextSeqEvent(seqp);
        break;

      case 4: /* AL_SEQ_END_EVT */
        seqp->state = AL_STOPPING;
        evt.type    = 17; /* AL_SEQP_STOP_EVT */
        n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END, 0);
        break;

      case 19: /* AL_TRACK_END */
      case 20: /* AL_CSP_LOOPSTART */
      case 21: /* AL_CSP_LOOPEND */
        __n_CSPPostNextSeqEvent(seqp);
        break;

      default:
        break;
    }
}
static void __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
    N_ALVoice           *voice;
    s32                 status;
    u8                  chan;
    u8                  key;
    u8                  byte1;
    u8                  vel;
    ALMIDIEvent         *midi = &event->msg.midi;
    N_ALEvent           evt;
    ALMicroTime         deltaTime;
    N_ALVoiceState      *vs;
    ALChanState         *cs;
    s32                 tmp;

    status = midi->status & AL_MIDI_StatusMask;
    chan = midi->status & AL_MIDI_ChannelMask;
    byte1 = key = midi->byte1;
    vel = midi->byte2;

    if (seqp->chanState[chan].instMissing && status != AL_MIDI_ProgramChange) {
        evt.type = 2; /* AL_SEQP_MIDI_EVT */
        evt.msg.midi = *midi;
        n_alEvtqPostEvent(&seqp->evtq, &evt, 33333, 0);
        return;
    }

    switch (status) {
        case (AL_MIDI_NoteOn):

            if (vel != 0) { /* a real note on */
                ALVoiceConfig   config;
                ALSound         *sound;
                s16             cents;
                f32             pitch;
                f32             oscValue;
                u8              fxmix;
                u8              unk7A;
                ALPan           pan;
                s16             vol;
                f32             unk70;
                void            *oscState = 0;
                ALInstrument    *inst;

                if (seqp->state != AL_PLAYING || !(seqp->chanMask & (1 << chan))) {
                    if (midi->duration) {
                        evt.type = 2; /* AL_SEQP_MIDI_EVT */
                        evt.msg.midi.status = chan | AL_MIDI_NoteOff;
                        evt.msg.midi.byte1 = key;
                        evt.msg.midi.byte2 = 0;
                        deltaTime = seqp->uspt * midi->duration;
                        D_80042810[chan] = deltaTime;
                        n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                    }
                    break;
                }

                cs = &seqp->chanState[chan];
                sound = __n_lookupSoundQuick((N_ALSeqPlayer *)seqp, key, vel, chan);
                if (sound == NULL)
                    break;
                ALFlagFailIf(!sound, seqp->debugFlags & NO_SOUND_ERR_MASK,
                             ERR_ALSEQP_NO_SOUND);

                config.priority = cs->priority;
                config.fxBus    = cs->unkB;
                config.unityPitch = 0;
                config.unk8 = 0;

                vs = __n_mapVoice((N_ALSeqPlayer*)seqp, key, vel, chan);
                ALFlagFailIf(!vs, seqp->debugFlags & NO_VOICE_ERR_MASK,
                             ERR_ALSEQP_NO_VOICE );

                voice = &vs->voice;

                n_alSynAllocVoice(voice, &config);

                /*
                 * set up the voice state structure
                 */
                vs->sound = sound;
                vs->envPhase = AL_PHASE_ATTACK;
                if (cs->sustain > AL_SUSTAIN)
                    vs->phase = AL_PHASE_SUSTAIN;
                else
                    vs->phase = AL_PHASE_NOTEON;

                cents = (key - sound->keyMap->keyBase) * 100
                    + sound->keyMap->detune;
                if (cs->useEnv)
                    cents += cs->detune;

                vs->pitch = alCents2Ratio(cents);
                if (cs->useEnv) {
                    vs->envGain = cs->attackVolume;
                    vs->envEndTime = seqp->curTime + cs->attackTime;
                } else {
                    vs->envGain = sound->envelope->attackVolume;
                    vs->envEndTime = seqp->curTime +
                        sound->envelope->attackTime;
                }

                /*
                 * setup tremelo and vibrato if active
                 */
                vs->flags = 0;
                if (cs->useEnv) {
                    tmp = cs->tremType;
                } else {
                    inst = seqp->chanState[chan].instrument;
                    tmp = inst->tremType;
                }

                oscValue = (f32)AL_VOL_FULL;  /* set this as a default */
                if (tmp)
                {
                    if (seqp->initOsc)
                    {
                        if (cs->useEnv) {
                            deltaTime = (*seqp->initOsc)(&oscState, &oscValue, cs->tremType,
                                                     cs->tremRate, cs->tremDepth, cs->tremDelay, cs->unk35);
                        } else {
                            deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->tremType,
                                                     inst->tremRate, inst->tremDepth, inst->tremDelay, cs->unk35);
                        }
                        if (deltaTime) /* if deltaTime = zero, don't run osc */
                        {
                            evt.type = 23; /* AL_TREM_OSC_EVT */
                            evt.msg.osc.vs = vs;
                            evt.msg.osc.oscState = oscState;
                            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                            vs->flags |= 0x01; /* set tremelo flag bit */
                            vs->tremOscState = oscState;
                        }
                    }
                }
                vs->tremelo = (u8)oscValue;  /* will default if not changed by initOsc */

                oscValue = 1.0f;  /* set this as a default */
                if (cs->useEnv) {
                    tmp = cs->vibType;
                } else {
                    tmp = inst->vibType;
                }
                if (tmp)
                {
                    if (seqp->initOsc)
                    {
                        if (cs->useEnv) {
                            deltaTime = (*seqp->initOsc)(&oscState, &oscValue, cs->vibType,
                                                     cs->vibRate, cs->vibDepth, cs->vibDelay, cs->unk35);
                        } else {
                            deltaTime = (*seqp->initOsc)(&oscState, &oscValue, inst->vibType,
                                                     inst->vibRate, inst->vibDepth, inst->vibDelay, cs->unk35);
                        }
                        if (deltaTime)  /* if deltaTime = zero,don't run osc. */
                        {
                            evt.type = 24; /* AL_VIB_OSC_EVT */
                            evt.msg.osc.vs = vs;
                            evt.msg.osc.oscState = oscState;
                            evt.msg.osc.chan = chan;
                            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                            vs->flags |= 0x02; /* set the vibrato flag bit */
                            vs->vibOscState = oscState;
                        }
                    }
                }
                vs->vibrato = oscValue;  /* will default if not changed by initOsc */

                /*
                 * calculate the note on parameters
                 */
                pitch = vs->pitch * cs->pitchBend * vs->vibrato;
                fxmix = audio_getVoiceFxMix(vs, seqp);
                unk7A = cs->unk14;
                if (unk7A) {
                    unk70 = func_1001CEA4(cents / 100 + cs->unk15 - 0x40) * 440.0f * cs->pitchBend;
                } else {
                    unk70 = 127.0f;
                }
                pan = __n_vsPan(vs, (N_ALSeqPlayer*)seqp);
                vol = __n_vsVol(vs, (N_ALSeqPlayer*)seqp);
                if (cs->useEnv) {
                    deltaTime = cs->attackTime;
                } else {
                    deltaTime = sound->envelope->attackTime;
                }

                n_alSynStartVoiceParams(voice, sound->wavetable,
                                        pitch, vol, pan, fxmix, unk7A, unk70, cs->unk16, deltaTime);

                evt.type = AL_SEQP_ENV_EVT;
                evt.msg.vol.voice = voice;
                if (cs->useEnv) {
                    evt.msg.vol.vol = cs->decayVolume;
                    evt.msg.vol.delta = cs->decayTime;
                } else {
                    evt.msg.vol.vol = sound->envelope->decayVolume;
                    evt.msg.vol.delta = sound->envelope->decayTime;
                }
                n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);

                if (midi->duration) {
                    evt.type = 22; /* AL_CSP_NOTEOFF_EVT */
                    evt.msg.midi.status = chan | AL_MIDI_NoteOff;
                    evt.msg.midi.byte1 = key;
                    evt.msg.midi.byte2 = 0;
                    deltaTime = seqp->uspt * midi->duration;
                    D_80042810[chan] = deltaTime;
                    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                }
                if ((cs->unk17 & 1) && seqp->msgQueue) {
                    osSendMesg(seqp->msgQueue, (OSMesg)((D_80042810[chan] & ~0xFF) | (cs->unk17 >> 2)), OS_MESG_NOBLOCK);
                }
                break;
            }

            /*
             * NOTE: intentional fall-through for note on with zero
             * velocity
             */

        case (AL_MIDI_NoteOff):
            vs = __n_lookupVoice((N_ALSeqPlayer *)seqp, key, chan);
            ALFlagFailIf(!vs, (seqp->debugFlags & NOTE_OFF_ERR_MASK) &&
                         (seqp->debugFlags & NOTE_OFF_ERR_MASK), ERR_ALSEQP_OFF_VOICE );

            cs = &seqp->chanState[chan];
            if (vs->phase == AL_PHASE_SUSTAIN)
                vs->phase = AL_PHASE_SUSTREL;
            else
            {
                vs->phase = AL_PHASE_RELEASE;
                if (cs->useEnv) {
                    __n_seqpReleaseVoice((N_ALSeqPlayer*)seqp, &vs->voice, cs->releaseTime);
                } else {
                    __n_seqpReleaseVoice((N_ALSeqPlayer*)seqp, &vs->voice,
                                         vs->sound->envelope->releaseTime);
                }
            }
            if ((cs->unk17 & 2) && seqp->msgQueue) {
                osSendMesg(seqp->msgQueue, (OSMesg)((key << 16) | 8 | (cs->unk17 >> 2)), OS_MESG_NOBLOCK);
            }
            break;

        case (AL_MIDI_PolyKeyPressure):
            vs = __n_lookupVoice((N_ALSeqPlayer *)seqp, key, chan);
            ALFailIf(!vs,  ERR_ALSEQP_POLY_VOICE );

            vs->velocity = vel;
            n_alSynSetVol(&vs->voice,
                          __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
                          __n_vsDelta(vs, seqp->curTime));
            break;

        case (AL_MIDI_ChannelPressure):
            {
                N_ALVoiceState *vs;

                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if (vs->channel == chan) {
                        vs->velocity = byte1;
                        n_alSynSetVol(&vs->voice,
                                      __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
                                      __n_vsDelta(vs, seqp->curTime));
                    }
                }
            }
            break;

        case (AL_MIDI_ControlChange):
            {
                CSPCtrlHandler handler;

                if (byte1 < 0x5D) {
                    handler = D_8002BA50[byte1];
                } else if (byte1 >= 0xFC) {
                    handler = D_8002BA50[0x5D + (0xFF - byte1)];
                } else {
                    handler = NULL;
                }
                if (handler) {
                    if (0) {
                        /* compiled-out debug block; the shipped code keeps its dead test */
                        if (chan == 2) {
                        }
                    }
                    handler(seqp, event, chan, vel);
                }
            }
            break;

        case (AL_MIDI_ProgramChange):
            tmp = (seqp->chanState[chan].unk8 << 7) + key;
            if (tmp < seqp->bank->instCount) {
                if (audio_loadChannelProgram((N_ALSeqPlayer *)seqp, tmp, chan)) {
                    evt.type = 2; /* AL_SEQP_MIDI_EVT */
                    evt.msg.midi.ticks = 0;
                    evt.msg.midi.status = chan | AL_MIDI_ProgramChange;
                    evt.msg.midi.byte1 = key;
                    evt.msg.midi.byte2 = 0;
                    n_alEvtqPostEvent(&seqp->evtq, &evt, 33333, 0);
                }
            } else {
                /* __osError(ERR_ALSEQPINVALIDPROG, ...) compiled out */
            }
            break;

        case (AL_MIDI_PitchBendChange):
            {
                s32 bendVal;
                f32 bendRatio;
                s32 cents;
                N_ALVoiceState *vs;

                /* get 14-bit unsigned midi value */
                bendVal = ( (vel << 7) + byte1) - 8192;

                /* calculate pitch bend in cents */
                cents = (seqp->chanState[chan].bendRange * bendVal)/8192;

                /* calculate the corresponding ratio  */
                bendRatio = alCents2Ratio(cents);
                seqp->chanState[chan].pitchBend = bendRatio;

                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if (vs->channel == chan) {
                        n_alSynSetPitch(&vs->voice,
                                        vs->pitch * bendRatio * vs->vibrato);
                        if (seqp->chanState[chan].unk14) {
                            func_1001CA90(&vs->voice,
                                440.0f * func_1001CEA4(seqp->chanState[chan].unk15 + (vs->key - vs->sound->keyMap->keyBase) - 0x40)
                                * bendRatio * vs->vibrato);
                        }
                    }
                }
            }
            break;

        default:
            break;
    }
}

void __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
  ALTempoEvent    *tevt = &event->msg.tempo;
  s32             tempo;
  s32             oldUspt;
  u32             ticks;
  ALMicroTime         tempDelta,curDelta = 0;
  N_ALEventListItem     *thisNode,*nextNode,*firstTemp = 0;
  N_ALEventListItem     *temp0;
  ALLink                *temp1,*temp2;

  if (event->msg.tempo.status == AL_MIDI_Meta) {
    if (event->msg.tempo.type == AL_MIDI_META_TEMPO) {
      oldUspt = seqp->uspt;
      tempo = (tevt->byte1 << 16) | (tevt->byte2 <<  8) | (tevt->byte3 <<  0);
      __n_setUsptFromTempo (seqp, (f32)tempo);    /* sct 1/8/96 */

      thisNode = (N_ALEventListItem*)seqp->evtq.allocList.next;
      while (thisNode) {
          curDelta += thisNode->delta;
          nextNode = (N_ALEventListItem*)thisNode->node.next;
          if (thisNode->evt.type == 0x16 ) { // AL_CSP_NOTEOFF_EVT
              // custom
              temp0 = thisNode;
              if (temp0->node.next) {
                  temp0->node.next->prev = temp0->node.prev;
              }
              if (temp0->node.prev) {
                  temp0->node.prev->next = temp0->node.next;
              }
              if (firstTemp != 0) {
                  temp1 = (ALLink *)thisNode;
                  if (1) {
                      temp2 = (ALLink *)firstTemp;

                      temp1->next = temp2->next;
                      temp1->prev = temp2;

                      if (temp2->next != 0) {
                          temp2->next->prev = temp1;
                      }
                      temp2->next = temp1;
                  }
              } else {
                    thisNode->node.next = 0;
                    thisNode->node.prev = 0;
                    firstTemp = thisNode;
              }

              tempDelta = curDelta;                   /* record the current delta */
              if (nextNode)                           /* don't do this if no nextNode */ {
                  curDelta -= thisNode->delta;        /* subtract out this delta */
                  nextNode->delta += thisNode->delta; /* add it to next event */
              }
              thisNode->delta = tempDelta;            /* set this event delta from current */
          }
          thisNode = nextNode;
      }

      thisNode = firstTemp;
      while (thisNode) {
          nextNode = (N_ALEventListItem*)thisNode->node.next;
          ticks = thisNode->delta/oldUspt;
          thisNode->delta = ticks * seqp->uspt;
          __n_CSPRepostEvent(&seqp->evtq,thisNode);
          thisNode = nextNode;
      }
    }
  }
}

void __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item)
{
    N_ALEventListItem *thisNode;
    N_ALEventListItem *next;

    for (thisNode = (N_ALEventListItem *)&evtq->allocList; thisNode;
         thisNode = (N_ALEventListItem *)thisNode->node.next) {
        if (thisNode->node.next == 0) {
            ALLink *element = (ALLink *)item;
            ALLink *after = (ALLink *)thisNode;
            element->next = after->next;
            element->prev = after;
            if (after->next)
                after->next->prev = element;
            after->next = element;
            break;
        } else {
            next = (N_ALEventListItem *)thisNode->node.next;
            if (item->delta < next->delta) {
                next->delta -= item->delta;
                {
                    ALLink *element = (ALLink *)item;
                    ALLink *after = (ALLink *)thisNode;
                    element->next = after->next;
                    element->prev = after;
                    if (after->next)
                        after->next->prev = element;
                    after->next = element;
                }
                break;
            }
            item->delta -= next->delta;
        }
    }
}

void __n_setUsptFromTempo (N_ALCSPlayer *seqp, f32 tempo)
{
  if (seqp->target)
    seqp->uspt = (s32)((f32)tempo * seqp->target->qnpt);
  else
    seqp->uspt = 488;    /* This is the initial value set by alSeqpNew. */
}

void __n_CSPPostNextSeqEvent(N_ALCSPlayer *seqp)
{
  N_ALEvent   evt;
  s32    deltaTicks;

  if (seqp->state != AL_PLAYING || seqp->target == NULL)
    return;

  /* Get the next event time in ticks. */
  /* If false is returned, then there is no next delta (ie. end of sequence reached). */
  if (!__alCSeqNextDelta(seqp->target, &deltaTicks))
    return;

  evt.type = AL_SEQ_REF_EVT;
  n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt, 0);
}
