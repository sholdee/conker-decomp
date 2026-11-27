#include <ultra64.h>
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "libc/math.h"

/* Spawns a particle burst at bone 10 of `actor`. */
void particle_spawnBoneModelBurst(Actor *actor, u8 arg1, s32 arg2) {
    ModelBurst burst;
    MtxF *bones;

    if ((actor != NULL) && (actor->type != 0)) {
        bones = actor->ob_matrix_ptr;
        if (actor->ob_matrix_ptr == NULL) { // matching: the original reloads the field rather than testing bones
            return;
        }
        math_getBoneTranslation(bones, 10, &burst.position);
        burst.countBase = 8;
        burst.countRange = 7;
        burst.angles.yawBase = 0;
        burst.angles.yawRange = 255;
        burst.angles.pitchBase = -33;
        burst.angles.pitchRange = 24;
        burst.speedBase = 12.40300083f;
        burst.speedRange = 5.039000034f;
        burst.drag = 0.908870995f;
        burst.gravityBase = -0.7070000172f;
        burst.gravityRange = 0.2610000074f;
        burst.lifetimeBase = 40;
        burst.lifetimeRange = 35;
        burst.model = 0xA0;
        burst.unk38 = 0x13;
        burst.unk3C = 0;
        burst.unk40 = 0.0f;
        burst.unk44 = 1.960000038f;
        burst.scaleBase = 0.3070000112f;
        burst.scaleRange = 0.0f;
        burst.rotVelBase.x = burst.rotVelBase.z = -12.85999966f;
        burst.rotVelBase.y = 0; // matching: integer 0 (sw zero); 0.0f emits an FP-register store
        burst.rotVelRange.x = burst.rotVelRange.z = 25.37999916f;
        burst.rotVelRange.y = 0;
        burst.flags = 0x21;
        burst.color0.red = 255;
        burst.color0.green = 255;
        burst.color0.blue = 255;
        burst.color0.alpha = 255;
        burst.color1.red = 255;
        burst.color1.green = 255;
        burst.color1.blue = 255;
        burst.color1.alpha = 255;
        burst.motionFlags = 0xF;
        burst.fadeStart = 25;
        burst.fadeRate = 10;
        particle_spawnModelBurst(&burst, arg1, arg2);
    }
}

/* Creates a blood_spawnStrip object attached to `owner`. */
void particle_attachBloodStrip(Actor *owner, s32 arg1, s32 arg2) {
    BloodStripDesc desc;

    if ((owner != NULL) && (owner->type != 0)) {
        desc.ownerId = owner->unique_id;
        desc.owner = owner;
        desc.bone = 0;
        desc.offsetA.x = -28.0f;
        desc.offsetA.y = 6.0f;
        desc.offsetA.z = 1.0f;
        desc.offsetB.x = -58.0f;
        desc.offsetB.y = -3.0f;
        desc.offsetB.z = -31.0f;
        desc.flags = 2;
        desc.duration = 30;
        desc.pause = 25;
        desc.lifetime = 750;
        desc.sampleCallback = 3;
        desc.emitCallback = 3;
        desc.rateCallback = -1;
        desc.rate = 0.3115699887f;
        desc.mode = 4;
        desc.updateCallback = -1;
        blood_spawnStrip(&desc, 0);
    }
}

/* Sets the emitter's velocity scale. */
s32 particle_sampleSegmentPower(SegmentEmitter *emitter) {
    emitter->scale = 0.40200001f;
    return TRUE;
}

/*
 * Spawns one particle on the segment `from` -> `to`, at fraction `t` along it, moving with velocity
 * (to - from) * emitter->scale. Skipped while the owner is in water or moving slower than 8.1.
 */
void particle_emitOnSegment(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, SegmentEmitter *emitter) {
    EffectParams params;
    EffectMotion motion;
    const Vec3f *ownerMotion;
    s32 pad[2]; // matching: unused stack slots
    f32 x;
    f32 y;
    f32 z;

    if (!sprite_findActorMotion(emitter->owner, &ownerMotion)) {
        return;
    }
    if (emitter->owner->inwater) {
        return;
    }

    x = ownerMotion->x;
    y = ownerMotion->y;
    z = ownerMotion->z;
    if (sqrtf((x * x) + (y * y) + (z * z)) < 8.100000381f) {
        return;
    }

    params.unk38 = 0;
    params.flags = 0x27;
    params.updateFunc = 0;
    params.updateFunc2 = 1;
    params.preDrawFunc = -1;
    params.postDrawFunc = -1;
    params.lifetime = (math_random() % 41U) + 90;
    params.model = 0xA0;
    params.bank = 0x13;
    params.render.setGeometryMode = 0x220405;
    params.render.clearGeometryMode = 0x40200;
    params.render.envMode = 8;
    params.render.renderMode = 1;
    params.render.combiner = 0x38;
    params.render.otherModeH = 0;
    params.render.primMode = 0;
    params.render.colorDither = 0x80;
    params.render.alphaDither = 0x20;
    params.color0.red = 255;
    params.color0.green = 255;
    params.color0.blue = 255;
    params.color0.alpha = 255;
    params.color1.red = 255;
    params.color1.green = 255;
    params.color1.blue = 255;
    params.color1.alpha = 255;
    params.owner = emitter->owner;
    params.ownerId = emitter->owner->unique_id;
    params.lightMode = 1;
    params.pos = *from;
    motion.vel.x = (to->x - from->x) * emitter->scale;
    motion.vel.y = (to->y - from->y) * emitter->scale;
    motion.vel.z = (to->z - from->z) * emitter->scale;
    params.pos.x += motion.vel.x * t;
    params.pos.y += motion.vel.y * t;
    params.pos.z += motion.vel.z * t;
    motion.rot.x = math_randomFloat() * 360.0f;
    motion.rot.y = math_randomFloat() * 360.0f;
    motion.rot.z = math_randomFloat() * 360.0f;
    motion.scale = ((math_randomFloat() * 0.0f) + 340.0f) * 0.001000000047f;
    motion.rotVel.x = ((math_randomFloat() * 1000.0f) + -500.0f) * 0.009999999776f;
    motion.rotVel.y = 0.0f;
    motion.rotVel.z = ((math_randomFloat() * 1000.0f) + -500.0f) * 0.009999999776f;
    motion.gravity = ((math_randomFloat() * 121.0f) + -135.0f) * 0.001000000047f;
    motion.drag = 0.9828829765f;
    params.fadeStart = 25;
    params.fadeRate = 10;
    motion.flags = 0xF;
    effect_newMovingModelEffect(&params, &motion, 0, (math_randomFloat() * 1814.0f) * 0.001000000047f, 0, 0, 0, emitter->unkC, emitter->unk1);
}
