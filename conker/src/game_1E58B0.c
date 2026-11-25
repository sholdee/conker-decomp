#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_proto
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


extern u8 D_800AA490;
extern f32 D_800DCA24;

/* The leading fields of a bullet object (see func_151C0698 in game_1ED0F0.c). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 fxList;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 fxOwner;
    /* 0x0D */ u8 padD[0x2B];
    /* 0x38 */ vertex pos;
    /* 0x44 */ vertex vel;
} Bullet;

/* Spawn template of a particle (same layout as ParticleDef in game_10B7D0.c). */
typedef struct {
    /* 0x00 */ u8 texture;
    /* 0x01 */ u8 type;
    /* 0x02 */ u16 renderModes;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ vertex pos;
    /* 0x28 */ vertex unk28;
    /* 0x34 */ vertex unk34; /* added to unk28 every frame by func_151B85AC */
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 fadeStart;
    /* 0x56 */ s16 fadeRate;
} ParticleDef; /* size 0x58 */

/* A particle spawned by func_151B8400, which keeps its bullet at 0x110. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0xA0];
    /* 0x110 */ Bullet *bullet;
} BulletParticle;

/* Spawns the particle that follows bullet (updated by func_151B85AC). */
BulletParticle *func_151B8400(Bullet *bullet) {
    BulletParticle *particle;
    struct {
        ParticleDef def;
        Bullet *owner; /* copied to the new particle's bullet field */
    } spawn;

    spawn.owner = bullet;
    spawn.def.texture = 0xFF;
    spawn.def.type = 0;
    spawn.def.renderModes = 0x5901;
    spawn.def.lifetime = 50;
    spawn.def.frame = 0;
    spawn.def.frameSpeed = 0;
    spawn.def.rgba[0] = 0xFF;
    spawn.def.rgba[1] = 0xE6;
    spawn.def.rgba[2] = 0xB6;
    spawn.def.rgba[3] = 0xFF;
    spawn.def.width = 9.0f;
    spawn.def.height = 1.0f;
    spawn.def.pos = spawn.def.unk28 = bullet->pos;
    spawn.def.unk34.x = bullet->vel.x * 0.25f;
    spawn.def.unk34.y = bullet->vel.y * 0.25f;
    spawn.def.unk34.z = bullet->vel.z * 0.25f;
    spawn.def.flags = 0x0CCC0000;
    spawn.def.unk44 = 0xC8;
    spawn.def.unk45 = 0xFF;
    spawn.def.unk46 = 0;
    spawn.def.unk47 = 6;
    spawn.def.unk48 = NULL;
    spawn.def.viewportMask = 0xFF;
    spawn.def.unk50 = 0;
    spawn.def.fadeStart = 50;
    spawn.def.fadeRate = 5;

    particle = func_1513D2F0(&spawn.def, (s32)&D_800AA490, 0x1B, 0, 0, 0x19, 0, 0, 0, 4, bullet->fxOwner,
                             bullet->fxList);
    if (particle != NULL) {
        memcpy(&particle->bullet, &spawn.owner, sizeof(spawn.owner));
    }
    return particle;
}

/* Keeps the particle on its bullet and advances unk28 by unk34; flags the particle (bit 0) once the
 * bullet is gone. */
s32 func_151B85AC(BulletParticle *particle) {
    Bullet *bullet = particle->bullet;

    if (bullet != NULL) {
        particle->def.pos = bullet->pos;
        particle->def.unk28.x += particle->def.unk34.x * D_800BE9A4;
        particle->def.unk28.y += particle->def.unk34.y * D_800BE9A4;
        particle->def.unk28.z += particle->def.unk34.z * D_800BE9A4;
    } else {
        particle->def.flags |= 1;
    }
    return 1;
}

void func_1513FA70(BulletParticle *particle, s16 viewport);

/* Draws the particle with the default particle drawer. */
void func_151B863C(BulletParticle *particle, s16 viewport) {
    func_1513FA70(particle, viewport);
}

/* Spawn parameters of a BloodTrail effect (see game_161520.c). */
typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 callback;
    /* 0x1A */ u8 pad1A[6]; // matching: frame slot is 0x20 bytes; the callee copies 0x1C
} BloodTrailDesc;

extern void func_15134908(BloodTrailDesc *, s32, u8, s32);

/* Starts a BloodTrail effect (emitter 3) that tracks the bullet's position. */
void func_151B8668(Bullet *bullet, u8 fxOwner, s32 fxList) {
    BloodTrailDesc desc;

    desc.radius = 10.0f;
    desc.x = &bullet->pos.x;
    desc.y = &bullet->pos.y;
    desc.z = &bullet->pos.z;
    desc.rate = 0.2074999958f * D_800DCA24;
    desc.lifetime = 300;
    desc.flags = 0;
    desc.emitter = 3;
    desc.unk18 = 0;
    func_15134908(&desc, 0, fxOwner, fxList);
}

void func_15143794(s16, s16, f32, f32 *);
void func_151A26EC(f32 *, f32 *, f32 *, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150ADA20(void);

void func_151B86F4(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, u8 *a6) {
    f32 sp84[3];
    f32 sp78[3];
    f32 sp6C[3];
    f32 r;

    sp84[0] = a0;
    sp84[1] = a1;
    sp84[2] = a2;
    func_15143794((s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 65U) - 32),
                  func_150ADA68() * 945.0f * 0.00999999977648f, sp78);
    r = (func_150ADA68() * 157.0f + 604.0f) * 0.00100000005f;
    sp6C[0] = 0.0f;
    sp6C[1] = 0.0f;
    sp6C[2] = 0.0f;
    sp78[0] += -a3 * D_800BE9A8 * r;
    sp78[1] += -a4 * D_800BE9A8 * r;
    sp78[2] += -a5 * D_800BE9A8 * r;
    func_151A26EC(sp84, sp6C, sp78, 1.0f,
                  (func_150ADA68() * 157.0f + -151.0f) * 0.00100000005f,
                  func_150ADA68() * 55.0f + 75.0f, (func_150ADA20() % 26U) + 25,
                  (func_150ADA20() % 101U) + 100, 10, 25, 0, -1, 0, 0, 0, a6[0xC], a6[1]);
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Struct151B8908Particle; /* 0x70 */

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x2B];
    struct17 unk38;
} Struct151B8908;

typedef struct {
    s32 v[4];
} Struct151B8908Tbl;

extern Struct151B8908Tbl D_800AA4B8;
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151B8908(Struct151B8908 *arg0) {
    s32 pad;
    Struct151B8908Particle spawn;
    Struct151B8908Tbl tbl;

    tbl = D_800AA4B8;

    spawn.unk1D = tbl.v[func_150ADA20() & 3];
    spawn.unk8 = 0x1303;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkA = 300;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk14 = 0xFF;
    spawn.unk15 = 0xFF;
    spawn.unk16 = 0xFF;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0xFF;
    spawn.unk19 = 0xFF;
    spawn.unk1A = 0xFF;
    spawn.unk1B = 0xFF;
    spawn.unk1C = 0xFF;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 500.0f + 900.0f;
    spawn.pos = arg0->unk38;
    spawn.unk3C = *(struct17 *)&D_800A5480;
    spawn.vel = *(struct17 *)&D_800A5480;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    spawn.unk58 = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 | 0x40000 | 0x800000;
    spawn.unk60 = 6;
    spawn.unk61 = 5;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    func_15130280(&spawn, 1, 0, 0, arg0->unkC, arg0->unk1);
}
