#include <ultra64.h>
#include "structs.h"

// This file's .bss (0x800D9890-0x800D98BF), in the original order. IDO places the variables that
// a header declares after the ones the .c file declares first, so these are declared only here, before
// the subsystem headers: the order also depends on how many types and variables come before them (see init/Sched/SchedInit/schedinit.c).
u8 gSpriteQueuedCmd; // queued command for sprite_runQueuedCommand
EffectHeader *gSpriteTintEffect;
// matching: defined in-file so as1 shares one lui across the unrolled stores in sprite_resetSlots
static struct Sprite2D *gSpriteSlots[10];

#include "conker/effect.h"
#include "conker/game.h"
#include "conker/screen.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/view.h"


/* Runs the command queued in gSpriteQueuedCmd (each handler resets it to 3). */
void sprite_runQueuedCommand(s32 unused) {
    switch (gSpriteQueuedCmd) {
    case 1:
        sprite_resetSlots();
        return;
    case 2:
        sprite_settleFrameScrolls();
        return;
    case 4:
        sprite_freeAll();
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

/* Recreates the screen_spawnTintEffect effect and clears every sprite slot (without freeing the sprites). */
void sprite_resetSlots(void) {
    s32 i;

    if (gSpriteTintEffect != NULL) {
        effect_destroyObject(gSpriteTintEffect);
    }
    gSpriteTintEffect = screen_spawnTintEffect(0, 0, 0xFF, 0, 0, gLastPlayerIndex, 0, 0);
    gSpriteQueuedCmd = 3;
    for (i = 0; i < 10; i++) {
        gSpriteSlots[i] = NULL;
    }
}

/* Lets the three scrolling sprites in slots 5-7 stop once they reach their target frame and y. */
void sprite_settleFrameScrolls(void) {
    s32 i;
    Sprite2D *sprite;

    for (i = 0; i < 3; i++) {
        sprite = gSpriteSlots[i + 5];
        if (sprite != NULL) {
            sprite->params.data.scroll.stage = 1;
        }
    }
    gSpriteQueuedCmd = 3;
}

/* Frees the screen_spawnTintEffect effect and every sprite. */
void sprite_freeAll(void) {
    s32 i;

    if (gSpriteTintEffect != NULL) {
        effect_destroyObject(gSpriteTintEffect);
        gSpriteTintEffect = NULL;
    }
    for (i = 0; i < 10; i++) {
        if (gSpriteSlots[i] != NULL) {
            effect_destroyObject(&gSpriteSlots[i]->header);
            gSpriteSlots[i] = NULL;
        }
    }
    gSpriteQueuedCmd = 3;
}

/* Update routine 4: advances unk34 by 0x30 per frame, wrapping from above 0x800 back to -0xC00. */
void sprite_updateTexScroll(Sprite2D *sprite) {
    sprite->params.sOffset += gFrameTicks * 0x30;
    if (sprite->params.sOffset > 0x800) {
        sprite->params.sOffset = -0xC00;
    }
}


/* Update routine 5: scrolls the sprite up through its animation frames, fading it with its distance
 * from y = 170, until it reaches its target frame and y. Once stopped at y = 170 it blinks, and after
 * the third blink it turns into the D_80091918 sprite run by update routine 8 (sprite_updateSlideBlink). */
void sprite_updateFrameScroll(Sprite2D *sprite) {
    u32 three;
    f32 dist;
    s32 alpha;

    dist = 0.0f; // matching: a dead first def makes IDO give dist the first FP register
    three = 3;   // matching: one register for the multu step and the stage compare
    if (sprite->params.data.scroll.stage == 0 || sprite->params.frame != sprite->params.data.scroll.frame ||
        (dist = (sprite->params.y < sprite->params.data.scroll.y)
                    ? -(sprite->params.y - sprite->params.data.scroll.y) : (sprite->params.y - sprite->params.data.scroll.y),
         (s32)(gFrameTicks * three) < dist)) {
        sprite->params.y -= (s32)(gFrameTicks * three);
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
        alpha += gFrameTicks * 8;
        if (alpha >= 0x100) {
            sprite->params.data.scroll.stage++;
            if (sprite->params.data.scroll.stage == three) {
                sprite->params.texture = &D_80091918;
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
void sprite_updateSlideBlink(Sprite2D *sprite) {
    s32 elapsed;
    s32 alpha;

    elapsed = sprite->params.data.slide.elapsed;
    if (elapsed == 30) {
        alpha = sprite->params.alpha;
        alpha += gFrameTicks * 8;
        if (alpha >= 0x100) {
            alpha = 0;
        }
        sprite->params.alpha = alpha;
    }
    elapsed += gFrameTicks * 2;
    if (elapsed > 30) {
        elapsed = 30;
    }
    sprite->params.data.slide.elapsed = elapsed;
    sprite->params.x = elapsed * -84 / 30 + 230;
    sprite->params.y = elapsed * -50 / 30 + 170;
}

/* Update routine 6: grows scaleX up to 1.0, then scaleY. */
void sprite_updateGrowIn(Sprite2D *sprite) {
    if (sprite->params.scaleX != 0x1000) {
        sprite->params.scaleX += gFrameTicks * 0x100;
        if (sprite->params.scaleX > 0x1000) {
            sprite->params.scaleX = 0x1000;
        }
    } else if (sprite->params.scaleY != 0x1000) {
        sprite->params.scaleY += gFrameTicks * 0x100;
        if (sprite->params.scaleY > 0x1000) {
            sprite->params.scaleY = 0x1000;
        }
    }
}


/* Spawns four D_80091924 sprites (update routine 7) that converge on (targetX, targetY) from above, below,
 * left and right over `duration` frames. */
void sprite_spawnConverging(s32 targetX, s32 targetY, s32 duration, s32 owner) {
    Sprite2DParams desc;
    s32 r;
    s32 x;
    s32 y;

    // matching: copies of the args give the 0x80 frame and IDO's register choice
    x = targetX;
    y = targetY;
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
    desc.combiner = 0x11;
    if (x < (gLayoutViewWidths[0] >> 1)) {
        do { // matching: the loop's -g3 markers steer IDO's register choice for r
            r = gLayoutViewWidths[0] - x;
        } while (0);
    } else {
        r = x;
    }

    desc.data.words[2] = (x << 16) | ((y - r) & 0xFFFF);
    desc.flags = 0;
    desc.frame = 0;
    desc.x = x;
    desc.y = y - r;
    sprite_newSprite2D(&desc, owner);

    desc.data.words[2] = (x << 16) | ((y + r) & 0xFFFF);
    desc.flags = 8;
    desc.y = y + r;
    sprite_newSprite2D(&desc, owner);

    desc.x = x - r;
    desc.data.words[2] = ((x - r) << 16) | (y & 0xFFFF);
    desc.flags = 4;
    desc.frame = 0x100;
    desc.y = y;
    sprite_newSprite2D(&desc, owner);

    desc.data.words[2] = ((x + r) << 16) | (y & 0xFFFF);
    desc.flags = 0;
    desc.x = x + r;
    sprite_newSprite2D(&desc, owner);
}

/* Update routine 7: moves the sprite in a straight line from its start point to its end point as its
 * timer runs down from the duration to 0. */
void sprite_updateLinearMove(Sprite2D *sprite) {
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
    desc.combiner = 0x15;
    desc.cycleType = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (gSpriteSlots[0] != NULL) {
        effect_destroyObject(&gSpriteSlots[0]->header);
    }
    gSpriteSlots[0] = sprite_newSprite2DDefault(&desc);
    gSpriteQueuedCmd = 3;
}

/* Frees sprite slot 0 and puts a static 15-frame D_800918DC sprite at (60, 60) in slot 3. */
void func_150B7560(void) {
    Sprite2DParams desc;

    if (gSpriteSlots[0] != NULL) {
        effect_destroyObject(&gSpriteSlots[0]->header);
        gSpriteSlots[0] = NULL;
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
    desc.combiner = 0x11;
    desc.cycleType = 1;
    desc.flags = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    desc.x = 60.0f;
    desc.y = 60.0f;
    if (gSpriteSlots[3] != NULL) {
        effect_destroyObject(&gSpriteSlots[3]->header);
    }
    gSpriteSlots[3] = sprite_newSprite2DDefault(&desc);
    gSpriteQueuedCmd = 3;
}

/* Spawns the growing D_800918E8 sprite at x = 60 in slot 1. */
void func_150B765C(void) {
    sprite_spawnGrowing(0x3C, 1);
    gSpriteQueuedCmd = 3;
}

/* Spawns the growing D_800918E8 sprite at x = 230 in slot 2. */
void func_150B768C(void) {
    sprite_spawnGrowing(0xE6, 2);
    gSpriteQueuedCmd = 3;
}

/* Replaces sprite `slot` with a D_800918E8 sprite at (x, 170) that grows in (update routine 6). */
void sprite_spawnGrowing(s32 x, s32 slot) {
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
    desc.combiner = 0x15;
    desc.cycleType = 1;
    desc.scaleX = 0x51;
    desc.scaleY = 0x51;
    if (gSpriteSlots[slot] != NULL) {
        effect_destroyObject(&gSpriteSlots[slot]->header);
    }
    gSpriteSlots[slot] = sprite_newSprite2DDefault(&desc);
}

/* Frees sprite slot 1, then puts the D_800918F4/D_80091900 sprite (update routine 4) in slot 4
 * and a static D_800918E8 sprite in slot 8, both at (60, 170). */
void func_150B77A8(void) {
    Sprite2DParams desc;

    if (gSpriteSlots[1] != NULL) {
        effect_destroyObject(&gSpriteSlots[1]->header);
        gSpriteSlots[1] = NULL;
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
    desc.combiner = 0x12;
    desc.cycleType = 2;
    desc.flags = 3;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (gSpriteSlots[4] != NULL) {
        effect_destroyObject(&gSpriteSlots[4]->header);
    }
    gSpriteSlots[4] = sprite_newSprite2DDefault(&desc);

    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.width = 88;
    desc.height = 64;
    desc.behavior = 0;
    desc.renderMode = 7;
    desc.combiner = 0x11;
    desc.texture = &D_800918E8;
    desc.cycleType = 1;
    if (gSpriteSlots[8] != NULL) {
        effect_destroyObject(&gSpriteSlots[8]->header);
    }
    gSpriteSlots[8] = sprite_newSprite2DDefault(&desc);
    gSpriteQueuedCmd = 3;
}


/* Frees sprite slot 2, then puts three D_8009190C frames that scroll into place at y = 150, 170
 * and 190 (update routine 5) in slots 5-7, and a static D_800918E8 sprite at (230, 170) in slot 9. */
void func_150B791C(void) {
    Sprite2DParams desc;

    if (gSpriteSlots[2] != NULL) {
        effect_destroyObject(&gSpriteSlots[2]->header);
        gSpriteSlots[2] = NULL;
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
    desc.combiner = 0x11;
    desc.cycleType = 1;
    desc.flags = 0;
    desc.scaleX = 0x1000;
    desc.scaleY = 0x1000;
    if (gSpriteSlots[5] != NULL) {
        effect_destroyObject(&gSpriteSlots[5]->header);
    }
    gSpriteSlots[5] = sprite_newSprite2DDefault(&desc);
    desc.frame = 0x100;
    desc.data.scroll.frame = 0x100;
    desc.y = 170.0f;
    desc.data.scroll.y = desc.y;
    desc.alpha = 0xFF;
    if (gSpriteSlots[6] != NULL) {
        effect_destroyObject(&gSpriteSlots[5]->header); /* sic: frees slot 5, not 6 */
    }
    gSpriteSlots[6] = sprite_newSprite2DDefault(&desc);
    desc.frame = 0x200;
    desc.data.scroll.frame = 0x200;
    desc.y = 190.0f;
    desc.data.scroll.y = desc.y;
    desc.alpha = 0x80;
    if (gSpriteSlots[7] != NULL) {
        effect_destroyObject(&gSpriteSlots[5]->header); /* sic */
    }
    gSpriteSlots[7] = sprite_newSprite2DDefault(&desc);
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
    if (gSpriteSlots[9] != NULL) {
        effect_destroyObject(&gSpriteSlots[9]->header);
    }
    gSpriteSlots[9] = sprite_newSprite2DDefault(&desc);
    gSpriteQueuedCmd = 3;
}
