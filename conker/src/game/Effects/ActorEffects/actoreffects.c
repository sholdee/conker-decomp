#include <ultra64.h>

#include "functions.h"
#include "variables.h"

struct Msg151419D0 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ union {
        s32 w;
        u8 b;
    } unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
};

/* The block at 0x28 of the receiving object. */
typedef struct {
    /* 0x00 */ s32 handler; /* index into gActorEffects (EffectInfo.handler) */
    /* 0x04 */ s32 owner;   /* the owner actor */
    /* 0x08 */ u8 ownerId;  /* owner->unique_id */
} Link151419D0;

/* An object carrying a Link151419D0 at 0x28. */
struct LinkedObject151419D0 {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Link151419D0 link;
};

/* Calls effect_detachHandle with the object and its owner (link.owner). */
void effect_detachTimedHandler(LinkedObject151419D0 *obj) {
    effect_detachHandle(obj, (Actor *)obj->link.owner);
}

void effect_destroyTimedHandler(void *arg0) {
    effect_detachTimedHandler(arg0);
}

void effect_freeTimedHandler(void *arg0) {
    effect_detachTimedHandler(arg0);
}

/* Message handler: message 0 destroys the object if it refers to it; 0x2D re-links it. */
void effect_timedHandlerMessage(LinkedObject151419D0 *obj, Msg151419D0 *msg, u8 msgType) {
    Link151419D0 *link = &obj->link;
    s32 first;

    if (msgType == 0) {
        first = msg->unk0;
        if ((first == link->owner) || (msg->unk4.b == link->ownerId)) {
            effect_destroyObject(&obj->base.header);
        }
    } else if (msgType == 0x2D) {
        if (link->owner == msg->unk0) {
            link->owner = msg->unk4.w;
            link->ownerId = msg->unk9;
        } else if (link->owner == msg->unk4.w) {
            link->owner = msg->unk0;
            link->ownerId = msg->unk8;
        }
    }
}

/* Payload copied into an attached effect (see effect_attachActorEffect). */
typedef struct {
    /* 0x00 */ s32 handler;    /* index into gActorEffects */
    /* 0x04 */ Actor *owner;
    /* 0x08 */ u8 ownerId;     /* owner->unique_id */
} EffectInfo;

/* Timed effect that runs handler gActorEffects[info.handler] on its owner while it lasts (effect_attachActorEffect); listed
 * on the owner as ATTACH_TIMED_EFFECT. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EffectInfo info;
} TimedHandlerEffect;

/* List tag of the attached timed effects. */
#define ATTACH_TIMED_EFFECT 0x1A

/* This file's .data. */

/* The probe visible callbacks of game_1A11B0.c (probe, r, g, b); the only entry is empty. */
void (*gProbeVisibleCallbacks[1])(void *probe, s32 r, s32 g, s32 b) = { NULL };

/*
 * Per effect selector (effect_getActorEffectSelector): maps the floor material's effect kind and the actor to an index into
 * gActorEffects, or -1. Declared without a prototype: some selectors take the actor as an s32. 11: none.
 */
static s32 (*gActorEffectSelectors[12])() = {
    func_15146970, func_150C1A2C, func_150D8A20, func_150BAA00, func_150B2480, func_150CB1E0, func_151AF6C0,
    func_150EB030, func_150F237C, func_150BEAA8, func_150BED28, NULL,
};

/*
 * The actor effects: the handler and the duration of its timed effect (-1: runs once). Only this file uses it,
 * but it stays global: one declaration fewer in the headers changes the .bss order of game/View/Views/views.c.
 */
ActorEffectDef gActorEffects[20] = {
    { func_15146BB8, 130 }, { func_15146BD8, 130 }, { func_15146A98, 85 },  { func_15146D60, 85 },
    { func_15146E84, 80 },  { func_15147040, -1 },  { func_15147318, 130 }, { func_150C1A40, -1 },
    { attach_spawnBonePointEffects, -1 },  { particle_spawnObjEffects, -1 },  { particle_spawnObjGroundBurst, -1 },  { particle_spawnSideDust, -1 },
    { particle_spawnSideSpray, -1 },  { func_150EB090, 85 },  { func_150F2390, -1 },  { func_15147478, 130 },
    { func_151475D8, 130 }, { func_150BEACC, 85 },  { func_150BED4C, 85 },  { NULL, -1 },
};


/* Runs the actor's model-specific effect selector, then every attached timed effect's handler. */
void effect_runActorEffects(Actor *actor, s32 arg1) {
    s32 selector;
    s32 handler;

    if (gIsMultiplayer == 0) {
        selector = effect_getActorEffectSelector(actor);
        if (gActorEffectSelectors[selector] != 0) {
            handler = gActorEffectSelectors[selector](effect_getSurfaceEffectVariant(collision_getSurfaceType(actor->floorMaterial)), actor);
            if (handler != -1) {
                if (gActorEffects[handler].handler != NULL) {
                    if (gActorEffects[handler].duration > 0) {
                        effect_attachActorEffect(actor, handler);
                    } else {
                        gActorEffects[handler].handler(actor, arg1, 0);
                    }
                }
            }
        }
        {
        Attachment *node;
        node = actor->attachments;
        if (effect_findAttachmentByType(node, ATTACH_TIMED_EFFECT, &node) != 0) {
            do {
                TimedHandlerEffect *effect;
                effect = node->handle;
                if (gActorEffects[effect->info.handler].handler != NULL) {
                    // matching: the u32 view makes IDO reload the index instead of reusing it
                    gActorEffects[*(u32 *)&effect->info.handler].handler(actor, arg1, effect->base.header.state);
                }
                node = node->next;
            } while (effect_findAttachmentByType(node, ATTACH_TIMED_EFFECT, &node) != 0);
        }
        }
    }
}

/* Maps an actor's model id to its effect selector index (11: none). */
s32 effect_getActorEffectSelector(Actor *actor) {
    switch (actor->character_type) {
    case 121:
        return 10;
    case 33:
        return 9;
    case 123:
        return 8;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 150:
        return 0;
    case 16:
    case 145:
        return 1;
    case 43:
        return 2;
    case 84:
        return 5;
    case 54:
    case 83:
    case 165:
        return 6;
    case 88:
        return 7;
    case 69:
        return 3;
    case 75:
        return 4;
    default:
        return 11;
    }
}
/* Picks an effect variant from the current world (level) and the given type. */
s32 effect_getSurfaceEffectVariant(u32 type) {
    if (level == 0x2F) {
        return 6;
    }
    if (level == 0x42) {
        return 7;
    }
    if (level == 0x27) {
        return 8;
    }
    if (level == 0x19) {
        return 5;
    }
    switch (type) {
    case 10:
        return 0;
    case 7:
        return 2;
    case 11:
        return 1;
    case 15:
        return 3;
    case 2:
    case 8:
    case 12:
        if (level == 2) {
            return 7;
        }
        return 4;
    case 5:
        if (level == 0x14) {
            return 5;
        }
        return 9;
    case 0:
        return 9;
    default:
        return 9;
    }
}

/* Attaches timed effect `handler` to the actor when both indices are valid and it has a duration. */
void effect_tryAttachActorEffect(void *actor, s32 selector, s32 handler) {
    if ((selector < 12) && (selector >= 0) &&
        (handler < 20) && (handler >= 0) &&
        (gIsMultiplayer == 0) &&
        (gActorEffectSelectors[selector] != 0) && (handler != -1)) {
        if ((gActorEffects[handler].handler != NULL) && (gActorEffects[handler].duration > 0)) {
            effect_attachActorEffect(actor, handler);
        }
    }
}

void *memcpy(void *, const void *, u32);

/* Restarts the actor's timed effect `handler`, or creates and attaches it if it has none. */
void effect_attachActorEffect(Actor *actor, s32 handler) {
    Attachment *found;
    Attachment *node;
    s32 pad; // matching: unused stack slot (keeps `info` at sp+0x40)

    found = NULL;
    node = actor->attachments;
    if (effect_findAttachmentByType(node, ATTACH_TIMED_EFFECT, &node) != 0) {
        do {
            TimedHandlerEffect *effect;
            effect = node->handle;
            if (handler == effect->info.handler) {
                found = node;
                effect->base.header.state = gActorEffects[handler].duration;
            }
            node = node->next;
        } while (effect_findAttachmentByType(node, ATTACH_TIMED_EFFECT, &node) != 0);
    }
    if (found == NULL) {
        TimedHandlerEffect *effect;
        EffectInfo info;
        info.handler = handler;
        info.owner = actor;
        info.ownerId = actor->unique_id;
        effect = effect_newTimedEffect(gActorEffects[handler].duration, -1, -1, -1, 1, 50, 0xC, 0xFF, 1);
        if (effect != NULL) {
            memcpy(&effect->info, &info, sizeof(info));
            effect_attach(effect, actor, ATTACH_TIMED_EFFECT);
        }
    }
}

/* Spawns a burst of particles of the given kind, sized by `scale`. */
void func_15141F78(u8 kind, CollisionHit *src, f32 scale, u8 arg3, Vec3f *pos, u8 arg5) {
    SpriteDef packet;
    f32 size;
    s32 flag;

    packet.texture = kind;
    packet.unk7 = 0;
    packet.flags = 0x6F701;
    packet.lifetime = (func_150ADA20() % 61U) + 100;
    packet.frame = 0;
    packet.frameSpeed = 0;
    packet.unk10 = (func_150ADA20() & 0x7F) + 128;
    packet.unk11 = 0xFF;
    packet.rgba[0] = 0xFF;
    packet.rgba[1] = 0xFF;
    packet.rgba[2] = 0xFF;
    packet.rgba[3] = 0xFF;
    packet.renderModes = 0x3B0002;
    packet.unk16 = 0;
    packet.unk17 = 7;
    packet.viewportMask = 0xFF;
    packet.unk1C = src->material;
    packet.fadeStart = 40;
    packet.fadeRate = 6;

    size = ((func_150ADA68() * 5.0f) + 10.0f) * scale;
    if (arg5 == 2) {
        flag = 1;
    } else {
        flag = 0;
    }

    particle_spawnSurfaceQuadSprite(&packet, 0, 0, &src->tri, pos->x, src->height, pos->z, size, size, arg3, flag, 3, 1, 0, 0xFF, 1);
}

/* Template copied onto the stack for zone_findActorZone. */
struct Args151420F8 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

const Args151420F8 D_800A5200 = { 0x402E, 0x402F, 0x4030, 0x4031, 0x4032, 0x4033 };

s32 func_151420F8(Actor *actor) {
    Args151420F8 args;

    args = D_800A5200;
    if (zone_findActorZone(actor - movement_obj, 6, &args, actor) == -1) {
        return 0;
    }
    return 1;
}

/* Spawns the particle_spawnSplashSprayAtAngles effect of the given kind at `pos`. */
void effect_spawnSplashSpray(u8 kind, Vec3f *pos, s32 arg2, f32 scaleA, f32 scaleB) {
    SplashSpraySpec params;

    params.spray.pos = *pos;
    params.spray.countBase = 3;
    params.spray.countRange = 3;
    params.angles.yawRange = 255;
    params.angles.pitchBase = -25;
    params.angles.pitchRange = 10;
    params.spray.scaleBase = 2.5f * scaleA;
    params.spray.scaleRange = (1.0f + 1.0f) * scaleA; // matching: folds to 2.0f but avoids IDO turning 2.0f * x into x + x
    params.spray.sizeBase = 0.1080000028f;
    params.spray.sizeRange = 0.2050000131f;
    params.spray.speedBase = 3.0f * scaleB;
    params.spray.speedRange = 3.5f * scaleB;
    params.angles.yawBase = 0;
    params.spray.itemCountBase = 3;
    params.spray.itemCountRange = 1;
    params.spray.lifeBase = 9;
    params.spray.lifeRange = 15;
    params.spray.unk30 = 180;
    params.spray.unk32 = 75;
    params.spray.unk38 = 0.0f;
    params.spray.unk3C = 12;
    params.spray.unk3E = 21;
    params.spray.unk40 = arg2;
    params.spray.kind = kind;
    particle_spawnSplashSprayAtAngles(&params.angles, &params.spray, 0, 0xFF, 1);
}
