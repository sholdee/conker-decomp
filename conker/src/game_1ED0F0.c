#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_150E7FEC (game_113D60.c) is declared in each file that calls it: game_1E73B0.c and game_1F4650.c were
// compiled with a different view of it.
void func_150E7FEC(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);

/* Fills in the impact particle parameters; fewer particles on busy frames. */
void func_151BFC40(JetSpray *config, f32 *scale) {
    config->countBase = 3;
    config->countRange = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
    *scale = 494.0f;
    config->speedBase = 0.06089999899f;
    config->speedRange = 0.02520000003f;
    config->sizeBase = 45.0f;
    config->sizeRange = 53.0f;
    config->sizeSlopeBase = 203.0f;
    config->sizeSlopeRange = 414.0f;
    config->particleCountBase = 7;
    config->particleCountRange = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
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
    config->eventFunc = (D_80082FA0 >= 2 || D_8008FD8C >= 8) ? -1 : 0;
}


/* Bullet hit with no surface: sparks and smoke at pos, plus impact particles thrown back along -dir. */
void func_151BFDA0(Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    DirectedJetSpray fx;

    func_151C0418(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (withParticles) {
        fx.direction.x = -dir->x;
        fx.direction.y = -dir->y;
        fx.direction.z = -dir->z;
        func_151BFC40(&fx.spray, &fx.spread);
        fx.spray.pos = *pos;
        func_1514FB98(&fx, fxOwner, fxList);
    }
}

/* Bullet hit against a collision surface (or none). */
void func_151BFE84(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 withParticles, u8 fxOwner, s32 fxList) {
    Vec3f v0;
    Vec3f v1;
    Vec3f v2;

    // matching: taking the addresses keeps fxOwner/fxList in their stack slots (reloaded at each use)
    func_151C0418(pos, *(u8 *)&fxOwner, *(s32 *)&fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);
    if (surface != NULL && withParticles) {
        if (func_15144E80(surface, &v0, &v1, &v2) != 0 && v2.y < 0.0f) {
            func_151C0644(pos, fxOwner, fxList);
        }
    }
    if (surface != NULL && withParticles) {
        func_151C0360(surface, pos, fxOwner, fxList);
    }
    if (withParticles) {
        if (surface != NULL) {
            SurfaceJetSpray fx;

            fx.source.normal.x = -dir->x;
            fx.source.normal.y = -dir->y;
            fx.source.normal.z = -dir->z;
            fx.source.triangle = *surface;
            fx.spray.pos = *pos;
            func_151BFC40(&fx.spray, &fx.source.spread);
            func_1514FBFC(&fx, fxOwner, fxList);
        } else {
            DirectedJetSpray fx;

            fx.direction.x = -dir->x;
            fx.direction.y = -dir->y;
            fx.direction.z = -dir->z;
            func_151BFC40(&fx.spray, &fx.spread);
            fx.spray.pos = *pos;
            func_1514FB98(&fx, fxOwner, fxList);
        }
    }
}

/* Bullet hit on an object: sparks and debris at pos, plus a ground mark below it if hitObj is set. */
void func_151C0098(Vec3f *pos, Actor *hitObj, u8 withParticles, u8 fxOwner, s32 fxList) {
    JetSpraySpec fx;
    Vec3f probe;
    CollisionHit floor;
    Vec3f markPos;

    func_151C0418(pos, fxOwner, fxList);
    func_151C04F8(pos, fxOwner, fxList);
    func_151C05A4(pos, fxOwner, fxList);
    func_151C05F0(pos, fxOwner, fxList);

    // matching: taking the address keeps withParticles in its stack slot (reloaded at each use)
    if (*(u8 *)&withParticles) {
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
        func_1514FCE8(&fx, fxOwner, fxList);
    }

    if (withParticles && hitObj != NULL) {
        probe.x = pos->x;
        probe.y = pos->y + 100.0f;
        probe.z = pos->z;
        func_1504715C(&floor, hitObj);
        if (func_15046C80(&probe, 0, pos->y - 1000.0f, &floor) != 0) {
            markPos.x = probe.x;
            markPos.y = floor.height;
            markPos.z = probe.z;
            func_151C0360(&floor.tri, &markPos, fxOwner, fxList);
            func_151C0644(pos, fxOwner, fxList);
        }
    }
}

/* Whether obj is a valid target: not in the ignore list, not self, active, and with a valid id. */
s32 func_151C02E4(Actor *obj, Actor *self, s32 ignoreCount, Actor **ignore) {
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

void func_151C0360(CollisionTri *surface, Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { u32 w; f64 align; } shade;
    f32 rand;
    u32 duration;

    rand = func_150ADA68();
    shade.w = func_150ADA20();
    duration = func_150ADA20();
    func_150E7FEC(rand * 75.0f + 75.0f, (u8)(shade.w % 56 + 200), surface, pos,
                  duration % 205 + 299, 1, 1, 0, 0, 0, fxOwner, 0);
}

void func_151C0418(Vec3f *pos, u8 fxOwner, s32 fxList) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = func_150ADA20() % 11 + 20;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    func_151602C0(&header, &header2, func_150ADA20() % 121 + 60, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

void func_151C04F8(Vec3f *pos, u8 fxOwner, s32 fxList) {
    // matching: the f64 member gives the random value an 8-byte aligned stack slot
    union { f32 f; f64 align; } rand;
    u32 shade;

    rand.f = func_150ADA68();
    shade = func_150ADA20();
    func_151541B8(pos, rand.f * 4.0f + 14.0f, 1.182089925f, (f32)(shade % 56 + 200), 0.0f, fxOwner, fxList);
}

void func_151C05A4(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_151D5334(pos, 800.0f, 2000.0f, 0.0005f, 5, fxOwner, fxList);
}

void func_151C05F0(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_151D5404(pos, 800.0f, 2000.0f, 0.0005f, 15, 20, fxOwner, fxList);
}

void func_151C0644(Vec3f *pos, u8 fxOwner, s32 fxList) {
    func_150E83AC(pos, func_150ADA20() % 62 + 120, fxOwner, fxList);
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
void func_151C0698(Actor *shooter, Actor *aimer, IgnoreList *ignore, u8 impactType, u8 fxOwner, s32 fxList) {
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
    u8 scratch[0x14];
    BulletState state;
    ParticleSpawn desc;
    void *obj; // matching: one local (one stack slot) holds the camera, then the new bullet
    BulletState *bulletState;
    LightEffectHeader header;
    Vec3i header2;

    passDist = 0.0f;
    if (D_800BE616 != 0) {
        if (shooter->ob_matrix_ptr != NULL) {
            func_151C1798(shooter, &pos);
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
                extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

                angle = func_15144AA8(((Camera *)obj)->CNumber, obj) * 0.01745329238f;
                pos.x = cosf(angle) * 25.0f + src->x;
                pos.y = src->y;
                pos.z = src->z - sinf(angle) * 25.0f;
            } else if (shooter->ob_matrix_ptr != NULL) {
                func_151C1798(shooter, &pos);
            } else {
                pos.x = shooter->xpos;
                pos.y = shooter->ypos + 65.0f;
                pos.z = shooter->zpos;
            }
        } else if (shooter->ob_matrix_ptr != NULL) {
            func_151C1798(shooter, &pos);
        } else {
            pos.x = shooter->xpos;
            pos.y = shooter->ypos + 65.0f;
            pos.z = shooter->zpos;
        }
    }

    if ((shooter->mv != NULL && shooter->mv->SniperState != 0) || aimer != NULL) {
        extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

        who = (aimer != NULL) ? aimer : shooter;

        useAim = TRUE;
        src = &who->mv->aimEye;
        aimPos = *src;
        aimDir.x = who->mv->aimDir.x;
        aimDir.y = who->mv->aimDir.y;
        aimDir.z = who->mv->aimDir.z;
    } else {
        func_15145740(shooter, &dir, scratch, 0, 0.0f);
        useAim = FALSE;
        dir.x *= 0.001000000047f;
        dir.y *= 0.001000000047f;
        dir.z *= 0.001000000047f;
    }

    if (level == 36) {
        mode = 0;
    } else {
        mode = func_151D5B6C(useAim ? &aimPos : &pos, useAim ? &aimDir : &dir, shooter, 3, ignore);
    }

    if (func_151C196C(&ray, &pos, useAim ? NULL : &dir, useAim ? &aimPos : NULL,
                      useAim ? &aimDir : NULL, shooter, ignore, &passPos, &passDist, &pass,
                      mode) != 0) {
        extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

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
        func_15145974(&desc.vel, &desc.rot.y, &desc.rot.x);
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
        obj = func_15132A4C(&desc, 3, 0xFF, sizeof(BulletState), fxOwner, fxList);
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
            bulletState->trail = func_151602C0(&header, &header2, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
            bulletState->follow = (struct FollowFx *)func_151B8908(obj);
            bulletState->sound = func_1000FA64(func_150ADA20() % 3 + 750, pos.x, pos.y, pos.z, 30000, 1000, 500,
                                               (SoundRecordCallback)func_151C110C, obj, 0, 4, 0);
            bulletState->unk88 = (struct Bullet88 *)func_151B8400(obj);
            if (D_80082FA0 < 2 && D_8008FD8C < 4) {
                bulletState->unk8C = func_151B8668(obj, fxOwner, fxList);
            } else {
                extern u8 D_800BE616; // matching: a declaration makes this an IDO scope block (register choice)

                bulletState->unk8C = NULL;
            }
        } else {
            if (state.trail != NULL) {
                func_1516972C((EffectHeader *)state.trail);
            }
            if (state.follow != NULL) {
                func_1516972C(&state.follow->header);
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

                scale.width = func_150ADA68() * 10.0f + 25.0f;
                scale.height = func_150ADA68() * 1.009000063f + 2.013000011f;
                in[0] = &a;
                in[1] = &b;
                out[0] = &outA;
                out[1] = &outB;
                func_15145EA4(in, out, &shooter->ob_matrix_ptr[9], 2);
                func_15102B38(shooter, 9, &a, &c, &scale, func_150ADA20() % 3 + 8,
                              func_150ADA20() % 56 + 200, func_150ADA68() * 204.0f + 601.0f,
                              &outA, 0xFF, 0, -1, fxOwner, fxList);
                func_15103254(func_150ADA20() % 3 + 8, func_150ADA20() % 56 + 200,
                              func_150ADA68() * 204.0f + 601.0f, &outB, 0xFF, fxOwner, fxList);
                func_151C1860(&outB, fxOwner, fxList);
            }
        }
    }
}

/* Sound callback registered by func_151C0698: keeps the bullet's sound at the bullet's position. */
s32 func_151C110C(SoundEmitter *emitter, s32 arg1, void **arg2, s32 arg3, s32 arg4, s32 arg5, s16 *arg6) {
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
s32 func_151C1180(Bullet *bullet) {
    BulletState *state;
    f32 step;
    Vec3f delta;
    Vec3f hitPos;
    RayHit hit;
    LightEffect *trail;

    state = &bullet->state;
    if (state->lifetime < D_800BE9A4) {
        step = state->speed * state->lifetime;
    } else {
        step = state->speed * D_800BE9A4;
    }
    delta.x = state->ray.dir.x * step;
    delta.y = state->ray.dir.y * step;
    delta.z = state->ray.dir.z * step;
    if (D_800E0930 != NULL) {
        if (((s32 (*)(Vec3f *, Vec3f *, Vec3f *))D_800E0930)(&bullet->head.pos, &delta, &hitPos) != 0) {
            func_15055A2C(0, hitPos.x, hitPos.y, hitPos.z, 0);
            func_151C1654(&hitPos, &state->ray.dir, state->owner, state->impactType, NULL);
            func_151BFDA0(&hitPos, &state->ray.dir, !(state->flags & 1), bullet->head.header.owner, bullet->head.header.kind);
            return FALSE;
        }
    }
    if (step) {
    fence: // matching: an unused label splits IDO's basic block here
        func_15081690(state->owner, bullet->head.pos.x, bullet->head.pos.y, bullet->head.pos.z, delta.x, delta.y, delta.z,
                      &hit, step, 1, 0, 1, 3, &state->ignore, state->mode);

        if (hit.hitType >= 2) {
            func_15055A2C(1, hit.pos.x, hit.pos.y, hit.pos.z, 0);
            func_151C1654(&hit.pos, &state->ray.dir, state->owner, state->impactType, NULL);
            func_151C0098(&hit.pos, hit.obj, !(state->flags & 1), bullet->head.header.owner, bullet->head.header.kind);
            return FALSE;
        }
    }
    bullet->head.pos.x += bullet->head.vel.x * D_800BE9A4;
    bullet->head.pos.y += bullet->head.vel.y * D_800BE9A4;
    bullet->head.pos.z += bullet->head.vel.z * D_800BE9A4;
    if (state->flags & 1) {
        if (state->passTimer > 0.0f) {
            state->passTimer -= D_800BE9A4;
            if (state->passTimer <= 0.0f) {
                func_151ABE40(&state->passPos, &state->pass, 2, bullet->head.header.owner, bullet->head.header.kind);
            }
        }
    }
    state->lifetime -= D_800BE9A4;
    if (state->lifetime <= 0.0f) {
        if (state->ray.hitType == 1) {
            func_15055A2C(0, state->ray.pos.x, state->ray.pos.y, state->ray.pos.z, 0);
            func_151C1654(&state->ray.pos, &state->ray.dir, state->owner, state->impactType, &state->ray.tri);
            func_151BFE84(&state->ray.tri, &state->ray.pos, &bullet->head.vel, !(state->flags & 1),
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
void func_151C1570(Bullet *bullet) {
    BulletState *state;

    state = &bullet->state;
    if (state->trail != NULL) {
        func_1516972C((EffectHeader *)state->trail);
    }
    if (state->follow != NULL) {
        func_1516972C(&state->follow->header);
    }
    if (state->unk88 != NULL) {
        // matching: the pointer temp gets its own stack slot, which the frame size needs
        s32 *flag = &state->unk88->unk110;

        *flag = 0;
    }
    if (state->unk8C != NULL) {
        func_1516972C(state->unk8C);
    }
    func_1000FD38((SoundRecordCallback)func_151C110C, bullet, 0);
}

void func_151C15FC(Bullet *bullet) {
    func_151C1570(bullet);
    func_15132570(bullet);
}

void func_151C1628(Bullet *bullet) {
    func_151C1570(bullet);
    func_1513259C(bullet);
}

/* Impact sound/decal for a bullet hit, chosen by impactType. */
void func_151C1654(Vec3f *pos, Vec3f *dir, Actor *owner, u8 impactType, CollisionTri *surface) {
    void (*callback)(s32, s32, s32);

    callback = (void (*)(s32, s32, s32))D_800E0934;
    if (callback != NULL) {
        callback(pos->x, pos->y, pos->z);
    }

    switch (impactType) {
    case 1:
        func_151D42E8(pos, dir, owner, surface, 36);
        return;
    case 2:
        func_151D42E8(pos, dir, owner, surface, 37);
        return;
    case 3:
        func_151D40D4(pos, dir, owner, 0, surface, 22, 38, 0);
        return;
    case 0:
    default:
        func_151D40D4(pos, dir, owner, 0, surface, 22, 21, 0);
        return;
    }
}

/* Bone index per muzzle. */
const u8 D_800AA954[3] = { 9, 4, 4 };
/* Muzzle offset in bone space. */
const Vec3f D_800AA958[3] = {
    { 5.0f, 29.0f, 37.0f },
    { 0.0f, 0.0f, 70.0f },
    { 0.0f, 0.0f, 70.0f },
};

/* World position of obj's gun muzzle, from its bone matrices. */
void func_151C1798(Actor *obj, Vec3f *pos) {
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
    func_15143134(&D_800AA958[index], pos, &obj->ob_matrix_ptr[D_800AA954[index]]);
}

/* Event handler: event 0x2D hands the bullet's ownership between the two objects in pair. */
void func_151C1814(Bullet *bullet, Actor **pair, u8 event) {
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
    header.lifetime = func_150ADA20() % 5 + 5;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    func_151602C0(&header, &header2, func_150ADA20() % 11 + 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, fxOwner, fxList);
}

/* Target filter callback: func_151C02E4 with the ignore list packed in a struct. */
s32 func_151C1940(Actor *obj, Actor *self, IgnoreList *ignore) {
    return func_151C02E4(obj, self, ignore->count, ignore->objs);
}

/*
 * Traces a bullet from start along dir. With an aim view (aimPos/aimDir), first traces the view ray and
 * then re-aims from start at whatever it hit. On a miss, ray describes a point 5000 units along the
 * direction. Returns FALSE only when re-aiming fails.
 */
s32 func_151C196C(RayHit *ray, Vec3f *start, Vec3f *dir, Vec3f *aimPos, Vec3f *aimDir,
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
        func_151C1D5C(shooter, &ray->origin, &ray->dir, ray, 0.0f, 0, 0, 1, 3, ignore, passPos,
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
                if (func_15145128(&ray->dir, &ray->dir, &ray->dist, NULL) == 0) {
                    return FALSE;
                }
            }
        }
    } else {
        ray->origin = *start;
        ray->dir = *dir;
    }

    if (trace) {
        func_151C1D5C(shooter, &ray->origin, &ray->dir, ray, 0.0f, 0, 1, 1, 3, ignore, passPos,
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
void func_151C1D5C(Actor *shooter, Vec3f *start, Vec3f *dir, RayHit *ray, f32 arg4,
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
        func_15081690(shooter, pos.x, pos.y, pos.z, dir->x, dir->y, dir->z, ray, arg4, arg5,
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


void func_151C1FB8(Actor *obj) {
    RumbleParams event;

    if (obj->CameraPtr != NULL) {
        event.flags = 1;
        event.duration = (func_150ADA20() & 7) + 13;
        event.playerMask = 1 << obj->CameraPtr->CNumber;
        event.strength = func_150ADA20() % 3 + 6;
        event.updateKind = -1;
        func_151D8868(&event, 0, 0xFF, 1);
    }
}
