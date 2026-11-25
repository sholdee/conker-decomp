#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Callbacks of the burst effect's sparks and glows (spawned by func_151BD828 in game_1E9A30.c), and the
 * message handler of a particle that follows an owner actor.
 */

/* Weak reference to an actor: valid while actor->unique_id still equals uniqueId. */
typedef struct {
    struct127 *actor;
    u8 uniqueId;
} ActorRef;

/* Message 0x2D payload: two actors exchange identities. */
typedef struct {
    struct127 *actorA;
    struct127 *actorB;
    u8 idA;
    u8 idB;
} ActorSwapMsg;

/* Payload of the actor messages handled below (0: an actor, 0x2D: an actor swap). */
typedef union {
    ActorRef ref;
    ActorSwapMsg swap;
} ActorMsg;

/* Head of a particle spawned by func_1513D2F0 (game_169510.c's Particle); its ParticleDef starts at 0x18. */
typedef struct {
    u8 pad0[0x18];
    u8 texture;            /* 0x18: def.texture */
    u8 pad19[0x13];
    f32 width;             /* 0x2C: def.width */
    f32 height;            /* 0x30: def.height */
    struct17 start;        /* 0x34: def.pos */
    struct17 end;          /* 0x40: def.rot, the far end of a beam */
    u8 pad4C[0x10];
    u8 alpha;              /* 0x5C */
    u8 pad5D[0x110 - 0x5D];
} ParticleHead; /* size 0x110 */

typedef struct BurstSpark BurstSpark;
typedef struct BurstGlow BurstGlow;

/* Common head of effect objects; each effect's own data starts at 0x28. */
typedef struct {
    u8 pad0[0x28];
} EffectHeader;

/* Data of the burst effect object (at 0x28). */
typedef struct {
    ActorRef owner;
    BurstSpark *sparks[2];
    BurstGlow *glows[2];
} BurstData;

typedef struct {
    EffectHeader header;
    BurstData data;
} BurstEffect;

/*
 * Per-object data func_151BD828 copies into each spark (at spark+0x110). Each flickering value has a
 * countdown timer; when it runs out a new random target base + rand * range is picked.
 */
typedef struct {
    struct127 *owner;
    u8 ownerId;
    u8 index;              /* slot in burst->data.sparks */
    u8 pad06[2];
    f32 widthBase;         /* 0x08 */
    f32 heightBase;        /* 0x0C */
    f32 widthRange;        /* 0x10 */
    f32 heightRange;       /* 0x14: one of the two height ranges */
    f32 widthTarget;       /* 0x18 */
    f32 heightTarget;      /* 0x1C */
    f32 heightRange2;      /* 0x20: the other height range */
    f32 widthTimer;        /* 0x24 */
    f32 heightTimer;       /* 0x28 */
    f32 alphaBase;         /* 0x2C */
    f32 alphaRange;        /* 0x30 */
    f32 alphaTarget;       /* 0x34 */
    f32 alphaTimer;        /* 0x38 */
    f32 alpha;             /* 0x3C */
    f32 beamWidth;         /* 0x40: BeamScale passed to func_1513FAB4 (times scale) */
    f32 beamLength;        /* 0x44 */
    f32 beamWidthTarget;   /* 0x48 */
    f32 beamLengthTarget;  /* 0x4C */
    BurstEffect *burst;
    f32 scale;             /* 0x54: the owner's average scale */
    u8 flags;              /* bit 0: owner visible */
    u8 pad59[3];
} BurstSparkData;

/* Glow parameters func_151407D0 copies to glow+0x110 (game_16DC80.c's GlowParams); only scale is used here. */
typedef struct {
    u8 pad0[0x48];
    f32 scale;
    u8 pad4C[0x14];
} BurstGlowShape;

/* Per-object data func_151BD828 copies into each glow (at glow+0x170). */
typedef struct {
    s16 textureTimer;      /* frames until the next random texture */
    u8 pad02[2];
    f32 targetScale;       /* shape.scale eases towards it */
    BurstEffect *burst;
    u8 index;              /* slot in burst->data.glows */
    u8 pad0D[3];
    struct127 *owner;
    u8 ownerId;
    u8 pad15[3];
} BurstGlowData;

struct BurstSpark {
    ParticleHead particle;
    BurstSparkData data;   /* 0x110 */
};

struct BurstGlow {
    ParticleHead particle;
    BurstGlowShape shape;  /* 0x110 */
    BurstGlowData data;    /* 0x170 */
};

/* Fractions of a beam particle's width and length to draw. */
typedef struct {
    f32 width;
    f32 length;
} BeamScale;

Vtx *func_1513FAB4(void *particle, u8 reversed, BeamScale *scale, s16 viewport);

/* Per-frame update of a burst spark: flickers its width, height and alpha towards random targets. */
s32 func_151BE210(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    data->widthTimer -= D_800BE9A4;
    if (data->widthTimer < 0.0f) {
        data->widthTimer = func_150ADA68() * 4.0f;
        data->widthTarget = func_150ADA68() * data->widthRange + data->widthBase;
    }
    spark->particle.width += (data->widthTarget - spark->particle.width) * 0.35f;
    data->heightTimer -= D_800BE9A4;
    if (data->heightTimer < 0.0f) {
        data->heightTimer = func_150ADA68() * 9.0f;
        if (func_150ADA20() & 1) {
            data->heightTarget = func_150ADA68() * data->heightRange + data->heightBase;
        } else {
            data->heightTarget = func_150ADA68() * data->heightRange2 + data->heightBase;
        }
    }
    spark->particle.height += (data->heightTarget - spark->particle.height) * 0.4f;
    data->alphaTimer -= D_800BE9A4;
    if (data->alphaTimer < 0.0f) {
        data->alphaTimer = func_150ADA68() * 10.0f;
        data->alphaTarget = func_150ADA68() * data->alphaRange + data->alphaBase;
    }
    data->alpha += (data->alphaTarget - data->alpha) * 0.4f;
    spark->particle.alpha = data->alpha;
    data->beamWidth += (data->beamWidthTarget - data->beamWidth) * 0.65000004f;
    data->beamLength += (data->beamLengthTarget - data->beamLength) * 0.65000004f;
    return 1;
}

/* Builds a burst spark's beam vertices; frees the spark once its owner is gone, and draws nothing while it is hidden. */
Vtx *func_151BE4B8(BurstSpark *spark, s16 viewport) {
    BurstSparkData *data = &spark->data;
    struct127 *owner = data->owner;
    BeamScale scale;

    if (owner->unique_id != data->ownerId || owner->interaction_state == 0) {
        func_1516972C((struct102 *)spark);
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
typedef struct {
    u8 pad0[0xB0];
    ActorRef owner;
} OwnedParticle;

/* Message handler: 0x2D makes the particle follow its owner through a swap; message 0 for its owner frees it. */
void func_151BE558(OwnedParticle *particle, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &particle->owner;

    if (msgId == 0x2D) {
        struct127 *actorA;

        if (owner->actor == (actorA = msg->swap.actorA)) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
        } else if (owner->actor == msg->swap.actorB) {
            owner->actor = actorA;
            owner->uniqueId = msg->swap.idA;
        }
    } else if (msgId == 0) {
        struct127 *actor = msg->ref.actor;

        if (actor == owner->actor || msg->ref.uniqueId == owner->uniqueId) {
            func_1516972C((struct102 *)particle);
        }
    }
}

/* Message handler of a burst spark: passes the message on with the spark's owner reference. */
void func_151BE604(BurstSpark *spark, s32 msg, u8 msgId) {
    func_15169850(msg, msgId, (s32)&spark->data.owner, (s32)&spark->data.ownerId, (s32)spark);
}

/* Clears the spark's slot in its burst effect. */
void func_151BE644(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->sparks[data->index] = NULL;
    }
}

void func_151BE674(BurstSpark *spark) {
    func_151BE644(spark);
    func_1513CA6C((struct210 *)spark);
}

void func_151BE6A0(BurstSpark *spark) {
    func_151BE644(spark);
    func_1513CAA0((struct210 *)spark);
}

/* Glow textures a burst glow picks from at random (0x60..0x63). */
typedef struct {
    s32 textures[4];
} GlowTextures;

extern GlowTextures D_800AA7A8;

/* Per-frame update of a burst glow: switches it to a random texture every 3-10 frames and eases its scale. */
s32 func_151BE6CC(BurstGlow *glow) {
    s32 pad; // matching: unused stack slot
    GlowTextures choices;
    BurstGlowShape *shape = &glow->shape;

    choices = D_800AA7A8;
    glow->data.textureTimer -= D_800BE9E4;
    if (glow->data.textureTimer < 0) {
        glow->particle.texture = choices.textures[func_150ADA20() & 3];
        glow->data.textureTimer = (func_150ADA20() & 7) + 3;
    }
    shape->scale += (glow->data.targetScale - shape->scale) * 0.65000004f;
    return 1;
}

/* Message handler of a burst glow: passes the message on with the glow's owner reference. */
void func_151BE788(BurstGlow *glow, s32 msg, u8 msgId) {
    func_15169850(msg, msgId, (s32)&glow->data.owner, (s32)&glow->data.ownerId, (s32)glow);
}

/* Clears the glow's slot in its burst effect. */
void func_151BE7C8(BurstGlow *glow) {
    BurstGlowData *data = &glow->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->glows[data->index] = NULL;
    }
}

void func_151BE7F8(BurstGlow *glow) {
    func_151BE7C8(glow);
    func_151411A4((struct210 *)glow);
}

void func_151BE824(BurstGlow *glow) {
    func_151BE7C8(glow);
    func_151411C4((struct210 *)glow);
}
