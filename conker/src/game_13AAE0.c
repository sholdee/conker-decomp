#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Releases every id in the list, then frees the list. */
void func_1510D630(IdList *list) {
    s16 *id = list->ids;
    s16 *end;
    s16 count = list->count;

    if (count > 0) {
        end = count + list->ids; // matching: operand order sets IDO's addu register order
        do {
            func_1510D694(*id);
            id++;
        } while (end != id);
    }
    func_10004074(list);
}

/* Drops one reference to id; when the last one goes, records id in the [D_800D9F58, D_800D9F5C] range and calls func_1510D608(id, 3). */
void func_1510D694(s32 id) {
    if (D_800BC448[id] != 0 && D_800D9F68[id] != 0 && --D_800D9F68[id] == 0) {
        if (id < D_800D9F58) {
            D_800D9F58 = id;
        }
        if (id > D_800D9F5C) {
            D_800D9F5C = id;
        }
        func_1510D608(id, 3);
    }
}
