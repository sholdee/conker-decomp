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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1228D0/func_150F5C98.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1228D0/func_150F6478.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1228D0/func_150F64DC.s")

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
