#include <ultra64.h>
#include "structs.h"

/* The .bss of the text box unit (this file and game/Text/TextBox/textbox.c), defined before conker/text.h declares it
 * (see init/Sched/SchedInit/schedinit.c). */
s32 D_800DD250;           /* not referenced */
MessageEntry *D_800DD254;
MessageEntry *D_800DD258;
s32 D_800DD25C;           /* not referenced */
u8 D_800DD260[0x40];      /* not referenced */

#include "functions.h"
#include "variables.h"

/* Text box types: copies of the definitions in game/Text/TextBox/textbox.c, which holds the rest of the text box code. */

#define OBJTYPE_TEXTBOX 0x51

/* TextBoxConfig.flags (subset used here) */
#define TEXTBOX_BUBBLES         0x2     /* body drawn with extra thought bubbles: larger margin */
#define TEXTBOX_CB_CLOSED       0x1000  /* set while the callback runs: box closed */
#define TEXTBOX_CB_PAGE_ENDED   0x2000  /* set while the callback runs: a page was left */
#define TEXTBOX_CB_PAGE_SHOWN   0x4000  /* set while the callback runs: a page is fully shown */

#define TEXTBOX_STATE_INIT 0x1


/* Splits a caller-supplied string into pages: each 0xBD separator becomes a NUL. Returns the page count. */
s32 text_splitPages(u8 *text) {
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
TextBox *text_createBox(u32 msgIndex, s32 objIndex, s32 arg2,
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

    box = effect_newObject(OBJTYPE_TEXTBOX, 0, sizeof(TextBox), 0, owner, 2);
    if (box != NULL) {
        if (suppliedConfig == NULL) {
        // matching: unused label adds a block so IDO doesn't hoist &width/&height/&advance into s-regs
        unused:
            if (msgIndex >= 0x100) {
                config = D_800DD258[msgIndex - 0x100].data;
            } else {
                config = D_800DD254[msgIndex].data;
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
            box->noInput = 1;
            box->timer = suppliedConfig->duration;
        } else {
            box->noInput = 0;
            box->timer = config->duration >> 1;
        }
        box->fadeTimer = 0;
        box->textAlpha = 0;
        box->scale = 0.0f;
        box->width = 0.0f;
        box->height = 0.0f;
        if (box->source == 2) {
            config->numPages = text_splitPages((u8 *)config->text);
        }
        for (i = 0; i < config->numPages; i++) {
            page = text_getPage(config, i, box->source);
            text_measureString(page, &width, &height, &advance);
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
        box->text = text_getPage(config, 0, box->source);
        box->page = 0;
        config->flags &= ~(TEXTBOX_CB_CLOSED | TEXTBOX_CB_PAGE_ENDED | TEXTBOX_CB_PAGE_SHOWN);
        movement_obj[config->objIndex].unk1E3 = 1;
        box->visible = FALSE;
    }
    return box;
}
