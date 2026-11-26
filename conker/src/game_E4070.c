#include <ultra64.h>
#include "structs.h"

// This file's .bss (0x800D9890-0x800D98BF), in the original order. IDO places the variables that
// a header declares after the ones the .c file declares first, so these are declared only here, before
// functions.h: the order also depends on how many types and variables come before them (see init_50A0.c).
u8 D_800D9890; // queued command for func_150B6BC0
EffectHeader *D_800D9894;
// matching: defined in-file so as1 shares one lui across the unrolled stores in func_150B6C90
static struct Sprite2D *D_800D9898[10];

#include "functions.h"
#include "variables.h"


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
    Sprite2D *sprite;

    for (i = 0; i < 3; i++) {
        sprite = D_800D9898[i + 5];
        if (sprite != NULL) {
            sprite->params.data.scroll.stage = 1;
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
            func_1516972C(&D_800D9898[i]->header);
            D_800D9898[i] = NULL;
        }
    }
    D_800D9890 = 3;
}

/* Update routine 4: advances unk34 by 0x30 per frame, wrapping from above 0x800 back to -0xC00. */
void func_150B6DFC(Sprite2D *sprite) {
    sprite->params.sOffset += D_800BE9E4 * 0x30;
    if (sprite->params.sOffset > 0x800) {
        sprite->params.sOffset = -0xC00;
    }
}

extern u8 D_80091918[];

/* Update routine 5: scrolls the sprite up through its animation frames, fading it with its distance
 * from y = 170, until it reaches its target frame and y. Once stopped at y = 170 it blinks, and after
 * the third blink it turns into the D_80091918 sprite run by update routine 8 (func_150B709C). */
void func_150B6E3C(Sprite2D *sprite) {
    u32 three;
    f32 dist;
    s32 alpha;

    dist = 0.0f; // matching: a dead first def makes IDO give dist the first FP register
    three = 3;   // matching: one register for the multu step and the stage compare
    if (sprite->params.data.scroll.stage == 0 || sprite->params.frame != sprite->params.data.scroll.frame ||
        (dist = (sprite->params.y < (f32)sprite->params.data.scroll.y)
                    ? -(sprite->params.y - (f32)sprite->params.data.scroll.y) : (sprite->params.y - (f32)sprite->params.data.scroll.y),
         (f32)(s32)(D_800BE9E4 * three) < dist)) {
        sprite->params.y -= (f32)(s32)(D_800BE9E4 * three);
        if (sprite->params.y <= 140.0f) {
            sprite->params.frame += 0x100;
            if (sprite->params.frame >= (sprite->params.frameCount << 8)) {
                sprite->params.frame = 0;
            }
            sprite->params.y = 200.0f - (140.0f - sprite->params.y);
        }
        alpha = 0xFF - ((sprite->params.y < 170.0f) ? (s32)-(sprite->params.y - 170.0f)
                                                      : (s32)(sprite->params.y - 170.0f)) * 8;
        if (alpha < 0) {
            alpha = 0;
        }
        sprite->params.alpha = alpha;
        return;
    }

    sprite->params.y = sprite->params.data.scroll.y;
    if (sprite->params.data.scroll.y == 170) {
        alpha = sprite->params.alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            sprite->params.data.scroll.stage++;
            if (sprite->params.data.scroll.stage == three) {
                sprite->params.texture = D_80091918;
                sprite->params.behavior = 8;
                sprite->params.alpha = 0xFF;
                sprite->params.width = 128;
                sprite->params.height = 16;
                sprite->params.frame = 0;
                sprite->params.data.slide.elapsed = 0;
                return;
            }
            alpha = 0;
        }
        sprite->params.alpha = alpha;
        return;
    }
    alpha = 0xFF - ((sprite->params.y < 170.0f) ? (s32)-(sprite->params.y - 170.0f)
                                                      : (s32)(sprite->params.y - 170.0f)) * 9;
    if (alpha < 0) {
        alpha = 0;
    }
    sprite->params.alpha = alpha;
}

/* Update routine 8: slides the sprite from (230, 170) to (146, 120) over 30 steps, then blinks it. */
void func_150B709C(Sprite2D *sprite) {
    s32 elapsed;
    s32 alpha;

    elapsed = sprite->params.data.slide.elapsed;
    if (elapsed == 30) {
        alpha = sprite->params.alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            alpha = 0;
        }
        sprite->params.alpha = alpha;
    }
    elapsed += D_800BE9E4 * 2;
    if (elapsed > 30) {
        elapsed = 30;
    }
    sprite->params.data.slide.elapsed = elapsed;
    sprite->params.x = elapsed * -84 / 30 + 230;
    sprite->params.y = elapsed * -50 / 30 + 170;
}

/* Update routine 6: grows scaleX up to 1.0, then scaleY. */
void func_150B71A8(Sprite2D *sprite) {
    if (sprite->params.scaleX != 0x1000) {
        sprite->params.scaleX += D_800BE9E4 * 0x100;
        if (sprite->params.scaleX > 0x1000) {
            sprite->params.scaleX = 0x1000;
        }
    } else if (sprite->params.scaleY != 0x1000) {
        sprite->params.scaleY += D_800BE9E4 * 0x100;
        if (sprite->params.scaleY > 0x1000) {
            sprite->params.scaleY = 0x1000;
        }
    }
}

extern u8 D_80091924;

/* Spawns four D_80091924 sprites (update routine 7) that converge on (arg0, arg1) from above, below,
 * left and right over `duration` frames. */
void func_150B7220(s32 arg0, s32 arg1, s32 duration, s32 arg3) {
    Sprite2DParams desc;
    s32 r;
    s32 x;
    s32 y;

    // matching: copies of the args give the 0x80 frame and IDO's register choice
    x = arg0;
    y = arg1;
    desc.texture = &D_80091924;
    desc.data.words[0] = (x << 16) | (y & 0xFFFF);
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    desc.data.words[1] = duration;
    desc.timer = duration;
    desc.frameSpeed = 0;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.width = 16;
    desc.height = 16;
    desc.behavior = 7;
    desc.frameCount = 2;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    if (y) {} // matching: steers IDO's s1/s2 choice for y and r
    desc.alpha = 0xFF;
    desc.renderMode = 7;
    desc.unk37 = 0x11;
    if (x < (D_800BE638[0] >> 1)) {
        do { // matching: the loop's -g3 markers steer IDO's register choice for r
            r = D_800BE638[0] - x;
        } while (0);
    } else {
        r = x;
    }

    desc.data.words[2] = (x << 16) | ((y - r) & 0xFFFF);
    desc.flags = 0;
    desc.frame = 0;
    desc.x = x;
    desc.y = y - r;
    func_15169900(&desc, arg3);

    desc.data.words[2] = (x << 16) | ((y + r) & 0xFFFF);
    desc.flags = 8;
    desc.y = y + r;
    func_15169900(&desc, arg3);

    desc.x = x - r;
    desc.data.words[2] = ((x - r) << 16) | (y & 0xFFFF);
    desc.flags = 4;
    desc.frame = 0x100;
    desc.y = y;
    func_15169900(&desc, arg3);

    desc.data.words[2] = ((x + r) << 16) | (y & 0xFFFF);
    desc.flags = 0;
    desc.x = x + r;
    func_15169900(&desc, arg3);
}

/* Update routine 7: moves the sprite in a straight line from its start point to its end point as its
 * timer runs down from the duration to 0. */
void func_150B73F0(Sprite2D *sprite) {
    s32 endX = sprite->params.data.move.endX;
    s32 endY = sprite->params.data.move.endY;
    s32 startX = sprite->params.data.move.startX;
    s32 startY = sprite->params.data.move.startY;
    s32 duration = sprite->params.data.move.duration;
    s32 timer = sprite->params.timer;
    s32 t = (timer << 16) / duration; /* time left as a 16.16 fraction */

    sprite->params.x = (((startX - endX) * t) >> 16) + endX;
    sprite->params.y = (((startY - endY) * t) >> 16) + endY;
}


extern u8 D_800918DC;
extern u8 D_800918E8;
extern u8 D_800918F4;
extern u8 D_80091900;

/* Replaces sprite slot 0 with a D_800918DC sprite at (60, 60) that grows in (update routine 6). */
void func_150B7484(void) {
    Sprite2DParams desc;

    desc.frameCount = 1;
    desc.width = 88;
    desc.x = 60.0f;
    desc.y = 60.0f;
    desc.timer = 0;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.height = 88;
    desc.behavior = 6;
    desc.texture = &D_800918DC;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.flags = 0;
    desc.renderMode = 7;
    desc.unk37 = 0x15;
    desc.cycleType = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (D_800D9898[0] != NULL) {
        func_1516972C(&D_800D9898[0]->header);
    }
    D_800D9898[0] = func_15169968(&desc);
    D_800D9890 = 3;
}

/* Frees sprite slot 0 and puts a static 15-frame D_800918DC sprite at (60, 60) in slot 3. */
void func_150B7560(void) {
    Sprite2DParams desc;

    if (D_800D9898[0] != NULL) {
        func_1516972C(&D_800D9898[0]->header);
        D_800D9898[0] = NULL;
    }
    desc.frameSpeed = 0x4D;
    desc.frameCount = 15;
    desc.timer = 0;
    desc.frame = 0;
    desc.width = 88;
    desc.height = 88;
    desc.behavior = 0;
    desc.texture = &D_800918DC;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.renderMode = 7;
    desc.unk37 = 0x11;
    desc.cycleType = 1;
    desc.flags = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    desc.x = 60.0f;
    desc.y = 60.0f;
    if (D_800D9898[3] != NULL) {
        func_1516972C(&D_800D9898[3]->header);
    }
    D_800D9898[3] = func_15169968(&desc);
    D_800D9890 = 3;
}

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
    Sprite2DParams desc;

    desc.x = x;
    desc.y = 170.0f;
    desc.frameCount = 1;
    desc.width = 88;
    desc.height = 64;
    desc.behavior = 6;
    desc.timer = 0;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.texture = &D_800918E8;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.flags = 0;
    desc.renderMode = 7;
    desc.unk37 = 0x15;
    desc.cycleType = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (D_800D9898[slot] != NULL) {
        func_1516972C(&D_800D9898[slot]->header);
    }
    D_800D9898[slot] = func_15169968(&desc);
}

/* Frees sprite slot 1, then puts the D_800918F4/D_80091900 sprite (update routine 4) in slot 4
 * and a static D_800918E8 sprite in slot 8, both at (60, 170). */
void func_150B77A8(void) {
    Sprite2DParams desc;

    if (D_800D9898[1] != NULL) {
        func_1516972C(&D_800D9898[1]->header);
        D_800D9898[1] = NULL;
    }

    desc.x = 60.0f;
    desc.y = 170.0f;
    desc.width = 64;
    desc.height = 64;
    desc.behavior = 4;
    desc.texture = &D_800918F4;
    desc.tile1Texture = &D_80091900;
    desc.timer = 0;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.frameCount = 0;
    desc.sOffset = -0x800;
    desc.extraWidth = 9;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0xFF;
    desc.renderMode = 0xD;
    desc.unk37 = 0x12;
    desc.cycleType = 2;
    desc.flags = 3;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (D_800D9898[4] != NULL) {
        func_1516972C(&D_800D9898[4]->header);
    }
    D_800D9898[4] = func_15169968(&desc);

    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.width = 88;
    desc.height = 64;
    desc.behavior = 0;
    desc.renderMode = 7;
    desc.unk37 = 0x11;
    desc.texture = &D_800918E8;
    desc.cycleType = 1;
    if (D_800D9898[8] != NULL) {
        func_1516972C(&D_800D9898[8]->header);
    }
    D_800D9898[8] = func_15169968(&desc);
    D_800D9890 = 3;
}

extern u8 D_8009190C;

/* Frees sprite slot 2, then puts three D_8009190C frames that scroll into place at y = 150, 170
 * and 190 (update routine 5) in slots 5-7, and a static D_800918E8 sprite at (230, 170) in slot 9. */
void func_150B791C(void) {
    Sprite2DParams desc;

    if (D_800D9898[2] != NULL) {
        func_1516972C(&D_800D9898[2]->header);
        D_800D9898[2] = NULL;
    }
    desc.x = 230.0f;
    desc.frameCount = 3;
    desc.width = 64;
    desc.height = 16;
    desc.behavior = 5;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.timer = 0;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.texture = &D_8009190C;
    desc.tile1Texture = NULL;
    desc.data.scroll.frame = 0;
    desc.y = 150.0f;
    desc.data.scroll.y = desc.y;
    desc.data.scroll.stage = 0;
    desc.red = 0xFF;
    desc.green = 0;
    desc.blue = 0;
    desc.alpha = 0x80;
    desc.renderMode = 7;
    desc.unk37 = 0x11;
    desc.cycleType = 1;
    desc.flags = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (D_800D9898[5] != NULL) {
        func_1516972C(&D_800D9898[5]->header);
    }
    D_800D9898[5] = func_15169968(&desc);
    desc.frame = 0x100;
    desc.data.scroll.frame = 0x100;
    desc.y = 170.0f;
    desc.data.scroll.y = desc.y;
    desc.alpha = 0xFF;
    if (D_800D9898[6] != NULL) {
        func_1516972C(&D_800D9898[5]->header); /* sic: frees slot 5, not 6 */
    }
    D_800D9898[6] = func_15169968(&desc);
    desc.frame = 0x200;
    desc.data.scroll.frame = 0x200;
    desc.y = 190.0f;
    desc.data.scroll.y = desc.y;
    desc.alpha = 0x80;
    if (D_800D9898[7] != NULL) {
        func_1516972C(&D_800D9898[5]->header); /* sic */
    }
    D_800D9898[7] = func_15169968(&desc);
    desc.timer = 0;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.frameCount = 0;
    desc.width = 88;
    desc.height = 64;
    desc.alpha = 0xFF;
    desc.behavior = 0;
    desc.texture = &D_800918E8;
    desc.x = 230.0f;
    desc.y = 170.0f;
    if (D_800D9898[9] != NULL) {
        func_1516972C(&D_800D9898[9]->header);
    }
    D_800D9898[9] = func_15169968(&desc);
    D_800D9890 = 3;
}
