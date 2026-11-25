#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Update for a flickering light tied to an actor (payload: actor reference, then jitter state). */
void func_151903D0(LightEffect *effect) {
    func_15163DEC(effect, &effect->data.actorJitter.jitter);
    func_15163F50(effect, &effect->data.actorJitter.ref);
}
