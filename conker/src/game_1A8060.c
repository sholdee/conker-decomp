#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Ages the cached morph keyframes and frees the ones that have not been used for a while. */
void func_1517ABB0(void) {
    s32 group;
    s32 part;
    s32 frame;

    for (group = 0; group < 2; group++) {
        if (D_800DD460[group] != NULL) {
            for (part = 0; part < D_800C4310[D_800A7230[group].kind]; part++) {
                for (frame = 0; frame < D_800A7230[group].frameCount; frame++) {
                    if (D_800DD460[group][part][frame].positions != NULL) {
                        if (D_800DD460[group][part][frame].timer != 0) {
                            D_800DD460[group][part][frame].timer--;
                        } else {
                            func_10004074(D_800DD460[group][part][frame].positions);
                            D_800DD460[group][part][frame].positions = NULL;
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
s32 func_1517AD00(s32 frame, s32 duration, s32 actorIndex) {
    VtxMorphState *morph;
    VtxMorphGroup *config;
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
    kind = func_150849A0(&movement_obj[actorIndex]);
    set = movement_obj[actorIndex].current_lod;
    config = NULL;
    for (i = 0; i < 2; i++) {
        if (kind == D_800A7230[i].kind) {
            config = &D_800A7230[i];
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
        if (movement_obj[actorIndex].modelData[set][D_800BE9C0] == NULL || D_800D19A0[id] == NULL) {
            if (frame == morph->cur) {
                if (D_800BEAC0 == 0) {
                    morph->blend += morph->rate * D_800BE9E4;
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
    if (D_800D19A0[id] == NULL) {
        return -1;
    }
    if (movement_obj[actorIndex].modelData[set][D_800BE9C0] == NULL) {
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
    if (D_800DD460[group] == NULL) {
        s32 failed;

        D_800DD460[group] = allocate_memory(D_800C4310[config->kind] * sizeof(VtxMorphFrame *), 1, 0, 2);
        if (D_800DD460[group] == NULL) {
            return -1;
        }
        failed = FALSE;
        for (i = 0; i < D_800C4310[config->kind]; i++) {
            D_800DD460[group][i] = allocate_memory(config->frameCount * sizeof(VtxMorphFrame), 1, 0, 2);
            if (D_800DD460[group][i] != NULL) {
                bzero(D_800DD460[group][i], config->frameCount * sizeof(VtxMorphFrame));
            } else {
                failed = TRUE;
            }
        }
        if (failed) {
            for (i = 0; i < D_800C4310[config->kind]; i++) {
                if (D_800DD460[group][i] != NULL) {
                    // matching: the s32 round trip changes IDO's register choice for the row pointer
                    func_10004074((void *)(s32)D_800DD460[group][i]);
                }
            }
            return -1;
        }
    }
    if (D_800BEAC0 == 0) {
        morph->blend += morph->rate * D_800BE9E4;
    }
    if (morph->blend > 1.0f) {
        morph->blend = 1.0f;
    }
    vtxBase = (Vtx *)movement_obj[actorIndex].modelData[set][D_800BE9C0];
    for (part = 0; part < D_800C4310[config->kind]; part++) {
        if (D_800DD460[group][part][morph->cur].positions == NULL) {
            D_800DD460[group][part][morph->cur].positions =
                func_1502B5C8(&size, 4, 0x13, group, part, morph->cur);
            if (D_800DD460[group][part][morph->cur].positions == NULL) {
                return -1;
            }
        }
        if (D_800DD460[group][part][morph->prev].positions == NULL) {
            D_800DD460[group][part][morph->prev].positions =
                func_1502B5C8(&size, 4, 0x13, group, part, morph->prev);
            if (D_800DD460[group][part][morph->prev].positions == NULL) {
                func_10004074(D_800DD460[group][part][morph->cur].positions);
                D_800DD460[group][part][morph->cur].positions = NULL;
                return -1;
            }
        }
        D_800DD460[group][part][morph->cur].timer = 3;
        D_800DD460[group][part][morph->prev].timer = 3;
        from = D_800DD460[group][part][morph->prev].positions;
        to = D_800DD460[group][part][morph->cur].positions;
        vtx = &vtxBase[D_800C4020[id][part]];
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
