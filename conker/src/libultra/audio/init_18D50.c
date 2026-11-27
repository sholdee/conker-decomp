#include "n_libaudio.h"


void func_10018D50(N_ALCSPlayer *seqp)
{
    N_ALEvent evt;

    evt.type            = AL_SEQP_PAUSE_EVT;

    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
