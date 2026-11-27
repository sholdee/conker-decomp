#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


/* Posts filter type 18: n_alLoadParam (init/Audio/VoiceLowPass/voicelowpass.c) stores the value as
 * the fgain of the voice's low-pass. */
void audio_setVoiceLowPassGain( N_ALVoice *v, u8 fgain)
{
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
        update->type   = 18;
        update->data.i = fgain;
        update->next   = 0;

	n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}
