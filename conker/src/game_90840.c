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
  struct tgt150636A4 *target;
  struct mid150636A4 *temp_v0;
  temp_v0 = ((struct sub150636A4 *) arg0->unk31C)->unkB0;
  if (temp_v0 != 0)
  {
    target = temp_v0->unk31C;
    if (target != 0)
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

/* Object-level asm-differ reports score 40 here, but the difference is NOT codegen: all
 * 137 instructions, registers, branch targets, delay slots and IDO's peel-1-then-unroll-by-4
 * are identical. The only differing words are four relocation ADDENDS, where the extracted
 * .s names interior symbols of D_800CC2D0[26] (D_800CC5FC = +0x32C, D_800CC2D4 = +0x4,
 * D_800CC335 = +0x65, D_800D121C = +0x4F4C) and C naturally spells them as base+offset.
 * Both resolve to the same linked addresses, so the ROM bytes are identical -- which the
 * force-clean full-ROM sha1 gate confirms. */
struct127 *func_15063C60(struct127 *arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) && (arg1 == D_800CC2D0[i].id) &&
            (((arg0 - D_800CC2D0) + 1) == D_800CC2D0[i].unk65)) {
            return &D_800CC2D0[i];
        }
    }
    return NULL;
}

typedef struct Ctl15063E84 {
    u8 pad0[0x27];
    u8 unk27;
    u8 pad28[0x50];
    u8 unk78;
    u8 pad79[0x1C];
    u8 unk95;
    u8 pad96[0x84];
    u8 unk11A;
} Ctl15063E84;

/* struct126 declares unk95 as s8; golden reads it with lbu, so view the block through a local u8 layout. */
#define CTL ((Ctl15063E84 *)arg0->unk31C)

s32 func_15063E84(struct127 *arg0, u16 anim, u16 flags, s32 arg3, s32 arg4) {
    s32 state;

    if (arg4 & 0x4000) {
        if (arg0->unk28 == 0.0f) {
            if (arg0->y_velocity <= 0.0f) {
                if (CTL->unk95 == 0) {
                    if (arg0->unk84.uh != anim) {
                        state = CTL->unk11A;
                        if (state == 0 || state == 2) {
                            if (CTL->unk27 == 0) {
                                if (arg3 != 0 && arg0->disable_jump != 0) {
                                    return 999;
                                }
                                if (flags & 1) {
                                    arg0->disable_jump = 20;
                                }
                                if (arg3 != 0) {
                                    if (CTL->unk11A == 2) {
                                        return 999;
                                    }
                                    CTL->unk11A = 1;
                                } else {
                                    CTL->unk11A = 3;
                                }
                                CTL->unk78 = arg3;
                                arg0->disable_run = 0xFF;
                                arg0->unk83 = 0xFF;
                                return anim;
                            }
                        }
                    }
                }
            }
        }
    }
    return 999;
}

#undef CTL

typedef struct {
    u8 pad0[0xB8];
    f32 unkB8;
    f32 unkBC;
    f32 unkC0;
    u8 padC4[0x109 - 0xC4];
    u8 unk109;
    u8 pad10A[0x13C - 0x10A];
    f32 unk13C;
    f32 unk140;
    f32 unk144;
    u8 pad148[0x16C - 0x148];
    f32 unk16C;
    f32 unk170;
} s15063FA0;

void func_15063FA0(struct127 *arg0, s32 arg1) {
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 dist;
    f32 pos[3];
    f32 off[2];
    f32 height;
    f32 radius;
    f32 angle;
    f32 pitch;
    f32 yaw;
    f32 dx;
    f32 dy;
    f32 dz;
    s15063FA0 *p;

    dist = 300.0f;
    height = 0.0f;
    radius = 0.0f;
    switch (arg1) {
    case 2:
        radius = 27.0f;
        break;
    case 3:
    case 6:
        height = -11.0f;
        dist = 500.0f;
        break;
    }
    angle = ((s15063FA0 *)arg0->unk31C)->unk16C * 0.01745329238f;
    off[0] = sinf(angle) * radius;
    off[1] = cosf(angle) * radius;
    pos[0] = off[0] + ((s15063FA0 *)arg0->unk31C)->unk13C;
    pos[1] = ((s15063FA0 *)arg0->unk31C)->unk140 + height;
    pos[2] = off[1] + ((s15063FA0 *)arg0->unk31C)->unk144;
    p = (s15063FA0 *)arg0->unk31C;
    if (p->unk109 == 0) {
        pitch = -p->unk170;
        yaw = p->unk16C + 90.0f;
    } else {
        dx = p->unkB8 - pos[0];
        dy = p->unkBC - pos[1];
        dz = p->unkC0 - pos[2];
        pitch = func_150484A0(dy, sqrtf(dx * dx + dz * dz)) * 57.29577637f;
        yaw = func_150484A0(dx, dz) * 57.29577637f;
    }
    func_1505A184((s16)(pitch * 182.0444489f), dist, 0.0f, &sp84, &sp80, &sp7C);
    sp80 = -sp80;
    func_1506C460(yaw, sp84, sp80, 0.0f, 100, arg1, 0.0f, 0.5f, (s32)pos, 1, 1);
}


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

void func_15064A14(struct127 *arg0) {
    struct127 *partner;
    s32 anim;
    s32 big;

    partner = &D_800CC2D0[arg0->unk65 - 1];
    anim = 0xF;
    if (partner->id == 0xC) {
        anim = 0x5A;
        if (partner->unk84.uh == 0xC) {
            anim = 0x5B;
        } else if (partner->unk84.uh == 0xD) {
            anim = 0x5C;
        }
    } else if (partner->id == 0x53) {
        anim = 0x8C;
        if (partner->unk84.uh == 0xB) {
            anim = 0xA2;
        } else if (partner->unk84.uh == 0xD) {
            anim = 0xA3;
        }
    } else if (partner->id == 0x21) {
        anim = 0x169;
    } else if (partner->id == 0x8A) {
        big = ((arg0->unk31C != NULL) && (((u8 *)arg0->unk31C)[0x1B2] != 0)) ? 1 : 0;
        if (partner->unk232 == 4) {
            anim = big ? 0x350 : 0x1A4;
        } else {
            anim = big ? 0x34F : 0x1A1;
        }
    }
    func_1505E650(arg0, anim, 1.0f, 6.0f, 0.0f, 0.0f, 0);
    func_150649A0(arg0->unk65 - 1, D_800C3E78);
}

extern u8 D_800B85A4[];

s32 func_15064B94(u8 *arg0, f32 arg1, f32 arg2, s32 arg3, f32 arg4) {
    s32 anim;
    f32 blend;
    f32 speed;
    u8 *inner;

    anim = 0;
    blend = 4.0f;
    inner = *(u8 **)(arg0 + 0x31C);
    speed = 1.0f;
    if (*(s16 *)(inner + 0x46) > 0) {
        anim = 0x4C;
        speed = arg1 / 24.0f;
    } else if (inner[0x4E] == 2) {
        anim = 5;
        if ((s8)inner[0x50] > 0) {
            anim = 0x54;
        }
        blend = 2.0f;
    } else if (arg0[0x13C] >= 100) {
        if (*(f32 *)(arg0 + 0x3C) < 1.0f) {
            speed = 1.0f;
            anim = func_1504C078();
        } else {
            switch (D_800B85A4[((u8 *)D_800D154C)[0x13C] * 0x32C]) {
            case 0x8C:
                arg0[0xAA] = 0x5A;
                anim = 0x1A6;
                speed = arg1 / 6.0f + 0.4f;
                if (*(f32 *)(arg0 + 0x3C) > 14.0f) {
                    anim = 0x1A7;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0x57:
                arg0[0xAA] = 0x5A;
                anim = 0x116;
                speed = arg1 / 6.0f + 0.4f;
                if (*(f32 *)(arg0 + 0x3C) > 14.0f) {
                    anim = 0x117;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0xA8:
            case 0xA9:
                if (*(f32 *)(arg0 + 0x3C) > 14.0f) {
                    anim = 0x1B1;
                    speed = arg1 / 22.0f + 0.4f;
                } else {
                    anim = 0x1B0;
                    speed = arg1 * 0.0625f + 0.6f;
                }
                break;
            case 0x13:
                anim = 0x14C;
                speed = arg1 / 6.0f;
                break;
            case 0x89:
                anim = 0x1FB;
                speed = arg1 / 6.0f;
                break;
            default:
                if (*(f32 *)(arg0 + 0x3C) > 14.0f) {
                    anim = 0x2E4;
                    speed = arg1 * 0.0625f + 0.5f;
                } else {
                    anim = 0xD7;
                    speed = arg1 / 6.0f + 0.4f;
                }
                break;
            }
        }
    } else if (arg3 != 0 && inner[0x78] == 0) {
        anim = arg3;
        speed = arg1 / 15.0f + 0.6f;
    } else if (2.6f < *(f32 *)(arg0 + 0x44) && (*(u16 *)(inner + 0x32) & 1)) {
        arg0[0xAA] = 0x28;
        blend = 5.0f;
        speed = *(f32 *)(arg0 + 0x44) / 15.0f + 0.5f;
        anim = func_1504C0B8();
    } else if (inner[0x78] == 0x16) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            anim = inner[0x1B2] != 0 ? 0x2C6 : 0xE8;
            arg0[0xAA] = 0x1E;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 9) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            if (D_800BE616 == 0) {
                arg0[0xAA] = 0x37;
            }
            anim = 0x7E;
            if (D_800BE616 == 0) {
                speed = arg1 * 0.0625f + 0.6f;
            } else if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x22C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x38) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            anim = 0x1E1;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x1E2;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x39) {
        if (*(u16 *)(arg0 + 0x84) != 0x1EB) {
            if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
                anim = 0x1EC;
                if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                    anim = 0x1ED;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = func_1504C078();
            }
        } else {
            anim = 999;
        }
    } else if (inner[0x78] == 0x37) {
        if (*(u16 *)(arg0 + 0x84) != 0x1F4) {
            if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
                anim = 0x1F5;
                if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                    anim = 0x1F6;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = func_1504C078();
            }
        } else {
            anim = 999;
        }
    } else if (inner[0x78] == 0x3B) {
        if (*(u16 *)(arg0 + 0x84) != 0x222 && *(u16 *)(arg0 + 0x84) != 0x236) {
            if (*(f32 *)(arg0 + 0x44) >= 1.0f || (s8)inner[0x23] > 0) {
                anim = 0x223;
                arg0[0xAA] = 0x37;
                if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                    anim = 0x224;
                    speed = arg1 / 22.0f + 0.4f;
                }
            } else {
                speed = 1.0f;
                anim = func_1504C078();
            }
        } else {
            anim = 999;
        }
    } else if (inner[0x78] == 0x12 || inner[0x78] == 0x18 || inner[0x78] == 0x41) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0xEC;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0xF8;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x24) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0x158;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x159;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x22) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0x15B;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x15C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x21) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x6E;
            anim = 0x123;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 15.0f) {
                anim = 0x124;
                speed = arg1 / 27.0f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x26) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0x178;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x179;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x3A) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0x1FB;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x25) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0xD7;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x2D || inner[0x78] == 0x2E) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            anim = 0x1B0;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x1B1;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x23) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0x161;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                anim = 0x162;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x15) {
        if (*(f32 *)(arg0 + 0x44) >= 1.0f) {
            arg0[0xAA] = 0x50;
            anim = 0xED;
            speed = arg1 * 0.0625f + 0.6f;
            if (*(f32 *)(arg0 + 0x44) >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x78] == 0x19 || inner[0x78] == 0x40) {
        if (*(f32 *)(arg0 + 0x3C) >= 1.0f) {
            anim = 0x112;
            blend = 8.0f;
            speed = arg1 * 0.125f + 0.3f;
            if (*(f32 *)(arg0 + 0x3C) >= 18.0f) {
                anim = 0x111;
                speed = arg1 / 20.0f + 0.3f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (inner[0x1A] != 0 && arg2 > 1.0f) {
        anim = 0x6D;
        speed = arg1 / 6.5f + 0.3f;
        arg0[0xAA] = 0x28;
        blend = 8.0f;
    } else if (*(s16 *)(inner + 8) > 0) {
        if (arg2 < 1.0f) {
            speed = 1.0f;
            anim = func_1504C078();
        } else {
            anim = *(u16 *)(inner + 0xE);
            speed = arg1 / 6.0f + 0.3f;
            blend = 16.0f;
        }
    } else if (inner[0x17] != 0) {
        blend = 5.0f;
        if (*(u16 *)D_800CC284 & 0x2000) {
            if (*(u16 *)(arg0 + 0x84) != 0x283) {
                *(f32 *)(arg0 + 0x3C) *= 0.2f;
                D_800D1580 = 0xFF010604;
                func_1506E5FC();
                speed = 1.0f;
            }
            anim = 0x283;
            arg0[0x89] = 3;
            arg0[0x8A] = 3;
        } else {
            if (*(u16 *)(arg0 + 0x84) == 0x283) {
                D_800D1580 = 0xFF010604;
                func_1506E5FC();
            }
            if (*(f32 *)(arg0 + 0x44) >= 1.0f && arg0[0x89] == 0) {
                arg0[0xAA] = 0x2D;
                speed = *(f32 *)(arg0 + 0x3C) / 9.0f;
                anim = 0x4E;
            } else {
                speed = 1.0f;
                anim = func_1504C078();
            }
        }
    } else if ((*(u16 *)(inner + 0x32) & 2) && *(f32 *)(arg0 + 0x44) >= 1.0f) {
        inner[0x3C] = 0x1C;
        arg0[0xAA] = 0xF;
        speed = arg4 / 5.0f + 0.3f;
        blend = 8.0f;
        anim = 0x64;
        if (func_150ADA20() % 1000U >= 988) {
            anim = (func_150ADA20() & 1) + 0x65;
            speed = 0.65f;
            arg0[0x89] = 0xFF;
            arg0[0x83] = 0xFF;
        }
    }
    if (anim != 0 && anim != 999) {
        func_1505E650((struct127 *)arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
    return anim;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_90840/func_15065A5C.s")
