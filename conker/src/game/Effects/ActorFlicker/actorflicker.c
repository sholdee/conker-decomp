#include <ultra64.h>
#include "conker/light.h"

/* Update for a flickering light tied to an actor (payload: actor reference, then jitter state). */
void light_updateActorFlicker(LightEffect *effect) {
    light_applyJitter(effect, &effect->data.actorJitter.jitter);
    light_trackActorRef(effect, &effect->data.actorJitter.ref);
}
