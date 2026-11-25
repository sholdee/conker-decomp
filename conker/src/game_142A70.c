#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-type update routines for 0xA0-byte level objects (WorldObject): moving platforms, a seesaw
 * and a scrolling texture. Shared layout: rotation f32[3] at 0x0, position s16[3] at 0x10, vertex
 * count at 0x16, vertex buffers at 0x20 (double-buffered by D_800BE9C0), packed param at 0x3C,
 * flags at 0x4F (bit 2 set while triggered), per-frame position delta s16[3] at 0x5A and
 * per-type state at 0x7C-0x87. Each routine views the object through its own struct.
 */

extern s32 func_10010E78(u16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

struct LiftY {
    u8 pad0[0x12];
    s16 y;
    u8 pad14[0x28];
    s32 param; // s16 target y | u8 steps << 16 | u8 (wait frames - 1) << 24
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xC];
    s16 moveY;
    u8 pad5E[0x1E];
    s32 homeY;
    s32 timer; // > 0 moving out or waiting at target, < 0 cooling down at home
};

/* Moves the object vertically to a target height while triggered, waits, then returns home. */
void func_151155C0(struct LiftY *obj) {
    s32 steps;
    s32 waitFrames;
    s32 target;
    s32 triggered;
    s32 moving;
    s32 startY;
    s32 step;
    s16 next;

    steps = (u8)(obj->param >> 16);
    waitFrames = (u8)(obj->param >> 24) + 1;
    target = (s16)obj->param;
    triggered = FALSE;
    moving = FALSE;
    startY = obj->y;
    if ((obj->flags & 4) == 4) {
        triggered = TRUE;
    }
    if (obj->homeY == 0) {
        obj->homeY = obj->y;
    }
    step = (target - obj->homeY) / steps;
    if (obj->timer == 0) {
        if (obj->y == obj->homeY) {
            if (triggered) {
                obj->timer = waitFrames;
            }
        } else {
            next = obj->y - step;
            if (obj->homeY < target) {
                if (obj->homeY < next) {
                    moving = TRUE;
                }
            } else if (next < obj->homeY) {
                moving = TRUE;
            }
            if (moving) {
                obj->y = next;
            } else {
                obj->y = obj->homeY;
                obj->timer = -waitFrames;
            }
        }
    }
    if (obj->timer > 0) {
        next = obj->y + step;
        if (obj->homeY < target) {
            if (next < target) {
                moving = TRUE;
            }
        } else if (target < next) {
            moving = TRUE;
        }
        if (moving) {
            obj->y = next;
            obj->timer = waitFrames;
        } else {
            obj->y = target;
            obj->timer--;
        }
    }
    if (obj->timer < 0) {
        obj->timer++;
    }
    obj->moveY = obj->y - startY;
}

struct LiftXYZ {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 param; // s8 dx | s8 dz << 8 | u8 frames << 16 | s8 dy << 24, offsets in units of 64
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xA];
    s16 moveX;
    s16 moveY;
    s16 moveZ;
    u8 pad60[0x1C];
    s32 homeXY;     // home x << 16 | home y
    s32 homeZState; // home z | s8 state << 16 | u8 progress << 24
    s32 sound;      // sound id | playing sound handle << 16
};

/*
 * Moves the object by a packed offset over `frames` frames while triggered, waits, then returns
 * home, playing a looping sound while moving and a stop sound (0x83) when it arrives.
 */
void func_1511575C(struct LiftXYZ *obj) {
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

    frames = obj->param >> 16;
    dx = (s8)obj->param << 6;
    dy = (s8)(obj->param >> 24) << 6;
    dz = (s16)((s8)(obj->param >> 8) << 6);
    triggered = FALSE;
    soundId = obj->sound;
    startX = obj->x;
    startY = obj->y;
    startZ = obj->z;
    handle = (u16)(obj->sound >> 16);
    if ((obj->flags & 4) == 4) {
        triggered = TRUE;
    }
    if (obj->homeXY == 0) {
        obj->homeXY = (obj->x << 16) | (obj->y & 0xFFFF);
        obj->homeZState = obj->z & 0xFFFF;
    }
    homeX = obj->homeXY >> 16;
    homeY = obj->homeXY;
    targetX = homeX + dx;
    homeZ = obj->homeZState;
    state = (s8)(obj->homeZState >> 16);
    progress = (u8)(obj->homeZState >> 24);
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
                    func_100111C8(handle);
                    handle = 0;
                    func_10010F88(0x83, 0x5000, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
                }
            } else if (soundId && handle == 0) {
                handle = (u16)func_10010E78(handle, soundId, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
            }
            obj->x = homeX + (f32)(targetX - homeX) * (progress * (1.0f / 255.0f));
            obj->y = homeY + (f32)(targetY - homeY) * (progress * (1.0f / 255.0f));
            obj->z = homeZ + (f32)(targetZ - homeZ) * (progress * (1.0f / 255.0f));
        }
    }
    if (state > 0) {
        if (progress < 255) {
            progress += (u8)(255.0f / frames);
            state = 100;
            if (soundId && handle == 0) {
                handle = (u16)func_10010E78(handle, soundId, 0x7FFF, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
            }
            if (progress > 255) {
                progress = 255;
            }
        } else {
            state--;
            progress = 255;
            if (handle) {
                func_100111C8(handle);
                handle = 0;
                func_10010F88(0x83, 0x5000, 0, 0, 0, obj->x, obj->y, obj->z, 200, 1000);
            }
        }
        obj->x = homeX + (f32)(targetX - homeX) * (progress * (1.0f / 255.0f));
        obj->y = homeY + (f32)(targetY - homeY) * (progress * (1.0f / 255.0f));
        obj->z = homeZ + (f32)(targetZ - homeZ) * (progress * (1.0f / 255.0f));
    }
    if (state < 0) {
        state++;
    }
    obj->homeZState &= 0xFFFF;
    obj->homeZState |= ((state & 0xFF) << 16) | ((progress & 0xFF) << 24);
    obj->moveX = obj->x - startX;
    obj->moveY = obj->y - startY;
    obj->moveZ = obj->z - startZ;
    obj->sound = (handle << 16) | soundId;
}

struct Seesaw {
    f32 rotX;
    f32 rotY;
    u8 pad8[0x8];
    s16 x;
    u8 pad12[0x2];
    s16 z;
    u8 pad16[0x26];
    s32 param; // s8 weight divisor | s8 max tilt << 8 | s8 bias << 16 | s8 min tilt << 24
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0x2C];
    f32 weightX;
    f32 weightZ;
};

/* While triggered, adds the player's x/z offset from the object, rotated into its frame, to the weights. */
void func_15115E0C(struct Seesaw *obj, struct127 *player) {
    f32 sinYaw;
    f32 cosYaw;
    f32 dx;
    f32 dz;

    sinYaw = func_150AD780(obj->rotY * 0.01745329238f);
    cosYaw = func_150AD78C(obj->rotY * 0.01745329238f);
    dx = player->x_position - (f32)obj->x;
    dz = player->z_position - (f32)obj->z;
    if ((obj->flags & 4) == 4) {
        obj->weightX += (cosYaw * dx + sinYaw * dz);
        obj->weightZ -= (cosYaw * dz + sinYaw * dx);
    }
}

/* func_15115E0C, with the weight gained this frame multiplied by 5 when player->unk84 is 0x4B. */
void func_15115EDC(struct Seesaw *obj, struct127 *player) {
    f32 oldX;
    f32 oldZ;

    oldX = obj->weightX;
    oldZ = obj->weightZ;
    func_15115E0C(obj, player);
    if (player->unk84.uh == 0x4B) {
        // matching: the old values are reused to hold this frame's extra gain
        oldX = (obj->weightX - oldX) * 4.0f;
        oldZ = (obj->weightZ - oldZ) * 4.0f;
        obj->weightX += oldX;
        obj->weightZ += oldZ;
    }
}

/* Tilts the seesaw by its accumulated weight (or a constant bias), clamps it, and clears the weights. */
void func_15115F68(struct Seesaw *obj) {
    s8 divisor;
    s8 bias;
    s8 maxTilt;
    s8 minTilt;

    // matching: two assignments per line steer as1's -g3 scheduling
    divisor = obj->param; bias = obj->param >> 16;
    maxTilt = obj->param >> 8; minTilt = obj->param >> 24;
    if (divisor) {
        obj->rotX += (obj->weightX / divisor) + bias;
    } else {
        obj->rotX += (s32)(bias * D_800BE9E4);
    }
    if (maxTilt < obj->rotX) {
        obj->rotX = maxTilt;
    } else if (obj->rotX < minTilt) {
        obj->rotX = minTilt;
    }
    obj->weightZ = 0.0f;
    obj->weightX = 0.0f;
}

struct ScrollingTexture {
    u8 pad0[0x16];
    u16 vertexCount;
    u8 pad18[0x8];
    Vtx *vertices[2];
    u8 pad28[0x14];
    s32 param; // s16 ds << 16 | s16 dt
};

/* Scrolls the texture coordinates of every vertex by (ds, dt), from last frame's buffer into this one. */
void func_15116058(struct ScrollingTexture *obj) {
    s16 ds;
    s16 dt;
    s32 i;

    // matching: IDO only emits this register order for the explicit sign-extension shifts
    ds = (((obj->param >> 16) & 0xFFFF) << 16) >> 16;
    dt = ((obj->param & 0xFFFF) << 16) >> 16;
    for (i = 0; i < obj->vertexCount; i++) {
        obj->vertices[D_800BE9C0][i].v.tc[0] = obj->vertices[!D_800BE9C0][i].v.tc[0] + ds;
        obj->vertices[D_800BE9C0][i].v.tc[1] = obj->vertices[!D_800BE9C0][i].v.tc[1] + dt;
    }
}
