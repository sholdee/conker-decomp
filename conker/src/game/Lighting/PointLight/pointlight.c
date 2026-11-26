#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
s32 gLightSpaceScale[4]; // light-space scale matrix of the current player (func_1515EF74)
u8 gAmbientColour[3];
u8 D_800DCD23; /* not referenced */
u8 D_800DCD24[3];
u8 gAmbientPriority;
u8 D_800DCD28[3];
s8 gPlayerLightDirs[4][3];
u8 D_800DCD3C;
u8 D_800DCD3D;
PointLightGfx D_800DCD40;
u8 D_800DCD70[8]; /* not referenced */
PointLight *gPointLights;
s32 D_800DCD7C;
ZoneLink *gAmbientZones;
ZoneLink *gDirectionalZones;

#include "functions.h"
#include "variables.h"

/* Allocates a zeroed 0x10-byte block. */
void *func_1515D440(void) {
    void *block;

    block = allocate_memory(0x10, 1, 2, 0);
    bzero(block, 0x10);
    return block;
}

/* Allocates a zeroed array of count 0x60-byte entries. */
void *light_allocGfxLights(s32 count) {
    void *block;

    block = allocate_memory(count * 0x60, 1, 2, 0);
    bzero(block, count * 0x60);
    return block;
}

/* Sets the ambient colour unless a higher-priority one is already set. */
void light_setAmbient(s32 r, s32 g, s32 b, u8 priority) {
    if (priority >= gAmbientPriority) {
        gAmbientColour[0] = r;
        gAmbientColour[1] = g;
        gAmbientColour[2] = b;
        D_800DCD7C = 1;
        gAmbientPriority = priority;
    }
}

/* Allocates a zeroed point light and appends it to the light list. */
PointLight *light_allocPointLight(void) {
    PointLight *light;
    PointLight *tail;
    PointLight *next;

    light = allocate_memory(sizeof(PointLight), 1, 2, 2);
    if (light != NULL) {
        bzero(light, sizeof(PointLight));
        if (gPointLights != NULL) {
            tail = gPointLights;
            next = tail->next;
            while (next != NULL) {
                tail = next;
                next = next->next;
            }
            tail->next = light;
        } else {
            gPointLights = light;
        }
        light->next = NULL;
    }
    return light;
}

/* Wrapper for light_createPointLight. */
PointLight *func_1515D5AC(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    return light_createPointLight(x, y, z, radius, r, g, b, arg7, disabled, group);
}

/* Creates a point light at (x, y, z). */
PointLight *light_createPointLight(s32 x, s32 y, s32 z, s32 radius, s32 r, s32 g, s32 b, s32 arg7, s32 disabled, u8 group) {
    PointLight *light = light_allocPointLight();

    if (light != NULL) {
        light->unk4 = 0;
        light->color[0] = r;
        light->color[1] = g;
        light->color[2] = b;
        light->lightMask = arg7;
        light->disabled = disabled;
        light->group = group;
        light->hiddenPlayers = 0;
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
void light_clearPlayerMasks(void) {
    PointLight *light;

    for (light = gPointLights; light != NULL; light = light->next) {
        light->pickedPlayers = 0;
        light->viewPosValid = 0;
    }
}

void func_1515D6C8(void) {
}

/* G_MOVEWORD index this microcode uses for the light-space scale matrix. */
#define G_MW_LIGHTSCALE 0x10

/* Enables lighting and uploads the light-space scale matrix of player `player`. */
Gfx *func_1515D6D0(Gfx *gfx, s32 player) {
    gMoveWd(gfx++, G_MW_NUMLIGHT, 0, 0);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    func_1515EF74((s16 *)&gViews[player].projectionMtx[D_800BE9C0]);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x00, ((gLightSpaceScale[0] >> 16) << 16) | ((gLightSpaceScale[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x08, ((gLightSpaceScale[0] >> 16) << 16) | ((gLightSpaceScale[1] >> 16) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x04, (gLightSpaceScale[2] >> 16) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x0C, (gLightSpaceScale[2] >> 16) << 16);
    // matching: the doubled & 0xFFFF steers IDO's register choice for the low halves
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x10, ((gLightSpaceScale[0] & 0xFFFF) << 16) | ((gLightSpaceScale[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x18, ((gLightSpaceScale[0] & 0xFFFF) << 16) | ((gLightSpaceScale[1] & 0xFFFF) & 0xFFFF));
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x14, (gLightSpaceScale[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x1C, (gLightSpaceScale[2] & 0xFFFF) << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x20, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x24, gLightSpaceScale[3] << 16);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x28, 0);
    gMoveWd(gfx++, G_MW_LIGHTSCALE, 0x2C, gLightSpaceScale[3] << 16);
    return gfx;
}

/* Loads the 0x30-byte light at `l` into the microcode light table at byte offset `ofs` (point lights start at 0x60). */
#define gSPPointLight(pkt, l, ofs) gDma2p((pkt), G_MOVEMEM, (l), sizeof(PointLightGfx), G_MV_LIGHT, (ofs))

/*
 * Picks the (maxLights - 1) point lights nearest to (x, y, z) for player `player`, fills this
 * frame's slice of lightBuf with them plus a directional light, and emits the commands that
 * load them, followed by `ambient`.
 */
Gfx *light_pickLights(Gfx *gfx, s32 player, s32 x, s32 y, s32 z, u32 arg5, PointLightGfx *lightBuf, s32 maxLights,
                   Ambient *ambient, u8 *outCount, s32 mask, u8 *smoothColor, s32 flags, PointLight **outLights) {
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

    light_getAmbientAt(x, y, z, mask, color, arg5, flags);
    j = 1 << player; // matching: dead store IDO still emits
    if (flags & 2) {
        func_1515E43C(x, y, z, mask, colorA, colorB, &amountA, &amountB);
        for (i = 0; i < 3; i++) {
            s32 intensity;
            intensity = (u8)((((0xFF - color[i]) * amountA) >> 8) + color[i]);
            dirColor[i] = (colorA[i] * intensity) >> 8;
            intensity = (u8)((((0xFF - color[i]) * amountB) >> 8) + color[i]);
            color[i] = (colorB[i] * intensity) >> 8;
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
        dir[0] = camera_global_Cameras[player].target.x - camera_global_Cameras[player].eye.x;
        dir[1] = camera_global_Cameras[player].target.y - camera_global_Cameras[player].eye.y;
        dir[2] = camera_global_Cameras[player].target.z - camera_global_Cameras[player].eye.z;
        scale = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (scale != 0.0f) {
            scale = 1000.0f / scale;
            pos[0] = dir[0] * scale + x;
            pos[1] = dir[1] * scale + y;
            pos[2] = dir[2] * scale + z;
        } else {
            pos[0] = x;
            pos[1] = y;
            pos[2] = z;
        }
    }
    count = 0;
    for (light = gPointLights; light != NULL; light = light->next) {
        if (((light->pickedPlayers & (1 << player)) || (flags & 0x20) || !(light->lightMask & 1)) && (light->lightMask & mask) &&
            (light->disabled == 0) && !(light->hiddenPlayers & (1 << player))) {
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
        out->dir[0] = gPlayerLightDirs[player][0];
        out->dir[1] = gPlayerLightDirs[player][1];
        out->dir[2] = gPlayerLightDirs[player][2];
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
                light->pickedPlayers |= 1 << player;
            }
            if (light->unk4 == 0) {
                light_fillGfxLight(light, player, out, x, y, z, flags);
            }
            gSPPointLight(gfx++, out, (i + 2) * 0x30U);
            continue; // matching: see G
        }
        if (padLight) {
            gSPPointLight(gfx++, &D_800DCD40, (i + 2) * 0x30U);
        }
        gSPPointLight(gfx++, &lightBuf[D_800BE9C0 * maxLights + i], (i + padLight + 1) * 0x30U + 0x30);
        gSPPointLight(gfx++, ambient, (i + padLight + 2) * 0x30U + 0x30);
    }
    return gfx;
}

/*
 * Ambient colour at (x, y, z): the colour of the first zone in gAmbientZones containing the
 * point, else the global ambient colour dimmed by bits 5-6 of arg5. With flags & 0x10 the
 * gDirectionalZones tint is applied on top.
 */
void light_getAmbientAt(s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg5, s32 flags) {
    TriggerZone *zone;
    s32 i;
    s32 sum;
    u8 tint[3];
    u8 colorA[3];
    u8 colorB[3];
    u8 amountA;
    u8 amountB;
    s32 intensity;
    u8 *out;
    s32 scale;

    zone = light_findZone(x, y, z, mask, gAmbientZones);
    if (zone != NULL) {
        color[0] = zone->data1 & 0xFF;
        color[1] = (zone->data1 >> 8) & 0xFF;
        color[2] = (zone->data1 >> 16) & 0xFF;
    } else {
        scale = 0x100 - ((arg5 >> 5) & 3) * 0x40;
        out = color;
        for (i = 0; i < 3; i++) {
            *out++ = (gAmbientColour[i] * scale) >> 8;
        }
    }
    if (flags & 0x10) {
        func_1515E43C(x, y, z, mask, colorA, colorB, &amountA, &amountB);
        out = color;
        for (i = 0; i < 3; i++) {
            intensity = (u8)((((0xFF - *out) * amountA) >> 8) + *out);
            tint[i] = (colorA[i] * intensity) >> 8;
            intensity = (u8)((((0xFF - *out) * amountB) >> 8) + *out);
            *out = (colorB[i] * intensity) >> 8;
            sum = *out + ((tint[i] * 7) >> 3);
            if (sum > 0xFF) {
                sum = 255;
            }
            *out = sum;
            out++;
        }
    }
}

/* Tint colours and amounts at (x, y, z), from the first gDirectionalZones zone containing it or the defaults. */
void func_1515E43C(s32 x, s32 y, s32 z, s32 arg3, u8 *colorA, u8 *colorB, u8 *amountA, u8 *amountB) {
    TriggerZone *zone;

    zone = light_findZone(x, y, z, 0, gDirectionalZones);
    if (zone != NULL) {
        colorA[0] = zone->data1 & 0xFF;
        colorA[1] = (zone->data1 >> 8) & 0xFF;
        colorA[2] = (zone->data1 >> 16) & 0xFF;
        colorB[0] = zone->data2 & 0xFF;
        colorB[1] = (zone->data2 >> 8) & 0xFF;
        colorB[2] = (zone->data2 >> 16) & 0xFF;
        *amountA = (zone->data2 >> 24) & 0xFF;
        *amountB = (zone->data1 >> 24) & 0xFF;
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
 * Re-emits the light commands light_pickLights built for this frame, from its light slice
 * `lightBuf` and the count byte it stored (bit 7 = padded with the dummy light).
 */
Gfx *light_reemitLights(Gfx *gfx, PointLightGfx *lightBuf, u8 maxLights, u8 countByte, Ambient *ambient) {
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
    gSPPointLight(gfx++, &lightBuf[D_800BE9C0 * maxLights + i], (i + padLight + 1) * 0x30U + 0x30);
    gSPPointLight(gfx++, ambient, (i + padLight + 2) * 0x30U + 0x30);
    return gfx;
}

/*
 * Lit colour at (x, y, z): the zone ambient colour (light_getAmbientAt) plus the contribution of
 * each point light picked for this frame, falling off with squared distance.
 */
void light_getLitColourAt(s32 player, s32 x, s32 y, s32 z, s32 mask, u8 *color, u32 arg6, s32 flags) {
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

    light_getAmbientAt(x, y, z, mask, color, arg6, flags);
    sum[0] = color[0];
    sum[1] = color[1];
    sum[2] = color[2];
    v = D_800D9E21 & 0x7F;
    count = v - 1;
    i = 0;
    if (count > 0) {
        lights = gPickedLights;
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

/* First zone in the list whose type matches `mask` (0 = any) and that contains (x, y, z). */
TriggerZone *light_findZone(s32 x, s32 y, s32 z, s32 mask, ZoneLink *node) {
    Actor probe;
    TriggerZone *zone;

    probe.xpos = x;
    probe.ypos = y;
    probe.floorY = y;
    probe.old_xpos = x;
    probe.old_ypos = y;
    probe.zpos = z;
    probe.old_zpos = z;

    for (; node != NULL; node = node->next) {
        zone = node->zone;
        if (zone->inactive == 0 && (mask == 0 || ((zone->data1 >> 24) & mask)) &&
            (ZONE_TYPE(zone) == ZONE_TYPE_LIGHT_DIRECTIONAL_LOCAL || !(zone->data1 >> 31)) && zone_isOutside(&probe, zone, 0) == 0) {
            return node->zone;
        }
    }
    return NULL;
}

/*
 * Fills the microcode light `out` from `light` as seen by player `player`: colour, direction
 * towards the light from (x, y, z) when flags & 2, and the view-space position (cached per player).
 */
void light_fillGfxLight(PointLight *light, s32 player, PointLightGfx *out, s32 x, s32 y, s32 z, s32 flags) {
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
        math_transformPoint(gViewMatrices[player], pos[0], pos[1], pos[2], &viewPos[0], &viewPos[1], &viewPos[2]);
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
    out->radius = light->radius;
}

/*
 * Stores the light-space scale for a fixed-point Mtx (viewed as halfwords: 16 integer parts,
 * then 16 fraction parts): 1 / the diagonal as 16.16 in gLightSpaceScale[0..2], and -z translation
 * in gLightSpaceScale[3].
 */
void func_1515EF74(s16 *mtx) {
    func_1515F040(1.0f / light_getMtxElement(mtx, 0), 0);
    func_1515F040(1.0f / light_getMtxElement(mtx, 5), 1);
    func_1515F040(1.0f / light_getMtxElement(mtx, 10), 2);
    func_1515F0AC(-light_getMtxElement(mtx, 14), 3);
}

/* Element `i` (0-15) of a fixed-point Mtx as a float. */
f32 light_getMtxElement(s16 *mtx, s32 i) {
    s32 value = mtx[i];
    value = (value << 16) | (u16)mtx[i + 16];
    return value * (1.0f / 65536.0f);
}

/* Clamps `value` to the s16 range and stores it in gLightSpaceScale[i] as 16.16 fixed point. */
void func_1515F040(f32 value, s32 i) {
    if (value >= 32767.0f) {
        value = 32767.0f;
    } else if (value < -32768.0f) {
        value = -32768.0f;
    }
    value *= 65536.0f;
    gLightSpaceScale[i] = value;
}

/* Clamps `value` to the s16 range and stores it in gLightSpaceScale[i] as an integer. */
void func_1515F0AC(f32 value, s32 i) {
    if (value >= 32767.0f) {
        value = 32767.0f;
    } else if (value < -32768.0f) {
        value = -32768.0f;
    }
    gLightSpaceScale[i] = value;
}

/* Unlinks `light` from the light list and frees it. */
void light_freePointLight(light)
    PointLight *light;
{
    PointLight *node;
    PointLight *prev;

    prev = NULL;
    for (node = gPointLights; node != light; node = node->next) {
        prev = node;
    }
    if (prev != NULL) {
        prev->next = node->next;
    } else {
        gPointLights = node->next;
    }
    heap_free(node);
}

/* Sets the disabled flag of every light in `group`. */
void light_setGroupDisabled(s32 group, u8 disabled) {
    PointLight *light;

    for (light = gPointLights; light != NULL; light = light->next) {
        if (group == light->group) {
            light->disabled = disabled;
        }
    }
}
