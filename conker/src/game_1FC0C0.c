#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define TRAIL_HAS_POS 4 /* WaveTrail.head.tmpl.flags: pos holds a valid previous position */

/* Writes the point a trail is attached to: the anchor's position if there is one, else the owner's. */
s32 func_151CEC10(Vec3f *out, Actor *owner, DescParticleHead *anchor) {
    if (anchor != NULL) {
        *out = anchor->desc.pos;
    } else {
        out->x = owner->xpos;
        out->y = owner->ypos;
        out->z = owner->zpos;
    }
    return TRUE;
}

/*
 * Follows the trail's attachment point and, once it has moved far enough, pushes it as the newest point
 * of the ring buffer `points`. Points carry an XZ normal (and that normal scaled by 51) perpendicular
 * to the trail; with WAVE_TRAIL_SMOOTH the previous point's normal is re-averaged from its neighbours.
 * Returns 0 when the owner is gone or no longer matches.
 */
s32 func_151CEC54(WaveTrail *trail) {
    WaveTrailData *info;
    WaveTrailPoint *entries;
    Actor *owner;
    DescParticleHead *anchor;
    Vec3f prev;
    Vec3f delta;
    f32 dist;
    f32 len;
    f32 inv;
    f32 dx;
    f32 dz;
    f32 nx;
    f32 nz;

    info = trail->data;
    entries = trail->points;
    owner = info->owner;
    anchor = info->anchor;
    if (owner != NULL && (owner->type == 0 || owner->unique_id != info->ownerId)) {
        return 0;
    }
    if (D_800BE9E4 > 0) {
        prev = trail->head.tmpl.pos;
        if (func_151CEC10(&trail->head.tmpl.pos, owner, anchor)) {
            if (!(trail->head.tmpl.flags & TRAIL_HAS_POS)) {
                prev = trail->head.tmpl.pos;
            } else if (!(info->flags & WAVE_TRAIL_HAS_NORMAL)) {
                dx = trail->head.tmpl.pos.x - prev.x;
                dz = trail->head.tmpl.pos.z - prev.z;
                len = sqrtf(dx * dx + dz * dz);
                if (len != 0.0f) {
                    inv = 1.0f / len;
                    info->flags |= WAVE_TRAIL_HAS_NORMAL;
                    nz = dx * inv;
                    nx = -(dz * inv);
                    info->normal.x = nx;
                    info->scaled.x = nx * 51.0f;
                    info->normal.z = nz;
                    info->scaled.z = nz * 51.0f;
                }
            }
            trail->head.tmpl.flags |= TRAIL_HAS_POS;
        } else {
            trail->head.tmpl.flags &= ~TRAIL_HAS_POS;
            return 1;
        }
        delta.x = trail->head.tmpl.pos.x - prev.x;
        delta.y = trail->head.tmpl.pos.y - prev.y;
        delta.z = trail->head.tmpl.pos.z - prev.z;
        dist = func_15143E64(&delta);
        if (dist > 0.0001f) {
            WaveTrailPoint *newest;

            newest = &entries[trail->head.end];
            newest->pos = trail->head.tmpl.pos;
            newest->dist = dist;
            len = sqrtf(delta.x * delta.x + delta.z * delta.z);
            if (len == 0.0f) {
                newest->invLen = 1.0f;
            } else {
                newest->invLen = 1.0f / len;
            }
            newest->wave = 0.0f;
            newest->scaled = info->scaled;
            newest->normal = info->normal;
            trail->head.end++;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
            }
            if ((info->flags & WAVE_TRAIL_SMOOTH) && trail->head.count >= 3) {
                WaveTrailPoint *mid;
                WaveTrailPoint *oldest;
                f32 headInv;
                s16 midIdx;
                s16 oldIdx;
                f32 d1z;
                f32 d1x;
                f32 d2z;
                f32 d2x;
                f32 midInv;
                f32 scale; // matching: a named local for 51.0f steers FP register allocation
                TrailVec2 n;
                f32 lenSq;

                midIdx = trail->head.end - 1;
                if (midIdx < 0) {
                    midIdx = trail->head.tmpl.itemCount - 1;
                }
                midIdx--;
                if (midIdx < 0) {
                    midIdx = trail->head.tmpl.itemCount - 1;
                }
                oldIdx = midIdx - 1;
                if (oldIdx < 0) {
                    oldIdx = trail->head.tmpl.itemCount - 1;
                }
                mid = &entries[midIdx];
                oldest = &entries[oldIdx];
                headInv = newest->invLen;
                midInv = mid->invLen;
                d1x = (newest->pos.x - mid->pos.x) * headInv;
                d1z = (newest->pos.z - mid->pos.z) * headInv;
                d2x = (oldest->pos.x - mid->pos.x) * midInv;
                d2z = (oldest->pos.z - mid->pos.z) * midInv;
                n.x = -(d1z - d2z);
                n.z = d1x - d2x;
                lenSq = n.x * n.x + n.z * n.z;
                if (lenSq > 0.0001f) {
                    f32 normInv;

                    scale = 51.0f;
                    normInv = 1.0f / sqrtf(lenSq);
                    n.x *= normInv;
                    n.z *= normInv;
                    mid->normal = n;
                    mid->scaled.x = n.x * scale;
                    mid->scaled.z = n.z * scale;
                } else {
                    mid->scaled.x = 0.0f;
                    mid->scaled.z = 0.0f;
                    mid->normal.x = 0.0f;
                    mid->normal.z = 0.0f;
                }
            }
        }
    }
    return 1;
}
