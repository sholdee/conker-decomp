#include <ultra64.h>

#include "functions.h"
#include "variables.h"

static u16 *D_800BE2B0[4]; // per camera: a row buffer of gScreenWidth u16 (HitsZBufferHoriDataPtr)

/**
 * Loads the resource's data and allocates and initialises the gScreenLayout + 1 view (Camera) states.
 */
void func_15005290(s32 resource, s32 mode) {
    Camera *view;
    u32 size;
    s32 count;
    u32 i;

    D_800DC020 = (CamPose *)func_1502B5C8(&size, 3, 12, resource, 8);
    D_80089550 = size / 24;
    count = gScreenLayout + 1;
    camera_global_Cameras = allocate_memory(count * sizeof(Camera), 1, 1, 0);
    D_800DC2B0 = allocate_memory(count * sizeof(CamMsgQueue), 1, 0, 0);
    D_800DC2A0[0] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    D_800DC2A0[1] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    for (i = 0; i <= gScreenLayout; i++) {
        view = &camera_global_Cameras[i];
        D_800BE2B0[i] = allocate_memory(gScreenWidth * 2, 1, 2, 0);
        func_150054C4(view, i, resource, mode);
        {
            s32 pad; // matching: -g3 block marker keeps this store after the call

            D_800DBFF4[i] = 0;
        }
    }
    func_1512ABF8();
    func_1512D238();
    D_800894B0 = 0;
    D_800DC010 = allocate_memory(gScreenLayout * sizeof(Camera) + sizeof(Camera), 1, 2, 0);
}

/**
 * Resets view state `index` and attaches it, as camera, to actor movement_obj[index].
 */
void func_150054C4(Camera *view, s32 index, s16 resource, s32 mode) {
    u16 i;
    Actor *actor;
    u8 actorIndex;

    bzero(view, sizeof(Camera));
    view->Snap = 1;
    view->CNumber = index;
    for (i = 0; i < 21; i++) {
        func_151239CC(view, i);
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
    func_15124B18(view);
    view->FCameraIndex = -1;
    view->UDHitsCounter = 1;
    view->TonyHitsClear = 1;
    view->NearPlane = (u32)D_800B0DF0->unk2;
    view->lastPlayerPos.x = view->focus.x;
    view->lastPlayerPos.y = view->focus.y;
    view->lastPlayerPos.z = view->focus.z;
    view->LockedCameraAngle = 999.0f;
    view->SpecialFlags |= 0x80000000;
    func_15124AB4(view);
    func_15125394(view);
    view->SHitsOffLastFrame = 10;
    view->SlackenK = 1.0; // matching: 1.0f reorders the float constant loads; double (or int) does not
    func_1512D66C(view);
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
    func_1512D6F0(view);
    func_15125924(view);
    view->CharTrackingSpeedTarget = 1.0f;
    player_moveToSpawn(view->CharPtr, mode, 0);
    func_15005A60(view, resource);
    view->zoneWaterY = -10000.0f;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->savedTarget = view->target;
    view->savedEye = view->eye;
    view->CameraRadius = 50.0f;
    view->ForceSwimAngle = 999.0f;
    view->HitsZBufferHoriDataPtr = D_800BE2B0[view->CNumber];
    D_80089520 = 0;
    D_800C3671 = 0;
    D_80089524[view->CNumber] = 0;
    D_80089534[view->CNumber] = 0;
    view->unk5FE = 60;
    view->Snap = 1;
}

/**
 * Moves camera `view` to `eye`, aims it at `actor`'s position and resets its derived state.
 */
void func_15005818(Camera *view, Actor *actor, Vec3f *eye) {
    Vec3f *viewEye;

    view->Snap = 1;
    view->eye.x = eye->x;
    view->eye.y = eye->y;
    view->eye.z = eye->z;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->unk27C = view->eye;
    view->prevEye = view->eye;

    viewEye = &view->eye; // matching: kept live across the call in a stack slot
    func_1510E7A4(&view->TrianglePtr, view->floorTri.v, &view->unk354, &view->waterY, &view->Attributes, 0,
                  view->eye.x, view->eye.y, view->eye.z, view->eye.y, 0, 0, -10000.0f, view->eye.y);

    view->unk358 = view->unk354;
    view->unk35C = view->unk354;

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
    func_15124B18(view);
    func_15125330(view);
    func_1512523C(view);
    func_15123070(view);

    view->SpecialFlags |= 4;
    if (level != 0x21 && D_800C35EA[0] != 1) {
        func_1512D560(view, 5, NULL);
        gCamShot.yaw = view->CameraAngle;
        func_1512D560(view, 8, &gCamShot);
        func_1512D560(view, 6, NULL);
    }
}
