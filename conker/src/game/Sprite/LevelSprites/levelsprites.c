#include <ultra64.h>
#include "structs.h"

/* A billboard queued by sprite_queueBillboard and drawn by sprite_drawQueuedBillboards. */
struct Entry1517D578 {
    s16 x;
    s16 y;
    s16 z;
    u8  frame; /* texture frame (from the gBillboardAnimCounter animation counter) */
    u16 halfWidth;
    u16 height;
    f32 angle;
};

/*
 * .bss, linked at 0x800DD470..0x800DDD60 in this order. Defined before conker/sprite.h declares them: IDO
 * lays out .bss in first-declaration order, and an extern there would count. The struct tags of
 * SpriteTexSize and Game1A89B0Position are completed by conker/sprite.h.
 */
struct { s16 x; s16 y; s16 z; } gSpriteDisturbPoint; /* set by sprite_setDisturbPoint; x == 0 means unset */
s32 gSpriteTypeFrames[15 * 26]; // matching: an int that holds an address; a pointer type changes IDO's registers
ParticleTexFile gSpriteTypeTexFiles[15];
struct SpriteTexSize gSpriteTypeTexSizes[15];
u8 gSpriteTypeTexInfoIds[15];
u8 gSpriteTypeFrameCounts[15];
u8 gSpriteTypeDisturbFrames[15];
u8 gSpriteTypePasses[15];
s32 D_800DDC00;
s32 gSpritePingPongTypes;
s32 gSpriteRandomFlipTypes;
u8 gSpritePingPongDirs[15];
u8 D_800DDC20[15][2];
u32 gSpriteTypeNearest[15]; /* per sprite type: distance to the nearest sprite this frame */
u8 gSpriteTypeKindRows[15];
s16 gSpriteTypePhases[60];
s32 D_800DDD08; /* not referenced */
s32 gSpriteMaxPerFrame;
Vtx *gSpriteVtxBuffers[2];
struct Game1A89B0Position *gLevelSprites;
u8 gBillboardAnimCounter;
s32 gLevelSpriteCount;
s32 gBillboardTexture; /* texture strip of the billboards */
struct Entry1517D578 gBillboardQueue[3];
s32 gSpriteVtxCount; /* sprites in this frame's vertex buffer */

#include <math.h>
#include "conker/actor.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/rom.h"
#include "conker/sprite.h"
#include "conker/swarm.h"
#include "conker/texture.h"

u8 gBillboardQueueCount = 0;
s32 gLevelSpriteSortAxis = 0;

SpriteKind gSpriteKinds[6] = {
    { sprite_scatterParticles, NULL, 0 },
    { sprite_placeCentreParticle, sprite_animJitterHeight, 0 },
    { sprite_scatterParticles87, sprite_animFadeOffset, 0 },
    { sprite_scatterParticles8, func_1517B7A8, 0 },
    { sprite_placeParticlePattern, NULL, 0 },
    { sprite_scatterParticles33, sprite_animHop, 0 },
};

SpriteKindChoice gSpriteKindChoices[5][8] = {
    { { 0, 1 }, { 0, 2 }, { 4, 0 }, { 0, 4 }, { 4, 1 }, { 0, 6 }, { 4, 2 }, { 0, 8 } },
    { { 1, 500 }, { 1, 250 }, { 1, 125 }, { 1, 62 }, { 1, 46 }, { 1, 31 }, { 1, 15 }, { 0, 1 } },
    { { 2, 1 }, { 2, 2 }, { 2, 3 }, { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 0, 1 } },
    { { 3, 1 }, { 3, 2 }, { 3, 3 }, { 3, 4 }, { 3, 5 }, { 3, 6 }, { 3, 7 }, { 0, 1 } },
    { { 5, 1 }, { 5, 2 }, { 5, 3 }, { 5, 4 }, { 5, 5 }, { 5, 6 }, { 0, 1 }, { 0, 3 } },
};

/* Render setup of the sprites and billboards. */
static Gfx gLevelSpriteRenderDl[] = {
    gsSPMatrix(&D_89470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsDPPipeSync(),
    gsSPClearGeometryMode(0xFFFF),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_LOD),
    gsDPSetOtherMode(0x182C3F, 0x0C184B50),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL1, COMBINED, ENVIRONMENT, SHADE, PRIMITIVE, COMBINED, 0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, 1, G_ON),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};

/*
 * Sprite animation callbacks (the anim functions of gSpriteKinds): each copies the sprite's position into its first
 * vertex, possibly offset, and may advance the animation phase or lower the alpha. Nonzero hides the sprite.
 */

/* Advances the phase by half again, wrapping at the frame count, and drifts the height toward 41 with jitter. */
s32 sprite_animJitterHeight(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
    s32 diff;
    s32 limit;
    s32 rnd;

    *phase = (*phase * 3) / 2;
    limit = gParticleTexInfos[pos->texInfo].frameCount << 8;
    if (*phase >= limit) {
        *phase -= limit;
    }

    vtx->v.ob[0] = pos->x;
    vtx->v.ob[1] = pos->y;
    vtx->v.ob[2] = pos->z;

    diff = 41 - pos->height;
    if (diff == 0) {
        pos->height -= (math_rand() % 8) - 4;
    } else if (diff < 0) {
        rnd = math_rand();
        pos->height -= ((math_rand() % -diff) >> 1) + (rnd % 8) - 4;
    } else {
        rnd = math_rand();
        pos->height += ((math_rand() % diff) >> 1) + (rnd % 8) - 4;
    }

    vtx->v.ob[0] = pos->x;
    vtx->v.ob[1] = pos->y;
    vtx->v.ob[2] = pos->z;
    return 0;
}

/* Once the phase reaches 0x600, offsets the vertex by it, fades the alpha and holds the phase at 0x600. */
s32 sprite_animFadeOffset(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
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
s32 sprite_animHop(Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha) {
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

/* Quicksorts gLevelSprites[first..last] by x, or by z when gLevelSpriteSortAxis is set (pivot = last element). */
void sprite_sortLevelSprites(s32 first, s32 last) {
    s32 left;
    s32 right;
    s32 pivot;

    left = first;
    if (first < last) {
        right = last;
        while (1) {
            if (gLevelSpriteSortAxis == 0) {
                pivot = gLevelSprites[last].x;
                while (gLevelSprites[left].x < pivot && left < last) left++;
                while (pivot < gLevelSprites[right].x && first < right) right--;
            } else {
                pivot = gLevelSprites[last].z;
                while (gLevelSprites[left].z < pivot && left < last) left++;
                while (pivot < gLevelSprites[right].z && first < right) right--;
            }
            if (left < right) {
                {
                    Game1A89B0Position swap = gLevelSprites[left];
                    gLevelSprites[left] = gLevelSprites[right];
                    gLevelSprites[right] = swap;
                }
                left++;
                right--;
            } else {
                break;
            }
        }
        {
            Game1A89B0Position pivotSwap = gLevelSprites[left];
            gLevelSprites[left] = gLevelSprites[last];
            gLevelSprites[last] = pivotSwap;
            sprite_sortLevelSprites(first, left - 1);
            sprite_sortLevelSprites(left + 1, last);
        }
    }
}

/* Per sprite type: the texture file (gSpriteTypeTexFiles) and the 26 frame pointers into its decoded buffer. */
#define FRAMES_PER_TYPE 26

/* This frame's sprite vertex buffer, four vertices per sprite. */
#define VTX(n) (((Vtx *)(s32)gSpriteVtxBuffers[gDrawBufferIndex])[n]) // matching: the int round trip keeps the original addressing

/* Whether sorted sprite k is still inside the box, walking from its near edge in direction dir along the axis. */
#define IN_WINDOW(k)                                                                \
    ((axis == 0 && ((dir == 1 && (k) < gLevelSpriteCount && gLevelSprites[k].x < maxX) ||   \
                    (dir == -1 && (k) >= 0 && minX < gLevelSprites[k].x))) ||         \
     (axis != 0 && ((dir == 1 && (k) < gLevelSpriteCount && gLevelSprites[k].z < maxZ) ||   \
                    (dir == -1 && (k) >= 0 && minZ < gLevelSprites[k].z))))

/*
 * Builds this frame's sprite vertex buffer: walks the sprites sorted along the axis facing `angle`
 * (re-sorting when that axis changes) from the near edge of the box of half-size `radius` around (x, y, z),
 * and for each one within `radius` loads its type's texture on demand, runs its animation callback and
 * fills four vertices (alpha falls off with distance; sprites within 140 of (fadeX, fadeY, fadeZ) are
 * skipped, fading in up to 240). Then steps the per-type animation phases and turns every quad to face
 * `angle`.
 */
// matching: hi, fadeDist, j and y are reused for unrelated values; separate locals change IDO's allocation
void sprite_buildLevelSprites(s32 x, s32 y, s32 z, s32 radius, f32 angle, s32 fadeX, s32 fadeY, s32 fadeZ) {
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

    gSpriteVtxCount = 0;
    maxX = x + radius; minX = x - radius;
    maxZ = z + radius; minZ = z - radius;
    dist = gLevelSpriteCount - 1; hi = dist; // matching: routing through dist (and below) steers IDO's allocation
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
    if (axis != gLevelSpriteSortAxis) {
        gLevelSpriteSortAxis = axis;
        dist = gLevelSpriteCount - 1;
        sprite_sortLevelSprites(0, dist);
    }
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (axis == 0) {
            if (dir == 1) {
                if (minX < gLevelSprites[mid].x) {
                    hi = mid - 1;
                } else if (gLevelSprites[mid].x < minX) {
                    lo = mid + 1;
                } else if (minX == gLevelSprites[mid].x) {
                    break;
                }
            } else {
                if (maxX < gLevelSprites[mid].x) {
                    hi = mid - 1;
                } else if (gLevelSprites[mid].x < maxX) {
                    lo = mid + 1;
                } else if (maxX == gLevelSprites[mid].x) {
                    break;
                }
            }
        } else {
            if (dir == 1) {
                if (minZ < gLevelSprites[mid].z) {
                    hi = mid - 1;
                } else if (gLevelSprites[mid].z < minZ) {
                    lo = mid + 1;
                } else if (minZ == gLevelSprites[mid].z) {
                    break;
                }
            } else {
                if (maxZ < gLevelSprites[mid].z) {
                    hi = mid - 1;
                } else if (gLevelSprites[mid].z < maxZ) {
                    lo = mid + 1;
                } else if (maxZ == gLevelSprites[mid].z) {
                    break;
                }
            }
        }
    }
    /* nearest distance per sprite type */
    for (nearest = gSpriteTypeNearest; nearest < &gSpriteTypeNearest[15]; nearest++) {
        *nearest = 0x7FFF;
    }
    i = mid;
    if (IN_WINDOW(mid)) {
        j = ((u32) gLevelSpriteCount < gSpriteMaxPerFrame) ? gLevelSpriteCount : gSpriteMaxPerFrame;
        gSpriteVtxBuffers[gDrawBufferIndex] = allocate_memory(j << 6, 4, 2, 0);
        if (gSpriteVtxBuffers[gDrawBufferIndex] == NULL) {
            return;
        }
    } else {
        gSpriteVtxBuffers[gDrawBufferIndex] = NULL;
        return;
    }
    for (j = 0; (u32) j < gSpriteMaxPerFrame && IN_WINDOW(i); i += dir) {
        dist = math_distance3D(gLevelSprites[i].x - x, gLevelSprites[i].y - y, gLevelSprites[i].z - z);
        if (dist < radius) {
            type = gLevelSprites[i].unk6 & 0xF;
            if (dist < gSpriteTypeNearest[type]) {
                gSpriteTypeNearest[type] = dist;
            }
            fadeDist = math_distance3D(gLevelSprites[i].x - fadeX, gLevelSprites[i].y - fadeY, gLevelSprites[i].z - fadeZ);
            if (fadeDist > 140) {
                if (fadeDist < 240) {
                    dist = radius - ((radius - dist) * (fadeDist - 140)) / 100;
                }
                if (gSpriteTypeFrames[type * FRAMES_PER_TYPE] == 0) {
                    if (gSpriteTypeTexFiles[type].devAddr != 0) {
                        hi = gParticleTexInfos[gSpriteTypeTexInfoIds[type]].frameSize * gParticleTexInfos[gSpriteTypeTexInfoIds[type]].frameCount;
                        gSpriteTypeTexFiles[type].buf = heap_alloc(gSpriteTypeTexFiles[type].size, 1, 2, 1, 0);
                        if (gSpriteTypeTexFiles[type].buf != 0) {
                            gSpriteTypeFrames[type * FRAMES_PER_TYPE] = heap_alloc(hi, 4, 1, 0, 0);
                            if (gSpriteTypeFrames[type * FRAMES_PER_TYPE] != 0) {
                                rom_read(gSpriteTypeTexFiles[type].devAddr, gSpriteTypeTexFiles[type].buf, gSpriteTypeTexFiles[type].size, 1);
                                gSpriteTypeTexFiles[type].used = 1;
                                hi = gParticleTexInfos[gSpriteTypeTexInfoIds[type]].frameSize;
                                for (frame = 1; frame < FRAMES_PER_TYPE; frame++) {
                                    gSpriteTypeFrames[type * FRAMES_PER_TYPE + frame] = gSpriteTypeFrames[type * FRAMES_PER_TYPE + frame - 1] + hi;
                                }
                            } else {
                                heap_free(gSpriteTypeTexFiles[type].buf);
                            }
                        }
                    }
                texdone:; // matching: IDO needs this empty labelled statement (a block join) here
                } else {
                    if (gSpriteTypeTexFiles[type].used == 0) {
                        gSpriteTypeTexFiles[type].used = 1;
                        heap_setTag((s32 *) gSpriteTypeFrames[type * FRAMES_PER_TYPE], 4);
                    }
                }
                vtx = &VTX(j << 2);
                phase = gSpriteTypePhases[gLevelSprites[i].phaseIdx];
                if (phase < 0) {
                    phase = 0;
                }
                if (gSpriteDisturbPoint.x != 0) {
                    if (math_distance3D(gLevelSprites[i].x - gSpriteDisturbPoint.x, gLevelSprites[i].y - gSpriteDisturbPoint.y,
                                      gLevelSprites[i].z - gSpriteDisturbPoint.z) < 80 &&
                        gSpriteTypeTexInfoIds[type] == 0) {
                        if (gLevelSprites[i].disturbState != 0x21) {
                            gLevelSprites[i].disturbState = 0x21;
                            func_15170500(gLevelSprites[i].x, gLevelSprites[i].y + 68, gLevelSprites[i].z, 0, 0, 255, 0);
                        }
                    }
                }
                if (gLevelSprites[i].disturbState >= 0x21) {
                    if (gLevelSprites[i].disturbState >= 0x22) {
                        gLevelSprites[i].disturbState--;
                    }
                    vtx[3].v.flag = gLevelSprites[i].height;
                } else {
                    if (dist < 48 && gSpriteTypeTexInfoIds[type] == 0) {
                        gLevelSprites[i].disturbState = 0x22;
                        func_15170500(gLevelSprites[i].x, gLevelSprites[i].y + 68, gLevelSprites[i].z, 0, 0, 255, 0);
                    }
                    vtx[3].v.flag = gLevelSprites[i].height;
                }
                vtx[2].v.flag = gLevelSprites[i].halfWidth;
                dist = (u32) (radius - dist) >> 1;
                if (dist >= 256) {
                    dist = 255;
                }
                vtx[0].v.cn[3] = dist;
                if (gSpriteKinds[gSpriteKindChoices[gSpriteTypeKindRows[type]][gLevelSprites[i].kind].emitter].anim != NULL) {
                    if (gSpriteKinds[gSpriteKindChoices[gSpriteTypeKindRows[type]][gLevelSprites[i].kind].emitter].anim(&gLevelSprites[i], vtx, &phase, &vtx[0].v.cn[3]) != 0) {
                        continue;
                    }
                } else {
                    vtx[0].v.ob[0] = gLevelSprites[i].x;
                    vtx[0].v.ob[1] = gLevelSprites[i].y;
                    vtx[0].v.ob[2] = gLevelSprites[i].z;
                }
                shade = gLevelSprites[i].unk6 | 0x9F;
                fadeDist = vtx[0].v.cn[3] & 0xFF;
                vtx[0].v.cn[0] = vtx[0].v.cn[1] = vtx[0].v.cn[2] = shade;
                vtx[1].v.cn[0] = vtx[1].v.cn[1] = vtx[1].v.cn[2] = shade;
                vtx[2].v.cn[0] = vtx[2].v.cn[1] = vtx[2].v.cn[2] = shade;
                vtx[3].v.cn[0] = vtx[3].v.cn[1] = vtx[3].v.cn[2] = shade;
                vtx[1].v.cn[3] = vtx[2].v.cn[3] = vtx[3].v.cn[3] = fadeDist;
                if (gLevelSprites[i].disturbState >= 0x21) {
                    vtx[0].v.flag = gSpriteTypeDisturbFrames[type] + type * FRAMES_PER_TYPE;
                } else {
                    vtx[0].v.flag = type * FRAMES_PER_TYPE + (phase >> 8);
                }
                vtx++;
                vtx->v.flag = gSpriteTypePasses[type];
                VTX((j << 2) + 1).v.tc[0] =
                    VTX((j << 2) + 2).v.tc[0] = gSpriteTypeTexSizes[type].s;
                VTX((j << 2) + 2).v.tc[1] =
                    VTX((j << 2) + 3).v.tc[1] = gSpriteTypeTexSizes[type].t;
                j++;
            }
        }
    }
    gSpriteVtxCount = j;
    /* step each type's 4 phases: ping-pong (types in gSpritePingPongTypes, gSpriteRandomFlipTypes adds random flips) or loop */
    if (gPaused == 0) {
        for (i = 0; i != 15; i++) {
            hi = (gSpriteTypeFrameCounts[i] << 8) - 1;
            if (hi > 0x100) {
                y = 1 << i;
                if (y & gSpritePingPongTypes) {
                    if (y & gSpriteRandomFlipTypes) {
                        step = math_rand() % 10;
                        if (step == 0) {
                            gSpritePingPongDirs[i] |= math_rand() % 16;
                        } else if (step == 1) {
                            gSpritePingPongDirs[i] &= math_rand() % 16;
                        } else {
                            step = gFrameTicks << 5;
                        }
                    } else {
                        step = gFrameTicks << 6;
                    }
                    fadeDist = i << 2;
                    for (j = 0; j != 4; j++, fadeDist++) {
                        bits = gSpritePingPongDirs[i];
                        y = 1 << j;
                        if (bits & y) {
                            gSpriteTypePhases[fadeDist] -= step;
                            if (gSpriteTypePhases[fadeDist] <= 0) {
                                gSpriteTypePhases[fadeDist] = -gSpriteTypePhases[fadeDist];
                                gSpritePingPongDirs[i] = bits ^ y;
                            }
                        } else {
                            gSpriteTypePhases[fadeDist] += step;
                            if (gSpriteTypePhases[fadeDist] >= hi) {
                                gSpriteTypePhases[fadeDist] = hi * 2 - gSpriteTypePhases[fadeDist];
                                gSpritePingPongDirs[i] = bits ^ y;
                            }
                        }
                    }
                } else {
                    bits = gFrameTicks * 48;
                    fadeDist = i << 2;
                    for (j = 0; j != 4; j++, fadeDist++) {
                        gSpriteTypePhases[fadeDist] += bits;
                        if (gSpriteTypePhases[fadeDist] >= hi) {
                            gSpriteTypePhases[fadeDist] -= hi;
                        }
                    }
                }
            }
        }
    }
    /* occasionally spawn a particle at a random sprite showing frame 0 */
    i = (u32) math_rand() % gSpriteMaxPerFrame;
    if ((u32) i < gSpriteVtxCount) {
        i *= 4;
        j = VTX(i).v.flag;
        if (j % 26 == 0 && gSpriteTypeTexInfoIds[j / 26] == 0) {
            func_15170500(VTX(i).v.ob[0], VTX(i).v.ob[1] + 0x44, VTX(i).v.ob[2], 0, 1, 0xFF, 0);
        }
    }
    /* turn each quad to face `angle`: halfWidth across, height * 4 up */
    angle *= 0.01745329238f;
    cosA = cosf(angle) * 128.0f;
    sinA = sinf(angle);
    for (i = 0; (u32) i < gSpriteVtxCount; i++) {
        halfWidth = VTX((i << 2) + 2).v.flag;
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
    gSpriteDisturbPoint.x = 0;
}

/* Releases every sprite type's texture file buffer, first passing it with the frames to boot_decompress. */
void sprite_freeTypeTextures(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (gSpriteTypeTexFiles[i].buf != 0) {
            if (gSpriteTypeFrames[i * FRAMES_PER_TYPE] != 0) {
                boot_decompress(gSpriteTypeTexFiles[i].buf, gSpriteTypeFrames[i * FRAMES_PER_TYPE], gDecompressWork);
                heap_free(gSpriteTypeTexFiles[i].buf);
            }
            gSpriteTypeTexFiles[i].buf = 0;
        }
    }
}


/* Draws one textured billboard quad at (x, y, z) facing `angle` into the sprite vertex buffer's next slot. */
Gfx *sprite_drawBillboard(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 texWidth, u8 texHeight, s32 texture) {
    u32 count;
    s16 dx;
    s16 dz;
    Vtx *v;

    count = gSpriteVtxCount;
    v = &VTX(count * 4);
    if ((u32)gSpriteVtxCount >= gSpriteMaxPerFrame) {
        return gfx;
    }

    gSPDisplayList(gfx++, gLevelSpriteRenderDl);
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
    gSpriteVtxCount++;
    return gfx;
}

/* Queues a billboard (at most 3 per frame; gBillboardQueueCount counts them). */
void sprite_queueBillboard(s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 frame) {
    struct Entry1517D578 *e;
    u8 idx = gBillboardQueueCount;

    if (gBillboardQueueCount < 3) {
        e = &gBillboardQueue[idx];
        e->x = x;
        e->y = y;
        e->z = z;
        e->angle = angle;
        e->halfWidth = halfWidth;
        e->height = height;
        e->frame = frame;
        gBillboardQueueCount = idx + 1;
    }
}

/* Queues a billboard facing player `player`'s view angle, showing the current animation frame. */
void sprite_queuePlayerBillboard(s16 x, s16 y, s16 z, s32 player, s32 halfWidth, s32 height) {
    sprite_queueBillboard(x, y, z, camera_global_Cameras[player].TrueCameraAngle, halfWidth, height, gBillboardAnimCounter >> 3);
}

/*
 * Advances the billboard animation counter and drains the queue, drawing each billboard with its frame of
 * the 16x32 texture strip at `texture` (nothing is drawn when it is 0). Leaves the sprite count unchanged.
 */
Gfx *sprite_drawQueuedBillboards(Gfx *gfx, s32 texture) {
    s32 saved;

    saved = gSpriteVtxCount;
    if ((gBillboardAnimCounter += 5) >= 200) {
        gBillboardAnimCounter -= 200;
    }

    while (gBillboardQueueCount != 0) {
        gBillboardQueueCount--;
        if (texture != 0) {
            /* frames are 0x500 bytes apart */
            gfx = sprite_drawBillboard(gfx, gBillboardQueue[gBillboardQueueCount].x, gBillboardQueue[gBillboardQueueCount].y, gBillboardQueue[gBillboardQueueCount].z,
                                gBillboardQueue[gBillboardQueueCount].angle, gBillboardQueue[gBillboardQueueCount].halfWidth,
                                gBillboardQueue[gBillboardQueueCount].height, 16, 32,
                                ((gBillboardQueue[gBillboardQueueCount].frame * 0x280) << 1) + texture); // matching: `* 0x500` codegens differently
        }
    }

    gSpriteVtxCount = saved;
    return gfx;
}

/*
 * Draws the queued billboards (sprite_drawQueuedBillboards), then every sprite built by sprite_buildLevelSprites whose second
 * vertex's flag equals `pass`, loading each texture frame once per batch.
 */
void sprite_drawLevelSprites(Gfx **gfx, s32 pass) {
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
    if (gSpriteVtxBuffers[gDrawBufferIndex] == NULL) {
        return;
    }
    *gfx = sprite_drawQueuedBillboards(*gfx, gBillboardTexture);
    if (gLevelSpriteCount == 0 || gSpriteVtxCount == 0) {
        return;
    }
    quad = 0;
    gSPDisplayList((*gfx)++, gLevelSpriteRenderDl);
    *gfx = texture_setTintColors(*gfx, 0xFF, 0xFF, 0);
    sprite = 0;
    /* Batches of up to 4 sprites (16 vertices); within a batch draw the quads grouped by texture. */
    while (sprite < gSpriteVtxCount) {
        done = 0;
        vtxCount = (gSpriteVtxCount - sprite >= 5) ? 16 : (gSpriteVtxCount - sprite) * 4;
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
                        gDPLoadTextureBlock((*gfx)++, gSpriteTypeFrames[tex], G_IM_FMT_RGBA, G_IM_SIZ_16b, width,
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
void sprite_updateLevelSprites(void) {
    s32 i;

    if (gLevelSpriteCount != 0) {
        for (i = 0; i < 15; i++) {
            gSpriteTypeTexFiles[i].used = 0;
        }
        sprite_buildLevelSprites(movement_obj->xpos, movement_obj->ypos, movement_obj->zpos, 1500,
                      camera_global_Cameras->TrueCameraAngle, camera_global_Cameras->eye.x, camera_global_Cameras->eye.y, camera_global_Cameras->eye.z);
        for (i = 0; i < 15; i++) {
            if (gSpriteTypeFrames[i * FRAMES_PER_TYPE] != 0 && gSpriteTypeTexFiles[i].used == 0) {
                gSpriteTypeFrames[i * FRAMES_PER_TYPE] = 0;
                gSpriteTypeTexFiles[i].buf = 0;
            }
        }
    }
}


/* Sets this frame's disturbance point: sprites within 80 of it react as if the player were close. */
void sprite_setDisturbPoint(s32 x, s32 y, s32 z) {
    gSpriteDisturbPoint.x = x;
    gSpriteDisturbPoint.y = y;
    gSpriteDisturbPoint.z = z;
}
