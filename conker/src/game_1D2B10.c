#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn region an emitter scatters its particles over. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; // degrees
    /* 0x14 */ u8 state;
    /* 0x15 */ u8 flags; // bits 0-1: region shape
} EmitterRegion;

/* Particle emitter state, embedded at offset 0x28 of the effect object. */
typedef struct {
    /* 0x00 */ EmitterRegion *region;
    /* 0x04 */ f32 rate;
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 pending; // particles owed; one is spawned per whole unit
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 xScaleBase;
    /* 0x18 */ f32 xScaleRange;
    /* 0x1C */ f32 yScaleBase;
    /* 0x20 */ f32 yScaleRange;
    /* 0x24 */ s16 unk24Base;
    /* 0x26 */ s16 unk24Range;
    /* 0x28 */ f32 unk38Base;
    /* 0x2C */ f32 unk38Range;
    /* 0x30 */ f32 unk40Base;
    /* 0x34 */ f32 unk40Range;
    /* 0x38 */ f32 unk44Base;
    /* 0x3C */ f32 unk44Range;
    /* 0x40 */ u8 useGround;
    /* 0x41 */ u8 unk2CBase;
    /* 0x42 */ u8 unk2CRange;
    /* 0x43 */ u8 pad43[5];
    /* 0x48 */ CollisionHit ground;
    /* 0x6C */ s16 *unk6C;
} RegionEmitterState;


/*
 * Per-frame update of a particle emitter effect: accumulates rate * scale * D_800BE9A4 and spawns one
 * particle per whole unit at a random point of the emitter's region, optionally snapped to the ground.
 */
void func_151A5660(u8 *effect) {
    RegionEmitterState *emitter;
    ParticleEmitterDesc particle;
    f32 low;
    f32 high;
    f32 xScale;
    f32 yScale;

    emitter = (RegionEmitterState *)(effect + 0x28);
    if (emitter->region == NULL) {
        *(s16 *)(effect + 0xE) = -1;
        return;
    }
    if (emitter->region->state == 1) {
        return;
    }
    if (emitter->unk6C != 0 && func_151464B8(emitter->unk6C) != 0) {
        return;
    }
    emitter->pending += emitter->scale * emitter->rate * D_800BE9A4;
    if (emitter->pending > 1.0f) {
        particle.maxC = 0x6231;
        particle.minC = 0x1A4D;
        particle.color[0] = 0;
        particle.color[1] = 0;
        particle.color[2] = 0;
        particle.color[3] = 0xFF;
        particle.flags = 1;
        particle.unk2D = 0xFF;
        particle.extraSize = 0;
        particle.unk2E = 0;
        particle.unk2F = 0;
        particle.effect = 0x10;
        particle.maxHeight = emitter->unk10;
        do {
            switch (emitter->region->flags & 3) {
            case 2: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;
                f32 z;

                angle = emitter->region->rotation * 0.7111111f;
                cosine = func_151423D8(angle - 0x40);
                sine = func_151423D8(angle);
                x = func_150ADA68() * (2.0f * emitter->region->radiusX) + -emitter->region->radiusX;
                z = func_150ADA68() * (2.0f * emitter->region->radiusZ) + -emitter->region->radiusZ;
                particle.pos.x = emitter->region->x + (x * sine + z * cosine);
                particle.pos.z = emitter->region->z + (z * sine - x * cosine);
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
            } break;
            case 0: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;

                angle = func_150ADA20() & 0xFF;
                cosine = func_151423D8(angle - 0x40);
                sine = func_151423D8(angle);
                x = func_150ADA68() * emitter->region->radiusX;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y;
            } break;
            case 1: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;

                angle = func_150ADA20() & 0xFF;
                cosine = func_151423D8(angle - 0x40);
                sine = func_151423D8(angle);
                x = func_150ADA68() * emitter->region->radiusX;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
            } break;
            default:
                particle.pos.x = emitter->region->x;
                particle.pos.z = emitter->region->z;
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
                break;
            }
            if (emitter->useGround != 0) {
                f32 probe[3];

                probe[0] = particle.pos.x;
                probe[1] = high;
                probe[2] = particle.pos.z;
                if (func_15046C80(probe, 0, low, &emitter->ground) != 0) {
                    particle.pos.y = emitter->ground.height;
                } else {
                    particle.pos.y = low;
                }
            } else {
                particle.pos.y = low;
            }
            xScale = func_150ADA68() * emitter->xScaleRange + emitter->xScaleBase;
            yScale = func_150ADA68() * emitter->yScaleRange + emitter->yScaleBase;
            particle.maxA = 22.6f * xScale;
            particle.minA = 13.786f * xScale;
            particle.maxB = 57.969f * yScale;
            particle.minB = 42.714f * yScale;
            particle.altB = 118.763f * yScale;
            particle.life = func_150ADA20() % (emitter->unk24Range + 1) + emitter->unk24Base;
            particle.scale = func_150ADA68() * emitter->unk38Range + emitter->unk38Base;
            particle.shrinkRate = func_150ADA68() * emitter->unk40Range + emitter->unk40Base;
            particle.growRate = func_150ADA68() * emitter->unk44Range + emitter->unk44Base;
            particle.unk2C = func_150ADA20() % (emitter->unk2CRange + 1) + emitter->unk2CBase;
            func_15149550(&particle, 10, 0, 0, effect[0xC], effect[1]);
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}

/* Effect state embedded at offset 0x58 of the TimedEffect passed to the two functions below. */
typedef struct {
    u8 pad0[0x14];
    /* 0x14 */ u16 soundHandle;
} EffectSoundState;

/* Releases the effect's sound (if any), then hands the effect to func_1514933C. */
void func_151A5CAC(TimedEffect *arg0) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)arg0 + 0x58);

    // matching: the test reads through arg0 and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)arg0 + 0x6C) != 0) {
        func_151A5D2C(state->soundHandle);
    }
    func_1514933C(arg0);
}

/* Releases the effect's sound (if any), then hands the effect to func_15149368. */
void func_151A5CEC(TimedEffect *arg0) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)arg0 + 0x58);

    // matching: the test reads through arg0 and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)arg0 + 0x6C) != 0) {
        func_151A5D2C(state->soundHandle);
    }
    func_15149368(arg0);
}

/* Releases a sound handle. */
void func_151A5D2C(u16 soundHandle) {
    func_100111C8(soundHandle);
}

/*
 * Spawns an effect via func_1513C650 at pos, scaled by size on both axes, living duration + 20 frames.
 * Returns NULL if duration <= 0, so the duration == -1 cases below are unreachable (the original has them).
 */
QuadSprite *func_151A5D58(f32 size, u8 arg1, CollisionTri *surface, Vec3f *pos, s16 duration, u8 arg5, u8 arg6,
                          u8 arg7, s32 arg8) {
    SpriteDef2C params;

    if (duration <= 0) {
        return NULL;
    }
    params.fadeStart = 20;
    params.fadeRate = 12;
    params.texture = 0x38;
    params.unk7 = 0;
    params.flags = ((duration == -1) ? 0 : 1) | (arg5 ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x2000 | 0x8000 | 0x20000;
    if (duration == -1) {
        params.lifetime = 300;
    } else {
        params.lifetime = duration + 20;
    }
    params.frame = 0;
    if (duration == -1) {
        params.frameSpeed = 0;
    } else {
        params.frameSpeed = 0x140000 / (duration + 20);
    }
    params.unk10 = arg1;
    params.unk11 = 0xFF;
    params.rgba[0] = 0xFF;
    params.rgba[1] = 0x83;
    params.rgba[2] = 30;
    params.rgba[3] = 0xFF;
    params.renderModes = (arg6 ? 2 : 3) + 0x440000;
    params.unk16 = 0;
    params.unk17 = 6;
    params.viewportMask = 0xFF;
    return func_1513C650(&params, 0, 0, surface, pos->x, pos->y, pos->z, size, size,
                         func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2),
                         arg5 ? 3 : 0, arg5 ? 0xFF : 0, 0, arg7, arg8);
}
