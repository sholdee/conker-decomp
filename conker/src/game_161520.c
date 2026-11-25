#include <ultra64.h>
/* This file needs other prototypes for these two than functions.h declares. */
#define func_151D9B8C func_151D9B8C_s32proto
#include "functions.h"
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, Vec3f *, s16, u8, u8, u8, u8, s32);
#include "variables.h"
#include "effects.h"

extern void func_15153F18(void *, void *, s32, u8, s32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
/* Transforms the point `in` by a 4x4 matrix into `out`. */
extern void func_15143134(void *in, void *out, void *matrix);
extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
extern s32 func_15146078(Vec3f *, Vec3f *, Vec3f *);
extern void func_151BC5A4(struct260 *, s32, u8);

/* Per-model blood settings, indexed by func_15134070's profile index. */
typedef struct {
    Vec3f offset;           /* where blood comes from, relative to the actor's bone 12 */
    u16 unkC;
    u8 unkE;
    u8 padF;
} BloodProfile;

extern BloodProfile D_800A3FD8[20];
/* Entries of D_800A3FD8 that the code singles out (compared by address). */
extern BloodProfile D_800A3FF8; /* [2] */
extern BloodProfile D_800A4018; /* [4] */
extern BloodProfile D_800A4028; /* [5] */
extern BloodProfile D_800A4038; /* [6] */
extern BloodProfile D_800A4058; /* [8] */
extern BloodProfile D_800A4068; /* [9] */
extern BloodProfile D_800A40C8; /* [15] */
extern BloodProfile D_800A40F8; /* [18] */

typedef struct {
    s32 unk0[4];
} Struct15136698Table;

extern Struct15136698Table D_80089BAC;

/* Actor model ids (Actor::id) that have a blood profile. */
enum {
    MODEL_CONKER = 0,
    MODEL_CONKER_VARIANT_1 = 1,
    MODEL_CONKER_VARIANT_2 = 2,
    MODEL_CONKER_VARIANT_3 = 3,
    MODEL_CONKER_VARIANT_4 = 4,
    MODEL_ROCKMAN = 16,
    MODEL_WEASEL_GUARD_TALL_VARIANT = 17,
    MODEL_WEASEL_GUARD_SHORT_SHIELD_VARIANT = 20,
    MODEL_UGA_BUGA_BLUE_HEADGEAR = 22,
    MODEL_TNT_IMP = 52,
    MODEL_ROCKMAN_BOW_TIE = 56,
    MODEL_WISE_GUY = 59,
    MODEL_ROBO_SPIDER = 71,
    MODEL_SHC_SOLDIER = 88,
    MODEL_TEDIZ = 90,
    MODEL_RODENT = 91,
    MODEL_TEDIZ_AMMUNITION_BELT = 95,
    MODEL_GREGG_THE_GRIM_REAPER_SCYTHE = 112,
    MODEL_TEDIZ_VARIANT_1 = 116,
    MODEL_TEDIZ_VARIANT_2 = 117,
    MODEL_TEDIZ_VARIANT_3 = 122,
    MODEL_SHC_SOLDIER_VARIANT = 128,
    MODEL_SHC_SOLDIER_DECORATED_UNIFORM = 135,
    MODEL_UGA_BUGA = 136,
    MODEL_SURF_PUNK_SUNGLASSES = 144,
    MODEL_ROCKWOMAN = 145,
    MODEL_CONKER_BLACK_OUTFIT = 150,
    MODEL_WEASEL_BLACK_HELMET_AND_UNIFORM = 152,
    MODEL_UNKNOWN_154 = 154,
    MODEL_VILLAGER_BROWN_HAT = 156,
    MODEL_VILLAGER_STRIPED_BONNET = 157,
    MODEL_ZOMBIE_DARK_SUIT = 159,
    MODEL_ZOMBIE_PURPLE_DRESS = 160,
    MODEL_UNKNOWN_171 = 171,
    MODEL_SHC_SOLDIER_DECORATED_UNIFORM_VARIANT = 176,
    MODEL_TEDIZ_BROAD_SHOULDERED_VARIANT = 177,
    MODEL_GREGG_THE_GRIM_REAPER_HOODED = 178,
    MODEL_GREGG_THE_GRIM_REAPER_WITHOUT_ROBE = 180
};

#define BLOOD_PROFILE_NONE 99

/* An actor's bone matrix (unk1D4 holds 0x40-byte matrices). */
#define BONE_MATRIX(actor, bone) ((u8 *)(actor)->unk1D4 + (bone) * 0x40)

/* Returns the blood profile index (0-19) for an actor's model, or BLOOD_PROFILE_NONE. */
s32 func_15134070(Actor *actor) {
    switch (actor->id) {
    case MODEL_SHC_SOLDIER_DECORATED_UNIFORM:
    case MODEL_SHC_SOLDIER_DECORATED_UNIFORM_VARIANT:
        return 14;
    case MODEL_TEDIZ_BROAD_SHOULDERED_VARIANT:
        return 15;
    case MODEL_GREGG_THE_GRIM_REAPER_WITHOUT_ROBE:
        return 16;
    case MODEL_GREGG_THE_GRIM_REAPER_SCYTHE:
    case MODEL_GREGG_THE_GRIM_REAPER_HOODED:
        return 17;
    case MODEL_UNKNOWN_171:
        return 18;
    case MODEL_ROCKMAN:
    case MODEL_ROCKMAN_BOW_TIE:
    case MODEL_ROCKWOMAN:
        return 13;
    case MODEL_WEASEL_BLACK_HELMET_AND_UNIFORM:
        return 12;
    case MODEL_TNT_IMP:
        return 11;
    case MODEL_ROBO_SPIDER:
        return 10;
    case MODEL_WEASEL_GUARD_TALL_VARIANT:
    case MODEL_WEASEL_GUARD_SHORT_SHIELD_VARIANT:
    case MODEL_WISE_GUY:
        return 1;
    case MODEL_TEDIZ:
    case MODEL_TEDIZ_AMMUNITION_BELT:
    case MODEL_TEDIZ_VARIANT_1:
    case MODEL_TEDIZ_VARIANT_2:
    case MODEL_TEDIZ_VARIANT_3:
        return 2;
    case MODEL_SHC_SOLDIER:
    case MODEL_RODENT:
    case MODEL_SHC_SOLDIER_VARIANT:
        return 3;
    case MODEL_UGA_BUGA_BLUE_HEADGEAR:
    case MODEL_UGA_BUGA:
    case MODEL_SURF_PUNK_SUNGLASSES:
        return 4;
    case MODEL_VILLAGER_BROWN_HAT:
        return 5;
    case MODEL_VILLAGER_STRIPED_BONNET:
        return 6;
    case MODEL_CONKER_BLACK_OUTFIT:
        return 7;
    case MODEL_ZOMBIE_PURPLE_DRESS:
        return 8;
    case MODEL_ZOMBIE_DARK_SUIT:
        return 9;
    case MODEL_UNKNOWN_154:
        return 19;
    case MODEL_CONKER:
    case MODEL_CONKER_VARIANT_1:
    case MODEL_CONKER_VARIANT_2:
    case MODEL_CONKER_VARIANT_3:
    case MODEL_CONKER_VARIANT_4:
        return 0;
    default:
        return BLOOD_PROFILE_NONE;
    }
}

s32 func_1513416C(struct102 *effect) {
    s16 value = effect->unk1C;

    if (value < 32) {
        effect->unk28 = value * 8;
    }
    return 1;
}


/* Blood dripping from a point, optionally attached to an actor or one of its bones. */
typedef struct {
    u8 pad0[0x10];
    s32 target[2];          /* matrices to transform offset by, double-buffered */
    u8 ownerId;             /* owner->unique_id when attached */
    u8 pad19[3];
    Actor *owner;
    u8 bone;
    u8 pad21[3];
    f32 offset[3];
    f32 radius;
    f32 rate;
    s16 lifetime;
    u8 flags;
    u8 emitter;
    s8 callback;
    u8 pad3D[3];
    Vec3f previous;
    f32 inverseDiameter;
    f32 accumulator;
} BloodDrip;

#define BLOOD_DRIP_FOLLOW_OWNER  0x01
#define BLOOD_DRIP_ON_BONE       0x02
#define BLOOD_DRIP_LIFETIME      0x04
#define BLOOD_DRIP_NO_PREVIOUS   0x08
#define BLOOD_DRIP_STATIC        0x10

/* Spawns a BloodDrip effect from a template. */
BloodDrip *func_1513418C(void *desc, s32 arg1, u8 arg2, s32 arg3) {
    BloodDrip *drip;
    Actor *owner;
    u8 flags;

    drip = func_15167A68(0x28, arg3, arg1 + 0x58, 1, arg2, 1);
    if (drip == NULL) {
        return NULL;
    }
    memcpy(&drip->target, desc, 0x30);
    flags = drip->flags;
    if (flags & BLOOD_DRIP_ON_BONE) {
        owner = drip->owner;
        if ((owner->interaction_state == 0) || (owner->unique_id != drip->ownerId)) {
            func_1516972C((struct102 *)drip);
            return NULL;
        }
        if ((owner->unk1D4 != 0) && ((owner->unk74 & 0xF) != 0xF)) {
            func_15143134(drip->offset, &drip->previous, BONE_MATRIX(owner, drip->bone));
        } else {
            drip->flags = flags | BLOOD_DRIP_NO_PREVIOUS;
        }
    } else {
        drip->flags = flags | (BLOOD_DRIP_NO_PREVIOUS | BLOOD_DRIP_STATIC);
    }
    drip->inverseDiameter = 1.0f / (drip->radius + drip->radius);
    drip->accumulator = 0.0f;
    return drip;
}


extern void (*D_80089A70[])(f32, f32, f32, f32, f32, f32, void *);
extern s32 (*D_80089AA0[])(void *);

/* Per-frame update: follows the drip source and emits drops along the path it moved. */
void func_151342BC(BloodDrip *drip) {
    f32 *source;
    u8 dead;
    u8 flags;
    f32 position[3];
    s32 target;
    u8 *bones;
    f32 deltaZ;
    f32 deltaY;
    f32 deltaX;
    f32 randomX;
    f32 randomY;
    s32 index;
    f32 scale;
    f32 distance;

    dead = 0;
    if (drip->flags & BLOOD_DRIP_LIFETIME) {
        drip->lifetime -= D_800BE9E4;
        if (drip->lifetime < 0) {
            dead = 1;
        }
    }
    flags = drip->flags;
    source = (flags & BLOOD_DRIP_NO_PREVIOUS) ? position : &drip->previous.x;
    if (flags & BLOOD_DRIP_FOLLOW_OWNER) {
        position[0] = drip->owner->x_position;
        position[1] = drip->owner->y_position;
        position[2] = drip->owner->z_position;
    } else {
        if (flags & BLOOD_DRIP_ON_BONE) {
            if ((drip->owner->interaction_state == 0) || (drip->owner->interaction_state == 8) ||
                (drip->owner->unique_id != drip->ownerId)) {
                func_1516972C((struct102 *)drip);
                return;
            }
            bones = (u8 *)drip->owner->unk1D4;
            if ((bones == NULL) || ((drip->owner->unk74 & 0xF) == 0xF)) {
                drip->flags = flags | BLOOD_DRIP_NO_PREVIOUS;
                return;
            }
            target = (s32)((u8 (*)[0x40])bones)[drip->bone];
        } else if (!(flags & BLOOD_DRIP_STATIC)) {
            index = D_800BE9C0 ? 0 : 1;
            target = drip->target[index];
        } else {
            return;
        }
        func_15143134(drip->offset, position, (void *)target);
    }
    deltaX = source[0] - position[0];
    deltaY = source[1] - position[1];
    deltaZ = source[2] - position[2];
    distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    scale = drip->inverseDiameter * distance + 1.0f;
    drip->accumulator += scale * D_800BE9A4 * drip->rate;
    if (drip->accumulator > 7.0f) {
        drip->accumulator = 7.0f;
    }
    if ((drip->flags & 0x20) && (D_800C35EA == 1)) {
        while (drip->accumulator > 1.0f) {
            drip->accumulator -= 1.0f;
        }
    } else {
        while (drip->accumulator > 1.0f) {
            f32 diameter;
            f32 fraction;

            diameter = drip->radius + drip->radius;
            fraction = func_150ADA68();
            if (D_80089A70[drip->emitter] != NULL) {
                randomX = func_150ADA68();
                randomY = func_150ADA68();
                D_80089A70[drip->emitter](
                    (position[0] + deltaX * fraction + drip->radius) - randomX * diameter,
                    (position[1] + deltaY * fraction + drip->radius) - randomY * diameter,
                    (position[2] + deltaZ * fraction + drip->radius) - func_150ADA68() * diameter,
                    deltaX, deltaY, deltaZ, drip);
            }
            drip->accumulator -= 1.0f;
        }
    }
    drip->previous = *(Vec3f *)position;
    drip->flags &= ~BLOOD_DRIP_NO_PREVIOUS;
    if (!dead) {
        if ((drip->callback != -1) && (D_80089AA0[drip->callback](drip) == 0)) {
            dead = 1;
        }
    }
    if (dead) {
        func_1516972C((struct102 *)drip);
    }
}

s32 func_151346D0(s32 arg0, struct102 *effect, s32 arg2) {
    effect->unk3A &= ~0x10;
    return arg0;
}

void func_151346EC(struct102 *effect) {
    func_15169804(effect);
}

void func_1513470C(struct102 *effect) {
    func_15169824(effect);
}

/* Calls the D_80089AAC handler selected by effect->unk3D (0 when out of range). */
void func_1513472C(struct102 *effect) {
    s32 index = effect->unk3D;

    if (index < 0) {
        index = 0;
    }
    if (index >= 10) {
        index = 0;
    }
    D_80089AAC[index]();
}

/* Calls the D_80089AD4 handler selected by effect->unk3D (0 when out of range). */
void func_1513477C(struct102 *effect) {
    s32 index = effect->unk3D;

    if (index < 0) {
        index = 0;
    }
    if (index >= 10) {
        index = 0;
    }
    D_80089AD4[index]();
}

typedef struct {
    u8 pad0[0x18];
    u8 ownerId;
    u8 pad19[0x3];
    Actor *owner;
    u8 pad20[0x1D];
    u8 unk3D;
} SubA_151347CC;

/*
 * Actor message payload. Messages 0, 3 and 0x11 carry an actor and (in the first byte of
 * the second word) its unique id; message 0x2D swaps actorA and actorB.
 */
typedef struct {
    Actor *actorA;
    union {
        Actor *actorB;
        u8 actorAId;
    } u4;
    u8 swapIdA;
    u8 swapIdB;
} BloodActorMsg;

/* Message handler: frees the effect when its owner goes away, follows owner handle swaps. */
void func_151347CC(void *arg0, BloodActorMsg *msg, u8 message) {
    SubA_151347CC *effect = arg0; // matching: copying the parameter keeps IDO's register choice in case 0x2D
    Actor *actorA;

    if ((message == 0) || (message == 3)) {
        if ((msg->actorA == effect->owner) || (msg->u4.actorAId == effect->ownerId)) {
            func_1516972C((struct102 *)effect);
        }
    } else if (message == 0x11) {
        if (effect->unk3D == 5) {
            if ((msg->actorA == effect->owner) || (msg->u4.actorAId == effect->ownerId)) {
                func_1516972C((struct102 *)effect);
            }
        }
    } else if (message == 0x16) {
        if ((Actor *)msg == effect->owner) {
            func_1516972C((struct102 *)effect);
        }
    } else if (message == 0x2D) {
        actorA = msg->actorA;
        if (effect->owner == actorA) {
            effect->owner = msg->u4.actorB;
            effect->ownerId = msg->swapIdB;
        } else if (effect->owner == msg->u4.actorB) {
            effect->owner = actorA;
            effect->ownerId = msg->swapIdA;
        }
    }
}

/* Empty callback. */
void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

/* Spawn parameters of a BloodTrail; copied verbatim into the effect. */
typedef struct {
    f32 *x;
    f32 *y;
    f32 *z;
    f32 radius;
    f32 rate;
    s16 lifetime;
    u8 flags;
    u8 emitter;
    u8 field28;
    s8 callback;
    u8 pad2A[2];
} BloodTrailDesc;

/* Blood trailing behind a moving point given by three coordinate pointers. */
typedef struct {
    u8 pad0[0x10];
    f32 *x;
    f32 *y;
    f32 *z;
    f32 radius;
    f32 rate;
    s16 lifetime;
    u8 flags;
    u8 emitter;
    u8 field28;
    s8 callback;
    u8 pad2A[2];
    Vec3f previous;
    f32 inverseDiameter;
    f32 accumulator;
} BloodTrail;

#define BLOOD_TRAIL_LIFETIME   0x01
#define BLOOD_TRAIL_SKIP_FRAME 0x02 /* don't emit on the next update */
#define BLOOD_TRAIL_CALLBACK   0x04

/* Spawns a BloodTrail effect from desc, starting at the current position. */
BloodTrail *func_15134908(BloodTrailDesc *desc, s32 arg1, u8 arg2, s32 arg3) {
    BloodTrail *trail;

    trail = func_15167A68(0x2A, arg3, arg1 + 0x40, 1, arg2, 1);
    if (trail == NULL) {
        return NULL;
    }
    desc->flags |= BLOOD_TRAIL_SKIP_FRAME;
    memcpy(&trail->x, desc, sizeof(BloodTrailDesc));
    trail->previous.x = *trail->x;
    trail->previous.y = *trail->y;
    trail->previous.z = *trail->z;
    trail->inverseDiameter = 1.0f / (trail->radius + trail->radius);
    trail->accumulator = 0.0f;
    return trail;
}

extern s32 (*D_80089B18[])(void *);
extern void (*D_80089AFC[])(f32, f32, f32, f32, f32, f32, void *);

/* Per-frame update: emits drops along the path the trail point moved since last frame. */
void func_151349D0(BloodTrail *trail) {
    u8 flags;
    f32 position[3];
    f32 deltaZ;
    f32 deltaY;
    f32 deltaX;
    f32 randomX;
    f32 randomY;

    position[0] = *trail->x;
    position[1] = *trail->y;
    position[2] = *trail->z;
    if (trail->flags & BLOOD_TRAIL_CALLBACK) {
        if ((trail->callback != -1) && (D_80089B18[trail->callback](trail) == 0)) {
            func_1516972C((struct102 *)trail);
            return;
        }
    }
    flags = trail->flags;
    if (flags & BLOOD_TRAIL_SKIP_FRAME) {
        trail->flags = flags & ~BLOOD_TRAIL_SKIP_FRAME;
    } else {
        f32 scale;
        f32 distance;

        deltaX = trail->previous.x - position[0];
        deltaY = trail->previous.y - position[1];
        deltaZ = trail->previous.z - position[2];
        distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
        scale = trail->inverseDiameter * distance + 1.0f;
        trail->accumulator += scale * D_800BE9A4 * trail->rate;
        if (trail->accumulator > 7.0f) {
            trail->accumulator = 7.0f;
        }
        while (trail->accumulator > 1.0f) {
            f32 diameter;
            f32 fraction;

            diameter = trail->radius + trail->radius;
            fraction = func_150ADA68();
            if (D_80089AFC[trail->emitter] != NULL) {
                randomX = func_150ADA68();
                randomY = func_150ADA68();
                D_80089AFC[trail->emitter](
                    (position[0] + deltaX * fraction + trail->radius) - randomX * diameter,
                    (position[1] + deltaY * fraction + trail->radius) - randomY * diameter,
                    (position[2] + deltaZ * fraction + trail->radius) - func_150ADA68() * diameter,
                    deltaX, deltaY, deltaZ, trail);
            }
            trail->accumulator -= 1.0f;
        }
    }
    trail->previous = *(Vec3f *)position;
    if (trail->flags & BLOOD_TRAIL_LIFETIME) {
        trail->lifetime -= D_800BE9E4;
        if (trail->lifetime < 0) {
            func_1516972C((struct102 *)trail);
        }
    }
}

/* Message handler: forwards to func_151BC5A4 while the effect's unk28 is 1. */
void func_15134C98(struct260 *arg0, s32 arg1, u8 arg2) {
    if (((struct102 *)arg0)->unk28 == 1) {
        func_151BC5A4(arg0, arg1, arg2);
    }
}

/* Empty callback. */
void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0xE];
    u8 unk2E;
    u8 pad2F[0x41];
    f32 unk70;
    f32 unk74;
} Struct15134CEC;

/* Particle callback: accelerates and moves the particle and fades unk2E; returns 0 to kill it. */
s32 func_15134CEC(Struct15134CEC *particle) {
    s32 fade;

    particle->unk70 += 0.125f * D_800BE9A4;
    fade = particle->unk2E;
    particle->unk74 += 0.07999999821f * D_800BE9A4;
    particle->unk14 += particle->unk70 * D_800BE9A4;
    particle->unk1C += particle->unk74 * D_800BE9A4;
    if (particle->unk14 > 130.0f) {
        return 0;
    }
    fade -= D_800BE9E4 * 2;
    if (fade < 0) {
        return 0;
    }
    particle->unk2E = fade;
    return 1;
}

/* Spawn parameters of a BloodStrip (copied to BloodStrip::ownerId). */
typedef struct {
    u8 ownerId;
    u8 pad1[3];
    Actor *owner;
    u8 bone;
    u8 pad9[3];
    Vec3f offsetA;
    Vec3f offsetB;
    u8 flags;
    u8 pad25;
    s16 duration;
    s16 pause;
    s16 lifetime;
    u16 sound;
    s8 sampleCallback;
    s8 emitCallback;
    s8 rateCallback;
    u8 pad31[3];
    f32 rate;
    u8 mode;
    s8 updateCallback;
    u8 pad3A[2];
} BloodStripDesc;

/*
 * Blood swept between two points (offsetA, offsetB) on an actor's bone: every frame the
 * points are transformed and the emit callback runs at steps between the old and new spots.
 */
typedef struct {
    u8 pad0[0x10];
    s32 active;
    s32 unk14;
    u8 ownerId;
    u8 pad19[3];
    Actor *owner;
    u8 bone;
    u8 pad21[3];
    Vec3f offsetA;
    Vec3f offsetB;
    u8 flags;
    u8 pad3D;
    s16 duration;           /* frames of emission after each sample */
    s16 pause;              /* frames to wait (below 0) before the next sample */
    s16 lifetime;
    u16 sound;              /* playing sound, stopped when the strip is freed */
    s8 sampleCallback;
    s8 emitCallback;
    s8 rateCallback;
    u8 pad49[3];
    f32 rate;
    u8 mode;                /* selects the message handler */
    s8 updateCallback;
    u8 pad52[2];
    s16 timer;
    u8 pad56[2];
    Vec3f posA;
    Vec3f posB;
    f32 accumulator;
    f32 power;              /* set by the rate callback */
    f32 time;
} BloodStrip;

#define BLOOD_STRIP_LIFETIME 0x02

/* Spawns a BloodStrip effect from desc. */
BloodStrip *func_15134DAC(BloodStripDesc *desc, s32 arg1) {
    BloodStrip *strip;

    strip = func_15167A68(0x29, 0, arg1 + 0x80, 1, 0xFF, 1);
    if (strip == NULL) {
        return NULL;
    }
    memcpy(&strip->ownerId, desc, sizeof(BloodStripDesc));
    strip->timer = -desc->pause;
    strip->active = 1;
    strip->unk14 = 0;
    strip->accumulator = 0.0f;
    strip->power = 0.0f;
    strip->time = 0.0f;
    return strip;
}

extern s32 (*D_80089B1C[])(BloodStrip *);
extern void (*D_80089B38[])(Vec3f *, Vec3f *, Vec3f *, Vec3f *, f32, BloodStrip *);
extern f32 (*D_80089B5C[])(BloodStrip *);
extern s32 (*D_80089B6C[])(BloodStrip *);
void func_15135424(void *matrix, Vec3f *inA, Vec3f *inB, Vec3f *outA, Vec3f *outB);

/* Per-frame update of a BloodStrip. */
void func_15134E48(BloodStrip *arg0) {
    BloodStrip *strip = arg0; // matching: the parameter copy reserves the stack slot IDO kept
    u8 kill;
    u8 *boneMatrix;

    kill = 0;
    if (strip->owner->interaction_state == 0 || strip->owner->unique_id != strip->ownerId) {
        kill = 1;
    }
    if (strip->active & 1) {
        if (!kill && strip->owner->unk1D4 != 0 && (strip->owner->unk74 & 0xF) != 0xF) {
            boneMatrix = BONE_MATRIX(strip->owner, strip->bone);
            strip->timer -= D_800BE9E4;
            if (strip->timer < -strip->pause) {
                strip->timer = strip->duration;
                func_15135424(boneMatrix, &strip->offsetA, &strip->offsetB, &strip->posA, &strip->posB);
                if (strip->sampleCallback != -1 && D_80089B1C[strip->sampleCallback](strip) == 0) {
                    kill = 1;
                    strip->timer = -1;
                }
            }
            if (strip->timer > 0) {
                if (strip->rateCallback != -1) {
                    strip->power = D_80089B5C[strip->rateCallback](strip);
                }
                strip->accumulator += strip->rate * D_800BE9A4;
                if (strip->accumulator > 1.0f) {
                    Vec3f newA;
                    Vec3f newB;
                    Vec3f velocityA;
                    Vec3f velocityB;
                    f32 invCount;
                    f32 time;
                    f32 timeStep;
                    Vec3f pointA;
                    Vec3f pointB;
                    f32 deltaAZ;
                    f32 deltaAY;
                    f32 deltaAX;
                    Vec3f deltaB;
                    f32 stepAX;
                    f32 stepAY;
                    f32 stepAZ;
                    f32 stepBX;
                    f32 stepBY;
                    f32 stepBZ;
                    invCount = 1.0f / strip->accumulator;
                    time = strip->time + D_800BE9A4;
                    timeStep = time * invCount;
                    pointA = strip->posA;
                    pointB = strip->posB;
                    func_15135424(boneMatrix, &strip->offsetA, &strip->offsetB, &newA, &newB);
                    deltaAX = newA.x - strip->posA.x;
                    deltaAY = newA.y - strip->posA.y;
                    deltaAZ = newA.z - strip->posA.z;
                    deltaB.x = newB.x - strip->posB.x;
                    deltaB.y = newB.y - strip->posB.y;
                    deltaB.z = newB.z - strip->posB.z;
                    stepAX = deltaAX * invCount;
                    stepAY = deltaAY * invCount;
                    stepAZ = deltaAZ * invCount;
                    stepBX = deltaB.x * invCount;
                    stepBY = deltaB.y * invCount;
                    stepBZ = deltaB.z * invCount;
                    velocityA.x = deltaAX * D_800BE9A8;
                    velocityA.y = deltaAY * D_800BE9A8;
                    velocityA.z = deltaAZ * D_800BE9A8;
                    velocityB.x = deltaB.x * D_800BE9A8;
                    velocityB.y = deltaB.y * D_800BE9A8;
                    velocityB.z = deltaB.z * D_800BE9A8;
                    do {
                        if (strip->emitCallback != -1) {
                            D_80089B38[strip->emitCallback](&pointA, &pointB, &velocityA, &velocityB, time, strip);
                        }
                        pointA.x += stepAX;
                        pointA.y += stepAY;
                        pointA.z += stepAZ;
                        pointB.x += stepBX;
                        pointB.y += stepBY;
                        pointB.z += stepBZ;
                        time -= timeStep;
                        strip->accumulator -= 1.0f;
                    } while (strip->accumulator > 1.0f);
                    strip->posA = pointA;
                    strip->posB = pointB;
                    strip->time = time;
                }
            }
        }
        if (strip->updateCallback != -1 && D_80089B6C[strip->updateCallback](strip) == 0) {
            kill = 1;
        }
        if (strip->flags & BLOOD_STRIP_LIFETIME) {
            strip->lifetime -= D_800BE9E4;
            if (strip->lifetime < 0) {
                kill = 1;
            }
        }
    }
    if (kill) {
        func_1516972C((struct102 *)strip);
    }
}

void func_151352EC(struct102 *effect) {
    func_15169804(effect);
}

void func_1513530C(struct102 *effect) {
    func_15169824(effect);
}

extern void (*D_80089B70[])(BloodStrip *);

/* Stops the strip's sound, then calls the D_80089B70 handler for its mode. */
void func_1513532C(BloodStrip *arg0) {
    BloodStrip *strip = arg0; // matching: the parameter copy keeps strip in a saved register
    s32 mode;

    mode = strip->mode;
    if (mode < 0) {
        mode = 0;
    } else if (mode >= 6) {
        mode = 0;
    }
    if (strip->sound != 0) {
        func_100111C8(strip->sound);
        strip->sound = 0;
    }
    D_80089B70[mode](strip);
}

extern void (*D_80089B88[])(BloodStrip *);

/* Stops the strip's sound, then calls the D_80089B88 handler for its mode. */
void func_151353A8(BloodStrip *arg0) {
    BloodStrip *strip = arg0; // matching: the parameter copy keeps strip in a saved register
    s32 mode;

    mode = strip->mode;
    if (mode < 0) {
        mode = 0;
    } else if (mode >= 6) {
        mode = 0;
    }
    if (strip->sound != 0) {
        func_100111C8(strip->sound);
        strip->sound = 0;
    }
    D_80089B88[mode](strip);
}

extern s32 func_15145EA4(Vec3f **in, Vec3f **out, void *matrix, s32 count);

/* Transforms inA and inB by matrix into outA and outB. */
void func_15135424(void *matrix, Vec3f *inA, Vec3f *inB, Vec3f *outA, Vec3f *outB) {
    Vec3f *in[2];
    Vec3f *out[2];

    in[0] = inA;
    in[1] = inB;
    out[0] = outA;
    out[1] = outB;
    func_15145EA4(in, out, matrix, 2);
}

void func_1513555C(BloodStrip *strip, BloodActorMsg *msg, u8 message);
void func_151355B8(BloodStrip *strip, BloodActorMsg *msg, u8 message);

/* BloodStrip message handler: follows owner swaps, then dispatches on strip->mode. */
void func_15135480(BloodStrip *strip, BloodActorMsg *msg, u8 message) {
    s32 actorA;
    s32 owner;

    if (message == 0x2D) {
        actorA = (s32)msg->actorA;
        owner = (s32)strip->owner;
        if (actorA == owner) {
            strip->owner = msg->u4.actorB;
            strip->ownerId = msg->swapIdB;
        } else if ((owner | 0) == (s32)msg->u4.actorB) { // matching: `| 0` makes IDO copy owner, fixing operand order
            strip->owner = (Actor *)actorA;
            strip->ownerId = msg->swapIdA;
        }
    }
    switch (strip->mode) {
    case 1:
        func_151355B8(strip, msg, message);
        break;
    case 2:
        func_1513555C(strip, msg, message);
        break;
    default:
        if (message == 0) {
            if ((s32)msg->actorA == ((owner = (s32)strip->owner) | 0) || strip->ownerId == msg->u4.actorAId) { // matching: as above
                func_1516972C((struct102 *)strip);
            }
        }
        break;
    }
}

/* Mode 2 handler: frees the strip when its owner is removed (messages 0 and 0x12). */
void func_1513555C(BloodStrip *strip, BloodActorMsg *msg, u8 message) {
    if (message == 0 || message == 0x12) {
        if (strip->owner == msg->actorA || strip->ownerId == msg->u4.actorAId) {
            func_1516972C((struct102 *)strip);
        }
    }
}

/* Mode 1 handler: frees the strip on message 0, stops it on message 3, for its owner. */
void func_151355B8(BloodStrip *strip, BloodActorMsg *msg, u8 message) {
    switch (message) {
    case 0:
        if (strip->owner == msg->actorA || strip->ownerId == msg->u4.actorAId) {
            func_1516972C((struct102 *)strip);
        }
        break;
    case 3:
        if (strip->owner == msg->actorA || strip->ownerId == msg->u4.actorAId) {
            s32 *active = &strip->active;

            *active &= ~1;
            *active = *active; // matching: IDO emits the extra reload/store of this self-assignment
        }
        break;
    }
}

/* BloodStrip sample callback 0: full power. */
s32 func_15135658(BloodStrip *strip) {
    strip->power = 1.0f;
    return 1;
}

/* BloodStrip rate callback 0: the tunable "power" (../Effects/Blood/blood.c), in thousandths. */
f32 func_15135670(BloodStrip *strip) {
    return func_151422DC(0, &D_800A3FB4, 0, 2000, 1000, &D_800A3FBC, 2938) * 0.001f;
}

void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
typedef struct {
    f32 x, y;
} Vec2f;

typedef struct {
    Vec2f position;
    f32 width, height;
    u8 texture;
    u8 pad11;
    s16 lifetime;
    u16 flags;
    s16 duration, period;
    u8 mode, red, green, blue, alpha, maximum;
    u8 endRed, endGreen, endBlue, endAlpha;
    s32 zero24, zero28, zero2C;
    s32 field30, field34, field38, field3C;
    u8 field40, field41;
    u8 pad42[0x16];
} Blood356D4Particle;

/* Motion state of a blood splat particle, kept at BloodSplat::motion. */
typedef struct {
    s16 timer;              /* frames of growth before the splat starts to fall */
    f32 growth;
    Vec2f velocity;
    f32 gravity;
    f32 spread;             /* growth per unit of horizontal speed while falling */
} Blood356D4Motion;

typedef struct {
    u8 value[2];
} Blood356D4Kinds;

/* A 2D blood splat particle spawned by func_151356D4. */
typedef struct {
    u8 pad0[0x10];
    Vec2f position;
    f32 width;
    f32 height;
    u8 pad20[0x50];
    Blood356D4Motion motion;
} BloodSplat;

extern Blood356D4Kinds D_80089BA0;
extern s32 D_80089BA4;
extern Blood356D4Kinds D_80089BA8;

/* Spawns a blood splat particle at *position plus a ring of smaller spark particles around it. */
void *func_151356D4(Vec2f *position, f32 size, f32 gravity, f32 spread, u8 alpha, u8 endAlpha, s16 delay, s16 lifetime, s16 duration, s16 period, u8 arg10, s32 arg11) {
    void *result;
    Blood356D4Particle particle;
    Blood356D4Motion motion;
    Blood356D4Kinds kinds;
    s32 data;
    f32 scale;
    f32 count;
    Blood356D4Particle spark;
    Blood356D4Kinds sparkKinds;
    s32 flip;

    kinds = D_80089BA0;
    data = D_80089BA4;
    motion.timer = delay;
    motion.growth = (func_150ADA68() * 100.0f + 100.0f) * size * 0.001f;
    motion.velocity.x = func_150ADA68() * 6.0f * position->x * 0.001f;
    motion.velocity.y = func_150ADA68() * 6.0f * position->y * 0.001f;
    motion.gravity = gravity;
    motion.spread = spread;
    particle.position = *position;
    particle.width = size;
    particle.height = size;
    particle.texture = kinds.value[func_150ADA20() & 1];
    particle.lifetime = lifetime;
    flip = (func_150ADA20() & 1) ? 2 : 0;
    particle.flags = ((particle.texture == 58) ? ((func_150ADA20() & 1) ? 4 : 0) : 0) | 9 | flip;
    particle.duration = duration;
    particle.period = period;
    particle.mode = 2;
    particle.red = 0xFF;
    particle.green = 0xFF;
    particle.blue = 0xFF;
    particle.maximum = 0xFF;
    particle.endRed = 0xFF;
    particle.endGreen = 0xFF;
    particle.endBlue = 0xFF;
    particle.zero24 = 0;
    particle.zero28 = 0;
    particle.zero2C = 0;
    particle.field40 = 0;
    particle.field41 = 9;
    particle.field30 = 7;
    particle.field34 = 60;
    particle.field38 = 128;
    particle.field3C = 32;
    particle.alpha = alpha;
    particle.endAlpha = endAlpha;
    result = func_1515548C(&particle, 3, (s32)&data, 1, sizeof(motion), arg10, arg11);
    if (result != NULL) {
        memcpy(&((BloodSplat *)result)->motion, &motion, sizeof(motion));
        scale = (particle.width + particle.height) * 0.5f;
        count = (func_150ADA68() * 300.0f + 100.0f) * size * 0.001f;
        spark.lifetime = 50;
        spark.flags = 9;
        spark.duration = 16;
        spark.period = 15;
        spark.mode = 3;
        spark.red = 0xFF;
        spark.green = 0xFF;
        spark.blue = 0xFF;
        spark.alpha = alpha;
        spark.maximum = 0xFF;
        spark.endRed = 0xFF;
        spark.endGreen = 0xFF;
        spark.endBlue = 0xFF;
        spark.endAlpha = endAlpha;
        spark.zero24 = 0;
        spark.zero28 = 0;
        spark.zero2C = 0;
        spark.field40 = 0;
        spark.field41 = 9;
        spark.field30 = 7;
        spark.field34 = 60;
        spark.field38 = 64;
        spark.field3C = 0;
        spark.width = (func_150ADA68() * 120.0f + 80.0f) * (size * scale) * 0.0001f;
        spark.height = (func_150ADA68() * 120.0f + 80.0f) * (size * scale) * 0.0001f;
        while (count > 1.0f) {
            sparkKinds = D_80089BA8;
            {
                s16 angle;
                f32 cosine;
                f32 sine;
                f32 distance;

                angle = func_150ADA20() & 0xFF;
                cosine = func_151423D8(angle - 64);
                sine = func_151423D8(angle);
                distance = (func_150ADA68() * 300.0f + 1300.0f) * scale * 0.001f;
                spark.position.x = position->x + distance * sine;
                spark.position.y = position->y + distance * cosine;
            }
            spark.texture = sparkKinds.value[func_150ADA20() & 1];
            spark.flags &= ~6;
            if (func_150ADA20() & 1) {
                spark.flags |= 2;
            }
            if (func_150ADA20() & 1) {
                spark.flags |= 4;
            }
            func_1515548C(&spark, 0, 0, 0, 0, arg10, arg11);
            count -= 1.0f;
        }
    }
    return result;
}


/* BloodSplat callback: grows the splat, then lets it fall; returns 0 once it leaves the area. */
s32 func_15135BF8(BloodSplat *splat) {
    Blood356D4Motion *motion;
    s32 alive;

    motion = &splat->motion;
    alive = 1;
    if (splat->motion.timer > 0) {
        motion->timer -= D_800BE9E4;
        splat->position.x += motion->velocity.x * D_800BE9A4;
        splat->position.y += motion->velocity.y * D_800BE9A4;
        splat->width += motion->growth * D_800BE9A4;
        splat->height += motion->growth * D_800BE9A4;
    } else {
        Vec2f average;

        average = motion->velocity;
        splat->position.y += motion->velocity.y * D_800BE9A4 + motion->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
        motion->velocity.y += motion->gravity * D_800BE9A4;
        average.x += motion->velocity.x;
        average.y += motion->velocity.y;
        average.x *= 0.5f;
        average.y *= 0.5f;
        {
            f32 growth = fabsf(average.x) * motion->spread;

            splat->width += growth * D_800BE9A4;
            splat->height += growth * D_800BE9A4;
        }
    }
    if (splat->position.x > 200.0f || splat->position.x < -200.0f || splat->position.y > 200.0f ||
        splat->position.y < -200.0f) {
        alive = 0;
    }
    return alive;
}

/* Result of a ground probe (func_15046460 / func_15046C80). */
typedef struct {
    f32 height;
    u8 unk4[0x14];
    s32 unk18;
    u8 flags;
    u8 surfaceType;
    u8 pad1E[2];
    s32 unk20;
} GroundHit;

extern s32 func_15046460(Vec3f *, u16, f32, GroundHit *);
extern void func_151DB5D0(u8, void *, Vec3f *, f32, f32, f32, f32, s32, s32, s32, s32, s32, u8, s32);
extern void func_151DA6F8(void *, Vec3f *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s32, s32, s32, u8, s32);
extern void func_151D9014(void *, void *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
extern void func_15143794(s16, s16, f32, f32 *);

/* Blood spray from `from` towards `to`; splashes droplets instead when the ground below is surface type 3. */
void func_15135DD0(Vec3f *from, Vec3f *to, f32 radius, u8 arg3, s32 arg4) {
    Vec3f direction;
    f32 fromX;
    Vec3f probe;
    GroundHit hit;
    u32 flagsCopy;
    Vec3f velocity;
    s32 count;
    f32 speed, scale;
    Vec3f offset;
    f32 size;
    s32 life;
    Vec3f spread;
    s32 drops;

    flagsCopy = arg3; // matching: IDO keeps this u32 copy of arg3 live in a saved register for the last call
    hit.unk18 = 0;
    hit.flags = 0;
    hit.surfaceType = 0;
    hit.unk20 = 0;
    hit.height = -10000.0f;
    direction.x = to->x - (fromX = from->x);
    direction.y = to->y - from->y;
    direction.z = to->z - from->z;
    probe.x = fromX;
    probe.y = 10000.0f;
    probe.z = from->z;
    if (func_15046460(&probe, 0, from->y, &hit) != 0 && hit.surfaceType == 3) {
        for (drops = func_150ADA20() % 11U + 10; drops > 0; drops--) {
            speed = (func_150ADA68() * 67.0f + 86.0f) * 0.001000000047f;
            velocity.x = direction.x * speed;
            velocity.y = direction.y * speed;
            velocity.z = direction.z * speed;
            life = (s16)(func_150ADA20() % 52U + 99);
            size = func_150ADA68();
            speed = func_150ADA68();
            scale = func_150ADA68();
            func_151DB5D0(0, from, &velocity, size * 50.0f + 30.0f, speed * 0 + 1.00999999f, 1.0f, scale * 0.04100000113f, life, func_150ADA20() % 156U + 100, life, 255 / life, 1, arg3, arg4);
        }
    } else {
        for (count = func_150ADA20() % 5U + 3; count > 0; count--) {
            scale = (func_150ADA68() * 618.0f + 201.0f) * 0.001000000047f;
            func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, radius, &offset.x);
            offset.x += direction.x * scale;
            offset.y += direction.y * scale;
            offset.z += direction.z * scale;
            {
                u32 randA;
                speed = func_150ADA68();
                randA = func_150ADA20();
                life = func_150ADA20();
                scale = func_150ADA68();
                func_151DA6F8(from, &offset, speed * 1.013000011f, randA % 21U + 20, (u32)life % 101U + 100, scale * 4.200000286f + 1.899999976f, func_150ADA20() % 3U + 3, 0, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0, arg3, arg4);
            }
        }
        {
            u32 randB;
            for (count = func_150ADA20() % 11U + 10; count > 0; count--) {
                scale = func_150ADA68() * 200.0f * 0.001000000047f;
                func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, radius, &spread.x);
                spread.x += direction.x * scale;
                spread.y += direction.y * scale;
                spread.z += direction.z * scale;
                {
                    u32 randA;
                    speed = func_150ADA68();
                    randA = func_150ADA20();
                    randB = func_150ADA20();
                    func_151D9014(from, &spread, 0, speed * 0.1469999999f + -0.302000016f, randA % 52U + 31, randB % 101U + 155, func_150ADA68() * 50.0f + 50.0f, 0, 1.0f, 1.0f, 0, 0, 1, 0, flagsCopy, arg4);
                }
            }
        }
    }
}

extern void func_15143874(s16, f32, f32 *, f32 *);
void func_1504715C();
s32 func_15046C80(Vec3f *, u16, f32, GroundHit *);

/* Scatters blood decals on the ground within radius of the actor, more for a larger area. */
void func_15136404(Actor *actor, f32 radius, u8 arg2, s32 arg3) {
    f32 area;
    f32 count;
    GroundHit hit;
    Vec3f probe;
    Vec3f pos;

    if (actor != NULL) {
        area = 3.141592741f * radius * radius;
        count = (func_150ADA68() * 305.0f + 108.0f) * 9.999999975e-07f * area;
        func_1504715C(&hit, actor);
        while (count > 1.0f) {
            func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * radius, &probe.x, &probe.z);
            probe.x += actor->x_position;
            probe.z += actor->z_position;
            probe.y = actor->y_position + 500.0f;
            if (func_15046C80(&probe, 0, actor->y_position - 5000.0f, &hit) != 0 && hit.surfaceType != 3) {
                pos.x = probe.x;
                pos.y = hit.height;
                pos.z = probe.z;
                func_151D9B8C(0, func_150ADA68() * 20.0f + 10.0f, func_150ADA20() % 101U + 100, hit.unk4, (Vec3f *)&pos,
                              func_150ADA20() % 201U + 400, 1, 1, 0, arg2, arg3);
            }
            count -= 1.0f;
        }
    }
}

/* Spawns a blood particle of the given size with one of the four D_80089BAC textures. */
s32 func_15136698(f32 size, f32 arg1, u8 alpha, u8 arg3, s16 lifetime, s32 arg5, Vec3f *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10) {
    s32 particle;
    ParticleDef desc;
    f32 extra;
    Struct15136698Table textures;
    s32 argB;
    s32 argC;
    s32 value;
    s32 random1;
    s32 random0;

    textures = D_80089BAC;
    extra = arg1;
    desc.texture = textures.unk0[func_150ADA20() & 3];
    if (arg7 != 0) {
        value = 2;
    } else {
        value = 1;
    }
    desc.renderModes = value + 0x300;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.rgba[0] = 0;
    desc.rgba[1] = 0;
    desc.rgba[2] = 0;
    desc.rgba[3] = 255;
    desc.height = size;
    desc.width = size;
    desc.pos = *arg6;
    desc.flags = 0;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    if (lifetime == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = lifetime + 32;
        desc.flags = 1;
    }
    desc.unk44 = alpha;
    desc.unk45 = arg3;
    if (arg8 != 0) {
        argB = 3;
        argC = 255;
    } else {
        argB = 0;
        argC = 0;
    }

    random0 = func_150ADA20();
    random1 = func_150ADA20();
    value = func_150ADA20();
    particle = func_1513D594(&desc, 0, 0, 0x1A, 0, (random1 & 1) + (random0 & 1), value & 0xFF, 500.0f, 500.0f, 0, arg5, argB, argC, 0, 4, arg9, arg10);
    if (particle != 0) {
        memcpy(particle + 0x128, &extra, sizeof(extra));
    }
    return particle;
}


/* The particle func_15136698 spawns; its desc is copied to +0x18 and `extra` to growth. */
typedef struct {
    u8 pad0[0x1C];
    s16 timer;
    u8 pad1E[0xE];
    f32 width;
    f32 height;
    u8 pad34[0x28];
    u8 alpha;
    u8 pad5D[0xCB];
    f32 growth;
} GrowingBloodParticle;

/* GrowingBloodParticle callback: fades out over the last 32 frames and grows by `growth`. */
s32 func_151368A8(GrowingBloodParticle *particle) {
    s32 timer = particle->timer;
    f32 *growth = &particle->growth; // matching: IDO forms this address before the fade test

    if (timer < 32) {
        s32 alpha = timer << 3;
        if (alpha < particle->alpha) {
            particle->alpha = alpha;
        }
    }
    particle->width += *growth * D_800BE9A4;
    particle->height += *growth * D_800BE9A4;
    return 1;
}

struct LocalDef15136918 {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    s8  unk7;
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    s8  unk16;
    s8  unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

/* Spawns a func_1513C73C particle (type 0xD) of the given size at position. */
void func_15136918(f32 size, u8 arg1, u8 arg2, s16 lifetime, s16 arg4, s32 arg5, f32 *position, u8 arg7, s32 arg8) {
    struct LocalDef15136918 desc;
    f64 pad; // matching: an unused 8-byte stack slot

    pad = size;
    desc.unk6 = 0x55;
    desc.unk0 = 0x300;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = arg1;
    desc.unk11 = arg2;
    desc.unk12 = 0;
    desc.unk13 = 0;
    desc.unk14 = 0;
    desc.unk15 = 0;
    desc.unk18 = 0x280001;
    desc.unk16 = 1;
    desc.unk17 = 1;
    if (lifetime == -1) {
        desc.unk4 = 300;
    } else {
        desc.unk0 = 0x301;
        desc.unk4 = lifetime + 32;
    }
    func_1513C73C((s32)&desc, 0xD, 0, arg5, position[0], position[1], position[2], size, size, arg4, 0, 0, arg7, arg8);
}

/* Particle callback: fades unk28 out over the last 32 frames. */
s32 func_15136A1C(struct102 *effect) {
    s16 timer = effect->unk1C;
    s32 alpha;

    if (timer < 32) {
        alpha = timer << 3;
        if (alpha < effect->unk28) {
            effect->unk28 = alpha;
        }
    }
    return 1;
}

/* Starts a short blood trail behind the point (*x, *y, *z). */
void func_15136A50(f32 *x, f32 *y, f32 *z, s16 lifetime, s32 arg4, s32 arg5) {
    BloodTrailDesc desc;

    desc.radius = 10.30000019f;
    desc.x = x;
    desc.y = y;
    desc.z = z;
    desc.rate = 0.5008999705f;
    desc.lifetime = lifetime;
    desc.flags = BLOOD_TRAIL_LIFETIME | BLOOD_TRAIL_CALLBACK;
    desc.emitter = 5;
    desc.field28 = 2;
    desc.callback = -1;
    func_15134908(&desc, 0, arg4, arg5);
}

void func_151D9014(void *position, void *velocity, s32 arg2, f32 arg3, s32 arg4, s32 arg5,
                   f32 arg6, s32 arg7, f32 arg8, f32 arg9, s32 argA, s32 argB,
                   s32 argC, s32 argD, s32 argE, s32 argF);

extern f32 D_800A4624; // matching: 0.001f as a literal grows the stack frame by 8

/* Sprays blood from (x, y, z) against the direction (dirX, dirY, dirZ). */
void func_15136AE4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6) {
    f32 position[3];
    f32 velocity[3];
    f32 speed;
    struct {
        f32 size;
        s32 random0;
        u32 random1;
        s32 pad;
    } r; // matching: IDO places these three locals in this order on the stack

    position[0] = x;
    position[1] = y;
    position[2] = z;
    speed = (func_150ADA68() * 112.0f + 247.0f) * D_800A4624;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    r.size = func_150ADA68();
    r.random0 = func_150ADA20();
    r.random1 = func_150ADA20();
    func_151D9014(position, velocity, 0, r.size * 0.2700000107f + -0.3100000024f,
                    (r.random0 & 0xF) + 25, (r.random1 % 101) + 155,
                    func_150ADA68() * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    arg6[0xC], arg6[1]);
}

typedef struct Blood36C3CSpray {
    s32 countBase;
    s32 countRange;
    Vec3f origin;
    s16 angleBase;
    s16 angleRange;
    s16 pitchBase;
    s16 pitchRange;
    f32 maxDistance;
    f32 sizeBase;
    f32 sizeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 kind;
} Blood36C3CSpray;

void func_1515C244(Actor *, Vec3f *, f32 *, f32 *);
void func_1513A24C(Vec3f *, s32 *, f32 *, s32, f32, u8, u8, u8 *, void *,
                   u8, s32);
void func_15150D1C(Blood36C3CSpray *, u8, s32);
void func_151A9834(Vec3f *, f32, f32, GroundHit *, s32, u8, s8 *, u8, s32);
void func_151C61A0(s32, s32, u8, s32);
void func_1513A48C(Vec3f *, u8, s32);
extern s32 *D_80089A20[];
extern f32 D_800A3C30;
extern s32 D_800A3F14[];
extern s32 D_800A3F64[];


/*
 * Blood effects for a hit on actor: arg3 runs func_1513A24C (arg5 lowers its count), arg1
 * a spray, arg2 drops falling to the ground below, arg4 a splat at the actor's position.
 */
void func_15136C3C(Actor *actor, u8 arg1, u8 arg2, u8 arg3, u8 arg4,
                   u8 arg5, u8 arg6, s32 arg7) {
    Vec3f position;
    f32 unused0;
    f32 unused1;
    f32 scale;
    GroundHit hit;
    s32 kind;
    Blood36C3CSpray spray;
    s8 heavy;

    if (actor == NULL) {
        return;
    }
    scale = (actor->xz_scale + actor->y_scale) * 0.5f;
    func_1515C244(actor, &position, &unused0, &unused1);
    kind = func_15134070(actor);
    if (kind == BLOOD_PROFILE_NONE) {
        return;
    }
    func_1504715C(&hit, actor);
    if (arg3 != 0) {
        func_1513A24C(&position, D_80089A20[kind], &D_800A3C30,
                      ((arg5 != 0) || ((kind == 4) && (actor->id != MODEL_UGA_BUGA_BLUE_HEADGEAR)))
                          ? D_800A3F14[kind] - 2
                          : D_800A3F14[kind],
                      scale, kind, D_800A3F64[kind], (u8 *)actor, &hit, arg6,
                      arg7);
    }
    if (D_800A3FD8[kind].unkE == 2) {
        return;
    }
    if (arg1 != 0) {
        spray.countBase = 12;
        spray.countRange = 3;
        spray.origin = position;
        spray.angleBase = 0;
        spray.angleRange = 255;
        spray.pitchBase = -64;
        spray.pitchRange = 69;
        spray.lifeBase = 150;
        spray.lifeRange = 100;
        spray.maxDistance = 998.0f;
        spray.sizeBase = 25.0f;
        spray.sizeRange = 10.0f;
        if ((&D_800A3FF8 == &D_800A3FD8[kind]) || (&D_800A40C8 == &D_800A3FD8[kind]) ||
            (&D_800A40F8 == &D_800A3FD8[kind])) {
            spray.kind = 1;
        } else {
            spray.kind = 0;
        }
        func_15150D1C(&spray, arg6, arg7);
    }
    if (arg2 != 0) {
        if ((&D_800A3FF8 == &D_800A3FD8[kind]) || (&D_800A40C8 == &D_800A3FD8[kind]) ||
            (&D_800A40F8 == &D_800A3FD8[kind])) {
            heavy = 1;
        } else {
            heavy = 0;
        }
        func_151A9834(&position, position.y - 300.0f, 300.0f, &hit,
                      (func_150ADA20() % 6U) + 18, 0, &heavy, arg6, arg7);
    }
    if (arg4 != 0) {
        if ((&D_800A3FF8 == &D_800A3FD8[kind]) || (&D_800A40C8 == &D_800A3FD8[kind])) {
            func_151C61A0((s32)&position, (s32)&hit, arg6, arg7);
        } else {
            func_1513A48C(&position, arg6, arg7);
        }
    }
}

extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, f32 *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_151DC034(Vec3f *, f32, s32, s16, u8, u8, s32);
s32 func_15137C64(Vec3f *start, Vec3f *end, Vec3f *dir, Vec3f *side, Vec3f *up,
                  Vec3f *from, Vec3f *to, Vec3f *direction);

/* Blood for an impact from `from` to `to`: a burst, a spray along the decalPos and wall decals. */
void func_15136F50(Vec3f *from, Vec3f *to, Vec3f *direction, u8 arg3, u8 arg4, s32 arg5) {
    Vec3f start;
    Vec3f end;
    Vec3f dir;
    Vec3f side;
    Vec3f up;
    u8 pad0; // matching: pad0-pad3 and pad9 are unused stack slots of the original
    u8 doSparks;
    u8 doDecals;
    s32 count;
    f32 sideOffset;
    f32 upOffset;
    Vec3f velocity;
    f32 randomA;
    f32 randomB;
    f32 sparkSide;
    f32 sparkUp;
    Vec3f sparkVelocity;
    s32 frame;
    u32 random;
    s16 sparks;
    s32 j;
    f32 decalSide;
    f32 decalUp;
    s32 pad1;
    s32 pad2;
    f32 normal[3];
    Vec3f decalPos;
    f32 distance;
    s32 surface;
    s32 pad3;

    doSparks = 1;
    doDecals = 1;
    frame = func_15137C64(&start, &end, &dir, &side, &up, from, to, direction);
    if (frame != 0) {
        if (frame == 2) {
            doSparks = 0;
            doDecals = 0;
        }
        randomA = func_150ADA68();
        random = func_150ADA20();
        func_151DC034(&start, randomA * 192.0f + 192.0f, (random % 101U + 0x9B) & 0xFF, (func_150ADA20() & 7) + 0xF, arg3, arg4, arg5);
        {
        s32 pad9;
        count = (func_150ADA20() & 3) + 4;
        for (; count > 0; count--) {
            randomA = func_150ADA68() * 0.09290000051f + 0.06430000067f;
            random = func_150ADA20();
            func_15143874(random & 0xFF, func_150ADA68() * 50.0f, &upOffset, &sideOffset);
            velocity.x = (dir.x * 100.0f + side.x * upOffset + up.x * sideOffset) * randomA;
            velocity.y = (dir.y * 100.0f + side.y * upOffset + up.y * sideOffset) * randomA;
            velocity.z = (dir.z * 100.0f + side.z * upOffset + up.z * sideOffset) * randomA;
            randomB = func_150ADA68();
            randomA = func_150ADA68();
            random = func_150ADA20();
            func_151DB5D0(arg3, &end, &velocity, randomB * 80.0f + 120.0f, 1.033617973f, 0.959544003f, randomA * 0,
                          random % 31U + 0x1E, func_150ADA20() % 156U + 0x64, 0x2E, 5, 0, arg4, arg5);
        }
        }
        if (doSparks) {
            sparks = (func_150ADA20() & 3) + 4;
            if (sparks != 0) {
                do {
                    random = func_150ADA20();
                    func_15143874(random & 0xFF, func_150ADA68() * 2 + 6.0f, &sparkUp, &sparkSide);
                    sparkVelocity.x = side.x * sparkUp + up.x * sparkSide;
                    sparkVelocity.y = side.y * sparkUp + up.y * sparkSide;
                    sparkVelocity.z = side.z * sparkUp + up.z * sparkSide;
                    random = func_150ADA20();
                    randomA = func_150ADA68();
                    func_151DA6F8(&start, &sparkVelocity, 0.0f, random % 6U + 8, 0xFF, randomA * 1.300000072f + 1.899999976f,
                                  func_150ADA20() % 3U + 4, 0, 1.0f, 1.0f, 1, arg3, 0, 0x10, 0xF, 0, arg4, arg5);
                    sparks--;
                } while (sparks != 0);
            }
        }
        if (doDecals && D_800BE616) {
            count = (func_150ADA20() & 3) + 3;
            if (count != 0) {
                do {
                    random = func_150ADA20();
                    func_15143874(random & 0xFF, func_150ADA68() * 140.0f, &decalUp, &decalSide);
                    if (func_150AC9C0(end.x, end.y, end.z,
                                      dir.x * 100.0f + side.x * decalUp + up.x * decalSide,
                                      dir.y * 100.0f + side.y * decalUp + up.y * decalSide,
                                      dir.z * 100.0f + side.z * decalUp + up.z * decalSide,
                                      0, normal, &decalPos.x, &decalPos.y, &decalPos.z, &distance, &surface, 0, 0.0f) != 0 &&
                        func_15145C90(surface) != 0 && distance < 1999.0f) {
                        randomA = func_150ADA68();
                        random = func_150ADA20();
                        func_151D9B8C(arg3, randomA * 16.0f + 25.0f, random % 156U + 0x64, normal, &decalPos,
                                      func_150ADA20() % 101U + 0x64, 1, 1, 1, arg4, arg5);
                    }
                    count--;
                } while (count != 0);
            }
        }
    }
}


void func_151C329C(Vec3f *, u8, s32);
void func_151C577C(Vec3f *, Vec3f *, Vec3f *, u8, u8, u8, u8, s32);
void func_151036B4(void *, u8, s32);

/* Picks the impact effect for a hit on actor (NULL: plain blood) from its model and state. */
void func_15137610(Actor *actor, Vec3f *from, Vec3f *to,
                   Vec3f *direction, u8 arg4, s32 arg5) {
    s32 mode;

    if (actor != 0) {
        if (actor->unk1D4 == 0 || (actor->unk74 & 0xF) == 0xF ||
            (s32)actor->health <= 0) {
            return;
        }
        mode = actor->id;
        switch (mode) {
            case 5:
            case 173:
            case 174:
            case 175:
                mode = 3;
                break;
            case MODEL_RODENT:
            case MODEL_GREGG_THE_GRIM_REAPER_SCYTHE:
            case 166:
            case MODEL_GREGG_THE_GRIM_REAPER_HOODED:
            case MODEL_GREGG_THE_GRIM_REAPER_WITHOUT_ROBE:
                func_151036B4(from, arg4, arg5);
                mode = 2;
                break;
            case 40:
            case 119:
            case 138:
            case 140:
                if ((func_150ADA20() & 1) == 0) {
                    mode = 2;
                    break;
                }
                return;
            case 66:
                if ((func_150ADA20() & 3) == 0) {
                    mode = 2;
                    break;
                }
                return;
            case MODEL_TEDIZ:
            case MODEL_TEDIZ_AMMUNITION_BELT:
            case MODEL_TEDIZ_VARIANT_1:
            case MODEL_TEDIZ_VARIANT_2:
            case MODEL_TEDIZ_VARIANT_3:
            case 141:
            case MODEL_TEDIZ_BROAD_SHOULDERED_VARIANT:
                mode = actor->immune;
                if (mode == 0xFF) {
                    if ((func_150ADA20() & 0x1F) == 0) {
                        mode = 1;
                        break;
                    }
                } else if (mode == 0 && actor->stunned == 0) {
                    mode = 1;
                    break;
                }
                return;
            default:
                if (actor->immune == 0 && actor->stunned == 0) {
                    mode = 0;
                } else {
                    return;
                }
                break;
        }
    } else {
        mode = 0;
    }
    switch (mode) {
        case 1:
            func_151C577C(from, to, direction, 1, 1, 1, arg4, arg5);
            return;
        case 2:
            func_151C329C(from, arg4, arg5);
            return;
        case 3:
            func_15136F50(from, to, direction, 1, arg4, arg5);
            return;
        default:
        case 0:
            func_15136F50(from, to, direction, 0, arg4, arg5);
            return;
    }
}

typedef void *(*Blood3783CSpawn)(Vec3f *, Vec3f *, f32, Vec3f *, Vec3f *, f32,
                                 s32, s32, s32, f32, s32, s32, s32, s32);

s32 func_1514672C(f32 *);
void *func_151B5BF0(Vec3f *, Vec3f *, f32, Vec3f *, Vec3f *, f32, s32, s32,
                    s32, f32, s32, s32, s32, s32);
void *func_151B6010(Vec3f *, Vec3f *, f32, Vec3f *, Vec3f *, f32, s32, s32,
                    s32, f32, s32, s32, s32, s32);
/* Blood when an actor dies: a burst, a splat and, on flagged ground, a pool and a chunk. */
void func_1513783C(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f position;
    f32 unused0;
    f32 unused1;
    f32 scale;
    Vec3f probe;
    GroundHit hit;
    s32 kind;
    u8 pool;
    Vec3f origin;
    Vec3f velocity;
    Vec3f rotation;
    Vec3f spin;
    Blood3783CSpawn spawn;

    if (actor == NULL) {
        return;
    }
    kind = func_15134070(actor);
    if (kind == BLOOD_PROFILE_NONE) {
        return;
    }
    scale = (actor->xz_scale + actor->y_scale) * 0.5f;
    func_1515C244(actor, &position, &unused0, &unused1);
    func_1504715C(&hit, actor);
    probe.x = position.x;
    probe.y = position.y + 100.0f;
    probe.z = position.z;
    if (func_1514672C(&probe.x) == 0) {
        return;
    }
    if (func_15046C80(&probe, 0, probe.y - 500.0f, &hit) != 0) {
        if (hit.flags & 1) {
            pool = 1;
        } else {
            pool = 0;
        }
        probe.y = hit.height;
    } else {
        pool = 0;
        probe = position;
    }
    func_1513A24C(&position, D_80089A20[kind] + 2, &D_800A3C30,
                  D_800A3F14[kind] - 2, scale, kind, D_800A3F64[kind],
                  (u8 *)actor, &hit, arg1, arg2);
    if (D_800A3FD8[kind].unkE != 2) {
        func_1513A48C(&position, arg1, arg2);
        if (pool != 0) {
            func_15136698(((func_150ADA68() * 25.0f) + 75.0f) * 0.001000000047f,
                          ((func_150ADA68() * 106.0f) + 75.0f) * 9.999999747e-06f,
                          (func_150ADA20() % 101U) + 155,
                          (func_150ADA20() % 101U) + 100,
                          (func_150ADA20() % 56U) + 203, (s32)hit.unk4,
                          &probe, 1, 1, arg1, arg2);
            if (func_150ADA20() & 1) {
                spawn = func_151B5BF0;
            } else {
                spawn = func_151B6010;
            }
            origin.x = probe.x;
            velocity.x = 0.0f;
            origin.y = probe.y + 10.0f;
            velocity.y = 0.0f;
            velocity.z = 0.0f;
            origin.z = probe.z;
            rotation.x = func_150ADA68() * 360.0f;
            rotation.y = func_150ADA68() * 360.0f;
            rotation.z = func_150ADA68() * 360.0f;
            spin.x = 0.0f;
            spin.y = 0.0f;
            spin.z = 0.0f;
            spawn(&origin, &velocity,
                  ((func_150ADA68() * 101.0f) + 600.0f) * 0.001000000047f, &rotation,
                  &spin, 0.0f, 1000, 255, 255, 1.0f, 3, 0, arg1, arg2);
        }
    }
    func_15145A50(actor);
}

/*
 * Builds an impact frame: start/end from `from`/`to` (either may be NULL), dir = direction or
 * the normalized end - start, side/up perpendicular to it. Returns 0 if impossible, 2 without
 * a side/up basis, else 1.
 */
s32 func_15137C64(Vec3f *start, Vec3f *end, Vec3f *dir, Vec3f *side,
                  Vec3f *up, Vec3f *from, Vec3f *to, Vec3f *direction) {
    f32 length;
    f32 unused;

    if ((from != NULL) && (to != NULL)) {
        *start = *from;
        *end = *to;
    } else if (from != NULL) {
        *start = *from;
        *end = *from;
    } else if (to != NULL) {
        *start = *to;
        *end = *to;
    } else {
        return 0;
    }

    if (direction == NULL) {
        dir->x = end->x - start->x;
        dir->y = end->y - start->y;
        dir->z = end->z - start->z;
        if (func_15145128(dir, dir, &length, &unused) == 0) {
            return 0;
        }
    } else {
        *dir = *direction;
    }

    if (func_15146078(dir, side, up) == 0) {
        return 2;
    }
    return 1;
}

/* BloodStrip sample callback 5: random power. */
s32 func_15137E10(BloodStrip *strip) {
    f32 random = func_150ADA68();

    strip->power = (random * 50.0f + 580.0f) * 0.001000000047f;
    return 1;
}

void func_15137F30(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB,
                   f32 time, BloodStrip *strip, Vec3f *start, Vec3f *end,
                   Vec3f *velocity, f32 *arg3, s16 *arg4, u8 *arg5, f32 *arg6);

/* BloodStrip emit callback 5: one func_151D9014 blood particle. */
void func_15137E60(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA,
                   Vec3f *velocityB, f32 time, BloodStrip *strip) {
    Vec3f start;
    Vec3f end;
    Vec3f velocity;
    f32 arg3;
    s16 arg4;
    u8 arg5;
    f32 arg6;

    func_15137F30(pointA, pointB, velocityA, velocityB, time, strip,
                  &start, &end, &velocity, &arg3, &arg4, &arg5, &arg6);
    func_151D9014(&start, &velocity, 0, arg3, arg4, arg5, arg6, 0, 1.0f, 1.0f,
                  1, 0, 1, 0, ((u8 *)strip)[0xC], ((u8 *)strip)[1]);
}

/*
 * Moves both strip points `time` along their velocities and returns the particle parameters for
 * func_151D9014: start at A, velocity from A to B scaled by the strip's power, random extras.
 */
void func_15137F30(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB,
                   f32 time, BloodStrip *strip, Vec3f *start, Vec3f *end,
                   Vec3f *velocity, f32 *arg3, s16 *arg4, u8 *arg5, f32 *arg6) {
    start->x = pointA->x + velocityA->x * time;
    start->y = pointA->y + velocityA->y * time;
    start->z = pointA->z + velocityA->z * time;
    end->x = pointB->x + velocityB->x * time;
    end->y = pointB->y + velocityB->y * time;
    end->z = pointB->z + velocityB->z * time;
    velocity->x = (end->x - start->x) * strip->power;
    velocity->y = (end->y - start->y) * strip->power;
    velocity->z = (end->z - start->z) * strip->power;
    *arg3 = (func_150ADA68() * 217.0f + -456.0f) * 0.001000000047f;
    *arg4 = func_150ADA20() % 31U + 30;
    *arg5 = func_150ADA20() % 156U + 100;
    *arg6 = func_150ADA68() * 35.0f + 40.0f;
}

/* Writes the actor's blood point for profile `kind` (bone 12 space) to out; 0 if no skeleton. */
s32 func_151380B4(Actor *actor, s32 kind, f32 *out) {
    s32 bones = (s32)actor->unk1D4;

    if (bones == 0) {
        return 0;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[kind].offset, out, (void *)(bones + 12 * 0x40));
    return 1;
}

/* Starts a bleeding BloodStrip on the actor's bone 12 (or 1) playing sound 0x4FE. */
void func_15138120(Actor *actor, s32 kind, u8 arg2) {
    BloodStripDesc desc;

    if (D_800A3FD8[kind].unkE != 2) {
        desc.ownerId = actor->unique_id;
        desc.owner = actor;
        if (arg2) {
            desc.bone = 12;
        } else {
            desc.bone = 1;
        }
        desc.flags = BLOOD_STRIP_LIFETIME;
        desc.duration = 40;
        desc.pause = 16;
        desc.offsetA.x = 0.0f;
        desc.offsetA.y = 0.0f;
        desc.offsetA.z = 0.0f;
        desc.offsetB.x = 0.0f;
        desc.offsetB.z = 0.0f;
        desc.offsetB.y = 20.0f;
        if ((&D_800A4058 == &D_800A3FD8[kind]) || (&D_800A4068 == &D_800A3FD8[kind])) {
            desc.lifetime = (actor->unk94 & 0xE) ? 120 : 240;
        } else {
            desc.lifetime = 600;
        }
        desc.sampleCallback = 5;
        if (D_800A3FD8[kind].unkE == 0) {
            desc.emitCallback = 5;
        } else {
            desc.emitCallback = 6;
        }
        desc.rateCallback = -1;
        desc.mode = 0;
        desc.updateCallback = -1;
        desc.rate = 1.0f;
        desc.sound = func_1000FA64(0x4FE, (s16)(s32)actor->x_position,
                                   (s16)(s32)actor->y_position,
                                   (s16)(s32)actor->z_position, 0x5DC0,
                                   600, 300, (SoundRecordCallback)func_1000EBC4,
                                   (void *)120, 0, 0, 0);
        func_15134DAC(&desc, 0);
    }
}

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    Vec3f position;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} LocalDef151382E0;

/* Spawns a func_15153F18 blood burst at position for profile `kind` (none for type 2). */
void func_151382E0(Vec3f *position, s32 kind, void *arg2, u8 arg3, s32 arg4) {
    LocalDef151382E0 burst;
    s32 type;

    type = D_800A3FD8[kind].unkE;
    if (type != 2) {
        burst.position = *position;
        burst.unk14 = 1.9799999f;
        burst.unk18 = 2.00999999f;
        burst.unk1C = 0.202000007f;
        burst.unk20 = 0.6460000277f;
        burst.unk24 = 4.0f;
        burst.unk28 = 9.0f;
        burst.unk2C = 18;
        burst.unk2E = 7;
        burst.unk02 = 255;
        burst.unk00 = 0;
        burst.unk04 = -63;
        burst.unk06 = 78;
        burst.unk30 = 3;
        burst.unk32 = 3;
        burst.unk34 = 20;
        burst.unk36 = 30;
        burst.unk38 = 155;
        burst.unk3A = 100;
        burst.unk44 = 16;
        burst.unk46 = 15;
        burst.unk48 = 0;
        burst.unk40 = 0.3000000119f;
        if (type == 1) {
            burst.unk3C = 1;
        } else {
            burst.unk3C = 0;
        }
        func_15153F18(&burst, &burst.position, (s32)arg2, arg3, arg4);
    }
}

extern void func_1503F404(void *matrix, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);

/*
 * Throws a func_1513264C chunk (model D_800A3FD8[kind].unkC) off the actor from position, and for
 * profiles 4-6 also the accessories it still wears (bits of unk94).
 */
void func_15138424(Actor *actor, Vec3f *position, s32 kind, void *arg3, u8 arg4, s32 arg5) {
    Vec3f v;
    ParticleSpawn spawn;
    Vec3f a;
    Vec3f b;

    func_1503F404(BONE_MATRIX(actor, 12), &a.x, &a.y, &a.z, &v.x, &v.y, &v.z, &b.x, &b.y, &b.z);
    spawn.scale.x = 1.0f;
    spawn.radius = 20.0f;
    spawn.elasticity = 0.5610000491f;
    if (D_800A3FD8[kind].unkE == 1) {
        spawn.scale.y = 1.0f;
    } else {
        spawn.scale.y = 0.0f;
    }
    if (D_800A3FD8[kind].unkE == 2) {
        spawn.scale.y = 2.0f;
    }
    spawn.scale.z = 1.0f;
    spawn.flags = 0x39E9;
    spawn.rotVel.y = 0.0f;
    spawn.lifetime = func_150ADA20() % 51U + 250;
    spawn.model = D_800A3FD8[kind].unkC;
    spawn.sizeY = ((&D_800A40C8 == &D_800A3FD8[kind]) ? 0.6000000238f : 1.0f) * ((actor->xz_scale + actor->y_scale) * 0.5f);
    spawn.sizeXZ = spawn.sizeY;
    spawn.gravity = (func_150ADA68() * 292.0f + -1099.0f) * 0.001000000047f;
    spawn.pos = *position;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 21U - 83), func_150ADA68() * 8.0f + 9.0f, &spawn.vel.x);
    spawn.rot = v;
    spawn.rotVel.x = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.rotVel.z = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 255;
    spawn.updateFn = 13;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 8;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 1;
    spawn.owner = actor;
    spawn.ownerId = actor->unique_id;
    spawn.fadeStart = 12;
    spawn.fadeRate = 21;
    func_1513264C(&spawn, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
    if (&D_800A4018 == &D_800A3FD8[kind]) {
        const s32 ids[3] = { 0x76, 0x77, 0x78 };
        u8 i;
        u8 found;

        i = 0;
        found = 0;
        do {
            if (func_1503195C(actor, ids[i], 0) != NULL) {
                found = 1;
            } else {
                i++;
            }
        } while (i < 3 && !found);
        if (found) {
            const s32 ids2[3] = { 0x84, 0x83, 0x82 };

            spawn.model = ids2[i];
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
        }
    } else if (&D_800A4028 == &D_800A3FD8[kind]) {
        const s32 masks[4] = { 2, 8, 0x10, 0x20 };
        u8 i;
        const s32 ids[4] = { 0x87, 0x88, 0x89, 0x8A };

        for (i = 0; i < 4; i++) {
            if (!(masks[i] & actor->unk94)) {
                spawn.model = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    } else if (&D_800A4038 == &D_800A3FD8[kind]) {
        const s32 masks[2] = { 2, 8 };
        u8 i;
        const s32 ids[2] = { 0x8D, 0x8B };

        for (i = 0; i < 2; i++) {
            if (!(masks[i] & actor->unk94)) {
                spawn.model = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    }
}

/* A bouncing chunk thrown by func_1513264C / func_15132A4C (from a ParticleSpawn desc at +0x10). */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 radius;
    f32 bounce;             /* velocity factor kept on each bounce */
    f32 width;
    f32 height;
    u8 pad20[0xC];
    f32 unk2C;
    f32 type;               /* 0, 1 or 2 (no decal), from the blood profile */
    f32 unk34;
    Vec3f position;
    Vec3f velocity;
    Vec3f spin;
    f32 unk5C;
    s32 flags;
    u8 pad64[0x10C];
    s32 unk170;             /* counted down by message 0x45; first byte set by the spawner */
} BloodChunk;

/* BloodChunk ground-hit callback: bounces off groundY and leaves a blood decal there. */
s32 func_151389A8(BloodChunk *chunk, s32 arg1, s32 arg2, s32 arg3, f32 groundY, s32 surface) {
    Vec3f pos;
    f32 bounce;
    u32 random;
    f32 randomF;
    s32 flag;

    bounce = chunk->bounce;
    chunk->position.y = chunk->radius + groundY;
    chunk->velocity.x = chunk->velocity.x * bounce;
    chunk->velocity.y = chunk->velocity.y * -bounce;
    chunk->velocity.z = chunk->velocity.z * bounce;
    chunk->spin.x = chunk->spin.x * bounce;
    chunk->spin.y = chunk->spin.y * bounce;
    chunk->spin.z = chunk->spin.z * bounce;
    if (fabsf(chunk->velocity.y) < 4.0f) {
        chunk->velocity.x = 0.0f;
        chunk->flags &= ~0x69;
        chunk->velocity.y = 0.0f;
        chunk->velocity.z = 0.0f;
        chunk->spin.x = 0.0f;
        chunk->spin.y = 0.0f;
        chunk->spin.z = 0.0f;
        chunk->unk5C = 0.0f;
    }
    if (chunk->type != 2.0f) {
        pos.x = chunk->position.x;
        pos.y = groundY;
        pos.z = chunk->position.z;
        if (chunk->type) {
            flag = 1;
        } else {
            flag = 0;
        }
        randomF = func_150ADA68();
        random = func_150ADA20();
        func_151D9B8C(flag, (randomF * 25.39999962f + 39.79999924f) * ((chunk->width + chunk->height) * 0.5f),
                      (random % 101U) + 100, surface, &pos, (func_150ADA20() % 101U) + 80, 1, 1, 0,
                      chunk->unkC, chunk->unk1);
        chunk->unk2C = 0.0f;
    }
    return 1;
}

/* Blood for a hit on the actor's bone-12 blood point: strip, burst and chunk. */
void func_15138BC0(Actor *actor, u8 arg1, s32 arg2) {
    s32 kind;
    Vec3f position;
    u8 valid;
    GroundHit hit;

    kind = func_15134070(actor);
    if (kind != BLOOD_PROFILE_NONE) {
        valid = func_151380B4(actor, kind, &position.x);
        func_15138120(actor, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, actor);
            func_151382E0(&position, kind, &hit, arg1, arg2);
            func_15138424(actor, &position, kind, &hit, arg1, arg2);
        }
    }
}

void func_15138E98(Actor *, f32 *, u8, void *, u8, s32);
void func_15139768(Actor *, f32 *, f32, void *, u8, s32);
void func_15139D74(Actor *, f32 *, void *, u8, s32);
void func_1513A6E0(Actor *, f32 *, void *, u8, s32);
void func_1513ABB8(Actor *, f32 *, u8, void *, u8, s32);
void func_1513B0F8(Actor *, f32 *, void *, u8, s32);
void func_1513A594(Actor *, Vec3f *, GroundHit *, u8, s32);
void func_151036B4(void *, u8, s32);

/* Blood for a fatal hit: strip and burst, then the per-profile gore effect. */
void func_15138C80(Actor *actor, u8 arg1, s32 arg2) {
    s32 kind;
    Vec3f position;
    u8 valid;
    GroundHit hit;

    kind = func_15134070(actor);
    if (kind != BLOOD_PROFILE_NONE) {
        valid = func_151380B4(actor, kind, &position.x);
        func_15138120(actor, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, actor);
            func_151382E0(&position, kind, &hit, arg1, arg2);
            switch (kind) {
            case 16:
                func_1513B0F8(actor, &position.x, &hit, arg1, arg2);
                return;
            case 17:
                func_151036B4(&position.x, arg1, arg2);
                return;
            case 0:
            case 7:
                func_1513A6E0(actor, &position.x, &hit, arg1, arg2);
                return;
            case 3:
                func_15138E98(actor, &position.x, 1, &hit, arg1, arg2);
                return;
            case 5:
            case 6:
                func_15138E98(actor, &position.x, 0, &hit, arg1, arg2);
                return;
            case 15:
                func_15139768(actor, &position.x, 0.6f, &hit, arg1, arg2);
                return;
            case 2:
                func_15139768(actor, &position.x, 1.0f, &hit, arg1, arg2);
                return;
            case 4:
                func_1513A594(actor, &position, &hit, arg1, arg2);
                return;
            case 8:
            case 9:
                func_1513ABB8(actor, &position.x, kind == 9, &hit, arg1, arg2);
                return;
            case 10:
            case 11:
            case 14:
            case 18:
            case 19:
                return;
            case 1:
            case 12:
            case 13:
            default:
                func_15139D74(actor, &position.x, &hit, arg1, arg2);
                break;
            }
        }
    }
}

extern u8 D_800A4160[];
extern s32 D_800A4168[];
extern s32 D_800A4178[];
extern Vec3f D_800A4188[];
extern Vec3f D_800A41B8;
void func_1513A5E0(Vec3f *position, s32 arg1, s32 arg2);

/* Gore for profile 3, 5 and 6 actors: a random chunk set from D_800A4160 and, with extraSpray, chunk model 0x48. */
void func_15138E98(Actor *actor, f32 *origin, u8 extraSpray, void *hit, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 6U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        burst.rot.x = 0.0f;
        burst.rot.y = 0.0f;
        burst.rot.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.pos.x = 0.0f;
        burst.pos.y = 0.0f;
        burst.pos.z = 0.0f;
        burst.vel.x = 0.0f;
        burst.vel.y = 0.0f;
        burst.vel.z = 0.0f;
        burst.rotVel.x = 0.0f;
        burst.rotVel.y = 0.0f;
        burst.rotVel.z = 0.0f;
        burst.gravity = 0.0f;
        burst.flags = 0x101900;
        burst.lifetime = 100;
        burst.variant = 5;
        burst.unk5C = 0;
        burst.alpha = 255;
        burst.updateFn = 0;
        burst.update2Fn = 0;
        burst.floorHitFn = 0;
        burst.floorType3Fn = 0;
        burst.wallHitFn = 0;
        burst.destroyFn = 0;
        burst.squashFn = 0;
        burst.matrixFn = 5;
        burst.unk6A = 1;
        burst.owner = actor;
        burst.ownerId = actor->unique_id;
        burst.fadeStart = 12;
        burst.fadeRate = 21;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (!(D_800A4160[index] & mask)) {
                continue;
            }
            burst.model = D_800A4168[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy(&((BloodChunk *)result)->unk170, tag, 1);
            }
        }
    }
    if (actor->unk1D4 == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((Vec3f *)origin, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        ParticleSpawn packet;
        u8 j;
        u8 bit;
        ParticleSpawn spray;
        u32 yaw;
        u32 pitch;

        func_1503F404(BONE_MATRIX(actor, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.radius = 1.0f;
        packet.elasticity = 0.64900005f;
        packet.sizeXZ = packet.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        packet.rot = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.rotVel.y = 0.0f;
        packet.variant = 0;
        packet.unk5C = 0;
        packet.alpha = 255;
        packet.update2Fn = 0;
        packet.floorType3Fn = 0;
        packet.wallHitFn = 0;
        packet.destroyFn = 0;
        packet.squashFn = 0;
        packet.unk6A = 1;
        packet.owner = actor;
        packet.ownerId = actor->unique_id;
        packet.fadeStart = 12;
        packet.fadeRate = 21;
        packet.flags = 0x1039E9;
        packet.updateFn = 13;
        packet.matrixFn = 2;
        packet.floorHitFn = 9;
        for (j = 0, bit = 1; j < 4; j++, bit <<= 1) {
            if (!(D_800A4160[index] & bit)) {
                packet.model = D_800A4178[j];
                func_15143134(&D_800A4188[j], &packet.pos, BONE_MATRIX(actor, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 84), func_150ADA68() * 6.12f + 3.0f,
                              &packet.vel.x);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)hit, 0, arg4, arg5);
            }
        }
        if (extraSpray == 0) {
            return;
        }
        spray.radius = 1.0f;
        spray.elasticity = 0.64900005f;
        spray.sizeXZ = spray.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        spray.rot = position;
        spray.scale.x = 1.0f;
        spray.scale.y = 1.0f;
        spray.scale.z = 1.0f;
        spray.rotVel.y = 0.0f;
        spray.variant = 0;
        spray.unk5C = 0;
        spray.alpha = 255;
        spray.update2Fn = 0;
        spray.floorType3Fn = 0;
        spray.wallHitFn = 0;
        spray.destroyFn = 0;
        spray.squashFn = 0;
        spray.unk6A = 1;
        spray.owner = actor;
        spray.ownerId = actor->unique_id;
        spray.fadeStart = 12;
        spray.fadeRate = 21;
        spray.flags = 0x1039E9;
        spray.updateFn = 13;
        spray.matrixFn = 2;
        spray.floorHitFn = 11;
        spray.model = 0x48;
        func_15143134(&D_800A41B8, &spray.pos, BONE_MATRIX(actor, 12));
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794((u8)yaw, (s16)(pitch % 16U - 60), func_150ADA68() * 7.1499996f + 10.889999f,
                      &spray.vel.x);
        spray.rotVel.x = func_150ADA68() * 48.158f + -24.079f;
        spray.rotVel.z = func_150ADA68() * 48.158f + -24.079f;
        spray.gravity = func_150ADA68() * 0.323f + -1.4210001f;
        spray.lifetime = func_150ADA20() % 61U + 100;
        func_1513264C(&spray, 3, 0xFF, (s32)hit, 0, arg4, arg5);
    }
}

/* BloodChunk ground-hit callback: bounces off groundY and leaves a blood decal there. */
s32 func_15139578(BloodChunk *chunk, s32 arg1, s32 arg2, s32 arg3, f32 groundY, s32 surface) {
    Vec3f pos;
    f32 bounce;
    u32 random;
    f32 randomF;
    s32 flag;

    bounce = chunk->bounce;
    chunk->position.y = chunk->radius + groundY;
    chunk->velocity.x = chunk->velocity.x * bounce;
    chunk->velocity.y = chunk->velocity.y * -bounce;
    chunk->velocity.z = chunk->velocity.z * bounce;
    chunk->spin.x = chunk->spin.x * bounce;
    chunk->spin.y = chunk->spin.y * bounce;
    chunk->spin.z = chunk->spin.z * bounce;
    if (fabsf(chunk->velocity.y) < 4.0f) {
        chunk->velocity.x = 0.0f;
        chunk->flags &= ~0x69;
        chunk->velocity.y = 0.0f;
        chunk->velocity.z = 0.0f;
        chunk->spin.x = 0.0f;
        chunk->spin.y = 0.0f;
        chunk->spin.z = 0.0f;
        chunk->unk5C = 0.0f;
    }
    if (chunk->type != 2.0f) {
        pos.x = chunk->position.x;
        pos.y = groundY;
        pos.z = chunk->position.z;
        if (chunk->type) {
            flag = 1;
        } else {
            flag = 0;
        }
        randomF = func_150ADA68();
        random = func_150ADA20();
        func_151D9B8C(flag, (randomF * 27.60000038f + 19.70000076f) * ((chunk->width + chunk->height) * 0.5f),
                      (random % 101U) + 100, surface, &pos, (func_150ADA20() % 144U) + 80, 1, 1, 0,
                      chunk->unkC, chunk->unk1);
    }
    return 1;
}

extern u8 D_800A41C4[];
extern s32 D_800A41C8[];
extern s32 D_800A41D8[];
extern Vec3f D_800A41E8[];
extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);

/* Gore for profile 2/15 actors, chunk sizes scaled by `scale`. */
void func_15139768(Actor *actor, f32 *origin, f32 scale, void *hit, u8 arg4, s32 arg5) {
    u8 idx;
    u8 i;
    u8 mask;
    Vec3f v;

    idx = func_150ADA20() & 3;
    {
        ParticleSpawn spawn;
        u8 tag[4];
        tag[0] = 12;
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.sizeXZ = spawn.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f * scale;
        spawn.rot.x = 0.0f;
        spawn.rot.y = 0.0f;
        spawn.rot.z = 0.0f;
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.rotVel.x = 0.0f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = 0.0f;
        spawn.flags = 0x101900;
        spawn.lifetime = 100;
        spawn.variant = 5;
        spawn.unk5C = 0;
        spawn.alpha = 255;
        spawn.updateFn = 0;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 5;
        spawn.unk6A = 1;
        spawn.owner = actor;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.ownerId = actor->unique_id;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (D_800A41C4[idx] & mask) {
                s32 ret;
                spawn.model = D_800A41C8[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg4, arg5);
                if (ret != 0) {
                    memcpy(&((BloodChunk *)ret)->unk170, tag, 1);
                }
            }
        }
    }
    if (actor->unk1D4 != 0 && (actor->unk74 & 0xF) != 0xF) {
        s32 count;
        u8 mask2;
        u8 j;
        Vec3f dir;
        Vec3f a;
        Vec3f b;
        ParticleSpawn spawn2;
        for (count = (func_150ADA20() & 3) + 5; count > 0; count--) {
            func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 71U - 60), func_150ADA68() * 6.0f + 6.0f, &dir.x);
            func_151C5F44((s32)origin, &dir.x, func_150ADA68() * 0.1170000061f + -0.1970000118f, 0.9508649707f,
                          func_150ADA20() % 66U + 190, func_150ADA20() % 45U + 44, func_150ADA68() * 20.0f + 50.0f,
                          0, (s32)hit, arg4, arg5);
        }
        {
            s32 q; // matching: unused stack slot
            func_1503F404(BONE_MATRIX(actor, 12), &a.x, &a.y, &a.z, &v.x, &v.y, &v.z, &b.x, &b.y, &b.z);
        }
        spawn2.radius = 1.0f;
        spawn2.elasticity = 0.6490000486f;
        spawn2.sizeXZ = spawn2.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f * scale;
        spawn2.rot = v;
        spawn2.rotVel.y = 0.0f;
        spawn2.variant = 0;
        spawn2.unk5C = 0;
        spawn2.alpha = 255;
        spawn2.update2Fn = 0;
        spawn2.floorType3Fn = 0;
        spawn2.wallHitFn = 0;
        spawn2.destroyFn = 0;
        spawn2.squashFn = 0;
        spawn2.unk6A = 1;
        spawn2.owner = actor;
        spawn2.scale.x = 1.0f;
        spawn2.scale.y = 1.0f;
        spawn2.scale.z = 1.0f;
        spawn2.ownerId = actor->unique_id;
        spawn2.fadeStart = 12;
        spawn2.fadeRate = 21;
        spawn2.flags = 0x1039E9;
        spawn2.updateFn = 13;
        spawn2.matrixFn = 2;
        spawn2.floorHitFn = 9;
        for (j = 0, mask2 = 1; j < 4; j++, mask2 <<= 1) {
            if (!(D_800A41C4[idx] & mask2)) {
                spawn2.model = D_800A41D8[j];
                func_15143134(&D_800A41E8[j], &spawn2.pos, BONE_MATRIX(actor, 12));
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 35U - 84), func_150ADA68() * 6.119999886f + 3.0f, &spawn2.vel.x);
                spawn2.rotVel.x = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.rotVel.z = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.gravity = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, (s32)hit, 0, arg4, arg5);
            }
        }
    }
}

extern u8 D_800A4218[];
extern s32 D_800A421C[];
extern s32 D_800A4228[];
extern Vec3f D_800A4234[];

/* Default gore (profiles 1, 12, 13 and others): a random chunk set from bone 12. */
void func_15139D74(Actor *actor, f32 *origin, void *hit, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        burst.rot.x = 0.0f;
        burst.rot.y = 0.0f;
        burst.rot.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.pos.x = 0.0f;
        burst.pos.y = 0.0f;
        burst.pos.z = 0.0f;
        burst.vel.x = 0.0f;
        burst.vel.y = 0.0f;
        burst.vel.z = 0.0f;
        burst.rotVel.x = 0.0f;
        burst.rotVel.y = 0.0f;
        burst.rotVel.z = 0.0f;
        burst.gravity = 0.0f;
        burst.flags = 0x101900;
        burst.lifetime = 400;
        burst.variant = 5;
        burst.unk5C = 0;
        burst.alpha = 255;
        burst.updateFn = 0;
        burst.update2Fn = 0;
        burst.floorHitFn = 0;
        burst.floorType3Fn = 0;
        burst.wallHitFn = 0;
        burst.destroyFn = 0;
        burst.squashFn = 0;
        burst.matrixFn = 5;
        burst.unk6A = 1;
        burst.owner = actor;
        burst.ownerId = actor->unique_id;
        burst.fadeStart = 12;
        burst.fadeRate = 21;
        for (i = 0, mask = 1; i < 3; i++, mask <<= 1) {
            if (!(D_800A4218[index] & mask)) {
                continue;
            }
            burst.model = D_800A421C[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg3, arg4);
            if (result != 0) {
                memcpy(&((BloodChunk *)result)->unk170, tag, 1);
            }
        }
    }
    if (actor->unk1D4 == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((Vec3f *)origin, arg3 & 0xFF, arg4);
    {
        f32 first[3];
        f32 second[3];
        ParticleSpawn packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404(BONE_MATRIX(actor, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.radius = 1.0f;
        packet.elasticity = 0.64900005f;
        packet.sizeXZ = packet.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        packet.rot = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.rotVel.y = 0.0f;
        packet.variant = 0;
        packet.unk5C = 0;
        packet.alpha = 255;
        packet.update2Fn = 0;
        packet.floorType3Fn = 0;
        packet.wallHitFn = 0;
        packet.destroyFn = 0;
        packet.squashFn = 0;
        packet.unk6A = 1;
        packet.owner = actor;
        packet.ownerId = actor->unique_id;
        packet.fadeStart = 12;
        packet.fadeRate = 21;
        packet.flags = 0x1039E9;
        packet.updateFn = 13;
        packet.matrixFn = 2;
        packet.floorHitFn = 9;
        for (j = 0, bit = 1; j < 3; j++, bit <<= 1) {
            if (!(D_800A4218[index] & bit)) {
                packet.model = D_800A4228[j];
                func_15143134(&D_800A4234[j], &packet.pos, BONE_MATRIX(actor, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 84), func_150ADA68() * 6.12f + 3.0f,
                              &packet.vel.x);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)hit, 0, arg3, arg4);
            }
        }
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 unk40;
} Cfg080C_1513A24C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x08 */ s32 unk8;
} Extra080C_1513A24C;

void func_1515080C(Cfg080C_1513A24C *, s32 *, f32 *, s32, Extra080C_1513A24C *, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15133E3C(s32, u8);

/* Blood burst (func_1515080C) for an actor of blood profile `kind`; actor is a Actor. */
void func_1513A24C(Vec3f *position, s32 *table, f32 *arg2, s32 count, f32 scale, u8 kind, u8 arg6, u8 *actor, void *hit, u8 arg9, s32 argA) {
    Cfg080C_1513A24C config;
    Extra080C_1513A24C extra;
    s32 flag;

    config.unk0 = 40;
    config.unk4 = 0;
    config.pos = *position;
    config.unk14 = 0;
    config.unk16 = 255;
    config.unk2C = 480;
    config.unk2E = 240;
    config.unk30 = 1 * scale;
    config.unk34 = 0 * scale;
    config.unk40 = arg6;
    extra.unk0 = 4;
    flag = -1;
    config.unk18 = -64;
    config.unk1A = 12;
    config.unk20 = 5.0f;
    config.unk3C = 1.0f;
    config.unk1C = 13.0f;
    config.unk24 = -1.007000089f;
    config.unk28 = 0.2520000041f;
    config.unk38 = 16.10200119f;
    switch (kind) {
    case 19:
        if (D_800BE9F0 == 60) {
            config.unk3C = 0.0f;
        }
        break;
    case 13:
        config.unk18 = -46;
        config.unk1A = 20;
        config.unk1C = 15.0f;
        config.unk20 = 8.0f;
        config.unk24 = -1.262000084f;
        config.unk28 = 0.4900000095f;
        config.unk38 = 12.91100025f;
        break;
    case 1: {
        const u16 models[4] = { 0x83, 0x81, 0x82, 0x84 };
        s32 index;
        index = actor[0x128]; /* Actor byte 0x128, not in the shared struct yet */
        if (index >= 4) {
            index = 3;
        }
        extra.unk4 = models[index];
        extra.unk6 = 6;
        extra.unk8 = 3;
        flag = 1;
        break;
    }
    }
    func_1515080C(&config, table, arg2, count, &extra, 0xC, 6, 1, flag, hit, 5.0f, 1, D_800BE616 ? 1 : 0, arg9, argA);
    func_15133E3C(0, 0x45);
}

/* Parameters of a func_15152190 blood splash. */
typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3f position;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
} BloodSplashConfig;

void func_15152190(BloodSplashConfig *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4260[];
extern u8 D_800A4264[];

/* Small blood splash at position (func_15152190 with D_800A4260). */
void func_1513A48C(Vec3f *position, u8 arg1, s32 arg2) {
    BloodSplashConfig config;

    config.unk0 = 8;
    config.unk4 = 4;
    config.position = *position;
    config.unk14 = 0;
    config.unk16 = 255;
    config.unk18 = -55;
    config.unk1A = 32;
    config.unk2C = 40;
    config.unk2E = 20;
    config.unk30 = 0.07f;
    config.unk34 = 0.07f;
    config.unk1C = 10.0f;
    config.unk20 = 9.0f;
    config.unk24 = -1.0040001f;
    config.unk28 = 0.254f;
    config.unk38 = 23.446001f;
    func_15152190(&config, D_800A4260, D_800A4264, 1, 0.0f, 1,
                  (s32)arg1, arg2);
}

/* Gore for profile 4 actors: just a splash. */
void func_1513A594(Actor *actor, Vec3f *origin, GroundHit *hit, u8 arg3, s32 arg4) {
    func_1513A5E0(origin, arg3 & 0xFF, arg4);
    if (actor->unk1D4 != 0) { // matching: dead test, IDO still emits the unk1D4 load
        if (actor) {
        }
    }
}

extern u8 D_800A4268[];
extern u8 D_800A4270[];

/* Blood splash at position (func_15152190 with D_800A4268). */
void func_1513A5E0(Vec3f *position, s32 arg1, s32 arg2) {
    BloodSplashConfig config;

    config.unk0 = 7;
    config.unk4 = 7;
    config.position = *position;
    config.unk14 = 0;
    config.unk16 = 255;
    config.unk18 = -50;
    config.unk1A = 27;
    config.unk1C = 4.0f;
    config.unk20 = 4.0f;
    config.unk2C = 25;
    config.unk2E = 40;
    config.unk30 = 0.0201f;
    config.unk34 = 0.0201f;
    config.unk24 = -0.43f;
    config.unk28 = 0.272f;
    config.unk38 = 20.187f;
    func_15152190(&config, D_800A4268, D_800A4270, 2, 0.0f, 1,
                  (u8)arg1, arg2);
}

extern u8 D_800A4278[];
extern s32 D_800A4280[];
extern s32 D_800A4290[];
extern Vec3f D_800A42A0[];
extern void func_1503F404(void *matrix, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);

/* Gore for profile 0/7 actors. */
void func_1513A6E0(Actor *actor, f32 *origin, void *hit, u8 arg3, s32 arg4) {
    u8 idx;
    u8 i;
    u8 mask;
    Vec3f v;

    idx = func_150ADA20() % 6U;
    {
        ParticleSpawn spawn;
        u8 extra[4];
        s32 pad1; // matching: unused stack slot
        extra[0] = 12;
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.sizeXZ = spawn.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        spawn.rot.x = 0.0f;
        spawn.rot.y = 0.0f;
        spawn.rot.z = 0.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.rotVel.x = 0.0f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = 0.0f;
        spawn.flags = 0x101900;
        spawn.lifetime = 100;
        spawn.variant = 5;
        spawn.unk5C = 0;
        spawn.alpha = 255;
        spawn.updateFn = 0;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 5;
        spawn.unk6A = 1;
        spawn.owner = actor;
        spawn.ownerId = actor->unique_id;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (D_800A4278[idx] & mask) {
                s32 ret;
                spawn.model = D_800A4280[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
                if (ret != 0) {
                    memcpy((void *)(ret + 0x170), extra, 1);
                }
            }
        }
    }
    if (actor->unk1D4 != 0 && (actor->unk74 & 0xF) != 0xF) {
        Vec3f a;
        Vec3f b;
        ParticleSpawn spawn2;
        u8 mask2;
        u32 r1;
        u32 r2;
        func_1513A5E0((Vec3f *)origin, arg3 & 0xFF, arg4);
        {
            s32 q; // matching: unused stack slot
            func_1503F404(BONE_MATRIX(actor, 12), &a.x, &a.y, &a.z, &v.x, &v.y, &v.z, &b.x, &b.y, &b.z);
        }
        spawn2.radius = 1.0f;
        spawn2.elasticity = 0.6490000486f;
        spawn2.sizeXZ = spawn2.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        spawn2.rot = v;
        spawn2.scale.x = 1.0f;
        spawn2.scale.y = 0.0f;
        spawn2.scale.z = 1.0f;
        spawn2.rotVel.y = 0.0f;
        spawn2.variant = 0;
        spawn2.unk5C = 0;
        spawn2.alpha = 255;
        spawn2.update2Fn = 0;
        spawn2.floorType3Fn = 0;
        spawn2.wallHitFn = 0;
        spawn2.destroyFn = 0;
        spawn2.squashFn = 0;
        spawn2.unk6A = 1;
        spawn2.owner = actor;
        spawn2.ownerId = actor->unique_id;
        spawn2.fadeStart = 12;
        spawn2.fadeRate = 21;
        spawn2.flags = 0x1039E9;
        spawn2.updateFn = 13;
        spawn2.matrixFn = 2;
        spawn2.floorHitFn = 9;
        for (i = 0, mask2 = 1; i < 4; i++, mask2 <<= 1) {
            if (!(D_800A4278[idx] & mask2)) {
                spawn2.model = D_800A4290[i];
                func_15143134(&D_800A42A0[i], &spawn2.pos, BONE_MATRIX(actor, 12));
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_15143794(r1 & 0xFF, r2 % 35U - 84, func_150ADA68() * 6.119999886f + 3.0f, &spawn2.vel.x);
                spawn2.rotVel.x = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.rotVel.z = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.gravity = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, (s32)hit, 0, arg3, arg4);
            }
        }
    }
}

extern u8 D_800A42D0[];
extern s32 D_800A42D4[][2];
extern s32 D_800A42E4[][2];
extern Vec3f D_800A42F4[][2];

/* Gore for profile 8/9 actors (variant 1 for profile 9). */
void func_1513ABB8(Actor *actor, f32 *origin, u8 variant, void *hit, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        burst.rot.x = 0.0f;
        burst.rot.y = 0.0f;
        burst.rot.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.pos.x = 0.0f;
        burst.pos.y = 0.0f;
        burst.pos.z = 0.0f;
        burst.vel.x = 0.0f;
        burst.vel.y = 0.0f;
        burst.vel.z = 0.0f;
        burst.rotVel.x = 0.0f;
        burst.rotVel.y = 0.0f;
        burst.rotVel.z = 0.0f;
        burst.gravity = 0.0f;
        burst.flags = 0x101900;
        burst.lifetime = 100;
        burst.variant = 5;
        burst.unk5C = 0;
        burst.alpha = 255;
        burst.updateFn = 0;
        burst.update2Fn = 0;
        burst.floorHitFn = 0;
        burst.floorType3Fn = 0;
        burst.wallHitFn = 0;
        burst.destroyFn = 0;
        burst.squashFn = 0;
        burst.matrixFn = 5;
        burst.unk6A = 1;
        burst.owner = actor;
        burst.ownerId = actor->unique_id;
        burst.fadeStart = 12;
        burst.fadeRate = 21;
        for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
            if (!(D_800A42D0[index] & mask)) {
                continue;
            }
            burst.model = D_800A42D4[variant][i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy(&((BloodChunk *)result)->unk170, tag, 1);
            }
        }
    }
    if (actor->unk1D4 == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((Vec3f *)origin, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        ParticleSpawn packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404(BONE_MATRIX(actor, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.radius = 1.0f;
        packet.elasticity = 0.64900005f;
        packet.sizeXZ = packet.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
        packet.rot = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.rotVel.y = 0.0f;
        packet.variant = 0;
        packet.unk5C = 0;
        packet.alpha = 255;
        packet.update2Fn = 0;
        packet.floorType3Fn = 0;
        packet.wallHitFn = 0;
        packet.destroyFn = 0;
        packet.squashFn = 0;
        packet.unk6A = 1;
        packet.owner = actor;
        packet.ownerId = actor->unique_id;
        packet.fadeStart = 12;
        packet.fadeRate = 21;
        packet.flags = 0x1039E9;
        packet.updateFn = 13;
        packet.matrixFn = 2;
        packet.floorHitFn = 9;
        for (j = 0, bit = 1; j < 2; j++, bit <<= 1) {
            if (!(D_800A42D0[index] & bit)) {
                packet.model = D_800A42E4[variant][j];
                func_15143134(&D_800A42F4[variant][j], &packet.pos, BONE_MATRIX(actor, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 84), func_150ADA68() * 6.12f + 3.0f,
                              &packet.vel.x);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)hit, 0, arg4, arg5);
            }
        }
    }
}

/* BloodChunk message handler: message 0x45 (sent by func_1513A24C) counts unk170 down, then sets flag 0x80. */
void func_1513B0B8(BloodChunk *chunk, s32 arg1, u8 message) {
    s32 *counter = &chunk->unk170; // matching: IDO forms the address before the message test

    if (message == 0x45) {
        if (--(*counter) < 0) {
            chunk->flags |= 0x80;
        }
    }
}

void func_151036B4(void *, u8, s32);
extern u8 D_800A4324[];
extern s32 D_800A4328[];
extern s32 D_800A4330[];
extern f32 D_800A4338[][3];
/* Gore for profile 16 actors. */
void func_1513B0F8(Actor *actor, f32 *origin, void *hit, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f rotation;
    ParticleSpawn spawn;
    u8 i;
    u8 mask;
    u8 bit;
    u8 tag;
    u32 yaw;
    u32 pitch;
    f32 first[3];
    f32 second[3];
    ParticleSpawn drop;

    func_151036B4(origin, arg3, arg4);
    {
        u32 random = func_150ADA20();

        index = random % 3;
    }
    tag = 0xC;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeXZ = spawn.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.rotVel.x = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = 0.0f;
    spawn.flags = 0x101900;
    spawn.lifetime = 100;
    spawn.variant = 5;
    spawn.unk5C = 0;
    spawn.alpha = 255;
    spawn.updateFn = 0;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 5;
    spawn.unk6A = 1;
    spawn.owner = actor;
    spawn.ownerId = actor->unique_id;
    spawn.fadeStart = 12;
    spawn.fadeRate = 21;
    for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
        if (D_800A4324[index] & mask) {
            void *particle;

            spawn.model = D_800A4328[i];
            particle = (void *)func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (particle != NULL) {
                memcpy(&((BloodChunk *)particle)->unk170, &tag, 1);
            }
        }
    }
    if (actor->unk1D4 == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1503F404(BONE_MATRIX(actor, 12), &first[0], &first[1], &first[2],
                  &rotation.x, &rotation.y, &rotation.z,
                  &second[0], &second[1], &second[2]);
    drop.radius = 1.0f;
    drop.elasticity = 0.64900005f;
    drop.sizeXZ = drop.sizeY = (actor->xz_scale + actor->y_scale) * 0.5f;
    drop.rot = rotation;
    drop.scale.x = 1.0f;
    drop.scale.y = 2.0f;
    drop.scale.z = 1.0f;
    drop.rotVel.y = 0.0f;
    drop.variant = 0;
    drop.unk5C = 0;
    drop.alpha = 255;
    drop.update2Fn = 0;
    drop.floorType3Fn = 0;
    drop.wallHitFn = 0;
    drop.destroyFn = 0;
    drop.squashFn = 0;
    drop.unk6A = 1;
    drop.owner = actor;
    drop.ownerId = actor->unique_id;
    drop.fadeStart = 12;
    drop.fadeRate = 21;
    drop.flags = 0x1039E9;
    drop.updateFn = 13;
    drop.matrixFn = 2;
    drop.floorHitFn = 9;
    for (i = 0, bit = 1; i < 2; i++, bit <<= 1) {
        if (!(D_800A4324[index] & bit)) {
            u32 random;

            drop.model = D_800A4330[i];
            func_15143134(D_800A4338[i], &drop.pos, BONE_MATRIX(actor, 12));
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794((u8)yaw, (pitch % 35U) - 0x54, (func_150ADA68() * 6.12f) + 3.0f, &drop.vel.x);
            drop.rotVel.x = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.rotVel.z = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.gravity = (func_150ADA68() * 0.208f) + -0.708f;
            random = func_150ADA20();
            drop.lifetime = (random % 101U) + 0x3C;
            func_1513264C(&drop, 3, 0xFF, (s32)hit, 0, arg3, arg4);
        }
    }
}
