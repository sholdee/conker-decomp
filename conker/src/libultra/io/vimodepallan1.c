/* SDK PAL video mode (LAN1): L = low resolution, A = anti-aliased, N = non-interlaced,
 * 1 = 16-bit framebuffer. Data only (libultra 2.0G vimodes/vimodepallan1.c). */
#include "vimodes.h"

OSViMode osViModePalLan1 = {
    OS_VI_PAL_LAN1, /* type */
    {
        /* comRegs */
        LAN1_CTRL,    /* ctrl */
        WIDTH(320),   /* width */
        PAL_BURST, /* burst */
        VSYNC(625),   /* vSync */
        PAL_HSYNC, /* hSync */
        PAL_LEAP, /* leap */
        HSTART(128, 768), /* hStart */
        SCALE(2, 0),  /* xScale */
        VCURRENT(0),  /* vCurrent */
    },
    {
        /* fldRegs */
        {
            ORIGIN(640),     /* origin */
            SCALE(1, 0),     /* yScale */
            HSTART(95, 569), /* vStart */
            BURST(107, 2, 9, 0), /* vBurst */
            VINTR(2),        /* vIntr */
        },
        {
            ORIGIN(640),
            SCALE(1, 0),
            HSTART(95, 569),
            BURST(107, 2, 9, 0),
            VINTR(2),
        },
    },
};
