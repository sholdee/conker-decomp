#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-type update routines for world objects (WorldObject): moving platforms, a seesaw and a
 * scrolling texture. flags bit 2 is set while triggered; per-type state lives in work (Work142A70).
 */

/*
 * Behaviour scratch state (WorldObject.touch) of the objects in this file. The param word packs each
 * behaviour's settings (see each function).
 */
typedef union {
    struct {
        s32 homeY;
        s32 timer; // > 0 moving out or waiting at target, < 0 cooling down at home
    } liftY;
    struct {
        s32 homeXY;     // home x << 16 | home y
        s32 homeZState; // home z | s8 state << 16 | u8 progress << 24
        s32 sound;      // sound id | playing sound handle << 16
    } liftXYZ;
    struct {
        f32 weightX;
        f32 weightZ;
    } seesaw;
} Work142A70;

#define WORK(obj) ((Work142A70 *)&(obj)->touch)

/* Moves the object vertically to a target height while triggered, waits, then returns home.
 * param: s16 target y | u8 steps << 16 | u8 (wait frames - 1) << 24. */
void prop_moveToHeight(WorldObject *obj) {
    s32 steps;
    s32 waitFrames;
    s32 target;
    s32 triggered;
    s32 moving;
    s32 startY;
    s32 step;
    s16 next;

    steps = (u8)(obj->moveData.word >> 16);
    waitFrames = (u8)(obj->moveData.word >> 24) + 1;
    target = (s16)obj->moveData.word;
    triggered = FALSE;
    moving = FALSE;
    startY = obj->y;
    if ((obj->object_status & 4) == 4) {
        triggered = TRUE;
    }
    if (WORK(obj)->liftY.homeY == 0) {
        WORK(obj)->liftY.homeY = obj->y;
    }
    step = (target - WORK(obj)->liftY.homeY) / steps;
    if (WORK(obj)->liftY.timer == 0) {
        if (obj->y == WORK(obj)->liftY.homeY) {
            if (triggered) {
                WORK(obj)->liftY.timer = waitFrames;
            }
        } else {
            next = obj->y - step;
            if (WORK(obj)->liftY.homeY < target) {
                if (WORK(obj)->liftY.homeY < next) {
                    moving = TRUE;
                }
            } else if (next < WORK(obj)->liftY.homeY) {
                moving = TRUE;
            }
            if (moving) {
                obj->y = next;
            } else {
                obj->y = WORK(obj)->liftY.homeY;
                WORK(obj)->liftY.timer = -waitFrames;
            }
        }
    }
    if (WORK(obj)->liftY.timer > 0) {
        next = obj->y + step;
        if (WORK(obj)->liftY.homeY < target) {
            if (next < target) {
                moving = TRUE;
            }
        } else if (target < next) {
            moving = TRUE;
        }
        if (moving) {
            obj->y = next;
            WORK(obj)->liftY.timer = waitFrames;
        } else {
            obj->y = target;
            WORK(obj)->liftY.timer--;
        }
    }
    if (WORK(obj)->liftY.timer < 0) {
        WORK(obj)->liftY.timer++;
    }
    obj->moveY = obj->y - startY;
}

/*
 * Moves the object by a packed offset over `frames` frames while triggered, waits, then returns
 * home, playing a looping sound while moving and a stop sound (0x83) when it arrives.
 * param: s8 dx | s8 dz << 8 | u8 frames << 16 | s8 dy << 24, offsets in units of 64.
 */
void prop_moveByOffset(WorldObject *obj) {
    u8 frames;
    s16 dx;
    s16 targetX;
    s16 dy;
    s32 dz; // matching: s32 with an s16 cast; an s16 local allocates differently
    s16 targetY;
    s16 targetZ;
    s32 startX;
    s32 startY;
    s32 startZ;
    s16 homeX;
    s16 homeY;
    s16 homeZ;
    s16 unused; // matching: stack slot of a dropped local
    s16 state;
    s32 handle;
    s32 progress;
    s32 triggered;
    u16 soundId;

    frames = obj->moveData.word >> 16;
    dx = (s8)obj->moveData.word << 6;
    dy = (s8)(obj->moveData.word >> 24) << 6;
    dz = (s16)((s8)(obj->moveData.word >> 8) << 6);
    triggered = FALSE;
    soundId = WORK(obj)->liftXYZ.sound;
    startX = obj->x;
    startY = obj->y;
    startZ = obj->z;
    handle = (u16)(WORK(obj)->liftXYZ.sound >> 16);
    if ((obj->object_status & 4) == 4) {
        triggered = TRUE;
    }
    if (WORK(obj)->liftXYZ.homeXY == 0) {
        WORK(obj)->liftXYZ.homeXY = (obj->x << 16) | (obj->y & 0xFFFF);
        WORK(obj)->liftXYZ.homeZState = obj->z & 0xFFFF;
    }
    homeX = WORK(obj)->liftXYZ.homeXY >> 16;
    homeY = WORK(obj)->liftXYZ.homeXY;
    targetX = homeX + dx;
    homeZ = WORK(obj)->liftXYZ.homeZState;
    state = (s8)(WORK(obj)->liftXYZ.homeZState >> 16);
    progress = (u8)(WORK(obj)->liftXYZ.homeZState >> 24);
    targetY = homeY + dy;
    targetZ = homeZ + dz;
    if (state == 0) {
        if (homeX == obj->x && homeY == obj->y && homeZ == obj->z) {
            if (triggered) {
                state = 100;
            }
        } else {
            progress -= (u8)(255.0f / frames);
            if (progress <= 0) {
                progress = 0;
                state = -100;
                if (handle) {
                    sound_stop(handle);
                    handle = 0;
                    sound_playAt(0x83, 0x5000, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
                }
            } else if (soundId && handle == 0) {
                handle = sound_startVoiceAt(handle, soundId, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000) & 0xFFFF; // matching: masks the u16 again
            }
            obj->x = homeX + (targetX - homeX) * (progress * (1.0f / 255.0f));
            obj->y = homeY + (targetY - homeY) * (progress * (1.0f / 255.0f));
            obj->z = homeZ + (targetZ - homeZ) * (progress * (1.0f / 255.0f));
        }
    }
    if (state > 0) {
        if (progress < 255) {
            progress += (u8)(255.0f / frames);
            state = 100;
            if (soundId && handle == 0) {
                handle = sound_startVoiceAt(handle, soundId, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000) & 0xFFFF; // matching: masks the u16 again
            }
            if (progress > 255) {
                progress = 255;
            }
        } else {
            state--;
            progress = 255;
            if (handle) {
                sound_stop(handle);
                handle = 0;
                sound_playAt(0x83, 0x5000, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
            }
        }
        obj->x = homeX + (targetX - homeX) * (progress * (1.0f / 255.0f));
        obj->y = homeY + (targetY - homeY) * (progress * (1.0f / 255.0f));
        obj->z = homeZ + (targetZ - homeZ) * (progress * (1.0f / 255.0f));
    }
    if (state < 0) {
        state++;
    }
    WORK(obj)->liftXYZ.homeZState &= 0xFFFF;
    WORK(obj)->liftXYZ.homeZState |= ((state & 0xFF) << 16) | ((progress & 0xFF) << 24);
    obj->moveX = obj->x - startX;
    obj->moveY = obj->y - startY;
    obj->moveZ = obj->z - startZ;
    WORK(obj)->liftXYZ.sound = (handle << 16) | soundId;
}

/* While triggered, adds the player's x/z offset from the object, rotated into its frame, to the weights.
 * param: s8 weight divisor | s8 max tilt << 8 | s8 bias << 16 | s8 min tilt << 24. */
void prop_addSeesawWeight(WorldObject *obj, Actor *player) {
    f32 sinYaw;
    f32 cosYaw;
    f32 dx;
    f32 dz;

    sinYaw = math_cos(obj->u * 0.01745329238f);
    cosYaw = math_sin(obj->u * 0.01745329238f);
    dx = player->xpos - obj->x;
    dz = player->zpos - obj->z;
    if ((obj->object_status & 4) == 4) {
        WORK(obj)->seesaw.weightX += (cosYaw * dx + sinYaw * dz);
        WORK(obj)->seesaw.weightZ -= (cosYaw * dz + sinYaw * dx);
    }
}

/* prop_addSeesawWeight, with the weight gained this frame multiplied by 5 when player->unk84 is 0x4B. */
void func_15115EDC(WorldObject *obj, Actor *player) {
    f32 oldX;
    f32 oldZ;

    oldX = WORK(obj)->seesaw.weightX;
    oldZ = WORK(obj)->seesaw.weightZ;
    prop_addSeesawWeight(obj, player);
    if (player->animseq.uh == 0x4B) {
        // matching: the old values are reused to hold this frame's extra gain
        oldX = (WORK(obj)->seesaw.weightX - oldX) * 4.0f;
        oldZ = (WORK(obj)->seesaw.weightZ - oldZ) * 4.0f;
        WORK(obj)->seesaw.weightX += oldX;
        WORK(obj)->seesaw.weightZ += oldZ;
    }
}

/* Tilts the seesaw by its accumulated weight (or a constant bias), clamps it, and clears the weights. */
void prop_tiltSeesaw(WorldObject *obj) {
    s8 divisor;
    s8 bias;
    s8 maxTilt;
    s8 minTilt;

    // matching: two assignments per line steer as1's -g3 scheduling
    divisor = obj->moveData.word; bias = obj->moveData.word >> 16;
    maxTilt = obj->moveData.word >> 8; minTilt = obj->moveData.word >> 24;
    if (divisor) {
        obj->t += (WORK(obj)->seesaw.weightX / divisor) + bias;
    } else {
        obj->t += (bias * D_800BE9E4);
    }
    if (maxTilt < obj->t) {
        obj->t = maxTilt;
    } else if (obj->t < minTilt) {
        obj->t = minTilt;
    }
    WORK(obj)->seesaw.weightZ = 0.0f;
    WORK(obj)->seesaw.weightX = 0.0f;
}

/* Scrolls the texture coordinates of every vertex by (ds, dt) (param: s16 ds << 16 | s16 dt), from last
 * frame's buffer into this one. */
void prop_scrollTexture(WorldObject *obj) {
    s16 ds;
    s16 dt;
    s32 i;

    // matching: IDO only emits this register order for the explicit sign-extension shifts
    ds = (((obj->moveData.word >> 16) & 0xFFFF) << 16) >> 16;
    dt = ((obj->moveData.word & 0xFFFF) << 16) >> 16;
    for (i = 0; i < obj->numVertices; i++) {
        obj->vertices[gDrawBufferIndex][i].v.tc[0] = obj->vertices[!gDrawBufferIndex][i].v.tc[0] + ds;
        obj->vertices[gDrawBufferIndex][i].v.tc[1] = obj->vertices[!gDrawBufferIndex][i].v.tc[1] + dt;
    }
}
