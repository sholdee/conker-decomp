#include <ultra64.h>
#include "functions.h"
#include "variables.h"
extern u8 D_800DCE50[];
void func_1515FDA0(void **q) {
    D_800DD190 += 1;
    if (q != NULL) {
        D_800DD198[D_800DD190] = q;
    }
}
