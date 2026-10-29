#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad_0x0[0x18];
    s32 field_0x18;
    s32 field_0x1C;
    u8 pad_0x20[0x34];
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5C;
    u8 pad_0x60[0x8];
    s32 field_0x68;
    s32 field_0x6C;
    s32 field_0x70;
    s32 field_0x74;
    s32 field_0x78;
    f32 field_0x7C[2][4][4];
    s32 field_0xFC;
    u8 field_0x100;
    u8 pad_0x101[0x3];
    s32 field_0x104[4];
    s32 field_0x114;
    s32 field_0x118;
    u8 pad_0x11C[0x4];
    f32 field_0x120[0xE];
} struct15157010;

extern struct15157010 *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_1503F62C(s32, s32, s32 *, s32 *, s32 *, s32 *, s32 *);
extern void func_1503F5B8(s32, s32, s32, f32, f32, s32);
extern void guMtxIdentF(float mf[4][4]);
extern s32 func_1515D480(s32);
extern s32 func_1515D440(void);
extern void func_151539B4(void *, u8);
extern void func_15130374(s32, u8, s32, u8, s32);
extern s32 (*D_8008AD90[])(void *);
extern s32 (*D_8008ADA0[])(void *);
extern void func_1503F4B0(void *, void *, u8);

#define LOW_U8_151568F8(arg) (*(u8 *)((u8 *)&(arg) + 3))

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} struct151568F8_0C;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct151568F8_08;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    s8 unk11;
    s8 unk12;
    u8 pad13[0x3];
    s16 unk16;
    u8 pad18[0x2B];
    u8 unk43;
    u8 pad44[0x20];
    s16 unk64;
    s16 unk66;
    void *unk68;
} struct151572D0;

typedef struct {
    struct151568F8_0C unk0;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
    u8 unk47;
    u16 unk48;
    u8 pad4A[2];
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    u8 pad56[2];
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69[3];
    struct151568F8_08 unk6C;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    u8 pad7A[2];
    f32 unk7C;
    s32 unk80;
    u8 unk84;
    u8 unk85;
    s8 unk86;
    s8 unk87;
    u8 unk88;
} struct151568F8_src;

typedef struct {
    s16 unk0;
    s16 unk2;
    struct151568F8_0C unk4;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
} struct151568F8_first;

typedef struct {
    s32 unk0;
    s32 unk4;
    u16 unk8;
    s16 unkA;
    s32 unkC;
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
    struct151568F8_08 unk28;
    struct151568F8_0C unk30;
    u8 pad3C[0xC];
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
    u8 pad64[0xC];
} struct151568F8_second;

typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 pad41;
    s16 unk42;
    u8 pad44[0x8];
    u8 unk4C;
    u8 pad4D;
    s16 unk4E;
    s16 unk50;
} Struct151563B8;


/* prior best attempt (old score 15); may be a good start */
struct Obj15156190Vec {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Obj15156190Vec4 {
    struct Obj15156190Vec unk0;
    s32 padC;
};

struct Obj15156190Src {
    struct Obj15156190Vec unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 pad29;
    s16 unk2A;
    u16 unk2C;
    u8 pad2E[2];
    f32 unk30;
    u8 unk34;
    u8 pad35;
    s16 unk36;
    s16 unk38;
};

struct Obj15156190Ret {
    u8 pad0[0x10];
    struct Obj15156190Vec unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct Obj15156190Vec unk28;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 pad41;
    s16 unk42;
    u16 unk44;
    u8 pad46[2];
    f32 unk48;
    u8 unk4C;
    u8 pad4D;
    s16 unk4E;
    s16 unk50;
    u8 pad52[0xC];
    s16 unk5E;
    s16 unk60;
    s16 unk62;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 pad68[6];
    s16 unk6E;
    u8 pad70[4];
    u8 unk74;
    u8 unk75;
    u8 unk76;
    u8 unk77;
    u8 pad78[6];
    s16 unk7E;
    u8 pad80[4];
    u8 unk84;
    u8 unk85;
    u8 unk86;
    u8 unk87;
    u8 unk88[0x10];
};

void *func_15156190(struct Obj15156190Src *arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  struct Obj15156190Ret *temp_v0;
  struct Obj15156190Ret *sp44;
  s32 id;
  struct Obj15156190Vec sp30;
  switch (arg1)
  {
    case 0:
      id = 0x2C;
      break;

    case 1:
      id = 0x53;
      break;

    default:
      id = 0x2C;
      break;

  }

  temp_v0 = (struct Obj15156190Ret *) func_15167A68(id, arg4, arg2 + 0x98, 1, (u8) arg3, 1);
  if (temp_v0 == 0)
  {
    return 0;
  }
  sp44 = temp_v0;
  sp44->unk5E = 0;
  sp44->unk64 = arg0->unk24;
  sp44->unk65 = arg0->unk25;
  sp44->unk66 = arg0->unk26;
  sp44->unk67 = arg0->unk27;
  sp44->unk60 = 0;
  sp44->unk62 = 0;
  sp44->unk6E = 0;
  sp44->unk74 = arg0->unk20;
  sp44->unk75 = arg0->unk21;
 do { } while (0);
  sp44->unk76 = arg0->unk22;
  sp44->unk77 = arg0->unk23;
  sp44->unk7E = 0;
  sp44->unk84 = arg0->unk20;
  sp44->unk85 = arg0->unk21;
  sp44->unk86 = arg0->unk22;
  sp44->unk87 = arg0->unk23;
  sp30 = arg0->unk0;
  sp44->unk28 = sp30;
  sp44->unk10 = sp30;
  sp44->unk1C = arg0->unkC * arg0->unk18;
  sp44->unk20 = arg0->unk10 * arg0->unk18;
  sp44->unk24 = arg0->unk14 * arg0->unk18;
  sp44->unk34 = arg0->unkC * arg0->unk1C;
  sp44->unk38 = arg0->unk10 * arg0->unk1C;
  sp44->unk3C = arg0->unk14 * arg0->unk1C;
  sp44->unk40 = arg0->unk28;
  sp44->unk42 = arg0->unk2A;
  sp44->unk44 = arg0->unk2C;
  sp44->unk48 = arg0->unk30;
  sp44->unk4C = arg0->unk34;
  sp44->unk4E = arg0->unk36;
  sp44->unk50 = arg0->unk38;
  bzero(sp44->unk88, 0x10);
  return sp44;
}


extern void *func_15156190(struct Obj15156190Src *arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_15156388(void *arg0, u8 arg1, s32 arg2) {
    func_15156190(arg0, arg1, arg2, 0xFF, 0);
}

void func_151563B8(void *arg0) {
    Struct151563B8 *self;
    u8 done;
    s16 temp;

    self = arg0;
    done = 0;
    if (self->unk40 & 1) {
        self->unk42 -= D_800BE9E4;
        if (self->unk42 < 0) {
            done = 1;
        }
    }
    if (!done) {
        self->unk10 += self->unk1C * D_800BE9A4;
        self->unk14 += self->unk20 * D_800BE9A4;
        self->unk18 += self->unk24 * D_800BE9A4;
        self->unk28 += self->unk34 * D_800BE9A4;
        self->unk2C += self->unk38 * D_800BE9A4;
        self->unk30 += self->unk3C * D_800BE9A4;
        if (self->unk40 & 8) {
            if (self->unk42 < self->unk4E) {
                temp = self->unk42 * self->unk50;
                if (temp < self->unk4C) {
                    self->unk4C = temp;
                }
            }
        }
    }
    if (done) {
        func_1516972C((struct102 *)self);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_183640/func_151564F8.s")

void func_151568F8(struct151568F8_src *arg0, s32 arg1) {
    struct151568F8_first first;
    struct151568F8_second second;

    first.unk0 = arg0->unkC;
    first.unk2 = arg0->unkE;
    first.unk4 = arg0->unk0;
    first.unk10 = arg0->unk10;
    first.unk14 = arg0->unk14;
    first.unk12 = arg0->unk12;
    first.unk16 = arg0->unk16;
    first.unk18 = arg0->unk18;
    first.unk1C = arg0->unk1C;
    first.unk20 = arg0->unk20;
    first.unk24 = arg0->unk24;
    first.unk28 = arg0->unk28;
    first.unk29 = arg0->unk29;
    first.unk2A = arg0->unk2A;
    first.unk2B = arg0->unk2B;
    first.unk2C = arg0->unk2C;
    first.unk2D = arg0->unk2D;
    first.unk2E = arg0->unk2E;
    first.unk2F = arg0->unk2F;
    first.unk30 = arg0->unk30;
    first.unk32 = arg0->unk32;
    first.unk34 = arg0->unk34;
    first.unk36 = arg0->unk36;
    first.unk38 = arg0->unk38;
    first.unk3C = arg0->unk3C;
    first.unk40 = arg0->unk40;
    first.unk41 = arg0->unk41;
    first.unk42 = arg0->unk42;
    first.unk44 = arg0->unk44;
    first.unk46 = arg0->unk46;
    func_151539B4(&first, LOW_U8_151568F8(arg1));

    second.unk1D = arg0->unk47;
    second.unk8 = arg0->unk48;
    second.unk4 = arg0->unk50;
    second.unk0 = arg0->unk4C;
    second.unkC = arg0->unk58;
    second.unk10 = arg0->unk5C;
    second.unk14 = arg0->unk60;
    second.unk15 = arg0->unk61;
    second.unk16 = arg0->unk62;
    second.unk17 = arg0->unk63;
    second.unk18 = arg0->unk64;
    second.unk19 = arg0->unk65;
    second.unk1A = arg0->unk66;
    second.unk30 = arg0->unk0;
    second.unk58 = arg0->unk80;
    second.unk1B = arg0->unk67;
    second.unk1C = arg0->unk68;
    second.unk62 = arg0->unk86;
    second.unk63 = arg0->unk87;
    second.unk1E = arg0->unk74;
    second.unk20 = arg0->unk76;
    second.unk60 = arg0->unk84;
    second.unk61 = arg0->unk85;
    second.unk22 = arg0->unk78;
    second.unk24 = arg0->unk7C;
    second.unkA = arg0->unk54;
    second.unk28 = arg0->unk6C;
    second.unk48 = 0.0f;
    second.unk4C = 0.0f;
    second.unk50 = 0.0f;
    second.unk54 = 0.0f;
    func_15130374((s32)&second, arg0->unk88, 0, LOW_U8_151568F8(arg1), 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_183640/func_15156B54.s")

typedef struct {
    struct151568F8_0C unk0;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 pad31;
    s16 unk32;
    s16 unk34;
    u16 unk36;
    f32 unk38;
    f32 unk3C;
    u8 unk40;
    u8 unk41;
    s16 unk42;
    s16 unk44;
    u8 unk46;
    u8 unk47;
    u16 unk48;
    u8 pad4A[2];
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    u8 pad56[2];
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69[3];
    f32 unk6C;
    f32 unk70;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    u8 pad7A[2];
    f32 unk7C;
    s32 unk80;
    u8 unk84;
    u8 unk85;
    s8 unk86;
    s8 unk87;
    u8 unk88;
    u8 unk89;
    u8 unk8A;
    u8 unk8B;
    u8 unk8C;
} struct15156D24;

extern f32 D_800A6040;
extern f32 D_800A6044;
extern f32 D_800A6048;
extern f32 D_800A604C;
extern f32 D_800A6050;

void func_15156D24(void *arg0, u8 arg1) {
    struct15156D24 s;

    memcpy(&s.unk0, arg0, 0xC);
    s.unkC = 0xA;
    s.unkE = 0x1E;
    s.unk12 = -0x3F;
    s.unk14 = 0xFF;
    s.unk16 = 0x80;
    s.unk8A = s.unk28 = s.unk64 = 0xFF;
    s.unk8B = s.unk29 = s.unk65 = 0xFF;
    s.unk2A = s.unk66 = 0xFF;
    s.unk8C = 0xFF;
    s.unk2B = 0xFF;
    s.unk2C = 0xFF;
    s.unk2D = 0xFF;
    s.unk2E = 0xFF;
    s.unk2F = 0xFF;
    s.unk30 = 9;
    s.unk10 = 0;
    s.unk32 = 5;
    s.unk34 = 0xA;
    s.unk36 = 0x1601;
    s.unk40 = 0x64;
    s.unk41 = 0x9B;
    s.unk42 = 4;
    s.unk44 = 0x3F;
    s.unk46 = 0;
    s.unk47 = 0x2B;
    s.unk48 = 0x1A01;
    s.unk4C = 0x200005;
    s.unk50 = 0;
    s.unk20 = s.unk24 = D_800A6040;
    s.unk18 = D_800A6044;
    s.unk1C = D_800A6048;
    s.unk38 = D_800A604C;
    s.unk3C = D_800A6050;
    s.unk54 = (func_150ADA20() % 3U) + 7;
    s.unk58 = 0;
    s.unk5C = 0;
    s.unk60 = 0xFF;
    s.unk61 = 0xFF;
    s.unk62 = 0xFF;
    s.unk63 = 0xFF;
    func_150ADA20();
    s.unk67 = 0xFF;
    s.unk68 = 0xFF;
    s.unk6C = s.unk70 = (func_150ADA68() * 150.0f) + 196.0f;
    s.unk74 = 4;
    s.unk76 = 0x3F;
    s.unk78 = 0x16;
    s.unk7C = 1.25f;
    s.unk80 = 0xE01;
    if (func_150ADA20() & 1) {
        s.unk80 |= 0x40;
    }
    if (func_150ADA20() & 1) {
        s.unk80 |= 0x80;
    }
    s.unk84 = 4;
    s.unk85 = 4;
    s.unk86 = -1;
    s.unk87 = -1;
    s.unk88 = 1;
    s.unk89 = 1;
    func_151568F8((struct151568F8_src *)&s, arg1);
}

extern void func_151D5E30(void *);

void func_15156F94(void *arg0) {
    func_151D5E30((void *)((u8 *)arg0 + 0x88));
}

void func_15156FB8(struct102 *arg0) {
    func_15156F94(arg0);
    func_15169804(arg0);
}

void func_15156FE4(struct102 *arg0) {
    func_15156F94(arg0);
    func_15169824(arg0);
}

struct15157010 *func_15157010(u8 *arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, s32 arg7) {
    struct15157010 *temp_v0;
    f32 (*mtx1)[4];
    f32 (*mtx2)[4];
    s32 id;
    s32 i;

    id = 0x36;
    if (arg0[0] & 0x80) {
        id = 0x5B;
    } else if (arg0[0] & 0x10) {
        id = 0x4C;
    }
    temp_v0 = func_15167A68(id, arg7, arg5 + 0x120, 1, arg6, 1);
    if (temp_v0 == 0) {
        return NULL;
    }
    memcpy(&temp_v0->pad_0x0[0x10], arg0, 0x58);
    func_1503F62C(temp_v0->field_0x18, temp_v0->field_0x1C, &temp_v0->field_0x6C, &temp_v0->field_0x70, &temp_v0->field_0x74, &temp_v0->field_0x78, &temp_v0->field_0x68);
    mtx1 = temp_v0->field_0x7C[0];
    guMtxIdentF(mtx1);
    mtx2 = temp_v0->field_0x7C[1];
    guMtxIdentF(mtx2);
    *(f32 (**) [4]) (temp_v0->field_0x68 + 0x3E0) = mtx1;
    *(f32 (**) [4]) (temp_v0->field_0x68 + 0x3E4) = mtx2;
    func_1503F5B8(temp_v0->field_0x68, 1, arg1, arg2, 0.0f, 0);
    temp_v0->field_0xFC = arg3;
    temp_v0->field_0x118 = arg4;
    temp_v0->field_0x100 = 0;
    for (i = 0; i < 4; i++) {
        temp_v0->field_0x104[i] = 0;
    }

    temp_v0->field_0x114 = 0;
    if (arg3) {
        for (i = 0; i <= D_80082FA0; i++) {
            temp_v0->field_0x104[i] = func_1515D480(arg3);
        }
        temp_v0->field_0x114 = func_1515D440();
    }
    return temp_v0;
}

void func_100043B4(s32 *arg0, u32 arg1);
extern s32 D_80082FA0;
void func_151571C4(void *arg0)
{
  s32 i;
  s32 *p;
  for (i = 0, p = (s32 *) arg0; i <= D_80082FA0; i++, p++)
  {
    if (p[0x41] != 0)
    {
      func_100043B4((s32 *) p[0x41], 4);
    }
  }

  p = (s32 *) ((s32 *) arg0)[0x45];
  if (((s32 *) arg0)[0x45] != 0)
  {
    func_100043B4(p, 4);
  }
}

extern void func_151571C4(void *);


extern void func_151571C4(void *);
extern void func_1518CA04(void *);
extern void func_1503F7B8(void *);

void func_15157248(u8 *arg0) {
    func_151571C4(arg0);
    func_1518CA04(*(void **)(arg0 + 0x18));
    func_1503F7B8(*(void **)(arg0 + 0x68));
    func_15169804(arg0);
}

void func_1515728C(u8 *arg0) {
    func_151571C4(arg0);
    func_1518CA04(*(void **)(arg0 + 0x18));
    func_1503F7B8(*(void **)(arg0 + 0x68));
    func_15169824(arg0);
}

void func_151572D0(struct151572D0 *ptr) {
    register s32 temp;
    register u8 failed;
    register s16 temp_v1;

    failed = 0;
    if (ptr->unk10 & 1) {
        ptr->unk16 -= D_800BE9E4;
        if (ptr->unk16 < 0) {
            failed = 1;
        }
    }
    if (failed == 0) {
        temp = ptr->unk11;
        if (temp != -1) {
            if (D_8008AD90[temp](ptr) == 0) {
                failed = 1;
            }
        }
        temp = ptr->unk12;
        if (temp != -1) {
            if (D_8008ADA0[temp](ptr) == 0) {
                failed = 1;
            }
        }
    }
    if (failed) {
    } else {
        func_1503F4B0(ptr->unk68, ptr, failed);
    }
    if (failed) {
    } else if (ptr->unk10 & 0x20) {
        temp_v1 = ptr->unk16;
        if (temp_v1 < ptr->unk64) {
            temp = temp_v1 * ptr->unk66;
            if (temp < ptr->unk43) {
                ptr->unk43 = temp;
            }
        }
    }
    if (failed) {
        func_1516972C((struct102 *) ptr);
    }
}

typedef struct {
    u8 pad0[0x2];
    s16 unk2;
} Struct15157420_60;

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct15157420_mode;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[0x2];
    s8 unk13;
    s8 unk14;
    u8 pad15[0xB];
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x3];
    u8 unk33;
    s32 unk34;
    s32 unk38;
    s8 unk3C;
    s8 unk3D;
    u8 pad3E[0x2];
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 pad48[0x4];
    void *unk4C;
    u8 unk50;
    u8 unk51;
    u8 pad52[0x2];
    u8 unk54[0xC];
    Struct15157420_60 *unk60;
    u8 pad64[0x4];
    s32 *unk68;
    s32 *unk6C;
    u8 unk70;
    u8 pad71[0x3];
    s32 unk74;
    u8 pad78[0x84];
    u8 unkFC[1];
} Struct15157420;

extern Gfx *(*D_8008ADBC[])(Gfx *, Struct15157420 *, s16, u8 *, u8 *);
extern Gfx *(*D_8008ADCC[])(Gfx *, Struct15157420 *, s16, u8 *);
extern Struct15157420_mode D_800A4AC8[];
extern u8 D_80083740[];
extern u8 D_800838C0[];
extern u8 D_80083A40[];
extern u8 D_80083BC0[];
extern u8 D_800BE9C0;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151441A4(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void func_151442FC(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void *func_151462C8(void *arg0, void *arg1, u8 arg2, void *arg3, u8 arg4, s16 arg5, void *arg6, u8 arg7, s32 arg8);

Gfx *func_15157420(Gfx *gfx, Struct15157420 *arg1, s16 arg2) {
    s32 i;
    u8 update;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 cont;

    update = 1;
    if (arg1->unk13 != -1) {
        gfx = D_8008ADBC[arg1->unk13](gfx, arg1, arg2, &update, &cont);
        if (cont == 0) {
            return gfx;
        }
    }
    if ((arg1->unk10 & 8) && (arg1->unk60 != NULL) && !(arg1->unk60->unk2 & (1 << arg2))) {
        return gfx;
    }
block:
    gfx = func_15142B7C(gfx, arg1->unk24, arg1->unk28);
    func_151441A4(&primR, &primG, &primB, &primA, arg1->unk40, arg1->unk41, arg1->unk42, arg1->unk43,
                  arg1->unk44, arg1->unk45, arg1->unk46, arg1->unk47, 0xFF, arg1->unk3C);
    func_151442FC(&envR, &envG, &envB, &envA, arg1->unk40, arg1->unk41, arg1->unk42, arg1->unk43,
                  arg1->unk44, arg1->unk45, arg1->unk46, arg1->unk47, 0xFF, arg1->unk3D);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
    gfx = func_1513F4E4(gfx, arg1->unk33, &update);
    gfx = func_15142FBC(gfx, arg1->unk20 | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | arg1->unk34 | arg1->unk38,
                        D_800A4AC8[arg1->unk2C].unk0 | D_800A4AC8[arg1->unk2C].unk4, &update);
    if (arg1->unk10 & 2) {
        gfx = func_151462C8(gfx, arg1->unkFC, arg1->unk51, arg1->unk4C, arg1->unk50, arg2, arg1->unk54,
                            (arg1->unk10 & 4) ? 1 : 0, 0);
    }
    gDPPipeSync(gfx++);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, arg1->unk68[D_800BE9C0 + 0xFA]);
    gSPSegment(gfx++, 1, arg1->unk74);
    if (arg1->unk43 < 0xFF || arg1->unk47 < 0xFF) {
        gSPSegment(gfx++, 8, (arg1->unk10 & 0x40) ? D_80083A40 : D_80083740);
    } else {
        gSPSegment(gfx++, 8, (arg1->unk10 & 0x40) ? D_80083BC0 : D_800838C0);
    }
    for (i = 0; i < arg1->unk70; i++) {
        gSPDisplayList(gfx++, arg1->unk6C[i]);
    }
    if (arg1->unk14 != -1) {
        gfx = D_8008ADCC[arg1->unk14](gfx, arg1, arg2, &update);
    }
    return gfx;
}

extern u8 D_800BE9C0;
extern void guMtxIdentF(float mf[4][4]);

s32 func_15157860(u8 *arg0) {
    guMtxIdentF((float (*)[4])(arg0 + 0x7C + (D_800BE9C0 << 6)));
    return 1;
}

struct15157010 *func_15157898(u8 *arg0, void *arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8) {
    struct15157010 *temp_v0;
    struct15157010 *sp2C;

    temp_v0 = func_15157010(arg0, arg2, arg3, arg4, arg5, arg6 + 0x38, arg7, arg8);
    if (temp_v0 == 0) {
        return NULL;
    }
    sp2C = temp_v0;
    memcpy(temp_v0->field_0x120, arg1, 0x38);
    return sp2C;
}

s32 func_15157918(struct15157010 *arg0) {
    f32 *arg1;

    arg1 = arg0->field_0x120;
    func_150A8050(arg0->field_0x7C[D_800BE9C0], arg1[0], arg1[1], arg1[2]);
    arg0->field_0x7C[D_800BE9C0][3][0] = arg0->field_0x54;
    arg0->field_0x7C[D_800BE9C0][3][1] = arg0->field_0x58;
    arg0->field_0x7C[D_800BE9C0][3][2] = arg0->field_0x5C;
    arg0->field_0x7C[D_800BE9C0][0][0] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][0][1] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][0][2] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][1][0] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][1][1] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][1][2] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][2][0] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][2][1] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][2][2] *= arg1[3];
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} MotionVec;

typedef struct {
    MotionVec rot;
    f32 scale;
    MotionVec vel;
    MotionVec rotVel;
    f32 gravity;
    f32 drag;
    u8 flags;
} MotionState;

typedef struct {
    u8 pad_0x0[0x54];
    f32 posX;
    f32 posY;
    f32 posZ;
    u8 pad_0x60[0xC0];
    MotionState motion;
} struct15157AA8;

s32 func_15157AA8(struct15157AA8 *arg0) {
    MotionState *p;
    MotionVec oldVel;
    f32 accelX;
    f32 accelY;
    f32 accelZ;
    s32 i;

    p = &arg0->motion;
    if (arg0->motion.flags & 1) {
        oldVel = p->vel;
        if (p->flags & 8) {
            for (i = D_800BE9E4; i != 0; i--) {
                p->vel.x *= p->drag;
                p->vel.z *= p->drag;
            }
        }
        if (p->flags & 4) {
            p->vel.y += p->gravity * D_800BE9A4;
            accelY = p->gravity;
        } else {
            accelY = 0.0f;
        }
        accelX = (p->vel.x - oldVel.x) * D_800BE9A8;
        accelZ = (p->vel.z - oldVel.z) * D_800BE9A8;
        arg0->posX += (oldVel.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        arg0->posY += (oldVel.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        arg0->posZ += (oldVel.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
    }
    if (p->flags & 2) {
        p->rot.x += p->rotVel.x * D_800BE9A4;
        p->rot.y += p->rotVel.y * D_800BE9A4;
        p->rot.z += p->rotVel.z * D_800BE9A4;
    }
    return 1;
}

void func_15157D88(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x4C, arg0 + 0x50, arg0);
}

extern void func_15157DEC(struct15157010 *, f32 *);

s32 func_15157DC8(struct15157010 *arg0) {
    func_15157DEC(arg0, arg0->field_0x120);
    return 1;
}

void func_15157DEC(struct15157010 *arg0, f32 *arg1) {
    func_150A8050(arg0->field_0x7C[D_800BE9C0], arg1[0], arg1[1], arg1[2]);
    arg0->field_0x7C[D_800BE9C0][3][0] = arg0->field_0x54;
    arg0->field_0x7C[D_800BE9C0][3][1] = arg0->field_0x58;
    arg0->field_0x7C[D_800BE9C0][3][2] = arg0->field_0x5C;
    arg0->field_0x7C[D_800BE9C0][0][0] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][0][1] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][0][2] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][1][0] *= arg1[4];
    arg0->field_0x7C[D_800BE9C0][1][1] *= arg1[4];
    arg0->field_0x7C[D_800BE9C0][1][2] *= arg1[4];
    arg0->field_0x7C[D_800BE9C0][2][0] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][2][1] *= arg1[3];
    arg0->field_0x7C[D_800BE9C0][2][2] *= arg1[3];
}

extern Mtx D_80089470;

Gfx *func_15157F80(Gfx *pkt, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    gSPMatrix(pkt++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(pkt++, &((Mtx *)D_800DCC10)[arg2], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    *arg4 = 1;
    return pkt;
}

#define WGFX15157FE8(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *func_15157FE8(Gfx *pkt, s32 arg1, s32 arg2, s32 arg3) {
    WGFX15157FE8(pkt++, 0xDA380007, (u8 *)((struct259 *)D_800BE628 + arg2) + (D_800BE9C0 << 6) + 0x100);
    WGFX15157FE8(pkt++, 0xDA380005, ((s32 *)&D_800DC2A0)[D_800BE9C0] + (arg2 << 6));
    return pkt;
}

extern s32 D_800A6060;

void func_15158078(s32 arg0, u8 arg1) {
    func_15169260((s32)&D_800A6060, 3, arg0, arg1);
}
