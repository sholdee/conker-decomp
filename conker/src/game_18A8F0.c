#include <ultra64.h>
#define func_1515D4D4 func_1515D4D4_prototype
#include "functions.h"
#undef func_1515D4D4
#define D_800DCD40 D_800DCD40_hdr
#define D_800DCD78 D_800DCD78_hdr
#define D_800DCD80 D_800DCD80_hdr
#define D_800DCD84 D_800DCD84_hdr
#include "variables.h"
#undef D_800DCD40
#undef D_800DCD78
#undef D_800DCD80
#undef D_800DCD84

/*
 * A dynamic point light (0x34 bytes). Every allocated light sits in the singly
 * linked list D_800DCD78. struct226 in structs.h is a partial view of this.
 */
typedef struct PointLight {
    /* 0x00 */ struct PointLight *next;
    /* 0x04 */ u8 unk4;              // only lights with 0 get a position / distance
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 unk8;              // matched against the caller's light mask; bit 0 = per-player
    /* 0x09 */ u8 disabled;
    /* 0x0A */ u8 unkA;              // per-player bit: hidden from that player
    /* 0x0B */ u8 group;             // id used by func_1515F170
    /* 0x0C */ u8 unkC;              // per-player bit, cleared every frame by func_1515D69C
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;                // x == -0x8000: (y << 16 | z) is a pointer to an f32[3] position
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 viewPos[4][3];    // position transformed by player i's view matrix
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 radius;
    /* 0x30 */ u8 viewPosValid;      // per-player bit: viewPos[i] is up to date
} PointLight;

/* A point light as the microcode reads it (0x30 bytes, loaded with G_MV_LIGHT). */
typedef struct {
    /* 0x00 */ u8 col[3];
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u8 colc[3];
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s8 dir[3];
    /* 0x0B */ u8 padB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x13];
    /* 0x20 */ s16 pos[4];
    /* 0x28 */ s16 pos2[4];
} PointLightGfx;

extern u8 D_800DCD27;               // priority of the current ambient colour
extern PointLightGfx D_800DCD40;    // padding light for odd light counts
extern PointLight *D_800DCD78;      // list of all point lights
extern struct ZoneNode *D_800DCD80; // ambient colour zones
extern struct ZoneNode *D_800DCD84; // tint zones

void *allocate_memory(s32 size, s32, s32, s32);
f32 func_1515F008(s16 *, s32);
void func_1515F040(f32, s32);
void func_1515F0AC(f32, s32);
extern void func_10004074(void *);

/* The type-0x2E object func_15105C24 finds by owner (Func15105C24Obj in game_131F30.c). */
struct ZoneLinkObj {
    u8 pad0[0x98];
    Actor *unk98;
};

struct ZoneLinkObj *func_15105C24(s32 owner);

/* Allocates a zeroed 0x10-byte block. */
void *func_1515D440(void) {
    void *block;

    block = allocate_memory(0x10, 1, 2, 0);
    bzero(block, 0x10);
    return block;
}

/* Allocates a zeroed array of count 0x60-byte entries. */
void *func_1515D480(s32 count) {
    void *block;

    block = allocate_memory(count * 0x60, 1, 2, 0);
    bzero(block, count * 0x60);
    return block;
}

/* Sets the ambient colour unless a higher-priority one is already set. */
void func_1515D4D4(s32 r, s32 g, s32 b, u8 priority) {
    if (priority >= D_800DCD27) {
        D_800DCD20[0] = r;
        D_800DCD20[1] = g;
        D_800DCD20[2] = b;
        D_800DCD7C = 1;
        D_800DCD27 = priority;
    }
}

/* Allocates a zeroed point light and appends it to the light list. */
PointLight *func_1515D520(void) {
    PointLight *light;
    PointLight *tail;
    PointLight *next;

    light = allocate_memory(sizeof(PointLight), 1, 2, 2);
    if (light != NULL) {
        bzero(light, sizeof(PointLight));
        if (D_800DCD78 != NULL) {
            tail = D_800DCD78;
            next = tail->next;
            while (next != NULL) {
                tail = next;
                next = next->next;
            }
            tail->next = light;
        } else {
            D_800DCD78 = light;
        }
        light->next = NULL;
    }
    return light;
}

PointLight *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group);

/* Wrapper for func_1515D5F8. */
PointLight *func_1515D5AC(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    return func_1515D5F8(x, y, z, radius, r, g, b, arg7, disabled, group);
}

/* Creates a point light at (x, y, z). */
PointLight *func_1515D5F8(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    PointLight *light = func_1515D520();

    if (light != NULL) {
        light->unk4 = 0;
        light->color[0] = r;
        light->color[1] = g;
        light->color[2] = b;
        light->unk8 = arg7;
        light->disabled = disabled;
        light->group = group;
        light->unkA = 0;
        light->x = x;
        light->y = y;
        light->z = z;
        light->radius = radius;
        light->unk2C = 0x7F;
        light->unk2D = 0;
        light->unk2E = 0;
    }
    return light;
}

/* Clears every light's per-frame player masks. */
void func_1515D69C(void) {
    PointLight *light;

    for (light = D_800DCD78; light != NULL; light = light->next) {
        light->unkC = 0;
        light->viewPosValid = 0;
    }
}

void func_1515D6C8(void) {
}

extern s32 D_800DCD10[];
void func_1515EF74(s16 *mtx);

/* G_MOVEWORD index this microcode uses for the light-space scale matrix. */
#define G_MW_LIGHTSCALE 0x10

/* Enables lighting and uploads the light-space scale matrix of player `player`. */
Gfx *func_1515D6D0(Gfx *gfx, s32 player) {
    gMoveWd(gfx++, G_MW_NUMLIGHT, 0, 0);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    func_1515EF74((s16 *)&D_800BE628[player].projectionMtx[D_800BE9C0]);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x00, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x08, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x04, (D_800DCD10[2] >> 16) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x0C, (D_800DCD10[2] >> 16) << 16);
    // matching: the doubled & 0xFFFF steers IDO's register choice for the low halves
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x10, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x18, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x14, (D_800DCD10[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x1C, (D_800DCD10[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x20, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x24, D_800DCD10[3] << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x28, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x2C, D_800DCD10[3] << 16);
    return gfx;
}

void func_1515E43C(s32 x, s32 y, s32 z, s32 arg3, u8 *colorA, u8 *colorB, u8 *amountA, u8 *amountB);
void func_1515E278(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags);
void func_1515EC78(PointLight *light, s32 player, PointLightGfx *out, s32 x, s32 y, s32 z, s32 flags);

/* Loads the 0x30-byte light at `l` into the microcode light table at byte offset `ofs` (point lights start at 0x60). */
#define gSPPointLight(pkt, l, ofs) gDma2p((pkt), G_MOVEMEM, (l), sizeof(PointLightGfx), G_MV_LIGHT, (ofs))

/*
 * Picks the (maxLights - 1) point lights nearest to (x, y, z) for player `player`, fills this
 * frame's slice of lightBuf with them plus a directional light, and emits the commands that
 * load them, followed by `ambient`.
 */
Gfx *func_1515D914(Gfx *gfx, s32 player, s32 x, s32 y, s32 z, u32 arg5, PointLightGfx *lightBuf, s32 maxLights,
                   Ambient *ambient, s8 *outCount, s32 mask, u8 *smoothColor, s32 flags, PointLight **outLights) {
    s32 i;
    s32 j;
    s32 count;
    s32 padLight;
    u8 *unused; // matching: stack slot
    s16 pos[3];
    u8 color[3];
    u8 dirColor[3];
    PointLight *light;
    PointLight *nearest[11];
    s32 nearestDist[11];
    s32 dist;
    u8 colorA[3];
    u8 colorB[3];
    u8 amountA;
    u8 amountB;
    f32 scale;
    f32 dir[3];
    s32 lightPos[3];
    f32 *posPtr;

    func_1515E278(x, y, z, mask, color, arg5, flags);
    j = 1 << player; // matching: dead store IDO still emits
    if (flags & 2) {
        func_1515E43C(x, y, z, mask, colorA, colorB, &amountA, &amountB);
        for (i = 0; i < 3; i++) {
            s32 level;
            level = (u8)((((0xFF - color[i]) * amountA) >> 8) + color[i]);
            dirColor[i] = (colorA[i] * level) >> 8;
            level = (u8)((((0xFF - color[i]) * amountB) >> 8) + color[i]);
            color[i] = (colorB[i] * level) >> 8;
        }
    }
    for (i = 0; i < 3; i++) {
        if (smoothColor != NULL) {
            if (!(flags & 8)) {
                color[i] = (color[i] + smoothColor[i] * 3) >> 2;
            }
            smoothColor[i] = color[i];
        }
        ambient->l.col[i] = ambient->l.colc[i] = color[i];
        continue; // matching: see G
    }
    for (i = 0; i < maxLights; i++) {
        nearestDist[i] = 0x7FFFFFFF;
    }
    if (!(flags & 1)) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
    } else {
        // sample 1000 units along the camera's view direction instead
        dir[0] = D_800DBFF0[player].target.x - D_800DBFF0[player].eye.x;
        dir[1] = D_800DBFF0[player].target.y - D_800DBFF0[player].eye.y;
        dir[2] = D_800DBFF0[player].target.z - D_800DBFF0[player].eye.z;
        scale = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (scale != 0.0f) {
            scale = 1000.0f / scale;
            pos[0] = dir[0] * scale + (f32)x;
            pos[1] = dir[1] * scale + (f32)y;
            pos[2] = dir[2] * scale + (f32)z;
        } else {
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
        }
    }
    count = 0;
    for (light = D_800DCD78; light != NULL; light = light->next) {
        if (((light->unkC & (1 << player)) || (flags & 0x20) || !(light->unk8 & 1)) && (light->unk8 & mask) &&
            (light->disabled == 0) && !(light->unkA & (1 << player))) {
            if (light->unk4 == 0) {
            G: // matching: this unused label (like the if (1) and continues below) steers IDO's regalloc
                lightPos[0] = light->x;
                if (lightPos[0] != -0x8000) {
                    lightPos[1] = light->y;
                    lightPos[2] = light->z;
                } else {
                    posPtr = (f32 *)((light->z & 0xFFFF) | (light->y << 16));
                    lightPos[0] = posPtr[0];
                    lightPos[1] = posPtr[1];
                    lightPos[2] = posPtr[2];
                }
                dist = (pos[0] - lightPos[0]) * (pos[0] - lightPos[0]) + (pos[1] - lightPos[1]) * (pos[1] - lightPos[1]) +
                       (pos[2] - lightPos[2]) * (pos[2] - lightPos[2]);
                if (!(flags & 4) && ((light->radius << 17) < dist)) {
                    dist = 0x7FFFFFFF;
                }
            }
            // insert into the distance-sorted nearest list
            for (i = 0; i < maxLights - 1; i++) {
                if (dist < nearestDist[i]) {
                    for (j = maxLights - 2; j > i; j--) {
                        nearestDist[j] = nearestDist[j - 1];
                        nearest[j] = nearest[j - 1];
                    }
                    nearestDist[i] = dist;
                    nearest[i] = light;
                    if (count != maxLights - 1) {
                        count++;
                    }
                    break;
                }
            }
            if (1) {} // matching: see G
        }
    }
    if (maxLights != 0) {
        // an odd light count is padded with the dummy light D_800DCD40
        if (count & 1) {
            padLight = 1;
            if (outLights != NULL) {
                outLights[count] = NULL;
            }
        } else {
            padLight = 0;
        }
        if (outCount != NULL) {
            if (padLight) {
                *outCount = (count + padLight + 1) | 0x80;
            } else {
                *outCount = count + padLight + 1;
            }
        }
        gMoveWd(gfx++, G_MW_NUMLIGHT, 0, (count + padLight) * 0x30 + 0x30);
    } else {
        if (outCount != NULL) {
            *outCount = count;
        }
        gMoveWd(gfx++, G_MW_NUMLIGHT, 0, count * 0x30);
    }
    if (maxLights != 0) {
        PointLightGfx *out;
        // the slot after the point lights holds the directional light
        out = &lightBuf[D_800BE9C0 * maxLights + count];
        out->dir[0] = D_800DCD30[player][0];
        out->dir[1] = D_800DCD30[player][1];
        out->dir[2] = D_800DCD30[player][2];
        if (flags & 2) {
            out->col[0] = out->colc[0] = dirColor[0];
            out->col[1] = out->colc[1] = dirColor[1];
            out->col[2] = out->colc[2] = dirColor[2];
        } else {
            out->col[0] = out->colc[0] = 0;
            out->col[1] = out->colc[1] = 0;
            out->col[2] = out->colc[2] = 0;
        }
        out -= count;
        for (i = 0; i < count; i++, out++) {
            light = nearest[i];
            if (outLights != NULL) {
                outLights[i] = light;
            }
            if (flags & 0x20) {
                light->unkC |= 1 << player;
            }
            if (light->unk4 == 0) {
                func_1515EC78(light, player, out, x, y, z, flags);
            }
            gSPPointLight(gfx++, out, (i + 2) * 0x30U);
            continue; // matching: see G
        }
        if (padLight) {
            gSPPointLight(gfx++, &D_800DCD40, (i + 2) * 0x30U);
        }
        gSPPointLight(gfx++, &lightBuf[D_800BE9C0 * maxLights + i], (u32)((i + padLight + 1) * 0x30U + 0x30));
        gSPPointLight(gfx++, ambient, (u32)((i + padLight + 2) * 0x30U + 0x30));
    }
    return gfx;
}


/* A zone record: struct178 with the fields this file reads named (structs.h has them as a blob). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ u8 pad8[0xC];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;     // >> 2 is the zone type
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u32 unk18;    // light zones: colour in the low 24 bits, mask in the top byte;
                             // trigger zones: index into D_8008B090
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8 pad20[0x14];
} Zone;

typedef struct ZoneNode {
    struct ZoneNode *next;
    Zone *zone;
} ZoneNode;

Zone *func_1515EB84(s32 x, s32 y, s32 z, s32 mask, ZoneNode *node);

/*
 * Ambient colour at (x, y, z): the colour of the first zone in D_800DCD80 containing the
 * point, else the global ambient colour dimmed by bits 5-6 of arg5. With flags & 0x10 the
 * D_800DCD84 tint is applied on top.
 */
void func_1515E278(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags) {
    Zone *zone;
    s32 i;
    s32 sum;
    u8 tint[3];
    u8 colorA[3];
    u8 colorB[3];
    u8 amountA;
    u8 amountB;
    s32 level;
    u8 *out;
    s32 scale;

    zone = func_1515EB84(x, y, z, mask, D_800DCD80);
    if (zone != NULL) {
        color[0] = zone->unk18 & 0xFF;
        color[1] = (zone->unk18 >> 8) & 0xFF;
        color[2] = (zone->unk18 >> 16) & 0xFF;
    } else {
        scale = 0x100 - ((arg5 >> 5) & 3) * 0x40;
        out = color;
        for (i = 0; i < 3; i++) {
            *out++ = (D_800DCD20[i] * scale) >> 8;
        }
    }
    if (flags & 0x10) {
        func_1515E43C(x, y, z, mask, colorA, colorB, &amountA, &amountB);
        out = color;
        for (i = 0; i < 3; i++) {
            level = (u8)((((0xFF - *out) * amountA) >> 8) + *out);
            tint[i] = (colorA[i] * level) >> 8;
            level = (u8)((((0xFF - *out) * amountB) >> 8) + *out);
            *out = (colorB[i] * level) >> 8;
            sum = *out + ((tint[i] * 7) >> 3);
            if (sum > 0xFF) {
                sum = 255;
            }
            *out = sum;
            out++;
        }
    }
}


/* Tint colours and amounts at (x, y, z), from the first D_800DCD84 zone containing it or the defaults. */
void func_1515E43C(s32 x, s32 y, s32 z, s32 arg3, u8 *colorA, u8 *colorB, u8 *amountA, u8 *amountB) {
    Zone *zone;

    zone = func_1515EB84(x, y, z, 0, D_800DCD84);
    if (zone != NULL) {
        colorA[0] = zone->unk18 & 0xFF;
        colorA[1] = (zone->unk18 >> 8) & 0xFF;
        colorA[2] = (zone->unk18 >> 16) & 0xFF;
        colorB[0] = zone->unk1C & 0xFF;
        colorB[1] = (zone->unk1C >> 8) & 0xFF;
        colorB[2] = (zone->unk1C >> 16) & 0xFF;
        *amountA = (zone->unk1C >> 24) & 0xFF;
        *amountB = (zone->unk18 >> 24) & 0xFF;
    } else {
        colorA[0] = D_800DCD24[0];
        colorA[1] = D_800DCD24[1];
        colorA[2] = D_800DCD24[2];
        colorB[0] = D_800DCD28[0];
        colorB[1] = D_800DCD28[1];
        colorB[2] = D_800DCD28[2];
        *amountA = D_800DCD3C;
        *amountB = D_800DCD3D;
    }
}

/*
 * Re-emits the light commands func_1515D914 built for this frame, from its light slice
 * `lightBuf` and the count byte it stored (bit 7 = padded with the dummy light).
 */
Gfx *func_1515E544(Gfx *gfx, PointLightGfx *lightBuf, u8 maxLights, u8 countByte, Ambient *ambient) {
    s32 count;
    s32 padLight;
    s32 i;

    count = countByte & 0x7F;
    if (count != countByte) {
        padLight = 1;
    } else {
        padLight = 0;
    }
    gMoveWd(gfx++, G_MW_NUMLIGHT, 0, count * 0x30);
    for (i = 0; i < count - padLight - 1; i++) {
        gSPPointLight(gfx++, &lightBuf[D_800BE9C0 * maxLights + i], (i + 2) * 0x30U);
    }
    if (padLight) {
        gSPPointLight(gfx++, &D_800DCD40, (i + 2) * 0x30U);
    }
    gSPPointLight(gfx++, &lightBuf[D_800BE9C0 * maxLights + i], (u32)((i + padLight + 1) * 0x30U + 0x30));
    gSPPointLight(gfx++, ambient, (u32)((i + padLight + 2) * 0x30U + 0x30));
    return gfx;
}

extern u8 D_800D9E21;                // light count byte func_1515D914 stored for player 0
extern PointLight *D_800D9E28[];     // the lights it picked

/*
 * Lit colour at (x, y, z): the zone ambient colour (func_1515E278) plus the contribution of
 * each point light picked for this frame, falling off with squared distance.
 */
void func_1515E888(s32 player, s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg6, s32 flags) {
    f32 distSq;
    f32 range;
    f32 pos[3];
    f32 sum[3];
    s32 i;
    f32 *lightPos;
    s32 count;
    PointLight *light;
    PointLight **lights;
    f32 f;
    s32 v;

    func_1515E278(x, y, z, mask, color, arg6, flags);
    sum[0] = color[0];
    sum[1] = color[1];
    sum[2] = color[2];
    v = D_800D9E21 & 0x7F;
    count = v - 1;
    i = 0;
    if (count > 0) {
        lights = D_800D9E28;
        lightPos = pos; // matching: distances read pos through this alias
        for (;;) { // matching: a do-while allocates color differently
            light = *lights;
            if (light != NULL) {
                pos[0] = light->x;
                pos[1] = light->y;
                pos[2] = light->z;
                range = light->radius * 2048.0f;
                distSq = (lightPos[0] - x) * (lightPos[0] - x) + (lightPos[1] - y) * (lightPos[1] - y) +
                     (lightPos[2] - z) * (lightPos[2] - z);
                f = range / distSq;
                if (f > 1.0f) {
                    f = 1.0f;
                }
                sum[0] += f * light->color[0];
                sum[1] += f * light->color[1];
                sum[2] += f * light->color[2];
            }
            i++;
            lights++;
            if (i == count) {
                break;
            }
        }
    }
    v = sum[0];
    if (v > 255) {
        v = 255;
    }
    color[0] = v;
    v = sum[1];
    if (v > 255) {
        v = 255;
    }
    color[1] = v;
    v = sum[2];
    if (v > 255) {
        v = 255;
    }
    color[2] = v;
}

s32 func_150A1DA0(Actor *obj, Zone *zone, s32);

/* First zone in the list whose type matches `mask` (0 = any) and that contains (x, y, z). */
Zone *func_1515EB84(s32 x, s32 y, s32 z, s32 mask, ZoneNode *node) {
    Actor probe;
    Zone *zone;

    probe.x_position = x;
    probe.y_position = y;
    probe.unk180 = y;
    probe.old_x_position = x;
    probe.old_y_position = y;
    probe.z_position = z;
    probe.old_z_position = z;

    for (; node != NULL; node = node->next) {
        zone = node->zone;
        if (zone->unk14 == 0 && (mask == 0 || ((zone->unk18 >> 24) & mask)) &&
            ((zone->unk15 >> 2) == 0x18 || !(zone->unk18 >> 31)) && func_150A1DA0(&probe, zone, 0) == 0) {
            return node->zone;
        }
    }
    return NULL;
}

extern MtxF D_800D9C10[];
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

/*
 * Fills the microcode light `out` from `light` as seen by player `player`: colour, direction
 * towards the light from (x, y, z) when flags & 2, and the view-space position (cached per player).
 */
void func_1515EC78(PointLight *light, s32 player, PointLightGfx *out, s32 x, s32 y, s32 z, s32 flags) {
    s32 i;
    f32 viewPos[3];
    f32 dx;
    f32 dy;
    f32 dz;
    f32 scale;
    s32 dir[3];
    s32 pos[3];
    f32 *posPtr;

    pos[0] = light->x;
    if (pos[0] != -0x8000) {
        pos[1] = light->y;
        pos[2] = light->z;
    } else {
        posPtr = (f32 *)((light->z & 0xFFFF) | (light->y << 16));
        pos[0] = posPtr[0];
        pos[1] = posPtr[1];
        pos[2] = posPtr[2];
    }
    if (flags & 2) {
        dx = pos[0] - x;
        dy = pos[1] - y;
        dz = pos[2] - z;
        scale = sqrtf(dx * dx + dy * dy + dz * dz);
        if (scale) {
            scale = 127.0f / scale;
            dx *= scale;
            dy *= scale;
            dz *= scale;
        } else {
            dz = dy = 0.0f;
            dx = 127.0f;
        }
        dir[0] = dx;
        dir[1] = dy;
        dir[2] = dz;
    } else {
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = 127;
    }
    if (light->viewPosValid & (1 << player)) {
        pos[0] = light->viewPos[player][0];
        pos[1] = light->viewPos[player][1];
        pos[2] = light->viewPos[player][2];
    } else {
        func_150A7960(&D_800D9C10[player][0][0], pos[0], pos[1], pos[2], &viewPos[0], &viewPos[1], &viewPos[2]);
        pos[0] = viewPos[0];
        pos[1] = viewPos[1];
        pos[2] = viewPos[2];
        light->viewPos[player][0] = pos[0];
        light->viewPos[player][1] = pos[1];
        light->viewPos[player][2] = pos[2];
        light->viewPosValid |= (1 << player);
    }
    for (i = 0; i < 3; i++) {
        out->col[i] = light->color[i];
        out->colc[i] = light->color[i];
        out->dir[i] = dir[i];
        out->pos[i] = pos[i];
        out->pos2[i] = pos[i];
    }
    out->unkC = light->radius;
}

/*
 * Stores the light-space scale for a fixed-point Mtx (viewed as halfwords: 16 integer parts,
 * then 16 fraction parts): 1 / the diagonal as 16.16 in D_800DCD10[0..2], and -z translation
 * in D_800DCD10[3].
 */
void func_1515EF74(s16 *mtx) {
    func_1515F040(1.0f / func_1515F008(mtx, 0), 0);
    func_1515F040(1.0f / func_1515F008(mtx, 5), 1);
    func_1515F040(1.0f / func_1515F008(mtx, 10), 2);
    func_1515F0AC(-func_1515F008(mtx, 14), 3);
}

/* Element `i` (0-15) of a fixed-point Mtx as a float. */
f32 func_1515F008(s16 *mtx, s32 i) {
    s32 value = mtx[i];
    value = (value << 16) | (u16)mtx[i + 16];
    return (f32)value * (1.0f / 65536.0f);
}

/* Clamps `value` to the s16 range and stores it in D_800DCD10[i] as 16.16 fixed point. */
void func_1515F040(f32 value, s32 i) {
    if (value >= 32767.0f) {
        value = 32767.0f;
    } else if (value < -32768.0f) {
        value = -32768.0f;
    }
    value *= 65536.0f;
    D_800DCD10[i] = (s32)value;
}

/* Clamps `value` to the s16 range and stores it in D_800DCD10[i] as an integer. */
void func_1515F0AC(f32 value, s32 i) {
    if (value >= 32767.0f) {
        value = 32767.0f;
    } else if (value < -32768.0f) {
        value = -32768.0f;
    }
    D_800DCD10[i] = (s32)value;
}

/* Unlinks `light` from the light list and frees it. */
void func_1515F10C(PointLight *light) {
    PointLight *node;
    PointLight *prev;

    prev = NULL;
    for (node = D_800DCD78; node != light; node = node->next) {
        prev = node;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        D_800DCD78 = node->next;
    }
    func_10004074(node);
}

/* Sets the disabled flag of every light in `group`. */
void func_1515F170(s32 group, u8 disabled) {
    PointLight *light;

    for (light = D_800DCD78; light != NULL; light = light->next) {
        if (group == light->group) {
            light->disabled = disabled;
        }
    }
}

/* A node of a list of spawn regions with their cached volume and area. */
typedef struct RegionNode {
    /* 0x00 */ struct134 *region;
    /* 0x04 */ f32 volume;
    /* 0x08 */ f32 area;
    /* 0x0C */ struct RegionNode *next;
} RegionNode;

RegionNode *func_1515F1B0(struct134 *region) {
    RegionNode *node;

    node = (RegionNode *)func_10003C6C(sizeof(RegionNode), 1, 2, 0, 1);
    if (node == NULL) {
        return NULL;
    }
    node->region = region;
    if (region != NULL) {
        node->volume = func_1514462C((s32)region);
    } else {
        node->volume = 0.0f;
    }
    if (region != NULL) {
        node->area = func_15144598(region);
    } else {
        node->area = 0.0f;
    }
    node->next = NULL;
    return node;
}

/* Pushes `node` onto the front of `list`. */
void func_1515F25C(RegionNode **list, RegionNode *node) {
    node->next = *list;
    *list = node;
}

typedef void (*ZoneHandler)(Actor *obj, Zone *zone);

extern ZoneHandler D_8008B090[12];

/* Runs zone handler zone->unk18 for `obj`. */
void func_1515F270(Actor *obj, Zone *zone) {
    s32 idx = zone->unk18;

    if (idx >= 0 && idx < 12) {
        if (D_8008B090[idx] != NULL) {
            D_8008B090[idx](obj, zone);
        }
    }
}

s32 func_1505D024(Actor *obj, s32 arg1, u16 arg2, s32 arg3);

void func_1515F2B8(Actor *obj, Zone *zone) {
    func_1505D024(obj, 0x6001D, obj->unk7A, -1);
}

extern ZoneHandler D_8008B0C0[3];

/* Zone handler: when the player (unique id 1) is in the zone, runs ambient effect zone->unk1C. */
void func_1515F2E8(Actor *obj, Zone *zone) {
    s32 idx;

    if (obj->unique_id == 1) {
        idx = zone->unk1C;
        if (idx >= 0 && idx < 3) {
            D_8008B0C0[idx](obj, zone);
        }
    }
}

/* Ambient effect 0: eases the ambient colour halfway towards (97, 96, 98) each frame. */
void func_1515F338(Actor *obj, Zone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = D_800DCD20[0];
    green = D_800DCD20[1];
    blue = D_800DCD20[2];
    if (D_800DCD94 != 97.0f) {
        D_800DCD94 += (97.0f - D_800DCD94) * 0.5f;
        red = (u32)D_800DCD94;
    }
    if (D_800DCD98 != 96.0f) {
        D_800DCD98 += (96.0f - D_800DCD98) * 0.5f;
        green = (u32)D_800DCD98;
    }
    if (D_800DCD9C != 98.0f) {
        D_800DCD9C += (98.0f - D_800DCD9C) * 0.5f;
        blue = (u32)D_800DCD9C;
    }
    func_1515D4D4(red, green, blue, 0);
}

/* Ambient effect 1: eases the ambient colour halfway towards (229, 253, 160) each frame. */
void func_1515F5C4(Actor *obj, Zone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = D_800DCD20[0];
    green = D_800DCD20[1];
    blue = D_800DCD20[2];
    if (D_800DCD94 != 229.0f) {
        D_800DCD94 += (229.0f - D_800DCD94) * 0.5f;
        red = (u32)D_800DCD94;
    }
    if (D_800DCD98 != 253.0f) {
        D_800DCD98 += (253.0f - D_800DCD98) * 0.5f;
        green = (u32)D_800DCD98;
    }
    if (D_800DCD9C != 160.0f) {
        D_800DCD9C += (160.0f - D_800DCD9C) * 0.5f;
        blue = (u32)D_800DCD9C;
    }
    func_1515D4D4(red, green, blue, 0);
}

/* Ambient effect 2: eases the ambient colour towards a colour pulsing with sin(D_800DCDA0). */
void func_1515F850(Actor *obj, Zone *zone) {
    f32 s;
    f32 targetR;
    f32 targetG;
    f32 targetB;
    u8 color[3];

    color[0] = D_800DCD20[0];
    color[1] = D_800DCD20[1];
    color[2] = D_800DCD20[2];
    s = sinf(D_800DCDA0);
    targetR = (s * 29.5f) + 127.5f;
    targetG = (s * 71.0f) + 109.0f;
    targetB = (s * 26.0f) + 26.0f;
    D_800DCDA0 += 0.07100000232f * D_800BE9A4;
    D_800DCDA0 = func_15144B68(D_800DCDA0);
    if (targetR != D_800DCD94) {
        D_800DCD94 += (targetR - D_800DCD94) * 0.5f;
        color[0] = (u32)D_800DCD94;
    }
    if (targetG != D_800DCD98) {
        D_800DCD98 += (targetG - D_800DCD98) * 0.5f;
        color[1] = (u32)D_800DCD98;
    }
    if (targetB != D_800DCD9C) {
        D_800DCD9C += (targetB - D_800DCD9C) * 0.5f;
        color[2] = (u32)D_800DCD9C;
    }
    func_1515D4D4(color[0], color[1], color[2], 0);
}

/* Zone handler: func_1515F2E8 for a handler table that is empty in this build. */
void func_1515FB70(Actor *obj, Zone *zone) {
    s32 idx;

    if (obj->unique_id == 1) {
        idx = zone->unk1C;
        if (idx >= 0 && idx < 0) {
        }
    }
}

void func_1515FB94(Actor *obj, Zone *zone) {
    func_1505D024(obj, 0x6002D, obj->unk7A, -1);
}

/* Zone handler: like func_1515FB94, also passing the index of the object linked to this zone (or -1). */
void func_1515FBC4(Actor *obj, Zone *zone) {
    struct ZoneLinkObj *link;
    Actor *target;
    s32 targetIndex;

    target = NULL;
    link = func_15105C24((s32)zone);
    if (link != NULL) {
        target = link->unk98;
    }
    if (target != NULL) {
        targetIndex = target - D_800CC2D0;
    } else {
        targetIndex = -1;
    }
    func_1505D024(obj, 0x6002E, obj->unk7A, targetIndex);
}

void func_1515FC34(Actor *obj, Zone *zone) {
    func_1505D024(obj, 0x33, 0xC000, -1);
}

s32 func_150AD960(s32, s32, s32, s32);
void func_15136C3C(Actor *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(Actor *, s32);

/*
 * Zone handler: unless the zone has a range (unk1C, in hundredths of its radius) that `obj`
 * is outside of, fires event 0x2F on `obj`.
 */
void func_1515FC60(Actor *obj, Zone *zone) {
    s32 pad; // matching: stack slot
    f32 limit;
    f32 scale;
    s32 value;

    if (obj->unk31C != NULL && obj->unk31C->unk120 == 0) {
        value = zone->unk1C;
        if (value != 0) {
            scale = (f32)(u32)value * 0.009999999776f;
            value = func_150AD960(zone->x, zone->z, obj->x_position, obj->z_position);
            limit = (zone->radius * scale) - obj->unkE4;
            if (limit > value) {
                return;
            }
        }
        func_1505D024(obj, 0x2F, 0, -1);
        func_15136C3C(obj, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(obj);
        func_1507CD64(obj, 6);
    }
}
