#include <ultra64.h>
// matching: this file calls func_100109D0/func_10010A3C (void (Actor *)) through unprototyped views
#define func_100109D0 func_100109D0_hdr
#define func_10010A3C func_10010A3C_hdr
#include "functions.h"
#undef func_100109D0
s32 func_100109D0();
#undef func_10010A3C
s32 func_10010A3C();
#include "variables.h"

extern s16 D_800CC2B2;
extern f32 D_800CC2B4;
void func_15143134(void *, f32 *, s32);
void func_1506AC8C(Actor *, s32, void *);

/* Packet handed to func_1506AC8C by func_15063B64. */
typedef struct ThrowPacket {
    Actor *thrower;
    f32 position[3];
    s32 throwerId;          /* object index + 1 */
    volatile s32 anim;      // matching: volatile keeps the anim in its stack slot across the call
} ThrowPacket;

/* Returns the angle from this object to the object it is linked to (unk222). */
s16 func_15063390(Actor *arg0) {
    Actor *other;

    other = &D_800CC2D0[arg0->unk222];
    return func_1505A630(other->x_position - arg0->x_position, arg0->z_position - other->z_position, 0);
}

void func_15063404(Actor *arg0) {
    Actor *self = &D_800CC2D0[arg0 - D_800CC2D0];

    self->unk31C->unk78 = 9;
    func_15083568(arg0, 0x1D, 1.0f, 0);
    func_15083568(arg0, 0x1E, 1.0f, 0);
    arg0->disable_jump = 20;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
    self->unk31C->unk24 = 60;
    self->unk31C->unk11A = 2;
    func_1505E650(arg0, 0x7F, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

void func_150836CC(Actor *arg0, s32 arg1);

void func_150634E4(Actor *arg0) {
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk78 = 0;
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk11A = 0;
    func_150836CC(arg0, 0x1D);
    func_150836CC(arg0, 0x1E);
    arg0->disable_jump = 0;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
}

void func_15063570(Actor *arg0) {
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk78 = 0x3B;
    D_800CC2D0[arg0 - D_800CC2D0].unk31C->unk11A = 2;
    func_15083568(arg0, 0x89, 1.0f, 0);
    arg0->disable_jump = 20;
    arg0->disable_run = 0;
    arg0->unk83 = 0;
    func_1505E650(arg0, 0x221, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

void func_15081690(Actor *, f32, f32, f32, f32, f32, f32, Actor **, f32, s32, s32, s32, s32, s32, s32);
void func_150636A4(Actor *arg0);

void func_15063628(Actor *arg0, f32 arg1) {
    ActorCtl *ctl = arg0->unk31C;

    func_15081690(arg0, ctl->aimEye.x, ctl->aimEye.y, ctl->aimEye.z,
                  ctl->aimDir.x, ctl->aimDir.y, ctl->aimDir.z, &ctl->unkB0,
                  arg1, 0, 0, 0, -1, 0, 0);
    func_150636A4(arg0);
}

/* Tells the object in unkB0 (if any) that this object is targeting it. */
void func_150636A4(Actor *arg0) {
    Actor *target;
    ActorCtl *targetCtl;

    target = arg0->unk31C->unkB0;
    if (target != NULL) {
        targetCtl = target->unk31C;  // matching: the null test uses this copy; the stores reload unk31C
        if (targetCtl != NULL) {
            target->unk31C->unk195 = 30;
            target->unk31C->unk196 = arg0 - D_800CC2D0;
        }
    }
}

void func_15063FA0(Actor *, s32);
void func_151D57F8(Actor *, s32);
s32 func_15081E78(Actor *, Actor **, s32);
void func_10010558(u16, void *, s32, s32, s32, s32);
void func_150FFCC8(void *, void *, void *, void *, void *);
void func_150FFD84(f32 *, f32 *, s32, s32);
void func_1512D2E4(void *, s32);

/* Runs the work each object queued in its control block's `pending` flags this frame. */
void func_150636F0(void) {
    Actor *actor;
    s32 index;
    s32 action;
    f32 position[3];
    f32 direction[3];
    f32 side[3];
    f32 up[3];
    ActorCtl *ctl;
    Camera *camera;

    D_800CC2B0 = 0;
    actor = D_800CC2D0;
    for (index = 0; index < 25; index++, actor++) {
        if ((actor->interaction_state != 0) && (actor->unk31C != NULL) && (actor->unk31C->unk1B4 != 0)) {
            D_800C3E78 = index;
            D_800D154C = actor;
            ctl = actor->unk31C;
            if (ctl->unk1B4 & 1) {
                func_15063628(actor, 0.0f);
                ctl = actor->unk31C;
            }
            if (ctl->unk1B4 & 4) {
                func_151D57F8(actor, 0);
                ctl = actor->unk31C;
            }
            if (ctl->unk1B4 & 8) {
                func_151D57F8(actor, 1);
                ctl = actor->unk31C;
            }
            if (ctl->unk1B4 & 2) {
                action = ctl->unk78 & ~0x80;
                switch (action) {
                case 0x18:
                case 0x41:
                    func_15063FA0(actor, 6);
                    ctl = actor->unk31C;
                    ctl->unk1AA++;
                    camera = actor->camera;
                    if (camera != NULL) {
                        func_1512D2E4(camera, 1);
                    }
                    break;
                case 0x14:
                case 0x3F:
                    if (action == 0x14) {
                        func_15063FA0(actor, 2);
                    } else {
                        func_15063FA0(actor, 0x12);
                    }
                    ctl = actor->unk31C;
                    ctl->unk1AA++;
                    camera = actor->camera;
                    if (camera != NULL) {
                        func_1512D2E4(camera, 1);
                    }
                    break;
                case 0x24:
                    if (func_15081E78(actor, &ctl->unkB0, 7) != 2) {
                        ctl = actor->unk31C;
                        if (ctl->unkAF != 0) {
                            ctl->unkAF = 0;
                            if (func_10010894(actor) == 0) {
                                func_10010558(func_150ADA20() % 3 + 0x53B, actor, 0x7D00, 800, 3000, 30);
                            }
                        }
                    }
                    camera = actor->camera;
                    if (camera != NULL) {
                        func_1512D2E4(camera, 1);
                    }
                    break;
                case 0x3B:
                    func_15081E78(actor, &ctl->unkB0, 0x10);
                    func_150FFCC8(actor, position, direction, side, up);
                    func_150FFD84(position, direction, 0xFF, 1);
                    break;
                case 0x23:
                    func_15081E78(actor, &ctl->unkB0, 8);
                    camera = actor->camera;
                    if (camera != NULL) {
                        func_1512D2E4(camera, 0);
                    }
                    break;
                }
                ctl = actor->unk31C;
            }
            ctl->unk1B4 = 0;
        }
    }
}

/* Stores in D_800CC2B2 (and returns) how far this object's heading is off the bearing to its
 * linked object, clamped to +-16000. */
s32 func_150639BC(Actor *arg0) {
    D_800CC2B2 = arg0->unk7A - arg0->unk31C->unk12 - func_15063390(arg0);
    if (D_800CC2B2 > 16000) {
        D_800CC2B2 = 16000;
    }
    if (D_800CC2B2 < -16000) {
        D_800CC2B2 = -16000;
    }
    return D_800CC2B2;
}

/* When any of `mask` is set in `buttons`, turns the object's unk12 offset from the stick (or
 * toward its linked object), limited to +-9000; the excess turns the whole object. */
s32 func_15063A38(Actor *arg0, s32 buttons, s32 mask) {
    if (buttons & mask) {
        D_800CC2B2 = D_800CC284->stick_x * 200;
        if (arg0->unk31C->unk84 != 0) {
            D_800CC2B2 = func_150639BC(arg0);
        }

        D_800CC2B4 = 12.0f;
        D_800CC2B2 = arg0->unk31C->unk12 + ((D_800CC2B2 * D_800CC264) / 2000);

        if (D_800CC2B2 < -9000) {
            arg0->unk76 = arg0->unk7A = (arg0->unk7A - D_800CC2B2) - 9000;
            D_800CC2B2 = -9000;
        }

        if (D_800CC2B2 > 9000) {
            arg0->unk76 = arg0->unk7A = (arg0->unk7A - D_800CC2B2) + 9000;
            D_800CC2B2 = 9000;
        }

        arg0->unk31C->unk12 = D_800CC2B2;
        return 1;
    }
    return 0;
}

/* Throws one of the object's grenades (anim 0x64 for ids 0x75 and 0xB1, 0x2C otherwise). */
void func_15063B64(Actor *arg0) {
    ThrowPacket packet;

    if ((arg0->id == 0x75) || (arg0->id == 0xB1)) {
        packet.anim = 0x64;
    } else {
        packet.anim = 0x2C;
    }

    if (func_1503195C(arg0, packet.anim, 0) != 0) {
        if (arg0->unk1D4 != NULL) {
            func_15143134(NULL, packet.position, (s32)arg0->unk1D4 + 0x100);
        } else {
            packet.position[0] = arg0->x_position;
            packet.position[1] = arg0->y_position;
            packet.position[2] = arg0->z_position;
        }

        packet.throwerId = (arg0 - D_800CC2D0) + 1;
        packet.thrower = arg0;
        func_1506AC8C(arg0, 0xB, &packet);
        func_150836CC(arg0, packet.anim);

        if (arg0->unk31C->grenade_count != 0) {
            arg0->unk31C->grenade_count--;
        }
        arg0->unk31C->unk11A = 0;
    }
}

/* Returns the active object with id `id` that is linked (unk65) to arg0, or NULL.
 * (asm-differ shows relocation-addend diffs here: the .s names interior symbols of
 * D_800CC2D0[], C spells them base+offset; the linked bytes are identical.) */
Actor *func_15063C60(Actor *arg0, s32 id) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) && (id == D_800CC2D0[i].id) &&
            (((arg0 - D_800CC2D0) + 1) == D_800CC2D0[i].unk65)) {
            return &D_800CC2D0[i];
        }
    }
    return NULL;
}

/* Starts `anim` as a new action when the object stands on the ground with nothing else going
 * on and B is pressed. Returns `anim`, or 999 when the action can't start. */
s32 func_15063E84(Actor *arg0, u16 anim, u16 flags, s32 action, s32 buttons) {
    s32 state;

    if ((buttons & B_BUTTON) && (arg0->unk28 == 0.0f) && (arg0->y_velocity <= 0.0f) &&
        (arg0->unk31C->unk95 == 0) && (arg0->unk84.uh != anim)) {
        state = arg0->unk31C->unk11A;
        if ((state == 0 || state == 2) && (arg0->unk31C->unk27 == 0)) {
            if (action != 0 && arg0->disable_jump != 0) {
                return 999;
            }
            if (flags & 1) {
                arg0->disable_jump = 20;
            }
            if (action != 0) {
                if (arg0->unk31C->unk11A == 2) {
                    return 999;
                }
                arg0->unk31C->unk11A = 1;
            } else {
                arg0->unk31C->unk11A = 3;
            }
            arg0->unk31C->unk78 = action;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
            return anim;
        }
    }
    return 999;
}

/* Fires a shot of kind `kind` (2, 3, 6, 0x12) from the object's muzzle (unk13C..unk144),
 * aimed along its yaw/pitch or, when unk109 is set, at the point unkB8..unkC0. */
void func_15063FA0(Actor *arg0, s32 kind) {
    f32 velX;
    f32 velY;
    f32 velZ;
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
    ActorCtl *ctl;

    dist = 300.0f;
    height = 0.0f;
    radius = 0.0f;
    switch (kind) {
    case 2:
        radius = 27.0f;
        break;
    case 3:
    case 6:
        height = -11.0f;
        dist = 500.0f;
        break;
    }
    angle = arg0->unk31C->aimYaw * 0.01745329238f;
    off[0] = sinf(angle) * radius;
    off[1] = cosf(angle) * radius;
    pos[0] = off[0] + arg0->unk31C->aimEye.x;
    pos[1] = arg0->unk31C->aimEye.y + height;
    pos[2] = off[1] + arg0->unk31C->aimEye.z;
    ctl = arg0->unk31C;
    if (ctl->unk109 == 0) {
        pitch = -ctl->aimPitch;
        yaw = ctl->aimYaw + 90.0f;
    } else {
        dx = ctl->unkB8 - pos[0];
        dy = ctl->unkBC - pos[1];
        dz = ctl->unkC0 - pos[2];
        pitch = func_150484A0(dy, sqrtf(dx * dx + dz * dz)) * 57.29577637f;
        yaw = func_150484A0(dx, dz) * 57.29577637f;
    }
    func_1505A184((s16)(pitch * 182.0444489f), dist, 0.0f, &velX, &velY, &velZ);
    velY = -velY;
    func_1506C460(yaw, velX, velY, 0.0f, 100, kind, 0.0f, 0.5f, (s32)pos, 1, 1);
}

s32 func_1505DADC(Actor *, u16 *, s32, s32, s32);

/* Picks animA or animB at random; if one of them is already playing, picks animC instead
 * (or returns 999 while unk107 < 40). Returns 999 when animC is already playing. */
u16 func_150641D8(Actor *arg0, u16 animA, u16 animB, u16 animC) {
    u16 selected;
    Actor *actor = arg0;  // matching: the copy reserves the extra stack slot IDO gave the original
    u16 angle;

    angle = 0;
    selected = animA;
    if (func_150ADA20() & 1) {
        selected = animB;
    }
    if ((actor->unk84.uh == animA) || (actor->unk84.uh == animB)) {
        if (actor->unk107 < 40) {
            return 999;
        }
        selected = animC;
    }
    if (actor->unk84.uh == animC) {
        selected = 999;
    }
    if (func_1505DADC(actor, &angle, 0, 0xFE, 0x40) != 0xFF) {
        actor->unk76 = angle;
    }
    actor->unk83 = 0xFF;
    return selected;
}

extern s32 D_800CC28C;

/* A libultra fixed-point Mtx viewed as its integer and fraction halves. */
typedef struct MtxParts {
    s16 whole[4][4];
    u16 frac[4][4];
} MtxParts;
void func_15074664(void);
void func_15060A30(s32 arg0, Actor *arg1);
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);

/* Handles the scripted move requested in unk7B, then the default B-button move; plays the
 * chosen animation unless `noPlay` is set. */
void func_150642AC(Actor *arg0, s32 buttons, f32 arg2, s32 noPlay, s32 lockB) {
    s32 anim;
    f32 speed;
    f32 blend;
    Actor *obj;
    s32 pad;  // matching: unused stack slot
    MtxParts *mtx;
    f32 dx;
    f32 dist;
    f32 dz;
    s16 x;
    s16 z;
    f32 pushX;
    f32 pushY;
    f32 pushZ;

    anim = 0;
    speed = 1.0f;
    blend = 4.0f;
    if (lockB) {
        buttons &= ~B_BUTTON;
        D_800CC28C &= ~B_BUTTON;
    }
    if (arg0->unk31C->unk7B != 0) {
        switch (arg0->unk31C->unk7B) {
        case 1: {
            s16 pad;  // matching: unused stack slot

            arg0->unk83 = 0xFF;
            arg0->disable_run = 0xFF;
            x = (arg0->unk2E4 >> 16) & 0xFFFF;
            z = arg0->unk2E4 & 0xFFFF;
            arg0->unk84.uh = 0;
            arg0->unk31C->unk30 = 0xD;
            arg0->unk31C->unk28 = x;
            arg0->unk31C->unk2C = z;
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
                    mtx = &((MtxParts *)obj->unk1D4)[5];
                    arg0->y_position = mtx->whole[3][1] + 20;
                }
            }
            if (buttons & A_BUTTON) {
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
            if ((buttons & B_BUTTON) && (arg2 <= 0.0f)) {
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
                func_1505A184(angle, 20.0f, 0.0f, &pushX, &pushY, &pushZ);
                func_1505A250(pushX, pushY, (1200.0f - dist) * 0.001000000047f, &arg0->unk16C, &arg0->unk170);
            }
            break;
        default:
            break;
        }
        arg0->unk31C->unk7B = 0;
    }
    if (arg0->unk31C->unk198 == 0 && anim == 0 && (buttons & B_BUTTON) && (arg0->unk31C->unk4E & 0xF) != 1 &&
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
        } else if ((arg0->unk31C->unk75 == 0 && arg0->unk31C->unk7C == 0) || arg0->unk31C->unk4E == 2) {
            if (D_800BE9F0 != 0x2D && D_800BE9F0 != 0x2B && D_800BE9F0 != 0x30 && D_800BE9F0 != 0x34) {
                anim = 0x33;
                if (arg0->unk31C->unk8 != 0) {
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
                        speed = 1.0;  // matching: double literal; 1.0f drops its .rodata entry
                        anim = 0x46;
                    }
                    arg0->unk80 = 1;
                    blend = 0.0f;
                }
            }
        }
    }
    if (anim != 0 && anim != 999 && !noPlay) {
        func_1505E650(arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
}

/* Copies object srcIndex's animation position (unk2D0->unk8) to object dstIndex, keeping it
 * below dstIndex's unk18. */
void func_150649A0(s32 srcIndex, s32 dstIndex) {
    Actor *objects;
    AnimInstance *dst;
    AnimInstance *src;

    objects = D_800CC2D0;
    dst = objects[dstIndex].unk2D0;
    src = objects[srcIndex].unk2D0;

    if (dst != NULL && src != NULL) {
        dst->frame = src->frame;
        if (dst->numFrames <= dst->frame) {
            dst->frame = dst->numFrames - 1.0f;
        }
    }
}

/* Plays the animation that pairs with what the linked object (unk65) is doing, and syncs
 * this object's animation to it. */
void func_15064A14(Actor *arg0) {
    Actor *partner;
    s32 anim;
    s32 variant;

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
        variant = ((arg0->unk31C != NULL) && (arg0->unk31C->unk1B2 != 0)) ? 1 : 0;
        if (partner->unk232 == 4) {
            anim = variant ? 0x350 : 0x1A4;
        } else {
            anim = variant ? 0x34F : 0x1A1;
        }
    }
    func_1505E650(arg0, anim, 1.0f, 6.0f, 0.0f, 0.0f, 0);
    func_150649A0(arg0->unk65 - 1, D_800C3E78);
}

/* Picks the object's movement animation from its action and speed, plays it, and returns it
 * (0 or 999 when nothing new plays). */
s32 func_15064B94(Actor *arg0, f32 arg1, f32 arg2, s32 overrideAnim, f32 arg4) {
    s32 anim;
    f32 blend;
    f32 speed;
    ActorCtl *ctl;

    anim = 0;
    blend = 4.0f;
    ctl = arg0->unk31C;
    speed = 1.0f;
    if (ctl->unk46 > 0) {
        anim = 0x4C;
        speed = arg1 / 24.0f;
    } else if (ctl->unk4E == 2) {
        anim = 5;
        if (ctl->unk50 > 0) {
            anim = 0x54;
        }
        blend = 2.0f;
    } else if (arg0->unk13C >= 100) {
        if (arg0->xz_velocity < 1.0f) {
            speed = 1.0f;
            anim = func_1504C078();
        } else {
            switch (D_800CC2D0[D_800D154C->unk13C - 100].id) {
            case 0x8C:
                arg0->unkAA = 0x5A;
                anim = 0x1A6;
                speed = arg1 / 6.0f + 0.4f;
                if (arg0->xz_velocity > 14.0f) {
                    anim = 0x1A7;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0x57:
                arg0->unkAA = 0x5A;
                anim = 0x116;
                speed = arg1 / 6.0f + 0.4f;
                if (arg0->xz_velocity > 14.0f) {
                    anim = 0x117;
                    speed = arg1 / 26.0f + 0.5f;
                }
                break;
            case 0xA8:
            case 0xA9:
                if (arg0->xz_velocity > 14.0f) {
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
                if (arg0->xz_velocity > 14.0f) {
                    anim = 0x2E4;
                    speed = arg1 * 0.0625f + 0.5f;
                } else {
                    anim = 0xD7;
                    speed = arg1 / 6.0f + 0.4f;
                }
                break;
            }
        }
    } else if (overrideAnim != 0 && ctl->unk78 == 0) {
        anim = overrideAnim;
        speed = arg1 / 15.0f + 0.6f;
    } else if (2.6f < arg0->unk44 && (ctl->unk32 & 1)) {
        arg0->unkAA = 0x28;
        blend = 5.0f;
        speed = arg0->unk44 / 15.0f + 0.5f;
        anim = func_1504C0B8();
    } else if (ctl->unk78 == 0x16) {
        if (arg0->unk44 >= 1.0f) {
            anim = ctl->unk1B2 != 0 ? 0x2C6 : 0xE8;
            arg0->unkAA = 0x1E;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 9) {
        if (arg0->unk44 >= 1.0f) {
            if (D_800BE616 == 0) {
                arg0->unkAA = 0x37;
            }
            anim = 0x7E;
            if (D_800BE616 == 0) {
                speed = arg1 * 0.0625f + 0.6f;
            } else if (arg0->unk44 >= 20.0f) {
                anim = 0x22C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x38) {
        if (arg0->unk44 >= 1.0f) {
            anim = 0x1E1;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x1E2;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x39) {
        if (arg0->unk84.uh != 0x1EB) {
            if (arg0->unk44 >= 1.0f) {
                anim = 0x1EC;
                if (arg0->unk44 >= 20.0f) {
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
    } else if (ctl->unk78 == 0x37) {
        if (arg0->unk84.uh != 0x1F4) {
            if (arg0->unk44 >= 1.0f) {
                anim = 0x1F5;
                if (arg0->unk44 >= 20.0f) {
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
    } else if (ctl->unk78 == 0x3B) {
        if (arg0->unk84.uh != 0x222 && arg0->unk84.uh != 0x236) {
            if (arg0->unk44 >= 1.0f || ctl->unk23 > 0) {
                anim = 0x223;
                arg0->unkAA = 0x37;
                if (arg0->unk44 >= 20.0f) {
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
    } else if (ctl->unk78 == 0x12 || ctl->unk78 == 0x18 || ctl->unk78 == 0x41) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xEC;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0xF8;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x24) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x158;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x159;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x22) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x15B;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x15C;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x21) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x6E;
            anim = 0x123;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 15.0f) {
                anim = 0x124;
                speed = arg1 / 27.0f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x26) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x178;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x179;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x3A) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x1FB;
            speed = arg1 * 0.0625f + 0.6f;
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x25) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xD7;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x2D || ctl->unk78 == 0x2E) {
        if (arg0->unk44 >= 1.0f) {
            anim = 0x1B0;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x1B1;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x23) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0x161;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                anim = 0x162;
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x15) {
        if (arg0->unk44 >= 1.0f) {
            arg0->unkAA = 0x50;
            anim = 0xED;
            speed = arg1 * 0.0625f + 0.6f;
            if (arg0->unk44 >= 20.0f) {
                speed = arg1 / 22.0f + 0.4f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk78 == 0x19 || ctl->unk78 == 0x40) {
        if (arg0->xz_velocity >= 1.0f) {
            anim = 0x112;
            blend = 8.0f;
            speed = arg1 * 0.125f + 0.3f;
            if (arg0->xz_velocity >= 18.0f) {
                anim = 0x111;
                speed = arg1 / 20.0f + 0.3f;
            }
        } else {
            speed = 1.0f;
            anim = func_1504C078();
        }
    } else if (ctl->unk1A != 0 && arg2 > 1.0f) {
        anim = 0x6D;
        speed = arg1 / 6.5f + 0.3f;
        arg0->unkAA = 0x28;
        blend = 8.0f;
    } else if (ctl->unk8 > 0) {
        if (arg2 < 1.0f) {
            speed = 1.0f;
            anim = func_1504C078();
        } else {
            anim = ctl->unkE;
            speed = arg1 / 6.0f + 0.3f;
            blend = 16.0f;
        }
    } else if (ctl->unk17 != 0) {
        blend = 5.0f;
        if (D_800CC284->button & Z_TRIG) {
            if (arg0->unk84.uh != 0x283) {
                arg0->xz_velocity *= 0.2f;
                D_800D1580 = 0xFF010604;
                func_1506E5FC();
                speed = 1.0f;
            }
            anim = 0x283;
            arg0->disable_run = 3;
            arg0->disable_jump = 3;
        } else {
            if (arg0->unk84.uh == 0x283) {
                D_800D1580 = 0xFF010604;
                func_1506E5FC();
            }
            if (arg0->unk44 >= 1.0f && arg0->disable_run == 0) {
                arg0->unkAA = 0x2D;
                speed = arg0->xz_velocity / 9.0f;
                anim = 0x4E;
            } else {
                speed = 1.0f;
                anim = func_1504C078();
            }
        }
    } else if ((ctl->unk32 & 2) && arg0->unk44 >= 1.0f) {
        ctl->unk3C = 0x1C;
        arg0->unkAA = 0xF;
        speed = arg4 / 5.0f + 0.3f;
        blend = 8.0f;
        anim = 0x64;
        if (func_150ADA20() % 1000U >= 988) {
            anim = (func_150ADA20() & 1) + 0x65;
            speed = 0.65f;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
        }
    }
    if (anim != 0 && anim != 999) {
        func_1505E650(arg0, anim, speed, blend, 0.0f, 0.0f, 0);
    }
    return anim;
}

extern u16 D_800996A0[][4];
extern u16 D_800996C0[];
extern u16 D_800996C8[];
extern u16 D_800996D0[];
extern u8 D_800C3A58;
extern s32 D_800CC290;
extern WorldObject *D_800CC5EC;
extern void func_1000EC24();
extern void func_1000EFB4();
extern void func_151037DC();
s32 func_1000F568(s32 arg0, s32 arg1);
void func_1000FD38(void *, void *, s32);
void func_15073FA0(void);
struct216 *func_1507BB28(s32, s32);
void func_1507E7E4(void *, u8, u8, u16, s32);
void func_1507EABC(void *);
void func_1507EB4C(Actor *, s32);
void func_15080828(s32 arg0);
void func_15080BE8(void);
s32 func_15080CF4(void);
void func_150D8B88(Actor *);
s32 func_150DBD70(u8);
void func_150FC438(Actor *, Actor *, u8, u8);
void func_1514F568(Actor *);
void func_151D4668(void *);
void func_151D469C(void *arg0, s32 a1, s32 a2, s32 a3, s32 a4);
void func_151D4D58(void *arg0);
s32 func_151F2CDC(void);

/* Per-frame handler for the object's held item / special action `act`: starts, steps and ends
 * it from the held and pressed buttons. Returns the animation started (0 or 999 for none), or
 * 1000 when the action consumed the input. */
s32 func_15065A5C(Actor *arg0, s32 heldButtons, s32 pressedButtons, f32 arg3, s32 arg4, s32 act, s32 arg6, s32 arg7, s32 arg8) {
    s32 anim;
    f32 speed;
    f32 blend;
    f32 animParam;
    s32 clearAction;
    s32 setUnk58;
    u8 handled;
    s32 pendingSound;

    clearAction = 1;
    setUnk58 = 0;
    handled = 0;
    pendingSound = 0;
    speed = 1.0f;
    blend = 4.0f;
    animParam = 0.0f;
    anim = 0;

    if ((arg0->unk31C->unk11A == 3) || (arg0->stunned != 0) || (arg4 != 0)) {
        pressedButtons &= ~B_BUTTON;
        D_800CC28C &= ~B_BUTTON;
        if (arg4 != 0) {
            if (arg0->stunned == 0) {
                arg0->events = NULL;
            }
            pressedButtons &= ~Z_TRIG;
            D_800CC28C &= ~Z_TRIG;
        }
    }
    if (arg0->y_velocity > 0.0f) {
        arg3 += 50.0f;
    }

    switch (act) {
    case 0x2:
    case 0xA:
    case 0x13:
    case 0x34:
        {
            s32 idx;

            handled = 1;
            if (act == 2) {
                idx = 0;
            } else if (act == 0x13) {
                idx = 1;
            } else if (act == 0xA) {
                idx = 2;
            } else {
                idx = 3;
            }
            if (arg6 != 0) {
                switch (act) {
                case 0x34:
                    func_150836CC(arg0, 0x3D);
                    arg0->unk31C->unk11A = 0;
                    break;
                case 0xA:
                    func_150836CC(arg0, 0x28);
                    func_150836CC(arg0, 0x29);
                    arg0->unk31C->unk11A = 0;
                    break;
                }
            }
            if (act == arg0->unk31C->unk78) {
                arg0->xz_velocity = 0.0f;
            }
            if ((arg0->in_water != 0) && ((act == arg0->unk31C->unk78) || (arg0->unk31C->unk11A != 0))) {
                arg0->unk31C->unk78 = 0;
                arg0->disable_run = 0;
                arg0->unk83 = 0;
                if (act == 0xA) {
                    func_150836CC(arg0, 0x28);
                    func_150836CC(arg0, 0x29);
                    arg0->unk31C->unk11A = 0;
                }
            } else if (((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) || (arg4 != 0)) {
                if ((arg0->unk31C->unk78 != 0) || (arg4 != 0)) {
                    if ((arg0->unk31C->unk72 == 0) || (arg4 != 0)) {
                        arg0->disable_run = 0xFF;
                        arg0->disable_jump = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = D_800996A0[idx][3];
                        arg0->unk31C->unk11A = 3;
                        arg0->unk31C->unk72 = 0x5A;
                        setUnk58 = 1;
                    }
                } else if (arg0->unk31C->unk72 == 0) {
                    if ((arg0->camera->aimState != 1) && (arg0->camera->aimState != 3)) {
                        arg0->unk31C->unk1B = 0;
                        arg0->unk31C->unk11A = 1;
                        anim = D_800996A0[idx][0];
                        arg0->unk83 = 0xFF;
                        arg0->disable_run = 0xFF;
                        arg0->unk31C->unk72 = 0x5A;
                    }
                }
                pressedButtons &= ~B_BUTTON;
            }
            if (arg0->unk31C->unk78 && (pressedButtons & Z_TRIG) && (anim == 0) && (arg0->unk84.uh == D_800996A0[idx][1])) {
                anim = D_800996A0[idx][2];
                arg0->unk31C->unk1B++;
            }
            if (((arg0->unk84.uh == D_800996A0[idx][1]) || (arg0->unk84.uh == D_800996A0[idx][0]) ||
                 (arg0->unk84.uh == D_800996A0[idx][2]) || (anim != 0)) && (anim != D_800996A0[idx][3])) {
                arg0->unk31C->unk78 = act;
                D_800CC284->button &= ~Z_TRIG;
                setUnk58 = 1;
            } else {
                arg0->unk31C->unk78 = 0;
            }
            if (D_800C3A58 != 0) {
                if ((arg0->unk31C->unk11A == 1) || (arg0->unk31C->unk11A == 2U)) {
                    anim = D_800996A0[idx][1];
                    arg0->unk83 = 0xFF;
                    arg0->unk31C->unk78 = act;
                    arg0->unk31C->unk11A = 2;
                    switch (act) {
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
                    arg0->unk31C->unk11A = 0;
                    arg0->unk31C->unk78 = 0;
                    switch (act) {
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

            if (act == 0x14) {
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
            if ((arg7 != 0) && (arg0->unk31C->unk78 != 0)) {
                func_15083568(arg0, sfx, 1.0f, 0);
            }
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
                if (arg0->unk31C->unk78 != 0) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->disable_jump = 0xFF;
                    arg0->unk31C->unk11A = 3;
                    speed = 2.0f;
                    anim = animEnd;
                    arg0->unk31C->unk78 = 0;
                } else {
                    arg0->disable_run = 0x10;
                    arg0->unk83 = 0x10;
                    arg0->disable_jump = 0x10;
                    speed = 2.0f;
                    arg0->unk31C->unk11A = 1;
                    anim = animStart;
                    arg0->unk31C->unk78 = act;
                }
            }
            if (anim != 999) {
                speed = 2.0f;
            } else if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, sfx);
                arg0->unk31C->unk11A = 0;
            } else if (act == arg0->unk31C->unk78) {
                arg0->unkAA = 0x4B;
                if (arg6 != 0) {
                    clearAction = 0;
                } else if (arg0->unk31C->aimState != 0) {
                    arg0->disable_jump = 0xA;
                    arg0->disable_run = 0;
                    if (pressedButtons & Z_TRIG) {
                        if (arg0->unk31C->unk72 == 0) {
                            arg0->unk83 = 0xF;
                            arg0->unk31C->unk1B4 |= 3;
                            D_800CC2B0 = 1;
                            anim = animHit;
                            animParam = 6.0f;
                            arg0->unk31C->unk72 = 0x28;
                        }
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg3 <= 0.0f) && (arg0->disable_run == 0) && (arg0->unk31C->unk72 == 0)) {
                    arg0->unk31C->unk72 = 0x28;
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
        handled = 1;
        if ((arg4 != 0) || (arg6 != 0)) {
            if (D_800BE9F0 != 0xA) {
                arg0->unk31C->unk78 = 0;
                arg0->unk31C->unkD = 0;
                arg0->unk83 = 0;
            }
            if (arg6 != 0) {
                clearAction = 0;
            }
        } else if (arg0->unk31C->unk16 == 0) {
            if (arg0->unk31C->unk8 != 0) {
                arg0->unk31C->unk8 -= D_800BE9A0;
                if (arg0->unk31C->unk8 < 0x64) {
                    arg0->unk31C->unkE = 0xAD;
                    arg0->unk31C->unk16 = 0xC8;
                    func_1507EB4C(arg0, 0x2A);
                    speed = 1.3f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0x2D;
                    arg0->unk31C->unk78 = 0;
                }
            }
            if (arg0->unk31C->unk16 == 0) {
                if ((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    if (arg0->unk31C->unk78 != 0) {
                        anim = 0x2D;
                        speed = 1.3f;
                        arg0->unk31C->unk78 = 0;
                    } else {
                        arg0->unk31C->unk78 = 3;
                        anim = 0x2E;
                        arg0->unk31C->unkD = 0;
                        speed = 1.6f;
                    }
                } else if ((arg0->unk31C->unk78 == 3) && (arg0->disable_run != 0xFF)) {
                    arg0->unk31C->unk78 = 3;
                    anim = 0x2C;
                    if (D_800BE616 != 0) {
                        arg0->disable_jump = 0xA;
                    }
                    D_800CC2B2 = D_800CC284->stick_x * 0x96;
                    if (arg0->unk31C->unk84 != 0) {
                        arg0->unk31C->unk12 = 0;
                    } else if ((D_800CC2B2 < -0x1F40) && (arg0->unk31C->unk12 < -0x1B58)) {
                        arg0->unk76 = arg0->unk7A = arg0->unk76 + (D_800CC264 * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    } else if ((D_800CC2B2 > 0x1F40) && (arg0->unk31C->unk12 > 0x1B58)) {
                        arg0->unk76 = arg0->unk7A = arg0->unk76 - (D_800CC264 * 7);
                        anim = 0xE0;
                        speed = 3.0f;
                    }
                    D_800CC2B4 = 10.0f;
                    blend = 8.0f;
                    arg0->disable_run = 0x10;
                    arg0->unk83 = 0x10;
                    if (arg0->unk31C->unkD == 0) {
                        arg0->unk31C->unkD = 1;
                        func_150D8B88(arg0);
                    }
                }
            }
        }
        break;
    case 0x1:
    case 0x35:
        if ((pressedButtons & B_BUTTON) && ((arg3 <= 0.0f) || (act == 0x35)) && (D_800CC5EC->attach != 1) &&
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
            arg0->unk31C->unk31 = 0;
        }
        break;
    case 0x4:
        if (pressedButtons & B_BUTTON) {
            arg0->unk31C->unk78 = act;
            return 0;
        }
        arg0->unk31C->unk78 = 0;
        return 0;
    case 0x42:
        if (!(D_800D2E68 & 4)) {
            if (pressedButtons & B_BUTTON) {
                arg0->unk31C->unk78 = act;
                return 0;
            }
            arg0->unk31C->unk78 = 0;
        }
        return 0;
    case 0x5:
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            if (arg0->unk84.uh == 0x2F) {
                anim = 999;
            } else {
                arg0->unk31C->unk30 = 0x12;
                anim = 0x2F;
                if (D_800BE9F0 == 0x31) {
                    arg0->unk31C->unk28 = D_800D3098[12].x;
                    arg0->unk31C->unk2C = D_800D3098[12].z;
                    func_150DBD70(5);
                } else {
                    arg0->unk31C->unk28 = D_800D3098[2].x;
                    arg0->unk31C->unk2C = D_800D3098[2].z;
                    func_150DBD70(0);
                }
                arg0->unk76 = 0xC000;
                speed = 1.049999952f;
                blend = 6.0f;
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                pendingSound = 0x143;
            }
        }
        break;
    case 0x6:
        if (arg0->unk31C->unk78 == 0) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->unk31C->unk72 == 0)) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                anim = 0x44;
                arg0->unk31C->unk78 = 6;
            }
        } else if (arg0->unk84.uh != 0x44) {
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            arg0->unk31C->unk78 = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x36:
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            ActorCtl *found;

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
        if (arg0->unk31C->unk78 == 0) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->unk31C->unk72 == 0)) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                anim = 0x45;
                arg0->unk31C->unk78 = 7;
            }
        } else if (arg0->unk84.uh != 0x45) {
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            arg0->unk31C->unk78 = 0;
        } else {
            anim = 999;
        }
        break;
    case 0x8:
        {
            Actor *obj;

            obj = &D_800CC2D0[arg0->unk65 - 1];
            arg0->disable_jump = 0x14;
            if (pressedButtons & (A_BUTTON | B_BUTTON)) {
                if ((obj->unk84.uh == 0) || (obj->unk22E != 0) || (obj->unk84.uh == 0xC) || (obj->unk84.uh == 0xD)) {
                    u8 mode;

                    mode = 0x10;
                    D_800D154C = obj;
                    if (pressedButtons & B_BUTTON) {
                        mode = 0xE;
                    } else {
                        obj->unk84.uh = 0;
                    }
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
        if (act != arg0->unk31C->unk78) {
            arg0->unk31C->unk78 = 0x27;
            arg0->lodRequest = 6;
            arg0->unk31C->unk17 = 1;
        }
        break;
    case 0x9:
        if (D_800C3A58 != 0) {
            if ((arg0->unk31C->unk11A == 1) || (arg0->unk31C->unk11A == 2)) {
                func_15063404(arg0);
            } else {
                func_150634E4(arg0);
            }
        }
        if (act == arg0->unk31C->unk78) {
            s16 delta;

            delta = (D_800CC284->stick_x * -100) - arg0->unk31C->unk80;
            if (delta > 600) {
                delta = 600;
            }
            if (delta < -600) {
                delta = -600;
            }
            arg0->unk31C->unk80 += delta;
            delta = ((D_800CC284->stick_y << 5) - arg0->unk31C->unk82) + 576;
            if (delta > 300) {
                delta = 300;
            }
            if (delta < -300) {
                delta = -300;
            }
            arg0->unk31C->unk82 += delta;
            if (((arg4 != 0) && ((arg0->unk65 != 0) || (arg0->stunned != 0) || (arg0->unk28 != 0.0f))) || (arg0->in_water != 0)) {
                func_150634E4(arg0);
            } else if (arg4 != 0) {
                speed = 1.399999976f;
                pressedButtons |= B_BUTTON;
                anim = func_15063E84(arg0, 0x7D, 1, 0, pressedButtons);
                if (anim == 999) {
                    func_150634E4(arg0);
                }
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->disable_jump = 0xA;
                if (arg0->unk31C->matrix_physics != 0) {
                    u16 prev;
                    u8 lowBits;

                    if ((heldButtons & Z_TRIG) && (arg0->unk48 < 0.6000000238f)) {

                        prev = arg0->unk31C->unk7E;
                        arg0->unk31C->unk7E = prev + ((D_800CC264 * 4) / 3);
                        if ((prev ^ arg0->unk31C->unk7E) & 0x100) {
                            D_800D1580 = 0x13;
                            if (arg0->unk31C->unk7E & 0x200) {
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
                        lowBits = (arg0->unk31C->unk7E << 1) | 1;
                        arg0->unk31C->unk7E = (arg0->unk31C->unk7E & 0xFF00) | lowBits;
                    }
                } else {
                    func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                    if ((arg0->unk84.uh != 0x11A) && (arg0->unk84.uh != 0x7B)) {
                        u8 endEffect;
                        endEffect = 0;
                        if (!(heldButtons & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG)) {
                            endEffect = 1;
                        }
                        if ((arg0->unk31C->unk24 == 0) || (pressedButtons & A_BUTTON)) {
                            anim = 0x11A;
                            if (!(pressedButtons & A_BUTTON)) {
                                endEffect = 1;
                            }
                            arg0->unk31C->unk24 = 0x3C;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                        } else if ((heldButtons & Z_TRIG) && (arg0->disable_run == 0)) {
                            if (arg0->xz_velocity != 0.0f) {
                                if (arg0->unk44 >= 20.0f) {
                                    anim = 0x238;
                                    speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                                } else {
                                    anim = 0x20D;
                                }
                                if (!(heldButtons & R_TRIG)) {
                                    arg0->unk31C->unk12 = 0;
                                    arg0->unk31C->unk3C = 3;
                                } else {
                                    arg0->unk31C->unk3C = 0;
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
                                func_1000FA64(0x2CF, arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, (s32)D_800996C0, 0, 0); // matching: the (s32) casts on these tables steer IDO; a plain pointer reschedules
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
                            if ((pressedButtons & B_BUTTON) && (arg0->unk2EC == 1) && (arg0->unk28 == 0.0f)) {
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
                            anim = func_15063E84(arg0, 0x7D, 1, 0, pressedButtons);
                        }
                        if (endEffect != 0) {
                            func_151D4D58(arg0);
                        }
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x7B, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->unk31C->unk24 = 0x3C;
            }
        }
        break;
    case 0x38:
        if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x7B);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                u8 endEffect;
                u16 pad[2];  // matching: unused stack slots

                arg0->disable_jump = 0xA;
                func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                if (arg0->unk84.uh != 0x1DD) {
                    endEffect = 0;
                    if (!(heldButtons & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG)) {
                        endEffect = 1;
                    }
                    if (heldButtons & Z_TRIG) {
                        if (arg0->xz_velocity != 0.0f) {
                            if (arg0->unk44 >= 20.0f) {
                                anim = 0x254;
                                speed = (arg0->xz_velocity / 22.0f) + 0.400000006f;
                            } else {
                                anim = 0x253;
                            }
                            if (!(heldButtons & R_TRIG)) {
                                arg0->unk31C->unk3C = 3;
                                arg0->unk31C->unk12 = 0;
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
                        arg0->unk31C->unk1A4 += D_800BE9E4 * 0xA;
                        if (arg0->unk31C->unk1A2 > 0xAF) {
                            arg0->unk31C->unk1A2 = 0xAF;
                        }
                        arg0->unk31C->unk1A2 += arg0->unk31C->unk1A4 * D_800BE9E4;
                        if (arg0->unk31C->unk1A2 > 0x1E00) {
                            arg0->unk31C->unk1A2 = 0x1E00;
                        }
                    } else {
                        speed = 1.399999976f;
                        anim = func_15063E84(arg0, 0x1DE, 1, 0, pressedButtons);
                        arg0->unk31C->unk1A2 = 0;
                        arg0->unk31C->unk1A4 = 0;
                    }
                    if (endEffect != 0) {
                        func_151D469C(arg0, 5, 0x78, 0xFF, 1);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1DD, 1, act, pressedButtons);
        }
        break;
    case 0x39:
        if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x82);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                if (arg0->unk84.uh != 0x1EE) {
                    if (arg0->unk84.uh == 0x21E) {
                        arg0->disable_run = 0xFF;
                    }
                    if ((arg0->unk31C->unk24 == 0) || ((pressedButtons & A_BUTTON) && (arg0->unk84.uh != 0x21E))) {
                        if (arg0->unk28 == 0.0f) {
                            arg0->unk31C->unk24 = 0x1E;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            anim = 0x21E;
                        }
                    } else if ((pressedButtons & Z_TRIG) && (arg0->unk84.uh != 0x1EE) && (arg0->unk84.uh != 0x21E)) {
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
                        anim = func_15063E84(arg0, 0x1EF, 1, 0, pressedButtons);
                    }
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1EE, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->unk31C->unk24 = 0x1E;
            }
        }
        break;
    case 0x37:
        if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x85);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                if ((pressedButtons & Z_TRIG) && (arg0->unk84.uh != 0x1F7) && (arg0->unk31C->unk19E == 0)) {
                    if (func_150ADA20() & 3) {
                        anim = 0x1F4;
                    } else {
                        anim = 0x235;
                    }
                    blend = 0.0f;
                    arg0->unk31C->unk19E = 0x78;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                } else if (pressedButtons & B_BUTTON) {
                    speed = 1.399999976f;
                    anim = func_15063E84(arg0, 0x1F8, 1, 0, pressedButtons);
                }
            }
        } else {
            speed = 1.399999976f;
            anim = func_15063E84(arg0, 0x1F7, 1, act, pressedButtons);
        }
        break;
    case 0xB:
        if (arg0->unk84.uh == 0x68) {
            if ((arg0->unk2D0->frame < 68.0f) && (arg0->unk222 != 0)) {
                D_800CC2B2 = (arg0->unk7A - func_15063390(arg0)) - 0x2000;
            }
            anim = 999;
            D_800CC2B4 = 4.0f;
        } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
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
        if ((arg4 != 0) && (act == 0x16)) {
            if (act == arg0->unk31C->unk78) {
                s32 cancel;

                cancel = 1;
                if ((s32)arg0->unk28 == 0) {
                    pressedButtons |= B_BUTTON;
                    anim = func_15063E84(arg0, 0x78, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    func_150836CC(arg0, 0x24);
                    arg0->unk31C->unk11A = 0;
                    arg0->unk83 = 0;
                    arg0->disable_run = 0;
                }
            }
        } else if (act == arg0->unk31C->unk78) {
            arg0->disable_jump = 0xA;
            if ((arg4 != 0) && ((arg0->unk65 != 0) || ((s32)arg0->unk28 != 0))) {
                func_150836CC(arg0, 0x24);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                if ((act == 0x16) || ((act == 0xC) && (arg0->unk28 == 0.0f) && (arg0->health != 0))) {
                    clearAction = 0;
                } else {
                    func_150836CC(arg0, 0x24);
                    arg0->unk31C->unk11A = 0;
                }
            } else if ((pressedButtons & B_BUTTON) || (arg4 != 0)) {
                pressedButtons |= B_BUTTON;
                anim = func_15063E84(arg0, 0x78, 1, 0, pressedButtons);
                if ((anim == 999) && (arg4 != 0)) {
                    func_150836CC(arg0, 0x24);
                    arg0->unk31C->unk11A = 0;
                    arg0->unk83 = 0;
                }
            } else if ((pressedButtons & Z_TRIG) && (arg0->unk84.uh != 0x76) && (arg0->unk31C->unk19E == 0)) {
                if (arg0->unk31C->unk1B2 != 0) {
                    anim = 0x2C7;
                } else if ((act == 0xC) || (D_800BE9F0 == 0x2B) || (D_800BE616 == 0)) {
                    anim = 0x207;
                } else {
                    anim = 0x77;
                }
                blend = 0.0f;
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                if (arg0->unk31C->aimState != 0) {
                    if (arg0->camera != NULL) {
                        func_1512D2E4(arg0->camera, 0);
                    }
                }
                arg0->unk31C->unk19E = 0x78;
            } else if ((arg0->unk31C->aimState != 0) && (arg0->camera != NULL) && (arg0->id == 0x80) && (func_10010894(arg0) == 0) && !(func_150ADA20() & 0x7F)) {
                func_15060A30(func_1000F568(0x568, 4), arg0);
            }
        } else {
            anim = func_15063E84(arg0, 0x76, 1, act, pressedButtons);
        }
        if ((act == 0xC) && ((act == arg0->unk31C->unk78) || (arg0->unk31C->unk11A != 0))) {
            arg0->xz_velocity = 0.0f;
            if ((s32)arg0->unk28 != 0) {
                func_150836CC(arg0, 0x24);
                arg0->unk31C->unk11A = 0;
                arg0->unk31C->unk78 = 0;
            }
        }
        break;
    case 0x19:
    case 0x40:
        {
            s32 animStart;
            s32 animLoop;

            if (act == 0x19) {
                animStart = 0x10E;
                animLoop = 0x110;
            } else {
                animStart = 0x2C2;
                animLoop = 0x2C3;
            }
            if (arg6 != 0) {
                clearAction = 0;
            } else if (act == arg0->unk31C->unk78) {
                if (arg0->unk31C->unk11A == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = func_150641D8(arg0, 0x113, 0x113, 0x119);
                        arg0->disable_run = 0xFF;
                    } else {
                        anim = func_15063E84(arg0, animLoop, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = func_15063E84(arg0, animStart, 1, act, pressedButtons);
            }
        }
        break;
    case 0xE:
        arg0->disable_jump = 0x14;
        anim = 999;
        break;
    case 0xF:
        anim = 999;
        if ((act == arg0->unk31C->unk78) && ((arg6 != 0) || (arg0->unk65 != 0) || (arg4 != 0))) {
            func_150836CC(arg0, 0x3A);
            arg0->unk31C->unk11A = 0;
            arg0->disable_run = 0;
            arg0->unk83 = 0;
        } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            anim = 0xB4;
            if (act == arg0->unk31C->unk78) {
                anim = 0xB6;
            }
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
            arg0->unk31C->unk78 = act;
        }
        if (act == arg0->unk31C->unk78) {
            arg0->disable_jump = 0xA;
            arg0->xz_velocity = 0.0f;
        }
        break;
    case 0x10:
        speed = 1.399999976f;
        if (act == arg0->unk31C->unk78) {
            D_800CC2B2 = D_800CC284->stick_x * 0xC8;
            arg0->disable_jump = 0xA;
            if ((heldButtons & Z_TRIG) && (anim = 0xC1, (arg0->unk84.uh != 0xBE))) {
                speed = 1.0f;
            } else {
                anim = func_15063E84(arg0, 0xC0, 1, 0, pressedButtons);
            }
        } else {
            anim = func_15063E84(arg0, 0xBE, 1, act, pressedButtons);
        }
        break;
    case 0x15:
        if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x4B);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else {
                arg0->disable_jump = 0xA;
                func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                if ((heldButtons & Z_TRIG) && (arg0->unk84.uh != 0x168)) {
                    if (arg0->xz_velocity != 0.0f) {
                        arg0->unkAA = 0x50;
                        arg0->unk31C->unk3C = 3;
                        arg0->unk31C->unk12 = 0;
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
                        arg0->unk31C->unk1AA++;
                        func_1506AC8C(arg0, 9, anims);
                    }
                } else {
                    anim = func_15063E84(arg0, 0xE4, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x168, 1, act, pressedButtons);
        }
        break;
    case 0x18:
    case 0x41:
        {
            s32 animStart;
            s32 animAlt;

            if (act == 0x18) {
                animStart = 0xFB;
                animAlt = 0xFC;
            } else {
                animStart = 0x2C8;
                animAlt = 0x2C9;
            }
            if (act == arg0->unk31C->unk78) {
                if (arg6 != 0) {
                    clearAction = 0;
                } else if ((arg0->unk31C->aimState != 0) && (arg0->unk31C->unk11A == 2)) {
                    arg0->disable_jump = 0xA;
                    arg0->disable_run = 0;
                    arg0->unk83 = 0xA;
                    if ((D_800CC284->button & Z_TRIG) || (arg0->unk31C->prevButtons & Z_TRIG)) {
                        arg0->unk31C->unk1B4 |= 1;
                        D_800CC2B0 = 1;
                    }
                    if (animStart != arg0->unk84.uh) {
                        if (!(D_800CC284->button & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG) && (arg0->unk31C->unk19E == 0)) {
                            arg0->unk31C->unk1B4 |= 2;
                            D_800CC2B0 = 1;
                            blend = 0.0f;
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            anim = 0x10C;
                            arg0->unk31C->unk19E = 0x3C;
                        } else if (arg0->unk84.uh != 0x10C) {
                            anim = 0x10A;
                        }
                    }
                } else {
                    arg0->disable_jump = 0x14;
                    if ((pressedButtons & Z_TRIG) && (animStart != arg0->unk84.uh) && (arg0->unk31C->unk19E == 0)) {
                        anim = animAlt;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        arg0->unk31C->unk19E = 0x3C;
                    } else {
                        anim = func_15063E84(arg0, 0xC6, 1, 0, pressedButtons);
                    }
                }
            } else {
                anim = func_15063E84(arg0, animStart, 1, act, pressedButtons);
            }
        }
        break;
    case 0x11:
        if (pressedButtons & B_BUTTON) {
            u8 pad;  // matching: unused stack slot

            func_15083E90(7)->unk2E4 = 1;
        }
        break;
    case 0x25:
    case 0x2D:
    case 0x2E:
        arg0->unk31C->unk78 = act;
        arg0->disable_jump = 0x14;
        break;
    case 0x28:
        if (arg4 != 0) {
            arg0->unk31C->unk78 = 0;
        } else if ((arg3 <= 0.0f) && (pressedButtons & B_BUTTON)) {
            if (act != arg0->unk31C->unk78) {
                arg0->unk31C->unk78 = act;
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
                Actor *obj;

                old = arg0->unk31C->unk18;
                arg0->unk83 = 0xFF;
                arg0->disable_run = 0xFF;
                if (arg0->unk31C->unk18 >= 0) {
                    arg0->unk31C->unk18 -= 0x14;
                } else {
                    arg0->unk31C->unk18 += 0x14;
                }
                if ((old ^ arg0->unk31C->unk18) & 0x8000) {
                    arg0->unk31C->unk18 = 0;
                }
                if (arg0->unk44 > 10.0f) {
                    s16 diff;

                    diff = arg0->unk31C->unk4C - arg0->unk31C->unk7E;
                    if ((diff < 0x2000) && (diff > -0x2000) && ((diff > 0x100) || (diff < -0x100))) {
                        if (diff & 0x8000) {
                            arg0->unk31C->unk18 -= 0x28;
                        } else {
                            arg0->unk31C->unk18 += 0x28;
                        }
                        if (arg0->unk31C->unk18 > 0x1000) {
                            arg0->unk31C->unk18 = 0x1000;
                        }
                        if (arg0->unk31C->unk18 < -0x1000) {
                            arg0->unk31C->unk18 = -0x1000;
                        }
                    }
                }
                arg0->unk76 = arg0->unk7A = arg0->unk7A + ((arg0->unk31C->unk18 * D_800CC264) / 100);
                if ((arg0->unk31C->unk18 > 0x200) || (arg0->unk31C->unk18 < -0x200)) {
                    after = D_800CC280 - arg0->unk7A;
                    if ((((before < 0x1000) || (before > 0xF000)) && (after >= 0x1000) && (after < 0x7000) && (arg0->unk31C->unk18 < 0)) ||
                        ((before > 0x7000) && (after <= 0x7000) && (arg0->unk31C->unk18 > 0))) {
                        D_800D1580 = -0xFEFA4A;
                        func_1506E5FC();
                    }
                }
                arg0->unk31C->unk7E = arg0->unk31C->unk4C;
                anim = 0x288;
                mag = fabsf(arg0->unk31C->unk18);
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
                if (pressedButtons & B_BUTTON) {
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
            } else if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f) && (arg0->disable_jump == 0)) {
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
            arg0->unk31C->unk11A = 0;
        } else if (arg6 != 0) {
            clearAction = 0;
        } else if (arg3 <= 0.0f) {
            if (act == arg0->unk31C->unk78) {
                if (arg0->unk31C->unk11A == 2) {
                    if (pressedButtons & Z_TRIG) {
                        anim = func_150641D8(arg0, 0x129, 0x12A, 0x128);
                        speed = 1.299999952f;
                        arg0->disable_run = 0xFF;
                    } else {
                        anim = func_15063E84(arg0, 0x121, 1, 0, pressedButtons);
                        speed = 1.399999976f;
                    }
                }
            } else {
                anim = func_15063E84(arg0, 0x120, 1, act, pressedButtons);
                speed = 1.399999976f;
            }
        }
        break;
    case 0x22:
        speed = 1.399999976f;
        if ((arg4 != 0) && (arg0->unk65 != 0)) {
            func_150836CC(arg0, 0x56);
            arg0->unk31C->unk11A = 0;
        } else if (arg6 != 0) {
            clearAction = 0;
        } else if ((act == arg0->unk31C->unk78) && (arg3 <= 0.0f)) {
            if (arg0->unk31C->unk11A == 2) {
                if (pressedButtons & Z_TRIG) {
                    anim = func_150641D8(arg0, 0x15F, 0x15F, 0x15F);
                    speed = 1.299999952f;
                    arg0->disable_run = 0xFF;
                } else {
                    anim = func_15063E84(arg0, 0x15E, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x15D, 1, act, pressedButtons);
        }
        break;
    case 0x24:
        if (arg4 != 0) {
            if (act == arg0->unk31C->unk78) {
                s32 cancel;

                cancel = 1;
                if (((s32)arg0->unk28 == 0) && (arg0->unk31C->unk11A == 2)) {
                    pressedButtons |= B_BUTTON;
                    anim = func_15063E84(arg0, 0x155, 1, 0, pressedButtons);
                    if (anim != 999) {
                        cancel = 0;
                    }
                }
                if (cancel != 0) {
                    func_150836CC(arg0, 0x63);
                    arg0->unk31C->unk11A = 0;
                    arg0->unk83 = 0;
                    arg0->disable_run = 0;
                }
            }
        } else if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x63);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else if ((arg0->unk31C->aimState != 0) && (arg0->unk31C->unk11A == 2) && (arg4 == 0)) {
                arg0->disable_jump = 0xA;
                arg0->disable_run = 0;
                arg0->unk83 = 0xA;
                if ((arg0->unk31C->unk19E == 0) && ((D_800CC284->button & Z_TRIG) || (arg0->unk31C->prevButtons & Z_TRIG))) {
                    arg0->unk31C->unk1B4 |= 1;
                    D_800CC2B0 = 1;
                }
                if (arg0->unk84.uh != 0x152) {
                    if (!(D_800CC284->button & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG) && (arg0->unk84.uh == 0x157) && (arg0->unk31C->unk19E == 0)) {
                        arg0->unk31C->unk1B4 |= 2;
                        D_800CC2B0 = 1;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        arg0->unk31C->unk19E = 0x78;
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
                        arg0->unk31C->unk11A = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg0->unk84.uh != 0x152) && (arg0->unk31C->unk19E == 0)) {
                    anim = 0x154;
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->unk31C->unk19E = 0x78;
                } else {
                    anim = func_15063E84(arg0, 0x155, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x152, 1, act, pressedButtons);
        }
        break;
    case 0x3B:
        {
            s32 held;

            if (D_800C3A58 != 0) {
                if (act == arg0->unk31C->unk78) {
                    func_15083568(arg0, 0x89, 1.0f, 0);
                    arg0->unk31C->unk11A = 2;
                } else {
                    func_150836CC(arg0, 0x89);
                    arg0->unk31C->unk11A = 0;
                }
            }
            if (((act == arg0->unk31C->unk78) || (arg0->unk31C->unk11A != 0)) && (arg0->in_water != 0)) {
                arg0->unk31C->unk78 = 0;
                arg0->disable_run = 0;
                arg0->unk83 = 0;
                func_150836CC(arg0, 0x89);
                arg0->unk31C->unk11A = 0;
            } else if (act == arg0->unk31C->unk78) {
                if ((arg4 != 0) && ((arg0->unk65 != 0) || (arg0->stunned != 0) || (arg0->unk13C != 0))) {
                    func_150836CC(arg0, 0x89);
                    arg0->unk31C->unk11A = 0;
                    arg0->unk31C->unk78 = 0;
                } else if (arg4 != 0) {
                    pressedButtons |= B_BUTTON;
                    anim = func_15063E84(arg0, 0x226, 1, 0, pressedButtons);
                    if (anim == 999) {
                        func_150836CC(arg0, 0x89);
                        arg0->unk31C->unk11A = 0;
                        arg0->unk31C->unk78 = 0;
                        arg0->unk83 = 0;
                    }
                } else if (arg6 != 0) {
                    clearAction = 0;
                } else {
                    arg0->disable_jump = 0x14;
                    func_15063A38(arg0, heldButtons, Z_TRIG | R_TRIG);
                    if ((arg0->unk31C->unk19E == 0) && ((D_800CC290 & Z_TRIG) || (arg0->unk31C->prevButtons & Z_TRIG))) {
                        if (arg0->unk31C->unk198 == 2) {
                            arg0->unk31C->unk1B4 |= 4;
                            D_800CC2B0 = 1;
                        } else {
                            arg0->unk31C->unk1B4 |= 1;
                            D_800CC2B0 = 1;
                        }
                    }
                    if (arg4 != 0) {
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = 0x226;
                        if ((s32)arg0->unk28 != 0) {
                            func_150836CC(arg0, 0x89);
                            arg0->unk31C->unk11A = 0;
                        }
                    } else {
                        held = D_800CC290 & R_TRIG;
                        if ((((held != 0) && !(D_800CC290 & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG)) ||
                                    ((held == 0) && (D_800CC28C & Z_TRIG))) &&
                                   (arg0->unk84.uh != 0x225) && (arg0->unk84.uh != 0x222) && (arg0->unk84.uh != 0x236) &&
                                   (arg0->unk84.uh != 0x227) && (arg0->unk84.uh != 0x237) && (arg0->unk31C->unk19E == 0)) {
                            if (D_800CC284->button & R_TRIG) {
                                if (func_150ADA20() & 3) {
                                    anim = 0x227;
                                } else {
                                    anim = 0x237;
                                }
                                arg0->unk31C->unk1B4 |= 2;
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
                            arg0->unk31C->unk19E = 0x3C;
                        } else if ((held != 0) && (D_800CC290 & Z_TRIG) && (arg0->unk84.uh != 0x225)) {
                            arg0->disable_run = 0;
                            arg0->disable_jump = 8;
                            arg0->unk83 = 8;
                            if ((arg0->unk84.uh != 0x222) && (arg0->unk84.uh != 0x236) && (arg0->unk84.uh != 0x227) && (arg0->unk84.uh != 0x237)) {
                                if ((arg0->unk44 >= 1.0f) || (arg0->unk31C->unk23 > 0)) {
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
                            anim = func_15063E84(arg0, 0x226, 1, 0, pressedButtons);
                        }
                    }
                }
            } else {
                anim = func_15063E84(arg0, 0x225, 1, act, pressedButtons);
            }
        }
        break;
    case 0x12:
        if (((act == arg0->unk31C->unk78) || (arg0->unk31C->unk11A != 0)) && (arg0->in_water != 0)) {
            arg0->unk31C->unk78 = 0;
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            arg0->unk31C->unk11A = 0;
        } else if (act == arg0->unk31C->unk78) {
            arg0->xz_velocity = 0.0f;
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                arg0->unk31C->unk11A = 0;
            } else {
                arg0->disable_jump = 0x14;
                if ((D_800CC284->button & Z_TRIG) || (arg0->unk31C->prevButtons & Z_TRIG)) {
                    if (arg0->unk31C->unk198 == 2) {
                        arg0->unk31C->unk1B4 |= 8;
                        D_800CC2B0 = 1;
                    } else {
                        arg0->unk31C->unk1B4 |= 1;
                        D_800CC2B0 = 1;
                    }
                }
                if (arg4 != 0) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0xC6;
                    if ((s32)arg0->unk28 != 0) {
                        arg0->unk31C->unk11A = 0;
                    }
                } else if (!(D_800CC284->button & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG) && (arg0->unk84.uh != 0xC4) && (arg0->unk31C->unk19E == 0)) {
                    anim = 0xC7;
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->unk31C->unk19E = 0x3C;
                } else if ((D_800CC284->button & Z_TRIG) && (arg0->unk84.uh != 0xC4)) {
                    arg0->disable_run = 0;
                    arg0->disable_jump = 8;
                    arg0->unk83 = 8;
                    if (arg0->unk84.uh != 0xC7) {
                        anim = 0x10A;
                    }
                } else {
                    anim = func_15063E84(arg0, 0xC6, 1, 0, pressedButtons);
                }
            }
        } else {
            anim = func_15063E84(arg0, 0xC4, 1, act, pressedButtons);
            if (anim != 999) {
                arg0->xz_velocity = 0.0f;
            }
        }
        break;
    case 0x23:
        if (act == arg0->unk31C->unk78) {
            if ((arg4 != 0) && (arg0->unk65 != 0)) {
                func_150836CC(arg0, 0x16);
                arg0->unk31C->unk11A = 0;
            } else if (arg6 != 0) {
                clearAction = 0;
            } else if ((arg0->unk31C->aimState != 0) && (arg0->unk31C->unk11A == 2)) {
                arg0->disable_jump = 0xA;
                arg0->disable_run = 0;
                arg0->unk83 = 0xA;
                if ((arg0->unk31C->unk19E == 0) && ((D_800CC284->button & Z_TRIG) || (arg0->unk31C->prevButtons & Z_TRIG))) {
                    arg0->unk31C->unk1B4 |= 1;
                    D_800CC2B0 = 1;
                }
                if (arg0->unk84.uh != 0x163) {
                    if ((arg0->unk31C->unk199 >= 6) || ((pressedButtons & A_BUTTON) && (arg0->unk84.uh != 0x26E))) {
                        if (arg0->unk28 == 0.0f) {
                            arg0->disable_run = 0xFF;
                            arg0->unk83 = 0xFF;
                            arg0->unk31C->unk19E = 0xB4;
                            anim = 0x26E;
                            arg0->unk31C->unk199 = 0;
                        }
                    } else if (!(D_800CC284->button & Z_TRIG) && (arg0->unk31C->prevButtons & Z_TRIG) && (arg0->unk84.uh == 0x165) && (arg0->unk31C->unk19E == 0)) {
                        arg0->unk31C->unk1B4 |= 2;
                        D_800CC2B0 = 1;
                        blend = 0.0f;
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        anim = 0x167;
                        arg0->unk31C->unk199++;
                        arg0->unk31C->unk19E = 0x1E;
                    } else if (arg0->unk84.uh != 0x167) {
                        anim = 0x165;
                    }
                }
            } else {
                arg0->disable_jump = 0x14;
                if ((arg0->unk84.uh != 0x163) && ((arg0->unk31C->unk199 >= 6) || ((pressedButtons & A_BUTTON) && (arg0->unk84.uh != 0x26E)))) {
                    if (arg0->unk28 == 0.0f) {
                        arg0->disable_run = 0xFF;
                        arg0->unk83 = 0xFF;
                        arg0->unk31C->unk19E = 0xB4;
                        anim = 0x26E;
                        arg0->unk31C->unk199 = 0;
                    }
                } else if ((pressedButtons & Z_TRIG) && (arg0->unk84.uh != 0x163) && (arg0->unk84.uh != 0x166) && (arg0->unk31C->unk19E == 0)) {
                    blend = 0.0f;
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    anim = 0x166;
                    arg0->unk31C->unk199++;
                    arg0->unk31C->unk19E = 0x1E;
                } else {
                    anim = func_15063E84(arg0, 0x164, 1, 0, pressedButtons);
                    if (anim == 0x163) {
                        arg0->unk31C->unk199 = 0;
                    }
                }
            }
        } else {
            anim = func_15063E84(arg0, 0x163, 1, act, pressedButtons);
        }
        break;
    case 0x17:
        anim = 999;
        if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
            s32 pad;  // matching: unused stack slot
            s32 ok;
            f32 angle;

            ok = 1;
            if (arg0->unk31C->unk7A != 0) {
                angle = func_15048A70(D_800D3098[arg0->unk31C->unk7A].yaw, arg0->unk40);
                if ((D_800D3098[arg0->unk31C->unk7A].unk18 != 0) && ((angle < -90.0f) || (angle > 90.0f))) {
                    ok = 0;
                }
            }
            if (ok != 0) {
                arg0->disable_run = 0xFF;
                arg0->unk83 = 0xFF;
                arg0->unk31C->unk78 = 0x17;
                anim = 0x8E;
                arg0->unk76 = (u32)(D_800D3098[arg0->unk31C->unk7A].yaw * 182.0444489f) - 0x4000;
            }
        }
        break;
    case 0x1A:
        handled = 1;
        anim = 999;
        if (act != arg0->unk31C->unk78) {
            if ((pressedButtons & B_BUTTON) && (arg3 <= 0.0f)) {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                arg0->xz_velocity = 0.0f;
                anim = 0x12C;
                arg0->unk31C->unk78 = act;
                func_15080828(0);
            }
        } else if (arg6 != 0) {
            func_15080BE8();
            arg0->unk31C->unk78 = 0;
        } else if (func_15080CF4() != 0) {
            if (D_800D1940 == 0x42) {
                arg0->unk31C->unk78 = 0;
                anim = 0x358;
                D_800D2E60[D_800D1940 >> 3] |= 1 << (D_800D1940 & 7);
            } else {
                anim = 0x12E;
            }
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0xFF;
        } else if ((arg0->unk84.uh != 0x12C) && (D_800BE710[D_800C3E78] & 0x8000) && (!(D_800D1958.flags & 0x20) || (func_151F2CDC() == 0))) {
            anim = 0x142;
        }
        break;
    case 0x2A:
        handled = 1;
        anim = 999;
        if (act != arg0->unk31C->unk78) {
            if ((arg3 <= 0.0f) && (arg0->unk110 == 0) && (arg0->disable_jump == 0)) {
                arg0->disable_run = 0xFF;
                arg0->disable_jump = 0xFF;
                arg0->unk83 = 0xFF;
                arg0->xz_velocity = 0.0f;
                arg0->unk31C->unk78 = act;
                anim = 0x26F;
                arg0->unk276 = 6;
                arg0->unk282 = 0x64;
                arg0->unk278 = 0;
                arg0->unk284 = 0;
                func_15080828(1);
            }
        } else if (arg6 != 0) {
            func_15080BE8();
            arg0->unk31C->unk78 = 0;
        } else if (func_15080CF4() != 0) {
            arg0->unk31C->unk78 = 0;
            arg0->disable_run = 0xFF;
            arg0->unk83 = 0;
            func_1507EABC(arg0);
        } else {
            s32 sounds[2] = { 0x39, 0x35 };

            arg0->unk282 = 0x64;
            if ((s8)arg0->unk284 == 0) {  // matching: read signed (lb); Actor declares u8
                arg0->unk278 = (u16)arg0->unk278 + D_800BE9E4;
                if (((func_150ADA20() % 120U) + 0xB4) < (u16)arg0->unk278) {
                    func_1507E7E4(arg0, sounds[func_150ADA20() & 1], 1, (func_150ADA20() % 180U) + 0x1A4, 0);
                    arg0->unk278 = 0;
                    arg0->unk284 = 1;
                }
            } else if (((s8)arg0->unk284 != 0) && (arg0->expression == arg0->defaultExpression)) {
                arg0->unk284 = 0;
            }
            arg0->disable_run = 0xA;
            arg0->disable_jump = 0xA;
            arg0->unk83 = 0xA;
        }
        break;
    case 0x1B:
        {
            Actor *obj;
            s32 animHold;
            s32 animIdle;
            s32 animRelease;
            s32 animGrab;
            s32 animSpecial;
            s32 next;

            obj = &D_800CC2D0[arg0->unk65 - 1];
            arg0->unk31C->unk78 = 0x1B;
            arg0->disable_jump = 0x14;
            if ((D_800BE616 == 0) || (arg0->unk65 != 0)) {
                clearAction = 0;
            }
            if (arg0->unk31C->unk19E == 0) {
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
                if (D_800CC284->button & Z_TRIG) {
                    if (animHold != obj->unk232) {
                        func_10010A3C(obj);
                        func_100109D0(obj);
                    }
                    obj->unk232 = animHold;
                    obj->unk218 = NULL;
                    arg0->unk31C->unk199 = 1;
                    if ((animGrab != obj->unk84.uh) || ((D_800C3A58 != 0) && (animSpecial != 0))) {
                        func_150FC438(obj, arg0, 0, animGrab);
                        func_10010A3C(obj);
                        if ((arg0->id == 0x80) && !(func_150ADA20() & 3)) {
                            func_1000FA64(func_1000F568(0x57C, 6), arg0->x_position, arg0->y_position, arg0->z_position, 0x7FFF, 0x3E8, 0x1F4, func_1000EFB4, arg0, D_800996D0, 0, 0);
                        }
                    }
                    if (D_800BE9E4 < arg0->unk31C->unk24) {
                        arg0->unk31C->unk24 -= D_800BE9E4;
                    } else {
                        next = animRelease;
                        func_100109D0(obj);
                    }
                } else if ((D_800C3A58 != 0) && (animSpecial != obj->unk232) && (animSpecial != 0)) {
                    obj->unk232 = animSpecial;
                    obj->unk218 = NULL;
                } else if (arg0->unk31C->unk199 == 1) {
                    arg0->unk31C->unk199 = 0;
                    if (func_1000FF90(func_1000EC24, -1, -1) == -1) {
                        func_1000FA64((func_150ADA20() % 3U) + 0x618, arg0->x_position, arg0->y_position, arg0->z_position, 0x7D00, 0x1F4, 0xC8, func_1000EC24, (void *)0x1E, 0, 0x300, 0);
                    }
                }
                if (pressedButtons & B_BUTTON) {
                    next = animIdle;
                    func_10010A3C(obj);
                    func_100109D0(obj);
                }
                if (next != 0) {
                    arg0->unk31C->unk24 = 0x258;
                    arg0->unk31C->unk19E = 0x96;
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
            if ((arg0->id == 0x75) || (animDrop = 0x16C, (arg0->id == 0xB1))) {  // matching: IDO schedules the default store between the two tests
                animDrop = 0x16B;
                item = 0x64;
                sfx = 0x11;
            } else {
                animThrow = 0x175;
                item = 0x2C;
                sfx = 0x12;
            }
            if (arg6 != 0) {
                if ((act == arg0->unk31C->unk78) || (arg0->unk31C->unk19B == 4)) {
                    if ((func_1503195C(arg0, item, 0) != NULL) || (arg0->unk31C->unk19B == 4)) {
                        func_150836CC(arg0, item);
                        D_800D1580 = (u16)sfx | 0x10000;
                        func_15073FA0();
                        if (arg0->unk31C->grenade_count != 0) {
                            arg0->unk31C->grenade_count--;
                        }
                    }
                    arg0->unk31C->unk19B = 0;
                    arg0->unk31C->unk11A = 0;
                }
            } else {
                func_15063A38(arg0, heldButtons, R_TRIG);
                if (arg0->unk31C->unk19B == 1) {
                    if (act != arg0->unk31C->unk78) {
                        arg0->unk31C->unk78 = act;
                        arg0->unk31C->unk11A = 1;
                        arg0->disable_run = 0x1E;
                        arg0->disable_jump = 0x1E;
                        arg0->unk83 = 0x1E;
                        arg0->unk31C->unk19B = 2;
                        anim = animThrow;
                        arg0->unk31C->unk19C = 0xF0;
                        arg0->unk31C->unk1AA++;
                    }
                } else if (arg0->unk31C->unk19B == 2) {
                    if (!(D_800CC290 & D_CBUTTONS) && !(arg0->unk2F8 & 0x10)) {
                        arg0->unk31C->unk19B = 3;
                    }
                    if (D_800BE9E4 < arg0->unk31C->unk19C) {
                        arg0->unk31C->unk19C -= D_800BE9E4;
                    } else {
                        arg0->unk31C->unk19B = 0;
                        arg0->unk31C->unk78 = 0;
                        arg0->immune = 0;
                        func_15063B64(arg0);
                    }
                } else if ((arg0->unk31C->unk19B == 3) && (arg3 <= 0.0f) && (animThrow != arg0->unk84.uh)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->unk31C->unk19B = 4;
                    anim = animDrop;
                    arg0->unk31C->unk78 = 0;
                    arg0->unk31C->unk11A = 3;
                    if (arg0->unk31C->grenade_count != 0) {
                        arg0->unk31C->grenade_count--;
                    }
                }
            }
        }
        break;
    case 0x3A:
        {
            Actor *obj;
            s32 item;

            if (D_800BE9F0 == 0x3F) {
                item = 0xBA;
            } else {
                item = 0x89;
            }
            if (arg6 != 0) {
                if (act == arg0->unk31C->unk78) {
                    obj = func_15063C60(arg0, item);
                    if (obj != NULL) {
                        obj->unk65 = 0;
                        obj->gravity = 4.0f;
                        obj->y_velocity = 0.0f;
                        obj->xz_velocity = 0.0f;
                    }
                    arg0->unk31C->unk19B = 0;
                    arg0->unk31C->unk78 = 0;
                    arg0->unk31C->unk11A = 0;
                }
            } else {
                func_15063A38(arg0, heldButtons, R_TRIG);
                if (arg0->unk31C->unk19B == 5) {
                    if (act != arg0->unk31C->unk78) {
                        if (item == 0x89) {
                            obj = func_15083568(arg0, 0x86, 1.0f, 0);
                        } else {
                            obj = func_15083568(arg0, 0x9F, 1.0f, 0);
                        }
                        if (obj != NULL) {
                            arg0->unk31C->unk78 = act;
                            anim = 0x1F9;
                            arg0->unk31C->unk11A = 1;
                            arg0->disable_run = 0x1E;
                            arg0->disable_jump = 0xFF;
                            arg0->unk83 = 0xFF;
                            arg0->unk31C->unk19B = 6;
                            obj->immune = 0xFF;
                            func_15062BDC(obj, 1.0f, 1.0f);
                            obj->unk2E4 = 0xF0;
                            obj->unk2E8 = 0xF0;
                            obj->unk124 = D_800C3E78 + 1;
                            obj->unkF8 = 0x401807;
                            func_1505E650(obj, 2, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                            func_1514F568(obj);
                        } else {
                            arg0->unk31C->unk19B = 0;
                        }
                    }
                } else if (arg0->unk31C->unk19B == 6) {
                    if (!(D_800CC290 & D_CBUTTONS)) {
                        arg0->unk31C->unk19B = 7;
                    }
                } else if ((arg0->unk31C->unk19B == 7) && (arg3 <= 0.0f) && (arg0->unk84.uh != 0x1F9)) {
                    arg0->disable_run = 0xFF;
                    arg0->unk83 = 0xFF;
                    arg0->unk31C->unk19B = 0;
                    anim = 0x1FC;
                    arg0->unk31C->unk78 = 0;
                    arg0->unk31C->unk11A = 0;
                    if (arg0->unk31C->grenade_count != 0) {
                        arg0->unk31C->grenade_count--;
                    }
                    arg0->unk31C->unk1AA++;
                    obj = func_15063C60(arg0, item);
                    if (obj != NULL) {
                        s16 pitch;
                        f32 vx;
                        f32 vy;
                        f32 vz;
                        f32 dist;

                        obj->unk65 = 0;
                        dist = ((240 - obj->unk2E4) * 210.0f) / 240.0f;
                        dist += 40.0f;
                        if (arg0->unk31C->unk198 != 0) {
                            pitch = (30.0f - arg0->unk31C->aimPitch) * 182.0444489f;
                        } else {
                            pitch = 0x1555;
                        }
                        func_1505A184(pitch, dist, 0.0f, &vx, &vy, &vz);
                        obj->y_velocity = -vy;
                        obj->xz_velocity = vx;
                        obj->gravity = 4.0f;
                        obj->unk76 = obj->unk7A = arg0->unk7A - arg0->unk31C->unk12;
                    }
                }
            }
        }
        break;
    case 0x3E:
        if (arg0->team == 0) {
            if (arg0->unk83 == 0) {
                arg0->unk83 = 1;
            }
            if (arg0->unk31C->unk12C >= D_800BE9E4) {
                arg0->unk31C->unk12C -= D_800BE9E4;
                arg0->unkAA = 0x96;
            } else {
                arg0->unkAA = 0x4B;
                arg0->unk31C->unk12C = 0;
            }
        }
        break;
    }

    if ((arg4 != 0) || (arg6 != 0)) {
        arg0->unk31C->unk72 = 0;
        if (arg6 != 0) {
            anim = 0;
            handled = 1;
            if (clearAction != 0) {
                arg0->unk31C->unk78 = 0;
            }
        } else {
            arg0->unk31C->unk75 = 0;
            arg0->unk31C->unk78 = 0;
            if (arg0->unk31C->unk11A != 3) {
                arg0->unk31C->unk11A = 0;
            }
        }
    }
    if ((anim != 0) && (anim != 999) && (arg8 == 0)) {
        func_1505E650(arg0, anim, speed, blend, animParam, 0.0f, 0);
    }
    if (setUnk58 != 0) {
        arg0->unk31C->idle.active = 1;
    }
    if (pendingSound != 0) {
        func_15060A30(pendingSound, arg0);
    }
    if (handled != 0) {
        return 1000;
    } else {
        return anim;
    }
}
