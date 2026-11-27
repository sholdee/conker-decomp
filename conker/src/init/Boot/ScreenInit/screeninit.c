#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
f32 gScreenScaleX; // screen width / 292
f32 gScreenScaleY; // screen height / 216

#include "conker/boot.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/os.h"
#include "conker/view.h"
#include "vimodes.h"

/* NTSC: a 292-pixel, 16-bit, anti-aliased mode without gamma correction (it keeps the LPN1 type id). */
static OSViMode gViModeNtsc = {
    OS_VI_NTSC_LPN1, /* type */
    {
        /* comRegs */
        VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_DIVOT_ON | VI_CTRL_ANTIALIAS_MODE_1 |
            VI_CTRL_PIXEL_ADV_3, /* ctrl */
        WIDTH(292),         /* width */
        BURST(57, 34, 5, 62), /* burst */
        VSYNC(525),         /* vSync */
        HSYNC(3093, 0),  /* hSync */
        LEAP(3093, 3093), /* leap */
        START(140, 725),    /* hStart */
        SCALE(2, 0),        /* xScale */
        VCURRENT(0),        /* vCurrent */
    },
    {
        /* fldRegs */
        {
            ORIGIN(584),        /* origin: 292 pixels * 2 bytes */
            SCALE(1, 0),        /* yScale */
            START(61, 488),     /* vStart */
            BURST(4, 2, 14, 0), /* vBurst */
            VINTR(2),           /* vIntr */
        },
        {
            ORIGIN(584),        /* origin: 292 pixels * 2 bytes */
            SCALE(1, 0),        /* yScale */
            START(61, 488),     /* vStart */
            BURST(4, 2, 14, 0), /* vBurst */
            VINTR(2),           /* vIntr */
        },
    },
};

/* MPAL: the same mode with the MPAL timings. */
static OSViMode gViModeMpal = {
    OS_VI_MPAL_LPN1, /* type */
    {
        /* comRegs */
        VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_DIVOT_ON | VI_CTRL_ANTIALIAS_MODE_1 |
            VI_CTRL_PIXEL_ADV_3, /* ctrl */
        WIDTH(292),         /* width */
        BURST(57, 30, 5, 70), /* burst */
        VSYNC(525),         /* vSync */
        HSYNC(3089, 4),  /* hSync */
        LEAP(3097, 3098), /* leap */
        START(140, 725),    /* hStart */
        SCALE(2, 0),        /* xScale */
        VCURRENT(0),        /* vCurrent */
    },
    {
        /* fldRegs */
        {
            ORIGIN(584),        /* origin: 292 pixels * 2 bytes */
            SCALE(1, 0),        /* yScale */
            START(61, 488),     /* vStart */
            BURST(4, 2, 14, 0), /* vBurst */
            VINTR(2),           /* vIntr */
        },
        {
            ORIGIN(584),        /* origin: 292 pixels * 2 bytes */
            SCALE(1, 0),        /* yScale */
            START(61, 488),     /* vStart */
            BURST(4, 2, 14, 0), /* vBurst */
            VINTR(2),           /* vIntr */
        },
    },
};

void boot_initScreen(void) {
    OSViMode *mode;

    gScreenWidth = 292; // screen width px
    gScreenHeight = 216; // screen height px
    gScreenScaleX =  gScreenWidth / 292.0f;
    gScreenScaleY =  gScreenHeight / 216.0f;
    gZBuffer = allocate_memory(gScreenWidth * gScreenHeight * 2, 255, 3, 0);
    boot_clearFramebuffers(0, 0, 0);
    view_initLayouts(gScreenWidth, gScreenHeight);
    if (D_80000300 == 2) {
        mode = &gViModeMpal;
    } else {
        mode = &gViModeNtsc;
    }
    osViSetMode(mode);
    osViSwapBuffer(gFramebuffers[gDrawBufferIndex ^ 1]);
}

void boot_clearFramebuffers(s32 red, s32 green, s32 blue) {
    u16 *pixels;
    u16 *second;
    u32 byteCount;
    s32 count;
    s32 i;
    u32 color;

    byteCount = gScreenWidth * gScreenHeight * 2;
    pixels = gFramebuffers[0];
    if (pixels != 0) {
        color = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E) | 1;
        count = (s32)byteCount >> 1;
        second = pixels;
        for (i = 0; i < count; i++) {
            // matching: the index through an array-pointer view; &second[i + 1] makes a longer loop
            pixels = &(*(u16 (*)[])second)[i + 1];
            pixels[-1] = color;
        }
        second = gFramebuffers[1];
        for (i = 0; i < count; i++) {
            second[i] = color;
        }
    }
}
