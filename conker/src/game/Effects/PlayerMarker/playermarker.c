#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define OBJ_TYPE_PLAYER_MARKER 0x50

/* The marker type's render teardown display list (object type table): reloads the identity modelview. */
Gfx gPlayerMarkerRenderTeardownDl[] = {
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

/* Allocates a marker object (object list 0) for player `playerId`. */
PlayerMarker *effect_newPlayerMarker(s32 playerId, s32 owner) {
    PlayerMarker *marker;

    marker = effect_newObject(OBJ_TYPE_PLAYER_MARKER, 0, sizeof(PlayerMarker), 1, owner, 1);
    if (marker == NULL) {
        return marker;
    }
    marker->playerId = playerId;
    marker->heightOffset = 0.0f;
    marker->state = 0;
    marker->light = NULL;
    attach_loadModelDl(0xA6);
    return marker;
}

/* Finds or creates player `playerId`'s marker and shows it for `duration` frames, `heightOffset` higher. */
void effect_showPlayerMarker(s32 playerId, s32 duration, f32 heightOffset) {
    PlayerMarker *marker;

    marker = effect_findPlayerMarker(playerId);
    if (marker == NULL) {
        marker = effect_newPlayerMarker(playerId, 0xFF);
    }
    if (marker != NULL) {
        marker->heightOffset = heightOffset;
        if (movement_obj[playerId].inwater) {
            marker->header.state = 0;
            marker->state = 0;
        } else {
            marker->state = 3;
            marker->header.state = duration;
        }
    }
}

/*
 * Per-frame update. State 0 waits until the player's mv->context_b allows a marker and the player is out of
 * water, then plays sound 0x1EB and shows it for 120 frames (state 1). State 2 (finished) waits for unk75 to
 * drop to 0 before re-arming. State 3 is a timed marker set by effect_showPlayerMarker. While shown, the light follows
 * the player and bobs; otherwise it is freed.
 */
void effect_updatePlayerMarker(PlayerMarker *marker) {
    s32 playerId;
    Actor *player;
    s32 timer;
    s32 kind;
    ActorCtl *info;

    playerId = marker->playerId;
    // matching: indexing with marker->playerId here and movement_obj[playerId] below (not player->) orders the loads
    player = &movement_obj[marker->playerId];
    switch (marker->state) {
    case 0:
        info = movement_obj[playerId].mv;
        kind = (info != NULL) ? info->context_b : 0;
        if (!(kind & 0x80) && (kind != 0x2A) && (kind != 0x2B) && ((kind != 0x42) || !(gSeenTextBits[8] & 4)) &&
            (kind != 0x2C) && (info->SniperState == 0) && (kind != 0) && (kind != 0x1F) && !player->inwater) {
            marker->state = 1;
            marker->header.state = 120;
            sound_play(0x1EB, 0x7FFF, 0x40, 0, 0);
        } else if (marker->light != NULL) {
            light_freePointLight(marker->light);
            marker->light = NULL;
        }
        break;
    case 1:
        timer = marker->header.state;
        timer -= D_800BE9E4; // matching: a single `timer = marker->header.state - D_800BE9E4;` schedules differently
        if ((timer <= 0) || (movement_obj[playerId].mv->context_b == 0)) {
            marker->state = 2;
            timer = 0;
        }
        marker->header.state = timer;
        marker->bobPhase += D_800BE9E4 * 2;
        if (marker->light != NULL) {
            f32 extraHeight;

            if (movement_obj[playerId].mv->context_b == 5) {
                extraHeight = 85.0f;
            } else {
                extraHeight = 0.0f;
            }
            marker->light->x = player->xpos;
            marker->light->y = math_sinBinAngle(marker->bobPhase) * 15.0f +
                                   (player->ypos + 160.0f + marker->heightOffset + extraHeight);
            marker->light->z = player->zpos;
        } else {
            marker->light = light_createPointLight(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    case 2:
        if (movement_obj[playerId].mv->context_b == 0) {
            marker->state = 0;
        }
        if (marker->light != NULL) {
            light_freePointLight(marker->light);
            marker->light = NULL;
        }
        break;
    case 3:
        timer = marker->header.state;
        timer -= D_800BE9E4;
        if (timer <= 0) {
            marker->state = 2;
            marker->header.state = 0;
            break;
        }
        marker->header.state = timer;
        marker->bobPhase += D_800BE9E4 * 2;
        if (marker->light != NULL) {
            marker->light->x = player->xpos;
            marker->light->y = math_sinBinAngle(marker->bobPhase) * 15.0f + (player->ypos + 160.0f);
            marker->light->z = player->zpos;
        } else {
            marker->light = light_createPointLight(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    }
}

/* Draws the marker sprite (2.25x scale, rotated by camera_global_Cameras[view].TrueCameraAngle) while the marker is shown. */
Gfx *effect_drawPlayerMarker(Gfx *gfx, PlayerMarker *marker, s16 view) {
    s32 state;
    s32 playerId;
    f32 yaw;
    Actor *player;
    f32 extraHeight;

    state = marker->state;
    playerId = marker->playerId;
    if ((state == 1) || (state == 3)) {
        player = &movement_obj[playerId];
        if (player->mv->context_b == 5) {
            extraHeight = 85.0f;
        } else {
            extraHeight = 0.0f;
        }
        yaw = camera_global_Cameras[view].TrueCameraAngle;
        gfx = sprite_endDraw(gfx);
        math_mtxRotScaleTrans(&marker->mtx[gDrawBufferIndex], 0.0f, yaw, 0.0f, 2.25f, 2.25f, 2.25f, player->xpos,
                      math_sinBinAngle(marker->bobPhase) * 15.0f +
                          (player->ypos + 160.0f + extraHeight + marker->heightOffset),
                      player->zpos);
        gSPMatrix(gfx++, &marker->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPClearGeometryMode(gfx++, G_ZBUFFER);
        gSPDisplayList(gfx++, gAttachModelHeaders[166]->dlists); /* attachment model 166, loaded by attach_loadModelDl */
        gSPSetGeometryMode(gfx++, G_ZBUFFER);
    }
    return gfx;
}


/* Destroy handler: frees the light, then the object. */
void effect_freePlayerMarker(PlayerMarker *arg0) {
    PlayerMarker *marker = arg0; // matching: IDO keeps the object in a1 only through a copy of the parameter

    if (marker->light != NULL) {
        light_freePointLight(marker->light);
    }
    effect_defaultDestroy(&marker->header);
    attach_releaseModel(0xA6);
}

/* For the current player: re-arms a finished marker (state 2 -> 0) or ends a timed one (state 3 -> 2). */
void effect_rearmPlayerMarker(void) {
    PlayerMarker *marker = effect_findPlayerMarker(dc_glbl_CharNum);

    if (marker != NULL) {
        if (marker->state == 2) {
            marker->state = 0;
        } else if (marker->state == 3) {
            marker->state = 2;
        }
    }
}

/* Turns the current player's timed marker (state 3) into a normal shown one (state 1). */
void effect_keepPlayerMarkerShown(void) {
    PlayerMarker *marker = effect_findPlayerMarker(dc_glbl_CharNum);

    if ((marker != NULL) && (marker->state == 3)) {
        marker->state = 1;
    }
}

/* Returns player `playerId`'s marker object from either object list, or NULL. */
PlayerMarker *effect_findPlayerMarker(s32 playerId) {
    s32 list;
    PlayerMarker *marker;

    for (list = 0; list < 2; list++) {
        for (marker = (PlayerMarker *)gObjectLists[list][OBJ_TYPE_PLAYER_MARKER]; marker != NULL; marker = (PlayerMarker *)marker->header.next) {
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
void effect_playerMarkerMessage(PlayerMarker *marker, Actor **actors, u8 eventType) {
    if (eventType == 0) {
        s32 player = actors[0] - movement_obj;

        if ((player >= 0) && (player < 25) && (player == marker->playerId)) {
            effect_destroyObject(&marker->header);
        }
    } else if (eventType == 0x2D) {
        s32 from = actors[0] - movement_obj;
        s32 to = actors[1] - movement_obj;

        if ((from >= 0) && (from < 25) && (to >= 0) && (to < 25)) {
            if (from == marker->playerId) {
                marker->playerId = to;
            } else if (to == marker->playerId) {
                marker->playerId = from;
            }
        }
    }
}
