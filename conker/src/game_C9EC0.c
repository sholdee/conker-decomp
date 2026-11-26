#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* One 0x14-byte record per resource/task; D_80087430 holds RESOURCE_COUNT of them. */
typedef struct ResourceDescriptor {
    /* 0x00 */ const char *name;            // NULL for unused slots
    /* 0x04 */ s32 packedId;                // low 29 bits: id - 1
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;                    // low 12 bits used
    /* 0x0C */ u16 *dependencies;           // 0xFFFF-terminated resource indices
    /* 0x10 */ u16 *relatedDependencies;    // 0xFFFF-terminated resource indices
} ResourceDescriptor; // size = 0x14

#define RESOURCE_COUNT 204
#define RESOURCE_LIST_END 0xFFFF
#define RESOURCE_STATE_LOADED 3

extern ResourceDescriptor D_80087430[RESOURCE_COUNT];

/* Per-resource "loaded" bitset and per-resource state bytes. */
#define gResourceLoadedBits D_800D2E4C
#define gResourceStates D_800D2E70
#define RESOURCE_IS_LOADED(idx) (gResourceLoadedBits[(idx) >> 3] & (1 << ((idx) & 7)))

/* Returns the name of resource `index`, or NULL for an empty slot. */
const char *func_1509CA10(s32 index) {
    return D_80087430[index].name;
}

u16 func_1509CA30(s32 index) {
    return D_80087430[index].unk8;
}

s32 func_1509CA50(s32 index) {
    return D_80087430[index].unkA & 0xFFF;
}

s32 func_1509CA78(s32 index) {
    return D_80087430[index].packedId;
}

/* Finds the resource whose id is `id`; returns RESOURCE_COUNT if none. */
s32 func_1509CA98(s32 id) {
    s32 i = 0; // matching: initialising here schedules the i = 0 move before the loop setup

    for (; i < RESOURCE_COUNT; i++) {
        if (D_80087430[i].name != NULL && id == (D_80087430[i].packedId & 0x1FFFFFFF) + 1) {
            return i;
        }
    }
    return RESOURCE_COUNT;
}

/* Counts the descriptor slots that are in use. */
s32 func_1509CB68(void) {
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
s32 func_1509CBD4(s32 index) {
    u16 *deps;
    s32 depsLoaded;
    s32 i;

    deps = D_80087430[index].dependencies;
    for (i = 0; deps[i] != RESOURCE_LIST_END; i++) {
        depsLoaded = func_1509CBD4(deps[i]);
        if (!RESOURCE_IS_LOADED(deps[i]) || depsLoaded == 0) {
            return 0;
        }
    }
    return 1;
}


void func_1509CC94(s32 index) {
    func_1509CCB4(index);
}

/* Marks every dependency of `index` loaded, depth first. */
void func_1509CCB4(s32 index) {
    func_1509CE64(D_80087430[index].dependencies, func_1509CCB4);
}

/*
 * Marks each related dependency of `index` loaded; the ones not yet in the loaded state
 * also get their own dependencies marked. Returns how many changed state.
 */
s32 func_1509CCF4(s32 index) {
    u16 *p;
    s32 count;

    count = 0;
    p = D_80087430[index].relatedDependencies;
    while (*p != RESOURCE_LIST_END) {
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        if (gResourceStates[*p] != RESOURCE_STATE_LOADED) {
            gResourceStates[*p] = RESOURCE_STATE_LOADED;
            func_1509CCB4(*p);
            count++;
        }
        p++;
    }
    return count;
}

/* Marks `index` and, until nothing changes, everything reachable from loaded resources. */
void func_1509CDDC(s32 index) {
    s32 i;
    s32 changed;

    func_1509CCF4(index);
    do {
        changed = 0;
        for (i = 0; i < RESOURCE_COUNT; i++) {
            if (gResourceStates[i] == RESOURCE_STATE_LOADED) {
                changed += func_1509CCF4(i);
            }
        }
    } while (changed != 0);
}

/* Calls `callback` on each index in `list`, then marks that index loaded. */
void func_1509CE64(u16 *list, void (*callback)(s32)) {
    u16 *p;

    p = list;
    while (*p != RESOURCE_LIST_END) {
        callback(*p);
        gResourceStates[*p] = RESOURCE_STATE_LOADED;
        gResourceLoadedBits[*p >> 3] |= 1 << (*p & 7);
        p++;
    }
}

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

extern ResourceSlot *D_800D2FB0;

/* Loads the resource table, relocates every block in it and returns its resource count in *countOut. */
s32 func_1509CF28(s32 arg0, s32 *countOut) {
    ResourceSlot *table;
    u32 i;
    s32 done;
    s32 count;
    RelocPair *reloc;
    u32 flags;
    s32 pad[2]; // matching: frame size

    count = 0;
    D_800D2FB0 = func_1502B6BC(0, 0, &count, 3, 8, 0, arg0);
    if (D_800D2FB0 == NULL) {
        *countOut = 0;
        return 0;
    }
    table = D_800D2FB0;
    table[0].unk4 = count - 1;
    *countOut = count - 1;
    for (i = 1; i < count; i++) {
        done = 0;
        reloc = (RelocPair *)D_800D2FB0[i].data;
        do {
            reloc->offset += (s32)D_800D2FB0[i].data;
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
void func_1509D054(void) {
    ResourceSlot *table;

    if (D_800D2FB0 != NULL) {
        table = D_800D2FB0;
        func_10004074(table);
        D_800D2FB0 = NULL;
    }
}

/*
 * Returns 1 if every dependency of loaded resource `index` is loaded. Also reports its first
 * dependency (0xBF if missing) and the first halfword of its unk0 list (0 if missing).
 */
s32 func_1509D08C(s32 unused, s32 index, s32 *firstDepOut, s32 *unk0Out) {
    ResourceSlot *table;
    ResourceSlot *slot;
    LoadedResource *resource;
    s16 *deps;
    u32 i;
    u32 count;

    table = D_800D2FB0;
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

extern u8 D_8009F0D0;

/* Starts script command `cmdId` with arguments `cmd`; returns a handle for commands 4-6. */
s32 func_1509D180(s32 cmdId, ScriptCmd *cmd) {
    s32 ret;
    Actor *obj;

    ret = 0;
    switch (cmdId) {
        case 51:
            func_150BD070(0xFF, 1);
            break;
        case 49:
            func_150FAD28();
            break;
        case 48:
            func_150FAA40(0xFF, 1);
            break;
        case 47:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                func_15108120(obj, 0xFF, 1);
            }
            break;
        case 46:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                func_150ECC00(obj, 0xFF, 1);
            }
            break;
        case 44:
            func_150FAD78();
            break;
        case 42:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                func_150FCFD4(obj, 0xFF, 1);
            }
            break;
        case 43:
            func_150E0300();
            break;
        case 37:
            func_151557FC(CMD_OBJECT_ID(cmd->arg), 120, cmd->a[0].w);
            break;
        case 36:
            {
                Actor *target;

                target = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
                if (target != NULL) {
                    func_1514EC1C(func_150F6DE4(target), target, 27);
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
                if (func_15046C80(&pos, 0, floor.height, &floor) != 0) {
                    { // matching: -g3 block keeps the float stores out of the branch delay slot
                        Vec3f dir;

                        pos.y = floor.height;
                        dir.x = 0.0f;
                        dir.z = 0.0f;
                        dir.y = -1.0f;
                        func_151BFE84(&floor.tri, &pos, &dir, 1, 0xFF, 1);
                    }
                }
            }
            break;
        case 35:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            func_15136C3C(obj, 1, 1, 1, 1, 0, 0xFF, 1);
            break;
        case 33:
            {
                s32 pad[2]; // matching: frame size
                Vec3f pos;

                obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
                if (obj != NULL) {
                    pos.x = obj->xpos;
                    pos.y = obj->ypos + 50.0f;
                    pos.z = obj->zpos;
                    func_151C0098(&pos, obj, 1, 0xFF, 1);
                }
            }
            break;
        case 27:
            func_150FCA30();
            break;
        case 24:
            {
                u8 values[8];

                values[0] = D_8009F0D0;
                obj = func_15083E90(cmd->arg.b.lo);
                // matching: the no-op ternary orders the argument moves
                func_1518F51C(obj ? obj : obj, values[cmd->a[0].w], cmd->a[1].h.lo, -1, -1, -1, 0, 0, 0xFF, 1);
            }
            break;
        case 20:
            func_150E1060(cmd->arg.b.lo, 0xFF, 1);
            break;
        case 18:
            func_151CD35C(cmd->arg.w);
            break;
        case 16:
            func_1000E7A0(16, 0);
            light_sendEvent(22, 31, 0);
            break;
        case 0:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            func_150B53D0(obj, cmd->a[0].h.lo, 0xFF, 1);
            break;
        case 1:
            func_150BEF7C(cmd->arg.w);
            return 1;
        case 2:
            D_800D9920 = 1;
            break;
        case 3:
            func_15178EFC(cmd->arg.w);
            break;
        case 4:
            ret = func_150E3208(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w,
                                cmd->a[4].w, cmd->a[5].w, cmd->a[6].w);
            if (ret != 0) {
                ret = (ret << 6) | 0xE000 | 4;
            }
            break;
        case 5:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->a[2]));
            if (obj != NULL) {
                ret = func_150E32D0(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, obj, cmd->a[3].w, cmd->a[5].w);
                if (ret != 0) {
                    s32 pad; // matching: -g3 block keeps the handle store in the branch delay slot
                    ret = (ret << 6) | 0xE000 | 4;
                }
            }
            break;
        case 6:
            ret = func_150E3414(cmd->arg.w, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w, cmd->a[3].w,
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
                    func_150DECC0(&pos, 0xFF, 1);
                } else {
                    func_150E81A8(cmd->arg.w & 0xFF, 0xFF, 1);
                }
            }
            break;
        case 15:
            func_150E8930();
            break;
        case 19:
            func_151D26C0(cmd->arg.h.lo);
            break;
        case 22:
            light_sendEvent(35, 39, 0);
            light_sendEvent(36, 39, 0);
            break;
        case 23:
            func_15163A60(cmd->arg.b.lo, 0xFF, 1);
            break;
        case 32:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            func_150D5A6C(obj, 0xFF, 1);
            break;
    }
    return ret;
}



/* Ends script command `cmdId` started by func_1509D180. */
s32 func_1509D780(s32 cmdId, ScriptCmd *cmd) {
    Actor *obj;

    switch (cmdId) {
        case 41:
            func_150EB8C4();
            break;
        case 39:
            func_150F9720(cmd->arg.b.lo);
            break;
        case 38:
            light_sendEvent(41, 24, 0);
            break;
        case 36:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            if (obj != NULL) {
                func_150F6DB0(obj);
            }
            break;
        case 26:
            light_sendEvent(38, 24, 0);
            break;
        case 18:
            func_151CD394(cmd->arg.w);
            break;
        case 2:
            D_800D9920 = 0;
            break;
        case 4:
        case 5:
        case 6:
            func_150E35DC(cmdId >> 6);
            break;
        case 8:
            func_150B3DD0();
            break;
        case 9:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->arg));
            if (obj == NULL) {
                return 0;
            }
            func_150C5C9C(obj);
            break;
        case 10:
            func_150C522C();
            break;
        case 19:
            func_151D2718(cmd->arg.h.lo);
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

extern u8 D_800A0960[][16];

/* Advances script command `cmdId` to `phase`. */
s32 func_1509D8FC(s32 cmdId, s32 phase, ScriptCmd *cmd) {
    switch (cmdId) {
        case 52:
            if (phase == 3) {
                func_15197A7C(func_1505EEF4(CMD_OBJECT_ID(cmd->arg)));
            }
            break;

        case 50:
            if (phase == 2) {
                func_151403A8(0, 83);
            } else if (phase == 3) {
                func_151403A8(0, 84);
            }
            break;
        case 45:
            func_151645C4(phase == 6);
            break;
        case 40:
            if (phase == 2) {
                func_1515F170(4, 1);
                func_1515F170(5, 0);
            } else if (phase == 3) {
                func_1515F170(4, 0);
                func_1515F170(5, 1);
            }
            break;
        case 31:
            if (phase == 5) {
                u8 buf[8];
                buf[0] = cmd->a[0].w;
                func_151403A8((s32)buf, 54);
            }
            // fallthrough
        case 30:
            if (phase == 2) {
                u8 buf2[4];
                buf2[0] = cmd->a[0].w;
                func_151494E0((s32)buf2, 52);
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
                func_1516944C(53, (s32)&msg, 51);
            }
            break;
        case 28:
            if (phase == 5) {
                s32 idx = cmd->a[0].w;
                if (idx < 0 || idx >= 3) {
                    return 1;
                }
                func_151494E0((s32)D_800A0960[idx], 50);
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
            func_150DEC28(cmd->a[0].w, phase == 3);
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
s32 func_1509DBBC(s32 cmdId, s32 phase, s32 arg2) {
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
                        node = (Node1509DBBC *)D_800DCE50[i][D_800A6060[j]];
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
                    func_150E36BC(handle, &hi, &result, &lo);
                    if (phase == 0) {
                        result = (hi << 16) | (lo & 0xFFFF);
                    }
                    return result;
                }
            }
            break;
    }
}
