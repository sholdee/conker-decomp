#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_80091930;
extern u8 D_8009193C;
extern u8 D_80091948;

/* Shows a sprite (picked by `style`) that fades in over `fadeIn` frames, holds for
 * `hold` frames and fades out over `fadeOut` frames. */
void func_150F5420(s32 fadeIn, s32 hold, s32 fadeOut, s32 style) {
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
    func_15169968(&desc);
}

/* Grows the sprite as its timer runs down, then updates its fade. */
void func_150F5590(Sprite2D *sprite) {
    s32 scale = 0x1000 - (sprite->params.timer << 2);

    sprite->params.scaleX = scale;
    sprite->params.scaleY = sprite->params.scaleX;
    func_150F55C8(sprite);
}

/* Sets the sprite's alpha from its timer: fade in, hold at 0xFF, then fade out. */
void func_150F55C8(Sprite2D *sprite) {
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

void func_150F568C(s32 arg0) {
    func_15179008(0);
}

extern s32 D_80090274[];

/* Model attachment hook 4 (D_800841C0[4]). Picks the attachment's segment-6 texture (an entry of
 * D_80090274) from the actor's animation frame while the actor is in state 0xAE. */
s32 func_150F56B0(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim;
    s32 frame;
    f32 two; // matching: IDO materializes 2.0f once only when it is a variable
    f32 t;

    anim = actor->unk2D0;
    if (anim == NULL) {
        return 0;
    }
    frame = 6;
    two = 2.0f;
    if (actor->animseq.uh == 0xAE) {
        if ((anim->frame >= 36.0f) && (anim->frame <= 51.0f)) {
            t = anim->frame - 36.0f;
            t *= 0.0625f;
            t = 1.0f - t;
            frame = (s32)(6.0f * t);
        } else if ((anim->frame > 51.0f) && (anim->frame <= 54.0f)) {
            frame = 0;
        } else if ((anim->frame > 54.0f) && (anim->frame <= 60.0f)) {
            t = anim->frame - 54.0f;
            t *= 0.1428571492f;
            frame = (s32)(6.0f * t);
        } else if ((anim->frame > 60.0f) && (anim->frame <= 65.0f)) {
            t = anim->frame - 60.0f;
            t *= 0.1666666716f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame > 65.0f) && (anim->frame <= 67.0f)) {
            t = anim->frame - 65.0f;
            t *= 0.3333333433f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame > 67.0f) && (anim->frame <= 70.0f)) {
            t = anim->frame - 67.0f;
            t *= 0.25f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 4;
        } else if ((anim->frame > 70.0f) && (anim->frame <= 74.0f)) {
            t = anim->frame - 70.0f;
            t *= 0.200000003f;
            frame = (s32)(two * t) + 4;
        } else if ((anim->frame > 74.0f) && (anim->frame <= 119.0f)) {
            t = anim->frame - 74.0f;
            t *= 0.02173913084f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->frame > 119.0f) && (anim->frame <= 150.0f)) {
            t = anim->frame - 119.0f;
            t *= 0.03125f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        }
    }
    att->seg6Id = D_80090274[frame];
    return 0;
}

/* Model attachment hook 21 (D_800841C0[21]). While the actor is in state 5, slides the attachment's
 * texture over animation frames 69-84 by patching the upper-left t of the first G_SETTILESIZE in its
 * display list. unk38 is the fade state: 0 = start, 1 = shown, 2 = faded out. */
s32 func_150F5A54(ModelAttachment *att, Actor *actor) {
    AnimInstance *anim;
    Gfx *gfx;
    s32 count;
    s32 notStarted;
    f32 intensity;
    s32 ult;
    s32 i;

    anim = actor->unk2D0;
    if (anim == NULL) {
        return 0;
    }

    gfx = *att->dlists;
    count = 1;
    notStarted = FALSE;
    switch (att->unk38) {
    case 0:
        if (actor->animseq.uh == 5) {
            intensity = 1.0f;
            att->unk38 = 1;
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
        if (anim->frame < 69.0f) {
            intensity = 1.0f;
        } else if (84.0f < anim->frame) {
            intensity = 0.0f;
            att->unk38 = 2;
        } else {
            intensity = (anim->frame - 69.0f) / 15;
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
        ult = (s32)((58.0f * intensity) + 44.0f);
    }
    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | (ult & 0xFFF);
    return 0;
}

/* Spawns a spark effect (func_150F5C98) on `owner`. */
void func_150F5C08(Actor *owner, s16 arg1, u8 arg2, s32 arg3) {
    SparkData data;
    TimedEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.accum = 0.0f;

    effect = func_15149130(arg1, -1, 0x51, -1, 1, 0x3E, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&((BoneSparkEffect *)effect)->data, &data, sizeof(data));
    }
}

extern Vec3f D_800A1B00;
extern Vec3f D_800A1B0C;

/* Spark effect: while the owner lives, emits sparks along the segment between two points of
 * its bone 5, at a random rate. */
void func_150F5C98(BoneSparkEffect *effect) {
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
        data->accum += (0.2600000203f + func_150ADA68() * 0.1950000077f) * D_800BE9A4;
        if (data->accum > 1.0f) {
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.r2 = 0xB4;
            spawn.g2 = 0xB4;
            spawn.b2 = 0xB4;
            srcs[0] = &D_800A1B00;
            srcs[1] = &D_800A1B0C;
            dsts[0] = &start;
            dsts[1] = &end;
            func_15145EA4(srcs, dsts, &owner->ob_matrix_ptr[5], 2);
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
            spawn.offset = D_800A5480;
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

                spawn.timer = func_150ADA20() % 21U + 20;
                spawn.scaleStart = spawn.timer;
                spawn.fadeStart = spawn.timer;
                spawn.fadeRate = 0xFF / spawn.timer;
                spawn.alpha = func_150ADA20() % 26U + 8;
                spawn.width = spawn.height = func_150ADA68() * 70.0f + 148.0f;
                spawn.gravity = func_150ADA68() * 0.2190000117f + 0.08600000292f;
                speed = func_150ADA68() * 0.04100000113f + 0.07800000161f;
                spawn.vel.x = dx * speed;
                spawn.vel.y = dy * speed;
                spawn.vel.z = dz * speed;
                /* Start a random part of a frame along the path. */
                t = func_150ADA68() * D_800BE9A4;
                spawn.flags &= ~0xC0;
                spawn.pos.x = spawn.vel.x * t + start.x;
                spawn.pos.y = spawn.vel.y * t + start.y;
                spawn.pos.z = spawn.vel.z * t + start.z;
                flipX = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flipX;
                spark = func_15130280(&spawn, 1, 0, 4, effect->base.header.owner, effect->base.header.kind);
                if (spark != NULL) {
                    memcpy(&spark->data.drag, &scale, sizeof(scale));
                }
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        }
    }
}

/* Effect callback for the spark effect: hands func_15149514 the effect's owner reference
 * (pointer and id). */
void func_150F6138(BoneSparkEffect *effect, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

extern Vec3f D_800A1B18;
extern Vec3f D_800A1B24;

/* Beam effect: while the owner lives and has a model (and unk94 bit 1 is clear), stretches the
 * beam between two points of the model and puts the flash at the first; otherwise hides both. */
void func_150F6178(ModelBeamEffect *effect) {
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
    if (owner->ob_matrix_ptr != NULL && !(owner->unk94 & 2)) {
        ModelBeam *beam;
        LightEffect *flash;
        PointLight *light;

        srcs[0] = &D_800A1B18;
        srcs[1] = &D_800A1B24;
        dsts[0] = &start;
        dsts[1] = &end;
        func_15145EA4(srcs, dsts, owner->ob_matrix_ptr, 2);
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
void func_150F631C(ModelBeamEffect *effect) {
    ModelBeamData *data = &effect->data;

    if (data->beam != NULL) {
        func_1516972C(&data->beam->head.header);
    }
    if (data->flash != NULL) {
        func_1516972C((EffectHeader *)data->flash);
    }
}

/* Beam effect destructors: free the beam and flash, then the effect. */
void func_150F6368(ModelBeamEffect *effect) {
    func_150F631C(effect);
    func_1514933C(&effect->base);
}

void func_150F6394(ModelBeamEffect *effect) {
    func_150F631C(effect);
    func_15149368(&effect->base);
}

/* Effect callback for the beam effect: hands func_15169850 the owner reference. */
void func_150F63C0(ModelBeamEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Clears the beam effect's reference to this beam. */
void func_150F6400(ModelBeam *beam) {
    BeamPart *part = &beam->part;

    if (part->effect != NULL) {
        ModelBeamData *data = &part->effect->data;

        data->beam = NULL;
    }
}

/* Beam destructors: unlink the beam from its effect, then free it. */
void func_150F6420(ModelBeam *beam) {
    func_150F6400(beam);
    func_1513CA6C(&beam->head);
}

void func_150F644C(ModelBeam *beam) {
    func_150F6400(beam);
    func_1513CAA0(&beam->head);
}

void func_150F6478(DefParticleHead *obj) {
}

void func_150F6484(DefParticleHead *obj) {
    func_150F6478(obj);
    func_151411A4(obj);
}

void func_150F64B0(DefParticleHead *obj) {
    func_150F6478(obj);
    func_151411C4(obj);
}

extern Vec3f D_800A1B30;

/* Burst effect: while the owner lives and has a model (and unk94 bit 1 is clear), every 90-180
 * frames calls func_10010F88(0x679) at the owner, makes a flash at a point of the model and emits
 * 2 or 3 blue-white particles from it. */
void func_150F64DC(FlashBurstEffect *effect) {
    FlashBurstData *data;
    Actor *owner;
    s8 count;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if ((owner->ob_matrix_ptr != NULL) && !(owner->unk94 & 2)) {
        data->timer -= D_800BE9E4;
        if (data->timer < 0) {
            func_10010F88(0x679, 0x18CE, 0, 0, 0, owner->xpos, owner->ypos, owner->zpos,
                          0x7918, 0x7D00);
            count = (func_150ADA20() & 1) + 2;
            {
                Vec3f pos;
                LightEffectHeader header;
                Vec3i origin;
                u8 color[4];
                s32 rand1;
                s32 rand2;
                s32 rand3;

                func_15143134(&D_800A1B30, &pos, owner->ob_matrix_ptr);
                header.flags = 3;
                header.update = -1;
                header.lifetime = (func_150ADA20() % 9U) + 10;
                header.lightType = 0;
                origin.x = pos.x;
                origin.y = pos.y;
                origin.z = pos.z;
                rand3 = (func_150ADA20() % 61U) + 60;
                func_151602C0(&header, &origin, rand3, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, effect->base.header.owner,
                              effect->base.header.kind);
                do {
                    color[0] = 0xA0;
                    color[1] = 0xA0;
                    color[2] = 0xFF;
                    color[3] = (func_150ADA20() % 101U) + 155;
                    rand1 = func_150ADA20();
                    rand2 = func_150ADA20();
                    rand3 = func_150ADA20();
                    func_15107C1C(owner, 0, &D_800A1B30, (u8)rand1, (rand2 % 43U) - 50, (rand3 % 18U) + 5, 4,
                                  27.0f, (func_150ADA68() * 16.0f) + 20.0f, 0, 2, color, effect->base.header.owner,
                                  effect->base.header.kind);
                    count--;
                } while (count > 0);
                data->timer = (func_150ADA20() % 91U) + 90;
            }
        }
    }
}

/* Effect callback for the burst effect: hands func_15169850 the owner reference. */
void func_150F6850(FlashBurstEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Creates a func_15134DAC object attached to `owner`. */
void func_150F6890(Actor *owner, s16 arg1, u8 arg2, s32 arg3) {
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

    func_15134DAC(&desc, 0);
}

// matching: this file was compiled with s32 where the definition (game_2062D0.c) has u8 (args 3, 6, 11, 15) and
// s16 (arg 5), and with a void return. The definition is SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel,
// u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7, f32 burstScale, f32 altBurstScale, u8 argA,
// CollisionHit *ground, s32 wobbles, s32 bounces, u8 argE, s32 argF).
void func_151D9014(Vec3f *, Vec3f *, s32, f32, s32, s32, f32, s32, f32, f32, s32, CollisionHit *, s32, s32, s32, s32);
/* BloodStrip emit callback: one func_151D9014 blood particle, larger than func_15137E60's. 60% of
 * them get a ground probe under the strip's owner; the rest get none (height -10000). */
void func_150F695C(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip) {
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

    func_15137F30(pointA, pointB, velocityA, velocityB, time, strip, &start, &end, &velocity, &arg3,
                  &arg4, &arg5, &arg6);
    random = func_150ADA68();
    arg6 *= 1.0f + (random + random);
    probeGround = func_150ADA68() < 0.6f;
    if (probeGround) {
        func_1504715C(&ground, strip->owner);
    } else {
        ground.material = 0;
        ground.flags = 0;
        ground.source = 0;
        ground.surface = 0;
        ground.height = -10000.0f;
    }
    if (probeGround) {
        flag = func_150ADA68() < 0.5f;
    } else {
        flag = 0;
    }
    func_151D9014(&start, &velocity, 1, arg3, arg4, arg5, arg6, probeGround, 1.8f, 1.8f, 1,
                  &ground, 1, flag, strip->header.owner, strip->header.kind);
}
