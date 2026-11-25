#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15052F9C(Actor *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(Actor *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15060A30(s32, void *);
void func_1507CD64(void *, s32);
void func_1505A770(Actor *arg0);
void func_15055B0C(Actor *arg0, s32 arg1);
void func_151026BC(void *, s32, s32, s32, s32, s32);
void func_15188810(void *, s32, s32);
void func_150836CC(Actor *, s32);
void func_1507C324(void *, void *);
void func_1514AD9C(f32 *, s32, s32);
void func_1503192C(Actor *, s32, s32, s32);
void func_151DCDE0(Vec3f *, s32, s32);

s32 func_1505C7D8(Actor *, u8);
void func_15059C84(Actor *);

typedef struct {
    f32 unk0;
    u8 pad4[0x2B];
    s8 unk2F;
    u8 pad30[0x54];
} Struct800872A0;

extern Struct800872A0 *D_800872A0;
s32 func_1508855C(Actor *arg0);
void func_15087350(s32, s32, f32);
Actor *func_150883B0(s32, s32, f32 *, s32 *);
s32 func_15104634(f32, f32, f32, f32 *);
u8 func_15059B54(Actor *arg0, u16 arg1);
s32 func_151EF610(void);
s8 func_1505DDA8(Actor *arg0, s32 arg1, u16 arg2, f32 arg3);
void func_15081690(Actor *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, void *arg7, f32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
s32 func_15081E78(Actor *, void *, s32);
void func_1505A3A8(f32 arg0, Actor *arg1, f32 arg2, f32 arg3, u8 arg4);
void func_1505D6F0(Actor *arg0, s32 arg1);
void func_150D7120(f32 *arg0, u8 arg1, s32 arg2);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);
void func_150D728C(void *arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4);
void func_15088780(Actor *);

void func_150536D0(Actor *obj) {
    obj->interaction_state = 13;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
}

void func_150536E8(Actor *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
}

void func_1505371C(Actor *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
}

// Moves the object's scale towards (unk154, unk158) by at most unk15C per call.
void func_15053750(Actor *obj) {
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

void func_15053894(Actor *obj) {
    obj->unk7F++;
    if (obj->unk7F >= 31) {
        func_15060F28(obj, 1);
    }
}

void func_150538CC(Actor *obj) {
    s8 done;
    s32 left;
    f32 prevY;
    s32 kind;
    s32 hitFlags;
    s32 sweep;
    s32 slot;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 maxStep;
    f32 slotValue;
    s32 searchDir;
    s32 slotFlag;
    Actor *target;

    done = 0;
    kind = (u16)obj->unk278;
    sweep = (s8)obj->unk284;
    hitFlags = 0;
    if (((kind == 2) || (kind == 18)) && (u16)obj->unk282) {
        if (D_800BE9E4 < (u16)obj->unk282) {
            obj->unk282 = (u16)obj->unk282 - D_800BE9E4;
        } else {
            // matching: dead leftover; keeps the target's dead `move a0,v0` (as1 drops the subu, not the copy)
            left = (u16)obj->unk282 - D_800BE9E4;
            if (left + left) {
            }
            obj->unk282 = 0;
            obj->gravity = 1.0f;
        }
    }
    if ((kind == 13) || (kind == 14)) {
        slot = func_1508855C(obj);
        if (slot != -1) {
            if (kind == 13) {
                dist = 0.200000003f;
                searchDir = 1;
                maxStep = 200.0f;
            } else {
                dist = 0.0f;
                searchDir = -1;
                maxStep = 30.0f;
            }
            func_15087350(slot, 0, dist);
            obj->unk328 = 1; // 0x328
            if (obj->unk221 == -1) {
                target = func_150883B0(slot, searchDir, &slotValue, &slotFlag);
                if ((obj->unk124 != 0) && (target + 1 == &D_800CC2D0[obj->unk124])) {
                    target = NULL;
                }
            } else {
                target = &D_800CC2D0[obj->unk221];
            }
            if ((target != NULL) && ((target->unk31C->unk120 != 0) || (target->immune > 0))) {
                target = NULL;
                obj->unk221 = -1;
            }
            if (target != NULL) {
                dx = target->x_position - obj->x_position;
                dy = target->y_position - obj->y_position;
                dz = target->z_position - obj->z_position;
                dist = (dx * dx) + (dz * dz);
            } else {
                slotFlag = (D_8008FDBC & 0x40) != 0;
                slotValue = 0.5f;
            }
            if (obj->unk81 < 10) {
                target = NULL;
            }
            if ((target == NULL) || (dist > 1000000.0f) || (kind == 14)) {
                dx = D_800D239C - obj->x_position;
                dy = (D_800D23A0 + 100.0f) - obj->y_position;
                dz = D_800D23A4 - obj->z_position;
                D_800872A0[slot].unk0 += (slotValue - D_800872A0[slot].unk0) * 0.125f;
                D_800872A0[slot].unk2F = slotFlag;
            } else {
                obj->unk221 = target - D_800CC2D0;
            }
            dist = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
            if (dist > 1.0f) {
                if (dist < maxStep) {
                    maxStep = dist;
                }
                dist = maxStep / dist;
                dx *= dist;
                dy *= dist;
                dz *= dist;
                obj->unk76 = func_1505A630(-dx, dz, 0);
            }
            obj->x_position += dx;
            obj->y_position += dy;
            obj->z_position += dz;
            {
                f32 groundY;

                if (func_15104634(obj->x_position, obj->z_position, obj->y_position + 50.0f, &groundY) > 0) {
                    if (obj->y_position < groundY + 50.0f) {
                        obj->y_position = groundY + 50.0f;
                    }
                }
            }
            obj->disable_run = 1;
            obj->unk103 = 1;
            obj->xz_velocity = 0.0f;
        }
        obj->unkB2++;
        obj->unkB2 &= 1;
        if (obj->unkB2 != 0) {
            obj->unk83++;
        }
        if ((D_800E0C20 != 0) || (D_800E0B94 != 0)) {
            obj->unk83 = 1;
            done = 1;
        }
        if (obj->unk81 < 10) {
            obj->unk81++;
        }
    }
    if (obj->unk80 < 100) {
        s32 pad[2]; // matching: unused; shifts the block's stack slots to the target's offsets
        f32 ownerDist;
        f32 climb;

        if (obj->unk102 != 0) {
            obj->unk82 = 40;
        }
        obj->unk78 = func_1505A630(D_800CC2D0[obj->unk126].x_position - obj->x_position, obj->z_position - D_800CC2D0[obj->unk126].z_position, 0);
        if ((u8)(((s32)(obj->unk78 - obj->unk76) >> 8) + obj->unk82) < (obj->unk82 * 2)) {
            climb = ((D_800CC2D0[obj->unk126].y_position + (s16)obj->unk224) - obj->y_position) * 0.1800000072f;
            ownerDist = func_1505A72C(&D_800CC2D0[obj->unk126], obj);
            func_15059B54(obj, 0x300);
            if ((obj->unk102 != 0) && (ownerDist < (obj->xz_velocity * 16.0f))) {
                if (climb > 90.0f) {
                    climb = 90.0f;
                } else if (climb < -90.0f) {
                    climb = -90.0f;
                }
                obj->y_velocity += (climb - obj->y_velocity) * 0.1800000072f;
            }
        }
    }
    if (obj->unk82 >= 9) {
        obj->unk82--;
    }
    if (obj->unk103 != 0) {
        func_15059C84(obj);
    }
    switch (kind) {
    case 2:
    case 12:
    case 18:
        if (obj->xz_velocity != 0.0f) {
            obj->unkB8 += 15.0f * D_800BE9E4;
            if (func_100107F8(obj) == 0) {
                func_10010154((func_151EF610() % 2) + 0x22F, obj, 30000, 500, 1000);
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
        obj->unkB8 = -func_150484A0(obj->y_velocity, obj->xz_velocity) * 57.29577637f;
        switch (kind) {
        case 4:
        case 5:
        case 9:
        case 10:
        case 13:
            if (obj->unkA8 < 30) {
                obj->unkA8 += D_800BE9E4;
            } else if (func_100107F8(obj) == 0) {
                func_10010154((func_150ADA20() % 3U) + 0x2EE, obj, 30000, 500, 1000);
            }
            break;
        case 3:
        case 6:
        case 11:
        case 17:
            if ((func_1505DDA8(obj, obj->unk124, 0x200, 15.0f) != -1) && (obj->unkA9 == 0)) {
                func_10010154((func_150ADA20() & 3) + 0x43E, obj, 30000, 2000, 2100);
                obj->unkA9 = 1;
            }
            break;
        }
        break;
    }
    obj->stunned = 1;
    if (obj->unk103 != 0) {
        func_1505A770(obj);
    }
    D_800CC254 = obj->x_position;
    D_800CC258 = obj->y_position;
    D_800CC25C = obj->z_position;
    prevY = obj->y_position;
    func_15058898(obj, obj->old_y_position);
    func_1505B5F8(obj, obj->floorY);
    if (sweep != 0) {
        f32 dir[3];
        f32 len;
        f32 inv;
        s32 hit;
        Actor *owner;

        dir[0] = obj->x_position - obj->old_x_position;
        dir[1] = obj->y_position - obj->old_y_position;
        dir[2] = obj->z_position - obj->old_z_position;
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
        func_15081690(obj, obj->old_x_position, obj->old_y_position, obj->old_z_position, dir[0], dir[1], dir[2], &obj->unk31C->unkB0, len, 0, 0, 0, -1, 0, 0);
        hit = func_15081E78(obj, &obj->unk31C->unkB0, kind);
        if (hit != 0) {
            done = 1;
            if ((hit == 1) && (obj->unk124 > 0)) {
                owner = &D_800CC2D0[obj->unk124 - 1];
                if ((owner->unk31C != NULL) && (owner->unk31C->unkAF != 0)) {
                    if (func_10010894(owner) == 0) {
                        func_15060A30((func_150ADA20() % 3U) + 0x53B, owner);
                    }
                }
            }
        }
    } else {
        if ((obj->unk103 != 0) && ((obj->unkF4 & 0x18000) || (prevY != obj->y_position))) {
            if (obj->xz_velocity > 0.0f) {
                hitFlags = 1;
                obj->xz_velocity *= 0.5f;
                if ((kind == 0) && (obj->xz_velocity > 20.0f) && (obj->unkF4 & 0x10000)) {
                    s32 sfx;

                    sfx = func_150ADA20() % 9U;
                    func_15060778((sfx < 2) ? sfx + 0x18 : sfx + 0x29, obj, 30000, 0, 500, 1500, 5);
                    func_15174690(D_800C3E78, 0, 0x14, 0, 0x199, 4, 0xAA, 0xFF, 0);
                }
            }
        }
        if (obj->unkD0 != 0) {
            s32 hit;

            hit = func_1505C7D8(obj, D_800C3E78);
            if (hit != 0) {
                D_800CC260 = hit;
                obj->disable_run = 1;
                hitFlags |= 8;
                obj->xz_velocity *= 0.3000000119f;
                if (kind == 15) {
                    Actor *other;

                    other = func_15083E90(6);
                    if (other != NULL) {
                        if (other->stunned != 0) {
                            obj->unkF8 |= 0x01000000;
                            obj->unk83 = 6;
                        }
                        obj->unk76 = func_1505A630(other->x_position - obj->x_position, obj->z_position - other->z_position, 0);
                        obj->y_velocity *= 0.400000006f;
                    }
                } else {
                    s32 attacker;

                    attacker = obj->unk108 & 0x7F;
                    if (attacker < 25) {
                        obj->unk76 = func_1505A630(obj->old_x_position - D_800CC2D0[attacker].x_position, D_800CC2D0[attacker].z_position - obj->old_z_position, 0);
                    }
                    obj->y_velocity = 35.0f;
                    obj->gravity = 6.0f;
                }
            }
        }
        if (kind == 15) {
            func_1505A3A8(0.0f, obj, 1.0f, 1.0f, 0);
            func_1505D6F0(obj, D_800C3E78);
            if ((D_800CC268 >> 1) != 0) {
                Actor *other;

                other = func_15083E90(6);
                if (D_800CC268 & (1U << (other - D_800CC2D0))) {
                    D_800D1580 = 0xFF0100A7;
                    func_1506E6FC();
                    if (other != NULL) {
                        obj->unk76 = func_1505A630(obj->x_position - other->x_position, other->z_position - obj->z_position, 0);
                    }
                    obj->y_velocity = 38.0f;
                    obj->gravity = 5.0f;
                    obj->xz_velocity = 40.0f;
                }
            }
        }
        if ((obj->unk103 != 0) && (obj->unk28 == 0.0f)) {
            hitFlags |= 2;
        }
        if (hitFlags != 0) {
            if (kind == 17) {
                obj->health = 0;
            }
            if (D_800BE616 != 0) {
                obj->health = 0;
            }
        }
        obj->unk40 = (obj->unk76 + 0x4000) * 0.005493164f;
        if (obj->unk28 == 0.0f) {
            obj->unk7F++;
            if (obj->unk232 != 0) {
                obj->y_position = obj->old_y_position;
                done = 1;
            }
        }
    }
    if (obj->unk7F >= 31) {
        done = 1;
    }
    if (--obj->unk83 == 0) {
        done = 1;
    }
    if (obj->health == 0) {
        done = 1;
    }
    if (done != 0) {
        switch (kind) {
        case 13:
            if (hitFlags & 8) {
                func_150D7120(&obj->x_position, 0xFF, 1);
            } else {
                f32 vec[3];

                vec[0] = D_800CC254 - obj->old_x_position;
                vec[1] = D_800CC258 - obj->old_y_position;
                vec[2] = D_800CC25C - obj->old_z_position;
                if (func_15145128(vec, vec, NULL, NULL) != 0) {
                    func_150D728C(NULL, &obj->x_position, vec, 0xFF, 1);
                }
            }
            func_15088780(obj);
            break;
        case 14:
            func_15088780(obj);
            break;
        }
        func_15060F28(obj, 1);
        return;
    }
    obj->unk103 = 1;
}

void func_150548D8(s32 arg0) {
}

void func_150548E4(Actor *obj) {
    Actor *owner;
    s32 sfx;
    s32 i;

    owner = &D_800CC2D0[obj->unk65 - 1];

    for (i = 0; i < D_8008FD8C; i++) {
        if ((D_800CC2D0[i].objectNum == (obj - D_800CC2D0) + 1) && (owner->unk232 == 1)) {
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

void func_15054A0C(Actor *obj) {
    if (obj->unk2D0 != NULL) {
        if ((obj->unk2D0->numFrames - 1.0f) <= obj->unk2D0->frame) {
            func_15060F28(obj, 0);
        }
    }
}

void func_15054A5C(Actor *obj, Actor *other) {
    other->unk31C->unk11A = 0;
    func_151027E8(other);
    func_15060F28(obj, 0);
}

void func_15054A94(Actor *obj) {
    AnimInstance *anim;
    Actor *other;
    AnimInstance *otherAnim;
    s32 motion;
    s32 attach;
    s32 action;
    s32 hold;
    s32 enter;
    ActorCtl *state;
    s32 index;
    u8 ready;

    anim = obj->unk2D0;
    if (anim == NULL || obj->unk65 == 0) {
        return;
    }
    other = &D_800CC2D0[obj->unk65 - 1];
    otherAnim = other->unk2D0;
    if (otherAnim == NULL) {
        return;
    }
    if (obj->id == 0x41) {
        if (other->unk84.uh != 0x48 && other->unk84.uh != 0x4B) {
            func_15060F28(obj, 0);
            return;
        }
        if (obj->unk84.uh == 0 && anim->frame == 0.0f) {
            func_15188810(obj, 100, 0);
        }
        if (obj->unk84.uh == 0 && other->unk84.uh == 0x4B) {
            func_1505E650(obj, 1, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        }
        if (obj->unk84.uh == 1 && anim->numFrames - 1.0f <= anim->frame) {
            func_15060F28(obj, 0);
        }
        return;
    }
    if (obj->id != 0x1D && obj->id != 0x6E && obj->id != 0x2F) {
        return;
    }
    if (obj->unk65 == 0 || other->unk65 != 0 || other->health == 0) {
        func_15054A5C(obj, other);
        return;
    }
    state = other->unk31C;
    if (state->unk78 != 0x12 && state->unk78 != 0x18 && state->unk78 != 0x41 && state->unk11A != 3) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->id == 0x1D) {
        enter = 0xC4;
        hold = 0xC7;
        action = 0x39;
    } else if (obj->id == 0x6E) {
        enter = 0xFB;
        hold = 0xFC;
        action = 0x51;
    } else {
        enter = 0x2C8;
        hold = 0x2C9;
        action = 0x51;
    }
    if (other->unk13C != 0) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->id == 0x1D && other->stunned != 0) {
        func_15054A5C(obj, other);
        return;
    }
    attach = 1;
    if (hold == other->unk84.uh) {
        motion = 2;
        if (obj->unk2E4 == 0) {
            if (otherAnim->frame >= 6.0f && otherAnim->frame < 11.0f) {
                obj->unk2E4 = 1;
                func_15083568(obj, action, 1.0f, 0);
            }
        } else if (otherAnim->frame >= 11.0f) {
            obj->unk2E4 = 0;
            func_150836CC(obj, action);
        }
    } else {
        if (obj->unk2E4 == 1) {
            obj->unk2E4 = 0;
            func_150836CC(obj, action);
        }
        if (enter == other->unk84.uh) {
            motion = 0;
        } else if (other->unk84.uh == 0xC6) {
            motion = 3;
        } else {
            motion = 1;
            attach = 0;
        }
    }
    func_1505E650(obj, motion, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    if (attach) {
        func_1507C324(obj, other);
    }
    state = other->unk31C;
    if (state->unk78 == 0x12) {
        ready = 1;
    } else {
        ready = state->aimState;
        if (state->unk84 == 0) {
            index = other - D_800CC2D0;
            // D_800BE728[0] points to the per-player controller pads
            if (index < 4 && !(D_800BE728[index]->button & R_TRIG)) {
                ready = 0;
            }
        }
    }
    if (other->unk84.uh == 0x10A && otherAnim->unk3C <= 0 && ready &&
        (state->buttons & 0x2000)) {
        if (obj->unk2E8 == 0) {
            func_151026BC(other, -1, 3, 1, 0xFF, 1);
            obj->unk2E8 = 1;
        }
    } else if (obj->unk2E8 == 1) {
        obj->unk2E8 = 0;
        func_151027E8(other);
    }
}

void func_15054F74(Actor *obj) {
    s16 angle;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    s16 x;
    s16 z;

    if (obj->id == 12) {
        if ((D_800C3E78 + 1) != D_800CC2D0[obj->unk124].unk65) {
            if (obj->unk232 == 18) {
                func_15052F9C(obj, 50.0f, 0, 4, 0, 12, 0, 8, 0, 0);
            } else if (D_800CC268 & 1) {
                if (obj->unk25C & 0x400) {
                    obj->unk124 = 0;
                    func_1505327C(obj, 39.0f, 3.0f, 18, 0);
                }
            }
        }
    } else if (obj->id == 0x4B) {
        func_1505A184(obj->unk7A ^ 0x8000, 500.0f, 0.0f, &offsetX, &offsetZ, &offsetY);
        x = (s16)obj->x_position + offsetX;
        z = (s16)obj->z_position + offsetZ;
        obj->unk2EC = (x << 16) | (z & 0xFFFF);
        if (obj->unk84.uh != 15) {
            angle = func_1505A630(D_800CC2D0[0].x_position - obj->x_position, obj->z_position - D_800CC2D0[0].z_position, 0);
        } else {
            angle = obj->unk7A - 0x800;
        }
        angle -= obj->unk7A;
        angle /= 3;
        obj->unk7A += angle;
        obj->xz_scale = 2.0f;
        obj->y_scale = 2.0f;
        obj->unk40 = 90.0f;
    // matching: an empty labelled statement ends the block so as1 keeps the last store out of the branch delay slot
    label:
        ;
    } else if (((obj->id == 0x1B) || (obj->id == 0x7E)) && (obj->unk2E4 != 0)) {
        if (D_800BE616 != 0) {
            func_1508B20C(obj->x_position, obj->y_position, obj->z_position, 900.0f);
        }
        if (D_800BE9E4 < obj->unk2E4) {
            if ((obj->unk2E4 >= 120) && (obj->unk2E4 < (D_800BE9E4 + 120))) {
                func_10010154(0x3A1, obj, 0x7D00, 500, 1000);
            }
            obj->unk2E4 -= D_800BE9E4;
        } else {
            obj->unk232 = 6;
            obj->unk218 = NULL;
        }
    }
}

void func_15055260(Actor *obj, Actor *target, s32 hitIndex) {
    func_150335C8(obj, target, 0x4D, hitIndex, 0, 0);
}

void func_1505528C(Actor *obj) {
    f32 prevX;
    f32 prevY;
    f32 prevZ;
    s32 alpha;
    s32 kind;
    f32 hoverHeight;
    s32 hitFlags;
    s32 timer;
    Actor *target;

    kind = (u16)obj->unk278;
    hitFlags = 0;
    hoverHeight = 0.0f;

    switch (kind) {
    case 0x1F:
        obj->unkB8 += obj->xz_velocity * D_800BE9E4;
        if (obj->unk2E4 == 1) {
            func_10010344(0x230, obj, 0x7D00, 500, 2500);
            obj->unk2E4 = 0;
        }
        break;
    case 0x42: {
        s32 hit;

        hoverHeight = 25.0f;
        if ((obj->gravity != 0.0f) && ((obj->y_velocity > 0.0f) || (obj->unk28 > hoverHeight))) {
            obj->unkB8 = -func_150484A0(obj->y_velocity, obj->xz_velocity) * 57.29577637f;
        }
        hit = func_15081574((s32)obj, 40.0f, 10.0f, (s32)&target, 0xF, 0x53);
        if (hit != 0) {
            func_15055260(obj, target, hit - 1);
            func_15060F28(obj, 1);
            return;
        }
        break;
    }
    case 0x4E:
        hoverHeight = 25.0f;
        if (obj->unk28 > hoverHeight) {
            obj->unkB8 += D_800BE9E4 * 3;
        }
        break;
    case 0x35:
        obj->unkB8 += D_800BE9E4 * 16;
        break;
    case 0x88:
        obj->unkB8 = -func_150484A0(obj->y_velocity, obj->xz_velocity) * 57.29577637f;
        obj->unkD0 = 16;
        if (func_1505C7D8(obj, D_800C3E78) != 0) {
            func_15060F28(obj, 1);
            return;
        }
        break;
    }
    func_15059C84(obj);
    obj->stunned = 1;
    func_1505A770(obj);
    prevX = obj->x_position;
    prevY = obj->y_position;
    prevZ = obj->z_position;
    func_15058898(obj, obj->old_y_position);
    if ((obj->unk118 != -10000.0f) && (obj->y_position <= obj->unk118) && (obj->unk118 <= obj->old_y_position)) {
        obj->xz_velocity *= 0.1000000015f;
        obj->unk2E4 = 2;
        obj->gravity = 0.0f;
        obj->y_velocity = -3.0f;
    } else if ((prevX != obj->x_position) || (prevY != obj->y_position) || (prevZ != obj->z_position)) {
        hitFlags = 1;
        if ((kind == 0x1F) && (obj->unk2E4 != 2)) {
            func_10010344((func_150ADA20() % 5U) + 0x3A4, obj, 0x7D00, 500, 2500);
        }
    }
    func_1505B5F8(obj, obj->floorY);
    if (obj->unk28 <= hoverHeight) {
        obj->unk7F++;
        hitFlags |= 2;
        if ((obj->unk184 & 0x1F) == 0xE) {
            func_1514AD9C(&obj->x_position, 0xFF, 0);
            func_15060F28(obj, 1);
            return;
        }
        if (kind == 0x42) {
            f32 t;
            f32 y;

            D_800D1580 = 0xFF0100B0;
            if (obj->gravity != 0.0f) {
                func_1506E5FC();
            }
            // matching: the int 0 stores below (not 0.0f) steer where IDO materializes its shared FP zero
            y = obj->y_position;
            obj->gravity = 0;
            t = y - obj->old_y_position;
            obj->y_velocity = 0;
            obj->xz_velocity = 0;
            obj->y_position = obj->floorY + hoverHeight;
            if (t != 0.0f) {
                t = (obj->y_position - obj->old_y_position) / t;
            } else {
                t = 0;
            }
            obj->x_position = obj->old_x_position + ((obj->x_position - obj->old_x_position) * t);
            obj->z_position = obj->old_z_position + ((obj->z_position - obj->old_z_position) * t);
        } else if (obj->y_position < obj->unk118) {
            obj->gravity = 0.0f;
            obj->y_velocity = 0.0f;
            obj->xz_velocity = 0.0f;
        } else if ((obj->y_velocity > 5.0f) && (kind == 0x1F) && (obj->unk2E4 != 2)) {
            func_10010344(0x39E, obj, 0x7D00, 500, 2500);
            obj->unk2E4 = 2;
        }
    }
    obj->unk40 = (obj->unk76 + 0x4000) * 0.005493164f;
    if ((hitFlags != 0) && (kind == 0x35)) {
        func_10010630(0x360, obj, 0x4E20, 500, 2500);
        if (obj->unk118 < obj->y_position) {
            Vec3f pos;

            pos.x = obj->x_position;
            pos.y = obj->y_position + 20.0f;
            pos.z = obj->z_position;
            func_151DCDE0(&pos, 0xFF, 1);
        }
        func_15060F28(obj, 1);
        return;
    }
    timer = --obj->unk83;
    if ((timer == 0) || (obj->unk7F >= 31)) {
        func_15060F28(obj, 1);
        return;
    }
    if (timer < 10) {
        alpha = (timer * 255) / 10;
    } else if (obj->unk7F >= 21) {
        alpha = ((30 - obj->unk7F) * 255) / 10;
    } else {
        alpha = 255;
    }
    func_1503192C(obj, kind, alpha, 0);
    if (kind == 0x1F) {
        Camera *player;
        f32 distSq;

        player = D_800DBFF0;
        distSq = ((player->eye.x - obj->x_position) * (player->eye.x - obj->x_position)) +
                 ((player->eye.y - obj->y_position) * (player->eye.y - obj->y_position)) +
                 ((player->eye.z - obj->z_position) * (player->eye.z - obj->z_position));
        if (distSq < 900.0f) {
            func_1512D748(player, 8, 0);
            func_10010F88(0x2E1, 0x6D60, 0, 0, 0, obj->x_position, obj->y_position, obj->z_position, 500, 1000);
            func_10010A3C(obj);
            func_15060F28(obj, 1);
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

void func_15055B0C(Actor *obj, s32 duration) {
    obj->interaction_state = 39;
    func_1505E650(obj, obj->unk84.uh, 0, 0.0f, 0.0f, 0.0f, 0);
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->unk21C = duration;
}

void func_15055B64(Actor *obj) {
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

void func_15055BF8(Actor *obj) {
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    obj->unkF8 |= 0x1000000;
    obj->y_position -= D_800BE9A4;

    if (obj->y_position < (obj->floorY - 100.0f)) {
        func_15060F28(obj, 0);
    }
}

void func_15055C88(Actor *obj) {
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

void func_15055D48(Actor *obj) {
    obj->unk31C->unk36 += D_800BE9A0;
    if (obj->unk31C->unk36 >= 141) {
        D_800D18A0 |= 1 << (obj - D_800CC2D0);
        if (D_800D18A0 != 0) {
            obj->immune = 0xFF;
            obj->stunned = 0xFF;
            obj->disable_run = 0xFF;
            obj->unkF8 |= 0x200;
            obj->bodyRadius = 0;
            obj->bodyHeight = 0;
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
