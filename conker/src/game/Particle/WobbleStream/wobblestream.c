#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/weapon.h"

/* Emits this frame's particles along a sine-wobbled direction and advances the wobble phases. */
void particle_updateWobbleStream(ProjectileStreamEffect *actor) {
    ProjectileStream *emission;
    f32 inverse;
    f32 time;
    f32 timeStep;
    f32 speedX;
    f32 speedY;
    f32 phaseX;
    f32 phaseY;
    f32 phaseStepX;
    f32 phaseStepY;
    Vec3f direction;

    emission = &actor->emission;
    emission->stream.remaining += emission->stream.rate * gFrameDelta;
    if (emission->stream.remaining > 1.0f) {
        inverse = 1.0f / emission->stream.remaining;
        time = emission->stream.time + gFrameDelta;
        timeStep = time * inverse;
        speedX = emission->stream.speedX * gFrameDelta;
        speedY = emission->stream.speedY * gFrameDelta;
        phaseX = emission->stream.phaseX;
        phaseY = emission->stream.phaseY;
        phaseStepX = speedX * inverse;
        phaseStepY = speedY * inverse;
        do {
            f32 sineX;
            f32 sineY;
            f32 x;
            f32 y;
            f32 randomX;
            f32 randomY;

            sineX = sinf(phaseX);
            sineY = sinf(phaseY);
            x = emission->stream.angleX + sineX * emission->stream.amplitudeX;
            y = emission->stream.angleY + sineY * emission->stream.amplitudeY;
            math_dirFromAngles(x, y, 5.0f, &direction);
            randomX = math_randomFloat();
            randomY = math_randomFloat();
            weapon_spawnProjectile(&emission->stream.position, &direction, 0, NULL, 0, 0, emission->speed, emission->distScale,
                          randomX * 40.0f + 55.0f, randomY * 300.0f + 250.0f, emission->defaultRemaining,
                          math_random() % 56U + 200, NULL, 0, 0, 0, 0, 0, 0, -1, 0, time, 255, -1, 0,
                          actor->base.header.owner, actor->base.header.kind);
            phaseX += phaseStepX;
            phaseY += phaseStepY;
            time -= timeStep;
            emission->stream.remaining -= 1.0f;
        } while (emission->stream.remaining > 1.0f);
        emission->stream.phaseX = math_wrapRadians(phaseX);
        emission->stream.phaseY = math_wrapRadians(phaseY);
        emission->stream.time = time;
    }
}
