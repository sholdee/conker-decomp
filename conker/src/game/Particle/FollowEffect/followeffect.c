#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Attaches a FollowEffect (particle_updateFollowEffect) to obj. */
void particle_attachFollowEffect(Actor *obj, u8 arg1, s32 arg2) {
    FollowEffectData data;
    FollowEffect *effect;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffect(300, -1, 0x38, -1, 0, 0x28, sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/*
 * FollowEffect update: while the owner is low (unk28 < 8) on a type-0xE surface, emits
 * particles on both sides of it at a rate of 3-4 per second (scaled by gFrameDelta).
 */
void particle_updateFollowEffect(FollowEffect *effect) {
    Actor *owner;
    FollowEffectData *data;
    ParticleDesc spawn;
    f32 cosYaw;
    f32 sinYaw;
    f32 rightX;
    f32 rightZ;
    const Vec3f *refVel;
    u8 right;
    f32 along;
    f32 speed;
    f32 sideOffset;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner) {} // matching: raises owner's register priority over data
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->shad < 8.0f && (owner->floorMaterial & 0x1F) == 0xE) {
        // matching: (f32)1 keeps the multiply by 1.0 that a 1.0f literal would fold away
        data->spawnAccum += (3.000000238f + math_randomFloat() * (f32)1) * gFrameDelta;
        if (data->spawnAccum > 1.0f) {
            if (sprite_findActorMotion(owner, &refVel) == 0) {
                refVel = &gZeroVec3f;
            }
            math_dirFromAngleXZ(owner->theta * 0.01745329238f, 1.0f, &sinYaw, &cosYaw);
            rightX = cosYaw;
            rightZ = -sinYaw;
            spawn.texture = 40;
            spawn.renderMode = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.r2 = 0xFF;
            spawn.g2 = 0xFF;
            spawn.b2 = 0xFF;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.brightness = 0xFF;
            spawn.fadeStart = 5;
            spawn.fadeRate = 51;
            spawn.scaleStart = 1;
            spawn.flags = 0x84C207;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.scaleRate = 1.0f;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            do {
                right = math_random() & 1;
                along = math_randomFloat();
                speed = math_randomFloat();
                if (right) {
                    sideOffset = 35.0f;
                } else {
                    sideOffset = -35.0f;
                }
                spawn.pos.x = owner->xpos + rightX * sideOffset - sinYaw * 180.0f * 0.5f + sinYaw * 180.0f * along;
                spawn.pos.y = owner->floorY;
                spawn.pos.z = owner->zpos + rightZ * sideOffset - cosYaw * 180.0f * 0.5f + cosYaw * 180.0f * along;
                // matching: the two spellings of 6.0f / 0.564f keep IDO from sharing one rodata load
                spawn.vel.x = (right ? (speed = speed * 2.907000065f + 6.000000477f)
                                     : -(speed = speed * 2.907000065f + 6.0000005f)) * rightX - refVel->x * 0.5640000105f;
                spawn.vel.y = 3.156000137f * speed;
                spawn.vel.z = (right ? speed : -speed) * rightZ - refVel->z * 0.564f;
                spawn.timer = math_random() % 10U + 31;
                spawn.alpha = math_random() % 76U + 180;
                spawn.width = spawn.height = math_randomFloat() * 364.0f + 111.0f;
                spawn.gravity = math_randomFloat() * 0.1570000052f + -2.111000061f;
                spawn.flags &= ~0xC0;
                right = (math_random() & 1) ? 0x80 : 0; // matching: reuses the u8 local for the 0x80 flag
                spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | right;
                particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0, effect->base.header.owner, effect->base.header.kind);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

/* FollowEffect message handler: dies with its owner and follows it through message 0x2D. */
void particle_onFollowEffectMsg(FollowEffect *effect, ActorMsg *msg, u8 type) {
    FollowEffectData *data = &effect->data;
    Actor *object;

    if (type == 0 || type == 0x2F || type == 3) {
        object = msg->ref.actor;
        if (object == data->owner || msg->ref.uniqueId == data->ownerId) {
            effect_destroyObject(&effect->base.header);
        }
    } else if (type == 0x2D) {
        if (data->owner == msg->swap.actorA) {
            data->owner = msg->swap.actorB;
            data->ownerId = msg->swap.idB;
        } else if (data->owner == msg->swap.actorB) {
            data->owner = msg->swap.actorA;
            data->ownerId = msg->swap.idA;
        }
    }
}

/* FollowEffect callback; forwards to effect_defaultDestroyTimedEffect. */
void particle_destroyFollowEffect(TimedEffect *effect) {
    effect_defaultDestroyTimedEffect(effect);
}

/* FollowEffect callback; forwards to effect_defaultFreeTimedEffect. */
void particle_freeFollowEffect(TimedEffect *effect) {
    effect_defaultFreeTimedEffect(effect);
}


/*
 * Breakup effect for obj: plays sounds, then spawns pieces (three fixed models, then
 * gBloodModelCounts[type] - 2 type-specific ones) flying off the surface obj's last move hit,
 * plus impact and dust effects.
 */
void particle_spawnBreakup(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    Vec3f vel;
    Vec3f hitPos;
    u8 hit;
    s32 useDefaultBasis;
    // matching: only 12 bytes, although collision_castRayWorld writes a whole CollisionTri (18 bytes) here
    f32 hitTri[3];
    f32 ignored0;
    f32 ignored1;
    s32 hitSurface;
    s32 type;
    f32 dist;
    Vec3f normal;
    Vec3f tangentU;
    Vec3f tangentV;
    s32 moving;
    ParticleSpawn piece;

    hit = 0;
    pos.x = obj->xpos;
    pos.y = obj->ypos + 50.0f;
    pos.z = obj->zpos;
    vel.x = obj->xpos - obj->old_xpos;
    vel.y = obj->ypos - obj->old_ypos;
    vel.z = obj->zpos - obj->old_zpos;
    type = blood_getProfile(obj);
    if (type == 99) {
        return;
    }
    if ((obj->floorMaterial & 0x1F) == 0xE) {
        sound_playActorUntracked(0x434, obj, 32000, 700, 2000);
    } else {
        sound_playActorUntracked(0x4C8, obj, -32000, 700, 2000);
    }
    sound_playActorUntracked((math_random() & 1) + 0x2B6, obj, 32000, 1000, 3000);
    sound_playActorUntracked((math_random() & 3) + 0x2B1, obj, 32000, 700, 2000);
    weapon_wobbleNearbyCameras(&pos, arg1, arg2);
    weapon_rumbleNearbyPlayers(&pos, arg1, arg2);
    func_15145A50(obj);
    if (obj->ob_matrix_ptr == NULL) {
        return;
    }
    if ((obj->unk74 & 0xF) == 0xF) {
        return;
    }
    moving = math_vec3Normalize(&vel, &vel, &ignored0, &ignored1) ? 1 : 0; // matching: != 0 and a plain store differ
    if (moving) {
        typedef s32 ScopeBarrier1; // matching: -g3 scope pins the 0.0f register setup at block entry

        if (collision_castRayWorld(obj->old_xpos, obj->old_ypos, obj->old_zpos, vel.x, vel.y, vel.z, 0,
                          (CollisionTri *)hitTri, &hitPos.x, &hitPos.y, &hitPos.z, &dist, 0, &hitSurface, 0.0f) &&
            dist < 397.0f) {
            hit = 1;
        }
    }
    {
        typedef s32 ScopeBarrier2; // matching: -g3 scope pins the 0.0f register setup at block entry

        useDefaultBasis = 1;
        if (hit) {
            useDefaultBasis = 0;
            if (math_getTriEdges((CollisionTri *)hitTri, &tangentU, &tangentV, &normal)) {
                f32 ignored2;
                f32 ignored3;
                f32 ignored4;
                f32 ignored5;
                f32 ignored6;
                f32 ignored7;

                math_vec3Normalize(&tangentU, &tangentU, &ignored2, &ignored3);
                math_vec3Normalize(&tangentV, &tangentV, &ignored4, &ignored5);
                math_vec3Normalize(&normal, &normal, &ignored6, &ignored7);
                if (math_vec3Dot(&normal, &vel) > 0.0f) {
                    normal.x = -normal.x;
                    normal.y = -normal.y;
                    normal.z = -normal.z;
                }
            } else {
                useDefaultBasis = 1;
            }
        }
        if (useDefaultBasis) {
            if (moving) {
                normal.x = -vel.x;
                normal.y = -vel.y;
                normal.z = -vel.z;
                math_buildPerpendicularBasis(&normal, &tangentU, &tangentV);
            } else {
                normal.x = 0.0f;
                normal.y = 1.0f;
                normal.z = 0.0f;
                tangentU.x = 1.0f;
                tangentU.y = 0.0f;
                tangentU.z = 0.0f;
                tangentV.x = 0.0f;
                tangentV.y = 0.0f;
                tangentV.z = 1.0f;
            }
        }
        piece.radius = 1.0f;
        piece.elasticity = 1.0f;
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        piece.sizeXZ = obj->scale_xz;
        piece.sizeY = obj->scale_y;
        piece.scale.x = 1.0f;
        piece.scale.y = 1.0f;
        piece.scale.z = 1.0f;
        piece.pos = pos;
        piece.flags = 0x21E8;
        piece.variant = 0;
        piece.unk5C = 0;
        piece.alpha = 0xFF;
        piece.updateFn = 8;
        piece.update2Fn = 0;
        piece.floorHitFn = 0;
        piece.floorType3Fn = 0;
        piece.wallHitFn = 0;
        piece.destroyFn = 0;
        piece.squashFn = 0;
        piece.matrixFn = 2;
        piece.unk6A = 1;
        piece.owner = obj;
        piece.ownerId = obj->unique_id;
        piece.sound = 0;
        piece.rotVel.y = 0.0f;
        {
            const s32 firstModels[3] = { 0x6C, 0x6D, 0x6E };
            s32 i;
            f32 offsetV;
            f32 offsetU;
            f32 scale;
            s32 randAngle;
            f32 offsetV2;
            f32 offsetU2;
            s32 j;

            piece.fadeStart = 12;
            piece.fadeRate = 21;
            for (i = 0; i < 3; i++) {
                scale = math_randomFloat() * 0.01319999993f + 0.02979999967f;
                randAngle = math_random();
                math_dirFromBinAngleXZ((u8)randAngle, math_randomFloat() * 759.0f, &offsetU, &offsetV);
                piece.vel.x = (tangentU.x * offsetU + tangentV.x * offsetV + normal.x) * scale;
                piece.vel.y = (tangentU.y * offsetU + tangentV.y * offsetV + normal.y) * scale;
                piece.vel.z = (tangentU.z * offsetU + tangentV.z * offsetV + normal.z) * scale;
                piece.rot.x = math_randomFloat() * 360.0f;
                piece.rot.y = math_randomFloat() * 360.0f;
                piece.rot.z = math_randomFloat() * 360.0f;
                piece.rotVel.x = math_randomFloat() * 38.44400024f + -19.22200012f;
                piece.rotVel.z = math_randomFloat() * 38.44400024f + -19.22200012f;
                piece.gravity = math_randomFloat() * 0.6480000019f + -1.397000074f;
                piece.lifetime = math_random() % 21U + 30;
                piece.model = firstModels[i];
                particle_newModelParticle(&piece, 0, 0, 0, arg1, arg2);
            }
            piece.fadeStart = 12;
            piece.fadeRate = 21;
            for (j = 2; j < gBloodModelCounts[type]; j++) {
                scale = math_randomFloat() * 0.01999999955f + 0.005200000014f;
                randAngle = math_random();
                math_dirFromBinAngleXZ((u8)randAngle, math_randomFloat() * 723.0f, &offsetU2, &offsetV2);
                piece.vel.x = (tangentU.x * offsetU2 + tangentV.x * offsetV2 + normal.x) * scale;
                piece.vel.y = (tangentU.y * offsetU2 + tangentV.y * offsetV2 + normal.y) * scale;
                piece.vel.z = (tangentU.z * offsetU2 + tangentV.z * offsetV2 + normal.z) * scale;
                piece.rot.x = math_randomFloat() * 360.0f;
                piece.rot.y = math_randomFloat() * 360.0f;
                piece.rot.z = math_randomFloat() * 360.0f;
                piece.rotVel.x = math_randomFloat() * 40.45000076f + -20.22500038f;
                piece.rotVel.z = math_randomFloat() * 40.45000076f + -20.22500038f;
                piece.gravity = math_randomFloat() * 0.6260000467f + -1.089000106f;
                piece.lifetime = math_random() % 41U + 40;
                piece.model = gBloodModelLists[type][j];
                particle_newModelParticle(&piece, 0, 0, 0, arg1, arg2);
            }
        }
        if (obj->CameraPtr != NULL && gScreenLayout <= 0) {
            particle_flashScreenWhite(obj->CameraPtr->CNumber, arg1, arg2);
        }
        if (hit && obj->CameraPtr != NULL) {
            if ((hitSurface & 0x1F) == 0xE) {
                func_150D65F0((CollisionTri *)hitTri, &hitPos, arg1, arg2);
            } else {
                weapon_spawnHitDecal((CollisionTri *)hitTri, &hitPos, arg1, arg2);
            }
        }
        particle_spawnRandomSwell(&pos, arg1, arg2);
        particle_spawnRandomGrowFade(&pos, arg1, arg2);
    }
}

/* Spawns a particle_spawnGrowFade effect at pos with a random size and a 200-255 fourth argument. */
void particle_spawnRandomGrowFade(Vec3f *pos, u8 arg1, s32 arg2) {
    u32 randInt;
    f32 randFrac;

    randFrac = math_randomFloat();
    randInt = math_random();
    particle_spawnGrowFade(pos, randFrac * 4.0f + 15.0f, 1.374570012f, randInt % 56 + 200, 0.0f, arg1, arg2);
}

/* Spawns a particle_spawnSwell effect at pos with randomized parameters. */
void particle_spawnRandomSwell(Vec3f *pos, u8 arg1, s32 arg2) {
    f32 rand3;
    f32 rand2;
    f32 rand1;

    rand1 = math_randomFloat();
    rand2 = math_randomFloat();
    rand3 = math_randomFloat();
    particle_spawnSwell(pos, rand1 * 3.0f + 8.0f, rand2 * 0.4030000269f + 1.305000067f, rand3 * 50.0f + 100.0f, arg1, arg2);
}

/* Fills in fixed settings; unk50 is -1 when gScreenLayout >= 2 or D_8008FD8C >= 8, else 0. */
void func_150D64E8(Struct150D64E8 *info, f32 *outValue) {
    info->unk00 = 6;
    info->unk04 = 3;
    *outValue = 1263.0f;
    info->unk14 = 0.08999999613f;
    info->unk18 = 0.03029999882f;
    info->unk1C = 150.0f;
    info->unk20 = 105.0f;
    info->unk24 = 396.0f;
    info->unk28 = 612.0f;
    info->unk2C = 4;
    info->unk30 = 3;
    info->unk34 = 25.0f;
    info->unk38 = 0.8120000362f;
    info->unk3C = -2.0f;
    info->unk40 = 1.618000031f;
    info->unk44 = 20;
    info->unk46 = 30;
    info->unk48 = 100;
    info->unk4A = 100;
    info->unk4C = 12;
    info->unk4E = 20;

    if (gScreenLayout >= 2 || D_8008FD8C >= 8) {
        info->unk50 = -1;
    } else {
        info->unk50 = 0;
    }
}

/* Impact effect on a type-0xE surface hit at pos (particle_spawnSurfaceMark with randomized parameters). */
void func_150D65F0(CollisionTri *surface, Vec3f *pos, u8 arg2, s32 arg3) {
    u32 rand3;
    u32 rand2;
    f32 rand1;

    rand1 = math_randomFloat();
    rand2 = math_random();
    rand3 = math_random();
    particle_spawnSurfaceMark(rand1 * 100.0f + 150.0f, rand2 % 71 + 130, surface, pos, rand3 % 31 + 50, 0, 1, arg2, arg3);
}

/* Calls screen_spawnFlash for camera arg0 with 0xFF, 0xFF, 0xFF, a random 200-255 and a random 25-30. */
void particle_flashScreenWhite(u8 arg0, u8 arg1, s32 arg2) {
    u32 rand2;
    u32 rand1;

    rand1 = math_random();
    rand2 = math_random();
    screen_spawnFlash(0xFF, 0xFF, 0xFF, rand1 % 56 + 200, rand2 % 6 + 25, arg0, arg1, arg2);
}

/* Spawns 15-25 drift particles and one grow particle that follow obj. */
void particle_spawnFollowDrift(Actor *obj, u8 arg1, s32 arg2) {
    s32 count;
    ParticleDesc spawn;
    FollowDriftData drift;
    FollowParticle *particle;
    ParticleDesc growSpawn;
    GrowParticleData grow;
    s32 r1;
    s32 r2;

    count = math_random() % 11U + 15;
    drift.owner = obj;
    drift.ownerId = obj->unique_id;
    drift.drag = 0.8711180091f;
    spawn.texture = 0x6C;
    spawn.renderMode = 0x5103;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r2 = 0x14;
    spawn.g2 = 0x14;
    spawn.b2 = 0x14;
    spawn.a = 0xFF;
    spawn.r = 0x4B;
    spawn.g = 0x4B;
    spawn.b = 0x4B;
    spawn.brightness = 0xFF;
    particle_getPointAboveObj(obj, &spawn.pos);
    spawn.fadeStart = 15;
    spawn.fadeRate = 17;
    spawn.flags = 0x84DE01;
    spawn.primColorMode = 8;
    spawn.envColorMode = 6;
    spawn.updateFunc = 30;
    spawn.deathFunc = -1;
    spawn.drawFunc = -1;
    spawn.handlerSet = 8;
    spawn.viewMask = 0;
    spawn.views = 0xFF;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.scaleRate = 1.062733054f;
    do {
        spawn.alpha = math_random() % 156U + 100;
        spawn.scaleStart = spawn.timer = math_random() % 14U + 8;
        spawn.width = spawn.height = math_randomFloat() * 207.0f + 238.0f;
        r1 = math_random();
        r2 = math_random();
        math_dirFromBinAngles(r1 & 0xFF, r2 % 36U - 25, math_randomFloat() * 30.0f + 25.0f, &spawn.vel);
        spawn.gravity = math_randomFloat() * (f32)0; // matching: keeps the multiply that a 0.0f literal folds away
        spawn.flags &= ~0xC0;
        r1 = (math_random() & 1) ? 0x80 : 0;
        r2 = (math_random() & 1) ? 0x40 : 0;
        spawn.flags |= r2 | r1;
        particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0xC, arg1, arg2);
        if (particle != NULL) {
            memcpy(&particle->data.drift, &drift, sizeof(drift));
        }
        count--;
    } while (count > 0);

    {
        s32 pad; // matching: an unused slot in a separate scope keeps the frame layout

        grow.owner = obj;
        grow.ownerId = obj->unique_id;
        grow.time = 0.0f;
        grow.growth = 2392.159912f;
        grow.lifetime = math_randomFloat() * 3.0f + 9.0f;
        grow.fadeStart = math_randomFloat() * 80.0f + 100.0f;
        grow.fadeRate = grow.fadeStart / (grow.lifetime * grow.lifetime);
        growSpawn.alpha = grow.fadeStart;
        growSpawn.texture = 0x79;
        growSpawn.renderMode = 0x4403;
        growSpawn.unk0 = 0x200005;
        growSpawn.unk4 = 0;
        growSpawn.frame = 0;
        growSpawn.frameRate = 0;
        growSpawn.r2 = 0xFF;
        growSpawn.g2 = 0xFF;
        growSpawn.b2 = 0xDF;
        growSpawn.a = 0xFF;
        growSpawn.r = 0xFF;
        growSpawn.g = 0xFF;
        growSpawn.b = 0xFF;
        growSpawn.brightness = 0xFF;
        particle_getPointAboveObj(obj, &growSpawn.pos);
        growSpawn.offset = gZeroVec3f;
        growSpawn.fadeStart = 1;
        growSpawn.fadeRate = 0xFF;
        growSpawn.scaleStart = 1;
        growSpawn.scaleRate = 1.0f;
        r1 = (math_random() & 1) ? 0x40 : 0;
        r2 = (math_random() & 1) ? 0x80 : 0;
        growSpawn.flags = r2 | 0x4C000 | r1;
        growSpawn.primColorMode = 6;
        growSpawn.envColorMode = 6;
        growSpawn.updateFunc = 31;
        growSpawn.deathFunc = -1;
        growSpawn.drawFunc = -1;
        growSpawn.handlerSet = 9;
        growSpawn.viewMask = 0;
        growSpawn.views = 0xFF;
        growSpawn.vel = gZeroVec3f;
        growSpawn.timer = 300;
        growSpawn.height = 0.0f;
        growSpawn.width = 0.0f;
        growSpawn.gravity = 0.0f;
        particle = particle_newSpriteParticleWithFloor(&growSpawn, 1, 0, 0x1C, arg1, arg2);
        if (particle != NULL) {
            memcpy(&particle->data.grow, &grow, sizeof(grow));
        }
    }
}

/* Writes the point 60 units above obj's position to out. */
void particle_getPointAboveObj(Actor *obj, Vec3f *out) {
    out->x = obj->xpos;
    out->y = obj->ypos + 60.0f;
    out->z = obj->zpos;
}

/* Drift particle update: follows the owner and integrates offset/velocity; returns 0 to kill. */
s32 particle_updateFollowDrift(FollowParticle *particle, s32 arg1) {
    FollowDriftData *data;
    Actor *owner;

    data = &particle->data.drift;
    owner = data->owner;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        return 0;
    }
    particle_getPointAboveObj(owner, &particle->head.desc.pos);
    {
        Vec3f prevVel = particle->head.desc.vel;
        f32 accelX;
        f32 accelY;
        f32 accelZ;

        particle->head.desc.vel.y += particle->head.desc.gravity * gFrameDelta;
        particle_applyDrag(&particle->head.desc.vel, data->drag);
        accelX = (particle->head.desc.vel.x - prevVel.x) * gFrameDeltaInv;
        accelY = (particle->head.desc.vel.y - prevVel.y) * gFrameDeltaInv;
        accelZ = (particle->head.desc.vel.z - prevVel.z) * gFrameDeltaInv;
        particle->head.desc.offset.x += prevVel.x * gFrameDelta +
            accelX * gFrameDelta * gFrameDelta * 0.5f;
        particle->head.desc.offset.y += prevVel.y * gFrameDelta +
            accelY * gFrameDelta * gFrameDelta * 0.5f;
        particle->head.desc.offset.z += prevVel.z * gFrameDelta +
            accelZ * gFrameDelta * gFrameDelta * 0.5f;
    }
    return 1;
}

/* Drift particle message handler: dies with its owner and follows it through message 0x2D. */
void particle_onFollowDriftMsg(FollowParticle *particle, ActorMsg *msg, u8 type) {
    FollowDriftData *data = &particle->data.drift;
    Actor *object;

    if (type == 0x2D) {
        if (data->owner == msg->swap.actorA) {
            data->owner = msg->swap.actorB;
            data->ownerId = msg->swap.idB;
        } else {
            if (data->owner == msg->swap.actorB) {
                data->owner = msg->swap.actorA;
                data->ownerId = msg->swap.idA;
            }
        trailing_150D6E60: // matching: a label here keeps IDO from merging this path's exit branch
            ;
        }
    } else if (type == 0) {
        object = msg->ref.actor;
        if (object == data->owner || msg->ref.uniqueId == data->ownerId) {
            effect_destroyObject(&particle->head.header);
        }
    }
}

/* Grow particle update: follows the owner, grows and fades; returns 0 once its lifetime is over. */
s32 particle_updateFollowGrow(FollowParticle *particle, s32 arg1) {
    GrowParticleData *data;
    Actor *owner;
    f32 size;

    data = &particle->data.grow;
    owner = data->owner;

    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        return 0;
    }

    particle_getPointAboveObj(owner, &particle->head.desc.pos);

    size = sqrtf(data->time) * data->growth;
    particle->head.desc.height = size;
    particle->head.desc.width = size;

    particle->head.desc.alpha = data->fadeStart - data->fadeRate * data->time * data->time;
    data->time += gFrameDelta;

    if (data->lifetime < data->time) {
        return 0;
    }
    return 1;
}

/* Grow particle message handler: dies with its owner and follows it through message 0x2D. */
void particle_onFollowGrowMsg(FollowParticle *particle, ActorMsg *msg, u8 type) {
    GrowParticleData *data = &particle->data.grow;
    Actor *object;

    if (type == 0x2D) {
        if (data->owner == msg->swap.actorA) {
            data->owner = msg->swap.actorB;
            data->ownerId = msg->swap.idB;
        } else {
            if (data->owner == msg->swap.actorB) {
                data->owner = msg->swap.actorA;
                data->ownerId = msg->swap.idA;
            }
        trailing_150D7068: // matching: a label here keeps IDO from merging this path's exit branch
            ;
        }
    } else if (type == 0) {
        object = msg->ref.actor;
        if (object == data->owner || msg->ref.uniqueId == data->ownerId) {
            effect_destroyObject(&particle->head.header);
        }
    }
}
