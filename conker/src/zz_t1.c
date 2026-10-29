#include <ultra64.h>
#include "functions.h"
#include "variables.h"
extern u8 D_800DCE50[];
extern void (*D_8008B0D8[])(void *, s32);

typedef struct Node1515FDA0 {
    u8 pad0[0x8];
    struct Node1515FDA0 *unk8;
    u8 padC[0x2];
    u8 unkE;
    u8 padF;
    s8 unk10;
} Node1515FDA0;
void func_1515FDA0(s32 arg0) {
    u8 i;
    Node1515FDA0 *node;
    Node1515FDA0 **slot;
    s32 run;

    for (i = 0; i < 2; i++) {
        node = ((Node1515FDA0 *(*)[104])D_800DCE50)[i][52];
        D_800DD190++;
        if (node != NULL) {
            slot = &((Node1515FDA0 **)D_800DD198)[D_800DD190];
            do {
                *slot = node->unk8;
                if (node->unkE & 4) {
                    node->unkE &= ~4;
                    slot = &((Node1515FDA0 **)D_800DD198)[D_800DD190];
                } else if (node->unk10 != -1) {
                    run = 1;
                    if ((node->unkE & 2) && (D_800C35EA == 1)) {
                        run = 0;
                    }
                    if ((node->unkE & 8) && ((func_15181CC8(0) == 0) || (func_1517EF00(0) != 0))) {
                        run = 0;
                    }
                    if (run) {
                        D_8008B0D8[node->unk10](node, arg0);
                    }
                    slot = &((Node1515FDA0 **)D_800DD198)[D_800DD190];
                }
                node = *slot;
            } while (node != NULL);
        }
        D_800DD190--;
    }
}
