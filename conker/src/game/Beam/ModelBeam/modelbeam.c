#include <ultra64.h>
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/texture.h"

/* Points in bone space: the ends of the beam_updateBoneSparks spark segment (bone 5), of the beam_updateModelBeam beam,
 * and the origin of the beam_updateFlashBurst bolts. */
static const Vec3f gBoneSparksStart = { 0.0f, 0.0f, 14.0f };
static const Vec3f gBoneSparksEnd = { 0.0f, 0.0f, 78.0f };
static const Vec3f gModelBeamStart = { 0.0f, -55.0f, 0.0f };
static const Vec3f gModelBeamEnd = { 0.0f, -115.0f, 0.0f };
static const Vec3f gFlashBurstOffset = { 0.0f, 465.0f, -102.0f };


/* Shows a sprite (picked by `style`) that fades in over `fadeIn` frames, holds for
 * `hold` frames and fades out over `fadeOut` frames. */
void beam_showFadeSprite(s32 fadeIn, s32 hold, s32 fadeOut, s32 style) {
    Sprite2DParams desc;
    s32 total;

    if (style != 0) {
        if (style == 2) {
            desc.width = 240;
            desc.height = 186;
            desc.texture = &D_80091948;
        } else {
            desc.width = 270;
            desc.height = 124;
            if (style == 1) {
                desc.texture = &D_80091930;
            } else {
                desc.texture = &D_8009193C;
            }
        }
        desc.behavior = 9;
        desc.flags = 0x10;
        desc.scaleX = 0;
        desc.scaleY = 0;
        desc.renderMode = 8;
    } else {
        desc.texture = &D_800917F8;
        desc.behavior = 10;
        desc.width = 160;
        desc.height = 192;
        desc.flags = 0x20;
        desc.scaleX = 0x1000;
        desc.scaleY = 0x1000;
        desc.renderMode = 0x18;
    }

    total = fadeIn + hold + fadeOut;
    desc.data.fade.fadeInHold = (fadeIn << 16) | hold;
    desc.data.fade.fadeOutTotal = (fadeOut << 16) | total;
    desc.data.fade.unk10 = 10000;
    desc.timer = total;
    desc.x = 146.0f;
    desc.y = 100.0f;
    desc.frameSpeed = 0;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.frameCount = 1;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.unk37 = 0x11;
    desc.alpha = 0;
    desc.frame = 0;
    sprite_newSprite2DDefault(&desc);
}

/* Grows the sprite as its timer runs down, then updates its fade. */
void beam_updateGrowSprite(Sprite2D *sprite) {
    s32 scale = 0x1000 - (sprite->params.timer << 2);

    sprite->params.scaleX = scale;
    sprite->params.scaleY = sprite->params.scaleX;
    beam_updateSpriteFade(sprite);
}

/* Sets the sprite's alpha from its timer: fade in, hold at 0xFF, then fade out. */
void beam_updateSpriteFade(Sprite2D *sprite) {
    s32 timer;
    s32 total;
    s32 fadeIn;
    s32 hold;
    s32 fadeInEnd;
    s32 fadeOut;

    total = sprite->params.data.fade.fadeOutTotal & 0xFFFF;
    fadeIn = sprite->params.data.fade.fadeInHold >> 16;
    hold = sprite->params.data.fade.fadeInHold & 0xFFFF;
    fadeOut = sprite->params.data.fade.fadeOutTotal >> 16;
    fadeInEnd = total - fadeIn;
    timer = sprite->params.timer;

    if (fadeInEnd < timer) {
        sprite->params.alpha = (total - timer) * 0xFF / fadeIn;
    } else if (fadeInEnd - hold < timer) {
        sprite->params.alpha = 0xFF;
    } else {
        sprite->params.alpha = timer * 0xFF / fadeOut;
    }
}

void beam_levelUpdateRain(s32 arg0) {
    effect_updateRain(0);
}


/* Model attachment hook 4 (gAttachHooks[4]). Picks the attachment's segment-6 texture (an entry of
 * D_80090274) from the actor's animation frame while the actor is in state 0xAE. */
s32 beam_hookFrameTexture(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim;
    s32 frame;
    f32 two; // matching: IDO materializes 2.0f once only when it is a variable
    f32 t;

    anim = actor->anim;
    if (anim == NULL) {
        return 0;
    }
    frame = 6;
    two = 2.0f;
    if (actor->animseq.uh == 0xAE) {
        if ((anim->frame[0] >= 36.0f) && (anim->frame[0] <= 51.0f)) {
            t = anim->frame[0] - 36.0f;
            t *= 0.0625f;
            t = 1.0f - t;
            frame = 6.0f * t;
        } else if ((anim->frame[0] > 51.0f) && (anim->frame[0] <= 54.0f)) {
            frame = 0;
        } else if ((anim->frame[0] > 54.0f) && (anim->frame[0] <= 60.0f)) {
            t = anim->frame[0] - 54.0f;
            t *= 0.1428571492f;
            frame = 6.0f * t;
        } else if ((anim->frame[0] > 60.0f) && (anim->frame[0] <= 65.0f)) {
            t = anim->frame[0] - 60.0f;
            t *= 0.1666666716f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame[0] > 65.0f) && (anim->frame[0] <= 67.0f)) {
            t = anim->frame[0] - 65.0f;
            t *= 0.3333333433f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame[0] > 67.0f) && (anim->frame[0] <= 70.0f)) {
            t = anim->frame[0] - 67.0f;
            t *= 0.25f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 4;
        } else if ((anim->frame[0] > 70.0f) && (anim->frame[0] <= 74.0f)) {
            t = anim->frame[0] - 70.0f;
            t *= 0.200000003f;
            frame = (s32)(two * t) + 4;
        } else if ((anim->frame[0] > 74.0f) && (anim->frame[0] <= 119.0f)) {
            t = anim->frame[0] - 74.0f;
            t *= 0.02173913084f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame[0] > 119.0f) && (anim->frame[0] <= 150.0f)) {
            t = anim->frame[0] - 119.0f;
            t *= 0.03125f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        }
    }
    att->seg6Id = D_80090274[frame];
    return 0;
}

/* Model attachment hook 21 (gAttachHooks[21]). While the actor is in state 5, slides the attachment's
 * texture over animation frames 69-84 by patching the upper-left t of the first G_SETTILESIZE in its
 * display list. unk38 is the fade state: 0 = start, 1 = shown, 2 = faded out. */
s32 beam_hookSlideTexture(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim;
    Gfx *gfx;
    s32 count;
    s32 notStarted;
    f32 intensity;
    s32 ult;
    s32 i;

    anim = actor->anim;
    if (anim == NULL) {
        return 0;
    }

    gfx = *att->dlists;
    count = 1;
    notStarted = FALSE;
    switch (att->hookState) {
    case 0:
        if (actor->animseq.uh == 5) {
            intensity = 1.0f;
            att->hookState = 1;
        } else {
            notStarted = TRUE;
        }
        break;
    case 1:
        intensity = 1.0f;
        break;
    case 2:
        intensity = 0.0f;
        break;
    }

    if (actor->animseq.uh == 5) {
        if (anim->frame[0] < 69.0f) {
            intensity = 1.0f;
        } else if (84.0f < anim->frame[0]) {
            intensity = 0.0f;
            att->hookState = 2;
        } else {
            intensity = (anim->frame[0] - 69.0f) / 15;
            intensity = 1.0f - intensity;
        }
    }

    /* Find the count-th G_SETTILESIZE. */
    i = 0;
    do {
        count--;
        while (*(s8 *)&gfx[i] != (s8)G_SETTILESIZE) {
            i++;
        }
        if (count != 0) {
            i++;
        }
    } while (count != 0);

    if (notStarted) {
        ult = 2;
    } else {
        ult = (58.0f * intensity) + 44.0f;
    }
    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | (ult & 0xFFF);
    return 0;
}

/* Spawns a spark effect (beam_updateBoneSparks) on `owner`. */
void beam_spawnBoneSparks(Actor *owner, s16 arg1, u8 arg2, s32 arg3) {
    SparkData data;
    BoneSparkEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.accum = 0.0f;

    effect = effect_newTimedEffect(arg1, -1, 0x51, -1, 1, 0x3E, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Spark effect: while the owner lives, emits sparks along the segment between two points of
 * its bone 5, at a random rate. */
void beam_updateBoneSparks(BoneSparkEffect *effect) {
    Actor *owner;
    SparkData *data;
    ParticleDesc spawn;
    f32 scale;
    Vec3f start;
    Vec3f end;
    f32 dx;
    f32 dy;
    f32 dz;
    const Vec3f *srcs[2];
    Vec3f *dsts[2];

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL) {
        data->accum += (0.2600000203f + math_randomFloat() * 0.1950000077f) * gFrameDelta;
        if (data->accum > 1.0f) {
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.r2 = 0xB4;
            spawn.g2 = 0xB4;
            spawn.b2 = 0xB4;
            srcs[0] = &gBoneSparksStart;
            srcs[1] = &gBoneSparksEnd;
            dsts[0] = &start;
            dsts[1] = &end;
            math_transformOffsets(srcs, dsts, &owner->ob_matrix_ptr[5], 2);
            spawn.texture = 0x6C;
            spawn.renderMode = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x9F0600;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.a = 0xFF;
            spawn.brightness = 0xFF;
            dx = end.x - start.x;
            dy = end.y - start.y;
            dz = end.z - start.z;
            spawn.offset = gZeroVec3f;
            spawn.flags = 0x84CE07;
            spawn.primColorMode = 8;
            spawn.envColorMode = 6;
            spawn.updateFunc = 0x10;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.minSize = 1000;
            spawn.maxSize = 1000.0f;
            scale = 0.9730340242f;
            spawn.scaleRate = 1.022472024f;
            do {
                f32 speed;
                f32 t;
                s32 flipX;
                SpriteParticle *spark;

                spawn.timer = math_random() % 21U + 20;
                spawn.scaleStart = spawn.timer;
                spawn.fadeStart = spawn.timer;
                spawn.fadeRate = 0xFF / spawn.timer;
                spawn.alpha = math_random() % 26U + 8;
                spawn.width = spawn.height = math_randomFloat() * 70.0f + 148.0f;
                spawn.gravity = math_randomFloat() * 0.2190000117f + 0.08600000292f;
                speed = math_randomFloat() * 0.04100000113f + 0.07800000161f;
                spawn.vel.x = dx * speed;
                spawn.vel.y = dy * speed;
                spawn.vel.z = dz * speed;
                /* Start a random part of a frame along the path. */
                t = math_randomFloat() * gFrameDelta;
                spawn.flags &= ~0xC0;
                spawn.pos.x = spawn.vel.x * t + start.x;
                spawn.pos.y = spawn.vel.y * t + start.y;
                spawn.pos.z = spawn.vel.z * t + start.z;
                flipX = (math_random() & 1) ? 0x80 : 0;
                spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | flipX;
                spark = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 4, effect->base.header.owner, effect->base.header.kind);
                if (spark != NULL) {
                    memcpy(&spark->data.drag, &scale, sizeof(scale));
                }
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        }
    }
}

/* Effect callback for the spark effect: hands effect_handleTimedEffectOwnerEvent the effect's owner reference
 * (pointer and id). */
void beam_boneSparksMessage(BoneSparkEffect *effect, s32 arg1, u8 arg2) {
    effect_handleTimedEffectOwnerEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Beam effect: while the owner lives and has a model (and hiddenParts bit 1 is clear), stretches the
 * beam between two points of the model and puts the flash at the first; otherwise hides both. */
void beam_updateModelBeam(ModelBeamEffect *effect) {
    ModelBeamData *data;
    Actor *owner;
    Vec3f start;
    Vec3f end;
    const Vec3f *srcs[2];
    Vec3f *dsts[2];

    data = &effect->data;
    owner = data->owner;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL && !(owner->hiddenParts & 2)) {
        ModelBeam *beam;
        LightEffect *flash;
        PointLight *light;

        srcs[0] = &gModelBeamStart;
        srcs[1] = &gModelBeamEnd;
        dsts[0] = &start;
        dsts[1] = &end;
        math_transformOffsets(srcs, dsts, owner->ob_matrix_ptr, 2);
        if (data->beam != NULL) {
            beam = data->beam;
            beam->part.flags |= 1;
            beam->head.def.pos = start;
            beam->head.def.rot = end;
        }
        flash = data->flash;
        if (flash != NULL) {
            light = flash->light;
            light->disabled = FALSE;
            light->x = start.x;
            light->y = start.y;
            light->z = start.z;
        }
    } else {
        if (data->beam != NULL) {
            BeamPart *part = &data->beam->part;

            part->flags &= ~1;
        }
        if (data->flash != NULL) {
            PointLight *light = data->flash->light;

            light->disabled = TRUE;
        }
    }
    if (data->beam != NULL) {
        BeamPart *part = &data->beam->part;

        part->unk48 = 4.0f;
        part->unk4C = 8.0f;
    }
}

/* Frees the beam effect's beam and flash. */
void beam_releaseModelBeamParts(ModelBeamEffect *effect) {
    ModelBeamData *data = &effect->data;

    if (data->beam != NULL) {
        effect_destroyObject(&data->beam->head.header);
    }
    if (data->flash != NULL) {
        effect_destroyObject(data->flash);
    }
}

/* Beam effect destructors: free the beam and flash, then the effect. */
void beam_destroyModelBeamEffect(ModelBeamEffect *effect) {
    beam_releaseModelBeamParts(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}

void beam_freeModelBeamEffect(ModelBeamEffect *effect) {
    beam_releaseModelBeamParts(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/* Effect callback for the beam effect: hands effect_handleOwnerActorEvent the owner reference. */
void beam_modelBeamMessage(ModelBeamEffect *effect, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Clears the beam effect's reference to this beam. */
void beam_unlinkModelBeam(ModelBeam *beam) {
    BeamPart *part = &beam->part;

    if (part->effect != NULL) {
        ModelBeamData *data = &part->effect->data;

        data->beam = NULL;
    }
}

/* Beam destructors: unlink the beam from its effect, then free it. */
void beam_destroyModelBeam(ModelBeam *beam) {
    beam_unlinkModelBeam(beam);
    particle_destroyQuadParticleDefault(&beam->head);
}

void beam_freeModelBeam(ModelBeam *beam) {
    beam_unlinkModelBeam(beam);
    particle_freeQuadParticleDefault(&beam->head);
}

void func_150F6478(DefParticleHead *obj) {
}

void func_150F6484(DefParticleHead *obj) {
    func_150F6478(obj);
    flare_destroyGlowDefault(obj);
}

void func_150F64B0(DefParticleHead *obj) {
    func_150F6478(obj);
    flare_freeGlowDefault(obj);
}

/* Burst effect: while the owner lives and has a model (and hiddenParts bit 1 is clear), every 90-180
 * frames calls sound_playAt(0x679) at the owner, makes a flash at a point of the model and emits
 * 2 or 3 blue-white particles from it. */
void beam_updateFlashBurst(FlashBurstEffect *effect) {
    FlashBurstData *data;
    Actor *owner;
    s8 count;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if ((owner->ob_matrix_ptr != NULL) && !(owner->hiddenParts & 2)) {
        data->timer -= gFrameTicks;
        if (data->timer < 0) {
            sound_playAt(0x679, 0x18CE, 0, 0, 0, owner->xpos, owner->ypos, owner->zpos,
                          0x7918, 0x7D00);
            count = (math_random() & 1) + 2;
            {
                Vec3f pos;
                LightEffectHeader header;
                Vec3i origin;
                u8 color[4];
                s32 rand1;
                s32 rand2;
                s32 rand3;

                math_transformOffset(&gFlashBurstOffset, &pos, owner->ob_matrix_ptr);
                header.flags = 3;
                header.update = -1;
                header.lifetime = (math_random() % 9U) + 10;
                header.lightType = 0;
                origin.x = pos.x;
                origin.y = pos.y;
                origin.z = pos.z;
                rand3 = (math_random() % 61U) + 60;
                light_createWithEffect(&header, &origin, rand3, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, effect->base.header.owner,
                              effect->base.header.kind);
                do {
                    color[0] = 0xA0;
                    color[1] = 0xA0;
                    color[2] = 0xFF;
                    color[3] = (math_random() % 101U) + 155;
                    rand1 = math_random();
                    rand2 = math_random();
                    rand3 = math_random();
                    beam_spawnBoneBolt(owner, 0, &gFlashBurstOffset, (u8)rand1, (rand2 % 43U) - 50, (rand3 % 18U) + 5, 4,
                                  27.0f, (math_randomFloat() * 16.0f) + 20.0f, 0, 2, color, effect->base.header.owner,
                                  effect->base.header.kind);
                    count--;
                } while (count > 0);
                data->timer = (math_random() % 91U) + 90;
            }
        }
    }
}

/* Effect callback for the burst effect: hands effect_handleOwnerActorEvent the owner reference. */
void beam_flashBurstMessage(FlashBurstEffect *effect, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Creates a blood_spawnStrip object attached to `owner`. */
void beam_spawnBoneBloodStrip(Actor *owner, s16 arg1, u8 arg2, s32 arg3) {
    BloodStripDesc desc;

    desc.ownerId = owner->unique_id;
    desc.offsetB.y = 4.700000286f;
    desc.offsetB.z = 8.5f;
    desc.rate = 1.0f;
    desc.owner = owner;
    desc.bone = 3;
    desc.offsetA.x = 0.0f;
    desc.offsetA.y = 0.0f;
    desc.offsetA.z = 0.0f;
    desc.offsetB.x = 0.0f;
    desc.flags = 2;
    desc.duration = 40;
    desc.pause = 16;
    desc.lifetime = arg1;
    desc.sampleCallback = 5;
    desc.emitCallback = 8;
    desc.rateCallback = -1;
    desc.mode = 0;
    desc.updateCallback = -1;
    desc.sound = 0;

    blood_spawnStrip(&desc, 0);
}

/* matching: compiled with s32 where steam.c's definition has u8 and s16 (args 3, 5, 6, 11, 15), and with a void return */
void particle_spawnSteamParticle(Vec3f *, Vec3f *, s32, f32, s32, s32, f32, s32, f32, f32, s32, CollisionHit *, s32, s32, s32, s32);
/* BloodStrip emit callback: one particle_spawnSteamParticle blood particle, larger than blood_emitStripParticle's. 60% of
 * them get a ground probe under the strip's owner; the rest get none (height -10000). */
void beam_emitBloodParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
    Vec3f start;
    Vec3f end;
    Vec3f velocity;
    f32 arg3;
    s16 arg4;
    u8 arg5;
    f32 arg6;
    CollisionHit ground;
    u8 probeGround;
    s32 flag;
    f32 random;

    blood_getStripParticleParams(pointA, pointB, velocityA, velocityB, time, strip, &start, &end, &velocity, &arg3,
                  &arg4, &arg5, &arg6);
    random = math_randomFloat();
    arg6 *= 1.0f + (random + random);
    probeGround = math_randomFloat() < 0.6f;
    if (probeGround) {
        collision_initHitFromObject(&ground, strip->owner);
    } else {
        ground.material = 0;
        ground.flags = 0;
        ground.source = 0;
        ground.surface = 0;
        ground.height = -10000.0f;
    }
    if (probeGround) {
        flag = math_randomFloat() < 0.5f;
    } else {
        flag = 0;
    }
    particle_spawnSteamParticle(&start, &velocity, 1, arg3, arg4, arg5, arg6, probeGround, 1.8f, 1.8f, 1,
                  &ground, 1, flag, strip->header.owner, strip->header.kind);
}
