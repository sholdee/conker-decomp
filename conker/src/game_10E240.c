#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Data of the effect effect_spawnPropEmitter spawns and effect_updatePropEmitter updates. */
typedef struct {
    /* 0x00 */ u8 index; // 0 or 1: row of gEmitterPropIds, D_800A0FE4 and D_800A0FFC
    /* 0x04 */ WorldObject *prop;
    /* 0x08 */ ZoneNode *zone; // collision_findZoneLeaf at the prop's x/z
    /* 0x0C */ f32 accum; // particles owed; one is emitted per whole unit
} PropEmitter;

/* Timed effect of effect_updatePropEmitter. */
struct PropEmitterEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ PropEmitter emitter;
};

/* The two emitter props. */
static const u8 gEmitterPropIds[2] = { 0xF7, 0xF6 };


/* Sets camera distances when camera->CharContext is 9 (else when unk2C is 8), and flag bits in unk84
 * from script_query queries for the levels level == 0x1E, 0x1B and 0x2F. */
void func_150E0D90(Camera *camera) {
    if (camera->CharContext == 9) {
        if (level == 0x1E) {
            script_command(0, 0x4044, 1);
        }
        if ((camera->Mode != 0x100) && (camera->cmdRunning == 0)) {
            if (camera_pushMode(camera, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
                camera->AvailableModes |= 0x300000;
                camera->AvailableModes &= ~4;
                camera->DistIndex = 1;
                camera->DistMask = 3;
                camera_loadDistancePreset(camera);
            }
            camera->height = 125.0f;
            camera->unk34C = 125.0f;
            camera->IdealTripodMagnitude = 220.0f;
            camera->YOffsetTget = 30.0f;
        } else {
            camera->YOffsetTget = 0.0f;
        }
    } else {
        if ((camera->Mode == 8) && (camera->cmdRunning == 0) && (camera_popMode(camera, CAMERA_MODE_NORMAL) != 0)) {
            camera->unk34C = 110.0f;
            camera->height = 110.0f;
            camera->IdealTripodMagnitude = 300.0f;
        }
        if (level == 0x1E) {
            script_command(0, 0x4044, 0);
        }
    }
    if (level == 0x2F) {
        camera->AvailableModes &= 0x7FFFFFFF;
    } else if (level == 0x1E) {
        if (script_query(1, 0x4042, 6, 0x9000) != 0) {
            camera->AvailableModes &= 0x7FFFFFFF;
        } else {
            camera->AvailableModes |= 0x80000000;
        }
        if (script_query(1, 0x4044, 6, 0x2000) != 0) {
            camera->AvailableModes |= 0x20001010;
            camera->AvailableModes &= ~8;
        } else {
            camera->AvailableModes &= 0xDFFFEFEF;
            camera->AvailableModes |= 8;
        }
    }
    if (level == 0x1B) {
        if (script_query(1, 0x406E, 6, 0x9000) != 0) {
            camera->AvailableModes |= 0x200;
        } else {
            camera->AvailableModes &= ~0x200;
        }
        if (script_query(1, 0x406F, 6, 0x9000) != 0) {
            camera->AvailableModes |= 0x80000000;
        } else {
            camera->AvailableModes &= 0x7FFFFFFF;
        }
    }
}

/* Spawns the emitter effect for prop gEmitterPropIds[index] (index 0 or 1); returns it, or NULL. */
TimedEffect *effect_spawnPropEmitter(u8 index, u8 owner, s32 kind) {
    TimedEffect *effect;
    PropEmitter emitter;

    if (index < 0) { // matching: always false for a u8, but removing it changes the code
        return NULL;
    }

    if (index >= 2) {
        return NULL;
    }

    emitter.index = index;
    emitter.prop = gfx_findObjectById(gEmitterPropIds[index]);
    if (emitter.prop == NULL) {
        return NULL;
    }

    collision_selectMap(0);
    emitter.zone = collision_findZoneLeaf(emitter.prop->x, emitter.prop->z);
    emitter.accum = 0.0f;

    effect = effect_newTimedEffect(300, -1, 0x1F, -1, 0, 0x1F, sizeof(emitter), owner, kind);
    if (effect != NULL) {
        memcpy(&((PropEmitterEffect *)effect)->emitter, &emitter, sizeof(emitter));
    }
    return effect;
}

/* Two points per emitter index, in the prop's space (start and end of the emission direction). */
static const Vec3f D_800A0FE4[2] = { { -26.08121f, 28.21353f, 482.1082f }, { 22.96786f, 28.15965f, 437.60574f } };
static const Vec3f D_800A0FFC[2] = { { -25.56817f, 28.21346f, 536.2602f }, { 22.96786f, 80.0f, 480.60574f } };

/* Emitter effect update: accumulates a random rate and, while the emitter's zone has one of the
 * low gScreenLayout + 1 bits set, emits one particle per whole unit along D_800A0FE4 -> D_800A0FFC. */
void effect_updatePropEmitter(PropEmitterEffect *effect) {
    PropEmitter *emitter;
    s32 i;
    s16 mask;
    ParticleDesc args;
    f32 extra;
    f32 dir[3];
    const f32 *points[2];
    f32 *results[2];
    s32 flip80;
    s32 flip40;
    f32 speed;
    TimedEffect *particle;

    emitter = &effect->emitter;
    emitter->accum += (204.0f + (func_150ADA68() * 159.0f)) * 0.001f * gFrameDelta;
    if (emitter->accum > 1.0f) {
        for (i = 0, mask = 0; i <= gScreenLayout; i++) {
            mask |= 1 << i;
        }
        if (!(emitter->zone->views & mask)) {
            do {
                emitter->accum -= 1.0f;
            } while (emitter->accum > 1.0f);
            return;
        }
        {
            f32 *d; // matching: the block-scoped pointer steers regalloc for the subtraction below
            args.texture = 0x29;
            args.renderMode = 0x4403;
            args.unk0 = 0x200005;
            args.fadeStart = 0x21;
            args.fadeRate = 7;
            args.flags = 0x1CE05;
            args.primColorMode = 6;
            args.envColorMode = 8;
            args.updateFunc = 0x10;
            args.deathFunc = -1;
            args.unk4 = 0;
            args.frame = 0;
            args.frameRate = 0;
            args.gravity = 0.0f;
            args.drawFunc = -1;
            args.handlerSet = 0;
            args.scaleStart = 0x27;
            args.r = 0xDD;
            args.g = 0xD3;
            args.b = 0xCD;
            args.a = 0xFF;
            args.r2 = 0x57;
            args.g2 = 0x55;
            args.b2 = 0x5A;
            args.brightness = 0xFF;
            extra = 0.962338984f;
            args.scaleRate = 1.011461973f;
            points[0] = &D_800A0FE4[emitter->index].x;
            points[1] = &D_800A0FFC[emitter->index].x;
            results[0] = &args.pos.x;
            results[1] = dir;
            math_transformObjectPointRefs(emitter->prop, points, results, 2);
            d = dir;
            d[0] -= args.pos.x;
            d[1] -= args.pos.y;
            d[2] -= args.pos.z;
        }
        do {
            args.flags &= ~0xC0;
            flip80 = (func_150ADA20() & 1) ? 0x80 : 0;
            flip40 = (func_150ADA20() & 1) ? 0x40 : 0;
            args.flags |= flip40 | flip80;
            args.offset.x = 0.0f;
            args.offset.y = 0.0f;
            args.offset.z = 0.0f;
            args.alpha = 0xFF;
            args.timer = (func_150ADA20() % 17U) + 50;
            args.width = args.height = (func_150ADA68() * 165.0f) + 357.0f;
            speed = ((func_150ADA68() * 195.0f) + 356.0f) * 0.001f;
            args.vel.x = dir[0] * speed;
            args.vel.y = dir[1] * speed;
            args.vel.z = dir[2] * speed;
            particle = particle_newOwnedSpriteParticle(&args, 1, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
            }
            emitter->accum -= 1.0f;
        } while (emitter->accum > 1.0f);
    }
}
