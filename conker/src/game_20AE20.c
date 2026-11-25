#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Settings record (23 entries) with its defaults and valid [min, max] ranges. */
// matching: static, not extern; IDO then knows the copy loop's stores can't alias the source
static s8 D_800E0BE0[23];

/* Resets every setting to its default. */
void func_151DD970(void) {
    s32 i;

    for (i = 0; i < 23; i++) {
        D_800E0BE0[i] = D_8008FE54[i];
    }
}

/* Resets each setting that lies outside its valid range to its default. */
void func_151DD9E4(void) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if (D_800E0BE0[i] < D_8008FE6C[i] || D_800E0BE0[i] > D_8008FE84[i]) {
            D_800E0BE0[i] = D_8008FE54[i];
        }
    }
}

/* Returns the bitwise complement of value. */
s32 func_151DDB94(s32 value) {
    return ~value;
}
