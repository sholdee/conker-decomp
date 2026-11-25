#ifndef VIMODES_H
#define VIMODES_H
/* Register-field helpers of the SDK's video-mode tables (ultralib PRinternal/viint.h, 2.0G values),
 * for the osViMode* data files. */
#include <ultra64.h>

#ifndef VI_CTRL_ANTIALIAS_MODE_1
#define VI_CTRL_ANTIALIAS_MODE_1 0x00100 /* Bit [9:8] anti-alias mode 1: resample only */
#endif
#ifndef VI_CTRL_PIXEL_ADV_3
#define VI_CTRL_PIXEL_ADV_3 0x03000
#endif

#define BURST(hsync_width, color_width, vsync_width, color_start) \
    (((u32)(hsync_width) & 0xFF) | (((u32)(color_width) & 0xFF) << 8) | (((u32)(vsync_width) & 0xF) << 16) | \
     (((u32)(color_start) & 0xFFFF) << 20))
#define WIDTH(v) (v)
#define VSYNC(v) (v)
#define HSYNC(duration, leap) (((u32)(leap) << 16) | ((u32)(duration) & 0xFFFF))
#define LEAP(upper, lower) (((u32)(upper) << 16) | ((u32)(lower) & 0xFFFF))
#define START(start, end) (((u32)(start) << 16) | ((u32)(end) & 0xFFFF))
#define FTOFIX(val, i, f) ((u32)((val) * (f32)(1 << (f))) & ((1 << ((i) + (f))) - 1))
#define F210(val) FTOFIX(val, 2, 10)
#define SCALE(scaleup, off) (F210((1.0f / (f32)(scaleup))) | (F210((f32)(off)) << 16))
#define VCURRENT(v) v
#define ORIGIN(v) v
#define VINTR(v) v
#define HSTART START

#define PAL_BURST BURST(58, 35, 4, 64)
#define PAL_HSYNC HSYNC(3177, 21)
#define PAL_LEAP LEAP(3183, 3182)

#define LAN1_CTRL (VI_CTRL_TYPE_16 | VI_CTRL_GAMMA_DITHER_ON | VI_CTRL_GAMMA_ON | VI_CTRL_DIVOT_ON | \
                   VI_CTRL_ANTIALIAS_MODE_1 | VI_CTRL_PIXEL_ADV_3)

#endif
