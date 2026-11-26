#include <ultra64.h>
#include "structs.h"

/*
 * Per-model tables, indexed by model id (0..186), linked at 0x800C4020..0x800C6650 in this order. They are
 * defined before functions.h and variables.h declare them (conker/model.h): IDO lays out .bss in the order
 * it first sees each name, and that order also depends on the declarations before it (see init_50A0.c).
 */
s32 *D_800C4020[187];                 // per model: first vertex of each part
u16 D_800C4310[187];                  // per model: part count
Gfx **gModelDlParts[187];                // per model: display-list parts (ModelHeader dlists)
u16 gModelDlPartCounts[187];                  // per model: display-list part count
Gfx **D_800C48F0[187];                // per model: display list table (ModelHeader dlists2)
void *gModelBones[187];                // per model: bone table (16-byte entries)
u16 gModelBoneCounts[187];                  // per model: bone count
void *D_800C5048[187];                // per model: ModelHeader unk20
ObjTexture *gModelTextures[187];          // per model: texture table
u16 gModelTextureCounts[187];                  // per model: texture count
u16 gModelVertexCounts[187];                  // per model: vertex count
u16 D_800C5918[187];                  // per model: triangle weight, see func_1503D984
u16 gModelAnimCounts[187];                  // per model: animation count
VtxRun *gModelVtxRuns[187];              // per model: vertex runs (gModelVtxRunCounts[id] of them)
u16 gModelVtxRunCounts[187];                  // per model: vertex run count
VtxTri *gModelTris[187];              // per model: triangles
TexCoord *gModelSavedTexCoords[187];            // per model: saved texture coordinates (model_saveTexCoords)

#include "functions.h"
#include "variables.h"

/*
 * A model file, as loaded by model_load: seven (data, byte size) sections whose offsets
 * func_1502B6BC has already relocated, followed by the model data proper (its vertices first),
 * which D_800D19A0[id] points at. Offsets inside the sections are relative to the header.
 */
typedef struct ModelHeader {
    /* 0x00 */ s32 *vtxIndices; // indices into the model's vertices (D_800C4020)
    /* 0x04 */ u32 vtxIndicesSize;
    /* 0x08 */ Gfx **dlists; // display-list parts (gModelDlParts)
    /* 0x0C */ u32 dlistsSize;
    /* 0x10 */ void *bones; // 16-byte entries (gModelBones)
    /* 0x14 */ u32 bonesSize;
    /* 0x18 */ ObjTexture *textures; // gModelTextures
    /* 0x1C */ u32 texturesSize;
    /* 0x20 */ void *unk20; // D_800C5048
    /* 0x24 */ u32 unk24;
    /* 0x28 */ Gfx **dlists2; // D_800C48F0
    /* 0x2C */ u32 dlists2Size;
    /* 0x30 */ u8 *unk30; // byte table read by func_1503DA3C
    /* 0x34 */ u32 unk30Size;
} ModelHeader; // size 0x38

/* Byte view of a Gfx command. */
typedef struct GfxBytes {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[2];
    /* 0x3 */ u8 index; // G_MOVEMEM: what is moved
    /* 0x4 */ s32 addr;
} GfxBytes;


/*
 * Loads model `id` (unless it is already loaded) and sets up its per-model tables. With
 * loadTextures, also loads its textures and patches them into its display lists. Returns 0, or an
 * OR of error bits (1 out of memory, 2/4 a missing data block, 8 no memory for the vertex runs,
 * 0x10 a missing texture), in which case everything loaded so far is freed again.
 */
s32 model_load(s32 id, s32 arg1, SpawnRecord *def, s32 loadTextures, s32 flags) {
    s32 ret;
    ModelHeader *hdr;
    u32 size;
    u32 n; // matching: one variable for the vertex end and the dlists2 count (splitting it changes regalloc)
    s32 i;

    if (id == 0xFF) {
        return 0;
    }
    if (id >= 187) {
        return 0;
    }
    ret = 0;
    if (D_800D19A0[id] != NULL) {
        return 0;
    }
    hdr = func_1502B6BC(&size, 7, NULL, 2, 1, id);
    if (hdr == NULL) {
        return 1;
    }
    D_800D19A0[id] = (u8 *)(hdr + 1);
    D_800C4020[id] = hdr->vtxIndices;
    gModelDlParts[id] = hdr->dlists;
    D_800C48F0[id] = hdr->dlists2;
    gModelBones[id] = hdr->bones;
    gModelTextures[id] = hdr->textures;
    D_800C5048[id] = hdr->unk20;
    D_800C4310[id] = hdr->vtxIndicesSize / sizeof(s32);
    // the vertices run up to the next section present
    if (D_800C4020[id] != NULL) {
        n = (u32)D_800C4020[id];
    } else {
        n = (u32)gModelDlParts[id];
    }
    gModelVertexCounts[id] = (n - (u32)hdr - sizeof(ModelHeader)) / sizeof(Vtx);
    gModelDlPartCounts[id] = hdr->dlistsSize / sizeof(Gfx *);
    for (i = 0; i < gModelDlPartCounts[id]; i++) {
        model_relocateOffset((s32 *)&gModelDlParts[id][i], (s32)hdr);
        model_relocateDisplayList((s8 *)gModelDlParts[id][i], (s32)hdr);
    }
    n = hdr->dlists2Size / sizeof(Gfx *);
    for (i = 0; i < n; i++) {
        model_relocateOffset((s32 *)&D_800C48F0[id][i], (s32)hdr);
        model_relocateDisplayList((s8 *)D_800C48F0[id][i], (s32)hdr);
    }
    gModelTextureCounts[id] = hdr->texturesSize / sizeof(ObjTexture);
    gModelBoneCounts[id] = hdr->bonesSize / 16;
    if (loadTextures) {
        ret |= model_loadTextures(id);
        if (ret == 0) {
            for (i = 0; i < gModelDlPartCounts[id]; i++) {
                func_1510CE60(gModelDlParts[id][i], 0, 0, 0x3E, NULL);
            }
        }
    }
    if (ret == 0) {
        func_1503D984(id);
        ret |= model_saveTexCoords(id);
    }
    if (ret == 0 && ((def != NULL && (def->objFlags & 0x4000)) || (flags & 1))) {
        if (func_150028BC(id) != 0) {
            ret |= 8;
        }
    }
    if (ret == 0) {
        ret |= func_1503D774(id, 0);
        if (ret == 0) {
            ret |= model_loadAnimTable(id, 0);
        }
    }
    if (ret != 0) {
        if (gModelSavedTexCoords[id] != NULL) {
            func_10004074(gModelSavedTexCoords[id]);
        }
        model_freeTextures(id);
        if (gModelVtxRuns[id] != NULL) {
            func_10004074((void *)(s32)gModelVtxRuns[id]); // matching: the int round trip keeps the load in v0
            gModelVtxRuns[id] = NULL;
        }
        if (gModelTris[id] != NULL) {
            func_10004074((void *)(s32)gModelTris[id]); // matching: as above
            gModelTris[id] = NULL;
        }
        if (D_800D19A0[id] != NULL) {
            func_10004074(D_800D19A0[id] - sizeof(ModelHeader));
            D_800D19A0[id] = NULL;
        }
    }
    return ret;
}

/* Relocates the vertex and matrix addresses (G_VTX, G_MOVEMEM G_MV_MATRIX) in display list dl. */
void model_relocateDisplayList(s8 *dl, s32 base) {
    GfxBytes *cmd;
    s32 i;

    if (dl != NULL) {
        // matching: byte pointer indexed with i << 3 (a GfxBytes index gets strength-reduced)
        for (i = 0; dl[i << 3] != (s8)G_ENDDL; i++) {
            cmd = (GfxBytes *)&dl[i << 3];
            switch (cmd->opcode) {
            case G_VTX:
                model_relocateOffset(&cmd->addr, base);
                break;
            case (s8)G_MOVEMEM:
                if (cmd->index == G_MV_MATRIX) {
                    model_relocateOffset(&cmd->addr, base);
                }
                break;
            }
        }
    }
}

/* Adds base to the offset in *offsetSlot, unless it is 0 or already a segmented address. */
void model_relocateOffset(s32 *offsetSlot, s32 base) {
    s32 offset = *offsetSlot;

    if (offset != 0 && (offset & 0x0F000000) == 0) {
        *offsetSlot = offset + base;
    }
}

/* Adds base to every offset of a 0-terminated list. */
void model_relocateOffsetList(OffsetEntry *entry, s32 base) {
    s32 offset = entry->offset;

    while (offset != 0) {
        entry->offset = offset + base;
        offset = entry[1].offset;
        entry++;
    }
}

/* AnimHeader as loaded: its pointers are still offsets from the header, made absolute by model_loadAnimTable. */
typedef struct AnimHeaderRaw {
    /* 0x0 */ s32 scripts; // OffsetEntry list
    /* 0x4 */ s32 hits;
    /* 0x8 */ s32 reactions;
    /* 0xC */ u32 reactionsSize;
} AnimHeaderRaw;

/* Relocates the offsets of the animation table starting at anim and stores its length in gModelAnimCounts[id]. */
void model_relocateAnimTable(ActorAnim *anim, s32 id) {
    ActorAnim *anims = anim;

    while (anim->unk0 != 999) {
        if (anim->offset != 0) {
            model_relocateOffset(&anim->offset, (s32)anims);
        }
        anim++;
    }

    gModelAnimCounts[id] = anim - anims;
}

/* Groups of model ids that share one animation table: D_80098888[i] ids in row D_80084410[i]. */
static const u8 D_80098860[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x3B, 0x82, 0x88, 0x90, 0x96, 0x98, 0x9C, 0x9D, 0x9F, 0xA0, 0xB0, 0xB1, 0xB2, 0x75, 0xB4,
};
static const u8 D_80098874[] = { 0x15, 0x18, 0x1E };
static const u8 D_80098878[] = { 0x3A, 0x3D };
static const u8 D_8009887C[] = { 0x5A, 0x74, 0x74, 0x5F, 0x8D };
static const u8 D_80098884[] = { 0x05, 0xAD, 0xAE, 0xAF };
static const u8 D_80098888[] = { 20, 3, 2, 5, 4 };
static const u8 *D_80084410[] = { D_80098860, D_80098874, D_80098878, D_8009887C, D_80098884 };

/* Copies model id's animation table and count to the other models of its group. */
void model_shareAnimTable(s32 id) {
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < D_80098888[i]; j++) {
            if (D_80084410[i][j] == id) {
                for (k = 0; k < D_80098888[i]; k++) {
                    gModelAnimTables[D_80084410[i][k]] = gModelAnimTables[id];
                    gModelAnimCounts[D_80084410[i][k]] = gModelAnimCounts[id];
                }
                return;
            }
        }
    }
}

/* Looks id up in the 5 rows of D_80084410 (row lengths in D_80098888) and returns the
 * first entry of the row that contains it, or id itself when no row does.
 * Same search as func_15084D00 (game_B21B0). */
s32 func_1503D5F0(s32 id) {
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < D_80098888[i]; j++) {
            if (D_80084410[i][j] == id) {
                return D_80084410[i][0];
            }
        }
    }
    return id;
}

/* gModelAnimTables[id] while it still points at the header (the original re-reads it on every use). */
#define HEADER (((AnimHeaderRaw **)gModelAnimTables)[id])

/*
 * Loads the animation table of model id (shared by its group, see model_shareAnimTable) into
 * gModelAnimTables[id], unless it is already loaded. Returns 0, or 4 if it is missing.
 */
s32 model_loadAnimTable(s32 id, s32 arg1) {
    u32 size;
    AnimHeaderRaw *data;

    if (HEADER != NULL) {
        return 0;
    }
    data = func_1502B5C8(&size, 2, 15, func_1503D5F0(id));
    if (data == NULL) {
        HEADER = NULL;
        gModelAnimCounts[id] = 0;
        model_shareAnimTable(id);
        return 4;
    }
    HEADER = data;
    if (HEADER->scripts != 0) {
        HEADER->scripts += (s32)HEADER;
        model_relocateOffsetList((OffsetEntry *)HEADER->scripts, (s32)(HEADER + 1));
    }
    if (HEADER->hits != 0) {
        HEADER->hits += (s32)HEADER;
    }
    if (HEADER->reactions != 0) {
        HEADER->reactions += (s32)HEADER;
    }
    HEADER++; // now the animation table proper
    model_relocateAnimTable((ActorAnim *)HEADER, id);
    model_shareAnimTable(id);
    return 0;
}

#undef HEADER

/* Loads model id's D_800D1C90 entry, unless it is already loaded. Returns 0, or 2 if it is missing. */
s32 func_1503D774(s32 id, s32 arg1) {
    u32 size;
    ModelDef **table;

    if (D_800D1C90[id] != NULL) {
        return 0;
    }
    table = func_1502B6BC(&size, 2, NULL, 2, 17, id);
    // matching: if/else (an early return without the else schedules the stores differently)
    if (table == NULL) {
        D_800D1C90[id] = NULL;
        return 2;
    } else {
        // the original stores the table, then replaces it with its first entry
        D_800D1C90[id] = (ModelDef *)table;
        D_800D1C90[id] = *table;
    }
    return 0;
}

/*
 * Saves the texture coordinates of the vertices that model 0x24 animates (94 of them, from its
 * first vertex index on) into gModelSavedTexCoords[id]. Returns 1 when out of memory.
 */
s32 model_saveTexCoords(s32 id) {
    s32 count;
    s32 first;
    Vtx *src;
    TexCoord *dst;
    s32 i;

    if (id == 0x24) {
        count = 94;
        first = 0;
    } else {
        first = -1;
        count = 0;
    }

    if (count == 0) {
        return 0;
    }
    if (gModelSavedTexCoords[id] != NULL) {
        return 0;
    }

    src = (Vtx *)D_800D19A0[id];
    if (first != -1) {
        src += D_800C4020[id][first];
    }
    gModelSavedTexCoords[id] = allocate_memory(count * sizeof(TexCoord), 1, 0, 2);
    if (gModelSavedTexCoords[id] == NULL) {
        return 1;
    }
    dst = gModelSavedTexCoords[id];
    for (i = 0; i < count; i++) {
        dst[i].s = src[i].v.tc[0];
        dst[i].t = src[i].v.tc[1];
    }
    return 0;
}

/* Sums primitive weights over model id's display list, up to G_ENDDL: G_TRI1 1, G_TRI2 2, ops 0x10-0x1F 4. */
void func_1503D984(s32 id) {
    s32 count = 0;
    s32 i;
    s32 j;
    Gfx *dl;
    s8 op;

    // only the first display-list part is counted; the binary keeps the one-pass loop
    for (i = 0; i < 1; i++) {
        dl = gModelDlParts[id][i];
        for (j = 0; (op = dl[j].words.w0 >> 24) != (s8)G_ENDDL; j++) {
            if (op == G_TRI1) {
                count += 1;
            } else if (op == G_TRI2) {
                count += 2;
            } else if ((op >> 4) == 1) {
                count += 4;
            }
        }
    }
    D_800C5918[id] = count;
}

/* Returns entry `index` of model id's unk30 byte table, or 0xFF if there is none. */
s32 func_1503DA3C(s32 id, s32 index) {
    u8 *data = D_800D19A0[id];
    ModelHeader *hdr = (ModelHeader *)data - 1;
    u8 *table;
    s32 ret;

    if (data == NULL) {
        return 0xFF;
    }
    if (hdr->unk30Size < index + 1) {
        return 0xFF;
    }
    table = hdr->unk30;
    ret = (table != NULL) ? table[index] : 0xFF;
    return ret;
}

/*
 * Makes sure model id is loaded for obj (and, with loadTextures, its textures), then sets up obj's
 * model'th model with func_15084044. Returns 0, or the error bits of the step that failed.
 */
s32 model_loadForActor(Actor *obj, s32 id, s32 model, s32 loadTextures) {
    SpawnRecord *def;
    Gfx *dl;
    s32 ret;
    s32 i;

    def = obj->listpos_ptr;
    if (D_800D19A0[id] == NULL) {
        ret = model_load(id, 0, def, loadTextures, obj->unkAC);
    } else {
        ret = 0;
        if ((def != NULL && (def->objFlags & 0x4000)) || (obj->unkAC & 1)) {
            if (gModelVtxRuns[id] == NULL) {
                func_150028BC(id);
            }
        }
        // textures not loaded yet: the first entry's addr is still an offset
        if (loadTextures && gModelTextureCounts[id] != 0 && gModelTextures[id]->addr < 0x10000000) {
            ret = model_loadTextures(id);
            for (i = 0; i < gModelDlPartCounts[id]; i++) {
                dl = gModelDlParts[id][i];
                func_1510CE60(dl, 0, 0, 0x3E, NULL);
            }
        }
    }
    if (ret == 0) {
        ret = func_15084044(obj, model);
    }
    return ret;
}

/* Loads model id's textures into its gModelTextures table. Returns 0x10 if any is missing, else 0. */
s32 model_loadTextures(s32 id) {
    s32 ret = 0;
    s32 i;

    for (i = 0; i < gModelTextureCounts[id]; i++) {
        gModelTextures[id][i].addr = func_1510D0EC(gModelTextures[id][i].id, NULL, 0x3E, 1);
        if (gModelTextures[id][i].addr == 0x80000000) {
            ret |= 0x10;
        }
    }
    return ret;
}

/* Releases model id's loaded textures. */
void model_freeTextures(s32 id) {
    s32 i;

    for (i = 0; i < gModelTextureCounts[id]; i++) {
        ObjTexture *tex = &gModelTextures[id][i];

        if (tex->addr != 0x80000000 && tex->id != (s32)tex->addr) { // matching: the cast keeps the original operand order
            func_1510D7AC(tex->id);
        }
    }
}
