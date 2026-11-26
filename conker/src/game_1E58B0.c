#include <ultra64.h>
// matching: func_151B8908's spill slot needs func_150ADA20 declared returning s32, not u8
#include "functions.h"
#include "variables.h"

/* The four vertex colours of the bullet particle. */
static const VtxColor D_800AA490[4] = {
    { 255, 255, 255, 255, 0 },
    { 255, 255, 255, 255, 0 },
    { 255, 255, 255, 0, 0 },
    { 255, 255, 255, 0, 0 },
};

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
    spawn.def.pos = spawn.def.rot = bullet->head.pos;
    spawn.def.scale.x = bullet->head.vel.x * 0.25f;
    spawn.def.scale.y = bullet->head.vel.y * 0.25f;
    spawn.def.scale.z = bullet->head.vel.z * 0.25f;
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

    particle = func_1513D2F0(&spawn.def, D_800AA490, 0x1B, 0, 0, 0x19, 0, 0, 0, 4, bullet->head.header.owner,
                             bullet->head.header.kind);
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
        particle->head.def.pos = bullet->head.pos;
        particle->head.def.rot.x += particle->head.def.scale.x * gFrameDelta;
        particle->head.def.rot.y += particle->head.def.scale.y * gFrameDelta;
        particle->head.def.rot.z += particle->head.def.scale.z * gFrameDelta;
    } else {
        particle->head.def.flags |= 1;
    }
    return 1;
}

/* Draws the particle with the default particle drawer. */
void func_151B863C(BulletParticle *particle, s16 viewport) {
    func_1513FA70(&particle->head, viewport);
}

/* Starts a BloodTrail effect (emitter 3) that tracks the bullet's position. */
BloodTrail *func_151B8668(Bullet *bullet, u8 fxOwner, s32 fxList) {
    s32 pad; // matching: stack space
    BloodTrailDesc desc;

    desc.radius = 10.0f;
    desc.x = &bullet->head.pos.x;
    desc.y = &bullet->head.pos.y;
    desc.z = &bullet->head.pos.z;
    desc.rate = 0.2074999958f * D_800DCA24;
    desc.lifetime = 300;
    desc.flags = 0;
    desc.emitter = 3;
    desc.unk18 = 0;
    return func_15134908(&desc, 0, fxOwner, fxList);
}

/* Emitter callback (D_80089B08): spawns one particle at (x, y, z), flung in a random direction and
 * pushed against the emitter's movement (dx, dy, dz). */
void func_151B86F4(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    Vec3f position;
    Vec3f velocity;
    Vec3f acceleration;
    f32 push;
    f32 random_c;
    u32 random_b;
    u32 random_a;
    f32 random_f;

    position.x = x;
    position.y = y;
    position.z = z;
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    random_f = func_150ADA68();
    func_15143794(random_a & 0xFF, random_b % 65U - 32, random_f * 945.0f * 0.00999999977648f, &velocity);
    push = (func_150ADA68() * 157.0f + 604.0f) * 0.00100000005f;
    acceleration.x = 0.0f;
    acceleration.y = 0.0f;
    acceleration.z = 0.0f;
    velocity.x += -dx * gFrameDeltaInv * push;
    velocity.y += -dy * gFrameDeltaInv * push;
    velocity.z += -dz * gFrameDeltaInv * push;
    random_f = func_150ADA68();
    random_c = func_150ADA68();
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    func_151A26EC(&position, &acceleration, &velocity, 1.0f, (random_f * 157.0f + -151.0f) * 0.00100000005f,
                  random_c * 55.0f + 75.0f, random_a % 26U + 25, random_b % 101U + 100, 10, 25, 0, -1, 0, 0, 0,
                  effect->owner, effect->kind);
}

static const TextureChoices D_800AA4B8 = { { 0x60, 0x61, 0x62, 0x63 } };

/* Spawns a particle at the bullet's position, with a random texture choice and random flag bits 0x40 / 0x80. */
SpriteParticle *func_151B8908(Bullet *bullet) {
    s32 pad; // matching: unused frame slot above spawn
    ParticleDesc spawn;
    TextureChoices choices;

    choices = D_800AA4B8;
    spawn.texture = choices.texture[func_150ADA20() & 3];
    spawn.renderMode = 0x1303;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.timer = 300;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r = 0xFF;
    spawn.g = 0xFF;
    spawn.b = 0xFF;
    spawn.a = 0xFF;
    spawn.r2 = 0xFF;
    spawn.g2 = 0xFF;
    spawn.b2 = 0xFF;
    spawn.alpha = 0xFF;
    spawn.brightness = 0xFF;
    spawn.width = spawn.height = func_150ADA68() * 500.0f + 900.0f;
    spawn.pos = bullet->head.pos;
    spawn.offset = D_800A5480;
    spawn.vel = D_800A5480;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.scaleStart = 1;
    spawn.gravity = 0.0f;
    spawn.scaleRate = 1.0f;
    spawn.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000 |
                  0x40000 | 0x800000;
    spawn.primColorMode = 6;
    spawn.envColorMode = 5;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    return func_15130280(&spawn, 1, 0, 0, bullet->head.header.owner, bullet->head.header.kind);
}
