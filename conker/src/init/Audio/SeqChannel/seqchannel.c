#include <ultra64.h>
#include <n_libaudio.h>


/* The queue that the player's note and controller messages go to (audio_ctrlSendMesg, noteMesg). */
void audio_setCspMsgQueue(N_ALCSPlayer *seqp, OSMesgQueue *queue) {
    seqp->msgQueue = queue;
}

/* Sets the channel's note messages (ALChanState.noteMesg). */
void audio_setCspNoteMesg(N_ALCSPlayer *seqp, s32 chan, u8 arg2) {
    seqp->chanState[chan].noteMesg = arg2;
}

/* Puts every channel in the channel mask with full fade volume. */
void audio_enableAllCspChannels(N_ALCSPlayer *seqp) {
    s32 chan;

    seqp->chanMask = 0xFFFF;
    for (chan = 0; chan < seqp->maxChannels; chan++)
    {
        seqp->chanState[chan].fadeTarget = 0xFF;
        seqp->chanState[chan].fadeVol = 0xFF;
    }
}

/* Controller 0xFC with 0 (audio_ctrlFadeSet): fade volume 0, which takes the channel out of the channel mask. */
void audio_muteCspChannel(N_ALCSPlayer *seqp, s32 arg1) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 252, 0);
}

/* Puts the channel in the channel mask and sets its fade volume to 255 (controller 0xFC). */
void audio_unmuteCspChannel(N_ALCSPlayer *seqp, s32 arg1) {
    seqp->chanMask |= 1 << arg1;
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, 0xFF);
}

/* Fades the channel to volume `arg2` in `arg3` steps: controller 0xFD (audio_ctrlFadeTime), then 0xFF
 * (audio_ctrlFadeTo). */
void audio_fadeCspChannel(N_ALCSPlayer *seqp, s32 arg1, u8 arg2, u8 arg3) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFD, arg3);
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFF, arg2);
}

void func_10017CE0(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0x41, arg2);
}

/* Sets the channel's fade volume to `arg2` at once (controller 0xFC, audio_ctrlFadeSet). */
void audio_setCspChannelFade(N_ALCSPlayer *seqp, s32 arg1, u8 arg2) {
    n_alCSPSendMidi(seqp, 0, arg1 | 0xB0, 0xFC, arg2);
}
