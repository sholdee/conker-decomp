#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init/Sched/SchedInit/schedinit.c).
f32 gTriPlaneNormalX; // plane of the last triangle (math_computeTriPlane): normal x
f32 gTriPlaneNormalY; // normal y
f32 gTriPlaneNormalZ; // normal z
f32 gTriPlaneDist; // normal . first vertex
f32 gTestBoxWidth; // box (math_setTestBox): width
f32 gTestBoxHeight; // height
f32 gTestBoxDepth; // depth
f32 gTestBoxHalfWidth; // half width
f32 gTestBoxHalfHeight; // half height
f32 gTestBoxHalfDepth; // half depth
f32 gTestBoxHeightRatio; // height / width
f32 gTestBoxDepthRatio; // depth / width
f32 D_800CC240[4]; // not referenced

#include "functions.h"
#include "variables.h"

/* Tests a triangle against the box set up by math_setTestBox. */
s32 math_testTriBox(Tri3f tri) {
    return math_triBoxOverlap(tri);
}

/* Sets up the box for math_testTriBox: its size, half-size and the height and depth
 * ratios to its width. */
void math_setTestBox(f32 width, f32 height, f32 depth) {
    gTestBoxWidth = width;
    gTestBoxHeight = height;
    gTestBoxDepth = depth;
    gTestBoxHalfWidth = width / 2;
    gTestBoxHalfHeight = height / 2;
    gTestBoxHalfDepth = depth / 2;

    if (width == 0.0f) {
        width = 0.001000000047f;
    }

    gTestBoxHeightRatio = height / width;
    gTestBoxDepthRatio = depth / width;
}

/* Computes the plane through a triangle: the (unnormalised) normal
 * (v0 - v1) x (v0 - v2) goes to gTriPlaneNormalX..gTriPlaneNormalZ, and its dot product with v0
 * to gTriPlaneDist. */
void math_computeTriPlane(Tri3f tri) {
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
    gTriPlaneNormalX = normalX;
    normalY = (-edge1X * edge2Z) + (edge1Z * edge2X);
    gTriPlaneNormalY = normalY;
    normalZ = (edge1X * edge2Y) - (edge1Y * edge2X);
    gTriPlaneNormalZ = normalZ;
    gTriPlaneDist = normalX * tri.pts[0].x + normalY * tri.pts[0].y + normalZ * tri.pts[0].z;
}
