#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/**
 * Transforms the point (x, y, z) by the view matrix `mf` and tests whether a box of half-size
 * (xExtent, yExtent) around it lies within the view volume: depth in [nearBound, farBound] and
 * view-space x/y within +-xBound / +-yBound. Returns the depth (truncated), or 0 when outside.
 */
s32 math_isBoxInView(MtxF mf, f32 x, f32 y, f32 z, f32 xExtent, f32 yExtent, f32 nearBound, f32 farBound,
                  f32 xBound, f32 yBound) {
    f32 depth;
    f32 viewX;
    f32 viewY;

    depth = -(mf[0][2] * x + mf[1][2] * y + mf[2][2] * z + mf[3][2]);
    if (depth < nearBound) {
        return 0;
    }
    if (farBound < depth) {
        return 0;
    }
    viewX = mf[0][0] * x + mf[1][0] * y + mf[2][0] * z + mf[3][0];
    if (xBound < viewX - xExtent) {
        return 0;
    }
    if (viewX + xExtent < -xBound) {
        return 0;
    }
    viewY = mf[0][1] * x + mf[1][1] * y + mf[2][1] * z + mf[3][1];
    if (yBound < viewY - yExtent) {
        return 0;
    }
    if (viewY + yExtent < -yBound) {
        return 0;
    }
    return depth;
}
