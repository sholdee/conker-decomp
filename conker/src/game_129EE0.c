#include <ultra64.h>
// functions.h declares func_1513EDE4 with the wrong return type (it returns its Vtx *), so its
// declaration is renamed away and redeclared below.
#define func_1513EDE4 func_1513EDE4_orig
#include "functions.h"
#undef func_1513EDE4
#include "variables.h"
#include "effects.h"

extern void func_150FCBC0(u8 arg0);
extern s32 func_1000EC24();
extern void func_15164F0C(s32 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Parameter block for func_151D8868. */
typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
} Params151D8868;

/* Spawns the effect at each of the three D_800D9AA0 spots, starts three positioned sounds,
 * then calls func_15164F0C for each of the D_80082FA0 + 1 entries and func_151D8868 once. */
void func_150FCA30(void) {
    s32 i;
    Params151D8868 params;

    func_150FCBC0(0);
    func_150FCBC0(1);
    func_150FCBC0(2);

    // func_1000FA64(sound, x, y, z, volume, farDistance, nearDistance, callback, owner, key, flags, cents)
    func_1000FA64(0x2B8, -2038, 1560, -2631, 28000, 9000, 7000, (s32)func_1000EC24, (void *)45, 0, 0, 0);
    func_1000FA64(0x2B7, -319, -273, -3419, 28000, 9000, 7000, (s32)func_1000EC24, NULL, 0, 0, 0);
    func_1000FA64(0x2B6, 1519, 947, -2633, 28000, 9000, 7000, (s32)func_1000EC24, (void *)90, 0, 0, 0);

    for (i = 0; i < D_80082FA0 + 1; i++) {
        func_15164F0C(4, i, 0, 0xFF, 1);
    }

    params.unk0 = 1;
    params.unk2 = 100;
    params.unk5 = 15;
    params.unk4 = 8;
    params.unk6 = -1;
    func_151D8868(&params, 0, 0xFF, 0);
}

typedef struct {
    /* 0x00 */ s16 angle;
    /* 0x02 */ s16 spread;
    /* 0x04 */ s16 pitch;
    /* 0x06 */ s16 pitchSpread;
    /* 0x08 */ s32 count;
    /* 0x0C */ s32 countSpread;
    /* 0x10 */ struct17 position;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8 unk58;
} ParticleBurstDesc; /* emitter description passed to func_1514FCE8 */


extern const s16 D_800A1EE0[3][2];
void *func_151541B8(struct17 *, f32, f32, f32, f32, u8, s32);
extern void func_15143874(s32, f32, f32 *, f32 *);
extern void func_1514FCE8(ParticleBurstDesc *, u8, s32);

/* Spawns an effect in the middle of area D_800D9AA0[index] and 3-4 particle bursts at random angles around it. */
void func_150FCBC0(u8 index) {
    struct134 *area;
    struct17 pos;
    const s16 *range;
    ParticleBurstDesc desc;
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
    pos.unk0 = area->unk0;
    pos.unk4 = (s16)area->unk2 + (s16)area->unk8 * 0.5f;
    pos.unk8 = area->unk4;
    randFrac = func_150ADA68();
    func_151541B8(&pos, randFrac * 4.0f + 12.0f, 6.6943097f, (f32)(func_150ADA20() % 56U + 200), 2.6090002f, 0xFF, 1);
    count = (func_150ADA20() & 1) + 3;
    if (count > 0) {
        range = D_800A1EE0[index];
        desc.spread = 80;
        desc.pitch = -32;
        desc.pitchSpread = 43;
        desc.count = 1;
        desc.countSpread = 8;
        desc.unk1C = 249.0f;
        desc.unk20 = 451.0f;
        desc.unk24 = 506.0f;
        desc.unk28 = 100.0f;
        desc.unk2C = 1505.0f;
        desc.unk30 = 1511.0f;
        desc.unk34 = 3;
        desc.unk38 = 2;
        desc.unk3C = 90.0f;
        desc.unk40 = 0.813f;
        desc.unk44 = -2.0f;
        desc.unk48 = 1.085f;
        desc.unk4C = 12;
        desc.unk4E = 50;
        desc.unk50 = 100;
        desc.unk52 = 100;
        desc.unk54 = 12;
        desc.unk56 = 12;
        desc.unk58 = -1;
        do {
            randInt = func_150ADA20();
            angle = range[0] + randInt % (range[1] + 1);
            desc.angle = angle - 40;
            func_15143874((s16)angle, (s16)area->unk6, &desc.position.unk0, &desc.position.unk8);
            desc.position.unk0 += area->unk0;
            desc.position.unk8 += area->unk4;
            desc.position.unk4 = func_150ADA68() * (s16)area->unk8 + (s16)area->unk2;
            func_1514FCE8(&desc, 0xFF, 1);
            count--;
        } while (count > 0);
    }
}

extern f32 func_15165BB0(void *arg0, struct17 *pos, f32 arg2, f32 arg3, f32 arg4);

/* func_15165BB0 of arg0 against the centre of area 0 (1.0f when there is no area 0). */
f32 func_150FCF1C(void *arg0) {
    struct134 *area;
    struct17 pos;

    area = D_800D9AA0[0];
    if (area == NULL) {
        return 1.0f;
    }
    pos.unk0 = area->unk0;
    pos.unk4 = (s16)area->unk2;
    pos.unk8 = area->unk4;
    return func_15165BB0(arg0, &pos, 2007.0f, 9005.0f, 0.00011104942f);
}

void func_150FCFB0(s32 arg0) {
    func_15103828();
}

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D[0x7];
} GroundHit; /* 0x24 */

typedef struct Actor150FD514 Actor150FD514;

/* State that follows the particle (its ParticleMotion slot at 0x110). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;      /* owner->unique_id when the link was made */
    /* 0x08 */ f32 maxWidth;
    /* 0x0C */ f32 timer;       /* counts down by D_800BE9A4; the particle dies below 0 */
    /* 0x10 */ f32 pulsePhase;
    /* 0x14 */ f32 texS0;       /* S texture coordinate of corners 0 and 3 */
    /* 0x18 */ f32 texS1;       /* S texture coordinate of corners 1 and 2 */
    /* 0x1C */ f32 texPhase0;
    /* 0x20 */ f32 texPhase1;
    /* 0x24 */ s32 unk24;       /* from func_1516284C, released with func_1516972C */
    /* 0x28 */ Actor150FD514 *groundParticle;
    /* 0x2C */ f32 emitCounter;
    /* 0x30 */ GroundHit ground;
    /* 0x54 */ u16 soundHandle;
    /* 0x56 */ u16 soundHandle2;
    /* 0x58 */ u8 burstDone;
    /* 0x59 */ u8 onGround;
} Ext150FCFD4; /* 0x5C */

/* A particle (Particle in game_169510.c) with Ext150FCFD4 as its motion state. */
struct Actor150FD514 {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xB];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0xA0];
    /* 0x110 */ Ext150FCFD4 ext;
};

extern Vtx *func_1513EDE4(Actor150FD514 *particle, s16 viewport);
extern s32 D_800A1EEC[];
extern void func_1504715C(GroundHit *, Actor *);
extern s32 func_15046C80(struct17 *, s32, f32, GroundHit *);
extern void func_1505D024(Actor *, s32, u16, s32);
extern s32 func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);

/*
 * Spawns a particle that follows obj, standing on the ground below it when the probe finds ground, with a
 * positioned sound, a func_1516284C effect and (on ground) a second particle lying on the ground.
 */
void func_150FCFD4(Actor *obj, u8 arg1, s32 arg2) {
    GroundHit ground;
    vertex pos;
    u8 onGround;
    struct17 probe;
    ParticleDef def;
    Ext150FCFD4 ext;
    Actor150FD514 *particle;
    Ext150FCFD4 *dst;
    Header header;
    s32 posInt[3];
    ParticleDef groundDef;
    s32 flag;

    if (obj->unk7 < 0xFF) {
        return;
    }
    pos.x = obj->x_position;
    probe.unk0 = pos.x;
    probe.unk4 = obj->y_position + 100.0f;
    pos.z = obj->z_position;
    probe.unk8 = pos.z;
    func_1504715C(&ground, obj);
    if (func_15046C80(&probe, 0, -10000.0f, &ground)) {
        pos.y = ground.height;
        onGround = 1;
    } else {
        pos.y = obj->y_position;
        onGround = 0;
    }
    func_1505D024(obj, 0x39, obj->unk7A, -1);
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
    ext.unk24 = 0;
    ext.groundParticle = NULL;
    ext.emitCounter = 0.0f;
    ext.ground = ground;
    ext.soundHandle = func_1000FA64(0x69E, pos.x, pos.y, pos.z, 32000, 3000, 500, 0, 0, 0, 0, 0);
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
    def.rot = *(vertex *)&D_800A5480;
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
        header.unk0 = 2;
        header.unk1 = 2;
        header.unk2 = 300;
        header.unk4 = 6;
        posInt[0] = pos.x;
        posInt[1] = pos.y;
        posInt[2] = pos.z;
        dst->unk24 = func_1516284C(&header, posInt, 0xFF, 0, 0, 0xFF, 0, 0, 0x17, arg1, arg2);
        // matching: this block and its redundant extern declarations are -g3 scope barriers that keep the
        // unk24 store and the onGround test ahead of the ground particle's constants (plain if mismatches)
        {
            extern s32 D_800A4AA0;

            if (onGround) {
                extern f32 D_800A5480;

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
                groundDef.rot = *(vertex *)&D_800A5480;
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
                dst->groundParticle = (Actor150FD514 *)func_1513D594(&groundDef, &D_800A4AA0, 0, 0, 0, 0, 0, 50.0f, 50.0f, 0, (s32)&ground.unk04, 0, 0, 0, 0, arg1, arg2);
            }
        }
    }
}

extern void func_1502EA98(Actor *, u8, u8, u8, u8, s32, u8);
extern void func_15107B78(Actor *, s32, s32, s32, s32);
extern void func_151D5334(struct17 *, f32, f32, f32, s32, u8, s32);
extern void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150E83AC(struct17 *, s16, u8, s32);
extern f32 sinf(f32);

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
    struct17 groundPos;

    ext = &particle->ext;
    owner = ext->owner;
    if (owner != NULL) {
        if (owner->interaction_state == 0 || owner->unique_id != ext->ownerId) {
            ext->owner = NULL;
            owner = NULL;
        }
        /* BUG (original game): owner may have just been set to NULL above, and is dereferenced anyway */
        x = owner->x_position;
        z = owner->z_position;
    } else {
        x = particle->def.pos.x;
        z = particle->def.pos.z;
    }
    if (owner != NULL) {
        func_1502EA98(owner, 0xFF, 0, 0, 0xFF, 0, 4);
        ext->emitCounter += (0.091000006f + func_150ADA68() * 0.07f) * D_800BE9A4;
        while (ext->emitCounter > 1.0f) {
            func_15107B78(owner, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 101) - 63), particle->unkC, particle->unk1);
            ext->emitCounter -= 1.0f;
        }
    }
    if (ext->timer > 20.0f) {
        particle->def.width = (50.0f - ext->timer) * 0.033333335f * ext->maxWidth;
    } else if (ext->timer > 10.0f) {
        particle->def.width = ext->maxWidth;
    } else {
        particle->def.width = ext->timer * 0.1f * ext->maxWidth;
        if (ext->burstDone == 0) {
            if (owner != NULL) {
                func_1505D024(owner, 0x16, owner->unk7A, -1);
                func_10010F88(0x69D, 32000, 0, 0, 0, particle->def.pos.x, particle->def.pos.y, particle->def.pos.z, 500, 3000);
            }
            ext->burstDone = 1;
            func_151D5404(&particle->def.pos, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, particle->unkC, particle->unk1);
            func_151D5334(&particle->def.pos, 506.0f, 1013.0f, 0.0009871669f, 5, particle->unkC, particle->unk1);
            if (ext->onGround & 1) {
                groundPos.unk0 = particle->def.pos.x;
                groundPos.unk4 = ext->ground.height;
                groundPos.unk8 = particle->def.pos.z;
                func_150E7FEC(func_150ADA68() * 78.0f + 125.0f, (u8)((func_150ADA20() % 76) + 180), &ext->ground.unk04, &groundPos,
                              (func_150ADA20() % 207) + 302, 0, 1, 0, 0, 0, particle->unkC, 0);
                func_150E83AC(&groundPos, (func_150ADA20() % 80) + 243, particle->unkC, particle->unk1);
            }
        }
    }
    particle->def.unk44 = sinf(ext->pulsePhase) * 57.0f + 198.0f;
    ext->pulsePhase += 0.28500003f * D_800BE9A4;
    ext->pulsePhase = func_15144B68(ext->pulsePhase);
    ext->texPhase0 += 0.10700001f * D_800BE9A4;
    ext->texPhase1 += -0.086f * D_800BE9A4;
    ext->texPhase0 = func_15144B68(ext->texPhase0);
    ext->texPhase1 = func_15144B68(ext->texPhase1);
    ext->texS0 = func_150AD78C(ext->texPhase0) * 1666.0f + -4633.0f;
    ext->texS1 = func_150AD78C(ext->texPhase1) * 1192.0f + 1242.0f;
    if (ext->groundParticle != NULL) {
        ext->groundParticle->def.width = ext->groundParticle->def.height = particle->def.width * 0.02f;
        ext->groundParticle->def.unk44 = particle->def.unk44;
        ext->groundParticle->def.pos.x = x;
        ext->groundParticle->def.pos.z = z;
    }
    particle->def.pos.x = x;
    particle->def.pos.z = z;
    ext->timer -= D_800BE9A4;
    if (ext->timer < 0.0f) {
        return 0;
    }
    return 1;
}


/* Event handler: retargets (0x2D) or drops (0) the owner link {owner, unique_id} at the start of ext. */
void func_150FDB0C(Actor150FD514 *actor, struct212 *event, u8 eventId) {
    struct212 *link = (struct212 *)&actor->ext;

    if (eventId == 0x2D) {
        if (link->unk0 == event->unk0) {
            link->unk0 = event->unk4.i.unk0;
            link->unk4.b.unk0 = event->unk9;
        } else if (link->unk0 == event->unk4.i.unk0) {
            struct212 *dst = link; // matching: the aliased scope keeps IDO from filling the jr delay slot

            dst->unk0 = event->unk0;
            dst->unk4.b.unk0 = event->unk8;
        }
    } else if (eventId == 0) {
        s32 owner = event->unk0;

        if (owner == link->unk0 || event->unk4.b.unk0 == link->unk4.b.unk0) {
            link->unk0 = 0;
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

    if (ext->unk24 != 0) {
        func_1516972C((struct102 *)ext->unk24);
    }
    if (ext->groundParticle != NULL) {
        func_1516972C((struct102 *)ext->groundParticle);
    }
    if (ext->soundHandle != 0) {
        func_100111C8(ext->soundHandle);
    }
    if (ext->soundHandle2 != 0) {
        func_100111C8(ext->soundHandle2);
    }
}

/* Releases the ext resources, then hands the particle to func_1513CA6C. */
void func_150FDCAC(Actor150FD514 *actor) {
    func_150FDC2C(actor);
    func_1513CA6C((struct210 *)actor);
}

/* Releases the ext resources, then hands the particle to func_1513CAA0. */
void func_150FDCD8(Actor150FD514 *actor) {
    func_150FDC2C(actor);
    func_1513CAA0((struct210 *)actor);
}
