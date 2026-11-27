#include <ultra64.h>

#include "conker/anim.h"
#include "conker/collision.h"
#include "conker/heap.h"
#include "conker/player.h"
#include "conker/rom.h"


void player_clearIdleRequests(void) {
    gIdleRequestFlags = 0;
}

void player_clearSpawnPoints(void) {
    gSpawnPoints = 0;
}

void player_loadSpawnPoints(s32 levelId) {
    s32 count;

    gTeam0SpawnMask = -1;
    gTeam1SpawnMask = -1;
    count = rom_loadPathTo(&gSpawnPoints, 3, 12, levelId, 6) / 24U;
    gSpawnPointCount = count;
    if (count == 0) {
        gSpawnPoints = allocate_memory(24, 1, 0, 0);
        gSpawnPointCount = 1U;
        if (gSpawnRecords) {
            gSpawnPoints->pos[0] = gSpawnRecords->x;
            gSpawnPoints->pos[1] = gSpawnRecords->y;
            gSpawnPoints->pos[2] = gSpawnRecords->z;
            gSpawnPoints->cameraEye.x = gSpawnRecords->x + 100;
            gSpawnPoints->cameraEye.y = gSpawnRecords->y + 100;
            gSpawnPoints->cameraEye.z = gSpawnRecords->z;
            gSpawnPoints->tilt = 0;
            gSpawnPoints->facing = gSpawnRecords->angle;
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
