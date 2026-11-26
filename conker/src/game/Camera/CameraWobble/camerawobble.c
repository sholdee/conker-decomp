#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
CamOffset gCameraOffsets[4];

#include "functions.h"
#include "variables.h"

/*
 * Camera wobble effects: up to six sine waves (position xyz, rotation xyz) that are summed into a
 * player's camera offset each frame (gCameraOffsets, applied to the view matrix in game/Camera/ViewMatrix/viewmatrix.c).
 */

/* Update callbacks (CamWobbleDef.updateFn; 0 stops the wobble) and amplitude scale callbacks
 * (CamWobbleDef.scaleFn). Declared without a prototype: the callbacks take the wobble as their own type. */
static s32 (*gWobbleUpdateCallbacks[])() = { camera_updateWobbleEnvelope };
static f32 (*gWobbleScaleCallbacks[])() = { func_150FCF1C, camera_getWobbleFalloff };

/*
 * Creates a wobble effect from def with extraSize bytes of extra state after it. Returns NULL if the
 * player's unk3D0->mv->AimState is set or no effect could be allocated.
 */
CamWobble *camera_createWobble(CamWobbleDef *def, s32 extraSize, u8 arg2, s32 arg3) {
    CamWobble *wobble;
    Camera *player;

    player = &camera_global_Cameras[def->playerIndex];
    if (player->CharPtr != NULL && player->CharPtr->mv != NULL && player->CharPtr->mv->AimState != 0) {
        return NULL;
    }
    wobble = effect_newObject(0x39, arg3, extraSize + sizeof(CamWobble), 1, arg2, 1);
    if (wobble == NULL) {
        return NULL;
    }
    memcpy(&wobble->def, def, sizeof(CamWobbleDef));
    wobble->posHalf.v[0] = 1;
    wobble->posHalf.v[1] = 1;
    wobble->posHalf.v[2] = 1;
    wobble->rotHalf.v[0] = 1;
    wobble->rotHalf.v[1] = 1;
    wobble->rotHalf.v[2] = 1;
    wobble->phase[0] = 0.0f;
    wobble->phase[1] = 0.0f;
    wobble->phase[2] = 0.0f;
    wobble->phase[3] = 0.0f;
    wobble->phase[4] = 0.0f;
    wobble->phase[5] = 0.0f;
    return wobble;
}

/* Asks the wobble to wind down: each wave stops at its next zero crossing. */
void camera_stopWobble(CamWobble *wobble) {
    wobble->def.flags |= CAM_WOBBLE_STOPPING;
}

/* Advances wave i (half: its CamWobbleHalves) and adds its displacement to the player's camera offset. */
#define WOBBLE_WAVE(i, axis, half, prevHalf)                                                      \
    if (wobble->def.speed[i] != 0.0f) {                                                           \
        wobble->phase[i] = math_wrapRadians(wobble->def.speed[i] * gFrameDelta + wobble->phase[i]);   \
        half.v[(i) % 3] = wobble->phase[i] < 3.1415927f;                                          \
        if ((wobble->def.flags & CAM_WOBBLE_STOPPING) && (prevHalf.v[(i) % 3] != half.v[(i) % 3])) { \
            wobble->def.speed[i] = 0.0f;                                                          \
            wobble->phase[i] = 0.0f;                                                              \
        }                                                                                         \
        gCameraOffsets[wobble->def.playerIndex].axis += sinf(wobble->phase[i]) * wobble->def.amplitude[i] * scale; \
    }

/* Per-frame update of a wobble effect; frees it once every wave has stopped. */
void camera_updateWobble(CamWobble *wobble) {
    s32 pad; // matching: unused stack slot in the original frame
    u8 finished;
    u8 stop;
    f32 scale;
    CamWobbleHalves prevPosHalf;
    CamWobbleHalves prevRotHalf;

    finished = 0;
    stop = 0;
    if (wobble->def.flags & CAM_WOBBLE_TIMED) {
        wobble->def.timer -= D_800BE9E4;
        if (wobble->def.timer < 0) {
            stop = 1;
        }
    }
    if (!stop && wobble->def.updateFn != -1) {
        if (!gWobbleUpdateCallbacks[wobble->def.updateFn](wobble)) {
            stop = 1;
        }
    }
    if (stop) {
        camera_stopWobble(wobble);
    }
    prevPosHalf = wobble->posHalf;
    prevRotHalf = wobble->rotHalf;
    scale = 1.0f;
    if (wobble->def.flags & CAM_WOBBLE_SCALED) {
        if (wobble->def.scaleFn != -1) {
            scale = gWobbleScaleCallbacks[wobble->def.scaleFn](wobble);
        }
    }
    WOBBLE_WAVE(0, pos.x, wobble->posHalf, prevPosHalf)
    WOBBLE_WAVE(1, pos.y, wobble->posHalf, prevPosHalf)
    WOBBLE_WAVE(2, pos.z, wobble->posHalf, prevPosHalf)
    WOBBLE_WAVE(3, rot.x, wobble->rotHalf, prevRotHalf)
    WOBBLE_WAVE(4, rot.y, wobble->rotHalf, prevRotHalf)
    WOBBLE_WAVE(5, rot.z, wobble->rotHalf, prevRotHalf)
    if (wobble->def.speed[0] == 0.0f && wobble->def.speed[1] == 0.0f && wobble->def.speed[2] == 0.0f &&
        wobble->def.speed[3] == 0.0f && wobble->def.speed[4] == 0.0f && wobble->def.speed[5] == 0.0f) {
        finished = 1;
    }
    if (finished) {
        effect_destroyObject(&wobble->header);
    }
}

/* Clears every player's camera offset. */
void camera_clearOffsets(void) {
    bzero(gCameraOffsets, sizeof(gCameraOffsets));
}

/*
 * Starts one of the preset wobbles for a player. Presets 5 and 6 fade with distance from falloff->origin;
 * only they return the wobble.
 */
CamWobbleEnv *camera_startWobblePreset(u8 presetIndex, u8 playerIndex, CamWobbleFalloff *falloff, u8 arg3, s32 arg4) {
    switch (presetIndex) {
    case 7: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 8.0f;
        preset.peak.ampPos.y = 12.0f;
        preset.peak.ampPos.z = 9.0f;
        preset.peak.ampRot.x = 0.5f;
        preset.peak.ampRot.y = 0.199999988f;
        preset.peak.ampRot.z = 0.299999982f;
        preset.peak.speedPos.x = 1.15600002f;
        preset.peak.speedPos.y = 0.964000046f;
        preset.peak.speedPos.z = 0.458000034f;
        preset.peak.speedRot.x = 2.27859998f;
        preset.peak.speedRot.y = 1.91939998f;
        preset.peak.speedRot.z = 1.52499998f;
        preset.riseTime = 4.0f;
        preset.holdTime = 10.0f;
        preset.fallTime = 16.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, -1, 0, arg3, arg4);
        break;
    }
    case 0: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 10.0f;
        preset.peak.ampPos.y = 4.0f;
        preset.peak.ampPos.z = 7.0f;
        preset.peak.ampRot.x = 0.780599952f;
        preset.peak.ampRot.y = 0.286899984f;
        preset.peak.ampRot.z = 0.888799965f;
        preset.peak.speedPos.x = 0.666000009f;
        preset.peak.speedPos.y = 1.19400001f;
        preset.peak.speedPos.z = 1.00999999f;
        preset.peak.speedRot.x = 0.435299993f;
        preset.peak.speedRot.y = 0.586699963f;
        preset.peak.speedRot.z = 0.709699988f;
        preset.riseTime = 6.0f;
        preset.holdTime = 17.0f;
        preset.fallTime = 48.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, -1, 0, arg3, arg4);
        break;
    }
    case 1: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 20.0f;
        preset.peak.ampPos.y = 19.0f;
        preset.peak.ampPos.z = 53.0f;
        preset.peak.ampRot.x = 0.0f;
        preset.peak.ampRot.y = 0.0f;
        preset.peak.ampRot.z = 1.47780001f;
        preset.peak.speedPos.x = 0.602000058f;
        preset.peak.speedPos.y = 1.02100003f;
        preset.peak.speedPos.z = 1.1910001f;
        preset.peak.speedRot.x = 0.0f;
        preset.peak.speedRot.y = 0.0f;
        preset.peak.speedRot.z = 2.90499997f;
        preset.riseTime = 3.0f;
        preset.holdTime = 5.0f;
        preset.fallTime = 30.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, -1, 0, arg3, arg4);
        break;
    }
    case 2: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 10.0f;
        preset.peak.ampPos.y = 7.0f;
        preset.peak.ampPos.z = 6.0f;
        preset.peak.ampRot.x = 1.0029f;
        preset.peak.ampRot.y = 0.497699976f;
        preset.peak.ampRot.z = 1.43059993f;
        preset.peak.speedPos.x = 0.654000044f;
        preset.peak.speedPos.y = 1.06200004f;
        preset.peak.speedPos.z = 0.32100001f;
        preset.peak.speedRot.x = 0.516900003f;
        preset.peak.speedRot.y = 0.772799969f;
        preset.peak.speedRot.z = 0.175599992f;
        preset.riseTime = 2.0f;
        preset.holdTime = 27.0f;
        preset.fallTime = 15.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, -1, 0, arg3, arg4);
        break;
    }
    case 3: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 7.0f;
        preset.peak.ampPos.y = 11.0f;
        preset.peak.ampPos.z = 5.0f;
        preset.peak.ampRot.x = 0.432099998f;
        preset.peak.ampRot.y = 0.6329f;
        preset.peak.ampRot.z = 0.808200002f;
        preset.peak.speedPos.x = 0.276000023f;
        preset.peak.speedPos.y = 0.539000034f;
        preset.peak.speedPos.z = 0.43900001f;
        preset.peak.speedRot.x = 1.67269993f;
        preset.peak.speedRot.y = 0.494699985f;
        preset.peak.speedRot.z = 0.98939997f;
        preset.riseTime = 5.0f;
        preset.holdTime = 15.0f;
        preset.fallTime = 12.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, -1, 0, arg3, arg4);
        break;
    }
    case 4: {
        CamWobblePreset preset;
        preset.peak.ampPos.x = 8.0f;
        preset.peak.ampPos.y = 17.0f;
        preset.peak.ampPos.z = 4.0f;
        preset.peak.ampRot.x = 0.761399984f;
        preset.peak.ampRot.y = 1.57069993f;
        preset.peak.ampRot.z = 1.26679993f;
        preset.peak.speedPos.x = 0.587000012f;
        preset.peak.speedPos.y = 0.906000018f;
        preset.peak.speedPos.z = 0.403000027f;
        preset.peak.speedRot.x = 1.15919995f;
        preset.peak.speedRot.y = 0.697299957f;
        preset.peak.speedRot.z = 1.34640002f;
        preset.riseTime = 5.0f;
        preset.holdTime = 3.0f;
        preset.fallTime = 45.0f;
        camera_startEnvelopeWobble(&preset, playerIndex, 0, 0, arg3, arg4);
        break;
    }
    case 5: {
        CamWobblePreset preset;
        CamWobbleEnv *wobble;
        preset.peak.ampPos.x = 15.0f;
        preset.peak.ampPos.y = 10.0f;
        preset.peak.ampPos.z = 19.0f;
        preset.peak.ampRot.x = 0.539699972f;
        preset.peak.ampRot.y = 0.76789999f;
        preset.peak.ampRot.z = 0.988099992f;
        preset.peak.speedPos.x = 1.04500008f;
        preset.peak.speedPos.y = 0.636000037f;
        preset.peak.speedPos.z = 1.40400004f;
        preset.peak.speedRot.x = 0.452600002f;
        preset.peak.speedRot.y = 0.484999985f;
        preset.peak.speedRot.z = 0.684300005f;
        preset.riseTime = 5.0f;
        preset.holdTime = 5.0f;
        preset.fallTime = 25.0f;
        wobble = camera_startEnvelopeWobble(&preset, playerIndex, 1, 24, arg3, arg4);
        if (wobble != NULL) {
            memcpy(&wobble->falloff, falloff, sizeof(CamWobbleFalloff));
        }
        return wobble;
    }
    case 6: {
        CamWobblePreset preset;
        CamWobbleEnv *wobble;
        preset.peak.ampPos.x = 15.0f;
        preset.peak.ampPos.y = 10.0f;
        preset.peak.ampPos.z = 19.0f;
        preset.peak.ampRot.x = 0.539699972f;
        preset.peak.ampRot.y = 0.76789999f;
        preset.peak.ampRot.z = 0.988099992f;
        preset.peak.speedPos.x = 1.04500008f;
        preset.peak.speedPos.y = 0.636000037f;
        preset.peak.speedPos.z = 1.40400004f;
        preset.peak.speedRot.x = 0.452600002f;
        preset.peak.speedRot.y = 0.484999985f;
        preset.peak.speedRot.z = 0.684300005f;
        preset.riseTime = 5.0f;
        preset.holdTime = 5.0f;
        preset.fallTime = 25.0f;
        wobble = camera_startEnvelopeWobble(&preset, playerIndex, 1, 24, arg3, arg4);
        if (wobble != NULL) {
            memcpy(&wobble->falloff, falloff, sizeof(CamWobbleFalloff));
        }
        return wobble;
    }
    }
}

/* Starts an envelope-driven wobble from preset; NULL if a duration is 0 or no effect was made. */
CamWobbleEnv *camera_startEnvelopeWobble(CamWobblePreset *preset, u8 playerIndex, s8 scaleFn, s32 extraSize, u8 arg4, s32 arg5) {
    CamWobbleDef def;
    CamWobbleEnvelope envelope;
    CamWobbleEnv *wobble;
    f32 invRiseTime;
    f32 invFallTime;

    if (preset->riseTime == 0.0f || preset->fallTime == 0.0f) {
        return NULL;
    }

    invRiseTime = 1.0f / preset->riseTime;
    invFallTime = 1.0f / preset->fallTime;

    def.flags = CAM_WOBBLE_SCALED;
    def.updateFn = 0; // camera_updateWobbleEnvelope
    def.timer = 300;
    def.playerIndex = playerIndex;
    def.scaleFn = scaleFn;
    // matching: integer 0s; 0.0f would reuse the register of the == 0.0f tests above
    def.amplitude[0] = 0;
    def.amplitude[1] = 0;
    def.amplitude[2] = 0;
    def.amplitude[3] = 0;
    def.amplitude[4] = 0;
    def.amplitude[5] = 0;
    def.speed[0] = 0;
    def.speed[1] = 0;
    def.speed[2] = 0;
    def.speed[3] = 0;
    def.speed[4] = 0;
    def.speed[5] = 0;

    envelope.riseRate.ampPos.x = preset->peak.ampPos.x * invRiseTime;
    envelope.riseRate.ampPos.y = preset->peak.ampPos.y * invRiseTime;
    envelope.riseRate.ampPos.z = preset->peak.ampPos.z * invRiseTime;
    envelope.riseRate.ampRot.x = preset->peak.ampRot.x * invRiseTime;
    envelope.riseRate.ampRot.y = preset->peak.ampRot.y * invRiseTime;
    envelope.riseRate.ampRot.z = preset->peak.ampRot.z * invRiseTime;
    envelope.riseRate.speedPos.x = preset->peak.speedPos.x * invRiseTime;
    envelope.riseRate.speedPos.y = preset->peak.speedPos.y * invRiseTime;
    envelope.riseRate.speedPos.z = preset->peak.speedPos.z * invRiseTime;
    envelope.riseRate.speedRot.x = preset->peak.speedRot.x * invRiseTime;
    envelope.riseRate.speedRot.y = preset->peak.speedRot.y * invRiseTime;
    envelope.riseRate.speedRot.z = preset->peak.speedRot.z * invRiseTime;

    envelope.fallRate.ampRot.x = preset->peak.ampPos.x * invFallTime;
    envelope.fallRate.ampRot.y = preset->peak.ampPos.y * invFallTime;
    envelope.fallRate.ampRot.z = preset->peak.ampPos.z * invFallTime;
    envelope.fallRate.ampPos.x = preset->peak.ampRot.x * invFallTime;
    envelope.fallRate.ampPos.y = preset->peak.ampRot.y * invFallTime;
    envelope.fallRate.ampPos.z = preset->peak.ampRot.z * invFallTime;
    envelope.fallRate.speedRot.x = preset->peak.speedPos.x * invFallTime;
    envelope.fallRate.speedRot.y = preset->peak.speedPos.y * invFallTime;
    envelope.fallRate.speedRot.z = preset->peak.speedPos.z * invFallTime;
    envelope.fallRate.speedPos.x = preset->peak.speedRot.x * invFallTime;
    envelope.fallRate.speedPos.y = preset->peak.speedRot.y * invFallTime;
    envelope.fallRate.speedPos.z = preset->peak.speedRot.z * invFallTime;

    envelope.peak.ampPos = preset->peak.ampPos;
    envelope.peak.ampRot = preset->peak.ampRot;
    envelope.peak.speedPos = preset->peak.speedPos;
    envelope.peak.speedRot = preset->peak.speedRot;

    envelope.riseTime = preset->riseTime;
    envelope.holdTime = preset->holdTime;
    envelope.fallTime = preset->fallTime;
    envelope.time = 0; // matching: integer 0, as above

    wobble = (CamWobbleEnv *)camera_createWobble(&def, extraSize + sizeof(CamWobbleEnvelope), arg4, arg5);
    if (wobble != NULL) {
        memcpy(&wobble->envelope, &envelope, sizeof(CamWobbleEnvelope));
    }
    return wobble;
}

/* Update callback 0: moves the amplitudes and speeds along the envelope; returns 0 once it has ended. */
s32 camera_updateWobbleEnvelope(CamWobbleEnv *wobble) {
    CamWobbleEnvelope *env;
    s32 active;

    env = &wobble->envelope;
    active = 1;
    env->time += gFrameDelta;
    if (env->time < env->riseTime) {
        wobble->shape.ampPos.x = env->riseRate.ampPos.x * env->time;
        wobble->shape.ampPos.y = env->riseRate.ampPos.y * env->time;
        wobble->shape.ampPos.z = env->riseRate.ampPos.z * env->time;
        wobble->shape.ampRot.x = env->riseRate.ampRot.x * env->time;
        wobble->shape.ampRot.y = env->riseRate.ampRot.y * env->time;
        wobble->shape.ampRot.z = env->riseRate.ampRot.z * env->time;
        wobble->shape.speedPos.x = env->riseRate.speedPos.x * env->time;
        wobble->shape.speedPos.y = env->riseRate.speedPos.y * env->time;
        wobble->shape.speedPos.z = env->riseRate.speedPos.z * env->time;
        wobble->shape.speedRot.x = env->riseRate.speedRot.x * env->time;
        wobble->shape.speedRot.y = env->riseRate.speedRot.y * env->time;
        wobble->shape.speedRot.z = env->riseRate.speedRot.z * env->time;
    } else if (env->time < env->riseTime + env->holdTime) {
        wobble->shape.ampPos = env->peak.ampPos;
        wobble->shape.ampRot = env->peak.ampRot;
        wobble->shape.speedPos = env->peak.speedPos;
        wobble->shape.speedRot = env->peak.speedRot;
    } else if (env->time < (env->riseTime + env->holdTime) + env->fallTime) {
        f32 t = env->time - (env->riseTime + env->holdTime);

        wobble->shape.ampPos.x = env->peak.ampPos.x - env->fallRate.ampRot.x * t;
        wobble->shape.ampPos.y = env->peak.ampPos.y - env->fallRate.ampRot.y * t;
        wobble->shape.ampPos.z = env->peak.ampPos.z - env->fallRate.ampRot.z * t;
        wobble->shape.ampRot.x = env->peak.ampRot.x - env->fallRate.ampPos.x * t;
        wobble->shape.ampRot.y = env->peak.ampRot.y - env->fallRate.ampPos.y * t;
        wobble->shape.ampRot.z = env->peak.ampRot.z - env->fallRate.ampPos.z * t;
        wobble->shape.speedPos.x = env->peak.speedPos.x - env->fallRate.speedRot.x * t;
        wobble->shape.speedPos.y = env->peak.speedPos.y - env->fallRate.speedRot.y * t;
        wobble->shape.speedPos.z = env->peak.speedPos.z - env->fallRate.speedRot.z * t;
        wobble->shape.speedRot.x = env->peak.speedRot.x - env->fallRate.speedPos.x * t;
        wobble->shape.speedRot.y = env->peak.speedRot.y - env->fallRate.speedPos.y * t;
        wobble->shape.speedRot.z = env->peak.speedRot.z - env->fallRate.speedPos.z * t;
    } else {
        active = 0;
    }
    return active;
}

/* Scale callback 1: fades the wobble with the player's distance from its falloff origin. */
f32 camera_getWobbleFalloff(CamWobbleEnv *wobble) {
    return camera_getFalloffScale((CamWobble *)wobble, &wobble->falloff.origin, wobble->falloff.radius, wobble->falloff.fadeRange,
                         wobble->falloff.invFadeRange);
}

/* 1 within radius of origin, 0 beyond radius + fadeRange, linear in between. */
f32 camera_getFalloffScale(CamWobble *wobble, Vec3f *origin, f32 radius, f32 fadeRange, f32 invFadeRange) {
    f32 scale;
    Vec3f delta;
    Vec3f *playerPos;
    f32 dist;

    playerPos = math_getCameraPos(wobble->def.playerIndex);
    delta.x = origin->x - playerPos->x;
    delta.y = origin->y - playerPos->y;
    delta.z = origin->z - playerPos->z;
    dist = math_vec3Length(&delta);
    if (dist < radius) {
        scale = 1.0f;
    } else if (radius + fadeRange < dist) {
        scale = 0.0f;
    } else {
        scale = 1.0f - (dist - radius) * invFadeRange;
    }
    return scale;
}
