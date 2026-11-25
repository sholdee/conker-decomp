#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Sprite descriptor passed to func_15169968; the sprite it creates keeps these fields from +0x10. */
typedef struct {
    /* 0x00 */ void *texture;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 fadeInHold;    /* fade-in frames << 16 | hold frames */
    /* 0x0C */ s32 fadeOutTotal;  /* fade-out frames << 16 | total frames */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 timer;         /* frames left, counts down from the total */
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 scaleX;        /* 0x1000 = 1.0 */
    /* 0x2A */ s16 scaleY;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8  unk30;
    /* 0x31 */ u8  unk31;
    /* 0x32 */ u8  red;
    /* 0x33 */ u8  green;
    /* 0x34 */ u8  blue;
    /* 0x35 */ u8  alpha;
    /* 0x36 */ u8  unk36;
    /* 0x37 */ u8  unk37;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ u8  unk39;
} struct_7484;

extern u8 D_80091930;
extern u8 D_8009193C;
extern u8 D_80091948;
EffectHeader *func_15169968(struct_7484 *);

/* Shows a sprite (picked by `style`) that fades in over `fadeIn` frames, holds for
 * `hold` frames and fades out over `fadeOut` frames. */
void func_150F5420(s32 fadeIn, s32 hold, s32 fadeOut, s32 style) {
    struct_7484 desc;
    s32 total;

    if (style != 0) {
        if (style == 2) {
            desc.unk2C = 240;
            desc.unk2E = 186;
            desc.texture = &D_80091948;
        } else {
            desc.unk2C = 270;
            desc.unk2E = 124;
            if (style == 1) {
                desc.texture = &D_80091930;
            } else {
                desc.texture = &D_8009193C;
            }
        }
        desc.unk30 = 9;
        desc.unk39 = 0x10;
        desc.scaleX = 0;
        desc.scaleY = 0;
        desc.unk36 = 8;
    } else {
        desc.texture = &D_800917F8;
        desc.unk30 = 10;
        desc.unk2C = 160;
        desc.unk2E = 192;
        desc.unk39 = 0x20;
        desc.scaleX = 0x1000;
        desc.scaleY = 0x1000;
        desc.unk36 = 0x18;
    }

    total = fadeIn + hold + fadeOut;
    desc.fadeInHold = (fadeIn << 16) | hold;
    desc.fadeOutTotal = (fadeOut << 16) | total;
    desc.unk10 = 10000;
    desc.timer = total;
    desc.unk1C = 146.0f;
    desc.unk20 = 100.0f;
    desc.unk18 = 0;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.unk31 = 1;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.unk37 = 0x11;
    desc.alpha = 0;
    desc.unk16 = 0;
    func_15169968(&desc);
}

/* The sprite created from a struct_7484 (its fields from +0x10 are the descriptor's). */
struct Struct150F5590 {
    char pad0[0x18];
    s32 fadeInHold;   /* fade-in frames << 16 | hold frames */
    s32 fadeOutTotal; /* fade-out frames << 16 | total frames */
    char pad20[0x24 - 0x20];
    s16 timer;        /* frames left */
    char pad26[0x38 - 0x26];
    s16 scaleX;       /* 0x1000 = 1.0 */
    s16 scaleY;
    char pad3C[0x45 - 0x3C];
    u8 alpha;
};

void func_150F55C8(struct Struct150F5590 *);

/* Grows the sprite as its timer runs down, then updates its fade. */
void func_150F5590(struct Struct150F5590 *sprite) {
    s32 scale = 0x1000 - (sprite->timer << 2);

    sprite->scaleX = scale;
    sprite->scaleY = sprite->scaleX;
    func_150F55C8(sprite);
}

/* Sets the sprite's alpha from its timer: fade in, hold at 0xFF, then fade out. */
void func_150F55C8(struct Struct150F5590 *sprite) {
    s32 timer;
    s32 total;
    s32 fadeIn;
    s32 hold;
    s32 fadeInEnd;
    s32 fadeOut;

    total = sprite->fadeOutTotal & 0xFFFF;
    fadeIn = sprite->fadeInHold >> 16;
    hold = sprite->fadeInHold & 0xFFFF;
    fadeOut = sprite->fadeOutTotal >> 16;
    fadeInEnd = total - fadeIn;
    timer = sprite->timer;

    if (fadeInEnd < timer) {
        sprite->alpha = (total - timer) * 0xFF / fadeIn;
    } else if (fadeInEnd - hold < timer) {
        sprite->alpha = 0xFF;
    } else {
        sprite->alpha = timer * 0xFF / fadeOut;
    }
}

void func_15179008(s32);

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
    if (actor->unk84.uh == 0xAE) {
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
    f32 level;
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
        if (actor->unk84.uh == 5) {
            level = 1.0f;
            att->unk38 = 1;
        } else {
            notStarted = TRUE;
        }
        break;
    case 1:
        level = 1.0f;
        break;
    case 2:
        level = 0.0f;
        break;
    }

    if (actor->unk84.uh == 5) {
        if (anim->frame < 69.0f) {
            level = 1.0f;
        } else if (84.0f < anim->frame) {
            level = 0.0f;
            att->unk38 = 2;
        } else {
            level = (anim->frame - 69.0f) / 15;
            level = 1.0f - level;
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
        ult = (s32)((58.0f * level) + 44.0f);
    }
    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | (ult & 0xFFF);
    return 0;
}

/* Data of the spark effect spawned by func_150F5C08 and run by func_150F5C98. */
typedef struct {
    Actor *owner;
    u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    u8 pad5[3];
    f32 accum;  /* sparks owed; one is spawned per whole unit */
} SparkData;

typedef struct {
    EffectHeader head;
    u8 pad10[0x18];
    SparkData data;
} SparkEffect;

/* Spawns a spark effect (func_150F5C98) on `owner`. */
void func_150F5C08(Actor *owner, s16 arg1, u8 arg2, s32 arg3) {
    SparkData data;
    TimedEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.accum = 0.0f;

    // the header's struct37 * parameter is really the size of the effect's data
    effect = func_15149130(arg1, -1, 0x51, -1, 1, 0x3E, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&((SparkEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Partial view of the particle func_15130280 creates. */
typedef struct {
    DescParticleHead head;
    f32 unkA8;
} Particle;

extern f32 D_800A1B00[3];
extern f32 D_800A1B0C[3];
extern s32 func_15145EA4(f32 **src, f32 **dst, Mtx *mtx, s32 count);
extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spark effect: while the owner lives, emits sparks along the segment between two points of
 * its bone 5, at a random rate. */
void func_150F5C98(SparkEffect *effect) {
    Actor *owner;
    SparkData *data;
    ParticleDesc spawn;
    f32 scale;
    Vec3f start;
    Vec3f end;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 *srcs[2];
    f32 *dsts[2];

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->head.state = -1;
        return;
    }
    if (owner->unk1D4 != NULL) {
        data->accum += (0.2600000203f + func_150ADA68() * 0.1950000077f) * D_800BE9A4;
        if (data->accum > 1.0f) {
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.r2 = 0xB4;
            spawn.g2 = 0xB4;
            spawn.b2 = 0xB4;
            srcs[0] = D_800A1B00;
            srcs[1] = D_800A1B0C;
            dsts[0] = &start.x;
            dsts[1] = &end.x;
            func_15145EA4(srcs, dsts, (Mtx *)owner->unk1D4 + 5, 2);
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
                Particle *spark;

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
                spark = func_15130280(&spawn, 1, 0, 4, effect->head.owner, effect->head.kind);
                if (spark != NULL) {
                    memcpy(&spark->unkA8, &scale, sizeof(scale));
                }
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        }
    }
}

/* Effect callback for the spark effect: hands func_15149514 the effect's owner reference
 * (pointer and id). */
void func_150F6138(SparkEffect *effect, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

typedef struct BeamEffect BeamEffect;

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ BeamEffect *effect; /* the beam effect that drives this beam */
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ u8 flags; /* bit 0 = shown */
} BeamPart;

/* Something drawn between two points (a beam or streak). */
typedef struct {
    DefParticleHead head; /* head.def.pos / rot: the two end points */
    BeamPart part;
} Beam;

typedef struct {
    u8 pad0[9];
    u8 hidden;
    u8 padA[4];
    s16 position[3];
} FlashSprite;

/* A flash created by func_151602C0. */
typedef struct {
    u8 pad0[0x14];
    FlashSprite *sprite;
} Flash;

typedef struct {
    Actor *owner;
    u8 ownerId;
    u8 pad5[3];
    Beam *beam;
    Flash *flash;
} BeamData;

struct BeamEffect {
    EffectHeader head;
    u8 pad10[0x18];
    BeamData data;
};

extern f32 D_800A1B18[3];
extern f32 D_800A1B24[3];

/* Beam effect: while the owner lives and has a model (and unk94 bit 1 is clear), stretches the
 * beam between two points of the model and puts the flash at the first; otherwise hides both. */
void func_150F6178(BeamEffect *effect) {
    BeamData *data;
    Actor *owner;
    Vec3f start;
    Vec3f end;
    f32 *srcs[2];
    f32 *dsts[2];

    data = &effect->data;
    owner = data->owner;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->head.state = -1;
        return;
    }
    if (owner->unk1D4 != NULL && !(owner->unk94 & 2)) {
        Beam *beam;
        Flash *flash;
        FlashSprite *sprite;

        srcs[0] = D_800A1B18;
        srcs[1] = D_800A1B24;
        dsts[0] = &start.x;
        dsts[1] = &end.x;
        func_15145EA4(srcs, dsts, (Mtx *)owner->unk1D4, 2);
        if (data->beam != NULL) {
            beam = data->beam;
            beam->part.flags |= 1;
            beam->head.def.pos = start;
            beam->head.def.rot = end;
        }
        flash = data->flash;
        if (flash != NULL) {
            sprite = flash->sprite;
            sprite->hidden = FALSE;
            sprite->position[0] = start.x;
            sprite->position[1] = start.y;
            sprite->position[2] = start.z;
        }
    } else {
        if (data->beam != NULL) {
            BeamPart *part = &data->beam->part;

            part->flags &= ~1;
        }
        if (data->flash != NULL) {
            FlashSprite *sprite = data->flash->sprite;

            sprite->hidden = TRUE;
        }
    }
    if (data->beam != NULL) {
        BeamPart *part = &data->beam->part;

        part->unk48 = 4.0f;
        part->unk4C = 8.0f;
    }
}

/* Frees the beam effect's beam and flash. */
void func_150F631C(BeamEffect *effect) {
    BeamData *data = &effect->data;

    if (data->beam != NULL) {
        func_1516972C((EffectHeader *)data->beam);
    }
    if (data->flash != NULL) {
        func_1516972C((EffectHeader *)data->flash);
    }
}

/* Beam effect destructors: free the beam and flash, then the effect. */
void func_150F6368(BeamEffect *effect) {
    func_150F631C(effect);
    func_1514933C((TimedEffect *)effect);
}


void func_150F6394(BeamEffect *effect) {
    func_150F631C(effect);
    func_15149368((TimedEffect *)effect);
}

/* Effect callback for the beam effect: hands func_15169850 the owner reference. */
void func_150F63C0(BeamEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Clears the beam effect's reference to this beam. */
void func_150F6400(Beam *beam) {
    BeamPart *part = &beam->part;

    if (part->effect != NULL) {
        BeamData *data = &part->effect->data;

        data->beam = NULL;
    }
}

/* Beam destructors: unlink the beam from its effect, then free it. */
void func_150F6420(Beam *beam) {
    func_150F6400(beam);
    func_1513CA6C(&beam->head);
}

void func_150F644C(Beam *beam) {
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

/* Data of the effect run by func_150F64DC. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer; /* frames until the next burst */
} FlashBurstData;

typedef struct {
    EffectHeader head;
    u8 pad10[0x18];
    FlashBurstData data;
} FlashBurstEffect;

extern f32 D_800A1B30[3];
void func_15143134(void *, f32 *, Mtx *);
extern LightEffect *func_151602C0(LightEffectHeader *, Vec3i *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
void *func_15107C1C(Actor *, u8, void *, s16, s32, s32, s32, f32, f32, s32, s32, u8 *, s32, s32);

/* Burst effect: while the owner lives and has a model (and unk94 bit 1 is clear), every 90-180
 * frames calls func_10010F88(0x679) at the owner, makes a flash at a point of the model and emits
 * 2 or 3 blue-white particles from it. */
void func_150F64DC(FlashBurstEffect *effect) {
    FlashBurstData *data;
    Actor *owner;
    s8 count;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->interaction_state == 0) || (owner->unique_id != data->ownerId)) {
        effect->head.state = -1;
        return;
    }
    if ((owner->unk1D4 != NULL) && !(owner->unk94 & 2)) {
        data->timer -= D_800BE9E4;
        if (data->timer < 0) {
            func_10010F88(0x679, 0x18CE, 0, 0, 0, owner->x_position, owner->y_position, owner->z_position,
                          0x7918, 0x7D00);
            count = (func_150ADA20() & 1) + 2;
            {
                f32 pos[3];
                LightEffectHeader header;
                Vec3i origin;
                u8 color[4];
                s32 rand1;
                s32 rand2;
                s32 rand3;

                func_15143134(D_800A1B30, pos, (Mtx *)owner->unk1D4);
                header.flags = 3;
                header.update = -1;
                header.lifetime = (func_150ADA20() % 9U) + 10;
                header.lightType = 0;
                origin.x = pos[0];
                origin.y = pos[1];
                origin.z = pos[2];
                rand3 = (func_150ADA20() % 61U) + 60;
                func_151602C0(&header, &origin, rand3, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, effect->head.owner,
                              effect->head.kind);
                do {
                    color[0] = 0xA0;
                    color[1] = 0xA0;
                    color[2] = 0xFF;
                    color[3] = (func_150ADA20() % 101U) + 155;
                    rand1 = func_150ADA20();
                    rand2 = func_150ADA20();
                    rand3 = func_150ADA20();
                    func_15107C1C(owner, 0, D_800A1B30, (u8)rand1, (rand2 % 43U) - 50, (rand3 % 18U) + 5, 4,
                                  27.0f, (func_150ADA68() * 16.0f) + 20.0f, 0, 2, color, effect->head.owner,
                                  effect->head.kind);
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

void *func_15134DAC(BloodStripDesc *, s32);

/* Creates a func_15134DAC object attached to `owner`. */
void func_150F6890(Actor *owner, s16 arg1, s32 arg2, s32 arg3) {
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

/* Partial view of game_161520.c's BloodStrip. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1C - 0xD];
    /* 0x1C */ Actor *owner;
} BloodStrip;

/* Ground probe result filled by func_1504715C (game_105760.c's Ground_150D85AC). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} Ground;

void func_15137F30(f32 *pointA, f32 *pointB, f32 *velocityA, f32 *velocityB, f32 time, BloodStrip *strip,
                   f32 *start, f32 *end, f32 *velocity, f32 *arg3, s16 *arg4, s8 *arg5, f32 *arg6);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
void func_1504715C(Ground *, Actor *);

/* BloodStrip emit callback: one func_151D9014 blood particle, larger than func_15137E60's. 60% of
 * them get a ground probe under the strip's owner; the rest get none (height -10000). */
void func_150F695C(f32 *pointA, f32 *pointB, f32 *velocityA, f32 *velocityB, f32 time, BloodStrip *strip) {
    f32 start[3];
    f32 end[3];
    f32 velocity[3];
    f32 arg3;
    s16 arg4;
    s8 arg5;
    f32 arg6;
    Ground ground;
    u8 probeGround;
    s32 flag;
    f32 random;

    func_15137F30(pointA, pointB, velocityA, velocityB, time, strip, start, end, velocity, &arg3,
                  &arg4, &arg5, &arg6);
    random = func_150ADA68();
    arg6 *= 1.0f + (random + random);
    probeGround = func_150ADA68() < 0.6f;
    if (probeGround) {
        func_1504715C(&ground, strip->owner);
    } else {
        ground.unk18 = 0;
        ground.unk1C = 0;
        ground.unk1D = 0;
        ground.unk20 = 0;
        ground.height = -10000.0f;
    }
    if (probeGround) {
        flag = func_150ADA68() < 0.5f;
    } else {
        flag = 0;
    }
    func_151D9014(start, velocity, 1, arg3, arg4, (u8)arg5, arg6, probeGround, 1.8f, 1.8f, 1,
                  (s32)&ground, 1, flag, strip->unkC, strip->unk1);
}
