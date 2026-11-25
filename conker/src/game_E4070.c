#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-update-routine parameters (sprite +0x18, descriptor +0x08). */
typedef union {
    struct {
        s32 frame; /* frame (8.8) to stop scrolling at */
        s32 y;     /* y to stop scrolling at */
        s32 stage;
    } scroll; /* func_150B6E3C */
    struct {
        s32 elapsed;
    } slide; /* func_150B709C */
    struct {
        s16 endX;
        s16 endY;
        s32 duration;
        s16 startX;
        s16 startY;
    } move; /* func_150B73F0: from start to end as the timer runs down */
    s32 words[3]; /* move's points packed as x << 16 | y */
} SpriteParams;

/* SpriteObj descriptor passed to func_15169968/func_15169900, which copy it to the new sprite at +0x10. */
typedef struct {
    /* 0x00 */ void *texture;
    /* 0x04 */ void *unk4;
    /* 0x08 */ SpriteParams params;
    /* 0x14 */ s16 timer;
    /* 0x16 */ s16 frame;      /* 8.8 animation frame */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 x;
    /* 0x20 */ f32 y;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 scaleX;     /* 0x1000 = 1.0 */
    /* 0x2A */ s16 scaleY;
    /* 0x2C */ s16 width;
    /* 0x2E */ s16 height;
    /* 0x30 */ u8 update;      /* index into the D_8008CA20 update routines */
    /* 0x31 */ u8 frameCount;
    /* 0x32 */ u8 red;
    /* 0x33 */ u8 green;
    /* 0x34 */ u8 blue;
    /* 0x35 */ u8 alpha;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
} SpriteDesc;

/* A sprite object: a 0x10-byte header followed by its SpriteDesc fields. */
typedef struct {
    /* 0x00 */ u8 header[0x10];
    /* 0x10 */ void *texture;
    /* 0x14 */ void *unk14;
    /* 0x18 */ SpriteParams params;
    /* 0x24 */ s16 timer;
    /* 0x26 */ s16 frame;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 x;
    /* 0x30 */ f32 y;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 scaleX;
    /* 0x3A */ s16 scaleY;
    /* 0x3C */ s16 width;
    /* 0x3E */ s16 height;
    /* 0x40 */ u8 update;
    /* 0x41 */ u8 frameCount;
    /* 0x42 */ u8 red;
    /* 0x43 */ u8 green;
    /* 0x44 */ u8 blue;
    /* 0x45 */ u8 alpha;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
} SpriteObj;


void func_150B6C90(void);
void func_150B6D34(void);
void func_150B6D78(void);
void func_150B7484(void);
void func_150B7560(void);
void func_150B765C(void);
void func_150B77A8(void);
void func_150B768C(void);
void func_150B791C(void);

/* Runs the command queued in D_800D9890 (each handler resets it to 3). */
void func_150B6BC0(s32 unused) {
    switch (D_800D9890) {
    case 1:
        func_150B6C90();
        return;
    case 2:
        func_150B6D34();
        return;
    case 4:
        func_150B6D78();
        return;
    case 5:
        func_150B7484();
        return;
    case 6:
        func_150B7560();
        return;
    case 7:
        func_150B765C();
        return;
    case 8:
        func_150B77A8();
        return;
    case 9:
        func_150B768C();
        return;
    case 10:
        func_150B791C();
    default:
        return;
    }
}

// matching: defined in-file so as1 shares one lui across the unrolled stores in func_150B6C90
static EffectHeader *D_800D9898[10];
struct Obj151A4FD0 *func_151A4FD0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 a6, s32 a7);

/* Recreates the func_151A4FD0 effect and clears every sprite slot (without freeing the sprites). */
void func_150B6C90(void) {
    s32 i;

    if (D_800D9894 != NULL) {
        func_1516972C(D_800D9894);
    }
    D_800D9894 = (EffectHeader *)func_151A4FD0(0, 0, 0xFF, 0, 0, D_800BE9E8, 0, 0);
    D_800D9890 = 3;
    for (i = 0; i < 10; i++) {
        D_800D9898[i] = NULL;
    }
}

/* Lets the three scrolling sprites in slots 5-7 stop once they reach their target frame and y. */
void func_150B6D34(void) {
    s32 i;
    SpriteObj *sprite;

    for (i = 0; i < 3; i++) {
        sprite = (SpriteObj *)D_800D9898[i + 5];
        if (sprite != NULL) {
            sprite->params.scroll.stage = 1;
        }
    }
    D_800D9890 = 3;
}

/* Frees the func_151A4FD0 effect and every sprite. */
void func_150B6D78(void) {
    s32 i;

    if (D_800D9894 != NULL) {
        func_1516972C(D_800D9894);
        D_800D9894 = NULL;
    }
    for (i = 0; i < 10; i++) {
        if (D_800D9898[i] != NULL) {
            func_1516972C(D_800D9898[i]);
            D_800D9898[i] = NULL;
        }
    }
    D_800D9890 = 3;
}

/* Update routine 4: advances unk34 by 0x30 per frame, wrapping from above 0x800 back to -0xC00. */
void func_150B6DFC(SpriteObj *sprite) {
    sprite->unk34 += D_800BE9E4 * 0x30;
    if (sprite->unk34 > 0x800) {
        sprite->unk34 = -0xC00;
    }
}

extern u8 D_80091918[];

/* Update routine 5: scrolls the sprite up through its animation frames, fading it with its distance
 * from y = 170, until it reaches its target frame and y. Once stopped at y = 170 it blinks, and after
 * the third blink it turns into the D_80091918 sprite run by update routine 8 (func_150B709C). */
void func_150B6E3C(SpriteObj *sprite) {
    u32 three;
    f32 dist;
    s32 alpha;

    dist = 0.0f; // matching: a dead first def makes IDO give dist the first FP register
    three = 3;   // matching: one register for the multu step and the stage compare
    if (sprite->params.scroll.stage == 0 || sprite->frame != sprite->params.scroll.frame ||
        (dist = (sprite->y < (f32)sprite->params.scroll.y) ? -(sprite->y - (f32)sprite->params.scroll.y)
                                                          : (sprite->y - (f32)sprite->params.scroll.y),
         (f32)(s32)(D_800BE9E4 * three) < dist)) {
        sprite->y -= (f32)(s32)(D_800BE9E4 * three);
        if (sprite->y <= 140.0f) {
            sprite->frame += 0x100;
            if (sprite->frame >= (sprite->frameCount << 8)) {
                sprite->frame = 0;
            }
            sprite->y = 200.0f - (140.0f - sprite->y);
        }
        alpha = 0xFF - ((sprite->y < 170.0f) ? (s32)-(sprite->y - 170.0f) : (s32)(sprite->y - 170.0f)) * 8;
        if (alpha < 0) {
            alpha = 0;
        }
        sprite->alpha = alpha;
        return;
    }

    sprite->y = sprite->params.scroll.y;
    if (sprite->params.scroll.y == 170) {
        alpha = sprite->alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            sprite->params.scroll.stage++;
            if (sprite->params.scroll.stage == three) {
                sprite->texture = D_80091918;
                sprite->update = 8;
                sprite->alpha = 0xFF;
                sprite->width = 128;
                sprite->height = 16;
                sprite->frame = 0;
                sprite->params.slide.elapsed = 0;
                return;
            }
            alpha = 0;
        }
        sprite->alpha = alpha;
        return;
    }
    alpha = 0xFF - ((sprite->y < 170.0f) ? (s32)-(sprite->y - 170.0f) : (s32)(sprite->y - 170.0f)) * 9;
    if (alpha < 0) {
        alpha = 0;
    }
    sprite->alpha = alpha;
}

/* Update routine 8: slides the sprite from (230, 170) to (146, 120) over 30 steps, then blinks it. */
void func_150B709C(SpriteObj *sprite) {
    s32 elapsed;
    s32 alpha;

    elapsed = sprite->params.slide.elapsed;
    if (elapsed == 30) {
        alpha = sprite->alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            alpha = 0;
        }
        sprite->alpha = alpha;
    }
    elapsed += D_800BE9E4 * 2;
    if (elapsed > 30) {
        elapsed = 30;
    }
    sprite->params.slide.elapsed = elapsed;
    sprite->x = elapsed * -84 / 30 + 230;
    sprite->y = elapsed * -50 / 30 + 170;
}

/* Update routine 6: grows scaleX up to 1.0, then scaleY. */
void func_150B71A8(SpriteObj *sprite) {
    if (sprite->scaleX != 0x1000) {
        sprite->scaleX += D_800BE9E4 * 0x100;
        if (sprite->scaleX > 0x1000) {
            sprite->scaleX = 0x1000;
        }
    } else if (sprite->scaleY != 0x1000) {
        sprite->scaleY += D_800BE9E4 * 0x100;
        if (sprite->scaleY > 0x1000) {
            sprite->scaleY = 0x1000;
        }
    }
}

extern u8 D_80091924;
s32 func_15169900(SpriteDesc *desc, s32 arg1);

/* Spawns four D_80091924 sprites (update routine 7) that converge on (arg0, arg1) from above, below,
 * left and right over `duration` frames. */
void func_150B7220(s32 arg0, s32 arg1, s32 duration, s32 arg3) {
    SpriteDesc desc;
    s32 r;
    s32 x;
    s32 y;

    // matching: copies of the args give the 0x80 frame and IDO's register choice
    x = arg0;
    y = arg1;
    desc.texture = &D_80091924;
    desc.params.words[0] = (x << 16) | (y & 0xFFFF);
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    desc.params.words[1] = duration;
    desc.timer = duration;
    desc.unk18 = 0;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.width = 16;
    desc.height = 16;
    desc.update = 7;
    desc.frameCount = 2;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    if (y) {} // matching: steers IDO's s1/s2 choice for y and r
    desc.alpha = 0xFF;
    desc.unk36 = 7;
    desc.unk37 = 0x11;
    if (x < (D_800BE638[0] >> 1)) {
        do { // matching: the loop's -g3 markers steer IDO's register choice for r
            r = D_800BE638[0] - x;
        } while (0);
    } else {
        r = x;
    }

    desc.params.words[2] = (x << 16) | ((y - r) & 0xFFFF);
    desc.unk39 = 0;
    desc.frame = 0;
    desc.x = x;
    desc.y = y - r;
    func_15169900(&desc, arg3);

    desc.params.words[2] = (x << 16) | ((y + r) & 0xFFFF);
    desc.unk39 = 8;
    desc.y = y + r;
    func_15169900(&desc, arg3);

    desc.x = x - r;
    desc.params.words[2] = ((x - r) << 16) | (y & 0xFFFF);
    desc.unk39 = 4;
    desc.frame = 0x100;
    desc.y = y;
    func_15169900(&desc, arg3);

    desc.params.words[2] = ((x + r) << 16) | (y & 0xFFFF);
    desc.unk39 = 0;
    desc.x = x + r;
    func_15169900(&desc, arg3);
}

/* Update routine 7: moves the sprite in a straight line from its start point to its end point as its
 * timer runs down from the duration to 0. */
void func_150B73F0(SpriteObj *sprite) {
    s32 endX = sprite->params.move.endX;
    s32 endY = sprite->params.move.endY;
    s32 startX = sprite->params.move.startX;
    s32 startY = sprite->params.move.startY;
    s32 duration = sprite->params.move.duration;
    s32 timer = sprite->timer;
    s32 t = (timer << 16) / duration; /* time left as a 16.16 fraction */

    sprite->x = (((startX - endX) * t) >> 16) + endX;
    sprite->y = (((startY - endY) * t) >> 16) + endY;
}


extern u8 D_800918DC;
extern u8 D_800918E8;
extern u8 D_800918F4;
extern u8 D_80091900;
EffectHeader *func_15169968(SpriteDesc *desc);

/* Replaces sprite slot 0 with a D_800918DC sprite at (60, 60) that grows in (update routine 6). */
void func_150B7484(void) {
    SpriteDesc desc;

    desc.frameCount = 1;
    desc.width = 88;
    desc.x = 60.0f;
    desc.y = 60.0f;
    desc.timer = 0;
    desc.frame = 0;
    desc.unk18 = 0;
    desc.height = 88;
    desc.update = 6;
    desc.texture = &D_800918DC;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.unk39 = 0;
    desc.unk36 = 7;
    desc.unk37 = 0x15;
    desc.unk38 = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (D_800D9898[0] != NULL) {
        func_1516972C(D_800D9898[0]);
    }
    D_800D9898[0] = func_15169968(&desc);
    D_800D9890 = 3;
}

/* Frees sprite slot 0 and puts a static 15-frame D_800918DC sprite at (60, 60) in slot 3. */
void func_150B7560(void) {
    SpriteDesc desc;

    if (D_800D9898[0] != NULL) {
        func_1516972C(D_800D9898[0]);
        D_800D9898[0] = NULL;
    }
    desc.unk18 = 0x4D;
    desc.frameCount = 15;
    desc.timer = 0;
    desc.frame = 0;
    desc.width = 88;
    desc.height = 88;
    desc.update = 0;
    desc.texture = &D_800918DC;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.unk36 = 7;
    desc.unk37 = 0x11;
    desc.unk38 = 1;
    desc.unk39 = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    desc.x = 60.0f;
    desc.y = 60.0f;
    if (D_800D9898[3] != NULL) {
        func_1516972C(D_800D9898[3]);
    }
    D_800D9898[3] = func_15169968(&desc);
    D_800D9890 = 3;
}

void func_150B76BC(s32 x, s32 slot);

/* Spawns the growing D_800918E8 sprite at x = 60 in slot 1. */
void func_150B765C(void) {
    func_150B76BC(0x3C, 1);
    D_800D9890 = 3;
}

/* Spawns the growing D_800918E8 sprite at x = 230 in slot 2. */
void func_150B768C(void) {
    func_150B76BC(0xE6, 2);
    D_800D9890 = 3;
}

/* Replaces sprite `slot` with a D_800918E8 sprite at (x, 170) that grows in (update routine 6). */
void func_150B76BC(s32 x, s32 slot) {
    SpriteDesc desc;

    desc.x = x;
    desc.y = 170.0f;
    desc.frameCount = 1;
    desc.width = 88;
    desc.height = 64;
    desc.update = 6;
    desc.timer = 0;
    desc.frame = 0;
    desc.unk18 = 0;
    desc.texture = &D_800918E8;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.unk39 = 0;
    desc.unk36 = 7;
    desc.unk37 = 0x15;
    desc.unk38 = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (D_800D9898[slot] != NULL) {
        func_1516972C(D_800D9898[slot]);
    }
    D_800D9898[slot] = func_15169968(&desc);
}

/* Frees sprite slot 1, then puts the D_800918F4/D_80091900 sprite (update routine 4) in slot 4
 * and a static D_800918E8 sprite in slot 8, both at (60, 170). */
void func_150B77A8(void) {
    SpriteDesc desc;

    if (D_800D9898[1] != NULL) {
        func_1516972C(D_800D9898[1]);
        D_800D9898[1] = NULL;
    }

    desc.x = 60.0f;
    desc.y = 170.0f;
    desc.width = 64;
    desc.height = 64;
    desc.update = 4;
    desc.texture = &D_800918F4;
    desc.unk4 = &D_80091900;
    desc.timer = 0;
    desc.frame = 0;
    desc.unk18 = 0;
    desc.frameCount = 0;
    desc.unk24 = -0x800;
    desc.unk26 = 9;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.unk36 = 0xD;
    desc.unk37 = 0x12;
    desc.unk38 = 2;
    desc.unk39 = 3;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (D_800D9898[4] != NULL) {
        func_1516972C(D_800D9898[4]);
    }
    D_800D9898[4] = func_15169968(&desc);

    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.width = 88;
    desc.height = 64;
    desc.update = 0;
    desc.unk36 = 7;
    desc.unk37 = 0x11;
    desc.texture = &D_800918E8;
    desc.unk38 = 1;
    if (D_800D9898[8] != NULL) {
        func_1516972C(D_800D9898[8]);
    }
    D_800D9898[8] = func_15169968(&desc);
    D_800D9890 = 3;
}

extern u8 D_8009190C;

/* Frees sprite slot 2, then puts three D_8009190C frames that scroll into place at y = 150, 170
 * and 190 (update routine 5) in slots 5-7, and a static D_800918E8 sprite at (230, 170) in slot 9. */
void func_150B791C(void) {
    SpriteDesc desc;

    if (D_800D9898[2] != NULL) {
        func_1516972C(D_800D9898[2]);
        D_800D9898[2] = NULL;
    }
    desc.x = 230.0f;
    desc.frameCount = 3;
    desc.width = 64;
    desc.height = 16;
    desc.update = 5;
    desc.unk24 = 0;
    desc.unk26 = 0;
    desc.timer = 0;
    desc.frame = 0;
    desc.unk18 = 0;
    desc.texture = &D_8009190C;
    desc.unk4 = NULL;
    desc.params.scroll.frame = 0;
    desc.y = 150.0f;
    desc.params.scroll.y = desc.y;
    desc.params.scroll.stage = 0;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0x80;
    desc.unk36 = 7;
    desc.unk37 = 0x11;
    desc.unk38 = 1;
    desc.unk39 = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (D_800D9898[5] != NULL) {
        func_1516972C(D_800D9898[5]);
    }
    D_800D9898[5] = func_15169968(&desc);
    desc.frame = 0x100;
    desc.params.scroll.frame = 0x100;
    desc.y = 170.0f;
    desc.params.scroll.y = desc.y;
    desc.alpha = 0xFF;
    if (D_800D9898[6] != NULL) {
        func_1516972C(D_800D9898[5]); /* sic: frees slot 5, not 6 */
    }
    D_800D9898[6] = func_15169968(&desc);
    desc.frame = 0x200;
    desc.params.scroll.frame = 0x200;
    desc.y = 190.0f;
    desc.params.scroll.y = desc.y;
    desc.alpha = 0x80;
    if (D_800D9898[7] != NULL) {
        func_1516972C(D_800D9898[5]); /* sic */
    }
    D_800D9898[7] = func_15169968(&desc);
    desc.timer = 0;
    desc.frame = 0;
    desc.unk18 = 0;
    desc.frameCount = 0;
    desc.width = 88;
    desc.height = 64;
    desc.alpha = 0xFF;
    desc.update = 0;
    desc.texture = &D_800918E8;
    desc.x = 230.0f;
    desc.y = 170.0f;
    if (D_800D9898[9] != NULL) {
        func_1516972C(D_800D9898[9]);
    }
    D_800D9898[9] = func_15169968(&desc);
    D_800D9890 = 3;
}
