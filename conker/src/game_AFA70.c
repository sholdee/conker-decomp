#include <ultra64.h>

#include "functions.h"
#include "variables.h"

// One 0x10-byte part of a part set (D_80086CC4): a child object or an attachment.
typedef struct PartDef {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 type;     // 0: child object (func_15083AC8), otherwise func_15030AF4
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u16 data[3]; // the attachment's ownerStates
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
} PartDef;

typedef struct PartSet {
    /* 0x0 */ PartDef *parts;
    /* 0x4 */ u8 count;
} PartSet;

extern PartSet D_80086CC4[];


/* Spawns the object for spawn record spawnIdx once it is due: skips records that are in use,
   disabled (x == 0x7FFF) or still counting down their D_800D2110 delay, and records whose flags
   ask for no player nearby (radius by flags & 6) or for being off-screen (flags & 8). */
s32 func_150825C0(s32 spawnIdx, s32 arg1) {
    f32 radiusSq;
    s32 i;
    u16 range;
    u8 clear;
    u8 flags;
    s32 j;
    f32 dx;
    f32 dz;

    if (D_800D210C[spawnIdx] != 0) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].unk2 == 1) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].x == 0x7FFF) {
        return 0;
    }
    if (D_800D2110[spawnIdx]) {
        if (D_800D2110[spawnIdx] <= D_800BE9A0) {
            D_800D2110[spawnIdx] = 0;
        } else {
            D_800D2110[spawnIdx] -= D_800BE9A0;
        }
        return 0;
    }
    flags = D_800D20FC[spawnIdx].flags;
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
            for (j = 0; j < D_8008FD8C; j++) {
                if (movement_obj[j].type == 0) {
                    continue;
                }
                if (movement_obj[j].playernumber == 0xFF) {
                    continue;
                }
                if (D_800D2138.unk208 & (1 << movement_obj[j].playernumber)) {
                    // matching: squaring in place (d = d * d) reuses the operand's register
                    dx = movement_obj[j].xpos - D_800D20FC[spawnIdx].x;
                    dx = dx * dx;
                    dz = movement_obj[j].zpos - D_800D20FC[spawnIdx].z;
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
        for (i = 0; i <= D_80082FA0; i++) {
            // matching: offset + base operand order
            if (func_150A6360(i + D_800BE628, D_800D9C10[i],
                              D_800D20FC[spawnIdx].x,
                              D_800D20FC[spawnIdx].y + 150,
                              D_800D20FC[spawnIdx].z,
                              150.0f, 150.0f, 20000.0f) != 0) {
                clear = 0;
            }
        }
        if (!clear) {
            return 0;
        }
    }
    return func_15082A44(&D_800D20FC[spawnIdx], spawnIdx, 0, 0, 0);
}

/* Runs func_150825C0 over the spawn records: [0, D_800D2101) when firstRange is set, else
   [D_800D2101, D_800D2100). */
void func_1508295C(s32 arg0, s32 arg1, s32 firstRange) {
    s32 start;
    s32 end;
    s32 i;

    if (firstRange != 0) {
        start = 0;
        end = D_800D2101;
    } else {
        start = D_800D2101;
        end = D_800D2100;
    }

    for (i = start; i < end; i++) {
        func_150825C0(i, arg1);
    }
}

void func_150829D8(Actor *obj) {
    obj->noshove &= 0xF7FFFFFD;
    obj->noshove |= 4;
    if ((obj->mv != NULL) && (obj->mv->unk56 == 0)) {
        switch (level) {
        case 0:
        case 10:
        case 14:
        case 18:
        case 19:
        case 25:
        case 27:
        case 44:
        case 60:
        case 61:
        case 66:
        case 67:
        case 68:
            break;
        default:
            obj->noshove |= 2;
            break;
        }
    }
}

/* Creates and initialises a game object from spawn record spawnIdx (slot: 1-based object slot to
   reuse, 0 for the default). Returns the 1-based slot, or 0 on failure. */
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot) {
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

    if (D_800D210C[spawnIdx] & ~0x80) {
        return 0;
    }
    type = spawn->type;
    if (slot != 0) {
        obj = &movement_obj[slot - 1];
        func_1505F188(obj);
    } else if ((spawnIdx != -1) && (spawnIdx < D_8008FD8C)) {
        obj = &movement_obj[spawnIdx];
        func_1505F188(obj);
    } else {
        obj = func_1505ED34();
    }
    idx = obj - movement_obj;
    if (idx == 25) {
        return 0;
    }
    if (spawnIdx != -1) {
        if (spawn->flags & 0x21) {
            D_800D210C[spawnIdx] = idx + 1;
        } else {
            D_800D210C[spawnIdx] = idx + 0x81;
        }
        obj->listpos = spawnIdx;
        if (spawnIdx < D_8008FD8C) {
            obj->playernumber = spawnIdx;
        } else {
            obj->playernumber = 0xFF;
        }
    }
    obj->listpos_ptr = spawn;
    ret = idx + 1;
    func_15084488(spawn, obj->listpos, arg3);
    if ((idx < D_8008FD8C) || (type == 0x98) || (type == 0x28)) {
        obj->mv = allocate_memory(sizeof(ActorCtl), 1, 0, 0);
        bzero(obj->mv, sizeof(ActorCtl));
        obj->mv->gunPitch = 5.0f;
    }
    obj->unique_id = spawn->unique_id;
    obj->unk1D0 = 2;
    obj->scale_xz = spawn->xz_scale;
    obj->scale_y = spawn->y_scale;
    obj->unk154 = spawn->xz_scale;
    obj->unk158 = spawn->y_scale;
    obj->type = spawn->interaction_state;
    obj->xpos = spawn->x;
    obj->ypos = spawn->y;
    obj->zpos = spawn->z;
    obj->unk212[0] = 0xFF;
    obj->unk20F = 0xFF;
    obj->unk210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->team = 0xFF;
    obj->old_xpos = obj->xpos;
    obj->old_ypos = obj->ypos;
    obj->unk1CC = obj->ypos;
    obj->old_zpos = obj->zpos;
    if ((spawn->flags & 6) || (spawn->flags & 4) || (spawn->flags & 2)) {
        func_1506160C(obj, 4, 0, 0, 0);
    }
    obj->intelwalk = spawn->unkD;
    if ((spawnIdx < D_8008FD8C) && (D_8008FE44[spawnIdx] == -2)) {
        obj->mv->bot = 1;
        obj->intelwalk = 1;
    }
    {
        // matching: unused stack slot
        u8 pad;

        if (obj->playernumber != 0xFF) {
            obj->team = func_151E5F64(obj->playernumber);
            obj->mv->numGrenades = D_800D2130;
            obj->mv->unk1AF = 0x40;
            obj->mv->lastSpawnIndex = -1;
        }
    }
    obj->energy = spawn->unkE & 0xF;
    obj->unk1E5 = spawn->unkF;
    obj->unk1E6 = obj->unk1E5;
    obj->unk221 = 1;
    obj->unk23C = spawn->unk1C;
    obj->unk23D = spawn->unk1D;
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
        obj->unk38 = obj->ypos - 500.0f;
    }
    if (obj->noshove & 0x800000) {
        D_800C67F0 = 1;
    }
    obj->family = spawn->unk11;
    obj->unk66 = spawn->unk10;
    obj->unk1DD[0] = 120;
    obj->unk1DD[1] = 120;
    obj->unk1DD[2] = 120;
    obj->unk1E0[0] = 255;
    obj->unk1E0[1] = 255;
    obj->unk1E0[2] = 255;
    obj->unk23E = 4;
    obj->intel_animspd = 1;
    obj->intel_accel = 100;
    obj->intel_decel = 100;
    obj->unk1FC |= 2;
    func_150839B8(obj, type, spawn);
    t = spawn->unk13 & 0x7F;
    obj->unk92 = ((spawn->unk13 & 0x80) << 8) | t;
    obj->unk90 = spawn->unk14;
    func_150837D4(idx, type, 1);
    if (type != 0xFF) {
        func_150838EC(obj, 0, 0, 1.0f);
    } else {
        obj->drawtype = 3;
    }
    if (obj->noshove & 0x4000) {
        func_1503DA9C(obj, obj->character_type, 0, 0);
    }
    if (spawn->partSet) {
        func_15083568(obj, spawn->partSet, 1.0f, 0);
    }
    if (obj->playernumber != 0xFF) {
        obj->energy = func_150859AC(obj->playernumber, 4);
        obj->unkB2 = D_800D2132;
        func_150829D8(obj);
        func_15083384(obj, func_150859AC(idx, 1));
    }
    if (((obj->unk66 & 0xC) == 8) || ((obj->unk66 & 0xC) == 0xC)) {
        func_150832AC(obj);
    } else {
        obj->unk314 = NULL;
    }
    obj->unk288 = func_15084CB0(type);
    if (D_800BE616 != 0) {
        t = func_151E5F64(obj->playernumber);
        if (t >= 4) {
            t = 3;
        }
        if (type == 0x3B) {
            obj->unk68[0] = t + 1;
        } else if (type == 0x77) {
            obj->unk68[0] = t + 0x10;
        }
    }
    if ((D_80038080 == 0) && (level != 29) && (spawn->keepTime == 0xFF)) {
        found = obj->modelCount;
        if (found == 0) {
            found = 1;
        }
        for (i = 0; i < found; i++) {
            D_800D2040[obj->modelIds[i]] = 0xFF;
        }
    }
    if (level == 29) {
        obj->unk66 &= ~0x20;
    }
    func_1514D15C(obj);
    model = func_150849A0(obj);
    found = 0;
    for (i = 0; i < 2; i++) {
        if (model == D_800A7230[i].kind) {
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

void func_150832AC(Actor *obj) {
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
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unk304[i] = func_1515D480(count);
        }
        if (obj->unk304[i - 1] != 0) {
            obj->unk301 = count;
        }
        obj->unk314 = func_1515D440();
    }
}

void func_15083384(Actor *obj, s32 arg1) {
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
        obj->unk94 |= 2;
        break;
    case 13:
        partSet = 0x76;
        break;
    case 14:
        partSet = 0x78;
        break;
    case 15:
        partSet = 0x77;
        obj->unk94 |= 2;
        break;
    case 29:
    case 30:
    case 31:
    case 32:
        {
            s32 n;

            n = arg1 - 29;
            obj->unk94 = 0x1E;
            if (n < 2) {
                obj->unk94 &= ~0x14;
            } else {
                obj->unk94 &= ~8;
            }
            if (n >> 1) {
                obj->unk94 &= ~2;
            }
        }
        break;
    case 17: case 18: case 19: case 20: case 21: case 22:
    case 23: case 24: case 25: case 26: case 27: case 28:
        group = (arg1 - 17) >> 2;
        obj->unk94 = 0x3E;
        switch (group) {
        case 0:
            obj->unk94 &= ~4;
            break;
        case 1:
            obj->unk94 &= ~8;
            break;
        case 2:
            obj->unk94 &= ~0x10;
            break;
        }
        switch (arg1 - group * 4 - 17) {
        case 1:
            obj->unk94 &= ~2;
            break;
        case 2:
            obj->unk94 &= ~0x20;
            break;
        case 3:
            obj->unk94 &= ~0x22;
            break;
        }
        break;
    }
    if (partSet != -1) {
        func_15083568(obj, partSet, 0.0f, 0);
    }
}

/* Creates every part of part set setId (1-based) on parent; returns the last part created (a child
 * Actor or a ModelAttachment, depending on its type). */
void *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3) {
    PartDef *parts;
    void *ret;
    s32 i;

    setId--;
    ret = NULL;
    parts = D_80086CC4[setId].parts;
    for (i = 0; i < D_80086CC4[setId].count; i++) {
        if (parts[i].type == 0) {
            ret = func_15083AC8(parent, parts[i].unk1, parts[i].unk0, 0, parts[i].unk2, parts[i].unk4, parts[i].unk5, parts[i].unk6, arg2);
        } else {
            ret = func_15030AF4(parent, parts[i].unk0, parts[i].unk1, parts[i].unk7, parts[i].unk4, parts[i].unk5, arg3, setId + 1, parts[i].unk2,
                                parts[i].data, (parts[i].type == 2) ? parts[i].unk6 : -1, parts[i].unkE, parts[i].unkF);
        }
    }
    return ret;
}

/* Removes the child objects of part set setId (1-based) that belong to parent. */
void func_150836CC(Actor *parent, s32 setId) {
    PartSet *set;
    s32 i;
    PartDef *parts;

    func_150302F0(parent, setId);
    setId--;
    set = &D_80086CC4[setId];
    parts = set->parts;
    for (i = 0; i < set->count; i++) {
        Actor *child;

        if (parts[i].type == 0) {
            child = func_1505F0AC(parts[i].unk0);
            if (child != NULL) {
                if (child->attach_char == ((parent - movement_obj) + 1)) {
                    func_15060F28(child, 0);
                }
            }
        }
    }
}

/* Sets object objIdx's model to modelId (0xFF for none) and loads its per-model state. */
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2) {
    Actor *obj = &movement_obj[objIdx];
    ModelDef *def;

    obj->character_type = modelId;
    if (modelId == 0xFF) {
        obj->bodyRadius = 0;
        obj->bodyHeight = 0;
        obj->bodyOffsetY = 0;
        obj->unkD2 = 70;
        obj->unkD4 = 70;
        obj->unkD6 = 0;
        obj->shadow_size = 0;
        obj->unkC8 = 0.0f;
    } else {
        obj->unkC8 = D_800D1C90[modelId]->unk6;
        func_15062BDC(obj, obj->scale_xz, obj->scale_y);
        def = D_800D1C90[modelId];
        obj->drawtype = def->unk12;
    }
    obj->unk6 = func_15084D00(obj);
    if (D_800C5A90[obj->unk6] != 0) {
        obj->unk58 = (s32)func_1502B020(0, 2, 2, obj->unk6);
    }
}

/* Allocates the animation state block of obj's model. Returns 1 only if the allocation fails. */
s32 func_150838EC(Actor *obj, s32 arg1, s32 arg2, f32 arg3) {
    s32 modelId;

    modelId = obj->character_type;
    if (D_800D1588[modelId] == NULL) {
        return 0;
    }
    if (D_800C5A90[modelId] == 0) {
        return 0;
    }
    obj->unk2D0 = allocate_memory(0x3E0, 1, 2, 0);
    if (obj->unk2D0 == NULL) {
        return 1;
    }
    bzero(obj->unk2D0, 0x40);
    func_1505E650(obj, arg1, arg3, 0.0f, 0.0f, 0.0f, arg2);
    return 0;
}

/* Copies model modelId's defaults from its ModelDef into obj; spawn (may be NULL) overrides unk10. */
void func_150839B8(Actor *obj, s32 modelId, SpawnRecord *spawn) {
    ModelDef *def;
    u16 override;

    if (modelId != 0xFF) {
        def = D_800D1C90[modelId];
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
            if (level == 29) {
                obj->cullDistance = 1000;
            }
        } else {
            obj->cullDistance = 0;
        }
        obj->unk13B = def->unk39;
        obj->unk2CB = def->unk33;
        obj->unk2CC = def->unk34;
        obj->drawtype = def->unk12;
        obj->unk68[0] = def->eyeDefault[0];
        obj->unk68[1] = def->eyeDefault[1];
        obj->shadow_size = def->unk2;
        if (def->subModelCount != 0) {
            obj->modelIds = D_80086CAC[def->subModelList];
            obj->lodCount = def->subModelCount;
        }
        obj->modelCount = obj->lodCount + def->extraModelCount;
        if (def->unk29 != 0) {
            func_15036C70(obj);
        }
    }
}

/* Creates a child object of parent with model modelId (a part from func_15083568). */
Actor *func_15083AC8(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8) {
    Actor *obj;
    // matching: unused stack slot
    s32 pad;
    s32 animFlag;
    s32 blocked;
    s32 i;

    func_1503D774(modelId, 0);
    func_1503D660(modelId, 0);
    obj = func_1505ED34();
    obj->unique_id = 0xFF - (obj - movement_obj);
    obj->unkAB = arg3;
    obj->type = state;
    func_150615DC(obj);
    obj->intelwalk = 1;
    obj->energy = 1;
    obj->imune = 0xFF;
    obj->attach_char = (parent - movement_obj) + 1;
    obj->basemtx = arg1;
    obj->unk123 = 1;
    obj->character_type = modelId;
    obj->unk66 = parent->unk66;
    obj->unk66 &= ~0x10;
    // matching: a no-op |= 0 (a constant flag field upstream) keeps the reload of unk66
    obj->unk66 |= 0;
    obj->unk1DD[0] = parent->unk1DD[0];
    obj->unk1DD[1] = parent->unk1DD[1];
    obj->unk1DD[2] = parent->unk1DD[1];
    obj->unk1E0[0] = parent->unk1E0[0];
    obj->unk1E0[1] = parent->unk1E0[1];
    obj->unk1E0[2] = parent->unk1E0[2];
    if (inheritScale == 0) {
        obj->scale_xz = 1.0f;
        obj->scale_y = 1.0f;
        obj->unk154 = 1.0f;
        obj->unk158 = 1.0f;
    } else {
        obj->scale_xz = parent->scale_xz;
        obj->scale_y = parent->scale_y;
        obj->unk154 = parent->unk154;
        obj->unk158 = parent->unk158;
    }
    obj->unk101 = inheritScale;
    obj->unkAC = arg5;
    func_1503D774(modelId, 0);
    if (arg5 & 1) {
        obj->noshove |= 0x4000;
        obj->cull_radius_xz = D_800D1C90[modelId]->bodyRadius * obj->scale_xz;
    }
    animFlag = 0;
    if (arg5 & 2) {
        animFlag = 1;
    }
    for (i = 0; i <= D_80082FA0; i++) {
        obj->unk304[i] = func_1515D480(1);
    }
    if (obj->unk304[i - 1] != 0) {
        obj->unk301 = 1;
    }
    obj->unk314 = func_1515D440();
    if (D_800C35EA[0] == 1) {
        blocked = 1;
    } else {
        blocked = 0;
    }
    if (blocked) {
        D_800C3638 = 0;
    }
    func_150839B8(obj, modelId, NULL);
    func_150837D4(obj - movement_obj, modelId, 1);
    func_150838EC(obj, arg7, animFlag, arg8);
    if (blocked) {
        D_800C3638 = 1;
    }
    return obj;
}

/* Returns the lowest unique id (1..255) that no spawn record uses. */
s32 func_15083DA0(void) {
    s32 id;
    s32 i;

    id = 1;
    for (i = 0; i < D_800D2100; i++) {
        if (id == D_800D20FC[i].unique_id) {
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
s32 func_15083E0C(u8 id) {
    s32 i;

    if (id == 0) {
        return -1;
    }
    for (i = 0; i < D_800D2100; i++) {
        if (D_800D20FC[i].unique_id == id) {
            return &D_800D20FC[i] - D_800D20FC;
        }
    }
    return -1;
}

/* Returns the active object with unique id id, or NULL. */
Actor *func_15083E90(u8 id) {
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
s32 func_15083FB0(u8 id) {
    Actor *obj;

    obj = func_15083E90(id);
    if (obj != NULL) {
        return obj - movement_obj;
    }
    return -1;
}

s32 func_15084000(s32 id) {
    Actor *obj = func_1505EEF4(id);

    if (obj != NULL) {
        return obj - movement_obj;
    }
    return -1;
}

/* Sets up obj's two data pointers for its model'th model: shared with D_800D19A0[id] or private
   copies, depending on the owning spawn record's mode (unk10 & 3). Returns 1 if an allocation failed. */
s32 func_15084044(Actor *obj, s32 model) {
    u8 *slot;
    SpawnRecord *spawn;
    s32 mode;
    u8 id;
    s32 val;
    s32 ret;
    s32 i;
    s32 size;

    // matching: every access to modelData[model] goes through this byte pointer (slot + 0x28C/0x290) or
    // an s32 row view, as s32 lvalues; typed field access reallocates registers (see the func_15084044 ledger)
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
    val = (s32)D_800D19A0[id];
    if ((s32)D_800D19A0[id] == 0) {
        return 0;
    }
    ret = 0;
    if (mode == 0) {
        val = (s32)D_800D19A0[id];
        *(s32 *)(slot + 0x290) = val;
        *(s32 *)(slot + 0x28C) = val;
        D_800D1F80[id] += 2;
    } else if (mode == 1) {
        if (*(s32 *)(slot + 0x28C) == 0 || *(s32 *)(slot + 0x28C) == (s32)D_800D19A0[id]) {
            if (D_800D1F80[id] != 0) {
                size = D_800C57A0[id] * 16;
                *(s32 *)(slot + 0x28C) = (s32)allocate_memory(size, 1, 2, 2);
                if (*(s32 *)(slot + 0x28C) == 0) {
                    ret = 1;
                } else if (*(s32 *)(slot + 0x28C) != 0) {
                    bcopy((void *)(s32)D_800D19A0[id], (void *)*(s32 *)(slot + 0x28C), size);
                } else {
                    *(s32 *)(slot + 0x28C) = (s32)D_800D19A0[id];
                }
            } else {
                val = (s32)D_800D19A0[id];
                *(s32 *)(slot + 0x290) = val;
                *(s32 *)(slot + 0x28C) = val;
                D_800D1F80[id] += 2;
            }
        }
        *(s32 *)(slot + 0x290) = *(s32 *)(slot + 0x28C);
    } else if (mode == 2) {
        for (i = 0; i < 2; i++) {
            if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0 || ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == (s32)D_800D19A0[id] || (i == 1 && *(s32 *)(slot + 0x28C) == *(s32 *)(slot + 0x290))) {
                if (D_800D1F80[id] != 0) {
                    size = D_800C57A0[id] * 16;
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)allocate_memory(size, 1, 2, 2);
                    if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0) {
                        ret = 1;
                        if (i == 1) {
                            if (*(s32 *)(slot + 0x28C) != (s32)D_800D19A0[id]) {
                                func_10004074((void *)*(s32 *)(slot + 0x28C));
                            } else {
                                D_800D1F80[id] -= 1;
                            }
                            *(s32 *)(slot + 0x28C) = 0;
                        }
                        break;
                    } else if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] != 0) {
                        bcopy((void *)(s32)D_800D19A0[id], (void *)((s32 (*)[2])((u8 *)obj + 0x28C))[model][i], size);
                    } else {
                        ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    }
                } else {
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    D_800D1F80[id] += 1;
                }
            }
        }
    }
    return ret;
}

/* Releases the data pointers of all of obj's models. */
void func_1508434C(Actor *obj) {
    s32 count;
    s32 i;

    count = obj->modelCount;
    if (count == 0) {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        func_150843AC(obj, i);
    }
}

/* Releases the data pointers of obj's model'th model: frees private copies, drops the
   reference count of shared ones. */
void func_150843AC(Actor *obj, s32 model) {
    // matching: a named trip count takes its saved register before the D_800D19A0 base
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
            if (data != D_800D19A0[id]) {
                func_100043B4((s32 *)data, 3);
            } else {
                D_800D1F80[id] -= 1;
            }
            obj->modelData[model][i] = NULL;
        }
    }
}

/* Loads model spawn->type and all of its sub-models. */
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2) {
    s32 count;
    s32 i;
    ModelDef *def;
    u8 *list;
    s32 modelId;

    modelId = spawn->type;
    if (modelId != 0xFF) {
        func_1503D774(modelId, arg2);
        def = D_800D1C90[modelId];
        list = &spawn->type;
        count = def->subModelCount;
        if (count == 0) {
            count = 1;
        } else {
            list = D_80086CAC[def->subModelList];
        }
        count += def->extraModelCount;
        for (i = 0; i < count; i++) {
            func_1503D774(list[i], arg2);
            func_1503D660(list[i], arg2);
        }
    }
}

/* Releases obj's model data, and frees each of its models (and their tables) that no other
   active object still uses. */
void func_15084558(Actor *self) {
    s32 id;
    u8 *list;
    s32 i;
    s32 j;
    s32 count;
    s32 k;
    s32 unique;
    s32 freeData;
    s32 freeDef;
    Actor *obj;
    u8 otherCount;
    u8 *otherList;
    // matching: unused stack slot
    s32 pad;
    ActorAnim *data;
    u8 *block;
    TexCoord *table;

    id = self->character_type;
    if (id != 0xFF && D_800D19A0[id] != NULL) {
        count = self->modelCount;
        list = self->modelIds;
        func_1508434C(self);
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
                            if (D_800D1588[otherList[k]] == D_800D1588[list[i]] && D_800D1588[otherList[k]] != NULL) {
                                freeData = 0;
                            }
                            if (D_800D1C90[otherList[k]] == D_800D1C90[list[i]] && D_800D1C90[otherList[k]] != NULL) {
                                freeDef = 0;
                            }
                        }
                    }
                }
            }
            if (freeData && D_800D1588[list[i]] != NULL) {
                data = D_800D1588[list[i]];
                {
                    func_100043B4((s32 *)ANIM_HEADER(data), 3);
                    for (k = 0; k < 187; k++) {
                        if (data == D_800D1588[k]) {
                            D_800D1588[k] = NULL;
                        }
                    }
                }
            }
            if (freeDef) {
                block = (u8 *)D_800D1C90[list[i]];
                if (block != NULL) {
                    block -= 0x10;  // the allocation starts 0x10 bytes before the data
                    func_100043B4((s32 *)block, 3);
                    D_800D1C90[list[i]] = NULL;
                }
            }
            if (unique) {
                if (D_800D19A0[list[i]] != NULL) {
                    if (D_800C5C08[id] != NULL) {
                        func_10004074((void *)(s32)D_800C5C08[id]); // matching: the int round trip keeps the load in v0
                        D_800C5C08[id] = NULL;
                        func_10004074(D_800C6070[id]);
                        D_800C6070[id] = NULL;
                    }
                    table = D_800C6360[id];
                    if (table != NULL) {
                        func_10004074(table);
                        D_800C6360[id] = NULL;
                    }
                }
            }
        }
    }
}

/* Returns obj's current model id (modelIds[forced_lod - 1], or the first). */
s32 func_150849A0(Actor *obj) {
    s32 current = obj->forced_lod;

    if (current != 0) {
        return obj->modelIds[current - 1];
    }
    return obj->modelIds[0];
}

/* Like func_150849A0, falling back to modelIds[lodCount - 1]; stores the index in *outIndex. */
s32 func_150849CC(Actor *obj, s32 *outIndex) {
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

/* Ticks the D_800D2040 countdowns; a model whose countdown expires while it has no D_800C5C08
   table is unloaded and every object's references to it are dropped. */
void func_15084A18(void) {
    s32 i;
    s32 m;
    Actor *obj;
    s32 j;
    s32 k;
    ObjTexture *textures;
    u8 *timer;

    timer = D_800D2040;
    for (i = 0; i < 187; i++, timer++) {
        if (*timer == 0xFF || *timer == 0) {
            continue;
        }
        if (*timer == 1) {
            if (D_800D19A0[i] != NULL && D_800C5C08[i] == NULL) {
                for (m = 0; m < 25; m++) {
                    obj = &movement_obj[m];
                    if (obj->type != 0) {
                        for (j = 0; j < obj->modelCount; j++) {
                            for (k = 0; k < 2; k++) {
                                if (obj->modelData[j][k] == D_800D19A0[i]) {
                                    obj->modelData[j][k] = NULL;
                                }
                            }
                            if (i == obj->modelIds[j]) {
                                func_150843AC(obj, j);
                            }
                        }
                    }
                }
                textures = D_800C5338[i];
                for (k = 0; k < D_800C5628[i]; k++) {
                    if (textures[k].addr >= 0x10000000U) {
                        func_1510D720(textures[k].id);
                    }
                }
                func_10004074(D_800D19A0[i] - 0x38);
                D_800D1F80[i] = 0;
                D_800D19A0[i] = NULL;
            }
        }
        (*timer)--;
    }
}

void func_15084C30(Actor *obj) {
    AnimInstance *anim;
    f32 phase;

    if (obj->character_type == 0x94) {
        anim = obj->unk2D0;
        phase = func_150ADA20() % (s32)anim->numFrames[0];
        anim->frame[0] = phase;
    }
}

/* Returns the index of modelId in D_800BE598, or 0. */
s32 func_15084CB0(s32 modelId) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < D_800BE590; i++) {
        if (modelId == D_800BE598[i]) {
            ret = i;
            break;
        }
    }
    return ret;
}
