#include <ultra64.h>
#include "effects.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/text.h"

/* This file's .data: the update functions and message handlers of the text box styles (TextLines.updateFn, .msgFn). */
static s32 (*gTextBoxUpdateFuncs[])(TextObj *) = { effect_updateFadeTextBox, effect_updatePromptTextBox, effect_updateSlowFadeTextBox, text_updateAmount };
static void (*gTextBoxMessageFuncs[])(TextObj *, u8 *, u8) = { NULL, text_handleAmountMessage };

/* Returns the end of the line at str: its 0xBD line break or the string's terminating 0. */
u8 *effect_findLineEnd(u8 *str) {
    while (*str != 0xBD && *str != 0) {
        str++;
    }
    return str;
}

/* Returns the end (terminating 0) of str. */
u8 *effect_findStringEnd(u8 *str) {
    while (*str != 0) {
        str++;
    }
    return str;
}

/* Returns the length of the line at str and stores its end in *lineEnd. */
s32 effect_getLineLength(u8 *str, u8 **lineEnd) {
    u8 *end;

    end = effect_findLineEnd(str);
    *lineEnd = end;
    return end - str;
}

/* Returns the length of the longest line in str. */
s32 effect_getLongestLineLength(u8 *str) {
    s32 longest;
    u8 *end;
    u8 *lineEnd;

    longest = 0;
    end = effect_findStringEnd(str);
    while (str < end) {
        s32 len = effect_getLineLength(str, &lineEnd);
        if (longest < len) {
            longest = len;
        }
        str = lineEnd + 1;
    }
    return longest;
}

/* Copies the current line into lines[page]. */
void effect_copyTextLine(TextObj *obj) {
    TextLines *text = &obj->text;

    memcpy(text->lines[text->page], text->line, text->lineLen);
    text->lines[text->page][text->lineLen] = 0;
}

/* Moves on to the next line, if there is one. */
void effect_nextTextLine(TextObj *obj) {
    TextLines *text = &obj->text;

    if (*text->lineEnd != 0) {
        text->line = text->lineEnd + 1;
        text->lineLen = effect_getLineLength(text->lineEnd + 1, &text->lineEnd);
        text->page ^= 1;
        effect_copyTextLine(obj);
        text->flags |= 1;
    }
}

/* Creates a text box object for str, with styleSize bytes of style state (left zeroed). */
TextObj *effect_newTextBox(u8 id, u8 *str, s16 duration, s32 styleSize, s8 updateFn, u8 msgFn, u8 arg6, s32 arg7) {
    TextLines init;
    TextObj *obj;
    s32 lineSize;
    TextLines *text;

    init.msgFn = msgFn;
    init.updateFn = updateFn;
    init.page = 0;
    init.flags = 1;
    init.str = str;
    init.line = str;
    init.id = id;
    init.lineLen = effect_getLineLength(str, &init.lineEnd);
    init.lines[0] = NULL;
    init.lines[1] = NULL;
    init.style = NULL;

    lineSize = effect_getLongestLineLength(str) + 1;

    obj = effect_newTimedEffect(duration, -1, -1, 5, 3, 0x47, (sizeof(TextLines) + styleSize + lineSize * 2), arg6, arg7);
    if (obj != NULL) {
        text = &obj->text;
        memcpy(text, &init, sizeof(TextLines));
        text->style = text + 1;
        text->lines[0] = (u8 *)text->style + styleSize;
        text->lines[1] = text->lines[0] + lineSize;
        *text->lines[0] = 0;
        *text->lines[1] = 0;
        effect_copyTextLine(obj);
    }
    return obj;
}

/* Text box update: runs the style's update function, removing the box when it returns 0. */
s32 effect_drawTextBox(s32 arg0, TextObj *obj, s32 arg2) {
    TextLines *text = &obj->text;

    if ((text->updateFn != -1) && (gTextBoxUpdateFuncs[text->updateFn](obj) == 0)) {
        obj->base.header.state = -1;
        return arg0;
    }
    text->flags &= ~1;
    return arg0;
}

/* Text box message handler: message 0x51 with this box's id advances the line; the style's
   handler gets every other message. */
void effect_textBoxMessage(TextObj *obj, u8 *payload, u8 type) {
    void (*handler)(TextObj *, u8 *, u8);

    if (type == 0x51) {
        if (obj->text.id == *payload) {
            effect_nextTextLine(obj);
        }
    } else {
        handler = gTextBoxMessageFuncs[obj->text.msgFn];
        if (handler != NULL) {
            handler(obj, payload, type);
        }
    }
}

/* Style 0 state: frames left in the crossfade to a new line. */
typedef struct {
    /* 0x00 */ s8 fadeTimer;
    /* 0x01 */ u8 pad1[7];
} CrossfadeStyle;

/* Creates a text box that crossfades between lines (style 0). */
void effect_newFadeTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    CrossfadeStyle style;
    TextObj *obj;

    style.fadeTimer = 0;
    obj = effect_newTextBox(id, str, duration, 8, 0, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style.fadeTimer, sizeof(style.fadeTimer));
    }
}

/* Style 0 update: draws the current line fading in over 8 frames and the previous one fading out. */
s32 effect_updateFadeTextBox(TextObj *obj) {
    s8 *fadeTimer;
    u8 alpha;
    u8 fade;
    TextLines *text;

    text = &obj->text;
    fadeTimer = text->style;
    if (text->flags & 1) {
        *fadeTimer = 8;
    }
    if (*fadeTimer > 0) {
        fade = *fadeTimer * 31;
        alpha = 255 - fade;
        *fadeTimer -= gFrameTicks;
    } else {
        alpha = 255;
        fade = 0;
    }
    if (alpha) {
        text_setColor(0xCE, 0xC4, 0x61, alpha);
        text_printAt(0x92, 0xBE, 0x81, (char *)text->lines[text->page]);
    }
    if (fade) {
        text_setColor(0xCE, 0xC4, 0x61, fade);
        text_printAt(0x92, 0xBE, 0x81, (char *)text->lines[text->page ^ 1]);
    }
    return 1;
}

/* Style 1 state: how much of the line has been typed out. */
typedef struct {
    /* 0x00 */ f32 progress; // fraction of the next character
    /* 0x04 */ s16 shown; // characters typed so far
    /* 0x06 */ u8 pad6[2];
} TypewriterStyle;

/* Creates a text box that types out each line behind a "C:> " prompt (style 1). */
void effect_newPromptTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    TextObj *obj;
    TypewriterStyle style;

    style.shown = 0;
    style.progress = 0.0f;
    obj = effect_newTextBox(id, str, duration, 8, 1, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style, sizeof(style));
    }
}

/* Style 1 update: types out the current line and draws it after the prompt, with a cursor. */
s32 effect_updatePromptTextBox(TextObj *obj) {
    TextLines *text;
    TypewriterStyle *style;
    u8 buf[0x50];
    u8 prompt[] = "C:> ";

    text = &obj->text;
    style = text->style;

    if (text->flags & 1) {
        style->shown = 0;
        style->progress = 0.0f;
    }

    if (style->progress < text->lineLen) {
        style->progress += 0.437f * gFrameDelta;
        while (style->progress > 1.0f) {
            style->shown++;
            if (text->lineLen < style->shown) {
                style->shown = text->lineLen;
            }
            style->progress -= 1.0f;
        }
    }

    memcpy(buf, prompt, 4);
    memcpy(&buf[4], text->lines[text->page], style->shown);
    buf[style->shown + 4] = ' ';
    buf[style->shown + 5] = 0xBB;
    buf[style->shown + 6] = 0;
    text_setColor(0, 0xFF, 0, 0x96);
    text_printAt(15, 0xBE, 0x80, (char *)buf);
    return 1;
}

/* Creates a text box that crossfades between lines more slowly, in white (style 2). */
void effect_newSlowFadeTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    CrossfadeStyle style;
    TextObj *obj;

    style.fadeTimer = 0;
    obj = effect_newTextBox(id, str, duration, 8, 2, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style.fadeTimer, sizeof(style.fadeTimer));
    }
}

/* Style 2 update: like effect_updateFadeTextBox but white and over 20 frames. */
s32 effect_updateSlowFadeTextBox(TextObj *obj) {
    s8 *fadeTimer;
    u8 alpha;
    u8 fade;
    TextLines *text;

    text = &obj->text;
    fadeTimer = text->style;
    if (text->flags & 1) {
        *fadeTimer = 20;
    }
    if (*fadeTimer > 0) {
        fade = *fadeTimer * 12;
        alpha = 255 - fade;
        *fadeTimer -= gFrameTicks;
    } else {
        alpha = 255;
        fade = 0;
    }
    if (alpha) {
        text_setColor(0xFF, 0xFF, 0xFF, alpha);
        text_printAt(20, 20, 0x80, (char *)text->lines[text->page]);
    }
    if (fade) {
        text_setColor(0xFF, 0xFF, 0xFF, fade);
        text_printAt(20, 20, 0x80, (char *)text->lines[text->page ^ 1]);
    }
    return 1;
}
