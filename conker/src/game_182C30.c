#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Type 0x50 object: a marker over player D_800CC2D0[playerId], made of a white point light and a sprite
 * (*D_800E03E0) that bob 160 units above the player.
 * The link header (type, list, prev, next) is shared with every object (see ObjLink in game_1944C0.c).
 */
typedef struct PlayerMarker {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct PlayerMarker *next;
    /* 0x0C */ u8 padC[0x2];
    /* 0x0E */ s16 timer;
    /* 0x10 */ u8 playerId;
    /* 0x11 */ u8 state;
    /* 0x12 */ u8 bobPhase;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ struct226 *light;
    /* 0x18 */ Mtx mtx[2];
    /* 0x98 */ f32 heightOffset;
} PlayerMarker;

#define OBJ_TYPE_PLAYER_MARKER 0x50

/* Object lists: D_800DCE50[list][type] is the head of a linked list of objects of that type. */
extern PlayerMarker *D_800DCE50[2][104];

extern void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
void func_1518C900(s32);
PlayerMarker *func_15155FD4(s32 playerId);

/* Allocates a marker object (object list 0) for player `playerId`. */
PlayerMarker *func_15155780(s32 playerId, s32 owner) {
    PlayerMarker *marker;

    marker = func_15167A68(OBJ_TYPE_PLAYER_MARKER, 0, sizeof(PlayerMarker), 1, owner, 1);
    if (marker == NULL) {
        return marker;
    }
    marker->playerId = playerId;
    marker->heightOffset = 0.0f;
    marker->state = 0;
    marker->light = NULL;
    func_1518C900(0xA6);
    return marker;
}

/* Finds or creates player `playerId`'s marker and shows it for `duration` frames, `heightOffset` higher. */
void func_151557FC(s32 playerId, s32 duration, f32 heightOffset) {
    PlayerMarker *marker;

    marker = func_15155FD4(playerId);
    if (marker == NULL) {
        marker = func_15155780(playerId, 0xFF);
    }
    if (marker != NULL) {
        marker->heightOffset = heightOffset;
        if (D_800CC2D0[playerId].in_water) {
            marker->timer = 0;
            marker->state = 0;
        } else {
            marker->state = 3;
            marker->timer = duration;
        }
    }
}

/* Creates a point light (struct226 is a partial view of PointLight in structs.h); func_1515F10C frees it. */
struct226 *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, s32 group);
void func_1515F10C();

/*
 * Per-frame update. State 0 waits until the player's unk31C->unk75 allows a marker and the player is out of
 * water, then plays sound 0x1EB and shows it for 120 frames (state 1). State 2 (finished) waits for unk75 to
 * drop to 0 before re-arming. State 3 is a timed marker set by func_151557FC. While shown, the light follows
 * the player and bobs; otherwise it is freed.
 */
void func_1515589C(PlayerMarker *marker) {
    s32 playerId;
    Actor *player;
    s32 timer;
    s32 kind;
    ActorCtl *info;

    playerId = marker->playerId;
    // matching: indexing with marker->playerId here and D_800CC2D0[playerId] below (not player->) orders the loads
    player = &D_800CC2D0[marker->playerId];
    switch (marker->state) {
    case 0:
        info = D_800CC2D0[playerId].unk31C;
        kind = (info != NULL) ? info->unk75 : 0;
        if (!(kind & 0x80) && (kind != 0x2A) && (kind != 0x2B) && ((kind != 0x42) || !(D_800D2E68 & 4)) &&
            (kind != 0x2C) && (info->aimState == 0) && (kind != 0) && (kind != 0x1F) && !player->in_water) {
            marker->state = 1;
            marker->timer = 120;
            func_10010F30(0x1EB, 0x7FFF, 0x40, 0, 0);
        } else if (marker->light != NULL) {
            func_1515F10C(marker->light);
            marker->light = NULL;
        }
        break;
    case 1:
        timer = marker->timer;
        timer -= D_800BE9E4; // matching: a single `timer = marker->timer - D_800BE9E4;` schedules differently
        if ((timer <= 0) || (D_800CC2D0[playerId].unk31C->unk75 == 0)) {
            marker->state = 2;
            timer = 0;
        }
        marker->timer = timer;
        marker->bobPhase += D_800BE9E4 * 2;
        if (marker->light != NULL) {
            f32 extraHeight;

            if (D_800CC2D0[playerId].unk31C->unk75 == 5) {
                extraHeight = 85.0f;
            } else {
                extraHeight = 0.0f;
            }
            marker->light->unkE = player->x_position;
            marker->light->unk10 = func_15048A40(marker->bobPhase) * 15.0f +
                                   (player->y_position + 160.0f + marker->heightOffset + extraHeight);
            marker->light->unk12 = player->z_position;
        } else {
            marker->light = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    case 2:
        if (D_800CC2D0[playerId].unk31C->unk75 == 0) {
            marker->state = 0;
        }
        if (marker->light != NULL) {
            func_1515F10C(marker->light);
            marker->light = NULL;
        }
        break;
    case 3:
        timer = marker->timer;
        timer -= D_800BE9E4;
        if (timer <= 0) {
            marker->state = 2;
            marker->timer = 0;
            break;
        }
        marker->timer = timer;
        marker->bobPhase += D_800BE9E4 * 2;
        if (marker->light != NULL) {
            marker->light->unkE = player->x_position;
            marker->light->unk10 = func_15048A40(marker->bobPhase) * 15.0f + (player->y_position + 160.0f);
            marker->light->unk12 = player->z_position;
        } else {
            marker->light = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    }
}

extern Gfx *func_15096934(Gfx *);
extern Gfx **D_800E03E0;

/* Draws the marker sprite (2.25x scale, rotated by D_800DBFF0[view].unk380) while the marker is shown. */
Gfx *func_15155CFC(Gfx *gfx, PlayerMarker *marker, s16 view) {
    s32 state;
    s32 playerId;
    f32 yaw;
    Actor *player;
    f32 extraHeight;

    state = marker->state;
    playerId = marker->playerId;
    if ((state == 1) || (state == 3)) {
        player = &D_800CC2D0[playerId];
        if (player->unk31C->unk75 == 5) {
            extraHeight = 85.0f;
        } else {
            extraHeight = 0.0f;
        }
        yaw = D_800DBFF0[view].unk380;
        gfx = func_15096934(gfx);
        func_15043D90(&marker->mtx[D_800BE9C0], 0.0f, yaw, 0.0f, 2.25f, 2.25f, 2.25f, player->x_position,
                      func_15048A40(marker->bobPhase) * 15.0f +
                          (player->y_position + 160.0f + extraHeight + marker->heightOffset),
                      player->z_position);
        gSPMatrix(gfx++, &marker->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPClearGeometryMode(gfx++, G_ZBUFFER);
        gSPDisplayList(gfx++, *D_800E03E0);
        gSPSetGeometryMode(gfx++, G_ZBUFFER);
    }
    return gfx;
}

void func_1518CA04(s32 arg0);

/* Destroy handler: frees the light, then the object. */
void func_15155EF8(PlayerMarker *arg0) {
    PlayerMarker *marker = arg0; // matching: IDO keeps the object in a1 only through a copy of the parameter

    if (marker->light != NULL) {
        func_1515F10C(marker->light);
    }
    func_15169804((struct102 *)marker);
    func_1518CA04(0xA6);
}

/* For the current player: re-arms a finished marker (state 2 -> 0) or ends a timed one (state 3 -> 2). */
void func_15155F3C(void) {
    PlayerMarker *marker = func_15155FD4(D_800C3E78);

    if (marker != NULL) {
        if (marker->state == 2) {
            marker->state = 0;
        } else if (marker->state == 3) {
            marker->state = 2;
        }
    }
}

/* Turns the current player's timed marker (state 3) into a normal shown one (state 1). */
void func_15155F90(void) {
    PlayerMarker *marker = func_15155FD4(D_800C3E78);

    if ((marker != NULL) && (marker->state == 3)) {
        marker->state = 1;
    }
}

/* Returns player `playerId`'s marker object from either object list, or NULL. */
PlayerMarker *func_15155FD4(s32 playerId) {
    s32 list;
    PlayerMarker *marker;

    for (list = 0; list < 2; list++) {
        for (marker = D_800DCE50[list][OBJ_TYPE_PLAYER_MARKER]; marker != NULL; marker = marker->next) {
            if (playerId == marker->playerId) {
                return marker;
            }
        }
    }
    return NULL;
}

/*
 * Event handler. `actors` is the event payload: event type 0 (actors[0] is gone) destroys the marker of that
 * player; type 0x2D (actors[0] and actors[1] swap) moves the marker to the other player.
 */
void func_15156028(PlayerMarker *marker, Actor **actors, u8 eventType) {
    if (eventType == 0) {
        s32 player = actors[0] - D_800CC2D0;

        if ((player >= 0) && (player < 25) && (player == marker->playerId)) {
            func_1516972C((struct102 *)marker);
        }
    } else if (eventType == 0x2D) {
        s32 from = actors[0] - D_800CC2D0;
        s32 to = actors[1] - D_800CC2D0;

        if ((from >= 0) && (from < 25) && (to >= 0) && (to < 25)) {
            if (from == marker->playerId) {
                marker->playerId = to;
            } else if (to == marker->playerId) {
                marker->playerId = from;
            }
        }
    }
}
