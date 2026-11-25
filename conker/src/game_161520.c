#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8proto
#define func_151D9B8C func_151D9B8C_s32proto
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

extern void func_15153F18(void *, void *, s32, u8, s32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_15143134(void *, s32, s32);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);
extern void func_151BC5A4(struct260 *, s32, u8);

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Per-model blood settings, indexed by func_15134070's profile index. */
typedef struct {
    Vec3f offset;           /* where blood comes from, relative to the actor's root bone */
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

/* Actor model ids (struct127::id) that have a blood profile. */
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
s32 func_15134070(struct127 *actor) {
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
    struct127 *owner;
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
    struct127 *owner;
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
            func_15143134(drip->offset, (s32)&drip->previous, (s32)BONE_MATRIX(owner, drip->bone));
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
        func_15143134(drip->offset, (s32)position, target);
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
    struct127 *owner;
    u8 pad20[0x1D];
    u8 unk3D;
} SubA_151347CC;

/*
 * Actor message payload. Messages 0, 3 and 0x11 carry an actor and (in the first byte of
 * the second word) its unique id; message 0x2D swaps actorA and actorB.
 */
typedef struct {
    struct127 *actorA;
    union {
        struct127 *actorB;
        u8 actorAId;
    } u4;
    u8 swapIdA;
    u8 swapIdB;
} BloodActorMsg;

/* Message handler: frees the effect when its owner goes away, follows owner handle swaps. */
void func_151347CC(void *arg0, BloodActorMsg *msg, u8 message) {
    SubA_151347CC *effect = arg0; // matching: copying the parameter keeps IDO's register choice in case 0x2D
    struct127 *actorA;

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
        if ((struct127 *)msg == effect->owner) {
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

void func_15134C98(struct260 *arg0, s32 arg1, u8 arg2) {
    if (((struct102 *)arg0)->unk28 == 1) {
        func_151BC5A4(arg0, arg1, arg2);
    }
}

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
    struct127 *owner;
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
    struct127 *owner;
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
            strip->owner = (struct127 *)actorA;
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
        memcpy((u8 *)result + 0x70, &motion, sizeof(motion));
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


/* A 2D blood splat particle spawned by func_151356D4. */
typedef struct {
    u8 pad0[0x10];
    Vec2f position;
    f32 width;
    f32 height;
    u8 pad20[0x50];
    Blood356D4Motion motion;
} BloodSplat;

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
void func_15136404(struct127 *actor, f32 radius, u8 arg2, s32 arg3) {
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
                func_151D9B8C(0, func_150ADA68() * 20.0f + 10.0f, func_150ADA20() % 101U + 100, hit.unk4, (struct17 *)&pos,
                              func_150ADA20() % 201U + 400, 1, 1, 0, arg2, arg3);
            }
            count -= 1.0f;
        }
    }
}

/* Spawn parameters passed to func_1513D594 by func_15136698. */
typedef struct {
    u8 texture;
    u8 pad1;
    s16 flags;
    s16 lifetime;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 width;
    f32 height;
    Vec3f unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 hasLifetime;
    u8 alpha;
    u8 unk45;
    u8 pad46[0x12];
} Struct15136698Local;

/* Spawns a blood particle of the given size with one of the four D_80089BAC textures. */
s32 func_15136698(f32 size, f32 arg1, u8 alpha, u8 arg3, s16 lifetime, s32 arg5, Vec3f *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10) {
    s32 particle;
    Struct15136698Local desc;
    f32 extra;
    Struct15136698Table textures;
    s32 sp64;
    s32 sp60;
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
    desc.flags = value + 0x300;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = 0;
    desc.unk11 = 0;
    desc.unk12 = 0;
    desc.unk13 = 255;
    desc.height = size;
    desc.width = size;
    desc.unk1C = *arg6;
    desc.hasLifetime = 0;
    desc.unk28 = 0.0f;
    desc.unk2C = 0.0f;
    desc.unk30 = 0.0f;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    if (lifetime == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = lifetime + 32;
        desc.hasLifetime = 1;
    }
    desc.alpha = alpha;
    desc.unk45 = arg3;
    if (arg8 != 0) {
        sp64 = 3;
        sp60 = 255;
    } else {
        sp64 = 0;
        sp60 = 0;
    }

    random0 = func_150ADA20();
    random1 = func_150ADA20();
    value = func_150ADA20();
    particle = func_1513D594((s32)&desc, 0, 0, 0x1A, 0, (random1 & 1) + (random0 & 1), value & 0xFF, 500.0f, 500.0f, 0, arg5, sp64, sp60, 0, 4, arg9, arg10);
    if (particle != 0) {
        memcpy(particle + 0x128, &extra, sizeof(extra));
    }
    return particle;
}

extern f32 D_800BE9A4;

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

void func_151D9014(void *a0, void *a1, s32 a2, f32 a3, s32 sp10, s32 sp14,
                     f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C,
                     s32 sp30, s32 sp34, s32 sp38, s32 sp3C);

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

s32 func_15134070();
void func_1504715C();
void func_1515C244(struct127 *, Vec3f *, f32 *, f32 *);
void func_1513A24C(Vec3f *, s32 *, f32 *, s32, f32, u8, u8, u8 *, void *,
                   u8, s32);
void func_15150D1C(Blood36C3CSpray *, u8, s32);
void func_151A9834(Vec3f *, f32, f32, GroundHit *, s32, u8, s8 *, u8, s32);
void func_151C61A0(s32, s32, u8, s32);
void func_1513A48C(void *, u8, s32);
extern s32 *D_80089A20[];
extern f32 D_800A3C30;
extern s32 D_800A3F14[];
extern s32 D_800A3F64[];


/*
 * Blood effects for a hit on actor: arg3 runs func_1513A24C (arg5 lowers its count), arg1
 * a spray, arg2 drops falling to the ground below, arg4 a splat at the actor's position.
 */
void func_15136C3C(struct127 *actor, u8 arg1, u8 arg2, u8 arg3, u8 arg4,
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

extern void func_15143874(s16, f32, f32 *, f32 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, f32 *, f32 *, f32 *, f32 *, f32 *, s32 *, s32, f32);
extern s32 func_15145C90(s32);
extern void func_151DC034(struct17 *, f32, s32, s16, u8, u8, s32);
s32 func_15137C64(struct17 *start, struct17 *end, struct17 *dir, struct17 *side, struct17 *up,
                  struct17 *from, struct17 *to, struct17 *direction);

/* Blood for an impact from `from` to `to`: a burst, a spray along the decalPos and wall decals. */
void func_15136F50(struct17 *from, struct17 *to, struct17 *direction, u8 arg3, u8 arg4, s32 arg5) {
    struct17 start;
    struct17 end;
    struct17 dir;
    struct17 side;
    struct17 up;
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
    struct17 decalPos;
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
            velocity.x = (dir.unk0 * 100.0f + side.unk0 * upOffset + up.unk0 * sideOffset) * randomA;
            velocity.y = (dir.unk4 * 100.0f + side.unk4 * upOffset + up.unk4 * sideOffset) * randomA;
            velocity.z = (dir.unk8 * 100.0f + side.unk8 * upOffset + up.unk8 * sideOffset) * randomA;
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
                    sparkVelocity.x = side.unk0 * sparkUp + up.unk0 * sparkSide;
                    sparkVelocity.y = side.unk4 * sparkUp + up.unk4 * sparkSide;
                    sparkVelocity.z = side.unk8 * sparkUp + up.unk8 * sparkSide;
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
                    if (func_150AC9C0(end.unk0, end.unk4, end.unk8,
                                      dir.unk0 * 100.0f + side.unk0 * decalUp + up.unk0 * decalSide,
                                      dir.unk4 * 100.0f + side.unk4 * decalUp + up.unk4 * decalSide,
                                      dir.unk8 * 100.0f + side.unk8 * decalUp + up.unk8 * decalSide,
                                      0, normal, &decalPos.unk0, &decalPos.unk4, &decalPos.unk8, &distance, &surface, 0, 0.0f) != 0 &&
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


void func_151C329C(struct17 *, u8, s32);
void func_151C577C(struct17 *, struct17 *, struct17 *, u8, u8, u8, u8, s32);
void func_151036B4(void *, u8, s32);

/* Picks the impact effect for a hit on actor (NULL: plain blood) from its model and state. */
void func_15137610(struct127 *actor, struct17 *from, struct17 *to,
                   struct17 *direction, u8 arg4, s32 arg5) {
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
void func_1513783C(struct127 *actor, u8 arg1, s32 arg2) {
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

s32 func_15137C64(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3,
                  struct17 *arg4, struct17 *arg5, struct17 *arg6, struct17 *arg7) {
    f32 sp2C;
    f32 sp28;

    if ((arg5 != NULL) && (arg6 != NULL)) {
        *arg0 = *arg5;
        *arg1 = *arg6;
    } else if (arg5 != NULL) {
        *arg0 = *arg5;
        *arg1 = *arg5;
    } else if (arg6 != NULL) {
        *arg0 = *arg6;
        *arg1 = *arg6;
    } else {
        return 0;
    }

    if (arg7 == NULL) {
        arg2->unk0 = arg1->unk0 - arg0->unk0;
        arg2->unk4 = arg1->unk4 - arg0->unk4;
        arg2->unk8 = arg1->unk8 - arg0->unk8;
        if (func_15145128(arg2, arg2, &sp2C, &sp28) == 0) {
            return 0;
        }
    } else {
        *arg2 = *arg7;
    }

    if (func_15146078(arg2, arg3, arg4) == 0) {
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
s32 func_151380B4(struct127 *actor, s32 kind, f32 *out) {
    s32 bones = (s32)actor->unk1D4;

    if (bones == 0) {
        return 0;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[kind].offset, (s32)out, bones + 12 * 0x40);
    return 1;
}

/* Starts a bleeding BloodStrip on the actor's bone 12 (or 1) playing sound 0x4FE. */
void func_15138120(struct127 *actor, s32 kind, u8 arg2) {
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
                                   600, 300, (s32)func_1000EBC4,
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

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    u16 unk56;
    s8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    s8 unk68;
    u8 unk69;
    s8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 unk78;
} Spawn138424;

extern void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
f32 func_150ADA68(void);

/*
 * Throws a func_1513264C chunk (model D_800A3FD8[kind].unkC) off the actor from position, and for
 * profiles 4-6 also the accessories it still wears (bits of unk94).
 */
void func_15138424(struct127 *actor, struct17 *position, s32 kind, void *arg3, u8 arg4, s32 arg5) {
    struct17 v;
    Spawn138424 spawn;
    struct17 a;
    struct17 b;

    func_1503F404((s32)BONE_MATRIX(actor, 12), &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
    spawn.unk1C = 1.0f;
    spawn.unk00 = 20.0f;
    spawn.unk04 = 0.5610000491f;
    if (D_800A3FD8[kind].unkE == 1) {
        spawn.unk20 = 1.0f;
    } else {
        spawn.unk20 = 0.0f;
    }
    if (D_800A3FD8[kind].unkE == 2) {
        spawn.unk20 = 2.0f;
    }
    spawn.unk24 = 1.0f;
    spawn.unk50 = 0x39E9;
    spawn.unk44 = 0.0f;
    spawn.unk54 = func_150ADA20() % 51U + 250;
    spawn.unk56 = D_800A3FD8[kind].unkC;
    spawn.unk0C = ((&D_800A40C8 == &D_800A3FD8[kind]) ? 0.6000000238f : 1.0f) * ((actor->xz_scale + actor->y_scale) * 0.5f);
    spawn.unk08 = spawn.unk0C;
    spawn.unk4C = (func_150ADA68() * 292.0f + -1099.0f) * 0.001000000047f;
    spawn.unk28 = *position;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 21U - 83), func_150ADA68() * 8.0f + 9.0f, &spawn.unk34.unk0);
    spawn.unk10 = v;
    spawn.unk40 = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.unk48 = (func_150ADA68() * 24496.0f + -12248.0f) * 0.001000000047f;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 255;
    spawn.unk61 = 13;
    spawn.unk62 = 0;
    spawn.unk63 = 8;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 2;
    spawn.unk6A = 1;
    spawn.unk6C = actor;
    spawn.unk70 = actor->unique_id;
    spawn.unk72 = 12;
    spawn.unk74 = 21;
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

            spawn.unk56 = ids2[i];
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
        }
    } else if (&D_800A4028 == &D_800A3FD8[kind]) {
        const s32 masks[4] = { 2, 8, 0x10, 0x20 };
        u8 i;
        const s32 ids[4] = { 0x87, 0x88, 0x89, 0x8A };

        for (i = 0; i < 4; i++) {
            if (!(masks[i] & actor->unk94)) {
                spawn.unk56 = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    } else if (&D_800A4038 == &D_800A3FD8[kind]) {
        const s32 masks[2] = { 2, 8 };
        u8 i;
        const s32 ids[2] = { 0x8D, 0x8B };

        for (i = 0; i < 2; i++) {
            if (!(masks[i] & actor->unk94)) {
                spawn.unk56 = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    }
}

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xC];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    s32 unk60;
} Struct151389A8;

s32 func_151389A8(Struct151389A8 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    struct17 pos;
    f32 damp;
    u32 r;
    f32 rf;
    s32 flag;

    damp = arg0->unk14;
    arg0->unk3C = arg0->unk10 + arg4;
    arg0->unk44 = arg0->unk44 * damp;
    arg0->unk48 = arg0->unk48 * -damp;
    arg0->unk4C = arg0->unk4C * damp;
    arg0->unk50 = arg0->unk50 * damp;
    arg0->unk54 = arg0->unk54 * damp;
    arg0->unk58 = arg0->unk58 * damp;
    if (fabsf(arg0->unk48) < 4.0f) {
        arg0->unk44 = 0.0f;
        arg0->unk60 &= ~0x69;
        arg0->unk48 = 0.0f;
        arg0->unk4C = 0.0f;
        arg0->unk50 = 0.0f;
        arg0->unk54 = 0.0f;
        arg0->unk58 = 0.0f;
        arg0->unk5C = 0.0f;
    }
    if (arg0->unk30 != 2.0f) {
        pos.unk0 = arg0->unk38;
        pos.unk4 = arg4;
        pos.unk8 = arg0->unk40;
        if (arg0->unk30) {
            flag = 1;
        } else {
            flag = 0;
        }
        rf = func_150ADA68();
        r = func_150ADA20();
        func_151D9B8C(flag, (rf * 25.39999962f + 39.79999924f) * ((arg0->unk18 + arg0->unk1C) * 0.5f),
                      (r % 101U) + 100, arg5, &pos, (func_150ADA20() % 101U) + 0x50, 1, 1, 0,
                      arg0->unkC, arg0->unk1);
        arg0->unk2C = 0.0f;
    }
    return 1;
}

/* Blood for a hit on the actor's bone-12 blood point: strip, burst and chunk. */
void func_15138BC0(struct127 *actor, u8 arg1, s32 arg2) {
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
            func_15138424(actor, (struct17 *)&position, kind, &hit, arg1, arg2);
        }
    }
}

void func_15138E98(struct127 *, f32 *, u8, void *, u8, s32);
struct Obj15139768;
struct ObjA6E0;
void func_15139768(struct Obj15139768 *, f32 *, f32, void *, u8, s32);
void func_15139D74(struct127 *, f32 *, void *, u8, s32);
void func_1513A6E0(struct ObjA6E0 *, f32 *, void *, u8, s32);
void func_1513ABB8(struct127 *, f32 *, u8, void *, u8, s32);
void func_1513B0F8(struct127 *, f32 *, void *, u8, s32);
void func_1513A594(void *, s32, s32, u8, s32);
void func_151036B4(void *, u8, s32);

void func_15138C80(struct127 *arg0, u8 arg1, s32 arg2) {
    s32 kind;
    Vec3f position;
    u8 valid;
    GroundHit hit;

    kind = func_15134070(arg0);
    if (kind != 0x63) {
        valid = func_151380B4(arg0, kind, &position.x);
        func_15138120(arg0, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, arg0);
            func_151382E0(&position.x, kind, &hit, arg1, arg2);
            switch (kind) {
            case 16:
                func_1513B0F8(arg0, &position.x, &hit, arg1, arg2);
                return;
            case 17:
                func_151036B4(&position.x, arg1, arg2);
                return;
            case 0:
            case 7:
                func_1513A6E0(arg0, &position.x, &hit, arg1, arg2);
                return;
            case 3:
                func_15138E98(arg0, &position.x, 1, &hit, arg1, arg2);
                return;
            case 5:
            case 6:
                func_15138E98(arg0, &position.x, 0, &hit, arg1, arg2);
                return;
            case 15:
                func_15139768(arg0, &position.x, 0.6f, &hit, arg1, arg2);
                return;
            case 2:
                func_15139768(arg0, &position.x, 1.0f, &hit, arg1, arg2);
                return;
            case 4:
                func_1513A594(arg0, (s32)&position.x, (s32)&hit, arg1, arg2);
                return;
            case 8:
            case 9:
                func_1513ABB8(arg0, &position.x, kind == 9, &hit, arg1, arg2);
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
                func_15139D74(arg0, &position.x, &hit, arg1, arg2);
                break;
            }
        }
    }
}

typedef struct BloodParticlePacket {
    f32 field0, field4, field8, fieldC;
    Vec3f angles, scale, position, velocity, field40;
    f32 field4C;
    s32 field50;
    s16 field54, field56;
    u8 field58, pad59[3];
    s32 field5C;
    u8 field60, field61, field62, field63;
    u8 field64, field65, field66, field67;
    u8 field68, pad69, field6A, pad6B;
    void *owner;
    u8 field70, pad71;
    s16 field72, field74;
    u8 pad76[2];
    s32 field78;
} BloodParticlePacket;

extern u8 D_800A4160[];
extern s32 D_800A4168[];
extern s32 D_800A4178[];
extern Vec3f D_800A4188[];
extern Vec3f D_800A41B8;
void func_1513A5E0(s32 arg0, s32 arg1, s32 arg2);

void func_15138E98(struct127 *arg0, f32 *arg1, u8 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 6U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 100;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->unique_id;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (!(D_800A4160[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A4168[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u8 j;
        u8 bit;
        BloodParticlePacket spray;
        u32 yaw;
        u32 pitch;

        func_1503F404((s32)BONE_MATRIX(arg0, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->unique_id;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 4; j++, bit <<= 1) {
            if (!(D_800A4160[index] & bit)) {
                packet.field56 = D_800A4178[j];
                func_15143134(&D_800A4188[j].x, (s32)&packet.position.x, (s32)BONE_MATRIX(arg0, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
        if (arg2 == 0) {
            return;
        }
        spray.field0 = 1.0f;
        spray.field4 = 0.64900005f;
        spray.field8 = spray.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        spray.angles = position;
        spray.scale.x = 1.0f;
        spray.scale.y = 1.0f;
        spray.scale.z = 1.0f;
        spray.field40.y = 0.0f;
        spray.field58 = 0;
        spray.field5C = 0;
        spray.field60 = 0xFF;
        spray.field62 = 0;
        spray.field64 = 0;
        spray.field65 = 0;
        spray.field66 = 0;
        spray.field67 = 0;
        spray.field6A = 1;
        spray.owner = arg0;
        spray.field70 = arg0->unique_id;
        spray.field72 = 0xC;
        spray.field74 = 0x15;
        spray.field50 = 0x1039E9;
        spray.field61 = 0xD;
        spray.field68 = 2;
        spray.field63 = 0xB;
        spray.field56 = 0x48;
        func_15143134(&D_800A41B8.x, (s32)&spray.position.x, (s32)BONE_MATRIX(arg0, 12));
        yaw = func_150ADA20();
        pitch = func_150ADA20();
        func_15143794((u8)yaw, (s16)(pitch % 16U - 0x3C), func_150ADA68() * 7.1499996f + 10.889999f,
                      &spray.velocity.x);
        spray.field40.x = func_150ADA68() * 48.158f + -24.079f;
        spray.field40.z = func_150ADA68() * 48.158f + -24.079f;
        spray.field4C = func_150ADA68() * 0.323f + -1.4210001f;
        spray.field54 = func_150ADA20() % 61U + 100;
        func_1513264C(&spray, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
    }
}

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x10];
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    s32 unk60;
} Struct15139578;

s32 func_15139578(Struct15139578 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    struct17 pos;
    f32 damp;
    u32 r;
    f32 rf;
    s32 flag;

    damp = arg0->unk14;
    arg0->unk3C = arg0->unk10 + arg4;
    arg0->unk44 = arg0->unk44 * damp;
    arg0->unk48 = arg0->unk48 * -damp;
    arg0->unk4C = arg0->unk4C * damp;
    arg0->unk50 = arg0->unk50 * damp;
    arg0->unk54 = arg0->unk54 * damp;
    arg0->unk58 = arg0->unk58 * damp;
    if (fabsf(arg0->unk48) < 4.0f) {
        arg0->unk44 = 0.0f;
        arg0->unk60 &= ~0x69;
        arg0->unk48 = 0.0f;
        arg0->unk4C = 0.0f;
        arg0->unk50 = 0.0f;
        arg0->unk54 = 0.0f;
        arg0->unk58 = 0.0f;
        arg0->unk5C = 0.0f;
    }
    if (arg0->unk30 != 2.0f) {
        pos.unk0 = arg0->unk38;
        pos.unk4 = arg4;
        pos.unk8 = arg0->unk40;
        if (arg0->unk30) {
            flag = 1;
        } else {
            flag = 0;
        }
        rf = func_150ADA68();
        r = func_150ADA20();
        func_151D9B8C(flag, (rf * 27.60000038f + 19.70000076f) * ((arg0->unk18 + arg0->unk1C) * 0.5f),
                      (r % 101U) + 100, arg5, &pos, (func_150ADA20() % 144U) + 0x50, 1, 1, 0,
                      arg0->unkC, arg0->unk1);
    }
    return 1;
}

typedef struct Obj15139768 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x14C - 0x75];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} Obj15139768;

extern u8 D_800A41C4[];
extern s32 D_800A41C8[];
extern s32 D_800A41D8[];
extern struct17 D_800A41E8[];
extern void func_151C5F44(s32, f32 *, f32, f32, s32, s32, f32, s32, s32, u8, s32);

void func_15139768(Obj15139768 *arg0, f32 *arg1, f32 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 idx;
    u8 i;
    u8 mask;
    struct17 v;

    idx = func_150ADA20() & 3;
    {
        Spawn138424 spawn;
        u8 kind[4];
        kind[0] = 0xC;
        spawn.unk00 = 1.0f;
        spawn.unk04 = 1.0f;
        spawn.unk08 = spawn.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f * arg2;
        spawn.unk10.unk0 = 0.0f;
        spawn.unk10.unk4 = 0.0f;
        spawn.unk10.unk8 = 0.0f;
        spawn.unk28.unk0 = 0.0f;
        spawn.unk28.unk4 = 0.0f;
        spawn.unk28.unk8 = 0.0f;
        spawn.unk34.unk0 = 0.0f;
        spawn.unk34.unk4 = 0.0f;
        spawn.unk34.unk8 = 0.0f;
        spawn.unk40 = 0.0f;
        spawn.unk44 = 0.0f;
        spawn.unk48 = 0.0f;
        spawn.unk4C = 0.0f;
        spawn.unk50 = 0x101900;
        spawn.unk54 = 0x64;
        spawn.unk58 = 5;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 0;
        spawn.unk62 = 0;
        spawn.unk63 = 0;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 5;
        spawn.unk6A = 1;
        spawn.unk6C = arg0;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk70 = arg0->unk3B;
        spawn.unk72 = 0xC;
        spawn.unk74 = 0x15;
        for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
            if (D_800A41C4[idx] & mask) {
                s32 ret;
                spawn.unk56 = D_800A41C8[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg4, arg5);
                if (ret != 0) {
                    memcpy((void *)(ret + 0x170), kind, 1);
                }
            }
        }
    }
    if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
        s32 count;
        u8 mask2;
        u8 j;
        struct17 dir;
        struct17 a;
        struct17 b;
        Spawn138424 spawn2;
        for (count = (func_150ADA20() & 3) + 5; count > 0; count--) {
            func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 71U - 60), func_150ADA68() * 6.0f + 6.0f, &dir.unk0);
            func_151C5F44((s32)arg1, &dir.unk0, func_150ADA68() * 0.1170000061f + -0.1970000118f, 0.9508649707f,
                          func_150ADA20() % 66U + 0xBE, func_150ADA20() % 45U + 0x2C, func_150ADA68() * 20.0f + 50.0f,
                          0, (s32)arg3, arg4, arg5);
        }
        {
            s32 q;
            func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
        }
        spawn2.unk00 = 1.0f;
        spawn2.unk04 = 0.6490000486f;
        spawn2.unk08 = spawn2.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f * arg2;
        spawn2.unk10 = v;
        spawn2.unk44 = 0.0f;
        spawn2.unk58 = 0;
        spawn2.unk5C = 0;
        spawn2.unk60 = 0xFF;
        spawn2.unk62 = 0;
        spawn2.unk64 = 0;
        spawn2.unk65 = 0;
        spawn2.unk66 = 0;
        spawn2.unk67 = 0;
        spawn2.unk6A = 1;
        spawn2.unk6C = arg0;
        spawn2.unk1C = 1.0f;
        spawn2.unk20 = 1.0f;
        spawn2.unk24 = 1.0f;
        spawn2.unk70 = arg0->unk3B;
        spawn2.unk72 = 0xC;
        spawn2.unk74 = 0x15;
        spawn2.unk50 = 0x1039E9;
        spawn2.unk61 = 0xD;
        spawn2.unk68 = 2;
        spawn2.unk63 = 9;
        for (j = 0, mask2 = 1; j < 4; j++, mask2 <<= 1) {
            if (!(D_800A41C4[idx] & mask2)) {
                spawn2.unk56 = D_800A41D8[j];
                func_15143134(&D_800A41E8[j], (s32)&spawn2.unk28, arg0->unk1D4 + 0x300);
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 35U - 0x54), func_150ADA68() * 6.119999886f + 3.0f, &spawn2.unk34.unk0);
                spawn2.unk40 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk48 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk4C = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.unk54 = func_150ADA20() % 101U + 0x3C;
                func_1513264C(&spawn2, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
    }
}

extern u8 D_800A4218[];
extern s32 D_800A421C[];
extern s32 D_800A4228[];
extern Vec3f D_800A4234[];

void func_15139D74(struct127 *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 0x190;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->unique_id;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 3; i++, mask <<= 1) {
            if (!(D_800A4218[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A421C[i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg3, arg4);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg3 & 0xFF, arg4);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404((s32)BONE_MATRIX(arg0, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->unique_id;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 3; j++, bit <<= 1) {
            if (!(D_800A4218[index] & bit)) {
                packet.field56 = D_800A4228[j];
                func_15143134(&D_800A4234[j].x, (s32)&packet.position.x, (s32)BONE_MATRIX(arg0, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
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

void func_1513A24C(Vec3f *arg0, s32 *arg1, f32 *arg2, s32 arg3, f32 arg4, u8 arg5, u8 arg6, u8 *arg7, void *arg8, u8 arg9, s32 argA) {
    Cfg080C_1513A24C a;
    Extra080C_1513A24C b;
    s32 flag;

    a.unk0 = 0x28;
    a.unk4 = 0;
    a.pos = *arg0;
    a.unk14 = 0;
    a.unk16 = 0xFF;
    a.unk2C = 0x1E0;
    a.unk2E = 0xF0;
    a.unk30 = 1 * arg4;
    a.unk34 = 0 * arg4;
    a.unk40 = arg6;
    b.unk0 = 4;
    flag = -1;
    a.unk18 = -0x40;
    a.unk1A = 0xC;
    a.unk20 = 5.0f;
    a.unk3C = 1.0f;
    a.unk1C = 13.0f;
    a.unk24 = -1.007000089f;
    a.unk28 = 0.2520000041f;
    a.unk38 = 16.10200119f;
    switch (arg5) {
    case 0x13:
        if (D_800BE9F0 == 0x3C) {
            a.unk3C = 0.0f;
        }
        break;
    case 0xD:
        a.unk18 = -0x2E;
        a.unk1A = 0x14;
        a.unk1C = 15.0f;
        a.unk20 = 8.0f;
        a.unk24 = -1.262000084f;
        a.unk28 = 0.4900000095f;
        a.unk38 = 12.91100025f;
        break;
    case 1: {
        const u16 tbl[4] = { 0x83, 0x81, 0x82, 0x84 };
        s32 idx;
        idx = arg7[0x128];
        if (idx >= 4) {
            idx = 3;
        }
        b.unk4 = tbl[idx];
        b.unk6 = 6;
        b.unk8 = 3;
        flag = 1;
        break;
    }
    }
    func_1515080C(&a, arg1, arg2, arg3, &b, 0xC, 6, 1, flag, arg8, 5.0f, 1, D_800BE616 ? 1 : 0, arg9, argA);
    func_15133E3C(0, 0x45);
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_A48C;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_A48C position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA48CConfig;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4260[];
extern u8 D_800A4264[];

void func_1513A48C(void *arg0, u8 arg1, s32 arg2) {
    BloodA48CConfig config;

    config.field_0 = 8;
    config.field_4 = 4;
    config.position = *(Vec3i_A48C *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x37;
    config.field_1A = 0x20;
    config.field_2C = 0x28;
    config.field_2E = 0x14;
    config.field_30 = 0.07f;
    config.field_34 = 0.07f;
    config.field_1C = 10.0f;
    config.field_20 = 9.0f;
    config.field_24 = -1.0040001f;
    config.field_28 = 0.254f;
    config.field_38 = 23.446001f;
    func_15152190(&config, D_800A4260, D_800A4264, 1, 0.0f, 1,
                  (s32)arg1, arg2);
}

void func_1513A594(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    func_1513A5E0(arg1, arg3 & 0xFF, arg4);
    if (*(s32 *)((u8 *)arg0 + 0x1D4) != 0) {
        if (arg0) {
        }
    }
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_1513A5E0;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_1513A5E0 position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA5E0Config;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4268[];
extern u8 D_800A4270[];

void func_1513A5E0(s32 arg0, s32 arg1, s32 arg2) {
    BloodA5E0Config config;

    config.field_0 = 7;
    config.field_4 = 7;
    config.position = *(Vec3i_1513A5E0 *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x32;
    config.field_1A = 0x1B;
    config.field_1C = 4.0f;
    config.field_20 = 4.0f;
    config.field_2C = 0x19;
    config.field_2E = 0x28;
    config.field_30 = 0.0201f;
    config.field_34 = 0.0201f;
    config.field_24 = -0.43f;
    config.field_28 = 0.272f;
    config.field_38 = 20.187f;
    func_15152190(&config, D_800A4268, D_800A4270, 2, 0.0f, 1,
                  (u8)arg1, arg2);
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    s8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    s8 unk68;
    u8 unk69;
    s8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 unk78;
} SpawnA6E0;

typedef struct ObjA6E0 {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x14C - 0x75];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} ObjA6E0;

extern u8 D_800A4278[];
extern s32 D_800A4280[];
extern s32 D_800A4290[];
extern struct17 D_800A42A0[];
extern void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
f32 func_150ADA68(void);

void func_1513A6E0(ObjA6E0 *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 idx;
    u8 i;
    u8 mask;
    struct17 v;

    idx = func_150ADA20() % 6U;
    {
    SpawnA6E0 spawn;
    u8 extra[4];
    s32 pad1;
    extra[0] = 0xC;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk08 = spawn.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
    spawn.unk10.unk0 = 0.0f;
    spawn.unk10.unk4 = 0.0f;
    spawn.unk10.unk8 = 0.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk28.unk0 = 0.0f;
    spawn.unk28.unk4 = 0.0f;
    spawn.unk28.unk8 = 0.0f;
    spawn.unk34.unk0 = 0.0f;
    spawn.unk34.unk4 = 0.0f;
    spawn.unk34.unk8 = 0.0f;
    spawn.unk40 = 0.0f;
    spawn.unk44 = 0.0f;
    spawn.unk48 = 0.0f;
    spawn.unk4C = 0.0f;
    spawn.unk50 = 0x101900;
    spawn.unk54 = 0x64;
    spawn.unk58 = 5;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 5;
    spawn.unk6A = 1;
    spawn.unk6C = arg0;
    spawn.unk70 = arg0->unk3B;
    spawn.unk72 = 0xC;
    spawn.unk74 = 0x15;
    for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
        if (D_800A4278[idx] & mask) {
            s32 ret;
            spawn.unk56 = D_800A4280[i];
            ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (ret != 0) {
                memcpy((void *)(ret + 0x170), extra, 1);
            }
        }
    }
    }
    if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
        struct17 a;
        struct17 b;
        SpawnA6E0 spawn2;
        u8 mask2;
        u32 r1;
        u32 r2;
        func_1513A5E0((s32)arg1, arg3 & 0xFF, arg4);
        {
        s32 q;
        func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
        }
        spawn2.unk00 = 1.0f;
        spawn2.unk04 = 0.6490000486f;
        spawn2.unk08 = spawn2.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
        spawn2.unk10 = v;
        spawn2.unk1C = 1.0f;
        spawn2.unk20 = 0.0f;
        spawn2.unk24 = 1.0f;
        spawn2.unk44 = 0.0f;
        spawn2.unk58 = 0;
        spawn2.unk5C = 0;
        spawn2.unk60 = 0xFF;
        spawn2.unk62 = 0;
        spawn2.unk64 = 0;
        spawn2.unk65 = 0;
        spawn2.unk66 = 0;
        spawn2.unk67 = 0;
        spawn2.unk6A = 1;
        spawn2.unk6C = arg0;
        spawn2.unk70 = arg0->unk3B;
        spawn2.unk72 = 0xC;
        spawn2.unk74 = 0x15;
        spawn2.unk50 = 0x1039E9;
        spawn2.unk61 = 0xD;
        spawn2.unk68 = 2;
        spawn2.unk63 = 9;
        for (i = 0, mask2 = 1; i < 4; i++, mask2 <<= 1) {
            if (!(D_800A4278[idx] & mask2)) {
                spawn2.unk56 = D_800A4290[i];
                func_15143134(&D_800A42A0[i], (s32)&spawn2.unk28, arg0->unk1D4 + 0x300);
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_15143794(r1 & 0xFF, r2 % 35U - 84, func_150ADA68() * 6.119999886f + 3.0f, &spawn2.unk34.unk0);
                spawn2.unk40 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk48 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk4C = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.unk54 = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
            }
        }
    }
}

extern u8 D_800A42D0[];
extern s32 D_800A42D4[][2];
extern s32 D_800A42E4[][2];
extern Vec3f D_800A42F4[][2];

void func_1513ABB8(struct127 *arg0, f32 *arg1, u8 arg2, void *arg3, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        BloodParticlePacket burst;
        u8 tag[1];
        s32 result;
        u8 i;
        u8 mask;

        tag[0] = 0xC;
        burst.field0 = 1.0f;
        burst.field4 = 1.0f;
        burst.field8 = burst.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        burst.angles.x = 0.0f;
        burst.angles.y = 0.0f;
        burst.angles.z = 0.0f;
        burst.scale.x = 1.0f;
        burst.scale.y = 1.0f;
        burst.scale.z = 1.0f;
        burst.position.x = 0.0f;
        burst.position.y = 0.0f;
        burst.position.z = 0.0f;
        burst.velocity.x = 0.0f;
        burst.velocity.y = 0.0f;
        burst.velocity.z = 0.0f;
        burst.field40.x = 0.0f;
        burst.field40.y = 0.0f;
        burst.field40.z = 0.0f;
        burst.field4C = 0.0f;
        burst.field50 = 0x101900;
        burst.field54 = 0x64;
        burst.field58 = 5;
        burst.field5C = 0;
        burst.field60 = 0xFF;
        burst.field61 = 0;
        burst.field62 = 0;
        burst.field63 = 0;
        burst.field64 = 0;
        burst.field65 = 0;
        burst.field66 = 0;
        burst.field67 = 0;
        burst.field68 = 5;
        burst.field6A = 1;
        burst.owner = arg0;
        burst.field70 = arg0->unique_id;
        burst.field72 = 0xC;
        burst.field74 = 0x15;
        for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
            if (!(D_800A42D0[index] & mask)) {
                continue;
            }
            burst.field56 = D_800A42D4[arg2][i];
            result = func_15132A4C(&burst, 3, 0xFF, 1, arg4, arg5);
            if (result != 0) {
                memcpy((u8 *)result + 0x170, tag, 1);
            }
        }
    }
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0((s32)arg1, arg4 & 0xFF, arg5);
    {
        f32 first[3];
        f32 second[3];
        BloodParticlePacket packet;
        u32 yaw;
        u32 pitch;
        u8 j;
        u8 bit;

        func_1503F404((s32)BONE_MATRIX(arg0, 12), &first[0], &first[1], &first[2],
                      &position.x, &position.y, &position.z,
                      &second[0], &second[1], &second[2]);
        packet.field0 = 1.0f;
        packet.field4 = 0.64900005f;
        packet.field8 = packet.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        packet.angles = position;
        packet.scale.x = 1.0f;
        packet.scale.y = 0.0f;
        packet.scale.z = 1.0f;
        packet.field40.y = 0.0f;
        packet.field58 = 0;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field62 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field6A = 1;
        packet.owner = arg0;
        packet.field70 = arg0->unique_id;
        packet.field72 = 0xC;
        packet.field74 = 0x15;
        packet.field50 = 0x1039E9;
        packet.field61 = 0xD;
        packet.field68 = 2;
        packet.field63 = 9;
        for (j = 0, bit = 1; j < 2; j++, bit <<= 1) {
            if (!(D_800A42D0[index] & bit)) {
                packet.field56 = D_800A42E4[arg2][j];
                func_15143134(&D_800A42F4[arg2][j].x, (s32)&packet.position.x, (s32)BONE_MATRIX(arg0, 12));
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794((u8)yaw, (s16)(pitch % 35U - 0x54), func_150ADA68() * 6.12f + 3.0f,
                              &packet.velocity.x);
                packet.field40.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field40.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.field4C = func_150ADA68() * 0.208f + -0.708f;
                packet.field54 = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, (s32)arg3, 0, arg4, arg5);
            }
        }
    }
}

void func_1513B0B8(void *arg0, s32 arg1, u8 arg2) {
    s32 *temp_v0 = (s32 *)((s32)arg0 + 0x170);

    if (arg2 == 0x45) {
        if (--(*temp_v0) < 0) {
            *(s32 *)((s32)arg0 + 0x60) |= 0x80;
        }
    }
}

void func_151036B4(void *, u8, s32);
extern u8 D_800A4324[];
extern s32 D_800A4328[];
extern s32 D_800A4330[];
extern f32 D_800A4338[][3];
void func_1513B0F8(struct127 *arg0, f32 *arg1, void *arg2, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f rotation;
    BloodParticlePacket spawn;
    u8 i;
    u8 mask;
    u8 bit;
    u8 tag;
    u32 yaw;
    u32 pitch;
    f32 first[3];
    f32 second[3];
    BloodParticlePacket drop;

    func_151036B4(arg1, arg3, arg4);
    {
        u32 random = func_150ADA20();

        index = random % 3;
    }
    tag = 0xC;
    spawn.field0 = 1.0f;
    spawn.field4 = 1.0f;
    spawn.field8 = spawn.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    spawn.angles.x = 0.0f;
    spawn.angles.y = 0.0f;
    spawn.angles.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.position.x = 0.0f;
    spawn.position.y = 0.0f;
    spawn.position.z = 0.0f;
    spawn.velocity.x = 0.0f;
    spawn.velocity.y = 0.0f;
    spawn.velocity.z = 0.0f;
    spawn.field40.x = 0.0f;
    spawn.field40.y = 0.0f;
    spawn.field40.z = 0.0f;
    spawn.field4C = 0.0f;
    spawn.field50 = 0x101900;
    spawn.field54 = 100;
    spawn.field58 = 5;
    spawn.field5C = 0;
    spawn.field60 = 0xFF;
    spawn.field61 = 0;
    spawn.field62 = 0;
    spawn.field63 = 0;
    spawn.field64 = 0;
    spawn.field65 = 0;
    spawn.field66 = 0;
    spawn.field67 = 0;
    spawn.field68 = 5;
    spawn.field6A = 1;
    spawn.owner = arg0;
    spawn.field70 = arg0->unique_id;
    spawn.field72 = 0xC;
    spawn.field74 = 0x15;
    for (i = 0, mask = 1; i < 2; i++, mask <<= 1) {
        if (D_800A4324[index] & mask) {
            void *particle;

            spawn.field56 = D_800A4328[i];
            particle = (void *)func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0x170, &tag, 1);
            }
        }
    }
    if (arg0->unk1D4 == NULL) {
        return;
    }
    if ((arg0->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1503F404((s32)BONE_MATRIX(arg0, 12), &first[0], &first[1], &first[2],
                  &rotation.x, &rotation.y, &rotation.z,
                  &second[0], &second[1], &second[2]);
    drop.field0 = 1.0f;
    drop.field4 = 0.64900005f;
    drop.field8 = drop.fieldC = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    drop.angles = rotation;
    drop.scale.x = 1.0f;
    drop.scale.y = 2.0f;
    drop.scale.z = 1.0f;
    drop.field40.y = 0.0f;
    drop.field58 = 0;
    drop.field5C = 0;
    drop.field60 = 0xFF;
    drop.field62 = 0;
    drop.field64 = 0;
    drop.field65 = 0;
    drop.field66 = 0;
    drop.field67 = 0;
    drop.field6A = 1;
    drop.owner = arg0;
    drop.field70 = arg0->unique_id;
    drop.field72 = 0xC;
    drop.field74 = 0x15;
    drop.field50 = 0x1039E9;
    drop.field61 = 0xD;
    drop.field68 = 2;
    drop.field63 = 9;
    for (i = 0, bit = 1; i < 2; i++, bit <<= 1) {
        if (!(D_800A4324[index] & bit)) {
            u32 random;

            drop.field56 = D_800A4330[i];
            func_15143134(D_800A4338[i], (s32)&drop.position.x, (s32)BONE_MATRIX(arg0, 12));
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794((u8)yaw, (pitch % 35U) - 0x54, (func_150ADA68() * 6.12f) + 3.0f, &drop.velocity.x);
            drop.field40.x = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.field40.z = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.field4C = (func_150ADA68() * 0.208f) + -0.708f;
            random = func_150ADA20();
            drop.field54 = (random % 101U) + 0x3C;
            func_1513264C(&drop, 3, 0xFF, (s32)arg2, 0, arg3, arg4);
        }
    }
}
