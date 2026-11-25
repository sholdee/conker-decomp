#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr2
#include "functions.h"
#include "variables.h"


/*
 * Type 0x50 object: a point light that bobs above player D_800CC2D0[playerId].
 * The link header (type, list, prev, next) is shared with every object (see ObjLink in game_1944C0.c).
 */
typedef struct PlayerGlow {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct PlayerGlow *next;
    /* 0x0C */ u8 padC[0x2];
    /* 0x0E */ s16 timer;
    /* 0x10 */ u8 playerId;
    /* 0x11 */ u8 state;
    /* 0x12 */ u8 bobPhase;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ struct226 *light;
    /* 0x18 */ Mtx mtx[2];
    /* 0x98 */ f32 heightOffset;
} PlayerGlow;

#define OBJ_TYPE_PLAYER_GLOW 0x50

/* Object lists: D_800DCE50[list][type] is the head of a linked list of objects of that type. */
extern PlayerGlow *D_800DCE50[2][104];

extern void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
void func_1518C900(s32);
PlayerGlow *func_15155FD4(s32 playerId);

/* Allocates a glow object for player `playerId` in list 0. */
PlayerGlow *func_15155780(s32 playerId, s32 owner) {
    PlayerGlow *glow;

    glow = func_15167A68(OBJ_TYPE_PLAYER_GLOW, 0, sizeof(PlayerGlow), 1, owner, 1);
    if (glow == NULL) {
        return glow;
    }
    glow->playerId = playerId;
    glow->heightOffset = 0.0f;
    glow->state = 0;
    glow->light = NULL;
    func_1518C900(0xA6);
    return glow;
}

/* Finds or creates player `playerId`'s glow and shows it for `duration` frames, `heightOffset` higher. */
void func_151557FC(s32 playerId, s32 duration, f32 heightOffset) {
    PlayerGlow *glow;

    glow = func_15155FD4(playerId);
    if (glow == NULL) {
        glow = func_15155780(playerId, 0xFF);
    }
    if (glow != NULL) {
        glow->heightOffset = heightOffset;
        if (D_800CC2D0[playerId].in_water) {
            glow->timer = 0;
            glow->state = 0;
        } else {
            glow->state = 3;
            glow->timer = duration;
        }
    }
}

/* functions.h declares func_15048A40 (sine of a u8 angle) as returning void. */
#undef func_15048A40
extern f32 func_15048A40(u8 angle);
/* Creates a point light (struct226 is a partial view of game_18A8F0.c's PointLight); func_1515F10C frees it. */
struct226 *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, s32 group);
void func_1515F10C();

/*
 * Per-frame update. State 0 waits until the player's character allows a glow, then shows it for 120 frames
 * (state 1); state 2 waits for unk31C->unk75 to drop to 0; state 3 is a timed glow set by func_151557FC.
 * While shown, the light bobs 160 units above the player.
 */
void func_1515589C(PlayerGlow *glow) {
    s32 playerId;
    struct127 *player;
    s32 timer;
    s32 kind;
    struct126 *info;

    playerId = glow->playerId;
    player = &D_800CC2D0[glow->playerId];
    switch (glow->state) {
    case 0:
        info = D_800CC2D0[playerId].unk31C;
        kind = (info != NULL) ? info->unk75 : 0;
        if (!(kind & 0x80) && (kind != 0x2A) && (kind != 0x2B) && ((kind != 0x42) || !(D_800D2E68 & 4)) &&
            (kind != 0x2C) && (info->pad196[1] == 0) && (kind != 0) && (kind != 0x1F) && !player->in_water) {
            glow->state = 1;
            glow->timer = 120;
            func_10010F30(0x1EB, 0x7FFF, 0x40, 0, 0);
            return;
        }
        if (glow->light != NULL) {
            func_1515F10C(glow->light);
            glow->light = NULL;
        }
        break;
    case 1:
        timer = glow->timer;
        timer -= D_800BE9E4;
        if ((timer <= 0) || (D_800CC2D0[playerId].unk31C->unk75 == 0)) {
            glow->state = 2;
            timer = 0;
        }
        glow->timer = timer;
        glow->bobPhase += D_800BE9E4 * 2;
        if (glow->light != NULL) {
            f32 extraHeight;

            if (D_800CC2D0[playerId].unk31C->unk75 == 5) {
                extraHeight = 85.0f;
            } else {
                extraHeight = 0.0f;
            }
            glow->light->unkE = player->x_position;
            glow->light->unk10 = func_15048A40(glow->bobPhase) * 15.0f +
                                 (player->y_position + 160.0f + glow->heightOffset + extraHeight);
            glow->light->unk12 = player->z_position;
        } else {
            glow->light = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    case 2:
        if (D_800CC2D0[playerId].unk31C->unk75 == 0) {
            glow->state = 0;
        }
        if (glow->light != NULL) {
            func_1515F10C(glow->light);
            glow->light = NULL;
        }
        break;
    case 3:
        timer = glow->timer;
        timer -= D_800BE9E4;
        if (timer <= 0) {
            glow->state = 2;
            glow->timer = 0;
            break;
        }
        glow->timer = timer;
        glow->bobPhase += D_800BE9E4 * 2;
        if (glow->light != NULL) {
            glow->light->unkE = player->x_position;
            glow->light->unk10 = func_15048A40(glow->bobPhase) * 15.0f + (player->y_position + 160.0f);
            glow->light->unk12 = player->z_position;
        } else {
            glow->light = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    }
}

extern Gfx *func_15096934(Gfx *);
extern Gfx **D_800E03E0;

/* Draws the glow sprite (2.25x scale, rotated by D_800DBFF0[view].unk380) while the glow is shown. */
Gfx *func_15155CFC(Gfx *gfx, PlayerGlow *glow, s16 view) {
    s32 state;
    s32 playerId;
    f32 yaw;
    struct127 *player;
    f32 extraHeight;

    state = glow->state;
    playerId = glow->playerId;
    if ((state == 1) || (state == 3)) {
        player = &D_800CC2D0[playerId];
        if (player->unk31C->unk75 == 5) {
            extraHeight = 85.0f;
        } else {
            extraHeight = 0.0f;
        }
        yaw = D_800DBFF0[view].unk380;
        gfx = func_15096934(gfx);
        func_15043D90(&glow->mtx[D_800BE9C0], 0.0f, yaw, 0.0f, 2.25f, 2.25f, 2.25f, player->x_position,
                      func_15048A40(glow->bobPhase) * 15.0f +
                          (player->y_position + 160.0f + extraHeight + glow->heightOffset),
                      player->z_position);
        gSPMatrix(gfx++, &glow->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPClearGeometryMode(gfx++, G_ZBUFFER);
        gSPDisplayList(gfx++, *D_800E03E0);
        gSPSetGeometryMode(gfx++, G_ZBUFFER);
    }
    return gfx;
}

void func_1518CA04(s32 arg0);

/* Destroy handler: frees the light, then the object. */
void func_15155EF8(PlayerGlow *arg0) {
    PlayerGlow *glow = arg0; // matching: IDO keeps the object in a1 only through a copy of the parameter

    if (glow->light != NULL) {
        func_1515F10C(glow->light);
    }
    func_15169804((struct102 *)glow);
    func_1518CA04(0xA6);
}

/* Hides the current player's glow, or (while it is timed) stops it from bobbing. */
void func_15155F3C(void) {
    PlayerGlow *glow = func_15155FD4(D_800C3E78);

    if (glow != NULL) {
        if (glow->state == 2) {
            glow->state = 0;
        } else if (glow->state == 3) {
            glow->state = 2;
        }
    }
}

/* Switches the current player's timed glow (state 3) to state 1. */
void func_15155F90(void) {
    PlayerGlow *glow = func_15155FD4(D_800C3E78);

    if (glow != NULL) {
        if (glow->state == 3) {
            glow->state = 1;
        }
    }
}

/* Returns player `playerId`'s glow object from either object list, or NULL. */
PlayerGlow *func_15155FD4(s32 playerId) {
    s32 list;
    PlayerGlow *glow;

    for (list = 0; list < 2; list++) {
        for (glow = D_800DCE50[list][OBJ_TYPE_PLAYER_GLOW]; glow != NULL; glow = glow->next) {
            if (playerId == glow->playerId) {
                return glow;
            }
        }
    }
    return NULL;
}

/*
 * Event handler. `actors` is the event payload: event type 0 (actors[0] is gone) destroys the glow of that
 * player; type 0x2D (actors[0] and actors[1] swap) moves the glow to the other player.
 */
void func_15156028(PlayerGlow *glow, struct127 **actors, u8 eventType) {
    if (eventType == 0) {
        s32 player = actors[0] - D_800CC2D0;

        if ((player >= 0) && (player < 25) && (player == glow->playerId)) {
            func_1516972C((struct102 *)glow);
        }
    } else if (eventType == 0x2D) {
        s32 from = actors[0] - D_800CC2D0;
        s32 to = actors[1] - D_800CC2D0;

        if ((from >= 0) && (from < 25) && (to >= 0) && (to < 25)) {
            if (from == glow->playerId) {
                glow->playerId = to;
            } else if (to == glow->playerId) {
                glow->playerId = from;
            }
        }
    }
}
