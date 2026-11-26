#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Update handlers of a strip (StripObj.updateIdx); 0 removes the strip. */
static s32 (*D_80088760[])(StripObj *) = { func_150C5B88, func_150C5BD4 };

/* Render setup and reset display lists of the strips; the strip type's entry in the object type table
 * D_8008B4D4 points to them. */
Gfx D_80088768[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx D_80088790[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    // gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5) as Rare's gbi.h encoded it: it
    // shifted k2's high bits left (_SHIFTL(k2, 5, 4)) where 2.0L shifts them right
    {{ 0xEC15FDF0, 0x3B78E42A }},
    gsSPEndDisplayList(),
};

/* Destroy and free handlers per strip kind (StripObj.unk38) */
static void (*D_800887B8[])(StripObj *) = { func_150C5430, func_150C5510 };
static void (*D_800887C0[])(StripObj *) = { func_150C5450, func_150C553C };
/* Message handlers per strip kind */
static void (*D_800887C8[])(StripObj *, ActorMsg *, u8) = { NULL, func_150C5D0C };

/** Allocates a strip (0xC8-byte header plus `payloadSize` bytes of payload, its target) and caches its length. */
StripObj *func_150C5370(StripDesc *desc, s32 payloadSize) {
    StripObj *strip;

    strip = func_15167A68(0x46, 0, payloadSize + 0xC8, 1, 0xFF, 1);
    if (strip == NULL) {
        return NULL;
    }
    memcpy(&strip->p0, desc, sizeof(StripDesc));
    {
        Vec3f diff;

        diff.x = strip->p0.x - strip->p1.x;
        diff.y = strip->p0.y - strip->p1.y;
        diff.z = strip->p0.z - strip->p1.z;
        strip->length = func_15143E64(&diff);
    }
    strip->unk10 = 1;
    strip->unk14 = 0;
    return strip;
}

/** Destroy handler: hands the strip to func_15169804. */
void func_150C5430(StripObj *strip) {
    func_15169804(&strip->header);
}

/** Free handler: hands the strip to func_15169824. */
void func_150C5450(StripObj *strip) {
    func_15169824(&strip->header);
}

/** Calls the D_800887B8 handler for unk38, or the first one when unk38 is out of range. */
void func_150C5470(StripObj *strip) {
    s32 idx = strip->unk38;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 2) {
        idx = 0;
    }
    D_800887B8[idx](strip);
}

/** Calls the D_800887C0 handler for unk38, or the first one when unk38 is out of range. */
void func_150C54C0(StripObj *strip) {
    s32 idx = strip->unk38;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 2) {
        idx = 0;
    }
    D_800887C0[idx](strip);
}

/** Detaches the strip from its target character, then destroys it (func_150C5430). */
void func_150C5510(StripObj *strip) {
    func_1514EDF0(strip, strip->target.obj);
    func_150C5430(strip);
}

/** Detaches the strip from its target character, then frees it (func_150C5450). */
void func_150C553C(StripObj *strip) {
    func_1514EDF0(strip, strip->target.obj);
    func_150C5450(strip);
}

/** Runs the strip's update handler; removes the strip when it returns 0, else refreshes the length if it moved. */
void func_150C5568(StripObj *strip) {
    s32 idx;
    u8 remove;
    Vec3f prev;

    if (strip->unk10 & 1) {
        remove = FALSE;
        idx = strip->updateIdx;
        prev.x = strip->p0.x - strip->p1.x;
        prev.y = strip->p0.y - strip->p1.y;
        prev.z = strip->p0.z - strip->p1.z;

        if (idx != -1) {
            if (D_80088760[idx](strip) == 0) {
                remove = TRUE;
            }
        }
        if (!remove) {
            Vec3f diff;
            s32 pad; // matching: reserves the stack word the target frame has

            diff.x = strip->p0.x - strip->p1.x;
            diff.y = strip->p0.y - strip->p1.y;
            diff.z = strip->p0.z - strip->p1.z;
            if (diff.x != prev.x || diff.y != prev.y || diff.z != prev.z) {
                strip->length = func_15143E64(&diff);
            }
        }
        if (remove) {
            func_1516972C(&strip->header);
        }
    }
}

extern s32 D_800A4AC8[][2];

/** Draws the strip for viewport `view` as a quad from p0 to p1 widened sideways to face the camera. */
Gfx *func_150C56A4(Gfx *gfx, StripObj *strip, s16 view) {
    s32 pad; // matching: unused stack word in the target frame
    u8 update;
    Vtx *vtx;
    Vec3f *eye;
    Vec3f dir;
    Vec3f toEye;
    Vec3f side;
    f32 lensq;
    f32 scale;
    s32 texV;

    if (strip->unk10 & 1) {
        // matching: dead reads/stores; they fix the order of IDO's FP spill slots
        side.z = dir.z;
        scale = dir.y * toEye.z - toEye.y * dir.z;
        lensq = dir.z * toEye.x - toEye.z * dir.x;
        toEye.z = dir.z = 0.0f;

        update = TRUE;
        vtx = strip->quad[D_800BE9C0];
        eye = &camera_global_Cameras[view].eye;
        gfx = func_15142B7C(gfx, 1, 0x600);
        gfx = func_15142E24(gfx, &D_80090DE8, 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
        if (camera_global_Cameras[view].SpecialFlags & 1) {
            gfx = func_15142C10(gfx, D_800D9B78[view][0], D_800D9B78[view][1], D_800D9B78[view][2], 0xFF, &update);
            gfx = func_15142CF0(gfx, 0xF2, 0, D_800D9B68[view][0], D_800D9B68[view][1], D_800D9B68[view][2], 0xFF, &update);
            gfx = func_1513F4E4(gfx, 30, &update);
        } else {
            gfx = func_15142C10(gfx, 0, 0, 0, 0, &update);
            gfx = func_15142CF0(gfx, 0, 0, 0, 0, 0, 0, &update);
            gfx = func_1513F4E4(gfx, 19, &update);
        }
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1][0] | D_800A4AC8[1][1], &update);

        scale = strip->p1.z;
        dir.x = strip->p1.x - strip->p0.x;
        dir.y = strip->p1.y - strip->p0.y;
        dir.z = strip->p1.z - strip->p0.z;
        toEye.x = strip->p1.x - eye->x;
        toEye.y = strip->p1.y - eye->y;
        toEye.z = strip->p1.z - eye->z;
        side.x = dir.y * toEye.z - toEye.y * dir.z;
        side.y = dir.z * toEye.x - toEye.z * dir.x;
        side.z = dir.x * toEye.y - toEye.x * dir.y;
        lensq = side.x * side.x + side.y * side.y + side.z * side.z;
        if (lensq == 0.0f) {
            side.x = 0.0f;
            side.y = 0.0f;
            side.z = 0.0f;
        } else {
            scale = 8.399999619f / sqrtf(lensq);
            side.x *= scale;
            side.y *= scale;
            side.z *= scale;
        }
        texV = strip->length * strip->texScale * 64.0f;
        vtx->v.ob[0] = strip->p1.x + side.x;
        vtx->v.ob[1] = strip->p1.y + side.y;
        vtx->v.ob[2] = strip->p1.z + side.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = strip->p1.x - side.x;
        vtx->v.ob[1] = strip->p1.y - side.y;
        vtx->v.ob[2] = strip->p1.z - side.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = strip->p0.x - side.x;
        vtx->v.ob[1] = strip->p0.y - side.y;
        vtx->v.ob[2] = strip->p0.z - side.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = texV;
        vtx++;
        vtx->v.ob[0] = strip->p0.x + side.x;
        vtx->v.ob[1] = strip->p0.y + side.y;
        vtx->v.ob[2] = strip->p0.z + side.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = texV;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

/** Moves p1 to the target character's position; returns 0 once the character is gone. */
s32 func_150C5B88(StripObj *strip) {
    StripTarget *target = &strip->target;

    if (target->obj->type == 0) {
        return 0;
    }
    strip->p1.x = target->obj->xpos;
    strip->p1.y = target->obj->ypos;
    strip->p1.z = target->obj->zpos;
    return 1;
}

/**
 * Moves p1 to the target's bone-local offset transformed by its bone matrix, or to its position when it has
 * no matrices. Returns 0 once the character is gone or its slot was reused.
 */
s32 func_150C5BD4(StripObj *strip) {
    StripTarget *target = &strip->target;
    MtxF *bones;

    if (target->obj->type == 0) {
        return 0;
    }
    if (target->obj->unique_id != target->uniqueId) {
        return 0;
    }
    bones = target->obj->ob_matrix_ptr;
    if (bones == NULL) {
        strip->p1.x = target->obj->xpos;
        strip->p1.y = target->obj->ypos;
        strip->p1.z = target->obj->zpos;
    } else {
        // matching: byte offset; &bones[target->bone] swaps the addu operands
        func_15143134(&target->offset, &strip->p1, (u8 *)bones + target->bone * sizeof(MtxF));
    }
    return 1;
}

/** Sends message 1 to `obj`'s type-0x15 attachments (its strips). */
void func_150C5C74(Actor *obj) {
    func_1514D3B0(obj, 0x15, 1, 0);
}

/** Sends message 2 to `obj`'s type-0x15 attachments (its strips). */
void func_150C5C9C(Actor *obj) {
    func_1514D3B0(obj, 0x15, 2, 0);
}

/** Forwards a message to the D_800887C8 handler for unk38, if there is one. */
void func_150C5CC4(StripObj *strip, ActorMsg *data, u8 msg) {
    void (*handler)(StripObj *, ActorMsg *, u8) = D_800887C8[strip->unk38];

    if (handler != NULL) {
        handler(strip, data, msg);
    }
}

/**
 * Message 0: removes the strip if it targets data's character (same pointer or same unique id).
 * Message 0x2D: retargets the strip from either of data's two characters (A, B) to the other.
 */
void func_150C5D0C(StripObj *strip, ActorMsg *data, u8 msg) {
    StripTarget *target = &strip->target;

    if (msg == 0) {
        if (data->ref.actor == target->obj || data->ref.uniqueId == target->uniqueId) {
            func_1516972C(&strip->header);
        }
    } else if (msg == 0x2D) {
        if (target->obj == data->swap.actorA) {
            target->obj = data->swap.actorB;
            target->uniqueId = data->swap.idB;
        } else if (target->obj == data->swap.actorB) {
            target->obj = data->swap.actorA;
            target->uniqueId = data->swap.idA;
        }
    }
}
