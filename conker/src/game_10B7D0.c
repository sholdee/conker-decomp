#include <ultra64.h>
/* functions.h has placeholder prototypes for these (void returns, no args); hide them while it is
 * included and declare the real ones below. */
#include "functions.h"
#include "variables.h"


void func_150DE320(s32 arg0) {
}

/* For each of eight zones records that zone_containsPoint accepts, sets the moving state of the
 * matching gfx_findObjectById object, or (for records without one) calls zone_setDisabled(id, 1). */
void func_150DE32C(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    WorldObject *prop;
    u8 ids[8] = { 0x18, 0x19, 0x1A, 0x1B, 0x0D, 0x30, 0x31, 0x32 };
    u8 propIds[8] = { 0xF6, 0xF3, 0xF0, 0xED, 0, 0, 0, 0 };

    for (i = 0; i < 8; i++) {
        if (zone_containsPoint(&zones[ids[i]], arg0, arg1, arg2) != 0) {
            if (propIds[i] != 0) {
                prop = gfx_findObjectById(propIds[i]);
                prop->task_flag = (prop->task_flag & ~TASK_FLAG_STATE) | TASK_FLAG_ON;
            } else {
                zone_setDisabled(ids[i], 1);
            }
        }
    }
}

/* Per-frame update: while moving, spins the rotation (wrapping each angle), applies gravity and
 * velocity (moveX..moveZ, gravity in param) to the position and counts alpha down as a lifetime
 * until inactive; kind 0xC props also get two assets of D_80090204 row touch.i.a passed to texture_addBinding. */
void func_150DE458(WorldObject *prop) {
    s32 pad[4]; // matching: unused frame space above size
    s32 size;
    f32 frames;
    f32 angle;
    s32 timer;
    s32 addr;
    s32 *asset;
    s32 slot;
    s32 nextSlot;

    if ((prop->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        frames = D_800BE9E4;

        prop->t += prop->spinX * frames;
        angle = prop->t;
        if (angle < 0.0f) {
            prop->t = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->t = angle - 360.0f;
        }

        prop->u += prop->spinY * frames;
        angle = prop->u;
        if (angle < 0.0f) {
            prop->u = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->u = angle - 360.0f;
        }

        prop->v += prop->spinZ * frames;
        angle = prop->v;
        if (angle < 0.0f) {
            prop->v = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->v = angle - 360.0f;
        }

        timer = prop->alpha;
        prop->moveY -= prop->moveData.word * D_800BE9E4;
        prop->x += prop->moveX * D_800BE9E4;
        prop->y += prop->moveY * D_800BE9E4;
        prop->z += prop->moveZ * D_800BE9E4;

        timer -= D_800BE9E4;
        if (timer > 0) {
            prop->alpha = timer;
        } else {
            prop->inactive = 1;
        }
    }

    if ((prop->modelId & ~0x8000) == 0xC) {
        // matching: a for (i = 0; i < 2; i++) form (slots i * 2 + 4, i * 2 + 5) grows the frame by 8;
        // the original steps the asset pointer and both slot numbers separately
        asset = D_80090204[prop->touch.i.a];
        slot = 4;
        nextSlot = 5;
        do {
            addr = texture_getAddress(*asset, &size, 3, 0);
            texture_addBinding(prop, addr, size + addr - 0x200, slot, nextSlot);
            nextSlot += 2;
            asset++;
            slot += 2;
        } while (nextSlot != 9);
    }
}

/* Sways the object: y bobs around 1130 and rotX / rotZ rock, each driven by func_15048A40 of its own
 * phase (touch.i.a/b/c, in 1/8 steps of its angle); the phases advance 12, 16 and 24 per frame. */
void func_150DE6D8(WorldObject *prop) {
    prop->y = func_15048A40(prop->touch.i.a >> 3) * 10.19999981f + 1130.0f;
    prop->t = func_15048A40(prop->touch.i.b >> 3) * 1.100000024f;
    prop->v = func_15048A40(prop->touch.i.c >> 3) * 1.100000024f;
    prop->touch.i.a += D_800BE9E4 * 12;
    prop->touch.i.b += D_800BE9E4 * 16;
    prop->touch.i.c += D_800BE9E4 * 24;
}

/* State effect_updateZonePairEmitter stores after each particle it spawns. */
typedef struct {
    /* 0x00 */ Vec3f velocity;
    /* 0x0C */ f32 timeLeft; // until the particle reaches the target
    /* 0x10 */ Vec3f side;    // horizontal, perpendicular to forward
    /* 0x1C */ Vec3f forward; // unit vector from source to target, flattened onto the XZ plane
} ParticleTravel;

/* A particle spawned by particle_newQuadParticle, with effect_updateZonePairEmitter's state at 0x110. */
struct TravelParticle {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleTravel travel;
};

struct TexturePair {
    s32 ids[2];
};


/* Timed effect of effect_updateZonePairEmitter: particles that travel between two zones. */
struct ZonePairEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ZonePairEmitter link;
};

void *memcpy(void *, const void *, u32);
/* Particle textures: 0xBE, or 0xBF for the slower ones. */
const TexturePair D_800A0D00 = { { 0xBE, 0xBF } };

/* Every 200-400 frames, spawns a particle (texture 0xBE or 0xBF) at the link's source position that
 * travels to its target position at a random speed of 25-55 (0xBF ones at 0.18 of that). */
void effect_updateZonePairEmitter(ZonePairEffect *emitter) {
    ZonePairEmitter *link;
    Vec3f target;
    Vec3f delta;
    f32 length;
    f32 unused;
    f32 base;
    ParticleTravel travel;
    ParticleDef def;
    TexturePair textures;
    TravelParticle *particle;
    f32 speed;

    link = &emitter->link;
    link->timer -= D_800BE9E4;
    if (link->timer < 0) {
        textures = D_800A0D00;
        // matching: textures.ids[i] grows the frame by 8; the pointer-add form does not
        def.texture = *(textures.ids + (func_150ADA20() & 1));
        func_1514470C(link->source, &def.pos);
        func_1514470C(link->target, &target);
        delta.x = target.x - def.pos.x;
        delta.y = target.y - def.pos.y;
        delta.z = target.z - def.pos.z;
        func_15145128(&delta, &travel.velocity, &length, &unused);
        speed = func_150ADA68() * 30.0f + 25.0f;
        base = speed;
        if (def.texture == 0xBF) {
            speed = base * 0.1800000072f;
        }
        travel.velocity.x *= speed;
        travel.velocity.y *= speed;
        travel.velocity.z *= speed;
        travel.timeLeft = length / speed;
        travel.forward.x = delta.x;
        travel.forward.y = 0.0f;
        travel.forward.z = delta.z;
        func_15145128(&travel.forward, &travel.forward, NULL, NULL);
        travel.side.x = -travel.forward.z;
        travel.side.y = 0.0f;
        travel.side.z = travel.forward.x;
        def.rot = D_800A5480;
        def.type = 0;
        def.renderModes = 0x3B03;
        def.lifetime = 300;
        def.frame = 0;
        def.frameSpeed = 0;
        def.rgba[0] = 0xFF;
        def.rgba[1] = 0xFF;
        def.rgba[2] = 0xFF;
        def.rgba[3] = 0xFF;
        func_150ADA68();
        def.height = 706.0f;
        def.width = 706.0f;
        def.rot = D_800A5480;
        def.flags = 0x065C0000;
        def.unk44 = 0xFF;
        def.unk45 = 0xFF;
        def.unk46 = 0;
        def.unk47 = 7;
        def.unk48 = NULL;
        def.viewportMask = 0xFF;
        def.unk50 = 0;
        def.fadeStart = 1;
        def.fadeRate = 0xFF;
        def.scale.x = 1.0f;
        def.scale.y = 1.0f;
        def.scale.z = 1.0f;
        particle = particle_newQuadParticle(&def, &gWhiteVtxColors, 0x28, 0, 0, 0x26, 0, 3, 0xFF, 0x28, emitter->base.header.owner,
                                 emitter->base.header.kind);
        if (particle != NULL) {
            memcpy(&particle->travel, &travel, sizeof(travel));
        }
        link->timer = func_150ADA20() % 201U + 200;
    }
}

/* Moves the particle along its travel velocity; returns 0 once its travel time has run out. */
s32 effect_moveTravelParticle(TravelParticle *particle) {
    particle->head.def.pos.x += particle->travel.velocity.x * gFrameDelta;
    particle->head.def.pos.y += particle->travel.velocity.y * gFrameDelta;
    particle->head.def.pos.z += particle->travel.velocity.z * gFrameDelta;
    particle->travel.timeLeft -= gFrameDelta;
    if (particle->travel.timeLeft < 0.0f) {
        return 0;
    }
    return 1;
}

/* Calls particle_buildQuadFromAxes with the particle's side / forward axes, unless camera_global_Cameras[gScreenLayout].CameraPitch is
 * below 5. */
Vtx *func_150DEB58(TravelParticle *particle, s16 arg1) {
    if (camera_global_Cameras[gScreenLayout].CameraPitch < 5.0f) {
        return NULL;
    }
    return particle_buildQuadFromAxes(&particle->head, &particle->travel.side, &particle->travel.forward, arg1);
}

/* An s32 table entry read through its low byte (the symbols point at byte 3 of each word). */
static const s32 D_800A0D08[8] = { 0x17, 0x18, 0x1A, 0x19, 0x1B, 0x1C, 0x1E, 0x1D };
static const s32 D_800A0D28[8] = { 2, 3, 5, 4, 6, 7, 9, 8 };

/* Runs func_150DEC28 on entries 0-3. */
void func_150DEBE0(s32 arg0) {
    u8 i;

    for (i = 0; i < 4; i++) {
        func_150DEC28(i, 1);
    }
}

/* Passes entry `index` of the D_800A0D08 and D_800A0D28 tables to light_sendEvent and func_151417C4. */
void func_150DEC28(u8 index, u8 arg1) {
    s32 i = index; // matching: without this copy IDO spills the scaled index to another slot

    light_sendEvent(D_800A0D08[i], 0x22, 0);
    func_151417C4(D_800A0D28[i], 0x22);
}
