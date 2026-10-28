#include <ultra64.h>
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_15169260
#include "variables.h"



typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    s32 unk14;
    u8 unk18;
    u8 pad19[3];
    s32 unk1C;
    u8 unk20;
    u8 pad21[3];
} Payload_15108850;

typedef struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    u8 pad03;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[0xA];
    s32 unk30;
    s32 unk34;
    u8 unk38;
    u8 pad39[3];
} Init_15108850;

struct Ret_15108850 {
    u8 pad[0x50];
    s32 off;
};

struct Ret_15108850 *func_1513B5E0(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4);
struct225 *func_1516037C(void *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4);

void func_15108850(s32 arg0) {
    struct Ret_15108850 *ret;

    {
        Init_15108850 init1;
        Payload_15108850 payload1;

        payload1.unk10 = 226.0f;
        payload1.unk14 = 0x3E7;
        payload1.unk18 = 0xFF;
        payload1.unk1C = -1;
        payload1.unk20 = 1;
        init1.unk02 = 1;
        init1.unk04 = 0x12C;
        init1.unk30 = 9;
        payload1.unk00 = 0.0f;
        payload1.unk04 = 0.0f;
        payload1.unk08 = 0.0f;
        payload1.unk0C = 0.0f;
        init1.unk00 = 0;
        init1.unk01 = 0;
        init1.unk34 = 0x1AB;
        init1.unk08 = 1;
        init1.unk0C = 0x220205;
        init1.unk10 = 0x40600;
        init1.unk24 = 0;
        init1.unk25 = 0;
        init1.unk14 = 1;
        init1.unk18 = 0x36;
        init1.unk1C = 0x80;
        init1.unk20 = 0x20;
        init1.unk38 = 1;

        ret = func_1513B5E0(&init1, 1, 0x24, 0xFF, 1);
        if (ret != 0) {
            memcpy((u8 *)ret + ret->off + 0xF8, &payload1, 0x24);
            {
                Header header;
                void *ptr;
                ptr = ret;
                header.unk0 = 2;
                header.unk1 = 8;
                header.unk2 = 0x12C;
                header.unk4 = 0x13;
                ret = (struct Ret_15108850 *)func_1516037C(&header, arg0, 4, 0xFF, 1);
                if (ret != 0) {
                    memcpy((u8 *)ret + 0x18, &ptr, 4);
                }
            }
        }
    }

    {
        struct Ret_15108850 *ret2;
        s32 pad;
        Init_15108850 init2;
        Payload_15108850 payload2;

        payload2.unk10 = 226.0f;
        payload2.unk14 = 0x3E7;
        payload2.unk18 = 0xFF;
        payload2.unk1C = -1;
        payload2.unk20 = 1;
        init2.unk01 = 1;
        init2.unk02 = 4;
        init2.unk04 = 0x12C;
        init2.unk30 = 9;
        payload2.unk00 = 0.0f;
        payload2.unk04 = 0.0f;
        payload2.unk08 = 0.0f;
        payload2.unk0C = 0.0f;
        init2.unk00 = 0;
        init2.unk34 = 0x1AC;
        init2.unk08 = 1;
        init2.unk0C = 0x220205;
        init2.unk10 = 0x40600;
        init2.unk24 = 0;
        init2.unk25 = 0;
        init2.unk14 = 1;
        init2.unk18 = 0x36;
        init2.unk1C = 0x80;
        init2.unk20 = 0x20;
        init2.unk38 = 1;

        ret2 = func_1513B5E0(&init2, 1, 0x24, 0xFF, 1);
        if (ret2 != 0) {
            memcpy((u8 *)(ret2) + (ret2)->off + 0xF8, &payload2, 0x24);
        }
    }
}

extern f32 D_800BE9A4;
extern f32 D_800A2470;
extern f32 D_800A2474;
extern f32 D_800A2478;
extern f32 D_800A247C;
extern f32 D_800A2480;
extern f32 D_800A2484;
extern f32 D_800A2488;
extern f32 D_800A248C;
f32 sinf(f32);
f32 func_15144B68(f32);
struct Arg0_15108AB4;
void func_15108B80(struct Arg0_15108AB4 *);

struct Arg0_15108BC0;
void func_15108BC0(struct Arg0_15108BC0 *arg0);

struct Data_151090DC {
    s32 unk0;
    s32 unk4;
};
extern struct Data_151090DC D_80088C50;
extern struct Data_151090DC D_80088C58;
void func_15169260(struct Data_151090DC *, s32, s32, u8);

struct Arg0_15108AB4 {
    u8 pad_0[0x12];
    u8 field_0x12;
    u8 pad_1[0x3D];
    s32 off;
};

struct F8StateData {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xC;
    f32 field_0x10;
    s32 field_0x14;
    u8 field_0x18;
    u8 pad_0x19[0x3];
    s32 field_0x1C;
    u8 field_0x20;
};

s32 func_15108AB4(struct Arg0_15108AB4 *arg0) {
    struct F8StateData *v0 = (struct F8StateData *)((((u8 *)arg0) + arg0->off) + 0xF8);

    v0->field_0x8 = v0->field_0x8 + D_800A2470 * D_800BE9A4;
    v0->field_0xC = v0->field_0xC + D_800A2474 * D_800BE9A4;
    v0->field_0x8 = func_15144B68(v0->field_0x8);
    v0->field_0xC = func_15144B68(v0->field_0xC);
    v0->field_0x0 = sinf(v0->field_0x8) * D_800A2478;
    v0->field_0x4 = sinf(v0->field_0xC) * D_800A247C;
    func_15108B80(arg0);
    func_15108BC0((struct Arg0_15108BC0 *)arg0);
    return 1;
}

void func_15108B80(struct Arg0_15108AB4 *arg0) {
    s32 *p;

    p = (s32*)((u8*)arg0 + arg0->off);
    if ((p = (s32*)((u8*)p + 0xF8), 0x3E7) == p[0x14/4]) {
    } else {
        if ((p[0x1C/4] -= D_800BE9E4) < 0) {
            p[0x14/4] = 0x3E7;
        }
    }
}

struct Arg0_15108BC0 {
    u8 pad[0x50];
    s32 off;
};

void func_15108BC0(struct Arg0_15108BC0 *arg0) {
    struct F8StateData *v0 = (struct F8StateData *)(((u8 *)arg0 + arg0->off) + 0xF8);
    s32 v1 = v0->field_0x14;

    if (v1 == 0x3E7) {
        v0->field_0x10 = 226.0f;
        return;
    }
    if (D_800C35EA == 1) {
        struct233 **tmp = &D_800C3958;
        struct233 *e = *tmp + v1;
        v0->field_0x10 = e->unk4;
    } else {
        v0->field_0x10 = 226.0f;
    }
}

s32 func_15108C38(struct Arg0_15108AB4 *arg0) {
    struct F8StateData *v0 = (struct F8StateData *)((((u8 *)arg0) + arg0->off) + 0xF8);

    v0->field_0x8 = v0->field_0x8 + D_800A2480 * D_800BE9A4;
    v0->field_0xC = v0->field_0xC + D_800A2484 * D_800BE9A4;
    v0->field_0x8 = func_15144B68(v0->field_0x8);
    v0->field_0xC = func_15144B68(v0->field_0xC);
    v0->field_0x0 = sinf(v0->field_0x8) * D_800A2488;
    v0->field_0x4 = sinf(v0->field_0xC) * D_800A248C;
    func_15108B80(arg0);
    func_15108BC0((struct Arg0_15108BC0 *)arg0);
    if (v0->field_0x20 != 0) {
        arg0->field_0x12 = 4;
    } else {
        arg0->field_0x12 = 2;
    }
    return 1;
}

extern f32 D_800A2490;

s32 func_15108D24(struct Arg0_15108AB4 *arg0, s32 arg1) {
    f32 mtx[4][4];
    struct F8StateData *v0;

    v0 = (struct F8StateData *)((s32)arg0 + arg0->off + 0xF8);
    func_150A8050(mtx, v0->field_0x0, 0.0f, v0->field_0x4);
    mtx[3][1] = v0->field_0x10;
    mtx[0][0] *= D_800A2490;
    mtx[0][1] *= D_800A2490;
    mtx[0][2] *= D_800A2490;
    mtx[1][0] *= D_800A2490;
    mtx[1][1] *= D_800A2490;
    mtx[1][2] *= D_800A2490;
    mtx[2][0] *= D_800A2490;
    mtx[2][1] *= D_800A2490;
    mtx[2][2] *= D_800A2490;
    guMtxF2L(mtx, (Mtx *)((u8 *)arg0 + (D_800BE9C0 << 6) + 0x78));
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_135D00/func_15108E10.s")

struct Data_15108FFC {
    s32 unk0;
    s32 unk4;
    u8 unk8;
};

void func_15108FFC(s32 arg0, s32 arg1, u8 arg2) {
    struct Data_15108FFC sp24;
    struct Data_151090DC sp1C;

    sp1C = D_80088C50;
    sp24.unk0 = arg0;
    sp24.unk4 = arg1;
    sp24.unk8 = arg2;
    func_15169260(&sp1C, 2, (s32)&sp24, 0x1D);
}

struct Arg0_15109064
{
  u8 pad[0x50];
  s32 off;
};
void func_15109064(struct Arg0_15109064 *arg0, s32 *arg1, u8 arg2)
{
  u8 *v0 = (((u8 *) arg0) + arg0->off) + 0xF8;
  u8 *flag;
  switch (arg2)
  {
    case 0x1D:
      *((s32 *) (v0 + 0x14)) = arg1[0];
      *((u8 *) (v0 + 0x18)) = *((u8 *) (((u8 *) arg1) + 0x8));
      *((s32 *) (v0 + 0x1C)) = arg1[1];
 goto dummy_label_480240; dummy_label_480240: ;
      break;

    case 0x1E:
      if ((*((u8 *) (flag = v0 + 0x20))) != 0)
    {
      *((u8 *) flag) = 0;
    }
    else
    {
      *((u8 *) (v0 + 0x20)) = 1;
    }
      break;

  }

}


void func_151090DC(void) {
    struct Data_151090DC sp18;

    sp18 = D_80088C58;
    func_15169260(&sp18, 2, 0, 0x1E);
}

void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
f32 *func_15144B34(s32);

typedef struct {
    u8 pad0[0x50];
    s32 unk50;
    u8 pad54[4];
    Vtx *unk58[2][4];
    Mtx unk78[2];
} S15109120;

s32 func_15109120(S15109120 *arg0, s16 arg1) {
    f32 mtx[4][4];
    f32 *src;
    f32 *dir;
    f32 cs;
    f32 sn;
    f32 inv;
    f32 x;
    f32 z;
    f32 tu;
    f32 tv;
    f32 pa;
    f32 oz;
    f32 oy;
    f32 ox;
    s32 i;

    src = (f32 *)((u8 *)(arg0 + 1) + arg0->unk50);
    func_150A8050(mtx, src[0], 0.0f, src[1]);
    mtx[3][1] = src[4];
    mtx[0][0] *= 0.1570000052f;
    mtx[0][1] *= 0.1570000052f;
    mtx[0][2] *= 0.1570000052f;
    mtx[1][0] *= 0.1570000052f;
    mtx[1][1] *= 0.1570000052f;
    mtx[1][2] *= 0.1570000052f;
    mtx[2][0] *= 0.1570000052f;
    mtx[2][1] *= 0.1570000052f;
    mtx[2][2] *= 0.1570000052f;
    guMtxF2L(mtx, &arg0->unk78[D_800BE9C0]);
    dir = func_15144B34(arg1);
    x = dir[0];
    if (0.0000999999975f < fabsf(x) || 0.0000999999975f < fabsf(dir[1])) {
        f32 zz;
        f32 ii;
        zz = dir[2];
        ii = 1.0f / sqrtf(x * x + zz * zz);
        cs = zz * ii;
        sn = -x * ii;
    } else {
        cs = 1.0f;
        sn = 0.0f;
    }
    i = 0;
    do {
        Vtx *v;
        v = &arg0->unk58[D_800BE9C0][arg1][i];
        inv = v->v.ob[0]; pa = v->v.ob[1]; z = v->v.ob[2]; func_150A7960((f32 *)mtx, inv, pa, z, &ox, &oy, &oz);
        i++;
        tv = oy * 0.3580000103f + 81.0f;
        tu = ((ox * cs) + (oz * sn)) * 0.2130000144f + 26.0f;
        v->v.tc[0] = (s16)(s32)(tu * 32.0f);
        v->v.tc[1] = (s16)(s32)(tv * 32.0f);
    } while (i != 16);
    return 1;
}
