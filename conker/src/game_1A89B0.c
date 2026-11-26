#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_800DDD58;
extern Gfx D_8008CFA0[];

/*
 * Sprite animation callbacks (called through D_8008CEBC): each copies the sprite's position into its first
 * vertex, possibly offset, and may advance the animation phase or lower the alpha. Nonzero hides the sprite.
 */

/* Advances the phase by half again, wrapping at the frame count, and drifts the height toward 41 with jitter. */
s32 func_1517B500(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
    s32 diff;
    s32 limit;
    s32 rnd;

    *phase = (*phase * 3) / 2;
    limit = D_80095CE0[pos->texInfo].frameCount << 8;
    if (*phase >= limit) {
        *phase -= limit;
    }

    vtx->v.ob[0] = pos->x;
    vtx->v.ob[1] = pos->y;
    vtx->v.ob[2] = pos->z;

    diff = 41 - pos->height;
    if (diff == 0) {
        pos->height -= (func_151EF610() % 8) - 4;
    } else if (diff < 0) {
        rnd = func_151EF610();
        pos->height -= ((func_151EF610() % -diff) >> 1) + (rnd % 8) - 4;
    } else {
        rnd = func_151EF610();
        pos->height += ((func_151EF610() % diff) >> 1) + (rnd % 8) - 4;
    }

    vtx->v.ob[0] = pos->x;
    vtx->v.ob[1] = pos->y;
    vtx->v.ob[2] = pos->z;
    return 0;
}

/* Once the phase reaches 0x600, offsets the vertex by it, fades the alpha and holds the phase at 0x600. */
s32 func_1517B6E8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
    s32 fade;

    if (*phase >= 0x600) {
        vtx->v.ob[1] = pos->y + (*phase >> 6) - 24;
        vtx->v.ob[0] = pos->x + (*phase >> 8) - 6;
        fade = ((*alpha << 7) / ((*phase >> 6) - 23)) >> 4;
        if (fade < *alpha) {
            *alpha = fade;
        }
        *phase = 0x600;
    } else {
        vtx->v.ob[0] = pos->x;
        vtx->v.ob[1] = pos->y;
    }
    vtx->v.ob[2] = pos->z;
    return 0;
}


/* Pulls a phase above 0x500 back by 0x200 and clamps 0x300..0x500 to 0x300. */
s32 func_1517B7A8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
    s32 cur = *phase;

    if (cur >= 0x300) {
        if (cur > 0x500) {
            *phase = cur - 0x200;
        } else {
            *phase = 0x300;
        }
    }

    vtx->v.ob[0] = pos->x;
    vtx->v.ob[1] = pos->y;
    vtx->v.ob[2] = pos->z;
    return 0;
}

/* Hops along a parabola peaking at phase 0x600; hidden once the phase reaches 0xD00. */
s32 func_1517B7F8(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
    s32 t;
    s32 cur;

    cur = *phase;
    if (cur >= 0xD00) {
        return 1;
    }
    t = cur - 0x600;
    if (cur >= 0x100) {
        t = 0x190000 - (t * t);
        vtx->v.ob[1] = pos->y + (t / 7680);
        cur = *phase;
        if (cur >= 0x200) {
            if (cur > 0xA00) {
                *phase = cur - 0x800;
            } else {
                *phase = 0x200;
            }
        }
    } else {
        vtx->v.ob[1] = pos->y;
    }
    vtx->v.ob[0] = pos->x;
    vtx->v.ob[2] = pos->z;
    return 0;
}

extern Game1A89B0Position *D_800DDD18;

/* Quicksorts D_800DDD18[first..last] by x, or by z when D_8008CEB4 is set (pivot = last element). */
void func_1517B89C(s32 first, s32 last) {
    s32 left;
    s32 right;
    s32 pivot;

    left = first;
    if (first < last) {
        right = last;
        while (1) {
            if (D_8008CEB4 == 0) {
                pivot = D_800DDD18[last].x;
                while (D_800DDD18[left].x < pivot && left < last) left++;
                while (pivot < D_800DDD18[right].x && first < right) right--;
            } else {
                pivot = D_800DDD18[last].z;
                while (D_800DDD18[left].z < pivot && left < last) left++;
                while (pivot < D_800DDD18[right].z && first < right) right--;
            }
            if (left < right) {
                {
                    Game1A89B0Position swap = D_800DDD18[left];
                    D_800DDD18[left] = D_800DDD18[right];
                    D_800DDD18[right] = swap;
                }
                left++;
                right--;
            } else {
                break;
            }
        }
        {
            Game1A89B0Position pivotSwap = D_800DDD18[left];
            D_800DDD18[left] = D_800DDD18[last];
            D_800DDD18[last] = pivotSwap;
            func_1517B89C(first, left - 1);
            func_1517B89C(left + 1, last);
        }
    }
}

typedef struct {
    s32 (*fn)(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha); /* nonzero: hide the sprite */
    s32 unk4;
    s32 unk8;
} Game1A89B0Anim;

typedef struct {
    s16 idx;
    s16 unk2;
} Game1A89B0AnimRef;

/* Texture coordinate extents of a sprite type's frame. */
typedef struct {
    s16 s;
    s16 t;
} Game1A89B0TexSize;

extern Game1A89B0Anim D_8008CEBC[];
extern Game1A89B0AnimRef D_8008CF00[][8];
/* Per sprite type (15 types) unless noted: */
extern u32 D_800DDC40[15]; /* distance to the nearest sprite this frame */
extern Game1A89B0TexSize D_800DDB80[];
extern s32 math_distance3D(s32, s32, s32);

extern struct { s16 x; s16 y; s16 z; } D_800DD470; /* set by func_1517E05C; x == 0 means unset */

/* Per sprite type: the texture file (D_800DDA90) and the 26 frame pointers into its decoded buffer. */
#define FRAMES_PER_TYPE 26

/* This frame's sprite vertex buffer, four vertices per sprite. */
#define VTX(n) (((Vtx *)(s32)D_800DDD10[D_800BE9C0])[n]) // matching: the int round trip keeps the original addressing

/* Whether sorted sprite k is still inside the box, walking from its near edge in direction dir along the axis. */
#define IN_WINDOW(k)                                                                \
    ((axis == 0 && ((dir == 1 && (k) < D_800DDD20 && D_800DDD18[k].x < maxX) ||   \
                    (dir == -1 && (k) >= 0 && minX < D_800DDD18[k].x))) ||         \
     (axis != 0 && ((dir == 1 && (k) < D_800DDD20 && D_800DDD18[k].z < maxZ) ||   \
                    (dir == -1 && (k) >= 0 && minZ < D_800DDD18[k].z))))

/*
 * Builds this frame's sprite vertex buffer: walks the sprites sorted along the axis facing `angle`
 * (re-sorting when that axis changes) from the near edge of the box of half-size `radius` around (x, y, z),
 * and for each one within `radius` loads its type's texture on demand, runs its animation callback and
 * fills four vertices (alpha falls off with distance; sprites within 140 of (fadeX, fadeY, fadeZ) are
 * skipped, fading in up to 240). Then steps the per-type animation phases and turns every quad to face
 * `angle`.
 */
// matching: hi, fadeDist, j and y are reused for unrelated values; separate locals change IDO's allocation
void func_1517BBAC(s32 x, s32 y, s32 z, s32 radius, f32 angle, s32 fadeX, s32 fadeY, s32 fadeZ) {
    s32 i;
    s32 lo;
    s32 hi;
    s32 mid;
    s32 dist;
    s16 height;
    s16 dz;
    s32 phase;
    s32 dir = 1;
    s32 axis = 1;
    s32 type;
    s16 maxX;
    s16 minX;
    s16 maxZ;
    s16 minZ;
    s32 bits;
    s32 j; /* sprites emitted so far */
    s32 fadeDist; /* also the alpha, then the phase slot */
    s32 shade;
    s16 halfWidth;
    s16 dx;
    s32 step;
    u32 *nearest;
    Vtx *vtx;
    s32 frame;
    f32 sinA;
    s16 cosA;

    D_800DDD58 = 0;
    maxX = x + radius; minX = x - radius;
    maxZ = z + radius; minZ = z - radius;
    dist = D_800DDD20 - 1; hi = dist; // matching: routing through dist (and below) steers IDO's allocation
    lo = 0;
    mid = 0;
    if ((angle > 45.0f && angle < 135.0f) || (angle > 225.0f && angle < 315.0f)) {
        axis = 0;
        if (angle > 180.0f) {
            dir = -1;
        }
    } else if (angle > 90.0f && angle < 270.0f) {
        dir = -1;
    }
    if (axis != D_8008CEB4) {
        D_8008CEB4 = axis;
        dist = D_800DDD20 - 1;
        func_1517B89C(0, dist);
    }
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (axis == 0) {
            if (dir == 1) {
                if (minX < D_800DDD18[mid].x) {
                    hi = mid - 1;
                } else if (D_800DDD18[mid].x < minX) {
                    lo = mid + 1;
                } else if (minX == D_800DDD18[mid].x) {
                    break;
                }
            } else {
                if (maxX < D_800DDD18[mid].x) {
                    hi = mid - 1;
                } else if (D_800DDD18[mid].x < maxX) {
                    lo = mid + 1;
                } else if (maxX == D_800DDD18[mid].x) {
                    break;
                }
            }
        } else {
            if (dir == 1) {
                if (minZ < D_800DDD18[mid].z) {
                    hi = mid - 1;
                } else if (D_800DDD18[mid].z < minZ) {
                    lo = mid + 1;
                } else if (minZ == D_800DDD18[mid].z) {
                    break;
                }
            } else {
                if (maxZ < D_800DDD18[mid].z) {
                    hi = mid - 1;
                } else if (D_800DDD18[mid].z < maxZ) {
                    lo = mid + 1;
                } else if (maxZ == D_800DDD18[mid].z) {
                    break;
                }
            }
        }
    }
    /* nearest distance per sprite type */
    for (nearest = D_800DDC40; nearest < &D_800DDC40[15]; nearest++) {
        *nearest = 0x7FFF;
    }
    i = mid;
    if (IN_WINDOW(mid)) {
        j = ((u32) D_800DDD20 < (u32) D_800DDD0C) ? D_800DDD20 : D_800DDD0C;
        D_800DDD10[D_800BE9C0] = allocate_memory(j << 6, 4, 2, 0);
        if (D_800DDD10[D_800BE9C0] == NULL) {
            return;
        }
    } else {
        D_800DDD10[D_800BE9C0] = NULL;
        return;
    }
    for (j = 0; (u32) j < (u32) D_800DDD0C && IN_WINDOW(i); i += dir) {
        dist = math_distance3D(D_800DDD18[i].x - x, D_800DDD18[i].y - y, D_800DDD18[i].z - z);
        if (dist < radius) {
            type = D_800DDD18[i].unk6 & 0xF;
            if ((u32) dist < D_800DDC40[type]) {
                D_800DDC40[type] = dist;
            }
            fadeDist = math_distance3D(D_800DDD18[i].x - fadeX, D_800DDD18[i].y - fadeY, D_800DDD18[i].z - fadeZ);
            if (fadeDist > 140) {
                if (fadeDist < 240) {
                    dist = radius - ((radius - dist) * (fadeDist - 140)) / 100;
                }
                if (D_800DD478[type * FRAMES_PER_TYPE] == 0) {
                    if (D_800DDA90[type].devAddr != 0) {
                        hi = D_80095CE0[D_800DDBC0[type]].frameSize * D_80095CE0[D_800DDBC0[type]].frameCount;
                        D_800DDA90[type].buf = func_10003C6C(D_800DDA90[type].size, 1, 2, 1, 0);
                        if (D_800DDA90[type].buf != 0) {
                            D_800DD478[type * FRAMES_PER_TYPE] = func_10003C6C(hi, 4, 1, 0, 0);
                            if (D_800DD478[type * FRAMES_PER_TYPE] != 0) {
                                func_10004514(D_800DDA90[type].devAddr, (void *) D_800DDA90[type].buf, D_800DDA90[type].size, 1);
                                D_800DDA90[type].used = 1;
                                hi = D_80095CE0[D_800DDBC0[type]].frameSize;
                                for (frame = 1; frame < FRAMES_PER_TYPE; frame++) {
                                    D_800DD478[type * FRAMES_PER_TYPE + frame] = D_800DD478[type * FRAMES_PER_TYPE + frame - 1] + hi;
                                }
                            } else {
                                func_10004074(D_800DDA90[type].buf);
                            }
                        }
                    }
                texdone:; // matching: IDO needs this empty labelled statement (a block join) here
                } else {
                    if (D_800DDA90[type].used == 0) {
                        D_800DDA90[type].used = 1;
                        func_100043B4((s32 *) D_800DD478[type * FRAMES_PER_TYPE], 4);
                    }
                }
                vtx = &VTX(j << 2);
                phase = D_800DDC90[D_800DDD18[i].phaseIdx];
                if (phase < 0) {
                    phase = 0;
                }
                if (D_800DD470.x != 0) {
                    if (math_distance3D(D_800DDD18[i].x - D_800DD470.x, D_800DDD18[i].y - D_800DD470.y,
                                      D_800DDD18[i].z - D_800DD470.z) < 80 &&
                        D_800DDBC0[type] == 0) {
                        if (D_800DDD18[i].unkB != 0x21) {
                            D_800DDD18[i].unkB = 0x21;
                            func_15170500(D_800DDD18[i].x, D_800DDD18[i].y + 68, D_800DDD18[i].z, 0, 0, 255, 0);
                        }
                    }
                }
                if (D_800DDD18[i].unkB >= 0x21) {
                    if (D_800DDD18[i].unkB >= 0x22) {
                        D_800DDD18[i].unkB--;
                    }
                    vtx[3].v.flag = D_800DDD18[i].height;
                } else {
                    if (dist < 48 && D_800DDBC0[type] == 0) {
                        D_800DDD18[i].unkB = 0x22;
                        func_15170500(D_800DDD18[i].x, D_800DDD18[i].y + 68, D_800DDD18[i].z, 0, 0, 255, 0);
                    }
                    vtx[3].v.flag = D_800DDD18[i].height;
                }
                vtx[2].v.flag = D_800DDD18[i].halfWidth;
                dist = (u32) (radius - dist) >> 1;
                if (dist >= 256) {
                    dist = 255;
                }
                vtx[0].v.cn[3] = dist;
                if (D_8008CEBC[D_8008CF00[D_800DDC80[type]][D_800DDD18[i].animIdx].idx].fn != NULL) {
                    if (D_8008CEBC[D_8008CF00[D_800DDC80[type]][D_800DDD18[i].animIdx].idx].fn(&D_800DDD18[i], vtx, &phase, &vtx[0].v.cn[3]) != 0) {
                        continue;
                    }
                } else {
                    vtx[0].v.ob[0] = D_800DDD18[i].x;
                    vtx[0].v.ob[1] = D_800DDD18[i].y;
                    vtx[0].v.ob[2] = D_800DDD18[i].z;
                }
                shade = D_800DDD18[i].unk6 | 0x9F;
                fadeDist = vtx[0].v.cn[3] & 0xFF;
                vtx[0].v.cn[0] = vtx[0].v.cn[1] = vtx[0].v.cn[2] = shade;
                vtx[1].v.cn[0] = vtx[1].v.cn[1] = vtx[1].v.cn[2] = shade;
                vtx[2].v.cn[0] = vtx[2].v.cn[1] = vtx[2].v.cn[2] = shade;
                vtx[3].v.cn[0] = vtx[3].v.cn[1] = vtx[3].v.cn[2] = shade;
                vtx[1].v.cn[3] = vtx[2].v.cn[3] = vtx[3].v.cn[3] = fadeDist;
                if (D_800DDD18[i].unkB >= 0x21) {
                    vtx[0].v.flag = D_800DDBE0[type] + type * FRAMES_PER_TYPE;
                } else {
                    vtx[0].v.flag = type * FRAMES_PER_TYPE + (phase >> 8);
                }
                vtx++;
                vtx->v.flag = D_800DDBF0[type];
                VTX((j << 2) + 1).v.tc[0] =
                    VTX((j << 2) + 2).v.tc[0] = D_800DDB80[type].s;
                VTX((j << 2) + 2).v.tc[1] =
                    VTX((j << 2) + 3).v.tc[1] = D_800DDB80[type].t;
                j++;
            }
        }
    }
    D_800DDD58 = j;
    /* step each type's 4 phases: ping-pong (types in D_800DDC04, D_800DDC08 adds random flips) or loop */
    if (D_800BEAC0 == 0) {
        for (i = 0; i != 15; i++) {
            hi = (D_800DDBD0[i] << 8) - 1;
            if (hi > 0x100) {
                y = 1 << i;
                if (y & D_800DDC04) {
                    if (y & D_800DDC08) {
                        step = func_151EF610() % 10;
                        if (step == 0) {
                            D_800DDC10[i] |= func_151EF610() % 16;
                        } else if (step == 1) {
                            D_800DDC10[i] &= func_151EF610() % 16;
                        } else {
                            step = D_800BE9E4 << 5;
                        }
                    } else {
                        step = D_800BE9E4 << 6;
                    }
                    fadeDist = i << 2;
                    for (j = 0; j != 4; j++, fadeDist++) {
                        bits = D_800DDC10[i];
                        y = 1 << j;
                        if (bits & y) {
                            D_800DDC90[fadeDist] -= step;
                            if (D_800DDC90[fadeDist] <= 0) {
                                D_800DDC90[fadeDist] = -D_800DDC90[fadeDist];
                                D_800DDC10[i] = bits ^ y;
                            }
                        } else {
                            D_800DDC90[fadeDist] += step;
                            if (D_800DDC90[fadeDist] >= hi) {
                                D_800DDC90[fadeDist] = hi * 2 - D_800DDC90[fadeDist];
                                D_800DDC10[i] = bits ^ y;
                            }
                        }
                    }
                } else {
                    bits = D_800BE9E4 * 48;
                    fadeDist = i << 2;
                    for (j = 0; j != 4; j++, fadeDist++) {
                        D_800DDC90[fadeDist] += bits;
                        if (D_800DDC90[fadeDist] >= hi) {
                            D_800DDC90[fadeDist] -= hi;
                        }
                    }
                }
            }
        }
    }
    /* occasionally spawn a particle at a random sprite showing frame 0 */
    i = (u32) func_151EF610() % (u32) D_800DDD0C;
    if ((u32) i < (u32) D_800DDD58) {
        i *= 4;
        j = VTX(i).v.flag;
        if (j % 26 == 0 && D_800DDBC0[j / 26] == 0) {
            func_15170500(VTX(i).v.ob[0], VTX(i).v.ob[1] + 0x44, VTX(i).v.ob[2], 0, 1, 0xFF, 0);
        }
    }
    /* turn each quad to face `angle`: halfWidth across, height * 4 up */
    angle *= 0.01745329238f;
    cosA = cosf(angle) * 128.0f;
    sinA = sinf(angle);
    for (i = 0; (u32) i < (u32) D_800DDD58; i++) {
        halfWidth = (s16) VTX((i << 2) + 2).v.flag;
        x = VTX((i << 2)).v.ob[0];
        height = VTX((i << 2) + 3).v.flag * 4;
        y = VTX((i << 2)).v.ob[1];
        z = VTX((i << 2)).v.ob[2];
        dx = (halfWidth * cosA) >> 5;
        VTX((i << 2)).v.ob[0] = VTX((i << 2) + 3).v.ob[0] = dx + x;
        VTX((i << 2)).v.ob[1] = VTX((i << 2) + 1).v.ob[1] = y;
        dz = (halfWidth * (s16) (sinA * 128.0f)) >> 5;
        VTX((i << 2)).v.ob[2] = VTX((i << 2) + 3).v.ob[2] = z - dz;
        VTX((i << 2) + 1).v.ob[0] = VTX((i << 2) + 2).v.ob[0] = x - dx;
        VTX((i << 2) + 2).v.ob[1] = VTX((i << 2) + 3).v.ob[1] = height + y;
        VTX((i << 2) + 1).v.ob[2] = VTX((i << 2) + 2).v.ob[2] = dz + z;
        VTX((i << 2)).v.tc[0] = VTX((i << 2)).v.tc[1] = 0;
        VTX((i << 2) + 1).v.tc[1] = VTX((i << 2) + 3).v.tc[0] = 0;
    }
    D_800DD470.x = 0;
}

/* Releases every sprite type's texture file buffer, first passing it with the frames to func_10006240. */
void func_1517CFC4(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (D_800DDA90[i].buf != 0) {
            if (D_800DD478[i * FRAMES_PER_TYPE] != 0) {
                func_10006240(D_800DDA90[i].buf, D_800DD478[i * FRAMES_PER_TYPE], D_8003809C);
                func_10004074(D_800DDA90[i].buf);
            }
            D_800DDA90[i].buf = 0;
        }
    }
}


/* Draws one textured billboard quad at (x, y, z) facing `angle` into the sprite vertex buffer's next slot. */
Gfx *func_1517D074(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 texWidth, u8 texHeight, s32 texture) {
    u32 count;
    s16 dx;
    s16 dz;
    Vtx *v;

    count = D_800DDD58;
    v = &VTX(count * 4);
    if ((u32)D_800DDD58 >= (u32)D_800DDD0C) {
        return gfx;
    }

    gSPDisplayList(gfx++, D_8008CFA0);
    gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
    gSPClearGeometryMode(gfx++, G_CULL_BACK);
    gDPSetOtherMode(gfx++, 0x182C3F, 0x0C184B50);

    angle *= 0.01745329238f;
    dx = cosf(angle) * halfWidth;
    dz = sinf(angle) * halfWidth;

    v[0].v.ob[0] = v[3].v.ob[0] = x + dx;
    v[0].v.ob[1] = v[1].v.ob[1] = y;
    v[0].v.ob[2] = v[3].v.ob[2] = z - dz;
    v[1].v.ob[0] = v[2].v.ob[0] = x - dx;
    v[2].v.ob[1] = v[3].v.ob[1] = y + height + height;
    v[1].v.ob[2] = v[2].v.ob[2] = z + dz;
    v[1].v.tc[0] = v[2].v.tc[0] = (texWidth << 5) - 1;
    v[2].v.tc[1] = v[3].v.tc[1] = (texHeight << 5) - 1;

    gSPVertex(gfx++, &VTX(count * 4), 4, 0);
    gDPLoadTextureBlock(gfx++, texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, texWidth, (texHeight * 5 + 3) >> 2, 0,
                        G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, ((texWidth >> 1) + 7) >> 3, (texWidth * texHeight) >> 2, 1, 0,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gfx++, 1, 0, 0, (texWidth - 1) << G_TEXTURE_IMAGE_FRAC, (texHeight - 1) << G_TEXTURE_IMAGE_FRAC);

    VTX(count * 4).v.flag =
        VTX(count * 4 + 1).v.flag =
        VTX(count * 4 + 2).v.flag =
        VTX(count * 4 + 3).v.flag = 0;

    gSP1Triangle(gfx++, 0, 2, 1, 0);
    gSP1Triangle(gfx++, 0, 3, 2, 0);
    gSPSetGeometryMode(gfx++, G_SHADING_SMOOTH);
    D_800DDD58++;
    return gfx;
}

/* A billboard queued by func_1517D578 and drawn by func_1517D690. */
struct Entry1517D578 {
    s16 x;
    s16 y;
    s16 z;
    u8  frame; /* texture frame (from the D_800DDD1C animation counter) */
    u16 halfWidth;
    u16 height;
    f32 angle;
};
extern struct Entry1517D578 D_800DDD28[];

/* Queues a billboard (at most 3 per frame; D_8008CEB0 counts them). */
void func_1517D578(s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 frame) {
    struct Entry1517D578 *e;
    u8 idx = D_8008CEB0;

    if (D_8008CEB0 < 3) {
        e = &D_800DDD28[idx];
        e->x = x;
        e->y = y;
        e->z = z;
        e->angle = angle;
        e->halfWidth = halfWidth;
        e->height = height;
        e->frame = frame;
        D_8008CEB0 = idx + 1;
    }
}

/* Queues a billboard facing player `player`'s view angle, showing the current animation frame. */
void func_1517D5FC(s16 x, s16 y, s16 z, s32 player, s32 halfWidth, s32 height) {
    func_1517D578(x, y, z, camera_global_Cameras[player].TrueCameraAngle, halfWidth, height, D_800DDD1C >> 3);
}

/*
 * Advances the billboard animation counter and drains the queue, drawing each billboard with its frame of
 * the 16x32 texture strip at `texture` (nothing is drawn when it is 0). Leaves the sprite count unchanged.
 */
Gfx *func_1517D690(Gfx *gfx, s32 texture) {
    s32 saved;

    saved = D_800DDD58;
    if ((D_800DDD1C += 5) >= 200) {
        D_800DDD1C -= 200;
    }

    while (D_8008CEB0 != 0) {
        D_8008CEB0--;
        if (texture != 0) {
            /* frames are 0x500 bytes apart */
            gfx = func_1517D074(gfx, D_800DDD28[D_8008CEB0].x, D_800DDD28[D_8008CEB0].y, D_800DDD28[D_8008CEB0].z,
                                D_800DDD28[D_8008CEB0].angle, D_800DDD28[D_8008CEB0].halfWidth,
                                D_800DDD28[D_8008CEB0].height, 16, 32,
                                ((D_800DDD28[D_8008CEB0].frame * 0x280) << 1) + texture); // matching: `* 0x500` codegens differently
        }
    }

    D_800DDD58 = saved;
    return gfx;
}

extern s32 D_800DDD24;

/*
 * Draws the queued billboards (func_1517D690), then every sprite built by func_1517BBAC whose second
 * vertex's flag equals `pass`, loading each texture frame once per batch.
 */
void func_1517D7B0(Gfx **gfx, s32 pass) {
    Vtx *v;
    u32 sprite;
    u32 quad;
    u32 slot;
    u32 vtxCount;
    s32 done; /* bit (1 << q) per quad at vertex offset q in the batch: drawn, or not in this pass */
    s32 tex;
    s16 width;
    s16 height;

    tex = 0xFFFF; /* no texture loaded yet */
    if (D_800DDD10[D_800BE9C0] == NULL) {
        return;
    }
    *gfx = func_1517D690(*gfx, D_800DDD24);
    if (D_800DDD20 == 0 || D_800DDD58 == 0) {
        return;
    }
    quad = 0;
    gSPDisplayList((*gfx)++, D_8008CFA0);
    *gfx = func_1510CDB8(*gfx, 0xFF, 0xFF, 0);
    sprite = 0;
    /* Batches of up to 4 sprites (16 vertices); within a batch draw the quads grouped by texture. */
    while (sprite < D_800DDD58) {
        done = 0;
        vtxCount = (D_800DDD58 - sprite >= 5) ? 16 : (D_800DDD58 - sprite) * 4;
        for (slot = 0; slot < vtxCount; slot += 4) {
            if (pass != VTX(sprite * 4 + slot + 1).v.flag) {
                done |= 1 << slot;
            }
        }
        if (done != 0x1111) {
            gSPVertex((*gfx)++, &VTX(sprite * 4), vtxCount, 0);
            for (slot = 0; slot < vtxCount; slot += 4) {
                /* next undrawn quad using the loaded texture; else load the first undrawn one's */
                // matching: the (s32) casts below give the signed index arithmetic IDO emitted
                while (quad < vtxCount && (v = &VTX((s32)(sprite * 4)), tex != v[quad].v.flag || ((1 << quad) & done))) {
                    quad += 4;
                }
                if (quad >= vtxCount && done != 0x1111) {
                    for (quad = 0, v = &VTX((s32)(sprite * 4)); (1 << quad) & done; quad += 4) {
                    }
                    if (tex != v[quad].v.flag && quad < vtxCount) {
                        tex = v[quad].v.flag;
                        width = (v[quad + 2].v.tc[0] + 1) >> 5;
                        height = (v[quad + 2].v.tc[1] + 1) >> 5;
                        gDPLoadTextureBlock((*gfx)++, D_800DD478[tex], G_IM_FMT_RGBA, G_IM_SIZ_16b, width,
                                            (height * 5 + 3) >> 2, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                            G_TX_NOLOD, G_TX_NOLOD);
                        gDPSetTile((*gfx)++, G_IM_FMT_I, G_IM_SIZ_4b, ((width >> 1) + 7) >> 3, (width * height) >> 2, 1, 0,
                                   G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
                        gDPSetTileSize((*gfx)++, 1, 0, 0, (width - 1) << G_TEXTURE_IMAGE_FRAC, (height - 1) << G_TEXTURE_IMAGE_FRAC);
                    }
                }
                if (quad < vtxCount && !((1 << quad) & done)) {
                    VTX(sprite * 4 + quad + 3).v.flag = 0;
                    VTX(sprite * 4 + quad).v.flag =
                        VTX(sprite * 4 + quad + 1).v.flag =
                        VTX(sprite * 4 + quad + 2).v.flag =
                        VTX(sprite * 4 + quad + 3).v.flag;
                    gSP1Triangle((*gfx)++, quad, quad + 2, quad + 1, 0);
                    gSP1Triangle((*gfx)++, quad, quad + 3, quad + 2, 0);
                    done |= 1 << quad;
                }
            }
        }
        sprite += 4;
    }
    gSPSetGeometryMode((*gfx)++, G_SHADING_SMOOTH);
    gDPPipeSync((*gfx)++);
}


/* Rebuilds the sprites around player 0's view, then forgets the textures of types no sprite used. */
void func_1517DE5C(void) {
    s32 i;

    if (D_800DDD20 != 0) {
        for (i = 0; i < 15; i++) {
            D_800DDA90[i].used = 0;
        }
        func_1517BBAC(movement_obj->xpos, movement_obj->ypos, movement_obj->zpos, 1500,
                      camera_global_Cameras->TrueCameraAngle, camera_global_Cameras->eye.x, camera_global_Cameras->eye.y, camera_global_Cameras->eye.z);
        for (i = 0; i < 15; i++) {
            if (D_800DD478[i * FRAMES_PER_TYPE] != 0 && D_800DDA90[i].used == 0) {
                D_800DD478[i * FRAMES_PER_TYPE] = 0;
                D_800DDA90[i].buf = 0;
            }
        }
    }
}


/* Sets this frame's disturbance point: sprites within 80 of it react as if the player were close. */
void func_1517E05C(s32 x, s32 y, s32 z) {
    D_800DD470.x = x;
    D_800DD470.y = y;
    D_800DD470.z = z;
}
