#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define OBJTYPE_TEXTBOX 0x51

/* TextBoxConfig.flags */
#define TEXTBOX_BUBBLES         0x2     /* body drawn with D_8009056C plus three fading D_8009058C circles */
#define TEXTBOX_DARK            0x4     /* translucent black body, no drop shadow */
#define TEXTBOX_ALT_TAIL        0x8     /* tail drawn with D_8009059C */
#define TEXTBOX_INPUT           0x10    /* A/B buttons advance or close the box */
#define TEXTBOX_INPUT_CHECK     0x20    /* ... but only while func_151F2CDC() returns 0 */
#define TEXTBOX_NO_B_SKIP       0x40    /* B does not skip to the end */
#define TEXTBOX_B_RESTARTS      0x80    /* B on the last page restarts from the first */
#define TEXTBOX_CLOSE           0x100   /* request: close after the current page */
#define TEXTBOX_HOLD            0x200   /* keep the current page open */
#define TEXTBOX_SKIP_ANIM       0x400   /* skip every timed transition */
#define TEXTBOX_CB_CLOSED       0x1000  /* set while the callback runs: box closed */
#define TEXTBOX_CB_PAGE_ENDED   0x2000  /* set while the callback runs: a page was left */
#define TEXTBOX_CB_PAGE_SHOWN   0x4000  /* set while the callback runs: a page is fully shown */
#define TEXTBOX_SCREEN_POS      0x8000  /* positioned by screen percentages instead of a world point */
#define TEXTBOX_CLAMP           0x10000 /* keep a screen-positioned box inside the view bounds */
#define TEXTBOX_CENTERED        0x20000 /* centre each text line */
#define TEXTBOX_WHITE_TEXT      0x40000

/* TextBox.state */
#define TEXTBOX_STATE_INIT      0x1
#define TEXTBOX_STATE_GROW      0x2
#define TEXTBOX_STATE_SETTLE    0x4
#define TEXTBOX_STATE_BUBBLES   0x8
#define TEXTBOX_STATE_TEXT_IN   0x10
#define TEXTBOX_STATE_SHOW      0x20
#define TEXTBOX_STATE_TEXT_OUT  0x40
#define TEXTBOX_STATE_SHRINK    0x80


/* Per-frame update of a text box: runs the open / page / close state machine, then re-places it. */
void text_updateBox(TextBox *box) {
    s32 unused; // matching: unused local, needed for the original stack frame
    Camera *player;
    s32 width;
    s32 height;
    s32 advance;
    s32 skip;
    s32 advancePage;

    player = camera_global_Cameras;
    skip = box->config->flags & TEXTBOX_SKIP_ANIM;
    if (box->config->unk2B != 0 && gCutsceneStates[0] != 1) {
        box->config->numPages = 0;
        effect_destroyObject(box);
        return;
    }
    box->x = box->anchorX;
    box->y = box->anchorY;
    switch (box->state) {
    case TEXTBOX_STATE_INIT:
        box->timer = 0;
        box->fadeTimer = 0;
        if (skip) {
            box->scaleSpeed = 1.0f;
        } else {
            box->scaleSpeed = 0.333333343f;
        }
        if (box->config->flags & TEXTBOX_BUBBLES) {
            box->bubbleAlpha[2] = 0;
            box->bubbleAlpha[1] = 0;
            box->bubbleAlpha[0] = 0;
            box->bubbleIndex = 2;
            box->timer = 4;
            box->fadeTimer = 10;
            box->state = TEXTBOX_STATE_BUBBLES;
            box->scale = 1.0f;
            text_placeBox(box);
            box->visible = TRUE;
            box->bubbleX = box->anchorX;
        } else {
            box->state = TEXTBOX_STATE_GROW;
        }
        box->halfTextWidth = 1.0f;
        box->halfTextHeight = 1.0f;
        box->textAlpha = 0;
        break;
    case TEXTBOX_STATE_GROW:
        text_measureString(box->text, &width, &height, &advance);
        box->halfTextWidth = width >> 1;
        box->halfTextHeight = height >> 1;
        box->scale += box->scaleSpeed * gFrameDelta;
        if (skip) {
            box->fadeTimer = 0;
        }
        if (box->scale > 0.75f && box->fadeTimer == 0) {
            box->fadeTimer = 10;
        }
        if (box->fadeTimer != 0) {
            box->fadeTimer -= gFrameSteps;
            box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
            box->textAlpha = (10.0f - box->fadeTimer) / 10.0f * 255.0f;
        }
        if (box->scale > 1.0f) {
            box->scale = 1.0f;
            box->state = TEXTBOX_STATE_SETTLE;
            box->timer = 180;
        }
        box->visible = TRUE;
        break;
    case TEXTBOX_STATE_SETTLE:
        if (skip) {
            box->timer = 0;
        }
        box->timer -= gFrameSteps * 45;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        box->fadeTimer -= gFrameSteps;
        box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
        box->textAlpha = (10.0f - box->fadeTimer) / 10.0f * 255.0f;
        if (box->timer == 0) {
            box->state = TEXTBOX_STATE_TEXT_IN;
        }
        break;
    case TEXTBOX_STATE_BUBBLES:
        /* fade the circles in one at a time, from the anchor outwards */
        box->timer -= gFrameSteps;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        box->bubbleAlpha[box->bubbleIndex] = (4 - (f32)box->timer) / 4 * 170.0f;
        if (box->timer == 0) {
            if (--box->bubbleIndex > 100) {
                box->state = TEXTBOX_STATE_TEXT_IN;
                box->fadeTimer = 10;
            } else {
                box->timer = 4;
            }
        }
        break;
    case TEXTBOX_STATE_TEXT_OUT:
        box->timer -= gFrameSteps;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        if (skip) {
            box->timer = 0;
        }
        box->textAlpha = box->timer / 10.0f * 255.0f;
        if (box->timer == 0) {
            box->page++;
            if (box->page < box->config->numPages) {
                box->text = text_getPage(box->config, box->page, box->source);
                box->fadeTimer = 10;
                box->state = TEXTBOX_STATE_TEXT_IN;
                box->config->flags |= TEXTBOX_CB_PAGE_ENDED;
                if (box->config->callback != NULL) {
                    box->config->callback(box->config, box->page - 1, box->state);
                }
                box->config->flags &= ~TEXTBOX_CB_PAGE_ENDED;
            } else {
                box->fadeTimer = 5;
                box->state = TEXTBOX_STATE_SHRINK;
                box->scale = 1.0f;
                box->scaleSpeed = 0.25f;
            }
        }
        if (!skip) {
            break;
        }
        /* fallthrough */
    case TEXTBOX_STATE_TEXT_IN:
        text_measureString(box->text, &width, &height, &advance);
        box->halfTextWidth = width >> 1;
        box->halfTextHeight = height >> 1;
        box->fadeTimer -= gFrameSteps;
        box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
        if (skip) {
            box->fadeTimer = 0;
        }
        box->textAlpha = (10.0f - box->fadeTimer) / 10.0f * 255.0f;
        if (box->fadeTimer == 0) {
            box->timer = box->config->duration;
            box->state = TEXTBOX_STATE_SHOW;
            box->config->flags |= TEXTBOX_CB_PAGE_SHOWN;
        }
        if (!skip) {
            break;
        }
        /* fallthrough */
    case TEXTBOX_STATE_SHOW: {
        s32 bits;

        advancePage = FALSE;
        // matching: the (u16) casts on these two flag tests each take one ugen temp register
        if ((u16)(box->config->flags & TEXTBOX_CB_PAGE_SHOWN)) {
            if (box->config->callback != NULL) {
                box->config->callback(box->config, box->page, box->state);
            }
            box->config->flags &= ~TEXTBOX_CB_PAGE_SHOWN;
        }
        box->timer -= gFrameSteps;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        if ((u16)(box->config->flags & TEXTBOX_INPUT) && box->noInput == 0) {
            if (!(box->config->flags & TEXTBOX_INPUT_CHECK) || func_151F2CDC() == 0) {
                if ((box->config->flags & TEXTBOX_B_RESTARTS) && box->page + 1 == box->config->numPages) {
                    if (gButtonsPressed[0] & B_BUTTON) {
                        box->page = -1;
                        advancePage = TRUE;
                        gContextSoundNext = 0;
                    } else if (gButtonsPressed[0] & A_BUTTON) {
                        advancePage = TRUE;
                    }
                } else if (gButtonsPressed[0] & A_BUTTON) {
                    advancePage = TRUE;
                } else if (!(box->config->flags & TEXTBOX_NO_B_SKIP) && (gButtonsPressed[0] & B_BUTTON)) {
                    advancePage = TRUE;
                    box->page = box->config->numPages;
                }
            }
        }
        bits = box->config->flags & 0xF00;
        if (bits & TEXTBOX_CLOSE) {
            box->config->flags &= ~TEXTBOX_CLOSE;
            advancePage = TRUE;
            bits = box->config->flags & 0xF00;
        }
        if (!(bits & TEXTBOX_HOLD) && advancePage) {
            box->timer = 10;
            box->state = TEXTBOX_STATE_TEXT_OUT;
        }
    }
        break;
    case TEXTBOX_STATE_SHRINK:
        if (box->config->flags & TEXTBOX_BUBBLES) {
            box->fadeTimer -= gFrameSteps;
            box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
            box->bubbleAlpha[0] = box->fadeTimer / 5.0f * 85.0f;
            box->bubbleAlpha[1] = box->bubbleAlpha[0];
            box->bubbleAlpha[2] = box->bubbleAlpha[0];
        } else {
            box->scale -= box->scaleSpeed * gFrameDelta;
        }
        if (box->scale < 0.0f || box->fadeTimer == 0) {
            box->scale = 0.0f;
            box->state = 0;
            box->page = 0;
            box->config->flags |= TEXTBOX_CB_CLOSED;
            if (box->config->callback != NULL) {
                box->config->callback(box->config, box->page, box->state);
            }
            box->config->flags &= ~TEXTBOX_CB_CLOSED;
            movement_obj[box->config->objIndex].unk1E3 = 0;
            player->cmdState.unk0 = 0;
            box->config->numPages = 0;
            effect_destroyObject(box);
        }
        break;
    }
    text_placeBox(box);
}

/* Sprite parameters passed to sprite_loadDescFrame / sprite_drawScreen. */

#define SPRITE_FLIP_X 1 /* set while the body is left of the anchor */
#define SPRITE_FLIP_Y 2 /* set while the body is below the anchor */

/* Text box textures: D_8009055C body, D_8009056C bubble body, D_8009057C / D_8009059C tails,
 * D_8009058C bubble circle. */
/* Render mode pairs: cycle 1 and cycle 2 blender settings. */
extern struct {
    u32 cycle1;
    u32 cycle2;
} gEffectRenderModes[];

/* Draws a text box: shadow, body, tail (or thought-bubble circles), then the text via text_drawBoxText.
 * Returns gfx unchanged if the display list would overflow. */
Gfx *text_drawBox(Gfx *gfx, TextBox *box, s32 arg2) {
    SpriteParams sprite;
    s32 pad; // matching: unused stack slot
    f32 xoff;
    f32 yoff;
    s32 dark;
    s32 fade;
    s32 overflow;
    Gfx *start;
    f32 texW; // matching: dead copies; converting the texture size first orders the FP registers
    f32 texH;

    start = gfx;
    if (box->config->flags & TEXTBOX_DARK) {
        dark = TRUE;
    } else {
        dark = FALSE;
    }
    if (!box->visible) {
        return gfx;
    }
    if (D_800E0BDC == 0) {
        return gfx;
    }
    gSpritePrimR = gSpritePrimG = gSpritePrimB = 0;
    fade = D_800E0BDC + 1;
    sprite.flip = 0;
    sprite.alpha = (64 * fade) >> 8;
    texW = D_8009055C.width;
    texH = D_8009055C.height;
    sprite.scaleX = (2.0f * box->width + 1.0f) / D_8009055C.width * 2355.2f * box->scale;
    sprite.scaleY = (box->height + 1.0f) / D_8009055C.height * 4259.84f * box->scale;
    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f * box->scale;
    yoff = box->height / D_8009055C.height * D_8009057C.height * 0.5f * box->scale;
    box->bodyX = cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f * box->scale + box->x - xoff;
    box->bodyY = sinf(box->angle + 3.1415927f) * (box->height * 0.5f) * 1.04f * box->scale + box->y - yoff;
    /* shadow pass */
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0,
                      TEXEL0, 0);
    gDPSetOtherMode(gfx++, G_TF_BILERP | G_TC_FILT | 0xF, gEffectRenderModes[25].cycle1 | gEffectRenderModes[25].cycle2 | G_ZS_PRIM);
    if (box->config->flags & TEXTBOX_BUBBLES) {
        sprite.alpha = box->bubbleAlpha[0] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = sprite_loadDescFrame(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = sprite_loadDescFrame(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    if (!dark) {
        gfx = sprite_drawScreen(gfx, &sprite, box->bodyX + 5.0f, box->bodyY + -5.0f);
    }
    if (box->angle > 0.0f && box->angle < 3.1415927f) {
        sprite.flip &= ~SPRITE_FLIP_Y;
    } else {
        sprite.flip |= SPRITE_FLIP_Y;
    }
    if (box->angle > 1.5707964f && box->angle < 4.712389f) {
        sprite.flip |= SPRITE_FLIP_X;
    } else {
        sprite.flip &= ~SPRITE_FLIP_X;
    }
    overflow = (gfx - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    if (box->config->flags & TEXTBOX_BUBBLES) {
        if (box->state == TEXTBOX_STATE_INIT) {
            box->bubbleX = box->anchorX;
        }
        gfx = sprite_loadDescFrame(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[1] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        if (box->angle > 1.5707964f) {
            box->bubbleX += (box->x + xoff - box->bubbleX) * 0.3f;
            gfx = sprite_drawScreen(gfx, &sprite, box->bubbleX, box->y - yoff);
        } else {
            box->bubbleX += (box->x - xoff - box->bubbleX) * 0.3f;
            gfx = sprite_drawScreen(gfx, &sprite, box->bubbleX, box->y - yoff);
        }
        sprite.scaleX /= 2;
        sprite.scaleY /= 2;
        sprite.alpha = box->bubbleAlpha[2] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = sprite_loadDescFrame(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = sprite_drawScreen(gfx, &sprite, box->x - 2.5f, box->y + -2.5f);
        } else {
            gfx = sprite_drawScreen(gfx, &sprite, box->x + 2.5f, box->y + -2.5f);
        }
    } else {
        gfx = sprite_loadDescFrame(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (!dark) {
            if (box->angle > 1.5707964f) {
                gfx = sprite_drawScreen(gfx, &sprite, box->x + xoff - 2.5f, box->y - yoff + -2.5f);
            } else {
                gfx = sprite_drawScreen(gfx, &sprite, box->x - xoff + 2.5f, box->y - yoff + -2.5f);
            }
        }
    }
    overflow = (gfx - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    /* the box itself, tinted with the config's colour */
    if (dark) {
        gSpritePrimR = gSpritePrimG = gSpritePrimB = 0;
    } else {
        gSpritePrimR = box->config->r;
        gSpritePrimG = box->config->g;
        gSpritePrimB = box->config->b;
    }
    sprite.flip = 0;
    if (dark) {
        sprite.alpha = 0x55;
    } else {
        sprite.alpha = 0xFF;
    }
    sprite.alpha = (sprite.alpha * fade) >> 8;
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0,
                      TEXEL0, 0);
    gDPSetOtherMode(gfx++, G_TF_BILERP | G_TC_FILT | 0xF, gEffectRenderModes[25].cycle1 | gEffectRenderModes[25].cycle2 | G_ZS_PRIM);
    if (box->angle > 0.0f && box->angle < 3.1415927f) {
        sprite.flip &= ~SPRITE_FLIP_Y;
    } else {
        sprite.flip |= SPRITE_FLIP_Y;
    }
    if (box->angle > 1.5707964f && box->angle < 4.712389f) {
        sprite.flip |= SPRITE_FLIP_X;
    } else {
        sprite.flip &= ~SPRITE_FLIP_X;
    }
    sprite.scaleX = box->width / D_8009055C.width * 2355.2f * box->scale;
    sprite.scaleY = box->height / D_8009055C.height * 4259.84f * box->scale;
    if (box->config->flags & TEXTBOX_BUBBLES) {
        gfx = sprite_loadDescFrame(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[1];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = sprite_drawScreen(gfx, &sprite, box->bubbleX, box->y - yoff);
        sprite.scaleX /= 2;
        sprite.scaleY /= 2;
        gfx = sprite_loadDescFrame(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[2];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        if (box->angle > 1.5707964f) {
            gfx = sprite_drawScreen(gfx, &sprite, box->x, box->y);
        } else {
            gfx = sprite_drawScreen(gfx, &sprite, box->x, box->y);
        }
    } else if (box->config->flags & TEXTBOX_ALT_TAIL) {
        gfx = sprite_loadDescFrame(gfx, &D_8009059C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = sprite_drawScreen(gfx, &sprite, box->x + xoff, box->y - yoff);
        } else {
            gfx = sprite_drawScreen(gfx, &sprite, box->x - xoff, box->y - yoff);
        }
    } else {
        gfx = sprite_loadDescFrame(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = sprite_drawScreen(gfx, &sprite, box->x + xoff, box->y - yoff);
        } else {
            gfx = sprite_drawScreen(gfx, &sprite, box->x - xoff, box->y - yoff);
        }
    }
    sprite.scaleX = 2.0f * box->width / D_8009055C.width * 2355.2f * box->scale;
    sprite.scaleY = box->height / D_8009055C.height * 4259.84f * box->scale;
    if (box->config->flags & TEXTBOX_BUBBLES) {
        sprite.alpha = box->bubbleAlpha[0];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = sprite_loadDescFrame(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = sprite_loadDescFrame(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    gfx = sprite_drawScreen(gfx, &sprite, (s32)box->bodyX, (s32)box->bodyY);
    overflow = (gfx - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    return text_drawBoxText(gfx, box, 0, 0, 0, (box->textAlpha * fade) >> 8);
}

/* Returns the start of page `index` of a config's text: pages are separated by runs of NULs. */
u8 *text_getPage(TextBoxConfig *config, s32 index, s16 source) {
    u8 *str;
    u8 *uninit;
    s32 i;

    switch (source) {
    case 0:
        str = D_800DD254[config->text].data;
        break;
    case 1:
        str = D_800DD258[config->text].data;
        break;
    case 2:
        str = (u8 *)config->text;
        break;
    default:
        str = uninit; // matching: the original reads an uninitialised stack local here
        break;
    }
    for (i = 0; i < index; i++) {
        while (*str != 0) {
            str++;
        }
        while (*str == 0) {
            str++;
        }
    }
    return str;
}

/* Draws the current page of a text box line by line ('\n' separates lines), tinted r/g/b/a.
 * Returns gfx unchanged if the display list would overflow. */
Gfx *text_drawBoxText(gfx, box, r, g, b, a)
    Gfx *gfx;
    TextBox *box;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
{
    Gfx *original;
    u8 line[84];
    TextBox *self; // matching: a second copy of box; IDO allocates the two differently
    f32 offset;
    s32 i;
    s32 n;
    s32 first;

    i = 0;
    n = 0;
    self = box;
    original = gfx;
lbl: // matching: unused label; with the block below it sets IDO's register priorities for box/i
    {
        f32 x;
        f32 y;

        x = box->centerX - box->halfTextWidth;
        y = box->centerY - box->halfTextHeight;
        gSPDisplayList(gfx++, gText2DSetupDL);
        first = TRUE;
        while (self->text[i] == '\n') {
            i++;
        }
        do {
            if ((gfx - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit ? TRUE : FALSE) != 0) { // matching: inline flag form
                return original;
            }
            if (1) { // matching: empty block shifts IDO's temp-register assignment
            }
            line[n] = self->text[i];
            if (line[n] == '\n') {
                line[n] = 0;
            }
            if (line[n] == 0) {
                s32 width;
                s32 height;
                s32 advance;

                text_measureString(line, &width, &height, &advance);
                if (!first) {
                    y += height;
                } else {
                    first = FALSE;
                }
                if (self->config->flags & TEXTBOX_CENTERED) {
                    offset = box->halfTextWidth - (width >> 1) - 1.0f;
                } else {
                    offset = 0.0f;
                }
                if (self->config->flags & TEXTBOX_WHITE_TEXT) {
                    gfx = text_drawString(gfx, x + offset + 1.0f, y, line, 0xFF, 0xFF, 0xFF, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else if (!(self->config->flags & TEXTBOX_DARK)) {
                    gfx = text_drawString(gfx, x + offset + 1.0f, y, line, 0, 0, 0, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else {
                    gfx = text_drawString(gfx, x + offset, y, line, r, g, b, a, 4096.0f, 4096.0f, n);
                    n = 0;
                }
                y += advance;
            } else {
                n++;
            }
        } while (box->text[i++] != 0);
    }
    return gfx;
}

/* Places a text box: its anchor comes from screen percentages or from its object's projected position,
 * and the body is set at an angle derived from the object's heading. */
void text_placeBox(TextBox *box) {
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    TextBoxConfig *config;
    s32 xoff;
    s32 yoff;
    f32 scale;
    f32 angle;

    config = box->config;
    if (config->flags & TEXTBOX_SCREEN_POS) {
        scale = *config->screenXPercent * 0.01f;
        box->anchorX = gViews->width * scale;
        scale = *config->screenYPercent * 0.01f;
        scale = 1.0f - scale;
        box->anchorY = gViews->height * scale;
        angle = *config->anglePercent * 0.01f;
        angle = (angle < 0.0f) ? 0.0f : ((angle > 1.0f) ? 1.0f : angle);
        angle = 70.0f * angle + 55.0f;
        angle *= 0.017453292f;
        box->angle = angle;
        if (config->flags & TEXTBOX_CLAMP) {
            text_clampBoxToView(box, &box->anchorX, &box->anchorY, &box->angle);
        }
    } else {
        view_projectPoint(0, config->worldX, config->worldY, config->worldZ, &screenX, &screenY, &screenZ);
        if (box->placed == 0) {
            box->baseX = screenX;
            box->baseY = screenY;
            box->yaw = movement_obj[box->config->objIndex].theta - camera_global_Cameras->CameraAngle - 180.0f;
            box->placed = 1;
        }
        angle = box->yaw;
        box->anchorX = box->baseX;
        box->anchorY = box->baseY;
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        if (angle > 180.0f) {
            angle -= 360.0f;
        }
        if (angle > 90.0f) {
            angle = 180.0f - angle;
        } else if (angle < -90.0) {
            angle = -180.0f - angle;
        }
        angle = angle / -180.0f + 0.5f;
        angle = 70.0f * angle + 55.0f;
        angle *= 0.017453292f;
        box->angle = angle;
        text_clampBoxToView(box, &box->anchorX, &box->anchorY, &box->angle);
    }
    box->x = box->anchorX;
    box->y = box->anchorY;
    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f;
    yoff = box->height / D_8009055C.height * D_8009057C.height * 0.5f;
    box->centerX = (s32)(cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f + box->anchorX - xoff);
    box->centerY = sinf(box->angle + 3.1415927f) * (box->height * 0.5f) * 1.04f + box->anchorY - yoff;
}

/* Moves a box's anchor so the whole box stays inside the view bounds, then snaps it to whole pixels. */
void text_clampBoxToView(TextBox *box, f32 *x, f32 *y, f32 *angle) {
    f32 top;
    f32 anchorY;
    f32 left;
    f32 right;
    f32 xoff;
    f32 halfW;
    f32 h;
    f32 bodyX;

    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f * box->scale;
    bodyX = *x + cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f * box->scale - xoff;
    halfW = box->width * 0.5f;
    h = box->height;
    anchorY = *y;
    top = *y - h;
    left = bodyX - halfW;
    right = halfW + bodyX;
    if (box != NULL) {
        if (top < gViews->top) {
            *y = gViews->top + h;
        } else if (gViews->bottom < anchorY) {
            *y = gViews->bottom;
        }
        if (left < gViews->left) {
            *x -= left - gViews->left;
        } else if (gViews->right < right) {
            *x += gViews->right - right;
        }
        *x = (s32)*x;
        *y = (s32)*y;
    }
}

void func_1516D2D8(void) {
}

/* Removes a text box object immediately. */
void text_removeBox(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->numPages = 0;
        effect_destroyObject(box);
        box->state = 0;
    }
}

/* Asks a text box to close, skipping its remaining pages. */
void text_closeBoxSkipPages(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        box->page = box->config->numPages;
    }
}

/* Asks a text box to close after its current page; returns it, or NULL if it isn't one. */
TextBox *text_closeBoxAfterPage(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}

/* Asks a text box to close at once; returns it, or NULL if it isn't one. */
TextBox *text_closeBoxNow(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE | TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}
