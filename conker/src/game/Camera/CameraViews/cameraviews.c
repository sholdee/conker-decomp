#include <ultra64.h>

#include "functions.h"
#include "variables.h"

static u16 *gCameraZRowBuffers[4]; // per camera: a row buffer of gScreenWidth u16 (HitsZBufferHoriDataPtr)

/**
 * Loads the resource's data and allocates and initialises the gScreenLayout + 1 view (Camera) states.
 */
void camera_initViews(s32 resource, s32 mode) {
    Camera *view;
    u32 size;
    s32 count;
    u32 i;

    gCameraPoses = (CamPose *)rom_loadPath(&size, 3, 12, resource, 8);
    gNumCameraPoses = size / 24;
    count = gScreenLayout + 1;
    camera_global_Cameras = allocate_memory(count * sizeof(Camera), 1, 1, 0);
    gCameraMsgQueues = allocate_memory(count * sizeof(CamMsgQueue), 1, 0, 0);
    gViewMtxBuffers[0] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    gViewMtxBuffers[1] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    for (i = 0; i <= gScreenLayout; i++) {
        view = &camera_global_Cameras[i];
        gCameraZRowBuffers[i] = allocate_memory(gScreenWidth * 2, 1, 2, 0);
        camera_resetView(view, i, resource, mode);
        {
            s32 pad; // matching: -g3 block marker keeps this store after the call

            D_800DBFF4[i] = 0;
        }
    }
    camera_resetZScans();
    camera_loadShakeTables();
    gCameraFrames = 0;
    gSavedCameras = allocate_memory(gScreenLayout * sizeof(Camera) + sizeof(Camera), 1, 2, 0);
}

/**
 * Resets view state `index` and attaches it, as camera, to actor movement_obj[index].
 */
void camera_resetView(Camera *view, s32 index, s16 resource, s32 mode) {
    u16 i;
    Actor *actor;
    u8 actorIndex;

    bzero(view, sizeof(Camera));
    view->Snap = 1;
    view->CNumber = index;
    for (i = 0; i < 21; i++) {
        camera_popMode(view, i);
        view->savedDistIndex[i] = 999;
    }
    actorIndex = view->CNumber;
    actor = &movement_obj[actorIndex];
    view->ClampYTop = ((f32)gScreenHeight * 60.0f) / 216.0f;
    view->ClampYBottom = ((f32)gScreenHeight * 40.0f) / 216.0f;
    view->Mode = 1;
    view->Motion = 0;
    view->CharPtr = actor;
    view->DistSpeedXZ = 1.0f;
    view->DistSpeedY = 1.0f;
    view->CharIndex = actorIndex;
    actor->CameraPtr = view;
    view->Tightness = 1;
    view->AvailableModes = 14;
    view->SpecialMove = 0;
    view->DistMask = 13;
    view->DistIndex = 3;
    view->LockedCamAngleK = 1.0f;
    view->APitchAngle = 0.0f;
    camera_loadDistancePreset(view);
    view->FCameraIndex = -1;
    view->UDHitsCounter = 1;
    view->TonyHitsClear = 1;
    view->NearPlane = (u32)D_800B0DF0->unk2;
    view->lastPlayerPos.x = view->focus.x;
    view->lastPlayerPos.y = view->focus.y;
    view->lastPlayerPos.z = view->focus.z;
    view->LockedCameraAngle = 999.0f;
    view->SpecialFlags |= 0x80000000;
    camera_updateViewDir(view);
    camera_updateModeIndex(view);
    view->SHitsOffLastFrame = 10;
    view->SlackenK = 1.0; // matching: 1.0f reorders the float constant loads; double (or int) does not
    camera_clearMessages(view);
    actorIndex = view->CNumber;
    view->LastFrame = -1;
    view->CharIndex = actorIndex;
    view->PadNumber = actorIndex;
    actor = &movement_obj[actorIndex];
    view->CharPtr = actor;
    view->mv = actor->mv;
    func_151298C0(view, resource, actor);
    view->DelayOneFrame = 2;
    view->SHitsPIPOutXZ = -1.0f;
    view->SHitsPIPOutY = -1.0f;
    view->CentreBias = 999.0f;
    view->FZoneAdjustAngleRad = view->TrueCameraAngleRad;
    view->mv->unk18C = 999.0f;
    camera_stopEnvelope(view);
    camera_updateDeepWater(view);
    view->CharTrackingSpeedTarget = 1.0f;
    player_moveToSpawn(view->CharPtr, mode, 0);
    camera_runLevelSetup(view, resource);
    view->zoneWaterY = -10000.0f;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->savedTarget = view->target;
    view->savedEye = view->eye;
    view->CameraRadius = 50.0f;
    view->ForceSwimAngle = 999.0f;
    view->HitsZBufferHoriDataPtr = gCameraZRowBuffers[view->CNumber];
    D_80089520 = 0;
    D_800C3671 = 0;
    gSavedSniperStates[view->CNumber] = 0;
    gSavedAimStates[view->CNumber] = 0;
    view->unk5FE = 60;
    view->Snap = 1;
}

/**
 * Moves camera `view` to `eye`, aims it at `actor`'s position and resets its derived state.
 */
void camera_placeAt(Camera *view, Actor *actor, Vec3f *eye) {
    Vec3f *viewEye;

    view->Snap = 1;
    view->eye.x = eye->x;
    view->eye.y = eye->y;
    view->eye.z = eye->z;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->unk27C = view->eye;
    view->prevEye = view->eye;

    viewEye = &view->eye; // matching: kept live across the call in a stack slot
    collision_findFloorInRange(&view->TrianglePtr, view->floorTri.v, &view->unk354, &view->waterY, &view->Attributes, 0,
                  view->eye.x, view->eye.y, view->eye.z, view->eye.y, 0, 0, -10000.0f, view->eye.y);

    view->unk358 = view->unk354;
    view->floorY = view->unk354;

    view->focus.x = actor->xpos;
    view->focus.y = actor->ypos;
    view->focus.z = actor->zpos;
    view->target = view->focus;
    view->lastPlayerPos = view->focus;

    view->IdealTripodMagnitude = sqrtf((view->focus.x - view->eye.x) * (view->focus.x - view->eye.x) +
                         (view->focus.z - view->eye.z) * (view->focus.z - view->eye.z));
    view->unk34C = view->eye.y - view->unk354;
    view->height = view->eye.y - view->unk354;

    view->savedTarget = view->target;
    view->savedEye = *viewEye;
    camera_loadDistancePreset(view);
    camera_updateYaw(view);
    camera_updatePitch(view);
    camera_updatePlayerAngle(view);

    view->SpecialFlags |= 4;
    if (level != 0x21 && gCutsceneStates[0] != 1) {
        camera_queueMessage(view, 5, NULL);
        gCamShot.yaw = view->CameraAngle;
        camera_queueMessage(view, 8, &gCamShot);
        camera_queueMessage(view, 6, NULL);
    }
}
