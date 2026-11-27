#include <n_libaudio.h>


/* Posts event 26: the sequence player sets a parameter of an aux bus's effect (n_alSynSetFXParam), or of the
 * bus's low-pass when arg2 is 8 or more (audio_setBusLowPassParam). */
void audio_setCspFxParam(N_ALCSPlayer *seqp, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {

    N_ALEvent evt;
    evt.type = 0x1A;
    evt.msg.unknown2.unk0 = arg1;
    evt.msg.unknown2.unk1 = arg2;
    evt.msg.unknown2.unk2 = arg3;
    evt.msg.unknown2.unk4 = arg4;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
