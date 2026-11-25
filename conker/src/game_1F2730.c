#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 offset, u8 arg9, s32 argA);

void func_15145740(struct127 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4);
s32 func_1514654C(struct127 *, struct126 *, s32, vertex **, f32 **, s32);
s32 func_151C229C(f32 *arg0, s32 arg1, f32 *arg2, f32 *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11, struct127 *arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, s32 arg17, s32 arg18, s32 arg19, s32 arg20, f32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26);
void func_151C5588(struct127 *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4);
void func_151C56A4(f32 *pos, u8 arg1, s32 arg2);

/* Model-space points (model part 0x46) that func_151C5280 transforms to world space. */
extern vertex D_800AAA90;
extern vertex D_800AAAA8;

/* Spawns a func_151C229C effect along a segment in front of `obj`; with a model (unk1D4), from model
 * part 0x46 plus extra func_151C56A4 / func_151C5588 effects at its start. */
void func_151C5280(struct127 *obj, u8 arg1, s32 arg2) {
    f32 start[3];
    f32 end[3];
    f32 dir[3];
    f32 unkB0[3];
    struct126 *part;
    vertex *modelPoints[2];
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

extern void func_15102B38(struct127 *, u8, vertex *, vertex *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern vertex D_800AAA9C;

/* Spawns a randomly sized and rotated func_15102B38 effect at `pos` on `obj`. */
void func_151C5588(struct127 *obj, f32 *pos, s16 arg2, u8 arg3, s32 arg4) {
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

/* Particle spawn template passed to func_15130280 (same layout as ParticleSpawn in game_1028F0.c). */
typedef struct {
    /* 0x00 */ u32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
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
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} ParticleSpawn; /* size 0x70 */

/* Per-particle data at 0xB0 of the particles func_151C5F44 spawns. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x04 */ f32 unk4;
} ParticleExtra;

/* Live particle, as created by func_15130280; the caller-defined data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 unkA8;
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ ParticleExtra extra;
} Particle;

extern s32 func_15137C64(vertex *, vertex *, f32 *, f32 *, f32 *, vertex *, vertex *, vertex *);
extern void func_151DC034(vertex *, f32, u8, s16, s32, u8, s32);
extern void func_151DA6F8(vertex *, vertex *, f32, s16, s32, f32, s32, s32, f32, f32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, vertex *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern Particle *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);
void func_151D9014(vertex *a0, vertex *a1, s32 a2, f32 a3, s32 sp10, s32 sp14, f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C, s32 sp30, s32 sp34, u8 sp38, s32 sp3C);

/* Spawns a burst of effects from the frame func_15137C64 derives from arg0..arg2: always a
 * func_151DC034 effect, then (each when its flag is set) a ring of func_151DA6F8 particles,
 * func_151D9014 particles along `dir`, and func_15130280 particles thrown at random angles. */
void func_151C577C(vertex *arg0, vertex *arg1, vertex *arg2, u8 doForward, u8 doRing, u8 doScatter, u8 arg6,
                   s32 arg7) {
    vertex unkPos;
    vertex origin;
    f32 dir[3];
    f32 basisA[3];
    f32 basisB[3];
    s16 ringCount;
    f32 sinv;
    f32 cosv;
    vertex vel;
    s32 randB;
    s32 bit80;
    vertex vel2;
    s32 count;
    ParticleSpawn spawn;
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
            if (ringCount != 0) {
                do {
                    f = (func_150ADA68() * 33.0f + 60.0f) * 0.1f;
                    func_15143874(func_150ADA20() & 0xFF, f, &cosv, &sinv);
                    vel.x = basisB[0] * sinv + basisA[0] * cosv;
                    vel.y = basisB[1] * sinv + basisA[1] * cosv;
                    vel.z = basisB[2] * sinv + basisA[2] * cosv;
                    f = func_150ADA68();
                    randC = func_150ADA20();
                    randA = func_150ADA20();
                    f2 = func_150ADA68();
                    func_151DA6F8(&unkPos, &vel, f * 0.162f + 0.193f, randC % 9U + 18, randA % 156U + 100, f2 * 4.0f + 4.0f, 3, func_150ADA20() & 0, 1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, arg6, arg7);
                    ringCount--;
                } while (ringCount != 0);
            }
        }
        if (doForward) {
            count = func_150ADA20() % 6U + 5;
            if (count != 0) {
                do {
                    f = (func_150ADA68() * 150.0f + 200.0f) * 0.1f;
                    vel2.x = dir[0] * f;
                    vel2.y = dir[1] * f;
                    vel2.z = dir[2] * f;
                    f = func_150ADA68();
                    randA = func_150ADA20();
                    randB = func_150ADA20();
                    func_151D9014(&origin, &vel2, 1, f * 0.682000041f + -0.4120000303f, randA % 11U + 5, randB % 156U + 100, func_150ADA68() * 60.0f + 80.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0, arg6, arg7);
                    count--;
                } while (count != 0);
            }
        }
        if (doScatter) {
            count = func_150ADA20() % 7U + 3;
            unkA8 = 0.9508649707f;
            spawn.unk1D = 0x6C;
            spawn.unk08 = 0x5103;
            spawn.unk00 = 0x200005;
            spawn.unk1E = 20;
            spawn.unk20 = 12;
            spawn.flags = 0x80D207;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x10;
            spawn.unk04 = 0;
            spawn.unk0C = 0;
            spawn.unk10 = 0;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk22 = 1;
            spawn.unk24 = 1.0f;
            spawn.unk14 = 0xE2;
            spawn.unk15 = 0xB2;
            spawn.unk16 = 0x60;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0x39;
            spawn.unk19 = 0xF;
            spawn.unk1A = 0;
            spawn.unk1C = 0xFF;
            spawn.pos = origin;
            spawn.unk3C.x = 0.0f;
            spawn.unk3C.y = 0.0f;
            spawn.unk3C.z = 0.0f;
            for (; count > 0; count--) {
                randA = func_150ADA20();
                randC = func_150ADA20();
                func_15143794(randA & 0xFF, -(randC & 0x3F), func_150ADA68() * 7.0f + 2.0f, &spawn.vel);
                f = func_150ADA68() * 117.0f;
                spawn.flags &= ~0xC0;
                spawn.unk54 = (f + -197.0f) * 0.001f;
                bit80 = (func_150ADA20() & 1) ? 0x80 : 0;
                randB = (func_150ADA20() & 1) ? 0x40 : 0;
                spawn.flags |= randB | bit80;
                spawn.unk1B = func_150ADA20() % 66U + 190;
                spawn.unk0A = func_150ADA20() % 45U + 44;
                f2 = func_150ADA68() * 122.0f + 77.0f;
                spawn.unk28 = f2;
                spawn.unk2C = f2;
                particle = func_15130280(&spawn, 1, 0, sizeof(f32), arg6, arg7);
                if (particle != NULL) {
                    memcpy(&particle->unkA8, &unkA8, sizeof(unkA8));
                }
            }
        }
    }
}

/* Effect record passed to func_151C5E74; unkC and unk1 are the owner arguments of spawned particles. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
} Effect151C5E74;

void func_15137F30(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, Effect151C5E74 *effect, vertex *pos, vertex *unk60,
                   vertex *vel, f32 *out50, s16 *out4E, u8 *out4D, f32 *out48);

/* Spawns one func_151D9014 particle whose position, velocity and parameters func_15137F30 computes. */
void func_151C5E74(f32 *a0, f32 *a1, f32 *a2, f32 *a3, f32 arg4, Effect151C5E74 *effect) {
    vertex pos;
    vertex unk60;
    vertex vel;
    /* func_151D9014 arguments 3..6 */
    f32 param3;
    s16 param4;
    u8 param5;
    f32 param6;

    func_15137F30(a0, a1, a2, a3, arg4, effect, &pos, &unk60, &vel, &param3, &param4, &param5, &param6);
    func_151D9014(&pos, &vel, 1, param3, param4, param5, param6, 0, 1.0f, 1.0f, 1, 0, 1, 0, effect->unkC,
                  effect->unk1);
}

/* Spawns a func_15130280 particle at `pos` moving at `vel`. A `unk0A` of -1 means 300 and leaves flag
 * bit 0 clear. Returns the particle, or NULL. */
Particle *func_151C5F44(vertex *pos, vertex *vel, f32 unk54, f32 unkA8Value, u8 unk1B, s16 unk0A, f32 size,
                        u8 altUnk62, s32 arg8, u8 arg9, s32 argA) {
    ParticleSpawn spawn;
    f32 unkA8;
    Particle *particle;
    ParticleExtra extra;
    s32 bit80;
    s32 hasUnk0A;
    s32 bit40;

    unkA8 = unkA8Value;
    spawn.unk1D = 0x6C;
    spawn.unk08 = 0x5103;
    spawn.unk00 = 0x200005;
    spawn.unk04 = 0;
    spawn.unk0C = 0;
    spawn.unk10 = 0;
    spawn.unk1E = 20;
    spawn.unk20 = 12;
    bit40 = (func_150ADA20() & 1) ? 0x40 : 0;
    hasUnk0A = (unk0A == -1) ? 0 : 1;
    bit80 = (func_150ADA20() & 1) ? 0x80 : 0;
    spawn.flags = bit80 | hasUnk0A | 2 | 4 | bit40 | 0x4000 | 0x8000 | 0x200 | 0x800000;
    spawn.unk60 = 8;
    spawn.unk61 = 6;
    if (altUnk62) {
        spawn.unk62 = 0x1D;
    } else {
        spawn.unk62 = 0xA;
    }
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk22 = 1;
    spawn.unk65 = 0;
    extra.unk0 = 1;
    extra.unk1 = 0;
    spawn.unk14 = 0xE2;
    spawn.unk15 = 0xB2;
    spawn.unk16 = 0x60;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0x39;
    spawn.unk19 = 0xF;
    spawn.unk1A = 0;
    spawn.unk1C = 0xFF;
    spawn.unk24 = 1.0f;
    extra.unk4 = 0.703f;
    spawn.pos = *pos;
    spawn.unk3C.x = 0.0f;
    spawn.unk3C.y = 0.0f;
    spawn.unk3C.z = 0.0f;
    spawn.vel = *vel;
    spawn.unk54 = unk54;
    spawn.unk1B = unk1B;
    if (unk0A == -1) {
        spawn.unk0A = 300;
    } else {
        spawn.unk0A = unk0A;
    }
    spawn.unk2C = size;
    spawn.unk28 = size;
    particle = func_15130280(&spawn, 1, arg8, sizeof(f32) + 4 + sizeof(ParticleExtra), arg9, argA);
    if (particle != NULL) {
        memcpy(&particle->unkA8, &unkA8, sizeof(unkA8));
        memcpy(&particle->extra, &extra, sizeof(extra));
    }
    return particle;
}

struct conker151C61A0 {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s32 unk04;
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
    /* 0x38 */ s32 unk38;
};

void func_15154684(struct conker151C61A0 *arg0, u8 arg1, s32 arg2);

extern f32 D_800AAAF4;
extern f32 D_800AAAF8;
extern f32 D_800AAAFC;

void func_151C61A0(s32 a0, s32 a1, u8 a2, s32 a3) {
    struct conker151C61A0 sp1C;

    sp1C.unk00 = 0xA;
    sp1C.unk02 = 4;
    sp1C.unk04 = a0;
    sp1C.unk08 = 0;
    sp1C.unk0C = 0xFF;
    sp1C.unk0A = -0x18;
    sp1C.unk0E = 0x19;
    sp1C.unk10 = 5.0f;
    sp1C.unk14 = 7.0f;
    sp1C.unk18 = D_800AAAF4;
    sp1C.unk1C = D_800AAAF8;
    sp1C.unk20 = D_800AAAFC;
    sp1C.unk24 = 0xBE;
    sp1C.unk25 = 0x41;
    sp1C.unk26 = 0xC8;
    sp1C.unk28 = 0x96;
    sp1C.unk2C = 96.0f;
    sp1C.unk30 = 109.0f;
    sp1C.unk34 = 1.0f;
    sp1C.unk38 = a1;
    func_15154684(&sp1C, a2, a3);
}

s32 func_151C6290(u8 *arg0, s32 arg1, s32 arg2) {
    if (arg0[4] == 0x5A || arg0[4] == 0x74 || arg0[4] == 0x7A) {
        return 0;
    }
    return 1;
}
