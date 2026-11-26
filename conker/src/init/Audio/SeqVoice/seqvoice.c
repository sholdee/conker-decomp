#include <n_libaudio.h>

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan);

/* Unlinks the voice state that owns `voice` from the allocated list and returns it to the free list. */
void func_1001AAE0(N_ALSeqPlayer *seqp, N_ALVoice *voice) {
    N_ALVoiceState *prev = NULL;
    N_ALVoiceState *vs;

    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if (&vs->voice == voice) {
            if (prev) {
                prev->next = vs->next;
            } else {
                seqp->vAllocHead = vs->next;
            }
            if (vs == seqp->vAllocTail) {
                seqp->vAllocTail = prev;
            }
            vs->next = seqp->vFreeList;
            seqp->vFreeList = vs;
            seqp->voiceCount--;
            return;
        }
        prev = vs;
    }
}

/* libaudio's alUnlink()/alLink(), expanded inline as Conker's libaudio does. */
#define AL_UNLINK(ln) {                                     \
    ALLink *unlinkMe = (ln);                                \
    if (unlinkMe->next) {                                   \
        unlinkMe->next->prev = unlinkMe->prev;              \
    }                                                       \
    if (unlinkMe->prev) {                                   \
        unlinkMe->prev->next = unlinkMe->next;              \
    }                                                       \
}

#define AL_LINK(ln, to) {                                   \
    ALLink *relinkMe = (ln);                                \
    ALLink *after = (to);                                   \
    relinkMe->next = after->next;                           \
    relinkMe->prev = after;                                 \
    if (after->next) {                                      \
        after->next->prev = relinkMe;                       \
    }                                                       \
    after->next = relinkMe;                                 \
}

void __n_seqpReleaseVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime deltaTime) {
    N_ALEvent evt;
    N_ALVoiceState *vs = voice->unk10;

    /*
     * if in attack phase, remove all pending volume
     * events for this voice from the queue
     */
    if (vs->envPhase == AL_PHASE_ATTACK) {
        ALLink *thisNode;
        ALLink *nextNode;
        N_ALEventListItem *thisItem;
        N_ALEventListItem *nextItem;

        thisNode = seqp->evtq.allocList.next;
        while (thisNode != NULL) {
            nextNode = thisNode->next;
            thisItem = (N_ALEventListItem *)thisNode;
            nextItem = (N_ALEventListItem *)nextNode;
            if (thisItem->evt.type == AL_SEQP_ENV_EVT) {
                if (thisItem->evt.msg.vol.voice == voice) {
                    if (nextItem) {
                        nextItem->delta += thisItem->delta;
                    }
                    AL_UNLINK(thisNode);
                    AL_LINK(thisNode, &seqp->evtq.freeList);
                }
            }
            thisNode = nextNode;
        }
    }

    vs->velocity = 0;
    vs->envPhase = AL_PHASE_RELEASE;
    vs->envGain = 0;
    vs->envEndTime = seqp->curTime + deltaTime;

    n_alSynSetPriority(voice, 0); /* make candidate for stealing */
    n_alSynSetVol(voice, 0, deltaTime);

    evt.type = AL_NOTE_END_EVT;
    evt.msg.note.voice = voice;

    deltaTime += AL_USEC_PER_FRAME * 2;
    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
}

/*
 * Returns whether `voice` still needs a note kill at `killTime`: a pending
 * note-end event for it that falls after `killTime` is dropped from the queue
 * (TRUE), while one at or before `killTime` will end the note anyway (FALSE).
 */
s32 func_1001ADA4(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime) {
    ALLink *thisNode;
    ALLink *nextNode;
    N_ALEventListItem *thisItem;
    ALMicroTime itemTime = 0;
    u8 needsNoteKill = TRUE;

    thisNode = seqp->evtq.allocList.next;
    while (thisNode != NULL) {
        nextNode = thisNode->next;
        thisItem = (N_ALEventListItem *)thisNode;
        itemTime += thisItem->delta;
        if (thisItem->evt.type == AL_NOTE_END_EVT) {
            if (thisItem->evt.msg.note.voice == voice) {
                if (itemTime > killTime) {
                    if (nextNode) {
                        ((N_ALEventListItem *)nextNode)->delta += thisItem->delta;
                    }
                    AL_UNLINK(thisNode);
                    AL_LINK(thisNode, &seqp->evtq.freeList);
                } else {
                    needsNoteKill = FALSE;
                }
                break;
            }
        }
        thisNode = nextNode;
    }
    return needsNoteKill;
}

/* Takes a voice state off the free list and appends it to the allocated list for this note. */
N_ALVoiceState *__n_mapVoice(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 channel) {
    N_ALVoiceState *vs = seqp->vFreeList;

    if (seqp->voiceCount > seqp->voiceLimit) {
        return NULL;
    }

    if (vs) {
        seqp->vFreeList = vs->next;
        vs->next = NULL;

        if (!seqp->vAllocHead) {
            seqp->vAllocHead = vs;
        } else {
            seqp->vAllocTail->next = vs;
        }
        seqp->vAllocTail = vs;

        vs->channel = channel;
        vs->key = key;
        vs->velocity = vel;
        vs->voice.unk10 = vs; /* the SDK's clientPrivate */

        seqp->voiceCount++;
    }

    return vs;
}

/* Finds the allocated voice still sounding `key` on `channel` (not yet released). */
N_ALVoiceState *func_1001AFEC(N_ALSeqPlayer *seqp, u8 key, u8 channel) {
    N_ALVoiceState *vs = seqp->vAllocHead;

    while (vs) {
        if (vs->key == key && vs->channel == channel &&
            vs->phase != AL_PHASE_RELEASE && vs->phase != AL_PHASE_SUSTREL) {
            return vs;
        }
        vs = vs->next;
    }
    return NULL;
}

/* Binary-searches the channel instrument's sounds for the one whose keymap covers key and vel. */
ALSound *func_1001B07C(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan) {
    ALInstrument *inst = seqp->chanState[chan].instrument;
    s32 lo = 1;
    s32 hi;
    s32 mid;
    ALKeyMap *keyMap;

    if (inst == NULL) {
        return NULL;
    }
    hi = inst->soundCount;
    while (hi >= lo) {
        mid = (lo + hi) / 2;
        keyMap = inst->soundArray[mid - 1]->keyMap;
        if (key >= keyMap->keyMin && key <= keyMap->keyMax &&
            vel >= keyMap->velocityMin && vel <= keyMap->velocityMax) {
            return inst->soundArray[mid - 1];
        } else if (key < keyMap->keyMin ||
                   (vel < keyMap->velocityMin && key <= keyMap->keyMax)) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return NULL;
}

/*
 * __n_vsVol calculates the target volume for the voice based on the
 * note on velocity, envelope, sampleVolume and controller.
 */
s16 __n_vsVol(N_ALVoiceState *vs, N_ALSeqPlayer *seqp) {
    u32 t1, t2;

    t1 = (vs->tremelo * vs->velocity * vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume * seqp->vol * seqp->chanState[vs->channel].vol) >> 14;

    if (seqp->chanState[vs->channel].unkD != 0xFF) {
        t2 = ((seqp->chanState[vs->channel].unkD * t2) + 1) >> 8;
    }

    t1 *= t2;
    t1 >>= 15;

    return t1;
}

/* The voice's fx mix: the channel's mix scaled by the player's mix controls, keeping bit 7. */
u8 audio_getVoiceFxMix(N_ALVoiceState *vs, N_ALCSPlayer *seqp) {
    s32 sign;
    s32 fxmix;

    sign = seqp->chanState[vs->channel].fxmix & 0x80;
    fxmix = ((seqp->chanState[vs->channel].fxmix & 0x7F) + (s32)(seqp->fxMixOffset * 127.0f)) * seqp->fxMixScale;

    return MAX(0, MIN(127, fxmix)) | sign;
}

ALMicroTime __n_vsDelta(N_ALVoiceState *vs, ALMicroTime t) {
    /*
     * If we are interrupting a previously set envelope segment, we
     * need to recalculate the segment end time given the current
     * time. Note: this routine assumes that the voice is currently
     * playing.
     */
    s32 delta = vs->envEndTime - t;

    if (delta >= 0) {
        return delta;
    } else {
        return AL_GAIN_CHANGE_TIME;
    }
}

ALPan __n_vsPan(N_ALVoiceState *vs, N_ALSeqPlayer *seqp) {
    s32 tmp;

    tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER + vs->sound->samplePan;
    tmp = MAX(tmp, AL_PAN_LEFT);
    tmp = MIN(tmp, AL_PAN_RIGHT);

    return (ALPan)tmp;
}

/* Unlike the SDK version, this only resets the channels' performance state. */
void __n_initFromBank(N_ALSeqPlayer *seqp, ALBank *b) {
    s32 i;
    ALInstrument *inst = NULL;

    /* set to the first available instrument (left unused here). */
    for (i = 1; inst == NULL; i++) {
        inst = b->instArray[i];
    }

    for (i = 0; i < seqp->maxChannels; i++) {
        __n_resetPerfChanState(seqp, i);
    }

    if (b->percussion) {
        __n_resetPerfChanState(seqp, i);
    }
}

void __n_initChanState(N_ALSeqPlayer *seqp) {
    int i;

    for (i = 0; i < seqp->maxChannels; i++) {
        seqp->chanState[i].instrument = NULL;
        __n_resetPerfChanState(seqp, i);
    }
}

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan) {
    seqp->chanState[chan].fxmix = AL_DEFAULT_FXMIX;
    seqp->chanState[chan].pan = AL_PAN_CENTER;
    seqp->chanState[chan].vol = AL_VOL_FULL;
    seqp->chanState[chan].priority = AL_DEFAULT_PRIORITY;
    seqp->chanState[chan].sustain = 0;
    seqp->chanState[chan].bendRange = 200;
    seqp->chanState[chan].pitchBend = 1.0f;
    seqp->chanState[chan].unk17 = 0;
    seqp->chanState[chan].unkD = 255;
    seqp->chanState[chan].unkE = 255;
    seqp->chanState[chan].unkF = 0;
    seqp->chanState[chan].unkB = 0;
    seqp->chanState[chan].unk16 = 0;
    seqp->chanState[chan].unk15 = 0;
    seqp->chanState[chan].unk14 = 0;
    seqp->chanState[chan].unk8 = 0;
}


/*
 * Loads bank program `instNum` onto channel `chan`, releasing the instrument
 * the channel held before and copying the new one's settings into the channel.
 * Returns 1 if the program could not be loaded.
 */
s32 audio_loadChannelProgram(N_ALSeqPlayer *seqp, s32 instNum, s32 chan) {
    ALSound *sound;
    ALInstrument *inst;
    s32 i;

    inst = audio_loadInstrument(seqp->drvr, &seqp->bank->instArray[instNum], -1);
    if (seqp->chanState[chan].instrument != NULL) {
        seqp->drvr->bankRelease(seqp->bank->instArray[seqp->chanState[chan].instNum]);
        seqp->chanState[chan].instrument = NULL;
    }
    if (inst != NULL) {
        for (i = 0; i < inst->soundCount; i++) {
            sound = inst->soundArray[i];
            /* a small envelope "pointer" is still an offset from the instrument */
            if ((u32)sound->envelope < 0x100000) {
                func_10012C5C(sound, (s32)inst, seqp->drvr->waveTableBase);
            }
        }
        sound = inst->soundArray[0];
    }
    if (inst != NULL) {
        if (inst->soundCount == 0) {
            return 0;
        }
        if (sound != NULL) {
            seqp->chanState[chan].attackTime = sound->envelope->attackTime;
            seqp->chanState[chan].decayTime = sound->envelope->decayTime;
            seqp->chanState[chan].releaseTime = sound->envelope->releaseTime;
            seqp->chanState[chan].attackVolume = sound->envelope->attackVolume;
            seqp->chanState[chan].decayVolume = sound->envelope->decayVolume;
        }
        seqp->chanState[chan].pan = inst->pan;
        seqp->chanState[chan].vol = inst->volume;
        seqp->chanState[chan].priority = inst->priority;
        seqp->chanState[chan].bendRange = inst->bendRange;
        seqp->chanState[chan].tremType = inst->tremType;
        seqp->chanState[chan].tremRate = inst->tremRate;
        seqp->chanState[chan].tremDepth = inst->tremDepth;
        seqp->chanState[chan].tremDelay = inst->tremDelay;
        seqp->chanState[chan].vibType = inst->vibType;
        seqp->chanState[chan].vibRate = inst->vibRate;
        seqp->chanState[chan].vibDepth = inst->vibDepth;
        seqp->chanState[chan].vibDelay = inst->vibDelay;
        seqp->chanState[chan].instMissing = FALSE;
        seqp->chanState[chan].instrument = inst;
    } else {
        seqp->chanState[chan].instMissing = TRUE;
    }
    seqp->chanState[chan].detune = 0;
    seqp->chanState[chan].useEnv = 0;
    seqp->chanState[chan].unk35 = 0;
    seqp->chanState[chan].instNum = instNum;
    if (inst == NULL) {
        return 1;
    }
    return 0;
}

/*
 * Asks the driver's loader to resolve an instrument slot (soundIdx == -1) or
 * one of the instrument's soundArray[] slots, returning the loaded object's
 * RAM address, or NULL if there is no loader or the result is not a
 * word-aligned KSEG0 address.
 */
void *audio_loadInstrument(N_ALSynth *drvr, ALInstrument **instSlot, s32 soundIdx) {
    BankLoadProc loadProc;
    void *addr = NULL;

    loadProc = drvr->getLoadProc();
    if (loadProc != NULL) {
        if (soundIdx == -1) {
            addr = loadProc(instSlot, TRUE);
        } else {
            addr = loadProc(&(*instSlot)->soundArray[soundIdx], FALSE);
        }
        if (addr != NULL && ((u32)addr & 0xFF000003) != 0x80000000) {
            return NULL;
        }
    } else {
        return NULL;
    }
    return addr;
}

/* Releases an instrument (soundIdx == -1) or one of its sounds through the driver. */
void audio_releaseInstrument(N_ALSynth *drvr, ALInstrument *inst, s32 soundIdx) {
    if (soundIdx == -1) {
        drvr->bankAddRef(inst);
    } else {
        drvr->bankAddRef(inst->soundArray[soundIdx]);
    }
}

/* Conker's event numbering puts the oscillator events one past libaudio.h's enum ALMsg. */
#define TREM_OSC_EVT 23
#define VIB_OSC_EVT  24

/* Stops and removes the pending tremolo/vibrato oscillator events of voice state vs. */
void __n_seqpStopOsc(N_ALSeqPlayer *seqp, N_ALVoiceState *vs) {
    N_ALEventListItem *thisNode;
    N_ALEventListItem *nextNode;
    s16 evtType;

    thisNode = (N_ALEventListItem *)seqp->evtq.allocList.next;
    while (thisNode != NULL) {
        nextNode = (N_ALEventListItem *)thisNode->node.next;
        evtType = thisNode->evt.type;
        if (evtType == TREM_OSC_EVT || evtType == VIB_OSC_EVT) {
            if (thisNode->evt.msg.osc.vs == vs) {
                (*seqp->stopOsc)(thisNode->evt.msg.osc.oscState);
                AL_UNLINK((ALLink *)thisNode);
                if (nextNode != NULL) {
                    nextNode->delta += thisNode->delta;
                }
                AL_LINK((ALLink *)thisNode, &seqp->evtq.freeList);
                if (evtType == TREM_OSC_EVT) {
                    vs->flags &= 0xFE; /* tremolo off */
                } else {
                    vs->flags &= 0xFD; /* vibrato off */
                }
                if (vs->flags == 0) {
                    return; /* there should be no more events */
                }
            }
        }
        thisNode = nextNode;
    }
}
