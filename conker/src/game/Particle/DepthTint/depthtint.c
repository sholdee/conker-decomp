#include <ultra64.h>
#include "conker/particle.h"

/*
 * Computes an RGB tint (0-255 per channel) from a z coordinate: white for z > 0, fading to
 * (172, 142, 82) at z = -1463.2, then on towards (122, 100, 83) over the next ~1970 units.
 */
void particle_getDepthTint(f32 z, f32 *r, f32 *g, f32 *b) {
    f32 t;

    if (z > 0.0f) {
        *r = 255.0f;
        *g = 255.0f;
        *b = 255.0f;
        return;
    }

    if (z > -1463.202271f) {
        t = -z * 0.0006834325031f;
        *r = 255.0f - t * 83.0f;
        *g = 255.0f - t * 113.0f;
        *b = 255.0f - t * 173.0f;
        return;
    }

    t = -(z + 1463.202271f) * 0.0005075327354f;
    *r = 172.0f - t * 50.0f;
    *g = 142.0f - t * 42.0f;
    *b = 82.0f - t * -1.0f;
}
