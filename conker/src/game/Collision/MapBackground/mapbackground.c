#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void collision_clearUvWobbles(void) {
    gUvWobbleCount = 0;
}

/**
 * Sets up the current map's background according to gLevelHeader->skyMode:
 * mode 4 allocates two vertex buffers per view (gScreenLayout + 1 views), mode 2 builds a
 * 256-entry, four-band colour gradient (chosen by map) plus a quad per view and buffer.
 */
void collision_initMapBackground(s32 arg0) {
    s32 i;
    s32 j;
    s32 end; // one past the last gradient entry of the current band
    s32 start;
    s32 size;
    s32 from[3];
    s32 to[3];
    s32 cur[3];
    s32 color;
    s32 *buf;
    Vtx *vtx;

    gBackgroundColour[0] = gLevelHeader->backgroundRed;
    gBackgroundColour[1] = gLevelHeader->backgroundGreen;
    gBackgroundColour[2] = gLevelHeader->backgroundBlue;
    if (gIsMultiplayer != 0 && gLevelHeader->skyMode == 4) {
        gLevelHeader->skyMode = 2;
    }
    if (gLevelHeader->skyMode == 4) {
        size = (gScreenLayout != 1) ? 0x130 : 0x1C0;
        for (i = 0; i <= gScreenLayout; i++) {
            vtx = allocate_memory(size * 32, 1, 2, 0); // two buffers of `size` Vtx
            gSkyQuadVtx[i][0] = vtx;
            gSkyQuadVtx[i][1] = gSkyQuadVtx[i][0] + size;
        }
        rom_loadPathTo(&gSkyData, 2, 0xD, gLevelHeader->skyTextureSet);
        if (gExpansionPak != 0) {
            for (i = 0; i < 360; i++) {
                texture_getAddress(gSkyTextureSets[gLevelHeader->skyTextureSet][0] + i, 0, 0x3F, 0);
            }
        }
        gSkyMatrices = allocate_memory((gScreenLayout + 1) * 0x80, 1, 2, 0);
        if (level == LEVEL_W3_HAUNTED_HOUSE) {
            D_800DBEAB = 1;
        } else {
            D_800DBEAB = 0;
        }
    } else if (gLevelHeader->skyMode == 2) {
        gSkyData = allocate_memory(0x1000, 1, 1, 0);
        buf = gSkyData;
        switch (level) {
            case LEVEL_W1_BULL_PADDOCK:
            case LEVEL_W1_TRAINING:
            case LEVEL_WMP_MAIN_WAR:
            case LEVEL_WMP_BEACH_WAR:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0xBF;
                end = 0xA7;
                to[0] = 0xD0;
                to[1] = 0xD5;
                to[2] = 0xFF;
                break;
            case LEVEL_WMP_BEACH:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                end = 0x85;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x5D;
                break;
            case LEVEL_WMP_TANK:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                end = 0x7B;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x1F;
                break;
            default:
                from[0] = 0;
                from[1] = 0x67;
                from[2] = 0x38;
                end = 0x23;
                to[0] = 0xC8;
                to[1] = 0xD5;
                to[2] = 0x2E;
                break;
        }
        for (i = 0; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * i / end + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (level) {
            case LEVEL_W1_BULL_PADDOCK:
            case LEVEL_W1_TRAINING:
            case LEVEL_WMP_MAIN_WAR:
            case LEVEL_WMP_BEACH_WAR:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB1;
                to[0] = 0x12;
                to[1] = 0x4E;
                to[2] = 0x19;
                break;
            case LEVEL_WMP_BEACH:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0x9C;
                to[0] = 0x1C;
                to[1] = 0x15;
                to[2] = 0x93;
                break;
            case LEVEL_WMP_TANK:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xA2;
                to[0] = 0x3;
                to[1] = 0x1C;
                to[2] = 0x36;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0x63;
                to[0] = 0xFF;
                to[1] = 0x3E;
                to[2] = 0x12;
                break;
        }
        for (i = start; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (end - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (level) {
            case LEVEL_W1_BULL_PADDOCK:
            case LEVEL_W1_TRAINING:
            case LEVEL_WMP_MAIN_WAR:
            case LEVEL_WMP_BEACH_WAR:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xDD;
                to[0] = 0;
                to[1] = 0xC7;
                to[2] = 0;
                break;
            case LEVEL_WMP_BEACH:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB4;
                to[0] = 0xA;
                to[1] = 0x22;
                to[2] = 0xFF;
                break;
            case LEVEL_WMP_TANK:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB7;
                to[0] = 0x21;
                to[1] = 0x24;
                to[2] = 0x31;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xC8;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0;
                break;
        }
        for (i = start; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (end - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        for (i = end; i < 0x100; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = to[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        gSkyMatrices = allocate_memory((gScreenLayout + 1) * 0x80, 1, 1, 0);
        for (i = 0; i <= gScreenLayout; i++) {
            vtx = allocate_memory(2 * 4 * sizeof(Vtx), 1, 1, 0);
            gSkyQuadVtx[i][0] = vtx;
            gSkyQuadVtx[i][1] = vtx + 4;
            for (j = 0; j < 2; j++) {
                gSkyQuadVtx[i][j][0].v.ob[0] = -1750;
                gSkyQuadVtx[i][j][0].v.ob[1] = 1750;
                gSkyQuadVtx[i][j][0].v.ob[2] = -1000;
                gSkyQuadVtx[i][j][0].v.flag = 0;
                gSkyQuadVtx[i][j][0].v.tc[0] = 0x2000;
                gSkyQuadVtx[i][j][1].v.ob[0] = 1750;
                gSkyQuadVtx[i][j][1].v.ob[1] = 1750;
                gSkyQuadVtx[i][j][1].v.ob[2] = -1000;
                gSkyQuadVtx[i][j][1].v.flag = 0;
                gSkyQuadVtx[i][j][1].v.tc[0] = 0x2000;
                gSkyQuadVtx[i][j][2].v.ob[0] = 1750;
                gSkyQuadVtx[i][j][2].v.ob[1] = -1750;
                gSkyQuadVtx[i][j][2].v.ob[2] = -1000;
                gSkyQuadVtx[i][j][2].v.flag = 0;
                gSkyQuadVtx[i][j][2].v.tc[0] = 0x2000;
                gSkyQuadVtx[i][j][3].v.ob[0] = -1750;
                gSkyQuadVtx[i][j][3].v.ob[1] = -1750;
                gSkyQuadVtx[i][j][3].v.ob[2] = -1000;
                gSkyQuadVtx[i][j][3].v.flag = 0;
                gSkyQuadVtx[i][j][3].v.tc[0] = 0x2000;
            }
        }
    } else if (gLevelHeader->skyMode == 1) {
        env_initStars();
    }
    if (gTaskDoneBits[0x14] & 1) {
        D_800DBEAC = 1;
    } else {
        D_800DBEAC = 0;
    }
}
