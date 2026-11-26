#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Timed effect (expire callback 5, func_150C29F0) carrying a DiscEmitter at 0x28. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ DiscEmitter data;
} Effect1500EF20;

void func_1500EF20(void) {
    Effect1500EF20 *temp_v0;
    DiscEmitter tmp;

    func_15195AA8(D_800B0E00[0], D_800902B8, 1, -1, 0, 0, 0, -8);
    func_15195AA8(D_800B0E00[1], D_800902B8, 1, -1, 0, 1, 0, -8);

    tmp.x = D_80096220;
    tmp.z = D_80096224;
    tmp.y = D_80096228;
    tmp.radius = 400.0f;
    temp_v0 = func_151491F4((s16) ((func_150ADA20() % 0x33U) + 0x19), 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->data, &tmp, sizeof(tmp));
    }

    tmp.x = D_8009622C;
    tmp.z = D_80096230;
    tmp.y = D_80096234;
    temp_v0 = func_151491F4((s16) ((func_150ADA20() % 0x33U) + 0x19), 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->data, &tmp, sizeof(tmp));
    }

    tmp.z = D_80096238;
    temp_v0 = func_151491F4((s16) ((func_150ADA20() % 0x33U) + 0x19), 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->data, &tmp, sizeof(tmp));
    }

    tmp.x = D_8009623C;
    tmp.z = D_80096240;
    tmp.y = D_80096244;
    temp_v0 = func_151491F4((s16) ((func_150ADA20() % 0x33U) + 0x19), 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->data, &tmp, sizeof(tmp));
    }

    D_800DCD94 = D_800DCD20[0];
    D_800DCD98 = D_800DCD20[1];
    D_800DCD9C = D_800DCD20[2];
    D_800DCDA0 = 0.0f;
}
