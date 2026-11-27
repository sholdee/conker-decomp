#include <ultra64.h>
#include <n_libaudio.h>


void audio_setCspMsgQueue(N_ALCSPlayer *seqp, OSMesgQueue *queue) {
    seqp->msgQueue = queue;
}

void audio_setCspNoteMesg(N_ALCSPlayer *seqp, s32 chan, u8 arg2) {
    seqp->chanState[chan].noteMesg = arg2;
}

void audio_enableAllCspChannels(N_ALCSPlayer *seqp) {
    s32 chan;

    seqp->chanMask = 0xFFFF;
    for (chan = 0; chan < seqp->maxChannels; chan++)
    {
        seqp->chanState[chan].fadeTarget = 0xFF;
        seqp->chanState[chan].fadeVol = 0xFF;
    }
}

void audio_muteCspChannel(N_ALCSPlayer *seqp, s32 arg1) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 252, 0);
}

void audio_unmuteCspChannel(N_ALCSPlayer *seqp, s32 arg1) {
    seqp->chanMask |= 1 << arg1;
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, 0xFF);
}

void audio_fadeCspChannel(N_ALCSPlayer *seqp, s32 arg1, u8 arg2, u8 arg3) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFD, arg3);
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFF, arg2);
}

void func_10017CE0(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0x41, arg2);
}

void audio_setCspChannelFade(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, arg2);
}
