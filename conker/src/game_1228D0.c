#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_150F6890;

extern u8 D_800917F8;
extern u8 D_80091930;
extern u8 D_8009193C;
extern u8 D_80091948;
extern f32 D_800A1B70;
struct102 *func_15169968(struct_7484 *);
void *func_15134DAC(struct_150F6890 *, s32);

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

/* Partial view of the first argument of the draw callbacks func_150F56B0 and func_150F5A54. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 texture;
    /* 0x1A */ u8 pad1A[0x24 - 0x1A];
    /* 0x24 */ Gfx **displayList;
    /* 0x28 */ u8 pad28[0x38 - 0x28];
    /* 0x38 */ s32 fadeState; /* 0 = start, 1 = shown, 2 = faded out */
} DrawHook;

/* Picks the hook's texture (an entry of D_80090274) from the actor's animation frame while
 * the actor is in state 0xAE. */
s32 func_150F56B0(DrawHook *hook, struct127 *actor) {
    struct197 *anim;
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
        if ((anim->unk8 >= 36.0f) && (anim->unk8 <= 51.0f)) {
            t = anim->unk8 - 36.0f;
            t *= 0.0625f;
            t = 1.0f - t;
            frame = (s32)(6.0f * t);
        } else if ((anim->unk8 > 51.0f) && (anim->unk8 <= 54.0f)) {
            frame = 0;
        } else if ((anim->unk8 > 54.0f) && (anim->unk8 <= 60.0f)) {
            t = anim->unk8 - 54.0f;
            t *= 0.1428571492f;
            frame = (s32)(6.0f * t);
        } else if ((anim->unk8 > 60.0f) && (anim->unk8 <= 65.0f)) {
            t = anim->unk8 - 60.0f;
            t *= 0.1666666716f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->unk8 > 65.0f) && (anim->unk8 <= 67.0f)) {
            t = anim->unk8 - 65.0f;
            t *= 0.3333333433f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        } else if ((anim->unk8 > 67.0f) && (anim->unk8 <= 70.0f)) {
            t = anim->unk8 - 67.0f;
            t *= 0.25f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 4;
        } else if ((anim->unk8 > 70.0f) && (anim->unk8 <= 74.0f)) {
            t = anim->unk8 - 70.0f;
            t *= 0.200000003f;
            frame = (s32)(two * t) + 4;
        } else if ((anim->unk8 > 74.0f) && (anim->unk8 <= 119.0f)) {
            t = anim->unk8 - 74.0f;
            t *= 0.02173913084f;
            frame = (s32)(two * t) + 7;
        } else if ((anim->unk8 > 119.0f) && (anim->unk8 <= 150.0f)) {
            t = anim->unk8 - 119.0f;
            t *= 0.03125f;
            t = 1.0f - t;
            frame = (s32)(two * t) + 7;
        }
    }
    hook->texture = D_80090274[frame];
    return 0;
}

/* While the actor is in state 5, slides the hook's texture over animation frames 69-84 by
 * patching the upper-left t of the first G_SETTILESIZE in its display list. */
s32 func_150F5A54(DrawHook *hook, struct127 *actor) {
    struct197 *anim;
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

    gfx = *hook->displayList;
    count = 1;
    notStarted = FALSE;
    switch (hook->fadeState) {
    case 0:
        if (actor->unk84.uh == 5) {
            level = 1.0f;
            hook->fadeState = 1;
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
        if (anim->unk8 < 69.0f) {
            level = 1.0f;
        } else if (84.0f < anim->unk8) {
            level = 0.0f;
            hook->fadeState = 2;
        } else {
            level = (anim->unk8 - 69.0f) / 15;
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

/* Common head of an effect object (struct260 is a shorter view). Each effect keeps its own data
 * from +0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} EffectHead;

/* Data of the spark effect spawned by func_150F5C08 and run by func_150F5C98. */
typedef struct {
    struct127 *owner;
    u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    u8 pad5[3];
    f32 accum;  /* sparks owed; one is spawned per whole unit */
} SparkData;

typedef struct {
    EffectHead head;
    SparkData data;
} SparkEffect;

/* Spawns a spark effect (func_150F5C98) on `owner`. */
void func_150F5C08(struct127 *owner, s16 arg1, u8 arg2, s32 arg3) {
    SparkData data;
    struct260 *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.accum = 0.0f;

    // the header's struct37 * parameter is really the size of the effect's data
    effect = func_15149130(arg1, -1, 0x51, -1, 1, 0x3E, (struct37 *)sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&((SparkEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Particle descriptor passed to func_15130280 (0x70 bytes). */
typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 lifetime;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 flags;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} ParticleDesc; /* 0x70 */

/* Partial view of the particle func_15130280 creates. */
typedef struct {
    u8 pad0[0xA8];
    f32 unkA8;
} Particle;

extern f32 D_800A1B00[3];
extern f32 D_800A1B0C[3];
extern s32 func_15145EA4(f32 **src, f32 **dst, Mtx *mtx, s32 count);
extern Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);

/* Spark effect: while the owner lives, emits sparks along the segment between two points of
 * its bone 5, at a random rate. */
void func_150F5C98(SparkEffect *effect) {
    struct127 *owner;
    SparkData *data;
    ParticleDesc spawn;
    f32 scale;
    struct17 start;
    struct17 end;
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
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk18 = 0xB4;
            spawn.unk19 = 0xB4;
            spawn.unk1A = 0xB4;
            srcs[0] = D_800A1B00;
            srcs[1] = D_800A1B0C;
            dsts[0] = &start.unk0;
            dsts[1] = &end.unk0;
            func_15145EA4(srcs, dsts, (Mtx *)owner->unk1D4 + 5, 2);
            spawn.unk1D = 0x6C;
            spawn.unk8 = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x9F0600;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk1C = 0xFF;
            dx = end.unk0 - start.unk0;
            dy = end.unk4 - start.unk4;
            dz = end.unk8 - start.unk8;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.flags = 0x84CE07;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x10;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 1000;
            spawn.unk6C = 1000.0f;
            scale = 0.9730340242f;
            spawn.unk24 = 1.022472024f;
            do {
                f32 speed;
                f32 t;
                s32 flipX;
                Particle *spark;

                spawn.lifetime = func_150ADA20() % 21U + 20;
                spawn.unk22 = spawn.lifetime;
                spawn.unk1E = spawn.lifetime;
                spawn.unk20 = 0xFF / spawn.lifetime;
                spawn.unk1B = func_150ADA20() % 26U + 8;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 70.0f + 148.0f;
                spawn.unk54 = func_150ADA68() * 0.2190000117f + 0.08600000292f;
                speed = func_150ADA68() * 0.04100000113f + 0.07800000161f;
                spawn.vel.unk0 = dx * speed;
                spawn.vel.unk4 = dy * speed;
                spawn.vel.unk8 = dz * speed;
                /* Start a random part of a frame along the path. */
                t = func_150ADA68() * D_800BE9A4;
                spawn.flags &= ~0xC0;
                spawn.pos.unk0 = spawn.vel.unk0 * t + start.unk0;
                spawn.pos.unk4 = spawn.vel.unk4 * t + start.unk4;
                spawn.pos.unk8 = spawn.vel.unk8 * t + start.unk8;
                flipX = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | flipX;
                spark = func_15130280(&spawn, 1, 0, 4, effect->head.unkC, effect->head.unk1);
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
    func_15149514(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
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
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x110 - 0x4C];
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
    struct127 *owner;
    u8 ownerId;
    u8 pad5[3];
    Beam *beam;
    Flash *flash;
} BeamData;

struct BeamEffect {
    EffectHead head;
    BeamData data;
};

extern f32 D_800A1B18[3];
extern f32 D_800A1B24[3];

/* Beam effect: while the owner lives and has a model (and unk94 bit 1 is clear), stretches the
 * beam between two points of the model and puts the flash at the first; otherwise hides both. */
void func_150F6178(BeamEffect *effect) {
    BeamData *data;
    struct127 *owner;
    struct17 start;
    struct17 end;
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
        dsts[0] = &start.unk0;
        dsts[1] = &end.unk0;
        func_15145EA4(srcs, dsts, (Mtx *)owner->unk1D4, 2);
        if (data->beam != NULL) {
            beam = data->beam;
            beam->part.flags |= 1;
            beam->start = start;
            beam->end = end;
        }
        flash = data->flash;
        if (flash != NULL) {
            sprite = flash->sprite;
            sprite->hidden = FALSE;
            sprite->position[0] = start.unk0;
            sprite->position[1] = start.unk4;
            sprite->position[2] = start.unk8;
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
        func_1516972C((struct102 *)data->beam);
    }
    if (data->flash != NULL) {
        func_1516972C((struct102 *)data->flash);
    }
}

/* Beam effect destructors: free the beam and flash, then the effect. */
void func_150F6368(BeamEffect *effect) {
    func_150F631C(effect);
    func_1514933C((struct260 *)effect);
}

void func_15149368(struct260 *);

void func_150F6394(BeamEffect *effect) {
    func_150F631C(effect);
    func_15149368((struct260 *)effect);
}

/* Effect callback for the beam effect: hands func_15169850 the owner reference. */
void func_150F63C0(BeamEffect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
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
    func_1513CA6C((struct210 *)beam);
}

void func_150F644C(Beam *beam) {
    func_150F6400(beam);
    func_1513CAA0((struct210 *)beam);
}

void func_150F6478(struct210 *arg0) {
}

void func_150F6484(struct210 *arg0) {
    func_150F6478(arg0);
    func_151411A4(arg0);
}

void func_151411C4(struct210 *);

void func_150F64B0(struct210 *arg0) {
    func_150F6478(arg0);
    func_151411C4(arg0);
}

typedef struct {
    /* 0x00 */ struct127 *obj;
    /* 0x04 */ u8 id;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer;
} Func150F64DCLink;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Func150F64DCLink link;
} Func150F64DCSelf;

extern u8 D_800A1B30;
void func_15143134(void *, f32 *, s32);
extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
void *func_15107C1C(struct127 *, u8, void *, s16, s32, s32, s32, f32, f32, s32, s32, u8 *, s32, s32);

void func_150F64DC(Func150F64DCSelf *arg0) {
    Func150F64DCLink *link;
    struct127 *obj;
    s8 count;

    obj = arg0->link.obj;
    link = &arg0->link;
    if ((obj->interaction_state == 0) || (obj->unique_id != link->id)) {
        arg0->unkE = -1;
        return;
    }
    if ((obj->unk1D4 != 0) && !(*(s32 *)((u8 *)obj + 0x94) & 2)) {
        link->timer -= D_800BE9E4;
        if (link->timer < 0) {
            func_10010F88(0x679, 0x18CE, 0, 0, 0, obj->x_position, obj->y_position, obj->z_position, 0x7918, 0x7D00);
            count = (func_150ADA20() & 1) + 2;
            {
    f32 pos[3];
    Header header;
    Header2 header2;
    u8 color[4];
    s32 r1;
    s32 r2;
    s32 r3;
            func_15143134(&D_800A1B30, pos, (s32)obj->unk1D4);
            header.unk0 = 3;
            header.unk1 = -1;
            header.unk2 = (func_150ADA20() % 9U) + 10;
            header.unk4 = 0;
            header2.unk0 = pos[0];
            header2.unk4 = pos[1];
            header2.unk8 = pos[2];
            r3 = (func_150ADA20() % 61U) + 60;
            func_151602C0(&header, &header2, r3, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);
            do {
                color[0] = 0xA0;
                color[1] = 0xA0;
                color[2] = 0xFF;
                color[3] = (func_150ADA20() % 101U) + 155;
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                r3 = func_150ADA20();
                func_15107C1C(obj, 0, &D_800A1B30, (u8)r1, (r2 % 43U) - 50, (r3 % 18U) + 5, 4, 27.0f, (func_150ADA68() * 16.0f) + 20.0f, 0, 2, color, arg0->unkC, arg0->unk1);
                count--;
            } while (count > 0);
            link->timer = (func_150ADA20() % 91U) + 90;
            }
        }
    }
}

void func_150F6850(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_150F6890(void *arg0, s16 arg1, s32 arg2, s32 arg3) {
    struct_150F6890 sp1C;

    sp1C.unk0 = *((u8 *)arg0 + 0x3B);
    sp1C.unk1C = D_800A1B70;
    sp1C.unk20 = 8.5f;
    sp1C.unk34 = 1.0f;
    sp1C.unk4 = arg0;
    sp1C.unk8 = 3;
    sp1C.unkC = 0.0f;
    sp1C.unk10 = 0.0f;
    sp1C.unk14 = 0.0f;
    sp1C.unk18 = 0.0f;
    sp1C.unk24 = 2;
    sp1C.unk26 = 0x28;
    sp1C.unk28 = 0x10;
    sp1C.unk2A = arg1;
    sp1C.unk2E = 5;
    sp1C.unk2F = 8;
    sp1C.unk30 = -1;
    sp1C.unk38 = 0;
    sp1C.unk39 = -1;
    sp1C.unk2C = 0;

    func_15134DAC(&sp1C, 0);
}

void func_15137F30(void *, void *, void *, void *, f32, void *, void *, void *,
                   void *, f32 *, s16 *, s8 *, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32,
                   s32, s32, s32, s32, s32, s32);
void func_1504715C(f32 *, s32);

void func_150F695C(void *arg0, void *arg1, void *arg2, void *arg3,
                   f32 arg4, u8 *arg5) {
    struct {
        f32 value;
        u8 pad60[0x14];
        s32 field74;
        s8 field78;
        s8 field79;
        u8 pad7A[2];
        s32 field7C;
        f32 scale;
        u8 pad84;
        s8 field85;
        s16 field86;
        f32 field88;
        f32 vector8C[3];
        u8 field98[0xC];
        f32 vectorA4[3];
    } local;
    u8 selected;
    s32 variant;
    f32 random;

    func_15137F30(arg0, arg1, arg2, arg3, arg4, arg5,
                  local.vectorA4, local.field98, local.vector8C,
                  &local.field88, &local.field86, &local.field85,
                  &local.scale);
    random = func_150ADA68();
    local.scale *= 1.0f + (random + random);
    selected = func_150ADA68() < 0.6f;
    if (selected != 0) {
        func_1504715C(&local.value, *(s32 *)(arg5 + 0x1C));
    } else {
        local.field74 = 0;
        local.field78 = 0;
        local.field79 = 0;
        local.field7C = 0;
        local.value = -10000.0f;
    }
    if (selected) {
        variant = func_150ADA68() < 0.5f;
    } else {
        variant = 0;
    }
    func_151D9014(local.vectorA4, local.vector8C, 1, local.field88,
                  local.field86, (u8)local.field85, local.scale, selected,
                  1.8f, 1.8f, 1, (s32)&local.value, 1,
                  variant, arg5[0xC], arg5[1]);
}
