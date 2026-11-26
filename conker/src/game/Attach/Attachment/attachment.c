#include <ultra64.h>
#include "functions.h"
#include "variables.h"

ModelAttachment *gAttachments; // head of the model attachment list



/* Loads (or reuses) attachment model `model` and returns its display lists and vertex data. */
s32 attach_loadModel(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *arg6) {
    AttachModelHeader *header;
    s32 i;

    if (gAttachModelRefCounts[model] == 0) {
        header = rom_loadPathTable(0, 0, 0, 2, 9, model);
        if (header == NULL) {
            return 1;
        }
        *dlists = header->dlists;
        *numDLists = header->dlistsSize >> 2;
        for (i = 0; i < *numDLists; i++) {
            (*dlists)[i] = (Gfx *)((u8 *)(*dlists)[i] + (s32)header);
            texture_resolveDl((*dlists)[i], 0, 1, 0x3E, &gAttachModelTexIds[model]);
            effect_relocateDisplayList((*dlists)[i], (s32)header);
        }
        gAttachModelHeaders[model] = header;
    } else {
        header = gAttachModelHeaders[model];
        *dlists = header->dlists;
        *numDLists = header->dlistsSize >> 2;
    }
    gAttachModelRefCounts[model]++;
    *data = header->data;
    if (arg4 != NULL) {
        *arg4 = header->unk10;
    }
    if (arg5 != NULL) {
        *arg5 = header->unk8;
        if (arg6 != NULL) {
            *arg6 = header->unkC >> 4;
        }
    }
    return 0;
}





/* Loads the attachment's model, making private per-frame vertex copies when the owner needs them. */
s32 attach_initModel(ModelAttachment *att, Actor *owner) {
    u8 *data;
    s32 mode;
    s32 size;

    if (att->dlists == NULL) {
        if (att->unk17 == -1) {
            if (attach_loadModel(att->model, &att->dlists, &att->numDLists, &data, &att->unk30, NULL, NULL) != 0) {
                return 1;
            }
        } else {
            if (anim_createInstance(att->model, att->unk17, &att->dlists, &att->numDLists, &data, &att->unk30, &att->anim) != 0) {
                return 1;
            }
        }

        mode = owner->unk66 & 0xC;
        size = (u8 *)att->dlists - data;
        if (!(att->flags & 0x10) && (mode == 8) && (att->unk30 != 0)) {
            att->vtx[0] = allocate_memory(size, 1, 2, 2);
            if (att->vtx[0] == NULL) {
                return 1;
            }

            att->vtx[1] = allocate_memory(size, 1, 2, 2);
            if (att->vtx[1] == NULL) {
                heap_free(att->vtx[0]);
                att->vtx[0] = NULL;
                return 1;
            }

            bcopy(data, att->vtx[0], size);
            bcopy(data, att->vtx[1], size);
            att->ownsVtx = 1;
        } else {
            att->vtx[0] = data;
            att->vtx[1] = data;
        }
        att->loaded = 1;
    }
    return 0;
}

/* Unlinks and frees an attachment; `immediate` frees its buffers now instead of deferring it. */
void attach_free(ModelAttachment *att, s32 immediate) {
    if (!immediate) {
        attach_undoOwnerEffects(att);
    }
    if (att->loaded) {
        if (att->ownsVtx) {
            if (immediate) {
                heap_free(att->vtx[0]);
                heap_free(att->vtx[1]);
            } else {
                heap_setTag(att->vtx[0], 4);
                heap_setTag(att->vtx[1], 4);
            }
        }
        if (att->anim != NULL) {
            anim_freeInstance(att->anim);
        }
        attach_releaseModel(att->model);
    }
    if (att->mtx != NULL) {
        if (immediate) {
            heap_free(att->mtx);
        } else {
            heap_setTag(att->mtx, 4);
        }
    }
    if (att->offset != NULL) {
        AttachmentOffset *offset = att->offset; // matching: IDO loads it into v0 only via a local

        heap_free(offset);
    }
    if (att->reflMtx[0] != NULL) {
        if (immediate) {
            heap_free(att->reflMtx[0]);
        } else {
            heap_setTag(att->reflMtx[0], 4);
        }
    }
    if (att->reflMtx[1] != NULL) {
        if (immediate) {
            heap_free(att->reflMtx[1]);
        } else {
            heap_setTag(att->reflMtx[1], 4);
        }
    }

    if (att == gAttachments) {
        gAttachments = att->next;
    }
    if (att->prev != NULL) {
        att->prev->next = att->next;
    }
    if (att->next != NULL) {
        att->next->prev = att->prev;
    }
    heap_free(att);
}


/* Removes every attachment of set `setId` from `owner`. */
void attach_removeSet(Actor *owner, s32 setId) {
    attach_removeFromSet(owner, setId, 0xFF);
}

/* Removes attachment number `index` of set `setId` from `owner` (all of them when index is 0xFF). */
s32 attach_removeFromSet(Actor *owner, s32 setId, s32 index) {
    ModelAttachment *att;
    s32 removed;

    if (owner->unique_id == 0) {
        return 0;
    }
    removed = 0;
    att = gAttachments;
    while (att != NULL) {
        ModelAttachment *next = att->next;

        if ((att->ownerId == owner->unique_id) && (att->setId == setId)) {
            if ((index != 0xFF) && (index != 0)) {
                index--;
                att = next;
                continue;
            } else {
                removed = 1;
                attach_free(att, 0);
                if (index == 0) {
                    break;
                }
            }
        }
        att = next;
    }
    return removed;
}

/* Removes every attachment of `owner`. */
s32 attach_removeAll(Actor *owner) {
    ModelAttachment *att;
    ModelAttachment *next;
    s32 removed;

    if (owner->unique_id == 0) {
        return 0;
    }

    att = gAttachments;
    removed = 0;
    while (att != NULL) {
        next = att->next;
        if (att->ownerId == owner->unique_id) {
            removed = 1;
            attach_free(att, 0);
        }
        att = next;
    }
    return removed;
}

/* Per-model hooks, indexed by ModelAttachment.handler; a nonzero return removes the attachment. */
typedef s32 (*AttachmentHook)(ModelAttachment *att, Actor *owner);
static AttachmentHook gAttachHooks[] = {
    NULL,
    attach_hookScrollTexH,
    attach_hookCycleTexture,
    attach_hookFrameTile,
    beam_hookFrameTexture,
    attach_hookHeldProp,
    attach_hookPlayOnce,
    func_15033328,
    attach_hookFlashingLight,
    attach_hookFollowAnim,
    attach_hookSpawnProjectile,
    func_15033440,
    func_150DFDD0,
    attach_scrollTile,
    func_1503378C,
    attach_hookToggleTexture,
    attach_hookStateEffect,
    func_150339C8,
    attach_hookDelayedSound,
    attach_hookScrollTexV,
    func_15033E00,
    beam_hookSlideTexture,
    func_150333A8,
    func_15033F0C,
    func_15033F70,
};

/* Per-frame update of every attachment: runs its hook, builds its matrix for this frame and removes
 * it when its owner is gone or it has expired. Attachments that follow another attachment's
 * matrix are updated in a second pass, after the ones they follow. */
void attach_updateAll(void) {
    ModelAttachment *att;
    ModelAttachment *next;
    Actor *owner;
    u16 timer;
    s32 ret;
    s32 i;
    s32 j;
    MtxF normMtx;
    MtxF offsetMtx;
    MtxF resultMtx;
    f32 (*boneMtx)[4];
    Mtx *mtx;
    s32 remove;
    MtxF boneBuf;
    s32 pass;
    s32 hasFollowers;
    f32 inv;
    Mtx *parentMtx;
    Mtx *parentBoneMtx;
    MtxF rotMtx;
    MtxF rotResultMtx;
    MtxF rotMtx2;
    MtxF rotResultMtx2;

    hasFollowers = FALSE;
    for (pass = 0; pass < 2; pass++) {
        if (pass == 1 && !hasFollowers) {
            break;
        }
        att = gAttachments;
        while (att != NULL) {
            next = att->next;
            ret = 0;
            if (pass == 0) {
                if (att->parentSet != 0) {
                    hasFollowers = TRUE;
                    att = next;
                    continue;
                }
            } else if (att->parentSet == 0) {
                att = next;
                continue;
            }
            owner = collision_findObjectById(att->ownerId);
            att->updated = TRUE;
            if (owner == NULL) {
                attach_free(att, 0);
                att = next;
                continue;
            }
            if (att->handler != 0) {
                ret = gAttachHooks[att->handler](att, owner);
            }
            if (ret != 0) {
                attach_free(att, 0);
                att = next;
                continue;
            }
            if (att->mtx != NULL || att->anim != NULL) {
                if (owner->ob_matrix_ptr != NULL) {
                    i = att->parentSet;
                    if (att->parentSet != 0) {
                        ret = (s32)attach_find(owner, i, 0); // matching: the parent pointer reuses ret
                        if (ret == 0) {
                            attach_free(att, 0);
                            att = next;
                            continue;
                        }
                        if (attach_getMatrices((ModelAttachment *)ret, owner, &parentMtx, &parentBoneMtx) == 0) {
                            attach_free(att, 0);
                            att = next;
                            continue;
                        }
                        boneMtx = boneBuf;
                        if (((ModelAttachment *)ret)->anim != NULL) {
                            parentMtx += att->parentMtx;
                            guMtxL2F(boneMtx, parentMtx);
                            mtx = (Mtx *)boneMtx; // matching: dead store (overwritten or unused below)
                        } else {
                            guMtxL2F(boneMtx, parentMtx);
                        }
                    } else {
                        boneMtx = owner->ob_matrix_ptr[att->bone];
                    }
                    if (att->mtx != NULL) {
                        mtx = att->mtx;
                        mtx += D_800BE9C0; // matching: not &att->mtx[D_800BE9C0], which IDO allocates differently
                        if (att->offset != NULL) {
                            /* bone rotation without its scale, then the offset */
                            for (i = 0; i < 3; i++) {
                                inv = 1.0f / sqrtf(boneMtx[i][0] * boneMtx[i][0] + boneMtx[i][1] * boneMtx[i][1] +
                                                   boneMtx[i][2] * boneMtx[i][2]);
                                for (j = 0; j < 3; j++) {
                                    normMtx[i][j] = boneMtx[i][j] * inv;
                                }
                                normMtx[i][3] = 0.0f;
                                normMtx[3][i] = boneMtx[3][i];
                            }
                            normMtx[3][3] = 1.0f;
                            math_mtxRotScaleF(offsetMtx, att->offset->rotX, att->offset->rotY, att->offset->rotZ,
                                          att->scale, att->scale, att->scale);
                            offsetMtx[3][0] = att->offset->x;
                            offsetMtx[3][1] = att->offset->y;
                            offsetMtx[3][2] = att->offset->z;
                            offsetMtx[0][3] = 0.0f;
                            offsetMtx[1][3] = 0.0f;
                            offsetMtx[2][3] = 0.0f;
                            offsetMtx[3][3] = 1.0f;
                            math_mtxMulF(offsetMtx, normMtx, resultMtx);
                            guMtxF2L(resultMtx, mtx);
                        } else if (att->unk16 == 4) {
                            /* bone matrix rescaled to att->scale */
                            for (i = 0; i < 3; i++) {
                                inv = sqrtf(boneMtx[i][0] * boneMtx[i][0] + boneMtx[i][1] * boneMtx[i][1] +
                                            boneMtx[i][2] * boneMtx[i][2]);
                                if (i == 1) { // matching: both arms are the same, but IDO emits the test
                                    inv = att->scale / inv;
                                } else {
                                    inv = att->scale / inv;
                                }
                                for (j = 0; j < 3; j++) {
                                    normMtx[i][j] = boneMtx[i][j] * inv;
                                }
                                normMtx[i][3] = 0.0f;
                                normMtx[3][i] = boneMtx[3][i];
                            }
                            normMtx[3][3] = 1.0f;
                            if (att->rotZ != 0) {
                                math_mtxRotateRPYF(rotMtx, 0.0f, 0.0f, att->rotZ * (360.0f / 65536.0f));
                                math_mtxMulF(rotMtx, normMtx, rotResultMtx);
                                guMtxF2L(rotResultMtx, mtx);
                            } else {
                                guMtxF2L(normMtx, mtx);
                            }
                        } else {
                            boneMtx[0][3] = 0.0f;
                            boneMtx[1][3] = 0.0f;
                            boneMtx[2][3] = 0.0f;
                            boneMtx[3][3] = 1.0f;
                            if (att->rotZ != 0) {
                                math_mtxRotateRPYF(rotMtx2, 0.0f, 0.0f, att->rotZ * (360.0f / 65536.0f));
                                math_mtxMulF(rotMtx2, boneMtx, rotResultMtx2);
                                guMtxF2L(rotResultMtx2, mtx);
                            } else {
                                guMtxF2L(boneMtx, mtx);
                            }
                        }
                    }
                } else {
                    boneMtx = NULL;
                    mtx = NULL;
                }
            }
            if (att->anim != NULL) {
                if (att->mtx != NULL) {
                    att->anim->matrices[D_800BE9C0] = (f32 (*)[4])mtx;
                } else {
                    att->anim->matrices[D_800BE9C0] = (f32 (*)[4])boneMtx;
                }
                anim_updateInstance(att->anim);
            }
            if (att->ownerStates[0] != 0) {
                remove = TRUE;
                for (i = 0; i < 3; i++) {
                    if (att->ownerStates[i] != 0 && owner->animseq.uh == att->ownerStates[i]) {
                        remove = FALSE;
                        break;
                    }
                }
                if (remove) {
                    attach_free(att, 0);
                    att = next;
                    continue;
                }
            }
            if ((timer = att->timer) != 0xFFFF) {
                if (timer != 0) {
                    if (timer > D_800BE9E4) {
                        att->timer = timer - D_800BE9E4;
                    } else {
                        att->timer = 0;
                    }
                } else {
                    attach_free(att, 0);
                }
            }
            att = next;
        }
    }
}


/* Attaches `model` to bone `bone` of `owner` as part of set `setId`. Returns NULL on failure, or
 * (unless flags & 8) when that model of that set is already attached. */
ModelAttachment *attach_add(Actor *owner, s32 model, s32 bone, s32 flags, s32 unk15, s32 unk16, s32 lifetime,
                          s32 setId, s32 handler, const u16 *ownerStates, s32 unk17, s32 parentSet, s32 parentMtx) {
    ModelAttachment *att;
    ModelAttachment *next;
    s32 err;

    if (owner->unique_id == 0) {
        return NULL;
    }
    if (!(flags & 8)) {
        att = gAttachments;
        if (att != NULL) {
            do {
                next = att->next;
                if ((att->ownerId == owner->unique_id) && (att->setId == setId) && (att->model == model)) {
                    return NULL;
                }
                att = next;
            } while (next != NULL);
        }
    }
    att = allocate_memory(sizeof(ModelAttachment), 1, 0, 2);
    if (att == NULL) {
        return NULL;
    }
    err = 0;
    if (gAttachments != NULL) {
        gAttachments->prev = att;
    }
    att->next = gAttachments;
    att->prev = NULL;
    gAttachments = att;
    att->ownerId = owner->unique_id;
    att->model = model;
    att->bone = bone;
    att->dlists = NULL;
    att->vtx[0] = NULL;
    att->vtx[1] = NULL;
    att->unk30 = 0;
    att->alpha = 0xFF;
    att->offset = NULL;
    att->flags = flags;
    att->setId = setId;
    att->scale = 1.0f;
    att->handler = handler;
    att->unk38 = 0;
    att->unk3C = 0;
    att->ownsVtx = 0;
    att->loaded = 0;
    att->seg6Id = 0;
    att->seg7Id = 0;
    att->ownerStates[0] = ownerStates[0];
    att->ownerStates[1] = ownerStates[1];
    att->ownerStates[2] = ownerStates[2];
    att->unk17 = unk17;
    att->anim = NULL;
    att->unk15 = unk15;
    att->unk16 = unk16;
    att->parentSet = parentSet;
    att->parentMtx = parentMtx;
    att->hiddenDLists = 0;
    att->reflMtx[0] = NULL;
    att->reflMtx[1] = NULL;
    att->updated = 0;
    att->rotZ = 0;
    if ((unk16 == 4) || (flags & 4)) {
        att->mtx = allocate_memory(2 * sizeof(Mtx), 1, 2, 2);
        if (att->mtx == NULL) {
            err = 1;
        }
    } else {
        att->mtx = NULL;
    }
    if (lifetime == 0) {
        att->timer = 0xFFFF;
    } else {
        att->timer = lifetime;
    }
    if (!err) {
        err |= attach_initModel(att, owner);
    }
    if (err) {
        attach_free(att, 1);
        return NULL;
    }
    attach_applyOwnerEffects(att, owner);
    return att;
}

/* Gives the attachment a fixed offset from its bone, and a matrix buffer to draw it with. */
void attach_setOffset(ModelAttachment *att, f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ) {
    att->offset = allocate_memory(sizeof(AttachmentOffset), 1, 0, 2);
    if (att->offset != NULL) {
        att->offset->x = x;
        att->offset->y = y;
        att->offset->z = z;
        att->offset->rotX = rotX;
        att->offset->rotY = rotY;
        att->offset->rotZ = rotZ;
        if (att->mtx == NULL) {
            att->mtx = allocate_memory(2 * sizeof(Mtx), 1, 2, 2);
        }
    }
}

/* Draws the attachments of every object visible in viewport `view`. `pass` 0 draws the opaque
 * ones, 1 the translucent ones, 2 those of objects whose unk5 is 7. */
Gfx *attach_drawView(Gfx *gdl, s32 view, s32 pass) {
    ModelAttachment *att;
    ModelAttachment *next;
    Actor *owner;
    s32 alpha;

    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        if (att->alpha == 0) {
            att = next;
            continue;
        }
        owner = collision_findObjectById(att->ownerId);
        if ((owner == NULL) || (owner->ob_matrix_ptr == NULL) || ((owner->unk74 & (1 << view)) == (1 << view))) {
            att = next;
            continue;
        }
        if (pass == 2) {
            if (owner->drawtype != 7) {
                att = next;
                continue;
            }
        } else {
            alpha = actor_getAlpha(owner, view);
            if (pass == 1) {
                if (!(att->flags & 2) && (alpha == 0xFF) && (owner->drawtype != 1)) {
                    att = next;
                    continue;
                }
            } else if ((att->flags & 2) || (alpha < 0xFF) || (owner->drawtype == 1)) {
                att = next;
                continue;
            }
        }
        gdl = attach_draw(gdl, att, owner, view, 0);
        att = next;
    }
    return gdl;
}

/* Draws the visible attachments of `owner` in viewport `view`. */
Gfx *attach_drawOwner(Gfx *gdl, Actor *owner, s32 view, s32 arg3) {
    ModelAttachment *att;
    ModelAttachment *next;

    if ((owner == NULL) || (owner->ob_matrix_ptr == NULL) || (owner->unique_id == 0)) {
        return gdl;
    }
    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        if ((att->ownerId != owner->unique_id) || (att->alpha == 0)) {
            att = next;
            continue;
        }
        gdl = attach_draw(gdl, att, owner, view, arg3);
        att = next;
    }
    return gdl;
}

/* Finds the matrix to draw `att` with this frame (*mtx) and the bone matrix it follows (*boneMtx).
 * Returns 0 when none is available yet. */
s32 attach_getMatrices(ModelAttachment *att, Actor *owner, Mtx **mtx, Mtx **boneMtx) {
    ModelAttachment *parent;

    if (att->anim != NULL) {
        if (att->anim->unk3F6 == 0) {
            return 0;
        }
        *mtx = att->anim->partMtx[D_800BE9C0];
        *boneMtx = (Mtx *)att->anim->matrices[D_800BE9C0];
    } else if (att->mtx != NULL) {
        *mtx = &att->mtx[D_800BE9C0];
        *boneMtx = (Mtx *)owner->ob_matrix_ptr + att->bone;
    } else if (att->parentSet) {
        parent = attach_find(owner, att->parentSet, 0);
        if (parent == NULL) {
            return 0;
        }
        if (attach_getMatrices(parent, owner, mtx, boneMtx) == 0) {
            return 0;
        }
        {
            s32 pad; // matching: as1 schedules the store after `li v0,1` only with this block
            *mtx += att->parentMtx;
        }
        return 1;
    } else {
        *mtx = (Mtx *)owner->ob_matrix_ptr;
        *mtx += att->bone;
        *boneMtx = *mtx;
    }
    return 1;
}


/* Draws one attachment of `owner` in viewport `view` with the owner's lighting, fog and alpha.
 * `arg4` nonzero draws it through model_drawAttachmentReflections instead of its own display lists. Returns the
 * original `gdl` if the display list would overflow. */
Gfx *attach_draw(Gfx *gdl, ModelAttachment *att, Actor *owner, s32 view, s32 arg4) {
    Mtx *mtx;
    Mtx *boneMtx;
    s32 segScratch[3];
    s32 fogOverride;
    s32 pad; // matching: unused stack slot
    s32 primR;
    s32 primG;
    s32 primB;
    s32 envR;
    s32 envG;
    s32 envB;
    s32 fogR;
    s32 fogG;
    s32 fogB;
    s32 fogA;
    s32 alpha;
    s32 light[5];
    u16 segId;
    Gfx *start;
    s32 v;
    s32 lit;
    s32 i; // matching: also the "collision_allocActorLights just ran" flag; a separate local mismatches

    start = gdl;
    if (!att->updated) {
        return gdl;
    }
    gDPPipeSync(gdl++);
    alpha = att->alpha;
    v = actor_getAlpha(owner, view);
    if (v != 0xFF) {
        alpha = (v * alpha) >> 8;
    }
    if (alpha < 0xFF) {
        gSPSegment(gdl++, 8, gModelXluRenderModes);
    } else {
        gSPSegment(gdl++, 8, gModelOpaRenderModes);
    }
    if (attach_getMatrices(att, owner, &mtx, &boneMtx) == 0) {
        return gdl;
    }
    func_1502D54C(owner - movement_obj, light);
    if (D_800B0DF0->unk3E != 0) {
        actor_getFogAlpha(&movement_obj[owner - movement_obj], light, view, owner - movement_obj);
    } else {
        light[3] = 0xFF;
    }
    actor_getDrawColours(owner, view, alpha, light, &primR, &primG, &primB, &envR, &envG, &envB, &fogR, &fogG, &fogB,
                  &fogA);
    lit = TRUE;
    fogOverride = D_800B0DF0->unk3E;
    if ((owner->unk66 & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, G_LIGHTING);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if (((owner->unk66 & 0xC) != 8) && !(att->flags & 1)) {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gdl++, G_LIGHTING | 0x400000);
    } else {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    }
    if (lit) {
        if (owner->unk304[view] == 0) {
            collision_allocActorLights(owner);
            i = TRUE;
        } else {
            i = FALSE;
        }
        if (owner->unk304[view] != 0) {
            if ((owner->unk2FD != 0) || i) {
                v = 0xA;
            } else {
                v = 2;
            }
            if ((owner->drawtype != 5) && (owner->unk302 != 0) && (owner->unk66 & 0xC) && !i) {
                gdl = light_reemitLights(gdl, owner->unk304[view], owner->unk301, owner->unk302,
                                    &owner->unk314[D_800BE9C0]);
            } else {
                gdl = light_pickLights(gdl, view, owner->xpos, owner->ypos, owner->zpos,
                                    owner->floorMaterial, owner->unk304[view], owner->unk301,
                                    &owner->unk314[D_800BE9C0], &owner->unk302, 2,
                                    owner->unk1DD, v, gPickedLights);
            }
        }
    }
    if (fogOverride == 0) {
        gDPSetFogColor(gdl++, fogR, fogG, fogB, fogA);
    } else {
        gDPSetFogColor(gdl++, gFogColour[0], gFogColour[1], gFogColour[2], fogA);
    }
    gDPSetFogColor(gdl++, fogR, fogG, fogB, fogA);
    gDPSetPrimColor(gdl++, 0xF2, 0, primR, primG, primB, 0);
    gDPSetEnvColor(gdl++, envR, envG, envB, alpha);
    gSPSegment(gdl++, 3, mtx);
    gSPSegment(gdl++, 1, att->vtx[D_800BE9C0]);
    segId = att->seg6Id;
    if (segId != 0) {
        // matching: the ids go through a pointer conversion; a plain int argument numbers IDO's temps differently
        v = texture_getAddress((s32)(void *)segId, segScratch, 0x3E, 0);
        gSPSegment(gdl++, 6, v);
    }
    if (att->seg7Id != 0) {
        v = texture_getAddress((s32)(void *)att->seg7Id, segScratch, 0x3E, 0);
        gSPSegment(gdl++, 7, v);
    }
    if (arg4 == 0) {
        gSPSetGeometryMode(gdl++, G_CULL_BACK);
        for (i = 0; i < att->numDLists; i++) {
            if (!(att->hiddenDLists & (1 << i))) {
                gSPDisplayList(gdl++, att->dlists[i]);
            }
        }
    } else {
        gSPSetGeometryMode(gdl++, G_CULL_FRONT);
        gdl = model_drawAttachmentReflections(gdl, att, envR, envG, envB);
    }
    if (D_800BEBA4 < (gdl - gDisplayListBuffers[D_800BE9C0])) {
        v = TRUE;
    } else {
        v = FALSE;
    }
    if (v) {
        return start;
    }
    return gdl;
}

/* Sets the alpha of attachment number `index` of set `setId` on `owner`. */
void attach_setAlpha(Actor *owner, s32 setId, s32 alpha, s32 index) {
    ModelAttachment *att;

    att = attach_find(owner, setId, index);
    if (att != NULL) {
        att->alpha = alpha;
    }
}

/* Returns attachment number `index` of set `setId` on `owner`, or NULL. */
ModelAttachment *attach_find(Actor *owner, s32 setId, s32 index) {
    s32 ownerId;
    ModelAttachment *att;
    ModelAttachment *next;

    ownerId = owner->unique_id;
    if (ownerId == 0) {
        return NULL;
    }

    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        if ((ownerId == att->ownerId) && (setId == att->setId)) {
            if (index != 0) {
                index--;
                att = next;
            } else {
                return att;
            }
        } else {
            att = next;
        }
    }
    return NULL;
}

/* Returns the first attachment of set `setId`, preferring one owned by `owner`. */
ModelAttachment *attach_findInSet(s32 setId, Actor *owner) {
    ModelAttachment *att;
    ModelAttachment *next;
    u8 ownerId;

    if (owner != NULL) {
        att = gAttachments;
        if (att != NULL) {
            ownerId = owner->unique_id;
            do {
                next = att->next;
                if ((att->ownerId == ownerId) && (att->setId == setId)) {
                    return att;
                }
                att = next;
            } while (att != NULL);
        }
    }
    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        if (att->setId == setId) {
            return att;
        }
        att = next;
    }
    return NULL;
}


/* Applies a model's side effects to its owner when it is attached (undone by attach_undoOwnerEffects). */
void attach_applyOwnerEffects(ModelAttachment *att, Actor *owner) {
    s32 model = att->model;
    ActorCtl *unk;

    switch (model) {
    case 0x37:
        effect_spawnOwnerEffect(owner);
        break;
    case 0x5A:
        unk = owner->mv;
        if (unk != NULL) {
            unk->unk1A6 += 0xAA;
        }
        break;
    case 0x90:
        owner->unk9C |= 0x70;
        break;
    case 0x8F:
        owner->unk9C |= 0xE00;
        break;
    case 0x49:
        light_createActorPulse(owner, 0xFF, 1);
        break;
    case 0x5D:
        trail_spawnCharacterTrail(owner, 0xFF, 1);
        particle_attachFollowEffect(owner, 0xFF, 1);
        break;
    case 0x3D:
        effect_spawnBoneBurst(owner, 0xFF, 1);
        break;
    case 0x1D:
        trail_attachToBone(owner, 0, 2, 0xFF, 1);
        break;
    case 0x5E:
    case 0x85:
        owner->unk9C |= 0x6000;
        break;
    case 0x8D:
        if (player_getStat(0, 6) < 100) {
            att->seg6Id = D_80090228;
        } else {
            att->seg6Id = D_8009022C;
        }
        break;
    case 0x82:
        trail_attachToBone(owner, 6, -1, 0xFF, 1);
        break;
    }
}


/* Undoes attach_applyOwnerEffects's side effects on the owner when an attachment is removed. */
void attach_undoOwnerEffects(ModelAttachment *att) {
    Actor *owner;
    ActorRef ref49;
    ActorRef ref5D;
    ActorCtl *unk;
    ActorRef *volatile ref; // matching: IDO spills the pointer across the call only when volatile

    owner = collision_findObjectById(att->ownerId);
    if (owner != NULL) {
        switch (att->model) {
        case 0x5A:
            unk = owner->mv;
            if (unk != NULL) {
                unk->unk1A6 -= 0xAA;
            }
            break;
        case 0x90:
            owner->unk9C &= ~0x70;
            break;
        case 0x8F:
            owner->unk9C &= ~0xE00;
            break;
        case 0x37:
        case 0x4B:
        case 0x4C:
            sound_stopByCallback(attach_soundCallback, att, (s32)owner);
            if (att->model == 0x37) {
                effect_sendOwnerGone(owner);
            }
            break;
        case 0x49:
            ref49.actor = owner;
            ref49.uniqueId = owner->unique_id;
            light_sendEvent(0x10, 0x29, &ref49);
            break;
        case 0x5D:
            ref5D.actor = owner;
            ref5D.uniqueId = owner->unique_id;
            effect_sendEventToEffectObjs((s32)(ref = &ref5D), 0x2E);
            effect_sendEventToTimedEffects((s32)ref, 0x2F);
            break;
        case 0x3D:
            effect_sendBurstOwnerGone(owner);
            break;
        case 0x1A:
        case 0x1B:
        case 0x5F:
        case 0x65:
        case 0x66:
            particle_detachObjEmitters(owner);
            break;
        case 0x1D:
        case 0x82:
            trail_broadcastActorGone(owner);
            break;
        case 0x5E:
        case 0x85:
            owner->unk9C &= ~0x6000;
            break;
        }
        switch (att->setId) {
        case 0x16:
        case 0x63:
        case 0x89:
            particle_stopObjEffect(owner);
            particle_detachObjEmitters(owner);
            break;
        }
    }
}

/* Hook: cycles seg6Id back and forth through the three D_800902BC entries, one step per call. */
s32 attach_hookCycleTexture(ModelAttachment *att, Actor *owner) {
    s32 step;

    step = att->unk38;
    if (step >= 3) {
        step = 5 - step;
    }
    att->seg6Id = D_800902BC[step];
    att->unk38++;
    if (att->unk38 >= 6) {
        att->unk38 = 0;
    }
    return 0;
}

/* First byte of a display list command, read signed as the game does. */
#define GFX_CMD(gfx) (*(s8 *)(gfx))

/* Hook: sets the tile t of the first display list's 4th G_SETTILESIZE from the owner's
 * animation frame (0..120 maps to 27..2). */
s32 attach_hookFrameTile(ModelAttachment *att, Actor *owner) {
    AnimInstance *anim;
    Gfx *gfx;
    s32 count;
    s32 i;
    f32 t;

    anim = owner->unk2D0;
    if (anim == NULL) {
        return 0;
    }

    gfx = att->dlists[0];
    if (gfx == NULL) {
        return 0;
    }

    if (owner->animseq.uh == 0x55) {
        t = 1.0f;
    } else if (owner->animseq.uh == 0x56) {
        t = 0.0f;
    } else if ((0.0f <= anim->frame[0]) && (anim->frame[0] <= 120.0f)) {
        t = anim->frame[0] * 0.008333333768f;
        t = 1.0f - t;
    } else {
        t = 0.0f;
    }

    i = 0;
    count = 4;
    do {
        count--;
        while (GFX_CMD(&gfx[i]) != (s8)G_SETTILESIZE) {
            i++;
        }
        if (count != 0) {
            i++;
        }
    } while (count != 0);

    gfx[i].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | ((s32)((25.0f * t) + 2.0f) & 0xFFF);
    return 0;
}


/* Hook for held/worn props: picks the prop's animation (and for some models its bone and parent)
 * from the owner's id and state, then keeps its frame in step with the owner's animation. */
s32 attach_hookHeldProp(ModelAttachment *att, Actor *owner) {
    AnimInstance *ownerAnim;
    s32 animId;
    u8 pad1; // matching: stack padding
    s32 state;
    AnimInstance *anim;
    s32 curAnimId;
    f32 lastFrame;
    s32 pad; // matching: stack padding
    s32 syncAll;

    ownerAnim = owner->unk2D0;
    animId = -1;
    anim = att->anim;
    if (anim == NULL) {
        return 0;
    }
    curAnimId = anim->anim[0] & ~0x8000;
    state = owner->animseq.uh;
    syncAll = TRUE;
    switch (owner->character_type) {
    case 0x58:
        if (att->model == 0x9B) {
            att->bone = 0x12;
        }
        switch (state) {
        case 39:
            animId = 3;
            break;
        case 40:
            animId = 4;
            break;
        case 2:
            animId = 1;
            break;
        case 3:
            animId = 0;
            break;
        default:
            animId = 0;
            if (att->model == 0x9B) {
                animId = 3;
            }
            break;
        }
        break;
    case 0x5B:
        animId = 0;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        if (owner->taskint[1] != 0) {
            att->seg6Id = D_800902D4;
        } else {
            att->seg6Id = D_800902D0;
        }
        switch (state) {
        case 6:
            animId = 7;
            break;
        case 38:
        case 39:
            animId = 0xF;
            break;
        case 1:
        case 42:
            animId = 3;
            break;
        case 0:
            animId = 5;
            break;
        case 3:
            animId = 6;
            break;
        case 7:
            animId = 2;
            break;
        case 10:
            animId = 4;
            break;
        case 15:
        case 30:
            animId = 8;
            break;
        case 19:
            animId = 0xD;
            break;
        case 4:
            animId = 9;
            break;
        case 5:
            animId = 0xA;
            break;
        case 16:
            animId = 0xB;
            break;
        case 8:
            animId = 0x10;
            break;
        case 20:
            animId = 0xE;
            break;
        case 21:
            animId = 0xE;
            break;
        case 0x50:
            animId = 0x13;
            break;
        case 0x51:
            animId = 0x14;
            break;
        default:
            animId = 6;
            break;
        }
        break;
    default:
        switch (att->model) {
        case 0x9E:
            att->bone = 9;
            att->parentSet = 0x41;
            att->parentMtx = 1;
            switch (state) {
            case 0x1D6:
                animId = 1;
                break;
            case 0x1D7:
                animId = 2;
                att->bone = 0x13;
                break;
            case 0x257:
                animId = 4;
                att->bone = 0x13;
                break;
            case 0x275:
                animId = 5;
                att->bone = 0x13;
                break;
            case 0x1D8:
                animId = 3;
                break;
            }
            syncAll = FALSE;
            if (att->bone == 0x13) {
                att->parentSet = 0;
            }
            break;
        case 0x9B:
            animId = 0;
            if (owner->drawtype == 5) {
                att->bone = 0;
            } else if (owner->character_type == 0x8B) {
                att->bone = 6;
                switch (state) {
                case 12:
                    animId = 6;
                    break;
                case 19:
                    animId = 7;
                    break;
                }
            } else {
                att->bone = 9;
                switch (state) {
                case 0x22E:
                    animId = 5;
                    break;
                case 0x1B7:
                    animId = 1;
                    break;
                case 0x1B8:
                    animId = 2;
                    break;
                }
            }
            break;
        case 0x9F:
            animId = 0;
            if (owner->drawtype == 5) {
                att->bone = 0;
            } else if (owner->character_type == 0xB5) {
                att->bone = 0x12;
                switch (state) {
                case 0x34:
                    animId = 1;
                    break;
                case 0x35:
                    animId = 2;
                    break;
                case 0x36:
                    animId = 3;
                    break;
                case 0x37:
                    animId = 4;
                    break;
                }
            } else {
                att->bone = 9;
            }
            break;
        case 0x9C:
            animId = 8;
            break;
        case 0x9D:
            animId = 0;
            if (owner->drawtype == 5) {
                att->bone = 0;
            } else if (owner->character_type == 0x8B) {
                att->bone = 6;
                switch (state) {
                case 19:
                case 32:
                    animId = 4;
                    break;
                case 34:
                    animId = 3;
                    break;
                }
            } else {
                att->bone = 9;
                switch (state) {
                case 0x21C:
                    animId = 1;
                    break;
                case 0x241:
                    animId = 2;
                    break;
                }
            }
            break;
        case 0x9A:
            switch (owner->character_type) {
            case 0x87:
                att->bone = 0x12;
                switch (state) {
                case 1:
                    animId = 1;
                    break;
                case 5:
                    animId = 2;
                    break;
                default:
                    animId = 0;
                    break;
                }
                break;
            case 0x99:
                att->bone = 0x15;
                switch (state) {
                case 20:
                    animId = 3;
                    break;
                case 21:
                    animId = 4;
                    break;
                case 22:
                    animId = 5;
                    break;
                case 23:
                    animId = 6;
                    break;
                case 24:
                    animId = 7;
                    break;
                case 26:
                    animId = 8;
                    break;
                case 27:
                    animId = 9;
                    break;
                case 28:
                    animId = 0xA;
                    break;
                case 29:
                    animId = 0xB;
                    break;
                case 30:
                    animId = 0xC;
                    break;
                case 31:
                    animId = 0xD;
                    break;
                case 32:
                    animId = 0xE;
                    break;
                case 33:
                    animId = 0xF;
                    break;
                case 34:
                    animId = 0x10;
                    break;
                case 35:
                    animId = 0x11;
                    break;
                default:
                    animId = 0;
                    break;
                }
                break;
            }
            break;
        case 0x8D:
            switch (state) {
            case 0x323:
                animId = 1;
                break;
            case 0x324:
                animId = 2;
                break;
            default:
                animId = 0;
                break;
            }
            break;
        case 0x8E:
            switch (state) {
            case 0xB4:
                animId = 0;
                break;
            case 0xB6:
                animId = 1;
                break;
            case 0xB5:
                animId = 2;
                break;
            case 0xDD:
                animId = 3;
                break;
            case 0xDE:
                animId = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x8F:
        case 0x90:
            switch (state) {
            case 0xB9:
                animId = 1;
                break;
            case 0xAE:
                animId = 2;
                break;
            case 0xDA:
                animId = 3;
                break;
            case 0x104:
                animId = 4;
                break;
            case 0x107:
                animId = 5;
                break;
            case 0x10B:
                animId = 6;
                break;
            case 0x11E:
                animId = 7;
                break;
            case 0x11F:
                animId = 8;
                break;
            case 0x134:
                animId = 9;
                break;
            case 0x135:
                animId = 0xA;
                break;
            case 0x136:
                animId = 0xB;
                break;
            case 0x13B:
                animId = 0xD;
                break;
            case 0x13C:
                animId = 0xE;
                break;
            case 0x145:
                animId = 0xF;
                break;
            case 0x146:
                animId = 0x10;
                break;
            case 0x141:
                animId = 0x11;
                break;
            case 0x147:
                animId = 0x12;
                break;
            case 0x14B:
                animId = 0x16;
                break;
            case 0x14A:
                animId = 0x15;
                break;
            case 0x149:
                animId = 0x14;
                break;
            case 0x148:
                animId = 0x13;
                break;
            case 0x151:
                animId = 0x18;
                break;
            case 0x17F:
                animId = 0x1A;
                break;
            case 0x180:
                animId = 0x1B;
                break;
            case 0x181:
                animId = 0x1C;
                break;
            case 0x182:
                animId = 0x1D;
                break;
            case 0x186:
                animId = 0x1E;
                break;
            case 0x187:
                animId = 0x1F;
                break;
            case 0x189:
                animId = 0x20;
                break;
            case 0x18C:
                animId = 0x21;
                break;
            case 0x18D:
                animId = 0x22;
                break;
            case 0x18E:
                animId = 0x23;
                break;
            case 0x1A0:
                animId = 0x24;
                break;
            case 0x1A3:
                animId = 0x25;
                break;
            case 0x1B3:
                animId = 0x26;
                break;
            case 0x1B4:
                animId = 0x27;
                break;
            case 0x1B5:
                animId = 0x28;
                break;
            case 0x1B6:
                animId = 0x29;
                break;
            case 0x1B7:
                animId = 0x2A;
                break;
            case 0x1B8:
                animId = 0x2B;
                break;
            case 0x1BD:
                animId = 0x2C;
                break;
            case 0x1BE:
                animId = 0x2D;
                break;
            case 0x1BF:
                animId = 0x2E;
                break;
            case 0x1C0:
                animId = 0x2F;
                break;
            case 0x1C1:
                animId = 0x30;
                break;
            case 0x1C2:
                animId = 0x31;
                break;
            case 0x1C3:
                animId = 0x32;
                break;
            case 0x1C4:
                animId = 0x33;
                break;
            case 0x1C5:
                animId = 0x34;
                break;
            case 0x1C6:
                animId = 0x35;
                break;
            case 0x1C7:
                animId = 0x36;
                break;
            case 0x1C8:
                animId = 0x37;
                break;
            case 0x1C9:
                animId = 0x38;
                break;
            case 0x1CA:
                animId = 0x39;
                break;
            case 0x1D1:
                animId = 0x3A;
                break;
            case 0x1D2:
                animId = 0x3B;
                break;
            case 0x1D3:
                animId = 0x3C;
                break;
            case 0x1D5:
                animId = 0x3D;
                break;
            case 0x1D9:
                animId = 0x3E;
                break;
            case 0x1DA:
                animId = 0x3F;
                break;
            case 0x1DB:
                animId = 0x40;
                break;
            case 0x1E3:
                animId = 0x41;
                break;
            case 0x1E4:
                animId = 0x42;
                break;
            case 0x1E5:
                animId = 0x43;
                break;
            case 0x1E6:
                animId = 0x44;
                break;
            case 0x1E7:
                animId = 0x45;
                break;
            case 0x1E8:
                animId = 0x47;
                break;
            case 0x1E9:
                animId = 0x44;
                break;
            case 0x1F0:
                animId = 0x48;
                break;
            case 0x1F1:
                animId = 0x49;
                break;
            case 0x1F2:
                animId = 0x4A;
                break;
            case 0x201:
                animId = 0x4B;
                break;
            case 0x202:
                animId = 0x4D;
                break;
            case 0x203:
                animId = 0x4E;
                break;
            case 0x204:
                animId = 0x4F;
                break;
            case 0x200:
                animId = 0x50;
                break;
            case 0x205:
                animId = 0x51;
                break;
            case 0x208:
                animId = 0x52;
                break;
            case 0x209:
                animId = 0x53;
                break;
            case 0x20A:
                animId = 0x54;
                break;
            case 0x20B:
                animId = 0x55;
                break;
            case 0x20C:
                animId = 0x56;
                break;
            case 0x211:
                animId = 0x57;
                break;
            case 0x212:
                animId = 0x58;
                break;
            case 0x216:
                animId = 0x59;
                break;
            case 0x217:
                animId = 0x5A;
                break;
            case 0x218:
                animId = 0x5B;
                break;
            case 0x219:
                animId = 0x5C;
                break;
            case 0x21A:
                animId = 0x5D;
                break;
            case 0x21B:
                animId = 0x5E;
                break;
            case 0xA8:
                animId = 0x5F;
                break;
            case 0x1FD:
                animId = 0x4C;
                break;
            case 0x228:
                animId = 0x62;
                break;
            case 0x22E:
                animId = 0x63;
                break;
            case 0x22F:
                animId = 0x64;
                break;
            case 0x232:
                animId = 0x61;
                break;
            case 0x220:
                animId = 0x60;
                break;
            case 0x23A:
                animId = 0x65;
                break;
            case 0x23B:
                animId = 0x66;
                break;
            case 0x24D:
                animId = 0x6E;
                break;
            case 0x21C:
                animId = 0x6F;
                break;
            case 0x241:
                animId = 0x70;
                break;
            case 0x23C:
                animId = 0x67;
                break;
            case 0x24B:
                animId = 0x6D;
                break;
            case 0x24A:
                animId = 0x6C;
                break;
            case 0x249:
                animId = 0x6B;
                break;
            case 0x248:
                animId = 0x6A;
                break;
            case 0x246:
                animId = 0x69;
                break;
            case 0x245:
                animId = 0x68;
                break;
            case 0x25C:
                animId = 0x71;
                break;
            case 0x25D:
                animId = 0x72;
                break;
            case 0x25E:
                animId = 0x73;
                break;
            case 0x260:
                animId = 0x74;
                break;
            case 0x266:
                animId = 0x77;
                break;
            case 0x267:
                animId = 0x78;
                break;
            case 0x268:
                animId = 0x79;
                break;
            case 0x26A:
                animId = 0x7A;
                break;
            case 0x26B:
                animId = 0x7B;
                break;
            case 0x26C:
                animId = 0x7C;
                break;
            case 0x26D:
                animId = 0x7D;
                break;
            case 0x275:
                animId = 0x7E;
                break;
            case 0x1D6:
                animId = 0x7F;
                break;
            case 0x1D7:
                animId = 0x86;
                break;
            case 0x1D8:
                animId = 0x85;
                break;
            case 0x276:
                animId = 0x80;
                break;
            case 0x277:
                animId = 0x81;
                break;
            case 0x278:
                animId = 0x82;
                break;
            case 0x265:
                animId = 0x76;
                break;
            case 0x27F:
                animId = 0x83;
                break;
            case 0x280:
                animId = 0x84;
                break;
            case 0x281:
                animId = 0x87;
                break;
            case 0x282:
                animId = 0x88;
                break;
            case 0x290:
                animId = 0x89;
                break;
            case 0x291:
                animId = 0x8A;
                break;
            case 0x292:
                animId = 0x8B;
                break;
            case 0x293:
                animId = 0x8C;
                break;
            case 0x294:
                animId = 0x8D;
                break;
            case 0x295:
                animId = 0x8E;
                break;
            case 0x298:
                animId = 0x8F;
                break;
            case 0x29A:
                animId = 0x90;
                break;
            case 0x29B:
                animId = 0x91;
                break;
            case 0x29C:
                animId = 0x92;
                break;
            case 0x29D:
                animId = 0x93;
                break;
            case 0x2A9:
                animId = 0x94;
                break;
            case 0x2AA:
                animId = 0x95;
                break;
            case 0x2AC:
                animId = 0x96;
                break;
            case 0x2AD:
                animId = 0x97;
                break;
            case 0x2AE:
                animId = 0x98;
                break;
            case 0x2AF:
                animId = 0x99;
                break;
            case 0x2B0:
                animId = 0x9A;
                break;
            case 0x2B2:
                animId = 0x9B;
                break;
            case 0x2B3:
                animId = 0x9C;
                break;
            case 0x2B4:
                animId = 0x9D;
                break;
            case 0x2B5:
                animId = 0x9E;
                break;
            case 0x2B6:
                animId = 0x9F;
                break;
            case 0x2B8:
                animId = 0xA0;
                break;
            case 0x2B9:
                animId = 0xA1;
                break;
            case 0x2BA:
                animId = 0xA2;
                break;
            case 0x2BB:
                animId = 0xA3;
                break;
            case 0x2BC:
                animId = 0xA4;
                break;
            case 0x2BD:
                animId = 0xA5;
                break;
            case 0x2BE:
                animId = 0xA6;
                break;
            case 0x2BF:
                animId = 0xA7;
                break;
            case 0x2C0:
                animId = 0xA8;
                break;
            case 0x2C1:
                animId = 0xA9;
                break;
            case 0x2C4:
                animId = 0xAA;
                break;
            case 0x2CF:
                animId = 0xAB;
                break;
            case 0x2DA:
                animId = 0xAC;
                break;
            case 0x2DB:
                animId = 0xAD;
                break;
            case 0x2DD:
                animId = 0xAE;
                break;
            case 0x2DC:
                animId = 0xAF;
                break;
            case 0x2D9:
                animId = 0xB0;
                break;
            case 0x2DF:
                animId = 0xB1;
                break;
            case 0x2E0:
                animId = 0xB2;
                break;
            case 0x2E1:
                animId = 0xB3;
                break;
            case 0x2E2:
                animId = 0xB4;
                break;
            case 0x2E3:
                animId = 0xB5;
                break;
            case 0x2EC:
                animId = 0xB6;
                break;
            case 0x2ED:
                animId = 0xB7;
                break;
            case 0x2EE:
                animId = 0xB8;
                break;
            case 0x2EF:
                animId = 0xB9;
                break;
            case 0x2F0:
                animId = 0xBA;
                break;
            case 0x2F1:
                animId = 0xBB;
                break;
            case 0x2F8:
                animId = 0xBC;
                break;
            case 0x2F9:
                animId = 0xBD;
                break;
            case 0x301:
                animId = 0xBE;
                break;
            case 0x304:
                animId = 0xBF;
                break;
            case 0x305:
                animId = 0xC0;
                break;
            case 0x306:
                animId = 0xC1;
                break;
            case 0x307:
                animId = 0xC2;
                break;
            case 0x308:
                animId = 0xC3;
                break;
            case 0x309:
                animId = 0xC4;
                break;
            case 0x30A:
                animId = 0xC5;
                break;
            case 0x30B:
                animId = 0xC6;
                break;
            case 0x30C:
                animId = 0xC7;
                break;
            case 0x30D:
                animId = 0xC8;
                break;
            case 0x30E:
                animId = 0xC9;
                break;
            case 0x30F:
                animId = 0xCA;
                break;
            case 0x314:
                animId = 0xCB;
                break;
            case 0x315:
                animId = 0xCC;
                break;
            case 0x316:
                animId = 0xCD;
                break;
            case 0x317:
                animId = 0xCE;
                break;
            case 0x318:
                animId = 0xCF;
                break;
            case 0x319:
                animId = 0xD0;
                break;
            case 0x31C:
                animId = 0xD1;
                break;
            case 0x31D:
                animId = 0xD2;
                break;
            case 0x31E:
                animId = 0xD3;
                break;
            case 0x320:
                animId = 0xD4;
                break;
            case 0x31F:
                animId = 0xD5;
                break;
            case 0x321:
                animId = 0xD6;
                break;
            case 0x322:
                animId = 0xD7;
                break;
            case 0x323:
                animId = 0xD8;
                break;
            case 0x324:
                animId = 0xD9;
                break;
            case 0x326:
                animId = 0xDA;
                break;
            case 0x327:
                animId = 0xDB;
                break;
            case 0x328:
                animId = 0xDC;
                break;
            case 0x32B:
                animId = 0xDD;
                break;
            case 0x32C:
                animId = 0xDE;
                break;
            case 0x32D:
                animId = 0xDF;
                break;
            case 0x32E:
                animId = 0xE0;
                break;
            case 0x32F:
                animId = 0xE1;
                break;
            case 0x330:
                animId = 0xE2;
                break;
            case 0x331:
                animId = 0xE3;
                break;
            case 0x332:
                animId = 0xE4;
                break;
            case 0x333:
                animId = 0xE5;
                break;
            case 0x334:
                animId = 0xE6;
                break;
            case 0x335:
                animId = 0xE7;
                break;
            case 0x345:
                animId = 0xE8;
                break;
            case 0x347:
                animId = 0xEA;
                break;
            case 0x348:
                animId = 0xEB;
                break;
            case 0x349:
                animId = 0xEC;
                break;
            case 0x34A:
                animId = 0xED;
                break;
            case 0x34B:
                animId = 0xEE;
                break;
            case 0x34C:
                animId = 0xEF;
                break;
            case 0x34D:
                animId = 0xF0;
                break;
            case 0x240:
                animId = 0xE9;
                break;
            default:
                animId = 0;
                break;
            }
            break;
        case 0x83:
            switch (state) {
            case 0x5E:
                animId = 0;
                break;
            case 0x5F:
            case 0xFA:
                animId = 1;
                break;
            case 0x60:
                animId = 2;
                break;
            case 0xB3:
                animId = 3;
                break;
            default:
                return 1;
            }
            break;
        case 0x85:
            if (state == 0x13E) {
                animId = 5;
            } else if (state == 0x13F) {
                animId = 4;
            } else if (state == 0x7C) {
                animId = 2;
            } else if (owner->speed > 0.0f) {
                animId = 1;
            } else {
                animId = 0;
            }
            break;
        case 0x88:
            switch (state) {
            default:
                animId = 2;
                break;
            case 0x11:
                animId = 0;
                break;
            case 0x26:
                animId = 1;
                break;
            }
            break;
        case 0x98:
            if (owner->character_type == 0x8B) {
                att->bone = 6;
                animId = 5;
                break;
            }
            switch (state) {
            case 0x12C:
                animId = 0;
                break;
            case 0x12D:
                animId = 3;
                break;
            case 0x142:
                animId = 4;
                break;
            case 0x12E:
            case 0x358:
                animId = 2;
                break;
            case 0x2AA:
                animId = 6;
                break;
            default:
                return 1;
            }
            break;
        case 0x87:
            switch (state) {
            case 0x172:
                animId = 0;
                break;
            case 0x174:
                animId = 2;
                break;
            case 0x17A:
                animId = 3;
                break;
            case 0x171:
            case 0x173:
                animId = 1;
                break;
            default:
                return 1;
            }
            break;
        case 0x89:
            if (owner->drawtype == 5) {
                animId = 3;
                att->bone = 0;
                break;
            }
            switch (state) {
            case 0x49:
                animId = 0;
                att->bone = 9;
                break;
            case 0x47:
                animId = 1;
                att->bone = 9;
                break;
            case 0x81:
                animId = 2;
                att->bone = 0x13;
                break;
            default:
                return 1;
            }
            break;
        case 0x91:
            if (owner->drawtype == 5) {
                att->bone = 0;
                if (owner->ygrav == 0.0f) {
                    animId = 3;
                } else {
                    animId = 0;
                }
            } else {
                att->bone = 0xE;
                switch (state) {
                default:
                    animId = 4;
                    break;
                case 0x12:
                    animId = 2;
                    break;
                case 0x10:
                    animId = 1;
                    break;
                }
            }
            break;
        }
        break;
    }
    if (animId != -1) {
        anim_play(att->anim, 0, animId, 0.0f, 0.0f, 1);
    }
    if (ownerAnim != NULL) {
        anim = att->anim;
        // matching: an if-block here makes IDO keep animId in s0; only the gotos match
        if (animId != curAnimId) {
            goto copy_frame;
        }
        if (ownerAnim->unk3C != 0x3FF) {
            goto copy_frame;
        }
        actor_saveBlendAnim(anim);
        anim = att->anim;
    copy_frame:
        anim->frame[0] = ownerAnim->frame[0];
        anim = att->anim;
        if (syncAll && (anim->nextFrameOffset[1] != 0 || anim->header[1][5] == 0)) {
            anim->unk3A = ownerAnim->unk3A;
            att->anim->unk3C = ownerAnim->unk3C;
            anim = att->anim;
        }
        lastFrame = anim->numFrames[0] - 1.0f;
        if (lastFrame <= anim->frame[0]) {
            anim->frame[0] = lastFrame;
        }
    }
    return 0;
}

/* Hook: plays the owner's taskint[0] animation on the attachment and keeps it in step with the owner's frame. */
s32 attach_hookFollowAnim(ModelAttachment *att, Actor *owner) {
    AnimInstance *ownerAnim;
    AnimInstance *anim;

    ownerAnim = owner->unk2D0;
    anim = att->anim;
    if (anim == NULL) {
        return 0;
    }
    if ((owner->taskint[0] & 0xFF) != 0xFF) {
        anim_play(anim, 0, owner->taskint[0] & 0xFF, 1.0f, 0.0f, 1);
    }
    if (ownerAnim != NULL) {
        att->anim->frame[0] = ownerAnim->frame[0];
        anim = att->anim;
        if (anim->numFrames[0] <= anim->frame[0]) {
            anim->frame[0] = anim->numFrames[0] - 1.0f;
        }
    }
    return 0;
}

/* Hook: plays animation 0 on the attachment once; removes the attachment when it has finished. */
s32 attach_hookPlayOnce(ModelAttachment *att, Actor *owner) {
    AnimInstance *anim;

    anim = att->anim;
    if (anim == NULL) {
        return 0;
    }
    if ((anim->anim[0] & 0x8000) != 0x8000) {
        anim_play(anim, 0, 0, 1.0f, 0.0f, 1);
        anim = att->anim;
    }
    if (anim->numFrames[0] - 1.0f <= anim->frame[0]) {
        return 1;
    }
    return 0;
}

/* Hook: removes the attachment after 30 ticks with its owner out of water and waterHeight < unk180. */
s32 func_15033328(ModelAttachment *att, Actor *owner) {
    s32 delta;

    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    if (owner->inwater == 0) {
        if (owner->waterHeight < owner->floorY) {
            delta = D_800BE9E4;
            if (delta < att->unk38) {
                att->unk38 -= delta;
                return 0;
            } else {
                return 1;
            }
        }
    }
    att->unk38 = 30;
    return 0;
}

/* Hook: hides the attachment while the owner is below waterHeight + 300; removes it in water. */
s32 func_150333A8(ModelAttachment *att, Actor *owner) {
    ActorCtl *unk;

    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    if (owner->inwater != 0) {
        unk = owner->mv;
        if (unk != NULL) {
            unk->weapon_status = 0;
        }
        return 1;
    }
    if ((owner->waterHeight != -10000.0f) && (owner->ypos < owner->waterHeight + 300.0f)) {
        att->alpha = 0;
    } else {
        att->alpha = 0xFF;
    }
    return 0;
}

/* Hook: while the owner's unk5 is 5, moves models 0x27/0x35/0x29 to bone 0 (spinning 0x27/0x35). */
s32 func_15033440(ModelAttachment *att, Actor *owner) {
    switch (att->model) {
    case 0x27:
    case 0x35:
        if (owner->drawtype == 5) {
            att->bone = 0;
            att->rotZ += D_800BE9E4 * 0xAAA;
        }
        break;
    case 0x29:
        if (owner->drawtype == 5) {
            att->bone = 0;
        }
        break;
    }
    return 0;
}

/* Scrolls the tile of model 0x37's 4th G_SETTILESIZE, wrapping s and t within the tile size. */
s32 attach_scrollTile(ModelAttachment *att, Actor *owner) {
    Gfx *gfx;
    s32 count;
    s32 i;
    s32 w0;
    s32 w1;
    s32 width;
    s32 s;
    s32 t;
    s32 height;
    s32 offset;

    gfx = NULL;
    count = 0;
    offset = 0;
    if (att->model == 0x37) {
        gfx = att->dlists[0];
        count = 4;
        offset = -100;
    }
    if (gfx != NULL) {
        i = 0;
        while (count != 0) {
            count--;
            while (GFX_CMD(&gfx[i]) != (s8)G_SETTILESIZE) {
                i++;
            }
            if (count != 0) {
                i++;
            }
        }

        w0 = gfx[i].words.w0;
        w1 = gfx[i].words.w1;
        width = ((w1 >> 12) & 0xFFF) + 2;
        s = ((w0 >> 12) & 0xFFF) + offset;
        t = w0 & 0xFFF;
        if (s >= width) {
            s -= width;
        }
        if (s < 0) {
            s += width;
        }
        height = (w1 & 0xFFF) + 2;
        if (t >= height) {
            t -= height;
        }
        if (t < 0) {
            t += height;
        }
        gfx[i].words.w0 = (G_SETTILESIZE << 24) | ((s & 0xFFF) << 12) | (t & 0xFFF);
    }
    return 0;
}

/* Attaches set `setId` to bone `bone` of `target`, with the offset that keeps it where `obj` is now
 * (only rotation when `noTranslation`). */
ModelAttachment *attach_attachInPlace(Actor *obj, Actor *target, s32 setId, s32 bone, s32 arg4, s32 noTranslation) {
    ModelAttachment *att;
    MtxF boneMtx;
    MtxF invBoneMtx;
    MtxF objMtx;
    MtxF relMtx;
    f32 x;
    f32 y;
    f32 z;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 scale;

    if (target->ob_matrix_ptr == NULL) {
        return NULL;
    }

    att = collision_createPartSet(target, setId, 1.0f, 0);
    if (att != NULL) {
        att->bone = bone;
        att->scale = obj->scale_xz;

        if (arg4 != 0) {
            att->unk16 &= ~4;
        } else {
            att->unk16 |= 4;
        }

        guMtxL2F(boneMtx, (Mtx *)target->ob_matrix_ptr + bone);
        math_mtxRigidInverse(boneMtx, invBoneMtx);

        scale = obj->scale_xz;
        math_mtxRotScaleF(objMtx, obj->vtilt, obj->theta, obj->lean_angle, scale, obj->scale_y, scale);

        objMtx[3][0] = obj->xpos;
        objMtx[3][1] = obj->ypos;
        objMtx[3][2] = obj->zpos;
        objMtx[0][3] = 0.0f;
        objMtx[1][3] = 0.0f;
        objMtx[2][3] = 0.0f;
        objMtx[3][3] = 1.0f;

        math_mtxMulF(objMtx, invBoneMtx, relMtx);
        model_decomposeMatrix(relMtx, &x, &y, &z, &rotX, &rotY, &rotZ, &scaleX, &scaleY, &scaleZ);

        if (noTranslation) {
            z = 0.0f;
            y = 0.0f;
            x = 0.0f;
        }

        attach_setOffset(att, x, y, z, rotX, rotY, rotZ);
        return att;
    }

    return NULL;
}

/* Hook: model 0x11 is removed unless its owner is in one of a few states. */
s32 func_1503378C(ModelAttachment *att, Actor *owner) {
    u16 state = owner->animseq.uh;

    if (att->model == 0x11) {
        if ((state == 0x3E) || (state == 0x3D) || (state == 0x41) || (state == 0xD9) || (state == 0x138) ||
            (state == 0x139)) {
            return 0;
        }
    }
    return 1;
}


/* Hook: toggles seg6Id between the two D_800902FC entries every 16 ticks. */
s32 attach_hookToggleTexture(ModelAttachment *att, Actor *owner) {
    att->unk38 += D_800BE9E4;
    if (att->unk38 >= 16) {
        att->unk38 = 0;
        att->unk3C ^= 1;
    }
    att->seg6Id = D_800902FC[att->unk3C];
    return 0;
}

/* Hook: starts particle_startObjRayBeam effect `kind` while the owner is in the set's trigger state (and
 * mv allows it), and stops it with particle_stopObjEffect when that no longer holds. */
s32 attach_hookStateEffect(ModelAttachment *att, Actor *owner) {
    u8 kind;
    s32 triggered;
    ActorCtl *unk;

    triggered = 0;
    if (att->setId == 0x16) {
        kind = 4;
        if (owner->animseq.uh == 0x165) {
            triggered = 1;
        }
    } else if (att->setId == 0x89) {
        kind = 6;
        switch (owner->animseq.uh) {
        case 0x221:
        case 0x223:
        case 0x31B:
            if (owner->mv->AimState == 2) {
                triggered = 1;
            }
            break;
        }
    } else {
        kind = 5;
        if (owner->animseq.uh == 0x157) {
            triggered = 1;
        }
    }
    if (att->unk38 == 0) {
        if (triggered) {
            unk = owner->mv;
            if ((unk->SniperState != 0) || (kind == 6)) {
                if ((unk->buttons & 0x2000) == 0x2000) {
                    if (unk->ReloadTimer == 0) {
                        att->unk38 = 1;
                        particle_startObjRayBeam(owner, -1, kind, 1, 0xFF, 1);
                    }
                }
            }
        }
    } else if (!triggered || (unk = owner->mv, (unk->buttons & 0x2000) == 0) ||
               ((unk->SniperState == 0) && (kind != 6))) {
        att->unk38 = 0;
        particle_stopObjEffect(owner);
    }
    return 0;
}

/* Hook (model 0x37): scrolls the texture and keeps looping sound 0x448 playing at the owner
 * unless gFrameRateDivider is set. */
s32 func_150339C8(ModelAttachment *att, Actor *owner) {
    attach_scrollTile(att, owner);
    if (gCutsceneStates[0] != 1) {
        if (gFrameRateDivider != 0) {
            if (att->unk3C != 0) {
                sound_stopByCallback(attach_soundCallback, att, (s32)owner);
            }
            att->unk3C = 0;
        } else if (att->unk3C == 0) {
            att->unk3C = sound_addDynamicSound(0x448, owner->xpos, owner->ypos, owner->zpos, 32000, 1000,
                                       500, attach_soundCallback, att, (s32)owner, 0, 0) |
                         0x80000000;
        }
    }
    return 0;
}

/* Hook: after a 30-tick delay, starts sound 0x513 at the owner (attach_soundCallback follows it up). */
s32 attach_hookDelayedSound(ModelAttachment *att, Actor *owner) {
    if (gIsMultiplayer != 0) {
        mp_addZone(owner->xpos, owner->ypos, owner->zpos, 900.0f);
    }
    if (att->unk38 == 0) {
        if (att->unk3C < 30) {
            att->unk3C += D_800BE9E4;
        } else {
            att->unk3C = sound_addDynamicSound(0x513, owner->xpos, owner->ypos, owner->zpos, 32000, 1000,
                                       500, attach_soundCallback, att, (s32)owner, 0, 0);
            att->unk38 = 0x513;
        }
    }
    return 0;
}

/* Sound record callback of the sounds the attachment hooks start (sound_addDynamicSound with the attachment
 * and its owner as the parameters): tracks the owner's position and plays the model's follow-up
 * sounds. Returns 1 when the sound should stop. */
s32 attach_soundCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound) {
    ModelAttachment *att;
    Actor *owner;
    ActorCtl *unk;

    att = record->callback_param1.ptr;
    owner = record->callback_param2.obj;
    if ((att != NULL) && (owner != NULL) && (owner->type != 0)) {
        record->x = owner->xpos;
        record->y = owner->ypos;
        record->z = owner->zpos;

        if (*volume != 0) {
            if (att->model == 0x37) {
                s32 lastState = att->unk38;
                s32 sfxId = -1;

                if (owner->animseq.uh != (lastState & 0xFFFF)) {
                    if (owner->animseq.uh == 0x15F) {
                        sfxId = (math_random() & 3) + 0x444;
                    }
                }
                if (sfxId != -1) {
                    sound_playOnActor(0, sfxId, 24000, 0, 0, owner);
                }
                att->unk38 = owner->animseq.uh;
            } else {
                unk = owner->mv;
                if (unk != NULL) {
                    if (unk->GrenadeTimer < 120) {
                        if (att->unk38 == 0x513) {
                            att->unk38 = 0x3A1;
                            att->unk3C = sound_addDynamicSound(0x3A1, owner->xpos, owner->ypos,
                                                       owner->zpos, 32000, 1000, 500, attach_soundCallback, att,
                                                       (s32)owner, 0, 0);
                            return 1;
                        }
                    }
                }
            }
            return 0;
        }

        if (att->model == 0x37) {
            if (record->sound_id != 0) {
                sound_stop(record->sound_id);
                record->sound_id = 0;
            }
            *sound = 0;
            return 0;
        }
    }
    return 1;
}

/* Hook: removes the attachment once its owner's unk5 becomes 3. */
s32 func_15033E00(ModelAttachment *att, Actor *owner) {
    if (owner->drawtype == 3) {
        return 1;
    }
    return 0;
}

/* Stores every attachment of `owner` into `out` and returns how many there are. */
s32 attach_getAll(Actor *owner, ModelAttachment **out) {
    ModelAttachment *head;
    ModelAttachment *att;
    ModelAttachment *next;
    s32 count;

    head = gAttachments;
    count = 0;
    if (head == NULL) {
        return 0;
    }
    att = head; // matching: the second pointer moves the walk from v0 into a0
    while (att != NULL) {
        next = att->next;
        if (att->ownerId == owner->unique_id) {
            out[count] = att;
            count++;
        }
        att = next;
    }
    return count;
}

/* Returns the first attachment of `owner`, or NULL. */
ModelAttachment *attach_getFirst(Actor *owner) {
    ModelAttachment *att;
    ModelAttachment *next;
    u8 ownerId;

    att = gAttachments;
    if (att != NULL) {
        ownerId = owner->unique_id;
        do {
            next = att->next;
            if (att->ownerId == ownerId) {
                return att;
            }
            att = next;
        } while (att != NULL);
    }
    return NULL;
}

/* Swaps the owners of all attachments of owner ids `a` and `b`. */
void attach_swapOwners(s32 a, s32 b) {
    ModelAttachment *att;
    ModelAttachment *next;
    s32 idA;
    s32 ownerId;

    idA = a; // matching: IDO keeps the copy in a2
    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        ownerId = att->ownerId;
        if (idA == ownerId) {
            att->ownerId = b;
        } else if (b == ownerId) {
            att->ownerId = idA;
        }
        att = next;
    }
}

/* Hook: removes the attachment (clearing mv->weapon_status) unless mv->rt_context_b is 9 or weapon_status is 3. */
s32 func_15033F0C(ModelAttachment *att, Actor *owner) {
    ActorCtl *unk;

    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    unk = owner->mv;
    if (unk != NULL) {
        if (unk->rt_context_b != 9) {
            if (unk->weapon_status != 3) {
                unk->weapon_status = 0;
                return 1;
            }
        }
    }
    return 0;
}

/* Hook: like func_15033F0C, but keeps the attachment while mv->rt_context_b is 0xC or 0x16. */
s32 func_15033F70(ModelAttachment *att, Actor *owner) {
    ActorCtl *unk;

    if (gCutsceneStates[0] == 1) {
        return 0;
    }
    unk = owner->mv;
    if (unk != NULL) {
        if ((unk->rt_context_b != 0xC) && (unk->rt_context_b != 0x16)) {
            if (unk->weapon_status != 3) {
                unk->weapon_status = 0;
                return 1;
            }
        }
    }
    return 0;
}
