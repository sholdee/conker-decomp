#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Updates flag bits in unk84 (0x4, 0x4000, 0x10000, 0x1000000) from func_1509BE40 queries; the first two
   checks are skipped when D_800D2E4C[0x4] bit 0x80 / unk1 bit 4 are set. */
void func_150AE5B0(Camera *arg0) {
    if (!(D_800D2E4C[0x4] & 0x80)) {
        if (func_1509BE40(0, 0x2000, 0xBB) != -1) {
            if (func_15123934(arg0, arg0->Mode, 0, arg0->Tightness, CAMERA_MODE_PICKED_UP) != 0) {
                arg0->AvailableModes |= 0x1000000;
                func_151254F4(arg0, movement_obj[0].attach_char - 1);
            }
        } else if (func_151239CC(arg0, CAMERA_MODE_PICKED_UP) != 0) {
            func_151254F4(arg0, 0);
        }
    }
    arg0->AvailableModes &= ~0x4000;
    if (!(D_800D2E4C[0x1] & 4)) {
        if (func_1509BE40(1, 0x2000, 0x95, func_1509BE40(0, 0x2014, 0xB7) | 0x2000) != 0) {
            arg0->AvailableModes |= 0x1000000;
            if ((arg0->Mode & 1) && func_15123934(arg0, arg0->Mode, 0, arg0->Tightness, CAMERA_MODE_NORMAL) != 0) {
                arg0->DistIndex = 3;
                arg0->AvailableModes &= ~4;
                func_15124B18(arg0);
            }
        } else if (func_151239CC(arg0, CAMERA_MODE_NORMAL) != 0) {
            func_15124B18(arg0);
            arg0->AvailableModes &= ~0x1000000;
        }
    }
    if (func_1509BE40(1, 0x4082, 6, 0x9000) != 0) {
        arg0->AvailableModes |= 0x10000;
    } else {
        arg0->AvailableModes &= ~0x10000;
    }
}


/* Link from an effect to the actor it is attached to. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when the link was made */
    /* 0x05 */ u8 bone;
} OwnerLink;

/* Effect object attached to an actor (created by func_150AEB9C). */
struct AttachedEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ OwnerLink link;
};


/* Bursts 12-20 particles plus one emission from the owner's bone, when the owner is still the same actor. */
void func_150AE790(AttachedEffect *effect) {
    OwnerLink *link;
    Actor *owner;
    EffectTemplate18 spawn;
    TrailMotion motion;
    s32 pad; // matching: unused stack slot
    TrailRender style;
    f32 sinYaw;
    f32 cosYaw;
    f32 speed;
    f32 horizontal;
    f32 sinPitch;
    f32 cosPitch;
    BillboardTemplate emission;
    s32 count;
    u8 yaw;
    u8 pitch;
    MtxF *bones;
    f32 duration;
    f32 distance;

    link = &effect->link;
    if (effect->link.owner == NULL) {
        return;
    }
    owner = link->owner;
    if (link->owner->type == 0) {
        return;
    }
    bones = owner->ob_matrix_ptr;
    if (bones != NULL && link->owner->unique_id == link->ownerId) {
        func_15142314(link->owner->ob_matrix_ptr, link->bone, &spawn.pos);
        spawn.kind = 1;
        spawn.flags = 1;
        motion.texture = 0;
        motion.shade = 0xFF - ((link->owner->floorMaterial >> 5) & 3) * 64;
        motion.unk14 = link->owner->floorY;
        motion.flags = 8;
        motion.alpha = 0xFF;
        count = func_150ADA20() % 9U + 12;
        style.unk0 = 0;
        style.setGeometryMode = 1;
        style.clearGeometryMode = 0x160600;
        style.renderMode = 3;
        style.unk10 = 0x10;
        style.unk14 = 0x80;
        style.unk18 = 0x20;
        style.primMode = 0;
        style.envMode = 9;
        for (; count != 0; count--) {
            yaw = func_150ADA20() & 0xFF;
            pitch = func_150ADA20() % 51U;
            sinYaw = func_151423D8(yaw);
            cosYaw = func_151423D8(yaw - 0x40);
            sinPitch = func_151423D8(pitch);
            cosPitch = func_151423D8(pitch - 0x40);
            speed = func_150ADA68() * 0.5f + 18.79999924f;
            spawn.itemCount = 4;
            spawn.life = (func_150ADA20() & 0xF) + 35;
            motion.halfWidth = func_150ADA68() * 2.299999952f + 2.900000095f;
            motion.gravity = func_150ADA68() * 0.1000000015f + 0.6999999881f;
            horizontal = speed * sinPitch;
            motion.velocity.x = horizontal * cosYaw;
            motion.velocity.y = -speed * cosPitch;
            motion.velocity.z = horizontal * sinYaw;
            func_15147DA0(&spawn, &motion, 0, 1, 2, 0, 0, 0, 0, 0, 0, &style, 0, effect->base.header.owner, effect->base.header.kind);
        }

        duration = func_150ADA68() + 2.5f;
        distance = func_150ADA68() * 200.0f + 600.0f;
        emission.unk1A = distance;
        emission.unk4 = distance / duration;
        emission.height = 0;
        emission.width = 0;
        emission.primR = 0xFF;
        emission.envR = 0;
        emission.envG = 0;
        emission.envB = 0;
        emission.primG = 0xFF;
        emission.primB = 0xFF;
        emission.life = 100;
        emission.primA = 0xFF;
        emission.fadeStep = 0;
        emission.frame = 0;
        emission.frameStep = 0;
        emission.subX = 0;
        emission.subY = 0;
        emission.subZ = 0;
        emission.velX = 0;
        emission.velZ = 0;
        emission.accelY = 0;
        emission.velY = 0;
        emission.anim = D_8008CA4C[50];
        emission.x = spawn.pos.x;
        emission.y = spawn.pos.y;
        emission.z = spawn.pos.z;
        emission.callback = 0xB;
        emission.flags = 0;
        emission.unk2B = 0;
        emission.unk8 = 0;
        func_15167D84(&emission, 0, 0, -1, effect->base.header.owner, effect->base.header.kind);
    }
}
/* One effect to attach: the object to spawn and the bone it follows. */
typedef struct {
    /* 0x00 */ u8 bone;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 objectId;
} EffectDef;

typedef struct {
    s32 count;
    const EffectDef *defs;
} EffectDefList;

static const EffectDef D_8009F760[] = {
    { 0x00, 0, 0x00 },
    { 0x10, 0, 0x12 },
    { 0x17, 0, 0x18 },
    { 0x19, 0, 0x28 },
    { 0x1A, 0, 0x32 },
};

static EffectDefList D_800886E0[] = {
    { 5, D_8009F760 },
};

/* Spawns the effects in D_800886E0[0] and attaches each to `actor`. Also checksums the code from
   func_15001A08 up to func_15001B08 and, if it was modified, empties object list 35. */
void func_150AEB9C(Actor *actor) {
    u32 *p;
    u32 sum;
    s32 i;
    s32 idx; // matching: the list index must be a variable, not a constant 0
    u32 end;

    idx = 0;
    if (actor != NULL) {
        sum = 0;
        end = (u32)func_15001B08;
        for (p = (u32 *)func_15001A08; (u32)p < end; p++) {
            sum ^= *p;
            sum <<= 1;
        }
        if (sum != 0xB4E42D60) {
            D_800DCE50[0][35] = NULL;
            D_800DCE50[1][35] = NULL;
        }
        for (i = 0; i < D_800886E0[idx].count; i++) {
            AttachedEffect *effect;
            effect = func_151491F4(D_800886E0[idx].defs[i].objectId, 0, -1, 1, 0x27, 8, 0xFF, 0);
            if (effect == NULL) {
                break;
            } else { // matching: the else-block and its `list` local steer as1's delay-slot fill
                EffectDefList *list;
                list = &D_800886E0[idx];
                effect->link.owner = actor;
                effect->link.ownerId = actor->unique_id;
                effect->link.bone = list->defs[i].bone;
            }
        }
    }
}

/* Accelerates unk96 by unk94 per elapsed frame (capped at 0x1400), moves unk9E down by unk96 / 256,
   and counts unkA4 up to 25. */
void func_150AECCC(struct42 *arg0) {
    arg0->unk96 += arg0->unk94 * D_800BE9E4;
    if (arg0->unk96 > 0x1400) {
        arg0->unk96 = 0x1400;
    }
    arg0->unk9E -= arg0->unk96 >> 8;
    arg0->unkA4 += D_800BE9E4;
    if (arg0->unkA4 > 25) {
        arg0->unkA4 = 25;
    }
}

/* Particle callback 11 (D_8008C9C8[11]): grows the billboard square by tmpl.unk4 per elapsed frame up to the
   size tmpl.unk1A, then starts its fade (fadeStep 70). */
void func_150AED4C(BillboardParticle *p) {
    p->tmpl.width += p->tmpl.unk4 * D_800BE9E4;
    if (p->tmpl.unk1A < p->tmpl.width) {
        p->tmpl.fadeStep = 70;
        p->tmpl.width = p->tmpl.unk1A;
    }
    p->tmpl.height = p->tmpl.width;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x1B];
    /* 0x1B */ u8 unk1B;
} Sprite150AED9C;

struct Particle150AED9C {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[0x98 - 0x1E];
    /* 0x98 */ Sprite150AED9C *sprite;
};

/* Sets the sprite's unk1B to unk1C * 8, capped at 255. Always returns 1. */
s32 func_150AED9C(Particle150AED9C *particle) {
    Sprite150AED9C *sprite = particle->sprite;
    s32 value = particle->unk1C * 8;

    if (value > 255) {
        value = 255;
    }
    sprite->unk1B = value;
    if (sprite->unk1B < 0) { // always false for a u8, but the original tests it
        return 0;
    }
    return 1;
}

/* Sprite update D_80089C70[4]: over the last 32 frames of its lifetime, def.unk10 falls with it (lifetime * 8). */
s32 func_150AEDD8(QuadSprite *sprite) {
    if (sprite->def.lifetime < 32) {
        sprite->def.unk10 = sprite->def.lifetime * 8;
    }
    return 1;
}

/* Message handler for an AttachedEffect: on 0x2D, re-links the effect from one actor of a pair to the
   other; on 0, destroys the effect if the given actor (or id) is its owner. */
void func_150AEDF8(AttachedEffect *effect, ActorMsg *msg, u8 type) {
    OwnerLink *link = &effect->link;

    if (type == 0x2D) {
        if (link->owner == msg->swap.actorA) {
            link->owner = msg->swap.actorB;
            link->ownerId = msg->swap.idB;
        } else if (link->owner == msg->swap.actorB) {
            OwnerLink *dst = link; // matching: a block-local alias keeps the ownerId store out of the branch delay slot

            dst->owner = msg->swap.actorA;
            dst->ownerId = msg->swap.idA;
        }
    } else if (type == 0) {
        Actor *obj = msg->ref.actor;

        if (obj == link->owner || msg->ref.uniqueId == link->ownerId) {
            func_1516972C(&effect->base.header);
        }
    }
}
