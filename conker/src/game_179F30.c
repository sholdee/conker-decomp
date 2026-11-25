#include <ultra64.h>
#define func_15083568 func_15083568_orig
#define func_1514D3B0 func_1514D3B0_orig
#include "functions.h"
#undef func_15083568
#undef func_1514D3B0
#include "variables.h"

struct St1514DE94 {
    char pad0[0x38];
    s32 unk38;
};
struct St1514DE94 *func_15083568(struct127 *, s32, f32, s32);

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

extern void (*D_8008AB58[])(s32);
extern s32 (*D_8008AB64[])(s32, s32, s32);
extern AttachmentType *D_8008ABE8[];

Attachment *func_1514EC1C(s32 handle, struct127 *owner, s16 type);
s32 func_1514D4B8(s16 root, s16 type);

typedef struct Vec3w1514F308 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3w1514F308;

typedef struct Struct1514F5CCArg {
    s32 unk00;
    f32 unk04;
    struct127 *unk08;
    u8 unk0C;
    u8 pad0D[3];
    f32 unk10;
    s16 unk14;
    s16 unk16;
    u8 unk18;
} Struct1514F5CCArg;

typedef struct HudSpriteState {
    char pad_0x00[0x9];
    u8 field_0x09;
} HudSpriteState;

typedef struct HudSpriteHandle {
    char pad_0x00[0x14];
    HudSpriteState *field_0x14;
} HudSpriteHandle;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern Vec3w1514F308 D_800A5970;
extern Vec3w1514F308 D_800A597C;
extern u8 D_800A58A0;
extern u8 D_800A5958;
extern u8 D_800A5964;
extern s32 D_800A58D0;
extern s32 D_800A58E4;
extern s32 D_800A58F8;
extern s32 D_800A590C;
extern s32 D_800A5920;
s32 func_15189FF0(void *, void *, void *, s32, s32, s32, s32);
extern void func_151D74B0(struct127 *, s32, s32, s32, s32);
void func_151BB044(s32);
void func_151BC074(s32);
void func_151BD2F8(void *, s32, s32);
void func_151B9F00(void *, s32, s32);
void func_150C0AC0(Struct1514F5CCArg *, s32, s32);
s32 func_150C5370(void *, s32);
extern struct102 *func_15155FD4(s32);
extern void *func_15155780(s32, s32);
s32 func_1514CA80(struct127 *, s32 *, s32 *, s32 *);
extern void (*D_8008AA70[])(struct127 *);
extern void (*D_8008AACC[])(struct127 *);
extern u8 *D_8008AB90[];
extern u8 D_800A5EC0[];
extern u8 D_800A5ED8[];
s32 func_15160A58(struct127 *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_1518D1C0(s32, s32, s32, s32, s32, s32, s32 *);
s32 func_1515D5F8(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150C4120(s32, s32, s32, s32);

/* Tells a spawned emitter how to follow its owner: two points given in the space of bone `bone`
 * of `target`, which func_1518A094 transforms every frame. */
typedef struct EmitterAttach {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 x2;
    /* 0x10 */ f32 y2;
    /* 0x14 */ f32 z2;
    /* 0x18 */ struct127 *target;
    /* 0x1C */ u8 bone;
    /* 0x1D */ u8 targetId;             // the target's unique_id
    /* 0x1E */ s8 handler;              // index into D_8008D5A0, or -1
    /* 0x1F */ u8 pad1F;
} EmitterAttach;

typedef struct Struct1514E00C_4C {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 pad14[8];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 pad46[0x12];
} Struct1514E00C_4C;

typedef struct Struct1514E00C_A4 {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x24];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 pad58[4];
} Struct1514E00C_A4;


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

typedef struct Struct1514D564Desc {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s8 unk18;
    u8 pad19[3];
    f32 unk1C;
    s8 unk20;
    u8 pad21[3];
} Struct1514D564Desc;

void func_1514D564(struct127 *obj) {
    Struct1514D564Desc desc;
    s32 spawned;
    Struct1514D564Payload payload;

    payload.obj = obj;
    payload.uniqueId = obj->unique_id;
    payload.unk8 = 0.0f;
    payload.unkC = -180.0f;
    payload.unk10 = 77.0f;
    desc.unk0 = -477.0f;
    desc.unk4 = -2015.0f;
    desc.unk8 = 2244.0f;
    payload.unk5 = 6;
    desc.unk18 = 1;
    desc.unkC = obj->x_position;
    desc.unk10 = obj->y_position;
    desc.unk14 = obj->z_position;
    desc.unk20 = 1;
    desc.unk1C = 0.3199999928f;
    spawned = func_150C5370(&desc, sizeof(payload));
    if (spawned != 0) {
        memcpy((void *)(spawned + 0xC8), &payload, sizeof(payload));
        func_1514EC1C(spawned, obj, 0x15);
    }
}

void func_1514D64C(struct127 *obj) {
    Struct1514E00C_A4 emitter;
    Struct1514E00C_4C spawn;
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
    attach.x = 0.0f;
    attach.y = 0.0f;
    attach.x2 = 0.0f;
    attach.y2 = 0.0f;
    attach.bone = 0x2B;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = 1;

    attach.z = 25.0f;
    attach.z2 = 75.0f;
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
    attach.y = 10.0f;
    attach.x = 0.0f;
    attach.x2 = 0.0f;
    attach.z = -70.0f;
    attach.y2 = 20.0f;
    attach.z2 = -140.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 0xA);
    func_15160A58(obj, 5, &attach, 2, 0x12C, 0x28, 0xFF, 0, 0, 0xFF, 0, 1, 0, 0, 0xFF, 1);
}

void func_1514D96C(s32 arg0) {
}

s32 func_15158BD0(s32, s32, s32);

void func_1514D978(s32 arg0) {
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        f32 unk10;
        s32 unk14;
        s32 unk18;
        s32 unk1C;
    } sp20;
    s32 v0;

    sp20.unk0 = 0;
    sp20.unk4 = 0;
    sp20.unk8 = 0;
    sp20.unkC = 0;
    sp20.unk14 = 0;
    sp20.unk18 = 0;
    sp20.unk1C = 0;
    sp20.unk10 = 12.0f;

    v0 = func_15158BD0(arg0, 1, 0x20);
    if (v0 != 0) {
        memcpy((void *)(v0 + 0x58), &sp20, 0x20);
        func_1514EC1C(v0, arg0, 0x13);
    }
}

s32 func_151ACA60(s32, f32, s32);

void func_1514D9F4(s32 arg0) {
    s32 v0;
    func_1514D978(arg0);
    v0 = func_151ACA60(arg0, 20.0f, 0);
    func_1514EC1C(v0, arg0, 0x14);
}

void func_1514DA38(s32 arg0) {
    s32 temp_v0;
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
        s32 unk10;
        s32 unk14;
        s32 unk18;
    } sp18;

    sp18.unk14 = 0;
    sp18.unk18 = 0;
    sp18.unk0 = 0;
    sp18.unk4 = 0;
    sp18.unk8 = 0;
    sp18.unkC = 0;
    temp_v0 = func_15158BD0(arg0, 1, 0x1C);
    if (temp_v0 != 0) {
        memcpy((void *)(temp_v0 + 0x58), &sp18, 0x1C);
        func_1514EC1C(temp_v0, arg0, 0x13);
    }
}

void func_1514DAA4(struct127 *arg0) {
    s32 v0;
    s32 sp1C[2];

    arg0->unk94 |= 0x2;
    sp1C[0] = 0;
    sp1C[1] = 0;
    {
        extern s32 func_15158BD0();

        if ((v0 = func_15158BD0((s32) arg0, 1, 8)) != 0) {
            memcpy((void *)(v0 + 0x58), sp1C, 8);
            func_1514EC1C(v0, (s32) arg0, 0x13);
        }
    }
}

s32 func_15158BD0(s32, s32, s32);

void func_1514DB18(s32 arg0) {
    s32 v0 = func_15158BD0(arg0, 1, 0);
    if (v0 != 0) {
        func_1514EC1C(v0, arg0, 0x13);
    }
}

void func_1514DB58(s32 arg0) {
}

extern s32 D_800BE9F0;
void func_151B2060(void);

void func_1514DB64(void) {
    if (D_800BE9F0 == 0x14) {
        func_151B2060();
    }
}

void func_1514F194();

void func_1514DB98(void) {
    func_1514F194();
}

void func_1514DBB8(s32 arg0) {
    func_15160A58(arg0, 2, &D_800A58A0, 2, 0x12C, 0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, 0xFF, 1);
}

void func_1514DC38(struct127 *arg0) {
    func_1500EE18(arg0, 0xFF, 1);
}

void func_151D0F60(void *, s32, s32, s32);

void func_1514DC5C(void *arg0) {
    func_151D0F60(arg0, 0, 0xFF, 1);
}

typedef struct {
    u8 pad0[0x94];
    /* 0x94 */ s32 unk94;
} S1514DC84;

void func_1514DC84(S1514DC84 *arg0) {
    arg0->unk94 |= 0x2;
}

typedef struct {
    u8 pad0[0x94];
    /* 0x94 */ s32 unk94;
} S1514DC98;

void func_1514DC98(S1514DC98 *arg0) {
    arg0->unk94 |= 0x710;
}

void func_1514DCAC(arg0)
struct127 *arg0;
{
    *(s32 *)((u8 *)arg0 + 0x9C) = 0x6000;
    func_15083568(arg0, 0x23, 1.0f, 0);
    func_15083568(arg0, 0x44, 1.0f, 0);
}

struct struct1514DCF4 {
    u8 pad[0x31C];
    struct127 *unk31C;
};

void func_1514DCF4(struct struct1514DCF4 *arg0) {
    if (arg0->unk31C != NULL) {
        *(u8 *)((u8 *)arg0->unk31C + 0x94) = 1;
    }
    func_15083568((struct127 *)arg0, 0x17, 1.0f, 0);
}

void func_1514DD2C(void) {
    func_1514DCAC();
}

void func_1514DD4C(void) {
    func_1514DCAC();
}

void func_1514DD6C(void) {
    func_1514DCAC();
}

void func_1514DD8C(void) {
    func_1514DCAC();
}

void func_1514DDAC(void) {
    func_1514DCAC();
}

void func_1514DDCC(void) {
    func_1514DCAC();
}

void func_1514DDEC(void) {
    func_1514DCAC();
}

void func_1514DE0C(void) {
    func_1514DCAC();
}

void func_1514DE2C(s32 arg0) {
}

void func_1514DE38(s32 arg0) {
}

void func_1514DE44(s32 arg0) {
}

void func_1514DE50(void) {
    if (((u8 *)D_800D2E4C)[0x11] & 8) {
        func_1514DCAC();
    }
}

void func_1514DE88(s32 arg0) {
}

void func_15160CDC(s32, s32, void *, void *, f32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_15160A58(struct127 *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern u8 D_800A58AC;
extern u8 D_800A58B8;
extern u8 D_800A58C4;
extern u8 D_800BE9B4;

void func_1514DE94(s32 arg0) {
    struct St1514DE94 *temp;

    func_15160CDC(arg0, 0xC, &D_800A58AC, &D_800A58B8, 0.008333000354f, 2, 0x12C,
                  0xFF, 0xFF, 0xFF, 0xFF, 0, 1, 1, 0xFF, 1);
    temp = func_15083568((struct127 *)arg0, 0x3C, 1.0f, 0);
    if (D_800BE9B4 == 0) {
        temp->unk38 = 0x960;
    }
    func_1514EC1C(func_15160A58(arg0, 0, &D_800A58C4, 2, 0x12C, 0xB, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1), arg0, 0x19);
}

s32 func_15083FB0(s32);

void func_1514DFD0(u8 *arg0) {
    arg0[0x65] = func_15083FB0(9) + 1;
    arg0[0x101] |= 0x34;
}

void func_1514E00C(struct127 *obj) {
    Struct1514E00C_A4 emitter;
    Struct1514E00C_4C spawn;
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
    attach.x = 35.0f;
    attach.y = 27.0f;
    attach.x2 = 35.0f;
    attach.y2 = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = -1;

    attach.z = 59.0f;
    attach.z2 = 559.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E194(struct127 *obj) {
    Struct1514E00C_A4 emitter;
    Struct1514E00C_4C spawn;
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
    attach.x = 35.0f;
    attach.y = 27.0f;
    attach.x2 = 35.0f;
    attach.y2 = 27.0f;
    attach.target = obj;
    attach.bone = 6;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = -1;

    attach.z = 59.0f;
    attach.z2 = 559.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 0, 0, 0), obj, 6);
    func_151D74B0(obj, 5, -1, 0xFF, 1);
}

void func_1514E31C(struct127 *obj) {
    Struct1514E00C_A4 emitter;
    Struct1514E00C_4C spawn;
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
    attach.x = 0.0f;
    attach.y = 0.0f;
    attach.x2 = 0.0f;
    attach.y2 = 0.0f;
    attach.target = obj;
    attach.bone = 0xC;
    emitter.unk54 = 0;
    emitter.unk55 = 0;
    emitter.unk56 = 0;
    emitter.unk57 = 0;
    attach.handler = 2;

    attach.z = 100.0f;
    attach.z2 = 600.0f;
    emitter.unk48 = 1.0f;

    func_1514EC1C(func_15189FF0(&spawn, &emitter, &attach, 2, 3, 0, 0), obj, 7);

    {
        struct {
            f32 unk0;
            f32 unk4;
            f32 unk8;
        } sp50;

        sp50.unk0 = 0.0f;
        sp50.unk4 = 0.0f;
        sp50.unk8 = 150.0f;
        func_15160A58(obj, 0xC, &sp50, 2, 0x12C, 0x28, 0xFF, 0xB5, 0, 0xFF, 0, 2, 0, 0, 0xFF, 1);
    }
}

void func_1514E508(s32 arg0) {
    s32 v0;
    s32 temp;

    v0 = func_1518D1C0(arg0, 5, 0, 1, 0xFF, 1, &D_800A58D0);
    func_1514EC1C(v0, arg0, 0xD);
    temp = arg0 + 0x14;
    v0 = func_1515D5F8(-0x8000, temp >> 16, temp & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(v0, arg0, 0x18);
}

void func_1514E5B8(s32 arg0) {
    s32 v0;
    s32 temp;

    v0 = func_1518D1C0(arg0, 5, 0, 1, 0xFF, 1, &D_800A58E4);
    func_1514EC1C(v0, arg0, 0xD);
    temp = arg0 + 0x14;
    v0 = func_1515D5F8(-0x8000, temp >> 16, temp & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(v0, arg0, 0x18);
}

void func_1514E668(s32 arg0) {
    s32 v0;
    s32 temp;

    v0 = func_1518D1C0(arg0, 5, 0, 1, 0xFF, 1, &D_800A58F8);
    func_1514EC1C(v0, arg0, 0xD);
    temp = arg0 + 0x14;
    v0 = func_1515D5F8(-0x8000, temp >> 16, temp & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(v0, arg0, 0x18);
}

void func_1514E718(s32 arg0) {
    s32 v0;
    s32 temp;

    v0 = func_1518D1C0(arg0, 5, 0, 1, 0xFF, 1, &D_800A590C);
    func_1514EC1C(v0, arg0, 0xE);
    temp = arg0 + 0x14;
    v0 = func_1515D5F8(-0x8000, temp >> 16, temp & 0xFFFF, 5, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0);
    func_1514EC1C(v0, arg0, 0x18);
}

void func_1514E7C8(s32 arg0) {
    s32 v0;

    v0 = func_1518D1C0(arg0, 7, 0, 1, 0xFF, 1, &D_800A5920);
    func_1514EC1C(v0, arg0, 0xF);
}

void func_1514E824(s32 arg0) {
}

void func_1514E830(struct102 *arg0) {
    func_1516972C(arg0);
}

void func_1518E308(void *);

void func_1514E850(void *arg0) {
    func_1518E308(arg0);
    func_1516972C(arg0);
}

void func_1514E87C(void) {
    func_1515F10C();
}

s32 func_1514E89C(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
s32 arg2;
{
    s32 flags;

    switch (arg1) {
    case 0:
        flags = *(s32 *) (arg0 + 0x10);
        if (flags & 1) {
            *(s32 *) (arg0 + 0x10) = flags & ~1;
        } else {
            *(s32 *) (arg0 + 0x10) = flags | 1;
        }
        break;
    case 2:
        *(s32 *) (arg0 + 0x10) = *(s32 *) (arg0 + 0x10) & ~1;
        break;
    case 1:
        *(s32 *) (arg0 + 0x10) = *(s32 *) (arg0 + 0x10) | 1;
        break;
    default:
        return 0;
    }
    return 1;
}

s32 func_1514E89C();

s32 func_1514E920(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
s32 arg2;
{
    u8 *sub;
    s32 flags;
    u8 *ptr;

    sub = (u8 *) arg0 + 0x110;
    switch (arg1) {
    case 0:
        flags = *(s32 *) (arg0 + 0x10);
        if (flags & 1) {
            *(s32 *) (arg0 + 0x10) = flags & ~1;
            ptr = *(u8 **) (sub + 0x44);
            if (ptr != 0) {
                ptr[0x30] = 0;
            }
        } else {
            *(s32 *) (arg0 + 0x10) = flags | 1;
        }
        break;
    case 2:
        *(s32 *) (arg0 + 0x10) = *(s32 *) (arg0 + 0x10) & ~1;
        ptr = *(u8 **) (sub + 0x44);
        if (ptr != 0) {
            ptr[0x30] = 0;
        }
        break;
    case 1:
        *(s32 *) (arg0 + 0x10) = *(s32 *) (arg0 + 0x10) | 1;
        break;
    default:
        return func_1514E89C(arg0, arg1, arg2);
    }
    return 1;
}

s32 func_1514E920();

void func_1514E9DC(void) {
    func_1514E920();
}

s32 func_1514E89C();

void func_1514E9FC(void) {
    func_1514E89C();
}

void func_1518E37C(void *, u8);

s32 func_1514EA1C(void *arg0, s32 arg1, s32 arg2) {
    switch (arg1) {
        case 3:
            if (*(u8 *)((u8 *)arg0 + 0x22) == 2) {
                func_1518E37C(arg0, 3);
            } else {
                func_1518E37C(arg0, 2);
            }
            break;
        case 4:
            func_1518E37C(arg0, 3);
            break;
        case 5:
            func_1518E37C(arg0, 2);
            break;
        case 6:
            if (*(u8 *)((u8 *)arg0 + 0x22) == 2) {
                func_1518E37C(arg0, 5);
            } else {
                func_1518E37C(arg0, 2);
            }
            break;
        case 7:
            func_1518E37C(arg0, 5);
            break;
        case 8:
            func_1518E37C(arg0, 2);
            break;
        case 9:
            if (*(u8 *)((u8 *)arg0 + 0x22) == 2) {
                func_1518E37C(arg0, 6);
            } else {
                func_1518E37C(arg0, 2);
            }
            break;
        case 10:
            func_1518E37C(arg0, 6);
            break;
        case 11:
            func_1518E37C(arg0, 2);
            break;
        case 12:
            func_1518E37C(arg0, 2);
            break;
        default:
            return func_1514E89C(arg0, arg1, arg2);
    }
    return 1;
}

void func_1514EB6C(void) {
    func_1514E89C();
}

s32 func_1514EB8C(s32 arg0, s32 arg1, s32 arg2) {
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
        ret = func_1514ED3C(node, handle, &match);
        found = ret;
        if (ret) {
            node = match->next;
            func_1514ED8C(match, owner);
        }
    } while (found);
}

struct Struct1514EE70 {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
};

s32 func_1515BE50(struct Struct1514EE70 *, s32, s32, s32);

void func_1514EE70(u8 *arg0) {
    struct Struct1514EE70 sp20;
    s32 v0;

    sp20.unk0 = (s32) arg0;
    sp20.unk4 = arg0[0x3B];
    sp20.unk5 = 0;
    sp20.unk6 = 0x12C;
    v0 = func_1515BE50(&sp20, 0, 0xFF, 1);
    func_1514EC1C(v0, (s32) arg0, 0x16);
}

typedef struct Struct1514EECC_C4 {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
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
} Struct1514EECC_C4;

typedef struct Struct1514EECC_48 {
    /* 0x00 */ Vec3w1514F308 unk00;
    /* 0x0C */ Vec3w1514F308 unk0C;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ u8 pad1F;
} Struct1514EECC_48;

extern u8 D_800A5934;
extern Vec3w1514F308 D_800A5940;
extern Vec3w1514F308 D_800A594C;

void func_1514EECC(s32 arg0) {
    s32 v0;
    v0 = func_15160A58(arg0, 1, &D_800A5934, 2, 0x12C, 2, 0xFF, 0, 0, 0xFF, 0, 3, 0, 0, 0xFF, 1);
    func_1514EC1C(v0, arg0, 0x19);
    {
    Struct1514EECC_C4 spC4;
    Struct1514E00C_A4 sp68;
    Struct1514EECC_48 sp48;


    spC4.unk00 = 0x69;
    spC4.unk40 = 0x04ED200A;
    spC4.unk04 = 0x12C;
    spC4.unk08 = 0;
    spC4.unk0C = 0;
    spC4.unk45 = 0xFF;
    spC4.unk10 = 0xFF;
    spC4.unk11 = 0;
    spC4.unk12 = 0;
    spC4.unk13 = 0xFF;
    spC4.unk02 = 0x4403;
    spC4.unk1C = 0.0f;
    spC4.unk20 = 0.0f;
    spC4.unk24 = 0.0f;
    spC4.unk28 = 0.0f;
    spC4.unk2C = 0.0f;
    spC4.unk30 = 0.0f;
    spC4.unk01 = 3;
    spC4.unk18 = 100.0f;
    spC4.unk14 = 100.0f;
    spC4.unk34 = 1.0f;
    spC4.unk38 = 1.0f;
    spC4.unk3C = 1.0f;
    spC4.unk44 = 0xFF;
    spC4.unk46 = 0;
    spC4.unk47 = 6;
    spC4.unk48 = 0;
    spC4.unk4C = 0xFF;
    spC4.unk50 = 0;
    spC4.unk54 = 1;
    spC4.unk56 = 0xFF;

    sp68.unk04 = 95.0f;
    sp68.unk00 = 95.0f;
    sp68.unk0C = 65.0f;
    sp68.unk08 = 65.0f;
    sp68.unk10 = 0.150000006f;
    sp68.unk14 = 0.150000006f;
    sp68.unk18 = 0.6999999881f;
    sp68.unk1C = 0.400000006f;
    sp68.unk20 = 9.999999975e-07f;

    sp48.unk1D = ((u8 *)arg0)[0x3B];
    sp48.unk18 = arg0;
    sp48.unk00 = D_800A5940;
    sp48.unk0C = D_800A594C;
    sp48.unk1C = 1;
    sp68.unk54 = 0;
    sp68.unk55 = 0;
    sp68.unk56 = 0;
    sp68.unk57 = 0;
    sp68.unk48 = 1.0f;
    sp48.unk1E = 3;

    func_15189FF0(&spC4, &sp68, &sp48, 2, 0, 0, 0);
    }
}

void func_1514F110(void) {
    func_1514F194();
}

s32 func_1514F130(HudSpriteHandle *arg0, s32 arg1, s32 arg2) {
    switch (arg1) {
    case 13:
        arg0->field_0x14->field_0x09 = 0;
        break;
    case 14:
        arg0->field_0x14->field_0x09 = 1;
        break;
    default:
        return func_1514E89C((s32) arg0, arg1, arg2);
    }
    return 1;
}

void func_1514F194(s32 arg0) {
    s32 v0;

    func_151D0F60((void *)arg0, 1, 0xFF, 1);
    func_151D0F60((void *)arg0, 2, 0xFF, 1);
    func_151BB044(arg0);
    if (D_800BE616 == 0) {
        func_151BC074(arg0);
    }
    func_151BD2F8((void *)arg0, 0xFF, 1);
    func_151B9F00((void *)arg0, 0xFF, 1);
    func_15160A58(arg0, 1, &D_800A5958, 2, 0x12C, 0xC, 0xFF, 0xFF, 0xFF, 2, 0, -1, 0, 0, 0xFF, 1);
    v0 = func_15160A58(arg0, 1, &D_800A5964, 2, 0x12C, 0xF, 0xFF, 0, 0, 0xFF, 1, -1, 0, 0, 0xFF, 1);
    if (v0 != 0) {
        func_1514EC1C(v0, arg0, 0x19);
    }
}

void func_1514F308(s32 arg0) {
    Vec3w1514F308 sp54;
    Vec3w1514F308 sp48;

    sp54 = D_800A5970;
    sp48 = D_800A597C;
    func_15160CDC(arg0, 5, &sp54, &sp48, 0.02350199968f, 2, 0x12C,
                  0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
}

void func_1514F3CC(struct127 *arg0) {
    struct {
        struct127 *unk0;
        u8 unk4;
        f32 unk8;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = arg0->unique_id;
    sp34.unk8 = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x3A, -1, 0, 0x2B, (struct37 *)0xC, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

extern s32 D_800A5988;
s32 func_1518D1C0(s32, s32, s32, s32, s32, s32, s32 *);

void func_1514F44C(s32 arg0) {
    s32 v0;

    v0 = func_1518D1C0(arg0, 5, 0, 1, 0xFF, 1, &D_800A5988);
    func_1514EC1C(v0, arg0, 0xD);
}

extern void func_151D74B0(struct127 *, s32, s32, s32, s32);

void func_1514F4A8(s32 arg0) {
    func_151D74B0(arg0, 1, -1, 0xFF, 1);
}

void func_1514F4D8(s32 arg0) {
    func_151D74B0(arg0, 2, 1, 0xFF, 1);
}

void func_1514F508(s32 arg0) {
    func_151D74B0(arg0, 3, 0, 0xFF, 1);
}

void func_1514F538(s32 arg0) {
    func_151D74B0(arg0, 4, -1, 0xFF, 1);
}

void func_1514F568(s32 arg0) {
    func_150C4120(arg0, -1, 0xFF, 1);
}

void func_1501175C(struct127 *arg0, u8 arg1, s32 arg2);

void func_1514F590(struct127 *arg0) {
    func_1501175C(arg0, 0xFF, 1);
    func_15011A78(arg0, 0xFF, 1);
}

void func_1514F5CC(struct127 *arg0) {
    Struct1514F5CCArg sp1C;

    sp1C.unk00 = 0;
    sp1C.unk08 = arg0;
    sp1C.unk04 = 0.0f;
    sp1C.unk0C = arg0->unique_id;
    sp1C.unk10 = 0.349999994f;
    sp1C.unk14 = 0x14;
    sp1C.unk16 = 0x12C;
    sp1C.unk18 = 4;
    func_150C0AC0(&sp1C, 0xFF, 1);
}
