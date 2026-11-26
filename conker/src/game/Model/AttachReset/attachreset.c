#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void model_resetAttachments(void) {
    gAttachments = NULL;
}

void model_resetBoneChains(void) {
    gBoneChainResetMask = 0xF;
}
