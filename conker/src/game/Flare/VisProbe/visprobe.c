#include <ultra64.h>
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/view.h"

/* Copies 4 pixels starting at (x, y) of the z-buffer into zDest and of the current frame buffer into
 * colorDest, using copy-mode texture rectangles that load one screen row of view's width. */
Gfx *flare_copyProbePixels(Gfx *gfx, s32 x, s32 y, s32 unused, void *zDest, void *colorDest, s16 view) {
    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, 0, 0);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    gDPSetCycleType(gfx++, G_CYC_COPY);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    gDPSetTextureLUT(gfx++, G_TT_NONE);
    gDPSetColorDither(gfx++, G_CD_DISABLE);
    gDPSetAlphaDither(gfx++, G_AD_DISABLE);
    gDPLoadTextureBlock(gfx++, gZBuffer + y * gScreenWidth, G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)gViews[view].width, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, zDest);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, (s32)gViews[view].width, 1);
    gSPTextureRectangle(gfx++, 0, 0, 3 << 2, 1 << 2, G_TX_RENDERTILE, x << 5, 0, 4 << 10, 1 << 10);
    gDPLoadTextureBlock(gfx++, (u8 *)gFramebuffers[gDrawBufferIndex] + (y * gScreenWidth * 2), G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)gViews[view].width, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, colorDest);
    gSPTextureRectangle(gfx++, 0, 0, 3 << 2, 1 << 2, G_TX_RENDERTILE, x << 5, 0, 4 << 10, 1 << 10);

    gfx = view_setScissor(view_setColorImage(gfx), gCurrentView, 0, 0, 0, 0);
    gDPSetTexturePersp(gfx++, G_TP_PERSP);
    return gfx;
}

/* A world point whose visibility is tested by sampling the z-buffer (up to 3 at once). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ u8 state; // 0 free, 3 queued, else frame buffer index + 1 it was sampled in
    /* 0x10 */ ProbeData data;
    /* 0x34 */ s8 callback; // index into gProbeVisibleCallbacks, -1 for none
    /* 0x38 */ s32 unk38;
} Probe;

s32 gProbeDepths[3]; // each probe's projected depth
u16 gProbeZSamples[3][4]; // each probe's z-buffer sample
u16 gProbeColourSamples[3][4]; // each probe's frame buffer sample
s32 gLastProbeColour; // the last visible probe's frame buffer sample
Probe gProbes[3];

/* Runs the visibility probes (view 0 only): a probe sampled last frame calls its callback with the sampled
 * pixel color when the z-buffer depth there is within 300 of its own depth; a newly queued probe is projected
 * to the screen and, when inside the view, gets its z-buffer and frame buffer pixels copied for next frame. */
Gfx *flare_runProbes(Gfx *gfx, s32 view) {
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    Probe *probe;
    s32 i;
    u16 zraw;
    s8 callback;
    ZDecode *fmt;
    u32 depth;
    s32 color;
    s32 depthDiff;
    s32 r;
    s32 g;
    s32 b;
    s32 never; // matching: see the dead guard at the end of the second loop
    s32 x;
    s32 y;

    probe = gProbes;
    i = 0;
    never = 0;
    if (view != 0) {
        return gfx;
    }
    // matching: a do-while (no pretest edge) and the dead guard below set uopt's register priorities
    do {
        if (probe->state == gDrawBufferIndex + 1) {
            zraw = gProbeZSamples[i][0];
            fmt = &gZDecodeTable[zraw >> 13];
            depth = (fmt->base + (((zraw >> 2) & 0x7FF) << fmt->shift)) >> 3;
            {
            s32 d;
            d = depth - gProbeDepths[i];
            depthDiff = (d < 0) ? -d : d;
            }
            if (depthDiff < 300) {
                callback = probe->callback;
                color = gProbeColourSamples[i][0];
                r = (gProbeColourSamples[i][0] >> 8) & 0xF8;
                g = (gProbeColourSamples[i][0] >> 3) & 0xF8;
                b = (gProbeColourSamples[i][0] << 2) & 0xF8;
                gLastProbeColour = color;
                if (callback != 0xFFFFFFFF) { // matching: != -1 compiles differently
                    gProbeVisibleCallbacks[callback](probe, r, g, b);
                }
            }
            probe->state = 0;
        }
        i++;
        probe++;
    } while (i < 3);
    for (i = 0; i < 3; i++) {
        if (gProbes[i].state == 3) {
            // matching: byte offsets here (not gProbeZSamples[i]) keep IDO's shared i * 8 address
            *(u16 *)((u8 *)gProbeZSamples + i * 8) = 0xFFFF;
            gProbes[i].state = gDrawBufferIndex + 1;
            view_projectPoint(view, gProbes[i].x, gProbes[i].y, gProbes[i].z, &screenX, &screenY, &screenZ);
            x = screenX;
            y = screenY;
            gProbeDepths[i] = screenZ;
            if (x >= (s32)gViews[view].left && x < (s32)gViews[view].right && y >= (s32)gViews[view].top &&
                y < (s32)gViews[view].bottom) {
                // matching: byte offsets, as at the store above
                gfx = flare_copyProbePixels(gfx, x, y, 1, (u8 *)gProbeZSamples + i * 8, (u8 *)gProbeColourSamples + i * 8, view);
            }
            // matching: dead guard; gives 0xFFFF a 2nd reference so it outranks view for a register
            if (never != 0) {
                *(u16 *)((u8 *)gProbeZSamples + i * 8) = 0xFFFF;
            }
        }
    }
    return gfx;
}

/* Queues a visibility probe at (x, y, z); returns 1 when all probe slots are in use. */
s32 flare_queueProbe(f32 x, f32 y, f32 z, ProbeData *data, s8 callback, s32 arg5) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (gProbes[i].state == 0) {
            gProbes[i].x = x;
            gProbes[i].y = y;
            gProbes[i].state = 3;
            gProbes[i].z = z;
            gProbes[i].callback = callback;
            gProbes[i].unk38 = arg5;
            if (data != NULL) {
                gProbes[i].data = *data;
            }
            return 0;
        }
    }
    return 1;
}
