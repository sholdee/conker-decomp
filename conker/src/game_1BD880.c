#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Light effect helpers from game_18D770.c; their payload types (LightJitter, LightActorRef) live there. */
void func_15163DEC(struct225 *effect, void *jitter);
s32 func_15163F50(struct225 *effect, void *ref);

/*
 * Update for a flickering light tied to an actor: the payload holds the actor reference at 0x18,
 * followed by the jitter state at 0x20.
 */
void func_151903D0(struct225 *effect) {
    func_15163DEC(effect, &effect->unk20);
    func_15163F50(effect, &effect->unk18);
}
