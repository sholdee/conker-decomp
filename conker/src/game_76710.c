#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Tests a triangle against the box set up by func_150492CC. */
s32 func_15049260(Tri3f tri) {
    return func_150AAD98(tri);
}

/* Sets up the box for func_15049260: its size, half-size and the height and depth
 * ratios to its width. */
void func_150492CC(f32 width, f32 height, f32 depth) {
    D_800CC220 = width;
    D_800CC224 = height;
    D_800CC228 = depth;
    D_800CC22C = width / 2;
    D_800CC230 = height / 2;
    D_800CC234 = depth / 2;

    if (width == 0.0f) {
        width = 0.001000000047f;
    }

    D_800CC238 = height / width;
    D_800CC23C = depth / width;
}

/* Computes the plane through a triangle: the (unnormalised) normal
 * (v0 - v1) x (v0 - v2) goes to D_800CC210..D_800CC218, and its dot product with v0
 * to D_800CC21C. */
void func_15049350(Tri3f tri) {
    f32 edge1X;
    f32 edge1Y;
    f32 edge1Z;
    f32 edge2X;
    f32 edge2Y;
    f32 edge2Z;
    f32 normalX;
    f32 normalY;
    f32 normalZ;

    edge1X = tri.pts[0].x - tri.pts[1].x;
    edge1Y = tri.pts[0].y - tri.pts[1].y;
    edge1Z = tri.pts[0].z - tri.pts[1].z;
    edge2X = tri.pts[0].x - tri.pts[2].x;
    edge2Y = tri.pts[0].y - tri.pts[2].y;
    edge2Z = tri.pts[0].z - tri.pts[2].z;

    normalX = (edge1Y * edge2Z) - (edge1Z * edge2Y);
    D_800CC210 = normalX;
    normalY = (-edge1X * edge2Z) + (edge1Z * edge2X);
    D_800CC214 = normalY;
    normalZ = (edge1X * edge2Y) - (edge1Y * edge2X);
    D_800CC218 = normalZ;
    D_800CC21C = normalX * tri.pts[0].x + normalY * tri.pts[0].y + normalZ * tri.pts[0].z;
}
