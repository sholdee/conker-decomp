#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Per-mode tables indexed by D_800CC27C (D_800990C0 and D_800990E0 are read by game_79F10.c). */
const f32 D_800990C0[8] = { 0.0f, 0.6000000238f, 0.1800000072f, 0.3000000119f, 0.0f, 2.0f, 2.0f, 4.0f };
const f32 D_800990E0[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
const f32 D_80099100[8] = { 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f };
const f32 D_80099120[8] = { 0.0f, 0.75f, 0.75f, 1.0f, 1.25f, 0.5f, 0.5f, 0.5f };

/* Indexed by floor material (func_1504BA38). */
const Struct80099140 D_80099140[20] = {
    { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },     { 0, 0, 0 },
    { 0, 0, 0 },     { 0, 0, 0 },     { 45, -5, 50 },  { 0, 0, 0 },     { 0, 0, 0 },
    { -6, 0, 90 },   { -6, 0, 90 },   { 44, -6, 40 },  { 0, 0, 0 },     { -100, 0, 0 },
    { 44, -4, 80 },  { 0, 0, 0 },     { 0, 0, 0 },     { 44, -4, 80 },  { -100, 0, 30 },
};

/* Angle for each 4-bit direction mask (read by game_79F10.c). */
const u16 D_8009917C[16] = {
    0x0000, 0xC000, 0x4000, 0xFFFF, 0x8000, 0xA000, 0x6000, 0x8000,
    0x0000, 0xE000, 0x2000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* Sound ids (read by game_80B80.c). */
const u16 D_8009919C[6] = { 0x2B1, 0x2B5, 0x2B6, 0x2B7, 0x2BD, 0x2BE };


void func_1504A730(void) {
    Actor *obj;
    s32 i;
    s32 tickResult;
    OSContPad noPad;
    f32 maxStep;
    f32 step;
    s32 j;

    D_800BE9EC = 0;
    D_800D1548 += 1;
    D_800CC250 += D_800BE9A0;
    dc_glbl_CharNum = 0;
    D_800CC2BC = 0;
    func_1508295C(level, 0, 0);
    D_800CC2B8 ^= 1;

    for (i = 0; i != 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0) {
            continue;
        }
        obj->stand_on_char = 0;
        obj->unk300 = 0;
        obj->mvmt_control &= 0xFFEE5FFF;
        dc_glbl_CharNum = i;
        if (obj->ypos <= (f32)obj->unk38) {
            if (obj->playernumber == 0xFF) {
                func_15060F28(obj, 0);
                continue;
            }
            if (D_800BE616 == 0) {
                func_1507D4F8(i);
                continue;
            }
        }
        if (obj->unk2FA == 0) {
            continue;
        }
        D_800D154C = obj;
        obj->unk12++;
        D_800D35DC = i + 1;
        D_800CC2BA = 0;
        tickResult = 0;
        if (D_800BE616 == 0 || (tickResult = func_1504C8BC(obj)) != 0) {
            maxStep = 2.0f;
            if (i < D_8008FD8C) {
                obj->mv->being_sniped -= D_800BE9E4;
                if (obj->mv->being_sniped < 0) {
                    obj->mv->being_sniped = 0;
                }
                obj->mv->unk57 = 0;
                if (i < 4) {
                    D_800CC284 = D_800BE728[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    D_800CC284 = &noPad;
                }
            }
            if (obj->CameraPtr != NULL) {
                if (obj->CameraPtr->LockedCameraAngle == 999.0f) {
                    D_800CC280 = obj->CameraPtr->CameraAngle * (65536.0f / 360.0f);
                } else {
                    D_800CC280 = obj->CameraPtr->LockedCameraAngle * (65536.0f / 360.0f);
                }
            } else {
                D_800CC280 = 0;
            }
            if (obj->playernumber != 0xFF) {
                func_150A11C4(obj);
            } else if (obj->noshove & 0x400000) {
                func_150A278C(obj);
            }
            obj->old_xpos = obj->xpos;
            obj->old_ypos = obj->ypos;
            obj->old_zpos = obj->zpos;
            D_800CC27C = (obj->floorMaterial >> 28) & 7;
            step = D_800BE9A4;
            if (D_800BE616 != 0 && (D_8008FDBC & 0x100)) {
                maxStep = 5.0f;
            }
            if (maxStep < step) {
                step = maxStep;
            }
            D_800D1550 = obj->speedup * step;
            D_800CC264 = D_800D1550 * 100.0f;
            {
                // matching: a -g3 scope here keeps the lbu after the sh above and lets the lui hoist
                extern u8 D_800CC27C;
                if (D_800CC27C >= 6) {
                    D_800CC27C = 0;
                }
            }
            if (D_800B0DF0->unk49 != 0) {
                D_80086004[D_800B0DF0->unk49]((s32)obj);
            }
            if (!(obj->task_attrib & 0x200)) {
                if (D_80086014[obj->type] != NULL) {
                    D_80086014[obj->type](obj);
                }
            }
            if (obj->type != 0) {
                func_1507E73C(obj);
                func_1504AF10(obj, 0, 0);
                if (i < D_8008FD8C && D_800D18A8 == 0) {
                    func_15085710(obj->playernumber, 5, obj->energy);
                }
            }
            obj->unk2FF = 1;
            if (tickResult == 2) {
                D_800BE9E4 = D_800CC2BA;
                D_800BE9A4 = D_800BE9E4 * 0.5f;
                if (D_800BE9A4 != 0.0f) {
                    D_800BE9A8 = 1.0f / D_800BE9A4;
                } else {
                    D_800BE9A8 = 0.0f;
                }
            }
            if (obj->old_xpos != obj->xpos || obj->old_ypos != obj->ypos ||
                obj->old_zpos != obj->zpos) {
                obj->unk300 = 1;
            }
            if (D_800B0DF0->unk4A != 0) {
                D_8008600C[D_800B0DF0->unk4A]((s32)obj);
            }
        }
    }

    for (j = 0; j < D_8008FD8C; j++) {
        if ((movement_obj)[j].type != 0 && (movement_obj)[j].mv != NULL) {
            if ((movement_obj)[j].unk137 != 0) {
                if (D_800CBDA0[j] != 0) {
                    db_Object_array[D_800CBDA0[j] - 1].object_status |= 0x84;
                    gWorldObjectContacts[D_800CBDA0[j] - 1] |= 1 << j;
                }
                (movement_obj)[j].mv->unk56 = 5;
            } else if ((movement_obj)[j].mv->unk56 != 0) {
                (movement_obj)[j].mv->unk56--;
            }
        }
    }
    func_15061B4C();
    func_15080C64();
    D_800D154C = movement_obj;
    dc_glbl_CharNum = 0;
    D_800D35DC = 0;
}

void func_1504ADD0(void) {
    s32 i;
    Actor *obj;
    OSContPad noPad;

    if (D_800C35EA[0] != 1) {
        for (i = 0; i < 25; i++) {
            obj = &movement_obj[i];
            if (obj->type == 0) {
                continue;
            }
            dc_glbl_CharNum = i;
            D_800D154C = obj;
            if (i < D_8008FD8C) {
                if (i < 4) {
                    D_800CC284 = D_800BE728[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    D_800CC284 = &noPad;
                }
            }
            switch (obj->character_type) {
            case 0x28:
            case 0x77:
                func_150ED578(obj);
                break;
            }
        }
    }
}

s32 func_1504AEF4(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return 0;
    }
    return 0;
}

void func_1504AF10(Actor *obj, s32 force, s32 release) {
    s32 pad; // matching: unused slot that places index at its original stack offset
    s32 index;
    s32 type;
    s32 id;

    if (obj->unk92 && (force || !(obj->unk92 >> 15))) {
        id = obj->unk92 & 0x7FFF;
        if ((type = (obj->unk92 >> 12) & 7) == 0) {
            type = func_1504AEF4(id, (s32)&index);
            if (type != 0) {
                obj->unk92 = (type << 12) | index;
            }
        }
        index = obj->unk92 & 0xFFF;
        if (type == 3) {
            if (release) {
                db_Object_array[index].attach = 0;
                obj->unk92 = 0;
                return;
            }
            db_Object_array[index].attach = (obj - movement_obj) + 100;
            if (force) {
                db_Object_array[index].attach = 0;
                return;
            }
            db_Object_array[index].object_status &= ~0x60;
            db_Object_array[index].object_status |= 0x20;
            db_Object_array[index].x = obj->xpos;
            db_Object_array[index].y = obj->ypos + obj->unk90;
            db_Object_array[index].z = obj->zpos;
        }
    }
}


f32 func_1504B0FC(Actor *obj, f32 arg1) {
    s32 belowLevel;
    f32 targetPitch;
    f32 delta;
    f32 result;
    s8 stickX;
    s32 stickY;
    f32 step;
    f32 out0;
    f32 out1;
    f32 out2;
    f32 out3;
    f32 out4;
    f32 excess;

    result = 0.0f;
    obj->mv->unk68 = 0;
    obj->unk110 = 0;
    if (obj->stunned == 0 && obj->energy != 0 && obj->mv->AimState == 0) {
        stickX = D_800CC284->stick_x;
        stickY = D_800CC284->stick_y;
    } else {
        stickX = 0;
        stickY = 0;
    }
    if (obj->mv->unk22 > 100) {
        stickY = 80;
    }
    delta = (obj->ypos - ((obj->unk118 - 60.0f) - 170.0f)) * 2.0f;
    if ((obj->mv->water_attribs & 1) || obj->mv->unk21 != 0) {
        obj->unkB2 = 0;
        if (delta < 400.0f) {
            // matching: loading 8.0f into step first gives the target's mul.s operand order
            step = 8.0f;
            step *= D_800D1550;
            if (stickY > 10) {
                stickY = 10;
            }
            if (delta + step > 400.0f) {
                step = 400.0f - delta;
            }
            obj->ypos += step;
        }
    }
    if (obj->inwater == 1) {
        obj->unk1CC = obj->ypos;
        if (obj->unk118 == -10000.0f) {
            func_15052408(obj);
            return 0.0f;
        }
        if (stickY != 0 || ((obj->unk118 - 60.0f) - 175.0f) < obj->ypos) {
            targetPitch = stickY * 2.25f;
        } else {
            targetPitch = obj->vtilt;
        }
        if (obj->mv->unk21 != 0) {
            targetPitch = 0.0f;
        }
        func_15059444(obj);
        if ((obj->stunned != 0 && (obj->energy != 0 || obj->yspd >= 60.0f)) || obj->animseq.uh == 0x262) {
            return 0.0f;
        }
        if (delta > 0.0f && obj->animseq.uh != 0x22) {
            if (delta > 300.0f) {
                delta = 300.0f;
            }
            obj->ypos += delta * arg1 * 0.002997f;
        }
    } else if ((obj->ygrav < 0.0f && obj->yspd > 0.0f) || (obj->ygrav > 0.0f && obj->yspd < 0.0f)) {
        obj->ygrav = 0.0f;
        obj->yspd = 0.0f;
    }
    if (obj->inwater != 1 || obj->shad >= 10.0f || obj->ypos < (obj->unk118 - 60.0f) + 20.0f) {
        if (obj->inwater == 1) {
            if ((obj->unk118 - 50.0f < obj->aim.y || (obj->mv->water_attribs & 1)) && obj->energy != 0) {
                if (obj->unkB2 > 30) {
                    if (obj->unkB2 > 1500) {
                        func_15060A30(0x1A7, obj);
                    } else if (obj->unkB2 > 720) {
                        func_15060A30(0x168, obj);
                    }
                }
                obj->unkB2 = 0;
            } else {
                obj->unkB2 += D_800BE9E4;
                if (obj->unkB2 > 1560 || (obj->energy == 0 && obj->mv->death_State == 0)) {
                    obj->unkB2 -= 60;
                    if (obj->energy >= 2) {
                        if (obj->energy == 4) {
                            func_15060A30(0x499, obj);
                        }
                        obj->energy--;
                    } else {
                        obj->energy = 0;
                        if (obj->stunned != 0) {
                            obj->ygrav = 0.0f;
                            func_1507CD64(obj, 4);
                        } else {
                            func_1507CD64(obj, 3);
                        }
                    }
                } else if (obj->unkB2 > 1440 && obj->unkB2 < 1500) {
                    obj->unkB2 += 120;
                    if (obj->energy < 4) {
                        func_15060A30(0x499, obj);
                    }
                }
            }
            if ((obj->yspd != 0.0f || obj->ygrav != 0.0f) && obj->ygrav < 0.0f && obj->yspd > 0.0f) {
                obj->ygrav = 0.0f;
                obj->yspd = 0.0f;
            }
            if (((obj->unk118 - 60.0f) - 175.0f) < obj->ypos && obj->animseq.uh != 0x28) {
                if (obj->mv->water_up < 2.5f) {
                    obj->mv->water_up += 0.05f;
                }
            } else {
                obj->mv->water_up = 0.0f;
            }
            obj->ypos += obj->mv->water_up * D_800D1550;
            excess = obj->ypos - ((obj->unk118 - 60.0f) + 14.0f);
            if (excess > 0.0f) {
                excess *= 0.65f;
                obj->ypos -= excess;
                obj->mv->water_up = 0.0f;
            }
            if (((obj->unk118 - 60.0f) - 75.0f) < obj->ypos) {
                targetPitch -= 30.0f;
            }
        }
        if (obj->animseq.uh == 0x22 && targetPitch < -20.0f) {
            targetPitch = -20.0f;
        }
        if (obj->shad < 40.0f) {
            func_1510F820(obj->floorTriVerts, obj->floorTri.v, &out0, &out1, &out2, &out3, &out4);
            out3 = func_1505210C(obj, out3, out4, &out4, 180.0f);
            if (out4 < obj->vtilt) {
                obj->vtilt += (out4 - obj->vtilt) * 0.2f;
            }
        }
        obj->unkBC = targetPitch - obj->vtilt;
        if (2.7f * D_800D1550 > fabsf(obj->unkBC)) {
            obj->vtilt = targetPitch;
        } else {
            if (obj->unkBC > 2.7f) {
                obj->unkBC = 2.7f;
            }
            if (obj->unkBC < -2.7f) {
                obj->unkBC = -2.7f;
            }
            obj->vtilt += obj->unkBC * D_800D1550;
        }
        if (obj->vtilt < -90.0f) {
            obj->vtilt = -90.0f;
        }
        if (obj->vtilt > 90.0f) {
            obj->vtilt = 90.0f;
        }
        if (obj->mv->unk21 == 0) {
            if ((stickX + 8) / 18 != 0) {
                obj->angle -= ((((stickX + 8) / 18) * D_800CC264) << 8) / 100;
                obj->unk7E = 0x17;
            }
            result = obj->wantspeed;
        }
        obj->count2 = 1;
    } else {
        func_15052408(obj);
        if (obj->speed > 16.0f) {
            obj->speed = 16.0f;
        }
        result = obj->speed;
    }
    if (obj->mv->ladder != 0) {
        obj->ypos = obj->old_ypos;
        belowLevel = obj->unk118 != -10000.0f && obj->ypos < obj->unk118 - 60.0f;
        if (!belowLevel) {
            obj->inwater = 0;
        }
    }
    return result;
}

void func_1504BA38(Actor *obj) {
    Struct80099140 *entry;
    s8 unk2;
    s32 unk1;

    if (obj->shad > 10.0f) {
        obj->unkCE = 0;
        unk1 = entry->unk1; // matching: dead use of entry here sets the register allocation
        return;
    }
    entry = &D_80099140[obj->floorMaterial & 0x1F];
    unk2 = entry->unk2;
    if (unk2 != 0 && obj->unkAA == 0) {
        obj->unkAA = unk2;
    }
    unk1 = entry->unk1;
    if (unk1 == 0) {
        obj->unkCE = entry->unk0;
        return;
    }
    obj->unkCE = unk1 * 0.1000000015f * (entry->unk0 - obj->speed);
    if (unk2) { // matching: keeps unk2 live to the end (in a1 rather than v0)
    }
}

void func_1504BAF0(Actor *obj) {
    s16 index;
    Actor *other;

    index = obj->lifting - 100;
    if (index >= 0) {
        other = &movement_obj[index];
        if (other->carried >= 100 && (other->character_type == 168 || other->character_type == 169)) {
            D_800D1580 = 3;
            func_1507490C();
        }
    }
    func_1506EBC0();
}

void func_1504BB88(Actor *obj) {
    if (obj->task_attrib & 0x10) {
        obj->count5 = 0xFF;
        obj->norun = 0xFF;
        if (obj->type == 1) {
            func_1505E650(obj, 0xD6, 1.15f, 3.0f, 0.0f, 0.0f, 0);
        }
        obj->task_attrib &= ~0x10;
    }

    if (obj->task_attrib & 2) {
        if (obj->lifting != 0) {
            func_1504BAF0(obj);
        }
        obj->task_attrib &= ~2;
    }
}

void func_1504BC38(Actor *obj) {
    if (obj->norun != 0 && obj->norun < 250) {
        if (obj->norun <= D_800BE9A0) {
            obj->norun = 0;
        } else {
            obj->norun -= D_800BE9A0;
        }
    }
    if (obj->unk228) { // matching: "!= 0" here changes the register allocation (a2 -> a1)
        if (obj->unk228 <= D_800BE9A0) {
            obj->unk228 = 0;
        } else {
            obj->unk228 -= D_800BE9A0;
        }
    }
    if (obj->unkAE != 0) {
        obj->unkAE--;
    }
    if (obj->nojump != 0xFF) {
        if (obj->nojump <= D_800BE9A0) {
            obj->nojump = 0;
        } else {
            obj->nojump -= D_800BE9A0;
        }
    }
    if (obj->imune != 0xFF) {
        if (obj->imune <= D_800BE9A0) {
            obj->imune = 0;
        } else {
            obj->imune -= D_800BE9A0;
        }
    }
    if (obj->mv->chased > 0) {
        obj->mv->chased -= D_800BE9A0;
    }
    if (obj->mv->chasing > 0) {
        obj->mv->chasing -= D_800BE9A0;
    }
    if (obj->mv->camjolt > 0) {
        obj->mv->camjolt -= D_800BE9A0;
    }
    if (obj->ivehitit != 0) {
        obj->ivehitit--;
    }
    if (obj->unk1D0 > 0) {
        obj->unk1D0 -= D_800BE9A0;
    }
    if (obj->mv->side > 0) {
        obj->mv->side -= D_800BE9A0;
    }
    if (obj->mv->save_done != 0) {
        obj->mv->save_done--;
    }

    obj->mv->use_cam_butt >>= 1;

    if (obj->mv->ReloadTimer != 0) {
        if (obj->mv->ReloadTimer > D_800BE9E4) {
            obj->mv->ReloadTimer -= D_800BE9E4;
        } else {
            obj->mv->ReloadTimer = 0;
        }
    }
    if (obj->mv->unk1A0 != 0) {
        if (obj->mv->unk1A0 > D_800BE9E4) {
            obj->mv->unk1A0 -= D_800BE9E4;
        } else {
            obj->mv->unk1A0 = 0;
        }
    }
}

void func_1504BE2C(Actor *obj, u16 *arg1, f32 *arg2, s8 *arg3) {
    s32 pad[2]; // matching: the frame has two unused words
    f32 angle;

    angle = obj->mv->pissed_cycle * 0.024543693f; // pi / 128: pissed_cycle counts 256ths of a turn
    D_800CC2B4 = 16.0f;
    if (obj->mv->pissed_hangover != 0 && obj->mv->pissed_hangover == 0) { // never true, but compiled in
        func_1507EB2C(obj);
        obj->mv->pissed_up = 0;
    }
    if (obj->mv->pissed_walk == 0) {
        obj->mv->pissed_walk = 0xA6;
    }
    obj->mv->turn = 5;
    if (arg1) { // matching: this and the empty if below keep arg1 live in a1 across the calls
    }
    if (*arg2 > 4.0f && obj->animseq.uh == 0xAA) {
        obj->mv->turn = 1;
        obj->mv->pissed_angle = *arg1;
    }
    func_150AD78C(angle);
    if (arg1) {
    }
    obj->mv->pissed_cycle += 2;
    if (obj->mv->pissed_hangover != 0) {
        obj->unkAA = 25;
    } else if (obj->animseq.uh == 0xA7) {
        obj->unkAA = 50;
    } else {
        obj->unkAA = 32;
    }
    obj->unk54 *= 0.5f;
    if (obj->animseq.uh == 0x2C || obj->animseq.uh == 0xE0) {
        *arg3 = D_800CC284->stick_y >> 1;
    } else {
        if (!(func_150ADA20() & 0x3F)) {
            obj->mv->pssd_waist_wobble = (func_150ADA20() % 25000U) - 12500;
        }
        D_800CC2B2 = obj->mv->pssd_waist_wobble;
        if (obj->mv->pissed_hangover != 0) {
            D_800CC2B2 /= 3;
        }
    }
    if (D_800C35EA[0] == 0 && obj->mv->pissed_hangover == 0) {
        obj->headEffectTimer = 30;
        obj->headEffect = 0;
    }
}

s32 func_1504C078(void) {
    if (D_800D154C->mv->idle.active != 1) {
        D_800D154C->mv->idle.holdFrames = 0;
        func_1507F640();
    }
    return 999;
}

s32 func_1504C0B8(void) {
    if (level == 27 || level == 30) {
        return 395;
    }
    return 27;
}

void func_1504C0E8(Actor *obj, s32 arg1, f32 arg2) {
    f32 blend;
    f32 start;
    f32 vel;
    s32 pressed;
    f32 speed;
    s32 anim;
    ActorCtl *ext;
    f32 diff;

    if (D_800CC2B2 == 0 && (ext = obj->mv)->pssd_waist_tilt >= -29 && ext->pssd_waist_tilt < 30) {
        ext->pssd_waist_tilt = 0;
    } else {
        ext = obj->mv;
        // matching: computed in two statements to keep the target's float register order
        diff = D_800CC2B2 - ext->pssd_waist_tilt;
        diff = (D_800D1550 / D_800CC2B4) * diff;
        ext->pssd_waist_tilt += (s16)diff;
    }
    ext = obj->mv;
    if (ext->unk6B != 0) {
        return;
    }
    anim = 999;
    if (obj->attach_char != 0 && obj->stunned == 0) {
        func_15064A14(obj);
        return;
    }
    if (obj->count3 == 0 && obj->stunned == 0 && obj->inwater == 0) {
        if (obj->count5 == 0 && ext->weapon_status != 3) {
            obj->count2 = 1;
            obj->unk100 &= 0xF3;
            D_800CC2BC |= 0x80;
            speed = 1.0f;
            start = 0.0f;
            blend = 4.0f;
            vel = obj->speed * 0.4000000060f + arg2 * 0.6000000238f;
            ext = obj->mv;
            if (ext->bitten > 0 || ext->transport != 0 || obj->lifting >= 100 || arg1 != 0 ||
                (ext->walk_attrib & 3) || ext->rt_context_b != 0 || ext->injured != 0 ||
                ext->pissed_up > 0 || ext->env_suit != 0) {
                if (func_15064B94(obj, vel, vel, arg1, arg2) != 0) {
                    return;
                }
                ext = obj->mv;
            }
            pressed = D_800CC284->button & Z_TRIG;
            if (pressed == 0 || D_800CC2B1 != 0 || ext->rt_context_b != 0 || (ext->transport & 0xF) == 1) {
                if (obj->speed > 4.0f) {
                    ext->dwalk = 0;
                    ext = obj->mv;
                }
                pressed = ext->transport; // matching: reusing pressed for this value sets the register allocation
                if ((ext->transport == 1 || pressed == 0x81) && vel > 1.0f) {
                    arg2 = 10.0f;
                    anim = 0x302;
                    speed = obj->speed / 7.0f + 0.2000000030f;
                    if (obj->speed > 23.0f) {
                        anim = 0x303;
                        speed = obj->speed / 23.0f + 0.3000000119f;
                    }
                } else if (ext->chased > 0 && vel >= 5.0f) {
                    anim = 0x2B;
                    speed = vel / 12.0f;
                } else if (ext->chasing > 0 && vel >= 5.0f) {
                    anim = 0xDF;
                    speed = vel / 12.0f;
                } else if (obj->unk118 != -10000.0f && obj->ypos < obj->unk118 &&
                           obj->speed >= 1.0f && ext->transport == 0) {
                    speed = vel / 18.0f + 0.2000000030f;
                    anim = 0x8B;
                    obj->unkAA = 0x4B;
                } else if (vel > 14.0f) {
                    anim = (level != 4) ? 2 : 0x1B2;
                    speed = vel / 23.0f + 0.8000000119f;
                } else if (vel > 6.0f || (arg2 < 1.0f && obj->speed > 1.0f)) {
                    anim = 1;
                    speed = vel * 0.125f + 0.3000000119f;
                } else if (vel > 1.0f) {
                    blend = 15.0f;
                    anim = (obj->character_type == 0x80) ? 1 : 0;
                    speed = vel * 0.125f + 0.1000000015f;
                } else {
                    anim = func_1504C078();
                }
                if (obj->shad > 60.0f && obj->mv->transport == 0) {
                    speed = 0.5f;
                    anim = 0x38;
                    obj->unk100 |= 0x10;
                }
                if (obj->speed > 18.0f && obj->mv->weapon_status != 1 && obj->mv->weapon_status != 3 && arg2 == 0.0f &&
                    obj->shad == 0.0f && D_800CC27C == 0) {
                    speed = 0.5f;
                    anim = 0x21;
                    blend = 3.0f;
                    obj->norun = 8;
                    obj->count5 = 0x11;
                }
            } else if (pressed != 0) {
                if (obj->speed < 3.0f) {
                    ext->dwalk = 1;
                }
                if (obj->stickspeed > 2.599999905f && (ext = obj->mv)->dwalk != 0) {
                    ext->duck_tilt = 1;
                    obj->unkAA = 0x28;
                    blend = 5.0f;
                    speed = obj->stickspeed / 15.0f + 0.5f;
                    anim = func_1504C0B8();
                } else {
                    blend = 6.0f;
                    speed = 1.5f;
                    anim = 0x16;
                }
            }
        }
        if (anim != 999) {
            func_1505E650(obj, anim, speed, blend, start, 0.0f, 0);
        }
    } else if (obj->animseq.uh == 0x21 && D_800CC2B1 == 0 && obj->stickspeed == 0.0f &&
               (D_800CC284->button & Z_TRIG)) {
        func_1505E650(obj, 0x16, 1.5f, 6.0f, 0.0f, 0.0f, 0);
    }
}

void func_1504C854(Actor *obj) {
    D_800CC288 = obj->mv->pressedButtons;
    if (obj->mv->buttonsTimer > 0) {
        obj->mv->buttonsTimer -= D_800BE9A0;
    } else {
        obj->mv->buttons = 0;
    }
    if (obj->mv->pressedButtonsTimer > 0) {
        obj->mv->pressedButtonsTimer -= D_800BE9A0;
    } else {
        obj->mv->pressedButtons = 0;
    }
}

s32 func_1504C8BC(Actor *obj) {
    ActorCtl *ext;

    ext = obj->mv;
    if (ext == NULL) {
        return 1;
    }
    if (obj->ob_matrix_ptr != NULL || level == 0x33 || obj->type == 0x25) {
        ext->unk1A8 = 0;
        return 1;
    }
    if ((obj->playernumber & 1) == D_800CC2B8) {
        if (ext->unk1A8 != 0) {
            D_800CC2BA = D_800BE9E4;
            D_800BE9E4 += obj->mv->unk1A8;
            D_800BE9A4 = D_800BE9E4 * 0.5f;
            if (D_800BE9A4 != 0.0f) {
                D_800BE9A8 = 1.0f / D_800BE9A4;
            } else {
                D_800BE9A8 = 0.0f;
            }
            obj->mv->unk1A8 = 0;
            return 2;
        }
        return 1;
    }
    ext->unk1A8 += D_800BE9E4;
    return 0;
}

void func_1504C9E4(Actor *obj, s8 target, u8 mode) {
    u8 step;
    s32 delta;
    s32 saved;

    delta = (u8)(obj->unk1D1 - target);
    if (delta != 0) {
        step = 3;
        if (mode == 16) {
            step = 6;
        }
        saved = delta;
        if (delta >= 128) {
            obj->unk1D1 += step;
        } else {
            obj->unk1D1 -= step;
        }
        // stop at the target once the difference changes sign
        delta = saved; // matching: the round trip through saved keeps a separate copy of delta in a3
        delta ^= obj->unk1D1 - target;
        if ((u8)delta & 0x80) {
            obj->unk1D1 = target;
        }
    }
}
