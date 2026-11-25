#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct PipeVec3f {
    f32 x, y, z;
} PipeVec3f;

/* One of the three exploding pipes: where it is and which way it faces. */
typedef struct PipeSpot {
    PipeVec3f pos;
    u8 yaw;
} PipeSpot;

typedef struct PipeIds {
    s32 id[3];
} PipeIds;

/* Parameter block for func_151A6F00. */
typedef struct Fx151A6F00Args {
    s16 unk00;
    s16 pad02;
    PipeVec3f pos;
    s16 unk10;
    s16 pad12;
    f32 unk14;
    s16 angle;
    s16 unk1A, unk1C, unk1E;
    f32 unk20, unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44, unk48, unk4C, unk50;
    u8 unk54, unk55;
    s16 unk56, unk58;
} Fx151A6F00Args;

/* Parameter block for func_151539B4. */
typedef struct Fx151539B4Args {
    s16 unk00, unk02;
    PipeVec3f pos;
    s16 angle;
    s16 unk12, unk14, unk16;
    f32 unk18, unk1C, unk20, unk24;
    u8 unk28, unk29, unk2A, unk2B, unk2C, unk2D, unk2E, unk2F, unk30;
    u8 pad31;
    s16 unk32, unk34, unk36;
    f32 unk38, unk3C;
    u8 unk40, unk41;
    s16 unk42, unk44;
    u8 unk46;
    s32 unk48;
} Fx151539B4Args;

/* Parameter block for func_15152F70. */
typedef struct Fx15152F70Args {
    PipeVec3f pos;
    f32 unk0C, unk10, unk14, unk18, unk1C, unk20;
    s16 unk24, unk26;
    s16 angle;
    s16 unk2A, unk2C, unk2E, unk30, unk32, unk34, unk36;
    u8 unk38, unk39;
    s16 pad3A;
    s32 unk3C, unk40, unk44, unk48, unk4C, unk50;
    u8 unk54, unk55;
} Fx15152F70Args;

/* Parameter block for func_150B3F5C. */
typedef struct Fx150B3F5CArgs {
    PipeVec3f vel;
    f32 unk0C;
    u8 unk10;
    f32 unk14, unk18;
    u8 unk1C;
    f32 unk20;
    u8 unk24;
    s16 unk26;
    f32 unk28, unk2C;
    u8 unk30, unk31, unk32, unk33, unk34, unk35;
    s16 unk36;
    u8 unk38;
    f32 unk3C;
} Fx150B3F5CArgs;

extern PipeSpot D_8009F920[];
extern PipeIds D_8009F950;
extern PipeIds D_8009F95C;
extern void func_151A6F00(Fx151A6F00Args *, s32, u8, s32);
extern void func_1514C858(f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, f32, s32, u8);
extern void func_151539B4(Fx151539B4Args *, u8);
extern void func_15152F70(Fx15152F70Args *, u8);
extern void func_150B3F5C(Fx150B3F5CArgs *, PipeVec3f *, s32);

/* Spawns the full explosion effect (debris, smoke, sparks, sounds) for pipe `pipe`. */
void func_150B2740(u8 pipe, u8 arg1) {
    Fx151A6F00Args argsA;
    s16 i;
    Fx151539B4Args argsB;
    Fx15152F70Args argsC;
    f32 yawSin; // matching: also holds the debris size; a separate local mismatches
    f32 yawCos;
    PipeVec3f origin;
    Fx150B3F5CArgs argsD;

    func_1512D748(&D_800DBFF0[D_800BE9E8], 7, 1);
    argsA.unk00 = (func_150ADA20() & 1) + 3;
    argsA.pos = D_8009F920[pipe].pos;
    argsA.unk14 = 170.0f;
    argsA.unk20 = 16.20000076f;
    argsA.unk24 = 17.60000038f;
    argsA.unk28 = 19.30000114f;
    argsA.unk2C = 21.30000114f;
    argsA.unk10 = 300;
    argsA.angle = D_8009F920[pipe].yaw - 0x20;
    argsA.unk1A = 65;
    argsA.unk1C = -40;
    argsA.unk1E = 80;
    argsA.unk54 = 1;
    argsA.unk55 = 0;
    argsA.unk56 = 50;
    argsA.unk58 = 25;
    argsA.unk30 = 0.0f;
    argsA.unk34 = 0.0f;
    argsA.unk38 = 0.0f;
    argsA.unk3C = 0.4670000076f;
    argsA.unk40 = 0.5139999986f;
    argsA.unk44 = -1.0f;
    argsA.unk48 = -2.600000143f;
    argsA.unk4C = -5.0f;
    argsA.unk50 = 10.0f;
    func_151A6F00(&argsA, 0, arg1, 1);
    i = (func_150ADA20() & 1) + 1;
    while (i--) {
        yawSin = (func_150ADA68() * 73.0f) + 31.0f;
        func_1514C858(D_8009F920[pipe].pos.x, D_8009F920[pipe].pos.y, D_8009F920[pipe].pos.z, yawSin, D_8009F920[pipe].yaw + 0x40, 0, 0xFF, (func_150ADA20() % 3U) + 4, 9, 0, 0.009615384974f, 0, arg1);
    }
    argsB.unk00 = 30;
    argsB.unk02 = 15;
    argsB.pos = D_8009F920[pipe].pos;
    argsB.angle = D_8009F920[pipe].yaw - 0x40;
    argsB.unk14 = 128;
    argsB.unk12 = -45;
    argsB.unk16 = 90;
    argsB.unk28 = 0xFF;
    argsB.unk29 = 0xFF;
    argsB.unk2A = 0xFF;
    argsB.unk2B = 0xFF;
    argsB.unk2C = 0xFF;
    argsB.unk2D = 0xFF;
    argsB.unk2E = 0xFF;
    argsB.unk2F = 0xFF;
    argsB.unk30 = 9;
    argsB.unk32 = 16;
    argsB.unk34 = 22;
    argsB.unk36 = 0x1601;
    argsB.unk40 = 0x7F;
    argsB.unk41 = 0x7F;
    argsB.unk42 = 10;
    argsB.unk44 = 25;
    argsB.unk46 = 0;
    argsB.unk18 = 6.0f;
    argsB.unk1C = 2.549999952f;
    argsB.unk20 = 2.700000048f;
    argsB.unk24 = 3.419999838f;
    argsB.unk38 = 4.139999866f;
    argsB.unk3C = 4.859999657f;
    func_151539B4(&argsB, arg1);
    {
    // matching: a declaration here opens a -g3 lexical block, which steers as1's scheduling
    extern void func_15152F70(Fx15152F70Args *, u8);

    argsC.pos = D_8009F920[pipe].pos;
    argsC.unk24 = 15;
    argsC.unk26 = 8;
    argsC.angle = D_8009F920[pipe].yaw - 25;
    argsC.unk2A = 50;
    argsC.unk2C = -50;
    argsC.unk2E = 60;
    argsC.unk30 = 3;
    argsC.unk32 = 2;
    argsC.unk34 = 60;
    argsC.unk36 = 20;
    argsC.unk38 = 6;
    argsC.unk39 = 8;
    argsC.unk3C = 1;
    argsC.unk40 = 7;
    argsC.unk44 = 0;
    argsC.unk48 = 0;
    argsC.unk4C = 0;
    argsC.unk50 = 0;
    argsC.unk54 = 0xFF;
    argsC.unk55 = 0xA0;
    argsC.unk1C = 20.0f;
    argsC.unk20 = 40.0f;
    argsC.unk0C = 10.0f;
    argsC.unk10 = 30.0f;
    argsC.unk14 = 1.5f;
    argsC.unk18 = 1.0f;
    func_15152F70(&argsC, arg1);
    }
    argsC.unk24 = 10;
    argsC.unk26 = 6;
    argsC.unk34 = 100;
    argsC.unk36 = 0;
    argsC.unk39 = 9;
    argsC.unk48 = 5;
    func_15152F70(&argsC, arg1);
    yawSin = func_151423D8(D_8009F920[pipe].yaw);
    yawCos = func_151423D8(D_8009F920[pipe].yaw - 0x40);
    origin.x = D_8009F920[pipe].pos.x - 150.0f * yawCos;
    origin.y = D_8009F920[pipe].pos.y;
    origin.z = D_8009F920[pipe].pos.z - 150.0f * yawSin;
    argsD.unk24 = 0x80;
    argsD.vel.x = 10.0f * yawCos;
    argsD.vel.y = 3.0f;
    argsD.vel.z = 10.0f * yawSin;
    argsD.unk0C = -3.710000038f;
    argsD.unk26 = 0xFF;
    argsD.unk30 = 2;
    argsD.unk31 = 5;
    argsD.unk10 = 4;
    argsD.unk38 = 0;
    argsD.unk36 = 30;
    argsD.unk1C = 6;
    argsD.unk32 = 5;
    argsD.unk33 = 0x33;
    argsD.unk34 = 3;
    argsD.unk35 = 0x55;
    argsD.unk20 = 0.400000006f;
    argsD.unk2C = 47.0f;
    argsD.unk28 = 1.840000033f;
    argsD.unk14 = 3.299999952f;
    argsD.unk18 = 0.6999999881f;
    argsD.unk3C = 0.3000000119f;
    {
        u8 idBuf[4];
        PipeIds idsA;
        PipeIds idsB;

        idsA = D_8009F950;
        idsB = D_8009F95C;
        idBuf[0] = idsA.id[pipe];
        func_151494E0((s32)idBuf, 0x17);
        idBuf[0] = idsB.id[pipe];
        func_151494E0((s32)idBuf, 0x17);
    }
    func_150B3F5C(&argsD, &origin, 20);
    func_10010F88(0xE7, 0x7FFF, 0, 2, 0, (s16)D_8009F920[pipe].pos.x, (s16)D_8009F920[pipe].pos.y, (s16)D_8009F920[pipe].pos.z, 1000, 3000);
    func_10010F88(0x22, 0x7FFF, 0, 0, 0, (s16)D_8009F920[pipe].pos.x, (s16)D_8009F920[pipe].pos.y, (s16)D_8009F920[pipe].pos.z, 1000, 3000);
}

/* Spawn request passed to func_15132A4C (same layout as game_111670.c's Struct150E5558). */
typedef struct PipeChunkSpawn {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex unk40;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ s16 kind;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[6];
} PipeChunkSpawn;

/* The chunk object func_15132A4C creates; func_150B36AC is its floor-hit callback. */
typedef struct PipeChunk {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ f32 unk10;
    /* 0x014 */ f32 restitution;
    /* 0x018 */ u8 pad18[0x3C - 0x18];
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ u8 pad40[0x44 - 0x40];
    /* 0x044 */ vertex vel;
    /* 0x050 */ vertex unk50;
    /* 0x05C */ u8 pad5C[0x60 - 0x5C];
    /* 0x060 */ s32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0x170 - 0x66];
    /* 0x170 */ f32 minBounceVelY; // a hit with vel.y above this stops the chunk
} PipeChunk;

extern PipeChunk *func_15132A4C(PipeChunkSpawn *, s32, s32, s32, u8, s32);

/* Spawns one pipe chunk at (x, y, z), flung out along yaw `arg8` with a random speed. */
s32 func_150B2EB4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 yawCos;
    f32 yawSin;
    s16 angle;
    f32 headingCos;
    f32 headingSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;
    PipeChunkSpawn spawn;

    yawCos = func_151423D8(arg8 - 0x40);
    yawSin = func_151423D8(arg8);

    spawn.flags = 0x9E8;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.unk04 = 0.200000003f;

    speed = (func_150ADA68() * 31.0f) + 18.0f;
    angle = (arg9 - 0x40) + (s16)(60.0f * yawSin * argA * argC);
    headingCos = func_151423D8(angle - 0x40);
    headingSin = func_151423D8(angle);
    angle = (s16)(48.0f * yawCos) - 20;
    pitchCos = func_151423D8(angle - 0x40);
    pitchSin = func_151423D8(angle);

    spawn.vel.x = (speed * pitchSin) * headingCos;
    spawn.vel.y = -speed * pitchCos;
    spawn.vel.z = (speed * pitchSin) * headingSin;

    spawn.unk40.x = (func_150ADA68() * 80.0f) + -40.0f;
    spawn.unk40.y = 0.0f;
    spawn.unk40.z = (func_150ADA68() * 80.0f) + -40.0f;
    spawn.lifetime = 100;

    spawn.gravity = (func_150ADA68() * -1.752000093f) + -0.8370000124f;
    spawn.unk00 = 1.0f;

    pitchSin = (func_150ADA68() * 72.5f) + 29.80000114f; // matching: reused as the size
    spawn.kind = 0x1C;
    spawn.unk58 = 0;
    spawn.unk08 = pitchSin;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 1;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 0;
    spawn.unk6A = 2;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    spawn.unk0C = pitchSin;

    func_15132A4C(&spawn, 3, 0xFF, 0, argE, 0);
    return 1;
}

void *memcpy(void *, const void *, u32);

/*
 * Tunable variant of func_150B2EB4: the ranges come from debug sliders (func_151422DC), and the chunk
 * bounces until it lands slowly.
 */
s32 func_150B3188(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, f32 argA, s32 argB,
                  f32 argC, s32 argD, u8 argE) {
    f32 yawCos;
    f32 yawSin;
    s16 angle;
    f32 headingCos;
    f32 headingSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;
    s16 life;
    f32 minGrav;
    f32 gravRange;
    f32 minSpeed;
    f32 speedRange;
    f32 minScale;
    f32 scaleRange;
    f32 xzAngle;
    f32 yzAngle;
    PipeChunkSpawn spawn;
    PipeChunk *chunk;
    f32 minBounceVelY;

    yawCos = func_151423D8(arg8 - 0x40);
    yawSin = func_151423D8(arg8);

    life = func_151422DC(6, "life", 1, 500, 300, "../Effects/PipeExplode/pipeexplode.c", 0x742);
    minGrav = func_151422DC(7, "min grav", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x750) * 0.001000000047f;
    gravRange = func_151422DC(8, "grav range", -5000, 0, 0, "../Effects/PipeExplode/pipeexplode.c", 0x752) * 0.001000000047f;
    minSpeed = func_151422DC(9, "min speed", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75A);
    speedRange = func_151422DC(10, "speeed range", 0, 500, 0, "../Effects/PipeExplode/pipeexplode.c", 0x75C);
    minScale = func_151422DC(11, "min scale", 0, 5000, 1000, "../Effects/PipeExplode/pipeexplode.c", 0x761) * 0.1000000015f;
    scaleRange = func_151422DC(12, "scale  range", 0, 5000, 0, "../Effects/PipeExplode/pipeexplode.c", 0x763) * 0.1000000015f;
    xzAngle = func_151422DC(13, "xz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x769);
    yzAngle = func_151422DC(14, "yz angle", -255, 255, 0, "../Effects/PipeExplode/pipeexplode.c", 0x76F);

    spawn.flags = 0x29E9;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    minBounceVelY = -18.0f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.unk04 = 0.200000003f;

    speed = (func_150ADA68() * speedRange) + minSpeed;
    angle = (arg9 - 0x40) + (s16)(xzAngle * yawSin * argA * argC);
    headingCos = func_151423D8(angle - 0x40);
    headingSin = func_151423D8(angle);
    angle = (s16)(yzAngle * yawCos) - 20;
    pitchCos = func_151423D8(angle - 0x40);
    pitchSin = func_151423D8(angle);

    spawn.vel.x = (speed * pitchSin) * headingCos;
    spawn.vel.y = -speed * pitchCos;
    spawn.vel.z = (speed * pitchSin) * headingSin;

    spawn.unk40.x = (func_150ADA68() * 4.0f) + -2.0f;
    spawn.unk40.y = 0.0f;
    spawn.unk40.z = (func_150ADA68() * 4.0f) + -2.0f;
    spawn.lifetime = life + 32;

    spawn.gravity = (func_150ADA68() * gravRange) + minGrav;

    pitchSin = (func_150ADA68() * scaleRange) + minScale; // matching: reused as the size
    spawn.unk08 = pitchSin;
    spawn.kind = 0x1C;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 1;
    spawn.unk00 = pitchSin * 0.1000000015f;
    spawn.unk62 = 0;
    spawn.unk63 = 5;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 0;
    spawn.unk6A = 2;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 32;
    spawn.unk74 = 7;
    spawn.unk0C = pitchSin;

    chunk = func_15132A4C(&spawn, 3, 0xFF, 4, argE, 0);
    if (chunk != NULL) {
        memcpy(&chunk->minBounceVelY, &minBounceVelY, sizeof(f32));
    }
    return 1;
}

/* Floor-hit callback for a pipe chunk: bounce it, or stop it once it lands slowly enough. */
s32 func_150B36AC(PipeChunk *chunk, s32 arg1, s32 arg2, s32 arg3, f32 arg4) {
    chunk->unk3C = chunk->unk10 + arg4;
    if (chunk->minBounceVelY < chunk->vel.y) {
        chunk->flags &= ~0x7;
        chunk->flags &= ~0x8;
        chunk->flags &= ~0x40;
        chunk->flags &= ~0x20;
        chunk->vel.x = 0.0f;
        chunk->vel.y = 0.0f;
        chunk->vel.z = 0.0f;
        if (chunk->lifetime > 32) {
            chunk->lifetime = 32;
        }
    } else {
        chunk->vel.x *= chunk->restitution;
        chunk->vel.y = -chunk->vel.y * chunk->restitution;
        chunk->vel.z *= chunk->restitution;
        chunk->unk50.x *= chunk->restitution;
        chunk->unk50.y *= chunk->restitution;
        chunk->unk50.z *= chunk->restitution;
    }
    return 1;
}

s32 func_150B378C(void *a0) {
    u8 *p;
    s32 v1;
    p = *(u8 **)((u8 *)a0 + 0x98);
    v1 = *(s16 *)((u8 *)a0 + 0x1C) << 4;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    if (v1 < p[0x1B]) {
        p[0x1B] = v1;
    }
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 unk0C;
    f32 unk10;
} Pt_150B37C8;

typedef struct {
    f32 unk00;
    f32 unk04;
    u8 pad08[4];
    f32 unk0C;
    f32 unk10;
    u8 pad14[5];
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
    u8 unk20;
} State_150B37C8;

typedef struct {
    u8 pad00[1];
    u8 unk01;
    u8 pad02[0xA];
    u8 unk0C;
    u8 pad0D[0x18];
    u8 unk25;
    u8 pad26[7];
    s8 unk2D;
    u8 pad2E[0x66];
    Pt_150B37C8 *unk94;
    State_150B37C8 *unk98;
} Src_150B37C8;

typedef struct {
    s32 unk00;
    s16 unk04;
    s8 unk06;
    u8 pad07;
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    s32 unk18;
    u8 pad1C[0xC];
    Pt_150B37C8 *unk28;
} Params_150B37C8;

typedef struct {
    s16 v[3];
} S16x3_150B37C8;

extern S16x3_150B37C8 D_80088700;

s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14);

s32 func_150B37C8(Src_150B37C8 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
    State_150B37C8 *state;
    Params_150B37C8 tmp;
    f32 scale;
    S16x3_150B37C8 tbl;
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; s32 pad18; } pos;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; u8 pad14[4]; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s32 pad1C; } motion;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } style;

    {
    s32 rand2;
    s32 rand1;
    s32 rand0;
    state = arg0->unk98;
    tmp.unk28 = arg0->unk94;
    tbl = D_80088700;
    state->unk20 = 4;
    scale = func_150ADA68() * 50.0f + 50.0f;
    tmp.unk06 = *(tbl.v + func_150ADA20() % 3U);
    tmp.unk08 = 0;
    tmp.unk0C = 0;
    tmp.unk00 = 0x11;
    tmp.unk04 = 0x96;
    tmp.unk10 = state->unk1B;
    tmp.unk11 = state->unk1A;
    tmp.unk12 = 0;
    tmp.unk13 = 0;
    tmp.unk14 = 0;
    tmp.unk15 = 0xFF;
    tmp.unk18 = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&tmp, 1, 0, arg5,
                  tmp.unk28[arg0->unk2D].x,
                  arg4,
                  tmp.unk28[arg0->unk2D].z,
                  scale, scale,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0,
                  arg0->unk0C, arg0->unk01);

    }
    style.unk00 = 0;
    style.unk04 = 1;
    style.unk08 = 0x160600;
    style.unk0C = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    pos.unk10 = 1;
    pos.unk0E = 1;
    motion.unk19 = state->unk19;
    motion.unk1A = state->unk1A;
    motion.unk18 = 8;
    motion.unk1B = state->unk1B;
    pos.x = tmp.unk28[arg0->unk2D].x;
    pos.y = arg4;
    pos.z = tmp.unk28[arg0->unk2D].z;
    pos.unk15 = arg0->unk25;
    pos.unk0C = 0x3C;
    motion.unk00 = state->unk00 * 0.75f;
    motion.unk10 = state->unk10;
    motion.unk04 = state->unk04;
    motion.unk08 = func_150ADA68() * 15.0f + 15.0f;
    motion.unk0C = state->unk0C;
    func_15147DA0(&pos, &motion, 0, 1, 7, 0, 0, 0, 0, 0, 0, (s32 *)&style, 0, arg0->unk0C, arg0->unk01);
    return 1;
}
