#include <ultra64.h>
#define func_1515D4D4 func_1515D4D4_prototype
#define func_1515D6D0 func_1515D6D0_prototype
#include "functions.h"
#undef func_1515D4D4
#undef func_1515D6D0
#define D_800DCD20 D_800DCD20_hdr
#define D_800DCD24 D_800DCD24_hdr
#define D_800DCD30 D_800DCD30_hdr
#define D_800DCD40 D_800DCD40_hdr
#define D_800DCD78 D_800DCD78_hdr
#define D_800DCD80 D_800DCD80_hdr
#define D_800DCD84 D_800DCD84_hdr
#include "variables.h"
#undef D_800DCD20
#undef D_800DCD24
#undef D_800DCD30
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

extern u8 D_800DCD20[3];            // ambient colour
extern u8 D_800DCD24[3];
extern u8 D_800DCD27;               // priority of the current ambient colour
extern s8 D_800DCD30[4][3];         // per-player directional light direction
extern PointLightGfx D_800DCD40;    // padding light for odd light counts
extern PointLight *D_800DCD78;      // list of all point lights
extern struct LightZoneNode *D_800DCD80; // ambient colour zones
extern struct LightZoneNode *D_800DCD84; // tint zones

void *allocate_memory(s32 size, s32, s32, s32);
f32 func_1515F008(s16 *, s32);
void func_1515F040(f32, s32);
void func_1515F0AC(f32, s32);
extern void func_10004074(void *);
extern f32 D_800A6530;

struct Struct1515FBC4 {
    u8 pad0[0x98];
    struct127 *unk98;
};

struct Struct1515FBC4 *func_15105C24(s32);

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
void func_1515EF74(s16 *arg0);

/* G_MOVEWORD index this microcode uses for the light-space scale matrix. */
#define G_MW_LIGHTSCALE 0x10

/* Enables lighting and uploads the light-space scale matrix of player `player`. */
Gfx *func_1515D6D0(Gfx *gfx, s32 player) {
    gMoveWd(gfx++, G_MW_NUMLIGHT, 0, 0);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    // each struct259 holds one Mtx per frame buffer at +0x100
    func_1515EF74((s16 *)((Mtx *)((u8 *)((struct259 *)D_800BE628 + player) + 0x100) + D_800BE9C0));
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

/* Loads microcode light slot `l` at byte offset `ofs` (point lights start at 0x60). */
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
        dir[0] = D_800DBFF0[player].unk2BC - D_800DBFF0[player].unk2F8;
        dir[1] = D_800DBFF0[player].unk2C0 - D_800DBFF0[player].unk2FC;
        dir[2] = D_800DBFF0[player].unk2C4 - D_800DBFF0[player].unk300;
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


/* struct178 with the fields this file reads named (structs.h has them as a blob). */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;     // >> 2 is the zone type
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u32 unk18;    // colour in the low 24 bits; top byte doubles as a mask
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u8 pad20[0x14];
} LightZone;

typedef struct LightZoneNode {
    struct LightZoneNode *next;
    LightZone *zone;
} LightZoneNode;

LightZone *func_1515EB84(s32 x, s32 y, s32 z, s32 mask, LightZoneNode *node);

/*
 * Ambient colour at (x, y, z): the colour of the first zone in D_800DCD80 containing the
 * point, else the global ambient colour dimmed by bits 5-6 of arg5. With flags & 0x10 the
 * D_800DCD84 tint is applied on top.
 */
void func_1515E278(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags) {
    LightZone *zone;
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
    LightZone *zone;

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

struct Light1515E888 {
    u8 pad0[5];
    u8 r;
    u8 g;
    u8 b;
    u8 pad8[6];
    s16 x;
    s16 y;
    s16 z;
    u8 pad14[0x1B];
    u8 radius;
};

extern u8 D_800D9E21;
extern struct Light1515E888 *D_800D9E28[];

void func_1515E888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, u8 *arg7)
{
  f32 sq;
  f32 dist;
  f32 pos[3];
  f32 color[3];
  s32 i;
  f32 *new_var;
  s32 n;
  struct Light1515E888 *light;
  struct Light1515E888 **p;
  f32 f;
  s32 v;
  func_1515E278(arg1, arg2, arg3, arg4, arg5, (u32)arg6, (s32)arg7);
  color[0] = arg5[0];
  color[1] = arg5[1];
  color[2] = arg5[2];
  v = D_800D9E21 & 0x7F;
  n = v - 1;
  i = 0;
  if (n > 0)
  {
    p = D_800D9E28;
    new_var = pos;
    loop:
    light = *p;

    if (light != 0)
    {
      pos[0] = light->x;
      pos[1] = light->y;
      pos[2] = light->z;
      dist = light->radius * 2048.0f;
      sq = (((new_var[0] - arg1) * (new_var[0] - arg1)) + ((new_var[1] - arg2) * (new_var[1] - arg2))) + ((new_var[2] - arg3) * (new_var[2] - arg3));
      f = dist / sq;
      if (f > 1.0f)
      {
        f = 1.0f;
      }
      color[0] += f * light->r;
      color[1] += f * light->g;
      color[2] += f * light->b;
    }
    i++;
    p++;
    if (i != n)
    {
      goto loop;
    }
  }
  v = color[0];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[0] = v;
  v = color[1];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[1] = v;
  v = color[2];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[2] = v;
}

s32 func_150A1DA0(struct127 *obj, LightZone *zone, s32);

/* First zone in the list whose type matches `mask` (0 = any) and that contains (x, y, z). */
LightZone *func_1515EB84(s32 x, s32 y, s32 z, s32 mask, LightZoneNode *node) {
    struct127 probe;
    LightZone *zone;

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

extern f32 D_800D9C10[][4][4];
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
        dir[2] = 0x7F;
    }
        if (light->viewPosValid & (1 << player)) {
        pos[0] = light->viewPos[player][0];
        pos[1] = light->viewPos[player][1];
        pos[2] = light->viewPos[player][2];
    } else {
        func_150A7960(D_800D9C10[player][0], pos[0], pos[1], pos[2], &viewPos[0], &viewPos[1], &viewPos[2]);
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

void func_1515EF74(s16 *arg0) {
    func_1515F040(1.0f / func_1515F008(arg0, 0), 0);
    func_1515F040(1.0f / func_1515F008(arg0, 5), 1);
    func_1515F040(1.0f / func_1515F008(arg0, 0xA), 2);
    func_1515F0AC(-func_1515F008(arg0, 0xE), 3);
}

f32 func_1515F008(s16 *arg0, s32 arg1) {
    s32 hi = arg0[arg1];
    hi = (hi << 16) | (u16)arg0[arg1 + 0x10];
    return (f32)hi * 1.52587890625e-05f;
}

extern s32 D_800DCD10[];

void func_1515F040(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (var_fv0 <= arg0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    arg0 *= 65536.0f;
    D_800DCD10[arg1] = (s32) arg0;
}

extern s32 D_800DCD10[];

void func_1515F0AC(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (arg0 >= var_fv0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    D_800DCD10[arg1] = (s32) arg0;
}

struct Node1515F10C {
    struct Node1515F10C *next;
};

void func_1515F10C(struct Node1515F10C *arg0) {
    struct Node1515F10C *node;
    struct Node1515F10C *prev;

    node = (struct Node1515F10C *)D_800DCD78;
    prev = 0;
    if (node != arg0) {
        do {
            prev = node;
            node = node->next;
        } while (node != arg0);
    }
    if (prev != 0) {
        prev->next = node->next;
    } else {
        D_800DCD78 = (s32)node->next;
    }
    func_10004074(node);
}

struct Node1515F170 {
    struct Node1515F170 *next;
    char pad4[5];
    s8 unk9;
    char padA[1];
    u8 unkB;
};

void func_1515F170(s32 arg0, u8 arg1) {
    struct Node1515F170 *p;

    for (p = (struct Node1515F170 *)D_800DCD78; p != 0; p = p->next) {
        if (arg0 == p->unkB) {
            p->unk9 = arg1;
        }
    }
}

struct Node1515F1B0 {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    s32 unkC;
};

struct Node1515F1B0 *func_1515F1B0(s32 arg0) {
    struct Node1515F1B0 *temp;

    temp = (struct Node1515F1B0 *)func_10003C6C(0x10, 1, 2, 0, 1);
    if (temp == 0) {
        return 0;
    }
    temp->unk0 = arg0;
    if (arg0 != 0) {
        temp->unk4 = func_1514462C(arg0);
    } else {
        temp->unk4 = 0.0f;
    }
    if (arg0 != 0) {
        temp->unk8 = func_15144598((struct134 *)arg0);
    } else {
        temp->unk8 = 0.0f;
    }
    temp->unkC = 0;
    return temp;
}

void func_1515F25C(void **arg0, void *arg1) {
    *(void **)((u8 *)arg1 + 0xC) = *arg0;
    *arg0 = arg1;
}

extern void (*D_8008B090[])(s32);

void func_1515F270(s32 arg0, s32 *arg1) {
    s32 idx = arg1[6];

    if (idx >= 0 && idx < 12) {
        if (D_8008B090[idx] != 0) {
            D_8008B090[idx](arg0);
        }
    }
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515F2B8(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6001D, arg0->unk7A, -1);
}

extern void (*D_8008B0C0[])(void);

void func_1515F2E8(u8 *arg0, s32 *arg1) {
    s32 idx;

    if (arg0[0x3B] == 1) {
        idx = arg1[7];
        if (idx >= 0 && idx < 3) {
            D_8008B0C0[idx]();
        }
    }
}

void func_1515F338(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 97.0f) {
        D_800DCD94 += (97.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 96.0f) {
        D_800DCD98 += (96.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 98.0f) {
        D_800DCD9C += (98.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

void func_1515F5C4(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 229.0f) {
        D_800DCD94 += (229.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 253.0f) {
        D_800DCD98 += (253.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 160.0f) {
        D_800DCD9C += (160.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

struct Stack1515F850 {
    s32 pad14;
    s32 pad18;
    u8 sp1C;
    u8 sp1D;
    u8 sp1E;
    u8 pad1F;
    f32 sp20;
    f32 sp24;
    f32 sp28;
};

void func_1515F850(s32 arg0, s32 arg1) {
    s32 pad;
    struct Stack1515F850 stack;
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;

    stack.sp1C = ((u8 *)&D_800DCD20)[0];
    stack.sp1D = ((u8 *)&D_800DCD20)[1];
    stack.sp1E = ((u8 *)&D_800DCD20)[2];
    temp_f0 = sinf(D_800DCDA0);
    temp_f14 = (temp_f0 * 29.5f) + 127.5f;
    temp_f16 = (temp_f0 * 71.0f) + 109.0f;
    stack.sp28 = temp_f14;
    stack.sp24 = temp_f16;
    temp_f18 = (temp_f0 * 26.0f) + 26.0f;
    stack.sp20 = temp_f18;
    D_800DCDA0 += D_800A6530 * D_800BE9A4;
    D_800DCDA0 = func_15144B68(D_800DCDA0);
    if (stack.sp28 != D_800DCD94) {
        D_800DCD94 += (stack.sp28 - D_800DCD94) * 0.5f;
        stack.sp1C = (u8)(u32)D_800DCD94;
    }
    if (stack.sp24 != D_800DCD98) {
        D_800DCD98 += (stack.sp24 - D_800DCD98) * 0.5f;
        stack.sp1D = (u8)(u32)D_800DCD98;
    }
    if (stack.sp20 != D_800DCD9C) {
        D_800DCD9C += (stack.sp20 - D_800DCD9C) * 0.5f;
        stack.sp1E = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(stack.sp1C, stack.sp1D, stack.sp1E, 0);
}

long func_1515FB70(u8 *a0, u8 *a1)
{
  long offset;
  s32 v0;
  s32 *slot;
  v0 = (*((u8 *) (a0 + 0x3B))) == 1;
  if ((*((u8 *) (a0 + 0x3B))) == 1)
  {
    v0 = *((s32 *) (a1 + 0x1C));
    if (v0 >= 0)
    {
      offset = 0x1C;
      if (!v0)
      {
        slot = (s32 *) (a1 + offset);
      }
    }
  }
}


s32 func_1515FB94(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6002D, arg0->unk7A, -1);
}

void func_1515FBC4(struct127 *arg0, s32 arg1) {
    struct Struct1515FBC4 *temp_v0;
    struct127 *temp_v1;
    s32 temp_a3;

    temp_v1 = 0;
    temp_v0 = func_15105C24(arg1);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0->unk98;
    }
    if (temp_v1 != 0) {
        temp_a3 = ((s32)temp_v1 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
    } else {
        temp_a3 = -1;
    }
    func_1505D024(arg0, 0x6002E, arg0->unk7A, temp_a3);
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515FC34(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x33, 0xC000, -1);
}

struct Struct1515FC60 {
    s16 unk0;
    u8 pad2[2];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x14];
    u32 unk1C;
};

union Union1515FC60 {
    f32 f;
    f32 pad[2];
};

extern f32 D_800A6534;
s32 func_150AD960(s32, s32, s32, s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(struct127 *, s32);

void func_1515FC60(struct127 *arg0, struct Struct1515FC60 *arg1) {
    f32 temp_f0;
    union Union1515FC60 temp_f18;
    s32 temp_v0;

    if (arg0->unk31C != NULL) {
        if (arg0->unk31C->unk120 == 0) {
            temp_v0 = arg1->unk1C;
            if (temp_v0 != 0) {
                temp_f18.f = (f32)(u32)temp_v0 * D_800A6534;
                temp_v0 = func_150AD960(arg1->unk0, arg1->unk4, (s32)arg0->x_position, (s32)arg0->z_position);
                temp_f0 = ((f32)arg1->unk6 * temp_f18.f) - (f32)arg0->unkE4;
                if (temp_f0 > (f32)temp_v0) {
                    return;
                }
            }
            func_1505D024(arg0, 0x2F, 0, -1);
            func_15136C3C(arg0, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(arg0);
            func_1507CD64(arg0, 6);
        }
    }
}
