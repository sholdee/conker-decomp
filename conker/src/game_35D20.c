#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef void (*ResetFunc)(void);

/* One-entry reset callback tables for modes 1 and 2. */
static ResetFunc gEffectResetFuncsMode1[] = { func_150089D0 };
static ResetFunc gEffectResetFuncsMode2[] = { func_150089D0 };

/* Empties the object lists of both groups: mode 1 clears types 0-100, mode 2 types 101-103, any other mode all. */
void effect_clearObjectLists(s32 mode) {
    s32 first = 0;
    s32 last = OBJ_TYPE_COUNT;
    s32 list;
    s32 type;

    if (mode == 1) {
        last = 101;
    } else if (mode == 2) {
        first = 101;
    }
    for (list = 0; list < 2; list++) {
        for (type = first; type < last; type++) {
            gObjectLists[list][type] = NULL;
        }
    }
}

/* Runs the reset callbacks for mode 1 or 2, then invalidates the cached texture (D_800DD1B0 = -1 forces a reload). */
void effect_runResetCallbacks(s32 mode) {
    ResetFunc *funcs;
    s32 count;
    s32 i;

    if (mode == 1) {
        funcs = gEffectResetFuncsMode1;
        count = 1;
    } else if (mode == 2) {
        funcs = gEffectResetFuncsMode2;
        count = 1;
    } else {
        count = 0;
    }
    for (i = 0; i < count; i++) {
        funcs[i]();
    }
    D_800DD1B0 = -1;
}
