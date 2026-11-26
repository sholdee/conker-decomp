#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_1944C0.c. Not in effect.h: game_16F770.c calls it with two more arguments. */
void func_15169040(s32 event, u8 eventType);

/* Defined in game_981E0.c. Not in event.h: game_83300.c calls it with a fourth argument. */
f32 func_1506AD30(Actor *obj, f32 time, u8 filtered);

/* The 7 model-id rows of D_80087240 (a pointer table in .data); the first id of a row stands for the row. */
const u8 D_8009D920[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x3B, 0x75, 0x80, 0x82, 0x88, 0x90,
                          0x96, 0x98, 0x9C, 0x9D, 0x9F, 0xA0, 0xB0, 0xB1, 0xB2, 0xB4 };
const u8 D_8009D938[] = { 0x5A, 0x5F, 0x74, 0x7A, 0x8D };
const u8 D_8009D940[] = { 0x3A, 0x3D };
const u8 D_8009D944[] = { 0x50, 0x9A, 0x9B };
const u8 D_8009D948[] = { 0x10, 0x38 };
const u8 D_8009D94C[] = { 0x53, 0xA5 };
const u8 D_8009D950[] = { 0x05, 0xAD, 0xAE, 0xAF };
/* Row lengths (12 bytes in the ROM: the unit's literals start at the next 16-byte line). */
const u8 D_8009D954[12] = { 21, 5, 2, 3, 2, 2, 4 };

extern u8 *D_80087240[];

/* Looks obj->id up in the 7 rows of D_80087240 (row lengths in D_8009D954) and returns
 * the first entry of the row that contains it, or the id itself when no row does.
 * Same search as func_1503D5F0 (game_6A3D0). */
s32 func_15084D00(Actor *obj) {
    s32 id = obj->character_type;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < D_8009D954[i]; j++) {
            if (id == D_80087240[i][j]) {
                return D_80087240[i][0];
            }
        }
    }
    return id;
}

/* Copies spawn point `spawnIndex` (or, when it is -1, the player's stored one) into the out
 * parameters. In multiplayer (D_800BE616) with D_800D18A8 set, it instead picks the team's spawn
 * point farthest from the other team's players, plus a random term. When `useLink` is set, the
 * point's link is followed. Remembers the chosen index in the player's state; returns 1 when
 * there is no spawn table, else 0. */
s32 func_15084D70(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, Vec3f *outUnk8, u8 *outUnk6,
                  u8 *outFacing, s32 *outUnk14, s32 *outUnk15, s32 unused, s32 *outLinkFlag) {
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
    if (D_800BE616 && D_8008FDD4->selection == 0 && D_800E0BE0[3] != 0) {
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
    if (spawnIndex >= D_800D2128 || spawnIndex < 0) {
        spawnIndex = 0;
    }
    if (D_800D212C != NULL) {
        if (D_800BE616 && D_800D18A8) {
            u8 candidates[32];

            team = movement_obj[playerIndex].team;
            state = movement_obj[playerIndex].mv;
            count = 0;
            best = 0;
            if (team == 0) {
                mask = D_800D2120;
            } else {
                mask = D_800D2124;
            }
            for (j = 0; j < D_800D2128; j++) {
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
                            dx = movement_obj[mask].xpos - D_800D212C[candidates[i]].pos[0];
                            dz = movement_obj[mask].zpos - D_800D212C[candidates[i]].pos[2];
                            dx = dx * dx + dz * dz;
                            if (dx < nearestDistSq) {
                                nearestDistSq = dx;
                            }
                        }
                    }
                    dz = func_150ADA68() * 500.0f;
                    nearestDistSq += dz * dz;
                    if (bestDistSq < nearestDistSq) {
                        best = i;
                        bestDistSq = nearestDistSq;
                    }
                }
            }
            if (D_8008FDD4->selection != 0 || D_800E0BE0[3] == 0) {
                spawnIndex = candidates[best];
            }
            state = movement_obj[playerIndex].mv;
            state->lastSpawnIndex = spawnIndex;
            useLink = FALSE;
        }
        j = spawnIndex;
        if (useLink && (mask = D_800D212C[spawnIndex].link) != 0) {
            j = mask;
            if (mask == -1 || mask >= D_800D2128) {
                j = spawnIndex;
            }
            if (outLinkFlag != NULL) {
                mask = D_800D212C[j].unk15;
                if (mask >= 0) {
                    *outLinkFlag = 0;
                } else if (mask == -1) {
                    *outLinkFlag = 0;
                } else {
                    *outLinkFlag = 1;
                }
            }
        } else if (useLink && D_800D212C[spawnIndex].link == 0) {
            D_800DDE38 = 2;
        }
        outUnk8->x = D_800D212C[j].unk8.x;
        outUnk8->y = D_800D212C[j].unk8.y;
        outUnk8->z = D_800D212C[j].unk8.z;
        outPos[0] = D_800D212C[j].pos[0];
        outPos[1] = D_800D212C[j].pos[1];
        outPos[2] = D_800D212C[j].pos[2];
        *outUnk6 = D_800D212C[j].unk6;
        *outFacing = D_800D212C[j].facing;
        *outUnk14 = D_800D212C[j].unk14;
        *outUnk15 = D_800D212C[j].unk15;
        if (D_800BE616 && D_800D18A8 == 0) {
            f32 angle = *outFacing * 3.1415927410f * (1.0f / 128);
            f32 offset = func_150ADA68() * 80.0f - 40.0f;
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

/* Sets the spawn index func_15084D70 falls back to for this player object. */
void func_15085410(Actor *obj, s32 spawnIndex) {
    obj->mv->spawnIndex = spawnIndex;
}

/* Returns the player object's stored spawn index. */
u8 func_15085420(Actor *obj) {
    return obj->mv->spawnIndex;
}

/* Moves obj to spawn point `spawnIndex` (see func_15084D70): sets its position and angles,
 * clears its motion, places its camera and passes {obj, unique_id} to func_15169040. */
void func_15085430(Actor *obj, s32 spawnIndex, s32 unused) {
    s16 pos[3];
    Vec3f unk8;
    u8 unk6;
    u8 facing;
    s32 unk15;
    s32 unk14;
    Actor *savedObject;
    s32 savedIndex;
    ActorRef event;

    func_15084D70(obj->playernumber, spawnIndex, D_800D18A8, pos, &unk8, &unk6, &facing, &unk14, &unk15, unused, NULL);
    savedObject = D_800D154C;
    savedIndex = dc_glbl_CharNum;
    D_800D154C = obj;
    dc_glbl_CharNum = obj - movement_obj;
    func_1506AD30(obj, 800.0f, 1);
    D_800D154C = savedObject;
    dc_glbl_CharNum = savedIndex;
    obj->xpos = pos[0];
    obj->ypos = pos[1];
    obj->zpos = pos[2];
    obj->old_xpos = obj->xpos;
    obj->old_ypos = *(volatile f32 *)&obj->ypos; // matching: stops IDO carrying ypos across the loop below
    obj->old_zpos = obj->zpos;
    obj->angle = obj->fangle = obj->langle = facing << 8;
    obj->theta = (s16)(obj->langle + 0x4000) * (360.0f / 65536);
    obj->vtilt = (unk6 * 360) >> 8;
    while (obj->vtilt > 180.0f) {
        obj->vtilt -= 360.0f;
    }
    obj->shad = 0.0f;
    obj->floorY = obj->ypos;
    if (obj->mv != NULL) {
        func_1507C3E0(obj, &obj->mv->height, &obj->mv->width, &obj->mv->depth);
    }
    obj->unk1CC = obj->ypos;
    obj->unk2F8 &= ~0x10;
    obj->unk22C |= 0x200;
    obj->ob_slopex = 0.0f;
    obj->ob_slopey = 0.0f;
    obj->ob_overslopex = 0.0f;
    obj->ob_overslopey = 0.0f;
    if (obj->playernumber != 0xFF) {
        obj->mv->door_walkf_time = unk15;
        if (D_800BE616 && D_800D18A8) {
            func_1505D6F0(obj, obj - movement_obj);
        }
        if (obj->mv->rt_context_b == 0 && obj->mv->GrenadeThrowState != 4 && obj->mv->GrenadeThrowState != 7) {
            obj->count5 = 0;
        }
    } else {
        obj->count5 = 0;
    }
    if (obj->CameraPtr != NULL) {
        func_15005818(obj->CameraPtr, obj, &unk8);
    }
    event.actor = obj;
    event.uniqueId = obj->unique_id;
    func_15169040(&event, 4);
}
