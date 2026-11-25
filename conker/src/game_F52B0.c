#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_1509BE40(s32, s32, s32, ...);
void func_15123070(struct108 *);
void func_151254F4(struct108 *, s32);
extern s32 D_80088800;

/* Updates camera flags and range fields (unk190/unk348/unk34C/unk374) from func_1509BE40 state queries 0x5071/0x5072. */
void func_150C7E00(struct108 *camera)
{
  s32 queryBC;
  s32 globalBB;
  s32 queryBB;
  f32 t;
  f32 blend;
  struct127 *target;
  struct127 *actor;
  queryBC = func_1509BE40(0, func_1509BE40(0, 0x2010, 0xB7) | 0x2000, 0xBC);
  globalBB = func_1509BE40(0, 0x2000, 0xBB);
  queryBB = func_1509BE40(0, func_1509BE40(0, 0x2010, 0xB7) | 0x2000, 0xBB);
  if (func_1509BE40(0, 0x5071, 0x1A) == 0)
  {
    if ((globalBB != -1) && (queryBC == 0))
    {
      if (func_15123934(camera, camera->unk2C, 0, camera->unk134, 3) != 0)
      {
        camera->unk84 |= 0x01000000;
        func_151254F4(camera, D_800CC335 - 1);
        camera->unk674 = 0.0f;
      }
    }
    else
    {
      if (globalBB) // matching: an extra use of globalBB makes IDO load it into v0 and -1 into v1
      {
      }
      if ((globalBB != -1) && (queryBC != 0))
      {
        func_151239CC(camera, 2);
        if (func_15123934(camera, 8, 0, camera->unk134, 3) != 0)
        {
          func_151254F4(camera, D_800CC335 - 1);
          camera->unk84 &= ~4;
          camera->unk674 = 0.0f;
          func_15123070(camera);
        }
        camera->unk5F0 |= 0x200;
        camera->unk348 = 500.0f;
        camera->unk34C = 500.0f;
        camera->unk374 = 800.0f;
      }
      else
      {
        if (func_151239CC(camera, 3) != 0)
        {
          func_151254F4(camera, 0);
          camera->unk84 &= ~0x01000000;
          camera->unk3D4->unk198 = 0;
          camera->unk73C = 0;
          if (camera->unk6C8 == 0)
          {
            camera->unk674 = 0.0f;
          }
        }
        camera->unk5F0 &= ~0x200;
      }
    }
  }
  else
  {
    if (func_1509BE40(0, 0x5072, 0x1A) == 0)
    {
      if (*camera->unk36C & 4)
      {
        camera->unk190 = 20.0f;
      }
      if (func_1509BE40(0, 0x2000, 0x93) == 0)
      {
        camera->unk190 = 0.0f;
        return;
      }
      func_1509BFB0(1, 0x9000, 0x17, func_1509BE40(1, func_1509BE40(0, 0x2011, 0xB7) | 0x2000, 0x9C, 0x2000));
      t = ((f32)func_1509BE40(1, func_1509BE40(0, 0x2011, 0xB7) | 0x2000, 0x9A, 0x2000) - 300.0f) / 900.0f;
      blend = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;
      if (queryBC == 0)
      {
        camera->unk348 = camera->unk34C = 200.0f;
        camera->unk190 = 100.0f;
      }
      else
      {
        camera->unk190 = (103.0f * blend) + 287.0f;
        camera->unk374 = (224.0f * blend) + 750.0f;
        camera->unk348 = camera->unk34C = 231.0f * blend;
      }
      func_1509BFB0(2, 0x9000, 6, 1, 0x20000);
      func_1509BFB0(2, 0x9000, 6, 0, 4);
      if (queryBB != -1)
      {
        camera->unk670 = 0.0f;
      }
      D_80088800 = 0;
      if (camera->unk5F0 & 4)
      {
        target = func_15083E90(0x10);
        if (target != NULL)
        {
          actor = camera->unk3D0;
          if (actor->unk65 != 0)
          {
            actor->x_position = target->x_position;
            camera->unk3D0->y_position = target->y_position;
            camera->unk3D0->z_position = target->z_position;
          }
        }
      }
      if (func_151239CC(camera, 3) != 0)
      {
        camera->unk5F0 &= ~0x200;
        func_151254F4(camera, 0);
        camera->unk84 &= ~0x01000000;
        camera->unk3D4->unk198 = 0;
        camera->unk73C = 0;
        if (camera->unk6C8 == 0)
        {
          s32 pad; // matching: block-scope decl keeps as1 from moving this store into the branch delay slot
          camera->unk674 = 0.0f;
        }
      }
    }
    else if ((func_151239CC(camera, 3) != 0) || (D_80088800 == 0))
    {
      camera->unk84 |= 4;
      D_80088800 = 1;
      func_151254F4(camera, 0);
      camera->unk3D4->unk198 = 0;
      camera->unk73C = 0;
      camera->unk1B4 = 3;
      func_15124B18(camera);
      camera->unk190 = 0.0f;
      if (camera->unk6C8 == 0)
      {
        camera->unk674 = 0.0f;
      }
    }
    if (*camera->unk36C & 4)
    {
      camera->unk190 = 0.0f;
    }
  }
}
