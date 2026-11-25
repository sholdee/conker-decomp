#define func_1514D3B0 func_1514D3B0_hdr
#include <ultra64.h>
#include "functions.h"
#undef func_1514D3B0
// matching: functions.h's (s32, s16, u8, u8) prototype changes the call; this file passes plain ints
extern s32 func_1514D3B0(void *, s16, s32, s32);
#include "variables.h"

extern void func_151254F4(Camera *camera, s32 index);
extern void func_1517F488(s32 r, s32 g, s32 b, s32 alpha, s32 duration, s32 view);
extern f32 func_15144BC8(f32 angle); /* wraps an angle into [0, 360] degrees */


/* Actor (structs.h) declares 0x6C, 0x128, 0x1A8 and 0x2E4 with other widths, so the code below
   reaches the bytes and halves it uses through those fields. */

/* Signed angle in degrees from `from` to `to`, in (-180, 180]. */
f32 func_150ED1D0(f32 from, f32 to) {
    f32 delta;

    from = func_15144BC8(from);
    delta = func_15144BC8(func_15144BC8(to) - from);
    if (delta > 180.0f) {
        delta = -360.0f + delta;
    }
    return delta;
}

/* Signed angle in degrees from `self`'s heading (unk40) to the direction of `target`. */
f32 func_150ED234(Actor *self, Actor *target) {
    s32 angle;

    angle = func_1505A630(target->x_position - self->x_position, self->z_position - target->z_position, 0) + 0x4000;
    return func_150ED1D0(angle * (360.0f / 65536.0f), self->unk40);
}

/* Transforms the point (x, y, z) by the 4x4 float matrix `mf`. */
extern void func_150A7960(f32 *mf, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/*
 * Ballistic aim error for a shell fired from `obj` (from its bone matrix 3 when it has a skeleton) at
 * elevation `pitch` degrees towards (targetX, targetY, targetZ), aiming `radius` short of it.
 * Returns how far the shell lands short (+) or long (-) of the target, or, when the target is too high
 * to reach, 1000 times the extra elevation it needs.
 */
f32 func_150ED298(Actor *obj, f32 pitch, f32 targetX, f32 targetY, f32 targetZ, f32 radius) {
    Mtx *bones;
    f32 gravity;
    f32 disc;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    f32 speedY;
    f32 speedXZ;
    f32 dz;
    f32 dy;
    f32 dist;
    f32 time;
    f32 result;
    f32 root;
    MtxF mf;
    f32 firstTime;

    if (obj->unk1D4 != NULL) {
        bones = (Mtx *)obj->unk1D4;
        guMtxL2F(mf, &bones[3]);
        func_150A7960(mf[0], 0.0f, 0.0f, 1.0f, &x, &y, &z);
        x -= mf[3][0];
        y -= mf[3][1];
        z -= mf[3][2];
        pitch = func_150484A0(y, sqrtf(x * x + z * z)) * 57.29577637f;
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        x = mf[3][0];
        y = mf[3][1];
        z = mf[3][2];
    } else {
        x = obj->x_position;
        y = obj->y_position;
        z = obj->z_position;
    }
    dx = targetX - x;
    dy = targetY - y;
    dz = targetZ - z;
    dist = sqrtf(dx * dx + dz * dz) - radius;
    if (dist < 100.0f) {
        dist = 100.0f;
    }
    result = 5000.0f;
    speedXZ = cosf(pitch * 0.01745329238f) * 148.25f;
    speedY = sinf(pitch * 0.01745329238f) * 148.25f;
    gravity = -2.5f;
    disc = speedY * speedY + 2 * (dy * gravity);
    if (disc > 0.0f) {
        root = sqrtf(disc);
        firstTime = (-speedY - root) / gravity;
        time = firstTime;
        if (firstTime > 0.0f) {
            disc = (-speedY + root) / gravity;
            if (disc > 0.0f && firstTime < disc) {
                time = disc; // matching: disc doubles as the second root; a separate local grows the frame
            }
        }
        if (time > 0.0f) {
            time *= speedXZ;
            result = dist - time;
        }
    } else {
        result = (func_15048408(sqrtf(-2.0f * dy * gravity) * 0.006745362654f) * 57.29577637f - pitch) * 1000.0f;
    }
    return result;
}

/* Lets `obj` see the controlling player's camera (bit in unk2FC) while that player holds R with unk197 set. */
void func_150ED578(Actor *obj) {
    ActorCtl *state;
    s32 player;
    Actor *owner;
    Camera *camera;

    state = obj->unk31C;
    if ((state != NULL) && (state->unk84 == 0)) {
        player = D_800BE616 ? obj->unk127 : obj->unk124;
        if (D_800BE728[player]->button & R_TRIG) {
            owner = &D_800CC2D0[player];
            if (owner->unk31C->aimState != 0) {
                camera = owner->camera;
                if (camera != NULL) {
                    obj->unk2FC |= 1 << camera->playerIndex;
                }
            }
        }
    }
}

// matching: arg2 as s32 here; the u8 of game_83300.c's definition would add a narrowing
extern void func_15062FC0(Actor *actor, s32 group, s32 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt);

/* Scrolls texture groups 0 and 1 of `obj` (its two treads) by the track speeds, each clamped to [-45, 45]. */
void func_150ED638(Actor *obj, s32 left, s32 right) {
    s32 temp;
    s32 size;

    left = (left < -45) ? -45 : (left > 45) ? 45 : left;
    right = (right < -45) ? -45 : (right > 45) ? 45 : right;

    if (obj->id == 0x28) {
        size = 124;
    } else {
        temp = left;
        left = right;
        size = 28;
        right = temp;
    }

    func_15062FC0(obj, 0, 0, 0x800, 0x800, size, -right * 7, 0);
    func_15062FC0(obj, 1, size, 0x800, 0x800, size, -left * 7, 0);
}

extern s8 D_800E0BCE[];
extern s32 func_1508DAEC(s32 player, s32 mode);
extern struct216 *func_1507BB28(s32 arg0, s32 scriptId);
extern void func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7, s32 force, s32 arg9);
extern void func_1505327C(Actor *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32, s32, f32 *, f32 *, f32 *, f32 *, s32, s32, f32);
extern void func_15052590(Actor *obj);
/* Ray (origin, normalized dir) vs sphere (center, radius); 0 on a miss or when the near hit is behind. */
extern s32 func_151452C4(f32 *origin, f32 *dir, f32 *center, f32 radius, f32 *hitNear, f32 *hitFar, f32 *tNear, f32 *tFar);

/*
 * Per-frame update of a player-driven tracked vehicle with an aimable gun.
 * - The controlling player (D_800CC2D0[unk127] when D_800BE616 is set, else object 0) drives it: stick X steers,
 *   stick Y sets the throttle, A boosts while the player's state allows it, R stops it.
 * - The two treads are scrolled from the forward speed and the turn rate (func_150ED638).
 * - The gun angles (STATE aimYaw/aimPitch) either lead STATE->target, or turn with C-left/C-right while
 *   the pitch homes in on the best object ahead, or follow the line of sight of camera camSlot - 1.
 *   They end up in unk2E4 as two s16 angles (x50).
 */
void func_150ED748(Actor *obj) {
    s32 viewer[1]; // matching: an array keeps the viewer index in memory; a scalar is register-allocated
    f32 velocity;
    s32 health;
    u8 controlled;
    u8 player;
    u16 prevFacing;
    s32 buttonPad;
    f32 throttle;
    s32 lockFlags;
    f32 trackSpeed;
    f32 throttleScale;
    s8 steer;
    s16 turnRate;
    s16 turn;
    s32 left;
    s32 right;
    s32 stateFlags;
    f32 blend;
    Actor *driver;
    Actor *other;
    s32 reversing;
    OSContPad *pad;
    s8 unused1; // matching: unused, but keeps the stack layout
    Actor *target;
    f32 aimOffset;
    f32 heading;
    f32 leadX;
    f32 leadZ;
    f32 unused2; // matching: unused, but keeps the stack layout
    f32 bearing;
    f32 lead;
    f32 dirX;
    f32 angle2;
    f32 angle;
    f32 deltaZ;
    f32 distance;
    f32 score;
    f32 bestScore;
    s32 i;
    s32 bestIdx;
    s32 objCount;
    f32 yawStep;
    f32 offset;
    s8 camSlot;
    s32 j;
    s32 hit;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 dir[3];
    f32 limit;
    f32 dist;
    f32 nearPoint[3];
    f32 farPoint[3];
    f32 tNear;
    f32 tFar;
    Camera *camera;
    Actor *subject;

    health = 1;
    if (D_800BE616) {
        viewer[0] = obj->unk127;
    } else {
        viewer[0] = 0;
    }
    obj->unk2FC = 0;
    obj->unk124 = 0;
    controlled = 0;
    player = obj->unk124;
    prevFacing = obj->unk7A;
    obj->unk247 = 16;
    obj->unk248 = 8;
    if (D_800BE616 == 0) {
        if (obj->unk13C != 0) {
            controlled = 1;
            velocity = obj->xz_velocity;
            if (fabsf(velocity) > 5.0f) {
                obj->unkD0 = 1;
                obj->unk114 = 2.0f;
                if (velocity < 0.0f) {
                    obj->unkD0 = 2;
                }
            }
        }
    } else {
        player = obj->unk127;
        if (obj->unk127 != 0xFF) {
            if (D_800CC2D0[obj->unk127].unk31C->unk84 == 0) {
                controlled = 1;
            }
            driver = &D_800CC2D0[player];
            if (driver->health != 0) {
                if (driver->unk31C->unk128 & 0x80) {
                    if (driver->unk31C->unk12C != 0) {
                        obj->immune = 0xFF;
                        if (D_800BE9E4 < D_800CC2D0[player].unk31C->unk12C) {
                            D_800CC2D0[player].unk31C->unk12C -= D_800BE9E4;
                        } else {
                            D_800CC2D0[player].unk31C->unk12C = 0;
                            func_1508DAEC(player, 0);
                            obj->immune = 0;
                        }
                    }
                } else if ((driver->unk31C->unk128 & 0x10) && (driver->unk31C->unk12C == 0)) {
                    func_1508DAEC(player, 0);
                }
            }
        }
    }
    /* Driver input. */
    if (controlled) {
        s32 unused3; // matching: an unused block local here steers IDO's register allocation
        throttleScale = 0.0f;
        lockFlags = 0;
        health = D_800CC2D0[player].health;
        if (health != 0) {
            steer = D_800BE748[player].stick_x;
            obj->unk247 = 50;
            obj->unk248 = 50;
            throttleScale = 0.6399999857f;
            stateFlags = D_800CC2D0[player].unk31C->unk128;
            if ((stateFlags & 1) && (D_800BE728[player]->button & A_BUTTON) && (D_800CC2D0[player].unk31C->unk12C != 0)) {
                obj->unk247 = 28;
                obj->unk248 = 28;
                D_800BE748[player].stick_y = 80;
                throttleScale += throttleScale;
                if (D_800BE9E4 < D_800CC2D0[player].unk31C->unk12C) {
                    D_800CC2D0[player].unk31C->unk12C -= D_800BE9E4;
                } else {
                    D_800CC2D0[player].unk31C->unk12C = 0;
                    func_1508DAEC(player, 0);
                }
            }
            turnRate = 18.0f - obj->xz_velocity * (0.3199999928f / throttleScale);
            turnRate = (turnRate * 0.5f + 9.0f) * D_800BE9A4;
            if (turnRate < 10) {
                turnRate = 10;
            }
            if ((D_800BE728[player]->button & R_TRIG) && ((D_800CC2D0[player].unk31C->unk128 & 2) || (D_800BE616 == 0))) {
                throttleScale = 0.0f;
                lockFlags = 0x1F;
                steer = 0;
                obj->unk22B = 0;
                obj->xz_velocity = 0.0f;
            }
            obj->unk22B = (s8)obj->unk22B + (steer - (s8)obj->unk22B) / 4;
            if (D_800BE616 != 0) {
                obj->unk22B = steer;
            }
            if (steer == 0) {
                if (((s8)obj->unk22B >= -4) && ((s8)obj->unk22B < 5)) {
                    obj->unk22B = 0;
                }
            }
            obj->unk78 = obj->unk7A - (s8)obj->unk22B * turnRate;
            obj->unkF4 |= 0x40;
            buttonPad = player;
            if ((D_800BE616 != 0) && (player < 2)) {
                if (D_800E0BCE[player] != 0) {
                    buttonPad = D_800E0BCE[player];
                }
            }
            if (D_800BE710[buttonPad] & Z_TRIG) {
                obj->unk21C = 0;
                obj->unk218 = func_1507BB28(0, 2);
            }
        }
        if (obj->disable_run != 0) {
            throttleScale = 0.0f;
            obj->disable_run--;
        }
        obj->unk44 = D_800BE748[player].stick_y * throttleScale;
        D_800CC2D0[player].unk31C->unk75 = lockFlags;
        D_800CC2D0[player].unk31C->unk78 = lockFlags;
    } else if (D_800C3E78 < D_8008FD8C) {
        func_1504C854(obj);
        if ((player < 4) && (D_800BE728[player]->button & Z_TRIG)) {
            D_800CC288 = Z_TRIG;
        }
        if (D_800CC288 & Z_TRIG) {
            obj->unk244 = 1;
        }
    }
    obj->unk247 = 50;
    obj->unk248 = 50;
    if ((D_800BE616 == 0) || (D_800C3E78 >= D_8008FD8C) || (obj->unk31C->unk120 == 0)) {
        func_15052590(obj);
    }
    if (obj->interaction_state != 0) {
        if (obj->unk232 == 13) {
            func_15052F9C(obj, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 0, 0);
        } else if ((D_800BE616 == 0) && (D_800CC268 & 1)) {
            subject = &D_800CC2D0[obj->unk124];
            if ((subject->unk65 == 0) && (subject->unk31C->unk78 == 0) && (obj->unk25C & 0x400)) {
                if ((subject->unk44 > 15.0f) && (subject->y_velocity > 10.0f)) {
                    func_1505327C(obj, 68.0f, 4.0f, 13, 5);
                } else if ((subject->disable_run == 0) && (subject->unk28 > 70.0f) && (subject->stunned == 0) && (func_1505A6F8(subject, obj) < 100.0f)) {
                    func_15052F9C(obj, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 1, 0);
                }
            }
        }
        {
            s32 unused4; // matching: as above

            blend = 1.0f; // matching: a literal 0.4f alone schedules differently
            if (obj->health != 0) {
                typedef s32 scope; // matching: a declaration here gives IDO a new block scope
                throttle = obj->unk44;
                trackSpeed = obj->xz_velocity;
                turn = (s16)(obj->unk7A - prevFacing) / 70;
                if (fabsf(throttle) > 4.0f) {
                    trackSpeed = trackSpeed * 0.6000000238f + throttle * (0.400000006f * blend);
                }
                left = (s32)trackSpeed + turn;
                right = (s32)trackSpeed - turn;
            } else {
                right = 0;
                left = 0;
            }
            func_150ED638(obj, left, right);
        }
        if ((obj->id == 0x77) || (obj->id == 0x28)) {
            if (((trackSpeed = obj->xz_velocity) > 5.0f) && (obj->unk44 <= 0.0f)) {
                reversing = 1;
            } else if ((trackSpeed < -5.0f) && (obj->unk44 >= 0.0f)) {
                reversing = 1;
            } else {
                reversing = 0;
            }
            if (reversing) {
                func_1514D3B0(obj, 25, 13, 0);
            } else {
                func_1514D3B0(obj, 25, 14, 0);
            }
            reversing = reversing ? 12 : 15;
            obj->eyeCode[0] = reversing + 10;
            obj->eyeCode[1] = 19;
        }
        /* Gun aim. */
        if (D_800CC2D0[viewer[0]].unk31C->unk84 != 0) {
            if (obj->unk31C->unk129 >= 0) {
                /* Lead the target: move it along its heading by speed * distance / 200 while aiming. */
                f32 dirZ;
                target = &D_800CC2D0[obj->unk31C->unk129];
                offset = target->x_position - obj->x_position;
                score = target->z_position - obj->z_position;
                lead = sqrtf(offset * offset + score * score) / 200.0f;
                lead = target->xz_velocity * lead;
                heading = target->unk40 * 0.01745329238f;
                leadX = sinf(heading) * lead;
                leadZ = cosf(heading) * lead;
                target->x_position += leadX;
                target->z_position += leadZ;
                bearing = func_150ED234(obj, target) * 0.01745329238f;
                dirX = sinf(bearing) * cosf(obj->unkC4 * 0.01745329238f);
                dirZ = cosf(bearing) * cosf(obj->unkB8 * 0.01745329238f);
                angle = func_150484A0(dirX * 1000.0f, dirZ * 1000.0f);
                aimOffset = obj->unk31C->unk12A * 400.0f;
                obj->unk31C->unk1BC -= func_150ED1D0(angle * 57.29577637f + obj->unk31C->unk12A * 10.0f, obj->unk31C->unk1BC) * 0.125f;
                obj->unk31C->unk1B8 += func_150ED298(obj, obj->unk31C->unk1B8 - obj->unkB8, target->x_position, target->y_position, target->z_position, aimOffset) * 0.001000000047f;
                target->x_position -= leadX;
                target->z_position -= leadZ;
            } else {
                obj->unk31C->unk1BC *= 0.8999999762f;
                obj->unk31C->unk1B8 *= 0.8999999762f;
            }
        } else if (controlled && health) {
            typedef s32 scope; // matching: as above
            bestScore = 0.0f;
            yawStep = 3.0f * D_800BE9A4;
            bestIdx = -1;
            camSlot = 0;
            objCount = D_8008FD8C;
            if ((D_800BE616 != 0) && (viewer[0] < 2) && (D_800E0BCE[viewer[0]] != 0)) {
                camSlot = D_800E0BCE[viewer[0]] + 1;
            } else {
                pad = &D_800BE748[viewer[0]];
                if (pad->button & R_CBUTTONS) {
                    obj->unk31C->unk1BC += yawStep;
                }
                if (pad->button & L_CBUTTONS) {
                    obj->unk31C->unk1BC -= yawStep;
                }
            }
            if (D_800CC2D0[player].unk31C->aimState != 0) {
                camSlot = viewer[0] + 1;
            }
            if (D_800BE616 == 0) {
                objCount = 25;
            }
            if (camSlot == 0) {
                /* Pick the object closest to the gun's yaw (within 35 degrees), weighted by distance. */
                for (i = 0; i < objCount; i++) {
                    if (i != viewer[0]) {
                        other = &D_800CC2D0[i];
                        if ((D_800BE616 == 0) && !(other->unkF8 & 0x40)) {
                            continue;
                        }
                        offset = func_150ED1D0(func_150ED234(obj, other), obj->unk31C->unk1BC);
                        score = offset;
                        if (offset < 0.0f) {
                            score = -offset;
                        }
                        score = 35.0f - score;
                        if (score < 0.0f) {
                            score = 0.0f;
                        }
                        if (score > 0.0f) {
                            distance = obj->x_position - other->x_position;
                            deltaZ = obj->z_position - other->z_position;
                            distance = sqrtf(distance * distance + deltaZ * deltaZ);
                            if (distance < 7000.0f) {
                                if (distance < 1.0f) {
                                    distance = 1.0f;
                                }
                                score = score / distance;
                                if (bestScore < score) {
                                    bestScore = score;
                                    bestIdx = i;
                                }
                            }
                        }
                    }
                }
                if (bestIdx != -1) {
                    typedef s32 scope; // matching: as above
                    subject = &D_800CC2D0[bestIdx];
                    if (D_800BE616 != 0) {
                        limit = subject->y_position + 40.0f;
                    } else {
                        limit = subject->aim.topY;
                    }
                    deltaZ = func_150ED298(obj, 0.0f, subject->x_position, limit, subject->z_position, 0.0f) * 0.001000000047f;
                    limit = 4.0f * D_800BE9A4;
                    if (limit < deltaZ) {
                        deltaZ = limit;
                    }
                    if (deltaZ < -limit) {
                        deltaZ = -limit;
                    }
                    obj->unk31C->unk1B8 += deltaZ;
                } else {
                    obj->unk31C->unk1B8 *= 0.8999999762f;
                }
            } else {
                camera = &D_800DBFF0[camSlot - 1];
                subject = camera->playerExt;
                if (camera->playerExt->aimState != 0) {
                    /* Copy the camera subject's angles. */
                    typedef s32 scope; // matching: as above
                    angle = -subject->unk170;
                    angle -= obj->unkB8;
                    angle2 = subject->unk16C;
                    angle2 = (obj->unk40 - 90.0f) - angle2;
                    while (angle > 180.0f) {
                        angle -= 360.0f;
                        if (angle) {} // matching: steers the scheduling of the wrap loop
                    }
                    while (angle < -180.0f) {
                        angle += 360.0f;
                    }
                    while (angle2 > 180.0f) {
                        angle2 -= 360.0f;
                    }
                    while (angle2 < -180.0f) {
                        angle2 += 360.0f;
                    }
                    obj->unk31C->unk1B8 = angle;
                    obj->unk31C->unk1BC = angle2;
                } else {
                    /* Aim at what the camera's look ray hits: level geometry or another object. */
                    dir[0] = camera->target.x - camera->eye.x;
                    dir[1] = camera->target.y - camera->eye.y;
                    dir[2] = camera->target.z - camera->eye.z;
                    score = 1.0f / sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
                    dir[0] *= score;
                    dir[1] *= score;
                    dir[2] *= score;
                    dist = 100000.0f;
                    hit = func_150AC9C0(camera->eye.x, camera->eye.y, camera->eye.z, dir[0], dir[1], dir[2], 0, 0, &hitX, &hitY, &hitZ, &dist, 0, 0, 0.0f);
                    if (hit != 0) {
                        hit = 1;
                    }
                    for (j = 0; j < D_8008FD8C; j++) {
                        if (obj->team != D_800CC2D0[j].team) {
                            hitX = D_800CC2D0[j].x_position - camera->eye.x;
                            hitZ = D_800CC2D0[j].z_position - camera->eye.z;
                            if (sqrtf(hitX * hitX + hitZ * hitZ) + 200.0f < dist) {
                                if (func_151452C4(&camera->eye.x, dir, &D_800CC2D0[j].x_position, 150.0f, nearPoint, farPoint, &tNear, &tFar) && (tNear < dist)) {
                                    dist = tNear;
                                    hit = 2;
                                }
                            }
                        }
                    }
                    subject->team &= ~0x40;
                    if (hit != 0) {
                        if (hit == 2) {
                            subject->team |= 0x40;
                        }
                        hitX = dir[0] * dist + camera->eye.x;
                        hitY = dir[1] * dist + camera->eye.y;
                        hitZ = dir[2] * dist + camera->eye.z;
                        obj->unk31C->unk1B8 += func_150ED298(obj, 0.0f, hitX, hitY, hitZ, 0.0f) * 0.001000000047f;
                    } else {
                        obj->unk31C->unk1B8 *= 0.8999999762f;
                    }
                }
            }
        }
        if (obj->unk31C->unk1B8 < -20.0f) {
            obj->unk31C->unk1B8 = -20.0f;
        }
        if (obj->unk31C->unk1B8 > 80.0f) {
            obj->unk31C->unk1B8 = 80.0f;
        }
        if (obj->unk31C->unk1BC > 360.0f) {
            obj->unk31C->unk1BC -= 360.0f;
        }
        if (obj->unk31C->unk1BC < 0.0f) {
            obj->unk31C->unk1BC += 360.0f;
        }
        ((s16 *)&obj->unk2E4)[0] = obj->unk31C->unk1BC * 50.0f;
        ((s16 *)&obj->unk2E4)[1] = -(s16)(obj->unk31C->unk1B8 * 50.0f);
    }
}

/* Sets `obj` to state 3 (immune) and gives its camera back to player object 0 or 1, clearing that pad's R. */
void func_150EEC84(Actor *obj) {
    Camera *camera;
    Actor *player;
    s32 index;

    obj->unk5 = 3;
    obj->unkE4 = 0;
    obj->immune = 0xFF;
    obj->unk328 = 0;
    if (obj->camera != NULL) {
        camera = obj->camera;
        index = 0;
        if (camera->playerIndex == 3) {
            index = 1;
        }
        func_151254F4(camera, index);
        obj->unk31C->unk78 = 0x29;
        D_800BE748[index].button &= ~R_TRIG;
        player = &D_800CC2D0[index];
        player->unk2FC |= 1 << camera->playerIndex;
        if (player->pad10A != 0) {
            func_1517F488(0xFF, 0, 0, 180, 20, camera->playerIndex);
            player->pad10A = 0;
        }
    }
}

extern void func_151045E0(void *, s32, f32);

/* Sets `obj` to state 3 (immune) if it is not already, then runs the common object update. */
void func_150EEDA8(Actor *obj) {
    if (obj->unk5 != 3) {
        func_151045E0(obj, 15, 250.0f);
        obj->unk5 = 3;
        obj->unkE4 = 0;
        obj->immune = 0xFF;
    }
    func_15052590(obj);
}
