#include <ultra64.h>
#include "libc/stdarg.h"

#define func_15043BB8 func_15043BB8_header_mismatch
#include "functions.h"
#undef func_15043BB8
#include "variables.h"

void func_15042ECC();
Gfx *func_15043384(Gfx *gfx);
s32 strlen(const char *);

typedef struct Game70200Anchor {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
} Game70200Anchor;

typedef struct Game70200TextNode {
    Game70200Anchor *attachedObject;
    f32 scale;
    s16 x;
    s16 yOrVerticalOffset;
    u8 flagsRaw;
    u8 kindSelector;
    u8 primaryRed;
    u8 primaryGreen;
    u8 primaryBlue;
    u8 primaryAlpha;
    u8 field_12;
    u8 field_13;
    u8 field_14;
    u8 field_15;
    u8 pad_16[2];
    struct Game70200TextNode *next;
    u8 text[0x40];
} Game70200TextNode;

void *allocate_memory(s32, s32, s32, s32);
s32 func_151EFF94(u8 *, const u8 *, ...);
extern u8 D_80085CC0[];
extern u8 D_80085CC4;
extern s32 D_80098B90;
extern s32 D_80098B94;
extern Game70200TextNode *D_800CBD68;
extern u8 D_800CBD6C;
extern u8 D_800CBD6D;
extern u8 D_800CBD6E;
extern u8 D_800CBD6F;



/** Discard all queued text nodes and reset the text state. */
void func_15042D50(void) {
    D_800CBD64 = 0;
    func_15043384(0);
}

/** Set the flags used by subsequently queued text. */
void func_15042D78(u8 flags) {
    D_800CBD74 = flags;
}

/** Queue formatted text at (x, y) with the given flags; see func_15042ECC for the format. */
void func_15042D94(s32 x, s32 y, u8 flags, s32 format, ...) {
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
    func_15042ECC(format, argumentWords);
}

/** Queue formatted text at the current text position. */
void func_15042E3C(s32 format, ...) {
    va_list args;
    s32 argumentWords[16];
    s32 i;

    va_start(args, format);
    for (i = 0; i < 16; i++) {
        argumentWords[i] = va_arg(args, s32);
    }
    func_15042ECC(format, argumentWords);
}

void func_15042ECC(u8 *format, s32 *argumentWords) {
    struct {
        Game70200TextNode *node;
        s32 pad;
        s32 finished;
        s32 pad2;
    } local;
    Game70200TextNode *node;
    u8 *out;
    s32 argumentIndex;
    s32 conversionIndex;
    u8 ch;
    u8 flags;
    u8 *conversion;

    argumentIndex = 0;
    conversion = &D_80085CC4;
    while (*format != 0) {
        local.node = allocate_memory(sizeof(Game70200TextNode), 1, 0, 1);
        if (local.node == 0) {
            return;
        }
        local.node->next = 0;
        local.finished = 0;
        if (D_800CBD68 == 0) {
            D_800CBD64 = (s32)local.node;
        } else {
            D_800CBD68->next = local.node;
        }
        out = local.node->text;
        local.node->kindSelector = 0;
        do {
            ch = *format;
            if (ch == '%') {
                format++;
                *conversion = '%';
                for (conversionIndex = 1; conversionIndex != 0; conversionIndex++, format++) {
                    u8 *destination = &(&D_80085CC4)[conversionIndex];

                    destination[0] = *format;
                    destination[1] = 0;
                    switch (destination[0]) {
                    case 'X':
                    case 'd':
                    case 'x':
                        func_151EFF94(out, &D_80085CC4,
                                     argumentWords[argumentIndex++]);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    case 'F':
                    case 'f': {
                        f32 *number = (f32 *)argumentWords[argumentIndex++];
                        f32 value = *number;

                        func_151EFF94(out, D_80085CC0, &D_80098B90,
                                     &D_80098B94,
                                     (f64)value);
                        while (*out != 0) {
                            out++;
                        }
                        conversionIndex = -1;
                        break;
                    }
                    case 's':
                        func_151EFF94(out, &D_80085CC4,
                                     argumentWords[argumentIndex++]);
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
                    default:
                        break;
                    }
                }
            } else if (ch == '#') {
                local.node->kindSelector = argumentWords[argumentIndex++];
                format += 2;
            } else if (ch == '\n') {
                format++;
                local.finished = 1;
            } else if (ch == 0) {
                local.finished = 1;
            } else {
                *out = ch;
                format++;
                out++;
                *out = 0;
            }
        } while (local.finished == 0);

        node = local.node;
        node->flagsRaw = D_800CBD74;
        node->attachedObject = (Game70200Anchor *)D_800CBD78;
        if (node->attachedObject != 0) {
            node->yOrVerticalOffset = D_800CBD7C;
            flags = node->flagsRaw | 1;
            node->flagsRaw = (node->flagsRaw = flags);
            if (D_8008FD90 >= 2) {
                node->scale = D_800CBD80 + D_800CBD80;
            } else {
                node->scale = D_800CBD80;
            }
        } else {
            node->x = D_800CBD70;
            node->yOrVerticalOffset = D_800CBD72;
            node->scale = D_800CBD80;
        }
        node->primaryRed = D_800CBD60;
        node->primaryGreen = D_800CBD61;
        node->primaryBlue = D_800CBD62;
        node->primaryAlpha = D_800CBD63;
        node->field_12 = D_800CBD6C;
        node->field_13 = D_800CBD6D;
        node->field_14 = D_800CBD6E;
        node->field_15 = D_800CBD6F;
        D_800CBD68 = node;
        if (node->kindSelector != 0) {
            node->primaryRed = 0xFF;
            node->primaryGreen = 0xFF;
            node->primaryBlue = 0xFF;
        } else {
            D_800CBD72 += 0xB;
        }
    }
    D_800CBD78 = 0;
}

/** Set the scale of subsequently queued text. */
void func_150432BC(f32 scale) {
    D_800CBD80 = scale;
}

/** Attach the next queued text to an object, `yOffset` above its position. */
void func_150432CC(s32 object, s32 yOffset) {
    D_800CBD74 |= 1;
    D_800CBD74 = D_800CBD74; // matching: IDO re-stores the flags byte after the OR
    D_800CBD7C = yOffset;
    D_800CBD78 = object;
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

typedef struct Game70200Glyph {
    u8 width;
    u8 height;
    u8 scale;
    u8 flags;
    s32 image;
} Game70200Glyph;

typedef struct Game70200Sprite {
    s32 image;
    s16 pad4;
    s16 width;
    s16 height;
} Game70200Sprite;

extern Gfx D_800859A0[];
extern Game70200Glyph D_800859E0[];
extern Game70200Glyph D_80085AA8;
extern s32 D_80085CD0;
extern s32 D_800BE9AC;
extern Game70200Sprite D_80090060;
extern f32 D_8008FE1C;
extern f32 D_8008FE20;
void func_10004074(void *);
void func_150428D4(u8 *, s32 *, s32 *, s32 *);
s32 func_1509563C(f32, f32, f32, f32 *, f32 *, f32 *, f32 *, f32);
Gfx *func_150417AC(Gfx *, f32, f32, u8 *, s32, s32, s32, s32, f32, f32, s32);
Gfx *func_151ED430(Gfx *, void *, s32, s32, s32, s32, f32, s32);

#define GFX_RAW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }

Gfx *func_15043384(Gfx *gfx) {
    s32 width;
    s32 height;
    s32 lines;
    s32 kind;
    Game70200TextNode *node;
    Game70200TextNode *next;
    Game70200Anchor *anchor;
    u32 visible;
    s32 shadow;
    f32 scale;
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 screenW;
    s32 v;
    s32 frame;

    node = (Game70200TextNode *)D_800CBD64;
    D_80085CD0 = ((s32 *)D_80085CC0)[4] + 1;
    if (node != NULL) {
        gSPDisplayList(gfx++, D_800859A0);
        GFX_RAW(gfx++, 0xEF002C3F, 0x504240);
        while (node != NULL) {
            if (node->kindSelector == 0) {
                visible = 1;
                if (node->flagsRaw & 0x40) {
                    node->flagsRaw &= ~0x40;
                } else {
                    node->x = node->x * D_8008FE1C;
                    node->yOrVerticalOffset = node->yOrVerticalOffset * D_8008FE20;
                }
                if (node->flagsRaw & 0x80) {
                    shadow = 1;
                    node->flagsRaw &= ~0x80;
                } else {
                    shadow = 0;
                }
                if (node->attachedObject != NULL) {
                    anchor = node->attachedObject;
                    D_80082FA4 = 0;
                    visible = func_1509563C(anchor->x, anchor->y + node->yOrVerticalOffset, anchor->z,
                                            &screenX, &screenY, &screenZ, &screenW, 30000.0f);
                    if (visible) {
                        node->x = screenX;
                        node->yOrVerticalOffset = screenY;
                    }
                }
                if (node->flagsRaw == 1 || node->field_15 != 0) {
                    func_150428D4(node->text, &width, &height, &lines);
                    width = width * node->scale;
                }
                if (node->flagsRaw == 1) {
                    node->x -= width >> 1;
                }
                if (visible) {
                    scale = node->scale * 4096.0f;
                    if (shadow) {
                        gfx = func_150417AC(gfx, node->x + 1, node->yOrVerticalOffset + 1, node->text, 0, 0, 0,
                                            node->primaryAlpha, scale, scale, strlen(node->text));
                    }
                    gfx = func_150417AC(gfx, node->x, node->yOrVerticalOffset, node->text, node->primaryRed,
                                        node->primaryGreen, node->primaryBlue, node->primaryAlpha, scale,
                                        scale, strlen(node->text));
                }
                gDPPipeSync(gfx++);
            } else {
                kind = node->kindSelector - 1;
                scale = D_800859E0[kind].scale * 0.0078125f;
                GFX_RAW(gfx++, 0xFC12D225, 0xFFA7FFFF);
                if (D_800859E0[kind].flags & 2) {
                    D_80090060.width = 0x10;
                    D_80090060.height = 0x10;
                }
                if (D_800859E0[kind].flags & 1) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, node->primaryRed, node->primaryGreen, node->primaryBlue,
                                   node->primaryAlpha);
                    D_80090060.image = D_800859E0[kind - 1].image;
                    gfx = func_151ED430(gfx, &D_80090060, node->x, node->yOrVerticalOffset, D_800859E0[kind - 1].width,
                                        D_800859E0[kind - 1].height, scale, 0);
                    v = (D_800BE9AC * 2) & 0x7F;
                    if (v >= 0x40) {
                        v = 0x7F - v;
                    }
                    v = (node->primaryAlpha * (v + v + v + 0x3F)) >> 8;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    node->primaryAlpha = v;
                }
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, node->primaryRed, node->primaryGreen, node->primaryBlue,
                               node->primaryAlpha);
                D_80090060.image = D_800859E0[kind].image;
                if (&D_80085AA8 == &D_800859E0[kind]) {
                    frame = (D_80085CD0 >> 1) % 10;
                    v = frame;
                    if (frame >= 6) {
                        v = 10 - frame;
                    }
                    D_80090060.image = D_800859E0[kind].image + v;
                }
                gfx = func_151ED430(gfx, &D_80090060, node->x, node->yOrVerticalOffset, D_800859E0[kind].width,
                                    D_800859E0[kind].height, scale, 0);
                gSPDisplayList(gfx++, D_800859A0);
                GFX_RAW(gfx++, 0xEF002C3F, 0x504240);
                if (D_800859E0[kind].flags & 2) {
                    D_80090060.width = 0x20;
                    D_80090060.height = 0x20;
                }
            }
            next = node->next;
            func_10004074(node);
            node = next;
        }
        gDPPipeSync(gfx++);
    }
    D_800CBD60 = 0xFF;
    D_800CBD61 = 0xFF;
    D_800CBD62 = 0xFF;
    D_800CBD63 = 0xFF;
    D_800CBD6C = 0;
    D_800CBD6D = 0;
    D_800CBD6E = 0;
    D_800CBD6F = 0x80;
    D_800CBD70 = 10;
    D_800CBD72 = 10;
    D_800CBD80 = 1.0f;
    D_800CBD74 = 0;
    D_800CBD64 = 0;
    D_800CBD68 = 0;
    D_800CBD78 = 0;
    return gfx;
}

/*
 * Ring buffer of length-prefixed messages (struct105):
 *   unk0 = buffer, unk4 = capacity, unk8 = read offset, unkC = write offset.
 */

/** Initialise a ring buffer over `buffer` of `capacity` bytes. */
void func_15043A00(struct105 *ring, s32 buffer, s32 capacity) {
    if (ring != NULL) {
        ring->unk0 = buffer;
        ring->unk4 = capacity;
        ring->unkC = 0;
        ring->unk8 = 0;
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
s32 func_15043BB8(struct105 *ring, u8 *message, s32 size) {
    s32 writeOffset;
    s32 readOffset;

    if (size != 0 && message != NULL) {
        size += 4;
        size = (size + 3) & ~3;
        writeOffset = ring->unkC;
        readOffset = ring->unk8;
        if (writeOffset < readOffset) {
            if (writeOffset + size >= readOffset) {
                return 1;
            }
        } else if (writeOffset + size - ring->unk4 >= readOffset) {
            return 1;
        }
        size -= 4;
        writeOffset = func_15043A20((u8 *)ring->unk0, ring->unk4, writeOffset, (u8 *)&size, 4);
        ring->unkC = func_15043A20((u8 *)ring->unk0, ring->unk4, writeOffset, message, size);
    }
    return 0;
}

/**
 * Pop the next message into `dst` (at most `dstSize` bytes; a longer message is truncated and
 * NUL-terminated). Returns the message's stored size, or 0 if the ring is empty.
 */
s32 func_15043CA4(struct105 *ring, u8 *dst, s32 dstSize) {
    s32 size; // matching: the length is compared from a register copy, not reloaded
    s32 messageSize;
    s32 readOffset;
    s32 offset;

    messageSize = 0;
    readOffset = ring->unk8;
    if (readOffset == ring->unkC) {
        return 0;
    }
    offset = func_15043AC8((u8 *)ring->unk0, ring->unk4, readOffset, (u8 *)&messageSize, 4);
    size = messageSize;
    if (dstSize < size) {
        dstSize -= 1;
        offset = func_15043AC8((u8 *)ring->unk0, ring->unk4, offset, dst, dstSize);
        dst[dstSize] = 0;
        offset = func_15043B70((u8 *)ring->unk0, ring->unk4, offset, messageSize - dstSize);
    } else if (size != 0) {
        offset = func_15043AC8((u8 *)ring->unk0, ring->unk4, offset, dst, messageSize);
    }
    ring->unk8 = offset;
    return messageSize;
}
