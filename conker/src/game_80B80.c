#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Defined in game_803A0.c. Not in actor.h: game_1048D0.c was compiled with an s32 return. */
void func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9);

void func_150536D0(Actor *obj) {
    obj->type = 13;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
}

void func_150536E8(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
}

void func_1505371C(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
}

// Moves the object's scale towards (unk154, unk158) by at most unk15C per call.
void func_15053750(Actor *obj) {
    f32 diff;
    f32 step;

    diff = obj->unk154 - obj->scale_xz;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            func_15062BDC(obj, obj->unk154, obj->scale_y);
        } else {
            func_15062BDC(obj, (diff < 0.0f ? -1 : 1) * step + obj->scale_xz, obj->scale_y);
        }
    }

    diff = obj->unk158 - obj->scale_y;
    if (diff != 0.0f) {
        step = obj->unk15C;
        if (fabsf(diff) < step) {
            func_15062BDC(obj, obj->scale_xz, obj->unk158);
        } else {
            func_15062BDC(obj, obj->scale_xz, (diff < 0.0f ? -1 : 1) * step + obj->scale_y);
        }
    }
}

void func_15053894(Actor *obj) {
    obj->count1++;
    if (obj->count1 >= 31) {
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
    kind = (u16)obj->headEffectState;
    sweep = (s8)obj->unk284;
    hitFlags = 0;
    if (((kind == 2) || (kind == 18)) && (u16)obj->headEffectTimer) {
        if (D_800BE9E4 < (u16)obj->headEffectTimer) {
            obj->headEffectTimer = (u16)obj->headEffectTimer - D_800BE9E4;
        } else {
            // matching: dead leftover; keeps the target's dead `move a0,v0` (as1 drops the subu, not the copy)
            left = (u16)obj->headEffectTimer - D_800BE9E4;
            if (left + left) {
            }
            obj->headEffectTimer = 0;
            obj->ygrav = 1.0f;
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
                if ((obj->owner != 0) && (target + 1 == &movement_obj[obj->owner])) {
                    target = NULL;
                }
            } else {
                target = &movement_obj[obj->unk221];
            }
            if ((target != NULL) && ((target->mv->death_State != 0) || (target->imune > 0))) {
                target = NULL;
                obj->unk221 = -1;
            }
            if (target != NULL) {
                dx = target->xpos - obj->xpos;
                dy = target->ypos - obj->ypos;
                dz = target->zpos - obj->zpos;
                dist = (dx * dx) + (dz * dz);
            } else {
                slotFlag = (D_8008FDBC & 0x40) != 0;
                slotValue = 0.5f;
            }
            if (obj->count3 < 10) {
                target = NULL;
            }
            if ((target == NULL) || (dist > 1000000.0f) || (kind == 14)) {
                dx = D_800D239C - obj->xpos;
                dy = (D_800D23A0 + 100.0f) - obj->ypos;
                dz = D_800D23A4 - obj->zpos;
                D_800872A0[slot].lateral += (slotValue - D_800872A0[slot].lateral) * 0.125f;
                D_800872A0[slot].route = slotFlag;
            } else {
                obj->unk221 = target - movement_obj;
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
                obj->angle = func_1505A630(-dx, dz, 0);
            }
            obj->xpos += dx;
            obj->ypos += dy;
            obj->zpos += dz;
            {
                f32 groundY;

                if (func_15104634(obj->xpos, obj->zpos, obj->ypos + 50.0f, &groundY) > 0) {
                    if (obj->ypos < groundY + 50.0f) {
                        obj->ypos = groundY + 50.0f;
                    }
                }
            }
            obj->norun = 1;
            obj->unk103 = 1;
            obj->speed = 0.0f;
        }
        obj->unkB2++;
        obj->unkB2 &= 1;
        if (obj->unkB2 != 0) {
            obj->count5++;
        }
        if ((D_800E0C20 != 0) || (D_800E0B94 != 0)) {
            obj->count5 = 1;
            done = 1;
        }
        if (obj->count3 < 10) {
            obj->count3++;
        }
    }
    if (obj->count2 < 100) {
        s32 pad[2]; // matching: unused; shifts the block's stack slots to the target's offsets
        f32 ownerDist;
        f32 climb;

        if (obj->unk102 != 0) {
            obj->count4 = 40;
        }
        obj->fangle = func_1505A630(movement_obj[obj->unk126].xpos - obj->xpos, obj->zpos - movement_obj[obj->unk126].zpos, 0);
        if ((u8)(((s32)(obj->fangle - obj->angle) >> 8) + obj->count4) < (obj->count4 * 2)) {
            climb = ((movement_obj[obj->unk126].ypos + (s16)obj->unk224) - obj->ypos) * 0.1800000072f;
            ownerDist = func_1505A72C(&movement_obj[obj->unk126], obj);
            func_15059B54(obj, 0x300);
            if ((obj->unk102 != 0) && (ownerDist < (obj->speed * 16.0f))) {
                if (climb > 90.0f) {
                    climb = 90.0f;
                } else if (climb < -90.0f) {
                    climb = -90.0f;
                }
                obj->yspd += (climb - obj->yspd) * 0.1800000072f;
            }
        }
    }
    if (obj->count4 >= 9) {
        obj->count4--;
    }
    if (obj->unk103 != 0) {
        func_15059C84(obj);
    }
    switch (kind) {
    case 2:
    case 12:
    case 18:
        if (obj->speed != 0.0f) {
            obj->vtilt += 15.0f * D_800BE9E4;
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
        obj->vtilt = -func_150484A0(obj->yspd, obj->speed) * 57.29577637f;
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
            if ((func_1505DDA8(obj, obj->owner, 0x200, 15.0f) != -1) && (obj->unkA9 == 0)) {
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
    D_800CC254 = obj->xpos;
    D_800CC258 = obj->ypos;
    D_800CC25C = obj->zpos;
    prevY = obj->ypos;
    func_15058898(obj, obj->old_ypos);
    func_1505B5F8(obj, obj->floorY);
    if (sweep != 0) {
        f32 dir[3];
        f32 len;
        f32 inv;
        s32 hit;
        Actor *owner;

        dir[0] = obj->xpos - obj->old_xpos;
        dir[1] = obj->ypos - obj->old_ypos;
        dir[2] = obj->zpos - obj->old_zpos;
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
        func_15081690(obj, obj->old_xpos, obj->old_ypos, obj->old_zpos, dir[0], dir[1], dir[2], &obj->mv->rayHit, len, 0, 0, 0, -1, 0, 0);
        hit = func_15081E78(obj, &obj->mv->rayHit, kind);
        if (hit != 0) {
            done = 1;
            if ((hit == 1) && (obj->owner > 0)) {
                owner = &movement_obj[obj->owner - 1];
                if ((owner->mv != NULL) && (owner->mv->unkAF != 0)) {
                    if (func_10010894(owner) == 0) {
                        func_15060A30((func_150ADA20() % 3U) + 0x53B, owner);
                    }
                }
            }
        }
    } else {
        if ((obj->unk103 != 0) && ((obj->mvmt_control & 0x18000) || (prevY != obj->ypos))) {
            if (obj->speed > 0.0f) {
                hitFlags = 1;
                obj->speed *= 0.5f;
                if ((kind == 0) && (obj->speed > 20.0f) && (obj->mvmt_control & 0x10000)) {
                    s32 sfx;

                    sfx = func_150ADA20() % 9U;
                    func_15060778((sfx < 2) ? sfx + 0x18 : sfx + 0x29, obj, 30000, 0, 500, 1500, 5);
                    func_15174690(dc_glbl_CharNum, 0, 0x14, 0, 0x199, 4, 0xAA, 0xFF, 0);
                }
            }
        }
        if (obj->doing_hit != 0) {
            s32 hit;

            hit = func_1505C7D8(obj, dc_glbl_CharNum);
            if (hit != 0) {
                D_800CC260 = hit;
                obj->norun = 1;
                hitFlags |= 8;
                obj->speed *= 0.3000000119f;
                if (kind == 15) {
                    Actor *other;

                    other = func_15083E90(6);
                    if (other != NULL) {
                        if (other->stunned != 0) {
                            obj->noshove |= 0x01000000;
                            obj->count5 = 6;
                        }
                        obj->angle = func_1505A630(other->xpos - obj->xpos, obj->zpos - other->zpos, 0);
                        obj->yspd *= 0.400000006f;
                    }
                } else {
                    s32 attacker;

                    attacker = obj->unk108 & 0x7F;
                    if (attacker < 25) {
                        obj->angle = func_1505A630(obj->old_xpos - movement_obj[attacker].xpos, movement_obj[attacker].zpos - obj->old_zpos, 0);
                    }
                    obj->yspd = 35.0f;
                    obj->ygrav = 6.0f;
                }
            }
        }
        if (kind == 15) {
            func_1505A3A8(0.0f, obj, 1.0f, 1.0f, 0);
            func_1505D6F0(obj, dc_glbl_CharNum);
            if ((D_800CC268 >> 1) != 0) {
                Actor *other;

                other = func_15083E90(6);
                if (D_800CC268 & (1U << (other - movement_obj))) {
                    D_800D1580 = 0xFF0100A7;
                    func_1506E6FC();
                    if (other != NULL) {
                        obj->angle = func_1505A630(obj->xpos - other->xpos, other->zpos - obj->zpos, 0);
                    }
                    obj->yspd = 38.0f;
                    obj->ygrav = 5.0f;
                    obj->speed = 40.0f;
                }
            }
        }
        if ((obj->unk103 != 0) && (obj->shad == 0.0f)) {
            hitFlags |= 2;
        }
        if (hitFlags != 0) {
            if (kind == 17) {
                obj->energy = 0;
            }
            if (D_800BE616 != 0) {
                obj->energy = 0;
            }
        }
        obj->theta = (obj->angle + 0x4000) * 0.005493164f;
        if (obj->shad == 0.0f) {
            obj->count1++;
            if (obj->intelwalk != 0) {
                obj->ypos = obj->old_ypos;
                done = 1;
            }
        }
    }
    if (obj->count1 >= 31) {
        done = 1;
    }
    if (--obj->count5 == 0) {
        done = 1;
    }
    if (obj->energy == 0) {
        done = 1;
    }
    if (done != 0) {
        switch (kind) {
        case 13:
            if (hitFlags & 8) {
                func_150D7120((Vec3f *)&obj->xpos, 0xFF, 1); // xpos, ypos, zpos as one Vec3f
            } else {
                Vec3f vec;

                vec.x = D_800CC254 - obj->old_xpos;
                vec.y = D_800CC258 - obj->old_ypos;
                vec.z = D_800CC25C - obj->old_zpos;
                if (func_15145128(&vec, &vec, NULL, NULL) != 0) {
                    func_150D728C(NULL, (Vec3f *)&obj->xpos, &vec, 0xFF, 1); // xpos, ypos, zpos as one Vec3f
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

    owner = &movement_obj[obj->attach_char - 1];

    for (i = 0; i < D_8008FD8C; i++) {
        if ((movement_obj[i].stand_on_char == (obj - movement_obj) + 1) && (owner->intelwalk == 1)) {
            owner->scriptCmd = NULL;
            if (func_150ADA20() & 1) {
                owner->intelwalk = 2;
                sfx = 0x1DB;
            } else {
                owner->intelwalk = 3;
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
    other->mv->weapon_status = 0;
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
    if (anim == NULL || obj->attach_char == 0) {
        return;
    }
    other = &movement_obj[obj->attach_char - 1];
    otherAnim = other->unk2D0;
    if (otherAnim == NULL) {
        return;
    }
    if (obj->character_type == 0x41) {
        if (other->animseq.uh != 0x48 && other->animseq.uh != 0x4B) {
            func_15060F28(obj, 0);
            return;
        }
        if (obj->animseq.uh == 0 && anim->frame == 0.0f) {
            func_15188810(obj, 100, 0);
        }
        if (obj->animseq.uh == 0 && other->animseq.uh == 0x4B) {
            func_1505E650(obj, 1, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        }
        if (obj->animseq.uh == 1 && anim->numFrames - 1.0f <= anim->frame) {
            func_15060F28(obj, 0);
        }
        return;
    }
    if (obj->character_type != 0x1D && obj->character_type != 0x6E && obj->character_type != 0x2F) {
        return;
    }
    if (obj->attach_char == 0 || other->attach_char != 0 || other->energy == 0) {
        func_15054A5C(obj, other);
        return;
    }
    state = other->mv;
    if (state->rt_context_b != 0x12 && state->rt_context_b != 0x18 && state->rt_context_b != 0x41 && state->weapon_status != 3) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->character_type == 0x1D) {
        enter = 0xC4;
        hold = 0xC7;
        action = 0x39;
    } else if (obj->character_type == 0x6E) {
        enter = 0xFB;
        hold = 0xFC;
        action = 0x51;
    } else {
        enter = 0x2C8;
        hold = 0x2C9;
        action = 0x51;
    }
    if (other->lifting != 0) {
        func_15054A5C(obj, other);
        return;
    }
    if (obj->character_type == 0x1D && other->stunned != 0) {
        func_15054A5C(obj, other);
        return;
    }
    attach = 1;
    if (hold == other->animseq.uh) {
        motion = 2;
        if (obj->taskint[0] == 0) {
            if (otherAnim->frame >= 6.0f && otherAnim->frame < 11.0f) {
                obj->taskint[0] = 1;
                func_15083568(obj, action, 1.0f, 0);
            }
        } else if (otherAnim->frame >= 11.0f) {
            obj->taskint[0] = 0;
            func_150836CC(obj, action);
        }
    } else {
        if (obj->taskint[0] == 1) {
            obj->taskint[0] = 0;
            func_150836CC(obj, action);
        }
        if (enter == other->animseq.uh) {
            motion = 0;
        } else if (other->animseq.uh == 0xC6) {
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
    state = other->mv;
    if (state->rt_context_b == 0x12) {
        ready = 1;
    } else {
        ready = state->SniperState;
        if (state->bot == 0) {
            index = other - movement_obj;
            // D_800BE728[0] points to the per-player controller pads
            if (index < 4 && !(D_800BE728[index]->button & R_TRIG)) {
                ready = 0;
            }
        }
    }
    if (other->animseq.uh == 0x10A && otherAnim->unk3C <= 0 && ready &&
        (state->buttons & 0x2000)) {
        if (obj->taskint[1] == 0) {
            func_151026BC(other, -1, 3, 1, 0xFF, 1);
            obj->taskint[1] = 1;
        }
    } else if (obj->taskint[1] == 1) {
        obj->taskint[1] = 0;
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

    if (obj->character_type == 12) {
        if ((dc_glbl_CharNum + 1) != movement_obj[obj->owner].attach_char) {
            if (obj->intelwalk == 18) {
                func_15052F9C(obj, 50.0f, 0, 4, 0, 12, 0, 8, 0, 0);
            } else if (D_800CC268 & 1) {
                if (obj->task_attrib & 0x400) {
                    obj->owner = 0;
                    func_1505327C(obj, 39.0f, 3.0f, 18, 0);
                }
            }
        }
    } else if (obj->character_type == 0x4B) {
        func_1505A184(obj->langle ^ 0x8000, 500.0f, 0.0f, &offsetX, &offsetZ, &offsetY);
        x = (s16)obj->xpos + offsetX;
        z = (s16)obj->zpos + offsetZ;
        obj->taskint[2] = (x << 16) | (z & 0xFFFF);
        if (obj->animseq.uh != 15) {
            angle = func_1505A630(movement_obj[0].xpos - obj->xpos, obj->zpos - movement_obj[0].zpos, 0);
        } else {
            angle = obj->langle - 0x800;
        }
        angle -= obj->langle;
        angle /= 3;
        obj->langle += angle;
        obj->scale_xz = 2.0f;
        obj->scale_y = 2.0f;
        obj->theta = 90.0f;
    // matching: an empty labelled statement ends the block so as1 keeps the last store out of the branch delay slot
    label:
        ;
    } else if (((obj->character_type == 0x1B) || (obj->character_type == 0x7E)) && (obj->taskint[0] != 0)) {
        if (D_800BE616 != 0) {
            func_1508B20C(obj->xpos, obj->ypos, obj->zpos, 900.0f);
        }
        if (D_800BE9E4 < obj->taskint[0]) {
            if ((obj->taskint[0] >= 120) && (obj->taskint[0] < (D_800BE9E4 + 120))) {
                func_10010154(0x3A1, obj, 0x7D00, 500, 1000);
            }
            obj->taskint[0] -= D_800BE9E4;
        } else {
            obj->intelwalk = 6;
            obj->scriptCmd = NULL;
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

    kind = (u16)obj->headEffectState;
    hitFlags = 0;
    hoverHeight = 0.0f;

    switch (kind) {
    case 0x1F:
        obj->vtilt += obj->speed * D_800BE9E4;
        if (obj->taskint[0] == 1) {
            func_10010344(0x230, obj, 0x7D00, 500, 2500);
            obj->taskint[0] = 0;
        }
        break;
    case 0x42: {
        s32 hit;

        hoverHeight = 25.0f;
        if ((obj->ygrav != 0.0f) && ((obj->yspd > 0.0f) || (obj->shad > hoverHeight))) {
            obj->vtilt = -func_150484A0(obj->yspd, obj->speed) * 57.29577637f;
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
        if (obj->shad > hoverHeight) {
            obj->vtilt += D_800BE9E4 * 3;
        }
        break;
    case 0x35:
        obj->vtilt += D_800BE9E4 * 16;
        break;
    case 0x88:
        obj->vtilt = -func_150484A0(obj->yspd, obj->speed) * 57.29577637f;
        obj->doing_hit = 16;
        if (func_1505C7D8(obj, dc_glbl_CharNum) != 0) {
            func_15060F28(obj, 1);
            return;
        }
        break;
    }
    func_15059C84(obj);
    obj->stunned = 1;
    func_1505A770(obj);
    prevX = obj->xpos;
    prevY = obj->ypos;
    prevZ = obj->zpos;
    func_15058898(obj, obj->old_ypos);
    if ((obj->unk118 != -10000.0f) && (obj->ypos <= obj->unk118) && (obj->unk118 <= obj->old_ypos)) {
        obj->speed *= 0.1000000015f;
        obj->taskint[0] = 2;
        obj->ygrav = 0.0f;
        obj->yspd = -3.0f;
    } else if ((prevX != obj->xpos) || (prevY != obj->ypos) || (prevZ != obj->zpos)) {
        hitFlags = 1;
        if ((kind == 0x1F) && (obj->taskint[0] != 2)) {
            func_10010344((func_150ADA20() % 5U) + 0x3A4, obj, 0x7D00, 500, 2500);
        }
    }
    func_1505B5F8(obj, obj->floorY);
    if (obj->shad <= hoverHeight) {
        obj->count1++;
        hitFlags |= 2;
        if ((obj->floorMaterial & 0x1F) == 0xE) {
            func_1514AD9C((Vec3f *)&obj->xpos, 0xFF, 0); // xpos, ypos, zpos as one Vec3f
            func_15060F28(obj, 1);
            return;
        }
        if (kind == 0x42) {
            f32 t;
            f32 y;

            D_800D1580 = 0xFF0100B0;
            if (obj->ygrav != 0.0f) {
                func_1506E5FC();
            }
            // matching: the int 0 stores below (not 0.0f) steer where IDO materializes its shared FP zero
            y = obj->ypos;
            obj->ygrav = 0;
            t = y - obj->old_ypos;
            obj->yspd = 0;
            obj->speed = 0;
            obj->ypos = obj->floorY + hoverHeight;
            if (t != 0.0f) {
                t = (obj->ypos - obj->old_ypos) / t;
            } else {
                t = 0;
            }
            obj->xpos = obj->old_xpos + ((obj->xpos - obj->old_xpos) * t);
            obj->zpos = obj->old_zpos + ((obj->zpos - obj->old_zpos) * t);
        } else if (obj->ypos < obj->unk118) {
            obj->ygrav = 0.0f;
            obj->yspd = 0.0f;
            obj->speed = 0.0f;
        } else if ((obj->yspd > 5.0f) && (kind == 0x1F) && (obj->taskint[0] != 2)) {
            func_10010344(0x39E, obj, 0x7D00, 500, 2500);
            obj->taskint[0] = 2;
        }
    }
    obj->theta = (obj->angle + 0x4000) * 0.005493164f;
    if ((hitFlags != 0) && (kind == 0x35)) {
        func_10010630(0x360, obj, 0x4E20, 500, 2500);
        if (obj->unk118 < obj->ypos) {
            Vec3f pos;

            pos.x = obj->xpos;
            pos.y = obj->ypos + 20.0f;
            pos.z = obj->zpos;
            func_151DCDE0(&pos, 0xFF, 1);
        }
        func_15060F28(obj, 1);
        return;
    }
    timer = --obj->count5;
    if ((timer == 0) || (obj->count1 >= 31)) {
        func_15060F28(obj, 1);
        return;
    }
    if (timer < 10) {
        alpha = (timer * 255) / 10;
    } else if (obj->count1 >= 21) {
        alpha = ((30 - obj->count1) * 255) / 10;
    } else {
        alpha = 255;
    }
    func_1503192C(obj, kind, alpha, 0);
    if (kind == 0x1F) {
        Camera *player;
        f32 distSq;

        player = camera_global_Cameras;
        distSq = ((player->eye.x - obj->xpos) * (player->eye.x - obj->xpos)) +
                 ((player->eye.y - obj->ypos) * (player->eye.y - obj->ypos)) +
                 ((player->eye.z - obj->zpos) * (player->eye.z - obj->zpos));
        if (distSq < 900.0f) {
            func_1512D748(player, 8, 0);
            func_10010F88(0x2E1, 0x6D60, 0, 0, 0, obj->xpos, obj->ypos, obj->zpos, 500, 1000);
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
    obj->type = 39;
    func_1505E650(obj, obj->animseq.uh, 0, 0.0f, 0.0f, 0.0f, 0);
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->unk21C = duration;
}

void func_15055B64(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    obj->noshove |= 0x1000000;

    if (D_800BE9A0 < obj->unk21C) {
        obj->unk21C -= D_800BE9E4;
    } else {
        obj->unk21C = 0;
        if (obj->ob_matrix_ptr == NULL) {
            func_15060F28(obj, 1);
        }
    }
}

void func_15055BF8(Actor *obj) {
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    obj->noshove |= 0x1000000;
    obj->ypos -= D_800BE9A4;

    if (obj->ypos < (obj->floorY - 100.0f)) {
        func_15060F28(obj, 0);
    }
}

void func_15055C88(Actor *obj) {
    s32 sfx = 0x2007E4;

    if (!(obj->unk9C & 0x1000) && func_1506E46C(obj, &sfx, 1)) {
        func_15060A30(sfx, obj);
    }
    obj->mv->over_edge = 100;
    func_1506160C(obj, 2, 0, 6, 0);
    obj->norun = 200;
    obj->count5 = 200;
    obj->unk1CC = 70000.0f;
    obj->type = 41;
    obj->ygrav = 0.0f;
    obj->yspd = 0.0f;
    obj->speed = 0.0f;
    func_1507CD64(obj, 9);
}

void func_15055D48(Actor *obj) {
    obj->mv->over_edge += D_800BE9A0;
    if (obj->mv->over_edge >= 141) {
        D_800D18A0 |= 1 << (obj - movement_obj);
        if (D_800D18A0 != 0) {
            obj->imune = 0xFF;
            obj->stunned = 0xFF;
            obj->norun = 0xFF;
            obj->noshove |= 0x200;
            obj->bodyRadius = 0;
            obj->bodyHeight = 0;
            obj->unk7 = 0;
            obj->energy = 0;
            obj->type = 5;
            obj->speed = 0.0f;
            obj->yspd = 0.0f;
            obj->ygrav = 0.0f;
            return;
        }
        func_1506D538();
    }
    obj->energy = 0;
    obj->scale_xz *= 0.9499999881f;
    obj->scale_y *= 0.9499999881f;
    obj->speed *= 0.9499999881f;
}
