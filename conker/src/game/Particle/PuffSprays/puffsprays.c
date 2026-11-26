#include <ultra64.h>

#include "functions.h"
#include "variables.h"

// particle_spawnSteamParticle (game/Particle/Steam/steam.c) is declared in each file that calls it: game/Beam/ModelBeam/modelbeam.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

void func_150C1F60(u8 arg0) {
    PuffSpray params;

    params.pos.x = 8152.0f;
    params.pos.y = 40.0f;
    params.pos.z = -309.0f;
    params.speedBase = 40.92f;
    params.speedRange = 20.45f;
    params.sizeBase = 9.0f;
    params.sizeRange = 21.9f;
    params.countBase = 0xC;
    params.countRange = 0xD;
    params.angles.yawBase = -0xA0;
    params.angles.pitchBase = -0x3C;
    params.angles.yawRange = 0x45;
    params.angles.pitchRange = 0x43;
    params.itemCountBase = 3;
    params.itemCountRange = 5;
    params.lifeBase = 0x32;
    params.lifeRange = 0x1F;
    params.growthBase = 1.46f;
    params.growthRange = 0.5f;
    params.green = 0xB;
    params.headerFlags = 1;
    params.red = 0x28;
    params.handlers[0] = 1;
    params.handlers[1] = 4;
    params.handlers[2] = 0;
    params.handlers[3] = 0;
    params.handlers[4] = 0;
    params.handlers[5] = 0;
    params.blue = 0xFF;
    params.alphaBase = 0xFF;
    params.alphaRange = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.render.unk0 = 0;
    params.render.setGeometryMode = 0x220005;
    params.render.clearGeometryMode = 0x1D0600;
    params.render.renderMode = 1;
    params.render.unk10 = 0x3B;
    params.render.unk14 = 0x80;
    params.render.unk18 = 0x20;
    params.render.primMode = 0;
    params.render.envMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    particle_spawnPuffSpray(&params, arg0, 1);
    params.speedBase = 100.0f;
    params.speedRange = 57.5f;
    params.sizeBase = 55.6f;
    params.sizeRange = 39.0f;
    params.countBase = 2;
    params.countRange = 3;
    params.angles.yawBase = -0x6C;
    params.angles.pitchBase = -0x16;
    params.angles.yawRange = 0x15;
    params.angles.pitchRange = 0x16;
    params.itemCountBase = 7;
    params.itemCountRange = 0;
    params.lifeBase = 0x30;
    params.lifeRange = 0x15;
    params.growthBase = 0.46f;
    params.growthRange = 1.0f;
    particle_spawnPuffSpray(&params, arg0, 1);
    {
    DropSpraySpec params2;
    params2.spray.pos = params.pos;
    params2.spray.countBase = 0x19;
    params2.spray.countRange = 0x12;
    params2.angles.yawBase = -0x9F;
    params2.angles.yawRange = 0x44;
    params2.angles.pitchBase = -0x34;
    params2.angles.pitchRange = 0x26;
    params2.spray.lifeBase = 0x19;
    params2.spray.lifeRange = 0x19;
    params2.spray.unk24 = 0x9B;
    params2.spray.unk25 = 0x64;
    params2.spray.unk30 = 1;
    params2.spray.kind = 6;
    params2.spray.unk34 = 0.0f;
    params2.spray.wobbles = 1;
    params2.spray.bounceChance = 0.0f;
    params2.spray.speedBase = 40.0f;
    params2.spray.speedRange = 35.0f;
    params2.spray.gravityBase = -2.0210001f;
    params2.spray.gravityRange = 0.89400005f;
    params2.spray.sizeBase = 1152.0f;
    params2.spray.sizeRange = 848.0f;
    particle_spawnDropSprayAtAngles(&params2.angles, &params2.spray, 0, arg0, 1);
}
}

void func_150C2290(u8 arg0) {
    PuffSpray params;

    params.pos.x = 8152.0f;
    params.pos.y = 40.0f;
    params.pos.z = 309.0f;
    params.speedBase = 100.0f;
    params.speedRange = 57.5f;
    params.sizeBase = 55.6f;
    params.sizeRange = 39.0f;
    params.countBase = 2;
    params.countRange = 3;
    params.angles.yawBase = -0x29;
    params.angles.pitchBase = -0x16;
    params.angles.yawRange = 0x15;
    params.angles.pitchRange = 0x16;
    params.itemCountBase = 7;
    params.itemCountRange = 0;
    params.lifeBase = 0x30;
    params.lifeRange = 0x15;
    params.growthBase = 0.46f;
    params.growthRange = 1.0f;
    params.green = 0xB;
    params.headerFlags = 1;
    params.red = 0x28;
    params.handlers[0] = 1;
    params.handlers[1] = 4;
    params.handlers[2] = 0;
    params.handlers[3] = 0;
    params.handlers[4] = 0;
    params.handlers[5] = 0;
    params.blue = 0xFF;
    params.alphaBase = 0xFF;
    params.alphaRange = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.render.unk0 = 0;
    params.render.setGeometryMode = 0x220005;
    params.render.clearGeometryMode = 0x1D0600;
    params.render.renderMode = 1;
    params.render.unk10 = 0x3B;
    params.render.unk14 = 0x80;
    params.render.unk18 = 0x20;
    params.render.primMode = 0;
    params.render.envMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    particle_spawnPuffSpray(&params, arg0, 1);
}

void func_150C2424(u8 arg0) {
    particle_emitOnLine(8500.0f, -490.0f, -328.0f, 8500.0f, -490.0f, 328.0f,
                  (math_randomFloat() * 8.0f) + 8.0f, 1, 0, 0.0f, 0, arg0);
    particle_emitOnLine(8843.0f, -560.0f, -580.0f, 8117.0f, -560.0f, -580.0f,
                  (math_randomFloat() * 3.0f) + 4.0f, 3, 0, 0.0f, 0, arg0);
}

/* Trail update handler (gMotionTrailUpdateProcs): sets the trail's alpha from its remaining life (as trail_fadeKind2Trail). */
s32 particle_fadeMotionTrailByLife(MotionTrail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->head.tmpl.life << 3;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    if (params->motion.alpha < 0) { // never true for a u8, but the compiled code has this compare
        return 0;
    }
    return 1;
}

s32 func_150C2558(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    PuffSpray params;

    params.pos.x = x;
    params.pos.y = y;
    params.pos.z = (z + 15.0f) - (math_randomFloat() * 30.0f);
    params.countBase = 1;
    params.countRange = 0;
    params.angles.yawBase = (s32)(params.pos.z * 0.07012195f) - 0x40;
    params.angles.yawRange = 9;
    params.angles.pitchBase = 0;
    params.angles.pitchRange = 0;
    params.itemCountBase = 3;
    params.itemCountRange = 3;
    params.lifeBase = 0x28;
    params.lifeRange = 0x15;
    params.speedBase = 200.0f;
    params.speedRange = 150.0f;
    params.sizeBase = 22.0f;
    params.growthBase = 0.0f;
    params.growthRange = 0.0f;
    params.sizeRange = 44.0f;
    params.green = 0xB;
    params.headerFlags = 1;
    params.red = 0x28;
    params.handlers[0] = 1;
    params.handlers[1] = 4;
    params.handlers[2] = 0;
    params.handlers[3] = 0;
    params.handlers[4] = 0;
    params.handlers[5] = 0;
    params.blue = 0xFF;
    params.alphaBase = 0xFF;
    params.alphaRange = 0;
    params.unk58 = 3;
    params.unk5C = 0xFF;
    params.render.unk0 = 0;
    params.render.setGeometryMode = 0x220005;
    params.render.clearGeometryMode = 0x1D0600;
    params.render.renderMode = 1;
    params.render.unk10 = 0x3B;
    params.render.unk14 = 0x80;
    params.render.unk18 = 0x20;
    params.render.primMode = 0;
    params.render.envMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    particle_spawnPuffSpray(&params, argE, 1);
    return 1;
}

s32 func_150C2700(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    DropSpraySpec params;

    params.spray.pos.x = x;
    params.spray.pos.y = y;
    params.spray.countBase = 6;
    params.spray.countRange = 5;
    params.angles.yawBase = 0x6B;
    params.angles.yawRange = 0x46;
    params.angles.pitchBase = -0x1F;
    params.angles.pitchRange = 0x2E;
    params.spray.lifeBase = 0x64;
    params.spray.lifeRange = 0;
    params.spray.unk24 = 0x9B;
    params.spray.unk25 = 0x64;
    params.spray.sizeRange = 0.0f;
    params.spray.unk30 = 1;
    params.spray.kind = 6;
    params.spray.unk34 = 0.0f;
    params.spray.wobbles = 1;
    params.spray.bounceChance = 0.0f;
    params.spray.pos.z = z;
    params.spray.speedBase = 30.0f;
    params.spray.speedRange = 35.0f;
    params.spray.gravityBase = -2.328f;
    params.spray.gravityRange = 1.6630001f;
    params.spray.sizeBase = 1655.0f;
    particle_spawnDropSprayAtAngles(&params.angles, &params.spray, 0, argE, 1);
    return 1;
}

void func_150C2804(f32 *x, f32 *y, f32 *z, s16 delay, s32 arg4, s32 arg5) {
    BloodTrailDesc params;

    params.radius = 10.3f;
    params.x = x;
    params.y = y;
    params.z = z;
    params.rate = 0.5009f;
    params.lifetime = delay;
    params.flags = 5;
    params.emitter = 6;
    params.unk18 = 3;
    params.callback = -1;
    blood_spawnTrail(&params, 0, arg4, arg5);
}

void func_150C2898(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter) {
    Vec3f pos;
    Vec3f vel;
    f32 speed;
    f32 rand3;
    u32 rand2;
    u32 rand1;
    f32 rand0;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    speed = (math_randomFloat() * 112.0f + 247.0f) * 0.001f;
    vel.x = -dirX * speed;
    vel.y = -dirY * speed;
    vel.z = -dirZ * speed;
    rand0 = math_randomFloat();
    rand1 = math_random();
    rand2 = math_random();
    rand3 = math_randomFloat();
    particle_spawnSteamParticle(&pos, &vel, 6, rand0 * 0.27f + -0.31f,
                    (rand1 & 0xF) + 25, (rand2 % 101) + 155,
                    rand3 * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    emitter->base.header.owner, emitter->base.header.kind);
}
