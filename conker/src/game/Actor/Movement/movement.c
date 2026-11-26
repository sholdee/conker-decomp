#include <ultra64.h>
#include "structs.h"

/* This file's .bss (0x800CC250-0x800D155F), defined in address order before functions.h and variables.h
   declare the globals: IDO lays out .bss in the order it first sees each name, and that order also
   depends on how many types and variables are declared before it (see init/Sched/SchedInit/schedinit.c). */
s32 D_800CC250;
f32 D_800CC254; // object position saved at the hit test (x, y, z)
f32 D_800CC258;
f32 D_800CC25C;
s32 gAttackHitMask;
s16 gActorStepPercent; // frame speed in percent (100 = one step)
s32 gCollidedActors;
u8 D_800CC26C;
u8 D_800CC26D;
u8 D_800CC26E;
f32 gHitAttackerX; // attacker position saved at the hit (x, z, y)
f32 gHitAttackerZ;
f32 gHitAttackerY;
u8 gFloorMaterialClass; // floor material class of the object being updated
u16 D_800CC27E;
s32 D_800CC280;
OSContPad *gCurrentPad; // the current player's controller input
s32 gActorPressedButtons;
s32 gActorNewButtons;
s32 gActorHeldButtons;
s32 D_800CC294; // not referenced by any code
TexAnimState D_800CC298;
u8 gActionsPending;
u8 D_800CC2B1;
s16 gWaistTiltTarget;
f32 gWaistTiltDamping;
u8 gActorUpdateParity;
u16 D_800CC2BA; // frame step of the actor being updated
s32 D_800CC2BC;
f32 gHitPointX;
f32 gHitPointY;
f32 gHitPointZ;
Actor movement_obj[26]; // the actors (original: movement_obj, an array of movement_move)
s32 gActorFrameCount;
Actor *gCurrentActor; // the actor being updated
f32 gActorTimeStep; // frame time step

#include "functions.h"
#include "variables.h"

/* Per-mode tables indexed by gFloorMaterialClass (D_800990C0 and D_800990E0 are read by func_15051558). */
static const f32 D_800990C0[8] = { 0.0f, 0.6000000238f, 0.1800000072f, 0.3000000119f, 0.0f, 2.0f, 2.0f, 4.0f };
static const f32 D_800990E0[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
static const f32 D_80099100[8] = { 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f };
static const f32 D_80099120[8] = { 0.0f, 0.75f, 0.75f, 1.0f, 1.25f, 0.5f, 0.5f, 0.5f };

/* Indexed by floor material (func_1504BA38). */
static const Struct80099140 D_80099140[20] = {
    { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },
    { 0, 0, 0 },     { 0, 0, 0 },     { 45, -5, 50 },  { 0, 0, 0 },     { 0, 0, 0 },
    { -6, 0, 90 },   { -6, 0, 90 },   { 44, -6, 40 },  { 0, 0, 0 },     { -100, 0, 0 },
    { 44, -4, 80 },  { 0, 0, 0 },     { 0, 0, 0 },     { 44, -4, 80 },  { -100, 0, 30 },
};

/* Angle for each 4-bit direction mask (read by actor_updatePlayer). */
static const u16 gDirMaskAngles[16] = {
    0x0000, 0xC000, 0x4000, 0xFFFF, 0x8000, 0xA000, 0x6000, 0x8000,
    0x0000, 0xE000, 0x2000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* Sound ids. */
static const u16 gImpactSounds[6] = { 0x2B1, 0x2B5, 0x2B6, 0x2B7, 0x2BD, 0x2BE };

s32 D_80086000 = 0; /* cleared by game_43760.c */

/* Level callbacks run for each object, before and after its update (indexed by the level header's unk49, unk4A). */
static void (*gLevelPreUpdateFuncs[])(Actor *) = { NULL, func_150D1410 };
static void (*gLevelPostUpdateFuncs[])(Actor *) = { NULL, func_150D146C };

/* Per-type update handlers, indexed by the object's type (also run by game/Cutscene/Sequence/sequence.c). */
void (*gActorUpdateHandlers[])(Actor *) = {
    NULL, actor_updatePlayer, actor_updateCommon, func_15053894,
    func_150538CC, func_150548D8, func_15052458, func_150B12FC,
    func_1505371C, func_150B1484, func_150B19E0, func_150B17DC,
    actor_updatePathFollower, func_150536E8, NULL, func_15052EF0,
    func_150B10F0, actor_updateCommon, func_150548E4, func_150C3994,
    func_150C3994, func_150F34F4, NULL, NULL,
    func_150C3574, actor_removeIfDead, func_150E0BE0, actor_removeAtAnimEnd,
    func_15054A94, NULL, weapon_updateTank, actor_updateMount,
    func_150D7928, func_1505528C, NULL, func_150EEDA8,
    actor_updateFollower, weapon_releaseTankCamera, func_150C3230, func_15055B64,
    effect_spinDownActor, actor_updateOverEdge, actor_updateSinkAway,
};


void actor_updateAll(void) {
    Actor *obj;
    s32 i;
    s32 tickResult;
    OSContPad noPad;
    f32 maxStep;
    f32 step;
    s32 j;

    D_800BE9EC = 0;
    gActorFrameCount += 1;
    D_800CC250 += gFrameSteps;
    dc_glbl_CharNum = 0;
    D_800CC2BC = 0;
    collision_trySpawnRecords(level, 0, 0);
    gActorUpdateParity ^= 1;

    for (i = 0; i != 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0) {
            continue;
        }
        obj->stand_on_char = 0;
        obj->unk300 = 0;
        obj->mvmt_control &= 0xFFEE5FFF;
        dc_glbl_CharNum = i;
        if (obj->ypos <= obj->unk38) {
            if (obj->playernumber == 0xFF) {
                actor_remove(obj, 0);
                continue;
            }
            if (gIsMultiplayer == 0) {
                actor_finishDeath(i);
                continue;
            }
        }
        if (obj->unk2FA == 0) {
            continue;
        }
        gCurrentActor = obj;
        obj->unk12++;
        gCurrentActorSlot = i + 1;
        D_800CC2BA = 0;
        tickResult = 0;
        if (gIsMultiplayer == 0 || (tickResult = actor_interleaveUpdate(obj)) != 0) {
            maxStep = 2.0f;
            if (i < D_8008FD8C) {
                obj->mv->being_sniped -= D_800BE9E4;
                if (obj->mv->being_sniped < 0) {
                    obj->mv->being_sniped = 0;
                }
                obj->mv->unk57 = 0;
                if (i < 4) {
                    gCurrentPad = gPadTable[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    gCurrentPad = &noPad;
                }
            }
            if (obj->CameraPtr != NULL) {
                if (obj->CameraPtr->LockedCameraAngle == 999.0f) {
                    D_800CC280 = obj->CameraPtr->CameraAngle * (65536.0f / 360.0f);
                } else {
                    D_800CC280 = obj->CameraPtr->LockedCameraAngle * (65536.0f / 360.0f);
                }
            } else {
                D_800CC280 = 0;
            }
            if (obj->playernumber != 0xFF) {
                zone_updateActor(obj);
            } else if (obj->noshove & 0x400000) {
                zone_applyWarps(obj);
            }
            obj->old_xpos = obj->xpos;
            obj->old_ypos = obj->ypos;
            obj->old_zpos = obj->zpos;
            gFloorMaterialClass = (obj->floorMaterial >> 28) & 7;
            step = gFrameDelta;
            if (gIsMultiplayer != 0 && (D_8008FDBC & 0x100)) {
                maxStep = 5.0f;
            }
            if (maxStep < step) {
                step = maxStep;
            }
            gActorTimeStep = obj->speedup * step;
            gActorStepPercent = gActorTimeStep * 100.0f;
            {
                // matching: a -g3 scope here keeps the lbu after the sh above and lets the lui hoist
                extern u8 gFloorMaterialClass;
                if (gFloorMaterialClass >= 6) {
                    gFloorMaterialClass = 0;
                }
            }
            if (D_800B0DF0->unk49 != 0) {
                gLevelPreUpdateFuncs[D_800B0DF0->unk49](obj);
            }
            if (!(obj->task_attrib & 0x200)) {
                if (gActorUpdateHandlers[obj->type] != NULL) {
                    gActorUpdateHandlers[obj->type](obj);
                }
            }
            if (obj->type != 0) {
                face_update(obj);
                actor_updateAttachedObject(obj, 0, 0);
                if (i < D_8008FD8C && D_800D18A8 == 0) {
                    player_setStat(obj->playernumber, 5, obj->energy);
                }
            }
            obj->unk2FF = 1;
            if (tickResult == 2) {
                D_800BE9E4 = D_800CC2BA;
                gFrameDelta = D_800BE9E4 * 0.5f;
                if (gFrameDelta != 0.0f) {
                    gFrameDeltaInv = 1.0f / gFrameDelta;
                } else {
                    gFrameDeltaInv = 0.0f;
                }
            }
            if (obj->old_xpos != obj->xpos || obj->old_ypos != obj->ypos ||
                obj->old_zpos != obj->zpos) {
                obj->unk300 = 1;
            }
            if (D_800B0DF0->unk4A != 0) {
                gLevelPostUpdateFuncs[D_800B0DF0->unk4A](obj);
            }
        }
    }

    for (j = 0; j < D_8008FD8C; j++) {
        if ((movement_obj)[j].type != 0 && (movement_obj)[j].mv != NULL) {
            if ((movement_obj)[j].unk137 != 0) {
                if (gActorTouchedObject[j] != 0) {
                    db_Object_array[gActorTouchedObject[j] - 1].object_status |= 0x84;
                    gWorldObjectContacts[gActorTouchedObject[j] - 1] |= 1 << j;
                }
                (movement_obj)[j].mv->unk56 = 5;
            } else if ((movement_obj)[j].mv->unk56 != 0) {
                (movement_obj)[j].mv->unk56--;
            }
        }
    }
    actor_updateTypeAnims();
    text_updateContextText();
    gCurrentActor = movement_obj;
    dc_glbl_CharNum = 0;
    gCurrentActorSlot = 0;
}

void func_1504ADD0(void) {
    s32 i;
    Actor *obj;
    OSContPad noPad;

    if (gCutsceneStates[0] != 1) {
        for (i = 0; i < 25; i++) {
            obj = &movement_obj[i];
            if (obj->type == 0) {
                continue;
            }
            dc_glbl_CharNum = i;
            gCurrentActor = obj;
            if (i < D_8008FD8C) {
                if (i < 4) {
                    gCurrentPad = gPadTable[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    gCurrentPad = &noPad;
                }
            }
            switch (obj->character_type) {
            case 0x28:
            case 0x77:
                func_150ED578(obj);
                break;
            }
        }
    }
}

s32 func_1504AEF4(s32 arg0, s32 *arg1) {
    if (arg0 == 0) {
        return 0;
    }
    return 0;
}

void actor_updateAttachedObject(Actor *obj, s32 force, s32 release) {
    s32 pad; // matching: unused slot that places index at its original stack offset
    s32 index;
    s32 type;
    s32 id;

    if (obj->unk92 && (force || !(obj->unk92 >> 15))) {
        id = obj->unk92 & 0x7FFF;
        if ((type = (obj->unk92 >> 12) & 7) == 0) {
            type = func_1504AEF4(id, &index);
            if (type != 0) {
                obj->unk92 = (type << 12) | index;
            }
        }
        index = obj->unk92 & 0xFFF;
        if (type == 3) {
            if (release) {
                db_Object_array[index].attach = 0;
                obj->unk92 = 0;
                return;
            }
            db_Object_array[index].attach = (obj - movement_obj) + 100;
            if (force) {
                db_Object_array[index].attach = 0;
                return;
            }
            db_Object_array[index].object_status &= ~0x60;
            db_Object_array[index].object_status |= 0x20;
            db_Object_array[index].x = obj->xpos;
            db_Object_array[index].y = obj->ypos + obj->unk90;
            db_Object_array[index].z = obj->zpos;
        }
    }
}


f32 actor_updateSwim(Actor *obj, f32 arg1) {
    s32 belowLevel;
    f32 targetPitch;
    f32 delta;
    f32 result;
    s8 stickX;
    s32 stickY;
    f32 step;
    f32 out0;
    f32 out1;
    f32 out2;
    f32 out3;
    f32 out4;
    f32 excess;

    result = 0.0f;
    obj->mv->unk68 = 0;
    obj->unk110 = 0;
    if (obj->stunned == 0 && obj->energy != 0 && obj->mv->AimState == 0) {
        stickX = gCurrentPad->stick_x;
        stickY = gCurrentPad->stick_y;
    } else {
        stickX = 0;
        stickY = 0;
    }
    if (obj->mv->unk22 > 100) {
        stickY = 80;
    }
    delta = (obj->ypos - ((obj->waterHeight - 60.0f) - 170.0f)) * 2.0f;
    if ((obj->mv->water_attribs & 1) || obj->mv->unk21 != 0) {
        obj->underwaterTime = 0;
        if (delta < 400.0f) {
            // matching: loading 8.0f into step first gives the target's mul.s operand order
            step = 8.0f;
            step *= gActorTimeStep;
            if (stickY > 10) {
                stickY = 10;
            }
            if (delta + step > 400.0f) {
                step = 400.0f - delta;
            }
            obj->ypos += step;
        }
    }
    if (obj->inwater == 1) {
        obj->unk1CC = obj->ypos;
        if (obj->waterHeight == -10000.0f) {
            actor_leaveWater(obj);
            return 0.0f;
        }
        if (stickY != 0 || ((obj->waterHeight - 60.0f) - 175.0f) < obj->ypos) {
            targetPitch = stickY * 2.25f;
        } else {
            targetPitch = obj->vtilt;
        }
        if (obj->mv->unk21 != 0) {
            targetPitch = 0.0f;
        }
        actor_applyWaterCurrent(obj);
        if ((obj->stunned != 0 && (obj->energy != 0 || obj->yspd >= 60.0f)) || obj->animseq.uh == 0x262) {
            return 0.0f;
        }
        if (delta > 0.0f && obj->animseq.uh != 0x22) {
            if (delta > 300.0f) {
                delta = 300.0f;
            }
            obj->ypos += delta * arg1 * 0.002997f;
        }
    } else if ((obj->ygrav < 0.0f && obj->yspd > 0.0f) || (obj->ygrav > 0.0f && obj->yspd < 0.0f)) {
        obj->ygrav = 0.0f;
        obj->yspd = 0.0f;
    }
    if (obj->inwater != 1 || obj->shad >= 10.0f || obj->ypos < (obj->waterHeight - 60.0f) + 20.0f) {
        if (obj->inwater == 1) {
            if ((obj->waterHeight - 50.0f < obj->aim.y || (obj->mv->water_attribs & 1)) && obj->energy != 0) {
                if (obj->underwaterTime > 30) {
                    if (obj->underwaterTime > 1500) {
                        actor_playSound(0x1A7, obj);
                    } else if (obj->underwaterTime > 720) {
                        actor_playSound(0x168, obj);
                    }
                }
                obj->underwaterTime = 0;
            } else {
                obj->underwaterTime += D_800BE9E4;
                if (obj->underwaterTime > 1560 || (obj->energy == 0 && obj->mv->death_State == 0)) {
                    obj->underwaterTime -= 60;
                    if (obj->energy >= 2) {
                        if (obj->energy == 4) {
                            actor_playSound(0x499, obj);
                        }
                        obj->energy--;
                    } else {
                        obj->energy = 0;
                        if (obj->stunned != 0) {
                            obj->ygrav = 0.0f;
                            actor_kill(obj, 4);
                        } else {
                            actor_kill(obj, 3);
                        }
                    }
                } else if (obj->underwaterTime > 1440 && obj->underwaterTime < 1500) {
                    obj->underwaterTime += 120;
                    if (obj->energy < 4) {
                        actor_playSound(0x499, obj);
                    }
                }
            }
            if ((obj->yspd != 0.0f || obj->ygrav != 0.0f) && obj->ygrav < 0.0f && obj->yspd > 0.0f) {
                obj->ygrav = 0.0f;
                obj->yspd = 0.0f;
            }
            if (((obj->waterHeight - 60.0f) - 175.0f) < obj->ypos && obj->animseq.uh != 0x28) {
                if (obj->mv->water_up < 2.5f) {
                    obj->mv->water_up += 0.05f;
                }
            } else {
                obj->mv->water_up = 0.0f;
            }
            obj->ypos += obj->mv->water_up * gActorTimeStep;
            excess = obj->ypos - ((obj->waterHeight - 60.0f) + 14.0f);
            if (excess > 0.0f) {
                excess *= 0.65f;
                obj->ypos -= excess;
                obj->mv->water_up = 0.0f;
            }
            if (((obj->waterHeight - 60.0f) - 75.0f) < obj->ypos) {
                targetPitch -= 30.0f;
            }
        }
        if (obj->animseq.uh == 0x22 && targetPitch < -20.0f) {
            targetPitch = -20.0f;
        }
        if (obj->shad < 40.0f) {
            collision_getTriSlope(obj->floorTriVerts, obj->floorTri.v, &out0, &out1, &out2, &out3, &out4);
            out3 = actor_slopeToAngles(obj, out3, out4, &out4, 180.0f);
            if (out4 < obj->vtilt) {
                obj->vtilt += (out4 - obj->vtilt) * 0.2f;
            }
        }
        obj->unkBC = targetPitch - obj->vtilt;
        if (2.7f * gActorTimeStep > fabsf(obj->unkBC)) {
            obj->vtilt = targetPitch;
        } else {
            if (obj->unkBC > 2.7f) {
                obj->unkBC = 2.7f;
            }
            if (obj->unkBC < -2.7f) {
                obj->unkBC = -2.7f;
            }
            obj->vtilt += obj->unkBC * gActorTimeStep;
        }
        if (obj->vtilt < -90.0f) {
            obj->vtilt = -90.0f;
        }
        if (obj->vtilt > 90.0f) {
            obj->vtilt = 90.0f;
        }
        if (obj->mv->unk21 == 0) {
            if ((stickX + 8) / 18 != 0) {
                obj->angle -= ((((stickX + 8) / 18) * gActorStepPercent) << 8) / 100;
                obj->unk7E = 0x17;
            }
            result = obj->wantspeed;
        }
        obj->count2 = 1;
    } else {
        actor_leaveWater(obj);
        if (obj->speed > 16.0f) {
            obj->speed = 16.0f;
        }
        result = obj->speed;
    }
    if (obj->mv->ladder != 0) {
        obj->ypos = obj->old_ypos;
        belowLevel = obj->waterHeight != -10000.0f && obj->ypos < obj->waterHeight - 60.0f;
        if (!belowLevel) {
            obj->inwater = 0;
        }
    }
    return result;
}

void func_1504BA38(Actor *obj) {
    const Struct80099140 *entry;
    s8 unk2;
    s32 unk1;

    if (obj->shad > 10.0f) {
        obj->unkCE = 0;
        unk1 = entry->unk1; // matching: dead use of entry here sets the register allocation
        return;
    }
    entry = &D_80099140[obj->floorMaterial & 0x1F];
    unk2 = entry->unk2;
    if (unk2 != 0 && obj->unkAA == 0) {
        obj->unkAA = unk2;
    }
    unk1 = entry->unk1;
    if (unk1 == 0) {
        obj->unkCE = entry->unk0;
        return;
    }
    obj->unkCE = unk1 * 0.1000000015f * (entry->unk0 - obj->speed);
    if (unk2) { // matching: keeps unk2 live to the end (in a1 rather than v0)
    }
}

void func_1504BAF0(Actor *obj) {
    s16 index;
    Actor *other;

    index = obj->lifting - 100;
    if (index >= 0) {
        other = &movement_obj[index];
        if (other->carried >= 100 && (other->character_type == 168 || other->character_type == 169)) {
            gEventParam = 3;
            func_1507490C();
        }
    }
    event_dropHeldObject();
}

void func_1504BB88(Actor *obj) {
    if (obj->task_attrib & 0x10) {
        obj->count5 = 0xFF;
        obj->norun = 0xFF;
        if (obj->type == 1) {
            actor_playAnim(obj, 0xD6, 1.15f, 3.0f, 0.0f, 0.0f, 0);
        }
        obj->task_attrib &= ~0x10;
    }

    if (obj->task_attrib & 2) {
        if (obj->lifting != 0) {
            func_1504BAF0(obj);
        }
        obj->task_attrib &= ~2;
    }
}

void actor_tickTimers(Actor *obj) {
    if (obj->norun != 0 && obj->norun < 250) {
        if (obj->norun <= gFrameSteps) {
            obj->norun = 0;
        } else {
            obj->norun -= gFrameSteps;
        }
    }
    if (obj->unk228) { // matching: "!= 0" here changes the register allocation (a2 -> a1)
        if (obj->unk228 <= gFrameSteps) {
            obj->unk228 = 0;
        } else {
            obj->unk228 -= gFrameSteps;
        }
    }
    if (obj->unkAE != 0) {
        obj->unkAE--;
    }
    if (obj->nojump != 0xFF) {
        if (obj->nojump <= gFrameSteps) {
            obj->nojump = 0;
        } else {
            obj->nojump -= gFrameSteps;
        }
    }
    if (obj->imune != 0xFF) {
        if (obj->imune <= gFrameSteps) {
            obj->imune = 0;
        } else {
            obj->imune -= gFrameSteps;
        }
    }
    if (obj->mv->chased > 0) {
        obj->mv->chased -= gFrameSteps;
    }
    if (obj->mv->chasing > 0) {
        obj->mv->chasing -= gFrameSteps;
    }
    if (obj->mv->camjolt > 0) {
        obj->mv->camjolt -= gFrameSteps;
    }
    if (obj->ivehitit != 0) {
        obj->ivehitit--;
    }
    if (obj->unk1D0 > 0) {
        obj->unk1D0 -= gFrameSteps;
    }
    if (obj->mv->side > 0) {
        obj->mv->side -= gFrameSteps;
    }
    if (obj->mv->save_done != 0) {
        obj->mv->save_done--;
    }

    obj->mv->use_cam_butt >>= 1;

    if (obj->mv->ReloadTimer != 0) {
        if (obj->mv->ReloadTimer > D_800BE9E4) {
            obj->mv->ReloadTimer -= D_800BE9E4;
        } else {
            obj->mv->ReloadTimer = 0;
        }
    }
    if (obj->mv->unk1A0 != 0) {
        if (obj->mv->unk1A0 > D_800BE9E4) {
            obj->mv->unk1A0 -= D_800BE9E4;
        } else {
            obj->mv->unk1A0 = 0;
        }
    }
}

void actor_updatePissed(Actor *obj, u16 *arg1, f32 *arg2, s8 *arg3) {
    s32 pad[2]; // matching: the frame has two unused words
    f32 angle;

    angle = obj->mv->pissed_cycle * 0.024543693f; // pi / 128: pissed_cycle counts 256ths of a turn
    gWaistTiltDamping = 16.0f;
    if (obj->mv->pissed_hangover != 0 && obj->mv->pissed_hangover == 0) { // never true, but compiled in
        face_resetDefaultExpression(obj);
        obj->mv->pissed_up = 0;
    }
    if (obj->mv->pissed_walk == 0) {
        obj->mv->pissed_walk = 0xA6;
    }
    obj->mv->turn = 5;
    if (arg1) { // matching: this and the empty if below keep arg1 live in a1 across the calls
    }
    if (*arg2 > 4.0f && obj->animseq.uh == 0xAA) {
        obj->mv->turn = 1;
        obj->mv->pissed_angle = *arg1;
    }
    math_sin(angle);
    if (arg1) {
    }
    obj->mv->pissed_cycle += 2;
    if (obj->mv->pissed_hangover != 0) {
        obj->unkAA = 25;
    } else if (obj->animseq.uh == 0xA7) {
        obj->unkAA = 50;
    } else {
        obj->unkAA = 32;
    }
    obj->unk54 *= 0.5f;
    if (obj->animseq.uh == 0x2C || obj->animseq.uh == 0xE0) {
        *arg3 = gCurrentPad->stick_y >> 1;
    } else {
        if (!(math_random() & 0x3F)) {
            obj->mv->pssd_waist_wobble = (math_random() % 25000U) - 12500;
        }
        gWaistTiltTarget = obj->mv->pssd_waist_wobble;
        if (obj->mv->pissed_hangover != 0) {
            gWaistTiltTarget /= 3;
        }
    }
    if (gCutsceneStates[0] == 0 && obj->mv->pissed_hangover == 0) {
        obj->headEffectTimer = 30;
        obj->headEffect = 0;
    }
}

s32 actor_updateIdleAnim(void) {
    if (gCurrentActor->mv->idle.active != 1) {
        gCurrentActor->mv->idle.holdFrames = 0;
        anim_updateIdle();
    }
    return 999;
}

s32 actor_getDuckWalkAnim(void) {
    if (level == 27 || level == 30) {
        return 395;
    }
    return 27;
}

void actor_updateWalkAnim(Actor *obj, s32 arg1, f32 arg2) {
    f32 blend;
    f32 start;
    f32 vel;
    s32 pressed;
    f32 speed;
    s32 anim;
    ActorCtl *ext;
    f32 diff;

    if (gWaistTiltTarget == 0 && (ext = obj->mv)->pssd_waist_tilt >= -29 && ext->pssd_waist_tilt < 30) {
        ext->pssd_waist_tilt = 0;
    } else {
        ext = obj->mv;
        // matching: computed in two statements to keep the target's float register order
        diff = gWaistTiltTarget - ext->pssd_waist_tilt;
        diff = (gActorTimeStep / gWaistTiltDamping) * diff;
        ext->pssd_waist_tilt += (s16)diff;
    }
    ext = obj->mv;
    if (ext->unk6B != 0) {
        return;
    }
    anim = 999;
    if (obj->attach_char != 0 && obj->stunned == 0) {
        actor_syncAnimToLinked(obj);
        return;
    }
    if (obj->count3 == 0 && obj->stunned == 0 && obj->inwater == 0) {
        if (obj->count5 == 0 && ext->weapon_status != 3) {
            obj->count2 = 1;
            obj->unk100 &= 0xF3;
            D_800CC2BC |= 0x80;
            speed = 1.0f;
            start = 0.0f;
            blend = 4.0f;
            vel = obj->speed * 0.4000000060f + arg2 * 0.6000000238f;
            ext = obj->mv;
            if (ext->bitten > 0 || ext->transport != 0 || obj->lifting >= 100 || arg1 != 0 ||
                (ext->walk_attrib & 3) || ext->rt_context_b != 0 || ext->injured != 0 ||
                ext->pissed_up > 0 || ext->env_suit != 0) {
                if (actor_updateMoveAnim(obj, vel, vel, arg1, arg2) != 0) {
                    return;
                }
                ext = obj->mv;
            }
            pressed = gCurrentPad->button & Z_TRIG;
            if (pressed == 0 || D_800CC2B1 != 0 || ext->rt_context_b != 0 || (ext->transport & 0xF) == 1) {
                if (obj->speed > 4.0f) {
                    ext->dwalk = 0;
                    ext = obj->mv;
                }
                pressed = ext->transport; // matching: reusing pressed for this value sets the register allocation
                if ((ext->transport == 1 || pressed == 0x81) && vel > 1.0f) {
                    arg2 = 10.0f;
                    anim = 0x302;
                    speed = obj->speed / 7.0f + 0.2000000030f;
                    if (obj->speed > 23.0f) {
                        anim = 0x303;
                        speed = obj->speed / 23.0f + 0.3000000119f;
                    }
                } else if (ext->chased > 0 && vel >= 5.0f) {
                    anim = 0x2B;
                    speed = vel / 12.0f;
                } else if (ext->chasing > 0 && vel >= 5.0f) {
                    anim = 0xDF;
                    speed = vel / 12.0f;
                } else if (obj->waterHeight != -10000.0f && obj->ypos < obj->waterHeight &&
                           obj->speed >= 1.0f && ext->transport == 0) {
                    speed = vel / 18.0f + 0.2000000030f;
                    anim = 0x8B;
                    obj->unkAA = 0x4B;
                } else if (vel > 14.0f) {
                    anim = (level != 4) ? 2 : 0x1B2;
                    speed = vel / 23.0f + 0.8000000119f;
                } else if (vel > 6.0f || (arg2 < 1.0f && obj->speed > 1.0f)) {
                    anim = 1;
                    speed = vel * 0.125f + 0.3000000119f;
                } else if (vel > 1.0f) {
                    blend = 15.0f;
                    anim = (obj->character_type == 0x80) ? 1 : 0;
                    speed = vel * 0.125f + 0.1000000015f;
                } else {
                    anim = actor_updateIdleAnim();
                }
                if (obj->shad > 60.0f && obj->mv->transport == 0) {
                    speed = 0.5f;
                    anim = 0x38;
                    obj->unk100 |= 0x10;
                }
                if (obj->speed > 18.0f && obj->mv->weapon_status != 1 && obj->mv->weapon_status != 3 && arg2 == 0.0f &&
                    obj->shad == 0.0f && gFloorMaterialClass == 0) {
                    speed = 0.5f;
                    anim = 0x21;
                    blend = 3.0f;
                    obj->norun = 8;
                    obj->count5 = 0x11;
                }
            } else if (pressed != 0) {
                if (obj->speed < 3.0f) {
                    ext->dwalk = 1;
                }
                if (obj->stickspeed > 2.599999905f && (ext = obj->mv)->dwalk != 0) {
                    ext->duck_tilt = 1;
                    obj->unkAA = 0x28;
                    blend = 5.0f;
                    speed = obj->stickspeed / 15.0f + 0.5f;
                    anim = actor_getDuckWalkAnim();
                } else {
                    blend = 6.0f;
                    speed = 1.5f;
                    anim = 0x16;
                }
            }
        }
        if (anim != 999) {
            actor_playAnim(obj, anim, speed, blend, start, 0.0f, 0);
        }
    } else if (obj->animseq.uh == 0x21 && D_800CC2B1 == 0 && obj->stickspeed == 0.0f &&
               (gCurrentPad->button & Z_TRIG)) {
        actor_playAnim(obj, 0x16, 1.5f, 6.0f, 0.0f, 0.0f, 0);
    }
}

void actor_updateButtonTimers(Actor *obj) {
    gActorPressedButtons = obj->mv->pressedButtons;
    if (obj->mv->buttonsTimer > 0) {
        obj->mv->buttonsTimer -= gFrameSteps;
    } else {
        obj->mv->buttons = 0;
    }
    if (obj->mv->pressedButtonsTimer > 0) {
        obj->mv->pressedButtonsTimer -= gFrameSteps;
    } else {
        obj->mv->pressedButtons = 0;
    }
}

s32 actor_interleaveUpdate(Actor *obj) {
    ActorCtl *ext;

    ext = obj->mv;
    if (ext == NULL) {
        return 1;
    }
    if (obj->ob_matrix_ptr != NULL || level == 0x33 || obj->type == 0x25) {
        ext->skippedTime = 0;
        return 1;
    }
    if ((obj->playernumber & 1) == gActorUpdateParity) {
        if (ext->skippedTime != 0) {
            D_800CC2BA = D_800BE9E4;
            D_800BE9E4 += obj->mv->skippedTime;
            gFrameDelta = D_800BE9E4 * 0.5f;
            if (gFrameDelta != 0.0f) {
                gFrameDeltaInv = 1.0f / gFrameDelta;
            } else {
                gFrameDeltaInv = 0.0f;
            }
            obj->mv->skippedTime = 0;
            return 2;
        }
        return 1;
    }
    ext->skippedTime += D_800BE9E4;
    return 0;
}

void func_1504C9E4(Actor *obj, s8 target, u8 mode) {
    u8 step;
    s32 delta;
    s32 saved;

    delta = (u8)(obj->unk1D1 - target);
    if (delta != 0) {
        step = 3;
        if (mode == 16) {
            step = 6;
        }
        saved = delta;
        if (delta >= 128) {
            obj->unk1D1 += step;
        } else {
            obj->unk1D1 -= step;
        }
        // stop at the target once the difference changes sign
        delta = saved; // matching: the round trip through saved keeps a separate copy of delta in a3
        delta ^= obj->unk1D1 - target;
        if ((u8)delta & 0x80) {
            obj->unk1D1 = target;
        }
    }
}


void func_1504CA60(Actor *actor) {
    f32 range;
    f32 height;

    height = actor->floorY + 20.0f;
    actor->unk1CC = actor->ypos;
    range = 18.0f;
    if (actor->shad > 30.0f) {
        range = 80.0f;
    }
    if (actor->count3 != 0) {
        range = 80.0f;
        height -= 30.0f;
        if ((actor->shad < 5.0f) && (actor->yspd < 0.0f)) {
            actor->count3 = 0;
            actor->yspd = 45.0f;
        }
    } else if (actor->shad < 20.0f) {
        if ((math_randomU8() & 0x3F) == 0) {
            actor->yspd = 25.0f;
        }
    }
    func_15058EA4(actor, height, 3.8f, height, -3.5f, 80.0f, -range);
    actor->count2 = 1;
}

// matching: second names for D_800CC280/D_800CC2BC (aliases in undefined_syms.us.txt). As separate
// symbols IDO neither shares their %hi nor reuses loads across them; a few uses need that.
extern s32 D_800cc280;
extern s32 D_800cc2bc;
// matching: a declaration-only block scope (no storage). IDO opens a lexical block for it, which changes
// register allocation/scheduling of the surrounding statements; no storage-free idiomatic form matches.
#define BLK typedef s32 blk_t

/*
 * Per-frame update of a player-controlled actor: reads (or overrides) the controller, turns stick input
 * into a move angle/speed, then runs turning, swimming, climbing, pushing, floor collision and water entry.
 */
void actor_updatePlayer(Actor *actor) {
    u16 prevFacing;
    u16 moveAngle;
    u8 pad1; // matching: unused; keeps the original stack layout
    u8 turn;
    u8 turnRate;
    f32 moveSpeed;
    s16 angleDelta;
    u16 heading;
    s32 diff;
    f32 surfaceDelta;
    s8 turnStep;
    s8 pad2; // matching: unused; keeps the original stack layout
    u8 pad11; // matching: unused; keeps the original stack layout
    u8 pad3; // matching: unused; keeps the original stack layout
    u8 pad4; // matching: unused; keeps the original stack layout
    s32 variant;
    s32 pushAnim;
    s8 lean;
    f32 startX;
    f32 startZ;
    s32 buttons;
    OSContPad savedPad;
    s32 padOverridden;
    f32 probeTop;
    s32 mode;
    TriggerZone *climb;
    TriggerZone *climbSpot;
    f32 climbTop;
    u8 pad5; // matching: unused; keeps the original stack layout
    f32 animSpeed;
    f32 stickY;
    u32 chance;
    f32 steer;
    f32 climbRate;
    f32 swimSpeed;
    s32 turnSize;
    u16 lockAngle;
    u16 minOffset;
    u16 pad6; // matching: unused; keeps the original stack layout
    s32 anim;
    u8 turnLeft;
    f32 phase;
    u16 deflect;
    u16 quadrant;
    f32 pitchInput;
    f32 friction;
    Actor *target;
    f32 tilt;
    f32 steerDiv;
    f32 offsetX;
    f32 offsetZ;
    f32 offsetY;
    f32 pushX;
    f32 pushZ;
    f32 pushY;
    s32 climbAnim;
    f32 climbAnimSpeed;
    u16 moveDelta;
    u16 pad7; // matching: unused; keeps the original stack layout
    s16 stickX;
    u16 pad8; // matching: unused; keeps the original stack layout
    u16 pad9; // matching: unused; keeps the original stack layout
    u16 offset;
    u16 maxOffset;
    u16 centerAngle;
    s16 pad10; // matching: unused; keeps the original stack layout
    s32 climbIndex;
    s32 physics;
    f32 scratch;
    f32 nudgeX;
    f32 nudgeZ;
    f32 nudgeY;
    f32 follow;
    u8 carry;
    f32 pushScale;
    f32 pushAccel;
    f32 floorY;
    f32 snapRange;
    s32 dirMask;
    s32 lockDiff;
    s32 wantAngle;
    f32 climbInput;
    Rope *path;
    u16 dirAngle;
    u16 otherFacing;
    f32 tiltDeg;
    f32 turnLean;
    f32 prevSteer;
    s16 ledgeAngle;
    f32 height;
    f32 ledgeY;
    f32 splashSpeed;
    s32 sound;
    f32 climbSpeed;
    f32 heightDiff;

    moveSpeed = 0.0f;
    pushAnim = 0;
    lean = 0;
    gActorNewButtons = gButtonsPressed[dc_glbl_CharNum];
    gActorHeldButtons = gCurrentPad->button;
    D_800CC2B1 = 0;
    gWaistTiltTarget = 0;
    gWaistTiltDamping = 4.0f;
    prevFacing = actor->angle;
    padOverridden = 0;
    savedPad = *gCurrentPad;
    D_800CC2BC = 1;
    actor->mv->unk1B3 = 0;
    if (actor->mv->bot != 0) {
        if (actor->stunned == 0) {
            actor_updateChaseAi(actor);
        }
        savedPad = *gCurrentPad;
        gCurrentPad->stick_x = actor->mv->stick_x;
        gCurrentPad->stick_y = actor->mv->stick_y;
        gCurrentPad->button = actor->mv->buttons;
        gActorPressedButtons = actor->mv->pressedButtons;
        buttons = actor->mv->buttons;
        if (actor->CameraPtr == NULL) {
            camera_updateObjectAim(actor, gCurrentPad);
            camera_publishAimRay(actor, gCurrentPad);
        }
        actor_updateButtonTimers(actor);
    } else {
        actor->mv->buttons = gCurrentPad->button;
        gActorPressedButtons = gButtonsPressed[dc_glbl_CharNum];
        buttons = actor->mv->buttons;
    }
    if ((gScreenLayout >= dc_glbl_CharNum) && (gIrisTracking[dc_glbl_CharNum] != 0) && (gIrisClosures[dc_glbl_CharNum] > 0.75f) && (gFrontEndState != 2)) {
        gCurrentPad->stick_x = 0;
        gCurrentPad->stick_y = 0;
        gCurrentPad->button = 0;
        gActorPressedButtons = 0;
        padOverridden = 1;
        D_800CC2BC |= 2;
    }
    if ((actor->stunned != 0) && (actor->energy != 0) && ((((actor->mv->break_stun & 1) != 0) && (gActorPressedButtons & 0x8000)) || ((actor->mv->break_stun & 2) && (actor_getStickDeflection(gCurrentPad) > 15.0f)))) {
        actor->unk10C = 0;
        actor->stunnednum = 3;
        actor_updateStunned(actor, 0.05f);
    }
    if (gIsMultiplayer != 0) {
        if (gFrameSteps < actor->mv->unk1B1) {
            actor->mv->unk1B1 -= gFrameSteps;
            if (actor->fogMode != 4) {
                actor_startFogPulse(actor, 0xFF, 0xFF, 0xFF, 0xBF, 4);
            }
        } else if (actor->fogMode == 4) {
            actor->mv->unk1B1 = 0;
            actor_fadeFogPulse(actor);
        }
        if (((level == 0x2D) || (level == 0x30) || (level == 0x34) || (level == 0x1A) || (level == 0x3F)) && (gActorPressedButtons & 4) && !(gActorPressedButtons & 0xC000) && ((s32) actor->shad == 0) && (actor->yspd < 0.0f) && (actor->norun == 0) && (actor->lifting == 0) && (actor->mv->GrenadeThrowState == 0) && (actor->mv->rt_context_b == 0) && ((actor->mv->context_b & ~0x80) != 3) && (actor->mv->numGrenades != 0)) {
            if ((actor->character_type == 0x3B) || (level == 0x1A) || (level == 0x3F)) {
                actor->mv->GrenadeThrowState = 5;
            } else {
                actor->mv->GrenadeThrowState = 1;
            }
        }
    }
    actor->mv->old_keys = gCurrentPad->button;
    if (D_800C3638 != 0) {
        gCurrentPad->stick_x = 0;
        gCurrentPad->stick_y = 0;
        gCurrentPad->button = 0;
        gActorPressedButtons = 0;
        D_800CC2BC |= 4;
    }
    anim_updateIdleTimer(actor);
    func_1504BB88(actor);
    func_1504BA38(actor);
    if ((actor->unk228 != 0) || (actor->unk2F8 & 0x10)) {
        gCurrentPad->stick_x = 0;
        gCurrentPad->stick_y = 0;
        gCurrentPad->button = 0;
        gActorPressedButtons = 0;
        padOverridden = 1;
        D_800cc2bc |= 8;
    }

    if (actor->energy == 0) {
        actor->norun = 0x14;
        if ((actor->stunned == 0) && (actor->mv->death_State == 0)) {
            if ((actor->floorMaterial & 0x1F) == 0xE) {
                actor_kill(actor, 0xD);
            } else if ((actor->floorMaterial & 0x1F) == 0x13) {
                actor_kill(actor, 0xD);
            } else {
                actor_kill(actor, 0);
            }

        }
    }
    if (actor->mv->rt_context_b != 0x1B) {
        if (((actor->mv->AimState == 0) || (actor->mv->rt_context_b == 3)) && (actor->mv->rt_context_b != 0x12) && (actor->mv->rt_context_b != 2) && (actor->mv->rt_context_b != 0x13) && (actor->mv->rt_context_b != 0xA) && (actor->mv->rt_context_b != 0x34)) {
            mode = actor->mv->rt_context_b;
            D_800CC2BC |= 0x10;
            moveAngle = actor_getDirectionAngle(gCurrentPad->stick_x, gCurrentPad->stick_y, 0);
            moveSpeed = actor_getStickDeflection(gCurrentPad);
            physics = actor->mv->transport;
            if (physics == 2) {
                angleDelta = gCurrentPad->stick_x * gActorStepPercent;
                pitchInput = gCurrentPad->stick_y;
                pitchInput *= 0.6f;
                if (pitchInput > 45.0f) {
                    pitchInput = 45.0f;
                }
                if (pitchInput < -35.0f) {
                    pitchInput = -35.0f;
                }
                if (actor->mv->unk50 > 0) {
                    actor->mv->unk50 -= gFrameSteps;
                    angleDelta = (angleDelta / 3);
                    actor->nojump = 6;
                    pitchInput -= 60.0f;
                } else if ((actor->shad > 100.0f) || (actor->count5 != 0)) {
                    angleDelta = (angleDelta / 2);
                }
                moveSpeed = 135.0f + pitchInput;
                actor->intelhitroute = 0x29;
                if (actor->mv->bot == 0) {
                    if (actor->norun == 0) {
                        actor->angle -= angleDelta / 9;
                    } else {
                        moveSpeed = 0.0f;
                    }
                    mp_clampRacerAngle(dc_glbl_CharNum, actor);
                    moveAngle = actor->angle - D_800CC280;
                }
                physics = actor->mv->transport;
            }
            if (physics == 0x81) {
                centerAngle = 0x6000;
                angleDelta = gCurrentPad->stick_x * gActorStepPercent;
                actor->angle -= angleDelta / 8;
                if (level == 6) {
                    moveSpeed = 42.0f;
                    centerAngle = actor_getDirectionAngle(2540.0f - actor->xpos, actor->zpos - -160.0f, 0);
                    maxOffset = 0xF000;
                    minOffset = 0x8000;
                } else {
                    maxOffset = 0xB000;
                    minOffset = 0x2000;
                    moveSpeed = 65.0f;
                }
                offset = (actor->angle - centerAngle);
                if (maxOffset < offset) {
                    actor->angle = centerAngle + maxOffset;
                }
                if (offset < minOffset) {
                    actor->angle = centerAngle + minOffset;
                }
                moveAngle = actor->angle - D_800CC280;
                actor->fangle = actor->angle;
                { BLK; actor->langle = actor->angle; }
            }
            if (actor->mv->SniperState != 0) {
                moveSpeed = 0.1f;
                actor->langle = (s32) (actor->mv->aimYaw * 182.04445f);
                moveAngle = actor->langle - D_800CC280;
                if (actor->mv->bot != 0) {
                    actor->angle = actor->langle;
                }
                scratch = actor->mv->aimPitch;
                if (scratch > 180.0f) {
                    scratch -= 360.0f;
                }
                scratch *= 0.91022223f;
                lean = scratch;
                if ((gIsMultiplayer != 0) && ((actor->character_type == 0x3B) || (actor->character_type == 0x80)) && ((mode == 0x18) || (mode == 0x41) || (mode == 0x23) || (mode == 0x24)) && (gCurrentPad->button & 0x2000) && ((actor->mv->rayHit.hitType == 3) || (actor->mv->rayHit.hitType == 2))) {
                    chance = 0;
                    if (actor->mv->unkAF < 0x3C) {
                        chance = 0x30;
                    }
                    if (sound_isActorSound1Playing(actor) == 0) {
                            if (chance >= (math_randomU8() & 0x5F)) {
                            if (actor->character_type == 0x80) {
                                if ((mode == 0x23) && !(math_randomU8() & 7)) {
                                    actor_playSound(0x586, actor);
                                } else {
                                    variant = (mode == 0x23) ? 4 : 5;
                                    actor_playSound(sound_pickVariant(0x574, variant), actor);
                                }
                            } else if ((gCurrentPad->button & 8) && !(math_randomU8() & 1)) {
                                actor_playSound(0x532, actor);
                            } else {
                                if ((actor->mv->rayHit.hitType == 3) && (actor->mv->rayHit.part == 0xA) && !(math_randomU8() & 3)) {
                                    actor_playSound(0x533, actor);
                                } else {
                                    actor_playSound((math_randomU8() & 3) + 0x534, actor);
                                }
                            }
                        }
                    }
                    actor->mv->unkAF = 0x78;
                }
                if (actor->mv->unkAF != 0) {
                    if (D_800BE9E4 < actor->mv->unkAF) {
                        actor->mv->unkAF -= D_800BE9E4;
                    } else {
                        actor->mv->unkAF = 0;
                    }
                }
                { BLK; }
            } else {
                actor->mv->unkAF = 0;
            }
            if (actor->mv->lockout > 0) {
                actor->mv->lockout -= gActorStepPercent;
                moveAngle = actor->angle - D_800CC280;
            }
            if (actor->mv->bitten > 0) {
                actor->mv->bitten -= gActorStepPercent;
                if (gActorPressedButtons & 0xC000) {
                    if (actor->shad == 0.0f) {
                        gEventParam = 0xFF030170;
                        actor->speed *= 0.4f;
                        actor->yspd = 10.0f;
                        actor->ygrav = 5.0f;
                    }
                    actor->mv->bitten -= 0x190;
                }
                if (actor->norun == 0) {
                    phase = ((D_800CC250 * 0xC) & 0xFF) * 0.024543693f;
                    if (moveSpeed < 5.0f) {
                        moveAngle = actor->angle - D_800CC280;
                    }
                    moveSpeed = 28.0f;
                    actor->mv->turn = 4;
                    scratch = math_sin(phase) * 10240.0f;
                    actor->mv->pissed_cycle += 0x10;
                    actor->mv->pissed_angle = scratch;
                }
            } else {} // matching: the empty else changes block layout
            if ((actor->animseq.uh == 0xBF) || (actor->animseq.uh == 0xC1)) {
                lean = (gCurrentPad->stick_y / 2);
            }
            if (actor->mv->rt_context_b == 0x38) {
                lean -= actor->mv->gunRecoil >> 8;
            }
            { BLK; }
        } else {
            moveAngle = 0;
            moveSpeed = 0.0f;
            D_800cc2bc |= 0x20;
            if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x12) || (actor->mv->rt_context_b == 0x13) || (actor->mv->rt_context_b == 0x10) || (actor->mv->rt_context_b == 0xA) || (actor->mv->rt_context_b == 0x34) || ((gCurrentPad->button & 0x10) && (actor->mv->rt_context_b == 0x16)) || (actor->mv->rt_context_b == 9) || (actor->mv->rt_context_b == 0x38) || (actor->mv->rt_context_b == 0x39) || (actor->mv->rt_context_b == 0x37) || (actor->mv->rt_context_b == 0x3B) || (actor->mv->rt_context_b == 0x15) || (actor->mv->rt_context_b == 0x26) || (actor->mv->rt_context_b == 0x3A)) {
                if ((actor->mv->rt_context_b == 0x10) || (actor->mv->rt_context_b == 0x14) || (actor->mv->rt_context_b == 0x3F)) {
                    lean = (gCurrentPad->stick_y / 2);
                }
                if ((actor->mv->rt_context_b == 0xC) || (actor->mv->rt_context_b == 0x16) || (actor->mv->rt_context_b == 0x15) || (actor->mv->rt_context_b == 0x38) || (actor->mv->rt_context_b == 0x39) || (actor->mv->rt_context_b == 0x37) || (actor->mv->rt_context_b == 0x3B) || (actor->mv->rt_context_b == 0x12) || (actor->mv->rt_context_b == 9)) {
                    tilt = actor->mv->aimPitch;
                    if (tilt > 180.0f) {
                        tilt -= 360.0f;
                    }
                    tilt *= 0.91022223f;
                    lean = tilt;
                    if (actor->mv->rt_context_b == 0x38) {
                        lean -= actor->mv->gunRecoil >> 8;
                    }
                }
                if ((actor->mv->rt_context_b != 9) && (actor->mv->rt_context_b != 0x38) && (actor->mv->rt_context_b != 0x39) && (actor->mv->rt_context_b != 0x37) && (actor->mv->rt_context_b != 0x3B) && (actor->mv->rt_context_b != 0x15) && (actor->mv->rt_context_b != 0x26) && (actor->mv->rt_context_b != 0x3A)) {
                    actor->langle = (s32) ((actor->mv->aimYaw + 90.0f) * 182.04445f);
                    actor->angle = actor->langle;
                }
            } else if ((actor->mv->rt_context_b != 0x14) && (actor->mv->rt_context_b != 0x3F)) {
                D_800CC2BC |= 0x100;
                gCurrentPad->button &= ~0x2000;
                gActorPressedButtons &= ~0x2000;
            }
            if (actor->mv->rt_context_b != 9) {
                D_800CC2BC |= 0x200;
                gCurrentPad->button &= ~0x8000;
                gActorPressedButtons &= ~0x8000;
            }
        }
    }
    if (!(actor->mv->transport & 0xF) && (actor->energy != 0) && (actor->stunned == 0) && (actor->shad == 0.0f) && ((gIsMultiplayer != 0) || ((actor->mv->rt_context_b == 9) && (actor->mv->matrix_physics == 0)) || (actor->mv->rt_context_b == 0x16) || (actor->mv->rt_context_b == 0x3B)) && ((actor->mv->weapon_status == 2) || (actor->mv->weapon_status == 0))) { BLK;
        dirMask = ((actor->mv->rt_context_b) && ((actor->mv->SniperState == 0) || (actor->mv->rt_context_b == 0x16)) && (actor->mv->GrenadeThrowState == 0)) ? 0xF : 3;
        if ((gCurrentPad->button & dirMask) != 0) {
            dirMask = (gCurrentPad->button & dirMask);
            if ((actor->mv->SniperState == 0) && (actor->mv->AimState == 0)) {
                if (gCurrentPad->stick_y >= 6) {
                    dirMask |= 8;
                } else if (gCurrentPad->stick_y < -5) {
                    dirMask |= 4;
                }
            }
            dirAngle = gDirMaskAngles[dirMask];
            actor->mv->use_cam_butt = 2;
            if (dirAngle != 0xFFFF) {
                if ((actor->unkAA == 0) || (actor->unkAA >= 0x42)) {
                    actor->unkAA = 0x41;
                }
                if (actor->speed > 12.0f) {
                    actor->mv->turn = 4;
                }
                if (actor->angle != actor->langle) {
                    actor->mv->turn = 0;
                }
                actor->langle -= ((gCurrentPad->stick_x * gActorStepPercent) >> 4);
                moveAngle = ((actor->langle + dirAngle) - actor->mv->pssd_waist_tilt) - D_800CC280;
                moveSpeed = 34.0f;
                if (actor->mv->SniperState != 0) {
                    actor->angle = moveAngle + D_800CC280;
                }
                actor->mv->unk1B3 = 1;
            } else {
                actor->speed = 0.0f;
            }
        } else if (actor->mv->use_cam_butt != 0) {
            actor->angle = actor->langle;
            if (actor->mv->SniperState != 0) {
                actor->speed = 0.0f;
            }
        }
        
    }
    if (actor->mv->door_walkf_time > 0) {
        actor->norun = 0;
        actor->count5 = 0;
        moveSpeed = 18.0f;
        moveAngle = actor->angle - D_800CC280;
        gCurrentPad->stick_x = 0;
        gCurrentPad->stick_y = 0;
        gCurrentPad->button = 0;
        gActorPressedButtons = 0;
        padOverridden = 1;
        actor->mv->door_walkf_time -= gFrameSteps;
        D_800cc2bc |= 0x40;
    }
    if (actor->mv->unk6B != 0) {
        func_15052260(actor);
    }
    surfaceDelta = actor->waterHeight - actor->prevWaterHeight;
    if (fabsf(surfaceDelta) > 30.0f) {
        surfaceDelta = 0.0f;
    }
    actor->prevWaterHeight = actor->waterHeight;
    if (actor->mv->bot == 0) {
        actor->mv->buttons = gCurrentPad->button;
        actor->mv->pressedButtons = gActorPressedButtons;
    }
    if ((actor->animseq.uh == 0) && (actor->animsp == 0.0f)) {
        actor_playAnim(actor, 0x24, 1.0f, 1.0f, 0.0f, 0.0f, 0);
    }
    if (gCurrentPad->stick_y == 0) {
        actor->mv->hang_ledge = 0;
    }
    if (actor->unk137 == 0xFF) {
        actor->unk137 = 0;
    }
    turnRate = 0xC;
    if ((actor->mv->pissed_up > 0) || (actor->mv->rt_context_b == 3)) {
        actor_updatePissed(actor, &moveAngle, &moveSpeed, &lean);
    }
    func_1504C9E4(actor, lean, actor->mv->rt_context_b);
    if (actor->stunned == 0) {
        if (actor->mv->bot != 0) {
            moveAngle = actor->fangle;
            moveSpeed = actor->stickspeed;
        }
        if (moveSpeed > 0.0f) {
            lockAngle = 0;
            lockDiff = 0;
            diff = 0;
            actor->mv->press_angle = moveAngle + D_800cc280;
            if (actor->mv->walk_attrib & 2) {
                lockAngle = actor->mv->unk34;
                wantAngle = actor->mv->press_angle;
                lockDiff = actor->mv->unk34 - wantAngle;
                if (!((lockDiff + 0x2000) & 0xC000)) {
                    actor->mv->press_angle = actor->mv->unk34;
                    lockAngle = actor->mv->unk34;
                    wantAngle = actor->mv->press_angle;
                    lockDiff = lockAngle - wantAngle;
                }
                if (!((lockDiff + 0xA000) & 0xC000)) {
                    actor->mv->press_angle = lockAngle + 0x8000;
                    wantAngle = actor->mv->press_angle;
                }
                moveAngle = wantAngle - D_800cc280;
            }
        } else {
            actor->mv->press_angle = actor->langle;
        }
        if (actor->inwater != 0) {
            swimSpeed = actor_updateSwim(actor, surfaceDelta);
            moveAngle = actor->mv->press_angle;
            if (actor->mv->unk22 >= 0) {
                actor->mv->unk22 -= gFrameSteps;
                if ((actor->mv->unk22 >= 0x5B) && (actor->mv->unk22 < 0x64)) {
                    actor->mv->unk22 = 0;
                }
            }
            if (actor->mv->unk21 == 0) {
                moveSpeed = swimSpeed;
                moveAngle = actor->angle;
                if (!(gCurrentPad->button & 0x4000)) {
                    if ((actor->mv->unk22 <= 0) && (((actor->waterHeight - 60.0f) - 30.0f) < actor->ypos)) {
                        actor->mv->unk21 = 1;
                        actor->norun = 0xC;
                    }
                }
            } else {
                if ((gActorPressedButtons & 0x4000) && (gIsMultiplayer == 0) && (level != 0x29)) {
                    if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100) && !(actor->mv->water_attribs & 1)) {
                        actor->mv->unk21 = 0;
                        actor->mv->unk22 = 0x76;
                        if (((actor->waterHeight - 60.0f) - 30.0f) < actor->ypos) {
                            actor->vtilt = 20.0f;
                        }
                        actor_playSound(0x66F, actor);
                    }
                }
                actor->mv->turn = 4;
                {
                BLK;
                actor->unkAA = 0x28;
                }
            }
        } else {
            moveAngle += D_800cc280;
        }
        D_800CC27E = moveAngle;
        actor->stickspeed = moveSpeed;
        if (actor->unkAA != 0) {
            moveSpeed *= actor->unkAA * 0.01f;
        }
        if (actor->lifting >= 0x64) {
            moveSpeed *= 0.8f;
        }
        if ((actor->norun != 0) || (actor->count3 >= 0x79)) {
            moveSpeed = 0.0f;
        }
        actor_tickTimers(actor);
        steerDiv = 4.0f;
        steer = 0.0f;
        if ((actor->inwater != 0) && (actor->mv->unk21 == 0)) {
            if ((actor->stunned == 0) && (actor->animseq.uh != 0x262)) {
                steer = (gCurrentPad->stick_x / 2);
            }
        } else {
            if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x13) || (actor->mv->rt_context_b == 0xA) || (actor->mv->rt_context_b == 0x34)) {
                if (actor->mv->AimState != 0) {
                    tiltDeg = -actor->mv->aimPitch;
                    if ((actor->mv->rt_context_b == 2) || (actor->mv->rt_context_b == 0x13)) {
                        tiltDeg += 30.0f;
                    } else {
                        tiltDeg = -tiltDeg;
                    }
                    { BLK; steer = (s8) tiltDeg; }
                } else {
                    steer = (gCurrentPad->stick_y / 2);
                }
            } else {
                if (actor->norun != 0xFF) {
                    angleDelta = moveAngle - prevFacing;
                    if ((actor->shad < 20.0f) && (actor->speed < 1.5f)) {
                        steer = actor->stickspeed * 0.3f;
                    } else {
                        steer = actor->stickspeed * 1.75f;
                        turnLean = fabsf(angleDelta * 0.00390625f * 5.0f);
                        if (turnLean > 50.0f) {
                            turnLean = 50.0f;
                        }
                        if (turnLean < steer) {
                            steer = turnLean;
                        }
                        if (actor->shad > 30.0f) {
                            steer *= 0.6f;
                        }
                        if (angleDelta >= 0) {
                            steer = 0.0f - steer;
                        }
                    }
                }
                if (((actor->lifting != 0) && (movement_obj[gCurrentActor->lifting - 100].character_type != 0x57)) || (actor->mv->AimState != 0) || (gCurrentPad->button & 0x2000)) {
                    steer = 0.0f;
                }
            }
        }
        if (actor->mv->env_suit != 0) {
            if (actor->animseq.uh == 0x279) {
                if (actor->norun == 0) {
                    if (gCurrentPad->button & 0x8000) {
                        actor->ygrav = 1.1f;
                    } else {
                        if (actor->yspd > 0.0f) {
                            actor->yspd *= 0.85f;
                        }
                        actor->ygrav = 2.5f;
                    }
                }
                if (actor->shad > 0.0f) {
                    actor->lean_angle += (((actor->unk54 * 4.0f) - actor->lean_angle) * 0.15f);
                    actor->vtilt = actor->speed * 1.5f;
                }
                actor->unkAA = 0x3C;
                actor->count2 = 1;
                actor->count5 = 0xFF;
            }
            steer *= 0.6f;
            steerDiv = 12.0f;
        }
        if ((actor->unk110 == 2) || (actor->animseq.uh == 0xCD)) {
            steer = -steer;
        }
        steer = (steer - actor->unk54) / (steerDiv / gActorTimeStep);
        if ((actor->shad < 30.0f) || (actor->mv->env_suit != 0)) {
            if (((steer > 0.0f) && (actor->mv->lean_last < 0.0f)) || ((prevSteer = actor->mv->lean_last, steer < 0.0f) && (prevSteer > 0.0f))) {
                steer = 0.0f;
            } else if (fabsf(steer - prevSteer) > 2.0f) {
                if (prevSteer < steer) {
                    steer = prevSteer + 2.0f;
                } else {
                    steer = prevSteer - 2.0f;
                }
            }
        }
        actor->mv->lean_last = steer;
        if ((gPaused == 0) && (gFrameRateDivider == 0)) {
            actor->unk54 += steer;
        }
        if (moveSpeed != 0.0f) {
            if (moveAngle != actor->angle) {
                if (actor->speed < 5.0f) {
                    turnStep = 0x19;
                } else {
                    turnStep = (((64.0f - actor->speed) * 0.25f));
                    if (actor->mv->prop >= 3) {
                        turnStep += 1;
                    } else if (actor->count3 > 0) {
                        turnStep = ((51.0f - actor->speed) / 13.0f);
                        if (actor->yspd < -10.0f) {
                            turnStep = 0;
                        }
                    }
                }
                turn = (moveAngle - actor->angle) >> 8;
                turnLeft = turn & 0x80;
                turnSize = turn;
                if (turn >= 0x80) {
                    turn = -turn;
                    turnSize = turn;
                }
                if (actor->unk102 != 0) {
                    turnStep = 5;
                } else if ((turnSize >= 0x51) && (actor->inwater == 0)) {
                    if ((actor->mv->bitten <= 0) && (actor->mv->pissed_up <= 0)) {
                        if ((actor->mv->transport & 0xF) == 1) {
                            if (actor->speed > 7.0f) {
                                turnStep = 0;
                                actor->speed -= 3.5f;
                            } else {
                                turnStep = 6;
                            }
                        } else if (actor->speed < 7.0f) {
                            if (actor->speed > 3.0f) {
                                turnStep = 0xC;
                                actor->speed = 0.0f;
                                if (actor->count5 >= 5) {
                                    if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3)) {
                                        actor->count5 = 4;
                                    }
                                }
                            }
                        } else {
                            moveSpeed = 0.0f;
                            turnStep = 0;
                            if (actor->unkAE == 0) {
                                actor->speed -= 2.0f;
                            }
                        }
                    }
                }
                turnStep = ((turnStep * gActorStepPercent) / 100);
                if (actor->lifting >= 0x64) {
                    turnStep = ((turnStep * 2) / 3);
                } else if (actor->unk110 != 0) {
                    turnStep = 2;
                }
                if (actor->inwater != 0) {
                    turnStep = (turnStep / 4);
                }
                if (actor->mv->turn != 0) {
                    turnStep = ((actor->mv->turn * gActorStepPercent) / 100);
                }
                if (turnSize < turnStep) {
                    turnStep = turnSize;
                }
                if (turnLeft) {
                    actor->angle -= (turnStep << 8);
                } else {
                    actor->angle += (turnStep << 8);
                }
            }
        }
        actor->mv->turn = 0;
        if (actor->unk110 != 0) {
            turnRate = 0x10;
            if (actor->unk110 & 1) {
                moveAngle = actor->mv->xslopeangle;
            } else {
                moveAngle = actor->mv->xslopeangle ^ 0x8000;
            }
        } else if ((moveSpeed == 0.0f) || (actor->inwater != 0) || (actor->mv->env_suit != 0) || (actor->shad < 50.0f)) {
            moveAngle = actor->angle;
        }
        turn = 0;
        if (actor->mv->matrix_physics != 0) {
            moveAngle = 0x4000;
            turnRate = 0x14;
        } else if ((actor->mv->env_suit != 0) && (actor->lifting == 0)) {
            target = collision_findObjectById(6);
            if (target != NULL) {
                if (actor_distanceXZ(target, actor) < 600.0f) {
                    moveAngle = actor_getDirectionAngle(target->xpos - actor->xpos, actor->zpos - target->zpos, 0);
                } else {
                    actor->mv->turn = 5;
                }
            }
        } else { } // matching: the empty else changes block layout
        if (actor->mv->use_cam_butt == 0) {
            turn = actor_turnFacing(actor, turnRate, moveAngle);
        }
        if ((turn >= 0x5B) && (actor->speed > 12.0f) && (actor->inwater == 0) && (actor->count3 == 0)) {
            moveSpeed = 0.0f;
        }
        if (actor->angle == actor->langle) {
            actor->count2 = 0;
        }
        friction = 0.0f;
        physics = actor->mv->transport;
        if ((physics & 0xF) == 1) {
            if (actor->mv->transport_surface != 0) {
                friction = 0.27f;
            } else {
                friction = 0.8f;
                if (physics == 0x81) {
                    friction = 0.2f;
                }
            }
            steer = 0.65f;
        } else {
            steer = 2.5f;
            if (actor->animseq.uh == 0x16) {
                steer = 3.2f;
                if (gCurrentPad->button & 0x2000) {
                    moveSpeed = 0.0f;
                }
            } else if (actor->lifting != 0) {
                if (actor->character_type == 0x13) {
                    steer = 0.75f;
                }
                friction = 0.75f;
            } else if (actor->unk110 != 0) {
                moveSpeed = 0.0f;
                steer = 2.0f;
            } else {
                if (actor->mv->bitten > 0) {
                    friction = 4.0f;
                } else {
                    friction = 1.0f;
                }
            }
            if (actor->mv->pissed_up > 0) {
                friction = 1.0f;
                steer = 2.0f;
            }
        }
        if (actor->mv->matrix_physics != 0) {
            steer = 0.0f;
        }
        actor_accelerate(moveSpeed, actor, friction, steer, turn);
        actor_updatePlayerControls(actor, gCurrentPad);
    } else {
        if (actor->inwater != 0) {
            actor_updateSwim(actor, surfaceDelta);
        }
        if (actor->stunned != 0xFF) {
            actor_runPlayerAction(actor, gCurrentPad->button, gActorPressedButtons, actor->shad, 0);
            actor->unk54 = 0.0f;
            actor->norun = 0;
            actor->count5 = 0;
            if (((actor->animseq.uh == 0xF1) || (actor->animseq.uh == 0xF6)) && (actor->shad == 0.0f)) {
                actor->yspd = 0.0f;
                actor->ygrav = 0.0f;
                actor->noshove |= 0x01008000;
            }
            if (!(actor->unk10B & 1) && (actor->speed < 2.0f) && (actor->stunned < 0xD2) && (actor->shad < 10.0f) && (actor->yspd < 0.5f)) {
                actor->stunned = 1;
                actor->unk10C = 1;
                actor->stunnednum = 3;
            }
            actor_updateStunned(actor, 0.1f);
            if (actor->unk10B & 0x40) {
                height = actor->ypos - gHitAttackerY;
                if (height < 400.0f) {
                    if (height < 10.0f) {
                        height = 10.0f;
                    }
                    actor->angle += (gActorStepPercent * 0x1400) / 100;
                    actor->yspd = (height * 0.06f) + 6.0f;
                    actor->speed = 3.0f;
                    actor_splitVelocity(actor->angle, height, 0.0f, &offsetX, &offsetZ, &offsetY);
                    actor->mv->swing_xvel = offsetX + gHitAttackerX;
                    actor->mv->swing_zvel = offsetZ + gHitAttackerZ;
                    actor->mv->gpoint = 2;
                } else {
                    actor->unk10B &= ~0x40;
                    actor->speed = 30.0f;
                    actor->yspd = 90.0f;
                    actor->ygrav = 4.0f; { BLK; }
                }
            }
            if (actor->unk10B & 0x80) {
                actor->unk228 = 0;
                actor->imune = 0x5A;
                if ((actor->shad == 0.0f) && (actor->yspd < 24.0f)) {
                    actor->unk10C = 0;
                    if (actor->unk10B & 0x20) {
                        actor->mv->bitten = 0xFA0;
                    }
                }
                if (moveSpeed > 5.0f) {
                    moveAngle += D_800CC280;
                    actor->count2 = 1;
                    actor_turnFacing(actor, 6, moveAngle);
                    actor_splitVelocity(moveAngle, moveSpeed, 0.0f, &pushX, &pushZ, &pushY);
                    actor->ob_overslopex += ((pushX - actor->ob_overslopex) * 0.1000000015f);
                    actor->ob_overslopey += ((pushZ - actor->ob_overslopey) * 0.1000000015f);
                    actor->angle = actor->langle;
                }
            }
        }
    }
    if ((actor->mv->rope != 0) && (actor->stunned == 0)) {
        stickX = gCurrentPad->stick_x;
        stickY = gCurrentPad->stick_y;
        actor->mv->prop = 0;
        actor->unk54 = 0.0f;
        actor->norun = 0xFF;
        actor->speed *= 0.95f;
        actor->yspd = 0.0f;
        actor->ygrav = 0.0f;
        actor->count3 = 1;
        if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode == 0x100)) {
            stickY = 0.0f;
            stickX = 0;
        }
        climbAnimSpeed = stickY;
        if ((stickY == 0.0f) || (stickY < -40.0f)) {
            climbAnimSpeed = 40.0f;
            climbAnim = 0xA4;
            if (stickY < -40.0f) {
                stickY *= 2.0f;
            }
        } else {
            climbAnim = 0x16A;
            if (stickY > 0.0f) {
                climbAnim = 0x98;
            } else {
                climbAnimSpeed = -stickY;
            }
        }
        path = actor->mv->ropePtr;
        climbRate = path->unk2C;
        climbRate = 0.08f / climbRate;
        actor->mv->ropeProgress += stickY * climbRate * gActorTimeStep;
        if (actor->mv->ropeProgress > 1.0f) {
            actor->mv->ropeProgress = 1.0f;
        } else if (actor->mv->ropeProgress < 0.0f) {
            rope_release(actor);
            actor->ygrav = 5.0f;
        }
        actor->unk1CC = actor->ypos;
        actor->mv->side = 8;
        heading = actor->angle + ((stickX * gActorStepPercent) / 5);
        actor->fangle = heading;
        actor->langle = heading;
        actor->angle = heading;
        actor_playAnim(actor, climbAnim, climbAnimSpeed * 0.025f, 6.0f, 0.0f, 0.0f, 0);
    }
    if ((actor->mv->ladder != 0) && (actor->stunned == 0)) {
        if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100)) {
            climbIndex = (actor->mv->ladder & ~0x80) - 1;
            climbSpeed = gCurrentPad->stick_y;
            climbSpot = &zones[climbIndex];
            if ((actor->inwater != 0) && (actor->ypos < actor->waterHeight)) {
                actor->mv->unk96 = 8;
            }
            if (actor->mv->unk96 != 0) {
                actor->mv->unk96 -= 1;
                climbSpeed = 60.0f;
            }
            actor->mv->swing_xvel = climbSpot->x;
            actor->mv->swing_zvel = climbSpot->z;
            actor->mv->gpoint = 0xA;
            actor->mv->prop = 0;
            actor->speed = 0.0f;
            actor->unk54 = 0.0f;
            actor->norun = 0xFF;
            actor->ygrav = 0.0f;
            actor->count3 = 1;
            actor->count2 = 1;
            actor->angle = (s32) (climbSpot->yaw * 182.04445f) - 0x4000;
            actor->yspd = climbSpeed * 0.14f;
            if (actor->mv->ladderFromTop != 0) {
                actor->yspd = 0.0f;
            }
            actor->unk1CC = actor->ypos;
            actor->mv->side = 8;
        }
    }
    if (actor->mv->unk6B == 0) {
        actor_moveHorizontal(actor);
        if (actor->mv->gpoint > 0) {
            actor->mv->gpoint -= gFrameSteps;
            actor->xpos += ((actor->mv->swing_xvel - actor->xpos) * 0.2f);
            actor->zpos += ((actor->mv->swing_zvel - actor->zpos) * 0.2f);
        }
        cutscene_applyObjectTrack(actor, 0, -1);
        actor_applyGravity(actor);
        actor_resolveCollisions(actor, dc_glbl_CharNum);
        if ((D_800CC26C != 0) && (actor->mv->pissed_up <= 0) && (actor->lifting == 0)) {
            heading = actor_getDirectionAngle(movement_obj[D_800CC26C].xpos - actor->xpos, actor->zpos - movement_obj[D_800CC26C].zpos, 0);
            
            if ((actor->shad == 0.0f) && (moveSpeed > 15.0f) && (((D_800CC27E - heading) - 0x4000) & 0x8000)) { BLK;
                follow = 0.1f;
                carry = 1;
                if (movement_obj[D_800CC26C].character_type == 0x61) {
                    if (actor->animseq.uh == 0x7A) {
                        otherFacing = movement_obj[D_800CC26C].langle;
                        diff = actor->angle - otherFacing;
                        quadrant = (diff + 0x2000) & 0xC000;
                        if (quadrant & 0x4000) {
                            movement_obj[D_800CC26C].scriptCmd = NULL;
                            movement_obj[D_800CC26C].intelwalk = 2;
                            movement_obj[D_800CC26C].angle = otherFacing + quadrant;
                            actor->angle = movement_obj[D_800CC26C].angle;
                            actor->mv->turn = 1;
                            { BLK; actor->unk54 *= 0.4f; }
                        }
                    }
                } else {
                    pushScale = 0.3f;
                    pushAccel = 0.5f;
                    heightDiff = fabsf(actor->ypos - movement_obj[D_800CC26C].ypos);
                    nudgeX = heightDiff;
                    if (heightDiff > 30.0f) {
                        nudgeX = 30.0f;
                    }
                    nudgeX = (2.0f * movement_obj[D_800CC26C].bodyRadius) - nudgeX;
                    actor->unk54 *= 0.6f;
                    actor->speed = movement_obj[D_800CC26C].stickspeed;
                    if (movement_obj[D_800CC26C].stickspeed < movement_obj[D_800CC26C].speed) {
                        nudgeZ = movement_obj[D_800CC26C].speed;
                    } else {
                        nudgeZ = movement_obj[D_800CC26C].stickspeed;
                    }
                    actor->mv->turn = ((32.0f - nudgeZ) * 0.125f);
                    if (movement_obj[D_800CC26C].character_type == 0x34) {
                        if (movement_obj[D_800CC26C].unk251 != 1) {
                            carry = 0;
                        }
                        pushAnim = 0x23;
                        actor->mv->turn = 2;
                        follow = 0.3f;
                        if (actor->speed > 5.0f) {
                            follow = 0.6f;
                        }
                    } else {
                        pushAnim = 0x18;
                    }
                    if ((pushAnim != actor->animseq.uh) && (movement_obj[D_800CC26C].character_type != 0x17)) {
                        gEventParam = 0xFF08012B;
                        pushAccel = 0.5f;
                        func_1506E8D8();
                    }
                    if (movement_obj[D_800CC26C].unk224 != 0) {
                        pushScale = movement_obj[D_800CC26C].unk224 * 0.01f;
                    }
                    if (carry != 0) {
                        pushAccel = 0.5f;
                        actor_splitVelocity(actor->angle, nudgeX, 0.0f, &nudgeX, &nudgeZ, &nudgeY);
                        actor->xpos += ((((movement_obj[D_800CC26C].xpos - nudgeX) + movement_obj[D_800CC26C].unkBC) - actor->xpos) * follow);
                        actor->zpos += ((((movement_obj[D_800CC26C].zpos - nudgeZ) + movement_obj[D_800CC26C].unk148) - actor->zpos) * follow);
                        movement_obj[D_800CC26C].angle = actor->angle;
                    }
                    movement_obj[D_800CC26C].unk87 = 1;
                    nudgeX = actor->stickspeed * pushScale;
                    if (movement_obj[D_800CC26C].character_type == 0x25) {
                        nudgeX = actor->stickspeed * 0.6f;
                        actor->speed += 2.0f;
                        pushAccel = 1.0f;
                        actor->mv->turn += 3;
                    }
                    if (movement_obj[D_800CC26C].character_type != 0x34) {
                        if (movement_obj[D_800CC26C].speed < nudgeX) {
                            movement_obj[D_800CC26C].speed += (pushAccel * gFrameDelta);
                        }
                    } else {
                        actor->speed = movement_obj[D_800CC26C].speed;
                    }
                }
            }
        }
    }
    if ((gIsMultiplayer != 0) && (gCollidedActors != 0) && (actor->mv->on_fire != 0)) {
        actor_igniteTouching(actor);
    }
    actor->theta = (actor->langle + 0x4000) * 0.005493164f;
    cutscene_applyObjectTrack(actor, 0, -1);
    startX = actor->xpos;
    startZ = actor->zpos;
    if (!(actor->noshove & 0x8000)) {
        actor_clampToZoneSet(actor, actor->family);
        if (actor->mvmt_control & 0x100000) {
            probeTop = 30000.0f;
        } else {
            probeTop = actor->old_ypos;
        }
        if ((actor->waterHeight != -10000.0f) || ((actor->noshove & 2) == 0) || ((collision_getTriHeightAt(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &floorY) == 0))) {
            collision_findFloorEx(&actor->floorTriVerts, actor->floorTri.v, &floorY, &actor->waterHeight, &actor->floorMaterial, &actor->floorType, actor->xpos, actor->ypos, actor->zpos, probeTop, 0, actor, -10000.0f, actor->ypos, actor->noshove & 0x04000000);
            if (actor->waterHeight < actor->unk120) {
                actor->waterHeight = actor->unk120;
            }
        }
        if (actor->ypos < floorY) {
            snapRange = 30.0f;
        } else {
            snapRange = 15.0f;
        }
        if ((actor->noshove & 4) && (actor->mv->side <= 0) && (actor->mv->unk6B == 0) && (actor->unk137 == 0) && (actor->attach_char == 0) && ((actor->mv->bot == 0) || (actor->xpos != actor->old_xpos) || (actor->ypos != actor->old_ypos) || (actor->zpos != actor->old_zpos))) {
            if (actor->noshove & 0x08000000) {
                D_800CBDD3 = 1;
            }
            if (actor->mv->transport == 2) {
                D_800CBDD4 = 1;
            }
            collision_resolveActorWalls(actor->old_xpos, actor->old_ypos, actor->old_zpos, actor, 1, 1);
            D_800CBDD4 = 0;
            D_800CBDD3 = 0;
        } else {
            gLedgeHeight1 = gLedgeHeight2 = -32768.0f;
        }
        if ((startX != actor->xpos) || (startZ != actor->zpos)) {
            scratch = -10000.0f;
            actor->mvmt_control |= 0x10000;
            if ((scratch != actor->waterHeight) || (floorY != scratch) || !(actor->noshove & 2) || (collision_getTriHeightAt(actor->xpos, actor->zpos, actor->floorTriVerts, actor->floorTri.v, &floorY) == 0)) {
                collision_findFloorEx(&actor->floorTriVerts, actor->floorTri.v, &floorY, &actor->waterHeight, &actor->floorMaterial, &actor->floorType, actor->xpos, actor->ypos, actor->zpos, probeTop, 0, actor, scratch, actor->ypos, actor->noshove & 0x04000000);
                if (actor->waterHeight < actor->unk120) {
                    actor->waterHeight = actor->unk120;
                }
            }
            heading = actor_getDirectionAngle(actor->xpos - startX, startZ - actor->zpos, 0);
            moveDelta = actor->angle - heading;
            if (actor->mv->transport == 2) {
                if ((moveDelta >= 0x1E01) && (moveDelta < 0xE200)) {
                    dirAngle = actor->angle - heading - 0x8000;
                    if (dirAngle & 0x8000) {
                        deflect = (dirAngle >> 2) + 0x9800;
                    } else {
                        deflect = (dirAngle >> 2) + 0x2800;
                    }
                    actor->angle = heading - deflect;
                    if (actor->mv->camjolt <= 0) {
                        if ((actor->energy == 1) || ((moveDelta >= 0x5E01) && (moveDelta < 0xA200))) {
                            actor->energy = 0;
                            particle_spawnBreakup(actor, 0xFF, 1);
                            actor_kill(actor, 8);
                        } else if (actor->energy != 0) {
                            if (gIsMultiplayer == 0) {
                                gEventParam = 0xFF0201D6;
                                func_1506E9D8();
                            }
                            actor->energy -= 1;
                        }
                        actor->mv->camjolt = 0x14;
                        if (moveDelta >= 0x5E01) {} // matching: dead test the original evaluates
                    }
                }
                if (actor->speed > 30.0f) {
                    actor->speed *= 0.7f;
                    actor->norun = 0xC;
                }
                if ((actor->energy != 0) && (sound_isActorSound2Playing(actor) == 0)) {
                    gEventParam = 0xFF08030A;
                    func_1506E5FC();
                }
            } else {
                turnSize = actor->inwater;
                if (turnSize == 0) {
                    if (actor->shad > 60.0f) {
                        if ((moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (turnSize != 0x64) /* matching: always true here, still emitted */ && (actor->speed > 14.0f)) {
                            actor->speed = 14.0f;
                        }
                    } else if ((actor->mv->env_suit == 0) && (actor->mv->unk1B3 == 0) && (actor->lifting == 0) && (moveDelta >= 0x5E01) && (moveDelta < 0xA200) && (((scratch = actor->speed) > 10.0f) || ((actor->mv->duck_tilt == 0) && (scratch > 2.0f)))) {
                        if (scratch > 14.0f) {
                            actor->speed = 14.0f;
                        }
                        if ((actor->mv->transport == 0) && (actor->mv->weapon_status == 0)) {
                            pushAnim = 0x7A;
                            if ((actor->count3 == 0) && (actor->count5 == 0)) {
                                actor->theta = (heading - 0x4000) * 0.005493164f;
                            }
                        }
                    }
                }
            }
        }
        gFloorMaterialClass = (actor->floorMaterial >> 28) & 7;
        if (gFloorMaterialClass >= 7) {
            gFloorMaterialClass = 0;
        }
        actor->floorY = floorY;
        actor->mv->transport_surface = 0;
        physics = actor->mv->transport;
        if ((physics & 0xF) == 1) {
            if (actor->ygrav < 1.0f) {
                actor->ygrav = 5.0f;
            }
            if ((floorY + 45.0f) < actor->waterHeight) {
                actor->mv->transport_surface = 1;
                snapRange = 0.0f;
            }
        } else if (physics == 2) {
            if (floorY == -10000.0f) {
                actor->energy = 0;
                particle_spawnBreakup(actor, 0xFF, 1);
                actor_kill(actor, 8);
            }
            if (actor->mv->unk50 <= 0) {
                func_1504CA60(actor);
            }
            actor->unkCC = 0x1E;
            actor->unkCE = actor->unkCC;
            if ((D_800BE9E4 < actor->mv->unk12C) && (actor->energy != 0)) {
                actor->mv->unk12C -= D_800BE9E4;
                actor->speed = 254.0f;
            } else {
                actor->mv->unk12C = 0;
            }
            if ((actor->mv->pressedButtons & 0x2000) && (actor->energy != 0)) {
                func_150D7790(actor, actor->mv->context_b & 0x7F);
            }
        }
        if ((actor->ypos < (floorY + snapRange)) && (actor->yspd < 0.0f) && ((snapRange > 20.0f) || (actor->shad == 0.0f))) {
            actor->ypos = floorY - 1.0f;
        }
        if ((actor->mv->transport == 2) || (actor->mv->matrix_physics != 0)) {
            actor_landNonPlayer(actor, actor->floorY);
        } else {
            actor_landPlayer(actor, actor->floorY);
        }
        if (actor->mv->ladder != 0) {
            actor->vtilt = 0.0f;
            if (actor->stunned == 0) {
                if ((actor->CameraPtr != NULL) && (actor->CameraPtr->Mode != 0x100)) {
                    climbInput = gCurrentPad->stick_y;
                    climbIndex = (actor->mv->ladder & ~0x80) - 1;
                    climb = &zones[climbIndex];
                    if (actor->mv->unk96 != 0) {
                        climbInput = 60.0f;
                    }
                    climbTop = (climb->y + climb->ySize);
                    animSpeed = (climbInput * 0.025f);
                    actor->unk1CC = actor->ypos;
                    if (actor->mv->ladderFromTop != 0) {
                        actor->count5 = 0xFF;
                        actor->norun = 0xC;
                        animSpeed = 1.0f;
                        actor->unk100 |= 0x20;
                        anim = 0x31A;
                    } else {
                        if ((climbTop <= actor->ypos) && (climbInput >= 0.0f)) {
                            animSpeed = 1.0f;
                            anim = 0x206;
                        } else {
                            anim = 0x297;
                            if (climbInput >= 0.0f) {
                                anim = 0x193;
                            } else {
                                animSpeed = -(climbInput * 0.025f);
                            }
                        }
                        actor->floorMaterial &= ~0x1F;
                        if (level == 0x23) {
                            actor->floorMaterial |= 9;
                        } else {
                            actor->floorMaterial |= 3;
                        }
                        if ((climbTop - 80.0f) <= actor->ypos) {
                            if (!(actor->mv->ladder & 0x80)) {
                                actor->unk137 = 2;
                                animSpeed = 3.0f;
                                anim = 0x262;
                                actor->mv->ladder = 0;
                            } else if (climbTop <= actor->ypos) {
                                animSpeed = 1.0f;
                                anim = 0x206;
                                actor->yspd = 0.0f;
                                actor->ypos = climbTop;
                            }
                        } else if (actor->ypos < climb->y) {
                            actor->norun = 0;
                            actor->mv->ladder = 0;
                            actor->ygrav = 4.0f;
                        }
                        if ((actor->mv->ladder == 0) && (actor->inwater != 0)) {
                            actor->ygrav = 0.0f;
                        }
                    }
                    if (anim != -1) {
                        actor_playAnim(actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                    }
                }
            }
            if (actor->shad == 0.0f) {
                if (actor->mv->ladderFromTop == 0) {
                    actor->mv->ladder = 0;
                    actor->norun = 6;
                    actor->ygrav = 4.0f;
                }
            }
        }
    }
    if ((actor->shad == 0.0f) && (actor->mv->transport != 2) && (actor->energy != 0) && ((actor->floorMaterial & 0x1F) == 0xE)) {
        actor_hitScripted(actor, 1, actor->langle, -1);
        actor->unk10B |= 0x80;
    }
    if ((actor->floorY < -9000.0f) && (actor->yspd < -120.0f)) {
        actor->mv->over_edge += gFrameSteps;
        if (actor->mv->over_edge >= 0x24) {
            func_1506D538();
        }
    } else {
        actor->mv->over_edge = 0;
    }
    func_15051558(actor, startX, startZ, moveAngle);
    if (actor->mv->transport == 0) {
        if ((actor->shad > 5.0f) && (actor->unk137 == 0) && (actor->norun == 0) && (actor->mv->rt_context_b == 0) && (actor->mv->env_suit == 0) && (actor->stunned == 0) && (actor->mv->bitten == 0) && (actor->inwater == 0) && (actor->lifting == 0) && (actor->floorY != -10000.0f) && (actor->energy != 0) && (actor->mv->pissed_up == 0) && (actor->mv->transport == 0) && (actor->unk110 == 0) && ((actor->unk1CC - actor->ypos) < 450.0f)) {
            anim = 0;
            if (gLedgeHeight1 != -32768.0f) {
                ledgeAngle = (((actor->theta + gLedgeAngle1) - 90.0f) / 1.40625f);
                ledgeY = gLedgeHeight1;
                anim = 1;
            } else if ((gLedgeHeight2 != -32768.0f) && (actor->speed < 10.0f)) {
                ledgeAngle = ((actor->theta + gLedgeAngle2 + 90.0f) / 1.40625f);
                ledgeY = gLedgeHeight2;
                anim = 2;
            }
            if (anim != 0) {
                actor->angle = (ledgeAngle << 8);
                actor->langle = actor->angle;
                actor->ypos = ledgeY - 25.0f;
                actor->xpos = gLedgeX;
                actor->zpos = gLedgeZ;
                actor->mv->hang_ledge = 1;
                actor->mv->save_done = 0;
                actor->norun = 0xFF;
                actor->unk137 = 1;
                actor->count3 = 1;
                actor->speed = 0.0f;
                actor->yspd = 0.0f;
                actor->ygrav = 0.0f;
                actor->unk1CC = actor->ypos;
                actor_playAnim(actor, 0x42, 1.0f, 1.0f, 0.0f, 0.0f, 0);
                actor->mv->camjolt = 0xC;
            }
        }
        if (actor->yspd < 5.0f) {
            if ((actor->waterHeight != -10000.0f) && (actor->ypos < (actor->waterHeight - 60.0f)) && (actor->inwater != 1)) {
                splashSpeed = 4.0f;
                actor->mv->save_done = 0;
                actor->mv->prop = 0;
                actor->inwater = 1;
                actor->ob_slopex = 0.0f;
                actor->ob_slopey = 0.0f;
                actor->unk162 = 0;
                actor->unk1CC = actor->ypos;
                actor->speed *= 0.5f;
                actor->angle = actor->langle;
                actor->mv->unk22 = 0;
                if (D_800BE9B4 == 0) {
                    actor->mv->unk21 = 1;
                    actor->underwaterTime = 0;
                }
                if ((actor->mv->water_attribs & 1) && (gIsMultiplayer == 0)) {
                    collision_createPartSet(actor, 0x38, 0.0f, 0);
                    splashSpeed = 4.0f;
                }
                if (actor->unk110 == 2) {
                    actor->angle ^= 0x8000;
                    actor->langle = actor->angle;
                    actor->speed = 20.0f;
                    actor->vtilt = -30.0f;
                }
                actor->unk110 = 0;
                if ((actor->animseq.uh == 0x3C) || (actor->animseq.uh == 0x3B)) {
                    splashSpeed = 0.0f;
                    actor->yspd *= 0.5f;
                    actor->ygrav = -2.4f;
                    actor->vtilt = 89.0f;
                } else {
                    actor->yspd *= 0.4f;
                    actor->ygrav = -3.0f;
                }
                actor->stunned = 0;
                actor->wantspeed = 0.0f;
                actor->count3 = 0;
                actor->unkA9 = 0;
                if ((actor->mv->ladder == 0) && (actor->energy != 0)) {
                    event_playAnim(0xD1, 2.0f, splashSpeed);
                    actor->norun = 0;
                }
                if (actor->energy == 0) {
                    actor->norun = 0xFF;
                    actor_kill(actor, 4);
                    actor->stunned = 0xFF;
                }
            }
        }
    }
    func_1505250C(actor, 1);
    if (actor->unkCC < -0x5D) {
        actor->energy = 0;
        actor_kill(actor, 3);
    }
    if (actor->doing_hit != 0) {
        actor_testAttack(actor, dc_glbl_CharNum);
    }
    if (actor->count1 >= 9) {
        actor->count2 = 1;
    }
    if (actor->count5 != 0) {
        if (actor->count5 != 0xFF) {
            if (gFrameSteps >= actor->count5) {
                actor->count5 = 0;
            } else {
                actor->count5 -= gFrameSteps;
            }
        }
        if ((actor->animseq.uh == 0x36) || (actor->animseq.uh == 0x19D)) {
            if ((actor->speed > 8.0f) && (actor->norun == 0)) {
                actor->count5 = 0;
                moveSpeed = actor->speed;
            }
        }
    } else {
        actor->unkAA = 0;
    }
    if (actor->unkA8 != 0) {
        actor->unkA8 -= 1;
    }
    if (actor->unk102 != 0) {
        actor->unk102--;
        actor->ygrav = 5.0f;
        if (actor->unk102 == 0) {
            func_1506AF74();
        }
    }
    if (actor->unk100 & 1) {
        if (actor->mv->rt_context_b == 0) {
            if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->inwater == 0) && (actor->stunned == 0) && (D_800CC2B1 == 0) && ((actor->norun == 0) || (actor->animseq.uh == 0x279)) && (actor->mv->pissed_up <= 0) && (actor->shad == 0.0f) && (gCurrentPad->button & 0x2000)) {
                actor->count3 = 0;
                actor->count5 = 0;
                actor->unk100 &= 0xFE;
                actor->angle = actor->langle;
            }
        }
    }
    if (((actor->unk1CC - actor->ypos) > 600.0f) && (actor->mv->death_State == 0)) {
        if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->waterHeight == -10000.0f)) {
            if ((actor->animseq.uh != 0x101) && (actor->inwater == 0)) {
                if (actor->animseq.uh != 0xCF) {
                    sound = 0x2007E5;
                    if (event_resolveSoundCommand(actor, &sound, 1) != 0) {
                        actor_playSound(sound, actor);
                    }
                    { BLK; }
                }
                actor->mv->prop = 0;
                actor_playAnim(actor, 0xCF, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                actor->count5 = 0xC8;
            }
        }
    }
    actor->mv->duck_tilt = 0;
    actor_updateWalkAnim(actor, pushAnim, moveSpeed);
    actor->mv->actual_speed = 2.0f * sqrtf(((actor->xpos - actor->old_xpos) * (actor->xpos - actor->old_xpos)) + ((actor->zpos - actor->old_zpos) * (actor->zpos - actor->old_zpos)));
    actor->mv->walk_attrib = 0;
    if ((actor->mv->bot != 0) || (padOverridden != 0)) {
        *gCurrentPad = savedPad;
    }
    actor->mv->prevButtons = buttons;
}

/* Tilts the actor toward the slope of the ground under it (pitch/roll and the unk164/unk168 lean). */
void actor_tiltToGround(Actor *actor) {
    f32 slopeX;
    f32 slopeZ;
    f32 normalX;
    f32 normalZ;
    f32 leanRate;

    if (actor->shad <= 0.0f) {
        s32 clamped;
        f32 gain;
        f32 damping;
        f32 scale;
        f32 rise;
        f32 roll;

        gain = 0.6999999881f;
        damping = 0.3000000119f;
        scale = 35.0f;
        // matching: a literal spelled unlike gain's is not CSE'd with it by IDO
        rise = 0.69999998807907104f;
        if ((actor->character_type == 0x28) || (actor->character_type == 0x77)) {
            gain = 0.8999999762f;
            damping = 0.200000003f;
            scale = 7.0f;
            rise = 0.6000000238f;
        }
        collision_getTriSlope(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
        if (actor->noshove & 0x100000) {
            f32 pitch;

            collision_getTriSlope(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &slopeX, &slopeZ);
            roll = actor_slopeToAngles(actor, slopeX, slopeZ, &pitch, 50.0f);
            pitch *= gain;
            actor->vtilt += (pitch - actor->vtilt) * damping;
            actor->lean_angle += (roll * gain - actor->lean_angle) * damping;
        }
        if (!(actor->noshove & 0x80000)) {
            return;
        }
        if (actor->unk226 != 0) {
            scale = actor->unk226;
        }
        if ((fabsf(slopeX) < 0.09000000358f) && (fabsf(slopeZ) < 0.09000000358f)) {
            slopeX = 0.0f;
            leanRate = 0.200000003f;
            slopeZ = 0.0f;
        } else {
            f32 steepness;

            clamped = 0;
            if (fabsf(slopeX) > 1.0f) {
                // matching: 1.00000000f (not 1.0f) keeps IDO from reusing the comparison's 1.0f register
                if (slopeX < 0.0f) {
                    slopeX = -1.0f;
                } else {
                    slopeX = 1.00000000f;
                }
                clamped = 1;
            }
            if (fabsf(slopeZ) > 1.0f) {
                if (slopeZ < 0.0f) {
                    slopeZ = -1.0f;
                } else {
                    slopeZ = 1.00000000f;
                }
                clamped = 1;
            }
            if (clamped && (actor->character_type == 0x77)) {
                if (actor->speed > 15.0f) {
                    actor->speed *= 0.8000000119f;
                }
                if (actor->norun < 8) {
                    actor->norun = 6;
                }
            }
            steepness = sqrtf(slopeX * slopeX + slopeZ * slopeZ);
            slopeX *= scale;
            slopeZ *= scale;
            leanRate = steepness * rise + 0.05000000075f;
        }
    } else {
        slopeZ = 0.0f;
        slopeX = 0.0f;
        leanRate = 0.1000000015f;
    }
    actor_stepVector2D(-slopeX, -slopeZ, leanRate, &actor->ob_slopex, &actor->ob_slopey);
}


/* Ground/water tilt for a player-like actor: slides on steep slopes, then eases roll (unkC4) and pitch (unkB8). */
void func_15051558(Actor *actor, f32 x, f32 z, u16 facing) {
    f32 roll;
    f32 pitch;
    f32 blend;
    f32 height;
    u8 bumped;

    roll = 0.0f;
    bumped = 0;
    blend = 0.200000003f;
    height = actor->shad;
    pitch = 0.0f;
    if (actor->inwater == 0) {
        s32 wasSliding;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 moved;
        f32 normalX;
        f32 normalZ;
        f32 leanRate;
        u16 moveAngle;
        f32 animSpeed;
        u16 heading;
        f32 sway;
        s16 shake;
        s32 kind;
        u16 anim;

        wasSliding = actor->unk162;
        normalX = 0.0f;
        normalZ = 0.0f;
        leanRate = 3.0f;
        if (actor->mv->transport == 2) {
            collision_getTriSlope(actor->floorTriVerts, actor->floorTri.v, &normalX, &normalZ, &leanRate, &roll, &pitch);
            if ((((roll * roll) + (pitch * pitch)) > 1.39999998f) && ((roll * roll) + (pitch * pitch) < 1000.0f)) {
                actor->nojump = 6;
                if ((height < 80.0f) && (actor->norun == 0)) {
                    mp_alignRacerToTrack(dc_glbl_CharNum, actor);
                }
            }
            if (height >= 0.0f) {
                normalX = 0.0f;
                normalZ = 0.0f;
                roll = 0.0f;
                pitch = 0.0f;
                leanRate = 3.0f;
            }
        } else if (((height <= 0.0f) || (actor->unk110 != 0)) && (actor->mv->transport_surface == 0)) {
            collision_getTriSlope(NULL, actor->floorTri.v, &normalX, &normalZ, &leanRate, &roll, &pitch);
        }
        if ((actor->mv->transport == 0) && (actor->mv->bot == 0) &&
            (((gFloorMaterialClass == 0) && ((((roll * roll) + (pitch * pitch)) > 1.0f) || (((actor->floorMaterial >> 12) & 0xF) == 4))) ||
             ((gFloorMaterialClass != 0) && (actor->mv->env_suit != 0))) &&
            (((actor->floorMaterial >> 12) & 0xF) != 3)) {
            gFloorMaterialClass = 1;
            bumped = 1;
        }
        kind = (actor->stand_on_char != 0) ? movement_obj[actor->stand_on_char - 1].character_type : -1;
        if (gFloorMaterialClass == 0) {
            normalX = 0.0f;
            normalZ = 0.0f;
            leanRate = 3.0f;
        }
        switch (kind) {
        case 8:
            bumped = 0;
            break;
        case 0x49:
            bumped = 0;
            break;
        }
        if ((gFloorMaterialClass != 0) && (actor->shad < 5.0f) && (actor->unk110 != 0)) {
            actor->unk162 = 1;
            if (actor->norun != 0xFE) {
                actor->norun = gFrameSteps + 1;
            }
        } else {
            actor->unk162 = 0;
        }
        dx = x - actor->old_xpos;
        dy = actor->ypos - actor->old_ypos;
        dz = z - actor->old_zpos;
        moved = 2.0f * sqrtf((dx * dx) + (dy * dy) + (dz * dz)) * gFrameDeltaInv;
        if (actor->unk162 - wasSliding) { // matching: the original tests the difference (subu + bnez), not !=
            if (actor->unk162 != 0) {
                actor->ob_slopex = dx * gFrameDeltaInv;
                actor->ob_slopey = dz * gFrameDeltaInv;
                actor->speed = 0.0f;
            } else {
                actor->speed = moved;
                if (moved < 22.0f) {
                    actor->speed = 22.0f;
                }
                if (actor->unk110 == 3) {
                    actor->unk110 = 0;
                    if (actor->norun != 0xFE) {
                        actor->norun = 0;
                    }
                    actor->speed = 17.8999996f;
                }
                actor->angle = actor_getDirectionAngle(x - actor->old_xpos, actor->old_zpos - z, 0);
                actor->ob_slopex = 0.0f;
                actor->ob_slopey = 0.0f;
                actor->ob_overslopex = 0.0f;
                actor->ob_overslopey = 0.0f;
            }
        }
        if ((gFloorMaterialClass != 0) && (actor->shad < 5.0f)) {
            animSpeed = 1.0f;
            actor->nojump = gFrameSteps + 8;
            actor->unk112 = actor_getDirectionAngle(-normalX, normalZ, 0);
            moveAngle = actor_getDirectionAngle(actor->xpos - actor->old_xpos, actor->old_zpos - actor->zpos, 0);
            gFloorMaterialClass = 1;
            if (actor->unk110 == 0) {
                leanRate = D_800990C0[gFloorMaterialClass];
                if ((u8)(((actor->unk112 - moveAngle) >> 8) + 0x30) < 0x60) {
                    heading = (actor->stickspeed > 8.0f) ? facing : actor->langle;
                    if (bumped != 0) {
                        actor->unk110 = 3;
                        anim = 0x28E;
                        animSpeed = 2.0f;
                    } else {
                        anim = 0xD0;
                        if ((u8)(((actor->unk112 - heading) >> 8) + 0x58) < 0xB0) {
                            actor->unk110 = 1;
                            anim = 0x101;
                        } else {
                            actor->unk110 = 2;
                        }
                    }
                    if ((actor->stunned == 0) && (actor->lifting == 0) && (actor->norun != 0xFF) && (actor->count5 < 0xFA)) {
                        if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3) && (actor->mv->death_State == 0) && (actor->mv->GrenadeThrowState == 0)) {
                            actor_playAnim(actor, anim, animSpeed, 6.0f, 0.0f, 0.0f, 0);
                        }
                    }
                }
            } else {
                leanRate = D_800990E0[gFloorMaterialClass];
            }
        }
        actor_stepVector2D(-normalX, -normalZ, leanRate, &actor->ob_slopex, &actor->ob_slopey);
        actor->ob_overslopex += (0.0f - actor->ob_overslopex) * 0.150000006f;
        actor->ob_overslopey += (0.0f - actor->ob_overslopey) * 0.150000006f;
        if (actor->unk110 != 0) {
            actor->mv->xslopeangle = actor_getDirectionAngle(actor->xpos - actor->old_xpos, actor->old_zpos - actor->zpos, 0);
            if ((actor->animseq.uh == 0x28E) || (actor->animseq.uh == 0x101) || (actor->animseq.uh == 0xD0)) {
                if ((actor->mv->weapon_status != 1) && (actor->mv->weapon_status != 3)) {
                    actor->count5 = gFrameSteps + 1;
                }
            }
            if (gFloorMaterialClass == 0) {
                if (actor->unk110 == 2) {
                    actor->mv->xslopeangle = actor->angle = actor->unk112;
                    if ((actor->speed < 2.0f) && (actor->shad == 0.0f)) {
                        if (actor->stunned == 0) {
                            actor_playAnim(actor, 0xA5, 1.39999998f, 5.0f, 0.0f, 0.0f, 0);
                            actor->count5 = 0x3C;
                            actor->norun = 0x3C;
                            actor->nojump = 0x1E;
                            actor->angle ^= 0x8000;
                            actor->langle = actor->angle;
                            actor->unk100 |= 0x20;
                        }
                        actor->unk110 = 0;
                    }
                } else if (actor->speed < 12.0f) {
                    actor->unk110 = 0;
                }
            }
        }
        if (height <= 0.0f) {
            roll = actor_slopeToAngles(actor, roll, pitch, &pitch, 50.0f);
            if (actor->mv->transport != 2) {
                if ((actor->mv->duck_tilt == 0) && (actor->unk110 == 0) && ((actor->mv->transport & 0xF) != 1)) {
                    roll *= (actor->mv->actual_speed * 0.00899999961f) + 0.200000003f;
                    pitch *= (actor->mv->actual_speed * 0.0140000004f) + 0.200000003f;
                } else {
                    roll *= 0.800000012f;
                    pitch *= 0.800000012f;
                    if ((actor->mv->transport & 0xF) == 1) {
                        roll *= 0.5f;
                        pitch = 0.0f;
                    }
                }
            }
        }
        if (actor->mv->transport == 2) {
            {
                f32 yVelocity = actor->yspd;
                sway = actor->unk54 * 6.0f;
                if (yVelocity < -30.0f) {
                    pitch -= -30.0f;
                } else {
                    pitch -= yVelocity;
                }
            }
            blend = 0.400000006f;
            if (actor->mv->camjolt > 0) {
                shake = (D_800CC250 & 4);
                shake -= 2;
                shake *= (u32)(actor->mv->camjolt * 2); // matching: unsigned multiply (multu)
                sway += shake;
                blend = 0.25f;
            }
            if ((fabsf(actor->lean_angle) > 70.0f) && (gIsMultiplayer == 0) && (actor->energy != 0) && (actor->mv->gun_before_reload == 0)) {
                gEventParam = 0xFF020146;
                func_1506E8D8();
                actor->mv->gun_before_reload = 1;
            }
            if (fabsf(actor->lean_angle) < 20.0f) {
                actor->mv->gun_before_reload = 0;
            }
            roll += sway;
        }
        actor->vtilt += (pitch - actor->vtilt) * blend;
    } else {
        f32 depth;

        depth = (actor->waterHeight - 30.0f) - actor->ypos;
        depth += actor->speed * 6.0f;
        if (depth > 250.0f) {
            depth = 2.5f;
        } else {
            depth *= 0.00999999978f;
        }
        if ((fabsf(actor->vtilt) < 35.0f) && (depth > 0.0f)) {
            roll += actor->unk54 * depth;
        }
        actor_stepVector2D(0.0f, 0.0f, 0.300000012f, &actor->ob_slopex, &actor->ob_slopey);
    }
    actor->lean_angle += (roll - actor->lean_angle) * blend;
}
/* Turns a surface slope into roll/pitch angles (degrees) in the actor's frame, clamped to +-limit. */
f32 actor_slopeToAngles(Actor *actor, f32 slopeX, f32 slopeZ, f32 *pitch, f32 limit) {
    f32 cosYaw;
    s32 pad; // matching: keeps the stack frame at the original size
    f32 yaw;
    f32 sinYaw;
    f32 localX;
    f32 minLimit;

    slopeZ = -slopeZ;
    yaw = actor->theta * 0.01745329238f;
    cosYaw = math_cos(yaw);
    sinYaw = math_sin(yaw);
    localX = (slopeX * cosYaw) + (slopeZ * sinYaw);
    slopeZ = (-slopeX * sinYaw) + (slopeZ * cosYaw);
    slopeX = math_atan2(localX, 1.0f) * 57.29577637f;
    slopeZ = math_atan2(slopeZ, 1.0f) * 57.29577637f;
    if (slopeX >= 180.0f) {
        slopeX -= 360.0f;
    }
    if (slopeZ >= 180.0f) {
        slopeZ -= 360.0f;
    }
    minLimit = -limit;
    if (slopeX > limit) {
        slopeX = limit;
    }
    if (slopeX < minLimit) {
        slopeX = minLimit;
    }
    if (slopeZ > limit) {
        slopeZ = limit;
    }
    if (slopeZ < minLimit) {
        slopeZ = minLimit;
    }
    *pitch = slopeZ;
    return slopeX;
}

/*
 * While mv->unk6B < 4, moves the actor toward the s16 target position in its mv block
 * (all the way, or 40% per call in modes 1 and 3); otherwise clears the mode once unk28 runs out.
 * Ends the mode and sets inwater when the actor drops below waterHeight.
 */
void func_15052260(Actor *actor) {
    ActorCtl *ctrl = actor->mv;
    f32 blend;

    if (ctrl->unk6B == 1 || ctrl->unk6B == 3) {
        blend = 0.400000006f;
    } else {
        blend = 1.0f;
    }
    actor->unk1CC = actor->ypos;

    if (ctrl->unk6B < 4) {
        actor->xpos += (ctrl->holdX - actor->xpos) * blend;
        actor->ypos += ((ctrl->holdY - 80) - actor->ypos) * blend;
        actor->zpos += (ctrl->holdZ - actor->zpos) * blend;
        actor_playAnim(actor, 20, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        actor->yspd = -10.0f;
    } else {
        if ((s32)actor->shad == 0) {
            ctrl->unk6B = 0;
        } else {
            actor_playAnim(actor, 56, 1.0f, 6.0f, 0.0f, 0.0f, 0);
        }
        actor_applyGravity(actor);
    }
    actor->ygrav = 6.199999809f;
    if (actor->ypos < actor->waterHeight) {
        actor->ygrav = 0.0f;
        actor->yspd = 0.0f;
        actor->mv->unk6B = 0;
        actor->count5 = 0;
        actor->inwater = 1;
    }
}

/* Clears inwater, underwaterTime and mv->ladder, sets gravity 4, speed 15; a living actor also gets norun 6. */
void actor_leaveWater(Actor *actor) {
    actor->underwaterTime = 0;
    actor->mv->ladder = 0;
    if (actor->energy != 0) {
        actor->count3 = 0;
        actor->count5 = 0;
        actor->norun = 6;
    }
    actor->inwater = 0;
    actor->ygrav = 4.0f;
    actor->vtilt = 0.0f;
    actor->speed = 15.0f;
}

void func_15052458(Actor *unused) {
}

/* Calls actor_remove(actor, 1) once the actor's health reaches zero. */
void actor_removeIfDead(Actor *actor) {
    if (actor->energy == 0) {
        actor_remove(actor, 1);
    }
}

/*
 * Sets unk40 to the facing angle plus a quarter turn, in degrees, and adds amount * scale to unkB8,
 * subtracting it instead when `angle` lies in the half-turn behind that direction.
 */
void func_15052490(Actor *actor, u16 angle, f32 amount, f32 scale) {
    scale *= gActorTimeStep;

    actor->theta = (actor->langle + 0x4000) * (360.0f / 65536.0f);

    if ((angle - actor->langle + 0x4000) & 0x8000) {
        actor->vtilt -= amount * scale;
    } else {
        actor->vtilt += amount * scale;
    }
}

/* Steps unkCC toward unkCE by a twelfth of the gap plus one, without overshooting. */
void func_1505250C(Actor *actor, s32 unused) {
    s16 target = actor->unkCE;
    s16 current = actor->unkCC;

    if (target != current) {
        if (current < target) {
            actor->unkCC = current + (target - current) / 12 + 1;
            if (target < actor->unkCC) {
                actor->unkCC = target;
            }
        } else {
            actor->unkCC = current + (target - current) / 12 - 1;
            if (actor->unkCC < target) {
                actor->unkCC = target;
            }
        }
    }
}

/* Object update step shared by several behaviours: runs the common per-object helpers. */
void actor_updateCommon(Actor *obj) {
    s32 flags;

    flags = obj->task_attrib;
    if (flags & 0x80) {
        obj->task_attrib = flags & ~0x80;
        particle_spawnBoneSpriteEmitter(dc_glbl_CharNum, 3, 0x3FC, 6, 0xFF, 0);
        particle_spawnBoneSpriteEmitter(dc_glbl_CharNum, 2, 0x3FC, 6, 0xFF, 0);
    }

    if ((gCutsceneStates[0] != 1) || ((D_800C3654[0] != 0) && (cutscene_isActorBound(obj) == 0))) {
        actor_stepScale(obj);
        func_1504BB88(obj);
        obj->count2 = 10;

        if (!obj->stunned) {
            actor_updateChaseAi(obj);
        } else if (obj->carried == 0) {
            actor_updateStunned(obj, 0.05f);
        }

        // facing angle (0x10000 units per turn) rotated a quarter turn, in degrees
        obj->theta = (s16)(obj->langle + 0x4000) * (360.0f / 65536.0f);
        actor_updatePhysics(obj);

        if ((obj->shad == 0.0f) && ((obj->floorMaterial & 0x1F) == 14)) {
            obj->energy = 0;
        }

        actor_checkGrab(obj);

        if (obj->carried != 0) {
            actor_moveToHost(obj);
        } else {
            obj->lean_angle += (0.0f - obj->lean_angle) * 0.2f;
        }

        func_15054F74(obj);
        if (actor_checkDeath(obj) == 0) {
            cutscene_applyObjectTrack(obj, 0, -1);
        }
    }
}

/**
 * Eases the object's unkC4 toward its turn rate: the change from prevAngle to the current moving angle,
 * scaled by 100 / gActorStepPercent.
 */
void actor_leanIntoTurn(Actor *obj, s16 prevAngle) {
    f32 blend;
    s8 direction;
    s32 divisor;
    s32 turnRate;
    s32 id;
    s32 rate;

    prevAngle = (prevAngle & 0xFFFF) - obj->angle;
    turnRate = (prevAngle * 100) / gActorStepPercent;
    id = obj->character_type;
    // matching: the redundant id == 0x29 test is emitted as its own branch
    if (((id == 0x29) || (id != 8)) && !(obj->unk22C & 8)) {
        blend = 0.07999999821f;
        divisor = 7;
        if (id == 10) {
            divisor = 40;
        }
        if (obj->unk226 == 0xFF) {
            direction = turnRate >> 8;
            if ((obj->unk87 ^ direction) & 0x80) {
                turnRate = 0;
            }
            blend = 0.3000000119f;
            divisor = 4;
            obj->unk87 = direction;
        }
        rate = obj->unk1E5;
        if (rate != 0) {
            obj->lean_angle += ( (turnRate / (rate * divisor)) - obj->lean_angle) * blend;
        }
    }
}


/**
 * Per-frame movement for a path-following object: picks a target height (the next path point, or the
 * object in unk222), eases ypos toward it, updates the pitch-like unkB8, then moves the object.
 */
void actor_updatePathFollower(Actor *obj) {
    f32 targetY;
    f32 climbSpeed;
    u32 pitch;
    s16 prevAngle;
    f32 floorY;
    s32 autoTilt;
    s32 heightMode; // 0: free, 1: approach targetY, 2: snapped to targetY
    f32 maxStep;
    f32 dy;
    f32 range;

    climbSpeed = (s8)obj->unk22B;
    prevAngle = obj->angle;
    floorY = gSpawnPaths[obj->listpos][0].y;
    autoTilt = 1;
    heightMode = 0;
    // matching: the u32 compare (a no-op on a u8) steers IDO's register allocation
    if ((obj->character_type == 0x4F) && ((u32)obj->attach_char != 0) && (obj->ob_matrix_ptr != NULL)) {
        obj->ypos = movement_obj[obj->attach_char].ypos + (movement_obj[obj->attach_char - 1].scale_y * 284.0f);
    }
    obj->noshove |= 0x10000;
    obj->count2 = 10;
    if (obj->character_type == 10) {
        targetY = movement_obj[obj->unk222].ypos + 80.0f;
    } else {
        targetY = gSpawnPaths[obj->listpos][obj->unk21E + 1].y;
    }
    if (obj->unk224 != 0) {
        targetY = obj->unk224;
    }
    if ((obj->unk223 == 14) || (obj->unk223 == 3)) {
        targetY = movement_obj[obj->unk222].ypos - 40.0f;
    }
    if ((obj->character_type == 0x4F) || (obj->character_type == 0x83)) {
        if (obj->unk251 == 1) {
            autoTilt = 0;
            pitch = actor_getDirectionAngle(obj->speed, obj->yspd, 0);
            obj->vtilt = pitch * -0.005493164f;
        } else {
            heightMode = 1;
            if (movement_obj[obj->owner - 1].playernumber == 0xFF) {
                obj->vtilt *= 0.200000003f;
            }
        }
    }
    if (obj->character_type == 8) {
        targetY = movement_obj[obj->unk222].ypos - 40.0f;
        if (obj->unk224 != 0) {
            targetY = obj->unk224;
        }
        if ((floorY < obj->ypos) && (obj->yspd > 0.0f)) {
            obj->yspd *= 0.8000000119f;
        }
        if (obj->unk223 != 3) {
            heightMode = 1;
        }
    }
    if (floorY < targetY) {
        targetY = floorY;
    }
    if (heightMode != 0) {
        maxStep = climbSpeed * gFrameDelta;
        dy = targetY - obj->ypos;
        if ((dy < maxStep) && (-maxStep < dy)) {
            heightMode = 2;
            obj->ypos += dy * 0.150000006f;
            obj->yspd = 0.0f;
            obj->ygrav = 0.0f;
        }
    }
    if (obj->unk226 == 0xFF) {
        f32 vel;
        if (obj->mv != NULL) {
            if (D_800BE9E4 < obj->mv->unk12C) {
                obj->mv->unk12C -= D_800BE9E4;
                if (obj->energy > 0) {
                    obj->speed = 254.0f;
                }
            } else {
                obj->mv->unk12C = 0;
            }
            if ((obj->mv->pressedButtons & 0x2000) && (obj->energy != 0)) {
                func_150D7790(obj, obj->mv->context_b & 0x7F);
            }
        }
        func_1504CA60(obj);
        vel = -obj->yspd;
        if (vel > 30.0f) {
            vel = 30.0f;
        }
        obj->vtilt += (vel - obj->vtilt) * 0.400000006f;
    } else {
        // matching: as above, the compare needs the u32 cast
        if ((heightMode != 2) && ((u32)obj->unk226 != 0)) {
            range = obj->unk226 * 0.1000000015f;
            func_15058EA4(obj, targetY, range, targetY, -range, climbSpeed, -climbSpeed);
        }
        if ((autoTilt != 0) && (obj->character_type != 0x49) && !(obj->unk22C & 8)) {
            obj->vtilt += ((obj->yspd * -2.5f) - obj->vtilt) * 0.1800000072f;
        }
    }
    if (obj->stunned != 0) {
        actor_updateStunned(obj, 0.0500000007f);
    } else {
        actor_updateChaseAi(obj);
    }
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    actor_updatePhysics(obj);
    if (autoTilt != 0) {
        actor_leanIntoTurn(obj, prevAngle);
    }
    if (obj->character_type == 0x9E) {
        obj->taskint[0] = 0;
    }
    if (((gIsMultiplayer != 0) && (dc_glbl_CharNum < D_8008FD90)) || (actor_checkDeath(obj) == 0)) {
        cutscene_applyObjectTrack(obj, 0, -1);
    }
}


/* Sets the object's immunity timer and heading (in degrees, from its facing angle) and starts animation 0. */
void func_15052EF0(Actor *obj) {
    obj->imune = 100;
    // binary angle (0x10000 per turn) to degrees, a quarter turn ahead of the facing angle
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    actor_playAnim(obj, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

/* Resets object `index` (unk13C, unk218) and sets its unk232; returns the object. */
Actor *func_15052F58(s32 index, s32 arg1) {
    Actor *obj;

    obj = &movement_obj[index];
    obj->lifting = 0;
    obj->scriptCmd = NULL;
    obj->intelwalk = arg1;
    return obj;
}

/*
 * When the object linked by arg0->unk124 is falling below arg0's top (y + height * scale_y), or always if
 * `force` is set: records it on arg0 (unk13C), copies arg0's facing angle to it and resets its state; with
 * arg0 in type 0x20 it is also stopped and turned to face arg0's way.
 */
void actor_seatRider(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9) {
    Actor *obj;
    s32 anim;
    s32 callF28;
    ModelAttachment *info;
    u16 pad; // matching: unused stack slot
    s32 state;

    callF28 = FALSE;
    anim = -1;
    if (arg0->character_type == 0x48) {
        info = attach_getFirst(arg0);
        if (info != NULL) {
            anim = info->setId;
        }
    }
    obj = &movement_obj[arg0->owner];
    if (force || ((obj->ypos < arg0->ypos + height * arg0->scale_y) && (obj->yspd < 0.0f))) {
        arg0->lifting = arg0->owner + 100;
        if (arg9 != 0) {
            gEventParam = arg9;
            func_1506E5FC();
        }
        obj->unk1CC = obj->ypos;
        obj->angle = arg0->langle;
        obj->mv->gpoint = 0;
        if (arg0->type == 0x20) {
            obj->yspd = 0.0f;
            state = 2;
            arg0->yspd = 0.0f;
            if (arg0->character_type == 0x48) {
                if (anim == 0x7A) {
                    arg0->attach_char = arg0->owner + 1;
                    arg0->basemtx = arg2;
                    arg0->unk101 |= arg3;
                    actor_playAnim(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
                    collision_createPartSet(obj, 0x79, 1.0f, 0);
                    callF28 = TRUE;
                }
            } else {
                arg0->scale_y = 0.75f;
                obj->ypos = arg0->ypos;
                state = 1;
                if (level == 6) {
                    state = 0x81;
                }
            }
            obj->angle = arg0->langle;
            obj->fangle = obj->angle;
            obj->langle = obj->angle;
            obj->theta = (s16)(obj->angle + 0x4000) * 0.005493164f; // binary angle to degrees
            obj->mv->transport = state;
            obj->count5 = 0;
            obj->norun = 0;
        // matching: an empty label here makes IDO fill the branch delay slot as the original
        end_hold:;
        } else {
            obj->attach_char = dc_glbl_CharNum + 1;
            obj->basemtx = arg2;
            obj->unk101 |= arg3;
            obj->norun = 0;
        }
        obj->imune = immune;
        obj->intelhitroute = arg7;
        obj->mv->jump_on_char = 0;
        arg0->intelwalk = arg5;
        arg0->scriptCmd = NULL;
        if (arg4 != 0) {
            actor_fadeCommand(obj, 1, 0, 0, 0);
            obj->unk101 |= 0x40;
        }
    }
    if (callF28) {
        actor_remove(arg0, 0);
    }
}

/*
 * Launches the object linked by arg0->unk124 (velocity/gravity given), starts animation 50 and sets its
 * mv target x/z to arg0's position, or to the translation of matrix `mtxIndex` of arg0->ob_matrix_ptr.
 */
void actor_startRiderJump(Actor *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex) {
    s32 pad[2]; // matching: frame keeps 8 unused bytes above pos
    Vec3f pos;
    Actor *obj;

    obj = &movement_obj[arg0->owner];
    if ((obj->norun == 0) && (obj->stunned == 0)) {
        if ((obj->mv != NULL) && (obj->mv->unk6B != 0)) {
            return;
        }
        obj->imune = 0xFF;
        obj->yspd = yVelocity;
        obj->ygrav = gravity;
        obj->count5 = 0xFF;
        obj->norun = 0xFF;
        obj->angle = arg0->langle;
        arg0->intelwalk = arg3;
        arg0->scriptCmd = NULL;
        actor_playAnim(obj, 50, 1.3f, 4.0f, 0.0f, 0.0f, 0);
        movement_obj[arg0->owner].mv->gpoint = 70;
        movement_obj[arg0->owner].mv->swing_xvel = arg0->xpos;
        movement_obj[arg0->owner].mv->swing_zvel = arg0->zpos;
        if ((arg0->ob_matrix_ptr != NULL) && (mtxIndex != -1)) {
        // matching: an unused label keeps IDO from sinking the index shift below the -1 test
        joint:
            math_mtxGetTranslation(&pos, &((Mtx *)arg0->ob_matrix_ptr)[mtxIndex]);
            movement_obj[arg0->owner].mv->swing_xvel = pos.x;
            movement_obj[arg0->owner].mv->swing_zvel = pos.z;
        }
        movement_obj[arg0->owner].mv->jump_on_char = 1;
    }
}

/**
 * Death handling for obj (called once its health is 0): picks the outcome from the
 * flag byte of its gModelDefs entry. Returns 0 only when unk10E is in 1..0xEF.
 */
s32 actor_handleDeath(Actor *obj) {
    u8 flags;

    if (obj->character_type == 0xFF) {
        actor_remove(obj, 1);
        return 1;
    }

    flags = gModelDefs[obj->character_type]->flags;
    if ((obj->unk10E != 0) && (obj->unk10E < 0xF0)) {
        actor_startKnockback(obj, 9);
    } else {
        s32 result = 0;

        if ((obj->unk10E < 0xF0) && (flags & 0x20)) {
            particle_spawnPuffQuad(obj->xpos, obj->ypos + 50.0f, obj->zpos, 0.5f, 0xFF, 0);
        }
        if (flags & 4) {
            result = actor_runDeathEffect(obj);
        }
        if (flags & 2) {
            obj->type = 42;
        } else if (flags & 0x80) {
            func_15055B0C(obj, 1200);
        } else if (flags & 1) {
            func_15053694(obj);
        } else if (flags & 0x40) {
            gSpawnDelays[obj->listpos] = 1250;
            actor_remove(obj, 1);
        } else if (result == 1) {
            func_150536D0(obj);
        } else if (result == 0) {
            actor_remove(obj, 1);
        }
        return 1;
    }
    return 0;
}

/**
 * Runs actor_handleDeath for obj once its health is 0 and its gModelDefs flags (or
 * unk10F == 0) allow it. Returns 0 when nothing was done.
 */
s32 actor_checkDeath(Actor *obj) {
    u8 flags;

    if (obj->energy == 0) {
        if (obj->character_type == 0xFF) {
            actor_remove(obj, 1);
            return 1;
        }
        flags = gModelDefs[obj->character_type]->flags;
        if ((flags & 0x10) ||
            ((flags & 8) && (obj->shad == 0.0f)) ||
            (obj->unk10F == 0)) {
            return actor_handleDeath(obj);
        }
    }
    return 0;
}

/** Sets obj's interaction state to 8 and starts actor_fadeCommand mode 6. */
void func_15053694(Actor *obj) {
    obj->type = 8;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    actor_fadeCommand(obj, 6, 0, 0, 0);
}

void func_150536D0(Actor *obj) {
    obj->type = 13;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
}

void func_150536E8(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
}

void func_1505371C(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
}

// Moves the object's scale towards (unk154, unk158) by at most unk15C per call.
void actor_stepScale(Actor *obj) {
    f32 diff;
    f32 step;

    diff = obj->unk154 - obj->scale_xz;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            actor_setScale(obj, obj->unk154, obj->scale_y);
        } else {
            actor_setScale(obj, (diff < 0.0f ? -1 : 1) * step + obj->scale_xz, obj->scale_y);
        }
    }

    diff = obj->unk158 - obj->scale_y;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            actor_setScale(obj, obj->scale_xz, obj->unk158);
        } else {
            actor_setScale(obj, obj->scale_xz, (diff < 0.0f ? -1 : 1) * step + obj->scale_y);
        }
    }
}

void func_15053894(Actor *obj) {
    obj->count1++;
    if (obj->count1 >= 31) {
        actor_remove(obj, 1);
    }
}

void func_150538CC(Actor *obj) {
    s8 done;
    s32 left;
    f32 prevY;
    s32 kind;
    s32 hitFlags;
    s32 sweep;
    s32 slot;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 maxStep;
    f32 slotValue;
    s32 searchDir;
    s32 slotFlag;
    Actor *target;

    done = 0;
    kind = obj->headEffectState;
    sweep = obj->unk284;
    hitFlags = 0;
    if (((kind == 2) || (kind == 18)) && obj->headEffectTimer) {
        if (D_800BE9E4 < obj->headEffectTimer) {
            obj->headEffectTimer = obj->headEffectTimer - D_800BE9E4;
        } else {
            // matching: dead leftover; keeps the target's dead `move a0,v0` (as1 drops the subu, not the copy)
            left = obj->headEffectTimer - D_800BE9E4;
            if (left + left) {
            }
            obj->headEffectTimer = 0;
            obj->ygrav = 1.0f;
        }
    }
    if ((kind == 13) || (kind == 14)) {
        slot = mp_findRacer(obj);
        if (slot != -1) {
            if (kind == 13) {
                dist = 0.200000003f;
                searchDir = 1;
                maxStep = 200.0f;
            } else {
                dist = 0.0f;
                searchDir = -1;
                maxStep = 30.0f;
            }
            mp_updateRacer(slot, 0, dist);
            obj->unk328 = 1; // 0x328
            if (obj->unk221 == -1) {
                target = mp_findAdjacentRacer(slot, searchDir, &slotValue, &slotFlag);
                if ((obj->owner != 0) && (target + 1 == &movement_obj[obj->owner])) {
                    target = NULL;
                }
            } else {
                target = &movement_obj[obj->unk221];
            }
            if ((target != NULL) && ((target->mv->death_State != 0) || (target->imune > 0))) {
                target = NULL;
                obj->unk221 = -1;
            }
            if (target != NULL) {
                dx = target->xpos - obj->xpos;
                dy = target->ypos - obj->ypos;
                dz = target->zpos - obj->zpos;
                dist = (dx * dx) + (dz * dz);
            } else {
                slotFlag = (D_8008FDBC & 0x40) != 0;
                slotValue = 0.5f;
            }
            if (obj->count3 < 10) {
                target = NULL;
            }
            if ((target == NULL) || (dist > 1000000.0f) || (kind == 14)) {
                dx = gRaceTargetX - obj->xpos;
                dy = (gRaceTargetY + 100.0f) - obj->ypos;
                dz = gRaceTargetZ - obj->zpos;
                gRacers[slot].lateral += (slotValue - gRacers[slot].lateral) * 0.125f;
                gRacers[slot].route = slotFlag;
            } else {
                obj->unk221 = target - movement_obj;
            }
            dist = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
            if (dist > 1.0f) {
                if (dist < maxStep) {
                    maxStep = dist;
                }
                dist = maxStep / dist;
                dx *= dist;
                dy *= dist;
                dz *= dist;
                obj->angle = actor_getDirectionAngle(-dx, dz, 0);
            }
            obj->xpos += dx;
            obj->ypos += dy;
            obj->zpos += dz;
            {
                f32 groundY;

                if (collision_getFloorNearY(obj->xpos, obj->zpos, obj->ypos + 50.0f, &groundY) > 0) {
                    if (obj->ypos < groundY + 50.0f) {
                        obj->ypos = groundY + 50.0f;
                    }
                }
            }
            obj->norun = 1;
            obj->unk103 = 1;
            obj->speed = 0.0f;
        }
        obj->underwaterTime++;
        obj->underwaterTime &= 1;
        if (obj->underwaterTime != 0) {
            obj->count5++;
        }
        if ((gMatchResult != 0) || (gFrontEndState != 0)) {
            obj->count5 = 1;
            done = 1;
        }
        if (obj->count3 < 10) {
            obj->count3++;
        }
    }
    if (obj->count2 < 100) {
        s32 pad[2]; // matching: unused; shifts the block's stack slots to the target's offsets
        f32 ownerDist;
        f32 climb;

        if (obj->unk102 != 0) {
            obj->count4 = 40;
        }
        obj->fangle = actor_getDirectionAngle(movement_obj[obj->unk126].xpos - obj->xpos, obj->zpos - movement_obj[obj->unk126].zpos, 0);
        if ((u8)(((obj->fangle - obj->angle) >> 8) + obj->count4) < (obj->count4 * 2)) {
            climb = ((movement_obj[obj->unk126].ypos + obj->unk224) - obj->ypos) * 0.1800000072f;
            ownerDist = actor_distance3D(&movement_obj[obj->unk126], obj);
            actor_turnMoving(obj, 0x300);
            if ((obj->unk102 != 0) && (ownerDist < (obj->speed * 16.0f))) {
                if (climb > 90.0f) {
                    climb = 90.0f;
                } else if (climb < -90.0f) {
                    climb = -90.0f;
                }
                obj->yspd += (climb - obj->yspd) * 0.1800000072f;
            }
        }
    }
    if (obj->count4 >= 9) {
        obj->count4--;
    }
    if (obj->unk103 != 0) {
        actor_moveHorizontal(obj);
    }
    switch (kind) {
    case 2:
    case 12:
    case 18:
        if (obj->speed != 0.0f) {
            obj->vtilt += 15.0f * D_800BE9E4;
            if (sound_isActorSound2Playing(obj) == 0) {
                sound_playActorSound2((math_rand() % 2) + 0x22F, obj, 30000, 500, 1000);
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
    case 13:
    case 15:
    case 17:
        obj->vtilt = -math_atan2(obj->yspd, obj->speed) * 57.29577637f;
        switch (kind) {
        case 4:
        case 5:
        case 9:
        case 10:
        case 13:
            if (obj->unkA8 < 30) {
                obj->unkA8 += D_800BE9E4;
            } else if (sound_isActorSound2Playing(obj) == 0) {
                sound_playActorSound2((math_random() % 3U) + 0x2EE, obj, 30000, 500, 1000);
            }
            break;
        case 3:
        case 6:
        case 11:
        case 17:
            if ((actor_findPlayerAhead(obj, obj->owner, 0x200, 15.0f) != -1) && (obj->unkA9 == 0)) {
                sound_playActorSound2((math_random() & 3) + 0x43E, obj, 30000, 2000, 2100);
                obj->unkA9 = 1;
            }
            break;
        }
        break;
    }
    obj->stunned = 1;
    if (obj->unk103 != 0) {
        actor_applyGravity(obj);
    }
    D_800CC254 = obj->xpos;
    D_800CC258 = obj->ypos;
    D_800CC25C = obj->zpos;
    prevY = obj->ypos;
    actor_updateFloorAndWalls(obj, obj->old_ypos);
    actor_landNonPlayer(obj, obj->floorY);
    if (sweep != 0) {
        f32 dir[3];
        f32 len;
        f32 inv;
        s32 hit;
        Actor *owner;

        dir[0] = obj->xpos - obj->old_xpos;
        dir[1] = obj->ypos - obj->old_ypos;
        dir[2] = obj->zpos - obj->old_zpos;
        len = (dir[0] * dir[0]) + (dir[1] * dir[1]) + (dir[2] * dir[2]);
        len = sqrtf(len);
        if (len != 0.0f) {
            inv = 1.0f / len;
        } else {
            inv = 9.999999747e-05f;
        }
        dir[0] *= inv;
        dir[1] *= inv;
        dir[2] *= inv;
        collision_castRay(obj, obj->old_xpos, obj->old_ypos, obj->old_zpos, dir[0], dir[1], dir[2], &obj->mv->rayHit, len, 0, 0, 0, -1, 0, 0);
        hit = collision_applyRayHit(obj, &obj->mv->rayHit, kind);
        if (hit != 0) {
            done = 1;
            if ((hit == 1) && (obj->owner > 0)) {
                owner = &movement_obj[obj->owner - 1];
                if ((owner->mv != NULL) && (owner->mv->unkAF != 0)) {
                    if (sound_isActorSound1Playing(owner) == 0) {
                        actor_playSound((math_random() % 3U) + 0x53B, owner);
                    }
                }
            }
        }
    } else {
        if ((obj->unk103 != 0) && ((obj->mvmt_control & 0x18000) || (prevY != obj->ypos))) {
            if (obj->speed > 0.0f) {
                hitFlags = 1;
                obj->speed *= 0.5f;
                if ((kind == 0) && (obj->speed > 20.0f) && (obj->mvmt_control & 0x10000)) {
                    s32 sfx;

                    sfx = math_random() % 9U;
                    actor_playPlayerSound((sfx < 2) ? sfx + 0x18 : sfx + 0x29, obj, 30000, 0, 500, 1500, 5);
                    particle_spawnWakePuff(dc_glbl_CharNum, 0, 0x14, 0, 0x199, 4, 0xAA, 0xFF, 0);
                }
            }
        }
        if (obj->doing_hit != 0) {
            s32 hit;

            hit = actor_testAttack(obj, dc_glbl_CharNum);
            if (hit != 0) {
                gAttackHitMask = hit;
                obj->norun = 1;
                hitFlags |= 8;
                obj->speed *= 0.3000000119f;
                if (kind == 15) {
                    Actor *other;

                    other = collision_findObjectById(6);
                    if (other != NULL) {
                        if (other->stunned != 0) {
                            obj->noshove |= 0x01000000;
                            obj->count5 = 6;
                        }
                        obj->angle = actor_getDirectionAngle(other->xpos - obj->xpos, obj->zpos - other->zpos, 0);
                        obj->yspd *= 0.400000006f;
                    }
                } else {
                    s32 attacker;

                    attacker = obj->unk108 & 0x7F;
                    if (attacker < 25) {
                        obj->angle = actor_getDirectionAngle(obj->old_xpos - movement_obj[attacker].xpos, movement_obj[attacker].zpos - obj->old_zpos, 0);
                    }
                    obj->yspd = 35.0f;
                    obj->ygrav = 6.0f;
                }
            }
        }
        if (kind == 15) {
            actor_accelerate(0.0f, obj, 1.0f, 1.0f, 0);
            actor_resolveCollisions(obj, dc_glbl_CharNum);
            if ((gCollidedActors >> 1) != 0) {
                Actor *other;

                other = collision_findObjectById(6);
                if (gCollidedActors & (1U << (other - movement_obj))) {
                    gEventParam = 0xFF0100A7;
                    func_1506E6FC();
                    if (other != NULL) {
                        obj->angle = actor_getDirectionAngle(obj->xpos - other->xpos, other->zpos - obj->zpos, 0);
                    }
                    obj->yspd = 38.0f;
                    obj->ygrav = 5.0f;
                    obj->speed = 40.0f;
                }
            }
        }
        if ((obj->unk103 != 0) && (obj->shad == 0.0f)) {
            hitFlags |= 2;
        }
        if (hitFlags != 0) {
            if (kind == 17) {
                obj->energy = 0;
            }
            if (gIsMultiplayer != 0) {
                obj->energy = 0;
            }
        }
        obj->theta = (obj->angle + 0x4000) * 0.005493164f;
        if (obj->shad == 0.0f) {
            obj->count1++;
            if (obj->intelwalk != 0) {
                obj->ypos = obj->old_ypos;
                done = 1;
            }
        }
    }
    if (obj->count1 >= 31) {
        done = 1;
    }
    if (--obj->count5 == 0) {
        done = 1;
    }
    if (obj->energy == 0) {
        done = 1;
    }
    if (done != 0) {
        switch (kind) {
        case 13:
            if (hitFlags & 8) {
                effect_spawnRayHitDebris((Vec3f *)&obj->xpos, 0xFF, 1); // xpos, ypos, zpos as one Vec3f
            } else {
                Vec3f vec;

                vec.x = D_800CC254 - obj->old_xpos;
                vec.y = D_800CC258 - obj->old_ypos;
                vec.z = D_800CC25C - obj->old_zpos;
                if (math_vec3Normalize(&vec, &vec, NULL, NULL) != 0) {
                    effect_spawnRaySurfaceImpact(NULL, (Vec3f *)&obj->xpos, &vec, 0xFF, 1); // xpos, ypos, zpos as one Vec3f
                }
            }
            mp_releaseSpareRacer(obj);
            break;
        case 14:
            mp_releaseSpareRacer(obj);
            break;
        }
        actor_remove(obj, 1);
        return;
    }
    obj->unk103 = 1;
}

void func_150548D8(Actor *obj) {
}

void func_150548E4(Actor *obj) {
    Actor *owner;
    s32 sfx;
    s32 i;

    owner = &movement_obj[obj->attach_char - 1];

    for (i = 0; i < D_8008FD8C; i++) {
        if ((movement_obj[i].stand_on_char == (obj - movement_obj) + 1) && (owner->intelwalk == 1)) {
            owner->scriptCmd = NULL;
            if (math_random() & 1) {
                owner->intelwalk = 2;
                sfx = 0x1DB;
            } else {
                owner->intelwalk = 3;
                sfx = 0x1DC;
            }
            sound_playActorSound1(sfx, owner, 0x7D00, 500, 2500);
        }
    }
}

void actor_removeAtAnimEnd(Actor *obj) {
    if (obj->unk2D0 != NULL) {
        if ((obj->unk2D0->numFrames[0] - 1.0f) <= obj->unk2D0->frame[0]) {
            actor_remove(obj, 0);
        }
    }
}

void func_15054A5C(Actor *obj, Actor *other) {
    other->mv->weapon_status = 0;
    particle_stopObjEffect(other);
    actor_remove(obj, 0);
}

void func_15054A94(Actor *obj) {
    AnimInstance *anim;
    Actor *other;
    AnimInstance *otherAnim;
    s32 motion;
    s32 attach;
    s32 action;
    s32 hold;
    s32 enter;
    ActorCtl *state;
    s32 index;
    u8 ready;

    anim = obj->unk2D0;
    if (anim == NULL || obj->attach_char == 0) {
        return;
    }
    other = &movement_obj[obj->attach_char - 1];
    otherAnim = other->unk2D0;
    if (otherAnim == NULL) {
        return;
    }
    if (obj->character_type == 0x41) {
        if (other->animseq.uh != 0x48 && other->animseq.uh != 0x4B) {
            actor_remove(obj, 0);
            return;
        }
        if (obj->animseq.uh == 0 && anim->frame[0] == 0.0f) {
            trail_startActorAfterimage(obj, 100, 0);
        }
        if (obj->animseq.uh == 0 && other->animseq.uh == 0x4B) {
            actor_playAnim(obj, 1, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        }
        if (obj->animseq.uh == 1 && anim->numFrames[0] - 1.0f <= anim->frame[0]) {
            actor_remove(obj, 0);
        }
        return;
    }
    if (obj->character_type != 0x1D && obj->character_type != 0x6E && obj->character_type != 0x2F) {
        return;
    }
    if (obj->attach_char == 0 || other->attach_char != 0 || other->energy == 0) {
        func_15054A5C(obj, other);
        return;
    }
    state = other->mv;
    if (state->rt_context_b != 0x12 && state->rt_context_b != 0x18 && state->rt_context_b != 0x41 && state->weapon_status != 3) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->character_type == 0x1D) {
        enter = 0xC4;
        hold = 0xC7;
        action = 0x39;
    } else if (obj->character_type == 0x6E) {
        enter = 0xFB;
        hold = 0xFC;
        action = 0x51;
    } else {
        enter = 0x2C8;
        hold = 0x2C9;
        action = 0x51;
    }
    if (other->lifting != 0) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->character_type == 0x1D && other->stunned != 0) {
        func_15054A5C(obj, other);
        return;
    }
    attach = 1;
    if (hold == other->animseq.uh) {
        motion = 2;
        if (obj->taskint[0] == 0) {
            if (otherAnim->frame[0] >= 6.0f && otherAnim->frame[0] < 11.0f) {
                obj->taskint[0] = 1;
                collision_createPartSet(obj, action, 1.0f, 0);
            }
        } else if (otherAnim->frame[0] >= 11.0f) {
            obj->taskint[0] = 0;
            collision_removePartSet(obj, action);
        }
    } else {
        if (obj->taskint[0] == 1) {
            obj->taskint[0] = 0;
            collision_removePartSet(obj, action);
        }
        if (enter == other->animseq.uh) {
            motion = 0;
        } else if (other->animseq.uh == 0xC6) {
            motion = 3;
        } else {
            motion = 1;
            attach = 0;
        }
    }
    actor_playAnim(obj, motion, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    if (attach) {
        anim_copyTime(obj, other);
    }
    state = other->mv;
    if (state->rt_context_b == 0x12) {
        ready = 1;
    } else {
        ready = state->SniperState;
        if (state->bot == 0) {
            index = other - movement_obj;
            // gPadTable[0] points to the per-player controller pads
            if (index < 4 && !(gPadTable[index]->button & R_TRIG)) {
                ready = 0;
            }
        }
    }
    if (other->animseq.uh == 0x10A && otherAnim->unk3C <= 0 && ready &&
        (state->buttons & 0x2000)) {
        if (obj->taskint[1] == 0) {
            particle_startObjRayBeam(other, -1, 3, 1, 0xFF, 1);
            obj->taskint[1] = 1;
        }
    } else if (obj->taskint[1] == 1) {
        obj->taskint[1] = 0;
        particle_stopObjEffect(other);
    }
}

void func_15054F74(Actor *obj) {
    s16 angle;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    s16 x;
    s16 z;

    if (obj->character_type == 12) {
        if ((dc_glbl_CharNum + 1) != movement_obj[obj->owner].attach_char) {
            if (obj->intelwalk == 18) {
                actor_seatRider(obj, 50.0f, 0, 4, 0, 12, 0, 8, 0, 0);
            } else if (gCollidedActors & 1) {
                if (obj->task_attrib & 0x400) {
                    obj->owner = 0;
                    actor_startRiderJump(obj, 39.0f, 3.0f, 18, 0);
                }
            }
        }
    } else if (obj->character_type == 0x4B) {
        actor_splitVelocity(obj->langle ^ 0x8000, 500.0f, 0.0f, &offsetX, &offsetZ, &offsetY);
        x = (s16)obj->xpos + offsetX;
        z = (s16)obj->zpos + offsetZ;
        obj->taskint[2] = (x << 16) | (z & 0xFFFF);
        if (obj->animseq.uh != 15) {
            angle = actor_getDirectionAngle(movement_obj[0].xpos - obj->xpos, obj->zpos - movement_obj[0].zpos, 0);
        } else {
            angle = obj->langle - 0x800;
        }
        angle -= obj->langle;
        angle /= 3;
        obj->langle += angle;
        obj->scale_xz = 2.0f;
        obj->scale_y = 2.0f;
        obj->theta = 90.0f;
    // matching: an empty labelled statement ends the block so as1 keeps the last store out of the branch delay slot
    label:
        ;
    } else if (((obj->character_type == 0x1B) || (obj->character_type == 0x7E)) && (obj->taskint[0] != 0)) {
        if (gIsMultiplayer != 0) {
            mp_addZone(obj->xpos, obj->ypos, obj->zpos, 900.0f);
        }
        if (D_800BE9E4 < obj->taskint[0]) {
            if ((obj->taskint[0] >= 120) && (obj->taskint[0] < (D_800BE9E4 + 120))) {
                sound_playActorSound2(0x3A1, obj, 0x7D00, 500, 1000);
            }
            obj->taskint[0] -= D_800BE9E4;
        } else {
            obj->intelwalk = 6;
            obj->scriptCmd = NULL;
        }
    }
}

void func_15055260(Actor *obj, Actor *target, s32 hitIndex) {
    attach_attachInPlace(obj, target, 0x4D, hitIndex, 0, 0);
}

void func_1505528C(Actor *obj) {
    f32 prevX;
    f32 prevY;
    f32 prevZ;
    s32 alpha;
    s32 kind;
    f32 hoverHeight;
    s32 hitFlags;
    s32 timer;
    Actor *target;

    kind = obj->headEffectState;
    hitFlags = 0;
    hoverHeight = 0.0f;

    switch (kind) {
    case 0x1F:
        obj->vtilt += obj->speed * D_800BE9E4;
        if (obj->taskint[0] == 1) {
            sound_playActorSound1(0x230, obj, 0x7D00, 500, 2500);
            obj->taskint[0] = 0;
        }
        break;
    case 0x42: {
        s32 hit;

        hoverHeight = 25.0f;
        if ((obj->ygrav != 0.0f) && ((obj->yspd > 0.0f) || (obj->shad > hoverHeight))) {
            obj->vtilt = -math_atan2(obj->yspd, obj->speed) * 57.29577637f;
        }
        hit = collision_findHitSphereTarget(obj, 40.0f, 10.0f, &target, 0xF, 0x53);
        if (hit != 0) {
            func_15055260(obj, target, hit - 1);
            actor_remove(obj, 1);
            return;
        }
        break;
    }
    case 0x4E:
        hoverHeight = 25.0f;
        if (obj->shad > hoverHeight) {
            obj->vtilt += D_800BE9E4 * 3;
        }
        break;
    case 0x35:
        obj->vtilt += D_800BE9E4 * 16;
        break;
    case 0x88:
        obj->vtilt = -math_atan2(obj->yspd, obj->speed) * 57.29577637f;
        obj->doing_hit = 16;
        if (actor_testAttack(obj, dc_glbl_CharNum) != 0) {
            actor_remove(obj, 1);
            return;
        }
        break;
    }
    actor_moveHorizontal(obj);
    obj->stunned = 1;
    actor_applyGravity(obj);
    prevX = obj->xpos;
    prevY = obj->ypos;
    prevZ = obj->zpos;
    actor_updateFloorAndWalls(obj, obj->old_ypos);
    if ((obj->waterHeight != -10000.0f) && (obj->ypos <= obj->waterHeight) && (obj->waterHeight <= obj->old_ypos)) {
        obj->speed *= 0.1000000015f;
        obj->taskint[0] = 2;
        obj->ygrav = 0.0f;
        obj->yspd = -3.0f;
    } else if ((prevX != obj->xpos) || (prevY != obj->ypos) || (prevZ != obj->zpos)) {
        hitFlags = 1;
        if ((kind == 0x1F) && (obj->taskint[0] != 2)) {
            sound_playActorSound1((math_random() % 5U) + 0x3A4, obj, 0x7D00, 500, 2500);
        }
    }
    actor_landNonPlayer(obj, obj->floorY);
    if (obj->shad <= hoverHeight) {
        obj->count1++;
        hitFlags |= 2;
        if ((obj->floorMaterial & 0x1F) == 0xE) {
            particle_spawnRandomChannelParticle((Vec3f *)&obj->xpos, 0xFF, 0); // xpos, ypos, zpos as one Vec3f
            actor_remove(obj, 1);
            return;
        }
        if (kind == 0x42) {
            f32 t;
            f32 y;

            gEventParam = 0xFF0100B0;
            if (obj->ygrav != 0.0f) {
                func_1506E5FC();
            }
            // matching: the int 0 stores below (not 0.0f) steer where IDO materializes its shared FP zero
            y = obj->ypos;
            obj->ygrav = 0;
            t = y - obj->old_ypos;
            obj->yspd = 0;
            obj->speed = 0;
            obj->ypos = obj->floorY + hoverHeight;
            if (t != 0.0f) {
                t = (obj->ypos - obj->old_ypos) / t;
            } else {
                t = 0;
            }
            obj->xpos = obj->old_xpos + ((obj->xpos - obj->old_xpos) * t);
            obj->zpos = obj->old_zpos + ((obj->zpos - obj->old_zpos) * t);
        } else if (obj->ypos < obj->waterHeight) {
            obj->ygrav = 0.0f;
            obj->yspd = 0.0f;
            obj->speed = 0.0f;
        } else if ((obj->yspd > 5.0f) && (kind == 0x1F) && (obj->taskint[0] != 2)) {
            sound_playActorSound1(0x39E, obj, 0x7D00, 500, 2500);
            obj->taskint[0] = 2;
        }
    }
    obj->theta = (obj->angle + 0x4000) * 0.005493164f;
    if ((hitFlags != 0) && (kind == 0x35)) {
        sound_playActorUntracked(0x360, obj, 0x4E20, 500, 2500);
        if (obj->waterHeight < obj->ypos) {
            Vec3f pos;

            pos.x = obj->xpos;
            pos.y = obj->ypos + 20.0f;
            pos.z = obj->zpos;
            particle_spawnSmallDebrisBurst(&pos, 0xFF, 1);
        }
        actor_remove(obj, 1);
        return;
    }
    timer = --obj->count5;
    if ((timer == 0) || (obj->count1 >= 31)) {
        actor_remove(obj, 1);
        return;
    }
    if (timer < 10) {
        alpha = (timer * 255) / 10;
    } else if (obj->count1 >= 21) {
        alpha = ((30 - obj->count1) * 255) / 10;
    } else {
        alpha = 255;
    }
    attach_setAlpha(obj, kind, alpha, 0);
    if (kind == 0x1F) {
        Camera *player;
        f32 distSq;

        player = camera_global_Cameras;
        distSq = ((player->eye.x - obj->xpos) * (player->eye.x - obj->xpos)) +
                 ((player->eye.y - obj->ypos) * (player->eye.y - obj->ypos)) +
                 ((player->eye.z - obj->zpos) * (player->eye.z - obj->zpos));
        if (distSq < 900.0f) {
            camera_startEnvelope(player, 8, 0);
            sound_playAt(0x2E1, 0x6D60, 0, 0, 0, obj->xpos, obj->ypos, obj->zpos, 500, 1000);
            sound_stopActorSound1(obj);
            actor_remove(obj, 1);
        }
    }
}

void actor_playImpactSound(s32 arg0, f32 x, f32 y, f32 z, s32 arg4) {
    s32 sfx;
    s32 range;

    range = 1000;
    if (arg4 == 1) {
        sfx = 1549;
        range = 100;
    } else {
        sfx = gImpactSounds[math_random() % 6U];
    }
    sound_playAt(sfx, 32700, math_random() % 500U, 0, 0, x, y, z, range, 3000);
}

void func_15055B0C(Actor *obj, s32 duration) {
    obj->type = 39;
    actor_playAnim(obj, obj->animseq.uh, 0, 0.0f, 0.0f, 0.0f, 0);
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->unk21C = duration;
}

void func_15055B64(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    obj->noshove |= 0x1000000;

    if (gFrameSteps < obj->unk21C) {
        obj->unk21C -= D_800BE9E4;
    } else {
        obj->unk21C = 0;
        if (obj->ob_matrix_ptr == NULL) {
            actor_remove(obj, 1);
        }
    }
}

void actor_updateSinkAway(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    obj->noshove |= 0x1000000;
    obj->ypos -= gFrameDelta;

    if (obj->ypos < (obj->floorY - 100.0f)) {
        actor_remove(obj, 0);
    }
}

void actor_startOverEdge(Actor *obj) {
    s32 sfx = 0x2007E4;

    if (!(obj->unk9C & 0x1000) && event_resolveSoundCommand(obj, &sfx, 1)) {
        actor_playSound(sfx, obj);
    }
    obj->mv->over_edge = 100;
    actor_fadeCommand(obj, 2, 0, 6, 0);
    obj->norun = 200;
    obj->count5 = 200;
    obj->unk1CC = 70000.0f;
    obj->type = 41;
    obj->ygrav = 0.0f;
    obj->yspd = 0.0f;
    obj->speed = 0.0f;
    actor_kill(obj, 9);
}

void actor_updateOverEdge(Actor *obj) {
    obj->mv->over_edge += gFrameSteps;
    if (obj->mv->over_edge >= 141) {
        gEliminatedPlayers |= 1 << (obj - movement_obj);
        if (gEliminatedPlayers != 0) {
            obj->imune = 0xFF;
            obj->stunned = 0xFF;
            obj->norun = 0xFF;
            obj->noshove |= 0x200;
            obj->bodyRadius = 0;
            obj->bodyHeight = 0;
            obj->unk7 = 0;
            obj->energy = 0;
            obj->type = 5;
            obj->speed = 0.0f;
            obj->yspd = 0.0f;
            obj->ygrav = 0.0f;
            return;
        }
        func_1506D538();
    }
    obj->energy = 0;
    obj->scale_xz *= 0.9499999881f;
    obj->scale_y *= 0.9499999881f;
    obj->speed *= 0.9499999881f;
}
