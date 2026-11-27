#include <ultra64.h>
#include "structs.h"

/* A pointer fixup: `offset` is relative to its block until relocated; bit 31 of `flags` ends a list. */
typedef struct RelocPair {
    /* 0x0 */ s32 offset;
    /* 0x4 */ u32 flags;
} RelocPair;

#define RELOC_LAST 0x80000000

/* Header of a loaded resource block; it starts with its own fixup list (0x0/0x4, 0x8/0xC). */
typedef struct LoadedResource {
    /* 0x0 */ s16 *unk0;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ s16 *dependencies;
    /* 0xC */ u32 dependencyCount;   // stored doubled in the file
} LoadedResource;

/* The loaded resource table: slot 0's unk4 holds the slot count, slots 1.. point at resource blocks. */
typedef struct ResourceSlot {
    /* 0x0 */ LoadedResource *data;
    /* 0x4 */ s32 unk4;
} ResourceSlot;

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
u8 D_800D2FA0[0x10]; /* not referenced */
static ResourceSlot *gResourceTable;

#include "functions.h"
#include "variables.h"


#define RESOURCE_STATE_LOADED 3

/* Per-resource "loaded" bitset and per-resource state bytes. */
#define gResourceLoadedBits gTaskDoneBits
#define gResourceStates gTaskStates
#define RESOURCE_IS_LOADED(idx) (gResourceLoadedBits[(idx) >> 3] & (1 << ((idx) & 7)))

/* Returns the name of resource `index`, or NULL for an empty slot. */
const char *script_getTaskName(s32 index) {
    return D_80087430[index].name;
}

u16 script_getTaskLevel(s32 index) {
    return D_80087430[index].levelId;
}

s32 script_getTaskSpawnPoint(s32 index) {
    return D_80087430[index].packedSpawnPoint & 0xFFF;
}

s32 script_getTaskPackedId(s32 index) {
    return D_80087430[index].packedId;
}

/* Finds the resource whose id is `id`; returns RESOURCE_COUNT if none. */
s32 script_findTaskIndex(s32 id) {
    s32 i = 0; // matching: initialising here schedules the i = 0 move before the loop setup

    for (; i < RESOURCE_COUNT; i++) {
        if (D_80087430[i].name != NULL && id == (D_80087430[i].packedId & 0x1FFFFFFF) + 1) {
            return i;
        }
    }
    return RESOURCE_COUNT;
}

/* Counts the descriptor slots that are in use. */
s32 script_countTasks(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < RESOURCE_COUNT; i++) {
        if (D_80087430[i].name != NULL) {
            count++;
        }
    }
    return count;
}

/* Returns 1 if every dependency of `index` is loaded, recursively. */
s32 script_areTaskDepsDone(s32 index) {
    const u16 *deps;
    s32 depsLoaded;
    s32 i;

    deps = D_80087430[index].dependencies;
    for (i = 0; deps[i] != RESOURCE_LIST_END; i++) {
        depsLoaded = script_areTaskDepsDone(deps[i]);
        if (!RESOURCE_IS_LOADED(deps[i]) || depsLoaded == 0) {
            return 0;
        }
    }
    return 1;
}


void func_1509CC94(s32 index) {
    script_markTaskDepsDone(index);
}

/* Marks every dependency of `index` loaded, depth first. */
void script_markTaskDepsDone(s32 index) {
    script_markTasksDone(D_80087430[index].dependencies, script_markTaskDepsDone);
}

/*
 * Marks each related dependency of `index` loaded; the ones not yet in the loaded state
 * also get their own dependencies marked. Returns how many changed state.
 */
s32 script_markRelatedTasksDone(s32 index) {
    const u16 *p;
    s32 count;

    count = 0;
    p = D_80087430[index].relatedDependencies;
    while (*p != RESOURCE_LIST_END) {
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        if (gResourceStates[*p] != RESOURCE_STATE_LOADED) {
            gResourceStates[*p] = RESOURCE_STATE_LOADED;
            script_markTaskDepsDone(*p);
            count++;
        }
        p++;
    }
    return count;
}

/* Marks `index` and, until nothing changes, everything reachable from loaded resources. */
void script_propagateTasksDone(s32 index) {
    s32 i;
    s32 changed;

    script_markRelatedTasksDone(index);
    do {
        changed = 0;
        for (i = 0; i < RESOURCE_COUNT; i++) {
            if (gResourceStates[i] == RESOURCE_STATE_LOADED) {
                changed += script_markRelatedTasksDone(i);
            }
        }
    } while (changed != 0);
}

/* Calls `callback` on each index in `list`, then marks that index loaded. */
void script_markTasksDone(const u16 *list, void (*callback)(s32)) {
    const u16 *p;

    p = list;
    while (*p != RESOURCE_LIST_END) {
        callback(*p);
        gResourceStates[*p] = RESOURCE_STATE_LOADED;
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        p++;
    }
}


/* Loads the resource table, relocates every block in it and returns its resource count in *countOut. */
s32 script_loadResourceTable(s32 arg0, s32 *countOut) {
    ResourceSlot *table;
    u32 i;
    s32 done;
    s32 count;
    RelocPair *reloc;
    u32 flags;
    s32 pad[2]; // matching: frame size

    count = 0;
    gResourceTable = rom_loadPathTable(0, 0, &count, 3, 8, 0, arg0);
    if (gResourceTable == NULL) {
        *countOut = 0;
        return 0;
    }
    table = gResourceTable;
    table[0].unk4 = count - 1;
    *countOut = count - 1;
    for (i = 1; i < count; i++) {
        done = 0;
        reloc = (RelocPair *)gResourceTable[i].data;
        do {
            reloc->offset += (s32)gResourceTable[i].data;
            flags = reloc->flags;
            if (flags & RELOC_LAST) {
                if (1) { // matching: -g3 block keeps IDO's register order
                    done = 1;
                }
            }
            reloc->flags = flags & 0x0FFFFFFF;
            reloc++;
        } while (!done);
        // matching: indexing from &table[1] keeps IDO's stepped table pointer
        (&table[1])[i - 1].data->dependencyCount = ((&table[1])[i - 1].data->dependencyCount & 0x0FFFFFFF) >> 1;
    }
    return 1;
}

/* Frees the loaded resource table. */
void script_freeResourceTable(void) {
    ResourceSlot *table;

    if (gResourceTable != NULL) {
        table = gResourceTable;
        heap_free(table);
        gResourceTable = NULL;
    }
}

/*
 * Returns 1 if every dependency of loaded resource `index` is loaded. Also reports its first
 * dependency (0xBF if missing) and the first halfword of its unk0 list (0 if missing).
 */
s32 script_areResourceDepsLoaded(s32 unused, s32 index, s32 *firstDepOut, s32 *unk0Out) {
    ResourceSlot *table;
    ResourceSlot *slot;
    LoadedResource *resource;
    s16 *deps;
    u32 i;
    u32 count;

    table = gResourceTable;
    slot = ((ResourceSlot (*)[1])table)[index]; // matching: row view keeps IDO's base+offset operand order
    if (table == NULL) {
        if (firstDepOut != NULL) {
            *firstDepOut = 0xBF;
        }
        if (unk0Out != NULL) {
            *unk0Out = 0;
        }
        return 0;
    }
    resource = slot[1].data;
    if (resource == NULL) {
        if (firstDepOut != NULL) {
            *firstDepOut = 0xBF;
        }
        if (unk0Out != NULL) {
            *unk0Out = 0;
        }
        return 0;
    }
    if (firstDepOut != NULL) {
        *firstDepOut = resource->dependencies[0];
        resource = slot[1].data;
    }
    if (unk0Out != NULL) {
        *unk0Out = resource->unk0[0];
        resource = slot[1].data;
    }
    i = 0;
    count = resource->dependencyCount;
    if (count != 0) {
        deps = resource->dependencies;
        do {
            i++;
            if (!RESOURCE_IS_LOADED(*deps)) {
                return 0;
            }
            deps++;
        } while (i < count);
    }
    return 1;
}

static const u8 D_8009F0D0[1] = { 0 };

/* Starts script command `cmdId` with arguments `cmd`; returns a handle for commands 4-6. */
s32 script_startEffect(s32 cmdId, ScriptCmd *cmd) {
    s32 ret;
    Actor *obj;

    ret = 0;
    switch (cmdId) {
        case 51:
            func_150BD070(0xFF, 1);
            break;
        case 49:
            effect_turnLightsOn();
            break;
        case 48:
            effect_startLineEmitter(0xFF, 1);
            break;
        case 47:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                func_15108120(obj, 0xFF, 1);
            }
            break;
        case 46:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                particle_spawnFloorImpactGreenFlash(obj, 0xFF, 1);
            }
            break;
        case 44:
            effect_turnLightsOff();
            break;
        case 42:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                effect_spawnActorBeam(obj, 0xFF, 1);
            }
            break;
        case 43:
            effect_setupWarpModelOnce();
            break;
        case 37:
            effect_showPlayerMarker(CMD_OBJECT_ID(cmd->arg), 120, cmd->a[0].w);
            break;
        case 36:
            {
                Actor *target;

                target = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
                if (target != NULL) {
                    effect_attach(beam_spawnCastBeam(target), target, 27);
                }
            }
            break;
        case 34:
            {
                s32 pad[2]; // matching: unused, keeps the frame size and floor's stack offset
                CollisionHit floor;
                Vec3f pos;

                floor.height = -10000.0f;
                floor.material = 0;
                floor.flags = 0;
                floor.source = 0;
                floor.surface = NULL;
                pos.x = cmd->arg.w;
                pos.y = 20000.0f;
                pos.z = cmd->a[0].w;
                if (collision_probeFloorSurfABSet0Cached(&pos, 0, floor.height, &floor) != 0) {
                    { // matching: -g3 block keeps the float stores out of the branch delay slot
                        Vec3f dir;

                        pos.y = floor.height;
                        dir.x = 0.0f;
                        dir.z = 0.0f;
                        dir.y = -1.0f;
                        weapon_spawnSurfaceHit(&floor.tri, &pos, &dir, 1, 0xFF, 1);
                    }
                }
            }
            break;
        case 35:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            blood_spawnHitEffects(obj, 1, 1, 1, 1, 0, 0xFF, 1);
            break;
        case 33:
            {
                s32 pad[2]; // matching: frame size
                Vec3f pos;

                obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
                if (obj != NULL) {
                    pos.x = obj->xpos;
                    pos.y = obj->ypos + 50.0f;
                    pos.z = obj->zpos;
                    weapon_spawnObjectHit(&pos, obj, 1, 0xFF, 1);
                }
            }
            break;
        case 27:
            effect_eruptSprayAreas();
            break;
        case 24:
            {
                u8 values[8];

                values[0] = D_8009F0D0[0];
                obj = collision_findObjectById(cmd->arg.b.lo);
                // matching: the no-op ternary orders the argument moves
                particle_startAnchorEmitter(obj ? obj : obj, values[cmd->a[0].w], cmd->a[1].h.lo, -1, -1, -1, 0, 0, 0xFF, 1);
            }
            break;
        case 20:
            effect_spawnPropEmitter(cmd->arg.b.lo, 0xFF, 1);
            break;
        case 18:
            trail_startStream(cmd->arg.w);
            break;
        case 16:
            music_postEvent(16, 0);
            light_sendEvent(22, 31, 0);
            break;
        case 0:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            effect_spawnOwnerSpark(obj, cmd->a[0].h.lo, 0xFF, 1);
            break;
        case 1:
            prop_startMirroredEmitters(cmd->arg.w);
            return 1;
        case 2:
            D_800D9920 = 1;
            break;
        case 3:
            effect_requestRainState(cmd->arg.w);
            break;
        case 4:
            ret = effect_startHomingToPoint(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w,
                                cmd->a[4].w, cmd->a[5].w, cmd->a[6].w);
            if (ret != 0) {
                ret = (ret << 6) | 0xE000 | 4;
            }
            break;
        case 5:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->a[2]));
            if (obj != NULL) {
                ret = effect_startHomingOnActor(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, obj, cmd->a[3].w, cmd->a[5].w);
                if (ret != 0) {
                    s32 pad; // matching: -g3 block keeps the handle store in the branch delay slot
                    ret = (ret << 6) | 0xE000 | 4;
                }
            }
            break;
        case 6:
            ret = effect_startHomingWithSound(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w,
                                cmd->a[4].w, movement_obj, cmd->a[5].w, cmd->a[6].w);
            if (ret != 0) {
                ret = (ret << 6) | 0xE000 | 4;
            }
            break;
        case 7:
            {
                s32 pad[2]; // matching: frame size
                Vec3f pos;

                if (cmd->arg.w == 999) {
                    pos.x = cmd->a[0].w;
                    pos.y = cmd->a[1].w;
                    pos.z = cmd->a[2].w;
                    effect_spawnExplosion(&pos, 0xFF, 1);
                } else {
                    func_150E81A8(cmd->arg.w & 0xFF, 0xFF, 1);
                }
            }
            break;
        case 15:
            particle_fireShake();
            break;
        case 19:
            screen_spawnOverlay(cmd->arg.h.lo);
            break;
        case 22:
            light_sendEvent(35, 39, 0);
            light_sendEvent(36, 39, 0);
            break;
        case 23:
            light_createObjectPulse(cmd->arg.b.lo, 0xFF, 1);
            break;
        case 32:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            particle_spawnBreakup(obj, 0xFF, 1);
            break;
    }
    return ret;
}



/* Ends script command `cmdId` started by script_startEffect. */
s32 script_endEffect(s32 cmdId, ScriptCmd *cmd) {
    Actor *obj;

    switch (cmdId) {
        case 41:
            func_150EB8C4();
            break;
        case 39:
            effect_jumpStagesToEnd(cmd->arg.b.lo);
            break;
        case 38:
            light_sendEvent(41, 24, 0);
            break;
        case 36:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                beam_sendOwnerGone(obj);
            }
            break;
        case 26:
            light_sendEvent(38, 24, 0);
            break;
        case 18:
            trail_stopStream(cmd->arg.w);
            break;
        case 2:
            D_800D9920 = 0;
            break;
        case 4:
        case 5:
        case 6:
            effect_stopHomingEffect(cmdId >> 6);
            break;
        case 8:
            func_150B3DD0();
            break;
        case 9:
            obj = actor_findOnPath(CMD_OBJECT_ID(cmd->arg));
            if (obj == NULL) {
                return 0;
            }
            beam_sendStripMessage2(obj);
            break;
        case 10:
            prop_destroyLevelSprays();
            break;
        case 19:
            screen_fadeOutOverlays(cmd->arg.h.lo);
            break;
        case 21:
            light_sendEvent(32, 24, 0);
            break;
        case 22:
            light_sendEvent(35, 40, 0);
            light_sendEvent(36, 40, 0);
            break;
    }
}

typedef struct Msg1509D8FC {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} Msg1509D8FC;


/* Advances script command `cmdId` to `phase`. */
s32 script_effectCommand(s32 cmdId, s32 phase, ScriptCmd *cmd) {
    switch (cmdId) {
        case 52:
            if (phase == 3) {
                flamethrower_stopJets(actor_findOnPath(CMD_OBJECT_ID(cmd->arg)));
            }
            break;

        case 50:
            if (phase == 2) {
                particle_broadcastToQuadParticles(0, 83);
            } else if (phase == 3) {
                particle_broadcastToQuadParticles(0, 84);
            }
            break;
        case 45:
            light_setDaylight(phase == 6);
            break;
        case 40:
            if (phase == 2) {
                light_setGroupDisabled(4, 1);
                light_setGroupDisabled(5, 0);
            } else if (phase == 3) {
                light_setGroupDisabled(4, 0);
                light_setGroupDisabled(5, 1);
            }
            break;
        case 31:
            if (phase == 5) {
                u8 buf[8];
                buf[0] = cmd->a[0].w;
                particle_broadcastToQuadParticles((s32)buf, 54);
            }
            // fallthrough
        case 30:
            if (phase == 2) {
                u8 buf2[4];
                buf2[0] = cmd->a[0].w;
                effect_sendEventToTimedEffects((s32)buf2, 52);
            }
            break;
        case 29:
            if (phase == 5) {
                Msg1509D8FC msg;
                msg.unk0 = cmd->a[1].w;
                msg.unk4 = cmd->a[0].w;
                msg.unk5 = cmd->a[2].w;
                msg.unk6 = cmd->a[3].w;
                msg.unk7 = cmd->a[4].w;
                effect_sendEventToType(53, (s32)&msg, 51);
            }
            break;
        case 28:
            if (phase == 5) {
                s32 idx = cmd->a[0].w;
                if (idx < 0 || idx >= 3) {
                    return 1;
                }
                effect_sendEventToTimedEffects((s32)&gRaySpinnerSpinPresets[idx], 50);
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14: {
            s32 type;
            u8 index;
            u8 param;

            type = cmdId & 0x3F;
            if (type == 11) {
                index = 1;
            } else if (type == 12) {
                index = 2;
            } else if (type == 13) {
                index = 3;
            } else if (type == 14) {
                index = 4;
            } else {
                index = 0;
            }
            if (phase == 2) {
                param = 23;
            } else if (phase == 3) {
                param = 24;
            } else {
                param = 23;
            }
            light_sendEvent(index, param, 0);
            break;
        }
        case 17:
            effect_sendLightAndGlowEvent(cmd->a[0].w, phase == 3);
            break;
    }
}

typedef struct Node1509DBBC {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct Node1509DBBC *next;
    /* 0x0C */ u8 padC[0x3C];
    /* 0x48 */ u8 type;
} Node1509DBBC;

/* Polls script command `cmdId` in `phase`; returns its status or query value. */
s32 script_effectQuery(s32 cmdId, s32 phase, s32 arg2) {
    s32 hi;
    s32 result;
    s32 lo;
    s32 handle;

    hi = 0;
    result = 0;
    lo = 0;
    switch (cmdId) {
        case 25:
            if (phase == 4) {
                u8 i;
                s8 j;
                Node1509DBBC *node;

                /* Done once no node of type 2 is left in the watched lists. */
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        node = (Node1509DBBC *)gObjectLists[i][gModelEffectTypes[j]];
                        while (node != NULL) {
                            Node1509DBBC *next;

                            next = node->next;
                            if (node->type == 2) {
                                return 0;
                            }
                            node = next;
                        }
                    }
                }
                return 1;
            }
            break;
        case 4:
        case 5:
        case 6:
            if (phase == 0 || phase == 1) {
                handle = cmdId >> 6;
                if (handle != 0) {
                    effect_getHomingEffectPos(handle, &hi, &result, &lo);
                    if (phase == 0) {
                        result = (hi << 16) | (lo & 0xFFFF);
                    }
                    return result;
                }
            }
            break;
    }
}
