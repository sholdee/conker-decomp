#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


void n_alSynSetFXParam(void *fx, s16 paramID, void *value) {
    void *sp1C = fx;
    n_alFxParamHdl(sp1C, paramID, value);
}

/* Sets a parameter of a bus's low-pass: 8 is the gain (in tenths), 9 the cutoff. Then the coefficients
 * are computed again. */
void audio_setBusLowPassParam(LowPass *lp, s16 paramID, s32 *value) {
    if (paramID == 8) {
        lp->fgain = *value * 0.1f;
    } else if (paramID == 9) {
        lp->fc = *value;
    }
    audio_updateLowPassCoefs(lp, n_syn->outputRate);
}
