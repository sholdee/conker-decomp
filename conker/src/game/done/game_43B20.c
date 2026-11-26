#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15016670(void) {
    D_800D18C0 = 0;
}

void player_clearSpawnPoints(void) {
    gSpawnPoints = 0;
}

void player_loadSpawnPoints(s32 arg0) {
    s32 tmp;

    gTeam0SpawnMask = -1;
    gTeam1SpawnMask = -1;
    tmp = func_1502B7F0(&gSpawnPoints, 3, 12, arg0, 6) / 24U;
    gSpawnPointCount = tmp;
    if (tmp == 0) {
        gSpawnPoints = allocate_memory(24, 1, 0, 0);
        gSpawnPointCount = 1U;
        if (D_800D20FC) {
            gSpawnPoints->pos[0] = D_800D20FC->x;
            gSpawnPoints->pos[1] = D_800D20FC->y;
            gSpawnPoints->pos[2] = D_800D20FC->z;
            gSpawnPoints->cameraEye.x = D_800D20FC->x + 100;
            gSpawnPoints->cameraEye.y = D_800D20FC->y + 100;
            gSpawnPoints->cameraEye.z = D_800D20FC->z;
            gSpawnPoints->tilt = 0;
            gSpawnPoints->facing = D_800D20FC->angle;
            gSpawnPoints->doorWalkTime = 0;
        } else {
            gSpawnPoints->pos[0] = 0xE10;
            gSpawnPoints->pos[1] = -0xC80;
            gSpawnPoints->pos[2] = -0x898;
            gSpawnPoints->cameraEye.z = 1000.0f;
            gSpawnPoints->cameraEye.x = gSpawnPoints->cameraEye.y = gSpawnPoints->cameraEye.z;
            gSpawnPoints->tilt = 0;
            gSpawnPoints->facing = 0;
            gSpawnPoints->doorWalkTime = 0;
        }
    }
}
