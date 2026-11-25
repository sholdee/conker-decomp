#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Effect descriptor that func_1515FF74 copies into the new effect node at 0xE. */
typedef struct {
    /* 0x0 */ u8 flags;        /* 1: the effect ends when lifetime runs out */
    /* 0x1 */ s8 updateIndex;  /* D_8008B0D0 handler */
    /* 0x2 */ s8 drawIndex;    /* D_8008B0D8 handler */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 lifetime;
    /* 0x6 */ u8 messageIndex; /* D_8008B0E4 handler */
    /* 0x7 */ u8 pad7;
} EffectDesc;

/*
 * Mosaic (pixelate) effect: a screen rectangle around one of the owner's bones is redrawn in
 * blocks of 0x10000 / step pixels.
 */
typedef struct {
    /* 0x000 */ struct127 *owner;
    /* 0x004 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x008 */ f32 halfWidth;
    /* 0x00C */ f32 halfHeight;
    /* 0x010 */ s32 step;  /* 16.16 source advance per pixel */
    /* 0x014 */ u8 flags;  /* 1: the rectangle is on screen */
    /* 0x018 */ s32 left;
    /* 0x01C */ s32 top;
    /* 0x020 */ s32 right;
    /* 0x024 */ s32 bottom;
    /* 0x028 */ f32 center[2];
    /* 0x030 */ s16 srcColumn[640];
    /* 0x530 */ s16 srcRow[480];
} MosaicData; /* size 0x8F0 */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ MosaicData data;
} MosaicEffect;

extern MosaicEffect *func_1515FF74(EffectDesc *desc, s32 dataSize, u8 arg2, s32 arg3);

/* Starts a mosaic effect over owner for lifetime frames. */
MosaicEffect *func_150CE150(struct127 *owner, s16 lifetime, u8 arg2, s32 arg3) {
    EffectDesc desc;
    MosaicData data;
    MosaicEffect *effect;

    if (owner == NULL) {
        return NULL;
    }
    data.halfWidth = 30000.0f;
    desc.flags = 1;
    desc.updateIndex = 1;
    desc.drawIndex = 0;
    desc.lifetime = lifetime;
    desc.messageIndex = 2;
    data.step = 10000;
    data.flags = 0;
    data.halfHeight = 30000.0f;
    data.owner = owner;
    data.ownerId = owner->unique_id;
    effect = func_1515FF74(&desc, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    // matching: the original falls off the end here without returning effect
}


extern f32 D_800A07F0[3];
void func_15143134(f32 *in, f32 *out, Mtx *mtx);
s32 func_15144CEC(f32 *pos, f32 *screen, f32 *arg2, f32 *arg3, f32 *scale, s32 arg5);

/*
 * Update handler: projects the owner's bone 52 to the screen and sets the mosaic rectangle,
 * clamped to the 292x216 view. Returns 0 to end the effect once the owner is gone.
 */
s32 func_150CE200(MosaicEffect *effect) {
    MosaicData *mosaic = &effect->data;
    f32 pos[3];
    f32 projectedZ;
    f32 projectedW;
    f32 scale;

    if (mosaic->owner->interaction_state == 0) {
        return 0;
    }
    if (mosaic->owner->unique_id != mosaic->ownerId) {
        return 0;
    }
    if (mosaic->owner->unk1D4 == NULL) {
        mosaic->flags &= ~1;
    } else {
        func_15143134(D_800A07F0, pos, (Mtx *)mosaic->owner->unk1D4 + 52);
        if (func_15144CEC(pos, mosaic->center, &projectedZ, &projectedW, &scale, D_80082FA4)) {
            f32 dx;
            f32 dy;

            dx = mosaic->halfWidth * scale;
            dy = mosaic->halfHeight * scale;
            mosaic->left = mosaic->center[0] - dx;
            mosaic->top = mosaic->center[1] - dy;
            mosaic->right = mosaic->center[0] + dx;
            mosaic->bottom = mosaic->center[1] + dy;
            if (mosaic->left > 291.0f || mosaic->right < 0.0f || mosaic->top > 215.0f || mosaic->bottom < 0.0f) {
                mosaic->flags &= ~1;
            } else {
                if (mosaic->left < 0.0f) {
                    mosaic->left = 0;
                }
                if (mosaic->top < 0.0f) {
                    mosaic->top = 0;
                }
                if (mosaic->right > 291.0f) {
                    mosaic->right = 291;
                }
                if (mosaic->bottom > 215.0f) {
                    mosaic->bottom = 215;
                }
                mosaic->flags |= 1;
            lbl_ret: // matching: an unused label here keeps IDO from merging this return with the one below
                return 1;
            }
        } else {
            mosaic->flags &= ~1;
        }
    }
    return 1;
}

/*
 * Draw handler: redraws the mosaic rectangle of the framebuffer in place. Each pixel takes the
 * colour of its block's source pixel; rows whose source row equals the row below are copied
 * whole. Works bottom-up and right-to-left so sources are read before they are overwritten.
 */
void func_150CE450(MosaicEffect *effect, u16 *fb) {
    MosaicData *mosaic = &effect->data;
    s16 *srcColumn;
    s16 *srcRow;
    u32 acc;
    u32 i;
    s32 first;
    s32 src;
    s32 y;
    s32 x;

    if ((mosaic->flags & 1) == 0) {
        return;
    }

    srcColumn = mosaic->srcColumn;
    srcRow = mosaic->srcRow;

    acc = 0;
    first = mosaic->left;
    src = first;
    for (i = first; i <= mosaic->right; i++) {
        srcColumn[i] = src;
        acc += mosaic->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    acc = 0;
    first = mosaic->top;
    src = first;
    for (i = first; i <= mosaic->bottom; i++) {
        srcRow[i] = src;
        acc += mosaic->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    for (y = mosaic->bottom; y >= mosaic->top; y--) {
        if (y != mosaic->bottom && srcRow[y + 1] == srcRow[y]) {
            // matching: byte offsets keep fb as the first addu operand (&fb[i] swaps them)
            memcpy((u8 *)fb + (y * D_800BE620 + mosaic->left) * 2,
                   (u8 *)fb + ((y + 1) * D_800BE620 + mosaic->left) * 2,
                   (mosaic->right - mosaic->left + 1) * sizeof(u16));
        } else {
            for (x = mosaic->right; x >= mosaic->left; x--) {
                fb[y * D_800BE620 + x] = fb[srcRow[y] * D_800BE620 + srcColumn[x]];
            }
        }
    }
}

/* Payload of the messages the effect handlers receive. */
typedef union {
    struct {
        /* 0x00 */ struct127 *object;
        /* 0x04 */ u8 id;
    } one;          /* message 0 */
    struct {
        /* 0x00 */ struct127 *from;
        /* 0x04 */ struct127 *to;
        /* 0x08 */ u8 fromId;
        /* 0x09 */ u8 toId;
    } pair;         /* message 0x2D */
} ObjectMessage;

/*
 * Message handler: message 0x2D moves the effect between the two objects of the pair,
 * message 0 ends it when its owner goes away.
 */
void func_150CE694(MosaicEffect *effect, ObjectMessage *msg, u8 type) {
    MosaicData *mosaic = &effect->data;
    struct127 *object;

    if (type == 0x2D) {
        if (mosaic->owner == msg->pair.from) {
            mosaic->owner = msg->pair.to;
            mosaic->ownerId = msg->pair.toId;
        } else {
            if (mosaic->owner == msg->pair.to) {
                mosaic->owner = msg->pair.from;
                mosaic->ownerId = msg->pair.fromId;
            }
        // matching: an empty labelled statement makes this branch jump straight to the epilogue
        end_swap:
            ;
        }
    } else if (type == 0) {
        object = msg->one.object;
        if (object == mosaic->owner || msg->one.id == mosaic->ownerId) {
            func_1516972C((struct102 *)effect);
        }
    }
}

extern u16 D_800BE648[];
extern u8 D_800A080F[];
extern u8 D_800A0818[];
void func_15052590(struct127 *arg0);
void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15063168(struct127 *arg0);

/*
 * Per-frame control update: picks an action (unk232) from the button word D_800CC288, sets the
 * target speed (unk44) and heading (unk78), then eases unkC4 toward the clamped turn. Objects
 * with id 0x21 use their own tables and a random wandering heading.
 */
void func_150CE740(struct127 *obj) {
    struct126 *owner;
    u16 heading;
    u16 prevFacing;
    u8 isType21;
    u8 action;
    s16 turn;
    s16 v; // matching: one s16 for both the control index and the 3/4 anim value merges their registers

    isType21 = 0;
    prevFacing = obj->unk7A;
    if (obj->id == 0x21) {
        isType21 = 1;
    }
    v = obj->unk13C;
    if ((v == 0) && (D_800BE616 == 0)) {
        goto control_done; // matching: a goto (not an if block) lets IDO keep D_800BE616 in a register
    }
    action = 0;
    if (D_800BE616 != 0) {
        if (obj->unk31C->unk84 != 0) {
            func_1504C854(obj);
        } else {
            D_800CC288 = D_800BE710[D_800C3E78];
        }
    } else {
        D_800CC288 = D_800BE648[v];
    }
    if ((D_800CC288 & 0x8000) && (obj->disable_jump == 0) && (obj->unk28 == 0.0f)) {
        action = 1;
    }
    if ((D_800CC288 & 0x6000) && (obj->unk28 == 0.0f) &&
        ((isType21 && (obj->disable_run != 0xFF)) || (obj->disable_run == 0))) {
        action = 2;
    }
    if (action) {
        if (isType21) {
            action += 4;
        }
        if (D_800BE616 != 0) {
            action += 2;
        }
        obj->disable_run = 180;
        obj->unk232 = D_800A080F[action];
        obj->unk218 = NULL;
        if ((obj->unk232 == 0x22) && (obj->unk2E4 == 0)) {
            obj->unk31C->unk1AA++;
        }
    }
    obj->unk124 = 0;
    if ((obj->disable_run == 0) && ((D_800BE616 == 0) || (obj->unk31C->unk84 == 0))) {
        if (D_800BE616 != 0) {
            /* camera yaw in degrees -> 16-bit binary angle (65536 / 360) */
            D_800CC280 = D_800DBFF0[D_800C3E78].unk37C * 182.0444488525390625f;
            heading = func_1505A630(((struct49 *)D_800CC284)->unk2, ((struct49 *)D_800CC284)->unk3, 0) + D_800CC280;
            obj->unk44 = func_1505A5CC((struct49 *)D_800CC284);
        } else {
            obj->unk44 = D_800CC2D0[obj->unk124].unk44 * 1.75f;
            heading = D_800CC2D0[obj->unk124].unk31C->unk4C;
        }
        if (obj->unk223 == 13) {
            obj->unk44 = 0.0f;
        }
        if (isType21) {
            if (obj->unk44 < 5.0f) {
                heading = func_150ADA20();
                obj->unk44 = (func_150ADA20() % 60U) + 20.0f;
            }
            obj->unk78 = ((func_150ADA20() % 10000U) + heading) - 5000;
            obj->unk44 = (obj->unk44 * 0.5f) + 22.0f;
            if ((u8)(((s32)(obj->unk76 - obj->unk78) >> 8) - 0x50) < 0x60) {
                obj->unk218 = NULL;
                obj->unk232 = 0x10;
            }
        } else if (obj->unk44 < 5.0f) {
            obj->unk44 = 0.0f;
            if (obj->unk232 != 5) {
                obj->unk78 = obj->unk76;
            }
        } else {
            obj->unk78 = heading;
            if (obj->unk223 == 1) {
                v = 3;
                if (obj->xz_velocity < 35.0f) {
                    v = 4;
                }
                obj->unk1E6 = v;
                obj->unk1E5 = v;
                if ((u8)(((s32)(obj->unk76 - obj->unk78) >> 8) - 0x50) < 0x60) {
                    if (obj->xz_velocity > 35.0f) {
                        obj->unk232 = 4;
                    } else {
                        obj->unk232 = 5;
                    }
                } else {
                    obj->unk232 = 3;
                }
                obj->unk218 = NULL;
            }
        }
    }
control_done:
    if ((D_800BE616 == 0) || (D_800C3E78 >= D_8008FD8C) || (obj->unk31C->unk120 == 0)) {
        func_15052590(obj);
    }
    if ((D_800CC2D0[0].unk31C->unk27 != 0) && ((obj->unk232 != D_800A0818[isType21]) || (obj->stunned != 0))) {
        D_800CC2D0[0].unk31C->unk27 = 0;
        D_800CC2D0[0].disable_run = 0;
        D_800CC2D0[0].unk83 = 0;
        D_800CC2D0[0].immune = 0;
        obj->unk232 = 0x11;
    }
    if (D_800BE616 == 0) {
        /* D_800A0818 holds three 2-entry tables indexed by isType21 */
        // matching: (&table[i])[n] adds n after i; table[i + n] folds n into the address
        if (obj->unk232 == D_800A0818[isType21]) {
            func_15052F9C(obj, 156.0f, (&D_800A0818[isType21])[4], 4, 0, (&D_800A0818[isType21])[2], 0xFF, 0, 0, 0);
        } else if ((D_800CC268 & 1) && (D_800CC2D0[obj->unk124].unk65 == 0) && (obj->unk25C & 0x400)) {
            obj->unk124 = 0;
            func_1505327C(obj, 50.0f, 3.0f, D_800A0818[isType21], (&D_800A0818[isType21])[4]);
        }
    }
    turn = 0;
    if ((isType21 && (obj->xz_velocity > 5.0f)) || (!isType21 && (obj->unk84.uh == 10))) {
        turn = obj->unk76 - obj->unk78;
        if (turn > 0x3000) {
            turn = 0x3000;
        }
        if (turn < -0x3000) {
            turn = -0x3000;
        }
        if (isType21 && (obj->unk13C == 0)) {
            turn >>= 1;
        }
    }
    obj->unkC4 += ((turn * 0.005f) - obj->unkC4) * 0.15f;
    if (isType21) {
        obj->unk2E4 += ((s16)((prevFacing - obj->unk7A) * 4) - (s16)obj->unk2E4) / 8;
        obj->unk2E8 += (-turn - obj->unk2E8) / 8;
    }
    if ((D_800BE616 != 0) && (D_800CC268 != 0)) {
        owner = obj->unk31C;
        if ((owner != NULL) && (((u8 *)owner)[0x1AC] != 0)) { /* past the end of struct126 as declared */
            func_15063168(obj);
        }
    }
}
