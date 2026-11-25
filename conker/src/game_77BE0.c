#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct GameObj {
    /* 0x000 */ s32 interaction_state;
    /* 0x004 */ u8 id;
    /* 0x005 */ u8 pad5[0xD];
    /* 0x012 */ u16 unk12;
    /* 0x014 */ f32 x_position;
    /* 0x018 */ f32 y_position;
    /* 0x01C */ f32 z_position;
    /* 0x020 */ f32 y_velocity;
    /* 0x024 */ f32 gravity;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 old_x_position;
    /* 0x030 */ f32 old_y_position;
    /* 0x034 */ f32 old_z_position;
    /* 0x038 */ s16 unk38;
    /* 0x03A */ u8 pad3A[0x2];
    /* 0x03C */ f32 xz_velocity;
    /* 0x040 */ u8 pad40[0x4];
    /* 0x044 */ f32 unk44;
    /* 0x048 */ f32 unk48;
    /* 0x04C */ u8 pad4C[0x8];
    /* 0x054 */ f32 unk54;
    /* 0x058 */ u8 pad58[0xD];
    /* 0x065 */ u8 unk65;
    /* 0x066 */ u8 pad66[0x10];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u8 pad78[0x6];
    /* 0x07E */ u8 unk7E;
    /* 0x07F */ u8 pad7F[0x1];
    /* 0x080 */ u8 unk80;
    /* 0x081 */ u8 unk81;
    /* 0x082 */ u8 pad82[0x1];
    /* 0x083 */ u8 unk83;
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[0x3];
    /* 0x089 */ u8 disable_run;
    /* 0x08A */ u8 pad8A[0x20];
    /* 0x0AA */ u8 unkAA;
    /* 0x0AB */ u8 padAB[0x2];
    /* 0x0AD */ u8 in_water;
    /* 0x0AE */ u8 padAE[0x4];
    /* 0x0B2 */ u16 unkB2;
    /* 0x0B4 */ u8 padB4[0x4];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 target_speed;
    /* 0x0C4 */ u8 padC4[0x30];
    /* 0x0F4 */ s32 unkF4;
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[0x4];
    /* 0x100 */ u8 unk100;
    /* 0x101 */ u8 pad101[0x3];
    /* 0x104 */ u8 stunned;
    /* 0x105 */ u8 pad105[0xB];
    /* 0x110 */ u8 unk110;
    /* 0x111 */ u8 pad111[0x7];
    /* 0x118 */ f32 unk118;
    /* 0x11C */ u8 pad11C[0xB];
    /* 0x127 */ u8 unk127;
    /* 0x128 */ u8 pad128[0xF];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x4];
    /* 0x13C */ u8 unk13C;
    /* 0x13D */ u8 pad13D[0x47];
    /* 0x184 */ u32 unk184;
    /* 0x188 */ void *unk188;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ u8 pad190[0x16];
    /* 0x1A6 */ s16 unk1A6;
    /* 0x1A8 */ u8 pad1A8[0x22];
    /* 0x1CA */ u8 health;
    /* 0x1CB */ u8 pad1CB[0x1];
    /* 0x1CC */ f32 unk1CC;
    /* 0x1D0 */ u8 pad1D0[0x4];
    /* 0x1D4 */ s32 unk1D4;
    /* 0x1D8 */ u8 pad1D8[0x84];
    /* 0x25C */ s32 unk25C;
    /* 0x260 */ u8 pad260[0x14];
    /* 0x274 */ u8 unk274;
    /* 0x275 */ u8 pad275[0x1];
    /* 0x276 */ s8 unk276;
    /* 0x277 */ u8 pad277[0xB];
    /* 0x282 */ s16 unk282;
    /* 0x284 */ u8 pad284[0x76];
    /* 0x2FA */ u8 unk2FA;
    /* 0x2FB */ u8 pad2FB[0x4];
    /* 0x2FF */ u8 unk2FF;
    /* 0x300 */ u8 unk300;
    /* 0x301 */ u8 pad301[0x17];
    /* 0x318 */ Camera *camera;
    /* 0x31C */ ActorCtl *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} GameObj; // size 0x32C

extern u16 D_800CBDA0[];
extern s32 *D_800DBF94;
void func_1508295C(s32, s32, s32);
void func_1507D4F8(s32);
s32 func_1504C8BC(GameObj *);
void func_150A11C4(void *);
void func_150A278C(void *);
void func_1507E73C(void *);
void func_1504AF10(Actor *, s32, s32);
void func_15061B4C(void);
void func_15080C64(void);

void func_1504A730(void) {
    GameObj *obj;
    s32 i;
    s32 tickResult;
    OSContPad noPad;
    f32 maxStep;
    f32 step;
    s32 j;

    D_800BE9EC = 0;
    D_800D1548 += 1;
    D_800CC250 += D_800BE9A0;
    D_800C3E78 = 0;
    D_800CC2BC = 0;
    func_1508295C(D_800BE9F0, 0, 0);
    D_800CC2B8 ^= 1;

    for (i = 0; i != 25; i++) {
        obj = (GameObj *)&D_800CC2D0[i];
        if (obj->interaction_state == 0) {
            continue;
        }
        obj->unk274 = 0;
        obj->unk300 = 0;
        obj->unkF4 &= 0xFFEE5FFF;
        D_800C3E78 = i;
        if (obj->y_position <= (f32)obj->unk38) {
            if (obj->unk127 == 0xFF) {
                func_15060F28((Actor *)obj, 0);
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
        D_800D154C = (Actor *)obj;
        obj->unk12++;
        *(u8 *)&D_800D35DC = i + 1;
        *(u16 *)&D_800CC2BA = 0;
        tickResult = 0;
        if (D_800BE616 == 0 || (tickResult = func_1504C8BC(obj)) != 0) {
            maxStep = 2.0f;
            if (i < D_8008FD8C) {
                obj->unk31C->unk195 -= D_800BE9E4;
                if (obj->unk31C->unk195 < 0) {
                    obj->unk31C->unk195 = 0;
                }
                obj->unk31C->unk57 = 0;
                // D_800CC284 is the current controller's OSContPad *, D_800BE728 points at one per controller
                if (i < 4) {
                    D_800CC284 = (*(s32 **)&D_800BE728)[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    *(OSContPad **)&D_800CC284 = &noPad;
                }
            }
            if (obj->camera != NULL) {
                if (obj->camera->unk384 == 999.0f) {
                    D_800CC280 = obj->camera->yaw * (65536.0f / 360.0f);
                } else {
                    D_800CC280 = obj->camera->unk384 * (65536.0f / 360.0f);
                }
            } else {
                D_800CC280 = 0;
            }
            if (obj->unk127 != 0xFF) {
                func_150A11C4(obj);
            } else if (obj->unkF8 & 0x400000) {
                func_150A278C(obj);
            }
            obj->old_x_position = obj->x_position;
            obj->old_y_position = obj->y_position;
            obj->old_z_position = obj->z_position;
            D_800CC27C = (obj->unk184 >> 28) & 7;
            step = D_800BE9A4;
            if (D_800BE616 != 0 && (D_8008FDBC & 0x100)) {
                maxStep = 5.0f;
            }
            if (maxStep < step) {
                step = maxStep;
            }
            D_800D1550[0] = obj->unk48 * step;
            D_800CC264 = D_800D1550[0] * 100.0f;
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
            if (!(obj->unk25C & 0x200)) {
                if (D_80086014[obj->interaction_state] != NULL) {
                    D_80086014[obj->interaction_state]((s32)obj);
                }
            }
            if (obj->interaction_state != 0) {
                func_1507E73C(obj);
                func_1504AF10((Actor *)obj, 0, 0);
                if (i < D_8008FD8C && D_800D18A8 == 0) {
                    func_15085710(obj->unk127, 5, obj->health);
                }
            }
            obj->unk2FF = 1;
            if (tickResult == 2) {
                D_800BE9E4 = *(u16 *)&D_800CC2BA;
                D_800BE9A4 = D_800BE9E4 * 0.5f;
                if (D_800BE9A4 != 0.0f) {
                    D_800BE9A8 = 1.0f / D_800BE9A4;
                } else {
                    D_800BE9A8 = 0.0f;
                }
            }
            if (obj->old_x_position != obj->x_position || obj->old_y_position != obj->y_position ||
                obj->old_z_position != obj->z_position) {
                obj->unk300 = 1;
            }
            if (D_800B0DF0->unk4A != 0) {
                D_8008600C[D_800B0DF0->unk4A]((s32)obj);
            }
        }
    }

    for (j = 0; j < D_8008FD8C; j++) {
        if (((GameObj *)D_800CC2D0)[j].interaction_state != 0 && ((GameObj *)D_800CC2D0)[j].unk31C != NULL) {
            if (((GameObj *)D_800CC2D0)[j].unk137 != 0) {
                if (D_800CBDA0[j] != 0) {
                    D_800DBEF4[D_800CBDA0[j] - 1].flags |= 0x84;
                    D_800DBF94[D_800CBDA0[j] - 1] |= 1 << j;
                }
                ((GameObj *)D_800CC2D0)[j].unk31C->unk56 = 5;
            } else if (((GameObj *)D_800CC2D0)[j].unk31C->unk56 != 0) {
                ((GameObj *)D_800CC2D0)[j].unk31C->unk56--;
            }
        }
    }
    func_15061B4C();
    func_15080C64();
    D_800D154C = D_800CC2D0;
    D_800C3E78 = 0;
    *(u8 *)&D_800D35DC = 0;
}

void func_150ED578(Actor *arg0);

void func_1504ADD0(void) {
    s32 i;
    Actor *obj;
    OSContPad noPad;

    if (D_800C35EA != 1) {
        for (i = 0; i < 25; i++) {
            obj = &D_800CC2D0[i];
            if (obj->interaction_state == 0) {
                continue;
            }
            D_800C3E78 = i;
            D_800D154C = obj;
            if (i < D_8008FD8C) {
                if (i < 4) {
                    D_800CC284 = (*(s32 **)&D_800BE728)[i];
                } else {
                    bzero(&noPad, sizeof(noPad));
                    *(OSContPad **)&D_800CC284 = &noPad;
                }
            }
            switch (obj->id) {
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
                D_800DBEF4[index].attach = 0;
                obj->unk92 = 0;
                return;
            }
            D_800DBEF4[index].attach = (obj - D_800CC2D0) + 100;
            if (force) {
                D_800DBEF4[index].attach = 0;
                return;
            }
            D_800DBEF4[index].flags &= ~0x60;
            D_800DBEF4[index].flags |= 0x20;
            D_800DBEF4[index].x = obj->x_position;
            D_800DBEF4[index].y = obj->y_position + (s16)obj->unk90;
            D_800DBEF4[index].z = obj->z_position;
        }
    }
}

void func_1510F820(void *, void *, f32 *, f32 *, f32 *, f32 *, f32 *);
f32 func_1505210C(Actor *arg0, f32 arg1, f32 arg2, f32 *arg3, f32 arg4);
void func_15060A30(s32, void *);
void func_1507CD64(void *, s32);

f32 func_1504B0FC(GameObj *obj, f32 arg1) {
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
    obj->unk31C->unk68 = 0;
    obj->unk110 = 0;
    if (obj->stunned == 0 && obj->health != 0 && obj->unk31C->unk198 == 0) {
        stickX = (*(OSContPad **)&D_800CC284)->stick_x;
        stickY = (*(OSContPad **)&D_800CC284)->stick_y;
    } else {
        stickX = 0;
        stickY = 0;
    }
    if (obj->unk31C->unk22 > 100) {
        stickY = 80;
    }
    delta = (obj->y_position - ((obj->unk118 - 60.0f) - 170.0f)) * 2.0f;
    if ((obj->unk31C->unk20 & 1) || obj->unk31C->unk21 != 0) {
        obj->unkB2 = 0;
        if (delta < 400.0f) {
            // matching: loading 8.0f into step first gives the target's mul.s operand order
            step = 8.0f;
            step *= D_800D1550[0];
            if (stickY > 10) {
                stickY = 10;
            }
            if (delta + step > 400.0f) {
                step = 400.0f - delta;
            }
            obj->y_position += step;
        }
    }
    if (obj->in_water == 1) {
        obj->unk1CC = obj->y_position;
        if (obj->unk118 == -10000.0f) {
            func_15052408((Actor *)obj);
            return 0.0f;
        }
        if (stickY != 0 || ((obj->unk118 - 60.0f) - 175.0f) < obj->y_position) {
            targetPitch = stickY * 2.25f;
        } else {
            targetPitch = obj->unkB8;
        }
        if (obj->unk31C->unk21 != 0) {
            targetPitch = 0.0f;
        }
        func_15059444((Actor *)obj);
        if ((obj->stunned != 0 && (obj->health != 0 || obj->y_velocity >= 60.0f)) || obj->unk84 == 0x262) {
            return 0.0f;
        }
        if (delta > 0.0f && obj->unk84 != 0x22) {
            if (delta > 300.0f) {
                delta = 300.0f;
            }
            obj->y_position += delta * arg1 * 0.002997f;
        }
    } else if ((obj->gravity < 0.0f && obj->y_velocity > 0.0f) || (obj->gravity > 0.0f && obj->y_velocity < 0.0f)) {
        obj->gravity = 0.0f;
        obj->y_velocity = 0.0f;
    }
    if (obj->in_water != 1 || obj->unk28 >= 10.0f || obj->y_position < (obj->unk118 - 60.0f) + 20.0f) {
        if (obj->in_water == 1) {
            if ((obj->unk118 - 50.0f < obj->unk1A6 || (obj->unk31C->unk20 & 1)) && obj->health != 0) {
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
                if (obj->unkB2 > 1560 || (obj->health == 0 && obj->unk31C->unk120 == 0)) {
                    obj->unkB2 -= 60;
                    if (obj->health >= 2) {
                        if (obj->health == 4) {
                            func_15060A30(0x499, obj);
                        }
                        obj->health--;
                    } else {
                        obj->health = 0;
                        if (obj->stunned != 0) {
                            obj->gravity = 0.0f;
                            func_1507CD64(obj, 4);
                        } else {
                            func_1507CD64(obj, 3);
                        }
                    }
                } else if (obj->unkB2 > 1440 && obj->unkB2 < 1500) {
                    obj->unkB2 += 120;
                    if (obj->health < 4) {
                        func_15060A30(0x499, obj);
                    }
                }
            }
            if ((obj->y_velocity != 0.0f || obj->gravity != 0.0f) && obj->gravity < 0.0f && obj->y_velocity > 0.0f) {
                obj->gravity = 0.0f;
                obj->y_velocity = 0.0f;
            }
            if (((obj->unk118 - 60.0f) - 175.0f) < obj->y_position && obj->unk84 != 0x28) {
                if (obj->unk31C->unk1C < 2.5f) {
                    obj->unk31C->unk1C += 0.05f;
                }
            } else {
                obj->unk31C->unk1C = 0.0f;
            }
            obj->y_position += obj->unk31C->unk1C * D_800D1550[0];
            excess = obj->y_position - ((obj->unk118 - 60.0f) + 14.0f);
            if (excess > 0.0f) {
                excess *= 0.65f;
                obj->y_position -= excess;
                obj->unk31C->unk1C = 0.0f;
            }
            if (((obj->unk118 - 60.0f) - 75.0f) < obj->y_position) {
                targetPitch -= 30.0f;
            }
        }
        if (obj->unk84 == 0x22 && targetPitch < -20.0f) {
            targetPitch = -20.0f;
        }
        if (obj->unk28 < 40.0f) {
            func_1510F820(obj->unk188, &obj->unk18C, &out0, &out1, &out2, &out3, &out4);
            out3 = func_1505210C((Actor *)obj, out3, out4, &out4, 180.0f);
            if (out4 < obj->unkB8) {
                obj->unkB8 += (out4 - obj->unkB8) * 0.2f;
            }
        }
        obj->unkBC = targetPitch - obj->unkB8;
        if (2.7f * D_800D1550[0] > fabsf(obj->unkBC)) {
            obj->unkB8 = targetPitch;
        } else {
            if (obj->unkBC > 2.7f) {
                obj->unkBC = 2.7f;
            }
            if (obj->unkBC < -2.7f) {
                obj->unkBC = -2.7f;
            }
            obj->unkB8 += obj->unkBC * D_800D1550[0];
        }
        if (obj->unkB8 < -90.0f) {
            obj->unkB8 = -90.0f;
        }
        if (obj->unkB8 > 90.0f) {
            obj->unkB8 = 90.0f;
        }
        if (obj->unk31C->unk21 == 0) {
            if ((stickX + 8) / 18 != 0) {
                obj->unk76 -= ((((stickX + 8) / 18) * D_800CC264) << 8) / 100;
                obj->unk7E = 0x17;
            }
            result = obj->target_speed;
        }
        obj->unk80 = 1;
    } else {
        func_15052408((Actor *)obj);
        if (obj->xz_velocity > 16.0f) {
            obj->xz_velocity = 16.0f;
        }
        result = obj->xz_velocity;
    }
    if (obj->unk31C->unk95 != 0) {
        obj->y_position = obj->old_y_position;
        belowLevel = obj->unk118 != -10000.0f && obj->y_position < obj->unk118 - 60.0f;
        if (!belowLevel) {
            obj->in_water = 0;
        }
    }
    return result;
}
// 3-byte entries of D_80099140
typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
} Struct80099140;

void func_1504BA38(Actor *obj) {
    Struct80099140 *entry;
    s8 unk2;
    s32 unk1;

    if (obj->unk28 > 10.0f) {
        obj->unkCE = 0;
        unk1 = entry->unk1; // matching: dead use of entry here sets the register allocation
        return;
    }
    entry = &((Struct80099140 *)D_80099140)[obj->unk184 & 0x1F];
    unk2 = entry->unk2;
    if (unk2 != 0 && obj->unkAA == 0) {
        obj->unkAA = unk2;
    }
    unk1 = entry->unk1;
    if (unk1 == 0) {
        obj->unkCE = entry->unk0;
        return;
    }
    obj->unkCE = unk1 * D_800991D4 * (entry->unk0 - obj->xz_velocity);
    if (unk2) { // matching: keeps unk2 live to the end (in a1 rather than v0)
    }
}

void func_1504BAF0(Actor *obj) {
    s16 index;
    Actor *other;

    index = obj->unk13C - 100;
    if (index >= 0) {
        other = &D_800CC2D0[index];
        if (other->unk13D >= 100 && (other->id == 168 || other->id == 169)) {
            D_800D1580 = 3;
            func_1507490C();
        }
    }
    func_1506EBC0();
}

void func_1504BB88(Actor *obj) {
    if (obj->unk25C & 0x10) {
        obj->unk83 = 0xFF;
        obj->disable_run = 0xFF;
        if (obj->interaction_state == 1) {
            func_1505E650(obj, 0xD6, 1.15f, 3.0f, 0.0f, 0.0f, 0);
        }
        obj->unk25C &= ~0x10;
    }

    if (obj->unk25C & 2) {
        if (obj->unk13C != 0) {
            func_1504BAF0(obj);
        }
        obj->unk25C &= ~2;
    }
}

void func_1504BC38(Actor *obj) {
    if (obj->disable_run != 0 && obj->disable_run < 250) {
        if (obj->disable_run <= D_800BE9A0) {
            obj->disable_run = 0;
        } else {
            obj->disable_run -= D_800BE9A0;
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
    if (obj->disable_jump != 0xFF) {
        if (obj->disable_jump <= D_800BE9A0) {
            obj->disable_jump = 0;
        } else {
            obj->disable_jump -= D_800BE9A0;
        }
    }
    if (obj->immune != 0xFF) {
        if (obj->immune <= D_800BE9A0) {
            obj->immune = 0;
        } else {
            obj->immune -= D_800BE9A0;
        }
    }
    if (obj->unk31C->chased > 0) {
        obj->unk31C->chased -= D_800BE9A0;
    }
    if (obj->unk31C->chasing > 0) {
        obj->unk31C->chasing -= D_800BE9A0;
    }
    if (obj->unk31C->unk44 > 0) {
        obj->unk31C->unk44 -= D_800BE9A0;
    }
    if (obj->unk107 != 0) {
        obj->unk107--;
    }
    if (obj->unk1D0 > 0) {
        obj->unk1D0 -= D_800BE9A0;
    }
    if (obj->unk31C->unk54 > 0) {
        obj->unk31C->unk54 -= D_800BE9A0;
    }
    if (obj->unk31C->unk55 != 0) {
        obj->unk31C->unk55--;
    }

    obj->unk31C->unk52 >>= 1;

    if (obj->unk31C->unk19E != 0) {
        if (obj->unk31C->unk19E > D_800BE9E4) {
            obj->unk31C->unk19E -= D_800BE9E4;
        } else {
            obj->unk31C->unk19E = 0;
        }
    }
    if (obj->unk31C->unk1A0 != 0) {
        if (obj->unk31C->unk1A0 > D_800BE9E4) {
            obj->unk31C->unk1A0 -= D_800BE9E4;
        } else {
            obj->unk31C->unk1A0 = 0;
        }
    }
}

void func_1507EB2C(void *arg0);
extern s16 D_800CC2B2;
extern f32 D_800CC2B4;

void func_1504BE2C(GameObj *obj, u16 *arg1, f32 *arg2, s8 *arg3) {
    s32 pad[2]; // matching: the frame has two unused words
    f32 angle;

    angle = obj->unk31C->unkB * 0.024543693f; // pi / 128: unkB counts 256ths of a turn
    D_800CC2B4 = 16.0f;
    if (obj->unk31C->unk16 != 0 && obj->unk31C->unk16 == 0) { // never true, but compiled in
        func_1507EB2C(obj);
        obj->unk31C->unk8 = 0;
    }
    if (obj->unk31C->unkE == 0) {
        obj->unk31C->unkE = 0xA6;
    }
    obj->unk31C->unk3C = 5;
    if (arg1) { // matching: this and the empty if below keep arg1 live in a1 across the calls
    }
    if (*arg2 > 4.0f && obj->unk84 == 0xAA) {
        obj->unk31C->unk3C = 1;
        obj->unk31C->unk10 = *arg1;
    }
    func_150AD78C(angle);
    if (arg1) {
    }
    obj->unk31C->unkB += 2;
    if (obj->unk31C->unk16 != 0) {
        obj->unkAA = 25;
    } else if (obj->unk84 == 0xA7) {
        obj->unkAA = 50;
    } else {
        obj->unkAA = 32;
    }
    obj->unk54 *= 0.5f;
    if (obj->unk84 == 0x2C || obj->unk84 == 0xE0) {
        *arg3 = (*(OSContPad **)&D_800CC284)->stick_y >> 1;
    } else {
        if (!(func_150ADA20() & 0x3F)) {
            obj->unk31C->unk14 = (func_150ADA20() % 25000U) - 12500;
        }
        D_800CC2B2 = obj->unk31C->unk14;
        if (obj->unk31C->unk16 != 0) {
            D_800CC2B2 /= 3;
        }
    }
    if (D_800C35EA == 0 && obj->unk31C->unk16 == 0) {
        obj->unk282 = 30;
        obj->unk276 = 0;
    }
}

s32 func_1504C078(void) {
    if (D_800D154C->unk31C->idle.active != 1) {
        D_800D154C->unk31C->idle.holdFrames = 0;
        func_1507F640();
    }
    return 999;
}

s32 func_1504C0B8(void) {
    if (D_800BE9F0 == 27 || D_800BE9F0 == 30) {
        return 395;
    }
    return 27;
}

void func_15064A14(void *);
s32 func_15064B94(GameObj *, f32, f32, s32, f32);
extern f32 D_800991DC;
extern f32 D_800991E0;
extern f32 D_800991E4;
extern f32 D_800991E8;
extern f32 D_800991EC;
extern f32 D_800991F0;
extern f32 D_800991F4;
extern f32 D_800991F8;
extern f32 D_800991FC;
extern f32 D_80099200;
extern u8 D_800CC2B1;

void func_1504C0E8(GameObj *obj, s32 arg1, f32 arg2) {
    f32 blend;
    f32 start;
    f32 vel;
    s32 pressed;
    f32 speed;
    s32 anim;
    ActorCtl *ext;
    f32 diff;

    if (D_800CC2B2 == 0 && (ext = obj->unk31C)->unk12 >= -29 && ext->unk12 < 30) {
        ext->unk12 = 0;
    } else {
        ext = obj->unk31C;
        // matching: computed in two statements to keep the target's float register order
        diff = D_800CC2B2 - ext->unk12;
        diff = (D_800D1550[0] / D_800CC2B4) * diff;
        ext->unk12 += (s16)diff;
    }
    ext = obj->unk31C;
    if (ext->unk6B != 0) {
        return;
    }
    anim = 999;
    if (obj->unk65 != 0 && obj->stunned == 0) {
        func_15064A14(obj);
        return;
    }
    if (obj->unk81 == 0 && obj->stunned == 0 && obj->in_water == 0) {
        if (obj->unk83 == 0 && ext->unk11A != 3) {
            obj->unk80 = 1;
            obj->unk100 &= 0xF3;
            D_800CC2BC |= 0x80;
            speed = 1.0f;
            start = 0.0f;
            blend = 4.0f;
            vel = obj->xz_velocity * D_800991DC + arg2 * D_800991E0;
            ext = obj->unk31C;
            if (ext->unk46 > 0 || ext->unk4E != 0 || obj->unk13C >= 100 || arg1 != 0 ||
                (ext->unk32 & 3) || ext->unk78 != 0 || ext->unk1A != 0 ||
                ext->unk8 > 0 || ext->unk17 != 0) {
                if (func_15064B94(obj, vel, vel, arg1, arg2) != 0) {
                    return;
                }
                ext = obj->unk31C;
            }
            pressed = (*(OSContPad **)&D_800CC284)->button & Z_TRIG;
            if (pressed == 0 || D_800CC2B1 != 0 || ext->unk78 != 0 || (ext->unk4E & 0xF) == 1) {
                if (obj->xz_velocity > 4.0f) {
                    ext->unk37 = 0;
                    ext = obj->unk31C;
                }
                pressed = ext->unk4E; // matching: reusing pressed for this value sets the register allocation
                if ((ext->unk4E == 1 || pressed == 0x81) && vel > 1.0f) {
                    arg2 = 10.0f;
                    anim = 0x302;
                    speed = obj->xz_velocity / 7.0f + D_800991E4;
                    if (obj->xz_velocity > 23.0f) {
                        anim = 0x303;
                        speed = obj->xz_velocity / 23.0f + D_800991E8;
                    }
                } else if (ext->chased > 0 && vel >= 5.0f) {
                    anim = 0x2B;
                    speed = vel / 12.0f;
                } else if (ext->chasing > 0 && vel >= 5.0f) {
                    anim = 0xDF;
                    speed = vel / 12.0f;
                } else if (obj->unk118 != D_800991EC && obj->y_position < obj->unk118 &&
                           obj->xz_velocity >= 1.0f && ext->unk4E == 0) {
                    speed = vel / 18.0f + D_800991F0;
                    anim = 0x8B;
                    obj->unkAA = 0x4B;
                } else if (vel > 14.0f) {
                    anim = (D_800BE9F0 != 4) ? 2 : 0x1B2;
                    speed = vel / 23.0f + D_800991F4;
                } else if (vel > 6.0f || (arg2 < 1.0f && obj->xz_velocity > 1.0f)) {
                    anim = 1;
                    speed = vel * 0.125f + D_800991F8;
                } else if (vel > 1.0f) {
                    blend = 15.0f;
                    anim = (obj->id == 0x80) ? 1 : 0;
                    speed = vel * 0.125f + D_800991FC;
                } else {
                    anim = func_1504C078();
                }
                if (obj->unk28 > 60.0f && obj->unk31C->unk4E == 0) {
                    speed = 0.5f;
                    anim = 0x38;
                    obj->unk100 |= 0x10;
                }
                if (obj->xz_velocity > 18.0f && obj->unk31C->unk11A != 1 && obj->unk31C->unk11A != 3 && arg2 == 0.0f &&
                    obj->unk28 == 0.0f && D_800CC27C == 0) {
                    speed = 0.5f;
                    anim = 0x21;
                    blend = 3.0f;
                    obj->disable_run = 8;
                    obj->unk83 = 0x11;
                }
            } else if (pressed != 0) {
                if (obj->xz_velocity < 3.0f) {
                    ext->unk37 = 1;
                }
                if (obj->unk44 > D_80099200 && (ext = obj->unk31C)->unk37 != 0) {
                    ext->unk4 = 1;
                    obj->unkAA = 0x28;
                    blend = 5.0f;
                    speed = obj->unk44 / 15.0f + 0.5f;
                    anim = func_1504C0B8();
                } else {
                    blend = 6.0f;
                    speed = 1.5f;
                    anim = 0x16;
                }
            }
        }
        if (anim != 999) {
            func_1505E650((Actor *)obj, anim, speed, blend, start, 0.0f, 0);
        }
    } else if (obj->unk84 == 0x21 && D_800CC2B1 == 0 && obj->unk44 == 0.0f &&
               ((*(OSContPad **)&D_800CC284)->button & Z_TRIG)) {
        func_1505E650((Actor *)obj, 0x16, 1.5f, 6.0f, 0.0f, 0.0f, 0);
    }
}

void func_1504C854(Actor *obj) {
    D_800CC288 = obj->unk31C->unk8C;
    if (obj->unk31C->unk8E > 0) {
        obj->unk31C->unk8E -= D_800BE9A0;
    } else {
        obj->unk31C->buttons = 0;
    }
    if (obj->unk31C->unk8F > 0) {
        obj->unk31C->unk8F -= D_800BE9A0;
    } else {
        obj->unk31C->unk8C = 0;
    }
}

s32 func_1504C8BC(GameObj *obj) {
    ActorCtl *ext;

    ext = obj->unk31C;
    if (ext == NULL) {
        return 1;
    }
    if (obj->unk1D4 != 0 || D_800BE9F0 == 0x33 || obj->interaction_state == 0x25) {
        ext->unk1A8 = 0;
        return 1;
    }
    if ((obj->unk127 & 1) == D_800CC2B8) {
        if (ext->unk1A8 != 0) {
            *(s16 *)&D_800CC2BA = D_800BE9E4;
            D_800BE9E4 += obj->unk31C->unk1A8;
            D_800BE9A4 = D_800BE9E4 * 0.5f;
            if (D_800BE9A4 != 0.0f) {
                D_800BE9A8 = 1.0f / D_800BE9A4;
            } else {
                D_800BE9A8 = 0.0f;
            }
            obj->unk31C->unk1A8 = 0;
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
