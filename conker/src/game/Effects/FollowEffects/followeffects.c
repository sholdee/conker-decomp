#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/model.h"
#include "conker/particle.h"
#include "conker/save.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/view.h"
#include "conker/weapon.h"
#include "libc/string.h"



/* Spawns the effect tied to `owner` and records the owner in the effect's extra data. */
void *effect_spawnOwnerModel(Actor *owner, u8 fxOwner, s32 fxList) {
    DynModel *effect;
    DynModelDesc desc;
    ActorRef ref;

    if (owner == NULL) {
        return NULL;
    }
    ref.actor = owner;
    ref.uniqueId = owner->unique_id;
    desc.flags = 2;
    desc.updateFunc = -1;
    desc.drawFunc = 3;
    desc.lifetime = 300;
    desc.assetTable = 9;
    desc.assetIndex = 0x1AD;
    desc.viewportMask = 0xFF;
    {
        s32 objIndex = owner - movement_obj;

        if ((objIndex < 2) && (gPartnerSlots[objIndex] != 0)) {
            if (gHumanCount == 2) {
                desc.viewportMask = 0xFD;
            } else if (gHumanCount == 4) {
                if (objIndex == 0) {
                    desc.viewportMask = 0xFB;
                } else {
                    desc.viewportMask = 0xF7;
                }
            }
        }
    }
    desc.unk8 = 1;
    desc.unkC = 0x200205;
    desc.unk10 = 0x60600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.kind = 2;
    effect = effect_newDynModel(&desc, 1, 8, fxOwner, fxList);
    if (effect != NULL) {
        memcpy(DYNMODEL_USERDATA(effect), &ref, sizeof(ref));
    }
    return effect;
}


/*
 * Per-frame update of an effect that follows its owner's model: builds the effect's matrix from the
 * owner's and sets each vertex's texture coordinates from its projected position in `viewport`.
 * Removes the effect when the owner is gone; returns 0 when it should not be drawn.
 */
s32 effect_projectOwnerModelTexCoords(DynModel *effect, s16 viewport) {
    ActorRef *ref;
    Actor *owner;
    MtxF *model;
    MtxF modelMtx;
    Actor *player;
    MtxF viewMtx;
    s32 i;
    f32 originS;
    f32 origin[3];
    Vec3f originWorld;
    f32 originT;
    f32 x;
    f32 y;
    f32 z;

    ref = DYNMODEL_USERDATA(effect);
    owner = ref->actor;
    if (owner->type == 0 || owner->unique_id != ref->uniqueId) {
        effect_destroyObject(&effect->header);
    }
    model = owner->ob_matrix_ptr;
    if (model == NULL) {
        return 0;
    }
    if ((owner->cullMask & 0xF) == 0xF) {
        return 0;
    }
    if (gIsMultiplayer != 0) {
        if (owner->CameraPtr != NULL && owner->mv != NULL &&
            owner->mv->SniperState != 0 && viewport == owner->CameraPtr->CNumber) {
            return 0;
        }
    } else {
        player = &movement_obj[0];
        if (player->CameraPtr != NULL && player->mv != NULL &&
            player->mv->SniperState != 0 && viewport == player->CameraPtr->CNumber) {
            return 0;
        }
    }
    // matching: the two unused labels stop IDO from hoisting the shared call arguments out of the branches
    if (gBoneMatricesFlushed != 0) {
    lbl2:
        guMtxL2F(modelMtx, (Mtx *)&model[1]);
    } else {
    lbl0:
        memcpy(modelMtx, &model[1], sizeof(modelMtx));
    }
    modelMtx[0][3] = 0.0f;
    modelMtx[1][3] = 0.0f;
    modelMtx[2][3] = 0.0f;
    modelMtx[3][3] = 1.0f;
    guMtxF2L(modelMtx, &effect->mtx[gDrawBufferIndex]);
    memcpy(viewMtx, &gViewProjMatrices[viewport], sizeof(viewMtx));
    viewMtx[0][3] = 0.0f;
    viewMtx[1][3] = 0.0f;
    viewMtx[2][3] = 0.0f;
    viewMtx[3][3] = 1.0f;
label: // matching: unused label keeps the viewMtx stores ahead of the argument setup
    math_transformPoint(modelMtx, 0.0f, 67.0f, 159.0f, &originWorld.x, &originWorld.y, &originWorld.z);
    math_transformPoint(viewMtx, originWorld.x, originWorld.y, originWorld.z, &origin[0], &origin[1], &origin[2]);
    origin[0] *= 2.002f;
    origin[1] *= 2.002f;
    /* keep texture coordinates near zero by removing whole 1024-unit tiles */
    originS = (s32)(origin[0] / 1024.0f) * 1024.0f;
    originT = (s32)(origin[1] / 1024.0f) * 1024.0f;
    {
        Vec3f world;
        Vec3f proj;

        for (i = 0; i < 20; i++) {
            Vtx_t *vtx = &effect->vertices[gDrawBufferIndex][viewport][i].v;

            x = vtx->ob[0];
            y = vtx->ob[1];
            z = vtx->ob[2];
            math_transformPoint(modelMtx, x, y, z, &world.x, &world.y, &world.z);
            math_transformPoint(viewMtx, world.x, world.y, world.z, &proj.x, &proj.y, &proj.z);
            proj.x *= 2.002f;
            proj.y *= 2.002f;
            vtx->tc[0] = proj.x - originS;
            vtx->tc[1] = proj.y - originT;
        }
    }
    return 1;
}

/* Message handler: removes the effect with its owner, and follows the owner when two objects swap. */
void effect_ownerModelMessage(DynModel *effect, ActorMsg *msg, u8 msgType) {
    ActorRef *ref = DYNMODEL_USERDATA(effect);
    Actor *owner = ref->actor;

    if (msgType == 0) {
        if ((owner == msg->ref.actor) || (msg->ref.uniqueId == owner->unique_id)) {
            effect_destroyObject(&effect->header);
        }
    } else if (msgType == 0x2D) {
        if (ref->actor == msg->swap.actorA) {
            ref->actor = msg->swap.actorB;
            ref->uniqueId = msg->swap.idB;
        } else if (ref->actor == msg->swap.actorB) {
            ref->actor = msg->swap.actorA;
            ref->uniqueId = msg->swap.idA;
        }
    }
}

/* The launch paths of effect_spawnTankExplosion's particles, in the space of the object's model matrix: start and
 * end points (the end minus the start gives the direction), sizes and extra rotations. */
static const Vec3f gTankDebris27Start = { 0.0f, 131.0f, 0.0f };
static const Vec3f gTankDebris27End = { 0.0f, 210.0f, 0.0f };
static const Vec3f gTankDebris28Start = { 0.0f, 116.0f, 200.0f };
static const Vec3f gTankDebris28Ends[2] = { { 90.0f, 189.0f, 201.0f }, { -75.0f, 192.0f, 183.0f } };
static const Vec3f gTankDebris29Starts[4] = {
    { 128.0f, -29.0f, -67.0f }, { -128.0f, -29.0f, -67.0f }, { -128.0f, -29.0f, 62.0f }, { 128.0f, -29.0f, 62.0f },
};
static const Vec3f gTankDebris29Ends[4] = {
    { 223.0f, 24.0f, -93.0f }, { -223.0f, 24.0f, -93.0f }, { -205.0f, 8.0f, 119.0f }, { 205.0f, 8.0f, 119.0f },
};
static const f32 gTankDebris29Sizes[4] = { 0.6000000238f, 0.6000000238f, 0.5f, 0.5f };
static const Vec3f gTankDebris2BStart = { 0.0f, 132.0f, 0.0f };
static const Vec3f gTankDebris2BEnds[2] = { { 95.0f, 125.0f, 32.0f }, { -95.0f, 125.0f, 32.0f } };
static const Vec3f gTankDebris2BRotations[2] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 180.0f, 0.0f } };
static const Vec3f gTankDebris2CStarts[4] = {
    { 114.0f, -17.0f, 45.0f }, { -114.0f, -17.0f, 45.0f }, { 114.0f, -17.0f, -58.0f }, { -114.0f, -17.0f, -58.0f },
};
static const Vec3f gTankDebris2CEnds[4] = {
    { 198.0f, 20.0f, 104.0f }, { -198.0f, 20.0f, 104.0f }, { 189.0f, 50.0f, -108.0f }, { -189.0f, 50.0f, -108.0f },
};
static const Vec3f gTankDebris2CRotations[4] = {
    { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { -21.0f, 179.0f, 0.0f }, { -21.0f, 179.0f, 0.0f },
};
static const f32 gTankDebris2CSizes[4] = { 0.4890000224f, 0.4890000224f, 0.6200000048f, 0.6200000048f };

/*
 * Spawns a burst effect at `obj` plus up to twelve particles launched along fixed paths in the space of
 * the object's model matrix (several of them picked at random).
 */
void effect_spawnTankExplosion(Actor *obj, u8 owner, s32 list) {
    JetSpraySpec burst;
    CollisionHit floor;
    Vec3f objPos;
    MtxF *mtx;
    ParticleSpawn part;

    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -0x3F;
    burst.angles.pitchRange = 0x34;
    burst.spray.countBase = 6;
    burst.spray.countRange = 5 >> ((gScreenLayout >= 2) ? 1 : 0);
    burst.spray.pos.x = obj->xpos;
    burst.spray.pos.y = obj->ypos + 25.0f;
    burst.spray.pos.z = obj->zpos;
    burst.spray.particleCountBase = 6;
    burst.spray.speedBase = 50.0f;
    burst.spray.speedRange = 42.0f;
    burst.spray.sizeBase = 98.0f;
    burst.spray.sizeRange = 68.0f;
    burst.spray.sizeSlopeBase = 609.0f;
    burst.spray.sizeSlopeRange = 271.0f;
    burst.spray.particleCountRange = 3 >> (((gScreenLayout >= 2) || (gPlayerCount >= 8)) ? 1 : 0);
    burst.spray.delayBase = 25;
    burst.spray.delayRange = 15;
    burst.spray.alphaBase = 100;
    burst.spray.alphaRange = 100;
    burst.spray.fadeStart = 12;
    burst.spray.fadeOutStart = 20;
    burst.spray.spread = 18.0f;
    burst.spray.drag = 0.7690000534f;
    burst.spray.gravityBase = -0.1690000147f;
    burst.spray.gravityRange = 0;
    if ((gScreenLayout >= 2) || (gPlayerCount >= 8)) {
        burst.spray.eventFunc = -1;
    } else {
        burst.spray.eventFunc = 0;
    }
    particle_spawnJetSprayBursts(&burst, owner, list);
    if ((obj->ob_matrix_ptr != NULL) && ((obj->cullMask & 0xF) != 0xF)) {
        collision_initHitFromObject(&floor, obj);
        mtx = &obj->ob_matrix_ptr[1];
        {
            f32 outA2, outA1, outA0, outB2, outB1, outB0;

            model_decomposeBoneMatrix(mtx, &outA0, &outA1, &outA2, &objPos.x, &objPos.y, &objPos.z, &outB0, &outB1, &outB2);
        }
        {
            s32 i, j, k;
            f32 speed, scale, accel;

            part.radius = 1.0f;
            part.scale.x = 1.0f;
            part.scale.y = 1.0f;
            part.scale.z = 1.0f;
            part.elasticity = 0.7f;
            part.rotVel.y = 0.0f;
            part.flags = 0x39E9;
            part.variant = 0;
            part.zone = 0;
            part.alpha = 0xFF;
            part.updateFn = 0xD;
            part.update2Fn = 0;
            part.floorHitFn = 0xC;
            part.floorType3Fn = 0;
            part.wallHitFn = 0;
            part.destroyFn = 0;
            part.squashFn = 0;
            part.matrixFn = 2;
            part.lightMode = 0;
            part.owner = obj;
            part.ownerId = obj->unique_id;
            part.fadeStart = 0xC;
            part.fadeRate = 0x15;
        scale = 0.001f;
        accel = -488.0f * scale;

        {
            f32 speed;

            speed = ((math_randomFloat() * 200.0f) + 83.0f) * scale;
            part.lifetime = (math_random() % 36U) + 30;
            part.model = 0x27;
            part.sizeXZ = 0.75f;
            part.sizeY = 0.75f;
            math_randomFloat();
            part.gravity = accel;
            math_transformOffset(&gTankDebris27Start, &part.pos, mtx);
            math_transformOffset(&gTankDebris27End, &part.vel, mtx);
            part.vel.x = part.vel.x - part.pos.x;
            part.vel.y = part.vel.y - part.pos.y;
            part.vel.z = part.vel.z - part.pos.z;
            part.vel.x = part.vel.x * speed;
            part.vel.y = part.vel.y * speed;
            part.vel.z = part.vel.z * speed;
            part.rot = objPos;
            part.rot.x += 109.0f;
            part.rotVel.x = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
            part.rotVel.z = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
            particle_newModelParticleWithFloor(&part, 3, 0xFF, &floor, 0, owner, list);
        }

        {
            f32 speed;

            speed = ((math_randomFloat() * 200.0f) + 83.0f) * scale;
            part.lifetime = (math_random() % 36U) + 30;
            part.model = 0x28;
            part.sizeXZ = 0.5f;
            part.sizeY = 0.5f;
            math_randomFloat();
            part.gravity = accel;
            math_transformOffset(&gTankDebris28Start, &part.pos, mtx);
            math_transformOffset(&gTankDebris28Ends[math_random() & 1], &part.vel, mtx);
            part.vel.x = part.vel.x - part.pos.x;
            part.vel.y = part.vel.y - part.pos.y;
            part.vel.z = part.vel.z - part.pos.z;
            part.vel.x = part.vel.x * speed;
            part.vel.y = part.vel.y * speed;
            part.vel.z = part.vel.z * speed;
            part.rot = objPos;
            part.rotVel.x = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
            part.rotVel.z = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
            particle_newModelParticleWithFloor(&part, 3, 0xFF, &floor, 0, owner, list);
        }

        part.model = 0x29;
        for (i = 0; i != 4; i++) {
            if (math_random() & 1) {
                speed = ((math_randomFloat() * 200.0f) + 83.0f) * scale;
                part.lifetime = (math_random() % 36U) + 30;
                part.sizeXZ = part.sizeY = gTankDebris29Sizes[i];
                math_randomFloat();
                part.gravity = accel;
                math_transformOffset(&gTankDebris29Starts[i], &part.pos, mtx);
                math_transformOffset(&gTankDebris29Ends[i], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rotVel.x = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                particle_newModelParticleWithFloor(&part, 3, 0xFF, &floor, 0, owner, list);
            }
        }

        part.model = 0x2B;
        math_transformOffset(&gTankDebris2BStart, &part.pos, mtx);
        part.sizeXZ = part.sizeY = 0.551f;
        for (j = 0; j != 2; j++) {
            if (math_random() & 1) {
                speed = ((math_randomFloat() * 200.0f) + 83.0f) * scale;
                part.lifetime = (math_random() % 36U) + 30;
                math_randomFloat();
                part.gravity = accel;
                math_transformOffset(&gTankDebris2BEnds[j], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += gTankDebris2BRotations[j].x;
                part.rot.y += gTankDebris2BRotations[j].y;
                part.rot.z += gTankDebris2BRotations[j].z;
                part.rotVel.x = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                particle_newModelParticleWithFloor(&part, 3, 0xFF, &floor, 0, owner, list);
            }
        }

        part.model = 0x2C;
        for (k = 0; k != 4; k++) {
            if (math_random() & 1) {
                speed = ((math_randomFloat() * 200.0f) + 83.0f) * scale;
                part.lifetime = (math_random() % 36U) + 30;
                part.sizeXZ = part.sizeY = gTankDebris2CSizes[k];
                math_randomFloat();
                part.gravity = accel;
                math_transformOffset(&gTankDebris2CStarts[k], &part.pos, mtx);
                math_transformOffset(&gTankDebris2CEnds[k], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += gTankDebris2CRotations[k].x;
                part.rot.y += gTankDebris2CRotations[k].y;
                part.rot.z += gTankDebris2CRotations[k].z;
                part.rotVel.x = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((math_randomFloat() * 28864.0f) + -14432.0f) * scale;
                particle_newModelParticleWithFloor(&part, 3, 0xFF, &floor, 0, owner, list);
            }
        }
        }
    }
}


/* Starts sound 0x4A7 at `obj`; effect_updateTankSound keeps it updated. */
void effect_startTankSound(Actor *obj) {
    sound_addDynamicSound(0x4A7, obj->xpos, obj->ypos, obj->zpos, 0x2000, 0x320, 0xC8,
                  (SoundRecordCallback)effect_updateTankSound, obj, 0, 8, 0);
}

/* A playing positional sound, as seen by its update callback. */
struct SoundChannel {
    /* 0x00 */ u16 sample;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ Actor *obj;
    /* 0x1C */ u32 userData; /* bits 0-6 state, 7 fading in, 8-15 fade volume, 16-27 timer */
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ u16 sound_id; /* voice handle */
};

/*
 * Update callback of the sounds started by effect_startTankSound: follows the object, switches between the
 * 0x4A7/0x4A8/0x4A9 sounds by how it moves, and fades the volume. Returns 1 when the sound should stop.
 */
s32 effect_updateTankSound(SoundChannel *chan, s32 active, u32 *volume, s32 pan, s32 *speedOut,
                  s32 mix, u16 *soundId) {
    Actor *obj;
    s32 state;
    s32 prevState;
    s32 sound;
    s32 param;
    s32 fade;
    s32 timer;
    s32 fadingIn;

    obj = chan->obj;
    if ((obj != NULL) && (obj->energy != 0)) {
        state = 0;
        sound = 0;
        prevState = chan->userData & 0x7F;
        fade = (chan->userData >> 8) & 0xFF;
        timer = (chan->userData >> 16) & 0xFFF;
        fadingIn = (chan->userData >> 7) & 1;
        chan->x = obj->xpos;
        chan->y = obj->ypos;
        chan->z = obj->zpos;
        if (((obj->speed > 1.0f) && (obj->stickspeed > 0.0f)) ||
            ((obj->speed < -1.0f) && (obj->stickspeed < 0.0f))) {
            state = 1;
        } else if (((obj->speed > 5.0f) && (obj->stickspeed <= 0.0f)) ||
                   ((obj->speed < -5.0f) && (obj->stickspeed >= 0.0f))) {
            state = 2;
        }
        // matching: emits no code; the extra reads of state raise its uopt register priority
        // above sound's, which puts state in v1 and sound in t0
        if (state + state + state + state) {}
        if ((state ^ prevState) & 0x7F) {
            if ((((state ^ prevState) & 0x7F) & 2) && (state & 2)) {
                sound = 0x4A5;
            } else {
                sound = sound_pickVariant(0x4A3, 4);
            }
            if (!sound_isActorSound1Playing(obj)) {
                sound_playActorSound2(sound, obj, 0x6D60, 300, 1800);
            }
            sound = 0;
        }
        if ((state != 0) && (*soundId == 0x4A7)) {
            sound = 0x4A8;
            param = 0x2EE0;
        } else if ((state == 0) && ((*soundId == 0x4A9) || (*soundId == 0x4A8))) {
            if (*soundId == 0x4A9) {
                return 1;
            }
            sound = 0x4A7;
            param = 0x2000;
        }
        if (sound != 0) {
            sound_addDynamicSound(sound, obj->xpos, obj->ypos, obj->zpos, param, 0x320, 0xC8,
                          (SoundRecordCallback)effect_updateTankSound, obj, state | 0x12CFF00, 8, 0);
            goto end; // matching: a plain return 1 here is scheduled differently
        }
        if (*soundId != 0x4A7) {
            if (timer == 0) {
                *volume = (*volume * fade) >> 8;
                if (fadingIn) {
                    fade += 8;
                    if (fade >= 0x100) {
                        fade = 0xFF;
                        timer = (math_random() & 0x200) + 300;
                        fadingIn = FALSE;
                    }
                } else {
                    fade -= 8;
                    if (fade <= 0) {
                        return 1;
                    }
                }
            } else {
                timer -= gFrameTicks;
                if (timer <= 0) {
                    timer = 0;
                    if (*soundId == 0x4A8) {
                        sound_addDynamicSound(0x4A9, obj->xpos, obj->ypos, obj->zpos, 0x2EE0, 0x320,
                                      0xC8, (SoundRecordCallback)effect_updateTankSound, obj, state | 0x880, 8, 0);
                        timer = (math_random() & 0x200) + 900;
                    }
                }
            }
            *speedOut = fabsf(obj->speed) * obj->vtilt * 0.5f;
        }
        chan->userData = (fadingIn << 7) | state | (fade << 8) | (timer << 16);
        if ((*volume == 0) || (chan->flags & 0x80)) {
            if (chan->sound_id != 0) {
                sound_stop(chan->sound_id);
                chan->sound_id = 0;
            }
            *soundId = 0;
            chan->flags &= ~0x80;
            *volume = 0;
        }
        return 0;
    }
end:
    return 1;
}

/*
 * Impact effect for a shot from `from` towards `to`: effects at the origin, then a ray cast along the
 * shot and, where it hits a surface, impact effects there.
 */
void effect_spawnShotImpact(Actor *obj, Vec3f *from, Vec3f *to, u8 owner, s32 list) {
    CollisionTri surf;
    Vec3f hit;
    Vec3f delta;
    s32 hitInfo;
    Vec3f edgeA;
    Vec3f edgeB;
    Vec3f normal;
    SurfaceJetSpray fx;

    if (obj != NULL) {
        actor_playImpactSound(0, from->x, from->y, from->z, 0);
        delta.x = to->x - from->x;
        delta.y = to->y - from->y;
        delta.z = to->z - from->z;
        weapon_wobbleNearbyCameras(from, owner, list);
        weapon_rumbleNearbyPlayers(from, owner, list);
        weapon_spawnHitGrowFade(from, owner, list);
        effect_spawnImpactLight(from, owner, list);
        if (collision_castRayWorld(from->x, from->y, from->z, delta.x, delta.y, delta.z, 0, &surf, &hit.x, &hit.y, &hit.z, NULL,
                          &hitInfo, 0, 0.0f) != 0) {
            if (math_isObjectHidden(hitInfo) != 0) {
                effect_spawnRandomImpactMark(&surf, &hit, owner & 0xFF, list);
            }
            if (math_getTriEdges(&surf, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, owner, list);
            }
            fx.source.normal.x = -delta.x;
            fx.source.normal.y = -delta.y;
            fx.source.normal.z = -delta.z;
            fx.source.triangle = surf;
            fx.source.spread = 494.0f;
            fx.spray.pos = hit;
            fx.spray.countBase = 3;
            fx.spray.countRange = 4;
            fx.spray.speedBase = 0.0609f;
            fx.spray.speedRange = 0.0252f;
            fx.spray.sizeBase = 45.0f;
            fx.spray.sizeRange = 53.0f;
            fx.spray.sizeSlopeBase = 203.0f;
            fx.spray.sizeSlopeRange = 414.0f;
            fx.spray.particleCountBase = 7;
            fx.spray.particleCountRange = 3;
            fx.spray.spread = 15.0f;
            fx.spray.drag = 0.8800000548f;
            fx.spray.gravityBase = -0.3980000317f;
            fx.spray.gravityRange = 0.3910000324f;
            fx.spray.delayBase = 25;
            fx.spray.delayRange = 15;
            fx.spray.alphaBase = 100;
            fx.spray.alphaRange = 100;
            fx.spray.fadeStart = 12;
            fx.spray.fadeOutStart = 20;
            fx.spray.eventFunc = 0;
            particle_spawnSurfaceJetSpray(&fx, owner, list);
            {
                const s32 variantIds[9] = { 4, 4, 7, 8, 24, 25, 29, 30, 32 };
                const f32 variantScales[9] = { 1.0f, 1.0f, 0.150000006f, 0.150000006f, 1.0f, 1.0f, 0.09700000286f,
                                               1.0f, 0.200000003f };
                SurfaceDebrisBurst desc;

                desc.countBase = 6;
                desc.countRange = 6;
                desc.pos = hit;
                desc.source.normal.x = -delta.x;
                desc.source.normal.y = -delta.y;
                desc.source.normal.z = -delta.z;
                desc.source.triangle = surf;
                desc.source.spread = 456.0f;
                desc.speedBase = 0.02000000142f;
                desc.speedRange = 0.02000000142f;
                desc.gravityBase = -1.71f;
                desc.gravityRange = 0.448f;
                desc.lifeBase = 50;
                desc.lifeRange = 20;
                desc.scaleBase = 0.499f;
                desc.scaleRange = 0.7010000348f;
                desc.spin = 21.502f;
                particle_spawnSurfaceDebrisBurst(&desc, variantIds, variantScales, 9, owner, list);
            }
        }
    }
}


/*
 * Impact effect halfway between `a` and `b`: a burst there, a ground impact on the floor below it,
 * and a shower of debris.
 */
void effect_spawnMidpointImpact(Actor *obj, f32 *a, f32 *b, u8 owner, s32 list) {
    Vec3f center;
    JetSpraySpec burst;
    CollisionHit floor;
    Vec3f probe;
    Vec3f hit;
    Vec3f edgeA;
    Vec3f edgeB;
    Vec3f normal;

    if (obj != NULL) {
        center.x = (b[0] + a[0]) * 0.5f;
        center.y = (b[1] + a[1]) * 0.5f;
        center.z = (b[2] + a[2]) * 0.5f;
        weapon_spawnHitGrowFade(&center, owner, list);
        weapon_wobbleNearbyCameras(&center, owner, list);
        weapon_rumbleNearbyPlayers(&center, owner, list);
        actor_playImpactSound(1, center.x, center.y, center.z, 0);
        effect_spawnImpactLight(&center, owner, list);
        burst.angles.yawBase = 0;
        burst.angles.yawRange = 0xFF;
        burst.angles.pitchBase = -0x40;
        burst.angles.pitchRange = 0x47;
        burst.spray.countBase = 6;
        burst.spray.countRange = 4 >> ((gScreenLayout >= 2) ? 1 : 0);
        burst.spray.pos = center;
        burst.spray.particleCountBase = 7;
        burst.spray.speedBase = 23.0f;
        burst.spray.speedRange = 30.0f;
        burst.spray.sizeBase = 45.0f;
        burst.spray.sizeRange = 53.0f;
        burst.spray.sizeSlopeBase = 203.0f;
        burst.spray.sizeSlopeRange = 414.0f;
        burst.spray.particleCountRange = 3 >> ((gScreenLayout >= 2 || gPlayerCount >= 8) ? 1 : 0);
        burst.spray.delayBase = 25;
        burst.spray.delayRange = 15;
        burst.spray.alphaBase = 100;
        burst.spray.alphaRange = 100;
        burst.spray.fadeStart = 12;
        burst.spray.fadeOutStart = 20;
        burst.spray.spread = 15.0f;
        burst.spray.drag = 0.8720000386f;
        burst.spray.gravityBase = -0.3980000317f;
        burst.spray.gravityRange = 0.3640000224f;
        if (gScreenLayout >= 2 || gPlayerCount >= 8) {
            burst.spray.eventFunc = -1;
        } else {
            burst.spray.eventFunc = 0;
        }
        particle_spawnJetSprayBursts(&burst, owner, list);
        collision_initHitFromObject(&floor, obj);
        probe.x = center.x;
        probe.y = center.y + 100.0f;
        probe.z = center.z;
        if (collision_probeFloorSurfABSet0Cached(&probe, 0, center.y - 1000.0f, &floor) != 0) {
            hit.x = probe.x;
            hit.y = floor.height;
            hit.z = probe.z;
            effect_spawnRandomImpactMark(&floor.tri, &hit, owner, list);
            if (math_getTriEdges(&floor.tri, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, owner, list);
            }
        }
        {
            DebrisBurst debris;
            s32 variantIds[3] = { 45, 46, 42 };
            f32 variantScales[3] = { 1.0f, 1.0f, 1.0f };

            debris.countBase = 4;
            debris.countRange = 8;
            debris.pos = center;
            debris.speedBase = 12.0f;
            debris.speedRange = 6.0f;
            debris.angles.yawBase = 0;
            debris.angles.yawRange = 0xFF;
            debris.angles.pitchBase = -0x3F;
            debris.angles.pitchRange = 0x57;
            debris.lifeBase = 50;
            debris.lifeRange = 10;
            debris.gravityBase = -1.438000083f;
            debris.gravityRange = 0.4490000308f;
            debris.scaleBase = 0.2480000108f;
            debris.scaleRange = 0.09900000691f;
            debris.spin = 22.85900116f;
            particle_spawnDebrisBurst(&debris, variantIds, variantScales, 3, 0.0f, 1, owner, list);
        }
    }
}

/* Spawns a short-lived effect at `pos` with a random size and lifetime. */
void effect_spawnImpactLight(Vec3f *pos, u8 owner, s32 list) {
    LightEffectHeader header;
    Vec3i position;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (math_random() & 0xF) + 5;
    header.lightType = 0;

    position.x = pos->x;
    position.y = pos->y;
    position.z = pos->z;

    light_createWithEffect(&header, &position, (math_random() % 21U) + 30, 0xFF, 0x61, 0x36, 0xFF, 0, 0, owner, list);
}

/* matching: compiled with s32 where emissionsource.c's definition has u8 and s16 (args 2, 5-11), so the calls do not narrow */
void particle_spawnImpactMark(f32, s32, CollisionTri *, Vec3f *, s32, s32, s32, s32, s32, s32, s32, s32);
/* Spawns a randomised particle_spawnImpactMark effect on surface `surf` at `pos`. */
void effect_spawnRandomImpactMark(CollisionTri *surf, Vec3f *pos, s32 owner, s32 list) {
    s32 pad; // matching: unused stack slot
    u32 rnd1;
    f32 rnd0;
    u32 rnd2;

    rnd0 = math_randomFloat();
    rnd1 = math_random();
    rnd2 = math_random();
    particle_spawnImpactMark(rnd0 * 75.0f + 75.0f, ((rnd1 % 56) + 200) & 0xFF, surf, pos,
                  (rnd2 % 205) + 299, 1, 1, 0, 0, 0, (u8)owner, 0);
}

static const Vec3f gTankHeadlightBeamStart = { 0.0f, 100.0f, 80.0f };
static const Vec3f gTankHeadlightBeamEnd = { 0.0f, -90.0f, 450.0f };

/* Spawns the light_createBeam effect (tables gTankHeadlightBeamStart/gTankHeadlightBeamEnd) on `obj`. */
void effect_attachTankHeadlightBeam(Actor *obj) {
    if (obj != NULL) {
        light_createBeam(obj, 1, &gTankHeadlightBeamStart, &gTankHeadlightBeamEnd, 0.01769899949f, 2, 300,
                      0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
    }
}



/* Data block of the object returned by effect_newEffectObj. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
} Data15147A80;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ Data15147A80 *data;
} Fx15147A80;

/*
 * Spawns three effects that follow `obj` (the first only while gScreenLayout < 2 and gPlayerCount < 5),
 * each with a reference back to its owner.
 */
void func_151BC104(Actor *obj, u8 fxOwner, s32 fxList) {
    BloodTrail *fx1;

    if ((gScreenLayout < 2) && (gPlayerCount < 5)) {
        BloodTrailDesc desc;
        ActorRef ref;

        desc.radius = 15.0f;
        desc.x = &obj->xpos;
        desc.y = &obj->ypos;
        desc.z = &obj->zpos;
        desc.lifetime = 300;
        desc.rate = 0.3033999801f * gLayoutDetailScale;
        desc.flags = 4;
        desc.emitter = 4;
        desc.mode = 1;
        desc.callback = 0;
        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        fx1 = blood_spawnTrail(&desc, 8, fxOwner, fxList);
        if (fx1 != NULL) {
            memcpy(fx1 + 1, &ref, sizeof(ref)); /* the owner reference, in the 8 extra bytes after the trail */
        }
    }
    {
        EffectTemplate desc;
        Fx15147A80 *fx2;
        Data15147A80 data;

        desc.itemCount = 40;
        desc.pos.x = obj->xpos;
        desc.pos.y = obj->ypos;
        desc.pos.z = obj->zpos;
        desc.life = 300;
        desc.flags = 6;
        desc.kind = 12;
        data.owner = obj;
        data.ownerId = obj->unique_id;
        data.pos = desc.pos;
        data.unk14 = 0.0f;
        data.unk18 = 0.0f;
        data.unk20 = -16384.0f;
        data.unk1C = -16384.0f;
        fx2 = effect_newEffectObj(&desc, 40, 20, 10, 10, 10, 0, 0, 0, fxOwner, fxList);
        if (fx2 != NULL) {
            memcpy(fx2->data, &data, sizeof(data));
        }
    }
    {
        ActorRef ref;
        LightEffect *fx3;
        LightEffectHeader header;

        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        header.flags = 2;
        header.update = 14;
        header.lifetime = 300;
        header.lightType = 31;
        fx3 = light_createTracking(&header, &obj->xpos, &obj->ypos, &obj->zpos,
                           0, 0, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 8, fxOwner, fxList);
        if (fx3 != NULL) {
            memcpy(&fx3->data.trackActor.ref, &ref, sizeof(ref));
        }
    }
}

/* Drip emitter callback (gBloodDripEmitters[39]): spawns one particle at (x, y, z), flung in a random
 * direction and pushed against the drip's movement (dx, dy, dz). The particle_spawnDragFadeSprite arguments
 * after velocity are drag, gravity, size, timer, alpha, fade timings and an (r, g, b) of (203, 0, 0). */
void effect_emitBloodDropSpray(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    Vec3f position;
    Vec3f velocity;
    Vec3f offset;
    f32 scale;
    f32 fraction_b;
    u32 random_b;
    u32 random_a;
    f32 fraction_a;

    position.x = x;
    position.y = y;
    position.z = z;
    random_a = math_random();
    random_b = math_random();
    fraction_a = math_randomFloat();
    math_dirFromBinAngles(random_a & 0xFF, random_b % 65U - 32, fraction_a * 1680.0f * 0.01f, &velocity);
    scale = (math_randomFloat() * 202.0f + 208.0f) * 0.001f;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    velocity.x += -dx * gFrameDeltaInv * scale;
    velocity.y += -dy * gFrameDeltaInv * scale;
    velocity.z += -dz * gFrameDeltaInv * scale;
    fraction_a = math_randomFloat();
    fraction_b = math_randomFloat();
    random_a = math_random();
    random_b = math_random();
    particle_spawnDragFadeSprite(&position, &offset, &velocity, 1.0f, (fraction_a * 1049.0f + -1022.0f) * 0.001f,
                  fraction_b * 124.0f + 202.0f, (random_a & 0xF) + 20, random_b % 201U + 55, 15, 15, 0, -1,
                  203, 0, 0, effect->owner, effect->kind);
}

/* Returns whether the effect's owner is still active. */
s32 effect_isBloodTrailOwnerActive(OwnedEffect40 *effect) {
    Actor *owner = effect->owner.actor;

    if (owner->type == 0) {
        return 0;
    }
    return 1;
}

/* Message handler: message 0 for its owner frees the effect; 0x2D makes it follow the owner through a swap. */
void effect_bloodTrailOwnerMessage(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &effect->owner;
    Actor *actor = owner->actor;

    if (msgId == 0) {
        if (actor == msg->ref.actor || owner->uniqueId == msg->ref.uniqueId) {
            effect_destroyObject(&effect->trail.header);
        }
    } else if (msgId == 0x2D) {
        if (owner->actor == msg->swap.actorA) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
        } else if (owner->actor == msg->swap.actorB) {
            owner->actor = msg->swap.actorA;
            owner->uniqueId = msg->swap.idA;
        }
    }
}

/* Ages the trail: drops expired points and fades the last ones. Returns 0 once a finished trail is empty. */
s32 effect_ageRibbonTrail(RibbonTrail *trail) {
    RibbonTrailPoint *points;
    RibbonTrailPoint *point;
    s32 i;
    s16 lifetime;
    s32 alpha;

    points = trail->points;
    if (trail->head.count < 2 && (trail->head.tmpl.flags & 8)) {
        return 0;
    }
    i = trail->head.end;
    while (i != trail->head.start) {
        i--;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        point = &points[i];
        point->lifetime -= gFrameTicks;
        lifetime = point->lifetime;
        if (lifetime < 0) {
            /* this point and every older one expire */
            while (i != trail->head.start) {
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
        } else if (lifetime < 10) {
            alpha = lifetime * 25;
            if (alpha < point->alpha) {
                point->alpha = alpha;
            }
        }
    }
    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    return 1;
}

/* Follows the owner and emits trail points evenly along the distance it moved. */
s32 effect_followRibbonTrail(RibbonTrail *trail) {
    RibbonTrailData *data;
    RibbonTrailPoint *points;
    Actor *owner;
    Vec3f delta;
    RibbonTrailPoint *point;
    f32 dist;
    f32 inv;
    f32 startU;

    data = trail->data;
    points = trail->points;
    owner = data->owner.actor;
    if (owner->type == 0 || owner->unique_id != data->owner.uniqueId) {
        trail->head.tmpl.flags |= 8;
        return 1;
    }
    trail->head.tmpl.pos.x = owner->xpos;
    trail->head.tmpl.pos.y = owner->ypos;
    trail->head.tmpl.pos.z = owner->zpos;
    delta.x = trail->head.tmpl.pos.x - data->lastPos.x;
    delta.y = trail->head.tmpl.pos.y - data->lastPos.y;
    delta.z = trail->head.tmpl.pos.z - data->lastPos.z;
    dist = math_vec3Length(&delta);
    data->pending += dist * 0.04870799929f * gFrameDelta;
    data->texU += dist * 9.708835602f;
    if (data->pending > 1.0f) {
        Vec3f pos;
        f32 u;
        f32 y;
        f32 yStep;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 uSpan;

        inv = 1.0f / data->pending;
        y = data->unk18 + gFrameDelta;
        pos = data->lastPos;
        startU = data->lastTexU;
        u = startU;
        yStep = -(y * inv);
        dx = delta.x * inv;
        dy = delta.y * inv;
        dz = delta.z * inv;
        uSpan = data->texU - startU;
        do {
            point = &points[trail->head.end];
            point->pos = pos;
            point->lifetime = 12;
            point->alpha = 100;
            point->texU = u;
            while (point->texU > 16384.0f) {
                point->texU -= 32768.0f;
            }
            trail->head.end++;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                /* ring full: drop the oldest point */
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            u += uSpan * inv;
            pos.x += dx;
            pos.y += dy;
            pos.z += dz;
            y += yStep;
            data->pending -= 1.0f;
        } while (data->pending > 1.0f);
        data->lastPos = pos;
        data->lastTexU = u;
        data->unk18 = y;
    }
    return 1;
}

/* Draws the trail as a camera-facing ribbon, two vertices per point. */
Gfx *effect_drawRibbonTrail(RibbonTrail *trail, Gfx *gfx, s16 viewport) {
    Vtx *vtx;
    RibbonTrailPoint *points;
    Vec3f *camPos;
    u8 dirty;
    Vec3f prev;
    Vec3f cur;
    f32 viewX;
    f32 viewY;
    f32 viewZ;
    f32 edgeX;
    f32 edgeY;
    f32 edgeZ;
    f32 sideX;
    f32 sideY;
    f32 sideZ;
    s32 i;
    s32 j;
    f32 lenSq;
    u8 curAlpha;
    f32 prevTexU;
    f32 curTexU;
    u8 prevAlpha;
    f32 scale;

    if (trail->head.count >= 2) {
        particle_getViewBuffer(trail->buffers, viewport, (trail->head.tmpl.itemCount + 5) * 2 * sizeof(Vtx), &vtx, NULL);
        if (vtx != NULL) {
            points = trail->points;
            camPos = math_getCameraPos(viewport);
            dirty = 1;
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[31], 0, 0, 0, 0, 0x1F, 0, 0, &dirty, 3);
            gfx = math_gfxSetEnvColor(gfx, 0xFF, 0xFF, 0xFF, 0xFF, &dirty);
            gfx = particle_setCombineMode(gfx, 0x4E, &dirty);
            gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x1F0600);
            gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[3].cycle1 | gEffectRenderModes[3].cycle2, &dirty);
            if (trail->head.tmpl.flags & 2) {
                /* ribbon starts at the live position */
                i = trail->head.end - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                cur = trail->head.tmpl.pos;
                curAlpha = 0x80;
                curTexU = 0.0f;
            } else {
                j = trail->head.end - 1;
                if (j < 0) {
                    j = trail->head.tmpl.itemCount - 1;
                }
                i = j - 1;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                cur = points[j].pos;
                curAlpha = points[j].alpha;
                curTexU = points[j].texU;
            }
            prev = points[i].pos;
            prevAlpha = points[i].alpha;
            prevTexU = points[i].texU;

            edgeX = cur.x - prev.x;
            edgeY = cur.y - prev.y;
            edgeZ = cur.z - prev.z;
            viewX = cur.x - camPos->x;
            viewY = cur.y - camPos->y;
            viewZ = cur.z - camPos->z;
            sideX = (edgeY * viewZ) - (viewY * edgeZ);
            sideY = (edgeZ * viewX) - (viewZ * edgeX);
            sideZ = (edgeX * viewY) - (viewX * edgeY);
            lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
            if (lenSq == 0.0f) {
                sideZ = sideY = sideX = 0.0f;
            } else {
                scale = 35.0f / sqrtf(lenSq);
                sideX = sideX * scale;
                sideY = sideY * scale;
                sideZ = sideZ * scale;
            }
            vtx->v.ob[0] = cur.x + sideX;
            vtx->v.ob[1] = cur.y + sideY;
            vtx->v.ob[2] = cur.z + sideZ;
            vtx->v.tc[0] = curTexU;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = cur.x - sideX;
            vtx->v.ob[1] = cur.y - sideY;
            vtx->v.ob[2] = cur.z - sideZ;
            vtx->v.tc[0] = curTexU;
            vtx->v.tc[1] = 0x7FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = curAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                j = i;
                edgeX = cur.x - prev.x;
                edgeY = cur.y - prev.y;
                edgeZ = cur.z - prev.z;
                viewX = prev.x - camPos->x;
                viewY = prev.y - camPos->y;
                viewZ = prev.z - camPos->z;
                sideX = (edgeY * viewZ) - (viewY * edgeZ);
                sideY = (edgeZ * viewX) - (viewZ * edgeX);
                sideZ = (edgeX * viewY) - (viewX * edgeY);
                lenSq = (sideX * sideX) + (sideY * sideY) + (sideZ * sideZ);
                if (lenSq == 0.0f) {
                    sideZ = sideY = sideX = 0.0f;
                } else {
                    scale = 35.0f / sqrtf(lenSq);
                    sideX = sideX * scale;
                    sideY = sideY * scale;
                    sideZ = sideZ * scale;
                }
                vtx->v.ob[0] = prev.x + sideX;
                vtx->v.ob[1] = prev.y + sideY;
                vtx->v.ob[2] = prev.z + sideZ;
                vtx->v.tc[0] = prevTexU;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = prevAlpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = prev.x - sideX;
                vtx->v.ob[1] = prev.y - sideY;
                vtx->v.ob[2] = prev.z - sideZ;
                vtx->v.tc[0] = prevTexU;
                vtx->v.tc[1] = 0x7FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = prevAlpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                i--;
                if (i < 0) {
                    i = trail->head.tmpl.itemCount - 1;
                }
                if (j != trail->head.start) {
                    cur = prev;
                    prev = points[i].pos;
                    prevAlpha = points[i].alpha;
                    prevTexU = points[i].texU;
                }
            } while (j != trail->head.start);
        }
    }
    return gfx;
}

/* Message handler: message 0 for its owner finishes the trail; 0x2D makes it follow the owner through a swap. */
void effect_ribbonTrailMessage(RibbonTrail *trail, ActorMsg *msg, u8 msgId) {
    ActorRef *owner;
    Actor *actor;
    Actor *actorA;

    owner = &trail->data->owner;
    actor = owner->actor;
    if (msgId == 0) {
        if (actor == msg->ref.actor || msg->ref.uniqueId == owner->uniqueId) {
            u16 *flags = &trail->head.tmpl.flags; // matching: keeps the flags store out of the jr delay slot

            trail->head.updateKind2 = 0;
            *flags |= 8;
        }
    } else if (msgId == 0x2D) {
        actorA = msg->swap.actorA;
        if (actorA == owner->actor) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
            return;
        }
        if (owner->actor == msg->swap.actorB) {
            owner->actor = actorA;
            owner->uniqueId = msg->swap.idA;
        }
    }
}

/* Returns whether the effect's owner is still the same active actor. */
s32 effect_isLightTrackOwnerValid(OwnedEffect28 *effect) {
    Actor *owner = effect->owner.actor;

    if (owner->type == 0) {
        return 0;
    }
    if (owner->unique_id != effect->owner.uniqueId) {
        return 0;
    }
    return 1;
}

/* Spawns two mirrored blood strips on bone 1 of actor. */
void effect_spawnBloodStrips(Actor *actor, s32 owner, s32 list) {
    BloodStripDesc desc;
    struct {
        s16 value;
        u8 pad[2]; // matching: IDO gives the initial value a word-aligned slot
    } sampleAge;
    AgedBloodStrip *strip;

    if (actor == NULL) {
        return;
    }

    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offsetA.x = 31.0f;
    desc.offsetA.y = -34.0f;
    desc.offsetA.z = -117.0f;
    desc.offsetB.x = 35.0f;
    desc.offsetB.y = -24.0f;
    desc.offsetB.z = -182.0f;
    desc.flags = 0;
    desc.duration = 50;
    desc.pause = 100;
    desc.lifetime = 300;
    desc.sampleCallback = 4; /* effect_resetBloodStripAge */
    desc.emitCallback = 4;   /* effect_emitBloodStripParticle */
    desc.rateCallback = 3;   /* effect_getBloodStripRate */
    desc.rate = 0.3006100059f;
    desc.mode = 5;
    desc.updateCallback = -1;

    sampleAge.value = 0;

    strip = blood_spawnStrip(&desc, sizeof(sampleAge.value));
    if (strip != NULL) {
        memcpy(&strip->sampleAge, &sampleAge.value, sizeof(sampleAge.value));
    }

    desc.offsetA.x = -desc.offsetA.x;
    desc.offsetB.x = -desc.offsetB.x;

    strip = blood_spawnStrip(&desc, sizeof(sampleAge.value));
    if (strip != NULL) {
        memcpy(&strip->sampleAge, &sampleAge.value, sizeof(sampleAge.value));
    }
}

/* BloodStrip sample callback: restarts the sample age. */
s32 effect_resetBloodStripAge(AgedBloodStrip *strip) {
    strip->sampleAge = 0;
    return 1;
}

/* BloodStrip emit callback: spawns one blood droplet at pointA moving towards pointB. */
void effect_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velA, Vec3f *velB, f32 time,
                   BloodStrip *strip) {
    SpriteParticle *particle; /* with data.dragOwner */
    Actor *owner;
    ParticleDesc68 desc;
    f32 drag;
    s32 pad;  // matching: stack slot of the original frame
    ActorRef ownerRef;
    s32 pad2; // matching: stack slot of the original frame

    owner = strip->owner;
    if ((owner->cullMask & 0xF) != 0xF) {
        desc.texture = 0x29;
        desc.renderMode = 0xE03;
        desc.unk0 = 0x200005;
        desc.unk4 = 0;
        desc.frame = 0;
        desc.frameRate = 0;
        desc.gravity = ((math_randomFloat() * 251.0f) + 44.0f) * 0.001000000047f;
        desc.fadeStart = 20;
        desc.fadeRate = 12;
        desc.flags = (math_random() & 1 ? PARTICLE_FLIP_S : 0) |
                     (PARTICLE_LIFETIME | PARTICLE_GRAVITY | PARTICLE_MOVE | (math_random() & 1 ? PARTICLE_FLIP_T : 0)) |
                     PARTICLE_FADE | PARTICLE_SCALE | PARTICLE_SCALE_MUL | PARTICLE_USE_OFFSET | PARTICLE_HANDLERS |
                     PARTICLE_DRAW_FUNC;
        desc.primColorMode = 3;
        desc.envColorMode = 3;
        desc.deathFunc = -1;
        desc.drawFunc = -1;
        desc.handlerSet = 7;
        desc.scaleStart = 34;
        desc.r = 221;
        desc.g = 211;
        desc.b = 205;
        desc.a = 255;
        desc.r2 = 87;
        desc.g2 = 85;
        desc.b2 = 90;
        drag = 0.9347190261f;
        desc.scaleRate = 1.034358025f;
        desc.alpha = (math_random() % 156U) + 100;
        desc.brightness = 255;
        desc.timer = (math_random() % 18U) + 25;
        desc.width = desc.height = (math_randomFloat() * 53.0f) + 49.0f;
        desc.vel.x = (pointB->x - pointA->x) * strip->power;
        desc.vel.y = (pointB->y - pointA->y) * strip->power;
        desc.vel.z = (pointB->z - pointA->z) * strip->power;
        desc.pos.x = pointA->x - strip->owner->xpos;
        desc.pos.y = pointA->y - strip->owner->ypos;
        desc.pos.z = pointA->z - strip->owner->zpos;
        desc.pos.x += desc.vel.x * time;
        desc.pos.y += desc.vel.y * time;
        desc.pos.z += desc.vel.z * time;
        ownerRef.actor = strip->owner;
        ownerRef.uniqueId = strip->owner->unique_id;
        desc.updateFunc = 0x13;
        desc.offset.x = strip->owner->xpos;
        desc.offset.y = strip->owner->ypos;
        desc.offset.z = strip->owner->zpos;
        particle = particle_newOwnedSpriteParticle(&desc, 1, 0x10, strip->header.owner, strip->header.kind);
        if (particle != NULL) {
            memcpy(&particle->data.dragOwner.drag, &drag, sizeof(drag));
            memcpy(&particle->data.dragOwner.owner, &ownerRef, sizeof(ownerRef));
        }
    }
}


/* BloodStrip rate callback: the rate grows with the time since the last sample. */
f32 effect_getBloodStripRate(AgedBloodStrip *strip) {
    f32 rate;

    // matching: `1.0f + 1.0f` keeps the multiply by 2 (a 2.0f literal becomes an add); 40 * 0.001f folds
    // to the ROM's 0.04000000283f
    rate = strip->sampleAge * (1.0f + 1.0f) * 0.001000000047f + 40.0f * 0.001000000047f;
    strip->sampleAge += gFrameTicks;
    return rate;
}

/* Particle update: keeps the droplet moving with its owner, then runs the standard update. */
void effect_updateBloodStripParticle(SpriteParticle *particle, Vec3f *prevPos) {
    Actor *owner = particle->data.dragOwner.owner.actor;

    if (owner->type != 0 && owner->character_type != 0xFF) {
        particle->head.desc.offset.x = owner->xpos;
        particle->head.desc.offset.y = owner->ypos;
        particle->head.desc.offset.z = owner->zpos;
    }
    particle_updateDrag(particle, prevPos);
}

/* Broadcasts message 0x3B for actor. */
void effect_sendBurstOwnerGone(Actor *actor) {
    ActorRef ref;

    ref.actor = actor;
    ref.uniqueId = actor->unique_id;
    effect_sendEventToTimedEffects((s32)&ref, 0x3B);
}

/* Spawns the burst effect on actor: two sparks and two glows that the effect keeps on the actor. */
void effect_spawnBoneBurst(Actor *actor, u8 owner, s32 list) {
    BurstEffect *effect;
    BurstData burst;
    BurstData *data;

    burst.owner = actor;
    burst.ownerId = actor->unique_id;
    burst.sparks[0] = NULL;
    burst.sparks[1] = NULL;
    burst.glows[0] = NULL;
    burst.glows[1] = NULL;
    effect = effect_newTimedEffect(300, -1, 0x40, -1, 0, 0x33, sizeof(BurstData), owner, list);
    if (effect != NULL) {
        data = &effect->data;
        memcpy(data, &burst, sizeof(burst));
        {
            BurstSparkData spark;
            ParticleDef sparkEffect;

            spark.flags = 0;
            spark.scale = (actor->scale_xz + actor->scale_y) * 0.5f;
            spark.owner = actor;
            spark.ownerId = actor->unique_id;
            spark.widthBase = 15.045272f;
            spark.widthRange = 5.8676558f;
            spark.widthTarget = 12.111444f;
            spark.heightBase = 0.396174f;
            spark.widthTimer = 0.0f;
            spark.heightTimer = 0.0f;
            spark.alphaTimer = 0.0f;
            sparkEffect.width = 0.0f;
            sparkEffect.height = 0.0f;
            spark.alpha = 0.0f;
            spark.beamWidthTarget = 1.0f;
            spark.beamWidth = 1.0f;
            spark.beamLengthTarget = 1.0f;
            spark.beamLength = 1.0f;
            spark.burst = effect;
            sparkEffect.texture = 0x5F;
            sparkEffect.type = 8;
            sparkEffect.renderModes = 0x2203;
            sparkEffect.lifetime = 300;
            sparkEffect.frame = 0;
            sparkEffect.frameSpeed = 0;
            sparkEffect.rgba[0] = 0xFF;
            sparkEffect.rgba[1] = 0xFF;
            sparkEffect.rgba[2] = 0xFF;
            sparkEffect.rgba[3] = 0xFF;
            spark.heightRange = 0.1042563f;
            spark.heightRange2 = 0.5197371f;
            spark.heightTarget = 0.34404588f;
            spark.alphaBase = 160.0f;
            spark.alphaRange = 95.0f;
            spark.alphaTarget = 207.5f;
            sparkEffect.pos = gZeroVec3f;
            sparkEffect.rot = gZeroVec3f;
            sparkEffect.scale.x = 1.0f;
            sparkEffect.scale.y = 1.0f;
            sparkEffect.scale.z = 1.0f;
            sparkEffect.flags = 0x40EC0000;
            sparkEffect.unk44 = 0;
            sparkEffect.unk45 = 0xFF;
            sparkEffect.unk46 = 0;
            sparkEffect.unk47 = 7;
            sparkEffect.unk48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                data->sparks[spark.index] = particle_newQuadParticle(&sparkEffect, &gWhiteVtxColors, 0, 0x25, 0, 0x1D,
                    ((math_random() & 1) ? 2 : 0) + 1, 0, 0, sizeof(spark), owner, list);
                if (data->sparks[spark.index] != NULL) {
                    memcpy(&data->sparks[spark.index]->data, &spark, sizeof(spark));
                }
            }
        }
        {
            GlowShape shape;
            ParticleDef glowEffect;
            BurstGlowData glow;

            glow.textureTimer = 0;
            glow.targetScale = 1.0f;
            glow.owner = actor;
            glow.burst = effect;
            glow.ownerId = actor->unique_id;
            shape.width1 = 50.3f;
            shape.height1 = 50.3f;
            shape.width0 = 30.9f;
            shape.height0 = 30.9f;
            shape.offset1 = 0.1f;
            shape.offset0 = 0.1f;
            shape.alpha0 = 0.7f;
            shape.fadeRate = 0.000001f;
            shape.screenX[0] = -1;
            shape.screenY[0] = -1;
            shape.screenX[1] = -1;
            shape.screenY[1] = -1;
            shape.screenX[2] = -1;
            shape.screenY[2] = -1;
            shape.screenX[3] = -1;
            shape.screenY[3] = -1;
            shape.alpha1 = 1.0f;
            shape.attach = 0;
            shape.scale = 1.0f;
            shape.zbufSample[0] = 0;
            shape.zbufSample[1] = 0;
            shape.zbufSample[2] = 0;
            shape.zbufSample[3] = 0;
            shape.visibility[0] = 0;
            shape.visibility[1] = 0;
            shape.visibility[2] = 0;
            shape.visibility[3] = 0;
            shape.type = 0xF;
            shape.handler = -1;
            glowEffect.texture = 0x60;
            glowEffect.type = 3;
            glowEffect.renderModes = 0x2203;
            glowEffect.lifetime = 300;
            glowEffect.frame = 0;
            glowEffect.frameSpeed = 0;
            glowEffect.rgba[0] = 0xFF;
            glowEffect.rgba[1] = 0xFF;
            glowEffect.rgba[2] = 0xFF;
            glowEffect.rgba[3] = 0xFF;
            glowEffect.width = 100.0f;
            glowEffect.height = 100.0f;
            glowEffect.pos = gZeroVec3f;
            glowEffect.rot = gZeroVec3f;
            glowEffect.scale.x = 1.0f;
            glowEffect.scale.y = 1.0f;
            glowEffect.scale.z = 1.0f;
            glowEffect.flags = 0xCD2002;
            glowEffect.unk44 = 0xFF;
            glowEffect.unk45 = 0xFF;
            glowEffect.unk46 = 0;
            glowEffect.unk47 = 7;
            glowEffect.unk48 = 0;
            glowEffect.viewportMask = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                data->glows[glow.index] = flare_spawnGlow(&shape, 0x78, &glowEffect, 0, 0x2A, 0, 0, -1, owner, list);
                if (data->glows[glow.index] != NULL) {
                    memcpy(&data->glows[glow.index]->data, &glow, sizeof(glow));
                }
            }
        }
    }
}

/* The start (gBoneBurstSpark0Start, gBoneBurstSpark1Start) and end points (gBoneBurstSpark0End, gBoneBurstSpark1End) of the burst's two sparks, in the
 * space of bone 1 of the owner. */
static const Vec3f gBoneBurstSpark0Start = { 37.0f, -58.0f, -241.0f };
static const Vec3f gBoneBurstSpark1Start = { -37.0f, -58.0f, -241.0f };
static const Vec3f gBoneBurstSpark0End = { 45.0f, -58.0f, -300.0f };
static const Vec3f gBoneBurstSpark1End = { -45.0f, -58.0f, -300.0f };

/* Per-frame update of the burst: keeps the sparks and glows on bone 1 of the owner. */
void effect_updateBoneBurst(BurstEffect *effect) {
    BurstData *data;
    Actor *owner;
    Vec3f starts[2];
    Vec3f ends[2];
    u8 i;
    u8 j;
    u8 k;
    const Vec3f *srcs[4];
    Vec3f *dsts[4];
    s32 player;

    data = &effect->data;
    owner = data->owner;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL && (owner->cullMask & 0xF) != 0xF) {
        BurstSpark *spark;
        BurstGlow *glow;
        s32 pad1; // matching: stack slots of the original frame
        s32 pad2;

        srcs[0] = &gBoneBurstSpark0Start;
        srcs[1] = &gBoneBurstSpark1Start;
        srcs[2] = &gBoneBurstSpark0End;
        srcs[3] = &gBoneBurstSpark1End;
        dsts[0] = &starts[0];
        dsts[1] = &starts[1];
        dsts[2] = &ends[0];
        dsts[3] = &ends[1];
        math_transformOffsets(srcs, dsts, owner->ob_matrix_ptr + 1, 4);
        for (i = 0; i < 2; i++) {
            if (data->sparks[i] != NULL) {
                spark = data->sparks[i];
                spark->data.flags |= 1;
                spark->head.def.pos = starts[i];
                spark->head.def.rot = ends[i];
            }
            if (data->glows[i] != NULL) {
                glow = data->glows[i];
                glow->head.def.flags |= 2;
                glow->head.def.flags &= ~4;
                glow->head.def.pos = starts[i];
                glow->head.def.rot = ends[i];
            }
        }
    } else {
        for (j = 0; j < 2; j++) {
            if (data->sparks[j] != NULL) {
                BurstSparkData *sparkData = &data->sparks[j]->data;

                sparkData->flags &= ~1;
            }
            if (data->glows[j] != NULL) {
                data->glows[j]->head.def.flags &= ~2;
            }
        }
    }
    player = owner - movement_obj;
    if (player < 4) {
        for (k = 0; k < 2; k++) {
            if (data->sparks[k] != NULL) {
                BurstSparkData *sparkData = &data->sparks[k]->data;

                if (gPadTable[(u8)player]->button & A_BUTTON) {
                    sparkData->beamWidthTarget = 6.0f;
                    sparkData->beamLengthTarget = 21.0f;
                } else {
                    sparkData->beamWidthTarget = 3.0f;
                    sparkData->beamLengthTarget = 8.0f;
                }
            }
            if (data->glows[k] != NULL) {
                BurstGlowData *glowData = &data->glows[k]->data;

                if (gPadTable[(u8)player]->button & A_BUTTON) {
                    glowData->targetScale = 1.5f;
                } else {
                    glowData->targetScale = 1.0f;
                }
            }
        }
    }
}

/* Message handler of the burst: message 0x3B (effect_sendBurstOwnerGone) for its owner frees it; the rest go to effect_handleOwnerActorEvent. */
void effect_boneBurstMessage(void *effect, s32 msg, u8 msgId) {
    // matching: the original reads its parameters through this pointer; typed parameters
    // with the same body let IDO merge the two loads of msg
    void **args = &effect;
    BurstData *data;

    if (msgId == 0x3B) {
        data = &((BurstEffect *)args[0])->data;
        if (((ActorRef *)msg)->actor == data->owner || data->ownerId == ((ActorRef *)args[1])->uniqueId) {
            effect_destroyObject(args[0]);
        }
    } else {
        data = &((BurstEffect *)args[0])->data;
        effect_handleOwnerActorEvent(msg, msgId, &data->owner, &data->ownerId, args[0]);
    }
}

/* Frees the burst's sparks and glows. */
void effect_freeBurstParts(BurstEffect *effect) {
    BurstData *data = &effect->data;

    if (data->sparks[0] != NULL) {
        effect_destroyObject(&data->sparks[0]->head.header);
    }
    if (data->sparks[1] != NULL) {
        effect_destroyObject(&data->sparks[1]->head.header);
    }
    if (data->glows[0] != NULL) {
        effect_destroyObject(&data->glows[0]->head.header);
    }
    if (data->glows[1] != NULL) {
        effect_destroyObject(&data->glows[1]->head.header);
    }
}

void effect_destroyBoneBurst(BurstEffect *effect) {
    effect_freeBurstParts(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}

void effect_freeBoneBurst(BurstEffect *effect) {
    effect_freeBurstParts(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/*
 * Callbacks of the burst effect's sparks and glows (spawned by effect_spawnBoneBurst above), and the
 * message handler of a particle that follows an owner actor.
 */

/* Per-frame update of a burst spark: flickers its width, height and alpha towards random targets. */
s32 effect_updateBurstSpark(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    data->widthTimer -= gFrameDelta;
    if (data->widthTimer < 0.0f) {
        data->widthTimer = math_randomFloat() * 4.0f;
        data->widthTarget = math_randomFloat() * data->widthRange + data->widthBase;
    }
    spark->head.def.width += (data->widthTarget - spark->head.def.width) * 0.35f;
    data->heightTimer -= gFrameDelta;
    if (data->heightTimer < 0.0f) {
        data->heightTimer = math_randomFloat() * 9.0f;
        if (math_random() & 1) {
            data->heightTarget = math_randomFloat() * data->heightRange + data->heightBase;
        } else {
            data->heightTarget = math_randomFloat() * data->heightRange2 + data->heightBase;
        }
    }
    spark->head.def.height += (data->heightTarget - spark->head.def.height) * 0.4f;
    data->alphaTimer -= gFrameDelta;
    if (data->alphaTimer < 0.0f) {
        data->alphaTimer = math_randomFloat() * 10.0f;
        data->alphaTarget = math_randomFloat() * data->alphaRange + data->alphaBase;
    }
    data->alpha += (data->alphaTarget - data->alpha) * 0.4f;
    spark->head.def.unk44 = data->alpha;
    data->beamWidth += (data->beamWidthTarget - data->beamWidth) * 0.65000004f;
    data->beamLength += (data->beamLengthTarget - data->beamLength) * 0.65000004f;
    return 1;
}

/* Builds a burst spark's beam vertices; frees the spark once its owner is gone, and draws nothing while it is hidden. */
Vtx *effect_buildBurstSparkVtx(BurstSpark *spark, s16 viewport) {
    BurstSparkData *data = &spark->data;
    Actor *owner = data->owner;
    BeamScale scale;

    if (owner->unique_id != data->ownerId || owner->type == 0) {
        effect_destroyObject(&spark->head.header);
        return NULL;
    }
    if (!(data->flags & 1)) {
        return NULL;
    }
    scale.width = data->beamWidth * data->scale;
    scale.length = data->beamLength * data->scale;
    return particle_buildBeamQuad(&spark->head, FALSE, &scale, viewport);
}

/* A particle_newSpriteParticleWithFloor particle (game/Particle/SpriteParticle/spriteparticle.c's Particle) whose caller-sized data (at 0xA8) holds its owner at +8. */
struct OwnedParticle {
    EffectHeader header;
    u8 pad10[0xA0];
    ActorRef owner;
};

/* Message handler: 0x2D makes the particle follow its owner through a swap; message 0 for its owner frees it. */
void effect_ownedParticleMessage(OwnedParticle *particle, ActorMsg *msg, u8 msgId) {
    ActorRef *owner = &particle->owner;

    if (msgId == 0x2D) {
        Actor *actorA;

        if (owner->actor == (actorA = msg->swap.actorA)) {
            owner->actor = msg->swap.actorB;
            owner->uniqueId = msg->swap.idB;
        } else if (owner->actor == msg->swap.actorB) {
            owner->actor = actorA;
            owner->uniqueId = msg->swap.idA;
        }
    } else if (msgId == 0) {
        Actor *actor = msg->ref.actor;

        if (actor == owner->actor || msg->ref.uniqueId == owner->uniqueId) {
            effect_destroyObject(&particle->header);
        }
    }
}

/* Message handler of a burst spark: passes the message on with the spark's owner reference. */
void effect_burstSparkMessage(BurstSpark *spark, s32 msg, u8 msgId) {
    effect_handleOwnerActorEvent(msg, msgId, &spark->data.owner, &spark->data.ownerId, spark);
}

/* Clears the spark's slot in its burst effect. */
void effect_unlinkBurstSpark(BurstSpark *spark) {
    BurstSparkData *data = &spark->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->sparks[data->index] = NULL;
    }
}

/* Teardown of a burst spark: clears its slot in the burst, then runs the particle teardown particle_destroyQuadParticleDefault. */
void effect_destroyBurstSpark(BurstSpark *spark) {
    effect_unlinkBurstSpark(spark);
    particle_destroyQuadParticleDefault(&spark->head);
}

/* Teardown of a burst spark: clears its slot in the burst, then runs the particle teardown particle_freeQuadParticleDefault. */
void effect_freeBurstSpark(BurstSpark *spark) {
    effect_unlinkBurstSpark(spark);
    particle_freeQuadParticleDefault(&spark->head);
}

/* Per-frame update of a burst glow: switches it to a random texture every 3-10 frames and eases its scale. */
s32 effect_updateBurstGlow(BurstGlow *glow) {
    s32 pad; // matching: unused stack slot above textures
    const s32 textures[4] = { 0x60, 0x61, 0x62, 0x63 };
    GlowShape *shape = &glow->shape;

    glow->data.textureTimer -= gFrameTicks;
    if (glow->data.textureTimer < 0) {
        glow->head.def.texture = textures[math_random() & 3];
        glow->data.textureTimer = (math_random() & 7) + 3;
    }
    shape->scale += (glow->data.targetScale - shape->scale) * 0.65000004f;
    return 1;
}

/* Message handler of a burst glow: passes the message on with the glow's owner reference. */
void effect_burstGlowMessage(BurstGlow *glow, s32 msg, u8 msgId) {
    effect_handleOwnerActorEvent(msg, msgId, &glow->data.owner, &glow->data.ownerId, glow);
}

/* Clears the glow's slot in its burst effect. */
void effect_unlinkBurstGlow(BurstGlow *glow) {
    BurstGlowData *data = &glow->data;

    if (data->burst != NULL) {
        BurstData *burst = &data->burst->data;

        burst->glows[data->index] = NULL;
    }
}

/* Teardown of a burst glow: clears its slot in the burst, then runs the particle teardown flare_destroyGlowDefault. */
void effect_destroyBurstGlow(BurstGlow *glow) {
    effect_unlinkBurstGlow(glow);
    flare_destroyGlowDefault(&glow->head);
}

/* Teardown of a burst glow: clears its slot in the burst, then runs the particle teardown flare_freeGlowDefault. */
void effect_freeBurstGlow(BurstGlow *glow) {
    effect_unlinkBurstGlow(glow);
    flare_freeGlowDefault(&glow->head);
}
