#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name (see init/Sched/SchedInit/schedinit.c). The multiplayer spawn points of the level: */
s32 gTeam0SpawnMask;          // team 0 spawn mask (bit per spawn point)
s32 gTeam1SpawnMask;          // team 1 spawn mask
s32 gSpawnPointCount;          // number of entries in gSpawnPoints
SpawnPoint *gSpawnPoints;  // the level's spawn point table

#include "functions.h"
#include "variables.h"

/* The 7 model-id rows of gCharacterTypeGroups (a pointer table in .data); the first id of a row stands for the row. */
const u8 D_8009D920[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x3B, 0x75, 0x80, 0x82, 0x88, 0x90,
                          0x96, 0x98, 0x9C, 0x9D, 0x9F, 0xA0, 0xB0, 0xB1, 0xB2, 0xB4 };
const u8 D_8009D938[] = { 0x5A, 0x5F, 0x74, 0x7A, 0x8D };
const u8 D_8009D940[] = { 0x3A, 0x3D };
const u8 D_8009D944[] = { 0x50, 0x9A, 0x9B };
const u8 D_8009D948[] = { 0x10, 0x38 };
const u8 D_8009D94C[] = { 0x53, 0xA5 };
const u8 D_8009D950[] = { 0x05, 0xAD, 0xAE, 0xAF };
/* Row lengths (12 bytes in the ROM: the unit's literals start at the next 16-byte line). */
const u8 gCharacterTypeGroupLengths[12] = { 21, 5, 2, 3, 2, 2, 4 };

/* This file's .data: the rows above, one pointer each. */
static const u8 *gCharacterTypeGroups[7] = {
    D_8009D920, D_8009D938, D_8009D940, D_8009D944, D_8009D948, D_8009D94C, D_8009D950,
};

/* Looks obj->id up in the 7 rows of gCharacterTypeGroups (row lengths in gCharacterTypeGroupLengths) and returns
 * the first entry of the row that contains it, or the id itself when no row does.
 * Same search as model_getAnimGroupLeader (game/Model/ModelLoad/modelload). */
s32 player_getBaseCharacterType(Actor *obj) {
    s32 id = obj->character_type;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < gCharacterTypeGroupLengths[i]; j++) {
            if (id == gCharacterTypeGroups[i][j]) {
                return gCharacterTypeGroups[i][0];
            }
        }
    }
    return id;
}

/* Copies spawn point `spawnIndex` (or, when it is -1, the player's stored one) into the out
 * parameters. In multiplayer (gIsMultiplayer) with D_800D18A8 set, it instead picks the team's spawn
 * point farthest from the other team's players, plus a random term. When `useLink` is set, the
 * point's link is followed. Remembers the chosen index in the player's state; returns 1 when
 * there is no spawn table, else 0. */
s32 player_getSpawnPoint(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, Vec3f *outCameraEye, u8 *outTilt,
                  u8 *outFacing, s32 *outUnk14, s32 *outDoorWalkTime, s32 unused, s32 *outLinkFlag) {
    s32 remember;
    ActorCtl *state;
    s32 mask; // matching: also the player loop counter and link index; separate locals change regalloc
    s32 team;
    s32 best;
    s32 i;
    f32 nearestDistSq;
    f32 bestDistSq;
    f32 dx;
    f32 dz;
    s32 count;
    s32 lastSpawn;
    s32 j; // spawn loop counter, then the chosen spawn index

    remember = TRUE;
    if (level == 0x2B) {
        remember = FALSE;
        D_800D18A8 = 0;
    }
    if (gIsMultiplayer && gMenuState->selection == 0 && gSettings[3] != 0) {
        remember = FALSE;
    }
    if (playerIndex != 0xFF) {
        if (spawnIndex == -1) {
            spawnIndex = movement_obj[playerIndex].mv->spawnIndex;
        }
        if (remember) {
            movement_obj[playerIndex].mv->spawnIndex = 0;
        }
    }
    spawnIndex &= 0xFFF;
    if (spawnIndex >= gSpawnPointCount || spawnIndex < 0) {
        spawnIndex = 0;
    }
    if (gSpawnPoints != NULL) {
        if (gIsMultiplayer && D_800D18A8) {
            u8 candidates[32];

            team = movement_obj[playerIndex].team;
            state = movement_obj[playerIndex].mv;
            count = 0;
            best = 0;
            if (team == 0) {
                mask = gTeam0SpawnMask;
            } else {
                mask = gTeam1SpawnMask;
            }
            for (j = 0; j < gSpawnPointCount; j++) {
                if ((1 << j) & mask) {
                    candidates[count] = j;
                    count++;
                }
            }
            bestDistSq = 0.0f;
            lastSpawn = state->lastSpawnIndex;
            for (i = 0; i < count; i++) {
                if (lastSpawn != candidates[i]) {
                    nearestDistSq = 100000000.0f;
                    for (mask = 0; mask < D_8008FD8C; mask++) {
                        if (team != movement_obj[mask].team) {
                            dx = movement_obj[mask].xpos - gSpawnPoints[candidates[i]].pos[0];
                            dz = movement_obj[mask].zpos - gSpawnPoints[candidates[i]].pos[2];
                            dx = dx * dx + dz * dz;
                            if (dx < nearestDistSq) {
                                nearestDistSq = dx;
                            }
                        }
                    }
                    dz = math_randomFloat() * 500.0f;
                    nearestDistSq += dz * dz;
                    if (bestDistSq < nearestDistSq) {
                        best = i;
                        bestDistSq = nearestDistSq;
                    }
                }
            }
            if (gMenuState->selection != 0 || gSettings[3] == 0) {
                spawnIndex = candidates[best];
            }
            state = movement_obj[playerIndex].mv;
            state->lastSpawnIndex = spawnIndex;
            useLink = FALSE;
        }
        j = spawnIndex;
        if (useLink && (mask = gSpawnPoints[spawnIndex].link) != 0) {
            j = mask;
            if (mask == -1 || mask >= gSpawnPointCount) {
                j = spawnIndex;
            }
            if (outLinkFlag != NULL) {
                mask = gSpawnPoints[j].doorWalkTime;
                if (mask >= 0) {
                    *outLinkFlag = 0;
                } else if (mask == -1) {
                    *outLinkFlag = 0;
                } else {
                    *outLinkFlag = 1;
                }
            }
        } else if (useLink && gSpawnPoints[spawnIndex].link == 0) {
            D_800DDE38 = 2;
        }
        outCameraEye->x = gSpawnPoints[j].cameraEye.x;
        outCameraEye->y = gSpawnPoints[j].cameraEye.y;
        outCameraEye->z = gSpawnPoints[j].cameraEye.z;
        outPos[0] = gSpawnPoints[j].pos[0];
        outPos[1] = gSpawnPoints[j].pos[1];
        outPos[2] = gSpawnPoints[j].pos[2];
        *outTilt = gSpawnPoints[j].tilt;
        *outFacing = gSpawnPoints[j].facing;
        *outUnk14 = gSpawnPoints[j].unk14;
        *outDoorWalkTime = gSpawnPoints[j].doorWalkTime;
        if (gIsMultiplayer && D_800D18A8 == 0) {
            f32 angle = *outFacing * 3.1415927410f * (1.0f / 128);
            f32 offset = math_randomFloat() * 80.0f - 40.0f;
            outPos[0] += sinf(angle) * offset;
            outPos[2] += cosf(angle) * offset;
        }
        if (playerIndex != 0xFF && remember) {
            movement_obj[playerIndex].mv->spawnIndex = spawnIndex;
        }
        return 0;
    }
    return 1;
}

/* Sets the spawn index player_getSpawnPoint falls back to for this player object. */
void player_setSpawnIndex(Actor *obj, s32 spawnIndex) {
    obj->mv->spawnIndex = spawnIndex;
}

/* Returns the player object's stored spawn index. */
u8 player_getSpawnIndex(Actor *obj) {
    return obj->mv->spawnIndex;
}

/* Moves obj to spawn point `spawnIndex` (see player_getSpawnPoint): sets its position and angles,
 * clears its motion, places its camera and passes {obj, unique_id} to effect_broadcastEvent. */
void player_moveToSpawn(Actor *obj, s32 spawnIndex, s32 unused) {
    s16 pos[3];
    Vec3f cameraEye;
    u8 tilt;
    u8 facing;
    s32 doorWalkTime;
    s32 unk14;
    Actor *savedObject;
    s32 savedIndex;
    ActorRef event;

    player_getSpawnPoint(obj->playernumber, spawnIndex, D_800D18A8, pos, &cameraEye, &tilt, &facing, &unk14, &doorWalkTime, unused, NULL);
    savedObject = gCurrentActor;
    savedIndex = dc_glbl_CharNum;
    gCurrentActor = obj;
    dc_glbl_CharNum = obj - movement_obj;
    event_runTimedEvents(obj, 800.0f, 1);
    gCurrentActor = savedObject;
    dc_glbl_CharNum = savedIndex;
    obj->xpos = pos[0];
    obj->ypos = pos[1];
    obj->zpos = pos[2];
    obj->old_xpos = obj->xpos;
    obj->old_ypos = *(volatile f32 *)&obj->ypos; // matching: stops IDO carrying ypos across the loop below
    obj->old_zpos = obj->zpos;
    obj->angle = obj->fangle = obj->langle = facing << 8;
    obj->theta = (s16)(obj->langle + 0x4000) * (360.0f / 65536);
    obj->vtilt = (tilt * 360) >> 8;
    while (obj->vtilt > 180.0f) {
        obj->vtilt -= 360.0f;
    }
    obj->shad = 0.0f;
    obj->floorY = obj->ypos;
    if (obj->mv != NULL) {
        actor_computeSize(obj, &obj->mv->height, &obj->mv->width, &obj->mv->depth);
    }
    obj->unk1CC = obj->ypos;
    obj->unk2F8 &= ~0x10;
    obj->unk22C |= 0x200;
    obj->ob_slopex = 0.0f;
    obj->ob_slopey = 0.0f;
    obj->ob_overslopex = 0.0f;
    obj->ob_overslopey = 0.0f;
    if (obj->playernumber != 0xFF) {
        obj->mv->door_walkf_time = doorWalkTime;
        if (gIsMultiplayer && D_800D18A8) {
            actor_resolveCollisions(obj, obj - movement_obj);
        }
        if (obj->mv->rt_context_b == 0 && obj->mv->GrenadeThrowState != 4 && obj->mv->GrenadeThrowState != 7) {
            obj->count5 = 0;
        }
    } else {
        obj->count5 = 0;
    }
    if (obj->CameraPtr != NULL) {
        camera_placeAt(obj->CameraPtr, obj, &cameraEye);
    }
    event.actor = obj;
    event.uniqueId = obj->unique_id;
    effect_broadcastEvent((s32)&event, 4);
}
