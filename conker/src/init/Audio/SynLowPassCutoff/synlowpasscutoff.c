#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


/* Posts filter type 19: n_alLoadParam (init/Audio/VoiceLowPass/voicelowpass.c) stores the value as
 * the cutoff (fc) of the voice's low-pass. */
void audio_setVoiceLowPassCutoff( N_ALVoice *v, f32 fc) {
    ALParam  *update;

    if (v->pvoice) {
        /*
         * get new update struct from the free list
         */

        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        /*
         * set offset and data
         */
#ifdef SAMPLE_ROUND
        update->delta  = SAMPLE184( n_syn->paramSamples + v->pvoice->offset);
#else
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
#endif
        update->type   = 19;
        update->data.f = fc;
        update->next   = 0;

        n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
      }
}
