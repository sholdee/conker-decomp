#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_150DBD70(u8 kind);

/* Spawn template of a sprite (as declared in game_169510.c). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 frame;          /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 unk10;           /* faded out with the lifetime */
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 rgba[4];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 renderModes;    /* low byte: D_800A4AC8 index, high half: combiner index */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 viewportMask;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 fadeStart;
    /* 0x24 */ s16 fadeRate;
    /* 0x26 */ u8 pad26[2];
} SpriteDef; /* size 0x28 */

/* Sprite effect created by func_1513C350 (see game_169510.c); only the template is used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ SpriteDef def;
} SpriteFx;

/* Billboard particle (see game_1944C0.c); only the fields used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
} Particle;

/* The trap object func_1511BEBC passes to its trigger callback. */
typedef struct {
    /* 0x00 */ u8 pad0[0x72];
    /* 0x72 */ u8 tag;
} TrapTrigger;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ u8 unk20;
} EffectSpawnState;

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
} EffectSpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 padD[0x20];
    /* 0x2D */ s8 pointIndex;
    /* 0x2E */ u8 pad2E[0x66];
    /* 0x94 */ EffectSpawnPoint *points;
    /* 0x98 */ EffectSpawnState *state;
} EffectSpawnSource;

/* Trap trigger callback: spawns a type 100 object whose kind follows the trap's tag (0xFB-0xFE). */
void func_150DDED0(TrapTrigger *trap) {
    struct102 *obj = (struct102 *)func_151491F4(100, 2, -1, 1, 0, 1, 0xFF, 0);

    if (obj != NULL) {
        switch (trap->tag) {
        case 0xFB:
            obj->unk28 = 1;
            break;
        case 0xFC:
            obj->unk28 = 2;
            break;
        case 0xFD:
            obj->unk28 = 4;
            break;
        case 0xFE:
            obj->unk28 = 3;
            break;
        default:
            func_1516972C(obj);
            break;
        }
    }
}

/* Runs func_150DBD70 for the object's kind. */
s32 func_150DDF88(struct102 *obj) {
    return func_150DBD70(obj->unk28);
}

/* Spawns a sprite (texture 0x1D or 0x1E, random size 10-80) at the source's current point, at height y. */
s32 func_150DDFAC(EffectSpawnSource *source, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5) {
    EffectSpawnState *state;
    EffectSpawnPoint *points;
    SpriteDef def;
    f32 size;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    state = source->state;
    points = source->points;
    size = func_150ADA68() * 70.0f + 10.0f;
    state->unk20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        def.texture = 0x1D;
        break;
    case 1:
        def.texture = 0x1E;
        break;
    }

    def.frame = 0;
    def.frameSpeed = 0;
    def.flags = 0x11;
    def.lifetime = 100;
    def.unk10 = 0xFF;
    def.unk11 = 0xFF;
    def.rgba[0] = 0;
    def.rgba[1] = 0;
    def.rgba[2] = 0;
    def.rgba[3] = 0xFF;
    def.renderModes = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&def, 5, 0, arg5, points[source->pointIndex].x, y, points[source->pointIndex].z, size, size,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0, source->unk0C, source->unk01);
    return 1;
}

/* Spawns a sprite (texture 0x1B or 0x1C, random size 10-20) at the source's current point, at height y. */
s32 func_150DE12C(EffectSpawnSource *source, s32 arg1, s32 arg2, s32 arg3, f32 y, s32 arg5) {
    EffectSpawnState *state;
    EffectSpawnPoint *points;
    SpriteDef def;
    f32 size;
    s32 rand2;
    s32 rand1;
    s32 rand0;

    state = source->state;
    points = source->points;
    size = func_150ADA68() * 10.0f + 10.0f;
    state->unk20 = 4;

    switch (func_150ADA20() & 1) {
    case 0:
        def.texture = 0x1B;
        break;
    case 1:
        def.texture = 0x1C;
        break;
    }

    def.frame = 0;
    def.frameSpeed = 0;
    def.flags = 0x11;
    def.lifetime = 100;
    def.unk10 = 0xFF;
    def.unk11 = 0xFF;
    def.rgba[0] = 0;
    def.rgba[1] = 0;
    def.rgba[2] = 0;
    def.rgba[3] = 0xAA;
    def.renderModes = 0x30002;

    rand0 = func_150ADA20();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();

    func_1513C73C((s32)&def, 5, 0, arg5, points[source->pointIndex].x, y, points[source->pointIndex].z, size, size,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0, source->unk0C, source->unk01);
    return 1;
}

/* Sprite update callback 5: fades unk10 out over the sprite's last 32 frames. */
s32 func_150DE2A4(SpriteFx *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 32) {
        sprite->def.unk10 = lifetime * 8;
    }
    return 1;
}

/* Particle callback 12: shrinks the particle and kills it once it is under 10 units wide or high. */
void func_150DE2C4(Particle *particle) {
    particle->width -= 26;
    particle->height -= 26;
    if (particle->width < 10 || particle->height < 10) {
        particle->life = 0;
    }
}
