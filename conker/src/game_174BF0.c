#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata: the two object types of the effect objects (func_15147A80), read by game_1FA770.c too. */
const s32 D_800A5760[2] = { 0x4D, 0x22 };

/*
 * This file's .data. The render setup and reset display lists of the effect objects; their entry in the object
 * type table D_8008B4D4 points to them.
 */
Gfx D_8008A1D0[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx D_8008A1F0[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/*
 * The handler tables, by EffectObj kind. Each kind keeps its own view of the object, so the tables are declared
 * without a prototype.
 */
/* Update handlers (head.updateKind, then head.updateKind2), (obj, dead): 0 destroys the object. */
static s32 (*D_8008A200[15])() = {
    NULL,          trail_updateMotionTrail, flamethrower_updateJet, trail_ageMatrixTrail, func_150DC558, trail_ageSurfaceTrail, trail_ageBoneTrail,
    trail_ageScatterTrail, trail_moveFallingTrail, trail_moveStreamParticles, func_151BC64C, trail_ageFollowTrail, flamethrower_updateFlame, trail_fallAnchoredTrail,
    trail_movePissPoints,
};
static s32 (*D_8008A23C[18])() = {
    NULL,          trail_emitPissPoints, flamethrower_emitJet, trail_followMatrixTrail, func_150DCEA0, trail_followSurfaceTrail, trail_followBoneTrail,
    trail_emitScatterTrail, trail_fadeFallingTrail, trail_spawnStreamParticles, func_151BC794, trail_followTarget, flamethrower_emitFlame, trail_updateCharacterTrail,
    trail_moveRibbon, trail_followWaveTrail, trail_followAnchoredTrail, trail_sampleTrackerTrail,
};
/* Update handlers by head.tmpl.unk14 (flag 0x10). */
static s32 (*D_8008A284[8])() = {
    flamethrower_shapeFlame, trail_trimCharacterTrail, trail_shapeRibbon, trail_shapeWaveTrail, trail_shapeAnchoredTrail, trail_tracePissTrail, trail_shapeTrackerTrail,
    trail_fadeStreamEnds,
};
/* Draw handlers (head.drawKind), (obj, gfx, view): return the advanced display list. */
static Gfx *(*D_8008A2A4[19])() = {
    NULL,          trail_drawMotionTrail, flamethrower_drawJet, trail_drawMatrixTrail, func_150DD640, trail_drawSurfaceTrail, trail_drawBoneTrail,
    trail_drawScatterTrail, trail_drawFallingTrail, trail_drawStream, func_151BCA90, trail_drawFollowTrail, flamethrower_drawFlame, trail_drawCharacterTrail,
    trail_drawRibbon, trail_drawWaveRibbon, trail_drawAnchoredTrail, trail_drawPissTrail, trail_drawTrackerTrail,
};
/* Destroy handlers (head.tmpl.kind). */
static void (*D_8008A2F0[20])() = {
    func_151478F4, trail_destroyEffectObj, trail_destroyPissTrail, flamethrower_destroyJet, trail_destroyMatrixTrail, func_151478F4, trail_destroySurfaceTrail,
    trail_destroySurfaceTrail, func_151478F4, trail_destroyScatterTrail, func_151478F4, trail_destroyStreamEmitter, func_151478F4, func_151478F4,
    flamethrower_destroyFlame, trail_destroyCharacterTrail, func_151B83A0, func_151478F4, trail_destroyAnchoredTrail, trail_destroyTrackerTrail,
};
/* Free handlers (head.tmpl.kind). */
static void (*D_8008A340[20])() = {
    func_15147928, trail_destroyEffectObj, trail_freePissTrail, flamethrower_freeJet, trail_freeMatrixTrail, func_15147928, trail_freeSurfaceTrail,
    trail_freeSurfaceTrail, func_15147928, trail_freeScatterTrail, func_15147928, trail_freeStreamEmitter, func_15147928, func_15147928,
    flamethrower_freeFlame, trail_freeCharacterTrail, func_151B83CC, func_15147928, trail_freeAnchoredTrail, trail_freeTrackerTrail,
};
/* Message handlers (head.tmpl.kind); optional. func_15147D1C calls them through this prototype (the ROM
 * passes msgType as a u8). */
static void (*D_8008A390[20])(EffectObj *obj, s32 msg, u8 msgType) = {
    NULL,          NULL,          trail_onPissTrailMessage, flamethrower_onJetMessage, trail_onMatrixTrailEvent, NULL,          trail_onSurfaceTrailEvent,
    trail_onSurfaceTrailEvent, trail_onBoneTrailMessage, trail_onScatterTrailMessage, NULL,          NULL,          func_151BD21C, trail_onFollowTrailMessage,
    flamethrower_onFlameMessage, trail_onCharacterTrailMessage, trail_onRibbonMessage, trail_onFollowerMessage, NULL,          NULL,
};

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

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= gScreenLayout; i++) {
        if (obj->head.model.handles[i] != NULL) {
            func_100043B4((void *)(s32)obj->head.model.handles[i], 4);
        }
    }

    if (obj->head.model.sharedHandle != NULL) {
        func_100043B4((void *)(s32)obj->head.model.sharedHandle, 4);
    }
}

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
                    s32 drawKind, s32 arg6, s32 arg7, CollisionHit *hit, u8 arg9, s32 argA) {
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

    if (hit != NULL) {
        obj->collision = *hit;
    } else {
        obj->collision.flags = 0;
    }

    obj->head.model.model = arg6;
    obj->head.model.unk1C = arg7;
    obj->head.model.unk4 = 0;

    for (i = 0; i < 4; i++) {
        obj->head.model.handles[i] = NULL;
    }
    obj->head.model.sharedHandle = NULL;

    if (arg6 != 0) {
        for (i = 0; i <= gScreenLayout; i++) {
            obj->head.model.handles[i] = light_allocGfxLights(arg6);
        }
        obj->head.model.sharedHandle = func_1515D440();
    }

    obj->head.modelPos.x = 0.0f;
    obj->head.modelPos.y = 0.0f;
    obj->head.modelPos.z = 0.0f;
    bzero(obj->buffers, sizeof(obj->buffers));
    return obj;
}

/* Draws the object through func_151462C8 and then its drawKind handler; returns the advanced display list. */
void *func_15147C4C(void *gfx, EffectObj *obj, s16 arg2) {
    s32 material;

    if (obj->head.tmpl.flags & 0x20) {
        material = obj->head.tmpl.material;
    } else {
        material = 0;
    }
    gfx = func_151462C8(gfx, &obj->head.model, 0, NULL, 0, arg2, &obj->head.modelPos, 2, material);
    if (obj->head.drawKind >= 19) {
        func_1516972C(&obj->head.header);
        return gfx;
    }
    if (obj->head.drawKind != 0) {
        return D_8008A2A4[obj->head.drawKind](obj, gfx, arg2);
    }
    return gfx;
}

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
