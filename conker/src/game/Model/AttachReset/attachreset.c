#include <ultra64.h>

#include "conker/attach.h"
#include "conker/bone.h"
#include "conker/model.h"


void model_resetAttachments(void) {
    gAttachments = NULL;
}

void model_resetBoneChains(void) {
    gBoneChainResetMask = 0xF;
}
