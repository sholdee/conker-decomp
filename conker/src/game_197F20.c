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

/* Message table entry (D_800DD254 / D_800DD258). */
typedef struct {
    u8 *data;
    s32 unk4;
} MessageEntry;

/* Text box setup: a message-table entry, or one supplied by the caller (see func_1516A7B0). */
typedef struct TextBoxConfig {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 text;            /* message table index, or the string itself when supplied */
    /* 0x08 */ u8 numPages;         /* strings in the text, separated by NULs */
    /* 0x09 */ u8 objIndex;         /* D_800CC2D0 object the box belongs to */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
    /* 0x0C */ f32 worldX;          /* projected to screen when not TEXTBOX_SCREEN_POS */
    /* 0x10 */ f32 worldY;
    /* 0x14 */ f32 worldZ;
    /* 0x18 */ f32 *screenXPercent; /* TEXTBOX_SCREEN_POS placement */
    /* 0x1C */ f32 *screenYPercent;
    /* 0x20 */ f32 *anglePercent;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ u16 duration;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 unk2B;            /* box is removed unless D_800C35EA == 1 */
    /* 0x2C */ void (*callback)(struct TextBoxConfig *config, s32 page, s32 state);
} TextBoxConfig;

/* Object type OBJTYPE_TEXTBOX, 0x74 bytes. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0xC];
    /* 0x0D */ u8 source;           /* 0: D_800DD254 text, 1: D_800DD258 text, 2: caller string */
    /* 0x0E */ u8 visible;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ TextBoxConfig *config;
    /* 0x14 */ u8 unk14;            /* set for caller-supplied configs; disables button input */
    /* 0x15 */ u8 state;            /* TEXTBOX_STATE_* */
    /* 0x16 */ s16 timer;
    /* 0x18 */ s16 fadeTimer;
    /* 0x1A */ u8 textAlpha;
    /* 0x1B */ u8 bubbleAlpha[3];
    /* 0x1E */ u8 bubbleIndex;
    /* 0x1F */ u8 placed;           /* screen position taken from the world point */
    /* 0x20 */ s8 page;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ u8 *text;            /* current page */
    /* 0x28 */ f32 scale;
    /* 0x2C */ f32 scaleSpeed;
    /* 0x30 */ f32 halfTextWidth;
    /* 0x34 */ f32 halfTextHeight;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 height;
    /* 0x40 */ f32 x;               /* anchor (tip of the tail) */
    /* 0x44 */ f32 y;
    /* 0x48 */ f32 bodyX;           /* scaled body position */
    /* 0x4C */ f32 bodyY;
    /* 0x50 */ f32 centerX;         /* unscaled body position: the text origin */
    /* 0x54 */ f32 centerY;
    /* 0x58 */ f32 angle;           /* direction from the anchor to the body, radians */
    /* 0x5C */ f32 anchorX;
    /* 0x60 */ f32 anchorY;
    /* 0x64 */ f32 baseX;
    /* 0x68 */ f32 baseY;
    /* 0x6C */ f32 yaw;             /* object heading relative to the camera, degrees */
    /* 0x70 */ f32 bubbleX;
} TextBox;

/* TextBox.state */
#define TEXTBOX_STATE_INIT      0x1
#define TEXTBOX_STATE_GROW      0x2
#define TEXTBOX_STATE_SETTLE    0x4
#define TEXTBOX_STATE_BUBBLES   0x8
#define TEXTBOX_STATE_TEXT_IN   0x10
#define TEXTBOX_STATE_SHOW      0x20
#define TEXTBOX_STATE_TEXT_OUT  0x40
#define TEXTBOX_STATE_SHRINK    0x80

extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;

extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source);

extern u8 D_800CC4B3[][0x32C]; /* byte 0x1E3 of each D_800CC2D0 object, set by func_1516A7B0 */
s32 func_151F2CDC(void);
void func_1516CC58(); // matching: unprototyped (a prototype changes AA70's register allocation)

/* Per-frame update of a text box: runs the open / page / close state machine, then re-places it. */
void func_1516AA70(TextBox *box) {
    s32 unused; // matching: unused local, needed for the original stack frame
    Camera *player;
    s32 width;
    s32 height;
    s32 advance;
    s32 skip;
    s32 advancePage;

    player = D_800DBFF0;
    skip = box->config->flags & TEXTBOX_SKIP_ANIM;
    if (box->config->unk2B != 0 && D_800C35EA != 1) {
        box->config->numPages = 0;
        func_1516972C((struct102 *)box);
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
            func_1516CC58(box);
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
        func_150428D4(box->text, &width, &height, &advance);
        box->halfTextWidth = width >> 1;
        box->halfTextHeight = height >> 1;
        box->scale += box->scaleSpeed * D_800BE9A4;
        if (skip) {
            box->fadeTimer = 0;
        }
        if (box->scale > 0.75f && box->fadeTimer == 0) {
            box->fadeTimer = 10;
        }
        if (box->fadeTimer != 0) {
            box->fadeTimer -= D_800BE9A0;
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
        box->timer -= D_800BE9A0 * 45;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        box->fadeTimer -= D_800BE9A0;
        box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
        box->textAlpha = (10.0f - box->fadeTimer) / 10.0f * 255.0f;
        if (box->timer == 0) {
            box->state = TEXTBOX_STATE_TEXT_IN;
        }
        break;
    case TEXTBOX_STATE_BUBBLES:
        /* fade the circles in one at a time, from the anchor outwards */
        box->timer -= D_800BE9A0;
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
        box->timer -= D_800BE9A0;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        if (skip) {
            box->timer = 0;
        }
        box->textAlpha = box->timer / 10.0f * 255.0f;
        if (box->timer == 0) {
            box->page++;
            if (box->page < box->config->numPages) {
                box->text = func_1516C878(box->config, box->page, box->source);
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
        func_150428D4(box->text, &width, &height, &advance);
        box->halfTextWidth = width >> 1;
        box->halfTextHeight = height >> 1;
        box->fadeTimer -= D_800BE9A0;
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
        box->timer -= D_800BE9A0;
        box->timer = (box->timer < 0) ? 0 : box->timer;
        if ((u16)(box->config->flags & TEXTBOX_INPUT) && box->unk14 == 0) {
            if (!(box->config->flags & TEXTBOX_INPUT_CHECK) || func_151F2CDC() == 0) {
                if ((box->config->flags & TEXTBOX_B_RESTARTS) && box->page + 1 == box->config->numPages) {
                    if (D_800BE710[0] & B_BUTTON) {
                        box->page = -1;
                        advancePage = TRUE;
                        D_800D1994 = 0;
                    } else if (D_800BE710[0] & A_BUTTON) {
                        advancePage = TRUE;
                    }
                } else if (D_800BE710[0] & A_BUTTON) {
                    advancePage = TRUE;
                } else if (!(box->config->flags & TEXTBOX_NO_B_SKIP) && (D_800BE710[0] & B_BUTTON)) {
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
            box->fadeTimer -= D_800BE9A0;
            box->fadeTimer = (box->fadeTimer < 0) ? 0 : box->fadeTimer;
            box->bubbleAlpha[0] = box->fadeTimer / 5.0f * 85.0f;
            box->bubbleAlpha[1] = box->bubbleAlpha[0];
            box->bubbleAlpha[2] = box->bubbleAlpha[0];
        } else {
            box->scale -= box->scaleSpeed * D_800BE9A4;
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
            D_800CC4B3[box->config->objIndex][0] = 0;
            *(s16 *)((u8 *)player + 0x6CC) = 0; /* Camera has no field here yet */
            box->config->numPages = 0;
            func_1516972C((struct102 *)box);
        }
        break;
    }
    func_1516CC58(box);
}

/* Sprite parameters passed to func_15094F70 / func_15095A48. */
typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s16 w;
    /* 0x08 */ s16 h;
    /* 0x0A */ u8 alpha;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 flags; /* SPRITE_FLIP_* */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ void *tex;
} TextBoxSprite;

#define SPRITE_FLIP_X 1 /* set while the body is left of the anchor */
#define SPRITE_FLIP_Y 2 /* set while the body is below the anchor */

/* Sprite texture header (D_8009055C body, D_8009056C bubble body, D_8009057C / D_8009059C tails,
 * D_8009058C bubble circle). */
typedef struct {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 width;
    /* 0x8 */ u16 height;
} TextBoxTexture;

extern TextBoxTexture D_8009055C;
extern TextBoxTexture D_8009057C;
extern TextBoxTexture D_8009056C;
extern TextBoxTexture D_8009058C;
extern TextBoxTexture D_8009059C;
/* Render mode pairs: cycle 1 and cycle 2 blender settings. */
extern struct {
    u32 cycle1;
    u32 cycle2;
} D_800A4AC8[];
extern Gfx *D_800BE9C8[];
extern s32 D_800BEBA4;
extern u8 D_800D2DA8;
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern u8 D_800E0BDC;
Gfx *func_15094F70(Gfx *, TextBoxTexture *, s32, TextBoxSprite *, s32, s32, s32, s32, s32);
Gfx *func_15095A48(Gfx *, TextBoxSprite *, f32, f32);
Gfx *func_1516C934(Gfx *, TextBox *, s32, s32, s32, s32);

/* Draws a text box: shadow, body, tail (or thought-bubble circles), then the text via func_1516C934.
 * Returns gfx unchanged if the display list would overflow. */
Gfx *func_1516B6BC(Gfx *gfx, TextBox *box, s32 arg2) {
    TextBoxSprite sprite;
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
    D_800D2DA8 = D_800D2DA9 = D_800D2DAA = 0;
    fade = D_800E0BDC + 1;
    sprite.flags = 0;
    sprite.alpha = (64 * fade) >> 8;
    texW = D_8009055C.width;
    texH = D_8009055C.height;
    sprite.w = (2.0f * box->width + 1.0f) / D_8009055C.width * 2355.2f * box->scale;
    sprite.h = (box->height + 1.0f) / D_8009055C.height * 4259.84f * box->scale;
    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f * box->scale;
    yoff = box->height / D_8009055C.height * D_8009057C.height * 0.5f * box->scale;
    box->bodyX = cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f * box->scale + box->x - xoff;
    box->bodyY = sinf(box->angle + 3.1415927f) * (box->height * 0.5f) * 1.04f * box->scale + box->y - yoff;
    /* shadow pass */
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0,
                      TEXEL0, 0);
    gDPSetOtherMode(gfx++, G_TF_BILERP | G_TC_FILT | 0xF, D_800A4AC8[25].cycle1 | D_800A4AC8[25].cycle2 | G_ZS_PRIM);
    if (box->config->flags & TEXTBOX_BUBBLES) {
        sprite.alpha = box->bubbleAlpha[0] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    if (!dark) {
        gfx = func_15095A48(gfx, &sprite, box->bodyX + 5.0f, box->bodyY + -5.0f);
    }
    if (box->angle > 0.0f && box->angle < 3.1415927f) {
        sprite.flags &= ~SPRITE_FLIP_Y;
    } else {
        sprite.flags |= SPRITE_FLIP_Y;
    }
    if (box->angle > 1.5707964f && box->angle < 4.712389f) {
        sprite.flags |= SPRITE_FLIP_X;
    } else {
        sprite.flags &= ~SPRITE_FLIP_X;
    }
    overflow = (gfx - D_800BE9C8[D_800BE9C0] > D_800BEBA4) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    if (box->config->flags & TEXTBOX_BUBBLES) {
        if (box->state == TEXTBOX_STATE_INIT) {
            box->bubbleX = box->anchorX;
        }
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[1] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        if (box->angle > 1.5707964f) {
            box->bubbleX += (box->x + xoff - box->bubbleX) * 0.3f;
            gfx = func_15095A48(gfx, &sprite, box->bubbleX, box->y - yoff);
        } else {
            box->bubbleX += (box->x - xoff - box->bubbleX) * 0.3f;
            gfx = func_15095A48(gfx, &sprite, box->bubbleX, box->y - yoff);
        }
        sprite.w /= 2;
        sprite.h /= 2;
        sprite.alpha = box->bubbleAlpha[2] * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = func_15095A48(gfx, &sprite, box->x - 2.5f, box->y + -2.5f);
        } else {
            gfx = func_15095A48(gfx, &sprite, box->x + 2.5f, box->y + -2.5f);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (!dark) {
            if (box->angle > 1.5707964f) {
                gfx = func_15095A48(gfx, &sprite, box->x + xoff - 2.5f, box->y - yoff + -2.5f);
            } else {
                gfx = func_15095A48(gfx, &sprite, box->x - xoff + 2.5f, box->y - yoff + -2.5f);
            }
        }
    }
    overflow = (gfx - D_800BE9C8[D_800BE9C0] > D_800BEBA4) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    /* the box itself, tinted with the config's colour */
    if (dark) {
        D_800D2DA8 = D_800D2DA9 = D_800D2DAA = 0;
    } else {
        D_800D2DA8 = box->config->r;
        D_800D2DA9 = box->config->g;
        D_800D2DAA = box->config->b;
    }
    sprite.flags = 0;
    if (dark) {
        sprite.alpha = 0x55;
    } else {
        sprite.alpha = 0xFF;
    }
    sprite.alpha = (sprite.alpha * fade) >> 8;
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0,
                      TEXEL0, 0);
    gDPSetOtherMode(gfx++, G_TF_BILERP | G_TC_FILT | 0xF, D_800A4AC8[25].cycle1 | D_800A4AC8[25].cycle2 | G_ZS_PRIM);
    if (box->angle > 0.0f && box->angle < 3.1415927f) {
        sprite.flags &= ~SPRITE_FLIP_Y;
    } else {
        sprite.flags |= SPRITE_FLIP_Y;
    }
    if (box->angle > 1.5707964f && box->angle < 4.712389f) {
        sprite.flags |= SPRITE_FLIP_X;
    } else {
        sprite.flags &= ~SPRITE_FLIP_X;
    }
    sprite.w = box->width / D_8009055C.width * 2355.2f * box->scale;
    sprite.h = box->height / D_8009055C.height * 4259.84f * box->scale;
    if (box->config->flags & TEXTBOX_BUBBLES) {
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[1];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = func_15095A48(gfx, &sprite, box->bubbleX, box->y - yoff);
        sprite.w /= 2;
        sprite.h /= 2;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = box->bubbleAlpha[2];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        if (box->angle > 1.5707964f) {
            gfx = func_15095A48(gfx, &sprite, box->x, box->y);
        } else {
            gfx = func_15095A48(gfx, &sprite, box->x, box->y);
        }
    } else if (box->config->flags & TEXTBOX_ALT_TAIL) {
        gfx = func_15094F70(gfx, &D_8009059C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = func_15095A48(gfx, &sprite, box->x + xoff, box->y - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, box->x - xoff, box->y - yoff);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (box->angle > 1.5707964f) {
            gfx = func_15095A48(gfx, &sprite, box->x + xoff, box->y - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, box->x - xoff, box->y - yoff);
        }
    }
    sprite.w = 2.0f * box->width / D_8009055C.width * 2355.2f * box->scale;
    sprite.h = box->height / D_8009055C.height * 4259.84f * box->scale;
    if (box->config->flags & TEXTBOX_BUBBLES) {
        sprite.alpha = box->bubbleAlpha[0];
        sprite.alpha = (sprite.alpha * fade) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    gfx = func_15095A48(gfx, &sprite, (s32)box->bodyX, (s32)box->bodyY);
    overflow = (gfx - D_800BE9C8[D_800BE9C0] > D_800BEBA4) ? TRUE : FALSE;
    if (overflow) {
        return start;
    }
    return func_1516C934(gfx, box, 0, 0, 0, (box->textAlpha * fade) >> 8);
}

/* Returns the start of page `index` of a config's text: pages are separated by runs of NULs. */
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source) {
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

Gfx *func_150417AC(Gfx *, f32, f32, u8 *, u8, u8, u8, u8, f32, f32, s32);
extern u8 D_800859A0[];

/* Draws the current page of a text box line by line ('\n' separates lines), tinted r/g/b/a.
 * Returns gfx unchanged if the display list would overflow. */
Gfx *func_1516C934(gfx, box, r, g, b, a)
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
        gSPDisplayList(gfx++, D_800859A0);
        first = TRUE;
        while (self->text[i] == '\n') {
            i++;
        }
        do {
            if ((gfx - D_800BE9C8[D_800BE9C0] > D_800BEBA4 ? TRUE : FALSE) != 0) { // matching: inline flag form
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

                func_150428D4(line, &width, &height, &advance);
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
                    gfx = func_150417AC(gfx, x + offset + 1.0f, y, line, 0xFF, 0xFF, 0xFF, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else if (!(self->config->flags & TEXTBOX_DARK)) {
                    gfx = func_150417AC(gfx, x + offset + 1.0f, y, line, 0, 0, 0, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else {
                    gfx = func_150417AC(gfx, x + offset, y, line, r, g, b, a, 4096.0f, 4096.0f, n);
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

/* What D_800BE628 points to. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 width;
    /* 0x08 */ f32 height;
    /* 0x0C */ u8 padC[0x18];
    /* 0x24 */ f32 minY;
    /* 0x28 */ f32 maxY;
    /* 0x2C */ f32 minX;
    /* 0x30 */ f32 maxX;
} TextBoxView;

#define VIEW ((TextBoxView *)D_800BE628)

void func_1501A764(s16, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *screenZ);
void func_1516D0CC(TextBox *box, f32 *x, f32 *y, f32 *angle);

/* Places a text box: its anchor comes from screen percentages or from its object's projected position,
 * and the body is set at an angle derived from the object's heading. */
void func_1516CC58(TextBox *box) {
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
        box->anchorX = VIEW->width * scale;
        scale = *config->screenYPercent * 0.01f;
        scale = 1.0f - scale;
        box->anchorY = VIEW->height * scale;
        angle = *config->anglePercent * 0.01f;
        angle = (angle < 0.0f) ? 0.0f : ((angle > 1.0f) ? 1.0f : angle);
        angle = 70.0f * angle + 55.0f;
        angle *= 0.017453292f;
        box->angle = angle;
        if (config->flags & TEXTBOX_CLAMP) {
            func_1516D0CC(box, &box->anchorX, &box->anchorY, &box->angle);
        }
    } else {
        func_1501A764(0, config->worldX, config->worldY, config->worldZ, &screenX, &screenY, &screenZ);
        if (box->placed == 0) {
            box->baseX = screenX;
            box->baseY = screenY;
            box->yaw = D_800CC2D0[box->config->objIndex].unk40 - D_800DBFF0->yaw - 180.0f;
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
        func_1516D0CC(box, &box->anchorX, &box->anchorY, &box->angle);
    }
    box->x = box->anchorX;
    box->y = box->anchorY;
    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f;
    yoff = box->height / D_8009055C.height * D_8009057C.height * 0.5f;
    box->centerX = (s32)(cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f + box->anchorX - xoff);
    box->centerY = sinf(box->angle + 3.1415927f) * (box->height * 0.5f) * 1.04f + box->anchorY - yoff;
}

/* Moves a box's anchor so the whole box stays inside the view bounds, then snaps it to whole pixels. */
void func_1516D0CC(TextBox *box, f32 *x, f32 *y, f32 *angle) {
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
        if (top < VIEW->minY) {
            *y = VIEW->minY + h;
        } else if (VIEW->maxY < anchorY) {
            *y = VIEW->maxY;
        }
        if (left < VIEW->minX) {
            *x -= left - VIEW->minX;
        } else if (VIEW->maxX < right) {
            *x += VIEW->maxX - right;
        }
        *x = (s32)*x;
        *y = (s32)*y;
    }
}

void func_1516D2D8(void) {
}

/* Removes a text box object immediately. */
void func_1516D2E0(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->numPages = 0;
        func_1516972C((struct102 *)box);
        box->state = 0;
    }
}

/* Asks a text box to close, skipping its remaining pages. */
void func_1516D328(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        box->page = box->config->numPages;
    }
}

/* Asks a text box to close after its current page; returns it, or NULL if it isn't one. */
TextBox *func_1516D378(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}

/* Asks a text box to close at once; returns it, or NULL if it isn't one. */
TextBox *func_1516D3C4(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE | TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}
