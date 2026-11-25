#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

void *memcpy(void *, const void *, u32);
void func_15143134(f32 *, f32 *, s32);
void func_15143874(s16, f32, f32 *, f32 *);
struct260 *func_15130374(void *, u8, s32, u8, s32);

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x7A - 0x3C];
    u16 unk7A;
    u8 pad7C[0x1D4 - 0x7C];
    s32 unk1D4;
} Owner151B1AB0;

/* Per-task data kept at offset 0x28 of the task created by func_151B1AB0. */
typedef struct {
    Owner151B1AB0 *owner;
    u8 ownerUnk3B;
    f32 timer;
} Data151B1AB0;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Data151B1AB0 data;
} Task151B1AB0;

/* Message passed to func_151B1FAC: kind 0x2D swaps two owners, kind 0 names an owner to drop. */
typedef union {
    struct {
        Owner151B1AB0 *a;
        Owner151B1AB0 *b;
        u8 aUnk3B;
        u8 bUnk3B;
    } swap;
    struct {
        Owner151B1AB0 *owner;
        u8 ownerUnk3B;
    } remove;
} Msg151B1FAC;

typedef struct { f32 v[3]; } Vec3_151B1B34;
extern Vec3_151B1B34 D_8008FAD0;
extern Vec3_151B1B34 D_8008FADC;

/* Spawns the task that func_151B1B34 updates for this owner. */
void func_151B1AB0(Owner151B1AB0 *owner) {
    Task151B1AB0 *task;
    Data151B1AB0 data;

    // matching: an integer test (not != NULL) makes IDO reload owner from its arg slot for each use
    if ((s32)owner != 0) {
        data.owner = owner;
        data.ownerUnk3B = owner->unk3B;
        data.timer = 0.0f;

        task = (Task151B1AB0 *)func_151491F4(0x3C, -1, 0x15, 1, 0x11, 0xC, 0xFF, 1);
        if (task != NULL) {
            memcpy(&task->data, &data, sizeof(data));
        }
    }
}

void func_151B1B34(Task151B1AB0 *arg0) {
    Data151B1AB0 *p;
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

    p = &arg0->data;
    p->timer += (697.0f + (func_150ADA68() * 342.0f)) * 0.001000000047f * D_800BE9A4;
    if (p->timer > 1.0f) {
        if (p->owner->unk1D4 != 0) {
            {
                Vec3_151B1B34 sp98 = D_8008FAD0;
                func_15143134(sp98.v, spB0, p->owner->unk1D4 + 0x800);
            }
            {
                Vec3_151B1B34 sp8C = D_8008FADC;
                func_15143134(sp8C.v, spA4, p->owner->unk1D4 + 0x640);
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
                    shade = (p->owner->unk7A >> 8) + 0x40;
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


/* Message handler for the task: follows an owner swap, or destroys itself when its owner goes away. */
void func_151B1FAC(Task151B1AB0 *task, Msg151B1FAC *msg, u8 kind) {
    Data151B1AB0 *data = &task->data;

    if (kind == 0x2D) {
        if (data->owner == msg->swap.a) {
            data->owner = msg->swap.b;
            data->ownerUnk3B = msg->swap.bUnk3B;
        } else if (data->owner == msg->swap.b) {
            // matching: the copy keeps IDO from moving the byte store into the branch delay slot
            Data151B1AB0 *dst = data;

            dst->owner = msg->swap.a;
            dst->ownerUnk3B = msg->swap.aUnk3B;
        }
    } else if (kind == 0) {
        Owner151B1AB0 *owner = msg->remove.owner;

        if (owner == data->owner || msg->remove.ownerUnk3B == data->ownerUnk3B) {
            func_1516972C((struct102 *)task);
        }
    }
}
