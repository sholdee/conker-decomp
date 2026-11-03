#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *func_151CD4C0();
void func_151CE47C();

struct Vec3w151CEC10;

extern f32 D_800AB020;
extern f32 D_800AB024;
extern f32 D_800AB008;
extern f32 D_800D9860;
extern f32 func_15144AA8(s32);
extern struct17 *func_15144B34(s32);
extern s32 func_15046C80(f32 *, s32, f32, f32 *);
extern struct260 *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
s32 func_151CEC10(struct Vec3w151CEC10 *, f32 *, void *);

typedef struct {
    char pad_0x0[0x48];
    s32 field_0x48;
} Func151CE4DCTemp;

typedef struct {
    char pad_0x0[0x98];
    s32 field_0x98;
} Func151CE4DCArg0;

typedef struct {
    char pad_0x0[0x30];
    s32 field_0x30;
} ConkerField30Object;

typedef struct {
    ConkerField30Object *field_0x0;
} ConkerField0Slot;

typedef struct {
    char pad_0x0[0x98];
    ConkerField0Slot *field_0x98;
} ConkerField98Owner;


struct260 *func_151CD2C0(void *arg0, u8 arg1, s32 arg2) {
    struct260 *temp_v0;
    struct {
        void *unk0;
        s32 unk4;
        s32 unk8;
        u8 unkC;
        u8 padD;
        u8 padE;
        u8 padF;
    } sp34;

    sp34.unk0 = arg0;
    sp34.unk4 = *(s32 *)((s32)arg0 + 0x18);
    sp34.unk8 = 0;
    sp34.unkC = *(s32 *)((s32)arg0 + 0x1C);

    temp_v0 = func_15149130(0x12C, -1, -1, -1, 0, 0x1E, (struct37 *)0x10, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x10);
    }
    return temp_v0;
}

void func_151CD35C(s32 arg0) {
    s32 sp1C;

    if (arg0 >= 0 && arg0 < 4) {
        sp1C = arg0;
        func_151494E0((s32)&sp1C, 0x17);
    }
}

void func_151CD394(s32 arg0) {
    s32 sp1C;

    if (arg0 >= 0 && arg0 < 4) {
        sp1C = arg0;
        func_151494E0((s32)&sp1C, 0x18);
    }
}

typedef struct {
    void *unk0;
    s32 unk4;
    void *unk8;
    u8 unkC;
} Sub151CD3CC;

void func_151CD3CC(struct260 *arg0, s32 arg1, u8 arg2) {
    Sub151CD3CC *sub;
    /* Reserves 12 bytes at the top of the -g3 stack frame that the shipped
     * build allocated but never referenced. */
    s32 unused[3];

    sub = (Sub151CD3CC *)((s32)arg0 + 0x28);
    if (arg2 == 0x17) {
        if (sub->unk4 == *(s32 *)arg1) {
            if (sub->unk8 == NULL) {
                sub->unk8 = func_151CD4C0(sub->unk0, sub->unkC, sub->unk4, arg0->pad0[0xC], arg0->pad0[1]);
                *(u8 *)((s32)sub->unk0 + 0x14) = 0;
            dummy_label_151CD3CC_1: ;
            }
        }
    } else if (arg2 == 0x18) {
        if (sub->unk4 == *(s32 *)arg1) {
            if (sub->unk8 != NULL) {
                func_151CE47C(sub->unk8);
            }
        }
    } else if (arg2 == 0x23) {
        if (sub->unk4 == *(s32 *)arg1) {
            sub->unk8 = NULL;
            *(u8 *)((s32)sub->unk0 + 0x14) = 1;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CD4C0.s")

typedef struct Game1FA770Particle {
    f32 x;
    f32 y;
    u8 pad08[4];
    f32 velocity;
    u8 pad10[0xC];
} Game1FA770Particle;

typedef struct Vec3i151CD674 {
    s32 x;
    s32 y;
    s32 z;
} Vec3i151CD674;

s32 func_151CD674(u8 *actor) {
    u8 *owner;
    Game1FA770Particle *particles;
    Game1FA770Particle *particle;
    s32 cursor;
    f32 velocity;
    f32 gravity;
    f32 acceleration;
    f32 step;
    f32 travel;
    f32 squared;

    owner = *(u8 **)(actor + 0x98);
    particles = *(Game1FA770Particle **)(actor + 0x94);
    if ((*(s8 *)(actor + 0x2C) < 2) && (*(u16 *)(actor + 0x1E) & 8)) {
        return 0;
    }
    cursor = *(s8 *)(actor + 0x2E);
    if (cursor != *(s8 *)(actor + 0x2D)) {
        gravity = -0.25500003f;
        acceleration = -0.12750001f;
        do {
            cursor--;
            if (cursor < 0) {
                cursor = actor[0x25] - 1;
            }
            particle = &particles[cursor];
            velocity = particle->velocity;
            step = D_800BE9A4;
            travel = velocity * step;
            squared = step * step;
            particle->y = particle->y + (travel + acceleration * squared);
            particle->velocity = (f32)velocity + gravity * D_800BE9A4;
            if (particle->y < *(f32 *)(owner + 0x44)) {
                if (cursor != *(s8 *)(actor + 0x2D)) {
                    do {
                        *(s8 *)(actor + 0x2D) += 1;
                        if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                            *(s8 *)(actor + 0x2D) = 0;
                        }
                        *(s8 *)(actor + 0x2C) -= 1;
                    } while (cursor != *(s8 *)(actor + 0x2D));
                }
                particles[*(s8 *)(actor + 0x2D)].y = *(f32 *)(owner + 0x44);
            }
        } while (cursor != *(s8 *)(actor + 0x2D));
    }
    *(Vec3i151CD674 *)(actor + 0x54) = *(Vec3i151CD674 *)(actor + 0x10);
    return 1;
}

typedef struct Vec2f151CD7BC { f32 x; f32 z; } Vec2f151CD7BC;

typedef struct Owner151CD7BC {
    f32 amp0;
    f32 amp1;
    f32 phase0;
    f32 phase1;
    f32 rate0;
    f32 rate1;
    Vec2f151CD7BC pos;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 phase2;
    f32 rate2;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Owner151CD7BC;

typedef struct Particle151CD7BC {
    f32 x;
    f32 y;
    f32 z;
    f32 vel;
    f32 unk10;
    s16 alpha;
    f32 unk18;
} Particle151CD7BC;

s32 func_151CD7BC(u8 *actor) {
    Owner151CD7BC *owner;
    Particle151CD7BC *particles;
    Particle151CD7BC *p;
    f32 h;
    f32 dh;
    f32 dz;
    f32 dx;
    Vec2f151CD7BC pos;
    f32 inv;
    f32 a;
    f32 da;
    f32 b;
    f32 vel;
    f32 db;
    f32 gravity;
    f32 acceleration;
    s32 tail;

    owner = *(Owner151CD7BC **)(actor + 0x98);
    particles = *(Particle151CD7BC **)(actor + 0x94);
    owner->phase0 += owner->rate0 * D_800BE9A4;
    owner->phase1 += owner->rate1 * D_800BE9A4;
    owner->phase0 = func_15144B68(owner->phase0);
    owner->phase1 = func_15144B68(owner->phase1);
    owner->phase2 += owner->rate2 * D_800BE9A4;
    owner->phase2 = func_15144B68(owner->phase2);
    owner->unk40 += 0.2730129957f * D_800BE9A4;
    owner->unk3C += (10220.0f + func_150ADA68() * 9434.0f) * 0.009999999776f * D_800BE9A4;
    while (owner->unk3C >= 16384.0f) {
        owner->unk3C -= 16384.0f;
    }
    if (owner->unk40 > 1.0f) {
        inv = 1.0f / owner->unk40;
        h = owner->unk20 + D_800BE9A4;
        dh = -(h * inv);
        pos = owner->pos;
        dx = (*(f32 *)(actor + 0x10) + (sinf(owner->phase0) * owner->amp0)) - owner->pos.x;
        dz = (*(f32 *)(actor + 0x18) + (sinf(owner->phase1) * owner->amp1)) - owner->pos.z;
        da = (owner->unk24 + (sinf(owner->phase2) * owner->unk28)) - owner->unk34;
        db = owner->unk3C - owner->unk38;
        a = owner->unk34;
        b = owner->unk3C;
        gravity = -0.255000025f;
        acceleration = -0.1275000125f;
        vel = -3.772000074f;
        while (owner->unk40 > 1.0f) {
            p = &particles[*(s8 *)(actor + 0x2E)];
            p->x = pos.x;
            p->y = *(f32 *)(actor + 0x14);
            p->z = pos.z;
            p->vel = vel;
            p->unk10 = a;
            p->alpha = 0xFF;
            p->unk18 = b;
            p->y = p->y + ((vel * D_800BE9A4) + (acceleration * (D_800BE9A4 * D_800BE9A4)));
            p->vel = (f32)vel + gravity * D_800BE9A4;
            *(s8 *)(actor + 0x2E) += 1;
            if (actor[0x25] == *(s8 *)(actor + 0x2E)) {
                *(s8 *)(actor + 0x2E) = 0;
            }
            tail = *(s8 *)(actor + 0x2D);
            *(s8 *)(actor + 0x2C) += 1;
            if (tail == *(s8 *)(actor + 0x2E)) {
                *(s8 *)(actor + 0x2D) = tail + 1;
                if (actor[0x25] == *(s8 *)(actor + 0x2D)) {
                    *(s8 *)(actor + 0x2D) = 0;
                }
                *(s8 *)(actor + 0x2C) -= 1;
            }
            pos.x += dx * inv;
            pos.z += dz * inv;
            a += da * inv;
            b += db * inv;
            h += dh;
            owner->unk40 -= 1.0f;
        }
        owner->pos = pos;
        owner->unk34 = a;
        owner->unk38 = b;
        owner->unk20 = h;
    }
    return 1;
}

typedef struct Game1FA770Trail {
    f32 x;
    f32 y;
    u8 pad08[0xC];
    s16 alpha;
    u8 pad16[0x6];
} Game1FA770Trail;

s32 func_151CDB94(u8 *actor) {
    Game1FA770Trail *trail;
    Game1FA770Trail *start;
    Game1FA770Trail *cur;
    s32 i;
    f32 range;
    f32 inv;
    f32 d;
    f32 fade;
    f32 k;

    trail = *(Game1FA770Trail **)(actor + 0x94);
    if (*(s8 *)(actor + 0x2C) <= 0) {
        return 1;
    }
    i = *(s8 *)(actor + 0x2E) - 1;
    if (i < 0) {
        i = actor[0x25] - 1;
    }
    start = &trail[*(s8 *)(actor + 0x2D)];
    cur = &trail[i];
    range = fabsf(cur->y - start->y);
    if (range == 0.0f) {
        return 1;
    }
    inv = 1.0f / range;
    fade = inv * 10.0f;
    i = *(s8 *)(actor + 0x2D);
    do {
        cur = &trail[i];
        d = cur->y - start->y;
        cur->alpha = 0xFF;
        if (d < range * 0.1970000118f) {
            k = inv * 5.076141834f;
            cur->alpha = (u8)(d * k * 255.0f);
        } else if (range - range * 0.1000000015f < d) {
            cur->alpha = (u8)((range - d) * fade * 255.0f);
        }
        i++;
        if (i >= actor[0x25]) {
            i = 0;
        }
    } while (i != *(s8 *)(actor + 0x2E));
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CDE20.s")

typedef struct {
    char pad0[0x1E];
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
} ConkerStruct151CE47C;

void func_151CE47C(ConkerStruct151CE47C *arg0) {
    arg0->unk30 = 0;
    arg0->unk1E &= ~0x2;
    arg0->unk1E |= 0x8;
}

extern void func_151478F4(void *);

void func_151CE49C(Func151CE4DCArg0 *arg0) {
    s32 sp1C;
    Func151CE4DCTemp *temp;

    temp = (Func151CE4DCTemp *)arg0->field_0x98;
    sp1C = temp->field_0x48;
    func_151494E0((s32)&sp1C, 0x23);
    func_151478F4((void *)arg0);
}

extern void func_15147928(s32);

void func_151CE4DC(Func151CE4DCArg0 *arg0) {
    s32 sp1C;
    Func151CE4DCTemp *temp;

    temp = (Func151CE4DCTemp *)arg0->field_0x98;
    sp1C = temp->field_0x48;
    func_151494E0((s32)&sp1C, 0x23);
    func_15147928((s32)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CE51C.s")

typedef struct Obj151CE634Sub {
    u8 pad0[0x48];
    s32 unk48;
} Obj151CE634Sub;

typedef struct Obj151CE634 {
    u8 pad0[8];
    struct Obj151CE634 *next;
    u8 padC[0x20 - 0xC];
    s32 unk20;
    u8 pad24[0x98 - 0x24];
    Obj151CE634Sub *unk98;
} Obj151CE634;

extern s32 D_800A5760[];
extern Obj151CE634 *D_800DCE50[][0x68];

Obj151CE634 *func_151CE634(s32 arg0) {
    u8 i;
    s32 j;
    Obj151CE634 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            p = D_800DCE50[j][D_800A5760[i]];
            while (p != NULL) {
                if (p->unk20 == 0xB) {
                    Obj151CE634Sub *sub = p->unk98;

                    if (sub->unk48 == arg0) {
                        return p;
                    }
                }
                p = p->next;
            }
        }
    }
    return NULL;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CE6D0.s")

s32 func_151CEA20(f32 *arg0) {
    f32 temp_f2;
    f32 temp_f12;

    temp_f2 = arg0[0x11];
    temp_f12 = arg0[0x10];
    arg0[0x11] += temp_f12 * D_800BE9A4;
    *(volatile f32 *)&arg0[0xE] = arg0[0xE] + ((temp_f2 + ((0.5f * temp_f12) * D_800BE9A4)) * D_800BE9A4);
    *(f32 *)&arg0[0x14] = ((*(volatile f32 *)&arg0[0x13]) * D_800BE9A4) + (*(volatile f32 *)&arg0[0x14]);
    if (arg0[0x14] > 1.0f) {
        arg0[0x14] = 1.0f;
    }
    return 1;
}

struct260 *func_151CEAAC(void *arg0, void *arg1, u8 arg2, u8 arg3, s32 arg4) {
    struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        s16 unkC;
        s16 unkE;
        s32 unk10;
        u8 unk14;
        u8 unk15;
        u8 pad16[2];
        s32 unk18;
    } sp6C;
    struct {
        void *unk0;
        u8 unk4;
        u8 pad5[3];
        void *unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        u8 unk1C;
        u8 pad1D[3];
        f32 unk20;
        f32 unk24;
    } sp44;
    struct260 *temp_v0;
    s32 flag;
    f32 temp_f;

    sp6C.unk15 = 0x19;
    sp6C.unkC = 0x12C;
    sp6C.unkE = 0x10;
    sp6C.unk10 = 0x11;
    sp6C.unk14 = 3;

    sp44.unk0 = arg0;
    if (arg0 != NULL) {
        sp44.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    } else {
        sp44.unk4 = 0;
    }

    sp44.unk10 = 1.0f;
    sp44.unk18 = 1.0f;
    sp44.unk8 = arg1;
    sp44.unkC = 0.0f;
    sp44.unk14 = 0.0f;
    if (arg2 != 0) {
        flag = 2;
    } else {
        flag = 0;
    }
    sp44.unk1C = flag;

    sp44.unk20 = (func_150ADA68() * 400.0f) + 400.0f;
    temp_f = func_150ADA68();
    sp44.unk24 = (temp_f + temp_f) * D_800AB008;

    if (func_151CEC10((struct Vec3w151CEC10 *)&sp6C, (f32 *)arg0, arg1) == 0) {
        sp6C.unk0 = 0.0f;
        sp6C.unk4 = 0.0f;
        sp6C.unk8 = 0.0f;
    }

    temp_v0 = func_15147A80(&sp6C, 0x28, 0x28, 0, 0xF, 0xF, 0, 0, 0, arg3, arg4);
    if (temp_v0 != NULL) {
        memcpy((void *)*(s32 *)((s32)temp_v0 + 0x98), &sp44, 0x28);
    }
    return temp_v0;
}

struct Vec3w151CEC10 { s32 x, y, z; };

s32 func_151CEC10(struct Vec3w151CEC10 *arg0, f32 *arg1, void *arg2) {
    if (arg2 != NULL) {
        *arg0 = *(struct Vec3w151CEC10 *)((s32)arg2 + 0x40);
    } else {
        *(f32 *)&arg0->x = arg1[5];
        *(f32 *)&arg0->y = arg1[6];
        *(f32 *)&arg0->z = arg1[7];
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CEC54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CF120.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CF380.s")

struct s151CF844 { s32 pad[0x26]; s32 *unk98p; };

void func_151CF844(struct s151CF844 *arg0, s32 arg1, u8 arg2) {
    s32 *temp = arg0->unk98p;
    if (temp[0] != 0) {
        func_15169850(arg1, arg2, (s32)temp, (s32)temp + 4, (s32)arg0);
    }
}

typedef struct {
    char pad0[0x23D];
    u8 unk23D;
} ConkerStruct151CF898Camera;

typedef struct {
    struct17 vec;
    f32 unkC;
} ConkerStruct151CF898Payload;

void func_151CF898(struct127 *arg0, f32 arg1, f32 arg2) {
    ConkerStruct151CF898Camera *camera;
    struct17 sp58;
    struct17 *pos;
    f32 yaw;
    f32 angle;
    f32 distance;
    ConkerStruct151CF898Payload sp38;
    struct260 *temp_v0;

    camera = (ConkerStruct151CF898Camera *)arg0->camera;
    if (camera != NULL) {
        if (func_150ADA68() < D_800AB020) {
            pos = func_15144B34(camera->unk23D);
            yaw = func_15144AA8(camera->unk23D);
            angle = ((func_150ADA68() * 80.0f) + (yaw - 40.0f)) * D_800AB024;
            distance = func_150ADA68() * 2000.0f;

            sp58.unk0 = pos->unk0 - (sinf(angle) * distance);
            sp58.unk4 = arg1;
            sp58.unk8 = pos->unk8 - (cosf(angle) * distance);

            if (func_15046C80((f32 *)&sp58, 0, arg2, &D_800D9860) != 0) {
                sp58.unk4 = D_800D9860;
                sp38.vec = sp58;
                sp38.unkC = 0.0f;

                temp_v0 = func_15149130((s16)((func_150ADA20() % 0x83U) + 0x33),
                                        -1, 0x20, -1, 1, 0, (struct37 *)0x10, 0xFF, 1);
                if (temp_v0 != NULL) {
                    memcpy((u8 *)temp_v0 + 0x28, &sp38, 0x10);
                }
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151CFA4C.s")

void func_151D0024(struct127 *arg0) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp18;

    sp18.unk0 = (s32)arg0;
    sp18.unk4 = arg0->unique_id;
    func_151494E0((s32)&sp18, 0x18);
}

void func_151D0058(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5;
        u16 unk6;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE;
        u8 padF;
    } sp3C;

    sp3C.unk0 = arg0;
    sp3C.unk4 = arg0->unique_id;
    sp3C.unk8 = (arg0->xz_scale + arg0->y_scale) * 0.5f;
    sp3C.unkD = arg1;
    sp3C.unkC = (func_150ADA20() % 0x38U) + 0xC8;
    sp3C.unk6 = arg0->unk84.uh;

    temp_v0 = func_15149130(0x12C, -1, 0x61, 4, 0, 0x31, (struct37 *)0x10, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp3C, 0x10);
    }
}

typedef struct {
    char pad84[0x84];
    u16 unk84;
} ConkerStruct151D0128b;

typedef struct {
    char padE[0xE];
    s16 unkE;
    char pad10[0x18];
    ConkerStruct151D0128b *unk28;
    char pad2C[0x2];
    u16 unk2E;
} ConkerStruct151D0128;

void func_151D0128(ConkerStruct151D0128 *arg0) {
    ConkerStruct151D0128b *p = arg0->unk28;
    if (arg0->unk2E != p->unk84) {
        arg0->unkE = -1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151D014C.s")

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151D08F0;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151D08F0;

void func_151D08F0(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151D08F0 *temp_v0 = (SubA151D08F0 *)((u8 *)arg0 + 0x28);
    ArgB151D08F0 *b = (ArgB151D08F0 *)arg1;
    s32 b0;

    if (arg2 == 0 || arg2 == 0x18) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
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

typedef struct {
    u8 unk0;
    struct17 unk4;
    struct17 unk10;
    struct17 unk1C;
    f32 unk28;
} ShapeA151D09A8; /* 0x2C */

typedef struct {
    u8 unk0[0x24];
} Dst151D09A8;

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
    u8 pad26[2];
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 pad32[2];
    f32 unk34;
    u8 unk38;
    u8 pad39[3];
    f32 unk3C;
} EmitA151D09A8; /* 0x40 */

typedef struct {
    struct17 unk0;
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
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    s16 unk3C;
    s16 unk3E;
    s32 unk40;
} EmitB151D09A8; /* 0x44 */

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
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
    f32 unk28;
    f32 unk2C;
    struct17 pos;
    struct17 unk3C;
    struct17 vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Particle151D09A8; /* 0x70 */

extern f32 D_800AAF84[3];
extern f32 D_800AAF90[3];
extern void func_15145EA4(s32 *, s32 *, s32, s32);
extern void func_1504715C(void *, struct127 *);
extern void func_1514FF44(ShapeA151D09A8 *, EmitA151D09A8 *, Dst151D09A8 *, u8, s32);
extern void func_15153CCC(ShapeA151D09A8 *, EmitB151D09A8 *, Dst151D09A8 *, u8, s32);
extern s32 func_1514F6E8(ShapeA151D09A8 *);
extern void func_1514F808(ShapeA151D09A8 *, f32, struct17 *);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151D09A8(struct127 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;
    struct17 dir;
    Dst151D09A8 dst;
    ShapeA151D09A8 a;

    if (arg0->unk1D4 != NULL) {
        {
            s32 srcs[2];
            s32 dsts[2];

            srcs[0] = (s32)D_800AAF84;
            srcs[1] = (s32)D_800AAF90;
            dsts[0] = (s32)&pos;
            dsts[1] = (s32)&dir;
            func_15145EA4(srcs, dsts, (s32)arg0->unk1D4 + 0x80, 2);
            dir.unk0 -= pos.unk0;
            dir.unk4 -= pos.unk4;
            dir.unk8 -= pos.unk8;
        }
        func_1504715C(&dst, arg0);
        a.unk0 = 0;
        a.unk4 = dir;
        a.unk10 = a.unk1C = *(struct17 *)&D_800A5480;
        {
            EmitA151D09A8 emitA;

            emitA.unk0 = pos;
            emitA.unkC = 6;
            emitA.unkE = 8;
            emitA.unk10 = 0.03999999911f;
            emitA.unk14 = 0.02999999933f;
            emitA.unk18 = 0x1E;
            emitA.unk1A = 0x1E;
            emitA.unk1C = -1.467000127f;
            emitA.unk20 = 0.756000042f;
            emitA.unk24 = 0x9B;
            emitA.unk25 = 0x64;
            emitA.unk28 = 403.0f;
            emitA.unk2C = 455.0f;
            emitA.unk30 = 1;
            emitA.unk31 = 2;
            emitA.unk34 = 0.0f;
            emitA.unk38 = 1;
            emitA.unk3C = 0.0f;
            a.unk28 = 750.0f;
            func_1514FF44(&a, &emitA, &dst, arg1, arg2);
        }
        {
            EmitB151D09A8 emitB;

            emitB.unk0 = pos;
            emitB.unkC = 10.0f;
            emitB.unk10 = 25.0f;
            emitB.unk14 = 1.253000021f;
            emitB.unk18 = 0.8230000138f;
            emitB.unk1C = 0.03500000015f;
            emitB.unk20 = 0.03199999779f;
            emitB.unk24 = 0xF;
            emitB.unk26 = 8;
            emitB.unk28 = 3;
            emitB.unk2A = 2;
            emitB.unk2C = 0x1E;
            emitB.unk2E = 0x19;
            emitB.unk30 = 0x9B;
            emitB.unk32 = 0x64;
            emitB.unk38 = 0.0f;
            emitB.unk3C = 0x10;
            emitB.unk3E = 0xF;
            emitB.unk40 = 0;
            emitB.unk34 = 2;
            a.unk28 = 750.0f;
            func_15153CCC(&a, &emitB, &dst, arg1, arg2);
        }
        if (func_1514F6E8(&a) != 0) {
            s32 count;
            Particle151D09A8 spawn;
            void *ret;
            s32 pad;

            a.unk28 = 1000.0f;
            count = (func_150ADA20() % 6U) + 5;
            spawn.unk1D = 0x3E;
            spawn.unk8 = 0x2203;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk14 = 0;
            spawn.unk15 = 0;
            spawn.unk16 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk18 = 0;
            spawn.unk19 = 0;
            spawn.unk1A = 0;
            spawn.unk1B = 0;
            spawn.unk1C = 0xFF;
            spawn.pos = pos;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.unk22 = 1;
            spawn.unk58 = 0xC007;
            spawn.unk24 = 1.0f;
            spawn.unkA = (func_150ADA20() % 11U) + 0x28;
            spawn.unk1E = 1;
            spawn.unk20 = 0xFF;
            spawn.unk60 = 6;
            spawn.unk61 = 8;
            spawn.unk64 = -1;
            spawn.unk62 = -1;
            spawn.unk63 = -1;
            spawn.unk65 = 0xB;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 0;
            spawn.unk2C = 0.0f;
            spawn.unk28 = 0.0f;
            spawn.unk6C = 10000.0f;
            while (count != 0) {
                func_1514F808(&a, (func_150ADA68() * 0.03999999911f) + 0.04999999702f, &spawn.vel);
                spawn.unk54 = (func_150ADA68() * 1.340000033f) + -2.328999996f;
                ret = func_15130280(&spawn, 1, 0, 4, arg1, arg2);
                if (ret != NULL) {
                    *(struct260 **)((u8 *)ret + 0xA8) = func_151CEAAC(NULL, ret, 0, arg1, arg2);
                }
                count--;
            }
        }
    }
}

void func_151D0ED8(void *arg0) {
    if (*(struct102 **)((u8 *)arg0 + 0xA8) != 0) {
        func_1516972C(*(struct102 *volatile *)((u8 *)arg0 + 0xA8));
    }
}

void func_151D0ED8(void *arg0);

void func_1513173C(void *arg0);

void func_151D0F08(void *arg0) {
    func_151D0ED8(arg0);
    func_1513173C(arg0);
}

void func_1513175C(void *arg0);

void func_151D0F34(void *arg0) {
    func_151D0ED8(arg0);
    func_1513175C(arg0);
}

extern s32 (*D_8008FC30[])(struct127 *, void *);

struct vec3_151D0F60 { f32 x; f32 y; f32 z; };

void func_151D0F60(struct127 *arg0, u8 arg1, u8 arg2, s32 arg3) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        struct vec3_151D0F60 unkC;
        struct vec3_151D0F60 unk18;
        struct vec3_151D0F60 unk24;
        f32 unk30;
        u8 unk34;
    } sp34;

    sp34.unk0 = arg0;
    sp34.unk4 = arg0->unique_id;
    sp34.unk8 = 0;

    if (D_8008FC30[arg1](arg0, &sp34.unk18) == 0) {
        sp34.unk18 = *(struct vec3_151D0F60 *)&D_800A5480;
    }

    sp34.unkC = sp34.unk18;
    sp34.unk24 = *(struct vec3_151D0F60 *)&D_800A5480;
    sp34.unk30 = 0.0f;
    sp34.unk34 = arg1;

    temp_v0 = func_15149130(0x12C, -1, 0x5F, -1, 0, 0x48, (struct37 *)0x38, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x38);
    }
}

s32 func_151D1074(f32 *arg0, f32 *arg1) {
    arg1[0] = arg0[5];
    arg1[1] = arg0[96] + 8.0f;
    arg1[2] = arg0[7];
    return 1;
}

s32 func_151D10E4(void *arg0, s32 arg1, u8 arg2);

s32 func_151D10A4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 0);
}

s32 func_151D10C4(void *arg0, s32 arg1) {
    return func_151D10E4(arg0, arg1, 1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151D10E4.s")

typedef struct {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    s32 unk8;
    struct17 unkC;
    struct17 unk18;
    struct17 unk24;
    f32 unk30;
    u8 unk34;
} Sub151D1138;

extern f32 func_15143E64(struct17 *);
extern s32 func_1510F8CC(s32);
void func_151D13E0(struct260 *arg0);
void func_151D1448(struct260 *arg0);

void func_151D1138(struct260 *arg0) {
    Sub151D1138 *sub;
    struct127 *obj;
    u8 finished = 0;
    s32 state;

    sub = (Sub151D1138 *)((u8 *)arg0 + 0x28);
    obj = sub->unk0;

    if (obj->interaction_state == 0 || obj->unique_id != sub->unk4) {
        arg0->unkE = -1;
        return;
    }

    sub->unkC = sub->unk18;
    if (D_8008FC30[sub->unk34](obj, &sub->unk18) == 0) {
        sub->unk18 = sub->unkC;
        finished = 1;
    }

    sub->unk24.unk0 = sub->unk18.unk0 - sub->unkC.unk0;
    sub->unk24.unk4 = sub->unk18.unk4 - sub->unkC.unk4;
    sub->unk24.unk8 = sub->unk18.unk8 - sub->unkC.unk8;
    sub->unk30 = func_15143E64(&sub->unk24);

    state = func_1510F8CC(obj->unk184);
    if (sub->unk30 > 300.0f || sub->unk30 <= 0.0f || obj->unk28 != 0.0f || obj->in_water != 0 ||
        state == 5 || state == 6 || state == 9 || state == 0xD || state == 0xE) {
        finished = 1;
    }

    if (finished != 0) {
        func_151D13E0(arg0);
    } else if (sub->unk8 == 0) {
        func_151D1448(arg0);
    }
}

void func_151D1328(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_151D13E0(struct260 *arg0);

void func_151D1368(struct260 *arg0) {
    func_151D13E0(arg0);
}

void func_151D1388(struct260 *arg0) {
    func_151D1368(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_151D13B4(struct260 *arg0) {
    func_151D1368(arg0);
    func_15149368(arg0);
}

struct Obj151D13E0 {
    char pad0[0x1C];
    s16 unk1C;
    u16 unk1E;
    char pad20[0x10];
    u8 unk30;
    char pad31[0x67];
    s32 *unk98;
};

struct Slot151D13E0 {
    char pad0[0x8];
    struct Obj151D13E0 *unk8;
};

void func_151D13E0(struct260 *arg0) {
    struct Slot151D13E0 *slot = (struct Slot151D13E0 *)((char *)arg0 + 0x28);
    struct Obj151D13E0 *obj;
    s32 *p;

    if (slot->unk8 != 0) {
        p = slot->unk8->unk98;
        slot->unk8->unk30 = 0;
        slot->unk8->unk1E &= ~0x2;
        slot->unk8->unk1E |= 0x8;
        slot->unk8->unk1E |= 0x1;
        slot->unk8->unk1C = 0x28;
        *p = 0;
        slot->unk8 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151D1448.s")

typedef struct {
    f32 x, y, z;
} Vec3_151D15D0;

typedef struct {
    f32 x, z;
} Vec2_151D15D0;

typedef struct {
    Vec3_151D15D0 pos;
    f32 speed;
    f32 invLen;
    u8 alpha;
    u8 pad15[3];
    f32 unk18;
    f32 unk1C;
    Vec2_151D15D0 n;
} Rec_151D15D0;

typedef struct {
    u8 pad0[0x18];
    Vec3_151D15D0 pos;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 speed;
} Inner_151D15D0;

typedef struct {
    u8 pad0[0x28];
    Inner_151D15D0 inner;
} Base_151D15D0;

typedef struct {
    Base_151D15D0 *base;
    f32 angle;
} Src_151D15D0;

typedef struct {
    u8 pad0[0x10];
    Vec3_151D15D0 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 count;
    u8 pad26[6];
    s8 n;
    s8 tail;
    s8 head;
    u8 pad2F[0x94 - 0x2F];
    Rec_151D15D0 *recs;
    Src_151D15D0 *src;
} Obj_151D15D0;

extern f32 func_15144528(f32, f32, f32);

s32 func_151D15D0(Obj_151D15D0 *arg0) {
    Src_151D15D0 *src;
    Rec_151D15D0 *recs;
    Base_151D15D0 *base;
    Inner_151D15D0 *inner;

    src = arg0->src;
    recs = arg0->recs;
    base = src->base;
    inner = &base->inner;
    if (D_800BE9E4 > 0) {
        arg0->pos = inner->pos;
        arg0->flags |= 4;
        {
        Rec_151D15D0 *rec;
        f32 len;
        s16 i0;
        s16 i1;
        s16 i2;
        f32 dx0;
        f32 dz0;
        f32 inv1;
        Vec2_151D15D0 d1;
        Rec_151D15D0 *p1;
        Rec_151D15D0 *p2;
        Vec2_151D15D0 nrm;
        f32 lenSq;

        rec = &recs[arg0->head];
        rec->pos = arg0->pos;
        rec->speed = inner->speed;
        len = sqrtf(inner->vx * inner->vx + inner->vz * inner->vz);
        if (len == 0.0f) {
            rec->invLen = 1.0f;
        } else {
            rec->invLen = 1.0f / len;
        }
        rec->unk1C = 100.0f;
        rec->unk18 = src->angle;
        rec->alpha = 0xFF;
        src->angle = src->angle + rec->speed * 21.85000038f;
        src->angle = func_15144528(src->angle, 8192.0f, 0.0f);
        if (len) {
            rec->n.x = -inner->vz * rec->invLen;
            rec->n.z = inner->vx * rec->invLen;
        } else {
            rec->n.x = 0.0f;
            rec->n.z = 0.0f;
        }
        arg0->head++;
        if (arg0->head == arg0->count) {
            arg0->head = 0;
        }
        arg0->n++;
        if (arg0->head == arg0->tail) {
            arg0->tail++;
            if (arg0->tail == arg0->count) {
                arg0->tail = 0;
            }
            arg0->n--;
        }
        if (arg0->n >= 3) {
            i1 = arg0->head - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i1 = i1 - 1;
            if (i1 < 0) {
                i1 = arg0->count - 1;
            }
            i2 = i1 - 1;
            if (i2 < 0) {
                i2 = arg0->count - 1;
            }
            if (rec->speed) {
                p1 = &recs[i1];
                if (p1->speed) {
                    p2 = &recs[i2];
                    inv1 = p1->invLen;
                    dx0 = (rec->pos.x - p1->pos.x) * rec->invLen;
                    dz0 = (rec->pos.z - p1->pos.z) * rec->invLen;
                    d1.x = (p2->pos.x - p1->pos.x) * inv1;
                    d1.z = (p2->pos.z - p1->pos.z) * inv1;
                    nrm.x = -(dz0 - d1.z);
                    nrm.z = dx0 - d1.x;
                    lenSq = nrm.x * nrm.x + nrm.z * nrm.z;
                    if (lenSq > 9.999999747e-05f) {
                        f32 inv;

                        inv = 1.0f / sqrtf(lenSq);
                        nrm.x *= inv;
                        nrm.z *= inv;
                        p1->n = nrm;
                    } else {
                        p1->n.x = 0.0f;
                        p1->n.z = 0.0f;
                    }
                }
            }
        }
        }
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151D197C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1FA770/func_151D1C98.s")

void func_151D223C(ConkerField98Owner *arg0) {
    ConkerField0Slot *temp_v0;
    ConkerField30Object *temp_v1;

    temp_v0 = arg0->field_0x98;
    temp_v1 = temp_v0->field_0x0;
    if (temp_v1 != 0) {
        temp_v1->field_0x30 = 0;
    }
}

extern void func_151478F4(void *);

void func_151D2258(s32 arg0) {
    func_151D223C(arg0);
    func_151478F4((void *)arg0);
}

extern void func_15147928(s32);

void func_151D2284(s32 arg0) {
    func_151D223C(arg0);
    func_15147928(arg0);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_151D22B0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3_151D22B0 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
} Cfg080C_151D22B0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vec3_151D22B0 pos;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
} Cfg2190_151D22B0;

typedef struct {
    /* 0x00 */ Vec3_151D22B0 pos;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ s8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s8 unk7C;
    /* 0x7D */ s8 unk7D;
    /* 0x7E */ s16 pad7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 pad84;
} Cfg1A38_151D22B0;

extern s32 D_800AB0B0[];
extern f32 D_800AB0E0[];
extern s32 D_800AB110[];
extern f32 D_800AB114[];

void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_1504715C(void *, struct127 *);
void func_1515080C(Cfg080C_151D22B0 *, s32 *, f32 *, s32, s32, s32, s32, s32, s32, void *, f32, s32, s32, u8, s32);
void func_15152190(Cfg2190_151D22B0 *, s32 *, f32 *, s32, f32, s32, u8, s32);
void func_15151A38(Cfg1A38_151D22B0 *, u8, s32);

void func_151D22B0(struct127 *arg0, u8 arg1, s32 arg2) {
    Vec3_151D22B0 pos;
    f32 lo;
    f32 hi;
    f32 mid;
    u8 floor[0x24];

    if (arg0 != NULL) {
        func_10010154(0xAA, arg0, 0x7FFF, 0x3E8, 0xBB8);
        func_10010A3C(arg0);
        mid = (arg0->xz_scale + arg0->y_scale) * 0.5f;
        func_1515C244(arg0, &pos.x, &lo, &hi);
        func_1504715C(floor, arg0);
        {
        Cfg080C_151D22B0 a;
        a.unk0 = 5;
        a.unk4 = 5;
        a.pos = pos;
        a.unk14 = 0;
        a.unk16 = 0xFF;
        a.unk18 = -0x3C;
        a.unk1A = 0x34;
        a.unk1C = 15.0f;
        a.unk20 = 20.0f;
        a.unk24 = -1.447000027f;
        a.unk28 = 0.5790000558f;
        a.unk2C = 0x50;
        a.unk2E = 0x1E;
        a.unk30 = 1 * mid;
        a.unk34 = 0 * mid;
        a.unk38 = 20.24800110f;
        a.unk3C = 1.0f;
        a.unk40 = 0xB;
        func_1515080C(&a, D_800AB0B0, D_800AB0E0, 0xC, 0, 0, 0, 1, -1, floor, 20.0f, 0, 0, arg1, arg2);
        }
        {
        Cfg2190_151D22B0 b;
        b.unk0 = 0xF;
        b.unk4 = 7;
        b.pos = pos;
        b.unk1C = 30.0f;
        b.unk20 = 10.0f;
        b.unk14 = 0;
        b.unk16 = 0xFF;
        b.unk18 = -0x1D;
        b.unk1A = 0x14;
        b.unk2C = 0x20;
        b.unk2E = 0x12;
        b.unk24 = -1.309000015f;
        b.unk28 = 0.4200000167f;
        b.unk30 = 0.1070000082f;
        b.unk34 = 0.3000000119f;
        b.unk38 = 19.23800087f;
        func_15152190(&b, D_800AB110, D_800AB114, 1, 0.0f, 0, arg1, arg2);
        }
        {
        Cfg1A38_151D22B0 c;
        c.pos = pos;
        c.unk0C = 5.0f;
        c.unk10 = 10.0f;
        c.unk14 = 0.4990000129f;
        c.unk18 = 0.7040000558f;
        c.unk1C = 20.0f;
        c.unk20 = 25.0f;
        c.unk24 = 0xA;
        c.unk26 = 7;
        c.unk2A = 0xFF;
        c.unk2C = -0x32;
        c.unk2E = 0x1E;
        c.unk30 = 3;
        c.unk32 = 3;
        c.unk34 = 0x23;
        c.unk36 = 0xF;
        c.unk39 = 1;
        c.unk3A = 0x48;
        c.unk3C = 1;
        c.unk55 = 0xC8;
        c.unk56 = 0x37;
        c.unk58 = 3;
        c.unk28 = 0;
        c.unk38 = 0;
        c.unk40 = 0;
        c.unk44 = 0;
        c.unk48 = 0;
        c.unk4C = 0;
        c.unk50 = 0;
        c.unk54 = 0;
        c.unk5C = 0xFF;
        c.unk60 = 0;
        c.unk64 = 0x220005;
        c.unk68 = 0x1D0600;
        c.unk6C = 3;
        c.unk70 = 0x3B;
        c.unk74 = 0x80;
        c.unk78 = 0x20;
        c.unk7C = 0;
        c.unk7D = 7;
        c.unk80 = 0xC;
        c.unk82 = 0x15;
        func_15151A38(&c, arg1, arg2);
        }
    }
}
