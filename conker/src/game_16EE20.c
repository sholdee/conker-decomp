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
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;   /* the owner actor */
    /* 0x08 */ u8 unk8;
} Link151419D0;

/* An object carrying a Link151419D0 at 0x28. */
struct LinkedObject151419D0 {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Link151419D0 link;
};

/* Calls func_1514EDF0 with the object and its owner (link.unk4). */
void func_15141970(LinkedObject151419D0 *obj) {
    func_1514EDF0(obj, (Actor *)obj->link.unk4);
}

void func_15141990(void *arg0) {
    func_15141970(arg0);
}

void func_151419B0(void *arg0) {
    func_15141970(arg0);
}

/* Message handler: message 0 destroys the object if it refers to it; 0x2D re-links it. */
void func_151419D0(LinkedObject151419D0 *obj, Msg151419D0 *msg, u8 msgType) {
    Link151419D0 *link = &obj->link;
    s32 first;

    if (msgType == 0) {
        first = msg->unk0;
        if ((first == link->unk4) || (msg->unk4.b == link->unk8)) {
            func_1516972C(&obj->base.header);
        }
    } else if (msgType == 0x2D) {
        if (link->unk4 == msg->unk0) {
            link->unk4 = msg->unk4.w;
            link->unk8 = msg->unk9;
        } else if (link->unk4 == msg->unk4.w) {
            link->unk4 = msg->unk0;
            link->unk8 = msg->unk8;
        }
    }
}

/* Payload copied into an attached effect (see func_15141E38). */
typedef struct {
    /* 0x00 */ s32 handler;    /* index into D_8008A0B4 */
    /* 0x04 */ Actor *owner;
    /* 0x08 */ u8 ownerId;     /* owner->unique_id */
} EffectInfo;

/* Timed effect that runs handler D_8008A0B4[info.handler] on its owner while it lasts (func_15141E38); listed
 * on the owner as ATTACH_TIMED_EFFECT. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EffectInfo info;
} TimedHandlerEffect;

/* List tag of the attached timed effects. */
#define ATTACH_TIMED_EFFECT 0x1A

typedef s32 (*EffectSelector)(s32, Actor *);


/* Runs the actor's model-specific effect selector, then every attached timed effect's handler. */
void func_15141A7C(Actor *actor, s32 arg1) {
    s32 selector;
    s32 handler;

    if (D_800BE616 == 0) {
        selector = func_15141C0C(actor);
        if (D_8008A084[selector] != 0) {
            handler = ((EffectSelector)D_8008A084[selector])(func_15141CC0(func_1510F8CC(actor->floorMaterial)), actor);
            if (handler != -1) {
                if (D_8008A0B4[handler].handler != NULL) {
                    if (D_8008A0B4[handler].duration > 0) {
                        func_15141E38(actor, handler);
                    } else {
                        D_8008A0B4[handler].handler(actor, arg1, 0);
                    }
                }
            }
        }
        {
        Attachment *node;
        node = actor->attachments;
        if (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0) {
            do {
                TimedHandlerEffect *effect;
                effect = node->handle;
                if (D_8008A0B4[effect->info.handler].handler != NULL) {
                    // matching: the u32 view makes IDO reload the index instead of reusing it
                    D_8008A0B4[*(u32 *)&effect->info.handler].handler(actor, arg1, effect->base.header.state);
                }
                node = node->next;
            } while (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0);
        }
        }
    }
}

/* Maps an actor's model id to its effect selector index (11: none). */
s32 func_15141C0C(Actor *actor) {
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
s32 func_15141CC0(u32 type) {
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
void func_15141DA4(void *actor, s32 selector, s32 handler) {
    if ((selector < 12) && (selector >= 0) &&
        (handler < 20) && (handler >= 0) &&
        (D_800BE616 == 0) &&
        (D_8008A084[selector] != 0) && (handler != -1)) {
        if ((D_8008A0B4[handler].handler != NULL) && (D_8008A0B4[handler].duration > 0)) {
            func_15141E38(actor, handler);
        }
    }
}

void *memcpy(void *, const void *, u32);

/* Restarts the actor's timed effect `handler`, or creates and attaches it if it has none. */
void func_15141E38(Actor *actor, s32 handler) {
    Attachment *found;
    Attachment *node;
    s32 pad; // matching: unused stack slot (keeps `info` at sp+0x40)

    found = NULL;
    node = actor->attachments;
    if (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0) {
        do {
            TimedHandlerEffect *effect;
            effect = node->handle;
            if (handler == effect->info.handler) {
                found = node;
                effect->base.header.state = D_8008A0B4[handler].duration;
            }
            node = node->next;
        } while (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0);
    }
    if (found == NULL) {
        TimedHandlerEffect *effect;
        EffectInfo info;
        info.handler = handler;
        info.owner = actor;
        info.ownerId = actor->unique_id;
        effect = func_15149130(D_8008A0B4[handler].duration, -1, -1, -1, 1, 50, 0xC, 0xFF, 1);
        if (effect != NULL) {
            memcpy(&effect->info, &info, sizeof(info));
            func_1514EC1C(effect, actor, ATTACH_TIMED_EFFECT);
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

    func_1513C650(&packet, 0, 0, &src->tri, pos->x, src->height, pos->z, size, size, arg3, flag, 3, 1, 0, 0xFF, 1);
}

/* Template copied onto the stack for func_150A2AEC. */
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
    if (func_150A2AEC(actor - movement_obj, 6, &args, actor) == -1) {
        return 0;
    }
    return 1;
}

/* Spawns the func_15153F18 effect of the given kind at `pos`. */
void func_15142180(u8 kind, Vec3f *pos, s32 arg2, f32 scaleA, f32 scaleB) {
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
    func_15153F18(&params.angles, &params.spray, 0, 0xFF, 1);
}
