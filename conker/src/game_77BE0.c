#include <ultra64.h>

#include "functions.h"
#include "variables.h"


// File-local views of a game object (struct127, the 0x32C-byte entries of D_800CC2D0) and of the
// extension block it points to at 0x31C (struct126). include/structs.h lacks or mistypes several
// fields used here (0x12 is a u16, 0x38 an s16, 0xBC an f32, ...); fold them in during the central pass.
typedef struct GameObjExt {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s16 unk8;
    /* 0x00A */ u8 padA[0x1];
    /* 0x00B */ u8 unkB;
    /* 0x00C */ u8 padC[0x2];
    /* 0x00E */ u16 unkE;
    /* 0x010 */ u16 unk10;
    /* 0x012 */ s16 unk12;
    /* 0x014 */ u16 unk14;
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 pad18[0x2];
    /* 0x01A */ u8 unk1A;
    /* 0x01B */ u8 pad1B[0x1];
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 unk20;
    /* 0x021 */ u8 unk21;
    /* 0x022 */ s8 unk22;
    /* 0x023 */ u8 pad23[0xF];
    /* 0x032 */ u16 unk32;
    /* 0x034 */ u8 pad34[0x3];
    /* 0x037 */ u8 unk37;
    /* 0x038 */ u8 pad38[0xE];
    /* 0x046 */ s16 unk46;
    /* 0x048 */ u8 pad48[0x2];
    /* 0x04A */ s8 chased;
    /* 0x04B */ s8 chasing;
    /* 0x04C */ u8 pad4C[0x2];
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 pad4F[0x7];
    /* 0x056 */ u8 unk56;
    /* 0x057 */ u8 unk57;
    /* 0x058 */ u8 pad58[0x10];
    /* 0x068 */ s16 unk68;
    /* 0x06A */ u8 pad6A[0x1];
    /* 0x06B */ u8 unk6B;
    /* 0x06C */ u8 pad6C[0xC];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[0x1C];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x84];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[0x5];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0x74];
    /* 0x195 */ s8 unk195;
    /* 0x196 */ u8 pad196[0x2];
    /* 0x198 */ u8 unk198;
    /* 0x199 */ u8 pad199[0xF];
    /* 0x1A8 */ u8 unk1A8;
    /* 0x1A9 */ u8 pad1A9[0x3];
} GameObjExt; // size 0x1AC

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
    /* 0x275 */ u8 pad275[0x85];
    /* 0x2FA */ u8 unk2FA;
    /* 0x2FB */ u8 pad2FB[0x4];
    /* 0x2FF */ u8 unk2FF;
    /* 0x300 */ u8 unk300;
    /* 0x301 */ u8 pad301[0x17];
    /* 0x318 */ struct108 *camera;
    /* 0x31C */ GameObjExt *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} GameObj; // size 0x32C

// struct131 (the 0xA0-byte entries of D_800DBEF4) with unk4F read as the unsigned flag byte it is here
typedef struct {
    /* 0x00 */ u8 pad0[0x4F];
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 pad50[0x50];
} Struct131u;

extern u16 D_800CBDA0[];
extern s32 *D_800DBF94;
void func_1508295C(s32, s32, s32);
void func_1507D4F8(s32);
s32 func_1504C8BC(void *);
void func_150A11C4(void *);
void func_150A278C(void *);
void func_1507E73C(void *);
void func_1504AF10(struct127 *, s32, s32);
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
                func_15060F28((struct127 *)obj, 0);
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
        D_800D154C = (struct127 *)obj;
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
                    D_800CC280 = obj->camera->unk37C * (65536.0f / 360.0f);
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
                func_1504AF10((struct127 *)obj, 0, 0);
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
                    ((Struct131u *)D_800DBEF4)[D_800CBDA0[j] - 1].unk4F |= 0x84;
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

void func_150ED578(struct127 *arg0);

void func_1504ADD0(void) {
    s32 i;
    struct127 *obj;
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

void func_1504AF10(struct127 *obj, s32 force, s32 release) {
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
                D_800DBEF4[index].unk4E = 0;
                obj->unk92 = 0;
                return;
            }
            D_800DBEF4[index].unk4E = (obj - D_800CC2D0) + 100;
            if (force) {
                D_800DBEF4[index].unk4E = 0;
                return;
            }
            ((Struct131u *)D_800DBEF4)[index].unk4F &= ~0x60;
            ((Struct131u *)D_800DBEF4)[index].unk4F |= 0x20;
            D_800DBEF4[index].unk10 = obj->x_position;
            D_800DBEF4[index].unk12 = obj->y_position + (s16)obj->unk90;
            D_800DBEF4[index].unk14 = obj->z_position;
        }
    }
}

void func_1510F820(void *, void *, f32 *, f32 *, f32 *, f32 *, f32 *);
f32 func_1505210C(struct127 *arg0, f32 arg1, f32 arg2, f32 *arg3, f32 arg4);
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
            func_15052408((struct127 *)obj);
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
        func_15059444((struct127 *)obj);
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
            out3 = func_1505210C((struct127 *)obj, out3, out4, &out4, 180.0f);
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
        func_15052408((struct127 *)obj);
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

void func_1504BA38(struct127 *obj) {
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

void func_1504BAF0(struct127 *obj) {
    s16 index;
    struct127 *other;

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

void func_1504BB88(struct127 *obj) {
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

void func_1504BC38(struct127 *obj) {
    if ((obj->disable_run != 0) && (obj->disable_run < 0xFA)) {
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

typedef struct {
    u8  pad0[0x8];
    s16 unk8;
    u8  padA;
    u8  unkB;
    u8  padC[2];
    u16 unkE;
    u16 unk10;
    u8  pad12[2];
    u16 unk14;
    u8  unk16;
    u8  pad17[0x3C - 0x17];
    u8  unk3C;
} Struct1504BE2C;

void func_1507EB2C(void *arg0);
extern u16 D_800CC2B2;
extern f32 D_800CC2B4;

void func_1504BE2C(struct127 *arg0, u16 *arg1, f32 *arg2, s8 *arg3) {
    s32 pad0;
    s32 pad1;
    f32 temp_f12;
    Struct1504BE2C *sub;

    temp_f12 = ((Struct1504BE2C *)arg0->unk31C)->unkB * 0.02454369329f;
    D_800CC2B4 = 16.0f;
    sub = (Struct1504BE2C *)arg0->unk31C;
    if ((sub->unk16 != 0) && (sub->unk16 == 0)) {
        func_1507EB2C(arg0);
        ((Struct1504BE2C *)arg0->unk31C)->unk8 = 0;
        sub = (Struct1504BE2C *)arg0->unk31C;
    }
    if (sub->unkE == 0) {
        sub->unkE = 0xA6;
        sub = (Struct1504BE2C *)arg0->unk31C;
    }
    sub->unk3C = 5;
    if (arg1) {
    }
    if ((*arg2 > 4.0f) && (arg0->unk84.uh == 0xAA)) {
        ((Struct1504BE2C *)arg0->unk31C)->unk3C = 1;
        ((Struct1504BE2C *)arg0->unk31C)->unk10 = *arg1;
    }
    func_150AD78C(temp_f12);
    if (arg1) {
    }
    ((Struct1504BE2C *)arg0->unk31C)->unkB += 2;
    if (((Struct1504BE2C *)arg0->unk31C)->unk16 != 0) {
        arg0->unkAA = 0x19;
    } else if (arg0->unk84.uh == 0xA7) {
        arg0->unkAA = 0x32;
    } else {
        arg0->unkAA = 0x20;
    }
    *(f32 *)&arg0->pad54 *= 0.5f;
    if ((arg0->unk84.uh == 0x2C) || (arg0->unk84.uh == 0xE0)) {
        *arg3 = ((s8 *)D_800CC284)[3] >> 1;
    } else {
        if (!(func_150ADA20() & 0x3F)) {
            ((Struct1504BE2C *)arg0->unk31C)->unk14 = (func_150ADA20() % 25000U) - 12500;
        }
        D_800CC2B2 = ((Struct1504BE2C *)arg0->unk31C)->unk14;
        if (((Struct1504BE2C *)arg0->unk31C)->unk16 != 0) {
            D_800CC2B2 = (s16)D_800CC2B2 / 3;
        }
    }
    if ((D_800C35EA == 0) && (((Struct1504BE2C *)arg0->unk31C)->unk16 == 0)) {
        arg0->unk282 = 0x1E;
        arg0->unk276 = 0;
    }
}



s32 func_1504C078(void) {
    if (D_800D154C->unk31C->unk58 != 1) {
        D_800D154C->unk31C->unk59 = 0;
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
s32 func_15064B94(u8 *, f32, f32, s32, f32);
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
extern u16 D_800CC2B2;
extern f32 D_800CC2B4;

void func_1504C0E8(u8 *arg0, s32 arg1, f32 arg2) {
    f32 blend;
    f32 start;
    f32 vel;
    s32 pressed;
    f32 speed;
    s32 anim;
    u8 *st;

    f32 diff;

    if ((s16)D_800CC2B2 == 0 && (st = *(u8 **)(arg0 + 0x31C), *(s16 *)(st + 0x12) >= -29) && *(s16 *)(st + 0x12) < 30) {
        *(s16 *)(st + 0x12) = 0;
    } else {
        st = *(u8 **)(arg0 + 0x31C);
        diff = (s16)D_800CC2B2 - *(s16 *)(st + 0x12);
        diff = (D_800D1550[0] / D_800CC2B4) * diff;
        *(s16 *)(st + 0x12) += (s16)(s32)diff;
    }
    st = *(u8 **)(arg0 + 0x31C);
    if (*(st + 0x6B) != 0) {
        return;
    }
    anim = 999;
    if (arg0[0x65] != 0 && arg0[0x104] == 0) {
        func_15064A14(arg0);
        return;
    }
    if (arg0[0x81] == 0 && arg0[0x104] == 0 && arg0[0xAD] == 0) {
        if (arg0[0x83] == 0 && *(st + 0x11A) != 3) {
            arg0[0x80] = 1;
            arg0[0x100] &= 0xF3;
            D_800CC2BC |= 0x80;
            speed = 1.0f;
            start = 0.0f;
            blend = 4.0f;
            vel = *(f32 *)(arg0 + 0x3C) * D_800991DC + arg2 * D_800991E0;
            st = *(u8 **)(arg0 + 0x31C);
            if (*(s16 *)(st + 0x46) > 0 || *(st + 0x4E) != 0 || arg0[0x13C] >= 100 || arg1 != 0 ||
                (*(u16 *)(st + 0x32) & 3) || *(st + 0x78) != 0 || *(st + 0x1A) != 0 ||
                *(s16 *)(st + 8) > 0 || *(st + 0x17) != 0) {
                if (func_15064B94(arg0, vel, vel, arg1, arg2) != 0) {
                    return;
                }
                st = *(u8 **)(arg0 + 0x31C);
            }
            pressed = *(u16 *)D_800CC284 & 0x2000;
            if (pressed == 0 || D_800CC2B1 != 0 || *(st + 0x78) != 0 || (*(st + 0x4E) & 0xF) == 1) {
                if (*(f32 *)(arg0 + 0x3C) > 4.0f) {
                    *(st + 0x37) = 0;
                    st = *(u8 **)(arg0 + 0x31C);
                }
                pressed = *(st + 0x4E);
                if ((*(st + 0x4E) == 1 || pressed == 0x81) && vel > 1.0f) {
                    arg2 = 10.0f;
                    anim = 0x302;
                    speed = *(f32 *)(arg0 + 0x3C) / 7.0f + D_800991E4;
                    if (*(f32 *)(arg0 + 0x3C) > 23.0f) {
                        anim = 0x303;
                        speed = *(f32 *)(arg0 + 0x3C) / 23.0f + D_800991E8;
                    }
                } else if (*(s8 *)(st + 0x4A) > 0 && vel >= 5.0f) {
                    anim = 0x2B;
                    speed = vel / 12.0f;
                } else if (*(s8 *)(st + 0x4B) > 0 && vel >= 5.0f) {
                    anim = 0xDF;
                    speed = vel / 12.0f;
                } else if (*(f32 *)(arg0 + 0x118) != D_800991EC && *(f32 *)(arg0 + 0x18) < *(f32 *)(arg0 + 0x118) &&
                           *(f32 *)(arg0 + 0x3C) >= 1.0f && *(st + 0x4E) == 0) {
                    speed = vel / 18.0f + D_800991F0;
                    anim = 0x8B;
                    arg0[0xAA] = 0x4B;
                } else if (vel > 14.0f) {
                    anim = (D_800BE9F0 != 4) ? 2 : 0x1B2;
                    speed = vel / 23.0f + D_800991F4;
                } else if (vel > 6.0f || (arg2 < 1.0f && *(f32 *)(arg0 + 0x3C) > 1.0f)) {
                    anim = 1;
                    speed = vel * 0.125f + D_800991F8;
                } else if (vel > 1.0f) {
                    blend = 15.0f;
                    anim = (arg0[4] == 0x80) ? 1 : 0;
                    speed = vel * 0.125f + D_800991FC;
                } else {
                    anim = func_1504C078();
                }
                if (*(f32 *)(arg0 + 0x28) > 60.0f && *(*(u8 **)(arg0 + 0x31C) + 0x4E) == 0) {
                    speed = 0.5f;
                    anim = 0x38;
                    arg0[0x100] |= 0x10;
                }
                if (*(f32 *)(arg0 + 0x3C) > 18.0f && *(*(u8 **)(arg0 + 0x31C) + 0x11A) != 1 && *(*(u8 **)(arg0 + 0x31C) + 0x11A) != 3 && arg2 == 0.0f &&
                    *(f32 *)(arg0 + 0x28) == 0.0f && D_800CC27C == 0) {
                    speed = 0.5f;
                    anim = 0x21;
                    blend = 3.0f;
                    arg0[0x89] = 8;
                    arg0[0x83] = 0x11;
                }
            } else if (pressed != 0) {
                if (*(f32 *)(arg0 + 0x3C) < 3.0f) {
                    *(st + 0x37) = 1;
                }
                if (*(f32 *)(arg0 + 0x44) > D_80099200 && *((st = *(u8 **)(arg0 + 0x31C)) + 0x37) != 0) {
                    *(s32 *)(st + 4) = 1;
                    arg0[0xAA] = 0x28;
                    blend = 5.0f;
                    speed = *(f32 *)(arg0 + 0x44) / 15.0f + 0.5f;
                    anim = func_1504C0B8();
                } else {
                    blend = 6.0f;
                    speed = 1.5f;
                    anim = 0x16;
                }
            }
        }
        if (anim != 999) {
            func_1505E650((struct127 *)arg0, anim, speed, blend, start, 0.0f, 0);
        }
    } else if (*(u16 *)(arg0 + 0x84) == 0x21 && D_800CC2B1 == 0 && *(f32 *)(arg0 + 0x44) == 0.0f &&
               (*(u16 *)D_800CC284 & 0x2000)) {
        func_1505E650((struct127 *)arg0, 0x16, 1.5f, 6.0f, 0.0f, 0.0f, 0);
    }
}

void func_1504C854(struct127 *arg0) {
    D_800CC288 = arg0->unk31C->unk8C;

    if (arg0->unk31C->unk8E > 0) {
        arg0->unk31C->unk8E -= D_800BE9A0;
    } else {
        arg0->unk31C->unk8A = 0;
    }
    if (arg0->unk31C->unk8F > 0) {
        arg0->unk31C->unk8F -= D_800BE9A0;
    } else {
        arg0->unk31C->unk8C = 0;
    }
}

s32 func_1504C8BC(void *arg0) {
    void *temp_v1;

    temp_v1 = *(void **)((u8 *)arg0 + 0x31C);
    if (temp_v1 == 0) {
        return 1;
    }
    if (*(s32 *)((u8 *)arg0 + 0x1D4) != 0 || D_800BE9F0 == 0x33 || *(s32 *)arg0 == 0x25) {
        *((u8 *)temp_v1 + 0x1A8) = 0;
        return 1;
    }
    if (D_800CC2B8 == (*((u8 *)arg0 + 0x127) & 1)) {
        if (*((u8 *)temp_v1 + 0x1A8) != 0) {
            *(s16 *)&D_800CC2BA = (s16)D_800BE9E4;
            D_800BE9E4 += (u32)*((u8 *)*(void **)((u8 *)arg0 + 0x31C) + 0x1A8);
            D_800BE9A4 = (f32)D_800BE9E4 * 0.5f;
            if (D_800BE9A4 != 0.0f) {
                D_800BE9A8 = 1.0f / D_800BE9A4;
            } else {
                D_800BE9A8 = 0.0f;
            }
            *((u8 *)*(void **)((u8 *)arg0 + 0x31C) + 0x1A8) = 0;
            return 2;
        }
        return 1;
    }
    *((u8 *)temp_v1 + 0x1A8) += (u32)D_800BE9E4;
    return 0;
}

void func_1504C9E4(struct127 *arg0, s8 arg1, u8 arg2) {
    u8 phi_a2;
    s32 tmp0;
    s32 tmp1;

    tmp0 = (u8)(arg0->unk1D1 - arg1);

    if (tmp0) {
        phi_a2 = 3;
        if (arg2 == 16) {
            phi_a2 = 6;
        }

        tmp1 = tmp0;
        if (tmp0 >= 128) {
            arg0->unk1D1 += phi_a2;
        }
        else {
            arg0->unk1D1 -= phi_a2;
        }

        tmp0 = tmp1;
        tmp0 ^= arg0->unk1D1 - arg1;
        if ((u8)tmp0 & 0x80)
        {
            arg0->unk1D1 = arg1;
        }
    }
}
