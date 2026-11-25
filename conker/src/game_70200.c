#include <ultra64.h>
#include "libc/stdarg.h"

#include "functions.h"
#include "variables.h"

s32 strlen(const char *);

/* One queued line of HUD text (or, when `icon` is nonzero, one icon), freed after drawing. */
typedef struct Game70200TextNode {
    Actor *anchor; /* if set, the text is drawn over this actor */
    f32 scale;
    s16 x;
    s16 y; /* height above the anchor while one is attached */
    u8 flags; /* TEXT_FLAG_* */
    u8 icon; /* 1-based index into D_800859E0; 0 = text */
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
    struct Game70200TextNode *next;
    u8 text[0x40];
} Game70200TextNode;

#define TEXT_FLAG_CENTERED 0x01 /* centre horizontally on x */
#define TEXT_FLAG_SCREEN_SPACE 0x40 /* x/y are already in screen pixels: skip the resolution scale */
#define TEXT_FLAG_SHADOW 0x80

extern u8 D_80085CC0[]; /* "%s%s", deliberately unterminated: runs on into the spec buffer below */
extern u8 D_80085CC4[]; /* conversion spec being parsed, e.g. "%2d" */
extern Game70200TextNode *D_800CBD64; /* head */
extern Game70200TextNode *D_800CBD68; /* tail */
extern u8 D_800CBD6C;
extern u8 D_800CBD6D;
extern u8 D_800CBD6E;
extern u8 D_800CBD6F;

/*
 * Text queue state (D_800CBD64/68 declared above): D_800CBD64 = head node, D_800CBD68 = tail node,
 * D_800CBD60..63 = RGBA, D_800CBD6C..6F = node unk12..unk15, D_800CBD70/72 = x/y,
 * D_800CBD74 = TEXT_FLAG_*, D_800CBD78 = anchor actor, D_800CBD7C = height above it,
 * D_800CBD80 = scale. All apply to the next queued nodes; drawing resets them.
 */

/** Drop the queued text without drawing (or freeing) it and reset the text state. */
void func_15042D50(void) {
    D_800CBD64 = NULL;
    func_15043384(NULL);
}

/** Set the flags used by subsequently queued text. */
void func_15042D78(u8 flags) {
    D_800CBD74 = flags;
}

/** Queue formatted text at (x, y) with the given flags; see func_15042ECC for the format. */
void func_15042D94(s32 x, s32 y, u8 flags, char *format, ...) {
    va_list args;
    s32 argumentWords[16];
    s32 i;

    D_800CBD74 = flags;
    D_800CBD70 = x;
    D_800CBD72 = y;
    va_start(args, format);
    for (i = 0; i < 16; i++) {
        argumentWords[i] = va_arg(args, s32);
    }
    func_15042ECC((u8 *)format, argumentWords);
}

/** Queue formatted text at the current text position. */
void func_15042E3C(char *format, ...) {
    va_list args;
    s32 argumentWords[16];
    s32 i;

    va_start(args, format);
    for (i = 0; i < 16; i++) {
        argumentWords[i] = va_arg(args, s32);
    }
    func_15042ECC((u8 *)format, argumentWords);
}

/**
 * Parse `format` into queued text nodes, one per line. Supports %d/%x/%X/%s, %f/%F (the argument
 * word is a pointer to an f32), %%, "#?" (the next argument becomes an icon node; the character
 * after '#' is skipped) and '\n' (starts a new node 11 pixels lower).
 */
void func_15042ECC(u8 *format, s32 *argumentWords) {
    s32 pad; // matching: pads place `finished` at sp+0x50 and `newNode` at sp+0x48
    s32 finished;
    s32 pad2;
    Game70200TextNode *newNode;
    Game70200TextNode *node;
    u8 *out;
    s32 argumentIndex;
    s32 conversionIndex;
    u8 *spec;
    u8 ch;

    argumentIndex = 0;
    spec = D_80085CC4;
    while (*format != 0) {
        newNode = allocate_memory(sizeof(Game70200TextNode), 1, 0, 1);
        if (newNode == NULL) {
            return;
        }
        newNode->next = NULL;
        finished = 0;
        if (D_800CBD68 == NULL) {
            D_800CBD64 = newNode;
        } else {
            D_800CBD68->next = newNode;
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

                        func_151EFF94(out, D_80085CC0, "", "", (f64)value);
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
        node->flags = D_800CBD74;
        node->anchor = D_800CBD78;
        if (node->anchor != NULL) {
            node->y = D_800CBD7C;
            node->flags |= TEXT_FLAG_CENTERED;
            node->flags = node->flags; // matching: IDO re-stores the flags byte after the OR
            if (D_8008FD90 >= 2) {
                node->scale = D_800CBD80 + D_800CBD80;
            } else {
                node->scale = D_800CBD80;
            }
        } else {
            node->x = D_800CBD70;
            node->y = D_800CBD72;
            node->scale = D_800CBD80;
        }
        node->red = D_800CBD60;
        node->green = D_800CBD61;
        node->blue = D_800CBD62;
        node->alpha = D_800CBD63;
        node->unk12 = D_800CBD6C;
        node->unk13 = D_800CBD6D;
        node->unk14 = D_800CBD6E;
        node->unk15 = D_800CBD6F;
        D_800CBD68 = node;
        if (node->icon != 0) {
            node->red = 0xFF;
            node->green = 0xFF;
            node->blue = 0xFF;
        } else {
            D_800CBD72 += 11; /* line height */
        }
    }
    D_800CBD78 = NULL;
}

/** Set the scale of subsequently queued text. */
void func_150432BC(f32 scale) {
    D_800CBD80 = scale;
}

/** Attach the next queued text to an object, `yOffset` above its position. */
void func_150432CC(s32 object, s32 yOffset) {
    D_800CBD74 |= TEXT_FLAG_CENTERED;
    D_800CBD74 = D_800CBD74; // matching: IDO re-stores the flags byte after the OR
    D_800CBD7C = yOffset;
    D_800CBD78 = (Actor *)object;
}

/** Set the screen position of subsequently queued text. */
void func_150432FC(s16 x, s16 y) {
    D_800CBD70 = x;
    D_800CBD72 = y;
}

/** Set the colour of subsequently queued text. */
void func_1504332C(u8 red, u8 green, u8 blue, u8 alpha) {
    D_800CBD60 = red;
    D_800CBD61 = green;
    D_800CBD62 = blue;
    D_800CBD63 = alpha;
}

/* HUD icon table entry (D_800859E0). */
typedef struct Game70200Glyph {
    u8 width;
    u8 height;
    u8 scale; /* 128 = 1.0 */
    u8 flags; /* 1: draw the previous entry behind it and pulse the alpha; 2: 16x16 sprite (default 32x32) */
    u8 *image; /* texture id (a symbol address), consecutive ids for animation frames */
} Game70200Glyph;

extern Game70200Glyph D_800859E0[];
extern s32 D_80085CD0;

/**
 * Draw every queued text/icon node (projecting anchored ones to the screen), free the queue and
 * reset the text state to its defaults. func_15042D50 calls it with NULL on an emptied queue.
 */
Gfx *func_15043384(Gfx *gfx) {
    s32 width;
    s32 height;
    s32 lines;
    s32 icon;
    Game70200TextNode *node;
    Game70200TextNode *next;
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

    node = D_800CBD64;
    // matching: D_80085CD0 (frame counter) is read as word 4 past D_80085CC0, i.e. D_80085CD0++
    D_80085CD0 = ((s32 *)D_80085CC0)[4] + 1;
    if (node != NULL) {
        gSPDisplayList(gfx++, D_800859A0);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x00504240);
        while (node != NULL) {
            if (node->icon == 0) {
                visible = 1;
                if (node->flags & TEXT_FLAG_SCREEN_SPACE) {
                    node->flags &= ~TEXT_FLAG_SCREEN_SPACE;
                } else {
                    node->x = node->x * D_8008FE1C;
                    node->y = node->y * D_8008FE20;
                }
                if (node->flags & TEXT_FLAG_SHADOW) {
                    shadow = 1;
                    node->flags &= ~TEXT_FLAG_SHADOW;
                } else {
                    shadow = 0;
                }
                if (node->anchor != NULL) {
                    anchor = node->anchor;
                    D_80082FA4 = 0;
                    visible = func_1509563C(anchor->xpos, anchor->ypos + node->y, anchor->zpos,
                                            &screenX, &screenY, &screenZ, &screenW, 30000.0f);
                    if (visible) {
                        node->x = screenX;
                        node->y = screenY;
                    }
                }
                if (node->flags == TEXT_FLAG_CENTERED || node->unk15 != 0) {
                    func_150428D4(node->text, &width, &height, &lines);
                    width *= node->scale;
                }
                if (node->flags == TEXT_FLAG_CENTERED) {
                    node->x -= width >> 1;
                }
                if (visible) {
                    scale = node->scale * 4096.0f;
                    if (shadow) {
                        gfx = func_150417AC(gfx, node->x + 1, node->y + 1, node->text, 0, 0, 0,
                                            node->alpha, scale, scale, strlen(node->text));
                    }
                    gfx = func_150417AC(gfx, node->x, node->y, node->text, node->red,
                                        node->green, node->blue, node->alpha, scale,
                                        scale, strlen(node->text));
                }
                gDPPipeSync(gfx++);
            } else {
                icon = node->icon - 1;
                scale = D_800859E0[icon].scale * 0.0078125f;
                gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0, TEXEL0, 0, ENVIRONMENT, 0,
                                  ENVIRONMENT, 0, TEXEL0, 0);
                if (D_800859E0[icon].flags & 2) {
                    D_80090060.width = 16;
                    D_80090060.height = 16;
                }
                if (D_800859E0[icon].flags & 1) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, node->red, node->green, node->blue,
                                   node->alpha);
                    D_80090060.texId = D_800859E0[icon - 1].image;
                    gfx = func_151ED430(gfx, &D_80090060, node->x, node->y, D_800859E0[icon - 1].width,
                                        D_800859E0[icon - 1].height, scale, 0);
                    /* pulse the alpha: scale by (63 + 3 * a 0..63..0 triangle wave) / 256 */
                    pulse = (D_800BE9AC * 2) & 0x7F;
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
                D_80090060.texId = D_800859E0[icon].image;
                if (icon == 25) {
                    frame = (D_80085CD0 >> 1) % 10;
                    pulse = frame;
                    if (frame >= 6) {
                        pulse = 10 - frame;
                    }
                    D_80090060.texId = D_800859E0[icon].image + pulse;
                }
                gfx = func_151ED430(gfx, &D_80090060, node->x, node->y, D_800859E0[icon].width,
                                    D_800859E0[icon].height, scale, 0);
                gSPDisplayList(gfx++, D_800859A0);
                gDPSetOtherMode(gfx++, 0x002C3F, 0x00504240);
                if (D_800859E0[icon].flags & 2) {
                    D_80090060.width = 32;
                    D_80090060.height = 32;
                }
            }
            next = node->next;
            func_10004074(node);
            node = next;
        }
        gDPPipeSync(gfx++);
    }
    D_800CBD60 = 255;
    D_800CBD61 = 255;
    D_800CBD62 = 255;
    D_800CBD63 = 255;
    D_800CBD6C = 0;
    D_800CBD6D = 0;
    D_800CBD6E = 0;
    D_800CBD6F = 0x80;
    D_800CBD70 = 10;
    D_800CBD72 = 10;
    D_800CBD80 = 1.0f;
    D_800CBD74 = 0;
    D_800CBD64 = NULL;
    D_800CBD68 = 0;
    D_800CBD78 = NULL;
    return gfx;
}

/*
 * Ring buffer of length-prefixed messages (MsgRing):
 *   unk0 = buffer, unk4 = capacity, unk8 = read offset, unkC = write offset.
 */

/** Initialise a ring buffer over `buffer` of `capacity` bytes. */
void func_15043A00(MsgRing *ring, u8 *buffer, s32 capacity) {
    if (ring != NULL) {
        ring->buffer = buffer;
        ring->capacity = capacity;
        ring->writeOffset = 0;
        ring->readOffset = 0;
    }
}

/** Copy `size` bytes from `src` into the ring at `offset`, wrapping; returns the new offset. */
s32 func_15043A20(u8 *buffer, s32 capacity, s32 offset, u8 *src, s32 size) {
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
s32 func_15043AC8(u8 *buffer, s32 capacity, s32 offset, u8 *dst, s32 size) {
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
s32 func_15043B70(u8 *buffer, s32 capacity, s32 offset, s32 size) {
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
s32 func_15043BB8(MsgRing *ring, u8 *message, s32 size) {
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
        writeOffset = func_15043A20(ring->buffer, ring->capacity, writeOffset, (u8 *)&size, 4);
        ring->writeOffset = func_15043A20(ring->buffer, ring->capacity, writeOffset, message, size);
    }
    return 0;
}

/**
 * Pop the next message into `dst` (at most `dstSize` bytes; a longer message is truncated and
 * NUL-terminated). Returns the message's stored size, or 0 if the ring is empty.
 */
s32 func_15043CA4(MsgRing *ring, u8 *dst, s32 dstSize) {
    s32 size; // matching: the length is compared from a register copy, not reloaded
    s32 messageSize;
    s32 readOffset;
    s32 offset;

    messageSize = 0;
    readOffset = ring->readOffset;
    if (readOffset == ring->writeOffset) {
        return 0;
    }
    offset = func_15043AC8(ring->buffer, ring->capacity, readOffset, (u8 *)&messageSize, 4);
    size = messageSize;
    if (dstSize < size) {
        dstSize -= 1;
        offset = func_15043AC8(ring->buffer, ring->capacity, offset, dst, dstSize);
        dst[dstSize] = 0;
        offset = func_15043B70(ring->buffer, ring->capacity, offset, messageSize - dstSize);
    } else if (size != 0) {
        offset = func_15043AC8(ring->buffer, ring->capacity, offset, dst, messageSize);
    }
    ring->readOffset = offset;
    return messageSize;
}
