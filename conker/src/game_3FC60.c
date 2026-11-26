#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init_50A0.c).
u8 (*gVtxBaseColours[4])[3]; // [0]: base RGB of every level vertex, then the props'; [i]: level model i's first vertex
u8 (***gVtxGroupBaseColours)[3];   // [set][group][i]: base RGB of each color-group vertex
u16 *gPropVtxColourStarts;         // first gVtxBaseColours[0] entry of each prop
static u16 gVtxColourCount;    // vertices recorded so far
static u8 gLevelEnvColours[30]; // RGB of each G_SETENVCOLOR of the level's first display list
static s16 gLevelEnvColourCmds[10]; // its command index
static u8 gLevelEnvColourCount;     // count of gLevelEnvColours entries

#include "functions.h"
#include "variables.h"

void env_initVtxColours(void) {
    s32 total;
    s32 i;
    s32 j;
    s32 k;
    VtxColorGroup *b;
    Vtx *src;

    gVtxGroupBaseColours = NULL;
    if (D_800B0DF0->vtxColorFlags != 0) {
        total = 0;
        if (D_800B0DF0->vtxColorFlags & 1) {
            for (i = 0; i < 4; i++) {
                total += gLevelVertexCounts[i];
            }
        }
        if (D_800B0DF0->vtxColorFlags & 2) {
            for (i = 0; i < gWorldObjectCount; i++) {
                total += db_Object_array[i].numVertices;
            }
        }
        gVtxBaseColours[0] = allocate_memory(total * 3, 1, 0, 0);
        if (gVtxBaseColours[0] == NULL) {
            gVtxBaseColours[1] = NULL;
            return;
        }
        gVtxColourCount = 0;
        gLevelEnvColourCount = 0;
        if (D_800B0DF0->vtxColorFlags & 1) {
            for (i = 0; i < 4; i++) {
                if (gLevelVertices[i] != NULL) {
                    gVtxBaseColours[i] = &gVtxBaseColours[0][gVtxColourCount];
                    env_recordVtxColours(gLevelVertices[i], gLevelVertexCounts[i]);
                } else {
                    gVtxBaseColours[i] = NULL;
                }
            }
        }
        if (D_800B0DF0->vtxColorFlags & 2) {
            gPropVtxColourStarts = allocate_memory(gWorldObjectCount * 2, 1, 0, 0);
            for (i = 0; i < gWorldObjectCount; i++) {
                if (db_Object_array[i].numVertices != 0) {
                    gPropVtxColourStarts[i] = gVtxColourCount;
                    env_recordVtxColours(db_Object_array[i].srcVertices, db_Object_array[i].numVertices);
                } else {
                    gPropVtxColourStarts[i] = 0xFFFF;
                }
            }
        }
        if (D_800B0DF0->vtxColorFlags & 4) {
            gVtxGroupBaseColours = allocate_memory(gVtxColorSetCount * 4, 1, 0, 0);
            bzero(gVtxGroupBaseColours, gVtxColorSetCount * 4);
            for (i = 0; i < gVtxColorSetCount; i++) {
                if (gVtxColorGroupCounts[i] != 0) {
                    gVtxGroupBaseColours[i] = allocate_memory(gVtxColorGroupCounts[i] * 4, 1, 0, 0);
                    b = gVtxColorGroups[i];
                    for (j = 0; j < gVtxColorGroupCounts[i]; j++) {
                        gVtxGroupBaseColours[i][j] = allocate_memory(b[j].count * 3, 1, 0, 0);
                        if (i < 4) {
                            src = gLevelVertices[i];
                        } else {
                            src = db_Object_array[i - 4].srcVertices;
                        }
                        for (k = 0; k < b[j].count; k++) {
                            gVtxGroupBaseColours[i][j][k][0] = src[b[j].vertexIndices[k]].v.cn[0];
                            gVtxGroupBaseColours[i][j][k][1] = src[b[j].vertexIndices[k]].v.cn[1];
                            gVtxGroupBaseColours[i][j][k][2] = src[b[j].vertexIndices[k]].v.cn[2];
                        }
                    }
                } else {
                }
            }
        }
        env_recordEnvColours(gLevelDisplayLists[0]);
    }
}

void env_recordVtxColours(Vtx *arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < arg1; i++) {
        gVtxBaseColours[0][gVtxColourCount][0] = arg0[i].v.cn[0];
        gVtxBaseColours[0][gVtxColourCount][1] = arg0[i].v.cn[1];
        gVtxBaseColours[0][gVtxColourCount][2] = arg0[i].v.cn[2];
        gVtxColourCount++;
    }
}

/* Records the colour of each G_SETENVCOLOR command (opcode 0xFB) in display list dl, up to its G_ENDDL. */
void env_recordEnvColours(Gfx *dl) {
    s32 i;
    s32 op;

    for (i = 0; (op = (s8)(dl[i].words.w0 >> 24)) != -0x21; i++) {
        if (op == -5) {
            gLevelEnvColourCmds[gLevelEnvColourCount] = i;
            gLevelEnvColours[gLevelEnvColourCount * 3] = (dl[i].words.w1 >> 24) & 0xFF;
            gLevelEnvColours[gLevelEnvColourCount * 3 + 1] = (dl[i].words.w1 >> 16) & 0xFF;
            gLevelEnvColours[gLevelEnvColourCount * 3 + 2] = (dl[i].words.w1 >> 8) & 0xFF;
            gLevelEnvColourCount++;
        }
    }
}

/* Clears the attachment model reference counts (game/Attach/AttachModel/attachmodel), all but the last of the 0x1E4. */
void env_clearAttachModelRefs(void) {
    s32 i;

    // matching: IDO peels the 3 left over from 0x1E3 / 4 before the unrolled loop
    for (i = 0; i < 0x1E3; i++) {
        gAttachModelRefCounts[i] = 0;
    }
}
