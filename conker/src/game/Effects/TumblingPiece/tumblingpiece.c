#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"

/* The two display lists (indices into gEffectActorDisplayLists) that a piece picks from, and the alternative pair. */
static s32 gTumblePieceDls[2] = { 0, 1 };
static s32 gTumblePieceAltDls[2] = { 2, 3 };

/*
 * Spawns a tumbling piece at position with the given motion; it fades out over its last 12 frames.
 * altDisplayList picks the display list table (gTumblePieceAltDls instead of gTumblePieceDls).
 */
EffectActor *effect_spawnTumblingPiece(Vec3f *position, Vec3f *angle, f32 scale, Vec3f *velocity, Vec3f *angularVelocity,
                           f32 gravity, f32 damping, u8 flags, s16 lifetime, u8 altDisplayList, s32 extraSize,
                           u8 owner, s32 list) {
    EffectActor *actor;
    EffectActorTemplate template;
    TumbleMotion motion;
    EffectFade fade;

    motion.rot = *angle;
    motion.vel = *velocity;
    motion.rotVel = *angularVelocity;
    motion.scale = scale;
    motion.gravity = gravity;
    motion.drag = damping;
    template.flags = flags;
    template.updateCallback = 1;
    template.drawCallback = 1;
    template.lifetime = lifetime;

    if (altDisplayList) {
        template.displayList = gTumblePieceAltDls[math_random() & 1];
    } else {
        template.displayList = gTumblePieceDls[math_random() & 1];
    }

    template.render.setGeometryMode = 0x220205;
    template.render.clearGeometryMode = 0x40600;
    template.render.envMode = 7;
    template.render.renderMode = 1;
    template.render.combiner = 0x4A;
    template.render.colorDither = 0x80;
    template.render.alphaDither = 0x20;
    template.render.otherModeH = 0;
    template.render.primMode = 0;
    template.unk01 = 1;
    template.color0.red = 255;
    template.color0.green = 255;
    template.color0.blue = 255;
    template.color0.alpha = 255;
    template.color1.red = 255;
    template.color1.green = 255;
    template.color1.blue = 255;
    template.color1.alpha = 255;
    template.owner = NULL;
    template.ownerId = 0;
    template.attachMode = 2;
    template.position = *position;

    fade.duration = 12;
    fade.alphaPerTick = 21;

    actor = effect_newEffectActor(&template, 3, 255, 1, extraSize + 0x38, owner, list);
    if (actor == NULL) {
        return NULL;
    }
    memcpy(&actor->data.piece.fade, &fade, sizeof(fade));
    memcpy(&actor->data.piece.motion, &motion, sizeof(motion));
    return actor;
}

/*
 * Update callback: fades the actor out, applies gravity and per-frame damping to the velocity,
 * moves the position with the average velocity over the step and spins the angle.
 */
s32 effect_updateTumblingPiece(EffectActor *actor) {
    TumbleMotion *motion = &actor->data.piece.motion;

    effect_fadeEffectActor(actor);

    {
        Vec3f prevVel;
        s32 i;

        prevVel = motion->vel;
        motion->vel.y += motion->gravity * gFrameDelta;
        for (i = gFrameTicks; i != 0; i--) {
            motion->vel.x *= motion->drag;
            motion->vel.y *= motion->drag;
            motion->vel.z *= motion->drag;
        }
        {
            f32 accelX;
            f32 accelY;
            f32 accelZ;

            accelX = (motion->vel.x - prevVel.x) * gFrameDeltaInv;
            accelY = (motion->vel.y - prevVel.y) * gFrameDeltaInv;
            accelZ = (motion->vel.z - prevVel.z) * gFrameDeltaInv;
            actor->tmpl.position.x += (prevVel.x + 0.5f * accelX * gFrameDelta) * gFrameDelta;
            actor->tmpl.position.y += (prevVel.y + 0.5f * accelY * gFrameDelta) * gFrameDelta;
            actor->tmpl.position.z += (prevVel.z + 0.5f * accelZ * gFrameDelta) * gFrameDelta;
        }
    }
    motion->rot.x += motion->rotVel.x * gFrameDelta;
    motion->rot.y += motion->rotVel.y * gFrameDelta;
    motion->rot.z += motion->rotVel.z * gFrameDelta;
    return 1;
}

/* Draw callback: builds the actor's model matrix from its angle, scale and position. */
s32 effect_buildTumblingPieceMtx(Mtx *mtx, EffectActor *actor) {
    TumbleMotion *motion = &actor->data.piece.motion;
    MtxF mf;

    math_mtxRotateRPYF(mf, motion->rot.x, motion->rot.y, motion->rot.z);
    mf[3][0] = actor->tmpl.position.x;
    mf[3][1] = actor->tmpl.position.y;
    mf[3][2] = actor->tmpl.position.z;
    mf[0][0] *= motion->scale;
    mf[0][1] *= motion->scale;
    mf[0][2] *= motion->scale;
    mf[1][0] *= motion->scale;
    mf[1][1] *= motion->scale;
    mf[1][2] *= motion->scale;
    mf[2][0] *= motion->scale;
    mf[2][1] *= motion->scale;
    mf[2][2] *= motion->scale;
    guMtxF2L(mf, mtx);
    return 1;
}
