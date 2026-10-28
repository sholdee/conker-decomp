#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s16 D_800CC2B2;
extern f32 D_800CC2B4;
s16 func_15063390(struct127 *arg0);
void func_15143134(void *, f32 *, s32);
void func_1506AC8C(struct127 *, s32, void *);

struct func_15063B64_sp28 {
    struct127 *unk0;
    f32 unk4[3];
    s32 unk10;
    volatile s32 unk14;
};

s16 func_15063390(struct127 *arg0) {
    struct127 *temp;

    temp = &D_800CC2D0[arg0->unk222];
    return func_1505A630(temp->x_position - arg0->x_position, arg0->z_position - temp->z_position, 0);
}

void func_15063404(struct127 *arg0) {
    struct127 *temp = &D_800CC2D0[arg0 - D_800CC2D0];
    temp->unk31C->unk78 = 0x9;
    func_15083568(arg0, 0x1D, 1.0f, 0);
    func_15083568(arg0, 0x1E, 1.0f, 0);
    arg0->disable_jump = 0x14;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
    *(s16 *)((u8 *)temp->unk31C + 0x24) = 0x3C;
    temp->unk31C->unk11A = 2;
    func_1505E650(arg0, 0x7F, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

void func_150836CC(struct127 *arg0, s32 arg1);

void func_150634E4(struct127 *arg0) {
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk78 = 0;
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk11A = 0;
    func_150836CC(arg0, 0x1D);
    func_150836CC(arg0, 0x1E);
    arg0->disable_jump = 0;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
}

void func_15063570(struct127 *arg0) {
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk78 = 0x3B;
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk11A = 2;
    func_15083568(arg0, 0x89, 1.0f, 0);
    arg0->disable_jump = 0x14;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
    func_1505E650(arg0, 0x221, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

struct s15063628 {
    u8  pad0[0xB0];
    s32 unkB0;
    u8  padB4[0x130 - 0xB4];
    f32 unk130;
    f32 unk134;
    f32 unk138;
    f32 unk13C;
    f32 unk140;
    s32 unk144;
};

void func_15081690(s32, f32, f32, s32, f32, f32, f32, s32, f32, s32, s32, s32, s32, s32, s32);
void func_150636A4(struct127 *arg0);

void func_15063628(struct127 *arg0, f32 arg1) {
    struct s15063628 *p = (struct s15063628 *)arg0->unk31C;

    func_15081690(arg0, p->unk13C, p->unk140, p->unk144,
                  p->unk130, p->unk134, p->unk138, (s32)&p->unkB0,
                  arg1, 0, 0, 0, -1, 0, 0);
    func_150636A4(arg0);
}

extern struct127 D_800CC2D0[26];
void func_150636A4(struct127 *arg0);
struct tgt150636A4
{
  u8 pad0[0x195];
  u8 unk195;
  u8 unk196;
};
struct mid150636A4
{
  u8 pad0[0x31C];
  struct tgt150636A4 *unk31C;
};
struct sub150636A4
{
  u8 pad0[0xB0];
  struct mid150636A4 *unkB0;
};
void func_150636A4(struct127 *arg0)
{
  struct tgt150636A4 *new_var;
  struct mid150636A4 *temp_v0;
  temp_v0 = ((struct sub150636A4 *) arg0->unk31C)->unkB0;
  if (temp_v0 != 0)
  {
    new_var = temp_v0->unk31C;
    if (new_var != 0)
    {
      temp_v0->unk31C->unk195 = 0x1E;
      temp_v0->unk31C->unk196 = arg0 - D_800CC2D0;
    }
  }
}


typedef struct Game90840Requests {
    u8 pad0[0x78];
    u8 action;
    u8 pad79[0x36];
    u8 soundPending;
    u8 padB0[0xFA];
    s16 count;
    u8 pad1AC[8];
    u8 pending;
} Game90840Requests;

void func_15063FA0(struct127 *, s32);
void func_151D57F8(struct127 *, s32);
s32 func_15081E78(struct127 *, void *, s32);
void func_10010558(u16, void *, s32, s32, s32, s32);
void func_150FFCC8(void *, void *, void *, void *, void *);
void func_150FFD84(f32 *, f32 *, s32, s32);
void func_1512D2E4(void *, s32);

void func_150636F0(void) {
    struct127 *actor;
    s32 index;
    s32 action;
    f32 position[3];
    f32 direction[3];
    f32 side[3];
    f32 up[3];
    Game90840Requests *requests;
    void *resource;

    D_800CC2B0 = 0;
    actor = D_800CC2D0;
    index = 0;
    do {
        if ((*(s32 *)actor != 0) && (actor->unk31C != 0) &&
            (((Game90840Requests *)actor->unk31C)->pending != 0)) {
            D_800C3E78 = index;
            D_800D154C = actor;
            requests = (Game90840Requests *)actor->unk31C;
            if (requests->pending & 1) {
                func_15063628(actor, 0.0f);
                requests = (Game90840Requests *)actor->unk31C;
            }
            if (requests->pending & 4) {
                func_151D57F8(actor, 0);
                requests = (Game90840Requests *)actor->unk31C;
            }
            if (requests->pending & 8) {
                func_151D57F8(actor, 1);
                requests = (Game90840Requests *)actor->unk31C;
            }
            if (requests->pending & 2) {
                action = requests->action & ~0x80;
                switch (action) {
                case 0x18:
                case 0x41:
                    func_15063FA0(actor, 6);
                    requests = (Game90840Requests *)actor->unk31C;
                    requests->count++;
                    resource = *(void **)((u8 *)actor + 0x318);
                    if (resource != 0) {
                        func_1512D2E4(resource, 1);
                    }
                    break;
                case 0x14:
                case 0x3F:
                    if (action == 0x14) {
                        func_15063FA0(actor, 2);
                    } else {
                        func_15063FA0(actor, 0x12);
                    }
                    requests = (Game90840Requests *)actor->unk31C;
                    requests->count++;
                    resource = *(void **)((u8 *)actor + 0x318);
                    if (resource != 0) {
                        func_1512D2E4(resource, 1);
                    }
                    break;
                case 0x24:
                    if (func_15081E78(actor, (u8 *)requests + 0xB0, 7) != 2) {
                        requests = (Game90840Requests *)actor->unk31C;
                        if (requests->soundPending != 0) {
                            requests->soundPending = 0;
                            if (func_10010894(actor) == 0) {
                                func_10010558((u16)((u32)func_150ADA20() % 3 + 0x53B),
                                             actor, 0x7D00, 0x320, 0xBB8, 0x1E);
                            }
                        }
                    }
                    resource = *(void **)((u8 *)actor + 0x318);
                    if (resource != 0) {
                        func_1512D2E4(resource, 1);
                    }
                    break;
                case 0x3B:
                    func_15081E78(actor, (u8 *)requests + 0xB0, 0x10);
                    func_150FFCC8(actor, position, direction, side, up);
                    func_150FFD84(position, direction, 0xFF, 1);
                    break;
                case 0x23:
                    func_15081E78(actor, (u8 *)requests + 0xB0, 8);
                    resource = *(void **)((u8 *)actor + 0x318);
                    if (resource != 0) {
                        func_1512D2E4(resource, 0);
                    }
                    break;
                }
                requests = (Game90840Requests *)actor->unk31C;
            }
            requests->pending = 0;
        }
        index++;
        actor++;
    } while (index != 0x19);
}

s32 func_150639BC(struct127 *arg0) {
    D_800CC2B2 = arg0->unk7A - arg0->unk31C->unk12 - func_15063390(arg0);
    if (D_800CC2B2 > 0x3E80) {
        D_800CC2B2 = 0x3E80;
    }
    if (D_800CC2B2 < -0x3E80) {
        D_800CC2B2 = -0x3E80;
    }
    return D_800CC2B2;
}

s32 func_15063A38(struct127 *arg0, s32 arg1, s32 arg2) {
    if (arg1 & arg2) {
        D_800CC2B2 = ((s8 *)D_800CC284)[2] * 200;
        if (arg0->unk31C->unk84 != 0) {
            D_800CC2B2 = func_150639BC(arg0);
        }

        D_800CC2B4 = 12.0f;
        D_800CC2B2 = arg0->unk31C->unk12 + ((D_800CC2B2 * D_800CC264) / 0x7D0);

        if (D_800CC2B2 < -0x2328) {
            arg0->unk76 = arg0->unk7A = (arg0->unk7A - D_800CC2B2) - 0x2328;
            D_800CC2B2 = -0x2328;
        }

        if (D_800CC2B2 >= 0x2329) {
            arg0->unk76 = arg0->unk7A = (arg0->unk7A - D_800CC2B2) + 0x2328;
            D_800CC2B2 = 0x2328;
        }

        arg0->unk31C->unk12 = D_800CC2B2;
        return 1;
    }
    return 0;
}

void func_15063B64(struct127 *arg0) {
    struct func_15063B64_sp28 sp28;

    if ((arg0->id == 0x75) || (arg0->id == 0xB1)) {
        sp28.unk14 = 0x64;
    } else {
        sp28.unk14 = 0x2C;
    }

    if (func_1503195C(arg0, sp28.unk14, 0) != 0) {
        if (arg0->unk1D4 != 0) {
            func_15143134(0, sp28.unk4, (s32)arg0->unk1D4 + 0x100);
        } else {
            sp28.unk4[0] = arg0->x_position;
            sp28.unk4[1] = arg0->y_position;
            sp28.unk4[2] = arg0->z_position;
        }

        sp28.unk10 = ((s32)arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127) + 1;
        sp28.unk0 = arg0;
        func_1506AC8C(arg0, 0xB, &sp28);
        func_150836CC(arg0, sp28.unk14);

        if (arg0->unk31C->grenade_count != 0) {
            arg0->unk31C->grenade_count--;
        }
        arg0->unk31C->unk11A = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15063C60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15063E84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15063FA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_150641D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_150642AC.s")

void func_150649A0(s32 arg0, s32 arg1) {
    struct127 *objects;
    struct197 *dst;
    struct197 *src;

    objects = D_800CC2D0;
    dst = objects[arg1].unk2D0;
    src = objects[arg0].unk2D0;

    if (dst != 0) {
        if (src != 0) {
            dst->unk8 = src->unk8;
            if (dst->unk18 <= dst->unk8) {
                dst->unk8 = dst->unk18 - 1.0f;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15064A14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15064B94.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15065A5C.s")
