#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/flare.h describes it. */
u8 gScreenFlash;

#include "functions.h"
#include "variables.h"

#define WGFX15172F70(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *flare_drawScreenFlash(Gfx *arg0) {
    u32 color;

    color = gScreenFlash;
    if (color != 0) {
        WGFX15172F70(arg0++, 0xD9E0FFFE, 0);
        WGFX15172F70(arg0++, 0xD9FFFFFF, 0x00200004);
        WGFX15172F70(arg0++, 0xE7000000, 0);
        WGFX15172F70(arg0++, 0xE3000A01, 0);
        WGFX15172F70(arg0++, 0xFA000000, (color & 0xFF) | ~0xFF);
        WGFX15172F70(arg0++, 0xE200001C, 0x00504340);
        WGFX15172F70(arg0++, 0xE3000C00, 0);
        WGFX15172F70(arg0++, 0xE3000F00, 0);
        WGFX15172F70(arg0++, 0xFCFFFFFF, 0xFFFDF6FB);
        WGFX15172F70(arg0++,
            0xF6000000 | ((((u32)gViews->bottom) & 0x3FF) << 2) | ((((u32)gViews->right) & 0x3FF) << 14),
            ((((u32)gViews->top) & 0x3FF) << 2) | ((((u32)gViews->left) & 0x3FF) << 14));
    }

    return arg0;
}
