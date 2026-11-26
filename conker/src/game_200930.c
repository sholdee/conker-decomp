#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_15D730.c as void particle_applyDrag(Vec3f *vel, f32 drag). This file was compiled with a
   view that takes a third argument. */
void particle_applyDrag(void *vel, f32 drag, void *arg2);

/* The point functions of particle_attachPointEmitter's emitters, indexed by ObjEmitter.posFunc (ObjEmitter.unk6 is
 * their second argument). */
static void (*gEmitterPointFuncs[])(Actor *obj, s16 arg1, Vec3f *out) = {
    weapon_getSide0JointPos, weapon_getSide1JointPos, func_150FE794, weapon_getLauncherMuzzlePos, weapon_getGunMuzzlePos, weapon_getForwardProjectileStart, weapon_getAttachmentPos,
};
/* The guns' muzzle points and the bones they are on, for particle_getGunAxes (by gun). */
const Vec3f *gGunMuzzlePoints[] = { &gGunMuzzleStart, &D_800A19E0 };
u8 *gGunMuzzleBones[] = { &gGunMuzzleBone, &D_80088B10 };

void *memcpy(void *, const void *, u32);

/* Spawns the impact effects (sound, sparks, debris, decal) of a hit at pos travelling along dir. */
void particle_spawnHitImpact(Vec3f *pos, Vec3f *dir, CollisionTri *surface, u8 kind, u8 arg4, s32 arg5) {
    Vec3f hitPos;
    CollisionTri hitSurface;
    u8 hit;
    s32 surfaceId;
    u8 solid;
    s32 particleType;
    s32 big;
    s32 hitVerts;
    f32 hitDist;
    s32 sfx;

    if (kind >= 6) {
        return;
    }
    switch (kind) {
        case 4:
            particleType = 12;
            break;
        case 2:
            particleType = 87;
            break;
        case 3:
            particleType = 88;
            break;
        case 0:
            particleType = 52;
            break;
        case 1:
            particleType = 77;
            break;
        case 5:
        default:
            particleType = 0;
            break;
    }
    if (surface == NULL) {
        if (func_150AC9C0(pos->x - dir->x, pos->y - dir->y, pos->z - dir->z, dir->x, dir->y, dir->z,
                          &hitVerts, &hitSurface, &hitPos.x, &hitPos.y, &hitPos.z, &hitDist, &surfaceId, 0, 0.0f)) {
            hit = 1;
        } else {
            typedef s32 BlockScope; // matching: a declaration in this block changes IDO's branch layout
            hit = 0;
            surfaceId = -1;
        }
    } else {
        surfaceId = -1;
        hitPos = *pos;
        hitSurface = *surface;
        hit = 1;
    }
    solid = func_15145C90(surfaceId);
    if ((func_150ADA20() & 1) || !solid) {
        big = 1;
    } else {
        big = 0;
    }
    if (kind == 1) {
        if (big) {
            sfx = (func_150ADA20() % 3U) + 0x459;
            if (gScreenLayout == 0) {
                sound_addDynamicSound(sfx, pos->x, pos->y, pos->z, 0x4000, 1200, 800,
                              (SoundRecordCallback)sound_delayedPlayCallback, (void *)40, 0, 0, 0);
                sfx = 0x106;
            }
        } else {
            sfx = 0xB0;
        }
        func_10010E78(0, sfx, 0x6590, (func_150ADA20() % 500U) - 250, 0, 0, pos->x, pos->y, pos->z, 500, 800);
    }
    if (func_150ADA68() < D_800DCA24) {
        TrailBurst sparks;

        sparks.countBase = 5 >> D_800DCA20;
        sparks.countRange = 4 >> D_800DCA20;
        sparks.pos = *pos;
        sparks.sizeBase = 8.25f;
        sparks.sizeRange = 5.859999657f;
        sparks.gravityBase = 0.0f;
        sparks.gravityRange = 0.0f;
        sparks.speedBase = 7.0f;
        sparks.speedRange = 16.0f;
        sparks.angles.yawBase = 0;
        sparks.angles.yawRange = 255;
        sparks.angles.pitchBase = -61;
        sparks.angles.pitchRange = 80;
        sparks.capacityBase = 3;
        sparks.capacityRange = 1;
        sparks.lifeBase = 12;
        sparks.lifeRange = 10;
        sparks.headerFlags = 1;
        sparks.flags = 4;
        sparks.primMode = 2;
        sparks.envMode = 3;
        sparks.colorBase.red = 255;
        sparks.colorBase.green = 255;
        sparks.colorBase.blue = 255;
        sparks.colorBase.alpha = 255;
        sparks.colorRange.red = 0;
        sparks.colorRange.green = 0;
        sparks.colorRange.blue = 0;
        sparks.colorRange.alpha = 0;
        sparks.color2Base.red = 255;
        sparks.color2Base.green = 255;
        sparks.color2Base.blue = 255;
        sparks.color2Base.alpha = 255;
        sparks.color2Range.red = 0;
        sparks.color2Range.green = 0;
        sparks.color2Range.blue = 0;
        sparks.color2Range.alpha = 0;
        sparks.colorLevelBase = 255;
        sparks.colorLevelRange = 0;
        sparks.renderMode = 1;
        sparks.unk21 = 36;
        sparks.setGeometryMode = 0x200005;
        sparks.clearGeometryMode = 0x60600;
        sparks.fadeStart = 10;
        sparks.fadeRate = 25;
        sparks.shrinkStart = 1;
        sparks.shrinkPaused = 0;
        sparks.shrinkFactor = 1.0f;
        sparks.unk38 = -1;
        sparks.unk39 = 0;
        sparks.unk72 = -1;
        sparks.unk73 = -1;
        particle_spawnTrailBurst(&sparks, arg4, arg5);
    }
    if (particleType != 0 && gScreenLayout < 2) {
        ParticleSpawn p;

        p.pos = *pos;
        func_15145974(dir, &p.rot.y, &p.rot.x);
        p.radius = 1.0f;
        p.elasticity = 1.0f;
        p.sizeY = 0.6499999762f;
        p.sizeXZ = 0.6499999762f;
        p.rot.z = 0.0f;
        p.scale.x = 1.0f;
        p.scale.y = 1.0f;
        p.scale.z = 1.0f;
        p.model = particleType;
        p.variant = 0;
        p.unk5C = 0;
        p.alpha = 255;
        p.update2Fn = 0;
        p.floorHitFn = 0;
        p.floorType3Fn = 0;
        p.wallHitFn = 0;
        p.destroyFn = 0;
        p.squashFn = 0;
        p.matrixFn = 2;
        p.unk6A = 0;
        p.owner = NULL;
        p.ownerId = 0;
        p.fadeStart = 12;
        p.fadeRate = 21;
        if (big) {
            Vec3f unitDir;
            f32 unkOut1;
            f32 unkOut2;
            f32 speed;

            if (func_15145128(dir, &unitDir, &unkOut1, &unkOut2)) {
                speed = func_150ADA68() * 12.0f + 3.0f;
                p.lifetime = (func_150ADA20() % 11U) + 30;
                p.vel.x = -unitDir.x * speed;
                p.vel.y = -unitDir.y * speed;
                p.vel.z = -unitDir.z * speed;
                p.rotVel.x = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.rotVel.y = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.rotVel.z = (func_150ADA68() * 203280.0f + -101640.0f) * 9.999999747e-05f;
                p.gravity = (func_150ADA68() * 449.0f + -744.0f) * 0.001000000047f;
                p.flags = 0x39E8;
                p.updateFn = 8;
                particle_newModelParticle(&p, 3, 255, 0, arg4, arg5);
            }
        } else {
            BobMotion wobble;
            void *obj;

            wobble.timer = (func_150ADA20() & 0xF) + 20;
            wobble.phase = 0.0f;
            wobble.phaseSpeed = (func_150ADA68() * 10000.0f + 10000.0f) * 9.999999747e-05f;
            wobble.amplitude = (func_150ADA68() * 21000.0f + 4000.0f) * 9.999999747e-05f;
            p.lifetime = 300;
            // matching: integer 0 (not 0.0f) makes IDO store with sw zero instead of an FPU move
            p.vel.x = 0;
            p.vel.y = 0;
            p.vel.z = 0;
            p.rotVel = p.rot;
            p.flags = 0x3980;
            p.updateFn = 12;
            p.gravity = 0;
            obj = particle_newModelParticle(&p, 3, 255, sizeof(BobMotion), arg4, arg5);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0x170, &wobble, sizeof(BobMotion));
            }
        }
    }
    if (hit && solid) {
        effect_spawnSurfaceSprite(func_150ADA68() * 20.0f + 15.0f, 255, &hitSurface, &hitPos, 300, 1, 1, 0, arg4, arg5);
    }
}

/* Particle update: bob unk20.y around unk50.y while the wobble timer runs, then settle on unk50. */
s32 particle_settleWobble(WobbleParticle *particle) {
    BobMotion *wobble = &particle->wobble;

    if (wobble->timer > 0) {
        wobble->phase += wobble->phaseSpeed * gFrameDelta;
        wobble->phase = func_15144B68(wobble->phase);
        particle->unk20.y = particle->unk50.y + sinf(wobble->phase) * wobble->amplitude;
        wobble->timer -= D_800BE9E4;
    } else {
        particle->unk20 = particle->unk50;
    }
    return 1;
}

/* Writes the world position of a point attached to bone `bone` of obj to out; objects without bone
   matrices use their origin raised by yOffset. */
void particle_getBonePoint(Actor *obj, Vec3f *out, const Vec3f *localPos, u8 bone, f32 yOffset) {
    if (obj->ob_matrix_ptr != NULL) {
        func_15143134(localPos, out, &obj->ob_matrix_ptr[bone]);
    } else {
        out->x = obj->xpos;
        out->y = obj->ypos + yOffset;
        out->z = obj->zpos;
    }
}

/* Calls actor_runAttackAt at the midpoint of a and b, with obj's movement angle relative to its mv. */
void func_151D3E6C(Actor *obj, Vec3f *a, Vec3f *b, s32 arg3) {
    f32 x = (a->x + b->x) * 0.5f;
    f32 y = (a->y + b->y) * 0.5f;
    f32 z = (a->z + b->z) * 0.5f;

    {
        u16 angle;

        if (obj->mv != NULL) {
            angle = obj->angle - obj->mv->pssd_waist_tilt;
        } else {
            angle = obj->angle;
        }
        actor_runAttackAt(x, y, z, arg3, obj - movement_obj, angle, 0, 0);
    }
}

void particle_spawnShortLightFlash(Vec3f *pos, u8 arg1, s32 arg2) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() % 3U) + 4;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    light_createWithEffect(&header, &header2, (func_150ADA20() % 13U) + 20, 255, 232, 171, 255, 0, 0, arg1, arg2);
}

void particle_spawnLightFlash(const Vec3f *pos, u8 arg1, s32 arg2) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() % 11U) + 20;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    light_createWithEffect(&header, &header2, (func_150ADA20() % 24U) + 34, 255, 161, 162, 255, 0, 0, arg1, arg2);
}

/* Calls actor_runAttackAt twice (with arg5 and arg6) at pos; the second point is 90 units back along dir,
   or pos raised by half of target's height when there is no dir. */
void func_151D40D4(Vec3f *pos, Vec3f *dir, Actor *obj, Actor *target, CollisionTri *surface, s32 arg5, s32 arg6,
                   s32 arg7) {
    u16 angle;
    Vec3f point;
    s16 height;

    if (dir != NULL) {
        angle = func_150484A0(dir->x, dir->z) * 10430.37793f;
        angle = (angle - 0x4000) | 1;
        point.x = pos->x + (dir->x * -90.0f);
        point.y = pos->y + (dir->y * -90.0f);
        point.z = pos->z + (dir->z * -90.0f);
    } else {
        point = *pos;
        if (target != NULL) {
            actor_computeSize(target, &height, NULL, NULL);
            point.y += height >> 1;
        }
        angle = 0;
    }
    actor_runAttackAt(pos->x, pos->y, pos->z, arg5, obj - movement_obj, angle, arg7, (s32)&point);
    actor_runAttackAt(pos->x, pos->y, pos->z, arg6, obj - movement_obj, angle, arg7, (s32)&point);
}

void func_151D42E8(Vec3f *pos, Vec3f *dir, Actor *obj, CollisionTri *surface, s32 arg4) {
    f32 yaw;
    u16 angle;

    if (dir != NULL) {
        yaw = func_150484A0(dir->x, dir->z);
        angle = ((u16)(yaw * 10430.37793f) - 0x4000) | 1;
    } else {
        angle = 0;
    }
    actor_runAttackAt(pos->x, pos->y, pos->z, arg4, obj - movement_obj, angle, 0, 0);
}

/* Unused outputs of model_decomposeBoneMatrix's matrix decomposition. */
struct S151D4408Temps {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    s32 pad18; // matching: keeps the stack layout of the original temporaries
};

/* Spawns one particle at pos flying along dir, oriented by matrix mtx and attached to owner. */
void particle_spawnBoneDirParticle(Vec3f *pos, Vec3f *dir, void *mtx, Actor *owner, f32 scale, u8 arg5, s32 arg6) {
    ParticleSpawn p;
    struct S151D4408Temps unused;
    f32 speed;

    model_decomposeBoneMatrix(mtx, &unused.unk0C, &unused.unk10, &unused.unk14, &p.rot.x, &p.rot.y, &p.rot.z,
                  &unused.unk00, &unused.unk04, &unused.unk08);
    p.rot.x += 180.0f;
block_151D4408: // matching: an unused label ends the basic block, keeping the store above out of the jal delay slot
    speed = func_150ADA68();

    p.radius = 1.0f;
    p.elasticity = 1.0f;
    p.sizeY = 0.04200000316f * scale;
    p.sizeXZ = p.sizeY;
    p.scale.x = 1.0f;
    p.scale.y = 1.0f;
    p.scale.z = 1.0f;
    p.pos = *pos;

    speed = (speed * 105.0f + 199.0f) * 0.001000000047f;
    p.vel.x = dir->x * speed;
    p.vel.y = dir->y * speed;
    p.vel.z = dir->z * speed;

    p.rotVel.x = (func_150ADA68() * 6378.0f + -3189.0f) * 0.001000000047f;
    p.rotVel.y = 0.0f;
    p.rotVel.z = (func_150ADA68() * 6378.0f + -3189.0f) * 0.001000000047f;
    p.gravity = (func_150ADA68() * 320.0f + -707.0f) * 0.001000000047f;
    p.flags = 0x29E8;
    p.lifetime = (func_150ADA20() & 7) + 28;
    p.model = 37;
    p.variant = 0;
    p.unk5C = 0;
    p.alpha = 255;
    p.updateFn = 8;
    p.update2Fn = 0;
    p.floorHitFn = 0;
    p.floorType3Fn = 0;
    p.wallHitFn = 0;
    p.destroyFn = 0;
    p.squashFn = 0;
    p.matrixFn = 2;
    p.unk6A = 1;
    p.owner = owner;
    p.ownerId = owner->unique_id;
    p.fadeStart = 6;
    p.fadeRate = 42;
    particle_newModelParticle(&p, 3, 255, 0, arg5, arg6);
}

/* Sends message 0x3C (handled by particle_onPointEmitterMsg) for obj's attached effects. */
void particle_detachObjEmitters(Actor *obj) {
    ActorRef handle;

    handle.actor = obj;
    handle.uniqueId = obj->unique_id;
    effect_sendEventToTimedEffects((s32)&handle, 0x3C);
}

/* Creates an effect that follows the point gEmitterPointFuncs[posFunc] computes on obj. */
void particle_attachPointEmitter(obj, posFunc, arg2, arg3, arg4)
    Actor *obj;
    u8 posFunc;
    s16 arg2;
    u8 arg3;
    s32 arg4;
{
    ObjEmitter emitter;
    TimedEffect *effect;

    emitter.obj = obj;
    emitter.uniqueId = obj->unique_id;
    emitter.posFunc = posFunc;
    emitter.spawnTimer = 0.0f;
    emitter.unk6 = -1;
    emitter.flags = 0;

    if (emitter.posFunc == 6) {
        emitter.unk6 = 0x82;
    }

    if (obj->ob_matrix_ptr != NULL) {
        if ((obj->unk74 & 0xF) != 0xF) {
            gEmitterPointFuncs[posFunc](obj, emitter.unk6, &emitter.pos);
            emitter.flags |= 1;
        }
    }

    effect = effect_newTimedEffect(arg2, -1, 0x41, -1, 1, 0x35, 0x20, arg3, arg4);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &emitter, sizeof(ObjEmitter));
    }
}

/* Effect update: tracks the emitter's point on its object and spawns smoke puffs along the path it moved. */
void particle_updatePointSmoke(ObjEmitterEffect *effect) {
    ObjEmitter *emitter;
    u8 wasValid;
    Vec3f prev;
    Actor *obj;
    Vec3f diff;

    obj = effect->emitter.obj;
    prev = effect->emitter.pos;
    emitter = &effect->emitter;
    wasValid = effect->emitter.flags & 1;
    if (obj->type == 0 || obj->unique_id != emitter->uniqueId) {
        effect->base.header.state = -1;
        return;
    }
    if (obj->ob_matrix_ptr != NULL && (obj->unk74 & 0xF) != 0xF) {
        gEmitterPointFuncs[emitter->posFunc](obj, emitter->unk6, &emitter->pos);
        emitter->flags |= 1;
    }
    if (wasValid) {
        diff.x = emitter->pos.x - prev.x;
        diff.y = emitter->pos.y - prev.y;
        diff.z = emitter->pos.z - prev.z;
        if (func_15143E64(&diff) > 5.0f) {
            return;
        }
        emitter->spawnTimer += (0.200000003f + func_150ADA68() * 0.2090000063f) * gFrameDelta;
        if (emitter->spawnTimer > 1.0f) {
            ParticleDesc spawn;
            WobbleData extra;
            void *ret;

            extra.drag = 0.9156860113f;
            spawn.texture = 108;
            spawn.renderMode = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.fadeStart = 70;
            spawn.fadeRate = 3;
            spawn.flags = 0x90DE07;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = 32;
            spawn.deathFunc = -1;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.scaleStart = 70;
            spawn.r = 221;
            spawn.g = 211;
            spawn.b = 205;
            spawn.a = 255;
            spawn.r2 = 87;
            spawn.g2 = 85;
            spawn.b2 = 90;
            spawn.brightness = 255;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            spawn.scaleRate = 1.025550008f;
            do {
                {
                    f32 t = func_150ADA68();

                    spawn.pos.x = diff.x * t + prev.x;
                    spawn.pos.y = diff.y * t + prev.y;
                    spawn.pos.z = diff.z * t + prev.z;
                }
                spawn.vel.x = diff.x * gFrameDeltaInv * 0.2980000079f;
                spawn.vel.y = diff.y * gFrameDeltaInv * 0.2980000079f;
                spawn.vel.z = diff.z * gFrameDeltaInv * 0.2980000079f;
                extra.phase[0] = func_150ADA20() & 0xFF;
                extra.phase[1] = func_150ADA20() & 0xFF;
                extra.phaseStep[0] = func_150ADA20() % 5U + 4;
                extra.phaseStep[1] = func_150ADA20() % 5U + 4;
                extra.amplitude[0] = func_150ADA68() * 6.0f;
                extra.amplitude[1] = func_150ADA68() * 6.0f;
                spawn.gravity = func_150ADA68() * 0.1800000072f + 0.1300000101f;
                spawn.flags &= ~0xC0;
                spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
                spawn.alpha = func_150ADA20() % 61U + 60;
                spawn.timer = func_150ADA20() % 11U + 70;
                spawn.width = spawn.height = func_150ADA68() * 40.0f + 40.0f;
                ret = particle_newSpriteParticleWithFloor(&spawn, 1, 0, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
                }
                emitter->spawnTimer -= 1.0f;
            } while (emitter->spawnTimer > 1.0f);
        }
    }
}

/* Effect message handler: 0x3C (see particle_detachObjEmitters) destroys the effect when it belongs to the given
   object; 4 marks its tracked point as invalid. */
void particle_onPointEmitterMsg(ObjEmitterEffect *effect, ActorRef *handle, u8 msg) {
    ObjEmitter *emitter;

    emitter = &effect->emitter;
    if (msg == 0x3C) {
        if ((handle->actor == emitter->obj) || (handle->uniqueId == emitter->uniqueId)) {
            effect_destroyObject(&effect->base.header);
        }
    } else if (msg == 4) {
        if ((handle->actor == emitter->obj) || (handle->uniqueId == emitter->uniqueId)) {
            emitter->flags &= ~1;
        }
    block_151D4C38: // matching: an empty labelled statement keeps the flag store out of the branch delay slot
        ;
    } else {
        effect_handleTimedEffectOwnerEvent((s32)handle, msg, &emitter->obj, &emitter->uniqueId, effect);
    }
}

/* Puff update callback. */
s32 particle_updatePointPuff(SpriteParticle *puff, s32 arg1) {
    WobbleData *extra;

    extra = &puff->data.wobble;
    particle_wobbleSpriteOffset(&puff->head, extra->amplitude, extra->phase, extra->phaseStep);
    particle_applyDrag(&puff->head.desc.vel, extra->drag, extra);
    return 1;
}

/* Attaches two emitters (point functions 0 and 1) to obj. */
void particle_attachTwinEmitters(Actor *obj) {
    particle_attachPointEmitter(obj, 0, 80, 255, 1);
    particle_attachPointEmitter(obj, 1, 80, 255, 1);
}



/* Applies a hit from attacker to victim travelling along dir. */
s32 func_151D4DAC(Actor *victim, Actor *attacker, Vec3f *from, Vec3f *to, RayHit *hit, s32 flags, Vec3f *dir, u8 arg7,
                  s32 arg8) {
    s32 alreadyDead;
    s32 health;
    s32 hp;
    u16 angle;

    if ((victim->character_type == 9) || (victim->character_type == 0xF) || (victim->character_type == 0x10) || (victim->character_type == 0x12) ||
        (victim->character_type == 0x17) || (victim->character_type == 0x1B) || (victim->character_type == 0x1E) || (victim->character_type == 0x28) ||
        (victim->character_type == 0x29) || (victim->character_type == 0x2A) || (victim->character_type == 0x2B) || (victim->character_type == 0x2C) ||
        (victim->character_type == 0x2E) || (victim->character_type == 0x38) || (victim->character_type == 0x41) || (victim->character_type == 0x42) ||
        (victim->character_type == 0x4B) || (victim->character_type == 0x46) || (victim->character_type == 0x47) || (victim->character_type == 0x49) ||
        (victim->character_type == 0x4A) || (victim->character_type == 0x4C) || (victim->character_type == 0x4D) || (victim->character_type == 0x4E) ||
        (victim->character_type == 0x4F) || (victim->character_type == 0x52) || (victim->character_type == 0x5D) || (victim->character_type == 0x60) ||
        (victim->character_type == 0x61) || (victim->character_type == 0x66) || (victim->character_type == 0x67) || (victim->character_type == 0x70) ||
        (victim->character_type == 0x73) || (victim->character_type == 0x77) || (victim->character_type == 0x7B) || (victim->character_type == 0x89) ||
        (victim->character_type == 0x8C) || (victim->character_type == 0x8E) || (victim->character_type == 0x8F) || (victim->character_type == 0x91) ||
        (victim->character_type == 0x9E) || (victim->character_type == 0xA6) || (victim->character_type == 0xAB) || (victim->character_type == 0xAC) ||
        (victim->character_type == 0xB2) || (victim->character_type == 0xB4) || (victim->character_type == 0x5B)) {
        weapon_queueShotSound(attacker, from, dir, 9, (hit != NULL) ? hit->dist : 0.0f);
    }

    health = victim->energy;
    if (health == 0) {
        alreadyDead = 1;
    } else {
        alreadyDead = 0;
    }

    flags |= 0x60000;
    hp = victim->energy;
    if ((hit != NULL) && (hit->hitType == 3) && (hit->part == 10)) {
        if (health >= 3) {
            victim->energy = hp - 1;
        } else {
            victim->energy = 0;
            func_1507DE4C(victim);
            func_15138C80(victim, 0xFF, 1);
            flags = 0x100020;
        }
    }

    func_15137610(victim, from, to, dir, arg7, arg8);

    if (!alreadyDead) {
        angle = func_150484A0(dir->x, dir->z) * 10430.37793f;
        actor_hitScripted(victim, flags | 0x80000, (angle - 0x4000) | 1,
                      (attacker != NULL) ? (attacker - movement_obj) : -1);
    }
    // no return statement here: the original falls off the end (game/Weapon/Projectile/projectile.c returns what is left in v0)
}

/* Counts down mv->gun_before_reload and counts up mv->shotsfired. */
void particle_countShot(Actor *obj) {
    if (obj->mv->gun_before_reload > 0) {
        obj->mv->gun_before_reload--;
    }
    obj->mv->shotsfired++;
}

/* If obj's mv is in state 2 (or bot is set): intersects the line through mv->aimEye along
   unk31C->unk130 with the plane through ref (arg9, or arg1 without it) whose normal func_151450B4 derives
   from arg2..arg4. The point and the line direction go to pointBuf/dirBuf and *outPoint/*outDir point at
   them; otherwise both are NULL. */
void func_151D5174(Actor *obj, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, Vec3f *arg4, Vec3f *pointBuf, Vec3f *dirBuf, Vec3f **outPoint, Vec3f **outDir, Vec3f *arg9) {
    Vec3f *origin;
    Vec3f temp;
    Vec3f normal;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;

    if (obj->ob_matrix_ptr != NULL) {
        if (obj->mv != NULL && (obj->mv->AimState == 2 || obj->mv->bot != 0)) {
            origin = &obj->mv->aimEye;
            func_151450B4(arg2, arg3, &temp);
            func_151450B4(&temp, arg4, &normal);
            if (arg9 != NULL) {
                dx = arg9->x - origin->x;
                dy = arg9->y - origin->y;
                dz = arg9->z - origin->z;
            } else {
                dx = arg1->x - origin->x;
                dy = arg1->y - origin->y;
                dz = arg1->z - origin->z;
            }
            t = normal.x * dx + normal.y * dy + normal.z * dz;
            t /= normal.x * obj->mv->aimDir.x + normal.y * obj->mv->aimDir.y +
                 normal.z * obj->mv->aimDir.z;
            pointBuf->x = origin->x + t * obj->mv->aimDir.x;
            pointBuf->y = origin->y + t * obj->mv->aimDir.y;
            pointBuf->z = origin->z + t * obj->mv->aimDir.z;
            dirBuf->x = obj->mv->aimDir.x;
            dirBuf->y = obj->mv->aimDir.y;
            dirBuf->z = obj->mv->aimDir.z;
            *outPoint = pointBuf;
            *outDir = dirBuf;
        end: // matching: the unused label changes IDO's block layout and register allocation
            return;
        }
    }
    *outPoint = NULL;
    *outDir = NULL;
}

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} Params151D5334;

/* Calls func_15164F0C once for each i in 0..gScreenLayout with the same parameters. */
void particle_startPlayerCameraWobbles(Vec3f *pos, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5, s32 arg6) {
    s32 i;
    Params151D5334 params;

    params.pos = *pos;
    params.unkC = arg1;
    params.unk10 = arg2;
    params.unk14 = arg3;

    for (i = 0; i < gScreenLayout + 1; i++) {
        // matching: &params goes through an int (a plain pointer changes the register allocation)
        func_15164F0C(arg4, i, (void *)(s32)&params, arg5, arg6);
    }
}

/* User data of the effects particle_startPlayerRumbles creates. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 index;
    /* 0x19 */ u8 pad19[7]; // matching: the original stack slot is 0x20 bytes; only 0x1C are copied
} Params151D5404;

/* Creates one func_151D8868 effect for each i in 0..gScreenLayout, with unk2 random in [base, base + range]. */
void particle_startPlayerRumbles(Vec3f *pos, f32 arg1, f32 arg2, f32 arg3, s32 base, s32 range, s32 arg6, s32 arg7) {
    Params151D5404 params;
    RumbleParams header;
    void *effect;
    s32 i;

    params.pos = *pos;
    params.unkC = arg1;
    params.unk10 = arg2;
    params.unk14 = arg3;
    header.flags = 1;
    header.duration = (func_150ADA20() % ((s16)range + 1)) + (s16)base;
    header.strength = 0;
    header.updateKind = 0;

    for (i = 0; i < gScreenLayout + 1; i++) {
        header.playerMask = 1 << i;
        params.index = i;
        effect = func_151D8868(&header, 0x1C, 255, 0);
        if (effect != NULL) {
            memcpy((u8 *)effect + 0x18, &params, 0x1C);
        }
    }
}

void particle_spawnBlastSpray(const Vec3f *pos, u8 arg1, s32 arg2) {
    JetSpraySpec spawn;

    spawn.angles.yawBase = 0;
    spawn.angles.yawRange = 255;
    spawn.angles.pitchBase = -64;
    spawn.angles.pitchRange = 74;
    spawn.spray.countBase = 9;
    spawn.spray.countRange = 3;
    spawn.spray.pos = *pos;
    spawn.spray.speedBase = 323.0f;
    spawn.spray.speedRange = 214.0f;
    spawn.spray.sizeBase = 203.0f;
    spawn.spray.sizeRange = 263.0f;
    spawn.spray.sizeSlopeBase = 677.0f;
    spawn.spray.sizeSlopeRange = 895.0f;
    spawn.spray.particleCountBase = 7;
    spawn.spray.particleCountRange = 3;
    spawn.spray.spread = 45.0f;
    spawn.spray.drag = 0.6020000577f;
    spawn.spray.gravityBase = -1.732000113f;
    spawn.spray.gravityRange = 1.512000084f;
    spawn.spray.delayBase = 15;
    spawn.spray.delayRange = 15;
    spawn.spray.alphaBase = 100;
    spawn.spray.alphaRange = 100;
    spawn.spray.fadeStart = 12;
    spawn.spray.fadeOutStart = 20;
    spawn.spray.eventFunc = 0;
    particle_spawnJetSprayBursts(&spawn, arg1, arg2);
}

/* Combined effect at pos: particle_spawnBlastSpray, particle_spawnLightFlash and particle_spawnGrowFade. */
void particle_spawnBlast(Vec3f *pos, u8 arg1, s32 arg2) {
    u32 randU;
    f32 randF;

    particle_spawnBlastSpray(pos, arg1, arg2);
    particle_spawnLightFlash(pos, arg1, arg2);
    randF = func_150ADA68();
    randU = func_150ADA20();
    particle_spawnGrowFade(pos, randF * 4.0f + 12.0f, 1.640999913f,
                  (randU % 56U) + 200, 0.0f, arg1, arg2);
}

/* Spawns a particle_spawnBoneDirParticle particle on bone `bone` of obj, from local point from towards local point to. */
void particle_spawnBoneSegmentParticle(Actor *obj, const Vec3f *from, const Vec3f *to, u8 bone, f32 scale, u8 arg5, s32 arg6) {
    Vec3f start;
    Vec3f dir;
    MtxF *mtx;
    const Vec3f *srcs[2];
    Vec3f *dsts[2];

    if (obj->ob_matrix_ptr != NULL) {
        if ((obj->unk74 & 0xF) != 0xF) {
            mtx = obj->ob_matrix_ptr + bone;
            srcs[0] = from;
            srcs[1] = to;
            dsts[0] = &start;
            dsts[1] = &dir;
            func_15145EA4(srcs, dsts, mtx, 2);
            dir.x -= start.x;
            dir.y -= start.y;
            dir.z -= start.z;
            { // matching: a block holding a declaration changes how as1 schedules the call setup
void particle_spawnBoneDirParticle(Vec3f *pos, Vec3f *dir, void *mtx, Actor *owner, f32 scale, u8 arg5, s32 arg6);
                particle_spawnBoneDirParticle(&start, &dir, mtx, obj, scale, arg5, arg6);
            }
        }
    }
}

/* Computes obj's emission point and direction for slot `slot`, stores a far target point along it in
   unk31C->unkB8..unkC0, and calls collision_castRay from the point towards that target. */
void func_151D57F8(Actor *obj, u8 slot) {
    Vec3f pos;
    Vec3f forward;
    Vec3f axis1;
    Vec3f axis2;
    Vec3f pointBuf;
    Vec3f dirBuf;
    Vec3f *origin;
    Vec3f *dir;

    particle_getGunAxes(obj, &pos, &forward, &axis1, &axis2, gGunMuzzlePoints[slot], *gGunMuzzleBones[slot]);
    func_151D5174(obj, &pos, &forward, &axis1, &axis2, &pointBuf, &dirBuf, &origin, &dir, NULL);
    obj->mv->rayHit.hitType = 1;
    if (origin == NULL) {
        origin = &pos;
    }
    if (dir == NULL) {
        dir = &forward;
    }
    obj->mv->rayHit.pos.x = origin->x + dir->x * 9999.0f;
    obj->mv->rayHit.pos.y = origin->y + dir->y * 9999.0f;
    obj->mv->rayHit.pos.z = origin->z + dir->z * 9999.0f;
    if (obj->mv->rayHit.hitType) {
        Vec3f delta;
        f32 len;

        delta.x = obj->mv->rayHit.pos.x - pos.x;
        delta.y = obj->mv->rayHit.pos.y - pos.y;
        delta.z = obj->mv->rayHit.pos.z - pos.z;
        if (func_15145128(&delta, &delta, &len, NULL) != 0) {
            collision_castRay(obj, pos.x, pos.y, pos.z, delta.x, delta.y, delta.z,
                          &obj->mv->rayHit, len * 1.00999999f, 0, 0, 0, -1, 0, 0);
        }
    }
    func_150636A4(obj);
}

/* Computes obj's axes (forward, axis1, axis2) and the world position of localPos on bone `bone`;
   objects without bone matrices use a point 34 units out and 49 up from their origin. */
void particle_getGunAxes(Actor *obj, Vec3f *out, Vec3f *forward, Vec3f *axis1, Vec3f *axis2, const Vec3f *localPos, u8 bone) {
    const Vec3f *src;
    Vec3f *dst;

    func_15145740(obj, forward, axis1, axis2, 0.8110000491f);
    if (obj->ob_matrix_ptr != NULL) {
        dst = out;
        src = localPos;
        func_15145EA4(&src, &dst, obj->ob_matrix_ptr + bone, 1);
    } else {
        f32 dz;
        f32 dx;

        if ((9.999999747e-05f < fabsf(forward->x)) || (9.999999747e-05f < fabsf(forward->z))) {
            f32 inv;
            f32 z;

            z = forward->z;
            inv = 1.0f / sqrtf((forward->x * forward->x) + (z * z));
            dz = z * inv;
            dx = -forward->x * inv;
        } else {
            dz = 1.0f;
            dx = 0.0f;
        }
        out->x = obj->xpos + (34.0f * dx);
        out->y = obj->ypos + 49.0f;
        out->z = obj->zpos + (34.0f * dz);
    }
}


/* Runs func_151452C4(p0, p1, center, 2 * radius) for every active object except `self` (and, when filter
   is not -1, only for those gObjectFilters[filter] accepts). Returns a mask with bit i cleared for each
   object i it reports. */
s32 func_151D5B6C(Vec3f *p0, Vec3f *p1, Actor *self, s8 filter, void *filterArg) {
    f32 radius;
    Actor *obj;
    s32 mask;
    Vec3f center;
    Vec3f out0;
    Vec3f out1;
    f32 outF0;
    f32 outF1;
    u8 i;

    mask = -1;
    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0) continue;
        if (obj->drawtype == 3) continue;
        if (level != 0x23 && obj->character_type == 0xFF) continue;
        if (obj == self) continue;
        if (!(obj->noshove & 0x40)) continue;
        if (filter != -1 && gObjectFilters[filter](obj, self, filterArg) == 0) continue;
        if (obj->unkD2 < obj->unkD4) {
            radius = obj->unkD4;
        } else {
            radius = obj->unkD2;
        }
        center.x = obj->xpos;
        center.y = obj->ypos + obj->unkD6;
        center.z = obj->zpos;
        if (func_151452C4(p0, p1, &center, radius + radius, &out0, &out1, &outF0, &outF1)) {
            mask &= ~(1 << i);
        }
    }
    return mask;
}

/* Returns in *out the half of the double buffer slots[index] selected by D_800BE9C0, allocating the buffer
   (2 * size bytes) on first use; *allocated (optional) is set when it was allocated now. The buffers hold
   vertices of several types, so slots points to an array of buffer pointers of any type and out to a
   pointer of any type. */
void particle_getViewBuffer(void *slots, s16 index, s32 size, void *out, u8 *allocated) {
    u8 dummy;

    if (allocated == NULL) {
        allocated = &dummy;
    }
    *allocated = 0;
    if (((void **)slots)[index] == NULL) {
        ((void **)slots)[index] = allocate_memory(size * 2, 1, 2, 1);
        if (((void **)slots)[index] == NULL) {
            *(void **)out = NULL;
            return;
        }
        *allocated = 1;
    }
    *(void **)out = (u8 *)((void **)slots)[index] + ((D_800BE9C0 != 0) ? 0 : size);
}

/* Releases the four buffers of a particle_getViewBuffer slot array. */
void particle_freeViewBuffers(void *slots) {
    u8 i;

    for (i = 0; i < 4; i++) {
        // matching: the buffer addresses are read as integers and converted to pointers
        if (((s32 *)slots)[i] != 0) {
            func_100043B4((void *)((s32 *)slots)[i], 3);
        }
    }
}
