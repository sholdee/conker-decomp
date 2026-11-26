#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: a 0.01745329238f literal changes the frame layout and a const f32[1] the multiply's operand
// order; the factor is a const struct in .rodata
static const struct {
    f32 degToRad;
} D_800A6C70 = { 0.01745329238f };

/*
 * Places the vertices listed in vtxIndices[0 .. count/2) at (-x, y) and those in
 * vtxIndices[count/2 .. count) at (x, y), where (x, y) is `radius` rotated by *angle
 * degrees. Then advances *angle by *velocity per frame, reversing the velocity and
 * clamping when the angle reaches maxAngle or minAngle.
 */
void func_15165C80(f32 *angle, Vtx *verts, f32 radius, f32 *velocity, f32 maxAngle, f32 minAngle, u8 *vtxIndices,
                   u8 count) {
    f32 degrees;
    f32 sinA;
    f32 sinR;
    f32 cosA;
    f32 cosR;
    f32 speed;
    s32 i;
    s16 y;
    s16 x;
    f32 radians;

    degrees = *angle;
    speed = *velocity;
    radians = degrees * D_800A6C70.degToRad;
    sinA = func_150AD78C(radians);
    sinR = sinA * radius;
    cosA = func_150AD780(radians);
    cosR = cosA * radius;
    count = count >> 1; // matching: `count >>= 1` schedules differently
    y = sinR;
    x = cosR;
    for (i = 0; i < count; i++) {
        verts[vtxIndices[i]].v.ob[0] = -x;
        verts[vtxIndices[i]].v.ob[1] = y;
        verts[vtxIndices[i + count]].v.ob[0] = x;
        verts[vtxIndices[i + count]].v.ob[1] = y;
    }
    *angle += speed * D_800BE9E4;
    if (maxAngle <= *angle) {
        *velocity = -speed;
        *angle = maxAngle;
    } else if (*angle <= minAngle) {
        *velocity = -speed;
        *angle = minAngle;
    }
}
