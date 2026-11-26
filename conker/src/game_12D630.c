#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
    TimedEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0;

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
            func_1516972C(&effect->base.header);
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

    if (owner->type == 0 || owner->character_type == 0xFF || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->CameraPtr != NULL) {
        data->timer -= D_800BE9E4;
    }
}
