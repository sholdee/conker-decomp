#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

void func_151058B4();

extern s32 D_800DCE50[][0x68];

typedef struct Func15105C24Obj {
    u8 pad0[8];
    struct Func15105C24Obj *next;
    u8 padC[7];
    u8 type;
    u8 pad14[0x14];
    s32 owner;
} Func15105C24Obj;

extern s32 *D_800DBF94;

typedef struct {
    s32 count;
    const u8 *list;
    struct127 *obj;
} Msg35;

void func_15104A80(struct131 *arg0) {
    struct127 *found;

    found = NULL;
    {
        s32 mask;
        s32 i;
        struct127 *obj;

        i = arg0 - D_800DBEF4;
        mask = D_800DBF94[i];
        if (mask != 0) {
            for (i = 0; i < 25 && found == NULL; i++) {
                obj = &D_800CC2D0[i];
                if (obj->interaction_state != 0 && (mask & (1U << i))) {
                    found = obj;
                }
            }
        }
    }
    if (((u8 *)arg0)[0x72] == 0xF9 || ((u8 *)arg0)[0x72] == 0xF8 || ((u8 *)arg0)[0x72] == 0xF7) {
        Msg35 msg;

        msg.obj = found;
        if (((u8 *)arg0)[0x72] == 0xF9) {
            const u8 list[9] = { 0, 1, 2, 3, 4, 5, 0xF, 0x10, 0x11 };

            msg.count = 9;
            msg.list = list;
        } else if (((u8 *)arg0)[0x72] == 0xF8) {
            const u8 list[9] = { 6, 7, 8, 0x18, 0x19, 0x1A, 0x12, 0x13, 0x14 };

            msg.count = 9;
            msg.list = list;
        } else if (((u8 *)arg0)[0x72] == 0xF7) {
            const u8 list[9] = { 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0x15, 0x16, 0x17 };

            msg.count = 9;
            msg.list = list;
        }
        func_151494E0((s32)&msg, 0x35);
    } else if (((u8 *)arg0)[0x72] == 0xF6) {
        Msg35 msg2;

        msg2.obj = found;
        func_151494E0((s32)&msg2.obj, 0x38);
    }
}

s32 func_1000FD38();
extern s32 func_151464B8(s16 *);

typedef struct {
    void *target;
    f32 accum;
    s16 timer;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 yOffset;
    f32 floor;
    u8 pad24[0x20];
    s16 *unk44;
} Func15104C44Data;

typedef struct {
    s16 unk0;
    u8 unk2;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    s8 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    u8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
} Func15104C44Desc;

extern void func_15143874(s16, f32, f32 *, f32 *);
extern s32 func_15046C80(f32 *, s32, f32, f32 *);
extern void func_15105CE0(Func15104C44Desc *, s32, s32, s32);

void func_15104C44(struct131 *arg0) {
    Func15104C44Data *data;

    data = (Func15104C44Data *)((u8 *)arg0 + 0x28);
    if (data->timer >= 0) {
        data->timer -= D_800BE9E4;
        if (data->timer < 0) {
            func_1000FD38(func_1000EF40, data->target, 0);
        }
        if (func_151464B8(data->unk44) == 0) {
            data->accum += (0.1970000118f + func_150ADA68() * 0.3f) * D_800BE9A4;
            if (data->accum > 1.0f) {
                Func15104C44Desc desc;
                f32 pos[3];
                f32 offZ1;
                f32 offX1;
                f32 offZ2;
                f32 offX2;

                pos[1] = data->y + data->yOffset;
                desc.unk2 = 1;
                desc.unk34 = -1;
                desc.unk44 = 3;
                desc.unk46 = 4;
                desc.unk4C = 0;
                desc.unk40 = 45.0f;
                desc.unk3C = 25.0f;
                desc.unk4 = data->x;
                desc.unk8 = data->y + data->yOffset;
                desc.unkC = data->z;
                desc.unk14 = data->y + data->yOffset - 180.0f;
                desc.unk48 = 0x61;
                desc.unk49 = 0xF2;
                desc.unk4A = 0xFF;
                do {
                    func_15143874(func_150ADA20() & 0xFF, (func_150ADA68() * 0.5f + 0.5f) * data->radius, &pos[0], &pos[2]);
                    pos[0] += data->x;
                    pos[2] += data->z;
                    if (func_15046C80(pos, 0, data->y, &data->floor) != 0) {
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX1, &offZ1);
                        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 220.0f, &offX2, &offZ2);
                        desc.unk10 = data->x + offX1;
                        desc.unk18 = data->z + offZ1;
                        desc.unk28 = pos[0];
                        desc.unk2C = data->floor;
                        desc.unk30 = pos[2];
                        desc.unk1C = offX2 + pos[0];
                        desc.unk20 = desc.unk2C + 180.0f;
                        desc.unk24 = offZ2 + pos[2];
                        desc.unk0 = func_150ADA20() % 13U + 8;
                        desc.unk4B = func_150ADA20() % 156U + 100;
                        desc.unk38 = func_150ADA20() % 3U + 5;
                        func_15105CE0(&desc, 0, ((u8 *)arg0)[0xC], ((u8 *)arg0)[1]);
                    }
                    data->accum -= 1.0f;
                } while (data->accum > 1.0f);
            }
        }
    }
}

s32 func_1000FD38();

void func_15104FF8(struct207 *arg0, s32 arg1, u8 arg2) {
    struct206 *m = (struct206 *)&arg0->unk28;
    if ((arg2 == 0x38) && (m->unk0->unk14 == 1)) {
        *(s16 *)((u8 *)m + 8) = 300;
        func_1000FD38(func_1000EF40, (struct205 *)((s32)m->unk0), 0);
        func_1000FA64(
            0x236,
            ((s16 *)m->unk0)[0],
            ((s16 *)m->unk0)[1],
            ((s16 *)m->unk0)[2],
            0x4000,
            0x5DC,
            0x3E8,
            (s32)func_1000EF40,
            m->unk0,
            0,
            8,
            0);
    }
}

typedef struct {
    f32 x, y, z;
} Vec3_151050B0;

typedef struct {
    f32 f[5];
    s16 h[5];
    s32 w0;
    s32 w1;
    u8 b[5];
} Params_151050B0;

typedef struct {
    struct205 *unk0;
    s16 unk4;
    f32 unk8;
    Vec3_151050B0 pos[4];
    Vec3_151050B0 dir[4];
    s32 unk6C;
} State_151050B0;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    State_151050B0 unk28;
} Obj_151050B0;

void *func_15106F98(Vec3_151050B0 *, Vec3_151050B0 *, s32, Params_151050B0 *, f32, u8, u8, s32);

void func_151050B0(Obj_151050B0 *arg0) {
    State_151050B0 *s;
    Params_151050B0 p;
    Vec3_151050B0 a;
    Vec3_151050B0 b;
    f32 t;
    u32 i;
    u8 j;

    s = &arg0->unk28;
    if (s->unk4 < 0) {
        s->unk0->unk14 = 1;
        return;
    }
    s->unk0->unk14 = 0;
    s->unk4 -= D_800BE9E4;
    if (s->unk4 < 0) {
        func_151494E0(0, 0x39);
        func_151494E0(0, 0x4B);
    }
    if (func_151464B8(s->unk6C) != 0) {
        return;
    }
    t = func_150ADA68();
    s->unk8 += (0.04000000283f + t * 0.0700000003f) * D_800BE9A4;
    if (s->unk8 > 1.0f) {
        p.f[0] = 10.0f;
        p.f[1] = 15.0f;
        p.f[2] = 20.0f;
        p.f[3] = 44.0f;
        p.f[4] = 37.0f;
        p.h[0] = 100;
        p.h[1] = 0;
        p.h[2] = 10;
        p.h[3] = 1;
        p.h[4] = 10;
        p.w0 = 4;
        p.w1 = 2;
        p.b[0] = 0x7E;
        p.b[1] = 0xF9;
        p.b[2] = 0xFF;
        p.b[3] = 0x37;
        p.b[4] = 0xC8;
        do {
            i = (u8)(func_150ADA68() * 3.0f);
            j = (func_150ADA68() * 2 + 1) + i;
            if (j >= 4) {
                j -= 4;
            }
            t = func_150ADA68();
            a.x = s->pos[i].x + s->dir[i].x * t;
            a.y = s->pos[i].y + s->dir[i].y * t;
            a.z = s->pos[i].z + s->dir[i].z * t;
            t = func_150ADA68();
            b.x = s->pos[j].x + s->dir[j].x * t;
            b.y = s->pos[j].y + s->dir[j].y * t;
            b.z = s->pos[j].z + s->dir[j].z * t;
            func_15106F98(&a, &b, 3, &p, 0.3250000179f, 2, arg0->unkC, arg0->unk1);
            s->unk8 -= 1.0f;
        } while (s->unk8 > 1.0f);
    }
}

void func_1510550C(struct102 *arg0, s32 arg1, u8 arg2) {
    if (arg2 == 0x4B) {
        func_1516972C(arg0);
    }
}

void func_15105548(struct207 *arg0, s32 *arg1, u8 arg2) {
    struct206 *temp_v0 = &arg0->unk28;
    if ((arg2 == 0x38) && (temp_v0->unk0->unk14 == 1)) {
        temp_v0->unk70 = *arg1;
        temp_v0->unk4 = 300;
    }
}

typedef struct { f32 x, y, z; } Vec3f_1510558C;
typedef struct Game131F30Surface { s16 values[9]; } Game131F30Surface;
typedef struct Game131F30Color { s32 value; } Game131F30Color;
typedef struct Game131F30Factor { f32 value; } Game131F30Factor;
typedef struct Game131F30Descriptor {
    s32 count, spread;
    Vec3f_1510558C position, velocity;
    Game131F30Surface surface;
    u8 pad32[2];
    f32 field34, field38, field3C, field40, field44;
    s16 duration, durationRange;
    f32 field4C, field50, field54;
} Game131F30Descriptor;
typedef struct Game131F30Part {
    u8 pad0[0x30];
    Vec3f_1510558C position;
    u8 pad3C[0x24];
    Vec3f_1510558C velocity;
    Game131F30Surface surface;
    u8 pad7E[0xA];
    s32 flags;
} Game131F30Part;
typedef struct Game131F30Emitter {
    u8 pad0, group, pad2[0xA], mode, padD[0x103];
    Game131F30Part part;
} Game131F30Emitter;

s32 func_15102920(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32);
void func_151C329C(void *, u8, s32);
void func_15151D6C(Game131F30Descriptor *, Game131F30Color *, Game131F30Factor *, s32, u8, s32);
extern Game131F30Color D_800A23A4;
extern Game131F30Factor D_800A23A8;
extern f32 D_800A23C0, D_800A23C4, D_800A23C8, D_800A23CC, D_800A23D0;
extern f32 D_800A23D4, D_800A23D8, D_800A23DC, D_800A23E0, D_800A23E4, D_800A23E8;

void func_1510558C(Game131F30Emitter *arg0) {
    Game131F30Part *part;
    u32 flags;
    struct {
        Game131F30Factor factor;
        Game131F30Color color;
        Game131F30Descriptor descriptor;
    } work;
    u32 lifetime;
    u32 random;
    f32 fraction;

    part = (Game131F30Part *)((u8 *)arg0 + 0x110);
    fraction = func_150ADA68();
    random = func_150ADA20();
    lifetime = func_150ADA20() % 81U + 0x32;
    func_15102920(fraction * 25.0f + 15.0f, ((random % 56U) + 0xC8) & 0xFF,
                  &part->surface, &part->position, lifetime, 1, 1,
                  part->flags, arg0->mode, arg0->group);
    flags = part->flags;
    if ((flags & 0x1F) == 9) {
        if (func_150ADA68() < D_800A23C0) {
            func_151C329C(&part->position, arg0->mode, arg0->group);
        }
    } else {
        if (func_150ADA68() < D_800A23C4) {
            func_151C329C(&part->position, arg0->mode, arg0->group);
        }
        if (func_150ADA68() < D_800A23C8) {
            work.color = D_800A23A4;
            work.factor = D_800A23A8;
            work.descriptor.count = 1;
            work.descriptor.spread = 3;
            work.descriptor.position = part->position;
            work.descriptor.velocity.x = -part->velocity.x;
            work.descriptor.velocity.y = -part->velocity.y;
            work.descriptor.velocity.z = -part->velocity.z;
            work.descriptor.surface = part->surface;
            work.descriptor.field34 = D_800A23CC;
            work.descriptor.field38 = D_800A23D0;
            work.descriptor.duration = 15;
            work.descriptor.durationRange = 15;
            work.descriptor.field3C = D_800A23D4;
            work.descriptor.field40 = D_800A23D8;
            work.descriptor.field44 = D_800A23DC;
            work.descriptor.field4C = D_800A23E0;
            work.descriptor.field50 = D_800A23E4;
            work.descriptor.field54 = D_800A23E8;
            func_15151D6C(&work.descriptor, &work.color, &work.factor, 1, arg0->mode, arg0->group);
        }
    }
}

void func_15105848(struct207 *arg0, s32 arg1, u8 arg2) {
    struct206 *temp_v0;

    if (arg2 == 0x38) {
        temp_v0 = &arg0->unk28;
        func_151058B4(arg0);
        temp_v0->unkC |= 1;
    } else {
        temp_v0 = &arg0->unk28;
        if (arg2 == 0x39) {
            temp_v0->unkC &= 0xFFFE;
        }
    }
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} Func151058B4Target;

typedef struct {
    Func151058B4Target *target;
    f32 rate;
    s16 *unk8;
} Func151058B4Data;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    s8 unk14;
    u8 unk15;
} Func151058B4Spawn;

typedef struct {
    f32 unk0;
    f32 vel[3];
    f32 unk10;
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
    s32 unk24;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    f32 unk34;
    s8 unk38;
    u8 unk39;
} Func151058B4Particle;

extern void func_151432BC(Func151058B4Target *, f32 *, f32 *, f32 *, f32 *);
extern void func_15143794(s16, s16, f32, f32 *);
extern void func_1515C2F0(Func151058B4Spawn *, s32, Func151058B4Particle *, s32, s32, s32);

void func_151058B4(struct131 *arg0) {
    Func151058B4Data *data;
    f32 count;
    u32 angle;
    Func151058B4Spawn spawn;
    Func151058B4Particle part;
    f32 sp8C;
    f32 sp88;

    if (func_151464B8(((Func151058B4Data *)((u8 *)arg0 + 0x28))->unk8) == 0) {
        data = (Func151058B4Data *)((u8 *)arg0 + 0x28);
        count = (func_150ADA68() * 0.0013f + 0.001899999916f) * data->rate;
        if (count > 1.0f) {
            spawn.unkE = 0x15;
            spawn.unk10 = 0xA;
            spawn.unk14 = -1;
            part.unk14 = 4;
            part.unk15 = 2;
            part.unk16 = 3;
            part.unk17 = 0x61;
            part.unk18 = 0xF2;
            part.unk19 = 0xFF;
            part.unk1B = 0xFF;
            part.unk1C = 0xFF;
            part.unk1D = 0xFF;
            part.unk1E = 0xFF;
            part.unk1F = 0xFF;
            part.unk20 = 3;
            part.unk21 = 0x24;
            part.unk24 = 0x200005;
            part.unk28 = 0x60600;
            part.unk2C = 0x14;
            part.unk2E = 0xC;
            part.unk30 = 1;
            part.unk32 = 0;
            part.unk34 = 1.0f;
            part.unk38 = -1;
            part.unk39 = 0;
            spawn.y = data->target->y;
            do {
                part.unk1A = func_150ADA20() % 101U + 155;
                spawn.unk15 = (func_150ADA20() & 3) + 3;
                func_151432BC(data->target, &spawn.x, &spawn.z, &sp8C, &sp88);
                spawn.unkC = func_150ADA20() % 31U + 30;
                part.unk0 = func_150ADA68() * 10.13f + 7.399999619f;
                angle = func_150ADA20();
                func_15143794(angle & 0xFF, func_150ADA20() % 22U - 54, func_150ADA68() * 20.0f + 30.0f, part.vel);
                part.unk10 = func_150ADA68() * 1.486000061f + -3.048f;
                func_1515C2F0(&spawn, 0, &part, 0, ((u8 *)arg0)[0xC], ((u8 *)arg0)[1]);
                count -= 1.0f;
            } while (count > 1.0f);
        }
    }
}
// void func_151058B4(void *arg0) {
//     s8 spE1;
//     s8 spE0;
//     ?32 spDC;
//     s16 spDA;
//     s16 spD8;
//     ? spD4;
//     f32 spD0;
//     ? spCC;
//     s8 spC9;
//     s8 spC8;
//     f32 spC4;
//     s8 spC2;
//     s16 spC0;
//     s16 spBE;
//     s16 spBC;
//     ?32 spB8;
//     ?32 spB4;
//     s8 spB1;
//     s8 spB0;
//     s8 spAF;
//     s8 spAE;
//     s8 spAD;
//     s8 spAC;
//     s8 spAB;
//     s8 spAA;
//     s8 spA9;
//     s8 spA8;
//     s8 spA7;
//     s8 spA6;
//     s8 spA5;
//     s8 spA4;
//     f32 spA0;
//     ? sp94;
//     f32 sp90;
//     ? sp8C;
//     ? sp88;
//     f32 temp_f20;
//     f32 temp_f20_2;
//     f32 temp_f24;
//     f32 temp_f26;
//     s32 temp_s0;
//     u32 temp_s1;
//     void *temp_s4;
//     f32 phi_f20;
//
//     if (func_151464B8(arg0->unk30) == 0) {
//         temp_s4 = arg0 + 0x28;
//         temp_f20 = ((func_150ADA68() * *(void *)0x800A23EC) + *(void *)0x800A23F0) * temp_s4->unk4;
//         if (temp_f20 > 1.0f) {
//             spDA = 0x15;
//             spDC = 0xA;
//             spA7 = 0x61;
//             spA8 = 0xF2;
//             spE0 = -1;
//             spA4 = 4;
//             spA5 = 2;
//             spA6 = 3;
//             spA9 = 0xFF;
//             spAB = 0xFF;
//             spAC = 0xFF;
//             spAD = 0xFF;
//             spAE = 0xFF;
//             spAF = 0xFF;
//             spB0 = 3;
//             spB1 = 0x24;
//             spB4 = 0x200005;
//             spB8 = 0x60600;
//             spBC = 0x14;
//             spBE = 0xC;
//             spC0 = 1;
//             spC2 = 0;
//             spC4 = 1.0f;
//             spC8 = -1;
//             spC9 = 0;
//             temp_f26 = *(void *)0x800A23F4;
//             temp_f24 = *(void *)0x800A23F8;
//             spD0 = (f32) temp_s4->unk0->unk2;
//             phi_f20 = temp_f20;
// loop_3:
//             spAA = (func_150ADA20() % 0x65U) + 0x9B;
//             spE1 = (func_150ADA20() & 3) + 3;
//             func_151432BC(temp_s4->unk0, &spCC, &spD4, &sp8C, &sp88);
//             spD8 = (func_150ADA20() % 0x1FU) + 0x1E;
//             sp90 = (func_150ADA68() * temp_f24) + temp_f26;
//             temp_s0 = func_150ADA20();
//             temp_s1 = func_150ADA20();
//             func_15143794((s16) (temp_s0 & 0xFF), (s16) ((temp_s1 % 0x16U) - 0x36), (func_150ADA68() * 20.0f) + 30.0f, &sp94);
//             spA0 = (func_150ADA68() * *(void *)0x800A23FC) + *(void *)0x800A2400;
//             func_1515C2F0(&spCC, 0, &sp90, 0, (?32) arg0->unkC, (?32) arg0->unk1);
//             temp_f20_2 = phi_f20 - 1.0f;
//             phi_f20 = temp_f20_2;
//             if (temp_f20_2 > 1.0f) {
//                 goto loop_3;
//             }
//         }
//     }
// }

void func_15105BC8(struct204 *arg0) {
    if ((arg0->unk34 & 1) != 0) {
        func_1508B20C(arg0->unk28->unk0, arg0->unk28->unk2, arg0->unk28->unk4, 500.0f);
    }
}

Func15105C24Obj *func_15105C24(s32 arg0)
{
  u8 i;
  u8 j;
  Func15105C24Obj *obj;
 i = 0; do {
    j = 0;
    do
    {
      obj = (Func15105C24Obj *) D_800DCE50[j][(&D_800A5770)[i]];
      if (obj != 0)
      {
        do
        {
          if ((obj->type == 0x2E) && (arg0 == obj->owner))
          {
            return obj;
          }
          obj = obj->next;
        }
        while (obj != 0);
      }
      next_j:
      j++;

    }
    while (j < 2);
    next_i:
    i++;

  }
  while (i < 2);
  return 0;
}
// void *func_15105C24(s32 arg0) {
//     s32 temp_t4;
//     s32 temp_t5;
//     void *temp_a0;
//     void *temp_a0_2;
//     s32 phi_v1;
//     s32 phi_v0;
//     void *phi_a0;
//
//     phi_v0 = 0;
// loop_1:
//     phi_v1 = 0;
// loop_2:
//     temp_a0 = *((((((phi_v1 * 4) - phi_v1) * 4) + phi_v1) << 5) + 0x800DCE50 + (((phi_v0 * 4) + 0x800A0000)->unk5770 * 4));
//     phi_a0 = temp_a0;
//     if (temp_a0 != 0) {
// loop_3:
//         if ((phi_a0->unk13 == 0x2E) && (arg0 == phi_a0->unk28)) {
//             return phi_a0;
//         }
//         temp_a0_2 = phi_a0->unk8;
//         phi_a0 = temp_a0_2;
//         if (temp_a0_2 != 0) {
//             goto loop_3;
//         }
//     }
//     temp_t4 = (phi_v1 + 1) & 0xFF;
//     phi_v1 = temp_t4;
//     if (temp_t4 < 2) {
//         goto loop_2;
//     }
//     temp_t5 = (phi_v0 + 1) & 0xFF;
//     phi_v0 = temp_t5;
//     if (temp_t5 < 2) {
//         goto loop_1;
//     }
//     return NULL;
// }
