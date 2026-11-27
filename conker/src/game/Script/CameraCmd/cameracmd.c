#include <ultra64.h>
#include "structs.h"

/* A scripted actor move: actor `actor` is sent to pose `poseIndex` until `timer` runs out. */
typedef struct {
    /* 0x00 */ u8 state; // 0 = idle, 1 = start pending, 2 = running
    /* 0x01 */ u8 actor; // index into camera_global_Cameras
    /* 0x02 */ u8 poseIndex; // index into the gCameraPoses pose table
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 timer;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s32 flags;
    /* 0x10 */ f32 scale; // copied (truncated) to gPoseCommandScales when the command starts
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 savedCameraAngle; // actor->CameraAngle at setup, written to actor->CutbackAngle when done
} C3E20Command; // size 0x24

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
f32 gPendingCutbackAngle;
s32 gPoseCommandRunning; // a command is running
f32 gPoseSavedTargetY; // actor->unk2C0 saved while a command runs
C3E20Command gPoseCommands[3];
s32 gPoseCommandScales[4]; // per actor: the running command's scale

#include <math.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/script.h"
#include "conker/view.h"

/* Clears all command slots. */
void script_clearPoseCommands(void) {
    bzero(gPoseCommands, sizeof(gPoseCommands));
    gPoseCommandRunning = 0;
}

/* Returns TRUE if any of the first `count` command slots is active. */
s32 script_isPoseCommandActive(s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (gPoseCommands[i].state != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Advances command slot `slot`: starts the actor's move to its pose, then counts the timer down and
 * restores the actor when it runs out. Returns FALSE once the command has finished.
 */
s32 script_updatePoseCommand(s32 slot) {
    switch (gPoseCommands[slot].state) {
    case 1: {
        u8 camIndex;
        CamPose *pose;
        Camera *cam;

        camIndex = gPoseCommands[slot].actor;
        cam = &camera_global_Cameras[camIndex];
        pose = gCameraPoses;
        // matching: the redundant mask on the u8 index steers IDO's register allocation
        pose = &pose[gPoseCommands[slot].poseIndex & 0xFF];
        gPoseSavedTargetY = cam->target.y;
        gPoseCommandRunning = 1;
        gCameraSnapTimers[camIndex] = 3;
        gCamShot.mode = 1;
        gCamShot.scene = 0;
        gCamShot.eye.x = pose->x;
        gCamShot.eye.y = pose->y;
        gCamShot.eye.z = pose->z;
        gCamShot.at.aim.roll = pose->roll;
        gCamShot.at.aim.pitch = pose->pitch;
        gCamShot.at.aim.yaw = pose->yaw;
        gCamShot.fovOffset = 0.0f;
        camera_queueMessage(cam, 5, NULL);
        camera_queueMessage(cam, 7, &gCamShot);
        if (!(gPoseCommands[slot].flags & 1)) {
            cam->CharPtr->alpha = 0;
            cam->CharPtr->task_attrib |= 0x200;
        }
        gPoseCommandScales[slot] = gPoseCommands[slot].scale;
        gPoseCommands[slot].state = 2;
    }
        break;
    case 2: {
        Camera *cam;

        cam = &camera_global_Cameras[gPoseCommands[slot].actor];
        gPoseCommands[slot].timer -= gFrameSteps;
        if (cam->CharPtr->energy == 0) {
            gPoseCommands[slot].timer = 0;
        }
        if (gPoseCommands[slot].timer <= 0) {
            cam->CutbackAngle = gPoseCommands[slot].savedCameraAngle;
            if (!(gPoseCommands[slot].flags & 2)) {
                camera_queueMessage(cam, 6, NULL);
            }
            gPoseCommandRunning = 0;
            if (!(gPoseCommands[slot].flags & 1)) {
                cam->CharPtr->task_attrib &= ~0x200;
                cam->CharPtr->alpha = 0xFF;
            }
            gPoseCommands[slot].timer = 0;
            gPoseCommands[slot].state = 0;
            cam->target.y = gPoseSavedTargetY;
            return FALSE;
        }
    }
        break;
    }
    return TRUE;
}

/* Runs the active command slots until one of them is still busy. */
void script_updatePoseCommands(void) {
    s32 i;

    if (gCutsceneStates[0] != 1) {
        for (i = 0; i < 3; i++) {
            if (gPoseCommands[i].state != 0 && script_updatePoseCommand(i) != 0) {
                break;
            }
        }
    }
}

/*
 * Applies script command `opcode` with arguments cmd[2..] to camera `camIndex`.
 * Fixed-point arguments are 16.16. Returns 1 for some commands, else 0.
 */
s32 script_cameraCommand(s32 camIndex, s32 opcode, s32 *cmd) {
    Camera *cam;

    cam = &camera_global_Cameras[camIndex];
    if (camIndex > gScreenLayout) {
        return 0;
    }
    switch (opcode) {
    case 12:
        gPoseCommands[cmd[2]].savedCameraAngle = cam->CameraAngle;
        gPoseCommands[cmd[2]].actor = camIndex;
        gPoseCommands[cmd[2]].poseIndex = cmd[3];
        gPoseCommands[cmd[2]].timer = cmd[4];
        gPoseCommands[cmd[2]].state = 1;
        if (gScriptCmdWordCount >= 6) {
            gPoseCommands[cmd[2]].flags = cmd[5];
        } else {
            gPoseCommands[cmd[2]].flags = 0;
        }
        if (gScriptCmdWordCount >= 7) {
            gPoseCommands[cmd[2]].scale = cmd[6] * (1.0f / 65536.0f);
        } else {
            gPoseCommands[cmd[2]].scale = 1.0f;
        }
        if (!script_isPoseCommandActive(cmd[2])) {
            script_updatePoseCommand(cmd[2]);
        }
        break;
    case 4:
        camera_pushMode(cam, cmd[2], cmd[3], cmd[4], CAMERA_MODE_TASK);
        return 1;
    case 5:
        camera_popMode(cam, CAMERA_MODE_TASK);
        return 1;
    case 6:
        if (cmd[2] != 0) {
            cam->AvailableModes |= cmd[3];
        } else {
            cam->AvailableModes &= ~cmd[3];
        }
        return 1;
    case 8:
        if (cam->mv->death_State == 0) {
            cam->IdealTripodMagnitude = cmd[2];
            cam->idealHeight = cmd[3];
            cam->height = cmd[3];
        }
        return 1;
    case 7:
        cam->DistMask = cmd[2];
        cam->DistIndex = cmd[3];
        return 1;
    case 9:
        cam->Tightness = cmd[2];
        return 1;
    case 10: {
        CamPose *pose;

        pose = gCameraPoses;
        pose = &pose[cmd[2] & 0xFFF];
        gPoseCommandRunning = 1;
        gCameraSnapTimers[camIndex] = 3;
        gCamShot.mode = 1;
        gCamShot.scene = 0;
        gCamShot.eye.x = pose->x;
        gCamShot.eye.y = pose->y;
        gCamShot.eye.z = pose->z;
        gCamShot.at.aim.roll = pose->roll;
        gCamShot.at.aim.pitch = pose->pitch;
        gCamShot.at.aim.yaw = pose->yaw;
        gCamShot.fovOffset = 0.0f;
        D_800C365C[0] = 0;
        camera_queueMessage(cam, 5, NULL);
        camera_queueMessage(cam, 7, &gCamShot);
        movement_obj[0].task_attrib |= 0x200;
    }
        break;
    case 11:
        camera_queueMessage(cam, 6, NULL);
        gPoseCommandRunning = 0;
        movement_obj[0].task_attrib &= ~0x200;
        break;
    case 14:
        cam->focus.x = cmd[2];
        cam->focus.y = cmd[3];
        cam->focus.z = cmd[4];
        cam->target.x = cmd[2];
        cam->target.y = cmd[3];
        cam->target.z = cmd[4];
        cam->eye.x = cmd[5];
        cam->eye.y = cmd[6];
        cam->eye.z = cmd[7];
        cam->unk358 = cam->eye.y;
        camera_updateFloorHeights(cam);
        break;
    case 2:
        cam->eye.x = cmd[2];
        cam->eye.y = cmd[3];
        cam->eye.z = cmd[4];
        cam->unk358 = cam->eye.y;
        camera_updateFloorHeights(cam);
        break;
    case 3:
        cam->focus.x = cmd[2];
        cam->focus.y = cmd[3];
        cam->focus.z = cmd[4];
        break;
    case 15:
        cam->XZOffsetTget = cmd[2];
        if (gScriptCmdWordCount == 4) {
            cam->XZOffset = cmd[3];
        }
        break;
    case 16:
        cam->YOffsetTget = cmd[2];
        if (gScriptCmdWordCount == 4) {
            cam->YOffset = cmd[3];
        }
        break;
    case 17:
        if (gScriptCmdWordCount == 5 && cmd[4] != 0) {
            camera_startEnvelope(cam, cmd[2], cmd[3]);
            return 1;
        }
        if (gViewEnvelopes[camIndex].stage == 5) {
            camera_startEnvelope(cam, cmd[2], cmd[3]);
            return 1;
        }
        return 0;
    case 18:
        camera_attachToObject(cam, collision_findSlotBySpawnIndex(cmd[2] & 0xFFF));
        break;
    case 19:
        camera_setPairObject(cam, collision_findSlotBySpawnIndex(cmd[2] & 0xFFF));
        break;
    case 20:
        cam->TgetFOVX = cmd[2] * (1.0f / 65536.0f);
        cam->TgetFOVY = cmd[3] * (1.0f / 65536.0f);
        if (gScriptCmdWordCount == 5 && cmd[4] != 0) {
            cam->FOVX = cam->TgetFOVX;
            cam->FOVY = cam->TgetFOVY;
        }
        break;
    case 21: {
        CamPose *pose;

        pose = gCameraPoses;
        pose = &pose[cmd[2] & 0xFFF];
        pose->x = cmd[3];
        pose->y = cmd[4];
        pose->z = cmd[5];
    }
        break;
    case 22:
        cam->Snap = 1;
        break;
    case 23:
        cam->CentreBiasTarget = cmd[2] * (1.0f / 65536.0f);
        break;
    case 24:
        if (gScriptCmdWordCount == 5) {
            cam->aim.useOverride = 1;
            cam->aim.overrideYawOffset = cmd[2];
            cam->aim.overridePitch = cmd[3];
            cam->aim.overrideDistance = cmd[4];
        } else {
            cam->aim.useOverride = 0;
        }
        break;
    case 25:
        cam->CutbackAngle = cmd[2] * (1.0f / 65536.0f);
        gPendingCutbackAngle = cam->CutbackAngle;
        break;
    case 26:
        camera_reset(cam);
        break;
    case 27:
        gCameraCutsceneExitSkips = cmd[2] - 1;
        break;
    case 13:
        break;
    default:
        return 0;
    }
    return 0;
}


/*
 * Answers script query `opcode` about camera `camIndex` (arguments in cmd[2..]); fixed-point results
 * are 16.16.
 */
s32 script_cameraQuery(s32 camIndex, s32 opcode, s32 *cmd) {
    Camera *cam;
    s32 side;
    Vec3f facing;
    Vec3f toTarget;
    f32 angle;
    Actor *target;
    CamPose *pose;
    f32 dx;
    f32 dz;

    cam = &camera_global_Cameras[camIndex];
    if (camIndex > gScreenLayout) {
        return 0;
    }
    switch (opcode) {
    case 31:
        return cam->CharRelAngle * 65536.0f;
    case 30:
        return cam->CameraAngle * 65536.0f;
    case 29:
        return gPoseCommands[cmd[2]].state;
    case 32:
        // Horizontal distance from the actor to a pose.
        pose = gCameraPoses;
        pose = &pose[cmd[2] & 0xFFF];
        dx = cam->eye.x - pose->x;
        dz = cam->eye.z - pose->z;
        return sqrtf(dx * dx + dz * dz);
    case 33:
        // Which side of the actor's facing direction a path object is on: -1 or 1 (-1 if not found).
        target = actor_findOnPath(cmd[2] & 0xFFF);
        if (target == NULL) {
            return -1;
        }
        angle = cam->CameraAngleRad + 1.570796371f;
        facing.x = sinf(angle);
        facing.y = 0.0f;
        facing.z = cosf(angle);
        math_vec3Diff(&cam->eye, (Vec3f *)&target->xpos, &toTarget);
        toTarget.y = 0.0f;
        if (math_dot3(&facing.x, &toTarget.x) >= 0.0f) {
            side = -1;
        } else {
            side = 1;
        }
        return side;
    default:
        // matching: a plain `return 0` lets as1 fill the jump-table range check's delay slot
        goto unknown_query;
    }
unknown_query:
    return 0;
}

/* While a command runs, moves both actor points (unk2A4, unk2F8) along the unit vector between them, scaled by gPoseCommandScales. */
void script_dollyPoseCamera(s32 camIndex) {
    Camera *cam;
    Vec3f dir;

    cam = &camera_global_Cameras[camIndex];
    if (gPoseCommandRunning != 0) {
        math_vec3Diff(&cam->focus, &cam->eye, &dir);
        math_vec3Unit(&dir, &dir);

        cam->focus.x += dir.x * -2.5f * gPoseCommandScales[camIndex];
        cam->focus.y += dir.y * -2.0f * gPoseCommandScales[camIndex];
        cam->focus.z += dir.z * -2.5f * gPoseCommandScales[camIndex];
        cam->eye.x += dir.x * -2.5f * gPoseCommandScales[camIndex];
        cam->eye.y += dir.y * -2.0f * gPoseCommandScales[camIndex];
        cam->eye.z += dir.z * -2.5f * gPoseCommandScales[camIndex];
    }
}
