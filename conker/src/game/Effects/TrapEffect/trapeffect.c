#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

struct EffectSpawnSource {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ EffectSpawnPoint *points;
    /* 0x98 */ EffectSpawnState *state;
};

/* Timed effect spawned by a trap trigger (effect_onTrapTriggered). */
struct TrapEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ u8 kind;      /* effect_createFallingStream selector */
};

/* Trap trigger callback: spawns a timed effect (timer 100) whose kind follows the trap's unique_id (0xFB-0xFE). */
void effect_onTrapTriggered(WorldObject *trap) {
    TrapEffect *obj = effect_newTimedEffectNoDraw(100, 2, -1, 1, 0, 1, 0xFF, 0);

    if (obj != NULL) {
        switch (trap->unique_id) {
        case 0xFB:
            obj->kind = 1;
            break;
        case 0xFC:
            obj->kind = 2;
            break;
        case 0xFD:
            obj->kind = 4;
            break;
        case 0xFE:
            obj->kind = 3;
            break;
        default:
            effect_destroyObject(&obj->base.header);
            break;
        }
    }
}

/* Runs effect_createFallingStream for the object's kind. */
s32 effect_startTrapStream(TrapEffect *obj) {
    return effect_createFallingStream(obj->kind);
}

/* Spawns a sprite (texture 0x1D or 0x1E, random size 10-80) at the source's current point, at the given height. */
s32 func_150DDFAC(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
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

    particle_spawnSurfaceQuadSpriteNoModel(&def, 5, 0, surface, points[source->head.start].x, height, points[source->head.start].z, size, size,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0, source->head.header.owner, source->head.header.kind);
    return 1;
}

/* Spawns a sprite (texture 0x1B or 0x1C, random size 10-20) at the source's current point, at the given height. */
s32 func_150DE12C(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
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

    particle_spawnSurfaceQuadSpriteNoModel(&def, 5, 0, surface, points[source->head.start].x, height, points[source->head.start].z, size, size,
                  rand0 & 0xFF, (rand2 & 1) + (rand1 & 1), 0, source->head.header.owner, source->head.header.kind);
    return 1;
}

/* Sprite update callback 5: fades unk10 out over the sprite's last 32 frames. */
s32 func_150DE2A4(QuadSprite *sprite) {
    s16 lifetime = sprite->def.lifetime;

    if (lifetime < 32) {
        sprite->def.unk10 = lifetime * 8;
    }
    return 1;
}

/* Particle callback 12: shrinks the particle and kills it once it is under 10 units wide or high. */
void effect_shrinkBillboard(BillboardParticle *particle) {
    particle->tmpl.width -= 26;
    particle->tmpl.height -= 26;
    if (particle->tmpl.width < 10 || particle->tmpl.height < 10) {
        particle->tmpl.life = 0;
    }
}
