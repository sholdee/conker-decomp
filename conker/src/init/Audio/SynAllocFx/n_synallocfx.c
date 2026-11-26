#include <ultra64.h>

#include "n_libaudio.h"
#include "libultra/audio/n_synthInternals.h"

/* Creates the effect of aux bus `bus` (n_alSynNew) and returns it. */
ALFxRef func_1001E4A0(s16 bus, ALSynConfig *config, ALHeap *heap) {
    n_alFxNew(&n_syn->auxBus[bus].fx, config, bus, heap);
    return n_syn->auxBus[bus].fx;
}
