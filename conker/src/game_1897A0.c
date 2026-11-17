#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
} Struct1515C2F0Arg0;

typedef struct {
    u8 pad0[0x98];
    void *unk98;
} Struct1515C2F0Ret;

extern Struct1515C2F0Ret *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void *func_1515C2F0(Struct1515C2F0Arg0 *arg0, s32 arg1, void *arg2, s32 arg3, u8 arg4, s32 arg5) {
    Struct1515C2F0Ret *temp_v0;
    Struct1515C2F0Ret *sp3C;

    arg0->unk10 = 0xA;
    temp_v0 = func_15147A80(arg0, arg1 + 0x40, 0x10, 8, 8, 8, 0, 0, arg3, arg4, arg5);
    if (temp_v0 == 0) {
        return NULL;
    }
    sp3C = temp_v0;
    memcpy(temp_v0->unk98, arg2, 0x3C);
    return sp3C;
}
// void *func_1515C2F0(void *arg0, void *arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
//     void *sp3C;
//     void *temp_v0;
//
//     arg0->unk10 = 0xA;
//     temp_v0 = func_15147A80(&arg1->unk40, 0x10, 8, 8, 8, 0, 0, arg3, (?32) arg4, arg5);
//     if (temp_v0 == 0) {
//         return NULL;
//     }
//     sp3C = temp_v0;
//     memcpy(temp_v0->unk98, arg2, 0x3C); // memcpy
//     return sp3C;
// }

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec1515C388;

typedef struct {
    Vec1515C388 pos;
    f32 w;
} Ent1515C388;

typedef struct {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0xC - 0x8];
    f32 unkC;
    f32 unk10;
    u8 pad14[0x38 - 0x14];
    s8 unk38;
} Set1515C388;

typedef struct {
    u8 pad0[0x25];
    u8 unk25;
    u8 pad26[0x2C - 0x26];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x54 - 0x2F];
    Vec1515C388 unk54;
    u8 pad60[0x94 - 0x60];
    Ent1515C388 *unk94;
    Set1515C388 *unk98;
} Obj1515C388;

typedef s32 (*Game1897A0Callback)(void *, void *);
extern Game1897A0Callback D_8008B080[];

s32 func_1515C388(Obj1515C388 *arg0)
{
  Set1515C388 *settings;
  s32 callbackFailed;
  Ent1515C388 *ents;
  Vec1515C388 start;
  s32 i;
  Game1897A0Callback cb;
  settings = arg0->unk98;
  callbackFailed = 0;
  ents = arg0->unk94;
  if (arg0->unk2C)
  {
    start = ents[arg0->unk2D].pos;
    i = arg0->unk2D;
    while (i != arg0->unk2E)
    {
      ents[i].pos.x += settings->unk4 * D_800BE9A4;
      ents[i].pos.y += (ents[i].w * D_800BE9A4) + (((settings->unk10 * D_800BE9A4) * D_800BE9A4) * 0.5f);
      ents[i].pos.z += settings->unkC * D_800BE9A4;
      ents[i].w += settings->unk10 * D_800BE9A4;
      i++;
      if (i == arg0->unk25)
      {
        i = 0;
      }
    }

  }
  if (settings->unk38 != (-1))
  {
    {
    callbackFailed = (D_8008B080[settings->unk38](arg0, settings) == 0) & 0xFF;
    }
  }
  if (arg0->unk2C > 0)
  {
    arg0->unk54 = ents[arg0->unk2D].pos;
  }
  else
  {
    arg0->unk54.x = 0.0f;
    arg0->unk54.y = 0.0f;
    arg0->unk54.z = 0.0f;
  }
  if (callbackFailed != 0)
  {
    return 0;
  }
  return 1;
}
typedef s32 (*Game1897A0ActionCallback)(void *, void *);
extern Game1897A0ActionCallback D_8008B084[];

s32 func_1515C534(void *arg0) {
    struct Actor {
        u8 pad00[0x1C];
        s16 time;
        u8 pad1E[0x7A];
        void *settings;
    };
    struct Settings {
        f32 value;
        u8 pad04[0x10];
        u8 flags;
        u8 pad15[5];
        u8 intensity;
        u8 pad1B[0x11];
        s16 start;
        s16 slope;
        s16 end;
        u8 paused;
        u8 pad33;
        f32 attenuation;
        u8 pad38;
        s8 callbackIndex;
    } *settings;
    struct Actor *actor = arg0;
    s32 callbackFailed;
    u32 count;

    settings = actor->settings;
    callbackFailed = 0;
    if (settings->callbackIndex != -1) {
        callbackFailed = (D_8008B084[settings->callbackIndex](arg0, settings) == 0) & 0xFF;
    }
    if (callbackFailed == 0) {
        if (settings->flags & 4) {
            s16 time = actor->time;
            s32 newValue;

            if (time < settings->start) {
                newValue = time * settings->slope;
                if (newValue < settings->intensity) {
                    settings->intensity = newValue;
                }
            }
        }
        if ((settings->flags & 8) && actor->time < settings->end && settings->paused == 0) {
            count = D_800BE9E4;
            if (count != 0) {
                do {
                    settings->value *= settings->attenuation;
                    count--;
                } while (count != 0);
            }
        }
    }
    if (callbackFailed != 0) {
        return 0;
    }
    return 1;
}
typedef struct {
    f32 x, y, z;
} TrailVec;

typedef struct {
    TrailVec pos;
    s32 pad;
} TrailPoint;

typedef struct {
    f32 width;
    u8 pad4[0x10];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 pad22[2];
    s32 unk24;
    s32 unk28;
} TrailParams;

typedef struct {
    u8 pad0[0x25];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 unk84[0x10];
    TrailPoint *unk94;
    TrailParams *unk98;
} Trail;

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct800A4AC8;

extern u8 D_80090E78[];
extern Struct800A4AC8 D_800A4AC8[];
extern s32 D_800D2C9C;

extern void func_151D5D60(void *, s16, s32, Vtx **, s32);
extern void func_151441A4(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_151442FC(s16 *, s16 *, s16 *, s16 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
extern Gfx *func_1513F4E4(Gfx *, s32, u8 *);
extern Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
extern Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
extern Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
extern Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_1515C6F4(Trail *arg0, Gfx *gfx, s16 arg2) {
    Vtx *vtx;
    s32 i;
    u8 flag;
    TrailVec b;
    TrailVec a;
    TrailParams *params;
    TrailPoint *pts;
    f32 *cam;
    s32 j;
    f32 scale2;
    s32 sStep;
    s32 aStep;
    s32 s;
    s32 al;
    s32 alpha;
    s32 mode;
    f32 dx;
    f32 dy;
    f32 dz;
    s16 spE2;
    s16 spE0;
    s16 spDE;
    s16 spDC;
    s16 spDA;
    s16 spD8;
    s16 spD6;
    s16 spD4;
    f32 ex, ey, ez;
    f32 nx, ny, nz;
    f32 mag;
    f32 scale;

    if (arg0->unk2C >= 2) {
        func_151D5D60(arg0->unk84, arg2, (arg0->unk25 * 2 + 10) * sizeof(Vtx), &vtx, 0);
        if (vtx != NULL) {
            params = arg0->unk98;
            flag = 1;
            cam = &D_800DBFF0[arg2].unk2F8;
            pts = arg0->unk94;
            func_151441A4(&spE2, &spE0, &spDE, &spDC, params->unk1B, params->unk1C, params->unk1D, params->unk1E,
                          params->unk17, params->unk18, params->unk19, params->unk1A, params->unk1F, params->unk15);
            func_151442FC(&spDA, &spD8, &spD6, &spD4, params->unk1B, params->unk1C, params->unk1D, params->unk1E,
                          params->unk17, params->unk18, params->unk19, params->unk1A, params->unk1F, params->unk16);
            gfx = func_15142E24(gfx, (s32)D_80090E78, 0, 0, 0, 0, 0x42, 0, 0, &flag, 0x3E);
            gfx = func_1513F4E4(gfx, params->unk21, &flag);
            gfx = func_15142B7C(gfx, params->unk24, params->unk28);
            gfx = func_15142CF0(gfx, 0, 0, spE2, spE0, spDE, spDC, &flag);
            gfx = func_15142C10(gfx, spDA, spD8, spD6, spD4, &flag);
            if (params->unk14 & 1) {
                mode = 0x100000;
            } else {
                mode = 0;
            }
            gfx = func_15142FBC(gfx, mode | 0x80000 | D_800D2C9C | 0x2000 | 0xC00 | 0x80 | 0x20,
                                D_800A4AC8[params->unk20].unk0 | D_800A4AC8[params->unk20].unk4, &flag);
            i = arg0->unk2E - 1;
            i = (i < 0) ? arg0->unk25 - 1 : i;
            j = i - 1;
            j = (j < 0) ? arg0->unk25 - 1 : j;
            a = pts[i].pos;
            b = pts[j].pos;
            sStep = 0x800 / (arg0->unk2C - 1);
            aStep = 0xFF / (arg0->unk2C - 1);

            ex = a.x - b.x;
            ey = a.y - b.y;
            ez = a.z - b.z;
            dx = a.x - cam[0];
            dy = a.y - cam[1];
            dz = a.z - cam[2];
            nx = ey * dz - dy * ez;
            ny = ez * dx - dz * ex;
            nz = ex * dy - dx * ey;
            mag = nx * nx + ny * ny + nz * nz;
            if (mag == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale2 = params->width / sqrtf(mag);
                nx = nx * scale2;
                ny = ny * scale2;
                nz = nz * scale2;
            }
            vtx->v.ob[0] = a.x + nx;
            vtx->v.ob[1] = a.y + ny;
            vtx->v.ob[2] = a.z + nz;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = 0x3FF;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = a.x - nx;
            vtx->v.ob[1] = a.y - ny;
            vtx->v.ob[2] = a.z - nz;
            vtx->v.tc[0] = 0;
            vtx->v.tc[1] = 0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = 0;
            vtx->v.flag = 0;
            vtx++;
            s = sStep;
            al = aStep;
            do {
                i = j;
                ex = a.x - b.x;
                ey = a.y - b.y;
                ez = a.z - b.z;
                dx = b.x - cam[0];
                dy = b.y - cam[1];
                dz = b.z - cam[2];
                nx = ey * dz - dy * ez;
                ny = ez * dx - dz * ex;
                nz = ex * dy - dx * ey;
                mag = nx * nx + ny * ny + nz * nz;
                if (mag == 0.0f) {
                    nx = 0.0f;
                    ny = 0.0f;
                    nz = 0.0f;
                } else {
                    scale = params->width / sqrtf(mag);
                    nx = nx * scale;
                    ny = ny * scale;
                    nz = nz * scale;
                }
                alpha = (params->unk1A * al) >> 8;
                al += aStep;
                vtx->v.ob[0] = b.x + nx;
                vtx->v.ob[1] = b.y + ny;
                vtx->v.ob[2] = b.z + nz;
                vtx->v.tc[0] = s;
                vtx->v.tc[1] = 0x3FF;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = b.x - nx;
                vtx->v.ob[1] = b.y - ny;
                vtx->v.ob[2] = b.z - nz;
                vtx->v.tc[0] = s;
                s += sStep;
                vtx->v.tc[1] = 0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                j--;
                if (j < 0) {
                    j = arg0->unk25 - 1;
                }
                a = b;
                b = pts[j].pos;
            } while (i != arg0->unk2D);
        }
    }
    return gfx;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec1515CF9C;

typedef struct {
    Vec1515CF9C pos;
    f32 w;
} Ent1515CF9C;

typedef struct {
    u8 pad0[0x10];
    Vec1515CF9C unk10;
    u8 pad1C[0x25 - 0x1C];
    u8 unk25;
    u8 pad26[0x2C - 0x26];
    s8 unk2C;
    u8 pad2D;
    s8 unk2E;
    u8 pad2F[0x94 - 0x2F];
    Ent1515CF9C *unk94;
} Obj1515CF9C;

typedef struct {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x39 - 0xC];
    s8 unk39;
} Arg1515CF9C;

s32 func_1515CF9C(Obj1515CF9C *arg0, Arg1515CF9C *arg1) {
    if (arg0->unk2C < arg0->unk25 - 1) {
        Ent1515CF9C *ents;

        ents = arg0->unk94;
        arg0->unk2C++;
        ents[arg0->unk2E].pos = arg0->unk10;
        ents[arg0->unk2E].w = arg1->unk8;
        arg0->unk2E++;
        if (arg0->unk2E == arg0->unk25) {
            arg0->unk2E = 0;
        }
    } else {
        arg1->unk39 = -1;
    }
    return 1;
}

typedef struct {
    char pad0[0x25];
    u8 unk25;
    char pad26[0x6];
    s8 unk2C;
    char pad2D[0x1];
    s8 unk2E;
} Struct1515D030;

s32 func_1515D030(Struct1515D030 *arg0, s32 arg1) {
    s8 temp_v0;
    s32 result;

    temp_v0 = arg0->unk2C;
    result = 1;
    if ((s32) temp_v0 >= 3) {
        arg0->unk2C = (s8) (temp_v0 - 1);
        arg0->unk2E = (s8) (arg0->unk2E - 1);
        if ((s32) arg0->unk2E < 0) {
            arg0->unk2E = (s8) (arg0->unk25 - 1);
            result = 1;
        }
    } else {
        result = 0;
    }
    return result;
}

// s32 func_1515D030(void *arg0, ? arg1) {
//     s8 temp_v0;
//     s32 result;
//
//     temp_v0 = arg0->unk2C;
//     if ((s32) temp_v0 >= 3) {
//         arg0->unk2C = (s8) (temp_v0 - 1);
//         arg0->unk2E = (s8) (arg0->unk2E - 1);
//         result = 1;
//         if ((s32) arg0->unk2E < 0) {
//             arg0->unk2E = (s8) (arg0->unk25 - 1);
//             result = 1;
//         }
//     } else {
//         result = 0;
//     }
//     return result;
// }

struct260 *func_151491F4(s16 arg0, s8 arg1, s8 arg2, u8 arg3, u8 arg4, s32 arg5, u8 arg6, s32 arg7);
typedef struct 
{
  u8 pad0[0x18];
  s32 unk18;
} Struct1515D088Arg0;
s32 func_1515D088(Struct1515D088Arg0 *arg0)
{
  s32 temp_v0_2;
  struct 
  {
    void *unk0;
    f32 unk4;
    s8 unk8;
  } sp30;
  temp_v0_2 = arg0->unk18;
  sp30.unk8 = (s8) (temp_v0_2 & 0xFFFF);
  if (((temp_v0_2 & 0xFF) < 0) || ((temp_v0_2 & 0xFF) >= 2))
  {
    return 0;
  }
  sp30.unk0 = arg0;
  sp30.unk4 = 0.0f;
  temp_v0_2 = (s32) func_151491F4(0x12C, -1, 0x11, 0, 0xD, 0xC, 0xFF, 1);
  if (temp_v0_2 != 0)
  {
    memcpy((void *) (temp_v0_2 + 0x28), &sp30, 0xC);
  }
  return temp_v0_2;
}

// s32 func_1515D088(void *arg0) {
//     s32 sp3C;
//     s8 sp38;
//     f32 sp34;
//     void *sp30;
//     s32 temp_t7;
//     s32 temp_v0;
//     void * temp_v0_2;
//     s32 result;
//
//     temp_t7 = arg0->unk18;
//     temp_v0 = temp_t7 & 0xFF;
//     sp38 = (s8) temp_t7;
//     if ((temp_v0 < 0) || (temp_v0 >= 2)) {
//         return 0;
//     }
//     sp30 = arg0;
//     sp34 = 0.0f;
//     temp_v0_2 = func_151491F4(0x12C, -1, 0x11, 0, 0xD, 0xC, 0xFF, 1);
//     result = &temp_v0_2;
//     if (temp_v0_2 != 0) {
//         sp3C = temp_v0_2;
//         memcpy(temp_v0_2->unk28, &sp30, 0xC); // memcpy
//         result = sp3C;
//     }
//     return result;
// }

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Struct1515D130Src;

typedef struct {
    Struct1515D130Src *unk0;
    f32 unk4;
} Struct1515D130Emitter;

typedef struct {
    u8 pad0[0x1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Struct1515D130Emitter unk28;
    u8 unk30;
} Struct1515D130Arg0;

typedef struct {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    s32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[2];
    f32 unk3C;
} Struct1515D130Preset;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E;
    u8 unk4F;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    s16 unk68;
    u8 unk6A;
    u8 pad6B[1];
    f32 unk6C;
    s8 unk70;
    s8 unk71;
} Struct1515D130Spawn;

extern Struct1515D130Preset D_800A64A0[];
extern void func_15143794(s32, s32, f32, f32 *);
extern void func_15152B38(Struct1515D130Spawn *, u8, s32);

void func_1515D130(Struct1515D130Arg0 *arg0) {
    Struct1515D130Preset *preset;
    Struct1515D130Emitter *emitter;
    Struct1515D130Spawn spawn;
    s32 rand0;
    s32 rand1;

    preset = &D_800A64A0[arg0->unk30];
    emitter = &arg0->unk28;
    emitter->unk4 += preset->unk0 + (func_150ADA68() * preset->unk4);

    while (emitter->unk4 > 1.0f) {
        spawn.unk0 = preset->unk8;
        spawn.unk4 = preset->unkC;
        spawn.unk14 = preset->unk10;
        spawn.unk18 = preset->unk14;
        spawn.unk1C = preset->unk18;
        spawn.unk20 = preset->unk1C;
        spawn.unk24 = preset->unk20;
        spawn.unk28 = preset->unk24;
        spawn.unk2C = 0;
        spawn.unk2E = 0xFF;
        spawn.unk30 = -0x3F;
        spawn.unk32 = 0x50;
        spawn.unk34 = preset->unk28;
        spawn.unk38 = preset->unk2C;
        spawn.unk3C = preset->unk30;
        spawn.unk3E = preset->unk32;
        spawn.unk40 = 1;
        spawn.unk42 = 0xC;
        spawn.unk43 = 2;
        spawn.unk44 = 3;
        spawn.unk45 = 0xB4;
        spawn.unk46 = 0;
        spawn.unk47 = 0;
        spawn.unk48 = 0x9B;
        spawn.unk49 = 0x32;
        spawn.unk4A = 0x64;
        spawn.unk4B = 0;
        spawn.unk4C = 0x64;
        spawn.unk4D = 0xFF;
        spawn.unk4E = 0xFF;
        spawn.unk4F = 0xFF;
        spawn.unk50 = 0xFF;
        spawn.unk51 = 0;
        spawn.unk52 = 0;
        spawn.unk53 = 0;
        spawn.unk54 = 0;
        spawn.unk55 = 0xFF;
        spawn.unk56 = 0;
        spawn.unk57 = 1;
        spawn.unk58 = 0x24;
        spawn.unk5C = 0x200005;
        spawn.unk60 = 0x60600;
        spawn.unk64 = preset->unk34;
        spawn.unk66 = preset->unk36;
        spawn.unk68 = preset->unk38;
        spawn.unk6A = 0;
        spawn.unk6C = preset->unk3C;
        spawn.unk70 = -1;
        spawn.unk71 = 0;
        rand0 = func_150ADA20();
        rand1 = func_150ADA20();
        func_15143794((s16) (rand0 & 0xFF), (s16) (0x40 - (rand1 & 0x7F)), emitter->unk0->unk6, &spawn.unk8);
        spawn.unk8 += emitter->unk0->unk0;
        spawn.unkC += emitter->unk0->unk2;
        spawn.unk10 += emitter->unk0->unk4;
        func_15152B38(&spawn, arg0->unkC, arg0->unk1);
        emitter->unk4 -= 1.0f;
    }
}
