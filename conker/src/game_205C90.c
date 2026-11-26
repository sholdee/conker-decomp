#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/cont.h describes it. */
u8 D_800E0A00; /* set by func_151D8DE8; game_48FD0 keeps the motors off while it is set */

#include "functions.h"
#include "variables.h"

/* Rumble effect objects: a timed rumble on the controllers of the players in a mask. */

/* The update handlers (RumbleParams.updateKind). Declared without a prototype: the entry takes the
 * object as its own, longer view (RumbleFalloffObj). */
static void (*D_8008FCC0[])() = { func_151D8BE0 };

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

/* Spawns a rumble object (kind 0x3F) with `dataSize` bytes of extra data and starts the rumble.
 * Returns NULL while D_800E0B94 or any of D_800BEAC0..3 is set, when no player in the mask has
 * a rumble pak, or when any of those players' views is irised or fading. */
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
 * strength, and releases the object once the duration has run out. */
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
        func_1516972C(obj);
    }
}

/* Stops the rumble of every player in the object's mask. */
void func_151D8B24(RumbleObj *obj) {
    u8 player;

    for (player = 0; player < 4; player++) {
        if (obj->params.playerMask & (1 << player)) {
            func_1501C17C(player);
        }
    }
}

/* Destroy hook: stops the rumble, then the default destroy (func_15169804). */
void func_151D8B88(RumbleObj *obj) {
    func_151D8B24(obj);
    func_15169804(obj);
}

/* Free hook: stops the rumble, then the default free (func_15169824). */
void func_151D8BB4(RumbleObj *obj) {
    func_151D8B24(obj);
    func_15169824(obj);
}


/* Sets a RumbleFalloffObj's strength from its own falloff data. */
void func_151D8BE0(RumbleFalloffObj *obj) {
    func_151D8C00(&obj->base, &obj->falloff);
}

/* Sets the rumble strength from the distance between `falloff->pos` and the view's camera. */
void func_151D8C00(RumbleObj *obj, RumbleFalloff *falloff) {
    f32 scale;
    Vec3f delta;
    Vec3f *camera;
    f32 dist;

    camera = func_15144B34(falloff->view);
    delta.x = falloff->pos.x - camera->x;
    delta.y = falloff->pos.y - camera->y;
    delta.z = falloff->pos.z - camera->z;

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

/* Message handler: messages 0x58 and 0x47 release the object. */
void func_151D8D5C(RumbleObj *obj, s32 data, u8 message) {
    if (message == 0x58) {
        func_1516972C(obj);
    } else if (message == 0x47) {
        func_1516972C(obj);
    }
}

static const s32 D_800AB300[1] = { 0x3F }; /* object kinds to message: rumble objects */

/* Sends `message` with `data` to every rumble object. */
void func_151D8DB4(s32 data, u8 message) {
    func_15169260(D_800AB300, 1, data, message);
}


/* Sets D_800E0A00 and releases every rumble object (message 0x58). */
void func_151D8DE8(void) {
    D_800E0A00 = 1;
    func_151D8DB4(0, 0x58);
}
