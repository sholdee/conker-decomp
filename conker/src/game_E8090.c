/* functions.h declares func_150ADA20 as returning u8; the code here needs the s32 version. */
#define func_150ADA20 func_150ADA20_orig
#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#undef func_150ADA20
s32 func_150ADA20(void);

extern f32 D_8009FE7C;
extern f32 D_8009FE80;
extern f32 D_8009FE84;
extern f32 D_8009FE88;
extern f32 D_8009FE8C;
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3F_150BABE0;

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 object;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 active;
    /* 0x20 */ s32 unk20;
} Ground_150BABE0;

/* Spawn descriptor passed to func_1513C73C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21[7];
} Particle_150BABE0;

/* matching: the joint owner is passed as s32; a pointer parameter changes register allocation */
void func_15142314(s32, s32, Vec3F_150BABE0 *);
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);
void func_1504715C(Ground_150BABE0 *, struct127 *);
s32 func_15046C80(Vec3F_150BABE0 *, s32, f32, Ground_150BABE0 *);
s32 func_1518ABD0(s32, struct210 *, u8);

/*
 * Spawns the effects for one of obj's joints (side 0: joint 0x11, side 1: joint 0x1A): a sound,
 * two particle bursts and, when the ground below is found, a ground decal.
 */
// matching: each `*(u8 *)&arg2` reloads arg2 from its stack slot; plain or K&R arg2 is kept in a register
void func_150BABE0(struct127 *obj, u8 side, u8 arg2) {
    Vec3F_150BABE0 pos;
    s32 joint;
    s32 yaw;
    Ground_150BABE0 ground;
    Vec3F_150BABE0 probe;
    Particle_150BABE0 part;
    struct210 *decal;
    f32 size;

    /* camera angle in degrees -> 256-step angle */
    yaw = D_800DBFF0[D_80082FA4].unk380 * 0.7111111283f;
    if (obj == NULL) {
        return;
    }
    if (obj->interaction_state == 0 || obj->interaction_state == 8) {
        return;
    }
    if (obj->unk1D4 == NULL) {
        return;
    }
    switch (side) {
    case 0:
        joint = 0x11;
        break;
    case 1:
        joint = 0x1A;
        break;
    }
    func_15142314((s32)obj->unk1D4, joint, &pos);
    pos.y = obj->unk180;
    func_15143E94(5, 0x4022);
    func_15165F80(-1, pos.x, pos.y + 6.0f, pos.z, 25, 18, 0, *(u8 *)&arg2, 1);
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 50.0f + 40.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 11U + 30, 5, 0, 0.0f, 0, *(u8 *)&arg2);
    func_1504715C(&ground, obj);
    probe.x = pos.x;
    probe.y = pos.y + 100.0f;
    probe.z = pos.z;
    if (func_15046C80(&probe, 0, pos.y - 100.0f, &ground) != 0) {
        size = func_150ADA68() * 200.0f + 200.0f;
        part.unk6 = 0x2A;
        part.unk7 = 0;
        part.unk0 = 0x7340;
        part.unk4 = 300;
        part.unk8 = 0;
        part.unkC = func_150ADA20() % 2001U + 2000;
        part.unk10 = 0xFF;
        part.unk11 = 0xFF;
        part.unk12 = 0;
        part.unk13 = 0;
        part.unk14 = 0;
        part.unk15 = 0xFF;
        part.unk16 = 0;
        part.unk17 = 0xD;
        part.unk20 = 0xFF;
        part.unk1C = 0;
        part.unk18 = 0x170002;
        decal = func_1513C73C((s32)&part, 0, 0, (s32)ground.points, pos.x, ground.height, pos.z, size, size,
                              func_150ADA20() & 0xFF, (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), 4, *(u8 *)&arg2, 0);
        if (decal != NULL) {
            if (func_1518ABD0(D_80088750, decal, 1) == 0) {
                func_1516972C((struct102 *)decal);
            }
        }
    }
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 40.0f + 60.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 7U + 6, 6, 0, 0.0f, 0, *(u8 *)&arg2);
}

/* Particle descriptor for func_15130374 (as Struct15130374Arg in game_173D40.c). */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 lifetime;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 color0[4];
    /* 0x18 */ u8 color1[4];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 fadeDuration;
    /* 0x20 */ s16 fadeStep;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex position;
    /* 0x3C */ u8 pad3C[0xC];
    /* 0x48 */ vertex velocity;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags; /* 0x40, 0x80: random flips */
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[4];
} ParticleDesc_150BAFEC;

/* Live particle returned by func_15130374; its per-kind extra data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 flags;
    /* 0x1C */ s16 life;
    /* 0x1E */ u8 pad1E[0xA];
    /* 0x28 */ u8 alpha;
    /* 0x29 */ u8 pad29[0x2F];
    /* 0x58 */ vertex velocity;
    /* 0x64 */ u8 pad64[0x1D];
    /* 0x81 */ u8 unk81;
    /* 0x82 */ u8 pad82[0x26];
    /* 0xA8 */ f32 damping;   /* extra data of the func_150BAFEC particles */
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ s16 fadeStart; /* extra data of the func_150BB408 particles */
    /* 0xB2 */ s16 fadeStep;
} Particle_150BAFEC;

extern Particle_150BAFEC *func_15130374(ParticleDesc_150BAFEC *, u8, s32, u8, s32);

/* Spawns one particle at (x, y, z) flying off along `yaw` at a random pitch; it slows down by 0.95 per frame. */
s32 func_150BAFEC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 speed;
    Particle_150BAFEC *particle;
    ParticleDesc_150BAFEC desc;
    s32 pad; // matching: unused stack slot
    f32 damping;
    s16 pitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;

    cosYaw = func_151423D8(yaw - 0x40);
    sinYaw = func_151423D8(yaw);
    desc.unk1D = 0x29;
    desc.unk08 = 0xE03;
    desc.unk00 = 0x200005;
    desc.unk04 = 0;
    desc.lifetime = func_150ADA20() % 36U + 70;
    desc.unk0C = 0;
    desc.unk10 = 0;
    desc.color1[0] = 0xB0;
    desc.color1[1] = 0xA0;
    desc.color1[2] = 0x2A;
    desc.color0[0] = 0x40;
    desc.color0[1] = 0xB;
    desc.color0[2] = 0x6A;
    desc.color0[3] = 0xFF;
    desc.color1[3] = func_150ADA20() % 52U + 90;
    desc.unk1C = 0xFF;
    desc.unk60 = 3;
    desc.unk61 = 3;
    desc.width = desc.height = func_150ADA68() * 165.0f + 800.0f;
    desc.position.x = x;
    desc.position.y = y;
    desc.position.z = z;
    pitch = func_150ADA20() % 11U - 12;
    cosPitch = func_151423D8(pitch - 0x40);
    sinPitch = func_151423D8(pitch);
    speed = func_150ADA68() * 11.0f + 15.0f;
    desc.flags = 0xE05;
    desc.velocity.x = speed * sinPitch * cosYaw;
    desc.velocity.y = -speed * cosPitch;
    desc.velocity.z = speed * sinPitch * sinYaw;
    desc.unk54 = 0.0f;
    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }
    desc.unk62 = 2;
    desc.unk63 = -1;
    desc.fadeDuration = 22;
    desc.fadeStep = 11;
    desc.unk22 = 1;
    desc.unk24 = 1.02f;
    damping = 0.95f;
    particle = func_15130374(&desc, 0, sizeof(damping), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Per-frame update of a func_150BAFEC particle: damps its horizontal velocity once per elapsed frame. */
s32 func_150BB260(Particle_150BAFEC *particle, s32 arg1) {
    s32 i;
    f32 *damping;

    damping = &particle->damping;
    for (i = D_800BE9E4; i != 0; i--) {
        particle->velocity.x *= *damping;
        particle->velocity.z *= *damping;
    }
    return 1;
}

extern void func_1513F6C0(Particle_150BAFEC *, s32, s32);

/* Init callback: 50 frames of life, fading out over all of them at 5 alpha per frame. */
void func_150BB408(Particle_150BAFEC *particle) {
    particle->flags |= 1;
    particle->fadeStart = particle->life = 50;
    particle->fadeStep = 5;
    func_1513F6C0(particle, 6, particle->unk81);
}

/* Update callback: once life drops below fadeStart, caps alpha at life * fadeStep. */
s32 func_150BB450(Particle_150BAFEC *particle) {
    s16 alpha;

    if (particle->life < particle->fadeStart) {
        alpha = particle->life * particle->fadeStep;
        if (alpha < particle->alpha) {
            particle->alpha = alpha;
        }
    }
    return 1;
}

s32 func_150BB498(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    s16 angle;
    f32 spB8;
    f32 spB4;
    f32 spB0;
    f32 spAC;
    f32 temp_f2;
    struct {
        s32 pad00;
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        f32 unk2C;
        f32 unk30;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        f32 unk40;
        f32 unk44;
        f32 unk48;
        f32 unk4C;
        s32 unk50;
        s16 unk54;
        s16 unk56;
        u8 unk58;
        u8 pad59[3];
        s32 unk5C;
        u8 unk60;
        u8 unk61;
        u8 unk62;
        u8 unk63;
        u8 unk64;
        u8 unk65;
        u8 unk66;
        u8 unk67;
        u8 unk68;
        u8 pad69;
        u8 unk6A;
        u8 pad6B;
        s32 unk6C;
        u8 unk70;
        u8 pad71;
        s16 unk72;
        s16 unk74;
        u8 pad76[6];
    } sp2C;

    arg0 = arg8 - 0x40;
    spB8 = func_151423D8(arg0);
    spB4 = func_151423D8((u8)arg8);

    sp2C.unk50 = 0x9E8;
    sp2C.unk10 = 0.0f;
    sp2C.unk14 = 0.0f;
    sp2C.unk18 = 0.0f;
    sp2C.unk1C = 1.0f;
    sp2C.unk20 = 1.0f;
    sp2C.unk24 = 1.0f;
    sp2C.unk28 = arg2;
    sp2C.unk2C = arg3;
    sp2C.unk30 = arg4;
    sp2C.unk04 = D_8009FE7C;

    angle = (func_150ADA20() % 13U) - 0x34;
    arg0 = angle - 0x40;
    spB0 = func_151423D8(arg0);
    spAC = func_151423D8((u8)angle);

    temp_f2 = (func_150ADA68() * 9.0f) + 3.0f;
    sp2C.unk34 = (temp_f2 * spAC) * spB8;
    sp2C.unk38 = -temp_f2 * spB0;
    sp2C.unk3C = (temp_f2 * spAC) * spB4;

    sp2C.unk40 = (func_150ADA68() * 50.0f) + -25.0f;
    sp2C.unk44 = 0.0f;
    sp2C.unk48 = (func_150ADA68() * 50.0f) + -25.0f;
    sp2C.unk54 = (func_150ADA20() % 68U) + 0x26;

    sp2C.unk4C = (func_150ADA68() * D_8009FE80) + D_8009FE84;
    sp2C.unk00 = 1.0f;

    temp_f2 = (func_150ADA68() * D_8009FE88) + D_8009FE8C;
    sp2C.unk56 = 0x1F;
    sp2C.unk58 = 0;
    sp2C.unk5C = 0;
    sp2C.unk08 = temp_f2;
    sp2C.unk60 = 0xFF;
    sp2C.unk61 = 1;
    sp2C.unk62 = 0;
    sp2C.unk63 = 0;
    sp2C.unk64 = 0;
    sp2C.unk65 = 0;
    sp2C.unk66 = 0;
    sp2C.unk67 = 0;
    sp2C.unk68 = 0;
    sp2C.unk6A = 0;
    sp2C.unk6C = 0;
    sp2C.unk70 = 0;
    sp2C.unk72 = 1;
    sp2C.unk74 = 0xFF;
    sp2C.unk0C = temp_f2;

    func_15132A4C(&sp2C.unk00, 3, 0xFF, 0, arg14, 0);
    return 1;
}
