#include <ultra64.h>
#include "functions.h"
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


#pragma GLOBAL_ASM("asm/nonmatchings/game_1DD500/func_151B0050.s")

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

void func_151B01B8(void *arg0, void *arg1) {
    Game1DD500Impact packet;
    u8 shade;
    Game1DD500Hit hit;
    s32 effectProfileIndex;

    if ((arg0 != 0) && (*(s32 *)((u8 *)arg0 + 0x1D4) != 0) && ((*(u8 *)((u8 *)arg0 + 0x74) & 0xF) != 0xF)) {
        func_1504715C(&hit, arg0);
        effectProfileIndex = arg1 != 0 ?
            ((*(u8 *)((u8 *)arg1 + 4) == 0x53 || *(u8 *)((u8 *)arg1 + 4) == 0xA5) ?
                4 : func_15134070(arg1)) : 4;
        if (effectProfileIndex != 0x63) {
            if (D_800A3FD8[effectProfileIndex * 0x10 + 0xE] != 2) {
                shade = ((s32) *(u16 *)((u8 *)arg0 + 0x7A) >> 8) + 0x40;
                func_15143134(&D_800AA120, &packet.position[0], *(s32 *)((u8 *)arg0 + 0x1D4) + 0x180);
                packet.field2C = 0xA;
                packet.field2E = 0x14;
                packet.field00 = shade - 0x68;
                packet.field14 = 7.21999979f;
                packet.field02 = 0xD0;
                packet.field04 = -0x1B;
                packet.field06 = 0x36;
                packet.field30 = 3;
                packet.field32 = 2;
                packet.field34 = 0x1E;
                packet.field36 = 0x28;
                packet.field38 = 0x9B;
                packet.field3A = 0x64;
                packet.field18 = 3.039999962f;
                packet.field1C = 0.810000062f;
                packet.field20 = 0.405000031f;
                packet.field24 = 6.0f;
                packet.field28 = 13.90000057f;
                packet.field40 = 0.5f;
                if (D_800A3FD8[effectProfileIndex * 0x10 + 0xE] == 1) {
                    packet.field3C = 1;
                } else {
                    packet.field3C = 0;
                }
                packet.field44 = 0x10;
                packet.field46 = 0xF;
                packet.field48 = 0;
                func_15153F18(&packet.field00, &packet.position[0], (s32) &hit, 0xFF, 1);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1DD500/func_151B03B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1DD500/func_151B09BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1DD500/func_151B0B88.s")

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
void func_151DAB58(u8, f32, u8, f32 *, s32, s32, s32);

s32 func_151B118C(Game1DD500ProjObj *arg0) {
    f32 dx;
    f32 dz;
    f32 dy;
    Game1DD500Proj *p;
    s8 ret;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;
    f32 probe[3];
    f32 pos[3];
    f32 size;

    p = &arg0->proj;
    oldY = arg0->pos[1];
    oldVx = p->vx;
    oldVz = p->vz;
    ret = 1;
    p->vx *= 0.25f * D_800BE9A4;
    p->vz *= 0.25f * D_800BE9A4;
    dx = (p->vx + oldVx) * 0.5f * D_800BE9A4;
    dy = p->vy * D_800BE9A4 + p->ay * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (p->vz + oldVz) * 0.5f * D_800BE9A4;
    p->vy += p->ay * D_800BE9A4;
    arg0->pos[0] += dx;
    arg0->pos[1] += dy;
    arg0->pos[2] += dz;
    arg0->lag[0] += dx;
    arg0->lag[1] += dy;
    arg0->lag[2] += dz;
    arg0->lag[0] = (arg0->lag[0] - arg0->pos[0]) * 0.706000030f + arg0->pos[0];
    arg0->lag[1] = (arg0->lag[1] - arg0->pos[1]) * 0.706000030f + arg0->pos[1];
    arg0->lag[2] = (arg0->lag[2] - arg0->pos[2]) * 0.706000030f + arg0->pos[2];
    if (p->unk24) {
        probe[0] = arg0->pos[0];
        probe[1] = oldY - arg0->unk30;
        probe[2] = arg0->pos[2];
        if (func_15046C80(probe, 0, arg0->pos[1] - arg0->unk30, &arg0->unk78)) {
            pos[0] = probe[0];
            pos[1] = arg0->unk78 + 5.0f;
            pos[2] = probe[2];
            size = ((p->unk14 + p->unk1C * 0.5f) + (p->unk18 + p->unk20 * 0.5f)) * 0.5f;
            if (func_150ADA20() & 1) {
                func_151D9B8C(p->unk25, size * 1.70000005f, arg0->unk5C, (s32)&arg0->unk7C, (struct17 *)pos, 100, 0, 1, 0, arg0->unkC, arg0->unk1);
            } else {
                func_151DAB58(p->unk25, size * 0.680000007f, arg0->unk5C, pos, 1, arg0->unkC, arg0->unk1);
            }
            ret = 0;
        }
    }
    return ret;
}

s32 func_151B1478(struct260 *arg0) {
    s32 v1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) < 0x20) {
        v1 = *(s16 *)((u8 *)arg0 + 0x1C) << 3;
        if (v1 < *(u8 *)((u8 *)arg0 + 0x5C)) {
            *(u8 *)((u8 *)arg0 + 0x5C) = v1;
        }
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1DD500/func_151B14AC.s")

s32 func_151B1828(CompactOscillatorObject *arg0) {
    CompactOscillatorObject *p = arg0;
    CompactOscillatorMotionState *f = &arg0->field_0x110;
    f32 r1;
    f32 r2;
    f->field_0x10 = f->field_0x10 + f->field_0x12 * D_800BE9E4;
    f->field_0x11 = f->field_0x11 + f->field_0x13 * D_800BE9E4;
    r1 = func_151423D8((u8)(f->field_0x10 - 0x40));
    r2 = func_151423D8((u8)(f->field_0x11 - 0x40));
    p->field_0x2C += (f->field_0x14 + f->field_0x1C * r1 - p->field_0x2C) * 0.5f;
    p->field_0x30 += (f->field_0x18 + f->field_0x20 * r2 - p->field_0x30) * 0.5f;
    return 1;
}

void func_151B1918(struct260 *arg0) {
    struct {
        s32 head[2];
        s32 count;
        s32 entries[11][3];
    } *list;
    s32 i;

    list = (void *)((u8 *)arg0 + 0x28);
    list->count = 0;
    *(f32 *)((u8 *)arg0 + 0xB8) = 0.0f;
    for (i = 0; i < 11; i++) {
        s32 *entry = list->entries[i];

        if (list->entries[i][0] != 0) {
            func_1516972C((struct102 *)entry[0]);
        }
        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 0;
    }
}

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

void func_151B19A4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151B19A4 *temp_v0 = (SubA151B19A4 *)((u8 *)arg0 + 0x28);
    ArgB151B19A4 *b = (ArgB151B19A4 *)arg1;
    s32 v1 = temp_v0->unk0;

    if (arg2 == 0 || arg2 == 0x13) {
        if ((v1 == b->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

void func_151B1A58(struct260 *arg0) {
    func_151B1918(arg0);
    func_1514933C(arg0);
}

void func_151B1A84(struct260 *arg0) {
    func_151B1918(arg0);
    func_15149368(arg0);
}

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
