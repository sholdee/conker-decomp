#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Releases every id in the count-prefixed list (list[0] ids follow), then frees the list. */
void texture_releaseList(s16 *list) {
    s16 *id = &list[1];
    s16 *end;
    s16 count = list[0];

    if (count > 0) {
        end = count + &list[1]; // matching: operand order sets IDO's addu register order
        do {
            texture_releaseId(*id);
            id++;
        } while (end != id);
    }
    func_10004074(list);
}

/* Drops one reference to id; when the last one goes, records id in the [gTexAgeFirst, gTexAgeLast] range and calls texture_setLifetime(id, 3). */
void texture_releaseId(s32 id) {
    if (D_800BC448[id] != 0 && gTexUseCounts[id] != 0 && --gTexUseCounts[id] == 0) {
        if (id < gTexAgeFirst) {
            gTexAgeFirst = id;
        }
        if (id > gTexAgeLast) {
            gTexAgeLast = id;
        }
        texture_setLifetime(id, 3);
    }
}
