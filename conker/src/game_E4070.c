#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150B6C90(void);
void func_150B6D34(void);
void func_150B6D78(void);
void func_150B7484(void);
void func_150B7560(void);
void func_150B765C(void);
void func_150B77A8(void);
void func_150B768C(void);
void func_150B791C(void);

void func_150B6BC0(s32 arg0) {
    switch ((u8)D_800D9890) {
    case 1:
        func_150B6C90();
        return;
    case 2:
        func_150B6D34();
        return;
    case 4:
        func_150B6D78();
        return;
    case 5:
        func_150B7484();
        return;
    case 6:
        func_150B7560();
        return;
    case 7:
        func_150B765C();
        return;
    case 8:
        func_150B77A8();
        return;
    case 9:
        func_150B768C();
        return;
    case 10:
        func_150B791C();
    default:
        return;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E4070/func_150B6C90.s")

extern s32 *D_800D9898[];
extern s32 *D_800D98A4[];

void func_150B6D34(void) {
    s32 i;
    s32 *e;

    i = 0;
    do {
        e = D_800D9898[i + 5];
        if (e != 0) {
            e[8] = 1;
        }
        i++;
    } while (&D_800D98A4[0] != &D_800D9898[i]);
    D_800D9890 = 3;
}

extern s32 *D_800D9894;
extern s32 *D_800D98C0[];

void func_150B6D78(void) {
    s32 i;

    if (D_800D9894 != 0) {
        func_1516972C((struct102 *)D_800D9894);
        D_800D9894 = 0;
    }
    i = 0;
    do {
        if (D_800D9898[i] != 0) {
            func_1516972C((struct102 *)D_800D9898[i]);
            D_800D9898[i] = 0;
        }
        i++;
    } while (&D_800D98C0[0] != &D_800D9898[i]);
    D_800D9890 = 3;
}

void func_150B6DFC(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x34) += D_800BE9E4 * 0x30;
    if (*(s16 *)((u8 *)arg0 + 0x34) >= 0x801) {
        *(s16 *)((u8 *)arg0 + 0x34) = -0xC00;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E4070/func_150B6E3C.s")

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x10];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x11];
    u8 unk45;
} Struct150B709C;

void func_150B709C(Struct150B709C *arg0) {
    s32 div;
    s32 var_v0;
    s32 var_v1;

    div = 0x1E;
    var_v0 = arg0->unk18;
    if (var_v0 == div) {
        var_v1 = arg0->unk45;
        var_v1 += D_800BE9E4 * 8;
        if (var_v1 >= 0x100) {
            var_v1 = 0;
        }
        arg0->unk45 = var_v1;
    }
    var_v0 += D_800BE9E4 * 2;
    if (var_v0 >= 0x1F) {
        var_v0 = div;
    }
    arg0->unk18 = var_v0;
    arg0->unk2C = (f32)(((var_v0 * -0x54) / div) + 0xE6);
    arg0->unk30 = (f32)(((var_v0 * -0x32) / div) + 0xAA);
}

extern s32 D_800BE9E4;
void func_150B71A8(void *arg0)
{
  u8 *new_var;
  s16 temp_v0;
  unsigned int temp_v1;
  temp_v1 = 0x1000;
  temp_v0 = *((s16 *) (((u8 *) arg0) + 0x38));
  if (temp_v1 == temp_v0)
  {
  }
  else
  {
    *((s16 *) (((u8 *) arg0) + 0x38)) = temp_v0 + (D_800BE9E4 * 0x100);
    if ((*((s16 *) (((u8 *) arg0) - -0x38))) >= 0x1001)
    {
      *((s16 *) (((u8 *) arg0) + 0x38)) = temp_v1;
    }
    return;
  }
  temp_v0 = *((s16 *) (((u8 *) arg0) + 0x3A));
  if (temp_v1 != temp_v0)
  {
    *((s16 *) (((u8 *) arg0) + 0x3A)) = temp_v0 + (D_800BE9E4 * 0x100);
    if ((*((s16 *) (((u8 *) arg0) + 0x3A))) >= 0x1001)
    {
      new_var = (u8 *) arg0;
      *((s16 *) (new_var + 0x3A)) = temp_v1;
    }
  }
}


typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 pad3A[2];
} Struct150B7220;

extern u8 D_80091924;
s32 func_15169900(void *arg0, s32 arg1);

void func_150B7220(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  Struct150B7220 sp44;
  s32 r;
  s32 x;
  s32 y;
  x = arg0;
  y = arg1;
  sp44.unk0 = &D_80091924;
  sp44.unk8 = (x << 16) | (y & 0xFFFF);
  sp44.unk28 = 0x1000;
  sp44.unk2A = 0x1000;
  sp44.unkC = arg2;
  sp44.unk14 = arg2;
  sp44.unk18 = 0;
  sp44.unk24 = 0;
  sp44.unk26 = 0;
  sp44.unk2C = 0x10;
  sp44.unk2E = 0x10;
  sp44.unk30 = 7;
  sp44.unk31 = 2;
  sp44.unk32 = 0xFF;
  sp44.unk33 = 0;
  sp44.unk34 = 0;
  if (y)
  {
  }
  sp44.unk35 = 0xFF;
  sp44.unk36 = 7;
  sp44.unk37 = 0x11;
  if (x < (D_800BE638 >> 1))
  {
 do { r = D_800BE638 - x; } while (0);
  }
  else
  {
    r = x;
  }
  sp44.unk10 = (x << 16) | ((y - r) & 0xFFFF);
  sp44.unk39 = 0;
  sp44.unk16 = 0;
  sp44.unk1C = x;
  sp44.unk20 = y - r;
  func_15169900(&sp44, arg3);
  sp44.unk10 = (x << 16) | ((y + r) & 0xFFFF);
  sp44.unk39 = 8;
  sp44.unk20 = y + r;
  func_15169900(&sp44, arg3);
  sp44.unk1C = x - r;
  sp44.unk10 = ((x - r) << 16) | (y & 0xFFFF);
  sp44.unk39 = 4;
  sp44.unk16 = 0x100;
  sp44.unk20 = y;
  func_15169900(&sp44, arg3);
  sp44.unk10 = ((x + r) << 16) | (y & 0xFFFF);
  sp44.unk39 = 0;
  sp44.unk1C = x + r;
  func_15169900(&sp44, arg3);
}

typedef struct {
    char pad18[0x18];
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    char pad26[6];
    f32 unk2C;
    f32 unk30;
} Struct73F0;

void func_150B73F0(Struct73F0 *a0) {
    s32 a = a0->unk18;
    s32 b = a0->unk1A;
    s32 c = a0->unk20;
    s32 e = a0->unk22;
    s32 d = a0->unk1C;
    s32 n = a0->unk24;
    s32 t1 = (n << 16) / d;
    a0->unk2C = (f32)((((c - a) * t1) >> 16) + a);
    a0->unk30 = (f32)((((e - b) * t1) >> 16) + b);
}

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8  unk30;
    /* 0x31 */ u8  unk31;
    /* 0x32 */ u8  unk32;
    /* 0x33 */ u8  unk33;
    /* 0x34 */ u8  unk34;
    /* 0x35 */ u8  unk35;
    /* 0x36 */ u8  unk36;
    /* 0x37 */ u8  unk37;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ u8  unk39;
} struct_7484;

extern u8 D_800918DC;
extern u8 D_800918E8;
extern u8 D_800918F4;
extern u8 D_80091900;
struct102 *func_15169968(struct_7484 *);

void func_150B7484(void) {
    struct_7484 sp1C;

    sp1C.unk31 = 1;
    sp1C.unk2C = 0x58;
    sp1C.unk1C = 60.0f;
    sp1C.unk20 = 60.0f;
    sp1C.unk14 = 0;
    sp1C.unk16 = 0;
    sp1C.unk18 = 0;
    sp1C.unk2E = 0x58;
    sp1C.unk30 = 6;
    sp1C.unk0 = &D_800918DC;
    sp1C.unk32 = 0xFF;
    sp1C.unk33 = 0;
    sp1C.unk34 = 0;
    sp1C.unk35 = 0xFF;
    sp1C.unk24 = 0;
    sp1C.unk26 = 0;
    sp1C.unk39 = 0;
    sp1C.unk36 = 7;
    sp1C.unk37 = 0x15;
    sp1C.unk38 = 1;
    sp1C.unk28 = 0x51;
    sp1C.unk2A = 0x51;
    if (D_800D9898[0] != 0) {
        func_1516972C((struct102 *)D_800D9898[0]);
    }
    D_800D9898[0] = (s32 *)func_15169968(&sp1C);
    D_800D9890 = 3;
}

void func_150B7560(void) {
    struct_7484 sp1C;

    if (D_800D9898[0] != 0) {
        func_1516972C((struct102 *)D_800D9898[0]);
        D_800D9898[0] = 0;
    }
    sp1C.unk18 = 0x4D;
    sp1C.unk31 = 0xF;
    sp1C.unk14 = 0;
    sp1C.unk16 = 0;
    sp1C.unk2C = 0x58;
    sp1C.unk2E = 0x58;
    sp1C.unk30 = 0;
    sp1C.unk0 = &D_800918DC;
    sp1C.unk24 = 0;
    sp1C.unk26 = 0;
    sp1C.unk32 = 0xFF;
    sp1C.unk33 = 0;
    sp1C.unk34 = 0;
    sp1C.unk35 = 0xFF;
    sp1C.unk36 = 7;
    sp1C.unk37 = 0x11;
    sp1C.unk38 = 1;
    sp1C.unk39 = 0;
    sp1C.unk28 = 0x1000;
    sp1C.unk2A = 0x1000;
    sp1C.unk1C = 60.0f;
    sp1C.unk20 = 60.0f;
    if (D_800D98A4[0] != 0) {
        func_1516972C((struct102 *)D_800D98A4[0]);
    }
    D_800D98A4[0] = (s32 *)func_15169968(&sp1C);
    D_800D9890 = 3;
}

void func_150B76BC(s32, s32);

void func_150B765C(void) {
    func_150B76BC(0x3C, 1);
    D_800D9890 = 3;
}

void func_150B768C(void) {
    func_150B76BC(0xE6, 2);
    D_800D9890 = 3;
}

void func_150B76BC(s32 arg0, s32 arg1) {
    struct_7484 sp24;

    sp24.unk1C = (f32)arg0;
    sp24.unk20 = 170.0f;
    sp24.unk31 = 1;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk30 = 6;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk0 = &D_800918E8;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0xFF;
    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk39 = 0;
    sp24.unk36 = 7;
    sp24.unk37 = 0x15;
    sp24.unk38 = 1;
    sp24.unk28 = 0x51;
    sp24.unk2A = 0x51;
    if (D_800D9898[arg1] != 0) {
        func_1516972C((struct102 *)D_800D9898[arg1]);
    }
    D_800D9898[arg1] = (s32 *)func_15169968(&sp24);
}

void func_150B77A8(void) {
    struct_7484 sp24;

    if (D_800D9898[1] != 0) {
        func_1516972C((struct102 *)D_800D9898[1]);
        D_800D9898[1] = 0;
    }

    sp24.unk1C = 60.0f;
    sp24.unk20 = 170.0f;
    sp24.unk2C = 0x40;
    sp24.unk2E = 0x40;
    sp24.unk30 = 4;
    sp24.unk0 = &D_800918F4;
    sp24.unk4 = &D_80091900;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk31 = 0;
    sp24.unk24 = -0x800;
    sp24.unk26 = 9;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0xFF;
    sp24.unk36 = 0xD;
    sp24.unk37 = 0x12;
    sp24.unk38 = 2;
    sp24.unk39 = 3;
    sp24.unk28 = 0x1000;
    sp24.unk2A = 0x1000;
    if (D_800D9898[4] != 0) {
        func_1516972C((struct102 *)D_800D9898[4]);
    }
    D_800D9898[4] = (s32 *)func_15169968(&sp24);

    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk30 = 0;
    sp24.unk36 = 7;
    sp24.unk37 = 0x11;
    sp24.unk0 = &D_800918E8;
    sp24.unk38 = 1;
    if (D_800D9898[8] != 0) {
        func_1516972C((struct102 *)D_800D9898[8]);
    }
    D_800D9898[8] = (s32 *)func_15169968(&sp24);
    D_800D9890 = 3;
}

extern u8 D_8009190C;

void func_150B791C(void) {
    struct_7484 sp24;

    if (D_800D9898[2] != 0) {
        func_1516972C((struct102 *)D_800D9898[2]);
        D_800D9898[2] = 0;
    }
    sp24.unk1C = 230.0f;
    sp24.unk31 = 3;
    sp24.unk2C = 0x40;
    sp24.unk2E = 0x10;
    sp24.unk30 = 5;
    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk0 = &D_8009190C;
    sp24.unk4 = 0;
    sp24.unk8 = 0;
    sp24.unk20 = 150.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk10 = 0;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0x80;
    sp24.unk36 = 7;
    sp24.unk37 = 0x11;
    sp24.unk38 = 1;
    sp24.unk39 = 0;
    sp24.unk28 = 0x1000;
    sp24.unk2A = 0x1000;
    if (D_800D9898[5] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[5] = (s32 *)func_15169968(&sp24);
    sp24.unk16 = 0x100;
    sp24.unk8 = 0x100;
    sp24.unk20 = 170.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk35 = 0xFF;
    if (D_800D9898[6] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[6] = (s32 *)func_15169968(&sp24);
    sp24.unk16 = 0x200;
    sp24.unk8 = 0x200;
    sp24.unk20 = 190.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk35 = 0x80;
    if (D_800D9898[7] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[7] = (s32 *)func_15169968(&sp24);
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk31 = 0;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk35 = 0xFF;
    sp24.unk30 = 0;
    sp24.unk0 = &D_800918E8;
    sp24.unk1C = 230.0f;
    sp24.unk20 = 170.0f;
    if (D_800D9898[9] != 0) {
        func_1516972C((struct102 *)D_800D9898[9]);
    }
    D_800D9898[9] = (s32 *)func_15169968(&sp24);
    D_800D9890 = 3;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3_150B85C0;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_150B7B40;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ Vec3_150B7B40 pos;
    /* 0x34 */ f32 vel[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
} Particle150B7B40;

typedef struct {
    Vec3_150B7B40 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 unk7C;
    u8 unk7D;
    u8 pad7E[2];
    s16 unk80;
    s16 unk82;
    s32 unk84;
} Burst150B7B40;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  pad1D[0x7];
} Floor150B7B40;

extern f32 D_8009FD30[3];
extern f32 D_8009FD3C[3];
extern f32 D_8009FD48[3];

void func_15142314(s32, s32, f32 *);
void func_15143134(f32 *, f32 *, s32);
void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_150B82D0(Vec3_150B7B40 *, f32 *, f32 *, f32 *, u8, s32);
void func_150B85C0(Vec3_150B85C0 *, f32 *, f32 *, u8, s32);
void func_1504715C(Floor150B7B40 *, struct127 *);
s32 func_15046C80(Vec3_150B7B40 *, s32, f32, Floor150B7B40 *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_15151A38(Burst150B7B40 *, u8, s32);

void func_150B7B40(struct127 *arg0, u8 arg1, s32 arg2) {
    f32 pts[2][3];
    f32 base[3];
    f32 sx;
    f32 sy;
    f32 scale;
    f32 ss;
    f32 cc;
    s32 pad5;
    Particle150B7B40 p;
    s32 pad2[3];
    s8 i;
    f32 offX;
    f32 offZ;
    Vec3_150B7B40 pos;
    f32 h;
    f32 r;
    f32 ratio;

    if (arg0 != NULL) {
        if (arg0->unk1D4 != NULL) {
            func_15142314((s32)arg0->unk1D4, 4, pts[0]);
            func_15142314((s32)arg0->unk1D4, 5, pts[1]);
            func_15142314((s32)arg0->unk1D4, 0, base);
        } else {
            base[0] = arg0->x_position;
            base[1] = arg0->y_position;
            base[2] = arg0->z_position;
            pts[0][0] = base[0] + 20.0f;
            pts[0][1] = base[1] + 130.0f;
            pts[0][2] = base[2];
            pts[1][0] = base[0] - 20.0f;
            pts[1][1] = base[1] + 130.0f;
            pts[1][2] = base[2];
        }
        ss = func_151423D8(arg0->unk76 >> 8);
        cc = func_151423D8((arg0->unk76 >> 8) + 0x40);
        offX = arg0->xz_velocity * ss;
        offZ = arg0->xz_velocity * cc;
        {
            f32 a[3];
            f32 b[3];

            if (arg0->unk1D4 != NULL) {
                s32 m;
                f32 dx;
                f32 dz;

                m = (s32)arg0->unk1D4 + 0x80;
                func_15143134(D_8009FD30, &pos.x, m);
                func_15143134(D_8009FD3C, a, m);
                func_15143134(D_8009FD48, b, m);
                h = fabsf(pos.y - a[1]);
                dx = b[0] - a[0];
                dz = b[2] - a[2];
                r = sqrtf(dx * dx + dz * dz);
            } else {
                func_1515C244(arg0, &pos.x, &r, &h);
            }
        }
        if (r != 0.0f) {
            ratio = h / r;
        } else {
            ratio = 1.0f;
        }
        func_150B82D0(&pos, &h, &r, &ratio, arg1, arg2);
        func_150B85C0((Vec3_150B85C0 *)&pos, &h, &r, arg1, arg2);
        {
            Floor150B7B40 floor;
            Vec3_150B7B40 ray;

            ray.x = pos.x;
            ray.y = pos.y + 100.0f;
            ray.z = pos.z;
            func_1504715C(&floor, arg0);
            if (func_15046C80(&ray, 0, pos.y - 2000.0f, &floor) != 0) {
                ray.y = floor.unk00;
                func_1514C678(ray.x, ray.y + 31.0f, ray.z, r, 0, 0xFF,
                              (func_150ADA20() % 11U) + 0x1E, 0x19, 0, 0, 0, arg1);
            }
        }
        p.unk56 = 0;
        p.unk54 = 0x12C;
        p.unk4 = 0.6000000238f;
        p.unk10 = arg0->unkB8;
        p.unk14 = arg0->unk40;
        p.unk18 = arg0->unkC4;
        p.unk1C[0] = 1.0f;
        p.unk1C[1] = 1.0f;
        p.unk1C[2] = 1.0f;
        p.unk50 = 0x197D;
        p.unk58 = 0;
        p.unk5C = 0;
        p.unk60 = 0xFF;
        p.unk61 = 1;
        p.unk62 = 0;
        p.unk63 = 1;
        p.unk64 = 0;
        p.unk65 = 1;
        p.unk66 = 0;
        p.unk67 = 0;
        p.unk68 = 0;
        p.unk6A = 0;
        p.unk6C = 0;
        p.unk70 = 0;
        p.unk72 = 1;
        p.unk74 = 0xFF;
        p.unk48 = 0;
        p.unk4C = -1.600000024f;
        sx = arg0->xz_scale;
        if (sx > 1.0f) {
            sx = 1.0f;
        }
        if (sx < 0.5f) {
            sx = 0.5f;
        }
        sy = arg0->y_scale;
        if (sy > 1.0f) {
            sy = 1.0f;
        }
        if (sy < 0.5f) {
            sy = 0.5f;
        }
        p.unk0 = ((28.0f * sy) + (28.0f * sx)) * 0.5f;
        p.unk8 = 3.0f * sx;
        p.unkC = 3.0f * sy;
        for (i = 0; i < 2; i++) {
            f32 speed;
            s16 ang1;
            s16 ang2;
            f32 s1;
            f32 c1;
            f32 s2;
            f32 c2;
            f32 hv;
            Burst150B7B40 burst;

            speed = (func_150ADA68() * 30.0f * arg0->y_scale) + 10.0f;
            ang1 = func_150ADA20() & 0x7F;
            ang2 = -0x20 - (func_150ADA20() & 0x1F);
            s1 = func_151423D8(ang1);
            c1 = func_151423D8(ang1 - 0x40);
            s2 = func_151423D8(ang2);
            c2 = func_151423D8(ang2 - 0x40);
            hv = speed * s2;
            p.pos = *(Vec3_150B7B40 *)pts[i];
            p.vel[0] = (hv * c1) + offX;
            p.vel[1] = -speed * c2;
            p.vel[2] = (hv * s1) + offZ;
            p.unk40 = 30.0f - (func_150ADA68() * 60.0f);
            p.unk44 = 30.0f - (func_150ADA68() * 60.0f);
            func_15132A4C(&p, 3, 2, 0, arg1, 0);
            scale = ((arg0->xz_scale + arg0->y_scale) * 0.5f) / 0.9599999785f;
            burst.unk0 = *(Vec3_150B7B40 *)pts[i];
            burst.unk24 = 4;
            burst.unk26 = 4;
            burst.unk2A = 0xFF;
            burst.unk2C = -0x3F;
            burst.unk2E = 0x4B;
            burst.unkC = 3.0f * scale;
            burst.unk10 = 3.0f * scale;
            burst.unk14 = 0.6000000238f;
            burst.unk18 = 0.2310000062f;
            burst.unk1C = 15.0f * scale;
            burst.unk20 = 10.0f * scale;
            burst.unk28 = 0;
            burst.unk30 = 3;
            burst.unk32 = 3;
            burst.unk34 = 0x32;
            burst.unk36 = 0x14;
            burst.unk38 = 1;
            burst.unk39 = 0x21;
            burst.unk3A = 0x48;
            burst.unk3C = 1;
            burst.unk40 = 0;
            burst.unk44 = 0;
            burst.unk48 = 0;
            burst.unk4C = 0;
            burst.unk50 = 0;
            burst.unk54 = 0;
            burst.unk55 = 0x9B;
            burst.unk56 = 0x64;
            burst.unk58 = 3;
            burst.unk5C = 0xFF;
            burst.unk60 = 0;
            burst.unk64 = 0x220005;
            burst.unk68 = 0x1D0600;
            burst.unk6C = 1;
            burst.unk70 = 0x3B;
            burst.unk74 = 0x80;
            burst.unk78 = 0x20;
            burst.unk7C = 0;
            burst.unk7D = 7;
            burst.unk80 = 0xA;
            burst.unk82 = 0x19;
            burst.unk84 = arg0->unk184;
            func_15151A38(&burst, arg1, arg2);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_E4070/func_150B82D0.s")


typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
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
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec3_150B85C0 unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Fx150B85C0;

extern void *func_15130374(void *, u8, s32, u8, s32);

void func_150B85C0(Vec3_150B85C0 *arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4) {
    Fx150B85C0 sp28;

    sp28.unk1D = 0x29;
    sp28.unk08 = 0xE03;
    sp28.unk00 = 0x200005;
    sp28.unk04 = 0;
    sp28.unk0A = (func_150ADA20() % 11U) + 0x23;
    sp28.unk0C = 0;
    sp28.unk10 = 0;
    sp28.unk18 = 0xB0;
    sp28.unk19 = 0xA0;
    sp28.unk1A = 0x2A;
    sp28.unk14 = 0x40;
    sp28.unk15 = 0xB;
    sp28.unk16 = 0x6A;
    sp28.unk17 = 0xFF;
    sp28.unk1B = (func_150ADA20() % 51U) + 0x64;
    sp28.unk1C = 0xFF;
    sp28.unk60 = 3;
    sp28.unk61 = 3;
    sp28.unk28 = *arg2 * 20.0f;
    sp28.unk2C = *arg1 * 20.0f;
    sp28.unk30 = *arg0;
    sp28.unk3C = 0.0f;
    sp28.unk40 = 0.0f;
    sp28.unk44 = 0.0f;
    sp28.unk48 = 0.0f;
    sp28.unk4C = 0.0f;
    sp28.unk50 = 0.0f;
    sp28.unk54 = 0.0f;
    sp28.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE01) | 0x4000 | 0x8000;
    sp28.unk62 = -1;
    sp28.unk63 = -1;
    sp28.unk1E = 0x19;
    sp28.unk20 = 0xA;
    sp28.unk22 = 0x28;
    sp28.unk64 = -1;
    sp28.unk65 = 0;
    sp28.unk24 = 1.009518027f;
    func_15130374(&sp28, 1, 0, arg3, arg4);
}

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
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
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[0x9];
} Fx150B879C;

extern void func_15143874(s16, f32, f32 *, f32 *);
void *memcpy(void *, const void *, u32);

s32 func_150B879C(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 spA4;
    void *ret;
    Fx150B879C sp30;

    sp30.unk1D = 0x29;
    sp30.unk08 = 0xE03;
    sp30.unk00 = 0x200005;
    sp30.unk04 = 0;
    spA4 = 0.9172409773f;
    sp30.unk0A = (func_150ADA20() & 0xF) + 0x19;
    sp30.unk0C = 0;
    sp30.unk10 = 0;
    sp30.unk18 = 0xB0;
    sp30.unk19 = 0xA0;
    sp30.unk1A = 0x2A;
    sp30.unk14 = 0x40;
    sp30.unk15 = 0xB;
    sp30.unk16 = 0x6A;
    sp30.unk17 = 0xFF;
    sp30.unk1B = (func_150ADA20() % 71U) + 0x3C;
    sp30.unk1C = 0xFF;
    sp30.unk60 = 3;
    sp30.unk61 = 3;
    sp30.unk28 = sp30.unk2C = ((func_150ADA68() * 15738.0f) + 24699.0f) * 0.01f;
    sp30.unk30 = arg2;
    sp30.unk34 = arg3;
    sp30.unk38 = arg4;
    sp30.unk3C = 0.0f;
    sp30.unk40 = 0.0f;
    sp30.unk44 = 0.0f;
    func_15143874(arg8, ((func_150ADA68() * 154.0f) + 205.0f) * 0.1f, &sp30.unk48, &sp30.unk50);
    sp30.unk4C = 0.0f;
    sp30.unk54 = 0.0f;
    sp30.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE05) | 0x4000 | 0x8000;
    sp30.unk62 = 0xA;
    sp30.unk63 = -1;
    sp30.unk1E = 0x1C;
    sp30.unk20 = 9;
    sp30.unk22 = 0x19;
    sp30.unk64 = -1;
    sp30.unk65 = 0;
    sp30.unk24 = 1.033332944f;
    ret = func_15130374(&sp30, 1, 4, argE, 1);
    if (ret != 0) {
        memcpy((u8 *)ret + 0xA8, &spA4, 4);
    }
    return 1;
}
