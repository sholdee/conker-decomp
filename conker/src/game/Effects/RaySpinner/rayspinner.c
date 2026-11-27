#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"

/* Per-object state stored at offset 0x28 of the object. */
typedef struct {
    Vec3f position;
    Vec3f rotation;
    Vec3f angularVelocity;
    LightEffect *children[6]; /* light_createWithEffect lights, one per ray */
    f32 intensity;
    s32 unk40;
    u8 flags; // bit 0: enabled
} FF0E0State;

struct FF0E0Object {
    TimedEffect base;
    FF0E0State state;
};

/*
 * This file's .rodata front-end data: the six ray directions, the colours of the six children
 * (level_spawnDiscoBallZone of game/Level/ZoneSpawn/zonespawn.c reads the first six), and the spin presets that game/Script/EffectCmd/effectcmd.c sends
 * with message 0x32.
 */
static const Vec3f gRaySpinnerDirections[6] = {
    { 0.0f, 10.0f, 0.0f }, { 0.0f, -10.0f, 0.0f }, { 10.0f, 0.0f, 0.0f },
    { -10.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, -10.0f },
};
const u8 gRaySpinnerChildColors[13][3] = {
    { 0xFF, 0x00, 0x00 }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 }, { 0x00, 0xFF, 0x00 },
    { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF }, { 0x00, 0x00, 0xFF },
    { 0x00, 0x00, 0xFF }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0xFF },
    { 0xFF, 0x00, 0x00 },
};
const FF0E0SpinMsg gRaySpinnerSpinPresets[3] = {
    { { 0.91200006f, 2.4320002f, -1.064f }, 0.00129f },
    { { 1.2160001f, 5.6250005f, -0.76000005f }, 0.012363f },
    { { 3.3450003f, 9.882001f, -2.4320002f }, 0.03f },
};

/* Spins the object and casts a ray from its position along each of the six rotated directions,
 * moving each child's light to the hit point (light->disabled = 0) or setting light->disabled = 1 on a miss or while disabled. */
void effect_updateRaySpinner(FF0E0Object *obj) {
    FF0E0State *state;
    LightEffect *child;
    MtxF mtx;
    s32 i;

    state = &obj->state;
    if (obj->state.flags & 1) {
        state->rotation.x += state->angularVelocity.x * gFrameDelta;
        state->rotation.y += state->angularVelocity.y * gFrameDelta;
        state->rotation.z += state->angularVelocity.z * gFrameDelta;
        math_mtxRotateRPYF(mtx, state->rotation.x, state->rotation.y, state->rotation.z);
        for (i = 0; i < 6; i++) {
            child = state->children[i];
            if (child != NULL) {
                f32 targetZ;
                f32 targetY;
                f32 targetX;
                f32 hitZ;
                f32 hitY;
                f32 hitX;
                f32 fraction;
                math_transformPoint(mtx, gRaySpinnerDirections[i].x, gRaySpinnerDirections[i].y, gRaySpinnerDirections[i].z, &targetX, &targetY, &targetZ);
                if (collision_castRayWorld(state->position.x, state->position.y, state->position.z, targetX, targetY, targetZ,
                                  NULL, NULL, &hitX, &hitY, &hitZ, &fraction, NULL, NULL, 0.0f)) {
                    child->light->disabled = 0;
                    child->light->x = hitX;
                    child->light->y = hitY;
                    child->light->z = hitZ;
                    child->light->radius = state->intensity * fraction;
                } else {
                    child->light->disabled = 1;
                }
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            LightEffect *disabledChild = state->children[i];
            if (disabledChild != NULL) {
                disabledChild->light->disabled = 1;
            }
        }
    }
}
/* Message handler: 0x32 sets the spin and intensity, 0x30/0x31 enable/disable the object when the
 * argument matches unk40, 0x4E resets the spin from gZeroVec3f, 0x4F passes the object to effect_destroyObject. */
void effect_raySpinnerMessage(FF0E0Object *obj, s32 arg, u8 msg) {
    FF0E0State *state;
    FF0E0SpinMsg *msgData;

    state = &obj->state;
    if (msg == 0x32) {
        msgData = (FF0E0SpinMsg *)arg;
        state->angularVelocity = msgData->angularVelocity;
        state->intensity = msgData->intensity;
    // matching: empty labelled block makes as1 fill the branch delay slot from the epilogue
    end_set: ;
    } else if (msg == 0x30 || msg == 0x31) {
        if (arg == state->unk40) {
            if (msg == 0x30) {
                state->flags |= 1;
            } else {
                state->flags &= ~1;
            // matching: as above
            end_disable: ;
            }
        }
    } else if (msg == 0x4E) {
        obj->state.angularVelocity = gZeroVec3f;
    } else if (msg == 0x4F) {
        effect_destroyObject(&obj->base.header);
    }
}

/* Passes every child to effect_destroyObject. */
void effect_destroyRaySpinnerChildren(FF0E0Object *obj) {
    FF0E0State *state;
    LightEffect *child;
    u8 i;

    state = &obj->state;
    for (i = 0; i < 6; i++) {
        child = state->children[i];
        if (child != NULL) {
            effect_destroyObject(child);
        }
    }
}

void effect_destroyRaySpinner(FF0E0Object *obj) {
    effect_destroyRaySpinnerChildren(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}

void effect_freeRaySpinner(FF0E0Object *obj) {
    effect_destroyRaySpinnerChildren(obj);
    effect_defaultDestroyTimedEffect(&obj->base);
}
