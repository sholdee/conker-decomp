#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Rumble effect objects: a timed rumble on the controllers of the players in a mask. */

/* Spawn parameters of a rumble, copied into RumbleObj.params. */
typedef struct {
    /* 0x0 */ u8 flags;       /* bit 0: duration counts down */
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ u8 strength;    /* 0 (off) to 8 (full), see func_1501C010 */
    /* 0x5 */ u8 playerMask;  /* bit i: player i */
    /* 0x6 */ s8 updateKind;  /* index into D_8008FCC0, -1: none */
    /* 0x7 */ u8 unk7;
} RumbleParams;

/* Distance falloff data of a RumbleFalloffObj. */
typedef struct {
    /* 0x00 */ struct17 pos;
    /* 0x0C */ f32 innerRadius;  /* full strength inside this distance */
    /* 0x10 */ f32 falloffRange; /* zero strength beyond innerRadius + falloffRange */
    /* 0x14 */ f32 falloffScale; /* strength lost per unit beyond innerRadius */
    /* 0x18 */ u8 view;
} RumbleFalloff;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];    /* object header */
    /* 0x0E */ RumbleParams params;
    /* 0x16 */ u8 appliedStrength;
    /* 0x17 */ u8 pad17;
} RumbleObj; /* followed by the spawner's extra data */

/* A rumble whose strength falls off with distance from the camera. */
typedef struct {
    /* 0x00 */ RumbleObj base;
    /* 0x18 */ RumbleFalloff falloff;
} RumbleFalloffObj;

extern u8 D_80084060[]; /* controller port of each player, >= 4: none */
extern struct17 *func_15144B34(s32 view);
extern f32 func_15143E64(struct17 *);
extern void (*D_8008FCC0[])(RumbleObj *);
void func_1501C010(u8 player, u8 strength);
void func_1501C17C(u8 player);

/* Returns whether any player in `playerMask` has a rumble pak; false at the first player without a
 * controller. */
s32 func_151D87E0(u8 playerMask) {
    u8 port;
    u8 player;

    for (player = 0; player < 4; player++) {
        if (playerMask & (1 << player)) {
            port = D_80084060[player];
            if (port >= 4) {
                return FALSE;
            }
            if (D_800BE944[port] != 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

extern u8 D_800BEAC1;
extern u8 D_800BEAC2;
extern u8 D_800BEAC3;
s32 func_15181CC8(s32 view);
s32 func_1517EF00(s32 view);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Spawns a rumble object (kind 0x3F) with `dataSize` bytes of extra data and starts the rumble.
 * Returns NULL while D_800E0B94 or any of D_800BEAC0..3 is set, when no player in the mask has a rumble pak, or when
 * any of those players' views is irised or fading. */
RumbleObj *func_151D8868(RumbleParams *params, s32 dataSize, u8 arg2, s32 arg3) {
    RumbleObj *obj;
    u8 player;

    if (D_800E0B94 != 0) {
        return NULL;
    }
    if (!func_151D87E0(params->playerMask)) {
        return NULL;
    }
    if (D_800BEAC0 != 0 || D_800BEAC1 != 0 || D_800BEAC2 != 0 || D_800BEAC3 != 0) {
        return NULL;
    }
    {
        u8 view;

        for (view = 0; view <= D_80082FA0; view++) {
            if (params->playerMask & (1 << view)) {
                if (!func_15181CC8(view) || func_1517EF00(view) != 0) {
                    return NULL;
                }
            }
        }
    }
    obj = func_15167A68(0x3F, arg3, dataSize + sizeof(RumbleObj), 1, arg2, 1);
    if (obj == NULL) {
        return NULL;
    }
    memcpy(&obj->params, params, sizeof(RumbleParams));
    for (player = 0; player < 4; player++) {
        if (obj->params.playerMask & (1 << player)) {
            func_1501C010(player, params->strength);
        }
    }
    obj->appliedStrength = params->strength;
    return obj;
}

/* Per-frame update: counts the duration down, runs the update handler, re-applies a changed
 * strength, and destroys the object once the duration has run out. */
void func_151D8A24(RumbleObj *obj) {
    u8 pad[4]; // matching: unused stack slot in the original frame
    u8 expired;
    u8 player;
    s32 updateKind;

    expired = FALSE;
    if (obj->params.flags & 1) {
        obj->params.duration -= D_800BE9E4;
        if (obj->params.duration < 0) {
            expired = TRUE;
        }
    }

    updateKind = obj->params.updateKind;
    if (updateKind != -1) {
        D_8008FCC0[updateKind](obj);
    }

    if (obj->appliedStrength != obj->params.strength) {
        for (player = 0; player < 4; player++) {
            if (obj->params.playerMask & (1 << player)) {
                func_1501C17C(player);
                func_1501C010(player, obj->params.strength);
            }
        }
        obj->appliedStrength = obj->params.strength;
    }

    if (expired) {
        func_1516972C((struct102 *)obj);
    }
}

void func_1501C17C(u8);
void func_151D8B24(struct102 *arg0)
{
  s32 i;
  s32 j;
  i = 0;
  j = 0;
  do
  {
    if (arg0->unk13 & (1 << j))
    {
      func_1501C17C((u8) i);
    }
    i++;
    j = (i = (u8) i);
  }
  while (j < 4);
}

void func_151D8B24(struct102 *);


void func_151D8B24(struct102 *);

void func_151D8B88(struct102 *arg0) {
    func_151D8B24(arg0);
    func_15169804(arg0);
}

void func_151D8BB4(struct102 *arg0) {
    func_151D8B24(arg0);
    func_15169824(arg0);
}

void func_151D8C00(RumbleObj *obj, RumbleFalloff *falloff);

void func_151D8BE0(RumbleFalloffObj *obj) {
    func_151D8C00(&obj->base, &obj->falloff);
}

/* Sets the rumble strength from the distance between `falloff->pos` and the view's camera. */
void func_151D8C00(RumbleObj *obj, RumbleFalloff *falloff) {
    f32 scale;
    struct17 delta;
    struct17 *camera;
    f32 dist;

    camera = func_15144B34(falloff->view);
    delta.unk0 = falloff->pos.unk0 - camera->unk0;
    delta.unk4 = falloff->pos.unk4 - camera->unk4;
    delta.unk8 = falloff->pos.unk8 - camera->unk8;

    dist = func_15143E64(&delta);
    if (dist < falloff->innerRadius) {
        scale = 1.0f;
    } else if (falloff->innerRadius + falloff->falloffRange < dist) {
        scale = 0.0f;
    } else {
        scale = 1.0f - (dist - falloff->innerRadius) * falloff->falloffScale;
    }

    obj->params.strength = scale * 8.0f;
}

void func_151D8D5C(struct102 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x58) {
        func_1516972C(arg0);
    } else if (arg2 == 0x47) {
        func_1516972C(arg0);
    }
}

extern f32 D_800AB300;

void func_151D8DB4(s32 arg0, u8 arg1) {
    func_15169260(&D_800AB300, 1, arg0, arg1);
}

extern u8 D_800E0A00;

void func_151D8DE8(void) {
    D_800E0A00 = 1;
    func_151D8DB4(0, 0x58);
}
