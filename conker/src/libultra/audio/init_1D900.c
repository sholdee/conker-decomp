#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"




void n_alSynSetFXMix( N_ALVoice *v, u8 fxmix) {
    ALParam  *update;

    if (v->pvoice) {
        update = __n_allocParam();
        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);

        update->delta  = n_syn->paramSamples + v->pvoice->offset;
        update->type   = AL_FILTER_SET_FXAMT;
        update->data.i = fxmix;
        update->next   = 0;
        n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);
    }
}

void *func_1001D9B0(s16 arg0) {
    N_ALMainBus *sp4;

    sp4 = n_syn->mainBus;
    if (sp4->filter.handler == (N_ALCmdHandler)func_1001E530) {
        return n_syn->auxBus[arg0].sources;
    } else {
        return 0;
    }
}

LowPass *func_1001DA28(s16 arg0) {
    N_ALMainBus *sp4;

    sp4 = n_syn->mainBus;
    if (sp4->filter.handler == (N_ALCmdHandler)func_1001E530) {
        return (LowPass *) n_syn->auxBus[arg0].fx_array[7];
    } else {
        return 0;
    }
}

void func_1001DAA0(void *fx, s16 paramID, void *value) {
    void *sp1C = fx;
    n_alFxParamHdl(sp1C, paramID, value);
}

void func_1001DAE4(LowPass *lp, s16 paramID, s32 *value) {
    if (paramID == 8) {
        lp->fgain = *value * 0.1f;
    } else if (paramID == 9) {
        lp->fc = *value;
    }
    func_1001CF38(lp, n_syn->outputRate);
}
