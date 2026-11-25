#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 offset, u8 arg9, s32 argA);

void func_15145740(Actor *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4);
s32 func_1514654C(Actor *, ActorCtl *, s32, Vec3f **, f32 **, s32);
s32 func_151C229C(f32 *arg0, s32 arg1, f32 *arg2, f32 *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, Actor *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_151C5588(Actor *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4);
void func_151C56A4(f32 *pos, u8 arg1, s32 arg2);

/* Model-space points that func_1514654C transforms through model part 0x46 into world space. */
extern Vec3f D_800AAA90;
extern Vec3f D_800AAAA8;

/* Spawns a func_151C229C effect along a segment at `obj`: offset along func_15145740's `dir` from its
 * position, or, when it has unk1D4, between two points of model part 0x46, plus func_151C56A4 and
 * (half the time) func_151C5588 effects at the segment start. */
void func_151C5280(Actor *obj, u8 arg1, s32 arg2) {
    f32 start[3];
    f32 end[3];
    f32 dir[3];
    f32 unkB0[3];
    ActorCtl *part;
    Vec3f *modelPoints[2];
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
        if (obj->unk1D4 != NULL) {
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
            start[0] = obj->x_position;
            start[1] = obj->y_position + 106.0f;
            start[2] = obj->z_position;
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
        if (obj->unk1D4 != NULL) {
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

extern void func_15102B38(Actor *, u8, Vec3f *, Vec3f *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern Vec3f D_800AAA9C;

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
    Header header;
    Header2 intPos;

    header.unk0 = 3;
    header.unk1 = -1;
    header.unk2 = (func_150ADA20() % 3U) + 3;
    header.unk4 = 0;

    intPos.unk0 = pos[0];
    intPos.unk4 = pos[1];
    intPos.unk8 = pos[2];

    func_151602C0(&header, &intPos, (func_150ADA20() & 1) + 6, 0xFF, 0xE8, 0xAB, 0xFF, 0, 0, arg1, arg2);
}

/* Per-particle data at 0xB0 of the particles func_151C5F44 spawns. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x04 */ f32 unk4;
} ParticleExtra;

/* Caller-defined data of the particles spawned here (func_151C577C copies only unk0). */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ ParticleExtra extra;
} ParticleData; /* size 0x10 */

/* Live particle, as created by func_15130280; the caller-defined data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ ParticleData data;
} Particle;

extern s32 func_15137C64(Vec3f *, Vec3f *, f32 *, f32 *, f32 *, Vec3f *, Vec3f *, Vec3f *);
extern void func_151DC034(Vec3f *, f32, u8, s16, s32, u8, s32);
extern void func_151DA6F8(Vec3f *, Vec3f *, f32, s16, s32, f32, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, Vec3f *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
void func_151D9014(Vec3f *a0, Vec3f *a1, s32 a2, f32 a3, s32 sp10, s32 sp14, f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C, s32 sp30, s32 sp34, u8 sp38, s32 sp3C);

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
    Particle *particle;

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
                    memcpy(&particle->data.unk0, &unkA8, sizeof(unkA8));
                }
            }
        }
    }
}

void func_15137F30(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, EffectHeader *effect, Vec3f *pos, Vec3f *unk60,
                   Vec3f *vel, f32 *out50, s16 *out4E, u8 *out4D, f32 *out48);

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
Particle *func_151C5F44(Vec3f *pos, Vec3f *vel, f32 unk54, f32 unkA8Value, u8 unk1B, s16 unk0A, f32 size,
                        u8 altUnk62, s32 arg8, u8 arg9, s32 argA) {
    ParticleDesc spawn;
    f32 unkA8;
    Particle *particle;
    ParticleExtra extra;
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
    extra.unk0 = 1;
    extra.unk1 = 0;
    spawn.r = 0xE2;
    spawn.g = 0xB2;
    spawn.b = 0x60;
    spawn.a = 0xFF;
    spawn.r2 = 0x39;
    spawn.g2 = 0xF;
    spawn.b2 = 0;
    spawn.brightness = 0xFF;
    spawn.scaleRate = 1.0f;
    extra.unk4 = 0.703f;
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
    particle = func_15130280(&spawn, 1, arg8, sizeof(ParticleData), arg9, argA);
    if (particle != NULL) {
        memcpy(&particle->data.unk0, &unkA8, sizeof(unkA8));
        memcpy(&particle->data.extra, &extra, sizeof(extra));
    }
    return particle;
}

/* Effect descriptor passed to func_15154684. */
typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ Vec3f *unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s16 unk0C;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ s8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ Vec3f *unk38;
} Effect15154684;

void func_15154684(Effect15154684 *desc, u8 arg1, s32 arg2);

/* Spawns a fixed func_15154684 effect between the points `arg0` and `arg1`. */
void func_151C61A0(Vec3f *arg0, Vec3f *arg1, u8 arg2, s32 arg3) {
    Effect15154684 desc;

    desc.unk00 = 10;
    desc.unk02 = 4;
    desc.unk04 = arg0;
    desc.unk08 = 0;
    desc.unk0C = 255;
    desc.unk0A = -24;
    desc.unk0E = 25;
    desc.unk10 = 5.0f;
    desc.unk14 = 7.0f;
    desc.unk18 = -0.2080000043f;
    desc.unk1C = 0.08500000089f;
    desc.unk20 = 0.9706320167f;
    desc.unk24 = 190;
    desc.unk25 = 65;
    desc.unk26 = 200;
    desc.unk28 = 150;
    desc.unk2C = 96.0f;
    desc.unk30 = 109.0f;
    desc.unk34 = 1.0f;
    desc.unk38 = arg1;
    func_15154684(&desc, arg2, arg3);
}

/* D_80086C90 object filter: rejects objects with id 0x5A, 0x74 or 0x7A. */
s32 func_151C6290(Actor *obj, Actor *self, s32 arg2) {
    if (obj->id == 0x5A || obj->id == 0x74 || obj->id == 0x7A) {
        return FALSE;
    }
    return TRUE;
}
