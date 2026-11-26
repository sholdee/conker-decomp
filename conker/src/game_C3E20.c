#include <ultra64.h>
#include "structs.h"

/* A scripted actor move: actor `actor` is sent to pose `poseIndex` until `timer` runs out. */
typedef struct {
    /* 0x00 */ u8 state; // 0 = idle, 1 = start pending, 2 = running
    /* 0x01 */ u8 actor; // index into camera_global_Cameras
    /* 0x02 */ u8 poseIndex; // index into the D_800DC020 pose table
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 timer;
    /* 0x06 */ u8 pad6[6];
    /* 0x0C */ s32 flags;
    /* 0x10 */ f32 scale; // copied (truncated) to D_800D2E30 when the command starts
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 savedCameraAngle; // actor->CameraAngle at setup, written to actor->CutbackAngle when done
} C3E20Command; // size 0x24

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
f32 D_800D2DB0;
s32 D_800D2DB4; // a command is running
f32 D_800D2DB8; // actor->unk2C0 saved while a command runs
C3E20Command D_800D2DC0[3];
s32 D_800D2E30[4]; // per actor: the running command's scale

#include "functions.h"
#include "variables.h"

/* Clears all command slots. */
void func_15096970(void) {
    bzero(D_800D2DC0, sizeof(D_800D2DC0));
    D_800D2DB4 = 0;
}

/* Returns TRUE if any of the first `count` command slots is active. */
s32 func_150969A0(s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (D_800D2DC0[i].state != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Advances command slot `slot`: starts the actor's move to its pose, then counts the timer down and
 * restores the actor when it runs out. Returns FALSE once the command has finished.
 */
s32 func_15096A68(s32 slot) {
    switch (D_800D2DC0[slot].state) {
    case 1: {
        u8 actorIndex;
        CamPose *pose;
        Camera *actor;

        actorIndex = D_800D2DC0[slot].actor;
        actor = &camera_global_Cameras[actorIndex];
        pose = D_800DC020;
        // matching: the redundant mask on the u8 index steers IDO's register allocation
        pose = &pose[D_800D2DC0[slot].poseIndex & 0xFF];
        D_800D2DB8 = actor->target.y;
        D_800D2DB4 = 1;
        D_800DBFF4[actorIndex] = 3;
        gCamShot.mode = 1;
        gCamShot.unk4 = 0;
        gCamShot.eye.x = pose->x;
        gCamShot.eye.y = pose->y;
        gCamShot.eye.z = pose->z;
        gCamShot.at.angles.sway = pose->unk14;
        gCamShot.at.angles.pitch = pose->unkC;
        gCamShot.at.angles.yaw = pose->unk10;
        gCamShot.unk8 = 0.0f;
        func_1512D560(actor, 5, NULL);
        func_1512D560(actor, 7, &gCamShot);
        if (!(D_800D2DC0[slot].flags & 1)) {
            actor->CharPtr->unk7 = 0;
            actor->CharPtr->task_attrib |= 0x200;
        }
        D_800D2E30[slot] = D_800D2DC0[slot].scale;
        D_800D2DC0[slot].state = 2;
    }
        break;
    case 2: {
        Camera *actor;

        actor = &camera_global_Cameras[D_800D2DC0[slot].actor];
        D_800D2DC0[slot].timer -= D_800BE9A0;
        if (actor->CharPtr->energy == 0) {
            D_800D2DC0[slot].timer = 0;
        }
        if (D_800D2DC0[slot].timer <= 0) {
            actor->CutbackAngle = D_800D2DC0[slot].savedCameraAngle;
            if (!(D_800D2DC0[slot].flags & 2)) {
                func_1512D560(actor, 6, NULL);
            }
            D_800D2DB4 = 0;
            if (!(D_800D2DC0[slot].flags & 1)) {
                actor->CharPtr->task_attrib &= ~0x200;
                actor->CharPtr->unk7 = 0xFF;
            }
            D_800D2DC0[slot].timer = 0;
            D_800D2DC0[slot].state = 0;
            actor->target.y = D_800D2DB8;
            return FALSE;
        }
    }
        break;
    }
    return TRUE;
}

/* Runs the active command slots until one of them is still busy. */
void func_15096D08(void) {
    s32 i;

    if (D_800C35EA[0] != 1) {
        for (i = 0; i < 3; i++) {
            if (D_800D2DC0[i].state != 0 && func_15096A68(i) != 0) {
                break;
            }
        }
    }
}

/*
 * Applies script command `command` with arguments args[2..] to actor `actorIndex`.
 * Fixed-point arguments are 16.16. Returns 1 for some commands, else 0.
 */
s32 func_15096D78(s32 actorIndex, s32 command, s32 *args) {
    Camera *actor;

    actor = &camera_global_Cameras[actorIndex];
    if (actorIndex > D_80082FA0) {
        return 0;
    }
    switch (command) {
    case 12:
        D_800D2DC0[args[2]].savedCameraAngle = actor->CameraAngle;
        D_800D2DC0[args[2]].actor = actorIndex;
        D_800D2DC0[args[2]].poseIndex = args[3];
        D_800D2DC0[args[2]].timer = args[4];
        D_800D2DC0[args[2]].state = 1;
        if (D_800D3840 >= 6) {
            D_800D2DC0[args[2]].flags = args[5];
        } else {
            D_800D2DC0[args[2]].flags = 0;
        }
        if (D_800D3840 >= 7) {
            D_800D2DC0[args[2]].scale = args[6] * (1.0f / 65536.0f);
        } else {
            D_800D2DC0[args[2]].scale = 1.0f;
        }
        if (!func_150969A0(args[2])) {
            func_15096A68(args[2]);
        }
        break;
    case 4:
        func_15123934(actor, args[2], args[3], args[4], CAMERA_MODE_TASK);
        return 1;
    case 5:
        func_151239CC(actor, CAMERA_MODE_TASK);
        return 1;
    case 6:
        if (args[2] != 0) {
            actor->AvailableModes |= args[3];
        } else {
            actor->AvailableModes &= ~args[3];
        }
        return 1;
    case 8:
        if (actor->mv->death_State == 0) {
            actor->IdealTripodMagnitude = args[2];
            actor->unk34C = args[3];
            actor->height = args[3];
        }
        return 1;
    case 7:
        actor->DistMask = args[2];
        actor->DistIndex = args[3];
        return 1;
    case 9:
        actor->Tightness = args[2];
        return 1;
    case 10: {
        CamPose *pose;

        pose = D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        D_800D2DB4 = 1;
        D_800DBFF4[actorIndex] = 3;
        gCamShot.mode = 1;
        gCamShot.unk4 = 0;
        gCamShot.eye.x = pose->x;
        gCamShot.eye.y = pose->y;
        gCamShot.eye.z = pose->z;
        gCamShot.at.angles.sway = pose->unk14;
        gCamShot.at.angles.pitch = pose->unkC;
        gCamShot.at.angles.yaw = pose->unk10;
        gCamShot.unk8 = 0.0f;
        D_800C365C[0] = 0;
        func_1512D560(actor, 5, NULL);
        func_1512D560(actor, 7, &gCamShot);
        movement_obj[0].task_attrib |= 0x200;
    }
        break;
    case 11:
        func_1512D560(actor, 6, NULL);
        D_800D2DB4 = 0;
        movement_obj[0].task_attrib &= ~0x200;
        break;
    case 14:
        actor->focus.x = args[2];
        actor->focus.y = args[3];
        actor->focus.z = args[4];
        actor->target.x = args[2];
        actor->target.y = args[3];
        actor->target.z = args[4];
        actor->eye.x = args[5];
        actor->eye.y = args[6];
        actor->eye.z = args[7];
        actor->unk358 = actor->eye.y;
        func_1512A390(actor);
        break;
    case 2:
        actor->eye.x = args[2];
        actor->eye.y = args[3];
        actor->eye.z = args[4];
        actor->unk358 = actor->eye.y;
        func_1512A390(actor);
        break;
    case 3:
        actor->focus.x = args[2];
        actor->focus.y = args[3];
        actor->focus.z = args[4];
        break;
    case 15:
        actor->XZOffsetTget = args[2];
        if (D_800D3840 == 4) {
            actor->XZOffset = args[3];
        }
        break;
    case 16:
        actor->YOffsetTget = args[2];
        if (D_800D3840 == 4) {
            actor->YOffset = args[3];
        }
        break;
    case 17:
        if (D_800D3840 == 5 && args[4] != 0) {
            func_1512D748(actor, args[2], args[3]);
            return 1;
        }
        if (D_800DC2C0[actorIndex].stage == 5) {
            func_1512D748(actor, args[2], args[3]);
            return 1;
        }
        return 0;
    case 18:
        func_151254F4(actor, func_15084000(args[2] & 0xFFF));
        break;
    case 19:
        func_15124770(actor, func_15084000(args[2] & 0xFFF));
        break;
    case 20:
        actor->TgetFOVX = args[2] * (1.0f / 65536.0f);
        actor->TgetFOVY = args[3] * (1.0f / 65536.0f);
        if (D_800D3840 == 5 && args[4] != 0) {
            actor->FOVX = actor->TgetFOVX;
            actor->FOVY = actor->TgetFOVY;
        }
        break;
    case 21: {
        CamPose *pose;

        pose = D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        pose->x = args[3];
        pose->y = args[4];
        pose->z = args[5];
    }
        break;
    case 22:
        actor->Snap = 1;
        break;
    case 23:
        actor->CentreBiasTarget = args[2] * (1.0f / 65536.0f);
        break;
    case 24:
        if (D_800D3840 == 5) {
            actor->aim.useOverride = 1;
            actor->aim.overrideYawOffset = args[2];
            actor->aim.overridePitch = args[3];
            actor->aim.overrideDistance = args[4];
        } else {
            actor->aim.useOverride = 0;
        }
        break;
    case 25:
        actor->CutbackAngle = args[2] * (1.0f / 65536.0f);
        D_800D2DB0 = actor->CutbackAngle;
        break;
    case 26:
        func_1512868C(actor);
        break;
    case 27:
        D_80089520 = args[2] - 1;
        break;
    case 13:
        break;
    default:
        return 0;
    }
    return 0;
}


/*
 * Answers script query `query` about actor `actorIndex` (arguments in args[2..]); fixed-point results
 * are 16.16.
 */
s32 func_1509759C(s32 actorIndex, s32 query, s32 *args) {
    Camera *actor;
    s32 side;
    Vec3f facing;
    Vec3f toTarget;
    f32 angle;
    Actor *target;
    CamPose *pose;
    f32 dx;
    f32 dz;

    actor = &camera_global_Cameras[actorIndex];
    if (actorIndex > D_80082FA0) {
        return 0;
    }
    switch (query) {
    case 31:
        return actor->CharRelAngle * 65536.0f;
    case 30:
        return actor->CameraAngle * 65536.0f;
    case 29:
        return D_800D2DC0[args[2]].state;
    case 32:
        // Horizontal distance from the actor to a pose.
        pose = D_800DC020;
        pose = &pose[args[2] & 0xFFF];
        dx = actor->eye.x - pose->x;
        dz = actor->eye.z - pose->z;
        return sqrtf(dx * dx + dz * dz);
    case 33:
        // Which side of the actor's facing direction a path object is on: -1 or 1 (-1 if not found).
        target = actor_findOnPath(args[2] & 0xFFF);
        if (target == NULL) {
            return -1;
        }
        angle = actor->CameraAngleRad + 1.570796371f;
        facing.x = sinf(angle);
        facing.y = 0.0f;
        facing.z = cosf(angle);
        func_15048F90(&actor->eye, (Vec3f *)&target->xpos, &toTarget);
        toTarget.y = 0.0f;
        if (func_150AD900(&facing.x, &toTarget.x) >= 0.0f) {
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

/* While a command runs, moves both actor points (unk2A4, unk2F8) along the unit vector between them, scaled by D_800D2E30. */
void func_15097798(s32 actorIndex) {
    Camera *actor;
    Vec3f dir;

    actor = &camera_global_Cameras[actorIndex];
    if (D_800D2DB4 != 0) {
        func_15048F90(&actor->focus, &actor->eye, &dir);
        func_1504917C(&dir, &dir);

        actor->focus.x += dir.x * -2.5f * D_800D2E30[actorIndex];
        actor->focus.y += dir.y * -2.0f * D_800D2E30[actorIndex];
        actor->focus.z += dir.z * -2.5f * D_800D2E30[actorIndex];
        actor->eye.x += dir.x * -2.5f * D_800D2E30[actorIndex];
        actor->eye.y += dir.y * -2.0f * D_800D2E30[actorIndex];
        actor->eye.z += dir.z * -2.5f * D_800D2E30[actorIndex];
    }
}
