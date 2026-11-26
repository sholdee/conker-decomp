#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Scale factors passed to func_1513A24C (all 1.0f). */
const f32 D_800A3C30[15] = {
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
};

/* Per-profile blood model lists; the .data table D_80089A20[profile] points at one of them. */
const s32 D_800A3C6C[10] = { 2, 3, 0x3A, 0x3B, 0x3B, 0x3D, 0x3D, 0x3E, 0x3C, 0x3C };
const s32 D_800A3C94[8] = { 2, 3, 0x30, 0x92, 0x92, 0x91, 0x31, 0x31 };
const s32 D_800A3CB4[8] = { 2, 3, 0x44, 0x46, 0x46, 0x47, 0x45, 0x45 };
const s32 D_800A3CD4[8] = { 2, 3, 0x49, 0x49, 0x4B, 0x4C, 0x4A, 0x4A };
const s32 D_800A3CF4[8] = { 2, 3, 0x81, 0x80, 0x7F, 0x7F, 0x7E, 0x7E };
const s32 D_800A3D14[8] = { 2, 3, 0xB2, 0xA0, 0xA0, 0xA1, 0x9F, 0x9F };
const s32 D_800A3D34[8] = { 2, 3, 0xB2, 0x9D, 0x9D, 0x9E, 0x9C, 0x9C };
const s32 D_800A3D54[10] = { 2, 3, 0x3A, 0x8E, 0x8E, 0x3D, 0x3D, 0x8F, 0x90, 0x90 };
const s32 D_800A3D7C[8] = { 2, 3, 0xBB, 0xAC, 0xAC, 0xAE, 0xAD, 0xAD };
const s32 D_800A3D9C[8] = { 2, 3, 0xBC, 0xAF, 0xAF, 0xB1, 0xB0, 0xB0 };
const s32 D_800A3DBC[12] = { 0x95, 0x95, 0x96, 0x97, 0x95, 0x95, 0x2D, 0x2E, 0x2D, 0x2E, 0x95, 0x95 };
const s32 D_800A3DEC[8] = { 2, 3, 0x98, 0x99, 0x99, 0x9B, 0x9A, 0x9A };
const s32 D_800A3E0C[8] = { 2, 3, 0xC0, 0xC0, 0xC2, 0xC3, 0xC1, 0xC1 };
const s32 D_800A3E2C[7] = { 0xE7, 0xE7, 0xD8, 0xD8, 0xE7, 0xD7, 0xD7 };
const s32 D_800A3E48[10] = { 2, 3, 0xDC, 0xDC, 0xDB, 0xDB, 0xDB, 0xDA, 0xD9, 0xD9 };
const s32 D_800A3E70[8] = { 2, 3, 0xDE, 0xDE, 0xE8, 0x44, 0xDD, 0xDD };
const s32 D_800A3E90[9] = { 0xE2, 0xE2, 0xE1, 0xE1, 0xDF, 0xE3, 0xE5, 0xE0, 0xE0 };
const s32 D_800A3EB4[8] = { 0xE2, 0xE2, 0xD5, 0xD5, 0xD3, 0xD6, 0xD4, 0xD4 };
const s32 D_800A3ED4[7] = { 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE };
const s32 D_800A3EF0[9] = { 2, 3, 0x32, 0x17, 0x17, 0x33, 0x33, 0x1A, 0x1A };

/* Per-profile counts for func_1513A24C: entries of the D_80089A20[profile] list, and its last argument. */
const s32 D_800A3F14[20] = {
    10, 8, 8, 8, 8, 8, 8, 10, 8, 8,
    12, 8, 8, 7, 10, 8, 9, 8, 7, 9,
};
const s32 D_800A3F64[20] = {
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    12, 11, 11, 13, 11, 11, 11, 11, 10, 11,
};


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

/* An actor's bone matrix (ob_matrix_ptr holds 0x40-byte matrices). */
#define BONE_MATRIX(actor, bone) ((Mtx *)((u8 *)(actor)->ob_matrix_ptr + (bone) * 0x40))

/* Returns the blood profile index (0-19) for an actor's model, or BLOOD_PROFILE_NONE. */
s32 func_15134070(Actor *actor) {
    switch (actor->character_type) {
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

/* Particle callback: over the last 32 frames, sets rgba[0] from the remaining lifetime. */
s32 func_1513416C(DefParticleHead *particle) {
    s16 value = particle->def.lifetime;

    if (value < 32) {
        particle->def.rgba[0] = value * 8;
    }
    return 1;
}


/* Spawns a BloodDrip effect from a template. */
BloodDrip *func_1513418C(BloodDripDesc *desc, s32 arg1, u8 arg2, s32 arg3) {
    BloodDrip *drip;
    Actor *owner;
    u8 flags;

    drip = func_15167A68(0x28, arg3, arg1 + 0x58, 1, arg2, 1);
    if (drip == NULL) {
        return NULL;
    }
    memcpy(&drip->desc, desc, sizeof(BloodDripDesc));
    flags = drip->desc.flags;
    if (flags & BLOOD_DRIP_ON_BONE) {
        owner = drip->desc.owner;
        if ((owner->type == 0) || (owner->unique_id != drip->desc.ownerId)) {
            func_1516972C(&drip->header);
            return NULL;
        }
        if ((owner->ob_matrix_ptr != 0) && ((owner->unk74 & 0xF) != 0xF)) {
            func_15143134(&drip->desc.offset, &drip->previous, BONE_MATRIX(owner, drip->desc.bone));
        } else {
            drip->desc.flags = flags | BLOOD_DRIP_NO_PREVIOUS;
        }
    } else {
        drip->desc.flags = flags | (BLOOD_DRIP_NO_PREVIOUS | BLOOD_DRIP_STATIC);
    }
    drip->inverseDiameter = 1.0f / (drip->desc.radius + drip->desc.radius);
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
    Mtx *target;
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
    if (drip->desc.flags & BLOOD_DRIP_LIFETIME) {
        drip->desc.lifetime -= D_800BE9E4;
        if (drip->desc.lifetime < 0) {
            dead = 1;
        }
    }
    flags = drip->desc.flags;
    source = (flags & BLOOD_DRIP_NO_PREVIOUS) ? position : &drip->previous.x;
    if (flags & BLOOD_DRIP_FOLLOW_OWNER) {
        position[0] = drip->desc.owner->xpos;
        position[1] = drip->desc.owner->ypos;
        position[2] = drip->desc.owner->zpos;
    } else {
        if (flags & BLOOD_DRIP_ON_BONE) {
            if ((drip->desc.owner->type == 0) || (drip->desc.owner->type == 8) ||
                (drip->desc.owner->unique_id != drip->desc.ownerId)) {
                func_1516972C(&drip->header);
                return;
            }
            bones = (u8 *)drip->desc.owner->ob_matrix_ptr;
            if ((bones == NULL) || ((drip->desc.owner->unk74 & 0xF) == 0xF)) {
                drip->desc.flags = flags | BLOOD_DRIP_NO_PREVIOUS;
                return;
            }
            target = (Mtx *)bones + drip->desc.bone;
        } else if (!(flags & BLOOD_DRIP_STATIC)) {
            index = D_800BE9C0 ? 0 : 1;
            target = drip->desc.target[index];
        } else {
            return;
        }
        // matching: position must be an f32[3] (a Vec3f local changes the float register allocation)
        func_15143134(&drip->desc.offset, (Vec3f *)position, target);
    }
    deltaX = source[0] - position[0];
    deltaY = source[1] - position[1];
    deltaZ = source[2] - position[2];
    distance = sqrtf(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    scale = drip->inverseDiameter * distance + 1.0f;
    drip->accumulator += scale * D_800BE9A4 * drip->desc.rate;
    if (drip->accumulator > 7.0f) {
        drip->accumulator = 7.0f;
    }
    if ((drip->desc.flags & 0x20) && (D_800C35EA[0] == 1)) {
        while (drip->accumulator > 1.0f) {
            drip->accumulator -= 1.0f;
        }
    } else {
        while (drip->accumulator > 1.0f) {
            f32 diameter;
            f32 fraction;

            diameter = drip->desc.radius + drip->desc.radius;
            fraction = func_150ADA68();
            if (D_80089A70[drip->desc.emitter] != NULL) {
                randomX = func_150ADA68();
                randomY = func_150ADA68();
                D_80089A70[drip->desc.emitter](
                    (position[0] + deltaX * fraction + drip->desc.radius) - randomX * diameter,
                    (position[1] + deltaY * fraction + drip->desc.radius) - randomY * diameter,
                    (position[2] + deltaZ * fraction + drip->desc.radius) - func_150ADA68() * diameter,
                    deltaX, deltaY, deltaZ, drip);
            }
            drip->accumulator -= 1.0f;
        }
    }
    drip->previous = *(Vec3f *)position;
    drip->desc.flags &= ~BLOOD_DRIP_NO_PREVIOUS;
    if (!dead) {
        if ((drip->desc.callback != -1) && (D_80089AA0[drip->desc.callback](drip) == 0)) {
            dead = 1;
        }
    }
    if (dead) {
        func_1516972C(&drip->header);
    }
}

/* BloodDrip draw hook: clears BLOOD_DRIP_STATIC. */
s32 func_151346D0(s32 arg0, BloodDrip *drip, s32 arg2) {
    drip->desc.flags &= ~BLOOD_DRIP_STATIC;
    return arg0;
}

void func_151346EC(EffectHeader *effect) {
    func_15169804(effect);
}

void func_1513470C(EffectHeader *effect) {
    func_15169824(effect);
}

/* Calls the D_80089AAC handler selected by drip->desc.unk2D (0 when out of range). */
void func_1513472C(BloodDrip *drip) {
    s32 index = drip->desc.unk2D;

    if (index < 0) {
        index = 0;
    }
    if (index >= 10) {
        index = 0;
    }
    D_80089AAC[index]();
}

/* Calls the D_80089AD4 handler selected by drip->desc.unk2D (0 when out of range). */
void func_1513477C(BloodDrip *drip) {
    s32 index = drip->desc.unk2D;

    if (index < 0) {
        index = 0;
    }
    if (index >= 10) {
        index = 0;
    }
    D_80089AD4[index]();
}

typedef struct {
    EffectHeader header;
    u8 pad10[0x8];
    u8 ownerId;
    u8 pad19[0x3];
    Actor *owner;
    u8 pad20[0x1D];
    u8 unk3D;
} SubA_151347CC;

/* Message handler: frees the effect when its owner goes away, follows owner handle swaps. */
void func_151347CC(void *arg0, ActorMsg *msg, u8 message) {
    SubA_151347CC *effect = arg0; // matching: copying the parameter keeps IDO's register choice in case 0x2D
    Actor *actorA;

    if ((message == 0) || (message == 3)) {
        if ((msg->ref.actor == effect->owner) || (msg->ref.uniqueId == effect->ownerId)) {
            func_1516972C(&effect->header);
        }
    } else if (message == 0x11) {
        if (effect->unk3D == 5) {
            if ((msg->ref.actor == effect->owner) || (msg->ref.uniqueId == effect->ownerId)) {
                func_1516972C(&effect->header);
            }
        }
    } else if (message == 0x16) {
        if ((Actor *)msg == effect->owner) {
            func_1516972C(&effect->header);
        }
    } else if (message == 0x2D) {
        actorA = msg->swap.actorA;
        if (effect->owner == actorA) {
            effect->owner = msg->swap.actorB;
            effect->ownerId = msg->swap.idB;
        } else if (effect->owner == msg->swap.actorB) {
            effect->owner = actorA;
            effect->ownerId = msg->swap.idA;
        }
    }
}

/* Empty callback. */
void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

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
            func_1516972C(&trail->header);
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
            func_1516972C(&trail->header);
        }
    }
}

/* Message handler: forwards to func_151BC5A4 while the effect's unk28 is 1. */
void func_15134C98(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId) {
    if (effect->unk28 == 1) {
        func_151BC5A4(effect, msg, msgId);
    }
}

/* Empty callback. */
void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

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

/* Per-frame update of a BloodStrip. */
void func_15134E48(BloodStrip *arg0) {
    BloodStrip *strip = arg0; // matching: the parameter copy reserves the stack slot IDO kept
    u8 kill;
    Mtx *boneMatrix;

    kill = 0;
    if (strip->owner->type == 0 || strip->owner->unique_id != strip->ownerId) {
        kill = 1;
    }
    if (strip->active & 1) {
        if (!kill && strip->owner->ob_matrix_ptr != 0 && (strip->owner->unk74 & 0xF) != 0xF) {
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
        func_1516972C(&strip->header);
    }
}

void func_151352EC(EffectHeader *effect) {
    func_15169804(effect);
}

void func_1513530C(EffectHeader *effect) {
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

/* Transforms inA and inB by matrix into outA and outB. */
void func_15135424(void *matrix, Vec3f *inA, Vec3f *inB, Vec3f *outA, Vec3f *outB) {
    const Vec3f *in[2];
    Vec3f *out[2];

    in[0] = inA;
    in[1] = inB;
    out[0] = outA;
    out[1] = outB;
    func_15145EA4(in, out, matrix, 2);
}

/* BloodStrip message handler: follows owner swaps, then dispatches on strip->mode. */
void func_15135480(BloodStrip *strip, ActorMsg *msg, u8 message) {
    s32 actorA;
    s32 owner;

    if (message == 0x2D) {
        actorA = (s32)msg->swap.actorA;
        owner = (s32)strip->owner;
        if (actorA == owner) {
            strip->owner = msg->swap.actorB;
            strip->ownerId = msg->swap.idB;
        } else if ((owner | 0) == (s32)msg->swap.actorB) { // matching: `| 0` makes IDO copy owner, fixing operand order
            strip->owner = (Actor *)actorA;
            strip->ownerId = msg->swap.idA;
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
            if ((s32)msg->ref.actor == ((owner = (s32)strip->owner) | 0) || strip->ownerId == msg->ref.uniqueId) { // matching: as above
                func_1516972C(&strip->header);
            }
        }
        break;
    }
}

/* Mode 2 handler: frees the strip when its owner is removed (messages 0 and 0x12). */
void func_1513555C(BloodStrip *strip, ActorMsg *msg, u8 message) {
    if (message == 0 || message == 0x12) {
        if (strip->owner == msg->ref.actor || strip->ownerId == msg->ref.uniqueId) {
            func_1516972C(&strip->header);
        }
    }
}

/* Mode 1 handler: frees the strip on message 0, stops it on message 3, for its owner. */
void func_151355B8(BloodStrip *strip, ActorMsg *msg, u8 message) {
    switch (message) {
    case 0:
        if (strip->owner == msg->ref.actor || strip->ownerId == msg->ref.uniqueId) {
            func_1516972C(&strip->header);
        }
        break;
    case 3:
        if (strip->owner == msg->ref.actor || strip->ownerId == msg->ref.uniqueId) {
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
    return func_151422DC(0, "power", 0, 2000, 1000, "../Effects/Blood/blood.c", 2938) * 0.001f;
}

/*
 * Blood settings for each profile, read as one table D_800A3FD8[kind] (20 entries; other files declare
 * it so). The code also compares profiles against entries 2, 4, 5, 6, 8, 9, 15 and 18 by address,
 * and IDO gives those compares their own %hi/%lo only when the entries are separate objects
 * (&D_800A3FD8[n] shares the table base and changes the code). So the table is written as
 * consecutive arrays that start at those entries, and D_800A3FD8[kind] runs on through them.
 */
const BloodProfile D_800A3FD8[2] = {
    { { 0.0f, 30.0f, 8.0f }, 0x3A, 0, 0 }, /* [0] */
    { { 0.0f, 20.0f, 30.0f }, 0x30, 0, 0 }, /* [1] */
};
const BloodProfile D_800A3FF8[2] = {
    { { 0.0f, 30.0f, 8.0f }, 0x44, 1, 0 }, /* [2] */
    { { 0.0f, 30.0f, 8.0f }, 0x4B, 0, 0 }, /* [3] */
};
const BloodProfile D_800A4018[1] = {
    { { 0.0f, 30.0f, 8.0f }, 0x81, 0, 0 }, /* [4] */
};
const BloodProfile D_800A4028[1] = {
    { { 0.0f, 30.0f, 8.0f }, 0xB2, 0, 0 }, /* [5] */
};
const BloodProfile D_800A4038[2] = {
    { { 0.0f, 30.0f, 8.0f }, 0xB2, 0, 0 }, /* [6] */
    { { 0.0f, 30.0f, 8.0f }, 0x3A, 0, 0 }, /* [7] */
};
const BloodProfile D_800A4058[1] = {
    { { 0.0f, 30.0f, 8.0f }, 0xBB, 0, 0 }, /* [8] */
};
const BloodProfile D_800A4068[6] = {
    { { 0.0f, 30.0f, 8.0f }, 0xBC, 0, 0 }, /* [9] */
    { { 0.0f, 30.0f, 8.0f }, 0xBC, 1, 0 }, /* [10] */
    { { 0.0f, 30.0f, 8.0f }, 0x98, 0, 0 }, /* [11] */
    { { 0.0f, 20.0f, 30.0f }, 0x30, 0, 0 }, /* [12] */
    { { 0.0f, 20.0f, 30.0f }, 0x30, 0, 0 }, /* [13] */
    { { 0.0f, 20.0f, 30.0f }, 0xDA, 0, 0 }, /* [14] */
};
const BloodProfile D_800A40C8[3] = {
    { { 0.0f, 20.0f, 30.0f }, 0x44, 1, 0 }, /* [15] */
    { { 0.0f, 20.0f, 30.0f }, 0xE3, 2, 0 }, /* [16] */
    { { 0.0f, 20.0f, 30.0f }, 0xD3, 2, 0 }, /* [17] */
};
const BloodProfile D_800A40F8[2] = {
    { { 0.0f, 20.0f, 30.0f }, 0xD3, 1, 0 }, /* [18] */
    { { 0.0f, 30.0f, 8.0f }, 0x3A, 0, 0 }, /* [19] */
};

typedef struct {
    u8 value[2];
} Blood356D4Kinds;

extern Blood356D4Kinds D_80089BA0;
extern s32 D_80089BA4;
extern Blood356D4Kinds D_80089BA8;

/* Spawns a blood splat particle at *position plus a ring of smaller spark particles around it. */
void *func_151356D4(Vec2f *position, f32 size, f32 gravity, f32 spread, u8 alpha, u8 endAlpha, s16 delay, s16 lifetime, s16 duration, s16 period, u8 arg10, s32 arg11) {
    void *result;
    ScreenSpriteInfo particle;
    Blood356D4Motion motion;
    Blood356D4Kinds kinds;
    s32 data;
    f32 scale;
    f32 count;
    ScreenSpriteInfo spark;
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
    // matching: x and y are copied as one Vec2f
    *(Vec2f *)&particle.x = *position;
    particle.halfWidth = size;
    particle.halfHeight = size;
    particle.texture = kinds.value[func_150ADA20() & 1];
    particle.timer = lifetime;
    flip = (func_150ADA20() & 1) ? 2 : 0;
    particle.flags = ((particle.texture == 58) ? ((func_150ADA20() & 1) ? 4 : 0) : 0) | 9 | flip;
    particle.fadeStart = duration;
    particle.fadeRate = period;
    particle.kind = 2;
    particle.color1.red = 0xFF;
    particle.color1.green = 0xFF;
    particle.color1.blue = 0xFF;
    particle.color2.red = 0xFF;
    particle.color2.green = 0xFF;
    particle.color2.blue = 0xFF;
    particle.color2.alpha = 0xFF;
    particle.otherModeH = 0;
    particle.unk28 = 0;
    particle.unk2C = 0;
    particle.unk40 = 0;
    particle.unk41 = 9;
    particle.combineMode = 7;
    particle.unk34 = 60;
    particle.unk38 = 128;
    particle.unk3C = 32;
    particle.color1.alpha = alpha;
    particle.unk23 = endAlpha;
    result = func_1515548C(&particle, 3, (s32 *)&data, 1, sizeof(motion), arg10, arg11);
    if (result != NULL) {
        memcpy(&((BloodSplat *)result)->motion, &motion, sizeof(motion));
        scale = (particle.halfWidth + particle.halfHeight) * 0.5f;
        count = (func_150ADA68() * 300.0f + 100.0f) * size * 0.001f;
        spark.timer = 50;
        spark.flags = 9;
        spark.fadeStart = 16;
        spark.fadeRate = 15;
        spark.kind = 3;
        spark.color1.red = 0xFF;
        spark.color1.green = 0xFF;
        spark.color1.blue = 0xFF;
        spark.color1.alpha = alpha;
        spark.color2.red = 0xFF;
        spark.color2.green = 0xFF;
        spark.color2.blue = 0xFF;
        spark.color2.alpha = 0xFF;
        spark.unk23 = endAlpha;
        spark.otherModeH = 0;
        spark.unk28 = 0;
        spark.unk2C = 0;
        spark.unk40 = 0;
        spark.unk41 = 9;
        spark.combineMode = 7;
        spark.unk34 = 60;
        spark.unk38 = 64;
        spark.unk3C = 0;
        spark.halfWidth = (func_150ADA68() * 120.0f + 80.0f) * (size * scale) * 0.0001f;
        spark.halfHeight = (func_150ADA68() * 120.0f + 80.0f) * (size * scale) * 0.0001f;
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
                spark.x = position->x + distance * sine;
                spark.y = position->y + distance * cosine;
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

// matching: this file was compiled with u8 where the definition (game_2062D0.c) has s32 (arg 1), s32 where it has
// s16 (args 8, 10, 11) and u8 (args 9, 12), and with a void return. The definition is
// SteamParticle *func_151DB5D0(s32 kind, f32 *pos, f32 *vel, f32 size, f32 arg4, f32 drag, f32 gravity, s16 lifetime,
// u8 arg8, s16 fadeFrames, s16 fadeStep, u8 spin, u8 argC, s32 argD).
void func_151DB5D0(u8, void *, Vec3f *, f32, f32, f32, f32, s32, s32, s32, s32, s32, u8, s32);

// matching: this file was compiled with s32 where the definition (game_2062D0.c) has u8 (args 3, 6, 11, 15) and
// s16 (arg 5), and with a void return. The definition is SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel,
// u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7, f32 burstScale, f32 altBurstScale, u8 argA,
// CollisionHit *ground, s32 wobbles, s32 bounces, u8 argE, s32 argF).
void func_151D9014(void *, void *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
/* Blood spray from `from` towards `to`; splashes droplets instead when the ground below is surface type 3. */
void func_15135DD0(Vec3f *from, Vec3f *to, f32 radius, u8 arg3, s32 arg4) {
    Vec3f direction;
    f32 fromX;
    Vec3f probe;
    CollisionHit hit;
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
    hit.material = 0;
    hit.flags = 0;
    hit.source = 0;
    hit.surface = 0;
    hit.height = -10000.0f;
    direction.x = to->x - (fromX = from->x);
    direction.y = to->y - from->y;
    direction.z = to->z - from->z;
    probe.x = fromX;
    probe.y = 10000.0f;
    probe.z = from->z;
    if (func_15046460(&probe, 0, from->y, &hit) != 0 && hit.source == 3) {
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
            func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, radius, &offset);
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
                func_15143794(func_150ADA20() & 0xFF, (func_150ADA20() & 0x7F) - 0x3F, radius, &spread);
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


/* Scatters blood decals on the ground within radius of the actor, more for a larger area. */
void func_15136404(Actor *actor, f32 radius, u8 arg2, s32 arg3) {
    f32 area;
    f32 count;
    CollisionHit hit;
    Vec3f probe;
    Vec3f pos;

    if (actor != NULL) {
        area = 3.141592741f * radius * radius;
        count = (func_150ADA68() * 305.0f + 108.0f) * 9.999999975e-07f * area;
        func_1504715C(&hit, actor);
        while (count > 1.0f) {
            func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * radius, &probe.x, &probe.z);
            probe.x += actor->xpos;
            probe.z += actor->zpos;
            probe.y = actor->ypos + 500.0f;
            if (func_15046C80(&probe, 0, actor->ypos - 5000.0f, &hit) != 0 && hit.source != 3) {
                pos.x = probe.x;
                pos.y = hit.height;
                pos.z = probe.z;
                func_151D9B8C(0, func_150ADA68() * 20.0f + 10.0f, func_150ADA20() % 101U + 100, &hit.tri, &pos,
                              func_150ADA20() % 201U + 400, 1, 1, 0, arg2, arg3);
            }
            count -= 1.0f;
        }
    }
}

/* Spawns a blood particle of the given size with one of the four D_80089BAC textures. */
GrowingBloodParticle *func_15136698(f32 size, f32 arg1, u8 alpha, u8 arg3, s16 lifetime, CollisionTri *surface,
                                    Vec3f *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10) {
    GrowingBloodParticle *particle;
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
    particle = func_1513D594(&desc, 0, 0, 0x1A, 0, (random1 & 1) + (random0 & 1), value & 0xFF, 500.0f, 500.0f, NULL, surface, argB, argC, 0, 4, arg9, arg10);
    if (particle != NULL) {
        memcpy(&particle->growth, &extra, sizeof(extra));
    }
    return particle;
}


/* GrowingBloodParticle callback: fades out over the last 32 frames and grows by `growth`. */
s32 func_151368A8(GrowingBloodParticle *particle) {
    s32 timer = particle->head.def.lifetime;
    f32 *growth = &particle->growth; // matching: IDO forms this address before the fade test

    if (timer < 32) {
        s32 alpha = timer << 3;
        if (alpha < particle->head.def.unk44) {
            particle->head.def.unk44 = alpha;
        }
    }
    particle->head.def.width += *growth * D_800BE9A4;
    particle->head.def.height += *growth * D_800BE9A4;
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
void func_15136918(f32 size, u8 arg1, u8 arg2, s16 lifetime, s16 arg4, CollisionTri *surface, f32 *position, u8 arg7,
                   s32 arg8) {
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
    func_1513C73C(&desc, 0xD, 0, surface, position[0], position[1], position[2], size, size, arg4, 0, 0, arg7, arg8);
}

/* Particle callback: fades rgba[0] out over the last 32 frames. */
s32 func_15136A1C(DefParticleHead *particle) {
    s16 timer = particle->def.lifetime;
    s32 alpha;

    if (timer < 32) {
        alpha = timer << 3;
        if (alpha < particle->def.rgba[0]) {
            particle->def.rgba[0] = alpha;
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
    desc.unk18 = 2;
    desc.callback = -1;
    func_15134908(&desc, 0, arg4, arg5);
}

/* Sprays blood from (x, y, z) against the direction (dirX, dirY, dirZ). */
void func_15136AE4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6) {
    f32 position[3];
    f32 velocity[3];
    f32 speed;

    position[0] = x;
    position[1] = y;
    position[2] = z;
    speed = (func_150ADA68() * 112.0f + 247.0f) * 0.001f;
    velocity[0] = -dirX * speed;
    velocity[1] = -dirY * speed;
    velocity[2] = -dirZ * speed;
    func_151D9014(position, velocity, 0, func_150ADA68() * 0.2700000107f + -0.3100000024f,
                    (func_150ADA20() & 0xF) + 25, (func_150ADA20() % 101) + 155,
                    func_150ADA68() * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    arg6[0xC], arg6[1]);
}


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
    CollisionHit hit;
    s32 kind;
    DecalSpray spray;
    s8 heavy;

    if (actor == NULL) {
        return;
    }
    scale = (actor->scale_xz + actor->scale_y) * 0.5f;
    func_1515C244(actor, &position, &unused0, &unused1);
    kind = func_15134070(actor);
    if (kind == BLOOD_PROFILE_NONE) {
        return;
    }
    func_1504715C(&hit, actor);
    if (arg3 != 0) {
        func_1513A24C(&position, D_80089A20[kind], D_800A3C30,
                      ((arg5 != 0) || ((kind == 4) && (actor->character_type != MODEL_UGA_BUGA_BLUE_HEADGEAR)))
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
        spray.pos = position;
        spray.angles.yawBase = 0;
        spray.angles.yawRange = 255;
        spray.angles.pitchBase = -64;
        spray.angles.pitchRange = 69;
        spray.unk28 = 150;
        spray.unk2A = 100;
        spray.maxDistance = 998.0f;
        spray.sizeBase = 25.0f;
        spray.sizeRange = 10.0f;
        if ((D_800A3FF8 == &D_800A3FD8[kind]) || (D_800A40C8 == &D_800A3FD8[kind]) ||
            (D_800A40F8 == &D_800A3FD8[kind])) {
            spray.kind = 1;
        } else {
            spray.kind = 0;
        }
        func_15150D1C(&spray, arg6, arg7);
    }
    if (arg2 != 0) {
        if ((D_800A3FF8 == &D_800A3FD8[kind]) || (D_800A40C8 == &D_800A3FD8[kind]) ||
            (D_800A40F8 == &D_800A3FD8[kind])) {
            heavy = 1;
        } else {
            heavy = 0;
        }
        func_151A9834(&position, position.y - 300.0f, 300.0f, &hit,
                      (func_150ADA20() % 6U) + 18, 0, &heavy, arg6, arg7);
    }
    if (arg4 != 0) {
        if ((D_800A3FF8 == &D_800A3FD8[kind]) || (D_800A40C8 == &D_800A3FD8[kind])) {
            func_151C61A0(&position, &hit, arg6, arg7);
        } else {
            func_1513A48C(&position, arg6, arg7);
        }
    }
}

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
    CollisionTri tri; /* the hit triangle */
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
        func_151DC034(&start, randomA * 192.0f + 192.0f, random % 101U + 0x9B, (func_150ADA20() & 7) + 0xF, arg3, arg4, arg5);
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
                                      0, &tri, &decalPos.x, &decalPos.y, &decalPos.z, &distance, &surface, 0, 0.0f) != 0 &&
                        func_15145C90(surface) != 0 && distance < 1999.0f) {
                        randomA = func_150ADA68();
                        random = func_150ADA20();
                        func_151D9B8C(arg3, randomA * 16.0f + 25.0f, random % 156U + 0x64, &tri, &decalPos,
                                      func_150ADA20() % 101U + 0x64, 1, 1, 1, arg4, arg5);
                    }
                    count--;
                } while (count != 0);
            }
        }
    }
}


/* Picks the impact effect for a hit on actor (NULL: plain blood) from its model and state. */
void func_15137610(Actor *actor, Vec3f *from, Vec3f *to,
                   Vec3f *direction, u8 arg4, s32 arg5) {
    s32 mode;

    if (actor != 0) {
        if (actor->ob_matrix_ptr == 0 || (actor->unk74 & 0xF) == 0xF ||
            (s32)actor->energy <= 0) {
            return;
        }
        mode = actor->character_type;
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
                mode = actor->imune;
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
                if (actor->imune == 0 && actor->stunned == 0) {
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

/* func_151B5BF0 or func_151B6010 */
typedef PulseParticle *(*Blood3783CSpawn)(Vec3f *, Vec3f *, f32, Vec3f *, Vec3f *, f32, s16, u8, s32, f32, u8, u8,
                                          u8, s32);

/* Blood when an actor dies: a burst, a splat and, on flagged ground, a pool and a chunk. */
void func_1513783C(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f position;
    f32 unused0;
    f32 unused1;
    f32 scale;
    Vec3f probe;
    CollisionHit hit;
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
    scale = (actor->scale_xz + actor->scale_y) * 0.5f;
    func_1515C244(actor, &position, &unused0, &unused1);
    func_1504715C(&hit, actor);
    probe.x = position.x;
    probe.y = position.y + 100.0f;
    probe.z = position.z;
    if (func_1514672C(&probe) == 0) {
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
    func_1513A24C(&position, D_80089A20[kind] + 2, D_800A3C30,
                  D_800A3F14[kind] - 2, scale, kind, D_800A3F64[kind],
                  (u8 *)actor, &hit, arg1, arg2);
    if (D_800A3FD8[kind].unkE != 2) {
        func_1513A48C(&position, arg1, arg2);
        if (pool != 0) {
            func_15136698(((func_150ADA68() * 25.0f) + 75.0f) * 0.001000000047f,
                          ((func_150ADA68() * 106.0f) + 75.0f) * 9.999999747e-06f,
                          (func_150ADA20() % 101U) + 155,
                          (func_150ADA20() % 101U) + 100,
                          (func_150ADA20() % 56U) + 203, &hit.tri,
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
s32 func_151380B4(Actor *actor, s32 kind, Vec3f *out) {
    MtxF *bones = actor->ob_matrix_ptr;

    if (bones == NULL) {
        return 0;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[kind].offset, out, bones + 12);
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
        if ((D_800A4058 == &D_800A3FD8[kind]) || (D_800A4068 == &D_800A3FD8[kind])) {
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
        desc.sound = func_1000FA64(0x4FE, (s16)(s32)actor->xpos,
                                   (s16)(s32)actor->ypos,
                                   (s16)(s32)actor->zpos, 0x5DC0,
                                   600, 300, (SoundRecordCallback)func_1000EBC4,
                                   (void *)120, 0, 0, 0);
        func_15134DAC(&desc, 0);
    }
}

/* Spawns a func_15153F18 blood burst at position for profile `kind` (none for type 2). */
void func_151382E0(Vec3f *position, s32 kind, void *arg2, u8 arg3, s32 arg4) {
    SplashSpraySpec burst;
    s32 type;

    type = D_800A3FD8[kind].unkE;
    if (type != 2) {
        burst.spray.pos = *position;
        burst.spray.scaleBase = 1.9799999f;
        burst.spray.scaleRange = 2.00999999f;
        burst.spray.sizeBase = 0.202000007f;
        burst.spray.sizeRange = 0.6460000277f;
        burst.spray.speedBase = 4.0f;
        burst.spray.speedRange = 9.0f;
        burst.spray.countBase = 18;
        burst.spray.countRange = 7;
        burst.angles.yawRange = 255;
        burst.angles.yawBase = 0;
        burst.angles.pitchBase = -63;
        burst.angles.pitchRange = 78;
        burst.spray.itemCountBase = 3;
        burst.spray.itemCountRange = 3;
        burst.spray.lifeBase = 20;
        burst.spray.lifeRange = 30;
        burst.spray.unk30 = 155;
        burst.spray.unk32 = 100;
        burst.spray.unk3C = 16;
        burst.spray.unk3E = 15;
        burst.spray.unk40 = 0;
        burst.spray.unk38 = 0.3000000119f;
        if (type == 1) {
            burst.spray.kind = 1;
        } else {
            burst.spray.kind = 0;
        }
        func_15153F18(&burst.angles, &burst.spray, arg2, arg3, arg4);
    }
}

/*
 * Throws a func_1513264C chunk (model D_800A3FD8[kind].chunkModel) off the actor from position, and for
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
    spawn.model = D_800A3FD8[kind].chunkModel;
    spawn.sizeY = ((D_800A40C8 == &D_800A3FD8[kind]) ? 0.6000000238f : 1.0f) * ((actor->scale_xz + actor->scale_y) * 0.5f);
    spawn.sizeXZ = spawn.sizeY;
    spawn.gravity = (func_150ADA68() * 292.0f + -1099.0f) * 0.001000000047f;
    spawn.pos = *position;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 21U - 83), func_150ADA68() * 8.0f + 9.0f, &spawn.vel);
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
    func_1513264C(&spawn, 3, 0xFF, arg3, 0, arg4, arg5);
    if (D_800A4018 == &D_800A3FD8[kind]) {
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
    } else if (D_800A4028 == &D_800A3FD8[kind]) {
        const s32 masks[4] = { 2, 8, 0x10, 0x20 };
        u8 i;
        const s32 ids[4] = { 0x87, 0x88, 0x89, 0x8A };

        for (i = 0; i < 4; i++) {
            if (!(masks[i] & actor->unk94)) {
                spawn.model = ids[i];
                func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            }
        }
    } else if (D_800A4038 == &D_800A3FD8[kind]) {
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

/* BloodChunk ground-hit callback: bounces off groundY and leaves a blood decal there. */
s32 func_151389A8(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface) {
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
    CollisionHit hit;

    kind = func_15134070(actor);
    if (kind != BLOOD_PROFILE_NONE) {
        valid = func_151380B4(actor, kind, &position);
        func_15138120(actor, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, actor);
            func_151382E0(&position, kind, &hit, arg1, arg2);
            func_15138424(actor, &position, kind, &hit, arg1, arg2);
        }
    }
}

/* Blood for a fatal hit: strip and burst, then the per-profile gore effect. */
void func_15138C80(Actor *actor, u8 arg1, s32 arg2) {
    s32 kind;
    Vec3f position;
    u8 valid;
    CollisionHit hit;

    kind = func_15134070(actor);
    if (kind != BLOOD_PROFILE_NONE) {
        valid = func_151380B4(actor, kind, &position);
        func_15138120(actor, kind, 1);
        if (valid != 0) {
            func_1504715C(&hit, actor);
            func_151382E0(&position, kind, &hit, arg1, arg2);
            switch (kind) {
            case 16:
                func_1513B0F8(actor, &position, &hit, arg1, arg2);
                return;
            case 17:
                func_151036B4(&position, arg1, arg2);
                return;
            case 0:
            case 7:
                func_1513A6E0(actor, &position, &hit, arg1, arg2);
                return;
            case 3:
                func_15138E98(actor, &position, 1, &hit, arg1, arg2);
                return;
            case 5:
            case 6:
                func_15138E98(actor, &position, 0, &hit, arg1, arg2);
                return;
            case 15:
                func_15139768(actor, &position, 0.6f, &hit, arg1, arg2);
                return;
            case 2:
                func_15139768(actor, &position, 1.0f, &hit, arg1, arg2);
                return;
            case 4:
                func_1513A594(actor, &position, &hit, arg1, arg2);
                return;
            case 8:
            case 9:
                func_1513ABB8(actor, &position, kind == 9, &hit, arg1, arg2);
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
                func_15139D74(actor, &position, &hit, arg1, arg2);
                break;
            }
        }
    }
}

const u8 D_800A4160[6] = { 0x08, 0x0C, 0x0A, 0x0E, 0x0D, 0x00 };
const s32 D_800A4168[4] = { 0x59, 0x5A, 0x5B, 0x5C };
const s32 D_800A4178[4] = { 0x4E, 0x4F, 0x50, 0x51 };
const Vec3f D_800A4188[4] = {
    { -7.2473803f, 40.4506f, 6.7134f },
    { 10.305f, 37.725998f, 5.6588f },
    { -18.678f, 15.5406f, 0.3136f },
    { 4.6474f, 13.705f, 17.1138f },
};
const Vec3f D_800A41B8 = { -0.2532f, 47.663998f, -2.545f };

/* Gore for profile 3, 5 and 6 actors: a random chunk set from D_800A4160 and, with extraSpray, chunk model 0x48. */
void func_15138E98(Actor *actor, Vec3f *origin, u8 extraSpray, CollisionHit *hit, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 6U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        BloodChunk *result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
            if (result != NULL) {
                memcpy(&result->unk170, tag, 1);
            }
        }
    }
    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0(origin, arg4, arg5);
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
        packet.sizeXZ = packet.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                              &packet.vel);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, hit, 0, arg4, arg5);
            }
        }
        if (extraSpray == 0) {
            return;
        }
        spray.radius = 1.0f;
        spray.elasticity = 0.64900005f;
        spray.sizeXZ = spray.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                      &spray.vel);
        spray.rotVel.x = func_150ADA68() * 48.158f + -24.079f;
        spray.rotVel.z = func_150ADA68() * 48.158f + -24.079f;
        spray.gravity = func_150ADA68() * 0.323f + -1.4210001f;
        spray.lifetime = func_150ADA20() % 61U + 100;
        func_1513264C(&spray, 3, 0xFF, hit, 0, arg4, arg5);
    }
}

/* BloodChunk ground-hit callback: bounces off groundY and leaves a blood decal there. */
s32 func_15139578(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface) {
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

const u8 D_800A41C4[4] = { 0x08, 0x0C, 0x0A, 0x00 };
const s32 D_800A41C8[4] = { 0x61, 0x62, 0x63, 0x64 };
const s32 D_800A41D8[4] = { 0x5D, 0x5E, 0x5F, 0x60 };
const Vec3f D_800A41E8[4] = {
    { -15.082999f, 50.3348f, 3.2628f },
    { 14.2554f, 52.1254f, 4.4064f },
    { -14.681799f, 17.8482f, -1.3326f },
    { 9.2282f, 23.578999f, 8.4736f },
};

/* Gore for profile 2/15 actors, chunk sizes scaled by `scale`. */
void func_15139768(Actor *actor, Vec3f *origin, f32 scale, CollisionHit *hit, u8 arg4, s32 arg5) {
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
        spawn.sizeXZ = spawn.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f * scale;
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
                BloodChunk *ret;
                spawn.model = D_800A41C8[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg4, arg5);
                if (ret != NULL) {
                    memcpy(&ret->unk170, tag, 1);
                }
            }
        }
    }
    if (actor->ob_matrix_ptr != 0 && (actor->unk74 & 0xF) != 0xF) {
        s32 count;
        u8 mask2;
        u8 j;
        Vec3f dir;
        Vec3f a;
        Vec3f b;
        ParticleSpawn spawn2;
        for (count = (func_150ADA20() & 3) + 5; count > 0; count--) {
            func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 71U - 60), func_150ADA68() * 6.0f + 6.0f, &dir);
            func_151C5F44(origin, &dir, func_150ADA68() * 0.1170000061f + -0.1970000118f, 0.9508649707f,
                          func_150ADA20() % 66U + 190, func_150ADA20() % 45U + 44, func_150ADA68() * 20.0f + 50.0f,
                          0, hit, arg4, arg5);
        }
        {
            s32 q; // matching: unused stack slot
            func_1503F404(BONE_MATRIX(actor, 12), &a.x, &a.y, &a.z, &v.x, &v.y, &v.z, &b.x, &b.y, &b.z);
        }
        spawn2.radius = 1.0f;
        spawn2.elasticity = 0.6490000486f;
        spawn2.sizeXZ = spawn2.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f * scale;
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
                func_15143794((s16)(func_150ADA20() & 0xFF), (s16)(func_150ADA20() % 35U - 84), func_150ADA68() * 6.119999886f + 3.0f, &spawn2.vel);
                spawn2.rotVel.x = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.rotVel.z = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.gravity = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, hit, 0, arg4, arg5);
            }
        }
    }
}

const u8 D_800A4218[3] = { 0x00, 0x04, 0x00 };
const s32 D_800A421C[3] = { 0x69, 0x6A, 0x6B };
const s32 D_800A4228[3] = { 0x66, 0x67, 0x68 };
const Vec3f D_800A4234[3] = {
    { 0.06725f, 14.119f, 66.579254f },
    { 4.36975f, 22.8435f, 17.31975f },
    { -2.76875f, 8.011f, 10.78625f },
};

/* Default gore (profiles 1, 12, 13 and others): a random chunk set from bone 12. */
void func_15139D74(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        BloodChunk *result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
            if (result != NULL) {
                memcpy(&result->unk170, tag, 1);
            }
        }
    }
    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0(origin, arg3, arg4);
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
        packet.sizeXZ = packet.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                              &packet.vel);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, hit, 0, arg3, arg4);
            }
        }
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x08 */ s32 unk8;
} Extra080C_1513A24C;

/* Blood burst (func_1515080C) for an actor of blood profile `kind`; actor is a Actor. */
void func_1513A24C(Vec3f *position, const s32 *table, const f32 *arg2, s32 count, f32 scale, u8 kind, u8 arg6, u8 *actor, CollisionHit *hit, u8 arg9, s32 argA) {
    DebrisSpray config;
    Extra080C_1513A24C extra;
    s32 flag;

    config.countBase = 40;
    config.countRange = 0;
    config.pos = *position;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.lifeBase = 480;
    config.lifeRange = 240;
    config.scaleBase = 1 * scale;
    config.scaleRange = 0 * scale;
    config.floorHitFn = arg6;
    extra.unk0 = 4;
    flag = -1;
    config.angles.pitchBase = -64;
    config.angles.pitchRange = 12;
    config.speedRange = 5.0f;
    config.floorHitChance = 1.0f;
    config.speedBase = 13.0f;
    config.gravityBase = -1.007000089f;
    config.gravityRange = 0.2520000041f;
    config.spin = 16.10200119f;
    switch (kind) {
    case 19:
        if (level == 60) {
            config.floorHitChance = 0.0f;
        }
        break;
    case 13:
        config.angles.pitchBase = -46;
        config.angles.pitchRange = 20;
        config.speedBase = 15.0f;
        config.speedRange = 8.0f;
        config.gravityBase = -1.262000084f;
        config.gravityRange = 0.4900000095f;
        config.spin = 12.91100025f;
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

const s32 D_800A4260[1] = { 0x39 };
const f32 D_800A4264[1] = { 1.0f };

/* Small blood splash at position (func_15152190 with D_800A4260). */
void func_1513A48C(Vec3f *position, u8 arg1, s32 arg2) {
    DebrisBurst config;

    config.countBase = 8;
    config.countRange = 4;
    config.pos = *position;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.angles.pitchBase = -55;
    config.angles.pitchRange = 32;
    config.lifeBase = 40;
    config.lifeRange = 20;
    config.scaleBase = 0.07f;
    config.scaleRange = 0.07f;
    config.speedBase = 10.0f;
    config.speedRange = 9.0f;
    config.gravityBase = -1.0040001f;
    config.gravityRange = 0.254f;
    config.spin = 23.446001f;
    func_15152190(&config, D_800A4260, D_800A4264, 1, 0.0f, 1, arg1, arg2);
}

/* Gore for profile 4 actors: just a splash. */
void func_1513A594(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4) {
    func_1513A5E0(origin, arg3, arg4);
    if (actor->ob_matrix_ptr != 0) { // matching: dead test, IDO still emits the ob_matrix_ptr load
        if (actor) {
        }
    }
}

const s32 D_800A4268[2] = { 0x39, 0x65 };
const f32 D_800A4270[2] = { 1.0f, 4.0f };

/* Blood splash at position (func_15152190 with D_800A4268). */
void func_1513A5E0(Vec3f *position, u8 arg1, s32 arg2) {
    DebrisBurst config;

    config.countBase = 7;
    config.countRange = 7;
    config.pos = *position;
    config.angles.yawBase = 0;
    config.angles.yawRange = 255;
    config.angles.pitchBase = -50;
    config.angles.pitchRange = 27;
    config.speedBase = 4.0f;
    config.speedRange = 4.0f;
    config.lifeBase = 25;
    config.lifeRange = 40;
    config.scaleBase = 0.0201f;
    config.scaleRange = 0.0201f;
    config.gravityBase = -0.43f;
    config.gravityRange = 0.272f;
    config.spin = 20.187f;
    func_15152190(&config, D_800A4268, D_800A4270, 2, 0.0f, 1, arg1, arg2);
}

const u8 D_800A4278[6] = { 0x04, 0x0C, 0x06, 0x07, 0x0D, 0x00 };
const s32 D_800A4280[4] = { 0x7A, 0x7B, 0x7C, 0x7D };
const s32 D_800A4290[4] = { 0x76, 0x77, 0x78, 0x79 };
const Vec3f D_800A42A0[4] = {
    { 14.159599f, 51.866f, 11.1582f },
    { -16.682f, 45.5596f, 13.6838f },
    { -8.215199f, 13.5648f, 26.177399f },
    { 16.7724f, 15.549001f, 19.3364f },
};

/* Gore for profile 0/7 actors. */
void func_1513A6E0(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4) {
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
        spawn.sizeXZ = spawn.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                BloodChunk *ret;
                spawn.model = D_800A4280[i];
                ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
                if (ret != NULL) {
                    memcpy(&ret->unk170, extra, 1);
                }
            }
        }
    }
    if (actor->ob_matrix_ptr != 0 && (actor->unk74 & 0xF) != 0xF) {
        Vec3f a;
        Vec3f b;
        ParticleSpawn spawn2;
        u8 mask2;
        u32 r1;
        u32 r2;
        func_1513A5E0(origin, arg3, arg4);
        {
            s32 q; // matching: unused stack slot
            func_1503F404(BONE_MATRIX(actor, 12), &a.x, &a.y, &a.z, &v.x, &v.y, &v.z, &b.x, &b.y, &b.z);
        }
        spawn2.radius = 1.0f;
        spawn2.elasticity = 0.6490000486f;
        spawn2.sizeXZ = spawn2.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                func_15143794(r1 & 0xFF, r2 % 35U - 84, func_150ADA68() * 6.119999886f + 3.0f, &spawn2.vel);
                spawn2.rotVel.x = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.rotVel.z = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.gravity = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, hit, 0, arg3, arg4);
            }
        }
    }
}

const u8 D_800A42D0[3] = { 0x00, 0x01, 0x02 };
const s32 D_800A42D4[2][2] = { { 0xB3, 0xB4 }, { 0xB7, 0xB8 } };
const s32 D_800A42E4[2][2] = { { 0xB5, 0xB6 }, { 0xB9, 0xBA } };
const Vec3f D_800A42F4[2][2] = {
    { { 12.4256f, 18.9344f, 17.148f }, { -17.1182f, 32.8964f, 4.2846003f } },
    { { 11.5694f, 18.168f, 18.0524f }, { -13.6272f, 34.7778f, 7.015f } },
};

/* Gore for profile 8/9 actors (variant 1 for profile 9). */
void func_1513ABB8(Actor *actor, Vec3f *origin, u8 variant, CollisionHit *hit, u8 arg4, s32 arg5) {
    u8 index;
    Vec3f position;

    index = func_150ADA20() % 3U;
    {
        ParticleSpawn burst;
        u8 tag[1];
        BloodChunk *result;
        u8 i;
        u8 mask;

        tag[0] = 12;
        burst.radius = 1.0f;
        burst.elasticity = 1.0f;
        burst.sizeXZ = burst.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
            if (result != NULL) {
                memcpy(&result->unk170, tag, 1);
            }
        }
    }
    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    if ((actor->unk74 & 0xF) == 0xF) {
        return;
    }
    func_1513A5E0(origin, arg4, arg5);
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
        packet.sizeXZ = packet.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
                              &packet.vel);
                packet.rotVel.x = func_150ADA68() * 30.746002f + -15.373001f;
                packet.rotVel.z = func_150ADA68() * 30.746002f + -15.373001f;
                packet.gravity = func_150ADA68() * 0.208f + -0.708f;
                packet.lifetime = func_150ADA20() % 101U + 60;
                func_1513264C(&packet, 3, 0xFF, hit, 0, arg4, arg5);
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

const u8 D_800A4324[3] = { 0x00, 0x01, 0x02 };
const s32 D_800A4328[2] = { 0xE3, 0xE5 };
const s32 D_800A4330[2] = { 0xE4, 0xE6 };
const Vec3f D_800A4338[2] = {
    { -15.369999f, 15.095f, 13.3182f },
    { -0.0834f, -19.5498f, 23.541f },
};
/* Gore for profile 16 actors. */
void func_1513B0F8(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4) {
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
    spawn.sizeXZ = spawn.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
            particle = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (particle != NULL) {
                memcpy(&((BloodChunk *)particle)->unk170, &tag, 1);
            }
        }
    }
    if (actor->ob_matrix_ptr == NULL) {
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
    drop.sizeXZ = drop.sizeY = (actor->scale_xz + actor->scale_y) * 0.5f;
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
            func_15143134(&D_800A4338[i], &drop.pos, BONE_MATRIX(actor, 12));
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794((u8)yaw, (pitch % 35U) - 0x54, (func_150ADA68() * 6.12f) + 3.0f, &drop.vel);
            drop.rotVel.x = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.rotVel.z = (func_150ADA68() * 30.746002f) + -15.373001f;
            drop.gravity = (func_150ADA68() * 0.208f) + -0.708f;
            random = func_150ADA20();
            drop.lifetime = (random % 101U) + 0x3C;
            func_1513264C(&drop, 3, 0xFF, hit, 0, arg3, arg4);
        }
    }
}
