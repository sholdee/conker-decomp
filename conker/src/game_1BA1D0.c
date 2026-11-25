#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* ParticleDesc.flags */
#define PARTICLE_FLIP_S 0x40
#define PARTICLE_FLIP_T 0x80

/* Spawn parameters of a particle; func_15130280 copies them to Particle.desc (see game_15D730.c). */
typedef struct ParticleDesc {
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
    /* 0x30 */ struct17 pos;
    /* 0x3C */ struct17 offset;
    /* 0x48 */ struct17 vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;
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

typedef struct Particle {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0x28];
    /* 0xA8 */ f32 drag; /* per-particle data, read by the drag update funcs */
} Particle;

extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
extern void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z);
extern void func_15145DB4(void *placement, struct17 *in, struct17 *out, s32 count);

/* Spawns 25-35 particles on a ring around (111, -90, 0) in placement space, flying outward from its center. */
void func_1518CD20(void *placement, u8 arg1, s32 arg2) {
    s16 count;
    ParticleDesc spawn;
    s16 remaining;
    struct17 *points;
    s16 i;
    f32 drag;
    struct17 *ring;

    count = remaining = func_150ADA20() % 11U + 25;
    if (count > 0) {
        ring = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        points = (struct17 *)func_10003C6C((count + 1) * sizeof(struct17), 3, 1, 0, 1);
        if (ring == NULL || points == NULL) {
            return;
        }
        {
            for (i = 0; i < count; i++) {
                func_1514373C(func_150ADA68() * 3.141592741f + 3.141592741f, 114.0f, &ring[i].unk0, &ring[i].unk8);
                ring[i].unk0 += 111.0f;
                ring[i].unk4 = -90.0f;
            }
            i = 0;
            ring[count].unk0 = 111.0f;
            ring[count].unk4 = -90.0f;
            ring[count].unk8 = 0.0f;
            func_15145DB4(placement, ring, points, count + 1);
            spawn.texture = 0x6C;
            spawn.renderMode = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.r = 0xE7;
            spawn.g = 0xE7;
            spawn.b = 0xB6;
            spawn.r2 = 0x9E;
            spawn.g2 = 0x9E;
            spawn.b2 = 0x62;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.a = 0xFF;
            spawn.brightness = 0xFF;
            spawn.offset.unk0 = 0.0f;
            spawn.offset.unk4 = 0.0f;
            spawn.offset.unk8 = 0.0f;
            spawn.fadeStart = 25;
            spawn.fadeRate = 10;
            spawn.flags = 0x80CE07;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = 16;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.scaleRate = 1.040568948f;
            drag = 0.8533809781f;
            do {
                f32 speed;
                s32 flipT;
                Particle *particle;

                spawn.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
                flipT = (func_150ADA20() & 1) ? PARTICLE_FLIP_T : 0;
                spawn.flags |= ((func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0) | flipT;
                speed = (func_150ADA68() * 202.0f + 250.0f) * 0.001000000047f;
                spawn.scaleStart = spawn.timer = func_150ADA20() % 26U + 25;
                spawn.width = spawn.height = func_150ADA68() * 105.0f + 150.0f;
                spawn.pos = points[i];
                spawn.vel.unk0 = (points[i].unk0 - points[count].unk0) * speed;
                spawn.vel.unk4 = (points[i].unk4 - points[count].unk4) * speed;
                spawn.vel.unk8 = (points[i].unk8 - points[count].unk8) * speed;
                spawn.gravity = (func_150ADA68() * 880.0f + 13.0f) * 0.001000000047f;
                spawn.alpha = func_150ADA20() % 176U + 80;
                particle = func_15130280(&spawn, 1, 0, sizeof(particle->drag), arg1, arg2);
                if (particle != NULL) {
                    memcpy(&particle->drag, &drag, sizeof(drag));
                }
                remaining--;
                i++;
            } while (remaining > 0);
        }
    }
}
