#include <ultra64.h>
#include <n_libaudio.h>


void func_10017AF0(N_ALCSPlayer *seqp, OSMesgQueue *queue) {
    seqp->msgQueue = queue;
}

void func_10017B04(N_ALCSPlayer *seqp, s32 chan, u8 arg2) {
    seqp->chanState[chan].noteMesg = arg2;
}

void func_10017B30(N_ALCSPlayer *seqp) {
    s32 chan;

    seqp->chanMask = 0xFFFF;
    for (chan = 0; chan < seqp->maxChannels; chan++)
    {
        seqp->chanState[chan].fadeTarget = 0xFF;
        seqp->chanState[chan].fadeVol = 0xFF;
    }
}

void func_10017BB8(N_ALCSPlayer *seqp, s32 arg1) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 252, 0);
}

void func_10017C00(N_ALCSPlayer *seqp, s32 arg1) {
    seqp->chanMask |= 1 << arg1;
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, 0xFF);
}

void func_10017C68(N_ALCSPlayer *seqp, s32 arg1, u8 arg2, u8 arg3) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFD, arg3);
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFF, arg2);
}

void func_10017CE0(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0x41, arg2);
}

void func_10017D30(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, arg2);
}
