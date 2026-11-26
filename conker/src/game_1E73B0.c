#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Spawns the effect tied to `owner` and records the owner in the effect's extra data. */
void *func_151B9F00(Actor *owner, u8 arg1, s32 arg2) {
    DynModel *effect;
    DynModelDesc desc;
    ActorRef ref;

    if (owner == NULL) {
        return NULL;
    }
    ref.actor = owner;
    ref.uniqueId = owner->unique_id;
    desc.flags = 2;
    desc.updateFunc = -1;
    desc.drawFunc = 3;
    desc.lifetime = 300;
    desc.unk30 = 9;
    desc.unk34 = 0x1AD;
    desc.viewportMask = 0xFF;
    {
        s32 objIndex = owner - movement_obj;

        if ((objIndex < 2) && (D_800E0BCE[objIndex] != 0)) {
            if (D_8008FD90 == 2) {
                desc.viewportMask = 0xFD;
            } else if (D_8008FD90 == 4) {
                if (objIndex == 0) {
                    desc.viewportMask = 0xFB;
                } else {
                    desc.viewportMask = 0xF7;
                }
            }
        }
    }
    desc.unk8 = 1;
    desc.unkC = 0x200205;
    desc.unk10 = 0x60600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.kind = 2;
    effect = func_1513B5E0(&desc, 1, 8, arg1, arg2);
    if (effect != NULL) {
        memcpy(DYNMODEL_USERDATA(effect), &ref, sizeof(ref));
    }
    return effect;
}


/*
 * Per-frame update of an effect that follows its owner's model: builds the effect's matrix from the
 * owner's and sets each vertex's texture coordinates from its projected position in `viewport`.
 * Removes the effect when the owner is gone; returns 0 when it should not be drawn.
 */
s32 func_151BA084(DynModel *effect, s16 viewport) {
    ActorRef *ref;
    Actor *owner;
    u8 *model;
    MtxF modelMtx;
    Actor *player;
    MtxF viewMtx;
    s32 i;
    f32 originS;
    f32 origin[3];
    f32 originWorld[3];
    f32 originT;
    f32 x;
    f32 y;
    f32 z;
    f32 world[3];
    f32 proj[3];

    ref = DYNMODEL_USERDATA(effect);
    owner = ref->actor;
    if (owner->type == 0 || owner->unique_id != ref->uniqueId) {
        func_1516972C(&effect->header);
    }
    model = (u8 *)owner->ob_matrix_ptr;
    if (model == NULL) {
        return 0;
    }
    if ((owner->unk74 & 0xF) == 0xF) {
        return 0;
    }
    if (D_800BE616 != 0) {
        if (owner->CameraPtr != NULL && owner->mv != NULL &&
            owner->mv->SniperState != 0 && viewport == owner->CameraPtr->CNumber) {
            return 0;
        }
    } else {
        player = &movement_obj[0];
        if (player->CameraPtr != NULL && player->mv != NULL &&
            player->mv->SniperState != 0 && viewport == player->CameraPtr->CNumber) {
            return 0;
        }
    }
    // matching: the two unused labels stop IDO from hoisting the shared call arguments out of the branches
    if (D_800C3E90 != 0) {
    lbl2:
        guMtxL2F(modelMtx, (Mtx *)(model + 0x40));
    } else {
    lbl0:
        memcpy(modelMtx, model + 0x40, sizeof(modelMtx));
    }
    modelMtx[0][3] = 0.0f;
    modelMtx[1][3] = 0.0f;
    modelMtx[2][3] = 0.0f;
    modelMtx[3][3] = 1.0f;
    guMtxF2L(modelMtx, &effect->mtx[D_800BE9C0]);
    memcpy(viewMtx, &D_800D9D10[viewport], sizeof(viewMtx));
    viewMtx[0][3] = 0.0f;
    viewMtx[1][3] = 0.0f;
    viewMtx[2][3] = 0.0f;
    viewMtx[3][3] = 1.0f;
label: // matching: unused label keeps the viewMtx stores ahead of the argument setup
    func_150A7960(modelMtx, 0.0f, 67.0f, 159.0f, &originWorld[0], &originWorld[1], &originWorld[2]);
    func_150A7960(viewMtx, originWorld[0], originWorld[1], originWorld[2], &origin[0], &origin[1], &origin[2]);
    origin[0] *= 2.002f;
    origin[1] *= 2.002f;
    /* keep texture coordinates near zero by removing whole 1024-unit tiles */
    originS = (s32)(origin[0] / 1024.0f) * 1024.0f;
    originT = (s32)(origin[1] / 1024.0f) * 1024.0f;
    {
        s32 pad; // matching: -g3 block marker keeps the origin stores after the tile math

        for (i = 0; i < 20; i++) {
            Vtx_t *vtx = &effect->vertices[D_800BE9C0][viewport][i].v;

            x = vtx->ob[0];
            y = vtx->ob[1];
            z = vtx->ob[2];
            func_150A7960(modelMtx, x, y, z, &world[0], &world[1], &world[2]);
            func_150A7960(viewMtx, world[0], world[1], world[2], &proj[0], &proj[1], &proj[2]);
            proj[0] *= 2.002f;
            proj[1] *= 2.002f;
            vtx->tc[0] = proj[0] - originS;
            vtx->tc[1] = proj[1] - originT;
        }
    }
    return 1;
}

/* Message handler: removes the effect with its owner, and follows the owner when two objects swap. */
void func_151BA468(DynModel *effect, ActorMsg *msg, u8 msgType) {
    ActorRef *ref = DYNMODEL_USERDATA(effect);
    Actor *owner = ref->actor;

    if (msgType == 0) {
        if ((owner == msg->ref.actor) || (msg->ref.uniqueId == owner->unique_id)) {
            func_1516972C(&effect->header);
        }
    } else if (msgType == 0x2D) {
        if (ref->actor == msg->swap.actorA) {
            ref->actor = msg->swap.actorB;
            ref->uniqueId = msg->swap.idB;
        } else if (ref->actor == msg->swap.actorB) {
            ref->actor = msg->swap.actorA;
            ref->uniqueId = msg->swap.idA;
        }
    }
}

extern Vec3f D_800AA590, D_800AA59C, D_800AA5A8, D_800AA5B4[], D_800AA5CC[], D_800AA5FC[];
extern f32 D_800AA62C[];
extern Vec3f D_800AA63C, D_800AA648[], D_800AA660[], D_800AA678[], D_800AA6A8[], D_800AA6D8[];
extern f32 D_800AA708[];

/*
 * Spawns a burst effect at `obj` plus up to twelve particles launched along fixed paths in the space of
 * the object's model matrix (several of them picked at random).
 */
void func_151BA518(Actor *obj, u8 arg1, s32 arg2) {
    JetSpraySpec burst;
    CollisionHit floor;
    Vec3f objPos;
    ParticleSpawn80 part;
    f32 outA2, outA1, outA0, outB2, outB1, outB0;
    Mtx *mtx;
    s32 i, j, k;
    s32 shift;
    f32 speed, scale, accel;

    burst.angles.yawBase = 0;
    burst.angles.yawRange = 0xFF;
    burst.angles.pitchBase = -0x3F;
    burst.angles.pitchRange = 0x34;
    burst.spray.countBase = 6;
    if (D_80082FA0 >= 2) {
        shift = 1;
    } else {
        shift = 0;
    }
    burst.spray.countRange = 5 >> shift;
    burst.spray.pos.x = obj->xpos;
    burst.spray.pos.y = obj->ypos + 25.0f;
    burst.spray.pos.z = obj->zpos;
    burst.spray.particleCountBase = 6;
    burst.spray.speedBase = 50.0f;
    burst.spray.speedRange = 42.0f;
    burst.spray.sizeBase = 98.0f;
    burst.spray.sizeRange = 68.0f;
    burst.spray.sizeSlopeBase = 609.0f;
    burst.spray.sizeSlopeRange = 271.0f;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        shift = 1;
    } else {
        shift = 0;
    }
    burst.spray.particleCountRange = 3 >> shift;
    burst.spray.delayBase = 25;
    burst.spray.delayRange = 15;
    burst.spray.alphaBase = 100;
    burst.spray.alphaRange = 100;
    burst.spray.fadeStart = 12;
    burst.spray.fadeOutStart = 20;
    burst.spray.spread = 18.0f;
    burst.spray.drag = 0.7690000534f;
    burst.spray.gravityBase = -0.1690000147f;
    burst.spray.gravityRange = 0;
    if ((D_80082FA0 >= 2) || (D_8008FD8C >= 8)) {
        burst.spray.eventFunc = -1;
    } else {
        burst.spray.eventFunc = 0;
    }
    func_1514FCE8(&burst, arg1, arg2);
    if ((obj->ob_matrix_ptr != NULL) && ((obj->unk74 & 0xF) != 0xF)) {
        func_1504715C(&floor, obj);
        mtx = (Mtx *)((u8 *)obj->ob_matrix_ptr + 0x40);
        {
            typedef s32 T151; // matching: a declaration makes -g3 open a scope here, which steers the scheduler
            func_1503F404(mtx, &outA0, &outA1, &outA2, &objPos.x, &objPos.y, &objPos.z, &outB0, &outB1, &outB2);
        }
        {
            typedef s32 T152; // matching: as above
            part.radius = 1.0f;
            part.scale.x = 1.0f;
            part.scale.y = 1.0f;
            part.scale.z = 1.0f;
            part.elasticity = 0.7f;
            part.rotVel.y = 0.0f;
            part.flags = 0x39E9;
            part.variant = 0;
            part.unk5C = 0;
            part.alpha = 0xFF;
            part.updateFn = 0xD;
            part.update2Fn = 0;
            part.floorHitFn = 0xC;
            part.floorType3Fn = 0;
            part.wallHitFn = 0;
            part.destroyFn = 0;
            part.squashFn = 0;
            part.matrixFn = 2;
            part.unk6A = 0;
            part.owner = obj;
            part.ownerId = obj->unique_id;
            part.fadeStart = 0xC;
            part.fadeRate = 0x15;
        }
        scale = 0.001f;
        accel = -488.0f * scale;

        speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
        part.lifetime = (func_150ADA20() % 36U) + 30;
        part.model = 0x27;
        part.sizeXZ = 0.75f;
        part.sizeY = 0.75f;
        func_150ADA68();
        part.gravity = accel;
        func_15143134(&D_800AA590, &part.pos, mtx);
        func_15143134(&D_800AA59C, &part.vel, mtx);
        part.vel.x = part.vel.x - part.pos.x;
        part.vel.y = part.vel.y - part.pos.y;
        part.vel.z = part.vel.z - part.pos.z;
        part.vel.x = part.vel.x * speed;
        part.vel.y = part.vel.y * speed;
        part.vel.z = part.vel.z * speed;
        part.rot = objPos;
        part.rot.x += 109.0f;
        part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);

        speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
        part.lifetime = (func_150ADA20() % 36U) + 30;
        part.model = 0x28;
        part.sizeXZ = 0.5f;
        part.sizeY = 0.5f;
        func_150ADA68();
        part.gravity = accel;
        func_15143134(&D_800AA5A8, &part.pos, mtx);
        func_15143134(&D_800AA5B4[func_150ADA20() & 1], &part.vel, mtx);
        part.vel.x = part.vel.x - part.pos.x;
        part.vel.y = part.vel.y - part.pos.y;
        part.vel.z = part.vel.z - part.pos.z;
        part.vel.x = part.vel.x * speed;
        part.vel.y = part.vel.y * speed;
        part.vel.z = part.vel.z * speed;
        part.rot = objPos;
        part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
        func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);

        part.model = 0x29;
        for (i = 0; i != 4; i++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                part.sizeXZ = part.sizeY = D_800AA62C[i];
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA5CC[i], &part.pos, mtx);
                func_15143134(&D_800AA5FC[i], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }

        part.model = 0x2B;
        func_15143134(&D_800AA63C, &part.pos, mtx);
        part.sizeXZ = part.sizeY = 0.551f;
        for (j = 0; j != 2; j++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA648[j], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += D_800AA660[j].x;
                part.rot.y += D_800AA660[j].y;
                part.rot.z += D_800AA660[j].z;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }

        part.model = 0x2C;
        for (k = 0; k != 4; k++) {
            if (func_150ADA20() & 1) {
                speed = ((func_150ADA68() * 200.0f) + 83.0f) * scale;
                part.lifetime = (func_150ADA20() % 36U) + 30;
                part.sizeXZ = part.sizeY = D_800AA708[k];
                func_150ADA68();
                part.gravity = accel;
                func_15143134(&D_800AA678[k], &part.pos, mtx);
                func_15143134(&D_800AA6A8[k], &part.vel, mtx);
                part.vel.x = part.vel.x - part.pos.x;
                part.vel.y = part.vel.y - part.pos.y;
                part.vel.z = part.vel.z - part.pos.z;
                part.vel.x = part.vel.x * speed;
                part.vel.y = part.vel.y * speed;
                part.vel.z = part.vel.z * speed;
                part.rot = objPos;
                part.rot.x += D_800AA6D8[k].x;
                part.rot.y += D_800AA6D8[k].y;
                part.rot.z += D_800AA6D8[k].z;
                part.rotVel.x = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                part.rotVel.z = ((func_150ADA68() * 28864.0f) + -14432.0f) * scale;
                func_1513264C(&part, 3, 0xFF, &floor, 0, arg1, arg2);
            }
        }
    }
}


/* Starts sound 0x4A7 at `obj`; func_151BB0E0 keeps it updated. */
void func_151BB044(Actor *obj) {
    func_1000FA64(0x4A7, obj->xpos, obj->ypos, obj->zpos, 0x2000, 0x320, 0xC8,
                  (SoundRecordCallback)func_151BB0E0, obj, 0, 8, 0);
}

/* A playing positional sound, as seen by its update callback. */
struct SoundChannel {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 flags;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ Actor *obj;
    /* 0x1C */ u32 userData; /* bits 0-6 state, 7 fading in, 8-15 fade volume, 16-27 timer */
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ u16 unk24;
};

/*
 * Update callback of the sounds started by func_151BB044: follows the object, switches between the
 * 0x4A7/0x4A8/0x4A9 sounds by how it moves, and fades the volume. Returns 1 when the sound should stop.
 */
s32 func_151BB0E0(SoundChannel *chan, s32 arg1, u32 *volume, s32 arg3, s32 *speedOut,
                  s32 arg5, u16 *soundId) {
    Actor *obj;
    s32 state;
    s32 prevState;
    s32 sound;
    s32 param;
    s32 fade;
    s32 timer;
    s32 fadingIn;

    obj = chan->obj;
    if ((obj != NULL) && (obj->energy != 0)) {
        state = 0;
        sound = 0;
        prevState = chan->userData & 0x7F;
        fade = (chan->userData >> 8) & 0xFF;
        timer = (chan->userData >> 16) & 0xFFF;
        fadingIn = (chan->userData >> 7) & 1;
        chan->x = obj->xpos;
        chan->y = obj->ypos;
        chan->z = obj->zpos;
        if (((obj->speed > 1.0f) && (obj->stickspeed > 0.0f)) ||
            ((obj->speed < -1.0f) && (obj->stickspeed < 0.0f))) {
            state = 1;
        } else if (((obj->speed > 5.0f) && (obj->stickspeed <= 0.0f)) ||
                   ((obj->speed < -5.0f) && (obj->stickspeed >= 0.0f))) {
            state = 2;
        }
        // matching: emits no code; the extra reads of state raise its uopt register priority
        // above sound's, which puts state in v1 and sound in t0
        if (state + state + state + state) {}
        if ((state ^ prevState) & 0x7F) {
            if ((((state ^ prevState) & 0x7F) & 2) && (state & 2)) {
                sound = 0x4A5;
            } else {
                sound = func_1000F568(0x4A3, 4);
            }
            if (!func_10010894(obj)) {
                func_10010154(sound, obj, 0x6D60, 300, 1800);
            }
            sound = 0;
        }
        if ((state != 0) && (*soundId == 0x4A7)) {
            sound = 0x4A8;
            param = 0x2EE0;
        } else if ((state == 0) && ((*soundId == 0x4A9) || (*soundId == 0x4A8))) {
            if (*soundId == 0x4A9) {
                return 1;
            }
            sound = 0x4A7;
            param = 0x2000;
        }
        if (sound != 0) {
            func_1000FA64(sound, obj->xpos, obj->ypos, obj->zpos, param, 0x320, 0xC8,
                          (SoundRecordCallback)func_151BB0E0, obj, state | 0x12CFF00, 8, 0);
            goto end; // matching: a plain return 1 here is scheduled differently
        }
        if (*soundId != 0x4A7) {
            if (timer == 0) {
                *volume = (*volume * fade) >> 8;
                if (fadingIn) {
                    fade += 8;
                    if (fade >= 0x100) {
                        fade = 0xFF;
                        timer = (func_150ADA20() & 0x200) + 300;
                        fadingIn = FALSE;
                    }
                } else {
                    fade -= 8;
                    if (fade <= 0) {
                        return 1;
                    }
                }
            } else {
                timer -= D_800BE9E4;
                if (timer <= 0) {
                    timer = 0;
                    if (*soundId == 0x4A8) {
                        func_1000FA64(0x4A9, obj->xpos, obj->ypos, obj->zpos, 0x2EE0, 0x320,
                                      0xC8, (SoundRecordCallback)func_151BB0E0, obj, state | 0x880, 8, 0);
                        timer = (func_150ADA20() & 0x200) + 900;
                    }
                }
            }
            *speedOut = fabsf(obj->speed) * obj->vtilt * 0.5f;
        }
        chan->userData = (fadingIn << 7) | state | (fade << 8) | (timer << 16);
        if ((*volume == 0) || (chan->flags & 0x80)) {
            if (chan->unk24 != 0) {
                func_100111C8(chan->unk24);
                chan->unk24 = 0;
            }
            *soundId = 0;
            chan->flags &= ~0x80;
            *volume = 0;
        }
        return 0;
    }
end:
    return 1;
}

/* wrapped in structs so the tables are copied by struct assignment */
typedef struct { s32 v[9]; } S32x9;
typedef struct { f32 v[9]; } F32x9;
extern S32x9 D_800AA718;
extern F32x9 D_800AA73C;

/*
 * Impact effect for a shot from `from` towards `to`: effects at the origin, then a ray cast along the
 * shot and, where it hits a surface, impact effects there.
 */
void func_151BB61C(Actor *obj, Vec3f *from, Vec3f *to, u8 arg3, s32 arg4) {
    CollisionTri surf;
    Vec3f hit;
    Vec3f delta;
    s32 hitInfo;
    Vec3f edgeA;
    Vec3f edgeB;
    Vec3f normal;
    SurfaceJetSpray fx;

    if (obj != NULL) {
        func_15055A2C(0, from->x, from->y, from->z, 0);
        delta.x = to->x - from->x;
        delta.y = to->y - from->y;
        delta.z = to->z - from->z;
        func_151C05A4(from, arg3, arg4);
        func_151C05F0(from, arg3, arg4);
        func_151C04F8(from, arg3, arg4);
        func_151BBEE4(from, arg3, arg4);
        if (func_150AC9C0(from->x, from->y, from->z, delta.x, delta.y, delta.z, 0, &surf, &hit.x, &hit.y, &hit.z, NULL,
                          &hitInfo, 0, 0.0f) != 0) {
            if (func_15145C90(hitInfo) != 0) {
                func_151BBFBC(&surf, &hit, arg3 & 0xFF, arg4);
            }
            if (func_15144E80(&surf, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
            fx.source.normal.x = -delta.x;
            fx.source.normal.y = -delta.y;
            fx.source.normal.z = -delta.z;
            fx.source.triangle = surf;
            fx.source.spread = 494.0f;
            fx.spray.pos = hit;
            fx.spray.countBase = 3;
            fx.spray.countRange = 4;
            fx.spray.speedBase = 0.0609f;
            fx.spray.speedRange = 0.0252f;
            fx.spray.sizeBase = 45.0f;
            fx.spray.sizeRange = 53.0f;
            fx.spray.sizeSlopeBase = 203.0f;
            fx.spray.sizeSlopeRange = 414.0f;
            fx.spray.particleCountBase = 7;
            fx.spray.particleCountRange = 3;
            fx.spray.spread = 15.0f;
            fx.spray.drag = 0.8800000548f;
            fx.spray.gravityBase = -0.3980000317f;
            fx.spray.gravityRange = 0.3910000324f;
            fx.spray.delayBase = 25;
            fx.spray.delayRange = 15;
            fx.spray.alphaBase = 100;
            fx.spray.alphaRange = 100;
            fx.spray.fadeStart = 12;
            fx.spray.fadeOutStart = 20;
            fx.spray.eventFunc = 0;
            func_1514FBFC(&fx, arg3, arg4);
            {
                S32x9 colors;
                F32x9 scales;
                SurfaceDebrisBurst desc;
                colors = D_800AA718;
                scales = D_800AA73C;
                desc.countBase = 6;
                desc.countRange = 6;
                desc.pos = hit;
                desc.source.normal.x = -delta.x;
                desc.source.normal.y = -delta.y;
                desc.source.normal.z = -delta.z;
                desc.source.triangle = surf;
                desc.source.spread = 456.0f;
                desc.speedBase = 0.02000000142f;
                desc.speedRange = 0.02000000142f;
                desc.gravityBase = -1.71f;
                desc.gravityRange = 0.448f;
                desc.lifeBase = 50;
                desc.lifeRange = 20;
                desc.scaleBase = 0.499f;
                desc.scaleRange = 0.7010000348f;
                desc.spin = 21.502f;
                func_15151D6C(&desc, colors.v, scales.v, 9, arg3, arg4);
            }
        }
    }
}

extern Vec3f D_8008FBA0;
extern Vec3f D_8008FBAC;

/*
 * Impact effect halfway between `a` and `b`: a burst there, a ground impact on the floor below it,
 * and a shower of debris.
 */
void func_151BBA9C(Actor *obj, f32 *a, f32 *b, u8 arg3, s32 arg4) {
    Vec3f center;
    JetSpraySpec burst;
    CollisionHit floor;
    Vec3f probe;
    Vec3f hit;
    Vec3f edgeA;
    Vec3f edgeB;
    Vec3f normal;

    if (obj != NULL) {
        center.x = (b[0] + a[0]) * 0.5f;
        center.y = (b[1] + a[1]) * 0.5f;
        center.z = (b[2] + a[2]) * 0.5f;
        func_151C04F8(&center, arg3, arg4);
        func_151C05A4(&center, arg3, arg4);
        func_151C05F0(&center, arg3, arg4);
        func_15055A2C(1, center.x, center.y, center.z, 0);
        func_151BBEE4(&center, arg3, arg4);
        burst.angles.yawBase = 0;
        burst.angles.yawRange = 0xFF;
        burst.angles.pitchBase = -0x40;
        burst.angles.pitchRange = 0x47;
        burst.spray.countBase = 6;
        burst.spray.countRange = 4 >> ((D_80082FA0 >= 2) ? 1 : 0);
        burst.spray.pos = center;
        burst.spray.particleCountBase = 7;
        burst.spray.speedBase = 23.0f;
        burst.spray.speedRange = 30.0f;
        burst.spray.sizeBase = 45.0f;
        burst.spray.sizeRange = 53.0f;
        burst.spray.sizeSlopeBase = 203.0f;
        burst.spray.sizeSlopeRange = 414.0f;
        burst.spray.particleCountRange = 3 >> ((D_80082FA0 >= 2 || D_8008FD8C >= 8) ? 1 : 0);
        burst.spray.delayBase = 25;
        burst.spray.delayRange = 15;
        burst.spray.alphaBase = 100;
        burst.spray.alphaRange = 100;
        burst.spray.fadeStart = 12;
        burst.spray.fadeOutStart = 20;
        burst.spray.spread = 15.0f;
        burst.spray.drag = 0.8720000386f;
        burst.spray.gravityBase = -0.3980000317f;
        burst.spray.gravityRange = 0.3640000224f;
        if (D_80082FA0 >= 2 || D_8008FD8C >= 8) {
            burst.spray.eventFunc = -1;
        } else {
            burst.spray.eventFunc = 0;
        }
        func_1514FCE8(&burst, arg3, arg4);
        func_1504715C(&floor, obj);
        probe.x = center.x;
        probe.y = center.y + 100.0f;
        probe.z = center.z;
        if (func_15046C80(&probe, 0, center.y - 1000.0f, &floor) != 0) {
            hit.x = probe.x;
            hit.y = floor.height;
            hit.z = probe.z;
            func_151BBFBC(&floor.tri, &hit, arg3, arg4);
            if (func_15144E80(&floor.tri, &edgeA, &edgeB, &normal) != 0 && normal.y < 0.0f) {
                func_151C0644(&hit, arg3, arg4);
            }
        }
        {
            DebrisBurst debris;
            Vec3f first = D_8008FBA0;
            Vec3f second = D_8008FBAC;

            debris.countBase = 4;
            debris.countRange = 8;
            debris.pos = center;
            debris.speedBase = 12.0f;
            debris.speedRange = 6.0f;
            debris.angles.yawBase = 0;
            debris.angles.yawRange = 0xFF;
            debris.angles.pitchBase = -0x3F;
            debris.angles.pitchRange = 0x57;
            debris.lifeBase = 50;
            debris.lifeRange = 10;
            debris.gravityBase = -1.438000083f;
            debris.gravityRange = 0.4490000308f;
            debris.scaleBase = 0.2480000108f;
            debris.scaleRange = 0.09900000691f;
            debris.spin = 22.85900116f;
            func_15152190(&debris, &first, &second, 3, 0.0f, 1, arg3, arg4);
        }
    }
}

/* Spawns a short-lived effect at `pos` with a random size and lifetime. */
void func_151BBEE4(Vec3f *pos, u8 arg1, s32 arg2) {
    LightEffectHeader header;
    Vec3i header2;

    header.flags = 3;
    header.update = -1;
    header.lifetime = (func_150ADA20() & 0xF) + 5;
    header.lightType = 0;

    header2.x = pos->x;
    header2.y = pos->y;
    header2.z = pos->z;

    func_151602C0(&header, &header2, (func_150ADA20() % 21U) + 30, 0xFF, 0x61, 0x36, 0xFF, 0, 0, arg1, arg2);
}

// matching: this file was compiled with s32 parameters where the definition (game_113D60.c) has u8 and s16,
// so its calls do not narrow them. The definition is (f32 size, u8 arg1, s32 arg2, Vec3f *position,
// s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 owner, s32 kind).
void func_150E7FEC(f32, s32, CollisionTri *, Vec3f *, s32, s32, s32, s32, s32, s32, s32, s32);
/* Spawns a randomised func_150E7FEC effect on surface `surf` at `pos`. */
void func_151BBFBC(CollisionTri *surf, Vec3f *pos, s32 arg2, s32 arg3) {
    s32 pad; // matching: unused stack slot
    u32 rnd1;
    f32 rnd0;
    u32 rnd2;

    rnd0 = func_150ADA68();
    rnd1 = func_150ADA20();
    rnd2 = func_150ADA20();
    func_150E7FEC(rnd0 * 75.0f + 75.0f, ((rnd1 % 56) + 200) & 0xFF, surf, pos,
                  (rnd2 % 205) + 299, 1, 1, 0, 0, 0, (u8)arg2, 0);
}

extern u8 D_800AA760;
extern u8 D_800AA76C;

/* Spawns the func_15160CDC effect (tables D_800AA760/D_800AA76C) on `obj`. */
void func_151BC074(Actor *obj) {
    if (obj != NULL) {
        func_15160CDC(obj, 1, &D_800AA760, &D_800AA76C, 0.01769899949f, 2, 300,
                      0xFF, 0xFF, 0xFF, 0xFF, 1, 0, 0, 0xFF, 1);
    }
}


/* Objects returned by func_15134908 hold the owner reference at this offset. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ ActorRef ref;
} Fx15134908;

/* Data block of the object returned by func_15147A80. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
} Data15147A80;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ Data15147A80 *data;
} Fx15147A80;

/*
 * Spawns three effects that follow `obj` (the first only while D_80082FA0 < 2 and D_8008FD8C < 5),
 * each with a reference back to its owner.
 */
void func_151BC104(Actor *obj, u8 arg1, s32 arg2) {
    Fx15134908 *fx1;

    if ((D_80082FA0 < 2) && (D_8008FD8C < 5)) {
        BloodTrailDesc desc;
        ActorRef ref;

        desc.radius = 15.0f;
        desc.x = &obj->xpos;
        desc.y = &obj->ypos;
        desc.z = &obj->zpos;
        desc.lifetime = 300;
        desc.rate = 0.3033999801f * D_800DCA24;
        desc.flags = 4;
        desc.emitter = 4;
        desc.unk18 = 1;
        desc.callback = 0;
        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        fx1 = func_15134908(&desc, 8, arg1, arg2);
        if (fx1 != NULL) {
            memcpy(&fx1->ref, &ref, sizeof(ref));
        }
    }
    {
        EffectTemplate desc;
        Fx15147A80 *fx2;
        Data15147A80 data;

        desc.itemCount = 40;
        desc.pos.x = obj->xpos;
        desc.pos.y = obj->ypos;
        desc.pos.z = obj->zpos;
        desc.life = 300;
        desc.flags = 6;
        desc.kind = 12;
        data.owner = obj;
        data.ownerId = obj->unique_id;
        data.pos = desc.pos;
        data.unk14 = 0.0f;
        data.unk18 = 0.0f;
        data.unk20 = -16384.0f;
        data.unk1C = -16384.0f;
        fx2 = func_15147A80(&desc, 40, 20, 10, 10, 10, 0, 0, 0, arg1, arg2);
        if (fx2 != NULL) {
            memcpy(fx2->data, &data, sizeof(data));
        }
    }
    {
        ActorRef ref;
        LightEffect *fx3;
        LightEffectHeader header;

        ref.actor = obj;
        ref.uniqueId = obj->unique_id;
        header.flags = 2;
        header.update = 14;
        header.lifetime = 300;
        header.lightType = 31;
        fx3 = func_15163414(&header, &obj->xpos, &obj->ypos, &obj->zpos,
                           0, 0, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, 8, arg1, arg2);
        if (fx3 != NULL) {
            memcpy(&fx3->data.trackActor.ref, &ref, sizeof(ref));
        }
    }
}
