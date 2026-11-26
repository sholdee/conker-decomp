#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c, game_161520.c and
// game_1DC720.c were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Model-space points: func_151C5280 transforms D_800AAA90 and D_800AAAA8 through model part 0x46 into
 * world space; func_151C5588 passes D_800AAA90 and D_800AAA9C to func_15102B38. */
const Vec3f D_800AAA90 = { -113.8288f, -14.082394f, 14.5422f };
const Vec3f D_800AAA9C = { -124.648605f, -14.082394f, 14.5422f };
const Vec3f D_800AAAA8 = { 7.3199987f, -14.082394f, 4.5422006f };

/* Spawns a func_151C229C effect along a segment at `obj`: offset along func_15145740's `dir` from its
 * position, or, when it has ob_matrix_ptr, between two points of model part 0x46, plus func_151C56A4 and
 * (half the time) func_151C5588 effects at the segment start. */
void func_151C5280(Actor *obj, u8 arg1, s32 arg2) {
    f32 start[3];
    f32 end[3];
    f32 dir[3];
    f32 unkB0[3];
    ModelAttachment *part;
    const Vec3f *modelPoints[2];
    f32 *worldPoints[2];
    s32 arg16;
    s32 arg23;
    s32 randInt;
    f32 randB;
    f32 randA;

    if (obj != NULL) {
        func_15145740(obj, dir, unkB0, NULL, 0.0f);
        dir[0] *= 0.001f;
        dir[1] *= 0.001f;
        dir[2] *= 0.001f;
        if (obj->ob_matrix_ptr != NULL) {
            modelPoints[0] = &D_800AAA90;
            modelPoints[1] = &D_800AAAA8;
            worldPoints[0] = start;
            worldPoints[1] = end;
            part = func_1503195C(obj, 0x46, 0);
            if (part == NULL) {
                return;
            }
            if (func_1514654C(obj, part, 0, modelPoints, worldPoints, 2) == 0) {
                return;
            }
            func_151C56A4(start, arg1, arg2);
            if (func_150ADA20() & 1) {
                func_151C5588(obj, start, 0x46, arg1, arg2);
            }
        } else {
            start[0] = obj->xpos;
            start[1] = obj->ypos + 106.0f;
            start[2] = obj->zpos;
            end[0] = start[0] + (dir[0] * -82.0f);
            end[1] = start[1] + (dir[1] * -82.0f);
            end[2] = start[2] + (dir[2] * -82.0f);
            start[0] -= dir[0] * -120.0f;
            start[1] -= dir[1] * -120.0f;
            start[2] -= dir[2] * -120.0f;
        }
        randA = func_150ADA68();
        randB = func_150ADA68();
        randInt = func_150ADA20();
        if (obj->ob_matrix_ptr != NULL) {
            arg16 = 255;
        } else {
            arg16 = 128;
        }
        if (D_800BE616 != 0) {
            arg23 = -1;
        } else {
            arg23 = 2;
        }
        func_151C229C(start, 0, end, dir, 1, 0, 300.0f, 1.0f / 300.0f, (randA * 10.0f) + 25.0f,
                      (randB * 150.0f) + 400.0f, 80.0f, (randInt % 56U) + 200, obj, 1, 1, 0, arg16, 0, 1, 0,
                      0x1A, 0.0f, 0xFF, arg23, 0, arg1, arg2);
    }
}

/* Spawns a func_15102B38 effect at `pos` on `obj` with randomized scale and parameters. */
void func_151C5588(Actor *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4) {
    f32 scale[2];
    f32 angle;
    s32 rand[2]; /* rand[1] is drawn but never used */

    scale[1] = (func_150ADA68() * 1001.0f + 604.0f) * 0.01f;
    scale[0] = (func_150ADA68() * 59.0f + 141.0f) * 0.1f;
    rand[0] = func_150ADA20();
    rand[1] = func_150ADA20();
    angle = func_150ADA68();
    func_15102B38(obj, 0, &D_800AAA90, &D_800AAA9C, scale, (rand[0] & 3) + 6, 0xFF,
                  angle * 270.0f + 359.0f, pos, 0xFF, 0, arg2, arg3, arg4);
}

/* Spawns a func_151602C0 effect at `pos`, converted to integer coordinates. */
void func_151C56A4(f32 *pos, u8 arg1, s32 arg2) {
    LightEffectHeader header;
    Vec3i intPos;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() % 3U) + 3;
    header.lightType = 0;

    intPos.x = pos[0];
    intPos.y = pos[1];
    intPos.z = pos[2];

    func_151602C0(&header, &intPos, (func_150ADA20() & 1) + 6, 0xFF, 0xE8, 0xAB, 0xFF, 0, 0, arg1, arg2);
}

/* Spawns a burst of effects from the frame func_15137C64 derives from arg0..arg2: always a
 * func_151DC034 effect, then (each when its flag is set) a ring of func_151DA6F8 particles,
 * func_151D9014 particles along `dir`, and func_15130280 particles thrown at random angles. */
void func_151C577C(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2, u8 doForward, u8 doRing, u8 doScatter, u8 arg6,
                   s32 arg7) {
    Vec3f unkPos;
    Vec3f origin;
    f32 dir[3];
    f32 basisA[3];
    f32 basisB[3];
    s16 ringCount;
    f32 sinv;
    f32 cosv;
    Vec3f vel;
    s32 randB;
    s32 bit80;
    Vec3f vel2;
    s32 count;
    ParticleDesc spawn;
    f32 unkA8;
    f32 f;
    f32 f2;
    s32 randA;
    s32 randC;
    SpriteParticle *particle;

    if (func_15137C64(&unkPos, &origin, dir, basisA, basisB, arg0, arg1, arg2) != 0) {
        f2 = func_150ADA68();
        randA = func_150ADA20();
        func_151DC034(&unkPos, f2 * 24.0f + 168.0f, randA % 101U + 155, (func_150ADA20() & 7) + 15, 1, arg6, arg7);
        if (doRing) {
            ringCount = func_150ADA20() % 3U + 5;
            while (ringCount != 0) {
                f = (func_150ADA68() * 33.0f + 60.0f) * 0.1f;
                func_15143874(func_150ADA20() & 0xFF, f, &cosv, &sinv);
                vel.x = basisB[0] * sinv + basisA[0] * cosv;
                vel.y = basisB[1] * sinv + basisA[1] * cosv;
                vel.z = basisB[2] * sinv + basisA[2] * cosv;
                f = func_150ADA68();
                randC = func_150ADA20();
                randA = func_150ADA20();
                f2 = func_150ADA68();
                func_151DA6F8(&unkPos, &vel, f * 0.162f + 0.193f, randC % 9U + 18, randA % 156U + 100,
                              f2 * 4.0f + 4.0f, 3, func_150ADA20() % 1U, 1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, arg6,
                              arg7);
                ringCount--;
            }
            // matching: this dead store gives the ring loop's exit its own block; uopt reloads the loop
            // constants there as well as at the join, and as1 deletes those dead reloads but keeps the
            // extra 0.1f literal that the ROM has
            ringCount = 0;
        }
        if (doForward) {
            for (count = func_150ADA20() % 6U + 5; count != 0; count--) {
                f = (func_150ADA68() * 150.0f + 200.0f) * 0.1f;
                vel2.x = dir[0] * f;
                vel2.y = dir[1] * f;
                vel2.z = dir[2] * f;
                f = func_150ADA68();
                randA = func_150ADA20();
                randB = func_150ADA20();
                func_151D9014(&origin, &vel2, 1, f * 0.682000041f + -0.4120000303f, randA % 11U + 5,
                              randB % 156U + 100, func_150ADA68() * 60.0f + 80.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0, arg6,
                              arg7);
            }
        }
        if (doScatter) {
            count = func_150ADA20() % 7U + 3;
            unkA8 = 0.9508649707f;
            spawn.texture = 0x6C;
            spawn.renderMode = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.fadeStart = 20;
            spawn.fadeRate = 12;
            spawn.flags = 0x80D207;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = 0x10;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.r = 0xE2;
            spawn.g = 0xB2;
            spawn.b = 0x60;
            spawn.a = 0xFF;
            spawn.r2 = 0x39;
            spawn.g2 = 0xF;
            spawn.b2 = 0;
            spawn.brightness = 0xFF;
            spawn.pos = origin;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            for (; count > 0; count--) {
                randA = func_150ADA20();
                randC = func_150ADA20();
                func_15143794(randA & 0xFF, -(randC & 0x3F), func_150ADA68() * 7.0f + 2.0f, &spawn.vel);
                f = func_150ADA68() * 117.0f;
                spawn.flags &= ~0xC0;
                spawn.gravity = (f + -197.0f) * 0.001f;
                bit80 = (func_150ADA20() & 1) ? 0x80 : 0;
                // matching: reusing randB for the 0x40 bit; a separate local changes the frame
                randB = (func_150ADA20() & 1) ? 0x40 : 0;
                spawn.flags |= randB | bit80;
                spawn.alpha = func_150ADA20() % 66U + 190;
                spawn.timer = func_150ADA20() % 45U + 44;
                f2 = func_150ADA68() * 122.0f + 77.0f;
                spawn.width = f2;
                spawn.height = f2;
                particle = func_15130280(&spawn, 1, 0, sizeof(f32), arg6, arg7);
                if (particle != NULL) {
                    memcpy(&particle->data.drag, &unkA8, sizeof(unkA8));
                }
            }
        }
    }
}

/* Spawns one func_151D9014 particle whose position, velocity and parameters func_15137F30 computes. */
void func_151C5E74(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, EffectHeader *effect) {
    Vec3f pos;
    Vec3f unk60;
    Vec3f vel;
    /* func_151D9014 arguments 3..6 */
    f32 param3;
    s16 param4;
    u8 param5;
    f32 param6;

    func_15137F30(a0, a1, a2, a3, arg4, effect, &pos, &unk60, &vel, &param3, &param4, &param5, &param6);
    func_151D9014(&pos, &vel, 1, param3, param4, param5, param6, 0, 1.0f, 1.0f, 1, 0, 1, 0, effect->owner,
                  effect->kind);
}

/* Spawns a func_15130280 particle at `pos` moving at `vel`. A `unk0A` of -1 means 300 and leaves flag
 * bit 0 clear. Returns the particle, or NULL. */
SpriteParticle *func_151C5F44(Vec3f *pos, Vec3f *vel, f32 unk54, f32 unkA8Value, u8 unk1B, s16 unk0A, f32 size,
                        u8 altUnk62, CollisionHit *arg8, u8 arg9, s32 argA) {
    ParticleDesc spawn;
    f32 unkA8;
    SpriteParticle *particle;
    ParticleBounce bounce;
    s32 bit80;
    s32 hasUnk0A;
    s32 bit40;

    unkA8 = unkA8Value;
    spawn.texture = 0x6C;
    spawn.renderMode = 0x5103;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.fadeStart = 20;
    spawn.fadeRate = 12;
    bit40 = (func_150ADA20() & 1) ? 0x40 : 0;
    hasUnk0A = (unk0A == -1) ? 0 : 1;
    bit80 = (func_150ADA20() & 1) ? 0x80 : 0;
    spawn.flags = bit80 | hasUnk0A | 2 | 4 | bit40 | 0x4000 | 0x8000 | 0x200 | 0x800000;
    spawn.primColorMode = 8;
    spawn.envColorMode = 6;
    if (altUnk62) {
        spawn.updateFunc = 0x1D;
    } else {
        spawn.updateFunc = 0xA;
    }
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.scaleStart = 1;
    spawn.handlerSet = 0;
    bounce.flags = 1;
    bounce.hitFunc = 0;
    spawn.r = 0xE2;
    spawn.g = 0xB2;
    spawn.b = 0x60;
    spawn.a = 0xFF;
    spawn.r2 = 0x39;
    spawn.g2 = 0xF;
    spawn.b2 = 0;
    spawn.brightness = 0xFF;
    spawn.scaleRate = 1.0f;
    bounce.restitution = 0.703f;
    spawn.pos = *pos;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.vel = *vel;
    spawn.gravity = unk54;
    spawn.alpha = unk1B;
    if (unk0A == -1) {
        spawn.timer = 300;
    } else {
        spawn.timer = unk0A;
    }
    spawn.height = size;
    spawn.width = size;
    particle = func_15130280(&spawn, 1, arg8, sizeof(particle->data.dragBounce), arg9, argA);
    if (particle != NULL) {
        memcpy(&particle->data.dragBounce.drag, &unkA8, sizeof(unkA8));
        memcpy(&particle->data.dragBounce.bounce, &bounce, sizeof(bounce));
    }
    return particle;
}

/* Spawns a fixed func_15154684 effect between the points `arg0` and `arg1`. */
void func_151C61A0(Vec3f *arg0, CollisionHit *arg1, u8 arg2, s32 arg3) {
    ParticleBurst desc;

    desc.countBase = 10;
    desc.countRange = 4;
    desc.pos = arg0;
    desc.yawBase = 0;
    desc.yawRange = 255;
    desc.pitchBase = -24;
    desc.pitchRange = 25;
    desc.speedBase = 5.0f;
    desc.speedRange = 7.0f;
    desc.gravityBase = -0.2080000043f;
    desc.gravityRange = 0.08500000089f;
    desc.unk20 = 0.9706320167f;
    desc.alphaBase = 190;
    desc.alphaRange = 65;
    desc.timerBase = 200;
    desc.timerRange = 150;
    desc.sizeBase = 96.0f;
    desc.sizeRange = 109.0f;
    desc.altChance = 1.0f;
    desc.ground = arg1;
    func_15154684(&desc, arg2, arg3);
}

/* D_80086C90 object filter: rejects objects with id 0x5A, 0x74 or 0x7A. */
s32 func_151C6290(Actor *obj, Actor *self, s32 arg2) {
    if (obj->character_type == 0x5A || obj->character_type == 0x74 || obj->character_type == 0x7A) {
        return FALSE;
    }
    return TRUE;
}
