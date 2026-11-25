#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Data copied into a func_151491F4 particle at offset 0x28 by func_1515AF90. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 scale;
    /* 0x10 */ f32 zero;
    /* 0x14 */ f32 ground;
} GroundParticle;

/* Result of a floor probe (CollisionHit in game_71820.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 tri[9];
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 material;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 source;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ void *surface;
} GroundHit;

s32 func_1504697C(Vec3f *pos, u16 mask, f32 bottom, GroundHit *hit);

/*
 * With a 1-in-16 chance, spawns a particle (func_151491F4) on the ground 107-2010 units from
 * the player's camera, at an angle within 20 units of its yaw (unk380, converted from degrees).
 */
void func_1515AF90(s16 player) {
    if (!(func_150ADA20() & 0xF)) {
        void *particle;
        GroundParticle effect;
        Vec3f *eye;
        s32 yaw;
        u8 valid;
        s16 angle;
        f32 distance;
        f32 cosine;
        f32 sine;
        GroundHit hit;
        Vec3f probe;
        GroundHit lowerHit;
        Vec3f lowerProbe;
        f32 bottom;

        valid = TRUE;
        eye = (Vec3f *)&D_800DBFF0[player].unk2F8;
        yaw = D_800DBFF0[player].unk380 * 0.7111111283f;
        {
            u32 roll = func_150ADA20();

            angle = (roll % 41U) + yaw - 20;
            distance = func_150ADA68() * 1903.0f + 107.0f;
            cosine = func_151423D8(angle);
            sine = func_151423D8(angle - 0x40);
            effect.position.x = eye->x - distance * sine;
            effect.position.z = eye->z - distance * cosine;
        }
        probe.x = effect.position.x;
        hit.material = 0;
        hit.flags = 0;
        hit.source = 0;
        hit.surface = NULL;
        probe.y = 19500.0f;
        probe.z = effect.position.z;
        hit.height = -10000.0f;
        if (func_1504697C(&probe, 0, eye->y, &hit)) {
            effect.ground = hit.height;
        } else {
            valid = FALSE;
        }
        if (valid) {
            bottom = eye->y + -250.0f;
            lowerHit.surface = NULL;
            lowerHit.source = 0;
            lowerHit.flags = 0;
            lowerHit.material = 0;
            lowerHit.height = -10000.0f;
            lowerProbe.x = effect.position.x;
            lowerProbe.y = eye->y;
            lowerProbe.z = effect.position.z;
            if (func_1504697C(&lowerProbe, 0, bottom, &lowerHit)) {
                effect.position.y = lowerHit.height;
            } else {
                effect.position.y = bottom;
            }
        }
        effect.scale = (func_150ADA68() * 0.0f + 195.0f) * 0.001000000047f;
        effect.zero = 0.0f;
        if (valid) {
            angle = (func_150ADA20() % 61U) + 5;
            particle = func_151491F4(angle, -1, 12, 1, 7, 0x18, 0xFF, 0);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0x28, &effect, sizeof(effect));
            }
        }
    }
}

/* Sprite-particle descriptor passed to func_151303BC (ParticleDesc in game_117490.c). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 texture;
    /* 0x0A */ s16 lifetime;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14[7];
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f position;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3f velocity;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ s8 unk60[4];
    /* 0x64 */ u8 pad64[8];
} ParticleDesc;

/* Extra data copied into a func_151303BC particle at +0xA8 (ParticleExtra in game_117490.c). */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ u8 unk4[4];
    /* 0x08 */ f32 unk8[2];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18[4];
    /* 0x1C */ f32 unk1C[2];
    /* 0x24 */ f32 unk24;
} ParticleExtra;

/* Emitter state stored in an effect object at +0x28. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;        // particles per unit of D_800BE9A4
    /* 0x10 */ f32 accumulator; // pending particles; one is spawned per whole unit
    /* 0x14 */ f32 unk14;
} ParticleEmitter;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ ParticleEmitter emitter;
    /* 0x40 */ u8 pad40[4];
} EmitterObject;

extern f32 D_800BE9A4;
void *func_151303BC(ParticleDesc *, s32, s32);

/* Effect update: accumulates rate * D_800BE9A4 and spawns one func_151303BC particle per whole unit. */
void func_1515B21C(EmitterObject *obj) {
    ParticleEmitter *emitter;
    void *particle;
    ParticleDesc desc;
    ParticleExtra extra;

    emitter = &obj->emitter;
    emitter->accumulator += emitter->rate * D_800BE9A4;
    if (emitter->accumulator > 1.0f) {
        f32 size;

        desc.unk1D = 47;
        desc.texture = 0xC01;
        desc.flags = 0x200005;
        desc.unk4 = 0;
        desc.unk14[6] = 0;
        desc.unk14[5] = 0;
        desc.unk14[4] = 0;
        desc.unk14[3] = 0;
        desc.unk14[2] = 0;
        desc.unk14[1] = 0;
        desc.unk14[0] = 0;
        desc.unk10 = 0;
        desc.unkC = 0;
        desc.unk1C = 255;
        desc.unk22 = 0;
        desc.unk24 = 0.0f;
        desc.unk58 = 0x3207;
        desc.unk60[0] = 5;
        desc.unk60[1] = 5;
        desc.unk60[2] = 4;
        desc.unk60[3] = -1;
        desc.position = emitter->position;
        desc.unk3C.x = 0.0f;
        desc.unk3C.y = 0.0f;
        desc.unk3C.z = 0.0f;
        desc.unk1E = 50;
        desc.unk20 = 5;
        extra.unk4[0] = 0;
        extra.unk4[1] = 0;
        extra.unk18[0] = 0;
        extra.unk18[1] = 0;
        desc.velocity.x = 0.0f;
        desc.velocity.z = 0.0f;
        extra.unk10 = 1.200000048f;
        extra.unk14 = 0.8333333135f;
        extra.unk24 = emitter->unk14;

        if (func_150ADA20() & 1) {
            desc.unk58 |= 0x40;
        }
        if (func_150ADA20() & 1) {
            desc.unk58 |= 0x80;
        }

        do {
            desc.velocity.y = func_150ADA68() * 12.0f * 0.1000000015f;
            desc.lifetime = (func_150ADA20() % 152U) + 150;
            desc.alpha = (func_150ADA20() % 151U) + 55;
            size = (func_150ADA68() * 200.0f) + 100.0f;
            desc.width = size;
            extra.size = size;
            desc.height = size;
            extra.unk4[2] = (func_150ADA20() % 5U) + 4;
            extra.unk4[3] = (func_150ADA20() % 5U) + 4;
            extra.unk8[0] = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.size;
            extra.unk8[1] = ((func_150ADA68() * 0.25f) + 0.150000006f) * extra.size;
            extra.unk18[2] = (func_150ADA20() & 3) + 2;
            extra.unk18[3] = (func_150ADA20() & 3) + 2;
            extra.unk1C[0] = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.size;
            extra.unk1C[1] = ((func_150ADA68() * 0.0700000003f) + 0.05999999866f) * extra.size;
            desc.unk54 = ((func_150ADA68() * 1005.0f) + 600.0f) * 0.001000000047f;

            particle = func_151303BC(&desc, 2, 0x28);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            emitter->accumulator -= 1.0f;
        } while (emitter->accumulator > 1.0f);
    }
}

extern void func_1515572C(s16 *, s32);

void func_1515B5F4(s16 arg0) {
    s16 sp1C[2];

    sp1C[0] = arg0;
    func_1515572C(sp1C, 0xB);
}

void func_1515B62C(struct102 *arg0, s16 *arg1, u8 arg2) {
    if (arg2 == 0xB) {
        if (*(s16 *)((u8 *)arg0 + 0x70) == arg1[0]) {
            func_1516972C(arg0);
        }
    }
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[0x17];
} struct_1515B674;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  pad14[8];
} struct_1515B674b;

extern u8 (*D_8008FD04[])(void);
extern s32 func_151D8E20(void);
extern void *func_1515548C(struct_1515B674 *, s32, s32, s32, s32, u8, s32);

void func_1515B674(s16 arg0) {
    struct_1515B674 sp9C;
    struct_1515B674b sp80;
    void *temp_v0;
    s16 i;

    i = (func_150ADA20() % 1U) + 10;
    sp9C.unk14 = 0x19;
    sp9C.unk1A = 1;
    sp9C.unk16 = 0x14;
    sp9C.unk18 = 0xC;
    sp9C.unk1B = 0xFF;
    sp9C.unk1C = 0xFF;
    sp9C.unk1D = 0xFF;
    sp9C.unk1F = 0xFF;
    sp80.unk0 = arg0;
    sp9C.unk44 = arg0;
    sp9C.unk20 = 0xFF;
    sp9C.unk21 = 0xFF;
    sp9C.unk22 = 0xFF;
    sp9C.unk23 = 0xFF;
    sp9C.unk24 = 0;
    sp9C.unk28 = 0;
    sp9C.unk2C = 0;
    sp9C.unk40 = 0;
    sp9C.unk41 = 9;
    sp9C.unk30 = 7;
    sp9C.unk34 = 0x3C;
    sp9C.unk38 = 0x80;
    sp9C.unk3C = 0x20;
    while (i--) {
        sp80.unk4 = ((func_150ADA68() * 105.0f) + 105.0f) * 0.001000000047f;
        sp80.unk8 = func_150ADA68() * 3008.0f * 0.001000000047f;
        sp9C.unkC = sp9C.unk8 = sp80.unkC = ((func_150ADA68() * 1579.0f) + 489.0f) * 0.009999999776f;
        sp80.unk10 = ((func_150ADA68() * 301.0f) + 94.0f) * 0.009999999776f;
        sp9C.unk0 = (func_150ADA68() * 200.0f) + -100.0f;
        sp9C.unk4 = (func_150ADA68() * 148.0f) + -100.0f;
        sp9C.unk10 = D_8008FD04[func_151D8E20()]();
        sp9C.unk12 = (func_150ADA20() % 51U) + 0x32;
        sp9C.unk1E = (func_150ADA20() % 101U) + 0x64;
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 2;
        }
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 4;
        }
        temp_v0 = func_1515548C(&sp9C, 2, 0, 0, 0x18, 0xFF, 1);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x70, &sp80, 0x18);
        }
    }
}

typedef struct Obj1515B994 {
    char pad0[0x14];
    /* 0x14 */ f32 unk14;
    char pad18[4];
    /* 0x1C */ f32 unk1C;
    char pad20[0x74 - 0x20];
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
} Obj1515B994;

s32 func_1515B994(void *arg0) {
    typedef struct {
        u8 pad0[0x14];
        f32 x;
        u8 pad18[4];
        f32 z;
        u8 pad20[0x54];
        f32 acceleration;
        f32 velocity;
        f32 base;
        f32 scale;
    } Motion;
    Motion *motion = arg0;
    f32 previous;

    previous = motion->velocity;
    motion->x += previous * D_800BE9A4 + 0.5f * motion->acceleration * D_800BE9A4;
    motion->velocity = motion->velocity + motion->acceleration * D_800BE9A4;
    motion->z = motion->base + motion->scale * (motion->velocity + previous) * 0.5f;
    return 1;
}

void func_1515BA10(s32 arg0) {

}

extern void func_1515AF90(s16);

void func_1515BA1C(s16 arg0) {
    func_1515AF90(arg0);
}

void func_1515BA48(s32 arg0) {
}

extern void func_1515B674(s16);

void func_1515BA54(s16 arg0) {
    func_1515B674(arg0);
}

extern void func_1515B5F4(s16);

void func_1515BA80(s16 arg0) {
    func_1515B5F4(arg0);
}

void func_1515BAAC(s16 arg0) {
    func_1515B5F4(arg0);
}
