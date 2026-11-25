#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define EFFECT_WIDTH_GROWN  2
#define EFFECT_HEIGHT_GROWN 8

/* Fills the next vertex quad of buffer `index`: a billboard of the effect's width and height at pos. */
Vtx *func_15149FD0(FlickerParticle *effect, s16 index) {
    Vtx *vtx;
    Vtx *ret;
    f32 offsetZ;
    f32 offsetX;
    u8 fresh;

    func_151D5D60(effect->head.vtxBuffers, index, sizeof(effect->head.vtxTemplate), &vtx, &fresh);
    ret = vtx;
    // matching: an early `if (vtx == NULL) return NULL;` reorders the branches
    if (vtx) {
        if (fresh) {
            memcpy(effect->head.vtxBuffers[index], effect->head.vtxTemplate, sizeof(effect->head.vtxTemplate));
            memcpy(effect->head.vtxBuffers[index] + 4, effect->head.vtxTemplate, sizeof(effect->head.vtxTemplate));
        }
    } else {
        return NULL;
    }

    offsetZ = D_800DD1D8[index] * effect->head.def.width;
    offsetX = D_800DD1E8[index] * effect->head.def.width;

    vtx[0].v.flag = 0;
    vtx[1].v.flag = 0;
    vtx[2].v.flag = 0;
    vtx[3].v.flag = 0;
    vtx[0].v.ob[0] = vtx[3].v.ob[0] = effect->head.def.pos.x + offsetX;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = effect->head.def.pos.y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = effect->head.def.pos.z - offsetZ;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = effect->head.def.pos.x - offsetX;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = effect->head.def.pos.y + effect->head.def.height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = effect->head.def.pos.z + offsetZ;

    return ret;
}

/* Flickers width, height and unk24 towards random targets that are re-rolled on timers. */
s32 func_1514A19C(FlickerParticle *effect) {
    FlickerParams *params;

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
    effect->head.def.height += (params->heightTarget - effect->head.def.height) * 0.359f;

    params->widthTimer -= D_800BE9E4;
    if (params->widthTimer < 0) {
        params->widthTimer = func_150ADA20() % 17U;
        params->widthTarget = func_150ADA68() * (params->widthMax - params->widthMin) + params->widthMin;
    }
    effect->head.def.width += (params->widthTarget - effect->head.def.width) * 0.186f;

    params->unk24Timer -= D_800BE9E4;
    if (params->unk24Timer < 0) {
        params->unk24Timer = func_150ADA20() % 15U;
        params->unk24Target = func_150ADA68() * (params->unk24Max - params->unk24Min) + params->unk24Min;
    }
    effect->head.def.frameSpeed += (s32)((params->unk24Target - effect->head.def.frameSpeed) * 0.231f);
    return 1;
}

/* Grows width and height towards their targets; once both are within 95%, calls func_1513F680(..., 4, ...). */
s32 func_1514A380(FlickerParticle *effect) {
    FlickerParams *params;
    s32 done;

    done = TRUE;
    params = &effect->params;
    if (!(effect->head.unk74 & EFFECT_WIDTH_GROWN)) {
        effect->head.def.width += (params->widthTarget - effect->head.def.width) * 0.7f;
        if (params->widthTarget * 0.95f < effect->head.def.width) {
            effect->head.unk74 |= EFFECT_WIDTH_GROWN;
        } else {
            done = FALSE;
        }
    }
    if (!(effect->head.unk74 & EFFECT_HEIGHT_GROWN)) {
        effect->head.def.height += (params->heightTarget - effect->head.def.height) * 0.7f;
        if (params->heightTarget * 0.95f < effect->head.def.height) {
            effect->head.unk74 |= EFFECT_HEIGHT_GROWN;
        } else {
            done = FALSE;
        }
    }
    if (done) {
        func_1513F680(effect, effect->head.updateFn1, 4, effect->head.expireFn, effect->head.vertexFn);
    }
    return 1;
}

/* Shrinks width and height by shrinkRate; while unk1C < unk156, sets unk5C = unk1C * unk158. */
s32 func_1514A498(FlickerParticle *effect) {
    effect->head.def.height -= effect->head.def.height * effect->params.shrinkRate;
    effect->head.def.width -= effect->head.def.width * effect->params.shrinkRate;
    if (effect->head.def.lifetime < effect->params.unk156) {
        effect->head.def.unk44 = effect->head.def.lifetime * effect->params.unk158;
    }
    return 1;
}

/* Integrates velocity (damped x/z, gravity on y) into pos, remembering the previous pos. */
s32 func_1514A4EC(FlickerParticle *effect) {
    f32 damping;

    damping = effect->params.damping;
    effect->head.def.rot = effect->head.def.pos;
    effect->params.vel.x *= damping;
    effect->params.vel.y += effect->params.gravity * D_800BE9A4;
    // matching: block boundary makes IDO load the return value 1 into v0 early, freeing it from &D_800BE9A4
    if (1) {}
    effect->params.vel.z *= damping;
    effect->head.def.pos.x += effect->params.vel.x * D_800BE9A4;
    effect->head.def.pos.y += effect->params.vel.y * D_800BE9A4;
    effect->head.def.pos.z += effect->params.vel.z * D_800BE9A4;
    return 1;
}

/* Like func_1514A4EC, but moves prevPos along with pos and then pulls it towards pos by trailFactor. */
s32 func_1514A594(FlickerParticle *effect) {
    f32 dx;
    f32 dy;
    f32 dz;

    effect->params.vel.x *= effect->params.damping;
    effect->params.vel.y += effect->params.gravity * D_800BE9A4;
    effect->params.vel.z *= effect->params.damping;
    dx = effect->params.vel.x * D_800BE9A4;
    dy = effect->params.vel.y * D_800BE9A4;
    dz = effect->params.vel.z * D_800BE9A4;
    // matching: volatile read makes pos.x a fresh CSE entry after pos.y/z, giving the lerp its FP registers
    effect->head.def.pos.x = *(volatile f32 *)&effect->head.def.pos.x + dx;
    effect->head.def.pos.y += dy;
    effect->head.def.pos.z += dz;
    effect->head.def.rot.x += dx;
    effect->head.def.rot.y += dy;
    effect->head.def.rot.z += dz;
    effect->head.def.rot.x = effect->head.def.pos.x + (effect->head.def.rot.x - effect->head.def.pos.x) * effect->head.def.scale.z;
    effect->head.def.rot.y = effect->head.def.pos.y + (effect->head.def.rot.y - effect->head.def.pos.y) * effect->head.def.scale.z;
    effect->head.def.rot.z = effect->head.def.pos.z + (effect->head.def.rot.z - effect->head.def.pos.z) * effect->head.def.scale.z;
    return 1;
}
