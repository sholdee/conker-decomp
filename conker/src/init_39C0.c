#include <ultra64.h>

/* This file's .bss, defined before the headers: IDO lays out .bss in the order it first sees each name. */
f32 D_800380A0; // screen width / 292
f32 D_800380A4; // screen height / 216

#include "functions.h"
#include "variables.h"
#include "vimodes.h"

/* NTSC: a 292-pixel, 16-bit, anti-aliased mode without gamma correction (it keeps the LPN1 type id). */
static OSViMode D_8002AB90 = {
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
static OSViMode D_8002ABE0 = {
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

void func_100039C0(void) {
    OSViMode *mode;

    D_800BE620 = 292; // screen width px
    D_800BE624 = 216; // screen height px
    D_800380A0 =  D_800BE620 / 292.0f;
    D_800380A4 =  D_800BE624 / 216.0f;
    D_800BE9C4 = allocate_memory(D_800BE620 * D_800BE624 * 2, 255, 3, 0);
    func_10003ACC(0, 0, 0);
    func_15015FBC(D_800BE620, D_800BE624);
    if (D_80000300 == 2) {
        mode = &D_8002ABE0;
    } else {
        mode = &D_8002AB90;
    }
    osViSetMode(mode);
    osViSwapBuffer(D_8002AAE8[D_800BE9C0 ^ 1]);
}

void func_10003ACC(s32 red, s32 green, s32 blue) {
    u16 *pixels;
    u16 *second;
    u32 byteCount;
    s32 count;
    s32 i;
    u32 color;

    byteCount = D_800BE620 * D_800BE624 * 2;
    pixels = D_8002AAE8[0];
    if (pixels != 0) {
        color = ((red << 8) & 0xF800) | ((green << 3) & 0x7C0) | ((blue >> 2) & 0x3E) | 1;
        count = (s32)byteCount >> 1;
        second = pixels;
        for (i = 0; i < count; i++) {
            // matching: the index through an array-pointer view; &second[i + 1] makes a longer loop
            pixels = &(*(u16 (*)[])second)[i + 1];
            pixels[-1] = color;
        }
        second = D_8002AAE8[1];
        for (i = 0; i < count; i++) {
            second[i] = color;
        }
    }
}
