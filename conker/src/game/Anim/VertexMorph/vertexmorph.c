#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The models whose vertices morph between keyframes: model kind, keyframe count and the vertex count
 * of each part. */
const VtxMorphGroup gVtxMorphGroups[2] = {
    { 0x00, 16, { 337, 12, 12 } },
    { 0x3A, 7, { 109, 25, 24 } },
};

VtxMorphFrame **gVtxMorphCache[2];

/* Ages the cached morph keyframes and frees the ones that have not been used for a while. */
void anim_ageMorphCache(void) {
    s32 group;
    s32 part;
    s32 frame;

    for (group = 0; group < 2; group++) {
        if (gVtxMorphCache[group] != NULL) {
            for (part = 0; part < gModelVtxIndexCounts[gVtxMorphGroups[group].kind]; part++) {
                for (frame = 0; frame < gVtxMorphGroups[group].frameCount; frame++) {
                    if (gVtxMorphCache[group][part][frame].positions != NULL) {
                        if (gVtxMorphCache[group][part][frame].timer != 0) {
                            gVtxMorphCache[group][part][frame].timer--;
                        } else {
                            heap_free(gVtxMorphCache[group][part][frame].positions);
                            gVtxMorphCache[group][part][frame].positions = NULL;
                        }
                    }
                }
            }
        }
    }
}

/*
 * Morphs actor `actorIndex`'s vertices towards keyframe `frame` over `duration` ticks, loading the
 * keyframes on demand. Returns 0 when the vertices were written, -1 otherwise.
 */
s32 anim_morphVertices(s32 frame, s32 duration, s32 actorIndex) {
    VtxMorphState *morph;
    const VtxMorphGroup *config;
    s32 id;
    u32 size;
    s32 kind;
    s32 set;
    s32 i;
    s32 part;
    Vtx *vtx;
    s32 group;
    s16 (*from)[3];
    s16 (*to)[3];
    Vtx *vtxBase;

    id = movement_obj[actorIndex].character_type;
    kind = collision_getModelId(&movement_obj[actorIndex]);
    set = movement_obj[actorIndex].current_lod;
    config = NULL;
    for (i = 0; i < 2; i++) {
        if (kind == gVtxMorphGroups[i].kind) {
            config = &gVtxMorphGroups[i];
            group = i;
            break;
        }
    }
    if (config == NULL) {
        return -1;
    }
    morph = movement_obj[actorIndex].morph;
    if (set != 0) {
        set = 0;
        id = kind;
        if (movement_obj[actorIndex].modelData[set][gDrawBufferIndex] == NULL || gModelData[id] == NULL) {
            if (frame == morph->cur) {
                if (gPaused == 0) {
                    morph->blend += morph->rate * gFrameTicks;
                }
                if (morph->blend > 1.0f) {
                    morph->blend = 1.0f;
                }
            } else {
                if (morph->cur != 0xFF) {
                    morph->prev = morph->cur;
                }
                morph->cur = frame;
                morph->blend = 0.0f;
                if (duration <= 0) {
                    duration = 1;
                }
                morph->rate = 1.0f / duration;
            }
            morph->hold = 3;
            return -1;
        }
    }
    if (gModelData[id] == NULL) {
        return -1;
    }
    if (movement_obj[actorIndex].modelData[set][gDrawBufferIndex] == NULL) {
        return -1;
    }
    if (frame == morph->cur && morph->hold == 0) {
        morph->prev = morph->cur;
        return 0;
    }
    if (frame != morph->cur) {
        if (morph->cur != 0xFF) {
            morph->prev = morph->cur;
        }
        morph->cur = frame;
        morph->blend = 0.0f;
        if (duration <= 0) {
            duration = 1;
        }
        morph->hold = 3;
        morph->rate = 1.0f / duration;
    }
    if (gVtxMorphCache[group] == NULL) {
        s32 failed;

        gVtxMorphCache[group] = allocate_memory(gModelVtxIndexCounts[config->kind] * sizeof(VtxMorphFrame *), 1, 0, 2);
        if (gVtxMorphCache[group] == NULL) {
            return -1;
        }
        failed = FALSE;
        for (i = 0; i < gModelVtxIndexCounts[config->kind]; i++) {
            gVtxMorphCache[group][i] = allocate_memory(config->frameCount * sizeof(VtxMorphFrame), 1, 0, 2);
            if (gVtxMorphCache[group][i] != NULL) {
                bzero(gVtxMorphCache[group][i], config->frameCount * sizeof(VtxMorphFrame));
            } else {
                failed = TRUE;
            }
        }
        if (failed) {
            for (i = 0; i < gModelVtxIndexCounts[config->kind]; i++) {
                if (gVtxMorphCache[group][i] != NULL) {
                    // matching: the s32 round trip changes IDO's register choice for the row pointer
                    heap_free((void *)(s32)gVtxMorphCache[group][i]);
                }
            }
            return -1;
        }
    }
    if (gPaused == 0) {
        morph->blend += morph->rate * gFrameTicks;
    }
    if (morph->blend > 1.0f) {
        morph->blend = 1.0f;
    }
    vtxBase = (Vtx *)movement_obj[actorIndex].modelData[set][gDrawBufferIndex];
    for (part = 0; part < gModelVtxIndexCounts[config->kind]; part++) {
        if (gVtxMorphCache[group][part][morph->cur].positions == NULL) {
            gVtxMorphCache[group][part][morph->cur].positions =
                rom_loadPath(&size, 4, 0x13, group, part, morph->cur);
            if (gVtxMorphCache[group][part][morph->cur].positions == NULL) {
                return -1;
            }
        }
        if (gVtxMorphCache[group][part][morph->prev].positions == NULL) {
            gVtxMorphCache[group][part][morph->prev].positions =
                rom_loadPath(&size, 4, 0x13, group, part, morph->prev);
            if (gVtxMorphCache[group][part][morph->prev].positions == NULL) {
                heap_free(gVtxMorphCache[group][part][morph->cur].positions);
                gVtxMorphCache[group][part][morph->cur].positions = NULL;
                return -1;
            }
        }
        gVtxMorphCache[group][part][morph->cur].timer = 3;
        gVtxMorphCache[group][part][morph->prev].timer = 3;
        from = gVtxMorphCache[group][part][morph->prev].positions;
        to = gVtxMorphCache[group][part][morph->cur].positions;
        vtx = &vtxBase[gModelVtxIndices[id][part]];
        for (i = 0; i < config->partVtxCounts[part]; i++) {
            vtx[i].v.ob[0] = from[i][0] + ((f32)to[i][0] - from[i][0]) * morph->blend;
            vtx[i].v.ob[1] = from[i][1] + ((f32)to[i][1] - from[i][1]) * morph->blend;
            vtx[i].v.ob[2] = from[i][2] + ((f32)to[i][2] - from[i][2]) * morph->blend;
        }
        osWritebackDCache(vtx, config->partVtxCounts[part] * sizeof(Vtx));
    }
    if (morph->blend == 1.0f) {
        morph->hold--;
    } else {
        morph->hold = 3;
    }
    return 0;
}
