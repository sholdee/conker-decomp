#include <ultra64.h>

#include "functions.h"
#include "variables.h"

// matching: one struct pins the stack layout of these locals (checksum at sp+0xFC)
typedef struct {
    Vec3i pos;
    LightEffectHeader header;
    u8 pad12[0xA];
    s32 checksum;
} Struct150C3D5CLocals;

Gfx *func_150C3160(Gfx *gdl, Actor *obj) {
    s32 total;
    s32 uls;
    s32 ult;
    f32 progress;

    total = obj->taskint[1];
    progress = (total != 0) ? ((f32)obj->taskint[0] / total) : 1.0f;
    progress = 1.0f - progress;
    uls = (500.0f * progress) + 2.0f;
    ult = 2 - obj->taskint[2];
    obj->taskint[2] = uls / 3;
    while (ult < 0) {
        ult += 64;
    }
    gDPSetTileSize(gdl++, 4, uls, ult, 0x1FE, 0x03E);
    return gdl;
}
typedef struct {
    Actor *unk0;
    f32 pos[3];
    s32 targetIndex;
    s32 unk14;
} Struct150C3230;

void func_150C3230(Actor *obj) {
    s32 index;
    s32 type;
    Actor *target;
    Struct150C3230 eventData;

    obj->noshove |= 0x4000000;
    if ((obj->unk222 != 0) && (obj->attach_char == 0)) {
        obj->old_xpos = movement_obj[obj->owner - 1].xpos;
        obj->old_ypos = movement_obj[obj->owner - 1].ypos;
        obj->old_zpos = movement_obj[obj->owner - 1].zpos;
        obj->unk222 = 0;
    }

    func_15059C84(obj);
    func_1505A770(obj);
    func_15058898(obj, obj->old_ypos);
    func_1505B5F8(obj, obj->floorY);

    if (obj->shad == 0.0f) {
        obj->speed *= 0.5f;
    }

    index = obj->attach_char;
    if (index != 0) {
        obj->unk222 = index;
        if (movement_obj[index - 1].animseq.uh == 0x1F9) {
            type = 2;
        } else if (movement_obj[index - 1].animseq.uh == 0x1FB) {
            type = 3;
        } else {
            typedef s32 Type150C3230; // matching: a declaration here gives -g3 a block marker; as1 then fills the delay slot from this branch
            type = 1;
        }
    } else {
        type = 0;
    }

    func_1505E650(obj, type, 0.0f, 0.0f, 0.0f, 0.0f, 0);

    if (obj->attach_char != 0) {
        func_1507C324(obj, &movement_obj[obj->attach_char - 1]);
    }

    if (obj->taskint[0] != 0) {
        if (obj->attach_char != 0) {
            target = &movement_obj[obj->attach_char - 1];
            if (target->type == 5) {
                obj->attach_char = 0;
                obj->taskint[0] = 0;
            }
        } else {
            target = NULL;
        }

        if ((D_800BE616 == 0) && (D_800C35EA[0] == 0) && (obj->attach_char == 0)) {
            obj->taskint[0] = 0;
        }

        if (D_800BE9E4 < obj->taskint[0]) {
            obj->taskint[0] -= D_800BE9E4;
            if (D_800BE616 != 0) {
                func_1508B20C(obj->xpos, obj->ypos, obj->zpos, 900.0f);
            }
        } else {
            eventData.pos[0] = obj->xpos;
            eventData.pos[1] = obj->ypos;
            eventData.pos[2] = obj->zpos;
            eventData.unk0 = &movement_obj[obj->owner - 1];
            eventData.targetIndex = obj->attach_char;
            if (target != NULL) {
                target->imune = target->stunned = 0;
                if (target->energy != 0) {
                    target->energy = 1;
                }
            }
            func_1506AC8C(obj, (D_800BE616 != 0) ? 0xB : 0xE, &eventData);
            func_15060F28(obj, 1);
        }
    }
}

void func_150C3574(Actor *obj) {
    f32 accel;
    f32 dist;
    s32 vol;
    f32 scale;
    u16 angle;

    accel = 0.25f;
    scale = 0.5f;
    obj->count2 = 10;
    if (obj->character_type == 0x25) {
        func_1505A250(0.0f, 0.0f, 0.15f, &obj->ob_slopex, &obj->ob_slopey);
        accel = 0.1f;
        if (obj->unk87 & 1) {
            obj->unk224 = obj->ypos;
        }
        obj->shadow_size = 0;
    }
    func_1505A3A8(0.0f, obj, 2.0f, accel, 0);
    obj->ygrav = 3.0f;
    if (obj->character_type == 0x10) {
        scale = 0.375f;
    }
    if (obj->character_type == 0xAC) {
        scale = 1.5f;
    }
    scale *= 0.75f;
    func_15056B08(obj);
    if (obj->carried != 0) {
        func_150562FC(obj);
    }
    func_15059140(obj);
    obj->unkBC = (obj->xpos - obj->old_xpos) * D_800BE9A8;
    obj->unk148 = (obj->zpos - obj->old_zpos) * D_800BE9A8;
    dist = sqrtf(obj->unkBC * obj->unkBC + obj->unk148 * obj->unk148);
    if ((dist + 4.0f) + (dist + 4.0f) < obj->speed) {
        obj->speed = dist + dist;
    }
    if (obj->unk87 == 4) {
        D_800D1580 = 0xFF020133;
        D_800D154C = movement_obj;
        func_1506E8D8();
        D_800D154C = obj;
        obj->ob_slopex = obj->unkBC;
        obj->ob_slopey = obj->unk148;
        obj->speed = 0.0f;
    }
    obj->unk87 *= 2;
    if ((dist > 0.5f) && (obj->carried == 0)) {
        obj->fangle = func_1505A630(obj->xpos - obj->old_xpos, obj->old_zpos - obj->zpos, 0);
    }
    if (((obj->langle - obj->fangle) - 0x4000) & 0x8000) {
        angle = obj->fangle;
    } else {
        angle = obj->fangle ^ 0x8000;
    }
    func_150599C8(obj, 2, angle);
    if (obj->shad > 30.0f) {
        obj->stickspeed *= 0.9f;
    } else {
        obj->stickspeed = dist + dist;
    }
    if (obj->carried != 0) {
        obj->stickspeed = 0.0f;
    }
    func_15052490(obj, obj->fangle, obj->stickspeed, (2.0f - obj->scale_xz) * scale);
    if (obj->stickspeed < 0.2f) {
        func_100109D0(obj);
    } else if (func_100107F8(obj) == 0) {
        if (obj->character_type == 0xAC) {
            D_800D1580 = 0x630;
        } else if (obj->character_type == 0x25) {
            D_800D1580 = 0x67A;
        } else {
            D_800D1580 = 0x62A;
        }
        func_1506E5FC();
    } else {
        vol = obj->stickspeed * 2500.0f;
        if (vol > 0x6D60) {
            vol = 0x6D60;
        }
        if (obj->character_type == 0x25) {
            vol >>= 1;
        }
        func_1001091C(obj, vol);
    }
    if (obj->stickspeed < 0.2f) {
        func_100109D0(obj);
    }
}

void func_150C3994(Actor *obj) {
    if (obj->type == 0x14) {
        if (obj->taskint[0] == 0) {
            func_151B9BF0(0x2F, 1, obj->xpos, obj->ypos + 30.0f, obj->zpos, 0x7D0, 0x3E8, 0x190, 0xC8, 0xC00, 0xC00, 1, 1, 0, 0xFF, 0);
        } else {
            func_15188810(obj, 0x64, 0);
        }
    }
    if (obj->yspd < 0.0f) {
        if ((obj->shad > 20.0f) && (obj->unk136 == 0) && (obj->shad < (-20.0f * obj->yspd))) {
            func_10010630(0x92, obj, 0x7D00, 0xC8, 0x9C4);
            obj->unk136 = 1;
        }
    }
    if (obj->count5 == 0) {
        obj->unk148 = 70.0f;
        if (obj->taskint[0] == 0) {
            func_151B9BF0(0x2F, 1, obj->xpos, obj->ypos + 30.0f, obj->zpos, 0x898, 0x500, 0x320, 0x2BC, 0xC00, 0xC00, 0x14, 1, 0, 0xFF, 0);
            func_150C1660(obj->xpos, obj->ypos + 30.0f, obj->zpos, 0xFF);
            func_1506AC8C(obj, 6, NULL);
        } else {
            func_15188810(obj, 0x64, 0);
        }
        func_10010344(0x1A9, obj, 0x7FFF, 0x3E8, 0x7D0);
        obj->doing_hit = 1;
        obj->unk114 = 500.0f;
        obj->noshove |= 0x01008000;
    }
    obj->imune = 0xFF;
    obj->count5++;
    if (obj->count5 >= 0xB) {
        obj->noshove &= ~0x8000;
    }
    obj->theta = (obj->langle + 0x4000) * 0.005493164f; // 360 / 65536: binary angle to degrees
    func_15059140(obj);
    obj->vtilt += obj->unk148;
    obj->unk148 *= 0.96f;
    if (((obj->shad == 0.0f) && (obj->count5 >= 0x15) && (obj->unkCC == 0)) || (obj->ivehitit != 0) || (obj->energy == 0)) {
        func_150C3D5C(obj);
        func_10010630(0x93, obj, 0x7D00, 0xC8, 0x9C4);
        func_15060F28(obj, 1);
    }
}

s32 func_150C3D48(s32 value) {
    return value + 0xEDCBA988;
}

void func_150C3D5C(Actor *obj) {
    Struct150C3D5CLocals locals;

    func_151B9BF0(0x2F, 1, obj->xpos, obj->ypos,
                  obj->zpos, 0x578, 0x500, 0x320, 0x4B0, 0x200, 0x200,
                  0x1E, 1, 0, 0xFF, 0);

    locals.checksum = 0xC23459B8;
    // matching: calling through a function-pointer cast makes IDO emit jalr
    ((void (*)(s32, s32 *))func_151EF040)(func_150C3D48(locals.checksum), &locals.checksum);
    if (locals.checksum != 0x0330C820) {
        // tamper check failed: overwrite func_10004074 with "jr $ra; nop"
        ((s32 *)&func_10004074)[0] = 0x03E00008;
        ((s32 *)&func_10004074)[1] = 0;
    }

    func_15171200(3, 0xCB, obj->xpos, obj->ypos, obj->zpos, 10.0f, 1.0f,
                  2.0f, 60.0f, 3, 0x3C, -0.7f, 0xFF, 0);
    func_151709B4(obj->xpos, obj->ypos + 20.0f,
                  obj->zpos, 0xA, 2000.0f, 0xFF, 0);
    func_1516D99C(obj->xpos, obj->ypos,
                  obj->zpos, 0xD, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
                  0, 0xFF, 0, 0, 0, 0x14, 0x14, 2, 0, 0, 0, 0, 0x51, 0x51,
                  0x51, 0x51, 0xA, 1, 0, 0xFF, 0xA, 0x578, 0x190, 1, 1, 0, 1,
                  0, 0, 0, 0, 0, 0xFF, 0);

    locals.header.flags = 3;
    locals.header.update = -1;
    locals.header.lifetime = (func_150ADA20() % 11U) + 0xA;
    locals.header.lightType = 0;
    locals.pos.x = obj->xpos;
    locals.pos.y = obj->ypos;
    locals.pos.z = obj->zpos;
    func_151602C0(&locals.header, &locals.pos, (func_150ADA20() % 52U) + 0x28,
                  0xFF, 0xFF, 0x5A, 0xFF, 0, 0, 0xFF, 1);
}
