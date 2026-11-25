#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn header of an effect object, copied into EffectObj.header by func_15147A80. */
typedef struct EffectHeader {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s16 life;
    /* 0x0E */ u16 flags;     /* bit 0: life counts down; 0x10: run unk14's handler; 0x20: pass unk18 to the draw */
    /* 0x10 */ s32 kind;      /* index into D_8008A2F0 / D_8008A340 / D_8008A390 */
    /* 0x14 */ s8 unk14;      /* index into D_8008A284, -1: none */
    /* 0x15 */ u8 itemCount;  /* number of items allocated after the user data */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
} EffectHeader; /* size 0x1C */

typedef struct EffectObjUnk60 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ s32 unk20;
} EffectObjUnk60; /* size 0x24 */

/* An effect object. It is followed in memory by its user data and then its items. */
typedef struct EffectObj {
    /* 0x00 */ u8 pad0[0x10];   /* object header */
    /* 0x10 */ EffectHeader header;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 updateKind;   /* index into D_8008A200, 0: none */
    /* 0x30 */ u8 updateKind2;  /* index into D_8008A23C, 0: none */
    /* 0x31 */ u8 drawKind;     /* index into D_8008A2A4, 0: none */
    /* 0x32 */ u8 pad32[2];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ s32 unk3C[4];    /* buffers from func_1515D480, one per D_80082FA0 + 1 */
    /* 0x4C */ s32 unk4C;       /* buffer from func_1515D440 */
    /* 0x50 */ s32 unk50;
    /* 0x54 */ f32 unk54[3];
    /* 0x60 */ EffectObjUnk60 unk60;
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ u8 *items;
    /* 0x98 */ u8 *userData;
    /* 0x9C */ u8 pad9C[4];
} EffectObj; /* size 0xA0 */

extern f32 D_800A5760;
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern s32 func_1515D480(s32);
extern s32 func_1515D440(void);
extern s32 (*D_8008A200[])(EffectObj *, s8);
extern s32 (*D_8008A23C[])(EffectObj *, s8);
extern s32 (*D_8008A284[])(EffectObj *, s8);

/* Per-frame update: counts down the life and runs the update handlers; any failure destroys the object. */
void func_15147740(EffectObj *obj) {
    s32 kind;
    s8 dead;

    dead = FALSE;
    if (obj->header.flags & 1) {
        obj->header.life -= D_800BE9E4;
        if (obj->header.life < 0) {
            dead = TRUE;
        }
    }

    if (obj->updateKind >= 15) {
        func_1516972C((struct102 *)obj);
        return;
    }
    if (obj->updateKind != 0 && !dead) {
        if (D_8008A200[obj->updateKind](obj, dead) == 0) {
            dead = TRUE;
        }
    }

    if (obj->updateKind2 >= 18) {
        func_1516972C((struct102 *)obj);
        return;
    }
    if (obj->updateKind2 != 0 && !dead) {
        if (D_8008A23C[obj->updateKind2](obj, dead) == 0) {
            dead = TRUE;
        }
    }

    if (obj->header.flags & 0x10) {
        kind = obj->header.unk14;
        if (kind < -1 || kind >= 8) {
            func_1516972C((struct102 *)obj);
            return;
        }
        if (kind != -1 && !dead) {
            if (D_8008A284[kind](obj, dead) == 0) {
                dead = TRUE;
            }
        }
    }

    if (dead) {
        func_1516972C((struct102 *)obj);
    }
}

extern void func_151D5E30(void *);

void func_151478D0(EffectObj *obj) {
    func_151D5E30(obj->unk84);
}

extern void func_1514795C(EffectObj *);

void func_151478F4(EffectObj *obj) {
    func_151478D0(obj);
    func_1514795C(obj);
    func_15169804((struct102 *)obj);
}

void func_15147928(EffectObj *obj) {
    func_151478D0(obj);
    func_1514795C(obj);
    func_15169824((struct102 *)obj);
}

/* Frees the buffers func_15147A80 allocated. */
void func_1514795C(EffectObj *obj) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (obj->unk3C[i] != 0) {
            func_100043B4((s32 *)obj->unk3C[i], 4);
        }
    }

    if (obj->unk4C != 0) {
        func_100043B4((s32 *)obj->unk4C, 4);
    }
}

extern void (*D_8008A2F0[])(EffectObj *);

void func_151479E0(EffectObj *obj) {
    s32 kind = obj->header.kind;

    if (kind < 0) {
        kind = 0;
    } else if (kind >= 20) {
        kind = 0;
    }
    D_8008A2F0[kind](obj);
}

extern void (*D_8008A340[])(EffectObj *);

void func_15147A30(EffectObj *obj) {
    s32 kind = obj->header.kind;

    if (kind < 0) {
        kind = 0;
    } else if (kind >= 20) {
        kind = 0;
    }
    D_8008A340[kind](obj);
}

/* Allocates an effect object with dataSize bytes of user data and header->itemCount items of itemSize bytes. */
EffectObj *func_15147A80(EffectHeader *header, s32 dataSize, s32 itemSize, s32 updateKind, s32 updateKind2,
                         s32 drawKind, s32 arg6, s32 arg7, EffectObjUnk60 *arg8, s32 arg9, s32 argA) {
    EffectObj *obj;
    u8 type;
    s32 itemsSize;
    s32 i;

    itemsSize = header->itemCount * itemSize;
    if (header->flags & 0x40) {
        type = 0x4D;
    } else {
        type = 0x22;
    }

    obj = func_15167A68(type, argA, dataSize + itemsSize + sizeof(EffectObj), 1, (u8)arg9, 1);
    if (obj == NULL) {
        return NULL;
    }

    obj->userData = (u8 *)obj + sizeof(EffectObj);
    obj->items = obj->userData + dataSize;
    memcpy(&obj->header, header, sizeof(EffectHeader));
    obj->unk2C = 0;
    obj->unk2D = 0;
    obj->unk2E = 0;
    obj->updateKind = updateKind;
    obj->updateKind2 = updateKind2;
    obj->drawKind = drawKind;

    if (arg8 != NULL) {
        obj->unk60 = *arg8;
    } else {
        obj->unk60.unk1C = 0;
    }

    obj->unk34 = arg6;
    obj->unk50 = arg7;
    obj->unk38 = 0;

    for (i = 0; i < 4; i++) {
        obj->unk3C[i] = 0;
    }
    obj->unk4C = 0;

    if (arg6 != 0) {
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unk3C[i] = func_1515D480(arg6);
        }
        obj->unk4C = func_1515D440();
    }

    obj->unk54[0] = 0.0f;
    obj->unk54[1] = 0.0f;
    obj->unk54[2] = 0.0f;
    bzero(obj->unk84, sizeof(obj->unk84));
    return obj;
}

extern void *func_151462C8(void *, void *, u8, void *, s32, s16, void *, s32, void *);
extern void *(*D_8008A2A4[])(EffectObj *, void *, s16);

/* Draws the object through func_151462C8 and then its drawKind handler; returns the advanced display list. */
void *func_15147C4C(void *gfx, EffectObj *obj, s16 arg2) {
    s32 unk18;

    if (obj->header.flags & 0x20) {
        unk18 = obj->header.unk18;
    } else {
        unk18 = 0;
    }
    gfx = func_151462C8(gfx, &obj->unk34, 0, NULL, 0, arg2, obj->unk54, 2, (void *)unk18);
    if (obj->drawKind >= 19) {
        func_1516972C((struct102 *)obj);
        return gfx;
    }
    if (obj->drawKind != 0) {
        return D_8008A2A4[obj->drawKind](obj, gfx, arg2);
    }
    return gfx;
}

extern void (*D_8008A390[])(EffectObj *, s32, u8);

void func_15147D1C(EffectObj *obj, s32 arg1, u8 arg2) {
    void (*handler)(EffectObj *, s32, u8) = D_8008A390[obj->header.kind];

    if (handler != NULL) {
        handler(obj, arg1, arg2);
    }
}

void func_15147D64(s32 arg0, u8 arg1) {
    func_15169260(&D_800A5760, 2, arg0, arg1);
}
