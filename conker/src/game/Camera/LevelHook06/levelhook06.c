#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/script.h"


/* This file's .data. */
static s32 D_80088800 = 0;

/* Updates camera flags and range fields (liftTarget/height/unk34C/targetDistance) from script_query state queries 0x5071/0x5072. */
void camera_levelHook06(Camera *camera)
{
  s32 queryBC;
  s32 globalBB;
  s32 queryBB;
  f32 t;
  f32 blend;
  Actor *target;
  Actor *actor;
  queryBC = script_query(0, script_query(0, 0x2010, 0xB7) | 0x2000, 0xBC);
  globalBB = script_query(0, 0x2000, 0xBB);
  queryBB = script_query(0, script_query(0, 0x2010, 0xB7) | 0x2000, 0xBB);
  if (script_query(0, 0x5071, 0x1A) == 0)
  {
    if ((globalBB != -1) && (queryBC == 0))
    {
      if (camera_pushMode(camera, camera->Mode, 0, camera->Tightness, CAMERA_MODE_TRANSPORT) != 0)
      {
        camera->AvailableModes |= 0x01000000;
        camera_attachToObject(camera, movement_obj[0].attach_char - 1);
        camera->CharTrackingSpeedTarget = 0.0f;
      }
    }
    else
    {
      if (globalBB) // matching: an extra use of globalBB makes IDO load it into v0 and -1 into v1
      {
      }
      if ((globalBB != -1) && (queryBC != 0))
      {
        camera_popMode(camera, CAMERA_MODE_ZONES);
        if (camera_pushMode(camera, 8, 0, camera->Tightness, CAMERA_MODE_TRANSPORT) != 0)
        {
          camera_attachToObject(camera, movement_obj[0].attach_char - 1);
          camera->AvailableModes &= ~4;
          camera->CharTrackingSpeedTarget = 0.0f;
          camera_updatePlayerAngle(camera);
        }
        camera->SpecialFlags |= 0x200;
        camera->height = 500.0f;
        camera->unk34C = 500.0f;
        camera->IdealTripodMagnitude = 800.0f;
      }
      else
      {
        if (camera_popMode(camera, CAMERA_MODE_TRANSPORT) != 0)
        {
          camera_attachToObject(camera, 0);
          camera->AvailableModes &= ~0x01000000;
          camera->mv->AimState = 0;
          camera->DoingAim = 0;
          if (camera->cmdRunning == 0)
          {
            camera->CharTrackingSpeedTarget = 0.0f;
          }
        }
        camera->SpecialFlags &= ~0x200;
      }
    }
  }
  else
  {
    if (script_query(0, 0x5072, 0x1A) == 0)
    {
      if (camera->PadPtr->button & 4)
      {
        camera->YOffsetTget = 20.0f;
      }
      if (script_query(0, 0x2000, 0x93) == 0)
      {
        camera->YOffsetTget = 0.0f;
        return;
      }
      script_command(1, 0x9000, 0x17, script_query(1, script_query(0, 0x2011, 0xB7) | 0x2000, 0x9C, 0x2000));
      t = (script_query(1, script_query(0, 0x2011, 0xB7) | 0x2000, 0x9A, 0x2000) - 300.0f) / 900.0f;
      blend = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;
      if (queryBC == 0)
      {
        camera->height = camera->unk34C = 200.0f;
        camera->YOffsetTget = 100.0f;
      }
      else
      {
        camera->YOffsetTget = (103.0f * blend) + 287.0f;
        camera->IdealTripodMagnitude = (224.0f * blend) + 750.0f;
        camera->height = camera->unk34C = 231.0f * blend;
      }
      script_command(2, 0x9000, 6, 1, 0x20000);
      script_command(2, 0x9000, 6, 0, 4);
      if (queryBB != -1)
      {
        camera->CharTrackingSpeed = 0.0f;
      }
      D_80088800 = 0;
      if (camera->SpecialFlags & 4)
      {
        target = collision_findObjectById(0x10);
        if (target != NULL)
        {
          actor = camera->CharPtr;
          if (actor->attach_char != 0)
          {
            actor->xpos = target->xpos;
            camera->CharPtr->ypos = target->ypos;
            camera->CharPtr->zpos = target->zpos;
          }
        }
      }
      if (camera_popMode(camera, CAMERA_MODE_TRANSPORT) != 0)
      {
        camera->SpecialFlags &= ~0x200;
        camera_attachToObject(camera, 0);
        camera->AvailableModes &= ~0x01000000;
        camera->mv->AimState = 0;
        camera->DoingAim = 0;
        if (camera->cmdRunning == 0)
        {
          s32 pad; // matching: block-scope decl keeps as1 from moving this store into the branch delay slot
          camera->CharTrackingSpeedTarget = 0.0f;
        }
      }
    }
    else if ((camera_popMode(camera, CAMERA_MODE_TRANSPORT) != 0) || (D_80088800 == 0))
    {
      camera->AvailableModes |= 4;
      D_80088800 = 1;
      camera_attachToObject(camera, 0);
      camera->mv->AimState = 0;
      camera->DoingAim = 0;
      camera->DistIndex = 3;
      camera_loadDistancePreset(camera);
      camera->YOffsetTget = 0.0f;
      if (camera->cmdRunning == 0)
      {
        camera->CharTrackingSpeedTarget = 0.0f;
      }
    }
    if (camera->PadPtr->button & 4)
    {
      camera->YOffsetTget = 0.0f;
    }
  }
}
