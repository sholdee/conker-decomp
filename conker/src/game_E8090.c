#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

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
} DecalDesc_150BABE0;

/* matching: the joint owner is passed as s32; a pointer parameter changes register allocation */
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);
void func_1504715C(Ground_150BABE0 *, Actor *);
s32 func_15046C80(Vec3f *, s32, f32, Ground_150BABE0 *);
s32 func_1518ABD0(s32, struct210 *, u8);

/*
 * Spawns the effects for one of obj's joints (side 0: joint 0x11, side 1: joint 0x1A): a sound,
 * two particle bursts and, when the ground below is found, a ground decal.
 */
// matching: each `*(u8 *)&arg2` reloads arg2 from its stack slot; plain or K&R arg2 is kept in a register
void func_150BABE0(Actor *obj, u8 side, u8 arg2) {
    Vec3f pos;
    s32 joint;
    s32 yaw;
    Ground_150BABE0 ground;
    Vec3f probe;
    DecalDesc_150BABE0 desc;
    struct210 *decal;
    f32 size;

    /* the current camera's unk380 angle, degrees -> 256-step angle */
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
    pos.y = obj->floorY;
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
        desc.unk6 = 0x2A;
        desc.unk7 = 0;
        desc.unk0 = 0x7340;
        desc.unk4 = 300;
        desc.unk8 = 0;
        desc.unkC = func_150ADA20() % 2001U + 2000;
        desc.unk10 = 0xFF;
        desc.unk11 = 0xFF;
        desc.unk12 = 0;
        desc.unk13 = 0;
        desc.unk14 = 0;
        desc.unk15 = 0xFF;
        desc.unk16 = 0;
        desc.unk17 = 0xD;
        desc.unk20 = 0xFF;
        desc.unk1C = 0;
        desc.unk18 = 0x170002;
        decal = func_1513C73C((s32)&desc, 0, 0, (s32)ground.points, pos.x, ground.height, pos.z, size, size,
                              func_150ADA20() & 0xFF, (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), 4, *(u8 *)&arg2, 0);
        if (decal != NULL && func_1518ABD0(D_80088750, decal, 1) == 0) {
            func_1516972C((struct102 *)decal);
        }
    }
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 40.0f + 60.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 7U + 6, 6, 0, 0.0f, 0, *(u8 *)&arg2);
}

/* matching: the first 0x68 bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc68 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f offset;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
} ParticleDesc68;

/* Live particle returned by func_15130374; its per-kind extra data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 flags;
    /* 0x1C */ s16 life;
    /* 0x1E */ u8 pad1E[0xA];
    /* 0x28 */ u8 alpha;
    /* 0x29 */ u8 pad29[0x2F];
    /* 0x58 */ Vec3f velocity;
    /* 0x64 */ u8 pad64[0x1D];
    /* 0x81 */ u8 unk81;
    /* 0x82 */ u8 pad82[0x26];
    /* 0xA8 */ f32 damping;   /* extra data of the func_150BAFEC particles */
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ s16 fadeStart; /* extra data of the func_150BB408 particles */
    /* 0xB2 */ s16 fadeStep;
} Particle_150BAFEC;

extern Particle_150BAFEC *func_15130374(ParticleDesc68 *, u8, s32, u8, s32);

/* Spawns one particle at (x, y, z) flying off along `yaw` at a random pitch; it slows down by 0.95 per frame. */
s32 func_150BAFEC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 speed;
    Particle_150BAFEC *particle;
    ParticleDesc68 desc;
    s32 pad; // matching: unused stack slot
    f32 damping;
    s16 pitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;

    cosYaw = func_151423D8(yaw - 0x40);
    sinYaw = func_151423D8(yaw);
    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = func_150ADA20() % 36U + 70;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0xB;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = func_150ADA20() % 52U + 90;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    desc.width = desc.height = func_150ADA68() * 165.0f + 800.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    pitch = func_150ADA20() % 11U - 12;
    cosPitch = func_151423D8(pitch - 0x40);
    sinPitch = func_151423D8(pitch);
    speed = func_150ADA68() * 11.0f + 15.0f;
    desc.flags = 0xE05;
    desc.vel.x = speed * sinPitch * cosYaw;
    desc.vel.y = -speed * cosPitch;
    desc.vel.z = speed * sinPitch * sinYaw;
    desc.gravity = 0.0f;
    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }
    desc.updateFunc = 2;
    desc.deathFunc = -1;
    desc.fadeStart = 22;
    desc.fadeRate = 11;
    desc.scaleStart = 1;
    desc.scaleRate = 1.02f;
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
    f32 *damping; // matching: reading particle->damping directly changes the loop code

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

extern s32 func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);

/* Spawns one piece at (x, y, z) thrown along `yaw` at a random pitch, with random spin and size. */
s32 func_150BB498(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw,
                  s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    s16 pitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 speed;
    ParticleSpawn piece;
    f32 scale;

    cosYaw = func_151423D8(yaw - 0x40);
    sinYaw = func_151423D8(yaw);

    piece.flags = 0x9E8;
    piece.rot.x = 0.0f;
    piece.rot.y = 0.0f;
    piece.rot.z = 0.0f;
    piece.scale.x = 1.0f;
    piece.scale.y = 1.0f;
    piece.scale.z = 1.0f;
    piece.pos.x = x;
    piece.pos.y = y;
    piece.pos.z = z;
    piece.elasticity = 0.2f;

    pitch = func_150ADA20() % 13U - 52;
    cosPitch = func_151423D8(pitch - 0x40);
    sinPitch = func_151423D8(pitch);

    speed = func_150ADA68() * 9.0f + 3.0f;
    piece.vel.x = speed * sinPitch * cosYaw;
    piece.vel.y = -speed * cosPitch;
    piece.vel.z = speed * sinPitch * sinYaw;

    piece.rotVel.x = func_150ADA68() * 50.0f + -25.0f;
    piece.rotVel.y = 0.0f;
    piece.rotVel.z = func_150ADA68() * 50.0f + -25.0f;
    piece.lifetime = func_150ADA20() % 68U + 38;

    piece.gravity = func_150ADA68() * -0.1f + -0.3f;
    piece.radius = 1.0f;

    scale = func_150ADA68() * 18.80000114f + 5.6f;
    piece.model = 31;
    piece.variant = 0;
    piece.unk5C = 0;
    piece.sizeXZ = scale;
    piece.alpha = 0xFF;
    piece.updateFn = 1;
    piece.update2Fn = 0;
    piece.floorHitFn = 0;
    piece.floorType3Fn = 0;
    piece.wallHitFn = 0;
    piece.destroyFn = 0;
    piece.squashFn = 0;
    piece.matrixFn = 0;
    piece.unk6A = 0;
    piece.owner = 0;
    piece.ownerId = 0;
    piece.fadeStart = 1;
    piece.fadeRate = 0xFF;
    piece.sizeY = scale;

    func_15132A4C(&piece, 3, 0xFF, 0, arg14, 0);
    return 1;
}
