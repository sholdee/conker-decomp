#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
TriggerZone *D_800D9AA0[3]; // the three areas func_150FCBC0 sprays from (func_150142AC registers them)

#include "functions.h"
#include "variables.h"

/* This file's .rodata: the burst angle ranges per area, then the four vertex colours of the beam particle. */
static const s16 D_800A1EE0[3][2] = { { -40, 80 }, { -13, 63 }, { -50, 63 } };
static const VtxColor D_800A1EEC[4] = {
    { 255, 255, 255, 255, 0 },
    { 255, 255, 255, 0, 0 },
    { 255, 255, 255, 0, 0 },
    { 255, 255, 255, 255, 0 },
};

// func_150E7FEC (game_113D60.c) is declared in each file that calls it: game_1E73B0.c and game_1F4650.c were
// compiled with a different view of it.
void func_150E7FEC(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);

/* Spawns the effect at each of the three D_800D9AA0 spots, starts three positioned sounds,
 * then calls func_15164F0C for each of the D_80082FA0 + 1 entries and func_151D8868 once. */
void func_150FCA30(void) {
    s32 i;
    RumbleParams params;

    func_150FCBC0(0);
    func_150FCBC0(1);
    func_150FCBC0(2);

    // sound_addDynamicSound(sound, x, y, z, volume, farDistance, nearDistance, callback, owner, key, flags, cents)
    sound_addDynamicSound(0x2B8, -2038, 1560, -2631, 28000, 9000, 7000, sound_delayedPlayCallback, (void *)45, 0, 0, 0);
    sound_addDynamicSound(0x2B7, -319, -273, -3419, 28000, 9000, 7000, sound_delayedPlayCallback, NULL, 0, 0, 0);
    sound_addDynamicSound(0x2B6, 1519, 947, -2633, 28000, 9000, 7000, sound_delayedPlayCallback, (void *)90, 0, 0, 0);

    for (i = 0; i < D_80082FA0 + 1; i++) {
        func_15164F0C(4, i, 0, 0xFF, 1);
    }

    params.flags = 1;
    params.duration = 100;
    params.playerMask = 15;
    params.strength = 8;
    params.updateKind = -1;
    func_151D8868(&params, 0, 0xFF, 0);
}


/* Spawns an effect in the middle of area D_800D9AA0[index] and 3-4 particle bursts at random angles around it. */
void func_150FCBC0(u8 index) {
    TriggerZone *area;
    Vec3f pos;
    const s16 *range;
    JetSpraySpec desc;
    s16 count;
    u8 angle;
    u32 randInt;
    f32 randFrac;

    if (index >= 3) {
        return;
    }
    area = D_800D9AA0[index];
    if (area == NULL) {
        return;
    }
    pos.x = area->x;
    pos.y = area->y + area->ySize * 0.5f;
    pos.z = area->z;
    randFrac = func_150ADA68();
    func_151541B8(&pos, randFrac * 4.0f + 12.0f, 6.6943097f, func_150ADA20() % 56U + 200, 2.6090002f, 0xFF, 1);
    count = (func_150ADA20() & 1) + 3;
    if (count > 0) {
        range = D_800A1EE0[index];
        desc.angles.yawRange = 80;
        desc.angles.pitchBase = -32;
        desc.angles.pitchRange = 43;
        desc.spray.countBase = 1;
        desc.spray.countRange = 8;
        desc.spray.speedBase = 249.0f;
        desc.spray.speedRange = 451.0f;
        desc.spray.sizeBase = 506.0f;
        desc.spray.sizeRange = 100.0f;
        desc.spray.sizeSlopeBase = 1505.0f;
        desc.spray.sizeSlopeRange = 1511.0f;
        desc.spray.particleCountBase = 3;
        desc.spray.particleCountRange = 2;
        desc.spray.spread = 90.0f;
        desc.spray.drag = 0.813f;
        desc.spray.gravityBase = -2.0f;
        desc.spray.gravityRange = 1.085f;
        desc.spray.delayBase = 12;
        desc.spray.delayRange = 50;
        desc.spray.alphaBase = 100;
        desc.spray.alphaRange = 100;
        desc.spray.fadeStart = 12;
        desc.spray.fadeOutStart = 12;
        desc.spray.eventFunc = -1;
        do {
            randInt = func_150ADA20();
            angle = range[0] + randInt % (range[1] + 1);
            desc.angles.yawBase = angle - 40;
            func_15143874(angle, area->xSize, &desc.spray.pos.x, &desc.spray.pos.z);
            desc.spray.pos.x += area->x;
            desc.spray.pos.z += area->z;
            desc.spray.pos.y = func_150ADA68() * area->ySize + area->y;
            func_1514FCE8(&desc, 0xFF, 1);
            count--;
        } while (count > 0);
    }
}

/* func_15165BB0 of arg0 against the centre of area 0 (1.0f when there is no area 0). */
f32 func_150FCF1C(void *arg0) {
    TriggerZone *area;
    Vec3f pos;

    area = D_800D9AA0[0];
    if (area == NULL) {
        return 1.0f;
    }
    pos.x = area->x;
    pos.y = area->y;
    pos.z = area->z;
    return func_15165BB0(arg0, &pos, 2007.0f, 9005.0f, 0.00011104942f);
}

void func_150FCFB0(s32 arg0) {
    sound_updateCooldowns();
}


/* State that follows the particle (its DefParticleMotion slot at 0x110). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;      /* owner->unique_id when the link was made */
    /* 0x08 */ f32 maxWidth;
    /* 0x0C */ f32 timer;       /* counts down by gFrameDelta; the particle dies below 0 */
    /* 0x10 */ f32 pulsePhase;
    /* 0x14 */ f32 texS0;       /* S texture coordinate of corners 0 and 3 */
    /* 0x18 */ f32 texS1;       /* S texture coordinate of corners 1 and 2 */
    /* 0x1C */ f32 texPhase0;
    /* 0x20 */ f32 texPhase1;
    /* 0x24 */ LightEffect *light; /* from func_1516284C, released with func_1516972C */
    /* 0x28 */ Actor150FD514 *groundParticle;
    /* 0x2C */ f32 emitCounter;
    /* 0x30 */ CollisionHit ground;
    /* 0x54 */ u16 soundHandle;
    /* 0x56 */ u16 soundHandle2;
    /* 0x58 */ u8 burstDone;
    /* 0x59 */ u8 onGround;
} Ext150FCFD4; /* 0x5C */

/* A particle (Particle in game_169510.c) with Ext150FCFD4 as its motion state. */
struct Actor150FD514 {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ Ext150FCFD4 ext;
};

/*
 * Spawns a particle that follows obj, standing on the ground below it when the probe finds ground, with a
 * positioned sound, a func_1516284C effect and (on ground) a second particle lying on the ground.
 */
void func_150FCFD4(Actor *obj, u8 arg1, s32 arg2) {
    CollisionHit ground;
    Vec3f pos;
    u8 onGround;
    Vec3f probe;
    ParticleDef def;
    Ext150FCFD4 ext;
    Actor150FD514 *particle;
    Ext150FCFD4 *dst;
    LightEffectHeader header;
    Vec3i posInt;
    ParticleDef groundDef;
    s32 flag;

    if (obj->unk7 < 0xFF) {
        return;
    }
    pos.x = obj->xpos;
    probe.x = pos.x;
    probe.y = obj->ypos + 100.0f;
    pos.z = obj->zpos;
    probe.z = pos.z;
    func_1504715C(&ground, obj);
    if (func_15046C80(&probe, 0, -10000.0f, &ground)) {
        pos.y = ground.height;
        onGround = 1;
    } else {
        pos.y = obj->ypos;
        onGround = 0;
    }
    actor_hitScripted(obj, 0x39, obj->langle, -1);
    ext.owner = obj;
    ext.ownerId = obj->unique_id;
    ext.maxWidth = 60.0f;
    ext.timer = 50.0f;
    ext.pulsePhase = func_150ADA68() * 6.2831855f;
    ext.burstDone = 0;
    ext.texS0 = -2048.0f;
    ext.texS1 = -2048.0f;
    ext.texPhase0 = func_150ADA68() * 6.2831855f;
    ext.texPhase1 = func_150ADA68() * 6.2831855f;
    ext.light = NULL;
    ext.groundParticle = NULL;
    ext.emitCounter = 0.0f;
    ext.ground = ground;
    ext.soundHandle = sound_addDynamicSound(0x69E, pos.x, pos.y, pos.z, 32000, 3000, 500, 0, 0, 0, 0, 0);
    flag = onGround ? 1 : 0; // matching: the s32 temp keeps IDO's branchy 0/1 select
    ext.onGround = flag;
    def.texture = 0x59;
    def.type = 0xB;
    def.renderModes = 0x5B1A;
    def.lifetime = 300;
    def.frame = 0;
    def.frameSpeed = 0;
    def.rgba[0] = 0xFF;
    def.rgba[1] = 0;
    def.rgba[2] = 0;
    def.rgba[3] = 0xFF;
    def.unk44 = 0xFF;
    def.width = 0.0f;
    def.height = 1500.0f;
    def.pos = pos;
    def.rot = D_800A5480;
    def.scale.x = 1.0f;
    def.scale.y = 0.0f;
    def.scale.z = 1.0f;
    def.flags = 0x24EC0008;
    def.unk45 = 0xFF;
    def.unk46 = 0;
    def.unk47 = 6;
    def.unk48 = 0;
    def.viewportMask = 0xFF;
    def.unk50 = 0;
    def.fadeStart = 1;
    def.fadeRate = 0xFF;
    particle = func_1513D2F0(&def, D_800A1EEC, 0, 0x2B, 0, 0x24, 0, 0, 0, 0x5C, arg1, arg2);
    if (particle != NULL) {
        dst = &particle->ext;
        memcpy(dst, &ext, sizeof(ext));
        header.flags = 2;
        header.update = 2;
        header.lifetime = 300;
        header.lightType = 6;
        posInt.x = pos.x;
        posInt.y = pos.y;
        posInt.z = pos.z;
        dst->light = func_1516284C(&header, &posInt, 0xFF, 0, 0, 0xFF, 0, 0, 0x17, arg1, arg2);
        // matching: this block and its redundant extern declarations are -g3 scope barriers that keep the
        // light store and the onGround test ahead of the ground particle's constants (plain if mismatches)
        {
            extern const VtxColor D_800A4AA0[4];

            if (onGround) {
                extern const Vec3f D_800A5480;

                groundDef.texture = 3;
                groundDef.type = 0;
                groundDef.renderModes = 0x3103;
                groundDef.lifetime = 300;
                groundDef.frame = 0;
                groundDef.frameSpeed = 0;
                groundDef.rgba[0] = 0xFF;
                groundDef.rgba[1] = 0;
                groundDef.rgba[2] = 0;
                groundDef.rgba[3] = 0xFF;
                groundDef.unk44 = 0xFF;
                groundDef.width = 1.0f;
                groundDef.height = 1.0f;
                groundDef.pos.x = pos.x;
                groundDef.pos.y = pos.y + 5.0f;
                groundDef.pos.z = pos.z;
                groundDef.rot = D_800A5480;
                groundDef.scale.x = 1.0f;
                groundDef.scale.y = 1.0f;
                groundDef.scale.z = 1.0f;
                groundDef.flags = 0x04EC0008;
                groundDef.unk45 = 0xFF;
                groundDef.unk46 = 0;
                groundDef.unk47 = 6;
                groundDef.unk48 = 0;
                groundDef.viewportMask = 0xFF;
                groundDef.unk50 = 0;
                groundDef.fadeStart = 1;
                groundDef.fadeRate = 0xFF;
                dst->groundParticle = func_1513D594(&groundDef, &D_800A4AA0, 0, 0, 0, 0, 0, 50.0f, 50.0f, 0, &ground.tri, 0, 0, 0, 0, arg1, arg2);
            }
        }
    }
}

/*
 * Per-frame update of the func_150FCFD4 particle: follows its owner (calling func_15107B78 on it at a random
 * rate), grows to maxWidth while its timer falls from 50 to 20 and shrinks below 10, firing a
 * one-time burst as it starts shrinking, pulses def.unk44 and scrolls its texture. Returns 0 once expired.
 */
s32 func_150FD514(Actor150FD514 *particle) {
    Actor *owner;
    Ext150FCFD4 *ext;
    f32 z;
    f32 x;
    Vec3f groundPos;

    ext = &particle->ext;
    owner = ext->owner;
    if (owner != NULL) {
        if (owner->type == 0 || owner->unique_id != ext->ownerId) {
            ext->owner = NULL;
            owner = NULL;
        }
        /* BUG (original game): owner may have just been set to NULL above, and is dereferenced anyway */
        x = owner->xpos;
        z = owner->zpos;
    } else {
        x = particle->head.def.pos.x;
        z = particle->head.def.pos.z;
    }
    if (owner != NULL) {
        actor_flashFog(owner, 0xFF, 0, 0, 0xFF, 0, 4);
        ext->emitCounter += (0.091000006f + func_150ADA68() * 0.07f) * gFrameDelta;
        while (ext->emitCounter > 1.0f) {
            func_15107B78(owner, func_150ADA20() & 0xFF, (func_150ADA20() % 101) - 63, particle->head.header.owner, particle->head.header.kind);
            ext->emitCounter -= 1.0f;
        }
    }
    if (ext->timer > 20.0f) {
        particle->head.def.width = (50.0f - ext->timer) * 0.033333335f * ext->maxWidth;
    } else if (ext->timer > 10.0f) {
        particle->head.def.width = ext->maxWidth;
    } else {
        particle->head.def.width = ext->timer * 0.1f * ext->maxWidth;
        if (ext->burstDone == 0) {
            if (owner != NULL) {
                actor_hitScripted(owner, 0x16, owner->langle, -1);
                func_10010F88(0x69D, 32000, 0, 0, 0, particle->head.def.pos.x, particle->head.def.pos.y, particle->head.def.pos.z, 500, 3000);
            }
            ext->burstDone = 1;
            func_151D5404(&particle->head.def.pos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, particle->head.header.owner, particle->head.header.kind);
            func_151D5334(&particle->head.def.pos, 506.0f, 1013.0f, 0.0009871669f, 5, particle->head.header.owner, particle->head.header.kind);
            if (ext->onGround & 1) {
                groundPos.x = particle->head.def.pos.x;
                groundPos.y = ext->ground.height;
                groundPos.z = particle->head.def.pos.z;
                func_150E7FEC(func_150ADA68() * 78.0f + 125.0f, func_150ADA20() % 76 + 180, &ext->ground.tri, &groundPos,
                              (func_150ADA20() % 207) + 302, 0, 1, 0, 0, 0, particle->head.header.owner, 0);
                func_150E83AC(&groundPos, (func_150ADA20() % 80) + 243, particle->head.header.owner, particle->head.header.kind);
            }
        }
    }
    particle->head.def.unk44 = sinf(ext->pulsePhase) * 57.0f + 198.0f;
    ext->pulsePhase += 0.28500003f * gFrameDelta;
    ext->pulsePhase = func_15144B68(ext->pulsePhase);
    ext->texPhase0 += 0.10700001f * gFrameDelta;
    ext->texPhase1 += -0.086f * gFrameDelta;
    ext->texPhase0 = func_15144B68(ext->texPhase0);
    ext->texPhase1 = func_15144B68(ext->texPhase1);
    ext->texS0 = func_150AD78C(ext->texPhase0) * 1666.0f + -4633.0f;
    ext->texS1 = func_150AD78C(ext->texPhase1) * 1192.0f + 1242.0f;
    if (ext->groundParticle != NULL) {
        ext->groundParticle->head.def.width = ext->groundParticle->head.def.height = particle->head.def.width * 0.02f;
        ext->groundParticle->head.def.unk44 = particle->head.def.unk44;
        ext->groundParticle->head.def.pos.x = x;
        ext->groundParticle->head.def.pos.z = z;
    }
    particle->head.def.pos.x = x;
    particle->head.def.pos.z = z;
    ext->timer -= gFrameDelta;
    if (ext->timer < 0.0f) {
        return 0;
    }
    return 1;
}


/* Message handler: retargets (0x2D, an actor swap) or drops (0, the actor went away) the owner link. */
void func_150FDB0C(Actor150FD514 *actor, ActorMsg *msg, u8 eventId) {
    Ext150FCFD4 *link = &actor->ext;

    if (eventId == 0x2D) {
        if (link->owner == msg->swap.actorA) {
            link->owner = msg->swap.actorB;
            link->ownerId = msg->swap.idB;
        } else if (link->owner == msg->swap.actorB) {
            Ext150FCFD4 *dst = link; // matching: the aliased scope keeps IDO from filling the jr delay slot

            dst->owner = msg->swap.actorA;
            dst->ownerId = msg->swap.idA;
        }
    } else if (eventId == 0) {
        Actor *owner = msg->ref.actor;

        if (owner == link->owner || msg->ref.uniqueId == link->ownerId) {
            link->owner = NULL;
        }
    }
}

/* Draws the actor's camera-facing quad, then sets the corners' S texture coordinates to its scroll values. */
Vtx *func_150FDBA0(Actor150FD514 *actor, s16 viewport) {
    Vtx *vertices;

    vertices = func_1513EDE4(actor, viewport);
    if (vertices != NULL) {
        vertices[0].v.tc[0] = actor->ext.texS0;
        vertices[1].v.tc[0] = actor->ext.texS1;
        vertices[2].v.tc[0] = actor->ext.texS1;
        vertices[3].v.tc[0] = actor->ext.texS0;
    }
    return vertices;
}

/* Releases the effects and sound handles this actor's ext owns. */
void func_150FDC2C(Actor150FD514 *actor) {
    Ext150FCFD4 *ext = &actor->ext;

    if (ext->light != NULL) {
        func_1516972C(ext->light);
    }
    if (ext->groundParticle != NULL) {
        func_1516972C(&ext->groundParticle->head.header);
    }
    if (ext->soundHandle != 0) {
        sound_stop(ext->soundHandle);
    }
    if (ext->soundHandle2 != 0) {
        sound_stop(ext->soundHandle2);
    }
}

/* Releases the ext resources, then hands the particle to func_1513CA6C. */
void func_150FDCAC(Actor150FD514 *actor) {
    func_150FDC2C(actor);
    func_1513CA6C(&actor->head);
}

/* Releases the ext resources, then hands the particle to func_1513CAA0. */
void func_150FDCD8(Actor150FD514 *actor) {
    func_150FDC2C(actor);
    func_1513CAA0(&actor->head);
}
