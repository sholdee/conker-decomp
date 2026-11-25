/* SDK NTSC video mode (LAN1): L = low resolution, A = anti-aliased, N = non-interlaced,
 * 1 = 16-bit framebuffer. Data only (libultra 2.0G vimodes/vimodentsclan1.c). */
#include "vimodes.h"

OSViMode osViModeNtscLan1 = {
    OS_VI_NTSC_LAN1, /* type */
    {
        /* comRegs */
        LAN1_CTRL,    /* ctrl */
        WIDTH(320),   /* width */
        BURST(57, 34, 5, 62), /* burst */
        VSYNC(525),   /* vSync */
        HSYNC(3093, 0), /* hSync */
        LEAP(3093, 3093), /* leap */
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
