#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    char pad_0[0x28];
    u8 field_0x28;
    u8 field_0x29;
    u8 field_0x2A;
    char pad_0x2B[0x2D];
    s32 field_0x58;
} GameObjectColorFlags;

void func_150D7420(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 2:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

void func_150D74DC(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 &= ~2;
                return;
            case 2:
                arg0->field_0x58 |= 2;
                arg0->field_0x28 = 0xFF;
                arg0->field_0x29 = 0;
                arg0->field_0x2A = 0;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

void func_150D758C(GameObjectColorFlags *arg0, u8 *arg1, u8 arg2) {
    if (arg2 == 0x36) {
        switch (*arg1) {
            case 0:
                break;
            case 1:
                arg0->field_0x58 &= ~2;
                return;
            case 2:
                arg0->field_0x58 &= ~2;
                return;
            case 3:
                arg0->field_0x28 = 0;
                arg0->field_0x58 |= 2;
                arg0->field_0x29 = 0xFF;
                arg0->field_0x2A = 0;
                return;
        }
        arg0->field_0x58 &= ~2;
    }
}

extern u16 D_800D9910;

void func_150D7630(s32 arg0) {
    if (arg0 == 0) {
        D_800D9910 += D_800BE9E4 << 6;
    }
}

typedef struct {
    s32 *t[2];
} TexTables_150D765C;
extern TexTables_150D765C D_80088900; /* = { D_80090230, D_80090244 } */
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

Gfx *func_150D765C(Gfx *gdl) {
    s32 i;
    s32 seg;
    s32 frame;
    s32 out;
    s32 set;
    s32 idx;
    s32 addr;
    TexTables_150D765C tables = D_80088900;

    set = 0;
    frame = D_800D9910 >> 8;
    for (i = 0; i < 6; i++) {
        if (i >= 4) {
            set = 1;
        }
        idx = (frame + i * 13) % 5;
        addr = func_1510D0EC(tables.t[set][idx], &out, 3, 0);
        seg = i + 2;
        gSPSegment(gdl++, seg, addr);
    }
    return gdl;
}

s32 func_1508855C(s32);
void func_150885EC(s32, s32);
s32 func_1508868C(u8 *);

void func_150D7790(u8 *arg0, s32 arg1) {
    s32 new_id;
    s32 old_id;
    s32 variant;
    u8 *created;

    variant = 0;
    if ((*(u8 **)(arg0 + 0x31C)) != 0 && *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) <= 0) {
        return;
    }
    switch (arg1) {
    case 0x30:
    case 0x31:
        if ((*(u8 **)(arg0 + 0x31C)) != 0) {
            *(s16 *)((*(u8 **)(arg0 + 0x31C)) + 0x12C) = 0x78;
            *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) -= 1;
            func_10010154(0x628, arg0, 0x7FFF, 0x1F4, 0x3E8);
        }
        return;
    case 0x33:
        variant = 1;
    case 0x32:
        break;
    default:
        return;
    }
    created = func_1506C460(*(f32 *)(arg0 + 0x40), 310.0f, 0.0f, 0.0f,
                             ((variant * 0x1E) + 0x1E), 0xD, 0.0f, 1.0f, 0, 1, variant == 0);
    if (created != 0) {
        *(s8 *)(created + 0x124) = D_800C3E78 + 1;
    }
    if (created != 0 && ((variant * 0x1E) + 0x1E) != 0x1E) {
        *(s8 *)(created + 0x221) = -1;
        old_id = func_1508855C((s32)arg0);
        if (old_id != -1) {
            new_id = func_1508868C(created);
            if (new_id != -1) {
                func_150885EC(old_id, new_id);
            }
        }
    }
    if ((*(u8 **)(arg0 + 0x31C)) != 0) {
        *(s8 *)((*(u8 **)(arg0 + 0x31C)) + 0x1AE) -= 1;
    }
}

extern f32 D_800A0AF0;
extern f32 D_800A0AF4;
extern f32 D_800A0AF8;
extern f32 D_800A0AFC;
extern f32 D_800A0B00;
extern f32 D_800A0B04;
extern f32 D_800A0B08;
extern s16 D_800CC39C;
extern s8 D_800CC4B4;
extern u8 *func_15033E84(struct127 *arg0);
extern s32 func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
extern void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
extern void func_1505A3A8(f32 arg0, struct127 *arg1, f32 arg2, f32 arg3, u8 arg4);
extern void func_1505C7D8(struct127 *arg0, u8 arg1);
extern s32 func_150535F4(struct127 *arg0);
extern void func_1001091C(struct127 *arg0, s32 arg1);
extern void func_10010FFC(s32 a, s32 b, s32 c, s32 d, s32 e, struct127 *f);
#define BY(p, o) (*(u8 *)((u8 *)(p) + (o)))

void func_150D7928(struct127 *arg0) {
    f32 sp84;
    u8 sp83;
    f32 sp7C;
    struct127 *temp_a2;
    s32 sp74;
    s16 *var_v0;
    s16 *var_v1;
    s32 var_a0;
    s32 var_v0_3;
    f32 pad2;
    u8 temp_v0;
    u16 temp_v0_5;
    f32 sp58;
    s32 var_t0;
    f32 var_f0;

    sp83 = arg0->unk124;
    sp7C = arg0->xz_velocity;
    temp_v0 = arg0->id;
    sp74 = -1;
    if (temp_v0 == 0x48) {
        temp_a2 = (struct127 *)func_15033E84(arg0);
        if (temp_a2 != NULL) {
            sp74 = temp_a2->unk6;
        }
    }
    arg0->unkB0 = 0x19;
    if (arg0->unkE4 != 0) {
        arg0->unk90 = arg0->unkE4;
    }
    if (arg0->unk13C != 0) {
        temp_a2 = &D_800CC2D0[sp83];
        if (BY(temp_a2->unk31C, 0x4E) != 0) {
            var_t0 = temp_a2->unk7A;
            arg0->unkE4 = 0;
            arg0->unk180 = temp_a2->unk180;
                    arg0->unk184 = temp_a2->unk184;
            var_v0 = (s16 *)&arg0->unk18C;
            var_v1 = (s16 *)&temp_a2->unk18C;
            for (var_a0 = 0; var_a0 < 9; var_a0++) {
                var_v0[var_a0] = var_v1[var_a0];
            }
            arg0->gravity = 6.0f;
            sp84 = *(f32 *)&temp_a2->pad54 * D_800A0AF0;
            arg0->unk44 = temp_a2->unkC4;
            if (temp_v0 == 0x7F) {
                temp_a2->immune = 0x14;
                temp_a2->unk184 &= ~0x1F;
                temp_a2->unk184 |= 4;
                arg0->y_position = temp_a2->y_position;
                arg0->x_position = arg0->x_position + ((temp_a2->x_position - arg0->x_position) * 0.5f);
                arg0->z_position = arg0->z_position + ((temp_a2->z_position - arg0->z_position) * 0.5f);
                temp_a2->disable_jump = 0xA;
                if (BY(temp_a2->unk31C, 0x4F) == 2) {
                    if (sp7C < temp_a2->unk44 * D_800A0AF4) {
                        sp7C = temp_a2->unk44 * D_800A0AF4;
                    }
                    temp_a2->unkAA = 0x46;
                }
            } else {
                arg0->y_velocity = 0.0f;
            }
            temp_v0_5 = arg0->unk21C;
            arg0->unk21C = temp_v0_5 + ((s16)(var_t0 - temp_v0_5) / 3);
            arg0->unk7A = temp_v0_5;
            arg0->unk76 = temp_v0_5;
            arg0->xz_velocity = temp_a2->xz_velocity;
            if (arg0->unkD0 != 0) {
                func_1505C7D8(arg0, D_800C3E78);
            }
            goto block_36;
        }
    }
    arg0->unk21C = arg0->unk7A;
    if (arg0->unk13C != 0) {
        if (D_800CC3D4[sp83 * 0x32C] != 0) {
            arg0->target_speed = 15.0f;
            arg0->y_velocity = 80.0f;
        }
        arg0->unk13C = 0;
    }
    sp83 = D_800C3E78;
    arg0->unk65 = 0;
    arg0->unkE4 = *(s16 *)&arg0->unk90;
    func_1505A3A8(0.0f, arg0, 0.0f, 0.5f, 1);
    sp58 = arg0->xz_velocity;
    arg0->stunned = 0x1E;
    func_15059140(arg0);
    arg0->xz_velocity = sp58;
    sp84 = (arg0->unkC4 - arg0->unk44) * D_800A0AF8;
    if (arg0->unk232 == 4) {
        if (temp_v0 == 0x48) {
            D_800CC4B4 = 1;
        }
        if (func_15052F9C(arg0, 50.0f, 0x17, 4, 0, 5, 0, 0, 0, 0) != 0) {
            return;
        }
    } else {
        for (var_t0 = 0; var_t0 != 2; var_t0++) {
            if (D_800CC2D0[var_t0].interaction_state == 1) {
                if ((BY(D_800CC2D0[var_t0].unk31C, 0x78) == 0) && (BY(D_800CC2D0[var_t0].unk31C, 0x11A) == 0) && ((1 << var_t0) & D_800CC268) && (D_800CC2D0[var_t0].y_velocity < 0.0f)) {
                    arg0->unk124 = var_t0;
                    var_f0 = 39.0f;
                    if (temp_v0 == 0x48) {
                        D_800CC2D0[var_t0].stunned = 0;
                        var_f0 = 0.0f;
                    }
                    func_1505327C(arg0, var_f0, 3.0f, 4, -1);
                }
            }
        }
    }
block_36:
    arg0->immune = 0xFF;
    arg0->unk28 = arg0->y_position - arg0->unk180;
    if (temp_v0 == 0x48) {
        if (sp74 == 0x7A) {
            f32 temp_f0_3;
            temp_f0_3 = arg0->unk180 + 180.0f;
            arg0->unkF8 |= 0x10000;
            func_15058EA4(arg0, temp_f0_3 + 10.0f, 1.2f, temp_f0_3, D_800A0AFC, 4.0f, -4.0f);
            if (arg0->unk13C != 0) {
                *(s16 *)((u8 *)&D_800CC39C + arg0->unk124 * 0x32C) = 0x1E;
            }
            arg0->unkC4 = -arg0->y_velocity;
            arg0->unk40 = (f32)arg0->unk7A * 0.005493164f;
        }
    } else {
        s32 var_a2;
        s32 pad4;
        func_1506B100(0, 2.0f, 4.0f);
        if (arg0->unk28 > 75.0f) {
            var_v0_3 = arg0->unk102;
            if (var_v0_3 < 0x1E) {
                var_v0_3 = ++arg0->unk102;
            }
        } else {
            var_v0_3 = arg0->unk102;
            if (var_v0_3 < 0xA) {
                var_v0_3 = arg0->unk102 = 0;
            }
        }
        if ((var_v0_3 >= 0xB) && ((temp_a2 = &D_800CC2D0[sp83], (temp_a2->unk28 == 0.0f)) || (arg0->in_water != 0) || ((arg0->unk124 != 0) && (BY(temp_a2->unk31C, 0x4F) == 2)))) {
            if (arg0->unk13C != 0) {
                D_800CC2D0[arg0->unk124].unkCC = 0;
                BY(D_800CC2D0[arg0->unk124].unk31C, 0x4E) = 0;
                D_800CC2D0[arg0->unk124].in_water = 0;
                D_800CC2D0[arg0->unk124].gravity = 4.0f;
            }
            func_100109D0(arg0);
            func_10010FFC(0, 0x51, 0x6D60, 0, 0, arg0);
            arg0->health = 0;
        }
        if (arg0->y_scale < 1.0f) {
            arg0->y_scale = arg0->y_scale + D_800A0B00;
        }
        if (arg0->unk28 == 0.0f) {
            arg0->target_speed = 0.0f;
            *(f32 *)&arg0->padBC = *(f32 *)&arg0->padBC * 0.5f;
        }
        if (arg0->target_speed == 0.0f) {
            arg0->unkC4 = arg0->unk44 + sp84;
        } else {
            arg0->unkC4 += arg0->target_speed;
            if (arg0->unkC4 < 0.0f) {
                arg0->unkC4 = arg0->unkC4 + 360.0f;
            } else if (arg0->unkC4 > 360.0f) {
                arg0->unkC4 = arg0->unkC4 - 360.0f;
            }
        }
        if (arg0->unk13C != 0) {
            sp58 = (f32)(s16)(s32)(fabsf(arg0->unkC4 * D_800A0B04) + 44.0f);
            *(f32 *)&arg0->padC8 = sp58;
            D_800CC2D0[arg0->unk124].unkCC = (s16)(s32)((f32)(s16)(s32)(sp58 + 44) * arg0->y_scale);
        } else {
            *(f32 *)&arg0->padC8 = 44.0f;
        }
        if ((D_800A0B08 < sp7C) && (arg0->unk28 < 10.0f) && (arg0->health != 0) && (D_800C35EA != 1)) {
            var_a2 = (s32)(1000.0f * sp7C);
            if (var_a2 >= 0x4651) {
                var_a2 = 0x4650;
            }
            if (func_100107F8(arg0) == 0) {
                func_10010154(0x678, arg0, var_a2, 0x3E8, 0x7D0);
            } else {
                func_1001091C(arg0, var_a2);
            }
        } else {
            func_100109D0(arg0);
        }
        func_15052490(arg0, arg0->unk76, sp7C, 0.55f);
    }
    func_150535F4(arg0);
}

