#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static const Vec3f D_800A15F0[4] = { { -78.0f, 0.0f, 0.0f }, { 78.0f, 0.0f, 0.0f }, { -150.0f, 0.0f, 0.0f }, { 150.0f, 0.0f, 0.0f } };
static const Vec3f D_800A1620[2] = { { -78.0f, 20.0f, 0.0f }, { 78.0f, 20.0f, 0.0f } };
static const u8 D_800A1638[4] = { 0x16, 0x1A, 0x00, 0x00 };
static const u8 D_800A163C[4] = { 0x40, 0x20, 0x00, 0x00 };
static const Vec3f D_800A1640[2] = { { -175.0f, 42.0f, 0.0f }, { 175.0f, 42.0f, 0.0f } };
static const Vec3f D_800A1658[2] = { { -300.0f, 42.0f, 0.0f }, { 300.0f, 42.0f, 0.0f } };
static const u8 D_800A1670[4] = { 0x16, 0x1A, 0x00, 0x00 };
static const Vec3f D_800A1674 = { 0.0f, -29.0f, 0.0f };
static const Vec3f D_800A1680 = { 0.0f, 62.0f, 98.0f };
static const Vec3f D_800A168C[2][4][2] = {
    {
        { { -125.0f, 10.0f, -30.0f }, { 125.0f, 10.0f, 30.0f } },
        { { -125.0f, 10.0f, 30.0f }, { 125.0f, 10.0f, -30.0f } },
        { { -125.0f, 75.0f, -30.0f }, { 125.0f, 75.0f, 30.0f } },
        { { -125.0f, 75.0f, 30.0f }, { 125.0f, 75.0f, -30.0f } },
    },
    {
        { { -170.0f, 20.0f, -40.0f }, { 170.0f, 20.0f, 40.0f } },
        { { -170.0f, 20.0f, 40.0f }, { 170.0f, 20.0f, -40.0f } },
        { { -170.0f, 90.0f, -40.0f }, { 170.0f, 90.0f, 40.0f } },
        { { -170.0f, 90.0f, 40.0f }, { 170.0f, 90.0f, -40.0f } },
    },
};
static const Vec3f D_800A174C[2][4][2] = {
    {
        { { -200.0f, 10.0f, -30.0f }, { 200.0f, 10.0f, 30.0f } },
        { { -200.0f, 10.0f, 30.0f }, { 200.0f, 10.0f, -30.0f } },
        { { -200.0f, 75.0f, -30.0f }, { 200.0f, 75.0f, 30.0f } },
        { { -200.0f, 75.0f, 30.0f }, { 200.0f, 75.0f, -30.0f } },
    },
    {
        { { -270.0f, 20.0f, -40.0f }, { 270.0f, 20.0f, 40.0f } },
        { { -270.0f, 20.0f, 40.0f }, { 270.0f, 20.0f, -40.0f } },
        { { -270.0f, 90.0f, -40.0f }, { 270.0f, 90.0f, 40.0f } },
        { { -270.0f, 90.0f, 40.0f }, { 270.0f, 90.0f, -40.0f } },
    },
};
static const u8 D_800A180C[2][2] = { { 0x16, 0x1A }, { 0x09, 0x06 } };

/* The texture id pickers by effect kind (game_2062D0.c, where each takes no argument). This file was
 * compiled with a view that passes the actor and the slot (as a u8, by prototype) to the kind 12 picker. */
extern u8 (*D_8008FCD0[])(Actor *actor, u8 slot);

/* Fires beam slot `slot` (0 or 1) of `actor`: spawns its beam, a spark burst and an effect
 * at the muzzle. */
void func_150EEE00(actor, slot)
    Actor *actor;
    u8 slot;
{
    Vec3f muzzle;
    LightEffectHeader header;
    Vec3i origin;
    MtxF *bones;

    if (D_800A163C[slot] & actor->unk94) {
        return;
    }

    func_150EEF80(actor, slot, 0xFF, 1);
    bones = actor->ob_matrix_ptr;
    if (bones != NULL) {
        func_15143134(&D_800A15F0[slot], &muzzle, &bones[D_800A1638[slot]]);
        header.flags = 3;
        header.update = -1;
        header.lifetime = (func_150ADA20() % 3U) + 4;
        header.lightType = 0;
        origin.x = muzzle.x;
        origin.y = muzzle.y;
        origin.z = muzzle.z;
        func_151602C0(&header, &origin, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0xFF, 1);
        func_150F0A24(&muzzle);
    }
}

/* Sends message 0x43 for slot `slot` of `actor`. */
void func_150EEF40(actor, slot)
    Actor *actor;
    u8 slot;
{
    ActorSlotMsg msg;

    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    msg.slot = slot;
    func_151403A8((s32)&msg, 0x43);
}


/* Spawns the beam of slot `slot` of `actor`, with its sparks, emitter, glow and flash. */
AimedBeam *func_150EEF80(Actor *actor, u8 slot, u8 arg2, s32 arg3) {
    AimedBeam *beam;
    ParticleDef spawn;
    AimedBeamState init;
    AimedBeamState *link;

    if (actor == NULL) {
        return NULL;
    }
    if (slot >= 2) {
        return NULL;
    }
    init.widthPhase = func_150ADA68() * 6.283185482f;
    init.alphaPhase = func_150ADA68() * 6.283185482f;
    init.actor = actor;
    init.actorId = actor->unique_id;
    init.target = func_15083E90(0x12);
    init.targetId = 0x12;
    init.slot = slot;
    init.direction = D_800A5480;
    init.flags = 0;
    spawn.texture = 0x94;
    spawn.type = 7;
    spawn.renderModes = 0x2203;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0xFF;
    spawn.rgba[2] = 0xFF;
    spawn.rgba[3] = 0xFF;
    init.inverse = 1.0f;
    spawn.height = 1.0f;
    init.distance = 0.0f;
    spawn.scale.x = 0.0f;
    spawn.scale.y = 0.0f;
    spawn.scale.z = 0.0f;
    spawn.width = 100.0f;
    spawn.pos = D_800A5480;
    spawn.rot = D_800A5480;
    spawn.flags = 0x046C0008;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 7;
    spawn.unk48 = 0;
    spawn.viewportMask = 0xFF;
    spawn.unk50 = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    beam = func_1513D2F0(&spawn, &D_800A4AA0, 0x1D, 0, 0, 0x1C, 0, 0, 0, 0x44, arg2, arg3);
    if (beam != NULL) {
        link = &beam->link;
        memcpy(link, &init, sizeof(init));
        {
        BoltDef part;
        SparkLink sparkLink;
        EmitterLink emitterLink;
        BeamEmitter *emitter;
        part.lifetime = 300;
        part.numPoints = 9;
        part.jitterDelay = 1.0f;
        part.jitterDelayRange = 1.0f;
        part.flags = 0;
        part.color.r = 0xFF;
        part.color.g = 0xDC;
        part.color.b = 0xA0;
        part.color.a = 0xC8;
        part.update1 = 0;
        part.update2 = -1;
        part.start.x = 0.0f;
        part.start.y = 0.0f;
        part.start.z = 0.0f;
        part.end.x = 0.0f;
        part.end.y = 0.0f;
        part.end.z = 0.0f;
        part.up.x = 0.0f;
        part.up.y = 0.0f;
        part.up.z = 0.0f;
        part.length = 0.0f;
        part.invLength = 1.0f;
        sparkLink.actor = actor;
        part.jitter = 0.9499999881f;
        part.width = 40.0f;
        sparkLink.actorId = actor->unique_id;
        sparkLink.beam = beam;
        part.type = 1;
        for (sparkLink.index = 0; sparkLink.index < 2; sparkLink.index++) {
            link->sparks[sparkLink.index] = func_151A7950(&part, 0x10, arg2, arg3);
            if (link->sparks[sparkLink.index] != NULL) {
                memcpy(link->sparks[sparkLink.index]->link, &sparkLink, sizeof(sparkLink));
            }
        }
        emitterLink.beam = beam;
        emitterLink.accum = 0.0f;
        emitter = func_15149130(300, -1, 0x22, -1, 0, 0x20, sizeof(EmitterLink), arg2, arg3);
        link->emitter = emitter;
        if (emitter != NULL) {
            memcpy(&emitter->link, &emitterLink, sizeof(emitterLink));
        }
        link->glow = func_150EFEC8(actor, slot, 0xFF, 0xD9, 0xA0, beam, arg2, arg3);
        link->flash = func_150F0198(0x46, 0xFF, 0x82, 0, beam, arg2, arg3);
        }
    }
    return beam;
}

/* Per-frame update of a beam: aims it from the muzzle to the target, pulses it and applies its hit. */
s32 func_150EF38C(AimedBeam *beam) {
    AimedBeamState *link;
    Actor *actor;
    Actor *target;
    s16 *position;
    RayHit hit;
    f32 angle;
    void *object;

    link = &beam->link;
    actor = link->actor;
    target = link->target;
    if (actor->unique_id != link->actorId || actor->type == 0 || actor->character_type == 0xFF) {
        return 0;
    }
    if (target->unique_id != link->targetId || target->type == 0) {
        return 0;
    }
    if (actor->unk94 & D_800A163C[link->slot]) {
        return 0;
    }
    link->flags &= ~1;
    if (actor->ob_matrix_ptr != NULL) {
        func_15143134(&D_800A15F0[link->slot], &beam->head.def.pos, &actor->ob_matrix_ptr[D_800A1638[link->slot]]);
        beam->head.def.rot.x = target->xpos;
        beam->head.def.rot.y = target->ypos;
        beam->head.def.rot.z = target->zpos;
        link->direction.x = beam->head.def.rot.x - beam->head.def.pos.x;
        link->direction.y = beam->head.def.rot.y - beam->head.def.pos.y;
        link->direction.z = beam->head.def.rot.z - beam->head.def.pos.z;
        if (func_15145128(&link->direction, &link->direction, &link->distance, &link->inverse) != 0) {
            link->flags |= 1;
        }
    }
    link->widthPhase += 0.1600000113f * gFrameDelta;
    angle = func_15144B68(link->widthPhase);
    link->widthPhase = angle;
    beam->head.def.width = sinf(angle) * 5.0f + 16.0f;
    link->alphaPhase += 0.1600000113f * gFrameDelta;
    angle = func_15144B68(link->alphaPhase);
    link->alphaPhase = angle;
    beam->head.def.unk44 = sinf(angle) * 25.0f + 230.0f;
    if (link->flags & 1) {
        if (link->flash != NULL) {
            link->flash->light->disabled = FALSE;
            position = &link->flash->light->x;
            position[0] = beam->head.def.pos.x;
            position[1] = beam->head.def.pos.y;
            position[2] = beam->head.def.pos.z;
        }
        func_15081690(actor, beam->head.def.pos.x, beam->head.def.pos.y, beam->head.def.pos.z,
                      link->direction.x, link->direction.y, link->direction.z,
                      &hit, link->distance, 1, 0, 1, 4, 0, 0);
        if (hit.hitType >= 2) {
            object = hit.obj;
            actor_flashFog(object, 0xFF, 0, 0, 0x90, 0, 1);
            actor_hitScripted(object, 0xA0038, 0, actor - movement_obj);
        }
    } else if (link->flash != NULL) {
        link->flash->light->disabled = TRUE;
    }
    return 1;
}

/* Whether `actor` has id 0x28. */
s32 func_150EF784(Actor *actor, s32 arg1, s32 arg2) {
    if (actor->character_type == 0x28) {
        return TRUE;
    }
    return FALSE;
}

/* Releases a beam's sparks, emitter, glow and flash (func_1516972C), then the beam itself. */
void func_150EF7B0(AimedBeam *beam) {
    s32 i;
    void *part;
    AimedBeamState *link = &beam->link;

    for (i = 0; i < 2; i++) {
        part = link->sparks[i];
        if (part != NULL) {
            func_1516972C(part);
        }
    }
    part = link->emitter;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = link->glow;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = link->flash;
    if (part != NULL) {
        func_1516972C(part);
    }
    func_1513CA6C(&beam->head);
}

/* Like func_150EF7B0, through the func_1516979C / func_1513CAA0 variants. */
void func_150EF860(AimedBeam *beam) {
    s32 i;
    void *part;
    AimedBeamState *link = &beam->link;

    for (i = 0; i < 2; i++) {
        part = link->sparks[i];
        if (part != NULL) {
            func_1516979C(part);
        }
    }
    part = link->emitter;
    if (part != NULL) {
        func_1516979C(part);
    }
    part = link->glow;
    if (part != NULL) {
        func_1516979C(part);
    }
    part = link->flash;
    if (part != NULL) {
        func_1516979C(part);
    }
    func_1513CAA0(&beam->head);
}

/* Message handler of a beam: releases it when its actor goes away (types 0 and 0x43)
 * and follows its actor and target through an identity swap (type 0x2D). */
void func_150EF910(AimedBeam *beam, BeamMsg *msg, u8 type) {
    AimedBeamState *link = &beam->link;
    Actor *actor;
    Actor *msgActor;
    u8 actorId;
    u8 msgActorId;

    if (type == 0) {
        // matching: all four operands are loaded before the first compare
        actor = link->actor;
        msgActor = msg->ref.actor;
        actorId = link->actorId;
        msgActorId = msg->ref.uniqueId;
        if (actor == msgActor || actorId == msgActorId) {
            func_1516972C(&beam->head.header);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
        if (link->target == msg->swap.actorA) {
            link->target = msg->swap.actorB;
            link->targetId = msg->swap.idB;
        } else {
            if (link->target == msg->swap.actorB) {
                link->target = msg->swap.actorA;
                link->targetId = msg->swap.idA;
            }
        // matching: the label ends a block, so IDO emits the else path's own branch to the epilogue
        swapped:;
        }
    } else if (type == 0x43) {
        if (link->actor == msg->ref.actor || link->actorId == msg->ref.uniqueId) {
            if (link->slot == msg->ref.slot) {
                func_1516972C(&beam->head.header);
            }
        }
    }
}

/* Per-frame update of a beam spark: follows the beam while it is aimed, hides otherwise. */
s32 func_150EFA4C(Bolt *spark) {
    AimedBeam *beam;
    AimedBeamState *link;
    SparkLink *sparkLink;

    sparkLink = spark->link;
    beam = sparkLink->beam;
    link = &beam->link;
    if (link->flags & 1) {
        Actor *actor;

        actor = link->actor;
        if (actor->ob_matrix_ptr) {
            u8 slot;

            slot = link->slot;
            spark->def.start = beam->head.def.pos;
            spark->def.end = beam->head.def.rot;
            spark->def.length = link->distance;
            spark->def.invLength = link->inverse;
            func_15143134(&D_800A1620[slot], &spark->def.up, &actor->ob_matrix_ptr[D_800A1638[slot]]);
            spark->def.up.x = spark->def.up.x - spark->def.start.x;
            spark->def.up.y = spark->def.up.y - spark->def.start.y;
            spark->def.up.z = spark->def.up.z - spark->def.start.z;
            spark->def.flags |= 2;
        } else {
            spark->def.flags &= ~2;
        }
    } else {
        spark->def.flags &= ~2;
    }
    return 1;
}

/* Per-frame update of a beam's emitter: spawns about 0.36-0.66 particles per frame at the beam's end. */
void func_150EFB80(BeamEmitter *emitter) {
    EmitterLink *link;
    AimedBeam *beam;
    u32 yaw;
    u32 pitch;
    EffectTemplate18 spawn;
    TrailBurstParams part;
    f32 pad; // matching: unused stack slot

    link = &emitter->link;
    link->accum += (364.0f + func_150ADA68() * 300.0f) * 0.001000000047f;
    if (link->accum > 1.0f) {
        beam = link->beam;
        if (beam->link.flags & 1) {
            spawn.pos = beam->head.def.rot;
            spawn.flags = 5;
            spawn.kind = 10;
            part.flags = 4;
            part.primMode = 2;
            part.envMode = 3;
            part.color.red = 0x9D;
            part.color.green = 0x72;
            part.color.blue = 0x2E;
            part.color2.red = 0;
            part.color2.green = 0;
            part.color2.blue = 0;
            part.color2.alpha = 0;
            part.colorLevel = 0xFF;
            part.renderMode = 3;
            part.unk21 = 0x24;
            part.setGeometryMode = 0x200005;
            part.clearGeometryMode = 0x60600;
            part.fadeStart = 12;
            part.fadeRate = 21;
            part.shrinkStart = 1;
            part.shrinkPaused = 0;
            part.shrinkFactor = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            do {
                spawn.itemCount = func_150ADA20() % 3U + 3;
                spawn.life = (func_150ADA20() & 0xF) + 40;
                part.width = func_150ADA68() * 10.0f + 22.0f;
                yaw = func_150ADA20();
                pitch = func_150ADA20();
                func_15143794(yaw & 0xFF, pitch % 31U - 55, func_150ADA68() * 15.0f + 25.0f, &part.vel);
                part.gravity = (func_150ADA68() * 740.0f + -1761.0f) * 0.001000000047f;
                part.color.alpha = func_150ADA20() % 101U + 155;
                func_1515C2F0(&spawn, 0, &part, 0, emitter->base.header.owner, emitter->base.header.kind);
                link->accum -= 1.0f;
            } while (link->accum > 1.0f);
        } else {
            do {
                link->accum -= 1.0f;
            } while (link->accum > 1.0f);
        }
    }
}

/* Spawns the glow sprite of `beam` (slot `slot` of `actor`), tinted (r, g, b). */
Glow *func_150EFEC8(Actor *actor, u8 slot, u8 r, u8 g, u8 b, AimedBeam *beam, u8 arg6, s32 arg7) {
    Glow *glow;
    GlowShape sprite;
    ParticleDef spawn;
    GlowLink link;

    link.actor = actor;
    link.actorId = actor->unique_id;
    link.slot = slot;
    link.beam = beam;

    spawn.texture = D_8008FCD0[12](actor, slot);
    spawn.type = 3;
    spawn.renderModes = 0x2203;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = r;
    spawn.rgba[1] = g;
    spawn.rgba[2] = b;
    spawn.rgba[3] = 0xFF;
    spawn.height = 100.0f;
    spawn.width = 100.0f;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.flags = 0xCD2002;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    spawn.unk48 = 0;

    sprite.height1 = 160.0f;
    sprite.width1 = 160.0f;
    sprite.height0 = 80.0f;
    sprite.width0 = 80.0f;
    sprite.offset1 = 0.5f;
    sprite.offset0 = 0.5f;
    sprite.alpha1 = 1.0f;
    sprite.alpha0 = 0.6999999881f;
    sprite.fadeRate = 9.999999975e-07f;
    sprite.screenX[0] = -1;
    sprite.screenY[0] = -1;
    sprite.screenX[1] = -1;
    sprite.screenY[1] = -1;
    sprite.screenX[2] = -1;
    sprite.screenY[2] = -1;
    sprite.screenX[3] = -1;
    sprite.screenY[3] = -1;
    sprite.attach = 0;
    sprite.scale = 1.0f;
    sprite.zbufSample[0] = 0;
    sprite.zbufSample[1] = 0;
    sprite.zbufSample[2] = 0;
    sprite.zbufSample[3] = 0;
    sprite.visibility[0] = 0xFF;
    sprite.visibility[1] = 0xFF;
    sprite.visibility[2] = 0xFF;
    sprite.visibility[3] = 0xFF;
    sprite.type = 10;
    sprite.handler = -1;

    glow = func_151407D0(&sprite, 0x6C, &spawn, 0x1E, 0, 0, 0, -1, arg6, arg7);
    if (glow != NULL) {
        memcpy(&glow->link, &link, sizeof(link));
    }
    return glow;
}

/* Per-frame update of a beam's glow: spans 500 units from the beam's start along its direction. */
s32 func_150F00EC(Glow *glow) {
    AimedBeam *beam;
    AimedBeamState *link;

    beam = glow->link.beam;
    link = &beam->link;
    if (beam->link.flags & 1) {
        glow->head.def.pos = beam->head.def.pos;
        glow->head.def.rot.x = beam->head.def.pos.x + (link->direction.x * 500.0f);
        glow->head.def.rot.y = beam->head.def.pos.y + (link->direction.y * 500.0f);
        glow->head.def.rot.z = beam->head.def.pos.z + (link->direction.z * 500.0f);
        glow->head.def.flags |= 6;
    } else {
        glow->head.def.flags &= ~4;
        glow->head.def.flags &= ~2;
    }
    return 1;
}

/* Spawns the muzzle flash of `beam`, colored (r, g, b, a). */
LightEffect *func_150F0198(u8 r, u8 g, u8 b, u8 a, AimedBeam *beam, u8 arg5, s32 arg6) {
    LightEffect *flash;
    LightEffectHeader header;
    Vec3i origin;
    AimedBeam *link;

    link = beam; // matching: the payload is copied from a stack local, not the arg slot
    header.flags = 2;
    header.update = -1;
    header.lifetime = 300;
    header.lightType = 0x21;
    origin.x = 0;
    origin.y = 0;
    origin.z = 0;

    flash = func_151602C0(&header, &origin, r, g, b, a, 0xFF, 0, 4, arg5, arg6);
    if (flash != NULL) {
        memcpy(&flash->data.object, &link, sizeof(link));
    }
    return flash;
}

void func_150F0260(Bolt *spark) {
    func_150F02A0(spark);
}

void func_150F0280(Bolt *spark) {
    func_150F02A0(spark);
}

/* Clears the beam's reference to this spark. */
void func_150F02A0(Bolt *spark) {
    SparkLink *link = spark->link;
    AimedBeam *beam = link->beam;

    beam->link.sparks[link->index] = NULL;
}

void func_150F02C0(BeamEmitter *emitter) {
    func_150F0318(emitter);
    func_1514933C(&emitter->base);
}

void func_150F02EC(BeamEmitter *emitter) {
    func_150F0318(emitter);
    func_15149368(&emitter->base);
}

/* Clears the beam's reference to this emitter. */
void func_150F0318(BeamEmitter *emitter) {
    AimedBeam *beam = emitter->link.beam;

    beam->link.emitter = NULL;
}

void func_150F0328(Glow *glow) {
    func_150F0380(glow);
    func_151411A4(&glow->head);
}

void func_150F0354(Glow *glow) {
    func_150F0380(glow);
    func_151411C4(&glow->head);
}

/* Clears the beam's reference to this glow. */
void func_150F0380(Glow *glow) {
    AimedBeam *beam = glow->link.beam;

    beam->link.glow = NULL;
}

void func_150F0390(LightEffect *flash) {
    func_150F03E8(flash);
    func_151617C4(flash);
}

void func_150F03BC(LightEffect *flash) {
    func_150F03E8(flash);
    func_151617E4(flash);
}

/* Clears the beam's reference to this flash. */
void func_150F03E8(LightEffect *flash) {
    AimedBeam *beam = flash->data.object;

    beam->link.flash = NULL;
}

/* Fires a projectile (func_151C229C) from slot `slot` of `actor` once actor->taskint[2]
 * reaches 90, with a spark burst and a func_15102B38 effect at the muzzle. */
Game1EF500Trail *func_150F03F8(actor, slot, arg2, arg3)
    Actor *actor;
    u8 slot;
    u8 arg2;
    s32 arg3;
{
    Game1EF500Trail *ret;
    Vec3f muzzle;
    Vec3f dir;
    const Vec3f *src[2];
    Vec3f *dst[2];
    LightEffectHeader hdr;
    Vec3i origin;
    ParticleSize params;
    s32 randB;
    f32 rand1;
    f32 rand0;
    s32 randA;

    if (actor == NULL) {
        return 0;
    }
    if (slot >= 2) {
        return 0;
    }
    if (actor->ob_matrix_ptr == NULL) {
        return 0;
    }
    if (actor->taskint[2] < 90) {
        return 0;
    }
    src[0] = &D_800A1640[slot];
    src[1] = &D_800A1658[slot];
    dst[0] = &muzzle;
    dst[1] = &dir;
    func_15145EA4(src, dst, &actor->ob_matrix_ptr[D_800A1670[slot]], 2);
    dir.x -= muzzle.x;
    dir.y -= muzzle.y;
    dir.z -= muzzle.z;
    {
        f32 func_150ADA68(void); // matching: this block's extra line steers as1 scheduling

        rand0 = func_150ADA68();
        rand1 = func_150ADA68();
        func_150ADA20();
        ret = func_151C229C(&muzzle, &dir, 0, 0, 0, 0, 150.0f, 0.006666666828f, rand0 * 19.0f + 38.0f,
                            rand1 * 363.0f + 149.0f, 50.0f, 0xFF, actor, 1, 1, 0, 8, 0, 1, 0, 0xA0038, 0.0f,
                            0xFF, -1, 0, arg2, arg3);
    }
    hdr.flags = 3;
    hdr.update = -1;
    hdr.lifetime = (func_150ADA20() % 5U) + 4;
    hdr.lightType = 0;
    origin.x = muzzle.x;
    origin.y = muzzle.y;
    origin.z = muzzle.z;
    randA = (func_150ADA20() % 51U) + 80;
    func_151602C0(&hdr, &origin, randA, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg2, arg3);
    params.height = (func_150ADA68() * 1906.0f + 969.0f) * 0.001000000047f;
    params.width = (func_150ADA68() * 911.0f + 628.0f) * 0.1000000015f;
    randA = func_150ADA20();
    randB = func_150ADA20();
    rand0 = func_150ADA68() * 2000.0f + 2000.0f;
    func_15102B38(actor, D_800A1670[slot], &D_800A1640[slot], &D_800A1658[slot], &params,
                  (randA % 3U) + 8, (randB % 1U) + 0xFF, rand0, &muzzle, 0xFF, 0, -1, arg2, arg3);
    return ret;
}

/* Message handler of a beam spark: releases it when its actor goes away (type 0)
 * and follows its actor through an identity swap (type 0x2D). */
void func_150F07E4(Bolt *spark, s32 data, u8 type) {
    SparkLink *link = spark->link;
    ActorMsg *msg = (ActorMsg *)data;
    Actor *msgActor;

    if (type == 0) {
        msgActor = msg->ref.actor;
        if ((msgActor == link->actor) || (msg->ref.uniqueId == link->actorId)) {
            func_1516972C(&spark->header);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
    }
}

/* Message handler of a beam glow; same as func_150F07E4. */
void func_150F088C(Glow *glow, ActorMsg *msg, u8 type) {
    GlowLink *link = &glow->link;
    Actor *msgActor;

    if (type == 0) {
        msgActor = msg->ref.actor;
        if ((msgActor == link->actor) || (msg->ref.uniqueId == link->actorId)) {
            func_1516972C(&glow->head.header);
        }
    } else if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->actorId = msg->swap.idA;
        }
    }
}

void func_150F0938(Actor *actor) {
    func_15160A58(actor, 0x25, &D_800A1674, 2, 300, 4, 0, 0xFF, 0, 0xFF, 0, -1, 0, 0, 0xFF, 1);
    func_15160A58(actor, 2, &D_800A1680, 2, 300, 0xD, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

/* Spawns a particle effect (func_15152B38) at `pos`; func_150EEE00 uses it at the muzzle. */
void func_150F0A24(Vec3f *pos) {
    TrailBurst packet;

    packet.countBase = 9;
    packet.countRange = 6;
    packet.pos = *pos;
    packet.sizeBase = 14.88f;
    packet.sizeRange = 14.88f;
    packet.gravityBase = -1.353000045f;
    packet.gravityRange = 0.7180000544f;
    packet.speedBase = 39.0f;
    packet.speedRange = 35.0f;
    packet.angles.yawBase = 0;
    packet.angles.yawRange = 0xFF;
    packet.angles.pitchBase = -31;
    packet.angles.pitchRange = 80;
    packet.capacityBase = 3;
    packet.capacityRange = 4;
    packet.lifeBase = 20;
    packet.lifeRange = 30;
    packet.headerFlags = 1;
    packet.flags = 4;
    packet.primMode = 2;
    packet.envMode = 3;
    packet.colorBase.red = 0xFF;
    packet.colorBase.green = 0xFF;
    packet.colorBase.blue = 0xFF;
    packet.colorBase.alpha = 0xFF;
    packet.colorRange.red = 0;
    packet.colorRange.green = 0;
    packet.colorRange.blue = 0;
    packet.colorRange.alpha = 0;
    packet.color2Base.red = 0xFF;
    packet.color2Base.green = 0xFF;
    packet.color2Base.blue = 0xFF;
    packet.color2Base.alpha = 0xFF;
    packet.color2Range.red = 0;
    packet.color2Range.green = 0;
    packet.color2Range.blue = 0;
    packet.color2Range.alpha = 0;
    packet.colorLevelBase = 0xFF;
    packet.colorLevelRange = 0;
    packet.renderMode = 3;
    packet.unk21 = 0x24;
    packet.setGeometryMode = 0x200005;
    packet.clearGeometryMode = 0x60600;
    packet.fadeStart = 8;
    packet.fadeRate = 31;
    packet.shrinkStart = 1;
    packet.shrinkPaused = 0;
    packet.shrinkFactor = 1.0f;
    packet.unk38 = -1;
    packet.unk39 = 0;
    packet.unk72 = -1;
    packet.unk73 = -1;
    func_15152B38(&packet, 0xFF, 1, pos);
}

/* Spawns a particle that follows `actor` (see func_150F0E48). */
void func_150F0BEC(Actor *actor) {
    ParticleDesc spawn;
    ActorParticleLink link;
    AttachedSpriteParticle *particle;
    s32 flipY;
    s32 flipX;

    spawn.texture = 0x2B;
    spawn.renderMode = 0x4403;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0x70600;
    spawn.timer = 300;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xD9;
    spawn.b2 = 0xA0;
    spawn.alpha = 0xFF;
    spawn.brightness = 0xFF;
    spawn.height = 250.0f;
    spawn.width = 250.0f;
    spawn.pos.x = actor->xpos;
    spawn.pos.y = actor->ypos;
    spawn.pos.z = actor->zpos;
    spawn.offset = D_800A5480;
    spawn.vel = D_800A5480;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.scaleStart = 1;
    spawn.gravity = 0.0f;
    spawn.scaleRate = 1.0f;
    if (func_150ADA20() & 1) {
        flipX = 0x40;
    } else {
        flipX = 0;
    }
    if (func_150ADA20() & 1) {
        flipY = 0x80;
    } else {
        flipY = 0;
    }
    spawn.flags = (flipY | flipX) | 0x8000 | 0x4000 | 0x40000;
    spawn.primColorMode = 6;
    spawn.envColorMode = 6;
    spawn.updateFunc = 0x25;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0xA;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    spawn.minSize = 0;
    link.actor = actor;
    spawn.maxSize = 10000.0f;
    link.actorId = actor->unique_id;
    link.phase[0] = func_150ADA68() * 2.0f * 3.141592741f;
    link.phase[1] = func_150ADA68() * 2.0f * 3.141592741f;
    link.phase[2] = func_150ADA68() * 2.0f * 3.141592741f;
    particle = func_15130280(&spawn, 1, 0, 0x14, 0xFF, 1);
    if (particle != NULL) {
        memcpy(&particle->link.actor, &link, sizeof(link));
    }
}

/* Per-frame update of an actor-following particle: tracks the actor and pulses its size and alpha. */
s32 func_150F0E48(AttachedSpriteParticle *particle, s32 arg1) {
    ActorParticleLink *link;
    Actor *actor;

    link = &particle->link.actor;
    actor = link->actor;
    if (actor->unique_id != link->actorId) {
        return 0;
    }

    particle->head.desc.pos.x = actor->xpos;
    particle->head.desc.pos.y = actor->ypos;
    particle->head.desc.pos.z = actor->zpos;
    link->phase[0] += 0.2630000114f * gFrameDelta;
    link->phase[1] += 0.3030000031f * gFrameDelta;
    link->phase[2] += 0.25f * gFrameDelta;
    link->phase[0] = func_15144B68(link->phase[0]);
    link->phase[1] = func_15144B68(link->phase[1]);
    link->phase[2] = func_15144B68(link->phase[2]);
    particle->head.desc.width = (sinf(link->phase[0]) * 243.0f) + 780.0f;
    particle->head.desc.height = (sinf(link->phase[1]) * 243.0f) + 780.0f;
    particle->head.desc.alpha = (sinf(link->phase[2]) * 50.0f) + 200.0f;
    return 1;
}

/* Message handler of an actor-following particle: follows its actor through an identity
 * swap (type 0x2D) and is released when the actor goes away (types 0 and 0x43). */
void func_150F1020(AttachedSpriteParticle *particle, s32 data, u8 type) {
    ActorParticleLink *link = &particle->link.actor;
    ActorMsg *msg = (ActorMsg *)data;
    Actor *msgActor;

    if (type == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->actorId = msg->swap.idB;
        } else {
            if (link->actor == msg->swap.actorB) {
                link->actor = msg->swap.actorA;
                link->actorId = msg->swap.idA;
            }
        // matching: the label ends a block, so IDO emits the else path's own branch to the epilogue
        swapped:;
        }
    } else if (type == 0 || type == 0x43) {
        msgActor = msg->ref.actor;
        if ((msgActor == link->actor) || (msg->ref.uniqueId == link->actorId)) {
            func_1516972C(&particle->head.header);
        }
    }
}

/* Attaches a trail emitter (func_150F1170) to `actor`. */
void func_150F10D4(Actor *actor) {
    ActorTrailLink link;
    ActorTrailEmitter *emitter;

    link.actor = actor;
    link.actorId = actor->unique_id;
    link.accum = 0.0f;
    link.lastPos.x = actor->xpos;
    link.lastPos.y = actor->ypos;
    link.lastPos.z = actor->zpos;

    emitter = func_15149130(300, -1, 0x4C, -1, 0, 0x3A, sizeof(ActorTrailLink),
                                            0xFF, 1);
    if (emitter != NULL) {
        memcpy(&emitter->link, &link, sizeof(link));
    }
}

/* Per-frame update of an actor's trail emitter: scatters particles along the path the actor moved
 * this frame; the emitter ends when the actor goes away. */
void func_150F1170(ActorTrailEmitter *emitter) {
    ActorTrailLink *link;
    Actor *actor;
    Vec3f last;
    f32 dx;
    f32 dy;
    f32 dz;
    ParticleDesc spawn;
    TrailParticleLink ext;
    AttachedSpriteParticle *particle;
    s32 flipY;

    actor = emitter->link.actor;
    last = emitter->link.lastPos;
    link = &emitter->link;
    if ((actor->type == 0) || (actor->unique_id != link->actorId)) {
        emitter->base.header.state = -1;
        return;
    }
    link->lastPos.x = actor->xpos;
    link->lastPos.y = actor->ypos;
    link->lastPos.z = actor->zpos;
    dx = link->lastPos.x - last.x;
    dy = link->lastPos.y - last.y;
    dz = link->lastPos.z - last.z;
    link->accum += (0.1090000048f + func_150ADA68() * 0.2840000093f) * gFrameDelta;
    if (link->accum > 1.0f) {
        s32 pad; // matching: unused stack slot

        ext.unkC = 0.986371994f;
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.fadeStart = 0x23;
        spawn.fadeRate = 7;
        spawn.flags = 0x90DE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        spawn.updateFunc = 0x20;
        spawn.deathFunc = -1;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 0x23;
        spawn.r = 0xDD;
        spawn.g = 0xD3;
        spawn.b = 0xCD;
        spawn.a = 0xFF;
        spawn.r2 = 0x57;
        spawn.g2 = 0x55;
        spawn.b2 = 0x5A;
        spawn.brightness = 0xFF;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.scaleRate = 1.029834032f;
        do {
            {
            f32 r;
            r = func_150ADA68();
            spawn.pos.x = dx * r + last.x;
            spawn.pos.y = dy * r + last.y;
            spawn.pos.z = dz * r + last.z;
            }
            spawn.vel.x = dx * gFrameDeltaInv * 0.3330000043f;
            spawn.vel.y = dy * gFrameDeltaInv * 0.3330000043f;
            spawn.vel.z = dz * gFrameDeltaInv * 0.3330000043f;
            ext.unk0 = func_150ADA20();
            ext.unk1 = (s16)func_150ADA20(); // matching: the s16 cast keeps the value in its own temp
            ext.unk2 = (func_150ADA20() % 5U) + 4;
            ext.unk3 = (func_150ADA20() % 5U) + 4;
            ext.unk4 = func_150ADA68() * 39.0f;
            ext.unk8 = func_150ADA68() * 39.0f;
            spawn.gravity = func_150ADA68() * 0.5139999986f + 0.6560000181f;
            spawn.flags &= ~0xC0;
            flipY = (func_150ADA20() & 1) ? 0x80 : 0;
            spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flipY;
            spawn.alpha = (func_150ADA20() % 71U) + 80;
            spawn.timer = (func_150ADA20() % 31U) + 60;
            spawn.width = spawn.height = func_150ADA68() * 514.0f + 404.0f;
            particle = func_15130280(&spawn, 1, 0, 0x10, emitter->base.header.owner, emitter->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->link.trail, &ext, sizeof(ext));
            }
            link->accum -= 1.0f;
        } while (link->accum > 1.0f);
    }
}

/* Message handler of an actor's trail emitter: released by message 0x43 for its actor,
 * other messages go to the generic func_15149514. */
void func_150F15F8(ActorTrailEmitter *emitter, s32 data, u8 type) {
    ActorTrailEmitter **self = &emitter; // matching: address-taken args stay in their stack slots
    ActorMsg **msg = (ActorMsg **)&data;
    ActorTrailLink *link;
    Actor *msgActor;

    if (type == 0x43) {
        msgActor = ((ActorMsg *)data)->ref.actor;
        link = &(*self)->link;
        if ((msgActor == link->actor) || (link->actorId == (*msg)->ref.uniqueId)) {
            func_1516972C(&(*self)->base.header);
        }
    } else {
        link = &(*self)->link;
        func_15149514(data, type, &link->actor, &link->actorId, *self);
    }
}

/* Message handler of an object bound to an actor: released by message 0x43 for that actor. */
void func_150F1684(ActorBound *self, ActorMsg *msg, u8 type) {
    ActorRef *ref = &self->ref;
    Actor *msgActor;

    if (type == 0x43) {
        msgActor = msg->ref.actor;
        if ((ref->actor == msgActor) || (msg->ref.uniqueId == ref->uniqueId)) {
            func_1516972C(&self->header);
        }
    }
}

/* Spark burst and func_15102B38 effect at slot `slot` of `actor`, muzzle variant
 * `variant` (0-3); actors with id 0x23 use the second table set. */
void func_150F16DC(actor, slot, variant, arg3, arg4)
    Actor *actor;
    u8 slot;
    u8 variant;
    u8 arg3;
    s32 arg4;
{
    Vec3f muzzle;
    u8 kind;

    if (actor == NULL) {
        return;
    }
    if (variant >= 4) {
        return;
    }
    if (slot >= 2) {
        return;
    }
    if (actor->character_type == 0x23) {
        kind = 1;
    } else {
        const u8 masks[2] = { 4, 2 };
        if (masks[slot] & actor->unk94) {
            return;
        }
        kind = 0;
    }
    {
        const Vec3f *src[1];
        Vec3f *dst[1];
        LightEffectHeader hdr;
        Vec3i origin;
        ParticleSize params;
        s32 randB;
        f32 rand0;
        s32 randA;

        if (actor->ob_matrix_ptr == NULL) {
            return;
        }
        src[0] = &D_800A168C[kind][variant][slot];
        dst[0] = &muzzle;
        func_15145EA4(src, dst, &actor->ob_matrix_ptr[D_800A180C[kind][slot]], 1);
        hdr.flags = 3;
        hdr.update = -1;
        hdr.lifetime = (func_150ADA20() % 5U) + 4;
        hdr.lightType = 0;
        origin.x = muzzle.x;
        origin.y = muzzle.y;
        origin.z = muzzle.z;
        randA = (func_150ADA20() % 51U) + 80;
        func_151602C0(&hdr, &origin, randA, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg3, arg4);
        params.height = (func_150ADA68() * 701.0f + 1925.0f) * 0.001000000047f;
        params.width = (func_150ADA68() * 1155.0f + 1526.0f) * 0.1000000015f;
        randA = func_150ADA20();
        randB = func_150ADA20();
        rand0 = func_150ADA68() * 1877.0f + 2414.0f;
        func_15102B38(actor, D_800A180C[kind][slot], &D_800A168C[kind][variant][slot],
                      &D_800A174C[kind][variant][slot], &params, (randA % 6U) + 8, (randB % 1U) + 0xFF,
                      rand0, &muzzle, 0xFF, 0, -1, arg3, arg4);
    }
}
