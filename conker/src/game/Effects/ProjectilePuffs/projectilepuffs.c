#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/weapon.h"


/* Per-object state of the projectile spawned by effect_spawnProjectile (updated by effect_updateProjectile). */
typedef struct {
    /* 0x00 */ RayHit hit;   /* weapon_setupProjectileRay's ray along the flight path */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ f32 timeLeft;
    /* 0x6C */ f32 speed;
    /* 0x70 */ u8 flags; /* 1: impact effect, 2: ray-test the path, 4: see effect_updateProjectile */
    /* 0x71 */ u8 pad71[3];
    /* 0x74 */ Vec3f unk74;
    /* 0x80 */ Vec3f unk80;
    /* 0x8C */ f32 trailDist;
    /* 0x90 */ f32 trailAge;
    /* 0x94 */ f32 trailAgeStep;
    /* 0x98 */ Vec3f trailPos;
    /* 0xA4 */ Vec3f trailStep;
    /* 0xB0 */ s8 unkB0;
    /* 0xB1 */ u8 padB1[3];
} ProjectileState;

/* Per-object state of the effect spawned by effect_spawnPuffFollower (updated by effect_updatePuffFollower). */
typedef struct {
    /* 0x00 */ f32 *target; /* position (3 floats), then 3 more floats copied to unk20 */
    /* 0x04 */ Vec3f trailPos;
    /* 0x10 */ f32 trailAge;
} FollowState;

/* An object spawned by particle_newModelParticleWithFloor. */
struct ProjectileParticle {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0x13];
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ u8 pad50[0x2C];
    /* 0x07C */ Actor *owner;
    /* 0x080 */ u8 pad80[8];
    /* 0x088 */ s32 sound;
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ union {
        ProjectileState projectile;
        FollowState follow;
    } state;
};

/* Spawns a projectile at `pos`, aimed along `aim` (or from `altOrigin` at `target` when both are set). `knownHit`
 * is a ray already cast (see weapon_setupProjectileRay). */
void effect_spawnProjectile(Vec3f *pos, Vec3f *aim, Vec3f *altOrigin, Vec3f *target, u8 targetIsDir, RayHit *knownHit, f32 speed,
                   f32 arg7, f32 arg8, Actor *owner, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14,
                   s8 arg15, s32 sound, u8 arg17, u8 arg18, s32 arg19) {
    ProjectileState state;
    ParticleSpawn spawn;
    ProjectileParticle *obj;
    Vec3f aimDir;
    u8 hasAimDir;
    Vec3f targetDir;
    u8 hasTargetDir;

    hasAimDir = FALSE;
    hasTargetDir = targetIsDir;
    if (targetIsDir) {
        targetDir = *target;
    }
    state.flags = (arg11 ? 1 : 0) | (arg12 ? 2 : 0) | (arg13 ? 4 : 0);
    state.unkB0 = arg15;
    if (altOrigin != NULL && target != NULL) {
        if (!hasTargetDir) {
            if (!math_vec3Normalize(target, &targetDir, NULL, NULL)) {
                return;
            }
            hasTargetDir = TRUE;
        }
    } else if (aim != NULL) {
        if (!math_vec3Normalize(aim, &aimDir, NULL, NULL)) {
            return;
        }
        hasAimDir = TRUE;
    }
    if (!weapon_setupProjectileRay(knownHit, &state.hit, pos, hasAimDir ? &aimDir : NULL, &state.timeLeft, arg7, arg10, altOrigin,
                       hasTargetDir ? &targetDir : NULL, &state.flags, 1, arg8, owner, arg15, arg12, arg13, 0,
                       arg17)) {
        return;
    }
    if (!math_buildPerpendicularBasis(&state.hit.dir, &state.unk74, &state.unk80)) {
        return;
    }
    state.unk64 = arg14;
    state.trailAge = 0.0f;
    state.trailDist = 0.0f;
    state.trailAgeStep = 70.0f * arg7;
    state.trailPos = *pos;
    state.speed = speed;
    state.trailStep.x = state.hit.dir.x * 70.0f;
    state.trailStep.y = state.hit.dir.y * 70.0f;
    state.trailStep.z = state.hit.dir.z * 70.0f;
    state.flags |= (state.hit.dist > 150.0f) ? 1 : 0;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.sizeY = 0.02000000142f;
    spawn.sizeXZ = 0.02000000142f;
    math_getHeadingElevation(&state.hit.dir, &spawn.rot.y, &spawn.rot.x);
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rot.z = 0.0f;
    spawn.pos = *pos;
    spawn.vel.x = state.hit.dir.x * speed;
    spawn.vel.y = state.hit.dir.y * speed;
    spawn.vel.z = state.hit.dir.z * speed;
    spawn.rotVel = gZeroVec3f;
    spawn.gravity = 0.0f;
    spawn.flags = 0x120;
    spawn.lifetime = 300;
    spawn.model = 0x2F;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 0x13;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 0;
    spawn.owner = owner;
    spawn.ownerId = owner->unique_id;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    if (sound != 0) {
        spawn.sound = sound_playAt(sound, 18000, -500, 0, -1, spawn.pos.x, spawn.pos.y, spawn.pos.z, 1000,
                                    2000);
    } else {
        spawn.sound = 0;
    }
    obj = particle_newModelParticleWithFloor(&spawn, 0, 0, 0, 0xB4, arg18, arg19);
    if (obj != NULL) {
        memcpy(&obj->state.projectile, &state, sizeof(state));
    }
}

/* Per-object state of a puff spawned by particle_newQuadParticle (updated by effect_updateTrailPuff). */
typedef struct {
    /* 0x00 */ Vec3f axisU; /* quad axes for particle_buildQuadFromAxes (vertex function effect_buildTrailPuffQuad) */
    /* 0x0C */ Vec3f axisV;
    /* 0x18 */ f32 timeLeft;
    /* 0x1C */ f32 phase;
    /* 0x20 */ f32 phaseSpeed;
    /* 0x24 */ f32 sizeBase;
    /* 0x28 */ f32 sizeAmplitude;
} PuffState;

/* A puff a projectile leaves along its path (spawned by particle_newQuadParticle). */
struct TrailPuff {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ PuffState state;
};

/*
 * Per-frame update of a projectile: ray-tests and moves it, runs out its lifetime, and leaves a
 * trail of puffs every 70 units travelled. Returns 0 once it hit something or expired.
 */
s8 effect_updateProjectile(ProjectileParticle *obj) {
    ProjectileState *state;
    Actor *owner;
    s8 alive;
    f32 dt;
    f32 dist;
    u8 pad[0xC]; // matching: stack space (the old local view of RayHit was 0x70 bytes)
    RayHit hit;
    f32 puffs;
    TrailPuff *puff;

    alive = TRUE;
    state = &obj->state.projectile;
    owner = obj->owner;
    dt = (state->timeLeft < gFrameDelta) ? state->timeLeft : gFrameDelta;
    if (obj->sound != 0) {
        sound_setVoicePanAt(obj->sound, obj->pos.x, obj->pos.y, obj->pos.z);
    }
    dist = state->speed * dt;
    if (state->flags & 2) {
        collision_castRay(owner, obj->pos.x, obj->pos.y, obj->pos.z, state->hit.dir.x, state->hit.dir.y,
                      state->hit.dir.z, &hit, dist, 1, 0, !(state->flags & 4), state->unkB0, 0, 0);
        if (hit.hitType >= 2) {
            particle_applyShotHit(hit.obj, owner, &hit.pos, &hit.far, &hit, 0x1A, &state->hit.dir, obj->unkC, obj->unk1);
            if (state->flags & 1) {
                effect_spawnPuffFlash(&hit.pos, obj->unkC, obj->unk1);
            }
            alive = FALSE;
        }
    }
    obj->pos.x += obj->vel.x * dt;
    obj->pos.y += obj->vel.y * dt;
    obj->pos.z += obj->vel.z * dt;
    if (alive == TRUE) {
        state->timeLeft -= gFrameDelta;
        if (state->timeLeft <= 0.0f) {
            alive = FALSE;
            if (state->flags & 1) {
                effect_spawnPuffFlash(&obj->pos, obj->unkC, obj->unk1);
            }
        }
    }
    {
        ParticleDef54 spawn;

        state->trailDist += dist;
        state->trailAge += dt;
        puffs = state->trailDist * (1.0f / 70.0f);
        if (puffs > 1.0f) {
            PuffState puffState;

            puffState.axisU = state->unk74;
            puffState.axisV = state->unk80;
            puffState.sizeBase = 8.318000793f;
            puffState.sizeAmplitude = 8.318000793f;
            spawn.texture = 0x7A;
            spawn.type = 0;
            spawn.renderModes = 0x4404;
            spawn.lifetime = 10;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            puffState.phaseSpeed = 0.4670000076f;
            spawn.rot = gZeroVec3f;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0xCC0008;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 6;
            spawn.unk48 = 0;
            spawn.viewportMask = 0xFF;
            spawn.unk50 = 0;
            do {
                const VtxColor *colors = gWhiteVtxColors;

                puffState.phase = 4.712388992f;
                puffState.timeLeft = 27.0f - state->trailAge;
                spawn.pos = state->trailPos;
                spawn.unk44 = puffState.timeLeft * 6.481481552f;
                puff = particle_newQuadParticle(&spawn, colors, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC, obj->unk1);
                if (puff != NULL) {
                    memcpy(&puff->state, &puffState, sizeof(puffState));
                }
                state->trailDist -= 70.0f;
                state->trailAge -= state->trailAgeStep;
                state->trailPos.x += state->trailStep.x;
                state->trailPos.y += state->trailStep.y;
                state->trailPos.z += state->trailStep.z;
                puffs -= 1.0f;
            } while (puffs > 1.0f);
        }
    }
    return alive;
}

/* Per-frame update of a puff: fades it out over its lifetime and pulses its size. Returns 0 once expired. */
s32 effect_updateTrailPuff(TrailPuff *obj) {
    f32 size;
    PuffState *state;

    state = &obj->state;
    obj->head.def.unk44 = state->timeLeft * 6.481481552f;
    size = (sinf(state->phase) * state->sizeAmplitude) + state->sizeBase;
    obj->head.def.height = size;
    obj->head.def.width = size;
    state->phase = math_wrapRadians(state->phase + (state->phaseSpeed * gFrameDelta));
    state->timeLeft -= gFrameDelta;
    if (state->timeLeft < 0.0f) {
        return 0;
    }
    return 1;
}

/* Quad spanned by the puff's two axes (vertex function). */
void effect_buildTrailPuffQuad(TrailPuff *obj, s16 arg1) {
    particle_buildQuadFromAxes(&obj->head, &obj->state.axisU, &obj->state.axisV, arg1);
}

/* Impact effect at `pos`. */
void effect_spawnPuffFlash(Vec3f *pos, u8 arg1, s32 arg2) {
    f32 random1;
    f32 random0;

    random0 = math_randomFloat();
    random1 = math_randomFloat();
    particle_spawnGrowFade(pos, random0 * 3.0f + 9.0f, 0.5119050145f, random1 * 70.0f + 70.0f, 0.0f, arg1, arg2);
    particle_spawnShortLightFlash(pos, arg1, arg2);
}

/* Spawns an effect that follows `target` (a position followed by three more floats), trailing puffs behind it. */
void effect_spawnPuffFollower(f32 *target, s16 arg1, u8 arg2, s32 arg3) {
    struct {
        ParticleSpawn spawn;
        FollowState follow;
    } packet;
    ProjectileParticle *obj;

    packet.follow.target = target;
    packet.follow.trailPos.x = target[0];
    packet.follow.trailPos.y = target[1];
    packet.follow.trailPos.z = target[2];
    packet.follow.trailAge = 0.0f;
    packet.spawn.radius = 1.0f;
    packet.spawn.elasticity = 1.0f;
    packet.spawn.sizeXZ = 0.02000000142f;
    packet.spawn.sizeY = 0.02000000142f;
    packet.spawn.rot.x = target[3];
    packet.spawn.rot.y = target[4];
    packet.spawn.rot.z = target[5];
    packet.spawn.scale.x = 1.0f;
    packet.spawn.scale.y = 1.0f;
    packet.spawn.scale.z = 1.0f;
    packet.spawn.pos.x = target[0];
    packet.spawn.pos.y = target[1];
    packet.spawn.pos.z = target[2];
    packet.spawn.vel.x = 0.0f;
    packet.spawn.vel.y = 0.0f;
    packet.spawn.vel.z = 0.0f;
    packet.spawn.rotVel = gZeroVec3f;
    packet.spawn.gravity = 0.0f;
    packet.spawn.flags = 0x980;
    packet.spawn.lifetime = arg1;
    packet.spawn.model = 0x2F;
    packet.spawn.variant = 0;
    packet.spawn.unk5C = 0;
    packet.spawn.alpha = 0xFF;
    packet.spawn.updateFn = 0x14;
    packet.spawn.update2Fn = 0;
    packet.spawn.floorHitFn = 0;
    packet.spawn.floorType3Fn = 0;
    packet.spawn.wallHitFn = 0;
    packet.spawn.destroyFn = 0;
    packet.spawn.squashFn = 0;
    packet.spawn.matrixFn = 0;
    packet.spawn.unk6A = 0;
    packet.spawn.owner = NULL;
    packet.spawn.ownerId = 0;
    packet.spawn.fadeStart = 1;
    packet.spawn.fadeRate = 0xFF;
    packet.spawn.sound = 0;
    obj = particle_newModelParticleWithFloor(&packet.spawn, 3, 0xFF, 0, 0xB4, arg2, arg3);
    if (obj != NULL) {
        memcpy(&obj->state.follow, &packet.follow, sizeof(packet.follow));
    }
}

/* Per-frame update of a effect_spawnPuffFollower effect: snaps to its target and drops a puff every 20 units it moved. */
s32 effect_updatePuffFollower(ProjectileParticle *obj) {
    FollowState *follow;

    follow = &obj->state.follow;
    obj->pos.x = follow->target[0];
    obj->pos.y = follow->target[1];
    obj->pos.z = follow->target[2];
    obj->unk20 = follow->target[3];
    obj->unk24 = follow->target[4];
    obj->unk28 = follow->target[5];
    {
        Vec3f delta;
        f32 dist;
        f32 unused;
        Vec3f dir;
        f32 puffs;

        delta.x = obj->pos.x - follow->trailPos.x;
        delta.y = obj->pos.y - follow->trailPos.y;
        delta.z = obj->pos.z - follow->trailPos.z;
        follow->trailAge += gFrameDelta;
        if (math_vec3Normalize(&delta, &dir, &dist, &unused) != 0) {
            Vec3f step;
            TrailPuff *puff;
            ParticleDef54 spawn;
            PuffState puffState;

            step.x = dir.x * 20.0f;
            step.y = dir.y * 20.0f;
            step.z = dir.z * 20.0f;
            puffs = dist * 0.05000000075f;
            if (puffs > 1.0f) {
                f32 ageStep;

                ageStep = follow->trailAge / puffs;
                math_buildPerpendicularBasis(&delta, &puffState.axisU, &puffState.axisV);
                puffState.phaseSpeed = 0.4630000293f;
                puffState.sizeBase = 8.241000175f;
                puffState.sizeAmplitude = 3.888999939f;
                spawn.texture = 0x7A;
                spawn.type = 0;
                spawn.renderModes = 0x4404;
                spawn.lifetime = 10;
                spawn.frame = 0;
                spawn.frameSpeed = 0;
                spawn.rgba[0] = 0xFF;
                spawn.rgba[1] = 0xFF;
                spawn.rgba[2] = 0xFF;
                spawn.rgba[3] = 0xFF;
                spawn.height = 0.0f;
                spawn.width = 0.0f;
                spawn.rot = gZeroVec3f;
                spawn.scale.x = 1.0f;
                spawn.scale.y = 1.0f;
                spawn.scale.z = 1.0f;
                spawn.flags = 0xCC0008;
                spawn.unk45 = 0xFF;
                spawn.unk46 = 0;
                spawn.unk47 = 6;
                spawn.unk48 = 0;
                spawn.viewportMask = 0xFF;
                spawn.unk50 = 0;
                do {
                    const VtxColor *colors = gWhiteVtxColors;

                    puffState.phase = 4.712388992f;
                    puffState.timeLeft = 27.0f - follow->trailAge;
                    spawn.pos = follow->trailPos;
                    spawn.unk44 = puffState.timeLeft * 6.481481552f;
                    puff = particle_newQuadParticle(&spawn, colors, 0x24, 0, 0, 0x21, 0, 0, 0, 0x2C, obj->unkC, obj->unk1);
                    if (puff != NULL) {
                        memcpy(&puff->state, &puffState, sizeof(puffState));
                    }
                    follow->trailPos.x += step.x;
                    follow->trailPos.y += step.y;
                    follow->trailPos.z += step.z;
                    puffs -= 1.0f;
                    follow->trailAge -= ageStep;
                } while (puffs > 1.0f);
            }
        }
    }
    return 1;
}
