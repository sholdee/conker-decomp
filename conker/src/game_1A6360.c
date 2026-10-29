#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#define func_10010F30 func_10010F30_void
#include "functions.h"
#undef func_15048A40
#undef func_10010F30
#define D_800DD444 D_800DD444_hdr
#include "variables.h"
#undef D_800DD444
extern s16 D_800DD444;


extern u8 D_800DD432;
extern u8 D_800DD433;
extern u8 D_800DD434;
extern u16 D_800DD436;
extern s32 D_800DD440;
extern u8 D_800DD446;
extern u16 D_800DD430;
extern f32 D_800A7208;
extern f32 sqrtf(f32);
extern void func_10004074(void *arg0);
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 *allocate_memory(s32, s32, s32, s32);
void func_151794C8(void);
void func_15179600(s32 arg0, s32 arg1);
void func_15179AB8(void);
void func_15179B14(s32 arg0);
void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800();
s32 func_1510F8D8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} struct800DD438;

extern struct800DD438 D_800DD438;

typedef struct {
    u8 pad0[0x9C];
    s16 unk9C;
    s16 unk9E;
    s16 unkA0;
    u8 padA2[0x12];
    u8 unkB4;
} struct15179CB0;

void func_15178EB0(void) {
    D_800DD434 = 0;
    D_800DD446 = 0;
    D_800DD444 = 0x258;
    D_800DD432 = 0xF0;
    D_800DD433 = 0x14;
    D_800DD436 = 0;
    D_800DD440 = 0;
}

void func_15178EFC(s32 arg0) {
    s32 temp_v0;
    s32 i;
    struct102 *temp_a0;

    if (arg0 == 2) {
        D_800DD434 = 0;
        D_800DD446 = 0;
        if (D_800DD440 != 0) {
            i = 0;
            do {
                temp_a0 = *(struct102 **)(D_800DD440 + i);
                if (temp_a0 != 0) {
                    func_1516972C(temp_a0);
                    *(s32 *)(D_800DD440 + i) = 0;
                    temp_v0 = D_800DD440;
                }
                i += 4;
            } while (i != 0x4B0);
            func_10004074((void *) D_800DD440);
            D_800DD440 = 0;
        }
        func_100111C8(D_800DD430);
        D_800DD444 = 0;
        D_800DD436 = 0;
    } else if (arg0 == 3) {
        if ((s8) D_800DD446 == 0) {
            D_800DD444 = 0x258;
        } else if (3 == (s8) D_800DD446) {
            D_800DD446 = 1;
        }
        D_800DD434 = arg0;
    } else {
        D_800DD434 = arg0;
    }
}

#define S444 D_800DD444
#define S436 (*(s16 *)&D_800DD436)
void func_15179008(s32 arg0) {
    s32 i;
    s32 temp;
    s32 max;

    max = 300;
    temp = D_800DD432;
    temp -= D_800BE9E4;
    if (temp <= 0) {
        if (D_800DD433 < 40) {
            temp = (func_150ADA20() % 31U) + 20;
            D_800DD433 = (func_150ADA20() & 0xF) + 170;
        } else {
            temp = (func_150ADA20() % 63U) + 60;
            D_800DD433 = (func_150ADA20() & 0xF) + 20;
        }
    }
    D_800DD432 = temp;
    switch (D_800DD446) {
    case 0:
        if (D_800DD434 == 0) {
            break;
        }
        if (D_800DD434 == 4 || S444 >= 600) {
            D_800DD440 = (s32)allocate_memory(0x4B0, 1, 0, 0);
            i = 0;
            do {
                ((s32 *)D_800DD440)[i] = 0;
                i++;
            } while (i < max);
            if (D_800DD440 != 0) {
                D_800DD446 = 1;
                D_800DD444 = 0;
                D_800DD430 = func_10010F30(0x6F, 200, 0x40, 0, 0x80);
            }
        } else if ((&D_800C35EA)[arg0] == 0) {
            D_800DD444 += D_800BE9E4;
        }
        break;
    case 1:
        if (S436 == max) {
            D_800DD446 = 2;
            D_800DD444 = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            if (S436 != max) {
                func_15179600(1, 1);
            }
        }
        if (S444 >= 11 && S436 < max) {
            func_1000F85C(D_800DD430, 8, (S436 << 14) / max);
            func_1000F85C(D_800DD430, 0x2000, 0);
        }
        D_800DD444 += D_800BE9E4;
        break;
    case 2:
        if (D_800DD434 == 1) {
            *(s8 *)&D_800DD446 = 4;
            D_800DD444 = S436;
        } else if (D_800DD434 == 0 || S444 >= 120) {
            D_800DD446 = 3;
            D_800DD444 = S436;
        } else if (D_800DD434 == 3 && (&D_800C35EA)[arg0] == 0) {
            D_800DD444 += D_800BE9E4;
        }
        break;
    case 3:
        if (S436 == 0) {
            D_800DD446 = 0;
            D_800DD444 = 0;
            func_100111C8(D_800DD430);
            func_10004074((void *)D_800DD440);
            D_800DD440 = 0;
            break;
        }
        for (i = 0; i < 4; i++) {
            func_15179AB8();
        }
        if (S444 != 0) {
            func_1000F85C(D_800DD430, 8, (S436 << 13) / S444);
        }
        break;
    case 4:
        for (i = 0; i < S436; i++) {
            func_15179AB8();
        }
        D_800DD446 = 3;
        D_800DD434 = 0;
        break;
    }
    if (S436 != 0) {
        func_151794C8();
        func_15179B14(S436 / 30);
    }
}

void func_151794C8(void) {
    s32 x;
    s32 y;
    s32 z;
    f32 angle;
    f32 vy;
    f32 vz;
    f32 c;
    f32 s;
    f32 vx;

    x = (s32)D_800DBFF0->unk2F8;
    y = (s32)D_800DBFF0->unk2FC;
    z = (s32)D_800DBFF0->unk300;

    /* offset (0, 500, 0) rotated by unk398, then by unk3A0 */
    angle = D_800DBFF0->unk398;
    c = func_150AD78C(angle);
    s = func_150AD780(angle);
    vy = 500.0f * c;
    vz = -500.0f * s;
    vx = 0.0f;

    angle = D_800DBFF0->unk3A0;
    c = func_150AD78C(angle);
    s = func_150AD780(angle);
    x += vx * s + vz * c;
    z += vz * s - vx * c;
    y += vy;
    D_800DD438.unk0 = x;
    D_800DD438.unk4 = z;
    D_800DD438.unk2 = y;
}

void func_15179600(s32 arg0, s32 arg1) {
    typedef struct {
        union {
            void *align;
            u8 bytes[0x80];
        } pad0;
        s32 unk80;
        s16 unk84;
        u8 pad86[4];
        s16 unk8A;
        s16 unk8C;
        s16 unk8E;
        s16 unk90;
        s16 unk92;
        s16 unk94;
        s16 unk96;
        u16 unk98;
        s16 pad9A;
        s16 unk9C;
        s16 unk9E;
        s8 unkA0;
        s8 unkA1;
        s8 unkA2;
        u8 padA3[5];
    } EffectSpawnPacket15179600;

    s32 spF4;
    s32 spF0;
    s32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 rand;
    s32 ret;
    s32 pad[2];
    EffectSpawnPacket15179600 packet;

    packet.unkA0 = 0x11;
    packet.unk8A = 0x33;
    packet.unkA2 = 1;
    packet.unk98 = 0;
    func_1516865C(&packet, 0xFF, 0xFF, 0xFF, 0x50);

    spE8 = (s32)D_800DBFF0->unk2F8;
    spE4 = (s32)D_800DBFF0->unk2FC;
    spE0 = (s32)D_800DBFF0->unk300;

    spF4 = (func_150ADA20() % 500) + spE8;
    spEC = (func_150ADA20() % 500) + spE0;
    if (arg0 == 0) {
        spF0 = spE4 + 0x1F4;
    } else {
        spF0 = (func_150ADA20() % 500) + spE4;
    }

    rand = func_150ADA20() % 10U;
    packet.unk9C = 0;
    packet.unk9E = 0;
    packet.unk8E = spF0;
    packet.unk8C = spF4;
    packet.unk90 = spEC;
    packet.unk92 = 4;
    packet.unk94 = rand + 0x23;
    packet.unk98 = 0x8C00;
    packet.unkA1 = rand + 0xA;
    packet.unk80 = arg1;
    packet.unk84 = *(s16 *)&D_800DD436;

    ret = func_15168800(&packet, 0xFF, 0, spF0);
    if (ret != 0) {
        ((s32 *)D_800DD440)[(s16)D_800DD436] = ret;
        D_800DD436 = (s16)D_800DD436 + 1;
    }
}


typedef struct {
    u8 pad0[0x90];
    s32 unk90;
    s16 unk94;
    u8 pad96[0x6];
    s16 unk9C;
    s16 unk9E;
    s16 unkA0;
    u8 padA2[0xC];
    u16 unkAE;
    u8 padB0;
    u8 unkB1;
    u8 padB2[2];
    u8 unkB4;
} Obj151797B0;

extern f32 func_15048A40(u8 arg0);
extern f32 func_150489B0(u8 arg0);

void func_151797B0(Obj151797B0 *arg0) {
    f32 dist;
    f32 sn;
    f32 cs;
    f32 dx;
    f32 step;
    f32 dz;
    s32 speed;
    s32 vol;
    s32 wrapped;
    s32 flags;
    s16 cx;
    s16 cy;
    s16 cz;

    dx = (f32)arg0->unkA0 - D_800DBFF0->unk300;
    dz = (f32)arg0->unk9C - D_800DBFF0->unk2F8;
    dist = sqrtf(dx * dx + dz * dz);
    vol = 80.0f - (80.0f * dist) / 1000.0f;
    if (vol < 0) {
        vol = 0;
    }
    speed = arg0->unkAE;
    arg0->unkB4 = vol;
    speed += (u32)((D_800DD433 << 4) * ((func_150ADA20() & 0xFF) + 0x80)) >> 9;
    speed = speed * 0.888890028f;
    if (speed > 0x3200) {
        speed = 0x3200;
    }
    arg0->unkAE = speed;
    step = arg0->unkB1 * D_800BE9E4;
    sn = func_15048A40(arg0->unkAE >> 8);
    arg0->unk9C = arg0->unk9C + step * sn;
    cs = func_150489B0(arg0->unkAE >> 8);
    arg0->unk9E = arg0->unk9E - step * cs;
    wrapped = 0;
    cx = D_800DD438.unk0;
    cy = D_800DD438.unk2;
    cz = D_800DD438.unk4;
    if (arg0->unk9C < cx - 500) {
        arg0->unk9C = cx - (cx - arg0->unk9C) + 1000;
        wrapped = 1;
    } else if (arg0->unk9C > cx + 500) {
        arg0->unk9C = cx + (arg0->unk9C - cx) - 1000;
        wrapped = 1;
    }
    if (arg0->unk9E < cy - 500) {
        arg0->unk9E = cy - (cy - arg0->unk9E) + 1000;
        wrapped = 1;
    } else if (arg0->unk9E > cy + 500) {
        arg0->unk9E = cy + (arg0->unk9E - cy) - 1000;
        wrapped = 1;
    }
    if (arg0->unkA0 < cz - 500) {
        arg0->unkA0 = cz - (cz - arg0->unkA0) + 1000;
        wrapped = 1;
    } else if (arg0->unkA0 > cz + 500) {
        arg0->unkA0 = cz + (arg0->unkA0 - cz) - 1000;
        wrapped = 1;
    }
    flags = arg0->unk90;
    if ((flags & 2) && wrapped) {
        D_800DD436 = (s16)D_800DD436 - 1;
        ((s32 *)D_800DD440)[arg0->unk94] = 0;
        func_1516972C((struct102 *)arg0);
    } else if (flags & 1) {
        if (wrapped) {
            arg0->unk90 = flags ^ 1;
        } else {
            arg0->unkB4 = 0;
        }
    }
}

extern u16 D_800DD436;
extern s32 D_800DD440;
void func_15179AB8(void)
{
  s32 i;
  s32 off;
  s32 **p;
  int stride;
  s32 *obj;
  i = ((s16) D_800DD436) - 1;
  stride = 4;
  if (i < 0)
  {
    return;
  }
  off = i * stride;
  p = (s32 **) (((u8 *) D_800DD440) + off);
  do
  {
    obj = *p;
    if (obj != 0)
    {
      if (!(obj[36] & 0x2))
      {
        obj[36] |= 0x2;
        return;
      }
    }
    off -= stride;
    p--;
  }
  while (off >= 0);
}


void func_15179B14(s32 arg0)
{
    typedef struct {
        union {
            void *align;
            u8 bytes[0x80];
        } pad0;
        s32 unk80;
        s16 pad84;
        u8 pad86[4];
        s16 unk8A;
        s16 unk8C;
        s16 unk8E;
        s16 unk90;
        s16 unk92;
        s16 unk94;
        s16 unk96;
        u16 unk98;
        s16 pad9A;
        s16 pad9C;
        s16 pad9E;
        s8 unkA0;
        s8 unkA1;
        s8 unkA2;
        u8 padA3[5];
    } EffectSpawnPacket15179B14;

    EffectSpawnPacket15179B14 packet;
    EffectSpawnPacket15179B14 *packetPtr;
    s16 temp_s5;
    s16 temp_s6;
    s32 var_s2;
    f32 temp_f20;
    s32 temp_s3;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_v0;

    temp_s5 = D_800DD438.unk0;
    temp_s6 = D_800DD438.unk4;
    var_s2 = 0;
    if (arg0 > 0) {
        temp_f20 = D_800A7208;
        packetPtr = (EffectSpawnPacket15179B14 *)((u8 *)&packet - 0x20);
        temp_s3 = 0x1F4;
        do {
            temp_s0 = (func_150ADA20() % temp_s3) + temp_s5;
            temp_s1 = (func_150ADA20() % temp_s3) + temp_s6;
            temp_v0 = func_1510F8D8(temp_s0, 0x2710, temp_s1, 0);
            if (temp_f20 != (f32)temp_v0) {
                packetPtr->unk80 = 0;
                packetPtr->unk8A = 0x80;
                packetPtr->unk8C = temp_s0;
                packetPtr->unk8E = temp_v0;
                packetPtr->unk90 = temp_s1;
                packetPtr->unk94 = 0x19;
                packetPtr->unk92 = 0x19;
                packetPtr->unk96 = 0;
                packetPtr->unkA2 = 0xE;
                packetPtr->unkA0 = 0x12;
                packetPtr->unk98 = 0x9804;
                packetPtr->unkA1 = 0;
                func_1516865C(packetPtr, 0xFF, 0xFF, 0xFF, 0xFF);
                func_15168800(packetPtr, 0xFF, 0);
            }
            var_s2++;
        } while (var_s2 != arg0);
    }
}

void func_15179CB0(struct15179CB0 *arg0)
{
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f0;

    temp_f2 = (f32)arg0->unkA0 - D_800DBFF0->unk300;
    temp_f12 = (f32)arg0->unk9C - D_800DBFF0->unk2F8;
    temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f12 * temp_f12));
    arg0->unkB4 = (u32)(255.0f - ((160.0f * temp_f0) / 1000.0f));
}
