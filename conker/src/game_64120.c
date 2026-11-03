#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_150380C0(f32 *, s32, s32, s32, f32, s32, f32 *, s32, s32, s32, s32);
extern f32 D_800C3FD0[];


extern f32 D_80098250;
extern f32 D_8009862C;
extern s16 D_800C3FF4[];
extern u16 *D_80084380[];
extern u8 D_80097E7C[];
extern u8 D_80098050[];

void func_15039A78(f32 *, f32 *, f32 *, f32 *, f32, f32, s32);

typedef struct {
    u8 pad0[0x324];
    f32 *unk324;
} GameStruct_15036C70;

void func_15036C70(GameStruct_15036C70 *arg0) {
    s32 i;
    f32 val;

    arg0->unk324 = allocate_memory(0x48, 1, 0, 0);
    bzero(arg0->unk324, 0x48);
    val = D_80098250;
    for (i = 0; i != 0xC; i += 4) {
        *(f32 *)((u8 *)arg0->unk324 + i) = val;
        *(f32 *)((u8 *)arg0->unk324 + i + 0xC) = val;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_15036CE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_15036F34.s")

void func_15038468(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);

s32 func_15037698(s32 arg0, s32 arg1, s32 arg2, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 arg8, s32 arg9) {
    f32 (*src)[4];
    f32 (*dst)[4];
    f32 delta[3];
    f32 pos[3];
    f32 rx;
    f32 ry;
    f32 rz;
    f32 yaw;
    f32 pitch;

    if (arg8 != 0) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    src = ((f32 (*)[4][4])D_800CC2D0[arg0].unk1D4)[arg1];
    if (D_800CC2D0[arg0].unk1D4 == NULL) {
        return 0;
    }
    dst = ((f32 (*)[4][4])D_800CC2D0[arg0].unk1D4)[arg2];
    src[0][3] = src[1][3] = src[2][3] = dst[0][3] = dst[1][3] = dst[2][3] = 0.0f;
    src[3][3] = dst[3][3] = 1.0f;
    pos[0] = src[1][0];
    pos[1] = src[1][1];
    pos[2] = src[1][2];
    delta[0] = x - src[3][0];
    delta[1] = y - src[3][1];
    delta[2] = z - src[3][2];
    rx = delta[0] * dst[0][0] + delta[1] * dst[0][1] + delta[2] * dst[0][2];
    ry = delta[0] * dst[1][0] + delta[1] * dst[1][1] + delta[2] * dst[1][2];
    rz = delta[0] * dst[2][0] + delta[1] * dst[2][1] + delta[2] * dst[2][2];
    func_15038468(&yaw, &pitch, rx, ry, rz, arg9);
    if (yaw < range && -range < yaw) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    return 0;
}

s32 func_15037880(s32 arg0, f32 *arg1) {
    struct127 *obj;
    s32 temp;
    f32 dx;
    f32 dz;

    obj = &D_800CC2D0[arg0];
    temp = obj->unk2F8 & 7;
    if (temp == 1) {
        arg1[0] = D_800DBFF0->unk2F8;
        arg1[1] = D_800DBFF0->unk2FC;
        arg1[2] = D_800DBFF0->unk300;
        return 1;
    }
    if (temp == 2) {
        if (obj->unk5 == 4) {
            dx = obj->x_position - (f32)D_800C3FF4[0];
            dz = obj->z_position - (f32)D_800C3FF4[2];
            if (D_8009862C < (dx * dx) + (dz * dz)) {
                return 1;
            }
        }
        arg1[0] = D_800C3FF4[0];
        arg1[1] = D_800C3FF4[1];
        arg1[2] = D_800C3FF4[2];
        return 1;
    }
    if (temp == 3) {
        return 1;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_150379DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_150380C0.s")

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15038468(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    f32 var_fv0;
    f32 factor;

    switch (arg5) {
    case 0:
        *arg0 = func_150484A0(arg2, arg4);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 1:
        *arg0 = func_150484A0(-arg4, arg2);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 2:
        *arg0 = func_150484A0(arg4, -arg2);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 3:
        *arg0 = func_150484A0(arg4, -arg3);
        var_fv0 = func_150484A0(-arg2, sqrtf((arg3 * arg3) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    }
    factor = 57.295776f;
    *arg0 *= factor;
    *arg1 *= factor;
    var_fv0 = *arg0;
    if (var_fv0 > 180.0f) {
        *arg0 = var_fv0 - 360.0f;
    }
    var_fv0 = *arg1;
    if (var_fv0 > 180.0f) {
        *arg1 = var_fv0 - 360.0f;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_15038620.s")

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} GameStruct_15039A54;

extern GameStruct_15039A54 D_80098068[];

typedef struct {
    struct108 *unk0;
    u8 pad4[0x328];
} GameStruct_800CC5E8;

extern GameStruct_800CC5E8 D_800CC5E8[];

GameStruct_15039A54 *func_15039A54(s32 arg0, s32 arg1) {
    return &D_80098068[arg1];
}

void func_15039A78(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5, s32 arg6) {
    GameStruct_15039A54 *temp_v0;
    struct108 *temp_v1;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f12_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f12;
    f32 var_f12_2;

    temp_v1 = D_800CC5E8[arg6].unk0;
    temp_v0 = func_15039A54(arg6, 0);
    if ((temp_v1 != NULL) && (temp_v1->unk2C != 0x100)) {
        temp_f12 = temp_v0->unk10;
        temp_f0 = *arg0;
        if (((temp_v0->unk0 + temp_f12) < temp_f0) || (temp_f2 = temp_v0->unk4, (temp_f0 < (temp_f2 - temp_f12)))) {
            *arg0 = arg4;
            *arg1 = arg5;
            if (arg2 != NULL) {
                *arg2 = *arg0;
                *arg3 = *arg1;
            }
            return;
        }

        if (arg2 != NULL) {
            *arg2 = temp_f0;
            *arg3 = *arg1;
            temp_f2 = temp_v0->unk4;
            temp_f0 = *arg0;
        }

        if (temp_f0 < temp_f2) {
            *arg0 = temp_f2;
        } else {
            temp_f2_2 = temp_v0->unk0;
            if (temp_f2_2 < temp_f0) {
                var_f12 = temp_f2_2;
            } else {
                var_f12 = temp_f0;
            }
            *arg0 = var_f12;
        }

        temp_f14 = temp_v0->unk8;
        temp_f12 = temp_v0->unk14;
        temp_f2_3 = *arg1;
        if (((temp_f14 + temp_f12) < temp_f2_3) || (temp_f0_2 = temp_v0->unkC, (temp_f2_3 < (temp_f0_2 - temp_f12)))) {
            *arg1 = arg5;
            return;
        }

        if (temp_f2_3 < temp_f0_2) {
            *arg1 = temp_f0_2;
        } else {
            if (temp_f14 < temp_f2_3) {
                temp_f12 = temp_f14;
            } else {
                temp_f12 = temp_f2_3;
            }
            *arg1 = temp_f12;
        }
        return;
    }

    temp_f0 = *arg0;
    temp_f2 = temp_v0->unk4;
    if (temp_f0 < temp_f2) {
        *arg0 = temp_f2;
    } else {
        temp_f2_2 = temp_v0->unk0;
        if (temp_f2_2 < temp_f0) {
            var_f12 = temp_f2_2;
        } else {
            var_f12 = temp_f0;
        }
        *arg0 = var_f12;
    }

    temp_f2_3 = *arg1;
    temp_f0_2 = temp_v0->unkC;
    if (temp_f2_3 < temp_f0_2) {
        *arg1 = temp_f0_2;
    } else {
        temp_f14 = temp_v0->unk8;
        if (temp_f14 < temp_f2_3) {
            var_f12 = temp_f14;
        } else {
            var_f12 = temp_f2_3;
        }
        *arg1 = var_f12;
    }
}

void func_15039CC8(struct127 *arg0) {
    s32 id;
    s32 i;
    s32 idx;
    s32 count;
    s32 value;
    u16 *table;
    u8 *ptr;
    u8 *write_ptr;

    ptr = *(u8 **)((u8 *)arg0 + 0x324);
    if (ptr != NULL) {
        id = arg0->id;
        if (((id == 0x53) && (arg0->unk84.uh == 0x15)) || (D_800BE9B4 != 0)) {
            for (value = 0x28, i = 0, write_ptr = ptr; i != 3;) {
                i++;
                write_ptr++;
                write_ptr[0x2F] = value;
                write_ptr[0x32] = 0;
                write_ptr[0x35] = 4;
            }
        }

        idx = D_80097E7C[id];
        if (idx != 0) {
            idx--;
            table = D_80084380[idx];
            count = D_80098050[idx];
            for (i = 0; i < count; i++) {
                if (arg0->unk84.uh == table[i]) {
                    arg0->unk2FB |= 4;
                    return;
                }
            }

            if (arg0->interaction_state == 1) {
                if ((arg0->health == 0) ||
                        ((arg0->in_water != 0) && (arg0->xz_velocity < 5.0f)) ||
                        (((u8 *)arg0->unk31C)[0x197] != 0) ||
                        (arg0->xz_velocity > 20.0f)) {
                    arg0->unk2FB |= 4;
                }
            } else if (arg0->health == 0) {
                arg0->unk2FB |= 4;
            }
        }
    }
}

void func_15039ED0(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, f32 arg5, f32 arg6, s32 arg7) {
    GameStruct_15039A54 *temp_v0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 var_f2;
    f32 var_f2_2;

    if (arg0 == 0) {
        func_15039A78(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        return;
    }

    temp_v0 = func_15039A54(arg7, arg0);
    temp_f12 = temp_v0->unk10;
    temp_f0 = *arg1;
    if (((temp_v0->unk0 + temp_f12) < temp_f0) || (var_f2 = temp_v0->unk4, (temp_f0 < (var_f2 - temp_f12)))) {
        *arg1 = arg5;
        *arg2 = arg6;
        if (arg3 != NULL) {
            *arg3 = *arg1;
            *arg4 = *arg2;
        }
        return;
    }

    if (arg3 != NULL) {
        *arg3 = temp_f0;
        *arg4 = *arg2;
        var_f2 = temp_v0->unk4;
        temp_f0 = *arg1;
    }

    if (temp_f0 < var_f2) {
        *arg1 = var_f2;
    } else {
        temp_f12_2 = temp_v0->unk0;
        if (temp_f12_2 < temp_f0) {
            var_f2 = temp_f12_2;
        } else {
            var_f2 = temp_f0;
        }
        *arg1 = var_f2;
    }

    temp_f12_3 = temp_v0->unk8;
    temp_f14 = temp_v0->unk14;
    temp_f0_2 = *arg2;
    if (((temp_f12_3 + temp_f14) < temp_f0_2) || (var_f2_2 = temp_v0->unkC, (temp_f0_2 < (var_f2_2 - temp_f14)))) {
        *arg2 = arg6;
        return;
    }

    if (temp_f0_2 < var_f2_2) {
        *arg2 = var_f2_2;
    } else {
        if (temp_f12_3 < temp_f0_2) {
            var_f2_2 = temp_f12_3;
        } else {
            var_f2_2 = temp_f0_2;
        }
        *arg2 = var_f2_2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_1503A08C.s")

typedef struct {
    u8  pad0[0x30];
    f32 unk30;
    f32 unk34;
    f32 unk38;
} GameStruct_1503A60C;

void func_1503A60C(void) {
    GameStruct_1503A60C *dst = (GameStruct_1503A60C *)((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + 0x40);

    dst->unk30 = *(f32 *)&D_800CC2D0[D_800C3E78].pad174;
    dst->unk34 = D_800CC2D0[D_800C3E78].y_position;
    dst->unk38 = *(f32 *)&D_800CC2D0[D_800C3E78].pad178;
}

extern u8 D_800C3FFA;
extern u8 D_80084404[];
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

void func_1503A678(void) {
    struct127 *obj;
    f32 (*m)[4];
    f32 sz;
    f32 sy;
    f32 sx;
    f32 f;
    f32 inv;
    f32 mtx[4][4];
    f32 tmp[4][4];
    f32 pos[3];

    obj = &D_800CC2D0[D_800C3E78];
    if (obj->id == 0x22) {
        f = *((u8 *)&D_800CC2D0[D_800C3E78] + 0x20A);
        f /= 64.0f;
        f += 1.0f;
        if (D_800C3FFA == 0) {
            sx = f;
            sy = 1.0f;
            sz = f;
        } else {
            sx = 1.0f / f;
            sy = 1.0f;
            sz = 1.0f / f;
        }
        m = (f32 (*)[4])((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + (D_80084404[D_800C3FFA] << 6));
        m[3][3] = 1.0f;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        pos[0] = m[3][0];
        pos[1] = m[3][1];
        pos[2] = m[3][2];
        guMtxIdentF(mtx);
        func_150A7CB0(mtx, *(s32 *)&sx, *(s32 *)&sy, *(s32 *)&sz);
        func_150A7A48(m, mtx, tmp);
        bcopy(tmp, m, 0x40);
        m[3][0] = pos[0];
        m[3][1] = pos[1];
        m[3][2] = pos[2];
        D_800C3FFA++;
    }
}

extern u8 D_800C3FFA;
extern void func_15036F34(void);
extern void func_1503A678(void);

void func_1503A7F0(void) {
    s32 temp = D_800C3FFA;
    D_800C3FFA = 0;
    func_15036F34();
    D_800C3FFA = temp;
    func_1503A678();
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_1503A830.s")

void func_1503B708(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4,
                   s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    f32 *scratch;
    f32 value;

    scratch = D_800C3FD0;
    value = -99999.0f;
    *(f32 *)((u8 *)arg0 + 0) = value;
    *(f32 *)((u8 *)arg0 + 4) = value;
    *(f32 *)((u8 *)arg0 + 8) = value;
    scratch[0] = value;
    scratch[1] = value;
    scratch[2] = value;
    if (func_15037880(arg3, arg0) == 0) {
        func_150380C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                      arg8, arg9, arg10);
    }
}
