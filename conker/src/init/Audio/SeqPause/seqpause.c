#include "n_libaudio.h"


/* Posts Rare's pause event: a playing sequence player keeps its place (state 3) until n_alCSPPlay. */
void audio_pauseCsp(N_ALCSPlayer *seqp)
{
    N_ALEvent evt;

    evt.type            = AL_SEQP_PAUSE_EVT;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
