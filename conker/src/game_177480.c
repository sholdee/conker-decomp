#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_1513F680();
extern void func_151D5D60();

/* Per-effect behaviour parameters, at 0x110 in Effect. */
typedef struct {
    /* 0x00 */ f32 widthMax;
    /* 0x04 */ f32 widthMin;
    /* 0x08 */ f32 widthTarget;
    /* 0x0C */ f32 heightMid;
    /* 0x10 */ f32 heightMin;
    /* 0x14 */ f32 heightMax;
    /* 0x18 */ f32 heightTarget;
    /* 0x1C */ s16 widthTimer;
    /* 0x1E */ s16 heightTimer;
    /* 0x20 */ struct17 vel;
    /* 0x2C */ f32 gravity;
    /* 0x30 */ f32 damping;
    /* 0x34 */ f32 shrinkRate;
    /* 0x38 */ f32 unk24Max;
    /* 0x3C */ f32 unk24Min;
    /* 0x40 */ f32 unk24Target;
    /* 0x44 */ s16 unk24Timer;
    /* 0x46 */ s16 unk156;
    /* 0x48 */ s16 unk158;
} EffectParams;

/* Effect instance shared by the update callbacks in this file. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[0x6];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[0x4];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 height;
    /* 0x034 */ struct17 pos;
    /* 0x040 */ struct17 prevPos;
    /* 0x04C */ u8 pad4C[0x8];
    /* 0x054 */ f32 trailFactor;
    /* 0x058 */ u8 pad58[0x4];
    /* 0x05C */ u8 unk5C;
    /* 0x05D */ u8 pad5D[0x13];
    /* 0x070 */ u8 unk70;
    /* 0x071 */ u8 pad71;
    /* 0x072 */ u8 unk72;
    /* 0x073 */ u8 unk73;
    /* 0x074 */ u8 flags;
    /* 0x075 */ u8 pad75[0x4B];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuffers[4];
    /* 0x110 */ EffectParams params;
} Effect;

#define EFFECT_WIDTH_GROWN  2
#define EFFECT_HEIGHT_GROWN 8

/* Fills the next vertex quad of buffer `index`: a billboard of the effect's width and height at pos. */
Vtx *func_15149FD0(Effect *effect, s16 index) {
    Vtx *vtx;
    Vtx *ret;
    f32 offsetZ;
    f32 offsetX;
    u8 fresh;

    func_151D5D60(effect->vtxBuffers, index, sizeof(effect->vtxTemplate), &vtx, &fresh);
    ret = vtx;
    if (vtx) {
        if (fresh) {
            memcpy(effect->vtxBuffers[index], effect->vtxTemplate, sizeof(effect->vtxTemplate));
            memcpy(effect->vtxBuffers[index] + 4, effect->vtxTemplate, sizeof(effect->vtxTemplate));
        }
    } else {
        return NULL;
    }

    offsetZ = D_800DD1D8[index] * effect->width;
    offsetX = D_800DD1E8[index] * effect->width;

    vtx[0].v.flag = 0;
    vtx[1].v.flag = 0;
    vtx[2].v.flag = 0;
    vtx[3].v.flag = 0;
    vtx[0].v.ob[0] = vtx[3].v.ob[0] = effect->pos.unk0 + offsetX;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = effect->pos.unk4;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = effect->pos.unk8 - offsetZ;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = effect->pos.unk0 - offsetX;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = effect->pos.unk4 + effect->height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = effect->pos.unk8 + offsetZ;

    return ret;
}

/* Flickers width, height and unk24 towards random targets that are re-rolled on timers. */
s32 func_1514A19C(Effect *effect) {
    EffectParams *params;
    s32 prev;

    params = &effect->params;

    params->heightTimer -= D_800BE9E4;
    if (params->heightTimer < 0) {
        params->heightTimer = func_150ADA20() % 6U;
        if (func_150ADA20() & 3) {
            params->heightTarget = func_150ADA68() * (params->heightMid - params->heightMin) + params->heightMin;
        } else {
            params->heightTarget = func_150ADA68() * (params->heightMax - params->heightMid) + params->heightMid;
        }
    }
    effect->height = effect->height + (params->heightTarget - effect->height) * 0.359f;

    params->widthTimer -= D_800BE9E4;
    if (params->widthTimer < 0) {
        params->widthTimer = func_150ADA20() % 17U;
        params->widthTarget = func_150ADA68() * (params->widthMax - params->widthMin) + params->widthMin;
    }
    effect->width = effect->width + (params->widthTarget - effect->width) * 0.186f;

    params->unk24Timer -= D_800BE9E4;
    if (params->unk24Timer < 0) {
        params->unk24Timer = func_150ADA20() % 15U;
        params->unk24Target = func_150ADA68() * (params->unk24Max - params->unk24Min) + params->unk24Min;
    }
    prev = effect->unk24;
    effect->unk24 += (s32)((params->unk24Target - (f32)prev) * 0.231f);
    return 1;
}

/* Grows width and height towards their targets; once both are within 95%, calls func_1513F680 with state 4. */
s32 func_1514A380(Effect *effect) {
    EffectParams *params;
    s32 done;

    done = TRUE;
    params = &effect->params;
    if (!(effect->flags & EFFECT_WIDTH_GROWN)) {
        effect->width += (params->widthTarget - effect->width) * 0.7f;
        if (params->widthTarget * 0.95f < effect->width) {
            effect->flags |= EFFECT_WIDTH_GROWN;
        } else {
            done = FALSE;
        }
    }
    if (!(effect->flags & EFFECT_HEIGHT_GROWN)) {
        effect->height += (params->heightTarget - effect->height) * 0.7f;
        if (params->heightTarget * 0.95f < effect->height) {
            effect->flags |= EFFECT_HEIGHT_GROWN;
        } else {
            done = FALSE;
        }
    }
    if (done) {
        func_1513F680(effect, effect->unk70, 4, effect->unk72, effect->unk73);
    }
    return 1;
}

/* Shrinks width and height by shrinkRate; while unk1C < unk156, sets unk5C = unk1C * unk158. */
s32 func_1514A498(Effect *effect) {
    effect->height = effect->height - effect->height * effect->params.shrinkRate;
    effect->width = effect->width - effect->width * effect->params.shrinkRate;
    if (effect->unk1C < effect->params.unk156) {
        effect->unk5C = effect->unk1C * effect->params.unk158;
    }
    return 1;
}

/* Integrates velocity (damped x/z, gravity on y) into pos, remembering the previous pos. */
s32 func_1514A4EC(Effect *effect) {
    f32 damping;
    s32 dummy;

    damping = effect->params.damping;
    effect->prevPos = effect->pos;
    effect->params.vel.unk0 = effect->params.vel.unk0 * damping;
    effect->params.vel.unk4 = effect->params.vel.unk4 + effect->params.gravity * D_800BE9A4;
    dummy = effect ? 1 : 0;
    effect->params.vel.unk8 = effect->params.vel.unk8 * damping;
    effect->pos.unk0 = effect->pos.unk0 + effect->params.vel.unk0 * D_800BE9A4;
    effect->pos.unk4 = effect->pos.unk4 + effect->params.vel.unk4 * D_800BE9A4;
    effect->pos.unk8 = effect->pos.unk8 + effect->params.vel.unk8 * D_800BE9A4;
    return 1;
}

/* Like func_1514A4EC, but moves prevPos along with pos and then pulls it towards pos by trailFactor. */
s32 func_1514A594(Effect *arg0) {
    f32 vx;
    f32 vy;
    f32 vz;
    Effect *new_var;

    arg0->params.vel.unk0 = arg0->params.vel.unk0 * arg0->params.damping;
    new_var = arg0;
    new_var->params.vel.unk4 = arg0->params.vel.unk4 + (new_var->params.gravity * D_800BE9A4);
    arg0->params.vel.unk8 = arg0->params.vel.unk8 * new_var->params.damping;
    vx = new_var->params.vel.unk0 * D_800BE9A4;
    vy = new_var->params.vel.unk4 * D_800BE9A4;
    vz = new_var->params.vel.unk8 * D_800BE9A4;
    new_var->pos.unk0 = arg0->pos.unk0 + vx;
    arg0->pos.unk4 = new_var->pos.unk4 + vy;
    arg0->pos.unk8 = arg0->pos.unk8 + vz;
    new_var->prevPos.unk0 = new_var->prevPos.unk0 + vx;
    new_var->prevPos.unk4 = arg0->prevPos.unk4 + vy;
    arg0->prevPos.unk8 = new_var->prevPos.unk8 + vz;
    new_var->prevPos.unk0 = new_var->pos.unk0 + ((new_var->prevPos.unk0 - arg0->pos.unk0) * new_var->trailFactor);
    new_var->prevPos.unk4 = arg0->pos.unk4 + ((new_var->prevPos.unk4 - arg0->pos.unk4) * new_var->trailFactor);
    new_var->prevPos.unk8 = arg0->pos.unk8 + ((new_var->prevPos.unk8 - arg0->pos.unk8) * new_var->trailFactor);
    return 1;
}
