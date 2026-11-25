#include <n_libaudio.h>

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan);

// File-local view of the audio driver object (the N_ALSynth the players hang off
// of) passed to func_1001BD34/func_1001BE1C. It exposes a resource-release
// callback at 0x30 that is handed an ALInstrument or one of its ALSounds, and a
// loader-proc getter at 0x28 (see BankScanObj). Not typed in the shared headers yet.
typedef struct {
    u8   pad0[0x30];
    void (*releaseProc)(void *);
} InitBankCallbacks;

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
            seqp->usedVoices--;
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

    if (seqp->usedVoices > seqp->maxVoices) {
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

        seqp->usedVoices++;
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
u8 func_1001B310(N_ALVoiceState *vs, N_ALCSPlayer *seqp) {
    s32 sign;
    s32 fxmix;

    sign = seqp->chanState[vs->channel].fxmix & 0x80;
    fxmix = ((seqp->chanState[vs->channel].fxmix & 0x7F) + (s32)(seqp->unk7C * 127.0f)) * seqp->unk80;

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

ALPan __n_vsPan(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{
    s32 tmp;

    tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER +
        vs->sound->samplePan;
    tmp = MAX(tmp, AL_PAN_LEFT);
    tmp = MIN(tmp, AL_PAN_RIGHT);

    return (ALPan) tmp;
}

// not vanilla
void __n_initFromBank(N_ALSeqPlayer *seqp, ALBank *b)
{
    s32 i;
    ALInstrument *inst;

    inst = NULL;
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

void __n_initChanState(N_ALSeqPlayer *seqp)
{
    int i;

    for (i = 0; i < seqp->maxChannels; i++)
    {
        seqp->chanState[i].instrument = 0;
        __n_resetPerfChanState (seqp, i);
    }
}

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan) {
    seqp->chanState[chan].fxmix = 0;
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

s32 func_1001BD34();
void func_10012C5C(void *arg0, s32 arg1, s32 arg2);

// ALChanState (0x3C bytes) with Rare's pad1C[0x20] tail broken out: the tail
// caches the selected instrument's envelope and its tremolo/vibrato settings,
// plus the bank instrument index the channel is currently holding.
typedef struct {
    void *instrument;
    s16   bendRange;
    u8    pan;
    u8    priority;
    u8    unk8;
    u8    vol;
    u8    padA[0x12];
    s32   attackTime;
    s32   decayTime;
    s32   releaseTime;
    u8    unk28;
    u8    attackVolume;
    u8    decayVolume;
    u8    unk2B;
    u8    tremType;
    u8    tremRate;
    u8    tremDepth;
    u8    tremDelay;
    u8    vibType;
    u8    vibRate;
    u8    vibDepth;
    u8    vibDelay;
    u8    unk34;
    u8    unk35;
    u8    instMissing;
    u8    unk37;
    s16   instNum;
    u8    pad3A[0x2];
} B7DChanState;

// Same driver object as InitBankCallbacks/BankScanObj, seen from func_1001B7D0.
typedef struct {
    u8    pad0[0x34];
    void (*releaseProc)(void *);
    s32   unk38;
} B7DBankCb;

// N_ALSeqPlayer view: drvr at 0x14, ALBank at 0x20, chanState at 0x60.
typedef struct {
    u8            pad0[0x14];
    B7DBankCb    *drvr;
    u8            pad18[0x8];
    ALBank       *bank;
    u8            pad24[0x3C];
    B7DChanState *chanState;
} B7DSeqp;

s32 func_1001B7D0(B7DSeqp *seqp, s32 instNum, s32 chan) {
    ALSound *sound;
    ALInstrument *inst;
    s32 i;

    inst = (ALInstrument *)func_1001BD34(seqp->drvr, &seqp->bank->instArray[instNum], -1);
    if (seqp->chanState[chan].instrument != 0) {
        seqp->drvr->releaseProc(seqp->bank->instArray[seqp->chanState[chan].instNum]);
        seqp->chanState[chan].instrument = 0;
    }
    if (inst != 0) {
        for (i = 0; i < inst->soundCount; i++) {
            sound = inst->soundArray[i];
            if ((u32) sound->envelope < 0x100000) {
                func_10012C5C(sound, (s32) inst, seqp->drvr->unk38);
            }
        }
        sound = inst->soundArray[0];
    }
    if (inst != 0) {
        if (inst->soundCount == 0) {
            return 0;
        }
        if (sound != 0) {
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
        seqp->chanState[chan].instMissing = 0;
        seqp->chanState[chan].instrument = inst;
    } else {
        seqp->chanState[chan].instMissing = 1;
    }
    seqp->chanState[chan].unk2B = 0;
    seqp->chanState[chan].unk28 = 0;
    seqp->chanState[chan].unk35 = 0;
    seqp->chanState[chan].instNum = instNum;
    if (inst == 0) {
        return 1;
    }
    return 0;
}
// The loader proc resolves the pointer whose address it is handed (an
// instArray[] or soundArray[] slot) and returns its RAM address.
typedef s32 (*BankScanCb)(void *, s32);
typedef BankScanCb (*BankScanGetter)(void);

// Same driver object again, seen from func_1001BD34.
typedef struct {
    u8             pad0[0x28];
    BankScanGetter getLoadProc;
} BankScanObj;

s32 func_1001BD34(BankScanObj *drvr, void **ptrRef, s32 idx) {
    BankScanCb loadProc;
    s32 addr;

    addr = 0;
    loadProc = drvr->getLoadProc();
    if (loadProc != 0) {
        if (idx == -1) {
            addr = loadProc(ptrRef, 1);
        } else {
            addr = loadProc((u8 *)*ptrRef + idx * 4 + 0x10, 0);
        }
        if (addr != 0 && (addr & 0xFF000003) != 0x80000000) {
            return 0;
        }
    } else {
        return 0;
    }
    return addr;
}

void func_1001BE1C(InitBankCallbacks *drvr, ALInstrument *inst, s32 idx) {
    if (idx == -1) {
        drvr->releaseProc(inst);
    } else {
        drvr->releaseProc(inst->soundArray[idx]);
    }
}

// N_ALEventListItem again; here the event payload is an ALOscEvent, whose first
// two members are the voice state and the oscillator state.
typedef struct OscNode {
    struct OscNode *next;
    struct OscNode *prev;
    s32             delta;
    s16             type;
    u8              padE[0x2];
    void           *vs;
    void           *oscState;
} OscNode;

typedef struct {
    u8       pad0[0x48];
    OscNode *evtqFreeList;
    u8       pad4C[0x4];
    OscNode *evtqAllocList;
    u8       pad54[0x24];
    void   (*stopOsc)(void *);
} OscSeqp;

void __n_seqpStopOsc(OscSeqp *seqp, N_ALVoiceState *vs) {
    OscNode *thisNode;
    OscNode *nextNode;
    s16 type;
    OscNode *unlinkMe;
    OscNode *relinkMe;
    OscNode **freeList;

    thisNode = seqp->evtqAllocList;
    if (thisNode != 0) {
        do {
            nextNode = thisNode->next;
            type = thisNode->type;
            if (type == 0x17 || type == 0x18) {
                if (thisNode->vs == vs) {
                    seqp->stopOsc(thisNode->oscState);
                    unlinkMe = thisNode;
                    if (unlinkMe->next != 0) {
                        unlinkMe->next->prev = unlinkMe->prev;
                    }
                    if (unlinkMe->prev != 0) {
                        unlinkMe->prev->next = unlinkMe->next;
                    }
                    if (nextNode != 0) {
                        nextNode->delta += thisNode->delta;
                    }
                    relinkMe = thisNode;
                    freeList = &seqp->evtqFreeList;
                    relinkMe->next = *freeList;
                    relinkMe->prev = (OscNode *)freeList;
                    if (*freeList != 0) {
                        (*freeList)->prev = relinkMe;
                    }
                    *freeList = relinkMe;
                    if (type == 0x17) {
                        vs->flags &= 0xFE;
                    } else {
                        vs->flags &= 0xFD;
                    }
                    if (vs->flags == 0) {
                        return;
                    }
                }
            }
            thisNode = nextNode;
        } while (thisNode != 0);
    }
}
