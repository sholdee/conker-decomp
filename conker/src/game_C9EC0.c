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
#define gResourceLoadedBits ((u8 *)D_800D2E4C)
#define gResourceStates (*(u8 (*)[0xF0])&D_800D2E70)
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
    s32 i = 0;

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

void func_1509CCB4(s32 index);
void func_1509CE64(u16 *list, void (*callback)(s32));

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
extern void *func_1502B6BC(s32, s32, u32 *, s32, s32, s32, s32);
extern void func_10004074(void *);

/* Loads the resource table, relocates every block in it and returns its resource count in *countOut. */
s32 func_1509CF28(s32 arg0, s32 *countOut) {
    ResourceSlot *table;
    u32 i;
    s32 done;
    u32 count;
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
s32 func_1509D08C(s32 arg0, s32 index, s32 *firstDepOut, s32 *unk0Out) {
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

/* One argument word of a script command; handlers also read just its low halfword or low byte. */
typedef union CmdArg {
    s32 w;
    struct {
        s16 hi;
        s16 lo;
    } h;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 lo;
    } b;
} CmdArg;

typedef struct ScriptCmd {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CmdArg args[8];
} ScriptCmd;

/* Result of a floor probe (func_15046C80). */
typedef struct FloorHit {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ s32 pad8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 pad24[2];
} FloorHit; // size = 0x2C

/* Object id arguments carry flags above the low 12 bits. */
#define CMD_OBJECT_ID(arg) ((arg).w & 0xFFF)

extern struct127 *func_1505EEF4(s32);
extern void func_150BD070(s32, s32);
extern void func_150FAD28(void);
extern void func_150FAA40(s32, s32);
extern void func_15108120(struct127 *, s32, s32);
extern void func_150ECC00(struct127 *, s32, s32);
extern void func_150FAD78(void);
extern void func_150FCFD4(struct127 *, s32, s32);
extern void func_150E0300(void);
extern void func_151557FC(s32, s32, f32);
extern void *func_150F6DE4(struct127 *);
extern void func_1514EC1C(void *, struct127 *, s32);
extern s32 func_15046C80(f32 *, s32, f32, void *);
extern void func_151BFE84(void *, void *, void *, s32, s32, s32);
extern void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
extern void func_151C0098(f32 *, struct127 *, s32, s32, s32);
extern void func_150FCA30(void);
extern s32 func_1518F51C(struct127 *, u8, s32, s32, s8, s8, u8, s32, u8, s32);
extern void func_150E1060(s32, s32, s32);
extern void func_151CD35C(s32);
extern void func_1000E7A0(s32, s32);
extern void func_151616D0(u8, u8, s32);
extern void func_150B53D0(struct127 *, s32, s32, s32);
extern void func_150BEF7C(s32);
extern void func_15178EFC(s32);
extern s32 func_150E3208(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_150E32D0(s32, s32, s32, struct127 *, s32, f32);
extern s32 func_150E3414(s32, s32, s32, s32, s32, s32, void *, s32, f32);
extern void func_150DECC0(f32 *, s32, s32);
extern void func_150E81A8(s32, s32, s32);
extern void func_150E8930(void);
extern void func_151D26C0(s16);
extern void func_15163A60(s32, s32, s32);
extern void func_150D5A6C(struct127 *, s32, s32);
extern u8 D_8009F0D0;
extern u8 D_800D9920;
extern struct127 D_800CC2D0[26];

/* Starts script command `cmdId` with arguments `cmd`; returns a handle for commands 4-6. */
s32 func_1509D180(s32 cmdId, ScriptCmd *cmd) {
    s32 ret;
    struct127 *obj;

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
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            if (obj != NULL) {
                func_15108120(obj, 0xFF, 1);
            }
            break;
        case 46:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            if (obj != NULL) {
                func_150ECC00(obj, 0xFF, 1);
            }
            break;
        case 44:
            func_150FAD78();
            break;
        case 42:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            if (obj != NULL) {
                func_150FCFD4(obj, 0xFF, 1);
            }
            break;
        case 43:
            func_150E0300();
            break;
        case 37:
            func_151557FC(CMD_OBJECT_ID(cmd->args[0]), 120, cmd->args[1].w);
            break;
        case 36:
            {
                struct127 *target;

                target = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
                if (target != NULL) {
                    func_1514EC1C(func_150F6DE4(target), target, 27);
                }
            }
            break;
        case 34:
            {
                FloorHit floor;
                f32 pos[3];

                floor.height = -10000.0f;
                floor.unk18 = 0;
                floor.unk1C = 0;
                floor.unk1D = 0;
                floor.unk20 = 0;
                pos[0] = cmd->args[0].w;
                pos[1] = 20000.0f;
                pos[2] = cmd->args[1].w;
                if (func_15046C80(pos, 0, floor.height, &floor) != 0) {
                    { // matching: -g3 block keeps the float stores out of the branch delay slot
                        f32 dir[3];

                        pos[1] = floor.height;
                        dir[0] = 0.0f;
                        dir[2] = 0.0f;
                        dir[1] = -1.0f;
                        func_151BFE84(&floor.unk4, pos, dir, 1, 0xFF, 1);
                    }
                }
            }
            break;
        case 35:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            func_15136C3C(obj, 1, 1, 1, 1, 0, 0xFF, 1);
            break;
        case 33:
            {
                f32 pos[5];

                obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
                if (obj != NULL) {
                    pos[0] = obj->x_position;
                    pos[1] = obj->y_position + 50.0f;
                    pos[2] = obj->z_position;
                    func_151C0098(pos, obj, 1, 0xFF, 1);
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
                obj = func_15083E90(cmd->args[0].b.lo);
                // matching: the no-op ternary orders the argument moves
                func_1518F51C(obj ? obj : obj, values[cmd->args[1].w], cmd->args[2].h.lo, -1, -1, -1, 0, 0, 0xFF, 1);
            }
            break;
        case 20:
            func_150E1060(cmd->args[0].b.lo, 0xFF, 1);
            break;
        case 18:
            func_151CD35C(cmd->args[0].w);
            break;
        case 16:
            func_1000E7A0(16, 0);
            func_151616D0(22, 31, 0);
            break;
        case 0:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            func_150B53D0(obj, cmd->args[1].h.lo, 0xFF, 1);
            break;
        case 1:
            func_150BEF7C(cmd->args[0].w);
            return 1;
        case 2:
            D_800D9920 = 1;
            break;
        case 3:
            func_15178EFC(cmd->args[0].w);
            break;
        case 4:
            ret = func_150E3208(cmd->args[0].w, cmd->args[1].w, cmd->args[2].w, cmd->args[3].w, cmd->args[4].w,
                                cmd->args[5].w, cmd->args[6].w, cmd->args[7].w);
            if (ret != 0) {
                ret = (ret << 6) | 0xE000 | 4;
            }
            break;
        case 5:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[3]));
            if (obj != NULL) {
                ret = func_150E32D0(cmd->args[0].w, cmd->args[1].w, cmd->args[2].w, obj, cmd->args[4].w, cmd->args[6].w);
                if (ret != 0) {
                    s32 pad; // matching: -g3 block keeps the handle store in the branch delay slot
                    ret = (ret << 6) | 0xE000 | 4;
                }
            }
            break;
        case 6:
            ret = func_150E3414(cmd->args[0].w, cmd->args[1].w, cmd->args[2].w, cmd->args[3].w, cmd->args[4].w,
                                cmd->args[5].w, D_800CC2D0, cmd->args[6].w, cmd->args[7].w);
            if (ret != 0) {
                ret = (ret << 6) | 0xE000 | 4;
            }
            break;
        case 7:
            {
                f32 pos[5];

                if (cmd->args[0].w == 999) {
                    pos[0] = cmd->args[1].w;
                    pos[1] = cmd->args[2].w;
                    pos[2] = cmd->args[3].w;
                    func_150DECC0(pos, 0xFF, 1);
                } else {
                    func_150E81A8(cmd->args[0].w & 0xFF, 0xFF, 1);
                }
            }
            break;
        case 15:
            func_150E8930();
            break;
        case 19:
            func_151D26C0(cmd->args[0].h.lo);
            break;
        case 22:
            func_151616D0(35, 39, 0);
            func_151616D0(36, 39, 0);
            break;
        case 23:
            func_15163A60(cmd->args[0].b.lo, 0xFF, 1);
            break;
        case 32:
            obj = func_1505EEF4(CMD_OBJECT_ID(cmd->args[0]));
            func_150D5A6C(obj, 0xFF, 1);
            break;
    }
    return ret;
}


typedef struct {
    s32 unk0;
    s32 unk4;
} Func1509D780Arg;

extern void func_150EB8C4(void);
extern void func_150F9720(u8);
extern void func_150F6DB0(void *);
extern void func_151616D0(u8, u8, s32);
extern void func_151CD394(s32);
extern void func_150E35DC(s32);
extern void func_150B3DD0(void);
extern void func_150C5C9C(void *);
extern void func_150C522C(void);
extern void func_151D2718(s16);

s32 func_1509D780(s32 arg0, Func1509D780Arg *arg1) {
    void *p;

    switch (arg0) {
        case 41:
            func_150EB8C4();
            break;
        case 39:
            func_150F9720(*((u8 *)arg1 + 7));
            break;
        case 38:
            func_151616D0(0x29, 0x18, 0);
            break;
        case 36:
            p = func_1505EEF4(arg1->unk4 & 0xFFF);
            if (p != NULL) {
                func_150F6DB0(p);
            }
            break;
        case 26:
            func_151616D0(0x26, 0x18, 0);
            break;
        case 18:
            func_151CD394(arg1->unk4);
            break;
        case 2:
            D_800D9920 = 0;
            break;
        case 4:
        case 5:
        case 6:
            func_150E35DC(arg0 >> 6);
            break;
        case 8:
            func_150B3DD0();
            break;
        case 9:
            p = func_1505EEF4(arg1->unk4 & 0xFFF);
            if (p == NULL) {
                return 0;
            }
            func_150C5C9C(p);
            break;
        case 10:
            func_150C522C();
            break;
        case 19:
            func_151D2718(*(s16 *)((u8 *)arg1 + 6));
            break;
        case 21:
            func_151616D0(0x20, 0x18, 0);
            break;
        case 22:
            func_151616D0(0x23, 0x28, 0);
            func_151616D0(0x24, 0x28, 0);
            break;
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} Func1509D8FCArg;

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} Func1509D8FCMsg;

extern void func_15197A7C(void *);
extern void func_151645C4(u8);
extern void func_1515F170(s32 arg0, u8 arg1);
extern void func_151616D0(u8, u8, s32);
extern void func_150DEC28(u8, u8);
extern u8 D_800A0960[][16];

s32 func_1509D8FC(s32 arg0, s32 arg1, Func1509D8FCArg *arg2) {
    switch (arg0) {
        case 52:
            if (arg1 == 3) {
                func_15197A7C(func_1505EEF4(arg2->unk4 & 0xFFF));
            }
            break;

        case 50:
            if (arg1 == 2) {
                func_151403A8(0, 0x53);
            } else if (arg1 == 3) {
                func_151403A8(0, 0x54);
            }
            break;
        case 45:
            func_151645C4(arg1 == 6);
            break;
        case 40:
            if (arg1 == 2) {
                func_1515F170(4, 1);
                func_1515F170(5, 0);
            } else if (arg1 == 3) {
                func_1515F170(4, 0);
                func_1515F170(5, 1);
            }
            break;
        case 31:
            if (arg1 == 5) {
                u8 buf[8];
                buf[0] = arg2->unk8;
                func_151403A8((s32)buf, 0x36);
            }
        case 30:
            if (arg1 == 2) {
                u8 buf2[4];
                buf2[0] = arg2->unk8;
                func_151494E0((s32)buf2, 0x34);
            }
            break;
        case 29:
            if (arg1 == 5) {
                Func1509D8FCMsg msg;
                msg.unk0 = arg2->unkC;
                msg.unk4 = arg2->unk8;
                msg.unk5 = arg2->unk10;
                msg.unk6 = arg2->unk14;
                msg.unk7 = arg2->unk18;
                func_1516944C(0x35, (s32)&msg, 0x33);
            }
            break;
        case 28:
            if (arg1 == 5) {
                s32 idx = arg2->unk8;
                if (idx < 0 || idx >= 3) {
                    return 1;
                }
                func_151494E0((s32)D_800A0960[idx], 0x32);
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14: {
            s32 x;
            u8 a;
            u8 b;

            x = arg0 & 0x3F;
            if (x == 11) {
                a = 1;
            } else if (x == 12) {
                a = 2;
            } else if (x == 13) {
                a = 3;
            } else if (x == 14) {
                a = 4;
            } else {
                a = 0;
            }
            if (arg1 == 2) {
                b = 0x17;
            } else if (arg1 == 3) {
                b = 0x18;
            } else {
                b = 0x17;
            }
            func_151616D0(a, b, 0);
            break;
        }
        case 17:
            func_150DEC28(arg2->unk8, arg1 == 3);
            break;
    }
}

typedef struct Node1509DBBC {
    u8 pad0[8];
    struct Node1509DBBC *next;
    u8 padC[0x3C];
    u8 type;
} Node1509DBBC;

extern s32 D_800A6060[];
extern Node1509DBBC *D_800DCE50[][0x68];
void func_150E36BC(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3);

s32 func_1509DBBC(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 id;

    sp3C = 0;
    sp38 = 0;
    sp34 = 0;
    switch (arg0) {
    case 0x19:
        if (arg1 == 4) {
            u8 i;
            s8 j;
            Node1509DBBC *node;

            i = 0;
            do {
                for (j = 0; j < 3; j++) {
                    node = D_800DCE50[i][D_800A6060[j]];
                    while (node != NULL) {
                        Node1509DBBC *nx;
                        nx = node->next;
                        if (node->type == 2) {
                            return 0;
                        }
                        node = nx;
                    }
                }
                i++;
            } while (i < 2);
            return 1;
        }
        break;
    case 4:
    case 5:
    case 6:
        if (arg1 == 0 || arg1 == 1) {
            id = arg0 >> 6;
            if (id != 0) {
                func_150E36BC(id, &sp3C, &sp38, &sp34);
                if (arg1 == 0) {
                    sp38 = (sp3C << 16) | (sp34 & 0xFFFF);
                }
                return sp38;
            }
        }
        break;
    }
}
