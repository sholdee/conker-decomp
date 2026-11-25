#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Text box types: copies of the definitions in game_197F20.c, which holds the rest of the text box code. */

#define OBJTYPE_TEXTBOX 0x51

/* TextBoxConfig.flags (subset used here) */
#define TEXTBOX_BUBBLES         0x2     /* body drawn with extra thought bubbles: larger margin */
#define TEXTBOX_CB_CLOSED       0x1000  /* set while the callback runs: box closed */
#define TEXTBOX_CB_PAGE_ENDED   0x2000  /* set while the callback runs: a page was left */
#define TEXTBOX_CB_PAGE_SHOWN   0x4000  /* set while the callback runs: a page is fully shown */

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
    /* 0x0C */ f32 worldX;
    /* 0x10 */ f32 worldY;
    /* 0x14 */ f32 worldZ;
    /* 0x18 */ f32 *screenXPercent;
    /* 0x1C */ f32 *screenYPercent;
    /* 0x20 */ f32 *anglePercent;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ u16 duration;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 unk2B;
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
    /* 0x14 */ u8 unk14;            /* set for caller-supplied configs */
    /* 0x15 */ u8 state;
    /* 0x16 */ s16 timer;
    /* 0x18 */ s16 fadeTimer;
    /* 0x1A */ u8 textAlpha;
    /* 0x1B */ u8 bubbleAlpha[3];
    /* 0x1E */ u8 bubbleIndex;
    /* 0x1F */ u8 placed;
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

#define TEXTBOX_STATE_INIT 0x1

extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;
extern u8 D_800CC4B3[][0x32C]; /* byte 0x1E3 of each D_800CC2D0 object */

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source);

/* Splits a caller-supplied string into pages: each 0xBD separator becomes a NUL. Returns the page count. */
s32 func_1516A770(u8 *text) {
    s32 numPages = 1;

    while (*text != 0) {
        if (*text == 0xBD) {
            *text = 0;
            numPages++;
        }
        text++;
    }
    return numPages;
}

/*
 * Creates a text box object for message `msgIndex` (>= 0x100 selects the D_800DD258 table), or for
 * `suppliedConfig` when it is non-NULL, and sizes it to fit the widest and tallest page.
 */
TextBox *func_1516A7B0(u32 msgIndex, s32 objIndex, s32 arg2,
                       void (*callback)(TextBoxConfig *config, s32 page, s32 state),
                       TextBoxConfig *suppliedConfig, s32 owner) {
    TextBox *box;
    TextBoxConfig *config;
    s32 i;
    u8 *page;
    f32 margin;
    s32 width;
    s32 height;
    s32 advance;

    box = func_15167A68(OBJTYPE_TEXTBOX, 0, sizeof(TextBox), 0, owner, 2);
    if (box != NULL) {
        if (suppliedConfig == NULL) {
        // matching: unused label adds a block so IDO doesn't hoist &width/&height/&advance into s-regs
        unused:
            if (msgIndex >= 0x100) {
                config = (TextBoxConfig *)D_800DD258[msgIndex - 0x100].data;
            } else {
                config = (TextBoxConfig *)D_800DD254[msgIndex].data;
            }
        } else {
            config = suppliedConfig;
        }
        if (suppliedConfig != NULL) {
            box->source = 2;
        } else if (msgIndex >= 0x100) {
            box->source = 1;
        } else {
            box->source = 0;
        }
        config->objIndex = objIndex;
        config->unkA = arg2;
        config->callback = callback;
        box->unkF = config->flags & 0xF;
        box->config = config;
        box->state = TEXTBOX_STATE_INIT;
        box->placed = FALSE;
        if (suppliedConfig != NULL) {
            box->unk14 = 1;
            box->timer = suppliedConfig->duration;
        } else {
            box->unk14 = 0;
            box->timer = config->duration >> 1;
        }
        box->fadeTimer = 0;
        box->textAlpha = 0;
        box->scale = 0.0f;
        box->width = 0.0f;
        box->height = 0.0f;
        if (box->source == 2) {
            config->numPages = func_1516A770((u8 *)config->text);
        }
        for (i = 0; i < config->numPages; i++) {
            page = func_1516C878(config, i, box->source);
            func_150428D4(page, &width, &height, &advance);
            if (box->width < (f32)width) {
                box->width = width;
            }
            if (box->height < (f32)height) {
                box->height = height;
            }
        }
        if (config->flags & TEXTBOX_BUBBLES) {
            margin = 25.0f;
        } else {
            margin = 18.0f;
        }
        box->width = (box->width + margin * 0.5f) * 1.149999976f;
        box->height = (box->height + margin) * 1.039999962f;
        box->text = func_1516C878(config, 0, box->source);
        box->page = 0;
        config->flags &= ~(TEXTBOX_CB_CLOSED | TEXTBOX_CB_PAGE_ENDED | TEXTBOX_CB_PAGE_SHOWN);
        D_800CC4B3[config->objIndex][0] = 1;
        box->visible = FALSE;
    }
    return box;
}
