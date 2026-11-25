#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15142314(struct255 *bones, s32 bone, vertex *out);

/* Gets the position of the actor's bone 0x18 (side 1) or 0x15 (side 2). */
s32 func_150C19C0(vertex *pos, struct127 *actor, u8 side) {
    s32 bone;

    switch (side) {
    case 1:
        bone = 0x18;
        break;
    case 2:
        bone = 0x15;
        break;
    }

    func_15142314(actor->unk1D4, bone, pos);
    return 1;
}

s32 func_150C1A2C(s32 arg0, s32 arg1) {
    return 0x7;
}

typedef struct {
    char pad0[0x3B];
    u8 unk3B;
    char pad3C[0x3A];
    u16 unk76;
    char pad78[0xD4];
    f32 unk14C;
    f32 unk150;
    char pad154[0x2C];
    f32 unk180;
    char pad184[0x50];
    s32 unk1D4;
} Actor150C1A40;

typedef struct {
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
    u16 unk56;
    u8 unk58;
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
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 pad78;
} Particle150C1A40;

extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_1514C2F0(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                   u8 arg4, s8 arg5, s16 arg6, u8 arg7,
                   s32 arg8, f32 arg9, s32 arg10, u8 arg11);
void func_15165F80(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_150C1A40(Actor150C1A40 *arg0, s32 arg1, s32 arg2) {
    struct { f32 x, y, z; } pos;
    f32 size;
    Particle150C1A40 p;
    f32 radius;
    u8 angle;
    f32 speed;
    f32 c;
    f32 s;
    void *v;
    s32 n;
    u8 yaw;

    if (arg0->unk1D4 != 0) {
        size = (arg0->unk14C + arg0->unk150) * 0.5f;
        func_150C19C0((vertex *)&pos, (struct127 *)arg0, arg1);
        yaw = (arg0->unk76 >> 8) + 0x40;
        radius = size * 70.0f;
        func_1514C2F0(pos.x, arg0->unk180, pos.z, radius, yaw,
                      (arg1 == 1) ? 0x10 : -0x10, 8, 0, 0, radius * 4.039999962f, 0, 0xFF);
        func_15165F80(-1, (s32)pos.x, (s32)((s32)arg0->unk180 + 2.0f), (s32)pos.z, 8, 0x15, 0, 0xFF, 0);
        p.unk56 = 5;
        p.unk50 = 0x29E9;
        p.unk58 = 0;
        p.unk5C = 0;
        p.unk60 = 0xFF;
        p.unk61 = 1;
        p.unk62 = 0;
        p.unk63 = 0;
        p.unk64 = 0;
        p.unk65 = 0;
        p.unk66 = 0;
        p.unk67 = 0;
        p.unk68 = 0;
        p.unk6A = 1;
        p.unk6C = arg0;
        p.unk1C = 1.0f;
        p.unk20 = 1.0f;
        p.unk24 = 1.0f;
        p.unk04 = 0.400000006f;
        p.unk48 = 0.0f;
        p.unk4C = -0.8999999762f;
        p.unk70 = arg0->unk3B;
        p.unk72 = 0x10;
        p.unk74 = 0xF;
        n = (func_150ADA20() & 7) + 5;
        if (n != 0) {
            n--;
            do {
            angle = func_150ADA20();
            angle &= 0xFF;
            speed = (0.1000000015f + func_150ADA68() * 0.8999999762f) * 9.0f * size;
            c = func_151423D8(angle - 0x40);
            s = func_151423D8(angle);
            p.unk54 = (func_150ADA20() & 0xF) + 0x1E;
            p.unk28 = radius * c + pos.x;
            p.unk2C = arg0->unk180 + 2.0f;
            p.unk30 = radius * s + pos.z;
            p.unk34 = speed * c;
            p.unk38 = speed + speed;
            p.unk3C = speed * s;
            p.unk0C = p.unk08 = p.unk00 = (0.1000000015f + func_150ADA68() * 0.8999999762f) * size * 1.5f;
            p.unk10 = func_150ADA68() * 360.0f;
            p.unk14 = func_150ADA68() * 360.0f;
            p.unk18 = func_150ADA68() * 360.0f;
            p.unk40 = 25.0f - func_150ADA68() * 50.0f;
            p.unk44 = 25.0f - func_150ADA68() * 50.0f;
            v = func_15132A4C(&p, 3, 0xFF, 4, 0xFF, 0);
            if (v != NULL) {
                *(f32 *)((u8 *)v + 0x170) = arg0->unk180;
            }
            } while (n--);
        }
    }
}

/* Effect description passed to func_1518CA80 (same layout as ParticleEffect in game_1B9F30.c). */
typedef struct {
    /* 0x00 */ f32 x;            /* position as floats (mode 1) */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ s16 intX;         /* position as integer + 1/256ths (other modes) */
    /* 0x0E */ s16 intY;
    /* 0x10 */ s16 intZ;
    /* 0x12 */ u8 subX;
    /* 0x13 */ u8 subZ;
    /* 0x14 */ u8 subY;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 width;
    /* 0x18 */ s16 height;
    /* 0x1A */ s16 widthStep;    /* added to the particle's width every frame */
    /* 0x1C */ s16 heightStep;   /* added to the particle's height every frame */
    /* 0x1E */ s16 velX;
    /* 0x20 */ s16 velZ;
    /* 0x22 */ s16 velY;
    /* 0x24 */ s16 accelY;
    /* 0x26 */ s16 life;
    /* 0x28 */ u8 primColor[3];
    /* 0x2B */ u8 envColor[3];
    /* 0x2E */ u8 intensity;     /* both colours are scaled by intensity / 256 */
    /* 0x2F */ u8 alpha;
    /* 0x30 */ s8 fadeStep;
    /* 0x31 */ u8 onExpire;      /* low nibble: index into D_8008D5D0, run when life reaches 0 */
} ParticleEffect;

void func_151429E0(u8 palette, u8 *r, u8 *g, u8 *b);
void func_1518CA80(ParticleEffect *effect, u8 mode);

/*
 * Spawns a growing billboard particle of size `size` at (x, y, z), moving horizontally away from
 * (fromX, fromZ), with random colours from palette 0.
 */
s32 func_150C1E34(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, f32 fromX, f32 fromY, f32 fromZ,
                  s32 arg8, s32 arg9, s32 argA, s32 argB, f32 size) {
    ParticleEffect effect;

    effect.x = x;
    effect.y = y;
    effect.z = z;
    effect.velX = (x - fromX) * 10.0f;
    effect.velZ = (z - fromZ) * 10.0f;
    effect.width = effect.height = size;
    effect.heightStep = effect.widthStep = size * 0.1757;
    effect.velY = 150;
    effect.accelY = 20;
    effect.life = 600;
    func_151429E0(0, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
    func_151429E0(0, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
    effect.intensity = 255;
    effect.alpha = 255;
    effect.fadeStep = (func_150ADA20() & 15) + 12;
    effect.onExpire = 1;
    func_1518CA80(&effect, 1);
    return 1;
}
