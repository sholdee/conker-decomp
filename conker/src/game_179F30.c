#include <ultra64.h>
// functions.h declares these two with other signatures; this file uses its own prototypes
#define func_15083568 func_15083568_orig
#define func_1514D3B0 func_1514D3B0_orig
#include "functions.h"
#undef func_15083568
#undef func_1514D3B0
#include "variables.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* One node of an object's attachment list: something spawned for the object (an effect, a
 * sound, a sub-object) together with the attachment type that knows how to message or free it. */
typedef struct Attachment {
    /* 0x00 */ u8 pad0[0x10];               // allocator header
    /* 0x10 */ s32 handle;                  // the attached thing, passed to the type's handlers
    /* 0x14 */ struct Attachment *next;
    /* 0x18 */ struct Attachment *prev;
    /* 0x1C */ s16 type;                    // index into D_8008ABE8
} Attachment;

/* Game objects keep the head of their attachment list at +0x2F4 (padding in struct127). */
typedef struct AttachmentOwner {
    /* 0x000 */ u8 pad0[0x2F4];
    /* 0x2F4 */ Attachment *attachments;
} AttachmentOwner;
#define ATTACHMENTS(obj) (((AttachmentOwner *)(obj))->attachments)

/* Attachment types form a tree (first child / next sibling), so a message sent to a type
 * also reaches attachments of its subtypes. */
typedef struct AttachmentType {
    /* 0x0 */ s16 messageHandler;           // index into D_8008AB64, or -1
    /* 0x2 */ s16 destroyHandler;           // index into D_8008AB58, or -1
    /* 0x4 */ s16 firstChild;               // -1 terminates
    /* 0x6 */ s16 nextSibling;              // -1 terminates
} AttachmentType;

/* Tells a spawned emitter how to follow its owner: two points (p0, p1) given in the space of
 * bone `bone` of `target`, which func_1518A094 transforms every frame. */
typedef struct EmitterAttach {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ struct127 *target;
    /* 0x1C */ u8 bone;
    /* 0x1D */ u8 targetId;             // the target's unique_id
    /* 0x1E */ s8 handler;              // index into D_8008D5A0, or -1
    /* 0x1F */ u8 pad1F;
} EmitterAttach;

/* Object spawn parameters, passed on to func_1513D524 by func_151407D0. */
typedef struct SpawnParams {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;                 // unk10..unk13 hold RGBA-like values
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} SpawnParams;

/* Emitter parameters; func_151407D0 copies them into the spawned object at +0x110. */
typedef struct EmitterParams {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x20];
    /* 0x44 */ u8 *unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 pad58[4];
} EmitterParams;

/* What func_15083568 returns, seen as a raw view (struct127 has a u8 at 0x38). */
struct St1514DE94 {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 unk38;
};

typedef struct Struct1514F5CCArg {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ struct127 *obj;
    /* 0x0C */ u8 uniqueId;
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
} Struct1514F5CCArg;

typedef struct HudSpriteState {
    /* 0x0 */ u8 pad0[0x9];
    /* 0x9 */ u8 unk9;
} HudSpriteState;

typedef struct HudSpriteHandle {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ HudSpriteState *state;
} HudSpriteHandle;

/* Spawn-callback tables (see func_1514D15C). */
extern void (*D_8008AA70[])(struct127 *);
extern void (*D_8008AACC[])(struct127 *);
extern u8 *D_8008AB90[];
extern u8 D_800A5EC0[];
extern u8 D_800A5ED8[];

/* Attachment type tables. */
extern void (*D_8008AB58[])(s32);
extern s32 (*D_8008AB64[])(s32, s32, s32);
extern AttachmentType *D_8008ABE8[];

/* Emitter offsets. */
extern Vec3f D_800A58A0;
extern Vec3f D_800A58AC;
extern Vec3f D_800A58B8;
extern Vec3f D_800A58C4;
extern Vec3f D_800A5934;
extern Vec3f D_800A5940;
extern Vec3f D_800A594C;
extern Vec3f D_800A5958;
extern Vec3f D_800A5964;

/* This file. */
s32 func_1514CA80(struct127 *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot);
s32 func_1514D4B8(s16 root, s16 type);
Attachment *func_1514EC1C(s32 handle, struct127 *owner, s16 type);
void func_1514F194(struct127 *obj);

/* Elsewhere. */
void func_1501175C(struct127 *obj, u8 arg1, s32 arg2);
struct St1514DE94 *func_15083568(struct127 *obj, s32 arg1, f32 arg2, s32 arg3);
s32 func_15083FB0(u8 uniqueId);
void func_150C0AC0(Struct1514F5CCArg *desc, s32 arg1, s32 arg2);
void func_150C4120(struct127 *obj, s32 arg1, s32 arg2, s32 arg3);
s32 func_150C5370(void *desc, s32 payloadSize);
void *func_15155780(s32 slot, s32 arg1);
struct102 *func_15155FD4(s32 slot);
s32 func_15158BD0(struct127 *obj, s32 arg1, s32 payloadSize);
void func_1515F10C(void *);
s32 func_15160A58(struct127 *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_15160CDC(struct127 *, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void *func_15167A68(s32, s32, s32, s32, s32, s32);
s32 func_15189FF0(SpawnParams *spawn, EmitterParams *emitter, EmitterAttach *attach, s32, s32, s32, s32);
s32 func_1515D5F8(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_1518D1C0(struct127 *obj, s32, s32, s32, s32, s32, char *name);
void func_1518E308(void *);
s32 func_151ACA60(struct127 *obj, f32 arg1, s32 arg2);
void func_151B2060(struct127 *obj);
void func_151B9F00(struct127 *obj, s32, s32);
void func_151BB044(struct127 *obj);
void func_151BC074(struct127 *obj);
void func_151BD2F8(struct127 *obj, s32, s32);
void func_151D0F60(struct127 *obj, s32, s32, s32);
void func_151D74B0(struct127 *obj, s32, s32, s32, s32);




/**
 * Finds which spawn-callback slots apply to `obj`: one chosen by its object id (*idSlot, flag 4),
 * one chosen by D_800BE9F0 (*globalSlot, flag 2) and, inside that, one chosen by its unique_id
 * (*uniqueSlot, flag 1). Returns the OR of the flags that were set.
 */
s32 func_1514CA80(struct127 *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot) {
    u8 found = 0;

    switch (obj->id) {
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

    switch (D_800BE9F0) {
    case 43:
        *globalSlot = 21;
        found |= 2;
        if (obj->id == 156 || obj->id == 157) {
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
s32 func_1514D15C(struct127 *obj) {
    s32 uniqueSlot;
    s32 globalSlot;
    s32 idSlot;
    u8 flags;
    s32 found;
    s32 slot;
    s32 handler;
    struct102 *queued;

    uniqueSlot = -1;
    globalSlot = -1;
    idSlot = -1;
    slot = obj - D_800CC2D0;
    if (ATTACHMENTS(obj) != NULL) {
        return 0;
    }

    do {
        queued = func_15155FD4(slot);
        if (queued != NULL) {
            func_1516972C(queued);
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
s32 func_1514D310(struct127 *owner) {
    Attachment *node;
    Attachment *next;

    node = ATTACHMENTS(owner);
    while (node != NULL) {
        next = node->next;
        if (D_8008ABE8[node->type]->destroyHandler != -1) {
            D_8008AB58[D_8008ABE8[node->type]->destroyHandler](node->handle);
        }
        func_1516972C((struct102 *)node);
        node = next;
    }
    return 1;
}

/**
 * Sends message `msg` (with `arg`) to every attachment of `owner` whose type is `type` or one
 * of its subtypes. Returns the result of the last handler called, as 0 or 1.
 */
s32 func_1514D3B0(struct127 *owner, s16 type, s32 msg, s32 arg) {
    Attachment *node;
    s32 result;

    result = 0;
    for (node = ATTACHMENTS(owner); node != NULL; node = node->next) {
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

typedef struct Struct1514D564Payload {
    struct127 *obj;
    u8 uniqueId;
    s8 unk5;
    u8 pad6[2];
    f32 unk8;
    f32 unkC;
    f32 unk10;
} Struct1514D564Payload;

/* func_150C5370 copies the first 0x24 bytes and caches the distance between p0 and p1. */
typedef struct Struct1514D564Desc {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ s8 unk18;
    u8 pad19[3];
    f32 unk1C;
    s8 unk20;
    u8 pad21[3];
} Struct1514D564Desc;

/** Spawns a func_150C5370 object running from a fixed point to `obj` and attaches it (type 0x15). */
void func_1514D564(struct127 *obj) {
    Struct1514D564Desc desc;
    s32 spawned;   // the object func_150C5370 allocated; the payload goes at +0xC8
    Struct1514D564Payload payload;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.unk8 = 0.0f;
    payload.unkC = -180.0f;
    payload.unk10 = 77.0f;
    desc.p0.x = -477.0f;
    desc.p0.y = -2015.0f;
    desc.p0.z = 2244.0f;
    payload.unk5 = 6;
    desc.unk18 = 1;
    desc.p1.x = obj->x_position;
    desc.p1.y = obj->y_position;
    desc.p1.z = obj->z_position;
    desc.unk20 = 1;
    desc.unk1C = 0.3199999928f;
    spawned = func_150C5370(&desc, sizeof(payload));
    if (spawned != 0) {
        memcpy((u8 *)spawned + 0xC8, &payload, sizeof(payload));
        func_1514EC1C(spawned, obj, 0x15);
    }
}

/** Attaches two emitters to `obj` (bones 0x2B and 5, types 9 and 10) plus a func_15160A58 effect. */
void func_1514D64C(struct127 *obj) {
    EmitterParams emitter;
    SpawnParams spawn;
    EmitterAttach attach;

    spawn.unk00 = 3;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk40 = 0x12012;
    spawn.unk04 = 0;
    spawn.unk45 = 0xFF;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0;
    spawn.unk12 = 0;
    spawn.unk13 = 0xFF;
    spawn.unk02 = 0x401;
    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;

    emitter.unk00 = 28.0f;
    emitter.unk04 = 28.0f;
    emitter.unk10 = 0.5f;
    emitter.unk14 = 0.5f;
    emitter.unk08 = 4.0f;
    emitter.unk0C = 17.0f;
    emitter.unk18 = 0.75f;
    emitter.unk1C = 0.25f;
    emitter.unk20 = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.bone = 0x2B;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = 1;

    attach.p0.z = 25.0f;
    attach.p1.z = 75.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 9);

    spawn.unk00 = 3;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk40 = 0x12012;
    spawn.unk04 = 0;
    spawn.unk45 = 0xFF;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0;
    spawn.unk12 = 0;
    spawn.unk13 = 0xFF;
    spawn.unk02 = 0x401;

    emitter.unk00 = 30.0f;
    emitter.unk04 = 30.0f;
    emitter.unk08 = 10.0f;
    emitter.unk10 = 0.8000000119f;
    emitter.unk14 = 0.8000000119f;

    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;

    emitter.unk18 = 0.6000000238f;
    emitter.unk1C = 0.6000000238f;
    emitter.unk0C = 16.0f;
    emitter.unk20 = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.target = obj;
    attach.bone = 5;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = 0;
    attach.p0.y = 10.0f;
    attach.p0.x = 0.0f;
    attach.p1.x = 0.0f;
    attach.p0.z = -70.0f;
    attach.p1.y = 20.0f;
    attach.p1.z = -140.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 0xA);
    func_15160A58(obj, 5, &attach, 2, 300, 40, 0xFF, 0, 0, 0xFF, 0, 1, 0, 0, 0xFF, 1);
}

void func_1514D96C(struct127 *obj) {
}

/** Attaches a func_15158BD0 object (type 0x13) with a zeroed 0x20-byte payload. */
void func_1514D978(struct127 *obj) {
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
    s32 handle;

    payload.unk0 = 0;
    payload.unk4 = 0;
    payload.unk8 = 0;
    payload.unkC = 0;
    payload.unk14 = 0;
    payload.unk18 = 0;
    payload.unk1C = 0;
    payload.unk10 = 12.0f;

    handle = func_15158BD0(obj, 1, sizeof(payload));
    if (handle != 0) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        func_1514EC1C(handle, obj, 0x13);
    }
}

void func_1514D9F4(struct127 *obj) {
    s32 handle;

    func_1514D978(obj);
    handle = func_151ACA60(obj, 20.0f, 0);
    func_1514EC1C(handle, obj, 0x14);
}

/** Attaches a func_15158BD0 object (type 0x13) with a zeroed 0x1C-byte payload. */
void func_1514DA38(struct127 *obj) {
    s32 handle;
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
    if (handle != 0) {
        memcpy((u8 *)handle + 0x58, &payload, sizeof(payload));
        func_1514EC1C(handle, obj, 0x13);
    }
}

/** Sets bit 1 of obj->unk94 and attaches a func_15158BD0 object (type 0x13) with an 8-byte payload. */
void func_1514DAA4(struct127 *obj) {
    s32 handle;
    s32 payload[2];

    obj->unk94 |= 2;
    payload[0] = 0;
    payload[1] = 0;
    {
        // matching: an unprototyped call here changes how as1 schedules the argument setup
        extern s32 func_15158BD0();

        if ((handle = func_15158BD0(obj, 1, sizeof(payload))) != 0) {
            memcpy((u8 *)handle + 0x58, payload, sizeof(payload));
            func_1514EC1C(handle, obj, 0x13);
        }
    }
}

/** Attaches a func_15158BD0 object (type 0x13) without payload. */
void func_1514DB18(struct127 *obj) {
    s32 handle = func_15158BD0(obj, 1, 0);

    if (handle != 0) {
        func_1514EC1C(handle, obj, 0x13);
    }
}

void func_1514DB58(struct127 *obj) {
}

void func_1514DB64(struct127 *obj) {
    if (D_800BE9F0 == 20) {
        func_151B2060(obj);
    }
}

void func_1514DB98(struct127 *obj) {
    func_1514F194(obj);
}

void func_1514DBB8(struct127 *obj) {
    func_15160A58(obj, 2, &D_800A58A0, 2, 300, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

void func_1514DC38(struct127 *obj) {
    func_1500EE18(obj, 0xFF, 1);
}

void func_1514DC5C(struct127 *obj) {
    func_151D0F60(obj, 0, 0xFF, 1);
}

void func_1514DC84(struct127 *obj) {
    obj->unk94 |= 2;
}

void func_1514DC98(struct127 *obj) {
    obj->unk94 |= 0x710;
}

/** Sets obj->unk9C to 0x6000 and spawns part sets 0x23 and 0x44 on `obj`. */
void func_1514DCAC(struct127 *obj) {
    obj->unk9C = 0x6000;
    func_15083568(obj, 0x23, 1.0f, 0);
    func_15083568(obj, 0x44, 1.0f, 0);
}

void func_1514DCF4(struct127 *obj) {
    if (obj->unk31C != NULL) {
        obj->unk31C->unk94 = 1;
    }
    func_15083568(obj, 0x17, 1.0f, 0);
}

void func_1514DD2C(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DD4C(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DD6C(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DD8C(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DDAC(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DDCC(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DDEC(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DE0C(struct127 *obj) {
    func_1514DCAC(obj);
}

void func_1514DE2C(struct127 *obj) {
}

void func_1514DE38(struct127 *obj) {
}

void func_1514DE44(struct127 *obj) {
}

void func_1514DE50(struct127 *obj) {
    if (((u8 *)D_800D2E4C)[0x11] & 8) { // low byte of D_800D2E4C->unk10
        func_1514DCAC(obj);
    }
}

void func_1514DE88(struct127 *obj) {
}

void func_1514DE94(struct127 *obj) {
    struct St1514DE94 *part;

    func_15160CDC(obj, 12, &D_800A58AC, &D_800A58B8, 0.008333000354f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 0, 1, 1, 0xFF, 1);
    part = func_15083568(obj, 0x3C, 1.0f, 0);
    if (D_800BE9B4 == 0) {
        part->unk38 = 2400;
    }
    func_1514EC1C(func_15160A58(obj, 0, &D_800A58C4, 2, 300, 11, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1), obj, 0x19);
}

/** Links `obj` to the object whose unique_id is 9 (unk65 holds that object's slot + 1). */
void func_1514DFD0(struct127 *obj) {
    obj->unk65 = func_15083FB0(9) + 1;
    obj->unk101 |= 0x34;
}

void func_1514E00C(struct127 *obj) {
    EmitterParams emitter;
    SpawnParams spawn;
    EmitterAttach attach;

    spawn.unk00 = 3;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk40 = 0x12002;
    spawn.unk04 = 0;
    spawn.unk45 = 0xFF;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0x8E;
    spawn.unk12 = 0x19;
    spawn.unk13 = 0xFF;
    spawn.unk02 = 0x401;
    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;

    emitter.unk00 = 20.0f;
    emitter.unk04 = 20.0f;
    emitter.unk08 = 20.0f;
    emitter.unk0C = 20.0f;
    emitter.unk10 = 0.0f;
    emitter.unk14 = 0.0f;
    emitter.unk18 = 0.5f;
    emitter.unk1C = 0.5f;
    emitter.unk20 = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E194(struct127 *obj) {
    EmitterParams emitter;
    SpawnParams spawn;
    EmitterAttach attach;

    spawn.unk00 = 3;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk40 = 0x12002;
    spawn.unk04 = 0;
    spawn.unk45 = 0xFF;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0x8E;
    spawn.unk12 = 0x19;
    spawn.unk13 = 0xFF;
    spawn.unk02 = 0x401;
    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;

    emitter.unk00 = 16.0f;
    emitter.unk04 = 16.0f;
    emitter.unk08 = 16.0f;
    emitter.unk0C = 16.0f;
    emitter.unk10 = 0.0f;
    emitter.unk14 = 0.0f;
    emitter.unk18 = 0.5f;
    emitter.unk1C = 0.5f;
    emitter.unk20 = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 35.0f;
    attach.p0.y = 27.0f;
    attach.p1.x = 35.0f;
    attach.p1.y = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = -1;

    attach.p0.z = 59.0f;
    attach.p1.z = 559.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E31C(struct127 *obj) {
    EmitterParams emitter;
    SpawnParams spawn;
    EmitterAttach attach;

    spawn.unk00 = 3;
    spawn.unk08 = 0;
    spawn.unk0C = 0;
    spawn.unk40 = 0x2012;
    spawn.unk04 = 0;
    spawn.unk45 = 0xFF;
    spawn.unk10 = 0xFF;
    spawn.unk11 = 0xE6;
    spawn.unk12 = 0;
    spawn.unk13 = 0xFF;
    spawn.unk02 = 0x401;
    spawn.unk1C = 0.0f;
    spawn.unk20 = 0.0f;
    spawn.unk24 = 0.0f;
    spawn.unk28 = 0.0f;
    spawn.unk2C = 0.0f;
    spawn.unk30 = 0.0f;

    emitter.unk00 = 45.0f;
    emitter.unk0C = 45.0f;
    emitter.unk10 = 0.5f;
    emitter.unk14 = 0.5f;
    emitter.unk1C = 0.0f;
    emitter.unk04 = 55.0f;
    emitter.unk08 = 25.0f;
    emitter.unk18 = 0.8999999762f;
    emitter.unk20 = 9.999999975e-07f;

    attach.targetId = obj->unique_id;
    attach.p0.x = 0.0f;
    attach.p0.y = 0.0f;
    attach.p1.x = 0.0f;
    attach.p1.y = 0.0f;
    attach.target = obj;
    attach.bone = 0xC;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = 2;

    attach.p0.z = 100.0f;
    attach.p1.z = 600.0f;
    emitter.unk48 = 1.0f;

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
void func_1514E508(struct127 *obj) {
    s32 handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 0");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->x_position; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E5B8(struct127 *obj) {
    s32 handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect -1");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->x_position; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E668(struct127 *obj) {
    s32 handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 1");
    func_1514EC1C(handle, obj, 0xD);
    pos = (s32)&obj->x_position; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E718(struct127 *obj) {
    s32 handle;
    s32 pos;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect 2");
    func_1514EC1C(handle, obj, 0xE);
    pos = (s32)&obj->x_position; // passed as two 16-bit halves
    handle = func_1515D5F8(-0x8000, pos >> 16, pos & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(handle, obj, 0x18);
}

void func_1514E7C8(struct127 *obj) {
    s32 handle;

    handle = func_1518D1C0(obj, 7, 0, 1, 0xFF, 1, "Charactereffect 3");
    func_1514EC1C(handle, obj, 0xF);
}

/* Attachment destroy handlers (D_8008AB58 entries): release an attachment's handle. */

void func_1514E824(s32 handle) {
}

void func_1514E830(struct102 *handle) {
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

typedef struct ToggleTarget {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 flags;
    /* 0x014 */ u8 pad14[0xFC];
    /* 0x110 */ EmitterParams emitter;    // for objects spawned by func_15189FF0
} ToggleTarget;

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

/** Like func_1514E89C, but clearing bit 0 also zeroes byte 0x30 of target->emitter.unk44. */
s32 func_1514E920(s32 handle, s32 msg, s32 arg) {
    // matching: with an s32 parameter IDO keeps &target->unk110 in its own register
    ToggleTarget *target = (ToggleTarget *)handle;
    EmitterParams *emitter;
    s32 flags;
    u8 *ptr;

    emitter = &target->emitter;
    switch (msg) {
    case 0:
        flags = target->flags;
        if (flags & 1) {
            target->flags = flags & ~1;
            ptr = emitter->unk44;
            if (ptr != NULL) {
                ptr[0x30] = 0;
            }
        } else {
            target->flags = flags | 1;
        }
        break;
    case 2:
        target->flags &= ~1;
        ptr = emitter->unk44;
        if (ptr != NULL) {
            ptr[0x30] = 0;
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

void func_1514E9DC(s32 handle, s32 msg, s32 arg) {
    func_1514E920(handle, msg, arg);
}

void func_1514E9FC(ToggleTarget *target, s32 msg, s32 arg) {
    func_1514E89C(target, msg, arg);
}

typedef struct ModeTarget {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ u8 mode;                 // set through func_1518E37C
} ModeTarget;

void func_1518E37C(ModeTarget *, u8 mode);

/** Messages 3..12 switch the target between mode 2 and modes 3, 5 and 6. */
s32 func_1514EA1C(ModeTarget *target, s32 msg, s32 arg) {
    switch (msg) {
    case 3:
        if (target->mode == 2) {
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
        if (target->mode == 2) {
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
        if (target->mode == 2) {
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

void func_1514EB6C(ToggleTarget *target, s32 msg, s32 arg) {
    func_1514E89C(target, msg, arg);
}

s32 func_1514EB8C(s32 handle, s32 msg, s32 arg) {
    return 1;
}

/** Allocates an unlinked attachment node for `handle` with `extraSize` bytes of payload. */
Attachment *func_1514EBA4(s32 handle, s16 type, s32 extraSize) {
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
Attachment *func_1514EC1C(s32 handle, struct127 *owner, s16 type) {
    Attachment *result;
    Attachment *node;

    result = NULL;
    if (handle != 0) {
        node = func_1514EBA4(handle, type, 0);
        result = node;
        if (node != NULL) {
            node->next = ATTACHMENTS(owner);
            ATTACHMENTS(owner) = node;
            node->prev = NULL;
            if (node->next != NULL) {
                node->next->prev = node;
            }
        } else if (D_8008ABE8[type]->destroyHandler != -1) {
            D_8008AB58[D_8008ABE8[type]->destroyHandler](handle);
        } else {
            func_1516972C((struct102 *)handle);
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
s32 func_1514ED3C(Attachment *list, s32 handle, Attachment **out) {
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
s32 func_1514ED8C(Attachment *node, struct127 *owner) {
    s32 handle;

    if (node == ATTACHMENTS(owner)) {
        ATTACHMENTS(owner) = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    handle = node->handle;
    func_1516972C((struct102 *)node);
    return handle;
}

/** Removes (without releasing the handle) every attachment of `owner` that holds `handle`. */
void func_1514EDF0(s32 handle, struct127 *owner) {
    Attachment *node;
    s32 ret;
    Attachment *match;
    u8 found;

    node = ATTACHMENTS(owner);
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

struct Struct1514EE70 {
    struct127 *obj;
    u8 uniqueId;
    u8 unk5;
    u16 unk6;
};

s32 func_1515BE50(struct Struct1514EE70 *, s32, s32, s32);

/** Attaches a func_1515BE50 object for `obj` (type 0x16). */
void func_1514EE70(struct127 *obj) {
    struct Struct1514EE70 desc;
    s32 handle;

    desc.obj = obj;
    desc.uniqueId = obj->unique_id;
    desc.unk5 = 0;
    desc.unk6 = 300;
    handle = func_1515BE50(&desc, 0, 0xFF, 1);
    func_1514EC1C(handle, obj, 0x16);
}


void func_1514EECC(struct127 *obj) {
    s32 handle;

    handle = func_15160A58(obj, 1, &D_800A5934, 2, 300, 2, 0xFF, 0, 0, 0xFF, 0, 3, 0, 0, 0xFF, 1);
    func_1514EC1C(handle, obj, 0x19);
    {
        SpawnParams spawn;
        EmitterParams emitter;
        EmitterAttach attach;

        spawn.unk00 = 0x69;
        spawn.unk40 = 0x04ED200A;
        spawn.unk04 = 300;
        spawn.unk08 = 0;
        spawn.unk0C = 0;
        spawn.unk45 = 0xFF;
        spawn.unk10 = 0xFF;
        spawn.unk11 = 0;
        spawn.unk12 = 0;
        spawn.unk13 = 0xFF;
        spawn.unk02 = 0x4403;
        spawn.unk1C = 0.0f;
        spawn.unk20 = 0.0f;
        spawn.unk24 = 0.0f;
        spawn.unk28 = 0.0f;
        spawn.unk2C = 0.0f;
        spawn.unk30 = 0.0f;
        spawn.unk01 = 3;
        spawn.unk18 = 100.0f;
        spawn.unk14 = 100.0f;
        spawn.unk34 = 1.0f;
        spawn.unk38 = 1.0f;
        spawn.unk3C = 1.0f;
        spawn.unk44 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        spawn.unk48 = 0;
        spawn.unk4C = 0xFF;
        spawn.unk50 = 0;
        spawn.unk54 = 1;
        spawn.unk56 = 0xFF;

        emitter.unk04 = 95.0f;
        emitter.unk00 = 95.0f;
        emitter.unk0C = 65.0f;
        emitter.unk08 = 65.0f;
        emitter.unk10 = 0.150000006f;
        emitter.unk14 = 0.150000006f;
        emitter.unk18 = 0.6999999881f;
        emitter.unk1C = 0.400000006f;
        emitter.unk20 = 9.999999975e-07f;

        attach.targetId = obj->unique_id;
        attach.target = obj;
        attach.p0 = D_800A5940;
        attach.p1 = D_800A594C;
        attach.bone = 1;
        emitter.unk54 = 0;
        emitter.unk55 = 0;
        emitter.unk56 = 0;
        emitter.unk57 = 0;
        emitter.unk48 = 1.0f;
        attach.handler = 3;

        func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0);
    }
}

void func_1514F110(struct127 *obj) {
    func_1514F194(obj);
}

s32 func_1514F130(HudSpriteHandle *target, s32 msg, s32 arg) {
    switch (msg) {
    case 13:
        target->state->unk9 = 0;
        break;
    case 14:
        target->state->unk9 = 1;
        break;
    default:
        return func_1514E89C((ToggleTarget *)target, msg, arg);
    }
    return 1;
}

void func_1514F194(struct127 *obj) {
    s32 handle;

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
    if (handle != 0) {
        func_1514EC1C(handle, obj, 0x19);
    }
}

void func_1514F308(struct127 *obj) {
    Vec3f pointA = { 52.0f, 0.0f, 80.0f };
    Vec3f pointB = { 52.0f, 0.0f, 353.0f };

    func_15160CDC(obj, 5, &pointA, &pointB, 0.02350199968f, 2, 300,
                  0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
}

void func_1514F3CC(struct127 *obj) {
    struct {
        struct127 *obj;
        u8 uniqueId;
        f32 unk8;
    } payload;
    struct260 *spawned;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.unk8 = 0.0f;

    // the seventh argument is the payload size
    spawned = func_15149130(300, -1, 0x3A, -1, 0, 0x2B, (struct37 *)sizeof(payload), 0xFF, 1);
    if (spawned != NULL) {
        memcpy((u8 *)spawned + 0x28, &payload, sizeof(payload));
    }
}

void func_1514F44C(struct127 *obj) {
    s32 handle;

    handle = func_1518D1C0(obj, 5, 0, 1, 0xFF, 1, "Charactereffect frontend fire imp");
    func_1514EC1C(handle, obj, 0xD);
}

void func_1514F4A8(struct127 *obj) {
    func_151D74B0(obj, 1, -1, 0xFF, 1);
}

void func_1514F4D8(struct127 *obj) {
    func_151D74B0(obj, 2, 1, 0xFF, 1);
}

void func_1514F508(struct127 *obj) {
    func_151D74B0(obj, 3, 0, 0xFF, 1);
}

void func_1514F538(struct127 *obj) {
    func_151D74B0(obj, 4, -1, 0xFF, 1);
}

void func_1514F568(struct127 *obj) {
    func_150C4120(obj, -1, 0xFF, 1);
}

void func_1514F590(struct127 *obj) {
    func_1501175C(obj, 0xFF, 1);
    func_15011A78(obj, 0xFF, 1);
}

void func_1514F5CC(struct127 *obj) {
    Struct1514F5CCArg desc;

    desc.unk00 = 0;
    desc.obj = obj;
    desc.unk04 = 0.0f;
    desc.uniqueId = obj->unique_id;
    desc.unk10 = 0.349999994f;
    desc.unk14 = 20;
    desc.unk16 = 300;
    desc.unk18 = 4;
    func_150C0AC0(&desc, 0xFF, 1);
}
