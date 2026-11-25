#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* User data of this effect kind: an object the effect owns. */
typedef struct EffectUserData1E5820 {
    /* 0x00 */ EffectHeader *child;
} EffectUserData1E5820;

/* Effect object (see EffectObj in game_174BF0.c); only the user data pointer is used here. */
struct Effect1E5820 {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ EffectUserData1E5820 *userData;
};

/* Destroys the object owned by this effect, if any. */
void func_151B8370(Effect1E5820 *effect) {
    EffectUserData1E5820 *userData = effect->userData;
    EffectHeader *child = userData->child;

    if (child != NULL) {
        func_1516972C(child);
    }
}

/* Effect kind handler (D_8008A2F0): destroys the owned object, then runs the generic handler. */
void func_151B83A0(Effect1E5820 *effect) {
    func_151B8370(effect);
    func_151478F4(effect);
}

/* Effect kind handler (D_8008A340): destroys the owned object, then runs the generic handler. */
void func_151B83CC(Effect1E5820 *effect) {
    func_151B8370(effect);
    func_15147928(effect);
}
