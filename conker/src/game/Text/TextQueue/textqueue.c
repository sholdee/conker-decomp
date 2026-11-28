#include <ultra64.h>
#include "libc/stdarg.h"
#include "structs.h"

/*
 * Text queue state, this file's .bss. IDO lays out .bss in an order that depends on the declarations
 * before it, so it is defined before the subsystem headers. All of it applies to the next queued
 * nodes; drawing resets it.
 */
static u8 gQueuedTextRed; // red
static u8 gQueuedTextGreen; // green
static u8 gQueuedTextBlue; // blue
static u8 gQueuedTextAlpha; // alpha
static struct TextQueueNode *gTextQueueHead; // head node
static struct TextQueueNode *gTextQueueTail; // tail node
static u8 D_800CBD6C; // node unk12..unk15
static u8 D_800CBD6D;
static u8 D_800CBD6E;
static u8 D_800CBD6F;
static s16 gQueuedTextX; // x
static s16 gQueuedTextY; // y
static u8 gQueuedTextFlags;  // TEXT_FLAG_*
static Actor *gQueuedTextAnchor; // anchor actor
static s16 gQueuedTextAnchorHeight;    // height above the anchor
static f32 gQueuedTextScale;    // scale

#include "conker/game.h"
#include "conker/heap.h"
#include "conker/hud.h"
#include "conker/libc.h"
#include "conker/save.h"
#include "conker/sprite.h"
#include "conker/text.h"
#include "conker/view.h"
#include "libc/string.h"

/* One queued line of HUD text (or, when `icon` is nonzero, one icon), freed after drawing. */
typedef struct TextQueueNode {
    Actor *anchor; /* if set, the text is drawn over this actor */
    f32 scale;
    s16 x;
    s16 y; /* height above the anchor while one is attached */
    u8 flags; /* TEXT_FLAG_* */
    u8 icon; /* 1-based index into gHudIcons; 0 = text */
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    struct TextQueueNode *next;
    u8 text[0x40];
} TextQueueNode;

#define TEXT_FLAG_CENTERED 0x01 /* centre horizontally on x */
#define TEXT_FLAG_SCREEN_SPACE 0x40 /* x/y are already in screen pixels: skip the resolution scale */
#define TEXT_FLAG_SHADOW 0x80

/* HUD icon table entry (gHudIcons). */
typedef struct HudIcon {
    u8 width;
    u8 height;
    u8 scale; /* 128 = 1.0 */
    u8 flags; /* 1: draw the previous entry behind it and pulse the alpha; 2: 16x16 sprite (default 32x32) */
    u8 *image; /* texture id (a symbol address), consecutive ids for animation frames */
} HudIcon;

/* This file's .data. */

/* Render setup for 2D text and sprites: primitive colour, alpha from the I texture. */
Gfx gText2DSetupDL[] = {
    gsDPPipeSync(),
    gsDPSetOtherMode(0x002C0F, 0x00504244),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, 1, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0, 0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0),
    gsSPEndDisplayList(),
};
static f32 D_800859C8[4] = { 4096.0f, 4096.0f, 4096.0f, 4096.0f }; /* not referenced */
static s32 D_800859D8[2] = { 0x101F0000, 0 };                     /* not referenced */

/* The HUD icons, by icon number - 1. */
static HudIcon gHudIcons[] = {
    { 1, 1, 0x80, 0, (u8 *)0x894 },
    { 1, 1, 0x80, 0, (u8 *)0x895 },
    { 1, 1, 0x80, 0, (u8 *)0x896 },
    { 1, 1, 0x80, 0, (u8 *)0x897 },
    { 1, 1, 0x80, 0, (u8 *)0x87B },
    { 1, 1, 0x80, 0, (u8 *)0x876 },
    { 1, 1, 0x80, 0, (u8 *)0x877 },
    { 1, 1, 0x80, 0, (u8 *)0x8A7 },
    { 1, 1, 0x80, 0, (u8 *)0x8A3 },
    { 1, 1, 0x80, 0, (u8 *)0x8AC },
    { 1, 1, 0x80, 0, (u8 *)0x89B },
    { 2, 1, 0x80, 0, (u8 *)0x7D3 },
    { 2, 1, 0x80, 1, (u8 *)0x7D5 },
    { 2, 1, 0x80, 0, (u8 *)0x7FC },
    { 2, 1, 0x80, 1, (u8 *)0x7FE },
    { 2, 1, 0x80, 0, (u8 *)0x7CD },
    { 1, 1, 0x80, 0, (u8 *)0x7C4 },
    { 1, 1, 0x80, 0, (u8 *)0x7C5 },
    { 2, 1, 0x80, 0, (u8 *)0x818 },
    { 2, 1, 0x80, 0, (u8 *)0x81A },
    { 1, 1, 0x80, 0, (u8 *)0x7ED },
    { 2, 1, 0x80, 0, (u8 *)0x7F7 },
    { 2, 1, 0x80, 1, (u8 *)0x7F9 },
    { 2, 1, 0x80, 0, (u8 *)0x7D7 },
    { 2, 1, 0x80, 1, (u8 *)0x7D9 },
    { 1, 1, 0x80, 0, (u8 *)0x7E7 },
    { 2, 1, 0x80, 0, (u8 *)0x812 },
    { 2, 1, 0x80, 1, (u8 *)0x814 },
    { 1, 1, 0x80, 0, (u8 *)0x7F1 },
    { 1, 1, 0x80, 1, (u8 *)0x7F2 },
    { 1, 1, 0x80, 0, (u8 *)0x82F },
    { 1, 1, 0x80, 1, (u8 *)0x830 },
    { 2, 1, 0x80, 0, (u8 *)0x80E },
    { 2, 1, 0x80, 1, (u8 *)0x810 },
    { 2, 1, 0x80, 0, (u8 *)0x800 },
    { 2, 1, 0x80, 1, (u8 *)0x802 },
    { 2, 1, 0x80, 0, (u8 *)0x804 },
    { 2, 1, 0x80, 1, (u8 *)0x806 },
    { 2, 1, 0x80, 0, (u8 *)0x81C },
    { 2, 1, 0x80, 1, (u8 *)0x81E },
    { 2, 1, 0x80, 0, (u8 *)0x825 },
    { 2, 1, 0x80, 1, (u8 *)0x827 },
    { 2, 1, 0x80, 0, (u8 *)0x7C0 },
    { 2, 1, 0x80, 1, (u8 *)0x7C2 },
    { 2, 1, 0x80, 0, (u8 *)0x7CF },
    { 2, 1, 0x80, 1, (u8 *)0x7D1 },
    { 2, 1, 0x80, 0, (u8 *)0x7C6 },
    { 2, 1, 0x80, 1, (u8 *)0x7C8 },
    { 3, 1, 0x80, 0, (u8 *)0x7EE },
    { 2, 1, 0x80, 0, (u8 *)0x810 },
    { 2, 1, 0x80, 0, (u8 *)0x7FE },
    { 1, 1, 0x80, 0, (u8 *)0x87C },
    { 1, 1, 0x80, 0, (u8 *)0x88B },
    { 1, 1, 0x80, 0, (u8 *)0x88A },
    { 1, 1, 0x80, 0, (u8 *)0x889 },
    { 1, 1, 0x80, 0, (u8 *)0x8AA },
    { 1, 1, 0x80, 0, (u8 *)0x89A },
    { 1, 1, 0x80, 0, (u8 *)0x8A0 },
    { 1, 1, 0x80, 2, (u8 *)0x8B0 },
    { 1, 1, 0x80, 2, (u8 *)0x8B1 },
    { 1, 1, 0x80, 0, (u8 *)0x8AE },
    { 1, 1, 0x80, 0, (u8 *)0x89D },
    { 1, 1, 0x80, 0, (u8 *)0x844 },
    { 1, 1, 0x80, 0, (u8 *)0x842 },
    { 1, 1, 0x80, 0, (u8 *)0x865 },
    { 3, 1, 0x80, 0, (u8 *)0x83C },
    { 3, 1, 0x80, 0, (u8 *)0x84E },
    { 3, 1, 0x80, 0, (u8 *)0x836 },
    { 3, 1, 0x80, 0, (u8 *)0x833 },
    { 3, 1, 0x80, 0, (u8 *)0x845 },
    { 3, 1, 0x80, 0, (u8 *)0x84B },
    { 3, 1, 0x80, 0, (u8 *)0x848 },
    { 3, 1, 0x80, 0, (u8 *)0x83F },
    { 3, 1, 0x80, 0, (u8 *)0x839 },
    { 2, 1, 0x80, 0, (u8 *)0x816 },
    { 2, 1, 0x80, 0, (u8 *)0x863 },
    { 2, 1, 0x80, 0, (u8 *)0x851 },
    { 3, 1, 0x80, 0, (u8 *)0x858 },
    { 3, 1, 0x80, 0, (u8 *)0x86F },
    { 3, 1, 0x80, 0, (u8 *)0x866 },
    { 2, 1, 0x80, 0, (u8 *)0x869 },
    { 2, 1, 0x80, 0, (u8 *)0x872 },
    { 2, 1, 0x80, 0, (u8 *)0x86B },
    { 3, 1, 0x80, 0, (u8 *)0x860 },
    { 3, 1, 0x80, 0, (u8 *)0x855 },
    { 2, 1, 0x80, 0, (u8 *)0x86D },
    { 2, 1, 0x80, 0, (u8 *)0x85B },
    { 2, 1, 0x80, 0, (u8 *)0x853 },
    { 2, 1, 0x55, 0, (u8 *)0x863 },
    { 2, 1, 0x55, 0, (u8 *)0x851 },
    { 3, 1, 0x55, 0, (u8 *)0x858 },
    { 3, 1, 0x55, 0, (u8 *)0x86F },
};

static u8 D_80085CC0[4] = "%s%s"; /* deliberately unterminated: runs on into the spec buffer below */
static u8 D_80085CC4[12] = "        "; /* conversion spec being parsed, e.g. "%2d" */
static s32 D_80085CD0 = 0; /* frame counter */

/** Drop the queued text without drawing (or freeing) it and reset the text state. */
void text_clearQueue(void) {
    gTextQueueHead = NULL;
    text_drawQueue(NULL);
}

/** Set the flags used by subsequently queued text. */
void text_setFlags(u8 flags) {
    gQueuedTextFlags = flags;
}

/** Queue formatted text at (x, y) with the given flags; see text_parseFormat for the format. */
void text_printAt(s32 x, s32 y, u8 flags, char *format, ...) {
    va_list args;
    s32 argumentWords[16];
    s32 i;

    gQueuedTextFlags = flags;
    gQueuedTextX = x;
    gQueuedTextY = y;
    va_start(args, format);
    for (i = 0; i < 16; i++) {
        argumentWords[i] = va_arg(args, s32);
    }
    text_parseFormat((u8 *)format, argumentWords);
}

/** Queue formatted text at the current text position. */
void text_print(char *format, ...) {
    va_list args;
    s32 argumentWords[16];
    s32 i;

    va_start(args, format);
    for (i = 0; i < 16; i++) {
        argumentWords[i] = va_arg(args, s32);
    }
    text_parseFormat((u8 *)format, argumentWords);
}

/**
 * Parse `format` into queued text nodes, one per line. Supports %d/%x/%X/%s, %f/%F (the argument
 * word is a pointer to an f32), %%, "#?" (the next argument becomes an icon node; the character
 * after '#' is skipped) and '\n' (starts a new node 11 pixels lower).
 */
void text_parseFormat(u8 *format, s32 *argumentWords) {
    s32 pad; // matching: pads place `finished` at sp+0x50 and `newNode` at sp+0x48
    s32 finished;
    s32 pad2;
    TextQueueNode *newNode;
    TextQueueNode *node;
    u8 *out;
    s32 argumentIndex;
    s32 conversionIndex;
    u8 *spec;
    u8 ch;

    argumentIndex = 0;
    spec = D_80085CC4;
    while (*format != 0) {
        newNode = allocate_memory(sizeof(TextQueueNode), 1, 0, 1);
        if (newNode == NULL) {
            return;
        }
        newNode->next = NULL;
        finished = 0;
        if (gTextQueueTail == NULL) {
            gTextQueueHead = newNode;
        } else {
            gTextQueueTail->next = newNode;
        }
        out = newNode->text;
        newNode->icon = 0;
        do {
            ch = *format;
            if (ch == '%') {
                format++;
                spec[0] = '%';
                for (conversionIndex = 1; conversionIndex != 0; conversionIndex++, format++) {
                    spec[conversionIndex] = *format;
                    spec[conversionIndex + 1] = 0;
                    switch (spec[conversionIndex]) {
                    case 'X':
                    case 'd':
                    case 'x':
                        func_151EFF94(out, spec, argumentWords[argumentIndex++]);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    case 'F':
                    case 'f': {
                        f32 *number = (f32 *)argumentWords[argumentIndex++];
                        f32 value = *number;

                        func_151EFF94(out, D_80085CC0, "", "", value);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    }
                    case 's':
                        func_151EFF94(out, spec, argumentWords[argumentIndex++]);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    case '%':
                        *out++ = *format;
                        *out = 0;
                        conversionIndex = -1;
                        break;
                    default: /* flags/width/precision: keep copying into the spec */
                        break;
                    }
                }
            } else if (ch == '#') {
                newNode->icon = argumentWords[argumentIndex++];
                format += 2;
            } else if (ch == '\n') {
                format++;
                finished = 1;
            } else if (ch == 0) {
                finished = 1;
            } else {
                *out = ch;
                format++;
                out++;
                *out = 0;
            }
        } while (finished == 0);

        node = newNode;
        node->flags = gQueuedTextFlags;
        node->anchor = gQueuedTextAnchor;
        if (node->anchor != NULL) {
            node->y = gQueuedTextAnchorHeight;
            node->flags |= TEXT_FLAG_CENTERED;
            node->flags = node->flags; // matching: IDO re-stores the flags byte after the OR
            if (gHumanCount >= 2) {
                node->scale = gQueuedTextScale + gQueuedTextScale;
            } else {
                node->scale = gQueuedTextScale;
            }
        } else {
            node->x = gQueuedTextX;
            node->y = gQueuedTextY;
            node->scale = gQueuedTextScale;
        }
        node->red = gQueuedTextRed;
        node->green = gQueuedTextGreen;
        node->blue = gQueuedTextBlue;
        node->alpha = gQueuedTextAlpha;
        node->unk12 = D_800CBD6C;
        node->unk13 = D_800CBD6D;
        node->unk14 = D_800CBD6E;
        node->unk15 = D_800CBD6F;
        gTextQueueTail = node;
        if (node->icon != 0) {
            node->red = 0xFF;
            node->green = 0xFF;
            node->blue = 0xFF;
        } else {
            gQueuedTextY += 11; /* line height */
        }
    }
    gQueuedTextAnchor = NULL;
}

/** Set the scale of subsequently queued text. */
void text_setScale(f32 scale) {
    gQueuedTextScale = scale;
}

/** Attach the next queued text to an object, `yOffset` above its position. */
void text_setAnchor(Actor *object, s32 yOffset) {
    gQueuedTextFlags |= TEXT_FLAG_CENTERED;
    gQueuedTextFlags = gQueuedTextFlags; // matching: IDO re-stores the flags byte after the OR
    gQueuedTextAnchorHeight = yOffset;
    gQueuedTextAnchor = object;
}

/** Set the screen position of subsequently queued text. */
void text_setPosition(s16 x, s16 y) {
    gQueuedTextX = x;
    gQueuedTextY = y;
}

/** Set the colour of subsequently queued text. */
void text_setColor(u8 red, u8 green, u8 blue, u8 alpha) {
    gQueuedTextRed = red;
    gQueuedTextGreen = green;
    gQueuedTextBlue = blue;
    gQueuedTextAlpha = alpha;
}

/**
 * Draw every queued text/icon node (projecting anchored ones to the screen), free the queue and
 * reset the text state to its defaults. text_clearQueue calls it with NULL on an emptied queue.
 */
Gfx *text_drawQueue(Gfx *gfx) {
    s32 width;
    s32 height;
    s32 lines;
    s32 icon;
    TextQueueNode *node;
    TextQueueNode *next;
    Actor *anchor;
    u32 visible;
    s32 shadow;
    f32 scale;
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 screenW;
    s32 pulse;
    s32 frame;

    node = gTextQueueHead;
    // matching: D_80085CD0++, read as word 4 past D_80085CC0 (a plain ++ keeps one address register for
    // the load and the store; the original addresses them separately)
    D_80085CD0 = ((s32 *)D_80085CC0)[4] + 1;
    if (node != NULL) {
        gSPDisplayList(gfx++, gText2DSetupDL);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x00504240);
        while (node != NULL) {
            if (node->icon == 0) {
                visible = 1;
                if (node->flags & TEXT_FLAG_SCREEN_SPACE) {
                    node->flags &= ~TEXT_FLAG_SCREEN_SPACE;
                } else {
                    node->x = node->x * gHudScaleX;
                    node->y = node->y * gHudScaleY;
                }
                if (node->flags & TEXT_FLAG_SHADOW) {
                    shadow = 1;
                    node->flags &= ~TEXT_FLAG_SHADOW;
                } else {
                    shadow = 0;
                }
                if (node->anchor != NULL) {
                    anchor = node->anchor;
                    gCurrentView = 0;
                    visible = sprite_project(anchor->xpos, anchor->ypos + node->y, anchor->zpos,
                                            &screenX, &screenY, &screenZ, &screenW, 30000.0f);
                    if (visible) {
                        node->x = screenX;
                        node->y = screenY;
                    }
                }
                if (node->flags == TEXT_FLAG_CENTERED || node->unk15 != 0) {
                    text_measureString(node->text, &width, &height, &lines);
                    width *= node->scale;
                }
                if (node->flags == TEXT_FLAG_CENTERED) {
                    node->x -= width >> 1;
                }
                if (visible) {
                    scale = node->scale * 4096.0f;
                    if (shadow) {
                        gfx = text_drawString(gfx, node->x + 1, node->y + 1, node->text, 0, 0, 0,
                                            node->alpha, scale, scale, strlen((char *)node->text));
                    }
                    gfx = text_drawString(gfx, node->x, node->y, node->text, node->red,
                                        node->green, node->blue, node->alpha, scale,
                                        scale, strlen((char *)node->text));
                }
                gDPPipeSync(gfx++);
            } else {
                icon = node->icon - 1;
                scale = gHudIcons[icon].scale * 0.0078125f;
                gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0, TEXEL0, 0, ENVIRONMENT, 0,
                                  ENVIRONMENT, 0, TEXEL0, 0);
                if (gHudIcons[icon].flags & 2) {
                    gHudSprite.width = 16;
                    gHudSprite.height = 16;
                }
                if (gHudIcons[icon].flags & 1) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, node->red, node->green, node->blue,
                                   node->alpha);
                    gHudSprite.texId = gHudIcons[icon - 1].image;
                    gfx = hud_drawSprite(gfx, &gHudSprite, node->x, node->y, gHudIcons[icon - 1].width,
                                        gHudIcons[icon - 1].height, scale, 0);
                    /* pulse the alpha: scale by (63 + 3 * a 0..63..0 triangle wave) / 256 */
                    pulse = (gTotalTicks * 2) & 0x7F;
                    if (pulse >= 64) {
                        pulse = 127 - pulse;
                    }
                    pulse = (node->alpha * (pulse + pulse + pulse + 63)) >> 8; // matching: `pulse * 3` schedules differently
                    if (pulse >= 256) {
                        pulse = 255;
                    }
                    node->alpha = pulse;
                }
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, node->red, node->green, node->blue,
                               node->alpha);
                gHudSprite.texId = gHudIcons[icon].image;
                if (icon == 25) {
                    frame = (D_80085CD0 >> 1) % 10;
                    pulse = frame;
                    if (frame >= 6) {
                        pulse = 10 - frame;
                    }
                    gHudSprite.texId = gHudIcons[icon].image + pulse;
                }
                gfx = hud_drawSprite(gfx, &gHudSprite, node->x, node->y, gHudIcons[icon].width,
                                    gHudIcons[icon].height, scale, 0);
                gSPDisplayList(gfx++, gText2DSetupDL);
                gDPSetOtherMode(gfx++, 0x002C3F, 0x00504240);
                if (gHudIcons[icon].flags & 2) {
                    gHudSprite.width = 32;
                    gHudSprite.height = 32;
                }
            }
            next = node->next;
            heap_free(node);
            node = next;
        }
        gDPPipeSync(gfx++);
    }
    gQueuedTextRed = 255;
    gQueuedTextGreen = 255;
    gQueuedTextBlue = 255;
    gQueuedTextAlpha = 255;
    D_800CBD6C = 0;
    D_800CBD6D = 0;
    D_800CBD6E = 0;
    D_800CBD6F = 0x80;
    gQueuedTextX = 10;
    gQueuedTextY = 10;
    gQueuedTextScale = 1.0f;
    gQueuedTextFlags = 0;
    gTextQueueHead = NULL;
    gTextQueueTail = 0;
    gQueuedTextAnchor = NULL;
    return gfx;
}

/*
 * Ring buffer of length-prefixed messages (MsgRing):
 *   unk0 = buffer, unk4 = capacity, unk8 = read offset, unkC = write offset.
 */

/** Initialise a ring buffer over `buffer` of `capacity` bytes. */
void text_ringInit(MsgRing *ring, u8 *buffer, s32 capacity) {
    if (ring != NULL) {
        ring->buffer = buffer;
        ring->capacity = capacity;
        ring->writeOffset = 0;
        ring->readOffset = 0;
    }
}

/** Copy `size` bytes from `src` into the ring at `offset`, wrapping; returns the new offset. */
s32 text_ringWrite(u8 *buffer, s32 capacity, s32 offset, u8 *src, s32 size) {
    s32 chunk;

    while (size != 0) {
        if (capacity < offset + size) {
            chunk = capacity - offset;
        } else {
            chunk = size;
        }
        memcpy(buffer + offset, src, chunk);
        offset += chunk;
        src += chunk;
        size -= chunk;
        if (offset >= capacity) {
            offset = 0;
        }
    }
    return offset;
}

/** Copy `size` bytes out of the ring at `offset` into `dst`, wrapping; returns the new offset. */
s32 text_ringRead(u8 *buffer, s32 capacity, s32 offset, u8 *dst, s32 size) {
    s32 chunk;

    while (size != 0) {
        if (capacity < offset + size) {
            chunk = capacity - offset;
        } else {
            chunk = size;
        }
        memcpy(dst, buffer + offset, chunk);
        offset += chunk;
        dst += chunk;
        size -= chunk;
        if (offset >= capacity) {
            offset = 0;
        }
    }
    return offset;
}

/** Advance `offset` past `size` bytes of the ring, wrapping; returns the new offset. */
s32 text_ringSkip(u8 *buffer, s32 capacity, s32 offset, s32 size) {
    s32 chunk;

    while (size != 0) {
        if (capacity < offset + size) {
            chunk = capacity - offset;
        } else {
            chunk = size;
        }
        offset += chunk;
        size -= chunk;
        if (offset >= capacity) {
            offset = 0;
        }
    }
    return offset;
}

/** Push a `size`-byte message; returns 1 if it does not fit, 0 otherwise. */
s32 text_ringPush(MsgRing *ring, u8 *message, s32 size) {
    s32 writeOffset;
    s32 readOffset;

    if (size != 0 && message != NULL) {
        size += 4;
        size = (size + 3) & ~3;
        writeOffset = ring->writeOffset;
        readOffset = ring->readOffset;
        if (writeOffset < readOffset) {
            if (writeOffset + size >= readOffset) {
                return 1;
            }
        } else if (writeOffset + size - ring->capacity >= readOffset) {
            return 1;
        }
        size -= 4;
        writeOffset = text_ringWrite(ring->buffer, ring->capacity, writeOffset, (u8 *)&size, 4);
        ring->writeOffset = text_ringWrite(ring->buffer, ring->capacity, writeOffset, message, size);
    }
    return 0;
}

/**
 * Pop the next message into `dst` (at most `dstSize` bytes; a longer message is truncated and
 * NUL-terminated). Returns the message's stored size, or 0 if the ring is empty.
 */
s32 text_ringPop(MsgRing *ring, u8 *dst, s32 dstSize) {
    s32 size; // matching: the length is compared from a register copy, not reloaded
    s32 messageSize;
    s32 readOffset;
    s32 offset;

    messageSize = 0;
    readOffset = ring->readOffset;
    if (readOffset == ring->writeOffset) {
        return 0;
    }
    offset = text_ringRead(ring->buffer, ring->capacity, readOffset, (u8 *)&messageSize, 4);
    size = messageSize;
    if (dstSize < size) {
        dstSize -= 1;
        offset = text_ringRead(ring->buffer, ring->capacity, offset, dst, dstSize);
        dst[dstSize] = 0;
        offset = text_ringSkip(ring->buffer, ring->capacity, offset, messageSize - dstSize);
    } else if (size != 0) {
        offset = text_ringRead(ring->buffer, ring->capacity, offset, dst, messageSize);
    }
    ring->readOffset = offset;
    return messageSize;
}
