#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A07B4; // matching: 401.0f; as a literal IDO allocates the FP temps in a different order
extern f32 D_800A07C4;
extern f32 D_800A07C8;
extern f32 D_800A07CC;
extern f32 D_800A07D0;
extern f32 D_800A07D4;
extern f32 D_800A07D8;
extern f32 D_800A07DC;
extern f32 D_800A07E0;

void func_15143794(s32, s32, f32, f32 *);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

/* Ground contact found by func_15046C80 below a probe point (as in game_173D40.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

void func_1504715C(GroundHit *hit, struct127 *actor);
s32 func_15046C80(vertex *probe, s32 arg1, f32 bottom, GroundHit *hit);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);

/* Spawns two effects on the ground below the actor (if any is found within 3000 units). */
void func_150CDBB0(struct127 *actor, u8 owner, s32 arg2) {
    vertex pos;

    if (actor == NULL) {
        return;
    }
    {
        GroundHit hit;

        func_1504715C(&hit, actor);
        pos.x = actor->x_position;
        pos.y = actor->y_position + 1000.0f;
        pos.z = actor->z_position;
        if (!func_15046C80(&pos, 0, actor->y_position - 3000.0f, &hit)) {
            return;
        }
        pos.y = hit.height;
    }
    func_1514C678(pos.x, pos.y, pos.z, 251.0f, 0, 0xFF, (func_150ADA20() & 0xF) + 35, 0x17, 0, 0.0f, 0, owner);
    func_1514C678(pos.x, pos.y, pos.z, 290.0f, 0, 0xFF, (func_150ADA20() % 21U) + 30, 0x18, 0, 0.0f, 0, owner);
}

/* Spawn parameters of a func_15130374 particle (same layout as ParticleDesc in game_15D730.c). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode;
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;
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
    /* 0x1D */ u8 texture;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ s32 flags;
    /* 0x5C */ void *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;
    /* 0x63 */ s8 deathFunc;
    /* 0x64 */ s8 drawFunc;
    /* 0x65 */ u8 handlerSet;
    /* 0x66 */ u8 views;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 minSize;
    /* 0x6A */ s16 unk6A;
    /* 0x6C */ f32 maxSize;
} ParticleDesc; /* size 0x70 */

/* A particle object (same layout as Particle in game_15D730.c); its kind's own data follows at +0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0x28];
    /* 0xA8 */ f32 unkA8;
} Particle;

#define PARTICLE_FLIP_S 0x40
#define PARTICLE_FLIP_T 0x80

Particle *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);

/* Spawns a random purple-to-yellow puff particle at (x, y, z), flying off at a random pitch along `yaw`. */
s32 func_150CDCF4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    Particle *particle;
    ParticleDesc desc;
    f32 size;
    f32 unkA8;
    f32 speed;
    u32 random;

    desc.texture = 41;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = (func_150ADA20() % 41U) + 50;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 176;
    desc.g2 = 160;
    desc.b2 = 42;
    desc.r = 64;
    desc.g = 11;
    desc.b = 106;
    desc.a = 255;
    desc.alpha = (func_150ADA20() % 101U) + 100;
    desc.brightness = 255;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    size = (func_150ADA68() * D_800A07B4) + 404.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    desc.width = size;
    desc.height = size;
    random = func_150ADA20();
    speed = func_150ADA68();
    func_15143794(yaw, (s16)((random % 20U) - 19), ((speed * 150.0f) + 150.0f) * 0.1f, &desc.vel.x);
    desc.flags = 0xE05;
    desc.gravity = 0.0f;
    if (func_150ADA20() & 1) {
        desc.flags |= PARTICLE_FLIP_S;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= PARTICLE_FLIP_T;
    }
    desc.updateFunc = 10;
    desc.deathFunc = -1;
    desc.fadeStart = 30;
    desc.fadeRate = 8;
    desc.scaleStart = 70;
    desc.scaleRate = 1.0196f;
    unkA8 = 0.95091f;
    particle = func_15130374(&desc, 1, sizeof(f32), arg14, 1);
    if (particle != NULL) {
        memcpy(&particle->unkA8, &unkA8, sizeof(f32));
    }
    return 1;
}

s32 func_150CDF10(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    struct {
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
            u8 pad76[0xA];
    } sp28;

    sp28.unk28 = arg2;
    sp28.unk2C = arg3;
    sp28.unk1C = 1.0f;
    sp28.unk20 = 1.0f;
    sp28.unk24 = 1.0f;
    sp28.unk50 = 0x29E8;
    sp28.unk10 = 0.0f;
    sp28.unk14 = 0.0f;
    sp28.unk18 = 0.0f;
    sp28.unk44 = 0.0f;
    sp28.unk00 = 1.0f;
    sp28.unk56 = 0x20;
    sp28.unk30 = arg4;
    sp28.unk04 = D_800A07C4;

    ((s32 *)&sp28)[-2] = func_150ADA20();
    func_15143794(arg8, (s16)((((s32 *)&sp28)[-2] % 0x29U) - 0x3D),
                  (func_150ADA68() * 101.0f + 150.0f) * D_800A07C8,
                  &sp28.unk34);

    sp28.unk40 = (func_150ADA68() * 260.0f + -130.0f) * D_800A07CC;
    sp28.unk48 = (func_150ADA68() * 260.0f + -130.0f) * D_800A07D0;
    sp28.unk54 = (func_150ADA20() % 0x15U) + 0x32;
    sp28.unk4C = (func_150ADA68() * D_800A07D4 + D_800A07D8) * D_800A07DC;

    sp28.unk0C = (func_150ADA68() * 302.0f + 52.0f) * D_800A07E0;
    sp28.unk08 = sp28.unk0C;
    sp28.unk58 = 0;
    sp28.unk5C = 0;

    sp28.unk60 = (func_150ADA20() % 0x4CU) + 0xB4;
    sp28.unk61 = 8;
    sp28.unk62 = 0;
    sp28.unk63 = 0;
    sp28.unk64 = 0;
    sp28.unk65 = 0;
    sp28.unk66 = 0;
    sp28.unk67 = 0;
    sp28.unk68 = 0;
    sp28.unk6A = 2;
    sp28.unk6C = 0;
    sp28.unk70 = 0;
    sp28.unk72 = 0x20;
    sp28.unk74 = 7;

    func_15132A4C(&sp28, 3, 0xFF, 0, arg14, 0);
    return 1;
}
