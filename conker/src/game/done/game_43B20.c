#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15016670(void) {
    D_800D18C0 = 0;
}

void func_15016680(void) {
    D_800D212C = 0;
}

void func_15016690(s32 arg0) {
    s32 tmp;

    D_800D2120 = -1;
    D_800D2124 = -1;
    tmp = func_1502B7F0(&D_800D212C, 3, 12, arg0, 6) / 24U;
    D_800D2128 = tmp;
    if (tmp == 0) {
        D_800D212C = allocate_memory(24, 1, 0, 0);
        D_800D2128 = 1U;
        if (D_800D20FC) {
            D_800D212C->pos[0] = D_800D20FC->x;
            D_800D212C->pos[1] = D_800D20FC->y;
            D_800D212C->pos[2] = D_800D20FC->z;
            D_800D212C->cameraEye.x = D_800D20FC->x + 100;
            D_800D212C->cameraEye.y = D_800D20FC->y + 100;
            D_800D212C->cameraEye.z = D_800D20FC->z;
            D_800D212C->tilt = 0;
            D_800D212C->facing = D_800D20FC->angle;
            D_800D212C->doorWalkTime = 0;
        } else {
            D_800D212C->pos[0] = 0xE10;
            D_800D212C->pos[1] = -0xC80;
            D_800D212C->pos[2] = -0x898;
            D_800D212C->cameraEye.z = 1000.0f;
            D_800D212C->cameraEye.x = D_800D212C->cameraEye.y = D_800D212C->cameraEye.z;
            D_800D212C->tilt = 0;
            D_800D212C->facing = 0;
            D_800D212C->doorWalkTime = 0;
        }
    }
}
