#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Level hooks, installed by the per-level setup (game_3DB60.c, game/Level/LevelSetup18/levelsetup18.c) and cleared by game_initLevelSubsystems. */
s32 (*D_800E0930)(Vec3f *start, Vec3f *delta, Vec3f *hit);
void (*D_800E0934)(s32 x, s32 y, s32 z);

// particle_spawnImpactMark (game/Particle/EmissionSource/emissionsource.c) is declared in each file that calls it: game/Effects/FollowEffects/followeffects.c and game/Sight/SightBeam/sightbeam.c were
// compiled with a different view of it.
void particle_spawnImpactMark(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);

/* Fills in the impact particle parameters; fewer particles on busy frames. */
void weapon_initImpactSpray(JetSpray *config, f32 *scale) {
    config->countBase = 3;
    config->countRange = 4 >> ((gScreenLayout >= 2) ? 1 : 0);
    *scale = 494.0f;
    config->speedBase = 0.06089999899f;
    config->speedRange = 0.02520000003f;
    config->sizeBase = 45.0f;
    config->sizeRange = 53.0f;
    config->sizeSlopeBase = 203.0f;
    config->sizeSlopeRange = 414.0f;
    config->particleCountBase = 7;
    config->particleCountRange = 3 >> ((gScreenLayout >= 2 || D_8008FD8C >= 8) ? 1 : 0);
    config->spread = 15.0f;
    config->drag = 0.8800000548f;
    config->gravityBase = -0.3980000317f;
    config->gravityRange = 0.3910000324f;
    config->delayBase = 25;
    config->delayRange = 15;
    config->alphaBase = 100;
    config->alphaRange = 100;
    config->fadeStart = 12;
    config->fadeOutStart = 20;
    config->eventFunc = (gScreenLayout >= 2 || D_8008FD8C >= 8) ? -1 : 0;
}


/* Bullet hit with no surface: sparks and smoke at pos, plus impact particles thrown back along -dir. */
void weapon_spawnAirHit(Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    DirectedJetSpray fx;

    weapon_spawnHitLight(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (withParticles) {
        fx.direction.x = -dir->x;
        fx.direction.y = -dir->y;
        fx.direction.z = -dir->z;
        weapon_initImpactSpray(&fx.spray, &fx.spread);
        fx.spray.pos = *pos;
        particle_spawnDirectedJetSpray(&fx, fxOwner, fxList);
    }
}

/* Bullet hit against a collision surface (or none). */
void weapon_spawnSurfaceHit(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    Vec3f v0;
    Vec3f v1;
    Vec3f v2;

    // matching: taking the addresses keeps fxOwner/fxList in their stack slots (reloaded at each use)
    weapon_spawnHitLight(pos, *(u8 *)&fxOwner, *(s32 *)&fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (surface != NULL && withParticles) {
        if (math_getTriEdges(surface, &v0, &v1, &v2) != 0 && v2.y < 0.0f) {
            func_151C0644(pos, fxOwner, fxList);
        }
    }
    if (surface != NULL && withParticles) {
        weapon_spawnHitDecal(surface, pos, fxOwner, fxList);
    }
    if (withParticles) {
        if (surface != NULL) {
            SurfaceJetSpray fx;

            fx.source.normal.x = -dir->x;
            fx.source.normal.y = -dir->y;
            fx.source.normal.z = -dir->z;
            fx.source.triangle = *surface;
            fx.spray.pos = *pos;
            weapon_initImpactSpray(&fx.spray, &fx.source.spread);
            particle_spawnSurfaceJetSpray(&fx, fxOwner, fxList);
        } else {
            DirectedJetSpray fx;

            fx.direction.x = -dir->x;
            fx.direction.y = -dir->y;
            fx.direction.z = -dir->z;
            weapon_initImpactSpray(&fx.spray, &fx.spread);
            fx.spray.pos = *pos;
            particle_spawnDirectedJetSpray(&fx, fxOwner, fxList);
        }
    }
}

/* Bullet hit on an object: sparks and debris at pos, plus a ground mark below it if hitObj is set. */
void weapon_spawnObjectHit(Vec3f *pos, Actor *hitObj, u8 withParticles, u8 fxOwner, s32 fxList) {
    JetSpraySpec fx;
    Vec3f probe;
    CollisionHit floor;
    Vec3f markPos;

    weapon_spawnHitLight(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);

    // matching: taking the address keeps withParticles in its stack slot (reloaded at each use)
    if (*&withParticles) {
        fx.angles.yawBase = 0;
        fx.angles.yawRange = 255;
        fx.angles.pitchBase = -64;
        fx.angles.pitchRange = 71;
        fx.spray.countBase = 6;
        fx.spray.countRange = 4;
        fx.spray.pos = *pos;
        fx.spray.speedBase = 23.0f;
        fx.spray.speedRange = 30.0f;
        fx.spray.sizeBase = 45.0f;
        fx.spray.sizeRange = 53.0f;
        fx.spray.sizeSlopeBase = 203.0f;
        fx.spray.sizeSlopeRange = 414.0f;
        fx.spray.particleCountBase = 7;
        fx.spray.particleCountRange = 3;
        fx.spray.spread = 15.0f;
        fx.spray.drag = 0.8720000386f;
        fx.spray.gravityBase = -0.3980000317f;
        fx.spray.gravityRange = 0.3640000224f;
        fx.spray.delayBase = 25;
        fx.spray.delayRange = 15;
        fx.spray.alphaBase = 100;
        fx.spray.alphaRange = 100;
        fx.spray.fadeStart = 12;
        fx.spray.fadeOutStart = 20;
        fx.spray.eventFunc = 0;
        particle_spawnJetSprayBursts(&fx, fxOwner, fxList);
    }

    if (withParticles && hitObj != NULL) {
        probe.x = pos->x;
        probe.y = pos->y + 100.0f;
        probe.z = pos->z;
        collision_initHitFromObject(&floor, hitObj);
        if (collision_probeFloorSurfABSet0Cached(&probe, 0, pos->y - 1000.0f, &floor) != 0) {
            markPos.x = probe.x;
            markPos.y = floor.height;
            markPos.z = probe.z;
            weapon_spawnHitDecal(&floor.tri, &markPos, fxOwner, fxList);
            func_151C0644(pos, fxOwner, fxList);
        }
    }
}

/* Whether obj is a valid target: not in the ignore list, not self, active, and with a valid id. */
s32 weapon_isBulletTarget(Actor *obj, Actor *self, s32 ignoreCount, Actor **ignore) {
    u8 i;

    for (i = 0; i < ignoreCount; i++) {
        if (obj == ignore[i]) {
            return FALSE;
        }
    }
    if (obj == self) {
        return FALSE;
    }
    if (obj->type == 0) {
        return FALSE;
    }
    if (obj->character_type == 0xFF) {
        return FALSE;
    }
    return TRUE;
}

void weapon_spawnHitDecal(CollisionTri *surface, Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { u32 w; f64 align; } shade;
    f32 rand;
    u32 duration;

    rand = math_randomFloat();
    shade.w = math_random();
    duration = math_random();
    particle_spawnImpactMark(rand * 75.0f + 75.0f, shade.w % 56 + 200, surface, pos,
                  duration % 205 + 299, 1, 1, 0, 0, 0, fxOwner, 0);
}

void weapon_spawnHitLight(Vec3f *pos, u8 fxOwner, s32 fxList) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = math_random() % 11 + 20;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    light_createWithEffect(&header, &header2, math_random() % 121 + 60, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

void func_151C04F8(Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { f32 f; f64 align; } rand;
    u32 shade;

    rand.f = math_randomFloat();
    shade = math_random();
    particle_spawnGrowFade(pos, rand.f * 4.0f + 14.0f, 1.182089925f, shade % 56 + 200, 0.0f, fxOwner, fxList);
}

void func_151C05A4(Vec3f *pos, u8 fxOwner, s32 fxList) {
    particle_startPlayerCameraWobbles(pos, 800.0f, 2000.0f, 0.0005f, 5, fxOwner, fxList);
}

void func_151C05F0(Vec3f *pos, u8 fxOwner, s32 fxList) {
    particle_startPlayerRumbles(pos, 800.0f, 2000.0f, 0.0005f, 15, 20, fxOwner, fxList);
}

void func_151C0644(Vec3f *pos, u8 fxOwner, s32 fxList) {
    particle_startRandomEmitter(pos, math_random() % 62 + 120, fxOwner, fxList);
}


typedef struct FollowFx {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x30];
    /* 0x40 */ Vec3f pos;
} FollowFx;

typedef struct Bullet88 {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 unk110;
} Bullet88;

/* Fires a bullet from shooter (aimed by aimer's view, if given) and spawns its object and effects. */
void weapon_fireBullet(Actor *shooter, Actor *aimer, IgnoreList *ignore, u8 impactType, u8 fxOwner, s32 fxList) {
    RayHit ray;
    f32 passDist;
    Vec3f passPos;
    CollisionHit pass;
    u8 useAim;
    Vec3f pos;
    Vec3f dir;
    Vec3f aimPos;
    Vec3f aimDir;
    s32 mode;
    Vec3f *src;
    Actor *who;
    f32 angle;
    u8 pad[8]; // matching: stack space
    Vec3f scratch; // math_getActorAim's unused dir
    BulletState state;
    ParticleSpawn desc;
    void *obj; // matching: one local (one stack slot) holds the camera, then the new bullet
    BulletState *bulletState;
    LightEffectHeader header;
    Vec3i header2;

    passDist = 0.0f;
    if (gIsMultiplayer != 0) {
        if (shooter->ob_matrix_ptr != NULL) {
            weapon_getBulletMuzzlePos(shooter, &pos);
        } else {
            pos.x = shooter->xpos;
            pos.y = shooter->ypos + 65.0f;
            pos.z = shooter->zpos;
        }
    } else {
        src = &shooter->mv->aimEye;
        obj = shooter->CameraPtr;
        if (obj != NULL) {
            if (shooter->attach_char != 0) {
lbl1: // matching: an unused label splits IDO's basic block here (register choice)
                pos.x = src->x;
                pos.y = src->y + -13.0f;
                pos.z = src->z;
            } else if (shooter->mv->SniperState != 0) {
                extern u8 gIsMultiplayer; // matching: a declaration makes this an IDO scope block (register choice)

                angle = math_getCameraYaw(((Camera *)obj)->CNumber, obj) * 0.01745329238f;
                pos.x = cosf(angle) * 25.0f + src->x;
                pos.y = src->y;
                pos.z = src->z - sinf(angle) * 25.0f;
            } else if (shooter->ob_matrix_ptr != NULL) {
                weapon_getBulletMuzzlePos(shooter, &pos);
            } else {
                pos.x = shooter->xpos;
                pos.y = shooter->ypos + 65.0f;
                pos.z = shooter->zpos;
            }
        } else if (shooter->ob_matrix_ptr != NULL) {
            weapon_getBulletMuzzlePos(shooter, &pos);
        } else {
            pos.x = shooter->xpos;
            pos.y = shooter->ypos + 65.0f;
            pos.z = shooter->zpos;
        }
    }

    if ((shooter->mv != NULL && shooter->mv->SniperState != 0) || aimer != NULL) {
        extern u8 gIsMultiplayer; // matching: a declaration makes this an IDO scope block (register choice)

        who = (aimer != NULL) ? aimer : shooter;

        useAim = TRUE;
        src = &who->mv->aimEye;
        aimPos = *src;
        aimDir.x = who->mv->aimDir.x;
        aimDir.y = who->mv->aimDir.y;
        aimDir.z = who->mv->aimDir.z;
    } else {
        math_getActorAim(shooter, &dir, &scratch, 0, 0.0f);
        useAim = FALSE;
        dir.x *= 0.001000000047f;
        dir.y *= 0.001000000047f;
        dir.z *= 0.001000000047f;
    }

    if (level == 36) {
        mode = 0;
    } else {
        mode = particle_raySphereObjectMask(useAim ? &aimPos : &pos, useAim ? &aimDir : &dir, shooter, 3, ignore);
    }

    if (weapon_traceBullet(&ray, &pos, useAim ? NULL : &dir, useAim ? &aimPos : NULL,
                      useAim ? &aimDir : NULL, shooter, ignore, &passPos, &passDist, &pass,
                      mode) != 0) {
        extern u8 gIsMultiplayer; // matching: a declaration makes this an IDO scope block (register choice)

        state.mode = mode;
        state.trail = NULL;
        state.ray = ray;
        state.follow = NULL;
        desc.radius = 1.0f;
        desc.elasticity = 1.0f;
        desc.sizeXZ = 0.04700000212f;
        desc.sizeY = 0.04700000212f;
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.pos = pos;
        desc.gravity = 0.0f;
        desc.flags = 0x500920;
        desc.lifetime = 300;
        desc.model = 47;
        desc.variant = 1;
        state.owner = shooter;
        state.ignore = *ignore;
        state.passTimer = passDist * 0.004999999888f;
        state.passPos = passPos;
        state.flags = (s8)((passDist != 0) ? 1 : 0);
        state.pass = pass;
        state.speed = 200.0f;
        desc.vel.x = state.ray.dir.x * state.speed;
        desc.vel.y = state.ray.dir.y * state.speed;
        desc.vel.z = state.ray.dir.z * state.speed;
        math_getHeadingElevation(&desc.vel, &desc.rot.y, &desc.rot.x);
        desc.rot.z = 0.0f;
        desc.rotVel.x = 0.0f;
        desc.rotVel.y = 0.0f;
        desc.rotVel.z = 0.0f;
        state.lifetime = state.ray.dist * 0.004999999888f;
        state.impactType = impactType;
        desc.unk5C = 0;
        desc.alpha = 255;
        desc.updateFn = 10;
        desc.update2Fn = 0;
        desc.floorHitFn = 0;
        desc.floorType3Fn = 0;
        desc.wallHitFn = 0;
        desc.destroyFn = 0;
        desc.squashFn = 0;
        desc.matrixFn = 2;
        desc.unk6A = 2;
        desc.owner = 0;
        desc.ownerId = 0;
        desc.fadeStart = 1;
        desc.fadeRate = 255;
        obj = particle_newModelParticle(&desc, 3, 0xFF, sizeof(BulletState), fxOwner, fxList);
        if (obj != NULL) {
            bulletState = &((Bullet *)obj)->state;
            memcpy(bulletState, &state, sizeof(BulletState));
            header.flags = 2;
            header.update = -1;
            header.lifetime = 300;
            header.lightType = 0;
            header2.x = pos.x;
            header2.y = pos.y;
            header2.z = pos.z;
            bulletState->trail = light_createWithEffect(&header, &header2, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
            bulletState->follow = (struct FollowFx *)weapon_spawnBulletSprite(obj);
            bulletState->sound = sound_addDynamicSound(math_random() % 3 + 750, pos.x, pos.y, pos.z, 30000, 1000, 500,
                                               (SoundRecordCallback)weapon_bulletSoundCallback, obj, 0, 4, 0);
            bulletState->unk88 = (struct Bullet88 *)weapon_spawnBulletParticle(obj);
            if (gScreenLayout < 2 && D_8008FD8C < 4) {
                bulletState->unk8C = weapon_startBulletTrail(obj, fxOwner, fxList);
            } else {
                extern u8 gIsMultiplayer; // matching: a declaration makes this an IDO scope block (register choice)

                bulletState->unk8C = NULL;
            }
        } else {
            if (state.trail != NULL) {
                effect_destroyObject(state.trail);
            }
            if (state.follow != NULL) {
                effect_destroyObject(&state.follow->header);
            }
        }
        if (shooter->ob_matrix_ptr != NULL && (shooter->unk74 & 0xF) != 0xF) {
            if (shooter->character_type != 0x77 && shooter->character_type != 0x28) {
                const Vec3f a = { 3.0f, 125.0f, 35.0f };
                const Vec3f b = { 6.0f, -50.0f, 35.0f };
                const Vec3f c = { 3.0f, 200.0f, 35.0f };
                ParticleSize scale;
                Vec3f outA;
                Vec3f outB;
                const Vec3f *in[2];
                Vec3f *out[2];

                scale.width = math_randomFloat() * 10.0f + 25.0f;
                scale.height = math_randomFloat() * 1.009000063f + 2.013000011f;
                in[0] = &a;
                in[1] = &b;
                out[0] = &outA;
                out[1] = &outB;
                math_transformOffsets(in, out, &shooter->ob_matrix_ptr[9], 2);
                effect_spawnBoneBeamParticle(shooter, 9, &a, &c, &scale, math_random() % 3 + 8,
                              math_random() % 56 + 200, math_randomFloat() * 204.0f + 601.0f,
                              &outA, 0xFF, 0, -1, fxOwner, fxList);
                effect_spawnFlashParticle(math_random() % 3 + 8, math_random() % 56 + 200,
                              math_randomFloat() * 204.0f + 601.0f, &outB, 0xFF, fxOwner, fxList);
                func_151C1860(&outB, fxOwner, fxList);
            }
        }
    }
}

/* Sound callback registered by weapon_fireBullet: keeps the bullet's sound at the bullet's position. */
s32 weapon_bulletSoundCallback(SoundEmitter *emitter, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
    Bullet *bullet;

    bullet = emitter->bullet;
    if (bullet != NULL && *arg2 != NULL) {
        emitter->x = bullet->head.pos.x;
        emitter->y = bullet->head.pos.y;
        emitter->z = bullet->head.pos.z;
        return 0;
    }

    *arg6 = 0;
    return 0;
}

/* Per-frame bullet update: moves it, traces for hits and spawns the impact. Returns FALSE when it is done. */
s32 weapon_updateBullet(Bullet *bullet) {
    BulletState *state;
    f32 step;
    Vec3f delta;
    Vec3f hitPos;
    RayHit hit;
    LightEffect *trail;

    state = &bullet->state;
    if (state->lifetime < gFrameDelta) {
        step = state->speed * state->lifetime;
    } else {
        step = state->speed * gFrameDelta;
    }
    delta.x = state->ray.dir.x * step;
    delta.y = state->ray.dir.y * step;
    delta.z = state->ray.dir.z * step;
    if (D_800E0930 != NULL) {
        if (D_800E0930(&bullet->head.pos, &delta, &hitPos) != 0) {
            func_15055A2C(0, hitPos.x, hitPos.y, hitPos.z, 0);
            weapon_runImpactAttack(&hitPos, &state->ray.dir, state->owner, state->impactType, NULL);
            weapon_spawnAirHit(&hitPos, &state->ray.dir, !(state->flags & 1), bullet->head.header.owner, bullet->head.header.kind);
            return FALSE;
        }
    }
    if (step) {
    fence: // matching: an unused label splits IDO's basic block here
        collision_castRay(state->owner, bullet->head.pos.x, bullet->head.pos.y, bullet->head.pos.z, delta.x, delta.y, delta.z,
                      &hit, step, 1, 0, 1, 3, &state->ignore, state->mode);

        if (hit.hitType >= 2) {
            func_15055A2C(1, hit.pos.x, hit.pos.y, hit.pos.z, 0);
            weapon_runImpactAttack(&hit.pos, &state->ray.dir, state->owner, state->impactType, NULL);
            weapon_spawnObjectHit(&hit.pos, hit.obj, !(state->flags & 1), bullet->head.header.owner, bullet->head.header.kind);
            return FALSE;
        }
    }
    bullet->head.pos.x += bullet->head.vel.x * gFrameDelta;
    bullet->head.pos.y += bullet->head.vel.y * gFrameDelta;
    bullet->head.pos.z += bullet->head.vel.z * gFrameDelta;
    if (state->flags & 1) {
        if (state->passTimer > 0.0f) {
            state->passTimer -= gFrameDelta;
            if (state->passTimer <= 0.0f) {
                effect_spawnImpact(&state->passPos, &state->pass, 2, bullet->head.header.owner, bullet->head.header.kind);
            }
        }
    }
    state->lifetime -= gFrameDelta;
    if (state->lifetime <= 0.0f) {
        if (state->ray.hitType == 1) {
            func_15055A2C(0, state->ray.pos.x, state->ray.pos.y, state->ray.pos.z, 0);
            weapon_runImpactAttack(&state->ray.pos, &state->ray.dir, state->owner, state->impactType, &state->ray.tri);
            weapon_spawnSurfaceHit(&state->ray.tri, &state->ray.pos, &bullet->head.vel, !(state->flags & 1),
                          bullet->head.header.owner, bullet->head.header.kind);
        }
        return FALSE;
    }
    trail = state->trail;
    if (trail != NULL) {
        trail->light->x = bullet->head.pos.x;
        if ((bullet->head.header.kind && bullet->head.header.kind) && bullet->head.header.kind) { // matching: no-op that steers register choice for trail
        }
        trail->light->y = bullet->head.pos.y;
        trail->light->z = bullet->head.pos.z;
    }
    if (state->follow != NULL) {
        state->follow->pos = bullet->head.pos;
    }
    return TRUE;
}

/* Releases a bullet's effects and its sound callback. */
void weapon_releaseBullet(Bullet *bullet) {
    BulletState *state;

    state = &bullet->state;
    if (state->trail != NULL) {
        effect_destroyObject(state->trail);
    }
    if (state->follow != NULL) {
        effect_destroyObject(&state->follow->header);
    }
    if (state->unk88 != NULL) {
        // matching: the pointer temp gets its own stack slot, which the frame size needs
        s32 *flag = &state->unk88->unk110;

        *flag = 0;
    }
    if (state->unk8C != NULL) {
        effect_destroyObject(state->unk8C);
    }
    sound_stopByCallback((SoundRecordCallback)weapon_bulletSoundCallback, bullet, 0);
}

void func_151C15FC(Bullet *bullet) {
    weapon_releaseBullet(bullet);
    particle_destroyModelParticleDefault(&bullet->head);
}

void func_151C1628(Bullet *bullet) {
    weapon_releaseBullet(bullet);
    particle_freeModelParticleDefault(&bullet->head);
}

/* Impact sound/decal for a bullet hit, chosen by impactType. */
void weapon_runImpactAttack(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, CollisionTri *surface) {
    void (*callback)(s32, s32, s32);

    callback = D_800E0934;
    if (callback != NULL) {
        callback(pos->x, pos->y, pos->z);
    }

    switch (impactType) {
    case 1:
        particle_runAttackAlongDir(pos, dir, owner, surface, 36);
        return;
    case 2:
        particle_runAttackAlongDir(pos, dir, owner, surface, 37);
        return;
    case 3:
        particle_runAttackPairAt(pos, dir, owner, 0, surface, 22, 38, 0);
        return;
    case 0:
    default:
        particle_runAttackPairAt(pos, dir, owner, 0, surface, 22, 21, 0);
        return;
    }
}

/* Bone index per muzzle. */
const u8 gBulletMuzzleBones[3] = { 9, 4, 4 };
/* Muzzle offset in bone space. */
const Vec3f gBulletMuzzleOffsets[3] = {
    { 5.0f, 29.0f, 37.0f },
    { 0.0f, 0.0f, 70.0f },
    { 0.0f, 0.0f, 70.0f },
};

/* World position of obj's gun muzzle, from its bone matrices. */
void weapon_getBulletMuzzlePos(Actor *obj, Vec3f *pos) {
    s32 index;

    switch (obj->character_type) {
    case 0x77:
        index = 1;
        break;
    case 0x28:
        index = 2;
        break;
    default:
        index = 0;
        break;
    }
    math_transformOffset(&gBulletMuzzleOffsets[index], pos, &obj->ob_matrix_ptr[gBulletMuzzleBones[index]]);
}

/* Event handler: event 0x2D hands the bullet's ownership between the two objects in pair. */
void weapon_onBulletPairEvent(Bullet *bullet, Actor **pair, u8 event) {
    BulletState *state = &bullet->state;
    if (event == 0x2D) {
        // matching: the indexed form state[0] keeps IDO's compare operands in source order
        if (pair[0] == state[0].owner) {
            state->owner = pair[1];
        } else if (pair[1] == state[0].owner) {
            state->owner = pair[0];
        }
    }
}

void func_151C1860(Vec3f *pos, u8 fxOwner, s32 fxList) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = math_random() % 5 + 5;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    light_createWithEffect(&header, &header2, math_random() % 11 + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

/* Target filter callback: weapon_isBulletTarget with the ignore list packed in a struct. */
s32 weapon_filterBulletTarget(Actor *obj, Actor *self, IgnoreList *ignore) {
    return weapon_isBulletTarget(obj, self, ignore->count, ignore->objs);
}

/*
 * Traces a bullet from start along dir. With an aim view (aimPos/aimDir), first traces the view ray and
 * then re-aims from start at whatever it hit. On a miss, ray describes a point 5000 units along the
 * direction. Returns FALSE only when re-aiming fails.
 */
s32 weapon_traceBullet(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
                  Actor *shooter, IgnoreList *ignore, Vec3f *passPos, f32 *passDist,
                  CollisionHit *pass, s32 mode) {
    u8 trace;
    u8 hit;
    Vec3f *src;

    trace = TRUE;
    hit = FALSE;

    if (aimPos != NULL && aimDir != NULL) {
        ray->origin = *aimPos;
        ray->dir = *aimDir;
        weapon_castPiercingRay(shooter, &ray->origin, &ray->dir, ray, 0.0f, 0, 0, 1, 3, ignore, passPos,
                      passDist, pass, mode);

        if (ray->hitType == 0) {
            if (start->x != ray->origin.x || start->y != ray->origin.y || start->z != ray->origin.z) {
                ray->origin = *start;
                ray->dir = *aimDir;
            } else {
                trace = FALSE;
            }
        } else {
            if (start->x != ray->origin.x || start->y != ray->origin.y || start->z != ray->origin.z) {
                ray->origin = *start;
                ray->dir.x = ray->pos.x - start->x;
                ray->dir.y = ray->pos.y - start->y;
                ray->dir.z = ray->pos.z - start->z;
                if (math_vec3Normalize(&ray->dir, &ray->dir, &ray->dist, NULL) == 0) {
                    return FALSE;
                }
            }
        }
    } else {
        ray->origin = *start;
        ray->dir = *dir;
    }

    if (trace) {
        weapon_castPiercingRay(shooter, &ray->origin, &ray->dir, ray, 0.0f, 0, 1, 1, 3, ignore, passPos,
                      passDist, pass, mode);
        if (ray->hitType != 0) {
            hit = TRUE;
        }
    }

    if (!hit) {
        ray->obj = NULL;
        ray->dist = 5000.0f;
        ray->origin = *start;
        if (aimDir != NULL) {
            src = aimDir;
        } else {
            src = dir;
        }
        ray->dir = *src;
        ray->pos.x = ray->origin.x + ray->dir.x * 5000.0f;
        ray->pos.y = ray->origin.y + ray->dir.y * 5000.0f;
        ray->pos.z = ray->origin.z + ray->dir.z * 5000.0f;
        ray->centre = ray->pos;
        ray->far = ray->pos;
        ray->bone = 0;
        ray->part = 0;
        ray->hitType = 0;
        ray->index = 0;
        ray->surface = NULL;
        ray->material = 0;
    }
    return TRUE;
}

/*
 * Ray cast that continues through pass-through surfaces; the first one it passes is reported in
 * passPos/passDist/pass. ray->dist is the total distance travelled.
 */
void weapon_castPiercingRay(Actor *shooter, Vec3f *start, Vec3f *dir, RayHit *ray, f32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s8 arg8, IgnoreList *ignore, Vec3f *passPos,
                   f32 *passDist, CollisionHit *pass, s32 mode) {
    WorldObject *part;
    Vec3f pos;
    f32 dist;
    s32 more;

    dist = 0.0f;
    *passDist = 0.0f;
    pos = *start;

    do {
        collision_castRay(shooter, pos.x, pos.y, pos.z, dir->x, dir->y, dir->z, ray, arg4, arg5,
                      arg6, arg7, arg8, ignore, mode);
        dist += ray->dist;
        if (ray->surface != NULL) {
            WorldObject *p = ray->surface;

            if ((p->object_status & 0x60) == 0x40) {
                pos.x = ray->pos.x + ray->dir.x * 0.1f;
                part = p;
                pos.y = ray->pos.y + ray->dir.y * 0.1f;
                pos.z = ray->pos.z + ray->dir.z * 0.1f;
                *passPos = ray->pos;
                *passDist = dist;
                dist += 0.1f;
                pass->height = ray->pos.y;
                pass->tri = ray->tri;
                pass->material = ray->material;
                pass->flags = 7;
                pass->source = 3;
                pass->surface = part;
                more = TRUE;
            } else {
                more = FALSE;
            }
        } else {
            more = FALSE;
        }
    } while (more);

    ray->origin = *start;
    ray->dist = dist;
}


void weapon_rumbleShooter(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (math_random() & 7) + 13;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        event.strength = math_random() % 3 + 6;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 1);
    }
}
