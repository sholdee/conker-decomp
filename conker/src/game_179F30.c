#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn callbacks by slot (see func_1514D15C); slot 0 is empty. */
static void (*D_8008AA70[])(Actor *) = {
    NULL,           func_1514E00C, func_1514E194, func_1514E31C, func_1514E5B8, func_1514E668,
    func_1514E508,  func_1514E718, func_1514E7C8, func_1514E824, func_1514D9F4, func_1514D96C,
    func_1514D64C,  func_1514DB58, func_1514DE50, func_1514DFD0, func_1514D564, func_1514DA38,
    func_1514DAA4,  func_1514DB18, func_1514DB64, func_1514DE88, func_1514DB98,
};

/* Spawn callbacks by object id; entry 0 runs for every object first. */
static void (*D_8008AACC[])(Actor *) = {
    func_1514EE70, func_1514DD2C, func_1514DD4C, func_1514DD6C, func_1514DD8C, func_1514EECC,
    func_1514F110, func_150F0938, func_1514DDAC, func_1514DDCC, func_1514DDEC, func_1514DE38,
    func_1514DE44, func_1514F308, func_1514DE0C, func_1514DC84, func_1514F3CC, func_1514DE2C,
    func_1514DC98, func_1514DC38, func_1514DBB8, func_1514F44C, func_15010538, func_1514F4A8,
    func_1514F4D8, func_1514F508, func_1514F568, func_1514F590, func_150B0C34, func_1514F5CC,
    func_150F2C8C, func_1501214C, func_1514DC5C, func_1514F538, func_1514D978,
};

/* Attachment type handlers (AttachmentType.destroyHandler, .messageHandler). Declared without a
   prototype: the handlers take the attachment's handle as what it is. */
static void (*D_8008AB58[])() = { func_1514E830, func_1514E850, func_1514E87C };
static s32 (*D_8008AB64[])() = {
    func_1514E89C, func_1514E920, func_1514E9DC, func_1514E9FC,
    func_1514EA1C, func_1514EB6C, func_1514F130, func_1514EB8C,
};

/* Emitter offsets. */
static const Vec3f D_800A58A0 = { 0.0f, 75.0f, 96.0f };
static const Vec3f D_800A58AC = { 0.0f, 66.0f, 50.0f };
static const Vec3f D_800A58B8 = { 0.0f, 66.0f, 274.0f };
static const Vec3f D_800A58C4 = { 0.0f, 31.0f, 73.0f };

/**
 * Finds which spawn-callback slots apply to `obj`: one chosen by its object id (*idSlot, flag 4),
 * one chosen by level (*globalSlot, flag 2) and, inside that, one chosen by its unique_id
 * (*uniqueSlot, flag 1). Returns the OR of the flags that were set.
 */
s32 func_1514CA80(Actor *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot) {
    u8 found = 0;

    switch (obj->character_type) {
    case 94:
        *idSlot = 25;
        found = 4;
        break;
    case 154:
        *idSlot = 24;
        found = 4;
        break;
    case 16:
    case 46:
        *idSlot = 23;
        found = 4;
        break;
    case 127:
        *idSlot = 22;
        found = 4;
        break;
    case 153:
        *idSlot = 21;
        found = 4;
        break;
    case 137:
    case 186:
        *idSlot = 20;
        found = 4;
        break;
    case 173:
        *idSlot = 17;
        found = 4;
        break;
    case 134:
        *idSlot = 18;
        found = 4;
        break;
    case 82:
        *idSlot = 19;
        found = 4;
        break;
    case 95:
        *idSlot = 27;
        found = 4;
        break;
    case 107:
        *idSlot = 16;
        found = 4;
        break;
    case 148:
        *idSlot = 14;
        found = 4;
        break;
    case 37:
        *idSlot = 26;
        found = 4;
        break;
    case 146:
        *idSlot = 15;
        found = 4;
        break;
    case 159:
    case 160:
        *idSlot = 13;
        found = 4;
        break;
    case 147:
        *idSlot = 12;
        found = 4;
        break;
    case 141:
        *idSlot = 11;
        found = 4;
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 150:
        *idSlot = 0;
        found = 4;
        break;
    case 8:
        *idSlot = 2;
        found = 4;
        break;
    case 10:
        *idSlot = 3;
        found = 4;
        break;
    case 41:
    case 42:
        *idSlot = 4;
        found = 4;
        break;
    case 83:
        *idSlot = 5;
        found = 4;
        break;
    case 119:
        *idSlot = 6;
        found = 4;
        break;
    case 71:
        *idSlot = 7;
        found = 4;
        break;
    case 40:
        *idSlot = 8;
        found = 4;
        break;
    case 123:
        *idSlot = 9;
        found = 4;
        break;
    case 18:
        *idSlot = 10;
        found = 4;
        break;
    }

    switch (level) {
    case 43:
        *globalSlot = 21;
        found |= 2;
        if (obj->character_type == 156 || obj->character_type == 157) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 29:
        *globalSlot = 20;
        found |= 2;
        if (obj->unique_id == 9) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 0:
        *globalSlot = 16;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 44:
        *globalSlot = 17;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 35:
        *globalSlot = 15;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 47:
        *globalSlot = 18;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 53:
        *globalSlot = 19;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 40:
        *globalSlot = 14;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 27:
        *globalSlot = 10;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 30:
        *globalSlot = 11;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 38:
        *globalSlot = 12;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 28:
        *globalSlot = 13;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 14:
        *globalSlot = 7;
        found |= 2;
        switch (obj->unique_id) {
        case 8:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 1:
            *uniqueSlot = 1;
            found |= 1;
            break;
        }
        break;
    case 4:
        *globalSlot = 6;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 7:
        *globalSlot = 5;
        found |= 2;
        if (obj->unique_id == 16) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 12:
        *globalSlot = 3;
        found |= 2;
        break;
    case 2:
        *globalSlot = 0;
        found |= 2;
        switch (obj->unique_id) {
        case 9:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 7:
            *uniqueSlot = 1;
            found |= 1;
            break;
        case 14:
        case 15:
        case 23:
        case 25:
            *uniqueSlot = 2;
            found |= 1;
            break;
        case 13:
            *uniqueSlot = 3;
            found |= 1;
            break;
        }
        break;
    case 10:
        *globalSlot = 1;
        found |= 2;
        switch (obj->unique_id) {
        case 6:
            *uniqueSlot = 0;
            found |= 1;
            break;
        case 7:
            *uniqueSlot = 1;
            found |= 1;
            break;
        case 8:
            *uniqueSlot = 2;
            found |= 1;
            break;
        case 9:
            *uniqueSlot = 3;
            found |= 1;
            break;
        case 10:
            *uniqueSlot = 4;
            found |= 1;
            break;
        case 11:
            *uniqueSlot = 5;
            found |= 1;
            break;
        case 12:
            *uniqueSlot = 6;
            found |= 1;
            break;
        case 13:
            *uniqueSlot = 7;
            found |= 1;
            break;
        case 15:
            *uniqueSlot = 8;
            found |= 1;
            break;
        }
        break;
    case 6:
        *globalSlot = 2;
        found |= 2;
        break;
    case 16:
        *globalSlot = 4;
        found |= 2;
        if (obj->unique_id == 6) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 20:
        *globalSlot = 8;
        found |= 2;
        if (obj->unique_id == 16) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    case 23:
        *globalSlot = 9;
        found |= 2;
        if (obj->unique_id == 1) {
            *uniqueSlot = 0;
            found |= 1;
        }
        break;
    }
    return found;
}

/**
 * Runs the spawn callbacks for `obj` (see func_1514CA80), after freeing whatever is still
 * queued for its slot. Does nothing if the object already has attachments.
 */
s32 func_1514D15C(Actor *obj) {
    s32 uniqueSlot;
    s32 globalSlot;
    s32 idSlot;
    u8 flags;
    s32 found;
    s32 slot;
    s32 handler;
    PlayerMarker *queued;

    uniqueSlot = -1;
    globalSlot = -1;
    idSlot = -1;
    slot = obj - movement_obj;
    if (obj->attachments != NULL) {
        return 0;
    }

    do {
        queued = func_15155FD4(slot);
        if (queued != NULL) {
            func_1516972C(&queued->header);
        }
    } while (queued != NULL);

    if (slot == 0 && D_800BE616 == 0) {
        func_15155780(slot, 0xFF);
    }

    D_8008AACC[0](obj);
    // matching: the zero test must be on the s32 result, not on the u8 copy
    found = func_1514CA80(obj, &uniqueSlot, &globalSlot, &idSlot);
    flags = found;
    if (found == 0) {
        return 0;
    }

    if (flags & 4) {
        handler = D_800A5ED8[idSlot];
        if (handler != 0) {
            D_8008AA70[handler](obj);
        }
    }

    if (flags & 2) {
        handler = D_800A5EC0[globalSlot];
        if (handler != 0) {
            D_8008AA70[handler](obj);
        }
    }

    if ((flags & 2) && (flags & 1)) {
        if (D_8008AA70[D_8008AB90[globalSlot][uniqueSlot]] != NULL) {
            D_8008AA70[D_8008AB90[globalSlot][uniqueSlot]](obj);
        }
    }
    return 1;
}

/** Frees every attachment of `owner`, letting each attachment type release its handle first. */
s32 func_1514D310(Actor *owner) {
    Attachment *node;
    Attachment *next;

    node = owner->attachments;
    while (node != NULL) {
        next = node->next;
        if (D_8008ABE8[node->type]->destroyHandler != -1) {
            D_8008AB58[D_8008ABE8[node->type]->destroyHandler](node->handle);
        }
        func_1516972C(&node->header);
        node = next;
    }
    return 1;
}

/**
 * Sends message `msg` (with `arg`) to every attachment of `owner` whose type is `type` or one
 * of its subtypes. Returns the result of the last handler called, as 0 or 1.
 */
s32 func_1514D3B0(Actor *owner, s16 type, s32 msg, s32 arg) {
    Attachment *node;
    s32 result;

    result = 0;
    for (node = owner->attachments; node != NULL; node = node->next) {
        if (type == node->type || func_1514D4B8(D_8008ABE8[type]->firstChild, node->type)) {
            if (D_8008ABE8[node->type]->messageHandler != -1) {
                if (D_8008AB64[D_8008ABE8[node->type]->messageHandler](node->handle, msg, arg)) {
                    result = 1;
                } else {
                    result = 0;
                }
            }
        }
    }
    return result;
}

/** Returns whether attachment type `type` occurs in the type subtree rooted at `root` or its siblings. */
s32 func_1514D4B8(s16 root, s16 type) {
    if (root == -1) {
        return 0;
    }
    if (root == type) {
        return 1;
    }
    if (func_1514D4B8(D_8008ABE8[root]->nextSibling, type)) {
        return 1;
    }
    if (func_1514D4B8(D_8008ABE8[root]->firstChild, type)) {
        return 1;
    }
    return 0;
}

/** Spawns a func_150C5370 object running from a fixed point to `obj` and attaches it (type 0x15). */
void func_1514D564(Actor *obj) {
    StripDesc desc;
    StripObj *spawned;
    StripTarget payload;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.offset.x = 0.0f;
    payload.offset.y = -180.0f;
    payload.offset.z = 77.0f;
    desc.p0.x = -477.0f;
    desc.p0.y = -2015.0f;
    desc.p0.z = 2244.0f;
    payload.bone = 6;
    desc.updateIdx = 1;
    desc.p1.x = obj->xpos;
    desc.p1.y = obj->ypos;
    desc.p1.z = obj->zpos;
    desc.unk20 = 1;
    desc.texScale = 0.3199999928f;
    spawned = func_150C5370(&desc, sizeof(payload));
    if (spawned != NULL) {
        memcpy(&spawned->target, &payload, sizeof(payload));
        func_1514EC1C(spawned, obj, 0x15);
    }
}

/** Attaches two emitters to `obj` (bones 0x2B and 5, types 9 and 10) plus a func_15160A58 effect. */
void func_1514D64C(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12012;
    spawn.lifetime = 0;
    spawn.unk45 = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 28.0f;
    emitter.height1 = 28.0f;
    emitter.offset1 = 0.5f;
    emitter.offset0 = 0.5f;
    emitter.width0 = 4.0f;
    emitter.height0 = 17.0f;
    emitter.alpha1 = 0.75f;
    emitter.alpha0 = 0.25f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.bone = 0x2B;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 1;

    attach.p0.z = 25.0f;
    attach.p1.z = 75.0f;
    emitter.scale = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 9);

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12012;
    spawn.lifetime = 0;
    spawn.unk45 = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;

    emitter.width1 = 30.0f;
    emitter.height1 = 30.0f;
    emitter.width0 = 10.0f;
    emitter.offset1 = 0.8000000119f;
    emitter.offset0 = 0.8000000119f;

    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.alpha1 = 0.6000000238f;
    emitter.alpha0 = 0.6000000238f;
    emitter.height0 = 16.0f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.bone = 5;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 0;
    attach.p0.y = 10.0f;
    attach.p0.x = 0.0f;
    attach.p1.x = 0.0f;
    attach.p0.z = -70.0f;
    attach.p1.y = 20.0f;
    attach.p1.z = -140.0f;
    emitter.scale = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 0xA);
    func_15160A58(obj, 5, &attach.p0, 2, 300, 40, 0xFF, 0, 0, 0xFF, 0, 1, 0, 0, 0xFF, 1);
}

void func_1514D96C(Actor *obj) {
}

/** Attaches a func_15158BD0 object (type 0x13) with a zeroed 0x20-byte payload. */
void func_1514D978(Actor *obj) {
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        f32 unk10;
        s32 unk14;
        s32 unk18;
        s32 unk1C;
    } payload;
    void *handle;

    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = 0;
    payload.unkC = 0;
    payload.unk14 = 0;
    payload.unk18 = 0;
    payload.unk1C = 0;
    payload.unk10 = 12.0f;

    handle = func_15158BD0(obj, 1, sizeof(payload));
    if (handle != NULL) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        func_1514EC1C(handle, obj, 0x13);
    }
}

void func_1514D9F4(Actor *obj) {
    void *handle;

    func_1514D978(obj);
    handle = func_151ACA60(obj, 20.0f, 0);
    func_1514EC1C(handle, obj, 0x14);
}

/** Attaches a func_15158BD0 object (type 0x13) with a zeroed 0x1C-byte payload. */
void func_1514DA38(Actor *obj) {
    void *handle;
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        s32 unk10;
        s32 unk14;
        s32 unk18;
    } payload;

    payload.unk14 = 0;
    payload.unk18 = 0;
    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = 0;
    payload.unkC = 0;
    handle = func_15158BD0(obj, 1, sizeof(payload));
    if (handle != NULL) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        func_1514EC1C(handle, obj, 0x13);
    }
}

/** Sets bit 1 of obj->unk94 and attaches a func_15158BD0 object (type 0x13) with an 8-byte payload. */
void func_1514DAA4(Actor *obj) {
    void *handle;
    s32 payload[2];

    obj->unk94 |= 2;
    payload[0] = 0;
    payload[1] = 0;
    {
        // matching: a declaration here opens a scope block, which changes how as1 schedules the argument setup
        extern CharacterWatcher *func_15158BD0(Actor *chr, u8 enableActions, s32 extraSize);

        if ((handle = func_15158BD0(obj, 1, sizeof(payload))) != NULL) {
            memcpy((u8 *)handle + 0x58, payload, sizeof(payload));
            func_1514EC1C(handle, obj, 0x13);
        }
    }
}

/** Attaches a func_15158BD0 object (type 0x13) without payload. */
void func_1514DB18(Actor *obj) {
    void *handle = func_15158BD0(obj, 1, 0);

    if (handle != NULL) {
        func_1514EC1C(handle, obj, 0x13);
    }
}

void func_1514DB58(Actor *obj) {
}

void func_1514DB64(Actor *obj) {
    if (level == 20) {
        func_151B2060(obj);
    }
}

void func_1514DB98(Actor *obj) {
    func_1514F194(obj);
}

void func_1514DBB8(Actor *obj) {
    func_15160A58(obj, 2, &D_800A58A0, 2, 300, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

void func_1514DC38(Actor *obj) {
    func_1500EE18(obj, 0xFF, 1);
}

void func_1514DC5C(Actor *obj) {
    func_151D0F60(obj, 0, 0xFF, 1);
}

void func_1514DC84(Actor *obj) {
    obj->unk94 |= 2;
}

void func_1514DC98(Actor *obj) {
    obj->unk94 |= 0x710;
}

/** Sets obj->unk9C to 0x6000 and spawns part sets 0x23 and 0x44 on `obj`. */
void func_1514DCAC(Actor *obj) {
    obj->unk9C = 0x6000;
    func_15083568(obj, 0x23, 1.0f, 0);
    func_15083568(obj, 0x44, 1.0f, 0);
}

void func_1514DCF4(Actor *obj) {
    if (obj->mv != NULL) {
        obj->mv->unk94 = 1;
    }
    func_15083568(obj, 0x17, 1.0f, 0);
}

void func_1514DD2C(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DD4C(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DD6C(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DD8C(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DDAC(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DDCC(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DDEC(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DE0C(Actor *obj) {
    func_1514DCAC(obj);
}

void func_1514DE2C(Actor *obj) {
}

void func_1514DE38(Actor *obj) {
}

void func_1514DE44(Actor *obj) {
}

void func_1514DE50(Actor *obj) {
    if (D_800D2E4C[0x11] & 8) { // low byte of D_800D2E4C[0x10]
        func_1514DCAC(obj);
    }
}

void func_1514DE88(Actor *obj) {
}

void func_1514DE94(Actor *obj) {
    ModelAttachment *part;

    func_15160CDC(obj, 12, &D_800A58AC, &D_800A58B8, 0.008333000354f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 0, 1, 1, 0xFF, 1);
    part = func_15083568(obj, 0x3C, 1.0f, 0);
    if (D_800BE9B4 == 0) {
        part->unk38 = 2400;
    }
    func_1514EC1C(func_15160A58(obj, 0, &D_800A58C4, 2, 300, 11, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1), obj, 0x19);
}

/** Links `obj` to the object whose unique_id is 9 (unk65 holds that object's slot + 1). */
void func_1514DFD0(Actor *obj) {
    obj->attach_char = func_15083FB0(9) + 1;
    obj->unk101 |= 0x34;
}

void func_1514E00C(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12002;
    spawn.lifetime = 0;
    spawn.unk45 = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0x8E;
    spawn.rgba[2] = 0x19;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 20.0f;
    emitter.height1 = 20.0f;
    emitter.width0 = 20.0f;
    emitter.height0 = 20.0f;
    emitter.offset1 = 0.0f;
    emitter.offset0 = 0.0f;
    emitter.alpha1 = 0.5f;
    emitter.alpha0 = 0.5f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.scale = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E194(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x12002;
    spawn.lifetime = 0;
    spawn.unk45 = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0x8E;
    spawn.rgba[2] = 0x19;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 16.0f;
    emitter.height1 = 16.0f;
    emitter.width0 = 16.0f;
    emitter.height0 = 16.0f;
    emitter.offset1 = 0.0f;
    emitter.offset0 = 0.0f;
    emitter.alpha1 = 0.5f;
    emitter.alpha0 = 0.5f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.scale = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E31C(Actor *obj) {
    GlowShape emitter;
    ParticleDef spawn;
    EmitterAttach attach;

    spawn.texture = 3;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.flags = 0x2012;
    spawn.lifetime = 0;
    spawn.unk45 = 0xFF;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0xE6;
    spawn.rgba[2] = 0;
    spawn.rgba[3] = 0xFF;
    spawn.renderModes = 0x401;
    spawn.pos.x = 0.0f;
    spawn.pos.y = 0.0f;
    spawn.pos.z = 0.0f;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;

    emitter.width1 = 45.0f;
    emitter.height0 = 45.0f;
    emitter.offset1 = 0.5f;
    emitter.offset0 = 0.5f;
    emitter.alpha0 = 0.0f;
    emitter.height1 = 55.0f;
    emitter.width0 = 25.0f;
    emitter.alpha1 = 0.8999999762f;
    emitter.fadeRate = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.target = obj;
    attach.bone = 0xC;
    emitter.visibility[0] = 0;
    emitter.visibility[1] = 0;
    emitter.visibility[2] = 0;
    emitter.visibility[3] = 0;
    attach.handler = 2;

    attach.p0.z = 100.0f;
    attach.p1.z = 600.0f;
    emitter.scale = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 3, 0, 0), obj, 7);

    {
        Vec3f offset;

        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 150.0f;
        func_15160A58(obj, 12, &offset, 2, 300, 40, 0xFF, 0xB5, 0, 0xFF, 0, 2, 0, 0, 0xFF, 1);
    }
}

/** Attaches the "Charactereffect 0" effect (type 0xD) and a func_1515D5F8 object tracking obj's position (type 0x18). */
void func_1514E508(Actor *obj) {
    void *handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 0");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E5B8(Actor *obj) {
    void *handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect -1");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E668(Actor *obj) {
    void *handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 1");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E718(Actor *obj) {
    void *handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 2");
    func_1514EC1C(handle, obj, 0xE);
    pos = (s32)&obj->xpos; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E7C8(Actor *obj) {
    void *handle;

    handle = func_1518D1C0(obj, 7, 0, 1, 0xFF, 1, "Charactereffect 3");
    func_1514EC1C(handle, obj, 0xF);
}

/* Emitter offsets of func_1514EECC and func_1514F194. */
static const Vec3f D_800A5934 = { 0.0f, 90.0f, 0.0f };
static const Vec3f D_800A5940 = { 0.0f, 70.0f, 0.0f };
static const Vec3f D_800A594C = { 0.0f, 120.0f, 0.0f };
static const Vec3f D_800A5958 = { 0.0f, 112.0f, 151.0f };
static const Vec3f D_800A5964 = { 0.0f, 74.0f, -161.0f };

void func_1514E824(Actor *obj) {
}

/* Attachment destroy handlers (D_8008AB58 entries): release an attachment's handle. */
void func_1514E830(EffectHeader *handle) {
    func_1516972C(handle);
}

void func_1514E850(void *handle) {
    func_1518E308(handle);
    func_1516972C(handle);
}

void func_1514E87C(void *handle) {
    func_1515F10C(handle);
}

/* Attachment message handlers (D_8008AB64 entries). Each returns 1 if it handled `msg`. */

struct ToggleTarget {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 flags;
    /* 0x014 */ u8 pad14[0xFC];
    /* 0x110 */ GlowShape emitter;    // for objects spawned by func_15189FF0
};

/** Messages 0, 1 and 2 toggle, set and clear bit 0 of the handle's flags. */
s32 func_1514E89C(ToggleTarget *target, s32 msg, s32 arg) {
    s32 flags;

    switch (msg) {
    case 0:
        flags = target->flags;
        if (flags & 1) {
            target->flags = flags & ~1;
        } else {
            target->flags = flags | 1;
        }
        break;
    case 2:
        target->flags &= ~1;
        break;
    case 1:
        target->flags |= 1;
        break;
    default:
        return 0;
    }
    return 1;
}

/** Like func_1514E89C, but clearing bit 0 also hides the glow's attached lens flare. */
s32 func_1514E920(s32 handle, s32 msg, s32 arg) {
    // matching: with an s32 parameter IDO keeps &target->unk110 in its own register
    ToggleTarget *target = (ToggleTarget *)handle;
    GlowShape *emitter;
    s32 flags;
    LensFlare *flare;

    emitter = &target->emitter;
    switch (msg) {
    case 0:
        flags = target->flags;
        if (flags & 1) {
            target->flags = flags & ~1;
            flare = emitter->attach;
            if (flare != NULL) {
                flare->alpha = 0;
            }
        } else {
            target->flags = flags | 1;
        }
        break;
    case 2:
        target->flags &= ~1;
        flare = emitter->attach;
        if (flare != NULL) {
            flare->alpha = 0;
        }
        break;
    case 1:
        target->flags |= 1;
        break;
    default:
        return func_1514E89C(target, msg, arg);
    }
    return 1;
}

s32 func_1514E9DC(s32 handle, s32 msg, s32 arg) {
    return func_1514E920(handle, msg, arg);
}

s32 func_1514E9FC(ToggleTarget *target, s32 msg, s32 arg) {
    return func_1514E89C(target, msg, arg);
}

/** Messages 3..12 switch the emitter between table 2 and tables 3, 5 and 6. */
s32 func_1514EA1C(ActorEmitterProcess *target, s32 msg, s32 arg) {
    switch (msg) {
    case 3:
        if (target->params.table == 2) {
            func_1518E37C(target, 3);
        } else {
            func_1518E37C(target, 2);
        }
        break;
    case 4:
        func_1518E37C(target, 3);
        break;
    case 5:
        func_1518E37C(target, 2);
        break;
    case 6:
        if (target->params.table == 2) {
            func_1518E37C(target, 5);
        } else {
            func_1518E37C(target, 2);
        }
        break;
    case 7:
        func_1518E37C(target, 5);
        break;
    case 8:
        func_1518E37C(target, 2);
        break;
    case 9:
        if (target->params.table == 2) {
            func_1518E37C(target, 6);
        } else {
            func_1518E37C(target, 2);
        }
        break;
    case 10:
        func_1518E37C(target, 6);
        break;
    case 11:
        func_1518E37C(target, 2);
        break;
    case 12:
        func_1518E37C(target, 2);
        break;
    default:
        return func_1514E89C((ToggleTarget *)target, msg, arg);
    }
    return 1;
}

s32 func_1514EB6C(ToggleTarget *target, s32 msg, s32 arg) {
    return func_1514E89C(target, msg, arg);
}

s32 func_1514EB8C(s32 handle, s32 msg, s32 arg) {
    return 1;
}

/** Allocates an unlinked attachment node for `handle` with `extraSize` bytes of payload. */
Attachment *func_1514EBA4(void *handle, s16 type, s32 extraSize) {
    Attachment *node;

    node = func_15167A68(0x24, 1, extraSize + 0x20, 1, 0xFF, 1);
    if (node == NULL) {
        return NULL;
    }
    node->handle = handle;
    node->next = NULL;
    node->prev = NULL;
    node->type = type;
    return node;
}

/**
 * Attaches `handle` to `owner` as an attachment of `type` (pushed at the head of its list).
 * If no node can be allocated, the handle is released instead. Returns the node or NULL.
 */
Attachment *func_1514EC1C(void *handle, Actor *owner, s16 type) {
    Attachment *result;
    Attachment *node;

    result = NULL;
    if (handle != NULL) {
        node = func_1514EBA4(handle, type, 0);
        result = node;
        if (node != NULL) {
            node->next = owner->attachments;
            owner->attachments = node;
            node->prev = NULL;
            if (node->next != NULL) {
                node->next->prev = node;
            }
        } else if (D_8008ABE8[type]->destroyHandler != -1) {
            D_8008AB58[D_8008ABE8[type]->destroyHandler](handle);
        } else {
            func_1516972C(handle);
        }
    }
    return result;
}

/** Finds the first attachment of `type` in the list starting at `list`; stores it (or NULL) in *out. */
s32 func_1514ECE0(Attachment *list, s16 type, Attachment **out) {
    s32 found;
    Attachment *node;
    Attachment *next;

    found = FALSE;
    node = list;
    if (node != NULL) {
        do {
            next = node->next;
            if (type == node->type) {
                found = TRUE;
            } else {
                node = next;
            }
        } while (node != NULL && !found);
    }

    if (out != NULL) {
        *out = node;
    }
    return found;
}

/** Finds the first attachment holding `handle` in the list starting at `list`; stores it (or NULL) in *out. */
s32 func_1514ED3C(Attachment *list, void *handle, Attachment **out) {
    s32 found = FALSE;
    Attachment *node = list;
    Attachment *next;

    if (node != NULL) {
        do {
            next = node->next;
            if (handle == node->handle) {
                found = TRUE;
            } else {
                node = next;
            }
        } while (node != NULL && !found);
    }
    if (out != NULL) {
        *out = node;
    }
    return found;
}

/** Unlinks `node` from `owner`'s attachment list and frees it. Returns the node's handle. */
void *func_1514ED8C(Attachment *node, Actor *owner) {
    void *handle;

    if (node == owner->attachments) {
        owner->attachments = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    handle = node->handle;
    func_1516972C(&node->header);
    return handle;
}

/** Removes (without releasing the handle) every attachment of `owner` that holds `handle`. */
void func_1514EDF0(void *handle, Actor *owner) {
    Attachment *node;
    s32 ret;
    Attachment *match;
    u8 found;

    node = owner->attachments;
    match = NULL;
    do {
        // matching: test the s32 result, loop on the u8 copy
        ret = func_1514ED3C(node, handle, &match);
        found = ret;
        if (ret) {
            node = match->next;
            func_1514ED8C(match, owner);
        }
    } while (found);
}

/** Attaches a func_1515BE50 object for `obj` (type 0x16). */
void func_1514EE70(Actor *obj) {
    ActorTrackerDesc desc;
    ActorTracker *handle;

    desc.actor = obj;
    desc.actorId = obj->unique_id;
    desc.flags = 0;
    desc.lifetime = 300;
    handle = func_1515BE50(&desc, 0, 0xFF, 1);
    func_1514EC1C(handle, obj, 0x16);
}


void func_1514EECC(Actor *obj) {
    void *handle;

    handle = func_15160A58(obj, 1, &D_800A5934, 2, 300, 2, 0xFF, 0, 0, 0xFF, 0, 3, 0, 0, 0xFF, 1);
    func_1514EC1C(handle, obj, 0x19);
    {
        ParticleDef spawn;
        GlowShape emitter;
        EmitterAttach attach;

        spawn.texture = 0x69;
        spawn.flags = 0x04ED200A;
        spawn.lifetime = 300;
        spawn.frame = 0;
        spawn.frameSpeed = 0;
        spawn.unk45 = 0xFF;
        spawn.rgba[0] = 0xFF;
        spawn.rgba[1] = 0;
        spawn.rgba[2] = 0;
        spawn.rgba[3] = 0xFF;
        spawn.renderModes = 0x4403;
        spawn.pos.x = 0.0f;
        spawn.pos.y = 0.0f;
        spawn.pos.z = 0.0f;
        spawn.rot.x = 0.0f;
        spawn.rot.y = 0.0f;
        spawn.rot.z = 0.0f;
        spawn.type = 3;
        spawn.height = 100.0f;
        spawn.width = 100.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.unk44 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        spawn.unk48 = 0;
        spawn.viewportMask = 0xFF;
        spawn.unk50 = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;

        emitter.height1 = 95.0f;
        emitter.width1 = 95.0f;
        emitter.height0 = 65.0f;
        emitter.width0 = 65.0f;
        emitter.offset1 = 0.150000006f;
        emitter.offset0 = 0.150000006f;
        emitter.alpha1 = 0.6999999881f;
        emitter.alpha0 = 0.400000006f;
        emitter.fadeRate = 9.999999975e-07f;

        attach.targetId = obj->unique_id;
        attach.target = obj;
        attach.p0 = D_800A5940;
        attach.p1 = D_800A594C;
        attach.bone = 1;
        emitter.visibility[0] = 0;
        emitter.visibility[1] = 0;
        emitter.visibility[2] = 0;
        emitter.visibility[3] = 0;
        emitter.scale = 1.0f;
        attach.handler = 3;

        func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0);
    }
}

void func_1514F110(Actor *obj) {
    func_1514F194(obj);
}

/* Attachment message handler for a light effect (e.g. func_15160A58's): 13 turns the light on, 14 off. */
s32 func_1514F130(LightEffect *target, s32 msg, s32 arg) {
    switch (msg) {
    case 13:
        target->light->disabled = 0;
        break;
    case 14:
        target->light->disabled = 1;
        break;
    default:
        return func_1514E89C((ToggleTarget *)target, msg, arg);
    }
    return 1;
}

void func_1514F194(Actor *obj) {
    void *handle;

    func_151D0F60(obj, 1, 0xFF, 1);
    func_151D0F60(obj, 2, 0xFF, 1);
    func_151BB044(obj);
    if (D_800BE616 == 0) {
        func_151BC074(obj);
    }
    func_151BD2F8(obj, 0xFF, 1);
    func_151B9F00(obj, 0xFF, 1);
    func_15160A58(obj, 1, &D_800A5958, 2, 300, 12, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1);
    handle = func_15160A58(obj, 1, &D_800A5964, 2, 300, 15, 0xFF, 0, 0, 0xFF, 1, -1, 0, 0, 0xFF, 1);
    if (handle != NULL) {
        func_1514EC1C(handle, obj, 0x19);
    }
}

void func_1514F308(Actor *obj) {
    const Vec3f pointA = { 52.0f, 0.0f, 80.0f };
    const Vec3f pointB = { 52.0f, 0.0f, 353.0f };

    func_15160CDC(obj, 5, &pointA, &pointB, 0.02350199968f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
}

void func_1514F3CC(Actor *obj) {
    struct {
        Actor *obj;
        u8 uniqueId;
        f32 unk8;
    } payload;
    TimedEffect *spawned;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.unk8 = 0.0f;

    // the seventh argument is the payload size
    spawned = func_15149130(300, -1, 0x3A, -1, 0, 0x2B, sizeof(payload), 0xFF, 1);
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0x28, &payload, sizeof(payload));
    }
}

void func_1514F44C(Actor *obj) {
    void *handle;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect frontend fire imp");
    func_1514EC1C(handle, obj, 0xD);
}

void func_1514F4A8(Actor *obj) {
    func_151D74B0(obj, 1, -1, 0xFF, 1);
}

void func_1514F4D8(Actor *obj) {
    func_151D74B0(obj, 2, 1, 0xFF, 1);
}

void func_1514F508(Actor *obj) {
    func_151D74B0(obj, 3, 0, 0xFF, 1);
}

void func_1514F538(Actor *obj) {
    func_151D74B0(obj, 4, -1, 0xFF, 1);
}

void func_1514F568(Actor *obj) {
    func_150C4120(obj, -1, 0xFF, 1);
}

void func_1514F590(Actor *obj) {
    func_1501175C(obj, 0xFF, 1);
    func_15011A78(obj, 0xFF, 1);
}

void func_1514F5CC(Actor *obj) {
    PoolEmitterDesc desc;

    desc.source = NULL;
    desc.target = obj;
    desc.unk4 = 0.0f;
    desc.targetId = obj->unique_id;
    desc.rate = 0.349999994f;
    desc.slotCount = 20;
    desc.timer = 300;
    desc.flags = 4;
    func_150C0AC0(&desc, 0xFF, 1);
}
