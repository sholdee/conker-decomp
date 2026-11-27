#include <n_libaudio.h>


/* Posts event 25: the sequence player stores the two values as fxMixOffset and fxMixScale and sets the
 * effect mix of its voices again. */
void audio_setCspFxMix(N_ALCSPlayer *csp, f32 arg1, f32 arg2) {
    N_ALEvent event;

    event.type = 25;
    event.msg.unknown0.unk0 = arg1;
    event.msg.unknown0.unk4 = arg2;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}

/* Sends controller 0x5C (92, audio_ctrlFxBus): the aux bus of the channel's new voices. */
void audio_setCspChannelFxBus(N_ALCSPlayer *csp, u8 chan, u8 arg2) {
    N_ALEvent event;

    event.type = 2; // AL_SEQP_MIDI_EVT ?
    /* likely a custom event */
    event.msg.midi.ticks = 0;
    event.msg.midi.status = chan | 0xB0;
    event.msg.midi.byte1 = 92;
    event.msg.midi.byte2 = arg2;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}
