#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* matching: no header declares actor_seatRider (movement.c): tintedobject.c was compiled with an s32 return */
void actor_seatRider(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9);

/* Actor (structs.h) declares 0x6C, 0x128, 0x1A8 and 0x2E4 with other widths, so the code below
   reaches the bytes and halves it uses through those fields. */

/* Signed angle in degrees from `from` to `to`, in (-180, 180]. */
f32 weapon_getAngleDelta(f32 from, f32 to) {
    f32 delta;

    from = math_wrapDegrees(from);
    delta = math_wrapDegrees(math_wrapDegrees(to) - from);
    if (delta > 180.0f) {
        delta = -360.0f + delta;
    }
    return delta;
}

/* Signed angle in degrees from `self`'s heading (unk40) to the direction of `target`. */
f32 weapon_getAngleToTarget(Actor *self, Actor *target) {
    s32 angle;

    angle = actor_getDirectionAngle(target->xpos - self->xpos, self->zpos - target->zpos, 0) + 0x4000;
    return weapon_getAngleDelta(angle * (360.0f / 65536.0f), self->theta);
}

/* Transforms the point (x, y, z) by the 4x4 float matrix `mf`. */

/*
 * Ballistic aim error for a shell fired from `obj` (from its bone matrix 3 when it has a skeleton) at
 * elevation `pitch` degrees towards (targetX, targetY, targetZ), aiming `radius` short of it.
 * Returns how far the shell lands short (+) or long (-) of the target, or, when the target is too high
 * to reach, 1000 times the extra elevation it needs.
 */
f32 weapon_getShellAimError(Actor *obj, f32 pitch, f32 targetX, f32 targetY, f32 targetZ, f32 radius) {
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

    if (obj->ob_matrix_ptr != NULL) {
        bones = (Mtx *)obj->ob_matrix_ptr;
        guMtxL2F(mf, &bones[3]);
        math_transformPoint(mf, 0.0f, 0.0f, 1.0f, &x, &y, &z);
        x -= mf[3][0];
        y -= mf[3][1];
        z -= mf[3][2];
        pitch = math_atan2(y, sqrtf(x * x + z * z)) * 57.29577637f;
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        x = mf[3][0];
        y = mf[3][1];
        z = mf[3][2];
    } else {
        x = obj->xpos;
        y = obj->ypos;
        z = obj->zpos;
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
        result = (math_asinLookup(sqrtf(-2.0f * dy * gravity) * 0.006745362654f) * 57.29577637f - pitch) * 1000.0f;
    }
    return result;
}

/* Hides `obj` in the controlling player's camera view (its bit in forcedCullMask) while that player holds R with SniperState set. */
void func_150ED578(Actor *obj) {
    ActorCtl *state;
    s32 player;
    Actor *owner;
    Camera *camera;

    state = obj->mv;
    if ((state != NULL) && (state->bot == 0)) {
        player = gIsMultiplayer ? obj->playernumber : obj->owner;
        if (gPadTable[player]->button & R_TRIG) {
            owner = &movement_obj[player];
            if (owner->mv->SniperState != 0) {
                camera = owner->CameraPtr;
                if (camera != NULL) {
                    obj->forcedCullMask |= 1 << camera->CNumber;
                }
            }
        }
    }
}

/* Scrolls texture groups 0 and 1 of `obj` (its two treads) by the track speeds, each clamped to [-45, 45]. */
void weapon_scrollTreads(Actor *obj, s32 left, s32 right) {
    s32 temp;
    s32 size;

    left = (left < -45) ? -45 : (left > 45) ? 45 : left;
    right = (right < -45) ? -45 : (right > 45) ? 45 : right;

    if (obj->character_type == 0x28) {
        size = 124;
    } else {
        temp = left;
        left = right;
        size = 28;
        right = temp;
    }

    actor_scrollVertexTexCoords(obj, 0, 0, 0x800, 0x800, size, -right * 7, 0);
    actor_scrollVertexTexCoords(obj, 1, size, 0x800, 0x800, size, -left * 7, 0);
}

/* Ray (origin, normalized dir) vs sphere (center, radius); 0 on a miss or when the near hit is behind. */

/*
 * Per-frame update of a player-driven tracked vehicle with an aimable gun.
 * - The controlling player (movement_obj[unk127] when gIsMultiplayer is set, else object 0) drives it: stick X steers,
 *   stick Y sets the throttle, A boosts while the player's state allows it, R stops it.
 * - The two treads are scrolled from the forward speed and the turn rate (weapon_scrollTreads).
 * - The gun angles (mv->gunYaw/gunPitch) either lead STATE->target, or turn with C-left/C-right while
 *   the pitch homes in on the best object ahead, or follow the line of sight of camera camSlot - 1.
 *   They end up in taskint[0] as two s16 angles (x50).
 */
void weapon_updateTank(Actor *obj) {
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
    f32 dir[3]; // matching: an array; a Vec3f changes the register allocation
    f32 limit;
    f32 dist;
    Vec3f nearPoint;
    Vec3f farPoint;
    f32 tNear;
    f32 tFar;
    Camera *camera;
    ActorCtl *subject;

    health = 1;
    if (gIsMultiplayer) {
        viewer[0] = obj->playernumber;
    } else {
        viewer[0] = 0;
    }
    obj->forcedCullMask = 0;
    obj->owner = 0;
    controlled = 0;
    player = obj->owner;
    prevFacing = obj->langle;
    obj->intel_accel = 16;
    obj->intel_decel = 8;
    if (gIsMultiplayer == 0) {
        if (obj->lifting != 0) {
            controlled = 1;
            velocity = obj->speed;
            if (fabsf(velocity) > 5.0f) {
                obj->doing_hit = 1;
                obj->unk114 = 2.0f;
                if (velocity < 0.0f) {
                    obj->doing_hit = 2;
                }
            }
        }
    } else {
        player = obj->playernumber;
        if (obj->playernumber != 0xFF) {
            if (movement_obj[obj->playernumber].mv->bot == 0) {
                controlled = 1;
            }
            driver = &movement_obj[player];
            if (driver->energy != 0) {
                if (driver->mv->unk128 & 0x80) {
                    if (driver->mv->unk12C != 0) {
                        obj->imune = 0xFF;
                        if (D_800BE9E4 < movement_obj[player].mv->unk12C) {
                            movement_obj[player].mv->unk12C -= D_800BE9E4;
                        } else {
                            movement_obj[player].mv->unk12C = 0;
                            mp_releaseSlot(player, 0);
                            obj->imune = 0;
                        }
                    }
                } else if ((driver->mv->unk128 & 0x10) && (driver->mv->unk12C == 0)) {
                    mp_releaseSlot(player, 0);
                }
            }
        }
    }
    /* Driver input. */
    if (controlled) {
        s32 unused3; // matching: an unused block local here steers IDO's register allocation
        throttleScale = 0.0f;
        lockFlags = 0;
        health = movement_obj[player].energy;
        if (health != 0) {
            steer = gPads[player].stick_x;
            obj->intel_accel = 50;
            obj->intel_decel = 50;
            throttleScale = 0.6399999857f;
            stateFlags = movement_obj[player].mv->unk128;
            if ((stateFlags & 1) && (gPadTable[player]->button & A_BUTTON) && (movement_obj[player].mv->unk12C != 0)) {
                obj->intel_accel = 28;
                obj->intel_decel = 28;
                gPads[player].stick_y = 80;
                throttleScale += throttleScale;
                if (D_800BE9E4 < movement_obj[player].mv->unk12C) {
                    movement_obj[player].mv->unk12C -= D_800BE9E4;
                } else {
                    movement_obj[player].mv->unk12C = 0;
                    mp_releaseSlot(player, 0);
                }
            }
            turnRate = 18.0f - obj->speed * (0.3199999928f / throttleScale);
            turnRate = (turnRate * 0.5f + 9.0f) * gFrameDelta;
            if (turnRate < 10) {
                turnRate = 10;
            }
            if ((gPadTable[player]->button & R_TRIG) && ((movement_obj[player].mv->unk128 & 2) || (gIsMultiplayer == 0))) {
                throttleScale = 0.0f;
                lockFlags = 0x1F;
                steer = 0;
                obj->unk22B = 0;
                obj->speed = 0.0f;
            }
            obj->unk22B = (s8)obj->unk22B + (steer - (s8)obj->unk22B) / 4;
            if (gIsMultiplayer != 0) {
                obj->unk22B = steer;
            }
            if (steer == 0) {
                if (((s8)obj->unk22B >= -4) && ((s8)obj->unk22B < 5)) {
                    obj->unk22B = 0;
                }
            }
            obj->fangle = obj->langle - (s8)obj->unk22B * turnRate;
            obj->mvmt_control |= 0x40;
            buttonPad = player;
            if ((gIsMultiplayer != 0) && (player < 2)) {
                if (D_800E0BCE[player] != 0) {
                    buttonPad = D_800E0BCE[player];
                }
            }
            if (gButtonsPressed[buttonPad] & Z_TRIG) {
                obj->unk21C = 0;
                obj->scriptCmd = script_getScript(0, 2);
            }
        }
        if (obj->norun != 0) {
            throttleScale = 0.0f;
            obj->norun--;
        }
        obj->stickspeed = gPads[player].stick_y * throttleScale;
        movement_obj[player].mv->context_b = lockFlags;
        movement_obj[player].mv->rt_context_b = lockFlags;
    } else if (dc_glbl_CharNum < D_8008FD8C) {
        actor_updateButtonTimers(obj);
        if ((player < 4) && (gPadTable[player]->button & Z_TRIG)) {
            gActorPressedButtons = Z_TRIG;
        }
        if (gActorPressedButtons & Z_TRIG) {
            obj->intel_animseq = 1;
        }
    }
    obj->intel_accel = 50;
    obj->intel_decel = 50;
    if ((gIsMultiplayer == 0) || (dc_glbl_CharNum >= D_8008FD8C) || (obj->mv->death_State == 0)) {
        actor_updateCommon(obj);
    }
    if (obj->type != 0) {
        if (obj->intelwalk == 13) {
            actor_seatRider(obj, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 0, 0);
        } else if ((gIsMultiplayer == 0) && (gCollidedActors & 1)) {
            driver = &movement_obj[obj->owner];
            if ((driver->attach_char == 0) && (driver->mv->rt_context_b == 0) && (obj->task_attrib & 0x400)) {
                if ((driver->stickspeed > 15.0f) && (driver->yspd > 10.0f)) {
                    actor_startRiderJump(obj, 68.0f, 4.0f, 13, 5);
                } else if ((driver->norun == 0) && (driver->shad > 70.0f) && (driver->stunned == 0) && (actor_distanceXZ(driver, obj) < 100.0f)) {
                    actor_seatRider(obj, 180.0f, 5, 0x44, 1, 0x10, 0xFF, 0, 1, 0);
                }
            }
        }
        {
            s32 unused4; // matching: as above

            blend = 1.0f; // matching: a literal 0.4f alone schedules differently
            if (obj->energy != 0) {
                typedef s32 scope; // matching: a declaration here gives IDO a new block scope
                throttle = obj->stickspeed;
                trackSpeed = obj->speed;
                turn = (s16)(obj->langle - prevFacing) / 70;
                if (fabsf(throttle) > 4.0f) {
                    trackSpeed = trackSpeed * 0.6000000238f + throttle * (0.400000006f * blend);
                }
                left = (s32)trackSpeed + turn;
                right = (s32)trackSpeed - turn;
            } else {
                right = 0;
                left = 0;
            }
            weapon_scrollTreads(obj, left, right);
        }
        if ((obj->character_type == 0x77) || (obj->character_type == 0x28)) {
            if (((trackSpeed = obj->speed) > 5.0f) && (obj->stickspeed <= 0.0f)) {
                reversing = 1;
            } else if ((trackSpeed < -5.0f) && (obj->stickspeed >= 0.0f)) {
                reversing = 1;
            } else {
                reversing = 0;
            }
            if (reversing) {
                effect_sendAttachmentMessage(obj, 25, 13, 0);
            } else {
                effect_sendAttachmentMessage(obj, 25, 14, 0);
            }
            reversing = reversing ? 12 : 15;
            obj->eyeCode[0] = reversing + 10;
            obj->eyeCode[1] = 19;
        }
        /* Gun aim. */
        if (movement_obj[viewer[0]].mv->bot != 0) {
            if (obj->mv->unk129 >= 0) {
                /* Lead the target: move it along its heading by speed * distance / 200 while aiming. */
                f32 dirZ;
                target = &movement_obj[obj->mv->unk129];
                offset = target->xpos - obj->xpos;
                score = target->zpos - obj->zpos;
                lead = sqrtf(offset * offset + score * score) / 200.0f;
                lead = target->speed * lead;
                heading = target->theta * 0.01745329238f;
                leadX = sinf(heading) * lead;
                leadZ = cosf(heading) * lead;
                target->xpos += leadX;
                target->zpos += leadZ;
                bearing = weapon_getAngleToTarget(obj, target) * 0.01745329238f;
                dirX = sinf(bearing) * cosf(obj->lean_angle * 0.01745329238f);
                dirZ = cosf(bearing) * cosf(obj->vtilt * 0.01745329238f);
                angle = math_atan2(dirX * 1000.0f, dirZ * 1000.0f);
                aimOffset = obj->mv->unk12A * 400.0f;
                obj->mv->gunYaw -= weapon_getAngleDelta(angle * 57.29577637f + obj->mv->unk12A * 10.0f, obj->mv->gunYaw) * 0.125f;
                obj->mv->gunPitch += weapon_getShellAimError(obj, obj->mv->gunPitch - obj->vtilt, target->xpos, target->ypos, target->zpos, aimOffset) * 0.001000000047f;
                target->xpos -= leadX;
                target->zpos -= leadZ;
            } else {
                obj->mv->gunYaw *= 0.8999999762f;
                obj->mv->gunPitch *= 0.8999999762f;
            }
        } else if (controlled && health) {
            typedef s32 scope; // matching: as above
            bestScore = 0.0f;
            yawStep = 3.0f * gFrameDelta;
            bestIdx = -1;
            camSlot = 0;
            objCount = D_8008FD8C;
            if ((gIsMultiplayer != 0) && (viewer[0] < 2) && (D_800E0BCE[viewer[0]] != 0)) {
                camSlot = D_800E0BCE[viewer[0]] + 1;
            } else {
                pad = &gPads[viewer[0]];
                if (pad->button & R_CBUTTONS) {
                    obj->mv->gunYaw += yawStep;
                }
                if (pad->button & L_CBUTTONS) {
                    obj->mv->gunYaw -= yawStep;
                }
            }
            if (movement_obj[player].mv->SniperState != 0) {
                camSlot = viewer[0] + 1;
            }
            if (gIsMultiplayer == 0) {
                objCount = 25;
            }
            if (camSlot == 0) {
                /* Pick the object closest to the gun's yaw (within 35 degrees), weighted by distance. */
                for (i = 0; i < objCount; i++) {
                    if (i != viewer[0]) {
                        other = &movement_obj[i];
                        if ((gIsMultiplayer == 0) && !(other->noshove & 0x40)) {
                            continue;
                        }
                        offset = weapon_getAngleDelta(weapon_getAngleToTarget(obj, other), obj->mv->gunYaw);
                        score = offset;
                        if (offset < 0.0f) {
                            score = -offset;
                        }
                        score = 35.0f - score;
                        if (score < 0.0f) {
                            score = 0.0f;
                        }
                        if (score > 0.0f) {
                            distance = obj->xpos - other->xpos;
                            deltaZ = obj->zpos - other->zpos;
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
                    driver = &movement_obj[bestIdx];
                    if (gIsMultiplayer != 0) {
                        limit = driver->ypos + 40.0f;
                    } else {
                        limit = driver->aim.topY;
                    }
                    deltaZ = weapon_getShellAimError(obj, 0.0f, driver->xpos, limit, driver->zpos, 0.0f) * 0.001000000047f;
                    limit = 4.0f * gFrameDelta;
                    if (limit < deltaZ) {
                        deltaZ = limit;
                    }
                    if (deltaZ < -limit) {
                        deltaZ = -limit;
                    }
                    obj->mv->gunPitch += deltaZ;
                } else {
                    obj->mv->gunPitch *= 0.8999999762f;
                }
            } else {
                camera = &camera_global_Cameras[camSlot - 1];
                subject = camera->mv;
                if (camera->mv->SniperState != 0) {
                    /* Copy the camera subject's angles. */
                    typedef s32 scope; // matching: as above
                    angle = -subject->aimPitch;
                    angle -= obj->vtilt;
                    angle2 = subject->aimYaw;
                    angle2 = (obj->theta - 90.0f) - angle2;
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
                    obj->mv->gunPitch = angle;
                    obj->mv->gunYaw = angle2;
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
                    hit = collision_castRayWorld(camera->eye.x, camera->eye.y, camera->eye.z, dir[0], dir[1], dir[2], 0, 0, &hitX, &hitY, &hitZ, &dist, 0, 0, 0.0f);
                    if (hit != 0) {
                        hit = 1;
                    }
                    for (j = 0; j < D_8008FD8C; j++) {
                        if (obj->team != movement_obj[j].team) {
                            hitX = movement_obj[j].xpos - camera->eye.x;
                            hitZ = movement_obj[j].zpos - camera->eye.z;
                            if (sqrtf(hitX * hitX + hitZ * hitZ) + 200.0f < dist) {
                                if (math_raySphere(&camera->eye, (Vec3f *)dir, (Vec3f *)&movement_obj[j].xpos, 150.0f, &nearPoint, &farPoint, &tNear, &tFar) && (tNear < dist)) {
                                    dist = tNear;
                                    hit = 2;
                                }
                            }
                        }
                    }
                    subject->unk128 &= ~0x40;
                    if (hit != 0) {
                        if (hit == 2) {
                            subject->unk128 |= 0x40;
                        }
                        hitX = dir[0] * dist + camera->eye.x;
                        hitY = dir[1] * dist + camera->eye.y;
                        hitZ = dir[2] * dist + camera->eye.z;
                        obj->mv->gunPitch += weapon_getShellAimError(obj, 0.0f, hitX, hitY, hitZ, 0.0f) * 0.001000000047f;
                    } else {
                        obj->mv->gunPitch *= 0.8999999762f;
                    }
                }
            }
        }
        if (obj->mv->gunPitch < -20.0f) {
            obj->mv->gunPitch = -20.0f;
        }
        if (obj->mv->gunPitch > 80.0f) {
            obj->mv->gunPitch = 80.0f;
        }
        if (obj->mv->gunYaw > 360.0f) {
            obj->mv->gunYaw -= 360.0f;
        }
        if (obj->mv->gunYaw < 0.0f) {
            obj->mv->gunYaw += 360.0f;
        }
        ((s16 *)&obj->taskint[0])[0] = obj->mv->gunYaw * 50.0f;
        ((s16 *)&obj->taskint[0])[1] = -(s16)(obj->mv->gunPitch * 50.0f);
    }
}

/* Sets `obj` to state 3 (immune) and gives its camera back to player object 0 or 1, clearing that pad's R. */
void weapon_releaseTankCamera(Actor *obj) {
    Camera *camera;
    Actor *player;
    s32 index;

    obj->drawtype = 3;
    obj->bodyRadius = 0;
    obj->imune = 0xFF;
    obj->unk328 = 0;
    if (obj->CameraPtr != NULL) {
        camera = obj->CameraPtr;
        index = 0;
        if (camera->CNumber == 3) {
            index = 1;
        }
        camera_attachToObject(camera, index);
        obj->mv->rt_context_b = 0x29;
        gPads[index].button &= ~R_TRIG;
        player = &movement_obj[index];
        player->forcedCullMask |= 1 << camera->CNumber;
        if (player->unk10A != 0) {
            screen_startViewFlash(0xFF, 0, 0, 180, 20, camera->CNumber);
            player->unk10A = 0;
        }
    }
}


/* Sets `obj` to state 3 (immune) if it is not already, then runs the common object update. */
void func_150EEDA8(Actor *obj) {
    if (obj->drawtype != 3) {
        func_151045E0(obj, 15, 250.0f);
        obj->drawtype = 3;
        obj->bodyRadius = 0;
        obj->imune = 0xFF;
    }
    actor_updateCommon(obj);
}
