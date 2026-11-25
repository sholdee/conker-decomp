#include <ultra64.h>

// matching: functions.h declares func_150ADA20 as returning u8, but the callers here use its full s32 result
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15060A30(s32, void *);
void func_1507CD64(void *, s32);
void func_1505A770(struct127 *arg0);
void func_15055B0C(struct127 *arg0, s32 arg1);

void func_150536D0(struct127 *obj) {
    obj->interaction_state = 13;
    obj->unkE4 = 0;
    obj->unkE6 = 0;
}

void func_150536E8(struct127 *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
}

void func_1505371C(struct127 *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
}

// Moves the object's scale towards (unk154, unk158) by at most unk15C per call.
void func_15053750(struct127 *obj) {
    f32 diff;
    f32 step;

    diff = obj->unk154 - obj->xz_scale;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            func_15062BDC(obj, obj->unk154, obj->y_scale);
        } else {
            func_15062BDC(obj, (diff < 0.0f ? -1 : 1) * step + obj->xz_scale, obj->y_scale);
        }
    }

    diff = obj->unk158 - obj->y_scale;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            func_15062BDC(obj, obj->xz_scale, obj->unk158);
        } else {
            func_15062BDC(obj, obj->xz_scale, (diff < 0.0f ? -1 : 1) * step + obj->y_scale);
        }
    }
}

void func_15053894(struct127 *obj) {
    obj->unk7F++;
    if (obj->unk7F >= 31) {
        func_15060F28(obj, 1);
    }
}

s32 func_1505C7D8(struct127 *, u8);
void func_15059C84(struct127 *);

typedef struct {
    f32 unk0;
    u8 pad4[0x2B];
    s8 unk2F;
    u8 pad30[0x54];
} Struct800872A0;

extern Struct800872A0 *D_800872A0;
extern f32 D_800D239C;
extern f32 D_800D23A0;
extern f32 D_800D23A4;
extern f32 D_800CC254;
extern f32 D_800CC258;
extern f32 D_800CC25C;
extern s32 D_800CC260;
s32 func_1508855C(struct127 *arg0);
void func_15087350(s32, s32, f32);
struct127 *func_150883B0(s32, s32, f32 *, s32 *);
s32 func_15104634(f32, f32, f32, f32 *);
u8 func_15059B54(struct127 *arg0, u16 arg1);
s32 func_151EF610(void);
s8 func_1505DDA8(struct127 *arg0, s32 arg1, u16 arg2, f32 arg3);
void func_15081690(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, void *arg7, f32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
s32 func_15081E78(struct127 *, void *, s32);
void func_1505A3A8(f32 arg0, struct127 *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_1505D6F0(struct127 *arg0, s32 arg1);
void func_150D7120(f32 *arg0, u8 arg1, s32 arg2);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
void func_150D728C(void *arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4);
void func_15088780(struct127 *);

void func_150538CC(struct127 *arg0) {
    s8 spF7;
    s32 left;
    f32 spEC;
    s32 spE8;
    s32 spE4;
    s32 spE0;
    s32 spDC;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 spC8;
    f32 spC4;
    s32 spC0;
    s32 spBC;
    struct127 *target;

    spF7 = 0;
    spE8 = (u16)arg0->unk278;
    spE0 = (s8)arg0->unk284;
    spE4 = 0;
    if (((spE8 == 2) || (spE8 == 0x12)) && *(u16 *)&arg0->unk282) {
        if (D_800BE9E4 < *(u16 *)&arg0->unk282) {
            arg0->unk282 = *(u16 *)&arg0->unk282 - D_800BE9E4;
        } else {
            /* Dead leftover (stripped debug check?): the value is computed and never used, which is
               what keeps the target's dead `move a0,v0` above (as1 drops the subu but not the copy). */
            left = *(u16 *)&arg0->unk282 - D_800BE9E4;
            if (left + left) {
            }
            arg0->unk282 = 0;
            arg0->gravity = 1.0f;
        }
    }
    if ((spE8 == 0xD) || (spE8 == 0xE)) {
        spDC = func_1508855C(arg0);
        if (spDC != -1) {
            if (spE8 == 0xD) {
                dist = 0.200000003f;
                spC0 = 1;
                spC8 = 200.0f;
            } else {
                dist = 0.0f;
                spC0 = -1;
                spC8 = 30.0f;
            }
            func_15087350(spDC, 0, dist);
            arg0->pad320[8] = 1;
            if (arg0->unk221 == -1) {
                target = func_150883B0(spDC, spC0, &spC4, &spBC);
                if ((arg0->unk124 != 0) && (target + 1 == &D_800CC2D0[arg0->unk124])) {
                    target = NULL;
                }
            } else {
                target = &D_800CC2D0[arg0->unk221];
            }
            if ((target != NULL) && ((target->unk31C->unk120 != 0) || (target->immune > 0))) {
                target = NULL;
                arg0->unk221 = -1;
            }
            if (target != NULL) {
                dx = target->x_position - arg0->x_position;
                dy = target->y_position - arg0->y_position;
                dz = target->z_position - arg0->z_position;
                dist = (dx * dx) + (dz * dz);
            } else {
                spBC = (D_8008FDBC & 0x40) != 0;
                spC4 = 0.5f;
            }
            if (arg0->unk81 < 0xA) {
                target = NULL;
            }
            if ((target == NULL) || (dist > 1000000.0f) || (spE8 == 0xE)) {
                dx = D_800D239C - arg0->x_position;
                dy = (D_800D23A0 + 100.0f) - arg0->y_position;
                dz = D_800D23A4 - arg0->z_position;
                D_800872A0[spDC].unk0 += (spC4 - D_800872A0[spDC].unk0) * 0.125f;
                D_800872A0[spDC].unk2F = spBC;
            } else {
                arg0->unk221 = target - D_800CC2D0;
            }
            dist = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
            if (dist > 1.0f) {
                if (dist < spC8) {
                    spC8 = dist;
                }
                dist = spC8 / dist;
                dx *= dist;
                dy *= dist;
                dz *= dist;
                arg0->unk76 = func_1505A630(-dx, dz, 0);
            }
            arg0->x_position += dx;
            arg0->y_position += dy;
            arg0->z_position += dz;
            {
                f32 spB4;

                if (func_15104634(arg0->x_position, arg0->z_position, arg0->y_position + 50.0f, &spB4) > 0) {
                    if (arg0->y_position < spB4 + 50.0f) {
                        arg0->y_position = spB4 + 50.0f;
                    }
                }
            }
            arg0->disable_run = 1;
            arg0->unk103 = 1;
            arg0->xz_velocity = 0.0f;
        }
        arg0->unkB2++;
        arg0->unkB2 &= 1;
        if (arg0->unkB2 != 0) {
            arg0->unk83++;
        }
        if ((*(s8 *)&D_800E0C20 != 0) || (D_800E0B94 != 0)) {
            arg0->unk83 = 1;
            spF7 = 1;
        }
        if (arg0->unk81 < 0xA) {
            arg0->unk81++;
        }
    }
    if (arg0->unk80 < 0x64) {
        u16 angle;
        struct127 *p;
        f32 spA8;
        f32 spA4;

        if (arg0->unk102 != 0) {
            arg0->unk82 = 0x28;
        }
        arg0->unk78 = func_1505A630(D_800CC2D0[arg0->pad126].x_position - arg0->x_position, arg0->z_position - D_800CC2D0[arg0->pad126].z_position, 0);
        if ((u8)(((s32)(arg0->unk78 - arg0->unk76) >> 8) + arg0->unk82) < (arg0->unk82 * 2)) {
            spA4 = ((D_800CC2D0[arg0->pad126].y_position + (s16)arg0->unk224) - arg0->y_position) * 0.1800000072f;
            spA8 = func_1505A72C(&D_800CC2D0[arg0->pad126], arg0);
            func_15059B54(arg0, 0x300);
            if ((arg0->unk102 != 0) && (spA8 < (arg0->xz_velocity * 16.0f))) {
                if (spA4 > 90.0f) {
                    spA4 = 90.0f;
                } else if (spA4 < -90.0f) {
                    spA4 = -90.0f;
                }
                arg0->y_velocity += (spA4 - arg0->y_velocity) * 0.1800000072f;
            }
        }
    }
    if (arg0->unk82 >= 9) {
        arg0->unk82--;
    }
    if (arg0->unk103 != 0) {
        func_15059C84(arg0);
    }
    switch (spE8) {
    case 2:
    case 12:
    case 18:
        if (arg0->xz_velocity != 0.0f) {
            arg0->unkB8 += 15.0f * (f32)D_800BE9E4;
            if (func_100107F8(arg0) == 0) {
                func_10010154((func_151EF610() % 2) + 0x22F, arg0, 0x7530, 0x1F4, 0x3E8);
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
    case 11:
    case 13:
    case 15:
    case 17:
        arg0->unkB8 = -func_150484A0(arg0->y_velocity, arg0->xz_velocity) * 57.29577637f;
        switch (spE8) {
        case 4:
        case 5:
        case 9:
        case 10:
        case 13:
            if (arg0->unkA8 < 0x1E) {
                arg0->unkA8 += D_800BE9E4;
            } else if (func_100107F8(arg0) == 0) {
                func_10010154((func_150ADA20() % 3U) + 0x2EE, arg0, 0x7530, 0x1F4, 0x3E8);
            }
            break;
        case 3:
        case 6:
        case 11:
        case 17:
            if ((func_1505DDA8(arg0, arg0->unk124, 0x200, 15.0f) != -1) && (arg0->unkA9 == 0)) {
                func_10010154((func_150ADA20() & 3) + 0x43E, arg0, 0x7530, 0x7D0, 0x834);
                arg0->unkA9 = 1;
            }
            break;
        }
        break;
    }
    arg0->stunned = 1;
    if (arg0->unk103 != 0) {
        func_1505A770(arg0);
    }
    D_800CC254 = arg0->x_position;
    D_800CC258 = arg0->y_position;
    D_800CC25C = arg0->z_position;
    spEC = arg0->y_position;
    func_15058898(arg0, arg0->old_y_position);
    func_1505B5F8(arg0, arg0->unk180);
    if (spE0 != 0) {
        f32 dir[3];
        f32 len;
        f32 inv;
        s32 hit;
        struct127 *other;

        dir[0] = arg0->x_position - arg0->old_x_position;
        dir[1] = arg0->y_position - arg0->old_y_position;
        dir[2] = arg0->z_position - arg0->old_z_position;
        len = (dir[0] * dir[0]) + (dir[1] * dir[1]) + (dir[2] * dir[2]);
        len = sqrtf(len);
        if (len != 0.0f) {
            inv = 1.0f / len;
        } else {
            inv = 9.999999747e-05f;
        }
        dir[0] *= inv;
        dir[1] *= inv;
        dir[2] *= inv;
        func_15081690(arg0, arg0->old_x_position, arg0->old_y_position, arg0->old_z_position, dir[0], dir[1], dir[2], &arg0->unk31C->pad98[0x18], len, 0, 0, 0, -1, 0, 0);
        hit = func_15081E78(arg0, &arg0->unk31C->pad98[0x18], spE8);
        if (hit != 0) {
            spF7 = 1;
            if ((hit == 1) && (arg0->unk124 > 0)) {
                other = &D_800CC2D0[arg0->unk124 - 1];
                if ((other->unk31C != NULL) && (other->unk31C->pad98[0x17] != 0)) {
                    if (func_10010894(other) == 0) {
                        func_15060A30((func_150ADA20() % 3U) + 0x53B, other);
                    }
                }
            }
        }
    } else {
        if ((arg0->unk103 != 0) && ((arg0->unkF4 & 0x18000) || (spEC != arg0->y_position))) {
            if (arg0->xz_velocity > 0.0f) {
                spE4 = 1;
                arg0->xz_velocity *= 0.5f;
                if ((spE8 == 0) && (arg0->xz_velocity > 20.0f) && (arg0->unkF4 & 0x10000)) {
                    s32 r;

                    r = func_150ADA20() % 9U;
                    func_15060778((r < 2) ? r + 0x18 : r + 0x29, arg0, 0x7530, 0, 0x1F4, 0x5DC, 5);
                    func_15174690(D_800C3E78, 0, 0x14, 0, 0x199, 4, 0xAA, 0xFF, 0);
                }
            }
        }
        if (arg0->unkD0 != 0) {
            s32 v;

            v = func_1505C7D8(arg0, D_800C3E78);
            if (v != 0) {
                D_800CC260 = v;
                arg0->disable_run = 1;
                spE4 |= 8;
                arg0->xz_velocity *= 0.3000000119f;
                if (spE8 == 0xF) {
                    struct127 *o;

                    o = func_15083E90(6);
                    if (o != NULL) {
                        if (o->stunned != 0) {
                            arg0->unkF8 |= 0x01000000;
                            arg0->unk83 = 6;
                        }
                        arg0->unk76 = func_1505A630(o->x_position - arg0->x_position, arg0->z_position - o->z_position, 0);
                        arg0->y_velocity *= 0.400000006f;
                    }
                } else {
                    s32 t;

                    t = arg0->pad108 & 0x7F;
                    if (t < 0x19) {
                        arg0->unk76 = func_1505A630(arg0->old_x_position - D_800CC2D0[t].x_position, D_800CC2D0[t].z_position - arg0->old_z_position, 0);
                    }
                    arg0->y_velocity = 35.0f;
                    arg0->gravity = 6.0f;
                }
            }
        }
        if (spE8 == 0xF) {
            func_1505A3A8(0.0f, arg0, 1.0f, 1.0f, 0);
            func_1505D6F0(arg0, D_800C3E78);
            if ((D_800CC268 >> 1) != 0) {
                struct127 *o;

                o = func_15083E90(6);
                if (D_800CC268 & (1U << (o - D_800CC2D0))) {
                    D_800D1580 = 0xFF0100A7;
                    func_1506E6FC();
                    if (o != NULL) {
                        arg0->unk76 = func_1505A630(arg0->x_position - o->x_position, o->z_position - arg0->z_position, 0);
                    }
                    arg0->y_velocity = 38.0f;
                    arg0->gravity = 5.0f;
                    arg0->xz_velocity = 40.0f;
                }
            }
        }
        if ((arg0->unk103 != 0) && (arg0->unk28 == 0.0f)) {
            spE4 |= 2;
        }
        if (spE4 != 0) {
            if (spE8 == 0x11) {
                arg0->health = 0;
            }
            if (D_800BE616 != 0) {
                arg0->health = 0;
            }
        }
        arg0->unk40 = (arg0->unk76 + 0x4000) * 0.005493164f;
        if (arg0->unk28 == 0.0f) {
            arg0->unk7F++;
            if (arg0->unk232 != 0) {
                arg0->y_position = arg0->old_y_position;
                spF7 = 1;
            }
        }
    }
    if (arg0->unk7F >= 0x1F) {
        spF7 = 1;
    }
    if (--arg0->unk83 == 0) {
        spF7 = 1;
    }
    if (arg0->health == 0) {
        spF7 = 1;
    }
    if (spF7 != 0) {
        switch (spE8) {
        case 13:
            if (spE4 & 8) {
                func_150D7120(&arg0->x_position, 0xFF, 1);
            } else {
                f32 vec[3];

                vec[0] = D_800CC254 - arg0->old_x_position;
                vec[1] = D_800CC258 - arg0->old_y_position;
                vec[2] = D_800CC25C - arg0->old_z_position;
                if (func_15145128(vec, vec, NULL, NULL) != 0) {
                    func_150D728C(NULL, &arg0->x_position, vec, 0xFF, 1);
                }
            }
            func_15088780(arg0);
            break;
        case 14:
            func_15088780(arg0);
            break;
        }
        func_15060F28(arg0, 1);
        return;
    }
    arg0->unk103 = 1;
}

void func_150548D8(s32 arg0) {
}

void func_150548E4(struct127 *obj) {
    struct127 *owner;
    s32 sfx;
    s32 i;

    owner = &D_800CC2D0[obj->unk65 - 1];

    for (i = 0; i < D_8008FD8C; i++) {
        if ((D_800CC2D0[i].unk274 == (obj - D_800CC2D0) + 1) && (owner->unk232 == 1)) {
            owner->unk218 = NULL;
            if (func_150ADA20() & 1) {
                owner->unk232 = 2;
                sfx = 0x1DB;
            } else {
                owner->unk232 = 3;
                sfx = 0x1DC;
            }
            func_10010344(sfx, owner, 0x7D00, 500, 2500);
        }
    }
}

void func_15054A0C(struct127 *obj) {
    if (obj->unk2D0 != NULL) {
        if ((obj->unk2D0->unk18 - 1.0f) <= obj->unk2D0->unk8) {
            func_15060F28(obj, 0);
        }
    }
}

void func_15054A5C(struct127 *obj, struct127 *other) {
    other->unk31C->unk11A = 0;
    func_151027E8(other);
    func_15060F28(obj, 0);
}

void func_151026BC(void *, s32, s32, s32, s32, s32);
void func_15188810(u8 *, s32, s32);
void func_150836CC(u8 *, s32);
void func_1507C324(void *, void *);

void func_15054A94(u8 *arg0) {
    u8 *anim;
    u8 *other;
    u8 *otherAnim;
    s32 motion;
    s32 attach;
    s32 action;
    s32 hold;
    s32 enter;
    u8 *state;
    s32 index;
    u8 ready;

    anim = *(u8 **)(arg0 + 0x2D0);
    if (anim == NULL || arg0[0x65] == 0) {
        return;
    }
    other = (u8 *)D_800CC2D0 + (arg0[0x65] - 1) * 0x32C;
    otherAnim = *(u8 **)(other + 0x2D0);
    if (otherAnim == NULL) {
        return;
    }
    if (arg0[4] == 0x41) {
        if (*(u16 *)(other + 0x84) != 0x48 && *(u16 *)(other + 0x84) != 0x4B) {
            func_15060F28((struct127 *)arg0, 0);
            return;
        }
        if (*(u16 *)(arg0 + 0x84) == 0 && *(f32 *)(anim + 8) == 0.0f) {
            func_15188810(arg0, 0x64, 0);
        }
        if (*(u16 *)(arg0 + 0x84) == 0 && *(u16 *)(other + 0x84) == 0x4B) {
            func_1505E650((struct127 *)arg0, 1, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        }
        if (*(u16 *)(arg0 + 0x84) == 1 && *(f32 *)(anim + 0x18) - 1.0f <= *(f32 *)(anim + 8)) {
            func_15060F28((struct127 *)arg0, 0);
        }
        return;
    }
    if (arg0[4] != 0x1D && arg0[4] != 0x6E && arg0[4] != 0x2F) {
        return;
    }
    if (arg0[0x65] == 0 || other[0x65] != 0 || other[0x1CA] == 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    state = *(u8 **)(other + 0x31C);
    if (state[0x78] != 0x12 && state[0x78] != 0x18 && state[0x78] != 0x41 && state[0x11A] != 3) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    if (arg0[4] == 0x1D) {
        enter = 0xC4;
        hold = 0xC7;
        action = 0x39;
    } else if (arg0[4] == 0x6E) {
        enter = 0xFB;
        hold = 0xFC;
        action = 0x51;
    } else {
        enter = 0x2C8;
        hold = 0x2C9;
        action = 0x51;
    }
    if (other[0x13C] != 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    if (arg0[4] == 0x1D && other[0x104] != 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    attach = 1;
    if (hold == *(u16 *)(other + 0x84)) {
        motion = 2;
        if (*(s32 *)(arg0 + 0x2E4) == 0) {
            if (*(f32 *)(otherAnim + 8) >= 6.0f && *(f32 *)(otherAnim + 8) < 11.0f) {
                *(s32 *)(arg0 + 0x2E4) = 1;
                func_15083568((struct127 *)arg0, action, 1.0f, 0);
            }
        } else if (*(f32 *)(otherAnim + 8) >= 11.0f) {
            *(s32 *)(arg0 + 0x2E4) = 0;
            func_150836CC(arg0, action);
        }
    } else {
        if (*(s32 *)(arg0 + 0x2E4) == 1) {
            *(s32 *)(arg0 + 0x2E4) = 0;
            func_150836CC(arg0, action);
        }
        if (enter == *(u16 *)(other + 0x84)) {
            motion = 0;
        } else if (*(u16 *)(other + 0x84) == 0xC6) {
            motion = 3;
        } else {
            motion = 1;
            attach = 0;
        }
    }
    func_1505E650((struct127 *)arg0, motion, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    if (attach) {
        func_1507C324(arg0, other);
    }
    state = *(u8 **)(other + 0x31C);
    if (state[0x78] == 0x12) {
        ready = 1;
    } else {
        ready = state[0x197];
        if (state[0x84] == 0) {
            index = (other - (u8 *)D_800CC2D0) / 0x32C;
            if (index < 4 && !(*(*(u16 ***)D_800BE728)[index] & 0x10)) {
                ready = 0;
            }
        }
    }
    if (*(u16 *)(other + 0x84) == 0x10A && *(s16 *)(otherAnim + 0x3C) <= 0 && ready &&
        (*(u16 *)(state + 0x8A) & 0x2000)) {
        if (*(s32 *)(arg0 + 0x2E8) == 0) {
            func_151026BC(other, -1, 3, 1, 0xFF, 1);
            *(s32 *)(arg0 + 0x2E8) = 1;
        }
    } else if (*(s32 *)(arg0 + 0x2E8) == 1) {
        *(s32 *)(arg0 + 0x2E8) = 0;
        func_151027E8(other);
    }
}

void func_15054F74(struct127 *arg0) {
    s16 phi_v1;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s16 x;
    s16 z;

    if (arg0->id == 12) {
        if ((D_800C3E78 + 1) != D_800CC2D0[arg0->unk124].unk65) {
            if (arg0->unk232 == 18) {
                func_15052F9C(arg0, 50.0f, 0, 4, 0, 12, 0, 8, 0, 0);
            } else if (D_800CC268 & 1) {
                if (arg0->unk25C & 0x400) {
                    arg0->unk124 = 0;
                    func_1505327C(arg0, 39.0f, 3.0f, 18, 0);
                }
            }
        }
    } else if (arg0->id == 0x4B) {
        func_1505A184(arg0->unk7A ^ 0x8000, 500.0f, 0.0f, &sp50, &sp48, &sp4C);
        x = (s16)arg0->x_position + sp50;
        z = (s16)arg0->z_position + sp48;
        arg0->unk2EC = (x << 0x10) | (z & 0xFFFF);
        if (arg0->unk84.uh != 0xF) {
            phi_v1 = func_1505A630(D_800CC2D0[0].x_position - arg0->x_position, arg0->z_position - D_800CC2D0[0].z_position, 0);
        } else {
            phi_v1 = arg0->unk7A - 0x800;
        }
        phi_v1 -= arg0->unk7A;
        phi_v1 /= 3;
        arg0->unk7A += phi_v1;
        arg0->xz_scale = 2.0f;
        arg0->y_scale = 2.0f;
        arg0->unk40 = 90.0f;
    label:
        ;
    } else if (((arg0->id == 0x1B) || (arg0->id == 0x7E)) && (arg0->unk2E4 != 0)) {
        if (D_800BE616 != 0) {
            func_1508B20C(arg0->x_position, arg0->y_position, arg0->z_position, 900.0f);
        }
        if (D_800BE9E4 < arg0->unk2E4) {
            if ((arg0->unk2E4 >= 0x78) && (arg0->unk2E4 < (D_800BE9E4 + 0x78))) {
                func_10010154(0x3A1, arg0, 0x7D00, 0x1F4, 0x3E8);
            }
            arg0->unk2E4 -= D_800BE9E4;
        } else {
            arg0->unk232 = 6;
            arg0->unk218 = 0;
        }
    }
}
// void func_15054F74(struct127 *arg0) {
//     s16 phi_v1;
//
//     f32 sp50;
//     f32 sp4C;
//     f32 sp48;
//
//     f32 temp_f2;
//     f32 temp_f0;
//
//     if (arg0->id == 12) {
//         if ((D_800C3E78 + 1) != D_800CC2D0[arg0->unk124].unk65) {
//             if (arg0->unk232 == 18) {
//                 func_15052F9C(arg0, 50.f, 0, 4, 0, 12, 0, 8, 0, 0);
//             } else if ((D_800CC268 & 1) != 0) {
//                 if ((arg0->unk25C & 0x400) != 0) {
//                     arg0->unk124 = 0;
//                     func_1505327C(arg0, 39.0f, 3.0f, 18, 0);
//                 }
//             }
//         }
//     } else {
//         if (arg0->id == 0x4B) {
//             func_1505A184(arg0->unk7A ^ 0x8000, 500.0f, 0, &sp50, &sp48, &sp4C);
//             temp_f0 = arg0->unk14;
//             temp_f2 = arg0->unk1C;
//             arg0->unk2EC = (((s32) ((f32) (s16) (s32) temp_f0 + sp50) << 0x10) | ((s32) ((f32) (s16) (s32) temp_f2 + sp48) & 0xFFFF));
//             if (arg0->unk84.uh  != 0xF) {
//                 phi_v1 = func_1505A630(D_800CC2D0->unk14 - temp_f0, temp_f2 - D_800CC2D0->unk1C, 0);
//             } else {
//                 phi_v1 = (s16) (arg0->unk7A - 0x800);
//             }
//             arg0->unk7A += (s16) (phi_v1 - arg0->unk7A) / 3;
//             arg0->xz_scale = 2.0f;
//             arg0->y_scale = 2.0f;
//             arg0->unk40 = 90.0f;
//         } else if (((arg0->id == 0x1B) || ((arg0->id == 0x7E))) && (arg0->unk2E4 != 0)) {
//             if (D_800BE616 != 0) {
//                 func_1508B20C(arg0->unk14, arg0->unk18, 500.0f, 900.0f);
//             }
//             if ((arg0->unk2E4 > D_800BE9E4) && (arg0->unk2E4 >= 0x78) && (arg0->unk2E4 < (D_800BE9E4 + 0x78))) {
//                 func_10010154(0x3A1, arg0, 0x7D00, 0x1F4, 0x3E8);
//                 arg0->unk2E4 -= D_800BE9E4;
//             } else {
//                 arg0->unk232 = 6U;
//                 arg0->unk218 = 0;
//             }
//         }
//     }
// }

void func_15055260(s32 arg0, s32 arg1, s32 arg2) {
    func_150335C8(arg0, arg1, 0x4D, arg2, 0, 0);
}

void func_1514AD9C(f32 *, s32, s32);
void func_1503192C(struct127 *, s32, s32, s32);
s32 func_1505C7D8(struct127 *, u8);
void func_15059C84(struct127 *);
void func_1505A770(struct127 *);
void func_151DCDE0(struct17 *, s32, s32);

void func_1505528C(struct127 *arg0) {
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    s32 phi_a2;
    s32 sp74;
    f32 sp70;
    s32 sp6C;
    s32 temp_v1;
    s32 sp64;

    sp74 = (u16)arg0->unk278;
    sp6C = 0;
    sp70 = 0.0f;

    switch (sp74) {
        case 0x1F:
            arg0->unkB8 = (arg0->unkB8 + (arg0->xz_velocity * (f32) D_800BE9E4));
            if (arg0->unk2E4 == 1) {
                func_10010344(0x230, arg0, 0x7D00, 0x1F4, 0x9C4);
                arg0->unk2E4 = 0;
            }
            break;
        case 0x42: {
            s32 temp_v0;

            sp70 = 25.0f;
            if ((arg0->gravity != 0.0f) && ((arg0->y_velocity > 0.0f) || (arg0->unk28 > sp70))) {
                arg0->unkB8 = (-func_150484A0(arg0->y_velocity, arg0->xz_velocity) * D_80099408);
            }
            temp_v0 = func_15081574((s32)arg0, 40.0f, 10.0f, (s32)&sp64, 0xF, 0x53);
            if (temp_v0 != 0) {
                func_15055260(arg0, sp64, temp_v0 - 1);
                func_15060F28(arg0, 1);
                return;
            }
            } break;
        case 0x4E:
            sp70 = 25.0f;
            if (arg0->unk28 > sp70) {
                arg0->unkB8 = (arg0->unkB8 + (f32) (D_800BE9E4 * 3));
            }
            break;
        case 0x35:
            arg0->unkB8 = (arg0->unkB8 + (f32) (D_800BE9E4 * 0x10));
            break;
        case 0x88:
            arg0->unkB8 = -func_150484A0(arg0->y_velocity, arg0->xz_velocity) * D_8009940C;
            arg0->unkD0 = 0x10;
            if (func_1505C7D8(arg0, D_800C3E78) != 0) {
                func_15060F28(arg0, 1);
                return;
            }
            break;
    }
    func_15059C84(arg0);
    arg0->stunned = 1;
    func_1505A770(arg0);
    sp84 = arg0->x_position;
    sp80 = arg0->y_position;
    sp7C = arg0->z_position;
    func_15058898(arg0, arg0->old_y_position);
    if ((D_80099410 != arg0->unk118) && (arg0->y_position <= arg0->unk118) && (arg0->unk118 <= arg0->old_y_position)) {
        arg0->xz_velocity *= D_80099414;
        arg0->unk2E4 = 2;
        arg0->gravity = 0.0f;
        arg0->y_velocity = -3.0f;
    } else if ((sp84 != arg0->x_position) || (sp80 != arg0->y_position) || (sp7C != arg0->z_position)) {
        sp6C = 1;
        if ((sp74 == 0x1F) && (arg0->unk2E4 != 2)) {
            func_10010344(((func_150ADA20() % 5U) + 0x3A4), arg0, 0x7D00, 0x1F4, 0x9C4);
        }
    }
    func_1505B5F8(arg0, arg0->unk180);
    if (arg0->unk28 <= sp70) {
        arg0->unk7F++;
        sp6C |= 2;
        if ((arg0->unk184 & 0x1F) == 0xE) {
            func_1514AD9C(&arg0->x_position, 0xFF, 0);
            func_15060F28(arg0, 1);
            return;
        }
        if (sp74 == 0x42) {
            f32 phi_f0;
            f32 temp_f2;

            D_800D1580 = 0xFF0100B0;
            if (arg0->gravity != 0.0f) {
                func_1506E5FC();
            }
            temp_f2 = arg0->y_position;
            arg0->gravity = 0;
            phi_f0 = temp_f2 - arg0->old_y_position;
            arg0->y_velocity = 0;
            arg0->xz_velocity = 0;
            arg0->y_position = arg0->unk180 + sp70;
            if (phi_f0 != 0.0f) {
                phi_f0 = (arg0->y_position - arg0->old_y_position) / phi_f0;
            } else {
                phi_f0 = 0;
            }
            arg0->x_position = arg0->old_x_position + ((arg0->x_position - arg0->old_x_position) * phi_f0);
            arg0->z_position = arg0->old_z_position + ((arg0->z_position - arg0->old_z_position) * phi_f0);
        } else if (arg0->y_position < arg0->unk118) {
            arg0->gravity = 0.0f;
            arg0->y_velocity = 0.0f;
            arg0->xz_velocity = 0.0f;
        } else if ((arg0->y_velocity > 5.0f) && (sp74 == 0x1F) && (arg0->unk2E4 != 2)) {
            func_10010344(0x39E, arg0, 0x7D00, 0x1F4, 0x9C4);
            arg0->unk2E4 = 2;
        }
    }
    arg0->unk40 = ((arg0->unk76 + 0x4000) * 0.005493164f);
    if ((sp6C != 0) && (sp74 == 0x35)) {
        func_10010630(0x360, arg0, 0x4E20, 0x1F4, 0x9C4);
        if (arg0->unk118 < arg0->y_position) {
            struct17 tmp0;

            tmp0.unk0 = arg0->x_position;
            tmp0.unk4 = arg0->y_position + 20.0f;
            tmp0.unk8 = arg0->z_position;
            func_151DCDE0(&tmp0, 0xFF, 1);
        }
        func_15060F28(arg0, 1);
        return;
    }
    temp_v1 = --arg0->unk83;
    if ((temp_v1 == 0) || (arg0->unk7F >= 0x1F)) {
        func_15060F28(arg0, 1);
        return;
    }
    if (temp_v1 < 0xA) {
        phi_a2 = (temp_v1 * 0xFF) / 0xA;
    } else if (arg0->unk7F >= 0x15) {
        phi_a2 = ((-arg0->unk7F * 0xFF) + 0x1DE2) / 0xA;
    } else {
        phi_a2 = 0xFF;
    }
    func_1503192C(arg0, sp74, phi_a2, 0);
    if (sp74 == 0x1F) {
        struct108 *temp_a0;
        f32 temp_f14;

        temp_a0 = D_800DBFF0;
        temp_f14 = ((temp_a0->unk2F8 - arg0->x_position) * (temp_a0->unk2F8 - arg0->x_position)) + ((temp_a0->unk2FC - arg0->y_position) * (temp_a0->unk2FC - arg0->y_position)) + ((temp_a0->unk300 - arg0->z_position) * (temp_a0->unk300 - arg0->z_position));
        if (temp_f14 < 900.0f) {
            func_1512D748(temp_a0, 8, 0);
            func_10010F88(0x2E1, 0x6D60, 0, 0, 0, arg0->x_position, arg0->y_position, arg0->z_position, 0x1F4, 0x3E8);
            func_10010A3C(arg0);
            func_15060F28(arg0, 1);
        }
    }
}

void func_15055A2C(s32 arg0, f32 x, f32 y, f32 z, s32 arg4) {
    s32 sfx;
    s32 range;

    range = 1000;
    if (arg4 == 1) {
        sfx = 1549;
        range = 100;
    } else {
        sfx = D_8009919C[func_150ADA20() % 6U];
    }
    func_10010F88(sfx, 32700, func_150ADA20() % 500U, 0, 0, x, y, z, range, 3000);
}

void func_15055B0C(struct127 *obj, s32 duration) {
    obj->interaction_state = 39;
    func_1505E650(obj, obj->unk84.uh, 0, 0.0f, 0.0f, 0.0f, 0);
    obj->unkE4 = 0;
    obj->unkE6 = 0;
    obj->unk21C = duration;
}

void func_15055B64(struct127 *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    obj->unkF8 |= 0x1000000;

    if (D_800BE9A0 < obj->unk21C) {
        obj->unk21C -= D_800BE9E4;
    } else {
        obj->unk21C = 0;
        if (obj->unk1D4 == NULL) {
            func_15060F28(obj, 1);
        }
    }
}

void func_15055BF8(struct127 *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    obj->unkF8 |= 0x1000000;
    obj->y_position -= D_800BE9A4;

    if (obj->y_position < (obj->unk180 - 100.0f)) {
        func_15060F28(obj, 0);
    }
}

void func_15055C88(struct127 *obj) {
    s32 sfx = 0x2007E4;

    if (!(obj->unk9C & 0x1000) && func_1506E46C(obj, &sfx, 1)) {
        func_15060A30(sfx, obj);
    }
    obj->unk31C->unk36 = 100;
    func_1506160C(obj, 2, 0, 6, 0);
    obj->disable_run = 200;
    obj->unk83 = 200;
    obj->unk1CC = 70000.0f;
    obj->interaction_state = 41;
    obj->gravity = 0.0f;
    obj->y_velocity = 0.0f;
    obj->xz_velocity = 0.0f;
    func_1507CD64(obj, 9);
}

void func_15055D48(struct127 *obj) {
    obj->unk31C->unk36 += D_800BE9A0;
    if (obj->unk31C->unk36 >= 141) {
        D_800D18A0 |= 1 << (obj - D_800CC2D0);
        if (D_800D18A0 != 0) {
            obj->immune = 0xFF;
            obj->stunned = 0xFF;
            obj->disable_run = 0xFF;
            obj->unkF8 |= 0x200;
            obj->unkE4 = 0;
            obj->unkE6 = 0;
            obj->unk7 = 0;
            obj->health = 0;
            obj->interaction_state = 5;
            obj->xz_velocity = 0.0f;
            obj->y_velocity = 0.0f;
            obj->gravity = 0.0f;
            return;
        }
        func_1506D538();
    }
    obj->health = 0;
    obj->xz_scale *= 0.9499999881f;
    obj->y_scale *= 0.9499999881f;
    obj->xz_velocity *= 0.9499999881f;
}
