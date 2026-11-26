#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// particle_spawnSteamParticle (game/Particle/Steam/steam.c) is declared in each file that calls it: game/Beam/ModelBeam/modelbeam.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Attaches emitter 6 to bone 1 of `actor`; a nonzero `lifetime` ends it after that many frames. */
void *func_151AE7B0(Actor *actor, f32 rate, s16 lifetime, u8 arg3, s32 arg4) {
    void *effect; // matching: the named result keeps IDO's 80-byte frame (direct return: 72)
    BloodDripDesc desc;

    if (actor == NULL) {
        return NULL;
    }

    effect_sendActorEventToType(0x28, actor, 0x11);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 15.40000057f;
    desc.rate = rate;
    desc.flags = BLOOD_DRIP_ON_BONE;
    if (lifetime != 0) {
        desc.lifetime = lifetime;
        desc.flags = BLOOD_DRIP_ON_BONE | BLOOD_DRIP_LIFETIME;
    } else {
        desc.lifetime = 300;
    }
    desc.emitter = 6;
    desc.callback = -1; /* none */
    desc.unk2D = 5;
    effect = blood_spawnDrip(&desc, 0, arg3, arg4);
    return effect;
}

/* Randomly spawns a particle at (x, y, z): 15% nothing, 10% a random-direction one (func_151AEAB4),
 * 75% one moving against (dirX, dirY, dirZ) (func_151AE984). arg6[0xC] is passed through. */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6) {
    Vec3f pos;
    f32 roll;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    roll = math_randomFloat();
    if (roll < 0.15000000596f) {
        return;
    }
    if (roll < 0.25f) {
        u8 flag;

        roll = math_randomFloat();
        flag = (roll > 0.0f) ? 1 : 0; // matching: a plain `roll > 0.0f` compiles differently
        func_151AEAB4(&pos, flag, arg6[0xC]);
    } else {
        s32 flag;

        roll = math_randomFloat();
        flag = (roll > 0.8050000668f) ? 1 : 0;
        func_151AE984(&pos, dirX, dirY, dirZ, flag, arg6[0xC]);
    }
}

/* Spawns a particle at pos moving against (dirX, dirY, dirZ), with randomized parameters. */
void func_151AE984(Vec3f *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5) {
    f32 scale;
    Vec3f velocity;
    f32 shade;
    u32 rand2;
    u32 rand1;
    f32 rand0;

    scale = 0.1030000076f;
    velocity.x = -dirX * scale;
    velocity.y = -dirY * scale;
    velocity.z = -dirZ * scale;
    rand0 = math_randomFloat();
    rand1 = math_random();
    rand2 = math_random();
    shade = math_randomFloat() * 219.0f + 77.0f;
    // matching: the rodata holds -1.045f, so this is an add of a negative literal, not a sub
    particle_spawnSteamParticle(pos, &velocity, 0, rand0 * 0.6020000577f + -1.045000076f, rand1 % 31 + 60, rand2 % 101 + 155,
                  shade, (u8)flag, 1.0f, 1.0f, 0, 0, 1, 0, arg5, 1);
}

/* Spawns a particle at pos moving in a random direction at a random speed, with randomized parameters.
 * math_cosBinAngleMasked is cos() over a 256-step circle, so cos(a - 0x40) is sin(a). */
void func_151AEAB4(Vec3f *pos, u8 flag, u8 arg2) {
    u32 phase; // matching: passing yaw - 0x40 through a u32 temp is needed for identical bytes
    Vec3f velocity;
    u8 yaw;
    u8 pitch;
    f32 horizontal;
    f32 yawCos;
    f32 yawSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;

    yaw = math_random() & 0xFF; // matching: the redundant mask is needed for identical bytes
    pitch = (math_random() & 0x7F) - 0x3F;
    yawCos = math_cosBinAngleMasked(yaw);
    phase = yaw - 0x40;
    yawSin = math_cosBinAngleMasked(phase);
    pitchCos = math_cosBinAngleMasked(pitch);
    pitchSin = math_cosBinAngleMasked(pitch - 0x40);
    speed = (math_randomFloat() * 80.0f + 20.0f) * 0.1000000015f;
    horizontal = speed * pitchCos;
    velocity.x = horizontal * yawSin;
    velocity.y = -speed * pitchSin;
    velocity.z = horizontal * yawCos;
    {
        f32 rand3;
        u32 rand2;
        u32 rand1;
        f32 rand0;

        rand0 = math_randomFloat();
        rand1 = math_random();
        rand2 = math_random();
        rand3 = math_randomFloat();
        particle_spawnSplash(pos, &velocity, rand0 * 0.4060000181f + 0.2100000083f, rand1 % 41 + 60, rand2 % 101 + 155,
                      rand3 * 4.0f + 3.100000143f, math_random() % 5 + 3, flag, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0,
                      arg2, 1);
    }
}

/* Spawns a ground burst effect around obj: one particle_spawnSplashSprayAtAngles burst, 7-12 scattered
 * particle_spawnKindEmitter / particle_spawnSquash particles on the ground, and a blood_spawnGrowingParticle effect under obj. */
void blood_spawnGroundBurst(Actor *obj, u8 arg1, s32 arg2) {
    CollisionHit ground;
    SplashSpraySpec burst;
    s32 count;
    Vec3f probe;
    f32 pos[3];    // matching: pos and center stay f32[3] (a Vec3f changes IDO's schedule); passed as Vec3f below
    f32 center[3];
    Vec3f centerProbe;
    f32 unused[15]; // matching: unused, pads the frame to the original 0x160 bytes
    u32 randA;
    u32 randB;
    f32 randF1;
    f32 randF2;
    volatile u32 angle; // matching: volatile forces the spill to sp+0x6C between the two calls

    if (obj == NULL) {
        return;
    }
    collision_initHitFromObject(&ground, obj);
// matching: the unused labels lbD/lbA/lbC/lbB/lbl split basic blocks; without them uopt
// register-allocates far more (callee-saved $f22-$f30, s5/s6) and the frame grows
lbD:
    burst.spray.pos.x = obj->xpos;
    burst.spray.pos.y = obj->ypos;
    burst.spray.pos.z = obj->zpos;
    burst.spray.scaleBase = 5.079999924f;
    burst.spray.countBase = 10;
    burst.spray.countRange = 0;
    burst.angles.yawRange = 255;
    burst.angles.pitchBase = -63;
    burst.angles.yawBase = 0;
    burst.angles.pitchRange = 45;
    burst.spray.itemCountBase = 3;
    burst.spray.itemCountRange = 2;
    burst.spray.lifeBase = 40;
    burst.spray.lifeRange = 20;
    burst.spray.unk30 = 155;
    burst.spray.unk32 = 100;
    burst.spray.unk3C = 16;
    burst.spray.unk3E = 15;
    burst.spray.unk40 = 0;
    burst.spray.kind = 0;
    burst.spray.scaleRange = 3.019999981f;
    burst.spray.sizeBase = 0.807000041f;
    burst.spray.sizeRange = 0.7010000348f;
    burst.spray.speedBase = 18.0f;
    burst.spray.speedRange = 11.80000019f;
    burst.spray.unk38 = 0.5f;
    particle_spawnSplashSprayAtAngles(&burst.angles, &burst.spray, &ground, 255, 1);
lbA:
    count = (math_random() % 6) + 7;
    probe.y = obj->ypos + 100.0f;
    while (count != 0) {
        angle = math_random();
        math_dirFromBinAngleXZ((u8)angle, (math_randomFloat() * 59.0f) + 90.0f, &pos[0], &pos[2]);
lbC:
        pos[0] += obj->xpos;
        pos[2] += obj->zpos;
        probe.x = pos[0];
        probe.z = pos[2];
        if (collision_probeFloorSurfABSet0Cached(&probe, 0, obj->ypos - 500.0f, &ground) == 0 || ground.source == 3) {
            return;
        }
        pos[1] = ground.height;
        pos[1] += 10.0f;
        if (math_random() & 1) {
            particle_spawnKindEmitter(0, (math_randomFloat() * 4.5f) + 15.0f, (math_randomFloat() * 100.0f) + 155.0f,
                          &ground.tri, (Vec3f *)pos, 100, 0, 1, 0, arg1, arg2);
        } else {
            particle_spawnSquash(0, (math_randomFloat() * 5.900000095f) + 6.099999905f, (math_randomFloat() * 100) + 155, // matching: int 100/155 here, float literals change allocation
                          (Vec3f *)pos, 1, arg1, arg2);
        }
        count--;
    }
lbB:
    center[0] = obj->xpos;
    center[1] = obj->ypos;
    center[2] = obj->zpos;
    centerProbe.x = center[0];
    centerProbe.y = center[1] + 100.0f;
    centerProbe.z = center[2];
    if (collision_probeFloorSurfABSet0Cached(&centerProbe, 0, center[1] - 500.0f, &ground) == 0 || ground.source == 3) {
        return;
    }
    center[1] = ground.height;
    center[1] += 10.0f;
    randF1 = math_randomFloat();
    randF2 = math_randomFloat();
    randA = math_random();
    randB = math_random();
    blood_spawnGrowingParticle(((randF1 * 30.0f) + 90.0f) * 0.001000000047f,
                  ((randF2 * 103.0f) + 103.0f) * 9.999999747e-06f,
                  (randA % 101) + 155, (randB % 101) + 100,
                  (math_random() % 56) + 70, &ground.tri, (Vec3f *)center, 0, 1, arg1, arg2);
lbl:;
}

/* Attaches emitter 10 (func_151AF338) to bone 1 of `actor` for 320 frames. */
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2) {
    void *result;
    BloodDripDesc desc;

    if (actor == NULL) {
        return NULL;
    }
    effect_sendActorEventToType(40, actor, 22);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 9.800000191f;
    desc.rate = 0.1430000067f;
    desc.flags = 6; /* ON_BONE | LIFETIME (see game/Effects/Blood/blood.c) */
    desc.lifetime = 320;
    desc.emitter = 10;
    desc.callback = -1;
    desc.unk2D = 9;
    result = blood_spawnDrip(&desc, 0, arg1, arg2);
    return result;
}

/* Emitter callback 10 (gBloodDripEmitters): spawns one particle at (x, y, z) pushed against the
 * emitter's movement (dx, dy, dz). */
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    Vec3f position;

    position.x = x;
    position.y = y;
    position.z = z;
    func_151AF388(&position, dx, dy, dz, effect->owner);
}

/* Spawns one randomised particle_spawnSteamParticle particle at `position`, moving at -0.13 * (dx, dy, dz). */
void func_151AF388(Vec3f *position, f32 dx, f32 dy, f32 dz, u8 owner) {
    f32 factor = 0.13000001f;
    Vec3f velocity;
    u8 flag;

    velocity.x = -dx * factor;
    velocity.y = -dy * factor;
    velocity.z = -dz * factor;
    flag = (math_random() & 1) ? 1 : 0;
    particle_spawnSteamParticle(position, &velocity, 0,
                  math_randomFloat() * 0.126f + -0.702f,
                  (math_random() & 0xF) + 10,
                  math_random() % 45 + 137,
                  math_randomFloat() * 202.0f + 142.0f,
                  flag, 1.7f, 1.7f, 0, 0, 1, 0,
                  owner, 1);
}
