#include <ultra64.h>

/*
 * .bss, linked at 0x800C3E70..0x800C3EE0 in this order. Defined before conker/actor.h declares them:
 * IDO lays out .bss in first-declaration order, and an extern there would count.
 */
static u8 D_800C3E70;  // cleared at the end of actor_updateObjects
static s32 gStoodOnActors; // a bit for each character that some object stands on
u8 dc_glbl_CharNum;
u16 gBoneMatricesUsed;
u16 gBonePoolSize;
u8 *gBonePools[2];
u8 *gBonePoolCursor;
u8 *gBonePoolStart;
u8 gBoneMatricesFlushed;
static Mtx gObjectIdentityMtx; // the modelview matrix of the object being drawn

#include <math.h>
#include "conker/actor.h"
#include "conker/aim.h"
#include "conker/anim.h"
#include "conker/attach.h"
#include "conker/bone.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/env.h"
#include "conker/event.h"
#include "conker/face.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/model.h"
#include "conker/particle.h"
#include "conker/player.h"
#include "conker/rom.h"
#include "conker/screen.h"
#include "conker/shadow.h"
#include "conker/sprite.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/zone.h"

Gfx gObjectPassBeginDL[] = {
    gsDPPipeSync(),
    gsSPMatrix(OS_K0_TO_PHYSICAL(&gIdentityMtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPSetGeometryMode(G_SHADE | G_LIGHTING | G_SHADING_SMOOTH | 0x400000),
    gsSPClearGeometryMode(G_FOG | G_LOD),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx gObjectPassEndDL[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_LOD | 0x400000),
    gsDPSetEnvColor(0, 0, 0, 0),
    gsDPSetPrimColor(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};

/* This file */

/* Other files */

/* Emits the display list of every object drawn in one render pass (mode) for one player. */
Gfx *actor_drawPass(Gfx *gdl, s32 mode, s16 player) {
    Actor *actor;
    s32 i;
    s32 state;

    gSPDisplayList(gdl++, gObjectPassBeginDL);
    actor = movement_obj;
    for (i = 0; i != 25; i++, actor++) {
        gDebugStatusCode = (i & 0xFFFFFF) | 0x01000000;
        state = actor->type;
        if (state == 0 || actor->drawtype == 3 || actor->drawtype == 5) {
            continue;
        }
        if (actor->drawtype == 2) {
            if (mode != 2) {
                continue;
            }
        } else if (actor->character_type == 0xFF) {
            continue;
        }
        if (mode == 6) {
            if (actor->drawtype != 7) {
                continue;
            }
        } else if (mode == 0) {
            if (!((actor->floorMaterial >> 9) & 1)) {
                continue;
            }
        } else if (mode == 1) {
            if (actor->drawtype == 7 || actor->drawtype == 1) {
                continue;
            }
            if (actor->drawtype == 0 && actor_getAlpha(actor, player) < 0xFF) {
                continue;
            }
        } else if (mode == 2 && actor->drawtype != 2) {
            if (actor->drawtype == 7 || (actor->drawtype != 0 && actor->drawtype != 1)) {
                continue;
            }
            if (actor->drawtype == 0 && actor_getAlpha(actor, player) == 0xFF) {
                continue;
            }
        }
        if (actor->drawtype == 2) {
            gdl = actor_drawSprite(gdl, i);
        } else {
            gdl = actor_drawModel(gdl, i, player, mode, 0);
            if (i == 0) {
                gdl = model_drawActorQuad(gdl, i, player);
            }
        }
    }
    gDebugStatusCode = 0x01FFFFFF;
    if (mode == 1) {
        gdl = attach_drawView(gdl, player, 0);
    } else if (mode == 2) {
        gdl = attach_drawView(gdl, player, 1);
    } else if (mode == 6) {
        gdl = attach_drawView(gdl, player, 2);
    }
    gSPDisplayList(gdl++, gObjectPassEndDL);
    gDebugStatusCode = 0;
    return gdl;
}

/* Per-frame update of one object: LOD, culling, bone matrices, fog effect and attachments. */
void actor_updateObject(Actor *actor, s32 index) {
    actor->ob_matrix_ptr = NULL;
    if (actor->drawtype == 5) {
        actor_buildRootMatrix(index, TRUE);
        return;
    }
    if (actor->character_type == 0xFF) {
        return;
    }
    if (actor->drawtype == 3) {
        return;
    }
    if (actor->drawtype == 2) {
        actor_scrollSpriteTexture(index);
        return;
    }
    if (actor->forced_lod != 0) {
        actor_applyForcedLod(actor);
    }
    actor_updateCulling(index);
    actor_buildRootMatrix(index, FALSE);
    aim_updatePoints(actor);
    if (actor->ob_matrix_ptr == NULL) {
        actor->unk1FC = 2;
    } else {
        bone_updateObject(index);
        aim_turnHead(actor);
    }
    if (gObjPartsStates[index].state != 0) {
        model_updateParts(index);
    }
    if (actor->type != 0) {
        actor_updateEyes(index);
        actor_followParent(index);
        if (actor->fogMode != 0) {
            actor_updateFog(actor);
        }
        if (actor->noshove & 0x4000) {
            collision_updateActorHitMesh(actor);
        }
        if (actor->morph != NULL) {
            anim_morphVertices(actor->morphTarget, actor->morphDuration, index);
        }
    }
}

/* Updates every object, parents before the objects attached to them (by attachment depth). */
void actor_updateObjects(void) {
    Actor *actor;
    Actor *parent;
    s32 i;
    s32 j;
    s32 state;
    u8 order[25];
    u8 depth[25];
    s32 count;
    s32 maxDepth;

    zone_advanceObjectCursor();
    maxDepth = 0;
    gBoneMatricesFlushed = 0;
    gStoodOnActors = 0;
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].stand_on_char != 0) {
            gStoodOnActors |= 1 << (movement_obj[i].stand_on_char - 1);
        }
    }
    bzero(depth, 25);
    for (i = 0; i < 25; i++) {
        actor = &movement_obj[i];
        state = actor->type;
        if (state != 0) {
            if (actor->attach_char != 0) {
                parent = actor;
                depth[i] = 0;
                while (parent->attach_char != 0) {
                    parent = &movement_obj[parent->attach_char - 1];
                    depth[i]++;
                }
                if (maxDepth < depth[i]) {
                    maxDepth = depth[i];
                }
            } else {
                actor_updateObject(actor, i);
            }
        }
    }
    count = 0;
    if (maxDepth != 0) {
        for (i = 1; i <= maxDepth; i++) {
            for (j = 0; j < 25; j++) {
                if (i == depth[j]) {
                    order[count++] = j;
                }
            }
        }
        for (i = 0; i < count; i++) {
            actor_updateObject(&movement_obj[order[i]], order[i]);
        }
    }
    actor_carryStandingObjects();
    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type != 0) {
            actor_savePrevBones(&movement_obj[i]);
        }
    }
    attach_updateAll();
    if (gPaused == 0) {
        anim_advanceObjects(0);
    }
    D_800C3E70 = 0;
}

/* Creates the blobShadow effect for objects visible to some player and frees it for the rest. */
void actor_updateBlobShadows(void) {
    Actor *actor;
    s32 visible;
    s32 i;
    s32 variant;
    s32 variantFlag;
    s32 id;
    s32 state;
    EffectHeader *effect;
    s32 j;

    for (j = 0; j < 25; j++) {
        actor = &movement_obj[j];
        state = actor->type;
        if (state != 0) {
            id = actor->character_type;
            visible = FALSE;
            if (actor->drawtype != 3 && id != 0xFF && actor->noBlobShadow == 0 && (actor->unk66 & 0x10) != 0x10 &&
                actor->shadow_size != 0 && actor->attach_char == 0 &&
                (gCutsceneActorLock == 0 || gCutsceneShowUnboundActors[0] != 0 || cutscene_isActorBound(actor) != 0) &&
                (actor->unk88 == 0 || actor->heightOffset >= -150)) {
                for (i = 0; i <= gScreenLayout; i++) {
                    if (actor_getAlpha(actor, i) != 0) {
                        visible = TRUE;
                    }
                }
            }
            if (visible) {
                if (actor->blobShadow == NULL) {
                    variant = 0;
                    if (actor->character_type == 0x4D) {
                        variant = 2;
                        variantFlag = 1;
                    } else {
                        variantFlag = 0;
                    }
                    actor->blobShadow = (EffectHeader *)shadow_createBlob(0, actor, variant, 0, variantFlag);
                    // matching: the label ends the block so as1 can't move the store into the branch delay slot
                created:;
                }
            } else {
                effect = actor->blobShadow;
                if (effect != NULL) {
                    effect_destroyObject(effect);
                    actor->blobShadow = NULL;
                }
            }
        }
    }
}

/* Resets this frame buffer's bone-matrix pool. */
void actor_resetBonePool(void) {
    gBonePoolStart = gBonePoolCursor = gBonePools[gDrawBufferIndex];
    gBoneMatricesUsed = 0;
}

/* Texture index of a sprite object (out-of-range values fall back to 11). */
s32 actor_getSpriteTexture(s32 index) {
    s32 tex = movement_obj[index].spriteTexture;

    if (tex >= 70) {
        tex = 11;
    }
    return tex;
}

/* Draws a kind-2 object as a textured sprite. */
Gfx *actor_drawSprite(Gfx *gdl, s32 index) {
    SpriteParams sprite;
    s32 w;
    s32 h;
    s32 tex;
    s16 pad2; // matching: an unused stack slot that fixes the frame layout
    s16 scroll;
    s32 alpha;
    u8 size;

    if (gPaused != 0) {
        return gdl;
    }
    alpha = movement_obj[index].alpha;
    size = movement_obj[index].spriteSize;
    scroll = movement_obj[index].texScroll;
    if (alpha == 0) {
        return gdl;
    }
    h = w = size * 19;
    if (movement_obj[index].type == 3) {
        h = w = size * 15;
    }
    tex = actor_getSpriteTexture(index);
    gSPDisplayList(gdl++, gSpriteRenderDl);
    gDPPipeSync(gdl++);
    if (gdl != NULL) { // matching: the empty test changes the register choice of the spilled temps
    }
    gdl = sprite_loadDescFrame(gdl, gEffectAnimTextures[tex], scroll, &sprite, 0, 0, 0, 2, 3);
    sprite.flip = 0;
    sprite.alpha = alpha;
    sprite.scaleX = w;
    sprite.scaleY = h;
    sprite.x = movement_obj[index].xpos;
    sprite.y = movement_obj[index].ypos + (size / 2);
    sprite.z = movement_obj[index].zpos;
    gdl = sprite_drawWorld(gdl, &sprite);
    gSPDisplayList(gdl++, gSpriteEndDl);
    return gdl;
}

/* Advances a sprite object's texture scroll, wrapping at the texture's period. */
void actor_scrollSpriteTexture(s32 index) {
    s32 period;
    s16 scroll;
    s16 speed;

    if (gPaused == 0) {
        scroll = movement_obj[index].texScroll;
        speed = movement_obj[index].texScrollSpeed;
        if (movement_obj[index].alpha != 0) {
            period = gEffectAnimTextures[actor_getSpriteTexture(index)]->frameCount << 8;
            scroll += speed;
            if (scroll >= period) {
                scroll -= period;
            } else if (scroll < 0) {
                scroll += period;
            }
            movement_obj[index].texScroll = scroll;
        }
    }
}

/* Switches the object to the LOD model for its distance from the player's camera. */
void actor_selectLod(s32 index, s16 player, s32 mode) {
    Camera *cam;
    Actor *actor;
    s32 special;
    s32 lod;
    s32 maxLod;
    s32 model;
    f32 range[4];
    f32 diff;
    f32 distSq;

    cam = &camera_global_Cameras[player];
    actor = &movement_obj[index];
    if (actor->forced_lod != 0) {
        return;
    }
    if (actor->drawtype == 7) {
        return;
    }
    range[0] = 500.0f;
    range[1] = 730.0f;
    range[2] = 1500.0f;
    range[3] = 2000.0f;
    maxLod = actor->lodCount - 1;
    if (maxLod == -1) {
        return;
    }
    special = collision_getModelId(actor);
    diff = cam->eye.x - actor->xpos;
    distSq = diff * diff;
    diff = cam->eye.y - actor->ypos;
    diff = diff * diff;
    distSq += diff;
    diff = cam->eye.z - actor->zpos;
    diff = diff * diff;
    distSq += diff;
    if (distSq < range[0] * range[0]) {
        lod = 0;
    } else if (distSq < range[1] * range[1]) {
        lod = 1;
    } else if (distSq < range[2] * range[2]) {
        lod = 2;
    } else if (distSq < range[3] * range[3]) {
        lod = 3;
    } else {
        lod = 4;
    }
    if (lod >= 2 && actor->speed < 3.0f) {
        lod--;
    }
    if (special) {
        if (special == 90) {
            if (lod == 0) {
                lod = 1;
            }
        }
    } else if (gIsMultiplayer != 0) {
        lod = 1;
    }
    if (gCutsceneStates[0] == 1) {
        lod = 0;
    }
    if (maxLod < lod) {
        lod = maxLod;
        if (lod < 0) {
            lod = 0;
        }
    }
    if (lod == -1) {
        return;
    }
    if (lod == actor->current_lod) {
        return;
    }
    model = actor->modelIds[lod];
    collision_setObjectModel(index, model, 0);
    actor->current_lod = lod;
}

/* Draws one object's model for one player; drops the output if the display list would overflow. */
Gfx *actor_drawModel(Gfx *gdl, s32 index, s32 player, s32 mode, s32 wrap) {
    s32 lod;
    s32 alpha;
    s32 colour[4];
    s32 overflow;
    void *model;
    Gfx *start;
    Actor *actor;

    start = gdl;
    if (gCutsceneActorLock != 0 && gCutsceneShowUnboundActors[0] == 0 && cutscene_isActorBound(&movement_obj[index]) == 0) {
        return gdl;
    }
    actor = &movement_obj[index];
    if ((actor->cullMask & (1 << player)) == (1 << player)) {
        return gdl;
    }
    if (actor_getAlpha(actor, player) == 0) {
        return gdl;
    }
    if (actor->ob_matrix_ptr == NULL) {
        return gdl;
    }
    if (mode != 4 && mode != 5 && mode != 3) {
        actor_selectLod(index, player, mode);
        lod = actor->current_lod;
    } else {
        collision_getLodModelId(actor, &lod);
    }
    func_1502D54C(index, colour);
    if (gLevelHeader->unk3E != 0) {
        actor_getFogAlpha(actor, colour, player);
    } else {
        colour[3] = 0xFF;
    }
    alpha = actor_getAlpha(actor, player);
    if (alpha < 0xFF) {
        if (actor->CameraPtr != NULL && actor->CameraPtr->Mode == 0x100 && player != actor->playernumber) {
            alpha = 0xFF;
        }
    }
    if (mode == 4) {
        alpha = (gAfterimageAlpha * alpha) >> 8;
        model = gBoneTrailMtxs;
    } else {
        model = actor->ob_matrix_ptr;
    }
    gdl = actor_emitModel(gdl, index, player, model, alpha, colour, mode, wrap);
    // matching: IDO materialises the comparison as a 0/1 value only through this flag
    if (gdl - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) {
        overflow = TRUE;
    } else {
        overflow = FALSE;
    }
    if (overflow) {
        return start;
    }
    return gdl;
}

/* Gathers the colours an object is drawn with: the player's prim/env colours and the object's fog. */
void actor_getDrawColours(Actor *actor, s32 player, s32 alpha, s32 *colour, s32 *primR, s32 *primG, s32 *primB,
                   s32 *envR, s32 *envG, s32 *envB, s32 *fogR, s32 *fogG, s32 *fogB, s32 *fogA) {
    u8 *prim;
    u8 *env;

    prim = gTintPrimColors[player];
    *primR = prim[0];
    env = gTintEnvColors[player];
    *primG = prim[1];
    *primB = prim[2];
    *envR = env[0];
    *envG = env[1];
    *envB = env[2];
    *fogR = 0;
    *fogG = 0;
    *fogB = 0;
    *fogA = colour[3];
    if (actor->fogMode != 0) {
        actor_getFogColour(actor, fogR, fogG, fogB, fogA);
    }
}

/* Emits an object's model: segments, geometry mode, lighting, colours and each part's display list. */
Gfx *actor_emitModel(Gfx *gdl, s32 index, s32 player, void *model, s32 alpha, s32 *colour, s32 mode, s32 wrap) {
    Actor *actor;
    s32 i;
    s32 lod;
    s32 id;
    s32 colourSet;
    s32 fb;
    s32 lightFlags;
    s32 levelFog;
    s32 lit;
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
    s32 pad; // matching: unused stack slot

    actor = &movement_obj[index];
    id = actor->character_type;
    if (mode == 3 || mode == 4 || mode == 5) {
        id = collision_getLodModelId(actor, &lod);
    } else {
        lod = actor->current_lod;
    }
    if (model_loadForActor(actor, id, lod, (mode == 3) ? 0 : 1) != 0) {
        return gdl;
    }
    fb = gDrawBufferIndex;
    if (actor->modelData[lod][fb] == 0) {
        return gdl;
    }
    if (wrap) {
        gSPDisplayList(gdl++, gObjectPassBeginDL);
    } else {
        gDPPipeSync(gdl++);
    }
    gdl = actor_bindSwapTextures(gdl, index);
    gdl = actor_setTextureScroll(gdl, index);
    gSPSegment(gdl++, 3, model);
    gSPSegment(gdl++, 1, actor->modelData[lod][fb] - 0x38);
    actor_getDrawColours(actor, player, alpha, colour, &primR, &primG, &primB, &envR, &envG, &envB, &fogR, &fogG, &fogB,
                  &fogA);
    if (mode == 5) {
        gSPSegment(gdl++, 8, D_800832C0);
    } else if (mode == 3) {
        gSPSegment(gdl++, 8, D_800832C0);
    } else if (alpha < 0xFF) {
        gSPSegment(gdl++, 8, gModelXluRenderModes);
    } else {
        gSPSegment(gdl++, 8, gModelOpaRenderModes);
    }
    gSPClearGeometryMode(gdl++, G_CULL_BOTH);
    if (mode == 0) {
        gSPSetGeometryMode(gdl++, G_CULL_FRONT);
    } else {
        gSPSetGeometryMode(gdl++, G_CULL_BACK);
    }
    lit = TRUE;
    levelFog = gLevelHeader->unk3E;
    if ((actor->unk66 & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, G_LIGHTING);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if ((actor->unk66 & 0xC) != 8) {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    } else if (gLightingEnabled != 0) {
        gSPSetGeometryMode(gdl++, G_LIGHTING | 0x400000);
    } else {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    }
    if (mode == 3) {
        gSPClearGeometryMode(gdl++, G_LIGHTING | 0x400000);
        lit = FALSE;
    }
    if (lit) {
        lightFlags = (actor->modelDrawn == 0) ? 0xA : 2;
        if (gStrobeCount != 0) {
            lightFlags |= 8;
        }
        gdl = light_pickLights(gdl, player, actor->xpos, actor->ypos, actor->zpos,
                            actor->floorMaterial, actor->gfxLights[player], actor->maxLights, &actor->ambients[gDrawBufferIndex],
                            &actor->lightCount, 2, actor->ambientColor, lightFlags, gPickedLights);
    }
    colourSet = -1;
    for (i = 0; i < gModelDlPartCounts[id]; i++) {
        if (!(actor->hiddenParts & (1 << i))) {
            if (!(actor->unk98 & (1 << i))) {
                if (colourSet != 0) {
                    gDPPipeSync(gdl++);
                    colourSet = 0;
                    if (levelFog == 0) {
                        gDPSetFogColor(gdl++, fogR, fogG, fogB, fogA);
                    } else {
                        gDPSetFogColor(gdl++, gFogColour[0], gFogColour[1], gFogColour[2], fogA);
                    }
                    gDPSetPrimColor(gdl++, 0xF2, 0, primR, primG, primB, 0);
                    gDPSetEnvColor(gdl++, envR, envG, envB, alpha);
                }
            } else if (colourSet != 1) {
                gDPPipeSync(gdl++);
                gDPSetFogColor(gdl++, 0, 0, 0, 0xFF);
                gDPSetPrimColor(gdl++, 0xF2, 0, 0, 0, 0, 0);
                gDPSetEnvColor(gdl++, 0, 0, 0, 0xFF);
                colourSet = 1;
            }
            if (mode == 3) {
                if (D_800C48F0[id] != NULL) {
                    gSPDisplayList(gdl++, D_800C48F0[id][i]);
                    continue;
                }
            } else {
                func_151EFE88(&gObjectIdentityMtx);
                gSPMatrix(gdl++, OS_K0_TO_PHYSICAL(&gObjectIdentityMtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gdl++, gModelDlParts[id][i]);
            }
        }
    }
    actor_refreshModelUnload(actor);
    actor->modelDrawn = 1;
    if (mode == 0) {
        gdl = attach_drawOwner(gdl, actor, player, 1);
    }
    if (wrap) {
        gSPDisplayList(gdl++, gObjectPassEndDL);
    }
    if (mode == 0) {
        gSPGeometryMode(gdl++, G_CULL_FRONT, G_CULL_BACK);
    }
    return gdl;
}

/* Object colour: for (unk66 & 0xC) == 4 the inverse of the mean of ambientColor and unk1E0, else white. */
void func_1502D54C(s32 index, s32 *colour) {
    Actor *actor;

    actor = &movement_obj[index];
    if ((actor->unk66 & 0xC) == 4) {
        colour[0] = (actor->unk1E0[0] + actor->ambientColor[0]) / 2;
        colour[1] = (actor->unk1E0[1] + actor->ambientColor[1]) / 2;
        colour[2] = (actor->unk1E0[2] + actor->ambientColor[2]) / 2;
        colour[0] = 0xFF - colour[0];
        colour[1] = 0xFF - colour[1];
        colour[2] = 0xFF - colour[2];
        return;
    }
    colour[2] = 0xFF;
    colour[1] = 0xFF;
    colour[0] = 0xFF;
}

/* Sets colour[3] to the object's distance-fog alpha as seen from the player's view. */
void actor_getFogAlpha(actor, colour, player)
    Actor *actor;
    s32 *colour;
    s32 player;
{
    f32 far;
    f32 near;
    f32 step;
    f32 first;
    f32 last;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 amount;

    if (gDefaultFar != 0.0f) {
        first = 1.0f / gDefaultFar;
    } else {
        first = 0.001000000047f;
    }
    if (gDefaultNear != 0.0f) {
        last = 1.0f / gDefaultNear;
    } else {
        last = 0.001000000047f;
    }
    step = (first - last) * 0.001000000047f;
    near = D_800DD2E8 * step + last;
    far = D_800DD2EC * step + last;
    math_transformPointW(gSpriteViewMtx[player], actor->xpos, actor->ypos, actor->zpos, &x, &y, &z, &w);
    w = (w != 0.0f) ? 1.0f / w : 0.001000000047f;
    if (w < 0) { // matching: integer zeros here; 0.0f literals share the 1.0f register differently
        amount = 0;
    } else if (w >= far) {
        amount = 0;
    } else if (w <= near) {
        amount = 1.0f;
    } else {
        amount = (w - far) / (near - far);
    }
    colour[3] = (1.0f - amount) * 255.0f;
}

/* DMAs the current frame of each animation channel into buf (anim, or actor->anim when actor is given). */
void actor_loadAnimFrames(AnimInstance *anim, Actor *actor, u8 *buf) {
    s32 frameSize;
    u32 size;
    u32 src;
    s32 wrap;
    s32 frame;
    f32 pos;
    f32 frameCount;
    s32 channels;
    s32 step;
    s32 i;
    u8 off;
    u8 align;

    channels = 1;
    align = 0;
    if (actor != NULL) {
        if (actor->animseq.uh == 0xFFFF) {
            actor_stopAnim(actor);
        }
        anim = actor->anim;
    }
    if (anim == NULL) {
        return;
    }
    if (anim->unk3C > 0) {
        channels = 2;
    }
    for (i = 0; i < channels; i++) {
        frameSize = anim->header[i][5];
        wrap = FALSE;
        if (!frameSize) {
            anim->nextFrameOffset[i] = 0;
            continue;
        }
        if (anim->frameRom[i] == 0) {
            continue;
        }
        pos = anim->frame[i];
        frameCount = anim->numFrames[i];
        size = frameSize * 2;
        frame = pos;
        if (frameCount <= frame) {
            frame = frameCount - 1.0f;
        }
        step = anim->header[i][8] + 1;
        if (step >= 2) {
            if (frameCount - 1.0f <= frame) {
                frame = (frameCount - 1.0f + (step - 1)) / step;
            } else {
                frame = frame / step;
            }
        }
        src = anim->frameRom[i] + frameSize * frame;
        align = src & 3;
        src -= align;
        if (frameCount <= pos + 1) {
            wrap = TRUE;
            size = frameSize;
        } else {
            anim->nextFrameOffset[i] = frameSize;
        }
        buf = (u8 *)((((u32)buf + 0xF) >> 4) << 4);
        size += align;
        anim->frameBuf[i] = (u8 *)(align + (u32)buf);
        off = (u8)(s32)align; // matching: this dead use of off (see `off * 0` below) steers register allocation
        if (size < 200) {
            rom_copyUncached(src, buf, size);
        } else {
            rom_read(src, buf, (size + 0xF) & ~0xF, 1);
        }
        buf += size + (off * 0);
        if (wrap) {
            src = anim->frameRom[i];
            buf = (u8 *)((((u32)buf + 0xF) >> 4) << 4);
            off = src & 1;
            src -= off;
            size = off + frameSize;
            if (size < 200) {
                rom_copyUncached(src, buf, size);
            } else {
                rom_read(src, buf, (size + 0xF) & ~0xF, 1);
            }
            anim->nextFrameOffset[i] = (buf + off) - anim->frameBuf[i];
            buf += size;
        }
    }
}

/* Number of bone matrices computed for a model id (four fewer than its bone count for some ids). */
s32 actor_getBoneMatrixCount(s32 id) {
    switch (id) {
    case 0x3B:
    case 0x75:
    case 0x82:
    case 0x88:
    case 0x90:
    case 0x96:
    case 0x98:
    case 0x9C:
    case 0x9D:
    case 0x9F:
    case 0xA0:
    case 0xB1:
    case 0xB2:
    case 0xB4:
        return gModelBoneCounts[id] - 4;
    }
    return gModelBoneCounts[id];
}

/* Computes the object's bone matrices for this frame into the matrix pool (root transform mtx). */
void actor_computeBones(MtxF mtx, s32 index) {
    s16 cmd[42];
    s32 id;
    u8 *remap;
    Actor *actor;
    s32 count;
    s32 moved;
    u8 frameBuf[0x320];
    u8 remapBuf[0x44];
    SpawnRecord *spawn;

    actor = &movement_obj[index];
    id = actor->character_type;
    remap = NULL;
    if (gModelData[id] == NULL) {
        spawn = actor->listpos_ptr;
        if (model_load(id, 0, spawn, 1, actor->modelLoadFlags) != 0) {
            actor->ob_matrix_ptr = NULL;
            return;
        }
        if (gModelData[id] == NULL) {
            actor->ob_matrix_ptr = NULL;
            return;
        }
    }
    if (actor->anim != NULL) {
        goto animated;
    }
    // matching: the unused label splits the block so as1 keeps the call's argument setup after the test
load:
    if (collision_allocAnimState(actor, 0, 0, 1.0f) == 0) {
    animated:
        if (gModelBoneCounts[id] < 2 || actor->anim != NULL) {
            count = actor_getBoneMatrixCount(id);
            if (gBonePoolSize * sizeof(Mtx) < gBonePoolCursor + count * sizeof(Mtx) - gBonePoolStart) {
                actor->ob_matrix_ptr = NULL;
                return;
            }
            actor_refreshModelUnload(actor);
            actor->ob_matrix_ptr = (void *)gBonePoolCursor;
            {
                s32 k;

                if (gModelDefs[id]->boneRemapList != 0) {
                    const u8 *p;

                    gAimPass = 0;
                    bzero(remapBuf, gModelBoneCounts[id]);
                    p = gBoneRemapLists[gModelDefs[id]->boneRemapList - 1];
                    while (*p != 0xFF) {
                        k = p[0];
                        remapBuf[k] = p[1];
                        p += 2;
                    }
                    remap = remapBuf;
                }
            }
            bone_buildCommands((GameCommand *)cmd, index);
            actor_loadAnimFrames(NULL, actor, frameBuf);
            {
                s16 scale;

                if (gCutsceneStates[0] != 1) {
                    scale = actor->scale_y * actor->heightOffset;
                } else {
                    scale = 0;
                }
                anim_buildBoneMatrices(&gBonePoolCursor, mtx, actor->anim, (s32)gModelBones[id], count, cmd, remap, scale);
            }
            actor->mod_angle = gAnimModAngle;
            moved = D_80088558;
            if (moved != 0) {
                f32 value;

                value = D_80088554 / 32768.0f;
                if (actor->unk1FC & 4) {
                    actor->unk1F8 -= moved * 0.03125f;
                }
                actor->unk1F4 = value - actor->unk1F8;
                actor->unk1F8 = value;
                if (actor->unk1FC & 2) {
                    actor->unk1FC &= ~3;
                    actor->unk1F4 = actor->speed;
                } else {
                    actor->unk1FC |= 1;
                }
                if (actor->intel_animspd & 0x40) {
                    value = moved * 0.03125f;
                    if (1) {} // matching: with the volatile below, keeps IDO from reusing the earlier speed load
                    value = actor->scale_xz * value;
                    value = value / actor->anim->numFrames[0];
                    value = *(volatile f32 *)&actor->speed / value;
                    actor->anim->speed[0] = value * 0.5f;
                }
            }
            gBoneMatricesUsed += count;
        }
    }
}

/* Builds the object's root matrix (from its parent's bone or its own transform); with
   rootOnly it is stored as the object's only bone, otherwise the full skeleton is computed. */
void actor_buildRootMatrix(s32 index, s32 rootOnly) {
    MtxF mtx;
    Actor *actor;
    Actor *other;
    s32 canCull;
    s32 i;
    s32 j;
    s32 maxAlpha;
    s32 alpha;
    MtxF *bone;
    f32 scale;
    void *dst;
    s32 parent;

    actor = &movement_obj[index];
    if (actor->scale_xz == 0.0f) {
        actor->scale_xz = 0.1000000015f;
        actor->targetScaleXz = 0.1000000015f;
    }
    if (actor->scale_y == 0.0f) {
        actor->scale_y = 0.1000000015f;
        actor->targetScaleY = 0.1000000015f;
    }
    if ((actor->unk66 & 0x20) == 0x20) {
        canCull = FALSE;
    } else {
        canCull = TRUE;
    }
    if (actor->mv != NULL && actor->mv->SniperState != 0) {
        canCull = FALSE;
    }
    if ((1 << index) & gStoodOnActors) {
        canCull = FALSE;
    }
    if (actor->character_type == 0x8A) {
        other = event_getOwnedObject(actor, 0);
        if (other != NULL && other->mv != NULL && other->mv->SniperState != 0) {
            canCull = FALSE;
        }
    }
    if (canCull) {
        if ((actor->cullMask & 0xF) == 0xF) {
            return;
        }
        maxAlpha = 0;
        for (i = 0; i <= gCurrentView; i++) {
            alpha = actor_getAlpha(actor, i);
            if (maxAlpha < alpha) {
                maxAlpha = alpha;
            }
        }
        if (maxAlpha == 0) {
            return;
        }
    }
    dc_glbl_CharNum = index;
    if (actor->attach_char && !(actor->unk101 & 0x10)) {
        parent = actor->attach_char - 1;
        bone = movement_obj[parent].ob_matrix_ptr;
        if (bone == NULL) {
            return;
        }
        bone += actor->basemtx;
        if (!(actor->unk101 & 4)) {
            for (i = 0; i < 3; i++) {
                if (i == 1) {
                    scale = actor->scale_y;
                } else {
                    scale = actor->scale_xz;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = (*bone)[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = (*bone)[3][i];
            }
            mtx[3][3] = 1.0f;
        } else {
            f32 len;

            for (i = 0; i < 3; i++) {
                len = sqrtf((*bone)[i][0] * (*bone)[i][0] + (*bone)[i][1] * (*bone)[i][1] +
                            (*bone)[i][2] * (*bone)[i][2]);
                if (i == 1) {
                    scale = actor->scale_y / len;
                } else {
                    scale = actor->scale_xz / len;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = (*bone)[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = (*bone)[3][i];
            }
            mtx[3][3] = 1.0f;
        }
    } else {
        math_mtxRotScaleF(mtx, actor->vtilt, actor->theta, actor->lean_angle, actor->scale_xz, actor->scale_y, actor->scale_xz);
        if (actor->unk101 & 0x10) {
            other = &movement_obj[actor->attach_char - 1];
            bone = movement_obj[actor->attach_char - 1].ob_matrix_ptr;
            if (bone == NULL) {
                mtx[3][0] = 0;
                mtx[3][1] = 0;
                mtx[3][2] = 0;
            } else {
                bone += actor->basemtx;
                mtx[3][0] = (*bone)[3][0];
                mtx[3][1] = (*bone)[3][1];
                mtx[3][2] = (*bone)[3][2];
            }
        } else {
            other = actor;
            mtx[3][0] = actor->xpos;
            mtx[3][1] = actor->ypos + actor->scale_y * actor->rootOffsetY;
            mtx[3][2] = actor->zpos;
        }
        if (other->anim != NULL && other->anim->frameRom[0] == 0) {
            mtx[3][1] += other->heightOffset;
        }
    }
    if (rootOnly) {
        dst = gBonePoolCursor;
        actor->ob_matrix_ptr = dst;
        bcopy(mtx, dst, sizeof(mtx));
        gBonePoolCursor += sizeof(mtx);
        gBoneMatricesUsed++;
        return;
    }
    actor_computeBones(mtx, index);
}

/* Flushes this frame's bone matrices (gBoneMatricesUsed of them) from the pool. */
void actor_flushBoneMatrices(void) {
    if (gBoneMatricesUsed != 0) {
        math_mtxFToLInPlace(gBonePools[gDrawBufferIndex], gBoneMatricesUsed);
    }
    gBoneMatricesFlushed = 1;
}

/* Visibility test of the object against each player's view; updates the per-player culled mask
   (cullMask) and reports the result per player through actor_fadeCommand. */
void actor_updateCulling(s32 index) {
    Actor *actor;
    u8 id;
    MtxF *tbl;
    RenderView *view;
    f32 width;
    f32 height;
    f32 yOffset;
    f32 cullDist;
    s32 maxDist;
    s32 i;
    s32 mask;
    s32 inside;
    s32 range;
    s32 playerRange;
    s32 oldMask;
    s32 event;

    mask = 1;
    actor = &movement_obj[index];
    id = actor->character_type;
    if (actor->drawtype == 7) {
        actor->cullMask &= ~0xF;
        return;
    }
    oldMask = actor->cullMask;
    actor->cullMask |= 0xF;
    if (gCutsceneActorLock != 0 && gCutsceneShowUnboundActors[0] == 0 && cutscene_isActorBound(actor) == 0) {
        return;
    }
    if (actor->playernumber != 0xFF && !(gMatchState.playerMask & (1 << actor->playernumber))) {
        return;
    }
    if ((actor->unk101 & 1) == 1) {
        return;
    }
    if (actor->alpha == 0) {
        return;
    }
    if (actor->attach_char != 0) {
        actor->cullMask = movement_obj[actor->attach_char - 1].cullMask;
        return;
    }
    width = gModelDefs[id]->bodyRadius * actor->scale_xz;
    height = gModelDefs[id]->bodyRadius * actor->scale_y;
    yOffset = (gModelDefs[id]->bodyHeight + actor->heightOffset) * actor->scale_y;
    actor->cull_radius_xz = width;
    range = actor->cullDistance;
    if (gCutsceneStates[0] == 1 && gCutsceneUseCullDistance[0] == 0) {
        range = 0;
    }
    maxDist = 0;
    if (actor->unk101 & 0x80) {
        cullDist = 0.0f;
    } else {
        if (gModelDefs[id]->cullDistance == 0) {
            cullDist = actor->bodyRadius;
        } else if (gModelDefs[id]->cullDistance == 1) {
            cullDist = 0.0f;
        } else {
            cullDist = gModelDefs[id]->cullDistance * actor->scale_xz;
        }
    }
    for (i = 0; i <= gScreenLayout; i++) {
        s32 dist;

        view = &gViews[i];
        tbl = &gViewMatrices[i];
        inside = FALSE;
        if (camera_global_Cameras[i].mv->SniperState != 0) {
            playerRange = 100000;
        } else {
            playerRange = range;
        }
        dist = view_testCylinderInView(view, *tbl, actor->xpos, actor->ypos + yOffset, actor->zpos, width,
                             height, 20000.0f);
        if (dist == 0) {
            inside = TRUE;
        } else {
            if (dist < 0) {
                dist = -dist;
            }
            if (maxDist < dist) {
                maxDist = dist;
            }
            if ((actor->unk66 & 0x80) != 0x80 && dist <= cullDist && gCutsceneStates[0] != 1) {
                inside = TRUE;
            }
        }
        if (!inside) {
            if (playerRange != 0 && i != index) {
                if (playerRange < dist) {
                    event = 8;
                } else {
                    event = 9;
                }
                actor_fadeCommand(actor, event, 0, 0, i);
            } else {
                actor->playerFadeBits |= (1 << i) << 4;
                actor_fadeCommand(actor, 9, 0, 0, i);
                event = -1;
            }
            if ((actor->noshove & 0x800000) && event != 8 && gCutsceneStates[0] != 1 && zone_isActorHiddenFromView(actor, i, oldMask) != 0) {
                inside = TRUE;
            }
        } else {
            event = 10;
        }
        if (event != -1) {
            actor_fadeCommand(actor, event, 1, 0, i);
        }
        if (!inside) {
            actor->cullMask &= ~mask;
        }
        if (actor_getAlpha(actor, i) == 0) {
            actor->cullMask |= mask;
        }
        mask <<= 1;
    }
    func_1502E9FC(actor, maxDist);
    if (actor->CameraPtr != NULL && (actor->CameraPtr->Mode == 0x10000 || actor->mv->SniperState != 0)) {
        actor->cullMask |= 1 << index;
    }
    if (gCutsceneStates[0] != 1) {
        if (actor->forcedCullMask != 0) {
            actor->cullMask |= actor->forcedCullMask;
        }
    }
}

/* Empty in the final game. */
void func_1502E9FC(Actor *actor, s32 dist) {
}

/* Fog effect modes (actor->fogMode), advanced by actor_updateFog and turned into a fog colour by
   actor_getFogColour:
     1    fixed colour fogLevel/fogRate/fogFade, alpha fogColor (or *(f32 *)fogColor percent)
     2/3  black fog fading out / in at fogRate
     4    fogColor pulsing (fogLevel is the phase), 5 the same while fading out
     6/7  fogColor flash: one step up (6), then fading out at fogRate (7) */
/* Starts a pulsing fog of colour (r, g, b, a). */
void actor_startFogPulse(Actor *actor, s32 r, s32 g, s32 b, s32 a, s32 rate) {
    actor->fogMode = 4;
    actor->fogLevel = 0;
    actor->fogRate = rate;
    actor->fogColor = (a << 24) | (r << 16) | (g << 8) | b;
    actor->fogFade = 0xFF;
}

/* Fades out the pulsing fog. */
void actor_fadeFogPulse(Actor *actor) {
    actor->fogMode = 5;
}

/* Starts black fog fading out. */
void actor_startFogFadeOut(Actor *actor, s32 rate) {
    actor->fogMode = 2;
    actor->fogLevel = 0xFF;
    actor->fogRate = rate;
}

/* Starts black fog fading in. */
void actor_startFogFadeIn(Actor *actor, s32 rate) {
    actor->fogMode = 3;
    actor->fogLevel = 0xFF;
    actor->fogRate = rate;
}

/* Flashes fog of colour (r, g, b, a), from full strength when instant, then fades it out. */
void actor_flashFog(Actor *actor, s32 r, s32 g, s32 b, s32 a, s32 instant, s32 rate) {
    if (actor->fogMode != 6 && actor->fogMode != 7) {
        if (instant) {
            actor->fogLevel = 0xFF;
        } else {
            actor->fogLevel = 0;
        }
        actor->fogColor = (a << 24) | (r << 16) | (g << 8) | b;
        actor->fogRate = rate;
    }
    actor->fogMode = 6;
}

/* Advances the object's fog effect by one frame. */
void actor_updateFog(Actor *actor) {
    // matching: per-case blocks with the break after them keep the last store out of the delay slot
    switch (actor->fogMode) {
    case 2:
    case 3: {
        s32 step = actor->fogRate * gFrameTicks;
        u8 intensity = actor->fogLevel;

        if (step < intensity) {
            actor->fogLevel = intensity - step;
        } else {
            actor->fogLevel = 0;
        }
    }
        break;
    case 5: {
        s32 step = gFrameTicks * 10;
        u8 fade = actor->fogFade;

        if (step < fade) {
            actor->fogFade = fade - step;
        } else {
            actor->fogMode = 0;
        }
    }
        /* fallthrough */
    case 4:
        actor->fogLevel += actor->fogRate * gFrameTicks;
        break;
    case 6: {
        s32 step = actor->fogRate * gFrameTicks;
        u8 intensity = actor->fogLevel;

        if (intensity < 0xFF - step) {
            actor->fogLevel = intensity + step;
        } else {
            actor->fogLevel = 0xFF;
        }
        actor->fogMode = 7;
    }
        break;
    case 7: {
        s32 step = actor->fogRate * gFrameTicks;
        u8 intensity = actor->fogLevel;

        if (step < intensity) {
            actor->fogLevel = intensity - step;
        } else {
            actor->fogMode = 0;
        }
    }
        break;
    }
}

/* Fog colour (r, g, b, a) of the object's current fog effect (see actor_updateFog for the modes). */
void actor_getFogColour(actor, r, g, b, a)
Actor *actor;
s32 *r;
s32 *g;
s32 *b;
s32 *a;
{
    Actor *obj = actor; // matching: the parameter copy gets its own stack slot
    f32 intensity;

    switch (obj->fogMode) {
    case 1:
        *r = obj->fogLevel;
        *g = obj->fogRate;
        *b = obj->fogFade;
        if (obj->fogColor >= 0x100) {
            intensity = *(f32 *)obj->fogColor;
            intensity = (intensity < 0.0f) ? 0.0f : ((intensity > 255.0f) ? 255.0f : intensity);
            *a = intensity * 2.55f;
        } else {
            *a = obj->fogColor;
        }
        break;
    case 2:
    case 3:
        *r = 0;
        *g = 0;
        *b = 0;
        *a = obj->fogLevel;
        if (obj->fogMode == 3) {
            *a = 0xFF - *a;
        }
        break;
    case 4:
    case 5: {
        s32 alpha;
        f32 pulse;

        alpha = ((s32)obj->fogColor >> 24) & 0xFF;
        *r = ((s32)obj->fogColor >> 16) & 0xFF;
        *g = ((s32)obj->fogColor >> 8) & 0xFF;
        *b = obj->fogColor & 0xFF;
        pulse = (cosf(obj->fogLevel * 0.02454369329f) + 1.0f) * 0.5f;
        *a = alpha + 64.0f * pulse;
        *a += ((0xFF - *a) * (0xFF - obj->fogFade)) >> 8;
    }
        break; // matching: outside the block, so the store stays out of the branch delay slot
    case 6:
    case 7: {
        s32 alpha;

        alpha = ((s32)obj->fogColor >> 24) & 0xFF;
        *r = ((s32)obj->fogColor >> 16) & 0xFF;
        *g = ((s32)obj->fogColor >> 8) & 0xFF;
        *b = obj->fogColor & 0xFF;
        *a = 0xFF - ((obj->fogLevel * alpha) >> 8);
        break;
    }
    }
}

/* Maps the object's unk6A[slot] value to 0, 1 or 2 (values 2..3 -> 0..1, 4 and up -> 2). */
s32 actor_getEyeTarget(s32 index, s32 slot) {
    s32 value;
    s32 result;

    value = movement_obj[index].eyeClosed[slot];
    if (value >= 2) {
        if (value >= 4) {
            result = 2;
        } else {
            result = value - 2;
        }
    } else {
        result = value;
    }
    return result;
}

/* Steps each of the object's two unk6C counters towards the state actor_getEyeTarget reports. */
void actor_updateEyes(s32 index) {
    s32 i;
    u8 state;

    for (i = 0; i < 2; i++) {
        if (movement_obj[index].eyeCode[i] >= 10) {
            continue;
        }
        state = actor_getEyeTarget(index, i);
        if (state == 0) {
            if (movement_obj[index].eyeCode[i] > 0) {
                movement_obj[index].eyeCode[i]--;
            }
        } else if (state == 1) {
            if (movement_obj[index].eyeCode[i] < 2) {
                movement_obj[index].eyeCode[i]++;
            }
        } else {
            movement_obj[index].eyeCode[i] = 1;
        }
    }
    face_showEyeParts(&movement_obj[index]);
}

/* Binds the object's swappable textures to segments 6, 7 (unk6C slots), 10 and 11. */
Gfx *actor_bindSwapTextures(Gfx *gdl, s32 index) {
    Actor *actor;
    u8 texIndex[2];
    s32 id;
    ExpressionRecord *override;
    s32 i;
    ObjTexture *tex;

    actor = &movement_obj[index];
    id = actor->character_type;
    override = NULL;
    for (i = 0; i < 2; i++) {
        if (actor->eyeCode[i] >= 10) {
            texIndex[i] = actor->eyeCode[i] - 10;
        } else {
            texIndex[i] = gModelDefs[id]->texture[i][actor->eyeCode[i]];
            if (actor->expression && !override) {
                override = face_getExpressionRecord(actor, actor->expression);
            }
            if (override != NULL) {
                s32 overrideTex[2];

                overrideTex[0] = override->eyeCode[0];
                overrideTex[1] = override->eyeCode[1];
                if (override != NULL) { // matching: dropping this redundant re-test changes the code
                    if (overrideTex[i] != gModelDefs[id]->texture[i][2]) {
                        if (actor->eyeCode[i] == 0) {
                            texIndex[i] = overrideTex[i];
                        }
                    } else {
                        texIndex[i] = overrideTex[i];
                    }
                }
            }
        }
    }
    tex = gModelTextures[actor->character_type];
    if (tex != NULL) {
        gSPSegment(gdl++, 6, tex[texIndex[0]].addr);
        gSPSegment(gdl++, 7, tex[texIndex[1]].addr);
        gSPSegment(gdl++, 10, tex[actor->swapTexture[0]].addr);
        gSPSegment(gdl++, 11, tex[actor->swapTexture[1]].addr);
    }
    return gdl;
}

/* Moves an attached object onto its parent (or the parent's bone) and copies the parent's state. */
void actor_followParent(s32 index) {
    Actor *actor;
    Actor *parent;
    MtxF *bone;
    s32 i;

    actor = &movement_obj[index];
    if (actor->attach_char == 0) {
        return;
    }
    parent = &movement_obj[actor->attach_char - 1];
    if ((actor->unk101 & 2) == 2) {
        return;
    }
    bone = parent->ob_matrix_ptr;
    if (bone == NULL) {
        actor->xpos = parent->xpos;
        actor->ypos = parent->ypos;
        actor->zpos = parent->zpos;
    } else {
        bone += actor->basemtx;
        actor->xpos = (*bone)[3][0];
        actor->ypos = (*bone)[3][1];
        actor->zpos = (*bone)[3][2];
    }
    actor->floorY = parent->floorY;
    actor->yspd = -4.0f;
    if (!(actor->unk101 & 0x20)) {
        actor->angle = parent->angle;
        actor->speed = parent->speed;
        actor->theta = parent->theta;
        actor->angle = parent->angle; /* copied twice in the original */
        actor->langle = parent->langle;
        actor->fangle = parent->fangle;
    }
    if (!(actor->unk101 & 0x40)) {
        actor->alpha = parent->alpha;
        actor->targetAlpha = parent->targetAlpha;
        actor->fadeStep = parent->fadeStep;
        actor->fadeMode = parent->fadeMode;
        actor->playerFadeBits = parent->playerFadeBits;
        for (i = 0; i <= gScreenLayout; i++) {
            actor->playerAlphas[i] = parent->playerAlphas[i];
        }
    }
}

/* Carries every object standing on another object (unk274) along with that object's surface. */
void actor_carryStandingObjects(void) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if (movement_obj[i].type == 0) {
            continue;
        }
        if (movement_obj[i].stand_on_char == 0) {
            continue;
        }
        movement_obj[i].ypos = movement_obj[i].floorY;
        actor_movePointWithPlatform(&movement_obj[movement_obj[i].stand_on_char - 1], &movement_obj[i].xpos, &movement_obj[i].ypos,
                      &movement_obj[i].zpos, movement_obj[i].floorTriIndex);
        if (movement_obj[i].ypos < movement_obj[i].floorY) {
            movement_obj[i].ypos = movement_obj[i].floorY;
        } else {
            movement_obj[i].floorY = movement_obj[i].ypos;
        }
    }
}

/* Moves the point (x, y, z) with triangle tri of the platform's skinned model: finds the point's
   barycentric position in last frame's triangle and returns it from this frame's. */
void actor_movePointWithPlatform(Actor *platform, f32 *x, f32 *y, f32 *z, s32 tri) {
    f32 diff[3];
    f32 moved[3];
    s32 bone[3];
    s32 i;
    s32 j;
    s32 found;
    s32 range;
    u8 id;
    Vtx *vtx[3];
    f32 v[2][3][3];
    f32 e1[2][3];
    f32 e2[2][3];
    MtxF *bones;
    f32 s;
    f32 t;

    id = platform->character_type;
    if (gModelTris[id] == NULL) {
        return;
    }
    for (i = 0; i < 3; i++) {
        vtx[i] = (Vtx *)(gModelTris[id][tri].v[i] + gModelData[id]);
    }
    range = 0;
    found = 0;
    do {
        if (range == gModelVtxRunCounts[id]) {
            return;
        }
        for (i = 0; i < 3; i++) {
            if (!(found & (1 << i))) {
                if (vtx[i] >= gModelVtxRuns[id][range].start &&
                    vtx[i] < gModelVtxRuns[id][range].start + gModelVtxRuns[id][range].count) {
                    found = (1 << i) | found;
                    bone[i] = gModelVtxRuns[id][range].mtxIndex;
                }
            }
        }
        range++;
    } while (found != 7);

    if (platform->prevBones == NULL || platform->ob_matrix_ptr == NULL) {
        return;
    }
    bones = platform->prevBones;
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            bones = platform->ob_matrix_ptr;
        }
        for (j = 0; j < 3; j++) {
            // matching: byte-offset form; bones[bone[j]] swaps the addu operands
            math_transformPoint((f32 (*)[4])((u8 *)bones + (bone[j] << 6)), vtx[j]->v.ob[0], vtx[j]->v.ob[1],
                          vtx[j]->v.ob[2], &v[i][j][0], &v[i][j][1], &v[i][j][2]);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            e1[i][j] = v[i][1][j] - v[i][0][j];
            e2[i][j] = v[i][2][j] - v[i][0][j];
        }
    }
    diff[0] = *x - v[0][0][0];
    diff[1] = *y - v[0][0][1];
    diff[2] = *z - v[0][0][2];
    s = e1[0][0] * e2[0][2] - e1[0][2] * e2[0][0];
    if (s != 0.0f) {
        s = (diff[0] * e2[0][2] - diff[2] * e2[0][0]) / s;
    } else {
        s = -100.0f;
    }
    if (s < 0.0f || s > 1.0f) {
        return;
    }
    if (e2[0][2] != 0.0f) {
        t = (diff[2] - s * e1[0][2]) / e2[0][2];
    } else {
        t = -100.0f;
    }
    if (t < 0.0f || t > 1.0f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        moved[i] = s * e1[1][i] + e2[1][i] * t;
    }
    diff[1] = s * e1[0][1] + e2[0][1] * t;
    *x += moved[0] + v[1][0][0] - diff[0] - v[0][0][0];
    *y += moved[1] + v[1][0][1] - diff[1] - v[0][0][1];
    *z += moved[2] + v[1][0][2] - diff[2] - v[0][0][2];
}

/* Keeps a copy of the object's bone matrices in prevBones (allocated on first use). */
void actor_savePrevBones(Actor *actor) {
    s32 id;
    void *copy;

    if ((actor->noshove & 0x4000) && actor->hit_vtx_array != 0 && actor->ob_matrix_ptr != NULL) {
        id = actor->character_type;
        if (actor->prevBones == NULL) {
            copy = allocate_memory(gModelBoneCounts[id] * sizeof(*actor->ob_matrix_ptr), 1, 1, 2);
            actor->prevBones = copy;
            if (copy == NULL) {
                return;
            }
        }
        bcopy(actor->ob_matrix_ptr, actor->prevBones, gModelBoneCounts[id] * sizeof(*actor->ob_matrix_ptr));
    }
}

/* Sets the scroll of the object's two scrolling textures (tiles 5 and 4) from unk27A/unk27E. */
Gfx *actor_setTextureScroll(Gfx *gdl, s32 index) {
    s32 id;
    s32 texIndex;
    s32 i;
    ObjTexture *tex;
    s32 uls;
    s32 ult;
    s32 lrs;
    s32 lrt;
    s32 tile;

    id = movement_obj[index].character_type;
    if (id == 0x89 || id == 0xBA) {
        return func_150C3160(gdl, &movement_obj[index]);
    }
    if (id == 0) {
        texIndex = 14;
    } else if (id == 0x96) {
        texIndex = 7;
    } else if (id == 0x28) {
        texIndex = 4;
    } else if (id == 1 || id == 2 || id == 3 || id == 4) {
        texIndex = 5;
    } else {
        texIndex = 0;
    }
    if (gModelTextures[id] != NULL) {
        for (i = 0; i < 2; i++) {
            tex = &gModelTextures[id][texIndex];
            uls = (u16)(movement_obj[index].headYaw[i] + 2);
            ult = (u16)(movement_obj[index].headPitch[i] + 2);
            lrs = (u16)(tex->width * 4 - 2);
            lrt = (u16)(tex->height * 4 - 2);
            tile = 5 - i;
            gDPSetTileSize(gdl++, tile, (u16)uls, (u16)ult, (u16)lrs, (u16)lrt);
        }
    }
    return gdl;
}

/* Applies a requested LOD model change (forced_lod), restarting the animation if the new model
   uses a different animation set. */
void actor_applyForcedLod(Actor *actor) {
    s32 lod;
    ActorAnim *oldAnims;
    s32 model;

    if (actor->forced_lod == 0) {
        return;
    }
    oldAnims = gModelAnimTables[actor->character_type];
    if (actor->forced_lod == 0xFF) {
        actor->forced_lod = 0;
        lod = 0;
    } else {
        lod = actor->forced_lod - 1;
        if (lod >= actor->modelCount || lod == actor->current_lod) {
            return;
        }
    }
    model = actor->modelIds[lod];
    collision_setObjectModel(actor - movement_obj, model, 0);
    actor->current_lod = lod;
    if (gCutsceneStates[0] == 1) {
        gCutsceneActorLock = 0;
    }
    if (oldAnims != gModelAnimTables[model]) {
        if (actor->animseq.uh >= gModelAnimCounts[model]) {
            actor->animseq.uh = 0xFFFF;
            actor_playAnim(actor, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        } else {
            s32 anim = actor->animseq.uh;

            actor->animseq.uh = 0xFFFF;
            actor_playAnim(actor, anim, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        }
    }
    if (gCutsceneStates[0] == 1) {
        gCutsceneActorLock = 1;
    }
    face_revertExpression(actor);
}

/* Refreshes the gModelUnloadTimers counter of the object's model id (2 in one special case, 3 by default,
   or keepTime seconds). */
void actor_refreshModelUnload(Actor *actor) {
    Actor *obj = actor; // matching: the parameter copy changes register allocation
    u8 id = obj->character_type;
    u8 *counter;

    if (gExpansionPak == 0 && level == LEVEL_FRONTEND) {
        gModelUnloadTimers[id] = 2;
        return;
    }
    counter = &gModelUnloadTimers[id];
    if (*counter != 0xFF) {
        // matching: the shared default store needs the goto (an || condition reorders the tests)
        if (obj->listpos_ptr != NULL) {
            if (obj->listpos_ptr->keepTime != 0xFF) {
                if (obj->listpos_ptr->keepTime != 0) {
                    if (*counter < obj->listpos_ptr->keepTime * 30) {
                        *counter = obj->listpos_ptr->keepTime * 30;
                    }
                } else {
                    goto useDefault;
                }
            }
        } else {
        useDefault:
            *counter = 3;
        }
    }
}
