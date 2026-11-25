#include <ultra64.h>
#include "functions.h"
#include "variables.h"
typedef struct Node150E35DC {
    u8 pad0[0x8];
    struct Node150E35DC *unk8;
    u8 padC[0x3C];
    u8 unk48;
} Node150E35DC;
void func_150E3514(void *);
void func_1515FDA0(s32 arg0) {
    s32 i;
    Node150E35DC *node;

    arg0--;
    for (i = 0; i != 2; i++) {
        D_800DD190++;
        node = (Node150E35DC *)D_800DCE50[i][39];
        while (node != NULL) {
            D_800DD198[D_800DD190] = node->unk8;
            if (arg0 == -1 || arg0 == node->unk48) {
                func_150E3514(node);
            }
            node = D_800DD198[D_800DD190];
        }
        {
            D_800DD190--;
        }
    }
}
