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

#define TEXTBOX_STATE_INIT 0x1


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
            if (box->width < width) {
                box->width = width;
            }
            if (box->height < height) {
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
