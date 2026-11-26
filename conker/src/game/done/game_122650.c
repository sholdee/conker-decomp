#include <ultra64.h>
#include "functions.h"
#include "variables.h"

u8 D_80088B40 = 0;
static u8 D_80088B44 = 0;


/* Zone handler: sets D_80088B40. */
void func_150F51A0(Actor *obj, TriggerZone *zone) {
    D_80088B40 = 1;
}

s32 func_150F51BC(void) {
    if (D_80088B40 != 0) {
        D_80088B40 = 0;
        return 1;
    } else {
        return 0;
    }
}

void func_150F51E8(s32 arg0) {
    if (((movement_obj[0].floorMaterial & 0X1F) == 3) || ((movement_obj[0].floorMaterial & 0x1F) == 4)) {
        if (D_800BE9E4 < D_80088B44){
            if(D_800C35EA[0] != 1) {
                D_80088B44 = (D_80088B44 - D_800BE9E4);
            }
        } else {
            D_80088B44 = ((func_150ADA20() & 0x3F) + 0xC0);
            sound_play(15, 18000, ((func_150ADA20() & 0x3F) + 0x20), 0, 0);
        }
    }
}
