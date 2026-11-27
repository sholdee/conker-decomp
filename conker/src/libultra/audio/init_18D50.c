#include "n_libaudio.h"


void audio_pauseCsp(N_ALCSPlayer *seqp)
{
    N_ALEvent evt;

    evt.type            = AL_SEQP_PAUSE_EVT;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
