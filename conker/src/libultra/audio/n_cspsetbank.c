#include <n_libaudio.h>


void n_alCSPSetBank(N_ALCSPlayer *csp, ALBank *bank) {
    N_ALEvent event;

    event.type = AL_SEQP_BANK_EVT;
    event.msg.spbank.bank = bank;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}
