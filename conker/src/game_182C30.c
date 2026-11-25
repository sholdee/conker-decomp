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

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13;
    struct226 *unk14;
    u8 pad18[0x80];
    f32 unk98;
} Obj1515589C;

#undef func_15048A40
extern f32 func_15048A40(u8 arg0);
struct226 *func_1515D5F8(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_1515F10C();

void func_1515589C(Obj1515589C *arg0) {
    s32 idx;
    struct127 *obj;
    s32 t;
    s32 v;
    struct126 *p;

    idx = arg0->unk10;
    obj = &D_800CC2D0[arg0->unk10];
    switch (arg0->unk11) {
    case 0:
        p = D_800CC2D0[idx].unk31C;
        v = (p != NULL) ? p->unk75 : 0;
        if (!(v & 0x80) && (v != 0x2A) && (v != 0x2B) && ((v != 0x42) || !(D_800D2E68 & 4)) && (v != 0x2C) &&
            (p->pad196[1] == 0) && (v != 0) && (v != 0x1F) && (obj->in_water == 0)) {
            arg0->unk11 = 1;
            arg0->unkE = 0x78;
            func_10010F30(0x1EB, 0x7FFF, 0x40, 0, 0);
            return;
        }
        if (arg0->unk14 != NULL) {
            func_1515F10C(arg0->unk14);
            arg0->unk14 = NULL;
        }
        break;
    case 1:
        t = arg0->unkE;
        t -= D_800BE9E4;
        if ((t <= 0) || (D_800CC2D0[idx].unk31C->unk75 == 0)) {
            arg0->unk11 = 2;
            t = 0;
        }
        arg0->unkE = t;
        arg0->unk12 += D_800BE9E4 * 2;
        if (arg0->unk14 != NULL) {
            f32 f;

            if (D_800CC2D0[idx].unk31C->unk75 == 5) {
                f = 85.0f;
            } else {
                f = 0.0f;
            }
            arg0->unk14->unkE = obj->x_position;
            arg0->unk14->unk10 = func_15048A40(arg0->unk12) * 15.0f + (obj->y_position + 160.0f + arg0->unk98 + f);
            arg0->unk14->unk12 = obj->z_position;
        } else {
            arg0->unk14 = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    case 2:
        if (D_800CC2D0[idx].unk31C->unk75 == 0) {
            arg0->unk11 = 0;
        }
        if (arg0->unk14 != NULL) {
            func_1515F10C(arg0->unk14);
            arg0->unk14 = NULL;
        }
        break;
    case 3:
        t = arg0->unkE;
        t -= D_800BE9E4;
        if (t <= 0) {
            arg0->unk11 = 2;
            arg0->unkE = 0;
            break;
        }
        arg0->unkE = t;
        arg0->unk12 += D_800BE9E4 * 2;
        if (arg0->unk14 != NULL) {
            arg0->unk14->unkE = obj->x_position;
            arg0->unk14->unk10 = func_15048A40(arg0->unk12) * 15.0f + (obj->y_position + 160.0f);
            arg0->unk14->unk12 = obj->z_position;
        } else {
            arg0->unk14 = func_1515D5F8(0, 0, 0, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
        }
        break;
    }
}

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13[5];
    Mtx unk18[2];
    f32 unk98;
} Obj15155CFC;

extern Gfx *func_15096934(Gfx *);
extern s32 *D_800E03E0;

Gfx *func_15155CFC(Gfx *gfx, Obj15155CFC *arg1, s16 arg2) {
    s32 state;
    s32 idx;
    f32 sp5C;
    struct127 *obj;
    f32 f;

    state = arg1->unk11;
    idx = arg1->unk10;
    if ((state == 1) || (state == 3)) {
        obj = &D_800CC2D0[idx];
        if (obj->unk31C->unk75 == 5) {
            f = 85.0f;
        } else {
            f = 0.0f;
        }
        sp5C = D_800DBFF0[arg2].unk380;
        gfx = func_15096934(gfx);
        func_15043D90(&arg1->unk18[D_800BE9C0], 0.0f, sp5C, 0.0f, 2.25f, 2.25f, 2.25f, obj->x_position,
                      func_15048A40(arg1->unk12) * 15.0f + (obj->y_position + 160.0f + f + arg1->unk98),
                      obj->z_position);
        gSPMatrix(gfx++, &arg1->unk18[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPClearGeometryMode(gfx++, G_ZBUFFER);
        gSPDisplayList(gfx++, *D_800E03E0);
        gSPSetGeometryMode(gfx++, G_ZBUFFER);
    }
    return gfx;
}

void func_1515F10C(struct226 *arg0, struct225 *arg1);
void func_1518CA04(s32 arg0);

void func_15155EF8(struct225 *arg0) {
    if (arg0->unk14 != NULL) {
        func_1515F10C(arg0->unk14, arg0);
    }
    func_15169804((struct102 *)arg0);
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

typedef struct {
    u8 pad0[0x10];
    u8 owner;
} Node15156028;

void func_15156028(Node15156028 *arg0, struct127 **arg1, u8 arg2) {
    if (arg2 == 0) {
        s32 a = arg1[0] - D_800CC2D0;

        if ((a >= 0) && (a < 25) && (a == arg0->owner)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        s32 a = arg1[0] - D_800CC2D0;
        s32 b = arg1[1] - D_800CC2D0;

        if ((a >= 0) && (a < 25) && (b >= 0) && (b < 25)) {
            if (a == arg0->owner) {
                arg0->owner = b;
            } else if (b == arg0->owner) {
                arg0->owner = a;
            }
        }
    }
}
