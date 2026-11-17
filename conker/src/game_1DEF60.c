#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

typedef struct {
    u8 pad_0x0[0x10];
    u8 field_0x10;
    u8 field_0x11;
    s8 field_0x12;
    s8 field_0x13;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
} CompactOscillatorMotionState;

typedef struct {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    CompactOscillatorMotionState field_0x110;
} CompactOscillatorObject;

void func_151B1918(struct260 *arg0);
void func_15149368(struct260 *arg0);


void func_15143794(s16, s16, f32, f32 *);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_151B0050(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s16 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE);

typedef struct Game1DD500Impact {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    f32 position[3];
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    s8 field3C;
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Game1DD500Impact;
typedef struct Game1DD500Hit { f32 height; u8 geometry[0x20]; } Game1DD500Hit;
void func_1504715C(void *, void *);
s32 func_15134070(void *);
void func_15143134(f32 *, f32 *, s32);
void func_15153F18(s16 *, void *, s32, s32, s32);
extern u8 D_800A3FD8[];
extern f32 D_800AA120;
void func_151B01B8(void *arg0, void *arg1);

typedef struct {
    f32 y;
    u8 pad4[0x14];
    s32 f18;
    s8 f1C;
    u8 f1D;
    s32 f20;
} Probe151B03B8;
extern void func_15143874(s16, f32, f32 *, f32 *);
s32 func_15046C80(f32 *, s32, f32, void *);
s32 func_1504697C(f32 *, s32, f32, void *);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);
void func_15136698(f32, f32, s32, s32, s32, void *, f32 *, s32, s32, u8, s32);
extern f32 D_800AA2B4, D_800AA2B8, D_800AA2BC, D_800AA2C0, D_800AA2C4, D_800AA2C8;
extern f32 D_800AA2CC, D_800AA2D0, D_800AA2D4, D_800AA2D8, D_800AA2DC;
void func_151B03B8(void *arg0, u8 arg1, s32 arg2);

typedef struct {
    void *obj;
    u8 kind;
} Msg151B09BC;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Triple151B09BC;

typedef struct {
    void *obj;
    u8 kind;
    s32 unk08;
    Triple151B09BC slots[11];
    f32 unk90;
    u8 flag;
} Data151B09BC;

extern u8 D_800A3FE6[];
void *memcpy(void *, const void *, u32);
struct260 *func_151B09BC(void *arg0, void *arg1, s16 arg2, u8 arg3, s32 arg4);

typedef struct {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 ay;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
} Game1DD500Proj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x23];
    f32 unk30;
    f32 pos[3];
    f32 lag[3];
    u8 pad4C[0x10];
    u8 unk5C;
    u8 pad5D[0x1B];
    f32 unk78;
    f32 unk7C;
    u8 pad80[0x90];
    Game1DD500Proj proj;
} Game1DD500ProjObj;

s32 func_15046C80(f32 *, s32, f32, void *);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);
s32 func_151B118C(Game1DD500ProjObj *arg0);
s32 func_151B1478(struct260 *arg0);

struct Struct151B14AC {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
};

extern void func_151D5D60();
s16 *func_151B14AC(struct Struct151B14AC *arg0, s16 arg1);
s32 func_151B1828(CompactOscillatorObject *arg0);
void func_151B1918(struct260 *arg0);

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151B19A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151B19A4;
void func_151B19A4(struct260 *arg0, s32 arg1, u8 arg2);
void func_151B1A58(struct260 *arg0);
void func_151B1A84(struct260 *arg0);

void func_151B1AB0(void *arg0) {
    struct260 *temp_v0;
    struct {
        void *unk0;
        u8 unk4;
        f32 unk8;
    } sp28;

    if (*(s32 *)&arg0 != 0) {
        sp28.unk0 = arg0;
        sp28.unk4 = *(u8 *)((s32)arg0 + 0x3B);
        sp28.unk8 = 0.0f;

        temp_v0 = func_151491F4(0x3C, -1, 0x15, 1, 0x11, 0xC, 0xFF, 1);
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x28), &sp28, 0xC);
        }
    }
}

typedef struct {
    u8 pad0[0x7A];
    u16 unk7A;
    u8 pad7C[0x1D4 - 0x7C];
    s32 unk1D4;
} Obj151B1B34;

typedef struct {
    Obj151B1B34 *obj;
    f32 unk4;
    f32 timer;
} Sub151B1B34;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub151B1B34 sub;
} Arg151B1B34;

typedef struct { f32 v[3]; } Vec3_151B1B34;
extern Vec3_151B1B34 D_8008FAD0;
extern Vec3_151B1B34 D_8008FADC;
struct260 *func_15130374(void *, u8, s32, u8, s32);
extern void func_15143874(s16, f32, f32 *, f32 *);

void func_151B1B34(Arg151B1B34 *arg0) {
    Sub151B1B34 *p;
    struct260 *ret;
    f32 *src;
    f32 f;
    struct {
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
        u8 unk62;
        s8 unk63;
    } spC4;
    f32 zero;
    f32 spBC;
    f32 spB0[3];
    f32 spA4[3];

    p = &arg0->sub;
    p->timer += (697.0f + (func_150ADA68() * 342.0f)) * 0.001000000047f * D_800BE9A4;
    if (p->timer > 1.0f) {
        if (p->obj->unk1D4 != 0) {
            {
                Vec3_151B1B34 sp98 = D_8008FAD0;
                func_15143134(sp98.v, spB0, p->obj->unk1D4 + 0x800);
            }
            {
                Vec3_151B1B34 sp8C = D_8008FADC;
                func_15143134(sp8C.v, spA4, p->obj->unk1D4 + 0x640);
            }
            zero = 0.0f;
            spC4.unk1D = 0x29;
            spC4.unk08 = 0xE03;
            spC4.unk00 = 0x200005;
            spC4.unk04 = 0;
            spC4.unk18 = 0xB0;
            spC4.unk19 = 0xA0;
            spC4.unk1A = 0x2A;
            spC4.unk14 = 0x40;
            spC4.unk15 = 0xB;
            spC4.unk16 = 0x6A;
            spC4.unk0C = 0;
            spC4.unk10 = 0;
            spC4.unk17 = 0xFF;
            spC4.unk1C = 0xFF;
            spC4.unk60 = 3;
            spC4.unk61 = 3;
            spC4.unk54 = zero;
            spC4.unk58 = 0xE05;
            spC4.unk62 = 0xA;
            spC4.unk63 = -1;
            spC4.unk1E = 0x1E;
            spC4.unk20 = 8;
            spC4.unk22 = 0x32;
            spC4.unk24 = 1.02700007f;
            spBC = 0.9240000248f;
            do {
                {
                    s16 angle;
                    u8 shade;

                    spC4.unk0A = (func_150ADA20() % 41U) + 30;
                    spC4.unk1B = (func_150ADA20() % 156U) + 100;
                    f = (func_150ADA68() * 104.0f) + 199.0f;
                    spC4.unk2C = spC4.unk28 = f;
                    src = (func_150ADA20() & 1) ? spB0 : spA4;
                    shade = (p->obj->unk7A >> 8) + 0x40;
                    angle = (func_150ADA20() % 100U) + shade - 49;
                    func_15143874(angle, 36.0f, &spC4.unk30, &spC4.unk38);
                    spC4.unk34 = zero;
                    spC4.unk30 += src[0];
                    spC4.unk34 = src[1];
                    spC4.unk38 += src[2];
                    func_15143874(angle, ((func_150ADA68() * 1497.0f) + 995.0f) * 0.009999999776f, &spC4.unk48, &spC4.unk50);
                    spC4.unk4C = zero;
                }
                spC4.unk58 &= ~0xC0;
                if (func_150ADA20() & 1) {
                    spC4.unk58 |= 0x40;
                }
                if (func_150ADA20() & 1) {
                    spC4.unk58 |= 0x80;
                }
                ret = func_15130374(&spC4, 1, 4, arg0->unkC, arg0->unk1);
                if (ret != 0) {
                    memcpy((u8 *)ret + 0xA8, &spBC, 4);
                }
                p->timer -= 1.0f;
            } while (p->timer > 1.0f);
        } else {
            while (p->timer > 1.0f) {
                p->timer -= 1.0f;
            }
        }
    }
}


void func_151B1FAC(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151B19A4 *temp_v0 = (SubA151B19A4 *)((u8 *)arg0 + 0x28);
    ArgB151B19A4 *b = (ArgB151B19A4 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA151B19A4 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_151B1FAC:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
