#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Data of the effect spawned by func_151001B4 for an actor. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer;  /* counts down by D_800BE9E4 while the owner has a camera */
} OwnerEffectData;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ OwnerEffectData data;
} OwnerEffect;

/* Sends message 0x48 (func_151494E0) for `owner`; effects it owns destroy themselves on it. */
void func_15100180(Actor *owner) {
    ActorRef msg;

    msg.actor = owner;
    msg.uniqueId = owner->unique_id;
    func_151494E0((s32)&msg, 0x48);
}

/* Spawns the owner effect (func_15149130) on `owner`. */
void func_151001B4(Actor *owner) {
    OwnerEffectData data;
    struct260 *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0;

    // the header's struct37 * parameter is really the size of the effect's data
    effect = func_15149130(300, -1, 0x4E, -1, 0, 0x3B, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(&((OwnerEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Message handler: destroys the effect when its owner is reported gone (0x48); other messages
 * go to the default handler. */
void func_15100230(OwnerEffect *effect, ActorRef *msg, u8 msgId) {
    OwnerEffectData *data = &effect->data;

    if (msgId == 0x48) {
        // matching: the punned read of msg makes IDO load it a second time for the id compare
        if (msg->actor == data->owner || data->ownerId == ((ActorRef *)*(s32 *)&msg)->uniqueId) {
            func_1516972C((struct102 *)effect);
        }
    } else {
        func_15149514((s32)msg, msgId, &data->owner, &data->ownerId, effect);
    }
}

/* Per-frame update: frees the effect once its owner is gone, otherwise counts its timer down
 * while the owner has a camera. */
void func_151002BC(OwnerEffect *effect) {
    OwnerEffectData *data = &effect->data;
    Actor *owner = data->owner;

    if (owner->interaction_state == 0 || owner->id == 0xFF || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->camera != NULL) {
        data->timer -= D_800BE9E4;
    }
}
