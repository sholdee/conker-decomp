#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_150F6890;

extern u8 D_800917F8;
extern u8 D_80091930;
extern u8 D_8009193C;
extern u8 D_80091948;
extern f32 D_800A1B70;
struct102 *func_15169968(struct_7484 *);
void *func_15134DAC(struct_150F6890 *, s32);

void func_150F5420(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct_7484 sp24;
    s32 temp_v0;

    if (arg3 != 0) {
        if (arg3 == 2) {
            sp24.unk2C = 0xF0;
            sp24.unk2E = 0xBA;
            sp24.unk0 = &D_80091948;
        } else {
            sp24.unk2C = 0x10E;
            sp24.unk2E = 0x7C;
            if (arg3 == 1) {
                sp24.unk0 = &D_80091930;
            } else {
                sp24.unk0 = &D_8009193C;
            }
        }
        sp24.unk30 = 9;
        sp24.unk39 = 0x10;
        sp24.unk28 = 0;
        sp24.unk2A = 0;
        sp24.unk36 = 8;
    } else {
        sp24.unk0 = &D_800917F8;
        sp24.unk30 = 0xA;
        sp24.unk2C = 0xA0;
        sp24.unk2E = 0xC0;
        sp24.unk39 = 0x20;
        sp24.unk28 = 0x1000;
        sp24.unk2A = 0x1000;
        sp24.unk36 = 0x18;
    }

    temp_v0 = arg0 + arg1 + arg2;
    sp24.unk8 = (arg0 << 16) | arg1;
    sp24.unkC = (arg2 << 16) | temp_v0;
    sp24.unk10 = 0x2710;
    sp24.unk14 = temp_v0;
    sp24.unk1C = 146.0f;
    sp24.unk20 = 100.0f;
    sp24.unk18 = 0;
    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk31 = 1;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0xFF;
    sp24.unk34 = 0xFF;
    sp24.unk37 = 0x11;
    sp24.unk35 = 0;
    sp24.unk16 = 0;
    func_15169968(&sp24);
}

struct Struct150F5590 {
    char pad0[0x18];
    s32 unk18;
    s32 unk1C;
    char pad20[0x24 - 0x20];
    s16 unk24;
    char pad26[0x38 - 0x26];
    s16 unk38;
    s16 unk3A;
    char pad3C[0x45 - 0x3C];
    u8 unk45;
};

void func_150F55C8(struct Struct150F5590 *);

void func_150F5590(struct Struct150F5590 *arg0) {
    s32 temp = 0x1000 - (arg0->unk24 << 2);
    arg0->unk38 = temp;
    arg0->unk3A = arg0->unk38;
    func_150F55C8(arg0);
}

void func_150F55C8(struct Struct150F5590 *arg0) {
    s32 temp_t2;
    s32 temp_v0;
    s32 temp_a1;
    s32 temp_a3;
    s32 temp_t1;
    s32 temp_t0;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t4;

    temp_v0 = arg0->unk1C & 0xFFFF;
    temp_a1 = arg0->unk18 >> 16;
    temp_a3 = arg0->unk18 & 0xFFFF;
    temp_t0 = arg0->unk1C >> 16;
    temp_t1 = temp_v0 - temp_a1;
    temp_t2 = arg0->unk24;

    if (temp_t1 < temp_t2) {
        temp_t6 = temp_v0 - temp_t2;
        temp_t7 = temp_t6 * 0xFF;
        arg0->unk45 = temp_t7 / temp_a1;
    } else if ((temp_t1 - temp_a3) < temp_t2) {
        arg0->unk45 = 0xFF;
    } else {
        temp_t4 = temp_t2 * 0xFF;
        arg0->unk45 = temp_t4 / temp_t0;
    }
}

void func_15179008(s32);

void func_150F568C(s32 arg0) {
    func_15179008(0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1228D0/func_150F56B0.s")

s32 func_150F5A54(s32 arg0, struct127 *arg1) {
    struct197 *anim;
    Gfx *gfx;
    s32 count;
    s32 forceTwo;
    f32 scale;
    s32 value;

    anim = arg1->unk2D0;
    if (anim == 0) {
        return 0;
    }

    gfx = **(Gfx ***)(arg0 + 0x24);
    count = 1;
    forceTwo = 0;
    switch (*(s32 *)(arg0 + 0x38)) {
    case 0:
        if (arg1->unk84.uh == 5) {
            scale = 1.0f;
            *(s32 *)(arg0 + 0x38) = 1;
        } else {
            forceTwo = 1;
        }
        break;
    case 1:
        scale = 1.0f;
        break;
    case 2:
        scale = 0.0f;
        break;
    }

    if (arg1->unk84.uh == 5) {
        if (anim->unk8 < 69.0f) {
            scale = 1.0f;
        } else if (84.0f < anim->unk8) {
            scale = 0.0f;
            *(s32 *)(arg0 + 0x38) = 2;
        } else {
            scale = (anim->unk8 - 69.0f) / 15;
            scale = 1.0f - scale;
        }
    }

    arg0 = 0;
    do {
        count--;
        if (*(s8 *)((arg0 << 3) + (u8 *)gfx) != -0xE) {
            do {
            } while (*(s8 *)((u8 *)gfx + (++arg0 << 3)) != -0xE);
        }
        if (count != 0) {
            arg0++;
        }
    } while (count != 0);

    if (forceTwo != 0) {
        value = 2;
    } else {
        value = (s32)((58.0f * scale) + 44.0f);
    }
    gfx[arg0].words.w0 = (G_SETTILESIZE << 24) | (2 << 12) | (value & 0xFFF);
    return 0;
}

void func_150F5C08(void *arg0, s16 arg1, u8 arg2, s32 arg3) {
    struct {
        void *unk0;
        u8 unk4;
        u8 pad5;
        u8 pad6;
        u8 pad7;
        f32 unk8;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    sp34.unk8 = 0.0f;

    temp_v0 = func_15149130(arg1, -1, 0x51, -1, 1, 0x3E, (struct37 *)0xC, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

/* Partial view of the owning actor. */
typedef struct {
    s32 interaction_state;
    u8 pad4[0x3B - 0x4];
    u8 unique_id;
    u8 pad3C[0x1D4 - 0x3C];
    s32 model;
} EffectOwner5C98;

/* Particle descriptor passed to func_15130280 (0x70 bytes). */
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
} Particle5C98; /* 0x70 */

typedef struct {
    EffectOwner5C98 *owner;
    u8 ownerId;
    u8 pad5[3];
    f32 accum;
} Data5C98;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xC - 0x2];
    u8 unkC;
    u8 padD;
    s16 state;
    u8 pad10[0x28 - 0x10];
    Data5C98 data;
} Effect5C98;

extern struct17 D_800A1B00;
extern struct17 D_800A1B0C;
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150F5C98(Effect5C98 *effect) {
    EffectOwner5C98 *owner;
    Data5C98 *data;
    Particle5C98 spawn;
    f32 scale;
    struct17 a;
    struct17 b;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 srcs[2];
    s32 dsts[2];

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->model != 0) {
        data->accum += (0.2600000203f + func_150ADA68() * 0.1950000077f) * D_800BE9A4;
        if (data->accum > 1.0f) {
            spawn.unk14 = 0xFF;
            spawn.unk15 = 0xFF;
            spawn.unk16 = 0xFF;
            spawn.unk18 = 0xB4;
            spawn.unk19 = 0xB4;
            spawn.unk1A = 0xB4;
            srcs[0] = (s32)&D_800A1B00;
            srcs[1] = (s32)&D_800A1B0C;
            dsts[0] = (s32)&a;
            dsts[1] = (s32)&b;
            func_15145EA4(srcs, dsts, owner->model + 0x140, 2);
            spawn.unk1D = 0x6C;
            spawn.unk8 = 0x5103;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0x9F0600;
            spawn.unkC = 0;
            spawn.unk10 = 0;
            spawn.unk17 = 0xFF;
            spawn.unk1C = 0xFF;
            dx = b.unk0 - a.unk0;
            dy = b.unk4 - a.unk4;
            dz = b.unk8 - a.unk8;
            spawn.unk3C = *(struct17 *)&D_800A5480;
            spawn.unk58 = 0x84CE07;
            spawn.unk60 = 8;
            spawn.unk61 = 6;
            spawn.unk62 = 0x10;
            spawn.unk63 = -1;
            spawn.unk64 = -1;
            spawn.unk65 = 0;
            spawn.unk5C = 0;
            spawn.unk66 = 0xFF;
            spawn.unk68 = 1000;
            spawn.unk6C = 1000.0f;
            scale = 0.9730340242f;
            spawn.unk24 = 1.022472024f;
            do {
                s16 life;
                f32 k;
                f32 t;
                s32 flags;
                void *ret;

                spawn.unkA = func_150ADA20() % 21U + 20;
                spawn.unk22 = spawn.unkA;
                spawn.unk1E = spawn.unkA;
                spawn.unk20 = 0xFF / spawn.unkA;
                spawn.unk1B = func_150ADA20() % 26U + 8;
                spawn.unk28 = spawn.unk2C = func_150ADA68() * 70.0f + 148.0f;
                spawn.unk54 = func_150ADA68() * 0.2190000117f + 0.08600000292f;
                k = func_150ADA68() * 0.04100000113f + 0.07800000161f;
                spawn.vel.unk0 = dx * k;
                spawn.vel.unk4 = dy * k;
                spawn.vel.unk8 = dz * k;
                t = func_150ADA68() * D_800BE9A4;
                spawn.unk58 &= ~0xC0;
                spawn.pos.unk0 = spawn.vel.unk0 * t + a.unk0;
                spawn.pos.unk4 = spawn.vel.unk4 * t + a.unk4;
                spawn.pos.unk8 = spawn.vel.unk8 * t + a.unk8;
                flags = (func_150ADA20() & 1) ? 0x80 : 0;
                spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | flags;
                ret = func_15130280(&spawn, 1, 0, 4, effect->unkC, effect->unk1);
                if (ret != NULL) {
                    memcpy((u8 *)ret + 0xA8, &scale, 4);
                }
                data->accum -= 1.0f;
            } while (data->accum > 1.0f);
        }
    }
}

void func_150F6138(s32 arg0, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

/* Partial view of an actor (struct127) that an effect is attached to. */
typedef struct {
    s32 interaction_state; /* 0 = actor slot free */
    u8 pad4[0x10];
    f32 x_position;
    f32 y_position;
    f32 z_position;
    u8 pad20[0x3B - 0x20];
    u8 unique_id; /* changes when the slot is reused; effects compare it to detect a dead owner */
    u8 pad3C[0x94 - 0x3C];
    s32 unk94;
    u8 pad98[0x1D4 - 0x98];
    s32 model; /* model instance; points are transformed from its local space to world */
} EffectOwner;
typedef struct {
    u8 pad0[0x48];
    f32 unk48;
    f32 unk4C;
    u8 pad50[0x58 - 0x50];
    u8 flags; /* bit 0 = shown */
} Part6178;
/* Something drawn between two points (a beam or streak). */
typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x110 - 0x4C];
    Part6178 part;
} Beam6178;
/* Something drawn at a point (a glow or light). */
typedef struct {
    u8 pad0[0x9];
    u8 hidden;
    u8 padA[0xE - 0xA];
    s16 x;
    s16 y;
    s16 z;
} Glow6178;
typedef struct {
    u8 pad0[0x14];
    Glow6178 *glow;
} GlowHolder6178;
typedef struct {
    EffectOwner *owner;
    u8 ownerId;
    u8 pad5[3];
    Beam6178 *beam;
    GlowHolder6178 *glowHolder;
} BeamData;
typedef struct {
    u8 pad0[0xE];
    s16 state; /* set to -1 to free the effect */
    u8 pad10[0x28 - 0x10];
    BeamData data;
} BeamEffect;
extern struct17 D_800A1B18;
extern struct17 D_800A1B24;
extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_150F6178(BeamEffect *effect) {
    BeamData *data;
    EffectOwner *owner;
    struct17 a;
    struct17 b;
    s32 srcs[2];
    s32 dsts[2];

    data = &effect->data;
    owner = data->owner;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->state = -1;
        return;
    }
    if (owner->model != 0 && !(owner->unk94 & 2)) {
        Beam6178 *beam;
        GlowHolder6178 *holder;
        Glow6178 *glow;

        srcs[0] = (s32)&D_800A1B18;
        srcs[1] = (s32)&D_800A1B24;
        dsts[0] = (s32)&a;
        dsts[1] = (s32)&b;
        func_15145EA4(srcs, dsts, owner->model, 2);
        if (data->beam != NULL) {
            beam = data->beam;
            beam->part.flags |= 1;
            beam->start = a;
            beam->end = b;
        }
        holder = data->glowHolder;
        if (holder != NULL) {
            glow = holder->glow;
            glow->hidden = 0;
            glow->x = a.unk0;
            glow->y = a.unk4;
            glow->z = a.unk8;
        }
    } else {
        if (data->beam != NULL) {
            Part6178 *part = &data->beam->part;

            part->flags &= ~1;
        }
        if (data->glowHolder != NULL) {
            Glow6178 *glow = data->glowHolder->glow;

            glow->hidden = 1;
        }
    }
    if (data->beam != NULL) {
        Part6178 *part = &data->beam->part;

        part->unk48 = 4.0f;
        part->unk4C = 8.0f;
    }
}

void func_150F631C(struct260 *arg0) {
    struct260 *temp_a1;
    struct102 *temp_a0;

    temp_a1 = arg0;
    if (*(struct102 *volatile *)((u8 *)temp_a1 + 0x30) != NULL) {
        func_1516972C(*(struct102 **)((u8 *)temp_a1 + 0x30));
    }

    temp_a0 = *(struct102 **)((u8 *)temp_a1 + 0x34);
    if (temp_a0 != NULL) {
        func_1516972C(temp_a0);
    }
}

void func_150F631C(struct260 *arg0);

void func_150F6368(struct260 *arg0) {
    func_150F631C(arg0);
    func_1514933C(arg0);
}

void func_15149368(struct260 *arg0);

void func_150F6394(struct260 *arg0) {
    func_150F631C(arg0);
    func_15149368(arg0);
}

void func_150F63C0(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

typedef struct {
    char pad_0[0x8];
    s32 field_0x08;
} Field160ChildBlock;

typedef struct {
    char pad_0[0x160];
    u8 * volatile field_0x160;
} Field160Owner;

void func_150F6400(Field160Owner *arg0) {
    Field160ChildBlock *p;

    if (arg0->field_0x160 != 0) {
        p = (Field160ChildBlock *)(arg0->field_0x160 + 0x28);
        p->field_0x08 = 0;
    }
}

void func_150F6420(u8 *arg0) {
    func_150F6400(arg0);
    func_1513CA6C((struct210 *)arg0);
}

void func_150F644C(u8 *arg0) {
    func_150F6400(arg0);
    func_1513CAA0((struct210 *)arg0);
}

void func_150F6478(struct210 *arg0) {

}

void func_150F6478(struct210 *);

void func_150F6484(struct210 *arg0) {
    func_150F6478(arg0);
    func_151411A4(arg0);
}

void func_151411C4(struct210 *);

void func_150F64B0(struct210 *arg0) {
    func_150F6478(arg0);
    func_151411C4(arg0);
}

typedef struct {
    /* 0x00 */ struct127 *obj;
    /* 0x04 */ u8 id;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 timer;
} Func150F64DCLink;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ Func150F64DCLink link;
} Func150F64DCSelf;

extern u8 D_800A1B30;
void func_15143134(void *, f32 *, s32);
extern struct225 *func_151602C0(Header *, Header2 *, s32, s32, s32, s32, u8, u8, s32, u8, s32);
void *func_15107C1C(struct127 *, u8, void *, s16, s32, s32, s32, f32, f32, s32, s32, u8 *, s32, s32);

void func_150F64DC(Func150F64DCSelf *arg0) {
    Func150F64DCLink *link;
    struct127 *obj;
    s8 count;

    obj = arg0->link.obj;
    link = &arg0->link;
    if ((obj->interaction_state == 0) || (obj->unique_id != link->id)) {
        arg0->unkE = -1;
        return;
    }
    if ((obj->unk1D4 != 0) && !(*(s32 *)((u8 *)obj + 0x94) & 2)) {
        link->timer -= D_800BE9E4;
        if (link->timer < 0) {
            func_10010F88(0x679, 0x18CE, 0, 0, 0, obj->x_position, obj->y_position, obj->z_position, 0x7918, 0x7D00);
            count = (func_150ADA20() & 1) + 2;
            {
    f32 pos[3];
    Header header;
    Header2 header2;
    u8 color[4];
    s32 r1;
    s32 r2;
    s32 r3;
            func_15143134(&D_800A1B30, pos, (s32)obj->unk1D4);
            header.unk0 = 3;
            header.unk1 = -1;
            header.unk2 = (func_150ADA20() % 9U) + 10;
            header.unk4 = 0;
            header2.unk0 = pos[0];
            header2.unk4 = pos[1];
            header2.unk8 = pos[2];
            r3 = (func_150ADA20() % 61U) + 60;
            func_151602C0(&header, &header2, r3, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg0->unkC, arg0->unk1);
            do {
                color[0] = 0xA0;
                color[1] = 0xA0;
                color[2] = 0xFF;
                color[3] = (func_150ADA20() % 101U) + 155;
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                r3 = func_150ADA20();
                func_15107C1C(obj, 0, &D_800A1B30, (u8)r1, (r2 % 43U) - 50, (r3 % 18U) + 5, 4, 27.0f, (func_150ADA68() * 16.0f) + 20.0f, 0, 2, color, arg0->unkC, arg0->unk1);
                count--;
            } while (count > 0);
            link->timer = (func_150ADA20() % 91U) + 90;
            }
        }
    }
}

void func_150F6850(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

void func_150F6890(void *arg0, s16 arg1, s32 arg2, s32 arg3) {
    struct_150F6890 sp1C;

    sp1C.unk0 = *((u8 *)arg0 + 0x3B);
    sp1C.unk1C = D_800A1B70;
    sp1C.unk20 = 8.5f;
    sp1C.unk34 = 1.0f;
    sp1C.unk4 = arg0;
    sp1C.unk8 = 3;
    sp1C.unkC = 0.0f;
    sp1C.unk10 = 0.0f;
    sp1C.unk14 = 0.0f;
    sp1C.unk18 = 0.0f;
    sp1C.unk24 = 2;
    sp1C.unk26 = 0x28;
    sp1C.unk28 = 0x10;
    sp1C.unk2A = arg1;
    sp1C.unk2E = 5;
    sp1C.unk2F = 8;
    sp1C.unk30 = -1;
    sp1C.unk38 = 0;
    sp1C.unk39 = -1;
    sp1C.unk2C = 0;

    func_15134DAC(&sp1C, 0);
}

void func_15137F30(void *, void *, void *, void *, f32, void *, void *, void *,
                   void *, f32 *, s16 *, s8 *, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32,
                   s32, s32, s32, s32, s32, s32);
void func_1504715C(f32 *, s32);

void func_150F695C(void *arg0, void *arg1, void *arg2, void *arg3,
                   f32 arg4, u8 *arg5) {
    struct {
        f32 value;
        u8 pad60[0x14];
        s32 field74;
        s8 field78;
        s8 field79;
        u8 pad7A[2];
        s32 field7C;
        f32 scale;
        u8 pad84;
        s8 field85;
        s16 field86;
        f32 field88;
        f32 vector8C[3];
        u8 field98[0xC];
        f32 vectorA4[3];
    } local;
    u8 selected;
    s32 variant;
    f32 random;

    func_15137F30(arg0, arg1, arg2, arg3, arg4, arg5,
                  local.vectorA4, local.field98, local.vector8C,
                  &local.field88, &local.field86, &local.field85,
                  &local.scale);
    random = func_150ADA68();
    local.scale *= 1.0f + (random + random);
    selected = func_150ADA68() < 0.6f;
    if (selected != 0) {
        func_1504715C(&local.value, *(s32 *)(arg5 + 0x1C));
    } else {
        local.field74 = 0;
        local.field78 = 0;
        local.field79 = 0;
        local.field7C = 0;
        local.value = -10000.0f;
    }
    if (selected) {
        variant = func_150ADA68() < 0.5f;
    } else {
        variant = 0;
    }
    func_151D9014(local.vectorA4, local.vector8C, 1, local.field88,
                  local.field86, (u8)local.field85, local.scale, selected,
                  1.8f, 1.8f, 1, (s32)&local.value, 1,
                  variant, arg5[0xC], arg5[1]);
}
