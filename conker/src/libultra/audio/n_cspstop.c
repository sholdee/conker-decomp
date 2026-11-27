#include <libaudio.h>
#include "n_libaudio.h"
#include "n_seqp.h"


void n_alCSPStop(N_ALCSPlayer *seqp) {
    N_ALEvent evt;

    evt.type = AL_SEQP_STOPPING_EVT;
    n_alEvtqPostEvent(&seqp->evtq, &evt, 0, 2);
}
