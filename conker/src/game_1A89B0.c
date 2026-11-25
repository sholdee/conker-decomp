#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One sprite placed in the level (D_800DDD18 holds D_800DDD20 of them, sorted by x or z). */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 unk6;     /* low nibble: sprite type (texture slot 0..14) */
    u8 phaseIdx; /* index into D_800DDC90 */
    u8 texInfo;  /* index into D_80095CE0 */
    u8 pad9;
    u8 animIdx;  /* column of D_8008CF00 */
    u8 unkB;
    s8 halfWidth;
    s8 height;
} Game1A89B0Position;

typedef struct {
    u8 unk0;
    u8 frameCount;
    u16 frameSize;
    u8 pad4[6];
} Game1A89B0TexInfo;

extern s32 D_800DDD58;
extern Gfx D_8008CFA0[];
Gfx *func_1517D074(Gfx *, s16, s16, s16, f32, s32, s32, u8, u8, s32);

extern s32 func_151EF610(void);
extern Game1A89B0TexInfo D_80095CE0[];

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

extern s32 D_8008CEB4;
extern Game1A89B0Position *D_800DDD18;

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

typedef struct {
    s32 devAddr;
    s32 size;
    s32 buf;
    u8 loaded;
    u8 padD[3];
} Game1A89B0TexFile;

/* Texture coordinate extents of a sprite type's frame. */
typedef struct {
    s16 s;
    s16 t;
} Game1A89B0TexSize;

extern Game1A89B0Anim D_8008CEBC[];
extern Game1A89B0AnimRef D_8008CF00[][8];
extern u32 D_800DDC40[15];
extern s16 D_800DDC90[];
extern Game1A89B0TexSize D_800DDB80[];
extern u8 D_800DDBC0[];
extern u8 D_800DDBD0[];
extern u8 D_800DDBE0[];
extern u8 D_800DDBF0[];
extern u8 D_800DDC10[];
extern u8 D_800DDC80[];
extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_150AD9A0(s32, s32, s32);
extern void func_15170500(s32, s32, s32, s32, s32, s32, s32);

extern struct { s16 x; s16 y; s16 z; } D_800DD470; /* set by func_1517E05C; x == 0 means unset */
extern void func_10004074(s32);

/* Per sprite type: the texture file (D_800DDA90) and the 26 frame pointers into its decoded buffer. */
#define sTexFiles ((Game1A89B0TexFile *)&D_800DDA90)
#define FRAMES_PER_TYPE 26
#define sFrames ((s32 *)&D_800DD478) /* [15][FRAMES_PER_TYPE], indexed by a vertex's texture id */

/* This frame's sprite vertex buffer, four vertices per sprite. */
#define VTX(n) (((Vtx *) D_800DDD10[D_800BE9C0])[n])

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
 * Several locals are reused for unrelated values (hi, camDist, j, y); IDO's register allocation depends on it.
 */
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
    s32 camDist; /* also the alpha, then the phase slot */
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
    dist = D_800DDD20 - 1; hi = dist;
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
        D_800DDD10[D_800BE9C0] = (s32) allocate_memory(j << 6, 4, 2, 0);
        if (D_800DDD10[D_800BE9C0] == 0) {
            return;
        }
    } else {
        D_800DDD10[D_800BE9C0] = 0;
        return;
    }
    for (j = 0; (u32) j < (u32) D_800DDD0C && IN_WINDOW(i); i += dir) {
        dist = func_150AD9A0(D_800DDD18[i].x - x, D_800DDD18[i].y - y, D_800DDD18[i].z - z);
        if (dist < radius) {
            type = D_800DDD18[i].unk6 & 0xF;
            if ((u32) dist < D_800DDC40[type]) {
                D_800DDC40[type] = dist;
            }
            camDist = func_150AD9A0(D_800DDD18[i].x - fadeX, D_800DDD18[i].y - fadeY, D_800DDD18[i].z - fadeZ);
            if (camDist > 140) {
                if (camDist < 240) {
                    dist = radius - ((radius - dist) * (camDist - 140)) / 100;
                }
                if (sFrames[type * FRAMES_PER_TYPE] == 0) {
                    if (sTexFiles[type].devAddr != 0) {
                        hi = D_80095CE0[D_800DDBC0[type]].frameSize * D_80095CE0[D_800DDBC0[type]].frameCount;
                        sTexFiles[type].buf = func_10003C6C(sTexFiles[type].size, 1, 2, 1, 0);
                        if (sTexFiles[type].buf != 0) {
                            sFrames[type * FRAMES_PER_TYPE] = func_10003C6C(hi, 4, 1, 0, 0);
                            if (sFrames[type * FRAMES_PER_TYPE] != 0) {
                                func_10004514(sTexFiles[type].devAddr, (void *) sTexFiles[type].buf, sTexFiles[type].size, 1);
                                sTexFiles[type].loaded = 1;
                                hi = D_80095CE0[D_800DDBC0[type]].frameSize;
                                for (frame = 1; frame < FRAMES_PER_TYPE; frame++) {
                                    sFrames[type * FRAMES_PER_TYPE + frame] = sFrames[type * FRAMES_PER_TYPE + frame - 1] + hi;
                                }
                            } else {
                                func_10004074(sTexFiles[type].buf);
                            }
                        }
                    }
                texdone:; // matching: IDO needs this empty labelled statement (a block join) here
                } else {
                    if (sTexFiles[type].loaded == 0) {
                        sTexFiles[type].loaded = 1;
                        func_100043B4((s32 *) sFrames[type * FRAMES_PER_TYPE], 4);
                    }
                }
                vtx = &VTX(j << 2);
                phase = D_800DDC90[D_800DDD18[i].phaseIdx];
                if (phase < 0) {
                    phase = 0;
                }
                if (D_800DD470.x != 0) {
                    if (func_150AD9A0(D_800DDD18[i].x - D_800DD470.x, D_800DDD18[i].y - D_800DD470.y,
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
                camDist = vtx[0].v.cn[3] & 0xFF;
                vtx[0].v.cn[0] = vtx[0].v.cn[1] = vtx[0].v.cn[2] = shade;
                vtx[1].v.cn[0] = vtx[1].v.cn[1] = vtx[1].v.cn[2] = shade;
                vtx[2].v.cn[0] = vtx[2].v.cn[1] = vtx[2].v.cn[2] = shade;
                vtx[3].v.cn[0] = vtx[3].v.cn[1] = vtx[3].v.cn[2] = shade;
                vtx[1].v.cn[3] = vtx[2].v.cn[3] = vtx[3].v.cn[3] = camDist;
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
                    camDist = i << 2;
                    for (j = 0; j != 4; j++, camDist++) {
                        bits = D_800DDC10[i];
                        y = 1 << j;
                        if (bits & y) {
                            D_800DDC90[camDist] -= step;
                            if (D_800DDC90[camDist] <= 0) {
                                D_800DDC90[camDist] = -D_800DDC90[camDist];
                                D_800DDC10[i] = bits ^ y;
                            }
                        } else {
                            D_800DDC90[camDist] += step;
                            if (D_800DDC90[camDist] >= hi) {
                                D_800DDC90[camDist] = hi * 2 - D_800DDC90[camDist];
                                D_800DDC10[i] = bits ^ y;
                            }
                        }
                    }
                } else {
                    bits = D_800BE9E4 * 48;
                    camDist = i << 2;
                    for (j = 0; j != 4; j++, camDist++) {
                        D_800DDC90[camDist] += bits;
                        if (D_800DDC90[camDist] >= hi) {
                            D_800DDC90[camDist] -= hi;
                        }
                    }
                }
            }
        }
    }
    i = (u32) func_151EF610() % (u32) D_800DDD0C;
    if ((u32) i < (u32) D_800DDD58) {
        i *= 4;
        j = VTX(i).v.flag;
        if (j % 26 == 0 && D_800DDBC0[j / 26] == 0) {
            func_15170500(VTX(i).v.ob[0], VTX(i).v.ob[1] + 0x44, VTX(i).v.ob[2], 0, 1, 0xFF, 0);
        }
    }
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

extern void func_10006240(s32, s32, s32);
extern void func_10004074(s32);

struct s1517CFC4_a { s32 unk0; s32 unk4; s32 unk8; s32 unkC; };

void func_1517CFC4(void) {
    s32 i;
    s32 *q;
    struct s1517CFC4_a *p;

    for (q = (s32 *)&D_800DD478, p = (struct s1517CFC4_a *)&D_800DDA90, i = 0; i != 15; i++, p++) {
        if (p->unk8 != 0) {
            if (q[i * 26] != 0) {
                func_10006240(p->unk8, q[i * 26], D_8003809C);
                func_10004074(p->unk8);
            }
            p->unk8 = 0;
        }
    }
}


Gfx *func_1517D074(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 radius, s32 height, u8 width, u8 depth, s32 timg)
{
    u32 i;
    s16 c;
    s16 s;
    Vtx *v;

    i = D_800DDD58;
    v = &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4];
    if ((u32)D_800DDD58 >= (u32)D_800DDD0C)
    {
        return gfx;
    }

    gSPDisplayList(gfx++, &D_8008CFA0);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFCF438);
    gSPClearGeometryMode(gfx++, G_CULL_BACK);
    gDPSetOtherMode(gfx++, 0x182C3F, 0x0C184B50);

    angle *= 0.01745329238f;
    c = cosf(angle) * radius;
    s = sinf(angle) * radius;

    v[0].v.ob[0] = v[3].v.ob[0] = x + c;
    v[1].v.ob[1] = y;
    v[0].v.ob[1] = v[1].v.ob[1];
    v[0].v.ob[2] = v[3].v.ob[2] = z - s;
    v[1].v.ob[0] = v[2].v.ob[0] = x - c;
    v[2].v.ob[1] = v[3].v.ob[1] = (s16)(y + height + height);
    v[1].v.ob[2] = v[2].v.ob[2] = z + s;
    v[1].v.tc[0] = v[2].v.tc[0] = (width << 5) - 1;
    v[2].v.tc[1] = v[3].v.tc[1] = (depth << 5) - 1;

    gSPVertex(gfx++, &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4], 4, 0);
    gDPLoadTextureBlock(gfx++, timg, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, (depth * 5 + 3) >> 2, 0,
                        G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, ((width >> 1) + 7) >> 3, (width * depth) >> 2, 1, 0,
               G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPSetTileSize(gfx++, 1, 0, 0, (width - 1) << 2, (depth - 1) << 2);

    ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 1].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 2].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 3].v.flag = 0;

    gSP1Triangle(gfx++, 0, 2, 1, 0);
    gSP1Triangle(gfx++, 0, 3, 2, 0);
    gSPSetGeometryMode(gfx++, G_SHADING_SMOOTH);
    D_800DDD58++;
    return gfx;
}

struct Entry1517D578 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8  unk6;
    s16 unk8;
    s16 unkA;
    f32 unkC;
};
extern struct Entry1517D578 D_800DDD28[];

void func_1517D578(s16 arg0, s16 arg1, s16 arg2, f32 arg3, s32 arg4, s32 arg5, u8 arg6)
{
    struct Entry1517D578 *e;
    u8 idx = D_8008CEB0;
    if (D_8008CEB0 < 3)
    {
        e = &D_800DDD28[idx];
        e->unk0 = arg0;
        e->unk2 = arg1;
        e->unk4 = arg2;
        e->unkC = arg3;
        e->unk8 = arg4;
        e->unkA = arg5;
        e->unk6 = arg6;
        D_8008CEB0 = idx + 1;
    }
}

void func_1517D5FC(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    func_1517D578(arg0, arg1, arg2, D_800DBFF0[arg3].unk380, arg4, arg5, D_800DDD1C >> 3);
}

s32 func_1517D690(s32 arg0, s32 arg1) {
    s32 saved;

    saved = D_800DDD58;
    if ((D_800DDD1C = D_800DDD1C + 5) >= 0xC8) {
        D_800DDD1C -= 0xC8;
    }

    if (D_8008CEB0 != 0) {
        do {
            D_8008CEB0 = D_8008CEB0 - 1;
            if (arg1 != 0) {
                arg0 = func_1517D074(arg0,
                                      D_800DDD28[D_8008CEB0].unk0,
                                      D_800DDD28[D_8008CEB0].unk2,
                                      D_800DDD28[D_8008CEB0].unk4,
                                      D_800DDD28[D_8008CEB0].unkC,
                                      (s32)(u16)D_800DDD28[D_8008CEB0].unk8,
                                      (s32)(u16)D_800DDD28[D_8008CEB0].unkA,
                                      0x10, 0x20,
                                      ((D_800DDD28[D_8008CEB0].unk6 * 0x280) << 1) + arg1);
            }
        } while (D_8008CEB0 != 0);
    }

    D_800DDD58 = saved;
    return arg0;
}

extern s32 D_800DDD24;
Gfx *func_1510CDB8(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3);

void func_1517D7B0(Gfx **arg0, s32 arg1) {
    Vtx *v;
    u32 i;
    u32 k;
    u32 j;
    u32 n;
    s32 mask;
    s32 tex;
    s16 width;
    s16 height;

    tex = 0xFFFF;
    if (D_800DDD10[D_800BE9C0] != 0) {
        *arg0 = (Gfx *)func_1517D690((s32)*arg0, D_800DDD24);
        if (D_800DDD20 != 0 && D_800DDD58 != 0) {
            k = 0;
            gSPDisplayList((*arg0)++, D_8008CFA0);
            *arg0 = func_1510CDB8(*arg0, 0xFF, 0xFF, 0);
            i = 0;
            while (i < D_800DDD58) {
                mask = 0;
                n = (D_800DDD58 - i >= 5) ? 16 : (D_800DDD58 - i) * 4;
                for (j = 0; j < n; j += 4) {
                    if (arg1 != ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + j + 1].v.flag) {
                        mask |= 1 << j;
                    }
                }
                if (mask != 0x1111) {
                    gSPVertex((*arg0)++, &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4], n, 0);
                    for (j = 0; j < n; j += 4) {
                        while (k < n && (v = &((Vtx *)D_800DDD10[D_800BE9C0])[(s32)(i * 4)], tex != v[k].v.flag || ((1 << k) & mask))) {
                            k += 4;
                        }
                        if (k >= n && mask != 0x1111) {
                            for (k = 0, v = &((Vtx *)D_800DDD10[D_800BE9C0])[(s32)(i * 4)]; (1 << k) & mask; k += 4) {
                            }
                            if (tex != v[k].v.flag && k < n) {
                                tex = v[k].v.flag;
                                width = (v[k + 2].v.tc[0] + 1) >> 5;
                                height = (v[k + 2].v.tc[1] + 1) >> 5;
                                gDPLoadTextureBlock((*arg0)++, (&D_800DD478)[tex], G_IM_FMT_RGBA, G_IM_SIZ_16b, width,
                                                    (height * 5 + 3) >> 2, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
                                gDPSetTile((*arg0)++, G_IM_FMT_I, G_IM_SIZ_4b, ((width >> 1) + 7) >> 3, (width * height) >> 2, 1, 0,
                                           G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
                                gDPSetTileSize((*arg0)++, 1, 0, 0, (width - 1) << 2, (height - 1) << 2);
                            }
                        }
                        if (k < n && !((1 << k) & mask)) {
                            ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 3].v.flag = 0;
                            ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 1].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 2].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 3].v.flag;
                            gSP1Triangle((*arg0)++, k, k + 2, k + 1, 0);
                            gSP1Triangle((*arg0)++, k, k + 3, k + 2, 0);
                            mask |= 1 << k;
                        }
                    }
                }
                i += 4;
            }
            gSPSetGeometryMode((*arg0)++, G_SHADING_SMOOTH);
            gDPPipeSync((*arg0)++);
        }
    }
}


void func_1517BBAC(s32, s32, s32, s32, f32, s32, s32, s32);

void func_1517DE5C(void) {
    s32 i;
    struct s1517CFC4_a *p;

    if (D_800DDD20 != 0) {
        for (i = 0; i < 15; i++) {
            ((u8 (*)[16])&D_800DDA90)[i][0xC] = 0;
        }
        func_1517BBAC(D_800CC2D0->x_position, D_800CC2D0->y_position, D_800CC2D0->z_position, 1500,
                      D_800DBFF0->unk380, D_800DBFF0->unk2F8, D_800DBFF0->unk2FC, D_800DBFF0->unk300);
        for (i = 0; i < 15; i++) {
            if (((s32 *)&D_800DD478)[i * 26] != 0 && ((u8 (*)[16])&D_800DDA90)[i][0xC] == 0) {
                ((s32 *)&D_800DD478)[i * 26] = 0;
                ((s32 (*)[4])&D_800DDA90)[i][2] = 0;
            }
        }
    }
}


void func_1517E05C(s32 arg0, s32 arg1, s32 arg2) {
    D_800DD470.x = arg0;
    D_800DD470.y = arg1;
    D_800DD470.z = arg2;
}
