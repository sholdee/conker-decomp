#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_151D9014(f32 *pos, f32 *vel, s32 arg2, f32 arg3, s32 arg4, s32 arg5,
                   f32 arg6, s32 arg7, f32 arg8, f32 arg9, s32 arg10, s32 arg11,
                   s32 arg12, s32 arg13, s32 arg14, s32 arg15);
void func_151B9BF0(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                   s32 arg12, s32 arg13, s32 arg14, s32 arg15);
void func_151EF040(s32 arg0, s32 *arg1);
void func_15171200(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5,
                   f32 arg6, f32 arg7, f32 arg8, s32 arg9, s32 arg10, f32 arg11,
                   s32 arg12, s32 arg13);
void func_151709B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5,
                   s32 arg6);
void func_1516D99C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                   s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16,
                   s32 arg17, s32 arg18, s32 arg19, s32 arg20, s32 arg21,
                   s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26,
                   s32 arg27, s32 arg28, s32 arg29, s32 arg30, s32 arg31,
                   s32 arg32, s32 arg33, s32 arg34, s32 arg35, s32 arg36,
                   s32 arg37, s32 arg38, s32 arg39, s32 arg40, s32 arg41,
                   s32 arg42, s32 arg43, s32 arg44, s32 arg45, s32 arg46,
                   s32 arg47, s32 arg48);
struct225 *func_151602C0(Header *header, Header2 *header2, s32 arg2, s32 arg3,
                         s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 offset,
                         s32 arg9, s32 argA);
extern s32 func_10004074[];

// matching: one struct pins the stack layout of these locals (checksum at sp+0xFC)
typedef struct {
    Header2 pos;
    Header header;
    u8 pad12[0xA];
    s32 checksum;
} Struct150C3D5CLocals;

void func_15151A38(PuffSpray *, u8, s32);
void func_15150178(AngleRange *angles, DropSpray *spray, s32 arg2, u8 arg3, s32 arg4);

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
    params.render.envMode = 0;
    params.render.primMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, arg0, 1);
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
    func_15151A38(&params, arg0, 1);
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
    func_15150178(&params2.angles, &params2.spray, 0, arg0, 1);
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
    params.render.envMode = 0;
    params.render.primMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, arg0, 1);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ u8 pad34[4];
    /* 0x038 */ f32 unk38;
    /* 0x03C */ u8 pad3C[0x10];
    /* 0x04C */ f32 unk4C;
    /* 0x050 */ f32 unk50;
    /* 0x054 */ u8 pad54[0x44];
    /* 0x098 */ struct133 *unk98;
    /* 0x09C */ u8 pad9C[0xB4];
    /* 0x150 */ f32 unk150;
    /* 0x154 */ u8 pad154[0xC];
    /* 0x160 */ f32 unk160;
} Particle_EF410;

void func_1514C470(f32, f32, f32, f32, f32, f32, f32, s32, s32, f32, s32, u8);

void func_150C2424(u8 arg0) {
    func_1514C470(8500.0f, -490.0f, -328.0f, 8500.0f, -490.0f, 328.0f,
                  (func_150ADA68() * 8.0f) + 8.0f, 1, 0, 0.0f, 0, arg0);
    func_1514C470(8843.0f, -560.0f, -580.0f, 8117.0f, -560.0f, -580.0f,
                  (func_150ADA68() * 3.0f) + 4.0f, 3, 0, 0.0f, 0, arg0);
}

s32 func_150C251C(Particle_EF410 *particle) {
    struct133 *sub = particle->unk98;
    s32 alpha = particle->unk1C << 3;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    sub->unk1B = alpha;
    if (sub->unk1B < 0) { // never true for a u8, but the compiled code has this compare
        return 0;
    }
    return 1;
}


s32 func_150C2558(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    PuffSpray params;

    params.pos.x = x;
    params.pos.y = y;
    params.pos.z = (z + 15.0f) - (func_150ADA68() * 30.0f);
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
    params.render.envMode = 0;
    params.render.primMode = 7;
    params.unk80 = 0xC;
    params.unk82 = 0x15;
    func_15151A38(&params, argE, 1);
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
    func_15150178(&params.angles, &params.spray, 0, argE, 1);
    return 1;
}

typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 delay;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 unk19;
} Struct150C2804;

extern void func_15134908(Struct150C2804 *, s32, u8, s32);

void func_150C2804(f32 *x, f32 *y, f32 *z, s16 delay, s32 arg4, s32 arg5) {
    Struct150C2804 params;

    params.unk0C = 10.3f;
    params.x = x;
    params.y = y;
    params.z = z;
    params.unk10 = 0.5009f;
    params.delay = delay;
    params.unk16 = 5;
    params.unk17 = 6;
    params.unk18 = 3;
    params.unk19 = -1;
    func_15134908(&params, 0, arg4, arg5);
}
typedef struct {
    f32 x;
    f32 z;
    f32 scale;
    f32 y;
} PosSub;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    PosSub unk28;
} Struct150C29F0;

void func_150C2898(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter) {
    f32 pos[3];
    f32 vel[3];
    f32 speed;
    f32 rand3;
    u32 rand2;
    u32 rand1;
    f32 rand0;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    speed = (func_150ADA68() * 112.0f + 247.0f) * 0.001f;
    vel[0] = -dirX * speed;
    vel[1] = -dirY * speed;
    vel[2] = -dirZ * speed;
    rand0 = func_150ADA68();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    rand3 = func_150ADA68();
    func_151D9014(pos, vel, 6, rand0 * 0.27f + -0.31f,
                    (rand1 & 0xF) + 25, (rand2 % 101) + 155,
                    rand3 * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    emitter->unkC, emitter->unk1);
}
typedef struct {
    Vec3f unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg_EF410;

Particle_EF410 *func_15149550(Struct15149550Arg_EF410 *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);

void func_150C29F0(Struct150C29F0 *emitter) {
    PosSub *pos;
    Struct15149550Arg_EF410 particle;
    f32 scaleA;
    f32 scaleB;
    u8 angle;
    f32 cosine;
    f32 sine;
    f32 radius;

    angle = func_150ADA20() & 0xFF;
    cosine = func_151423D8(angle - 0x40);
    sine = func_151423D8(angle);
    pos = &emitter->unk28;
    radius = func_150ADA68() * pos->scale;
    particle.unk20 = 0x6231; particle.unk22 = 0x1A4D; // matching: one source line (-g3 line numbers steer as1 scheduling)
    particle.unk26 = 0;
    particle.unk27 = 0;
    particle.unk28 = 0;
    particle.unk29 = 0xFF;
    particle.unk2C = 0xFF;
    particle.unk2D = 0xFF;
    particle.unk30 = 0;
    particle.unk2A = 1;
    particle.unk2E = 0;
    particle.unk2F = 1;
    particle.unk24 = (func_150ADA20() % 201U) + 100;
    particle.unk34 = 0xF;
    particle.unk38 = 0.0f;
    particle.unk40 = 0.045f;
    particle.unk44 = 0.7f;
    particle.unk0.x = pos->x + radius * cosine;
    particle.unk0.y = pos->y;
    particle.unk0.z = pos->z + radius * sine;
    particle.unk3C = 10000.0f;
    scaleA = (func_150ADA68() * 0.5f) + 1.0f;
    scaleB = (func_150ADA68() * 0.5f) + 1.0f;
    particle.unkC = 22.6f * scaleA;
    particle.unk14 = 13.786f * scaleA;
    particle.unk10 = 57.969f * scaleB;
    particle.unk18 = 42.714f * scaleB;
    particle.unk1C = 118.763f * scaleB;
    func_15149550(&particle, 0xA, 1, 0, emitter->unkC, emitter->unk1);
    emitter->unkE = (func_150ADA20() % 51U) + 25;
}
void func_1514373C(f32 arg0, f32 arg1, f32 *arg2, f32 *arg3);
Vec3f *func_15144B34(s32);
void *memcpy(void *, const void *, u32);

void func_150C2C00(Struct150C29F0 *emitter) {
    f32 *timer;
    f32 *pos;
    f32 distSq;
    Struct15149550Arg_EF410 particle;
    f32 scaleA;
    f32 angle;
    f32 scaleB;
    Particle_EF410 *created;

    timer = &emitter->unk28.x;
    *timer += func_150ADA68() * 20.0f * 1e-9f * D_800BE9A4 * 36447764.0f;
    if (*timer > 1.0f) {
        if (1) { // matching: the extra block makes IDO give emitter $s3 and pos $s2
            pos = (f32 *)func_15144B34(D_80082FA4);
            distSq = (pos[2] * pos[2]) + (pos[0] * pos[0]);
            if (2500000.0f < distSq) {
                particle.unk20 = 0x6231;
                particle.unk22 = 0x1A4D;
                particle.unk0.y = -390.0f;
                particle.unk26 = 0;
                particle.unk27 = 0;
                particle.unk28 = 0;
                particle.unk29 = 0xFF;
                particle.unk2D = 0xFF;
                particle.unk2E = 20;
                particle.unk2F = 1;
                particle.unk30 = 4;
                particle.unk2A = 1;
                particle.unk34 = 25;
                particle.unk3C = 0.0f;
                particle.unk40 = 0.045f;
                particle.unk44 = 0.7f;
                do {
                    angle = (func_150ADA68() * 5.8659425f) + 0.957204f;
                    if (func_15144C8C(func_150484A0(pos[0], pos[2]), angle) < 0.4f) {
                        func_1514373C(angle, (func_150ADA68() * 1278.0f) + 3900.0f, &particle.unk0.x, &particle.unk0.z);
                        scaleA = ((func_150ADA68() * 1991.0f) + 1985.0f) * 0.001f;
                        scaleB = ((func_150ADA68() * 1493.0f) + 3000.0f) * 0.001f;
                        particle.unkC = 22.6f * scaleA;
                        particle.unk14 = 13.786f * scaleA;
                        particle.unk10 = 57.969f * scaleB;
                        particle.unk18 = 42.714f * scaleB;
                        particle.unk1C = 118.763f * scaleB;
                        particle.unk24 = (func_150ADA20() % 151U) + 50;
                        particle.unk38 = (func_150ADA68() * 500.0f) * 0.001f;
                        particle.unk2C = (func_150ADA20() % 156U) + 100;
                        created = func_15149550(&particle, 0xA, 1, 0, emitter->unkC, emitter->unk1);
                        if (created != NULL) {
                            memcpy(&created->unk160, &angle, sizeof(angle));
                        }
                    }
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            } else {
                do {
                    *timer -= 1.0f;
                } while (*timer > 1.0f);
            }
        }
    }
}
s32 func_150C2FCC(Particle_EF410 *particle) {
    particle->unk2C -= particle->unk2C * particle->unk150;
    particle->unk30 -= particle->unk30 * particle->unk150;
    particle->unk38 += (particle->unk50 * D_800BE9A4) + (0.5f * particle->unk4C * D_800BE9A4 * D_800BE9A4);
    particle->unk50 += particle->unk4C * D_800BE9A4;
    if ((particle->unk2C < 10.0f) || (particle->unk30 < 10.0f)) {
        return 0;
    }
    return 1;
}
s32 func_150C308C(Particle_EF410 *particle) {
    u8 triggered;
    f32 value; // matching: one local for the squared distance and then the angle (IDO puts both in $f12)
    Vec3f *pos;

    triggered = FALSE;
    if (particle->unk1C >= 6) {
        pos = func_15144B34(D_80082FA4);
        value = pos->x * pos->x + pos->z * pos->z;
        if (value < 2500000.0f) {
            triggered = TRUE;
        } else {
            value = func_150484A0(pos->x, pos->z);
            if (0.4f < func_15144C8C(value, particle->unk160)) {
                triggered = TRUE;
            }
        }
    }
    if (triggered) {
        particle->unk1C = 5;
    }
    return 1;
}

Gfx *func_150C3160(Gfx *gdl, Actor *obj) {
    s32 total;
    s32 uls;
    s32 ult;
    f32 progress;

    total = obj->unk2E8;
    progress = (total != 0) ? ((f32)obj->unk2E4 / (f32)total) : 1.0f;
    progress = 1.0f - progress;
    uls = (s32)((500.0f * progress) + 2.0f);
    ult = 2 - obj->unk2EC;
    obj->unk2EC = uls / 3;
    while (ult < 0) {
        ult += 64;
    }
    gDPSetTileSize(gdl++, 4, uls, ult, 0x1FE, 0x03E);
    return gdl;
}
typedef struct {
    Actor *unk0;
    f32 pos[3];
    s32 targetIndex;
    s32 unk14;
} Struct150C3230;

void func_15059C84(Actor *arg0);
void func_1505A770(Actor *arg0);
void func_1507C324(Actor *arg0, Actor *arg1);
void func_1506AC8C(Actor *arg0, s32 arg1, void *arg2);

void func_150C3230(Actor *obj) {
    s32 index;
    s32 type;
    Actor *target;
    Struct150C3230 eventData;

    obj->unkF8 |= 0x4000000;
    if ((obj->unk222 != 0) && (obj->unk65 == 0)) {
        obj->old_x_position = D_800CC2D0[obj->unk124 - 1].x_position;
        obj->old_y_position = D_800CC2D0[obj->unk124 - 1].y_position;
        obj->old_z_position = D_800CC2D0[obj->unk124 - 1].z_position;
        obj->unk222 = 0;
    }

    func_15059C84(obj);
    func_1505A770(obj);
    func_15058898(obj, obj->old_y_position);
    func_1505B5F8(obj, obj->floorY);

    if (obj->unk28 == 0.0f) {
        obj->xz_velocity *= 0.5f;
    }

    index = obj->unk65;
    if (index != 0) {
        obj->unk222 = index;
        if (D_800CC2D0[index - 1].unk84.uh == 0x1F9) {
            type = 2;
        } else if (D_800CC2D0[index - 1].unk84.uh == 0x1FB) {
            type = 3;
        } else {
            typedef s32 Type150C3230; // matching: a declaration here gives -g3 a block marker; as1 then fills the delay slot from this branch
            type = 1;
        }
    } else {
        type = 0;
    }

    func_1505E650(obj, type, 0.0f, 0.0f, 0.0f, 0.0f, 0);

    if (obj->unk65 != 0) {
        func_1507C324(obj, &D_800CC2D0[obj->unk65 - 1]);
    }

    if (obj->unk2E4 != 0) {
        if (obj->unk65 != 0) {
            target = &D_800CC2D0[obj->unk65 - 1];
            if (target->interaction_state == 5) {
                obj->unk65 = 0;
                obj->unk2E4 = 0;
            }
        } else {
            target = NULL;
        }

        if ((D_800BE616 == 0) && (D_800C35EA[0] == 0) && (obj->unk65 == 0)) {
            obj->unk2E4 = 0;
        }

        if (D_800BE9E4 < obj->unk2E4) {
            obj->unk2E4 -= D_800BE9E4;
            if (D_800BE616 != 0) {
                func_1508B20C(obj->x_position, obj->y_position, obj->z_position, 900.0f);
            }
        } else {
            eventData.pos[0] = obj->x_position;
            eventData.pos[1] = obj->y_position;
            eventData.pos[2] = obj->z_position;
            eventData.unk0 = &D_800CC2D0[obj->unk124 - 1];
            eventData.targetIndex = obj->unk65;
            if (target != NULL) {
                target->immune = target->stunned = 0;
                if (target->health != 0) {
                    target->health = 1;
                }
            }
            func_1506AC8C(obj, (D_800BE616 != 0) ? 0xB : 0xE, &eventData);
            func_15060F28(obj, 1);
        }
    }
}
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);
f32 func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_15056B08(Actor *arg0);
void func_150562FC(Actor *arg0);
u8 func_150599C8(Actor *arg0, u8 arg1, u16 arg2);
void func_1001091C(Actor *arg0, s32 arg1);

void func_150C3574(Actor *obj) {
    f32 accel;
    f32 dist;
    s32 vol;
    f32 scale;
    u16 angle;

    accel = 0.25f;
    scale = 0.5f;
    obj->unk80 = 10;
    if (obj->id == 0x25) {
        func_1505A250(0.0f, 0.0f, 0.15f, &obj->unk164, &obj->unk168);
        accel = 0.1f;
        if (obj->unk87 & 1) {
            obj->unk224 = (s32)obj->y_position;
        }
        obj->unk160 = 0;
    }
    func_1505A3A8(0.0f, obj, 2.0f, accel, 0);
    obj->gravity = 3.0f;
    if (obj->id == 0x10) {
        scale = 0.375f;
    }
    if (obj->id == 0xAC) {
        scale = 1.5f;
    }
    scale *= 0.75f;
    func_15056B08(obj);
    if (obj->unk13D != 0) {
        func_150562FC(obj);
    }
    func_15059140(obj);
    obj->unkBC = (obj->x_position - obj->old_x_position) * D_800BE9A8;
    obj->unk148 = (obj->z_position - obj->old_z_position) * D_800BE9A8;
    dist = sqrtf(obj->unkBC * obj->unkBC + obj->unk148 * obj->unk148);
    if ((dist + 4.0f) + (dist + 4.0f) < obj->xz_velocity) {
        obj->xz_velocity = dist + dist;
    }
    if (obj->unk87 == 4) {
        D_800D1580 = 0xFF020133;
        D_800D154C = D_800CC2D0;
        func_1506E8D8();
        D_800D154C = obj;
        obj->unk164 = obj->unkBC;
        obj->unk168 = obj->unk148;
        obj->xz_velocity = 0.0f;
    }
    obj->unk87 *= 2;
    if ((dist > 0.5f) && (obj->unk13D == 0)) {
        obj->unk78 = func_1505A630(obj->x_position - obj->old_x_position, obj->old_z_position - obj->z_position, 0);
    }
    if (((obj->unk7A - obj->unk78) - 0x4000) & 0x8000) {
        angle = obj->unk78;
    } else {
        angle = obj->unk78 ^ 0x8000;
    }
    func_150599C8(obj, 2, angle);
    if (obj->unk28 > 30.0f) {
        obj->unk44 *= 0.9f;
    } else {
        obj->unk44 = dist + dist;
    }
    if (obj->unk13D != 0) {
        obj->unk44 = 0.0f;
    }
    func_15052490(obj, obj->unk78, obj->unk44, (2.0f - obj->xz_scale) * scale);
    if (obj->unk44 < 0.2f) {
        func_100109D0(obj);
    } else if (func_100107F8(obj) == 0) {
        if (obj->id == 0xAC) {
            D_800D1580 = 0x630;
        } else if (obj->id == 0x25) {
            D_800D1580 = 0x67A;
        } else {
            D_800D1580 = 0x62A;
        }
        func_1506E5FC();
    } else {
        vol = obj->unk44 * 2500.0f;
        if (vol > 0x6D60) {
            vol = 0x6D60;
        }
        if (obj->id == 0x25) {
            vol >>= 1;
        }
        func_1001091C(obj, vol);
    }
    if (obj->unk44 < 0.2f) {
        func_100109D0(obj);
    }
}
void func_15188810(Actor *arg0, s32 arg1, s32 arg2);
void func_150C1660(f32 arg0, f32 arg1, f32 arg2, u8 arg3);
void func_150C3D5C(Actor *obj);

void func_150C3994(Actor *obj) {
    if (obj->interaction_state == 0x14) {
        if (obj->unk2E4 == 0) {
            func_151B9BF0(0x2F, 1, (s16)obj->x_position, (s16)(obj->y_position + 30.0f), (s32)obj->z_position, 0x7D0, 0x3E8, 0x190, 0xC8, 0xC00, 0xC00, 1, 1, 0, 0xFF, 0);
        } else {
            func_15188810(obj, 0x64, 0);
        }
    }
    if (obj->y_velocity < 0.0f) {
        if ((obj->unk28 > 20.0f) && (obj->unk136 == 0) && (obj->unk28 < (-20.0f * obj->y_velocity))) {
            func_10010630(0x92, obj, 0x7D00, 0xC8, 0x9C4);
            obj->unk136 = 1;
        }
    }
    if (obj->unk83 == 0) {
        obj->unk148 = 70.0f;
        if (obj->unk2E4 == 0) {
            func_151B9BF0(0x2F, 1, (s16)obj->x_position, (s16)(obj->y_position + 30.0f), (s32)obj->z_position, 0x898, 0x500, 0x320, 0x2BC, 0xC00, 0xC00, 0x14, 1, 0, 0xFF, 0);
            func_150C1660(obj->x_position, obj->y_position + 30.0f, obj->z_position, 0xFF);
            func_1506AC8C(obj, 6, NULL);
        } else {
            func_15188810(obj, 0x64, 0);
        }
        func_10010344(0x1A9, obj, 0x7FFF, 0x3E8, 0x7D0);
        obj->unkD0 = 1;
        obj->unk114 = 500.0f;
        obj->unkF8 |= 0x01008000;
    }
    obj->immune = 0xFF;
    obj->unk83++;
    if (obj->unk83 >= 0xB) {
        obj->unkF8 &= ~0x8000;
    }
    obj->unk40 = (f32)(obj->unk7A + 0x4000) * 0.005493164f; // 360 / 65536: binary angle to degrees
    func_15059140(obj);
    obj->unkB8 += obj->unk148;
    obj->unk148 *= 0.96f;
    if (((obj->unk28 == 0.0f) && (obj->unk83 >= 0x15) && (obj->unkCC == 0)) || (obj->unk107 != 0) || (obj->health == 0)) {
        func_150C3D5C(obj);
        func_10010630(0x93, obj, 0x7D00, 0xC8, 0x9C4);
        func_15060F28(obj, 1);
    }
}

s32 func_150C3D48(s32 value) {
    return value + 0xEDCBA988;
}

void func_150C3D5C(Actor *obj) {
    Struct150C3D5CLocals locals;

    func_151B9BF0(0x2F, 1, (s16)obj->x_position, (s16)obj->y_position,
                  (s32)obj->z_position, 0x578, 0x500, 0x320, 0x4B0, 0x200, 0x200,
                  0x1E, 1, 0, 0xFF, 0);

    locals.checksum = 0xC23459B8;
    // matching: calling through a function-pointer cast makes IDO emit jalr
    ((void (*)(s32, s32 *))func_151EF040)(func_150C3D48(locals.checksum), &locals.checksum);
    if (locals.checksum != 0x0330C820) {
        // tamper check failed: overwrite func_10004074 with "jr $ra; nop"
        func_10004074[0] = 0x03E00008;
        func_10004074[1] = 0;
    }

    func_15171200(3, 0xCB, obj->x_position, obj->y_position, obj->z_position, 10.0f, 1.0f,
                  2.0f, 60.0f, 3, 0x3C, -0.7f, 0xFF, 0);
    func_151709B4((s32)obj->x_position, (s32)(obj->y_position + 20.0f),
                  (s32)obj->z_position, 0xA, 2000.0f, 0xFF, 0);
    func_1516D99C((s16)obj->x_position, (s16)obj->y_position,
                  (s16)obj->z_position, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
                  0, 0xFF, 0, 0, 0, 0x14, 0x14, 2, 0, 0, 0, 0, 0x51, 0x51,
                  0x51, 0x51, 0xA, 1, 0, 0xFF, 0xA, 0x578, 0x190, 1, 1, 0, 1,
                  0, 0, 0, 0, 0, 0xFF, 0);

    locals.header.unk0 = 3;
    locals.header.unk1 = -1;
    locals.header.unk2 = (func_150ADA20() % 11U) + 0xA;
    locals.header.unk4 = 0;
    locals.pos.unk0 = (s32)obj->x_position;
    locals.pos.unk4 = (s32)obj->y_position;
    locals.pos.unk8 = (s32)obj->z_position;
    func_151602C0(&locals.header, &locals.pos, (func_150ADA20() % 52U) + 0x28,
                  0xFF, 0xFF, 0x5A, 0xFF, 0, 0, 0xFF, 1);
}
