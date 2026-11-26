#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Animation speed for `pitch`: 0.75, or faster the further it is below -5. */
f32 player_getFlyingAnimSpeed(Actor *obj, f32 pitch) {
    f32 speed;

    if (pitch < -5.0f) {
        speed = pitch * -0.01f + 0.7f;
    } else {
        speed = 0.75f;
    }

    return speed;
}

/* Per-frame control of a flying mode: the stick steers and pitches, B thrusts, A reverses, R holds altitude,
 * Z fires (only when level is 0x3C and fewer than 5 id-0x25 objects exist). Also picks up the
 * D_800CC26D object into unk13C and drops it again when unk25C bit 1 is set. */
void player_updateFlying(Actor *actor) {
    u16 angle;
    f32 mag;
    f32 accel;
    f32 temp; // matching: one local for stick_x and the climb limit (split locals move FP registers)
    f32 pitch;
    f32 speed; // capped xz speed, then the actor_accelerate rate, then the old yspd (IDO's allocation)
    f32 animSpeed;
    s32 anim;
    s32 count;
    f32 floorLevel;
    s32 rHeld;
    s32 i;
    f32 dist;
    f32 minDist;
    Actor *obj;
    s32 idx;

    floorLevel = 0.0f;
    actor->unkAB = 1;
    actor->count2 = 1;
    actor->unk222 = 0;
    gActorPressedButtons = gButtonsPressed[0];
    gCurrentPad = gPadTable[0];
    if ((dc_glbl_CharNum <= gScreenLayout) && (gIrisTracking[dc_glbl_CharNum] != 0) && (gIrisClosures[dc_glbl_CharNum] > 0.75f) && (gFrontEndState != 2)) {
        gCurrentPad->stick_x = 0;
        gCurrentPad->stick_y = 0;
        gCurrentPad->button = 0;
        gActorPressedButtons = 0;
    }
    actor->noshove |= 0x40;
    actor->unk1CC = actor->ypos;
    if (actor->unk103 != 0) {
        actor->unk103--;
    }
    if (actor->stunned != 0) {
        actor_updateStunned(actor, 0.25f);
        actor_updatePhysics(actor);
        actor->theta = ((s16)actor->langle + 0x4000) * (360.0f / 65536.0f);
        if (actor->lifting != 0) {
            Actor *rider;

            idx = actor->lifting - 100;
            rider = &movement_obj[idx];
            rider->carried = 0;
            rider->speed = actor->speed;
            rider->intelwalk = 0x21;
            rider->scriptCmd = 0;
            rider->stunned = 0;
            rider->noshove &= ~0x400;
            actor->lifting = 0;
        }
        return;
    }
    if (actor->unk102 == 0) {
        actor->inwater = 0;
        angle = actor_getDirectionAngle(gCurrentPad->stick_x, gCurrentPad->stick_y, 0);
        mag = actor_getStickDeflection(gCurrentPad);
        if (gCurrentPad->button & R_TRIG) {
            mag = 0.0f;
        }
        angle += D_800CC280;
        actor->fangle = actor->langle;
        if (mag > 1.0f) {
            actor->fangle = angle;
        }
        actor->stickspeed = mag * 0.35f;
        actor->intelwalk = 7;
        actor->scriptCmd = 0;
        if ((gActorPressedButtons & (A_BUTTON | B_BUTTON)) || (actor->shad > 100.0f)) {
            actor->intelwalk = 8;
            actor->unk102 = 1;
        }
        actor_updateCommon(actor);
        if (D_800CC26D != 0) {
            if (actor->shad > 30.0f) {
                gEventParam = 0xFF010074;
                event_playSound1Range1800();
                actor_attachToHost(&movement_obj[D_800CC26D - 100], dc_glbl_CharNum);
                actor->lifting = D_800CC26D;
                actor->unk102 = 1;
            }
            actor->yspd = 23.0f;
        }
        if (actor->ypos < actor->waterHeight) {
            actor->unk102 = 1;
            actor->unk86 = 1;
            actor->yspd = -6.0f;
        }
        return;
    }
    speed = actor->speed;
    if (speed > 60.0f) {
        speed = 60.0f;
    }
    pitch = gCurrentPad->stick_y * 1.25f;
    rHeld = gCurrentPad->button & R_TRIG;
    if (rHeld) {
        pitch = 0.0f;
    }
    if (actor->lifting != 0) {
        if (!rHeld) {
            actor->ypos -= 16.0f * gActorTimeStep;
        }
    } else {
        minDist = 1000.0f;
        for (i = 0; i < 25; i++) {
            if ((movement_obj[i].type != 0) && (movement_obj[i].energy != 0) &&
                (movement_obj[i].shad == 0.0f) && (movement_obj[i].intelwalk != 0x21) &&
                ((movement_obj[i].character_type == 0x9C) || (movement_obj[i].character_type == 0x9D))) {
                dist = actor_distanceXZ(gCurrentActor, &movement_obj[i]);
                if (dist < minDist) {
                    actor->unk222 = i;
                    minDist = dist;
                }
            }
        }
    }
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }
    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    if (actor->ypos > 2100.0f) {
        if (actor->yspd > -30.0f) {
            actor->yspd -= 1.5f;
        }
        pitch = 40.0f;
    }
    temp = gCurrentPad->stick_x;
    actor->lean_angle += (temp - actor->lean_angle) * (0.2f * gActorTimeStep);
    actor->angle -= (s16)(actor->lean_angle * gActorTimeStep * 0.04f * ((100.0f - speed) * 0.015f) * 220.0f);
    actor->unk7E = 0x19;
    actor_turnFacing(actor, 8, actor->angle);
    actor->wantspeed = 0.0f;
    speed = 0.0f;
    accel = 1.3f;
    if (actor->ypos < actor->waterHeight) {
        if (actor->unk86 == 0) {
            actor->unk86 = 1;
            actor->speed = 0.0f;
            actor->yspd = -6.0f;
        }
    } else if (actor->unk86 != 0) {
        actor->unk86 = 0;
        actor->yspd = 9.0f;
    }
    if (!(gCurrentPad->button & R_TRIG)) {
        if (gCurrentPad->button & A_BUTTON) {
            actor->wantspeed = -22.0f;
            speed = 1.0f;
            accel = 1.8f;
        }
        if (gCurrentPad->button & B_BUTTON) {
            actor->wantspeed = 30.0f;
            if (actor->yspd < 0.0f) {
                actor->wantspeed -= actor->yspd * 0.6f;
            }
            speed = 1.5f;
            accel = 0.3f;
        }
    }
    actor_accelerate(actor->wantspeed, actor, speed, accel, 1);
    actor->theta = ((s16)actor->langle + 0x4000) * (360.0f / 65536.0f);
    actor->ygrav = pitch * 0.000125f * (200.0f - actor->speed);
    if (actor->lifting == 0) {
        if (actor->yspd > 26.0f) {
            actor->yspd = 26.0f;
        }
        if (actor->yspd < -34.0f) {
            actor->yspd = -34.0f;
        }
    }
    temp = fabsf(pitch * 0.5f);
    if (actor->unk86 != 0) {
        temp = temp * 0.4f;
        actor->ygrav *= 0.6f;
    }
    if (actor->ygrav == 0.0f) {
        if (fabsf(actor->yspd) < 1.5f) {
            actor->yspd = 0.0f;
        } else if (actor->yspd > 0.0f) {
            actor->ygrav = 1.6f;
        } else {
            actor->ygrav = -1.6f;
        }
    } else if ((actor->yspd > temp * 1.2f) && (actor->ygrav < 0.0f)) {
        actor->ygrav = 0.0f;
    } else if ((actor->yspd < -temp * 1.5f) && (actor->ygrav > 0.0f)) {
        actor->ygrav = 0.0f;
    }
    actor->inwater = 10;
    if (gCurrentPad->button & R_TRIG) {
        actor->vtilt += (gCurrentPad->stick_y - actor->vtilt) * 0.2f;
        actor->yspd = 0.0f;
        actor->ygrav = 0.0f;
    } else {
        actor->vtilt = (actor->yspd - floorLevel) * -2.0f;
        if (actor->vtilt > 40.0f) {
            actor->vtilt = 40.0f;
        } else if (actor->vtilt < -55.0f) {
            actor->vtilt = -55.0f;
        }
    }
    speed = actor->yspd;
    D_800CBDD3 = 1;
    actor_updatePhysics(actor);
    D_800CBDD3 = 0;
    if ((actor->lifting != 0) && (actor->shad < 50.0f)) {
        floorLevel = 50.0f;
    }
    if (!(gCurrentPad->button & R_TRIG)) {
        if ((D_800CC26D != 0) && (actor->lifting == 0) && (movement_obj[D_800CC26D - 100].shad == 0.0f)) {
            actor->ypos = movement_obj[D_800CC26D - 100].ypos + 70.0f;
            actor->yspd = 20.0f;
            actor_attachToHost(&movement_obj[D_800CC26D - 100], dc_glbl_CharNum);
            actor->lifting = D_800CC26D;
            gEventParam = 0xFF010074;
            event_playSound1Range1800();
        }
        if (actor->lifting != 0) {
            if (actor->task_attrib & 2) {
                Actor *rider;

                actor->task_attrib &= ~2;
                idx = actor->lifting - 100;
                rider = &movement_obj[idx];
                rider->carried = 0;
                rider->speed = actor->speed;
                rider->intelwalk = 0x21;
                rider->scriptCmd = 0;
                rider->stunned = 0;
                rider->noshove &= ~0x400;
                actor->lifting = 0;
                actor->yspd = 0.0f;
                gEventParam = 0x76;
                event_playSound1Range1800();
            }
        } else if ((gActorPressedButtons & Z_TRIG) && (actor->unk103 == 0)) {
            count = 0;
            if (level == 0x3C) {
                for (idx = 1; idx < 25; idx++) {
                    if ((movement_obj[idx].type != 0) && (movement_obj[idx].character_type == 0x25)) {
                        count++;
                    }
                }
                if (count < 5) {
                    gEventParam = 6;
                    actor->unk103 = 4;
                    event_spawnThrownItem();
                }
            }
        }
    }
    if (actor->shad <= floorLevel) {
        actor->speed *= 0.6f;
        if ((gCurrentPad->button & B_BUTTON) || (actor->unk86 != 0) || (actor->lifting != 0)) {
            if (actor->lifting != 0) {
                if (actor->yspd < -10.0f) {
                    gEventParam = 0xFF0100A7;
                    event_playSound2Range1800();
                    gEventParam = 0xFF060372;
                    event_playSound1Range1800();
                }
            } else if (actor->ypos < 0.0f) {
                gEventParam = 0xFF010072;
                event_playSound1Range1800();
            }
            if (actor->lifting == 0) {
                actor->yspd = 20.0f;
            } else if (actor->unk86 == 0) {
                actor->yspd = 38.0f;
            }
            pitch = -80.0f;
        } else {
            actor->yspd = speed * -1.0f;
            actor->unk102 = 0;
            actor->shad = 0.1f;
            actor->vtilt = 0.0f;
        }
    }
    if (!(gCurrentPad->button & R_TRIG)) {
        actor->vtilt += 20.0f;
    } else {
        actor->yspd = actor->vtilt * -0.35f;
    }
    if (actor->unk86 != 0) {
        actor->count5 = 0;
        if ((fabsf(pitch) > 8.0f) || (actor->wantspeed > 7.0f)) {
            animSpeed = 1.0f;
        } else {
            animSpeed = 0.5f;
        }
    } else {
        animSpeed = player_getFlyingAnimSpeed(actor, pitch);
    }
    anim = 0xF;
    if ((actor->vtilt > 40.0f) && (pitch > 20.0f)) {
        anim = 0x11;
    }
    if ((actor->speed < 20.0f) && (actor->wantspeed <= 0.0f) && (pitch == 0.0f)) {
        anim = 0x18;
    }
    if (actor->speed < 0) {
        anim = 0x1F;
    }
    if (actor->lifting != 0) {
        anim = 0x17;
        animSpeed += 0.2f;
    }
    actor_playAnim(actor, anim, animSpeed, 9.0f, 0.0f, 0.0f, 0);
}

void func_150F43F0(Camera *actor) {
    if (actor->CharContext == 0x3B) {
        script_command(0, 0x405C, 1);
        if (actor->Mode != 0x100 && actor->cmdRunning == 0) {
            if (camera_pushMode(actor, 8, 0, 0, CAMERA_MODE_NORMAL) != 0) {
                actor->AvailableModes |= 0x300000;
                actor->AvailableModes &= ~4;
                actor->DistIndex = 1;
                actor->DistMask = 3;
                camera_loadDistancePreset(actor);
            }
            actor->Tightness = 0;
            actor->height = 125.0f;
            actor->unk34C = 125.0f;
            actor->IdealTripodMagnitude = 220.0f;
            actor->YOffsetTget = 30.0f;
        } else {
            actor->YOffsetTget = 0.0f;
        }
    } else if (actor->Mode == 8) {
        if (actor->cmdRunning == 0) {
            camera_popMode(actor, CAMERA_MODE_NORMAL);
            script_command(0, 0x405C, 0);
        }
    }

    if (script_query(1, 0x4054, 6, 0x9000) != 0 && script_query(1, 0x405E, 6, 0x9000) == 0) {
        actor->AvailableModes |= 0x80000000;
    } else {
        actor->AvailableModes &= ~0x80000000;
    }
}
