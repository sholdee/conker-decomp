#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* The actor an emitter is attached to. */
typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ Vec3f pos;
    /* 0x020 */ u8 pad20[0x1B];
    /* 0x03B */ u8 unk3B;
    /* 0x03C */ u8 pad3C[0x198];
    /* 0x1D4 */ s32 unk1D4; // when set, the emit point is D_800887D0 transformed by it
} GameF3270Actor;

struct GameF3270Emitter;

/* Wobble state copied to 0xB0 of the particle spawned by func_150C6460. */
typedef struct {
    /* 0x00 */ struct GameF3270Emitter *owner;
    /* 0x04 */ u8 alive; // set by the owner every frame, cleared by the update
    /* 0x08 */ f32 base;
    /* 0x0C */ f32 range;
    /* 0x10 */ f32 target;
    /* 0x14 */ s16 timer;
    /* 0x16 */ s16 periodMin;
    /* 0x18 */ s16 periodRange;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 rate;
} GameF3270Wobble; // size 0x20

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 flags;
    /* 0x1C */ u8 pad1C[0x8];
    /* 0x24 */ s32 unk24; // eased toward wobble.target
    /* 0x28 */ u8 pad28[0x88];
    /* 0xB0 */ GameF3270Wobble wobble;
} GameF3270Particle;

/* Link state copied to 0x58 of the object spawned by func_150C5F94. */
typedef struct {
    /* 0x00 */ struct GameF3270Emitter *owner;
    /* 0x04 */ u8 alive; // set by the owner every frame, cleared by the update
    /* 0x08 */ f32 surfaceY;
} GameF3270Link; // size 0xC

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ GameF3270Link link;
} GameF3270LinkedObject;

/* The objects an emitter keeps alive; each clears its own slot when it is destroyed. */
// matching: volatile makes IDO reload the slot after the NULL test (func_150C5DC0, func_150C5F40)
typedef struct {
    /* 0x0 */ GameF3270Particle *volatile particle;
    /* 0x4 */ GameF3270LinkedObject *volatile linked;
} GameF3270Children;

typedef struct GameF3270Emitter {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0xB];
    /* 0x18 */ GameF3270Actor *actor;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ f32 surfaceY; // written by the surface probe
    /* 0x38 */ u8 unk38[0x20];
    /* 0x58 */ GameF3270Children children;
} GameF3270Emitter;

GameF3270Particle *func_150C6460(GameF3270Emitter *emitter);
GameF3270LinkedObject *func_150C5F94(GameF3270Actor *actor, GameF3270Emitter *emitter);
s32 func_1513F6C0(void *obj, s32 arg1, s32 arg2);
void *memcpy(void *, const void *, u32);

/* Keeps the emitter's particle alive, spawning it if it doesn't exist yet. */
void func_150C5DC0(GameF3270Emitter *emitter) {
    if (emitter->children.particle != NULL) {
        GameF3270Wobble *wobble = &emitter->children.particle->wobble;

        wobble->alive = TRUE;
    } else {
        emitter->children.particle = func_150C6460(emitter);
    }
}

/* Particle update: eases unk24 toward a target re-rolled at random intervals. Returns whether it stays alive. */
s8 func_150C5E0C(GameF3270Particle *particle) {
    s8 alive;
    GameF3270Wobble *wobble;

    alive = TRUE;
    wobble = &particle->wobble;
    if (!wobble->alive) {
        alive = FALSE;
    }
    wobble->alive = FALSE;
    wobble->timer -= D_800BE9E4;
    if (wobble->timer < 0) {
        wobble->timer = func_150ADA20() % (u32)(wobble->periodRange + 1) + wobble->periodMin;
        wobble->target = func_150ADA68() * wobble->range + wobble->base;
    }
    particle->unk24 += (s32)((wobble->target - (f32)particle->unk24) * wobble->rate);
    return alive;
}

/* Particle destructor: detaches it from its emitter. */
s32 func_150C5EFC(GameF3270Particle *particle) {
    GameF3270Children *children = &particle->wobble.owner->children;

    children->particle = NULL;
    particle->flags |= 2;
    particle->wobble.owner = NULL;
    func_1513F6C0(particle, 0, 0);
    return 0;
}

/* Keeps the emitter's linked object alive, spawning it if it doesn't exist yet. */
void func_150C5F40(GameF3270Emitter *emitter) {
    GameF3270Actor *actor = emitter->actor;

    if (emitter->children.linked != NULL) {
        GameF3270Link *link = &emitter->children.linked->link;

        link->alive = TRUE;
    } else {
        emitter->children.linked = func_150C5F94(actor, emitter);
    }
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[0x3];
    /* 0x0C */ GameF3270Actor *actor;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ u8 unk2D;
} GameF3270SpawnDesc;

s32 func_15045800(Vec3f *from, s32 arg1, f32 minY, f32 *hitY);
GameF3270LinkedObject *func_1513418C(GameF3270SpawnDesc *desc, s32 arg1, u8 arg2, s32 arg3);

/* Spawns the emitter's linked object, recording the surface height above the actor. */
GameF3270LinkedObject *func_150C5F94(GameF3270Actor *actor, GameF3270Emitter *emitter) {
    GameF3270SpawnDesc desc;
    GameF3270Link link;
    GameF3270LinkedObject *obj;

    link.owner = emitter;
    link.alive = TRUE;
    {
        Vec3f from;

        from.x = actor->pos.x;
        from.y = actor->pos.y + 5000.0f;
        from.z = actor->pos.z;
        if (func_15045800(&from, 0, actor->pos.y - 100.0f, &emitter->surfaceY) != 0) {
            link.surfaceY = emitter->surfaceY;
        } else {
            link.surfaceY = actor->pos.y + 5000.0f;
        }
    }
    desc.unk0 = 0;
    desc.unk4 = 0;
    desc.unk8 = actor->unk3B;
    desc.actor = actor;
    desc.unk10 = 0;
    desc.unk14 = 0.0f;
    desc.unk18 = 0.0f;
    desc.unk1C = 0.0f;
    desc.unk20 = 25.0f;
    desc.unk24 = 0.132485f;
    desc.unk28 = 300;
    desc.unk2A = 10;
    desc.unk2B = 4;
    desc.unk2C = 1;
    desc.unk2D = 2;
    obj = func_1513418C(&desc, 12, 0xFF, 0);
    if (obj != NULL) {
        memcpy(&obj->link, &link, sizeof(link));
    }
    return obj;
}

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0xC];
} GameF3270Burst; // size 0x70

/* Copied to 0xA8 of the spawned burst. */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
} GameF3270BurstMotion; // size 0x28

typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ GameF3270BurstMotion motion;
} GameF3270BurstObject;

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ f32 unk60;
} GameF3270BurstSource;

GameF3270BurstObject *func_151303BC(GameF3270Burst *burst, u8 arg1, s32 arg2);

/* Spawns one randomised particle at (x, y, z), moving against (vx, vz) at a fifth of its speed. */
void func_150C60D8(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, GameF3270BurstSource *source) {
    GameF3270Burst burst;
    GameF3270BurstObject *spawned;
    GameF3270BurstMotion motion;

    burst.unk1D = 47;
    burst.unk8 = 0xC01;
    burst.flags = 0x200005;
    burst.unk4 = 0;
    burst.unk1A = 0;
    burst.unk19 = 0;
    burst.unk18 = 0;
    burst.unk17 = 0;
    burst.unk16 = 0;
    burst.unk15 = 0;
    burst.unk14 = 0;
    burst.unk10 = 0;
    burst.unkC = 0;
    burst.unk1C = 0xFF;
    burst.unk22 = 0;
    burst.unk24 = 0.0f;
    burst.unk58 = 0x3207;
    burst.unk60 = 5;
    burst.unk61 = 5;
    burst.unk62 = 4;
    burst.unk63 = -1;
    burst.pos.x = x;
    burst.pos.y = y;
    burst.unk3C = 0.0f;
    burst.unk40 = 0.0f;
    burst.unk44 = 0.0f;
    burst.unk1E = 30;
    burst.unk20 = 8;
    burst.pos.z = z;
    motion.unk24 = source->unk60;
    motion.unk10 = 4.0f;
    motion.unk14 = 0.25f;
    motion.unk4 = 0;
    motion.unk5 = 0;
    motion.unk18 = 0;
    motion.unk19 = 0;
    burst.vel.x = -vx * 0.2f;
    burst.vel.y = 0.0f;
    burst.vel.z = -vz * 0.2f;
    if (func_150ADA20() & 1) {
        burst.unk58 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        burst.unk58 |= 0x80;
    }
    burst.unkA = func_150ADA20() % 51U + 106;
    burst.unk1B = func_150ADA20() % 101U + 100;
    motion.size = burst.width = burst.height = func_150ADA68() * 62.0f + 41.0f;
    motion.unk6 = func_150ADA20() % 5U + 4;
    motion.unk7 = func_150ADA20() % 5U + 4;
    motion.unk8 = (func_150ADA68() * 0.25f + 0.15f) * motion.size;
    motion.unkC = (func_150ADA68() * 0.25f + 0.15f) * motion.size;
    motion.unk1A = func_150ADA20() % 5U + 4;
    motion.unk1B = func_150ADA20() % 5U + 4;
    motion.unk1C = (func_150ADA68() * 0.1f + 0.08f) * motion.size;
    motion.unk20 = (func_150ADA68() * 0.1f + 0.08f) * motion.size;
    burst.unk54 = (func_150ADA68() * 60.0f + 21.0f) * 0.001f;
    spawned = func_151303BC(&burst, 2, 40);
    if (spawned != NULL) {
        memcpy(&spawned->motion, &motion, sizeof(motion));
    }
}

/* Linked-object update: returns whether its owner kept it alive this frame. */
s32 func_150C63EC(GameF3270LinkedObject *obj) {
    s32 alive = TRUE;

    if (!obj->link.alive) {
        alive = FALSE;
    }
    obj->link.alive = FALSE;
    return alive;
}

/* Linked-object destructor: detaches it from its emitter. */
void func_150C6410(GameF3270LinkedObject *obj) {
    GameF3270Children *children = &obj->link.owner->children;

    children->linked = NULL;
    func_151346EC(obj);
}

/* Linked-object destructor: detaches it from its emitter. */
void func_150C6438(GameF3270LinkedObject *obj) {
    GameF3270Children *children = &obj->link.owner->children;

    children->linked = NULL;
    func_1513470C(obj);
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 r;
    /* 0x13 */ u8 g;
    /* 0x14 */ u8 b;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21[7];
} GameF3270ParticleDesc; // size 0x28

extern Vec3f D_800887D0;
extern u8 D_800AB414[][3]; // RGB colours
void func_15143134(Vec3f *in, Vec3f *out, s32 mtx);
u8 func_151D8E20(void);

/* Spawns the emitter's particle on the surface below the actor, if there is one within 200 units. */
GameF3270Particle *func_150C6460(GameF3270Emitter *emitter) {
    GameF3270Particle *particle;
    Vec3f pos;
    GameF3270Actor *actor;

    particle = NULL;
    actor = emitter->actor;
    if (actor->unk1D4) {
        Vec3f origin;

        origin = D_800887D0;
        func_15143134(&origin, &pos, actor->unk1D4);
    } else {
        pos.x = actor->pos.x;
        pos.y = actor->pos.y;
        pos.z = actor->pos.z;
    }
    {
        Vec3f probe;

        probe.x = pos.x;
        probe.y = pos.y + 200.0f;
        probe.z = pos.z;
        if (func_15045800(&probe, 0, pos.y - 200.0f, &emitter->surfaceY) != 0) {
            GameF3270ParticleDesc desc;
            GameF3270Wobble wobble;
            f32 size;
            u8 colorIndex;
            s32 rand2;
            s32 rand1;
            s32 rand0;

            colorIndex = func_151D8E20();
            probe.y = emitter->surfaceY;
            size = func_150ADA68() * 50.0f + 100.0f;
            wobble.owner = emitter;
            wobble.alive = TRUE;
            wobble.base = 26214.4f;
            wobble.range = 32768.0f;
            wobble.target = 0.0f;
            wobble.timer = 0;
            wobble.periodMin = 10;
            wobble.periodRange = 10;
            wobble.rate = 0.231f;
            desc.unkC = (s32)(wobble.range * 0.5f + wobble.base);
            desc.unk6 = 56;
            desc.unk0 = 0x20300;
            desc.unk4 = 300;
            desc.unk10 = 150;
            desc.unk8 = 0;
            desc.unk11 = 0xFF;
            desc.r = D_800AB414[colorIndex][0];
            desc.g = D_800AB414[colorIndex][1];
            desc.b = D_800AB414[colorIndex][2];
            desc.unk15 = 0xFF;
            desc.unk18 = 0x440001;
            desc.unk16 = 0;
            desc.unk17 = 6;
            rand0 = func_150ADA20();
            rand1 = func_150ADA20();
            rand2 = func_150ADA20();
            // matching: the explicit & 0xFF on rand0 (a (u8) cast emits no andi)
            particle = (GameF3270Particle *)func_1513C73C(
                (s32)&desc, 8, 2, (s32)emitter->unk38, probe.x, probe.y, probe.z, size, size, rand0 & 0xFF,
                (rand2 & 1) * 2 + (rand1 & 1), 32, emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(&particle->wobble, &wobble, sizeof(wobble));
            }
        }
    }
    return particle;
}
