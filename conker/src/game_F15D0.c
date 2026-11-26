#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// particle_spawnSteamParticle (game_2062D0.c) is declared in each file that calls it: game/Beam/ModelBeam/modelbeam.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Data of the effect spawned by effect_spawnPulseEffect (at +0x28 of the effect object). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;       /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x08 */ f32 unk8;
    /* 0x0C */ SpriteParticle *particle;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 flags;
    /* 0x18 */ s32 unk18;
} EffectData; /* size 0x1C */

/* Effect object (TimedEffect is a shorter view of its head). */
struct PulseEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EffectData data;
};

/* Spawns an effect that follows `owner` (lifetime `duration`, -1 for the default 300) together with
 * a pulsing particle. */
void effect_spawnPulseEffect(Actor *owner, s16 duration, u8 arg2, s32 arg3) {
    EffectData data;
    PulseEffect *effect;
    EffectData *state;
    ParticleDesc spawn;
    ParticlePulse pulse;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.unk8 = 0.0f;
    data.particle = NULL;
    data.unk10 = 0.0f;
    data.unk14 = -1;
    data.flags = 0;
    data.unk18 = 0;
    if (owner->character_type == 0x34) {
        data.unk14 = 0x54;
        data.flags = 1;
    }
    effect = effect_newTimedEffect((duration == -1) ? 300 : duration, -1, 0x53, -1, (duration == -1) ? 0 : 1,
                                     0x40, sizeof(EffectData), arg2, arg3); /* arg6 is the data size; its prototype is wrong */
    if (effect != NULL) {
        state = &effect->data;
        memcpy(state, &data, sizeof(EffectData));
        { // matching: textures must be scoped after the memcpy
            const s32 textures[4] = { 0x60, 0x61, 0x62, 0x63 };

            pulse.phase[0] = func_150ADA20();
            pulse.phase[1] = func_150ADA20();
            pulse.baseSize = spawn.width = spawn.height = 700.0f;
            pulse.phaseStep[0] = (s8)(func_150ADA20() % 6U) + 5; // matching: the (s8) steers t-register allocation
            pulse.phaseStep[1] = func_150ADA20() % 6U + 5;
            pulse.amplitude[0] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
            pulse.amplitude[1] = (func_150ADA68() * 0.25f + 0.200000003f) * pulse.baseSize;
            spawn.texture = *(textures + (func_150ADA20() & 3)); // matching: textures[i] reserves an extra temp
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r = 0;
            spawn.g = 0;
            spawn.b = 0;
            spawn.a = 0xFF;
            spawn.r2 = 0;
            spawn.g2 = 0;
            spawn.b2 = 0;
            spawn.alpha = 0xFF;
            spawn.brightness = 0xFF;
            spawn.pos = D_800A5480;
            spawn.offset = D_800A5480;
            spawn.vel = D_800A5480;
            spawn.scaleStart = 1;
            spawn.gravity = 0.0f;
            spawn.scaleRate = 1.0f;
            spawn.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x8000 | 0x4000;
            spawn.timer = 300;
            spawn.maxSize = 10000.0f;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.drawFunc = -1;
            spawn.updateFunc = 0x26;
            spawn.deathFunc = -1;
            spawn.handlerSet = 0;
            spawn.viewMask = NULL;
            spawn.views = 0xFF;
            spawn.minSize = 0;
            state->particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, sizeof(ParticlePulse), arg2, arg3);
            if (state->particle != NULL) {
                memcpy(&state->particle->data.pulse, &pulse, sizeof(ParticlePulse));
            }
        }
    }
}

const Vec3f D_800A0350[3] = {
    { 0.0f, 16.0f, 0.0f },
    { 0.0f, 16.0f, 0.0f },
    { 0.0f, 13.0f, 0.0f },
};
const u8 D_800A0374[4] = { 1, 2, 3, 0 };
const Vec3f D_800A0378[4] = {
    { 0.0f, 37.44779587f, 32.76682281f },
    { 0.0f, 31.68659782f, 29.16607285f },
    { 0.0f, 34.56719589f, 30.96644783f },
    { 0.0f, 29.10982704f, 25.31375504f },
};
const u8 D_800A03A8[4] = { 1, 2, 3, 4 };

/* Per-frame update of the effect_spawnPulseEffect effect: keeps its particle at a point on the owner and emits
 * smaller particles from there. Frees the effect once the owner is gone. */
void effect_updatePulseEffect(PulseEffect *effect) {
    Actor *owner;
    EffectData *data;
    Vec3f pos;
    Vec3f offset;
    f32 progress;
    s32 i;
    f32 t;
    CollisionHit ground;
    const Vec3f *offsetPtr;
    Vec3f *posPtr;
    ModelAttachment *attachment;
    s32 emit;
    const Vec3f *wind;
    Vec3f vel;
    s32 rand1;
    s32 rand0;
    f32 randF0;
    f32 randF1;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if ((owner->ob_matrix_ptr == NULL) || (data->flags & 2)) {
        if (data->particle != NULL) {
            data->particle->head.desc.drawFunc = 3;
        }
        return;
    }
    collision_initHitFromObject(&ground, owner);
    if (data->unk18 == 0) {
        data->unk18 = sound_playActorSound2(150, owner, 0x36B0, 400, 0x4268);
    }
    if (data->flags & 1) {
        attachment = attach_find(owner, data->unk14, 0);
        if (attachment == NULL) {
            return;
        }
        if (owner->taskint[2] != 0) { // fade active: timer unk2D8[0] times rate unk2D8[1]
            progress = owner->unk2D8[0] * owner->unk2D8[1];
        } else {
            progress = 0.0f;
        }
        i = progress * 4.0f;
        if (i >= 4) {
            i = 3;
        }
        t = (progress - i * 0.25f) * 4.0f;
        offset.x = D_800A0378[i].x * t;
        offset.y = D_800A0378[i].y * t;
        offset.z = D_800A0378[i].z * t;
        offsetPtr = &offset;
        posPtr = &pos;
        if (func_1514654C(owner, attachment, D_800A03A8[i], &offsetPtr, &posPtr, 1) == 0) {
            pos.x = owner->xpos;
            pos.y = owner->ypos + 800.0f;
            pos.z = owner->zpos;
        }
    } else {
        if (owner->taskint[1] != 0) {
            progress = (f32)owner->taskint[0] / owner->taskint[1];
        } else {
            progress = 1.0f;
        }
        i = progress * 3.0f;
        if (i >= 3) {
            i = 2;
        }
        t = (progress - i * 0.33333334f) * 3.0f;
        offset.x = D_800A0350[i].x * t;
        offset.y = D_800A0350[i].y * t;
        offset.z = D_800A0350[i].z * t;
        func_15143134(&offset, &pos, owner->ob_matrix_ptr + D_800A0374[i]);
    }
    if (data->particle != NULL) {
        data->particle->head.desc.drawFunc = -1;
        data->particle->head.desc.pos = pos;
    }
    emit = TRUE;
    if ((level == 2) && (zone_isActorOutsideId(owner - movement_obj, 0x4065) == 0)) {
        emit = FALSE;
    }
    if (emit) {
        data->unk8 += (0.1290000081f + func_150ADA68() * 0.1170000061f) * gFrameDelta;
        while (data->unk8 > 1.0f) {
            weapon_spawnSmallImpactBurst(&pos, effect->base.header.owner, effect->base.header.kind);
            data->unk8 -= 1.0f;
        }
    }
    data->unk10 += (0.1300000101f + func_150ADA68() * 0.2f) * gFrameDelta;
    if (data->unk10 > 1.0f) {
        if (sprite_findActorMotion(owner, &wind) == 0) {
            wind = &D_800A5480;
        }
        do {
            rand1 = func_150ADA20();
            rand0 = func_150ADA20();
            func_15143794(rand1 & 0xFF, (rand0 % 25U) - 0x40, func_150ADA68() * 2.597000122f + 4.870000362f, &vel);
            vel.x -= wind->x * 0.6f;
            vel.z -= wind->z * 0.6f;
            randF0 = func_150ADA68();
            rand1 = func_150ADA20();
            rand0 = func_150ADA20();
            randF1 = func_150ADA68();
            particle_spawnSteamParticle(&pos, &vel, 0xC, randF0 * 0.2580000162f + -0.6080000401f, (rand1 % 41U) + 60, (rand0 % 56U) + 200,
                          randF1 * 160.0f + 121.0f, func_150ADA68() < 0.7000000477f, 1.0f, 1.0f, 0, &ground, 1, 1,
                          effect->base.header.owner, effect->base.header.kind);
            data->unk10 -= 1.0f;
        } while (data->unk10 > 1.0f);
    }
}

/* Frees the effect's particle, if it has one. */
void effect_freePulseParticle(PulseEffect *effect) {
    // matching: the s32 read keeps IDO from merging the test and the argument into one load
    if (*(s32 *)&effect->data.particle != 0) {
        effect_destroyObject(&effect->data.particle->head.header);
    }
}

/* Frees the effect's particle, then calls effect_defaultDestroyTimedEffect on the effect. */
void effect_destroyPulseEffect(PulseEffect *effect) {
    effect_freePulseParticle(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}

/* Frees the effect's particle, then calls effect_defaultFreeTimedEffect on the effect. */
void effect_freePulseEffect(PulseEffect *effect) {
    effect_freePulseParticle(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/* Event handler: events 0x55/0x56 clear/set flag 2 (pausing the effect), others go to effect_handleTimedEffectOwnerEvent. */
void effect_pulseEffectMessage(PulseEffect *effect, s32 arg1, u8 event) {
    EffectData *data = &effect->data;

    if (event == 0x55) {
        data->flags &= ~2;
    } else if (event == 0x56) {
        data->flags |= 2;
    } else {
        effect_handleTimedEffectOwnerEvent(arg1, event, &data->owner, &data->ownerId, effect);
    }
}
