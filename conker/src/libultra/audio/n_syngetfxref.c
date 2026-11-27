#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


void *n_alSynGetFXRef(s16 arg0) {
    N_ALMainBus *sp4;

    sp4 = n_syn->mainBus;
    if (sp4->filter.handler == (N_ALCmdHandler)n_alFxPull) {
        return n_syn->auxBus[arg0].sources;
    } else {
        return 0;
    }
}

LowPass *audio_getBusLowPass(s16 arg0) {
    N_ALMainBus *sp4;

    sp4 = n_syn->mainBus;
    if (sp4->filter.handler == (N_ALCmdHandler)n_alFxPull) {
        return (LowPass *) n_syn->auxBus[arg0].fx_array[7];
    } else {
        return 0;
    }
}
