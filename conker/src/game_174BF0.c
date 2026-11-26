#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 (*D_8008A200[])(EffectObj *, s8);
extern s32 (*D_8008A23C[])(EffectObj *, s8);
extern s32 (*D_8008A284[])(EffectObj *, s8);

/* Per-frame update: counts down the life and runs the update handlers; any failure destroys the object. */
void func_15147740(EffectObj *obj) {
    s32 kind;
    s8 dead;

    dead = FALSE;
    if (obj->head.tmpl.flags & 1) {
        obj->head.tmpl.life -= D_800BE9E4;
        if (obj->head.tmpl.life < 0) {
            dead = TRUE;
        }
    }

    if (obj->head.updateKind >= 15) {
        func_1516972C(&obj->head.header);
        return;
    }
    if (obj->head.updateKind != 0 && !dead) {
        if (D_8008A200[obj->head.updateKind](obj, dead) == 0) {
            dead = TRUE;
        }
    }

    if (obj->head.updateKind2 >= 18) {
        func_1516972C(&obj->head.header);
        return;
    }
    if (obj->head.updateKind2 != 0 && !dead) {
        if (D_8008A23C[obj->head.updateKind2](obj, dead) == 0) {
            dead = TRUE;
        }
    }

    if (obj->head.tmpl.flags & 0x10) {
        kind = obj->head.tmpl.unk14;
        if (kind < -1 || kind >= 8) {
            func_1516972C(&obj->head.header);
            return;
        }
        if (kind != -1 && !dead) {
            if (D_8008A284[kind](obj, dead) == 0) {
                dead = TRUE;
            }
        }
    }

    if (dead) {
        func_1516972C(&obj->head.header);
    }
}

void func_151478D0(EffectObj *obj) {
    func_151D5E30(obj->buffers);
}

/* Frees the object's buffers, then hands it to func_15169804. obj is any view of a func_15147A80 object. */
void func_151478F4(void *arg) {
    EffectObj *obj = arg;

    func_151478D0(obj);
    func_1514795C(obj);
    func_15169804(&obj->head.header);
}

/* Frees the object's buffers, then hands it to func_15169824. obj is any view of a func_15147A80 object. */
void func_15147928(void *arg) {
    EffectObj *obj = arg;

    func_151478D0(obj);
    func_1514795C(obj);
    func_15169824(&obj->head.header);
}

/* Frees the buffers func_15147A80 allocated. */
void func_1514795C(EffectObj *obj) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (obj->head.model.handles[i] != 0) {
            func_100043B4((s32 *)obj->head.model.handles[i], 4);
        }
    }

    if (obj->head.model.sharedHandle != 0) {
        func_100043B4((s32 *)obj->head.model.sharedHandle, 4);
    }
}

extern void (*D_8008A2F0[])(EffectObj *);

/* Runs the D_8008A2F0 handler for header.kind (out-of-range kinds use handler 0). */
void func_151479E0(EffectObj *obj) {
    s32 kind = obj->head.tmpl.kind;

    if (kind < 0) {
        kind = 0;
    } else if (kind >= 20) {
        kind = 0;
    }
    D_8008A2F0[kind](obj);
}

extern void (*D_8008A340[])(EffectObj *);

/* Runs the D_8008A340 handler for header.kind (out-of-range kinds use handler 0). */
void func_15147A30(EffectObj *obj) {
    s32 kind = obj->head.tmpl.kind;

    if (kind < 0) {
        kind = 0;
    } else if (kind >= 20) {
        kind = 0;
    }
    D_8008A340[kind](obj);
}

/* Allocates an effect object with dataSize bytes of user data and header->itemCount items of itemSize bytes.
 * Returns it as void *: each caller keeps it in its own view of the object. */
void *func_15147A80(EffectTemplate *header, s32 dataSize, s32 itemSize, s32 updateKind, s32 updateKind2,
                    s32 drawKind, s32 arg6, s32 arg7, EffectObjUnk60 *arg8, u8 arg9, s32 argA) {
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

    obj = func_15167A68(type, argA, dataSize + itemsSize + sizeof(EffectObj), 1, arg9, 1);
    if (obj == NULL) {
        return NULL;
    }

    obj->userData = (u8 *)obj + sizeof(EffectObj);
    obj->items = (u8 *)obj->userData + dataSize;
    memcpy(&obj->head.tmpl, header, sizeof(EffectTemplate));
    obj->head.count = 0;
    obj->head.start = 0;
    obj->head.end = 0;
    obj->head.updateKind = updateKind;
    obj->head.updateKind2 = updateKind2;
    obj->head.drawKind = drawKind;

    if (arg8 != NULL) {
        obj->unk60 = *arg8;
    } else {
        obj->unk60.unk1C = 0;
    }

    obj->head.model.model = arg6;
    obj->head.model.unk1C = arg7;
    obj->head.model.unk4 = 0;

    for (i = 0; i < 4; i++) {
        obj->head.model.handles[i] = 0;
    }
    obj->head.model.sharedHandle = 0;

    if (arg6 != 0) {
        for (i = 0; i <= D_80082FA0; i++) {
            obj->head.model.handles[i] = func_1515D480(arg6);
        }
        obj->head.model.sharedHandle = func_1515D440();
    }

    obj->head.modelPos.x = 0.0f;
    obj->head.modelPos.y = 0.0f;
    obj->head.modelPos.z = 0.0f;
    bzero(obj->buffers, sizeof(obj->buffers));
    return obj;
}

extern void *(*D_8008A2A4[])(EffectObj *, void *, s16);

/* Draws the object through func_151462C8 and then its drawKind handler; returns the advanced display list. */
void *func_15147C4C(void *gfx, EffectObj *obj, s16 arg2) {
    void *unk18;

    if (obj->head.tmpl.flags & 0x20) {
        unk18 = obj->head.tmpl.unk18;
    } else {
        unk18 = NULL;
    }
    gfx = func_151462C8(gfx, &obj->head.model, 0, NULL, 0, arg2, &obj->head.modelPos, 2, unk18);
    if (obj->head.drawKind >= 19) {
        func_1516972C(&obj->head.header);
        return gfx;
    }
    if (obj->head.drawKind != 0) {
        return D_8008A2A4[obj->head.drawKind](obj, gfx, arg2);
    }
    return gfx;
}

extern void (*D_8008A390[])(EffectObj *, s32, u8);

/* Runs the optional D_8008A390 handler for header.kind. */
void func_15147D1C(EffectObj *obj, s32 arg1, u8 arg2) {
    void (*handler)(EffectObj *, s32, u8) = D_8008A390[obj->head.tmpl.kind];

    if (handler != NULL) {
        handler(obj, arg1, arg2);
    }
}

void func_15147D64(s32 arg0, u8 arg1) {
    func_15169260(D_800A5760, 2, arg0, arg1);
}
