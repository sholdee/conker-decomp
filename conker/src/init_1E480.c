#include <ultra64.h>

#include "n_libaudio.h"

void n_alSynDelete(void) {
    n_syn->head = NULL;
}
