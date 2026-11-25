/* SDK MPAL video mode (LAN1): L = low resolution, A = anti-aliased, N = non-interlaced,
 * 1 = 16-bit framebuffer. Data only (libultra 2.0G vimodes/vimodempallan1.c). */
#include "vimodes.h"

OSViMode osViModeMpalLan1 = {
    OS_VI_MPAL_LAN1, /* type */
    {
        /* comRegs */
        LAN1_CTRL,    /* ctrl */
        WIDTH(320),   /* width */
        BURST(57, 30, 5, 70), /* burst */
        VSYNC(525),   /* vSync */
        HSYNC(3089, 4), /* hSync */
        LEAP(3097, 3098), /* leap */
        HSTART(108, 748), /* hStart */
        SCALE(2, 0),  /* xScale */
        VCURRENT(0),  /* vCurrent */
    },
    {
        /* fldRegs */
        {
            ORIGIN(640),     /* origin */
            SCALE(1, 0),     /* yScale */
            HSTART(37, 511), /* vStart */
            BURST(4, 2, 14, 0), /* vBurst */
            VINTR(2),        /* vIntr */
        },
        {
            ORIGIN(640),
            SCALE(1, 0),
            HSTART(37, 511),
            BURST(4, 2, 14, 0),
            VINTR(2),
        },
    },
};
