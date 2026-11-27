#include <ultra64.h>

#include "conker/collision.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/math.h"
#include "conker/rom.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/swarm.h"

/* Particle slot state (15 slots). */
static u16 gParticleSlotParity;               /* parity bits of light_placeParticlePattern, one per slot */
static s16 gParticleSlotScales[15][2];        /* base x/y scale of each slot */
static u16 gParticleSlotTriCounts[15];           /* triangles placed per slot */
static u8 D_800BE498[8];             /* not referenced */
static ParticleResource gParticleSlotResources[15]; /* this level's resource per slot */

/* Particle patterns of light_placeParticlePattern: a pair count, then the x/z offset of each particle. */
static const f32 gParticlePattern3[] = { 3.0f, 32.0f, 22.0f, -30.0f, 25.0f, 0.0f, -28.0f };
static const f32 gParticlePattern5[] = { 5.0f, 32.0f, 22.0f, -30.0f, 25.0f, 0.0f, 0.0f, 27.0f, -27.0f, -22.0f, -30.0f };
static const f32 gParticlePattern7[] = {
    7.0f, 32.0f, 22.0f, -30.0f, 25.0f, 0.0f, 0.0f, 27.0f, -27.0f, -22.0f, -30.0f, 50.0f, 5.0f, -47.0f, -5.0f,
};

/* Pairs of barycentric weights for the particles that light_scatterParticles scatters over a triangle. */
static const f32 gParticleScatterWeights[] = {
    0.5f, 0.5f, 0.3f, 0.12f, 0.7f, 0.88f, 0.9f, 0.4f, 0.1f, 0.6f, 0.6f, 0.25f, 0.3f, 0.75f, 0.8f, 0.2f,
};

/* Not referenced. */
static const u8 D_80095CA4[32] = {
    0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 7, 15, 4, 12, 3, 11, 5, 6, 0, 0, 2, 2, 0, 0, 0, 0,
};

/* Initial value of both D_800DDC20 counters of each slot. */
static const u8 gParticleSlotCounterInits[15] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

/* The patterns of light_placeParticlePattern. */
static const f32 *gParticlePatterns[] = { gParticlePattern3, gParticlePattern5, gParticlePattern7 };

/** Clears the particle slot tables and picks this level's resources. */
void light_initParticleSlots(s32 arg0) {
    bzero(gParticleSlotResources, sizeof(gParticleSlotResources));
    rom_loadPathInto(gParticleSlotResources, sizeof(gParticleSlotResources), 3, 12, arg0, 10);
    bzero(gSpriteTypeTexFiles, sizeof(gSpriteTypeTexFiles));
    bzero(gSpriteTypeFrames, sizeof(gSpriteTypeFrames));
}

/** Looks up the texture file of every used particle slot and computes its texture extents. */
void light_loadParticleTextures(void) {
    s32 i;
    s32 id;
    s32 size;
    u32 length;

    for (i = 0; i < 15; i++) {
        gSpriteTypeTexFiles[i].devAddr = 0;
        gSpriteTypeTexFiles[i].buf = 0;
        gSpriteTypeTexFiles[i].size = 0;
        gSpriteTypeTexFiles[i].used = 0;
        id = gParticleSlotResources[i].id;
        if (id != 0xFF && (gParticleSlotTriCounts[i] != 0 || gParticleSlotResources[i].id == 0x3B)) {
            gSpriteTypeTexFiles[i].devAddr = (s32)rom_findPath(&size, 2, 0, id);
            gSpriteTypeTexFiles[i].size = (size + 15) & ~15;
            length = gParticleTexInfos[id].frameSize;
            if (size == 0 || length == 0) {
                length = 0;
                gSpriteTypeTexFiles[i].devAddr = 0;
            }
            gSpriteTypeTexSizes[i].s = (gParticleTexInfos[id].width << 5) - 1;
            gSpriteTypeTexSizes[i].t = (((length / gParticleTexInfos[id].width) << 6) / 5) - 1;
        }
    }
}

/** Sets up particle slot `slot` for resource `id` (0xFF: unused). */
void light_setupParticleSlot(s32 slot, s32 id, s32 flags) {
    const ParticleTexInfo *info;
    u8 *frames;
    s32 infoFlags;

    if (id != 255) {
        frames = &gSpriteTypeFrameCounts[slot];
        info = &gParticleTexInfos[id];
        *frames = info->frameCount;
        infoFlags = info->flags;
        if (infoFlags & 0x10) {
            *frames -= 1;
            gSpriteTypeDisturbFrames[slot] = *frames;
        } else {
            gSpriteTypeDisturbFrames[slot] = 0;
        }
        if (infoFlags & 0x80) {
            gSpriteTypePasses[slot] = 2;
        } else {
            gSpriteTypePasses[slot] = 1;
        }
        gParticleSlotScales[slot][0] = info->scaleX;
        gParticleSlotScales[slot][1] = info->scaleY;
        gSpriteTypeTexInfoIds[slot] = id;
        gSpriteTypeKindRows[slot] = flags & 0xFF7F;
        if (flags & 0x80) {
            D_800DDC00 |= 1 << slot;
        }
        if (infoFlags & 1) {
            gSpritePingPongTypes |= 1 << slot;
        }
        if (infoFlags & 8) {
            gSpriteRandomFlipTypes |= 1 << slot;
        }
    } else {
        gSpriteTypeFrameCounts[slot] = 0;
    }
    gParticleSlotTriCounts[slot] = 0;
    D_800DDC20[slot][0] = gParticleSlotCounterInits[slot];
    D_800DDC20[slot][1] = gParticleSlotCounterInits[slot];
}


/** Fills particle `index` for particle slot `slot`; returns the next free index. */
s32 light_addLevelParticle(s32 index, s32 slot, s32 subtype, s16 x, s16 y, s16 z, s32 jitter, u8 kind, s32 parity) {
    s16 scale;

    if (index >= (u32)(gLevelHeader->maxParticles - 1)) {
        return gLevelHeader->maxParticles - 1;
    }

    gLevelSprites[index].x = x;
    gLevelSprites[index].y = y;
    gLevelSprites[index].z = z;

    if (gSpriteTypeFrameCounts[slot] >= 2) {
        if ((1 << slot) & D_800DDC00) {
            gLevelSprites[index].phaseIdx = (slot << 2) + (parity & 1);
        } else {
            gLevelSprites[index].phaseIdx = (index & 3) + (slot << 2);
        }
    } else {
        gLevelSprites[index].phaseIdx = 60;
    }

    gLevelSprites[index].unk6 = (subtype << 5) | slot;
    gLevelSprites[index].kind = kind;
    gLevelSprites[index].texInfo = gSpriteTypeTexInfoIds[slot];

    if (jitter != 0) {
        // matching: the "+ jitter - jitter" pair is needed for the original instruction sequence
        scale = (math_rand() % jitter + jitter) - jitter + 0x100;
        gLevelSprites[index].halfWidth = (gParticleSlotScales[slot][0] * scale) >> 10;
        if (gParticleTexInfos[gSpriteTypeTexInfoIds[slot]].flags & 4) {
            scale = math_rand() % jitter * 4 + scale;
        }
        gLevelSprites[index].height = (gParticleSlotScales[slot][1] * scale) >> 10;
    } else {
        gLevelSprites[index].halfWidth = gParticleSlotScales[slot][0] >> 2;
        gLevelSprites[index].height = gParticleSlotScales[slot][1] >> 2;
    }
    return index + 1;
}

/** Coordinate `axis` of vertex `k` of the mesh triangle `tri`. */
#define VTX(tri, k, axis) ((tri)->verts[k]->v.ob[axis])
/** Linear interpolation from a to b by t, with the difference computed in the integer type of a and b. */
#define LERP_INT(a, b, t) ((f32)(a) + (f32)((b) - (a)) * (t))
/** Linear interpolation from a to b by t. */
#define LERP(a, b, t) ((a) + ((b) - (a)) * (t))

/** Scatters `count` particles over the triangle `tri` at the barycentric weights of gParticleScatterWeights. */
s32 light_scatterParticles(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    s32 i;
    s32 n;
    s32 next;
    const f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = gParticleScatterWeights;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = light_addLevelParticle(index, slot, subtype,
                                 LERP(LERP_INT(VTX(tri, 0, 0), VTX(tri, 1, 0), w0), VTX(tri, 2, 0), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 1), VTX(tri, 1, 1), w0), VTX(tri, 2, 1), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 2), VTX(tri, 1, 2), w0), VTX(tri, 2, 2), w1),
                                 0x80, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    return index;
}

/**
 * Places the particles of pattern `pattern` (gParticlePatterns: a pair count, then x/z offsets) around the
 * centre of triangle `tri`, projecting each onto the triangle's plane (computed by math_computeTriPlane).
 */
s32 light_placeParticlePattern(s16 pattern, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    u32 i;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    f32 x;
    f32 y;
    f32 z;
    Tri3f triangle;

    centerX = 0.0f;
    centerY = 0.0f;
    centerZ = 0.0f;

    for (i = 0; i < 3; i++) {
        triangle.pts[i].x = VTX(tri, i, 0);
        centerX += triangle.pts[i].x / 3.0f;
        triangle.pts[i].y = VTX(tri, i, 1);
        centerY += triangle.pts[i].y / 3.0f;
        triangle.pts[i].z = VTX(tri, i, 2);
        centerZ += triangle.pts[i].z / 3.0f;
    }

    math_computeTriPlane(triangle);

    for (i = 1; i <= gParticlePatterns[pattern][0] * 2;) {
        x = gParticlePatterns[pattern][i++] + centerX;
        z = gParticlePatterns[pattern][i++] + centerZ;
        if (gTriPlaneNormalY != 0.0f) {
            y = ((gTriPlaneDist - gTriPlaneNormalX * x) - gTriPlaneNormalZ * z) / gTriPlaneNormalY;
        } else {
            y = centerY;
        }
        index = light_addLevelParticle(index, slot, subtype, x, y, z, 0x80, kind, gParticleSlotParity >> slot);
    }

    gParticleSlotParity ^= 1 << slot;
    return index;
}

/** Places one particle at the centre of triangle `tri` and starts positional sound 0x4D there. */
s32 light_placeCentreParticle(s16 amount, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    u16 clamped;
    s32 x;
    s32 y;
    s32 z;

    clamped = amount;

    x = (VTX(tri, 0, 0) + VTX(tri, 1, 0) + VTX(tri, 2, 0)) / 3;
    y = (VTX(tri, 0, 1) + VTX(tri, 1, 1) + VTX(tri, 2, 1)) / 3;
    z = (VTX(tri, 0, 2) + VTX(tri, 1, 2) + VTX(tri, 2, 2)) / 3;

    index = light_addLevelParticle(index, slot, subtype, x, y, z, 0x80, kind, index);

    if ((u16)amount > 62.5f) {
        clamped = 62;
    }

    sound_addDynamicSound(0x4D, x, y, z, 0x8000, clamped, clamped >> 3, 0, 0, 0, 0, 0);
    return index;
}
/**
 * Like light_scatterParticles without scale jitter, then sets the slot's frame count to 87. It has no return
 * statement: its caller light_buildLevelParticles (through gSpriteKinds) keeps the next index that the last
 * light_addLevelParticle call left in v0.
 */
s32 light_scatterParticles87(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    s32 i;
    s32 n;
    s32 next;
    const f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = gParticleScatterWeights;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = light_addLevelParticle(index, slot, subtype,
                                 LERP(LERP_INT(VTX(tri, 0, 0), VTX(tri, 1, 0), w0), VTX(tri, 2, 0), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 1), VTX(tri, 1, 1), w0), VTX(tri, 2, 1), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 2), VTX(tri, 1, 2), w0), VTX(tri, 2, 2), w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    gSpriteTypeFrameCounts[slot] = 87;
}
/**
 * Like light_scatterParticles without scale jitter, then sets the slot's frame count to 8. It has no return
 * statement: its caller light_buildLevelParticles (through gSpriteKinds) keeps the next index that the last
 * light_addLevelParticle call left in v0.
 */
s32 light_scatterParticles8(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    s32 i;
    s32 n;
    s32 next;
    const f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = gParticleScatterWeights;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = light_addLevelParticle(index, slot, subtype,
                                 LERP(LERP_INT(VTX(tri, 0, 0), VTX(tri, 1, 0), w0), VTX(tri, 2, 0), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 1), VTX(tri, 1, 1), w0), VTX(tri, 2, 1), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 2), VTX(tri, 1, 2), w0), VTX(tri, 2, 2), w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    gSpriteTypeFrameCounts[slot] = 8;
}
/**
 * Like light_scatterParticles without scale jitter, then sets the slot's frame count to 33. It has no return
 * statement: its caller light_buildLevelParticles (through gSpriteKinds) keeps the next index that the last
 * light_addLevelParticle call left in v0.
 */
s32 light_scatterParticles33(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, MeshTri *tri) {
    s32 i;
    s32 n;
    s32 next;
    const f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = gParticleScatterWeights;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = light_addLevelParticle(index, slot, subtype,
                                 LERP(LERP_INT(VTX(tri, 0, 0), VTX(tri, 1, 0), w0), VTX(tri, 2, 0), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 1), VTX(tri, 1, 1), w0), VTX(tri, 2, 1), w1),
                                 LERP(LERP_INT(VTX(tri, 0, 2), VTX(tri, 1, 2), w0), VTX(tri, 2, 2), w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    gSpriteTypeFrameCounts[slot] = 33;
}

void light_resetSpriteBuffers(s32 unused) {
    gSpriteVtxBuffers[0] = NULL;
    gSpriteVtxBuffers[1] = NULL;
    gSpriteMaxPerFrame = 110;
}

/** Resets the particle counters and slot bitmasks. */
void light_resetParticles(void) {
    gBillboardAnimCounter = 0;
    gBillboardQueueCount = 0;
    gSpritePingPongTypes = D_800DDC00 = gSpriteRandomFlipTypes = 0;
    gLevelSpriteCount = 0;
    light_resetSpriteBuffers(0);
}


/**
 * Builds the level's particles: sets up the 15 particle slots, then for every level triangle whose
 * packed word names a slot, runs that slot's emitter for the triangle's kind.
 */
void light_buildLevelParticles(void) {
    s32 i;
    s32 slot;
    u32 packed;
    s32 kind;

    light_resetParticles();
    if (gLevelHeader->maxParticles == 0) {
        return;
    }
    if ((level == LEVEL_THE_WINDMILL) && (gExpansionPak == 0)) {
        return;
    }
    if (level == LEVEL_FRONTEND) {
        return;
    }

    for (i = 0; i < 15; i++) {
        light_setupParticleSlot(i, gParticleSlotResources[i].id, gParticleSlotResources[i].flags);
    }

    gLevelSprites = allocate_memory(gLevelHeader->maxParticles * sizeof(Game1A89B0Position), 1, 2, 0);
    collision_selectMap(0);

    if (gMeshTriMaterials != NULL) {
        for (i = 0; i < gCurMapTriCount; i++) {
            packed = gMeshTriMaterials[i];
            if (packed & 0xF0000) {
                slot = ((packed >> 16) & 0xF) - 1;
                kind = (packed >> 20) & 7;
                gLevelSpriteCount = gSpriteKinds[gSpriteKindChoices[gSpriteTypeKindRows[slot]][kind].emitter].emit(
                    gSpriteKindChoices[gSpriteTypeKindRows[slot]][kind].count, kind, slot,
                    ((packed >> 5) & 3) ^ 3, gLevelSpriteCount, &gCurMapTris[i]);
                gParticleSlotTriCounts[slot]++;
            }
        }
    }

    for (i = 0; i < gLevelSpriteCount; i++) {
        gLevelSprites[i].disturbState = 0;
    }

    gLevelSpriteSortAxis = 0;
    sprite_sortLevelSprites(0, gLevelSpriteCount - 1);

    for (i = 0; i < 60; i++) {
        slot = i >> 2;
        if (gSpriteTypeFrameCounts[slot] != 0) {
            gSpriteTypePhases[i] = (math_rand() % gSpriteTypeFrameCounts[slot]) << 8;
        } else {
            gSpriteTypePhases[i] = 0;
        }
        gSpritePingPongDirs[slot] = math_rand();
    }
}
