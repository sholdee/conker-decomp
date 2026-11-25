#include <ultra64.h>
/* functions.h has placeholder prototypes for these (void returns, no args); hide them while it is
 * included and declare the real ones below. */
#define func_150DE32C func_150DE32C_hdr
#include "functions.h"
#undef func_150DE32C
/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the table. */
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#include "effects.h"
#undef D_800D3098
extern struct178 *D_800D3098;

extern s32 func_150A32B4(struct178 *, s32, s32, s32);
extern WorldObject *func_151149AC(u8);
extern void func_150A2864(s32, s32);
extern s32 D_80090204[][2];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(WorldObject *, s32, s32, s32, s32);

void func_150DE320(s32 arg0) {
}

/* For each of eight D_800D3098 records that func_150A32B4 accepts, sets the moving state of the
 * matching func_151149AC object, or (for records without one) calls func_150A2864(id, 1). */
void func_150DE32C(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    WorldObject *prop;
    const u8 ids[8] = { 0x18, 0x19, 0x1A, 0x1B, 0x0D, 0x30, 0x31, 0x32 };
    const u8 propIds[8] = { 0xF6, 0xF3, 0xF0, 0xED, 0, 0, 0, 0 };

    for (i = 0; i < 8; i++) {
        if (func_150A32B4(&D_800D3098[ids[i]], arg0, arg1, arg2) != 0) {
            if (propIds[i] != 0) {
                prop = func_151149AC(propIds[i]);
                prop->behaviorState = (prop->behaviorState & ~3) | 3;
            } else {
                func_150A2864(ids[i], 1);
            }
        }
    }
}

/* Per-frame update: while moving, spins the rotation (wrapping each angle), applies gravity and
 * velocity (moveX..moveZ, gravity in param) to the position and counts alpha down as a lifetime
 * until inactive; kind 0xC props also get two assets of D_80090204 row work.words[0] passed to func_1510D874. */
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

    if ((prop->behaviorState & 3) == 3) {
        frames = D_800BE9E4;

        prop->rotX += prop->spinX * frames;
        angle = prop->rotX;
        if (angle < 0.0f) {
            prop->rotX = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotX = angle - 360.0f;
        }

        prop->rotY += prop->spinY * frames;
        angle = prop->rotY;
        if (angle < 0.0f) {
            prop->rotY = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotY = angle - 360.0f;
        }

        prop->rotZ += prop->spinZ * frames;
        angle = prop->rotZ;
        if (angle < 0.0f) {
            prop->rotZ = angle + 360.0f;
        } else if (angle >= 360.0f) {
            prop->rotZ = angle - 360.0f;
        }

        timer = prop->alpha;
        prop->moveY -= prop->param.word * D_800BE9E4;
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
        asset = D_80090204[prop->work.words[0]];
        slot = 4;
        nextSlot = 5;
        do {
            addr = func_1510D0EC(*asset, &size, 3, 0);
            func_1510D874(prop, addr, size + addr - 0x200, slot, nextSlot);
            nextSlot += 2;
            asset++;
            slot += 2;
        } while (nextSlot != 9);
    }
}

/* Sways the object: y bobs around 1130 and rotX / rotZ rock, each driven by func_15048A40 of its own
 * phase (work.words[0..2], in 1/8 steps of its angle); the phases advance 12, 16 and 24 per frame. */
void func_150DE6D8(WorldObject *prop) {
    prop->y = func_15048A40(prop->work.words[0] >> 3) * 10.19999981f + 1130.0f;
    prop->rotX = func_15048A40(prop->work.words[1] >> 3) * 1.100000024f;
    prop->rotZ = func_15048A40(prop->work.words[2] >> 3) * 1.100000024f;
    prop->work.words[0] += D_800BE9E4 * 12;
    prop->work.words[1] += D_800BE9E4 * 16;
    prop->work.words[2] += D_800BE9E4 * 24;
}

/* State func_150DE7C0 stores after each particle it spawns. */
typedef struct {
    /* 0x00 */ Vec3f velocity;
    /* 0x0C */ f32 timeLeft; // until the particle reaches the target
    /* 0x10 */ Vec3f side;    // horizontal, perpendicular to forward
    /* 0x1C */ Vec3f forward; // unit vector from source to target, flattened onto the XZ plane
} ParticleTravel;

/* A particle spawned by func_1513D2F0, with func_150DE7C0's state at 0x110. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0xA0];
    /* 0x110 */ ParticleTravel travel;
} Particle;

typedef struct {
    s32 ids[2];
} TexturePair;

/* Emitter state at +0x28 of func_150DE7C0's object. */
typedef struct {
    /* 0x00 */ s16 timer; // frames until the next particle
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ s32 source;
    /* 0x08 */ s32 target;
} EmitterLink;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterLink link;
} Emitter;

void *memcpy(void *, const void *, u32);
void func_1514470C(s32, Vec3f *);
s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
s32 func_15140410(Particle *, Vec3f *, Vec3f *, s16);
extern TexturePair D_800A0D00;

/* Every 200-400 frames, spawns a particle (texture 0xBE or 0xBF) at the link's source position that
 * travels to its target position at a random speed of 25-55 (0xBF ones at 0.18 of that). */
void func_150DE7C0(Emitter *emitter) {
    EmitterLink *link;
    Vec3f target;
    Vec3f delta;
    f32 length;
    f32 unused;
    f32 base;
    ParticleTravel travel;
    ParticleDef def;
    TexturePair textures;
    Particle *particle;
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
        particle = func_1513D2F0(&def, &D_800A4AA0, 0x28, 0, 0, 0x26, 0, 3, 0xFF, 0x28, emitter->unkC,
                                 emitter->unk1);
        if (particle != NULL) {
            memcpy(&particle->travel, &travel, sizeof(travel));
        }
        link->timer = func_150ADA20() % 201U + 200;
    }
}

/* Moves the particle along its travel velocity; returns 0 once its travel time has run out. */
s32 func_150DEACC(Particle *particle) {
    particle->def.pos.x += particle->travel.velocity.x * D_800BE9A4;
    particle->def.pos.y += particle->travel.velocity.y * D_800BE9A4;
    particle->def.pos.z += particle->travel.velocity.z * D_800BE9A4;
    particle->travel.timeLeft -= D_800BE9A4;
    if (particle->travel.timeLeft < 0.0f) {
        return 0;
    }
    return 1;
}

/* Calls func_15140410 with the particle's side / forward axes, unless D_800DBFF0[D_80082FA0].pitch is
 * below 5. */
s32 func_150DEB58(Particle *particle, s16 arg1) {
    if (D_800DBFF0[D_80082FA0].pitch < 5.0f) {
        return 0;
    }
    return func_15140410(particle, &particle->travel.side, &particle->travel.forward, arg1);
}

/* An s32 table entry read through its low byte (the symbols point at byte 3 of each word). */
typedef struct {
    u8 value;
    u8 pad1[3];
} LowByte;

extern LowByte D_800A0D0B[];
extern LowByte D_800A0D2B[];
extern void func_151616D0(u8, u8, s32);
extern void func_151417C4(u8, s32);
void func_150DEC28(u8 index, s32 arg1);

/* Runs func_150DEC28 on entries 0-3. */
void func_150DEBE0(s32 arg0) {
    u8 i;

    for (i = 0; i < 4; i++) {
        func_150DEC28(i, 1);
    }
}

/* Passes entry `index` of the D_800A0D0B and D_800A0D2B byte tables to func_151616D0 and func_151417C4. */
void func_150DEC28(u8 index, s32 arg1) {
    s32 i = index; // matching: without this copy IDO spills the scaled index to another slot

    func_151616D0(D_800A0D0B[i].value, 0x22, 0);
    func_151417C4(D_800A0D2B[i].value, 0x22);
}
