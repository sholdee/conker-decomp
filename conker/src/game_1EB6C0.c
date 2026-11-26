#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Callbacks of the burst effect's sparks and glows (spawned by func_151BD828 in game_1E9A30.c), and the
 * message handler of a particle that follows an owner actor.
 */

/* Per-frame update of a burst spark: flickers its width, height and alpha towards random targets. */
s32 func_151BE210(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    data->widthTimer -= D_800BE9A4;
    if (data->widthTimer < 0.0f) {
        data->widthTimer = func_150ADA68() * 4.0f;
        data->widthTarget = func_150ADA68() * data->widthRange + data->widthBase;
    }
    spark->head.def.width += (data->widthTarget - spark->head.def.width) * 0.35f;
    data->heightTimer -= D_800BE9A4;
    if (data->heightTimer < 0.0f) {
        data->heightTimer = func_150ADA68() * 9.0f;
        if (func_150ADA20() & 1) {
            data->heightTarget = func_150ADA68() * data->heightRange + data->heightBase;
        } else {
            data->heightTarget = func_150ADA68() * data->heightRange2 + data->heightBase;
        }
    }
    spark->head.def.height += (data->heightTarget - spark->head.def.height) * 0.4f;
    data->alphaTimer -= D_800BE9A4;
    if (data->alphaTimer < 0.0f) {
        data->alphaTimer = func_150ADA68() * 10.0f;
        data->alphaTarget = func_150ADA68() * data->alphaRange + data->alphaBase;
    }
    data->alpha += (data->alphaTarget - data->alpha) * 0.4f;
    spark->head.def.unk44 = data->alpha;
    data->beamWidth += (data->beamWidthTarget - data->beamWidth) * 0.65000004f;
    data->beamLength += (data->beamLengthTarget - data->beamLength) * 0.65000004f;
    return 1;
}

/* Builds a burst spark's beam vertices; frees the spark once its owner is gone, and draws nothing while it is hidden. */
Vtx *func_151BE4B8(BurstSpark *spark, s16 viewport) {
    BurstSparkData *data = &spark->data;
    Actor *owner = data->owner;
    BeamScale scale;

    if (owner->unique_id != data->ownerId || owner->type == 0) {
        func_1516972C(&spark->head.header);
        return NULL;
    }
    if (!(data->flags & 1)) {
        return NULL;
    }
    scale.width = data->beamWidth * data->scale;
    scale.length = data->beamLength * data->scale;
    return func_1513FAB4(spark, FALSE, &scale, viewport);
}

/* A func_15130280 particle (game_15D730.c's Particle) whose caller-sized data (at 0xA8) holds its owner at +8. */
struct OwnedParticle {
    EffectHeader header;
    u8 pad10[0xA0];
    ActorRef owner;
};

/* Message handler: 0x2D makes the particle follow its owner through a swap; message 0 for its owner frees it. */
void func_151BE558(OwnedParticle *particle, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &particle->owner;

    if (msgId == 0x2D) {
        Actor *actorA;

        if (owner->actor == (actorA = msg->swap.actorA)) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
        } else if (owner->actor == msg->swap.actorB) {
            owner->actor = actorA;
            owner->uniqueId = msg->swap.idA;
        }
    } else if (msgId == 0) {
        Actor *actor = msg->ref.actor;

        if (actor == owner->actor || msg->ref.uniqueId == owner->uniqueId) {
            func_1516972C(&particle->header);
        }
    }
}

/* Message handler of a burst spark: passes the message on with the spark's owner reference. */
void func_151BE604(BurstSpark *spark, s32 msg, u8 msgId) {
    func_15169850(msg, msgId, &spark->data.owner, &spark->data.ownerId, spark);
}

/* Clears the spark's slot in its burst effect. */
void func_151BE644(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->sparks[data->index] = NULL;
    }
}

/* Teardown of a burst spark: clears its slot in the burst, then runs the particle teardown func_1513CA6C. */
void func_151BE674(BurstSpark *spark) {
    func_151BE644(spark);
    func_1513CA6C(&spark->head);
}

/* Teardown of a burst spark: clears its slot in the burst, then runs the particle teardown func_1513CAA0. */
void func_151BE6A0(BurstSpark *spark) {
    func_151BE644(spark);
    func_1513CAA0(&spark->head);
}

/* Per-frame update of a burst glow: switches it to a random texture every 3-10 frames and eases its scale. */
s32 func_151BE6CC(BurstGlow *glow) {
    s32 pad; // matching: unused stack slot above textures
    s32 textures[4] = { 0x60, 0x61, 0x62, 0x63 };
    GlowShape *shape = &glow->shape;

    glow->data.textureTimer -= D_800BE9E4;
    if (glow->data.textureTimer < 0) {
        glow->head.def.texture = textures[func_150ADA20() & 3];
        glow->data.textureTimer = (func_150ADA20() & 7) + 3;
    }
    shape->scale += (glow->data.targetScale - shape->scale) * 0.65000004f;
    return 1;
}

/* Message handler of a burst glow: passes the message on with the glow's owner reference. */
void func_151BE788(BurstGlow *glow, s32 msg, u8 msgId) {
    func_15169850(msg, msgId, &glow->data.owner, &glow->data.ownerId, glow);
}

/* Clears the glow's slot in its burst effect. */
void func_151BE7C8(BurstGlow *glow) {
    BurstGlowData *data = &glow->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->glows[data->index] = NULL;
    }
}

/* Teardown of a burst glow: clears its slot in the burst, then runs the particle teardown func_151411A4. */
void func_151BE7F8(BurstGlow *glow) {
    func_151BE7C8(glow);
    func_151411A4(&glow->head);
}

/* Teardown of a burst glow: clears its slot in the burst, then runs the particle teardown func_151411C4. */
void func_151BE824(BurstGlow *glow) {
    func_151BE7C8(glow);
    func_151411C4(&glow->head);
}
