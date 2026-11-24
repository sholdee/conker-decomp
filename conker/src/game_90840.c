#include <ultra64.h>
#define func_15083568 func_15083568_void_proto
#define func_150ADA20 func_150ADA20_u8_proto
#define func_1000FA64 func_1000FA64_hdr
#define func_1000FF90 func_1000FF90_hdr
#define func_100109D0 func_100109D0_hdr
#define func_10010A3C func_10010A3C_hdr
#include "functions.h"
#undef func_15083568
#undef func_150ADA20
#undef func_1000FA64
#undef func_1000FF90
#undef func_100109D0
s32 func_100109D0();
#undef func_10010A3C
s32 func_10010A3C();
s32 func_1000FF90(void *arg0, s32 arg1, s32 arg2);
u16 func_1000FA64(u16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5, s16 arg6, void *arg7, void *arg8, void *arg9, s32 argA, s32 argB);
u32 func_150ADA20(void);
struct127 *func_15083568(struct127 *, s32, f32, s32);
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


s32 func_1505DADC(void *, u16 *, s32, s32, s32);

u16 func_150641D8(void *arg0, u16 arg1, u16 arg2, u16 arg3) {
    u16 selected;
    u8 *actor = arg0;
    u16 output;

    output = 0;
    selected = arg1;
    if (func_150ADA20() & 1) {
        selected = arg2;
    }
    if ((arg1 == *(u16 *)(actor + 0x84)) || (arg2 == *(u16 *)(actor + 0x84))) {
        if (actor[0x107] < 0x28) {
            return 0x3E7;
        }
        selected = arg3;
    }
    if (arg3 == *(u16 *)(actor + 0x84)) {
        selected = 0x3E7;
    }
    if (func_1505DADC(actor, &output, 0, 0xFE, 0x40) != 0xFF) {
        *(u16 *)(actor + 0x76) = output;
    }
    *(u8 *)(actor + 0x83) = 0xFF;
    return selected;
}

extern s32 D_800CC28C;
void func_15074664(void);
void func_15060A30(s32 arg0, struct127 *arg1);
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);

void func_150642AC(struct127 *arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4) {
    s32 anim;
    f32 speed;
    f32 blend;
    struct127 *obj;
    struct126 *inner;
    u8 *sub;
    f32 dx;
    f32 dist;
    f32 dz;
    s16 x;
    s16 z;
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    anim = 0;
    speed = 1.0f;
    blend = 4.0f;
    if (arg4 != 0) {
        arg1 &= ~0x4000;
        D_800CC28C &= ~0x4000;
    }
    if (arg0->unk31C->pad79[2] != 0) {
        switch (arg0->unk31C->pad79[2]) {
        case 1: {
            s16 pad;

            arg0->unk83 = 0xFF;
            arg0->disable_run = 0xFF;
            x = (arg0->unk2E4 >> 16) & 0xFFFF;
            z = arg0->unk2E4 & 0xFFFF;
            arg0->unk84.uh = 0;
            arg0->unk31C->unk30 = 0xD;
            *(f32 *)&arg0->unk31C->unk28 = x;
            *(f32 *)&arg0->unk31C->unk2C = z;
            anim = 0x96;
            arg0->unk76 = arg0->unk2E8 & 0xFFFF;
        }
            break;
        case 2:
            speed = 0.8000000119f;
            anim = 0xDD;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
            break;
        case 3:
            D_800D1580 = 1;
            func_15074664();
            if (arg0->in_water != 0 || (s32)arg0->unk28 != 0) {
                func_15083568(arg0, 0x17, 1.0f, 0);
                arg0->unk31C->unk11A = 2;
            } else {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                speed = 3.0f;
                anim = 0x151;
                arg0->unk31C->unk11A = 1;
            }
            break;
        case 4:
            D_800D1580 = 0;
            func_15074664();
            if (arg0->in_water != 0 || (s32)arg0->unk28 != 0) {
                func_150836CC(arg0, 0x17);
                arg0->unk31C->unk11A = 0;
            } else {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                speed = 3.0f;
                anim = 0x14E;
                arg0->unk31C->unk11A = 3;
            }
            break;
        case 5:
            obj = func_1505F0AC(0x6C);
            arg0->unk81 = 1;
            if (obj->unk2E4 == 0) {
                arg0->unk1CC = arg0->y_position;
                obj->unk2E4 = 100;
                anim = 0x38;
            }
            if (obj->unk2E4 >= 100) {
                if (obj->unk1D4 != NULL) {
                    sub = (u8 *)obj->unk1D4 + 0x140;
                    arg0->y_position = *(s16 *)(sub + 0x1A) + 20;
                }
            }
            if (arg1 & 0x8000) {
                if (obj->unk2E4 == 100) {
                    obj->unk2E4 = 0x7A;
                    obj->unk2E8 = 0x7A;
                    anim = 0x16;
                    func_15060A30(0x38E, arg0);
                }
            }
            break;
        case 6:
        case 8:
            if ((arg1 & 0x4000) && (arg2 <= 0.0f)) {
                speed = 2.0f;
                arg0->unk83 = 0xFF;
                arg0->disable_run = 0xFF;
                anim = 0x33;
            }
            break;
        case 9:
            anim = 0x22D;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
            break;
        case 10:
            if (arg0->unk28 <= 1.0f) {
                anim = 0x19A;
                arg0->unk83 = 0xFF;
            }
            arg0->disable_run = 0xC8;
            arg0->disable_jump = 0xC8;
            break;
        case 12:
            if (arg0->unk28 <= 1.0f) {
                anim = 0x2E7;
                arg0->unk83 = 0xFF;
            }
            arg0->disable_run = 0xC8;
            arg0->disable_jump = 0xC8;
            break;
        case 11:
            dx = arg0->x_position - -1884.0f;
            dz = arg0->z_position - 100.0f;
            dist = sqrtf(dx * dx + dz * dz);
            if (dist < 1000.0f) {
                u16 angle;

                angle = func_1505A630(-dx, dz, 0);
                func_1505A184(angle, 20.0f, 0.0f, &sp44, &sp40, &sp3C);
                func_1505A250(sp44, sp40, (1200.0f - dist) * 0.001000000047f, &arg0->unk16C, &arg0->unk170);
            }
            break;
        default:
            break;
        }
        arg0->unk31C->pad79[2] = 0;
    }
    if (arg0->unk31C->unk198 == 0 && anim == 0 && (arg1 & 0x4000) && (arg0->unk31C->unk4E & 0xF) != 1 &&
        arg2 <= 0.0f && arg0->disable_run != 0xFF) {
        if (arg0->unk13C >= 100) {
            if (!(arg0->unk25C & 0x20)) {
                anim = 0xD6;
                speed = 1.149999976f;
                if (D_800BE616 != 0) {
                    anim = 0x118;
                    speed = 1.0f;
                }
                if (D_800BE9F0 == 0x30) {
                    anim = 0x28D;
                }
                arg0->unk83 = 0xFF;
                arg0->disable_run = 0xFF;
            }
        } else if ((arg0->unk31C->unk75 == 0 && arg0->unk31C->pad79[3] == 0) || arg0->unk31C->unk4E == 2) {
            if (D_800BE9F0 != 0x2D && D_800BE9F0 != 0x2B && D_800BE9F0 != 0x30 && D_800BE9F0 != 0x34) {
                anim = 0x33;
                if ((s16)arg0->unk31C->unk8 != 0) {
                    arg0->unk83 = 0xFF;
                    arg0->disable_run = 0xFF;
                    speed = 1.600000024f;
                } else {
                    arg0->unk83 = 0xFF;
                    if (D_800BE616 == 0) {
                        anim = 0x1DC;
                    } else if (D_800E9D00 & 0x400) {
                        anim = 0x355;
                    } else if (D_800E9D00 & 0x800) {
                        anim = 0x1DC;
                    } else {
                        anim = 0x354;
                    }
                    speed = 1.350000024f;
                    if (arg0->unk31C->unk84 == 0 && arg0->unk31C->unk4E == 0) {
                        arg0->disable_run = 0xFF;
                        arg0->y_velocity = -4.0f;
                        arg0->unk76 = arg0->unk31C->unk4C;
                        speed = 1.0;
                        anim = 0x46;
                    }
                    arg0->unk80 = 1;
                    blend = 0.0f;
                }
            }
        }
    }
    if (anim != 0 && anim != 999 && arg3 == 0) {
        func_1505E650(arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
}

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

/* BEGIN func_15065A5C */
typedef struct {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ s16 unk8;
    /* 0x00A */ u8 padA[3];
    /* 0x00D */ u8 unkD;
    /* 0x00E */ u16 unkE;
    /* 0x010 */ u8 pad10[2];
    /* 0x012 */ s16 unk12;
    /* 0x014 */ u8 pad14[2];
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8 pad1A;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ u8 pad1C[7];
    /* 0x023 */ s8 unk23;
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8 pad26[2];
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ u8 unk30;
    /* 0x031 */ u8 unk31;
    /* 0x032 */ u8 pad32[0xA];
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 pad3D[0xF];
    /* 0x04C */ u16 unk4C;
    /* 0x04E */ u8 pad4E[0xA];
    /* 0x058 */ u8 unk58;
    /* 0x059 */ u8 pad59[0x19];
    /* 0x072 */ u16 unk72;
    /* 0x074 */ u8 pad74;
    /* 0x075 */ u8 unk75;
    /* 0x076 */ u8 pad76[2];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79;
    /* 0x07A */ u8 unk7A;
    /* 0x07B */ u8 pad7B[2];
    /* 0x07D */ u8 unk7D;
    /* 0x07E */ u16 unk7E;
    /* 0x080 */ s16 unk80;
    /* 0x082 */ s16 unk82;
    /* 0x084 */ u8 unk84;
    /* 0x085 */ u8 pad85[3];
    /* 0x088 */ u16 unk88;
    /* 0x08A */ u8 pad8A[0x90];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[0x11];
    /* 0x12C */ u16 unk12C;
    /* 0x12E */ u8 pad12E[0x42];
    /* 0x170 */ f32 unk170;
    /* 0x174 */ u8 pad174[0x23];
    /* 0x197 */ u8 unk197;
    /* 0x198 */ u8 unk198;
    /* 0x199 */ u8 unk199;
    /* 0x19A */ u8 unk19A;
    /* 0x19B */ u8 unk19B;
    /* 0x19C */ u16 unk19C;
    /* 0x19E */ u16 unk19E;
    /* 0x1A0 */ u8 pad1A0[2];
    /* 0x1A2 */ u16 unk1A2;
    /* 0x1A4 */ u16 unk1A4;
    /* 0x1A6 */ u8 pad1A6[4];
    /* 0x1AA */ s16 unk1AA;
    /* 0x1AC */ u8 pad1AC[6];
    /* 0x1B2 */ u8 unk1B2;
    /* 0x1B3 */ u8 pad1B3;
    /* 0x1B4 */ u8 unk1B4;
} Ctl15065A5C;

typedef struct {
    /* 0x0 */ u16 buttons;
    /* 0x2 */ s8 stick_x;
    /* 0x3 */ s8 stick_y;
} ContPad15065A5C;

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x18];
} Gate15065A5C;

extern u16 D_800996A0[][4];
extern u16 D_800996C0[];
extern u16 D_800996C8[];
extern u16 D_800996D0[];
extern u8 D_800C3A58;
extern s32 D_800CC290;
extern struct131 *D_800CC5EC;
extern void func_1000EC24();
extern void func_1000EFB4();
extern void func_151037DC();
s32 func_1000F568(s32 arg0, s32 arg1);
void func_1000FD38(void *, void *, s32);
void func_15073FA0(void);
struct216 *func_1507BB28(s32, s32);
void func_1507E7E4(void *, u8, u8, u16, s32);
void func_1507EABC(void *);
void func_1507EB4C(struct127 *, s32);
void func_15080828(s32 arg0);
void func_15080BE8(void);
s32 func_15080CF4(void);
void func_150D8B88(struct127 *);
s32 func_150DBD70(u8);
void func_150FC438(struct127 *, struct127 *, u8, u8);
void func_1514F568(struct127 *);
void func_151D4668(void *);
void func_151D469C(void *arg0, s32 a1, s32 a2, s32 a3, s32 a4);
void func_151D4D58(void *arg0);
s32 func_151F2CDC(void);

#define CTL ((Ctl15065A5C *)arg0->unk31C)
#define PAD ((ContPad15065A5C *)D_800CC284)
#define GATES (*(Gate15065A5C **)&D_800D3098)
#define OBJ_U8(o, off) (((u8 *)(o))[off])

s32 func_15065A5C(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 anim;
    f32 speed;
    f32 blend;
    f32 sp148;
    s32 sp144;
    s32 sp140;
    u8 sp13F;
    s32 sp138;

    sp144 = 1;
    sp140 = 0;
    sp13F = 0;
    sp138 = 0;
    speed = 1.0f;
    blend = 4.0f;
    sp148 = 0.0f;
    anim = 0;

    if ((CTL->unk11A == 3) || (arg0->stunned != 0) || (arg4 != 0)) {
        arg2 &= ~0x4000;
        D_800CC28C &= ~0x4000;
        if (arg4 != 0) {
            if (arg0->stunned == 0) {
                arg0->pad1C4 = 0;
            }
            arg2 &= ~0x2000;
            D_800CC28C &= ~0x2000;
        }
    }
    if (arg0->y_velocity > 0.0f) {
        arg3 += 50.0f;
    }

    switch (arg5) {
    case 0x2:
    case 0xA:
    case 0x13:
    case 0x34:
        {
            s32 idx;

            sp13F = 1;
            if (arg5 == 2) {
                idx = 0;
            } else if (arg5 == 0x13) {
                idx = 1;
            } else if (arg5 == 0xA) {
                idx = 2;
            } else {
                idx = 3;
            }
            if (arg6 != 0) {
                switch (arg5) {
                case 0x34:
                    func_150836CC(arg0, 0x3D);
                    CTL->unk11A = 0;
                    break;
                case 0xA:
                    func_150836CC(arg0, 0x28);
                    func_150836CC(arg0, 0x29);
                    CTL->unk11A = 0;
                    break;
                }
            }
            if (arg5 == CTL->unk78) {
                arg0->xz_velocity = 0.0f;
            }
            if ((arg0->in_water != 0) && ((arg5 == CTL->unk78) || (CTL->unk11A != 0))) {
                CTL->unk78 = 0;
                arg0->disable_run = 0;
                arg0->unk83 = 0;
                if (arg5 == 0xA) {
                    func_150836CC(arg0, 0x28);
                    func_150836CC(arg0, 0x29);
                    CTL->unk11A = 0;
                }
            } else if (((arg3 <= 0.0f) && (arg2 & 0x4000)) || (arg4 != 0)) {
                if ((CTL->unk78 != 0) || (arg4 != 0)) {
                    if ((CTL->unk72 == 0) || (arg4 != 0)) {
                        arg0->disable_run = 0xFF;
                        arg0->disable_jump = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = D_800996A0[idx][3];
                        CTL->unk11A = 3;
                        CTL->unk72 = 0x5A;
                        sp140 = 1;
                    }
                } else if (CTL->unk72 == 0) {
                    if ((arg0->camera->unk73C != 1) && (arg0->camera->unk73C != 3)) {
                        CTL->unk1B = 0;
                        CTL->unk11A = 1;
                        anim = D_800996A0[idx][0];
                        arg0->unk83 = 0xFF;
                        arg0->disable_run = 0xFF;
                        CTL->unk72 = 0x5A;
                    }
                }
                arg2 &= ~0x4000;
            }
            if (CTL->unk78 && (arg2 & 0x2000) && (anim == 0) && (arg0->unk84.uh == D_800996A0[idx][1])) {
                anim = D_800996A0[idx][2];
                CTL->unk1B++;
            }
            if (((arg0->unk84.uh == D_800996A0[idx][1]) || (arg0->unk84.uh == D_800996A0[idx][0]) ||
                 (arg0->unk84.uh == D_800996A0[idx][2]) || (anim != 0)) && (anim != D_800996A0[idx][3])) {
                CTL->unk78 = arg5;
                PAD->buttons &= ~0x2000;
                sp140 = 1;
            } else {
                CTL->unk78 = 0;
            }
            if (D_800C3A58 != 0) {
                if ((CTL->unk11A == 1) || (CTL->unk11A == 2U)) {
                    anim = D_800996A0[idx][1];
                    arg0->unk83 = 0xFF;
                    CTL->unk78 = arg5;
                    CTL->unk11A = 2;
                    switch (arg5) {
                    case 0xA:
                        func_15083568(arg0, 0x28, 1.0f, 0);
                        func_15083568(arg0, 0x29, 1.0f, 0);
                        break;
                    case 0x2:
                    case 0x13:
                        func_15083568(arg0, 8, 1.0f, 0);
                        break;
                    }
                } else {
                    CTL->unk11A = 0;
                    CTL->unk78 = 0;
                    switch (arg5) {
                    case 0x34:
                        func_150836CC(arg0, 0x3D);
                        break;
                    case 0xA:
                        func_150836CC(arg0, 0x28);
                        func_150836CC(arg0, 0x29);
                        break;
                    case 0x2:
                    case 0x13:
                        func_150836CC(arg0, 8);
                        break;
                    }
                }
            }
        }
        break;
    case 0x14:
    case 0x3F:
        {
            s32 animStart;
            s32 animEnd;
            s32 animHit;
            s32 animSlow;
            s32 animFast;
            s32 sfx;

            if (arg5 == 0x14) {
                animStart = 0x102;
                animEnd = 0x103;
                sfx = 0x28;
                animHit = 0x12B;
                animSlow = 0xF7;
                animFast = 0xC3;
            } else {
                animStart = 0x2CA;
                animEnd = 0x2CB;
                sfx = 0x2F;
                animHit = 0x2CE;
                animSlow = 0x2CC;
                animFast = 0x2CD;
            }
            anim = 999;
            if ((arg7 != 0) && (CTL->unk78 != 0)) {
                func_15083568(arg0, sfx, 1.0f, 0);
            }
            if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
                if (CTL->unk78 != 0) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->disable_jump = 0xFF;
                    CTL->unk11A = 3;
                    speed = 2.0f;
                    anim = animEnd;
                    CTL->unk78 = 0;
                } else {
                    arg0->disable_run = 0x10;
                    arg0->unk83 = 0x10;
                    arg0->disable_jump = 0x10;
                    speed = 2.0f;
                    CTL->unk11A = 1;
                    anim = animStart;
                    CTL->unk78 = arg5;
                }
            }
            if (anim != 999) {
                speed = 2.0f;
            } else if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, sfx);
                CTL->unk11A = 0;
            } else if (arg5 == CTL->unk78) {
                arg0->unkAA = 0x4B;
                if (arg6 != 0) {
                    sp144 = 0;
                } else if (CTL->unk197 != 0) {
                    arg0->disable_jump = 0xA;
                    arg0->disable_run = 0;
                    if (arg2 & 0x2000) {
                        if (CTL->unk72 == 0) {
                            arg0->unk83 = 0xF;
                            CTL->unk1B4 |= 3;
                            D_800CC2B0 = 1;
                            anim = animHit;
                            sp148 = 6.0f;
                            CTL->unk72 = 0x28;
                        }
                    }
                } else if ((arg2 & 0x2000) && (arg3 <= 0.0f) && (arg0->disable_run == 0) && (CTL->unk72 == 0)) {
                    CTL->unk72 = 0x28;
                    if (arg0->xz_velocity > 2.0f) {
                        anim = animFast;
                    } else {
                        anim = animSlow;
                    }
                    arg0->disable_run = 0xF;
                    arg0->unk83 = 0xF;
                    arg0->disable_jump = 0xF;
                }
            }
        }
        break;
    case 0x3:
        sp13F = 1;
        if ((arg4 != 0) || (arg6 != 0)) {
            if (D_800BE9F0 != 0xA) {
                CTL->unk78 = 0;
                CTL->unkD = 0;
                arg0->unk83 = 0;
            }
            if (arg6 != 0) {
                sp144 = 0;
            }
        } else if (CTL->unk16 == 0) {
            if (CTL->unk8 != 0) {
                CTL->unk8 -= D_800BE9A0;
                if (CTL->unk8 < 0x64) {
                    CTL->unkE = 0xAD;
                    CTL->unk16 = 0xC8;
                    func_1507EB4C(arg0, 0x2A);
                    speed = 1.3f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0x2D;
                    CTL->unk78 = 0;
                }
            }
            if (CTL->unk16 == 0) {
                if ((arg3 <= 0.0f) && (arg2 & 0x4000)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    if (CTL->unk78 != 0) {
                        anim = 0x2D;
                        speed = 1.3f;
                        CTL->unk78 = 0;
                    } else {
                        CTL->unk78 = 3;
                        anim = 0x2E;
                        CTL->unkD = 0;
                        speed = 1.6f;
                    }
                } else if ((CTL->unk78 == 3) && (arg0->disable_run != 0xFF)) {
                    CTL->unk78 = 3;
                    anim = 0x2C;
                    if (D_800BE616 != 0) {
                        arg0->disable_jump = 0xA;
                    }
                    D_800CC2B2 = PAD->stick_x * 0x96;
                    if (CTL->unk84 != 0) {
                        CTL->unk12 = 0;
                    } else if ((D_800CC2B2 < -0x1F40) && (CTL->unk12 < -0x1B58)) {
                        arg0->unk76 = arg0->unk7A = arg0->unk76 + (D_800CC264 * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    } else if ((D_800CC2B2 > 0x1F40) && (CTL->unk12 > 0x1B58)) {
                        arg0->unk76 = arg0->unk7A = arg0->unk76 - (D_800CC264 * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    }
                    D_800CC2B4 = 10.0f;
                    blend = 8.0f;
                    arg0->disable_run = 0x10;
                    arg0->unk83 = 0x10;
                    if (CTL->unkD == 0) {
                        CTL->unkD = 1;
                        func_150D8B88(arg0);
                    }
                }
            }
        }
        break;
    case 0x1:
    case 0x35:
        if ((arg2 & 0x4000) && ((arg3 <= 0.0f) || (arg5 == 0x35)) && (D_800CC5EC->unk4E != 1) &&
            (arg0->unk13C < 0x64) && (arg0->unkA9 == 0)) {
            arg0->unk81 = 1;
            arg0->unkA9 = 2;
            arg0->disable_run = 0x78;
            arg0->gravity = 6.0f;
            arg0->unk1CC = -100000.0f;
            anim = 0x4A;
            arg0->xz_velocity *= 0.5f;
            speed = 1.799999952f;
            arg0->y_velocity = 80.0f;
            CTL->unk31 = 0;
        }
        break;
    case 0x4:
        if (arg2 & 0x4000) {
            CTL->unk78 = arg5;
            return 0;
        }
        CTL->unk78 = 0;
        return 0;
    case 0x42:
        if (!(D_800D2E68 & 4)) {
            if (arg2 & 0x4000) {
                CTL->unk78 = arg5;
                return 0;
            }
            CTL->unk78 = 0;
        }
        return 0;
    case 0x5:
        if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
            if (arg0->unk84.uh == 0x2F) {
                anim = 999;
            } else {
                CTL->unk30 = 0x12;
                anim = 0x2F;
                if (D_800BE9F0 == 0x31) {
                    CTL->unk28 = GATES[12].x;
                    CTL->unk2C = GATES[12].z;
                    func_150DBD70(5);
                } else {
                    CTL->unk28 = GATES[2].x;
                    CTL->unk2C = GATES[2].z;
                    func_150DBD70(0);
                }
                arg0->unk76 = 0xC000;
                speed = 1.049999952f;
                blend = 6.0f;
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                sp138 = 0x143;
            }
        }
        break;
    case 0x6:
        if (CTL->unk78 == 0) {
            if ((arg2 & 0x4000) && (arg3 <= 0.0f) && (CTL->unk72 == 0)) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                anim = 0x44;
                CTL->unk78 = 6;
            }
        } else if (arg0->unk84.uh != 0x44) {
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            CTL->unk78 = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x36:
        if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
            struct126 *found;

            found = func_1503195C(arg0, 0x3C, 0);
            if ((found != NULL) && (found->unk38 < 0x12C)) {
                anim = 0x240;
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
            }
        }
        break;
    case 0x7:
        if (CTL->unk78 == 0) {
            if ((arg2 & 0x4000) && (arg3 <= 0.0f) && (CTL->unk72 == 0)) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                anim = 0x45;
                CTL->unk78 = 7;
            }
        } else if (arg0->unk84.uh != 0x45) {
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            CTL->unk78 = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x8:
        {
            struct127 *obj;

            obj = &D_800CC2D0[arg0->unk65 - 1];
            arg0->disable_jump = 0x14;
            if (arg2 & 0xC000) {
                if ((obj->unk84.uh == 0) || (*(u16 *)&obj->unk22E != 0) || (obj->unk84.uh == 0xC) || (obj->unk84.uh == 0xD)) {
                    u8 mode;
 mode = 0x10;
 D_800D154C = obj;
 if (arg2 & 0x4000) { mode = 0xE; } else { obj->unk84.uh = 0; }
                    obj->unk218 = func_1507BB28(1, mode);
                    obj->unk21C = 0;
                    D_800D154C = arg0;
                    obj->unk22E = 0;
                }
                anim = 999;
            }
        }
        break;
    case 0x27:
        do { } while (0);
        if (arg5 != CTL->unk78) {
            CTL->unk78 = 0x27;
            arg0->unk1C9 = 6;
            CTL->unk17 = 1;
        }
        break;
    case 0x9:
        if (D_800C3A58 != 0) {
            if ((CTL->unk11A == 1) || (CTL->unk11A == 2)) {
                func_15063404(arg0);
            } else {
                func_150634E4(arg0);
            }
        }
        if (arg5 == CTL->unk78) {
            {
                s16 delta;

                delta = (PAD->stick_x * -0x64) - CTL->unk80;
                if (delta > 0x258) {
                    delta = 0x258;
                }
                if (delta < -0x258) {
                    delta = -0x258;
                }
                CTL->unk80 += delta;
                delta = ((PAD->stick_y << 5) - CTL->unk82) + 0x240;
                if (delta > 0x12C) {
                    delta = 0x12C;
                }
                if (delta < -0x12C) {
                    delta = -0x12C;
                }
                CTL->unk82 += delta;
            }
            {
            u16 prev;

            if (((arg4 != 0) && ((arg0->unk65 != 0) || (arg0->stunned != 0) || (arg0->unk28 != 0.0f))) || (arg0->in_water != 0)) {
                func_150634E4(arg0);
            } else if (arg4 != 0) {
                speed = 1.399999976f;
                arg2 |= 0x4000;
                anim = func_15063E84(arg0, 0x7D, 1, 0, arg2);
                if (anim == 999) {
                    func_150634E4(arg0);
                }
            } else if (arg6 != 0) {
                sp144 = 0;
            } else {
                arg0->disable_jump = 0xA;
                if (CTL->unk7D != 0) {
                    u8 pad2;


                    if ((arg1 & 0x2000) && (arg0->unk48 < 0.6000000238f)) {

                        prev = CTL->unk7E;
                        CTL->unk7E = prev + ((D_800CC264 * 4) / 3);
                        if ((prev ^ CTL->unk7E) & 0x100) {
                            D_800D1580 = 0x13;
                            if (CTL->unk7E & 0x200) {
                                D_800D1580 = 0x14;
                            }
                            func_15071D38();
                            D_800D1580 = -0xFCF920;
                            func_1506E5FC();
                            func_10010630((func_150ADA20() & 1) + 0x2F5, arg0, 0x7D00, 0x1F4, 0x3E8);
                            if (!(func_150ADA20() & 7)) {
                                func_10010720(0x5CD, arg0, 0x5DC0, 0x1F4, 0x3E8, 0x3C);
                            }
                        }
                    } else {
                        pad2 = (CTL->unk7E * 2) | 1; CTL->unk7E = (CTL->unk7E & 0xFF00) | pad2;
                    }
                } else {
                    func_15063A38(arg0, arg1, 0x2010);
                    if ((arg0->unk84.uh != 0x11A) && (arg0->unk84.uh != 0x7B)) {
                        u8 sp10C;
                        sp10C = 0;
                        if (!(arg1 & 0x2000) && (CTL->unk88 & 0x2000)) {
                            sp10C = 1;
                        }
                        if ((CTL->unk24 == 0) || (arg2 & 0x8000)) {
                            anim = 0x11A;
                            if (!(arg2 & 0x8000)) {
                                sp10C = 1;
                            }
                            CTL->unk24 = 0x3C;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                        } else if ((arg1 & 0x2000) && (arg0->disable_run == 0)) {
                            if (arg0->xz_velocity != 0.0f) {
                                if (arg0->unk44 >= 20.0f) {
                                    anim = 0x238;
                                    speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                                } else {
                                    anim = 0x20D;
                                }
                                if (!(arg1 & 0x10)) {
                                    CTL->unk12 = 0;
                                    CTL->unk3C = 3;
                                } else {
                                    CTL->unk3C = 0;
                                }
                                if (D_800BE616 == 0) {
                                    arg0->unkAA = 0x37;
                                }
                            } else {
                                anim = 0x7C;
                            }
                            arg0->unk83 = 8;
                            if ((arg0->unk84.uh != 0x7C) && (arg0->unk84.uh != 0x20D) && (arg0->unk84.uh != 0x238)) {
                                func_151D4668(arg0);
                                func_1000FA64(0x2CF, arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, (s32)D_800996C0, 0, 0);
                                if (((arg0->id == 0x3BU) || (arg0->id == 0x80)) && !(func_150ADA20() & 3)) {
                                    s32 sound;

                                    if (arg0->id == 0x3B) {
                                        sound = func_1000F568(0x543, 4);
                                    } else {
                                        sound = func_1000F568(0x57C, 6);
                                    }
                                    func_1000FA64(sound, arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, (s32)D_800996C0, 0, 0);
                                }
                            }
                        } else if ((arg0->unk2EC != 0) && (D_800BE9F0 == 0x36)) {
                            if ((arg2 & 0x4000) && (arg0->unk2EC == 1) && (arg0->unk28 == 0.0f)) {
                                blend = 16.0f;
                                speed = 0.6999999881f;
                                anim = 0x20F;
                                arg0->unk76 = 0;
                                if (arg0->x_position > 0.0f) {
                                    arg0->unk76 = 0x8000;
                                    anim = 0x20E;
                                }
                                arg0->xz_velocity = 0.0f;
                                arg0->disable_run = 0xFF;
                                arg0->unk83 = 0xFF;
                                arg0->unk81 = 1;
                            }
                        } else {
                            speed = 1.399999976f;
                            anim = func_15063E84(arg0, 0x7D, 1, 0, arg2);
                        }
                        if (sp10C != 0) {
                            func_151D4D58(arg0);
                        }
                    }
                }
            }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x7B, 1, arg5, arg2);
            if (anim != 999) {
                CTL->unk24 = 0x3C;
            }
        }
        break;
    case 0x38:
        if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x7B);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else {
                u8 sp107;
                u16 pressed;
                u16 pad7;

                arg0->disable_jump = 0xA;
                func_15063A38(arg0, arg1, 0x2010);
                if (arg0->unk84.uh != 0x1DD) {
                    arg1 = arg1 | 0;
                    sp107 = 0;
                    if (!(arg1 & 0x2000) && (CTL->unk88 & 0x2000)) {
                        sp107 = 1;
                    }
                    if (arg1 & 0x2000) {
                        if (arg0->xz_velocity != 0.0f) {
                            if (arg0->unk44 >= 20.0f) {
                                anim = 0x254;
                                speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                            } else {
                                anim = 0x253;
                            }
                            if (!(arg1 & 0x10)) {
                                CTL->unk3C = 3;
                                CTL->unk12 = 0;
                            }
                        } else {
                            anim = 0x1E0;
                        }
                        blend = 0.0f;
                        arg0->unk83 = 8;
                        if ((arg0->unk84.uh != 0x1E0) && (arg0->unk84.uh != 0x253) && (arg0->unk84.uh != 0x254)) {
                            func_151D4668(arg0);
                            func_1000FA64(0x2CF, arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, (s32)D_800996C8, 0, 0);
                            if (((arg0->id == 0x3BU) || (arg0->id == 0x80)) && !(func_150ADA20() & 3)) {
                                s32 sound;

                                if (arg0->id == 0x3B) {
                                    sound = func_1000F568(0x543, 4);
                                } else {
                                    sound = func_1000F568(0x57C, 6);
                                }
                                func_1000FA64(sound, arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, (s32)D_800996C8, 0, 0);
                            }
                        }
                        CTL->unk1A4 += D_800BE9E4 * 0xA;
                        if (CTL->unk1A2 > 0xAF) {
                            CTL->unk1A2 = 0xAF;
                        }
                        CTL->unk1A2 += CTL->unk1A4 * D_800BE9E4;
                        if (CTL->unk1A2 > 0x1E00) {
                            CTL->unk1A2 = 0x1E00;
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = func_15063E84(arg0, 0x1DE, 1, 0, arg2);
                        CTL->unk1A2 = 0;
                        CTL->unk1A4 = 0;
                    }
                    if (sp107 != 0) {
                        func_151D469C(arg0, 5, 0x78, 0xFF, 1);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1DD, 1, arg5, arg2);
        }
        break;
    case 0x39:
        if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x82);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, arg1, 0x2010);
                if (arg0->unk84.uh != 0x1EE) {
                    if (arg0->unk84.uh == 0x21E) {
                        arg0->disable_run = 0xFF;
                    }
                    if ((CTL->unk24 == 0) || ((arg2 & 0x8000) && (arg0->unk84.uh != 0x21E))) {
                        if (arg0->unk28 == 0.0f) {
                            CTL->unk24 = 0x1E;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            anim = 0x21E;
                        }
                    } else if ((arg2 & 0x2000) && (arg0->unk84.uh != 0x1EE) && (arg0->unk84.uh != 0x21E)) {
                        if (arg0->xz_velocity == 0.0f) {
                            anim = 0x1EB;
                        } else {
                            anim = 999;
                            D_800D1580 = 0x53;
                            func_15071D38();
                        }
                        func_1000FD38(func_151037DC, arg0, arg0->unique_id);
                        func_1000FA64((func_150ADA20() & 1) + 0x2F3, arg0->x_position, arg0->y_position, arg0->z_position, 0x7D00, 0x5DC, 0x1F4, func_151037DC, arg0, (void *)arg0->unique_id, 0, 0);
                        if (arg0->camera != NULL) {
                            func_10010558((func_150ADA20() % 3U) + 0x2EB, arg0, 0x7D00, 0x64, 0x78, 0x1E);
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = func_15063E84(arg0, 0x1EF, 1, 0, arg2);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1EE, 1, arg5, arg2);
            if (anim != 999) {
                CTL->unk24 = 0x1E;
            }
        }
        break;
    case 0x37:
        if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x85);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, arg1, 0x2010);
                if ((arg2 & 0x2000) && (arg0->unk84.uh != 0x1F7) && (CTL->unk19E == 0)) {
                    if (func_150ADA20() & 3) {
                        anim = 0x1F4;
                    } else {
                        anim = 0x235;
                    }
                    blend = 0.0f;
                    CTL->unk19E = 0x78;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                } else if (arg2 & 0x4000) {
                    speed = 1.399999976f;
                    anim = func_15063E84(arg0, 0x1F8, 1, 0, arg2);
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1F7, 1, arg5, arg2);
        }
        break;
    case 0xB:
        if (arg0->unk84.uh == 0x68) {
            if ((arg0->unk2D0->unk8 < 68.0f) && (arg0->unk222 != 0)) {
                D_800CC2B2 = (arg0->unk7A - func_15063390(arg0)) - 0x2000;
            }
            anim = 999;
            D_800CC2B4 = 4.0f;
        } else if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
            speed = 1.200000048f;
            anim = 0x68;
            blend = 6.0f;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
        }
        break;
    case 0xC:
    case 0x16:
        anim = 999;
        if ((arg4 != 0) && (arg5 == 0x16)) {
            if (arg5 == CTL->unk78) {
                s32 cancel;

                cancel = 1;
                if ((s32)arg0->unk28 == 0) {
                    arg2 |= 0x4000;
                    anim = func_15063E84(arg0, 0x78, 1, 0, arg2);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    func_150836CC(arg0, 0x24);
                    CTL->unk11A = 0;
                    arg0->unk83 = 0;
                    arg0->disable_run = 0;
                }
            }
        } else if (arg5 == CTL->unk78) {
            arg0->disable_jump = 0xA;
            if ((arg4 != 0) && ((arg0->unk65 != 0) || ((s32)arg0->unk28 != 0))) {
                func_150836CC(arg0, 0x24);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                if ((arg5 == 0x16) || ((arg5 == 0xC) && (arg0->unk28 == 0.0f) && (arg0->health != 0))) {
                    sp144 = 0;
                } else {
                    func_150836CC(arg0, 0x24);
                    CTL->unk11A = 0;
                }
            } else if ((arg2 & 0x4000) || (arg4 != 0)) {
                arg2 |= 0x4000;
                anim = func_15063E84(arg0, 0x78, 1, 0, arg2);
                if ((anim == 999) && (arg4 != 0)) {
                    func_150836CC(arg0, 0x24);
                    CTL->unk11A = 0;
                    arg0->unk83 = 0;
                }
            } else if ((arg2 & 0x2000) && (arg0->unk84.uh != 0x76) && (CTL->unk19E == 0)) {
                if (CTL->unk1B2 != 0) {
                    anim = 0x2C7;
                } else if ((arg5 == 0xC) || (D_800BE9F0 == 0x2B) || (D_800BE616 == 0)) {
                    anim = 0x207;
                } else {
                    anim = 0x77;
                }
                blend = 0.0f;
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                if (CTL->unk197 != 0) {
                    if (arg0->camera != NULL) {
                        func_1512D2E4(arg0->camera, 0);
                    }
                }
                CTL->unk19E = 0x78;
            } else if ((CTL->unk197 != 0) && (arg0->camera != NULL) && (arg0->id == 0x80) && (func_10010894(arg0) == 0) && !(func_150ADA20() & 0x7F)) {
                func_15060A30(func_1000F568(0x568, 4), arg0);
            }
        } else {
            anim = func_15063E84(arg0, 0x76, 1, arg5, arg2);
        }
        if ((arg5 == 0xC) && ((arg5 == CTL->unk78) || (CTL->unk11A != 0))) {
            arg0->xz_velocity = 0.0f;
            if ((s32)arg0->unk28 != 0) {
                func_150836CC(arg0, 0x24);
                CTL->unk11A = 0;
                CTL->unk78 = 0;
            }
        }
        break;
    case 0x19:
    case 0x40:
        {
            s32 animStart;
            s32 animLoop;

            if (arg5 == 0x19) {
                animStart = 0x10E;
                animLoop = 0x110;
            } else {
                animStart = 0x2C2;
                animLoop = 0x2C3;
            }
            if (arg6 != 0) {
                sp144 = 0;
            } else if (arg5 == CTL->unk78) {
                if (CTL->unk11A == 2) {
                    if (arg2 & 0x2000) {
                        anim = func_150641D8(arg0, 0x113, 0x113, 0x119);
                        arg0->disable_run = 0xFF;
                    } else {
                        anim = func_15063E84(arg0, animLoop, 1, 0, arg2);
                    }
                }
            } else {
                anim = func_15063E84(arg0, animStart, 1, arg5, arg2);
            }
        }
        break;
    case 0xE:
        arg0->disable_jump = 0x14;
        anim = 999;
        break;
    case 0xF:
        anim = 999;
        if ((arg5 == CTL->unk78) && ((arg6 != 0) || (arg0->unk65 != 0) || (arg4 != 0))) {
            func_150836CC(arg0, 0x3A);
            CTL->unk11A = 0;
            arg0->disable_run = 0;
            arg0->unk83 = 0;
        } else if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
            anim = 0xB4;
            if (arg5 == CTL->unk78) {
                anim = 0xB6;
            }
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
            CTL->unk78 = arg5;
        }
        if (arg5 == CTL->unk78) {
            arg0->disable_jump = 0xA;
            arg0->xz_velocity = 0.0f;
        }
        break;
    case 0x10:
        speed = 1.399999976f;
        if (arg5 == CTL->unk78) {
            D_800CC2B2 = PAD->stick_x * 0xC8;
            arg0->disable_jump = 0xA;
            if ((arg1 & 0x2000) && (anim = 0xC1, (arg0->unk84.uh != 0xBE))) {
                speed = 1.0f;
            } else {
                anim = func_15063E84(arg0, 0xC0, 1, 0, arg2);
            }
        } else {
            anim = func_15063E84(arg0, 0xBE, 1, arg5, arg2);
        }
        break;
    case 0x15:
        if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x4B);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, arg1, 0x2010);
                if ((arg1 & 0x2000) && (arg0->unk84.uh != 0x168)) {
                    if (arg0->xz_velocity != 0.0f) {
                        arg0->unkAA = 0x50;
                        CTL->unk3C = 3;
                        CTL->unk12 = 0;
                        anim = 0x22B;
                        speed = (arg0->xz_velocity * 0.0625f) + 0.6000000238f;
                        if (arg0->unk44 >= 20.0f) {
                            speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                        }
                    } else {
                        anim = 0xE7;
                    }
                    arg0->disable_run = 0;
                    arg0->unk83 = 8;
                    if ((arg0->unk84.uh != 0xE7) && (arg0->unk84.uh != 0x22B)) {
                        s32 anims[2];

                        anims[0] = 0xE7;
                        anims[1] = 0x22B;
                        CTL->unk1AA++;
                        func_1506AC8C(arg0, 9, anims);
                    }
                } else {
                    anim = func_15063E84(arg0, 0xE4, 1, 0, arg2);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x168, 1, arg5, arg2);
        }
        break;
    case 0x18:
    case 0x41:
        {
            s32 animStart;
            s32 animAlt;

            if (arg5 == 0x18) {
                animStart = 0xFB;
                animAlt = 0xFC;
            } else {
                animStart = 0x2C8;
                animAlt = 0x2C9;
            }
            if (arg5 == CTL->unk78) {
                if (arg6 != 0) {
                    sp144 = 0;
                } else if ((CTL->unk197 != 0) && (CTL->unk11A == 2)) {
                    arg0->disable_jump = 0xA;
                    arg0->disable_run = 0;
                    arg0->unk83 = 0xA;
                    if ((PAD->buttons & 0x2000) || (CTL->unk88 & 0x2000)) {
                        CTL->unk1B4 |= 1;
                        D_800CC2B0 = 1;
                    }
                    if (animStart != arg0->unk84.uh) {
                        if (!(PAD->buttons & 0x2000) && (CTL->unk88 & 0x2000) && (CTL->unk19E == 0)) {
                            CTL->unk1B4 |= 2;
                            D_800CC2B0 = 1;
                            blend = 0.0f;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            anim = 0x10C;
                            CTL->unk19E = 0x3C;
                        } else if (arg0->unk84.uh != 0x10C) {
                            anim = 0x10A;
                        }
                    }
                } else {
                    arg0->disable_jump = 0x14;
                    if ((arg2 & 0x2000) && (animStart != arg0->unk84.uh) && (CTL->unk19E == 0)) {
                        anim = animAlt;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        CTL->unk19E = 0x3C;
                    } else {
                        anim = func_15063E84(arg0, 0xC6, 1, 0, arg2);
                    }
                }
            } else {
                anim = func_15063E84(arg0, animStart, 1, arg5, arg2);
            }
        }
        break;
    case 0x11:
        if (arg2 & 0x4000) {
            u8 pad5;

            func_15083E90(7)->unk2E4 = 1;
        }
        break;
    case 0x25:
    case 0x2D:
    case 0x2E:
        CTL->unk78 = arg5;
        arg0->disable_jump = 0x14;
        break;
    case 0x28:
        if (arg4 != 0) {
            CTL->unk78 = 0;
        } else if ((arg3 <= 0.0f) && (arg2 & 0x4000)) {
            if (arg5 != CTL->unk78) {
                CTL->unk78 = arg5;
            }
        }
        break;
    case 0x3C:
        {
            u16 before;
            u16 after;

            before = D_800CC280 - arg0->unk7A;
            anim = 999;
            arg0->unk239 = 0x2F;
            if (arg0->unk13C != 0) {
                s16 old;
                f32 mag;
                struct127 *obj;

                old = CTL->unk18;
                arg0->unk83 = 0xFF;
                arg0->disable_run = 0xFF;
                if (CTL->unk18 >= 0) {
                    CTL->unk18 -= 0x14;
                } else {
                    CTL->unk18 += 0x14;
                }
                if ((old ^ CTL->unk18) & 0x8000) {
                    CTL->unk18 = 0;
                }
                if (arg0->unk44 > 10.0f) {
                    s16 diff;

                    diff = CTL->unk4C - CTL->unk7E;
                    if ((diff < 0x2000) && (diff > -0x2000) && ((diff > 0x100) || (diff < -0x100))) {
                        if (diff & 0x8000) {
                            CTL->unk18 -= 0x28;
                        } else {
                            CTL->unk18 += 0x28;
                        }
                        if (CTL->unk18 > 0x1000) {
                            CTL->unk18 = 0x1000;
                        }
                        if (CTL->unk18 < -0x1000) {
                            CTL->unk18 = -0x1000;
                        }
                    }
                }
                arg0->unk76 = arg0->unk7A = arg0->unk7A + ((CTL->unk18 * D_800CC264) / 100);
                if ((CTL->unk18 > 0x200) || (CTL->unk18 < -0x200)) {
                    after = D_800CC280 - arg0->unk7A;
                    if ((((before < 0x1000) || (before > 0xF000)) && (after >= 0x1000) && (after < 0x7000) && (CTL->unk18 < 0)) ||
                        ((before > 0x7000) && (after <= 0x7000) && (CTL->unk18 > 0))) {
                        D_800D1580 = -0xFEFA4A;
                        func_1506E5FC();
                    }
                }
                CTL->unk7E = CTL->unk4C;
                anim = 0x288;
                mag = fabsf(CTL->unk18);
                if (mag > 200.0f) {
                    anim = 0x286;
                }
                if (mag > 2000.0f) {
                    anim = 0x27E;
                }
                blend = 16.0f;
                if (D_800BE9B4 != 0) {
                    blend = 0.0f;
                }
                if (arg2 & 0x4000) {
                    u8 pad6;

                    obj = func_15083E90(6);
                    obj->xz_velocity = 80.0f;
                    if (mag > 2000.0f) {
                        obj->xz_velocity = (mag + 1000.0f) / 36.0f;
                    } else {
                        obj->xz_velocity = (mag + 2000.0f) / 80.0f;
                    }
                    obj->stunned = 0;
                    obj->unk13D = 0;
                    obj->unk218 = NULL;
                    obj->unk232 = 2;
                    arg0->unk13C = 0;
                    arg0->unk83 = 0;
                    arg0->disable_run = 0xA;
                }
            } else if ((arg2 & 0x4000) && (arg3 <= 0.0f) && (arg0->disable_jump == 0)) {
                if (arg0->unk84.uh != 0x27D) {
                    if ((arg0->disable_run == 0) || (arg0->unk84.uh < 0x27A) || (arg0->unk84.uh >= 0x27E)) {
                        anim = 0x27A;
                    } else if ((arg0->unk103 != 0) || (arg0->unk107 != 0)) {
                        anim = arg0->unk84.uh + 1;
                    }
                    if (anim != 999) {
                        arg0->unk107 = 0;
                        arg0->unk83 = 0xFF;
                        arg0->disable_run = 0xFF;
                    }
                }
            }
        }
        break;
    case 0x21:
        if ((arg4 != 0) && (arg0->unk65 != 0)) {
            func_150836CC(arg0, 0x58);
            CTL->unk11A = 0;
        } else if (arg6 != 0) {
            sp144 = 0;
        } else if (arg3 <= 0.0f) {
            if (arg5 == CTL->unk78) {
                if (CTL->unk11A == 2) {
                    if (arg2 & 0x2000) {
                        anim = func_150641D8(arg0, 0x129, 0x12A, 0x128);
                        speed = 1.299999952f;
                        arg0->disable_run = 0xFF;
                    } else {
                        anim = func_15063E84(arg0, 0x121, 1, 0, arg2);
                        speed = 1.399999976f;
                    }
                }
            } else {
                anim = func_15063E84(arg0, 0x120, 1, arg5, arg2);
                speed = 1.399999976f;
            }
        }
        break;
    case 0x22:
        speed = 1.399999976f;
        if ((arg4 != 0) && (arg0->unk65 != 0)) {
            func_150836CC(arg0, 0x56);
            CTL->unk11A = 0;
        } else if (arg6 != 0) {
            sp144 = 0;
        } else if ((arg5 == CTL->unk78) && (arg3 <= 0.0f)) {
            if (CTL->unk11A == 2) {
                if (arg2 & 0x2000) {
                    anim = func_150641D8(arg0, 0x15F, 0x15F, 0x15F);
                    speed = 1.299999952f;
                    arg0->disable_run = 0xFF;
                } else {
                    anim = func_15063E84(arg0, 0x15E, 1, 0, arg2);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x15D, 1, arg5, arg2);
        }
        break;
    case 0x24:
        if (arg4 != 0) {
            if (arg5 == CTL->unk78) {
                s32 cancel;

                cancel = 1;
                if (((s32)arg0->unk28 == 0) && (CTL->unk11A == 2)) {
                    arg2 |= 0x4000;
                    anim = func_15063E84(arg0, 0x155, 1, 0, arg2);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    func_150836CC(arg0, 0x63);
                    CTL->unk11A = 0;
                    arg0->unk83 = 0;
                    arg0->disable_run = 0;
                }
            }
        } else if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x63);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else if ((CTL->unk197 != 0) && (CTL->unk11A == 2) && (arg4 == 0)) {
                arg0->disable_jump = 0xA;
                arg0->disable_run = 0;
                arg0->unk83 = 0xA;
                if ((CTL->unk19E == 0) && ((PAD->buttons & 0x2000) || (CTL->unk88 & 0x2000))) {
                    CTL->unk1B4 |= 1;
                    D_800CC2B0 = 1;
                }
                if (arg0->unk84.uh != 0x152) {
                    if (!(PAD->buttons & 0x2000) && (CTL->unk88 & 0x2000) && (arg0->unk84.uh == 0x157) && (CTL->unk19E == 0)) {
                        CTL->unk1B4 |= 2;
                        D_800CC2B0 = 1;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        CTL->unk19E = 0x78;
                        anim = 0x156;
                        if ((arg0->camera != NULL) && (((arg0->unk184 & 0x1F) == 5) || ((arg0->unk184 & 0x1F) == 3) ||
                                                       ((arg0->unk184 & 0x1F) == 6) || ((arg0->unk184 & 0x1F) == 9))) {
                            func_10010558((func_150ADA20() % 3U) + 0x2EB, arg0, 0x7D00, 0x64, 0x78, 0x6E);
                        }
                    } else if (arg0->unk84.uh != 0x156) {
                        anim = 0x157;
                    }
                }
            } else {
                arg0->disable_jump = 0x14;
                if (arg4 != 0) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0x155;
                    if ((s32)arg0->unk28 != 0) {
                        func_150836CC(arg0, 0x63);
                        CTL->unk11A = 0;
                    }
                } else if ((arg2 & 0x2000) && (arg0->unk84.uh != 0x152) && (CTL->unk19E == 0)) {
                    anim = 0x154;
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    CTL->unk19E = 0x78;
                } else {
                    anim = func_15063E84(arg0, 0x155, 1, 0, arg2);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x152, 1, arg5, arg2);
        }
        break;
    case 0x3B:
        {
            s32 held;

            if (D_800C3A58 != 0) {
                if (arg5 == CTL->unk78) {
                    func_15083568(arg0, 0x89, 1.0f, 0);
                    CTL->unk11A = 2;
                } else {
                    func_150836CC(arg0, 0x89);
                    CTL->unk11A = 0;
                }
            }
            if (((arg5 == CTL->unk78) || (CTL->unk11A != 0)) && (arg0->in_water != 0)) {
                CTL->unk78 = 0;
                arg0->disable_run = 0;
                arg0->unk83 = 0;
                func_150836CC(arg0, 0x89);
                CTL->unk11A = 0;
            } else if (arg5 == CTL->unk78) {
                if ((arg4 != 0) && ((arg0->unk65 != 0) || (arg0->stunned != 0) || (arg0->unk13C != 0))) {
                    func_150836CC(arg0, 0x89);
                    CTL->unk11A = 0;
                    CTL->unk78 = 0;
                } else if (arg4 != 0) {
                    arg2 |= 0x4000;
                    anim = func_15063E84(arg0, 0x226, 1, 0, arg2);
                    if (anim == 999) {
                        func_150836CC(arg0, 0x89);
                        CTL->unk11A = 0;
                        CTL->unk78 = 0;
                        arg0->unk83 = 0;
                    }
                } else if (arg6 != 0) {
                    sp144 = 0;
                } else {
                    arg0->disable_jump = 0x14;
                    func_15063A38(arg0, arg1, 0x2010);
                    if ((CTL->unk19E == 0) && ((D_800CC290 & 0x2000) || (CTL->unk88 & 0x2000))) {
                        if (CTL->unk198 == 2) {
                            CTL->unk1B4 |= 4;
                            D_800CC2B0 = 1;
                        } else {
                            CTL->unk1B4 |= 1;
                            D_800CC2B0 = 1;
                        }
                    }
                    if (arg4 != 0) {
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = 0x226;
                        if ((s32)arg0->unk28 != 0) {
                            func_150836CC(arg0, 0x89);
                            CTL->unk11A = 0;
                        }
                    } else {
                        held = D_800CC290 & 0x10;
                        if ((((held != 0) && !(D_800CC290 & 0x2000) && (CTL->unk88 & 0x2000)) ||
                                    ((held == 0) && (D_800CC28C & 0x2000))) &&
                                   (arg0->unk84.uh != 0x225) && (arg0->unk84.uh != 0x222) && (arg0->unk84.uh != 0x236) &&
                                   (arg0->unk84.uh != 0x227) && (arg0->unk84.uh != 0x237) && (CTL->unk19E == 0)) {
                            if (PAD->buttons & 0x10) {
                                if (func_150ADA20() & 3) {
                                    anim = 0x227;
                                } else {
                                    anim = 0x237;
                                }
                                CTL->unk1B4 |= 2;
                                D_800CC2B0 = 1;
                            } else {
                                if (func_150ADA20() & 3) {
                                    anim = 0x222;
                                } else {
                                    anim = 0x236;
                                }
                            }
                            blend = 0.0f;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            CTL->unk19E = 0x3C;
                        } else if ((held != 0) && (D_800CC290 & 0x2000) && (arg0->unk84.uh != 0x225)) {
                            arg0->disable_run = 0;
                            arg0->disable_jump = 8;
                            arg0->unk83 = 8;
                            if ((arg0->unk84.uh != 0x222) && (arg0->unk84.uh != 0x236) && (arg0->unk84.uh != 0x227) && (arg0->unk84.uh != 0x237)) {
                                if ((arg0->unk44 >= 1.0f) || (CTL->unk23 > 0)) {
                                    anim = 0x223;
                                    if (arg0->unk44 >= 20.0f) {
                                        anim = 0x31B;
                                        speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                                    }
                                } else {
                                    anim = 0x221;
                                }
                            }
                        } else {
                            anim = func_15063E84(arg0, 0x226, 1, 0, arg2);
                        }
                    }
                }
            } else {
                anim = func_15063E84(arg0, 0x225, 1, arg5, arg2);
            }
        }
        break;
    case 0x12:
        if (((arg5 == CTL->unk78) || (CTL->unk11A != 0)) && (arg0->in_water != 0)) {
            CTL->unk78 = 0;
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            CTL->unk11A = 0;
        } else if (arg5 == CTL->unk78) {
            arg0->xz_velocity = 0.0f;
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                CTL->unk11A = 0;
            } else {
                arg0->disable_jump = 0x14;
                if ((PAD->buttons & 0x2000) || (CTL->unk88 & 0x2000)) {
                    if (CTL->unk198 == 2) {
                        CTL->unk1B4 |= 8;
                        D_800CC2B0 = 1;
                    } else {
                        CTL->unk1B4 |= 1;
                        D_800CC2B0 = 1;
                    }
                }
                if (arg4 != 0) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0xC6;
                    if ((s32)arg0->unk28 != 0) {
                        CTL->unk11A = 0;
                    }
                } else if (!(PAD->buttons & 0x2000) && (CTL->unk88 & 0x2000) && (arg0->unk84.uh != 0xC4) && (CTL->unk19E == 0)) {
                    anim = 0xC7;
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    CTL->unk19E = 0x3C;
                } else if ((PAD->buttons & 0x2000) && (arg0->unk84.uh != 0xC4)) {
                    arg0->disable_run = 0;
                    arg0->disable_jump = 8;
                    arg0->unk83 = 8;
                    if (arg0->unk84.uh != 0xC7) {
                        anim = 0x10A;
                    }
                } else {
                    anim = func_15063E84(arg0, 0xC6, 1, 0, arg2);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0xC4, 1, arg5, arg2);
            if (anim != 999) {
                arg0->xz_velocity = 0.0f;
            }
        }
        break;
    case 0x23:
        if (arg5 == CTL->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x16);
                CTL->unk11A = 0;
            } else if (arg6 != 0) {
                sp144 = 0;
            } else if ((CTL->unk197 != 0) && (CTL->unk11A == 2)) {
                arg0->disable_jump = 0xA;
                arg0->disable_run = 0;
                arg0->unk83 = 0xA;
                if ((CTL->unk19E == 0) && ((PAD->buttons & 0x2000) || (CTL->unk88 & 0x2000))) {
                    CTL->unk1B4 |= 1;
                    D_800CC2B0 = 1;
                }
                if (arg0->unk84.uh != 0x163) {
                    if ((CTL->unk199 >= 6) || ((arg2 & 0x8000) && (arg0->unk84.uh != 0x26E))) {
                        if (arg0->unk28 == 0.0f) {
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            CTL->unk19E = 0xB4;
                            anim = 0x26E;
                            CTL->unk199 = 0;
                        }
                    } else if (!(PAD->buttons & 0x2000) && (CTL->unk88 & 0x2000) && (arg0->unk84.uh == 0x165) && (CTL->unk19E == 0)) {
                        CTL->unk1B4 |= 2;
                        D_800CC2B0 = 1;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = 0x167;
                        CTL->unk199++;
                        CTL->unk19E = 0x1E;
                    } else if (arg0->unk84.uh != 0x167) {
                        anim = 0x165;
                    }
                }
            } else {
                arg0->disable_jump = 0x14;
                if ((arg0->unk84.uh != 0x163) && ((CTL->unk199 >= 6) || ((arg2 & 0x8000) && (arg0->unk84.uh != 0x26E)))) {
                    if (arg0->unk28 == 0.0f) {
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        CTL->unk19E = 0xB4;
                        anim = 0x26E;
                        CTL->unk199 = 0;
                    }
                } else if ((arg2 & 0x2000) && (arg0->unk84.uh != 0x163) && (arg0->unk84.uh != 0x166) && (CTL->unk19E == 0)) {
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0x166;
                    CTL->unk199++;
                    CTL->unk19E = 0x1E;
                } else {
                    anim = func_15063E84(arg0, 0x164, 1, 0, arg2);
                    if (anim == 0x163) {
                        CTL->unk199 = 0;
                    }
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x163, 1, arg5, arg2);
        }
        break;
    case 0x17:
        anim = 999;
        if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
            s32 pad3;
            s32 ok;
            f32 angle;

            ok = 1;
            if (CTL->unk7A != 0) {
                angle = func_15048A70(GATES[(s32)CTL->unk7A].unk10, arg0->unk40);
                if ((GATES[(s32)CTL->unk7A].unk18 != 0) && ((angle < -90.0f) || (angle > 90.0f))) {
                    ok = 0;
                }
            }
            if (ok != 0) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                CTL->unk78 = 0x17;
                anim = 0x8E;
                arg0->unk76 = (u32)(GATES[(s32)CTL->unk7A].unk10 * 182.0444489f) - 0x4000;
            }
        }
        break;
    case 0x1A:
        sp13F = 1;
        anim = 999;
        if (arg5 != CTL->unk78) {
            if ((arg2 & 0x4000) && (arg3 <= 0.0f)) {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                arg0->xz_velocity = 0.0f;
                anim = 0x12C;
                CTL->unk78 = arg5;
                func_15080828(0);
            }
        } else if (arg6 != 0) {
            func_15080BE8();
            CTL->unk78 = 0;
        } else if (func_15080CF4() != 0) {
            if (D_800D1940 == 0x42) {
                CTL->unk78 = 0;
                anim = 0x358;
                D_800D2E60[D_800D1940 >> 3] |= 1 << (D_800D1940 & 7);
            } else {
                anim = 0x12E;
            }
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
        } else if ((arg0->unk84.uh != 0x12C) && (D_800BE710[D_800C3E78] & 0x8000) && (!(D_800D1958[0] & 0x20) || (func_151F2CDC() == 0))) {
            anim = 0x142;
        }
        break;
    case 0x2A:
        sp13F = 1;
        anim = 999;
        if (arg5 != CTL->unk78) {
            if ((arg3 <= 0.0f) && (OBJ_U8(arg0, 0x110) == 0) && (arg0->disable_jump == 0)) {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                arg0->xz_velocity = 0.0f;
                CTL->unk78 = arg5;
                anim = 0x26F;
                arg0->unk276 = 6;
                arg0->unk282 = 0x64;
                arg0->unk278 = 0;
                arg0->unk284 = 0;
                func_15080828(1);
            }
        } else if (arg6 != 0) {
            func_15080BE8();
            CTL->unk78 = 0;
        } else if (func_15080CF4() != 0) {
            CTL->unk78 = 0;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0;
            func_1507EABC(arg0);
        } else {
            s32 sounds[2] = { 0x39, 0x35 };

            arg0->unk282 = 0x64;
            if ((s8)arg0->unk284 == 0) {
                arg0->unk278 = (u16)arg0->unk278 + D_800BE9E4;
                if (((func_150ADA20() % 120U) + 0xB4) < (u16)arg0->unk278) {
                    func_1507E7E4(arg0, (u8)sounds[func_150ADA20() & 1], 1, (func_150ADA20() % 180U) + 0x1A4, 0);
                    arg0->unk278 = 0;
                    arg0->unk284 = 1;
                }
            } else if (((s8)arg0->unk284 != 0) && (OBJ_U8(arg0, 0x70) == OBJ_U8(arg0, 0x6F))) {
                arg0->unk284 = 0;
            }
            arg0->disable_run = 0xA;
            arg0->disable_jump = 0xA;
            arg0->unk83 = 0xA;
        }
        break;
    case 0x1B:
        {
            struct127 *obj;
            s32 animHold;
            s32 animIdle;
            s32 animRelease;
            s32 animGrab;
            s32 animSpecial;
            s32 next;

            obj = &D_800CC2D0[arg0->unk65 - 1];
            CTL->unk78 = 0x1B;
            arg0->disable_jump = 0x14;
            if ((D_800BE616 == 0) || (arg0->unk65 != 0)) {
                sp144 = 0;
            }
            if (CTL->unk19E == 0) {
                next = 0;
                if (obj->id == 0x23) {
                    animHold = 0xD;
                    animIdle = 0x10;
                    animRelease = 0x15;
                    animGrab = 0xA;
                    animSpecial = 0xC;
                } else {
                    animHold = 4;
                    animIdle = 0xB;
                    animRelease = 0xC;
                    animGrab = 1;
                    animSpecial = 0;
                }
                if (PAD->buttons & 0x2000) {
                    if (animHold != obj->unk232) {
                        func_10010A3C(obj);
                        func_100109D0(obj);
                    }
                    obj->unk232 = animHold;
                    obj->unk218 = NULL;
                    CTL->unk199 = 1;
                    if ((animGrab != obj->unk84.uh) || ((D_800C3A58 != 0) && (animSpecial != 0))) {
                        func_150FC438(obj, arg0, 0, animGrab);
                        func_10010A3C(obj);
                        if ((arg0->id == 0x80) && !(func_150ADA20() & 3)) {
                            func_1000FA64(func_1000F568(0x57C, 6), arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, D_800996D0, 0, 0);
                        }
                    }
                    if (D_800BE9E4 < CTL->unk24) {
                        CTL->unk24 -= D_800BE9E4;
                    } else {
                        next = animRelease;
                        func_100109D0(obj);
                    }
                } else if ((D_800C3A58 != 0) && (animSpecial != obj->unk232) && (animSpecial != 0)) {
                    obj->unk232 = animSpecial;
                    obj->unk218 = NULL;
                } else if (CTL->unk199 == 1) {
                    CTL->unk199 = 0;
                    if (func_1000FF90(func_1000EC24, -1, -1) == -1) {
                        func_1000FA64((func_150ADA20() % 3U) + 0x618, arg0->x_position, arg0->y_position, arg0->z_position, 0x7D00, 0x1F4, 0xC8, func_1000EC24, (void *)0x1E, 0, 0x300, 0);
                    }
                }
                if (arg2 & 0x4000) {
                    next = animIdle;
                    func_10010A3C(obj);
                    func_100109D0(obj);
                }
                if (next != 0) {
                    CTL->unk24 = 0x258;
                    CTL->unk19E = 0x96;
                    func_1000FA64((func_150ADA20() % 3U) + 0x618, arg0->x_position, arg0->y_position, arg0->z_position, 0x7D00, 0x1F4, 0xC8, func_1000EC24, (void *)0x1E, 0, 0x300, 0);
                    obj->unk232 = next;
                    obj->unk218 = NULL;
                }
            }
            anim = 999;
        }
        break;
    case 0x26:
        {
            s32 animThrow;
            s32 animDrop;
            s32 item;
            s32 sfx;

            animThrow = 0x176;
            if ((arg0->id == 0x75) || (animDrop = 0x16C, (arg0->id == 0xB1))) {
                animDrop = 0x16B;
                item = 0x64;
                sfx = 0x11;
            } else {
                animThrow = 0x175;
                item = 0x2C;
                sfx = 0x12;
            }
            if (arg6 != 0) {
                if ((arg5 == CTL->unk78) || (CTL->unk19B == 4)) {
                    if ((func_1503195C(arg0, item, 0) != NULL) || (CTL->unk19B == 4)) {
                        func_150836CC(arg0, item);
                        D_800D1580 = (u16)sfx | 0x10000;
                        func_15073FA0();
                        if (CTL->unk19A != 0) {
                            CTL->unk19A--;
                        }
                    }
                    CTL->unk19B = 0;
                    CTL->unk11A = 0;
                }
            } else {
                func_15063A38(arg0, arg1, 0x10);
                if (CTL->unk19B == 1) {
                    if (arg5 != CTL->unk78) {
                        CTL->unk78 = arg5;
                        CTL->unk11A = 1;
                        arg0->disable_run = 0x1E;
                        arg0->disable_jump = 0x1E;
                        arg0->unk83 = 0x1E;
                        CTL->unk19B = 2;
                        anim = animThrow;
                        CTL->unk19C = 0xF0;
                        CTL->unk1AA++;
                    }
                } else if (CTL->unk19B == 2) {
                    if (!(D_800CC290 & 4) && !(arg0->unk2F8 & 0x10)) {
                        CTL->unk19B = 3;
                    }
                    if (D_800BE9E4 < CTL->unk19C) {
                        CTL->unk19C -= D_800BE9E4;
                    } else {
                        CTL->unk19B = 0;
                        CTL->unk78 = 0;
                        arg0->immune = 0;
                        func_15063B64(arg0);
                    }
                } else if ((CTL->unk19B == 3) && (arg3 <= 0.0f) && (animThrow != arg0->unk84.uh)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    CTL->unk19B = 4;
                    anim = animDrop;
                    CTL->unk78 = 0;
                    CTL->unk11A = 3;
                    if (CTL->unk19A != 0) {
                        CTL->unk19A--;
                    }
                }
            }
        }
        break;
    case 0x3A:
        {
            struct127 *obj;
            s32 item;

            if (D_800BE9F0 == 0x3F) {
                item = 0xBA;
            } else {
                item = 0x89;
            }
            if (arg6 != 0) {
                if (arg5 == CTL->unk78) {
                    obj = func_15063C60(arg0, item);
                    if (obj != NULL) {
                        obj->unk65 = 0;
                        obj->gravity = 4.0f;
                        obj->y_velocity = 0.0f;
                        obj->xz_velocity = 0.0f;
                    }
                    CTL->unk19B = 0;
                    CTL->unk78 = 0;
                    CTL->unk11A = 0;
                }
            } else {
                func_15063A38(arg0, arg1, 0x10);
                if (CTL->unk19B == 5) {
                    if (arg5 != CTL->unk78) {
                        if (item == 0x89) {
                            obj = func_15083568(arg0, 0x86, 1.0f, 0);
                        } else {
                            obj = func_15083568(arg0, 0x9F, 1.0f, 0);
                        }
                        if (obj != NULL) {
                            CTL->unk78 = arg5;
                            anim = 0x1F9;
                            CTL->unk11A = 1;
                            arg0->disable_run = 0x1E;
                            arg0->disable_jump = 0xFF;
                            arg0->unk83 = 0xFF;
                            CTL->unk19B = 6;
                            obj->immune = 0xFF;
                            func_15062BDC(obj, 1.0f, 1.0f);
                            obj->unk2E4 = 0xF0;
                            obj->unk2E8 = 0xF0;
                            obj->unk124 = D_800C3E78 + 1;
                            obj->unkF8 = 0x401807;
                            func_1505E650(obj, 2, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            func_1514F568(obj);
                        } else {
                            CTL->unk19B = 0;
                        }
                    }
                } else if (CTL->unk19B == 6) {
                    if (!(D_800CC290 & 4)) {
                        CTL->unk19B = 7;
                    }
                } else if ((CTL->unk19B == 7) && (arg3 <= 0.0f) && (arg0->unk84.uh != 0x1F9)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    CTL->unk19B = 0;
                    anim = 0x1FC;
                    CTL->unk78 = 0;
                    CTL->unk11A = 0;
                    if (CTL->unk19A != 0) {
                        CTL->unk19A--;
                    }
                    CTL->unk1AA++;
                    obj = func_15063C60(arg0, item);
                    if (obj != NULL) {
                        s16 pitch;
                        f32 vx;
                        f32 vy;
                        f32 vz;
                        f32 dist;

                        obj->unk65 = 0;
                        dist = ((f32)(0xF0 - obj->unk2E4) * 210.0f) / 240.0f;
                        dist += 40.0f;
                        if (CTL->unk198 != 0) {
                            pitch = (30.0f - CTL->unk170) * 182.0444489f;
                        } else {
                            pitch = 0x1555;
                        }
                        func_1505A184(pitch, dist, 0.0f, &vx, &vy, &vz);
                        obj->y_velocity = -vy;
                        obj->xz_velocity = vx;
                        obj->gravity = 4.0f;
                        obj->unk76 = obj->unk7A = arg0->unk7A - CTL->unk12;
                    }
                }
            }
        }
        break;
    case 0x3E:
        if (OBJ_U8(arg0, 0x128) == 0) {
            if (arg0->unk83 == 0) {
                arg0->unk83 = 1;
            }
            if (CTL->unk12C >= D_800BE9E4) {
                CTL->unk12C -= D_800BE9E4;
                arg0->unkAA = 0x96;
            } else {
                arg0->unkAA = 0x4B;
                CTL->unk12C = 0;
            }
        }
        break;
    }

    if ((arg4 != 0) || (arg6 != 0)) {
        CTL->unk72 = 0;
        if (arg6 != 0) {
            anim = 0;
            sp13F = 1;
            if (sp144 != 0) {
                CTL->unk78 = 0;
            }
        } else {
            CTL->unk75 = 0;
            CTL->unk78 = 0;
            if (CTL->unk11A != 3) {
                CTL->unk11A = 0;
            }
        }
    }
    if ((anim != 0) && (anim != 999) && (arg8 == 0)) {
        func_1505E650(arg0, anim, speed, blend, sp148, 0.0f, 0);
    }
    if (sp140 != 0) {
        CTL->unk58 = 1;
    }
    if (sp138 != 0) {
        func_15060A30(sp138, arg0);
    }
    if (sp13F != 0) {
        return 1000;
    } else {
        return anim;
    }
}
