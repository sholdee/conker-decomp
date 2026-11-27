#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/cont.h describes it. */
u8 gRumbleSuppressed; /* set by cont_killAllRumble; game/Cont/PadRead/padread keeps the motors off while it is set */

#include "functions.h"
#include "variables.h"

/* Rumble effect objects: a timed rumble on the controllers of the players in a mask. */

/* The update handlers (RumbleParams.updateKind). Declared without a prototype: the entry takes the
 * object as its own, longer view (RumbleFalloffObj). */
static void (*gRumbleUpdateHandlers[])() = { cont_updateRumbleFalloff };

/* Returns whether any player in `playerMask` has a rumble pak; false at the first player without a
 * controller. */
s32 cont_hasRumblePak(u8 playerMask) {
    u8 port;
    u8 player;

    for (player = 0; player < 4; player++) {
        if (playerMask & (1 << player)) {
            port = gPlayerChannels[player];
            if (port >= 4) {
                return FALSE;
            }
            if (gRumblePakPresent[port] != 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/* Spawns a rumble object (kind 0x3F) with `dataSize` bytes of extra data and starts the rumble.
 * Returns NULL while gFrontEndState or any of gPaused..3 is set, when no player in the mask has
 * a rumble pak, or when any of those players' views is irised or fading. */
RumbleObj *cont_spawnRumble(RumbleParams *params, s32 dataSize, u8 arg2, s32 arg3) {
    RumbleObj *obj;
    u8 player;

    if (gFrontEndState != 0) {
        return NULL;
    }
    if (!cont_hasRumblePak(params->playerMask)) {
        return NULL;
    }
    if (gPaused != 0 || gPauseRequested != 0 || gPauseCooldown != 0 || gRumbleStopRequest != 0) {
        return NULL;
    }
    {
        u8 view;

        for (view = 0; view <= gScreenLayout; view++) {
            if (params->playerMask & (1 << view)) {
                if (!screen_isIrisOpen(view) || screen_getViewFadeAlpha(view) != 0) {
                    return NULL;
                }
            }
        }
    }
    obj = effect_newObject(0x3F, arg3, dataSize + sizeof(RumbleObj), 1, arg2, 1);
    if (obj == NULL) {
        return NULL;
    }
    memcpy(&obj->params, params, sizeof(RumbleParams));
    for (player = 0; player < 4; player++) {
        if (obj->params.playerMask & (1 << player)) {
            cont_startRumble(player, params->strength);
        }
    }
    obj->appliedStrength = params->strength;
    return obj;
}

/* Per-frame update: counts the duration down, runs the update handler, re-applies a changed
 * strength, and releases the object once the duration has run out. */
void cont_updateRumbleObj(RumbleObj *obj) {
    u8 pad[4]; // matching: unused stack slot in the original frame
    u8 expired;
    u8 player;
    s32 updateKind;

    expired = FALSE;
    if (obj->params.flags & 1) {
        obj->params.duration -= gFrameTicks;
        if (obj->params.duration < 0) {
            expired = TRUE;
        }
    }

    updateKind = obj->params.updateKind;
    if (updateKind != -1) {
        gRumbleUpdateHandlers[updateKind](obj);
    }

    if (obj->appliedStrength != obj->params.strength) {
        for (player = 0; player < 4; player++) {
            if (obj->params.playerMask & (1 << player)) {
                cont_stopRumble(player);
                cont_startRumble(player, obj->params.strength);
            }
        }
        obj->appliedStrength = obj->params.strength;
    }

    if (expired) {
        effect_destroyObject(obj);
    }
}

/* Stops the rumble of every player in the object's mask. */
void cont_stopRumbleObj(RumbleObj *obj) {
    u8 player;

    for (player = 0; player < 4; player++) {
        if (obj->params.playerMask & (1 << player)) {
            cont_stopRumble(player);
        }
    }
}

/* Destroy hook: stops the rumble, then the default destroy (effect_defaultDestroy). */
void cont_destroyRumbleObj(RumbleObj *obj) {
    cont_stopRumbleObj(obj);
    effect_defaultDestroy(obj);
}

/* Free hook: stops the rumble, then the default free (effect_defaultFree). */
void cont_freeRumbleObj(RumbleObj *obj) {
    cont_stopRumbleObj(obj);
    effect_defaultFree(obj);
}


/* Sets a RumbleFalloffObj's strength from its own falloff data. */
void cont_updateRumbleFalloff(RumbleFalloffObj *obj) {
    cont_applyRumbleFalloff(&obj->base, &obj->falloff);
}

/* Sets the rumble strength from the distance between `falloff->pos` and the view's camera. */
void cont_applyRumbleFalloff(RumbleObj *obj, RumbleFalloff *falloff) {
    f32 scale;
    Vec3f delta;
    Vec3f *camera;
    f32 dist;

    camera = math_getCameraPos(falloff->view);
    delta.x = falloff->pos.x - camera->x;
    delta.y = falloff->pos.y - camera->y;
    delta.z = falloff->pos.z - camera->z;

    dist = math_vec3Length(&delta);
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
void cont_handleRumbleMessage(RumbleObj *obj, s32 data, u8 message) {
    if (message == 0x58) {
        effect_destroyObject(obj);
    } else if (message == 0x47) {
        effect_destroyObject(obj);
    }
}

static const s32 gRumbleObjKinds[1] = { 0x3F }; /* object kinds to message: rumble objects */

/* Sends `message` with `data` to every rumble object. */
void cont_sendRumbleMessage(s32 data, u8 message) {
    effect_sendEventToTypes(gRumbleObjKinds, 1, data, message);
}


/* Sets gRumbleSuppressed and releases every rumble object (message 0x58). */
void cont_killAllRumble(void) {
    gRumbleSuppressed = 1;
    cont_sendRumbleMessage(0, 0x58);
}
