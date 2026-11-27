#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
u8 *gModelData[187]; // per model id: model data (just past its header)
ModelDef *gModelDefs[187];
u8 D_800D1F80[187];
u8 gModelUnloadTimers[187]; // per model id counter
SpawnRecord *gSpawnRecords;
u8 gSpawnRecordCount;
u8 gSpawnFirstCount;
PathPoint **gSpawnPaths;
u8 *gSpawnPathCounts;
u8 *gSpawnInUse;
u16 *gSpawnDelays;

#include "conker/actor.h"
#include "conker/aim.h"
#include "conker/anim.h"
#include "conker/attach.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/model.h"
#include "conker/player.h"
#include "conker/rom.h"
#include "conker/save.h"
#include "conker/texture.h"
#include "conker/view.h"
#include "conker/zone.h"



/* Spawns the object for spawn record spawnIdx once it is due: skips records that are in use,
   disabled (x == 0x7FFF) or still counting down their gSpawnDelays delay, and records whose flags
   ask for no player nearby (radius by flags & 6) or for being off-screen (flags & 8). */
s32 collision_trySpawnRecord(s32 spawnIdx, s32 arg1) {
    f32 radiusSq;
    s32 i;
    u16 range;
    u8 clear;
    u8 flags;
    s32 j;
    f32 dx;
    f32 dz;

    if (gSpawnInUse[spawnIdx] != 0) {
        return 0;
    }
    if (gSpawnRecords[spawnIdx].removed == 1) {
        return 0;
    }
    if (gSpawnRecords[spawnIdx].x == 0x7FFF) {
        return 0;
    }
    if (gSpawnDelays[spawnIdx]) {
        if (gSpawnDelays[spawnIdx] <= gFrameSteps) {
            gSpawnDelays[spawnIdx] = 0;
        } else {
            gSpawnDelays[spawnIdx] -= gFrameSteps;
        }
        return 0;
    }
    flags = gSpawnRecords[spawnIdx].flags;
    if (spawnIdx >= 5) {
        range = flags & 6;
        if (range != 0) {
            if (range == 2) {
                radiusSq = 422500.0f;
            } else if (range == 4) {
                radiusSq = 1690000.0f;
            } else {
                radiusSq = 7562500.0f;
            }
            clear = 1;
            for (j = 0; j < gPlayerCount; j++) {
                if (movement_obj[j].type == 0) {
                    continue;
                }
                if (movement_obj[j].playernumber == 0xFF) {
                    continue;
                }
                if (gMatchState.playerMask & (1 << movement_obj[j].playernumber)) {
                    // matching: squaring in place (d = d * d) reuses the operand's register
                    dx = movement_obj[j].xpos - gSpawnRecords[spawnIdx].x;
                    dx = dx * dx;
                    dz = movement_obj[j].zpos - gSpawnRecords[spawnIdx].z;
                    dz = dz * dz;
                    if (dx + dz <= radiusSq) {
                        clear = 0;
                        break;
                    }
                }
            }
            if (clear) {
                return 0;
            }
        }
    }
    if (flags & 8) {
        clear = 1;
        for (i = 0; i <= gScreenLayout; i++) {
            // matching: offset + base operand order
            if (view_testCylinderInView(i + gViews, gViewMatrices[i],
                              gSpawnRecords[spawnIdx].x,
                              gSpawnRecords[spawnIdx].y + 150,
                              gSpawnRecords[spawnIdx].z,
                              150.0f, 150.0f, 20000.0f) != 0) {
                clear = 0;
            }
        }
        if (!clear) {
            return 0;
        }
    }
    return collision_spawnObject(&gSpawnRecords[spawnIdx], spawnIdx, 0, 0, 0);
}

/* Runs collision_trySpawnRecord over the spawn records: [0, gSpawnFirstCount) when firstRange is set, else
   [gSpawnFirstCount, gSpawnRecordCount). */
void collision_trySpawnRecords(s32 arg0, s32 arg1, s32 firstRange) {
    s32 start;
    s32 end;
    s32 i;

    if (firstRange != 0) {
        start = 0;
        end = gSpawnFirstCount;
    } else {
        start = gSpawnFirstCount;
        end = gSpawnRecordCount;
    }

    for (i = start; i < end; i++) {
        collision_trySpawnRecord(i, arg1);
    }
}

void func_150829D8(Actor *obj) {
    obj->noshove &= 0xF7FFFFFD;
    obj->noshove |= 4;
    if ((obj->mv != NULL) && (obj->mv->unk56 == 0)) {
        switch (level) {
        case LEVEL_W1_BULL_PADDOCK:
        case LEVEL_W2_BOILERROOM:
        case LEVEL_W2_TANK_FACTORY:
        case LEVEL_W1_INSIDE_SAFE:
        case LEVEL_W2_HARBOUR:
        case LEVEL_W1_VOLCANO_INSIDE:
        case LEVEL_W2_LIFTTUNNEL:
        case LEVEL_W1_POO_CABIN:
        case LEVEL_W3_HAUNTED_HOUSE:
        case LEVEL_W3_HAUNTED_OUTHOUSE:
        case LEVEL_W1_BRONTO_THROAT:
        case LEVEL_W1_RAPTOR_NEST:
        case LEVEL_W1_RAPTOR_NEST_FLOODED:
            break;
        default:
            obj->noshove |= 2;
            break;
        }
    }
}

/* Creates and initialises a game object from spawn record spawnIdx (slot: 1-based object slot to
   reuse, 0 for the default). Returns the 1-based slot, or 0 on failure. */
s32 collision_spawnObject(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot) {
    s32 type;
    Actor *obj;
    s32 idx;
    s32 ret;
    s32 t;
    s32 i;
    s32 found;
    u32 model;
    VtxMorphState *mem;
    u16 angle;

    if (gSpawnInUse[spawnIdx] & ~0x80) {
        return 0;
    }
    type = spawn->type;
    if (slot != 0) {
        obj = &movement_obj[slot - 1];
        actor_clearSlot(obj);
    } else if ((spawnIdx != -1) && (spawnIdx < gPlayerCount)) {
        obj = &movement_obj[spawnIdx];
        actor_clearSlot(obj);
    } else {
        obj = actor_alloc();
    }
    idx = obj - movement_obj;
    if (idx == 25) {
        return 0;
    }
    if (spawnIdx != -1) {
        if (spawn->flags & 0x21) {
            gSpawnInUse[spawnIdx] = idx + 1;
        } else {
            gSpawnInUse[spawnIdx] = idx + 0x81;
        }
        obj->listpos = spawnIdx;
        if (spawnIdx < gPlayerCount) {
            obj->playernumber = spawnIdx;
        } else {
            obj->playernumber = 0xFF;
        }
    }
    obj->listpos_ptr = spawn;
    ret = idx + 1;
    collision_loadSpawnModel(spawn, obj->listpos, arg3);
    if ((idx < gPlayerCount) || (type == 0x98) || (type == 0x28)) {
        obj->mv = allocate_memory(sizeof(ActorCtl), 1, 0, 0);
        bzero(obj->mv, sizeof(ActorCtl));
        obj->mv->gunPitch = 5.0f;
    }
    obj->unique_id = spawn->unique_id;
    obj->unk1D0 = 2;
    obj->scale_xz = spawn->xz_scale;
    obj->scale_y = spawn->y_scale;
    obj->targetScaleXz = spawn->xz_scale;
    obj->targetScaleY = spawn->y_scale;
    obj->type = spawn->interaction_state;
    obj->xpos = spawn->x;
    obj->ypos = spawn->y;
    obj->zpos = spawn->z;
    obj->pathAvoid = 0xFF;
    obj->pathNode = 0xFF;
    obj->prevPathNode = 0xFF;
    obj->pathGoal = 0xFF;
    obj->team = 0xFF;
    obj->old_xpos = obj->xpos;
    obj->old_ypos = obj->ypos;
    obj->fallStartY = obj->ypos;
    obj->old_zpos = obj->zpos;
    if ((spawn->flags & 6) || (spawn->flags & 4) || (spawn->flags & 2)) {
        actor_fadeCommand(obj, 4, 0, 0, 0);
    }
    obj->intelwalk = spawn->unkD;
    if ((spawnIdx < gPlayerCount) && (gRoster[spawnIdx] == -2)) {
        obj->mv->bot = 1;
        obj->intelwalk = 1;
    }
    {
        // matching: unused stack slot
        u8 pad;

        if (obj->playernumber != 0xFF) {
            obj->team = save_getTeam(obj->playernumber);
            obj->mv->numGrenades = gSpawnGrenadeCount;
            obj->mv->unk1AF = 0x40;
            obj->mv->lastSpawnIndex = -1;
        }
    }
    obj->energy = spawn->unkE & 0xF;
    obj->moveTurnRate = spawn->turnRate;
    obj->faceTurnRate = obj->moveTurnRate;
    obj->waypointStep = 1;
    obj->nearRangeDefault = spawn->unk1C;
    obj->sightRange = spawn->unk1D;
    obj->angle = spawn->angle << 8;
    angle = obj->angle;
    obj->langle = angle;
    obj->fangle = angle;
    obj->theta = (angle + 0x4000) * (360.0f / 65536.0f);
    obj->unk140 = arg2;
    obj->ygrav = 6.0f;
    obj->noshove = spawn->objFlags;
    if (obj->noshove & 0x200000) {
        obj->unk224 = obj->ypos;
    }
    if (obj->noshove & 0x10000000) {
        obj->killHeight = obj->ypos - 500.0f;
    }
    if (obj->noshove & 0x800000) {
        gZoneCursorEnabled = 1;
    }
    obj->family = spawn->unk11;
    obj->unk66 = spawn->unk10;
    obj->ambientColor[0] = 120;
    obj->ambientColor[1] = 120;
    obj->ambientColor[2] = 120;
    obj->unk1E0[0] = 255;
    obj->unk1E0[1] = 255;
    obj->unk1E0[2] = 255;
    obj->arriveRange = 4;
    obj->intel_animspd = 1;
    obj->intel_accel = 100;
    obj->intel_decel = 100;
    obj->unk1FC |= 2;
    collision_applyModelDefaults(obj, type, spawn);
    t = spawn->unk13 & 0x7F;
    obj->attachedObject = ((spawn->unk13 & 0x80) << 8) | t;
    obj->unk90 = spawn->unk14;
    collision_setObjectModel(idx, type, 1);
    if (type != 0xFF) {
        collision_allocAnimState(obj, 0, 0, 1.0f);
    } else {
        obj->drawtype = 3;
    }
    if (obj->noshove & 0x4000) {
        model_loadForActor(obj, obj->character_type, 0, 0);
    }
    if (spawn->partSet) {
        collision_createPartSet(obj, spawn->partSet, 1.0f, 0);
    }
    if (obj->playernumber != 0xFF) {
        obj->energy = player_getStat(obj->playernumber, 4);
        obj->underwaterTime = gCarriedUnderwaterTime;
        func_150829D8(obj);
        collision_applyCharacterVariant(obj, player_getStat(idx, 1));
    }
    if (((obj->unk66 & 0xC) == 8) || ((obj->unk66 & 0xC) == 0xC)) {
        collision_allocActorLights(obj);
    } else {
        obj->ambients = NULL;
    }
    obj->modelTypeIndex = collision_getSpawnModelTypeIndex(type);
    if (gIsMultiplayer != 0) {
        t = save_getTeam(obj->playernumber);
        if (t >= 4) {
            t = 3;
        }
        if (type == 0x3B) {
            obj->swapTexture[0] = t + 1;
        } else if (type == 0x77) {
            obj->swapTexture[0] = t + 0x10;
        }
    }
    if ((gExpansionPak == 0) && (level != LEVEL_FRONTEND) && (spawn->keepTime == 0xFF)) {
        found = obj->modelCount;
        if (found == 0) {
            found = 1;
        }
        for (i = 0; i < found; i++) {
            gModelUnloadTimers[obj->modelIds[i]] = 0xFF;
        }
    }
    if (level == LEVEL_FRONTEND) {
        obj->unk66 &= ~0x20;
    }
    effect_spawnAttachments(obj);
    model = collision_getModelId(obj);
    found = 0;
    for (i = 0; i < 2; i++) {
        if (model == gVtxMorphGroups[i].kind) {
            found = 1;
            break;
        }
    }
    if (found) {
        mem = allocate_memory(sizeof(VtxMorphState), 1, 0, 2);
        obj->morph = mem;
        if (mem != NULL) {
            mem->prev = 0;
            mem->cur = 0xFF;
            mem->blend = 0.0f;
            mem->rate = 1.0f;
        }
    }
    func_15084C30(obj);
    return ret;
}

void collision_allocActorLights(Actor *obj) {
    SpawnRecord *spawn;
    u8 n;
    s32 i;
    s32 count;

    spawn = obj->listpos_ptr;
    if (spawn != NULL) {
        n = spawn->unk2F;
        if (n == 0) {
            if (obj->playernumber == 0) {
                count = 5;
            } else {
                count = 3;
            }
        } else {
            count = n;
        }
        for (i = 0; i <= gScreenLayout; i++) {
            obj->gfxLights[i] = light_allocGfxLights(count);
        }
        if (obj->gfxLights[i - 1] != 0) {
            obj->maxLights = count;
        }
        obj->ambients = light_allocAmbients();
    }
}

void collision_applyCharacterVariant(Actor *obj, s32 arg1) {
    s32 partSet;
    s32 group;
    ActorCtl *ext;

    partSet = -1;
    switch (arg1) {
    case 37:
    case 38:
        ext = obj->mv;
        if (ext != NULL) {
            ext->unk1B2 = 1;
        }
        break;
    case 9:
        obj->hiddenParts |= 2;
        break;
    case 13:
        partSet = 0x76;
        break;
    case 14:
        partSet = 0x78;
        break;
    case 15:
        partSet = 0x77;
        obj->hiddenParts |= 2;
        break;
    case 29:
    case 30:
    case 31:
    case 32:
        {
            s32 n;

            n = arg1 - 29;
            obj->hiddenParts = 0x1E;
            if (n < 2) {
                obj->hiddenParts &= ~0x14;
            } else {
                obj->hiddenParts &= ~8;
            }
            if (n >> 1) {
                obj->hiddenParts &= ~2;
            }
        }
        break;
    case 17: case 18: case 19: case 20: case 21: case 22:
    case 23: case 24: case 25: case 26: case 27: case 28:
        group = (arg1 - 17) >> 2;
        obj->hiddenParts = 0x3E;
        switch (group) {
        case 0:
            obj->hiddenParts &= ~4;
            break;
        case 1:
            obj->hiddenParts &= ~8;
            break;
        case 2:
            obj->hiddenParts &= ~0x10;
            break;
        }
        switch (arg1 - group * 4 - 17) {
        case 1:
            obj->hiddenParts &= ~2;
            break;
        case 2:
            obj->hiddenParts &= ~0x20;
            break;
        case 3:
            obj->hiddenParts &= ~0x22;
            break;
        }
        break;
    }
    if (partSet != -1) {
        collision_createPartSet(obj, partSet, 0.0f, 0);
    }
}

/* Creates every part of part set setId (1-based) on parent; returns the last part created (a child
 * Actor or a ModelAttachment, depending on its type). */
void *collision_createPartSet(Actor *parent, s32 setId, f32 arg2, s32 arg3) {
    const PartDef *parts;
    void *ret;
    s32 i;

    setId--;
    ret = NULL;
    parts = gPartSets[setId].parts;
    for (i = 0; i < gPartSets[setId].count; i++) {
        if (parts[i].type == 0) {
            ret = collision_createChildObject(parent, parts[i].unk1, parts[i].model, 0, parts[i].unk2, parts[i].unk4, parts[i].unk5, parts[i].unk6, arg2);
        } else {
            ret = attach_add(parent, parts[i].model, parts[i].unk1, parts[i].flags, parts[i].unk4, parts[i].unk5, arg3, setId + 1, parts[i].unk2,
                                parts[i].data, (parts[i].type == 2) ? parts[i].unk6 : -1, parts[i].parentSet, parts[i].parentMtx);
        }
    }
    return ret;
}

/* Removes the child objects of part set setId (1-based) that belong to parent. */
void collision_removePartSet(Actor *parent, s32 setId) {
    const PartSet *set;
    s32 i;
    const PartDef *parts;

    attach_removeSet(parent, setId);
    setId--;
    set = &gPartSets[setId];
    parts = set->parts;
    for (i = 0; i < set->count; i++) {
        Actor *child;

        if (parts[i].type == 0) {
            child = actor_findByType(parts[i].model);
            if (child != NULL) {
                if (child->attach_char == ((parent - movement_obj) + 1)) {
                    actor_remove(child, 0);
                }
            }
        }
    }
}

/* Sets object objIdx's model to modelId (0xFF for none) and loads its per-model state. */
void collision_setObjectModel(s32 objIdx, s32 modelId, s32 arg2) {
    Actor *obj = &movement_obj[objIdx];
    ModelDef *def;

    obj->character_type = modelId;
    if (modelId == 0xFF) {
        obj->bodyRadius = 0;
        obj->bodyHeight = 0;
        obj->bodyOffsetY = 0;
        obj->hitRadius = 70;
        obj->hitHeight = 70;
        obj->hitOffsetY = 0;
        obj->shadow_size = 0;
        obj->rootOffsetY = 0.0f;
    } else {
        obj->rootOffsetY = gModelDefs[modelId]->unk6;
        actor_setScale(obj, obj->scale_xz, obj->scale_y);
        def = gModelDefs[modelId];
        obj->drawtype = def->unk12;
    }
    obj->baseCharacterType = player_getBaseCharacterType(obj);
    if (gModelAnimCounts[obj->baseCharacterType] != 0) {
        obj->animBankTable = rom_findPath(0, 2, 2, obj->baseCharacterType);
    }
}

/* Allocates the animation state block of obj's model. Returns 1 only if the allocation fails. */
s32 collision_allocAnimState(Actor *obj, s32 arg1, s32 arg2, f32 arg3) {
    s32 modelId;

    modelId = obj->character_type;
    if (gModelAnimTables[modelId] == NULL) {
        return 0;
    }
    if (gModelAnimCounts[modelId] == 0) {
        return 0;
    }
    obj->anim = allocate_memory(0x3E0, 1, 2, 0);
    if (obj->anim == NULL) {
        return 1;
    }
    bzero(obj->anim, 0x40);
    actor_playAnim(obj, arg1, arg3, 0.0f, 0.0f, 0.0f, arg2);
    return 0;
}

/* Copies model modelId's defaults from its ModelDef into obj; spawn (may be NULL) overrides unk10. */
void collision_applyModelDefaults(Actor *obj, s32 modelId, SpawnRecord *spawn) {
    ModelDef *def;
    u16 override;

    if (modelId != 0xFF) {
        def = gModelDefs[modelId];
        if (spawn != NULL) {
            override = spawn->unk2C;
            if (override == 0) {
                obj->cullDistance = def->unk2A;
            } else {
                if (override == 1) {
                    obj->cullDistance = 0;
                } else {
                    obj->cullDistance = override;
                }
            }
            if (level == LEVEL_FRONTEND) {
                obj->cullDistance = 1000;
            }
        } else {
            obj->cullDistance = 0;
        }
        obj->unk13B = def->unk39;
        obj->bouncePercent = def->unk33;
        obj->impactSound = def->unk34;
        obj->drawtype = def->unk12;
        obj->swapTexture[0] = def->eyeDefault[0];
        obj->swapTexture[1] = def->eyeDefault[1];
        obj->shadow_size = def->unk2;
        if (def->subModelCount != 0) {
            obj->modelIds = gSubModelLists[def->subModelList];
            obj->lodCount = def->subModelCount;
        }
        obj->modelCount = obj->lodCount + def->extraModelCount;
        if (def->unk29 != 0) {
            aim_initState(obj);
        }
    }
}

/* Creates a child object of parent with model modelId (a part from collision_createPartSet). */
Actor *collision_createChildObject(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8) {
    Actor *obj;
    // matching: unused stack slot
    s32 pad;
    s32 animFlag;
    s32 blocked;
    s32 i;

    model_loadDef(modelId, 0);
    model_loadAnimTable(modelId, 0);
    obj = actor_alloc();
    obj->unique_id = 0xFF - (obj - movement_obj);
    obj->unkAB = arg3;
    obj->type = state;
    actor_resetFade(obj);
    obj->intelwalk = 1;
    obj->energy = 1;
    obj->imune = 0xFF;
    obj->attach_char = (parent - movement_obj) + 1;
    obj->basemtx = arg1;
    obj->noBlobShadow = 1;
    obj->character_type = modelId;
    obj->unk66 = parent->unk66;
    obj->unk66 &= ~0x10;
    // matching: a no-op |= 0 (a constant flag field upstream) keeps the reload of unk66
    obj->unk66 |= 0;
    obj->ambientColor[0] = parent->ambientColor[0];
    obj->ambientColor[1] = parent->ambientColor[1];
    obj->ambientColor[2] = parent->ambientColor[1];
    obj->unk1E0[0] = parent->unk1E0[0];
    obj->unk1E0[1] = parent->unk1E0[1];
    obj->unk1E0[2] = parent->unk1E0[2];
    if (inheritScale == 0) {
        obj->scale_xz = 1.0f;
        obj->scale_y = 1.0f;
        obj->targetScaleXz = 1.0f;
        obj->targetScaleY = 1.0f;
    } else {
        obj->scale_xz = parent->scale_xz;
        obj->scale_y = parent->scale_y;
        obj->targetScaleXz = parent->targetScaleXz;
        obj->targetScaleY = parent->targetScaleY;
    }
    obj->unk101 = inheritScale;
    obj->modelLoadFlags = arg5;
    model_loadDef(modelId, 0);
    if (arg5 & 1) {
        obj->noshove |= 0x4000;
        obj->cull_radius_xz = gModelDefs[modelId]->bodyRadius * obj->scale_xz;
    }
    animFlag = 0;
    if (arg5 & 2) {
        animFlag = 1;
    }
    for (i = 0; i <= gScreenLayout; i++) {
        obj->gfxLights[i] = light_allocGfxLights(1);
    }
    if (obj->gfxLights[i - 1] != 0) {
        obj->maxLights = 1;
    }
    obj->ambients = light_allocAmbients();
    if (gCutsceneStates[0] == 1) {
        blocked = 1;
    } else {
        blocked = 0;
    }
    if (blocked) {
        gCutsceneActorLock = 0;
    }
    collision_applyModelDefaults(obj, modelId, NULL);
    collision_setObjectModel(obj - movement_obj, modelId, 1);
    collision_allocAnimState(obj, arg7, animFlag, arg8);
    if (blocked) {
        gCutsceneActorLock = 1;
    }
    return obj;
}

/* Returns the lowest unique id (1..255) that no spawn record uses. */
s32 collision_getFreeUniqueId(void) {
    s32 id;
    s32 i;

    id = 1;
    for (i = 0; i < gSpawnRecordCount; i++) {
        if (id == gSpawnRecords[i].unique_id) {
            id++;
            i = 0;
        }
    }
    if (id >= 0x100) {
        id = 0xFF;
    }
    return id;
}

/* Returns the index of the spawn record with unique id id, or -1. */
s32 collision_findSpawnById(u8 id) {
    s32 i;

    if (id == 0) {
        return -1;
    }
    for (i = 0; i < gSpawnRecordCount; i++) {
        if (gSpawnRecords[i].unique_id == id) {
            return &gSpawnRecords[i] - gSpawnRecords;
        }
    }
    return -1;
}

/* Returns the active object with unique id id, or NULL. */
Actor *collision_findObjectById(u8 id) {
    Actor *obj;
    s32 i;

    if (id == 0) {
        return NULL;
    }
    // matching: a walking pointer; indexing movement_obj[i] makes IDO unroll the loop differently
    obj = movement_obj;
    for (i = 0; i < 26; i++) {
        if (obj->type != 0 && obj->unique_id == id) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

/* Returns the slot of the active object with unique id id, or -1. */
s32 collision_findObjectSlotById(u8 id) {
    Actor *obj;

    obj = collision_findObjectById(id);
    if (obj != NULL) {
        return obj - movement_obj;
    }
    return -1;
}

s32 collision_findSlotBySpawnIndex(s32 id) {
    Actor *obj = actor_findOnPath(id);

    if (obj != NULL) {
        return obj - movement_obj;
    }
    return -1;
}

/* Sets up obj's two data pointers for its model'th model: shared with gModelData[id] or private
   copies, depending on the owning spawn record's mode (unk10 & 3). Returns 1 if an allocation failed. */
s32 collision_initModelData(Actor *obj, s32 model) {
    u8 *slot;
    SpawnRecord *spawn;
    s32 mode;
    u8 id;
    s32 val;
    s32 ret;
    s32 i;
    s32 size;

    // matching: every access to modelData[model] goes through this byte pointer (slot + 0x28C/0x290) or
    // an s32 row view, as s32 lvalues; typed field access reallocates registers (see the collision_initModelData ledger)
    slot = (u8 *)((s32)obj + model * 8);
    if (obj->attach_char != 0) {
        spawn = movement_obj[obj->attach_char - 1].listpos_ptr;
    } else {
        spawn = obj->listpos_ptr;
    }
    id = obj->modelIds[model];
    mode = (spawn != NULL) ? (spawn->unk10 & 3) : 0;
    if (id == 0) {
        mode = 2;
    }
    if (*(s32 *)(slot + 0x28C) != 0 && *(s32 *)(slot + 0x290) != 0) {
        return 0;
    }
    val = (s32)gModelData[id];
    if ((s32)gModelData[id] == 0) {
        return 0;
    }
    ret = 0;
    if (mode == 0) {
        val = (s32)gModelData[id];
        *(s32 *)(slot + 0x290) = val;
        *(s32 *)(slot + 0x28C) = val;
        D_800D1F80[id] += 2;
    } else if (mode == 1) {
        if (*(s32 *)(slot + 0x28C) == 0 || *(s32 *)(slot + 0x28C) == (s32)gModelData[id]) {
            if (D_800D1F80[id] != 0) {
                size = gModelVertexCounts[id] * 16;
                *(s32 *)(slot + 0x28C) = (s32)allocate_memory(size, 1, 2, 2);
                if (*(s32 *)(slot + 0x28C) == 0) {
                    ret = 1;
                } else if (*(s32 *)(slot + 0x28C) != 0) {
                    bcopy((void *)(s32)gModelData[id], (void *)*(s32 *)(slot + 0x28C), size);
                } else {
                    *(s32 *)(slot + 0x28C) = (s32)gModelData[id];
                }
            } else {
                val = (s32)gModelData[id];
                *(s32 *)(slot + 0x290) = val;
                *(s32 *)(slot + 0x28C) = val;
                D_800D1F80[id] += 2;
            }
        }
        *(s32 *)(slot + 0x290) = *(s32 *)(slot + 0x28C);
    } else if (mode == 2) {
        for (i = 0; i < 2; i++) {
            if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0 || ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == (s32)gModelData[id] || (i == 1 && *(s32 *)(slot + 0x28C) == *(s32 *)(slot + 0x290))) {
                if (D_800D1F80[id] != 0) {
                    size = gModelVertexCounts[id] * 16;
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)allocate_memory(size, 1, 2, 2);
                    if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0) {
                        ret = 1;
                        if (i == 1) {
                            if (*(s32 *)(slot + 0x28C) != (s32)gModelData[id]) {
                                heap_free((void *)*(s32 *)(slot + 0x28C));
                            } else {
                                D_800D1F80[id] -= 1;
                            }
                            *(s32 *)(slot + 0x28C) = 0;
                        }
                        break;
                    } else if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] != 0) {
                        bcopy((void *)(s32)gModelData[id], (void *)((s32 (*)[2])((u8 *)obj + 0x28C))[model][i], size);
                    } else {
                        ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)gModelData[id];
                    }
                } else {
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)gModelData[id];
                    D_800D1F80[id] += 1;
                }
            }
        }
    }
    return ret;
}

/* Releases the data pointers of all of obj's models. */
void collision_releaseAllModelData(Actor *obj) {
    s32 count;
    s32 i;

    count = obj->modelCount;
    if (count == 0) {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        collision_releaseModelData(obj, i);
    }
}

/* Releases the data pointers of obj's model'th model: frees private copies, drops the
   reference count of shared ones. */
void collision_releaseModelData(Actor *obj, s32 model) {
    // matching: a named trip count takes its saved register before the gModelData base
    u8 limit;
    u8 *data;
    s32 i;
    u8 id;

    if (obj->modelData[model][0] == NULL && obj->modelData[model][1] == NULL) {
        return;
    }
    limit = 2;
    id = obj->modelIds[model];
    for (i = 0; i != limit; i++) {
        data = obj->modelData[model][i];
        if (data != NULL) {
            if (data != gModelData[id]) {
                heap_setTag((s32 *)data, 3);
            } else {
                D_800D1F80[id] -= 1;
            }
            obj->modelData[model][i] = NULL;
        }
    }
}

/* Loads model spawn->type and all of its sub-models. */
void collision_loadSpawnModel(SpawnRecord *spawn, s32 arg1, s32 arg2) {
    s32 count;
    s32 i;
    ModelDef *def;
    const u8 *list;
    s32 modelId;

    modelId = spawn->type;
    if (modelId != 0xFF) {
        model_loadDef(modelId, arg2);
        def = gModelDefs[modelId];
        list = &spawn->type;
        count = def->subModelCount;
        if (count == 0) {
            count = 1;
        } else {
            list = gSubModelLists[def->subModelList];
        }
        count += def->extraModelCount;
        for (i = 0; i < count; i++) {
            model_loadDef(list[i], arg2);
            model_loadAnimTable(list[i], arg2);
        }
    }
}

/* Releases obj's model data, and frees each of its models (and their tables) that no other
   active object still uses. */
void collision_freeObjectModels(Actor *self) {
    s32 id;
    const u8 *list;
    s32 i;
    s32 j;
    s32 count;
    s32 k;
    s32 unique;
    s32 freeData;
    s32 freeDef;
    Actor *obj;
    u8 otherCount;
    const u8 *otherList;
    // matching: unused stack slot
    s32 pad;
    ActorAnim *data;
    u8 *block;
    TexCoord *table;

    id = self->character_type;
    if (id != 0xFF && gModelData[id] != NULL) {
        count = self->modelCount;
        list = self->modelIds;
        collision_releaseAllModelData(self);
        for (i = 0; i < count; i++) {
            unique = 1;
            freeData = 1;
            freeDef = 1;
            // matching: IDO emits this empty duplicate scan; it must stay
            for (j = 0; j < i; j++) {
                if (list[j] == list[i]) {
                }
            }
            for (j = 0; j < 25; j++) {
                if (j != (self - movement_obj)) {
                    obj = &movement_obj[j];
                    if (obj->type != 0 && obj->character_type != 0xFF && obj->drawtype != 2) {
                        otherCount = obj->modelCount;
                        otherList = obj->modelIds;
                        for (k = 0; k < otherCount; k++) {
                            if (otherList[k] == list[i]) {
                                unique = 0;
                            }
                            if (gModelAnimTables[otherList[k]] == gModelAnimTables[list[i]] && gModelAnimTables[otherList[k]] != NULL) {
                                freeData = 0;
                            }
                            if (gModelDefs[otherList[k]] == gModelDefs[list[i]] && gModelDefs[otherList[k]] != NULL) {
                                freeDef = 0;
                            }
                        }
                    }
                }
            }
            if (freeData && gModelAnimTables[list[i]] != NULL) {
                data = gModelAnimTables[list[i]];
                {
                    heap_setTag((s32 *)ANIM_HEADER(data), 3);
                    for (k = 0; k < 187; k++) {
                        if (data == gModelAnimTables[k]) {
                            gModelAnimTables[k] = NULL;
                        }
                    }
                }
            }
            if (freeDef) {
                block = (u8 *)gModelDefs[list[i]];
                if (block != NULL) {
                    block -= 0x10;  // the allocation starts 0x10 bytes before the data
                    heap_setTag((s32 *)block, 3);
                    gModelDefs[list[i]] = NULL;
                }
            }
            if (unique) {
                if (gModelData[list[i]] != NULL) {
                    if (gModelVtxRuns[id] != NULL) {
                        heap_free((void *)(s32)gModelVtxRuns[id]); // matching: the int round trip keeps the load in v0
                        gModelVtxRuns[id] = NULL;
                        heap_free(gModelTris[id]);
                        gModelTris[id] = NULL;
                    }
                    table = gModelSavedTexCoords[id];
                    if (table != NULL) {
                        heap_free(table);
                        gModelSavedTexCoords[id] = NULL;
                    }
                }
            }
        }
    }
}

/* Returns obj's current model id (modelIds[forced_lod - 1], or the first). */
s32 collision_getModelId(Actor *obj) {
    s32 current = obj->forced_lod;

    if (current != 0) {
        return obj->modelIds[current - 1];
    }
    return obj->modelIds[0];
}

/* Like collision_getModelId, falling back to modelIds[lodCount - 1]; stores the index in *outIndex. */
s32 collision_getLodModelId(Actor *obj, s32 *outIndex) {
    s32 index;
    s32 n;

    n = obj->forced_lod;
    if (n != 0) {
        index = n - 1;
    } else {
        n = obj->lodCount;
        if (n != 0) {
            index = n - 1;
        } else {
            index = 0;
        }
    }
    if (outIndex != NULL) {
        *outIndex = index;
    }
    return obj->modelIds[index];
}

/* Ticks the gModelUnloadTimers countdowns; a model whose countdown expires while it has no gModelVtxRuns
   table is unloaded and every object's references to it are dropped. */
void collision_updateModelUnload(void) {
    s32 i;
    s32 m;
    Actor *obj;
    s32 j;
    s32 k;
    ObjTexture *textures;
    u8 *timer;

    timer = gModelUnloadTimers;
    for (i = 0; i < 187; i++, timer++) {
        if (*timer == 0xFF || *timer == 0) {
            continue;
        }
        if (*timer == 1) {
            if (gModelData[i] != NULL && gModelVtxRuns[i] == NULL) {
                for (m = 0; m < 25; m++) {
                    obj = &movement_obj[m];
                    if (obj->type != 0) {
                        for (j = 0; j < obj->modelCount; j++) {
                            for (k = 0; k < 2; k++) {
                                if (obj->modelData[j][k] == gModelData[i]) {
                                    obj->modelData[j][k] = NULL;
                                }
                            }
                            if (i == obj->modelIds[j]) {
                                collision_releaseModelData(obj, j);
                            }
                        }
                    }
                }
                textures = gModelTextures[i];
                for (k = 0; k < gModelTextureCounts[i]; k++) {
                    if (textures[k].addr >= 0x10000000U) {
                        texture_dropUse(textures[k].id);
                    }
                }
                heap_free(gModelData[i] - 0x38);
                D_800D1F80[i] = 0;
                gModelData[i] = NULL;
            }
        }
        (*timer)--;
    }
}

void func_15084C30(Actor *obj) {
    AnimInstance *anim;
    f32 phase;

    if (obj->character_type == 0x94) {
        anim = obj->anim;
        phase = math_random() % (s32)anim->numFrames[0];
        anim->frame[0] = phase;
    }
}

/* Returns the index of modelId in gSpawnModelTypes, or 0. */
s32 collision_getSpawnModelTypeIndex(s32 modelId) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < gSpawnModelTypeCount; i++) {
        if (modelId == gSpawnModelTypes[i]) {
            ret = i;
            break;
        }
    }
    return ret;
}
